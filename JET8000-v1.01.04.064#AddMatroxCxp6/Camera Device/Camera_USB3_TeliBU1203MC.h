// Camera_USB3_TeliBU1203MC.h: interface for the CCamera_USB3_TeliBU1203MC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_USB3_TELIBU1203MC_H__471B1FD3_B918_409E_B8C4_8AE04C77C274__INCLUDED_)
#define AFX_CAMERA_USB3_TELIBU1203MC_H__471B1FD3_B918_409E_B8C4_8AE04C77C274__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Camera_Basic.h"
//-------------------------------------------------------------------------------------//
#ifdef USB3_TELI_BU1203MC_USE
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE		
	#include "..\\JET8000_Library\\TeliCam\\3_0_2_1\\Include\\TeliCamApi.h"
	#include "..\\JET8000_Library\\TeliCam\\3_0_2_1\\Include\\TeliCamUtl.h"	
	#ifdef _X64
		#pragma comment(lib, "..\\JET8000_Library\\TeliCam\\3_0_2_1\\x64\\Lib\\TeliCamApi64.lib")
		#pragma comment(lib, "..\\JET8000_Library\\TeliCam\\3_0_2_1\\x64\\Lib\\TeliCamUtl64.lib")
	#else
		#pragma comment(lib, "..\\JET8000_Library\\TeliCam\\3_0_2_1\\x32\\Lib\\TeliCamApi.lib")
		#pragma comment(lib, "..\\JET8000_Library\\TeliCam\\3_0_2_1\\x32\\Lib\\TeliCamUtl.lib")
	#endif//_X64
#endif
class CCamera_USB3_TeliBU1203MC : public CCamera_Basic  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CCamera_USB3_TeliBU1203MC)
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	CString                    m_CameraSerialNumber;//Camera Serial Number
	double                     m_CameraWhiteBalanceRatioRed;
	double                     m_CameraWhiteBalanceRatioGrn;
	double                     m_CameraWhiteBalanceRatioBlu;
	//---------------------------------------------------------------------------------//	
#ifndef CAMERA_OBJ_DISABLE
	Teli::CAM_HANDLE           m_hCam;         //Camera Handle.
	Teli::CAM_STRM_HANDLE      m_hStrm;        // Stream handle.
	Teli::CAM_EVT_HANDLE       m_hEvent;       // Event handle.
	HANDLE                     m_hStrmEvt;     //Completion event for stream.
	HANDLE                     m_hFrmTrgWaitEvt;//Event for Frame Trigger Event	
	bool                       m_TeliCameraBufferEnabled;
#endif//endif CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//	
protected:	
	//---------------------------------------------------------------------------------//
	virtual void               PreInitCamera();	
	bool                       ConnectCamera();			
	//---------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE
	Teli::CAM_HANDLE           GetCameraHandle();
	Teli::CAM_STRM_HANDLE      GetStreamHandle();
	Teli::CAM_EVT_HANDLE       GetEventHandle();
	void                       CloseTeliSystem();	
	void                       CloseCameraHandle(Teli::CAM_HANDLE &hCam);	
	void                       CloseEventHandle(Teli::CAM_EVT_HANDLE &hEvt, HANDLE &hTrgEvt);
	void                       CloseStreamHandle(Teli::CAM_STRM_HANDLE &hStrm, HANDLE &hStrmEvt);
	bool                       DoTeliCameraStartGrab();	
	bool                       GetTeliCameraBufferEnabled() const;
	bool                       GetTeliErrorString(Teli::CAM_API_STATUS Status, CString &err);	
#endif//CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CCamera_USB3_TeliBU1203MC();
	virtual ~CCamera_USB3_TeliBU1203MC();
	//---------------------------------------------------------------------------------//
	virtual bool               InitialCamera();//相機初始化
	virtual bool               ReleaseCamera();//相機釋放
	virtual bool               ResetCamera();//復歸相機
	//---------------------------------------------------------------------------------//		
	virtual bool               SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode=CAMERA_GRAB_FREE_RUN);
	virtual bool               SetExposureTime(const int ExposureTime);
	virtual bool               FreeCameraImagePtr(void *ImagePtr);//釋放相機內的影像
	virtual bool               StartCameraGrab();//只取一張影像-內部觸發用
	virtual bool               StartCameraLiveGrab();//連續取多張影像-外部觸發用
	virtual bool               StopCameraGrab();//停止取像
	virtual bool               FireSoftwareTrigger();//發射軟體觸發訊號
	virtual bool               WaitforCameraReadytoTrigger();//等待相機準備好可以觸發
	virtual bool               ResetCameraReadyTriggerEvent();//復歸相機準備好了的事件
	virtual bool               DoCameraDebayer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const unsigned char *pRaw, unsigned char *pResult, BAYER_PATTERN_MODE BayerPattern);
	virtual bool               GetWhiteBalanceParams(double &WBR, double &WBG, double &WBB);
	virtual bool               SetWhiteBalanceParams(double WBR, double WBG, double WBB);
	virtual bool               CalcWhiteBalanceParams();
	virtual bool               ResetWhiteBalanceParams();
	//---------------------------------------------------------------------------------//
	virtual bool               WriteCameraParameterToDevice();//儲存目前相機的參數至相機內部的韌體上	
	virtual bool               SaveCameraINIFile();
	virtual bool               LoadCameraINIFile();	
	//---------------------------------------------------------------------------------//		
};
//-------------------------------------------------------------------------------------//
extern CCamera_USB3_TeliBU1203MC Camera_USB3_TeliBU1203MC;
//-------------------------------------------------------------------------------------//
#endif//USB3_TELI_BU1203MC_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_CAMERA_USB3_TELIBU1203MC_H__471B1FD3_B918_409E_B8C4_8AE04C77C274__INCLUDED_)
