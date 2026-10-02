#if !defined(AFX_MESSAGEBOXWND_H__E4504A06_EA9E_4CCB_9451_07BE0C52E15C__INCLUDED_)
#define AFX_MESSAGEBOXWND_H__E4504A06_EA9E_4CCB_9451_07BE0C52E15C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <map>
#include <vector>
#include "resource.h"
// MessageBoxWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#define MSG_BOX_BTN_MAX_COUNT            3
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMessageBoxWnd dialog
//-------------------------------------------------------------------------------------//
class CMessageBoxWnd : public CBaseDialog
{
// Construction
public:
	CMessageBoxWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMessageBoxWnd)
	enum { IDD = IDD_MESSAGE_BOX_WND };
	CString	m_MessageText;
	CStatic	m_MessageIcon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageBoxWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//		
	void                       SetTextColor(COLORREF clr);//設定文字顏色
	void                       SetTextBkColor(COLORREF clr);//設定背景顏色
	void                       SetWndTitleText(LPCTSTR str);//設定視窗標題
	void                       SetFontInfo(int nSize, LPCWSTR sName=NULL);//設定字型參數
	int                        ShowMessageWnd(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp=0);//顯示訊息視窗
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CString                    m_WndTitleText;
	UINT                       m_BoxType;//視窗樣式
	std::vector<UINT>          m_BtnIDList;//按鈕編號列表
	std::map<UINT, bool>       m_BtnUsdMap;//按鈕使用列表
	HICON                      m_MsgIcon;
	//---------------------------------------------------------------------------------//
	//bool                       m_FontUsed;//字型使用
	int                        m_FontSize;//字型大小
	CString                    m_FontName;//字型名稱
	CFont                      m_TextFont;//字型原件	
	//---------------------------------------------------------------------------------//	
	COLORREF                   m_TextColor;//文字顏色
	COLORREF                   m_TextBkColor;//背景顏色
	CBrush                     m_TextBkBrush;//背景顏色	
	//---------------------------------------------------------------------------------//
	bool                       SetBoxType(UINT Type);
	UINT                       GetMsgBtnType() const;//取得訊息按鈕樣式
	UINT                       GetDefBtnType() const;//取得預設按鈕樣式
	UINT                       GetMsgIconType() const;//取得訊息圖示樣式
	void                       SetMessageText(LPCTSTR str);//設定訊息文字
	//---------------------------------------------------------------------------------//
	bool                       CreateWndFont();//建立視窗字型
	bool                       ReSizeMsgWnd();//調整訊息視窗尺寸
	bool                       InitalBtnWnd(UINT BtnID);	
	UINT                       GetMessageIconID() const;//取得訊息圖示編號
	UINT                       GetMessageBtnID(UINT ID) const;//取得訊息按鈕編號
	int                        GetMessageBtnCountUsed();//取得訊息按鈕使用數量
	SIZE                       GetMessageBtnSize();//取得訊息按鈕尺寸
	bool                       LoadMessageIcon();//載入訊息圖示
	SIZE                       GetMessageIconSize() const;//取得訊息圖示尺寸
	bool                       ShowCtrlWnd(UINT ID, BOOL bShow);//顯示控制視窗
	bool                       CheckUseTextFont() const;//確認使用文字字型
	bool                       CheckUIUseTextFont() const;//確認介面使用字型
	bool                       CheckUseTextColor() const;//確認使用文字顏色
	bool                       CheckUseTextBkColor() const;//確認使用背景顏色	
	bool                       CheckEnableCloseBtn() const;//確認啟用[關閉]按鈕
	bool                       CalcMsgStringSize(CWnd *WndPtr, LPCTSTR str, bool bUserFont, int &szX, int &szY);//計算訊息字串
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageBoxWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	
	virtual void OnCancel();
	afx_msg void OnDefBtn1();
	afx_msg void OnDefBtn2();
	afx_msg void OnDefBtn3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEBOXWND_H__E4504A06_EA9E_4CCB_9451_07BE0C52E15C__INCLUDED_)
