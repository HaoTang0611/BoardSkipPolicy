// ImageMaskWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ImageMaskWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageMaskWnd dialog
//-------------------------------------------------------------------------------------//
CImageMaskWnd::CImageMaskWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CImageMaskWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageMaskWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->m_ImageW = 0;
	this->m_ImageH = 0;
	this->m_BitCount = 24;
	this->m_ImageStep = 0;
	this->m_ImagePtr = NULL;	
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageMaskWnd)
	DDX_Control(pDX, IMGMASK_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CImageMaskWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CImageMaskWnd)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IMGMASK_LOAD_IMAGE_BTN, OnLoadImageBtn)	
	ON_BN_CLICKED(IMGMASK_SHOW_MASK_CHK, OnShowMaskChk)
	ON_BN_CLICKED(IMGMASK_SHOW_BIT01_CHK, OnShowBit01Chk)
	ON_BN_CLICKED(IMGMASK_SHOW_BIT02_CHK, OnShowBit02Chk)
	ON_BN_CLICKED(IMGMASK_SHOW_BIT03_CHK, OnShowBit03Chk)
	ON_BN_CLICKED(IMGMASK_SHOW_BIT04_CHK, OnShowBit04Chk)
	ON_BN_CLICKED(IMGMASK_SHOW_BIT05_CHK, OnShowBit05Chk)
	ON_BN_CLICKED(IMGMASK_SHOW_BIT06_CHK, OnShowBit06Chk)
	ON_BN_CLICKED(IMGMASK_SHOW_BIT07_CHK, OnShowBit07Chk)
	ON_BN_CLICKED(IMGMASK_SHOW_BIT08_CHK, OnShowBit08Chk)
	ON_BN_CLICKED(IMGMASK_DISABLE_ALL_BTN, OnDisableAllBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageMaskWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CImageMaskWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CImageMaskWnd::AdjustCtrlWnd(-1, -1);
	CImageMaskWnd::SwitchMultiLanguage();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	CImageMaskWnd::ReleaseImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CImageMaskWnd::AdjustCtrlWnd(cx, cy);
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 200;
	lpMMI->ptMinTrackSize.y = 200;
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::AdjustCtrlWnd(int cx, int cy)
{
	RECT Rect={0};
	if ( cx<0 || cy<0 )
	{
		this->GetClientRect(&Rect);
		cx = Rect.right-Rect.left;
		cy = Rect.bottom-Rect.top;
	}

	CWnd *pWnd = NULL;
	const int MarginW=4;
	const int MarginH=4;	
	pWnd = CWnd::GetDlgItem(IMGMASK_SHOW_SETTING_GROUP);
	if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
	{
		RECT WndRect={0};
		pWnd->GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.left = MarginW;
		WndRect.right = cx-MarginW;		
		pWnd->MoveWindow(&WndRect);
	}
	if ( m_ImageWnd.GetSafeHwnd() != NULL )
	{
		RECT WndRect={0};
		this->m_ImageWnd.GetWindowRect(&WndRect);
		this->ScreenToClient(&WndRect);
		WndRect.left = MarginW;
		WndRect.right = cx-MarginW;
		WndRect.bottom = cy-MarginH;
		this->m_ImageWnd.MoveWindow(&WndRect);		
	}
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_IMAGE_MASK_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_IMAGE_MASK_WND;
	WndKey = _T("IDD_IMAGE_MASK_WND");
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
	WndID = IMGMASK_LOAD_IMAGE_BTN;
	WndKey = _T("IMGMASK_LOAD_IMAGE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_SETTING_GROUP;
	WndKey = _T("IMGMASK_SHOW_SETTING_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_MASK_CHK;
	WndKey = _T("IMGMASK_SHOW_MASK_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_BIT01_CHK;
	WndKey = _T("IMGMASK_SHOW_BIT01_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_BIT02_CHK;
	WndKey = _T("IMGMASK_SHOW_BIT02_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_BIT03_CHK;
	WndKey = _T("IMGMASK_SHOW_BIT03_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = IMGMASK_SHOW_BIT04_CHK;
	WndKey = _T("IMGMASK_SHOW_BIT04_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_BIT05_CHK;
	WndKey = _T("IMGMASK_SHOW_BIT05_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_BIT06_CHK;
	WndKey = _T("IMGMASK_SHOW_BIT06_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_BIT07_CHK;
	WndKey = _T("IMGMASK_SHOW_BIT07_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGMASK_SHOW_BIT08_CHK;
	WndKey = _T("IMGMASK_SHOW_BIT08_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnLoadImageBtn() 
{
	// TODO: Add your control notification handler code here	
	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|JPEG Files (*.JPG)|*.JPG|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("BMP;JPEG"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }
	
	CString filename = dialog.GetPathName();	
	CImageMaskWnd::ExecLoadMaskFile(filename);
}
//-------------------------------------------------------------------------------------//
bool CImageMaskWnd::ExecLoadMaskFile(LPCTSTR filename)
{
	CString str;
	const char fnName[] = "CImageMaskWnd::ExecLoadMaskFile";
	if ( this->m_Dib.Load(filename) == false )
	{
		str.Format(_T("Error Load BMP File Fault(%s)"), filename);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	
	CWnd::SetWindowText(filename);
	CImageMaskWnd::ReleaseImageBuffer();
	this->m_ImageW = this->m_Dib.GetImageW();
	this->m_ImageH = this->m_Dib.GetImageH();
	this->m_BitCount = 24;
	this->m_ImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ImageW, m_BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_ImageStep, m_ImageH);	
	if ( JetMemory.alloc_func(BufferSize, m_ImagePtr, fnName, "m_ImagePtr") == false )
	{
		str = JetMemory.GetErrorString();
		JetAPI::ShowMessageBox(str);
		CImageMaskWnd::ReleaseImageBuffer();
		return false;
	}
	this->m_ImageWnd.SetImageBuffer(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImagePtr, false, true);
	CImageMaskWnd::UpdateShowImage();
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::ReleaseImageBuffer()
{
	if ( NULL != this->m_ImagePtr )
	{	JetMemory.free_func(m_ImagePtr);	}
	this->m_ImageW = 0;
	this->m_ImageH = 0;
	this->m_BitCount = 24;
	this->m_ImageStep = 0;	
	this->m_ImageWnd.SetImageBuffer(m_ImageW, m_ImageH, m_ImageStep, m_BitCount, NULL, false, true);
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::UpdateShowImage()
{
	if ( NULL == m_ImagePtr ) { return; }
	const unsigned char *SrcImagePtr = this->m_Dib.GetDIBBits();
	if ( NULL == SrcImagePtr ) { return; }

	size_t i=0, j=0;
	size_t srcIdx=0, dstIdx=0;
	const IMAGE_SIZE ImageW = this->m_Dib.GetImageW();
	const IMAGE_SIZE ImageH = this->m_Dib.GetImageH();
	const IMAGE_SIZE SrcBitCount = this->m_Dib.GetImageBitCount();
	const IMAGE_SIZE SrcChannel = JetAPI::GetImageChannels(SrcBitCount);
	const IMAGE_SIZE SrcImageStep = this->m_Dib.GetImageBytePerLine();
	unsigned char BitMask = 0x00;
	
	BitMask = 0x00;
	BOOL  bShowMask = CWnd::IsDlgButtonChecked(IMGMASK_SHOW_MASK_CHK);
	if ( TRUE == bShowMask )
	{		
		if ( CWnd::IsDlgButtonChecked(IMGMASK_SHOW_BIT01_CHK) == TRUE )
		{	BitMask |= 0x01;	}
		if ( CWnd::IsDlgButtonChecked(IMGMASK_SHOW_BIT02_CHK) == TRUE )
		{	BitMask |= 0x02;	}
		if ( CWnd::IsDlgButtonChecked(IMGMASK_SHOW_BIT03_CHK) == TRUE )
		{	BitMask |= 0x04;	}
		if ( CWnd::IsDlgButtonChecked(IMGMASK_SHOW_BIT04_CHK) == TRUE )
		{	BitMask |= 0x08;	}
		if ( CWnd::IsDlgButtonChecked(IMGMASK_SHOW_BIT05_CHK) == TRUE )
		{	BitMask |= 0x10;	}
		if ( CWnd::IsDlgButtonChecked(IMGMASK_SHOW_BIT06_CHK) == TRUE )
		{	BitMask |= 0x20;	}
		if ( CWnd::IsDlgButtonChecked(IMGMASK_SHOW_BIT07_CHK) == TRUE )
		{	BitMask |= 0x40;	}
		if ( CWnd::IsDlgButtonChecked(IMGMASK_SHOW_BIT08_CHK) == TRUE )
		{	BitMask |= 0x80;	}
	}	
	

	for ( i=0; i<ImageH; i++ )
	{
		srcIdx = i*SrcImageStep;
		dstIdx = i*m_ImageStep;
		for ( j=0; j<ImageW; j++ )
		{
			if ( 0x00 == BitMask )
			{
				m_ImagePtr[dstIdx]   = SrcImagePtr[srcIdx];
				m_ImagePtr[dstIdx+1] = SrcImagePtr[srcIdx];
				m_ImagePtr[dstIdx+2] = SrcImagePtr[srcIdx];
			}
			else
			{
				if ( (SrcImagePtr[srcIdx]&BitMask) != NULL )
				{
					m_ImagePtr[dstIdx] = 0xFF;
					m_ImagePtr[dstIdx+1] = 0xFF;	
					m_ImagePtr[dstIdx+2] = 0x00;	
				}
				else
				{	
					m_ImagePtr[dstIdx] = 0x00;	
					m_ImagePtr[dstIdx+1] = 0x00;
					m_ImagePtr[dstIdx+2] = 0x00;	
				}						
			}			
			dstIdx += 3;
			srcIdx += SrcChannel;
		}
	}

	CImageMaskWnd::m_ImageWnd.RedrawWnd(TRUE);
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowMaskChk() 
{
	// TODO: Add your control notification handler code here
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowBit01Chk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = IMGMASK_SHOW_BIT01_CHK;
	if ( CWnd::IsDlgButtonChecked(CtrlID) == TRUE )
	{	CWnd::CheckDlgButton(IMGMASK_SHOW_MASK_CHK, TRUE); }
	CImageMaskWnd::UpdateShowImage();	
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowBit02Chk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = IMGMASK_SHOW_BIT02_CHK;
	if ( CWnd::IsDlgButtonChecked(CtrlID) == TRUE )
	{	CWnd::CheckDlgButton(IMGMASK_SHOW_MASK_CHK, TRUE); }
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowBit03Chk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = IMGMASK_SHOW_BIT03_CHK;
	if ( CWnd::IsDlgButtonChecked(CtrlID) == TRUE )
	{	CWnd::CheckDlgButton(IMGMASK_SHOW_MASK_CHK, TRUE); }
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowBit04Chk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = IMGMASK_SHOW_BIT04_CHK;
	if ( CWnd::IsDlgButtonChecked(CtrlID) == TRUE )
	{	CWnd::CheckDlgButton(IMGMASK_SHOW_MASK_CHK, TRUE); }
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowBit05Chk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = IMGMASK_SHOW_BIT05_CHK;
	if ( CWnd::IsDlgButtonChecked(CtrlID) == TRUE )
	{	CWnd::CheckDlgButton(IMGMASK_SHOW_MASK_CHK, TRUE); }
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowBit06Chk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = IMGMASK_SHOW_BIT06_CHK;
	if ( CWnd::IsDlgButtonChecked(CtrlID) == TRUE )
	{	CWnd::CheckDlgButton(IMGMASK_SHOW_MASK_CHK, TRUE); }
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowBit07Chk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = IMGMASK_SHOW_BIT07_CHK;
	if ( CWnd::IsDlgButtonChecked(CtrlID) == TRUE )
	{	CWnd::CheckDlgButton(IMGMASK_SHOW_MASK_CHK, TRUE); }
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnShowBit08Chk() 
{
	// TODO: Add your control notification handler code here
	UINT CtrlID = IMGMASK_SHOW_BIT08_CHK;
	if ( CWnd::IsDlgButtonChecked(CtrlID) == TRUE )
	{	CWnd::CheckDlgButton(IMGMASK_SHOW_MASK_CHK, TRUE); }
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
void CImageMaskWnd::OnDisableAllBtn() 
{
	// TODO: Add your control notification handler code here
	CWnd::CheckDlgButton(IMGMASK_SHOW_BIT01_CHK, FALSE);
	CWnd::CheckDlgButton(IMGMASK_SHOW_BIT02_CHK, FALSE);
	CWnd::CheckDlgButton(IMGMASK_SHOW_BIT03_CHK, FALSE);
	CWnd::CheckDlgButton(IMGMASK_SHOW_BIT04_CHK, FALSE);
	CWnd::CheckDlgButton(IMGMASK_SHOW_BIT05_CHK, FALSE);
	CWnd::CheckDlgButton(IMGMASK_SHOW_BIT06_CHK, FALSE);
	CWnd::CheckDlgButton(IMGMASK_SHOW_BIT07_CHK, FALSE);
	CWnd::CheckDlgButton(IMGMASK_SHOW_BIT08_CHK, FALSE);
	CImageMaskWnd::UpdateShowImage();
}
//-------------------------------------------------------------------------------------//
