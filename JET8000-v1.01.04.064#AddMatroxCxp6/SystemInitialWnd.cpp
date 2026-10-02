// SystemInitialWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemInitialWnd.h"
//-------------------------------------------------------------------------------------//
#include "EvsAPI.h"
#include "MimAPI.h"
#include "Plc_Basic.h"
#include "Motion_Basic.h"
#include "Light3DCtrl.h"
#include "CameraCtrl.h"
#include "JetBarcode.h"
#include "InputListWnd.h"
#include "LightCtrlBoard.h"
#include "MES\\MES_Class.h"
#include "JetAlg\\JETAlg_Inc.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
CSystemInitialWnd  SystemInitialWnd;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemInitialWnd dialog
//-------------------------------------------------------------------------------------//
CSystemInitialWnd::CSystemInitialWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemInitialWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemInitialWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemInitialWnd)	
	DDX_Control(pDX, SYSINIT_BARCODE_DEVICE_INIT_ICON_8, m_BarcodeDeviceIcon8);
	DDX_Control(pDX, SYSINIT_BARCODE_DEVICE_INIT_ICON_7, m_BarcodeDeviceIcon7);
	DDX_Control(pDX, SYSINIT_BARCODE_DEVICE_INIT_ICON_6, m_BarcodeDeviceIcon6);
	DDX_Control(pDX, SYSINIT_BARCODE_DEVICE_INIT_ICON_5, m_BarcodeDeviceIcon5);
	DDX_Control(pDX, SYSINIT_BARCODE_DEVICE_INIT_ICON_4, m_BarcodeDeviceIcon4);
	DDX_Control(pDX, SYSINIT_BARCODE_DEVICE_INIT_ICON_3, m_BarcodeDeviceIcon3);
	DDX_Control(pDX, SYSINIT_BARCODE_DEVICE_INIT_ICON_2, m_BarcodeDeviceIcon2);
	DDX_Control(pDX, SYSINIT_BARCODE_DEVICE_INIT_ICON_1, m_BarcodeDeviceIcon1);	
	DDX_Control(pDX, SYSINIT_ITS_COMM_INIT_IMG, m_ITSCommImg);
	DDX_Control(pDX, SYSINIT_DTK_LIB_INIT_IMG, m_DTKLibImg);
	DDX_Control(pDX, SYSINIT_HON_LIB_INIT_IMG, m_HonLibImg);	
	DDX_Control(pDX, SYSINIT_ALG_LIB_INIT_IMG, m_AlgLibImg);
	DDX_Control(pDX, SYSINIT_PHASE_CTRL_INIT_IMG, m_PhaseCtrlImg);
	DDX_Control(pDX, SYSINIT_PLC_INIT_IMG, m_PlcInitImg);
	DDX_Control(pDX, SYSINIT_MOTION_INIT_IMG, m_MotionInitImg);
	DDX_Control(pDX, SYSINIT_LIGHT_CTRL_INIT_IMG, m_LightCtrlImg);
	DDX_Control(pDX, SYSINIT_IMAGE_LIB_INIT_IMG, m_ImageLibImg);
	DDX_Control(pDX, SYSINIT_CAMERA_INIT_IMG, m_CameraInitImg);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemInitialWnd, CDialog)
	//{{AFX_MSG_MAP(CSystemInitialWnd)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(SYSINIT_CAMERA_RESET_BTN, OnCameraResetBtn)
	ON_BN_CLICKED(SYSINIT_MOTION_RESET_BTN, OnMotionResetBtn)
	ON_BN_CLICKED(SYSINIT_PLC_RESET_BTN, OnPlcResetBtn)
	ON_BN_CLICKED(SYSINIT_LIGHT_CTRL_RESET_BTN, OnLightCtrlResetBtn)
	ON_BN_CLICKED(SYSINIT_LIGHT_CTRL_CONFIG_BTN, OnLightCtrlConfigBtn)	
	ON_BN_CLICKED(SYSINIT_PHASE_CTRL_RESET_BTN, OnPhaseCtrlResetBtn)
	ON_BN_CLICKED(SYSINIT_PHASE_CTRL_CONFIG_BTN, OnPhaseCtrlConfigBtn)	
	ON_BN_CLICKED(SYSINIT_ITS_COMM_RESET_BTN, OnITSCommResetBtn)
	ON_BN_CLICKED(SYSINIT_BARCODE_DEVICE_RESET_BTN, OnBarcodeDeviceResetBtn)	
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemInitialWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemInitialWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
#ifndef LIGHT_CTRL_BOARD_USE_IMP_CLS
	JetAPI::ShowCtrlWnd(this, SYSINIT_LIGHT_CTRL_CONFIG_BTN, FALSE);
#endif//LIGHT_CTRL_BOARD_USE_IMP_CLS
#ifndef LIGHT_3D_TI_DLP_IMP_USE
	JetAPI::ShowCtrlWnd(this, SYSINIT_PHASE_CTRL_CONFIG_BTN, FALSE);
#endif//LIGHT_3D_TI_DLP_IMP_USE

	this->m_LEDGreen.LoadBitmap(IDB_LED_MEDIAN_GREEN);
	this->m_LEDRed.LoadBitmap(IDB_LED_MEDIAN_RED);
	this->m_LEDGray.LoadBitmap(IDB_LED_MEDIAN_GRAY);

