#if !defined(AFX_INPUTCOMBOXWND_H__E9A19BB5_7634_4311_96D6_6007D90E63B2__INCLUDED_)
#define AFX_INPUTCOMBOXWND_H__E9A19BB5_7634_4311_96D6_6007D90E63B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputComboxWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
typedef struct tagComboxNode
{
	void*       Ptr;	
	CString     Text;	
	DWORD_PTR   Data;
	tagComboxNode()
	{
		Ptr = NULL;		
		Text = _T("");
		Data = 0;
	}
} TComboxNode, *PComboxNode;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CInputComboxWnd dialog
//-------------------------------------------------------------------------------------//
class CInputComboxWnd : public CBaseDialog
{
// Construction
public:
	CInputComboxWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputComboxWnd)
	enum { IDD = IDD_INPUT_COMBO_WND };
	CComboBox	m_DataCombox1;
	CString	m_TitleLabel1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputComboxWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//		
	void                       SetWndPos(const POINT &Pos);
	//---------------------------------------------------------------------------------//
	int                        GetSelIndex1() const;
	TComboxNode&               GetSelNode();	
	void*                      GetSelPtr() const;
	DWORD_PTR                  GetSelData() const;
	LPCTSTR                    GetSelText() const;	
	//---------------------------------------------------------------------------------//
	void                       SetSelIndex1(int nSel);
	void                       SetParam1(LPCTSTR WndTxt, LPCTSTR Title, void* Default, const std::vector<TComboxNode> &DataList);
	void                       SetParam1(LPCTSTR WndTxt, LPCTSTR Title, LPCTSTR Default, const std::vector<TComboxNode> &DataList);
	void                       SetParam1(LPCTSTR WndTxt, LPCTSTR Title, DWORD_PTR Default, const std::vector<TComboxNode> &DataList);	
	//---------------------------------------------------------------------------------//
	
protected:
	//---------------------------------------------------------------------------------//	
	int                        m_CurSel;
	POINT                      m_WndPos;
	CString                    m_WndText;
	bool                       m_WndMovePos;
	TComboxNode                m_SelData;
	std::vector<TComboxNode>   m_DataList;
	//---------------------------------------------------------------------------------//		
	bool                       BuildComboxCtrl();
	int                        SearchIndex(void *Ptr, const std::vector<TComboxNode> &DataList);
	int                        SearchIndex(LPCTSTR Text, const std::vector<TComboxNode> &DataList);
	int                        SearchIndex(DWORD_PTR Data, const std::vector<TComboxNode> &DataList);
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputComboxWnd)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_INPUTCOMBOXWND_H__E9A19BB5_7634_4311_96D6_6007D90E63B2__INCLUDED_)
