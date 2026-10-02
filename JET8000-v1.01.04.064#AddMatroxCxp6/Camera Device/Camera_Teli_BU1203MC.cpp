// Camera_Teli_BU1203MC.cpp: implementation of the CCamera_Teli_BU1203MC class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//	
#include "stdafx.h"
#include "Camera_Teli_BU1203MC.h"
//-------------------------------------------------------------------------------------//	
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//	
#ifdef TELI_BU1203MC_USE
//-------------------------------------------------------------------------------------//	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//	
CCamera_Teli_BU1203MC Camera_Teli_BU1203MC;
//-------------------------------------------------------------------------------------//	
#ifndef CAMERA_OBJ_DISABLE
using namespace Teli;
//-------------------------------------------------------------------------------------//	
void CALLBACK CameraCallBackImageAcquired(CAM_HANDLE hCam, CAM_STRM_HANDLE hStrm, CAM_IMAGE_INFO *psImageInfo, uint32_t uiBufferIndex, void *pvContext)// Callback for handling received image.
{
	CCamera_Teli_BU1203MC *This = reinterpret_cast<CCamera_Teli_BU1203MC*>(pvContext);
	if ( NULL == This ) { return ; }
	if ( NULL == psImageInfo ) { return; }
	unsigned char *ImgPtr = (unsigned char*)(psImageInfo->pvBuf);
	if ( NULL == ImgPtr ) { return; }

	Teli::CAM_API_STATUS   uiStatus = CAM_API_STS_SUCCESS;
	This->SaveCameraCurrentProcess(_T("CCamera_Teli_BU1203MC::CameraCallBackImageAcquired"));

	uiStatus = Teli::Strm_LockBuffer(hStrm, uiBufferIndex, psImageInfo);
	//const size_t   ImgSize= psImageInfo->uiSize;
	//const size_t   ImgId  = psImageInfo->ullImageId;
	//CAM_PIXEL_FORMAT PxlFrmt = psImageInfo->uiPixelFormat;
	//Teli::CAM_API_STATUS ImgSts=psImageInfo->uiStatus;
	This->IncrementCountForCameraCallback();
	This->AddCameraRingBufferList(ImgPtr);		
    uiStatus = Teli::Strm_UnlockBuffer(hStrm, uiBufferIndex);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CALLBACK CameraCallBackErrorImage(CAM_HANDLE hCam, CAM_STRM_HANDLE hStrm, CAM_API_STATUS uiErrorStatus, uint32_t uiBufferIndex, void *pvContext)// Callback for handling receive error.
{
	CCamera_Teli_BU1203MC *This = reinterpret_cast<CCamera_Teli_BU1203MC*>(pvContext);
	if ( NULL == This ) { return ; }
	This->SaveCameraCurrentProcess(_T("CCamera_Teli_BU1203MC::CameraCallBackErrorImage"));
	return ;
}
//-------------------------------------------------------------------------------------//
void CALLBACK CameraCallBackBufferBusy(CAM_HANDLE hCam, CAM_STRM_HANDLE hStrm, uint32_t uiBufferIndex, void *pvContext)// Callback for handling receive error.
{
	CCamera_Teli_BU1203MC *This = reinterpret_cast<CCamera_Teli_BU1203MC*>(pvContext);
	if ( NULL == This ) { return ; }
	This->SaveCameraCurrentProcess(_T("CCamera_Teli_BU1203MC::CameraCallBackBufferBusy"));
	return ;
}
//-------------------------------------------------------------------------------------//
void CALLBACK CameraCallBackExposureEnd(CAM_HANDLE hCam, CAM_STRM_HANDLE hStrm, void *pvContext)// Callback for handling receive error.
{
	CCamera_Teli_BU1203MC *This = reinterpret_cast<CCamera_Teli_BU1203MC*>(pvContext);
	if ( NULL == This ) { return ; }
	This->SaveCameraCurrentProcess(_T("CCamera_Teli_BU1203MC::CameraCallBackExposureEnd"));	
	This->IncrementCountForCameraExposuredEnd();
	This->CheckCameraExposuredEndEvent();
	return ;
}
//-------------------------------------------------------------------------------------//
#endif//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//	
IMPLEMENT_DYNAMIC(CCamera_Teli_BU1203MC, CCamera_Basic)
//-------------------------------------------------------------------------------------//
CCamera_Teli_BU1203MC::CCamera_Teli_BU1203MC():CCamera_Basic()
{	
	PreInitCamera();
	SetCameraConstructed(true);
}
//-------------------------------------------------------------------------------------//	
CCamera_Teli_BU1203MC::~CCamera_Teli_BU1203MC()
{
	ReleaseCamera();
}
//-------------------------------------------------------------------------------------//
void CCamera_Teli_BU1203MC::PreInitCamera()
{	
	this->m_CameraModelID = CAMERA_OBJ_TELI_BU1203MC;
	this->m_CameraModelName = _T("TELI_BU1203MC");

	this->m_PeriodTim_us = 45000;//30ms
	this->m_ExposureTime_us = 50000;//20ms
	this->m_ExposureTimeMin_us = 45;//曝光時間-Min
	this->m_ExposureTimeMax_us = 16023600;//曝光時間-Max
	this->m_TriggerDelay = 500;//處發延遲時間-us

	this->m_CameraFPS = 30.0;
	this->m_CameraBitCount = 8;
	this->m_CameraImageStep = 4000;//相機間距
	this->m_CameraImageW = 4000;//相機影像寬度
	this->m_CameraImageH = 3000;//相機影像高度
	this->m_CameraSizeRaw = m_CameraImageW*m_CameraImageH;
	this->m_CameraSizeColor = this->m_CameraSizeRaw*3;
	this->m_CameraBayerType = BAYER_PATTERN_GBRG;
	SetCameraImageBufferCount(CAMERA_IMAGE_BUFFER_COUNT_2D);

	m_CameraWhiteBalanceRatioRed = 1.0;
	m_CameraWhiteBalanceRatioGrn = 1.0;
	m_CameraWhiteBalanceRatioBlu = 1.0;

#ifndef CAMERA_OBJ_DISABLE
	m_hCam  = NULL;        //Camera Handle.
	m_hStrm = NULL;        // Stream handle.
	m_hEvent= NULL;        // Event handle.
	m_hStrmEvt = NULL;     //Completion event for stream.
	m_hFrmTrgWaitEvt = NULL;   //Event for Frame Trigger Wait Event
	m_TeliCameraBufferEnabled = true;
#endif//endif CAMERA_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::ConnectCamera()
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::InitialCamera()
{
	const char fnName[] = "CCamera_Teli_BU1203MC::InitialCamera";
	char string[128]={0};

	this->ReleaseCamera();
	if ( CCamera_Teli_BU1203MC::LoadCameraINIFile() == false ) { return false; }
	if ( CCamera_Teli_BU1203MC::SaveCameraINIFile() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE

	CString str;
	CString          Err;
	bool8_t          bStatus       = true;
    uint32_t         uiNum         = 0;
    uint32_t         uiImgBufSize  = 0;
	CAM_INFO         sCamInfo;
	CAM_SYSTEM_INFO  sSysInfo;	
	CAM_API_STATUS   uiStatus = CAM_API_STS_SUCCESS;
	
	CAM_HANDLE       s_hCam        = NULL;     //Camera Handle.
	CAM_STRM_HANDLE  hStrm         = NULL;     // Stream handle.
    CAM_EVT_HANDLE   hEvent        = NULL;     // Event handle.
	HANDLE           hStrmEvt      = NULL;	
	HANDLE           hFrmTrgWaitEvt= NULL;		

	::memset(&sCamInfo, 0x00, sizeof(sCamInfo));
	::memset(&sSysInfo, 0x00, sizeof(sSysInfo));

	// API initialization.	
	uiStatus = Teli::Sys_Initialize();    
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::Sys_Initialize Fault (%s)"), Err);
		return false; 
	}		
	
	uiStatus = Sys_GetInformation(&sSysInfo);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::Sys_GetInformation Fault (%s)"), Err);
		CloseTeliSystem();
		return false; 
	}

	// Get number of camera.
    uiStatus = Teli::Sys_GetNumOfCameras(&uiNum);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Sys_GetNumOfCameras Fault (%s)"), Err);
		CloseTeliSystem();
		return false; 
	}

	const int MatchCameraCnt = 1;
	if ( MatchCameraCnt != uiNum )
	{
		m_ErrorString.Format(_T("Error, Teli Camera count is exception (%d)"), uiNum);
		CloseTeliSystem();
		return false; 
	}

	int CameraIdx=0;	
	uiStatus = Cam_GetInformation((CAM_HANDLE)NULL, CameraIdx, &sCamInfo);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Cam_GetInformation Fault (%s)"), Err);
		CloseTeliSystem();
		return false; 
	}
	m_CameraSerialNumber=sCamInfo.szSerialNumber;

	if ( Teli::CAM_TYPE_U3V != sCamInfo.eCamType )
	{
		m_ErrorString.Format(_T("Error, Teli Camera is not USB Interface"));
		CloseTeliSystem();
		return false; 
	}

	// Open camera that is detected first, in this sample code.
    uiStatus = Cam_Open(CameraIdx, &s_hCam);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Cam_Open Fault (%s)"), Err);
		CloseTeliSystem();
		return false; 
	}
 
	/*
	Teli::CAM_PIXEL_FORMAT  PixelFormat = Teli::PXL_FMT_BayerGR8;
	s_uiStatus = Teli::SetCamPixelFormat(s_hCam, PixelFormat);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamPixelFormat Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}
	*/
	
	int ImageSizeW = (int)(this->GetCameraImageW());
	int ImageSizeH = (int)(this->GetCameraImageH());
	#if ( CAMERA_ROTATION_MODE==CAMERA_ROTATION_090 || CAMERA_ROTATION_MODE==CAMERA_ROTATION_270 || CAMERA_ROTATION_MODE==CAMERA_ROTATION_090_YMIRROR || CAMERA_ROTATION_MODE==CAMERA_ROTATION_270_YMIRROR )
	//	ImageSizeW = (int)(this->GetCameraImageH());
	//	ImageSizeH = (int)(this->GetCameraImageW());
	#endif

	uint32_t SensorSizeW=0;
	uint32_t SensorSizeH=0;	
	// Get sensor width.
	uiStatus = GetCamSensorWidth(s_hCam, &SensorSizeW);
    if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamSensorWidth Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	// Get sensor height.
	uiStatus = GetCamSensorHeight(s_hCam, &SensorSizeH);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamSensorHeight Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}  

	if ( ImageSizeW!=SensorSizeW || ImageSizeH!=SensorSizeH )
	{
		m_ErrorString.Format(_T("Error, Teli Cmaera Image size exception (W:%d, H:%d)"), SensorSizeW, SensorSizeH);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false;
	}

	//Set Image Width
	uiStatus = SetCamWidth(s_hCam, ImageSizeW);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamWidth Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}  

	//Set Image Height
	uiStatus = SetCamHeight(s_hCam, ImageSizeH);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamHeight Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}  

	uint32_t PayloadSize=0;	
	//Get Camera Stream Payload Size
	uiStatus = GetCamStreamPayloadSize(s_hCam, &PayloadSize);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamStreamPayloadSize Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	} 

	//重新變更影像相關尺寸, 避免記憶體空間不夠
	m_CameraImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageSizeW, 8, 4);
	m_CameraSizeRaw = m_CameraImageStep*ImageSizeH;
	m_CameraSizeColor = m_CameraImageStep*ImageSizeH*3;
	const size_t BufferSize = GetImageRawSize();
	if ( PayloadSize > BufferSize )//比預計的記憶體尺寸大, 錯誤
	{
		m_ErrorString.Format(_T("Error, Teli Cmaera Stream size exception (%d//%d)"), PayloadSize, BufferSize);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	uint32_t acqFrmCntMin=0, acqFrmCntMax=0;	
	//Get Camera Acquisition Frame Count Min Max //MultiFrame/ImageBuffer
	uiStatus = GetCamAcquisitionFrameCountMinMax(s_hCam, &acqFrmCntMin, &acqFrmCntMax);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamAcquisitionFrameCountMinMax Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}
	
	//SetCamHighFramerateMode//此功能跟相機有關係

	//Turn off auto gain
	Teli::CAM_GAIN_AUTO_TYPE eGainAuto = Teli::CAM_GAIN_AUTO_OFF;
	uiStatus = SetCamGainAuto(s_hCam, eGainAuto);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamGainAuto Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	// Set software trigger mode.
    // Set TriggerMode true.
    uiStatus = SetCamTriggerMode(s_hCam, true);
    if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerMode Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	const bool bSoftWareTrig=false;
	// Set TriggerSequence TriggerSequence1.
	if ( false == bSoftWareTrig )
	{	uiStatus = SetCamTriggerSequence(s_hCam, CAM_TRIGGER_SEQUENCE1); }
	else
	{	uiStatus = SetCamTriggerSequence(s_hCam, CAM_TRIGGER_SEQUENCE0); }
    if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerSequence Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	// Set TriggerSource hardware.
	if ( false == bSoftWareTrig )
	{	uiStatus = SetCamTriggerSource(s_hCam, CAM_TRIGGER_LINE0);	}
	else
	{	uiStatus = SetCamTriggerSource(s_hCam, CAM_TRIGGER_SOFTWARE);	}
    if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerSource Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	Teli::CAM_TRIGGER_ACTIVATION_TYPE eTriggerActivation = Teli::CAM_TRIGGER_FALLING_EDGE;	//CAM_TRIGGER_RISING_EDGE	
	uiStatus = Teli::SetCamTriggerActivation(s_hCam, eTriggerActivation);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerActivation Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	//設定相機曝光方式
	Teli::CAM_EXPOSURE_TIME_CONTROL_TYPE eExpControl = Teli::CAM_EXPOSURE_TIME_CONTROL_MANUAL;
	uiStatus = Teli::SetCamExposureTimeControl(s_hCam, eExpControl);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamExposureTimeControl Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	float64_t  dExpMin=0, dExpMax=0;
	// Get Exposure time minimum and maximum values.
    uiStatus = GetCamExposureTimeMinMax(s_hCam, &dExpMin, &dExpMax);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamExposureTimeMinMax Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false;	
	}
	m_ExposureTimeMin_us = dExpMin;
	m_ExposureTimeMax_us = dExpMax;	

	//Turn off auto white balance
	Teli::CAM_BALANCE_WHITE_AUTO_TYPE eBalanceWhiteAuto = Teli::CAM_BALANCE_WHITE_AUTO_OFF;
	uiStatus = SetCamBalanceWhiteAuto(s_hCam, eBalanceWhiteAuto);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamBalanceWhiteAuto Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}


	//白平衡參數	
	float64_t dBalanceRatioMinR=0, dBalanceRatioMaxR=0;
	float64_t dBalanceRatioMinB=0, dBalanceRatioMaxB=0;	
	uiStatus = Teli::GetCamBalanceRatioMinMax(s_hCam, Teli::CAM_BALANCE_RATIO_SELECTOR_RED, &dBalanceRatioMinR, &dBalanceRatioMaxR);
	uiStatus = Teli::GetCamBalanceRatioMinMax(s_hCam, Teli::CAM_BALANCE_RATIO_SELECTOR_BLUE, &dBalanceRatioMinB, &dBalanceRatioMaxB);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamBalanceRatioMinMax Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	float64_t dBalanceRatioRed=0.0, dBalanceRatioBlu=0.0;
	uiStatus = Teli::GetCamBalanceRatio(s_hCam, CAM_BALANCE_RATIO_SELECTOR_RED, &dBalanceRatioRed);
	uiStatus = Teli::GetCamBalanceRatio(s_hCam, CAM_BALANCE_RATIO_SELECTOR_BLUE, &dBalanceRatioBlu);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamBalanceRatio Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	float64_t dBalanceRatioRed2=0.0, dBalanceRatioBlu2=0.0;		
	dBalanceRatioRed2 = m_CameraWhiteBalanceRatioRed;
	dBalanceRatioBlu2 = m_CameraWhiteBalanceRatioBlu;
	uiStatus = Teli::SetCamBalanceRatio(s_hCam, CAM_BALANCE_RATIO_SELECTOR_RED, dBalanceRatioRed2);
	uiStatus = Teli::SetCamBalanceRatio(s_hCam, CAM_BALANCE_RATIO_SELECTOR_BLUE, dBalanceRatioBlu2);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamBalanceRatio Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	const bool bSaveCameraParam=false;
