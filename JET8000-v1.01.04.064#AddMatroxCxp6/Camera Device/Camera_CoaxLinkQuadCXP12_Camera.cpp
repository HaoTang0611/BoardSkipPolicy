// Camera_CoaxLinkQuadCXP12_Camera.cpp: implementation of the CCamera_CoaxLinkQuadCXP12_Camera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Camera_CoaxLinkQuadCXP12_Camera.h"
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
#ifdef COAXLINK_QUAD_CXP12_CAMERA_USE
//-------------------------------------------------------------------------------------//
CCamera_CoaxLinkQuadCXP12_Camera Camera_CoaxLinkQuadCXP12_Camera;
//-------------------------------------------------------------------------------------//	
#ifndef CAMERA_OBJ_DISABLE
CEGrabber::CEGrabber(Euresys::GenTL &gentl, int interfaceIndex, int deviceIndex, int dataStreamIndex):Euresys::EGrabber<Euresys::CallbackSingleThread>(gentl,interfaceIndex,deviceIndex,dataStreamIndex)
{
	m_CameraPtr = NULL;
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
CCamera_CoaxLinkQuadCXP12_Camera* CEGrabber::GetCameraPtr()
{
	return m_CameraPtr;
}
//-------------------------------------------------------------------------------------//}
void CEGrabber::SetCameraPtr(CCamera_CoaxLinkQuadCXP12_Camera *Ptr)
{
	m_CameraPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
inline void CEGrabber::onNewBufferEvent(const Euresys::NewBufferData& Data)//取像結束CallBack位置
{
	CCamera_Basic *CameraPtr = GetCameraPtr();
	if ( NULL == CameraPtr ) { return ; }
	CameraPtr->IncrementCountForCameraExposuredEnd();
	CameraPtr->IncrementCountForCameraCallback();
	CameraPtr->CheckCameraExposuredEndEvent();

	Euresys::ScopedBuffer buffer(*this , Data);
	unsigned char *ImgPtr = buffer.getInfo<unsigned char *>(Euresys::gc::BUFFER_INFO_BASE);	//影像資料的起始位置
	if ( CameraPtr->AddCameraRingBufferList(ImgPtr) == true )
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
IMPLEMENT_DYNAMIC(CCamera_CoaxLinkQuadCXP12_Camera, CCamera_Basic)
//-------------------------------------------------------------------------------------//
CCamera_CoaxLinkQuadCXP12_Camera::CCamera_CoaxLinkQuadCXP12_Camera():CCamera_Basic()
{
	this->PreInitCamera();
	SetCameraConstructed(true);
}
//-------------------------------------------------------------------------------------//	
CCamera_CoaxLinkQuadCXP12_Camera::~CCamera_CoaxLinkQuadCXP12_Camera()
{
	this->ReleaseCamera();
}
//-------------------------------------------------------------------------------------//	
void CCamera_CoaxLinkQuadCXP12_Camera::PreInitCamera()
{	
	this->m_CameraModelID = CAMERA_OBJ_COAXLINK_QUAD_CXP12_CAMERA;
	this->m_CameraModelName = _T("CoaxLinkQuadCXP12_Camera");
	this->m_CameraDeviceType = COAXPRESS_CAMERA_VCC_25CXPHSM;//Coaxlink連到相機的樣式

	const int PaddingTime_us = 1000;
	this->m_PeriodTim_us = 6700;//
	this->m_PeriodTim_us = 7000;//

	this->m_ExposureTime_us = 3000;//3ms
	this->m_ExposureTimeMin_us = 20;//曝光時間-Min
	this->m_ExposureTimeMax_us = 1000000;//曝光時間-Max
	this->m_TriggerDelay = 500;//處發延遲時間-us

	this->m_CameraFPS = 150;//181.0;
	this->m_CameraBitCount = 8;	
	this->m_CameraImageW = 5120;//相機影像寬度
	this->m_CameraImageH = 5120;//相機影像高度
	this->m_CameraImageStep = 5120;//相機間距
	this->m_CameraSizeRaw = m_CameraImageW*m_CameraImageH;
	this->m_CameraSizeColor = this->m_CameraSizeRaw*3;	

	this->m_FVersion = 0;	//韌體版本 (不同的韌體版本會有不同的參數設定)
	this->m_CameraBitMode = 8;//8Bit Mode
#ifndef CAMERA_OBJ_DISABLE	
	this->m_grabberPtr = NULL;	
#endif//CAMERA_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::InitialCamera()//相機初始化
{
	const char fnName[] = "CCamera_CoaxLinkQuadCXP12_Camera::InitialCamera";
	char string[128]={0};

	CCamera_Basic::SaveCameraProcess(_T("InitialCamera"), MSG_LEVEL_HIGH);

	this->ReleaseCamera();
	if ( CCamera_CoaxLinkQuadCXP12_Camera::LoadCameraINIFile() == false ) { return false; }
	if ( CCamera_CoaxLinkQuadCXP12_Camera::SaveCameraINIFile() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	CEGrabber*   grabberPtr = NULL;
	try
	{	
		CString DeviceName;
		int interfaceIndex=0;//CardIndex
		const int deviceIndex=0;//CameraIndex		
		COAXPRESS_CAMERA_DEVICE CameraDeviceType=GetCameraDeviceType();
		//====================================================================================================//
		//因為只有一張擷取卡與一個相機需要控制，所以程式被限制在各只有一個的寫法。
		//因為已EGrabber讀取相機資訊太慢，所以改成以底層的GebICam來擷取
		//string Card = grabberPtr->getString<Euresys::InterfaceModule>("InterfaceID");
		//string serial=grabberPtr->getString<Euresys::InterfaceModule>("SerialNumber");
		//string Device = grabberPtr->getString<Euresys::DeviceModule>("DeviceID");		
		//string tl_Vendor=m_genTL.tlGetInfo<string>(tl_Handle, GenICam::Client::TL_INFO_VENDOR);
		//unsigned int nIterfaces=m_genTL.tlGetNumInterfaces(tl_Handle);		
		//====================================================================================================//	
		GenICam::Client::TL_HANDLE tl_Handle;		
		tl_Handle = m_genTL.tlOpen();
		int nInterfaces=(int)(m_genTL.tlGetNumInterfaces(tl_Handle));
		if ( 0 == nInterfaces )
		{
			m_genTL.tlClose(tl_Handle);
			m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Card count is exception (%d)"), nInterfaces);
			return false;
		}	
		if ( 1==nInterfaces || m_CardSerialNumber.GetLength()==0 )
		{	interfaceIndex = 0;	}
		else
		{
			if ( FindInterfaceIndex_v2(tl_Handle, nInterfaces, m_CardSerialNumber, interfaceIndex) == false )
			{
				m_genTL.tlClose(tl_Handle);
				m_ErrorString.Format(_T("Error, Exec No Match Frame Grabber Card Serial Number (%s)"), m_CardSerialNumber);
				return false;
			}
		}
		DeviceName = m_genTL.tlGetInterfaceID(tl_Handle,interfaceIndex).c_str();
		//printf("%s\n" , DeviceName);
		m_genTL.tlClose(tl_Handle);
		//====================================================================================================//				
		grabberPtr = new CEGrabber(m_genTL, interfaceIndex, deviceIndex);
		if ( NULL == grabberPtr )
		{
			m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Create grabberPtr Fault"));
			return false;
		}
		m_FVersion = grabberPtr->getInteger<Euresys::InterfaceModule>("FirmwareRevision");//取得韌體版本(不同的韌體版本會有不同的參數設定)		
		m_CardSerialNumber=CString(grabberPtr->getString<Euresys::InterfaceModule>("SerialNumber").c_str());
		CString DeviceModelName=CString(grabberPtr->getString<Euresys::InterfaceModule>("DeviceModelName").c_str()); 
		if ( DeviceModelName.CompareNoCase(_T("VC-25MX2-M150I1")) == 0 )
		{	CameraDeviceType = COAXPRESS_CAMERA_VC_25MX2_M150I;	}		
		else if ( DeviceModelName.CompareNoCase(_T("VCC-25CXPHSM")) == 0 )
		{	CameraDeviceType = COAXPRESS_CAMERA_VCC_25CXPHSM;	}		
		else if ( DeviceModelName.CompareNoCase(_T("STC-LBGP251BCXP124"))==0 || DeviceModelName.CompareNoCase(_T("STC-LBGP251BCXP124-TRA"))==0 )
		{	CameraDeviceType = COAXPRESS_CAMERA_STC_LBGP251BCXP124;	}		
		m_CameraDeviceType=CameraDeviceType; 		
		if ( SetupCameraDevice(CameraDeviceType, grabberPtr) == false )
		{
			delete grabberPtr; grabberPtr=NULL;
			return false;
		}			

		//Reset FrameGrabber Count
		if ( ResetFrameGrabberEventCount(grabberPtr) == false )
		{
			delete grabberPtr; grabberPtr=NULL;
			return false; 
		}

		//Test Free Run 
		if ( SwitchFreeRunMode(CameraDeviceType, grabberPtr) == false )
		{
			delete grabberPtr; grabberPtr=NULL;
			return false; 
		}
		
		//Test Software Trigger
		if ( SwitchSoftwareTriggerMode(CameraDeviceType, grabberPtr) == false )
		{
			delete grabberPtr; grabberPtr=NULL;
			return false; 
		}

		//Test External Trigger
		if ( SwitchExternalTriggerMode(CameraDeviceType, grabberPtr) == false )		
		{
			delete grabberPtr; grabberPtr=NULL;
			return false;  
		}
		m_CameraFPS=ReadCameraFrameRate(CameraDeviceType, grabberPtr);
		grabberPtr->SetCameraPtr(this);
		CCamera_CoaxLinkQuadCXP12_Camera::SaveCameraINIFile();
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s") , str);

		delete grabberPtr; grabberPtr=NULL;
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
		delete grabberPtr; grabberPtr=NULL;	
		return false;
	}
	SetGrabberPtr(grabberPtr);
	::memset(m_ClonedRawImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH);	
	//::memset(m_ClonedColorImagePtr, 0x00, sizeof(IMAGE_DATA)*ImageWH*4);

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
bool CCamera_CoaxLinkQuadCXP12_Camera::ReleaseCamera()//相機釋放
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ReleaseCamera"), MSG_LEVEL_HIGH);	
#ifndef CAMERA_OBJ_DISABLE
	CEGrabber *pGrabber=GetGrabberPtr();
	if ( CheckGrabberPtr(pGrabber) == false ) { return true; }	
	try
	{		
		pGrabber->execute<Euresys::RemoteModule>("AcquisitionStop");
		pGrabber->stop();
	}
	catch(const std::exception &e)
	{	
		CString str = e.what();
		m_ErrorString.Format(_T("Error, Stop Grab Fault::%s"), str);		
		SetGrabberPtr(NULL);
		delete pGrabber;
		pGrabber=NULL;
		return false;
	}
	SetGrabberPtr(NULL);
	delete pGrabber;
	pGrabber=NULL;	
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
bool CCamera_CoaxLinkQuadCXP12_Camera::ExecCameraPowerOff()//相機斷電
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ExecCameraPowerOff"), MSG_LEVEL_HIGH);
#ifndef CAMERA_OBJ_DISABLE	
	try
	{
		CEGrabber *pGrabber=GetGrabberPtr();
		if ( CheckGrabberPtr(pGrabber) == false ) { return true; }	
		pGrabber->execute<Euresys::InterfaceModule>("CxpPoCxpTurnOff");
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
bool CCamera_CoaxLinkQuadCXP12_Camera::ExecCameraPowerOn()//相機開電
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ExecCameraPowerOn"), MSG_LEVEL_HIGH);
#ifndef CAMERA_OBJ_DISABLE	
	try
	{
		CEGrabber *pGrabber=GetGrabberPtr();
		if ( CheckGrabberPtr(pGrabber) == false ) { return true; }	
		pGrabber->execute<Euresys::InterfaceModule>("CxpPoCxpAuto");
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
bool CCamera_CoaxLinkQuadCXP12_Camera::ExecCameraPowerReset()//相機復歸
{
	if ( this->CheckCameraInited() == false ) { return true; }	
	CCamera_Basic::SaveCameraProcess(_T("ExecCameraPowerReset"), MSG_LEVEL_HIGH);
#ifndef CAMERA_OBJ_DISABLE	
	try
	{
		CEGrabber *pGrabber=GetGrabberPtr();
		if ( CheckGrabberPtr(pGrabber) == false ) { return true; }	
		pGrabber->execute<Euresys::InterfaceModule>("CxpPoCxpTripReset");
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
bool CCamera_CoaxLinkQuadCXP12_Camera::ResetCamera()//復歸相機
{	
	SetCameraGrabbing_Unlock(false);	
	if ( ExecCameraPowerOff() == false ) { return false; }
	::Sleep(3000);
	if ( ExecCameraPowerOn() == false ) { return false; }
	::Sleep(3000);
	if ( ExecCameraPowerReset() == false ) { return false; }
	::Sleep(3000);
#ifndef CAMERA_OBJ_DISABLE
	CEGrabber *pGrabber=GetGrabberPtr();
	COAXPRESS_CAMERA_DEVICE CameraDeviceType=GetCameraDeviceType();
	if ( SwitchExternalTriggerMode(CameraDeviceType, pGrabber) == false )	{ return false; }
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::CheckCanSetExposureTime() const//確認是否可以設定曝光時間
{	
	bool bCanSet=true;		
	CAMERA_GRAB_MODE GrabMode=GetCameraGrabMode();
	COAXPRESS_CAMERA_DEVICE CameraDeviceType=GetCameraDeviceType();
	switch ( CameraDeviceType )
	{	
	case COAXPRESS_CAMERA_STC_LBGP251BCXP124:
		switch ( GrabMode )
		{
		case CAMERA_GRAB_EXTERNAL_TRIGGER:
			bCanSet = false;
			break;
		}
		break;
	}	
	return bCanSet;	
}
//-------------------------------------------------------------------------------------//
inline bool CCamera_CoaxLinkQuadCXP12_Camera::ReturnErrorNoCameraDeviceType(COAXPRESS_CAMERA_DEVICE CameraDevice)
{
	m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Camera Device Type Fault [%d]") , CameraDevice);
	return false;
}
//-------------------------------------------------------------------------------------//
inline bool CCamera_CoaxLinkQuadCXP12_Camera::CheckGrabberPtr()
{
#ifndef CAMERA_OBJ_DISABLE
	return CheckGrabberPtr(m_grabberPtr);	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
COAXPRESS_CAMERA_DEVICE CCamera_CoaxLinkQuadCXP12_Camera::GetCameraDeviceType() const//Coaxlink連到相機的樣式
{
	return m_CameraDeviceType;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::DoCoaxLinkStartGrab()
{
	if ( this->CheckCameraInited() == false ) { return false; }	
	CCamera_Basic::SaveCameraProcess(_T("DoCoaxLinkStartGrab"), MSG_LEVEL_HIGH);

	this->m_CameraImagePtr = NULL;
#ifndef CAMERA_OBJ_DISABLE
	if ( true == m_CameraGrabbing ) 
	{	return true; }

	if ( this->StopCameraGrab() == false ) { return false; }	
	try
	{	
		CEGrabber *pGrabber=GetGrabberPtr();
		if ( CheckGrabberPtr(pGrabber) == false ) { return false; }

		if ( GetCameraDeviceType() != COAXPRESS_CAMERA_VC_25MX2_M150I )//VC25MX2相機只能下一次指令
		{	pGrabber->execute<Euresys::RemoteModule>("AcquisitionStart"); }
		pGrabber->start();
		SetCameraGrabbing_Unlock(true);		
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE			
	if ( m_CameraGrabMode == Mode ) 
	{	return true; }
	CEGrabber *pGrabber=GetGrabberPtr();
	COAXPRESS_CAMERA_DEVICE CameraDeviceType=GetCameraDeviceType();
	if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
	//if ( this->StopCameraGrab() == false ) { return false; }	
	switch ( Mode )
	{
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
		if ( this->SwitchExternalTriggerMode(CameraDeviceType, pGrabber) == false )
		{	return false; }		
		break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		if ( this->SwitchSoftwareTriggerMode(CameraDeviceType, pGrabber) == false )
		{	return false; }		
		break;
	default://CAMERA_GRAB_FREE_RUN
		if ( this->SwitchFreeRunMode(CameraDeviceType, pGrabber) == false )
		{	return false; }		
		break;
	}
	SetCameraGrabMode(Mode);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetExposureTime(const int ExposureTime)
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	if ( m_ExposureTime_us == ExposureTime ) 
	{	return true; }
	const bool bCanSetExpTime=CheckCanSetExposureTime();
	if ( false == bCanSetExpTime )
	{	return true;	}
	try
	{		
		CEGrabber *pGrabber=GetGrabberPtr();
		if ( CheckGrabberPtr(pGrabber) == false ) { return false; }		
		
		CCamera_Basic::SaveCameraProcess(_T("SetExposureTime"), MSG_LEVEL_HIGH);

		pGrabber->setInteger<Euresys::RemoteModule>("ExposureTime" , ExposureTime);

		if( m_FVersion < COAXLINK_FIREWARE_VERSION_217 )
		{	pGrabber->setInteger<Euresys::DeviceModule>("ExposureTime" , ExposureTime);		}
		else
		{
			if ( CAMERA_GRAB_EXTERNAL_TRIGGER != m_CameraGrabMode )
			{	pGrabber->setInteger<Euresys::DeviceModule>("ExposureTime" , ExposureTime);		}
		}
	
		m_ExposureTime_us = ExposureTime;
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::StartCameraGrab()//只取一張影像-內部觸發用
{
	CCamera_Basic::SaveCameraProcess(_T("StartCameraGrab"), MSG_LEVEL_HIGH);

	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();
	if ( this->DoCoaxLinkStartGrab() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::StartCameraLiveGrab()//連續取多張影像-外部觸發用
{
	CCamera_Basic::SaveCameraProcess(_T("StartCameraLiveGrab"), MSG_LEVEL_HIGH);

	this->ResetCameraExposureFinishEvent();
	this->ResetCameraGrabFinishEvent();

	if ( this->DoCoaxLinkStartGrab() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::StopCameraGrab()//停止取像
{
#ifndef CAMERA_OBJ_DISABLE	
	try
	{		
		CEGrabber *pGrabber=GetGrabberPtr();
		if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
		
		CCamera_Basic::SaveCameraProcess(_T("StopCameraGrab"), MSG_LEVEL_HIGH);
		pGrabber->execute<Euresys::RemoteModule>("AcquisitionStop");
		pGrabber->stop();
		SetCameraGrabbing_Unlock(false);
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::FireSoftwareTrigger()//發射軟體觸發訊號
{
#ifndef CAMERA_OBJ_DISABLE	
	try
	{		
		CEGrabber *pGrabber=GetGrabberPtr();
		if ( CheckGrabberPtr(pGrabber) == false ) { return false; }		
		
		CCamera_Basic::SaveCameraProcess(_T("FireSoftwareTrigger"), MSG_LEVEL_HIGH);
		pGrabber->start(1);
		pGrabber->execute<Euresys::DeviceModule>("StartCycle");		
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::WaitforCameraReadytoTrigger()//等待相機準備好可以觸發
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("WaitforCameraReadytoTrigger"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::ResetCameraReadyTriggerEvent()//復歸相機準備好了的事件
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("ResetCameraReadyTriggerEvent"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::DoCameraDebayer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const unsigned char *pRaw, unsigned char *pResult, BAYER_PATTERN_MODE BayerPattern)
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("DoCameraDebayer"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::GetWhiteBalanceParams(double &WBR, double &WBG, double &WBB)
{
#ifndef CAMERA_OBJ_DISABLE	
	WBR = 1.0;
	WBG = 1.0;
	WBB = 1.0;
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetWhiteBalanceParams(double WBR, double WBG, double WBB)
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("SetWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::CalcWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("CalcWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::ResetWhiteBalanceParams()
{
#ifndef CAMERA_OBJ_DISABLE	
	CCamera_Basic::SaveCameraProcess(_T("ResetWhiteBalanceParams"), MSG_LEVEL_HIGH);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
double CCamera_CoaxLinkQuadCXP12_Camera::ReadCameraTemperature()//讀取相機溫度
{
	double Temperature=0.0;
#ifndef CAMERA_OBJ_DISABLE	
	try
	{		
		CEGrabber *pGrabber=GetGrabberPtr();
		if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
		
		CCamera_Basic::SaveCameraProcess(_T("ReadCameraTemperature"), MSG_LEVEL_HIGH);
		Temperature = pGrabber->getFloat<Euresys::RemoteModule>("DeviceTemperature"); //設定相機感測元件異常需要處理(可能回臨近均值)
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return false;
	}
#endif//CAMERA_OBJ_DISABLE
	return Temperature;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::WriteCameraParameterToDevice()//儲存目前相機的參數至相機內部的韌體上
{
#ifndef CAMERA_OBJ_DISABLE	
	
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SaveCameraINIFile()
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
bool CCamera_CoaxLinkQuadCXP12_Camera::LoadCameraINIFile()
{
	if ( CCamera_Basic::LoadCameraINIFile() == false ) { return false; }

	this->SetCameraModelID(CAMERA_OBJ_COAXLINK_QUAD_CXP12_CAMERA);	

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
		case COAXPRESS_CAMERA_VC_12MX_M180:
		case COAXPRESS_CAMERA_VCC_25CXPHSM:
		case COAXPRESS_CAMERA_VC_25MX2_M150I:
		case COAXPRESS_CAMERA_STC_LBGP251BCXP124:
			m_CameraDeviceType=(COAXPRESS_CAMERA_DEVICE)(CameraDevice);
			break;
		default:
			m_CameraDeviceType = COAXPRESS_CAMERA_VCC_25CXPHSM;
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
CEGrabber*  CCamera_CoaxLinkQuadCXP12_Camera::GetGrabberPtr()
{
	return m_grabberPtr;
}
//-------------------------------------------------------------------------------------//
CEGrabber* CCamera_CoaxLinkQuadCXP12_Camera::CreateGrabberPtr(int interfaceIndex, int deviceIndex)
{
	CEGrabber *pGrabber=NULL;
	try
	{
		pGrabber=new CEGrabber(m_genTL, interfaceIndex, deviceIndex);				
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s") , str);
		delete pGrabber; pGrabber=NULL;		
		return NULL;
	}	
	return pGrabber;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetGrabberPtr(CEGrabber *Ptr)
{
	if ( CheckGrabberPtr(Ptr) == true )
	{
		if ( CheckGrabberPtr(m_grabberPtr) == true )
		{	return false; }	
	}
	m_grabberPtr = Ptr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::CheckGrabberPtr(CEGrabber *pGrabber)
{
	if ( NULL == pGrabber )
	{
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera pGrabber is NULL"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::FindInterfaceIndex(int nInterfaces, LPCTSTR CardSerial, int &interfaceIndex)
{	
	try
	{
		interfaceIndex = -1;
		const int deviceIndex = 0;
		for ( int i=0; i<nInterfaces; i++ )
		{	
			CEGrabber *pGrabber = CreateGrabberPtr(i, deviceIndex);
			if ( NULL == pGrabber ) { return false; }
			CString SerialNumber=CString(pGrabber->getString<Euresys::InterfaceModule>("SerialNumber").c_str());
			delete pGrabber; pGrabber=NULL;		
			if ( SerialNumber.CompareNoCase(CardSerial) != 0 )
			{	continue;	}
			interfaceIndex = i;
			return true;
		}
		return false;
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s") , str);		
		return false;
	}	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::FindInterfaceIndex_v2(GenICam::Client::TL_HANDLE &tl_Handle, int nInterfaces, LPCTSTR CardSerial, int &interfaceIndex)
{
	try
	{
		interfaceIndex = -1;
		for ( int i=0; i<nInterfaces; i++ )
		{			
			CString SerialNumber = m_genTL.tlGetInterfaceID(tl_Handle, i).c_str();
			const int Pos = SerialNumber.Find(CardSerial);
			if ( -1 == Pos )			
			{	continue;	}
			interfaceIndex = i;
			return true;
		}
		return false;
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s") , str);		
		return false;
	}	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetCoaxLinkFuncInt64(EURESYS_COAXLINK_MODULE Module, CEGrabber *pGrabber, const char *Key, int64_t Value)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		switch (Module)
		{
		case EURESYS_COAXLINK_SYSTEM:	 pGrabber->setInteger<Euresys::SystemModule>(Key, Value); break;
		case EURESYS_COAXLINK_INTERFACE: pGrabber->setInteger<Euresys::InterfaceModule>(Key, Value); break;
		case EURESYS_COAXLINK_DEVICE:	 pGrabber->setInteger<Euresys::DeviceModule>(Key, Value); break;
		case EURESYS_COAXLINK_REMOTE:	 pGrabber->setInteger<Euresys::RemoteModule>(Key, Value); break;
		case EURESYS_COAXLINK_STREAM:	 pGrabber->setInteger<Euresys::StreamModule>(Key, Value); break;
			break;
		default:
			m_ErrorString.Format(_T("Error, SetCoaxLinkFuncInt64 Fault[Score%d]"), Module);
			return false;
			break;
		}
	}
	catch (std::exception &e)
	{
		CString sValue;
		CString sKey(Key);
		CString str = e.what();
		sValue.Format(_T("%.6f"), Value);
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Set[%s => %s] Fault\n%s"), sKey, sValue, str);
		return false;
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetCoaxLinkFuncFlt(EURESYS_COAXLINK_MODULE Module, CEGrabber *pGrabber, const char *Key, float Value)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		switch (Module)
		{
		case EURESYS_COAXLINK_SYSTEM:	 pGrabber->setFloat<Euresys::SystemModule>(Key, Value); break;
		case EURESYS_COAXLINK_INTERFACE: pGrabber->setFloat<Euresys::InterfaceModule>(Key, Value); break;
		case EURESYS_COAXLINK_DEVICE:	 pGrabber->setFloat<Euresys::DeviceModule>(Key, Value); break;
		case EURESYS_COAXLINK_REMOTE:	 pGrabber->setFloat<Euresys::RemoteModule>(Key, Value); break;
		case EURESYS_COAXLINK_STREAM:	 pGrabber->setFloat<Euresys::StreamModule>(Key, Value); break;
			break;
		default:
			m_ErrorString.Format(_T("Error, SetCoaxLinkFuncFlt Fault[Score%d]"), Module);
			return false;
			break;
		}
	}
	catch (std::exception &e)
	{
		CString sValue;
		CString sKey(Key);		
		CString str = e.what();
		sValue.Format(_T("%I64d"), Value);
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Set[%s => %s] Fault\n%s"), sKey, sValue, str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetCoaxLinkFuncStr(EURESYS_COAXLINK_MODULE Module, CEGrabber *pGrabber, const char *Key, const char *Value)
{
	if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
	try
	{
		switch (Module)
		{
		case EURESYS_COAXLINK_SYSTEM:	 pGrabber->setString<Euresys::SystemModule>(Key, Value); break;
		case EURESYS_COAXLINK_INTERFACE: pGrabber->setString<Euresys::InterfaceModule>(Key, Value); break;
		case EURESYS_COAXLINK_DEVICE:	 pGrabber->setString<Euresys::DeviceModule>(Key, Value); break;
		case EURESYS_COAXLINK_REMOTE:	 pGrabber->setString<Euresys::RemoteModule>(Key, Value); break;
		case EURESYS_COAXLINK_STREAM:	 pGrabber->setString<Euresys::StreamModule>(Key, Value); break;
			break;
		default:
			m_ErrorString.Format(_T("Error, SetCoaxLinkFuncStr Fault[Score%d]"), Module);
			return false;
			break;
		}
	}
	catch (std::exception &e)
	{		
		CString sKey(Key);
		CString sValue(Value);
		CString str = e.what();				
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Set[%s => %s] Fault\n%s"), sKey, sValue, str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetupCameraDevice(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber)//設定相機
{
	bool bIsOK = true;
	switch ( CameraDevice )
	{
	case COAXPRESS_CAMERA_VC_12MX_M180:		bIsOK=SetupCameraDevice_VC_12MX_M180(pGrabber); break;
	case COAXPRESS_CAMERA_VCC_25CXPHSM:		bIsOK=SetupCameraDevice_VCC_25CXPHSM(pGrabber); break;
	case COAXPRESS_CAMERA_VC_25MX2_M150I:	bIsOK=SetupCameraDevice_VC_25MX2_M150I(pGrabber); break;
	case COAXPRESS_CAMERA_STC_LBGP251BCXP124:	bIsOK = SetupCameraDevice_STC_LBGP251BCXP124(pGrabber); break;
	default:								bIsOK=ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetupCameraDevice_VC_12MX_M180(CEGrabber *pGrabber)
{
	if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SetupCameraDevice_VC_12MX_M180"), MSG_LEVEL_HIGH);
		//About Reset Camera System
		pGrabber->execute<Euresys::RemoteModule>("AcquisitionStop");
		//pGrabber->setString<Euresys::RemoteModule>("ConnectionConfig" , "CXP3_X1");	//設定相機連線的配置(幾條線)，預設是CXP3_X1(約全速的八分之一)，因此設定為CXP6_X4全速。
		//pGrabber->setString<Euresys::RemoteModule>("ConnectionConfig" , "CXP6_X4");	//設定相機連線的配置(幾條線)，預設是CXP3_X1(約全速的八分之一)，因此設定為CXP6_X4全速。
		pGrabber->setString<Euresys::RemoteModule>("CxpLinkConfiguration", "CXP6_X4");	//設定相機連線的配置(幾條線)，預設是CXP3_X1(約全速的八分之一)，因此設定為CXP6_X4全速。
		pGrabber->setString<Euresys::RemoteModule>("PixelFormat" , "Mono8");	//設定相機像素格式
		//pGrabber->setString<Euresys::RemoteModule>("FlashStrobeMode" , "Disabled");//關閉相機輸出閃燈訊號模式
		pGrabber->setString<Euresys::RemoteModule>("LineSource" , "Off");//關閉相機線來源
		pGrabber->setString<Euresys::RemoteModule>("HDRMode" ,"Off");	//關閉相機HDR模式
		pGrabber->setFloat<Euresys::RemoteModule>("Gain" , 1.0f);		//增益設定為1.0(最大值為32.0)
		pGrabber->setFloat<Euresys::RemoteModule>("BlackLevel" , 1.0f);	//黑值設定為1.0(最大值為511.0)		
		pGrabber->setString<Euresys::RemoteModule>("DefectivePixelCorrection" , "True"); //設定相機感測元件異常需要處理(可能回臨近均值)		
		//====================================================================================================//		
		//設定相機取像大小的初始值
		unsigned int ImageSizeW = this->GetCameraImageW();
		unsigned int ImageSizeH = this->GetCameraImageH();
		int64_t sensorwidth = pGrabber->getInteger<Euresys::RemoteModule>("SensorWidth");		//取得相機最大寬度
		int64_t sensorheight = pGrabber->getInteger<Euresys::RemoteModule>("SensorHeight");	//取得相機最大高度
		pGrabber->setInteger<Euresys::RemoteModule>("Width" , sensorwidth);	//設定相機擷取影像的寬度
		int64_t CameraSizeW = pGrabber->getInteger<Euresys::RemoteModule>("Width");
		pGrabber->setInteger<Euresys::RemoteModule>("Height" , sensorheight);//設定相機擷取影像的高度	
		int64_t CameraSizeH = pGrabber->getInteger<Euresys::RemoteModule>("Height");		
		if ( CameraSizeW!=ImageSizeW || CameraSizeH!=ImageSizeH )
		{
			this->m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Camea Size Exception(%d, %d)"), CameraSizeW, CameraSizeH);		
			return false;	
		}
		//====================================================================================================//		
		//設定相機黑場(DarkField)
		const bool bSetDarkField = false;
		if ( true == bSetDarkField )
		{
			pGrabber->setString<Euresys::RemoteModule>("DF_BlackClamp" , "False");
			pGrabber->setString<Euresys::RemoteModule>("DF_ColumnOffsetCorrection" , "True");//True
			pGrabber->execute<Euresys::RemoteModule>("DF_RestoreFactory");
		}
		//====================================================================================================//
		//設定相機白場(BrightField)
		const bool bSetBrightField = false;
		if ( true == bSetBrightField )
		{
			pGrabber->setString<Euresys::RemoteModule>("BF_ColumnGainCorrection" , "True");//True
			pGrabber->setInteger<Euresys::RemoteModule>("BF_CalibrationVideoLevel" , 65);
			pGrabber->setString<Euresys::RemoteModule>("BF_OutputImagesDuringCalibration" , "False");
			pGrabber->execute<Euresys::RemoteModule>("BF_RestoreFactory");
		}
		//設定FFC
		pGrabber->setString<Euresys::RemoteModule>("FfcMode", "Off");
		//FPS
		double CameraFPS = pGrabber->getFloat<Euresys::RemoteModule>("AcquisitionFrameRate"); //設定相機感測元件異常需要處理(可能回臨近均值)
		CCamera_Basic::m_CameraFPS = CameraFPS;		
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s") , str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetupCameraDevice_VCC_25CXPHSM(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SetupCameraDevice_VCC_25CXPHSM"), MSG_LEVEL_HIGH);
		//About Reset Camera System
		pGrabber->execute<Euresys::RemoteModule>("AcquisitionStop");
		std::string CxpMode = pGrabber->getString<Euresys::RemoteModule>("ConnectionConfig");
		if (CxpMode != "CXP12_X4")//設定相機連線的配置(幾條線)，設定為CXP12_X4全速
		{	pGrabber->setString<Euresys::RemoteModule>("ConnectionConfig", "CXP12_X4");		}
		pGrabber->setString<Euresys::RemoteModule>("PixelFormat", "Mono8");	//設定相機像素格式		
		pGrabber->setString<Euresys::RemoteModule>("LineSource", "Off");//關閉相機線來源		
		//pGrabber->setFloat<Euresys::RemoteModule>("Gain", 1.0f);		//增益設定為1.0
		//pGrabber->setFloat<Euresys::RemoteModule>("BlackOffset", 40.0f);	//黑值設定為1.0
		//pGrabber->setString<Euresys::RemoteModule>("DefectivePixelCorrection", "On"); //設定相機感測元件異常需要處理(可能回臨近均值)-不支援
		//====================================================================================================//		
		//設定相機取像大小的初始值		
		unsigned int ImageSizeW = this->GetCameraImageW();
		unsigned int ImageSizeH = this->GetCameraImageH();
		
		int64_t sensorwidth = pGrabber->getInteger<Euresys::RemoteModule>("WidthMax");//取得相機最大寬度
		int64_t sensorheight = pGrabber->getInteger<Euresys::RemoteModule>("HeightMax");	//取得相機最大高度
		
		//設定相機影像區域//EffectiveRegion[FullFrame], Region1~16[Roi]
		const std::string sRegionSelector=pGrabber->getString<Euresys::RemoteModule>("RegionSelector");		
		if ( sRegionSelector != "EffectiveRegion" )
		{
			pGrabber->setInteger<Euresys::RemoteModule>("Width", sensorwidth);	//設定相機擷取影像的寬度//須[RegionSelector]設定成不是[EffectiveRegion]
			pGrabber->setInteger<Euresys::RemoteModule>("Height", sensorheight);//設定相機擷取影像的高度//須[RegionSelector]設定成不是[EffectiveRegion]
		}
		int64_t CameraSizeW = pGrabber->getInteger<Euresys::RemoteModule>("Width");		
		int64_t CameraSizeH = pGrabber->getInteger<Euresys::RemoteModule>("Height");
		if (CameraSizeW != ImageSizeW || CameraSizeH != ImageSizeH)
		{
			this->m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Camea Size Exception(%d, %d)"), CameraSizeW, CameraSizeH);
			return false;
		}
		//====================================================================================================//
		//FPS
		//double CameraFPS = pGrabber->getFloat<Euresys::RemoteModule>("AcquisitionFrameRate"); //取回相機取像速度-未支援, 以相機規格為主
		double CameraFPS = 150; //取回相機取像速度-未支援, 以相機規格為主
		CCamera_Basic::m_CameraFPS = CameraFPS;
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetupCameraDevice_VC_25MX2_M150I(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SetupCameraDevice_VC_25MX2_M150I"), MSG_LEVEL_HIGH);
		//About Reset Camera System
		pGrabber->execute<Euresys::RemoteModule>("AcquisitionStop");
		std::string CxpMode=pGrabber->getString<Euresys::RemoteModule>("CxpLinkConfiguration");
		if ( CxpMode != "CXP12_X4" )//設定相機連線的配置(幾條線)，設定為CXP12_X4全速-有時設定會造成異常
		{	
			pGrabber->setString<Euresys::RemoteModule>("CxpLinkConfiguration", "CXP12_X4");		
			::Sleep(5000);
		}
		pGrabber->setString<Euresys::RemoteModule>("PixelFormat", "Mono8");	//設定相機像素格式		

		pGrabber->setString<Euresys::RemoteModule>("LineSelector", "Line1");//切換線路通道
		pGrabber->setString<Euresys::RemoteModule>("LineMode", "Output");//設定線路模式
		pGrabber->setString<Euresys::RemoteModule>("LineSource", "Off");//關閉相機線來源		
		//pGrabber->setFloat<Euresys::RemoteModule>("Gain", 1.0f);		//增益設定為1.0
		//pGrabber->setFloat<Euresys::RemoteModule>("BlackOffset", 0.0f);	//黑值設定為1.0
		//pGrabber->setString<Euresys::RemoteModule>("DefectivePixelCorrection", "On"); //設定相機感測元件異常需要處理(可能回臨近均值)-不支援
		//====================================================================================================//		
		//設定相機取像大小的初始值		
		unsigned int ImageSizeW = this->GetCameraImageW();
		unsigned int ImageSizeH = this->GetCameraImageH();

		int64_t sensorwidth = pGrabber->getInteger<Euresys::RemoteModule>("WidthMax");//取得相機最大寬度
		int64_t sensorheight = pGrabber->getInteger<Euresys::RemoteModule>("HeightMax");	//取得相機最大高度

		//設定相機影像區域//EffectiveRegion[FullFrame], Region1~16[Roi]
		//const std::string sRegionSelector = pGrabber->getString<Euresys::RemoteModule>("RegionSelector");
		//if (sRegionSelector != "EffectiveRegion")
		//{
		pGrabber->setInteger<Euresys::RemoteModule>("Width", sensorwidth);	//設定相機擷取影像的寬度//須[RegionSelector]設定成不是[EffectiveRegion]
		pGrabber->setInteger<Euresys::RemoteModule>("Height", sensorheight);//設定相機擷取影像的高度//須[RegionSelector]設定成不是[EffectiveRegion]
		//}
		int64_t CameraSizeW = pGrabber->getInteger<Euresys::RemoteModule>("Width");
		int64_t CameraSizeH = pGrabber->getInteger<Euresys::RemoteModule>("Height");
		if (CameraSizeW != ImageSizeW || CameraSizeH != ImageSizeH)
		{
			this->m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Camea Size Exception(%d, %d)"), CameraSizeW, CameraSizeH);
			return false;
		}
		//====================================================================================================//
		//FPS
		double CameraFPS = pGrabber->getFloat<Euresys::RemoteModule>("AcquisitionFrameRate"); //取回相機取像速度-未支援, 以相機規格為主
		//double CameraFPS = 150; //取回相機取像速度-未支援, 以相機規格為主
		CCamera_Basic::m_CameraFPS = CameraFPS;
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SetupCameraDevice_STC_LBGP251BCXP124(CEGrabber * pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SetupCameraDevice_STC_LBGP251BCXP124"), MSG_LEVEL_HIGH);
		//About Reset Camera System		
		pGrabber->execute<Euresys::RemoteModule>("AcquisitionStop");
		std::string CxpMode = pGrabber->getString<Euresys::RemoteModule>("CxpLinkConfiguration");
		//pGrabber->setString<Euresys::RemoteModule>("ConnectionConfig" , "CXP6_X4");	//設定相機連線的配置(幾條線)，預設是CXP3_X1(約全速的八分之一)，因此設定為CXP6_X4全速。
		if (CxpMode != "CXP12_X4")//設定相機連線的配置(幾條線)，設定為CXP12_X4全速-有時設定會造成異常
		{
			pGrabber->setString<Euresys::RemoteModule>("CxpLinkConfiguration", "CXP12_X4");
			::Sleep(5000);
		}
		pGrabber->setString<Euresys::RemoteModule>("PixelFormat", "Mono8");	//設定相機像素格式
		//pGrabber->setString<Euresys::RemoteModule>("FlashStrobeMode" , "Disabled");//關閉相機輸出閃燈訊號模式
		//pGrabber->setString<Euresys::RemoteModule>("LineSource" , "StrobeOut");//相機線來源-StrobeOut-不支援//20240514
		//pGrabber->setString<Euresys::RemoteModule>("HDRMode" ,"Off");	//關閉相機HDR模式-不支援
		pGrabber->setFloat<Euresys::RemoteModule>("Gain", 1.0f);		//增益設定為1.0(最大值為32.0)
		pGrabber->setFloat<Euresys::RemoteModule>("BlackLevel", 8.0f);	//黑值設定為8.0(最大值為511.0), 預設值為8.0//20240514
		//pGrabber->setString<Euresys::RemoteModule>("DefectivePixelCorrection" , "True"); //設定相機感測元件異常需要處理(可能回臨近均值)-不支援//20240514		
		//====================================================================================================//		
		//設定相機取像大小的初始值
		unsigned int ImageSizeW = this->GetCameraImageW();
		unsigned int ImageSizeH = this->GetCameraImageH();

		int64_t sensorwidth = pGrabber->getInteger<Euresys::RemoteModule>("SensorWidth"); //取得相機最大寬度
		int64_t sensorheight = pGrabber->getInteger<Euresys::RemoteModule>("SensorHeight");	//取得相機最大高度

		//設定相機影像區域//EffectiveRegion[FullFrame], Region1~16[Roi]
		//const std::string sRegionSelector = pGrabber->getString<Euresys::RemoteModule>("RegionSelector");
		//if (sRegionSelector != "EffectiveRegion")
		//{
		//	pGrabber->setInteger<Euresys::RemoteModule>("Width", sensorwidth);	//設定相機擷取影像的寬度//須[RegionSelector]設定成不是[EffectiveRegion]
		//	pGrabber->setInteger<Euresys::RemoteModule>("Height", sensorheight);//設定相機擷取影像的高度//須[RegionSelector]設定成不是[EffectiveRegion]
		//}

		pGrabber->setInteger<Euresys::RemoteModule>("Height", sensorheight);//設定相機擷取影像的高度	
		pGrabber->setInteger<Euresys::RemoteModule>("Width", sensorwidth);	//設定相機擷取影像的寬度

		int64_t CameraSizeH = pGrabber->getInteger<Euresys::RemoteModule>("Height");
		int64_t CameraSizeW = pGrabber->getInteger<Euresys::RemoteModule>("Width");


		if (CameraSizeW != ImageSizeW || CameraSizeH != ImageSizeH)
		{
			this->m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera Camea Size Exception(%d, %d)"), CameraSizeW, CameraSizeH);
			return false;
		}
		//FPS
		double CameraFPS = pGrabber->getFloat<Euresys::RemoteModule>("AcquisitionFrameRate"); //設定相機感測元件異常需要處理(可能回臨近均值)
		CCamera_Basic::m_CameraFPS = CameraFPS;
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::ResetFrameGrabberEventCount(CEGrabber *pGrabber)//清除影像擷取卡的事件計數器
{
	if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
	try
	{	
		CCamera_Basic::SaveCameraProcess(_T("ResetFrameGrabberEventCount"), MSG_LEVEL_HIGH);

		//設定擷取卡外埠觸發的計數器與重置計數器
		pGrabber->setString<Euresys::InterfaceModule>("EventSelector" , "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("EventNotification" , "true");
		pGrabber->setString<Euresys::InterfaceModule>("EventNotificationContext1" , "EventSpecific");
		pGrabber->setString<Euresys::InterfaceModule>("EventNotificationContext2" , "EventSpecific");
		pGrabber->setString<Euresys::InterfaceModule>("EventNotificationContext3" , "EventSpecific");
		pGrabber->execute<Euresys::InterfaceModule>("EventCountReset");
		//重置擷取卡傳輸到相機的觸發遺失計數器
		if( m_FVersion < COAXLINK_FIREWARE_VERSION_217 )//舊版才有支援以下參數//disable@coaxlink_7.1.1.25
		{	pGrabber->execute<Euresys::DeviceModule>("CycleLostTriggerCountReset");	}
		//設定相機正源觸發次數與重置計數器
		pGrabber->setString<Euresys::DeviceModule>("EventSelector" , "CameraTriggerRisingEdge");
		pGrabber->setString<Euresys::DeviceModule>("EventNotification" , "true");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext1" , "EventSpecific");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext2" , "EventSpecific");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext3" , "EventSpecific");
		pGrabber->execute<Euresys::DeviceModule>("EventCountReset");
		//設定相機負源觸發次數與重置計數器
		pGrabber->setString<Euresys::DeviceModule>("EventSelector" , "CameraTriggerFallingEdge");
		pGrabber->setString<Euresys::DeviceModule>("EventNotification" , "true");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext1" , "EventSpecific");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext2" , "EventSpecific");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext3" , "EventSpecific");
		pGrabber->execute<Euresys::DeviceModule>("EventCountReset");
		//設定相機觸發次數與重置計數器
		pGrabber->setString<Euresys::DeviceModule>("EventSelector" , "Trigger");
		pGrabber->setString<Euresys::DeviceModule>("EventNotification" , "true");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext1" , "EventSpecific");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext2" , "EventSpecific");
		pGrabber->setString<Euresys::DeviceModule>("EventNotificationContext3" , "EventSpecific");
		pGrabber->execute<Euresys::DeviceModule>("EventCountReset");
		//設定開始相機到電腦記憶體傳送圖片的次數與重置計數器
		pGrabber->setString<Euresys::StreamModule>("EventSelector" , "StartOfCameraReadout");
		pGrabber->setString<Euresys::StreamModule>("EventNotification" , "true");
		pGrabber->setString<Euresys::StreamModule>("EventNotificationContext1" , "EventSpecific");
		pGrabber->setString<Euresys::StreamModule>("EventNotificationContext2" , "EventSpecific");
		pGrabber->setString<Euresys::StreamModule>("EventNotificationContext3" , "EventSpecific");
		pGrabber->execute<Euresys::StreamModule>("EventCountReset");
		//設定完成相機到電腦記憶體傳送圖片的次數與重置計數器
		pGrabber->setString<Euresys::StreamModule>("EventSelector" , "EndOfCameraReadout");
		pGrabber->setString<Euresys::StreamModule>("EventNotification" , "true");
		pGrabber->setString<Euresys::StreamModule>("EventNotificationContext1" , "EventSpecific");
		pGrabber->setString<Euresys::StreamModule>("EventNotificationContext2" , "EventSpecific");
		pGrabber->setString<Euresys::StreamModule>("EventNotificationContext3" , "EventSpecific");
		pGrabber->execute<Euresys::StreamModule>("EventCountReset");
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchFreeRunMode(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber)
{
	bool bIsOK = true;
	switch ( CameraDevice )
	{
	case COAXPRESS_CAMERA_VC_12MX_M180:		bIsOK=SwitchFreeRunMode_VC_12MX_M180(pGrabber); break;
	case COAXPRESS_CAMERA_VCC_25CXPHSM:		bIsOK=SwitchFreeRunMode_VCC_25CXPHSM(pGrabber); break;
	case COAXPRESS_CAMERA_VC_25MX2_M150I:	bIsOK=SwitchFreeRunMode_VC_25MX2_M150I(pGrabber); break;
	case COAXPRESS_CAMERA_STC_LBGP251BCXP124:	bIsOK = SwitchFreeRunMode_STC_LBGP251BCXP124(pGrabber); break;
	default:								bIsOK=ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	if ( true == bIsOK )
	{	SetCameraGrabMode(CAMERA_GRAB_FREE_RUN); }
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchFreeRunMode_VC_12MX_M180(CEGrabber *pGrabber)
{
	if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
	try
	{	
		const int ExposureTime = this->GetExposureTime();
		CCamera_Basic::SaveCameraProcess(_T("SwitchFreeRunMode_VC_12MX_M180"), MSG_LEVEL_HIGH);
		//設定相機連續取像狀態
		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod","NC");
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode" , "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource" , "CXPin");
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation" , "RisingEdge");//FallingEdge		
		pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "Timed");
		//pGrabber->setInteger<Euresys::DeviceModule>("ExposureTime" , ExposureTime);			
		//pGrabber->execute<Euresys::RemoteModule>("AcquisitionMaxFrameRate");
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchFreeRunMode_VCC_25CXPHSM(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		const int ExposureTime = this->GetExposureTime();
		CCamera_Basic::SaveCameraProcess(_T("SwitchFreeRunMode_VCC_25CXPHSM"), MSG_LEVEL_HIGH);
		//設定相機連續取像狀態
		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "NC");
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation", "RisingEdge");//FallingEdge		
		//pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "Timed");
		//pGrabber->setInteger<Euresys::DeviceModule>("ExposureTime" , ExposureTime);			
		//pGrabber->execute<Euresys::RemoteModule>("AcquisitionMaxFrameRate");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchFreeRunMode_VC_25MX2_M150I(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		const int ExposureTime = this->GetExposureTime();
		CCamera_Basic::SaveCameraProcess(_T("SwitchFreeRunMode_VC_25MX2_M150I"), MSG_LEVEL_HIGH);
		//設定相機連續取像狀態
		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "NC");
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation", "RisingEdge");//FallingEdge		
		//pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "Timed");
		//pGrabber->setInteger<Euresys::DeviceModule>("ExposureTime" , ExposureTime);			
		//pGrabber->execute<Euresys::RemoteModule>("AcquisitionMaxFrameRate");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchFreeRunMode_STC_LBGP251BCXP124(CEGrabber * pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		const int ExposureTime = this->GetExposureTime();
		CCamera_Basic::SaveCameraProcess(_T("SwitchFreeRunMode_STC_LBGP251BCXP124"), MSG_LEVEL_HIGH);
		//設定相機連續取像狀態
		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "NC");
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		//pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "Software");//"CXPin"20240514
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
																					//pGrabber->setString<Euresys::RemoteModule>("TriggerActivation" , "RisingEdge");//FallingEdge-不支援//20240514
		pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "Timed");//20240514
		pGrabber->setInteger<Euresys::RemoteModule>("ExposureTime", ExposureTime);
		//pGrabber->execute<Euresys::RemoteModule>("AcquisitionMaxFrameRate");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchSoftwareTriggerMode(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber)
{
	bool bIsOK = true;
	switch ( CameraDevice )
	{
	case COAXPRESS_CAMERA_VC_12MX_M180:		bIsOK=SwitchSoftwareTriggerMode_VC_12MX_M180(pGrabber); break;
	case COAXPRESS_CAMERA_VCC_25CXPHSM:		bIsOK=SwitchSoftwareTriggerMode_VCC_25CXPHSM(pGrabber); break;
	case COAXPRESS_CAMERA_VC_25MX2_M150I:	bIsOK=SwitchSoftwareTriggerMode_VC_25MX2_M150I(pGrabber); break;
	case COAXPRESS_CAMERA_STC_LBGP251BCXP124:	bIsOK = SwitchSoftwareTriggerMode_STC_LBGP251BCXP124(pGrabber); break;
	default:								bIsOK=ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	if ( true == bIsOK )
	{	SetCameraGrabMode(CAMERA_GRAB_SOFTWARE_TRIGGER); }
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchSoftwareTriggerMode_VC_12MX_M180(CEGrabber *pGrabber)
{
	if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
	try
	{		
		CCamera_Basic::SaveCameraProcess(_T("SwitchSoftwareTriggerMode_VC_12MX_M180"), MSG_LEVEL_HIGH);

		//設定相機軟體觸發狀態
		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime   = ExposureTime+1000;
		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod","RG");
		pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap" , "true");
		pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime" , 1000);
	//	pGrabber->setFloat<Euresys::DeviceModule>("ExposureTime" , 4600);
		pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "StartCycle");
		pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode" , "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "CXPin");
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation" , "FallingEdge");//RisingEdge, FallingEdge
		pGrabber->setString<Euresys::RemoteModule>("ExposureMode" , "TriggerWidth");
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchSoftwareTriggerMode_VCC_25CXPHSM(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchSoftwareTriggerMode_VCC_25CXPHSM"), MSG_LEVEL_HIGH);

		//設定相機軟體觸發狀態
		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = ExposureTime + 1000;
		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "RG");
		pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");
		pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);
		//	pGrabber->setFloat<Euresys::DeviceModule>("ExposureTime" , 4600);
		pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "StartCycle");
		pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation", "FallingEdge");//RisingEdge, FallingEdge, LevelHigh, LevelLow
		//pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchSoftwareTriggerMode_VC_25MX2_M150I(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchSoftwareTriggerMode_VC_25MX2_M150I"), MSG_LEVEL_HIGH);

		//設定相機軟體觸發狀態
		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = ExposureTime + 1000;
		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "RG");
		pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");
		pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);
		//	pGrabber->setFloat<Euresys::DeviceModule>("ExposureTime" , 4600);
		pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "StartCycle");
		pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);
		//pGrabber->setString<Euresys::RemoteModule>("TriggerSelector", "ExposureStart");
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "Software");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation", "FallingEdge");//RisingEdge, FallingEdge, LevelHigh, LevelLow
		//pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchSoftwareTriggerMode_STC_LBGP251BCXP124(CEGrabber * pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchSoftwareTriggerMode_STC_LBGP251BCXP124"), MSG_LEVEL_HIGH);

		//設定相機軟體觸發狀態
		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = ExposureTime + 1000;
		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "RG");
		pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");
		pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);
		//	pGrabber->setFloat<Euresys::DeviceModule>("ExposureTime" , 4600);
		pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "StartCycle");
		pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "Software");//20240514
																				//pGrabber->setString<Euresys::RemoteModule>("TriggerActivation" , "FallingEdge");//RisingEdge, FallingEdge-不支援//20240514
																				//pGrabber->setString<Euresys::RemoteModule>("ExposureMode" , "TriggerWidth");//20240514
		pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "Timed");//20240514
		pGrabber->setInteger<Euresys::RemoteModule>("ExposureTime", ExposureTime);

	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber)
{
	bool bIsOK = true;
	switch ( CameraDevice )
	{
	case COAXPRESS_CAMERA_VC_12MX_M180:		bIsOK=SwitchExternalTriggerMode_VC_12MX_M180_8DA1(pGrabber); break;
	case COAXPRESS_CAMERA_VCC_25CXPHSM:		bIsOK=SwitchExternalTriggerMode_VCC_25CXPHSM(pGrabber); break;	
	case COAXPRESS_CAMERA_VC_25MX2_M150I:	bIsOK=SwitchExternalTriggerMode_VC_25MX2_M150I_8DA1(pGrabber); break;
	case COAXPRESS_CAMERA_STC_LBGP251BCXP124:	bIsOK = SwitchExternalTriggerMode_STC_LBGP251BCXP124(pGrabber); break;
	default:								bIsOK=ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	if ( true == bIsOK )
	{	SetCameraGrabMode(CAMERA_GRAB_EXTERNAL_TRIGGER); }
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_VC_12MX_M180_8DA1(CEGrabber *pGrabber)
{
	if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
	try
	{	
		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode_VC_12MX_M180_8DA1"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime   = this->GetPeriodTime();//ExposureTime+1000;

		//設定相機外埠處發狀態
		pGrabber->setString<Euresys::InterfaceModule>("LineSelector" , "IIN11");//IIN11, IIN12
		//by Ken Error
		pGrabber->setString<Euresys::InterfaceModule>("LineInverter" , "True");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSelector" , "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSource" , "IIN11");//IIN11, IIN12
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolActivation", "RisingEdge");//FallingEdge, RisingEdge

		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSelector" , "DEL2");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource1" , "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource2" , "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolClockSource" , "TIME1US");
		pGrabber->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue" , 500);

		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod","EXTERNAL");//RG,
		if( m_FVersion < COAXLINK_FIREWARE_VERSION_217 )
		{	//舊版才有支援以下參數 
			pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap" , "true");//disable@coaxlink_7.1.1.25 for external trigger
			pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime" , 1000);//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);//disable@coaxlink_7.1.1.25 for external trigger 
		}

		pGrabber->setString<Euresys::RemoteModule>("TriggerMode" , "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource" , "CXPin");
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation" , "RisingEdge");//FallingEdge, RisingEdge
		pGrabber->setString<Euresys::RemoteModule>("ExposureMode" , "TriggerWidth");
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_VCC_25CXPHSM(CEGrabber *pGrabber)
{
	bool IsOK = true;
#ifndef LIGHT_CTRL_DISABLE		
	if (CheckGrabberPtr(pGrabber) == false) { return false; }	
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();
	switch (LightCtrlBoardType)
	{
	case LIGHT_CTRL_BOARD_3DA6:
		IsOK = SwitchExternalTriggerMode_VCC_25CXPHSM_A5V1(pGrabber);
		break;
	case LIGHT_CTRL_BOARD_8DA1:
	case LIGHT_CTRL_BOARD_ARDUINO:
		IsOK = SwitchExternalTriggerMode_VCC_25CXPHSM_8DA1(pGrabber);
		break;
	default:
		IsOK = SwitchExternalTriggerMode_VCC_25CXPHSM_DLP(pGrabber);
		break;
	}
#else
	IsOK = SwitchExternalTriggerMode_VCC_25CXPHSM_DLP(pGrabber);
#endif//LIGHT_CTRL_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_VCC_25CXPHSM_DLP(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode_VCC_25CXPHSM_DLP"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = ExposureTime + 1000;

		//設定相機外埠處發狀態
		pGrabber->setString<Euresys::InterfaceModule>("LineSelector", "IIN11");//IIN11, IIN12
																			   //by Ken Error
		pGrabber->setString<Euresys::InterfaceModule>("LineInverter", "False");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSelector", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSource", "IIN11");//IIN11, IIN12
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolActivation", "FallingEdge");//FallingEdge, RisingEdge

		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSelector", "DEL2");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource1", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource2", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolClockSource", "TIME1US");
		pGrabber->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue", 500);

		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "RG");//RG, EXTERNAL
		pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");
		pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);
		//pGrabber->setFloat<Euresys::DeviceModule>("ExposureTime" , 4600);
		pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");
		pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);

		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation", "FallingEdge");//FallingEdge, RisingEdge, LevelHigh, LevelLow
		//pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_VCC_25CXPHSM_A5V1(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode_VCC_25CXPHSM_A5V1"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = this->GetPeriodTime();//ExposureTime+1000;

													 //設定相機外埠處發狀態
		pGrabber->setString<Euresys::InterfaceModule>("LineSelector", "IIN11");//IIN11, IIN12
																			   //by Ken Error
		pGrabber->setString<Euresys::InterfaceModule>("LineInverter", "True");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSelector", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSource", "IIN11");//IIN11, IIN12
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolActivation", "RisingEdge");//FallingEdge, RisingEdge

		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSelector", "DEL2");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource1", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource2", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolClockSource", "TIME1US");
		pGrabber->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue", 500);

		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "EXTERNAL");//RG,
		if (m_FVersion < COAXLINK_FIREWARE_VERSION_217)
		{	//舊版才有支援以下參數 
			pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");//disable@coaxlink_7.1.1.25 for external trigger
			pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);//disable@coaxlink_7.1.1.25 for external trigger 
		}

		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation", "LevelHigh");//FallingEdge, RisingEdge, LevelHigh, LevelLow
		//pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_VCC_25CXPHSM_8DA1(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode_VCC_25CXPHSM_8DA1"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = this->GetPeriodTime();//ExposureTime+1000;

													 //設定相機外埠處發狀態
		pGrabber->setString<Euresys::InterfaceModule>("LineSelector", "IIN11");//IIN11, IIN12
																			   //by Ken Error
		pGrabber->setString<Euresys::InterfaceModule>("LineInverter", "True");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSelector", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSource", "IIN11");//IIN11, IIN12
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolActivation", "RisingEdge");//FallingEdge, RisingEdge

		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSelector", "DEL2");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource1", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource2", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolClockSource", "TIME1US");
		pGrabber->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue", 500);

		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "EXTERNAL");//RG,
		if (m_FVersion < COAXLINK_FIREWARE_VERSION_217)
		{	//舊版才有支援以下參數 
			pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");//disable@coaxlink_7.1.1.25 for external trigger
			pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);//disable@coaxlink_7.1.1.25 for external trigger 
		}

		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation", "LevelHigh");//FallingEdge, RisingEdge, LevelHigh, LevelLow
		//pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_VC_25MX2_M150I_8DA1(CEGrabber *pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode_VC_25MX2_M150I_8DA1"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = this->GetPeriodTime();

		//設定相機外埠處發狀態
		pGrabber->setString<Euresys::InterfaceModule>("LineSelector", "IIN11");//IIN11, IIN12
																			   //by Ken Error
		pGrabber->setString<Euresys::InterfaceModule>("LineInverter", "True");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSelector", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSource", "IIN11");//IIN11, IIN12
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolActivation", "RisingEdge");//FallingEdge, RisingEdge

		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSelector", "DEL2");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource1", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource2", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolClockSource", "TIME1US");
		pGrabber->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue", 500);

		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "EXTERNAL");//RG, EXTERNAL
		if (m_FVersion < COAXLINK_FIREWARE_VERSION_217)
		{	//舊版才有支援以下參數 
			pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");
			pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);
			pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");
			pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);
		}

		//pGrabber->setString<Euresys::RemoteModule>("TriggerSelector", "ExposureStart");
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//Line0, Line2, Counter0, Anyway, LinkTrigger0//需要確認
		pGrabber->setString<Euresys::RemoteModule>("TriggerActivation", "RisingEdge");//FallingEdge, RisingEdge, LevelHigh, LevelLow
		//pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_STC_LBGP251BCXP124(CEGrabber * pGrabber)
{
	bool IsOK = true;
#ifndef LIGHT_CTRL_DISABLE		
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = LightCtrlBoard.GetLightCtrlBoardType();
	switch (LightCtrlBoardType)
	{
	case LIGHT_CTRL_BOARD_3DA6:
		IsOK = SwitchExternalTriggerMode_STC_LBGP251BCXP124_A5V1(pGrabber);
		break;
	case LIGHT_CTRL_BOARD_8DA1:
	case LIGHT_CTRL_BOARD_ARDUINO:
		IsOK = SwitchExternalTriggerMode_STC_LBGP251BCXP124_8DA1(pGrabber);
		break;
	default:
		IsOK = SwitchExternalTriggerMode_STC_LBGP251BCXP124_DLP(pGrabber);
		break;
	}
#else
	IsOK = SwitchExternalTriggerMode_STC_LBGP251BCXP124_DLP(pGrabber);
#endif//LIGHT_CTRL_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_STC_LBGP251BCXP124_DLP(CEGrabber * pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode_STC_LBGP251BCXP124_DLP"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = ExposureTime + 1000;

		//設定相機外埠處發狀態
		pGrabber->setString<Euresys::InterfaceModule>("LineSelector", "IIN11");//IIN11, IIN12
																			   //by Ken Error
		pGrabber->setString<Euresys::InterfaceModule>("LineInverter", "False");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSelector", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSource", "IIN11");//IIN11, IIN12
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolActivation", "FallingEdge");//FallingEdge, RisingEdge

		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSelector", "DEL2");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource1", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource2", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolClockSource", "TIME1US");
		pGrabber->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue", 500);

		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "RG");//RG, EXTERNAL
		pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");
		pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);
		//pGrabber->setFloat<Euresys::DeviceModule>("ExposureTime" , 4600);
		pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");
		pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);

		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//20240514
																					//pGrabber->setString<Euresys::RemoteModule>("TriggerActivation" , "FallingEdge");//FallingEdge, RisingEdge-不支援//20240514
		pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");

	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_STC_LBGP251BCXP124_A5V1(CEGrabber * pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode_STC_CMB120ACXP_A5V1"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = this->GetPeriodTime();//ExposureTime+1000;

													 //設定相機外埠處發狀態
		pGrabber->setString<Euresys::InterfaceModule>("LineSelector", "IIN11");//IIN11, IIN12
																			   //by Ken Error
		pGrabber->setString<Euresys::InterfaceModule>("LineInverter", "True");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSelector", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSource", "IIN11");//IIN11, IIN12
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolActivation", "RisingEdge");//FallingEdge, RisingEdge

		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSelector", "DEL2");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource1", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource2", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolClockSource", "TIME1US");
		pGrabber->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue", 500);

		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "EXTERNAL");//RG,
		if (m_FVersion < COAXLINK_FIREWARE_VERSION_217)
		{	//舊版才有支援以下參數 
			pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");//disable@coaxlink_7.1.1.25 for external trigger
			pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);//disable@coaxlink_7.1.1.25 for external trigger 
		}

		pGrabber->setString<Euresys::RemoteModule>("TriggerSelector", "FrameStart");
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");
		//pGrabber->setString<Euresys::RemoteModule>("TriggerActivation" , "RisingEdge");//FallingEdge, RisingEdge-不支援//20240514
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_CoaxLinkQuadCXP12_Camera::SwitchExternalTriggerMode_STC_LBGP251BCXP124_8DA1(CEGrabber * pGrabber)
{
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("SwitchExternalTriggerMode_STC_LBGP251BCXP124_8DA1"), MSG_LEVEL_HIGH);

		const int ExposureTime = this->GetExposureTime();
		const int PeriodTime = this->GetPeriodTime();//ExposureTime+1000;

													 //設定相機外埠處發狀態
		pGrabber->setString<Euresys::InterfaceModule>("LineSelector", "IIN11");//IIN11, IIN12
																			   //by Ken Error
		pGrabber->setString<Euresys::InterfaceModule>("LineInverter", "True");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSelector", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolSource", "IIN11");//IIN11, IIN12
		pGrabber->setString<Euresys::InterfaceModule>("LineInputToolActivation", "RisingEdge");//FallingEdge, RisingEdge

		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSelector", "DEL2");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource1", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolSource2", "LIN1");
		pGrabber->setString<Euresys::InterfaceModule>("DelayToolClockSource", "TIME1US");
		pGrabber->setInteger<Euresys::InterfaceModule>("DelayToolDelayValue", 500);

		pGrabber->setString<Euresys::DeviceModule>("CameraControlMethod", "EXTERNAL");//RG,
		if (m_FVersion < COAXLINK_FIREWARE_VERSION_217)
		{	//舊版才有支援以下參數 
			pGrabber->setString<Euresys::DeviceModule>("ExposureReadoutOverlap", "true");//disable@coaxlink_7.1.1.25 for external trigger
			pGrabber->setInteger<Euresys::DeviceModule>("ExposureRecoveryTime", 1000);//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setString<Euresys::DeviceModule>("CycleTriggerSource", "DEL2_2");//disable@coaxlink_7.1.1.25 for external trigger 
			pGrabber->setInteger<Euresys::DeviceModule>("CycleTargetPeriod", PeriodTime);//disable@coaxlink_7.1.1.25 for external trigger 
		}

		pGrabber->setString<Euresys::RemoteModule>("TriggerSelector", "FrameStart");
		pGrabber->setString<Euresys::RemoteModule>("TriggerMode", "On");
		pGrabber->setString<Euresys::RemoteModule>("ExposureMode", "TriggerWidth");
		pGrabber->setString<Euresys::RemoteModule>("TriggerSource", "LinkTrigger0");//20240514
																					//pGrabber->setString<Euresys::RemoteModule>("TriggerActivation" , "RisingEdge");//FallingEdge, RisingEdge-不支援//20240514

	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera (%s)"), str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
double CCamera_CoaxLinkQuadCXP12_Camera::ReadCameraFrameRate(COAXPRESS_CAMERA_DEVICE CameraDevice, CEGrabber *pGrabber)
{
	double FrameRate=0.0;
	const int ExpTime=GetExposureTime();
	const bool bCanSetExpTime=CheckCanSetExposureTime();
	if ( CheckGrabberPtr(pGrabber) == false )
	{	return FrameRate; }
	if ( true == bCanSetExpTime ) //要先降低曝光時間, 否則會得到錯的FPS
	{	pGrabber->setInteger<Euresys::RemoteModule>("ExposureTime" , 100);	}
	switch ( CameraDevice )
	{
	case COAXPRESS_CAMERA_VC_12MX_M180:		FrameRate=ReadCameraFrameRate_VC_12MX_M180(pGrabber); break;
	case COAXPRESS_CAMERA_VCC_25CXPHSM:		FrameRate=ReadCameraFrameRate_VCC_25CXPHSM(pGrabber); break;
	case COAXPRESS_CAMERA_VC_25MX2_M150I:	FrameRate=ReadCameraFrameRate_VC_25MX2_M150I(pGrabber); break;
	case COAXPRESS_CAMERA_STC_LBGP251BCXP124:	FrameRate = ReadCameraFrameRate_STC_LBGP251BCXP124(pGrabber); break;
	default:								ReturnErrorNoCameraDeviceType(CameraDevice);	break;
	}
	if ( true == bCanSetExpTime ) 
	{	pGrabber->setInteger<Euresys::RemoteModule>("ExposureTime" , ExpTime);	}
	return FrameRate;
}
//-------------------------------------------------------------------------------------//
double CCamera_CoaxLinkQuadCXP12_Camera::ReadCameraFrameRate_VC_12MX_M180(CEGrabber *pGrabber)
{
	double FrameRate=CCamera_Basic::m_CameraFPS;
	if ( CheckGrabberPtr(pGrabber) == false ) { return false; }
	try
	{	
		CCamera_Basic::SaveCameraProcess(_T("ReadCameraFrameRate_VC_12MX_M180"), MSG_LEVEL_HIGH);		
		FrameRate = pGrabber->getFloat<Euresys::RemoteModule>("AcquisitionFrameRate"); //設定相機感測元件異常需要處理(可能回臨近均值)		
	}
	catch(std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s") , str);
		return FrameRate;
	}
	return FrameRate;
}
//-------------------------------------------------------------------------------------//
double CCamera_CoaxLinkQuadCXP12_Camera::ReadCameraFrameRate_VCC_25CXPHSM(CEGrabber *pGrabber)
{
	double FrameRate = CCamera_Basic::m_CameraFPS;
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{		
		CCamera_Basic::SaveCameraProcess(_T("ReadCameraFrameRate_VCC_25CXPHSM"), MSG_LEVEL_HIGH);
		FrameRate = 150.0;
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return FrameRate;
	}
	return FrameRate;
}
//-------------------------------------------------------------------------------------//
double CCamera_CoaxLinkQuadCXP12_Camera::ReadCameraFrameRate_VC_25MX2_M150I(CEGrabber *pGrabber)
{
	double FrameRate = CCamera_Basic::m_CameraFPS;
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("ReadCameraFrameRate_VC_25MX2_M150I"), MSG_LEVEL_HIGH);
		FrameRate = pGrabber->getFloat<Euresys::RemoteModule>("AcquisitionFrameRate"); //取回相機取像速度-未支援, 以相機規格為主
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return FrameRate;
	}
	return FrameRate;
}
//-------------------------------------------------------------------------------------//
double CCamera_CoaxLinkQuadCXP12_Camera::ReadCameraFrameRate_STC_LBGP251BCXP124(CEGrabber * pGrabber)
{
	double FrameRate = CCamera_Basic::m_CameraFPS;
	if (CheckGrabberPtr(pGrabber) == false) { return false; }
	try
	{
		CCamera_Basic::SaveCameraProcess(_T("ReadCameraFrameRate_STC_LBGP251BCXP124"), MSG_LEVEL_HIGH);
		FrameRate = pGrabber->getFloat<Euresys::RemoteModule>("AcquisitionFrameRate"); //設定相機感測元件異常需要處理(可能回臨近均值)		
	}
	catch (std::exception &e)
	{
		CString str = e.what();
		m_ErrorString.Format(_T("Error, CCamera_CoaxLinkQuadCXP12_Camera %s"), str);
		return FrameRate;
	}
	return FrameRate;
}
//-------------------------------------------------------------------------------------//
#endif//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
#endif//COAXLINK_QUAD_CXP12_CAMERA_USE