// CaliPaneDynamic.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "CaliPaneDynamic.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneDynamic dialog
//-------------------------------------------------------------------------------------//
CCaliPaneDynamic::CCaliPaneDynamic(CWnd* pParent /*=NULL*/)
	: CDialog(CCaliPaneDynamic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaliPaneDynamic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_CameraID = PRIMARY_CAMERA_ID;		

	m_BKColor = 0x000000;
	m_ImageZoom = 1.0;
	::memset(&m_ImageWndRect, 0x00, sizeof(m_ImageWndRect));		
	
	m_LastAxisIndex = AXIS_X;//上一次設定的軸
	m_LastAxisOffset = 40000;//上一次設定的偏移量

	m_ImageW = 1024;
	m_ImageH = 1024;
	m_ImageStep = 1024;
	m_BitCount = 8;

	m_ShowBuffer = NULL;
	m_ShowBuffer1 = NULL;	
	m_MaskBuffer = NULL;
	m_PhaseBuffer = NULL;
	m_PhaseBuffer1 = NULL;
	m_PhaseBuffer2 = NULL;
	m_ImageBuffer = NULL;
	m_ImageBuffer1 = NULL;
	m_ImageBuffer2 = NULL;
	m_SpaceBuffer = NULL;
	m_SpaceBuffer1 = NULL;
	m_ShowBufferSize = 0;
	m_ImageBufferSize = 0;
	m_SpaceBufferSize = 0;

	m_PatternX = 0;
	m_PatternY = 0;
	m_PatternW = 100;
	m_PatternH = 100;
	m_PatternStep = 100;
	m_PatternBitCnt = 8;
	m_PatternPtr = NULL;

	m_DynamicTuneIndex = 0;
	m_StopResultListBeSelected = false;
	m_CalibrationMode = CALIBRATION_DYNAMIC_STOP;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaliPaneDynamic)	
	DDX_Control(pDX, CALIDYNAMIC_RESULT_LIST_WND, m_ResultListWnd);
	DDX_Control(pDX, CALIDYNAMIC_SLICE_COMBO, m_SliceCombox);
	DDX_Control(pDX, CALIDYNAMIC_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCaliPaneDynamic, CDialog)
	//{{AFX_MSG_MAP(CCaliPaneDynamic)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(CALIDYNAMIC_POS_GO_BTN_A, OnPosGoBtnA)	
	ON_BN_CLICKED(CALIDYNAMIC_POS_GO_BTN_B, OnPosGoBtnB)	
	ON_BN_CLICKED(CALIDYNAMIC_POS_GO_BTN_C, OnPosGoBtnC)	
	ON_BN_CLICKED(CALIDYNAMIC_BUILD_PATTERN_BTN, OnBuildPatternBtn)
	ON_BN_CLICKED(CALIDYNAMIC_TEST_BTN_AA, OnTestBtnAA)
	ON_BN_CLICKED(CALIDYNAMIC_TEST_BTN_AB, OnTestBtnAB)
	ON_BN_CLICKED(CALIDYNAMIC_TEST_BTN_AC, OnTestBtnAC)
	ON_BN_CLICKED(CALIDYNAMIC_GRAB_BTN, OnGrabBtn)
	ON_CBN_SELCHANGE(CALIDYNAMIC_SLICE_COMBO, OnSelchangeSliceCombo)
	ON_NOTIFY(LVN_ITEMCHANGED, CALIDYNAMIC_RESULT_LIST_WND, OnItemchangedResultListWnd)
	ON_BN_CLICKED(CALIDYNAMIC_POS_GET_BTN_A, OnPosGetBtnA)
	ON_BN_CLICKED(CALIDYNAMIC_POS_GET_BTN_B, OnPosGetBtnB)
	ON_BN_CLICKED(CALIDYNAMIC_POS_GET_BTN_C, OnPosGetBtnC)	
	ON_BN_CLICKED(CALIDYNAMIC_MOTION_WND_BTN, OnMotionWndBtn)
	ON_BN_CLICKED(CALIDYNAMIC_JOG_CHK, OnJogChk)
	ON_BN_CLICKED(CALIDYNAMIC_SET_BC_POS_BTN, OnSetBCPosBtn)
	ON_BN_CLICKED(CALIDYNAMIC_AUTO_TUNE_BTN, OnAutoTuneBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneDynamic message handlers
//-------------------------------------------------------------------------------------//
BOOL CCaliPaneDynamic::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here		
	m_ImageWnd.SetShowLBtnPos(true);	
	m_ImageWnd.SetShowEditCenterLine(true);	
	m_ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);	
	m_ImageWnd.SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_STAGE_MOVE_TO);
	JetAPI::InitialListCtrl(m_ResultListWnd);
	m_MotionCtrlWnd.Create(IDD_MOTION_CTRL_WND, this);

	SwitchMultiLanguage();
	BuildResultListWndHeader();
	AOIDataDefine.BuildSystemSliceParamCombox(m_SliceCombox, false, false, false);
	JetAPI::SetComboxCurSel(m_SliceCombox, SLICE_UNIQUE_ID_DEFAULT);

	
	CWnd::SetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_A, 0);
	CWnd::SetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_A, 0);
	CWnd::SetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_B, 0);
	CWnd::SetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_B, 0);
	CWnd::SetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_C, 0);
	CWnd::SetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_C, 0);

	CWnd::SetDlgItemInt(CALIDYNAMIC_DELAY_TIME_EDIT, 0);
	CWnd::SetDlgItemInt(CALIDYNAMIC_IMAGE_COUNT_EDIT, 30);

	if ( MotionCtrlPtr->GetIsJogMode() == true ) 
	{	CWnd::CheckDlgButton(CALIDYNAMIC_JOG_CHK, TRUE); }
	else
	{	CWnd::CheckDlgButton(CALIDYNAMIC_JOG_CHK, FALSE); }

	CAMERA_ID CameraID = GetCameraID();
	m_ImageResolution.x = AOIDataCollect.GetCameraResolutionX(CameraID);
	m_ImageResolution.y = AOIDataCollect.GetCameraResolutionY(CameraID);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	this->m_ShowBuffer = NULL;
	this->m_ShowBuffer1 = NULL;
	this->m_MaskBuffer = NULL;
	this->m_SpaceBuffer = NULL;
	this->m_SpaceBuffer1 = NULL;
	this->m_PhaseBuffer = NULL;
	this->m_PhaseBuffer1 = NULL;
	this->m_PhaseBuffer2 = NULL;
	this->m_ImageBuffer = NULL;
	this->m_ImageBuffer1 = NULL;
	this->m_ImageBuffer2 = NULL;
	this->m_ShowBufferSize = 0;
	this->m_ImageBufferSize = 0;
	this->m_SpaceBufferSize = 0;

	ReleasePatternImage();
	ReleaseImageBufferList();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( GetSafeHwnd() == NULL )
	{	return; }	

	if ( m_ResultListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ResultListWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.bottom = cy-4;
		m_ResultListWnd.MoveWindow(&WndRect);
	}

	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx-4;
		WndRect.bottom = cy-4;
		m_ImageWnd.MoveWindow(&WndRect);
	}
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{			
		AOIDataCollect.SetCallbackWnd(GetSafeHwnd());	
		FocusToEditCtrl();
		if ( MotionCtrlPtr->GetIsJogMode() == true ) 
		{	CWnd::CheckDlgButton(CALIDYNAMIC_JOG_CHK, TRUE); }
		else
		{	CWnd::CheckDlgButton(CALIDYNAMIC_JOG_CHK, FALSE); }
		ExecStopGrab();
	}
	else
	{	MotionCtrlPtr->SetStopXYCalibration(true); }
}
//-------------------------------------------------------------------------------------//
BOOL CCaliPaneDynamic::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( MotionCtrlPtr->ExecJogMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }
	//if ( ExecMouseWheelMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	//{	return TRUE; }
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CCaliPaneDynamic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_CAMERA_CALLBACK:		
		switch ( wParam )
		{
		case WPARAM_CAMERA_1_CALLBACK:			
		case WPARAM_CAMERA_2_CALLBACK:			
		case WPARAM_CAMERA_3_CALLBACK:			
		case WPARAM_CAMERA_4_CALLBACK:			
		case WPARAM_CAMERA_5_CALLBACK:
			if ( this->RetrieveCameraImage(wParam, lParam, true) == false )
			{	this->LockUIWnd(false);	}
			break;
		}
		break;		
	case MSG_CAMERA_REGRAB_IMAGE:
		if ( GetCalibrationMode() == CALIBRATION_DYNAMIC_STOP )
		{	ExecStopGrab(); }
		break;	
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		//OnImageWndNotify(wParam, lParam);		
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::CreateTempFolder()//建立暫存資料夾
{
	CString str;
	CString Folder = AOIDataCollect.GetAOITempDirectory();
	if ( JetAPI::CreateFolder(Folder) == false )
	{
		str.Format(_T("Error, Create Temp Folder Fault [%s]"), Folder);
		JetAPI::ShowMessageBox(str);		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_CALIBRATION_PANE_DYNAMIC");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_CALIBRATION_PANE_DYNAMIC;
	WndKey = _T("IDD_CALIBRATION_PANE_DYNAMIC");
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
	WndID = CALIDYNAMIC_POSITION_GROUP;
	WndKey = _T("CALIDYNAMIC_POSITION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIDYNAMIC_POS_X_LABEL;
	WndKey = _T("CALIDYNAMIC_POS_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIDYNAMIC_POS_Y_LABEL;
	WndKey = _T("CALIDYNAMIC_POS_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_POS_LABEL_A;
	WndKey = _T("CALIDYNAMIC_POS_LABEL_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_POS_GO_BTN_A;
	WndKey = _T("CALIDYNAMIC_POS_GO_BTN_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_POS_GET_BTN_A;
	WndKey = _T("CALIDYNAMIC_POS_GET_BTN_A");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_POS_LABEL_B;
	WndKey = _T("CALIDYNAMIC_POS_LABEL_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_POS_GO_BTN_B;
	WndKey = _T("CALIDYNAMIC_POS_GO_BTN_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = CALIDYNAMIC_POS_GET_BTN_B;
	WndKey = _T("CALIDYNAMIC_POS_GET_BTN_B");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_POS_LABEL_C;
	WndKey = _T("CALIDYNAMIC_POS_LABEL_C");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_POS_GO_BTN_C;
	WndKey = _T("CALIDYNAMIC_POS_GO_BTN_C");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = CALIDYNAMIC_POS_GET_BTN_C;
	WndKey = _T("CALIDYNAMIC_POS_GET_BTN_C");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_DELAY_TIME_LABEL;
	WndKey = _T("CALIDYNAMIC_DELAY_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_SLICE_LABEL;
	WndKey = _T("CALIDYNAMIC_SLICE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = CALIDYNAMIC_IMAGE_COUNT_LABEL;
	WndKey = _T("CALIDYNAMIC_IMAGE_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = CALIDYNAMIC_JOG_CHK;
	WndKey = _T("CALIDYNAMIC_JOG_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_MOTION_WND_BTN;
	WndKey = _T("CALIDYNAMIC_MOTION_WND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_SET_BC_POS_BTN;
	WndKey = _T("CALIDYNAMIC_SET_BC_POS_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_GRAB_BTN;
	WndKey = _T("CALIDYNAMIC_GRAB_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_BUILD_PATTERN_BTN;
	WndKey = _T("CALIDYNAMIC_BUILD_PATTERN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_TEST_BTN_AA;
	WndKey = _T("CALIDYNAMIC_TEST_BTN_AA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_TEST_BTN_AB;
	WndKey = _T("CALIDYNAMIC_TEST_BTN_AB");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIDYNAMIC_TEST_BTN_AC;
	WndKey = _T("CALIDYNAMIC_TEST_BTN_AC");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = CALIDYNAMIC_AUTO_TUNE_BTN;
	WndKey = _T("CALIDYNAMIC_AUTO_TUNE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CCaliPaneDynamic::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_CALIBRATION_PANE_DYNAMIC");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::SetSpaceBuffer(size_t Size, SPACE_PTR Buffer, SPACE_PTR Buffer1)
{
	m_SpaceBuffer = Buffer;
	m_SpaceBuffer1 = Buffer1;
	m_SpaceBufferSize = Size;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::SetShowBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1)
{
	m_ShowBuffer = Buffer;
	m_ShowBuffer1 = Buffer1;
	m_ShowBufferSize = Size;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::SetImageBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1, IMAGE_PTR Buffer2, PHASE_PTR PhaseBuffer, PHASE_PTR PhaseBuffer1, PHASE_PTR PhaseBuffer2, MASK_PTR MaskBuffer)
{
	m_MaskBuffer = MaskBuffer;
	m_ImageBuffer = Buffer;
	m_ImageBuffer1 = Buffer1;
	m_ImageBuffer2 = Buffer2;	
	m_PhaseBuffer = PhaseBuffer;
	m_PhaseBuffer1 = PhaseBuffer1;
	m_PhaseBuffer2 = PhaseBuffer2;
	m_ImageBufferSize = Size;		
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像	
{
	CString str;		
	CAMERA_ID CameraID = CCameraCtrl::GetCaemraIDFromWParam(wParam);
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{
		str.Format(_T("Image Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));
		this->AddLogListBox(str);
		return true;	
	}
	if ( NULL==m_ShowBuffer || NULL==m_ImageBuffer || NULL==m_ImageBuffer1 || NULL==m_ImageBuffer2) 
	{ 
		return false; 
	}	
	CALIBRATION_DYNAMIC_MODE CalibrationMode = GetCalibrationMode();
	if ( CALIBRATION_DYNAMIC_TEST == CalibrationMode )
	{	JetAPI::SetFuncTimeEnd(m_TestEnd);	}
	if ( CALIBRATION_DYNAMIC_TUNE == CalibrationMode )
	{	JetAPI::SetFuncTimeEnd(m_TestEnd);	}

	str.Format(_T("Image Period Callback#%d"), CameraCtrl.GetImageCallbackCount(CameraID));
	this->AddLogListBox(str);

	CameraCtrl.SetCameraToSendCallback(CameraID, FALSE);
	if ( CameraCtrl.GetCameraImage3(CameraID, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer) == false )
	{				
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false); 
		return false;
	}

	const double ImageOffset = 0.0;
	const double ImageGain = m_SliceParam.SliceGainValue;
	if ( ImageAPI.ImageOffsetGain3(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, m_ImageBuffer, ImageOffset, ImageGain) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false); 
		return false;
	}
	CameraCtrl.KeepCameraTempRingBuffer(CameraID);
	CameraCtrl.IncrementCameraCopyToHostCount(CameraID);
	CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
	CameraCtrl.FreeCameraTempRingBuffer(CameraID);	
	//CameraCtrl.ResetCameraRingBuffer(CameraID);	
	if ( CameraCtrl.CheckNeedGrabNext3DImage() == true )//2次打光
	{
		bool bFinish = false;
		if ( CameraCtrl.BatchGrabNext3DImage(bFinish) == false )
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
			return false; 
		}
		return true;
	}
	if ( CALIBRATION_DYNAMIC_TEST == CalibrationMode )
	{	
		CloneImageBufferList();
		AnalysizeDynamicOffset();
		BuildResultListWnd();
		SetCalibrationMode(CALIBRATION_DYNAMIC_STOP);
	}		

	bool       bResetView = true;	
	TREGION4D  StageRgn;
	AOIDataCollect.GetFovStageRegionReal(StageRgn);	
	AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, m_ShowBuffer);	
	m_ImageWnd.SetImageInfo(CameraID, StageRgn, m_ImageResolution, IMAGE_DATA_FOV);
	m_ImageWnd.SetImageBuffer(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ShowBuffer, false, bResetView);
	m_ImageWnd.SetImageEditRect(m_PatternRect4D);	
	m_ImageWnd.RedrawWnd(false);

	if ( CALIBRATION_DYNAMIC_TUNE == CalibrationMode )
	{	
		CloneImageBufferList();
		AnalysizeDynamicTuneOffset();
		BuildResultListWnd();		
		m_DynamicTuneIndex ++;
		const size_t Count=m_DynamicTuneList.size();
		if ( m_DynamicTuneIndex < Count )
		{	
			if ( ExecDynamicTuneNext() == true )
			{	return true; }			
		}
		SetCalibrationMode(CALIBRATION_DYNAMIC_STOP);
		MotionCtrlPtr->GetMotionParameter()=m_MotionParam;
		MotionCtrlPtr->SaveMotionParamInternal();
		MotionCtrlPtr->UpdateMotionParamter();
		AnalysizeDynamicTuneResult();
	}

	LockUIWnd(false);
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::RedrawWnd()
{
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::CreateBKImage()
{
}
//-------------------------------------------------------------------------------------//
CAMERA_ID CCaliPaneDynamic::GetCameraID()
{
	return m_CameraID;
}
//-------------------------------------------------------------------------------------//
UINT CCaliPaneDynamic::GetImageCount()
{
	UINT ImageBufferCount = CameraCtrl.GetCameraImageBufferCount(GetCameraID());
	UINT ImageCnt  = CWnd::GetDlgItemInt(CALIDYNAMIC_IMAGE_COUNT_EDIT);
	UINT ImageCnt2 = MIN(ImageCnt, ImageBufferCount-1);
	UINT ImageCnt3 = MIN(ImageCnt2, MAX_TABLE_COUNT);
	UINT ImageCnt4 = MIN(ImageCnt3, 48);
	return ImageCnt4;
}
//-------------------------------------------------------------------------------------//
DWORD CCaliPaneDynamic::GetMoveDownDelyTime()//移動完成延遲時間	
{
	DWORD DelyTime = (DWORD)(CWnd::GetDlgItemInt(CALIDYNAMIC_DELAY_TIME_EDIT));
	return DelyTime;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::LockUIWnd(bool bLock)
{
	//AOIDataCollect.SetIsLockUIWnd(bLock);
	BOOL bEnable = TRUE;
	if ( true == bLock ) { bEnable = FALSE; }
	else { bEnable = TRUE; }

	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_POS_GO_BTN_A, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_POS_GET_BTN_A, bEnable);	
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_POS_GO_BTN_B, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_POS_GET_BTN_B, bEnable);	
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_POS_GO_BTN_C, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_POS_GET_BTN_C, bEnable);	

	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_JOG_CHK, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_MOTION_WND_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_SET_BC_POS_BTN, bEnable);	

	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_GRAB_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_BUILD_PATTERN_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_MOTION_WND_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_TEST_BTN_AA, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_TEST_BTN_AB, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_TEST_BTN_AC, bEnable);

	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_AUTO_TUNE_BTN, bEnable);

	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_SLICE_COMBO, bEnable);
	JetAPI::EnableCtrlWnd(this, CALIDYNAMIC_RESULT_LIST_WND, bEnable);

	JetAPI::EnableEditWnd(this, CALIDYNAMIC_POS_X_EDIT_A, bEnable);
	JetAPI::EnableEditWnd(this, CALIDYNAMIC_POS_Y_EDIT_A, bEnable);
	JetAPI::EnableEditWnd(this, CALIDYNAMIC_POS_X_EDIT_B, bEnable);
	JetAPI::EnableEditWnd(this, CALIDYNAMIC_POS_Y_EDIT_B, bEnable);
	JetAPI::EnableEditWnd(this, CALIDYNAMIC_POS_X_EDIT_C, bEnable);
	JetAPI::EnableEditWnd(this, CALIDYNAMIC_POS_Y_EDIT_C, bEnable);
	JetAPI::EnableEditWnd(this, CALIDYNAMIC_DELAY_TIME_EDIT, bEnable);	
	JetAPI::EnableEditWnd(this, CALIDYNAMIC_IMAGE_COUNT_EDIT, bEnable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return false;
	//return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::SetCalibrationMode(CALIBRATION_DYNAMIC_MODE Mode)
{
	m_CalibrationMode = Mode;
}
//-------------------------------------------------------------------------------------//
CALIBRATION_DYNAMIC_MODE CCaliPaneDynamic::GetCalibrationMode() const
{
	return m_CalibrationMode;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::ClearLogListBox()//清除紀錄列表視窗
{
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::AddLogListBox(LPCTSTR str)//加入紀錄列表視窗
{
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnPosGoBtnA() 
{
	// TODO: Add your control notification handler code here
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	double PosX = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_A));
	double PosY = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_A));
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY, OfflineMode) == false ) 
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecStopGrab();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnPosGoBtnB() 
{
	// TODO: Add your control notification handler code here
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	double PosX = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_B));
	double PosY = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_B));
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY, OfflineMode) == false ) 
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecStopGrab();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnPosGoBtnC() 
{
	// TODO: Add your control notification handler code here
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	double PosX = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_C));
	double PosY = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_C));
	if ( MotionCtrlPtr->XYMoveTo(PosX, PosY, OfflineMode) == false ) 
	{	JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	}
	ExecStopGrab();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnBuildPatternBtn() 
{
	// TODO: Add your control notification handler code here	
	const char fnName[] = "CCaliPaneDynamic::OnBuildPatternBtn";
	if ( NULL == m_ImageBuffer ) { return; }
	RECT    RoiRect={0};
	TRECT4D RoiRect4d;	
	m_ImageWnd.GetImageEditRect(RoiRect4d);	
	const int nAlign = 4;	
	const IMAGE_SIZE ImageW = (m_ImageW);
	const IMAGE_SIZE ImageH = (m_ImageH);
	const IMAGE_SIZE ImageStep = (m_ImageStep);
	const IMAGE_SIZE BitCount = (m_BitCount);
	const size_t     ImageBufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	IMAGE_PTR        ImagePtr = m_ImageBuffer;	
	RoiRect.left   = (int)(RoiRect4d.left+0.5);
	RoiRect.top    = (int)(RoiRect4d.top+0.5);
	RoiRect.right  = (int)(RoiRect4d.right+0.5);
	RoiRect.bottom = (int)(RoiRect4d.bottom+0.5);
	JetAPI::Rect4DToRect(RoiRect4d, RoiRect);
	if ( JetAPI::CheckImageRoi(ImageW, ImageH, RoiRect) == false )
	{	return; }	

	CString    str;
	IMAGE_PTR  RoiPtr=NULL;
	IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
	IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
	IMAGE_SIZE RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
	IMAGE_SIZE RoiBitCount=BitCount;
	const int  RoiPosX = (RoiRect.left+RoiRect.right)/2;
	const int  RoiPosY = (RoiRect.top+RoiRect.bottom)/2;
	const size_t RoiBufSize = ImageAPI.CalcBufferSize(RoiStep, RoiH);
	if ( JetMemory.alloc_func(RoiBufSize, RoiPtr, fnName, "RoiPtr") == false )
	{	return ; }	
	if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr, false) == false )
	{
		JetMemory.free_func(RoiPtr);
		return ;
	}

#ifdef _DEBUG
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("DyanmicRoi.PNG"));
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiPtr, false);
#endif//_DEBUG

	ReleasePatternImage();	
	m_PatternX = RoiPosX;
	m_PatternY = RoiPosY;
	m_PatternW = RoiW;
	m_PatternH = RoiH;
	m_PatternStep = RoiStep;
	m_PatternBitCnt = RoiBitCount;
	m_PatternPtr = RoiPtr;
	m_PatternRect4D = RoiRect4d;
	m_ImageWnd.SetImageEditRect(RoiRect4d);	
	m_ImageWnd.RedrawWnd(false);
	::memcpy(m_ImageBuffer2, ImagePtr, sizeof(IMAGE_DATA)*ImageBufferSize);	

	const size_t ImageCount = 1;
	CreateImageBufferList(ImageBufferSize, ImageCount);
	const size_t NodeCount = m_DynamicNodeList.size();
	if ( NodeCount > 0 ) 
	{
		const size_t idx = 0;		
		m_DynamicNodeList[idx].ImageW = ImageW;
		m_DynamicNodeList[idx].ImageH = ImageH;
		m_DynamicNodeList[idx].ImageStep = ImageStep;
		m_DynamicNodeList[idx].BitCount = BitCount;
		::memcpy(m_DynamicNodeList[idx].ImagePtr, ImagePtr, sizeof(IMAGE_DATA)*ImageBufferSize);
		//CloneImageBufferList();
		AnalysizeDynamicOffset();
	}
	BuildResultListWnd();
	OnPosGetBtnA();
	return ;	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ExecStopGrab()
{
	if ( ConfigGrabParam(CALIBRATION_DYNAMIC_STOP) == false )
	{	return false; }	
	SetCalibrationMode(CALIBRATION_DYNAMIC_STOP);
	ExecGrabFirst();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ExecDynamicTest(double PosX1, double PosY1, double PosX2, double PosY2)
{
	DWORD dwTime = 250;//GetMoveDownDelyTime();	
	if ( ConfigGrabParam(CALIBRATION_DYNAMIC_TEST) == false )
	{	return false; }	
	SetCalibrationMode(CALIBRATION_DYNAMIC_TEST);
	MotionCtrlPtr->XYMoveTo(PosX2, PosY2);
	MotionCtrlPtr->WaitForMotionStop();
	if ( dwTime > 0 ) 
	{	::Sleep(dwTime); }

	size_t    i=0;
	CAMERA_ID CameraID = GetCameraID();
	std::vector<TSliceParam> ParamList;
	const size_t ImageCount = GetImageCount();
	for ( i=0; i<ImageCount; i++ )
	{	ParamList.push_back(m_SliceParam);	}	
	if ( CameraCtrl.BatchGrabPrepare2(ParamList, true) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	
	MotionCtrlPtr->SetIsJogMode(false);
	CWnd::CheckDlgButton(CALIDYNAMIC_JOG_CHK, FALSE);
	MotionCtrlPtr->XYMoveTo(PosX1, PosY1);	
	
	LARGE_INTEGER fnStart, fnEnd;
	JetAPI::SetFuncTimeStart(fnStart);
	//ExecGrabFirst();
	ExecGrabNext();
	JetAPI::SetFuncTimeEnd(fnEnd);
	double Time = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ExecDynamicTuneFirst()
{
	CCaliWndDynamicTune TuneWnd;
	TuneWnd.SetTDynamicTuneParam(m_DynamicTuneParam);
	if ( TuneWnd.DoModal() == IDCANCEL )
	{	return true; }		
	
	TPOINT3D StagePos;
	TPOINT3D StageOrg;
	int      GoupID=0;
	bool    bValidPos;
	CString  str, str1, str2;	
	TDynamicTuneParam TuneParam;
	unsigned int &TuneParamIndex = m_DynamicTuneIndex;
	std::vector<TDynamicTuneParam> &TuneParamList = m_DynamicTuneList;	

	TuneParamIndex = 0;
	TuneParamList.clear();	
	TuneWnd.GetTDynamicTuneParam(TuneParam);	
	
	MotionCtrlPtr->GetCurrentPos(StageOrg.x, StageOrg.y, StageOrg.z);

	GoupID = 0;
	for ( int i=TuneParam.DistanceMin; i<=TuneParam.DistanceMax; i+=TuneParam.DistanceStep )
	{
		TuneParam.DistanceUse = i;
		//確認是否超過機台範圍
		StagePos = StageOrg;		
		switch ( TuneParam.nAxis )
		{
		case AXIS_X:	
			StagePos.x += i;	
			bValidPos = MotionCtrlPtr->CheckStagePosValidX(StagePos.x);
			break;
		case AXIS_Y:
			StagePos.y += i;
			bValidPos = MotionCtrlPtr->CheckStagePosValidX(StagePos.y);
			break;
		case AXIS_Z:
			StagePos.z += i;
			bValidPos = MotionCtrlPtr->CheckStagePosValidX(StagePos.z);
			break;
		default:
			bValidPos = true;
			break;
		}
		if ( false == bValidPos )
		{	continue; }

		for ( int j=TuneParam.VelocityMin; j<=TuneParam.VelocityMax; j+=TuneParam.VelocityStep )
		{
			TuneParam.VelocityUse = j;
			for ( int k=TuneParam.AccelerationMin; k<=TuneParam.AccelerationMax; k+=TuneParam.AccelerationStep )
			{
				GoupID ++;
				TuneParam.AccelerationUse = k;
				for ( int s=0; s<TuneParam.nRepeat; s++ )
				{
					TuneParam.nGroupID = GoupID;
					TuneParamList.push_back(TuneParam);
				}
			}
		}
	}
	const size_t TuneParamCount=TuneParamList.size();
	if ( 0 == TuneParamCount )
	{	return true; }
	
	CString strCount=AOIDataDefine.GetCountText();
	str1.Format(_T("%s = %d"), strCount, TuneParamCount);
	str2 = _T("Do you want to continue?");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s\n%s"), str1, str2);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return true; }
	
	m_DynamicTuneParam = TuneParam;
	if ( ConfigGrabParam(CALIBRATION_DYNAMIC_TUNE) == false )
	{	return true; }			
	
	size_t    i=0;
	CAMERA_ID CameraID = GetCameraID();
	std::vector<TSliceParam> ParamList;
	const size_t ImageCount = 1;
	for ( i=0; i<ImageCount; i++ )
	{	ParamList.push_back(m_SliceParam);	}	
	if ( CameraCtrl.BatchGrabPrepare2(ParamList, true) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	
	LockUIWnd(true);
	m_StageOrg = StageOrg;
	MotionCtrlPtr->SetIsJogMode(false);
	CWnd::CheckDlgButton(CALIDYNAMIC_JOG_CHK, FALSE);
	SetCalibrationMode(CALIBRATION_DYNAMIC_TUNE);	
	m_MotionParam = MotionCtrlPtr->GetMotionParameter();		
	if ( ExecDynamicTuneNext() == false )
	{	
		LockUIWnd(false);
		MotionCtrlPtr->GetMotionParameter()=m_MotionParam;
		MotionCtrlPtr->SaveMotionParamInternal();
		MotionCtrlPtr->UpdateMotionParamter();
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ExecDynamicTuneNext()
{
	unsigned int &TuneParamIndex = m_DynamicTuneIndex;
	std::vector<TDynamicTuneParam> &TuneParamList = m_DynamicTuneList;	
	TMotionParameter &MotionParamRef = MotionCtrlPtr->GetMotionParameter();
	const size_t TuneParamCount=TuneParamList.size();
	if ( TuneParamIndex >= TuneParamCount )
	{	return false;	}

	TDynamicTuneParam TuneParam = TuneParamList[TuneParamIndex];	
	double PosX1 = m_StageOrg.x;
	double PosY1 = m_StageOrg.y;
	double PosX2 = m_StageOrg.x;
	double PosY2 = m_StageOrg.y;
	double PosZ2 = 0;
	switch ( TuneParam.nAxis )
	{
	case AXIS_X:
		PosX2 += TuneParam.DistanceUse;
		MotionParamRef.m_MaxVelocityX = TuneParam.VelocityUse;
		MotionParamRef.m_GrabbingVelocityX = TuneParam.VelocityUse;		
		MotionParamRef.m_AccelerationValueX = TuneParam.AccelerationUse;		
		MotionParamRef.m_AccelerationTimeX = TuneParam.AccelerationMinTime;
		MotionParamRef.m_DecelerationTimeX = TuneParam.DecelerationMinTime;
		break;
	case AXIS_Y:
		PosY2 += TuneParam.DistanceUse;
		MotionParamRef.m_MaxVelocityY = TuneParam.VelocityUse;
		MotionParamRef.m_GrabbingVelocityY = TuneParam.VelocityUse;		
		MotionParamRef.m_AccelerationValueY = TuneParam.AccelerationUse;
		MotionParamRef.m_AccelerationTimeY = TuneParam.AccelerationMinTime;
		MotionParamRef.m_DecelerationTimeY = TuneParam.DecelerationMinTime;
		break;
	case AXIS_Z:
		PosZ2 += TuneParam.DistanceUse;
		MotionParamRef.m_MaxVelocityZ = TuneParam.VelocityUse;
		MotionParamRef.m_GrabbingVelocityZ = TuneParam.VelocityUse;		
		MotionParamRef.m_AccelerationValueZ = TuneParam.AccelerationUse;
		MotionParamRef.m_AccelerationTimeZ = TuneParam.AccelerationMinTime;
		MotionParamRef.m_DecelerationTimeZ = TuneParam.DecelerationMinTime;
		break;
	}	

	if ( MotionCtrlPtr->SaveMotionParamInternal() == false )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); 
		return false;
	}
	if ( MotionCtrlPtr->UpdateMotionParamter() == false )
	{	
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString()); 
		return false;
	}

	DWORD dwTime = 250;//GetMoveDownDelyTime();		
	if ( MotionCtrlPtr->XYMoveTo(PosX2, PosY2) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false; 
	}
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false; 
	}
	if ( dwTime > 0 ) 
	{	::Sleep(dwTime); }
			
	JetAPI::SetFuncTimeEnd(m_TuneStart);
	if ( MotionCtrlPtr->XYMoveTo(PosX1, PosY1) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false; 
	}
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false; 
	}
	
	LARGE_INTEGER fnStart, fnEnd;
	JetAPI::SetFuncTimeStart(fnStart);
	//ExecGrabFirst();
	m_CameraImageReceieveCount = 0;
	if ( ExecGrabNext() == false )
	{	return false;	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	double Time = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);	
	return true;	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnTestBtnAA() 
{
	// TODO: Add your control notification handler code here
	CreateTempFolder();
	const double PosAX = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_A));
	const double PosAY = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_A));	
	ExecDynamicTest(PosAX, PosAY, PosAX, PosAY);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnTestBtnAB() 
{
	// TODO: Add your control notification handler code here
	CreateTempFolder();
	const double PosAX = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_A));
	const double PosAY = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_A));
	const double PosBX = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_B));
	const double PosBY = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_B));	
	ExecDynamicTest(PosAX, PosAY, PosBX, PosBY);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnTestBtnAC() 
{
	// TODO: Add your control notification handler code here	
	CreateTempFolder();
	const double PosAX = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_A));
	const double PosAY = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_A));
	const double PosCX = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_X_EDIT_C));
	const double PosCY = (int)(CWnd::GetDlgItemInt(CALIDYNAMIC_POS_Y_EDIT_C));	
	ExecDynamicTest(PosAX, PosAY, PosCX, PosCY);
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ConfigGrabParam(CALIBRATION_DYNAMIC_MODE Mode)//組態取像參數
{	
	CString str;	
	unsigned int i=0, j=0;
	bool bGrabRepeat=false;
	double FOVW = 0, FOVH = 0;
	double ResX = 0, ResY = 0;
	TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();
	CAMERA_ID CameraID = GetCameraID();
	const IMAGE_SIZE ImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	const IMAGE_SIZE ImageH = CameraCtrl.GetCameraImageSizeH(CameraID);	
	const IMAGE_SIZE ImageStep =  CameraCtrl.GetCameraImageStep(CameraID);//取得相機影像每條寬度
	const IMAGE_SIZE ImageWHalf = ImageW/2;
	const IMAGE_SIZE ImageHHalf = ImageH/2;
	const size_t     ImageBuffer = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const size_t     ImageCount = GetImageCount();	
	const unsigned int SliceUniqueID = JetAPI::GetComboxCurSelData(m_SliceCombox);
	TSliceParam *SliceParamPtr = AOIDataCollect.GetSystemSliceParamPtrByUniqueID(SliceUniqueID);		
	if ( NULL != SliceParamPtr)
	{	m_SliceParam = *SliceParamPtr;	}
	else
	{	m_SliceParam = TSliceParam(); }	
	m_CameraImageReceieveCount = 0;
	switch ( Mode )
	{
	case CALIBRATION_DYNAMIC_TEST:
		if ( CreateImageBufferList(ImageBuffer, ImageCount+1) == false )
		{	return false; }
		break;	
	case CALIBRATION_DYNAMIC_TUNE:
		if ( CreateImageBufferList(ImageBuffer, 2) == false )
		{	return false; }
		break;
	default://case CALIBRATION_DYNAMIC_STOP:
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::FocusToEditCtrl()
{
	JetAPI::FocusCtrlWnd(this, CALIDYNAMIC_INFO_EDIT);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ExecGrabFirst()//執行第一次取像
{
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	DWORD DelayTime = GetMoveDownDelyTime();
	if ( DelayTime > 0 )
	{	Sleep(DelayTime); }
	
	size_t    i=0;
	CAMERA_ID CameraID = GetCameraID();
	std::vector<TSliceParam> ParamList;
	const size_t ImageCount = GetImageCount();
	CALIBRATION_DYNAMIC_MODE CalibrationMode = GetCalibrationMode();
	if ( CALIBRATION_DYNAMIC_TEST == CalibrationMode )
	{
		for ( i=0; i<ImageCount; i++ )
		{	ParamList.push_back(m_SliceParam);	}
	}
	else
	{	ParamList.push_back(m_SliceParam);	}
	
	if ( CameraCtrl.BatchGrabPrepare2(ParamList, true) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}	
	bool bFinish = false;		
	this->LockUIWnd(true);			
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());	
	if ( CameraCtrl.BatchGrabStart2(bFinish) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		this->LockUIWnd(false);
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ExecGrabNext()//執行下一次取像
{
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	JetAPI::SetFuncTimeStart(m_TestSetup);
	DWORD DelayTime = GetMoveDownDelyTime();
	if ( DelayTime > 0 )
	{	Sleep(DelayTime); }

	bool bFinish = false;	
	CAMERA_ID CameraID = GetCameraID();
	DWORD BatchGrabMode = CameraCtrl.GetBatchGrabMode();
	BATCH_GRAB_STEP GrabStep = CameraCtrl.GetBatchGrabFirstStep(BatchGrabMode);
	CameraCtrl.ClearCameraCount(CameraID);
	CameraCtrl.ResetAllCameraRingBuffer();
	CameraCtrl.SetBatchGrabStep(GrabStep);	
	CameraCtrl.BatchIndexReset();
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());			
	JetAPI::SetFuncTimeEnd(m_TestStart);
	if ( CameraCtrl.BatchGrabStart2(bFinish) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		CameraCtrl.SetCameraToSendCallback(CameraID, TRUE);
		this->LockUIWnd(false);
		return false;
	}	
	if ( false == bFinish )
	{	return true; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::CreateImageBufferList(size_t BufferSize, size_t ImageCount)
{
	const char   fnName[] = "CCaliPaneDynamic::CreateImageBufferList()";
	size_t       i=0;	
	TPOINT3D     Offset;
	IMAGE_PTR    ImagePtr=NULL;
	TDynamicNode DynamicNode;
	ReleaseImageBufferList();
	if ( 0 == BufferSize )
	{	return false; }	
	for ( i=0; i<ImageCount; i++ )
	{		
		if ( JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
		{
			ReleaseImageBufferList();
			return false;
		}		
		::memset(ImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
		DynamicNode.ImagePtr = ImagePtr;
		DynamicNode.BufferSize = BufferSize;
		m_DynamicNodeList.push_back(DynamicNode);		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::CloneImageBufferList()
{	
	long       i = 0;
	long       idx=0;	
	bool       bSaved=false;
	CString    str;
	size_t     BufferSize=0;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	CAMERA_ID  CameraID = GetCameraID();
	const long NodeCount = (long)(m_DynamicNodeList.size());
	const long ImageCount = CameraCtrl.GetImageCallbackCount(CameraID);		
	const long RingIndex = CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID);
	const long RingListSize  = CameraCtrl.GetCameraRingBufferListSize(CameraID);
	const double SetupTime = JetAPI::CalcFuncTimeSpent(m_TestSetup, m_TestStart);
	const double GrabAllTime = JetAPI::CalcFuncTimeSpent(m_TestStart, m_TestEnd);
	const DWORD  dwSetupTime = (DWORD)(SetupTime);

	idx = RingIndex;
	idx = idx-ImageCount;
	idx = idx+m_CameraImageReceieveCount;
	if ( idx < 0 ) { idx += RingListSize;  }

	if ( (ImageCount+1) != NodeCount )
	{	return false;	}

	i = 0;
	BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);
	if ( BufferSize > m_DynamicNodeList[i].BufferSize )
	{	return false;	}	
	m_DynamicNodeList[i].ImageW = m_ImageW;
	m_DynamicNodeList[i].ImageH = m_ImageH;
	m_DynamicNodeList[i].ImageStep = m_ImageStep;
	m_DynamicNodeList[i].BitCount = m_BitCount;
	m_DynamicNodeList[i].Time = dwSetupTime;
	::memcpy(m_DynamicNodeList[i].ImagePtr, m_ImageBuffer2, sizeof(IMAGE_DATA)*BufferSize);			
#ifdef _DEBUG
	//if ( true == bSaved )
	{
		str.Format(_T("%s\\%s#ORG.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("DynamicImg"));
		ImageAPI.SaveImage(str, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_DynamicNodeList[i].ImagePtr, false);
	}
#endif//_DEBUG

	
	for ( i=1; i<NodeCount; i++ )
	{
		if ( CameraCtrl.GetCameraRingBufferImage(CameraID, idx, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{
			JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
			return false; 
		}
		idx ++;
		m_CameraImageReceieveCount ++;

		BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( BufferSize > m_DynamicNodeList[i].BufferSize )
		{	return false;	}		
		m_DynamicNodeList[i].ImageW = ImageW;
		m_DynamicNodeList[i].ImageH = ImageH;
		m_DynamicNodeList[i].ImageStep = ImageStep;
		m_DynamicNodeList[i].BitCount = m_BitCount;
		m_DynamicNodeList[i].Time = (DWORD)((i*GrabAllTime)/ImageCount)+dwSetupTime;
		::memcpy(m_DynamicNodeList[i].ImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);		

	#ifdef _DEBUG
		if ( true == bSaved )
		{
			str.Format(_T("%s\\%s#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("DynamicImg"), i);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, m_BitCount, ImagePtr, false);
		}
	#endif//_DEBUG
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ReleaseImageBufferList()
{
	size_t       i=0;
	const size_t Count = m_DynamicNodeList.size();
	for ( i=0; i<Count; i++ )
	{
		JetMemory.free_func(m_DynamicNodeList[i].ImagePtr);
		m_DynamicNodeList[i].ImagePtr = NULL;
	}
	m_ImageIndex = 0;	
	m_DynamicNodeList.clear();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ReleasePatternImage()
{
	if ( NULL != m_PatternPtr )
	{	JetMemory.free_func(m_PatternPtr); }
	m_PatternW = 100;
	m_PatternH = 100;
	m_PatternStep = 100;
	m_PatternBitCnt = 8;
	m_PatternPtr = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::ClearResultListWnd()
{
	CThisListCtrl4 &ListCtrl = m_ResultListWnd;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }
	m_StopResultListBeSelected = true;
	ListCtrl.DeleteAllItems();
	m_StopResultListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::BuildResultListWnd()
{
	CThisListCtrl4 &ListCtrl = m_ResultListWnd;
	if ( ListCtrl.GetSafeHwnd() == NULL ) { return true; }
	ClearResultListWnd();

	size_t       i=0;
	int          nItem=0;
	int          nSubItem=0;
	CString      str;
	const size_t NodeCount = m_DynamicNodeList.size();	
	
	ListCtrl.SetRedraw(FALSE);
	m_StopResultListBeSelected = true;
	for ( i=0; i<NodeCount; i++ )
	{
		nSubItem = 0;
		if ( 0 == i )
		{	str = _T("ORG");	}
		else
		{	str.Format(_T("%d"), i); }
		ListCtrl.InsertItem(nItem, str);

		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str.Format(_T("%.1f"), m_DynamicNodeList[i].OffsetX);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str.Format(_T("%.1f"), m_DynamicNodeList[i].OffsetY);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str.Format(_T("%.1f"), m_DynamicNodeList[i].Score);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str.Format(_T("%d"), m_DynamicNodeList[i].Time);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}
	m_StopResultListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::BuildResultListWndHeader()
{
	CString str;
	CString strOffset;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT

	{
		CThisListCtrl4 &ListCtrl = m_ResultListWnd;

		strOffset = AOIDataDefine.GetOffsetText();
		ListCtrl.GetClientRect(&Rect);
		width = 48;
		width2 = (Rect.right-Rect.left-width-32)/4;
		str = _T("Index");
		str = AOIDataDefine.GetIndexText();		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str.Format(_T("%s-X-um"), strOffset);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str.Format(_T("%s-Y-um"), strOffset);
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;

		str = _T("Score");
		str = AOIDataDefine.GetScoreText();
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		

		str = _T("Time");
		str.Format(_T("%s-ms"), AOIDataDefine.GetTimeText());
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnGrabBtn() 
{
	// TODO: Add your control notification handler code here
	ExecStopGrab();	
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::AnalysizeDynamicOffset()//分析動態變化
{
	const char fnName[] = "CCaliPaneDynamic::AnalysizeDynamicOffset()";	
	if ( NULL == m_PatternPtr ) { return false; }
	const size_t NodeCount = m_DynamicNodeList.size();

	CJetMatch Match;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	return false;	}
	
	
	size_t    i=0;
	CString   str;
	RECT      RoiRect;
	TRECT4D   RoiRect4d;
	bool      bSaved=false;
	IMAGE_PTR RoiPtr= NULL;
	const int RoiExtW = 50;
	const int RoiExtH = 50;
	const int PatX = (int)(m_PatternX);
	const int PatY = (int)(m_PatternY);
	const int nPatW = (int)(m_PatternW);
	const int nPatH = (int)(m_PatternH);
	const int nPatStep = (int)(m_PatternStep);
	const int nPatBitCnt = (int)(m_PatternBitCnt);
	const int nRoiW = nPatW+RoiExtW+RoiExtW;
	const int nRoiH = nPatH+RoiExtH+RoiExtH;
	const int nRoiStep = JetAPI::GetBMPImagePixelsPerLine(nRoiW, nPatBitCnt, 4);
	const size_t RoiBufferSize = nRoiStep*nRoiH;
	CString Folder = AOIDataCollect.GetAOITempDirectory();	

	RoiRect4d.left   = PatX-(nRoiW/2);
	RoiRect4d.right  = RoiRect4d.left+(nRoiW);
	RoiRect4d.top    = PatY-(nRoiH/2);
	RoiRect4d.bottom = RoiRect4d.top+(nRoiH);

	RoiRect.left   = (int)(RoiRect4d.left+0.5);
	RoiRect.top    = (int)(RoiRect4d.top+0.5);
	RoiRect.right  = (int)(RoiRect4d.right+0.5);
	RoiRect.bottom = (int)(RoiRect4d.bottom+0.5);
	JetAPI::Rect4DToRect(RoiRect4d, RoiRect);
	const double RoiCpX = (RoiRect.right-RoiRect.left)*0.5;
	const double RoiCpY = (RoiRect.bottom-RoiRect.top)*0.5;
	const double RoiCpX2 = (RoiRect4d.right-RoiRect4d.left)*0.5;
	const double RoiCpY2 = (RoiRect4d.bottom-RoiRect4d.top)*0.5;

	Match.SetInterpolate(true);
	Match.SetMinReducedArea(128);
	Match.SetMaxInitialPositions(4);

	if ( Match.LearnPattern(nPatW, nPatH, nPatStep, nPatBitCnt, m_PatternPtr, true) == false ) 
	{
		JetAPI::ShowMessageBox(Match.GetErrorString());
		return false; 
	}
#ifdef _DEBUG
	if ( true == bSaved )
	{
		str.Format(_T("%s\\%s.PNG"), Folder, _T("DynamicMatPat"));
		ImageAPI.SaveImage(str, nPatW, nPatH, nPatStep, nPatBitCnt, m_PatternPtr, false);
	}
#endif//_DEBUG

	if ( JetMemory.alloc_func(RoiBufferSize, RoiPtr, fnName, "RoiPtr") == false ) 
	{
		JetAPI::ShowMessageBox(JetMemory.GetErrorString());
		return false; 
	}
	
	int NResults = 0;
	double ResX=0, ResY=0, ResS=0;	
	double FullResX=0, FullResY=0;	
	TPOINT3D Offset;
	TPOINT3D OffsetMax;	
	TPOINT3D OffsetMin;	
	TDynamicNode DynamicNode;
	for ( i=0; i<NodeCount; i++ )
	{
		DynamicNode = m_DynamicNodeList[i]; 		
		IMAGE_PTR  ImagePtr = DynamicNode.ImagePtr;
		IMAGE_SIZE ImageW = DynamicNode.ImageW;
		IMAGE_SIZE ImageH = DynamicNode.ImageH;
		IMAGE_SIZE ImageStep = DynamicNode.ImageStep;
		IMAGE_SIZE BitCount = DynamicNode.BitCount;
		if ( NULL == ImagePtr ) { continue; }		
		if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, nRoiStep, RoiPtr, false ) == false )
		{	continue; }		
		
	#ifdef _DEBUG
		if ( true == bSaved )
		{
			str.Format(_T("%s\\%s#%d.PNG"), Folder, _T("DynamicMatRoi"), i+1);
			ImageAPI.SaveImage(str, nRoiW, nRoiH, nRoiStep, BitCount, RoiPtr, false);
		}
	#endif//_DEBUG

		if ( Match.Match(nRoiW, nRoiH, nRoiStep, BitCount, RoiPtr, true) == false )
		{	continue; }

		NResults = Match.GetNumPositions();
		if ( 0 == NResults ) { continue; }
		ResX = Match.GetResultPosX(0);
		ResY = Match.GetResultPosY(0);
		ResS = Match.GetResultScore(0);
		FullResX = RoiRect4d.left+ResX;
		FullResY = RoiRect4d.top+ResY;
		Offset.x = (ResX-RoiCpX)*m_ImageResolution.x;
		Offset.y = (ResY-RoiCpY)*m_ImageResolution.y;
		Offset.z = ResS;

		if ( i < 2 ) 
		{	OffsetMax = OffsetMin = Offset;	}
		if ( OffsetMax.x < Offset.x ) { OffsetMax.x = Offset.x; }
		if ( OffsetMax.y < Offset.y ) { OffsetMax.y = Offset.y; }
		if ( OffsetMin.x > Offset.x ) { OffsetMin.x = Offset.x; }
		if ( OffsetMin.y > Offset.y ) { OffsetMin.y = Offset.y; }

		m_DynamicNodeList[i].OffsetX = Offset.x;	
		m_DynamicNodeList[i].OffsetY = Offset.y;	
		m_DynamicNodeList[i].Score = Offset.z;	
		m_DynamicNodeList[i].ResultRect.left = FullResX-(nPatW*0.5);
		m_DynamicNodeList[i].ResultRect.top  = FullResY-(nPatH*0.5);
		m_DynamicNodeList[i].ResultRect.right = m_DynamicNodeList[i].ResultRect.left+(nPatW);
		m_DynamicNodeList[i].ResultRect.bottom= m_DynamicNodeList[i].ResultRect.top+(nPatH);
	}
	JetMemory.free_func(RoiPtr);	

	const double MaxX = MAX(abs(OffsetMax.x), abs(OffsetMin.x));
	const double MaxY = MAX(abs(OffsetMax.y), abs(OffsetMin.y));
	const double DifX = OffsetMax.x-OffsetMin.x;
	const double DifY = OffsetMax.y-OffsetMin.y;
	str.Format(_T("Max Offset (%.1f, %.1f), Diff(%.1f, %.1f)"), MaxX, MaxY, DifX, DifY);
	SetInfoText(str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::AnalysizeDynamicTuneOffset()//分析動態變化
{
	if ( AnalysizeDynamicOffset() == false )
	{	return false; }

	CString str;	
	const size_t NodeCount=m_DynamicNodeList.size();
	const size_t TuneCount=m_DynamicTuneList.size();	
	str.Format(_T("    (%d/%d)"), m_DynamicTuneIndex+1, TuneCount);
	//AddInfoText(str);	
	
	if ( 0 == NodeCount )
	{	return false; }
	if ( m_DynamicTuneIndex >= TuneCount )
	{	return false; }
	const size_t idx=NodeCount-1;
	const TDynamicNode &DynamicNode=m_DynamicNodeList[idx];
	TDynamicTuneParam &TuneParam=m_DynamicTuneList[m_DynamicTuneIndex];
	TuneParam.OffsetX = DynamicNode.OffsetX;
	TuneParam.OffsetY = DynamicNode.OffsetY;
	TuneParam.TimeUse = JetAPI::CalcFuncTimeSpent(m_TuneStart, m_TestEnd);

	str.Format(_T("    (%d/%d, D:%d, V:%d, A:%d)"), m_DynamicTuneIndex+1, TuneCount, TuneParam.DistanceUse, TuneParam.VelocityUse, TuneParam.AccelerationUse);
	AddInfoText(str);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::AnalysizeDynamicTuneResult()//分析動態變化
{
	int i=0, j=0;
	int MaxGroupID=0;	
	std::vector<TDynamicTuneParam> BestList;
	std::vector<TDynamicTuneParam> GroupList;
	std::vector<TDynamicTuneParam> TempList=m_DynamicTuneList;
	if ( GroupTuneParamList(TempList, GroupList) == false )
	{	return false; }
	if ( FindBestTuneParamList(GroupList, BestList) == false )
	{	return false; }

	CString str;
	CString strFolder=AOIDataCollect.GetAOITempDirectory();
	str.Format(_T("%s\\%s"), strFolder, _T("TuneParamResultList.TXT"));
	//if ( SaveDynamicTuneParamList(str, GroupList) == true )	
	//if ( SaveDynamicTuneParamList(str, BestList) == true )	
	if ( SaveDynamicTuneParamList(str, BestList, GroupList) == true )		
	{	::ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::SaveDynamicTuneParamList(LPCTSTR filename, const std::vector<TDynamicTuneParam> &TuneList)
{	
	size_t   i=0;	
	TCHAR    TMode[32] = _T("");
	CString  str, strAxis, strDist, strVel, strAcc, strOffset, strTime;
	const size_t TuneParamCount=TuneList.size();
	
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile ) 
	{	return false;	}
	
	strAxis = AOIDataDefine.GetAxisText();
	strDist = AOIDataDefine.GetDistanceText();
	strVel  = AOIDataDefine.GetVelocityText();	
	strAcc = AOIDataDefine.GetAccelerationText();	
	strOffset = AOIDataDefine.GetOffsetText();
	strTime = AOIDataDefine.GetTimeText();
	
	::_ftprintf(pfile, _T("%s, %s, %s, %s, %s-X, %s-Y, %s(ms)\n"), strAxis, strDist, strVel, strAcc, strOffset, strOffset, strTime);		
	for ( i=0; i<TuneParamCount; i++ )
	{
		const TDynamicTuneParam &TuneParam=TuneList[i];	
		switch ( TuneParam.nAxis )
		{
		case AXIS_X:	str = _T("X");	break;
		case AXIS_Y:	str = _T("Y");	break;
		}
		::_ftprintf(pfile, _T("%s, %d, %d, %d, %.1f, %.1f, %.1f\n"), str, TuneParam.DistanceUse, TuneParam.VelocityUse, TuneParam.AccelerationUse, TuneParam.OffsetX, TuneParam.OffsetY, TuneParam.TimeUse);	
	}
	::fclose(pfile); pfile=NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::SaveDynamicTuneParamList(LPCTSTR filename, const std::vector<TDynamicTuneParam> &BestList, const std::vector<TDynamicTuneParam> &GroupList)
{
	size_t   i=0;	
	TCHAR    TMode[32] = _T("");	
	CString  str, strAxis, strDist, strVel, strAcc, strOffset, strTime;	
	
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile ) 
	{	return false;	}
	
	strAxis = AOIDataDefine.GetAxisText();
	strDist = AOIDataDefine.GetDistanceText();
	strVel  = AOIDataDefine.GetVelocityText();	
	strAcc = AOIDataDefine.GetAccelerationText();	
	strOffset = AOIDataDefine.GetOffsetText();
	strTime = AOIDataDefine.GetTimeText();		

	const size_t BestCount=BestList.size();
	if ( BestCount > 0 )
	{
		::_ftprintf(pfile, _T("%s, %s, %s, %s, %s-X, %s-Y, %s(ms)\n"), strAxis, strDist, strVel, strAcc, strOffset, strOffset, strTime);		
		for ( i=0; i<BestCount; i++ )
		{
			const TDynamicTuneParam &TuneParam=BestList[i];	
			switch ( TuneParam.nAxis )
			{
			case AXIS_X:	str = _T("X");	break;
			case AXIS_Y:	str = _T("Y");	break;
			}
			::_ftprintf(pfile, _T("%s, %d, %d, %d, %.1f, %.1f, %.1f\n"), str, TuneParam.DistanceUse, TuneParam.VelocityUse, TuneParam.AccelerationUse, TuneParam.OffsetX, TuneParam.OffsetY, TuneParam.TimeUse);	
		}
	}
	
	for ( i=0; i<64; i++ )
	{	::_ftprintf(pfile, _T("="));	}

	const size_t GroupCount=GroupList.size();
	if ( GroupCount > 0 )
	{
		::_ftprintf(pfile, _T("\n%s, %s, %s, %s, %s-X, %s-Y, %s(ms)\n"), strAxis, strDist, strVel, strAcc, strOffset, strOffset, strTime);		
		for ( i=0; i<GroupCount; i++ )
		{
			const TDynamicTuneParam &TuneParam=GroupList[i];	
			switch ( TuneParam.nAxis )
			{
			case AXIS_X:	str = _T("X");	break;
			case AXIS_Y:	str = _T("Y");	break;
			}
			::_ftprintf(pfile, _T("%s, %d, %d, %d, %.1f, %.1f, %.1f\n"), str, TuneParam.DistanceUse, TuneParam.VelocityUse, TuneParam.AccelerationUse, TuneParam.OffsetX, TuneParam.OffsetY, TuneParam.TimeUse);	
		}
	}
	::fclose(pfile); pfile=NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::GroupTuneParamList(const std::vector<TDynamicTuneParam> &SrcList, std::vector<TDynamicTuneParam> &DstList)//群組化列表
{
	int i=0, j=0;
	int MaxGroupID=0;
	const size_t SrcCount=SrcList.size();
	for ( i=0; i<SrcCount; i++ )
	{
		if ( MaxGroupID < SrcList[i].nGroupID )
		{	MaxGroupID = SrcList[i].nGroupID; }
	}
	MaxGroupID += 1;
	DstList.clear();
	for ( j=0; j<MaxGroupID; j++ )
	{
		int Cnt=0;
		TDynamicTuneParam TuneParam;
		TuneParam.nGroupID = -1;
		for ( i=0; i<SrcCount; i++ )
		{
			if ( j != SrcList[i].nGroupID )
			{	continue; }
			if ( -1 == TuneParam.nGroupID )
			{	TuneParam = SrcList[i];	}
			else
			{
				TuneParam.OffsetX += SrcList[i].OffsetX;
				TuneParam.OffsetY += SrcList[i].OffsetY;
				TuneParam.TimeUse += SrcList[i].TimeUse;
			}
			Cnt ++;
		}
		if ( 0 == Cnt )
		{	continue; }
		
		TuneParam.OffsetX /= Cnt;
		TuneParam.OffsetY /= Cnt;
		TuneParam.TimeUse /= Cnt;
		DstList.push_back(TuneParam);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCaliPaneDynamic::FindBestTuneParamList(const std::vector<TDynamicTuneParam> &SrcList, std::vector<TDynamicTuneParam> &DstList)//找最好化列表
{	//找同距離下時間滿足公差內的時間最小的參數
	int i=0, j=0;
	double Time=0.0;
	double Offset=0.0;
	size_t DistCount=0;
	std::vector<int> DistList;	
	const size_t SrcCount=SrcList.size();
	for ( i=0; i<SrcCount; i++ )
	{
		DistCount=DistList.size();
		for ( j=0; j<DistCount; j++ )
		{
			if ( DistList[j] == SrcList[i].DistanceUse )
			{	break; }
		}
		if ( j != DistCount )
		{	continue; }
		DistList.push_back(SrcList[i].DistanceUse);
	}	
	
	DstList.clear();
	DistCount=DistList.size();
	for ( j=0; j<DistCount; j++ )
	{	
		CSortObj SortObj;
		std::vector<CSortObj> SortList;
		SortObj.SetSortMode(SORT_BY_DBL);		
		for ( i=0; i<SrcCount; i++ )
		{
			if ( DistList[j] != SrcList[i].DistanceUse )
			{	continue; }			
			SortObj.SetID(i);
			SortObj.SetValueDbl(SrcList[i].TimeUse);
			SortList.push_back(SortObj);			
		}
		const size_t SortCnt=SortList.size();
		if ( 0 == SortCnt ) { continue; }
		//Sort By Time
		std::sort(SortList.begin(), SortList.end());

		int idx=0;
		std::vector<CSortObj> SortList2;
		//找滿足誤差範圍內的
		SortObj.SetSortMode(SORT_BY_DBL);		
		for ( i=0; i<SortCnt; i++ )
		{
			idx = SortList[i].GetID();
			if ( idx >= SrcCount ) { continue; }
			const TDynamicTuneParam &TuneParamRef=SrcList[idx];

			switch ( TuneParamRef.nAxis )
			{
			case AXIS_X: Offset = ::fabs(TuneParamRef.OffsetX);	break;
			case AXIS_Y: Offset = ::fabs(TuneParamRef.OffsetY);	break;
			default:
				Offset = 0;
				break;
			}
			SortObj.SetID(idx);
			SortObj.SetValueDbl(Offset);
			SortList2.push_back(SortObj);
			if ( Offset > TuneParamRef.nTolerance )
			{	continue; }
			break;			
		}
		if ( i < SortCnt )
		{	idx = SortList[i].GetID(); }
		else
		{
			//找誤差最小的
			std::sort(SortList2.begin(), SortList2.end());
			idx = SortList2[0].GetID();
		}		
		DstList.push_back(SrcList[idx]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnSelchangeSliceCombo() 
{
	// TODO: Add your control notification handler code here
	ExecStopGrab();	
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnItemchangedResultListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopResultListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	

	const size_t NodeCount = m_DynamicNodeList.size();
	if ( nItem >= NodeCount ) { return; }
	bool bResetView = false;
	TDynamicNode DynamicNode = m_DynamicNodeList[nItem];
	IMAGE_PTR  ImagePtr= DynamicNode.ImagePtr;
	IMAGE_SIZE ImageW = DynamicNode.ImageW;
	IMAGE_SIZE ImageH = DynamicNode.ImageH;
	IMAGE_SIZE ImageStep = DynamicNode.ImageStep;
	IMAGE_SIZE BitCount = DynamicNode.BitCount;
	AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, m_ShowBuffer);	
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, m_ShowBuffer, false, bResetView);
	m_ImageWnd.SetImageEditRect(DynamicNode.ResultRect);	
	m_ImageWnd.RedrawWnd(false);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::SetInfoText(LPCTSTR Text)
{
	CWnd::SetDlgItemText(CALIDYNAMIC_INFO_EDIT, Text);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::AddInfoText(LPCTSTR Text)
{
	CString str;
	CString ItemText;
	CWnd::GetDlgItemText(CALIDYNAMIC_INFO_EDIT, ItemText);
	str.Format(_T("%s%s"), ItemText, Text);
	SetInfoText(str);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnPosGetBtnA() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double  PosX=0, PosY=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY);
	str.Format(_T("%.0f"), PosX);	
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_X_EDIT_A, str);
	str.Format(_T("%.0f"), PosY);
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_Y_EDIT_A, str);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnPosGetBtnB() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double  PosX=0, PosY=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY);
	str.Format(_T("%.0f"), PosX);	
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_X_EDIT_B, str);
	str.Format(_T("%.0f"), PosY);
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_Y_EDIT_B, str);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnPosGetBtnC() 
{
	// TODO: Add your control notification handler code here
	CString str;
	double  PosX=0, PosY=0;
	MotionCtrlPtr->GetCurrentPos(PosX, PosY);
	str.Format(_T("%.0f"), PosX);	
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_X_EDIT_C, str);
	str.Format(_T("%.0f"), PosY);
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_Y_EDIT_C, str);
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnMotionWndBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_MotionCtrlWnd.GetSafeHwnd() == NULL  ) { return; }
	BOOL bVisible = m_MotionCtrlWnd.IsWindowVisible();
	if ( FALSE == bVisible )
	{	m_MotionCtrlWnd.ShowWindow(SW_SHOW); }
	else
	{	m_MotionCtrlWnd.ShowWindow(SW_HIDE); }
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnJogChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(CALIDYNAMIC_JOG_CHK);
	if ( TRUE == bCheck ) 
	{	MotionCtrlPtr->SetIsJogMode(true); }
	else
	{	MotionCtrlPtr->SetIsJogMode(false); }
	FocusToEditCtrl();
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnSetBCPosBtn() 
{
	// TODO: Add your control notification handler code here
	CString      str;
	CString      strLabel;
	CString      strValue;
	CString      strCaption;
	TListNode    Node;
	CInputBoxWnd InputBox;	
	CInputListWnd EnumWnd;
	std::vector<TListNode> NodelList;			
	const DWORD_PTR ComboxSel = m_LastAxisIndex;

	strCaption = _T("Set Axis Wnd");	
	strLabel = AOIDataDefine.GetAxisText();
	Node.Data = AXIS_X;	Node.Text = _T("X");	NodelList.push_back(Node);
	Node.Data = AXIS_Y;	Node.Text = _T("Y");	NodelList.push_back(Node);	
	Node.Data = -1;		Node.Text = AOIDataDefine.GetAllText();	NodelList.push_back(Node);	
	EnumWnd.SetParam1(strCaption, strLabel, ComboxSel, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }

	const unsigned int AxisIndex = (int)(EnumWnd.GetSelData());	

	strCaption = _T("Set Distance Wnd");	
	strLabel = AOIDataDefine.GetDistanceText();		
	switch ( AxisIndex ) 
	{
	case AXIS_X:
		strValue.Format(_T("%.0f"), MotionCtrlPtr->GetMotionParameter().m_TestTimeDistanceX);
		break;
	case AXIS_Y:
		strValue.Format(_T("%.0f"), MotionCtrlPtr->GetMotionParameter().m_TestTimeDistanceY);
		break;
	default:
		strValue = _T("40000");
		break;
	}	
	strValue.Format(_T("%.0f"), m_LastAxisOffset);
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) { return ; }
	const double Offset = ::_ttof(InputBox.m_DataEdit1);

	double  PosX_A=0, PosY_A=0;
	double  PosX_B=0, PosY_B=0;
	double  PosX_C=0, PosY_C=0;
	CString strPosX, strPosY;
	CWnd::GetDlgItemText(CALIDYNAMIC_POS_X_EDIT_A, strPosX);
	CWnd::GetDlgItemText(CALIDYNAMIC_POS_Y_EDIT_A, strPosY);
	PosX_C = PosX_B = PosX_A = ::_ttof(strPosX);
	PosY_C = PosY_B = PosY_A = ::_ttof(strPosY);

	switch ( AxisIndex ) 
	{
	case AXIS_X:
		PosX_B = PosX_A+Offset;
		PosX_C = PosX_A-Offset;
		break;
	case AXIS_Y:
		PosY_B = PosY_A+Offset;
		PosY_C = PosY_A-Offset;
		break;
	default://All
		PosX_B = PosX_A+Offset;
		PosY_B = PosY_A+Offset;

		PosX_C = PosX_A-Offset;		
		PosY_C = PosY_A-Offset;
		break;
	}
	m_LastAxisIndex = AxisIndex;
	m_LastAxisOffset = Offset;//上一次設定的偏移量	

	strPosX.Format(_T("%.0f"), PosX_B);
	strPosY.Format(_T("%.0f"), PosY_B);
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_X_EDIT_B, strPosX);
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_Y_EDIT_B, strPosY);
	strPosX.Format(_T("%.0f"), PosX_C);
	strPosY.Format(_T("%.0f"), PosY_C);
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_X_EDIT_C, strPosX);
	CWnd::SetDlgItemText(CALIDYNAMIC_POS_Y_EDIT_C, strPosY);
	return;
}
//-------------------------------------------------------------------------------------//
void CCaliPaneDynamic::OnAutoTuneBtn() 
{
	// TODO: Add your control notification handler code here
	ExecDynamicTuneFirst();
	return;
}
//-------------------------------------------------------------------------------------//