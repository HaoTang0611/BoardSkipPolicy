#if !defined(AFX_BARCODEDEVICEWND_H__4447E1FE_8DA5_4D8A_89AA_FA38B26F50FA__INCLUDED_)
#define AFX_BARCODEDEVICEWND_H__4447E1FE_8DA5_4D8A_89AA_FA38B26F50FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarcodeDeviceWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeDeviceWnd dialog
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_03     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
class CBarcodeDeviceWnd : public CBaseDialog
{
// Construction
public:
	CBarcodeDeviceWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarcodeDeviceWnd)
	enum { IDD = IDD_BARCODE_DEVICE_WND };
	CComboBox	m_BarcodeIDComboxCtrl;	
	CThisListCtrl_03	m_BarcodeCodeListCtrl;
	CThisListCtrl_03	m_LaneBarcodeListCtrl;
	CComboBox	m_LaneIDCombox;
	CComboBox	m_DeviceTypeCombox;
	CComboBox	m_DeviceIDCombox;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarcodeDeviceWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	int                        m_LaneBarcodeIndex;
	TBarcodeDevice             m_BarcodeDeviceParam;
	int                        m_LaneBarcodeIDList[MAX_BARCODE_DEVICE_COUNT];
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	LANE_ID                    GetActiveLaneID();
	//---------------------------------------------------------------------------------//	
	bool                       BuildBarcodeCodeListWnd();
	bool                       BuildBarcodeCodeListWnd_Header();
	//---------------------------------------------------------------------------------//	
	bool                       BuildLaneBarcodeDeviceIDListWnd();
	bool                       BuildLaneBarcodeDeviceIDListWnd_Header();
	bool                       ShowLaneBarcodeDeviceIDCombox(CThisListCtrl_03 &ListCtrl, int nItem, int nSubItem);
	//---------------------------------------------------------------------------------//	
	bool                       SwitchBarcodeDeviceID();
	bool                       SwitchBarcodeDeviceType();	
	//---------------------------------------------------------------------------------//
	bool                       ReadBarcodeDeviceCode(bool CheckReceieve);//讀取條碼內容
	void                       StopBarcodeDeviceCodeTimer();//停止讀取條碼內容
	void                       StartpBarcodeDeviceCodeTimer();//停止讀取條碼內容
	bool                       UpdateBarcodeDeviceToUI(bool UpdateType);
	bool                       LockUIWnd(bool bLock);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarcodeDeviceWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeDeviceIDCombox();
	afx_msg void OnSelchangeDeviceTypCombox();
	afx_msg void OnDeviceReadBtn();
	afx_msg void OnDeviceStopBtn();
	afx_msg void OnDeviceAdvanceBtn();
	afx_msg void OnDeviceConnectChk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	afx_msg void OnDeviceLanePCBInBtn();
	afx_msg void OnDeviceLanePCBOutBtn();
	afx_msg void OnDeviceLanePCBBackBtn();
	afx_msg void OnDeviceLaneClampOnBtn();
	afx_msg void OnDeviceLaneClampOffBtn();
	afx_msg void OnDeviceLaneBarcodeListEnableAllBtn();
	afx_msg void OnDeviceLaneBarcodeListDisableAllBtn();
	afx_msg void OnSelchangeDeviceLaneIdCombo();
	afx_msg void OnDblclkDeviceLaneBarcodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDeviceLaneBarcodeIdCombo();
	afx_msg void OnKillfocusDeviceLaneBarcodeIdCombo();
	afx_msg void OnDeviceDefaultBtn();
	afx_msg void OnItemchangedDeviceCodeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODEDEVICEWND_H__4447E1FE_8DA5_4D8A_89AA_FA38B26F50FA__INCLUDED_)
