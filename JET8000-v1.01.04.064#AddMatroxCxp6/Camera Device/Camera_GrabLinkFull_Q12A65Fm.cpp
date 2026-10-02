// Camera_GrabLinkFull_Q12A65Fm.cpp: implementation of the CCamera_GrabLinkFull_Q12A65Fm class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//	
#include "stdafx.h"
#include "Camera_GrabLinkFull_Q12A65Fm.h"
//-------------------------------------------------------------------------------------//	
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//	
#ifdef GRABLINKFULL_Q12A65FM_USE
//-------------------------------------------------------------------------------------//	
#define STX_CHAR	0x40
#define ETX_CHAR	0x0D
#define ACK_CHAR	0x06
#define NAK_CHAR	0x15
//-------------------------------------------------------------------------------------//	
CCamera_GrabLinkFull_Q12A65Fm Camera_GrabLinkFull_Q12A65Fm;
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE
void WINAPI GrabLinkFull_Callback(PMCSIGNALINFO SigInfo)
{
	if ( SigInfo == NULL ) { return; }

	int Flag = 0;
	switch ( SigInfo->Signal )
	{
	case MC_SIG_ANY:
		Flag = MC_SIG_ANY;
		break;
	case MC_SIG_SURFACE_PROCESSING:
		Flag = MC_SIG_SURFACE_PROCESSING;
		::GrabLinkFull_FrameCallback(SigInfo);
		break;
	case MC_SIG_SURFACE_FILLED:
		Flag = MC_SIG_SURFACE_FILLED;
		//::GrabLinkFull_FrameCallback(SigInfo);
		break;	
	case MC_SIG_UNRECOVERABLE_OVERRUN:	
		::AfxMessageBox(_T("Error, MC_SIG_UNRECOVERABLE_OVERRUN"));
		Flag = MC_SIG_UNRECOVERABLE_OVERRUN;
		::GrabLinkFull_ExceptionCallback(SigInfo);
		break;
	case MC_SIG_FRAMETRIGGER_VIOLATION:	
		::AfxMessageBox(_T("Error, MC_SIG_FRAMETRIGGER_VIOLATION"));
		Flag = MC_SIG_FRAMETRIGGER_VIOLATION;
		::GrabLinkFull_ExceptionCallback(SigInfo);
		break;
	case MC_SIG_START_EXPOSURE:
		Flag = MC_SIG_START_EXPOSURE;
		break;
	case MC_SIG_END_EXPOSURE:
		Flag = MC_SIG_END_EXPOSURE;
		::GrabLinkFull_ExposureEndCallback(SigInfo);
		break;
	case MC_SIG_ACQUISITION_FAILURE:
		::AfxMessageBox(_T("Error, MC_SIG_ACQUISITION_FAILURE"));
		Flag = MC_SIG_ACQUISITION_FAILURE;
		::GrabLinkFull_ExceptionCallback(SigInfo);
		break;
	case MC_SIG_CLUSTER_UNAVAILABLE:
		::AfxMessageBox(_T("Error, MC_SIG_CLUSTER_UNAVAILABLE"));
		Flag = MC_SIG_CLUSTER_UNAVAILABLE;
		::GrabLinkFull_ExceptionCallback(SigInfo);
		break;
	case MC_SIG_RELEASE:
		Flag = MC_SIG_RELEASE;		
		break;
	case MC_SIG_END_ACQUISITION_SEQUENCE:
		Flag = MC_SIG_END_ACQUISITION_SEQUENCE;		
		break;
	case MC_SIG_START_ACQUISITION_SEQUENCE:
		Flag = MC_SIG_START_ACQUISITION_SEQUENCE;		
		break;
	case MC_SIG_END_CHANNEL_ACTIVITY:
		Flag = MC_SIG_END_CHANNEL_ACTIVITY;		
		break;
	default:
		::AfxMessageBox(_T("Error, MC_SIG_NOT_DEFINED"));
		Flag = SigInfo->Signal;
		::GrabLinkFull_ExceptionCallback(SigInfo);
		break;
	}			
}
//------------------------------------------------------------------------------//
void WINAPI GrabLinkFull_FrameCallback(PMCSIGNALINFO SigInfo)
{
	CString fnName = "GrabLinkFull_FrameCallback";
	if ( SigInfo == NULL ) { return; }
	if ( SigInfo->Context == NULL ) { return; }

	PVOID m_pCurrent = NULL;	
	CCamera_GrabLinkFull_Q12A65Fm *This =reinterpret_cast<CCamera_GrabLinkFull_Q12A65Fm*>(SigInfo->Context);	
	This->IncrementCountForCameraCallback();
	McGetParamPtr(SigInfo->SignalInfo, MC_SurfaceAddr, &m_pCurrent); //避免64位元溢位, 使用指標函式
	if ( m_pCurrent == NULL ) { return; }

	MCHANDLE hSurface = NULL;
	hSurface = (MCHANDLE)SigInfo->SignalInfo;
	McSetParamInt(hSurface,MC_SurfaceState,MC_SurfaceState_FREE);	
	unsigned char *pSrc = (unsigned char*)m_pCurrent;
	if ( This->AddCameraRingBufferList(pSrc) == true )
	{					
		/*
		THREAD_GRAB_MODE  ThreadGrabMode = AOIDataCollect.GetThreadGrabMode();
		CAMERA_CALLBACK_TIMMING CallbackTimming = This->GetCameraCallbackTimming();
		const long cntBatchGrab = This->GetCountForBatchGrab();
		const long cntImageBack = This->GetCountForImageCallback();
		HWND hWndCallback = AOIDataCollect.GetCallbackWnd();
		WPARAM wParam = This->GetCameraWParam();
		if ( NULL != hWndCallback )
		{				
			if ( CAMERA_CALLBACK_FREE_FRAME == CallbackTimming )
			{
				This->SetCameraGrabFinishEvent();				
				if ( THREAD_GRAB_NONE==ThreadGrabMode && This->GetCameraToSendCallback()==TRUE )
				{	::PostMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK); }
			}
			else if ( CAMERA_CALLBACK_BATCH_GRAB_DONE==CallbackTimming )
			{
				if ( cntBatchGrab<0 || (cntImageBack%cntBatchGrab) == 0 )
				{
					This->SetCameraGrabFinishEvent();
					if ( THREAD_GRAB_NONE == ThreadGrabMode )
					{
						//::SendMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK); 
						::PostMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK); 
					}
				}
				else
				{	
					if ( THREAD_GRAB_NONE == ThreadGrabMode )
					{
						//::SendMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_BYPASS_CALLBACK); 
						::PostMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_BYPASS_CALLBACK); 
					}
				}
			}
			else //CAMERA_CALLBACK_EACH_FRAME
			{
				if ( THREAD_GRAB_NONE == ThreadGrabMode )
				{
					This->SetCameraGrabFinishEvent();
					//::SendMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK);	
					::PostMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK);	
				}
			}			
		}		
		*/
	}
	return ;
}
//------------------------------------------------------------------------------//
void WINAPI GrabLinkFull_ExposureEndCallback(PMCSIGNALINFO SigInfo)
{	
	CCamera_GrabLinkFull_Q12A65Fm *This =reinterpret_cast<CCamera_GrabLinkFull_Q12A65Fm*>(SigInfo->Context);		
	This->IncrementCountForCameraExposuredEnd();
	This->CheckCameraExposuredEndEvent();
}
//------------------------------------------------------------------------------//
void WINAPI GrabLinkFull_ExceptionCallback(PMCSIGNALINFO SigInfo)
{
	if ( SigInfo == NULL ) { return; }
	if ( SigInfo->Context == NULL ) { return; }
	CCamera_GrabLinkFull_Q12A65Fm *This =reinterpret_cast<CCamera_GrabLinkFull_Q12A65Fm*>(SigInfo->Context);
	This->SetCameraErrorString(_T("Error, Camera Callback Exception"));	
	WPARAM wParam = This->GetCameraWParam();
	AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, wParam, LPARAM_SYSTEM_EXCEPTION_CAMERA);
}
//------------------------------------------------------------------------------//
#endif//CAMERA_OBJ_DISABLE
//------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CCamera_GrabLinkFull_Q12A65Fm, CCamera_Basic)
//-------------------------------------------------------------------------------------//
CCamera_GrabLinkFull_Q12A65Fm::CCamera_GrabLinkFull_Q12A65Fm()
{
	PreInitCamera();
	SetCameraConstructed(true);
}
//------------------------------------------------------------------------------//
CCamera_GrabLinkFull_Q12A65Fm::~CCamera_GrabLinkFull_Q12A65Fm()
{
	ReleaseCamera();
}
//------------------------------------------------------------------------------//
void CCamera_GrabLinkFull_Q12A65Fm::PreInitCamera()
{
	this->m_CameraModelID = CAMERA_OBJ_GRABLINKFULL_Q12A65FM;
	this->m_CameraModelName = _T("GrabLinkFull_Q12A65Fm");

	this->m_PeriodTim_us = 16000;//16ms
	this->m_ExposureTime_us = 10000;//3ms
	this->m_ExposureTimeMin_us = 20;//曝光時間-Min
	this->m_ExposureTimeMax_us = 1000000;//曝光時間-Max
	this->m_TriggerDelay = 500;//處發延遲時間-us

	this->m_CameraFPS = 65.0;
	this->m_CameraBitCount = 8;
	this->m_CameraImageStep = 4072;//相機間距
	this->m_CameraImageW = 4072;//相機影像寬度
	this->m_CameraImageH = 3072;//相機影像高度
	this->m_CameraSizeRaw = m_CameraImageW*m_CameraImageH;
	this->m_CameraSizeColor = this->m_CameraSizeRaw*3;

#ifndef CAMERA_OBJ_DISABLE
	this->m_SerialPort = 20;
	this->m_CameraChannel = NULL;
#endif//CAMERA_OBJ_DISABLE
}
//------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::InitialCamera()
{
	const char fnName[] = "CCamera_GrabLinkFull_Q12A65Fm::InitialCamera";
	char string[128]={0};

	this->ReleaseCamera();
	if ( CCamera_GrabLinkFull_Q12A65Fm::LoadCameraINIFile() == false ) { return false; }
	if ( CCamera_GrabLinkFull_Q12A65Fm::SaveCameraINIFile() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE
//	if ( this->Initialization_acA2040kc() == false )
//	{	return false;	}

	CString str;
	CString strCAMFile;
	MCSTATUS Status = MC_OK ;
	char GrabLinkFullLog[1024]="";
	char CAMFile[1024]="";	

	::sprintf(CAMFile, "%s", "C:\\JETAOI3D\\Camera\\acA2040-180kc_P180RG.cam");	
	::sprintf(CAMFile, "%s\\%s\\%s", AOIDataCollect.GetAOIDirectoryA(), "Camera", "Q-12A65_P65RG.cam");
	::sprintf(GrabLinkFullLog, "%s\\%s", AOIDataCollect.GetAOIDirectoryA(), "GrabLinkFullError.log");	
	::DeleteFileA(GrabLinkFullLog);

	Status = ::McOpenDriver(NULL);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	return false; }
	
#ifdef _DEBUG
	Status = ::McSetParamInt(MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_MSGBOX);
#else
	Status = ::McSetParamInt(MC_CONFIGURATION, MC_ErrorHandling, MC_ErrorHandling_NONE);
#endif
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McCloseDriver();
		return false; 
	}

	Status = ::McSetParamStr(MC_CONFIGURATION, MC_ErrorLog, GrabLinkFullLog);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McCloseDriver();
		return false; 
	}

	//判斷GrabLink Full卡數
	int BoardCount=0;
	Status = McGetParamInt(MC_CONFIGURATION, MC_BoardCount, &BoardCount);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McCloseDriver();
		return false; 
	}
	if ( BoardCount != 1 )
	{
		this->m_ErrorString.Format(_T("Error, GrabLink Full board count is exception (%d)"), BoardCount);
		McCloseDriver();
		return false;
	}

	//判斷卡的型號
	int BoardType = 0;
	Status = McGetParamInt(MC_BOARD, MC_BoardType, &BoardType);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McCloseDriver();
		return false; 
	}

	if ( BoardType != MC_BoardType_GRABLINK_FULL )	
	{
		this->m_ErrorString.Format(_T("Error, Framegrabber board type is exception (%d)"), BoardType);
		McCloseDriver();
		return false;
	}

	// In order to support a 10-tap camera on Grablink Full
	// BoardTopology need to be set to MC_BoardTopology_MONO_DECA
	// In order to support 2 cameras on Grablink Expert 2
	// BoardTopology need to be set to MC_BoardTopology_DUO
	// In all other case the default value will work properly 
	// and the parameter don't need to be set

    // Set the board topology to support 2 cameras (only with a Grablink Expert 2)
    // McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_DUO);

	// Set the board topology to support 10 taps mode (only with a Grablink Full)
    // McSetParamInt(MC_BOARD + 0, MC_BoardTopology, MC_BoardTopology_MONO_DECA);

	int BoardTopology = MC_BoardTopology_MONO_DECA;
    Status = McSetParamInt(MC_BOARD+0, MC_BoardTopology, BoardTopology); 
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McCloseDriver();
		return false; 
	}

	// Create a channel and associate it with the first connector on the first board
    Status = McCreate(MC_CHANNEL, &m_CameraChannel);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McCloseDriver();
		return false; 
	}
	
    Status = McSetParamInt(m_CameraChannel, MC_DriverIndex, 0);//MC_CONFIGURATION	
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	// In order to use single camera on connector A
	// MC_Connector need to be set to A for Grablink Expert 2 and Grablink DualBase
	// For all the other Grablink boards the parameter has to be set to M 
	Status = McSetParamStr(m_CameraChannel, MC_Connector, "M");
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	//Load Camera CAM File
	Status = McSetParamStr(m_CameraChannel, MC_CamFile, CAMFile);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		str = m_ErrorString;
		strCAMFile = CAMFile;
		m_ErrorString.Format(_T("%s\n%s"), str, strCAMFile);
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}
	
	//設定ComPort
	//Status = McSetParamStr(MC_BOARD+1, MC_SerialControlA, "COM20");
	char ComName[64] ="";
	::sprintf(ComName, "COM%d", this->m_SerialPort);
	Status = McSetParamStr(MC_BOARD+0, MC_SerialControlA, ComName);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	//turn off camea grabbing first
	Status = McSetParamInt(m_CameraChannel,MC_ChannelState, MC_ChannelState_IDLE);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	//Tri it under x64 System
