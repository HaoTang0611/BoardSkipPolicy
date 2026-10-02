// ImagePhaseWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ImagePhaseWnd.h"
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImagePhaseWnd dialog
//-------------------------------------------------------------------------------------//
CImagePhaseWnd::CImagePhaseWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CImagePhaseWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImagePhaseWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->m_MovingPos.x = this->m_MovingPos.y = 0;
	this->m_RBtnUpPos.x = this->m_RBtnUpPos.y = -1;
	this->m_RBtnDownPos.x = this->m_RBtnDownPos.y = -1;
	this->m_LBtnUpPos.x = this->m_LBtnUpPos.y = -1;
	this->m_LBtnDownPos.x = this->m_LBtnDownPos.y = -1;	
	m_StartPos.x = m_StartPos.y = 0;

	this->m_ScaleMode = PHASE_TO_IMAGE_DYNAMIC_SCALE;
	this->m_bClonePhase = FALSE;
	this->m_bCloneSpace = FALSE;
	this->m_ImageW = 0;
	this->m_ImageH = 0;
	this->m_ImageStep = 0;
	this->m_PhaseStep = 0;
	this->m_SpaceStep = 0;
	this->m_BitCount = 0;
	this->m_ImageBuffer = NULL;
	this->m_PhaseBuffer = NULL;
	this->m_SpaceBuffer = NULL;
	this->m_PhaseAve = 0;
	this->m_SpaceAve = 0;
	this->m_ImageZoom = 1;		
	m_BkColor = 0x000000;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImagePhaseWnd)
	DDX_Control(pDX, IMGPHASE_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CImagePhaseWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CImagePhaseWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IMGPHASE_SAVE_BTN, OnSaveBtn)
	ON_BN_CLICKED(IMGPHASE_NOISE_LOW_CONTRAST_BTN, OnNoiseLowContrastBtn)
	ON_BN_CLICKED(IMGPHASE_NOISE_LOW_POTENTIAL_BTN, OnNoiseLowPotentialBtn)
	ON_BN_CLICKED(IMGPHASE_NOISE_OVER_SATURATED_BTN, OnNoiseOverSaturatedBtn)
	ON_BN_CLICKED(IMGPHASE_NOISE_VOID_EXTENDED_BTN, OnNoiseVoidExtendedBtn)
	ON_BN_CLICKED(IMGPHASE_NOISE_HEIGHT_UNEXPECTED_BTN, OnNoiseHeightUnexpectedBtn)
	ON_BN_CLICKED(IMGPHASE_NOISE_OVER_LOW_BTN, OnNoiseOverLowBtn)
	ON_BN_CLICKED(IMGPHASE_VALID_BEST_BTN, OnValidBestBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImagePhaseWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CImagePhaseWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SwitchMultiLanguage();

	this->AdjustCtrlWnd();
	const IMAGE_SIZE ImageW = this->m_ImageW;
	const IMAGE_SIZE ImageH = this->m_ImageH;
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, this->m_ImageWndRect, 1.1, this->m_ImageZoom);
	DrawImageWndMemDC();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	this->ReleaseBuffer();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->AdjustCtrlWnd(cx, cy);	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{	DrawImageWndMemDC();	}
}
//-------------------------------------------------------------------------------------//
BOOL CImagePhaseWnd::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class	
	if ( ExecMouseWheelMSG(pMsg->message, pMsg->wParam, pMsg->lParam) == true ) 
	{	return TRUE; }	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_IMAGE_PHASE_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_IMAGE_PHASE_WND;
	WndKey = _T("IDD_IMAGE_PHASE_WND");
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
	WndID = IMGPHASE_SAVE_BTN;
	WndKey = _T("IMGPHASE_SAVE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
	WndID = IMGPHASE_NOISE_LOW_CONTRAST_BTN;
	WndKey = _T("IMGPHASE_NOISE_LOW_CONTRAST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPHASE_NOISE_LOW_POTENTIAL_BTN;
	WndKey = _T("IMGPHASE_NOISE_LOW_POTENTIAL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPHASE_NOISE_OVER_SATURATED_BTN;
	WndKey = _T("IMGPHASE_NOISE_OVER_SATURATED_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPHASE_NOISE_VOID_EXTENDED_BTN;
	WndKey = _T("IMGPHASE_NOISE_VOID_EXTENDED_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPHASE_NOISE_HEIGHT_UNEXPECTED_BTN;
	WndKey = _T("IMGPHASE_NOISE_HEIGHT_UNEXPECTED_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = IMGPHASE_VALID_BEST_BTN;
	WndKey = _T("IMGPHASE_VALID_BEST_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::AdjustCtrlWnd(int cx, int cy)
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
	pWnd = this->GetDlgItem(IMGPHASE_INFO_EDIT);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
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
		m_ImageWnd.MoveWindow(&WndRect);
		m_ImageWnd.GetClientRect(&m_ImageWndRect);
		m_ImageWndMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);
		DrawImageWndMemDC();
	}
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::ReleaseBuffer()
{
	if ( NULL != this->m_ImageBuffer ) 
	{	JetMemory.free_func(m_ImageBuffer); }
	if ( TRUE==m_bClonePhase && NULL!=m_PhaseBuffer )
	{	
		JetMemory.free_func(m_MaskBuffer);	
		JetMemory.free_func(m_PhaseBuffer);			
	}
	if ( TRUE==m_bCloneSpace && NULL!=m_SpaceBuffer )
	{	
		JetMemory.free_func(m_MaskBuffer);	
		JetMemory.free_func(m_SpaceBuffer);	
	}
	
	this->m_ScaleMode = 1;
	this->m_bCloneSpace = FALSE;
	this->m_bClonePhase = FALSE;
	this->m_ImageW = 0;
	this->m_ImageH = 0;
	this->m_ImageStep = 0;
	this->m_PhaseStep = 0;
	this->m_SpaceStep = 0;
	this->m_BitCount = 0;	
	this->m_MaskBuffer = NULL;
	this->m_PhaseBuffer = NULL;	
	this->m_SpaceBuffer = NULL;
	this->m_PhaseAve = 0;
	this->m_SpaceAve = 0;
}
//-------------------------------------------------------------------------------------//
bool  CImagePhaseWnd::SetPhaseBuffer(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, int ScaleMode, BOOL bClone)
{
	const char fnName[] = "CImagePhaseWnd::SetPhaseBuffer";
	size_t i=0, j=0;
	size_t ImgIdx=0, PhsIdx=0;
	int    iMax=0, iMin=0, iValue=0;	
	const IMAGE_SIZE ImageW = PhaseW;
	const IMAGE_SIZE ImageH = PhaseH;
	const IMAGE_SIZE BitCount = 24;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);		
	const size_t PhaseSize = ImageAPI.CalcBufferSize(PhaseStep, PhaseH);
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);	

	this->ReleaseBuffer();
	if ( JetMemory.alloc_func(ImageSize, m_ImageBuffer, fnName, "m_ImageBuffer") == false )
	{	return false; }

	this->m_PhaseStep = PhaseStep;	
	if ( TRUE == bClone )
	{
		if ( NULL != MaskPtr )
		{
			if ( JetMemory.alloc_func(PhaseSize, m_MaskBuffer, fnName, "m_MaskBuffer") == false )
			{	return false; }
			::memcpy(m_MaskBuffer, MaskPtr, sizeof(MASK_DATA)*PhaseSize);
		}
		if ( JetMemory.alloc_func(PhaseSize, m_PhaseBuffer, fnName, "m_PhaseBuffer") == false )
		{
			JetMemory.free_func(m_MaskBuffer);
			return false; 
		}
		::memcpy(m_PhaseBuffer, PhasePtr, sizeof(PHASE_DATA)*PhaseSize);
	}
	else
	{	
		this->m_MaskBuffer = MaskPtr;	
		this->m_PhaseBuffer = PhasePtr;	
	}
	this->m_ScaleMode = ScaleMode;	
	this->m_bClonePhase = bClone;
	this->m_ImageW = ImageW;
	this->m_ImageH = ImageH;
	this->m_ImageStep = ImageStep;
	this->m_BitCount = 24;
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.0, m_ImageZoom);
	BuildPhaseImage();	
	DrawImageWndMemDC(); 
	return true;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::SetStartPos(int nX, int nY)
{
	m_StartPos.x = nX;
	m_StartPos.y = nY;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseWnd::SetSpaceBuffer(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ScaleMode, BOOL bClone)
{
	const char fnName[] = "CImagePhaseWnd::SetSpaceBuffer";
	size_t i=0, j=0;
	size_t ImgIdx=0, SpcIdx=0;
	double  dMax=0, dMin=0, dValue=0;	
	const IMAGE_SIZE ImageW = SpaceW;
	const IMAGE_SIZE ImageH = SpaceH;
	const IMAGE_SIZE BitCount = 24;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);	
	const size_t SpaceSize = ImageAPI.CalcBufferSize(SpaceStep, SpaceH);
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);	

	this->ReleaseBuffer();
	if ( JetMemory.alloc_func(ImageSize, m_ImageBuffer, fnName, "m_ImageBuffer") == false )
	{	return false; }

	this->m_SpaceStep = SpaceStep;
	if ( TRUE == bClone )
	{
		if ( NULL != MaskPtr )
		{
			if ( JetMemory.alloc_func(SpaceSize, m_MaskBuffer, fnName, "m_MaskBuffer") == false )			
			{	return false;	}
			::memcpy(m_MaskBuffer, MaskPtr, sizeof(MASK_DATA)*SpaceSize);
		}
		if ( JetMemory.alloc_func(SpaceSize, m_SpaceBuffer, fnName, "m_SpaceBuffer") == false )
		{	
			JetMemory.free_func(m_MaskBuffer);			
			return false; 
		}		
		::memcpy(m_SpaceBuffer, SpacePtr, sizeof(SPACE_DATA)*SpaceSize);
	}
	else
	{	
		this->m_MaskBuffer  = MaskPtr;
		this->m_SpaceBuffer = SpacePtr;
	}
	this->m_ScaleMode = ScaleMode;	
	this->m_bCloneSpace = bClone;
	this->m_ImageW = ImageW;
	this->m_ImageH = ImageH;
	this->m_ImageStep = ImageStep;
	this->m_BitCount = 24;
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, m_ImageWndRect, 1.0, m_ImageZoom);
	BuildSpaceImage();
	DrawImageWndMemDC(); 
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseWnd::BuildImage()
{
	if ( NULL != m_PhaseBuffer )
	{	CImagePhaseWnd::BuildPhaseImage();	}
	else if ( NULL != m_SpaceBuffer )
	{	CImagePhaseWnd::BuildSpaceImage();	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseWnd::BuildPhaseImage()
{
	if ( NULL == m_ImageBuffer ) { return false; }
	size_t i=0, j=0;
	size_t ImgIdx=0, PhsIdx=0;
	int    iMax=0, iMin=0, iValue=0;
	const IMAGE_SIZE PhaseW = m_ImageW;
	const IMAGE_SIZE PhaseH = m_ImageH;
	const IMAGE_SIZE PhaseStep = m_PhaseStep;
	const IMAGE_SIZE ImageW = PhaseW;
	const IMAGE_SIZE ImageH = PhaseH;
	const IMAGE_SIZE BitCount = 24;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);		
	const size_t PhaseSize = ImageAPI.CalcBufferSize(PhaseStep, PhaseH);
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);	
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF PhaseNoiseLowContrastColor = SysParam.m_PhaseNoiseLowContrastColor;
	COLORREF PhaseNoiseLowPotentialColor = SysParam.m_PhaseNoiseLowPotentialColor;
	COLORREF PhaseNoiseOverSaturatedColor = SysParam.m_PhaseNoiseOverSaturatedColor;
	COLORREF PhaseNoiseExtendVoidColor = SysParam.m_PhaseNoiseExtendVoidColor;
	COLORREF SpaceNoiseHeightUnexpectedColor = SysParam.m_SpaceNoiseHeightUnexpectedColor;
	COLORREF SpaceNoiseOverLowColor = SysParam.m_SpaceNoiseHeightOverLowColor;		
	COLORREF SpaceBestValidColor = SysParam.m_SpaceBestValidColor;	
	const unsigned char LowContrastClrRed   = GetRValue(PhaseNoiseLowContrastColor);
	const unsigned char LowContrastClrGreen = GetGValue(PhaseNoiseLowContrastColor);
	const unsigned char LowContrastClrBlue  = GetBValue(PhaseNoiseLowContrastColor);
	const unsigned char LowPotentialClrRed   = GetRValue(PhaseNoiseLowPotentialColor);
	const unsigned char LowPotentialClrGreen = GetGValue(PhaseNoiseLowPotentialColor);
	const unsigned char LowPotentialClrBlue  = GetBValue(PhaseNoiseLowPotentialColor);
	const unsigned char OverSaturatedClrRed   = GetRValue(PhaseNoiseOverSaturatedColor);
	const unsigned char OverSaturatedClrGreen = GetGValue(PhaseNoiseOverSaturatedColor);
	const unsigned char OverSaturatedClrBlue  = GetBValue(PhaseNoiseOverSaturatedColor);
	const unsigned char ExtendVoidClrRed   = GetRValue(PhaseNoiseExtendVoidColor);
	const unsigned char ExtendVoidClrGreen = GetGValue(PhaseNoiseExtendVoidColor);
	const unsigned char ExtendVoidClrBlue  = GetBValue(PhaseNoiseExtendVoidColor);
	const unsigned char HeightUnexpectedClrRed   = GetRValue(SpaceNoiseHeightUnexpectedColor);
	const unsigned char HeightUnexpectedClrGreen = GetGValue(SpaceNoiseHeightUnexpectedColor);
	const unsigned char HeightUnexpectedClrBlue  = GetBValue(SpaceNoiseHeightUnexpectedColor);
	const unsigned char HeightOverLowClrRed   = GetRValue(SpaceNoiseOverLowColor);
	const unsigned char HeightOverLowClrGreen = GetGValue(SpaceNoiseOverLowColor);
	const unsigned char HeightOverLowClrBlue  = GetBValue(SpaceNoiseOverLowColor);
	const unsigned char BestValidClrRed   = GetRValue(SpaceBestValidColor);
	const unsigned char BestValidClrGreen = GetGValue(SpaceBestValidColor);
	const unsigned char BestValidClrBlue  = GetBValue(SpaceBestValidColor);

	const int ScaleMode = m_ScaleMode;
	MASK_PTR  MaskPtr  = m_MaskBuffer;
	PHASE_PTR PhasePtr = m_PhaseBuffer;
	if ( PHASE_TO_IMAGE_DYNAMIC_SCALE == ScaleMode )
	{
		iMin = INT_MAX;
		iMax = INT_MIN;
		m_PhaseAve  = 0;
		for ( i=0; i<PhaseSize; i++ )
		{
			if ( NULL != MaskPtr )
			{
				if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[i]) == false ) { continue; }
			}
			if ( iMin > PhasePtr[i] ) { iMin = PhasePtr[i]; }
			if ( iMax < PhasePtr[i] ) { iMax = PhasePtr[i]; }
			m_PhaseAve += PhasePtr[i];
		}
		if ( PhaseSize > 0 ) 
		{	m_PhaseAve /= PhaseSize; }

		const double dRange = iMax-iMin;
		const double dFactor = 255/dRange;
		for ( i=0; i<PhaseH; i++ )
		{
			PhsIdx = i*PhaseStep;
			ImgIdx = i*ImageStep;			
			for ( j=0; j<PhaseW; j++ )
			{
				if ( NULL != MaskPtr )
				{
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_LOW_CONTRAST) != 0 ) 
					{
						m_ImageBuffer[ImgIdx]   = LowContrastClrBlue;
						m_ImageBuffer[ImgIdx+1] = LowContrastClrGreen;
						m_ImageBuffer[ImgIdx+2] = LowContrastClrRed;
						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_LOW_POTENTIAL) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx]   = LowPotentialClrBlue;
						m_ImageBuffer[ImgIdx+1] = LowPotentialClrGreen;
						m_ImageBuffer[ImgIdx+2] = LowPotentialClrRed;
						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_OVER_SATURATED) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = OverSaturatedClrBlue;
						m_ImageBuffer[ImgIdx+1] = OverSaturatedClrGreen;
						m_ImageBuffer[ImgIdx+2] = OverSaturatedClrRed;
						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_EXTEND_VOID) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = ExtendVoidClrBlue;
						m_ImageBuffer[ImgIdx+1] = ExtendVoidClrGreen;
						m_ImageBuffer[ImgIdx+2] = ExtendVoidClrRed;
						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_HEIGHT_UNEXPECTED) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = HeightUnexpectedClrBlue;
						m_ImageBuffer[ImgIdx+1] = HeightUnexpectedClrGreen;
						m_ImageBuffer[ImgIdx+2] = HeightUnexpectedClrRed;
						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_OVER_LOW) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = HeightOverLowClrBlue;
						m_ImageBuffer[ImgIdx+1] = HeightOverLowClrGreen;
						m_ImageBuffer[ImgIdx+2] = HeightOverLowClrRed;
						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}					
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_VALID_BEST) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = BestValidClrBlue;
						m_ImageBuffer[ImgIdx+1] = BestValidClrGreen;
						m_ImageBuffer[ImgIdx+2] = BestValidClrRed;
						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
				}

				iValue = PhasePtr[PhsIdx];
				iValue -= iMin;
				iValue  = (int)(iValue*dFactor);
				m_ImageBuffer[ImgIdx] = m_ImageBuffer[ImgIdx+1] = m_ImageBuffer[ImgIdx+2] = static_cast<unsigned char>(iValue);		
				PhsIdx ++;
				ImgIdx += 3;
			}	
		}
	}
	else
	{
		double dFactor = 256.0;
		m_PhaseAve = 0;
		dFactor = dFactor/PHASE_MAX;		
		for ( i=0; i<PhaseH; i++ )
		{
			PhsIdx = i*PhaseStep;
			ImgIdx = i*ImageStep;			
			for ( j=0; j<PhaseW; j++ )
			{
				m_PhaseAve += PhasePtr[PhsIdx];
				if ( NULL != MaskPtr )
				{
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_LOW_CONTRAST) != 0 ) 
					{
						m_ImageBuffer[ImgIdx]   = LowContrastClrBlue;
						m_ImageBuffer[ImgIdx+1] = LowContrastClrGreen;
						m_ImageBuffer[ImgIdx+2] = LowContrastClrRed;

						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_LOW_POTENTIAL) != 0 ) 
					{
						m_ImageBuffer[ImgIdx]   = LowPotentialClrBlue;
						m_ImageBuffer[ImgIdx+1] = LowPotentialClrGreen;
						m_ImageBuffer[ImgIdx+2] = LowPotentialClrRed;

						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_OVER_SATURATED) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx]   = OverSaturatedClrBlue;
						m_ImageBuffer[ImgIdx+1] = OverSaturatedClrGreen;
						m_ImageBuffer[ImgIdx+2] = OverSaturatedClrRed;

						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_EXTEND_VOID) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx]   = ExtendVoidClrBlue;
						m_ImageBuffer[ImgIdx+1] = ExtendVoidClrGreen;
						m_ImageBuffer[ImgIdx+2] = ExtendVoidClrRed;

						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[PhsIdx]&PHASE_MASK_HEIGHT_UNEXPECTED) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx]   = HeightUnexpectedClrBlue;
						m_ImageBuffer[ImgIdx+1] = HeightUnexpectedClrGreen;
						m_ImageBuffer[ImgIdx+2] = HeightUnexpectedClrRed;

						PhsIdx ++;
						ImgIdx += 3;
						continue; 
					}
				}

				iValue = PhasePtr[PhsIdx];
				if ( iValue < 0 ) 
				{	iValue -= PHASE_MIN; }
				iValue = (int)(iValue*dFactor);
				m_ImageBuffer[ImgIdx] = m_ImageBuffer[ImgIdx+1] = m_ImageBuffer[ImgIdx+2] = static_cast<unsigned char>(iValue);
				//m_ImageBuffer[i] = static_cast<unsigned char>(iValue>>8);		
				PhsIdx ++;
				ImgIdx += 3;
			}
		}
		if ( PhaseSize > 0 ) 
		{	m_PhaseAve /= PhaseSize; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseWnd::BuildSpaceImage()
{	
	if ( NULL == m_ImageBuffer ) { return false; }

	size_t i=0, j=0;
	size_t ImgIdx=0, SpcIdx=0;
	double  dMax=0, dMin=0, dValue=0;	
	const IMAGE_SIZE SpaceW = m_ImageW;
	const IMAGE_SIZE SpaceH = m_ImageH;
	const IMAGE_SIZE SpaceStep = m_SpaceStep;
	const IMAGE_SIZE ImageW = m_ImageW;
	const IMAGE_SIZE ImageH = m_ImageH;	
	const IMAGE_SIZE BitCount = 24;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);	
	const size_t SpaceSize = ImageAPI.CalcBufferSize(SpaceStep, ImageH);
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);	
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF PhaseNoiseLowContrastColor = SysParam.m_PhaseNoiseLowContrastColor;
	COLORREF PhaseNoiseLowPotentialColor = SysParam.m_PhaseNoiseLowPotentialColor;
	COLORREF PhaseNoiseOverSaturatedColor = SysParam.m_PhaseNoiseOverSaturatedColor;
	COLORREF PhaseNoiseExtendVoidColor = SysParam.m_PhaseNoiseExtendVoidColor;
	COLORREF SpaceNoiseHeightUnexpectedColor = SysParam.m_SpaceNoiseHeightUnexpectedColor;
	COLORREF SpaceNoiseOverLowColor = SysParam.m_SpaceNoiseHeightOverLowColor;
	COLORREF SpaceBestValidColor = SysParam.m_SpaceBestValidColor;	
	const unsigned char LowContrastClrRed   = GetRValue(PhaseNoiseLowContrastColor);
	const unsigned char LowContrastClrGreen = GetGValue(PhaseNoiseLowContrastColor);
	const unsigned char LowContrastClrBlue  = GetBValue(PhaseNoiseLowContrastColor);
	const unsigned char LowPotentialClrRed   = GetRValue(PhaseNoiseLowPotentialColor);
	const unsigned char LowPotentialClrGreen = GetGValue(PhaseNoiseLowPotentialColor);
	const unsigned char LowPotentialClrBlue  = GetBValue(PhaseNoiseLowPotentialColor);
	const unsigned char OverSaturatedClrRed   = GetRValue(PhaseNoiseOverSaturatedColor);
	const unsigned char OverSaturatedClrGreen = GetGValue(PhaseNoiseOverSaturatedColor);
	const unsigned char OverSaturatedClrBlue  = GetBValue(PhaseNoiseOverSaturatedColor);
	const unsigned char ExtendVoidClrRed   = GetRValue(PhaseNoiseExtendVoidColor);
	const unsigned char ExtendVoidClrGreen = GetGValue(PhaseNoiseExtendVoidColor);
	const unsigned char ExtendVoidClrBlue  = GetBValue(PhaseNoiseExtendVoidColor);
	const unsigned char HeightUnexpectedClrRed   = GetRValue(SpaceNoiseHeightUnexpectedColor);
	const unsigned char HeightUnexpectedClrGreen = GetGValue(SpaceNoiseHeightUnexpectedColor);
	const unsigned char HeightUnexpectedClrBlue  = GetBValue(SpaceNoiseHeightUnexpectedColor);
	const unsigned char HeightOverLowClrRed   = GetRValue(SpaceNoiseOverLowColor);
	const unsigned char HeightOverLowClrGreen = GetGValue(SpaceNoiseOverLowColor);
	const unsigned char HeightOverLowClrBlue  = GetBValue(SpaceNoiseOverLowColor);
	const unsigned char BestValidClrRed   = GetRValue(SpaceBestValidColor);
	const unsigned char BestValidClrGreen = GetGValue(SpaceBestValidColor);
	const unsigned char BestValidClrBlue  = GetBValue(SpaceBestValidColor);
	const float         SingleCastLowLimit = SysParam.m_SpaceNoiseSingleCastLowLimit;

	const int ScaleMode = m_ScaleMode;
	MASK_PTR  MaskPtr = m_MaskBuffer;
	SPACE_PTR SpacePtr = m_SpaceBuffer;	
	if ( PHASE_TO_IMAGE_DYNAMIC_SCALE == ScaleMode )
	{
		dMin = DBL_MAX;
		dMax = DBL_MIN;
		m_SpaceAve = 0;
		for ( i=0; i<SpaceSize; i++ )
		{
			if ( NULL != MaskPtr )
			{
				if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[i]) == false ) { continue; }
			}
			if ( dMin > SpacePtr[i] ) { dMin = SpacePtr[i]; }
			if ( dMax < SpacePtr[i] ) { dMax = SpacePtr[i]; }
			m_SpaceAve  += SpacePtr[i];
		}
		if ( SpaceSize > 0 ) 
		{	m_SpaceAve /= SpaceSize; }

		const double dRange = dMax-dMin;
		const double dFactor = 255/dRange;
		for ( i=0; i<SpaceH; i++ )
		{
			SpcIdx = i*SpaceStep;
			ImgIdx = i*ImageStep;			
			for ( j=0; j<SpaceW; j++ )
			{
				dValue = SpacePtr[SpcIdx];
				dValue -= dMin;
				dValue  = (dValue*dFactor);

				if ( NULL != MaskPtr )
				{
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_LOW_CONTRAST) != 0 ) 
					{
						m_ImageBuffer[ImgIdx]   = LowContrastClrBlue;
						m_ImageBuffer[ImgIdx+1] = LowContrastClrGreen;
						m_ImageBuffer[ImgIdx+2] = LowContrastClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_LOW_POTENTIAL) != 0 ) 
					{
						m_ImageBuffer[ImgIdx]   = LowPotentialClrBlue;
						m_ImageBuffer[ImgIdx+1] = LowPotentialClrGreen;
						m_ImageBuffer[ImgIdx+2] = LowPotentialClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_OVER_SATURATED) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = OverSaturatedClrBlue;
						m_ImageBuffer[ImgIdx+1] = OverSaturatedClrGreen;
						m_ImageBuffer[ImgIdx+2] = OverSaturatedClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_EXTEND_VOID) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = ExtendVoidClrBlue;
						m_ImageBuffer[ImgIdx+1] = ExtendVoidClrGreen;
						m_ImageBuffer[ImgIdx+2] = ExtendVoidClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_HEIGHT_UNEXPECTED) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = HeightUnexpectedClrBlue;
						m_ImageBuffer[ImgIdx+1] = HeightUnexpectedClrGreen;
						m_ImageBuffer[ImgIdx+2] = HeightUnexpectedClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_OVER_LOW) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = HeightOverLowClrBlue;
						m_ImageBuffer[ImgIdx+1] = HeightOverLowClrGreen;
						m_ImageBuffer[ImgIdx+2] = HeightOverLowClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_VALID_BEST) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = BestValidClrBlue;
						m_ImageBuffer[ImgIdx+1] = BestValidClrGreen;
						m_ImageBuffer[ImgIdx+2] = BestValidClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
				}
				/*
				if ( SpacePtr[SpcIdx] < SingleCastLowLimit ) 
				{
					m_ImageBuffer[ImgIdx] = HeightOverLowClrBlue;
					m_ImageBuffer[ImgIdx+1] = HeightOverLowClrGreen;
					m_ImageBuffer[ImgIdx+2] = HeightOverLowClrRed;
					SpcIdx ++;
					ImgIdx += 3;
					continue; 
				}
				*/
				m_ImageBuffer[ImgIdx] = m_ImageBuffer[ImgIdx+1] = m_ImageBuffer[ImgIdx+2] = static_cast<unsigned char>(dValue);		

				SpcIdx ++;
				ImgIdx += 3;
			}	
		}
	}
	else
	{
		m_SpaceAve = 0;
		const double dFactor = 255.0/4000.0;
		for ( i=0; i<SpaceH; i++ )
		{
			SpcIdx = i*SpaceStep;
			ImgIdx = i*ImageStep;			
			for ( j=0; j<SpaceW; j++ )
			{
				dValue = SpacePtr[SpcIdx];			
				m_SpaceAve += dValue;

				if ( NULL != MaskPtr )
				{
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_LOW_CONTRAST) != 0 ) 
					{
						m_ImageBuffer[ImgIdx]   = LowContrastClrBlue;
						m_ImageBuffer[ImgIdx+1] = LowContrastClrGreen;
						m_ImageBuffer[ImgIdx+2] = LowContrastClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_LOW_POTENTIAL) != 0 ) 
					{
						m_ImageBuffer[ImgIdx]   = LowPotentialClrBlue;
						m_ImageBuffer[ImgIdx+1] = LowPotentialClrGreen;
						m_ImageBuffer[ImgIdx+2] = LowPotentialClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_OVER_SATURATED) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = OverSaturatedClrBlue;
						m_ImageBuffer[ImgIdx+1] = OverSaturatedClrGreen;
						m_ImageBuffer[ImgIdx+2] = OverSaturatedClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_EXTEND_VOID) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = ExtendVoidClrBlue;
						m_ImageBuffer[ImgIdx+1] = ExtendVoidClrGreen;
						m_ImageBuffer[ImgIdx+2] = ExtendVoidClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
					if ( (MaskPtr[SpcIdx]&PHASE_MASK_HEIGHT_UNEXPECTED) != 0 ) 
					{ 
						m_ImageBuffer[ImgIdx] = HeightUnexpectedClrBlue;
						m_ImageBuffer[ImgIdx+1] = HeightUnexpectedClrGreen;
						m_ImageBuffer[ImgIdx+2] = HeightUnexpectedClrRed;
						SpcIdx ++;
						ImgIdx += 3;
						continue; 
					}
				}
				m_ImageBuffer[ImgIdx] = m_ImageBuffer[ImgIdx+1] = m_ImageBuffer[ImgIdx+2] = static_cast<unsigned char>(dValue*dFactor);		

				SpcIdx ++;
				ImgIdx += 3;

			}
		}
		if ( SpaceSize > 0 ) 
		{	m_SpaceAve /= SpaceSize; }
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::DrawImageWndMemDC()
{	
	HDC hDC = m_ImageWndMemDC.GetSafeHdc();	
	if ( NULL==m_ImageBuffer || NULL==hDC ) { return; }	
	COLORREF clrBK = 0xAFFFFF;
	HBRUSH hBrush = ::CreateSolidBrush(clrBK);
	if ( NULL != hBrush )
	{
		::FillRect(hDC, &m_ImageWndRect, hBrush);
		::DeleteObject(hBrush); hBrush = NULL;
	}	
	ImageAPI.DrawImageToDC(hDC, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, m_ImageWndRect, m_ImageOffset, m_ImageZoom, clrBK);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::RedrawWnd()
{
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CString  str;	
	CClientDC dc(&m_ImageWnd);	
	HDC hDC = dc.GetSafeHdc();	
	HDC hBKDC = m_ImageWndMemDC.GetSafeHdc();		
	::IntersectClipRect(hDC, this->m_ImageWndRect.left, this->m_ImageWndRect.top, m_ImageWndRect.right, m_ImageWndRect.bottom);	

	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	::BitBlt(hDC, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hBKDC, 0, 0, SRCCOPY );
	if ( NULL != m_PhaseBuffer )
	{
		CString str;
		str.Format(_T("Ave:%.2f"), m_PhaseAve );
		::TextOut(hDC, 0, 0, str, str.GetLength());
	}	
	if ( NULL != m_SpaceBuffer )
	{
		CString str;
		str.Format(_T("Ave:%.2f"), m_SpaceAve );
		::TextOut(hDC, 0, 0, str, str.GetLength());
	}	

	CImagePhaseWnd::DrawNoiseColorWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::DrawNoiseColorWnd()
{	
	UINT CtrlID = 0;
	COLORREF     clr = 0;
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();

	CtrlID = IMGPHASE_NOISE_LOW_CONTRAST_CLR;	
	clr = SysParam.m_PhaseNoiseLowContrastColor;
	DrawNoiseColorWnd(CtrlID, clr);

	CtrlID = IMGPHASE_NOISE_LOW_POTENTIAL_CLR;	
	clr = SysParam.m_PhaseNoiseLowPotentialColor;
	DrawNoiseColorWnd(CtrlID, clr);

	CtrlID = IMGPHASE_NOISE_OVER_SATURATED_CLR;	
	clr = SysParam.m_PhaseNoiseOverSaturatedColor;
	DrawNoiseColorWnd(CtrlID, clr);

	CtrlID = IMGPHASE_NOISE_VOID_EXTENDED_CLR;	
	clr = SysParam.m_PhaseNoiseExtendVoidColor;
	DrawNoiseColorWnd(CtrlID, clr);

	CtrlID = IMGPHASE_NOISE_HEIGHT_UNEXPECTED_CLR;	
	clr = SysParam.m_SpaceNoiseHeightUnexpectedColor;
	DrawNoiseColorWnd(CtrlID, clr);

	CtrlID = IMGPHASE_NOISE_OVER_LOW_CLR;	
	clr = SysParam.m_SpaceNoiseHeightOverLowColor;
	DrawNoiseColorWnd(CtrlID, clr);

	CtrlID = IMGPHASE_VALID_BEST_CLR;	
	clr = SysParam.m_SpaceBestValidColor;
	DrawNoiseColorWnd(CtrlID, clr);
	return;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::DrawNoiseColorWnd(UINT CtrlID, COLORREF clr)
{
	CWnd *pWnd = CWnd::GetDlgItem(CtrlID);
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )	{	return; }

	RECT      WndRect;
	CClientDC dc(pWnd);
	HDC hDC = dc.GetSafeHdc();
	if ( NULL == hDC ) { return; }
	pWnd->GetClientRect(&WndRect);
	::InflateRect(&WndRect, -1, -1);
	HBRUSH hBrush = ::CreateSolidBrush(clr);
	if ( NULL == hBrush ) { return; }
	::FillRect(hDC, &WndRect, hBrush);
	::DeleteObject(hBrush); hBrush = NULL;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	this->m_LBtnUpPos = point;
	this->m_LBtnDownPos = this->m_MovingPos = this->m_LBtnUpPos;
	this->SetCapture();
	
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, IMGPHASE_IMAGE_WND, &pt) == true )
	{	this->m_ImageWndPt1 = pt;	}
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, IMGPHASE_IMAGE_WND, &pt) == true )
	{	this->m_ImageWndPt2 = pt;	}

	this->m_LBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;
	this->RedrawWnd();
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->m_RBtnUpPos = point;
	this->m_RBtnDownPos = this->m_MovingPos = this->m_RBtnUpPos;
	this->SetCapture();
	CBaseDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	POINT pt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, pt, IMGPHASE_IMAGE_WND, &pt)== true )
	{	this->m_ImageWndPt2 = pt;	}

	this->m_RBtnUpPos = this->m_MovingPos = point;
	this->m_MovingPos.x = this->m_MovingPos.y = -1;
	this->m_LBtnUpPos = this->m_LBtnDownPos = this->m_MovingPos;
	this->RedrawWnd();
	CBaseDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT WndPt = point;
	if ( JetAPI::CheckPtInCtrlWnd(this, WndPt, IMGPHASE_IMAGE_WND, &WndPt) == true )
	{			
		int      R=0, G=0, B=0;
		size_t   Imgidx=0, PhsIdx=0, SpcIdx=0;
		double   value=0;
		POINT    ImagePos={0};
		CString  strPixel;
		TPOINT2D ImagePt=WndPt;
		TPOINT2D WndPt2 =WndPt;
		const IMAGE_SIZE ImageW = this->m_ImageW;
		const IMAGE_SIZE ImageH = this->m_ImageH;
		const int nImageW = (int)(ImageW);
		const int nImageH = (int)(ImageH);
		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, ImagePt, WndPt2);
		
		JetAPI::Point2DToPoint(ImagePt, ImagePos);
		if ( NULL==m_ImageBuffer || ImagePos.x<0 || ImagePos.y<0 || ImagePos.x>=nImageW || ImagePos.y>=nImageH )
		{	strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Roi(%d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, ImagePos.x+m_StartPos.x, ImagePos.y+m_StartPos.y); }
		else if ( 8 == m_BitCount )
		{					
			Imgidx = (ImagePos.y*m_ImageStep)+(ImagePos.x);			
			R = G = B = m_ImageBuffer[Imgidx];			
			if ( NULL != m_PhaseBuffer )
			{
				PhsIdx = (ImagePos.y*m_PhaseStep)+(ImagePos.x);
				value = m_PhaseBuffer[PhsIdx];			
				strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Roi(%d, %d), Phase=%.2f, RGB=(%d, %d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, ImagePos.x+m_StartPos.x, ImagePos.y+m_StartPos.y, value, R, G, B);
			}
			if ( NULL != m_SpaceBuffer )
			{
				SpcIdx = (ImagePos.y*m_SpaceStep)+(ImagePos.x);
				value = m_SpaceBuffer[SpcIdx];			
				strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Roi(%d, %d), Value=%.2f, RGB=(%d, %d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, ImagePos.x+m_StartPos.x, ImagePos.y+m_StartPos.y, value, R, G, B);
			}
		}
		else if ( 24 == m_BitCount )
		{	
			Imgidx = (ImagePos.y*m_ImageStep)+(ImagePos.x*3);
			B = m_ImageBuffer[Imgidx]; 
			G = m_ImageBuffer[Imgidx+1]; 
			R = m_ImageBuffer[Imgidx+2]; 

			if ( NULL != m_PhaseBuffer )
			{
				PhsIdx = (ImagePos.y*m_PhaseStep)+(ImagePos.x);
				value = m_PhaseBuffer[PhsIdx];			
				strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Roi(%d, %d), Phase=%.2f, RGB=(%d, %d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, ImagePos.x+m_StartPos.x, ImagePos.y+m_StartPos.y, value, R, G, B);
			}
			if ( NULL != m_SpaceBuffer )
			{
				SpcIdx = (ImagePos.y*m_SpaceStep)+(ImagePos.x);
				value = m_SpaceBuffer[SpcIdx];			
				strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Roi(%d, %d), Value=%.2f, RGB=(%d, %d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, ImagePos.x+m_StartPos.x, ImagePos.y+m_StartPos.y, value, R, G, B);
			}
		}
		else
		{	strPixel.Format(_T("Pos(%d, %d), Image(%.2f, %.2f), Roi(%d, %d)"), WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, ImagePos.x+m_StartPos.x, ImagePos.y+m_StartPos.y); }
		this->SetDlgItemText(IMGPHASE_INFO_EDIT, strPixel);
	}

	if ( this != GetCapture() ) 
	{
		CBaseDialog::OnMouseMove(nFlags, point);
		return; 
	}	

	POINT pt  = point;
	if ( nFlags&MK_LBUTTON )
	{
		if ( JetAPI::CheckPtInCtrlWnd(this, pt, IMGPHASE_IMAGE_WND, &pt) == true )
		{	this->m_ImageWndPt2 = (pt);	}
	}
	else if ( nFlags&MK_RBUTTON )
	{
		m_ImageOffset.x += point.x-m_MovingPos.x;
		m_ImageOffset.y += point.y-m_MovingPos.y;
		m_MovingPos = point;
		DrawImageWndMemDC();
	}
	this->RedrawWnd();
	CBaseDialog::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseWnd::ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( WM_MOUSEWHEEL != message ) { return false; }
	CWnd *pWnd = GetFocus();
	if ( NULL == pWnd ) { return false; }
	if ( this != pWnd )
	{	pWnd = pWnd->GetParent();	}	
	if ( this != pWnd ) { return false; }

	CPoint pt, point;
	point.x = pt.x = GET_X_LPARAM(lParam); 
	point.y = pt.y = GET_Y_LPARAM(lParam); 
	this->ScreenToClient(&point);
	if ( JetAPI::CheckPtInCtrlWnd(this, point, IMGPHASE_IMAGE_WND, NULL) == false ) 
	{	return false; }

	UINT nFlags = GET_KEYSTATE_WPARAM(wParam);
	short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);	
	if ( ExecMouseWheelEvent(nFlags, zDelta, pt) == true )
	{	return TRUE; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImagePhaseWnd::ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt)
{
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);	
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;	

	DrawImageWndMemDC();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
