// Camera_GrabLinkFull_CSC6M100BMP11.h: interface for the CCamera_GrabLinkFull_CSC6M100BMP11 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_GRABLINKFULL_CSC6M100BMP11_H__7D32ADA6_6607_4CF4_B073_AA4D7085BB57__INCLUDED_)
#define AFX_CAMERA_GRABLINKFULL_CSC6M100BMP11_H__7D32ADA6_6607_4CF4_B073_AA4D7085BB57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
#include "Camera_Basic.h"
//-------------------------------------------------------------------------------------//
#ifdef GRABLINKFULL_CSC6M100BMP11_P100_USE
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE	
	#include "JetSerial.h"   // RS232C application program
	#include "RS232.h"   // RS232C application program
	#include "..\\JET8000_Library\\Multicam\\6_8_0_1925\\Include\\multicam.h"
	#ifdef _X64
		#pragma comment(lib, "..\\JET8000_Library\\Multicam\\6_8_0_1925\\x64\\Lib\\multicam.lib")
	#else
		#pragma comment(lib, "..\\JET8000_Library\\Multicam\\6_8_0_1925\\x32\\Lib\\multicam.lib")
	#endif//_X64
#endif

#ifndef CAMERA_OBJ_DISABLE
	void WINAPI GrabLinkFull_Callback(PMCSIGNALINFO SigInfo);
	void WINAPI GrabLinkFull_FrameCallback(PMCSIGNALINFO SigInfo);
	void WINAPI GrabLinkFull_ExposureEndCallback(PMCSIGNALINFO SigInfo);
	void WINAPI GrabLinkFull_ExceptionCallback(PMCSIGNALINFO SigInfo);
#endif//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
class CCamera_GrabLinkFull_CSC6M100BMP11 : public CCamera_Basic  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CCamera_GrabLinkFull_CSC6M100BMP11)
	//---------------------------------------------------------------------------------//	
private:	
	//---------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE
	int                        m_SerialPort;                         //連到實體相機的虛擬RS232 
	CJetSerial                 m_CameraSerial;                       //連到實體相機的虛擬RS232
	//CRS232                     m_CameraSerial;                       //連到實體相機的虛擬RS232 
	MCHANDLE                   m_CameraChannel;                      //Euresys Grablink Full	
#endif//endif CAMERA_OBJ_DISABLE
	//---------------------------------------------------------------------------------//
protected:	
	//---------------------------------------------------------------------------------//
	virtual void               PreInitCamera();	
	//---------------------------------------------------------------------------------//
	bool                       DoGrabLinkFullStartGrab();
	//------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE
	bool                       GetGrabLinkFullErrorString(MCSTATUS Status, CString &err);	

	bool                       RS232_ConnectCamera(int Port);//相機連線
	bool                       RS232_SendExpTime(int exp);//設定曝光時間
	bool                       RS232_WriteData(LPCSTR Command, int value);//寫入相機資料
	bool                       RS232_ReadData(LPCSTR Command, CString &ReturnStr);//讀取相機資料	
	bool                       RS232_CheckResposed();//確認相機回傳碼
	bool                       RS232_SendShutterMode(int value);
	bool                       RS232_SendRandomTriggerMode(int value);
	bool                       RS232_SendTriggerPolarityMode(int value);
	bool                       RS232_SendTriggerSourceMode(int value);

#endif//endif CAMERA_OBJ_DISABLE
	//------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CCamera_GrabLinkFull_CSC6M100BMP11();
	virtual ~CCamera_GrabLinkFull_CSC6M100BMP11();
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
extern CCamera_GrabLinkFull_CSC6M100BMP11 Camera_GrabLinkFull_CSC6M100BMP11;
//-------------------------------------------------------------------------------------//
#endif//GRABLINKFULL_CSC6M100BMP11_P100_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_CAMERA_GRABLINKFULL_CSC6M100BMP11_H__7D32ADA6_6607_4CF4_B073_AA4D7085BB57__INCLUDED_)