//	int SurfaceAllocation = 0;
  //  Status = McGetParamInt(m_CameraChannel, MC_SurfaceAllocation, &SurfaceAllocation);
//	this->CheckMCResult(Status);
//	switch ( SurfaceAllocation )
//	{
//	case MC_SurfaceAllocation_ANYWHERE: 
//		SurfaceAllocation = SurfaceAllocation; break;
//	case MC_SurfaceAllocation_BELOW4G: 
//		SurfaceAllocation = SurfaceAllocation; break;	
//	}	

	int ImageSizeW = (int)(this->GetCameraImageW());
	int ImageSizeH = (int)(this->GetCameraImageH());
	#if ( CAMERA_ROTATION_MODE==CAMERA_ROTATION_090 || CAMERA_ROTATION_MODE==CAMERA_ROTATION_270 || CAMERA_ROTATION_MODE==CAMERA_ROTATION_090_YMIRROR || CAMERA_ROTATION_MODE==CAMERA_ROTATION_270_YMIRROR )
	//	ImageSizeW = (int)(this->GetCameraImageH());
	//	ImageSizeH = (int)(this->GetCameraImageW());
	#endif

	int ImageSizeW2=0;
	int ImageSizeH2=0;

	// Set image dimensions
    Status = McSetParamInt(m_CameraChannel, MC_ImageSizeX, ImageSizeW);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}
	Status = McGetParamInt(m_CameraChannel, MC_ImageSizeX, &ImageSizeW2);
	if ( ImageSizeW2 != ImageSizeW )
	{
		this->m_ErrorString.Format(_T("Error, Cam File Exception (Image Width::%d, %s)"), ImageSizeW2, CString(CAMFile));
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

    Status = McSetParamInt(m_CameraChannel, MC_ImageSizeY, ImageSizeH);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}
	Status = McGetParamInt(m_CameraChannel, MC_ImageSizeY, &ImageSizeH2);
	if ( ImageSizeH2 != ImageSizeH )
	{
		this->m_ErrorString.Format(_T("Error, Cam File Exception (Image Height::%d, %s)"), ImageSizeH2, CString(CAMFile));
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}
	
	int  m_BufferPitch = 0;
	Status = McGetParamInt(m_CameraChannel, MC_BufferPitch, &m_BufferPitch);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}    

    // The memory allocation for the images is automatically done by Multicam when activating the channel.
    // We only set the number of surfaces to be created by MultiCam.
	const int c_nImageBuffers = GetCameraImageBufferCount();//64
    Status = McSetParamInt(m_CameraChannel, MC_SurfaceCount, c_nImageBuffers);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}    
    
	int MinBufferSize = 0;
    Status = McGetParamInt(m_CameraChannel, MC_MinBufferSize, &MinBufferSize);

    // Choose the pixel color format
 // McSetParamInt(m_Channel, MC_ColorFormat, MC_ColorFormat_Y8);

	//Setting Hardware trigger 
	Status = McSetParamInt(m_CameraChannel, MC_TrigCtl, MC_TrigCtl_ISO);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	Status = McSetParamInt(m_CameraChannel, MC_TrigLine, MC_TrigLine_IIN1);//MC_TrigLine_NOM, MC_TrigLine_IIN1
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	//setting trigger for rising edge mode
	//Status = McSetParamInt(m_CameraChannel,MC_TrigEdge, MC_TrigEdge_GOHIGH);
	Status = McSetParamInt(m_CameraChannel,MC_TrigEdge, MC_TrigEdge_GOLOW);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	Status = McSetParamInt(m_CameraChannel,MC_TrigFilter, MC_TrigFilter_MEDIUM);//MC_TrigFilter_ON
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}
	
	//曝光時間
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();
	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6:
	case LIGHT_CTRL_BOARD_8DA1:
	case LIGHT_CTRL_BOARD_ARDUINO:
		//Status = McSetParamInt(m_CameraChannel, MC_Expose, MC_Expose_INTCTL);//由卡上控制相機的曝光時間
		Status = McSetParamInt(m_CameraChannel, MC_Expose, MC_Expose_PLSTRG);//由卡上控制相機的曝光時間	
		break;
	default:
		Status = McSetParamInt(m_CameraChannel, MC_Expose, MC_Expose_WIDTH);//由卡上控制相機的曝光時間
		break;
	}
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}	

	Status = McSetParamInt(m_CameraChannel, MC_ExposeMin_us, m_ExposureTimeMin_us);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	Status = McSetParamInt(m_CameraChannel, MC_ExposeMax_us, m_ExposureTimeMax_us);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}	

	//觸發延遲 - m_TriggerDelay