if ( true == bSaveCameraParam )
{
	Teli::CAM_USER_SET_SELECTOR_TYPE eSelector = Teli::CAM_USER_SET_SELECTOR_USER_SET1;
	//將參數設定於第1組組態檔
	uiStatus = Teli::ExecuteCamUserSetSave(s_hCam, eSelector);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::ExecuteCamUserSetSave Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}
	//CAMAPI ExecuteCamUserSetLoad(CAM_HANDLE hCam, CAM_USER_SET_SELECTOR_TYPE eSelector);
	//CAMAPI ExecuteCamUserSetSave(CAM_HANDLE hCam, CAM_USER_SET_SELECTOR_TYPE eSelector);	
	

	//還原參數設定於第1組組態檔
	//uiStatus = Teli::ExecuteCamUserSetLoad(s_hCam, eSelector);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::ExecuteCamUserSetLoad Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}
}//bSaveCameraParam

	// ImageBuffer
	uint32_t uiImgBufFrmCnt=0;
	CAM_IMAGE_BUFFER_MODE_TYPE eImgBufModeType;
	const bool UseCameraBuffer = GetTeliCameraBufferEnabled();
	uiStatus = GetCamImageBufferMode(s_hCam, &eImgBufModeType);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamImageBufferMode Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}
	if ( true == UseCameraBuffer )//
	{
		eImgBufModeType = CAM_IMAGE_BUFFER_MODE_ON;
		uiStatus = SetCamImageBufferMode(s_hCam, eImgBufModeType);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{	
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamImageBufferMode Fault (%s)"), Err);
			CloseCameraHandle(s_hCam);
			CloseTeliSystem();
			return false; 
		}
	}

	uiStatus = GetCamImageBufferFrameCount(s_hCam, &uiImgBufFrmCnt);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamImageBufferFrameCount Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	uint32_t uiImgBufFrmMaxCnt=0;
	uint64_t ullAdrs = 0x203098;//Read Camera Buffer Max Count Address;
	uint32_t uiSizeQuadlet=sizeof(uiImgBufFrmMaxCnt)/sizeof(uint32_t);
	uiStatus = Cam_ReadReg(s_hCam, ullAdrs, uiSizeQuadlet, &uiImgBufFrmMaxCnt);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Cam_ReadReg[ImageBufferFrameCountMax] Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}
    //CAMAPI ExecuteCamImageBufferRead(CAM_HANDLE hCam);

	// Open event.
	uint32_t uiEvtBufferCount = DEFAULT_API_BUFFER_CNT;		
	uiStatus = Evt_OpenSimple(s_hCam, &hEvent, uiEvtBufferCount);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Evt_OpenSimple Fault (%s)"), Err);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	// Create completion event for camera event.Trigger signal for image acquisition becomes acceptable
     //hFrmTrgWaitEvt = CreateEvent(NULL, FALSE, FALSE, NULL);//Sample Code
	BOOL bManualReset = TRUE;//TRUE, FALSE
	BOOL bInitialState = TRUE;
	hFrmTrgWaitEvt = CreateEvent(NULL, bManualReset, bInitialState, NULL);
	if ( NULL == hFrmTrgWaitEvt )
	{
		m_ErrorString = _T("Error, CreateEvent For Camera Fault (FrameTriggerWait)");
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	// Activate FrameTriggerWait event.
	uiStatus = Evt_Activate(hEvent, "FrameTriggerWait", hFrmTrgWaitEvt);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Evt_Activate Fault (%s)"), Err);
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	// Create completion event for stream.
	hStrmEvt = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL == hStrmEvt)
	{
		m_ErrorString = _T("Error, CreateEvent For Camera Stream Fault");
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false;
	}

	// Open stream.
	uint32_t uiApiBufferCount = DEFAULT_API_BUFFER_CNT;	
	uint32_t CameraImageBufferCount = GetCameraImageBufferCount();
	uiApiBufferCount = MIN(128, CameraImageBufferCount);//記憶體緩衝張數1 ~ 128
	uiStatus = Strm_OpenSimple(s_hCam, &hStrm, &uiImgBufSize, hStrmEvt, uiApiBufferCount);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Strm_OpenSimple Fault (%s)"), Err);
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}
	
	// Set callback for image acquired event 
	uiStatus = Strm_SetCallbackImageAcquired(hStrm, this, CameraCallBackImageAcquired);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Strm_SetCallbackImageAcquired Fault (%s)"), Err);
		CloseStreamHandle(hStrm, hStrmEvt);
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}

	// Set callback for stream error event 
    uiStatus = Strm_SetCallbackImageError(hStrm, this, CameraCallBackErrorImage);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Strm_SetCallbackImageError Fault (%s)"), Err);
		CloseStreamHandle(hStrm, hStrmEvt);
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}    

	// Set callback for buffer busy event 
    uiStatus = Strm_SetCallbackBufferBusy(hStrm, this, CameraCallBackBufferBusy);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Strm_SetCallbackBufferBusy Fault (%s)"), Err);
		CloseStreamHandle(hStrm, hStrmEvt);
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false; 
	}   

	//
	uiStatus = Evt_ActivateCallback(hEvent, "ExposureEnd", this, CameraCallBackExposureEnd);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{	
		m_ErrorString.Format(_T("Error, Exec Teli::Evt_ActivateCallback Fault (%s)"), Err);
		CloseStreamHandle(hStrm, hStrmEvt);
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
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

		CloseStreamHandle(hStrm, hStrmEvt);
		CloseEventHandle(hEvent, hFrmTrgWaitEvt);
		CloseCameraHandle(s_hCam);
		CloseTeliSystem();
		return false;
	}
	::memset(m_ClonedRawImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH);	
	//::memset(m_ClonedColorImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH*4);

	m_hCam   = s_hCam;
	m_hStrm  = hStrm;
	m_hEvent = hEvent;
	m_hStrmEvt = hStrmEvt;
	m_hFrmTrgWaitEvt = hFrmTrgWaitEvt;

	//this->CreateFFCParameters();
