#if !defined(AFX_DRAW3DWND_H__42893C97_828F_4B4D_9BB5_524362F33479__INCLUDED_)
#define AFX_DRAW3DWND_H__42893C97_828F_4B4D_9BB5_524362F33479__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Draw3DWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "OpenGLWnd.h"
#include "JetChart\\JETChartView.h"
//-------------------------------------------------------------------------------------//
#define CThisChartCtrl    CJETChartView//目前使用的繪圖控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDraw3DWnd dialog
//-------------------------------------------------------------------------------------//
class CDraw3DWnd : public CBaseDialog
{
// Construction
public:
	CDraw3DWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDraw3DWnd)
	enum { IDD = IDD_DRAW3D_WND };
	CThisChartCtrl	m_ProfileWnd;
	CSliderCtrl	m_SliderWndY2;
	CSliderCtrl	m_SliderWndY1;
	CSliderCtrl	m_SliderWndX2;
	CSliderCtrl	m_SliderWndX1;
	COpenGLWnd	m_OpenGLWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDraw3DWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//-------------------------------------------------------------------------------------//
	void ReleaseData();//釋放資料
	bool CheckCalcObject(IMAGE_SIZE DataW, IMAGE_SIZE DataH) const;//確認是否要計算目標物體
	void Set3DData(const int* p3D, const unsigned char *p2D, IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, bool IsColor, float RulerMinH,float RulerMaxH, float ShowMinH, float ShowMaxH, RECT PadRect, RECT ROIRect, float PadSpecHeight);
	void Set3DData(const float* p3D, const unsigned char *p2D, IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, bool IsColor, float RulerMinH,float RulerMaxH, float ShowMinH, float ShowMaxH, RECT PadRect, RECT ROIRect, float PadSpecHeight);
	//-------------------------------------------------------------------------------------//
	void SetBoundPadOpen(bool Open);//顯示物件外框
	//-------------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	struct TPiece
	{
		int   nX1;
		int   nX2;
		float fValue;
		TPiece()
		{
			nX1=-1;
			nX2=-1;
			fValue=0.0f;
		}
	};	
	//-------------------------------------------------------------------------------------//
	bool                       m_AnimationPlay;
	bool                       m_ShowProfileWnd;
	//-------------------------------------------------------------------------------------//
	double                     m_ColorRangeMax;
	double                     m_ColorRangeMin;
	bool                       m_ColorRangeDefined;
	//-------------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	void                       AdjustControlWnd();//修正控制項位置
	void                       UpdateCommandUI();
	//-------------------------------------------------------------------------------------//
	bool                       InitProfileWnd(CThisChartCtrl &ChartWnd);//初始化圖表視窗-剖線圖
	bool                       BuildProfileWnd(CThisChartCtrl &ChartWnd);//建立圖表視窗-剖線圖
	bool                       SmoothProfile(int KenSize, std::vector<float> &DataList);//平滑線段
	bool                       BuildPieceList(const std::vector<float> &Profile, double dRatio, int nMinW, std::vector<TPiece> &PieceList);//取得子線段
	//---------------------------------------------------------------------------------//
	bool                       ExecLoadImageFile();
	bool                       ExecLoadSpaceFile();
	bool                       ExecSaveFloatFile();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDraw3DWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLoadImageMenu();
	afx_msg void OnLoadSpaceMenu();
	afx_msg void OnSaveFloatMenu();
	afx_msg void OnObjectColorMenu();	
	afx_msg void OnObjectLineMenu();	
	afx_msg void OnObjectTextureMenu();	
	afx_msg void OnObjectColorLineMenu();	
	afx_msg void OnUpperPlaneMenu();	
	afx_msg void OnLowerPlaneMenu();		
	afx_msg void OnBoundBoxMenu();
	afx_msg void OnShowProfileWndMenu();
	afx_msg void OnShowAxisMenu();
	afx_msg void OnDefaultMenu();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnBasePlaneMenu();
	afx_msg void OnPlanePitchMenu();
	afx_msg void OnPlaneHeightMenu();
	afx_msg void OnColorRangeMenu();
	afx_msg void OnAnimationPlayMenu();
	afx_msg void OnAnimationStopMenu();		
	afx_msg void OnDetailLevelMenuHighMost();	
	afx_msg void OnDetailLevelMenuHighMore();	
	afx_msg void OnDetailLevelMenuHigh();	
	afx_msg void OnDetailLevelMenuMiddle();	
	afx_msg void OnDetailLevelMenuLow();
	afx_msg void OnDetailLevelMenuLowMore();
	afx_msg void OnDetailLevelMenuLowMost();
	afx_msg void OnClipPlaneMenu();	
	afx_msg void OnClipPlaneHorMenu();	
	afx_msg void OnClipPlaneVerMenu();
	afx_msg void OnClipPlaneAnyMenu();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAW3DWND_H__42893C97_828F_4B4D_9BB5_524362F33479__INCLUDED_)