#ifdef LIGHT_CTRL_DISABLE
	Status = McSetParamInt(m_CameraChannel, MC_TrigDelay_us, m_TriggerDelay);
#else
	Status = McSetParamInt(m_CameraChannel, MC_TrigDelay_us, 0);
#endif//LIGHT_CTRL_DISABLE
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}	

	Status = McSetParamInt(m_CameraChannel, MC_AcquisitionMode, MC_AcquisitionMode_SNAPSHOT);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}	
	
	//Strobe Out
	//McSetParamInt(m_CameraChannel, MC_OutputFunction, MC_OutputFunction_FREE);
    //McSetParamInt(m_CameraChannel, MC_OutputConfig, MC_OutputConfig_FREE);
    Status = McSetParamInt(m_CameraChannel, MC_StrobeCtl, MC_StrobeCtl_OPTO);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

    Status = McSetParamInt(m_CameraChannel, MC_StrobeMode, MC_StrobeMode_NONE);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

    Status = McSetParamInt(m_CameraChannel, MC_StrobeDur, 50);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

    Status = McSetParamInt(m_CameraChannel, MC_StrobePos, 50);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}
	
	//取像逾時-MC_AcqTimeout_ms
	//Status = McSetParamInt(m_CameraChannel, MC_AcqTimeout_ms, MC_INFINITE);
	Status = McSetParamInt(m_CameraChannel, MC_AcqTimeout_ms, 1000);//10 sec
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

    // Enable MultiCam signals
    Status = McSetParamInt(m_CameraChannel, MC_SignalEnable + MC_SIG_END_EXPOSURE, MC_SignalEnable_ON);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{		
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	Status = McSetParamInt(m_CameraChannel, MC_SignalEnable + MC_SIG_SURFACE_PROCESSING, MC_SignalEnable_ON);	
    //Status = McSetParamInt(m_CameraChannel, MC_SignalEnable + MC_SIG_SURFACE_FILLED, MC_SignalEnable_ON);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

    Status = McSetParamInt(m_CameraChannel, MC_SignalEnable + MC_SIG_ACQUISITION_FAILURE, MC_SignalEnable_ON);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	//Status = McSetParamInt(m_CameraChannel, MC_SignalEnable + MC_SIG_END_CHANNEL_ACTIVITY, MC_SignalEnable_ON);	
	
	// Register the callback function
    Status = McRegisterCallback(m_CameraChannel, GrabLinkFull_Callback, this);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}

	// Enable Multicam Signal Using Wait Signal	
    //Status = McSetParamInt(m_CameraChannel, MC_SignalHandling + MC_SIG_SURFACE_FILLED, MC_SignalHandling_WAITING_SIGNALING);	
    //Status = McSetParamInt(m_CameraChannel, MC_SignalHandling + MC_SIG_ACQUISITION_FAILURE, MC_SignalHandling_WAITING_SIGNALING);	
    //Status = McSetParamInt(m_CameraChannel, MC_SignalHandling + MC_SIG_END_CHANNEL_ACTIVITY, MC_SignalHandling_WAITING_SIGNALING);

	//連到實體相機控制
	if ( this->RS232_ConnectCamera(this->m_SerialPort) == false )
	{
		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
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

		McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
		McCloseDriver();
		return false;
	}
	::memset(m_ClonedRawImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH);	
	//::memset(m_ClonedColorImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH*4);

	//this->CreateFFCParameters();
