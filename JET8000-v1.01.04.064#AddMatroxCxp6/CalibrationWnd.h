#if !defined(AFX_CALIBRATIONWND_H__A66D3BF9_28C3_4CD3_8AAC_075631485AA1__INCLUDED_)
#define AFX_CALIBRATIONWND_H__A66D3BF9_28C3_4CD3_8AAC_075631485AA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalibrationWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "CaliPaneAlign.h"
#include "CaliPaneStage.h"
#include "CaliPaneDynamic.h"
#include "CaliPaneTargetSetting.h"
/////////////////////////////////////////////////////////////////////////////
// CCalibrationWnd dialog
//-------------------------------------------------------------------------------------//
class CCalibrationWnd : public CBaseDialog
{
// Construction
public:
	CCalibrationWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalibrationWnd)
	enum { IDD = IDD_CALIBRATION_WND };
	CTabCtrl	m_PaneTabWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibrationWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:	
	//---------------------------------------------------------------------------------//
	CString                    m_WndText;
	bool                       m_MotionJobMode;
	//---------------------------------------------------------------------------------//	
	MASK_PTR                   m_MaskBuffer;//標誌記憶體指標
	MASK_PTR                   m_MaskBuffer1;//標誌記憶體指標
	MASK_PTR                   m_MaskBuffer2;//標誌記憶體指標
	//---------------------------------------------------------------------------------//	
	PHASE_PTR                  m_PhaseBuffer;//相位記憶體指標
	PHASE_PTR                  m_PhaseBuffer1;//相位記憶體指標
	PHASE_PTR                  m_PhaseBuffer2;//相位記憶體指標
	IMAGE_PTR                  m_ImageBuffer;//影像記憶體指標
	IMAGE_PTR                  m_ImageBuffer1;//影像記憶體指標-1
	IMAGE_PTR                  m_ImageBuffer2;//影像記憶體指標-2
	size_t                     m_ImageBufferSize;//影像記憶體尺寸	
	bool                       CreateImageBuffer();//建立影像資料
	bool                       DestroyImageBuffer();//摧毀影像資料
	//---------------------------------------------------------------------------------//	
	IMAGE_PTR                  m_ShowBuffer;//顯示記憶體指標
	IMAGE_PTR                  m_ShowBuffer1;//顯示記憶體指標-1
	size_t                     m_ShowBufferSize;//顯記憶體尺寸	
	bool                       CreateShowBuffer();//建立顯示資料
	bool                       DestroyShowBuffer();//摧毀顯示資料
	//---------------------------------------------------------------------------------//	
	SPACE_PTR                  m_SpaceBuffer;//空間記憶體指標	
	SPACE_PTR                  m_SpaceBuffer1;//空間記憶體指標	
	SPACE_PTR                  m_SpaceBuffer2;//空間記憶體指標	
	size_t                     m_SpaceBufferSize;//空間記憶體尺寸	
	bool                       CreateSpaceBuffer();//建立空間資料
	bool                       DestroySpaceBuffer();//摧毀空間資料
	//---------------------------------------------------------------------------------//	
	CCaliPaneAlign             m_CaliPaneAlign;
	CCaliPaneStage             m_CaliPaneStage;
	CCaliPaneDynamic           m_CaliPaneDynamic;
	CCaliPaneTargetSetting     m_CaliPaneTargetPos;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       AdjustPaneWndPosition();
	void                       DoSelchangePaneTabWnd();
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalibrationWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTargetSettingBtn();
	afx_msg void OnSaveCalibrationBtn();
	virtual void OnOK();
	afx_msg void OnSelchangePaneTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATIONWND_H__A66D3BF9_28C3_4CD3_8AAC_075631485AA1__INCLUDED_)
