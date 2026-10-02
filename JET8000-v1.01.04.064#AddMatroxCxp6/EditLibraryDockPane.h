#pragma once
//-------------------------------------------------------------------------------------//
#include "EditLibraryWnd.h"
//-------------------------------------------------------------------------------------//
// CEditLibraryDockPane
//-------------------------------------------------------------------------------------//
class CEditLibraryDockPane : public CDockablePane
{
	DECLARE_DYNAMIC(CEditLibraryDockPane)

public:
	CEditLibraryDockPane();
	virtual ~CEditLibraryDockPane();

public:
	
protected:
	//---------------------------------------------------------------------------------//
	CEditLibraryWnd          m_wndLibrary;
	//---------------------------------------------------------------------------------//
	void                     OnChangeVisualStyle();
	//---------------------------------------------------------------------------------//
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate);
};
//-------------------------------------------------------------------------------------//