#endif	//CAMERA_OBJ_DISABLE
	SetCameraInited(true);
	SetCameraImageMode(CAMERA_IMAGE_GRAY);
	SetCameraBayerPattern(BAYER_PATTERN_NONE);	
	SetCameraGrabbing_Unlock(false);
	CreateBMPInfo(m_pColorInfo, true);
	CreateBMPInfo(m_pMonoInfo, false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::ReleaseCamera()
{
	if ( this->CheckCameraInited() == false ) { return true; }	

#ifndef CAMERA_OBJ_DISABLE
	m_CameraSerial.Close();
	if ( NULL != m_CameraChannel )
	{
		this->StopCameraGrab();

		// Set the channel to IDLE before deleting it
		::McSetParamInt(m_CameraChannel, MC_ChannelState, MC_ChannelState_IDLE);

		::McDelete(m_CameraChannel);
		m_CameraChannel = NULL;
	}
	::McCloseDriver();	
#endif//CAMERA_OBJ_DISABLE
	//this->ReleaseCamera_acA2040kc();
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
bool CCamera_GrabLinkFull_Q12A65Fm::ResetCamera()//復歸相機
{
	SetCameraGrabbing_Unlock(false);
#ifndef CAMERA_OBJ_DISABLE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode)
{
	if ( this->CheckCameraInited() == false ) { return false; }	
#ifndef CAMERA_OBJ_DISABLE
	if ( m_CameraGrabMode == Mode ) { return true; }
	//if ( this->StopCameraGrab() == false ) { return false; }	
	MCSTATUS Status = MC_OK;	
	switch ( Mode )
	{
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		//turn ON trigger mode
		Status = McSetParamInt(m_CameraChannel, MC_TrigMode, MC_TrigMode_HARD);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		Status = McSetParamInt(m_CameraChannel, MC_NextTrigMode, MC_NextTrigMode_SAME);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}
/*
		//Setting Hardware trigger 
		Status = McSetParamInt(m_CameraChannel, MC_TrigCtl, MC_TrigCtl_ISO);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		//setting trigger for rising edge mode
		Status = McSetParamInt(m_CameraChannel, MC_TrigLine, MC_TrigLine_IIN1);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		//setting trigger for rising edge mode
		Status = McSetParamInt(m_CameraChannel,MC_TrigEdge, MC_TrigEdge_GOHIGH);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		Status = McSetParamInt(m_CameraChannel,MC_TrigFilter, MC_TrigFilter_ON);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}
//*/		//
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		//turn ON trigger mode
		Status = McSetParamInt(m_CameraChannel, MC_TrigMode, MC_TrigMode_SOFT);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		Status = McSetParamInt(m_CameraChannel, MC_NextTrigMode, MC_NextTrigMode_SAME);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}
		break;
	default://CAMERA_GRAB_FREE_RUN
		//turn off trigger mode
		Status = McSetParamInt(m_CameraChannel, MC_TrigMode, MC_TrigMode_IMMEDIATE);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}		
		break;
	}
	this->m_CameraGrabMode = Mode;
#endif		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::SetExposureTime(const int ExposureTime)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE
	MCSTATUS Status = MC_OK;	
	//setting exposure time	
	Status = McSetParamInt(m_CameraChannel, MC_Expose_us, ExposureTime);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	return false;	}

