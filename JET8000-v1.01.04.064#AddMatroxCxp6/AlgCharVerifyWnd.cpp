// AlgCharVerifyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "AlgCharVerifyWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgCharVerifyWnd dialog
//-------------------------------------------------------------------------------------//
CAlgCharVerifyWnd::CAlgCharVerifyWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgCharVerifyWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgCharVerifyWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgCharVerifyWnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgCharVerifyWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgCharVerifyWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(ALGCHAR_ROI_ADD_BTN, OnRoiAddBtn)
	ON_BN_CLICKED(ALGCHAR_ROI_ADD_MATRIX_BTN, OnRoiAddMatrixBtn)
	ON_BN_CLICKED(ALGCHAR_ROI_DELETE_BTN, OnRoiDeleteBtn)
	ON_BN_CLICKED(ALGCHAR_ROI_CLEAR_BTN, OnRoiClearBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgCharVerifyWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgCharVerifyWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ALG_CHAR_VERIFY_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_ALG_CHAR_VERIFY_WND;
	WndKey = _T("IDD_ALG_CHAR_VERIFY_WND");
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
	WndID = ALGCHAR_ROI_GROUP;
	WndKey = _T("ALGCHAR_ROI_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ALGCHAR_ROI_ADD_BTN;
	WndKey = _T("ALGCHAR_ROI_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALGCHAR_ROI_ADD_MATRIX_BTN;
	WndKey = _T("ALGCHAR_ROI_ADD_MATRIX_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALGCHAR_ROI_DELETE_BTN;
	WndKey = _T("ALGCHAR_ROI_DELETE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = ALGCHAR_ROI_CLEAR_BTN;
	WndKey = _T("ALGCHAR_ROI_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	/*
	WndID = AAAAAAAAAAAAAAAAAAAAA;
	WndKey = _T("AAAAAAAAAAAAAAAAAAAAA");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	*/
}
//-------------------------------------------------------------------------------------//
CString CAlgCharVerifyWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_CHAR_VERIFY_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAlgCharVerifyWnd::GetWndPtr()
{
	return m_WndPtr;
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::SetWndPtr(CAOIWnd *WndPtr)
{
	m_WndPtr = WndPtr;
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::SetWndUniFrameList(std::vector<TUNI_FRAME> &UniFrameList)
{
	m_UniFrameList = UniFrameList;
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::OnRoiAddBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIWnd *WndPtr = GetWndPtr();
	if ( NULL == WndPtr ) { return; }

	TREGION4D     WndRgn;
	TREGION4D     WndRoiRgn;
	CAOIWndRoi   *WndRoiPtr = NULL;
	WndRoiPtr = AOIObjManager.CreateWndRoiObj();
	if ( NULL == WndRoiPtr )
	{		
		JetAPI::ShowMessageBox(_T("Error, Create Wnd Roi Obj Fault"));
		return;
	}
	WndPtr->GetWndRegion(WndRgn);
	const double WndCpX = WndRgn.GetCpX();
	const double WndCpY = WndRgn.GetCpY();
	const double WndSizeW = WndRgn.GetWidth();
	const double WndSizeH = WndRgn.GetHeight();
	const double WndRoiSizeW = WndSizeW/2;
	const double WndRoiSizeH = WndSizeH/2;
	WndRoiRgn.minX = WndCpX-(WndRoiSizeW/2);
	WndRoiRgn.minY = WndCpY-(WndRoiSizeH/2);
	WndRoiRgn.maxX = WndCpX+(WndRoiSizeW/2);
	WndRoiRgn.maxY = WndCpY+(WndRoiSizeH/2);
	
	WndRoiPtr->SetWndRoiSelected(true);
	WndRoiPtr->SetWndRoiRegion(WndRoiRgn);
	WndRoiPtr->SetWndRoiToward(WndPtr->GetWndToward());

	WndPtr->SetWndAllRoiWndActived(false);
	WndPtr->SetWndAllRoiWndSelected(false);
	WndPtr->AddWndRoiWndPtr(WndRoiPtr, false);
	WndPtr->SetWndRoiWndActived(WndRoiPtr);
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::OnRoiAddMatrixBtn() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::OnRoiDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgCharVerifyWnd::OnRoiClearBtn() 
{
	// TODO: Add your control notification handler code here
	
}
//-------------------------------------------------------------------------------------//