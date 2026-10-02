// BarcodeDeviceWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "BarcodeDeviceWnd.h"
//-------------------------------------------------------------------------------------//
#include "BarcodeGeneralWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define BARCODE_DEVICE_TIMER_READ_CODE      100
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeDeviceWnd dialog
//-------------------------------------------------------------------------------------//
CBarcodeDeviceWnd::CBarcodeDeviceWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CBarcodeDeviceWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeDeviceWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeDeviceWnd)		
	DDX_Control(pDX, BARCODE_DEVICE_CODE_LIST_WND, m_BarcodeCodeListCtrl);
	DDX_Control(pDX, BARCODE_DEVICE_LANE_BARCODE_ID_COMBO, m_BarcodeIDComboxCtrl);
	DDX_Control(pDX, BARCODE_DEVICE_LANE_BARCODE_LIST_WND, m_LaneBarcodeListCtrl);
	DDX_Control(pDX, BARCODE_DEVICE_LANE_ID_COMBO, m_LaneIDCombox);
	DDX_Control(pDX, BARCODE_DEVICE_TYP_COMBOX, m_DeviceTypeCombox);
	DDX_Control(pDX, BARCODE_DEVICE_ID_COMBOX, m_DeviceIDCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CBarcodeDeviceWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CBarcodeDeviceWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(BARCODE_DEVICE_ID_COMBOX, OnSelchangeDeviceIDCombox)
	ON_CBN_SELCHANGE(BARCODE_DEVICE_TYP_COMBOX, OnSelchangeDeviceTypCombox)
	ON_BN_CLICKED(BARCODE_DEVICE_READ_BTN, OnDeviceReadBtn)
	ON_BN_CLICKED(BARCODE_DEVICE_STOP_BTN, OnDeviceStopBtn)
	ON_BN_CLICKED(BARCODE_DEVICE_ADVANCE_BTN, OnDeviceAdvanceBtn)	
	ON_BN_CLICKED(BARCODE_DEVICE_CONNECT_CHK, OnDeviceConnectChk)
	ON_WM_TIMER()
	ON_BN_CLICKED(BARCODE_DEVICE_LANE_PCB_IN_BTN, OnDeviceLanePCBInBtn)
	ON_BN_CLICKED(BARCODE_DEVICE_LANE_PCB_OUT_BTN, OnDeviceLanePCBOutBtn)
	ON_BN_CLICKED(BARCODE_DEVICE_LANE_PCB_BACK_BTN, OnDeviceLanePCBBackBtn)
	ON_BN_CLICKED(BARCODE_DEVICE_LANE_CLAMP_ON_BTN, OnDeviceLaneClampOnBtn)
	ON_BN_CLICKED(BARCODE_DEVICE_LANE_CLAMP_OFF_BTN, OnDeviceLaneClampOffBtn)
	ON_BN_CLICKED(BARCODE_DEVICE_LANE_BARCODE_LIST_ENABLE_ALL_BTN, OnDeviceLaneBarcodeListEnableAllBtn)
	ON_BN_CLICKED(BARCODE_DEVICE_LANE_BARCODE_LIST_DISABLE_ALL_BTN, OnDeviceLaneBarcodeListDisableAllBtn)
	ON_CBN_SELCHANGE(BARCODE_DEVICE_LANE_ID_COMBO, OnSelchangeDeviceLaneIdCombo)
	ON_NOTIFY(NM_DBLCLK, BARCODE_DEVICE_LANE_BARCODE_LIST_WND, OnDblclkDeviceLaneBarcodeListWnd)
	ON_CBN_SELCHANGE(BARCODE_DEVICE_LANE_BARCODE_ID_COMBO, OnSelchangeDeviceLaneBarcodeIdCombo)
	ON_CBN_KILLFOCUS(BARCODE_DEVICE_LANE_BARCODE_ID_COMBO, OnKillfocusDeviceLaneBarcodeIdCombo)
	ON_BN_CLICKED(BARCODE_DEVICE_DEFAULT_BTN, OnDeviceDefaultBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, BARCODE_DEVICE_CODE_LIST_WND, OnItemchangedDeviceCodeListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeDeviceWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CBarcodeDeviceWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();	
	JetAPI::InitialListCtrl(m_BarcodeCodeListCtrl);	
	JetAPI::InitialListCtrl(m_LaneBarcodeListCtrl);	
	BuildBarcodeCodeListWnd_Header();
	BuildLaneBarcodeDeviceIDListWnd_Header();
	AOIDataDefine.BuildLaneIDCombox(m_LaneIDCombox);	
	AOIDataDefine.BuildBarcodeDeviceIDCombox(m_DeviceIDCombox, false);
	AOIDataDefine.BuildBarcodeDeviceIDCombox(m_BarcodeIDComboxCtrl, true);
	AOIDataDefine.BuildBarcodeDeviceTypeCombox(m_DeviceTypeCombox);	
	JetAPI::SetComboxCurSel(m_DeviceIDCombox, BARCODE_DEVICE_ID_01);	
	SwitchBarcodeDeviceID();
	OnSelchangeDeviceLaneIdCombo();

	UINT CtrlID=0;
	BOOL Enable=FALSE;	
	LANE_WORK_MODE LaneWorkMode_LA = AOIDataCollect.GetLaneWorkMode_LA();
	LANE_WORK_MODE LaneWorkMode_LB = AOIDataCollect.GetLaneWorkMode_LB();
	if ( LANE_WORK_DISABLE != LaneWorkMode_LB )
	{	JetAPI::SetComboxCurSel(m_LaneIDCombox, LANE_ID_B); }	
	if ( LANE_WORK_DISABLE != LaneWorkMode_LA )
	{	JetAPI::SetComboxCurSel(m_LaneIDCombox, LANE_ID_A); }	
#ifdef PLC_OBJ_DISABLE
	Enable=FALSE;
	CtrlID = BARCODE_DEVICE_LANE_PCB_IN_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = BARCODE_DEVICE_LANE_PCB_OUT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = BARCODE_DEVICE_LANE_PCB_BACK_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = BARCODE_DEVICE_LANE_CLAMP_ON_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID = BARCODE_DEVICE_LANE_CLAMP_OFF_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
#endif//PLC_OBJ_DISABLE
	if ( false == AOIDataCollect.GetUIEnablePCBOutButton() )
	{	JetAPI::EnableCtrlWnd(this, BARCODE_DEVICE_LANE_PCB_OUT_BTN, FALSE);	}
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	OnDeviceStopBtn();
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::BuildBarcodeCodeListWnd()
{
	size_t  i=0;
	int     nItem=0;
	int     nSubItem=0;
	CString strCaption, strValue;	
	CThisListCtrl_03 &ListCtrl = m_BarcodeCodeListCtrl;		
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	CWnd::SetDlgItemText(BARCODE_DEVICE_SUB_CODE_EDIT, _T(""));
	CBarcode_Basic *BarcodeDevicePtr = NULL;//取得條碼機指標
	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(m_BarcodeDeviceParam.nDeviceID);//取得條碼機指標
	if ( NULL == BarcodeDevicePtr ) { return true; }
	const size_t MaxBarcodeCodeCount = BarcodeDevicePtr->GetResultSubCount();

	nItem = 0;
	for ( i=0; i<MaxBarcodeCodeCount; i++ )
	{
		nSubItem = 0;
		strCaption.Format(_T("%d"), i+1);
		strValue = BarcodeDevicePtr->GetResultSubBuffer(i);

		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, strCaption); nSubItem++;
		ListCtrl.SetItemText(nItem, nSubItem, strValue);   nSubItem++;
		nItem ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::BuildBarcodeCodeListWnd_Header()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT	
	CThisListCtrl_03 &ListCtrl = m_BarcodeCodeListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/8;
	str = _T("Index");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width2 = width*7;
	str = _T("Code");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::BuildLaneBarcodeDeviceIDListWnd()
{
	size_t  i=0;
	int     nItem=0;
	int     nSubItem=0;
	CString strCaption, strValue;	
	CThisListCtrl_03 &ListCtrl = m_LaneBarcodeListCtrl;	
	const size_t MaxBarcodeDeviceCount = MAX_BARCODE_DEVICE_COUNT;

	nItem = 0;
	m_LaneBarcodeIndex = -1;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	for ( i=0; i<MaxBarcodeDeviceCount; i++ )
	{
		nSubItem = 0;
		strCaption.Format(_T("%d"), i+1);
		strValue.Format(_T("%d"), m_LaneBarcodeIDList[i]);		

		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, strCaption); nSubItem++;
		ListCtrl.SetItemText(nItem, nSubItem, strValue);   nSubItem++;
		nItem ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::BuildLaneBarcodeDeviceIDListWnd_Header()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT	
	CThisListCtrl_03 &ListCtrl = m_LaneBarcodeListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/4;
	str = _T("Index");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width2 = width*3;
	str = _T("Barcode ID");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnSelchangeDeviceIDCombox() 
{
	// TODO: Add your control notification handler code here
	SwitchBarcodeDeviceID();
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnSelchangeDeviceTypCombox() 
{
	// TODO: Add your control notification handler code here
	SwitchBarcodeDeviceType();
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceReadBtn() 
{
	// TODO: Add your control notification handler code here
	StopBarcodeDeviceCodeTimer();
	CBarcode_Basic *BarcodeDevicePtr = NULL;//取得條碼機指標
	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(m_BarcodeDeviceParam.nDeviceID);//取得條碼機指標
	if ( NULL == BarcodeDevicePtr ) { return ; }

	CWnd::SetDlgItemText(BARCODE_DEVICE_CODE_EDIT, _T(""));
	CWnd::SetDlgItemText(BARCODE_DEVICE_SUB_CODE_EDIT, _T(""));
	JetAPI::ClearListCtrl(m_BarcodeCodeListCtrl, FALSE);
	if ( BarcodeDevicePtr->StartToRead() == false )
	{
		CString str = BarcodeDevicePtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return;
	}
	StartpBarcodeDeviceCodeTimer();	
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceStopBtn() 
{
	// TODO: Add your control notification handler code here
	StopBarcodeDeviceCodeTimer();	
	CBarcode_Basic *BarcodeDevicePtr = NULL;//取得條碼機指標
	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(m_BarcodeDeviceParam.nDeviceID);//取得條碼機指標
	if ( NULL == BarcodeDevicePtr ) { return ; }

	ReadBarcodeDeviceCode(false);
	BarcodeDevicePtr->EndReading();
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceAdvanceBtn()
{
	OnDeviceStopBtn();	
	CBarcode_Basic *BarcodeDevicePtr = NULL;//取得條碼機指標
	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(m_BarcodeDeviceParam.nDeviceID);//取得條碼機指標
	if ( NULL == BarcodeDevicePtr ) { return ; }
	BARCODE_DEVICE_TYPE BarcodeDeviceType=BarcodeDevicePtr->GetBarcodeDeviceType();
	if ( BARCODE_DEVICE_GENERAL_GROUP == BarcodeDeviceType )
	{
		CBarcodeGeneralWnd Wnd;
		CBarcode_General *Barcode_GeneralPtr=(CBarcode_General*)BarcodeDevicePtr;
		Wnd.SetBarcodePtr(Barcode_GeneralPtr);
		Wnd.DoModal();
		UpdateBarcodeDeviceToUI(false);		
	}

}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_BARCODE_DEVICE_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_BARCODE_DEVICE_WND;
	WndKey = _T("IDD_BARCODE_DEVICE_WND");
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
	WndID = BARCODE_DEVICE_ID_LABEL;
	WndKey = _T("BARCODE_DEVICE_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCODE_DEVICE_TYP_LABEL;
	WndKey = _T("BARCODE_DEVICE_TYP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = BARCODE_DEVICE_CONNECT_PORT_LABL;
	WndKey = _T("BARCODE_DEVICE_CONNECT_PORT_LABL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCODE_DEVICE_CONNECT_CHK;
	WndKey = _T("BARCODE_DEVICE_CONNECT_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCODE_DEVICE_DEFAULT_BTN;
	WndKey = _T("BARCODE_DEVICE_DEFAULT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCODE_DEVICE_READ_BTN;
	WndKey = _T("BARCODE_DEVICE_READ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCODE_DEVICE_CODE_LABEL;
	WndKey = _T("BARCODE_DEVICE_CODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCODE_DEVICE_STOP_BTN;
	WndKey = _T("BARCODE_DEVICE_STOP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BARCODE_DEVICE_ADVANCE_BTN;
	WndKey = _T("BARCODE_DEVICE_ADVANCE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = BARCODE_DEVICE_LANE_GROUP;
	WndKey = _T("BARCODE_DEVICE_LANE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BARCODE_DEVICE_LANE_ID_LABEL;
	WndKey = _T("BARCODE_DEVICE_LANE_ID_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BARCODE_DEVICE_LANE_PCB_IN_BTN;
	WndKey = _T("BARCODE_DEVICE_LANE_PCB_IN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BARCODE_DEVICE_LANE_PCB_OUT_BTN;
	WndKey = _T("BARCODE_DEVICE_LANE_PCB_OUT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BARCODE_DEVICE_LANE_PCB_BACK_BTN;
	WndKey = _T("BARCODE_DEVICE_LANE_PCB_BACK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BARCODE_DEVICE_LANE_CLAMP_ON_BTN;
	WndKey = _T("BARCODE_DEVICE_LANE_CLAMP_ON_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = BARCODE_DEVICE_LANE_CLAMP_OFF_BTN;
	WndKey = _T("BARCODE_DEVICE_LANE_CLAMP_OFF_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//	
	WndID = BARCODE_DEVICE_LANE_BARCODE_LIST_ENABLE_ALL_BTN;
	WndKey = _T("BARCODE_DEVICE_LANE_BARCODE_LIST_ENABLE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = BARCODE_DEVICE_LANE_BARCODE_LIST_DISABLE_ALL_BTN;
	WndKey = _T("BARCODE_DEVICE_LANE_BARCODE_LIST_DISABLE_ALL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CBarcodeDeviceWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_BARCODE_DEVICE_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::SwitchBarcodeDeviceID()
{	
	OnDeviceStopBtn();

	const size_t DeviceID = JetAPI::GetComboxCurSelData(m_DeviceIDCombox);
	AOIDataCollect.GetBarcodeDeviceParam(DeviceID, m_BarcodeDeviceParam);	
	UpdateBarcodeDeviceToUI(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::SwitchBarcodeDeviceType()
{
	OnDeviceStopBtn();
	CString strConnectPort;
	CWnd::GetDlgItemText(BARCODE_DEVICE_CONNECT_PORT_EDIT, strConnectPort);
	const size_t DeviceType = JetAPI::GetComboxCurSelData(m_DeviceTypeCombox);
	m_BarcodeDeviceParam.eDevieType = (BARCODE_DEVICE_TYPE)(DeviceType);
	m_BarcodeDeviceParam.sDevicePort = strConnectPort;
	AOIDataCollect.SetBarcodeDeviceParam(m_BarcodeDeviceParam.nDeviceID, m_BarcodeDeviceParam);
	if ( AOIDataCollect.CreateBarcodeDevice(m_BarcodeDeviceParam) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	UpdateBarcodeDeviceToUI(false);
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceConnectChk() 
{
	// TODO: Add your control notification handler code here
	CBarcode_Basic *BarcodeDevicePtr = NULL;//取得條碼機指標
	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(m_BarcodeDeviceParam.nDeviceID);//取得條碼機指標
	if ( NULL == BarcodeDevicePtr ) { return ; }

	bool    bOk = true;
	CString strConnectPort;
	BOOL bCheck = CWnd::IsDlgButtonChecked(BARCODE_DEVICE_CONNECT_CHK);
	CWnd::GetDlgItemText(BARCODE_DEVICE_CONNECT_PORT_EDIT, strConnectPort);
	
	if ( TRUE == bCheck )
	{	
		BarcodeDevicePtr->SetBarcodeDevicePort(strConnectPort);
		bOk = BarcodeDevicePtr->ConnectToDevice(); 
		if ( TRUE == bOk )
		{
			m_BarcodeDeviceParam.sDevicePort = strConnectPort;
			AOIDataCollect.SetBarcodeDeviceParam(m_BarcodeDeviceParam.nDeviceID, m_BarcodeDeviceParam);
		}
	}
	else
	{	bOk = BarcodeDevicePtr->Disconnected(); }

	UpdateBarcodeDeviceToUI(false);

	if ( TRUE == bCheck )
	{
		CString str;
		if ( false == bOk )
		{	str = BarcodeDevicePtr->GetErrorString();	}
		else
		{	str = AOIDataDefine.GetFinishText(); }
		JetAPI::ShowMessageBox(str);
	}
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::UpdateBarcodeDeviceToUI(bool UpdateType)
{	
	CBarcode_Basic *BarcodeDevicePtr=NULL;
	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(m_BarcodeDeviceParam.nDeviceID);//取得條碼機指標

	if ( true == UpdateType )
	{	
		BARCODE_DEVICE_TYPE DeviceType = m_BarcodeDeviceParam.eDevieType;
		JetAPI::SetComboxCurSel(m_DeviceTypeCombox, DeviceType);
	}
	CString         strConnectPort = m_BarcodeDeviceParam.sDevicePort;
	CWnd::SetDlgItemText(BARCODE_DEVICE_CONNECT_PORT_EDIT, strConnectPort);		

	BOOL bEnable=TRUE;
	bool Connected = false;	
	if ( NULL != BarcodeDevicePtr )
	{	Connected = BarcodeDevicePtr->GetBarcodeDeviceConnected();	 }
	CWnd::CheckDlgButton(BARCODE_DEVICE_CONNECT_CHK, Connected);	
	if ( true == Connected )
	{	
		bEnable = FALSE;
		JetAPI::EnableEditWnd(this, BARCODE_DEVICE_CONNECT_PORT_EDIT, false); 
	}
	else
	{
		bEnable = TRUE;
		JetAPI::EnableEditWnd(this, BARCODE_DEVICE_CONNECT_PORT_EDIT, true); 
	}
	//JetAPI::EnableCtrlWnd(this, BARCODE_DEVICE_ID_COMBOX, bEnable);
	JetAPI::EnableCtrlWnd(this, BARCODE_DEVICE_TYP_COMBOX, bEnable);

	bool bLock = false;
	if ( NULL == BarcodeDevicePtr )
	{	bLock = true; }
	else
	{	bLock = false; }

	LockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::LockUIWnd(bool bLock)
{
	UINT    CtrlID=0;
	BOOL    Enable=TRUE;

	if ( true == bLock ) { Enable = FALSE; }
	else { Enable = TRUE; }

	CtrlID=BARCODE_DEVICE_CONNECT_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID=BARCODE_DEVICE_DEFAULT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID=BARCODE_DEVICE_READ_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID=BARCODE_DEVICE_STOP_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);
	CtrlID=BARCODE_DEVICE_ADVANCE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, Enable);

	//CtrlID=BARCODE_DEVICE_CONNECT_PORT_EDIT;
	//JetAPI::EnableEditWnd(this, CtrlID, Enable);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( nIDEvent )
	{
	case BARCODE_DEVICE_TIMER_READ_CODE:
		CWnd::KillTimer(nIDEvent);
		ReadBarcodeDeviceCode(true);
		StartpBarcodeDeviceCodeTimer();
		break;
	}
	CBaseDialog::OnTimer(nIDEvent);
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::StopBarcodeDeviceCodeTimer()//停止讀取條碼內容
{
	CWnd::KillTimer(BARCODE_DEVICE_TIMER_READ_CODE);
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::StartpBarcodeDeviceCodeTimer()//停止讀取條碼內容
{
	CWnd::SetTimer(BARCODE_DEVICE_TIMER_READ_CODE, 250, NULL);
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::ReadBarcodeDeviceCode(bool CheckReceieve)//讀取條碼內容
{	
	CBarcode_Basic *BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(m_BarcodeDeviceParam.nDeviceID);//取得條碼機指標
	if ( NULL == BarcodeDevicePtr ) { return false; }

	if ( true == CheckReceieve )
	{
		if ( BarcodeDevicePtr->RetrieveCode() == false ) { return false; }
	}
	CString      strContext;	
	const size_t BufferSize = 1024;
	char         Buffer[BufferSize]="";	
	BarcodeDevicePtr->CloneResultBuffer(Buffer, BufferSize);
	strContext = Buffer;
	//strContext = BarcodeDevicePtr->GetResultBuffer();
	CWnd::SetDlgItemText(BARCODE_DEVICE_CODE_EDIT, strContext);
	BuildBarcodeCodeListWnd();
	//StopBarcodeDeviceCodeTimer();
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnOK() 
{
	// TODO: Add extra validation here
	CString str;
	str = _T("Do you want to save the barcode device parameter?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDYES )
	{	AOIDataCollect.SaveSystemBarcodeParameter();	}

	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
LANE_ID CBarcodeDeviceWnd::GetActiveLaneID()
{
	LANE_ID LaneID = (LANE_ID)(JetAPI::GetComboxCurSelData(m_LaneIDCombox));
	return LaneID;
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceLanePCBInBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = GetActiveLaneID();	
	if ( AOIDataCollect.ExecPCBInProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());}
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceLanePCBOutBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBOutProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());}
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceLanePCBBackBtn() 
{
	// TODO: Add your control notification handler code here
	const bool bStep = true;
	LANE_ID LaneID = GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBBackProc(LaneID, true, bStep) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());}
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceLaneClampOnBtn() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID = GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBClampOnProc(LaneID) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());}
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceLaneClampOffBtn() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID = GetActiveLaneID();
	if ( AOIDataCollect.ExecPCBClampOffProc(LaneID) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());}
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceLaneBarcodeListEnableAllBtn() 
{
	// TODO: Add your control notification handler code here
	size_t  i=0;
	LANE_ID LaneID = GetActiveLaneID();	
	const size_t MaxBarcodeDeviceCount = MAX_BARCODE_DEVICE_COUNT;
	for ( i=0; i<MaxBarcodeDeviceCount; i++ )
	{
		switch ( i )
		{
		case 0:	m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_01;	break;
		case 1:	m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_02;	break;
		case 2:	m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_03;	break;
		case 3:	m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_04;	break;
		case 4:	m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_05;	break;
		case 5:	m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_06;	break;
		case 6:	m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_07;	break;
		case 7:	m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_08;	break;		
		default: m_LaneBarcodeIDList[i]=BARCODE_DEVICE_ID_OFF;	break;		
		}
	}
	switch ( LaneID )
	{
	case LANE_ID_A:	AOIDataCollect.SetBarcodeDeviceIDList_LA(m_LaneBarcodeIDList);	break;
	case LANE_ID_B:	AOIDataCollect.SetBarcodeDeviceIDList_LB(m_LaneBarcodeIDList);	break;		
	}	
	BuildLaneBarcodeDeviceIDListWnd();
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceLaneBarcodeListDisableAllBtn() 
{
	// TODO: Add your control notification handler code here
	size_t  i=0;
	LANE_ID LaneID = GetActiveLaneID();	
	const size_t MaxBarcodeDeviceCount = MAX_BARCODE_DEVICE_COUNT;
	for ( i=0; i<MaxBarcodeDeviceCount; i++ )
	{	m_LaneBarcodeIDList[i] = BARCODE_DEVICE_ID_OFF;	}

	switch ( LaneID )
	{
	case LANE_ID_A:	AOIDataCollect.SetBarcodeDeviceIDList_LA(m_LaneBarcodeIDList);	break;
	case LANE_ID_B:	AOIDataCollect.SetBarcodeDeviceIDList_LB(m_LaneBarcodeIDList);	break;		
	}	
	BuildLaneBarcodeDeviceIDListWnd();
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnSelchangeDeviceLaneIdCombo() 
{
	// TODO: Add your control notification handler code here
	LANE_ID LaneID = GetActiveLaneID();
	if ( LANE_ID_B == LaneID )
	{	AOIDataCollect.CloneBarcodeDeviceIDList_LB(m_LaneBarcodeIDList);	}
	else
	{	AOIDataCollect.CloneBarcodeDeviceIDList_LA(m_LaneBarcodeIDList);	}
	BuildLaneBarcodeDeviceIDListWnd();
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDblclkDeviceLaneBarcodeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here	
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	ShowLaneBarcodeDeviceIDCombox(m_LaneBarcodeListCtrl, nItem, nSubItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeDeviceWnd::ShowLaneBarcodeDeviceIDCombox(CThisListCtrl_03 &ListCtrl, int nItem, int nSubItem)
{	
	if ( ListCtrl.GetSafeHwnd() == NULL) { return false; }
	const int ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	CRect ItemRect;
	RECT  CtrlRect={0};
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	m_LaneBarcodeIndex = nItem;
	CComboBox &ComboxCtrl = m_BarcodeIDComboxCtrl;
	const int BarcodeDeviceID=m_LaneBarcodeIDList[nItem];
	if ( ComboxCtrl.GetSafeHwnd() == NULL ) { return false; }

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);	
	JetAPI::SetComboxCurSel(ComboxCtrl, BarcodeDeviceID);
	ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
	ComboxCtrl.SetFocus();
	ComboxCtrl.ShowDropDown();
	ComboxCtrl.ShowWindow(SW_SHOW);
	ComboxCtrl.BringWindowToTop();			
	ListCtrl.UpdateWindow();
	ComboxCtrl.Invalidate();
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnSelchangeDeviceLaneBarcodeIdCombo() 
{
	// TODO: Add your control notification handler code here	
	CComboBox &ComboxCtrl = m_BarcodeIDComboxCtrl;
	ComboxCtrl.ShowWindow(SW_HIDE);
	if ( -1==m_LaneBarcodeIndex || m_LaneBarcodeIndex>=MAX_BARCODE_DEVICE_COUNT ) { return; }	
	LANE_ID LaneID = GetActiveLaneID();
	const int BarcodeDeviceID = (int)(JetAPI::GetComboxCurSelData(ComboxCtrl));
	m_LaneBarcodeIDList[m_LaneBarcodeIndex] = BarcodeDeviceID;	
	switch ( LaneID )
	{
	case LANE_ID_A:	AOIDataCollect.SetBarcodeDeviceIDList_LA(m_LaneBarcodeIDList);	break;
	case LANE_ID_B:	AOIDataCollect.SetBarcodeDeviceIDList_LB(m_LaneBarcodeIDList);	break;		
	}

	const int ItemCount = m_LaneBarcodeListCtrl.GetItemCount();
	if ( m_LaneBarcodeIndex>=0 && m_LaneBarcodeIndex<ItemCount ) 
	{
		CString str;
		str.Format(_T("%d"), BarcodeDeviceID);
		m_LaneBarcodeListCtrl.SetItemText(m_LaneBarcodeIndex, 1, str);
	}
	//BuildLaneBarcodeDeviceIDListWnd();	
	m_LaneBarcodeListCtrl.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnKillfocusDeviceLaneBarcodeIdCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_BarcodeIDComboxCtrl.GetSafeHwnd() == NULL ) { return; }
	m_BarcodeIDComboxCtrl.ShowWindow(SW_HIDE);
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnDeviceDefaultBtn() 
{
	// TODO: Add your control notification handler code here
	CString    str;
	StopBarcodeDeviceCodeTimer();
	CBarcode_Basic *BarcodeDevicePtr = NULL;//取得條碼機指標
	BarcodeDevicePtr = AOIDataCollect.GetBarcodeDevicePtr(m_BarcodeDeviceParam.nDeviceID);//取得條碼機指標
	if ( NULL == BarcodeDevicePtr ) { return ; }
	if ( BarcodeDevicePtr->Initialize() == false )
	{
		str = BarcodeDevicePtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
	}
}
//-------------------------------------------------------------------------------------//
void CBarcodeDeviceWnd::OnItemchangedDeviceCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }
	CString ItemText;
	ItemText = m_BarcodeCodeListCtrl.GetItemText(nItem, 1);
	CWnd::SetDlgItemText(BARCODE_DEVICE_SUB_CODE_EDIT, ItemText);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//