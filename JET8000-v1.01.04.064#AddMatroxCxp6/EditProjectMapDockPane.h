#pragma once

#include "ProjectMapPane.h"
#include "ProjectMapWnd.h"
// CEditProjectMapDockPane
//-------------------------------------------------------------------------------------//
class CEditProjectMapDockPane : public CDockablePane
{
	DECLARE_DYNAMIC(CEditProjectMapDockPane)

public:
	CEditProjectMapDockPane();
	virtual ~CEditProjectMapDockPane();

public:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CMenu                      m_Menu;	
	CProjectMapPane            m_wndProjectMap;
	//CProjectMapWnd           m_wndProjectMap;
	void                       OnChangeVisualStyle();
	//---------------------------------------------------------------------------------//

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual void ShowPane(BOOL bShow, BOOL bDelay, BOOL bActivate);
};
//-------------------------------------------------------------------------------------//

