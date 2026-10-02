// BarcodeConfirmWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "BarcodeConfirmWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeConfirmWnd dialog
//-------------------------------------------------------------------------------------//
CBarcodeConfirmWnd::CBarcodeConfirmWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CBarcodeConfirmWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeConfirmWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ImageIndex = 0;
	m_PanelIndex = -1;
	m_BoardIndex = -1;
	m_ProjectPtr = NULL;
	m_BarcodePtr = NULL;
	m_ComponentPtr = NULL;
	m_ShowImagePtr = NULL;
	m_ShowBufferSize = 0;
	m_ImageIndexDefault = 0;
	m_BarcodeContext = _T("");
	m_BarcodeUniFrameList.clear();
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeConfirmWnd)
	DDX_Control(pDX, BARCON_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, BARCON_UNI_FRAME_COMBO, m_UniFrameCombox);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CBarcodeConfirmWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CBarcodeConfirmWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(BARCON_UNI_FRAME_COMBO, OnSelchangeUniFrameCombo)
	ON_BN_CLICKED(BARCON_PCB_BACK_BTN, OnPCBBackBtn)
	ON_BN_CLICKED(BARCON_PCB_OUT_BTN, OnPCBOutBtn)	
	ON_BN_CLICKED(BARCON_SHOW_PROJECT_MAP_BTN, OnShowProjectMapBtn)
	ON_BN_CLICKED(BARCON_SAVE_IMAGE_BTN, OnSaveImageBtn)
	ON_BN_CLICKED(BARCON_RECOGNIZE_BTN, OnRecognizeBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeConfirmWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CBarcodeConfirmWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageWnd.SetShowCursorLine(false);	
	if ( NULL != m_ProjectPtr )
	{
		m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);
		m_ProjectMapWnd.SetProjectPtr(m_ProjectPtr, false);
	}
	else
	{	JetAPI::EnableCtrlWnd(this, BARCON_SHOW_PROJECT_MAP_BTN, FALSE); }
	SwitchMultiLanguage();
	BuildUniFrameCombox();
	AdjustCtrlWndPosition();

	if ( NULL == GetActiveBarcode() )
	{	JetAPI::EnableCtrlWnd(this, BARCON_RECOGNIZE_BTN, FALSE);	}
	CWnd::SetDlgItemText(BARCON_BARCODE_EDIT, m_BarcodeContext);
	CreateShowBuffer();	
	UpdateFrameImage();
	EnableCloseBtn(FALSE);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseShowImageBuffer();
	m_BarcodeUniFrameList.clear();
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustCtrlWndPosition();
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnSelchangeUniFrameCombo()
{
	// TODO: Add your control notification handler code here
	m_ImageIndex = JetAPI::GetComboxCurSelData(m_UniFrameCombox);
	UpdateFrameImage();		
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnPCBBackBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	const bool bStep = true;
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	if ( PlcCtrlPtr->ExecPCBBack(LaneID, true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
	AOIDataCollect.SetIsNeedReCheckPCBInside(true);
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnPCBOutBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	const bool bStep = true;
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	if ( PlcCtrlPtr->ExecPCBOut(LaneID, true) == false )
	{	JetAPI::ShowMessageBox(PlcCtrlPtr->GetPLCErrorString());	}
	AOIDataCollect.SetIsNeedReCheckPCBInside(true);
}
//-------------------------------------------------------------------------------------//
LPCTSTR CBarcodeConfirmWnd::GetBarcodeContext() const
{
	return m_BarcodeContext;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::SetBarcodeContext(LPCSTR val)
{
	m_BarcodeContext = val;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::SetImageIndex(unsigned int val)
{
	m_ImageIndex = val;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::SetActiveProject(CAOIProject *Ptr)
{	
	m_ProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::SetActiveBarcodePtr(CAOIBarcode *Ptr)
{
	m_Name = _T("");
	m_PanelIndex = -1;
	m_BoardIndex = -1;
	m_BarcodePtr = Ptr;
	m_ComponentPtr = NULL;
	if ( NULL != Ptr )
	{
		m_PanelIndex = Ptr->GetBarcodePanelIndex_Project();
		m_BoardIndex = Ptr->GetBarcodeBoardIndex_Panel();		
	}
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::SetActiveComponentPtr(CAOIComponent *Ptr)
{
	m_Name = _T("");
	m_PanelIndex = -1;
	m_BoardIndex = -1;
	m_BarcodePtr = NULL;
	m_ComponentPtr = Ptr;
	if ( NULL != Ptr )
	{
		m_Name = Ptr->GetComponentName();
		m_PanelIndex = Ptr->GetComponentPanelIndex_Project();
		m_BoardIndex = Ptr->GetComponentBoardIndex_Panel();		
	}
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::SetUniFrameList(unsigned int FrameIndex, const std::vector<TUNI_FRAME> &UniFrameList)
{	
	const size_t Count = UniFrameList.size();
	if ( FrameIndex >= Count )
	{	m_ImageIndex = 0;	}
	else
	{	m_ImageIndex = FrameIndex; }
	m_ImageIndexDefault = m_ImageIndex;
	m_BarcodeUniFrameList = UniFrameList;
	return true;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CBarcodeConfirmWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
inline CAOIBarcode* CBarcodeConfirmWnd::GetActiveBarcode()
{
	return m_BarcodePtr;
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CBarcodeConfirmWnd::GetActiveComponent()
{
	return m_ComponentPtr;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::BuildUniFrameCombox()
{
	CComboBox &Combox=m_UniFrameCombox;
	const std::vector<TUNI_FRAME> &UniFrameList=m_BarcodeUniFrameList;	
	const size_t UniFrameCount=UniFrameList.size();
	if ( Combox.GetSafeHwnd() == NULL )
	{	return; }

	CString str;
	JetAPI::ClearCombox(Combox);
	for ( size_t i=0; i<UniFrameCount; i++ )
	{
		str.Format(_T("%d"), i+1);		
		if ( m_ImageIndexDefault == i )
		{	str.Format(_T("%d**"), i+1);	}
		Combox.InsertString(-1, str);
		Combox.SetItemData(i, i);		
	}
	if ( m_ImageIndexDefault < UniFrameCount )
	{	Combox.SetCurSel(m_ImageIndexDefault);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::AdjustCtrlWndPosition()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }

	CWnd *WndPtr = NULL;	
	RECT  MainRect={0};
	RECT  ImageWndRect={0};
	const int MarginX = 4;
	const int MarginY = 4;

	CWnd::GetClientRect(&MainRect);
	ImageWndRect = MainRect;

	std::vector<UINT> WndIDList;
	WndIDList.push_back(IDOK);
	WndIDList.push_back(IDCANCEL);
	WndIDList.push_back(BARCON_SHOW_PROJECT_MAP_BTN);
	WndIDList.push_back(BARCON_PCB_OUT_BTN);
	WndIDList.push_back(BARCON_PCB_BACK_BTN);
	WndIDList.push_back(BARCON_SAVE_IMAGE_BTN);
	WndIDList.push_back(BARCON_RECOGNIZE_BTN);
	WndIDList.push_back(BARCON_UNI_FRAME_COMBO);
	const size_t WndIDCount=WndIDList.size();
	for ( size_t i=0; i<WndIDCount; i++ )
	{
		UINT WndID=WndIDList[i];
		WndPtr = CWnd::GetDlgItem(WndID);
		if ( NULL == WndPtr ) { continue; }
		if ( NULL == WndPtr->GetSafeHwnd() ) { continue; }

		SIZE WndSize={0};
		RECT WndRect={0};
		WndPtr->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		JetAPI::GetRectSize(WndRect, WndSize);
		WndRect.right = MainRect.right-MarginX;
		WndRect.left = WndRect.right-WndSize.cx;
		WndPtr->MoveWindow(&WndRect);
		WndRect.left = WndRect.left - MarginX;
		if ( ImageWndRect.right > WndRect.left ) 
		{	ImageWndRect.right = WndRect.left; }
	}

	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		SIZE WndSize={0};
		RECT WndRect={0};
		m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.left = MainRect.left+MarginX;
		WndRect.right = ImageWndRect.right-MarginX;
		WndRect.bottom = MainRect.bottom-+MarginY;
		m_ImageWnd.MoveWindow(&WndRect);
		DrawWndBoxRect();
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString str;	
	CString WndKey;	
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString strPanel=AOIDataDefine.GetPanelText();
	CString strBoard=AOIDataDefine.GetBoardText();
	CString Section=_T("IDD_BARCODE_CONFIRM_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_BARCODE_CONFIRM_WND;
	WndKey = _T("IDD_BARCODE_CONFIRM_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	str = NewLabelText;
	NewLabelText.Format(_T("%s - %s[%d]-%s[%d]"), str, strPanel, m_PanelIndex+1, strBoard, m_BoardIndex+1);
	if ( m_Name.GetLength() != 0 )
	{
		LabelText = NewLabelText;		
		NewLabelText.Format(_T("%s-[%s]"), LabelText, m_Name);
	}
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = BARCON_BARCODE_LABEL;
	WndKey = _T("BARCON_BARCODE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCON_PCB_OUT_BTN;
	WndKey = _T("BARCON_PCB_OUT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCON_PCB_BACK_BTN;
	WndKey = _T("BARCON_PCB_BACK_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = BARCON_SHOW_PROJECT_MAP_BTN;
	WndKey = _T("BARCON_SHOW_PROJECT_MAP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = BARCON_SAVE_IMAGE_BTN;
	WndKey = _T("BARCON_SAVE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = BARCON_RECOGNIZE_BTN;
	WndKey = _T("BARCON_RECOGNIZE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CBarcodeConfirmWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_BARCODE_CONFIRM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::CreateShowBuffer()
{
	const char fnName[] = "CBarcodeConfirmWnd::CreateShowBuffer";
	ReleaseShowImageBuffer();
	const size_t UniFrameCount = m_BarcodeUniFrameList.size();
	if ( 0 == UniFrameCount ) { return false; }

	IMAGE_SIZE ImageW = m_BarcodeUniFrameList[0].ImageW;
	IMAGE_SIZE ImageH = m_BarcodeUniFrameList[0].ImageH;	
	IMAGE_PTR  Ptr = NULL;
	IMAGE_SIZE BitCount = 24;//直接開最大的
	IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "ShowBuffer") == false )
	{	return false; }
	::memset(Ptr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
	m_ShowImagePtr = Ptr;	
	m_ShowBufferSize = BufferSize;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::ReleaseShowImageBuffer()
{
	JetMemory.free_func(m_ShowImagePtr);	 
	m_ShowBufferSize = 0;;
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::RedrawWnd()
{
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::DrawWndBoxRect()
{
	CAOIComponent *ComponentPtr = GetActiveComponent();
	if ( NULL == ComponentPtr ) { return; }	
	CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return; }		
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }	
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	if ( ALG_BARCODE_RECOGNIZE != AlgType ) { return; }
	
	RECT WndRect;
	WndPtr->GetWndImageRect(WndRect);
	m_ImageWnd.SetShowEditLine(true);
	m_ImageWnd.SetImageEditRect(WndRect);					
	m_ImageWnd.RedrawWnd(FALSE);			
	return;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::UpdateFrameImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return false;	}		
	const size_t UniFrameCount = m_BarcodeUniFrameList.size();
	if ( 0 == UniFrameCount ) { return false; }
	//unsigned int ImageIndex = ProjectPtr->GetProjectMapIndex();		
	unsigned int ImageIndex = m_ImageIndex;	
	if ( ImageIndex >= UniFrameCount ) 
	{	ImageIndex = 0; }
	TUNI_FRAME UniFrame = m_BarcodeUniFrameList[ImageIndex];
	const IMAGE_SIZE ImageW = UniFrame.ImageW;
	const IMAGE_SIZE ImageH = UniFrame.ImageH;
	const IMAGE_SIZE ImageStep = UniFrame.ImageStep;
	const IMAGE_SIZE BitCount = UniFrame.BitCount;
	IMAGE_PTR ImagePtr = UniFrame.ImagePtr;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( BufferSize>m_ShowBufferSize || NULL==ImagePtr )
	{	return false;	}
	AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, m_ShowImagePtr);
	m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, m_ShowImagePtr, false, true);
	m_ImageWnd.ShowFittedZoom();	
	DrawWndBoxRect();
	return true;
	
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnOK() 
{
	// TODO: Add extra validation here
	CWnd::GetDlgItemText(BARCON_BARCODE_EDIT, m_BarcodeContext);	
	if ( NULL != m_ComponentPtr )
	{
		if ( CheckBarcodeContextValid(m_BarcodeContext) == false )
		{	return; }
	}
	if ( AOIDataCollect.GetVerifyJsonStringEnabled() == true )
	{
		if ( AOIDataCollect.VerifyJsonString(m_BarcodeContext) == false )
		{			
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return ;
		}
	}
	//m_BarcodeContext.MakeUpper();
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::EnableCloseBtn(BOOL bEnable)//啟用關閉按鈕
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
void CBarcodeConfirmWnd::OnShowProjectMapBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_ProjectMapWnd.GetSafeHwnd() == NULL ) { return; }
	BOOL bVisible = m_ProjectMapWnd.IsWindowVisible();	
	if ( FALSE == bVisible )
	{	m_ProjectMapWnd.ShowWindow(SW_SHOW);	}
	else
	{	m_ProjectMapWnd.ShowWindow(SW_HIDE);	}
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::ExecSaveImageBtn(LPCTSTR  filename)
{
	//return ExecSaveImageBtn_v1(filename);
	return ExecSaveImageBtn_v2(filename);
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::ExecSaveImageBtn_v1(LPCTSTR filename)
{
	if ( ExecSaveImageBtnFn(m_ImageIndex, filename) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::ExecSaveImageBtn_v2(LPCTSTR filename)
{
	if ( NULL == filename ) { return false; }
	const size_t UniFrameCount = m_BarcodeUniFrameList.size();	
	if ( 0 == UniFrameCount ) { return false; }

	CString MainName, ExtName, Filename, Error;	
	JetAPI::ExtractMainFileName(filename, MainName);
	JetAPI::ExtractExtendFileName(filename, ExtName);	
	::CreateDirectory(MainName, NULL);
	for ( size_t i=0; i<UniFrameCount; i++ )
	{
		Filename.Format(_T("%s\\Image#%02d.%s"), MainName, i+1, ExtName);
		if ( ExecSaveImageBtnFn((unsigned int)(i), Filename) == false )
		{	continue; }
	}

	CString Section, Key, String;
	unsigned int FrameUniqueID = 0;
	unsigned int FrameUniqueID_Max = 0;	
	std::vector<unsigned int> FrameUniqueIDList;
	Filename.Format(_T("%s\\ImageInfo.INI"), MainName);
	Section = _T("Basic");	Key = _T("Count"); String.Format(_T("%d"), UniFrameCount);
	JetAPI::SaveINIData(Section, Key, String, Filename, Error);

	for ( size_t i=0; i<UniFrameCount; i++ )
	{
		TUNI_FRAME UniFrame=m_BarcodeUniFrameList[i];
		Section.Format(_T("Frame %02d"), i+1);
		FrameUniqueID = UniFrame.FrameUniqueID;
		if ( FrameUniqueID_Max < FrameUniqueID )
		{	FrameUniqueID_Max = FrameUniqueID;	}

		FrameUniqueIDList.push_back(FrameUniqueID);
		Key = _T("UniqueID"); String.Format(_T("%d"), FrameUniqueID);
		JetAPI::SaveINIData(Section, Key, String, Filename, Error);

		Key = _T("Width"); String.Format(_T("%d"), UniFrame.ImageW);
		JetAPI::SaveINIData(Section, Key, String, Filename, Error);

		Key = _T("Height"); String.Format(_T("%d"), UniFrame.ImageH);
		JetAPI::SaveINIData(Section, Key, String, Filename, Error);

		Key = _T("Step"); String.Format(_T("%d"), UniFrame.ImageStep);
		JetAPI::SaveINIData(Section, Key, String, Filename, Error);

		Key = _T("BitCount"); String.Format(_T("%d"), UniFrame.BitCount);
		JetAPI::SaveINIData(Section, Key, String, Filename, Error);
		
		Key = _T("Filename"); String.Format(_T("Image#%02d.%s"), i+1, ExtName);
		JetAPI::SaveINIData(Section, Key, String, Filename, Error);
	}
	
	std::vector<unsigned int> FrameIndexMapList;
	for ( size_t i=0; i<FrameUniqueID_Max+1; i++ )
	{	FrameIndexMapList.push_back(-1);	}
	for ( size_t i=0; i<UniFrameCount; i++ )
	{	FrameIndexMapList[FrameUniqueIDList[i]] = i;	}

	bool IsOK=true;	
	CAOIBarcode *BarcodePtr=GetActiveBarcode();
	if ( NULL != BarcodePtr )
	{	
		CAOIFileIO FileIO;
		Filename.Format(_T("%s\\Barcode.%s"), MainName, _T("PRG"));
		FileIO.OpenSaveFile(Filename);
		IsOK = BarcodePtr->WriteBarcodeFile(FileIO);
		FileIO.CloseFile();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::ExecSaveImageBtnFn(unsigned int Index, LPCTSTR filename)
{
	if ( NULL == filename ) { return false; }
	const unsigned int ImageIndex = Index;	
	const size_t UniFrameCount = m_BarcodeUniFrameList.size();	
	if ( 0 == UniFrameCount ) { return false; }		
	if ( ImageIndex >= UniFrameCount ) { return false; }	
	TUNI_FRAME UniFrame = m_BarcodeUniFrameList[ImageIndex];
	const IMAGE_SIZE ImageW = UniFrame.ImageW;
	const IMAGE_SIZE ImageH = UniFrame.ImageH;
	const IMAGE_SIZE ImageStep = UniFrame.ImageStep;
	const IMAGE_SIZE BitCount = UniFrame.BitCount;
	IMAGE_PTR ImagePtr = UniFrame.ImagePtr;	
	if ( ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnSaveImageBtn() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	CString str;
	CString filename = dialog.GetPathName();
	if ( ExecSaveImageBtn(filename) == false ) 
	{
		str.Format(_T("Error, Save File Fault [%s]"), filename);
		JetAPI::ShowMessageBox(str);
		return;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::ExecBarcodeRecognize()
{
	CAOIBarcode *BarcodePtr=GetActiveBarcode();	
	if ( ExecBarcodeRecognize(BarcodePtr) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::ExecBarcodeRecognize(CAOIBarcode *BarcodePtr)
{
	CString str;	
	if ( NULL == BarcodePtr ) { return false; }
	CAOIModel *ModelPtr=BarcodePtr->GetBarcodeModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	ModelPtr->InitModelInspection();
	if ( ModelPtr->ExecModelInspection(m_BarcodeUniFrameList) == false )
	{
		str = _T("Error, Barcode ExecModelInspection Fault");
		JetAPI::ShowMessageBox(str);
		return false;
	}	
	BarcodePtr->UpdateBarcodeResultID();
	RESULT_ID ResultID = BarcodePtr->GetBarcodeResultID_AOI();
	if ( RESULT_ID_NG==ResultID || RESULT_ID_EXCEPTION==ResultID )
	{	str = _T("Error, Barcode Recognize Fault");	}
	else
	{	
		str = BarcodePtr->GetBarcodeResultText();
		CWnd::SetDlgItemText(BARCON_BARCODE_EDIT, str);
	}
	JetAPI::ShowMessageBox(str);
	return true;
}
//-------------------------------------------------------------------------------------//
void CBarcodeConfirmWnd::OnRecognizeBtn() 
{	
	// TODO: Add your control notification handler code here
	ExecBarcodeRecognize();
}
//-------------------------------------------------------------------------------------//
bool CBarcodeConfirmWnd::CheckBarcodeContextValid(LPCTSTR Code)
{
	if ( NULL == Code )
	{	return false; }
	const size_t Len=_tcslen(Code);
	if ( 0 == Len )
	{	return false; }
	CString Text=Code;
	Text.Trim(_T(' '));
	const int Len2=Text.GetLength();
	if ( 0 == Len2 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//