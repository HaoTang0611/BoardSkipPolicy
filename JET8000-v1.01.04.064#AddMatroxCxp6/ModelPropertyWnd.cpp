// ModelPropertyWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ModelPropertyWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelPropertyWnd dialog
//-------------------------------------------------------------------------------------//
CModelPropertyWnd::CModelPropertyWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CModelPropertyWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelPropertyWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ModelPtr = NULL;
	m_ExtendX = 0;
	m_ExtendY = 0;
	m_ExtendAutoAdjust = true;
	m_BodySizeX = 0;
	m_BodySizeY = 0;
	m_BodyHeight = 0;	
	m_StopLandListBeSelected = false;

	JetAPI::InitialUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);
	m_ImageW = m_UniFrameList[0].ImageW;
	m_ImageH = m_UniFrameList[0].ImageH;
	m_ImageStep = m_UniFrameList[0].ImageStep;
	m_BitCount = m_UniFrameList[0].BitCount;	

	m_ShowImageW = m_ImageW;
	m_ShowImageH = m_ImageH;
	m_ShowImageStep = m_ImageStep;
	m_ShowBitCount = m_BitCount;	
	m_ShowImagePtr = NULL;	
	m_ShowBufferSize = 0;;

	m_SpaceW = m_ImageW;
	m_SpaceH = m_ImageH;
	m_SpaceStep = m_ImageStep;
	m_SpaceBitCnt = m_BitCount;	
	m_MaskPtr = NULL;
	m_SpacePtr = NULL;	

	POINT Pt={0,0};
	m_ImageZoom = 1.0;
	m_ImageOffset.x = m_ImageOffset.y = 0.0;
	
	m_DrawRoiRect = false;
	m_BuidlRoiRect = false;
	m_RoiRectHeight = 0.0;
	m_RoiRect.left = m_RoiRect.right = 0;
	m_RoiRect.top = m_RoiRect.bottom = 0;
	m_MousePosLast = Pt;//滑鼠座標-上一個
	m_MousePosFirst = Pt;//滑鼠座標-第1個
	m_MousePosCurrent = Pt;//滑鼠座標-現今
	m_MousePosImageWnd = Pt;//滑鼠座標-圖像視窗
	m_MousePosMode = CURSOR_POS_NONE;
	return;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelPropertyWnd)	
	DDX_Control(pDX, MODELPTY_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, MODELPTY_LAND_GROUP_COMBO, m_LandGroupCombox);
	DDX_Control(pDX, MODELPTY_LAND_TYPE_COMBO, m_LandTypeCombox);	
	DDX_Control(pDX, MODELPTY_LAND_LIST_WND, m_LandListCtrl);		
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CModelPropertyWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CModelPropertyWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_SETCURSOR()
	ON_CBN_SELCHANGE(MODELPTY_LAND_GROUP_COMBO, OnSelchangeLandGroupCombo)
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, MODELPTY_LAND_LIST_WND, OnItemchangedLandListWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(MODELPTY_BODY_SIZE_XYZ_BTN, OnBodySizeXYZBtn)
	ON_BN_CLICKED(MODELPTY_LEAD_SIZE_XYZ_BTN, OnLeadSizeXYZBtn)
	ON_BN_CLICKED(MODELPTY_LEAD_TIP_SIZE_XYZ_BTN, OnLeadTipSizeXYZBtn)
	ON_BN_CLICKED(MODELPTY_LEAD_SHOULDER_SIZE_XYZ_BTN, OnLeadShoulderSizeXYZBtn)
	ON_EN_KILLFOCUS(MODELPTY_BODY_SIZE_X_EDIT, OnKillfocusBodySizeXEdit)
	ON_EN_KILLFOCUS(MODELPTY_BODY_SIZE_Y_EDIT, OnKillfocusBodySizeYEdit)
	ON_EN_KILLFOCUS(MODELPTY_BODY_SIZE_Z_EDIT, OnKillfocusBodySizeZEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_SIZE_X_EDIT, OnKillfocusLeadSizeXEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_SIZE_Y_EDIT, OnKillfocusLeadSizeYEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_SIZE_Z_EDIT, OnKillfocusLeadSizeZEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_TIP_SIZE_X_EDIT, OnKillfocusLeadTipSizeXEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_TIP_SIZE_Y_EDIT, OnKillfocusLeadTipSizeYEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_TIP_SIZE_Z_EDIT, OnKillfocusLeadTipSizeZEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_SHOULDER_SIZE_X_EDIT, OnKillfocusLeadShoulderSizeXEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_SHOULDER_SIZE_Y_EDIT, OnKillfocusLeadShoulderSizeYEdit)
	ON_EN_KILLFOCUS(MODELPTY_LEAD_SHOULDER_SIZE_Z_EDIT, OnKillfocusLeadShoulderSizeZEdit)
	ON_BN_CLICKED(MODELPTY_SHOW_LAND_LINE_CHK, OnShowLandLineChk)
	ON_EN_KILLFOCUS(MODELPTY_EXTEND_X_EDIT, OnKillfocusExtendXEdit)
	ON_EN_KILLFOCUS(MODELPTY_EXTEND_Y_EDIT, OnKillfocusExtendYEdit)
	ON_BN_CLICKED(MODELPTY_EXTEND_AUTO_ADJUST_CHK, OnExtendAutoAdjustChk)
	ON_BN_CLICKED(MODELPTY_LAND_PAD_ALIGN_CHK, OnLandPadAlignChk)	
	ON_BN_CLICKED(MODELPTY_LAND_PART_ALIGN_CHK, OnLandPartAlignChk)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelPropertyWnd message handlers
