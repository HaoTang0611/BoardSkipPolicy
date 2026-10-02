// Camera_CoaxLinkQuadCXP12_Camera.h: interface for the CCamera_CoaxLinkQuadCXP12_Camera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_COAXLINKQUADCPX12_CAMERA_H__78B9A893_949C_4BE0_847A_E2599AD57878__INCLUDED_)
#define AFX_CAMERA_COAXLINKQUADCPX12_CAMERA_H__78B9A893_949C_4BE0_847A_E2599AD57878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Camera_Basic.h"
//-------------------------------------------------------------------------------------//
#ifdef COAXLINK_QUAD_CXP12_CAMERA_USE
//-------------------------------------------------------------------------------------//
#define COAXLINK_FIREWARE_VERSION_196               196
#define COAXLINK_FIREWARE_VERSION_217               217
//-------------------------------------------------------------------------------------//
enum EURESYS_COAXLINK_MODULE
{
	EURESYS_COAXLINK_SYSTEM,
	EURESYS_COAXLINK_INTERFACE,
	EURESYS_COAXLINK_DEVICE,
	EURESYS_COAXLINK_REMOTE,
	EURESYS_COAXLINK_STREAM,
	EURESYS_SCORE_RETURN
};
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE		
	class CCamera_CoaxLinkQuadCXP12_Camera;
	#include "..\\JET8000_Library\\Coaxlink\\EuresysSharedGenTL.h"
	#include "..\\JET8000_Library\\Coaxlink\\EGrabber.h"	
	//---------------------------------------------------------------------------------//	
	class CEGrabber: public Euresys::EGrabber<Euresys::CallbackSingleThread>
	{
	public:
		//-----------------------------------------------------------------------------//			
		CEGrabber(Euresys::GenTL &gentl, int interfaceIndex=0, int deviceIndex=0, int dataStreamIndex=0);
		//-----------------------------------------------------------------------------//	
		virtual ~CEGrabber(void);		
		//-----------------------------------------------------------------------------//	
		CCamera_CoaxLinkQuadCXP12_Camera* GetCameraPtr();
		//-----------------------------------------------------------------------------//	
		void         SetCameraPtr(CCamera_CoaxLinkQuadCXP12_Camera *Ptr);
		//-----------------------------------------------------------------------------//	
	private:
		//-----------------------------------------------------------------------------//	
		CCamera_CoaxLinkQuadCXP12_Camera     *m_CameraPtr;		
		//-----------------------------------------------------------------------------//	
		virtual void onNewBufferEvent(const Euresys::NewBufferData& Data); //取像結束CallBack位置
		//-----------------------------------------------------------------------------//	
		virtual void onCicEvent(const Euresys::CicData &Data);		
		//-----------------------------------------------------------------------------//	
		virtual void onStreamDataEvent(const Euresys::DataStreamData &Data);		
		//-----------------------------------------------------------------------------//	
	};
	//---------------------------------------------------------------------------------//	
