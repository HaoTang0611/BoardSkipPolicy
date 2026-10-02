// CalibrationWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "CalibrationWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCalibrationWnd dialog
//-------------------------------------------------------------------------------------//
CCalibrationWnd::CCalibrationWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CCalibrationWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalibrationWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_MaskBuffer = NULL;//標誌記憶體指標
	m_MaskBuffer1 = NULL;//標誌記憶體指標
	m_MaskBuffer2 = NULL;//標誌記憶體指標

	m_PhaseBuffer = NULL;
	m_PhaseBuffer1 = NULL;
	m_PhaseBuffer2 = NULL;
	m_ImageBuffer = NULL;
	m_ImageBuffer1 = NULL;	
	m_ImageBuffer2 = NULL;	
	m_ImageBufferSize = 0;
	m_ShowBuffer = NULL;	
	m_ShowBuffer1 = NULL;
	m_ShowBufferSize = 0;
	m_SpaceBuffer = NULL;//空間記憶體指標	
	m_SpaceBuffer1 = NULL;//空間記憶體指標-1
	m_SpaceBuffer2 = NULL;//空間記憶體指標-1
	m_SpaceBufferSize = 0;//空間記憶體尺寸	
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalibrationWnd)
	DDX_Control(pDX, CALIWND_PANE_TAB, m_PaneTabWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCalibrationWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CCalibrationWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(CALIWND_TARGET_SETTING_BTN, OnTargetSettingBtn)
	ON_BN_CLICKED(CALIWND_SAVE_CALIBRATION_BTN, OnSaveCalibrationBtn)
	ON_NOTIFY(TCN_SELCHANGE, CALIWND_PANE_TAB, OnSelchangePaneTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCalibrationWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CCalibrationWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//---------------------------------------------------------------------------------//
	CreateImageBuffer();
	CreateShowBuffer();
	CreateSpaceBuffer();
	//---------------------------------------------------------------------------------//	
	m_CaliPaneAlign.SetSpaceBuffer(m_SpaceBufferSize, m_SpaceBuffer, m_SpaceBuffer1, m_SpaceBuffer2);
	m_CaliPaneAlign.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer, m_ShowBuffer1);
	m_CaliPaneAlign.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer, m_ImageBuffer1, m_ImageBuffer2, m_PhaseBuffer, m_PhaseBuffer1, m_PhaseBuffer2, m_MaskBuffer, m_MaskBuffer1, m_MaskBuffer2);
	//---------------------------------------------------------------------------------//
	m_CaliPaneStage.SetSpaceBuffer(m_SpaceBufferSize, m_SpaceBuffer, m_SpaceBuffer1);
	m_CaliPaneStage.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer, m_ShowBuffer1);
	m_CaliPaneStage.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer, m_ImageBuffer1, m_ImageBuffer2, m_PhaseBuffer, m_PhaseBuffer1, m_PhaseBuffer2, m_MaskBuffer);
	//---------------------------------------------------------------------------------//
	m_CaliPaneDynamic.SetSpaceBuffer(m_SpaceBufferSize, m_SpaceBuffer, m_SpaceBuffer1);
	m_CaliPaneDynamic.SetShowBuffer(m_ShowBufferSize, m_ShowBuffer, m_ShowBuffer1);
	m_CaliPaneDynamic.SetImageBuffer(m_ImageBufferSize, m_ImageBuffer, m_ImageBuffer1, m_ImageBuffer2, m_PhaseBuffer, m_PhaseBuffer1, m_PhaseBuffer2, m_MaskBuffer);
	//---------------------------------------------------------------------------------//
	m_CaliPaneTargetPos.Create(IDD_CALIBRATION_PANE_TARGET_SETTING, this);
	m_CaliPaneAlign.Create(IDD_CALIBRATION_PANE_ALIGN, this);
	m_CaliPaneStage.Create(IDD_CALIBRATION_PANE_STAGE, this);		
	m_CaliPaneDynamic.Create(IDD_CALIBRATION_PANE_DYNAMIC, this);		
	ShowWindow(SW_SHOWMAXIMIZED);

	CString str;
	int    tcIndex=0;
	TCITEM tcItem;	
	TCHAR   tcBuffer[MAX_JET_PATH]=_T("");
	::memset(&tcItem, 0x00, sizeof(tcItem));	
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	tcItem.pszText = tcBuffer;
	
	str = _T("Align");
	str = LoadMultiLanguageString(str, str);
	::_tcscpy(tcBuffer, str);		
	tcItem.lParam = (LPARAM)(&m_CaliPaneAlign);
	m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
	tcIndex ++;
	
	str = _T("Stage");
	str = LoadMultiLanguageString(str, str);
	::_tcscpy(tcBuffer, str);		
	tcItem.lParam = (LPARAM)(&m_CaliPaneStage);
	m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
	tcIndex ++;	

	str = _T("Dynamic");
	str = LoadMultiLanguageString(str, str);
	::_tcscpy(tcBuffer, str);		
	tcItem.lParam = (LPARAM)(&m_CaliPaneDynamic);
	m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
	tcIndex ++;	
	
