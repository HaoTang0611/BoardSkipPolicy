// MainDoc.cpp : implementation of the CMainDoc class
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JET8000.h"
#include "MainDoc.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainDoc
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CMainDoc, CDocument)
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
	//{{AFX_MSG_MAP(CMainDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainDoc construction/destruction
//-------------------------------------------------------------------------------------//
CMainDoc::CMainDoc()
{
	// TODO: add one-time construction code here

}
//-------------------------------------------------------------------------------------//
CMainDoc::~CMainDoc()
{
}
//-------------------------------------------------------------------------------------//
BOOL CMainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainDoc serialization
//-------------------------------------------------------------------------------------//
void CMainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainDoc diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CMainDoc::AssertValid() const
{
	CDocument::AssertValid();
}
//-------------------------------------------------------------------------------------//
void CMainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMainDoc commands
//-------------------------------------------------------------------------------------//
BOOL CMainDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))//會自動開檔, 跑到CMainDoc::Serialize(CArchive& ar), 因此不要呼叫
		return FALSE;

	// TODO:  在此加入特別建立的程式碼	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CMainDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	return CDocument::OnSaveDocument(lpszPathName);
}
//-------------------------------------------------------------------------------------//
void CMainDoc::OnCloseDocument()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	CDocument::OnCloseDocument();
}
//-------------------------------------------------------------------------------------//