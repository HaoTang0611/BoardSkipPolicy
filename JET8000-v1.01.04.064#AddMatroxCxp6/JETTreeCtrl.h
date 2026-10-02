#if !defined(AFX_JETTREECTRL_H__B2AAEE09_0E7A_44DD_8407_E4A433F1DF6E__INCLUDED_)
#define AFX_JETTREECTRL_H__B2AAEE09_0E7A_44DD_8407_E4A433F1DF6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JETTreeCtrl.h : header file
//
//-------------------------------------------------------------------------------------//
#if _MSC_VER == VC_6
	#pragma warning(disable : 4786)
#endif
//-------------------------------------------------------------------------------------//
#include <map>
#define JET_LIST_MAX_COLUMNS               10000
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CJETTreeCtrl window
//-------------------------------------------------------------------------------------//
class CJETTreeCtrl : public CBasicTreeCtrl
{
	//---------------------------------------------------------------------------------//	
	typedef struct _TVCOLOR
	{
		COLORREF clrText;
		COLORREF clrTextBk;
		_TVCOLOR()
		{
			clrText = CLR_DEFAULT;
			clrTextBk = CLR_DEFAULT;
		}
		_TVCOLOR(COLORREF text, COLORREF textbk)
		{
			clrText = text;
			clrTextBk = textbk;
		}
	} TVCOLOR, *PTVCOLOR;
	//---------------------------------------------------------------------------------//	
// Construction
public:
	CJETTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJETTreeCtrl)
	//}}AFX_VIRTUAL
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
private:
	//---------------------------------------------------------------------------------//	
	UINT                         m_nSBCode;
	UINT                         m_ScrollBarPosEnd;
	UINT                         m_ScrollBarPosBegin;
	//---------------------------------------------------------------------------------//	
	std::map<HTREEITEM, TVCOLOR> m_ItemTextColorMap;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	void SetItemTextColor(HTREEITEM hItem, COLORREF clrText);
	void SetItemTextBKColor(HTREEITEM hItem, COLORREF clrTextBk);
	//---------------------------------------------------------------------------------//
	BOOL DeleteItem(HTREEITEM hItem);
	BOOL DeleteAllItems();
	//---------------------------------------------------------------------------------//
public:
	virtual ~CJETTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CJETTreeCtrl)
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JETTREECTRL_H__B2AAEE09_0E7A_44DD_8407_E4A433F1DF6E__INCLUDED_)
