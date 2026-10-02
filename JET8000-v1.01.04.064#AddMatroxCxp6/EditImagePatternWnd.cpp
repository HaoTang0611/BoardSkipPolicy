// EditImagePatternWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "EditImagePatternWnd.h"
//-------------------------------------------------------------------------------------//
#include "AlgPatternEditWnd.h"
#include "AlgPatternListWnd.h"
#include "AlgPatternTextWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditImagePatternWnd dialog
//-------------------------------------------------------------------------------------//
CEditImagePatternWnd::CEditImagePatternWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CEditImagePatternWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditImagePatternWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WndPtr = NULL;	
	m_BkColor = 0x000000;
	m_PatternIndex = 0;
	m_PatternCount = 0;		
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditImagePatternWnd)
	DDX_Control(pDX, PATTERN_INDEX_SPIN, m_IndexSpin);
	DDX_Control(pDX, PATTERN_IMAGE_WND, m_ImageWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditImagePatternWnd, CDialog)
	//{{AFX_MSG_MAP(CEditImagePatternWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(PATTERN_ADD_BTN, OnAddBtn)
	ON_BN_CLICKED(PATTERN_DELETE_BTN, OnDeleteBtn)
	ON_BN_CLICKED(PATTERN_CLEAR_BTN, OnClearBtn)
	ON_BN_CLICKED(PATTERN_TEXT_BTN, OnTextBtn)
	ON_BN_CLICKED(PATTERN_ADVANCE_BTN, OnAdvanceBtn)
	ON_WM_PAINT()
	ON_NOTIFY(UDN_DELTAPOS, PATTERN_INDEX_SPIN, OnDeltaposIndexSpin)
	ON_BN_CLICKED(PATTERN_BINARY_CHK, OnBinaryChk)
	ON_BN_CLICKED(PATTERN_GRAY_CHK, OnGrayChk)
	ON_BN_CLICKED(PATTERN_SHOW_INFO_CHK, OnShowInfoChk)
	ON_BN_CLICKED(PATTERN_EDIT_BTN, OnEditBtn)
	ON_BN_CLICKED(PATTERN_UPDATE_BIN_TO_ALG_BTN, OnUpdateBinToAlgBtn)
	ON_BN_CLICKED(PATTERN_UPDATE_BIN_TO_PAT_BTN, OnUpdateBinToPatBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImagePatternWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditImagePatternWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_BkColor = 0x000000;
	m_MemDC.CreateMemDC(&m_ImageWnd, m_BkColor);
	m_IndexSpin.SetRange((short)0, (short)(0));
	CWnd::CheckDlgButton(PATTERN_SHOW_INFO_CHK, TRUE);
#ifndef PATTERN_BINARY_USE
	JetAPI::ShowCtrlWnd(this, PATTERN_UPDATE_BIN_TO_ALG_BTN, FALSE);
	JetAPI::ShowCtrlWnd(this, PATTERN_UPDATE_BIN_TO_PAT_BTN, FALSE);
#endif//PATTERN_BINARY_USE
	SwitchMultiLanguage();
	UpdateParamToUI(NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	return;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_IMAGE_PATTERN_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_IMAGE_PATTERN_WND;
	WndKey = _T("IDD_EDIT_IMAGE_PATTERN_WND");
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
	WndID = PATTERN_NUMBER_LABEL;
	WndKey = _T("PATTERN_NUMBER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_INDEX_LABEL;
	WndKey = _T("PATTERN_INDEX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_RESULT_LABEL;
	WndKey = _T("PATTERN_RESULT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_POLARITY_LABEL;
	WndKey = _T("PATTERN_POLARITY_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = PATTERN_GRAY_CHK;
	WndKey = _T("PATTERN_GRAY_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_BINARY_CHK;
	WndKey = _T("PATTERN_BINARY_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_SHOW_INFO_CHK;
	WndKey = _T("PATTERN_SHOW_INFO_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_ADD_BTN;
	WndKey = _T("PATTERN_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_EDIT_BTN;
	WndKey = _T("PATTERN_EDIT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_DELETE_BTN;
	WndKey = _T("PATTERN_DELETE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_CLEAR_BTN;
	WndKey = _T("PATTERN_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_TEXT_BTN;
	WndKey = _T("PATTERN_TEXT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = PATTERN_ADVANCE_BTN;
	WndKey = _T("PATTERN_ADVANCE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//			
	WndID = PATTERN_UPDATE_BIN_TO_ALG_BTN;
	WndKey = _T("PATTERN_UPDATE_BIN_TO_ALG_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);		

	WndID = PATTERN_UPDATE_BIN_TO_PAT_BTN;
	WndKey = _T("PATTERN_UPDATE_BIN_TO_PAT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//				
}
//-------------------------------------------------------------------------------------//
CString CEditImagePatternWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_IMAGE_PATTERN_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::ChangeDrawModelMode()
{
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
BOOL CEditImagePatternWnd::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CEditImagePatternWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	//switch ( message )
	//{
	//}
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnAddBtn() 
{
	// TODO: Add your control notification handler code here
	ExecAddPattern();
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	ExecDelPattern();
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnClearBtn() 
{
	// TODO: Add your control notification handler code here
	ExecClearPattern();
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnTextBtn()
{
	// TODO: Add your control notification handler code here
	ExecTextSetting();	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnAdvanceBtn() 
{
	// TODO: Add your control notification handler code here
	ExecAdvanceSetting();	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::ResetWnd()
{
	m_WndPtr = NULL;	
	m_WndResultID = RESULT_ID_NONE;
	m_PatternIndex = -1;
	m_PatternCount = 0;	
	m_PatternParam = CPatternParam();
	m_PatternRoiList.clear();
	m_Dib.ReleaseBuffer();		
	if ( CWnd::GetSafeHwnd() != NULL )
	{
		UpdateParamToUI(NULL);
		m_IndexSpin.SetRange((short)0, (short)(0));
		CWnd::CheckDlgButton(PATTERN_SHOW_INFO_CHK, TRUE);
		CreateBKImage();
		RedrawWnd();
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::SetImagePatternWndPtr(CAOIWnd *WndPtr, bool UpdateToUI)
{
	if ( NULL == WndPtr )
	{
		ResetWnd();		
		return;
	}
	
	//if ( CEditImagePatternWnd::m_WndPtr == WndPtr ) { return; }
	m_WndPtr = WndPtr;			
	CPatternParam *PatParamPtr = NULL;		
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	m_WndResultID = AlgParam.GetAlgResultID();
	m_PatternCount = AlgParam.GetAlgPatternCount();
	m_PatternIndex = AlgParam.GetAlgPatternResultIndex();		
	PatParamPtr = AlgParam.GetAlgPatternParamPtr(m_PatternIndex, true);
	if ( m_PatternCount>0 && m_PatternIndex==-1 )
	{	
		m_PatternIndex = m_PatternCount-1; 
		PatParamPtr = AlgParam.GetAlgPatternParamPtr(m_PatternIndex, true);
	}
	if ( NULL == PatParamPtr )
	{	
		m_Dib.ReleaseBuffer();
		m_PatternParam = CPatternParam();				
	}
	else
	{	m_PatternParam = *PatParamPtr;	}		
		
	if ( CWnd::GetSafeHwnd()!=NULL && true==UpdateToUI )
	{
		LockUIWnd(false);
		UpdateWndCtrlUI();	
		if ( SwitchPatternImage(m_PatternIndex) == false )
		{	ResetPatternParam();	}		
	}
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::RedrawWnd()
{
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_MemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }	
	if ( NULL == hMemDC ) { return; }	

	RECT WndRect={0,0,0,0};
	BOOL bShowInfo = CWnd::IsDlgButtonChecked(PATTERN_SHOW_INFO_CHK);
	m_ImageWnd.GetClientRect(&WndRect);		
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );	

	if ( m_PatternIndex >= 0 ) 
	{
		CString str;
		const int PolarityIdx=m_PatternParam.GetResultPolarityIdx();
		double Score = m_PatternParam.GetPatResultScore(PolarityIdx);
		double OffsetX = m_PatternParam.GetResultX(PolarityIdx);
		double OffsetY = m_PatternParam.GetResultY(PolarityIdx);
		double Skew = m_PatternParam.GetResultSkew(PolarityIdx);
		double ScaleX = m_PatternParam.GetResultScaleX(PolarityIdx);
		double ScaleY = m_PatternParam.GetResultScaleY(PolarityIdx);
		const size_t RoiCount = m_PatternRoiList.size();
		if ( 0==RoiCount && TRUE==bShowInfo )
		{
			str.Format(_T("S:%.0f, X:%.0f, Y:%.0f, A:%.2f, SX:%.2f, SY:%.2f"), Score, OffsetX, OffsetY, Skew, ScaleX, ScaleY);					
			COLORREF TextClr = 0;
			COLORREF OldTextClr = 0;
			int OldBkMode = ::SetBkMode(hDC, TRANSPARENT);
			switch ( m_WndResultID )
			{
			case RESULT_ID_OK:	TextClr = 0x00FF00;	break;
			case RESULT_ID_NG:	TextClr = 0x0000FF;	break;
			default: TextClr = 0xFFFFFF;	break;
			}
			OldTextClr = ::SetTextColor(hDC, TextClr);
			::TextOut(hDC, 4, 4, str, str.GetLength());
			::SetTextColor(hDC, OldTextClr);
			::SetBkMode(hDC, OldBkMode);
		}
	}
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::CreateBKImage()
{
	HDC hDC = m_MemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }

	RECT Rect={0,0,0,0};
	m_ImageWnd.GetClientRect(&Rect);		
	HBRUSH hBrush = ::CreateSolidBrush(m_BkColor);		
	if ( NULL != hBrush )
	{
		::FillRect(hDC, &Rect, hBrush);
		::DeleteObject(hBrush); hBrush=NULL;	
	}
	DrawImage(hDC, Rect);
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::DrawImage(HDC hDC, const RECT &Rect)
{
	unsigned char *pDibBits = m_Dib.GetDIBBits();
	if ( NULL == pDibBits ) { return; }
	
	CString str;
	double Scale = 0;
	double ScaleW = 0;
	double ScaleH = 0;
	const double WndW = Rect.right-Rect.left;
	const double WndH = Rect.bottom-Rect.top;
	const double ImageW = m_Dib.GetImageW();
	const double ImageH = m_Dib.GetImageH();	
	const int    PatternPolarity = CWnd::GetDlgItemInt(PATTERN_TOWARD_EDIT);
	const BOOL   bShowInfo = CWnd::IsDlgButtonChecked(PATTERN_SHOW_INFO_CHK);

	ScaleW = WndW;
	ScaleH = WndH;
	ScaleW = ScaleW/ImageW;
	ScaleH = ScaleH/ImageH;
	double NewWndW = ScaleH*ImageW;
	double NewWndH = ScaleW*ImageH;

	if ( NewWndW < WndW )
	{
		Scale = ScaleH;
		NewWndH = WndH;	
	}
	else
	{
		Scale = ScaleW;
		NewWndW = WndW;	
	}
	if ( AOIDataCollect.LimitImageZoomScale(Scale, false) == true )
	{
		NewWndW = Scale*ImageW;
		NewWndH = Scale*ImageH;
	}	
	const int BltMode = AOIDataCollect.GetSystemParameter().m_StretchBltMode;
	const int OldMode = ::SetStretchBltMode(hDC, BltMode);
	if ( 2 == PatternPolarity )
	{
		CDib TempDib = m_Dib;
		TempDib.DoRotate_180();
		TempDib.DrawPartion(hDC, 0, 0, (int)ImageW, (int)ImageH, 0, 0, (int)(NewWndW), (int)(NewWndH));
	}
	else
	{	m_Dib.DrawPartion(hDC, 0, 0, (int)ImageW, (int)ImageH, 0, 0, (int)(NewWndW), (int)(NewWndH));	}
	::SetStretchBltMode(hDC, OldMode);

	
	CAOIWnd *WndPtr = m_WndPtr;
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	if (WndPtr == NULL) { return; }
	if (ALG_MEASURE_SIP_DISTANCE == AlgType) { return; }

	size_t       i=0;
	RECT         RoiRect;
	RECT         RoiRectDraw;
	TPATTERN_ROI PatRoi;
	COLORREF     clrOK = 0x00F000;
	COLORREF     clrNG = 0x0000F0;
	COLORREF     clrBypass = 0xF0F0F0;
	COLORREF     OldTextClr = ::SetTextColor(hDC, 0x000000);
	HPEN hOkPen = ::CreatePen(PS_SOLID, 2, clrOK);
	HPEN hNGPen = ::CreatePen(PS_SOLID, 2, clrNG);
	HPEN hBypassPen = ::CreatePen(PS_SOLID, 2, clrBypass);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hOkPen));
	const size_t RoiCount = m_PatternRoiList.size();	
	for ( i=0; i<RoiCount; i++ )
	{
		PatRoi = m_PatternRoiList[i];
		//if ( RESULT_ID_NONE == PatRoi.ResultID ) { continue; }
		if ( 2 == PatternPolarity )
		{
			RoiRect.top    = ImageH-PatRoi.RoiRect.bottom;
			RoiRect.bottom = ImageH-PatRoi.RoiRect.top;
			RoiRect.right = ImageW-PatRoi.RoiRect.left;
			RoiRect.left = ImageW-PatRoi.RoiRect.right;
		}
		else
		{	RoiRect = PatRoi.RoiRect; }
		RoiRectDraw.left = (int)(RoiRect.left*Scale+0.5);
		RoiRectDraw.top = (int)(RoiRect.top*Scale+0.5);
		RoiRectDraw.right = (int)(RoiRect.right*Scale+0.5);
		RoiRectDraw.bottom = (int)(RoiRect.bottom*Scale+0.5);
		switch ( PatRoi.ResultID )
		{
		case RESULT_ID_OK:
			::SetTextColor(hDC, clrOK);
			::SelectObject(hDC, hOkPen);
			break;		
		case RESULT_ID_SKIP:
		case RESULT_ID_BYPASS:
			::SetTextColor(hDC, clrBypass);
			::SelectObject(hDC, hBypassPen);
			break;
		case RESULT_ID_NG:
		case RESULT_ID_EXCEPTION:
			::SetTextColor(hDC, clrNG);
			::SelectObject(hDC, hNGPen);
			break;
		}
		ImageAPI.DrawRectLine(hDC, RoiRectDraw);

		if ( RESULT_ID_NONE!=PatRoi.ResultID && TRUE==bShowInfo )
		{
			str.Format(_T("%.0f %%"), PatRoi.ResultScore);
			::TextOut(hDC, RoiRectDraw.left+4, RoiRectDraw.top+4, str, str.GetLength());
		}
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hOkPen); hOkPen=NULL;
	::DeleteObject(hNGPen); hNGPen=NULL;
	::DeleteObject(hBypassPen); hBypassPen=NULL;
	::SetTextColor(hDC, OldTextClr);	
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::UpdateWndCtrlUI()
{
	BOOL bEnable = TRUE;	
	BOOL bEnable2=TRUE;
	CAOIWnd *WndPtr = m_WndPtr;
	const bool bLock = this->GetLockUIWnd();
	if ( NULL == WndPtr )
	{	bEnable = FALSE;	}
	else
	{
		ALG_TYPE AlgType = WndPtr->GetWndAlgType();
		bool UseImage = CAlgParam::CheckAlgPatternFileUsed(AlgType);
		if ( false == UseImage ) 
		{	bEnable = FALSE; }		
	}	
	if ( true == bLock )	
	{	bEnable = FALSE;	}

	UpdateParamToUI(NULL);		
	if ( NULL == WndPtr)
	{	bEnable2 = FALSE;	}
	else
	{		
		bool bSysLock = AOIDataCollect.GetIsLockUIWnd();
		if ( true == bSysLock )
		{	bEnable2 = FALSE;	}
		else
		{	bEnable2 = TRUE;	}		
	}

	m_IndexSpin.SetRange((short)0, (short)(m_PatternCount));
	JetAPI::EnableCtrlWnd(this, PATTERN_ADD_BTN, bEnable2);			
	JetAPI::EnableCtrlWnd(this, PATTERN_INDEX_SPIN, bEnable2);
	JetAPI::EnableCtrlWnd(this, PATTERN_GRAY_CHK, bEnable2);
	JetAPI::EnableCtrlWnd(this, PATTERN_BINARY_CHK, bEnable2);

	JetAPI::EnableCtrlWnd(this, PATTERN_DELETE_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, PATTERN_CLEAR_BTN, bEnable);
	JetAPI::EnableCtrlWnd(this, PATTERN_TEXT_BTN, bEnable);	
	JetAPI::EnableCtrlWnd(this, PATTERN_ADVANCE_BTN, bEnable);
	/*
	if ( -1 != m_PatternIndex )
	{	
		if ( SwitchPatternImage(m_PatternIndex) == false )
		{	ResetPatternParam();	}
	}
	else
	{	ResetPatternParam();	}
	*/
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::UpdateParamToUI(UINT FromCtrlID)
{	
	if ( PATTERN_INDEX_SPIN != FromCtrlID )
	{	m_IndexSpin.SetPos(m_PatternIndex+1); }

	if ( PATTERN_NUMBER_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(PATTERN_NUMBER_EDIT, m_PatternCount);	}
	
	if ( PATTERN_INDEX_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(PATTERN_INDEX_EDIT, m_PatternIndex+1); }

	const int PolarityIdx = m_PatternParam.GetResultPolarityIdx();
	if ( PATTERN_TOWARD_EDIT != FromCtrlID )
	{	CWnd::SetDlgItemInt(PATTERN_TOWARD_EDIT, PolarityIdx+1); }
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecAddPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	return ExecAddPattern_Field();
	//return ExecAddPattern_Model();
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecTextPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	return ExecTextSetting();	
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExtractPattern_Field(bool bExtend, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &PatBitCount, IMAGE_PTR &PatPtr)
{
	//bool bLockUIWnd = GetLockUIWnd();
	bool bLockUIWnd = AOIDataCollect.GetIsLockUIWnd();
	if ( true == bLockUIWnd ) { return false; }

	CString str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	double AttachedAngle = 0;
	bool   IsExceptionAngle = 0;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	MODEL_ATTACHED_OBJ ModelAttachedObj = AOIDataCollect.GetModelAttachedObj();	
	switch ( ModelAttachedObj )
	{
	case MODEL_ATTACHED_FD:
		FdPtr = ModelPtr->GetModelFdPtr();
		if ( NULL == FdPtr ) { return false; }		
		break;
	case MODEL_ATTACHED_MARK:
		MarkPtr = ModelPtr->GetModelMarkPtr();
		if ( NULL == MarkPtr ) { return false; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	case MODEL_ATTACHED_BARCODE:
		BarcodePtr = ModelPtr->GetModelBarcodePtr();
		if ( NULL == BarcodePtr ) { return false; }		
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	case MODEL_ATTACHED_COMPONENT:
		ComponentPtr = ModelPtr->GetModelComponentPtr();
		if ( NULL == ComponentPtr ) { return false; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	default:
		return false;
		break;
	}	

	TREGION4D FieldRgn;
	const int  nAlign = 4;
	const bool bCloned = false;	
	TUNI_FRAME  *UniFramePtr = NULL;
	std::vector<TUNI_FRAME> UniFrameList;	
	if ( AOIDataCollect.CopyFieldUniFrameList(UniFrameList, bCloned) == false )
	{	return false;	}	
	FieldRgn = AOIDataCollect.GetFieldUniFrameStageRegion();
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return false; }

	CString    ModelFolder;
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam BinaryParam = (AlgParam.GetAlgImageBinParam());
	const int FrameIndex = BinaryParam.GetBinaryFrameIndex();
	if ( FrameIndex<0 || FrameIndex>= UniFrameCount ) { return false; }	

	TUNI_FRAME UniFrame = UniFrameList[FrameIndex];
	IMAGE_PTR  ImagePtr  = UniFrame.ImagePtr;
	IMAGE_SIZE ImageW    = UniFrame.ImageW;
	IMAGE_SIZE ImageH    = UniFrame.ImageH;
	IMAGE_SIZE ImageStep = UniFrame.ImageStep;
	IMAGE_SIZE BitCount  = UniFrame.BitCount;
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	BOOL       bSaved=TRUE;
	if ( NULL == ImagePtr ) { return false; }
//#ifdef _DEBUG	
	if ( TRUE == bSaved )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FieldImage.PNG"));
		ImageAPI.SavePNGImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
//#endif//_DEBUG

	RECT      BoxRect={0,0,0,0};	
	RECT      TmpRect={0,0,0,0};	
	TREGION4D BoxRgn;	
	TPOINT2D  ImageRes;
	TREGION4D BoxStageRgn;
	TREGION4D BoxImageRgn;	
	TPOINT2D  ImageCp, RgnCp;
	//ModelPtr->GetModelTotalRegion(ModelRgn);	
	IMAGE_PTR  RoiPtr=NULL;
	IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0;

	const double RegionW = FieldRgn.GetWidth();
	const double RegionH = FieldRgn.GetHeight();	

	RgnCp.x = FieldRgn.GetCpX();
	RgnCp.y = FieldRgn.GetCpY();	
	ImageRes.x = ImageW;
	ImageRes.y = ImageH;
	ImageRes.x = RegionW/ImageRes.x;
	ImageRes.y = RegionH/ImageRes.y;
	if ( false == IsExceptionAngle )
	{		
		if ( false == bExtend )
		{
			if (DRAW_MODEL_RESULT == DrawModelMode )
			{	WndPtr->GetWndRegionStageRes(BoxRgn);	}
			else
			{	WndPtr->GetWndRegionStage(BoxRgn);	}
		}
		else
		{
			if (DRAW_MODEL_RESULT == DrawModelMode )
			{	WndPtr->GetWndExtendBox().GetBoxRegionStageRes(BoxRgn);	}
			else
			{	WndPtr->GetWndExtendBox().GetBoxRegionStage(BoxRgn);	}
		}
		if (AlgParam.GetAlgType() == ALG_MEASURE_SIP_DISTANCE) {
			const size_t  WndRoiCount = WndPtr->GetWndRoiWndCount();
			CAOIWndRoi *WndRoi = NULL;
			if (WndRoiCount != 0) {
				WndRoi = WndPtr->GetWndRoiWndPtr(0, false);
				if (NULL == WndRoi) { return false; }
				WndRoi->GetWndRoiBox().GetBoxRegionStage(BoxRgn);
			}
		}
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, BoxRgn, RgnCp, BoxImageRgn);
		BoxRect.left = JetAPI::Floor(BoxImageRgn.minX+0.5);
		BoxRect.top = JetAPI::Floor(BoxImageRgn.minY+0.5);
		BoxRect.right = JetAPI::Floor(BoxImageRgn.maxX+0.5);
		BoxRect.bottom = JetAPI::Floor(BoxImageRgn.maxY+0.5);	

		RoiW = BoxRect.right-BoxRect.left;
		RoiH = BoxRect.bottom-BoxRect.top;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, BoxRect, RoiStep, RoiPtr, false) == false )
		{
			if ( true == bCloned )
			{	JetAPI::ClearUniFrameList(UniFrameList);	}
			return false; 
		}
	}
	else
	{
		RECT       ModelRect={0,0,0,0};
		TREGION4D  BoxRgnRotated;
		TPOINT2D   BoxCp;
		TPOINT2D   CornerPts[4];
		TPOINT2D   CornerPtsRotated[4];
		IMAGE_PTR  BoxPtrRotated=NULL;
		IMAGE_PTR  ImagePtrRotated=NULL;
		IMAGE_SIZE BoxWRotated=0, BoxHRotated=0, BoxStepRotated=0;
		IMAGE_SIZE ImageWRotated=0, ImageHRotated=0, ImageStepRotated=0;
		const double StageRotateAngle = JetAPI::MapCadAngleToStageAngle(AttachedAngle);
		const double ImageRotateAngle = JetAPI::MapCadAngleToImageAngle(AttachedAngle);

		if ( false == bExtend )
		{
			if (DRAW_MODEL_RESULT == DrawModelMode )
			{	WndPtr->GetWndCornerPosStageRes(CornerPts);	}
			else
			{	WndPtr->GetWndCornerPosStage(CornerPts);	}
		}
		else
		{
			if (DRAW_MODEL_RESULT == DrawModelMode )
			{	WndPtr->GetWndExtendBox().GetBoxCornerPosStageRes(CornerPts);	}
			else
			{	WndPtr->GetWndExtendBox().GetBoxCornerPosStage(CornerPts);	}
		}
		JetAPI::CornerPtToRegion(CornerPts, BoxRgn);

		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, BoxRgn, RgnCp, BoxImageRgn);
		BoxRect.left = JetAPI::Floor(BoxImageRgn.minX+0.5);
		BoxRect.top = JetAPI::Floor(BoxImageRgn.minY+0.5);
		BoxRect.right = JetAPI::Floor(BoxImageRgn.maxX+0.5);
		BoxRect.bottom = JetAPI::Floor(BoxImageRgn.maxY+0.5);		
		ImageWRotated = BoxRect.right-BoxRect.left;
		ImageHRotated = BoxRect.bottom-BoxRect.top;
		ImageStepRotated = JetAPI::GetBMPImagePixelsPerLine(ImageWRotated, BitCount, nAlign);
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, BoxRect, ImageStepRotated, ImagePtrRotated, false) == false )
		{
			if ( true == bCloned )
			{	JetAPI::ClearUniFrameList(UniFrameList);	}
			return false; 
		}
		
	#ifdef _DEBUG
		//bSaved = TRUE;
		if ( TRUE == bSaved )
		{
			str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PatternImage.PNG"));
			ImageAPI.SavePNGImage(str, ImageWRotated, ImageHRotated, ImageStepRotated, BitCount, ImagePtrRotated, true);
		}
	#endif//_DEBUG

		BoxCp.x = BoxRgn.GetCpX();
		BoxCp.y = BoxRgn.GetCpY();
		BoxCp.x = -BoxCp.x;
		BoxCp.y = -BoxCp.y;
		JetAPI::MoveCornerPts(CornerPts, BoxCp, CornerPtsRotated);
		JetAPI::RotateCornerPos(-StageRotateAngle, 0, 0, CornerPtsRotated);
		JetAPI::CornerPtToRegion(CornerPtsRotated, BoxRgnRotated);		
		if ( ImageAPI.RotateImage(-AttachedAngle, ImageWRotated, ImageHRotated, ImageStepRotated, BitCount, ImagePtrRotated, BoxWRotated, BoxHRotated, BoxStepRotated, BoxPtrRotated) == false )
		{
			if ( true == bCloned )
			{	JetAPI::ClearUniFrameList(UniFrameList);	}
			JetMemory.free_func(ImagePtrRotated);
			return false; 
		}
	#ifdef _DEBUG
		//bSaved = TRUE;
		if ( TRUE == bSaved )
		{
			str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PatternRotated.PNG"));
			ImageAPI.SavePNGImage(str, BoxWRotated, BoxHRotated, BoxStepRotated, BitCount, BoxPtrRotated, true);
		}
	#endif//_DEBUG
		
		double ImageCpX = BoxWRotated;
		double ImageCpY = BoxHRotated;
		ImageCpX = ImageCpX/2;
		ImageCpY = ImageCpY/2;
		BoxRgnRotated.minX = BoxRgnRotated.minX/ImageRes.x;
		BoxRgnRotated.minY = BoxRgnRotated.minY/ImageRes.y;
		BoxRgnRotated.maxX = BoxRgnRotated.maxX/ImageRes.x;
		BoxRgnRotated.maxY = BoxRgnRotated.maxY/ImageRes.y;

		BoxRect.left = JetAPI::Floor(BoxRgnRotated.minX+ImageCpX);
		BoxRect.top = JetAPI::Floor(BoxRgnRotated.minY+ImageCpY);
		BoxRect.right = JetAPI::Floor(BoxRgnRotated.maxX+ImageCpX);
		BoxRect.bottom = JetAPI::Floor(BoxRgnRotated.maxY+ImageCpY);
		RoiW = BoxRect.right-BoxRect.left;
		RoiH = BoxRect.bottom-BoxRect.top;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
		if ( ImageAPI.ExtractRoiImage(BoxWRotated, BoxHRotated, BoxStepRotated, BitCount, BoxPtrRotated, BoxRect, RoiStep, RoiPtr, false) == false )
		{			
			if ( true == bCloned )
			{	JetAPI::ClearUniFrameList(UniFrameList);	}
			JetMemory.free_func(BoxPtrRotated);
			JetMemory.free_func(ImagePtrRotated);
			return false; 
		}
		JetMemory.free_func(BoxPtrRotated);
		JetMemory.free_func(ImagePtrRotated);
	}
	if ( true == bCloned )
	{	JetAPI::ClearUniFrameList(UniFrameList);	}

//#ifdef _DEBUG
	//bSaved = TRUE;
	if ( TRUE == bSaved )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Pattern.PNG"));
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
	}
//#endif//_DEBUG	

	bool  bRotateImg = false;
	const int PolarityIdx = m_PatternParam.GetResultPolarityIdx();
	if ( 0==PolarityIdx || DRAW_MODEL_RESULT!=DrawModelMode)
	{	bRotateImg = false; }
	else
	{	bRotateImg = true; }
	if ( true == bRotateImg )
	{		
		IMAGE_SIZE RoiW_180=RoiW;
		IMAGE_SIZE RoiH_180=RoiH;
		IMAGE_SIZE RoiStep_180=RoiStep;
		IMAGE_PTR  RoiPtr_180 = NULL;
		const size_t BufferSize_180=ImageAPI.CalcBufferSize(RoiStep, RoiH);
		if ( JetMemory.alloc_func(BufferSize_180, RoiPtr_180, "CEditImagePatternWnd::ExtractPattern_Field", "RoiPtr_180") == false )
		{
			JetMemory.free_func(RoiPtr);
			return false;
		}
		if ( ImageAPI.RotateImage3(180, RoiW, RoiH, RoiStep, BitCount, RoiPtr, RoiW_180, RoiH_180, RoiStep_180, RoiPtr_180) == false )
		{
			JetMemory.free_func(RoiPtr);
			JetMemory.free_func(RoiPtr_180);
			return false;
		}
		JetMemory.free_func(RoiPtr);

		RoiW = RoiW_180;
		RoiH = RoiH_180;
		RoiStep = RoiStep_180;
		RoiPtr = RoiPtr_180;
	}

	PatW = RoiW;
	PatH = RoiH;
	PatStep = RoiStep;
	PatBitCount = BitCount;
	PatPtr = RoiPtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecAddPattern_Field()
{
	//bool bLockUIWnd = GetLockUIWnd();
	bool bLockUIWnd = AOIDataCollect.GetIsLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CString str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	double AttachedAngle = 0;
	bool   IsExceptionAngle = 0;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	MODEL_ATTACHED_OBJ ModelAttachedObj = AOIDataCollect.GetModelAttachedObj();	
	switch ( ModelAttachedObj )
	{
	case MODEL_ATTACHED_FD:
		FdPtr = ModelPtr->GetModelFdPtr();
		if ( NULL == FdPtr ) { return true; }		
		break;
	case MODEL_ATTACHED_MARK:
		MarkPtr = ModelPtr->GetModelMarkPtr();
		if ( NULL == MarkPtr ) { return true; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	case MODEL_ATTACHED_BARCODE:
		BarcodePtr = ModelPtr->GetModelBarcodePtr();
		if ( NULL == BarcodePtr ) { return true; }		
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	case MODEL_ATTACHED_COMPONENT:
		ComponentPtr = ModelPtr->GetModelComponentPtr();
		if ( NULL == ComponentPtr ) { return true; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	default:
		return true;
		break;
	}	

	IMAGE_PTR  RoiPtr=NULL;
	IMAGE_SIZE RoiW=0;
	IMAGE_SIZE RoiH=0;
	IMAGE_SIZE RoiStep=0;
	IMAGE_SIZE BitCount=0;
	const bool bExtend = false;

	if ( ExtractPattern_Field(bExtend, RoiW, RoiH, RoiStep, BitCount, RoiPtr) == false ) 
	{	return true; }
	if ( NULL == RoiPtr )
	{	return true; }
	
	CString    ModelFolder;
	int        PolarityIdx = 0;
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam BinaryParam = (AlgParam.GetAlgImageBinParam());	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( true == IsExceptionAngle ) 
	{	WndToward = JetAPI::RotateToward(-AttachedAngle, WndToward);	}

	ModelFolder = ModelPtr->GetModelFolder();
	JetAPI::CreateFolder(ModelFolder);	
	if ( DRAW_MODEL_RESULT != DrawModelMode )
	{	PolarityIdx = 0; }
	else
	{	PolarityIdx = m_PatternParam.GetResultPolarityIdx(); }
	if ( AlgParam.AddAlgPatternImage(RoiW, RoiH, RoiStep, BitCount, RoiPtr, ModelFolder, WndToward, PolarityIdx, BinaryParam) == false )
	{
		JetMemory.free_func(RoiPtr);
		return false;
	}
	JetMemory.free_func(RoiPtr);	
	ModelPtr->ApplyModelWnd(WndPtr);
	LogOperCtrl.SaveLogModelWndAlgPatternContentAdd(WndPtr);

	AOIDataCollect.CloseActiveComponent(ComponentPtr);
	m_PatternCount = AlgParam.GetAlgPatternCount();
	m_PatternIndex = m_PatternCount-1;
	AlgParam.SetAlgPatternResultIndex(m_PatternIndex);
	m_IndexSpin.SetRange((short)0, (short)(m_PatternCount));
	UpdateParamToUI(NULL);
	UpdatePatternParamToParentWnd();
	if ( SwitchPatternImage(m_PatternIndex) == false )
	{
		ResetPatternParam();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExtractPattern_Model(bool bExtend, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &PatBitCount, IMAGE_PTR &PatPtr)
{
	//bool bLockUIWnd = GetLockUIWnd();
	bool bLockUIWnd = AOIDataCollect.GetIsLockUIWnd();
	if ( true == bLockUIWnd ) { return false; }	

	CString str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) 
	{
		CString ModelName1 = ModelPtr->GetModelName();
		CString ModelName2 = WndPtr->GetWndModelPtr()->GetModelName();
		str.Format(_T("Error, Model Exception (%s and %s"), ModelName1, ModelName2);
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	double AttachedAngle = 0;
	bool   IsExceptionAngle = 0;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	MODEL_ATTACHED_OBJ ModelAttachedObj = AOIDataCollect.GetModelAttachedObj();	
	switch ( ModelAttachedObj )
	{
	case MODEL_ATTACHED_FD:
		FdPtr = ModelPtr->GetModelFdPtr();
		if ( NULL == FdPtr ) { return false; }		
		break;
	case MODEL_ATTACHED_MARK:
		MarkPtr = ModelPtr->GetModelMarkPtr();
		if ( NULL == MarkPtr ) { return false; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	case MODEL_ATTACHED_BARCODE:
		BarcodePtr = ModelPtr->GetModelBarcodePtr();
		if ( NULL == BarcodePtr ) { return false; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	case MODEL_ATTACHED_COMPONENT:
		ComponentPtr = ModelPtr->GetModelComponentPtr();
		if ( NULL == ComponentPtr ) { return false; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	default:
		return false;
		break;
	}	

	const int  nAlign = 4;
	const bool bCloned = false;
	TUNI_FRAME  *UniFramePtr = NULL;
	std::vector<TUNI_FRAME> UniFrameList;	
	if ( AOIDataCollect.CopyModelUniFrameList(UniFrameList, bCloned) == false )
	{	return false;	}	
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return false; }

	CString    ModelFolder;
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam BinaryParam = (AlgParam.GetAlgImageBinParam());
	const int FrameIndex = BinaryParam.GetBinaryFrameIndex();
	if ( FrameIndex<0 || FrameIndex>= UniFrameCount ) { return false; }	

	TUNI_FRAME UniFrame = UniFrameList[FrameIndex];
	IMAGE_PTR  ImagePtr  = UniFrame.ImagePtr;
	IMAGE_SIZE ImageW    = UniFrame.ImageW;
	IMAGE_SIZE ImageH    = UniFrame.ImageH;
	IMAGE_SIZE ImageStep = UniFrame.ImageStep;
	IMAGE_SIZE BitCount  = UniFrame.BitCount;
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	BOOL       bSaved=FALSE;
	
	RECT      BoxRect={0,0,0,0};
	TREGION4D ModelRgn, BoxRgn;
	TPOINT2D  Scale, ImageCp, RgnCp;
	ModelPtr->GetModelTotalRegion(ModelRgn);	
	IMAGE_PTR  RoiPtr=NULL;
	IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0;

	const double RegionW = ModelRgn.GetWidth();
	const double RegionH = ModelRgn.GetHeight();	
	Scale.x = ImageW;
	Scale.y = ImageH;
	Scale.x = Scale.x/RegionW;
	Scale.y = Scale.y/RegionH;
	if ( false == IsExceptionAngle )
	{
		if ( false == bExtend )
		{
			if (DRAW_MODEL_RESULT == DrawModelMode )
			{	WndPtr->GetWndRegionRes(BoxRgn);	}
			else
			{	WndPtr->GetWndRegion(BoxRgn);	} 
		}
		else
		{
			if (DRAW_MODEL_RESULT == DrawModelMode )
			{	WndPtr->GetWndExtendBox().GetBoxRegionStageRes(BoxRgn);	}
			else
			{	WndPtr->GetWndExtendBox().GetBoxRegionStage(BoxRgn);	}
		}

		RgnCp.x = ModelRgn.GetCpX();
		RgnCp.y = ModelRgn.GetCpY();
		ImageCp.x = ImageW;
		ImageCp.y = ImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;	

		CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, BoxRect);	

		RoiW = BoxRect.right-BoxRect.left;
		RoiH = BoxRect.bottom-BoxRect.top;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, BoxRect, RoiStep, RoiPtr, false) == false )
		{
			if ( true == bCloned )
			{	JetAPI::ClearUniFrameList(UniFrameList);	}
			return false; 
		}
	}
	else
	{
		RECT       ModelRect={0,0,0,0};
		TREGION4D  ModelRgnRotated;
		TPOINT2D   CornerPts[4];
		TPOINT2D   ModelCornerPts[4];
		IMAGE_PTR  ImagePtrModel=NULL;
		IMAGE_PTR  ImagePtrRotated=NULL;		
		IMAGE_SIZE ImageWModel=0, ImageHModel=0, ImageStepModel=0;
		IMAGE_SIZE ImageWRotated=0, ImageHRotated=0, ImageStepRotated=0;

		if ( false == bExtend )
		{
			if (DRAW_MODEL_RESULT == DrawModelMode )
			{	WndPtr->GetWndCornerPosRes(CornerPts);	}
			else
			{	WndPtr->GetWndCornerPos(CornerPts);	}
		}
		else
		{	
			if (DRAW_MODEL_RESULT == DrawModelMode )
			{	WndPtr->GetWndExtendBox().GetBoxCornerPosStageRes(CornerPts);	}
			else
			{	WndPtr->GetWndExtendBox().GetBoxCornerPosStage(CornerPts);	}		
		}
		ModelPtr->GetModelTotalCornerPts(ModelCornerPts);
		
		JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, CornerPts);
		JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, ModelCornerPts);

		JetAPI::CornerPtToRegion(CornerPts, BoxRgn);
		JetAPI::CornerPtToRegion(ModelCornerPts, ModelRgnRotated);
		if ( ImageAPI.RotateImage(-AttachedAngle, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageWRotated, ImageHRotated, ImageStepRotated, ImagePtrRotated) == false )
		{
			if ( true == bCloned )
			{	JetAPI::ClearUniFrameList(UniFrameList);	}
			return false; 
		}

		const double RgnWRotated = ModelRgnRotated.GetWidth();
		const double RgnHRotated = ModelRgnRotated.GetHeight();
		ImageWModel = JetAPI::Floor(RgnWRotated*Scale.x);
		ImageHModel = JetAPI::Floor(RgnHRotated*Scale.y);		
		ModelRect.left = (ImageWRotated-ImageWModel)/2;
		ModelRect.right = ModelRect.left+ImageWModel;
		ModelRect.top = (ImageHRotated-ImageHModel)/2;
		ModelRect.bottom = ModelRect.top+ImageHModel;
		ImageStepModel = JetAPI::GetBMPImagePixelsPerLine(ImageWModel, BitCount, nAlign);
		if ( ImageAPI.ExtractRoiImage(ImageWRotated, ImageHRotated, ImageStepRotated, BitCount, ImagePtrRotated, ModelRect, ImageStepModel, ImagePtrModel, false) == false )
		{
			if ( true == bCloned )
			{	JetAPI::ClearUniFrameList(UniFrameList);	}
			JetMemory.free_func(ImagePtrRotated);
			return false; 
		}
		JetMemory.free_func(ImagePtrRotated);
		
		Scale.x = ImageWModel;
		Scale.y = ImageHModel;
		Scale.x = Scale.x/RgnWRotated;
		Scale.y = Scale.y/RgnHRotated;
		RgnCp.x = ModelRgnRotated.GetCpX();
		RgnCp.y = ModelRgnRotated.GetCpY();
		ImageCp.x = ImageWModel;
		ImageCp.y = ImageHModel;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;

		CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageWModel, ImageHModel, RgnCp, Scale, ImageCp, BoxRect);	
		RoiW = BoxRect.right-BoxRect.left;
		RoiH = BoxRect.bottom-BoxRect.top;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
		if ( ImageAPI.ExtractRoiImage(ImageWModel, ImageHModel, ImageStepModel, BitCount, ImagePtrModel, BoxRect, RoiStep, RoiPtr, false) == false )
		{
			JetMemory.free_func(ImagePtrModel);
			if ( true == bCloned )
			{	JetAPI::ClearUniFrameList(UniFrameList);	}
			return false; 
		}
		JetMemory.free_func(ImagePtrModel);
	}
	if ( true == bCloned )
	{	JetAPI::ClearUniFrameList(UniFrameList);	}

#ifdef _DEBUG
	bSaved = TRUE;
	if ( TRUE == bSaved )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("Pattern.PNG"));
		ImageAPI.SavePNGImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
	}
#endif//_DEBUG

	bool  bRotateImg = false;
	const int PolarityIdx = m_PatternParam.GetResultPolarityIdx();
	if ( 0==PolarityIdx || DRAW_MODEL_RESULT!=DrawModelMode)
	{	bRotateImg = false; }
	else
	{	bRotateImg = true; }
	if ( true == bRotateImg )
	{		
		IMAGE_SIZE RoiW_180=RoiW;
		IMAGE_SIZE RoiH_180=RoiH;
		IMAGE_SIZE RoiStep_180=RoiStep;
		IMAGE_PTR  RoiPtr_180 = NULL;
		const size_t BufferSize_180=ImageAPI.CalcBufferSize(RoiStep, RoiH);
		if ( JetMemory.alloc_func(BufferSize_180, RoiPtr_180, "CEditImagePatternWnd::ExtractPattern_Model", "RoiPtr_180") == false )
		{
			JetMemory.free_func(RoiPtr);
			return false;
		}
		if ( ImageAPI.RotateImage3(180, RoiW, RoiH, RoiStep, BitCount, RoiPtr, RoiW_180, RoiH_180, RoiStep_180, RoiPtr_180) == false )
		{
			JetMemory.free_func(RoiPtr);
			JetMemory.free_func(RoiPtr_180);
			return false;
		}
		JetMemory.free_func(RoiPtr);

		RoiW = RoiW_180;
		RoiH = RoiH_180;
		RoiStep = RoiStep_180;
		RoiPtr = RoiPtr_180;
	}

	PatW = RoiW;
	PatH = RoiH;
	PatStep = RoiStep;
	PatBitCount = BitCount;
	PatPtr = RoiPtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecAddPattern_Model()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CString str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }
	CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) 
	{
		CString ModelName1 = ModelPtr->GetModelName();
		CString ModelName2 = WndPtr->GetWndModelPtr()->GetModelName();
		str.Format(_T("Error, Model Exception (%s and %s"), ModelName1, ModelName2);
		JetAPI::ShowMessageBox(str);
		return true; 
	}
	double AttachedAngle = 0;
	bool   IsExceptionAngle = 0;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	MODEL_ATTACHED_OBJ ModelAttachedObj = AOIDataCollect.GetModelAttachedObj();	
	switch ( ModelAttachedObj )
	{
	case MODEL_ATTACHED_FD:
		FdPtr = ModelPtr->GetModelFdPtr();
		if ( NULL == FdPtr ) { return true; }		
		break;
	case MODEL_ATTACHED_MARK:
		MarkPtr = ModelPtr->GetModelMarkPtr();
		if ( NULL == MarkPtr ) { return false; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	case MODEL_ATTACHED_BARCODE:
		BarcodePtr = ModelPtr->GetModelBarcodePtr();
		if ( NULL == BarcodePtr ) { return true; }		
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	case MODEL_ATTACHED_COMPONENT:
		ComponentPtr = ModelPtr->GetModelComponentPtr();
		if ( NULL == ComponentPtr ) { return true; }
		AttachedAngle = ModelPtr->GetModelAttachedAngle();
		IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		break;
	default:
		return true;
		break;
	}	

	IMAGE_PTR  RoiPtr=NULL;
	IMAGE_SIZE RoiW=0;
	IMAGE_SIZE RoiH=0;
	IMAGE_SIZE RoiStep=0;
	IMAGE_SIZE BitCount=0;
	const bool bExtend = false;

	if ( ExtractPattern_Model(bExtend, RoiW, RoiH, RoiStep, BitCount, RoiPtr) == false ) 
	{	return true; }
	if ( NULL == RoiPtr )
	{	return true; }
	
	CString    ModelFolder;
	int        PolarityIdx = 0;
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam BinaryParam = (AlgParam.GetAlgImageBinParam());
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();	

	ModelFolder = ModelPtr->GetModelFolder();
	JetAPI::CreateFolder(ModelFolder);	
	if ( DRAW_MODEL_RESULT != DrawModelMode )
	{	PolarityIdx = 0; }
	else
	{	PolarityIdx = m_PatternParam.GetResultPolarityIdx(); }
	if ( AlgParam.AddAlgPatternImage(RoiW, RoiH, RoiStep, BitCount, RoiPtr, ModelFolder, WndToward, PolarityIdx, BinaryParam) == false )
	{
		JetMemory.free_func(RoiPtr);
		return false;
	}
	JetMemory.free_func(RoiPtr);	
	ModelPtr->ApplyModelWnd(WndPtr);
	LogOperCtrl.SaveLogModelWndAlgPatternContentAdd(WndPtr);

	AOIDataCollect.CloseActiveComponent(ComponentPtr);
	m_PatternCount = AlgParam.GetAlgPatternCount();
	m_PatternIndex = m_PatternCount-1;
	AlgParam.SetAlgPatternResultIndex(m_PatternIndex);
	m_IndexSpin.SetRange((short)0, (short)(m_PatternCount));
	UpdateParamToUI(NULL);
	UpdatePatternParamToParentWnd();
	if ( SwitchPatternImage(m_PatternIndex) == false )
	{
		ResetPatternParam();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecEditPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	CString  str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }	
	CString        ModelFolder  = ModelPtr->GetModelFolder();
	CAOIFd        *FdPtr        = ModelPtr->GetModelFdPtr();
	CAOIBarcode   *BarcodePtr   = ModelPtr->GetModelBarcodePtr();
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		

	CString   filename;	
	CString   filenameDst;	
	CString   fileFolder = AOIDataCollect.GetAOITempDirectory();
	IMAGE_PTR  ImagePtr=NULL;
	CPatternParam *PatParamPtr=NULL;
	const int index = CWnd::GetDlgItemInt(PATTERN_INDEX_EDIT)-1;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;

	if ( true == IsExceptionAngle ) 
	{	WndToward = JetAPI::RotateToward(-AttachedAngle, WndToward);	}

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
	ModelPtr->SetModelNeedSaveFiles(true);
	LogOperCtrl.SaveLogModelWndAlgPatternContentModify(WndPtr);

	JetMemory.free_func(ImagePtr);
	SwitchPatternImage(index);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecDelPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CString  str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }
	CAOIFd        *FdPtr        = ModelPtr->GetModelFdPtr();
	CAOIBarcode   *BarcodePtr   = ModelPtr->GetModelBarcodePtr();
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();

	str = _T("Do you want to delete the patern?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return true; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.DeleteAlgPattern(m_PatternIndex);
	ModelPtr->ApplyModelWnd(WndPtr);	
	LogOperCtrl.SaveLogModelWndAlgPatternContentDelete(WndPtr);

	AOIDataCollect.CloseActiveComponent(ComponentPtr);
	m_PatternCount = AlgParam.GetAlgPatternCount();	
	if ( m_PatternIndex >= m_PatternCount )
	{	m_PatternIndex = m_PatternCount-1; }
	m_IndexSpin.SetRange((short)0, (short)(m_PatternCount));
	UpdateParamToUI(NULL);	
	UpdatePatternParamToParentWnd();
	if ( SwitchPatternImage(m_PatternIndex) == false )
	{
		ResetPatternParam();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecClearPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CString   str;
	CAOIWnd  *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }	
	CAOIFd        *FdPtr        = ModelPtr->GetModelFdPtr();
	CAOIBarcode   *BarcodePtr   = ModelPtr->GetModelBarcodePtr();
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();	

	str = _T("Do you want to clear all paterns?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return true; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.ClearAlgPatternFiles();
	ModelPtr->ApplyModelWnd(WndPtr);
	LogOperCtrl.SaveLogModelWndAlgPatternContentClearAll(WndPtr);

	AOIDataCollect.CloseActiveComponent(ComponentPtr);	
	m_PatternCount = AlgParam.GetAlgPatternCount();
	m_PatternIndex = -1;
	m_IndexSpin.SetRange((short)0, (short)(0));
	m_PatternParam = CPatternParam();	
	UpdateParamToUI(NULL);
	UpdatePatternParamToParentWnd();

	m_Dib.ReleaseBuffer();
	CreateBKImage();
	RedrawWnd();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecTextSetting()//文字設定	
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }
	if ( AOIDataCollect.CheckAIServerIsReady() == false )	
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}

	CString  str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }	
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }
	const int     Index = m_PatternIndex;
	BOX_TOWARD    WndToward     = WndPtr->GetWndToward();
	BOX_TOWARD    WndToward2    = WndPtr->GetWndToward();
	CAOIFd        *FdPtr        = ModelPtr->GetModelFdPtr();
	CAOIBarcode   *BarcodePtr   = ModelPtr->GetModelBarcodePtr();
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	const bool   bForceRotateAngle=true;//強迫轉正角度
	const double AttachedAngle  = ModelPtr->GetModelAttachedAngle();
	const bool IsExceptionAnlge = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( true==IsExceptionAnlge || true==bForceRotateAngle )
	{	WndToward2 = JetAPI::RotateToward(-AttachedAngle, WndToward);	}

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_AI_MODEL &aiModelParam=AlgParam.GetAlgParamAiModel();	
	CPatternParam *PatParamPtr=AlgParam.GetAlgPatternParamPtr(Index, true);	
	if ( NULL == PatParamPtr ) { return false; }

	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	if ( AlgParam.LoadAlgPatternImage(Index, WndToward2, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return false; }

	CAlgPatternTextWnd Wnd;
	float PatternAngle=aiModelParam.aiPatternAngle;
	Wnd.SetPatternParam(*PatParamPtr, WndToward2, PatternAngle);
	Wnd.SetImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	JetMemory.free_func(ImagePtr);	
	if ( Wnd.DoModal() == IDCANCEL )
	{	return false;	}	

	bool bUpdateWndParam=false;
	std::vector<TPATTERN_ROI> PatRoiListL;
	std::vector<TPATTERN_ROI> PatRoiListT;
	std::vector<TPATTERN_ROI> PatRoiListR;
	std::vector<TPATTERN_ROI> PatRoiListB;
	PatternAngle=Wnd.GetPatternAngle();
	CPatternParam &PatParamRef=Wnd.GetPatternParam();
	const size_t MaxPatternRoiCount=MAX_PATTERN_ROI_COUNT;	
	for ( size_t i=0; i<MaxPatternRoiCount; i++ )
	{
		PatRoiListL = PatParamRef.GetPatRoiListL(i);
		PatRoiListT = PatParamRef.GetPatRoiListT(i);
		PatRoiListR = PatParamRef.GetPatRoiListR(i);
		PatRoiListB = PatParamRef.GetPatRoiListB(i);

		PatParamPtr->SetPatRoiListL(i, PatRoiListL);
		PatParamPtr->SetPatRoiListT(i, PatRoiListT);
		PatParamPtr->SetPatRoiListR(i, PatRoiListR);
		PatParamPtr->SetPatRoiListB(i, PatRoiListB);		
	}
	if ( aiModelParam.aiPatternAngle != PatternAngle )
	{
		bUpdateWndParam = true;
		aiModelParam.aiPatternAngle = PatternAngle;
	}
	PatParamPtr->SetPatText(PatParamRef.GetPatText());
	PatParamPtr->SetPatSimilarTextList(PatParamRef.GetPatSimilarTextList());

	WndPtr->SetWndModified(true);
	ModelPtr->ApplyModelWnd(WndPtr);
	LogOperCtrl.SaveLogModelWndAlgPatternContentText(WndPtr);
	AOIDataCollect.CloseActiveComponent(ComponentPtr);			
	if ( true == bUpdateWndParam )
	{
		WndPtr->SetWndUIUpated_Param(false);
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_REBUILD_WND_PARAM_LIST, NULL);	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecAdvanceSetting()//進階設定
{
	CString str;
	const bool bShowMsg=true;
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) 
	{
		if ( true == bShowMsg )
		{
			str = _T("Error, UI is locked");
			JetAPI::ShowMessageBox(str);
		}
		return true; 
	}

	CAOIWnd *RefWndPtr = m_WndPtr;
	if ( NULL == RefWndPtr ) 
	{ 
		if ( true == bShowMsg )
		{
			str = _T("Error, WndPtr == NULL");
			JetAPI::ShowMessageBox(str);
		}
		return false; 
	}
	CAOIModel *RefModelPtr = RefWndPtr->GetWndModelPtr();
	if ( NULL == RefModelPtr ) 
	{
		if ( true == bShowMsg )
		{
			str = _T("Error, WndPtr' ModelPtr == NULL");
			JetAPI::ShowMessageBox(str);
		}
		return false; 
	}		
	if ( RefWndPtr->GetWndAlgParam().GetAlgPatternFileUsed() == false ) 
	{
		if ( true == bShowMsg )
		{
			str = _T("Error, Wnd Alg not support pattern file");
			JetAPI::ShowMessageBox(str);
		}
		return false; 
	}
	CAOIComponent *ComponentPtr = RefModelPtr->GetModelComponentPtr();
	const double AttachedAngle = RefModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	const bool bExtend = true;
	std::vector<TUNI_FRAME> WndUniFrameList;	
	if ( AOIDataCollect.CreateWndUniFrameListByField(bExtend, RefWndPtr, WndUniFrameList) == false ) 	
	{
		if ( true == bShowMsg )
		{
			str = _T("Error, Create Wnd Uni-Frame List By Field Fault");
			JetAPI::ShowMessageBox(str);
		}
		return false;	
	}
	
	CAOIWnd *WndPtr = NULL;	
	WndPtr = RefWndPtr->CloneWndObj();	
	if ( NULL == WndPtr  ) 
	{ 
		if ( true == bShowMsg )
		{
			str = _T("Error, Clone Wnd Obj Fault");
			JetAPI::ShowMessageBox(str);
		}
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false; 
	}	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	const size_t WndUniFrameCount = WndUniFrameList.size();	
	unsigned int FrameIndex = AlgParam.GetAlgImageBinParam().GetBinaryFrameIndex();
	if ( FrameIndex >= WndUniFrameCount ) 
	{		
		if ( true == bShowMsg )
		{
			str = _T("Error, Wnd Frame Index is out of Uni-Frame Count");
			JetAPI::ShowMessageBox(str);
		}
		AOIObjManager.DestroyWndObj(WndPtr);
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}	
	
	TREGION4D  WndRgn;	
	bool       bSaved=true;
	bool       bModelInit = false;		
	TPOINT2D   RgnCp, Scale, ImageCp;
	IMAGE_PTR  PatPtr=WndUniFrameList[FrameIndex].ImagePtr;
	IMAGE_SIZE PatW=WndUniFrameList[FrameIndex].ImageW;
	IMAGE_SIZE PatH=WndUniFrameList[FrameIndex].ImageH;
	IMAGE_SIZE PatStep=WndUniFrameList[FrameIndex].ImageStep;
	IMAGE_SIZE PatBitCount=WndUniFrameList[FrameIndex].BitCount;	
	TPOINT2D  ModelImageScale = RefModelPtr->GetModelImageScale();

#ifdef _DEBUG
	if ( true == bSaved )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("AdvancePat.PNG"));
		ImageAPI.SaveImage(str, PatW, PatH, PatStep, PatBitCount, PatPtr, true);
	}
#endif//_DEBUG
	
	WndPtr->InitWndInspection(bModelInit);
	AlgParam.SetAlgPatternTestAll(true);	
	if ( true == IsExceptionAngle )	
	{	WndPtr->RotateWnd(-AttachedAngle, 0, 0);	}
	if ( false == bExtend ) 
	{	WndPtr->GetWndRegion(WndRgn);	}
	else
	{	WndPtr->GetWndExtendBox().GetBoxRegion(WndRgn); }
	const double RegionW = WndRgn.GetWidth();
	const double RegionH = WndRgn.GetHeight();	
	Scale.x = PatW;
	Scale.y = PatH;
	Scale.x = Scale.x/RegionW;
	Scale.y = Scale.y/RegionH;	
	RgnCp.x = WndRgn.GetCpX();
	RgnCp.y = WndRgn.GetCpY();
	ImageCp.x = PatW;
	ImageCp.y = PatH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;	
	RefModelPtr->SetModelImageScale(Scale);
	if ( WndPtr->ExecWndInspection(RefModelPtr, WndRgn, RgnCp, Scale, ImageCp, WndUniFrameList) == false ) 
	{
		if ( true == bShowMsg )
		{
			str = _T("Error, ExecWndInspectoin Fault");
			JetAPI::ShowMessageBox(str);
		}
		AOIObjManager.DestroyWndObj(WndPtr);		
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}
	RefModelPtr->SetModelImageScale(ModelImageScale);
	
	CString TempFolder;
	CString ModelFolderTmp;
	CString PatternFolder;
	CString PatternFolderTmp;	
	CString ModelName=RefModelPtr->GetModelName();
	const int AlgGroupID=AlgParam.GetAlgGroupID();

	//WndPtr->SetWndModelPtr(NULL);	
	TempFolder = AOIDataCollect.GetAOITempDirectory();	
	ModelFolderTmp.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), ModelName);
	PatternFolder = AOIDataDefine.GetAlgPatternFolder(AlgGroupID);
	PatternFolderTmp.Format(_T("%s\\%s"), ModelFolderTmp, PatternFolder);	
	::CreateDirectory(TempFolder, NULL);	::Sleep(0);
	::CreateDirectory(ModelFolderTmp, NULL);	::Sleep(0);	
	::CreateDirectory(PatternFolderTmp, NULL);	::Sleep(0);

	PatternFolder = AlgParam.GetAlgPatternFolder();
	if ( JetAPI::CopyFolderAToFolderB(PatternFolder, PatternFolderTmp, false, true, _T(""), -1, -1) == false ) 
	{	
		if ( true == bShowMsg )
		{
			str.Format(_T("Error, CopyFolderAToFolderB Fault\n[%s] -> [%s]"), PatternFolder, PatternFolderTmp);
			JetAPI::ShowMessageBox(str);
		}
		AOIObjManager.DestroyWndObj(WndPtr);
		JetAPI::RemoveFolder(ModelFolderTmp);
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}
	AlgParam.SetAlgPatternFolder(PatternFolderTmp);
	
	CAlgPatternListWnd PatternListWnd;
	PatternListWnd.SetWndPtr(WndPtr);
	PatternListWnd.SetImagePtr(PatW, PatH, PatStep, PatBitCount, PatPtr);
	PatternListWnd.SetPatternFolder(WndPtr->GetWndAlgParam().GetAlgPatternFolder());

	DWORD dwRes =  PatternListWnd.DoModal();
	const bool bModified = PatternListWnd.GetModified();
	if ( IDCANCEL==dwRes || false==bModified ) 
	{		
		AOIObjManager.DestroyWndObj(WndPtr);
		JetAPI::RemoveFolder(ModelFolderTmp);
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}
	if ( true == IsExceptionAngle )	
	{	WndPtr->RotateWnd(AttachedAngle, 0, 0);	}
	JetAPI::ClearUniFrameList(WndUniFrameList);
	if ( JetAPI::CopyFolderAToFolderB(PatternFolderTmp, PatternFolder, false, true, _T(""), -1, -1) == false ) 
	{	
		AOIObjManager.DestroyWndObj(WndPtr);
		JetAPI::RemoveFolder(ModelFolderTmp);
		return false;
	}	
	JetAPI::RemoveFolder(ModelFolderTmp);
	RefWndPtr->SynchronousWnd(WndPtr);
	AOIObjManager.DestroyWndObj(WndPtr);

	CAlgParam &RefAlgParam = RefWndPtr->GetWndAlgParam();
	RefAlgParam.SetAlgPatternFolder(PatternFolder);	
	RefModelPtr->ApplyModelWnd(RefWndPtr);

	if ( NULL != ComponentPtr ) 
	{	AOIDataCollect.CloseActiveComponent(ComponentPtr); }

	m_PatternCount = RefAlgParam.GetAlgPatternCount();	
	if ( m_PatternIndex >= m_PatternCount )
	{	m_PatternIndex = m_PatternCount-1; }
	m_IndexSpin.SetRange((short)0, (short)(m_PatternCount));
	UpdateParamToUI(NULL);	
	UpdatePatternParamToParentWnd();
	if ( SwitchPatternImage(m_PatternIndex) == false )
	{
		ResetPatternParam();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecUpdateBinaryToAlg()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CString  str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }	
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CPatternParam *PatParamPtr=AlgParam.GetAlgPatternParamPtr(m_PatternIndex, true);
	if ( NULL == PatParamPtr ) { return true; }
	str = _T("Do you want to update pattern binary param to algorithm?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return true; }

	const CAlgBinaryParam &BinaryParam = PatParamPtr->GetBinaryParam();	
	AlgParam.SetAlgImageBinParam(BinaryParam);

	HWND hWnd=CWnd::GetSafeHwnd();
	AOIDataCollect.PostParentWndMessage(hWnd, MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_UPDATE_WND_ALG, (LPARAM)(WndPtr));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ExecUpdateBinaryToPattern()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }

	CString  str;
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return true; }	
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return true; }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CPatternParam *PatParamPtr=AlgParam.GetAlgPatternParamPtr(m_PatternIndex, true);
	if ( NULL == PatParamPtr ) { return true; }
	str = _T("Do you want to modify pattern binary param form algorithm?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return true; }
	const CAlgBinaryParam &BinaryParam = AlgParam.GetAlgImageBinParam();
	PatParamPtr->SetBinaryParam(BinaryParam);
	ModelPtr->ApplyModelWnd(WndPtr);
	if ( SwitchPatternImage(m_PatternIndex) == false )
	{	ResetPatternParam();	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::ResetPatternParam()//復歸樣板參數
{
	m_PatternIndex = -1;
	m_PatternParam = CPatternParam();
	UpdateParamToUI(NULL);
	m_Dib.ReleaseBuffer();	
	if ( CWnd::GetSafeHwnd() != NULL )
	{
		CreateBKImage();
		RedrawWnd();	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::UpdatePatternParamToParentWnd()
{
	CAOIProject *Project = AOIDataCollect.GetActiveProject();
	if ( NULL == Project ) { return; }
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return ; }
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return ; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return ; }	
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr ) { return ; }
	if ( Project->CheckProjectComponentValid(ComponentPtr) == false ) { return ; }	

	//ChangeDrawModelMode();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnDeltaposIndexSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	//bool bLockUIWnd = GetLockUIWnd();
	//if ( true == bLockUIWnd ) { return ; }

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
bool CEditImagePatternWnd::SwitchPatternImage(int index, bool UpdateUI)
{	
	CAOIWnd *WndPtr = m_WndPtr;
	if ( NULL == WndPtr ) { return false; }
	//CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();		
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( ModelPtr->CheckModelWndValid(WndPtr) == false ) { return false; }	
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	//if ( index<0 || index>=m_PatternCount ) { return false; }
	
	BOOL       bGray = CWnd::IsDlgButtonChecked(PATTERN_GRAY_CHK);
	BOOL       bBinary = CWnd::IsDlgButtonChecked(PATTERN_BINARY_CHK);
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		

	if ( true == IsExceptionAngle ) 
	{	WndToward = JetAPI::RotateToward(-AttachedAngle, WndToward);	}
	
	IMAGE_PTR  ImagePtr=NULL;
	CPatternParam *PatParamPtr=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;

	PatParamPtr = AlgParam.GetAlgPatternParamPtr(index, true);
	if ( NULL == PatParamPtr ) 
	{	return false; }
	if ( AlgParam.LoadAlgPatternImage(index, WndToward, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	
		//return false; 
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
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnBinaryChk() 
{
	// TODO: Add your control notification handler code here	
	BOOL bCheck = CWnd::IsDlgButtonChecked(PATTERN_BINARY_CHK);
	if ( TRUE == bCheck )
	{	CWnd::CheckDlgButton(PATTERN_GRAY_CHK, FALSE);	}	
	if ( SwitchPatternImage(m_PatternIndex) == false )
	{
		ResetPatternParam();
		return ;
	}
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::LockUIWnd(bool bLock)
{
	UINT CtrlID = 0;
	BOOL bEnable = true;		
	const bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLock ) 
	{	bEnable = FALSE; }
	else
	{	bEnable = TRUE; }	
	if ( true == bLockUIWnd )
	{	bEnable = FALSE;	}
	
	CtrlID = PATTERN_INDEX_SPIN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
	
	CtrlID = PATTERN_GRAY_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PATTERN_BINARY_CHK;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PATTERN_ADD_BTN;
	if ( AOIDataCollect.GetIsLockUIWnd() == true ) 
	{	JetAPI::EnableCtrlWnd(this, CtrlID, TRUE); }
	else
	{	JetAPI::EnableCtrlWnd(this, CtrlID, FALSE); }

	CtrlID = PATTERN_EDIT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PATTERN_DELETE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PATTERN_CLEAR_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PATTERN_TEXT_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);

	CtrlID = PATTERN_ADVANCE_BTN;
	JetAPI::EnableCtrlWnd(this, CtrlID, bEnable);
}
//-------------------------------------------------------------------------------------//
bool CEditImagePatternWnd::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	if ( true == AOIDataCollect.GetIsLockUIWnd() ) { return true; }
	return false;

	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();		
	if ( DRAW_MODEL_RESULT == DrawModelMode )	
	{	return true;	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnGrayChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(PATTERN_GRAY_CHK);
	if ( TRUE == bCheck )
	{	CWnd::CheckDlgButton(PATTERN_BINARY_CHK, FALSE);	}	
	if ( SwitchPatternImage(m_PatternIndex) == false )
	{
		ResetPatternParam();
		return ;
	}
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnShowInfoChk() 
{
	// TODO: Add your control notification handler code here
	CreateBKImage();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnEditBtn() 
{
	// TODO: Add your control notification handler code here
	ExecEditPattern();
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnUpdateBinToAlgBtn() 
{
	// TODO: Add your control notification handler code here
	ExecUpdateBinaryToAlg();
}
//-------------------------------------------------------------------------------------//
void CEditImagePatternWnd::OnUpdateBinToPatBtn() 
{
	// TODO: Add your control notification handler code here
	ExecUpdateBinaryToPattern();
}
//-------------------------------------------------------------------------------------//