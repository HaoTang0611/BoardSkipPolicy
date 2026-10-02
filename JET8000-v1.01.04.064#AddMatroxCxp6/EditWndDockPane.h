#pragma once
//-------------------------------------------------------------------------------------//
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
// CEditWndDockPane
class CEditWndDockPane: public CDockablePane
{
	DECLARE_DYNAMIC(CEditWndDockPane)

public:
	CEditWndDockPane();
	virtual ~CEditWndDockPane();
	//---------------------------------------------------------------------------------//
	void SetVSDotNetLook(BOOL bSet);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CEditWndView               m_EditWndView;
	//---------------------------------------------------------------------------------//
	void                       SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	void                       PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
//-------------------------------------------------------------------------------------//