#pragma once
//-------------------------------------------------------------------------------------//
#include "EditImageView.h"
//-------------------------------------------------------------------------------------//
// CEditImageDockPane
class CEditImageDockPane : public CDockablePane
{
	DECLARE_DYNAMIC(CEditImageDockPane)

public:
	CEditImageDockPane();
	virtual ~CEditImageDockPane();

	void                       OnChangeVisualStyle();
protected:	
	//---------------------------------------------------------------------------------//
	CEditImageView             m_EditImageView;
	//---------------------------------------------------------------------------------//
	void                       SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	void                       PotMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
//-------------------------------------------------------------------------------------//