#endif	//CAMERA_OBJ_DISABLE
	SetCameraInited(true);
	SetCameraImageMode(CAMERA_IMAGE_BAYER);	
	//SetCameraBayerPattern(BAYER_PATTERN_GBRG);//在PreInitCamera設定
	SetCameraGrabbing_Unlock(false);
	CreateBMPInfo(m_pColorInfo, true);
	CreateBMPInfo(m_pMonoInfo, false);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::ReleaseCamera()
{
	if ( this->CheckCameraInited() == false ) { return true; }	

#ifndef CAMERA_OBJ_DISABLE	
	CloseStreamHandle(m_hStrm, m_hStrmEvt);
	CloseEventHandle(m_hEvent, m_hFrmTrgWaitEvt);
	CloseCameraHandle(m_hCam);
	CloseTeliSystem();
#endif//CAMERA_OBJ_DISABLE	
	SetCameraInited(false);
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
bool CCamera_Teli_BU1203MC::ResetCamera()//復歸相機
{
	SetCameraGrabbing_Unlock(false);
#ifndef CAMERA_OBJ_DISABLE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode)
{
	if ( this->CheckCameraInited() == false ) { return false; }	
#ifndef CAMERA_OBJ_DISABLE
	if ( m_CameraGrabMode == Mode ) { return true; }
	if ( this->StopCameraGrab() == false ) { return false; }	
	CString Err;
	Teli::CAM_HANDLE     s_hCam = GetCameraHandle();
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;	
	switch ( Mode )
	{
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		// Set hardware trigger mode.
		// Set TriggerMode true.
		uiStatus = SetCamTriggerMode(s_hCam, true);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerMode Fault (%s)"), Err);
			return false;	
		}

		// Set TriggerSequence TriggerSequence1.(exp. time with pulse widht)
		uiStatus = SetCamTriggerSequence(s_hCam, CAM_TRIGGER_SEQUENCE1);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerSequence Fault (%s)"), Err);
			return false;	
		}

		// Set TriggerSource Hardware Trigger.
		uiStatus = SetCamTriggerSource(s_hCam, CAM_TRIGGER_LINE0);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerSource Fault (%s)"), Err);
			return false;	
		}
		//
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		// Set software trigger mode.
		// Set TriggerMode true.
		uiStatus = SetCamTriggerMode(s_hCam, true);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerMode Fault (%s)"), Err);
			return false;	
		}

		// Set TriggerSequence TriggerSequence0.
		uiStatus = SetCamTriggerSequence(s_hCam, CAM_TRIGGER_SEQUENCE0);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerSequence Fault (%s)"), Err);
			return false;	
		}

		// Set TriggerSource software.
		uiStatus = SetCamTriggerSource(s_hCam, CAM_TRIGGER_SOFTWARE);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerSource Fault (%s)"), Err);
			return false;	
		}
		break;
	default://CAMERA_GRAB_FREE_RUN
		//turn off trigger mode
		uiStatus = SetCamTriggerMode(s_hCam, false);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerMode Fault (%s)"), Err);
			return false;	
		}	
		break;
	}
	this->m_CameraGrabMode = Mode;