#ifndef HON_BARCODE_USE
	JetAPI::ShowCtrlWnd(this, SYSINIT_HON_LIB_INIT_IMG, FALSE);
	JetAPI::ShowCtrlWnd(this, SYSINIT_HON_LIB_INIT_EDIT, FALSE);
	JetAPI::ShowCtrlWnd(this, SYSINIT_HON_LIB_INIT_LABEL, FALSE);
#endif//HON_BARCODE_USE

	this->SwitchMultiLanguage();	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	UpdateBarcodeDeviceIcon(); }
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::RedrawWnd()
{
	//UpdateWindow();
	RECT WndRect;
	CWnd::GetClientRect(&WndRect);
	//RedrawWindow(&WndRect);
	//InvalidateRect(&WndRect);
	//Invalidate();
	//UpdateWindow();	
	RedrawWindow(NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_FRAME|RDW_ALLCHILDREN);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_INITIAL_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_INITIAL_WND;
	WndKey = _T("IDD_SYSTEM_INITIAL_WND");
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
	WndID = SYSINIT_APPLICATION_VERSION_LABEL;
	WndKey = _T("SYSINIT_APPLICATION_VERSION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_CAMERA_INIT_LABEL;
	WndKey = _T("SYSINIT_CAMERA_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_CAMERA_RESET_BTN;
	WndKey = _T("SYSINIT_CAMERA_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_MOTION_INIT_LABEL;
	WndKey = _T("SYSINIT_MOTION_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_MOTION_RESET_BTN;
	WndKey = _T("SYSINIT_MOTION_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = SYSINIT_PLC_INIT_LABEL;
	WndKey = _T("SYSINIT_PLC_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_PLC_RESET_BTN;
	WndKey = _T("SYSINIT_PLC_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_LIGHT_CTRL_INIT_LABEL;
	WndKey = _T("SYSINIT_LIGHT_CTRL_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_LIGHT_CTRL_RESET_BTN;
	WndKey = _T("SYSINIT_LIGHT_CTRL_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_LIGHT_CTRL_CONFIG_BTN;
	WndKey = _T("SYSINIT_LIGHT_CTRL_CONFIG_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSINIT_PHASE_CTRL_INIT_LABEL;
	WndKey = _T("SYSINIT_PHASE_CTRL_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = SYSINIT_PHASE_CTRL_RESET_BTN;
	WndKey = _T("SYSINIT_PHASE_CTRL_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = SYSINIT_PHASE_CTRL_CONFIG_BTN;
	WndKey = _T("SYSINIT_PHASE_CTRL_CONFIG_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = SYSINIT_IMAGE_LIB_INIT_LABEL;
	WndKey = _T("SYSINIT_IMAGE_LIB_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_DTK_LIB_INIT_LABEL;
	WndKey = _T("SYSINIT_DTK_LIB_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_HON_LIB_INIT_LABEL;
	WndKey = _T("SYSINIT_HON_LIB_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_ITS_COMM_INIT_LABEL;
	WndKey = _T("SYSINIT_ITS_COMM_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_ITS_COMM_RESET_BTN;
	WndKey = _T("SYSINIT_ITS_COMM_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_ALG_LIB_INIT_LABEL;
	WndKey = _T("SYSINIT_ALG_LIB_INIT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	LabelText = NewLabelText;
	NewLabelText.Format(_T("%s %s"), AOI3D_VENDOR, LabelText);
	CWnd::SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	//Barcode Device Initial
	WndID = SYSINIT_BARCODE_DEVICE_INIT_GROUP;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_INIT_LABEL_1;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_LABEL_1");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_INIT_LABEL_2;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_LABEL_2");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_INIT_LABEL_3;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_LABEL_3");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_INIT_LABEL_4;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_LABEL_4");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_INIT_LABEL_5;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_LABEL_5");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_INIT_LABEL_6;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_LABEL_6");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_INIT_LABEL_7;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_LABEL_7");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_INIT_LABEL_8;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_INIT_LABEL_8");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = SYSINIT_BARCODE_DEVICE_RESET_BTN;
	WndKey = _T("SYSINIT_BARCODE_DEVICE_RESET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CSystemInitialWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_INITIAL_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize()//系統初始化
{	
	double        fnTime=0.0;
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;
	bool         IsOK = false;	
	CString      str = _T("");
	CString      strErr = _T("");
	DWORD        SleepTime = 250;		
	HWND         hWnd = GetSafeHwnd();	
	const BOOL   bSkipMsg = TRUE;
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();

#ifdef OFFLINE_VERSION
	SleepTime = 50;
#endif//OFFLINE_VERSION
	SleepTime = 10;

	//App Version;
	JetAPI::SetFuncTimeStart(fnStart);
	str = ::AfxGetAppName();	
	CWnd::SetDlgItemText(SYSINIT_APPLICATION_VERSION_LABEL, str);
	str.Format(_T("Version:%s"), SysParam.m_AppVersion);
	CWnd::SetDlgItemText(SYSINIT_APPLICATION_VERSION_EDIT, str);
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::AfxGetAppName Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	IsOK = true;
	JetAPI::SetFuncTimeStart(fnStart);
	this->m_strInitial = _T("");

	//影像函式庫初始化
	if ( SystemInitialize_ImageLibrary(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}
	
	//相機初始化
	if ( SystemInitialize_CameraUnit(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}

	//軸控初始化
	if ( SystemInitialize_MotionXYZUnit(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}	

	//PLC初始化
	if ( SystemInitialize_PLCUnit(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}
	
	//燈盤控制初始化
	if ( SystemInitialize_LightCtrlBoard(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}

	//相位控制
	if ( SystemInitialize_PhaseCtrlBoard(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}

	//DTK 函式庫
	if ( SystemInitialize_DTKLibrary(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}	
	
	//Honeywell 函式庫
	if ( SystemInitialize_HonLibrary(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}	

	//ITS通訊
	if ( SystemInitialize_ITS_Comm(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}
	
	//JET Alg函式庫
	if ( SystemInitialize_JetAlgLibrary(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}

	AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_AI_SERVER);	

	//條碼機
	if ( SystemInitialize_BarcodeDevice(SleepTime, strErr) == false )
	{
		IsOK = false;
		if ( m_strInitial.GetLength() == 0 ) 
		{	m_strInitial = strErr; }
		else
		{
			str.Format(_T("%s\n%s"), m_strInitial, strErr);
			m_strInitial = str;
		}				
	}

	AOIDataCollect.ApplyCalibrationParameter();
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_CameraUnit(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;	
	//const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();

	//相機初始化
	JetAPI::SetFuncTimeStart(fnStart);
#ifndef CAMERA_OBJ_DISABLE
	if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_CAMERA) == false )
	{	
		IsOK = false;
		strErr.Format(_T("Camera(%s)"), AOIDataCollect.GetErrorString());
		m_CameraInitImg.SetBitmap(m_LEDRed); 		
	}
	else
	{	m_CameraInitImg.SetBitmap(m_LEDGreen);	}
#else
	m_CameraInitImg.SetBitmap(m_LEDGray);	
	JetAPI::EnableCtrlWnd(this, SYSINIT_CAMERA_RESET_BTN, FALSE);
#endif	

	CAMERA_ID CameraID=PRIMARY_CAMERA_ID;
	str.Format(_T("%dx%d, F:%.2f"), CameraCtrl.GetCameraImageSizeW(CameraID), CameraCtrl.GetCameraImageSizeH(CameraID), CameraCtrl.GetCameraFramePerSecond(CameraID));
	CWnd::SetDlgItemText(SYSINIT_CAMERA_INIT_EDIT, str);
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::Camera Unit Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_MotionXYZUnit(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;	
	//const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();

	//軸控初始化
	JetAPI::SetFuncTimeStart(fnStart);
#ifndef MOTION_OBJ_DISABLE
	if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_MOTION_XYZ) == false )
	{
		IsOK = false;
		strErr.Format(_T("Motion(%s)"), AOIDataCollect.GetErrorString());
		m_MotionInitImg.SetBitmap(m_LEDRed); 		
	}
	else
	{	m_MotionInitImg.SetBitmap(m_LEDGreen);	}
#else
	m_MotionInitImg.SetBitmap(m_LEDGray);
	JetAPI::EnableCtrlWnd(this, SYSINIT_MOTION_RESET_BTN, FALSE);
#endif
	str = MotionCtrlPtr->GetMotionName();
	CWnd::SetDlgItemText(SYSINIT_MOTION_INIT_EDIT, str);
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::Motion Unit Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_PLCUnit(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;	

	//PLC初始化
	JetAPI::SetFuncTimeStart(fnStart);
#ifndef PLC_OBJ_DISABLE
	if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_PLC) == false )
	{
		IsOK = false;
		strErr.Format(_T("PLC(%s)"), AOIDataCollect.GetErrorString());
		m_PlcInitImg.SetBitmap(m_LEDRed); 		
	}
	else
	{	m_PlcInitImg.SetBitmap(m_LEDGreen);	}
#else
	m_PlcInitImg.SetBitmap(m_LEDGray);
	JetAPI::EnableCtrlWnd(this, SYSINIT_PLC_RESET_BTN, FALSE);
#endif
	str = PlcCtrlPtr->GetPLCVersion();	
	CWnd::SetDlgItemText(SYSINIT_PLC_INIT_EDIT, str);
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::PLC Unit Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_LightCtrlBoard(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;

	//燈盤控制初始化
	JetAPI::SetFuncTimeStart(fnStart);
#ifndef LIGHT_CTRL_DISABLE
	if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_LIGHT_CTRL) == false )
	{
		IsOK = false;
		strErr.Format(_T("Light Ctrl(%s)"), AOIDataCollect.GetErrorString());
		m_LightCtrlImg.SetBitmap(m_LEDRed); 		
	}
	else
	{	m_LightCtrlImg.SetBitmap(m_LEDGreen);	}
#else
	m_LightCtrlImg.SetBitmap(m_LEDGray);
	JetAPI::EnableCtrlWnd(this, SYSINIT_LIGHT_CTRL_RESET_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, SYSINIT_LIGHT_CTRL_CONFIG_BTN, FALSE);
#endif

	str = LightCtrlBoard.GetFullVersion();
	CWnd::SetDlgItemText(SYSINIT_LIGHT_CTRL_INIT_EDIT, str);
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::LightCtrlBoard Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_PhaseCtrlBoard(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;

	//相位控制
	JetAPI::SetFuncTimeStart(fnStart);
#ifndef PHASE_CTRL_DISABLE
	if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_PHASE_CTRL) == false )
	{
		IsOK = false;
		strErr.Format(_T("Phase Ctrl(%s)"), AOIDataCollect.GetErrorString());
		m_PhaseCtrlImg.SetBitmap(m_LEDRed); 
	}
	else
	{	m_PhaseCtrlImg.SetBitmap(m_LEDGreen);	}	
#else
	m_PhaseCtrlImg.SetBitmap(m_LEDGray);
	JetAPI::EnableCtrlWnd(this, SYSINIT_PHASE_CTRL_RESET_BTN, FALSE);
	JetAPI::EnableCtrlWnd(this, SYSINIT_PHASE_CTRL_CONFIG_BTN, FALSE);	
#endif		

	str = Light3DCtrl.GetLight3DCastVersion();	
	CWnd::SetDlgItemText(SYSINIT_PHASE_CTRL_INIT_EDIT, str);
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::DLP Unit Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_ImageLibrary(DWORD SleepTime, CString &strErr)
{
	double        fnTime=0.0;
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;
	bool         IsOK = true;
	const size_t TextLen = 128;
	CString      str = _T("");	
	char         ansiText[TextLen]="";
	HWND         hWnd = GetSafeHwnd();	
	const BOOL   bSkipMsg = TRUE;	
	//const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	
	//影像函式庫初始化	
	JetAPI::SetFuncTimeStart(fnStart);	
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
#ifdef EVISION_USE	
	if ( JET_MATCH_LIB_EVS == MatchLibType )
	{
		if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_IMAGE_LIB_EURESYS) == false )
		{
			if ( false == IsOK )
			{
				str.Format(_T("%s, Image Lib(%s)"), m_strInitial, AOIDataCollect.GetErrorString());
				strErr = str;
			}
			else
			{	strErr.Format(_T("Image Lib(%s)"), AOIDataCollect.GetErrorString()); }				
			m_ImageLibImg.SetBitmap(m_LEDRed); 
			IsOK = false;
		}
		else
		{	
			m_ImageLibImg.SetBitmap(m_LEDGreen);
			CEvsAPI::GetVersion(ansiText, TextLen);
			str = ansiText;	
			this->SetDlgItemText(SYSINIT_IMAGE_LIB_INIT_EDIT, str);
		}
	}
#endif//EVISION_USE

#ifdef MIM_LIB_USE
	if ( JET_MATCH_LIB_MIM == MatchLibType )
	{
		if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_IMAGE_LIB_MIM_LIB) == false )
		{
			if ( false == IsOK )
			{
				str.Format(_T("%s, Image Lib(%s)"), m_strInitial, AOIDataCollect.GetErrorString());
				strErr = str;
			}
			else
			{	strErr.Format(_T("Image Lib(%s)"), AOIDataCollect.GetErrorString()); }				
			m_ImageLibImg.SetBitmap(m_LEDRed); 
			IsOK = false;
		}
		else
		{	
			m_ImageLibImg.SetBitmap(m_LEDGreen);
			CMimAPI::GetVersion(ansiText, TextLen);
			str = ansiText;	
			this->SetDlgItemText(SYSINIT_IMAGE_LIB_INIT_EDIT, str);
		}
	}
#endif//MIM_LIB_USE
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::Image Library Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	const bool bTestMatch = false;
	if ( true == bTestMatch )
	{
		CJetMatch    Match;		
		if ( Match.SetMatchLibType(MatchLibType)==true )
		{
			//JetAPI::ShowMessageBox("Match Test-Start");
			bool bIsColor=false;
			bool bRobustness = true;
			int  nMinReduceArea = 64;
			Match.SetMatchDefaultParam();
			Match.SetRobustness(bRobustness);
			Match.SetMinReducedArea(nMinReduceArea);
			Match.LearnPattern("C:\\MIMBug01.BMP", bIsColor);
			//JetAPI::ShowMessageBox("Match Test-End");
		}
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_DTKLibrary(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;		

	//DTK 條碼函式庫
	JetAPI::SetFuncTimeStart(fnStart);
#ifdef DTK_BARCODE_USE
	if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_IMAGE_LIB_DTK_LIB) == false )
	{	
		IsOK = false;
		strErr.Format(_T("DTK Lib(%s)"), AOIDataCollect.GetErrorString());
		m_DTKLibImg.SetBitmap(m_LEDRed); 		
	}
	else
	{	
		m_DTKLibImg.SetBitmap(m_LEDGreen);
		CDtkBarcode::GetDTKLibraryVersion(str);		
		this->SetDlgItemText(SYSINIT_DTK_LIB_INIT_EDIT, str);
	}	
#else
	m_DTKLibImg.SetBitmap(m_LEDGray);
#endif//DTK_BARCODE_USE	
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::DTK Library Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_HonLibrary(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;		
	const bool    HoneywellSwiftDecoderEnabled=AOIDataCollect.CheckHoneywellSwiftDecoderEnabled();
	//Honeywell 條碼函式庫
	JetAPI::SetFuncTimeStart(fnStart);

	if ( true == HoneywellSwiftDecoderEnabled )
	{	
		if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_IMAGE_LIB_HON_LIB) == false )
		{	
			IsOK = false;
			strErr.Format(_T("Honeywell Lib(%s)"), AOIDataCollect.GetErrorString());
			m_HonLibImg.SetBitmap(m_LEDRed); 		
		}
		else
		{	
			m_HonLibImg.SetBitmap(m_LEDGreen);
			HoneywellBarcodeSdk.GetHonLibraryVersion(str);
			this->SetDlgItemText(SYSINIT_HON_LIB_INIT_EDIT, str);
		}	
	}
	else
	{	
		m_HonLibImg.SetBitmap(m_LEDGray);	
		this->SetDlgItemText(SYSINIT_HON_LIB_INIT_EDIT, _T("No License"));
	}

	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::Honeywell Library Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_ITS_Comm(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;		

	//ITS通訊	
	JetAPI::SetFuncTimeStart(fnStart);
#ifndef MES_DISABLE
	if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_ITS_COMM) == false )
	{	
		IsOK = false;
		strErr.Format(_T("ITS(%s)"), AOIDataCollect.GetErrorString());
		m_ITSCommImg.SetBitmap(m_LEDRed);
	}
	else
	{	
		m_ITSCommImg.SetBitmap(m_LEDGreen);
		bool bConnected = MES_OBJ.GetMESConnected();
		if ( true == bConnected ) { str = _T("Connected"); }
		else { str = _T("Disconnected"); }

		CString str2=str;		
		const TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();
		CString Name=AOIDataDefine.GetMESContactSoftwareName(SysParam.m_ITSContactSoftware);
		str.Format(_T("%s [%s]"), str2, Name);
		this->SetDlgItemText(SYSINIT_ITS_COMM_INIT_EDIT, str);
	}
#else
	m_ITSCommImg.SetBitmap(m_LEDGray);
	JetAPI::EnableCtrlWnd(this, SYSINIT_ITS_COMM_RESET_BTN, FALSE);
#endif//MES_DISABLE
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::ITS Unit Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);		
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_JetAlgLibrary(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	CString       ver = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;		

	//Honeywell 條碼函式庫
	JetAPI::SetFuncTimeStart(fnStart);
	m_AlgLibImg.SetBitmap(m_LEDGreen);
	ver = CString(JET::Get_DllVersion_JETAlg());		
	str.Format(_T("%s Alg-v%s"), AOI3D_VENDOR, ver);
	this->SetDlgItemText(SYSINIT_ALG_LIB_INIT_EDIT, str);
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::%s Alg Library Time=%.f ms"), AOI3D_VENDOR, fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemInitialize_BarcodeDevice(DWORD SleepTime, CString &strErr)
{
	LARGE_INTEGER fnStart;
	LARGE_INTEGER fnEnd;	
	double        fnTime=0.0;
	bool          IsOK = true;
	CString       str = _T("");	
	HWND          hWnd = GetSafeHwnd();	
	const BOOL    bSkipMsg = TRUE;		

	//條碼機
	JetAPI::SetFuncTimeStart(fnStart);
	if ( AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_BARCODE_DEVICE) == false )
	{
		IsOK = false;
		strErr.Format(_T("Barcode(%s)"), AOIDataCollect.GetErrorString());		
	}
	UpdateBarcodeDeviceIcon();
	RedrawWnd();
	JetAPI::SleepMessage(SleepTime, bSkipMsg, hWnd);	
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("System Initial::Barcode Unit Time=%.f ms"), fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::SystemRelease()//系統釋放
{
	bool         IsOK = false;
	const size_t TextLen = 128;
	CString      str = _T("");
	DWORD        SleepTime = 250;	
	char         ansiText[TextLen]="";

#ifdef OFFLINE_VERSION
	SleepTime = 50;
#endif//OFFLINE_VERSION
	SleepTime = 10;

	IsOK = true;
	this->m_strInitial = _T("");

	this->m_ImageLibImg.SetBitmap(m_LEDGray);
	this->RedrawWindow();
	::Sleep(SleepTime);
	
	AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_AI_SERVER);

	//MES
#ifndef MES_DISABLE
	if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_ITS_COMM) == false )
	{	
		IsOK = false;
		m_strInitial.Format(_T("ITS(%s)\n"), AOIDataCollect.GetErrorString());
		this->m_ITSCommImg.SetBitmap(m_LEDRed); 
	}
	else
	{	this->m_ITSCommImg.SetBitmap(m_LEDGray);	}
#endif//MES_DISABLE
	this->RedrawWindow();
	::Sleep(SleepTime);

	//相位燈源控制	
#ifndef PHASE_CTRL_DISABLE
	if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_PHASE_CTRL) == false )
	{
		IsOK = false;
		m_strInitial.Format(_T("Camera(%s)\n"), AOIDataCollect.GetErrorString());
		this->m_PhaseCtrlImg.SetBitmap(m_LEDRed); 
	}
	else
	{	this->m_PhaseCtrlImg.SetBitmap(m_LEDGray);	}
#else
	this->m_PhaseCtrlImg.SetBitmap(m_LEDGray);
#endif
	this->RedrawWindow();
	::Sleep(SleepTime);

	//燈盤控制釋放
#ifndef LIGHT_CTRL_DISABLE
	if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_LIGHT_CTRL) == false )
	{
		IsOK = false;
		m_strInitial.Format(_T("Camera(%s)\n"), AOIDataCollect.GetErrorString());
		this->m_LightCtrlImg.SetBitmap(m_LEDRed); 
	}
	else
	{	this->m_LightCtrlImg.SetBitmap(m_LEDGray);	}
#else
	this->m_LightCtrlImg.SetBitmap(m_LEDGray);
#endif
	this->RedrawWindow();
	::Sleep(SleepTime);

	//PLC釋放
#ifndef PLC_OBJ_DISABLE
	if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_PLC) == false )
	{
		IsOK = false;
		m_strInitial.Format(_T("Camera(%s)\n"), AOIDataCollect.GetErrorString());
		this->m_PlcInitImg.SetBitmap(m_LEDRed); 
	}
	else
	{	this->m_PlcInitImg.SetBitmap(m_LEDGray);	}
#else
	this->m_PlcInitImg.SetBitmap(m_LEDGray);
#endif	
	this->RedrawWindow();
	::Sleep(SleepTime);

	//軸控釋放
#ifndef MOTION_OBJ_DISABLE
	if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_MOTION_XYZ) == false )
	{
		IsOK = false;
		m_strInitial.Format(_T("Camera(%s)\n"), AOIDataCollect.GetErrorString());
		this->m_MotionInitImg.SetBitmap(m_LEDRed); 
	}
	else
	{	this->m_MotionInitImg.SetBitmap(m_LEDGray);	}
#else
	this->m_MotionInitImg.SetBitmap(m_LEDGray);
#endif	
	this->RedrawWindow();
	::Sleep(SleepTime);	

	//相機釋放
#ifndef CAMERA_OBJ_DISABLE
	if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_CAMERA) == false )
	{	
		IsOK = false;
		m_strInitial.Format(_T("Camera(%s)\n"), AOIDataCollect.GetErrorString());
		this->m_CameraInitImg.SetBitmap(m_LEDRed); 
	}
	else
	{	this->m_CameraInitImg.SetBitmap(m_LEDGray);	}
#else
	this->m_CameraInitImg.SetBitmap(m_LEDGray);
#endif
	this->RedrawWindow();
	::Sleep(SleepTime);

	//DTK函式庫釋放
#ifdef DTK_BARCODE_USE
	if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_IMAGE_LIB_DTK_LIB) == false )
	{	
		IsOK = false;
		m_strInitial.Format(_T("DTK Lib(%s)\n"), AOIDataCollect.GetErrorString());
		this->m_DTKLibImg.SetBitmap(m_LEDRed); 
	}
	else
	{	this->m_DTKLibImg.SetBitmap(m_LEDGray);	}
#else
	this->m_DTKLibImg.SetBitmap(m_LEDGray);
#endif//DTK_BARCODE_USE
	this->RedrawWindow();
	::Sleep(SleepTime);

	//Honeywell函式庫釋放
	if ( AOIDataCollect.CheckHoneywellSwiftDecoderEnabled() )
	{
		if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_IMAGE_LIB_HON_LIB) == false )
		{	
			IsOK = false;
			m_strInitial.Format(_T("Honeywell Lib(%s)\n"), AOIDataCollect.GetErrorString());
			this->m_HonLibImg.SetBitmap(m_LEDRed); 
		}
		else
		{	this->m_HonLibImg.SetBitmap(m_LEDGray);	}
	}
	else
	{	this->m_HonLibImg.SetBitmap(m_LEDGray);	}
	this->RedrawWindow();
	::Sleep(SleepTime);

	//JET Alg函式庫
	m_AlgLibImg.SetBitmap(m_LEDGray);
	this->RedrawWindow();
	::Sleep(SleepTime);

	//條碼機
	if ( AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_BARCODE_DEVICE) == false )
	{
		IsOK = false;
		m_strInitial.Format(_T("DTK Lib(%s)\n"), AOIDataCollect.GetErrorString());
	}
	UpdateBarcodeDeviceIcon();		

	this->RedrawWindow();
	::Sleep(SleepTime);
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::UpdateBarcodeDeviceIcon()
{
	CBarcode_Basic *BarcodeDevicePtr = NULL;
	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(BARCODE_DEVICE_ID_01);//01
	UpdateBarcodeDeviceIconKernel(BarcodeDevicePtr, m_BarcodeDeviceIcon1, SYSINIT_BARCODE_DEVICE_INIT_EDIT_1);

	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(BARCODE_DEVICE_ID_02);//02
	UpdateBarcodeDeviceIconKernel(BarcodeDevicePtr, m_BarcodeDeviceIcon2, SYSINIT_BARCODE_DEVICE_INIT_EDIT_2);

	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(BARCODE_DEVICE_ID_03);//03
	UpdateBarcodeDeviceIconKernel(BarcodeDevicePtr, m_BarcodeDeviceIcon3, SYSINIT_BARCODE_DEVICE_INIT_EDIT_3);

	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(BARCODE_DEVICE_ID_04);//04
	UpdateBarcodeDeviceIconKernel(BarcodeDevicePtr, m_BarcodeDeviceIcon4, SYSINIT_BARCODE_DEVICE_INIT_EDIT_4);

	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(BARCODE_DEVICE_ID_05);//05
	UpdateBarcodeDeviceIconKernel(BarcodeDevicePtr, m_BarcodeDeviceIcon5, SYSINIT_BARCODE_DEVICE_INIT_EDIT_5);

	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(BARCODE_DEVICE_ID_06);//06
	UpdateBarcodeDeviceIconKernel(BarcodeDevicePtr, m_BarcodeDeviceIcon6, SYSINIT_BARCODE_DEVICE_INIT_EDIT_6);

	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(BARCODE_DEVICE_ID_07);//07
	UpdateBarcodeDeviceIconKernel(BarcodeDevicePtr, m_BarcodeDeviceIcon7, SYSINIT_BARCODE_DEVICE_INIT_EDIT_7);

	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(BARCODE_DEVICE_ID_08);//08
	UpdateBarcodeDeviceIconKernel(BarcodeDevicePtr, m_BarcodeDeviceIcon8, SYSINIT_BARCODE_DEVICE_INIT_EDIT_8);

#ifdef BARCODE_DEVICE_DISABLE
	JetAPI::EnableCtrlWnd(this, SYSINIT_BARCODE_DEVICE_RESET_BTN, FALSE);
#endif//BARCODE_DEVICE_DISABLE
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::UpdateBarcodeDeviceIconKernel(CBarcode_Basic *Ptr, CStatic &Icon, UINT EditID)
{
	if ( NULL == Ptr )
	{	
		Icon.SetBitmap(m_LEDGray);	
		CWnd::SetDlgItemText(EditID, _T(""));
	}
	else
	{
		CString DeviceName;
		DeviceName = Ptr->GetBarcodeDeviceFullName();
		CWnd::SetDlgItemText(EditID, DeviceName);
		if ( Ptr->GetBarcodeDeviceConnected() == true ) 
		{	Icon.SetBitmap(m_LEDGreen);	}
		else
		{	Icon.SetBitmap(m_LEDRed);	}
	}
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::ExecCameraResetBtn()
{
#ifndef CAMERA_OBJ_DISABLE
	CString strRes;
	bool    IsOK = true;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;	
	m_CameraInitImg.SetBitmap(m_LEDRed); 
	SetDlgItemText(SYSINIT_CAMERA_INIT_EDIT, _T(""));
	IsOK = AOIDataCollect.SystemReleaseStep(SYSTEM_INITIAL_CAMERA);
	if (false == IsOK)
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	IsOK = AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_CAMERA);
	if (false == IsOK)
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return false;
	}

	m_CameraInitImg.SetBitmap(m_LEDGreen);	
	strRes.Format(_T("%dx%d, F:%.2f"), CameraCtrl.GetCameraImageSizeW(CameraID), CameraCtrl.GetCameraImageSizeH(CameraID), CameraCtrl.GetCameraFramePerSecond(CameraID));
	SetDlgItemText(SYSINIT_CAMERA_INIT_EDIT, strRes);	
#endif//CAMERA_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnCameraResetBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bSucc = ExecCameraResetBtn();	
	return;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::ExecMotionResetBtn()
{
#ifndef MOTION_OBJ_DISABLE
	CString str;
	CString strRes;
	bool    IsOK = true;	
	m_MotionInitImg.SetBitmap(m_LEDRed); 
	strRes = MotionCtrlPtr->GetMotionName();
	SetDlgItemText(SYSINIT_MOTION_INIT_EDIT, strRes);
	IsOK = AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_MOTION_XYZ);
	if (false == IsOK)
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return false;
	}
	m_MotionInitImg.SetBitmap(m_LEDGreen);

	const bool bPcbInsideLA=PlcCtrlPtr->CheckPCBInside_LA();
	const bool bPcbInsideLB=PlcCtrlPtr->CheckPCBInside_LB();
	if ( true==bPcbInsideLA || true==bPcbInsideLB )
	{	str = _T("PCB Inside, Do you want to Home XYZ?");	}
	else
	{	str = _T("Do you want to Home XYZ?");	}
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{
		if ( MotionCtrlPtr->ExecHomeAll(true) == false )
		{	str = MotionCtrlPtr->GetErrorString();	}
		else
		{	str = AOIDataDefine.GetFinishText(); }
		JetAPI::ShowMessageBox(str);
	}
