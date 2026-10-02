#if !defined(AFX_BARCODEGENERALWND_H__8EACB592_B9A0_47FE_AB69_90F451D3FADE__INCLUDED_)
#define AFX_BARCODEGENERALWND_H__8EACB592_B9A0_47FE_AB69_90F451D3FADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarcodeGeneralWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "Barcode_General.h"
/////////////////////////////////////////////////////////////////////////////
// CBarcodeGeneralWnd dialog
//-------------------------------------------------------------------------------------//
class CBarcodeGeneralWnd : public CBaseDialog
{
// Construction
public:
	CBarcodeGeneralWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarcodeGeneralWnd)
	enum { IDD = IDD_BARCODE_GENERAL_WND };
	CComboBox	m_PortCombox;
	CComboBox	m_BaudCombox;
	CComboBox	m_ParityCombox;
	CComboBox	m_StopBitsCombox;		
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarcodeGeneralWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodePtr(CBarcode_General *BarcodePtr);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CBarcode_General          *m_BarcodePtr;
	CBarcode_General*          GetBarcodePtr();
	bool                       CheckBarcodePtr(CBarcode_General *Ptr);
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       UpdateToBarcodeDeviceUI(CBarcode_General *BarcodePtr);
	bool                       UpdateToBarcodeDeviceUIState(CBarcode_General *BarcodePtr);
	bool                       UpdateToBarcodeDevice(CBarcode_General *BarcodePtr, bool bSave=false);
	bool                       SetBarcodeStartEditText(LPCTSTR Text);
	bool                       SetBarcodeStopEditText(LPCTSTR Text);
	bool                       SetBarcodeReadEditText(LPCTSTR Text);
	bool                       SetBarcodeSeparatorEditText(LPCTSTR Text);
	bool                       SetBarcodeCodePrefixEditText(LPCTSTR Text);
	bool                       SetBarcodeCodeSuffixEditText(LPCTSTR Text);
	//---------------------------------------------------------------------------------//	
	bool                       ExecAddChar(LPCTSTR Fnuc, UINT CtrlID);
	bool                       ExecAddString(LPCTSTR Fnuc, UINT CtrlID);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarcodeGeneralWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnConnectChk();
	afx_msg void OnStartBtn();	
	afx_msg void OnStartClearBtn();
	afx_msg void OnStartAddCharBtn();
	afx_msg void OnStartAddStringBtn();
	afx_msg void OnStopBtn();
	afx_msg void OnStopClearBtn();
	afx_msg void OnStopAddCharBtn();
	afx_msg void OnStopAddStringBtn();
	afx_msg void OnReadBtn();	
	afx_msg void OnCodeSeparatorClearBtn();
	afx_msg void OnCodeSeparatorAddCharBtn();
	afx_msg void OnCodeSeparatorAddStringBtn();
	afx_msg void OnCodePrefixClearBtn();
	afx_msg void OnCodePrefixAddCharBtn();
	afx_msg void OnCodePrefixAddStringBtn();
	afx_msg void OnCodeSuffixClearBtn();
	afx_msg void OnCodeSuffixAddCharBtn();
	afx_msg void OnCodeSuffixAddStringBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODEGENERALWND_H__8EACB592_B9A0_47FE_AB69_90F451D3FADE__INCLUDED_)
