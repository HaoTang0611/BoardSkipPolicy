#if !defined(AFX_EDITMODELLISTWND_H__AEFC8614_3127_4572_9663_3926A500E68D__INCLUDED_)
#define AFX_EDITMODELLISTWND_H__AEFC8614_3127_4572_9663_3926A500E68D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLibraryWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditLibraryWnd dialog
//-------------------------------------------------------------------------------------//
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "resource.h"
//-------------------------------------------------------------------------------------//
class CEditLibraryWnd : public CDialog
{
public:
	CEditLibraryWnd(CWnd* pParent = NULL);           // standard constructor
	//DECLARE_DYNCREATE(CEditLibraryWnd)

// Dialog Data
public:
	//{{AFX_DATA(CEditLibraryWnd)
	enum { IDD = IDD_EDIT_LIBRARY_WND };
	CListCtrl	m_ModelIconListWnd;
	CListCtrl	m_ModelGroupListWnd;
	CListCtrl	m_ModelTypeListWnd;		
	CComboBox   m_ModelFrameIndexCombox;
	CComboBox   m_ModelIconSizeCombox;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditLibraryWnd)
	public:
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	bool                       ExecToActiveComponent();
	bool                       ExecShowLibraryWnd();//顯示資料庫視窗
	bool                       ExecHideLibraryWnd();//隱藏資料庫視窗
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	int                        m_FitModelLandCount;//匹配模組特徵框數
	double                     m_FitModelSizeW;//匹配模組的尺寸
	double                     m_FitModelSizeH;//匹配模組的尺寸	
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_ProjectPtr;
	CImageList                 m_ModelTypeImageList;
	CImageList                 m_ModelIconImageList;
	bool                       m_StopModelNameListBeSelected;
	bool                       m_StopModelTypeListBeSelected;
	bool                       m_StopModelGroupListBeSelected;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       CloseProject();	
	CAOIProject*               GetActiveProject();	
	void                       UpdateActiveProject();
	void                       SetActiveProject(CAOIProject* Ptr);
	//---------------------------------------------------------------------------------//
	bool                       CheckUsePartNumberName();//確認是否使用料號名稱
	//---------------------------------------------------------------------------------//
	void                       BuildModelIconSizeCombox();
	//---------------------------------------------------------------------------------//
	UINT                       GetModelTypeIcon(MODEL_TYPE ModelType, bool Small);	
	bool                       ClearModelTypeListCtrl(CListCtrl &ListCtrl);
	bool                       BuildModelTypeListCtrl(CListCtrl &ListCtrl);	
	bool                       SetModelTypeListCtrlItemSlected(MODEL_TYPE ModelType);
	//---------------------------------------------------------------------------------//
	bool                       InitModelGroupListCtrl(CListCtrl &ListCtrl);
	bool                       ClearModelGroupListCtrl(CListCtrl &ListCtrl);
	bool                       BuildModelGroupListCtrl(CListCtrl &ListCtrl, MODEL_TYPE ModelType, bool bBuildIconList);		
	bool                       SetModelGroupListCtrlItemSlected(LPCTSTR  GroupName);
	//---------------------------------------------------------------------------------//
	bool                       AddModelIconListItem(CAOIModel *ModelPtr);
	bool                       ClearModelIconListCtrl(CListCtrl &ListCtrl);
	bool                       UpdateModelIconListCtrl(CListCtrl &ListCtrl);
	bool                       BuildModelIconListCtrl(MODEL_TYPE ModelType, LPCTSTR  GroupName, CListCtrl &ListCtrl);		
	bool                       SetModelIconListCtrlItemSlected(LPCTSTR  ModelName);	
	//---------------------------------------------------------------------------------//	
	bool                       ExecModelGroupAdd(bool bPartNumberName);//新增加模組群組
	bool                       ExecModelGroupRename();//改名模組群組
	//---------------------------------------------------------------------------------//
	bool                       ExecModelMenu();
	bool                       ExecModelAdd(bool bPartNumberName);//新增加模組
	bool                       ExecModelClone(bool bPartNumberName);//複製模組
	bool                       ExecModelRename();//改名模組
	bool                       ExecModelDelete();//刪除模組
	bool                       ExecModelApply();//套用模組至零件上
	bool                       ExecModelRotate(double Angle);//旋轉模組至零件上
	bool                       ExecModelBKImageIndex();//變更模組底圖影像編號
	bool                       ExecModelImport();//模組匯入
	bool                       ExecModelModifyGroupName();//變更模組群組名稱
	bool                       ExecModelChangeModelType();//變更模組樣式	
	//---------------------------------------------------------------------------------//
	bool                       ExecModelSaveToServer();//儲存模組至伺服器
	bool                       ExecModelLoadFromServer();//從伺服器載入模組
	//---------------------------------------------------------------------------------//
// Implementation
protected:	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditLibraryWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();	
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickModelTypeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedModelTypeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModelGroupAddBtn();
	afx_msg void OnModelGroupRenameBtn();	
	afx_msg void OnClickModelGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedModelGroupListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModelAddBtn();
	afx_msg void OnModelDeleteBtn();
	afx_msg void OnClickModelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDbclickModelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedModelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnModelApplyBtn();
	afx_msg void OnModelRotateBtn();
	afx_msg void OnModelImportBtn();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSearchModelBtn();
	afx_msg void OnModelCloneBtn();
	afx_msg void OnDblclkModelTypeListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnArrangeLibraryBtn();	
	afx_msg void OnMenuLibraryModelRename();
	afx_msg void OnMenuLibraryModelApply();
	afx_msg void OnMenuLibraryModelClone();
	afx_msg void OnMenuLibraryModelRotate090();
	afx_msg void OnMenuLibraryModelRotate180();
	afx_msg void OnMenuLibraryModelRotate270();
	afx_msg void OnMenuLibraryModelDelete();
	afx_msg void OnMenuLibraryModelGroupName();
	afx_msg void OnMenuLibraryModelChangeType();	
	afx_msg void OnMenuLibraryModelBKImageIndex();	
	afx_msg void OnMenuLibraryModelSaveServer();	
	afx_msg void OnMenuLibraryModelLoadServer();	
	afx_msg void OnSelchangeModelBKImageIndexCombox();
	afx_msg void OnSelchangeModelIconSizeCombox();	
	afx_msg void OnModelGroupNameBtn();			
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMODELLISTWND_H__AEFC8614_3127_4572_9663_3926A500E68D__INCLUDED_)
