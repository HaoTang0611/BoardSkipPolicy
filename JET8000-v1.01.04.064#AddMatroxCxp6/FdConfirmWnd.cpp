// FdConfirmWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "FdConfirmWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdConfirmWnd dialog
//-------------------------------------------------------------------------------------//
CFdConfirmWnd::CFdConfirmWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CFdConfirmWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFdConfirmWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_FdPtr = NULL;
	m_FdWndPtr = NULL;
	m_ProjectPtr = NULL;
	m_FrameIndex = 0;
	m_BufferPtr = NULL;
	m_BufferSize = 0;	
	m_FdScore = 0;
	m_FdModified = false;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFdConfirmWnd)
	DDX_Control(pDX, FDCFM_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, FDCFM_PATTERN_WND, m_PatternWnd);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CFdConfirmWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CFdConfirmWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_BN_CLICKED(FDCFM_SHOW_PROJECT_MAP_BTN, OnShowProjectMapBtn)
	ON_BN_CLICKED(FDCFM_PATTERN_ADD_BTN, OnPatternAddBtn)
	ON_BN_CLICKED(FDCFM_PARAM_SET_BTN, OnParamSetBtn)
	ON_BN_CLICKED(FDCFM_SAVE_IMAGE_BTN, OnSaveImageBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdConfirmWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CFdConfirmWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str;
	m_ImageWnd.SetRBtnUpMode(IMAGE_RBTN_UP_NULL);//右鍵放開功能
	m_ImageWnd.SetRBtnClickMode(IMAGE_RBTN_CLICK_CTRL_VIEW);//設定右鍵控制模式
	m_ImageWnd.SetLBtnClickMode(IMAGE_LBTN_CLICK_EDIT_BOX);//設定左鍵控制模式	
	m_ImageWnd.SetRBtnDbClickMode(IMAGE_RBTN_DBCLICK_NULL);//設定右鍵雙擊模式	
	m_ImageWnd.SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_NULL);//設定左鍵雙擊模式	

	m_PatternWnd.SetRBtnDbClickMode(IMAGE_RBTN_DBCLICK_NULL);//設定右鍵雙擊模式	
	m_PatternWnd.SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_NULL);//設定左鍵雙擊模式		
	
	if ( NULL != m_ProjectPtr )
	{
		m_ProjectMapWnd.Create(IDD_PROJECT_MAP_WND, this);
		m_ProjectMapWnd.SetProjectPtr(m_ProjectPtr, false);
	}
	else
	{	JetAPI::EnableCtrlWnd(this, FDCFM_SHOW_PROJECT_MAP_BTN, FALSE); }

	str.Format(_T("%.2f"), m_FdScore);
	CWnd::SetDlgItemText(FDCFM_SCORE_EDIT, str);
	CWnd::SetDlgItemText(FDCFM_INFO_EDIT, m_FdString);
	SwitchMultiLanguage();
	SwitchFdUniFrameList(m_FrameIndex);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseBuffer();
	JetAPI::ClearUniFrameList(m_PatternFrameList);
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
}
//-------------------------------------------------------------------------------------//
BOOL CFdConfirmWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CFdConfirmWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch ( message )
	{
	case MSG_IMAGE_WND_NOTIFY_EVENT:
		OnImageWndNotify(wParam, lParam);		
		break;
	}
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CFdConfirmWnd::OnImageWndNotify(WPARAM wParam, LPARAM lParam)
{
	switch ( wParam )
	{
	case WPARAM_LBUTTON_DOWN:
		break;
	case WPARAM_LBUTTON_UP:		
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
	case WPARAM_CONTEXT_MENU:
		SwitchFdUniFrameList();
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void  CFdConfirmWnd::ReleaseBuffer()
{
	if ( NULL != m_BufferPtr ) 
	{	JetMemory.free_func(m_BufferPtr);	}
	m_BufferSize = 0;
	m_BufferPtr = NULL;	
	return;
}
//-------------------------------------------------------------------------------------//
bool CFdConfirmWnd::GetFdModified() const
{
	return m_FdModified;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::SetFdModified(double val)
{
	m_FdModified = val;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::SetFdPtr(CAOIFd *FdPtr)
{	
	m_FdPtr = FdPtr;
	m_FdWndPtr = NULL;
	m_FdScore = 0.0;
	m_FdString = _T("");
	m_ProjectPtr = NULL;
	m_FrameIndex = 0;
	m_FdSize = TSIZE2D();
	m_FdTeachPos = TPOINT2D();
	m_FdStagePos = TPOINT2D();	
	m_FdImageOffset = TPOINT2D();	
	m_UniFrameList.clear();
	ReleaseBuffer();
	JetAPI::ClearUniFrameList(m_PatternFrameList);
	if ( NULL == FdPtr ) { return; }	
	m_ProjectPtr = FdPtr->GetFdProjectPtr();

	int        ImageIndex=0;
	size_t     i=0, j=0;
	CString    str;
	CString    str2;
	CString    PatFile;
	CString    PatFolder;
	CString    PatFullFile;
	size_t     PatCount=0;
	CAOIWnd   *WndPtr = NULL;
	BOX_TOWARD WndToward=BOX_TOWARD_NULL;
	CAOIModel *ModelPtr = FdPtr->GetFdModelPtr();
	const int    nAlign = 4;
	const LANE_ID LaneID = FdPtr->GetFdLaneID();
	double FdTeachPosX = FdPtr->GetFdTeachStagePosX();
	double FdTeachPosY = FdPtr->GetFdTeachStagePosY();
	const double FdStagePosX = FdPtr->GetFdStagePosX();
	const double FdStagePosY = FdPtr->GetFdStagePosY();	
	const size_t WndCount = ModelPtr->GetModelWndCount();	

	IMAGE_PTR    ImagePtr=NULL;
	IMAGE_SIZE   ImageW=0;
	IMAGE_SIZE   ImageH=0;
	IMAGE_SIZE   ImageStep=0;
	IMAGE_SIZE   BitCount=0;
	TUNI_FRAME   PatUniFrame;

	FdPtr->GetRgnUniFrameList(m_UniFrameList);		
	AOIDataCollect.MapStagePosLaneByLaneID(FdTeachPosX, FdTeachPosY, LaneID);	
	m_FdTeachPos.x = FdTeachPosX;
	m_FdTeachPos.y = FdTeachPosY;
	m_FdStagePos.x = FdStagePosX;
	m_FdStagePos.y = FdStagePosY;
	m_FdImageRes.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	m_FdImageRes.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);	

	WndPtr = FdPtr->GetFdWndPtr();
	if ( NULL == WndPtr )
	{	return; }
	
	ImageIndex=0;
	m_FdWndPtr = WndPtr;
	WndToward = WndPtr->GetWndToward();		
	m_FdSize.cx = WndPtr->GetWndBox().GetBoxSizeX();
	m_FdSize.cy = WndPtr->GetWndBox().GetBoxSizeY();

	const CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	m_FdImageOffset.x = AlgParam.GetAlgImageOffsetX();
	m_FdImageOffset.y = AlgParam.GetAlgImageOffsetY();
	//m_FdImageOffset.y += 100;
	m_FdImageRect4d.left   = m_FdImageOffset.x - (m_FdSize.cx*0.5/m_FdImageRes.x);
	m_FdImageRect4d.right  = m_FdImageOffset.x + (m_FdSize.cx*0.5/m_FdImageRes.x);
	m_FdImageRect4d.top    = m_FdImageOffset.y - (m_FdSize.cy*0.5/m_FdImageRes.y);
	m_FdImageRect4d.bottom = m_FdImageOffset.y + (m_FdSize.cy*0.5/m_FdImageRes.y);		
		
	str = AlgParam.GetAlgResultText();
	if ( m_FdString.GetLength() == 0 ) 
	{	m_FdString = str;	}
	else
	{
		str2 = m_FdString;
		m_FdString.Format(_T("%s\\%s"), str2, str);
	}
	m_FdScore = AlgParam.GetAlgPatternSimilarityLSL();
	//continue;
	PatFolder = AlgParam.GetAlgPatternFolder();
	PatCount = AlgParam.GetAlgPatternCount();
	m_FrameIndex = AlgParam.GetAlgImageBinParam().GetBinaryFrameIndex();
	for ( j=0; j<PatCount; j++  )
	{
		PatFile = AOIDataDefine.GetAlgPatternName(ImageIndex, WndToward);
		PatFullFile.Format(_T("%s\\%s"), PatFolder, PatFile);
		if ( ImageAPI.LoadImage(PatFullFile, PatUniFrame.ImageW, PatUniFrame.ImageH, PatUniFrame.ImageStep, PatUniFrame.BitCount, PatUniFrame.ImagePtr, nAlign, true) == true ) 
		{	m_PatternFrameList.push_back(PatUniFrame);	}
		PatUniFrame = TUNI_FRAME();
		ImageIndex ++;
	}
	return;
}
//-------------------------------------------------------------------------------------//
CAOIFd* CFdConfirmWnd::GetFdPtr()
{
	return m_FdPtr;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CFdConfirmWnd::GetFdWndPtr()
{
	return m_FdWndPtr;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString str;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_FD_CONFIRM_WND");	
	CAOIFd  *FdPtr = GetFdPtr();
	CString  strFd = AOIDataDefine.GetFdText();
	CString  strPanel = AOIDataDefine.GetPanelText();
	CString  strBoard = AOIDataDefine.GetBoardText();
	//---------------------------------------------------------------------------------//
	WndID = IDD_FD_CONFIRM_WND;
	WndKey = _T("IDD_FD_CONFIRM_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	if ( NULL != FdPtr )
	{	
		str = NewLabelText;
		CAOIBoard *BoardPtr = FdPtr->GetFdBoardPtr();
		unsigned int FdIndex = FdPtr->GetFdIndex_Project();
		unsigned int PanelIndex = FdPtr->GetFdPanelIndex_Project();
		unsigned int BoardIndex = FdPtr->GetFdBoardIndex_Panel();	
		if ( NULL == BoardPtr )
		{
			FdIndex = FdPtr->GetFdIndex_Project();
			FdIndex = FdPtr->GetFdIndex_Panel();
			NewLabelText.Format(_T("%s %s[%d]-%s[%d]"), str, strPanel, PanelIndex+1, strFd, FdIndex+1);
		}
		else
		{
			FdIndex = FdPtr->GetFdIndex_Project();
			FdIndex = FdPtr->GetFdIndex_Board();
			BoardIndex = FdPtr->GetFdBoardIndex_Panel();	
			NewLabelText.Format(_T("%s %s[%d]-%s[%d]-%s[%d]"), str, strPanel, PanelIndex+1, strBoard, BoardIndex+1, strFd, FdIndex+1);
		}		
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
	WndID = FDCFM_PATTERN_LABEL;
	WndKey = _T("FDCFM_PATTERN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = FDCFM_PATTERN_ADD_BTN;
	WndKey = _T("FDCFM_PATTERN_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = FDCFM_SCORE_LABEL;
	WndKey = _T("FDCFM_SCORE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDCFM_PARAM_SET_BTN;
	WndKey = _T("FDCFM_PARAM_SET_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = FDCFM_SHOW_PROJECT_MAP_BTN;
	WndKey = _T("FDCFM_SHOW_PROJECT_MAP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = FDCFM_SAVE_IMAGE_BTN;
	WndKey = _T("FDCFM_SAVE_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CString CFdConfirmWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_FD_CONFIRM_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CFdConfirmWnd::SwitchFdUniFrameList()
{
	unsigned int NexFrameIndex=m_FrameIndex+1;
	const size_t UniFrameCount = m_UniFrameList.size();
	if ( NexFrameIndex >= UniFrameCount ) { NexFrameIndex = 0; }
	
	TUNI_FRAME UniFrame = m_UniFrameList[NexFrameIndex];
	IMAGE_PTR  ImagePtr=UniFrame.ImagePtr;
	if ( NULL == ImagePtr ) { NexFrameIndex = 0; }
	m_FrameIndex = NexFrameIndex;
	
	if ( SwitchFdUniFrameList(m_FrameIndex) == false )
	{	return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CFdConfirmWnd::SwitchFdUniFrameList(unsigned int FrameIndex)
{	
	CString str;
	const char fnName[]="CFdConfirmWnd::SwitchFdUniFrameList";
	unsigned int FrameIndex2 = FrameIndex;
	const size_t UniFrameCount = m_UniFrameList.size();
	const bool bResetView=false;
	if ( UniFrameCount > 0 )
	{	
		FrameIndex2 = FrameIndex;
		if ( FrameIndex >= UniFrameCount ) { FrameIndex2 = 0; }
		TUNI_FRAME UniFrame = m_UniFrameList[FrameIndex2];
		IMAGE_PTR  ImagePtr=UniFrame.ImagePtr;
		if ( NULL != ImagePtr )
		{
			TRECT4D FdRect4d;
			IMAGE_SIZE ImageW=UniFrame.ImageW;
			IMAGE_SIZE ImageH=UniFrame.ImageH;
			IMAGE_SIZE ImageStep=UniFrame.ImageStep;
			IMAGE_SIZE BitCount=UniFrame.BitCount;	
			const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
			const int ImageCpX = (int)(ImageW/2);
			const int ImageCpY = (int)(ImageH/2);			
			FdRect4d.left  = ImageCpX+m_FdImageRect4d.left;
			FdRect4d.top = ImageCpY-m_FdImageRect4d.bottom;
			FdRect4d.right = ImageCpX+m_FdImageRect4d.right;
			FdRect4d.bottom = ImageCpY-m_FdImageRect4d.top;			
			if ( ImageSize > m_BufferSize )
			{
				IMAGE_PTR Ptr=NULL;
				ReleaseBuffer();
				if ( JetMemory.alloc_func(ImageSize, Ptr, "CFdConfirmWnd::SwitchFdUniFrameList", "m_BufferPtr") == false )
				{	return false; }
				m_BufferPtr = Ptr;
				m_BufferSize = ImageSize;
			}
			AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, m_BufferPtr);

		#ifdef _DEBUG
			str.Format(_T("%s\\FdConfirmImage.PNG"), AOIDataCollect.GetAOITempDirectory());
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, m_BufferPtr, true);
		#endif //_DEBUG			
			m_ImageWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, m_BufferPtr, true, bResetView, true);			
			m_ImageWnd.SetImageEditRect(FdRect4d);
			m_ImageWnd.RedrawWnd(FALSE);
		}
	}

	const size_t PatUniFrameCount = m_PatternFrameList.size();
	if ( PatUniFrameCount > 0 )
	{	
		FrameIndex2 = FrameIndex;
		if ( FrameIndex >= PatUniFrameCount ) { FrameIndex2 = 0; }
		TUNI_FRAME UniFrame = m_PatternFrameList[FrameIndex2];
		IMAGE_PTR  ImagePtr=UniFrame.ImagePtr;
		if ( NULL != ImagePtr )
		{
			IMAGE_SIZE ImageW=UniFrame.ImageW;
			IMAGE_SIZE ImageH=UniFrame.ImageH;
			IMAGE_SIZE ImageStep=UniFrame.ImageStep;
			IMAGE_SIZE BitCount=UniFrame.BitCount;	
			IMAGE_PTR  TempPtr=NULL;
			const size_t TempSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
			if ( JetMemory.alloc_func(TempSize, TempPtr, fnName, "TempPtr") == false )
			{	m_PatternWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, true, bResetView, true); }
			else
			{	
				AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, TempPtr);
				m_PatternWnd.SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, TempPtr, true, bResetView, true);
				JetMemory.free_func(TempPtr);
			}
			m_PatternWnd.ShowFittedZoom(false);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::RedrawWnd()
{
	DrawRoiImage();
	DrawPatternImage();
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::DrawRoiImage()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }	
	
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::DrawPatternImage()
{
	if ( m_PatternWnd.GetSafeHwnd() == NULL ) { return; }
	CAOIFd *FdPtr = GetFdPtr();
	if ( NULL == FdPtr ) { return ; }
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::DrawRoiImage(HDC hDC, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	return;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::DrawPatternImage(HDC hDC, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	return;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnOK() 
{
	// TODO: Add extra validation here
	CAOIFd *FdPtr = GetFdPtr();	
//	SetFdPtr(FdPtr);
//	SwitchFdUniFrameList(m_FrameIndex);
	
	TRECT4D FdImageRect4d;
	unsigned int FrameIndex = m_FrameIndex;
	const size_t UniFrameCount = m_UniFrameList.size();	
	if ( UniFrameCount > 0 )
	{	
		if ( FrameIndex >= UniFrameCount ) { FrameIndex = 0; }
		TRECT4D RoiRect4d;
		TPOINT2D StageOffset, CadOffset, ImageOffset;
		TUNI_FRAME UniFrame = m_UniFrameList[FrameIndex];		
		IMAGE_SIZE ImageW=UniFrame.ImageW;
		IMAGE_SIZE ImageH=UniFrame.ImageH;
		IMAGE_SIZE ImageStep=UniFrame.ImageStep;
		IMAGE_SIZE BitCount=UniFrame.BitCount;	
		const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		const int ImageCpX = (int)(ImageW/2);
		const int ImageCpY = (int)(ImageH/2);
		m_ImageWnd.GetImageEditRect(RoiRect4d);
		FdImageRect4d.left  = RoiRect4d.left-ImageCpX;
		FdImageRect4d.top  = ImageCpY-RoiRect4d.bottom;
		FdImageRect4d.right = RoiRect4d.right-ImageCpX;
		FdImageRect4d.bottom  = ImageCpY-RoiRect4d.top;

		ImageOffset.x = FdImageRect4d.GetCpX();
		ImageOffset.y = FdImageRect4d.GetCpY();
		CadOffset.x = ImageOffset.x*m_FdImageRes.x;
		CadOffset.y = ImageOffset.y*m_FdImageRes.y;
		AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
		
		const LANE_ID LaneID = FdPtr->GetFdLaneID();
		double FdTeachPosX = FdPtr->GetFdTeachStagePosX();
		double FdTeachPosY = FdPtr->GetFdTeachStagePosY();
		AOIDataCollect.MapStagePosLaneByLaneID(FdTeachPosX, FdTeachPosY, LaneID);	
		const double FdStagePosX = FdTeachPosX+StageOffset.x;
		const double FdStagePosY = FdTeachPosY+StageOffset.y;	
		const double FdStagePosX2 = FdPtr->GetFdStagePosX();
		const double FdStagePosY2 = FdPtr->GetFdStagePosY();
		FdPtr->SetFdStagePosX(FdStagePosX);
		FdPtr->SetFdStagePosY(FdStagePosY);		

		CAOIWnd *WndPtr = FdPtr->GetFdWndPtr();
		if ( NULL != WndPtr )
		{
			CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
			AlgParam.SetAlgImageOffsetX(ImageOffset.x);
			AlgParam.SetAlgImageOffsetY(ImageOffset.y);
		}
		CadOffset = CadOffset;
	}	
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnShowProjectMapBtn() 
{
	// TODO: Add your control notification handler code here
	if ( m_ProjectMapWnd.GetSafeHwnd() == NULL ) { return ; }

	BOOL bVisible = m_ProjectMapWnd.IsWindowVisible();	
	if ( TRUE == bVisible )
	{	m_ProjectMapWnd.ShowWindow(SW_HIDE);	}
	else
	{	m_ProjectMapWnd.ShowWindow(SW_SHOW);	}
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnPatternAddBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIFd  *FdPtr = GetFdPtr();
	if ( NULL == FdPtr )	{	return ; }
	CAOIModel *ModelPtr = FdPtr->GetFdModelPtr();
	if ( NULL == ModelPtr )	{	return ; }
	CAOIWnd *WndPtr = GetFdWndPtr();
	if ( NULL == WndPtr )	{	return ; }
	const size_t UniFrameCount = m_UniFrameList.size();	
	if ( 0 == UniFrameCount )	{	return ; }
	unsigned int FrameIndex = m_FrameIndex;
	if ( FrameIndex<0 || FrameIndex>=UniFrameCount ) { return; }

	CString    str;
	int        PolarityIdx = 0;	
	CString    Folder = AOIDataCollect.GetAOITempDirectory();
	TUNI_FRAME UniFrame = m_UniFrameList[FrameIndex];
	BOX_TOWARD WndToward=WndPtr->GetWndToward();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString ModelFolder = ModelPtr->GetModelFolder();
	CAlgBinaryParam BinaryParam = (AlgParam.GetAlgImageBinParam());	
	
	RECT    RoiRect={0};
	TRECT4D RoiRect4d;
	IMAGE_SIZE ImageW=UniFrame.ImageW;
	IMAGE_SIZE ImageH=UniFrame.ImageH;
	IMAGE_SIZE ImageStep=UniFrame.ImageStep;
	IMAGE_SIZE BitCount=UniFrame.BitCount;	
	IMAGE_PTR  ImagePtr=UniFrame.ImagePtr;
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const int ImageCpX = (int)(ImageW/2);
	const int ImageCpY = (int)(ImageH/2);
	m_ImageWnd.GetImageEditRect(RoiRect4d);

	const int  nAlign=4;
	JetAPI::Rect4DToRect(RoiRect4d, RoiRect);
	IMAGE_PTR  RoiPtr=NULL;
	IMAGE_SIZE RoiW=RoiRect.right-RoiRect.left;
	IMAGE_SIZE RoiH=RoiRect.bottom-RoiRect.top;
	IMAGE_SIZE RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
	const size_t RoiSize=ImageAPI.CalcBufferSize(RoiStep, RoiH);
	const char fnName[]="CFdConfirmWnd::OnPatternAddBtn";
#ifdef _DEBUG	
	str.Format(_T("%s\\%s"), Folder, _T("AddFdPatternImage.PNG"));
	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
#endif//_DEBUG
	if ( JetMemory.alloc_func(RoiSize, RoiPtr, fnName, "RoiPtr") == false )
	{
		str = JetMemory.GetErrorString();
		return;
	}
	if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr, false) == false )
	{
		str = ImageAPI.GetImageApiErrorString();
		JetAPI::ShowMessageBox(str);
		JetMemory.free_func(RoiPtr);
		return;
	}
#ifdef _DEBUG	
	str.Format(_T("%s\\%s"), Folder, _T("AddFdPatternRoi.PNG"));
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
#endif//_DEBUG

	if ( AlgParam.AddAlgPatternImage(RoiW, RoiH, RoiStep, BitCount, RoiPtr, ModelFolder, WndToward, PolarityIdx, BinaryParam) == false )
	{
		str = _T("Error, Add Pattern Image Fault");
		JetAPI::ShowMessageBox(str);
		JetMemory.free_func(RoiPtr);
		return ;
	}
	JetMemory.free_func(RoiPtr);	
	ModelPtr->ApplyModelWnd(WndPtr);
	LogOperCtrl.SaveLogModelWndAlgPatternContentAdd(WndPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnParamSetBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIWnd *WndPtr = GetFdWndPtr();
	if ( NULL == WndPtr )	{	return ; }

	double  fScore=0.0;
	CString strScore;
	const double fScoreLSL = WndPtr->GetWndAlgParam().GetAlgPatternSimilarityLSL();
	const double fScoreUSL = WndPtr->GetWndAlgParam().GetAlgPatternSimilarityUSL();

	CWnd::GetDlgItemText(FDCFM_SCORE_EDIT, strScore);

	fScore = ::_ttof(strScore);
	if ( fScore<0.0 || fScore>fScoreUSL )
	{	return ; }
	WndPtr->GetWndAlgParam().SetAlgPatternSimilarityLSL(fScore);
	SetFdModified(true);
	return;
}
//-------------------------------------------------------------------------------------//
bool CFdConfirmWnd::ExecSaveImageBtn(LPCTSTR filename)
{
	if ( NULL == filename ) { return false; }

	CString ExtName;
	CString MainName;
	CString filename_Roi;
	CString filename_Pat;

	if ( JetAPI::ExtractMainFileName(filename, MainName) == false )
	{	return false; }
	if ( JetAPI::ExtractExtendFileName(filename, ExtName) == false )
	{	return false; }

	const bool bReverse= true;
	const bool bSave3D = false;
	const bool bEnhance = false;
	const bool bAppend = false;
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();

	JetAPI::CreateFolder(MainName);
	filename_Roi.Format(_T("%s\\Roi.%s"), MainName, ExtName);
	filename_Pat.Format(_T("%s\\Pat.%s"), MainName, ExtName);
	ImageAPI.SaveUniFrameImage(filename_Roi, m_UniFrameList, bReverse, bEnhance, bSave3D, bAppend, SpaceRatio);
	ImageAPI.SaveUniFrameImage(filename_Pat, m_PatternFrameList, bReverse, bEnhance, bSave3D, bAppend, SpaceRatio);
	return true;
}
//-------------------------------------------------------------------------------------//
void CFdConfirmWnd::OnSaveImageBtn()
{
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|PNG Files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP;JPEG;PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	
	CString filename = dialog.GetPathName();	
	ExecSaveImageBtn(filename);
	return;
}
//-------------------------------------------------------------------------------------//