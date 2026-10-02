// PageDialogbarPartNumber.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "PageDialogbarPartNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageDialogbarPartNumber dialog


CPageDialogbarPartNumber::CPageDialogbarPartNumber(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CPageDialogbarPartNumber)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPageDialogbarPartNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageDialogbarPartNumber)
	DDX_Control(pDX, PPN_TYPE_COMBO, m_TypeCombox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageDialogbarPartNumber, CDialogBar)
	//{{AFX_MSG_MAP(CPageDialogbarPartNumber)
	ON_MESSAGE(WM_INITDIALOG , OnInitDialog)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageDialogbarPartNumber message handlers

LRESULT CPageDialogbarPartNumber::OnInitDialog(WPARAM wParam, LPARAM lParam) 
{
	//CDialogBar::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL bRet = HandleInitDialog(wParam, lParam);
	if ( !UpdateData(FALSE))//在此之後會自動建立控制項的物件
	{
		TRACE0("Warning: UpdateData failed during dialog init.\n");
		return FALSE;
	}

	if ( m_TypeCombox.GetSafeHwnd() != NULL )
	{
		m_TypeCombox.InsertString(-1, _T("None"));
		m_TypeCombox.InsertString(-1, _T("Board-1"));
		m_TypeCombox.InsertString(-1, _T("Board-2"));
		m_TypeCombox.SetCurSel(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageDialogbarPartNumber::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	CWnd *pWnd = CWnd::GetParent();
	CWnd *pWnd2 = CWnd::GetOwner();
	CWnd *pWnd3 = this;
	CDialogBar::OnUpdateCmdUI((CFrameWnd*)pWnd, bDisableIfNoHandler);	
}


