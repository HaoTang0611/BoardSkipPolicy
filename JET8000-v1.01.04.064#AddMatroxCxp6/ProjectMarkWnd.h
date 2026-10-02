#if !defined(AFX_PROJECTMARKWND_H__4AA16D0B_CD22_4569_98B6_51B18B26D14A__INCLUDED_)
#define AFX_PROJECTMARKWND_H__4AA16D0B_CD22_4569_98B6_51B18B26D14A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectMarkWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMarkWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectMarkWnd : public CDialog
{
// Construction
public:
	CProjectMarkWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectMarkWnd)
	enum { IDD = IDD_PROJECT_MARK_WND };
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectMarkWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	CAOIProject*               GetProjectPtr();//取得專案指標	
	void                       SetProjectPtr(CAOIProject *ProjectPtr);//設定專案指標	
	bool                       SetMarkFrameIndex(unsigned int index);//設定標記引數
	bool                       SetMarkStagePos(double PosX, double PosY, double PosZ);//設定標記機台座標
	bool                       SetMarkImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr);//設定標記影像
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;	
	//---------------------------------------------------------------------------------//	
	LANE_ID                    m_LaneID;
	bool                       m_Modified;
	unsigned int               m_MarkFrameIndex;
	TPOINT3D                   m_StagePos;
	IMAGE_SIZE                 m_MarkW;
	IMAGE_SIZE                 m_MarkH;
	IMAGE_SIZE                 m_MarkStep;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_PTR                  m_MarkPtr;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ExecApply();
	bool                       ExecClose();
	bool                       ClearMarkBuffer();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectMarkWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnGoToBtn();
	afx_msg void OnSetPosBtn();
	afx_msg void OnClearBtn();
	afx_msg void OnClose();
	afx_msg void OnCenterLineChk();
	virtual void OnOK();
	virtual void OnCancel();	
	afx_msg void OnApplyBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTMARKWND_H__4AA16D0B_CD22_4569_98B6_51B18B26D14A__INCLUDED_)