BOOL CModelPropertyWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_LandListCtrl);
	BuildLandListWndHeader();

	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	m_ImageWnd.GetClientRect(&m_ImageWndRect);
	m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, 0x000000);	
	m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, 0x000000);	
	m_ImageResolution.x = AOIDataCollect.GetCameraResolutionX(CameraID);
	m_ImageResolution.y = AOIDataCollect.GetCameraResolutionY(CameraID);	
	CWnd::CheckDlgButton(MODELPTY_SHOW_LAND_LINE_CHK, TRUE);
	CreateShowBuffer();
	SwitchFrameImage();	
	CreateBKImage();
	BuildActiveObjList(m_ModelPtr, false);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	AOIDataDefine.BuildLandTypeCombox(m_LandTypeCombox);	
	
	BuildModelProperty();
	BuildLandGroupCombox();
	UpdateModelPropertyToUI();
	RedrawWnd();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseShowImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	CWnd *WndPtr = NULL;
	m_DrawRoiRect = false;	
	m_BuidlRoiRect = false;
	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = cx-4;
		WndRect.bottom = cy-4;
		m_ImageWnd.MoveWindow(&WndRect);
		m_ImageWnd.GetClientRect(&m_ImageWndRect);
		m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, 0x000000);	
		m_ImageWndMemDC2.CreateMemDC(&m_ImageWnd, 0x000000);	
		CreateBKImage();
	}

	WndPtr = CWnd::GetDlgItem(MODELPTY_LAND_PARAM_GROUP);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		//WndRect.right = cx-4;
		WndRect.bottom = cy-4;
		WndPtr->MoveWindow(&WndRect);
	}

	if ( m_LandListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_LandListCtrl.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		//WndRect.right = cx-4;
		WndRect.bottom = cy-8;
		m_LandListCtrl.MoveWindow(&WndRect);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 640;
}
//-------------------------------------------------------------------------------------//
BOOL CModelPropertyWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UINT ControlID = pWnd->GetDlgCtrlID();	

	CURSOR_POS_MODE OldCursorMode = m_MousePosMode;
	CURSOR_POS_MODE CursorMode = CheckCursorPosMode(m_MousePosImageWnd);
	m_MousePosMode = CursorMode;
	
	if ( CursorMode != OldCursorMode )
	{	CModelPropertyWnd::RedrawWnd();	}
	if ( CURSOR_POS_NONE == CursorMode )
	{	return CBaseDialog::OnSetCursor(pWnd, nHitTest, message);	}

	JetAPI::UpdateCursor(CursorMode); 
	return TRUE;
	return CBaseDialog::OnSetCursor(pWnd, nHitTest, message);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::SetModelPtr(CAOIModel *Ptr)
{
	m_ModelPtr = Ptr;
	if ( NULL != Ptr ) 
	{	
		TREGION4D TotalRegion;
		Ptr->GetModelAttachedPosStage(m_ModelImagePosStage);					
		Ptr->GetModelTotalRegionStage(TotalRegion);
		m_ModelImagePosStage.x = TotalRegion.GetCpX();
		m_ModelImagePosStage.y = TotalRegion.GetCpY();
	}	
}
//-------------------------------------------------------------------------------------//
inline CAOIModel* CModelPropertyWnd::GetModelPtr()
{
	return m_ModelPtr;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::SetUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList)
{	
	size_t  i=0;
	const size_t MaxUnFrameCount = GetMaxFrameCount();
	JetAPI::ClearUniFrameList(m_UniFrameList, MaxUnFrameCount);
	const size_t UniFrameSize = UniFrameList.size();
	const size_t UniFrameCount = MIN(UniFrameSize, MaxUnFrameCount);
	for ( i=0; i<UniFrameCount; i++ )
	{	
		m_UniFrameList[i] = UniFrameList[i];	
		if ( NULL!=UniFrameList[i].SpacePtr && NULL!=UniFrameList[i].MaskPtr )
		{
			m_SpaceW = UniFrameList[i].ImageW;
			m_SpaceH = UniFrameList[i].ImageH;
			m_SpaceStep = UniFrameList[i].ImageStep;
			m_SpaceBitCnt = UniFrameList[i].BitCount;
			m_MaskPtr = UniFrameList[i].MaskPtr;
			m_SpacePtr = UniFrameList[i].SpacePtr;	
		}
	}

	m_MaskPtr = NULL;
	m_SpacePtr = NULL;	
	for ( i=0; i<UniFrameCount; i++ )
	{	
		if ( NULL==UniFrameList[i].SpacePtr || NULL==UniFrameList[i].MaskPtr )
		{	continue; }
		
		m_SpaceW = UniFrameList[i].ImageW;
		m_SpaceH = UniFrameList[i].ImageH;
		m_SpaceStep = UniFrameList[i].ImageStep;
		m_SpaceBitCnt = UniFrameList[i].BitCount;
		m_MaskPtr = UniFrameList[i].MaskPtr;
		m_SpacePtr = UniFrameList[i].SpacePtr;	
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CModelPropertyWnd::GetMaxFrameCount()
{
	return FRAME_MAX_COUNT;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnSelchangeLandGroupCombo() 
{
	// TODO: Add your control notification handler code here
	ExecSelchangeLandGroupCombo();
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_MODEL_PROPERTY_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_MODEL_PROPERTY_WND;
	WndKey = _T("IDD_MODEL_PROPERTY_WND");
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
	WndID = MODELPTY_EXTEND_PARAM_GROUP;
	WndKey = _T("MODELPTY_EXTEND_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_EXTEND_X_LABEL;
	WndKey = _T("MODELPTY_EXTEND_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_EXTEND_Y_LABEL;
	WndKey = _T("MODELPTY_EXTEND_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_EXTEND_AUTO_ADJUST_CHK;
	WndKey = _T("MODELPTY_EXTEND_AUTO_ADJUST_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = MODELPTY_BODY_PARAM_GROUP;
	WndKey = _T("MODELPTY_BODY_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_BODY_SIZE_X_LABEL;
	WndKey = _T("MODELPTY_BODY_SIZE_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_BODY_SIZE_Y_LABEL;
	WndKey = _T("MODELPTY_BODY_SIZE_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_BODY_SIZE_Z_LABEL;
	WndKey = _T("MODELPTY_BODY_SIZE_Z_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_BODY_SIZE_XYZ_BTN;
	WndKey = _T("MODELPTY_BODY_SIZE_XYZ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_SHOW_LAND_LINE_CHK;
	WndKey = _T("MODELPTY_SHOW_LAND_LINE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = MODELPTY_LAND_PARAM_GROUP;
	WndKey = _T("MODELPTY_LAND_PARAM_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LAND_GROUP_LABEL;
	WndKey = _T("MODELPTY_LAND_GROUP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LAND_TYPE_LABEL;
	WndKey = _T("MODELPTY_LAND_TYPE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = MODELPTY_LEAD_GROUP;
	WndKey = _T("MODELPTY_LEAD_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_SIZE_X_LABEL;
	WndKey = _T("MODELPTY_LEAD_SIZE_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_SIZE_Y_LABEL;
	WndKey = _T("MODELPTY_LEAD_SIZE_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_SIZE_Z_LABEL;
	WndKey = _T("MODELPTY_LEAD_SIZE_Z_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_SIZE_XYZ_BTN;
	WndKey = _T("MODELPTY_LEAD_SIZE_XYZ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//		
	WndID = MODELPTY_LEAD_TIP_GROUP;
	WndKey = _T("MODELPTY_LEAD_TIP_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_TIP_SIZE_X_LABEL;
	WndKey = _T("MODELPTY_LEAD_TIP_SIZE_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_TIP_SIZE_Y_LABEL;
	WndKey = _T("MODELPTY_LEAD_TIP_SIZE_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_TIP_SIZE_Z_LABEL;
	WndKey = _T("MODELPTY_LEAD_TIP_SIZE_Z_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_TIP_SIZE_XYZ_BTN;
	WndKey = _T("MODELPTY_LEAD_TIP_SIZE_XYZ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	
	WndID = MODELPTY_LEAD_SHOULDER_GROUP;
	WndKey = _T("MODELPTY_LEAD_SHOULDER_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_SHOULDER_SIZE_X_LABEL;
	WndKey = _T("MODELPTY_LEAD_SHOULDER_SIZE_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_SHOULDER_SIZE_Y_LABEL;
	WndKey = _T("MODELPTY_LEAD_SHOULDER_SIZE_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_SHOULDER_SIZE_Z_LABEL;
	WndKey = _T("MODELPTY_LEAD_SHOULDER_SIZE_Z_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LEAD_SHOULDER_SIZE_XYZ_BTN;
	WndKey = _T("MODELPTY_LEAD_SHOULDER_SIZE_XYZ_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//		
	WndID = MODELPTY_LAND_PAD_ALIGN_CHK;
	WndKey = _T("MODELPTY_LAND_PAD_ALIGN_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = MODELPTY_LAND_PART_ALIGN_CHK;
	WndKey = _T("MODELPTY_LAND_PART_ALIGN_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CModelPropertyWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_MODEL_PROPERTY_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CModelPropertyWnd::GetFrameImageW() const
{
	return m_ShowImageW;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CModelPropertyWnd::GetFrameImageH() const
{
	return m_ShowImageH;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::CreateShowBuffer()
{
	const char fnName[] = "CModelPropertyWnd::CreateShowBuffer";
	ReleaseShowImageBuffer();

	m_ImageW = m_UniFrameList[0].ImageW;
	m_ImageH = m_UniFrameList[0].ImageH;
	m_ImageStep = m_UniFrameList[0].ImageStep;
	m_BitCount = m_UniFrameList[0].BitCount;	

	IMAGE_PTR  Ptr = NULL;
	IMAGE_SIZE BitCount = 24;//直接開最大的
	IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, m_ImageH);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "Ptr") == false )
	{	return false; }
	::memset(Ptr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	m_ShowImagePtr = Ptr;	
	m_ShowBufferSize = BufferSize;

	const double Ratio = 1.05;	
	ImageAPI.CalcImageWndFitZoom(m_ImageW, m_ImageH, m_ImageWndRect, Ratio, m_ImageZoom);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ReleaseShowImageBuffer()
{
	JetMemory.free_func(m_ShowImagePtr);	 
	m_ShowBufferSize = 0;;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::UpdateFrameImage()
{
	//CAOIProject *ProjectPtr = GetActiveProject();
	//if ( NULL == ProjectPtr )	{	return false;	}		
	m_ImageIndex = 0;//ProjectPtr->GetProjectMapIndex();		
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	//m_ViewModelImage.SendMessage(MSG_EDIT_VIEW_3D_WND, WPARAM_UPDATE_3D_DATA, NULL);	
	//m_ViewModelImage.SendMessage(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::BuildShowImageBuffer()
{
	unsigned int Index = m_ImageIndex;
	const size_t UniFrameCount = FRAME_MAX_COUNT;
	if ( Index >= UniFrameCount ) { return false; }
	TUNI_FRAME UniFrame = m_UniFrameList[Index];

	if ( NULL == UniFrame.ImagePtr ) { return false; }
	if ( NULL == m_ShowImagePtr ) { return false; }

	m_ImageW = UniFrame.ImageW;
	m_ImageH = UniFrame.ImageH;
	m_ImageStep = UniFrame.ImageStep;
	m_BitCount = UniFrame.BitCount;	
	DRAW_IMAGE_MODE DrawImageMode = AOIDataCollect.GetDrawImageMode();

	if ( DRAW_IMAGE_BY_RAW != DrawImageMode )
	{
		AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_NORMAL);
		AOIDataCollect.ExecEnhanceDisplayImage(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, UniFrame.ImagePtr, m_ShowImagePtr);
	}
	else
	{	AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_BY_RAW);	}

	m_ShowImageW = m_ImageW;
	m_ShowImageH = m_ImageH;
	m_ShowImageStep = m_ImageStep;
	m_ShowBitCount = m_BitCount;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::SwitchFrameImage()
{	
	//CAOIProject *ProjectPtr = GetActiveProject();
	//if ( NULL == ProjectPtr ) { return true; }
	m_ImageIndex = 0;//ProjectPtr->GetProjectMapIndexNext(m_ImageIndex);
	//ProjectPtr->SetProjectMapIndex(m_ImageIndex);
	
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();	
	//m_ViewModelImage.PostMessage(MSG_EDIT_VIEW_3D_WND, WPARAM_UPDATE_3D_DATA, NULL);
	//m_ViewModelImage.SendMessage(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::BuildModelProperty()
{
	m_BodySizeX = m_BodySizeY = m_BodyHeight=0;
	m_LandPropertyList.clear();
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	
	size_t         i=0, j=0;
	int            LandGroupID=0;
	int            LandGroupID_Ref=0;
	int            LandGroupID_Last=-1;
	size_t         LandPropertyCount=0;
	CAOILand      *LandPtr = NULL;
	TLandProperty  LandProperty;
	TLandProperty *LandPropertyPtr=NULL;
	const size_t   LandCount = ModelPtr->GetModelLandCount();

	m_ExtendX = ModelPtr->GetModelExtendRangeX();
	m_ExtendY = ModelPtr->GetModelExtendRangeY();
	m_ExtendAutoAdjust = ModelPtr->GetModelExtendAutoAdjust();

	m_BodySizeX = ModelPtr->GetModelBodySizeX();
	m_BodySizeY = ModelPtr->GetModelBodySizeY();
	m_BodyHeight = ModelPtr->GetModelBodyHeight();

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandGroupID = LandPtr->GetLandGroupID();
		if ( LandGroupID == LandGroupID_Last )
		{	continue; }

		LandGroupID_Last = LandGroupID;
		LandPropertyCount = m_LandPropertyList.size();
		for ( j=0; j<LandPropertyCount; j++ )
		{
			LandPropertyPtr = &(m_LandPropertyList[j]);
			if ( LandGroupID == LandPropertyPtr->nLandGroupID ) 
			{	break; }
		}
		if ( j != LandPropertyCount ) { continue; }

		LandProperty = TLandProperty();
		LandProperty.eLandType = LandPtr->GetLandType();
		LandProperty.eLandToward = LandPtr->GetLandToward();
		LandProperty.nLandGroupID = LandPtr->GetLandGroupID();

		LandProperty.bPadAlign = LandPtr->GetLandIncludePadAlign();
		LandProperty.bPartAlign = LandPtr->GetLandIncludePartAlign();	

		LandProperty.dLeadSizeX = LandPtr->GetLandLeadSizeX();
		LandProperty.dLeadSizeY = LandPtr->GetLandLeadSizeY();
		LandProperty.dLeadHeight = LandPtr->GetLandLeadHeight();

		LandProperty.dLeadTipSizeX = LandPtr->GetLandLeadTipSizeX();
		LandProperty.dLeadTipSizeY = LandPtr->GetLandLeadTipSizeY();
		LandProperty.dLeadTipHeight = LandPtr->GetLandLeadTipHeight();

		LandProperty.dLeadShoulderSizeX = LandPtr->GetLandLeadShoulderSizeX();
		LandProperty.dLeadShoulderSizeY = LandPtr->GetLandLeadShoulderSizeY();
		LandProperty.dLeadShoulderHeight = LandPtr->GetLandLeadShoulderHeight();

		LandProperty.nIndex=(int)(m_LandPropertyList.size());
		m_LandPropertyList.push_back(LandProperty);
	}


	LandPropertyCount = m_LandPropertyList.size();
	for ( j=0; j<LandPropertyCount; j++ )
	{
		LandPropertyPtr = &(m_LandPropertyList[j]);
		LandPropertyPtr->nLandCount = 0;
		LandGroupID_Ref = LandPropertyPtr->nLandGroupID;
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			LandGroupID = LandPtr->GetLandGroupID();
			if ( LandGroupID != LandGroupID_Ref )
			{	continue; }
			LandPropertyPtr->nLandCount ++;
		}
	}
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::BuildLandGroupCombox()
{		
	size_t         i=0;
	int            idx=0;
	CString        str;	
	TLandProperty *LandPropertyPtr = NULL;
	const size_t   LandPropertyCount = m_LandPropertyList.size();;

	CComboBox     &Combox = m_LandGroupCombox;

	idx=0;
	JetAPI::ClearCombox(Combox);
	for ( i=0; i<LandPropertyCount; i++ )
	{
		LandPropertyPtr = &(m_LandPropertyList[i]);
		
		str.Format(_T("%d"), LandPropertyPtr->nLandGroupID+1);		
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, i);
		idx ++;
	}
	if ( idx > 0 ) 
	{	Combox.SetCurSel(0);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::UpdateModelPropertyToUI()
{
	CString str;

	str.Format(_T("%.0f"), m_ExtendX);
	CWnd::SetDlgItemText(MODELPTY_EXTEND_X_EDIT, str);
	str.Format(_T("%.0f"), m_ExtendY);
	CWnd::SetDlgItemText(MODELPTY_EXTEND_Y_EDIT, str);

	if ( true == m_ExtendAutoAdjust )
	{	CWnd::CheckDlgButton(MODELPTY_EXTEND_AUTO_ADJUST_CHK, TRUE); }
	else
	{	CWnd::CheckDlgButton(MODELPTY_EXTEND_AUTO_ADJUST_CHK, FALSE); }

	str.Format(_T("%.0f"), m_BodySizeX);
	CWnd::SetDlgItemText(MODELPTY_BODY_SIZE_X_EDIT, str);
	str.Format(_T("%.0f"), m_BodySizeY);
	CWnd::SetDlgItemText(MODELPTY_BODY_SIZE_Y_EDIT, str);
	str.Format(_T("%.0f"), m_BodyHeight);
	CWnd::SetDlgItemText(MODELPTY_BODY_SIZE_Z_EDIT, str);

	CWnd::SetDlgItemInt(MODELPTY_LAND_COUNT_EDIT, 0);
	ExecSelchangeLandGroupCombo();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ExecSelchangeLandGroupCombo()
{
	CString      str;
	CComboBox   &Combox = m_LandGroupCombox;
	const size_t index = JetAPI::GetComboxCurSelData(Combox);
	const size_t LandPropertyCount = m_LandPropertyList.size();;

	ClearLandListWnd();
	if ( index >= LandPropertyCount ) { return false; }
	m_LandProperty = m_LandPropertyList[index];
	CWnd::SetDlgItemInt(MODELPTY_LAND_COUNT_EDIT, m_LandProperty.nLandCount);
	JetAPI::SetComboxCurSel(m_LandTypeCombox, m_LandProperty.eLandType);	
	BuildLandListWnd(m_LandProperty.nLandGroupID);
	UpdateLandPropertyToUI(m_LandProperty);
	UpdateLandUIVisible(m_LandProperty.nLandCount, m_LandProperty.eLandType);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::UpdateLandPropertyToUI(const TLandProperty &LandProperty)
{
	CString str;
	CWnd::CheckDlgButton(MODELPTY_LAND_PAD_ALIGN_CHK, LandProperty.bPadAlign);
	CWnd::CheckDlgButton(MODELPTY_LAND_PART_ALIGN_CHK, LandProperty.bPartAlign);	

	//Lead
	str.Format(_T("%.0f"), LandProperty.dLeadSizeX);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SIZE_X_EDIT, str);
	str.Format(_T("%.0f"), LandProperty.dLeadSizeY);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SIZE_Y_EDIT, str);
	str.Format(_T("%.0f"), LandProperty.dLeadHeight);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SIZE_Z_EDIT, str);

	//Lead Tip
	str.Format(_T("%.0f"), LandProperty.dLeadTipSizeX);
	CWnd::SetDlgItemText(MODELPTY_LEAD_TIP_SIZE_X_EDIT, str);
	str.Format(_T("%.0f"), LandProperty.dLeadTipSizeY);
	CWnd::SetDlgItemText(MODELPTY_LEAD_TIP_SIZE_Y_EDIT, str);
	str.Format(_T("%.0f"), LandProperty.dLeadTipHeight);
	CWnd::SetDlgItemText(MODELPTY_LEAD_TIP_SIZE_Z_EDIT, str);

	//Lead Shoulder
	str.Format(_T("%.0f"), LandProperty.dLeadShoulderSizeX);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_X_EDIT, str);
	str.Format(_T("%.0f"), LandProperty.dLeadShoulderSizeY);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_Y_EDIT, str);
	str.Format(_T("%.0f"), LandProperty.dLeadShoulderHeight);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_Z_EDIT, str);

	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::UpdateLandPropertyToKernel(const TLandProperty &LandProperty)
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	
	size_t     i=0;
	int        AngleLabel=0;
	CAOILand  *LandPtr = NULL;
	BOX_TOWARD BoxToward   = LandProperty.eLandToward;
	const int LandGroupID  = LandProperty.nLandGroupID;
	const size_t LandCount = ModelPtr->GetModelLandCount();
	const size_t LandPropertyCount = m_LandPropertyList.size();

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }

		TLandProperty LocalProperty=LandProperty;
		LocalProperty.eLandToward = LandPtr->GetLandToward();
		AngleLabel = CAOIBox::CalcBoxTowardAngle(BoxToward, LocalProperty.eLandToward);
		JetAPI::RotateSize(AngleLabel, LocalProperty.dLeadSizeX, LocalProperty.dLeadSizeY);
		JetAPI::RotateSize(AngleLabel, LocalProperty.dLeadTipSizeX, LocalProperty.dLeadTipSizeY);
		JetAPI::RotateSize(AngleLabel, LocalProperty.dLeadShoulderSizeX, LocalProperty.dLeadShoulderSizeY);
		LandPtr->SetLandProperty(LocalProperty);
	}

	for ( i=0; i<LandPropertyCount; i++ )
	{
		TLandProperty &LocalProperty=m_LandPropertyList[i];
		if ( LocalProperty.nLandGroupID != LandGroupID ) { continue; }
		AngleLabel = CAOIBox::CalcBoxTowardAngle(BoxToward, LocalProperty.eLandToward);
		LocalProperty.dLeadSizeX = LandProperty.dLeadSizeX;
		LocalProperty.dLeadSizeY = LandProperty.dLeadSizeY;
		LocalProperty.dLeadHeight = LandProperty.dLeadHeight;
		LocalProperty.dLeadTipSizeX = LandProperty.dLeadTipSizeX;
		LocalProperty.dLeadTipSizeY = LandProperty.dLeadTipSizeY;
		LocalProperty.dLeadTipHeight = LandProperty.dLeadTipHeight;
		LocalProperty.dLeadShoulderSizeX = LandProperty.dLeadShoulderSizeX;
		LocalProperty.dLeadShoulderSizeY = LandProperty.dLeadShoulderSizeY;
		LocalProperty.dLeadShoulderHeight = LandProperty.dLeadShoulderHeight;
		JetAPI::RotateSize(AngleLabel, LocalProperty.dLeadSizeX, LocalProperty.dLeadSizeY);
		JetAPI::RotateSize(AngleLabel, LocalProperty.dLeadTipSizeX, LocalProperty.dLeadTipSizeY);
		JetAPI::RotateSize(AngleLabel, LocalProperty.dLeadShoulderSizeX, LocalProperty.dLeadShoulderSizeY);
	}	

	const size_t Count=m_LandPropertyList.size();
	const size_t Index=(size_t)(LandProperty.nIndex);
	if ( Index < Count )
	{	m_LandPropertyList[Index] = LandProperty;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::UpdateLandUIVisible(size_t LandCount, LAND_TYPE LandType)
{
	BOOL bShow=TRUE;
	BOOL bEnable=TRUE;
	UINT nCtrlID=0;
	//UINT nShowCmd=SW_SHOW;
	if ( 0 == LandCount ) 
	{	bEnable = FALSE;	}
	else
	{	bEnable = TRUE;	}
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_SIZE_X_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_SIZE_Y_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_SIZE_Z_EDIT, bEnable);
	JetAPI::EnableCtrlWnd(this, MODELPTY_LEAD_SIZE_XYZ_BTN, bEnable);
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_TIP_SIZE_X_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_TIP_SIZE_Y_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_TIP_SIZE_Z_EDIT, bEnable);
	JetAPI::EnableCtrlWnd(this, MODELPTY_LEAD_TIP_SIZE_XYZ_BTN, bEnable);
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_X_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_Y_EDIT, bEnable);
	JetAPI::EnableEditWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_Z_EDIT, bEnable);	
	JetAPI::EnableCtrlWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_XYZ_BTN, bEnable);
	
	//Lead UI
	if ( LAND_TYPE_PAD==LandType || LAND_TYPE_NULL==LandType ) 
	{	bShow=FALSE;	}
	else
	{	bShow=TRUE;	}
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_GROUP, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SIZE_X_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SIZE_X_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SIZE_Y_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SIZE_Y_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SIZE_Z_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SIZE_Z_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SIZE_XYZ_BTN, bShow);

	//Lead Tip UI
	if ( CAOILand::GetLandTypeUseLeadTip(LandType) == true )
	{	bShow = TRUE;	}
	else
	{	bShow = FALSE;	}
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_TIP_GROUP, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_TIP_SIZE_X_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_TIP_SIZE_X_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_TIP_SIZE_Y_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_TIP_SIZE_Y_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_TIP_SIZE_Z_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_TIP_SIZE_Z_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_TIP_SIZE_XYZ_BTN, bShow);

	//Lead Shoulder UI
	if ( CAOILand::GetLandTypeUseLeadShoulder(LandType) == true )
	{	bShow = TRUE;	}
	else
	{	bShow = FALSE;	}
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SHOULDER_GROUP, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_X_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_X_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_Y_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_Y_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_Z_LABEL, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_Z_EDIT, bShow);
	JetAPI::ShowCtrlWnd(this, MODELPTY_LEAD_SHOULDER_SIZE_XYZ_BTN, bShow);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::BuildLandListWndHeader()
{
	CThisListCtrl_13 &ListCtrl = m_LandListCtrl;
	JetAPI::ClearListCtrl(ListCtrl, TRUE);

	CString str;
	CString strWnd;
	CString strIndex;
	CString strToward;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT
	strWnd = AOIDataDefine.GetWndText();
	strIndex = AOIDataDefine.GetIndexText();
	strToward = AOIDataDefine.GetTowardText();
	{
		ListCtrl.GetClientRect(&Rect);
		width = 36;
		width2 = (Rect.right-Rect.left-width-16)/2;
		str = strIndex;		
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol ++;

		str = strWnd;
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		

		str = strToward;
		ListCtrl.InsertColumn(nCol, str, Align, width2);
		nCol ++;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ClearLandListWnd()
{
	CThisListCtrl_13 &ListCtrl = m_LandListCtrl;
	m_StopLandListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);
	m_StopLandListBeSelected = false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::BuildLandListWnd(int RefLandGroupID)
{
	ClearLandListWnd();	
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	
	size_t         i=0;
	CString        str;
	int            nItem=0;
	int            nSubItem=0;
	BOX_TOWARD     LandToward;
	int            LandGroupID=0;			
	CAOILand      *LandPtr = NULL;	
	const size_t   LandCount = ModelPtr->GetModelLandCount();
	CThisListCtrl_13 &ListCtrl = m_LandListCtrl;

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopLandListBeSelected = true;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandGroupID = LandPtr->GetLandGroupID();
		if ( LandGroupID != RefLandGroupID )
		{	continue; }

		nSubItem = 0;
		LandToward = LandPtr->GetLandToward();

		str.Format(_T("%d"), nItem+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, i);

		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str.Format(_T("%d"), i+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		str = AOIDataDefine.GetBoxTowardText(LandToward);//取得框朝向文字
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}
	m_StopLandListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::UpdateActiveObjList()
{
	CString     str;
	size_t      i=0;
	TRECT4D     Rect;	
	TPOINT2D    CornPoint[4];
	TPOINT2D    CornerPoint[4];	
	CAOIBox    *BoxPtr   = NULL;
	TActiveObj *ObjPtr = NULL;	
	const TPOINT2D Res = m_ImageResolution;
	const TPOINT2D StageCp = m_ModelImagePosStage;		
	const size_t NObjects = this->m_ActiveObjList.size();
	const IMAGE_SIZE ImageW = GetFrameImageW();
	const IMAGE_SIZE ImageH = GetFrameImageH();	
	const DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( NULL == ObjPtr ) { continue; }
		if ( NULL == ObjPtr->BoxPtr ) { continue; }
		BoxPtr = (CAOIBox*)(ObjPtr->BoxPtr);

		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{	BoxPtr->GetBoxCornerPosStageRes(CornerPoint);	}
		else
		{	BoxPtr->GetBoxCornerPosStage(CornerPoint); }		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, Res, CornerPoint, StageCp, CornPoint);//機台4端點對應到影像四點
		JetAPI::PointsToRect(CornPoint, 4, Rect);
		ObjPtr->Rect       = Rect;
		ObjPtr->CornerPts[0] = CornPoint[0];
		ObjPtr->CornerPts[1] = CornPoint[1];
		ObjPtr->CornerPts[2] = CornPoint[2];
		ObjPtr->CornerPts[3] = CornPoint[3];		
	}
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::BuildActiveObjList(CAOIModel *ModelPtr, bool ActiveOnly)
{
	bool CheckAddObj = false;
//	if ( AOIDataCollect.GetIsPressVRKey(m_MultiSelKey) == false )
//	{	CheckAddObj = false;}
//	else
//	{	CheckAddObj = true;	}
	this->m_ActiveObjList.clear();
	if ( NULL == ModelPtr ) { return; }

	CString      str;	
	size_t       i=0, j=0, k=0, s=0;
	size_t       LandCount = 0;	
	size_t       WndCount  = 0;
	size_t       BoxWndCount = 0;		
	size_t       WndRoiCount = 0;
	size_t       MaskWndCount = 0;
	CAOIBox     *BoxPtr   = NULL;
	CAOIWnd     *WndPtr   = NULL;
	CAOILand    *LandPtr  = NULL;
	CAOIWndRoi  *WndRoiPtr  = NULL;
	CAOIWndMask *WndMaskPtr  = NULL;
	TActiveObj   ActiveObj;
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	bool ModelActiveOnly = true;	
	bool AddModelRect    = true;
	bool bBoxWndSelected = false;
	bool bWndBoxWndSelected = false;
	bool bLandBoxWndSelected = false;	
	bool bWndRoiBoxSelected = false;
	bool bMaskBoxSelected = false;

	ActiveObj.ModelPtr = ModelPtr;	
	ModelActiveOnly = TRUE;
	
	ActiveObj = TActiveObj();
	ActiveObj.ComponentAngle = ComponentAngle;
	ActiveObj.IsExceptionAngle = IsExceptionAngle;	
	ActiveObj.WndPtr     = NULL;
	ActiveObj.LandPtr    = NULL;
	ActiveObj.BoxPtr     = NULL;
	ActiveObj.WndRoiPtr  = NULL;
	ActiveObj.WndMaskPtr = NULL;
	ActiveObj.SetEditabled(true);
	ActiveObj.PassObj    = false;	

	//if ( false == ActiveOnly ) 
	{		
		WndCount = ModelPtr->GetModelWndCount();			
		for ( j=0; j<WndCount; j++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(j, false);				
			if ( NULL == WndPtr ) { continue; }
			
			BoxPtr = WndPtr->GetWndBoxPtr();			
			if ( NULL == BoxPtr ) { continue; }
			//if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
			if ( BoxPtr->GetBoxVisibled() == false ) 
			{ 
				BoxPtr->SetBoxSelected(false);
				continue; 
			}			
			if ( true == ActiveOnly )
			{
				if ( BoxPtr->GetBoxSelected() == false ) { continue; }
			}
			//子框
			bWndRoiBoxSelected = false;
			WndRoiCount = WndPtr->GetWndRoiWndCount();
			for ( k=0; k<WndRoiCount; k++ )
			{
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(k, false);
				if ( NULL == WndRoiPtr ) { continue; }
				BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxSelected() == true ) 
				{	bWndRoiBoxSelected = true; }

				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = WndPtr;					
				ActiveObj.LandPtr    = NULL;
				ActiveObj.WndRoiPtr  = WndRoiPtr;
				ActiveObj.WndMaskPtr = NULL;
				ActiveObj.BoxPtr     = BoxPtr;				
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}
			//遮罩框
			bMaskBoxSelected = false;
			MaskWndCount = WndPtr->GetWndMaskWndCount();
			for ( k=0; k<MaskWndCount; k++ )
			{
				WndMaskPtr = WndPtr->GetWndMaskWndPtr(k, false);
				if ( NULL == WndMaskPtr ) { continue; }
				BoxPtr = WndMaskPtr->GetWndMaskBoxPtr();
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxSelected() == true ) 
				{	bMaskBoxSelected = true; }

				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = WndPtr;					
				ActiveObj.LandPtr    = NULL;
				ActiveObj.WndRoiPtr  = NULL;
				ActiveObj.WndMaskPtr = WndMaskPtr;
				ActiveObj.BoxPtr     = BoxPtr;				
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}			

			BoxPtr = WndPtr->GetWndBoxPtr();	
			ModelActiveOnly = false;
			ActiveObj.ModelPtr   = ModelPtr;
			ActiveObj.WndPtr     = WndPtr;					
			ActiveObj.LandPtr    = NULL;
			ActiveObj.WndRoiPtr  = NULL;
			ActiveObj.WndMaskPtr = NULL;
			ActiveObj.BoxPtr     = BoxPtr;
			ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
			ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
			ActiveObj.SetFocused(BoxPtr->GetBoxActived());
			if ( true==bWndRoiBoxSelected || true==bMaskBoxSelected )
			{	
				ActiveObj.SetSelected(false);
				ActiveObj.SetFocused(false);
			}
			this->AddActiveObject(ActiveObj, CheckAddObj);				
		}
	}	

	//if ( false == ActiveOnly ) 
	{
		LandCount = ModelPtr->GetModelLandCount();
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(j, false);			
			if ( NULL == LandPtr ) { continue; }
			for ( k=0; k<5; k++ )
			{				
				switch ( k )
				{				
				case 0:	BoxPtr = LandPtr->GetLandLeadBoxPtr();	break;
				case 1:	BoxPtr = LandPtr->GetLandPadBoxPtr();	break;
				case 2:	BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();	break;
				case 3:	BoxPtr = LandPtr->GetLandLeadTipBoxPtr();	break;
				case 4:	BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();	break;
				default:	BoxPtr = NULL;	break;
				}				
				if ( BoxPtr == NULL ) { continue; }
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
				if ( BoxPtr->GetBoxVisibled() == false ) { continue; }				
				if ( true == ActiveOnly )
				{
					if ( BoxPtr->GetBoxSelected() == false ) { continue; }
				}
				ModelActiveOnly = false;
				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = NULL;
				ActiveObj.WndRoiPtr  = NULL;
				ActiveObj.WndMaskPtr = NULL;
				ActiveObj.LandPtr    = LandPtr;
				ActiveObj.BoxPtr     = BoxPtr;
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}
		}
	}
	
	AddModelRect    = true;
	if ( true == ActiveOnly )
	{
		if ( (ModelPtr->GetModelBodyBox().GetBoxSelected()==false) || (false==ModelActiveOnly) )
		{	AddModelRect = false;	}
	}		

	if ( true == AddModelRect )
	{
		BoxPtr = ModelPtr->GetModelBodyBoxPtr();
		ActiveObj.ModelPtr   = ModelPtr;		
		ActiveObj.LandPtr    = NULL;
		ActiveObj.WndRoiPtr  = NULL;
		ActiveObj.WndMaskPtr = NULL;
		ActiveObj.BoxPtr     = BoxPtr;
		ActiveObj.WndPtr     = NULL;		
		ActiveObj.ComponentAngle = ComponentAngle;		
		ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
		ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
		ActiveObj.SetFocused(BoxPtr->GetBoxActived());
		AddActiveObject(ActiveObj, CheckAddObj);
	}	
	UpdateActiveObjList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::AddActiveObject(const TActiveObj &ActiveObj, bool Check)
{
	if ( Check == true ) 
	{
		size_t i = 0;
		TActiveObj   *ActiveObjPtr = NULL;
		size_t size = this->m_ActiveObjList.size();

		if ( size > 0 ) 
		{
			for ( i=0; i<size; i++ )
			{
				ActiveObjPtr = &(m_ActiveObjList[i]);
				if ( ActiveObjPtr->ModelPtr != ActiveObj.ModelPtr ) 
				{	break;  }
				if ( ActiveObjPtr->BoxPtr != ActiveObj.BoxPtr ) 
				{	break;  }
				if ( ActiveObjPtr->WndPtr != ActiveObj.WndPtr ) 
				{	break;  }
				if ( ActiveObjPtr->LandPtr != ActiveObj.LandPtr ) 
				{	break;  }				
				if ( ActiveObjPtr->WndRoiPtr != ActiveObj.WndRoiPtr ) 
				{	break;  }
				if ( ActiveObjPtr->WndMaskPtr != ActiveObj.WndMaskPtr ) 
				{	break;  }				
			}		
			if ( i == size ) { return; }
		}
	}	

	if ( ActiveObj.ComponentAngle < -1000 )
	{
		Check = Check;
	}
	this->m_ActiveObjList.push_back(ActiveObj);	
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CModelPropertyWnd::CheckCursorPosMode(POINT pt)//確認鼠標座標模式
{
	RECT         Rect;
	TRECT4D      dRect;
	SIZE         szGrid;
	double       Angle = 0;
	size_t       i = 0;	
	CAOIBox      *BoxPtr = NULL;
	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;	
	TActiveObj  *ObjPtr = NULL;	
	TActiveObj  *ObjPtrActived = NULL;	
	TPOINT2D     Cp;
	TPOINT2D     WndPt = pt;
	TPOINT2D     ImagePt, ImagePt2;
	TPOINT2D     CornerPoint[4];	
	POINT        ImagePoint;	
	double       ZoomScale = m_ImageZoom;		
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) 
	{	return CursorMode; }	
	if ( ModelPtr->GetModelEditMode() == false ) { return CursorMode; }	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) 
	{	return CursorMode; }
	MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
	const bool MultiSelectMode = AOIDataCollect.CheckMultiSelectMode();	
	const IMAGE_SIZE ImageW = GetFrameImageW();
	const IMAGE_SIZE ImageH = GetFrameImageH();	
	const size_t NObjects = this->m_ActiveObjList.size();	

	szGrid.cx = GetEditCheckSize();
	szGrid.cy = GetEditCheckSize();	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	Cp.x = Cp.y = 0;
	if ( MANIPULATE_MODEL_EDIT == ManiMode )
	{
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			if ( false == ObjPtr->GetEditabled() ) { continue; }
			if ( false == ObjPtr->GetSelected() ) { continue; }
			//if ( false == ObjPtr->GetFocused() ) { continue; }			

			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			if ( ObjPtr->IsExceptionAngle == false )
			{	
				JetAPI::Point2DToPoint(ImagePt, ImagePoint);
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);				
			}
			else
			{
				Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);
				ImagePt2 = ImagePt;
				CornerPoint[0] = ObjPtr->CornerPts[0];
				CornerPoint[1] = ObjPtr->CornerPts[1];
				CornerPoint[2] = ObjPtr->CornerPts[2];
				CornerPoint[3] = ObjPtr->CornerPts[3];				
				JetAPI::RotateCornerPos(-Angle, Cp.x, Cp.y, CornerPoint);
				JetAPI::RotatePos(-Angle, Cp.x, Cp.y, ImagePt2);
				JetAPI::PointsToRect(CornerPoint, 4, dRect);				
				JetAPI::Point2DToPoint(ImagePt2, ImagePoint);
				JetAPI::Rect4DToRect(dRect, Rect);				
			}

			CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);
			if( CURSOR_POS_NONE != CursorMode ) 
			{	break;	}
		}
	}
	else if ( MANIPULATE_MODEL_SELECT == ManiMode )
	{	
		ObjPtrActived = NULL;	
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			if ( false == ObjPtr->GetEditabled() ) { continue; }

			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;			
			if ( ObjPtr->IsExceptionAngle == false )
			{
				JetAPI::Point2DToPoint(ImagePt, ImagePoint);
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);				
			}
			else
			{
				Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);
				ImagePt2 = ImagePt;
				CornerPoint[0] = ObjPtr->CornerPts[0];
				CornerPoint[1] = ObjPtr->CornerPts[1];
				CornerPoint[2] = ObjPtr->CornerPts[2];
				CornerPoint[3] = ObjPtr->CornerPts[3];
				JetAPI::RotateCornerPos(-Angle, Cp.x, Cp.y, CornerPoint);
				JetAPI::RotatePos(-Angle, Cp.x, Cp.y, ImagePt2);
				JetAPI::PointsToRect(CornerPoint, 4, dRect);
				JetAPI::Point2DToPoint(ImagePt2, ImagePoint);
				JetAPI::Rect4DToRect(dRect, Rect);				
			}

			CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);
			if( CURSOR_POS_NONE != CursorMode ) 
			{	
				if ( false == MultiSelectMode )
				{
					ModelPtr->UnSelectModel();
					ModelPtr->SetModelWndActived(NULL);
					ModelPtr->SetModelLandActived(NULL);					
					ResetActiveObjPosSelect();
				}
				ObjPtrActived = ObjPtr;
				ObjPtr->SetFocused(true);
				ObjPtr->SetSelected(true);
				BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
				if ( NULL != BoxPtr )
				{	BoxPtr->SetBoxSelected(true);	}
				break; 
			}
		}
		if ( NULL!=ObjPtrActived && AOIDataCollect.CheckDoubleSideEdit() == false)
		{
			for ( i=0; i<NObjects; i++ )
			{
				ObjPtr = &(m_ActiveObjList[i]);
				if ( ObjPtr == ObjPtrActived ) { continue; }
				
				ObjPtr->SetFocused(false);
				BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
				if ( NULL != BoxPtr )
				{	BoxPtr->SetBoxSelected(false);	}				
			}
		}
	}	
	if ( CURSOR_POS_NONE != CursorMode )
	{
		if ( AOIDataCollect.CheckMoveObjectMode() == true )
		{	CursorMode = CURSOR_POS_INNER; }
	}
	return CursorMode;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::ResetActiveObjPosFocus()
{
	size_t       i = 0;	
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		ObjPtr->SetFocused(false);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::ResetActiveObjPosSelect()
{
	size_t       i = 0;	
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		ObjPtr->SetFocused(false);
		ObjPtr->SetSelected(false);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::CheckActiveObjFocus(TActiveObj &Obj)
{
	Obj = TActiveObj();
	if ( CURSOR_POS_NONE == m_MousePosMode )
	{	return; }
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( size_t i=0; i<NObjects; i++ )
	{
		const TActiveObj &ObjRef = m_ActiveObjList[i];
		if ( ObjRef.GetFocused() == false ) { continue; }
		Obj = ObjRef;
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::CheckActiveObjFocus(TActiveObj &Obj, CAOIBox &Box)
{
	CheckActiveObjFocus(m_ActiveObj);	
	if ( NULL == m_ActiveObj.BoxPtr )
	{	
		Box = CAOIBox();	
		return;
	}
			
	CAOIBox *BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, m_ActiveObj.BoxPtr);
	if ( NULL == BoxPtr )
	{	
		Box = CAOIBox();	
		return;
	}
	Box = *(BoxPtr);	
	return;	
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ExecModifyActiveObjPos()//執行選中物件的座標
{
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	return ExecModifyActiveObjPosKernel(nWndPx, nWndPy);
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ExecModifyActiveObjPosKernel(int nWndPx, int nWndPy)//執行選中物件的座標
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return false; }
	if ( ModelPtr->GetModelEditMode() == false ) { return false; }	
	if ( (0==nWndPx) && (0==nWndPy) )	{	return false; }

	size_t      i = 0;	
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *WndMaskPtr = NULL;
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();
	BoxPtr = NULL;
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( false == ObjPtr->GetFocused() ) { continue; }
		if ( false == ObjPtr->GetEditabled() ) { continue; }
		BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
		if ( NULL != BoxPtr )
		{	break;	}
	}
	if ( NULL == BoxPtr ) { return false; }		
	

	const double dImagePx = nWndPx*m_ImageZoom;
	const double dImagePy = nWndPy*m_ImageZoom;
	const double ResX = m_ImageResolution.x;
	const double ResY = m_ImageResolution.y;
	const double dCadPx = dImagePx*ResX;
	const double dCadPy = -1*dImagePy*ResY;//Y軸反向		
	const int    LinkMode = ModelPtr->GetModelWndLinkMode();

	BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	LandPtr = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);

	ModelPtr->ModifyModelBoxPos(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dCadPx, dCadPy, LinkMode);
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ExecModifyActiveObjSize(CURSOR_POS_MODE CursorMode)//執行選中物件的尺寸
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return true; }
	if ( ModelPtr->GetModelEditMode() == false ) { return true; }	

	size_t      i = 0;	
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *WndMaskPtr = NULL;
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();
	BoxPtr = NULL;
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( false == ObjPtr->GetFocused() ) { continue; }
		if ( false == ObjPtr->GetEditabled() ) { continue; }
		BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
		if ( NULL != BoxPtr )
		{	break;	}
	}
	if ( NULL == BoxPtr ) { return false; }

	TREGION4D dRgn;	
	const double dWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const double dWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	const double dImagePx = dWndPx*m_ImageZoom;
	const double dImagePy = dWndPy*m_ImageZoom;
	const double ResX = m_ImageResolution.x;
	const double ResY = m_ImageResolution.y;	
	const bool   DoubleSideEdit = AOIDataCollect.CheckDoubleSideEdit();		
	const int    LinkMode = ModelPtr->GetModelWndLinkMode();

	if ( true == ObjPtr->IsExceptionAngle )
	{
		double dRevPx=dImagePx*ResX;
		double dRevPy=dImagePy*ResY;		
		double Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);		
		JetAPI::RotatePos(-Angle, 0, 0, dRevPx, dRevPy);
		dRevPx =  dRevPx;
		dRevPy = -dRevPy;		
		JetAPI::CalcModifySizeRegion(CursorMode, DoubleSideEdit, dRevPx, dRevPy, dRgn);
	}
	else
	{
		const double dCadPx = dImagePx*ResX;
		const double dCadPy = -1*dImagePy*ResY;//Y軸反向	
		JetAPI::CalcModifySizeRegion(CursorMode, DoubleSideEdit, dCadPx, dCadPy, dRgn);	
	}
	
	BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	LandPtr = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);
	ModelPtr->ModifyModelBoxSize(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dRgn, LinkMode);
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RedrawWnd();
	// Do not call CBaseDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::RedrawWnd()
{
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC1 = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();
	if ( NULL==hDC || NULL==hMemDC1 || NULL==hMemDC2 ) { return; }

	RECT WndRect = m_ImageWndRect;	
	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC1, 0, 0, SRCCOPY );
	DrawModel(hMemDC2, WndRect);
	DrawRect(hMemDC2);
	ShowCursorInfo(hMemDC2);
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::CreateBKImage()
{	
	HDC hMemDC1 = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL==hMemDC1 ) { return false; }
	HBRUSH hBrush=NULL;
	//HBRUSH hBrush = ::CreateSolidBrush(0x000000);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC1, &m_ImageWndRect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}
	//DrawModel(hMemDC1, m_ImageWndRect);		
	COLORREF clrBK = 0x000000;	
	if ( ImageAPI.DrawImageToDC(hMemDC1, m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_ImageWndRect, m_ImageOffset, m_ImageZoom, clrBK) == false )
	{	return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::DrawRect(HDC hDC)
{
	if ( false == m_DrawRoiRect ) { return false; }

	RECT RoiRect=m_RoiRect;
	CWnd::MapWindowPoints(&m_ImageWnd, &RoiRect);

	HPEN hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN hPenOld = (HPEN)(::SelectObject(hDC, hPen));
	ImageAPI.DrawRectLine(hDC, RoiRect);
	::SelectObject(hDC, hPenOld);
	::DeleteObject(hPen);	hPen = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ShowCursorInfo(HDC hDC)
{
	POINT    Pos;
	CString  str;		
	double   Height;
	POINT    WndPos;		
	POINT    ImagePos;
	RECT     ImageRect;
	POINT    ImageOffset;	
	int      Red, Grn, Blu;	
	const int TextGap = 20;
	COLORREF clrText=0x0FF0FF;
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	
	WndPos = m_MousePosCurrent;		
	ImageOffset.x = JetAPI::Floor(m_ImageOffset.x);
	ImageOffset.y = JetAPI::Floor(m_ImageOffset.y);	
	CWnd::MapWindowPoints(&m_ImageWnd, &WndPos, 1);
	JetAPI::SizeToRect(m_ShowImageW, m_ShowImageH, ImageRect);
	ImageAPI.MapWndPtToImagePt_INT(m_ShowImageW, m_ShowImageH, m_ImageWndRect, ImageOffset, m_ImageZoom, WndPos, ImagePos);

	if ( ::PtInRect(&ImageRect, ImagePos)==FALSE || NULL==m_ShowImagePtr )
	{
		Height = 0;
		Red = Grn = Blu = 0;
	}
	else
	{		
		size_t index = 0;
		size_t IndexY = ((ImagePos.y)*m_ShowImageStep);		
		switch ( m_ShowBitCount ) 
		{
		case 24:
			index = IndexY + (ImagePos.x*3);
			Blu = m_ShowImagePtr[index];
			Grn = m_ShowImagePtr[index+1];
			Red = m_ShowImagePtr[index+2];			
			break;
		case 8:
		default:
			index = IndexY + (ImagePos.x);
			Red = Grn = Blu = m_ShowImagePtr[index];			
			break;
		}
		
		if ( NULL == m_SpacePtr )
		{	Height = 0.0;	}
		else
		{			
			IndexY = ((ImagePos.y)*m_SpaceStep);
			index = IndexY + (ImagePos.x);
			Height = m_SpacePtr[index];
		}
	}

	Pos.x = 4;
	Pos.y = 4;
	str.Format(_T("Wnd Pos(%d, %d), Image Pos(%d, %d), RGB=(%d, %d, %d), Height=%.0f um"), WndPos.x, WndPos.y, ImagePos.x, ImagePos.y, Red, Grn, Blu, Height);
	::TextOut(hDC, Pos.x, Pos.y, str, str.GetLength());
	Pos.y += TextGap;

	if ( true == m_DrawRoiRect )
	{ 
		RECT RoiRect=m_ImageRoiRect;
		double RoiHeight = m_RoiRectHeight;
		const double RoiSizeW = RoiRect.right-RoiRect.left;
		const double RoiSizeH = RoiRect.bottom-RoiRect.top;
		const double RoiSizeWum=RoiSizeW*m_ImageResolution.x;
		const double RoiSizeHum=RoiSizeH*m_ImageResolution.y;
		str.Format(_T("Roi(L:%d, T:%d, R:%d, B:%d), Size(%.0f, %.0f), Height=%.0f um"), RoiRect.left, RoiRect.top, RoiRect.right, RoiRect.bottom, RoiSizeWum, RoiSizeHum, RoiHeight);
		::TextOut(hDC, Pos.x, Pos.y, str, str.GetLength());
		Pos.y += TextGap;		
	}
	::SetTextColor(hDC, clrTextOld);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::DrawModel(HDC hDC, RECT &WndRect)
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	

	CAMERA_ID  CameraID=PRIMARY_CAMERA_ID;
	DRAW_MODEL_MODE    DrawMode=DRAW_MODEL_EDIT;
	TMODEL_DRAW_PARAM  DrawParam;	
	const BOOL bShowLandLine = CWnd::IsDlgButtonChecked(MODELPTY_SHOW_LAND_LINE_CHK);

	DrawParam.WndRect = WndRect;
	DrawParam.ShowWndBox = false;	
	if ( TRUE == bShowLandLine ) 
	{	DrawParam.ShowLandBox = true;	}
	else
	{	DrawParam.ShowLandBox = false;	}
	DrawParam.ShowEditLine = false;

	DrawParam.Scale = m_ImageZoom;
	DrawParam.ViewOffsetX =  m_ImageOffset.x;
	DrawParam.ViewOffsetY = -m_ImageOffset.y;	
	DrawParam.ResolutionX = m_ImageResolution.x;
	DrawParam.ResolutionY = m_ImageResolution.y;
	
	TPOINT2D ComponentStagePos;
	TPOINT2D StageOffset, CadOffset;

	const double StageCpx = m_ModelImagePosStage.x;
	const double StageCpy = m_ModelImagePosStage.y;	
	ModelPtr->GetModelAttachedPosStage(ComponentStagePos);
	const double StageOffsetX = (StageCpx-ComponentStagePos.x);	
	const double StageOffsetY = (StageCpy-ComponentStagePos.y);

	//Cad座標與影像座標為固定方位, 因此先將機台偏差改成Cad偏差, 再來處理
	StageOffset.x = StageOffsetX;
	StageOffset.y = StageOffsetY;
	AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);

	const double ImageOffsetX =  CadOffset.x/m_ImageResolution.x;
	const double ImageOffsetY = -CadOffset.y/m_ImageResolution.y;
	const double ViewOffsetX = ImageOffsetX/m_ImageZoom;
	const double ViewOffsetY = ImageOffsetY/m_ImageZoom;
	DrawParam.ViewCP.x = -JetAPI::Floor(ViewOffsetX);
	DrawParam.ViewCP.y = -JetAPI::Floor(ViewOffsetY);

	ModelPtr->DrawModel(hDC, DrawMode, DrawParam);
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnItemchangedLandListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopLandListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{	return;	}
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }	
	const size_t SelIndex = m_LandListCtrl.GetItemData(nItem);
	ExecSelectModelLand(SelIndex);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::CalcRoiRectHeight()
{	
	m_RoiRectHeight = 0.0;
	if ( NULL == m_SpacePtr ) { return false; }
	RECT RoiRect=m_RoiRect;
	RECT ImageRoiRect=RoiRect;
	TRECT4D ImageRoiRect4D;
	CWnd::MapWindowPoints(&m_ImageWnd, &RoiRect);
	
	int    index=0;
	int    i=0, j=0;
	int    RoiCnt=0;
	POINT  ImageOffset;
	double HeightRoi=0.0;
	ImageOffset.x = JetAPI::Floor(m_ImageOffset.x);
	ImageOffset.y = JetAPI::Floor(m_ImageOffset.y);	
	ImageAPI.MapWndRectToImageRect_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, ImageOffset, m_ImageZoom, RoiRect, ImageRoiRect4D);
	JetAPI::Rect4DToRect(ImageRoiRect4D, ImageRoiRect);
	for ( i=ImageRoiRect.top; i<ImageRoiRect.bottom; i++ )
	{
		if ( i<0 || i>=m_SpaceH ) { continue; }
		for ( j=ImageRoiRect.left; j<ImageRoiRect.right; j++ )
		{
			if ( j<0 || j>=m_SpaceW ) { continue; }
			index = (i*m_SpaceStep)+j;
			RoiCnt ++;
			HeightRoi += m_SpacePtr[index];
		}
	}
	if ( RoiCnt > 0 )
	{	HeightRoi /= RoiCnt;	}
	m_ImageRoiRect = ImageRoiRect;
	m_RoiRectHeight = HeightRoi;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::GetRoiRectParam(double &SizeX, double &SizeY, double &SizeZ)
{
	RECT     RoiRect=m_ImageRoiRect;
	double   RoiHeight = m_RoiRectHeight;
	CAOIModel *ModelPtr = GetModelPtr();
	const double RoiSizeX = RoiRect.right-RoiRect.left;
	const double RoiSizeY = RoiRect.bottom-RoiRect.top;

	SizeX = RoiSizeX*m_ImageResolution.x;
	SizeY = RoiSizeY*m_ImageResolution.y;
	SizeZ = RoiHeight;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ExecSelectModelLand(unsigned int LandIndex)
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	CAOILand  *LandPtr = ModelPtr->GetModelLandPtr(LandIndex, true);
	if ( NULL == LandPtr ) { return false; }

	const bool bToWnd = true;	
	CAOIBox &LeadBox = LandPtr->GetLandLeadBox();
	CAOIBox &LeadTipBox = LandPtr->GetLandLeadTipBox();
	CAOIBox &LeadShoulderBox = LandPtr->GetLandLeadShoulderBox();

	ModelPtr->UnSelectModelLand(bToWnd);
	ModelPtr->VisibleModelLand(bToWnd);
	LeadBox.SetBoxSelected(true);
	LeadTipBox.SetBoxSelected(true);
	LeadShoulderBox.SetBoxSelected(true);

	m_LandProperty = LandPtr->GetLandProperty();
	UpdateLandPropertyToUI(m_LandProperty);
	
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2)
{
	pt2 = pt;
	CWnd::ClientToScreen(&pt2);
	m_ImageWnd.ScreenToClient(&pt2);
	if ( ::PtInRect(&m_ImageWndRect, pt2) == FALSE )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
MANIPULATE_MODEL_MODE CModelPropertyWnd::GetManiModelMode() const
{
	return MANIPULATE_MODEL_EDIT;
}
//-------------------------------------------------------------------------------------//
int CModelPropertyWnd::GetEditLineSize()//取得編輯線的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditLineSize(m_ImageZoom, LineSizeLevel);
}
//-------------------------------------------------------------------------------------//
int CModelPropertyWnd::GetEditCheckSize()//取得編輯線比較的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditCheckSize(m_ImageZoom, LineSizeLevel);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt2={0};
	if ( PtInControlWnd(point, MODELEDIT_IMAGE_WND, pt2) == false )
	{
		CBaseDialog::OnLButtonDown(nFlags, point);
		return;
	}
	CWnd::SetCapture();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosFirst = m_MousePosLast = point;		
	m_DrawRoiRect = true;	
	m_BuidlRoiRect = true;
	m_RoiRectHeight = 0.0;
	m_RoiRect.left = m_RoiRect.right = point.x;
	m_RoiRect.top = m_RoiRect.bottom = point.y;
	CheckActiveObjFocus(m_ActiveObj, m_ActiveBox);
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CModelPropertyWnd::ExecSaveLogLButtonUp()
{
	if ( NULL == m_ActiveObj.BoxPtr ) { return true; }
	TActiveObj ActiveObj;			
	CheckActiveObjFocus(ActiveObj);	
	if ( m_ActiveObj.BoxPtr != ActiveObj.BoxPtr )
	{
		m_ActiveObj = TActiveObj();
		return true; 
	}

	double dPx=0, dPy=0;
	TREGION4D ModifyRgn;
	//DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	CAOIBox   *BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ActiveObj.BoxPtr);	
	CAOIWnd   *WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ActiveObj.WndPtr);
	CAOILand  *LandPtr = DYNAMIC_DOWNCAST(CAOILand, ActiveObj.LandPtr);
	CAOIModel *ModelPtr = DYNAMIC_DOWNCAST(CAOIModel, ActiveObj.ModelPtr);
	CAOIWndRoi *WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ActiveObj.WndRoiPtr);
	CAOIWndMask *WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ActiveObj.WndMaskPtr);
	if ( NULL == BoxPtr ) { return true; }

	switch ( m_MousePosMode )
	{
	case CURSOR_POS_INNER://Pos
		dPx = BoxPtr->GetBoxPosX()-m_ActiveBox.GetBoxPosX();
		dPy = BoxPtr->GetBoxPosY()-m_ActiveBox.GetBoxPosY();
		LogOperCtrl.SaveLogModelModifyPos(ModelPtr, BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dPx, dPy);
		break;
	case CURSOR_POS_LEFT:
	case CURSOR_POS_RIGHT:
	case CURSOR_POS_TOP:
	case CURSOR_POS_BOTTOM:
	case CURSOR_POS_LEFT_TOP:
	case CURSOR_POS_LEFT_BOTTOM:
	case CURSOR_POS_RIGHT_TOP:
	case CURSOR_POS_RIGHT_BOTTOM://Size
		ModifyRgn.minX = 0;
		ModifyRgn.minY = 0;
		ModifyRgn.maxX = BoxPtr->GetBoxSizeX()-m_ActiveBox.GetBoxSizeX();
		ModifyRgn.maxY = BoxPtr->GetBoxSizeY()-m_ActiveBox.GetBoxSizeY();
		LogOperCtrl.SaveLogModelModifySize(ModelPtr, BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, ModifyRgn);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();		
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	m_RoiRect.left   = MIN(m_MousePosFirst.x, point.x);
	m_RoiRect.top    = MIN(m_MousePosFirst.y, point.y);
	m_RoiRect.right  = MAX(m_MousePosFirst.x, point.x);
	m_RoiRect.bottom = MAX(m_MousePosFirst.y, point.y);
	if ( true == m_BuidlRoiRect ) 
	{	CalcRoiRectHeight();	}
	m_BuidlRoiRect = false;
	ExecSaveLogLButtonUp();
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT dPoint;
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);	
	dPoint.x = point.x - m_MousePosLast.x;
	dPoint.y = point.y - m_MousePosLast.y;
	if ( true == m_BuidlRoiRect ) 
	{
		m_RoiRect.left   = MIN(m_MousePosFirst.x, point.x);
		m_RoiRect.top    = MIN(m_MousePosFirst.y, point.y);
		m_RoiRect.right  = MAX(m_MousePosFirst.x, point.x);
		m_RoiRect.bottom = MAX(m_MousePosFirst.y, point.y);
		CalcRoiRectHeight();
	}
	if ( this != CWnd::GetCapture() )
	{
		RedrawWnd();
		CBaseDialog::OnMouseMove(nFlags, point);
		return;
	}

	//JetAPI::UpdateCursor();
	if ( nFlags & MK_LBUTTON )//滑鼠左鍵
	{				
		bool Modify = false;
		MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
		if ( MANIPULATE_MODEL_ADD == ManiMode )
		{	Modify = true;	}
		else 
		{
			switch ( m_MousePosMode )
			{
			case CURSOR_POS_INNER:
				Modify = ExecModifyActiveObjPos();
				break;
			case CURSOR_POS_LEFT:
			case CURSOR_POS_RIGHT:
			case CURSOR_POS_TOP:
			case CURSOR_POS_BOTTOM:
			case CURSOR_POS_LEFT_TOP:
			case CURSOR_POS_LEFT_BOTTOM:
			case CURSOR_POS_RIGHT_TOP:
			case CURSOR_POS_RIGHT_BOTTOM:
				Modify = ExecModifyActiveObjSize(m_MousePosMode);
				break;
			}
		}
		if ( MANIPULATE_MODEL_SELECT == ManiMode )
		{	Modify = true;	}
		if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode )
		{	Modify = true;	}

		Modify = true;
		if ( true == Modify )
		{	RedrawWnd();	}			
	}
	else if (nFlags & MK_RBUTTON )//滑鼠右鍵
	{
		m_ImageOffset.x += dPoint.x;
		m_ImageOffset.y += dPoint.y;
		CreateBKImage();
		RedrawWnd();
	}	
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CModelPropertyWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	RECT WndRect=m_ImageWndRect;
	m_ImageWnd.ClientToScreen(&WndRect);	
	if ( ::PtInRect(&WndRect, pt) == FALSE )
	{	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt); }

	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);	
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;		

	m_DrawRoiRect = false;	
	m_BuidlRoiRect = false;
	CreateBKImage();
	RedrawWnd();
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt2={0};
	if ( PtInControlWnd(point, MODELEDIT_IMAGE_WND, pt2) == false )
	{
		CBaseDialog::OnRButtonDown(nFlags, point);
		return;
	}	
	CWnd::SetFocus();
	CWnd::SetCapture();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosFirst = m_MousePosLast = point;
	m_DrawRoiRect = false;	
	m_BuidlRoiRect = false;
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();	
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosLast = point;

	POINT dp;
	dp.x = m_MousePosLast.x-m_MousePosFirst.x;
	dp.y = m_MousePosLast.y-m_MousePosFirst.y;
	const bool SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();		
	const int SwitchProject3DFrame = AOIDataCollect.GetSystemParameter().m_SwitchProject3DFrame;
	if ( true == SwitchFrameMode )
	{
		if ( dp.x>2 || dp.y>2 ) 
		{
		}
		else
		{
			unsigned int NextImageIndex = m_ImageIndex+1;
			const size_t MaxFrameCount = GetMaxFrameCount();
			if ( NextImageIndex >= MaxFrameCount ) { NextImageIndex = 0; }
			TUNI_FRAME UniFrame = m_UniFrameList[NextImageIndex];
			if ( FN_DISABLE == SwitchProject3DFrame )
			{
				if ( NULL!=UniFrame.SpacePtr || NULL!=UniFrame.MaskPtr )
				{	NextImageIndex = 0;	}
			}
			m_ImageIndex = NextImageIndex;
			BuildShowImageBuffer();
			CreateBKImage();
			RedrawWnd();
		}
	}
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnOK() 
{
	// TODO: Add extra validation here
	
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnBodySizeXYZBtn() 
{
	// TODO: Add your control notification handler code here
	CString  str;	
	CAOIModel *ModelPtr = GetModelPtr();
	double SizeX=0, SizeY=0, SizeZ=0;
	GetRoiRectParam(SizeX, SizeY, SizeZ);
	str.Format(_T("%.0f"), SizeX);
	CWnd::SetDlgItemText(MODELPTY_BODY_SIZE_X_EDIT, str);
	str.Format(_T("%.0f"), SizeY);
	CWnd::SetDlgItemText(MODELPTY_BODY_SIZE_Y_EDIT, str);
	str.Format(_T("%.0f"), SizeZ);
	CWnd::SetDlgItemText(MODELPTY_BODY_SIZE_Z_EDIT, str);

	m_BodySizeX = SizeX;
	m_BodySizeY = SizeY;
	m_BodyHeight = SizeZ;
	if ( NULL != ModelPtr )
	{
		ModelPtr->SetModelBodySizeX(SizeX);
		ModelPtr->SetModelBodySizeY(SizeY);
		ModelPtr->SetModelBodyHeight(SizeZ);
	}
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnLeadSizeXYZBtn() 
{
	// TODO: Add your control notification handler code here
	CString  str;	
	CAOIModel *ModelPtr = GetModelPtr();
	double SizeX=0, SizeY=0, SizeZ=0;
	GetRoiRectParam(SizeX, SizeY, SizeZ);
	str.Format(_T("%.0f"), SizeX);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SIZE_X_EDIT, str);
	str.Format(_T("%.0f"), SizeY);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SIZE_Y_EDIT, str);
	str.Format(_T("%.0f"), SizeZ);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SIZE_Z_EDIT, str);	

	m_LandProperty.dLeadSizeX = SizeX;
	m_LandProperty.dLeadSizeY = SizeY;
	m_LandProperty.dLeadHeight = SizeZ;
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnLeadTipSizeXYZBtn() 
{
	// TODO: Add your control notification handler code here
	CString  str;	
	CAOIModel *ModelPtr = GetModelPtr();
	double SizeX=0, SizeY=0, SizeZ=0;
	GetRoiRectParam(SizeX, SizeY, SizeZ);
	str.Format(_T("%.0f"), SizeX);
	CWnd::SetDlgItemText(MODELPTY_LEAD_TIP_SIZE_X_EDIT, str);
	str.Format(_T("%.0f"), SizeY);
	CWnd::SetDlgItemText(MODELPTY_LEAD_TIP_SIZE_Y_EDIT, str);
	str.Format(_T("%.0f"), SizeZ);
	CWnd::SetDlgItemText(MODELPTY_LEAD_TIP_SIZE_Z_EDIT, str);	

	m_LandProperty.dLeadTipSizeX = SizeX;
	m_LandProperty.dLeadTipSizeY = SizeY;
	m_LandProperty.dLeadTipHeight = SizeZ;
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnLeadShoulderSizeXYZBtn() 
{
	// TODO: Add your control notification handler code here
	CString  str;	
	CAOIModel *ModelPtr = GetModelPtr();
	double SizeX=0, SizeY=0, SizeZ=0;
	GetRoiRectParam(SizeX, SizeY, SizeZ);
	str.Format(_T("%.0f"), SizeX);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_X_EDIT, str);
	str.Format(_T("%.0f"), SizeY);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_Y_EDIT, str);
	str.Format(_T("%.0f"), SizeZ);
	CWnd::SetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_Z_EDIT, str);	

	m_LandProperty.dLeadShoulderSizeX = SizeX;
	m_LandProperty.dLeadShoulderSizeY = SizeY;
	m_LandProperty.dLeadShoulderHeight = SizeZ;
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusBodySizeXEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_BODY_SIZE_X_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	m_BodySizeX = val;
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelBodySizeX(val); }
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusBodySizeYEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_BODY_SIZE_Y_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	CAOIModel *ModelPtr = GetModelPtr();
	m_BodySizeY = val;
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelBodySizeY(val); }
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusBodySizeZEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_BODY_SIZE_Z_EDIT, str);
	const double val = ::_ttof(str);
	m_BodyHeight = val;
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelBodyHeight(val); }
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadSizeXEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_SIZE_X_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	m_LandProperty.dLeadSizeX = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadSizeYEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_SIZE_Y_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	m_LandProperty.dLeadSizeY = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadSizeZEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_SIZE_Z_EDIT, str);
	const double val = ::_ttof(str);	
	m_LandProperty.dLeadHeight = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadTipSizeXEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_TIP_SIZE_X_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	m_LandProperty.dLeadTipSizeX = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadTipSizeYEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_TIP_SIZE_Y_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	m_LandProperty.dLeadTipSizeY = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadTipSizeZEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_TIP_SIZE_Z_EDIT, str);
	const double val = ::_ttof(str);	
	m_LandProperty.dLeadTipHeight = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadShoulderSizeXEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_X_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	m_LandProperty.dLeadShoulderSizeX = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadShoulderSizeYEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_Y_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	m_LandProperty.dLeadShoulderSizeY = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusLeadShoulderSizeZEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_LEAD_SHOULDER_SIZE_Z_EDIT, str);
	const double val = ::_ttof(str);	
	m_LandProperty.dLeadShoulderHeight = val;	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnShowLandLineChk() 
{
	// TODO: Add your control notification handler code here
	CreateBKImage();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusExtendXEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_EXTEND_X_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	CAOIModel *ModelPtr = GetModelPtr();
	m_ExtendX = val;
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelExtendRangeX(val); }
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnKillfocusExtendYEdit() 
{
	// TODO: Add your control notification handler code here
	CString str;
	CWnd::GetDlgItemText(MODELPTY_EXTEND_Y_EDIT, str);
	const double val = ::_ttof(str);
	if ( val < 0 ) { return; }
	CAOIModel *ModelPtr = GetModelPtr();
	m_ExtendY = val;
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelExtendRangeY(val); }
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnExtendAutoAdjustChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(MODELPTY_EXTEND_AUTO_ADJUST_CHK);
	if ( TRUE == bCheck ) { m_ExtendAutoAdjust = true; }
	else { m_ExtendAutoAdjust = false; }
	CAOIModel *ModelPtr = GetModelPtr();	
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelExtendAutoAdjust(m_ExtendAutoAdjust); }
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnLandPadAlignChk()
{
	BOOL bChk=CWnd::IsDlgButtonChecked(MODELPTY_LAND_PAD_ALIGN_CHK);
	if ( TRUE == bChk ) { m_LandProperty.bPadAlign = true; }
	else { m_LandProperty.bPadAlign = false; }	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//
void CModelPropertyWnd::OnLandPartAlignChk()
{
	BOOL bChk=CWnd::IsDlgButtonChecked(MODELPTY_LAND_PART_ALIGN_CHK);
	if ( TRUE == bChk ) { m_LandProperty.bPartAlign = true; }
	else { m_LandProperty.bPartAlign = false; }	
	UpdateLandPropertyToKernel(m_LandProperty);
}
//-------------------------------------------------------------------------------------//