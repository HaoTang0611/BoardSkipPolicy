// Camera_MatroxRapixoCXP12_Camera: implementation of the CCamera_MatroxRapixoCXP12_Camera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_MATROXRAPIXOCXP12_CAMERA_H__78B9A893_949C_4BE0_847A_ASA12345__INCLUDED_)
#define AFX_CAMERA_MATROXRAPIXOCXP12_CAMERA_H__78B9A893_949C_4BE0_847A_ASA12345__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Camera_Basic.h"
#include <string>
//-------------------------------------------------------------------------------------//
#ifdef MATROX_RAPIXO_CXP12_CAMERA_USE
//-------------------------------------------------------------------------------------//
enum eTriggerSelector
{
	eTriggerSelector_AcquisitionStart = 0,
	eTriggerSelector_FrameStart,
	eTriggerSelector_ExposureStart,
};
//-------------------------------------------------------------------------------------//
enum eTriggerMode
{
	eTriggerMode_Off = 0,
	eTriggerMode_On,
};
//-------------------------------------------------------------------------------------//
enum eTriggerSource
{
	eTriggerSource_LinkTrigger0 = 0,
	eTriggerMode_Line0,
	eTriggerMode_Software,
};
//-------------------------------------------------------------------------------------//
enum eTriggerActivation
{
	eTriggerSource_RisingEdge = 0,
	eTriggerMode_FallingEdge,
	eTriggerMode_LevelHigh,
	eTriggerMode_LevelLow,
};
//-------------------------------------------------------------------------------------//
enum eExposureMode
{
	eExposureMode_Timed = 0,
	eExposureMode_TriggerWidth,
};
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE
class CCamera_MatroxRapixoCXP12_Camera;
#include "..\\JET8000_Library\\Matrox\\10_70_0963\\Include\\mil.h"
#pragma comment(lib, "..\\JET8000_Library\\Matrox\\10_70_0963\\Lib\\mil.lib")
//---------------------------------------------------------------------------------//
MIL_INT WINAPI ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);
#endif

class CCamera_MatroxRapixoCXP12_Camera : public CCamera_Basic
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CCamera_MatroxRapixoCXP12_Camera)
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	int                        m_FVersion;//韌體版本 (不同的韌體版本會有不同的參數設定)
	int                        m_CameraBitMode;//8Bit-10Bit
	CString                    m_CardSerialNumber;//Card Serial Number
	COAXPRESS_CAMERA_DEVICE    m_CameraDeviceType;//Coaxlink連到相機的樣式
	int						   m_BufferCount = 1024;
	UINT                       m_CCDTrigEdge = CCD_TRIG_EDGE_L;
#ifndef CAMERA_OBJ_DISABLE
	MIL_STRING				   m_CameraVendor;
	MIL_STRING                 m_CameraModel;
	static const int		   MilGrabBufferListMaxSize = 1024;

	MIL_ID                     m_MilApplication;// Application identifier. 
	MIL_ID                     m_MilSystem;     // System identifier. 
	MIL_ID                     m_MilDigitizer;  // Digitizer identifier.
												//MIL_ID                     m_MilImage;
	MIL_ID                     m_MilGrabBufferList[MilGrabBufferListMaxSize] = { 0 };
	MIL_INT					   m_MilGrabBufferListSize;