#ifdef _DEBUG
	int m_ExposureTime2 = 0;
	Status = McGetParamInt(m_CameraChannel, MC_Expose_us, &m_ExposureTime2);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	return false;	}
#endif //_DEBUG

	m_ExposureTime_us = ExposureTime;
#endif//CAMERA_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::DoGrabLinkFullStartGrab()
{	
	if ( this->CheckCameraInited() == false ) { return false; }
	if ( this->StopCameraGrab() == false ) { return false; }
	this->m_CameraImagePtr = NULL;
#ifndef CAMERA_OBJ_DISABLE
	MCSTATUS Status = MC_OK;	
	switch ( this->m_CameraGrabMode )
	{
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		//turn ON trigger mode
		/*
		Status = McSetParamInt(m_CameraChannel, MC_TrigMode, MC_TrigMode_HARD);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		Status = McSetParamInt(m_CameraChannel, MC_NextTrigMode, MC_NextTrigMode_SAME);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}
		*/
		Status = McSetParamInt(m_CameraChannel, MC_SeqLength_Fr, MC_INDETERMINATE);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		Status = McSetParamInt(m_CameraChannel, MC_ChannelState, MC_ChannelState_ACTIVE);
		SetCameraGrabbing_Unlock(true);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		//turn ON trigger mode
		/*
		Status = McSetParamInt(m_CameraChannel, MC_TrigMode, MC_TrigMode_SOFT);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		Status = McSetParamInt(m_CameraChannel, MC_NextTrigMode, MC_NextTrigMode_SAME);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}
		*/
		Status = McSetParamInt(m_CameraChannel, MC_SeqLength_Fr, MC_INDETERMINATE);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		Status = McSetParamInt(m_CameraChannel, MC_ChannelState, MC_ChannelState_ACTIVE);
		SetCameraGrabbing_Unlock(true);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}		
		break;
	default://CAMERA_GRAB_FREE_RUN
		//turn off trigger mode
		/*
		Status = McSetParamInt(m_CameraChannel, MC_TrigMode, MC_TrigMode_IMMEDIATE);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}		
		*/
		if ( this->m_CountForFramesToGrab == -1 )
		{	Status = McSetParamInt(m_CameraChannel, MC_SeqLength_Fr, MC_INDETERMINATE);	}
		else
		{	Status = McSetParamInt(m_CameraChannel, MC_SeqLength_Fr, m_CountForFramesToGrab);	}
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}

		Status = McSetParamInt(m_CameraChannel, MC_ChannelState, MC_ChannelState_ACTIVE);
		SetCameraGrabbing_Unlock(true);
		if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
		{	return false;	}
		break;
	}