#endif//MOTION_OBJ_DISABLE		
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnMotionResetBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bSucc = ExecMotionResetBtn();	
	return;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnPlcResetBtn() 
{
	// TODO: Add your control notification handler code here	
#ifndef PLC_OBJ_DISABLE
	CString strRes;
	bool    IsOK = true;	
	m_PlcInitImg.SetBitmap(m_LEDRed); 
	SetDlgItemText(SYSINIT_PLC_INIT_EDIT, _T(""));
	IsOK = AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_PLC);
	if (false == IsOK)
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return;
	}		
	m_PlcInitImg.SetBitmap(m_LEDGreen);
	strRes = PlcCtrlPtr->GetPLCVersion();	
	SetDlgItemText(SYSINIT_PLC_INIT_EDIT, strRes);
#endif//PLC_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::ExecLightCtrlResetBtn()
{
#ifndef LIGHT_CTRL_DISABLE
	CString strRes;
	bool    IsOK = true;	
	m_LightCtrlImg.SetBitmap(m_LEDRed); 
	SetDlgItemText(SYSINIT_LIGHT_CTRL_INIT_EDIT, _T(""));
	::Sleep(500);
	IsOK = AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_LIGHT_CTRL);
	if (false == IsOK)
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return false;
	}		
	m_LightCtrlImg.SetBitmap(m_LEDGreen);
	strRes = LightCtrlBoard.GetFullVersion();
	SetDlgItemText(SYSINIT_LIGHT_CTRL_INIT_EDIT, strRes);
