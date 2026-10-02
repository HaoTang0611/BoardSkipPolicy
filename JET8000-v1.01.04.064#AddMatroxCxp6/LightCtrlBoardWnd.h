#if !defined(AFX_LIGHTCTRLBOARDWND_H__5AB92F9C_0B58_4CC4_8BEB_9E5E5AF95158__INCLUDED_)
#define AFX_LIGHTCTRLBOARDWND_H__5AB92F9C_0B58_4CC4_8BEB_9E5E5AF95158__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightCtrlBoardWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "LightCtrlBoard.h"
//-------------------------------------------------------------------------------------//
const int LCB_TIMER_LOOP_BACK      = 100;
const int LCB_TIMER_REPEAT_TRIGGER = 200;
const int LCB_TIMER_REPEAT_WRITE_READ = 300;
/////////////////////////////////////////////////////////////////////////////
// CLightCtrlBoardWnd dialog
//-------------------------------------------------------------------------------------//
class CLightCtrlBoardWnd : public CDialog
{
// Construction
public:
	CLightCtrlBoardWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLightCtrlBoardWnd)
	enum { IDD = IDD_LIGHT_CTRL_BOARD_WND };
	CListCtrl	m_TableListWnd;
	CComboBox	m_3DCastIDComboxR;
	CComboBox	m_3DCastIDComboxW;
	CComboBox	m_TableTypeComboxR;
	CComboBox	m_TableTypeComboxW;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightCtrlBoardWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:	
	//---------------------------------------------------------------------------------//		
	std::vector<TLCB_TRIG_TABLE> m_TableList;
	bool                         m_StopListItemChanged;
	int                          m_TableFirstIndex;
	unsigned int                 m_WriteReadTableCount;
	unsigned int                 m_TriggerTestExecCount;
	DWORD                        m_TimerDelayTime;//計時器的延遲時間
	bool                         m_UpdateTableInfoEdit;//更新表格訊息
	//---------------------------------------------------------------------------------//	
	UINT                         m_TotalCnt_PCtoFPGA;
	UINT                         m_TotalCnt_FPGAtoCamera1;
	UINT                         m_TotalCnt_FPGAtoCamera2;
	UINT                         m_TotalCnt_FPGAtoCamera3;
	UINT                         m_TotalCnt_FPGAtoCamera4;
	UINT                         m_TotalCnt_FPGAtoCamera5;
	UINT                         m_TotalCnt_FPGAtoCamera6;
	UINT                         m_TotalCnt_FPGAtoCamera7;
	UINT                         m_TotalCnt_FPGAtoCamera8;
	UINT                         m_TotalCnt_CameraReceieve;
	UINT                         m_TotalCnt_FPGAtoDLP1;
	UINT                         m_TotalCnt_FPGAtoDLP2;
	UINT                         m_TotalCnt_FPGAtoDLP3;
	UINT                         m_TotalCnt_FPGAtoDLP4;
	UINT                         m_TotalCnt_FPGAtoDLP5;
	UINT                         m_TotalCnt_FPGAtoDLP6;
	UINT                         m_TotalCnt_FPGAtoDLP7;
	UINT                         m_TotalCnt_FPGAtoDLP8;
	UINT                         m_TotalCnt_DLP1toFPGA;
	UINT                         m_TotalCnt_DLP2toFPGA;
	UINT                         m_TotalCnt_DLP3toFPGA;
	UINT                         m_TotalCnt_DLP4toFPGA;
	UINT                         m_TotalCnt_DLP5toFPGA;
	UINT                         m_TotalCnt_DLP6toFPGA;
	UINT                         m_TotalCnt_DLP7toFPGA;
	UINT                         m_TotalCnt_DLP8toFPGA;	
	//---------------------------------------------------------------------------------//	
	void                       InitialParam();
	BOOL                       CheckContrlBoard();
	LIGHT_CTRL_BOARD_IMP_CLS   GetLightCtrlBoardClass();	
	void                       LockUIWnd(bool bLock);
	void                       SwitchMultiLanguage();	
	//---------------------------------------------------------------------------------//
	int                        m_LoopBackIndex;
	bool                       ExecLoopBackTimer(int LoopIndex);
	//---------------------------------------------------------------------------------//	
	bool                       ExecFireTriggerBtn();
	bool                       ExecFireTriggerNext();
	bool                       EnableDLPMask();//確認DLP使用遮罩
	bool                       ClearTotalCount();
	//---------------------------------------------------------------------------------//	
	bool                       UpdateFPGACtrlModeChk();	
	bool                       UpdateTableToReadUI_Camera(UINT val);
	bool                       UpdateTableToWriteUI_Camera(UINT val);
	bool                       UpdateTableFromWriteUI_Camera(UINT &val);
	bool                       UpdateTableToReadUI(TLCB_TRIG_TABLE &table);
	bool                       UpdateTableToWriteUI(TLCB_TRIG_TABLE &table);	
	bool                       UpdateTableListWnd(std::vector<TLCB_TRIG_TABLE> &TableList);		
	//---------------------------------------------------------------------------------//
	bool                       SwitchToBasicUI();//基本UI
	bool                       SwitchToAdvancedUI();//進階UI	
	//---------------------------------------------------------------------------------//
	bool                       ExecCameraGrab();//執行相機取像
	bool                       ClearCameraCount(bool ClearTotalCnt);//清除相機取相數量	
	bool                       UpdateCameraCount();//更新相機取相數量
	//---------------------------------------------------------------------------------//
	bool                       ExecReadCountBtn();
	bool                       ExecReadStatusBtn();
	bool                       ExecReadStatusBtn_Fpga();
	bool                       ExecReadStatusBtn_Arduino();
	bool                       CalcTableUsedCount(std::vector<TLCB_TRIG_TABLE> &TableList);
	//---------------------------------------------------------------------------------//
	bool                       CalcTriggerTableEllapsedTime();//計算觸發表格的經過時間
	//---------------------------------------------------------------------------------//	
	bool                       ExecWriteReadTableBtn();
	bool                       SetTableInfoEdit(LPCTSTR Info);
	//---------------------------------------------------------------------------------//
	bool                       ExecTableFirstIdSetBtn();	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLightCtrlBoardWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBasicWriteBtn();
	afx_msg void OnBasicReadBtn();
	afx_msg void OnConnectChk();
	afx_msg void OnFPGAModeChk();
	afx_msg void OnAssignModeChk();
	afx_msg void OnWriteModeChk();
	afx_msg void OnReadModeChk();	
	afx_msg void OnLoopBackChk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLoopBackResetBtn();
	afx_msg void OnClearCountBtn();
	afx_msg void OnClearAllBtn();
	afx_msg void OnReadCountBtn();
	afx_msg void OnTableWriteBtn();
	afx_msg void OnTableReadBtn();	
	afx_msg void OnFireTriggerBtn();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnReadAllTableBtn();
	afx_msg void OnClearAllTableBtn();
	afx_msg void OnReadStatusBtn();
	afx_msg void OnDblclkTableListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTableListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTWDefaultBtn();
	afx_msg void OnAdvancedChk();
	afx_msg void OnBuildInspectionTableBtn();
	afx_msg void OnItemchangedTableListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWriteReadTableTestBtn();
	afx_msg void OnTableFirstIdSetBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CLightCtrlBoardWnd LightCtrlBoardWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTCTRLBOARDWND_H__5AB92F9C_0B58_4CC4_8BEB_9E5E5AF95158__INCLUDED_)
