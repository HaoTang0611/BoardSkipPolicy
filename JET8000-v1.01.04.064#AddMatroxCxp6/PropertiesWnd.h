#pragma once

//-------------------------------------------------------------------------------------//
#include "JETPropertyGridCtrl.h"
//-------------------------------------------------------------------------------------//
// CPropertiesWnd
class CPropertiesWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CPropertiesWnd)

public:
	CPropertiesWnd();
	virtual ~CPropertiesWnd();

	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CJETPropertyGridCtrl m_wndPropList;

	void InitPropList();
	void AdjustLayout();
	void SetPropListFont();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg LRESULT OnPropertyLClicked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyRClicked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//

