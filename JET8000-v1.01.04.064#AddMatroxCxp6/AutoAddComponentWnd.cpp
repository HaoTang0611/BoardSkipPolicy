// AutoAddComponentWnd.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Jet8000.h"
#include "AutoAddComponentWnd.h"
#include "afxdialogex.h"
//-------------------------------------------------------------------------------------//
#include "AlgPatternEditWnd.h"

// CAutoAddComponentWnd 對話方塊

IMPLEMENT_DYNAMIC(CAutoAddComponentWnd, CDialogEx)
//-------------------------------------------------------------------------------------//
CAutoAddComponentWnd::CAutoAddComponentWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAutoAddComponentWnd::IDD, pParent)
{
	m_BkColor = 0xE0E0E0;
	m_ImageZoom = 1.0;
	m_ImageOffset = TPOINT2D();
	//m_MapIndex = 0;
	m_ProjectMapW = 1024;
	m_ProjectMapH = 1024;
	m_ProjectPtr = NULL;
	m_ShowImagePtr = m_ShowBufferPtr = NULL;
	m_FrameType = FRAME_COLOR;
	m_AAC_WND_IMAGE = AAC_WND_IMAGE_MAP;
	m_PatternIndex = -1;
	m_PatternCount = 0;
	m_PatternSimilarity = 80.0;
	m_PatternScaleUSL = 1.1;
	m_PatternScaleLSL = 0.9;
	m_WndPtr = AOIObjManager.CreateWndObj(); // 空Wnd，用來存放圖片
	//m_ModelPtr = NULL;
	m_PointAPos = TPOINT2D(0);
	m_PointBPos = TPOINT2D(0);
	m_EditRect = RECT{0};
	SetPatternFinding(false);
	m_EditLibraryWnd = NULL;
	m_wndColorFilter = NULL;
	m_ManiMode = AOIDataCollect.GetManipulateModelMode();
	m_bLocked = false;
}
//-------------------------------------------------------------------------------------//
CAutoAddComponentWnd::~CAutoAddComponentWnd()
{
	ClearShowImageBuffer();
	AOIDataCollect.SetCallbackWnd(m_ParentWnd);
	AOIObjManager.DestroyWndObj(m_WndPtr);
	if (m_EditLibraryWnd != NULL) {
		m_EditLibraryWnd->DestroyWindow();
		m_EditLibraryWnd = NULL;
	}
	if (m_wndColorFilter != NULL) {
		m_wndColorFilter->DestroyWindow();
		m_wndColorFilter = NULL;
	}
	::DeleteObject(m_hPen); m_hPen = NULL;
	::DeleteObject(m_hPenSel); m_hPenSel = NULL;
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, AAC_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, ACC_CONPONENT_IMAGE_WND, m_ComponentImageWnd);
	DDX_Control(pDX, AAC_COMPONENT_PANEL_INDEX_COMBO, m_PanelIndexComobx);
	DDX_Control(pDX, AAC_COMPONENT_BOARD_INDEX_COMBO, m_BoardIndexComobx);
	DDX_Control(pDX, AAC_BLOB_CONNECTIVITY_COMBO, m_BlobConnectivityComobx);
	DDX_Control(pDX, AAC_FOUND_LIST, m_BlobListCtrl);
}
//-------------------------------------------------------------------------------------//
LRESULT CAutoAddComponentWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class	
	switch (message)
	{
	case MSG_CAMERA_CALLBACK:
		m_ResetView = true;
		if(true == this->GetPatternFinding()){
			if (this->ExecFind_kn(wParam, lParam, true) == false){
				this->LockUIWnd(false);
			}
		}
		else{
			if (this->UpdateFovImage(wParam, lParam, true) == false){
				this->LockUIWnd(false);
			}
		}
		break;
	case MSG_CAMERA_REGRAB_IMAGE:
		this->ExecGrabImage();
		break;
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		AOIDataCollect.ExecSystemException(wParam, lParam);
		this->LockUIWnd(false);
		break;
	case MSG_IMAGE_WND_DRAW_NEXT:
		break;
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		switch (wParam)
		{
		case WPARAM_MODIFY_STAGE_POS:
			ExecRegionMoveToPoint();
			break;
		case WPARAM_LBUTTON_UP:
			OnLButtonUpImageWnd();
			break;
		case WPARAM_LBUTTON_DBCLICK:
			OnDBlickedImageWnd();
			break;
		}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch (wParam)
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			m_ResetView = false;
			if (this->UpdateFovImage(PRIMARY_CAMERA_ID, lParam, false) == false)
			{
				this->LockUIWnd(false);
			}
			break;
		case WPARAM_UPDATE_ALG_IMAGE:
			//TurnGatherColor();
			break;
		}
		break;
	case MSG_EDIT_IMAGE_PROCESS_WND:
		switch (wParam)
		{
		case WPARAM_UPDATE_ALG_PARAM:
			OnSelChangeColorWndList();
		default:
			break;
		}
	case MSG_SELF_WND_EXTRA_MESSAGE:
		switch (wParam)
		{
		case WPARAM_FIRST_UI_CALLBACK:
			UpdateModelPtr();
			break;

		}
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
BOOL CAutoAddComponentWnd::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	InitImageWnd();				//Image Wnd 初始化
	InitWndElementDefault();	//UI Edit default
	SetMethodUIGroupUIDList();	//設置方法UID-用於移動
	MoveMethodGroupUI();		//移動
	SetMethodGroupUIVisible();	//顯示
	SwitchMultiLanguage();
	UpdateParamToUI(FALSE);     //更新
	CreateShowImageBuffer(m_ProjectMapW, m_ProjectMapH);;

	//Blob 相關初始化
	JetAPI::InitialListCtrl(m_BlobListCtrl);
	m_BlobListCtrl.ModifyStyle(0, LVS_SHOWSELALWAYS);
	m_BlobListCtrl.SetExtendedStyle(m_BlobListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	m_ParentWnd = AOIDataCollect.GetCallbackWnd(); //用於離開視窗後歸還CallbackWnd
	AOIDataCollect.SetCallbackWnd(this->GetSafeHwnd());//用於接收CEditLibraryWnd選擇模組


	SwitchImageToMap();		//顯示底圖
	BuildPanelComboxSel();	
	BuildBoardComboxSel();
	BuildConnectivityComboxSel();

	m_EditLibraryWnd = new CEditLibraryWnd();//模組選擇
	m_wndColorFilter = new CEditImageColorFilterWnd();//抽色
	m_wndColorFilter->SetColorFilterParam(m_WndPtr, &m_BinaryParam, WND_DEFECT_NONE, true);

	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clrBoardSelected = SystemParam.m_BoardSelectedColor;
	//const COLORREF  clrComponentSelected = SystemParam.m_ComponentSelectedColor;
	const COLORREF  clrComponentSelected = 0x0000FF;
	m_hPen = ::CreatePen(PS_SOLID, 2.5, clrBoardSelected);
	m_hPenSel = ::CreatePen(PS_SOLID, 3, clrComponentSelected);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CAutoAddComponentWnd::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// 檢查焦點是否在 EditBox 上
		CWnd* pWnd = GetFocus();
		TCHAR szClass[256];
		GetClassName(pWnd->GetSafeHwnd(), szClass, 256);
		if (_tcsicmp(szClass, _T("Edit")) == 0) return TRUE;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnOK()
{
	const int NewComponentCount = m_FoundRectList.size();
	if (NewComponentCount == 0) {
		CBaseDialog::OnOK(); 
		return;
	}
	
	const int PanelIndex = JetAPI::GetComboxCurSelData(m_PanelIndexComobx)+1;
	const int BoardIndex = JetAPI::GetComboxCurSelData(m_BoardIndexComobx)+1;

	CString str;
	if (AOIDataCollect.OperateLevelEditFuncAddComponent() == false) { return ; }

	str.Format(_T("Add [%d] components to Board [%d],Panel [%d], ?"), NewComponentCount, BoardIndex, PanelIndex);
	DWORD res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if (res == IDNO) {
		CBaseDialog::OnOK();
		return;
	}
	else if (res == IDCANCEL) {
		return;
	}
	if (false == ExecAddComponent()) {
		str.Format(_T("Add Failed"));
		JetAPI::ShowMessageBox(str, MB_OK);
		CBaseDialog::OnOK();
		return;
	}
	CBaseDialog::OnOK();
	return;
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	if (GetSafeHwnd() == NULL) { return; }
	if (m_ImageWnd.GetSafeHwnd() != NULL)
	{
		RECT  WndRect = { 0 };
		this->m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.right = cx;
		WndRect.bottom = cy;
		this->m_ImageWnd.MoveWindow(&WndRect);
		this->m_ImageWnd.GetClientRect(&m_ImageWndRect);
	}
	if (m_ComponentImageWnd.GetSafeHwnd() != NULL) {
		m_ConponentImageWndMemDC.CreateMemDC(&m_ComponentImageWnd, m_BkColor);
	}
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnRegionSettingBtn()
{
	ExecRegionSetting();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnRegionMoveToABtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return ; }
	ExecRegionMoveToPoint(m_PointAPos);
	if (m_AAC_WND_IMAGE == AAC_WND_IMAGE_FOV) { return; }
	SwitchImageToCamara();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnRegionMoveToBBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecRegionMoveToPoint(m_PointBPos);
	if (m_AAC_WND_IMAGE == AAC_WND_IMAGE_FOV) { return; }
	SwitchImageToCamara();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnRegionSetupABtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecRegionSettingAPoint();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnRegionSetupBBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecRegionSettingBPoint();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnPatternAddBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecAddPattern();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnPatternDeleteBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecDelPattern();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnPatternClearBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecClearPattern();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnPatternEditBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecEditPattern();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnFindBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecFind();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnBnClickedImageMatchRadio()
{
	CWnd::CheckDlgButton(AAC_BLOB_RADIO, BST_UNCHECKED);
	SetMethodGroupUIVisible();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnBnClickedBlobRadio()
{
	CWnd::CheckDlgButton(AAC_IMAGE_MATCHING_RADIO, BST_UNCHECKED);
	SetMethodGroupUIVisible();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnDeltaposIndexSpin(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return ; }

	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	int       nPatternIndex = nNextPos-1;
	const int nOldPatternIndex = m_PatternIndex;
	if ( nPatternIndex >= m_PatternCount ) { nPatternIndex = m_PatternCount-1; }
	else if ( nPatternIndex < 0 ) { nPatternIndex = 0; }
	pNMUpDown->iDelta = nPatternIndex+1-nPos;

	m_PatternIndex = nPatternIndex;
	UpdateParamToUI(PATTERN_INDEX_SPIN);
	if ( nOldPatternIndex != m_PatternIndex )
	{	
		if( SwitchPatternImage(m_PatternIndex, false) == false )
		{
			ResetPatternParam();
			return ;
		}
	}
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnSelChangePanelIndexCombo()
{
	BuildBoardComboxSel();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnOpenModelLibraryBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecOpenModelLibrary();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnOpenImageProcessBtn()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return; }
	ExecOpenImageProcessWnd();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnNMDblclkFoundList(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pItem = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pItem->iItem;   // 被點擊的 row
	//int nSubItem = pItem->iSubItem; // column（如果是 report mode）
	int nSubItem;
	if (nItem == -1) { return; }
	CString str;
	//int StagePosX, StagePosY;
	TPOINT2D StagePos;

	nSubItem = 1;
	str = m_BlobListCtrl.GetItemText(nItem, nSubItem);
	StagePos.x = ::_ttoi(str);
	nSubItem = 2;
	str = m_BlobListCtrl.GetItemText(nItem, nSubItem);
	StagePos.y = ::_ttoi(str);

	ExecRegionMoveToPoint(StagePos);
	if (m_AAC_WND_IMAGE == AAC_WND_IMAGE_FOV) { return; }
	SwitchImageToCamara();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnNMClickFoundList(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pItem = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pItem->iItem;   // 被點擊的 row
	int nSubItem;
	if (nItem == -1) { return; }
	MarkSelectTempObj();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnFoundListAddBtn()
{
	if (m_AAC_WND_IMAGE == AAC_WND_IMAGE_MAP) { return; }
	TREGION4D FovRegion;
	RECT ImageRect, StageRect;
	IMAGE_SIZE ImageW, ImageH;
	TBOX_DRAW_PARAM    ComponentRect;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TPOINT2D ImagePt1, ImagePt2, ImageRes, FovRegionCp;
	TPOINT2D StagePt1, StagePt2, StagePtCp;
	m_ImageWnd.GetImageEditRect(ImageRect);
	SIZE RectSize;
	JetAPI::GetRectSize(ImageRect, RectSize);
	if (RectSize.cx == 0 || RectSize.cy == 0) { return; }
	ImagePt1.x = ImageRect.right;
	ImagePt1.y = ImageRect.bottom;
	ImagePt2.x = ImageRect.left;
	ImagePt2.y = ImageRect.top;
	AOIDataCollect.GetFovStageRegionReal(FovRegion);
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ImageRes);

	FovRegionCp.x = FovRegion.GetCpX();
	FovRegionCp.y = FovRegion.GetCpY();
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, FovRegionCp, StagePt1);
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, FovRegionCp, StagePt2);
	StagePtCp.x = (StagePt1.x + StagePt2.x) / 2;
	StagePtCp.y = (StagePt1.y + StagePt2.y) / 2;

	StageRect.left	 = MIN(StagePt1.x, StagePt2.x);
	StageRect.top	 = MIN(StagePt1.y, StagePt2.y);
	StageRect.right  = MAX(StagePt1.x, StagePt2.x);
	StageRect.bottom = MAX(StagePt1.y, StagePt2.y);
	ComponentRect.WndRect = StageRect;
	ComponentRect.ComponentAngle = 0;
	m_FoundRectList.push_back(ComponentRect);

	//---------------------------------------------------------------------------------//
	CString str;
	CThisListCtrl_72 &ListCtrl = m_BlobListCtrl;
	int nItem = ListCtrl.GetItemCount();
	str.Format(_T("%d"), nItem + 1);
	ListCtrl.InsertItem(nItem, str);
	ListCtrl.SetItemData(nItem, nItem);
	int nSubItem = 1;

	str.Format(_T("%.0f"), StagePtCp.x);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem++;

	str.Format(_T("%.0f"), StagePtCp.y);
	ListCtrl.SetItemText(nItem, nSubItem, str);
	nSubItem++;

}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnFoundListDeleteBtn()
{
	const int SelCount = m_BlobListCtrl.GetSelectedCount();
	if (SelCount == 0) { return; }
	CString str;
	str.Format(_T("Do you want to delete the selected objects?"));
	DWORD res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if (res == IDNO) {	return;	}

	
	std::vector<int> selectedIndices;
	POSITION pos = m_BlobListCtrl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int nItem = m_BlobListCtrl.GetNextSelectedItem(pos);
		selectedIndices.push_back(nItem);
	}
	for (int i = selectedIndices.size()-1; i >= 0; i--) {
		int index = selectedIndices.at(i);
		m_FoundRectList.erase(m_FoundRectList.begin() + index);
		m_BlobListCtrl.DeleteItem(index);
	}
	SetImageWndTempObj();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnFoundListClearBtn()
{
	CString str;
	str.Format(_T("Clear All of Found ?"));
	DWORD res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if (res == IDNO) { return; }
	ExecClearTempObj();
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnAddBtn()
{
	const int NewComponentCount = m_FoundRectList.size();
	if (NewComponentCount == 0) { return; }
	const int PanelIndex = JetAPI::GetComboxCurSelData(m_PanelIndexComobx) + 1;
	const int BoardIndex = JetAPI::GetComboxCurSelData(m_BoardIndexComobx) + 1;
	CString str;
	if (AOIDataCollect.OperateLevelEditFuncAddComponent() == false) { return; }
	str.Format(_T("Add [%d] components to Board [%d],Panel [%d], ?"), NewComponentCount, BoardIndex, PanelIndex);
	DWORD res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if (res == IDNO) { return; }
	if (false == ExecAddComponent()) {
		str.Format(_T("Add Failed"));
		JetAPI::ShowMessageBox(str, MB_OK);
		return;
	}
	ExecClearTempObj(); 
	m_ProjectPtr->SelectProjectAllComponents(false);
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAutoAddComponentWnd, CBaseDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(AAC_IMAGE_MATCHING_RADIO, OnBnClickedImageMatchRadio)
	ON_BN_CLICKED(AAC_BLOB_RADIO, OnBnClickedBlobRadio)
	ON_NOTIFY(UDN_DELTAPOS, AAC_PATTERN_INDEX_SPIN, OnDeltaposIndexSpin)
	ON_BN_CLICKED(AAC_REGION_SETTING_BTN, OnRegionSettingBtn)
	ON_BN_CLICKED(AAC_A_POINT_SETUP_BTN, OnRegionSetupABtn)
	ON_BN_CLICKED(AAC_B_POINT_SETUP_BTN, OnRegionSetupBBtn)
	ON_BN_CLICKED(AAC_A_POINT_MOVE_BTN, OnRegionMoveToABtn)
	ON_BN_CLICKED(AAC_B_POINT_MOVE_BTN, OnRegionMoveToBBtn)
	ON_BN_CLICKED(AAC_PATTERN_ADD_BTN, OnPatternAddBtn)
	ON_BN_CLICKED(AAC_PATTERN_DELETE_BTN, OnPatternDeleteBtn)
	ON_BN_CLICKED(AAC_PATTERN_CLEAR_BTN, OnPatternClearBtn)
	ON_BN_CLICKED(AAC_PATTERN_EDIT_BTN, OnPatternEditBtn)
	ON_BN_CLICKED(AAC_PATTERN_FIND_BTN, OnFindBtn)
	ON_BN_CLICKED(AAC_COMPONENT_LIBRARY_OPEN_BTN, OnOpenModelLibraryBtn)
	ON_CBN_SELCHANGE(AAC_COMPONENT_PANEL_INDEX_COMBO, OnSelChangePanelIndexCombo)
	//ON_CBN_SELCHANGE(AAC_COMPONENT_BOARD_INDEX_COMBO, OnSelChangeBoardIndexCombo)
	ON_BN_CLICKED(AAC_BLOB_IMAGE_PROCESS_BTN, OnOpenImageProcessBtn)
	ON_BN_CLICKED(AAC_BLOB_FIND_BTN, OnFindBtn)
	ON_NOTIFY(NM_DBLCLK, AAC_FOUND_LIST, OnNMDblclkFoundList)
	ON_NOTIFY(NM_CLICK, AAC_FOUND_LIST, OnNMClickFoundList)
	ON_BN_CLICKED(AAC_FOUND_LIST_ADD_BTN, OnFoundListAddBtn)
	ON_BN_CLICKED(AAC_FOUND_LIST_DEL_BTN, OnFoundListDeleteBtn)
	ON_BN_CLICKED(AAC_FOUND_LIST_CLEAR_BTN, OnFoundListClearBtn)
	ON_BN_CLICKED(AAC_ADD_COMPONENT_BTN, OnAddBtn)
END_MESSAGE_MAP()
// CAutoAddComponentWnd 訊息處理常式
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::SetProjectPtr(CAOIProject * Ptr)
{

	m_ProjectPtr = Ptr;
	if (NULL == Ptr) { return; }
	const int MapIndex = 0;
	TREGION4D CadRgn;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	Ptr->GetProjectMapInfo(m_FrameResolution, CadRgn, m_FrameStageRgn);
	Ptr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	m_ProjectMapW = ImageW;
	m_ProjectMapH = ImageH;
	m_ImageZoom = 1.0;
	m_ImageOffset = TPOINT2D();
}
//-------------------------------------------------------------------------------------//
CAOIProject * CAutoAddComponentWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::InitImageWnd()
{
	ImageAPI.CalcImageWndFitZoom(m_ShowImageW, m_ShowImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	CImageWnd &ImageWnd = m_ImageWnd;
	ImageWnd.ShowFittedZoom();
	ImageWnd.SetShowLBtnPos(false);
	ImageWnd.SetShowCameraRgn(true);
	ImageWnd.SetShowBoard(true);//顯示單板
	ImageWnd.SetShowSystem(true);//顯示系統	
	ImageWnd.SetShowFiducial(true);//顯示專案定位點
	ImageWnd.SetShowBarcode(true);//顯示專案條碼	
	ImageWnd.SetShowComponent(true);//顯示專案零件
	ImageWnd.SetShowCursorLine(true);
	ImageWnd.SetShowDistrictRect(true);
	ImageWnd.SetShowComponentName(false);
	if (FN_ENABLE == AOIDataCollect.GetSystemParameter().m_ShowComponentDefectOnly)
	{	ImageWnd.SetShowComponentDefectOnly(true);	}
	else
	{	ImageWnd.SetShowComponentDefectOnly(false);	}
	ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);
	ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);
	ImageWnd.SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_STAGE_MOVE_TO);
	ImageWnd.SetShowFieldRgn(false);
	ImageWnd.SetShowMoveToMsg(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::InitWndElementDefault()
{
	CString str;
	ShowWindow(SW_SHOWMAXIMIZED);
	CWnd::CheckDlgButton(AAC_IMAGE_MATCHING_RADIO, BST_CHECKED);
	str.Format(_T("%.1f"), m_PatternSimilarity);
	CWnd::SetDlgItemText(AAC_PATTERN_SIMILARITY_EDIT, str);
	str.Format(_T("%.2f"), m_PatternScaleUSL);
	CWnd::SetDlgItemText(AAC_PATTERN_SCALE_USL_EDIT, str);
	str.Format(_T("%.2f"), m_PatternScaleLSL);
	CWnd::SetDlgItemText(AAC_PATTERN_SCALE_LSL_EDIT, str);
	str = _T("NewComponent");
	CWnd::SetDlgItemText(AAC_COMPONENT_NAME_EDIT, str);

	//// --- 寬度 (XSize) ---
	//SetDlgItemInt(AAC_BLOB_XSIZE_LSL_EDIT, 200);   // 下限
	//SetDlgItemInt(AAC_BLOB_XSIZE_USL_EDIT, 2000);  // 上限

	//// --- 高度 (YSize) ---
	//SetDlgItemInt(AAC_BLOB_YSIZE_LSL_EDIT, 200);   // 下限
	//SetDlgItemInt(AAC_BLOB_YSIZE_USL_EDIT, 2000);  // 上限

	//// --- 對角線 (LSize) ---
	//SetDlgItemInt(AAC_BLOB_LSIZE_LSL_EDIT, 300); 
	//SetDlgItemInt(AAC_BLOB_LSIZE_USL_EDIT, 3000);

	//// --- 面積 (Area) ---
	//SetDlgItemInt(AAC_BLOB_AREA_LSL_EDIT, 40000);
	//SetDlgItemInt(AAC_BLOB_AREA_USL_EDIT, 4000000);

	//// --- 縱橫比 (Aspect) ---
	//SetDlgItemInt(AAC_BLOB_ASPECT_LSL_EDIT, 0);
	//SetDlgItemInt(AAC_BLOB_ASPECT_USL_EDIT, 100);

	//// --- 填滿率 (Fill) ---
	//SetDlgItemInt(AAC_BLOB_FILL_LSL_EDIT, 0);
	//SetDlgItemInt(AAC_BLOB_FILL_USL_EDIT, 100);

	//// --- L/S ---
	//SetDlgItemInt(AAC_BLOB_LS_LSL_EDIT, 0);
	//SetDlgItemInt(AAC_BLOB_LS_USL_EDIT, 100);

	SetDlgItemInt(AAC_FOUND_LIST_COUNT_EDIT, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::SetMethodUIGroupUIDList()
{
	m_ImageMatchUI_IDList = {
		AAC_IMAGE_MATCH_GROUP,		ACC_CONPONENT_IMAGE_WND,
		AAC_PATTERN_NUMBER_LABEL,	AAC_PATTERN_NUMBER_EDIT,
		AAC_PATTERN_INDEX_LABEL,	AAC_PATTERN_INDEX_EDIT,		
		AAC_PATTERN_ADD_BTN,		AAC_PATTERN_EDIT_BTN,
		AAC_PATTERN_DELETE_BTN,		AAC_PATTERN_CLEAR_BTN,
		AAC_PATTERN_SIMILARITY_LABEL,AAC_PATTERN_SIMILARITY_EDIT,
		AAC_PATTERN_SCALE_LABEL,	AAC_PATTERN_SCALE_LSL_EDIT,
		AAC_PATTERN_SCALE_USL_EDIT,
		AAC_PATTERN_FIND_BTN,		AAC_PATTERN_INDEX_SPIN,
		AAC_PATTERN_ROTATE_CHECKBOX
	};
	m_ImageBlobUI_IDList = {
		AAC_BLOB_GROUP,//AAC_BLOB_TAB,
		AAC_BLOB_LSL_LABEL,		AAC_BLOB_USL_LABEL,
		AAC_BLOB_XSIZE_LABEL,	AAC_BLOB_XSIZE_LSL_EDIT,	AAC_BLOB_XSIZE_USL_EDIT,
		AAC_BLOB_YSIZE_LABEL,	AAC_BLOB_YSIZE_LSL_EDIT,	AAC_BLOB_YSIZE_USL_EDIT,
		AAC_BLOB_LSIZE_LABEL,	AAC_BLOB_LSIZE_LSL_EDIT,	AAC_BLOB_LSIZE_USL_EDIT,
		AAC_BLOB_AREA_LABEL,	AAC_BLOB_AREA_LSL_EDIT,		AAC_BLOB_AREA_USL_EDIT,
		AAC_BLOB_IMAGE_PROCESS_BTN,AAC_BLOB_FIND_BTN,		//AAC_BLOB_LIST,
		AAC_BLOB_LSL_LABEL2,	AAC_BLOB_USL_LABEL2,
		AAC_BLOB_ASPECT_LABEL,	AAC_BLOB_ASPECT_LSL_EDIT,	AAC_BLOB_ASPECT_USL_EDIT,
		AAC_BLOB_FILL_LABEL,	AAC_BLOB_FILL_LSL_EDIT,		AAC_BLOB_FILL_USL_EDIT,
		AAC_BLOB_LS_LABEL,	AAC_BLOB_LS_LSL_EDIT,	AAC_BLOB_LS_USL_EDIT,
		AAC_BLOB_CONNECTIVITY_LABEL,AAC_BLOB_CONNECTIVITY_COMBO
	};
	m_ImageFoundUI_IDList = {
		AAC_FOUND_GROUP,AAC_FOUND_LIST,AAC_FOUND_LIST_DEL_BTN,
		AAC_FOUND_COUNT_LABEL,AAC_FOUND_LIST_COUNT_EDIT,
		AAC_FOUND_LIST_CLEAR_BTN,
	};
	return true;
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::ClearShowImageBuffer()
{
	if ( NULL != m_ShowImagePtr )
	{	JetMemory.free_func(m_ShowImagePtr); }
	if ( NULL != m_ShowBufferPtr) 
	{	JetMemory.free_func(m_ShowBufferPtr); }
	m_ShowImagePtr = NULL;
	m_ShowBufferPtr = NULL;
	m_ShowBitCount = 24;
	m_ShowImageW = m_ShowBufferW = 1024;
	m_ShowImageH = m_ShowBufferH = 1024;
	m_ShowImageStep = m_ShowBufferStep = 1024 * 3;
	m_ShowImageSize = m_ShowBufferSize = 0;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::CreateShowImageBuffer(IMAGE_SIZE ImageW,IMAGE_SIZE ImageH)
{
	ClearShowImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	IMAGE_PTR ImagePtr = NULL;
	IMAGE_PTR BufferPtr = NULL;

	const int nAlign = 4;
	const IMAGE_SIZE BitCount = 24;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, nAlign);	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, "CAutoAddComponentWnd::CreateShowImageBuffer()", "ImagePtr") == false )
	{	return false; }
	if ( JetMemory.alloc_func(BufferSize, BufferPtr, "CAutoAddComponentWnd::CreateShowImageBuffer()", "BufferPtr") == false)
	{	return false;}
	::memset(ImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	
	m_ShowImagePtr = ImagePtr;
	m_ShowBufferPtr = BufferPtr;
	m_ShowBitCount = BitCount;

	m_ShowImageW = m_ShowBufferW = ImageW;
	m_ShowImageH = m_ShowBufferH = ImageH;
	m_ShowImageStep = m_ShowBufferStep = ImageStep;
	m_ShowImageSize = m_ShowBufferSize = BufferSize;
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ModifiedShowImageSize()
{
	const int nAlign = 4;
	const IMAGE_SIZE ImageW = m_ShowImageW;
	const IMAGE_SIZE ImageH = m_ShowImageH;
	const IMAGE_SIZE BitCount = 24;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, nAlign);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	m_ShowBufferW = ImageW;
	m_ShowBufferH = ImageH;
	m_ShowImageStep = m_ShowBufferStep = ImageStep;
	if(m_ShowBufferSize < BufferSize){
		CreateShowImageBuffer(m_ShowBufferW,m_ShowBufferH);
	}
	::memset(m_ShowImagePtr, 0xFF, sizeof(IMAGE_DATA)*BufferSize);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::RedrewWnd()
{
	m_ImageWnd.RedrawWnd(false);
	CClientDC dc(&m_ComponentImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ConponentImageWndMemDC.GetSafeHdc();
	if (NULL == hDC || NULL == hMemDC) { return; }
	RECT WndRect = m_ImageWndRect;
	m_ComponentImageWnd.GetClientRect(&WndRect);
	::BitBlt(hMemDC, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hMemDC, 0, 0, SRCCOPY);
	::IntersectClipRect(hMemDC, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

	HDC hDCUsed = hMemDC;
	::BitBlt(hDC, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hDCUsed, 0, 0, SRCCOPY);
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnDBlickedImageWnd()
{
	LockUIWnd(true);
	switch (m_AAC_WND_IMAGE)
	{
	case AAC_WND_IMAGE_MAP:		
		SwitchImageToCamara();
		break;
	case AAC_WND_IMAGE_FOV:
		SwitchImageToMap();
		break;
	default:
		break;
	}
	LockUIWnd(false);
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::OnLButtonUpImageWnd()
{
	m_ManiMode = AOIDataCollect.GetManipulateModelMode();
	switch (m_ManiMode)
	{
	case MANIPULATE_MODEL_GATHER_COLOR:
		ExecGatherColor();
		LockUIWnd(false);
		break;
	default:
		ExecSelectTempObj();
		break;
	}
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::SwitchImageToCamara()
{
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	m_ShowImageW = CameraCtrl.GetCameraImageSizeW(CameraID);
	m_ShowImageH = CameraCtrl.GetCameraImageSizeH(CameraID);

	ModifiedShowImageSize();

	TPOINT2D   ImageRes;
	TREGION4D  StageRgn;

	AOIDataCollect.GetFovStageRegionReal(StageRgn);
	AOIDataCollect.GetCameraImageInfo(CameraID, m_ShowImageW, m_ShowImageH, ImageRes);
	ExecGrabImage();

	ImageAPI.CalcImageWndFitZoom(m_ShowImageW, m_ShowImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	m_ImageWnd.SetImageInfo(CameraID, StageRgn, ImageRes, IMAGE_DATA_FOV);
	m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, false, true);
	m_ImageWnd.ShowFittedZoom();

	m_AAC_WND_IMAGE = AAC_WND_IMAGE_FOV;

	m_ImageWnd.SetRBtnUpMode(IMAGE_RBTN_UP_MOVE_STAGE);
	m_ImageWnd.SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_NULL);


	GetDlgItem(AAC_PATTERN_ADD_BTN)->EnableWindow(true);

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::SwitchImageToMap()
{
	m_AAC_WND_IMAGE = AAC_WND_IMAGE_MAP;
	CAOIProject* ProjectPtr = GetActiveProject();
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TPOINT2D   ImageRes;
	TREGION4D  RgnCad, RgnStage;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	
	bool       bForce = false;
	const bool bTestMap = m_ImageWnd.CheckShowProjectTestMpa();
	if (true == bTestMap) { bForce = true; }
	const int CurrentMapIndex = ProjectPtr->GetProjectMapIndex();
	
	ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	ProjectPtr->GetProjectMapPtr(CurrentMapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	ProjectPtr->CreateProjectMapShowPtr(CurrentMapIndex, ImagePtr, bForce, bTestMap);

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	ModifiedShowImageSize();
	::memcpy(m_ShowImagePtr, ImagePtr, sizeof(IMAGE_DATA)*m_ShowImageSize);

	ImageAPI.CalcImageWndFitZoom(m_ShowImageW, m_ShowImageH, m_ImageWndRect, 1.1, m_ImageZoom);
	m_ImageWnd.SetProjectPtr(ProjectPtr);
	m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
	m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, false, true);
	m_ImageWnd.ShowFittedZoom();
	//m_ImageWnd.SetImageEditRect(m_EditRect);
	m_ImageWnd.RedrawWnd(false);
	m_ImageWnd.SetRBtnUpMode(IMAGE_RBTN_UP_NULL);
	m_ImageWnd.SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_STAGE_MOVE_TO);

	GetDlgItem(AAC_PATTERN_ADD_BTN)->EnableWindow(false);

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)
{
	if (m_AAC_WND_IMAGE == AAC_WND_IMAGE_MAP) { return true; }
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
	{	::memset(m_ShowImagePtr, 0x00, sizeof(unsigned char)*m_ShowBufferSize);	}
	else
	{
		if (AOIDataCollect.ExecEnhanceDisplayImage(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBitCount, m_ShowBufferPtr, m_ShowImagePtr) == false)
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return false;
		}
	}
	
	m_ShowImageW = m_ShowBufferW;
	m_ShowImageH = m_ShowBufferH;
	m_ShowImageStep = m_ShowBufferStep;
	m_ShowBitCount = m_ShowBitCount;
	m_ImageWnd.RedrawWnd(false);
	this->CreateBKDC(m_ResetView);
	this->LockUIWnd(false);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::UpdateFovImageToFind(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)
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
	if ( false == bGetImage )
	{	::memset(m_ShowBufferPtr, 0x00, sizeof(unsigned char)*m_ShowBufferSize);	}
	//this->LockUIWnd(false);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecGrabImage(bool bUseMessage)
{
	if (NULL == this->m_ProjectPtr) { return FALSE; }
#ifndef OFFLINE_VERSION
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	if (true == OfflineMode)
	{
		if (bUseMessage == false) { return TRUE; }
		PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
		return TRUE;
	}
	if (AOIDataCollect.CheckCanGrabNextUniFrameImage() == false)
	{
		return TRUE;
	}
	this->LockUIWnd(true);
	AOIDataCollect.SetThreadGrabMode(THREAD_GRAB_NONE);
#ifndef LIGHT_CTRL_DISABLE
	if (AOIDataCollect.ExecGrabNextUniFrameImage() == false)
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}
#else
	m_FrameIndex = 0;
	if (AOIDataCollect.ExecGrabFrameImage(m_FrameUniqueID, m_FrameType) == false)
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}
#endif//LIGHT_CTRL_DISABLE	
#else
	if (bUseMessage == false) { return TRUE; }
	PostMessage(MSG_CAMERA_CALLBACK, PRIMARY_CAMERA_ID, NULL);
#endif//OFFLINE_VERSION
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool & bGetImage)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ShowImagePtr) { return false; }
	if ( NULL == this->m_ShowBufferPtr) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }		
	
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

	IMAGE_DISPLAY_MODE ImageDisplayMode = AOIDataCollect.MapFrameTypeToImageDisplayMode(m_FrameType);
	//if ( CameraCtrl.FillCameraImage3(CameraID, ImageDisplayMode, m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowBuffer) == false )
	if ( CameraCtrl.FillCameraImage3(CameraID, ImageDisplayMode, m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBitCount, m_ShowBufferPtr) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());
		return false;
	}
	bGetImage = true;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool & bGetImage)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ShowImagePtr) { return false; }
	if ( NULL == this->m_ShowBufferPtr) { return false; }

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
	if ( NULL!=UniFrame.ImagePtr && BuffserSize <= m_ShowBufferSize )
	{	
		m_ShowBufferW = UniFrame.ImageW;
		m_ShowBufferH = UniFrame.ImageH;
		m_ShowBufferStep = UniFrame.ImageStep;
		m_ShowBitCount = UniFrame.BitCount;		
		::memcpy(m_ShowBufferPtr, UniFrame.ImagePtr, sizeof(unsigned char)*BuffserSize);
	}	
	else
	{	::memset(m_ShowBufferPtr, 0x00, sizeof(unsigned char)*m_ShowBufferSize);	}
	JetAPI::ClearUniFrameList(UniFrameList);
	bGetImage = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool & bGetImage)
{
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) {	return false; }
	if ( NULL == this->m_ShowImagePtr) { return false; }
	if ( NULL == this->m_ShowBufferPtr) { return false; }

	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }

	double PosX=0, PosY=0, PosZ=0;
	const int  MaxFrames = FRAME_MAX_COUNT;
	TUNI_FRAME UniFrameList[FRAME_MAX_COUNT];
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	const unsigned int MapIndex = ProjectPtr->GetProjectMapIndex();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);

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
			m_FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);

			BitCount = UniFrameList[MapIndex].BitCount;
			ImageStep = UniFrameList[MapIndex].ImageStep;	
			BuffserSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
			if ( NULL!=UniFrameList[MapIndex].ImagePtr && BuffserSize <= m_ShowBufferSize )
			{
				bGetImage = true;
				m_ShowBufferW = ImageW;
				m_ShowBufferH = ImageH;
				m_ShowBufferStep = ImageStep;
				m_ShowBitCount = BitCount;
				::memcpy(m_ShowBufferPtr, UniFrameList[MapIndex].ImagePtr, sizeof(unsigned char)*BuffserSize);
			}
		}
	}	
	JetAPI::ClearUniFrameList(UniFrameList, FRAME_MAX_COUNT);
	
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CAutoAddComponentWnd::CreateBKDC(bool bResetView)
{
	if (NULL == m_ShowImagePtr) { return FALSE; }
	TPOINT2D   ImageRes;
	TPOINT2D   StagePos;
	TREGION4D  StageRgn;

	IMAGE_SIZE ImageW = 0, ImageH = 0;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	AOIDataCollect.GetFovStageRegionReal(StageRgn);
	AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ImageRes);
	this->m_ImageWnd.SetImageInfo(CameraID, StageRgn, ImageRes, IMAGE_DATA_FOV);
	this->m_ImageWnd.SetImageBuffer(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, false, bResetView);
	
	//CNewProjectPaneAlignPanel::RedrawProjectImageWnd();
	return TRUE;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::LockUIWnd(bool bLock)
{
	//AOIDataCollect.SetIsLockUIWnd(bLock);
	CWnd* pChild = GetWindow(GW_CHILD);
	m_bLocked = bLock;
	while (pChild)
	{
		TCHAR szClassName[256];
		GetClassName(pChild->GetSafeHwnd(), szClassName, 256);

		if (//_tcsicmp(szClassName, _T("Button")) == 0 ||
			_tcsicmp(szClassName, _T("Edit")) == 0 ||
			_tcsicmp(szClassName, _T("ComboBox")) == 0 )// ||
			//_tcsicmp(szClassName, _T("SysTabControl32")) == 0)
		{
			pChild->EnableWindow(!bLock);
			pChild->Invalidate();      // 標記需要重繪
			pChild->UpdateWindow();
		}
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
	this->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

	if (bLock)
	{
		BeginWaitCursor();
	}
	else
	{
		EndWaitCursor();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::GetLockUIWnd()
{
	//return AOIDataCollect.GetIsLockUIWnd();
	return m_bLocked;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecRegionSetting()
{
	//bool bLockUIWnd = GetLockUIWnd();
	//if (true == bLockUIWnd) { return true; }
	//if (m_AAC_WND_IMAGE != AAC_WND_IMAGE_MAP) { return true; }

	RECT &ImageRect = m_EditRect;
	TREGION4D &StageRegion = m_StageRegion;
	TREGION4D FovRegion;
	TPOINT2D ImagePt1, ImagePt2, ImageRes, FovRegionCp;
	IMAGE_SIZE ImageW, ImageH;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;

	bool bRegion = false;
	m_ImageWnd.GetImageEditRect(ImageRect);
	if (ImageRect.bottom != -1) { bRegion = true; }
	if (bRegion == true) {
		ImagePt1.x = ImageRect.right;
		ImagePt1.y = ImageRect.bottom;
		ImagePt2.x = ImageRect.left;
		ImagePt2.y = ImageRect.top;
	}
	
	switch (m_AAC_WND_IMAGE)
	{
	case AAC_WND_IMAGE_MAP:
		FovRegion = m_FrameStageRgn;
		ImageW = m_ProjectMapW;
		ImageH = m_ProjectMapH;
		ImageRes = m_FrameResolution;
		break;
	case AAC_WND_IMAGE_FOV:
		AOIDataCollect.GetFovStageRegionReal(FovRegion);
		AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ImageRes);
		break;
	default:
		break;
	}
	if (bRegion == true) {
		FovRegionCp.x = FovRegion.GetCpX();
		FovRegionCp.y = FovRegion.GetCpY();
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, FovRegionCp, m_PointAPos);
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, FovRegionCp, m_PointBPos);
		StageRegion.minX = MIN(m_PointAPos.x, m_PointBPos.x);
		StageRegion.minY = MIN(m_PointAPos.y, m_PointBPos.y);
		StageRegion.maxX = MAX(m_PointAPos.x, m_PointBPos.x);
		StageRegion.maxY = MAX(m_PointAPos.y, m_PointBPos.y);
	}
	else {
		AOIDataCollect.GetFovStageRegionReal(FovRegion);
		StageRegion = FovRegion;
		m_PointAPos.x = StageRegion.maxX;
		m_PointAPos.y = StageRegion.maxY;
		m_PointBPos.x = StageRegion.minX;
		m_PointBPos.y = StageRegion.minY;
	}

	SetImageWndTempObj();
	UpdateParamToUI(FALSE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecRegionSettingAPoint()
{
	RECT &ImageRect = m_EditRect;
	TREGION4D &StageRegion = m_StageRegion;

	m_PointAPos.x = CWnd::GetDlgItemInt(AAC_A_POINT_X_EDIT);
	m_PointAPos.y = CWnd::GetDlgItemInt(AAC_A_POINT_Y_EDIT);

	StageRegion.minX = MIN(m_PointAPos.x, m_PointBPos.x);
	StageRegion.minY = MIN(m_PointAPos.y, m_PointBPos.y);
	StageRegion.maxX = MAX(m_PointAPos.x, m_PointBPos.x);
	StageRegion.maxY = MAX(m_PointAPos.y, m_PointBPos.y);

	SetImageWndTempObj();

	m_ImageWnd.RedrawWnd(false);
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecRegionSettingBPoint()
{
	RECT &ImageRect = m_EditRect;
	TREGION4D &StageRegion = m_StageRegion;

	m_PointBPos.x = CWnd::GetDlgItemInt(AAC_B_POINT_X_EDIT);
	m_PointBPos.y = CWnd::GetDlgItemInt(AAC_B_POINT_Y_EDIT);

	StageRegion.minX = MIN(m_PointAPos.x, m_PointBPos.x);
	StageRegion.minY = MIN(m_PointAPos.y, m_PointBPos.y);
	StageRegion.maxX = MAX(m_PointAPos.x, m_PointBPos.x);
	StageRegion.maxY = MAX(m_PointAPos.y, m_PointBPos.y);
	
	SetImageWndTempObj();

	m_ImageWnd.RedrawWnd(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecRegionMoveToPoint(TPOINT2D point, bool bUseMessage)
{
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	m_ImageWnd.MoveViewToStagePos(point.x, point.y, false);
	if (MotionCtrlPtr->XYMoveTo(point.x, point.y, OfflineMode) == false)
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	if (bUseMessage == false) { return true; }
	AOIDataCollect.PostCallbackWndMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecRegionMoveToPoint()
{
	TPOINT2D point = m_ImageSearchPoint[m_ImageSearchIndex++];
	ExecRegionMoveToPoint(point);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecAddPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return true; }
	if (m_AAC_WND_IMAGE != AAC_WND_IMAGE_FOV) { return true; }
	RECT RoiRect;
	IMAGE_SIZE RoiImageW;
	IMAGE_SIZE RoiImageH;
	IMAGE_SIZE RoiImageStep;
	IMAGE_PTR RoiImage;
	const int nAlign = 4;
	m_ImageWnd.GetImageEditRect(RoiRect);
	RoiImageW = RoiRect.right - RoiRect.left;
	RoiImageH = RoiRect.bottom - RoiRect.top;
	RoiImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiImageW, RoiImageH, nAlign);
	const size_t BufferSize = ImageAPI.CalcBufferSize(RoiImageStep, RoiImageH);
	if ( JetMemory.alloc_func(BufferSize, RoiImage, "CAutoAddComponentWnd::ExecAddPattern()", "RoiImage") == false )
	{	return false; }

	ImageAPI.ExtractRoiImage(m_ShowBufferW, m_ShowImageH, m_ShowBufferStep, m_ShowBitCount, m_ShowBufferPtr, RoiRect, RoiImageStep, RoiImage, false);
	
	CAlgParam &AlgParam = m_WndPtr->GetWndAlgParam();
	CAlgBinaryParam BinaryParam = m_BinaryParam;
	BOX_TOWARD WndToward = BOX_TOWARD_UP;
	const int PolarityIdx = 0;
	CString ModelFolder = AOIDataCollect.GetAOITempDirectory();
	UpdateParamToUI(NULL);
	if (AlgParam.AddAlgPatternImage(RoiImageW, RoiImageH, RoiImageStep, m_ShowBitCount, RoiImage, ModelFolder, WndToward, PolarityIdx, BinaryParam) == false)
	{
		JetMemory.free_func(RoiImage);
		return false;
	}
	m_PatternCount = AlgParam.GetAlgPatternCount();
	m_PatternIndex = m_PatternCount - 1;
	UpdateParamToUI(NULL);
	//m_Dib.ReleaseBuffer();
	AOIDataCollect.ExecEnhanceDisplayImage(RoiImageW, RoiImageH, RoiImageStep, m_ShowBitCount, RoiImage, RoiImage);
	m_Dib.SetImage(RoiImage, RoiImageW, RoiImageH, RoiImageStep, m_ShowBitCount, true);

	JetMemory.free_func(RoiImage);
	CreateBKImage();
	RedrewWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecEditPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	CString  str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	/*CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }	
	CString        ModelFolder  = ModelPtr->GetModelFolder();
	CAOIFd        *FdPtr        = ModelPtr->GetModelFdPtr();
	CAOIBarcode   *BarcodePtr   = ModelPtr->GetModelBarcodePtr();
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);*/
	CString ModelFolder = AOIDataCollect.GetAOITempDirectory();
	BOX_TOWARD WndToward = BOX_TOWARD_UP;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		

	CString   filename;	
	CString   filenameDst;	
	CString   fileFolder = AOIDataCollect.GetAOITempDirectory();
	IMAGE_PTR  ImagePtr=NULL;
	CPatternParam *PatParamPtr=NULL;
	const int index = CWnd::GetDlgItemInt(AAC_PATTERN_INDEX_EDIT)-1;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;

	//if ( true == IsExceptionAngle ) 
	//{	WndToward = JetAPI::RotateToward(-AttachedAngle, WndToward);	}

	PatParamPtr = AlgParam.GetAlgPatternParamPtr(index, true);
	if ( NULL == PatParamPtr ) 
	{ return false; }
	if ( AlgParam.LoadAlgPatternImage(index, WndToward, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return false; }	

	filename.Format(_T("%s\\%s"), fileFolder, _T("PatternEditOrg.PNG"));
	filenameDst.Format(_T("%s\\%s"), fileFolder, _T("PatternEditResult.PNG"));
	ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	JetMemory.free_func(ImagePtr);

	CAlgPatternEditWnd PatternEditWnd;
	PatternEditWnd.SetImageFilename(filename, filenameDst);
	if ( PatternEditWnd.DoModal() == IDCANCEL ) 
	{	return true;	}

	IMAGE_SIZE BeforW = ImageW;
	IMAGE_SIZE BeforH = ImageH;
	if ( ImageAPI.LoadImage(filenameDst, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false; }	
	AlgParam.ReplaceAlgPatternImage(index, WndToward, BeforW, BeforH, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ModelFolder);
	//ModelPtr->SetModelNeedSaveFiles(true);
	//LogOperCtrl.SaveLogModelWndAlgPatternContentModify(WndPtr);

	JetMemory.free_func(ImagePtr);
	SwitchPatternImage(index);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecDelPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CString  str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	//CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	//if ( NULL == ModelPtr ) { return true; }
	//if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }
	//CAOIFd        *FdPtr        = ModelPtr->GetModelFdPtr();
	//CAOIBarcode   *BarcodePtr   = ModelPtr->GetModelBarcodePtr();
	//CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();

	str = _T("Do you want to delete the patern?");
	//str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return true; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.DeleteAlgPattern(m_PatternIndex);
	//ModelPtr->ApplyModelWnd(WndPtr);	
	//LogOperCtrl.SaveLogModelWndAlgPatternContentDelete(WndPtr);

	//AOIDataCollect.CloseActiveComponent(ComponentPtr);
	m_PatternCount = AlgParam.GetAlgPatternCount();	
	if ( m_PatternIndex >= m_PatternCount )
	{	m_PatternIndex = m_PatternCount-1; }
	m_IndexSpin.SetRange((short)0, (short)(m_PatternCount));
	UpdateParamToUI(NULL);	
	//UpdatePatternParamToParentWnd();
	if ( SwitchPatternImage(m_PatternIndex) == false )
	{
		ResetPatternParam();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecClearPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CString   str;
	CAOIWnd  *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	//CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	//if ( NULL == ModelPtr ) { return true; }
	//if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }	
	//CAOIFd        *FdPtr        = ModelPtr->GetModelFdPtr();
	//CAOIBarcode   *BarcodePtr   = ModelPtr->GetModelBarcodePtr();
	//CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();	

	str = _T("Do you want to clear all paterns?");
	//str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return true; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.ClearAlgPatternFiles();
	//ModelPtr->ApplyModelWnd(WndPtr);
	//LogOperCtrl.SaveLogModelWndAlgPatternContentClearAll(WndPtr);

	//AOIDataCollect.CloseActiveComponent(ComponentPtr);	
	m_PatternCount = AlgParam.GetAlgPatternCount();
	m_PatternIndex = -1;
	m_IndexSpin.SetRange((short)0, (short)(0));
	m_PatternParam = CPatternParam();	
	UpdateParamToUI(NULL);
	//UpdatePatternParamToParentWnd();

	m_Dib.ReleaseBuffer();
	CreateBKImage();
	RedrewWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecFind()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return true; }
	SetPatternFinding(true);
	LockUIWnd(true);
	if (m_StageRegion.GetArea() == 0) {
		CString str = L"Set the current FOV as the search region.?";
		DWORD res = JetAPI::ShowMessageBox(str, MB_YESNO);
		if (res == IDNO) { return true; }
		ExecRegionSetting();
	}
	size_t i, j;
	m_FoundRectList.clear();
	bool bPatternFind = IsDlgButtonChecked(AAC_IMAGE_MATCHING_RADIO);
	bool bBlobFind = IsDlgButtonChecked(AAC_BLOB_RADIO);

	TPOINT2D ImageRes;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	IMAGE_SIZE CameraSizeW = CameraCtrl.GetCameraImageSizeW(CameraID);
	IMAGE_SIZE CameraSizeH = CameraCtrl.GetCameraImageSizeH(CameraID);
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraSizeW, CameraSizeH, ImageRes);

	IMAGE_SIZE PatternSizeW = 0, PatternSizeH = 0;
	IMAGE_SIZE MoveSizeW = 0, MoveSizeH = 0;
	const IMAGE_SIZE ExtendSize = 100;

	CAOIWnd *WndPtr = m_WndPtr;
	if (NULL == WndPtr) { return false; }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();

	IMAGE_PTR  ImagePtr = NULL;
	CPatternParam *PatParamPtr = NULL;
	BOX_TOWARD WndToward = BOX_TOWARD_UP;
	IMAGE_SIZE ImageW = 0, ImageH = 0, ImageStep = 0, BitCount = 0;

	if (bPatternFind) {
		const int PatternCount = m_PatternCount;
		for (i = 0; i < PatternCount; i++) {
			if (false == AlgParam.LoadAlgPatternImage(i, WndToward, ImageW, ImageH, ImageStep, BitCount, ImagePtr))
			{	return false;	}
			PatternSizeW = MAX(ImageW, PatternSizeW);
			PatternSizeH = MAX(ImageH, PatternSizeH);
		}
		JetMemory.free_func(ImagePtr);
		PatternSizeW = PatternSizeH = MAX(PatternSizeW, PatternSizeH);
	}

	MoveSizeW = CameraSizeW - ((PatternSizeW >> 1) + ExtendSize);
	MoveSizeH = CameraSizeH - ((PatternSizeH >> 1) + ExtendSize);
	MoveSizeW *= ImageRes.x;
	MoveSizeH *= ImageRes.y;

	ClearFoundListWnd();
	BuildFoundListWndHeader();

	m_ImageSearchPoint.clear();
	m_ImageSearchIndex = 0;
	TPOINT2D SearchPos, SearchStartPos;
	TREGION4D SearchRgn;

	bool bRestrictedX, bRestrictedY;
	int HCount = 1, WCount = 1;
	bRestrictedX = (m_StageRegion.GetWidth() > CameraSizeW*ImageRes.x);
	bRestrictedY = (m_StageRegion.GetHeight() > CameraSizeH*ImageRes.y);
	if (bRestrictedX) { WCount = std::ceil(m_StageRegion.GetWidth() / MoveSizeW); }
	if (bRestrictedY) { HCount = std::ceil(m_StageRegion.GetHeight() / MoveSizeH); }
	SearchStartPos.x = m_StageRegion.GetCpX() - (WCount*MoveSizeW - CameraSizeW*ImageRes.x) / 2;
	SearchStartPos.y = m_StageRegion.GetCpY() - (HCount*MoveSizeH - CameraSizeH*ImageRes.y) / 2;

	for (i = 0; i < HCount; i++) {
		SearchPos.y = SearchStartPos.y + i*MoveSizeH;
		SearchRgn.minY = SearchPos.y - CameraSizeH*ImageRes.y / 2;
		SearchRgn.maxY = SearchRgn.minY + CameraSizeH*ImageRes.y;
		if (true == bRestrictedY) {
			if (SearchRgn.minY < m_StageRegion.minY) {
				SearchRgn.Move(0, m_StageRegion.minY - SearchRgn.minY);
			}
			else if (SearchRgn.maxY > m_StageRegion.maxY) {
				SearchRgn.Move(0, m_StageRegion.maxY - SearchRgn.maxY);
			}
			SearchPos.y = SearchRgn.GetCpY();
		}
		for (j = 0; j < WCount; j++) {
			SearchPos.x = SearchStartPos.x + j*MoveSizeW;
			SearchRgn.minX = SearchPos.x - CameraSizeW*ImageRes.x / 2;
			SearchRgn.maxX = SearchRgn.minX + CameraSizeW*ImageRes.x;
			if (true == bRestrictedX) {
				if (SearchRgn.minX < m_StageRegion.minX) {
					SearchRgn.Move(m_StageRegion.minX - SearchRgn.minX, 0);
				}
				else if (SearchRgn.maxX > m_StageRegion.maxX) {
					SearchRgn.Move(m_StageRegion.maxX - SearchRgn.maxX, 0);
				}
				SearchPos.x = SearchRgn.GetCpX();
			}
			m_ImageSearchPoint.push_back(SearchPos);
		}
	}
	
	if(m_ImageSearchPoint.empty()){
		SetPatternFinding(false);
		LockUIWnd(false);
		return true;
	}
	PostMessage(MSG_IMAGE_WND_NOTIFY_EVENT,WPARAM_MODIFY_STAGE_POS,NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecFind_kn(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)
{
	switch (m_AAC_WND_IMAGE)
	{
	case AAC_WND_IMAGE_FOV:
		if (this->UpdateFovImage(wParam, lParam, true) == false) {
			this->LockUIWnd(false);
			return true;
		}
		break;
	default:
		if (this->UpdateFovImageToFind(wParam, lParam, true) == false){
		this->LockUIWnd(false);
		return true;
		}
		break;
	}
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam ){	return true;}	
	double x,y;
	TREGION4D SearchRegion;
	TPOINT2D SearchNowPos;

	bool bOffline = AOIDataCollect.GetOfflineMode();
	MotionCtrlPtr->GetCurrentPos(x,y,bOffline);
	bool bPatternFind = IsDlgButtonChecked(AAC_IMAGE_MATCHING_RADIO);
	SearchNowPos.x = x;SearchNowPos.y = y;
	CalcExtractRegion(SearchNowPos, SearchRegion);
	if (bPatternFind) { ExecFindPattern(SearchRegion); }
	else { ExecFindBlob(SearchRegion); }
	
	if(m_ImageSearchPoint.size()>m_ImageSearchIndex){
		PostMessage(MSG_IMAGE_WND_NOTIFY_EVENT,WPARAM_MODIFY_STAGE_POS,NULL);
	}
	else{
		MergeOverlappingRects();
		SetImageWndTempObj();
		LockUIWnd(false);
		SetPatternFinding(false);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecFindPattern(TREGION4D SearchRegion)
{
	CAOIWnd *WndPtr = m_WndPtr;
	if (NULL == WndPtr) { return true; }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	const size_t PatternCount = AlgParam.GetAlgPatternCount();
	if(PatternCount == 0) { return true; }

	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  StageRgn;
	TPOINT2D ImageRes, ImagePt1, ImagePt2, StageRgnCp, StagePosA, StagePosB, StagePosCp;
	RECT FindedRect;
	IMAGE_SIZE CameraSizeW, CameraSizeH;
	AOIDataCollect.GetFovStageRegionReal(StageRgn);
	StageRgnCp.x = StageRgn.GetCpX();
	StageRgnCp.y = StageRgn.GetCpY();
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraSizeW, CameraSizeH, ImageRes);

	CString str;

#ifdef _DEBUG
	
	CString DebugFolder = AOIDataCollect.GetAOIProjectDebugDirectory();
	str.Format(_T("%s\\FindedArea_%d.PNG"), DebugFolder, m_ImageSearchIndex);
	ImageAPI.SaveImage(str, m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBitCount, m_ShowBufferPtr, true);
#endif // _DEBUG
	size_t i, j;
	int    idx = 0;
	int    NResults = 0;
	CJetMatch    Match;	//影像匹配	
	const bool   bRobustness = true;
	CPatternParam  *PatParamPtr = NULL;
	BOX_TOWARD WndToward = BOX_TOWARD_UP;
	double ResultCX = 0.0, ResultCY = 0.0;
	double RoiOffsetX = 0.0, RoiOffsetY = 0.0;
	double CadOffsetX = 0.0, CadOffsetY = 0.0, CadSkew = 0.0, CadScaleX = 100.0, CadScaleY = 100.0;
	double ResultX = 0, ResultY = 0, ResultA = 0, ResultS = 0, ResultSX = 0, ResultSY = 0;

	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	const int       nMinReduceArea = AlgParam.GetAlgPatternMinReducedArea();
	const int       nFinalReduction = AlgParam.GetAlgPatternFinalReduction();
	//const int       nFinalReduction = 1;
	const bool      UseInterpolate = AlgParam.CheckAlgPatternMatrchInterpolate();
	const bool      bAdvancedLearning = AlgParam.GetAlgPatternAdvancedLearning();
	IMAGE_SIZE      PatImgW = 0, PatImgH = 0, PatImgStep = 0, PatBitCount = 0;
	TPOINT2D        CornerPt[4];
	IMAGE_PTR       PatImgPtr = NULL;
	IMAGE_PTR		SubImgPtr = NULL;
	RECT RoiRect = { 0 };

	
	//TComponentRect ComponentRect;
	int SearchAngle = 90;
	TBOX_DRAW_PARAM ComponentRect;
	if (Match.SetMatchLibType(MatchLibType) == false) { return false; }
	CWnd::GetDlgItemText(AAC_PATTERN_SIMILARITY_EDIT, str);
	const float ScoreLSL = ::_tcstod(str, NULL)*0.01;
	CWnd::GetDlgItemText(AAC_PATTERN_SCALE_LSL_EDIT, str);
	const float ScaleLSL = ::_tcstod(str, NULL);
	CWnd::GetDlgItemText(AAC_PATTERN_SCALE_USL_EDIT, str);
	const float ScaleUSL = ::_tcstod(str, NULL);
	if (IsDlgButtonChecked(AAC_PATTERN_ROTATE_CHECKBOX) == BST_CHECKED) {
		SearchAngle = 360;
	}

	CThisListCtrl_72 &ListCtrl = m_BlobListCtrl;
	//JetAPI::ClearListCtrl(ListCtrl, FALSE);
	//BuildFoundListWndHeader();
	int nItem = m_FoundRectList.size();
	int nSubItem = 1;

	Match.SetMatchDefaultParam();
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);
	Match.SetMinScale(ScaleLSL);
	Match.SetMaxScale(ScaleUSL);
	Match.SetMaxPositions(10);
	Match.SetInterpolate(UseInterpolate);
	Match.SetMinScore(ScoreLSL);//fMinScore
	Match.SetUseAngle(true);

	for (i = 0; i < PatternCount; i++)
	{
		PatParamPtr = AlgParam.GetAlgPatternParamPtr(i, true);
		if (NULL == PatParamPtr) { continue; }
		if (PatImgPtr != NULL) {
			JetMemory.free_func(PatImgPtr);
			PatImgPtr = NULL;
		}
		if (AlgParam.LoadAlgPatternImage(i, WndToward, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr) == false)
		{	continue;	}
		for (j = 0; j < SearchAngle; j+=90) {
			IMAGE_PTR  PatImgPtr2 = NULL;
			IMAGE_SIZE PatImgW2, PatImgH2, PatImgStep2;
			if (ImageAPI.RotateImage(j, PatImgW, PatImgH, PatImgStep, PatBitCount, PatImgPtr, PatImgW2, PatImgH2, PatImgStep2, PatImgPtr2) == false)
			{	
				JetMemory.free_func(PatImgPtr2);
				continue;	
			}
			if (Match.LearnPattern(PatImgW2, PatImgH2, PatImgStep2, PatBitCount, PatImgPtr2, true) == false)
			{
				JetMemory.free_func(PatImgPtr2);
				continue;
			}
			if (Match.Match(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBitCount, m_ShowBufferPtr, true) == false)
			{
				JetMemory.free_func(PatImgPtr2);
				continue;
			}
			NResults = Match.GetNumPositions();
			if (NResults == 0) { 
				JetMemory.free_func(PatImgPtr2);
				continue; 
			}
			for (idx = 0; idx < SearchAngle; idx++) {
				ResultS = Match.GetResultScore(idx)*100.0;
				ResultX = Match.GetResultPosX(idx);
				ResultY = Match.GetResultPosY(idx);
				ResultA = Match.GetResultAngle(idx);
				ResultSX = Match.GetResultScaleX(idx);
				ResultSY = Match.GetResultScaleY(idx);
				if (ResultS < 0.0) { ResultS = 0.0; }
				CadSkew = -ResultA;
				RoiOffsetX = ResultX - (PatImgW2 / 2);
				RoiOffsetY = ResultY - (PatImgH2 / 2);
				RoiRect.left = RoiOffsetX;
				RoiRect.right = RoiOffsetX + PatImgW2;
				RoiRect.top = RoiOffsetY;
				RoiRect.bottom = RoiOffsetY + PatImgH2;
#ifdef _DEBUG
				ImageAPI.ExtractRoiImage(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBitCount, m_ShowBufferPtr, RoiRect, PatImgStep, SubImgPtr, false);
				str.Format(_T("%s\\FindedPatternPtr_%d_%d.PNG"), DebugFolder, m_ImageSearchIndex, idx);
				ImageAPI.SaveImage(str, PatImgW2, PatImgH2, PatImgStep2, PatBitCount, SubImgPtr, true);
#endif // _DEBUG
				ImagePt1.x = RoiRect.left;
				ImagePt1.y = RoiRect.top;
				ImagePt2.x = RoiRect.right;
				ImagePt2.y = RoiRect.bottom;

				AOIDataCollect.MapCameraPtToStage(CameraSizeW, CameraSizeH, ImageRes, ImagePt1, StageRgnCp, StagePosA);
				AOIDataCollect.MapCameraPtToStage(CameraSizeW, CameraSizeH, ImageRes, ImagePt2, StageRgnCp, StagePosB);

				if (false == SearchRegion.CheckPtInside(StagePosA)) { 
					JetMemory.free_func(PatImgPtr2);
					continue; 
				}
				if (false == SearchRegion.CheckPtInside(StagePosB)) { 
					JetMemory.free_func(PatImgPtr2);
					continue; 
				}

				FindedRect.left = StagePosA.x;
				FindedRect.top = StagePosA.y;
				FindedRect.right = StagePosB.x;
				FindedRect.bottom = StagePosB.y;
				StagePosCp.x = (StagePosA.x + StagePosB.x) / 2;
				StagePosCp.y = (StagePosA.y + StagePosB.y) / 2;

				ComponentRect.ComponentAngle = j;
				ComponentRect.WndRect = FindedRect;
				m_FoundRectList.push_back(ComponentRect);
				JetMemory.free_func(PatImgPtr2);

				//-----------------------------------------------------------------------------//
				str.Format(_T("%d"), nItem + 1);
				ListCtrl.InsertItem(nItem, str);
				ListCtrl.SetItemData(nItem, nItem);
				nSubItem = 1;

				str.Format(_T("%.0f"), StagePosCp.x);
				ListCtrl.SetItemText(nItem, nSubItem, str);
				nSubItem++;

				str.Format(_T("%.0f"), StagePosCp.y);
				ListCtrl.SetItemText(nItem, nSubItem, str);
				nSubItem++;

				str.Format(_T("%.2f"), ResultSX);
				ListCtrl.SetItemText(nItem, nSubItem, str);
				nSubItem++;

				str.Format(_T("%.2f"), ResultSY);
				ListCtrl.SetItemText(nItem, nSubItem, str);
				nSubItem++;

				str.Format(_T("%.2f"), ResultS);
				ListCtrl.SetItemText(nItem, nSubItem, str);
				nSubItem++;

				str.Format(_T("%d"), j);
				ListCtrl.SetItemText(nItem, nSubItem, str);
				nSubItem++;
				nItem++;
			}
		}
		JetMemory.free_func(PatImgPtr);
	}
#ifdef _DEBUG
	JetMemory.free_func(SubImgPtr);
#endif // _DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecFindBlob(TREGION4D SearchRegion)
{
	CAlgBinaryParam &BinParam = m_BinaryParam;

	RECT MaskRect = { 0,0,m_ShowBufferW ,m_ShowBufferH };
	MASK_PTR   MaskPtr = NULL;
	const int  nAlign = 4;
	const IMAGE_SIZE MaskBitCount = 8;
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowBufferW, MaskBitCount, nAlign);
	bool bOpenMP = false;
	BOX_TOWARD WndToward = BOX_TOWARD_UP;

	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, m_ShowBufferH);
	if (JetMemory.alloc_func(MaskBufferSize, MaskPtr, "CAutoAddComponentWnd::ExecFindBlob()", "MaskPtr") == false)
	{	return false;	}

	if (ImageAPI.ColorImageColorFilter3(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBufferPtr, BinParam.GetBinaryColorGroup(), MaskRect, MaskStep, MaskPtr, true, bOpenMP) == false)
	{
		JetMemory.free_func(MaskPtr);
		return false;
	}

	CJetBlob BlobDetector;
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	//BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
	const int BlobConnectivity = GetDlgItemInt(AAC_BLOB_CONNECTIVITY_COMBO);
	if (BlobConnectivity == 8) { BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_8); }
	else { BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4); }

	if (BlobDetector.GrayImageRoiBlobDetect(m_ShowBufferW, m_ShowBufferH, MaskStep, MaskPtr, MaskRect, 128, 255) == false)
	{
		JetMemory.free_func(MaskPtr);
		return false;
	}

	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  StageRgn, ExtractRgn;
	TPOINT2D ImageRes, ImagePt1, ImagePt2, StageRgnCp, StagePosA, StagePosB, StagePosCp;
	RECT FindedRect;
	IMAGE_SIZE CameraSizeW, CameraSizeH;
	AOIDataCollect.GetFovStageRegionReal(StageRgn);

	StageRgnCp.x = StageRgn.GetCpX();
	StageRgnCp.y = StageRgn.GetCpY();

	AOIDataCollect.GetCameraImageInfo(CameraID, CameraSizeW, CameraSizeH, ImageRes);
	TPOINT2D        CornerPt[4];

	size_t i = 0;
	TPOINT2D     BoxPos;
	TSIZE2D      BoxSize;
	RECT         BlobRect = { 0,0,0,0 };
	CAOIBox      ResBox, *BoxPtr = NULL;
	double       BlobW = 0, BlobH = 0, BlobD = 0, BlobArea = 0, BlobAspectRatio = 0, BlobFillRatio = 0, BlobLongShortRatio = 0;
	TBlobResult *BlobPtr = NULL;
	const size_t BlobResCount = BlobDetector.GetBlobCount();

	TPOINT2D           RgnCp;
	TPOINT2D           ImageCp;
	TREGION4D          BoxRgn;
	TREGION4D          WndRgn;
	//TComponentRect     ComponentRect;
	TBOX_DRAW_PARAM    ComponentRect;

	//const double ScaleX = 1.0 / ImageRes.x;
	//const double ScaleY = 1.0 / ImageRes.y;
	const double ScaleX = ImageRes.x;
	const double ScaleY = ImageRes.y;
	BOOL bTranslated = FALSE;
	const double BlobMinW = GetDlgItemInt(AAC_BLOB_XSIZE_LSL_EDIT, &bTranslated);
	const bool bBlobMinWEnable = bTranslated;
	const double BlobMaxW = GetDlgItemInt(AAC_BLOB_XSIZE_USL_EDIT, &bTranslated);
	const bool bBlobMaxWEnable = bTranslated;
	const double BlobMinH = GetDlgItemInt(AAC_BLOB_YSIZE_LSL_EDIT, &bTranslated);
	const bool bBlobMinHEnable = bTranslated;
	const double BlobMaxH = GetDlgItemInt(AAC_BLOB_YSIZE_USL_EDIT, &bTranslated);
	const bool bBlobMaxHEnable = bTranslated;
	const double BlobMinD = GetDlgItemInt(AAC_BLOB_LSIZE_LSL_EDIT, &bTranslated);
	const bool bBlobMinDEnable = bTranslated;
	const double BlobMaxD = GetDlgItemInt(AAC_BLOB_LSIZE_USL_EDIT, &bTranslated);
	const bool bBlobMaxDEnable = bTranslated;
	const double BlobMinA = GetDlgItemInt(AAC_BLOB_AREA_LSL_EDIT, &bTranslated);
	const bool bBlobMinAEnable = bTranslated;
	const double BlobMaxA = GetDlgItemInt(AAC_BLOB_AREA_USL_EDIT, &bTranslated);
	const bool bBlobMaxAEnable = bTranslated;
	const double BlobMinAspectR = GetDlgItemInt(AAC_BLOB_ASPECT_LSL_EDIT, &bTranslated);
	const bool bBlobMinAspectEnable = bTranslated;
	const double BlobMaxAspectR = GetDlgItemInt(AAC_BLOB_ASPECT_USL_EDIT, &bTranslated);
	const bool bBlobMaxAspectEnable = bTranslated;
	const double BlobMinFillR = GetDlgItemInt(AAC_BLOB_FILL_LSL_EDIT, &bTranslated);
	const bool bBlobMinFillREnable = bTranslated;
	const double BlobMaxFillR = GetDlgItemInt(AAC_BLOB_FILL_USL_EDIT, &bTranslated);
	const bool bBlobMaxFillREnable = bTranslated;
	const double BlobMinLongShort = GetDlgItemInt(AAC_BLOB_LS_LSL_EDIT, &bTranslated);
	const bool bBlobMinLongShortEnable = bTranslated;
	const double BlobMaxLongShort = GetDlgItemInt(AAC_BLOB_LS_USL_EDIT, &bTranslated);
	const bool bBlobMaxLongShortEnable = bTranslated;


	CThisListCtrl_72 &ListCtrl = m_BlobListCtrl;
	int nItem = m_FoundRectList.size();
	int  nSubItem = 1;
	CString str;

	for (i = 0; i < BlobResCount; i++)
	{
		BlobPtr = BlobDetector.GetBlobPtr(i, false);
		if (NULL == BlobPtr) { continue; }
		//BlobRect = BlobPtr->m_BlobRectRaw;
		BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
		BlobW = BlobRect.right - BlobRect.left;
		BlobH = BlobRect.bottom - BlobRect.top;
		BlobArea = BlobPtr->m_BlobPixels;
		BlobW *= ScaleX;
		BlobH *= ScaleY;
		BlobArea *= ScaleX*ScaleY;
		BlobD = sqrt((BlobW*BlobW) + (BlobH*BlobH));
		BlobAspectRatio = JetAPI::CalcBlobRatio(BlobW, BlobH, WndToward)*100.0;
		BlobFillRatio = BlobArea*100.0 / (BlobW*BlobH);
		BlobLongShortRatio = BlobDetector.CalcBlobLongShortRatio(BlobW, BlobH)*100.0;
		if (true == bBlobMaxWEnable && BlobW > BlobMaxW) { continue; }
		if (true == bBlobMinWEnable && BlobW < BlobMinW) { continue; }
		if (true == bBlobMaxHEnable && BlobH > BlobMaxH) { continue; }
		if (true == bBlobMinHEnable && BlobH < BlobMinH) { continue; }
		if (true == bBlobMaxDEnable && BlobD > BlobMaxD) { continue; }
		if (true == bBlobMinDEnable && BlobD < BlobMinD) { continue; }
		if (true == bBlobMaxAEnable && BlobArea > BlobMaxA) { continue; }
		if (true == bBlobMinAEnable && BlobArea < BlobMinA) { continue; }
		if (true == bBlobMaxAspectEnable && BlobAspectRatio > BlobMaxAspectR) { continue; }
		if (true == bBlobMinAspectEnable && BlobAspectRatio < BlobMinAspectR) { continue; }
		if (true == bBlobMaxFillREnable && BlobFillRatio > BlobMaxFillR) { continue; }
		if (true == bBlobMinFillREnable && BlobFillRatio < BlobMinFillR) { continue; }
		if (true == bBlobMaxLongShortEnable && BlobLongShortRatio > BlobMaxLongShort) { continue; }
		if (true == bBlobMinLongShortEnable && BlobLongShortRatio < BlobMinLongShort) { continue; }
		ImagePt1.x = BlobRect.left;
		ImagePt1.y = BlobRect.top;
		ImagePt2.x = BlobRect.right;
		ImagePt2.y = BlobRect.bottom;

		AOIDataCollect.MapCameraPtToStage(CameraSizeW, CameraSizeH, ImageRes, ImagePt1, StageRgnCp, StagePosA);
		AOIDataCollect.MapCameraPtToStage(CameraSizeW, CameraSizeH, ImageRes, ImagePt2, StageRgnCp, StagePosB);
		
		StagePosCp.x = (StagePosA.x + StagePosB.x) / 2;
		StagePosCp.y = (StagePosA.y + StagePosB.y) / 2;

		if (false == SearchRegion.CheckPtInside(StagePosA)) { continue; }
		if (false == SearchRegion.CheckPtInside(StagePosB)) { continue; }
		//-----------------------------------------------------------------------------//
		str.Format(_T("%d"), nItem + 1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, nItem);
		nSubItem = 1;

		//StagePosX
		str.Format(_T("%.0f"), StagePosCp.x);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//StagePosY
		str.Format(_T("%.0f"), StagePosCp.y);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//Width
		str.Format(_T("%.0f"), BlobW);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//Height
		str.Format(_T("%.0f"), BlobH);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
	
		//Diag
		str.Format(_T("%.0f"), BlobD);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;	

		//Area
		str.Format(_T("%.0f"), BlobArea);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		//AspectRatio
		str.Format(_T("%.0f"), BlobAspectRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//BlobFillRatio
		str.Format(_T("%.0f"), BlobFillRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//BlobLongShortRatio
		str.Format(_T("%.0f"), BlobLongShortRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//-----------------------------------------------------------------------------//
		FindedRect.left = StagePosA.x;
		FindedRect.top = StagePosA.y;
		FindedRect.right = StagePosB.x;
		FindedRect.bottom = StagePosB.y;

		ComponentRect.ComponentAngle = 0;
		ComponentRect.WndRect = FindedRect;
		//JetAPI::RectToCornerPt(FindedRect, CornerPt);
		//memcpy(ComponentRect.CornerPts, CornerPt, 4 * sizeof(TPOINT2D));

		m_FoundRectList.push_back(ComponentRect);
		nItem += 1;
	}
	ListCtrl.SetRedraw(TRUE);

	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecFindBlob_CurrentFov()
{
	ClearFoundListWnd();
	BuildFoundListWndHeader();
	m_FoundRectList.clear();
	CAlgBinaryParam &BinParam = m_BinaryParam;

	RECT MaskRect = { 0,0,m_ShowBufferW ,m_ShowBufferH };
	MASK_PTR   MaskPtr = NULL;
	const int  nAlign = 4;
	const IMAGE_SIZE MaskBitCount = 8;
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowBufferW, MaskBitCount, nAlign);
	bool bOpenMP = false;
	BOX_TOWARD WndToward = BOX_TOWARD_UP;

	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, m_ShowBufferH);
	if (JetMemory.alloc_func(MaskBufferSize, MaskPtr, "CAutoAddComponentWnd::ExecFindBlob()", "MaskPtr") == false)
	{
		return false;
	}

	if (ImageAPI.ColorImageColorFilter3(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBufferPtr, BinParam.GetBinaryColorGroup(), MaskRect, MaskStep, MaskPtr, true, bOpenMP) == false)
	{
		JetMemory.free_func(MaskPtr);
		return false;
	}

	CJetBlob BlobDetector;
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	//BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
	const int BlobConnectivity = GetDlgItemInt(AAC_BLOB_CONNECTIVITY_COMBO);
	if (BlobConnectivity == 8) {BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_8);}
	else {	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);	}

	if (BlobDetector.GrayImageRoiBlobDetect(m_ShowBufferW, m_ShowBufferH, MaskStep, MaskPtr, MaskRect, 128, 255) == false)
	{
		JetMemory.free_func(MaskPtr);
		return false;
	}

	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  StageRgn, ExtractRgn;
	TPOINT2D ImageRes, ImagePt1, ImagePt2, StageRgnCp, StagePosA, StagePosB, StagePosCp;
	RECT FindedRect;
	IMAGE_SIZE CameraSizeW, CameraSizeH;
	AOIDataCollect.GetFovStageRegionReal(StageRgn);

	StageRgnCp.x = StageRgn.GetCpX();
	StageRgnCp.y = StageRgn.GetCpY();

	AOIDataCollect.GetCameraImageInfo(CameraID, CameraSizeW, CameraSizeH, ImageRes);
	TPOINT2D        CornerPt[4];

	size_t i = 0;
	TPOINT2D     BoxPos;
	TSIZE2D      BoxSize;
	RECT         BlobRect = { 0,0,0,0 };
	CAOIBox      ResBox, *BoxPtr = NULL;
	double       BlobW = 0, BlobH = 0, BlobD = 0, BlobArea = 0, BlobAspectRatio = 0, BlobFillRatio = 0, BlobLongShortRatio = 0;
	TBlobResult *BlobPtr = NULL;
	const size_t BlobResCount = BlobDetector.GetBlobCount();

	TPOINT2D           RgnCp;
	TPOINT2D           ImageCp;
	TREGION4D          BoxRgn;
	TREGION4D          WndRgn;
	//TComponentRect     ComponentRect;
	TBOX_DRAW_PARAM    ComponentRect;

	//const double ScaleX = 1.0 / ImageRes.x;
	//const double ScaleY = 1.0 / ImageRes.y;
	const double ScaleX = ImageRes.x;
	const double ScaleY = ImageRes.y;

	CThisListCtrl_72 &ListCtrl = m_BlobListCtrl;
	int nItem = m_FoundRectList.size();
	int  nSubItem = 1;
	CString str;

	for (i = 0; i < BlobResCount; i++)
	{
		BlobPtr = BlobDetector.GetBlobPtr(i, false);
		if (NULL == BlobPtr) { continue; }
		//BlobRect = BlobPtr->m_BlobRectRaw;
		BlobRect = BlobPtr->m_BlobRect;//20230628-Blob
		BlobW = BlobRect.right - BlobRect.left;
		BlobH = BlobRect.bottom - BlobRect.top;
		BlobArea = BlobPtr->m_BlobPixels;
		BlobW *= ScaleX;
		BlobH *= ScaleY;
		BlobArea *= ScaleX*ScaleY;
		BlobD = sqrt((BlobW*BlobW) + (BlobH*BlobH));
		BlobAspectRatio = JetAPI::CalcBlobRatio(BlobW, BlobH, WndToward)*100.0;
		BlobFillRatio = BlobArea*100.0 / (BlobW*BlobH);
		BlobLongShortRatio = BlobDetector.CalcBlobLongShortRatio(BlobW, BlobH)*100.0;
		ImagePt1.x = BlobRect.left;
		ImagePt1.y = BlobRect.top;
		ImagePt2.x = BlobRect.right;
		ImagePt2.y = BlobRect.bottom;

		AOIDataCollect.MapCameraPtToStage(CameraSizeW, CameraSizeH, ImageRes, ImagePt1, StageRgnCp, StagePosA);
		AOIDataCollect.MapCameraPtToStage(CameraSizeW, CameraSizeH, ImageRes, ImagePt2, StageRgnCp, StagePosB);

		StagePosCp.x = (StagePosA.x + StagePosB.x) / 2;
		StagePosCp.y = (StagePosA.y + StagePosB.y) / 2;
		//-----------------------------------------------------------------------------//

		str.Format(_T("%d"), nItem + 1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, nItem);
		nSubItem = 1;

		//StagePosX
		str.Format(_T("%.0f"), StagePosCp.x);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//StagePosY
		str.Format(_T("%.0f"), StagePosCp.y);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//Width
		str.Format(_T("%.0f"), BlobW);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//Height
		str.Format(_T("%.0f"), BlobH);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//Diag
		str.Format(_T("%.0f"), BlobD);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//Area
		str.Format(_T("%.0f"), BlobArea);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//AspectRatio
		str.Format(_T("%.0f"), BlobAspectRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//BlobFillRatio
		str.Format(_T("%.0f"), BlobFillRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//BlobLongShortRatio
		str.Format(_T("%.0f"), BlobLongShortRatio);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem++;

		//-----------------------------------------------------------------------------//
		FindedRect.left = StagePosA.x;
		FindedRect.top = StagePosA.y;
		FindedRect.right = StagePosB.x;
		FindedRect.bottom = StagePosB.y;

		ComponentRect.ComponentAngle = 0;
		ComponentRect.WndRect = FindedRect;
		//JetAPI::RectToCornerPt(FindedRect, CornerPt);
		//memcpy(ComponentRect.CornerPts, CornerPt, 4 * sizeof(TPOINT2D));

		m_FoundRectList.push_back(ComponentRect);
		nItem += 1;
	}
	ListCtrl.SetRedraw(TRUE);

	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecExtractImage(TPOINT2D SearchStartPt, TREGION4D &ExtractRegion)
{
	TPOINT2D ImageRes, SearchStartEdgePt, SearchEndEdgePt;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	IMAGE_SIZE CameraSizeW = CameraCtrl.GetCameraImageSizeW(CameraID);
	IMAGE_SIZE CameraSizeH = CameraCtrl.GetCameraImageSizeH(CameraID);
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraSizeW, CameraSizeH, ImageRes);

	ExtractRegion.minX = SearchStartPt.x - (CameraSizeW >> 1)*ImageRes.x;
	ExtractRegion.minY = SearchStartPt.y - (CameraSizeH >> 1)*ImageRes.y;
	ExtractRegion.maxX = MIN(m_StageRegion.maxX, ExtractRegion.minX + CameraSizeW*ImageRes.x);
	ExtractRegion.maxY = MIN(m_StageRegion.maxY, ExtractRegion.minY + CameraSizeH*ImageRes.y);
	RECT RoiRect = { 0 };
	
	IMAGE_PTR ImagePtr = NULL;
	IMAGE_SIZE ImageW = ExtractRegion.GetWidth() / ImageRes.x;
	IMAGE_SIZE ImageH = ExtractRegion.GetHeight() / ImageRes.y;

	IMAGE_SIZE BitCount = m_ShowBitCount;
	IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	size_t     ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	
	RoiRect.left = 0;
	RoiRect.right = ImageW;
	RoiRect.top = CameraSizeH - ImageH;
	RoiRect.bottom = CameraSizeH;
	if (JetMemory.alloc_func(ImageSize, ImagePtr, "CAutoAddComponentWnd::ExecExtractImage()", "ImagePtr") == false)
	{	return false;	}
	
	if (false == ImageAPI.ExtractColorRoiImage(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBufferPtr, RoiRect, ImageStep, ImagePtr, false)) {
		JetMemory.free_func(ImagePtr);
		return false;
	}

	m_ShowBufferW = ImageW;
	m_ShowBufferH = ImageH;
	m_ShowBufferStep = ImageStep;
	//m_ShowBufferSize = ImageSize;
	memcpy(m_ShowBufferPtr, ImagePtr, sizeof(IMAGE_DATA)*ImageSize);
	JetMemory.free_func(ImagePtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::CalcExtractRegion(TPOINT2D SearchStartPt, TREGION4D &ExtractRegion)
{
	TPOINT2D ImageRes;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	IMAGE_SIZE CameraSizeW = CameraCtrl.GetCameraImageSizeW(CameraID);
	IMAGE_SIZE CameraSizeH = CameraCtrl.GetCameraImageSizeH(CameraID);
	AOIDataCollect.GetCameraImageInfo(CameraID, CameraSizeW, CameraSizeH, ImageRes);

	ExtractRegion.minX = MAX(m_StageRegion.minX, SearchStartPt.x - (CameraSizeW >> 1)*ImageRes.x);
	ExtractRegion.minY = MAX(m_StageRegion.minY, SearchStartPt.y - (CameraSizeH >> 1)*ImageRes.y);
	ExtractRegion.maxX = MIN(m_StageRegion.maxX, SearchStartPt.x + (CameraSizeW >> 1)*ImageRes.x);
	ExtractRegion.maxY = MIN(m_StageRegion.maxY, SearchStartPt.y + (CameraSizeH >> 1)*ImageRes.y);

	return true;
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::DrawImage(HDC hDC, const RECT & Rect)
{
	unsigned char *pDibBits = m_Dib.GetDIBBits();
	if (NULL == pDibBits) { return; }

	CString str;
	double Scale = 0;
	double ScaleW = 0;
	double ScaleH = 0;
	const double WndW = Rect.right - Rect.left;
	const double WndH = Rect.bottom - Rect.top;
	const double ImageW = m_Dib.GetImageW();
	const double ImageH = m_Dib.GetImageH();
	const int    PatternPolarity = CWnd::GetDlgItemInt(PATTERN_TOWARD_EDIT);
	const BOOL   bShowInfo = CWnd::IsDlgButtonChecked(PATTERN_SHOW_INFO_CHK);
	
	ScaleW = WndW;
	ScaleH = WndH;
	ScaleW = ScaleW / ImageW;
	ScaleH = ScaleH / ImageH;
	double NewWndW = ScaleH*ImageW;
	double NewWndH = ScaleW*ImageH;

	if (NewWndW < WndW)
	{
		Scale = ScaleH;
		NewWndH = WndH;
	}
	else
	{
		Scale = ScaleW;
		NewWndW = WndW;
	}
	if (AOIDataCollect.LimitImageZoomScale(Scale, false) == true)
	{
		NewWndW = Scale*ImageW;
		NewWndH = Scale*ImageH;
	}
	const int BltMode = AOIDataCollect.GetSystemParameter().m_StretchBltMode;
	const int OldMode = ::SetStretchBltMode(hDC, BltMode);
	if (2 == PatternPolarity)
	{
		CDib TempDib = m_Dib;
		TempDib.DoRotate_180();
		TempDib.DrawPartion(hDC, 0, 0, (int)ImageW, (int)ImageH, 0, 0, (int)(NewWndW), (int)(NewWndH));
	}
	else
	{
		m_Dib.DrawPartion(hDC, 0, 0, (int)ImageW, (int)ImageH, 0, 0, (int)(NewWndW), (int)(NewWndH));
	}
	::SetStretchBltMode(hDC, OldMode);
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::CreateBKImage()
{
	HDC hDC = m_ConponentImageWndMemDC.GetSafeHdc();
	if (NULL == hDC) { return ; }

	RECT Rect = { 0,0,0,0 };
	m_ComponentImageWnd.GetClientRect(&Rect);
	HBRUSH hBrush = ::CreateSolidBrush(m_BkColor);
	if (NULL != hBrush)
	{
		::FillRect(hDC, &Rect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}
	DrawImage(hDC, Rect);
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::UpdateParamToUI(UINT FromCtrlID)
{
	if ( AAC_PATTERN_INDEX_SPIN != FromCtrlID )
	{	m_IndexSpin.SetPos(m_PatternIndex+1); }

	if (AAC_A_POINT_X_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(AAC_A_POINT_X_EDIT, m_PointAPos.x);	}
	if (AAC_A_POINT_Y_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(AAC_A_POINT_Y_EDIT, m_PointAPos.y);	}
	if (AAC_B_POINT_X_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(AAC_B_POINT_X_EDIT, m_PointBPos.x);	}
	if (AAC_B_POINT_Y_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(AAC_B_POINT_Y_EDIT, m_PointBPos.y);	}

	if (AAC_PATTERN_NUMBER_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(AAC_PATTERN_NUMBER_EDIT, m_PatternCount);	}
	
	if (AAC_PATTERN_INDEX_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(AAC_PATTERN_INDEX_EDIT, m_PatternIndex+1); }
	return;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::SwitchPatternImage(int index, bool UpdateUI)
{
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }

	BOOL       bGray = false;
	BOOL       bBinary = false;
	BOX_TOWARD WndToward = BOX_TOWARD_UP;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		
	
	IMAGE_PTR  ImagePtr=NULL;
	CPatternParam *PatParamPtr=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;

	PatParamPtr = AlgParam.GetAlgPatternParamPtr(index, true);
	if ( NULL == PatParamPtr ) 
	{	return false; }
	if ( AlgParam.LoadAlgPatternImage(index, WndToward, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	
		ImageW = 128;
		ImageH = 128;
		BitCount = 24;
		ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize,ImagePtr, "CEditImagePatternWnd::SwitchPatternImage()", "ImagePtr") == false )
		{	return false; }
		::memset(ImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	}
	
	size_t    i=0;
	TREGION4D RoiRegion;
	TREGION4D RoiRegionR;
	double    RoiCpX=0;
	double    RoiCpY=0;
	double    RotateAngle=0;		
	CAlgBinaryParam BinaryParam = PatParamPtr->GetBinaryParam();
	const double    OffsetValue=0.0;		
	const double    GainValue = BinaryParam.GetGrayGainValue();
	const bool      GainEnabled=BinaryParam.CheckGrayGainEnabed();

	m_PatternParam = *PatParamPtr;
	if ( true == UpdateUI )
	{	UpdateParamToUI(NULL);	}
	const int PolarityIdx = m_PatternParam.GetResultPolarityIdx();	
	m_PatternRoiList=m_PatternParam.GetPatRoiList(WndToward, PolarityIdx);
	if ( TRUE == bGray )
	{
		MASK_PTR   MaskPtr=NULL;		
		IMAGE_PTR  GrayPtr=NULL;		
		const int  nAlign = 4;
		const IMAGE_SIZE GrayBitCount = 8;
		const IMAGE_SIZE GrayStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, GrayBitCount, nAlign);		
		if ( AlgParam.ExecAlgImageBinary(BinaryParam, ImageW, ImageH, ImageStep, BitCount, ImagePtr, NULL, NULL, nAlign, GrayPtr, MaskPtr) == false )
		{
			JetMemory.free_func(ImagePtr);
			return false;
		}

		bool bIsOK = false;
		bIsOK = ImageAPI.RGBImageToColorImage3(ImageW, ImageH, GrayStep, GrayPtr, GrayPtr, GrayPtr, ImageStep, ImagePtr, false);		
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		if ( false == bIsOK )
		{	
			JetMemory.free_func(ImagePtr); 
			return false;
		}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr);
	}
	else if (TRUE==bBinary && BINARY_DISABLE!=BinaryParam.GetBinaryMode() )
	{		
		RECT       Rect={0,0,0,0};
		MASK_PTR   MaskPtr=NULL;		
		const int        nAlign = 4;
		const IMAGE_SIZE MaskBitCount = 8;
		const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, MaskBitCount, nAlign);		
		JetAPI::SizeToRect(ImageW, ImageH, Rect);
		if ( AlgParam.ExecAlgImageBinary_Rect(BinaryParam, Rect, Rect, ImageW, ImageH, ImageStep, BitCount, ImagePtr, NULL, NULL, nAlign, MaskPtr) == false )
		{
			JetMemory.free_func(ImagePtr);
			return false;
		}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr);
		
		bool bIsOK = false;
		MASK_DATA mask = 0xFF;	
		IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0xFF, Alpha=0;
		AOIDataCollect.GetMaskImageColor(FRAME_UNIQUE_ID_NULL, mskR, mskG, mskB, mskV, Alpha);
		if ( 24 == BitCount )
		{	bIsOK = ImageAPI.ColorImageApplyMask(ImageW, ImageH, ImageStep, ImagePtr, Rect, MaskStep, MaskPtr, mask, mskR, mskG, mskB, Alpha);	}
		else if ( 8 == BitCount )
		{	bIsOK = ImageAPI.GrayImageApplyMask(ImageW, ImageH, ImageStep, ImagePtr, Rect, MaskStep, MaskPtr, mask, mskV, Alpha);	}
		else
		{	bIsOK = false;	}
		JetMemory.free_func(MaskPtr);

		if ( false == bIsOK )
		{	
			JetMemory.free_func(ImagePtr); 
			return false;
		}
	}	
	else
	{	
	#ifdef PATTERN_BINARY_USE
		if ( true == GainEnabled )
		{	ImageAPI.ImageOffsetGain3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr, OffsetValue, GainValue); }
	#endif//PATTERN_BINARY_USE
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr); 
	}

	m_Dib.SetImage(ImagePtr, ImageW, ImageH, ImageStep, BitCount, true);
	JetMemory.free_func(ImagePtr);

	CreateBKImage();
	RedrewWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ResetPatternParam()
{
	m_PatternIndex = -1;
	m_PatternParam = CPatternParam();
	UpdateParamToUI(NULL);
	m_Dib.ReleaseBuffer();	
	if ( CWnd::GetSafeHwnd() != NULL )
	{
		CreateBKImage();
		RedrewWnd();	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::BuildPanelComboxSel()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return false; }
	CAOIPanel *PanelPtr = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	CComboBox &Combox = m_PanelIndexComobx;
	CString ItemName;
	size_t i, cnt = 0;
	for (i = 0; i < PanelCount; i++) {
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if (NULL == PanelPtr) { continue; }
		ItemName.Format(L"%d", i+1);
		Combox.InsertString(cnt, ItemName);
		Combox.SetItemData(cnt, cnt);
		cnt++;
	}
	JetAPI::SetComboxCurSel(Combox, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::BuildBoardComboxSel()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return false; }
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;

	const int ActPanelIndex = JetAPI::GetComboxCurSelData(m_PanelIndexComobx);
	if (ActPanelIndex < 0) { return false; }
	PanelPtr = ProjectPtr->GetProjectPanelPtr(ActPanelIndex, false);
	if (NULL == PanelPtr) { return false; }
	const size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();
	CComboBox &Combox = m_BoardIndexComobx;
	
	CString ItemName;
	size_t i, cnt = 0;
	for (i = 0; i < PanelBoardCount; i++) {
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if (NULL == BoardPtr) { continue; }
		ItemName.Format(L"%d", i+1);
		Combox.InsertString(cnt, ItemName);
		Combox.SetItemData(cnt, cnt);
		cnt++;
	}
	JetAPI::SetComboxCurSel(Combox, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::BuildConnectivityComboxSel()
{
	CComboBox &Combox = m_BlobConnectivityComobx;

	CString ItemName;
	int cnt = 0;
	ItemName = L"4";
	Combox.InsertString(cnt, ItemName);
	Combox.SetItemData(cnt, 4);
	cnt++;

	ItemName = L"8";
	Combox.InsertString(cnt, ItemName);
	Combox.SetItemData(cnt, 8);

	JetAPI::SetComboxCurSel(Combox, 4);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecOpenModelLibrary()
{
	CEditLibraryWnd *Wnd = m_EditLibraryWnd;
	CWnd *pAnchor = GetDlgItem(AAC_IMAGE_WND);
	if (::IsWindow(Wnd->GetSafeHwnd())) {
		if (Wnd->IsIconic()) { // 如果被最小化了
			Wnd->ShowWindow(SW_RESTORE);
			pAnchor->ShowWindow(SW_HIDE);
			pAnchor->EnableWindow(false);
		}
		else {
			Wnd->ShowWindow(SW_MINIMIZE);
			pAnchor->ShowWindow(SW_SHOW);
			pAnchor->EnableWindow(true);
		}
		//Wnd->BringWindowToTop();
		//Wnd->SetForegroundWindow();
		return true;
	}
	if (Wnd->Create(CEditLibraryWnd::IDD, this))
	{
		CRect AnchorRect,WndRect;
		pAnchor->GetWindowRect(&AnchorRect);
		ScreenToClient(&AnchorRect);
		pAnchor->ShowWindow(SW_HIDE);
		pAnchor->EnableWindow(false);
		
		Wnd->GetWindowRect(&WndRect);
		//Wnd->SetParent((CWnd*)&AgentWnd);
		//Wnd->SetParent(NULL);
		//Wnd->ModifyStyle(0, WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX, SWP_FRAMECHANGED);
		/*CMenu* pSysMenu = Wnd->GetSystemMenu(FALSE);
		if (pSysMenu) {
			pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED); 
		}*/
		Wnd->CenterWindow();
		Wnd->SetWindowPos(&CWnd::wndTop, AnchorRect.left, AnchorRect.top, WndRect.Width(), WndRect.Height(),
			SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		Wnd->BringWindowToTop();
		Wnd->PostMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
	}
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecAddComponent()
{

	CAOIProject* ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return false; }
	const int PanelIndex = JetAPI::GetComboxCurSelData(m_PanelIndexComobx);
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
	if (NULL == PanelPtr) { return false; }
	const int BoardIndex = JetAPI::GetComboxCurSelData(m_BoardIndexComobx);
	CAOIBoard * BoardPtr = PanelPtr->GetPanelBoardPtr(BoardIndex, true);
	if (NULL == BoardPtr) { return false; }
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	CMapCoordinate *MapSTCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);
	if (NULL == MapSTCPtr) { return false; }

	size_t i;
	CString str;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent* ComponentPtr = NULL;
	CString        ModelName = _T("");
	CString        ComponentName = _T("");
	CString        PartNumber = _T("");
	CString        ComponentSetName;
	CWnd::GetDlgItemText(AAC_MODEL_NAME_EDIT, ModelName);
	CWnd::GetDlgItemText(AAC_PART_NUMBER_EDIT, PartNumber);
	CWnd::GetDlgItemText(AAC_COMPONENT_NAME_EDIT, ComponentSetName);

	if (ModelName.IsEmpty()) { ModelName = ComponentSetName; }
	if (PartNumber.IsEmpty()) { PartNumber = ModelName; }
	double ComponentSizeW, ComponentSizeH;
	TPOINT2D StageRegionCp, ComponentStagePos, ComponentCadPos;
	TREGION4D    SpecRgn, ComponentCadRgn, ComponentStageRgn;

	IMAGE_SIZE   ImageW = m_ShowImageW;
	IMAGE_SIZE   ImageH = m_ShowImageH;
	TPOINT2D     ImageRes = m_FrameResolution;
	TREGION4D    ImageStageRgn = m_FrameStageRgn;
	TPOINT2D     ImageStagePos;
	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();
	
	TBasePlaneParam BasePlaneParam;
	TNoiseFilterParam NoiseFilterParam;
	const int BasePlaneIndex = AOIDataCollect.GetSystemParameter().m_DefaultSpaceBasePlaneIndex;
	const int NoiseFilterIndex = AOIDataCollect.GetSystemParameter().m_DefaultSpaceNoiseFilterIndex;
	if ( BasePlaneIndex >= 0 )
	{	AOIDataCollect.GetSystemBasePlaneParam(BasePlaneIndex, BasePlaneParam);	}
	if ( NoiseFilterIndex >= 0 )
	{	AOIDataCollect.GetSystemNoiseFilterParam(NoiseFilterIndex, NoiseFilterParam);	}

	TPOINT2D NewComponentCornerPt[4];
	const int NewComponentCount = m_FoundRectList.size();
	//TComponentRect NewComponentRect;
	TBOX_DRAW_PARAM NewComponentRect;
	RECT StageRect;
	ProjectPtr->SelectProjectAllComponents(false);
	for (i = 0; i < NewComponentCount; i++) {
		NewComponentRect = m_FoundRectList.at(i);
		StageRect = NewComponentRect.WndRect;
		JetAPI::Rect4DToCornerPt(StageRect, NewComponentCornerPt);
		JetAPI::PointsToRegion(NewComponentCornerPt, 4, ComponentStageRgn);

		ComponentStagePos.x = ComponentStageRgn.GetCpX();
		ComponentStagePos.y = ComponentStageRgn.GetCpY();
		MapSTCPtr->Map2D(ComponentStagePos.x, ComponentStagePos.y, ComponentCadPos.x, ComponentCadPos.y);
		if (fmod(NewComponentRect.ComponentAngle, 180) == 0) {
			ComponentSizeW = ComponentStageRgn.GetWidth();
			ComponentSizeH = ComponentStageRgn.GetHeight();
		}else {
			ComponentSizeW = ComponentStageRgn.GetHeight();
			ComponentSizeH = ComponentStageRgn.GetWidth();
		}
		ComponentName.Format(_T("%s_%d"), ComponentSetName, i);

		ComponentPtr = AOIObjManager.CreateComponentObj();
		if (NULL == ComponentPtr) { return false; }

		ComponentPtr->SetComponentName(ComponentName);
		ComponentPtr->SetComponentModelName(ModelName);
		ComponentPtr->SetComponentPartNumber(PartNumber);
		ComponentPtr->SetComponentAngle(NewComponentRect.ComponentAngle);
		ComponentPtr->SetComponentType(COMPONENT_TYPE_NORMAL);
		ComponentPtr->SetComponentBodySizeW(ComponentSizeW);
		ComponentPtr->SetComponentBodySizeH(ComponentSizeH);
		ComponentPtr->SetComponentRoiSizeW(ComponentSizeW);
		ComponentPtr->SetComponentRoiSizeH(ComponentSizeH);
		ComponentPtr->SetComponentCadPos(ComponentCadPos);
		ComponentPtr->SetComponentOrgCadPos(ComponentCadPos);
		ComponentPtr->SetComponentStagePos(ComponentStagePos);
		ComponentPtr->SetComponentDistrictID(DistrictID);
		ComponentPtr->CalcComponentCadCornerPos();
		ComponentPtr->LayoutComponentStageCornerPos();
		ComponentPtr->UpdateComponentParamToModel(false);
		ComponentPtr->SetComponentSpaceBasePlaneParam(BasePlaneParam);
		ComponentPtr->SetComponentSpaceNoiseFilterParam(NoiseFilterParam);

		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL != ModelPtr )
		{	ModelPtr->SetModelName(ModelName);	}
		

		CAOIModel *ModelPtrM = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
		if ( NULL != ModelPtrM )
		{	ComponentPtr->UpdateComponentModelFromLibrary(ModelPtrM);	}

		ProjectPtr->AddProjectComponentPtr(ComponentPtr, false);
		PanelPtr->AddPanelComponentPtr(ComponentPtr);
		BoardPtr->AddBoardComponentPtr(ComponentPtr);
		
		ComponentPtr->SetComponentSelected(true);
		BoardPtr->LayoutBoardRegion();
		PanelPtr->LayoutPanelRegion();
		LogOperCtrl.SaveLogProjectComponentSelectedCreate(ProjectPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::UpdateModelPtr()
{
	CAOIModel *ModelPtr = AOIDataCollect.GetModelPreViewPtr();
	if (ModelPtr == NULL) { return false; }
	CString str = ModelPtr->GetModelName();
	CWnd::SetDlgItemText(AAC_MODEL_NAME_EDIT, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::MoveMethodGroupUI()
{
	MoveMethodGroupUI(m_ImageMatchUI_IDList.data(), m_ImageMatchUI_IDList.size(), AAC_METHOD_ANCHOR);
	MoveMethodGroupUI(m_ImageBlobUI_IDList.data(), m_ImageBlobUI_IDList.size(), AAC_METHOD_ANCHOR);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::MoveMethodGroupUI(UINT * IDArray, size_t Count, UINT Anchor)
{
	if (Count == 0) { return true; }
	size_t i = 0;
	CRect rcCtrl;
	CWnd* pWnd = NULL;
	CRect rcAnchor, rcOldGroup;
	GetDlgItem(Anchor)->GetWindowRect(&rcAnchor);
	ScreenToClient(&rcAnchor);
	GetDlgItem(IDArray[0])->GetWindowRect(&rcOldGroup);
	ScreenToClient(&rcOldGroup);

	const int dx = rcAnchor.left - rcOldGroup.left;
	const int dy = rcAnchor.top - rcOldGroup.top;

	for (i = 0; i < Count; i++) {
		pWnd = GetDlgItem(IDArray[i]);
		if (pWnd == NULL) { continue; }
		pWnd->GetWindowRect(&rcCtrl);
		ScreenToClient(&rcCtrl);
		pWnd->SetWindowPos(NULL, rcCtrl.left + dx, rcCtrl.top + dy, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::SetMethodGroupUIVisible()
{
	bool bImageMatchEnable = CWnd::IsDlgButtonChecked(AAC_IMAGE_MATCHING_RADIO);
	bool bBlobEnable = CWnd::IsDlgButtonChecked(AAC_BLOB_RADIO);
	//bool bImageMatchEnable = false;
	//bool bBlobEnable = true;
	if (::IsWindow(m_wndColorFilter->GetSafeHwnd())) {
		m_wndColorFilter->ShowWindow(SW_HIDE);
	}
	SetMethodGroupUIVisible(m_ImageFoundUI_IDList.data(), m_ImageFoundUI_IDList.size(), true);
	if (true == bImageMatchEnable) {
		SetMethodGroupUIVisible(m_ImageMatchUI_IDList.data(), m_ImageMatchUI_IDList.size(), true);
		SetMethodGroupUIVisible(m_ImageBlobUI_IDList.data(), m_ImageBlobUI_IDList.size(), false);
		return true;
	}
	else if(true == bBlobEnable) {
		SetMethodGroupUIVisible(m_ImageMatchUI_IDList.data(), m_ImageMatchUI_IDList.size(), false);
		SetMethodGroupUIVisible(m_ImageBlobUI_IDList.data(), m_ImageBlobUI_IDList.size(), true);
		return true;
	}
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::SetMethodGroupUIVisible(UINT * IDArray, size_t Count, bool bVisible)
{
	size_t i = 0;
	CWnd* pWnd = NULL;
	for (i = 0; i < Count; i++) {
		pWnd = GetDlgItem(IDArray[i]);
		if (pWnd == NULL) { continue; }
		pWnd->ShowWindow(bVisible);
		pWnd->EnableWindow(bVisible);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecOpenImageProcessWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return true; }

	SetMethodGroupUIVisible(m_ImageBlobUI_IDList.data(), m_ImageBlobUI_IDList.size(), false);
	SetMethodGroupUIVisible(m_ImageFoundUI_IDList.data(), m_ImageFoundUI_IDList.size(), false);
	CWnd::CheckDlgButton(AAC_BLOB_RADIO, BST_UNCHECKED);
	CEditImageColorFilterWnd *Wnd = m_wndColorFilter;
	if (::IsWindow(Wnd->GetSafeHwnd())) {
		if (Wnd->IsIconic()) { // 如果被最小化了
			Wnd->ShowWindow(SW_RESTORE);
		}
		else {
			Wnd->ShowWindow(SW_SHOW);
		}
		Wnd->BringWindowToTop();
		Wnd->SetForegroundWindow();
		return true;
	}
	if (Wnd->Create(CEditImageColorFilterWnd::IDD, this))
	{
		CRect AnchorRect, rectPlace;
		GetDlgItem(AAC_METHOD_ANCHOR)->GetWindowRect(&AnchorRect);
		ScreenToClient(&AnchorRect);
		Wnd->GetWindowRect(&rectPlace);
		Wnd->SetWindowPos(&CWnd::wndTop, AnchorRect.left, AnchorRect.top, rectPlace.Width(), rectPlace.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
		Wnd->BringWindowToTop();
		InvisibleImageProcessWndElement();
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::InvisibleImageProcessWndElement()
{
	bool bVisible = false;
	CEditImageColorFilterWnd *Wnd = m_wndColorFilter;
	CWnd* pObj;
	pObj = Wnd->GetDlgItem(CLRFTR_EXTRACT_WND_COLOR_BTN);
	pObj->EnableWindow(bVisible);

	pObj = Wnd->GetDlgItem(CLRFTR_COLOR_GROUP_BTN_LINK);
	pObj->EnableWindow(bVisible);

	pObj = Wnd->GetDlgItem(CLRFTR_COLOR_GROUP_EDIT);
	pObj->EnableWindow(bVisible);

	pObj = Wnd->GetDlgItem(CLRFTR_COLOR_GROUP_BTN_SEND);
	pObj->EnableWindow(bVisible);

	pObj = Wnd->GetDlgItem(CLRFTR_GATHER_SHOW_RAW_CHK);
	pObj->EnableWindow(bVisible);

	pObj = Wnd->GetDlgItem(CLRFTR_EXTRACT_WND_COLOR_BTN);
	pObj->EnableWindow(bVisible);

	pObj = Wnd->GetDlgItem(CLRFTR_SHOW_WND_COLOR_BTN);
	pObj->EnableWindow(bVisible);

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecGatherColor()
{
	bool bLockUIWnd = GetLockUIWnd();
	if (true == bLockUIWnd) { return true; }
	if (m_AAC_WND_IMAGE == AAC_WND_IMAGE_MAP) { return true; }
	LockUIWnd(true);

	RECT RoiRect;
	CColorRGBV rgbv;
	CAlgBinaryParam *BinParamPtr = &m_BinaryParam;

	CColorRGBV *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if (NULL == rgbvPtr) { return true; }
	m_ImageWnd.GetImageEditRect(RoiRect);
	if (ImageAPI.CalcColorImageColorFilter(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBufferPtr, RoiRect, rgbv) == false)
	{	return false;	}
	bool CombineColorMode = false;
	if ( false==CombineColorMode || rgbvPtr->CheckUsed()==false )
	{	rgbvPtr->CopyColorRGBV(rgbv);	}
	else
	{	rgbvPtr->MergeColor(false, &rgbv);	}
	rgbvPtr->CalcShowColor();
	
	m_wndColorFilter->SendMessage(MSG_COLOR_FILTER_WND, WPARAM_UPDATE_COLOR_FILTER, NULL);
	
	RoiRect.left = -1;
	RoiRect.right = -1;
	RoiRect.top = -1;
	RoiRect.bottom = -1;
	m_ImageWnd.SetImageEditRect(RoiRect);
	m_wndColorFilter->PostMessage(MSG_COLOR_FILTER_WND, WPARAM_UPDATE_WND_COLOR, NULL);
	//BuildColorFilterImage(BinParamPtr);
	//ExecFindBlob_CurrentFov();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecSelectTempObj()
{
	if (m_FoundRectList.size() == 0) { return true; }
	RECT RoiRect, WndRect, StageRect;
	SIZE RectSize;
	m_ImageWnd.GetImageEditRect(RoiRect);
	JetAPI::GetRectSize(RoiRect, RectSize);
	if (RectSize.cx > 0 && RectSize.cy > 0) { return true; }
	POINT WndLBtnUpPos = m_ImageWnd.GetLBtnUpPos();
	TPOINT2D ImageLBtnUpPos, StageLBtnUpPos;

	TREGION4D FovRegion;
	IMAGE_SIZE ImageW, ImageH;
	TPOINT2D ImageRes, FovRegionCp;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	switch (m_AAC_WND_IMAGE)
	{
	case AAC_WND_IMAGE_MAP:
		FovRegion = m_FrameStageRgn;
		ImageW = m_ProjectMapW;
		ImageH = m_ProjectMapH;
		ImageRes = m_FrameResolution;
		break;
	case AAC_WND_IMAGE_FOV:
		AOIDataCollect.GetFovStageRegionReal(FovRegion);
		AOIDataCollect.GetCameraImageInfo(CameraID, ImageW, ImageH, ImageRes);
		break;
	default:
		break;
	}
		
	FovRegionCp.x = FovRegion.GetCpX();
	FovRegionCp.y = FovRegion.GetCpY();

	m_ImageOffset = m_ImageWnd.GetImageOffset();
	m_ImageZoom = m_ImageWnd.GetImageZoom();

	ImageAPI.MapWndPtToImagePt_DBL(m_ShowBufferW, m_ShowBufferH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndLBtnUpPos, ImageLBtnUpPos);
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImageLBtnUpPos, FovRegionCp, StageLBtnUpPos);

	TBOX_DRAW_PARAM* BoxParamPtr;
	TPOINT2D CornerPts[4];
	TREGION4D Region4D;
	size_t i, index;
	if (false == (GetKeyState(VK_CONTROL) & 0x8000)) {  
		// Ctrl 沒有按住
		//m_BlobListCtrl.SetItemState(-1, 0, LVIS_SELECTED);
		m_BlobListCtrl.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
	}
	for (i = 0; i < m_FoundRectList.size(); i++) {
		BoxParamPtr = &m_FoundRectList[i];
		StageRect = BoxParamPtr->WndRect;
		JetAPI::RectToCornerPt(StageRect, CornerPts);
		JetAPI::CornerPtToRegion(CornerPts, Region4D);
		if (false == Region4D.CheckPtInside(StageLBtnUpPos)) { continue; }
		UINT state = m_BlobListCtrl.GetItemState(i, LVIS_SELECTED);
		if (state & LVIS_SELECTED) { continue; }
		m_BlobListCtrl.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_BlobListCtrl.EnsureVisible(i, FALSE);
		m_BlobListCtrl.UpdateWindow();
		m_BlobListCtrl.SetFocus();
		break;
	}
	
	MarkSelectTempObj();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::OnSelChangeColorWndList()
{
	LockUIWnd(true);
	//CAlgBinaryParam BinParam;
	//AOIDataCollect.GetBinaryParamTemp(BinParam);
	CAlgBinaryParam *BinParamPtr = &m_BinaryParam;
	BuildColorFilterImage(BinParamPtr);
	ExecFindBlob_CurrentFov();
	SetImageWndTempObj();
	LockUIWnd(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::BuildColorFilterImage(CAlgBinaryParam *BinParamPtr)
{
	RECT MaskRect = { 0,0,m_ShowBufferW ,m_ShowBufferH };
	TPOINT2D WndCenterPos = m_ImageWnd.GetStagePosAtWndCenterPos();
	

	MASK_PTR   MaskPtr = NULL;
	const int  nAlign = 4;
	const IMAGE_SIZE MaskBitCount = 8;
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowBufferW, MaskBitCount, nAlign);
	bool bOpenMP = false;

	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, m_ShowBufferH);
	if (JetMemory.alloc_func(MaskBufferSize, MaskPtr, "CAutoAddComponentWnd::BuildColorFilterImage()", "MaskPtr") == false)
	{	return false;	}

	if (ImageAPI.ColorImageColorFilter3(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBufferPtr, BinParamPtr->GetBinaryColorGroup(), MaskRect, MaskStep, MaskPtr, true, bOpenMP) == false)
	{
		JetMemory.free_func(MaskPtr);
		return false;
	}
	
#ifdef _DEBUG
	CString str;
	CString DebugFolder = AOIDataCollect.GetAOIProjectDebugDirectory();
	str.Format(_T("%s\\ColorMask.PNG"), DebugFolder);
	ImageAPI.SaveImage(str, m_ShowBufferW, m_ShowBufferH, MaskStep, MaskBitCount, MaskPtr, true);
#endif // _DEBUG

	MASK_DATA mask = 0xFF;
	const unsigned int FrameUniqueID = BinParamPtr->GetBinaryFrameUniqueID();
	IMAGE_DATA mskR = 0xFF, mskG = 0xFF, mskB = 0x00, mskV = 0xFF, Alpha = 0;
	AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);

	if (AOIDataCollect.ExecEnhanceDisplayImage(m_ShowBufferW, m_ShowBufferH, m_ShowBufferStep, m_ShowBitCount, m_ShowBufferPtr, m_ShowImagePtr) == false)
	{
		JetMemory.free_func(MaskPtr);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}
	m_ShowImageW = m_ShowBufferW;
	m_ShowImageH = m_ShowBufferH;
	m_ShowImageStep = m_ShowBufferStep;
	m_ShowBitCount = m_ShowBitCount;

	if (false == ImageAPI.ColorImageApplyMask(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowImagePtr, MaskRect, MaskStep, MaskPtr, mask, mskR, mskG, mskB, Alpha)) {
		JetMemory.free_func(MaskPtr);
		return false;
	}

	JetMemory.free_func(MaskPtr);
	m_ImageWnd.ShowFittedZoom();
	m_ImageWnd.RedrawWnd(false);

	CreateBKDC(m_ResetView);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::MergeOverlappingRects()
{
	//用於合併重複框選的零件
	if (m_FoundRectList.empty()) { return true; }
	//const double Threshold = 0.8;

	size_t i, j;
	TREGION4D Region4D;
	std::vector<TREGION4D> RegionList;
	std::set<int> RemoveList;
	const size_t RectCount = m_FoundRectList.size();
	size_t RectTempCount = RectCount;

	TPOINT2D CornerPts[4];
	TPOINT2D CenterPt;
	for (i = 0; i < RectCount; i++) {
		JetAPI::RectToCornerPt(m_FoundRectList.at(i).WndRect, CornerPts);
		JetAPI::CornerPtToRegion(CornerPts, Region4D);
		RegionList.push_back(Region4D);
	}
	TREGION4D RegionRf, RegionUn,RegionIts;
	//uble IOU;
	for (i = 0; i<RectTempCount; i++) {
		TREGION4D &RegionTg = RegionList.at(i);
		j = i;
		while(++j<RectTempCount){
			RegionRf = RegionList.at(j);
			CenterPt.x = (RegionTg.GetCpX() + RegionRf.GetCpX())/2;
			CenterPt.y = (RegionTg.GetCpY() + RegionRf.GetCpY())/2;
			
			
			if (false == RegionTg.CheckPtInside(CenterPt)) { continue; };//排除兩個區域完全沒有重疊
			//JetAPI::UnionRegion(RegionTg, RegionRf, RegionUn);
			//JetAPI::IntersectRegion(RegionTg, RegionRf, RegionIts);
			//if (RegionIts.GetArea() == 0) { continue; }
			//IOU = RegionIts.GetArea() / RegionUn.GetArea();
			//if (IOU < Threshold) { continue; }
			JetAPI::UnionRegion(RegionTg, RegionRf, RegionUn);
			RegionRf = RegionUn;
			RegionList.erase(RegionList.begin() + j);
			m_BlobListCtrl.DeleteItem(j);

			m_FoundRectList.erase(m_FoundRectList.begin() + j);
			RectTempCount--;
			j--;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ReBuildFoundList()
{
	CString str;
	for (int i = 0; i < m_BlobListCtrl.GetItemCount(); i++) {
		str.Format(L"%d", i + 1);
		m_BlobListCtrl.SetItemText(i, 0, str);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::SetImageWndTempObj()
{
	ReBuildFoundList();
	std::vector<TBOX_DRAW_PARAM> TempObj = m_FoundRectList;
	SetDlgItemInt(AAC_FOUND_LIST_COUNT_EDIT, m_FoundRectList.size());
	TBOX_DRAW_PARAM SearchObj;//搜尋區域
	RECT SearchRect;
	JetAPI::Region4DToRect(m_StageRegion, SearchRect, false);
	SearchObj.WndRect = SearchRect;
	SearchObj.ComponentAngle = 0;
	SearchObj.hPenNull = m_hPen;
	TempObj.push_back(SearchObj);


	m_ImageWnd.SetTempObjList(TempObj);
	m_ImageWnd.RedrawWnd(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::BuildFoundListWndHeader()
{
	bool bImageMatchEnable = CWnd::IsDlgButtonChecked(AAC_IMAGE_MATCHING_RADIO);
	bool bBlobEnable = CWnd::IsDlgButtonChecked(AAC_BLOB_RADIO);
	if (true == bImageMatchEnable) {
		BuildFoundListWndHeader_Pattern();
	}
	else{
		BuildFoundListWndHeader_Blob();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::BuildFoundListWndHeader_Blob()
{
	CString str;
	int   nCol = 0;
	int width = 64;
	int width2 = 64;
	RECT      Rect;
	CThisListCtrl_72 &ListCtrl = m_BlobListCtrl;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT	
	{
		ListCtrl.GetClientRect(&Rect);
		width = 16;
		width2 = (Rect.right - Rect.left - width - 32);
		str = _T("idx");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width = 48;
		width2 -= width;
		str = _T("StagePosX");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*1.5);
		nCol++;

		width2 -= width;
		str = _T("StagePosX");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width*1.5);
		nCol++;

		width2 -= width;
		str = _T("X-Size");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width2 -= width;
		str = _T("Y-Size");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width2 -= width;
		str = _T("L-Size");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width2 -= width;
		str = _T("Area");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width * 1.5);
		nCol++;

		width2 -= width;
		str = _T("Aspect");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width2 -= width;
		str = _T("Fill");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		if (width2 > width) {
			width = width2;
		}
		str = _T("L/S");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::BuildFoundListWndHeader_Pattern()
{
	CString str;
	int   nCol = 0;
	int width = 96;
	int width2 = 64;
	RECT      Rect;
	CThisListCtrl_72 &ListCtrl = m_BlobListCtrl;
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT, LVCFMT_LEFT	
	{
		ListCtrl.GetClientRect(&Rect);
		width = 48;
		width2 = (Rect.right - Rect.left - width - 32);
		str = _T("idx");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width = 64;
		width2 -= width;
		str = _T("StagePosX");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width2 -= width;
		str = _T("StagePosX");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width2 -= width;
		str = _T("ScaleX");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width2 -= width;
		str = _T("ScaleY");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width2 -= width;
		str = _T("Similarity");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

		width = width2;
		str = _T("Angle");
		str = LoadMultiLanguageString(str, str);
		ListCtrl.InsertColumn(nCol, str, Align, width);
		nCol++;

	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ClearFoundListWnd()
{
	CThisListCtrl_72 &ListCtrl = m_BlobListCtrl;
	JetAPI::ClearListCtrl(ListCtrl, TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAutoAddComponentWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section = _T("IDD_AUTO_ADD_COMPONENT_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_AUTO_ADD_COMPONENT_WND;
	WndKey = _T("IDD_AUTO_ADD_COMPONENT_WND");
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
	WndID = AAC_REGION_CTRL_GROUP;
	WndKey = _T("AAC_REGION_CTRL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_A_POINT_LABEL;
	WndKey = _T("AAC_A_POINT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_B_POINT_LABEL;
	WndKey = _T("AAC_B_POINT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_POINT_X_LABEL;
	WndKey = _T("AAC_POINT_X_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_POINT_Y_LABEL;
	WndKey = _T("AAC_POINT_Y_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_A_POINT_SETUP_BTN;
	WndKey = _T("AAC_A_POINT_SETUP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_B_POINT_SETUP_BTN;
	WndKey = _T("AAC_B_POINT_SETUP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_A_POINT_MOVE_BTN;
	WndKey = _T("AAC_A_POINT_MOVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_B_POINT_MOVE_BTN;
	WndKey = _T("AAC_B_POINT_MOVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_REGION_SETTING_BTN;
	WndKey = _T("AAC_REGION_SETTING_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = AAC_COMPONENT_CTRL_GROUP;
	WndKey = _T("AAC_COMPONENT_CTRL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_COMPONENT_PANEL_INDEX_LABEL;
	WndKey = _T("AAC_COMPONENT_PANEL_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_COMPONENT_BOARD_INDEX_LABEL;
	WndKey = _T("AAC_COMPONENT_BOARD_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_COMPONENT_NAME_LABEL;
	WndKey = _T("AAC_COMPONENT_NAME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_COMPONENT_PART_NUMBER_LABEL;
	WndKey = _T("AAC_COMPONENT_PART_NUMBER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_COMPONENT_MODEL_LABEL;
	WndKey = _T("AAC_COMPONENT_MODEL_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_COMPONENT_LIBRARY_OPEN_BTN;
	WndKey = _T("AAC_COMPONENT_LIBRARY_OPEN_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = AAC_METHOD_GROUP;
	WndKey = _T("AAC_METHOD_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_IMAGE_MATCHING_RADIO;
	WndKey = _T("AAC_IMAGE_MATCHING_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_BLOB_RADIO;
	WndKey = _T("AAC_BLOB_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	
	CString ImageMatchUI_StringList[] = {
		_T("AAC_IMAGE_MATCH_GROUP"),    _T("ACC_CONPONENT_IMAGE_WND"),
		_T("AAC_PATTERN_NUMBER_LABEL"), _T("AAC_PATTERN_NUMBER_EDIT"),
		_T("AAC_PATTERN_INDEX_LABEL"),  _T("AAC_PATTERN_INDEX_EDIT"),
		_T("AAC_PATTERN_ADD_BTN"),      _T("AAC_PATTERN_EDIT_BTN"),
		_T("AAC_PATTERN_DELETE_BTN"),   _T("AAC_PATTERN_CLEAR_BTN"),
		_T("AAC_PATTERN_SIMILARITY_LABEL"), _T("AAC_PATTERN_SIMILARITY_EDIT"),
		_T("AAC_PATTERN_SCALE_LABEL"),  _T("AAC_PATTERN_SCALE_LSL_EDIT"),	_T("AAC_PATTERN_SCALE_USL_EDIT"),
		_T("AAC_PATTERN_FIND_BTN"),     _T("AAC_PATTERN_INDEX_SPIN"), _T("AAC_PATTERN_ROTATE_CHECKBOX")
	};
	TCHAR szClass[256];
	for (i = 0; i < m_ImageMatchUI_IDList.size(); i++) {
		WndID = m_ImageMatchUI_IDList[i];
		pWnd = GetDlgItem(WndID);
		GetClassName(pWnd->GetSafeHwnd(), szClass, 256);
		if (_tcsicmp(szClass, _T("Edit")) == 0) { continue; }
		if (_tcsicmp(szClass, _T("msctls_updown32")) == 0) { continue; }
		if (WndID == ACC_CONPONENT_IMAGE_WND) { continue; }
		WndKey = ImageMatchUI_StringList[i];
		this->GetDlgItemText(WndID, LabelText);
		if (LabelText.IsEmpty()) { continue; }
		AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
		this->SetDlgItemText(WndID, NewLabelText);
	}
	//---------------------------------------------------------------------------------//
	CString ImageBlobUI_StringList[] = {
		_T("AAC_BLOB_GROUP"),
		_T("AAC_BLOB_LSL_LABEL"),       _T("AAC_BLOB_USL_LABEL"),
		_T("AAC_BLOB_XSIZE_LABEL"),     _T("AAC_BLOB_XSIZE_LSL_EDIT"), _T("AAC_BLOB_XSIZE_USL_EDIT"),
		_T("AAC_BLOB_YSIZE_LABEL"),     _T("AAC_BLOB_YSIZE_LSL_EDIT"), _T("AAC_BLOB_YSIZE_USL_EDIT"),
		_T("AAC_BLOB_LSIZE_LABEL"),     _T("AAC_BLOB_LSIZE_LSL_EDIT"), _T("AAC_BLOB_LSIZE_USL_EDIT"),
		_T("AAC_BLOB_AREA_LABEL"),      _T("AAC_BLOB_AREA_LSL_EDIT"),  _T("AAC_BLOB_AREA_USL_EDIT"),
		_T("AAC_BLOB_IMAGE_PROCESS_BTN"),_T("AAC_BLOB_FIND_BTN"),
		_T("AAC_BLOB_LSL_LABEL2"),      _T("AAC_BLOB_USL_LABEL2"),
		_T("AAC_BLOB_ASPECT_LABEL"),    _T("AAC_BLOB_ASPECT_LSL_EDIT"), _T("AAC_BLOB_ASPECT_USL_EDIT"),
		_T("AAC_BLOB_FILL_LABEL"),      _T("AAC_BLOB_FILL_LSL_EDIT"),   _T("AAC_BLOB_FILL_USL_EDIT"),
		_T("AAC_BLOB_LS_LABEL"),        _T("AAC_BLOB_LS_LSL_EDIT"),     _T("AAC_BLOB_LS_USL_EDIT"),
		_T("AAC_BLOB_CONNECTIVITY_LABEL"),_T("AAC_BLOB_CONNECTIVITY_COMBO")
	};
	for (i = 0; i < m_ImageBlobUI_IDList.size(); i++) {
		WndID = m_ImageBlobUI_IDList[i];

		pWnd = GetDlgItem(WndID);
		GetClassName(pWnd->GetSafeHwnd(), szClass, 256);
		if (_tcsicmp(szClass, _T("Edit")) == 0) { continue; }
		if (_tcsicmp(szClass, _T("Combo")) == 0) { continue; }

		WndKey = ImageBlobUI_StringList[i];
		this->GetDlgItemText(WndID, LabelText);
		if (LabelText.IsEmpty()) { continue; }
		AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
		this->SetDlgItemText(WndID, NewLabelText);
	}
	//---------------------------------------------------------------------------------//

	WndID = AAC_FOUND_GROUP;
	WndKey = _T("AAC_FOUND_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = AAC_FOUND_LIST_ADD_BTN;
	WndKey = _T("AAC_FOUND_LIST_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_FOUND_LIST_DEL_BTN;
	WndKey = _T("AAC_FOUND_LIST_DEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_FOUND_COUNT_LABEL;
	WndKey = _T("AAC_FOUND_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = AAC_FOUND_LIST_CLEAR_BTN;
	WndKey = _T("AAC_FOUND_LIST_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = AAC_ADD_COMPONENT_BTN;
	WndKey = _T("AAC_ADD_COMPONENT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
}
//-------------------------------------------------------------------------------------//
CString CAutoAddComponentWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section = _T("IDD_AUTO_ADD_COMPONENT_WND");
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::ExecClearTempObj()
{
	ClearFoundListWnd();
	m_FoundRectList.clear();
	SetImageWndTempObj();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAutoAddComponentWnd::MarkSelectTempObj()
{
	size_t i;
	for (i = 0; i < m_FoundRectList.size(); i++) {
		TBOX_DRAW_PARAM &Obj = m_FoundRectList.at(i);//搜尋區域
		Obj.hPenNull = NULL;
	}
	POSITION pos = m_BlobListCtrl.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{
		int nItem = m_BlobListCtrl.GetNextSelectedItem(pos);
		TBOX_DRAW_PARAM &SelObj = m_FoundRectList.at(nItem);//搜尋區域
		SelObj.hPenNull = m_hPenSel;
	}
	SetImageWndTempObj();
	return true;
}
//-------------------------------------------------------------------------------------//

