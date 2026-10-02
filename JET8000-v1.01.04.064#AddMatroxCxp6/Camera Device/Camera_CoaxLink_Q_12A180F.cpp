// Camera_CoaxLink_Q_12A180F.cpp: implementation of the CCamera_CoaxLink_Q_12A180F class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Camera_CoaxLink_Q_12A180F.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#ifdef COAXLINK_Q_12A180F_USE
//-------------------------------------------------------------------------------------//
CCamera_CoaxLink_Q_12A180F Camera_CoaxLink_Q_12A180F;
//-------------------------------------------------------------------------------------//	
#ifndef CAMERA_OBJ_DISABLE
CEGrabber::CEGrabber(Euresys::GenTL &gentl):Euresys::EGrabber<Euresys::CallbackSingleThread>(gentl)
{
	const unsigned int BufferNum = CAMERA_IMAGE_BUFFER_COUNT_3D;
	reallocBuffers(BufferNum);	
	//enableEvent<Euresys::CicData>();
	//enableEvent<Euresys::DataStreamData>();
	enableEvent<Euresys::NewBufferData>();

	//enableEvent<Euresys::All>();	
}
//-------------------------------------------------------------------------------------//
CEGrabber::~CEGrabber(void)
{	
	//disableEvent<Euresys::All>();

	disableEvent<Euresys::NewBufferData>();
	//disableEvent<Euresys::DataStreamData>();
	//disableEvent<Euresys::CicData>();	
}
//-------------------------------------------------------------------------------------//
inline void CEGrabber::onNewBufferEvent(const Euresys::NewBufferData& Data)//取像結束CallBack位置
{
	Camera_CoaxLink_Q_12A180F.IncrementCountForCameraExposuredEnd();	
	Camera_CoaxLink_Q_12A180F.IncrementCountForCameraCallback();
	Camera_CoaxLink_Q_12A180F.CheckCameraExposuredEndEvent();

	Euresys::ScopedBuffer buffer(*this , Data);
	unsigned char *ImgPtr = buffer.getInfo<unsigned char *>(Euresys::gc::BUFFER_INFO_BASE);	//影像資料的起始位置
	if ( Camera_CoaxLink_Q_12A180F.AddCameraRingBufferList(ImgPtr) == true )
	{
	}
}
//-------------------------------------------------------------------------------------//
inline void CEGrabber::onCicEvent(const Euresys::CicData &Data)
{
	 switch (Data.numid) 
	 {
	case Euresys::gc::Euresys::EVENT_DATA_NUMID_CIC_CAMERA_TRIGGER_RISING_EDGE:
		//Tools::log("onCicEvent: Camera trigger rising edge: timestamp=" + Tools::formatTimestamp(data.timestamp));
		break;
	case Euresys::gc::Euresys::EVENT_DATA_NUMID_CIC_CAMERA_TRIGGER_FALLING_EDGE:
		//Tools::log("onCicEvent: Camera trigger falling edge: timestamp=" + Tools::formatTimestamp(data.timestamp));
		break;
	case Euresys::gc::Euresys::EVENT_DATA_NUMID_CIC_ALLOW_NEXT_CYCLE:
		//Tools::log("onCicEvent: Allow next cycle: timestamp=" + Tools::formatTimestamp(data.timestamp));
		//execute<DeviceModule>("StartCycle");
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------------------//
inline void CEGrabber::onStreamDataEvent(const Euresys::DataStreamData &Data)
{
	switch (Data.numid) 
	{
	case Euresys::ge::EVENT_DATA_NUMID_DATASTREAM_START_OF_CAMERA_READOUT:
		//Tools::log("StartOfCameraReadout");
		break;
	case Euresys::ge::EVENT_DATA_NUMID_DATASTREAM_END_OF_CAMERA_READOUT:
		//Tools::log("EndOfCameraReadout");
		break;
	case Euresys::ge::EVENT_DATA_NUMID_DATASTREAM_START_OF_SCAN:
		//Tools::log("StartOfScan");
		break;
	case Euresys::ge::EVENT_DATA_NUMID_DATASTREAM_END_OF_SCAN:
		//Tools::log("EndOfScan");
		break;
	case Euresys::ge::EVENT_DATA_NUMID_DATASTREAM_REJECTED_FRAME:
		//Tools::log("RejectedFrame");
		break;
	case Euresys::ge::EVENT_DATA_NUMID_DATASTREAM_REJECTED_SCAN:
		//Tools::log("RejectedScan");
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------------------//
#endif//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CCamera_CoaxLink_Q_12A180F, CCamera_Basic)
//-------------------------------------------------------------------------------------//
CCamera_CoaxLink_Q_12A180F::CCamera_CoaxLink_Q_12A180F()
{
	this->PreInitCamera();
	SetCameraConstructed(true);
}
//-------------------------------------------------------------------------------------//
CCamera_CoaxLink_Q_12A180F::~CCamera_CoaxLink_Q_12A180F()
{
	this->ReleaseCamera();
}
//-------------------------------------------------------------------------------------//
void CCamera_CoaxLink_Q_12A180F::PreInitCamera()
{
	this->m_CameraModelID = CAMERA_OBJ_COAXLINK_Q_12A180_FM;
	this->m_CameraModelName = _T("CoaxLink_Q_12A180FM");

	const int PaddingTime_us = 1000;
	this->m_PeriodTim_us = 5600;//5.6ms for Gen3
	this->m_PeriodTim_us = 7500;//7.5ms for Gen2	

	this->m_ExposureTime_us = 3000;//3ms
	this->m_ExposureTimeMin_us = 20;//曝光時間-Min
	this->m_ExposureTimeMax_us = 1000000;//曝光時間-Max
	this->m_TriggerDelay = 500;//處發延遲時間-us

	this->m_CameraFPS = 187.0;//187.0;
	this->m_CameraBitCount = 8;	
	this->m_CameraImageW = 4096;//相機影像寬度
	this->m_CameraImageH = 3072;//相機影像高度
	this->m_CameraImageStep = 4096;//相機間距
	this->m_CameraSizeRaw = m_CameraImageW*m_CameraImageH;
	this->m_CameraSizeColor = this->m_CameraSizeRaw*3;
	this->m_CameraBitMode = 8;//8Bit Mode

	this->m_FVersion = 0;	//韌體版本 (不同的韌體版本會有不同的參數設定)
#ifndef CAMERA_OBJ_DISABLE	
	this->m_grabberPtr = NULL;
#endif//CAMERA_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::InitialCamera()//相機初始化
{
	const char fnName[] = "CCamera_CoaxLink_Q_12A180F::InitialCamera";
	char string[128]={0};

	CCamera_Basic::SaveCameraProcess(_T("InitialCamera"), MSG_LEVEL_HIGH);

	this->ReleaseCamera();
	if ( CCamera_CoaxLink_Q_12A180F::LoadCameraINIFile() == false ) { return false; }
	if ( CCamera_CoaxLink_Q_12A180F::SaveCameraINIFile() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	try
	{
		CString CameraName;
		//====================================================================================================//
		//因為只有一張擷取卡與一個相機需要控制，所以程式被限制在各只有一個的寫法。
		//因為已EGrabber讀取相機資訊太慢，所以改成以底層的GebICam來擷取
		//string Card = m_grabberPtr->getString<Euresys::InterfaceModule>("InterfaceID");
		//string Device = m_grabberPtr->getString<Euresys::DeviceModule>("DeviceID");
		//====================================================================================================//
		GenICam::Client::TL_HANDLE tl_Handle;
		m_grabberPtr = new CEGrabber(m_genTL);
		if ( NULL == m_grabberPtr )
		{
			m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F Create m_grabberPtr Fault"));
			return false;
		}
		tl_Handle = m_genTL.tlOpen();
		CameraName = m_genTL.tlGetInterfaceID(tl_Handle,0).c_str();
		//printf("%s\n" , m_DeviceName);
		m_genTL.tlClose(tl_Handle);
		//====================================================================================================//		

		//About Reset Camera System
		pGrabber->execute<Euresys::RemoteModule>("AcquisitionStop");
		//m_grabberPtr->setString<Euresys::RemoteModule>("ConnectionConfig" , "CXP3_X1");	//設定相機連線的配置(幾條線)，預設是CXP3_X1(約全速的八分之一)，因此設定為CXP6_X4全速。
		m_grabberPtr->setString<Euresys::RemoteModule>("ConnectionConfig" , "CXP6_X4");	//設定相機連線的配置(幾條線)，預設是CXP3_X1(約全速的八分之一)，因此設定為CXP6_X4全速。
		m_grabberPtr->setString<Euresys::RemoteModule>("PixelFormat" , "Mono8");	//設定相機像素格式
		m_grabberPtr->setString<Euresys::RemoteModule>("FlashStrobeMode" , "Disabled");//關閉相機輸出閃燈訊號模式
		m_grabberPtr->setString<Euresys::RemoteModule>("HDR_Mode" ,"Off");	//關閉相機HDR模式
		m_grabberPtr->setFloat<Euresys::RemoteModule>("Gain" , 1.0f);		//增益設定為1.0(最大值為32.0)
		m_grabberPtr->setFloat<Euresys::RemoteModule>("BlackLevel" , 1.0f);	//黑值設定為1.0(最大值為511.0)
		if ( 8 == m_CameraBitMode )//設定相機解析度深度
		{	m_grabberPtr->setString<Euresys::RemoteModule>("SensorBitDepth" , "Resolution_8_Bit"); }
		else
		{	m_grabberPtr->setString<Euresys::RemoteModule>("SensorBitDepth" , "Resolution_10_Bit"); }
		m_grabberPtr->setString<Euresys::RemoteModule>("DefectPixelCorrectionEnable" , "True"); //設定相機感測元件異常需要處理(可能回臨近均值)		
		//====================================================================================================//
		m_FVersion = m_grabberPtr->getInteger<Euresys::InterfaceModule>("FirmwareRevision");		//取得韌體版本(不同的韌體版本會有不同的參數設定)
		//====================================================================================================//
		//設定相機取像大小的初始值
		unsigned int ImageSizeW = this->GetCameraImageW();
		unsigned int ImageSizeH = this->GetCameraImageH();
		#if ( CAMERA_ROTATION_MODE==CAMERA_ROTATION_090 || CAMERA_ROTATION_MODE==CAMERA_ROTATION_270 || CAMERA_ROTATION_MODE==CAMERA_ROTATION_090_YMIRROR || CAMERA_ROTATION_MODE==CAMERA_ROTATION_270_YMIRROR )
		//	ImageSizeW = (this->GetCameraImageH());
		//	ImageSizeH = (this->GetCameraImageW());
		#endif

		int64_t sensorwidth = m_grabberPtr->getInteger<Euresys::RemoteModule>("SensorWidth");		//取得相機最大寬度
		int64_t sensorheight = m_grabberPtr->getInteger<Euresys::RemoteModule>("SensorHeight");	//取得相機最大高度
		m_grabberPtr->setInteger<Euresys::RemoteModule>("Width" , sensorwidth);	//設定相機擷取影像的寬度
		int64_t CameraSizeW = m_grabberPtr->getInteger<Euresys::RemoteModule>("Width");
		m_grabberPtr->setInteger<Euresys::RemoteModule>("Height" , sensorheight);//設定相機擷取影像的高度	
		int64_t CameraSizeH = m_grabberPtr->getInteger<Euresys::RemoteModule>("Height");		
		if ( CameraSizeW!=ImageSizeW || CameraSizeH!=ImageSizeH )
		{
			this->m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F Camea Size Exception(%d, %d)"), CameraSizeW, CameraSizeH);
			delete m_grabberPtr; m_grabberPtr=NULL;
			return false;	
		}
		//====================================================================================================//
		//設定相機黑場(DarkField)
		m_grabberPtr->setString<Euresys::RemoteModule>("DF_BlackClamp" , "False");
		m_grabberPtr->setString<Euresys::RemoteModule>("DF_ColumnOffsetCorrection" , "True");//True
		m_grabberPtr->execute<Euresys::RemoteModule>("DF_RestoreFactory");
		//====================================================================================================//
		//設定相機白場(BrightField)
		m_grabberPtr->setString<Euresys::RemoteModule>("BF_ColumnGainCorrection" , "True");//True
		m_grabberPtr->setInteger<Euresys::RemoteModule>("BF_CalibrationVideoLevel" , 65);
		m_grabberPtr->setString<Euresys::RemoteModule>("BF_OutputImagesDuringCalibration" , "False");
		m_grabberPtr->execute<Euresys::RemoteModule>("BF_RestoreFactory");

		//Reset FrameGrabber Count
		if ( ResetFrameGrabberEventCount() == false )
		{
			delete m_grabberPtr; m_grabberPtr=NULL;
			return false; 
		}

		//Test Free Run 
		if ( SwitchFreeRunMode() == false )
		{
			delete m_grabberPtr; m_grabberPtr=NULL;
			return false; 
		}
		
		//Test Software Trigger
		if ( SwitchSoftwareTriggerMode() == false )
		{
			delete m_grabberPtr; m_grabberPtr=NULL;
			return false; 
		}

		//Test External Trigger
		if ( SwitchExternalTriggerMode() == false )		
		{
			delete m_grabberPtr; m_grabberPtr=NULL;
			return false;  
		}

		//FPS
		double CameraFPS = m_grabberPtr->getFloat<Euresys::RemoteModule>("AcquisitionFrameRate"); //設定相機感測元件異常需要處理(可能回臨近均值)
		CCamera_Basic::m_CameraFPS = CameraFPS;		
		//CCamera_Basic::m_CameraFPS = 160;		
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F %s") , str);

		delete m_grabberPtr; m_grabberPtr=NULL;
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
		delete m_grabberPtr; m_grabberPtr=NULL;	
		return false;
	}
	::memset(m_ClonedRawImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH);	
	//::memset(m_ClonedColorImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH*4);

#endif//CAMERA_OBJ_DISABLE
	SetCameraInited(true);
	SetCameraImageMode(CAMERA_IMAGE_GRAY);
	SetCameraBayerPattern(BAYER_PATTERN_NONE);
#ifdef DISABLE_3D
	SetCameraImageMode(CAMERA_IMAGE_BAYER);
	SetCameraBayerPattern(BAYER_PATTERN_RGGB);//For Test
#endif//DISABLE_3D	
	SetCameraGrabbing_Unlock(false);
	CreateBMPInfo(m_pColorInfo, true);
	CreateBMPInfo(m_pMonoInfo, false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::ReleaseCamera()//相機釋放
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ReleaseCamera"), MSG_LEVEL_HIGH);
#ifndef CAMERA_OBJ_DISABLE
	if( NULL==m_grabberPtr )
	{	return true;	}	
	try
	{
		m_grabberPtr->execute<Euresys::RemoteModule>("AcquisitionStop");
		m_grabberPtr->stop();
	}
	catch(const std::exception &e)
	{	
		CString str = e.what();
		m_ErrorString.Format(_T("Error, Stop Grab Fault::%s"), str);		

		delete m_grabberPtr;
		m_grabberPtr=NULL;
		return false;
	}
	delete m_grabberPtr;
	m_grabberPtr=NULL;
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
bool CCamera_CoaxLink_Q_12A180F::ExecCameraPowerOff()//相機斷電
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ExecCameraPowerOff"), MSG_LEVEL_HIGH);
#ifndef CAMERA_OBJ_DISABLE
	if( NULL==m_grabberPtr )
	{	return true;	}	
	try
	{
		m_grabberPtr->execute<Euresys::InterfaceModule>("CxpPoCxpTurnOff");
	}
	catch(const std::exception &e)
	{	
		CString str = e.what();
		m_ErrorString.Format(_T("Error, Power Off Camera Fault::%s"), str);
		return false;
	}	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::ExecCameraPowerOn()//相機開電
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ExecCameraPowerOn"), MSG_LEVEL_HIGH);
#ifndef CAMERA_OBJ_DISABLE
	if( NULL==m_grabberPtr )
	{	return true;	}	
	try
	{
		m_grabberPtr->execute<Euresys::InterfaceModule>("CxpPoCxpAuto");
	}
	catch(const std::exception &e)
	{	
		CString str = e.what();
		m_ErrorString.Format(_T("Error, Power On Camera Fault::%s"), str);
		return false;
	}	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::ExecCameraPowerReset()//相機復歸
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ExecCameraPowerReset"), MSG_LEVEL_HIGH);
#ifndef CAMERA_OBJ_DISABLE
	if( NULL==m_grabberPtr )
	{	return true;	}	
	try
	{
		m_grabberPtr->execute<Euresys::InterfaceModule>("CxpPoCxpTripReset");
	}
	catch(const std::exception &e)
	{	
		CString str = e.what();
		m_ErrorString.Format(_T("Error, Reset Camera Fault::%s"), str);
		return false;
	}	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::ResetCamera()//復歸相機
{	
	SetCameraGrabbing_Unlock(false);
	if ( ExecCameraPowerOff() == false ) { return false; }
	::Sleep(3000);
	if ( ExecCameraPowerOn() == false ) { return false; }
	::Sleep(3000);
	if ( ExecCameraPowerReset() == false ) { return false; }
	::Sleep(3000);
	if ( SwitchExternalTriggerMode() == false )	{ return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr()
{
#ifndef CAMERA_OBJ_DISABLE
	if ( NULL == m_grabberPtr )
	{
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F m_grabberPtr is NULL"));
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::DoCoaxLinkStartGrab()
{
	if ( this->CheckCameraInited() == false ) { return false; }	
	CCamera_Basic::SaveCameraProcess(_T("DoCoaxLinkStartGrab"), MSG_LEVEL_HIGH);

	this->m_CameraImagePtr = NULL;
#ifndef CAMERA_OBJ_DISABLE
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
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }

		m_grabberPtr->execute<Euresys::RemoteModule>("AcquisitionStart");
		m_grabberPtr->start();
		SetCameraGrabbing_Unlock(true);		
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F (%s)"), str);
		return false;
	}
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SwitchFreeRunMode()
{
#ifndef CAMERA_OBJ_DISABLE
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }
		const int ExposureTime = this->GetExposureTime();
		CCamera_Basic::SaveCameraProcess(_T("SwitchFreeRunMode"), MSG_LEVEL_HIGH);
		//設定相機連續取像狀態
		m_grabberPtr->setString<Euresys::DeviceModule>("CameraControlMethod","NC");
		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerSource" , "Trigger");
		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerActivation" , "RisingEdge");//FallingEdge
		m_grabberPtr->setString<Euresys::RemoteModule>("ExposureMode" , "Timed");
		//m_grabberPtr->setInteger<Euresys::DeviceModule>("ExposureTime" , ExposureTime);	
		m_grabberPtr->execute<Euresys::RemoteModule>("AcquisitionMaxFrameRate");
		SetCameraGrabMode(CAMERA_GRAB_FREE_RUN);
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F (%s)"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SwitchSoftwareTriggerMode()
{
#ifndef CAMERA_OBJ_DISABLE
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }

		CCamera_Basic::SaveCameraProcess(_T("SwitchSoftwareTriggerMode"), MSG_LEVEL_HIGH);

		//設定相機軟體觸發狀態
		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime   = ExposureTime+1000;
		m_grabberPtr->setString<Euresys::DeviceModule>("CameraControlMethod","RG");
		m_grabberPtr->setString<Euresys::DeviceModule>("ExposureReadoutOverlap" , "true");
		m_grabberPtr->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime" , 1000);
	//	m_grabberPtr->setFloat<Euresys::DeviceModule>("ExposureTime" , 4600);
		m_grabberPtr->setString<Euresys::DeviceModule>("CycleTriggerSource", "StartCycle");
		m_grabberPtr->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);
		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerSource" , "Trigger");
		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerActivation" , "FallingEdge");//RisingEdge, FallingEdge
		m_grabberPtr->setString<Euresys::RemoteModule>("ExposureMode" , "TriggerWidth");
		SetCameraGrabMode(CAMERA_GRAB_SOFTWARE_TRIGGER);
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F (%s)"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SwitchExternalTriggerMode()
{
	bool IsOK = true;
#ifndef LIGHT_CTRL_DISABLE	
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();
	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6:
		IsOK = SwitchExternalTriggerMode_A5V1();
		break;
	case LIGHT_CTRL_BOARD_8DA1:
	case LIGHT_CTRL_BOARD_ARDUINO:
		IsOK = SwitchExternalTriggerMode_8DA1();
		break;
	default:
		IsOK = SwitchExternalTriggerMode_DLP();
		break;
	}
#else
	IsOK = SwitchExternalTriggerMode_DLP();	
#endif//LIGHT_CTRL_DISABLE
	if ( true == bIsOK )
	{	SetCameraGrabMode(CAMERA_GRAB_EXTERNAL_TRIGGER); }
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SwitchExternalTriggerMode_DLP()
{
#ifndef CAMERA_OBJ_DISABLE
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }

		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime   = ExposureTime+1000;

		//設定相機外埠處發狀態
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineSelector" , "IIN11");//IIN11, IIN12
		//by Ken Error
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInverter" , "False");
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolSelector" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolSource" , "IIN11");//IIN11, IIN12
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolActivation", "FallingEdge");//FallingEdge, RisingEdge

		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSelector" , "DEL2");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSource1" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSource2" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolClockSource" , "TIME1US");
		m_grabberPtr->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue" , 500);

		m_grabberPtr->setString<Euresys::DeviceModule>("CameraControlMethod","RG");//RG, EXTERNAL
		m_grabberPtr->setString<Euresys::DeviceModule>("ExposureReadoutOverlap" , "true");
		m_grabberPtr->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime" , 1000);
		//m_grabberPtr->setFloat<Euresys::DeviceModule>("ExposureTime" , 4600);
		m_grabberPtr->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");
		m_grabberPtr->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);

		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerSource" , "Trigger");
		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerActivation" , "FallingEdge");//FallingEdge, RisingEdge
		m_grabberPtr->setString<Euresys::RemoteModule>("ExposureMode" , "TriggerWidth");
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F (%s)"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SwitchExternalTriggerMode_A5V1()
{
#ifndef CAMERA_OBJ_DISABLE
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }

		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime   = this->GetPeriodTime();//ExposureTime+1000;

		//設定相機外埠處發狀態
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineSelector" , "IIN11");//IIN11, IIN12
		//by Ken Error
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInverter" , "True");
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolSelector" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolSource" , "IIN11");//IIN11, IIN12
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolActivation", "RisingEdge");//FallingEdge, RisingEdge

		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSelector" , "DEL2");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSource1" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSource2" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolClockSource" , "TIME1US");
		m_grabberPtr->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue" , 500);

		m_grabberPtr->setString<Euresys::DeviceModule>("CameraControlMethod","EXTERNAL");//RG,
		if( m_FVersion < COAXLINK_FIREWARE_VERSION_217 )
		{	//舊版才有支援以下參數 
			m_grabberPtr->setString<Euresys::DeviceModule>("ExposureReadoutOverlap" , "true");//disable@coaxlink_7.1.1.25 for external trigger
			m_grabberPtr->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime" , 1000);//disable@coaxlink_7.1.1.25 for external trigger 
			m_grabberPtr->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");//disable@coaxlink_7.1.1.25 for external trigger 
			m_grabberPtr->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);//disable@coaxlink_7.1.1.25 for external trigger 
		}

		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerSource" , "Trigger");
		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerActivation" , "RisingEdge");//FallingEdge, RisingEdge
		m_grabberPtr->setString<Euresys::RemoteModule>("ExposureMode" , "TriggerWidth");
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F (%s)"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SwitchExternalTriggerMode_8DA1()	
{
#ifndef CAMERA_OBJ_DISABLE
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }

		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime   = this->GetPeriodTime();//ExposureTime+1000;

		//設定相機外埠處發狀態
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineSelector" , "IIN11");//IIN11, IIN12
		//by Ken Error
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInverter" , "True");
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolSelector" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolSource" , "IIN11");//IIN11, IIN12
		m_grabberPtr->setString<Euresys::InterfaceModule>("LineInputToolActivation", "RisingEdge");//FallingEdge, RisingEdge

		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSelector" , "DEL2");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSource1" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolSource2" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("DelayToolClockSource" , "TIME1US");
		m_grabberPtr->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue" , 500);

		m_grabberPtr->setString<Euresys::DeviceModule>("CameraControlMethod","EXTERNAL");//RG,
		if( m_FVersion < COAXLINK_FIREWARE_VERSION_217 )
		{	//舊版才有支援以下參數 
			m_grabberPtr->setString<Euresys::DeviceModule>("ExposureReadoutOverlap" , "true");//disable@coaxlink_7.1.1.25 for external trigger
			m_grabberPtr->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime" , 1000);//disable@coaxlink_7.1.1.25 for external trigger 
			m_grabberPtr->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");//disable@coaxlink_7.1.1.25 for external trigger 
			m_grabberPtr->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);//disable@coaxlink_7.1.1.25 for external trigger 
		}

		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerSource" , "Trigger");
		m_grabberPtr->setString<Euresys::RemoteModule>("TriggerActivation" , "RisingEdge");//FallingEdge, RisingEdge
		m_grabberPtr->setString<Euresys::RemoteModule>("ExposureMode" , "TriggerWidth");
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F (%s)"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::ResetFrameGrabberEventCount()//清除影像擷取卡的事件計數器
{
#ifndef CAMERA_OBJ_DISABLE
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }

		CCamera_Basic::SaveCameraProcess(_T("ResetFrameGrabberEventCount"), MSG_LEVEL_HIGH);

		//設定擷取卡外埠觸發的計數器與重置計數器
		m_grabberPtr->setString<Euresys::InterfaceModule>("EventSelector" , "LIN1");
		m_grabberPtr->setString<Euresys::InterfaceModule>("EventNotification" , "true");
		m_grabberPtr->setString<Euresys::InterfaceModule>("EventNotificationContext1" , "EventSpecific");
		m_grabberPtr->setString<Euresys::InterfaceModule>("EventNotificationContext2" , "EventSpecific");
		m_grabberPtr->setString<Euresys::InterfaceModule>("EventNotificationContext3" , "EventSpecific");
		m_grabberPtr->execute<Euresys::InterfaceModule>("EventCountReset");
		//重置擷取卡傳輸到相機的觸發遺失計數器
		if( m_FVersion < COAXLINK_FIREWARE_VERSION_217 )//舊版才有支援以下參數//disable@coaxlink_7.1.1.25
		{	m_grabberPtr->execute<Euresys::DeviceModule>("CycleLostTriggerCountReset");	}
		//設定相機正源觸發次數與重置計數器
		m_grabberPtr->setString<Euresys::DeviceModule>("EventSelector" , "CameraTriggerRisingEdge");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotification" , "true");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext1" , "EventSpecific");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext2" , "EventSpecific");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext3" , "EventSpecific");
		m_grabberPtr->execute<Euresys::DeviceModule>("EventCountReset");
		//設定相機負源觸發次數與重置計數器
		m_grabberPtr->setString<Euresys::DeviceModule>("EventSelector" , "CameraTriggerFallingEdge");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotification" , "true");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext1" , "EventSpecific");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext2" , "EventSpecific");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext3" , "EventSpecific");
		m_grabberPtr->execute<Euresys::DeviceModule>("EventCountReset");
		//設定相機觸發次數與重置計數器
		m_grabberPtr->setString<Euresys::DeviceModule>("EventSelector" , "Trigger");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotification" , "true");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext1" , "EventSpecific");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext2" , "EventSpecific");
		m_grabberPtr->setString<Euresys::DeviceModule>("EventNotificationContext3" , "EventSpecific");
		m_grabberPtr->execute<Euresys::DeviceModule>("EventCountReset");
		//設定開始相機到電腦記憶體傳送圖片的次數與重置計數器
		m_grabberPtr->setString<Euresys::StreamModule>("EventSelector" , "StartOfCameraReadout");
		m_grabberPtr->setString<Euresys::StreamModule>("EventNotification" , "true");
		m_grabberPtr->setString<Euresys::StreamModule>("EventNotificationContext1" , "EventSpecific");
		m_grabberPtr->setString<Euresys::StreamModule>("EventNotificationContext2" , "EventSpecific");
		m_grabberPtr->setString<Euresys::StreamModule>("EventNotificationContext3" , "EventSpecific");
		m_grabberPtr->execute<Euresys::StreamModule>("EventCountReset");
		//設定完成相機到電腦記憶體傳送圖片的次數與重置計數器
		m_grabberPtr->setString<Euresys::StreamModule>("EventSelector" , "EndOfCameraReadout");
		m_grabberPtr->setString<Euresys::StreamModule>("EventNotification" , "true");
		m_grabberPtr->setString<Euresys::StreamModule>("EventNotificationContext1" , "EventSpecific");
		m_grabberPtr->setString<Euresys::StreamModule>("EventNotificationContext2" , "EventSpecific");
		m_grabberPtr->setString<Euresys::StreamModule>("EventNotificationContext3" , "EventSpecific");
		m_grabberPtr->execute<Euresys::StreamModule>("EventCountReset");
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode)
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
bool CCamera_CoaxLink_Q_12A180F::SetExposureTime(const int ExposureTime)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	if ( m_ExposureTime_us == ExposureTime ) 
	{	return true; }
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }
		
		CCamera_Basic::SaveCameraProcess(_T("SetExposureTime"), MSG_LEVEL_HIGH);

		m_grabberPtr->setInteger<Euresys::RemoteModule>("ExposureTime" , ExposureTime);

		if( m_FVersion < COAXLINK_FIREWARE_VERSION_217 )
		{
			m_grabberPtr->setInteger<Euresys::DeviceModule>("ExposureTime" , ExposureTime);	
		}
		else
		{
			if ( CAMERA_GRAB_EXTERNAL_TRIGGER != m_CameraGrabMode )
			{
				m_grabberPtr->setInteger<Euresys::DeviceModule>("ExposureTime" , ExposureTime);	
			}
		}
	
		m_ExposureTime_us = ExposureTime;
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::StartCameraGrab()//只取一張影像-內部觸發用
{
	CCamera_Basic::SaveCameraProcess(_T("StartCameraGrab"), MSG_LEVEL_HIGH);

	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();
	if ( this->DoCoaxLinkStartGrab() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::StartCameraLiveGrab()//連續取多張影像-外部觸發用
{
	CCamera_Basic::SaveCameraProcess(_T("StartCameraLiveGrab"), MSG_LEVEL_HIGH);

	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();

	if ( this->DoCoaxLinkStartGrab() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::StopCameraGrab()//停止取像
{
#ifndef CAMERA_OBJ_DISABLE	
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }
		
		CCamera_Basic::SaveCameraProcess(_T("StopCameraGrab"), MSG_LEVEL_HIGH);
		m_grabberPtr->execute<Euresys::RemoteModule>("AcquisitionStop");
		m_grabberPtr->stop();
		SetCameraGrabbing_Unlock(false);		
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::FireSoftwareTrigger()//發射軟體觸發訊號
{
#ifndef CAMERA_OBJ_DISABLE	
	try
	{		
		if ( CCamera_CoaxLink_Q_12A180F::CheckGrabberPtr() == false )
		{	return false; }
		
		CCamera_Basic::SaveCameraProcess(_T("FireSoftwareTrigger"), MSG_LEVEL_HIGH);
		m_grabberPtr->start(1);
		m_grabberPtr->execute<Euresys::DeviceModule>("StartCycle");		
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLink_Q_12A180F %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::WaitforCameraReadytoTrigger()//等待相機準備好可以觸發
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("WaitforCameraReadytoTrigger"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::ResetCameraReadyTriggerEvent()//復歸相機準備好了的事件
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("ResetCameraReadyTriggerEvent"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::DoCameraDebayer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const unsigned char *pRaw, unsigned char *pResult, BAYER_PATTERN_MODE BayerPattern)
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("DoCameraDebayer"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::GetWhiteBalanceParams(double &WBR, double &WBG, double &WBB)
{
#ifndef CAMERA_OBJ_DISABLE	
	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SetWhiteBalanceParams(double WBR, double WBG, double WBB)
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("SetWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::CalcWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("CalcWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::ResetWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("ResetWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::WriteCameraParameterToDevice()//儲存目前相機的參數至相機內部的韌體上
{
#ifndef CAMERA_OBJ_DISABLE	
	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::SaveCameraINIFile()
{
	if ( CCamera_Basic::SaveCameraINIFile() == false ) { return false; }

	bool    IsOK = true;
	CString KeyName;
	CString KeyString;
	CString Section = CCamera_Basic::m_CameraModelName;
	CString FileName = CCamera_Basic::GetCameraINIFileName();

	KeyName.Format(_T("Camera Bit Mode")); KeyString.Format(_T("%d"), m_CameraBitMode);
	if (SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLink_Q_12A180F::LoadCameraINIFile()
{
	if ( CCamera_Basic::LoadCameraINIFile() == false ) { return false; }

	this->SetCameraModelID(CAMERA_OBJ_COAXLINK_Q_12A180_FM);	

	CString KeyName;
	CString KeyString;
	CString Section = CCamera_Basic::m_CameraModelName;
	CString FileName = CCamera_Basic::GetCameraINIFileName();
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];

	KeyName.Format(_T("Camera Bit Mode")); KeyString.Format(_T("%d"), m_CameraBitMode);
	if (LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	
		m_CameraBitMode = ::_ttoi(ReturnString);	
		switch ( m_CameraBitMode )
		{
		case 8:		m_CameraBitMode = 8;	break;
		case 10:	m_CameraBitMode = 10;	break;		
		default:	m_CameraBitMode = 8;	break;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
#endif//COAXLINK_Q_12A180F_USE
//-------------------------------------------------------------------------------------//