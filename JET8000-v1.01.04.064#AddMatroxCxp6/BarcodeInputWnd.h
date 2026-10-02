#if !defined(AFX_BARCODEINPUTWND_H__0CF9F8C9_A6D8_4034_B55A_82689C178ECF__INCLUDED_)
#define AFX_BARCODEINPUTWND_H__0CF9F8C9_A6D8_4034_B55A_82689C178ECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarcodeInputWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "JETListCtrl.h"
#include "Barcode_Handheld.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeInputWnd dialog
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_04     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
class CBarcodeInputWnd : public CBaseDialog
{
// Construction
public:
	CBarcodeInputWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarcodeInputWnd)
	enum { IDD = IDD_BARCODE_INPUT_WND };
	CThisListCtrl_04	m_BarcodeListCtrl;
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarcodeInputWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	void                      SetLaneID(LANE_ID LaneID);
	void                      SetProjectPtr(CAOIProject *ProjectPtr);
	void                      SetBarcodeReadMode(BARCODE_HANDHELD_READ_MODE ReadMode);
	//---------------------------------------------------------------------------------//	
	CBarcode_Handheld&        GetBarcodeHandheld();
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	LANE_ID                    m_LaneID;//20230425
	CAOIProject               *m_ProjectPtr;
	CBarcode_Handheld          m_BarcodeHandHeld;
	UINT                       m_BarcodeScope;
	BARCODE_HANDHELD_READ_MODE m_BarcodeReadMode;
	//---------------------------------------------------------------------------------//	
	bool                       InitialProjectMapWnd();
	//---------------------------------------------------------------------------------//	
	bool                       BuildBarcodeInfoListWnd();
	bool                       BuildBarcodeInfoListWnd_Header();
	//---------------------------------------------------------------------------------//	
	LANE_ID                    GetLaneID() const;
	CAOIProject*               GetActiveProjectPtr();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ExecInputBarcode(LPCTSTR Caption, LPCTSTR Label, LPCTSTR Default, CString &Value);	
	//---------------------------------------------------------------------------------//
	bool                       ExecBarcodeInput();//手動輸入
	bool                       ExecBarcodeInput_Panel();
	bool                       ExecBarcodeInput_Board();
	bool                       ExecBarcodeInput_Project();
	//---------------------------------------------------------------------------------//
	bool                       ExecBarcodeSequenceInput();//依序輸入條碼
	bool                       ExecBarcodeSequenceInput_Panel();
	bool                       ExecBarcodeSequenceInput_Board();
	bool                       ExecBarcodeSequenceInput_Project();
	//---------------------------------------------------------------------------------//
	void                       EnableCloseBtn(BOOL bEnable);//啟用關閉按鈕
	//---------------------------------------------------------------------------------//
	void                       OnImageWndNotify(WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarcodeInputWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnProjectScopeRadio();
	afx_msg void OnPanelScopeRadio();
	afx_msg void OnBoardScopeRadio();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnInputSequenceBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODEINPUTWND_H__0CF9F8C9_A6D8_4034_B55A_82689C178ECF__INCLUDED_)
