// Camera_DynamicModule.h: interface for the CCamera_DynamicModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_DYNAMICMODULE_H__15BD4AA6_E170_4D42_A3B0_BEDCCA758BF9__INCLUDED_)
#define AFX_CAMERA_DYNAMICMODULE_H__15BD4AA6_E170_4D42_A3B0_BEDCCA758BF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Camera_Basic.h"
//-------------------------------------------------------------------------------------//
#ifdef CAMERA_DYNAMIC_MODULE_USE
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE	
	#include "Camera2DUnit.h"
#endif
//-------------------------------------------------------------------------------------//
class CCamera_DynamicModule : public CCamera_Basic  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CCamera_DynamicModule)
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//
	bool                       m_LoadCameraDll;
	CString                    m_DynamicModuleName;
	//---------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE	
	C2D_HANDLE   m_CameraHandle;
#endif//CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	virtual void               PreInitCamera();
	//---------------------------------------------------------------------------------//
	bool                       CheckCameraHandle();
	//---------------------------------------------------------------------------------//
	bool                       LoadCameraDll(const wchar_t *pDll);
	bool                       FreeCameraDll();
	//---------------------------------------------------------------------------------//
	bool                       InitCameraInstance(const wchar_t *pDll, const wchar_t *pDllIni);
	bool                       ExitCameraInstance();
#ifndef CAMERA_OBJ_DISABLE	
	bool                       GetCameraErrorText(C2D_ERROR ErrCode, CString &str);
#endif//CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
	bool                       LoadCameraModuleDefine();//載入相機模組定義 
	bool                       UpdateCameraModuleName();//更新相機模組名稱
	//---------------------------------------------------------------------------------//
	bool                       DoCameraModuleStartGrab();
	//---------------------------------------------------------------------------------//
	bool                       SwitchFreeRunMode();
	bool                       SwitchSoftwareTriggerMode();
	bool                       SwitchExternalTriggerMode();
	bool                       SwitchExternalTriggerMode_DLP();
	bool                       SwitchExternalTriggerMode_A5V1();//LIGHT_CTRL_BOARD_TYPE_A5V1
	bool                       SwitchExternalTriggerMode_A5V2();//LIGHT_CTRL_BOARD_TYPE_A5V2	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CCamera_DynamicModule(CAMERA_ID CameraID);
	virtual ~CCamera_DynamicModule();
	//---------------------------------------------------------------------------------//
	virtual bool               InitialCamera();//相機初始化
	virtual bool               ReleaseCamera();//相機釋放
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
extern CCamera_DynamicModule Camera_DynamicModule;
//-------------------------------------------------------------------------------------//
#endif//CAMERA_DYNAMIC_MODULE_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_CAMERA_DYNAMICMODULE_H__15BD4AA6_E170_4D42_A3B0_BEDCCA758BF9__INCLUDED_)
