#if !defined(AFX_NEWPROJECTFIDUCIALPANE_H__F25EC866_D721_4FB9_8CAF_71118E31ECDB__INCLUDED_)
#define AFX_NEWPROJECTFIDUCIALPANE_H__F25EC866_D721_4FB9_8CAF_71118E31ECDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewProjectFiducialPane.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CNewProjectPaneFiducial dialog

class CNewProjectPaneFiducial : public CDialog
{
// Construction
public:
	CNewProjectPaneFiducial(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewProjectPaneFiducial)
	enum { IDD = IDD_NEW_PROJECT_PANE_FIDUCIAL };
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewProjectPaneFiducial)
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
protected:
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;
	CAOIProject               *m_ProjectPtr;	
	DISTRICT_ID                m_DistrictID;
	NEW_PROJECT_MODE           m_NewProjectMode;
	bool                       m_ReCalcStagePos;
	CAD_FILE_CONTENT_MODE      m_CADFileContentMode;
	bool                       m_EnableMultiDistrictMode;//多段模式
	//---------------------------------------------------------------------------------//	
	unsigned int               m_FdIndex;	
	//---------------------------------------------------------------------------------//	
	CAMERA_ID                  m_CameraID;
	LIGHT_MODE                 m_LightMode;
	FRAME_TYPE                 m_FrameType;
	unsigned int               m_FrameIndex;
	unsigned int               m_FrameUniqueID;//取像畫面的唯一碼
	//---------------------------------------------------------------------------------//
	bool                       m_ResetView;
	double                     m_FovStageX;
	double                     m_FovStageY;	
	TPOINT2D                   m_FdCadPos;
	TPOINT3D                   m_FdStagePos;
	bool                       m_GetComponentPos;
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
	CAOIFd*                    GetPanelFdPtr(size_t idx);
	CAOIProject*               GetActiveProjectPtr();//取得目前專案指標
	CAOIPanel*                 GetActivePanelPtr();//取得目前取用的整板指標
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	BOOL                       CreateBKDC(bool ResetView);//建立背景DC	
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
	BOOL                       UpdateFdUIRadio();//更新定位點Radio介面 
	BOOL                       UpdateFdParamToUI(unsigned int FdIdx);//更新參數至定位點介面
	BOOL                       UpdateFdParamToUI(CAOIFd *FdPtr);//更新參數至定位點介面
	BOOL                       UpdateFdParamToObj(CAOIFd *FdPtr);//更新參數至定位點物件	
	//---------------------------------------------------------------------------------//
	bool                       OnImageWndNotify(WPARAM wParam, LPARAM lParam);
	bool                       OnImageWndLButtonUp(LPARAM lParam);
	//---------------------------------------------------------------------------------//		
	bool                       UpdateFdCount();
	bool                       ExecSavePanelFd();
	bool                       ExecCalcPanelBasePlane();
	bool                       ChangeDistrictID(DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewProjectPaneFiducial)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFdAddBtn();
	afx_msg void OnFdDelBtn();
	afx_msg void OnFdRadio1();
	afx_msg void OnFdRadio2();
	afx_msg void OnFdRadio3();
	afx_msg void OnFdRadio4();
	afx_msg void OnUpdateExtendPatternWEdit();
	afx_msg void OnUpdateExtendPatternHEdit();	
	afx_msg void OnSelectComponentBtn();
	afx_msg void OnDistrictABtn();
	afx_msg void OnDistrictBBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPROJECTFIDUCIALPANE_H__F25EC866_D721_4FB9_8CAF_71118E31ECDB__INCLUDED_)
