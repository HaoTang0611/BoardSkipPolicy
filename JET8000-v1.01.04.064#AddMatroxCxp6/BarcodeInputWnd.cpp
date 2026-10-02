// BarcodeInputWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "BarcodeInputWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeInputWnd dialog
//-------------------------------------------------------------------------------------//
CBarcodeInputWnd::CBarcodeInputWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CBarcodeInputWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeInputWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_LaneID = LANE_ID_NULL;
	m_ProjectPtr = NULL;
	m_BarcodeScope = BIW_PROJECT_SCOPE_RADIO;
	m_BarcodeReadMode = BARCODE_HANDHELD_READ_MANUAL;
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeInputWnd)
	DDX_Control(pDX, BIW_BARCODE_LIST_WND, m_BarcodeListCtrl);
	DDX_Control(pDX, BIW_PROJECT_MAP_WND, m_ImageWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CBarcodeInputWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CBarcodeInputWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(BIW_PROJECT_SCOPE_RADIO, OnProjectScopeRadio)
	ON_BN_CLICKED(BIW_PANEL_SCOPE_RADIO, OnPanelScopeRadio)
	ON_BN_CLICKED(BIW_BOARD_SCOPE_RADIO, OnBoardScopeRadio)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(BIW_INPUT_SEQUENCE_BTN, OnInputSequenceBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeInputWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CBarcodeInputWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	SwitchMultiLanguage();
	JetAPI::InitialListCtrl(m_BarcodeListCtrl);	
	EnableCloseBtn(FALSE);

	switch ( m_BarcodeReadMode )
	{
	case BARCODE_HANDHELD_READ_PROJECT:	OnProjectScopeRadio();	break;
	case BARCODE_HANDHELD_READ_PANEL:	OnPanelScopeRadio(); break;
	case BARCODE_HANDHELD_READ_BOARD:	OnBoardScopeRadio(); break;
	}
	CWnd::CheckDlgButton(m_BarcodeScope, TRUE);	

	InitialProjectMapWnd();	
	CWnd::CenterWindow();
	CWnd::ShowWindow(SW_SHOWNORMAL);
	BuildBarcodeInfoListWnd_Header();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	CWnd::PostMessage(MSG_SELF_WND_EXTRA_MESSAGE, WPARAM_FIRST_UI_CALLBACK, NULL);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( GetSafeHwnd() == NULL ) { return; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	if ( m_BarcodeListCtrl.GetSafeHwnd() == NULL ) { return; }

	CWnd *WndPtr = NULL;	
	const int MarginX = 4;
	const int MarginY = 4;

	if ( m_BarcodeListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right  = cx-MarginX;
		WndRect.bottom = cy-MarginY;
		m_ImageWnd.MoveWindow(&WndRect, FALSE);		
		m_ImageWnd.ShowFittedZoom();
	}

	if ( m_BarcodeListCtrl.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		m_BarcodeListCtrl.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.bottom = cy-MarginY;
		m_BarcodeListCtrl.MoveWindow(&WndRect, FALSE);		
	}
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_BARCODE_INPUT_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_BARCODE_INPUT_WND;
	WndKey = _T("IDD_BARCODE_INPUT_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	LANE_ID LaneID = GetLaneID();
	if (LANE_ID_NULL != LaneID)
	{
		CString strCaptionNew;
		CString strLaneID = AOIDataDefine.GetLaneIDText(LaneID);
		strCaptionNew.Format(_T("%s [%s]"), NewLabelText, strLaneID);
		CWnd::SetWindowText(strCaptionNew);
	}
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = BIW_PROJECT_SCOPE_RADIO;
	WndKey = _T("BIW_PROJECT_SCOPE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BIW_PANEL_SCOPE_RADIO;
	WndKey = _T("BIW_PANEL_SCOPE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BIW_BOARD_SCOPE_RADIO;
	WndKey = _T("BIW_BOARD_SCOPE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BIW_INPUT_SEQUENCE_BTN;
	WndKey = _T("BIW_INPUT_SEQUENCE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CBarcodeInputWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_BARCODE_INPUT_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
LANE_ID CBarcodeInputWnd::GetLaneID() const
{
	return m_LaneID;
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::SetLaneID(LANE_ID LaneID)
{
	m_LaneID = LaneID;
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::SetProjectPtr(CAOIProject *ProjectPtr)
{
	m_ProjectPtr = ProjectPtr;
	m_BarcodeHandHeld.SetProjectPtr(ProjectPtr);
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::SetBarcodeReadMode(BARCODE_HANDHELD_READ_MODE ReadMode)
{
	m_BarcodeReadMode = ReadMode;
}
//-------------------------------------------------------------------------------------//
CBarcode_Handheld&  CBarcodeInputWnd::GetBarcodeHandheld()
{
	return m_BarcodeHandHeld;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CBarcodeInputWnd::GetActiveProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::InitialProjectMapWnd()
{
	TPOINT2D   ImageRes;	
	IMAGE_PTR  ImagePtr=NULL;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	TREGION4D  RgnCad, RgnStage;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	

	m_ImageWnd.SetShowLBtnPos(false);
	m_ImageWnd.SetShowCameraRgn(false);
	//m_ImageWnd.SetShowPanel(false);
	//m_ImageWnd.SetShowBoard(false);
	m_ImageWnd.SetShowFiducial(false);
	m_ImageWnd.SetShowComponent(false);
	m_ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_NULL);

	LANE_ID  LaneID = m_ProjectPtr->GetProjectActLaneID();
	unsigned int MapIndex = m_ProjectPtr->GetProjectMapIndex();
	m_ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	m_ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	m_ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	m_ProjectPtr->CreateProjectMapShowPtr(MapIndex, ImagePtr, false);	

	m_ImageWnd.SetProjectPtr(m_ProjectPtr);
	m_ImageWnd.SetImageInfo(CameraID, RgnStage, ImageRes, IMAGE_DATA_MAP);
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, true);
	m_ImageWnd.ShowFittedZoom();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::BuildBarcodeInfoListWnd()
{
	CThisListCtrl_04 &ListCtrl = m_BarcodeListCtrl;
	JetAPI::ClearListCtrl(ListCtrl, NULL);
	
	size_t        i=0;
	CString       str;
	int           nItem=0;
	int           nSubItem=0;
	TBarcodeInfo *BarcodInfoPtr=NULL;
	const size_t  BarcodeInfoCount = m_BarcodeHandHeld.GetBarcodeInfoCount();

	nItem = 0;
	ListCtrl.SetRedraw(FALSE);
	for (i=0; i<BarcodeInfoCount; i++ )
	{
		BarcodInfoPtr = m_BarcodeHandHeld.GetBarcodeInfoPtr(i, false);
		if ( NULL == BarcodInfoPtr ) { continue; }

		nSubItem = 0;
		str.Format(_T("%d"), nItem+1);
		ListCtrl.InsertItem(nItem, str);
		ListCtrl.SetItemData(nItem, i);

		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		//整板引數
		str.Format(_T("%d"), BarcodInfoPtr->nPanelIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		//單板引數
		str.Format(_T("%d"), BarcodInfoPtr->nBoardIndex+1);
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;
		//條碼內容
		str = BarcodInfoPtr->wsBarcode.c_str();
		ListCtrl.SetItemText(nItem, nSubItem, str);
		nSubItem ++;

		nItem ++;
	}
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::BuildBarcodeInfoListWnd_Header()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT	
	CThisListCtrl_04 &ListCtrl = m_BarcodeListCtrl;

	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right-Rect.left-8)/6;
	str = AOIDataDefine.GetIndexText();
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol ++;

	width2 = width;
	str = AOIDataDefine.GetPanelText();
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width;
	str = AOIDataDefine.GetBoardText();
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;

	width2 = width*3;
	str = AOIDataDefine.GetBarcodeText();
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol ++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnProjectScopeRadio() 
{
	// TODO: Add your control notification handler code here
	m_BarcodeScope = BIW_PROJECT_SCOPE_RADIO;
	m_ImageWnd.SetShowPanel(false);
	m_ImageWnd.SetShowBoard(false);
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnPanelScopeRadio() 
{
	// TODO: Add your control notification handler code here
	m_BarcodeScope = BIW_PANEL_SCOPE_RADIO;
	m_ImageWnd.SetShowPanel(true);
	m_ImageWnd.SetShowBoard(false);
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnBoardScopeRadio() 
{
	// TODO: Add your control notification handler code here
	m_BarcodeScope = BIW_BOARD_SCOPE_RADIO;
	m_ImageWnd.SetShowPanel(false);
	m_ImageWnd.SetShowBoard(true);
	m_ImageWnd.RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecInputBarcode(LPCTSTR Caption, LPCTSTR Label, LPCTSTR Default, CString &Value)
{	
	Value = Default;
	CInputBoxWnd InputBox;	
	InputBox.SetParam1(Caption, Label, Default);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }

	CString strValue = InputBox.m_DataEdit1;		
	if ( AOIDataCollect.GetVerifyJsonStringEnabled() == true )
	{
		if ( AOIDataCollect.VerifyJsonString(strValue) == false )
		{			
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return false;
		}
	}
	Value = strValue;
	//Value.MakeUpper();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecBarcodeInput()
{
	bool IsOK = true;
	switch ( m_BarcodeScope )
	{
	case BIW_PROJECT_SCOPE_RADIO:
		IsOK = ExecBarcodeInput_Project();
		break;
	case BIW_PANEL_SCOPE_RADIO:
		IsOK = ExecBarcodeInput_Panel();
		break;
	case BIW_BOARD_SCOPE_RADIO:
		IsOK = ExecBarcodeInput_Board();
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecBarcodeInput_Panel()
{	
	size_t i=0;
	CAOIPanel   *PanelPtr=NULL;
	TREGION4D SelRgn;
	POINT     Pt1, Pt2;		
	TPOINT2D  StagePt1, StagePt2;
	std::vector<CAOIPanel*> PanelList;
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	Pt2 = m_ImageWnd.GetLBtnUpPos();
	Pt1 = m_ImageWnd.GetLBtnDownPos();
	
	StagePt1 = m_ImageWnd.CalcStagePos(Pt1);
	StagePt2 = m_ImageWnd.CalcStagePos(Pt2);
	SelRgn.minX = MIN(StagePt1.x, StagePt2.x);
	SelRgn.minY = MIN(StagePt1.y, StagePt2.y);
	SelRgn.maxX = MAX(StagePt1.x, StagePt2.x);
	SelRgn.maxY = MAX(StagePt1.y, StagePt2.y);
	const bool MultiSelect = AOIDataCollect.CheckMultiSelectMode();

	if ( false == MultiSelect )
	{	ProjectPtr->SelectProjectAllPanels(false); }

	ProjectPtr->SelectProjectPanelsByStage(SelRgn, false, PanelList);

	const size_t PanelCount = PanelList.size();
	if ( 0 == PanelCount ) { return false; }
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = PanelList[i];
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->SetPanelSelected(true);
	}
	m_ImageWnd.RedrawWnd(FALSE);

	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	TBarcodeInfo BarcodeInfo;	

	strCaption = _T("Set Panel Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetBarcodeText();
	strValue = _T("");
	if ( ExecInputBarcode(strCaption, strLabel, strValue, strValue) == false )	
	{	return false;  }
	JetAPI::TCHAR2wstring(strValue, BarcodeInfo.wsBarcode);	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = PanelList[i];
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->SetPanelSelected(false);
		BarcodeInfo.pPanel = PanelPtr;
		BarcodeInfo.nBoardIndex = -1;
		BarcodeInfo.nPanelIndex = PanelPtr->GetPanelIndex_Project();
		m_BarcodeHandHeld.AddBarcodeInfo(BarcodeInfo);
	}
	BuildBarcodeInfoListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecBarcodeInput_Board()
{
	size_t i=0;
	CAOIBoard   *BoardPtr=NULL;
	TREGION4D SelRgn;
	POINT     Pt1, Pt2;		
	TPOINT2D  StagePt1, StagePt2;
	std::vector<CAOIBoard*> BoardList;
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	Pt2 = m_ImageWnd.GetLBtnUpPos();
	Pt1 = m_ImageWnd.GetLBtnDownPos();

	StagePt1 = m_ImageWnd.CalcStagePos(Pt1);
	StagePt2 = m_ImageWnd.CalcStagePos(Pt2);
	SelRgn.minX = MIN(StagePt1.x, StagePt2.x);
	SelRgn.minY = MIN(StagePt1.y, StagePt2.y);
	SelRgn.maxX = MAX(StagePt1.x, StagePt2.x);
	SelRgn.maxY = MAX(StagePt1.y, StagePt2.y);

	const bool MultiSelect = AOIDataCollect.CheckMultiSelectMode();
	if ( false == MultiSelect )
	{	ProjectPtr->SelectProjectAllBoards(false); }

	ProjectPtr->SelectProjectBoardsByStage(SelRgn, false, BoardList);

	const size_t BoardCount = BoardList.size();
	if ( 0 == BoardCount ) { return false; }
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = BoardList[i];
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardSelected(true);
	}
	m_ImageWnd.RedrawWnd(FALSE);
	
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	TBarcodeInfo BarcodeInfo;
	
	strCaption = _T("Set Board Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetBarcodeText();
	strValue = _T("");
	if ( ExecInputBarcode(strCaption, strLabel, strValue, strValue) == false )
	{	return false;  }
	JetAPI::TCHAR2wstring(strValue, BarcodeInfo.wsBarcode);	
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = BoardList[i];
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardSelected(false);
		BarcodeInfo.pBoard = BoardPtr;		
		BarcodeInfo.nBoardIndex = BoardPtr->GetBoardIndex_Project();
		BarcodeInfo.nPanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		m_BarcodeHandHeld.AddBarcodeInfo(BarcodeInfo);		
	}
	BuildBarcodeInfoListWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecBarcodeInput_Project()
{
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	TBarcodeInfo BarcodeInfo;	

	strCaption = _T("Set Project Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetBarcodeText();
	strValue = _T("");
	if ( ExecInputBarcode(strCaption, strLabel, strValue, strValue) == false )
	{	return false;  }
	JetAPI::TCHAR2wstring(strValue, BarcodeInfo.wsBarcode);	
	BarcodeInfo.pPanel = NULL;
	BarcodeInfo.nBoardIndex = -1;
	BarcodeInfo.nPanelIndex = -1;
	m_BarcodeHandHeld.AddBarcodeInfo(BarcodeInfo);	
	BuildBarcodeInfoListWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
LRESULT CBarcodeInputWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_SELF_WND_EXTRA_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_FIRST_UI_CALLBACK:
			ExecBarcodeSequenceInput();
			break;
		}
		break;
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		OnImageWndNotify(wParam, lParam);
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnImageWndNotify(WPARAM wParam, LPARAM lParam)
{	
	switch ( wParam )
	{
	case WPARAM_LBUTTON_DOWN:
		break;
	case WPARAM_LBUTTON_UP:
		ExecBarcodeInput();
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
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecBarcodeSequenceInput()
{
	bool IsOK = true;
	switch ( m_BarcodeReadMode )
	{
	case BARCODE_HANDHELD_READ_PROJECT:
		IsOK = ExecBarcodeSequenceInput_Project();
		break;
	case BARCODE_HANDHELD_READ_PANEL:
		IsOK = ExecBarcodeSequenceInput_Panel();
		break;
	case BARCODE_HANDHELD_READ_BOARD:
		IsOK = ExecBarcodeSequenceInput_Board();
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecBarcodeSequenceInput_Panel()
{	
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	size_t       i=0;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	DWORD        DelayTime=0;
	CAOIPanel   *PanelPtr = NULL;	
	std::wstring wsBarcode=L"";	
	TBarcodeInfo BarcodeInfo;	
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();

	strCaption = _T("Set Panel Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetBarcodeText();	

	ProjectPtr->SelectProjectAllPanels(false);
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->SetPanelSelected(true);
		m_ImageWnd.RedrawWnd(FALSE);		
		strValue = _T("");

		//確認條碼是否重複
		while ( true )
		{
			if ( ExecInputBarcode(strCaption, strLabel, strValue, strValue) == false )
			{	
				PanelPtr->SetPanelSelected(false);
				return false;  
			}
			JetAPI::TCHAR2wstring(strValue, wsBarcode);
			if ( m_BarcodeHandHeld.CheckBarcodeRepeated(wsBarcode.c_str()) == false )
			{	break; }
		};		
		PanelPtr->SetPanelSelected(false);		
		BarcodeInfo.wsBarcode = wsBarcode;
		BarcodeInfo.pPanel = PanelPtr;
		BarcodeInfo.nBoardIndex = -1;
		BarcodeInfo.nPanelIndex = PanelPtr->GetPanelIndex_Project();
		m_BarcodeHandHeld.AddBarcodeInfo(BarcodeInfo);
		BuildBarcodeInfoListWnd();
		
		if ( DelayTime > 0 ) 
		{	::Sleep(DelayTime); }
	}	
	m_ImageWnd.RedrawWnd(FALSE);	
	CBaseDialog::OnOK();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecBarcodeSequenceInput_Board()
{
	CAOIProject *ProjectPtr = GetActiveProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	size_t       i=0;
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	DWORD        DelayTime=0;
	CAOIBoard   *BoardPtr = NULL;	
	std::wstring wsBarcode=L"";	
	TBarcodeInfo BarcodeInfo;	
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();

	strCaption = _T("Set Board Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetBarcodeText();	

	ProjectPtr->SelectProjectAllPanels(false);
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardSelected(true);
		m_ImageWnd.RedrawWnd(FALSE);

		strValue = _T("");
		//確認條碼是否重複
		while ( true )
		{
			if ( ExecInputBarcode(strCaption, strLabel, strValue, strValue) == false )
			{	
				BoardPtr->SetBoardSelected(false);
				return false;  
			}
			JetAPI::TCHAR2wstring(strValue, wsBarcode);
			if ( m_BarcodeHandHeld.CheckBarcodeRepeated(wsBarcode.c_str()) == false )
			{	break; }
		};		
		BoardPtr->SetBoardSelected(false);		
		BarcodeInfo.wsBarcode = wsBarcode;
		BarcodeInfo.pBoard = BoardPtr;		
		BarcodeInfo.nBoardIndex = BoardPtr->GetBoardIndex_Project();
		BarcodeInfo.nPanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		m_BarcodeHandHeld.AddBarcodeInfo(BarcodeInfo);
		BuildBarcodeInfoListWnd();

		if ( DelayTime > 0 ) 
		{	::Sleep(DelayTime); }
	}	
	m_ImageWnd.RedrawWnd(FALSE);	
	CBaseDialog::OnOK();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeInputWnd::ExecBarcodeSequenceInput_Project()
{
	CString      strValue;
	CString      strLabel;
	CString      strCaption;
	std::wstring wsBarcode=L"";
	TBarcodeInfo BarcodeInfo;	

	strCaption = _T("Set Project Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = AOIDataDefine.GetBarcodeText();
	strValue = _T("");
	if ( ExecInputBarcode(strCaption, strLabel, strValue, strValue) == false )
	{	return false;  }
	JetAPI::TCHAR2wstring(strValue, wsBarcode);	
	BarcodeInfo.wsBarcode=wsBarcode;
	BarcodeInfo.pPanel = NULL;
	BarcodeInfo.nBoardIndex = -1;
	BarcodeInfo.nPanelIndex = -1;
	m_BarcodeHandHeld.AddBarcodeInfo(BarcodeInfo);	
	BuildBarcodeInfoListWnd();

	m_ImageWnd.RedrawWnd(FALSE);	
	CBaseDialog::OnOK();
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//CBaseDialog::OnCancel();
	//return;
	CBaseDialog::OnClose();//多執行緒下有蟲
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::EnableCloseBtn(BOOL bEnable)//啟用關閉按鈕
{
	//HWND hWnd = this->GetSafeHwnd();
	//JetAPI::EnableCloseButton(hWnd, FALSE);
	//JetAPI::EnableMaximizeButton(hWnd, FALSE);
	//JetAPI::EnableMinimizeButton(hWnd, TRUE);	
	CMenu *pMenu = GetSystemMenu(FALSE);
	if ( NULL == pMenu ) { return; }
	if ( TRUE == bEnable )
	{	pMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND|MF_ENABLED);}
	else
	{	pMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);	}		
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y =  640;
}
//-------------------------------------------------------------------------------------//
void CBarcodeInputWnd::OnInputSequenceBtn() 
{
	// TODO: Add your control notification handler code here
	switch ( m_BarcodeScope )
	{
	case BIW_PROJECT_SCOPE_RADIO:m_BarcodeReadMode = BARCODE_HANDHELD_READ_PROJECT;	break;
	case BIW_PANEL_SCOPE_RADIO:	m_BarcodeReadMode = BARCODE_HANDHELD_READ_PANEL;	break;		
	case BIW_BOARD_SCOPE_RADIO:	m_BarcodeReadMode = BARCODE_HANDHELD_READ_BOARD;	break;		
	}	
	m_BarcodeHandHeld.ClearBarcodeInfoList();
	ExecBarcodeSequenceInput();
}
//-------------------------------------------------------------------------------------//