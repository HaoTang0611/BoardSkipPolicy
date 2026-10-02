#if !defined(AFX_SPACENOISEFILTERPARAMWND_H__4E41A642_9B07_41D3_B69E_BA15B229B121__INCLUDED_)
#define AFX_SPACENOISEFILTERPARAMWND_H__4E41A642_9B07_41D3_B69E_BA15B229B121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpaceNoiseFilterParamWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "Draw3DWnd.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_28     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSpaceNoiseFilterParamWnd dialog
//-------------------------------------------------------------------------------------//
class CSpaceNoiseFilterParamWnd : public CBaseDialog
{
// Construction
public:
	CSpaceNoiseFilterParamWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpaceNoiseFilterParamWnd)
	enum { IDD = IDD_SPACE_NOISE_FILTER_PARAM_WND };	
	CComboBox	m_FirstFilterModeCombox;	
	CComboBox	m_OverLowModeCombox;
	CComboBox	m_HeightVarModeCombox;	
	CComboBox	m_FinalFilterModeCombox;
	CComboBox	m_FinalFilterModeCombox2;
	CComboBox	m_HeightCorrectModeCombox;
	CThisListCtrl_28	m_FilterListCtrl;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpaceNoiseFilterParamWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	void                       SetNoiseFilterParam(const TNoiseFilterParam &Param);
	void                       GetNoiseFilterParam(TNoiseFilterParam &Param);
	//---------------------------------------------------------------------------------//	
	void                       SetModelUniFrameList(unsigned int MapIndex, std::vector<TUNI_FRAME> &UniFrameList);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CDraw3DWnd                 m_Draw3DWnd;
	//---------------------------------------------------------------------------------//	
	unsigned int               m_MapIndex;
	std::vector<TUNI_FRAME>    m_UniFrameList;	
	//---------------------------------------------------------------------------------//	
	int                        m_DefaultIndex;
	TNoiseFilterParam          m_NoiseFilterParam;
	TNoiseFilterParam          m_NoiseFilterParamDefault;
	std::vector<TNoiseFilterParam> m_NoiseFilterParamList;	
	//---------------------------------------------------------------------------------//	
	bool                       m_StopFilterListBeSelected;
	bool                       BuildNoiseFilterList();
	bool                       BuildFilterListWndHeader();
	bool                       BuildFilterListWnd();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       UpdateParamToUI();
	bool                       UpdateUIToParam();
	void                       SendToSystemNoiseFilter(int index);
	void                       UpdateContentAwareUI();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpaceNoiseFilterParamWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual void OnOK();
	afx_msg void OnClickFilterListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedFilterListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSendToBtn01();
	afx_msg void OnSendToBtn02();
	afx_msg void OnSendToBtn03();
	afx_msg void OnSendToBtn04();
	afx_msg void OnSendToBtn05();
	afx_msg void OnSendToBtn06();
	afx_msg void OnSendToBtn07();
	afx_msg void OnSendToBtn08();
	afx_msg void OnSendToBtn09();
	afx_msg void OnSendToBtn10();
	afx_msg void OnSendToBtn11();
	afx_msg void OnSendToBtn12();
	afx_msg void OnUnlinkBtn();
	afx_msg void OnSaveParamBtn();
	afx_msg void OnLoadParamBtn();
	afx_msg void OnUpdateBtn();
	afx_msg void OnCbnSelchangeFirstFilterModeCombo();
	afx_msg void OnCbnSelchangeFinalFilterModeCombo();
	afx_msg void OnCbnSelchangeFinalFilterModeCombo2();
	afx_msg void OnBnClickedFirstFilterSearchonChk();
	afx_msg void OnBnClickedFinalFilterSearchonChk();
	afx_msg void OnBnClickedFinalFilterSearchonChk2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPACENOISEFILTERPARAMWND_H__4E41A642_9B07_41D3_B69E_BA15B229B121__INCLUDED_)