#endif		
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CCamera_Teli_BU1203MC::SetExposureTime(const int ExposureTime)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE
	CString    Err;
	float64_t            dExp=0, dExpMin=0, dExpMax=0;	
	Teli::CAM_HANDLE     s_hCam = GetCameraHandle();
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;	

	// Get Exposure time minimum and maximum values.
    uiStatus = GetCamExposureTimeMinMax(s_hCam, &dExpMin, &dExpMax);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamExposureTimeMinMax Fault (%s)"), Err);
		return false;	
	}

	dExp = ExposureTime;//us
	if ( dExp<dExpMin || dExp>dExpMax )
	{
		m_ErrorString.Format(_T("Error, Exposure Time out of range (%.2f ~ %.2f)"), dExpMin, dExpMax);
		return false;
	}

	// Set ExposureTime.
    uiStatus = SetCamExposureTime(s_hCam, dExp);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamTriggerMode Fault (%s)"), Err);
		return false;	
	}

#ifdef _DEBUG
	float64_t       dExp2;
	// Get current ExposureTime value.
    uiStatus = GetCamExposureTime(s_hCam, &dExp2);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamExposureTime Fault (%s)"), Err);
		return false;	
	}
#endif //_DEBUG
	m_ExposureTime_us = ExposureTime;
