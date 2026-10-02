#if !defined(AFX_NEWPROJECTPANELALIGNPANE_H__C44079A0_5387_4C61_A6A0_FF66D53A3379__INCLUDED_)
#define AFX_NEWPROJECTPANELALIGNPANE_H__C44079A0_5387_4C61_A6A0_FF66D53A3379__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectPanelAlignPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneAlignPanel dialog
//-------------------------------------------------------------------------------------//
class CNewProjectPaneAlignPanel : public CDialog
{
// Construction
public:
	CNewProjectPaneAlignPanel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectPaneAlignPanel)
	enum { IDD = IDD_NEW_PROJECT_PANE_ALIGN_PANEL };
	CComboBox	m_ComponentCombox4;
	CComboBox	m_ComponentCombox3;
	CComboBox	m_ComponentCombox2;
	CComboBox	m_ComponentCombox1;
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectPaneAlignPanel)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	bool                       ExecNextPane();
	bool                       ExecPrevPane();
	bool                       ExecFinishPane();
	bool                       ReInitialPane();
	//---------------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *ProjectPtr);	
	void                       SetShowBuffer(size_t BufferSize, IMAGE_PTR Ptr);
	void                       SetImageBuffer(size_t BufferSize, IMAGE_PTR ImagePtr);	
	//---------------------------------------------------------------------------------//
	DISTRICT_ID			       GetDistrictID() const;
	void                       SetDistrictID(DISTRICT_ID Mode);
	//---------------------------------------------------------------------------------//	
	NEW_PROJECT_MODE           GetNewProjectMode() const;
	void                       SetNewProjectMode(NEW_PROJECT_MODE Mode);
	//---------------------------------------------------------------------------------//	
	bool                       GetEnableMultiDistrictMode() const;
	void                       SetEnableMultiDistrictMode(bool Mode);
	//---------------------------------------------------------------------------------//
	bool                       GetEnableModifyPanelDirection() const;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;
	CAOIProject               *m_ProjectPtr;	
	DISTRICT_ID                m_DistrictID;
	NEW_PROJECT_MODE           m_NewProjectMode;	
	bool                       m_EnableMultiDistrictMode;//多段模式
	//---------------------------------------------------------------------------------//		
	RECT                       m_ImageWndRect;	
	FRAME_TYPE                 m_FrameType;
	unsigned int               m_FrameIndex;
	unsigned int               m_FrameUniqueID;//取像畫面的唯一碼
	//---------------------------------------------------------------------------------//
	bool                       m_ResetView;
	double                     m_FovStageX;
	double                     m_FovStageY;
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_PTR                  m_ImageBuffer;
	size_t                     m_ImageBufferSize;
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE                 m_ShowImageStep;
	IMAGE_SIZE                 m_ShowBitCount;
	IMAGE_PTR                  m_ShowBuffer;
	size_t                     m_ShowBufferSize;	
	//---------------------------------------------------------------------------------//	
	size_t                     m_CaliComponentIdx[4];//校正的零件引數
	double                     m_CaliComponentCadPosX[4];//校正零件的座標-X-Cad
	double                     m_CaliComponentCadPosY[4];//校正零件的座標-Y-Cad
	double                     m_CaliComponentStagePosX[4];//校正零件的座標-X-Stage
	double                     m_CaliComponentStagePosY[4];//校正零件的座標-Y-Stage
	//---------------------------------------------------------------------------------//	
	double                     m_EdgeStagePosX[2];
	double                     m_EdgeStagePosY[2];
	//---------------------------------------------------------------------------------//		
	bool                       m_BoxApplyRotation;
	//---------------------------------------------------------------------------------//		
	CAOIPanel*                 GetActivePanelPtr();//取得目前取用的整板指標
	CAOIProject*               GetActiveProjectPtr();//取得目前取用的專案指標
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//		
	void                       RedrawWnd();
	BOOL                       CreateBKDC(bool bResetView);//建立背景DC	
	BOOL                       ExecGrabImage();
	bool                       LockUIWnd(bool bLock);	
	BOOL                       DrawCtrlWnd(WPARAM wParam, LPARAM lParam);//在控制像繪圖後重新繪圖
	bool                       UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage);
	bool                       RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage);//取得相機影像	
	bool                       LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool &bGetImage);
	BOOL                       RedrawProjectImageWnd();//重繪專案影像視窗
	//---------------------------------------------------------------------------------//	
	bool                       SendParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	bool                       PostParentWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//發送訊息給父視窗
	//---------------------------------------------------------------------------------//	
	void                       BuildComponentCombox(CComboBox &Combox);
	void                       InitialComponentCombox();//分配零件預設使用四個端點
	//---------------------------------------------------------------------------------//
	bool                       ChangeDistrictID(DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjectPaneAlignPanel)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnOrientationRotate090Btn();
	afx_msg void OnOrientationRotate180Btn();
	afx_msg void OnOrientationRotate270Btn();
	afx_msg void OnOrientationMirrorXBtn();
	afx_msg void OnOrientationMirrorYBtn();
	afx_msg void OnPaint();
	afx_msg void OnOrientationCenteredBtn();
	afx_msg void OnSelchangeAlignComponentCombo1();
	afx_msg void OnAlignComponentSetBtn1();
	afx_msg void OnSelchangeAlignComponentCombo2();
	afx_msg void OnAlignComponentSetBtn2();
	afx_msg void OnAlignComponentGoBtn1();
	afx_msg void OnAlignComponentGoBtn2();
	afx_msg void OnSelchangeAlignComponentCombo3();
	afx_msg void OnAlignComponentSetBtn3();
	afx_msg void OnAlignComponentGoBtn3();
	afx_msg void OnAssignComponentBtn();
	afx_msg void OnSelchangeAlignComponentCombo4();
	afx_msg void OnAlignComponentSetBtn4();
	afx_msg void OnAlignComponentGoBtn4();
	afx_msg void OnPanelEdgePos1GoBtn();
	afx_msg void OnPanelEdgePos2GoBtn();
	afx_msg void OnPanelEdgePos1SetBtn();
	afx_msg void OnPanelEdgePos2SetBtn();
	afx_msg void OnPanelEdgeCalcBtn();
	afx_msg void OnShowCenterLine();
	afx_msg void OnDistrictABtn();
	afx_msg void OnDistrictBBtn();
	afx_msg void OnApplyRotation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTPANELALIGNPANE_H__C44079A0_5387_4C61_A6A0_FF66D53A3379__INCLUDED_)
