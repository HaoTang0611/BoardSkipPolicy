#if !defined(AFX_SPACEBASEPARAMWND_H__5E6BAE5B_0CBB_45DB_91EC_31B58ED0EB31__INCLUDED_)
#define AFX_SPACEBASEPARAMWND_H__5E6BAE5B_0CBB_45DB_91EC_31B58ED0EB31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpaceBaseParamWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_41     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSpaceBaseParamWnd dialog
//-------------------------------------------------------------------------------------//
class CSpaceBaseParamWnd : public CBaseDialog
{
// Construction
public:
	CSpaceBaseParamWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSpaceBaseParamWnd)
	enum { IDD = IDD_SPACE_BASE_PARAM_WND };
	CComboBox	m_BaseProcCombox;
	CComboBox	m_CalcBaseCombox;
	CComboBox	m_AutoRegionCombox;
	CComboBox	m_BodyOutsideCombox;
	CComboBox	m_TowardModeCombox;
	CComboBox	m_FilterModeCombox;
	CComboBox	m_FilterMode2DCombox;
	CThisListCtrl_41 m_BasePlaneListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpaceBaseParamWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	void                       SetProjectPtr(CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//	
	bool                       GetBaseColorEnabled();
	void                       SetBaseColorEnabled(bool val);
	//---------------------------------------------------------------------------------//	
	int                        GetBaseColorIndex();
	void                       SetBaseColorIndex(int val);
	//---------------------------------------------------------------------------------//	
	int                        GetLocalBasePlaneID();
	void                       SetLocalBasePlaneID(int val);
	//---------------------------------------------------------------------------------//	
	void                       SetBasePlaneParam(const TBasePlaneParam &Param);
	void                       GetBasePlaneParam(TBasePlaneParam &Param);
	//---------------------------------------------------------------------------------//		
	bool                       GetBasePlaneParamSetting();
	bool                       GetBasePlaneColorSetting();
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;		
	int                        m_BaseColorIndex;
	int                        m_LocalBasePlaneID;
	bool                       m_BaseColorEnabled;	
	bool                       m_BasePlaneParamSetting;
	bool                       m_BasePlaneColorSetting;
	//---------------------------------------------------------------------------------//	
	int                        m_DefaultIndex;
	TBasePlaneParam            m_BasePlaneParam;	
	TBasePlaneParam            m_BasePlaneParamDefault;
	std::vector<TBasePlaneParam> m_BasePlaneParamList;	
	//---------------------------------------------------------------------------------//	
	CAOIProject*               GetProjectPtr();
	//---------------------------------------------------------------------------------//	
	bool                       BuildFilterMode2DCombox(CComboBox &Combox);
	//---------------------------------------------------------------------------------//
	bool                       m_StopBasePlaneListBeSelected;
	bool                       BuildBasePlaneList();
	bool                       BuildBasePlaneListWndHeader();
	bool                       BuildBasePlaneListWnd();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       LoadProjectBaseColorName();
	//---------------------------------------------------------------------------------//	
	void                       UpdateUIEnable();
	void                       UpdateParamToUI();
	bool                       UpdateUIToParam();	
	void                       SendToSystemBasePlane(int index);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSpaceBaseParamWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	afx_msg void OnUseSideAllBtn();
	afx_msg void OnParamSettingChk();
	afx_msg void OnColorSettingChk();
	afx_msg void OnClickParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSendToBtn01();
	afx_msg void OnSendToBtn02();
	afx_msg void OnSendToBtn03();
	afx_msg void OnSendToBtn04();
	afx_msg void OnSendToBtn05();
	afx_msg void OnSendToBtn06();
	afx_msg void OnSendToBtn07();
	afx_msg void OnSendToBtn08();
	afx_msg void OnUnlinkBtn();
	afx_msg void OnSaveParamBtn();
	afx_msg void OnLoadParamBtn();
	afx_msg void OnSelchangeLevelCalcModeCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPACEBASEPARAMWND_H__5E6BAE5B_0CBB_45DB_91EC_31B58ED0EB31__INCLUDED_)