#endif//LIGHT_CTRL_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnLightCtrlResetBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bSucc = ExecLightCtrlResetBtn();	
	return;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnLightCtrlConfigBtn()
{
	// TODO: Add your control notification handler code here	
#ifndef LIGHT_CTRL_DISABLE	
	if ( AOIDataCollect.OperateLevel_Supervisor() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;

	}
	TListNode       Node;
	CString         strLabel;
	CString         strCaption;
	CInputListWnd   EnumWnd;	
	DWORD_PTR       dwDefault=LightCtrlBoard.GetLightCtrlBoardType();
	std::vector<TListNode> NodelList;
	strLabel = _T("Select Type");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Change Light Control Board Type");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	Node.Data = LIGHT_CTRL_BOARD_3DA6;
	Node.Text.Format(_T("[%d] %s"), Node.Data, _T("V6"));	
	//NodelList.push_back(Node);
	Node.Data = LIGHT_CTRL_BOARD_8DA1;
	Node.Text.Format(_T("[%d] %s"), Node.Data, _T("V6"));			
	NodelList.push_back(Node);
	Node.Data = LIGHT_CTRL_BOARD_ARDUINO;
	Node.Text.Format(_T("[%d] %s"), Node.Data, _T("V7"));	
	NodelList.push_back(Node);	
	EnumWnd.SetParam1(strCaption, strLabel, dwDefault, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	
	LIGHT_CTRL_BOARD_TYPE NewType = (LIGHT_CTRL_BOARD_TYPE)(EnumWnd.GetSelData());		
	if ( dwDefault == NewType ) 
	{	return; }
	if ( LightCtrlBoard.ChangeLightCtrlBoardType(NewType) == false )
	{
		JetAPI::ShowMessageBox(LightCtrlBoard.GetErrorString());
		return ;
	}
	CWnd::PostMessage(WM_COMMAND, SYSINIT_LIGHT_CTRL_RESET_BTN, NULL);
#endif//LIGHT_CTRL_DISABLE	
	return;
}
//-------------------------------------------------------------------------------------//
bool CSystemInitialWnd::ExecPhaseCtrlResetBtn()
{
#ifndef PHASE_CTRL_DISABLE
	CString strRes;
	bool    IsOK = true;	
	m_PhaseCtrlImg.SetBitmap(m_LEDRed); 
	SetDlgItemText(SYSINIT_PHASE_CTRL_INIT_EDIT, _T(""));
	IsOK = AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_PHASE_CTRL);
	if (false == IsOK)
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return false;
	}		
	m_PhaseCtrlImg.SetBitmap(m_LEDGreen);
	strRes = Light3DCtrl.GetLight3DCastVersion();	
	SetDlgItemText(SYSINIT_PHASE_CTRL_INIT_EDIT, strRes);
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnPhaseCtrlResetBtn() 
{
	// TODO: Add your control notification handler code here	
	const bool bSucc = ExecPhaseCtrlResetBtn();	
	return;
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnPhaseCtrlConfigBtn() 
{
	// TODO: Add your control notification handler code here	
#ifndef PHASE_CTRL_DISABLE
	if ( AOIDataCollect.OperateLevel_Supervisor() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;

	}
	TListNode       Node;
	CString         strLabel;
	CString         strCaption;
	CInputListWnd   EnumWnd;	
	DWORD_PTR       dwDefault=Light3DCtrl.GetLight3DDeviceType(LIGHT_3D_CAST_01);
	std::vector<TListNode> NodelList;
	strLabel = _T("Select Type");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strCaption = _T("Change Light 3D Device Type");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	
	Node.Data = LIGHT_3D_DEVICE_DLP4500;
	Node.Text.Format(_T("[%d] %s"), Node.Data, _T("DLP4500"));
	NodelList.push_back(Node);
	Node.Data = LIGHT_3D_DEVICE_DLP4710;
	Node.Text.Format(_T("[%d] %s"), Node.Data, _T("DLP4710"));	
	NodelList.push_back(Node);	
	EnumWnd.SetParam1(strCaption, strLabel, dwDefault, NodelList);	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	

	LIGHT_3D_DEVICE_TYPE NewType = (LIGHT_3D_DEVICE_TYPE)(EnumWnd.GetSelData());		
	if ( dwDefault == NewType ) 
	{	return; }
	if ( Light3DCtrl.ChangeLight3DDeviceType(NewType) == false )
	{
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return ;
	}
	CWnd::PostMessage(WM_COMMAND, SYSINIT_PHASE_CTRL_RESET_BTN, NULL);
#endif//PHASE_CTRL_DISABLE
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnITSCommResetBtn() 
{
	// TODO: Add your control notification handler code here		
#ifndef MES_DISABLE
	CString strRes;
	bool    IsOK = true;	
	m_ITSCommImg.SetBitmap(m_LEDRed); 
	SetDlgItemText(SYSINIT_ITS_COMM_INIT_EDIT, _T(""));
	IsOK = AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_ITS_COMM);
	if (false == IsOK)
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return;
	}		
	m_ITSCommImg.SetBitmap(m_LEDGreen);
	bool bConnected = MES_OBJ.GetMESConnected();
	if ( true == bConnected ) { strRes = _T("Connected"); }
	else { strRes = _T("Disconnected"); }
	CString str2=strRes;		
	const TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();
	CString Name=AOIDataDefine.GetMESContactSoftwareName(SysParam.m_ITSContactSoftware);
	strRes.Format(_T("%s [%s]"), str2, Name);
	SetDlgItemText(SYSINIT_ITS_COMM_INIT_EDIT, strRes);
#endif//MES_DISABLE	
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnBarcodeDeviceResetBtn()
{
	// TODO: Add your control notification handler code here		
#ifndef BARCODE_DEVICE_DISABLE	
	bool   IsOK = true;
	IsOK = AOIDataCollect.SystemInitializeStep(SYSTEM_INITIAL_BARCODE_DEVICE);
	if (false == IsOK)
	{	
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	
		return;
	}
	UpdateBarcodeDeviceIcon();
#endif//BARCODE_DEVICE_DISABLE
}
//-------------------------------------------------------------------------------------//
void CSystemInitialWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//