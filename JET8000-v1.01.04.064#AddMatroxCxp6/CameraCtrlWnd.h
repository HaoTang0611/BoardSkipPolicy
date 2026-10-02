#if !defined(AFX_CAMERACTRLWND_H__1406CC70_B311_4C59_8235_1B838D9AFF31__INCLUDED_)
#define AFX_CAMERACTRLWND_H__1406CC70_B311_4C59_8235_1B838D9AFF31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CameraCtrlWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "Draw3DWnd.h"
#include "ImagePhaseWnd.h"
//-------------------------------------------------------------------------------------//
enum CAMERA_CTRL_GRAB_MODE
{
	CAMERA_CTRL_GRAB_NORMAL,
	CAMERA_CTRL_GRAB_AUTO_FOCUS_1,
	CAMERA_CTRL_GRAB_AUTO_FOCUS_10,
	CAMERA_CTRL_GRAB_AUTO_FOCUS_100,
	CAMERA_CTRL_GRAB_RETURN
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCameraCtrlWnd dialog
//-------------------------------------------------------------------------------------//
class CCameraCtrlWnd : public CDialog
{
// Construction
public:
	CCameraCtrlWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCameraCtrlWnd)
	enum { IDD = IDD_CAMERA_CTRL_WND };
	CComboBox	m_DlpLEDColorCombox;
	CComboBox	m_ImageScaleCombox;
	CComboBox	m_ImageSouceCombox;
	CComboBox   m_Frame2DCombox;
	CComboBox	m_PhaseStepCombox;
	CComboBox	m_PhasePeriodCombox;
	CComboBox	m_PhaseCastCombox;
	CComboBox	m_LightNumCombox;
	CComboBox	m_CallbackTimmingCombox;
	CComboBox	m_GrabModeCombox;
	CComboBox	m_CameraIdCombox;	
	CStatic	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraCtrlWnd)
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
	CImagePhaseWnd             m_PhaseWnd;
	CDraw3DWnd                 m_Draw3DWnd;//3D繪圖視窗
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;
	size_t                     m_ImageBufferSize;
	IMAGE_PTR                  m_ImageBufferPtr;
	IMAGE_PTR                  m_ImageBufferPtr2;	
	size_t                     m_ShowBufferSize;
	IMAGE_PTR                  m_ShowBufferPtr;		
	TNoiseFilterParam          m_NoiseFilterParam;	
	//---------------------------------------------------------------------------------//	
	std::vector<TUNI_FRAME>    m_UniFrameList;
	//---------------------------------------------------------------------------------//	
	bool                       CreatImageBuffer();
	bool                       DestroyImageBuffer();
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	bool                       RetrieveCameraImage_Grab(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	bool                       RetrieveCameraImage_BatchGrab(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	bool                       RetrieveCameraImage_UniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	bool                       ExecFireTrigger();
	bool                       RetrieveStagePosition();
	//---------------------------------------------------------------------------------//		
	long                       m_cntCameraBak;
	long                       m_cntExpBak;
	long                       m_cntImageBak;
	long                       m_cntImageCpy;	
	long                       m_cntImageBypass;
	CAMERA_CTRL_GRAB_MODE      m_CameraCtrlGrabMode;
	//---------------------------------------------------------------------------------//		
	CAMERA_ID                  m_CameraID;
	UINT                       m_GameraGrabBtn;
	DWORD                      m_CameraBatchGrabMode;//批次取像模式	
	long                       m_CameraNFramesToGrab;
	LARGE_INTEGER              m_CameraGrabStartTime;//相機取像的起始時間
	LARGE_INTEGER              m_CameraGrabEndTime;//相機取像的結束時間
	//---------------------------------------------------------------------------------//		
	COLORREF                   m_BkColor;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;
	//---------------------------------------------------------------------------------//
	POINT                      m_MovingPos;
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;
	TPOINT2D                   m_ImageOffset;	
	double                     m_ImageZoom;	

	TPOINT2D                   m_ImageWndPt1;
	TPOINT2D                   m_ImageWndPt2;

	TPOINT2D                   m_ImagePt1;
	TPOINT2D                   m_ImagePt2;

	CString                    m_strPixel;
	bool                       m_DrawRect;
	//---------------------------------------------------------------------------------//
	bool                       m_IsLocked;
	double                     m_StagePosX;
	double                     m_StagePosY;
	double                     m_StagePosZ;
	//---------------------------------------------------------------------------------//
	TRECT4D                    m_AutoFrcusRect;
	double                     m_AutoFocusPitch;		
	double                     m_AutoFocustBestStd;
	double                     m_AutoFocustMinPosZ;
	double                     m_AutoFocustMaxPosZ;
	double                     m_AutoFocustBestPosZ;
	double                     m_AutoFocustLastPosZ;
	double                     m_AutoFocustScalePosZ;
	std::vector<TPOINT2D>      m_AutoFocusReadingList;
	//---------------------------------------------------------------------------------//
	bool                       PtInControlWnd(const POINT &pt, UINT ControlID, POINT &pt2);
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       UpdateShowBufferImage();//更新顯示影像
	void                       DrawImageWndMemDC();//建立影像的記憶體圖像
	void                       CreateImageSourceImage();//建立影像來源的圖像
	//---------------------------------------------------------------------------------//
	bool                       BuildImageScaleModeCombox(CComboBox &Combox);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	void                       DrawRect(HDC hDC);//繪製矩形
	void                       DrawCenterLine(HDC hDC);//繪製中心線
	void                       UpdateCursorInfo(POINT WndPt);//更新鼠標資訊
	//---------------------------------------------------------------------------------//
	void                       LockUIWnd(bool bLock);//鎖住視窗
	bool                       GetLockUIWnd() const;
	//---------------------------------------------------------------------------------//	
	bool                       ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam);
	bool                       ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt);
	//---------------------------------------------------------------------------------//
	bool                       StartTimerCameraGrab();//開始相機取像
	bool                       KillTimerCameraGrab();//停止相機取像
	//---------------------------------------------------------------------------------//	
	bool                       ResetShowImageBuffer();
	//---------------------------------------------------------------------------------//	
	bool                       ExecGrabFunc();
	bool                       ExecGrabTest();
	bool                       ExecGrabBatch();
	bool                       ExecGrabFrame();
	//---------------------------------------------------------------------------------//		
	bool                       ExecGrabNextFunc();
	bool                       ExecGrabFrameTest();
	bool                       ExecGrabBatchNext();
	bool                       ExecGrabFrameNext();
	//---------------------------------------------------------------------------------//		
	bool                       ExecStageMoveTo(POINT point);
	//---------------------------------------------------------------------------------//		
	bool                       ExecAutoFocusNext();
	//---------------------------------------------------------------------------------//		
	void                       ResetCameraCount();
	//---------------------------------------------------------------------------------//
	bool                       ExecBuild3DObject();
	bool                       SaveAutoFocusReading(const std::vector<TPOINT2D> &ReadingList);
	bool                       CalcObject(IMAGE_SIZE ModelW, IMAGE_SIZE ModelH, IMAGE_SIZE ModelStep, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, RECT &ObjRect, float &ObjH);//求得物體的資料
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCameraCtrlWnd)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnGrabBtn();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnFireTriggerBtn();
	afx_msg void OnCancelLockBtn();
	afx_msg void OnSaveImageBtn();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnGrabContinueChk();
	afx_msg void OnSelchangeCameraIDCombo();
	afx_msg void OnBatchGrabBtn();
	afx_msg void OnFrameGrabBtn();
	afx_msg void OnResetLightCtrlBtn();
	afx_msg void OnSelchangeFrame2dCombox();
	afx_msg void OnFocusAutoBtn();
	afx_msg void OnBuild3dObjBtn();
	afx_msg void OnViewAllBtn();
	afx_msg void OnView1x1Btn();
	afx_msg void OnViewCenterLineChk();
	afx_msg void OnFrame3dChk();
	afx_msg void OnSelchangeImageSourceCombo();
	afx_msg void OnSelchangeDlpLedColorCombo();
	afx_msg void OnResetCameraBtn();
	afx_msg void OnBasePlaneParamBtn();
	afx_msg void OnSpaceNoiseFilterBtn();
	afx_msg void OnWhiteBalanceSetBtn();
	afx_msg void OnWhiteBalanceReadBtn();
	afx_msg void OnWhiteBalanceCalcBtn();
	afx_msg void OnReadTemperatureBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
extern CCameraCtrlWnd   CameraCtrlWnd;
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERACTRLWND_H__1406CC70_B311_4C59_8235_1B838D9AFF31__INCLUDED_)