#endif//CAMERA_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CCamera_Teli_BU1203MC::StartCameraGrab()//只取一張影像-內部觸發用
{	
#ifndef CAMERA_OBJ_DISABLE
	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();
	if ( this->DoTeliCameraStartGrab() == false )
	{	return false; }
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::StartCameraLiveGrab()//連續取多張影像-外部觸發用
{	
#ifndef CAMERA_OBJ_DISABLE
	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();
	if ( this->DoTeliCameraStartGrab() == false )
	{	return false; }
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::StopCameraGrab()//停止取像
{
	if ( this->CheckCameraInited() == false )	{	return false;	}
#ifndef CAMERA_OBJ_DISABLE
	CString Err;
	Teli::CAM_STRM_HANDLE hStrm = GetStreamHandle();
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;
	SaveCameraCurrentProcess(_T("CCamera_Teli_BU1203MC::StopCameraGrab"));
	// Stop Stream.
	uiStatus = Teli::Strm_Stop(hStrm);
    if ( GetTeliErrorString(uiStatus, m_ErrorString) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::Strm_Stop Fault (%s)"), Err);
		return false;	
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::FireSoftwareTrigger()//發射軟體觸發訊號
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	CString Err;
	Teli::CAM_HANDLE     s_hCam = GetCameraHandle();
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;

	SaveCameraCurrentProcess(_T("CCamera_Teli_BU1203MC::FireSoftwareTrigger"));
	// Send Software Trigger command.
    uiStatus = ExecuteCamSoftwareTrigger(s_hCam);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::ExecuteCamSoftwareTrigger Fault (%s)"), Err);
		return false;	
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::WaitforCameraReadytoTrigger()//等待相機準備好可以觸發
{
	if ( this->CheckCameraInited() == false ) { return false; }	
#ifndef CAMERA_OBJ_DISABLE
	DWORD Res = 0;
	DWORD WaitTime=1000;
	double CameraFPS = GetCameraFPS();
	if ( NULL == m_hFrmTrgWaitEvt ) { return true; }
	WaitTime = (DWORD)(1000.0/CameraFPS)*2;
	Res = ::WaitForSingleObject(m_hFrmTrgWaitEvt, WaitTime);	
	if ( Res != WAIT_OBJECT_0 )
	{
		::SetEvent(m_hFrmTrgWaitEvt);
		m_ErrorString.Format(_T("Error, Wait for Camera Ready to Trigger too long"));
		return false;
	}	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::ResetCameraReadyTriggerEvent()//復歸相機準備好了的事件
{
	if ( this->CheckCameraInited() == false ) { return false; }	
#ifndef CAMERA_OBJ_DISABLE	
	if ( NULL == m_hFrmTrgWaitEvt ) { return true; }	
	::ResetEvent(m_hFrmTrgWaitEvt);	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::DoCameraDebayer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const unsigned char *pRaw, unsigned char *pResult, BAYER_PATTERN_MODE BayerPattern)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::GetWhiteBalanceParams(double &WBR, double &WBG, double &WBB)
{
	if ( this->CheckCameraInited() == false ) { return false; }	
#ifndef CAMERA_OBJ_DISABLE	
	CString Err;
	float64_t            dBalanceRatioRed=0.0;
	float64_t            dBalanceRatioGrn=1.0;
	float64_t            dBalanceRatioBlu=0.0;
	Teli::CAM_HANDLE     s_hCam = GetCameraHandle();
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;	
	
    uiStatus = Teli::GetCamBalanceRatio(s_hCam, CAM_BALANCE_RATIO_SELECTOR_RED, &dBalanceRatioRed);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamBalanceRatio(Red) Fault (%s)"), Err);
		return false;	
	}
	uiStatus = Teli::GetCamBalanceRatio(s_hCam, CAM_BALANCE_RATIO_SELECTOR_BLUE, &dBalanceRatioBlu);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::GetCamBalanceRatio(Blue) Fault (%s)"), Err);
		return false;	
	}

	WBR = dBalanceRatioRed;
	WBG = dBalanceRatioGrn;
	WBB = dBalanceRatioBlu;

	m_CameraWhiteBalanceRatioRed = dBalanceRatioRed;
	m_CameraWhiteBalanceRatioBlu = dBalanceRatioBlu;
#endif//CAMERA_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::SetWhiteBalanceParams(double WBR, double WBG, double WBB)
{
	if ( this->CheckCameraInited() == false ) { return false; }	
#ifndef CAMERA_OBJ_DISABLE	
	CString Err;
	float64_t            dBalanceRatioRed=0.0;
	float64_t            dBalanceRatioGrn=1.0;
	float64_t            dBalanceRatioBlu=0.0;
	Teli::CAM_HANDLE     s_hCam = GetCameraHandle();
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;	
	
	dBalanceRatioRed = WBR;
	dBalanceRatioGrn = WBG;
	dBalanceRatioBlu = WBB;

    uiStatus = Teli::SetCamBalanceRatio(s_hCam, CAM_BALANCE_RATIO_SELECTOR_RED, dBalanceRatioRed);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamBalanceRatio(Red) Fault (%s)"), Err);
		return false;	
	}
	uiStatus = Teli::SetCamBalanceRatio(s_hCam, CAM_BALANCE_RATIO_SELECTOR_BLUE, dBalanceRatioBlu);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamBalanceRatio(Blue) Fault (%s)"), Err);
		return false;	
	}	

	m_CameraWhiteBalanceRatioRed = dBalanceRatioRed;
	m_CameraWhiteBalanceRatioBlu = dBalanceRatioBlu;
#endif//CAMERA_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::CalcWhiteBalanceParams()
{
if ( this->CheckCameraInited() == false ) { return false; }		
#ifndef CAMERA_OBJ_DISABLE
	CString Err;
	Teli::CAM_HANDLE     s_hCam = GetCameraHandle();
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;	

	CCamera_Basic::SaveCameraProcess(_T("CalcWhiteBalanceParams"), MSG_LEVEL_HIGH);
	uiStatus = Teli::SetCamBalanceWhiteAuto(s_hCam, Teli::CAM_BALANCE_WHITE_AUTO_ONCE);
	if ( GetTeliErrorString(uiStatus, Err) == false )
	{
		m_ErrorString.Format(_T("Error, Exec Teli::SetCamBalanceWhiteAuto Fault (%s)"), Err);
		return false;	
	}		
#endif//CAMERA_OBJ_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//	
bool CCamera_Teli_BU1203MC::ResetWhiteBalanceParams()
{
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CCamera_Teli_BU1203MC::WriteCameraParameterToDevice()//儲存目前相機的參數至相機內部的韌體上
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::SaveCameraINIFile()
{
	if ( CCamera_Basic::SaveCameraINIFile() == false ) { return false; }

	bool    IsOK = true;
	CString KeyName;
	CString KeyString;
	CString Section = CCamera_Basic::m_CameraModelName;
	CString FileName = CCamera_Basic::GetCameraINIFileName();

	KeyName.Format(_T("Camera Serial Number")); KeyString.Format(_T("%s"), m_CameraSerialNumber);
	if (SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Camera White Balance Ratio Red")); KeyString.Format(_T("%.4f"), m_CameraWhiteBalanceRatioRed);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		

	KeyName.Format(_T("Camera White Balance Ratio Green")); KeyString.Format(_T("%.4f"), m_CameraWhiteBalanceRatioGrn);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		

	KeyName.Format(_T("Camera White Balance Ratio Blue")); KeyString.Format(_T("%.4f"), m_CameraWhiteBalanceRatioBlu);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::LoadCameraINIFile()
{
	if ( CCamera_Basic::LoadCameraINIFile() == false ) { return false; }

	this->SetCameraModelID(CAMERA_OBJ_TELI_BU1203MC);	

	CString KeyName;
	CString KeyString;
	CString Section = CCamera_Basic::m_CameraModelName;
	CString FileName = CCamera_Basic::GetCameraINIFileName();
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];
	
	KeyName.Format(_T("Camera Serial Number")); KeyString.Format(_T("%s"), m_CameraSerialNumber);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_CameraSerialNumber = ReturnString;	}

	KeyName.Format(_T("Camera White Balance Ratio Red")); KeyString.Format(_T("%.4f"), m_CameraWhiteBalanceRatioRed);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraWhiteBalanceRatioRed = ::_ttof(ReturnString); }

	KeyName.Format(_T("Camera White Balance Ratio Green")); KeyString.Format(_T("%.4f"), m_CameraWhiteBalanceRatioGrn);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraWhiteBalanceRatioGrn = ::_ttof(ReturnString); }

	KeyName.Format(_T("Camera White Balance Ratio Blue")); KeyString.Format(_T("%.4f"), m_CameraWhiteBalanceRatioBlu);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraWhiteBalanceRatioBlu = ::_ttof(ReturnString); }
	return true;
}
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE
Teli::CAM_HANDLE CCamera_Teli_BU1203MC::GetCameraHandle()
{	
	return m_hCam;
}
//-------------------------------------------------------------------------------------//
Teli::CAM_STRM_HANDLE CCamera_Teli_BU1203MC::GetStreamHandle()
{
	return m_hStrm;
}
//-------------------------------------------------------------------------------------//
Teli::CAM_EVT_HANDLE CCamera_Teli_BU1203MC::GetEventHandle()
{
	return m_hEvent;
}
//-------------------------------------------------------------------------------------//
void CCamera_Teli_BU1203MC::CloseEventHandle(Teli::CAM_EVT_HANDLE &hEvt, HANDLE &hTrgEvt)
{
	if ( NULL == hEvt )
	{
		// Close FrameTrigger event handle.
		if (NULL != hTrgEvt)
		{
			CloseHandle(hTrgEvt);
			hTrgEvt = NULL;
		}
		return; 
	}		
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;	

	// Deactivate FrameTrigger event.
    uiStatus = Teli::Evt_Deactivate(hEvt, "FrameTriggerWait");    

	// Close FrameTrigger event handle.
	if (NULL != hTrgEvt)
	{
		CloseHandle(hTrgEvt);
		hTrgEvt = NULL;
	}

	// Close the event interface.	
	Teli::Evt_Close(hEvt);
	hEvt = NULL;
	return;
}
//-------------------------------------------------------------------------------------//
void CCamera_Teli_BU1203MC::CloseCameraHandle(Teli::CAM_HANDLE &hCam)
{
	if ( NULL == hCam ) { return; }
	
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;	

	// Close camera.    
	Teli::Cam_Close(hCam);
    hCam = NULL;
	return;
}
//-------------------------------------------------------------------------------------//
void CCamera_Teli_BU1203MC::CloseStreamHandle(Teli::CAM_STRM_HANDLE &hStrm, HANDLE &hStrmEvt)
{
	if ( NULL == hStrm )
	{ 
		// Close completion event for stream.
		if ( NULL != hStrmEvt)
		{
			CloseHandle(hStrmEvt);
			hStrmEvt = NULL;
		}
		return; 
	}
	
	Teli::CAM_API_STATUS uiStatus = Teli::CAM_API_STS_SUCCESS;	

	// Stop Stream.	
	uiStatus = Teli::Strm_Stop(hStrm);
	
	// Close stream.
	uiStatus = Teli::Strm_Close(hStrm);    
	hStrm = NULL;

    // Close completion event for stream.
    if ( NULL != hStrmEvt)
    {
		CloseHandle(hStrmEvt);
		hStrmEvt = NULL;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CCamera_Teli_BU1203MC::CloseTeliSystem()
{
	Teli::Sys_Terminate();	
	return;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Teli_BU1203MC::DoTeliCameraStartGrab()
{	
	if ( this->CheckCameraInited() == false ) { return false; }
	if ( this->StopCameraGrab() == false ) { return false; }
	this->m_CameraImagePtr = NULL;
#ifndef CAMERA_OBJ_DISABLE
	CString Err;
	Teli::CAM_HANDLE      s_hCam = GetCameraHandle();
	Teli::CAM_STRM_HANDLE hStrm= GetStreamHandle();
	Teli::CAM_API_STATUS  uiStatus = Teli::CAM_API_STS_SUCCESS;
	const bool UseCameraBuffer = GetTeliCameraBufferEnabled();
	SaveCameraCurrentProcess(_T("CCamera_Teli_BU1203MC::DoTeliCameraStartGrab"));

	if ( true == UseCameraBuffer )
	{
		uint32_t uiApiBufferCount = DEFAULT_API_BUFFER_CNT;	
		uint32_t CameraImageBufferCount = GetCameraImageBufferCount();
		uiApiBufferCount = MIN(128, CameraImageBufferCount);//記憶體緩衝張數1 ~ 128
		uiStatus = SetCamAcquisitionFrameCount(s_hCam, uiApiBufferCount);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::SetCamAcquisitionFrameCount Fault (%s)"), Err);
			return false;	
		}	
	}

	switch ( this->m_CameraGrabMode )
	{
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		// Start stream.
        uiStatus = Strm_Start(hStrm, Teli::CAM_ACQ_MODE_CONTINUOUS);
        if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::ExecuteCamSoftwareTrigger Fault (%s)"), Err);
			return false;	
		}
		SetCameraGrabbing_Unlock(true);		
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		// Start stream.
        uiStatus = Strm_Start(hStrm, Teli::CAM_ACQ_MODE_CONTINUOUS);
        if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::Strm_Start Fault (%s)"), Err);
			return false;	
		}
		SetCameraGrabbing_Unlock(true);		
		break;
	default://CAMERA_GRAB_FREE_RUN
		//turn off trigger mode
		// Start stream.
		/*
		CAM_ACQ_MODE_CONTINUOUS                 = 8,        // Continuous
        CAM_ACQ_MODE_MULTI_FRAME                = 9,        // MultiFrame
        CAM_ACQ_MODE_IMAGE_BUFFER_READ          = 10,       // Camera Image Buffer Mode
        CAM_ACQ_MODE_SINGLE_FRAME               = 109,      // SingleFrame
		*/
        //uiStatus = Strm_Start(hStrm, Teli::CAM_ACQ_MODE_CONTINUOUS);
		uiStatus = Strm_Start(hStrm, Teli::CAM_ACQ_MODE_SINGLE_FRAME);
        if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::Strm_Start Fault (%s)"), Err);
			return false;	
		}
		SetCameraGrabbing_Unlock(true);		
		break;
	}

	if ( true == UseCameraBuffer )
	{		
		uiStatus = ExecuteCamImageBufferRead(s_hCam);
		if ( GetTeliErrorString(uiStatus, Err) == false )
		{
			m_ErrorString.Format(_T("Error, Exec Teli::ExecuteCamImageBufferRead Fault (%s)"), Err);
			return false;	
		}
	}

#endif	
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CCamera_Teli_BU1203MC::GetTeliCameraBufferEnabled() const
{
	return m_TeliCameraBufferEnabled;
}
//-------------------------------------------------------------------------------------//	
bool CCamera_Teli_BU1203MC::GetTeliErrorString(Teli::CAM_API_STATUS Status, CString &err)
{
	if ( CAM_API_STS_SUCCESS == Status ) { return true; }
	
	switch ( Status )
	{
	case CAM_API_STS_NOT_INITIALIZED:// API have not been made ready by Sys_Initialize.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NOT_INITIALIZED"));
		break;
	case CAM_API_STS_ALREADY_INITIALIZED:// API is already in ready state.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_ALREADY_INITIALIZED"));
		break;
	case CAM_API_STS_NOT_FOUND:// Camera is not found.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NOT_FOUND"));
		break;
	case CAM_API_STS_ALREADY_OPENED:// Specified handle is already opened.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_ALREADY_OPENED"));
		break;
	case CAM_API_STS_ALREADY_ACTIVATED:// Specified event is already registered.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_ALREADY_ACTIVATED"));
		break;
	case CAM_API_STS_INVALID_CAMERA_INDEX:// The specified camera index was not valid.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_CAMERA_INDEX"));
		break;
	case CAM_API_STS_INVALID_CAMERA_HANDLE:// The specified camera handle was not valid.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_CAMERA_HANDLE"));
		break;
	case CAM_API_STS_INVALID_NODE_HANDLE:// The specified node handle was not valid.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_NODE_HANDLE"));
		break;
	case CAM_API_STS_INVALID_STREAM_HANDLE:// The specified stream handle was not valid.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_STREAM_HANDLE"));
		break;
	case CAM_API_STS_INVALID_REQUEST_HANDLE:// The specified buffer handle was not valid.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_REQUEST_HANDLE"));
		break;
	case CAM_API_STS_INVALID_EVENT_HANDLE:// The specified event handle was not valid.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_EVENT_HANDLE"));
		break;
		// The specified image handle was not valid.
//      CAM_API_STS_INVALID_IMAGE_HANDLE     = 0x0000000C,


	case CAM_API_STS_INVALID_PARAMETER:// The specified parameter was not valid.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_PARAMETER"));
		break;
	case CAM_API_STS_BUFFER_TOO_SMALL:// Buffer size specified by user was too small to complete the request.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_BUFFER_TOO_SMALL"));
		break;
	case CAM_API_STS_NO_MEMORY:// Request cannot be completed because insufficient memory resource.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NO_MEMORY"));
		break;
	case CAM_API_STS_MEMORY_NO_ACCESS:// Memory location specified by user was not valid.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_MEMORY_NO_ACCESS"));
		break;
	case CAM_API_STS_NOT_IMPLEMENTED:// Feature is not implemented in the camera or API.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NOT_IMPLEMENTED"));
		break;        
	case CAM_API_STS_TIMEOUT:// Timeout expired.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_TIMEOUT"));
		break;
	case CAM_API_STS_CAMERA_NOT_RESPONDING:// The opened camera may be lost. You should re-enum the camera to confirm whether it exist.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_CAMERA_NOT_RESPONDING"));
		break;
	case CAM_API_STS_EMPTY_COMPLETE_QUEUE:// Stream or event "get request" is failed because there is no complete queue.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_EMPTY_COMPLETE_QUEUE"));
		break;
	case CAM_API_STS_NOT_READY:// Information is not yet ready.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NOT_READY"));
		break;
	case CAM_API_STS_ACCESS_MODE_SET_ERR:// Failed to set the access mode.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_ACCESS_MODE_SET_ERR"));
		break;
	case CAM_API_STS_IO_DEVICE_ERROR:// Controller caused an I/O error.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_IO_DEVICE_ERROR"));
		break;
	case CAM_API_STS_LOGICAL_PARAM_ERROR:// Passed parameters have logical error(s).
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_LOGICAL_PARAM_ERROR"));
		break;
	case CAM_API_STS_XML_LOAD_ERR:// Failed to load the XML.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_XML_LOAD_ERR"));
		break;
	case CAM_API_STS_GENICAM_ERR:// GenICam error occurred.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_GENICAM_ERR"));
		break;
	case CAM_API_STS_DLL_LOAD_ERR:// Failed to load the dll file.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_DLL_LOAD_ERR"));
		break;
	case CAM_API_STS_NO_SYSTEM_RESOURCES:// Insufficient system resources exist to complete the requested service.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NO_SYSTEM_RESOURCES"));
		break;
	case CAM_API_STS_INVALID_ADDRESS: // Attempt to access a not existing register address.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_ADDRESS"));
		break;
	case CAM_API_STS_WRITE_PROTECT:// Attempt to write to a read only register.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_WRITE_PROTECT"));
		break;
	case CAM_API_STS_BAD_ALIGNMENT:// Access registers with an address which is not aligned according to the underlying technology.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_BAD_ALIGNMENT"));
		break;
	case CAM_API_STS_ACCESS_DENIED:// Read a non-readable or write a non-writable register address.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_ACCESS_DENIED"));
		break;
	case CAM_API_STS_BUSY:// Camera is currently busy.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_BUSY"));
		break;
	case CAM_API_STS_NOT_READABLE:// Not readable.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NOT_READABLE"));
		break;
	case CAM_API_STS_NOT_WRITABLE:// Not writable.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NOT_WRITABLE"));
		break;
	case CAM_API_STS_NOT_AVAILABLE:// Function or the camera of the register is not currently available.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_NOT_AVAILABLE"));
		break;
	case CAM_API_STS_VERIFY_ERR:// Verify error occurred.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_VERIFY_ERR"));
		break;
	case CAM_API_STS_REQUEST_TIMEOUT:// User request had not be able to complete while user specified timeoout limit.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_REQUEST_TIMEOUT"));
		break;
	case CAM_API_STS_RESEND_TIMEOUT:// Stream resend request timeoout.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_RESEND_TIMEOUT"));
		break;
	case CAM_API_STS_RESPONSE_TIMEOUT:// Stream resend data receive timeoout.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_RESPONSE_TIMEOUT"));
		break;
	case CAM_API_STS_BUFFER_FULL:// Transferred frame data was larger than user specified max payload size.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_BUFFER_FULL"));
		break;
	case CAM_API_STS_UNEXPECTED_BUFFER_SIZE:// Actual received payload size was not equal to the size notified by Trailer.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_UNEXPECTED_BUFFER_SIZE"));
		break;
	case CAM_API_STS_UNEXPECTED_NUMBER:// Exceeded the MAX number of packets that is realized by Leader or Trailer.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_UNEXPECTED_NUMBER"));
		break;
	case CAM_API_STS_PACKET_STATUS_ERROR:// Any error was notified by stream or event status on transaction header.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_PACKET_STATUS_ERROR"));
		break;
	case CAM_API_STS_RESEND_NOT_IMPLEMENTED:// Packet resend command is not supported by the camera.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_RESEND_NOT_IMPLEMENTED"));
		break;
	case CAM_API_STS_PACKET_UNAVAILABLE:// The requested packet is not available anymore.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_PACKET_UNAVAILABLE"));
		break;
	// Frame data is terminated with next Leader or Trailer's BlockId is different from Leader's.
	case CAM_API_STS_MISSING_PACKETS:// Some packets may be lost.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_MISSING_PACKETS"));
		break;
	case CAM_API_STS_FLUSH_REQUESTED:// Requests were flushed by user request.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_FLUSH_REQUESTED"));
		break;
	case CAM_API_STS_TOO_MANY_PACKET_MISSING:// The loss of packet exceeded the specified value.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_TOO_MANY_PACKET_MISSING"));
		break;
	case CAM_API_STS_FLUSHED_BY_D0EXIT:// Requests were flushed because power state was chaned into save mode.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_FLUSHED_BY_D0EXIT"));
		break;
	case CAM_API_STS_FLUSHED_BY_CAMERA_REMOVE:// Requests were flushed by camera remove event.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_FLUSHED_BY_CAMERA_REMOVE"));
		break;
	case CAM_API_STS_DRIVER_LOAD_ERR:// Failed to load the driver.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_DRIVER_LOAD_ERR"));
		break;
		// Mapping user buffer to system-space virtual address is failed.
	case CAM_API_STS_MAPPING_ERROR:// It may be caused by low system resources.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_MAPPING_ERROR"));
		break;
	case CAM_API_STS_FILE_OPEN_ERROR:// Failed to open file.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_FILE_OPEN_ERROR"));
		break;
	case CAM_API_STS_FILE_WRITE_ERROR:// Failed to write data to file.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_FILE_WRITE_ERROR"));
		break;
	case CAM_API_STS_FILE_READ_ERROR:// Failed to read data from file.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_FILE_READ_ERROR"));
		break;
	case CAM_API_STS_FILE_NOT_FOUND:// Failed to find file.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_FILE_NOT_FOUND"));
		break;
	case CAM_API_STS_INVALID_PARAMETER_FROM_CAM:// At least one command parameter of CCD or SCD is invalid or out of range. 
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_INVALID_PARAMETER_FROM_CAM"));
		break;
		// The value written to the SI streaming size registers is not aligned		
	case CAM_API_STS_SI_PAYLOAD_SIZE_NOT_ALIGNED:// to Payload Size Alignment value of the SI Info register.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_SI_PAYLOAD_SIZE_NOT_ALIGNED"));
		break;
	case CAM_API_STS_DATA_DISCARDED:// Some data in the block has been discarded.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_DATA_DISCARDED"));
		break;
	case CAM_API_STS_DATA_OVERRUN:// The Camera cannot send all data because the data does not fit within the programmed SIRM register settings.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_DATA_OVERRUN"));
		break;
	case CAM_API_STS_UNSUCCESSFUL:// Unspecified error occurred.
		err.Format(_T("Teli::%s."), _T("CAM_API_STS_UNSUCCESSFUL"));
		break;
	default:
		err.Format(_T("Teli::%s(%d)."), _T("CAM_API_STS_NO_DEFINED"), Status);
		break;
	}   
	return false;
}
//-------------------------------------------------------------------------------------//
#endif//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
#endif//TELI_BU1203MC_USE
//-------------------------------------------------------------------------------------//	