//	tcItem.pszText = _T("Param");	
//	tcItem.lParam = (LPARAM)(&m_MotionPaneParam);
//	this->m_PaneTabWnd.InsertItem(tcIndex, &tcItem);	
//	tcIndex ++;
	SwitchMultiLanguage();
	DoSelchangePaneTabWnd();
	
	m_MotionJobMode = MotionCtrlPtr->GetIsJogMode();
	MotionCtrlPtr->SetIsJogMode(true);
	MotionCtrlPtr->SetStopXYCalibration(true);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( NULL == this->m_PaneTabWnd.GetSafeHwnd() ) { return; }
	RECT  WndRect={0};
	SIZE  WndSize={0};
	CWnd *pWnd = NULL;
	WndRect.right = cx;
	WndRect.bottom = cy-40;
	this->m_PaneTabWnd.MoveWindow(&WndRect);

	pWnd = this->GetDlgItem(IDOK);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;	
		WndSize.cy = WndRect.bottom-WndRect.top;

		WndRect.bottom = cy-8;
		WndRect.top    = WndRect.bottom-WndSize.cy;
		pWnd->MoveWindow(&WndRect);		
	}

	pWnd = this->GetDlgItem(IDCANCEL);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;	
		WndSize.cy = WndRect.bottom-WndRect.top;

		WndRect.bottom = cy-8;
		WndRect.top    = WndRect.bottom-WndSize.cy;
		pWnd->MoveWindow(&WndRect);		
	}
	pWnd = this->GetDlgItem(CALIWND_TARGET_SETTING_BTN);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;	
		WndSize.cy = WndRect.bottom-WndRect.top;

		WndRect.bottom = cy-8;
		WndRect.top    = WndRect.bottom-WndSize.cy;
		pWnd->MoveWindow(&WndRect);		
	}
	pWnd = this->GetDlgItem(CALIWND_SAVE_CALIBRATION_BTN);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndSize.cx = WndRect.right-WndRect.left;	
		WndSize.cy = WndRect.bottom-WndRect.top;

		WndRect.bottom = cy-8;
		WndRect.top    = WndRect.bottom-WndSize.cy;
		pWnd->MoveWindow(&WndRect);		
	}	
	this->AdjustPaneWndPosition();
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	DestroyImageBuffer();
	DestroyShowBuffer();
	DestroySpaceBuffer();
	CameraCtrl.ResetBatchGrabbing();
	MotionCtrlPtr->SetStopXYCalibration(false);
	MotionCtrlPtr->SetIsJogMode(m_MotionJobMode);	
}
//-------------------------------------------------------------------------------------//
void  CCalibrationWnd::AdjustPaneWndPosition()
{	
	if ( m_PaneTabWnd.GetSafeHwnd() == NULL ) { return; }
	RECT PaneWndRect={0};
	this->m_PaneTabWnd.GetClientRect(&PaneWndRect);
	this->m_PaneTabWnd.ClientToScreen(&PaneWndRect);
	this->ScreenToClient(&PaneWndRect);
	PaneWndRect.top += 24; 
	PaneWndRect.left += 4;
	PaneWndRect.right -= 4;
	PaneWndRect.bottom -= 4;
	if ( m_CaliPaneAlign.GetSafeHwnd() != NULL )
	{	m_CaliPaneAlign.MoveWindow(&PaneWndRect);	}
	if ( m_CaliPaneStage.GetSafeHwnd() != NULL )
	{	m_CaliPaneStage.MoveWindow(&PaneWndRect);	}
	if ( m_CaliPaneDynamic.GetSafeHwnd() != NULL )
	{	m_CaliPaneDynamic.MoveWindow(&PaneWndRect);	}
	
//	if ( this->m_MotionPaneParam.GetSafeHwnd() != NULL )
//	{	this->m_MotionPaneParam.MoveWindow(&PaneWndRect);	}
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::DoSelchangePaneTabWnd()
{
	int    i = 0;	
	CWnd  *pWnd = NULL;
	CString WndText;
	TCITEM tcItem;
	TCHAR  tcBuffer[MAX_JET_PATH]=_T("");	
	::memset(&tcItem, 0x00, sizeof(tcItem));
	tcItem.mask = TCIF_TEXT|TCIF_PARAM;
	tcItem.pszText = tcBuffer;
	tcItem.cchTextMax = MAX_JET_PATH;
	const int TabCount = this->m_PaneTabWnd.GetItemCount();
	const int TabIndex = this->m_PaneTabWnd.GetCurSel();
	//Hide Pane
	for ( i=0; i<TabCount; i++ )
	{
		if ( i == TabIndex ) { continue; }
		this->m_PaneTabWnd.GetItem(i, &tcItem);
		pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == FALSE ) { continue; }	
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { continue; }
		if ( pWnd->IsWindowVisible() == FALSE ) { continue; }
		pWnd->ShowWindow(SW_HIDE);
	}

	//Show Pane
	if ( TabIndex >= 0 )
	{
		this->m_PaneTabWnd.GetItem(TabIndex, &tcItem);
		pWnd = (CWnd*)(tcItem.lParam);
		if ( pWnd->IsKindOf(RUNTIME_CLASS(CWnd)) == FALSE ) { return; }
		if ( (NULL==pWnd) || (NULL==pWnd->GetSafeHwnd()) ) { return; }
		if ( m_WndText.GetLength() > 0 )
		{	WndText.Format(_T("%s-%s"), m_WndText, tcItem.pszText); }
		else
		{	WndText = tcItem.pszText; }
		CWnd::SetWindowText(WndText);
		pWnd->ShowWindow(SW_SHOW);		
	}
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::SwitchMultiLanguage()
{		
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_CALIBRATION_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_CALIBRATION_WND;
	WndKey = _T("IDD_CALIBRATION_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	m_WndText = NewLabelText;
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CALIWND_TARGET_SETTING_BTN;
	WndKey = _T("CALIWND_TARGET_SETTING_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CALIWND_SAVE_CALIBRATION_BTN;
	WndKey = _T("CALIWND_SAVE_CALIBRATION_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	
}
//-------------------------------------------------------------------------------------//
CString CCalibrationWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_CALIBRATION_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CCalibrationWnd::CreateImageBuffer()//建立影像資料
{	
	const char fnName[] = "CCalibrationWnd::CreateImageBuffer";
	this->DestroyImageBuffer();	
	const size_t BufferSize = CameraCtrl.GetMaxColorImageSize();	
	
	if ( JetMemory.alloc_func(BufferSize, this->m_MaskBuffer,  fnName, "m_MaskBuffer")== false ||
		 JetMemory.alloc_func(BufferSize, this->m_MaskBuffer1,  fnName, "m_MaskBuffer1")== false ||
		 JetMemory.alloc_func(BufferSize, this->m_MaskBuffer2,  fnName, "m_MaskBuffer2")== false ||
		 JetMemory.alloc_func(BufferSize, this->m_PhaseBuffer,  fnName, "m_PhaseBuffer")== false ||
		 JetMemory.alloc_func(BufferSize, this->m_PhaseBuffer1, fnName, "m_PhaseBuffer1")== false ||
		 JetMemory.alloc_func(BufferSize, this->m_PhaseBuffer2, fnName, "m_PhaseBuffer2")== false ||
		 JetMemory.alloc_func(BufferSize, this->m_ImageBuffer,  fnName, "m_ImageBuffer")== false ||
		 JetMemory.alloc_func(BufferSize, this->m_ImageBuffer1, fnName, "m_ImageBuffer1")== false ||
		 JetMemory.alloc_func(BufferSize, this->m_ImageBuffer2, fnName, "m_ImageBuffer2")== false )	
	{	
		this->DestroyImageBuffer();	
		return false;	
	}
	this->m_ImageBufferSize = BufferSize;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCalibrationWnd::DestroyImageBuffer()//摧毀影像資料
{
	if ( NULL != this->m_MaskBuffer )
	{	JetMemory.free_func(m_MaskBuffer); }
	if ( NULL != this->m_MaskBuffer1 )
	{	JetMemory.free_func(m_MaskBuffer1); }
	if ( NULL != this->m_MaskBuffer2 )
	{	JetMemory.free_func(m_MaskBuffer2); }
	if ( NULL != this->m_PhaseBuffer )
	{	JetMemory.free_func(m_PhaseBuffer); }
	if ( NULL != this->m_PhaseBuffer1 )
	{	JetMemory.free_func(m_PhaseBuffer1); }
	if ( NULL != this->m_PhaseBuffer2 )
	{	JetMemory.free_func(m_PhaseBuffer2); }
	if ( NULL != this->m_ImageBuffer )
	{	JetMemory.free_func(m_ImageBuffer); }
	if ( NULL != this->m_ImageBuffer1 )
	{	JetMemory.free_func(m_ImageBuffer1);	}
	if ( NULL != this->m_ImageBuffer2 )
	{	JetMemory.free_func(m_ImageBuffer2);	}
	m_ImageBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCalibrationWnd::CreateShowBuffer()//建立顯示資料
{
	const char fnName[] = "CCalibrationWnd::CreateShowBuffer";
	DestroyShowBuffer();	
	const size_t BufferSize = CameraCtrl.GetMaxColorImageSize();
	if ( JetMemory.alloc_func(BufferSize, this->m_ShowBuffer, fnName, "m_ShowBuffer")==false ||
		 JetMemory.alloc_func(BufferSize, this->m_ShowBuffer1, fnName, "m_ShowBuffer1")==false )
	{	return false;	}

	this->m_ShowBufferSize = BufferSize;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCalibrationWnd::DestroyShowBuffer()//摧毀顯示資料
{
	if ( NULL != this->m_ShowBuffer )
	{	JetMemory.free_func(m_ShowBuffer);	}
	if ( NULL != this->m_ShowBuffer1 )
	{	JetMemory.free_func(m_ShowBuffer1); }
	m_ShowBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCalibrationWnd::CreateSpaceBuffer()//建立空間資料
{
	const char fnName[] = "CCalibrationWnd::CreateSpaceBuffer";
	DestroySpaceBuffer();
	const size_t BufferSize = CameraCtrl.GetMaxGrayImageSize();
	if ( JetMemory.alloc_func(BufferSize, this->m_SpaceBuffer, fnName, "m_SpaceBuffer")==false || 
		 JetMemory.alloc_func(BufferSize, this->m_SpaceBuffer1, fnName, "m_SpaceBuffer1")==false ||
		 JetMemory.alloc_func(BufferSize, this->m_SpaceBuffer2, fnName, "m_SpaceBuffer2")==false )
	{	return false;	}
	this->m_SpaceBufferSize = BufferSize;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCalibrationWnd::DestroySpaceBuffer()//摧毀空間資料
{
	if ( NULL != this->m_SpaceBuffer )
	{	JetMemory.free_func(m_SpaceBuffer); }
	if ( NULL != this->m_SpaceBuffer1 )
	{	JetMemory.free_func(m_SpaceBuffer1); }
	if ( NULL != this->m_SpaceBuffer2 )
	{	JetMemory.free_func(m_SpaceBuffer2); }
	m_SpaceBufferSize = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::OnTargetSettingBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == this->m_CaliPaneTargetPos.GetSafeHwnd() ) { return; }
	this->m_CaliPaneTargetPos.ShowWindow(SW_SHOW);
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::OnSaveCalibrationBtn() 
{
	// TODO: Add your control notification handler code here	
	if ( AOIDataCollect.SaveAllCalibrationParameter() == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	m_CaliPaneAlign.SetModifiedCaliParam(false);
	//m_CaliPaneStage.SetModifiedCaliParam(false);

	m_CaliPaneAlign.PostMessage(MSG_CALIBRATION_ALIGN_WND, WPARAM_SAVE_SYSTEM_PARAM, NULL);
}
//-------------------------------------------------------------------------------------//
BOOL CCalibrationWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString str;
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_LEFT:
			str = _T("CCalibrationWnd::WM_KEYDOWN::VK_LEFT\n");
			TRACE(str);
			break;
		case VK_RIGHT:
			str = _T("CCalibrationWnd::WM_KEYDOWN::VK_RIGHT\n");
			TRACE(str);
			break;
		case VK_UP:
			str = _T("CCalibrationWnd::WM_KEYDOWN::VK_UP\n");
			TRACE(str);
			break;
		case VK_DOWN:
			str = _T("CCalibrationWnd::WM_KEYDOWN::VK_DOWN\n");
			TRACE(str);
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	str = _T("Do you wanto to save calibration parameter?");
	str = LoadMultiLanguageString(str, str);
	
	m_CaliPaneAlign.ExecOnOK();
	//m_CaliPaneStage.ExecOnOK();	
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{	OnSaveCalibrationBtn();		}

	DWORD RetID = IDYES;
#ifndef MUST_BACKUP_SYSTEM_FILE_USE
	str = _T("Do you want to backup System files?");
	str = LoadMultiLanguageString(str, str);
	RetID = JetAPI::ShowMessageBox(str, MB_YESNO);
#endif//MUST_BACKUP_SYSTEM_FILE_USE
	if ( IDYES == RetID )
	{
		AOIDataCollect.BackupAllSystemIniFiles();
		AOIDataCollect.BackupAllSystemBinFiles();
	}
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CCalibrationWnd::OnSelchangePaneTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DoSelchangePaneTabWnd();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//