#endif	
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::StartCameraGrab()//只取一張影像-內部觸發用
{	
	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();
	if ( this->DoGrabLinkFullStartGrab() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::StartCameraLiveGrab()//連續取多張影像-外部觸發用
{	
	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();
	if ( this->DoGrabLinkFullStartGrab() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::StopCameraGrab()//停止取像
{
	if ( this->CheckCameraInited() == false )	{	return false;	}
#ifndef CAMERA_OBJ_DISABLE
	MCSTATUS Status = MC_OK;
	
	// Stop an acquisition sequence by deactivating the channel
	Status = McSetParamInt(m_CameraChannel,MC_ChannelState, MC_ChannelState_IDLE);
	SetCameraGrabbing_Unlock(false);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	return false;	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::FireSoftwareTrigger()//發射軟體觸發訊號
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	MCSTATUS Status = MC_OK;	
	Status = McSetParamInt(m_CameraChannel, MC_ForceTrig, MC_ForceTrig_TRIG);
	if ( this->GetGrabLinkFullErrorString(Status, m_ErrorString) == false )
	{	return false;	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::WaitforCameraReadytoTrigger()//等待相機準備好可以觸發
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::ResetCameraReadyTriggerEvent()//復歸相機準備好了的事件
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::DoCameraDebayer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const unsigned char *pRaw, unsigned char *pResult, BAYER_PATTERN_MODE BayerPattern)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::GetWhiteBalanceParams(double &WBR, double &WBG, double &WBB)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::SetWhiteBalanceParams(double WBR, double WBG, double WBB)
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::CalcWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("CalcWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CCamera_GrabLinkFull_Q12A65Fm::ResetWhiteBalanceParams()
{
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CCamera_GrabLinkFull_Q12A65Fm::WriteCameraParameterToDevice()//儲存目前相機的參數至相機內部的韌體上
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::SaveCameraINIFile()
{
	if ( CCamera_Basic::SaveCameraINIFile() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::LoadCameraINIFile()
{
	if ( CCamera_Basic::LoadCameraINIFile() == false ) { return false; }

	this->SetCameraModelID(CAMERA_OBJ_GRABLINKFULL_Q12A65FM);	
	return true;
}
//-------------------------------------------------------------------------------------//
#ifndef CAMERA_OBJ_DISABLE
//------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::RS232_ConnectCamera(int Port)//相機連線
{	
	if ( m_CameraSerial.Open(m_SerialPort, 57600, 8, SERIES_PARITY_NONE, SERIES_STOPBITS_10) == false )
	//if ( m_CameraSerial.Open(m_SerialPort) == false )
	{
		this->m_ErrorString.Format(_T("Error, Camera RS232 Connect Fault(COM%d)"), Port);
		return false;
		
	}
	char EvenC = ETX_CHAR;//0x0D;//ETX_CHAR
	m_CameraSerial.SetEventChar(EvenC);
	//m_CameraSerial.SetLoopSleeipTime(10);
	
	CString ReturnStr = _T("");
	//if ( RS232_ReadData("A0", ReturnStr) == false )
	if ( RS232_ReadData("SN?", ReturnStr) == false )
	{	return false; }
	
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();
	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6:
	case LIGHT_CTRL_BOARD_8DA1:
	case LIGHT_CTRL_BOARD_ARDUINO:
		//相機直接接外部觸發訊號
		//RS232_SendShutterMode(2);			//Random Trigger
		//RS232_SendRandomTriggerMode(1);		//Pulse
		//RS232_SendTriggerPolarityMode(0);	//Negative
		//RS232_SendTriggerSourceMode(1);		//I/O
		break;
	default:
		//相機透過影像擷取卡-觸發訊號
		//RS232_SendShutterMode(2);			//Random Trigger
		//RS232_SendRandomTriggerMode(0);		//Fix
		//RS232_SendTriggerPolarityMode(1);	//Positive
		//RS232_SendTriggerSourceMode(0);		//CC
		break;
	}
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::RS232_ReadData(LPCSTR Command, CString &ReturnStr)//讀取相機資料
{
	//char EndStr[] = { 0x2C, 0x52, 0x51, 0x0D, '\0' };
	char EndStr[] = { ETX_CHAR, '\0' };
	char SendStr[32] = "";
	sprintf(SendStr, "%c%s%s", STX_CHAR, Command, EndStr);	

	const size_t MsgLen = 16;
	char SendMsg[MsgLen]="";
	char ResponseMsg[MsgLen]="";
	::memset(SendMsg, 0x00, sizeof(char)*MsgLen);	
	::memset(ResponseMsg, 0x00, sizeof(char)*MsgLen);	

	const int RealFullDataLength = 10;
	int GetResponseLength = 0;	
	DWORD dwMaxWait = 5000;

	int len = (int)(strlen(SendStr));
	if( m_CameraSerial.SendData(SendStr, len) == false )
	{
		this->m_ErrorString.Format(_T("Error, Camera RS232 SendData Fault. %s"), CString(Command)); 
		return false;
	}
	//Sleep(100);	
	
	GetResponseLength = m_CameraSerial.ReadDataByEndChar(ResponseMsg, MsgLen);
	//GetResponseLength = m_CameraSerial.ReadData(ResponseMsg, MsgLen, dwMaxWait);	
	if ( GetResponseLength <= 0 )
	{	return false; }

	len = (int)(strlen(ResponseMsg));
	if( len > 0 )
	{	ResponseMsg[len-1] = '\0'; }	
	ReturnStr= ResponseMsg;
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::RS232_CheckResposed()//確認相機回傳碼
{
	const size_t MsgLen = 16;
	char ResponseMsg[MsgLen]="";
	::memset(ResponseMsg, 0x00, sizeof(char)*MsgLen);	
	
	const int RealFullDataLength = 10;
	int GetResponseLength = 0;	
	DWORD dwMaxWait = 5000;
	
	GetResponseLength = m_CameraSerial.ReadDataByEndChar(ResponseMsg, MsgLen);
	//GetResponseLength = m_CameraSerial.ReadData(ResponseMsg, MsgLen, dwMaxWait);
	if( ResponseMsg <= 0 )
	{
		this->m_ErrorString.Format(_T("Error, Camera RS232 Read Response Fault!"));
		return false;
	}
	if( ResponseMsg[0] == NAK_CHAR )
	{
		this->m_ErrorString.Format(_T("Error, Camera RS232 Response NCK"));
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_GrabLinkFull_Q12A65Fm::GetGrabLinkFullErrorString(MCSTATUS Status, CString &err)
{
	if ( Status == MC_OK ) { return true; }
	switch ( Status )
	{
	case MC_NO_BOARD_FOUND:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_NO_BOARD_FOUND"));
		break;
	case MC_BAD_PARAMETER:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_BAD_PARAMETER"));
		break;
	case MC_IO_ERROR:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_IO_ERROR"));
		break;
	case MC_INTERNAL_ERROR:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_INTERNAL_ERROR"));
		break;
	case MC_NO_MORE_RESOURCES:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_NO_MORE_RESOURCES"));
		break;
	case MC_IN_USE:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_IN_USE"));
		break;
	case MC_NOT_SUPPORTED:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_NOT_SUPPORTED"));
		break;
	case MC_DATABASE_ERROR:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_DATABASE_ERROR"));
		break;
	case MC_OUT_OF_BOUND:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_OUT_OF_BOUND"));
		break;
	case MC_INSTANCE_NOT_FOUND:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_INSTANCE_NOT_FOUND"));
		break;
	case MC_INVALID_HANDLE:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_INVALID_HANDLE"));
		break;
	case MC_TIMEOUT:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_TIMEOUT"));
		break;
	case MC_INVALID_VALUE:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_INVALID_VALUE"));
		break;
	case MC_RANGE_ERROR:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_RANGE_ERROR"));
		break;
	case MC_BAD_HW_CONFIG:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_BAD_HW_CONFIG"));
		break;
	case MC_NO_EVENT:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_NO_EVENT"));
		break;
	case MC_LICENSE_NOT_GRANTED:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_LICENSE_NOT_GRANTED"));
		break;
	case MC_FATAL_ERROR:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_FATAL_ERROR"));
		break;
	case MC_HW_EVENT_CONFLICT:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_HW_EVENT_CONFLICT"));
		break;
	case MC_FILE_NOT_FOUND:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_FILE_NOT_FOUND"));
		break;
	case MC_OVERFLOW:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_OVERFLOW"));
		break;
	case MC_INVALID_PARAMETER_SETTING:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_INVALID_PARAMETER_SETTING"));
		break;
	case MC_PARAMETER_ILLEGAL_ACCESS:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_PARAMETER_ILLEGAL_ACCESS"));
		break;
	case MC_CLUSTER_BUSY:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_CLUSTER_BUSY"));
		break;
	case MC_SERVICE_ERROR:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_SERVICE_ERROR"));
		break;
	case MC_INVALID_SURFACE:
		err.Format(_T("Error, GrabLinkFull::%s."), _T("MC_INVALID_SURFACE"));
		break;	
	default:
		err.Format(_T("Error, GrabLinkFull::%s (%d)."), _T("MC do not defint it"), Status);
		break;
	}
	return false;
}
//------------------------------------------------------------------------------//
#endif//CAMERA_OBJ_DISABLE
#endif//GRABLINKFULL_Q12A65FM_USE