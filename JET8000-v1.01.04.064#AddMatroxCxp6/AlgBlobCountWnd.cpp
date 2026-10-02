// AlgBlobCountWnd.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "AlgBlobCountWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBlobCountWnd dialog
//-------------------------------------------------------------------------------------//
CAlgBlobCountWnd::CAlgBlobCountWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgBlobCountWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgBlobCountWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_WndPtr = NULL;
}
//-------------------------------------------------------------------------------------//
void CAlgBlobCountWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgBlobCountWnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgBlobCountWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgBlobCountWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBlobCountWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgBlobCountWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgBlobCountWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	//JetAPI::ClearUniFrameList(m_UniFrameList);
}
//-------------------------------------------------------------------------------------//
void CAlgBlobCountWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgBlobCountWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_ALG_BLOB_COUNT_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_ALG_BLOB_COUNT_WND;
	WndKey = _T("IDD_ALG_BLOB_COUNT_WND");
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
	//WndID = BARCON_BARCODE_LABEL;
	//WndKey = _T("BARCON_BARCODE_LABEL");
	//this->GetDlgItemText(WndID, LabelText);
	//AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	//this->SetDlgItemText(WndID, NewLabelText);	
}
//-------------------------------------------------------------------------------------//
CString CAlgBlobCountWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_BLOB_COUNT_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAlgBlobCountWnd::GetWndPtr()
{
	return m_WndPtr;
}
//-------------------------------------------------------------------------------------//
void CAlgBlobCountWnd::SetWndPtr(CAOIWnd *WndPtr)
{
	m_WndPtr = WndPtr;
}
//-------------------------------------------------------------------------------------//
void CAlgBlobCountWnd::SetWndUniFrameList(std::vector<TUNI_FRAME> &UniFrameList)
{
	m_UniFrameList = UniFrameList;
}
//-------------------------------------------------------------------------------------//