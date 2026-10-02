// Camera_Coaxlink_VC_12MX_M180.h: interface for the CCamera_Coaxlink_VC_12MX_M180 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_COAXLINK_VC_12MX_M180_H__0104B43A_AD28_4244_A47E_07F626CE8EF3__INCLUDED_)
#define AFX_CAMERA_COAXLINK_VC_12MX_M180_H__0104B43A_AD28_4244_A47E_07F626CE8EF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Camera_Basic.h"
//-------------------------------------------------------------------------------------//
#ifdef COAXLINK_VC_12MX_M180_USE
//-------------------------------------------------------------------------------------//
#define COAXLINK_FIREWARE_VERSION_196               196
#define COAXLINK_FIREWARE_VERSION_217               217
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE	
	#include "..\\JET8000_Library\\Coaxlink\\EuresysSharedGenTL.h"
	#include "..\\JET8000_Library\\Coaxlink\\EGrabber.h"	
	//---------------------------------------------------------------------------------//	
	class CEGrabber: public Euresys::EGrabber<Euresys::CallbackSingleThread>
	{
	public:
		//-----------------------------------------------------------------------------//			
		CEGrabber(Euresys::GenTL &gentl);		
		//-----------------------------------------------------------------------------//	
		virtual ~CEGrabber(void);		
		//-----------------------------------------------------------------------------//	
	private:
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
class CCamera_Coaxlink_VC_12MX_M180 : public CCamera_Basic  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CCamera_Coaxlink_VC_12MX_M180)
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	int                        m_FVersion;//韌體版本 (不同的韌體版本會有不同的參數設定)
#ifndef CAMERA_OBJ_DISABLE
	Euresys::GenTL             m_genTL;
	CEGrabber*                 m_grabberPtr;	
#endif//CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	virtual void               PreInitCamera();
	//---------------------------------------------------------------------------------//
	bool                       CheckGrabberPtr();
	//---------------------------------------------------------------------------------//
	bool                       DoCoaxLinkStartGrab();
	//---------------------------------------------------------------------------------//
	bool                       SwitchFreeRunMode();
	bool                       SwitchSoftwareTriggerMode();
	bool                       SwitchExternalTriggerMode();
	bool                       SwitchExternalTriggerMode_DLP();
	bool                       SwitchExternalTriggerMode_A5V1();
	bool                       SwitchExternalTriggerMode_8DA1();
	bool                       ResetFrameGrabberEventCount();//清除影像擷取卡的事件計數器
	//---------------------------------------------------------------------------------//
	bool                       ExecCameraPowerOff();//相機斷電
	bool                       ExecCameraPowerOn();//相機開電
	bool                       ExecCameraPowerReset();//相機復歸
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CCamera_Coaxlink_VC_12MX_M180();
	virtual ~CCamera_Coaxlink_VC_12MX_M180();
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
	//---------------------------------------------------------------------------------//
	virtual bool               WriteCameraParameterToDevice();//儲存目前相機的參數至相機內部的韌體上	
	virtual bool               SaveCameraINIFile();	
	virtual bool               LoadCameraINIFile();	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CCamera_Coaxlink_VC_12MX_M180 Camera_CoaxLink_VC_12MX_M180;
//-------------------------------------------------------------------------------------//
#endif//COAXLINK_VC_12MX_M180_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_CAMERA_COAXLINK_VC_12MX_M180_H__0104B43A_AD28_4244_A47E_07F626CE8EF3__INCLUDED_)
