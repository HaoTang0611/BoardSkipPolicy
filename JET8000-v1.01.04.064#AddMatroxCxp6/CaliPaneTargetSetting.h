#if !defined(AFX_CALIPANETARGETSETTING_H__501838A1_9E9C_48D4_8B15_08947CA9F91B__INCLUDED_)
#define AFX_CALIPANETARGETSETTING_H__501838A1_9E9C_48D4_8B15_08947CA9F91B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaliPaneTargetSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCaliPaneTargetSetting dialog

class CCaliPaneTargetSetting : public CDialog
{
// Construction
public:
	CCaliPaneTargetSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCaliPaneTargetSetting)
	enum { IDD = IDD_CALIBRATION_PANE_TARGET_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaliPaneTargetSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	//---------------------------------------------------------------------------------//	
	void                       UpdateTargetPosToUI();
	void                       SwitchMultiLanguage();	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCaliPaneTargetSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnGridPosGoBtn();
	afx_msg void OnRectPosGoBtn();
	afx_msg void OnWhitePosGoBtn();
	afx_msg void OnHeightPosGoBtn();
	afx_msg void OnGridPosSetBtn();
	afx_msg void OnRectPosSetBtn();
	afx_msg void OnWhitePosSetBtn();
	afx_msg void OnHeightPosSetBtn();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIPANETARGETSETTING_H__501838A1_9E9C_48D4_8B15_08947CA9F91B__INCLUDED_)
