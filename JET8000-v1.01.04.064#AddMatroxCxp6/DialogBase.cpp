// DialogBase.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDialogBase dialog
CDialogBase::CDialogBase(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogBase)
	m_bModalMode = false;
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
CDialogBase::CDialogBase(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CDialogBase)
	m_bModalMode = false;
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDialogBase, CDialog)
	//{{AFX_MSG_MAP(CDialogBase)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDialogBase message handlers
//-------------------------------------------------------------------------------------//
bool CDialogBase::CheckModalMode()
{
	if ( false == m_bModalMode ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
INT_PTR CDialogBase::DoModal()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別
	m_bModalMode = true;
	return CDialog::DoModal();
}
//-------------------------------------------------------------------------------------//
BOOL CDialogBase::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( CheckModalMode() )
	{	AOIDataCollect.AddWaitUserInputCount(); }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CDialogBase::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if ( CheckModalMode() )
	{
		AOIDataCollect.ReleaseWaitUserInputCount();
		if ( AOIDataCollect.CheckWaitUserInputCountZero() )
		{	AOIDataCollect.RegistUserLastInputTickCount();	}	
	}
	m_bModalMode = false;
}
//-------------------------------------------------------------------------------------//

