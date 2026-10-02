#if !defined(AFX_ALGHEIGHTRECOGNIZEWND_H__F22FC1B3_0CCA_4E23_803B_5E6B6456157F__INCLUDED_)
#define AFX_ALGHEIGHTRECOGNIZEWND_H__F22FC1B3_0CCA_4E23_803B_5E6B6456157F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgHeightRecognizeWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "ParamUni.h"
#include "JETListCtrl.h"
#include "JetAlg\JETAlg_Inc.h"
#include "SmartChart\SmartChart.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_01     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBarcodeRecognizeWnd dialog
//-------------------------------------------------------------------------------------//
class CStaticChartWnd : public CDialog
{
// Construction
public:
	CStaticChartWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgBarcodeRecognizeWnd)
	enum { IDD = IDD_SMART_CHART_WND };
	CComboBox	m_ComboxCtrl;
	CThisListCtrl_01	m_ParamListCtrl;
	CStatic	m_ImageWnd;	
	CButton	m_BtnCtrl;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgBarcodeRecognizeWnd)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetProject(CAOIProject* model);
	void                       SetComponent(vector<CAOIComponent*> component);
	bool					   SetWndPtr(vector<CAOIWnd*> WndPtr, CAOIWnd* aWndPtr);
	void                       SetType(int type) { smartChartType = type; }
	//---------------------------------------------------------------------------------//
	bool                       isSuccess();
protected:
	CSmartChart_Agent          *obj;
	HWND                       s;
	int                        smartChartType;	//畫圖類型0:結果point  1:結果數值統計 2:讀檔模式
	int                        algType;			//0:亮度比例 1:群組高度 2:群組角度 200:高度比例
	//---------------------------------------------------------------------------------//	
	CString                    m_WndTitle;
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ImageIndex;
	//---------------------------------------------------------------------------------//	
	CAOIProject*               m_ProjectPtr;
	vector<CAOIComponent*>     m_Component;
	vector<CAOIWnd*>		   m_WndPtr;
	CAOIWnd*		           m_ActiveWndPtr;
	//---------------------------------------------------------------------------------//
	std::wstring               m_titleName;
	std::wstring               m_labelame;
	std::vector<double>        m_readingData; //全部的數據
	std::vector<double>        m_readingDataGO;// GO的數據
	std::vector<double>        m_readingDataNG;// NG
	double                     m_readingDataAVG;// 平均數據
	std::vector<wstring>       m_componenetName;
	std::vector<double>        m_Index;
	std::vector<double>        m_XIndexOK;
	std::vector<double>        m_XIndexNG;
	std::vector<double>        m_XBase;//畫0
	double                     m_USL;
	double                     m_LSL;
	double                     m_Sec;
	double                     *m_USLPtr;
	double                     *m_LSLPtr;
	double                     m_newUSL;
	double                     m_newLSL;
	double                     m_Max;
	double                     m_Min;
	int                        m_roundLength;
	//---------------------------------------------------------------------------------//
	//CParamUni                 *m_ParamActPtr;
	//CParamList                 m_ParamList;
	bool                       m_StopParamListBeSelected;
	int                        m_ParamListIndex = -1;
	//---------------------------------------------------------------------------------//
	TALG_PARAM_GROUP_COMPARE *gcActiveParam;
	TALG_PARAM_BRIGHT_RATIO *brActiveParam;
	TALG_PARAM_OUTER_SHORT *osActiveParam;
	TALG_PARAM_OBJECT_MEASURE *omActiveParam;
	TALG_PARAM_IPC_PRODUCT *ipcActiveParam;
	TALG_PARAM_RESIN_HEIGHT *hightDetectActiveParm;
	TALG_PARAM_WIRE_WIDTH  *wireWidthActiveParm;
	//---------------------------------------------------------------------------------//
	//COLORREF                   m_BkColor;
	//CJetMemDC                  m_ImageMemDC;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default, int viewID);
	//---------------------------------------------------------------------------------//		
	//IMAGE_PTR                  GetImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount);	
	//---------------------------------------------------------------------------------//	
	//IMAGE_PTR                  GetShowBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount);	
	//---------------------------------------------------------------------------------//
	//CParamUni*                 GetActParamUni();
	//void                       SetActParamUni(CParamUni *Ptr);
	//---------------------------------------------------------------------------------//
	bool                       BuildCombox();
	bool                       BuildParamListWnd();
	bool                       BuildParamListWndHeader();	
	//---------------------------------------------------------------------------------//	
	void                       ClearChart();	
	void                       DrawSmartChart();
	//---------------------------------------------------------------------------------//
	bool                       ExecDataStatic();
	void                       UpdateWndParm();
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_01 &ListCtrl, int nItem);
	//bool                       ExecDblclkParamListWnd(CThisListCtrl_01 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//
	void                       clear(LPCTSTR Folder);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgBarcodeRecognizeWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
//	afx_msg void OnLoadImageBtn();
	afx_msg void OnClearBtn();
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkParamListWnd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeParamCombo();
//	afx_msg void OnKillfocusParamCombo();
//	afx_msg void OnSaveImageBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBnClickedOk();
//	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnStnDblclickImageWnd();

};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGHEIGHTRECOGNIZEWND_H__F22FC1B3_0CCA_4E23_803B_5E6B6456157F__INCLUDED_)
