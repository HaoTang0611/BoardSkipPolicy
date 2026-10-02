// Camera_DynamicModule.cpp: implementation of the CCamera_DynamicModule class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Camera_DynamicModule.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef CAMERA_DYNAMIC_MODULE_USE
//-------------------------------------------------------------------------------------//
CCamera_DynamicModule Camera_DynamicModule(CAMERA_ID_1);
//-------------------------------------------------------------------------------------//	
#ifndef CAMERA_OBJ_DISABLE
	int CameraCallbackFunc(unsigned char *pFrame, unsigned int BuffSize, int W, int H, short PixelSize)
	{
		Camera_DynamicModule.IncrementCountForCameraCallback();
		Camera_DynamicModule.AddCameraRingBufferList(pFrame);
		return 0;
	}
#endif//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CCamera_DynamicModule, CCamera_Basic)
//-------------------------------------------------------------------------------------//
CCamera_DynamicModule::CCamera_DynamicModule(CAMERA_ID CameraID)
{
	m_DynamicModuleName=_T("Camera Dynamic Module");
	SetCameraID(CameraID);
	this->PreInitCamera();
}
//-------------------------------------------------------------------------------------//
CCamera_DynamicModule::~CCamera_DynamicModule()
{
	this->ReleaseCamera();	
	return;
}
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE	
bool CCamera_DynamicModule::GetCameraErrorText(C2D_ERROR ErrCode, CString &str)
{
	switch ( ErrCode )
	{
	case C2D_ERR_SUCCESS:	str=_T("Success");	break;
	case C2D_ERR_FAILED:	str=_T("Failed");	break;
	case C2D_ERR_NOT_IMPLEMENTED:	str=_T("Not Implemented");	break;
	case C2D_ERR_DEVICE_NOT_OPEN:	str=_T("Device Not Open");	break;
	case C2D_ERR_INVALID_HANDLE:	str=_T("Invalid Handle");	break;
	case C2D_ERR_INITIALIZE_FAILED:	str=_T("Initialize Failed");	break;
	case C2D_ERR_COMMAND_INVALID:	str=_T("Command Invalid");	break;
	case C2D_ERR_COMMAND_PARAMETER_INVALID:	str=_T("Command Parameter Invalid");	break;
	default:
		str.Format(_T("Undefined[%d]"), ErrCode);
		break;
	}
	return true;
}
#endif//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
void CCamera_DynamicModule::PreInitCamera()
{
	this->m_LoadCameraDll = false;
	this->m_CameraModelID = CAMERA_OBJ_DYNAMIC_MODULE;
	this->m_CameraModelName = _T("DynamicModule");
	this->m_CameraFPS = 180.0;//180.0;
	this->m_CameraBitCount = 8;	
	this->m_CameraImageW = 4096;//相機影像寬度
	this->m_CameraImageH = 3072;//相機影像高度
	this->m_CameraImageStep = 4096;//相機間距
	this->LoadCameraModuleDefine();
	this->LoadCameraINIFile();//注意會從INI更新影像尺寸
	this->SaveCameraINIFile();

	const int PaddingTime_us = 1000;
	this->m_PeriodTim_us = 5600;//5.6ms for Gen3
	this->m_PeriodTim_us = 7500;//7.5ms for Gen2	

	this->m_ExposureTime_us = 3000;//3ms
	this->m_ExposureTimeMin_us = 20;//曝光時間-Min
	this->m_ExposureTimeMax_us = 1000000;//曝光時間-Max
	this->m_TriggerDelay = 500;//處發延遲時間-us

	this->m_CameraImageStep = m_CameraImageW;
	this->m_CameraSizeRaw = m_CameraImageW*m_CameraImageH;
	this->m_CameraSizeColor = this->m_CameraSizeRaw*3;
	
#ifndef CAMERA_OBJ_DISABLE
	m_CameraHandle = NULL;	
#endif//CAMERA_OBJ_DISABLE

	SetCameraConstructed(true);
	return;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::CheckCameraHandle()
{
#ifndef CAMERA_OBJ_DISABLE
	if ( NULL != m_CameraHandle ) 
	{	return true; }
	m_ErrorString = _T("Error, Camera Handle Exception");
	return false;
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::UpdateCameraModuleName()//更新相機模組名稱
{
	CString ModuleName;
	const int CameraModelID = GetCameraModelID();
	switch ( CameraModelID )
	{
	case CAMERA_OBJ_COAXLINK_Q_12A180_FM:			
		ModuleName = _T("CoaxLink_Q_12A180FM-Model");
		break;
	default:		
		ModuleName = m_DynamicModuleName;
		break;
	}
	SetCameraModelName(ModuleName);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::LoadCameraDll(const wchar_t *pDll)
{
#ifndef CAMERA_OBJ_DISABLE
	FreeCameraDll();
	bool bval = c2dLoadDLL(pDll);
	if ( false == bval ) { return false; }
	this->m_LoadCameraDll = true;
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::FreeCameraDll()
{
#ifndef CAMERA_OBJ_DISABLE
	if ( false == m_LoadCameraDll ) { return true; }
	c2dFreeDLL();
	this->m_LoadCameraDll = false;
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::InitCameraInstance(const wchar_t *pDll, const wchar_t *pDllIni)
{
#ifndef CAMERA_OBJ_DISABLE	
	//bool bval = c2dLoadDLL(pDll);		
	bool bval = LoadCameraDll(pDll);		
	if(bval  == false)
	{
		wchar_t strMsg[128]=L"";
		swprintf_s(strMsg, L"Cannot load %s", pDll);
		m_ErrorString = strMsg;
		MessageBoxW(NULL, strMsg,  L"Attention", MB_APPLMODAL | MB_ICONERROR| MB_OK);
		return (false);
	}

	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;		
	C2D_HANDLE   hCamera=NULL;
	const int CameraModelID = GetCameraModelID();
	Camera2DType CameraType=C2T_TOTAL;
	switch ( CameraModelID )
	{
	case CAMERA_OBJ_COAXLINK_Q_12A180_FM:	
		CameraType = C2T_COAXLINK_Q12A_180F;			
		break;
	default:
		CameraType=C2T_TOTAL;		
		break;
	}
	if ( C2T_TOTAL == CameraType )
	{
		m_ErrorString.Format(_T("Error, Camera Model ID Exception [%d]"), CameraModelID);
		return false;
	}	

	if (C2D_ERR_SUCCESS != (ErrCode = c2dOpen(pDllIni, CameraType, &hCamera) ) )
	{	return false; }
	
	if ( C2D_ERR_SUCCESS != (ErrCode = c2dInitialize(hCamera) ))
	{	return  (true);	}
	m_CameraHandle = hCamera;
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::ExitCameraInstance()
{
#ifndef CAMERA_OBJ_DISABLE	
	if ( NULL == m_CameraHandle ) 
	{
		FreeCameraDll();
		return true; 
	}
	c2dUninitialize(m_CameraHandle);		
	c2dClose(m_CameraHandle);		
	m_CameraHandle = NULL;	
	FreeCameraDll();
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::InitialCamera()//相機初始化
{
	const char fnName[] = "CCamera_DynamicModule::InitialCamera";
	char string[128]={0};

	CCamera_Basic::SaveCameraProcess(_T("InitialCamera"), MSG_LEVEL_HIGH);

	this->ReleaseCamera();
	if ( CCamera_DynamicModule::LoadCameraINIFile() == false ) { return false; }	
#ifndef CAMERA_OBJ_DISABLE	
	
	std::wstring Folder = AOIDataCollect.GetAOIDirectoryW();
#if _DEBUG
	std::wstring dllName=L"CameraModule2dUnitD.dll";	
#else
	std::wstring dllName=L"CameraModule2dUnit.dll";	
#endif

#ifdef _X64
	std::wstring iniPaht= Folder+std::wstring(L"\\Camera\\CameraModule2d.ini");
#else
	std::wstring iniPaht= Folder+std::wstring(L"\\Camera\\CameraModule2d(x32).ini");
#endif//_X64

	if ( InitCameraInstance(dllName.c_str(), iniPaht.c_str()) == false )
	{	return false; }		

	CString      str;
	C2D_ERROR    ErrCode;	
	CCameraInfo  CameraInfo;
	C2D_HANDLE   CameraHandle=m_CameraHandle;	
	ErrCode = c2dGetCameraInfo(CameraHandle, CameraInfo);		
	if (C2D_ERR_SUCCESS != ErrCode ) 
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false; 
	}

	ErrCode = c2dRegisterFunction(CameraHandle, CameraCallbackFunc);
	if (C2D_ERR_SUCCESS != ErrCode ) 
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false; 
	}

	m_CameraImageW = CameraInfo.m_Width;
	m_CameraImageH = CameraInfo.m_Height;
	m_CameraFPS    = CameraInfo.m_FPS;
	m_CameraImageStep= CameraInfo.m_Width;
	m_CameraSizeRaw = CameraInfo.m_PayLoadSize;
	m_CameraSizeColor = m_CameraSizeRaw*3;
	m_CameraBitCount = CameraInfo.m_PixelByte*8;
	//m_CameraBayerType

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
		return false;
	}
	::memset(m_ClonedRawImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH);	
	//::memset(m_ClonedColorImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH*4);

#endif//CAMERA_OBJ_DISABLE
	this->m_CameraBayerType = BAYER_PATTERN_GRAY;
	this->m_CameraInited = TRUE;	
	this->CreateBMPInfo(m_pColorInfo, true);
	this->CreateBMPInfo(m_pMonoInfo, false);

	if ( CCamera_DynamicModule::SaveCameraINIFile() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::ReleaseCamera()//相機釋放
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ReleaseCamera"), MSG_LEVEL_HIGH);

#ifndef CAMERA_OBJ_DISABLE
	ExitCameraInstance();
#endif//CAMERA_OBJ_DISABLE
	
	this->m_CameraInited = FALSE;		
	if ( this->m_pColorInfo != NULL )
	{	delete[] m_pColorInfo; m_pColorInfo=NULL; }
	if ( this->m_pMonoInfo != NULL )
	{	delete[] m_pMonoInfo; m_pMonoInfo=NULL; }

	if ( NULL != m_ClonedRawImagePtr )
	{	JetMemory.free_func(m_ClonedRawImagePtr); }

	if ( NULL != m_ClonedColorImagePtr )
	{	JetMemory.free_func(m_ClonedColorImagePtr);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::DoCameraModuleStartGrab()
{
	if ( this->CheckCameraInited() == false ) { return false; }	
	CCamera_Basic::SaveCameraProcess(_T("DoCameraModuleStartGrab"), MSG_LEVEL_HIGH);

	this->m_CameraImagePtr = NULL;
#ifndef CAMERA_OBJ_DISABLE
	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;
	if ( true == m_CameraGrabbing ) 
	{	return true; }

	if ( this->StopCameraGrab() == false ) { return false; }
	/*
	switch ( this->m_CameraGrabMode )
	{
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		if ( this->SwitchExternalTriggerMode() == false )
		{	return false; }		
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		if ( this->SwitchSoftwareTriggerMode() == false )
		{	return false; }		
		break;
	default://CAMERA_GRAB_FREE_RUN
		if ( this->SwitchFreeRunMode() == false )
		{	return false; }		
		break;
	}
	*/
	ErrCode = c2dLive(m_CameraHandle);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SwitchFreeRunMode()
{
#ifndef CAMERA_OBJ_DISABLE
	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;
	ErrCode = c2dSetGrabMode(m_CameraHandle, CGM_FREE_RUN);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}	

	ErrCode = c2dLive(m_CameraHandle);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SwitchSoftwareTriggerMode()
{
#ifndef CAMERA_OBJ_DISABLE
	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;	
	ErrCode = c2dSetGrabMode(m_CameraHandle, CGM_TRIGGER);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
	ErrCode = c2dSetTriggerMode(m_CameraHandle, CTM_INTERNAL);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SwitchExternalTriggerMode()
{
	bool IsOK = true;
#ifndef LIGHT_CTRL_DISABLE	
	#if LIGHT_CTRL_BOARD_TYPE == LIGHT_CTRL_BOARD_TYPE_A5V1
		IsOK = SwitchExternalTriggerMode_A5V1();
	#elif LIGHT_CTRL_BOARD_TYPE == LIGHT_CTRL_BOARD_TYPE_A5V2
		IsOK = SwitchExternalTriggerMode_A5V2();
	#else	
		IsOK = SwitchExternalTriggerMode_DLP();
	#endif//LIGHT_CTRL_BOARD_TYPE	
#else
	IsOK = SwitchExternalTriggerMode_DLP();	
#endif//LIGHT_CTRL_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SwitchExternalTriggerMode_DLP()
{
#ifndef CAMERA_OBJ_DISABLE
	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;	
	ErrCode = c2dSetTriggerMode(m_CameraHandle, CTM_EXTERNAL);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SwitchExternalTriggerMode_A5V1()
{
#ifndef CAMERA_OBJ_DISABLE
	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;	
	ErrCode = c2dSetGrabMode(m_CameraHandle, CGM_TRIGGER);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
	ErrCode = c2dSetTriggerMode(m_CameraHandle, CTM_EXTERNAL);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SwitchExternalTriggerMode_A5V2()//LIGHT_CTRL_BOARD_TYPE_A5V2	
{
#ifndef CAMERA_OBJ_DISABLE
	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;	
	ErrCode = c2dSetGrabMode(m_CameraHandle, CGM_TRIGGER);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
	ErrCode = c2dSetTriggerMode(m_CameraHandle, CTM_EXTERNAL);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE			
	if ( m_CameraGrabMode == Mode ) 
	{	return true; }
	//if ( this->StopCameraGrab() == false ) { return false; }	
	switch ( Mode )
	{
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		if ( this->SwitchExternalTriggerMode() == false )
		{	return false; }		
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		if ( this->SwitchSoftwareTriggerMode() == false )
		{	return false; }		
		break;
	default://CAMERA_GRAB_FREE_RUN
		if ( this->SwitchFreeRunMode() == false )
		{	return false; }		
		break;
	}
	m_CameraGrabMode = Mode;
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SetExposureTime(const int ExposureTime)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	if ( m_ExposureTime_us == ExposureTime ) 
	{	return true; }

	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;	
	ErrCode = c2dSetExposureTime(m_CameraHandle, ExposureTime);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
	m_ExposureTime_us = ExposureTime;

#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::FreeCameraImagePtr(void *ImagePtr)//釋放相機內的影像
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("FreeCameraImagePtr"), MSG_LEVEL_HIGH);

#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::StartCameraGrab()//只取一張影像-內部觸發用
{
	CCamera_Basic::SaveCameraProcess(_T("StartCameraGrab"), MSG_LEVEL_HIGH);

	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();
	if ( this->DoCameraModuleStartGrab() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::StartCameraLiveGrab()//連續取多張影像-外部觸發用
{
	CCamera_Basic::SaveCameraProcess(_T("StartCameraLiveGrab"), MSG_LEVEL_HIGH);

	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();

	if ( this->DoCameraModuleStartGrab() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::StopCameraGrab()//停止取像
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE
	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;	
	ErrCode = c2dFreeze(m_CameraHandle);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
	m_CameraGrabbing = false;	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::FireSoftwareTrigger()//發射軟體觸發訊號
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	if ( CheckCameraHandle() == false ) { return false; }

	CString      str;
	C2D_ERROR    ErrCode=C2D_ERR_SUCCESS;	
	ErrCode = c2dSnapshot(m_CameraHandle);
	if ( C2D_ERR_SUCCESS != ErrCode )
	{	
		GetCameraErrorText(ErrCode, str);
		m_ErrorString.Format(_T("Error, Camera Module %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::WaitforCameraReadytoTrigger()//等待相機準備好可以觸發
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("WaitforCameraReadytoTrigger"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::ResetCameraReadyTriggerEvent()//復歸相機準備好了的事件
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("ResetCameraReadyTriggerEvent"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::DoCameraDebayer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const unsigned char *pRaw, unsigned char *pResult, BAYER_PATTERN_MODE BayerPattern)
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("DoCameraDebayer"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::GetWhiteBalanceParams(double &WBR, double &WBG, double &WBB)
{
#ifndef CAMERA_OBJ_DISABLE	
	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SetWhiteBalanceParams(double WBR, double WBG, double WBB)
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("SetWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::CalcWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("CalcWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::ResetWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("ResetWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::WriteCameraParameterToDevice()//儲存目前相機的參數至相機內部的韌體上
{
#ifndef CAMERA_OBJ_DISABLE	
	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::LoadCameraModuleDefine()//載入相機模組定義 
{
	CString KeyName;
	CString KeyString;
	CString Section = m_DynamicModuleName;	
	CString FileName = CCamera_Basic::GetCameraINIFileName();
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];

	Section.Format(_T("%s %d"), m_DynamicModuleName, m_CameraID);
	KeyName.Format(_T("Camera Model ID")); KeyString.Format(_T("%d"),m_CameraModelID);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraModelID = ::_ttoi(ReturnString); }
	else
	{	JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString);	}
	UpdateCameraModuleName();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::SaveCameraINIFile()
{
	if ( CCamera_Basic::SaveCameraINIFile() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_DynamicModule::LoadCameraINIFile()
{	
	if ( CCamera_Basic::LoadCameraINIFile() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
#endif//CAMERA_DYNAMIC_MODULE_USE
//-------------------------------------------------------------------------------------//