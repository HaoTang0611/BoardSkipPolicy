#if !defined(AFX_EDITIMAGEVIEW3DPAGE_H__2D578B7A_369C_43CF_9DC8_1C8C1195FA3D__INCLUDED_)
#define AFX_EDITIMAGEVIEW3DPAGE_H__2D578B7A_369C_43CF_9DC8_1C8C1195FA3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImageView3DPage.h : header file
//
//-------------------------------------------------------------------------------------//
#include "OpenGLWnd.h"
#include "JetChart\\JETChartView.h"
//-------------------------------------------------------------------------------------//
#define CThisChartCtrl    CJETChartView//目前使用的繪圖控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageView3DPage dialog
//-------------------------------------------------------------------------------------//
class CEditImageView3DPage : public CDialog
{
// Construction
public:
	CEditImageView3DPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditImageView3DPage)
	enum { IDD = IDD_EDIT_IMAGE_VIEW3D_WND };
	COpenGLWnd	m_OpenGLWnd;
	CThisChartCtrl	m_ChartWnd;
	CComboBox	m_DetailLeveCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditImageView3DPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
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
	//---------------------------------------------------------------------------------//		
	CAOIWnd                   *m_WndPtr;
	CAOIProject               *m_ProjectPtr;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_SIZE                 m_ImageStep;	
	MASK_PTR                   m_MaskPtr;
	IMAGE_PTR                  m_ImagePtr;	
	SPACE_PTR                  m_SpacePtr;
	IMAGE_PTR				   m_ImageTmpPtr;
	float                      m_TargetHegiht;
	float                      m_TargetRuleMax;
	float                      m_TargetRuleMin;	
	//---------------------------------------------------------------------------------//	
	POINT                      m_MousePosFirst;
	POINT                      m_MousePosLast;
	//---------------------------------------------------------------------------------//		
	void                       CloseProject();
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//	
	void                       SetImageBuffer(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE Bit, IMAGE_PTR Ptr2D, MASK_PTR PtrMsk, SPACE_PTR Ptr3D);
	bool                       GetImageBuffer(IMAGE_SIZE &W, IMAGE_SIZE &H, IMAGE_SIZE &Step, IMAGE_SIZE &Bit, IMAGE_PTR &Ptr2D, MASK_PTR &PtrMsk, SPACE_PTR &Ptr3D);
	void                       ClearImageBuffer();
	//---------------------------------------------------------------------------------//	
	void                       AdjustView3DWnd();
	bool                       BuildDetailLeveCombox();
	void                       UpdateParamToUI();
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);	
	
	void                       UpdateCommandUI();
	void                       UpdateCtrlUIEnabled();//更新控制介面是否啟用
	bool                       ExecPopupMenu(POINT point, UINT menuID, UINT ID);
	//---------------------------------------------------------------------------------//
	void                       ReleaseUniFrameList();
	bool                       ClearView3DPoints();//清除顯示3D的資料
	bool                       GetRuleColorRange(float &MinH, float &MaxH);
	void                       BuildView3DPoints(bool UpdateView, bool ResetZoom);//建立顯示3D的資料
	bool                       ExecBuildView3DPoints(bool UpdateView, bool ResetZoom);//建立顯示3D的資料	
	void                       UpdateView3DBox();//更新顯示3D的資料	
	void                       UpdateView3DBox_Data();//更新顯示3D的資料
	void                       UpdateView3DBox_Rect();//更新顯示3D的資料	
	bool                       CheckUpdate3DBoxData();//確認更新顯示3D模式
	//---------------------------------------------------------------------------------//
	bool                       InitProfileWnd(CThisChartCtrl &ChartWnd);//初始化圖表視窗-剖線圖
	bool                       ShowProfileWnd(CThisChartCtrl &ChartWnd);//顯示圖表視窗-剖線圖
	bool                       BuildProfileWnd(CThisChartCtrl &ChartWnd);//建立圖表視窗-剖線圖
	bool                       SmoothProfile(int KenSize, std::vector<float> &DataList);//平滑線段
	bool                       BuildPieceList(const std::vector<float> &Profile, double dRatio, int nMinW, std::vector<TPiece> &PieceList);//取得子線段
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditImageView3DPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnObjectColorMenu();	
	afx_msg void OnObjectLineMenu();	
	afx_msg void OnObjectTextureMenu();	
	afx_msg void OnObjectColorLineMenu();	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnToolbarModelBtn();
	afx_msg void OnToolbarShowBtn();
	afx_msg void OnUpperPlaneMenu();	
	afx_msg void OnLowerPlaneMenu();	
	afx_msg void OnDefaultMenu();
	afx_msg void OnBasePlaneMenu();
	afx_msg void OnPlanePitchMenu();
	afx_msg void OnPlaneHeightMenu();
	afx_msg void OnObjectColorRadio();
	afx_msg void OnObjectLineRadio();
	afx_msg void OnObjectTextureRadio();
	afx_msg void OnObjectColorLineRadio();
	afx_msg void OnPlaneUpperChk();
	afx_msg void OnPlaneBaseChk();
	afx_msg void OnPlaneLowerChk();
	afx_msg void OnPlanePitchBtn();
	afx_msg void OnViewDefaultBtn();
	afx_msg void OnAnimationPlayBtn();
	afx_msg void OnAnimationStopBtn();
	afx_msg void OnPlaneClipChk();
	afx_msg void OnPlaneClipHorRdo();
	afx_msg void OnPlaneClipVerRdo();
	afx_msg void OnPlaneClipAnyRdo();
	afx_msg void OnSelchangeDetailLevelCombo();
	afx_msg void OnViewWndBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_EDITIMAGEVIEW3DPAGE_H__2D578B7A_369C_43CF_9DC8_1C8C1195FA3D__INCLUDED_)
