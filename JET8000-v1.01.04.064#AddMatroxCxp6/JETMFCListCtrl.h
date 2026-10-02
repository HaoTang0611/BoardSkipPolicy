#if !defined(AFX_JETMFCLISTCTRL_H__ED6FB957_7EBB_405E_95E6_6A19F4C846A2__INCLUDED_)
#define AFX_JETMFCLISTCTRL_H__ED6FB957_7EBB_405E_95E6_6A19F4C846A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JETListCtrl.h : header file
//
//-------------------------------------------------------------------------------------//
#if _MSC_VER == VC_6
	#pragma warning(disable : 4786)
#endif
//-------------------------------------------------------------------------------------//
#include <map>
#include <vector>
#define JET_LIST_MAX_COLUMNS               10000
//-------------------------------------------------------------------------------------//
#define CMFCBasicListCtrl     CMFCListCtrl//基礎的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CJETMFCListCtrl window
//-------------------------------------------------------------------------------------//
class CJETMFCListCtrl : public CMFCBasicListCtrl
{	
	//---------------------------------------------------------------------------------//	
	typedef struct _LVCOLOR
	{
		COLORREF clrText;
		COLORREF clrTextBk;
		_LVCOLOR()
		{
			clrText = CLR_DEFAULT;
			clrTextBk = CLR_DEFAULT;
		}
		_LVCOLOR(COLORREF text, COLORREF textbk)
		{
			clrText = text;
			clrTextBk = textbk;
		}
	} LVCOLOR, *PLVCOLOR;
	//---------------------------------------------------------------------------------//	
// Construction
public:
	CJETMFCListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJETMFCListCtrl)
	//}}AFX_VIRTUAL
// Implementation
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	//---------------------------------------------------------------------------------//	
	std::map<unsigned int, LVCOLOR> m_CellTextColorMap;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	int          GetColumnCount();
	unsigned int CalcItemColorKey(int nItem, int nSubItem=-1);
	void DeleteItemColor(int nItem);
	void InsertItemColor(int nItem, COLORREF clrText, COLORREF clrTextBk);
	void InsertItemColor(int nItem, int nSubItem, COLORREF clrText, COLORREF clrTextBk);
	int  InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	int InsertItem(const LVITEM* pItem);
	int InsertItem(int nItem, LPCTSTR lpszItem);
	int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
	//---------------------------------------------------------------------------------//
	BOOL DeleteItem(int nItem);
	BOOL DeleteAllItems();
	//---------------------------------------------------------------------------------//	
	BOOL SetItemTextColor(int nItem, COLORREF clrText);	
	BOOL SetItemTextColor(int nItem, int nSubItem, COLORREF clrText);	
	BOOL SetItemTextBkColor(int nItem, COLORREF clrTextBk);
	BOOL SetItemTextBkColor(int nItem, int nSubItem, COLORREF clrTextBk);
	//---------------------------------------------------------------------------------//
public:
	virtual ~CJETMFCListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CJETMFCListCtrl)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JETMFCLISTCTRL_H__ED6FB957_7EBB_405E_95E6_6A19F4C846A2__INCLUDED_)
