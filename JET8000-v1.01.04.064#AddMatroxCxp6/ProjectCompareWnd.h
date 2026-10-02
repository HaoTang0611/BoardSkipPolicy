#if !defined(AFX_PROJECTCOMPAREWND_H__9D33E8F1_3E4F_4B17_A17B_CB3B19491B44__INCLUDED_)
#define AFX_PROJECTCOMPAREWND_H__9D33E8F1_3E4F_4B17_A17B_CB3B19491B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectCompareWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
#include "ProjectMapWnd.h"
#define   CThisListCtrl_18     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
typedef struct tagComponentCmp
{
	CAOIProject   *ProjectPtr;
	CAOIPanel     *PanelPtr;
	CAOIBoard     *BoardPtr;
	CAOIComponent *ComponentPtr;
	CString        ModelName;
	CString        PartNumber;	
	int            CmpRest;
	int            CmpRest2;
	double         CadPosX;
	double         CadPosY;
	double         CadAngle;
	double         StagePosX;
	double         StagePosY;

	tagComponentCmp()
	{
		ProjectPtr = NULL;
		PanelPtr = NULL;
		BoardPtr = NULL;
		ComponentPtr = NULL;
		ModelName = _T("");
		PartNumber = _T("");		
		CmpRest = 0;
		CmpRest2= 0;
		CadPosX = 0;
		CadPosY = 0;
		CadAngle = 0;
		StagePosX = 0;
		StagePosY = 0;
	}
} TComponentCmp, *PComponentCmp;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectCompareWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectCompareWnd : public CBaseDialog
{
// Construction
public:
	CProjectCompareWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectCompareWnd)
	enum { IDD = IDD_PROJECT_COMPARE_WND };
	CComboBox     m_RefPanelCombox;
	CComboBox     m_RefBoardCombox;
	CComboBox     m_HostPanelCombox;
	CComboBox     m_HostBoardCombox;
	CThisListCtrl_18 m_RefComponentListWnd;
	CThisListCtrl_18 m_HostComponentListWnd;
	CThisListCtrl_18 m_CompareListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectCompareWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	void                       SetHostProjectPtr(CAOIProject *Ptr);	
	int                        CompareComponentItem(size_t index1, size_t index2);	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_RefProjectPtr;
	CAOIProject               *m_HostProjectPtr;
	std::vector<TComponentCmp> m_ComponentCmpList;//零件比較後的結果
	//---------------------------------------------------------------------------------//	
	CString                    m_strNone;
	CString                    m_strAdd;
	CString                    m_strDel;	
	CString                    m_strBypass;	
	CString                    m_strPos; 
	CString                    m_strAngle;	
	CString                    m_strPartNumber;	
	//---------------------------------------------------------------------------------//	
	bool                       m_Merged;
	int                        m_CompareColID;
	int                        m_CompareListSortMode;
	//-------------------------------------------------------------------------//
	CProjectMapWnd             m_ProjectMapWnd;
	//-------------------------------------------------------------------------//
	CAOIProject*               GetRefProjectPtr();
	CAOIProject*               GetHostProjectPtr();
	void                       SetRefProjectPtr(CAOIProject *Ptr);		
	bool                       CreateRefProjectObj();
	bool                       DestroyRefProjectObj();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	CString                    GetCompareResultText(int Res) const;
	//---------------------------------------------------------------------------------//	
	void                       SetMerged(bool val);
	bool                       GetMerged() const;
	//---------------------------------------------------------------------------------//
	void                       SetCompareListColID(int val);
	int                        GetCompareListColID() const;
	//---------------------------------------------------------------------------------//
	void                       SetCompareListSortMode(int val);
	int                        GetCompareListSortMode() const;		
	//---------------------------------------------------------------------------------//
	bool                       BuildHostPanelCombox();
	bool                       BuildHostBoardCombox();
	bool                       BuildHostComponentListWnd();
	bool                       BuildHostComponentListWndHeader();
	//---------------------------------------------------------------------------------//
	bool                       BuildRefPanelCombox();
	bool                       BuildRefBoardCombox();
	bool                       BuildRefComponentListWnd();
	bool                       BuildRefComponentListWndHeader();
	//---------------------------------------------------------------------------------//
	bool                       BuildCompareList();	
	bool                       BuildCompareListWnd();
	bool                       ClearCompareListWnd();
	bool                       BuildCompareListWndHeader();
	//---------------------------------------------------------------------------------//	
	bool                       CalcCadMap(CMapCoordinate &Map);
	bool                       ExecExportFile(LPCTSTR filename);
	bool                       ExecMoveToStagePos(double PosX, double PosY);
	//---------------------------------------------------------------------------------//	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectCompareWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual void OnOK();
	afx_msg void OnSelchangeHostPanelCombo();
	afx_msg void OnSelchangeHostBoardCombo();
	afx_msg void OnSelchangeRefPanelCombo();
	afx_msg void OnSelchangeRefBoardCombo();
	afx_msg void OnLoadRefCadxyBtn();
	afx_msg void OnCompareBtn();
	afx_msg void OnColumnclickCompareListWnd(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnRemoveBtn();
	afx_msg void OnRestoreBtn();
	afx_msg void OnMergeBtn();
	afx_msg void OnExportFileBtn();
	afx_msg void OnProjectMapBtn();
	afx_msg void OnDblclkCompareListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkHostComponentListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTCOMPAREWND_H__9D33E8F1_3E4F_4B17_A17B_CB3B19491B44__INCLUDED_)
