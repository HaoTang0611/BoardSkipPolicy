// EditFormView.cpp : implementation of the CEditFormView class
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JET8000.h"
#include "MainDoc.h"
#include "EditFormView.h"
//-------------------------------------------------------------------------------------//
#include "DebugWnd.h"
#include "Draw3DWnd.h"
#include "ImageDebugWnd.h"
#include "NewProjectWizardWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFormView
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CEditFormView, CFormView)
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditFormView, CFormView)
	//{{AFX_MSG_MAP(CEditFormView)	
	ON_WM_DESTROY()		
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)	
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFormView construction/destruction
//-------------------------------------------------------------------------------------//
CEditFormView::CEditFormView()
	: CFormView(CEditFormView::IDD)
{
	//{{AFX_DATA_INIT(CEditFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}
//-------------------------------------------------------------------------------------//
CEditFormView::~CEditFormView()
{
}
//-------------------------------------------------------------------------------------//
void CEditFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BOOL CEditFormView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}
//-------------------------------------------------------------------------------------//
void CEditFormView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();//會讓主視窗調整成FormView的尺寸
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFormView printing
//-------------------------------------------------------------------------------------//
BOOL CEditFormView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}
//-------------------------------------------------------------------------------------//
void CEditFormView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}
//-------------------------------------------------------------------------------------//
void CEditFormView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}
//-------------------------------------------------------------------------------------//
void CEditFormView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFormView diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CEditFormView::AssertValid() const
{
	CFormView::AssertValid();
}
//-------------------------------------------------------------------------------------//
void CEditFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
//-------------------------------------------------------------------------------------//
CMainDoc* CEditFormView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMainDoc)));
	return (CMainDoc*)m_pDocument;
}
//-------------------------------------------------------------------------------------//
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFormView message handlers
//-------------------------------------------------------------------------------------//
void CEditFormView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here	
}
//-------------------------------------------------------------------------------------//