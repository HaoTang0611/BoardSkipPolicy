#if !defined(AFX_BARCODELISTWND_H__30C8B1B5_F196_45AC_AA6B_396079E821A2__INCLUDED_)
#define AFX_BARCODELISTWND_H__30C8B1B5_F196_45AC_AA6B_396079E821A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarcodeListWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_49     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_49     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeListWnd dialog

class CBarcodeListWnd : public CBaseDialog
{
// Construction
public:
	CBarcodeListWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarcodeListWnd)
	enum { IDD = IDD_BARCODE_LIST_WND };
	CEdit           m_EditCtrl;
	CThisListCtrl_49	m_BarcodeListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarcodeListWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//-------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr);
	int                        CompareBarcodeItem(size_t index1, size_t index2);
	//-------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------//	
	int                        m_BarcodeColID;
	int                        m_BarcodeListSortMode;
	bool                       m_StopBarcodeListBeSelected;
	//-------------------------------------------------------------------------//
	int                        m_nItemAct;
	int                        m_nSubItemAct;
	CAOIBarcode               *m_BarcodePtr;
	CAOIProject               *m_ProjectPtr;	
	std::vector<CAOIBarcode*>  m_BarcodeList; 
	//-------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------//	
	bool                       BuildBarcodeListWnd();
	bool                       UpdateBarcodeListWnd();
	bool                       BuildBarcodeListWndHeader();
	//-------------------------------------------------------------------------//
	bool                       ClearBarcodeListWnd();
	//-------------------------------------------------------------------------//
	void                       SetBarcodeListColID(int val);
	int                        GetBarcodeListColD() const;
	//-------------------------------------------------------------------------//
	void                       SetBarcodeListSortMode(int val);
	int                        GetBarcodeListSortMode() const;	
	//-------------------------------------------------------------------------//	
	bool                       ExecUpdateParamByEdit();
	bool                       ExecReleaseParamCtrl();
	//-------------------------------------------------------------------------//
	void                       SetItemIndexAct(int nItem, int nSubItem);
	//-------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarcodeListWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnColumnclickBarcodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedBarcodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkBarcodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODELISTWND_H__30C8B1B5_F196_45AC_AA6B_396079E821A2__INCLUDED_)
