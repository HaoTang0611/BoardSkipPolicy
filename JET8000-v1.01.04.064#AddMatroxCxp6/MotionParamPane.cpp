// MotionParamPane.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "MotionParamPane.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
const int PARAM_TITLE      = 0;
const int AXIS_PARAM_X     = 1;
const int AXIS_PARAM_Y     = 2;
const int AXIS_PARAM_Z     = 3;
const int AXIS_PARAM_UNIT  = 4;
const int MOTION_PARAM_UNIT  = 2;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlPaneParam dialog
//-------------------------------------------------------------------------------------//
CMotionCtrlPaneParam::CMotionCtrlPaneParam(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionCtrlPaneParam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotionCtrlPaneParam)
	//}}AFX_DATA_INIT	
	m_ParamActPtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotionCtrlPaneParam)
	DDX_Control(pDX, MPARAM_PARAM_LIST_WND, m_ParamListWnd);
	DDX_Control(pDX, MPARAM_GRID_COMBO, m_ComboxCtrl);
	DDX_Control(pDX, MPARAM_GRID_EDIT_WND, m_EditCtrl);
	DDX_Control(pDX, MPARAM_AXIS_PARAM_LIST_WND, m_AxisParamListWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMotionCtrlPaneParam, CDialog)
	//{{AFX_MSG_MAP(CMotionCtrlPaneParam)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, MPARAM_AXIS_PARAM_LIST_WND, OnDblclkAxisParamListWnd)
	ON_EN_KILLFOCUS(MPARAM_GRID_EDIT_WND, OnKillfocusGridEditWnd)
	ON_NOTIFY(NM_DBLCLK, MPARAM_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_CBN_KILLFOCUS(MPARAM_GRID_COMBO, OnKillfocusGridCombo)
	ON_CBN_SELCHANGE(MPARAM_GRID_COMBO, OnSelchangeGridCombo)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMotionCtrlPaneParam message handlers
BOOL CMotionCtrlPaneParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	JetAPI::InitialListCtrl(this->m_ParamListWnd);
	JetAPI::InitialListCtrl(this->m_AxisParamListWnd);

	this->BuildParamListWndHeader();
	this->BuildAxisParamListWndHeader();
	this->SwitchMultiLanguage();

	//this->BuildAllParamList();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MOTION_PARAM_PANE");
	//---------------------------------------------------------------------------------//
	WndID = IDD_MOTION_PARAM_PANE;
	WndKey = _T("IDD_MOTION_PARAM_PANE");
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
	WndID = MPARAM_AXIS_PARAM_LABEL;
	WndKey = _T("MPARAM_AXIS_PARAM_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CMotionCtrlPaneParam::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_MOTION_PARAM_PANE");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_ParamListWnd.GetSafeHwnd() == NULL ) { return; }

	RECT  TopWndRect={0};	
	CWnd *WndPtr = NULL;
	const int ListSizeH = cy/3;
	const int MarginX = 4;
	const int MarginY = 4;
	if ( m_ParamListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ParamListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.left = MarginX;
		WndRect.right = cx-MarginX;
		WndRect.bottom = WndRect.top + ListSizeH;
		m_ParamListWnd.MoveWindow(&WndRect);
		TopWndRect = WndRect;
	}

	WndPtr = CWnd::GetDlgItem(MPARAM_AXIS_PARAM_LABEL);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		SIZE WndSize={0};
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.top = TopWndRect.bottom+MarginY;
		WndRect.bottom = WndRect.top + WndSize.cy;
		WndPtr->MoveWindow(&WndRect);
		TopWndRect.bottom = WndRect.bottom;
	}

	if ( m_AxisParamListWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_AxisParamListWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.left = MarginX;
		WndRect.right = cx-MarginX;
		WndRect.top = TopWndRect.bottom + 4;
		WndRect.bottom = cy-MarginY;
		m_AxisParamListWnd.MoveWindow(&WndRect);
	}
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::BuildParamListWndHeader()
{
	int nCol = 0;
	int width = 0;
	int Align = LVCFMT_RIGHT;//LVCFMT_CENTER;
	RECT Rect={0};
	CString str;

	this->m_ParamListWnd.GetClientRect(&Rect);
	width = Rect.right-Rect.left;
	width = width-32-96;
	width = width/3;

	str = _T("Name");
	str = LoadMultiLanguageString(str, str);
	this->m_ParamListWnd.InsertColumn(nCol, str, Align, width*2);
	nCol ++;	

	str = _T("Parameter");
	str = LoadMultiLanguageString(str, str);
	this->m_ParamListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;

	str = _T("Unit");
	str = LoadMultiLanguageString(str, str);
	this->m_ParamListWnd.InsertColumn(nCol, str, Align, 96);
	nCol ++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::BuildAxisParamListWndHeader()
{
	int nCol = 0;
	int width = 0;
	int Align = LVCFMT_RIGHT;//LVCFMT_CENTER;
	RECT Rect={0};
	CString str;

	this->m_AxisParamListWnd.GetClientRect(&Rect);
	width = Rect.right-Rect.left;
	width = width-32-96;
	width = width/4;

	str = _T("Name");
	str = LoadMultiLanguageString(str, str);
	this->m_AxisParamListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;	

	str = _T("X");
	str = LoadMultiLanguageString(str, str);
	this->m_AxisParamListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;

	str = _T("Y");
	str = LoadMultiLanguageString(str, str);
	this->m_AxisParamListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;

	str = _T("Z");
	str = LoadMultiLanguageString(str, str);
	this->m_AxisParamListWnd.InsertColumn(nCol, str, Align, width);
	nCol ++;

	str = _T("Unit");
	str = LoadMultiLanguageString(str, str);
	this->m_AxisParamListWnd.InsertColumn(nCol, str, Align, 96);
	nCol ++;

	return true;
}
//-------------------------------------------------------------------------------------//
CParamUni* CMotionCtrlPaneParam::GetActParamUni()
{
	return m_ParamActPtr;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::SetActParamUni(CParamUni *Ptr)
{
	m_ParamActPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::BuildParamList()
{
	size_t idx=0;
	int     Precision=4;
	int     nID=0;
	CString strID;
	CString Section;
	CString KeyName;		
	TMotionParamItem MotionParam;
	const TMotionParameter &MotionParamRef = MotionCtrlPtr->GetMotionParameter();
	
	this->m_ParamList.clear();	

	Section = MotionCtrlPtr->GetMotionParamSectionName();	
	MotionParam.sParam.SetSection(Section);	
	
	//是否儲存運動卡參數
	MotionParam.sUnit = _T("");		
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("Save Motion Card Parameters"));
	MotionParam.sParam.SetKeyName(KeyName);	
	nID = FN_DISABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_SAVE_MOTION_CARD_PARAM);
	MotionParam.sParam.SetValue_SEL(MotionParamRef.m_SaveMotionCardParam);
	this->m_ParamList.push_back(MotionParam);

	//是否儲存運動訊息
	MotionParam.sUnit = _T("");		
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("Save Current Motion Process"));
	MotionParam.sParam.SetKeyName(KeyName);	
	nID = FN_DISABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_SAVE_CURRENT_PROCESS);
	MotionParam.sParam.SetValue_SEL(MotionParamRef.m_SaveCurrentMotionProcess);
	this->m_ParamList.push_back(MotionParam);
	
	//加速度調整
	MotionParam.sUnit = _T("");		
	MotionParam.sParam.ClearSelList();	
	KeyName.Format(_T("Enable Acceleration Adjust"));
	MotionParam.sParam.SetKeyName(KeyName);	
	nID = FN_DISABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_ACCELERATION_ADJUST);
	MotionParam.sParam.SetValue_SEL(MotionParamRef.m_AccelerationAdjust);
	this->m_ParamList.push_back(MotionParam);

	//是否XY座標補正
	MotionParam.sUnit = _T("");		
	MotionParam.sParam.ClearSelList();	
	KeyName.Format(_T("Enable XY Calibration"));
	MotionParam.sParam.SetKeyName(KeyName);	
	nID = FN_DISABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_XY_CALI_ENABLE);
	MotionParam.sParam.SetValue_SEL(MotionParamRef.m_XYCaliEnable);
	this->m_ParamList.push_back(MotionParam);	

	//XY座標補正外擴範圍
	MotionParam.sUnit = _T("um");		
	MotionParam.sParam.ClearSelList();	
	KeyName.Format(_T("XY Calibration Extend Range"));
	MotionParam.sParam.SetKeyName(KeyName);		
	MotionParam.sParam.SetParamID(MOTION_PARAM_XY_CALI_EXTEND_RANGE);
	MotionParam.sParam.SetValue_DBL(MotionParamRef.m_XYCaliExtendRange, 0);
	this->m_ParamList.push_back(MotionParam);	
	
	//S-Curve速度比例 	
	MotionParam.sUnit = _T("%");		
	MotionParam.sParam.ClearSelList();	
	KeyName.Format(_T("S-Curve Velocity Ratio"));
	MotionParam.sParam.SetKeyName(KeyName);		
	MotionParam.sParam.SetParamID(MOTION_PARAM_S_CURVE_VELOCITY_RATIO);
	MotionParam.sParam.SetValue_DBL(MotionParamRef.m_SCurveVelRatio, 0, 0, 100);
	this->m_ParamList.push_back(MotionParam);	

	//內部方向性轉換
	MotionParam.sUnit = _T("");		
	MotionParam.sParam.ClearSelList();	
	KeyName.Format(_T("Sign Positive Convert"));
	MotionParam.sParam.SetKeyName(KeyName);	
	nID = FN_DISABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetEnableDisableText(nID);	MotionParam.sParam.AddSelItem(nID, strID);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_SIGN_POSITIVE_CONVERT);
	MotionParam.sParam.SetValue_SEL(MotionParamRef.m_SignPositiveConvert);
	this->m_ParamList.push_back(MotionParam);	

	//儲存In Position Delay Time
	MotionParam.sUnit = _T("ms");
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("In Position Delay Time"));		
	MotionParam.sParam.SetKeyName(KeyName);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_IN_POSITION_DELAY_TIME);
	MotionParam.sParam.SetValue_INT(MotionParamRef.m_InPositionDelayTime, 0);	
	this->m_ParamList.push_back(MotionParam);

	//儲存等待移動停止的等待時間 ms		
	MotionParam.sUnit = _T("ms");
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("Wait For Done Dwell Time"));		
	MotionParam.sParam.SetKeyName(KeyName);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_WAIT_FOR_DONW_DWELL_TIME);
	MotionParam.sParam.SetValue_INT(MotionParamRef.m_WaitForDoneDwellTime, 0);	
	this->m_ParamList.push_back(MotionParam);	

	//儲存 JOG Moving Velocity
	MotionParam.sUnit = _T("um/sec");
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("JOG Moving Velocity"));	
	MotionParam.sParam.SetKeyName(KeyName);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_JOG_MOVING_VELOCITY);
	MotionParam.sParam.SetValue_DBL(MotionParamRef.m_JogMovingVelocity, Precision);	
	//this->m_ParamList.push_back(MotionParam);	

	//儲存 JOG Max Velocity
	MotionParam.sUnit = _T("um/sec");
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("JOG Max Velocity"));	
	MotionParam.sParam.SetKeyName(KeyName);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_JOG_MAX_VELOCITY);
	MotionParam.sParam.SetValue_DBL(MotionParamRef.m_JogMaxVelocity, Precision);	
	//this->m_ParamList.push_back(MotionParam);

	//儲存 JOG Acceleration Time
	MotionParam.sUnit = _T("um/sec^2");
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("JOG Acceleration Time"));	
	MotionParam.sParam.SetKeyName(KeyName);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_JOG_ACCEL_TIME);
	MotionParam.sParam.SetValue_DBL(MotionParamRef.m_JogAccelerationTime, Precision);	
	this->m_ParamList.push_back(MotionParam);		

	//儲存 JOG Deceleration Time
	MotionParam.sUnit = _T("um/sec^2");
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("JOG Deceleration Time"));	
	MotionParam.sParam.SetKeyName(KeyName);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_JOG_DECEL_TIME);
	MotionParam.sParam.SetValue_DBL(MotionParamRef.m_JogDecelerationTime, Precision);	
	this->m_ParamList.push_back(MotionParam);		

	//是否啟用運動系統內部的軟體極限
	MotionParam.sUnit = _T("");
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("Using Motion Software Limit"));
	MotionParam.sParam.SetKeyName(KeyName);
	nID = FN_DISABLE;
	strID = AOIDataDefine.GetEnableDisableText(nID);
	MotionParam.sParam.AddSelItem(nID, strID);
	nID = FN_ENABLE;
	strID = AOIDataDefine.GetEnableDisableText(nID);
	MotionParam.sParam.AddSelItem(nID, strID);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_ENABLE_SOFTWARE_LIMIT);
	MotionParam.sParam.SetValue_SEL(MotionParamRef.m_UsingMotionSoftwareLimit);
	this->m_ParamList.push_back(MotionParam);
	
	//是否啟用節點別名模式 		
	MotionParam.sUnit = _T("");
	MotionParam.sParam.ClearSelList();
	KeyName.Format(_T("Using ECAT Node Alias Name"));
	MotionParam.sParam.SetKeyName(KeyName);
	AOIDataDefine.BuildEnableDisableParamUni(MotionParam.sParam);	
	MotionParam.sParam.SetParamID(MOTION_PARAM_USING_ECAT_NODE_ALIAS_NAME);
	MotionParam.sParam.SetValue_SEL(MotionParamRef.m_UsingEcatNodeAliasName);
	this->m_ParamList.push_back(MotionParam);

	this->BuildParamListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::BuildAxisParamList()
{
	//TMotionParamItem
	size_t idx=0;
	int     nID=0;
	int     Precision=4;
	CString strID;
	CString Section;
	CString KeyName;	
	TAxisParamItem  AxisParam;	
	const TMotionParameter &MotionParamRef = MotionCtrlPtr->GetMotionParameter();

	this->m_AxisParamList.clear();	

	Section = MotionCtrlPtr->GetMotionParamSectionName();	
	AxisParam.sParamX.SetSection(Section);
	AxisParam.sParamY.SetSection(Section);
	AxisParam.sParamZ.SetSection(Section);	

	//機台與Cad的正負方向
	AxisParam.sTitle = _T("Stage Sign Positive");
	AxisParam.sUnit = _T("");
	KeyName.Format(_T("X Stage Sign Positive"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.ClearSelList();	
	nID = FN_DISABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamX.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamX.AddSelItem(nID, strID);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_SIGN_POSITIVE_X);
	AxisParam.sParamX.SetValue_SEL(MotionParamRef.m_SignPositiveX);
	KeyName.Format(_T("Y Stage Sign Positive"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.ClearSelList();	
	nID = FN_DISABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamY.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamY.AddSelItem(nID, strID);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_SIGN_POSITIVE_Y);
	AxisParam.sParamY.SetValue_SEL(MotionParamRef.m_SignPositiveY);
	KeyName.Format(_T("Z Stage Sign Positive"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.ClearSelList();
	nID = FN_DISABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamZ.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamZ.AddSelItem(nID, strID);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_SIGN_POSITIVE_Z);
	AxisParam.sParamZ.SetValue_SEL(MotionParamRef.m_SignPositiveZ);
	this->m_AxisParamList.push_back(AxisParam);

	//X軸歸零前移動距離
	AxisParam.sTitle = _T("Home Pre Move Dis");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("X Axis Home Pre Move Dis"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_HOME_PRE_DIST_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_HomePreMoveDisX, Precision);
	KeyName.Format(_T("Y Axis Home Pre Move Dis"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_HOME_PRE_DIST_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_HomePreMoveDisY, Precision);
	KeyName.Format(_T("Z Axis Home Pre Move Dis"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_HOME_PRE_DIST_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_HomePreMoveDisZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);

	//儲存X軸原點偏差值
	AxisParam.sTitle = _T("Home Offset");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("X Axis Home Offset"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_HOME_ORG_OFFSET_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_HomeOrgOffsetX, Precision);
	KeyName.Format(_T("Y Axis Home Offset"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_HOME_ORG_OFFSET_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_HomeOrgOffsetY, Precision);
	KeyName.Format(_T("Z Axis Home Offset"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_HOME_ORG_OFFSET_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_HomeOrgOffsetZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);

	//儲存X軸原點速度值
	AxisParam.sTitle = _T("Home Velocity");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Home Velocity"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_HOME_VELOCITY_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_HomeVelocityX, Precision);	
	KeyName.Format(_T("Y Axis Home Velocity"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_HOME_VELOCITY_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_HomeVelocityY, Precision);
	KeyName.Format(_T("Z Axis Home Velocity"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_HOME_VELOCITY_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_HomeVelocityZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);	

	//儲存 X Axis Board In Velocity
	AxisParam.sTitle = _T("Board In Velocity");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Board In Velocity"));
	AxisParam.sParamX.SetKeyName(KeyName);		
	AxisParam.sParamX.SetParamID(MOTION_PARAM_BOARD_IN_VELOCITY_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_BoardInVelocityX, Precision);	
	KeyName.Format(_T("Y Axis Board In Velocity"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_BOARD_IN_VELOCITY_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_BoardInVelocityY, Precision);	
	KeyName.Format(_T("Z Axis Board In Velocity"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_BOARD_IN_VELOCITY_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_BoardInVelocityZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);

	//儲存 X Axis Board Out Velocity
	AxisParam.sTitle = _T("Board Out Velocity");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Board Out Velocity"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_BOARD_OUT_VELOCITY_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_BoardOutVelocityX, Precision);	
	KeyName.Format(_T("Y Axis Board Out Velocity"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_BOARD_OUT_VELOCITY_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_BoardOutVelocityY, Precision);	
	KeyName.Format(_T("Z Axis Board Out Velocity"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_BOARD_OUT_VELOCITY_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_BoardOutVelocityZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);

	//儲存 X Axis Max Velocity
	AxisParam.sTitle = _T("Max Velocity");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Max Velocity"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_MAX_VELOCITY_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_MaxVelocityX, Precision);
	KeyName.Format(_T("Y Axis Max Velocity"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_MAX_VELOCITY_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_MaxVelocityY, Precision);
	KeyName.Format(_T("Z Axis Max Velocity"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_MAX_VELOCITY_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_MaxVelocityZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);

	//儲存 X Axis Grabbing Velocity
	AxisParam.sTitle = _T("Grabbing Velocity");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Grabbing Velocity"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_GRABBING_VELOCITY_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_GrabbingVelocityX, Precision);
	KeyName.Format(_T("Y Axis Grabbing Velocity"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_GRABBING_VELOCITY_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_GrabbingVelocityY, Precision);
	KeyName.Format(_T("Z Axis Grabbing Velocity"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_GRABBING_VELOCITY_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_GrabbingVelocityZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);
	
	//儲存 Axis Grab Map Velocity
	AxisParam.sTitle = _T("Grab Map Velocity");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Grab Map Velocity"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_GRAB_MAP_VELOCITY_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_GrabMapVelocityX, Precision);
	KeyName.Format(_T("Y Axis Grab Map Velocity"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_GRAB_MAP_VELOCITY_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_GrabMapVelocityY, Precision);
	KeyName.Format(_T("Z Axis Grab Map Velocity"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_GRAB_MAP_VELOCITY_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_GrabMapVelocityZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);	

	//儲存 Jog Moving Velocity //Jog速度, 單位mm/s
	AxisParam.sTitle = _T("Jog Moving Velocity");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Moving Velocity"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_JOG_MOVING_VELOCITY_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_JogMovingVelocityX, Precision);
	KeyName.Format(_T("Y Axis Moving Velocity"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_JOG_MOVING_VELOCITY_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_JogMovingVelocityY, Precision);
	KeyName.Format(_T("Z Axis Moving Velocity"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_JOG_MOVING_VELOCITY_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_JogMovingVelocityZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);

	//儲存 Jog Max Velocity //Jog速度, 單位mm/s
	AxisParam.sTitle = _T("Jog Max Velocity");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Max Velocity"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_JOG_MAX_VELOCITY_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_JogMaxVelocityX, Precision);
	KeyName.Format(_T("Y Axis Max Velocity"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_JOG_MAX_VELOCITY_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_JogMaxVelocityY, Precision);
	KeyName.Format(_T("Z Axis Max Velocity"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_JOG_MAX_VELOCITY_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_JogMaxVelocityZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);	

	//儲存 Jog Direct //Jog方向
	AxisParam.sTitle = _T("Jog Direction");
	AxisParam.sUnit = _T("um/sec");
	KeyName.Format(_T("X Axis Jog Direction"));
	AxisParam.sParamX.SetKeyName(KeyName);		
	AxisParam.sParamX.ClearSelList();	
	nID = FN_DISABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamX.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamX.AddSelItem(nID, strID);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_JOG_DIRECTION_X);
	AxisParam.sParamX.SetValue_SEL(MotionParamRef.m_JogDirectionX);	
	KeyName.Format(_T("Y Axis Jog Direction"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.ClearSelList();
	nID = FN_DISABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamY.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamY.AddSelItem(nID, strID);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_JOG_DIRECTION_Y);
	AxisParam.sParamY.SetValue_SEL(MotionParamRef.m_JogDirectionY);	
	KeyName.Format(_T("Z Axis Jog Direction"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.ClearSelList();
	nID = FN_DISABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamZ.AddSelItem(nID, strID);
	nID = FN_ENABLE;	strID = AOIDataDefine.GetPositiveNegativeText(nID);	AxisParam.sParamZ.AddSelItem(nID, strID);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_JOG_DIRECTION_Z);
	AxisParam.sParamZ.SetValue_SEL(MotionParamRef.m_JogDirectionZ);	
	this->m_AxisParamList.push_back(AxisParam);		

	//儲存 X Axis Acceleration //X軸加速度, 單位mm/s/s
	AxisParam.sTitle = _T("Max Acceleration");
	AxisParam.sUnit = _T("um/sec^2");
	KeyName.Format(_T("X Axis Acceleration Value"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_ACCEL_VALUE_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_AccelerationValueX, Precision);
	KeyName.Format(_T("Y Axis Acceleration Value"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_ACCEL_VALUE_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_AccelerationValueY, Precision);
	KeyName.Format(_T("Z Axis Acceleration Value"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_ACCEL_VALUE_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_AccelerationValueZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);
	
	//儲存 Acceleration Time
	AxisParam.sTitle = _T("Acceleration Time");
	AxisParam.sUnit = _T("sec");
	KeyName.Format(_T("X Axis Acceleration Time"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_ACCEL_TIME_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_AccelerationTimeX, Precision);
	KeyName.Format(_T("Y Axis Acceleration Time"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_ACCEL_TIME_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_AccelerationTimeY, Precision);
	KeyName.Format(_T("Z Axis Acceleration Time"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_ACCEL_TIME_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_AccelerationTimeZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);	

	//儲存 Deceleration Time
	AxisParam.sTitle = _T("Deceleration Time");
	AxisParam.sUnit = _T("sec");
	KeyName.Format(_T("X Axis Deceleration Time"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_DECEL_TIME_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_DecelerationTimeX, Precision);
	KeyName.Format(_T("Y Axis Deceleration Time"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_DECEL_TIME_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_DecelerationTimeY, Precision);
	KeyName.Format(_T("Z Axis Deceleration Time"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_DECEL_TIME_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_DecelerationTimeZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);

	/*
	//儲存X軸加速度最小時間, sec
	AxisParam.sTitle = _T("Acceleration Min Time");
	AxisParam.sUnit = _T("sec");
	KeyName.Format(_T("X Axis Acceleration Min Time"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_ACCEL_MIN_TIME_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_AccelerationMinTimeX, Precision);
	KeyName.Format(_T("Y Axis Acceleration Min Time"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_ACCEL_MIN_TIME_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_AccelerationMinTimeY, Precision);
	KeyName.Format(_T("Z Axis Acceleration Min Time"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_ACCEL_MIN_TIME_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_AccelerationMinTimeZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);
	*/

	//調整加速度啟動距離-單位um		
	AxisParam.sTitle = _T("Adjust Acceleration Distance");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("X Axis Adjust Acceleration Distance"));
	AxisParam.sParamX.SetKeyName(KeyName);		
	AxisParam.sParamX.SetParamID(MOTION_PARAM_ADJUST_ACCEL_DIST_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_AdjustAccclerationDistX, Precision);
	KeyName.Format(_T("Y Axis Adjust Acceleration Distance"));
	AxisParam.sParamY.SetKeyName(KeyName);	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_ADJUST_ACCEL_DIST_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_AdjustAccclerationDistY, Precision);
	KeyName.Format(_T("Z Axis Adjust Acceleration Distance"));
	AxisParam.sParamZ.SetKeyName(KeyName);	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_ADJUST_ACCEL_DIST_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_AdjustAccclerationDistZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);	

	//移動曲線
	AxisParam.sTitle = _T("Moving Curve Mode");
	AxisParam.sUnit = _T("");
	KeyName.Format(_T("X Axis Moving Curve Mode"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.ClearSelList();
	AxisParam.sParamX.AddSelItem(MOVE_CURVE_T, _T("T-Curve"));
	AxisParam.sParamX.AddSelItem(MOVE_CURVE_S, _T("S-Curve"));	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_MOVING_CURVE_MODE_X);
	AxisParam.sParamX.SetValue_SEL(MotionParamRef.m_MovingCurveModeX);
	KeyName.Format(_T("Y Axis Moving Curve Mode"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.ClearSelList();
	AxisParam.sParamY.AddSelItem(MOVE_CURVE_T, _T("T-Curve"));
	AxisParam.sParamY.AddSelItem(MOVE_CURVE_S, _T("S-Curve"));	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_MOVING_CURVE_MODE_Y);
	AxisParam.sParamY.SetValue_SEL(MotionParamRef.m_MovingCurveModeY);
	KeyName.Format(_T("Z Axis Moving Curve Mode"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.ClearSelList();
	AxisParam.sParamZ.AddSelItem(MOVE_CURVE_T, _T("T-Curve"));
	AxisParam.sParamZ.AddSelItem(MOVE_CURVE_S, _T("S-Curve"));	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_MOVING_CURVE_MODE_Z);
	AxisParam.sParamZ.SetValue_SEL(MotionParamRef.m_MovingCurveModeZ);
	this->m_AxisParamList.push_back(AxisParam);	
	
	//加減速度時間調整模式	
	AxisParam.sTitle = _T("Acc Time Adjust Mode");
	AxisParam.sUnit = _T("");
	KeyName.Format(_T("X Axis Acc Time Adjust Mode"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.ClearSelList();
	AxisParam.sParamX.AddSelItem(ACC_TIME_ADJUST_OFF, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_OFF));	
	AxisParam.sParamX.AddSelItem(ACC_TIME_ADJUST_FIX_T, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_FIX_T));	
	AxisParam.sParamX.AddSelItem(ACC_TIME_ADJUST_MIN_T, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_MIN_T));	
	AxisParam.sParamX.AddSelItem(ACC_TIME_ADJUST_GAMMA, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_GAMMA));	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_ACC_TIME_ADJUST_MODE_X);
	AxisParam.sParamX.SetValue_SEL(MotionParamRef.m_AccTimeAdjustModeX);
	KeyName.Format(_T("Y Axis Acc Time Adjust Mode"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.ClearSelList();
	AxisParam.sParamY.AddSelItem(ACC_TIME_ADJUST_OFF, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_OFF));	
	AxisParam.sParamY.AddSelItem(ACC_TIME_ADJUST_FIX_T, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_FIX_T));	
	AxisParam.sParamY.AddSelItem(ACC_TIME_ADJUST_MIN_T, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_MIN_T));	
	AxisParam.sParamY.AddSelItem(ACC_TIME_ADJUST_GAMMA, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_GAMMA));	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_ACC_TIME_ADJUST_MODE_Y);
	AxisParam.sParamY.SetValue_SEL(MotionParamRef.m_AccTimeAdjustModeY);
	KeyName.Format(_T("Z Axis Acc Time Adjust Mode"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.ClearSelList();
	AxisParam.sParamZ.AddSelItem(ACC_TIME_ADJUST_OFF, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_OFF));	
	AxisParam.sParamZ.AddSelItem(ACC_TIME_ADJUST_FIX_T, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_FIX_T));	
	AxisParam.sParamZ.AddSelItem(ACC_TIME_ADJUST_MIN_T, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_MIN_T));	
	AxisParam.sParamZ.AddSelItem(ACC_TIME_ADJUST_GAMMA, AOIDataDefine.GetMotionAccTimeAdjustText(ACC_TIME_ADJUST_GAMMA));	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_ACC_TIME_ADJUST_MODE_Z);
	AxisParam.sParamZ.SetValue_SEL(MotionParamRef.m_AccTimeAdjustModeZ);
	this->m_AxisParamList.push_back(AxisParam);	

	//三角波抑制
	AxisParam.sTitle = _T("Triangle Correction");
	AxisParam.sUnit = _T("");
	KeyName.Format(_T("X Axis Triangle Correction"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.ClearSelList();
	AxisParam.sParamX.AddSelItem(FN_ENABLE, AOIDataDefine.GetEnableText());
	AxisParam.sParamX.AddSelItem(FN_DISABLE, AOIDataDefine.GetDisableText());	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_TRIANGLE_CORRECTION_X);
	AxisParam.sParamX.SetValue_SEL(MotionParamRef.m_TriangleCorrectionX);
	KeyName.Format(_T("Y Axis Triangle Correction"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.ClearSelList();
	AxisParam.sParamY.AddSelItem(FN_ENABLE, AOIDataDefine.GetEnableText());
	AxisParam.sParamY.AddSelItem(FN_DISABLE, AOIDataDefine.GetDisableText());	
	AxisParam.sParamY.SetParamID(MOTION_PARAM_TRIANGLE_CORRECTION_Y);
	AxisParam.sParamY.SetValue_SEL(MotionParamRef.m_TriangleCorrectionY);
	KeyName.Format(_T("Z Axis Triangle Correction"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.ClearSelList();
	AxisParam.sParamZ.AddSelItem(FN_ENABLE, AOIDataDefine.GetEnableText());
	AxisParam.sParamZ.AddSelItem(FN_DISABLE, AOIDataDefine.GetDisableText());	
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_TRIANGLE_CORRECTION_Z);
	AxisParam.sParamZ.SetValue_SEL(MotionParamRef.m_TriangleCorrectionZ);
	this->m_AxisParamList.push_back(AxisParam);
	
	//測驗時間的偏移量, um			
	AxisParam.sTitle = _T("Test Time Distance");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("X Axis Test Time Distance"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_TEST_TIME_DISTANCE_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_TestTimeDistanceX, 0);
	KeyName.Format(_T("Y Axis Test Time Distance"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_TEST_TIME_DISTANCE_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_TestTimeDistanceY, 0);
	KeyName.Format(_T("Z Axis Test Time Distance"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_TEST_TIME_DISTANCE_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_TestTimeDistanceZ, 0);	
	this->m_AxisParamList.push_back(AxisParam);

	//儲存 X 軸的最大範圍
	AxisParam.sTitle = _T("Limit Max");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("X Axis Limit Max"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LIMIT_MAX_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LimitMaxX, Precision);
	KeyName.Format(_T("Y Axis Limit Max"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LIMIT_MAX_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LimitMaxY, Precision);
	KeyName.Format(_T("Z Axis Limit Max"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LIMIT_MAX_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LimitMaxZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);

	//儲存 X 軸的最小範圍
	AxisParam.sTitle = _T("Limit Min");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("X Axis Limit Min"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LIMIT_MIN_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LimitMinX, Precision);
	KeyName.Format(_T("Y Axis Limit Min"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LIMIT_MIN_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LimitMinY, Precision);
	KeyName.Format(_T("Z Axis Limit Min"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LIMIT_MIN_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LimitMinZ, Precision);
	this->m_AxisParamList.push_back(AxisParam);
	
	//儲存 X 軸的範圍內縮值
	AxisParam.sTitle = _T("Limit Margin");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("X Axis Limit Margin"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LIMIT_MARGIN_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LimitMarginX, Precision);
	KeyName.Format(_T("Y Axis Limit Margin"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LIMIT_MARGIN_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LimitMarginY, Precision);
	KeyName.Format(_T("Z Axis Limit Margin"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LIMIT_MARGIN_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LimitMarginZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);	

	//儲存 Stage ConveyerPos X
	AxisParam.sTitle = _T("Stage Start Position");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Stage Start Pos X"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_STAGE_START_POS_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_StageStartPosX, Precision);
	KeyName.Format(_T("Stage Start Pos Y"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_STAGE_START_POS_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_StageStartPosY, Precision);
	KeyName.Format(_T("Stage Start Pos Z"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_STAGE_START_POS_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_StageStartPosZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);

	//機台離開位置
	AxisParam.sTitle = _T("Stage Leave Position");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Stage Leave Pos X"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_STAGE_LEAVE_POS_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_StageLeavePosX, Precision);
	KeyName.Format(_T("Stage Leave Pos Y"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_STAGE_LEAVE_POS_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_StageLeavePosY, Precision);
	KeyName.Format(_T("Stage Leave Pos Z"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_STAGE_LEAVE_POS_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_StageLeavePosZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);	
	
	//進板前座標
	AxisParam.sTitle = _T("Before PCB-In Position");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Before PCB-In Pos X"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_BEFORE_PCB_IN_POS_X);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_BeforePCBInPosX, Precision);
	KeyName.Format(_T("Before PCB-In Pos Y"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_BEFORE_PCB_IN_POS_Y);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_BeforePCBInPosY, Precision);
	KeyName.Format(_T("Before PCB-In Pos Z"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_BEFORE_PCB_IN_POS_Z);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_BeforePCBInPosZ, Precision);	
	this->m_AxisParamList.push_back(AxisParam);

	//A軌道停板座標-停右邊
	AxisParam.sTitle = _T("PCB-Stop Position Lane A");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("PCB-Stop Pos X Lane A"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_PCB_STOP_POS_X_LA);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_PCBStopRPosX_LA, Precision);
	KeyName.Format(_T("PCB-Stop Pos Y Lane A"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_PCB_STOP_POS_Y_LA);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_PCBStopRPosY_LA, Precision);
	KeyName.Format(_T("PCB-Stop Pos Z Lane A"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_PCB_STOP_POS_Z_LA);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_PCBStopRPosZ_LA, Precision);	
	this->m_AxisParamList.push_back(AxisParam);

	//B軌道停板座標-停右邊
	AxisParam.sTitle = _T("PCB-Stop Position Lane B");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("PCB-Stop Pos X Lane B"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_PCB_STOP_RIGHT_POS_X_LB);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_PCBStopRPosX_LB, Precision);
	KeyName.Format(_T("PCB-Stop Pos Y Lane B"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_PCB_STOP_RIGHT_POS_Y_LB);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_PCBStopRPosY_LB, Precision);
	KeyName.Format(_T("PCB-Stop Pos Z Lane B"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_PCB_STOP_RIGHT_POS_Z_LB);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_PCBStopRPosZ_LB, Precision);	
	this->m_AxisParamList.push_back(AxisParam);
	
	//A軌道停板座標-停左邊
	AxisParam.sTitle = _T("PCB-Stop Left Position Lane A");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("PCB-Stop Left Pos X Lane A"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_PCB_STOP_LEFT_POS_X_LA);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_PCBStopLPosX_LA, Precision);
	KeyName.Format(_T("PCB-Stop Left Pos Y Lane A"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_PCB_STOP_LEFT_POS_Y_LA);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_PCBStopLPosY_LA, Precision);
	KeyName.Format(_T("PCB-Stop Left Pos Z Lane A"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_PCB_STOP_LEFT_POS_Z_LA);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_PCBStopLPosZ_LA, Precision);	
	this->m_AxisParamList.push_back(AxisParam);

	//B軌道停板座標-停左邊
	AxisParam.sTitle = _T("PCB-Stop Left Position Lane B");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("PCB-Stop Left Pos X Lane B"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_PCB_STOP_LEFT_POS_X_LB);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_PCBStopLPosX_LB, Precision);
	KeyName.Format(_T("PCB-Stop Left Pos Y Lane B"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_PCB_STOP_LEFT_POS_Y_LB);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_PCBStopLPosY_LB, Precision);
	KeyName.Format(_T("PCB-Stop Left Pos Z Lane B"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_PCB_STOP_LEFT_POS_Z_LB);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_PCBStopLPosZ_LB, Precision);	
	this->m_AxisParamList.push_back(AxisParam);
	
	const bool bShowBLed=false;

	//A軌道LED停板的位置-停右邊
	AxisParam.sTitle = _T("Lane-LED Right Stop Position Lane A");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Lane-LED Right Stop Pos X Lane A"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_X_LA);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LaneLedStopRPosX_LA, Precision);
	KeyName.Format(_T("Lane-LED Right Stop Pos Y Lane A"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Y_LA);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LaneLedStopRPosY_LA, Precision);
	KeyName.Format(_T("Lane-LED Right Stop Pos Z Lane A"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Z_LA);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LaneLedStopRPosZ_LA, Precision);	
	this->m_AxisParamList.push_back(AxisParam);
	
	//A軌道LED減速的位置-停右邊
	AxisParam.sTitle = _T("Lane-LED Right Slow Position Lane A");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Lane-LED Right Slow Pos X Lane A"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_X_LA);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LaneLedSlowRPosX_LA, Precision);
	KeyName.Format(_T("Lane-LED Right Slow Pos Y Lane A"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Y_LA);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LaneLedSlowRPosY_LA, Precision);
	KeyName.Format(_T("Lane-LED Right Slow Pos Z Lane A"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Z_LA);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LaneLedSlowRPosZ_LA, Precision);	
	this->m_AxisParamList.push_back(AxisParam);
	
	//B軌道LED停板的位置-停右邊
	AxisParam.sTitle = _T("Lane-LED Right Stop Position Lane B");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Lane-LED Right Stop Pos X Lane B"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_X_LB);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LaneLedStopRPosX_LB, Precision);
	KeyName.Format(_T("Lane-LED Right Stop Pos Y Lane B"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Y_LB);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LaneLedStopRPosY_LB, Precision);
	KeyName.Format(_T("Lane-LED Right Stop Pos Z Lane B"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Z_LB);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LaneLedStopRPosZ_LB, Precision);	
	if ( true==bShowBLed ) { this->m_AxisParamList.push_back(AxisParam); }
	
	//B軌道LED減速的位置-停右邊
	AxisParam.sTitle = _T("Lane-LED Right Slow Position Lane B");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Lane-LED Right Slow Pos X Lane B"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_X_LB);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LaneLedSlowRPosX_LB, Precision);
	KeyName.Format(_T("Lane-LED Right Slow Pos Y Lane B"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Y_LB);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LaneLedSlowRPosY_LB, Precision);
	KeyName.Format(_T("Lane-LED Right Slow Pos Z Lane B"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Z_LB);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LaneLedSlowRPosZ_LB, Precision);	
	if ( true==bShowBLed ) { this->m_AxisParamList.push_back(AxisParam); }

	//A軌道LED停板的位置-停左邊
	AxisParam.sTitle = _T("Lane-LED Left Stop Position Lane A");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Lane-LED Left Stop Pos X Lane A"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LANE_LED_LEFT_STOP_POS_X_LA);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LaneLedStopLPosX_LA, Precision);
	KeyName.Format(_T("Lane-LED Left Stop Pos Y Lane A"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Y_LA);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LaneLedStopLPosY_LA, Precision);
	KeyName.Format(_T("Lane-LED Left Stop Pos Z Lane A"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Z_LA);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LaneLedStopLPosZ_LA, Precision);	
	this->m_AxisParamList.push_back(AxisParam);
	
	//A軌道LED減速的位置-停左邊
	AxisParam.sTitle = _T("Lane-LED Left Slow Position Lane A");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Lane-LED Left Slow Pos X Lane A"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_X_LA);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LaneLedSlowLPosX_LA, Precision);
	KeyName.Format(_T("Lane-LED Left Slow Pos Y Lane A"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Y_LA);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LaneLedSlowLPosY_LA, Precision);
	KeyName.Format(_T("Lane-LED Left Slow Pos Z Lane A"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Z_LA);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LaneLedSlowLPosZ_LA, Precision);	
	this->m_AxisParamList.push_back(AxisParam);
	
	//B軌道LED停板的位置-停左邊
	AxisParam.sTitle = _T("Lane-LED Left Stop Position Lane B");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Lane-LED Left Stop Pos X Lane B"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LANE_LED_LEFT_STOP_POS_X_LB);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LaneLedStopLPosX_LB, Precision);
	KeyName.Format(_T("Lane-LED Left Stop Pos Y Lane B"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Y_LB);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LaneLedStopLPosY_LB, Precision);
	KeyName.Format(_T("Lane-LED Left Stop Pos Z Lane B"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Z_LB);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LaneLedStopLPosZ_LB, Precision);	
	if ( true==bShowBLed ) { this->m_AxisParamList.push_back(AxisParam); }
	
	//B軌道LED減速的位置-停左邊
	AxisParam.sTitle = _T("Lane-LED Left Slow Position Lane B");
	AxisParam.sUnit = _T("um");
	KeyName.Format(_T("Lane-LED Left Slow Pos X Lane B"));
	AxisParam.sParamX.SetKeyName(KeyName);	
	AxisParam.sParamX.SetParamID(MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_X_LB);
	AxisParam.sParamX.SetValue_DBL(MotionParamRef.m_LaneLedSlowLPosX_LB, Precision);
	KeyName.Format(_T("Lane-LED Left Slow Pos Y Lane B"));
	AxisParam.sParamY.SetKeyName(KeyName);
	AxisParam.sParamY.SetParamID(MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Y_LB);
	AxisParam.sParamY.SetValue_DBL(MotionParamRef.m_LaneLedSlowLPosY_LB, Precision);
	KeyName.Format(_T("Lane-LED Left Slow Pos Z Lane B"));
	AxisParam.sParamZ.SetKeyName(KeyName);
	AxisParam.sParamZ.SetParamID(MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Z_LB);
	AxisParam.sParamZ.SetValue_DBL(MotionParamRef.m_LaneLedSlowLPosZ_LB, Precision);	
	if ( true==bShowBLed ) { this->m_AxisParamList.push_back(AxisParam); }

	this->BuildAxisParamListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::BuildParamListWnd()
{
	size_t  i = 0;
	int     nItem=0;
	CString str;
	CString ItemText;
	TMotionParamItem *MotionParamPtr = NULL;
	CListCtrl &ListCtrl = m_ParamListWnd;
	const size_t ParamCount = this->m_ParamList.size();

	SetActParamUni(NULL);
	ListCtrl.DeleteAllItems();
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<ParamCount; i++ )
	{
		MotionParamPtr = &(m_ParamList[i]);
		if ( NULL == MotionParamPtr ) { continue; }

		MotionParamPtr->sParam.SetListCtrl(&ListCtrl);
		MotionParamPtr->sParam.SetItemIndex(nItem);
		MotionParamPtr->sParam.SetSubItemIndex(1);

		str = MotionParamPtr->sParam.GetKeyName();
		ItemText = LoadMultiLanguageString(str, str);
		ListCtrl.InsertItem(nItem, ItemText);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, 1, MotionParamPtr->sParam.GetParamText());		
		ListCtrl.SetItemText(nItem, 2, MotionParamPtr->sUnit);		
		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::BuildAxisParamListWnd()
{
	size_t i = 0;
	int    nItem=0;
	CString str;
	CString ItemText;
	TAxisParamItem *AxisParamPtr = NULL;
	CListCtrl &ListCtrl = m_AxisParamListWnd;
	const size_t ParamCount = (this->m_AxisParamList.size());

	SetActParamUni(NULL);
	ListCtrl.DeleteAllItems();
	ListCtrl.SetRedraw(FALSE);
	for ( i=0; i<ParamCount; i++ )
	{
		AxisParamPtr = &(m_AxisParamList[i]);
		if ( NULL == AxisParamPtr ) { continue; }

		AxisParamPtr->sParamX.SetListCtrl(&ListCtrl);
		AxisParamPtr->sParamX.SetItemIndex(nItem);
		AxisParamPtr->sParamX.SetSubItemIndex(AXIS_PARAM_X);

		AxisParamPtr->sParamY.SetListCtrl(&ListCtrl);
		AxisParamPtr->sParamY.SetItemIndex(nItem);
		AxisParamPtr->sParamY.SetSubItemIndex(AXIS_PARAM_Y);

		AxisParamPtr->sParamZ.SetListCtrl(&ListCtrl);
		AxisParamPtr->sParamZ.SetItemIndex(nItem);
		AxisParamPtr->sParamZ.SetSubItemIndex(AXIS_PARAM_Z);

		str = AxisParamPtr->sTitle;
		ItemText = LoadMultiLanguageString(str, str);
		ListCtrl.InsertItem(nItem, ItemText);
		ListCtrl.SetItemData(nItem, i);

		ListCtrl.SetItemText(nItem, AXIS_PARAM_X, AxisParamPtr->sParamX.GetParamText());
		ListCtrl.SetItemText(nItem, AXIS_PARAM_Y, AxisParamPtr->sParamY.GetParamText());
		ListCtrl.SetItemText(nItem, AXIS_PARAM_Z, AxisParamPtr->sParamZ.GetParamText());

		ListCtrl.SetItemText(nItem, AXIS_PARAM_UNIT, AxisParamPtr->sUnit);		
		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CMotionCtrlPaneParam::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( pMsg->message )
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
			if ( m_EditCtrl.IsWindowVisible() == TRUE) 
			{
				ExecUpdateParamByEdit();
				m_EditCtrl.ShowWindow(SW_HIDE);	
				m_EditCtrl.SetWindowText(_T(""));
				return TRUE;								
			}
			if ( m_ComboxCtrl.IsWindowVisible() == TRUE )
			{
				ExecUpdateParamByCombox();
				m_ComboxCtrl.ShowWindow(SW_HIDE);
				JetAPI::ClearCombox(m_ComboxCtrl);
				return TRUE;				
			}
			break;
		case VK_ESCAPE:
			ExecReleaseParamCtrl();
			return TRUE;
			break;
		}
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnDblclkAxisParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here		
	LPNMITEMACTIVATE pItem = (LPNMITEMACTIVATE)pNMHDR;
	
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	SetActParamUni(NULL);
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	ExecDblclkAxisParamListWnd(m_AxisParamListWnd, nItem, nSubItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pItem = (LPNMITEMACTIVATE)pNMHDR;
	
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;	
	SetActParamUni(NULL);
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) { return; }
	ExecDblclkParamListWnd(m_ParamListWnd, nItem, nSubItem);	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnKillfocusGridEditWnd() 
{
	// TODO: Add your control notification handler code here
	if ( m_EditCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByEdit();
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_EditCtrl.SetWindowText(_T(""));
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnKillfocusGridCombo() 
{
	// TODO: Add your control notification handler code here
	if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
	ExecUpdateParamByCombox();
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);		
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnSelchangeGridCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ExecUpdateParamByCombox() == false )
	{	return; }
	m_ComboxCtrl.ShowWindow(SW_HIDE);	
	JetAPI::ClearCombox(m_ComboxCtrl);		
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::BuildAllParamList()
{
	this->BuildParamList();
	this->BuildAxisParamList();
	return true;	
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::SaveAllParamToINI()
{
	this->SaveParamToINI();
	this->SaveAxisParamToINI();
}
//-------------------------------------------------------------------------------------//
CString CMotionCtrlPaneParam::GetParamIniFilename()
{
	CString Filename;
	Filename = AOIDataCollect.GetSystemParamFilename();
	return Filename;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::SaveParamToINI()
{
	size_t i = 0;
	CString str;
	CString String;
	CString KeyName;
	CString Section;
	CString Filename;	
	CString ErrorString;
	CParamUni *ParamPtr = NULL;
	TMotionParamItem *MotinParamPtr = NULL;
	const size_t ParamCount = this->m_ParamList.size();
	Filename = GetParamIniFilename();
	for ( i=0; i<ParamCount; i++ )
	{
		MotinParamPtr = &(m_ParamList[i]);
		if ( NULL == MotinParamPtr ) { continue; }

		ParamPtr = &(MotinParamPtr->sParam);
		Section = ParamPtr->GetSection();
		KeyName = ParamPtr->GetKeyName();
		String  = ParamPtr->GetValue_STR();
		if ( JetAPI::SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
		{
			JetAPI::ShowMessageBox(ErrorString);
			return false;	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::SaveAxisParamToINI()
{
	size_t i = 0;
	CString str;
	CString String;
	CString KeyName;
	CString Section;
	CString Filename;	
	CString ErrorString;
	CParamUni *ParamPtr = NULL;
	TAxisParamItem *AxisParamPtr = NULL;
	const size_t ParamCount = this->m_AxisParamList.size();
	Filename = GetParamIniFilename();
	for ( i=0; i<ParamCount; i++ )
	{
		AxisParamPtr = &(m_AxisParamList[i]);
		if ( NULL == AxisParamPtr ) { continue; }

		ParamPtr = &(AxisParamPtr->sParamX);
		Section = ParamPtr->GetSection();
		KeyName = ParamPtr->GetKeyName();
		String  = ParamPtr->GetValue_STR();
		if ( JetAPI::SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
		{
			JetAPI::ShowMessageBox(ErrorString);
			return false;	
		}

		ParamPtr = &(AxisParamPtr->sParamY);
		Section = ParamPtr->GetSection();
		KeyName = ParamPtr->GetKeyName();
		String  = ParamPtr->GetValue_STR();
		if ( JetAPI::SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
		{
			JetAPI::ShowMessageBox(ErrorString);
			return false;	
		}

		ParamPtr = &(AxisParamPtr->sParamZ);
		Section = ParamPtr->GetSection();
		KeyName = ParamPtr->GetKeyName();
		String  = ParamPtr->GetValue_STR();
		if ( JetAPI::SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
		{
			JetAPI::ShowMessageBox(ErrorString);
			return false;	
		}
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
void CMotionCtrlPaneParam::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		SwitchMultiLanguage();
		this->BuildAllParamList();	
	}
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::ExecDblclkParamListWnd(CThisListCtrl_14 &ListCtrl, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < 1 ) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_ParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t            i=0;
	int               nSelIdx=0;
	int               nValue=0;
	CRect             ItemRect;
	RECT              CtrlRect={0};	
	CString           ItemText;	
	const int         Offset = 2;
	CParamUni        *ParamPtr = NULL;
	TMotionParamItem *MotionParamPtr = &(m_ParamList[ParamIndex]);	
	ParamPtr = &(MotionParamPtr->sParam);
	if ( NULL == ParamPtr ) { return true; }
	const bool      ReadOnly = ParamPtr->GetReadOnly();	
	if ( true == ReadOnly ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();

	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);	
	::OffsetRect(&CtrlRect, 0, -2);	
	SetActParamUni(ParamPtr);
	if ( PARAM_DATA_SEL == DataType )
	{
		CComboBox &ComboxCtrl = m_ComboxCtrl;
		if ( ComboxCtrl.GetSafeHwnd() != NULL )
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(ComboxCtrl);
			for ( i=0; i<SelItemCount; i++ )
			{
				if ( ParamPtr->GetSelItem(i, true, nValue, ItemText) == false ) { continue; }
				ComboxCtrl.InsertString(nSelIdx, ItemText);
				ComboxCtrl.SetItemData(nSelIdx, nValue);
				nSelIdx ++;
			}			
			JetAPI::SetComboxCurSel(ComboxCtrl, ParamPtr->GetSelParam());
			ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
			ComboxCtrl.SetFocus();
			ComboxCtrl.ShowDropDown();
			ComboxCtrl.ShowWindow(SW_SHOW);
			ComboxCtrl.BringWindowToTop();			
			ListCtrl.UpdateWindow();
			ComboxCtrl.Invalidate();
		}	
	}
	else
	{
		CEdit	&EditCtrl = m_EditCtrl;
		if ( EditCtrl.GetSafeHwnd() != NULL )
		{	
			::OffsetRect(&CtrlRect, 1, 1);
			::InflateRect(&CtrlRect, Offset, Offset);			
			EditCtrl.SetWindowText(ItemText);
			EditCtrl.MoveWindow(&CtrlRect, FALSE);
			EditCtrl.SetFocus();
			EditCtrl.SetSel(0,-1);			
			EditCtrl.ShowWindow(SW_SHOW);	
			EditCtrl.BringWindowToTop();
			ListCtrl.UpdateWindow();
			EditCtrl.Invalidate();			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::ExecDblclkAxisParamListWnd(CThisListCtrl_14 &ListCtrl, int nItem, int nSubItem)
{
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }
	if ( nSubItem < 1 ) { return true; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	const size_t ParamCount = m_AxisParamList.size();
	if ( ParamIndex >= ParamCount ) { return false; }		
	
	size_t          i=0;
	int             nSelIdx=0;
	int             nValue=0;
	CRect           ItemRect;
	RECT            CtrlRect={0};	
	CString         ItemText;	
	const int       Offset = 2;
	CParamUni      *ParamPtr = NULL;
	TAxisParamItem *AxisParamPtr = &(m_AxisParamList[ParamIndex]);	
	switch ( nSubItem )
	{
	case AXIS_PARAM_X:	ParamPtr = &(m_AxisParamList[ParamIndex].sParamX);	break;
	case AXIS_PARAM_Y:	ParamPtr = &(m_AxisParamList[ParamIndex].sParamY);	break;
	case AXIS_PARAM_Z:	ParamPtr = &(m_AxisParamList[ParamIndex].sParamZ);	break;
	}
	if ( NULL == ParamPtr ) { return true; }
	const bool      ReadOnly = ParamPtr->GetReadOnly();	
	if ( true == ReadOnly ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();

	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
	{	return false; }
	ItemText = ListCtrl.GetItemText(nItem, nSubItem);

	CtrlRect = ItemRect;
	ListCtrl.ClientToScreen(&CtrlRect);
	this->ScreenToClient(&CtrlRect);	
	SetActParamUni(ParamPtr);
	if ( PARAM_DATA_SEL == DataType )
	{
		CComboBox &ComboxCtrl = m_ComboxCtrl;
		if ( ComboxCtrl.GetSafeHwnd() != NULL )
		{
			nSelIdx = 0;
			JetAPI::ClearCombox(ComboxCtrl);
			for ( i=0; i<SelItemCount; i++ )
			{
				if ( ParamPtr->GetSelItem(i, true, nValue, ItemText) == false ) { continue; }
				ComboxCtrl.InsertString(nSelIdx, ItemText);
				ComboxCtrl.SetItemData(nSelIdx, nValue);
				nSelIdx ++;
			}			
			JetAPI::SetComboxCurSel(ComboxCtrl, ParamPtr->GetSelParam());
			ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
			ComboxCtrl.SetFocus();
			ComboxCtrl.ShowDropDown();
			ComboxCtrl.ShowWindow(SW_SHOW);
			ComboxCtrl.BringWindowToTop();			
			ListCtrl.UpdateWindow();
			ComboxCtrl.Invalidate();
		}	
	}
	else
	{
		CEdit	&EditCtrl = m_EditCtrl;
		if ( EditCtrl.GetSafeHwnd() != NULL )
		{	
			::OffsetRect(&CtrlRect, 1, 1);
			::InflateRect(&CtrlRect, Offset, Offset);			
			EditCtrl.SetWindowText(ItemText);
			EditCtrl.MoveWindow(&CtrlRect, FALSE);
			EditCtrl.SetFocus();
			EditCtrl.SetSel(0,-1);			
			EditCtrl.ShowWindow(SW_SHOW);	
			EditCtrl.BringWindowToTop();
			ListCtrl.UpdateWindow();
			EditCtrl.Invalidate();			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::ExecReleaseParamCtrl()
{	
	m_EditCtrl.ShowWindow(SW_HIDE);	
	m_ComboxCtrl.ShowWindow(SW_HIDE);

	m_EditCtrl.SetWindowText(_T(""));	
	JetAPI::ClearCombox(m_ComboxCtrl);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::ExecUpdateParamByEdit()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	if ( PARAM_DATA_SEL == DataType ) { return true; }
	SetActParamUni(NULL);	

	CString ItemText;	
	CEdit	&EditCtrl = m_EditCtrl;
	MOTION_PARAM_ID ParamID = (MOTION_PARAM_ID)(ParamPtr->GetParamID());
	TMotionParameter &MotionParameterPtr = MotionCtrlPtr->GetMotionParameter();	
	EditCtrl.GetWindowText(ItemText);
	if ( ParamPtr->SetNewValue(ItemText) == false )
	{		
		ItemText = ParamPtr->GetParamText();
		EditCtrl.SetWindowText(ItemText);
		return false;
	}
	if ( CMotion_Basic::SetMotionParameterStringByID(ParamID, MotionParameterPtr, ItemText) == false )
	{	return true; }
	
	CThisListCtrl_14 *pListCtrl = (CThisListCtrl_14*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem>=0 && nItem<ItemCount )
		{	
			pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
			pListCtrl->SetFocus();
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionCtrlPaneParam::ExecUpdateParamByCombox()
{
	CParamUni   *ParamPtr = GetActParamUni();
	if ( NULL == ParamPtr ) { return true; }	
	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	if ( PARAM_DATA_SEL != DataType ) { return true; }
	SetActParamUni(NULL);

	CString ItemText;	
	CComboBox &ComboxCtrl = m_ComboxCtrl;
	MOTION_PARAM_ID ParamID = (MOTION_PARAM_ID)(ParamPtr->GetParamID());
	TMotionParameter &MotionParameterPtr = MotionCtrlPtr->GetMotionParameter();
	const int nCurSel = ComboxCtrl.GetCurSel();
	if ( nCurSel < 0 ) { return true; }
	const DWORD Param = ComboxCtrl.GetItemData(nCurSel);
	if ( Param == ParamPtr->GetSelParam() ) { return false; }	
	ItemText.Format(_T("%d"),Param);
	if ( ParamPtr->SetNewValue_SEL(Param) == false )
	{	return false; }
	if ( CMotion_Basic::SetMotionParameterStringByID(ParamID, MotionParameterPtr, ItemText) == false )
	{	return true; }
	
	ItemText = ParamPtr->GetParamText();
	CThisListCtrl_14 *pListCtrl = (CThisListCtrl_14*)(ParamPtr->GetListCtrl());
	const int nItem = ParamPtr->GetItemIndex();
	const int nSubItem = ParamPtr->GetSubItemIndex();	
	if ( NULL != pListCtrl )
	{
		const int ItemCount = pListCtrl->GetItemCount();
		if ( nItem<0 || nItem>=ItemCount ) { return true; }	
		pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
		pListCtrl->SetFocus();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//