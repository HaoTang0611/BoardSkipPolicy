// JET8000.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "JET8000.h"

#include "MainFrm.h"
#include "MainDoc.h"
#include "EditFormView.h"
#include "EditModelView.h"
#include "OnlineFormView.h"
#include "OnlineFormView_Dual.h"
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJET8000App

BEGIN_MESSAGE_MAP(CJET8000App, CBasicApp)
	//{{AFX_MSG_MAP(CJET8000App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CBasicApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CBasicApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CBasicApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJET8000App construction
//-------------------------------------------------------------------------------------//
CJET8000App::CJET8000App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance		
	this->m_Mutex = NULL;
	this->m_bHiColorIcons = TRUE;

#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	// 支援重新啟動管理員
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

#ifdef _MANAGED
	// 如果應用程式是使用 Common Language Runtime 支援 (/clr) 建置的:
	//     1) 要使重新啟動管理員支援正常運作需要這個額外設定。
	//     2) 在專案中必須將參考加入至 System.Windows.Forms 才能進行建置。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif//_MANAGED

	// 字串格式為 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UIApp.AppID.NoVersion"));
#endif//FRAME_STYLE_TYPE

#if  FRAME_STYLE_TYPE == FRAME_STYLE_STUDIO
	this->m_nAppLook = ID_VIEW_APPLOOK_VS_2008;
#elif  FRAME_STYLE_TYPE == FRAME_STYLE_OFFICE
	this->m_nAppLook = ID_VIEW_APPLOOK_OFF_2007_BLACK;
#else
	this->m_nAppLook = ID_VIEW_APPLOOK_WIN_XP;
#endif//FRAME_STYLE_TYPE

	CString FullName;
	CString OsVer=_T("");
	CString AppName=_T("");
#ifdef _X64
	OsVer = _T("x64");	
#else
	OsVer = _T("x32");
#endif//_X64

#ifdef DISABLE_3D
	AppName = _T("JET7800");
#else
	AppName = _T("JET8000");
#endif//DISABLE_3D

#ifdef ODM_BRAND_VERSION	
	AppName = AOI3D_APP_NAME;
#endif//ODM_BRAND_VERSION

#ifdef LABORATORY_VERSION
	AppName += _T("_Lab");
#endif//LABORATORY_VERSION

#ifdef TB_SYSTEM_ONLY_BOT
	AppName += _T("B");
#endif//TB_SYSTEM_ONLY_BOT

#ifdef OFFLINE_VERSION	
	AppName += _T("_Offline");	
