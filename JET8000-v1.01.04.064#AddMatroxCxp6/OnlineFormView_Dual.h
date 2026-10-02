#if !defined(AFX_ONLINEFORMVIEW_DUAL_H__A7A243DC_23C9_4D6A_BF75_E8CF1167D997__INCLUDED_)
#define AFX_ONLINEFORMVIEW_DUAL_H__A7A243DC_23C9_4D6A_BF75_E8CF1167D997__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OnlineFormView_Dual.h : header file
//
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COnlineFormView_Dual form view
//-------------------------------------------------------------------------------------//
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "JETListCtrl.h"
#include "JetChart\\JETChartView.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_38     CJETListCtrl//目前使用的列表控制類別	
#define CThisChartCtrl    CJETChartView//目前使用的繪圖控制類別
//-------------------------------------------------------------------------------------//
class COnlineFormView_Dual : public CFormView
{
protected:
	COnlineFormView_Dual();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COnlineFormView_Dual)

// Form Data
public:
	//{{AFX_DATA(COnlineFormView_Dual)
	enum { IDD = IDD_ONLINE_FORMVIEW_DUAL };
	CComboBox	m_LaneIDCombox_LA;
	CComboBox	m_LaneIDCombox_LB;
	CComboBox	m_ProjectIDCombox_LA;
	CComboBox	m_ProjectIDCombox_LB;
	CStatic     m_LaneIcon_LA;
	CStatic     m_LaneIcon_LB;
	CStatic     m_ResultIcon_LA;
	CStatic     m_ResultIcon_LB;
	CStatic	m_NextStationSendIcon_LA;
	CStatic	m_NextStationSendIcon_LB;
	CStatic	m_NextStationSendOKIcon_LA;
	CStatic	m_NextStationSendOKIcon_LB;
	CStatic	m_NextStationSendNGIcon_LA;
	CStatic	m_NextStationSendNGIcon_LB;
	CStatic	m_NextStationRecieveIcon_LA;
	CStatic	m_NextStationRecieveIcon_LB;
	CStatic	m_LaneSensorPCBInIcon_LA;
	CStatic	m_LaneSensorPCBInIcon_LB;
	CStatic	m_LaneSensorSlowDownIcon_LA;
	CStatic	m_LaneSensorSlowDownIcon_LB;
	CStatic	m_LaneSensorPCBStopIcon_LA;
	CStatic	m_LaneSensorPCBStopIcon2_LA;
	CStatic	m_LaneSensorPCBStopIcon_LB;
	CStatic	m_LaneSensorPCBStopIcon2_LB;
	CStatic	m_LaneSensorPCBOutIcon_LA;	
	CStatic	m_LaneSensorPCBOutIcon_LB;
	CStatic	m_LastStationSendIcon_LA;
	CStatic	m_LastStationSendIcon_LB;
	CStatic	m_LastStationRecieveIcon_LA;
	CStatic	m_LastStationRecieveIcon_LB;
	CThisListCtrl_38	m_ResultListCtrl_LA;
	CThisListCtrl_38	m_ResultListCtrl_LB;
	CThisListCtrl_38	m_InfoListWnd_LA;
	CThisListCtrl_38	m_InfoListWnd_LB;
	CThisListCtrl_38	m_YieldingListWnd_LA;
	CThisListCtrl_38	m_YieldingListWnd_LB;
	CComboBox	m_YieldScopeCombox_LA;
	CComboBox	m_YieldScopeCombox_LB;
	CComboBox	m_Top10ScopeCombox_LA;
	CComboBox	m_Top10ScopeCombox_LB;
	CComboBox	m_DefectFromCombox_LA;
	CComboBox	m_DefectFromCombox_LB;
	CThisChartCtrl	m_ChartWnd_Defect_LA;
	CThisChartCtrl	m_ChartWnd_Defect_LB;
	CThisChartCtrl	m_ChartWnd_XYChart_LA;
	CThisChartCtrl	m_ChartWnd_XYChart_LB;
	CThisChartCtrl	m_ChartWnd_Top10_LA;	
	CThisChartCtrl	m_ChartWnd_Top10_LB;
	CThisChartCtrl	m_ChartWnd_Yielding_LA;
	CThisChartCtrl	m_ChartWnd_Yielding_LB;
	CTabCtrl	m_ChartTab_LA;
	CTabCtrl	m_ChartTab_LB;
	CComboBox	m_DefectModeCombox_LA;
	CComboBox	m_DefectModeCombox_LB;
	CThisListCtrl_38	m_DefectListCtrl_LA;
	CThisListCtrl_38	m_DefectListCtrl_LB;
	CThisListCtrl_38	m_ProjectInfoListCtrl_LA;
	CThisListCtrl_38	m_ProjectInfoListCtrl_LB;
	CImageWnd	m_ImageWnd_LA;
	CImageWnd	m_ImageWnd_LB;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnlineFormView_Dual)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL
	
