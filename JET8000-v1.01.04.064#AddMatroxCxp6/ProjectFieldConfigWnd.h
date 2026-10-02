#if !defined(AFX_PROJECTFIELDCONFIGWND_H__E58E6895_669F_419C_942A_FBF63FCFAB98__INCLUDED_)
#define AFX_PROJECTFIELDCONFIGWND_H__E58E6895_669F_419C_942A_FBF63FCFAB98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectFieldConfigWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_19     CListCtrl//ヘ玡ㄏノ北摸	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectFieldConfigWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectFieldConfigWnd : public CBaseDialog
{
// Construction
public:
	CProjectFieldConfigWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectFieldConfigWnd)
	enum { IDD = IDD_PROJECT_FIELD_CONFIG_WND };
	CStatic	m_ImageWnd;
	CThisListCtrl_19 m_FieldListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectFieldConfigWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetProjectPtr(CAOIProject *Ptr);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;
	CAOIProject*               GetProjectPtr();
	//---------------------------------------------------------------------------------//		
	UINT                       m_AreaID;
	UINT                       m_BuildID;
	UINT                       m_DivisionID;
	UINT                       m_FieldGrabTime;
	FIELD_BUILD_MODE           m_FieldBuildMode;//跋办ミ(皌竚)家Α	
	FIELD_BUILD_MODE           m_InspectionFieldBuildMode;//浪代ミ(皌竚)家Α
	FIELD_BUILD_AREA_MODE      m_FieldBuildAreaMode;//跋办ミ(皌竚)縩家Α
	FIELD_BUILD_AREA_MODE      m_InspectionFieldBuildAreaMode;//浪代ミ(皌竚)縩家Α
	TProjectParameter          m_ProjectParam;	
	std::vector<CAOIField*>    m_FieldList;	
	std::vector<CAOIField*>    m_ProjectFieldList;	
	//---------------------------------------------------------------------------------//		
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;
	COLORREF                   m_ImageWndBkColor;
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_ImageBit;	
	TPOINT2D                   m_Resolution;	
	TREGION4D                  m_ImageStageRgnDA;
	TREGION4D                  m_ImageStageRgnDB;
	DISTRICT_ID                m_DistrictID;
	//---------------------------------------------------------------------------------//
	POINT                      m_LBtnUpPt;
	POINT                      m_LBtnDownPt;
	POINT                      m_LastMovePt;	
	//---------------------------------------------------------------------------------//
	double                     m_ImageZoom;
	TPOINT2D                   m_ImageOffset;
	//---------------------------------------------------------------------------------//		
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       ClearFieldList(std::vector<CAOIField*> &FieldList);	
	void                       CloneFieldList(std::vector<CAOIField*> &SrcFieldList, std::vector<CAOIField*> &DstFieldList);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	void                       CreateBKImage(bool bResetView);
	void                       DrawProjectMap(HDC hDC, bool bResetView);	
	void                       DrawFieldList(HDC hDC);
	void                       DrawCameraPos(HDC hDC);
	void                       DrawComponentList(HDC hDC);
	//---------------------------------------------------------------------------------//
	void                       SetFieldInfo(LPCTSTR str);
	void                       EnableFieldAreaWnd(bool bEnable);
	void                       EnableFieldDivisionWnd(bool bEnable);
	bool                       ChangeProjectFieldType(UINT AreaID, UINT BuildID, UINT DivisionID);
	bool                       BuildRunPath(FIELD_PATH_MODE FieldPathMode, TPOINT2D &StartPos, double Factor, std::vector<CAOIField*> &FieldList, std::vector<TPOINT2D> &RunPathList);
	//---------------------------------------------------------------------------------//
	bool                       m_StopFieldListBeSelected;
	void                       BuildFieldListWndHeader();
	void                       BuildFieldListWnd();
	//---------------------------------------------------------------------------------//
	DISTRICT_ID                GetDistrictID();
	void                       SetDistrictID(DISTRICT_ID val);	
	//---------------------------------------------------------------------------------//
	bool                       SelectField(size_t idx);
	bool                       SeparateFieldList(const std::vector<CAOIField*> &List, std::vector<CAOIField*> &ListA, std::vector<CAOIField*> &ListB);
	bool                       CombinePositionList(const std::vector<TPOINT2D> &ListA, const std::vector<TPOINT2D> &ListB, std::vector<TPOINT2D> &List);
	//---------------------------------------------------------------------------------//
	void                       UpdateUIToParam();
	//---------------------------------------------------------------------------------//
	bool                       MoveListItem(int nItem, UINT Mode);
	//---------------------------------------------------------------------------------//
	bool                       ExecCheckBoardOneFovBtn();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectFieldConfigWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAreaByBoardChk();
	afx_msg void OnAreaByComponentChk();
	afx_msg void OnCheckBoardOneFovBtn();
	afx_msg void OnFieldByProjectChk();
	afx_msg void OnFieldByPanelChk();
	afx_msg void OnFieldByBoardChk();
	afx_msg void OnDivisionMassAreaChk();
	afx_msg void OnDivisionDiaLineChk();
	afx_msg void OnDivisionHorLineChk();
	afx_msg void OnDivisionVerLineChk();
	afx_msg void OnRunPathBtn();
	afx_msg void OnItemchangedFieldListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowFieldChk();
	afx_msg void OnShowComponentChk();
	afx_msg void OnFieldSortUpBtn();
	afx_msg void OnFieldSortDownBtn();
	afx_msg void OnFieldSortTopBtn();
	afx_msg void OnFieldSortBotBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTFIELDCONFIGWND_H__E58E6895_669F_419C_942A_FBF63FCFAB98__INCLUDED_)
