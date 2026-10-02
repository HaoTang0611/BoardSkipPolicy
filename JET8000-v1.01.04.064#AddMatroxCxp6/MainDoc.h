// MainDoc.h : interface of the CMainDoc class
//
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#if !defined(AFX_MAINDOC_H__0CFC6F0D_6BB8_49E1_848C_473D20A450AD__INCLUDED_)
#define AFX_MAINDOC_H__0CFC6F0D_6BB8_49E1_848C_473D20A450AD__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
class CMainDoc : public CDocument
{
protected: // create from serialization only
	CMainDoc();
	DECLARE_DYNCREATE(CMainDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//		
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	
// Implementation
public:
	virtual ~CMainDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMainDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDOC_H__0CFC6F0D_6BB8_49E1_848C_473D20A450AD__INCLUDED_)