protected:
	//---------------------------------------------------------------------------------//	
	CDib                       m_DibLane_LA;
	CDib                       m_DibLane_LB;
	CDib                       m_DibResult_LA;
	CDib                       m_DibResult_LB;
	//---------------------------------------------------------------------------------//	
	CBitmap                    m_LEDGreen;
	CBitmap                    m_LEDRed;
	CBitmap                    m_LEDGray;
	CBitmap                    m_LEDYellow;	
	//---------------------------------------------------------------------------------//	
	int                        m_LastStationSend_LA;
	int                        m_LastStationRecieve_LA;
	int                        m_NextStationSend_LA;
	int                        m_NextStationSendOK_LA;
	int                        m_NextStationSendNG_LA;
	int                        m_NextStationRecieve_LA;
	int                        m_LaneSensorPCBIn_LA;
	int                        m_LaneSensorPCBSlow_LA;
	int                        m_LaneSensorPCBStop_LA;
	int                        m_LaneSensorPCBStop2_LA;
	int                        m_LaneSensorPCBOut_LA;	
	int                        m_LastStationSend_LB;
	int                        m_LastStationRecieve_LB;
	int                        m_NextStationSend_LB;
	int                        m_NextStationSendOK_LB;
	int                        m_NextStationSendNG_LB;
	int                        m_NextStationRecieve_LB;
	int                        m_LaneSensorPCBIn_LB;
	int                        m_LaneSensorPCBSlow_LB;
	int                        m_LaneSensorPCBStop_LB;
	int                        m_LaneSensorPCBStop2_LB;
	int                        m_LaneSensorPCBOut_LB;	
	int                        m_MachineFrontCap;
	int                        m_MachineRearCap;
	int                        m_MachineAirLost;
	int                        m_MachineEMSOn;
	//---------------------------------------------------------------------------------//	
	LANE_ID                    m_LaneID;
	CAOIProject               *m_ProjectPtr_LA;
	CAOIProject               *m_ProjectPtr_LB;
	CString                    m_TestResultBarcode_LA;//檢測條碼
	CString                    m_TestResultBarcode_LB;//檢測條碼
	//---------------------------------------------------------------------------------//		
	bool                       AdjustCtrlWnd();//調整控制項檢測框位置
	bool                       AdjustCtrlWnd_LA(RECT &FullRect);//調整控制項檢測框位置
	bool                       AdjustCtrlWnd_LB(RECT &FullRect);//調整控制項檢測框位置
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       SwitchLane();
	//---------------------------------------------------------------------------------//	
	void                       CloseProject();	
	void                       CloseProject_LA();	
	void                       CloseProject_LB();	
	void                       SwitchProject();	
	void                       SwitchProject_LA();		
	void                       SwitchProject_LB();	
	void                       SwitchProject_LA(CAOIProject *ProjectPtr);	
	void                       SwitchProject_LB(CAOIProject *ProjectPtr);	
	void                       SwitchProjectMap();		
	CAOIProject*               GetActiveProject();
	CAOIProject*               GetActiveProject(LANE_ID LaneID);
	LANE_ID                    GetActiveLaneID_Dual();
	void                       SwitchProjectMark();	
	void                       ExecSwitchProject_LA(CAOIProject *ProjectPtr);
	void                       ExecSwitchProject_LB(CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//	
	bool                       BuildProjectIDCombox(LANE_ID LaneID, CComboBox &Combox);
	//---------------------------------------------------------------------------------//
	bool                       BuildDefectModeCombox(CComboBox &Combox);
	//---------------------------------------------------------------------------------//		
	void                       DrawIconWnd(CDib &dib, CStatic &IconWnd);
	void                       RedrawWnd(BOOL bRedrawBK);
	//---------------------------------------------------------------------------------//	
	bool                       ExecMoveToStage();	
	bool                       ExecUpdateProject();
	void                       ExecInspection_Finish();
	void                       ExecInspection_Finish_LA(CAOIProject *ProjectPtr);
	void                       ExecInspection_Finish_LB(CAOIProject *ProjectPtr);
	bool                       ExecOnlineInspection_Finish();
	//---------------------------------------------------------------------------------//	
	bool                       BuildDefectListWnd(LANE_ID LaneID, CAOIProject *ProjectPtr, CComboBox &DefectModeCombox);	
	bool                       BuildDefectListWndKernel_Current(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisListCtrl_38 &ListCtrl);	
	bool                       BuildDefectListWndKernel_Statistic(LANE_ID LaneID, CAOIProject *ProjectPtr, CComboBox &FromCombox, CThisListCtrl_38 &ListCtrl);	
	bool                       BuildDefectListWndHeader(CThisListCtrl_38 &ListCtrl);	
	bool                       SetDefectCountEdit(LANE_ID LaneID, int nDefects, size_t nTotal);
	//---------------------------------------------------------------------------------//
	bool                       BuildProjectInfoListWnd(CAOIProject *ProjectPtr, CThisListCtrl_38 &ListCtrl);
	bool                       BuildProjectInfoListWndHeader(CThisListCtrl_38 &ListCtrl);	
	//---------------------------------------------------------------------------------//
	bool                       BuildResultListWnd(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisListCtrl_38 &ListCtrl);	
	bool                       BuildResultListWndHeader(CThisListCtrl_38 &ListCtrl);
	//---------------------------------------------------------------------------------//	
	bool                       InitChartTab(CTabCtrl &TabCtrl);
	bool                       InitImageWnd(CImageWnd &ImageWnd);
	//---------------------------------------------------------------------------------//
	bool                       ClearChartWnd(CThisChartCtrl &ChartWnd);
	//---------------------------------------------------------------------------------//
	bool                       InitChartWnd(LANE_ID LaneID);//初始化圖表視窗
	bool                       InitChartWnd_Yielding(CThisChartCtrl &ChartWnd);//初始化圖表視窗-良率
	bool                       InitChartWnd_XYChart(CThisChartCtrl &ChartWnd);//初始化圖表視窗-座標圖
	bool                       InitChartWnd_Top10(CThisChartCtrl &ChartWnd);//初始化圖表視窗-Top-10
	bool                       InitChartWnd_DefectStatistic(CThisChartCtrl &ChartWnd);//初始化圖表視窗-瑕疵統計
	//---------------------------------------------------------------------------------//
	bool                       BuildChartWnd(LANE_ID LaneID, CAOIProject *ProjectPtr);//建立圖表視窗
	bool                       BuildChartWnd_Info(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisListCtrl_38 &ListCtrl);//建立圖表視窗-訊息
	bool                       BuildChartWnd_Yielding(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd, CThisListCtrl_38 &ListCtrl);//建立圖表視窗-良率
	bool                       BuildChartWnd_XYChart(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd);//建立圖表視窗-座標圖
	bool                       BuildChartWnd_Top10(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd);//建立圖表視窗-Top-10
	bool                       BuildChartWnd_DefectStatistic(LANE_ID LaneID, CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd, CComboBox &FromCombox);//建立圖表視窗-瑕疵統計
	//---------------------------------------------------------------------------------//
	bool                       ExecSelchangeChartTab(LANE_ID LaneID, CTabCtrl &TabCtrl);
	bool                       ResetMachineStates();//復歸機台狀態
	bool                       UpdateMachineStates(bool bReadPLC);//更新機台狀態
	//---------------------------------------------------------------------------------//
	bool                       LockUIWnd(bool bLock);//鎖住視窗
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
	bool                       ExecProjectClose(LANE_ID LaneID);//關閉專案
	//---------------------------------------------------------------------------------//
	bool                       ExecOnlineRun();
	bool                       ExecOnlineBypass();
	//---------------------------------------------------------------------------------//
// Implementation
protected:
	virtual ~COnlineFormView_Dual();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//---------------------------------------------------------------------------------//	
	// Generated message map functions
	//{{AFX_MSG(COnlineFormView_Dual)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnSelchangeDefectModeCombo_LA();
	afx_msg void OnSelchangeDefectModeCombo_LB();
	afx_msg void OnSelchangeChartTab_LA(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeChartTab_LB(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDefectFromCombo_LA();
	afx_msg void OnSelchangeDefectFromCombo_LB();
	afx_msg void OnSelchangeTop10ScopeCombo_LA();
	afx_msg void OnSelchangeTop10ScopeCombo_LB();
	afx_msg void OnClearStatisticBtn_LA();
	afx_msg void OnClearStatisticBtn_LB();
	afx_msg void OnSelchangeYieldingScopeCombo_LA();	
	afx_msg void OnSelchangeYieldingScopeCombo_LB();	
	afx_msg void OnSelchangeProjectIDCombo_LA();	
	afx_msg void OnSelchangeProjectIDCombo_LB();	
	afx_msg void OnProjectCloseBtnLA();
	afx_msg void OnProjectCloseBtnLB();
	afx_msg void OnBnClickedLaneRunBtnLA();
	afx_msg void OnBnClickedLaneRunBtnLB();
	afx_msg void OnBnClickedLaneStopBtnLA();
	afx_msg void OnBnClickedLaneStopBtnLB();
	afx_msg void OnBnClickedLaneBypassBtnLA();
	afx_msg void OnBnClickedLaneBypassBtnLB();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//---------------------------------------------------------------------------------//	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONLINEFORMVIEW_DUAL_H__A7A243DC_23C9_4D6A_BF75_E8CF1167D997__INCLUDED_)
