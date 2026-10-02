// Camera_MatroxRapixoCXP6_Camera.cpp: implementation of the CCamera_MatroxRapixoCXP6_Camera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Camera_MatroxRapixoCXP6_Camera.h"
#include <codecvt>
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef MATROX_RAPIXO_CXP6_CAMERA_USE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_MATROX_RAPIXO_CXP6_CAMERA
CCamera_MatroxRapixoCXP6_Camera Camera_MatroxRapixoCXP6_Camera;
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//	
#ifndef CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
inline MIL_INT WINAPI ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
{
	CCamera_MatroxRapixoCXP6_Camera *This =reinterpret_cast<CCamera_MatroxRapixoCXP6_Camera*>(HookDataPtr);
	if (NULL == This) { return 0; }
	This->IncrementCountForCameraExposuredEnd();
	This->IncrementCountForCameraCallback();
	This->CheckCameraExposuredEndEvent();

	//HookDataStruct *UserHookDataPtr = (HookDataStruct *)HookDataPtr;
	MIL_ID ModifiedBufferId;

	/* Retrieve the MIL_ID of the grabbed buffer. */
	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER + M_BUFFER_ID, &ModifiedBufferId);

	////MdigInquire(UserHookDataPtr->MilDigitizer, M_PROCESS_FRAME_RATE, &imgData.frameRate);

	unsigned char *ImgPtr = nullptr;
	MbufInquire(ModifiedBufferId, M_HOST_ADDRESS, &ImgPtr);
	if ( This->AddCameraRingBufferList(ImgPtr) == true )
	{
	}
	return 0;
}
//-------------------------------------------------------------------------------------//
#endif //CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CCamera_MatroxRapixoCXP6_Camera, CCamera_Basic)
//-------------------------------------------------------------------------------------//
CCamera_MatroxRapixoCXP6_Camera::CCamera_MatroxRapixoCXP6_Camera():CCamera_Basic()
{
	this->PreInitCamera();
	SetCameraConstructed(true);
}
//-------------------------------------------------------------------------------------//	
CCamera_MatroxRapixoCXP6_Camera::~CCamera_MatroxRapixoCXP6_Camera()
{
	this->ReleaseCamera();
}
//-------------------------------------------------------------------------------------//	
void CCamera_MatroxRapixoCXP6_Camera::PreInitCamera()
{	
	this->m_CameraModelID = CAMERA_OBJ_MATROX_RAPIXO_CXP6_CAMERA;
	this->m_CameraModelName = _T("MATROXRAPIXOCXP6_CAMERA");
	this->m_CameraDeviceType = COAXPRESS_CAMERA_STC_CMB120ACXP;//目前僅啟用已測試的CXP6相機

	const int PaddingTime_us = 1000;
	//this->m_PeriodTim_us = 5600;//5.6ms for Gen3
	this->m_PeriodTim_us = 7500;//7.5ms for Gen2	

	this->m_ExposureTime_us = 3000;//3ms
	this->m_ExposureTimeMin_us = 20;//曝光時間-Min
	this->m_ExposureTimeMax_us = 1000000;//曝光時間-Max
	this->m_TriggerDelay = 500;//處發延遲時間-us

	this->m_CameraFPS = 181.0;//181.0;
	this->m_CameraBitCount = 8;	
	this->m_CameraImageW = 4096;//相機影像寬度
	this->m_CameraImageH = 3072;//相機影像高度
	this->m_CameraImageStep = 4096;//相機間距
	this->m_CameraSizeRaw = m_CameraImageW*m_CameraImageH;
	this->m_CameraSizeColor = this->m_CameraSizeRaw*3;	

	this->m_FVersion = 0;	//韌體版本 (不同的韌體版本會有不同的參數設定)
	this->m_CameraBitMode = 8;//8Bit Mode
#ifndef CAMERA_OBJ_DISABLE	
	this->m_MilApplication = M_NULL;
	this->m_MilSystem = M_NULL;
	this->m_MilDigitizer = M_NULL;
	//this->m_MilImage = M_NULL;
#endif//CAMERA_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//	
bool CCamera_MatroxRapixoCXP6_Camera::InitialCamera()
{
	const char fnName[] = "CCamera_MatroxRapixoCXP6_Camera::InitialCamera";

	CCamera_Basic::SaveCameraProcess(_T("InitialCamera"), MSG_LEVEL_HIGH);
	
	this->ReleaseCamera();
	if ( CCamera_MatroxRapixoCXP6_Camera::LoadCameraINIFile() == false ) { return false; }
	if ( CCamera_MatroxRapixoCXP6_Camera::SaveCameraINIFile() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	try
	{
		COAXPRESS_CAMERA_DEVICE CameraDeviceType=GetCameraDeviceType();
		MappAlloc(M_DEFAULT, &m_MilApplication);
		if (m_MilApplication == M_NULL)
		{
			m_ErrorString = "Error, Not Found Matrox Applocation.";
			ReleaseMatroxObj();
			return false;
		}
		MsysAlloc(M_SYSTEM_DEFAULT, M_DEV0, M_DEFAULT, &m_MilSystem);
		if (m_MilSystem == M_NULL)
		{
			m_ErrorString = "Error, Not Found Matrox Capture Card.";
			ReleaseMatroxObj();
			return false;
		}

		MIL_INT SystemType = 0;
		MsysInquire(m_MilSystem, M_SYSTEM_TYPE, &SystemType);

		if (!(SystemType == M_SYSTEM_RADIENTCXP_TYPE ||
			SystemType == M_SYSTEM_RADIENTPRO_TYPE ||
			SystemType == M_SYSTEM_RADIENTCLHS_TYPE ||
			SystemType == M_SYSTEM_RADIENTEVCL_TYPE ||
			SystemType == M_SYSTEM_RAPIXOCL_TYPE ||
			SystemType == M_SYSTEM_RAPIXOCXP_TYPE))
		{
			m_ErrorString = "Wrong Matrox Capture Card Type.";
		}

		long boardType = MsysInquire(m_MilSystem, M_BOARD_TYPE, M_NULL);
		MdigAlloc(m_MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &m_MilDigitizer);
		//LocalBufferAllocDefault(&m_MilSystem, M_NULL/*&m_MilDisplay*/, &m_MilDigitizer, &m_MilImage);


		MdigInquireFeature(m_MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceVendorName"), M_TYPE_STRING, m_CameraVendor);
		MdigInquireFeature(m_MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, m_CameraModel);


		//connect
		if ( SetupCameraDevice(CameraDeviceType, &m_MilDigitizer) == false)
		{
			//delete &m_MilDigitizer; m_MilDigitizer=NULL;
			ReleaseMatroxObj();
			return false; 
		}

		/*if ( SwitchSoftwareTriggerMode(CameraDeviceType, &m_MilDigitizer) == false )
		{
			delete &m_MilDigitizer; m_MilDigitizer=NULL;
			ReleaseMatroxObj();
			return false; 
		}*/
		
		if ( SwitchExternalTriggerMode(CameraDeviceType, &m_MilDigitizer) == false)
		{
			//delete &m_MilDigitizer; m_MilDigitizer=NULL;
			ReleaseMatroxObj();
			return false; 
		}
		CCamera_MatroxRapixoCXP6_Camera::SaveCameraINIFile();
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s") , str);
		//delete &m_MilDigitizer; m_MilDigitizer=NULL;
		ReleaseMatroxObj();
		return false;
	}
	this->CreateCameraGrabFinishEvent();
	this->CreateCameraExposureFinishEvent();

	JetMemory.free_func(m_ClonedRawImagePtr);
	JetMemory.free_func(m_ClonedColorImagePtr);

	const size_t ImageWH  = this->GetImageRawSize();	 
	JetMemory.alloc_func(ImageWH, m_ClonedRawImagePtr, fnName, "m_ClonedRawImagePtr");	
//	JetMemory.alloc_func(ImageWH*4, m_ClonedColorImagePtr, fnName, "m_ClonedColorImagePtr");
	//if ( m_ClonedRawImagePtr==NULL || m_ClonedColorImagePtr==NULL )
	if ( m_ClonedRawImagePtr==NULL )
	{
		JetMemory.free_func(m_ClonedRawImagePtr);
		JetMemory.free_func(m_ClonedColorImagePtr);
		ReleaseMatroxObj();
		return false;
	}
	::memset(m_ClonedRawImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH);


#endif//CAMERA_OBJ_DISABLE
	SetCameraInited(true);	
	SetCameraImageMode(CAMERA_IMAGE_GRAY);
	SetCameraBayerPattern(BAYER_PATTERN_NONE);	
	SetCameraGrabbing_Unlock(false);	
	CreateBMPInfo(m_pColorInfo, true);
	CreateBMPInfo(m_pMonoInfo, false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::ReleaseCamera()//相機釋放
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ReleaseCamera"), MSG_LEVEL_HIGH);	
#ifndef CAMERA_OBJ_DISABLE
	StopDevice();
	ReleaseMatroxObj();

	if (this->m_pColorInfo != NULL)
	{
		delete[] m_pColorInfo; m_pColorInfo = NULL;
	}
	if (this->m_pMonoInfo != NULL)
	{
		delete[] m_pMonoInfo; m_pMonoInfo = NULL;
	}

	if (NULL != m_ClonedRawImagePtr)
	{
		JetMemory.free_func(m_ClonedRawImagePtr);
	}

	if (NULL != m_ClonedColorImagePtr)
	{
		JetMemory.free_func(m_ClonedColorImagePtr);
	}

	Sleep(5000);//等待相機完全釋放
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::ReleaseMatroxObj()
{
	for (int i = 0; i < MilGrabBufferListMaxSize; i++)
	{
		if (m_MilGrabBufferList[i])
		{
			MbufFree(m_MilGrabBufferList[i]); m_MilGrabBufferList[i] = M_NULL;
		}
	}

	//Free the digitizer [CALL TO MIL]
	if (m_MilDigitizer)
	{
		MdigFree(m_MilDigitizer);	m_MilDigitizer = M_NULL;
	}

	//Free the system [CALL TO MIL]
	if (m_MilSystem)
	{
		MsysFree(m_MilSystem);		m_MilSystem = M_NULL;
	}

	if (m_MilApplication)
	{
		// Enable the typical MIL error message display[CALL TO MIL]
		//MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);

		//// Unhook MIL error on function DisplayError() [CALL TO MIL]
		//if (m_isCurrentlyHookedOnErrors)
		//{
		//	MappHookFunction(M_DEFAULT, M_ERROR_CURRENT + M_UNHOOK, DisplayErrorExt, this);
		//	m_isCurrentlyHookedOnErrors = false;
		//}

		// Free the application [CALL TO MIL]
		MappFree(m_MilApplication);	m_MilApplication = M_NULL;
		
	}
#endif//CAMERA_OBJ_DISABLE
	SetCameraInited(false);
	return false;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::ResetCamera()//復歸相機
{	
	SetCameraGrabbing_Unlock(false);	
	if (StopDevice() == false) { return false; }
	::Sleep(3000);
	if ( StartDevice() == false ) { return false; }
	::Sleep(3000);
#ifndef CAMERA_OBJ_DISABLE
	//CEGrabber *pGrabber=GetGrabberPtr();
	//COAXPRESS_CAMERA_DEVICE CameraDeviceType=GetCameraDeviceType();
	//if ( SwitchExternalTriggerMode(CameraDeviceType, pGrabber) == false )	{ return false; }
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CCamera_MatroxRapixoCXP6_Camera::ReturnErrorNoCameraDeviceType(COAXPRESS_CAMERA_DEVICE CameraDevice)
{
	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera Camera Device Type Fault [%d]") , CameraDevice);
	return false;
}
//-------------------------------------------------------------------------------------//
inline bool CCamera_MatroxRapixoCXP6_Camera::CheckMilPtr()
{
#ifndef CAMERA_OBJ_DISABLE
	if ( M_NULL == m_MilDigitizer )
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera MilD is NULL"));
		return false;
	}
#endif // !CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
COAXPRESS_CAMERA_DEVICE CCamera_MatroxRapixoCXP6_Camera::GetCameraDeviceType() const//Coaxlink連到相機的樣式
{
	return m_CameraDeviceType;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::StartDevice()					//相機開始運轉
{
	COAXPRESS_CAMERA_DEVICE CameraDeviceType = GetCameraDeviceType();
#ifndef CAMERA_OBJ_DISABLE
	MIL_ID* MilPtr = GetMilPtr();
	if (false == CheckMilPtr(MilPtr)) { return false; }
	//MdigControl(*MilPtr, M_GRAB_ABORT, M_DEFAULT);
	//if (false == StartDevice_Setting(MilPtr)) { return false; }
	if (false == processing(MilPtr, true))
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera Start device failed"));
		return false;
	}
#endif // CAMERA_OBJ_DISABLE
	SetCameraGrabbing_Unlock(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::StartDevice_Setting(MIL_ID *m_MilD)
{
	COAXPRESS_CAMERA_DEVICE CameraDeviceType = GetCameraDeviceType();
	MIL_INT nValue;
	if (CheckMilPtr(m_MilD) == false) { return false; }
	switch (CameraDeviceType)
	{
	case COAXPRESS_CAMERA_STC_CMB120ACXP:
		MdigInquire(*m_MilD, M_TIMER_STATE + M_TIMER1, &nValue);
		if (M_ENABLE == nValue) { return true; }
		// Sets the state of the specified timer.
		// Specifies that the timer is disabled.
		MdigControl(*m_MilD, M_TIMER_STATE + M_TIMER1, M_ENABLE);
		MdigControl(*m_MilD, M_TIMER_STATE + M_TIMER2, M_ENABLE);
		break;
	default:
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::StopDevice()						//相機停止運轉
{
#ifndef CAMERA_OBJ_DISABLE
	MIL_ID* MilPtr = GetMilPtr();
	if (false == CheckMilPtr(MilPtr)) { return false; }
	//if (false == StopDevice_Setting(MilPtr)) { return false; }
	if (false == processing(MilPtr, false))
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera Stop device failed"));
		return false;
	}
#endif // CAMERA_OBJ_DISABLE
	SetCameraGrabbing_Unlock(false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::StopDevice_Setting(MIL_ID * m_MilD)
{
	COAXPRESS_CAMERA_DEVICE CameraDeviceType = GetCameraDeviceType();
	MIL_INT nValue;
	if (CheckMilPtr(m_MilD) == false) { return false; }
	switch (CameraDeviceType)
	{
	case COAXPRESS_CAMERA_STC_CMB120ACXP:
		MdigInquire(*m_MilD, M_TIMER_STATE + M_TIMER1, &nValue);
		if (M_DISABLE == nValue) { return true; }
		// Sets the state of the specified timer.
		// Specifies that the timer is disabled.
		MdigControl(*m_MilD, M_TIMER_STATE + M_TIMER1, M_DISABLE);
		MdigControl(*m_MilD, M_TIMER_STATE + M_TIMER2, M_DISABLE);
		break;
	default:
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE			
	if ( m_CameraGrabMode == Mode ) 
	{	return true; }
	
	COAXPRESS_CAMERA_DEVICE CameraDeviceType=GetCameraDeviceType();
	
	if (CheckMilPtr(&m_MilDigitizer) == false){return false;}

	//if ( this->StopCameraGrab() == false ) { return false; }	
	switch ( Mode )
	{
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		if ( this->SwitchExternalTriggerMode(CameraDeviceType, &m_MilDigitizer) == false )
		{	return false; }		
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		if ( this->SwitchSoftwareTriggerMode(CameraDeviceType, &m_MilDigitizer) == false )
		{	return false; }		
		break;
	default://CAMERA_GRAB_FREE_RUN
		if ( this->SwitchFreeRunMode(CameraDeviceType, &m_MilDigitizer) == false )
		{	return false; }		
		break;
	}
	SetCameraGrabMode(Mode);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SetExposureTime(const int ExposureTime)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	if ( m_ExposureTime_us == ExposureTime ) 
	{	return true; }
	
	try
	{	
		//get maximum exporsure time
		int ExpMax = 3000;
		if (this->getExposureTimeMax(&m_MilDigitizer, &ExpMax) == false)
		{
			this->m_ErrorString.Format(_T("%s"), _T("Error, Get ExposureTimeMax False"));
			return false;
		}
		if (ExposureTime > ExpMax || ExposureTime <= 0)
		{
			this->m_ErrorString.Format(_T("%s,%d,%d"), _T("Error, ExposureTime Greater Than ExpMax"), ExposureTime, ExpMax);
			return false;
		}
		if (this->setExposureTime(&m_MilDigitizer, ExposureTime) == false)
		{
			this->m_ErrorString.Format(_T("%s"), _T("Error, Set ExposureTime False"));

			return false;
		}
		int exTime;
		if (this->getExposureTime(&m_MilDigitizer, &exTime) == false)
		{
			this->m_ErrorString.Format(_T("%s"), _T("Error, Get ExposureTime False"));
			return false;
		}
	
		m_ExposureTime_us = ExposureTime;
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::StartCameraGrab()//只取一張影像-內部觸發用
{
	CCamera_Basic::SaveCameraProcess(_T("StartCameraGrab"), MSG_LEVEL_HIGH);

	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();
	if ( this->StartDevice() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::StartCameraLiveGrab()//連續取多張影像-外部觸發用
{
	CCamera_Basic::SaveCameraProcess(_T("StartCameraLiveGrab"), MSG_LEVEL_HIGH);

	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();

	if ( this->StartDevice() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::StopCameraGrab()//停止取像
{
#ifndef CAMERA_OBJ_DISABLE	
	try
	{		
		CCamera_Basic::SaveCameraProcess(_T("StopCameraGrab"), MSG_LEVEL_HIGH);
		this->StopDevice();
		//SetCameraGrabbing_Unlock(false);
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::FireSoftwareTrigger()//發射軟體觸發訊號
{
#ifndef CAMERA_OBJ_DISABLE	
	try
	{	
		//if (m_TriggerMode != CAMERA_DEVICE_TRIGGER_MODE_SOFTWARE)
		//{
		//	strm << __FUNCTION__ << ":: not software trigger mode now.";
		//	m_errstr = strm.str();
		//	return false;
		//}
		if (triggerSoftware(&m_MilDigitizer) == false)
		{
			return false;
		}
		return true;	
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::WaitforCameraReadytoTrigger()//等待相機準備好可以觸發
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("WaitforCameraReadytoTrigger"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::ResetCameraReadyTriggerEvent()//復歸相機準備好了的事件
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("ResetCameraReadyTriggerEvent"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::DoCameraDebayer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const unsigned char *pRaw, unsigned char *pResult, BAYER_PATTERN_MODE BayerPattern)
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("DoCameraDebayer"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::GetWhiteBalanceParams(double &WBR, double &WBG, double &WBB)
{
#ifndef CAMERA_OBJ_DISABLE	
	WBR = 1.0;
	WBG = 1.0;
	WBB = 1.0;
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SetWhiteBalanceParams(double WBR, double WBG, double WBB)
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("SetWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::CalcWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("CalcWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::ResetWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("ResetWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::WriteCameraParameterToDevice()//儲存目前相機的參數至相機內部的韌體上
{
#ifndef CAMERA_OBJ_DISABLE	
	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SaveCameraINIFile()
{
	if ( CCamera_Basic::SaveCameraINIFile() == false ) { return false; }

	bool    IsOK = true;
	CString KeyName;
	CString KeyString;
	CString Section = CCamera_Basic::GetCameraSectionName();
	CString FileName = CCamera_Basic::GetCameraINIFileName();

	KeyName.Format(_T("Card Serial Number")); KeyString.Format(_T("%s"), m_CardSerialNumber);
	if (SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Camera Device Type")); KeyString.Format(_T("%d"), m_CameraDeviceType);
	if (SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Camera Bit Mode")); KeyString.Format(_T("%d"), m_CameraBitMode);
	if (SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::LoadCameraINIFile()
{
	if ( CCamera_Basic::LoadCameraINIFile() == false ) { return false; }

	this->SetCameraModelID(CAMERA_OBJ_MATROX_RAPIXO_CXP6_CAMERA);	

	CString KeyName;
	CString KeyString;
	CString Section = CCamera_Basic::GetCameraSectionName();
	CString FileName = CCamera_Basic::GetCameraINIFileName();
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];	

 	KeyName.Format(_T("Card Serial Number")); KeyString.Format(_T("%s"), m_CardSerialNumber);
	if (LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_CardSerialNumber = ReturnString;	}
	
	KeyName.Format(_T("Camera Device Type")); KeyString.Format(_T("%d"), m_CameraDeviceType);
	if (LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	
		int CameraDevice = ::_ttoi(ReturnString);	
		switch ( CameraDevice )
		{
		//CXP12移植遺留設定，目前不啟用
		//case COAXPRESS_CAMERA_Q_12A180F:
		//case COAXPRESS_CAMERA_VC_12MX_M180:
		//case COAXPRESS_CAMERA_VC_12MX_M180_HOR:
		case COAXPRESS_CAMERA_STC_CMB120ACXP:
		case COAXPRESS_CAMERA_STC_LBGP251BCXP124:
			m_CameraDeviceType=(COAXPRESS_CAMERA_DEVICE)(CameraDevice);
			break;
		default:
			m_CameraDeviceType = COAXPRESS_CAMERA_STC_CMB120ACXP;
			break;
		}
	}

	KeyName.Format(_T("Camera Bit Mode")); KeyString.Format(_T("%d"), m_CameraBitMode);
	if (LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	
		int CameraBitMode = ::_ttoi(ReturnString);	
		switch ( CameraBitMode )
		{
		case 8:		m_CameraBitMode = 8;	break;
		case 10:	m_CameraBitMode = 10;	break;		
		default:	m_CameraBitMode = 8;	break;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE
bool CCamera_MatroxRapixoCXP6_Camera::CheckMilPtr(MIL_ID *m_MilD)
{
#ifndef CAMERA_OBJ_DISABLE
	//if ( M_NULL == m_MilD )
	//{
	//	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera MilD is NULL"));
	//	return false;
	//}
	if (M_NULL == *m_MilD)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera MilD is NULL"));
		return false;
	}
#endif // !CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SetupCameraDevice(COAXPRESS_CAMERA_DEVICE CameraDevice, MIL_ID *m_MilD)//設定相機
{
	bool bIsOK = true;
	switch ( CameraDevice )
	{
	//CXP12移植遺留設定，目前不啟用
	//case COAXPRESS_CAMERA_Q_12A180F:
	//case COAXPRESS_CAMERA_VC_12MX_M180:
	//case COAXPRESS_CAMERA_VC_12MX_M180_HOR:
	//	 bIsOK=SetupCameraDevice_VCC_25CXPHSM(m_MilD); break;
	case COAXPRESS_CAMERA_STC_CMB120ACXP:  bIsOK=SetupCameraDevice_STC_CMB120ACXP(m_MilD); break;
	case COAXPRESS_CAMERA_STC_LBGP251BCXP124: bIsOK = SetupCameraDevice_STC_LBGP251BCXP124(m_MilD); break;
	default:						      bIsOK=ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SetupCameraDevice_VCC_25CXPHSM(MIL_ID* m_MilD)
{
	try
	{
		int bufCount = (m_BufferCount < MilGrabBufferListMaxSize) ? m_BufferCount : MilGrabBufferListMaxSize;

		MappControl(M_ERROR, M_PRINT_DISABLE);

		// 先設定連線模式
		MdigControlFeature(m_MilDigitizer, M_FEATURE_VALUE,
			MIL_TEXT("ConnectionConfig"),
			M_TYPE_STRING,
			MIL_TEXT("CXP6_X4"));

		MIL_INT NonPagedMemoryTotalSize = 0;
		MIL_INT NonPagedMemoryUsed = 0;

		MappInquire(M_NON_PAGED_MEMORY_SIZE, &NonPagedMemoryTotalSize);

		MIL_INT CameraSizeW = MdigInquire(m_MilDigitizer, M_SIZE_X, M_NULL);
		MIL_INT CameraSizeH = MdigInquire(m_MilDigitizer, M_SIZE_Y, M_NULL);

		m_MilGrabBufferListSize = 0;

		for (int i = 0; i < bufCount; i++)
		{
			m_MilGrabBufferList[i] = M_NULL;

			MbufAlloc2d(m_MilSystem,
				CameraSizeW,
				CameraSizeH,
				8L + M_UNSIGNED,
				M_IMAGE + M_GRAB,
				&m_MilGrabBufferList[i]);

			if (m_MilGrabBufferList[i] == M_NULL)
			{
				break;
			}

			MbufClear(m_MilGrabBufferList[i], 0xFF);
			m_MilGrabBufferListSize++;

			MappInquire(M_NON_PAGED_MEMORY_USED, &NonPagedMemoryUsed);

			double ratio = 0.0;
			if (NonPagedMemoryTotalSize > 0)
				ratio = (double)NonPagedMemoryUsed / (double)NonPagedMemoryTotalSize;

			if (ratio > 0.8)
			{
				break;
			}
		}

		//CString dbg;
		//dbg.Format(_T("Requested=%d, Allocated=%d, NonPagedUsed=%lld MB / %lld MB"),
		//	bufCount,
		//	m_MilGrabBufferListSize,
		//	(long long)NonPagedMemoryUsed,
		//	(long long)NonPagedMemoryTotalSize);
		//AfxMessageBox(dbg);

		unsigned int ImageSizeW = this->GetCameraImageW();
		unsigned int ImageSizeH = this->GetCameraImageH();

		if (CameraSizeW != ImageSizeW || CameraSizeH != ImageSizeH)
		{
			this->m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera Camera Size Exception(%d, %d)"),
				(int)CameraSizeW, (int)CameraSizeH);
			return false;
		}

		double CameraFPS = 150;
		getFrameRate(m_MilD, &CameraFPS);
		CCamera_Basic::m_CameraFPS = CameraFPS;
	}
	catch (std::exception& e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s"), str);
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SetupCameraDevice_STC_CMB120ACXP(MIL_ID * m_MilD)
{
	//if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		// matrox 卡搭配此相機需外部供電，若是開啟 PoCXP ，會導致相機運行不穩定。
		// Sets whether to enable PoCL (power over Camera Link) and PoCXP (power over CoaXPress).
		// Specifies to enable PoCL/PoCXP.
		MdigControl(*m_MilD, M_POWER_OVER_CABLE, M_OFF);

		int bufCount = (m_BufferCount < MilGrabBufferListMaxSize) ? m_BufferCount : MilGrabBufferListMaxSize;

		/* Allocate the grab buffers and clear them. */
		MappControl(M_ERROR, M_PRINT_DISABLE);
		MIL_INT NonPagedMemoryTotalSize = 0;
		MIL_INT NonPagedMemoryUsed = 0;
		MappInquire(M_NON_PAGED_MEMORY_SIZE, &NonPagedMemoryTotalSize);
		int64_t CameraSizeW = MdigInquire(m_MilDigitizer, M_SIZE_X, M_NULL);
		int64_t CameraSizeH = MdigInquire(m_MilDigitizer, M_SIZE_Y, M_NULL);

		for (m_MilGrabBufferListSize = 0; m_MilGrabBufferListSize < bufCount; m_MilGrabBufferListSize++)
		{
			MbufAlloc2d(m_MilSystem,
				CameraSizeW,
				CameraSizeH,
				8L + M_UNSIGNED,
				M_IMAGE + M_GRAB,
				&m_MilGrabBufferList[m_MilGrabBufferListSize]);

			if (m_MilGrabBufferList[m_MilGrabBufferListSize])
				MbufClear(m_MilGrabBufferList[m_MilGrabBufferListSize], 0xFF);

			// Leave about 80% of free non paged memory for temporary buffer allocations.
			MappInquire(M_NON_PAGED_MEMORY_USED, &NonPagedMemoryUsed);
			if (((double)NonPagedMemoryUsed / (double)NonPagedMemoryTotalSize) > 0.8)
			{
				m_MilGrabBufferListSize++;
				break;
			}
		}

		unsigned int ImageSizeW = this->GetCameraImageW();
		unsigned int ImageSizeH = this->GetCameraImageH();
		if (CameraSizeW != ImageSizeW || CameraSizeH != ImageSizeH)
		{
			this->m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera Camea Size Exception(%d, %d)"), CameraSizeW, CameraSizeH);
			return false;
		}

		double CameraFPS = 150; //取回相機取像速度-未支援, 以相機規格為主
		getFrameRate(m_MilD, &CameraFPS);
		CCamera_Basic::m_CameraFPS = CameraFPS;
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SetupCameraDevice_STC_LBGP251BCXP124(MIL_ID * m_MilD)
{
	//if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		int bufCount = (m_BufferCount < MilGrabBufferListMaxSize) ? m_BufferCount : MilGrabBufferListMaxSize;

		// 降速
		// The following code can be used to control feature values.
		MdigControlFeature(m_MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("CxpLinkConfiguration"), M_TYPE_STRING, MIL_TEXT("CXP6_X4"));

		/* Allocate the grab buffers and clear them. */
		MappControl(M_ERROR, M_PRINT_DISABLE);
		MIL_INT NonPagedMemoryTotalSize = 0;
		MIL_INT NonPagedMemoryUsed = 0;
		MappInquire(M_NON_PAGED_MEMORY_SIZE, &NonPagedMemoryTotalSize);
		int64_t CameraSizeW = MdigInquire(m_MilDigitizer, M_SIZE_X, M_NULL);
		int64_t CameraSizeH = MdigInquire(m_MilDigitizer, M_SIZE_Y, M_NULL);

		for (m_MilGrabBufferListSize = 0; m_MilGrabBufferListSize < bufCount; m_MilGrabBufferListSize++)
		{
			MbufAlloc2d(m_MilSystem,
				CameraSizeW,
				CameraSizeH,
				8L + M_UNSIGNED,
				M_IMAGE + M_GRAB,
				&m_MilGrabBufferList[m_MilGrabBufferListSize]);

			if (m_MilGrabBufferList[m_MilGrabBufferListSize])
				MbufClear(m_MilGrabBufferList[m_MilGrabBufferListSize], 0xFF);

			// Leave about 80% of free non paged memory for temporary buffer allocations.
			MappInquire(M_NON_PAGED_MEMORY_USED, &NonPagedMemoryUsed);
			if (((double)NonPagedMemoryUsed / (double)NonPagedMemoryTotalSize) > 0.8)
			{
				m_MilGrabBufferListSize++;
				break;
			}
		}

		unsigned int ImageSizeW = this->GetCameraImageW();
		unsigned int ImageSizeH = this->GetCameraImageH();
		if (CameraSizeW != ImageSizeW || CameraSizeH != ImageSizeH)
		{
			this->m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera Camea Size Exception(%d, %d)"), CameraSizeW, CameraSizeH);
			return false;
		}


		double CameraFPS = 150; //取回相機取像速度-未支援, 以相機規格為主
		getFrameRate(m_MilD, &CameraFPS);
		CCamera_Basic::m_CameraFPS = CameraFPS;
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::ResetAllEventCounter()	//重置擷取卡與相機所有的計數器
{
	if (resetCounter() == false)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera ResetAllEventCounter"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchFreeRunMode(COAXPRESS_CAMERA_DEVICE CameraDevice, MIL_ID *m_MilD)
{
	bool bIsOK = true;
	switch ( CameraDevice )
	{
	//CXP12移植遺留設定，目前不啟用
	//case COAXPRESS_CAMERA_Q_12A180F:
	//case COAXPRESS_CAMERA_VC_12MX_M180:
	//case COAXPRESS_CAMERA_VC_12MX_M180_HOR:
	//	 bIsOK= SwitchFreeRunMode_VCC_25CXPHSM(m_MilD); break;
	case COAXPRESS_CAMERA_STC_CMB120ACXP: bIsOK= SwitchFreeRunMode_STC_CMB120ACXP(m_MilD); break;
	default:						   bIsOK=ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	if ( true == bIsOK )
	{	SetCameraGrabMode(CAMERA_GRAB_FREE_RUN); }
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchFreeRunMode_VCC_25CXPHSM(MIL_ID *m_MilD)
{
	if (CheckMilPtr(m_MilD) == false){return false;}

	if (StopDevice() == false)	//停止相機
	{
		return false;
	}
	
		//trigger selector
	int value = eTriggerSelector_FrameStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_Off;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerSource_LinkTrigger0;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	value = eTriggerMode_LevelLow;
	if (!setTriggerActivation(m_MilD, value)) { return false; }
	if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Previous version mark " NotSupport" 
	//Exposure mode
	//value = eExposureMode_TriggerWidth;
	//if (!setExposureMode(m_MilD, value)) { return false; }
	//if (!getExposureMode(m_MilD, &value)) { return false; }
	//if (switchFreeRunMode(m_MilD) == false)
	//{
	//	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera switchFreeRunMode"));
	//	return false;
	//}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchFreeRunMode_VC_25MX2_M150I1(MIL_ID *m_MilD)
{
	if (CheckMilPtr(m_MilD) == false){return false;}

	if (StopDevice() == false)	//停止相機
	{
		return false;
	}
	
	//trigger selector
	int value = eTriggerSelector_ExposureStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_Off;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerSource_LinkTrigger0;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	value = eTriggerMode_FallingEdge;
	if (!setTriggerActivation(m_MilD, value)) { return false; }
	if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Previous version mark " NotSupport" 
	//Exposure mode
	//value = eExposureMode_TriggerWidth;
	//if (!setExposureMode(m_MilD, value)) { return false; }
	//if (!getExposureMode(m_MilD, &value)) { return false; }
	//if (switchFreeRunMode(m_MilD) == false)
	//{
	//	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera switchFreeRunMode"));
	//	return false;
	//}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchFreeRunMode_STC_CMB120ACXP(MIL_ID * m_MilD)
{
	if (CheckMilPtr(m_MilD) == false) { return false; }

	if (StopDevice() == false)	//停止相機
	{	return false;	}
	
	//trigger selector
	int value = eTriggerSelector_ExposureStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_Off;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerSource_LinkTrigger0;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	//value = eTriggerMode_FallingEdge;
	//if (!setTriggerActivation(m_MilD, value)) { return false; }
	//if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Exposure mode
	value = eExposureMode_TriggerWidth;
	if (!setExposureMode(m_MilD, value)) { return false; }
	if (!getExposureMode(m_MilD, &value)) { return false; }
	//if (switchFreeRunMode(m_MilD) == false)
	//{
	//	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera switchFreeRunMode"));
	//	return false;
	//}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchSoftwareTriggerMode(COAXPRESS_CAMERA_DEVICE CameraDevice, MIL_ID *m_MilD)
{
	bool bIsOK = true;
	switch ( CameraDevice )
	{
	//CXP12移植遺留設定，目前不啟用
	//case COAXPRESS_CAMERA_Q_12A180F:
	//case COAXPRESS_CAMERA_VC_12MX_M180:
	//case COAXPRESS_CAMERA_VC_12MX_M180_HOR:
	//	 bIsOK= SwitchSoftwareTriggerMode_VCC_25CXPHSM(m_MilD); break;
	case COAXPRESS_CAMERA_STC_CMB120ACXP: bIsOK = SwitchSoftwareTriggerMode_STC_CMB120ACXP(m_MilD); break;
	default:						      bIsOK=ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	if ( true == bIsOK )
	{	SetCameraGrabMode(CAMERA_GRAB_SOFTWARE_TRIGGER); }
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchSoftwareTriggerMode_VCC_25CXPHSM(MIL_ID *m_MilD)
{
	if (CheckMilPtr(m_MilD) == false) { return false; }

	if (StopDevice() == false)	//停止相機
	{	return false;	}

	//trigger selector
	int value = eTriggerSelector_FrameStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_On;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerMode_Software;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	value = eTriggerSource_RisingEdge;
	if (!setTriggerActivation(m_MilD, value)) { return false; }
	if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Previous version mark " NotSupport" 
	//Exposure mode
	//value = eExposureMode_Timed;
	//if (!setExposureMode(m_MilD, value)) { return false; }
	//if (!getExposureMode(m_MilD, &value)) { return false; }
	//if (switchSoftwareTriggerMode(m_MilD) == false)	//重置軟體觸發模式
	//{
	//	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera switchSoftwareTriggerMode"));
	//	return false;
	//}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchSoftwareTriggerMode_VC_25MX2_M150I1(MIL_ID *m_MilD)
{
	if (CheckMilPtr(m_MilD) == false) { return false; }

	if (StopDevice() == false)	//停止相機
	{	return false;	}

	//trigger selector
	int value = eTriggerSelector_ExposureStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_On;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerMode_Software;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	value = eTriggerSource_RisingEdge;
	if (!setTriggerActivation(m_MilD, value)) { return false; }
	if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Previous version mark " NotSupport" 
	//Exposure mode
	//value = eExposureMode_Timed;
	//if (!setExposureMode(m_MilD, value)) { return false; }
	//if (!getExposureMode(m_MilD, &value)) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchSoftwareTriggerMode_STC_CMB120ACXP(MIL_ID * m_MilD)
{
	if (CheckMilPtr(m_MilD) == false) { return false; }

	if (StopDevice() == false)	//停止相機
	{	return false;	}

	//trigger selector
	int value = eTriggerSelector_ExposureStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_On;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerMode_Software;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	////TriggerActivation
	//value = eTriggerSource_RisingEdge;
	//if (!setTriggerActivation(m_MilD, value)) { return false; }
	//if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Exposure mode
	value = eExposureMode_Timed;
	if (!setExposureMode(m_MilD, value)) { return false; }
	if (!getExposureMode(m_MilD, &value)) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchExternalTriggerMode(COAXPRESS_CAMERA_DEVICE CameraDevice, MIL_ID *m_MilD)
{
	bool bIsOK = true;
	switch ( CameraDevice )
	{
	//CXP12移植遺留設定，目前不啟用
	//case COAXPRESS_CAMERA_Q_12A180F:
	//case COAXPRESS_CAMERA_VC_12MX_M180:
	//case COAXPRESS_CAMERA_VC_12MX_M180_HOR:
	//	 bIsOK= SwitchExternalTriggerMode_VCC_25CXPHSM(m_MilD); break;
	case COAXPRESS_CAMERA_STC_CMB120ACXP: bIsOK= SwitchExternalTriggerMode_STC_CMB120ACXP(m_MilD); break;
	case COAXPRESS_CAMERA_STC_LBGP251BCXP124: bIsOK = SwitchExternalTriggerMode_STC_LBGP251BCXP124(m_MilD); break;
	default:						      bIsOK=ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	if ( true == bIsOK )
	{	SetCameraGrabMode(CAMERA_GRAB_EXTERNAL_TRIGGER); }
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchExternalTriggerMode_VCC_25CXPHSM(MIL_ID *m_MilD)
{
	if (CheckMilPtr(m_MilD) == false) { return false; }

	if (StopDevice() == false)	//停止相機
	{	return false;	}
	// Sets the signal transition upon which to generate an interrupt, if interrupt generation has been enabled for the specified I/O signal.
	// Specifies to generate an interrupt upon both a low-to-high and a high-to-low signal transition.
	MdigControl(*m_MilD, M_IO_INTERRUPT_ACTIVATION + M_TL_TRIGGER, M_ANY_EDGE);
	// Sets whether to generate an interrupt upon the specified transition of the I/O signal.
	// Specifies not to generate an interrupt.
	MdigControl(*m_MilD, M_IO_INTERRUPT_STATE + M_TL_TRIGGER, M_DISABLE);
	// Sets the mode of the specified I/O signal.
	// Specifies that the signal is for output.
	MdigControl(*m_MilD, M_IO_MODE + M_TL_TRIGGER, M_OUTPUT);
	// Sets the type of signal to route to an output signal, or a bidirectional signal set to output mode.
	// Specifies to reroute auxiliary input signal n to the output signal, where n is the number of the auxiliary input signal.
	MdigControl(*m_MilD, M_IO_SOURCE + M_TL_TRIGGER, M_AUX_IO4);

	//trigger selector
	int value = eTriggerSelector_FrameStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_On;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerSource_LinkTrigger0;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	value = eTriggerMode_LevelLow;
	if (!setTriggerActivation(m_MilD, value)) { return false; }
	if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Previous version mark " NotSupport" 
	//Exposure mode
	//value = eExposureMode_TriggerWidth;
	//if (!setExposureMode(m_MilD, value)) { return false; }
	//if (!getExposureMode(m_MilD, &value)) { return false; }

	//if (switchExternalTriggerMode(m_MilD) == false)	//重置外部觸發模式
	//{
	//	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera switchExternalTriggerMode"));
	//	return false;
	//}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchExternalTriggerMode_VC_25MX2_M150I1(MIL_ID *m_MilD)
{
	if (CheckMilPtr(m_MilD) == false) { return false; }

	if (StopDevice() == false)	//停止相機
	{	return false;	}
	// Sets the signal transition upon which to generate an interrupt, if interrupt generation has been enabled for the specified I/O signal.
	// Specifies to generate an interrupt upon both a low-to-high and a high-to-low signal transition.
	MdigControl(*m_MilD, M_IO_INTERRUPT_ACTIVATION + M_TL_TRIGGER, M_ANY_EDGE);
	// Sets whether to generate an interrupt upon the specified transition of the I/O signal.
	// Specifies not to generate an interrupt.
	MdigControl(*m_MilD, M_IO_INTERRUPT_STATE + M_TL_TRIGGER, M_DISABLE);
	// Sets the mode of the specified I/O signal.
	// Specifies that the signal is for output.
	MdigControl(*m_MilD, M_IO_MODE + M_TL_TRIGGER, M_OUTPUT);
	// Sets the type of signal to route to an output signal, or a bidirectional signal set to output mode.
	// Specifies to reroute auxiliary input signal n to the output signal, where n is the number of the auxiliary input signal.
	MdigControl(*m_MilD, M_IO_SOURCE + M_TL_TRIGGER, M_AUX_IO4);

	//trigger selector
	int value = eTriggerSelector_ExposureStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_On;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerSource_LinkTrigger0;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	value = eTriggerMode_FallingEdge;
	if (!setTriggerActivation(m_MilD, value)) { return false; }
	if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Previous version mark " NotSupport" 
	//Exposure mode
	//value = eExposureMode_TriggerWidth;
	//if (!setExposureMode(m_MilD, value)) { return false; }
	//if (!getExposureMode(m_MilD, &value)) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchExternalTriggerMode_STC_CMB120ACXP(MIL_ID * m_MilD)
{
	if (CheckMilPtr(m_MilD) == false) { return false; }

	if (StopDevice() == false)	//停止相機
	{	return false;	}

	//// Because Matrox card not implement lineInverter, using time1,time2, to invert the trigger source.
	//// M_AUX_IO4 falling to Trigger Timer1
	// Sets the state of the specified timer.
	// Specifies that the timer is Enable.

	MdigControl(*m_MilD, M_TIMER_STATE + M_TIMER1, M_ENABLE);
	// Sets the delay between the timer trigger and the active portion of the timer output signal.
	MdigControl(*m_MilD, M_TIMER_DELAY + M_TIMER1, 8);
	// Sets the duration for the active portion of the timer output signal.
	MdigControl(*m_MilD, M_TIMER_DURATION + M_TIMER1, 72);
	// Sets the signal variation upon which to generate a timer trigger, if the specified timer is enabled.
	// Specifies that a timer trigger will be generated upon a high-to-low signal transition.
	MdigControl(*m_MilD, M_TIMER_TRIGGER_ACTIVATION + M_TIMER1, M_EDGE_FALLING);
	// Selects the trigger source for the specified timer when there are multiple sources available.
	// Specifies to use auxiliary input signal n as the trigger source for the specified timer, where n is the number of the auxiliary signal.
	MdigControl(*m_MilD, M_TIMER_TRIGGER_SOURCE + M_TIMER1, M_AUX_IO4);
		
	////M_AUX_IO4 Rising to Trigger Timer2
	// Sets the state of the specified timer.
	// Specifies that the timer is Enable.
	MdigControl(*m_MilD, M_TIMER_STATE + M_TIMER2, M_ENABLE);
	// Sets the delay between the timer trigger and the active portion of the timer output signal.
	MdigControl(*m_MilD, M_TIMER_DELAY + M_TIMER2, 0);
	// Sets the duration for the active portion of the timer output signal.
	MdigControl(*m_MilD, M_TIMER_DURATION + M_TIMER2, 72);
	// Sets the signal variation upon which to generate a timer trigger, if the specified timer is enabled.
	// Specifies that a timer trigger will be generated upon a high-to-low signal transition.
	MdigControl(*m_MilD, M_TIMER_TRIGGER_ACTIVATION + M_TIMER2, M_EDGE_RISING);
	// Selects the trigger source for the specified timer when there are multiple sources available.
	// Specifies to use auxiliary input signal n as the trigger source for the specified timer, where n is the number of the auxiliary signal.
	MdigControl(*m_MilD, M_TIMER_TRIGGER_SOURCE + M_TIMER2, M_AUX_IO4);

	// Sets the signal source to use to reset the timer to 0.
	// Specifies to use no trigger source.
	//MdigControl(*m_MilD, M_TIMER_RESET_SOURCE + M_TIMER1, M_TIMER2);

	////M_TIMER1 To  M_LINK_TRIGGER_0_OUT
	// Sets whether to enable a specific transmitter/receiver for an I/O signal, on systems whose transmitters/receivers are enabled through software and where the option of two or more signal formats are possible.
	// Specifies to use the transport layer link trigger for the specified I/O signal.
	MdigControl(*m_MilD, M_IO_FORMAT + M_LINK_TRIGGER_0_OUT, M_LINK_SIGNAL);
	// Sets the mode of the specified I/O signal.
	// Specifies that the signal is for output.
	MdigControl(*m_MilD, M_IO_MODE + M_LINK_TRIGGER_0_OUT, M_OUTPUT);
	// Sets the type of signal to route to an output signal, or a bidirectional signal set to output mode.
	// Specifies to route the output of timer n , where n is the number of timers available.
	MdigControl(*m_MilD, M_IO_SOURCE + M_LINK_TRIGGER_0_OUT, M_TIMER1);

	////M_TIMER2  To  M_LINK_TRIGGER_1_OUT
	// Sets whether to enable a specific transmitter/receiver for an I/O signal, on systems whose transmitters/receivers are enabled through software and where the option of two or more signal formats are possible.
	// Specifies to use the transport layer link trigger for the specified I/O signal.
	MdigControl(*m_MilD, M_IO_FORMAT + M_LINK_TRIGGER_1_OUT, M_LINK_SIGNAL);
	// Sets the mode of the specified I/O signal.
	// Specifies that the signal is for output.
	MdigControl(*m_MilD, M_IO_MODE + M_LINK_TRIGGER_1_OUT, M_OUTPUT);
	// Sets the type of signal to route to an output signal, or a bidirectional signal set to output mode.
	// Specifies to route the output of timer n , where n is the number of timers available.
	MdigControl(*m_MilD, M_IO_SOURCE + M_LINK_TRIGGER_1_OUT, M_TIMER2);

	MIL_DOUBLE Gain = 1;
	MdigControlFeature(*m_MilD, M_FEATURE_VALUE, MIL_TEXT("GainSelector"), M_TYPE_STRING, MIL_TEXT("AnalogAll"));
	MdigControlFeature(*m_MilD, M_FEATURE_VALUE, MIL_TEXT("Gain"), M_TYPE_DOUBLE, &Gain);

	//trigger selector
	//int value = eTriggerSelector_ExposureStart;
	int value = eTriggerSelector_FrameStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_On;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerSource_LinkTrigger0;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	//value = eTriggerMode_FallingEdge;
	//if (!setTriggerActivation(m_MilD, value)) { return false; }
	//if (!getTriggerActivation(m_MilD, &value)) { return false; }
		
	//Exposure mode
	value = eExposureMode_TriggerWidth;
	if (!setExposureMode(m_MilD, value)) { return false; }
	if (!getExposureMode(m_MilD, &value)) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::SwitchExternalTriggerMode_STC_LBGP251BCXP124(MIL_ID * m_MilD)
{
	if (CheckMilPtr(m_MilD) == false) { return false; }

	if (StopDevice() == false)	//停止相機
	{
		return false;
	}

	// The following code can be used to control feature values.
	//MdigControlFeature(*m_MilD, M_FEATURE_VALUE, MIL_TEXT("CxpLinkConfiguration"), M_TYPE_STRING, MIL_TEXT("CXP6_X4"));

	// Sets the amount of time that the specified auxiliary input signal is debounced. (unit ns)
	MdigControl(*m_MilD, M_IO_DEBOUNCE_TIME + M_AUX_IO4, 1000);

	//// Because Matrox card not implement lineInverter, using time1,time2, to invert the trigger source.
	//// M_AUX_IO4 falling to Trigger Timer1
	// Sets the state of the specified timer.
	// Specifies that the timer is Enable.
	MdigControl(*m_MilD, M_TIMER_STATE + M_TIMER1, M_ENABLE);
	// Sets the delay between the timer trigger and the active portion of the timer output signal.
	MdigControl(*m_MilD, M_TIMER_DELAY + M_TIMER1, 0);
	// Sets the duration for the active portion of the timer output signal.
	MdigControl(*m_MilD, M_TIMER_DURATION + M_TIMER1, 100);
	// Sets the signal variation upon which to generate a timer trigger, if the specified timer is enabled.
	// Specifies that a timer trigger will be generated upon a high-to-low signal transition.
	MdigControl(*m_MilD, M_TIMER_TRIGGER_ACTIVATION + M_TIMER1, M_EDGE_FALLING);
	// Selects the trigger source for the specified timer when there are multiple sources available.
	// Specifies to use auxiliary input signal n as the trigger source for the specified timer, where n is the number of the auxiliary signal.
	MdigControl(*m_MilD, M_TIMER_TRIGGER_SOURCE + M_TIMER1, M_AUX_IO4);

	////M_AUX_IO4 Rising to Trigger Timer2
	// Sets the state of the specified timer.
	// Specifies that the timer is Enable.
	MdigControl(*m_MilD, M_TIMER_STATE + M_TIMER2, M_ENABLE);
	// Sets the delay between the timer trigger and the active portion of the timer output signal.
	MdigControl(*m_MilD, M_TIMER_DELAY + M_TIMER2, 0);
	// Sets the duration for the active portion of the timer output signal.
	MdigControl(*m_MilD, M_TIMER_DURATION + M_TIMER2, 100);
	// Sets the signal variation upon which to generate a timer trigger, if the specified timer is enabled.
	// Specifies that a timer trigger will be generated upon a high-to-low signal transition.
	MdigControl(*m_MilD, M_TIMER_TRIGGER_ACTIVATION + M_TIMER2, M_EDGE_RISING);
	// Selects the trigger source for the specified timer when there are multiple sources available.
	// Specifies to use auxiliary input signal n as the trigger source for the specified timer, where n is the number of the auxiliary signal.
	MdigControl(*m_MilD, M_TIMER_TRIGGER_SOURCE + M_TIMER2, M_AUX_IO4);

	// Sets the signal source to use to reset the timer to 0.
	// Specifies to use no trigger source.
	//MdigControl(*m_MilD, M_TIMER_RESET_SOURCE + M_TIMER1, M_TIMER2);

	////M_TIMER1 To  M_LINK_TRIGGER_0_OUT
	// Sets whether to enable a specific transmitter/receiver for an I/O signal, on systems whose transmitters/receivers are enabled through software and where the option of two or more signal formats are possible.
	// Specifies to use the transport layer link trigger for the specified I/O signal.
	MdigControl(*m_MilD, M_IO_FORMAT + M_LINK_TRIGGER_0_OUT, M_LINK_SIGNAL);
	// Sets the mode of the specified I/O signal.
	// Specifies that the signal is for output.
	MdigControl(*m_MilD, M_IO_MODE + M_LINK_TRIGGER_0_OUT, M_OUTPUT);
	// Sets the type of signal to route to an output signal, or a bidirectional signal set to output mode.
	// Specifies to route the output of timer n , where n is the number of timers available.
	MdigControl(*m_MilD, M_IO_SOURCE + M_LINK_TRIGGER_0_OUT, M_TIMER1);

	////M_TIMER2  To  M_LINK_TRIGGER_1_OUT
	// Sets whether to enable a specific transmitter/receiver for an I/O signal, on systems whose transmitters/receivers are enabled through software and where the option of two or more signal formats are possible.
	// Specifies to use the transport layer link trigger for the specified I/O signal.
	MdigControl(*m_MilD, M_IO_FORMAT + M_LINK_TRIGGER_1_OUT, M_LINK_SIGNAL);
	// Sets the mode of the specified I/O signal.
	// Specifies that the signal is for output.
	MdigControl(*m_MilD, M_IO_MODE + M_LINK_TRIGGER_1_OUT, M_OUTPUT);
	// Sets the type of signal to route to an output signal, or a bidirectional signal set to output mode.
	// Specifies to route the output of timer n , where n is the number of timers available.
	MdigControl(*m_MilD, M_IO_SOURCE + M_LINK_TRIGGER_1_OUT, M_TIMER2);

	MIL_DOUBLE Gain = 1;
	MdigControlFeature(*m_MilD, M_FEATURE_VALUE, MIL_TEXT("GainSelector"), M_TYPE_STRING, MIL_TEXT("AnalogAll"));
	MdigControlFeature(*m_MilD, M_FEATURE_VALUE, MIL_TEXT("Gain"), M_TYPE_DOUBLE, &Gain);

	//trigger selector
	//int value = eTriggerSelector_ExposureStart;
	int value = eTriggerSelector_FrameStart;
	if (!setTriggerSelector(m_MilD, value)) { return false; }
	if (!getTriggerSelector(m_MilD, &value)) { return false; }

	//trigger mode
	value = eTriggerMode_On;
	if (!setTriggerMode(m_MilD, value)) { return false; }
	if (!getTriggerMode(m_MilD, &value)) { return false; }

	//trigger source
	value = eTriggerSource_LinkTrigger0;
	if (!setTriggerSource(m_MilD, value)) { return false; }
	if (!getTriggerSource(m_MilD, &value)) { return false; }

	//TriggerActivation
	//value = eTriggerMode_FallingEdge;
	//if (!setTriggerActivation(m_MilD, value)) { return false; }
	//if (!getTriggerActivation(m_MilD, &value)) { return false; }

	//Exposure mode
	value = eExposureMode_TriggerWidth;
	if (!setExposureMode(m_MilD, value)) { return false; }
	if (!getExposureMode(m_MilD, &value)) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::processing(MIL_ID *m_MilD, bool enable)
{
	//m_UserHookData.controller = this;
	if (enable)
	{
		MdigProcess(*m_MilD,
			m_MilGrabBufferList,
			m_MilGrabBufferListSize,
			M_START,
			M_DEFAULT,
			ProcessingFunction,
			this);
	}
	else
	{
		/* Stop processing. */
		MdigProcess(*m_MilD,
			m_MilGrabBufferList,
			m_MilGrabBufferListSize,
			M_STOP,
			M_DEFAULT,
			ProcessingFunction,
			this);
	}

	return true;
}
//-------------------------------------------------------------------------------------//
// convert string to wstring
std::wstring CCamera_MatroxRapixoCXP6_Camera::to_wide_string(const std::string& input)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(input);
}
//-------------------------------------------------------------------------------------//
// convert wstring to string 
std::string CCamera_MatroxRapixoCXP6_Camera::to_byte_string(const std::wstring& input)
{
	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(input);
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::executeCameraProperty(MIL_ID *m_MilD, const std::string property)
{
	if (m_MilD)
	{
#ifdef M_MIL_UNICODE_API
		std::wstring str = to_wide_string(property);
#else
		std::string str = property;
#endif
		try
		{
			//MdigControlFeature(MilDigitizer, M_FEATURE_VALUE, MIL_TEXT("TriggerSelector"), M_TYPE_STRING, TriggerSelector);
			//MdigControlFeature(m_MilDigitizer, M_FEATURE_EXECUTE, MIL_TEXT("TriggerSoftware"), M_DEFAULT, M_NULL);
			MdigControlFeature(*m_MilD, M_FEATURE_EXECUTE, str, M_DEFAULT, M_NULL);
		}
		catch (const std::runtime_error& e)
		{
			CString str = e.what();
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s") , str);
			return false;
		}
		catch (...)
		{
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraExecute %s") , property);
			return false;
		}
	}
	else
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s, . (mil null)\n") , property);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::setCameraProperty(MIL_ID *m_MilD, const std::string property, const std::string& value)
{
	if (m_MilD)
	{
#ifdef M_MIL_UNICODE_API
		std::wstring key = to_wide_string(property);
		std::wstring str = to_wide_string(value);
#else
		std::string key = property;
		std::string str = value;
#endif
		try
		{
			MdigControlFeature(*m_MilD, M_FEATURE_VALUE, key, M_TYPE_STRING, str);
		}
		catch (const std::runtime_error& e)
		{
			CString str = e.what();
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s") , str);
			return false;
		}
		catch (...)
		{
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraWrite %s") , property);
			return false;
		}
	}
	else
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraWrite %s, . (mil null)\n") , property);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::setCameraProperty(MIL_ID *m_MilD, const std::string property, __int64 value)
{
	if (m_MilD)
	{
#ifdef M_MIL_UNICODE_API
		std::wstring key = to_wide_string(property);
#else
		std::string key = property;
#endif
		try
		{
			MdigControlFeature(*m_MilD, M_FEATURE_VALUE, key, M_TYPE_INT64, &value);
		}
		catch (const std::runtime_error& e)
		{
			CString str = e.what();
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s") , str);
			return false;
		}
		catch (...)
		{
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraWrite %s") , property);
			return false;
		}
	}
	else
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraWrite %s, . (mil null)\n") , property);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::setCameraProperty(MIL_ID *m_MilD, const std::string property, double value)
{
	
	if (m_MilD)
	{
#ifdef M_MIL_UNICODE_API
		std::wstring key = to_wide_string(property);
#else
		std::string key = property;
#endif
		try
		{
			MdigControlFeature(*m_MilD, M_FEATURE_VALUE, key, M_TYPE_DOUBLE, &value);
		}
		catch (const std::runtime_error& e)
		{
			CString str = e.what();
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s") , str);
			return false;
		}
		catch (...)
		{
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraWrite %s") , property);
			return false;
		}
	}
	else
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraWrite %s, . (mil null)\n") , property);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getCameraProperty(MIL_ID *m_MilD, const std::string property, std::string* value)
{
	
	if (m_MilD)
	{
#ifdef M_MIL_UNICODE_API
		std::wstring key = to_wide_string(property);
		std::wstring str;
#else
		std::string key = property;
		std::string str;
#endif
		try
		{
			MdigInquireFeature(*m_MilD, M_FEATURE_VALUE, key, M_TYPE_STRING, str);
		}
		catch (const std::runtime_error& e)
		{
			CString str = e.what();
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s") , str);
			return false;
		}
		catch (...)
		{
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraRead %s") , property);
			return false;
		}
#ifdef M_MIL_UNICODE_API
		*value = to_byte_string(str);
#else
		*value = str;
#endif
	}
	else
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraRead %s, . (mil null)\n") , property);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getCameraProperty(MIL_ID *m_MilD, const std::string property, __int64* value)
{
	if (m_MilD)
	{
		MIL_INT64 ivalue = 0;
#ifdef M_MIL_UNICODE_API
		std::wstring key = to_wide_string(property);
#else
		std::string key = property;
#endif
		try
		{
		MdigInquireFeature(*m_MilD, M_FEATURE_VALUE, key, M_TYPE_INT64, &ivalue);
		*value = ivalue;
		}
		catch (const std::runtime_error& e)
		{
			CString str = e.what();
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s") , str);
			return false;
		}
		catch (...)
		{
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraRead %s") , property);
			return false;
		}
	}
	else
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraRead %s, . (mil null)\n") , property);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getCameraProperty(MIL_ID *m_MilD, const std::string property, double* value)
{
	if (m_MilD)
	{
		MIL_DOUBLE dbvalue = 0;
	#ifdef M_MIL_UNICODE_API
		std::wstring key = to_wide_string(property);
	#else
		std::string key = property;
	#endif
		try
		{
			MdigInquireFeature(*m_MilD, M_FEATURE_VALUE, key, M_TYPE_DOUBLE, &dbvalue);
			*value = dbvalue;
		}
		catch (const std::runtime_error& e)
		{
			CString str = e.what();
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera %s") , str);
			return false;
		}
		catch (...)
		{
			m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraRead %s") , property);
			return false;
		}
	}
	else
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera CxpCameraRead %s, . (mil null)\n") , property);
		return false;
	}
	return true;
}

//-------------------------------------------------------------------------------------//
//TriggerSelector
bool CCamera_MatroxRapixoCXP6_Camera::setTriggerSelector(MIL_ID *m_MilD, int value)
{//AcquisitionStart, FrameStart
	
	bool res = false;
	switch (value)
	{
	case eTriggerSelector_AcquisitionStart:
		res = setCameraProperty(m_MilD, "TriggerSelector", "AcquisitionStart"); break;
	case eTriggerSelector_FrameStart:
		res = setCameraProperty(m_MilD, "TriggerSelector", "FrameStart"); break;
	case eTriggerSelector_ExposureStart:
		res = setCameraProperty(m_MilD, "TriggerSelector", "ExposureStart"); break;
	default:
		
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera unknow TriggerSelector %d") , value);
		return false;
	}

	if (!res)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera setTriggerSelector fault"));
		return false;
	}
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getTriggerSelector(MIL_ID *m_MilD, int* value)
{
	std::string strValue = "AcquisitionStart";

	if (getCameraProperty(m_MilD, "TriggerSelector", &strValue) == false) 
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getTriggerSelector fault"));
		return false;
	}

	if (strValue.compare("AcquisitionStart") == 0) { *value = eTriggerSelector_AcquisitionStart; return true; }
	if (strValue.compare("FrameStart") == 0) { *value = eTriggerSelector_FrameStart; return true; }
	if (strValue.compare("ExposureStart") == 0) { *value = eTriggerSelector_ExposureStart; return true; }
	
	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getTriggerSelector fault. unknow TriggerSelector"));

	return false;
}
//-------------------------------------------------------------------------------------//
//TriggerMode
bool CCamera_MatroxRapixoCXP6_Camera::setTriggerMode(MIL_ID *m_MilD, int value)
{
	
	bool res = false;
	switch (value)
	{
	case eTriggerMode_Off:
		res = setCameraProperty(m_MilD, "TriggerMode", "Off"); break;
	case eTriggerMode_On:
		res = setCameraProperty(m_MilD, "TriggerMode", "On"); break;
	default:
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera unknow TriggerMode %d") , value);
		return false;
	}

	if (!res)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera setTriggerMode fault."));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getTriggerMode(MIL_ID *m_MilD, int* value)
{
	std::string strValue = "On";
	if (getCameraProperty(m_MilD, "TriggerMode", &strValue) == false) 
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getTriggerMode fault."));
		return false;
	}

	if (strValue.compare("Off") == 0) { *value = eTriggerMode_Off; return true; }
	if (strValue.compare("On") == 0) { *value = eTriggerMode_On; return true; }

	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getTriggerMode fault. unknow TriggerMode"));
	return false;
}
//-------------------------------------------------------------------------------------//
//TriggerSource 
bool CCamera_MatroxRapixoCXP6_Camera::setTriggerSource(MIL_ID *m_MilD, int value)
{
	//LinkTrigger0, Line0, Software
	
	bool res = false;
	switch (value)
	{
	case eTriggerSource_LinkTrigger0:
		res = setCameraProperty(m_MilD, "TriggerSource", "LinkTrigger0"); break;
	case eTriggerMode_Line0:
		res = setCameraProperty(m_MilD, "TriggerSource", "Line0"); break;
	case eTriggerMode_Software:
		res = setCameraProperty(m_MilD, "TriggerSource", "Software"); break;
	default:
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera unknow TriggerSource %d"), value);
		return false;
	}

	if (!res)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera setTriggerSource fault"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getTriggerSource(MIL_ID *m_MilD, int *value)
{
	
	std::string strValue = "Software";
	if (getCameraProperty(m_MilD, "TriggerSource", &strValue) == false)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getTriggerSource fault."));
		return false;
	}

	if (strValue.compare("LinkTrigger0") == 0) { *value = eTriggerSource_LinkTrigger0; return true; }
	if (strValue.compare("Line0") == 0) { *value = eTriggerMode_Line0; return true; }
	if (strValue.compare("Software") == 0) { *value = eTriggerMode_Software; return true; }

	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getTriggerSource fault. unknow TriggerSource"));

	return false;
}
//-------------------------------------------------------------------------------------//
//TriggerActivation
bool CCamera_MatroxRapixoCXP6_Camera::setTriggerActivation(MIL_ID *m_MilD, int value)
{//RisingEdge, FallingEdge, LevelHigh, LevelLow
	

	bool res = false;
	switch (value)
	{
	case eTriggerSource_RisingEdge:
		res = setCameraProperty(m_MilD, "TriggerActivation", "RisingEdge"); break;
	case eTriggerMode_FallingEdge:
		res = setCameraProperty(m_MilD, "TriggerActivation", "FallingEdge"); break;
	case eTriggerMode_LevelHigh:
		res = setCameraProperty(m_MilD, "TriggerActivation", "LevelHigh"); break;
	case eTriggerMode_LevelLow:
		res = setCameraProperty(m_MilD, "TriggerActivation", "LevelLow"); break;
	default:
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera unknow TriggerActivation %d"), value);
		return false;
	}

	if (!res)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera setTriggerActivation fault."));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getTriggerActivation(MIL_ID *m_MilD, int *value)
{
	
	std::string strValue = "RisingEdge";
	if (getCameraProperty(m_MilD, "TriggerActivation", &strValue) == false)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getTriggerActivation fault."));
		return false;
	}

	if (strValue.compare("RisingEdge") == 0) { *value = eTriggerSource_RisingEdge; return true; }
	if (strValue.compare("FallingEdge") == 0) { *value = eTriggerMode_FallingEdge; return true; }
	if (strValue.compare("LevelHigh") == 0) { *value = eTriggerMode_LevelHigh; return true; }
	if (strValue.compare("LevelLow") == 0) { *value = eTriggerMode_LevelLow; return true; }

	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getTriggerActivation fault. unknow value"));
	return false;
}
//-------------------------------------------------------------------------------------//
//ExposureMode
bool CCamera_MatroxRapixoCXP6_Camera::setExposureMode(MIL_ID *m_MilD, int value)
{//Timed, TriggerWidth
	bool res = false;
	switch (value)
	{
	case eExposureMode_Timed:
		res = setCameraProperty(m_MilD, "ExposureMode", "Timed"); break;
	case eExposureMode_TriggerWidth:
		res = setCameraProperty(m_MilD, "ExposureMode", "TriggerWidth"); break;
	default:
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera unknow setExposureMode %d"), value);

		return false;
	}

	if (!res)
	{
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getExposureMode(MIL_ID *m_MilD, int *value)
{
	
	std::string strValue = "TriggerWidth";
	if (getCameraProperty(m_MilD, "ExposureMode", &strValue) == false)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getExposureMode fault."));
		return false;
	}

	if (strValue.compare("Timed") == 0) { *value = eExposureMode_Timed; return true; }
	if (strValue.compare("TriggerWidth") == 0) { *value = eExposureMode_TriggerWidth; return true; }

	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getExposureMode fault.  unknow mode"));

	return false;
}
//-------------------------------------------------------------------------------------//
//ExposureTime  
bool CCamera_MatroxRapixoCXP6_Camera::setExposureTime(MIL_ID *m_MilD, int value)
{
	__int64 value64 = value;
	if (setCameraProperty(m_MilD, "ExposureTime", value64) == false)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera setExposureTime fault."));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getExposureTime(MIL_ID *m_MilD, int *value)
{
	__int64 value64 = 0;
	if (getCameraProperty(m_MilD, "ExposureTime", &value64) == false)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getExposureTime fault."));
		return false;
	}
	*value = static_cast<int>(value64);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getExposureTimeMax(MIL_ID *m_MilD, int *value)
{
	__int64 value64 = 1000000;
	//if (getCameraProperty(m_MilD, "ExposureTimeMax", &value64) == false)
	//{
	//	m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getExposureTimeMax fault."));
	//	return false;
	//}
	*value = static_cast<int>(value64);
	return true;
}
//-------------------------------------------------------------------------------------//
//FrameRate
bool CCamera_MatroxRapixoCXP6_Camera::getFrameRate(MIL_ID *m_MilD, double* value)
{
	if (getCameraProperty(m_MilD, "AcquisitionFrameRate", value) == false)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera getFrameRate fault."));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::triggerSoftware(MIL_ID *m_MilD)
{
	if (executeCameraProperty(m_MilD, "TriggerSoftware") == false)
	{
		m_ErrorString.Format(_T("Error, CCamera_MatroxRapixoCXP6_Camera triggerSoftware fault."));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::getFrameOutCount(MIL_ID *m_MilD, int* value)
{

	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::resetCounter()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_MatroxRapixoCXP6_Camera::cameraReset()
{
	return true;
}
//-------------------------------------------------------------------------------------//
#endif//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
#endif // MATROX_RAPIXO_CXP6_CAMERA_USE
