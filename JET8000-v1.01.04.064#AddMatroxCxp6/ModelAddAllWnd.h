#if !defined(AFX_MODELADDALLWND_H__F5937486_3CE2_4208_B5EC_8E7D109EE5B6__INCLUDED_)
#define AFX_MODELADDALLWND_H__F5937486_3CE2_4208_B5EC_8E7D109EE5B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelAddAllWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "AOIModel.h"
#include "ImageWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelAddAllWnd dialog
//-------------------------------------------------------------------------------------//
class CModelAddAllWnd : public CBaseDialog
{
// Construction
public:
	CModelAddAllWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelAddAllWnd)
	enum { IDD = IDD_MODEL_ADD_ALL_WND };
	CImageWnd m_ImageWnd;
	CComboBox m_ChipSizeModeCombox;
	CComboBox m_PartAlignModeCombox;
	CComboBox m_BodyMissingModeCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelAddAllWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetModelImageFilename(LPCTSTR name);
	//---------------------------------------------------------------------------------//
	void                       GetAddAllParam(TMODEL_DEFAULT_WND_PARAM &Param);
	void                       SetAddAllParam(const TMODEL_DEFAULT_WND_PARAM &Param, const CWndDefectItem &ExistDefectItems, const CWndDefectItem & BasicDefectItems, int nLevel);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CString                    m_ModelImageFilename;
	CWndDefectItem             m_ModelDefectItems;
	CWndDefectItem             m_ModelExistDefectItems;
	CWndDefectItem             m_ModelBasicDefectItems;
	CWndDefectItem             m_ModelDefaultModel;//預設模組的檢測框
	TMODEL_DEFAULT_WND_PARAM   m_ModelAddWndParam;
	TMODEL_DEFAULT_WND_PARAM   m_ModelDefaultParam;	
	std::map<UINT, WND_DEFECT_ID> m_CtrlIdMapDefectId;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//
	void                       UIToParam();
	void                       ParamToUI(bool Filter);
	void                       UpdateUse3DLight();
	void                       UpdateUseDefaultModel();	
	//---------------------------------------------------------------------------------//
	void                       BuildModleImageWnd();
	void                       BuildCtrlIdMapWndDefectId(std::map<UINT, WND_DEFECT_ID> &Map);
	//---------------------------------------------------------------------------------//
	void                       BuildChipSizeMode(CComboBox &Combox);
	void                       BuildPartAlignMode(CComboBox &Combox);
	void                       BuildBodyMissingMode(CComboBox &Combox);
	//---------------------------------------------------------------------------------//
	void                       EnableCheckWnd(UINT CtrlID, int nWnds, bool SetChk);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelAddAllWnd)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDisableAllBtn();
	afx_msg void OnDefaultParamBtn();
	afx_msg void OnDefaultParamSaveBtn();
	afx_msg void OnLevelBasicBtn();
	afx_msg void OnLevelAdvanceBtn();
	afx_msg void OnSelchangeChipSizeCombo();
	afx_msg void OnUseDefaultModelChk();
	afx_msg void OnShowAllGroupBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELADDALLWND_H__F5937486_3CE2_4208_B5EC_8E7D109EE5B6__INCLUDED_)
