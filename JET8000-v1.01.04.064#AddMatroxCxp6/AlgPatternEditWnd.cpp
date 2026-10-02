// AlgPatternEditWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AlgPatternEditWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternEditWnd dialog
//-------------------------------------------------------------------------------------//
CAlgPatternEditWnd::CAlgPatternEditWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgPatternEditWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgPatternEditWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	IMAGE_SIZE ImageW=256;
	IMAGE_SIZE ImageH=256;
	IMAGE_SIZE ImageStep=256*3;
	IMAGE_SIZE BitCount = 24;

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_BitCount = BitCount;
	m_ImagePtr = NULL;

	m_ShowW = ImageW;
	m_ShowH = ImageH;
	m_ShowStep = ImageStep;
	m_ShowBitCnt = BitCount;
	m_ShowPtr = NULL;	

	m_MovedW = ImageW;
	m_MovedH = ImageH;
	m_MovedStep = ImageStep;
	m_MovedBitCnt = BitCount;
	m_MovedPtr = NULL;

	m_RotatedW = ImageW;
	m_RotatedH = ImageH;
	m_RotatedStep = ImageStep;
	m_RotatedBitCnt = BitCount;
	m_RotatedPtr = NULL;	

	m_ClippedW = ImageW;
	m_ClippedH = ImageH;
	m_ClippedStep = ImageStep;
	m_ClippedBitCnt = BitCount;
	m_ClippedPtr = NULL;	

	m_MoveMax = 1000;
	m_MoveMin =-1000;

	m_AngleMax =  450;
	m_AngleMin = -450;
	m_AngleStepRatio = 5;
	m_SrinkMaxW = ImageW/2;
	m_SrinkMaxH = ImageH/2;
	m_SrinkMinW = 0;
	m_SrinkMinH = 0;
	m_FirstLoadImage = true;

	::memset(&m_ClipImageRect, 0x00, sizeof(m_ClipImageRect));

	int HatchType = HS_DIAGCROSS;
	//m_ImageWnd.SetBKColor(0xEAD999);
	//m_ImageWnd.SetBKColor(0xBE9270);	
	m_ImageWnd.SetBKHatch(HatchType, 0xFFFF00);
	//m_ImageWnd.SetBKColor(0x0000FF, HatchType);	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgPatternEditWnd)
	DDX_Control(pDX, PATEDT_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, PATEDT_ANGLE_SPIN, m_AngleSpin);	
	DDX_Control(pDX, PATEDT_MOVE_SPIN_X, m_MoveSpinX);
	DDX_Control(pDX, PATEDT_MOVE_SPIN_Y, m_MoveSpinY);
	DDX_Control(pDX, PATEDT_SHRINK_SPIN_W, m_ShrinkSpinW);
	DDX_Control(pDX, PATEDT_SHRINK_SPIN_H, m_ShrinkSpinH);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgPatternEditWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgPatternEditWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_EN_KILLFOCUS(PATEDT_ANGLE_EDIT, OnKillfocusAngleEdit)
	ON_NOTIFY(UDN_DELTAPOS, PATEDT_ANGLE_SPIN, OnDeltaposAngleSpin)
	ON_BN_CLICKED(PATEDT_ANGLE_BTN, OnAngleBtn)
	ON_EN_KILLFOCUS(PATEDT_SHRINK_EDIT_W, OnKillfocusShrinkEditW)
	ON_EN_KILLFOCUS(PATEDT_SHRINK_EDIT_H, OnKillfocusShrinkEditH)
	ON_NOTIFY(UDN_DELTAPOS, PATEDT_SHRINK_SPIN_W, OnDeltaposShrinkSpinW)
	ON_NOTIFY(UDN_DELTAPOS, PATEDT_SHRINK_SPIN_H, OnDeltaposShrinkSpinH)
	ON_BN_CLICKED(PATEDT_ANGLE_STEP_RDO_01, OnAngleStepRdo01)
	ON_BN_CLICKED(PATEDT_ANGLE_STEP_RDO_05, OnAngleStepRdo05)
	ON_BN_CLICKED(PATEDT_ANGLE_STEP_RDO_10, OnAngleStepRdo10)
	ON_EN_KILLFOCUS(PATEDT_MOVE_EDIT_X, OnKillfocusMoveEditX)
	ON_EN_KILLFOCUS(PATEDT_MOVE_EDIT_Y, OnKillfocusMoveEditY)
	ON_NOTIFY(UDN_DELTAPOS, PATEDT_MOVE_SPIN_X, OnDeltaposMoveSpinX)	
	ON_NOTIFY(UDN_DELTAPOS, PATEDT_MOVE_SPIN_Y, OnDeltaposMoveSpinY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternEditWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgPatternEditWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_ImageWnd.SetShowEditLine(true);	
	m_ImageWnd.SetShowCursorLine(false);
	m_ImageWnd.SetShowEditPosText(false);
	m_ImageWnd.SetShowImageCenterLine(true);

	SwitchMultiLanguage();
	ExecLoadImageFile(m_ImageFilename);

	const int MoveMax = (int)(m_MoveMax);
	const int MoveMin = (int)(m_MoveMin);
	m_MoveSpinX.SetRange((short)(MoveMin), (short)(MoveMax));
	m_MoveSpinY.SetRange((short)(MoveMin), (short)(MoveMax));
	m_MoveSpinX.SetPos32(0);
	m_MoveSpinY.SetPos32(0);
	CWnd::SetDlgItemInt(PATEDT_MOVE_EDIT_X, 0);
	CWnd::SetDlgItemInt(PATEDT_MOVE_EDIT_Y, 0);

	const int AngleMax = (int)(m_AngleMax);
	const int AngleMin = (int)(m_AngleMin);
	m_AngleSpin.SetRange((short)(AngleMin), (short)(AngleMax));
	m_AngleSpin.SetPos32(0);
	CWnd::SetDlgItemInt(PATEDT_ANGLE_EDIT, 0);
	CWnd::CheckDlgButton(PATEDT_ANGLE_STEP_RDO_05, TRUE);
	
	const int ShrinkMaxW = (int)(m_SrinkMaxW);
	const int ShrinkMinW = (int)(m_SrinkMinW);	
	const int ShrinkMaxH = (int)(m_SrinkMaxH);
	const int ShrinkMinH = (int)(m_SrinkMinH);	
	m_ShrinkSpinW.SetRange((short)(ShrinkMinW), (short)(ShrinkMaxW));
	m_ShrinkSpinH.SetRange((short)(ShrinkMinH), (short)(ShrinkMaxH));
	m_ShrinkSpinW.SetPos32(0);	
	m_ShrinkSpinH.SetPos32(0);	
	CWnd::SetDlgItemInt(PATEDT_SHRINK_EDIT_W, 0);
	CWnd::SetDlgItemInt(PATEDT_SHRINK_EDIT_H, 0);
	
#ifndef _DEBUG
	JetAPI::ShowCtrlWnd(this, PATEDT_ANGLE_BTN, FALSE);
#endif//_DEBUG

	ExecEditImageByEditKernel();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseShowBuffer();
	ReleaseImageBuffer();
	ReleaseMovedBuffer();
	ReleaseRotatedBuffer();
	ReleaseClippedBuffer();
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	int    BtnX=0;
	POINT  Offset={0,0};
	CWnd   *WndPtr = NULL;
	WndPtr = CWnd::GetDlgItem(PATEDT_ANGLE_BTN);
	if ( NULL!=WndPtr && WndPtr->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0,0,0,0};
		WndPtr->GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		Offset.x = cx-WndRect.right-8;
		Offset.y = 0;
		BtnX = cx-8-(WndRect.right-WndRect.left);
	}
	JetAPI::MoveCtrlWnd(this, IDOK, Offset);
	JetAPI::MoveCtrlWnd(this, IDCANCEL, Offset);

	JetAPI::MoveCtrlWnd(this, PATEDT_ANGLE_GROUP, Offset);	
	JetAPI::MoveCtrlWnd(this, PATEDT_ANGLE_STEP_RDO_01, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_ANGLE_STEP_RDO_05, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_ANGLE_STEP_RDO_10, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_ANGLE_EDIT, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_ANGLE_SPIN, Offset);

	JetAPI::MoveCtrlWnd(this, PATEDT_SHRINK_GROUP, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_SHRINK_LABEL_W, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_SHRINK_EDIT_W, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_SHRINK_SPIN_W, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_SHRINK_LABEL_H, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_SHRINK_EDIT_H, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_SHRINK_SPIN_H, Offset);

	JetAPI::MoveCtrlWnd(this, PATEDT_MOVE_GROUP, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_MOVE_LABEL_X, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_MOVE_EDIT_X, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_MOVE_SPIN_X, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_MOVE_LABEL_Y, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_MOVE_EDIT_Y, Offset);
	JetAPI::MoveCtrlWnd(this, PATEDT_MOVE_SPIN_Y, Offset);

	JetAPI::MoveCtrlWnd(this, PATEDT_ANGLE_BTN, Offset);

	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0,0,0,0};
		m_ImageWnd.GetWindowRect(&WndRect);
		CWnd::ScreenToClient(&WndRect);
		WndRect.right = BtnX-8;
		WndRect.bottom = cy-4;
		m_ImageWnd.MoveWindow(&WndRect);		
		m_ImageWnd.ShowFittedZoom();	
		m_ImageWnd.SetImageEditRect(m_ClipImageRect);
	}
	Invalidate();
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 640;
	lpMMI->ptMinTrackSize.y = 480;		
}
//-------------------------------------------------------------------------------------//
BOOL CAlgPatternEditWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int    DlgCtrlID=0;
	CWnd  *WndPtr = NULL;	
	switch ( pMsg->message ) 
	{
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_RETURN:
			WndPtr = GetFocus();
			if ( NULL != WndPtr ) 
			{
				DlgCtrlID  = WndPtr->GetDlgCtrlID();
				if ( PATEDT_ANGLE_EDIT == DlgCtrlID )
				{	ExecRotateImageByAngleEdit();	}
				if ( PATEDT_SHRINK_EDIT_W == DlgCtrlID )
				{	ExecShowImageByShrinkEdit();	}
				if ( PATEDT_SHRINK_EDIT_H == DlgCtrlID )
				{	ExecShowImageByShrinkEdit();	}
				if ( PATEDT_MOVE_EDIT_X == DlgCtrlID )
				{	ExecMoveImageByOffsetEdit();	}
				if ( PATEDT_MOVE_EDIT_Y == DlgCtrlID )
				{	ExecMoveImageByOffsetEdit();	}	
			}
			return TRUE;
			break;
		case VK_ESCAPE:
			return TRUE;
			break;
		}
		break;
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CAlgPatternEditWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ALG_PATTERN_EDIT_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_ALG_PATTERN_EDIT_WND;
	WndKey = _T("IDD_ALG_PATTERN_EDIT_WND");
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
	WndID = PATEDT_ANGLE_GROUP;
	WndKey = _T("PATEDT_ANGLE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PATEDT_SHRINK_GROUP;
	WndKey = _T("PATEDT_SHRINK_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PATEDT_SHRINK_LABEL_W;
	WndKey = _T("PATEDT_SHRINK_LABEL_W");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PATEDT_SHRINK_LABEL_H;
	WndKey = _T("PATEDT_SHRINK_LABEL_H");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	
	WndID = PATEDT_MOVE_GROUP;
	WndKey = _T("PATEDT_MOVE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PATEDT_MOVE_LABEL_X;
	WndKey = _T("PATEDT_MOVE_LABEL_X");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PATEDT_MOVE_LABEL_Y;
	WndKey = _T("PATEDT_MOVE_LABEL_Y");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = PATEDT_ANGLE_BTN;
	WndKey = _T("PATEDT_ANGLE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CAlgPatternEditWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_PATTERN_EDIT_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::SetImageFilename(LPCTSTR filename, LPCTSTR filenameRet)
{
	m_ImageFilename = filename;
	m_ImageFilenameResult = filenameRet;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::ReleaseShowBuffer()
{
	if ( NULL != m_ShowPtr )
	{	JetMemory.free_func(m_ShowPtr);	}

	m_ShowW = 256;
	m_ShowH = 256;
	m_ShowStep = m_ShowW*3;
	m_ShowBitCnt = 24;
	m_ShowPtr = NULL;
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::ReleaseImageBuffer()
{
	if ( NULL != m_ImagePtr )
	{	JetMemory.free_func(m_ImagePtr);	}

	m_ImageW = 256;
	m_ImageH = 256;
	m_ImageStep = m_ImageW*3;
	m_BitCount = 24;
	m_ImagePtr = NULL;
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::ReleaseMovedBuffer()
{
	if ( NULL != m_MovedPtr )
	{	JetMemory.free_func(m_MovedPtr);	}

	m_MovedW = 256;
	m_MovedH = 256;
	m_MovedStep = m_MovedW*3;
	m_MovedBitCnt = 24;
	m_MovedPtr = NULL;
	return ;	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::ReleaseRotatedBuffer()
{
	if ( NULL != m_RotatedPtr )
	{	JetMemory.free_func(m_RotatedPtr);	}

	m_RotatedW = 256;
	m_RotatedH = 256;
	m_RotatedStep = m_RotatedW*3;
	m_RotatedBitCnt = 24;
	m_RotatedPtr = NULL;
	return ;	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::ReleaseClippedBuffer()
{
	if ( NULL != m_ClippedPtr )
	{	JetMemory.free_func(m_ClippedPtr);	}

	m_ClippedW = 256;
	m_ClippedH = 256;
	m_ClippedStep = m_ClippedW*3;
	m_ClippedBitCnt = 24;
	m_ClippedPtr = NULL;
	return ;	
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecLoadImageFile(LPCTSTR filename)
{
	const char fnName[] = "CAlgPatternEditWnd::ExecLoadImageFile";
	ReleaseShowBuffer();
	ReleaseImageBuffer();
	ReleaseMovedBuffer();
	ReleaseRotatedBuffer();
	ReleaseClippedBuffer();

	IMAGE_SIZE  ImageW = 0;
	IMAGE_SIZE  ImageH = 0;
	IMAGE_SIZE  ImageStep = 0;
	IMAGE_SIZE  BitCount = 0;
	IMAGE_PTR   ImagePtr = NULL;
	const int   nAlign = 4;

	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true) == false ) 
	{	return false; }
	
	IMAGE_SIZE RotatedW = ImageAPI.CalcRotateImageSize(ImageW, ImageH);
	IMAGE_SIZE RotatedH = RotatedW;
	IMAGE_SIZE RotatedStep = JetAPI::GetBMPImagePixelsPerLine(RotatedW, BitCount, nAlign);
	const size_t RotatedSize = ImageAPI.CalcBufferSize(RotatedStep, RotatedH);
	if ( JetMemory.alloc_func(RotatedSize, m_ShowPtr, fnName, "m_ShowPtr") == false ||
		 JetMemory.alloc_func(RotatedSize, m_MovedPtr, fnName, "m_MovedPtr") == false ||
		 JetMemory.alloc_func(RotatedSize, m_RotatedPtr, fnName, "m_RotatedPtr") == false ||
		 JetMemory.alloc_func(RotatedSize, m_ClippedPtr, fnName, "m_ClippedPtr") == false )
	{
		JetMemory.free_func(ImagePtr);
		JetMemory.free_func(m_ShowPtr);
		JetMemory.free_func(m_MovedPtr);
		JetMemory.free_func(m_RotatedPtr);
		JetMemory.free_func(m_ClippedPtr);
		return false;
	}
	::memset(m_ShowPtr, 0x00, sizeof(IMAGE_DATA)*RotatedSize);
	::memset(m_MovedPtr, 0x00, sizeof(IMAGE_DATA)*RotatedSize);	
	::memset(m_RotatedPtr, 0x00, sizeof(IMAGE_DATA)*RotatedSize);
	::memset(m_ClippedPtr, 0x00, sizeof(IMAGE_DATA)*RotatedSize);

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_BitCount = BitCount;
	m_ImagePtr = ImagePtr;	

	m_MovedW = ImageW;
	m_MovedH = ImageH;
	m_MovedStep = ImageStep;
	m_MovedBitCnt = BitCount;	
	m_MoveMax = MAX(ImageW, ImageH);
	m_MoveMin = -m_MoveMax;	

	m_RotatedW = RotatedW;
	m_RotatedH = RotatedH;
	m_RotatedStep = RotatedStep;
	m_RotatedBitCnt = BitCount;	
	
	m_SrinkMaxW = ImageW/2;
	m_SrinkMaxH = ImageH/2;
	m_SrinkMinW = 0;
	m_SrinkMinH = 0;
	m_FirstLoadImage = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecMoveImageByOffsetEdit()
{
	CString strX, strY;
	const int MoveMax = (int)(m_MoveMax);
	const int MoveMin = (int)(m_MoveMin);
	CWnd::GetDlgItemText(PATEDT_MOVE_EDIT_X, strX);
	CWnd::GetDlgItemText(PATEDT_MOVE_EDIT_Y, strY);

	bool   bFixedX=false;
	bool   bFixedY=false;
	double OffsetX = ::_ttof(strX);
	double OffsetY = ::_ttof(strY);
	int    nPosX = (int)(OffsetX);
	int    nPosY = (int)(OffsetY);

	if ( nPosX > MoveMax ) 
	{ 
		bFixedX = true;
		nPosX = MoveMax;		
	}
	else if ( nPosX < MoveMin ) 
	{ 
		bFixedX = true;
		nPosX = MoveMin;		
	}
	if ( nPosY > MoveMax ) 
	{ 
		bFixedY = true;
		nPosY = MoveMax;		
	}
	else if ( nPosY < MoveMin ) 
	{ 
		bFixedY = true;
		nPosY = MoveMin;		
	}

	m_MoveSpinX.SetPos32(nPosX);
	m_MoveSpinY.SetPos32(nPosY);
	if ( true == bFixedX ) 
	{		
		double Pos=(double)(nPosX);
		strX.Format(_T("%.0f"), Pos);
		CWnd::SetDlgItemText(PATEDT_MOVE_EDIT_X, strX);		
	}
	if ( true == bFixedY ) 
	{		
		double Pos=(double)(nPosY);
		strY.Format(_T("%.0f"), Pos);
		CWnd::SetDlgItemText(PATEDT_MOVE_EDIT_Y, strY);		
	}
	return ExecEditImageByEditKernel();		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecMoveImageByOffsetEditKernel()
{
	CString strX, strY;	
	CWnd::GetDlgItemText(PATEDT_MOVE_EDIT_X, strX);
	CWnd::GetDlgItemText(PATEDT_MOVE_EDIT_Y, strY);
	int OffsetX = ::_ttoi(strX);	
	int OffsetY = ::_ttoi(strY);	
	if ( ExecMoveImage(OffsetX, OffsetY) == false )
	{	return false; }	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecMoveImage(int sX, int sY)
{
	IMAGE_SIZE  ImageW = m_ImageW;
	IMAGE_SIZE  ImageH = m_ImageH;
	IMAGE_SIZE  ImageStep = m_ImageStep;
	IMAGE_SIZE  BitCount = m_BitCount;
	IMAGE_PTR   ImagePtr = m_ImagePtr;
	IMAGE_PTR   ShowPtr = m_ShowPtr;
	IMAGE_PTR   MovedPtr = m_MovedPtr;
	if ( NULL == ImagePtr ) { return true; }
	if ( NULL == MovedPtr ) { return true; }	
	
	const int    nPosX=sX;
	const int    nPosY=sY;
	//const int    nPosX=JetAPI::Floor(sX);
	//const int    nPosY=JetAPI::Floor(sY);		
	if ( ImageAPI.MoveImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, nPosX, nPosY, MovedPtr) == false ) 
	{	return false; }	
	m_MovedW = ImageW;//移動圖寬度
	m_MovedH = ImageH;//移動圖長度
	m_MovedStep = ImageStep;//移動圖步長
	m_MovedBitCnt = BitCount;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecRotateImageByAngleEdit()
{
	CString strAngle;
	const int AngleMax = (int)(m_AngleMax);
	const int AngleMin = (int)(m_AngleMin);
	CWnd::GetDlgItemText(PATEDT_ANGLE_EDIT, strAngle);

	bool   bFixed=false;
	double Angle = ::_ttof(strAngle);
	int    nPos = (int)(Angle*10.0);
	if ( nPos > AngleMax ) 
	{ 
		bFixed = true;
		nPos = AngleMax;		
	}
	else if (  nPos < AngleMin ) 
	{ 
		bFixed = true;
		nPos = AngleMin;		
	}

	m_AngleSpin.SetPos32(nPos);
	if ( true == bFixed ) 
	{
		Angle = nPos*0.1;
		strAngle.Format(_T("%.1f"), Angle);
		CWnd::SetDlgItemText(PATEDT_ANGLE_EDIT, strAngle);
	}
	return ExecEditImageByEditKernel();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecRotateImageByAngleEditKernel()
{
	CString strAngle;	
	CWnd::GetDlgItemText(PATEDT_ANGLE_EDIT, strAngle);
	double Angle = ::_ttof(strAngle);	
	return ExecRotateImage(Angle);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecRotateImage(double Angle)
{
	IMAGE_SIZE  ImageW = m_MovedW;
	IMAGE_SIZE  ImageH = m_MovedH;
	IMAGE_SIZE  ImageStep = m_MovedStep;
	IMAGE_SIZE  BitCount = m_MovedBitCnt;
	IMAGE_PTR   ImagePtr = m_MovedPtr;
	IMAGE_PTR   ShowPtr = m_ShowPtr;
	IMAGE_PTR   RotatedPtr = m_RotatedPtr;

	if ( NULL == ImagePtr ) { return true; }
	if ( NULL == RotatedPtr ) { return true; }
	const int    nAlign = 4;
	const double dPrecision = 0.001;
	IMAGE_SIZE RotatedW = m_RotatedW;//旋轉圖寬度
	IMAGE_SIZE RotatedH = m_RotatedH;//旋轉圖長度
	IMAGE_SIZE RotatedStep = m_RotatedStep;//旋轉圖步長
	const size_t RotatedSize = ImageAPI.CalcBufferSize(RotatedStep, RotatedH);	
	RECT RoiRect={0,0,0,0};
	RoiRect.left = (RotatedW-ImageW)/2;
	RoiRect.top  = (RotatedH-ImageH)/2;
	RoiRect.right  = RoiRect.left+ImageW;
	RoiRect.bottom = RoiRect.top +ImageH;
	if ( fabs(Angle)<dPrecision )
	{	
		::memset(RotatedPtr, 0x00, sizeof(IMAGE_DATA)*RotatedSize);
		if ( ImageAPI.PasteRoiImage3(RotatedW, RotatedH, RotatedStep, BitCount, RotatedPtr, RoiRect, ImageStep, ImagePtr, false, false) == false ) 
		{	return false; }		
	}
	else 
	{
		if ( ImageAPI.RotateImage3(Angle, ImageW, ImageH, ImageStep, BitCount, ImagePtr, RotatedW, RotatedH, RotatedStep, RotatedPtr) == false ) 
		{	return false; }	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnKillfocusAngleEdit() 
{
	// TODO: Add your control notification handler code here
	ExecRotateImageByAngleEdit();
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnDeltaposAngleSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString   str;
	pNMUpDown->iDelta *= m_AngleStepRatio;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	const int AngleMax = (int)(m_AngleMax);
	const int AngleMin = (int)(m_AngleMin);
	if ( nNextPos > AngleMax ) { nNextPos = AngleMax; }
	else if ( nNextPos < AngleMin ) { nNextPos = AngleMin; }
	pNMUpDown->iDelta = nNextPos-nPos;

	double    dAngle = nNextPos/10.0;
	str.Format(_T("%.1f"), dAngle);
	CWnd::SetDlgItemText(PATEDT_ANGLE_EDIT, str);
	ExecEditImageByEditKernel();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecEditImageByEditKernel()
{
	if ( ExecMoveImageByOffsetEditKernel() == false ) { return false; }
	if ( ExecRotateImageByAngleEditKernel() == false ) { return false; }
	if ( ExecShowImageByShrinkEditKernel() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecClippedImage()
{
	IMAGE_SIZE  ImageW = m_ImageW;
	IMAGE_SIZE  ImageH = m_ImageH;
	IMAGE_SIZE  ImageStep = m_ImageStep;
	IMAGE_SIZE  BitCount = m_BitCount;
	IMAGE_PTR   ImagePtr = m_ImagePtr;	
	IMAGE_PTR   RotatedPtr = m_RotatedPtr;
	IMAGE_PTR   ClippedPtr = m_ClippedPtr;

	if ( NULL == ImagePtr ) { return true; }
	if ( NULL == RotatedPtr ) { return true; }
	if ( NULL == ClippedPtr ) { return true; }
	
	CString      str;
	const int    nAlign = 4;
	const double dPrecision = 0.001;
	IMAGE_SIZE RotatedW = m_RotatedW;//旋轉圖寬度
	IMAGE_SIZE RotatedH = m_RotatedH;//旋轉圖長度
	IMAGE_SIZE RotatedStep = m_RotatedStep;//旋轉圖步長
	const size_t RotatedSize = ImageAPI.CalcBufferSize(RotatedStep, RotatedH);
	RECT RoiRect={0,0,0,0};
	RoiRect.left = (RotatedW-ImageW)/2;
	RoiRect.top  = (RotatedH-ImageH)/2;
	RoiRect.right  = RoiRect.left+ImageW;
	RoiRect.bottom = RoiRect.top +ImageH;

	const int ShrinkW = CWnd::GetDlgItemInt(PATEDT_SHRINK_EDIT_W);
	const int ShrinkH = CWnd::GetDlgItemInt(PATEDT_SHRINK_EDIT_H);
	RoiRect.left += ShrinkW;
	RoiRect.top += ShrinkH;
	RoiRect.right -= ShrinkW;
	RoiRect.bottom -= ShrinkH;
	IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
	if ( ImageAPI.ExtractRoiImage3(RotatedW, RotatedH, RotatedStep, BitCount, RotatedPtr, RoiRect, RoiStep, ClippedPtr, false) == false )
	{	return false; }

	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PatternClipped.PNG"));
	str = m_ImageFilenameResult;
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, ClippedPtr, true);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnAngleBtn() 
{
	// TODO: Add your control notification handler code here
	ExecClippedImage();
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnOK() 
{
	// TODO: Add extra validation here
	ExecClippedImage();
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecShowImageByShrinkEdit()
{
	CString strShrinkW;
	CString strShrinkH;
	const int ShrinkMaxW = (int)(m_SrinkMaxW);
	const int ShrinkMinW = (int)(m_SrinkMinW);
	const int ShrinkMaxH = (int)(m_SrinkMaxH);
	const int ShrinkMinH = (int)(m_SrinkMinH);
	CWnd::GetDlgItemText(PATEDT_SHRINK_EDIT_W, strShrinkW);
	CWnd::GetDlgItemText(PATEDT_SHRINK_EDIT_H, strShrinkH);

	bool bFixedW=false;
	bool bFixedH=false;
	int  ShrinkW = ::_ttoi(strShrinkW);
	int  ShrinkH = ::_ttoi(strShrinkH);
	if ( ShrinkW > ShrinkMaxW )
	{
		bFixedW = true;
		ShrinkW = ShrinkMaxW; 
	}
	else if ( ShrinkW < ShrinkMinW )
	{
		bFixedW = true;
		ShrinkW = ShrinkMinW; 
	}		
	if ( ShrinkH > ShrinkMaxH )
	{
		bFixedH = true;
		ShrinkH = ShrinkMaxH; 
	}
	else if ( ShrinkH < ShrinkMinH )
	{
		bFixedH = true;
		ShrinkH = ShrinkMinH; 
	}	
	m_ShrinkSpinW.SetPos32(ShrinkW);
	m_ShrinkSpinH.SetPos32(ShrinkH);
	if ( true == bFixedW ) 
	{	CWnd::SetDlgItemInt(PATEDT_SHRINK_EDIT_W, ShrinkW);	}
	if ( true == bFixedH ) 
	{	CWnd::SetDlgItemInt(PATEDT_SHRINK_EDIT_H, ShrinkH);	}
	if ( ExecEditImageByEditKernel() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecShowImageByShrinkEditKernel()
{
	const int ShrinkW = CWnd::GetDlgItemInt(PATEDT_SHRINK_EDIT_W);
	const int ShrinkH = CWnd::GetDlgItemInt(PATEDT_SHRINK_EDIT_H);
	if ( ExecShowImage(ShrinkW, ShrinkH) == false )
	{ return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternEditWnd::ExecShowImage(int W, int H)
{
	IMAGE_SIZE  ImageW = m_ImageW;
	IMAGE_SIZE  ImageH = m_ImageH;
	IMAGE_SIZE  ImageStep = m_ImageStep;
	IMAGE_SIZE  BitCount = m_BitCount;	
	IMAGE_PTR   ShowPtr = m_ShowPtr;
	IMAGE_PTR   RotatedPtr = m_RotatedPtr;

	if ( NULL == ShowPtr ) { return true; }
	if ( NULL == RotatedPtr ) { return true; }

	const int    nAlign = 4;
	const double dPrecision = 0.001;
	IMAGE_SIZE RotatedW = m_RotatedW;//旋轉圖寬度
	IMAGE_SIZE RotatedH = m_RotatedH;//旋轉圖長度
	IMAGE_SIZE RotatedStep = m_RotatedStep;//旋轉圖步長
	const size_t RotatedSize = ImageAPI.CalcBufferSize(RotatedStep, RotatedH);	
	RECT RoiRect={0,0,0,0};
	RoiRect.left = (RotatedW-ImageW)/2;
	RoiRect.top  = (RotatedH-ImageH)/2;
	RoiRect.right  = RoiRect.left+ImageW;
	RoiRect.bottom = RoiRect.top +ImageH;
	
	m_ShowW = RotatedW;//顯示圖寬度
	m_ShowH = RotatedH;//顯示圖長度
	m_ShowStep = RotatedStep;//顯示圖步長
	m_ShowBitCnt = BitCount;//顯示圖位元數
	AOIDataCollect.ExecEnhanceDisplayImage(RotatedW, RotatedH, RotatedStep, BitCount, RotatedPtr, ShowPtr);

#ifdef _DEBUG
	bool   bSave = false;
	if ( true == bSave )
	{
		CString str, strAngle;
		CWnd::GetDlgItemText(PATEDT_ANGLE_EDIT, strAngle);
		double Angle = ::_ttof(strAngle);
		str.Format(_T("%s\\%s_%.2f.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("PatternShow"), Angle);
		ImageAPI.SaveImage(str, RotatedW, RotatedH, RotatedStep, BitCount, ShowPtr, true);
	}
#endif//_DEBUG
	
	const int ShrinkW = (W);
	const int ShrinkH = (H);
	RoiRect.left += ShrinkW;
	RoiRect.top += ShrinkH;
	RoiRect.right -= ShrinkW;
	RoiRect.bottom -= ShrinkH;
	m_ClipImageRect = RoiRect;

	const bool bRedraw = false;
	const bool bResetView = false;
	m_ImageWnd.SetImageBuffer(RotatedW, RotatedH, RotatedStep, BitCount, ShowPtr, true, bResetView, bRedraw);		
	if ( true == m_FirstLoadImage ) 
	{	m_ImageWnd.ShowFittedZoom(); }
	m_ImageWnd.SetImageEditRect(RoiRect);
	m_ImageWnd.RedrawWnd(TRUE);
	m_FirstLoadImage = false;
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnKillfocusShrinkEditW() 
{
	// TODO: Add your control notification handler code here
	ExecShowImageByShrinkEdit();	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnKillfocusShrinkEditH() 
{
	// TODO: Add your control notification handler code here
	ExecShowImageByShrinkEdit();	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnDeltaposShrinkSpinW(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString   str;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	const int ShrinkMax = (int)(m_SrinkMaxW);
	const int ShrinkMin = (int)(m_SrinkMinW);
	if ( nNextPos > ShrinkMax ) { nNextPos = ShrinkMax; }
	else if ( nNextPos < ShrinkMin ) { nNextPos = ShrinkMin; }
	pNMUpDown->iDelta = nNextPos-nPos;

	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PATEDT_SHRINK_EDIT_W, str);
	ExecEditImageByEditKernel();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnDeltaposShrinkSpinH(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString   str;
	const int nPos = pNMUpDown->iPos;	
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;
	const int ShrinkMax = (int)(m_SrinkMaxH);
	const int ShrinkMin = (int)(m_SrinkMinH);
	if ( nNextPos > ShrinkMax ) { nNextPos = ShrinkMax; }
	else if ( nNextPos < ShrinkMin ) { nNextPos = ShrinkMin; }
	pNMUpDown->iDelta = nNextPos-nPos;

	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PATEDT_SHRINK_EDIT_H, str);
	ExecEditImageByEditKernel();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnAngleStepRdo01() 
{
	// TODO: Add your control notification handler code here
	m_AngleStepRatio = 1;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnAngleStepRdo05() 
{
	// TODO: Add your control notification handler code here
	m_AngleStepRatio = 5;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnAngleStepRdo10() 
{
	// TODO: Add your control notification handler code here
	m_AngleStepRatio = 10;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnKillfocusMoveEditX() 
{
	// TODO: Add your control notification handler code here
	ExecMoveImageByOffsetEdit();
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnKillfocusMoveEditY() 
{
	// TODO: Add your control notification handler code here
	ExecMoveImageByOffsetEdit();
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnDeltaposMoveSpinX(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString   str;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;	
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PATEDT_MOVE_EDIT_X, str);
	ExecEditImageByEditKernel();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternEditWnd::OnDeltaposMoveSpinY(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString   str;
	const int nPos = pNMUpDown->iPos;
	const int nDelta = pNMUpDown->iDelta;
	int       nNextPos = nPos+nDelta;	
	str.Format(_T("%d"), nNextPos);
	CWnd::SetDlgItemText(PATEDT_MOVE_EDIT_Y, str);
	ExecEditImageByEditKernel();
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//