#endif
//-------------------------------------------------------------------------------------//
class CCamera_CoaxLinkQuadCXP12_Camera : public CCamera_Basic  
{
//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CCamera_CoaxLinkQuadCXP12_Camera)
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//		
	int                        m_FVersion;//韌體版本 (不同的韌體版本會有不同的參數設定)
	int                        m_CameraBitMode;//8Bit-10Bit
	CString                    m_CardSerialNumber;//Card Serial Number
	COAXPRESS_CAMERA_DEVICE    m_CameraDeviceType;//Coaxlink連到相機的樣式
#ifndef CAMERA_OBJ_DISABLE		
	Euresys::GenTL             m_genTL;
	CEGrabber*                 m_grabberPtr;		
#endif//CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	virtual void               PreInitCamera();
	//---------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE	
	//---------------------------------------------------------------------------------//
	CEGrabber*                 GetGrabberPtr();
	CEGrabber*                 CreateGrabberPtr(int interfaceIndex=0, int deviceIndex=0);
	bool                       SetGrabberPtr(CEGrabber *Ptr);
	bool                       CheckGrabberPtr(CEGrabber *pGrabber);	 
	bool                       FindInterfaceIndex(int nInterfaces, LPCTSTR CardSerial, int &interfaceIndex);
	bool                       FindInterfaceIndex_v2(GenICam::Client::TL_HANDLE &tl_Handle, int nInterfaces, LPCTSTR CardSerial, int &interfaceIndex);
	//---------------------------------------------------------------------------------//
	bool                       SetCoaxLinkFuncInt64(EURESYS_COAXLINK_MODULE Module, CEGrabber *pGrabber, const char *Key, int64_t Value);
	bool                       SetCoaxLinkFuncFlt(EURESYS_COAXLINK_MODULE Module, CEGrabber *pGrabber, const char *Key, float Value);
	bool                       SetCoaxLinkFuncStr(EURESYS_COAXLINK_MODULE Module, CEGrabber *pGrabber, const char *Key, const char *Value);
	//---------------------------------------------------------------------------------//
	bool                       SetupCameraDevice(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber);//設定相機	
	bool                       SetupCameraDevice_VC_12MX_M180(CEGrabber *pGrabber);//垂直接頭CX6
	bool                       SetupCameraDevice_VCC_25CXPHSM(CEGrabber *pGrabber);
	bool                       SetupCameraDevice_VC_25MX2_M150I(CEGrabber *pGrabber);
	bool                       SetupCameraDevice_STC_LBGP251BCXP124(CEGrabber *pGrabber);
	//---------------------------------------------------------------------------------//
	bool                       ResetFrameGrabberEventCount(CEGrabber *pGrabber);//清除影像擷取卡的事件計數器
	//---------------------------------------------------------------------------------//
	bool                       SwitchFreeRunMode(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber);		
	bool                       SwitchFreeRunMode_VC_12MX_M180(CEGrabber *pGrabber);
	bool                       SwitchFreeRunMode_VCC_25CXPHSM(CEGrabber *pGrabber);
	bool                       SwitchFreeRunMode_VC_25MX2_M150I(CEGrabber *pGrabber);
	bool                       SwitchFreeRunMode_STC_LBGP251BCXP124(CEGrabber *pGrabber);
	//---------------------------------------------------------------------------------//
	bool                       SwitchSoftwareTriggerMode(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber);		
	bool                       SwitchSoftwareTriggerMode_VC_12MX_M180(CEGrabber *pGrabber);	
	bool                       SwitchSoftwareTriggerMode_VCC_25CXPHSM(CEGrabber *pGrabber);
	bool                       SwitchSoftwareTriggerMode_VC_25MX2_M150I(CEGrabber *pGrabber);
	bool                       SwitchSoftwareTriggerMode_STC_LBGP251BCXP124(CEGrabber *pGrabber);
	//---------------------------------------------------------------------------------//
	bool                       SwitchExternalTriggerMode(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber);
	//---------------------------------------------------------------------------------//
	bool                       SwitchExternalTriggerMode_VC_12MX_M180_8DA1(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_VCC_25CXPHSM(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_VCC_25CXPHSM_DLP(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_VCC_25CXPHSM_A5V1(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_VCC_25CXPHSM_8DA1(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_VC_25MX2_M150I_8DA1(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_STC_LBGP251BCXP124(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_STC_LBGP251BCXP124_DLP(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_STC_LBGP251BCXP124_A5V1(CEGrabber *pGrabber);
	bool                       SwitchExternalTriggerMode_STC_LBGP251BCXP124_8DA1(CEGrabber *pGrabber);
	//---------------------------------------------------------------------------------//
	double                     ReadCameraFrameRate(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber);	
	double                     ReadCameraFrameRate_VC_12MX_M180(CEGrabber *pGrabber);
	double                     ReadCameraFrameRate_VCC_25CXPHSM(CEGrabber *pGrabber);
	double                     ReadCameraFrameRate_VC_25MX2_M150I(CEGrabber *pGrabber);
	double                     ReadCameraFrameRate_STC_LBGP251BCXP124(CEGrabber *pGrabber);
	//---------------------------------------------------------------------------------//	
#endif//CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
	bool                       CheckGrabberPtr();
	//---------------------------------------------------------------------------------//
	COAXPRESS_CAMERA_DEVICE     GetCameraDeviceType() const;//Coaxlink連到相機的樣式
	//---------------------------------------------------------------------------------//
	bool                       DoCoaxLinkStartGrab();
	//---------------------------------------------------------------------------------//
	bool                       ExecCameraPowerOff();//相機斷電
	bool                       ExecCameraPowerOn();//相機開電
	bool                       ExecCameraPowerReset();//相機復歸
	//---------------------------------------------------------------------------------//
	bool                       CheckCanSetExposureTime() const;//確認是否可以設定曝光時間
	bool                       ReturnErrorNoCameraDeviceType(COAXPRESS_CAMERA_DEVICE CameraDevice);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CCamera_CoaxLinkQuadCXP12_Camera();
	virtual ~CCamera_CoaxLinkQuadCXP12_Camera();
	//---------------------------------------------------------------------------------//
	virtual bool               InitialCamera();//相機初始化
	virtual bool               ReleaseCamera();//相機釋放
	virtual bool               ResetCamera();//復歸相機	
	//---------------------------------------------------------------------------------//	
	virtual bool               SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode=CAMERA_GRAB_FREE_RUN);
	virtual bool               SetExposureTime(const int ExposureTime);
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
	virtual double             ReadCameraTemperature();//讀取相機溫度
	//---------------------------------------------------------------------------------//
	virtual bool               WriteCameraParameterToDevice();//儲存目前相機的參數至相機內部的韌體上	
	virtual bool               SaveCameraINIFile();	
	virtual bool               LoadCameraINIFile();	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CCamera_CoaxLinkQuadCXP12_Camera Camera_CoaxLinkQuadCXP12_Camera;
//-------------------------------------------------------------------------------------//
#endif//COAXLINK_QUAD_CXP12_CAMERA_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_CAMERA_COAXLINKQUADCPX12_CAMERA_H__78B9A893_949C_4BE0_847A_E2599AD57878__INCLUDED_)