#endif // CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	virtual void               PreInitCamera();
	//---------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE	
	//---------------------------------------------------------------------------------//
	MIL_ID*                    GetMilPtr() { return &m_MilDigitizer;}
	bool                       CheckMilPtr(MIL_ID *m_MilD);
	//---------------------------------------------------------------------------------//
	bool					   SetupCameraDevice(COAXPRESS_CAMERA_DEVICE CameraDevice, MIL_ID *m_MilD);//設定相機	
	bool					   SetupCameraDevice_VCC_25CXPHSM(MIL_ID *m_MilD);
	bool					   SetupCameraDevice_STC_LBGP251BCXP124(MIL_ID *m_MilD);
	//---------------------------------------------------------------------------------//
	bool                       ResetAllEventCounter();
	//---------------------------------------------------------------------------------//
	bool                       SwitchFreeRunMode(COAXPRESS_CAMERA_DEVICE CameraDevice, MIL_ID *m_MilD);
	bool                       SwitchFreeRunMode_VCC_25CXPHSM(MIL_ID *m_MilD);
	bool                       SwitchFreeRunMode_VC_25MX2_M150I1(MIL_ID *m_MilD);
	bool                       SwitchFreeRunMode_STC_LBGP251BCXP124(MIL_ID *m_MilD);
	//---------------------------------------------------------------------------------//
	bool	                   SwitchSoftwareTriggerMode(COAXPRESS_CAMERA_DEVICE CameraDevice, MIL_ID *m_MilD);
	bool                       SwitchSoftwareTriggerMode_VCC_25CXPHSM(MIL_ID *m_MilD);
	bool                       SwitchSoftwareTriggerMode_VC_25MX2_M150I1(MIL_ID *m_MilD);
	bool                       SwitchSoftwareTriggerMode_STC_LBGP251BCXP124(MIL_ID *m_MilD);
	//---------------------------------------------------------------------------------//
	bool					   SwitchExternalTriggerMode(COAXPRESS_CAMERA_DEVICE CameraDevice, MIL_ID *m_MilD);
	bool                       SwitchExternalTriggerMode_VCC_25CXPHSM(MIL_ID *m_MilD);
	bool                       SwitchExternalTriggerMode_VC_25MX2_M150I1(MIL_ID *m_MilD);
	bool                       SwitchExternalTriggerMode_STC_LBGP251BCXP124(MIL_ID *m_MilD);
	//---------------------------------------------------------------------------------//
	bool executeCameraProperty(MIL_ID *m_MilD, const std::string property);
	bool setCameraProperty(MIL_ID *m_MilD, const std::string property, const std::string& value);
	bool setCameraProperty(MIL_ID *m_MilD, const std::string property, __int64 value);
	bool setCameraProperty(MIL_ID *m_MilD, const std::string property, double value);
	bool getCameraProperty(MIL_ID *m_MilD, const std::string property, std::string* value);
	bool getCameraProperty(MIL_ID *m_MilD, const std::string property, __int64* value);
	bool getCameraProperty(MIL_ID *m_MilD, const std::string property, double* value);

	/*string*/
	static std::wstring to_wide_string(const std::string& input);
	static std::string to_byte_string(const std::wstring& input);

	bool processing(MIL_ID *m_MilD, bool enable);

	bool switchFreeRunMode(MIL_ID *m_MilD);
	bool switchSoftwareTriggerMode(MIL_ID *m_MilD);
	bool switchExternalTriggerMode(MIL_ID *m_MilD);
	bool setTriggerSelector(MIL_ID *m_MilD, int value);
	bool getTriggerSelector(MIL_ID *m_MilD, int* value);
	bool setTriggerMode(MIL_ID *m_MilD, int value);
	bool getTriggerMode(MIL_ID *m_MilD, int* value);
	bool setTriggerSource(MIL_ID *m_MilD, int value);
	bool getTriggerSource(MIL_ID *m_MilD, int* value);
	bool setTriggerActivation(MIL_ID *m_MilD, int value);
	bool getTriggerActivation(MIL_ID *m_MilD, int* value);
	bool setExposureMode(MIL_ID *m_MilD, int value);
	bool getExposureMode(MIL_ID *m_MilD, int* value);
	bool setExposureTime(MIL_ID *m_MilD, int value);
	bool getExposureTime(MIL_ID *m_MilD, int* value);
	bool getExposureTimeMax(MIL_ID *m_MilD, int* value);
	bool getFrameRate(MIL_ID *m_MilD, double* value);
	bool triggerSoftware(MIL_ID *m_MilD);
	bool getFrameOutCount(MIL_ID *m_MilD, int* value);
	bool resetCounter();
	bool cameraReset();
	//---------------------------------------------------------------------------------//
#endif//CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
	bool                       CheckMilPtr();
	//---------------------------------------------------------------------------------//
	COAXPRESS_CAMERA_DEVICE    GetCameraDeviceType() const;//Coaxlink連到相機的樣式
	//---------------------------------------------------------------------------------//
	bool					   StartDevice();
	bool					   StopDevice();
	//---------------------------------------------------------------------------------//
	bool                       ReturnErrorNoCameraDeviceType(COAXPRESS_CAMERA_DEVICE CameraDevice);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CCamera_MatroxRapixoCXP12_Camera();
	virtual ~CCamera_MatroxRapixoCXP12_Camera();
	////---------------------------------------------------------------------------------//
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
#if CAMERA_OBJ_MODE == CAMERA_OBJ_MATROX_RAPIXO_CXP12_CAMERA
extern CCamera_MatroxRapixoCXP12_Camera Camera_MatroxRapixoCXP12_Camera;
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#endif//MATROX_RAPIXO_CXP12_CAMERA_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_CAMERA_MATROXRAPIXOCXP12_CAMERA_H__78B9A893_949C_4BE0_847A_ASA12345__INCLUDED_)