#endif//OFFLINE_VERSION	

	FullName.Format(_T("%s(%s)"), AppName, OsVer);
	m_pszAppName=_tcsdup(FullName);
	return;
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// The one and only CJET8000App object
//-------------------------------------------------------------------------------------//
CJET8000App theApp;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CJET8000App initialization
//-------------------------------------------------------------------------------------//
BOOL CJET8000App::InitInstance()
{	
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	// 假如應用程式資訊清單指定使用 ComCtl32.dll 6 (含) 以後版本，
	// 來啟動視覺化樣式，在 Windows XP 上，則需要 InitCommonControls()。
	// 否則任何視窗的建立都將失敗。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 設定要包含所有您想要用於應用程式中的
	// 通用控制項類別。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBasicApp::InitInstance();	
	if (!AfxSocketInit())
	{
		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		AfxMessageBox(_T("Error, AfxSocketInit Fault"));
		return FALSE;
	}

	// 初始化 OLE 程式庫
	if (!AfxOleInit())
	{
		//AfxMessageBox(IDP_OLE_INIT_FAILED);
		AfxMessageBox(_T("Error, AfxOleInit Fault"));
		return FALSE;
	}
#endif//FRAME_STYLE_TYPE

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#if _MSC_VER <= VC_6
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
#endif//_MSC_VER

#ifndef OFFLINE_VERSION
	//this->m_Mutex = ::CreateMutex(NULL, TRUE, _T("JET8000_Inline"));
	this->m_Mutex = ::CreateMutex(NULL, TRUE, AOI3D_APP_CAT("_Inline"));
	if ( ::GetLastError() == ERROR_ALREADY_EXISTS )
	{
		//::AfxMessageBox(_T("JET8000 Online Application Exist"));
		::AfxMessageBox(AOI3D_APP_CAT(" Online Application Exist"));
		//::ReleaseMutex(m_Mutex);//此時的Multex是前一個JET8000所產生的，不要在這裡刪除
		::CloseHandle(m_Mutex);
		m_Mutex=NULL;
		return FALSE;
	}	
#else
	//this->m_Mutex = ::CreateMutex(NULL, TRUE, _T("JET8000_Offline"));
	this->m_Mutex = ::CreateMutex(NULL, TRUE, AOI3D_APP_CAT("_Offline"));
	if ( ::GetLastError() == ERROR_ALREADY_EXISTS )
	{
		//::AfxMessageBox(_T("JET8000 Offine Application Exist"));
		::AfxMessageBox(AOI3D_APP_CAT(" Offine Application Exist"));
		//::ReleaseMutex(m_Mutex);//此時的Multex是前一個JET8000所產生的，不要在這裡刪除
		::CloseHandle(m_Mutex);
		m_Mutex=NULL;
		return FALSE;
	}
#endif//OFFLINE_VERSION

	if ( AOIDataCollect.OpenJetDongle() == false )
	{
		CString str;
		str = AOIDataCollect.GetErrorString();
		::AfxMessageBox(str);
		::CloseHandle(m_Mutex);
		m_Mutex=NULL;
		return FALSE;
	}
	if ( AOIDataCollect.CheckJetDongleWarning() == false )
	{
		CString str;
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	//m_bSaveState = FALSE;
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC

	InitContextMenuManager();
	InitShellManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

#endif//FRAME_STYLE_TYPE


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	ONLINE_FROMVIEW_MODE OnlineFormViewMode = AOIDataCollect.GetOnlineFormViewMode();
	if ( ONLINE_FROMVIEW_DUAL == OnlineFormViewMode )
	{	
		pDocTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CMainFrame),       // main SDI frame window
			RUNTIME_CLASS(COnlineFormView_Dual));
	}
	else
	{
		pDocTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(CMainDoc),
			RUNTIME_CLASS(CMainFrame),       // main SDI frame window
			RUNTIME_CLASS(COnlineFormView));
	}
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);	
	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//-------------------------------------------------------------------------------------//
class CAboutDlg : public CBaseDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//
// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
CAboutDlg::CAboutDlg() : CBaseDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAboutDlg, CBaseDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
void CAboutDlg::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ABOUTBOX");
	CString AppName = AOI3D_APP_NAME;//::AfxGetAppName();	
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	//---------------------------------------------------------------------------------//
	WndID = IDD_ABOUTBOX;
	WndKey = _T("IDD_ABOUTBOX");	
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	LabelText.Format(_T("%s %s"), NewLabelText, AppName);
	this->SetWindowText(LabelText);
	//---------------------------------------------------------------------------------//
	WndID = ABT_VERSION_LABEL;
	WndKey = _T("ABT_VERSION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	NewLabelText.Format(_T("%s Version %s"), AppName, SysParam.m_AppVersion);
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = ABT_COPYRIGHT_LABEL;
	WndKey = _T("ABT_COPYRIGHT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	NewLabelText = CString(AOI3D_COPYRIGHTS_A);
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	i = 0;
}
//-------------------------------------------------------------------------------------//
BOOL CAboutDlg::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
// App command to run the dialog
void CJET8000App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
//-------------------------------------------------------------------------------------//
// CUIAppApp 自訂載入/儲存方法
void CJET8000App::PreLoadState()
{
	/*
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
	*/
}
//-------------------------------------------------------------------------------------//
void CJET8000App::LoadCustomState()
{
}
//-------------------------------------------------------------------------------------//
void CJET8000App::SaveCustomState()
{
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CJET8000App message handlers
//-------------------------------------------------------------------------------------//
int CJET8000App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
#if FRAME_STYLE_TYPE != FRAME_STYLE_MFC
	AfxOleTerm(FALSE);
#endif//FRAME_STYLE_TYPE

	if ( this->m_Mutex != NULL )
	{
		::ReleaseMutex(m_Mutex);
		::CloseHandle(m_Mutex);
		this->m_Mutex=NULL;
	}
	return CBasicApp::ExitInstance();
}
//-------------------------------------------------------------------------------------//
BOOL CJET8000App::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CBasicApp::OnIdle(lCount);

	BOOL bMore = CBasicApp::OnIdle(lCount);
	if ( TRUE == bMore )
	{	return TRUE; }

	return bMore;
}
//-------------------------------------------------------------------------------------//