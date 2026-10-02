#if !defined(AFX_ONLINEFORMVIEW_H__A1FF2D52_7B76_4D54_B091_C646ADC1BE87__INCLUDED_)
#define AFX_ONLINEFORMVIEW_H__A1FF2D52_7B76_4D54_B091_C646ADC1BE87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OnlineFormView.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// COnlineFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
#include "JetChart\\JETChartView.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_15     CJETListCtrl//目前使用的列表控制類別	
#define CThisChartCtrl    CJETChartView//目前使用的繪圖控制類別
//-------------------------------------------------------------------------------------//
class COnlineFormView : public CFormView
{
protected:
	COnlineFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COnlineFormView)

// Form Data
public:
	//{{AFX_DATA(COnlineFormView)
	enum { IDD = IDD_ONLINE_FORMVIEW };
	CComboBox	m_LaneIDCombox;
	CComboBox	m_ProjectIDCombox;
	CStatic         m_ResultIcon;
	CThisListCtrl_15	m_ResultListCtrl;
	CStatic	m_MachineFrontCapIcon;
	CStatic	m_MachineRearCapIcon;
	CStatic	m_MachineAirLostIcon;
	CStatic	m_MachineEMSOnIcon;
	CStatic	m_NextStationSendIcon;
	CStatic	m_NextStationSendIcon_LB;
	CStatic	m_NextStationSendOKIcon;
	CStatic	m_NextStationSendOKIcon_LB;
	CStatic	m_NextStationSendNGIcon;
	CStatic	m_NextStationSendNGIcon_LB;
	CStatic	m_NextStationRecieveIcon;
	CStatic	m_NextStationRecieveIcon_LB;
	CStatic	m_LaneSensorPCBInIcon;
	CStatic	m_LaneSensorPCBInIcon_LB;
	CStatic	m_LaneSensorSlowDownIcon;
	CStatic	m_LaneSensorSlowDownIcon_LB;
	CStatic	m_LaneSensorPCBStopIcon;
	CStatic	m_LaneSensorPCBStopIcon2;
	CStatic	m_LaneSensorPCBStopIcon_LB;
	CStatic	m_LaneSensorPCBStopIcon_LB2;
	CStatic	m_LaneSensorPCBOutIcon;	
	CStatic	m_LaneSensorPCBOutIcon_LB;
	CStatic	m_LastStationSendIcon;
	CStatic	m_LastStationSendIcon_LB;
	CStatic	m_LastStationRecieveIcon;
	CStatic	m_LastStationRecieveIcon_LB;
	CThisListCtrl_15	m_InfoListWnd;
	CThisListCtrl_15	m_YieldingListWnd;
	CComboBox	m_YieldScopeCombox;
	CComboBox	m_Top10ScopeCombox;
	CComboBox	m_DefectFromCombox;
	CComboBox	m_CpkTypeCombox;
	CThisChartCtrl	m_ChartWnd_Defect;
	CThisChartCtrl	m_ChartWnd_XYChart;
	CThisChartCtrl	m_ChartWnd_Top10;	
	CThisChartCtrl	m_ChartWnd_Yielding;
	CThisChartCtrl	m_ChartWnd_CPK01;
	CThisChartCtrl	m_ChartWnd_CPK02;
	CThisChartCtrl	m_ChartWnd_CPK03;
	CThisChartCtrl	m_ChartWnd_CPK04;
	CThisChartCtrl	m_ChartWnd_CPK05;
	CTabCtrl	m_ChartTab;
	CComboBox	m_DefectModeCombox;
	CThisListCtrl_15	m_DefectListCtrl;
	CThisListCtrl_15	m_ProjectInfoListCtrl;
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnlineFormView)
	public:
	virtual void OnInitialUpdate();
	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//			
	CDib                       m_DibResult;
	CBitmap                    m_LEDGreen;
	CBitmap                    m_LEDRed;
	CBitmap                    m_LEDGray;
	CBitmap                    m_LEDYellow;	
	//---------------------------------------------------------------------------------//	
	int                        m_LastStationSend;
	int                        m_LastStationRecieve;
	int                        m_NextStationSend;
	int                        m_NextStationSendOK;
	int                        m_NextStationSendNG;
	int                        m_NextStationRecieve;
	int                        m_LaneSensorPCBIn;
	int                        m_LaneSensorPCBSlow;
	int                        m_LaneSensorPCBStop;
	int                        m_LaneSensorPCBStop2;
	int                        m_LaneSensorPCBOut;	
	int                        m_LastStationSend_LB;
	int                        m_LastStationRecieve_LB;
	int                        m_NextStationSend_LB;
	int                        m_NextStationSendOK_LB;
	int                        m_NextStationSendNG_LB;
	int                        m_NextStationRecieve_LB;
	int                        m_LaneSensorPCBIn_LB;
	int                        m_LaneSensorPCBSlow_LB;
	int                        m_LaneSensorPCBStop_LB;
	int                        m_LaneSensorPCBStop_LB2;
	int                        m_LaneSensorPCBOut_LB;	
	int                        m_MachineFrontCap;
	int                        m_MachineRearCap;
	int                        m_MachineAirLost;
	int                        m_MachineEMSOn;
	//---------------------------------------------------------------------------------//	
	LANE_ID                    m_LaneID;
	CAOIProject               *m_ProjectPtr;
	CString                    m_TestResultBarcode;//檢測條碼	
	//---------------------------------------------------------------------------------//	
	bool                       AdjustCtrlWnd();//調整控制項檢測框位置
	bool                       GetShowCpkChartWnd() const;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       CloseProject();	
	void                       SwitchLane();	
	void                       SwitchProject();	
	void                       SwitchProjectMap();	
	CAOIProject*               GetActiveProject();
	LANE_ID                    GetActiveLaneID();
	void                       SwitchProjectMark();	
	void                       ExecSwitchProject(LANE_ID LaneID, CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//
	bool                       BuildProjectIDCombox(LANE_ID LaneID, CComboBox &Combox);
	//---------------------------------------------------------------------------------//
	bool                       BuildDefectModeCombox(CComboBox &Combox);
	//---------------------------------------------------------------------------------//
	void                       DrawResultWnd();
	void                       RedrawWnd(BOOL bRedrawBK);
	//---------------------------------------------------------------------------------//	
	bool                       ExecMoveToStage();	
	bool                       ExecUpdateProject();
	void                       ExecInspection_Finish();
	bool                       ExecOnlineInspection_Finish();
	//---------------------------------------------------------------------------------//	
	bool                       BuildDefectListWnd(CAOIProject *ProjectPtr);	
	bool                       BuildDefectListWndKernel_Current(CAOIProject *ProjectPtr);	
	bool                       BuildDefectListWndKernel_Statistic(CAOIProject *ProjectPtr);	
	bool                       BuildDefectListWndHeader(CThisListCtrl_15 &ListCtrl);	
	bool                       SetDefectCountEdit(int nDefects, size_t nTotal);
	//---------------------------------------------------------------------------------//
	bool                       BuildProjectInfoListWnd(CAOIProject *ProjectPtr, CThisListCtrl_15 &ListCtrl);
	bool                       BuildProjectInfoListWndHeader(CThisListCtrl_15 &ListCtrl);	
	//---------------------------------------------------------------------------------//
	bool                       BuildResultListWnd(CAOIProject *ProjectPtr, CThisListCtrl_15 &ListCtrl);
	bool                       BuildResultListWndHeader(CThisListCtrl_15 &ListCtrl);
	//---------------------------------------------------------------------------------//
	bool                       InitChartTab(CTabCtrl &TabCtrl);
	bool                       InitImageWnd(CImageWnd &ImageWnd);
	//---------------------------------------------------------------------------------//
	bool                       ClearChartWnd(CThisChartCtrl &ChartWnd);
	//---------------------------------------------------------------------------------//
	bool                       InitChartWnd();//初始化圖表視窗
	bool                       InitChartWnd_Yielding(CThisChartCtrl &ChartWnd);//初始化圖表視窗-良率
	bool                       InitChartWnd_XYChart(CThisChartCtrl &ChartWnd);//初始化圖表視窗-座標圖
	bool                       InitChartWnd_Top10(CThisChartCtrl &ChartWnd);//初始化圖表視窗-Top-10
	bool                       InitChartWnd_DefectStatistic(CThisChartCtrl &ChartWnd);//初始化圖表視窗-瑕疵統計
	bool                       InitChartWnd_CPK(CThisChartCtrl &ChartWnd);//初始化圖表視窗-CPK
	//---------------------------------------------------------------------------------//
	bool                       BuildChartWnd(CAOIProject *ProjectPtr);//建立圖表視窗
	bool                       BuildChartWnd_Info(CAOIProject *ProjectPtr, CThisListCtrl_15 &ListCtrl);//建立圖表視窗-訊息
	bool                       BuildChartWnd_Yielding(CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd, CThisListCtrl_15 &ListCtrl);//建立圖表視窗-良率
	bool                       BuildChartWnd_XYChart(CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd);//建立圖表視窗-座標圖
	bool                       BuildChartWnd_Top10(CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd);//建立圖表視窗-Top-10
	bool                       BuildChartWnd_DefectStatistic(CAOIProject *ProjectPtr, CThisChartCtrl &ChartWnd);//建立圖表視窗-瑕疵統計
	//---------------------------------------------------------------------------------//
	bool                       BuildChartWnd_CPK(CAOIProject *ProjectPtr);//建立圖表視窗-常態分布	
	bool                       BuildChartWnd_CPK(CAOIProject *ProjectPtr, int nChart, CThisChartCtrl &ChartWnd);//建立圖表視窗-常態分布		
	//---------------------------------------------------------------------------------//
	bool                       ExecSelchangeChartTab();
	bool                       ResetMachineStates();//復歸機台狀態
	bool                       UpdateMachineStates(bool bReadPLC);//更新機台狀態
	bool                       UpdateMultiLaneUI();//更新多軌道介面
	//---------------------------------------------------------------------------------//
	bool                       LockUIWnd(bool bLock);//鎖住視窗
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//		
// Implementation
protected:
	virtual ~COnlineFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(COnlineFormView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnSelchangeDefectModeCombo();
	afx_msg void OnSelchangeChartTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDefectFromCombo();
	afx_msg void OnSelchangeTop10ScopeCombo();
	afx_msg void OnClearStatisticBtn();
	afx_msg void OnSelchangeYieldingScopeCombo();
	afx_msg void OnSelchangeCpkTypeCombo();
	afx_msg void OnSelchangeProjectIDCombo();	
	afx_msg void OnMachineButtonStartBtn();
	afx_msg void OnMachineButtonResetBtn();
	afx_msg void OnMachineButtonStopBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONLINEFORMVIEW_H__A1FF2D52_7B76_4D54_B091_C646ADC1BE87__INCLUDED_)