BOOL CImagePhaseWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CBaseDialog::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
	this->RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default	
	CBaseDialog::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 200;
	lpMMI->ptMinTrackSize.y = 200;
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnSaveBtn() 
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_ImageBuffer ) { return; }

	TCHAR szFilters[]=_T("BMP Files (*.BMP)|*.BMP|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("BMP"), _T("*.BMP"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }

	CString filename = dialog.GetPathName();
	if ( ImageAPI.SaveBMPImage(filename, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImageBuffer, true) == false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnNoiseLowContrastBtn() 
{
	// TODO: Add your control notification handler code here			
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF clr = SysParam.m_PhaseNoiseLowContrastColor;	
	CColorDialog Wnd(clr);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }
	clr = Wnd.GetColor();
	SysParam.m_PhaseNoiseLowContrastColor = clr;	
	BuildImage();
	DrawImageWndMemDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnNoiseLowPotentialBtn() 
{
	// TODO: Add your control notification handler code here		
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF     clr = SysParam.m_PhaseNoiseLowPotentialColor;
	CColorDialog Wnd(clr);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }
	clr = Wnd.GetColor();
	SysParam.m_PhaseNoiseLowPotentialColor = clr;	
	BuildImage();
	DrawImageWndMemDC();
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnNoiseOverSaturatedBtn() 
{
	// TODO: Add your control notification handler code here
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF     clr = SysParam.m_PhaseNoiseOverSaturatedColor;
	CColorDialog Wnd(clr);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }
	clr = Wnd.GetColor();
	SysParam.m_PhaseNoiseOverSaturatedColor = clr;	
	BuildImage();
	DrawImageWndMemDC();
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnNoiseVoidExtendedBtn() 
{
	// TODO: Add your control notification handler code here	
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF     clr = SysParam.m_PhaseNoiseExtendVoidColor;
	CColorDialog Wnd(clr);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }
	clr = Wnd.GetColor();
	SysParam.m_PhaseNoiseExtendVoidColor = clr;	
	BuildImage();
	DrawImageWndMemDC();
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnNoiseHeightUnexpectedBtn() 
{
	// TODO: Add your control notification handler code here	
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF     clr = SysParam.m_SpaceNoiseHeightUnexpectedColor;
	CColorDialog Wnd(clr);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }
	clr = Wnd.GetColor();
	SysParam.m_SpaceNoiseHeightUnexpectedColor = clr;	
	BuildImage();
	DrawImageWndMemDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnNoiseOverLowBtn() 
{
	// TODO: Add your control notification handler code here
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF     clr = SysParam.m_SpaceNoiseHeightOverLowColor;
	CColorDialog Wnd(clr);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }
	clr = Wnd.GetColor();
	SysParam.m_SpaceNoiseHeightOverLowColor = clr;	
	BuildImage();
	DrawImageWndMemDC();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CImagePhaseWnd::OnValidBestBtn() 
{
	// TODO: Add your control notification handler code here
	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	COLORREF     clr = SysParam.m_SpaceBestValidColor;
	CColorDialog Wnd(clr);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }
	clr = Wnd.GetColor();
	SysParam.m_SpaceBestValidColor = clr;	
	BuildImage();
	DrawImageWndMemDC();
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//