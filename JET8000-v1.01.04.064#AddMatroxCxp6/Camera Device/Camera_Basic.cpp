// CameraObj.cpp: implementation of the CCamera_Basic class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Camera_Basic.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//	
IMPLEMENT_DYNAMIC(CCamera_Basic, CObject)
//-------------------------------------------------------------------------------------//
CCamera_Basic::CCamera_Basic()
{	
	m_CameraRingBufferList = NULL;	
	m_CameraRingBufferStateList = NULL;
	m_CameraImageBufferCount = CAMERA_IMAGE_BUFFER_COUNT_3D;
	::InitializeCriticalSection(&m_csCamera);
	PreInitCamera();
}
//-------------------------------------------------------------------------------------//
CCamera_Basic::~CCamera_Basic()
{
	ReleaseCameraRingBuffer();
	DeleteCameraGrabFinishEvent();
	DeleteCameraExposureFinishEvent();
	::DeleteCriticalSection(&m_csCamera);	
	m_CameraConstructed = false;
}
//-------------------------------------------------------------------------------------//
void CCamera_Basic::PreInitCamera()
{
	m_ErrorString = _T("");		
	m_CameraID = CAMERA_ID_1;
	m_CameraInited = false;
	m_CameraConstructed = false;
	m_CameraCallbackTimming = CAMERA_CALLBACK_EACH_FRAME;
	m_CameraLogIndex = -1;

	m_PeriodTim_us = 1000000;
	m_ExposureTime_us = 1000;//
	m_ExposureTimeMin_us = 20;//曝光時間-Min
	m_ExposureTimeMax_us = 1000000;//曝光時間-Max
	m_TriggerDelay = 0;

	m_CameraFPS = 1.0;
	m_CameraBitCount = 8;//8Bit
	m_CameraImageStep = 1024;//相機間距
	m_CameraImageW = 1024;//相機影像寬度
	m_CameraImageH = 1024;//相機影像高度		
	m_CameraSizeRaw = m_CameraImageW*m_CameraImageH;
	m_CameraSizeColor = m_CameraSizeRaw*3;

	m_CameraWParam = 0;//影像回傳參數
	m_CameraLParam = 0;//影像回傳參數
	m_CameraToSendCallback = TRUE;//影像是否傳送callback

	m_pColorInfo = NULL;
	m_pMonoInfo  = NULL;
	m_CameraModelName = _T("Camera");

	m_CameraImagePtr   = NULL;
	m_ClonedRawImagePtr     = NULL;
	m_ClonedColorImagePtr = NULL;	
	
	m_CameraCallBackWnd = NULL;			
	m_CameraGrabMode = CAMERA_GRAB_UNDEFINED;//取像模式-外部觸發或者是內部直接取像	
	m_CameraImageMode = CAMERA_IMAGE_GRAY;
	m_CameraBayerType = BAYER_PATTERN_NONE;		
	m_CameraShutterMode = CAMERA_SHUTTER_GLOBAL;
	m_CameraExposureMode = CAMERA_EXPOSURE_TRIGGER_WIDTH;

	m_CameraGrabFinishEvent = NULL;
	m_CameraExposureFinishEvent = NULL;	
	m_CameraGrabbing = false;
	m_CameraApplyGamma = true;

	m_SaveCameraAddRingBufferLog = false;

	m_CountForFramesToGrab = -1;
	m_CountForCameraCallback = 0;//相機回傳-Step01
	m_CountForCameraExposuredEnd = 0;//曝光結束回傳-Step02
	m_CountForImageCallback = 0;//影像回傳-Step03
	m_CountForBufferCopyToHost = 0;//影像複製-Step03
	m_CountForGrabLoop = 1;

	InitialGammaLUT();
	CreateCameraRingBuffer();

	QueryPerformanceFrequency(&m_CameraFrequnce);
	QueryPerformanceCounter(&m_CameraFnStartTime);
	QueryPerformanceCounter(&m_CameraFnEndTime);
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CheckCameraInited()
{
	if ( this->m_CameraInited == FALSE )
	{
		this->m_ErrorString.Format(_T("%s"), _T("Error, Camera Not Initialization"));
		return false;
	}	
	return true;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraInited(bool val)
{
	m_CameraInited = val;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::RegisterCameraCurrentProcessFile()
{
#ifndef CAMERA_OBJ_DISABLE
	const bool bSaveHeader = true;
	CString LogFolder, LogFilename, LogExtName, LogBackup;	
	LogFolder = AOIDataCollect.GetAOILogDirectory();
	LogFilename = _T("CameraLog");//多相機時要修改
	LogExtName = _T("TXT");
	LogBackup = _T("");
	m_CameraLogIndex = LogManager.AddLogFile(LogFolder, LogFilename, LogExtName, LogBackup, CLogNode::LOG_FILENAME_BY_DATE, bSaveHeader);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::SaveCameraCurrentProcess(const char *String)
{
	//if ( this->m_MotionParameter.m_SaveCurrentMotionProcess == FN_DISABLE ) { return true; }
	if ( LogManager.AddLogMessage(m_CameraLogIndex, String) == false )
	{	return false; }
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::SaveCameraCurrentProcess(const wchar_t *String)
{
	//if ( this->m_MotionParameter.m_SaveCurrentMotionProcess == FN_DISABLE ) { return true; }
	if ( LogManager.AddLogMessage(m_CameraLogIndex, String) == false )
	{	return false; }
	return true;
}
//------------------------------------------------------------------------------//
IMAGE_SIZE CCamera_Basic::GetBitCount() const//取得影像灰階深度
{
	return this->m_CameraBitCount;
}
//------------------------------------------------------------------------------//
IMAGE_SIZE CCamera_Basic::GetImageStep()  const//取得整張影像每條記憶體寬度像素
{
	return this->m_CameraImageStep;
}
//------------------------------------------------------------------------------//
IMAGE_SIZE CCamera_Basic::GetCameraImageW()  const//取得整張影像寬度像素
{
	return m_CameraImageW;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CCamera_Basic::GetCameraImageH()  const//取得整張影像長度像素
{
	return m_CameraImageH;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CCamera_Basic::GetImageRawSize()  const//取得整張原始影像像素
{
	return m_CameraSizeRaw;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CCamera_Basic::GetImageColorSize()  const//取得整張彩色影像像素
{
	return m_CameraSizeColor;
}
//-------------------------------------------------------------------------------------//
double CCamera_Basic::GetCameraFPS()  const//取得相機的FPS
{
	return m_CameraFPS;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Basic::CalcCameraImageSize()//計算相機影像尺寸
{
	IMAGE_SIZE BitCount=GetBitCount();
	IMAGE_SIZE ImageW=GetCameraImageW();
	IMAGE_SIZE ImageStep=JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	this->m_CameraImageStep = ImageStep;
	this->m_CameraSizeRaw   = m_CameraImageW*m_CameraImageH;
	this->m_CameraSizeColor = this->m_CameraSizeRaw*3;
	return true;
}
//-------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraWParam(WPARAM wParam)//設定相機回傳參數-W
{
	this->m_CameraWParam = wParam;
}
//-------------------------------------------------------------------------------------//
WPARAM CCamera_Basic::GetCameraWParam() const//取得相機回傳參數-W
{
	return this->m_CameraWParam;
}
//-------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraLParam(LPARAM lParam)//設定相機回傳參數-L
{
	this->m_CameraLParam = lParam;
}
//-------------------------------------------------------------------------------------//
LPARAM CCamera_Basic::GetCameraLParam() const//取得相機回傳參數-L
{
	return this->m_CameraLParam;
}
//-------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraToSendCallback(BOOL val)//設定影像是否傳送callback
{
	this->m_CameraToSendCallback = val;
}
//-------------------------------------------------------------------------------------//
BOOL CCamera_Basic::GetCameraToSendCallback() const//取得影像是否傳送callback
{
	return m_CameraToSendCallback;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Basic::SetCameraCallbackTimming(CAMERA_CALLBACK_TIMMING val)//設定相機回傳時機
{
	this->m_CameraCallbackTimming = val;
	return true;
}
//-------------------------------------------------------------------------------------//
CAMERA_CALLBACK_TIMMING CCamera_Basic::GetCameraCallbackTimming() const//取得相機回傳時機
{
	return m_CameraCallbackTimming;
}
//-------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraErrorString(LPCSTR lpszText)
{
	this->m_ErrorString = lpszText;
}
//-------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraErrorString(LPCWSTR lpszText)
{
	this->m_ErrorString = lpszText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CCamera_Basic::GetCameraErrorString()
{		
	CString Key=GetCameraErrorKeyName();
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Camera_Others(m_ErrorStringOut);
	return this->m_ErrorStringOut;
}
//------------------------------------------------------------------------------//	
BITMAPINFO* CCamera_Basic::GetColorBitmapInfo()
{
	return this->m_pColorInfo;
}
//------------------------------------------------------------------------------//	
BITMAPINFO* CCamera_Basic::GetMonoBitmapInfo()
{
	return this->m_pMonoInfo;
}
//------------------------------------------------------------------------------//	
CAMERA_ID CCamera_Basic::GetCameraID() const//取得相機編號	
{
	return this->m_CameraID;
}
//------------------------------------------------------------------------------//	
void CCamera_Basic::SetCameraID(CAMERA_ID value)//設定相機編號
{
	this->m_CameraID = value;
}
//------------------------------------------------------------------------------//
HWND CCamera_Basic::GetCameraCallBackHWnd()
{
	return this->m_CameraCallBackWnd;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraCallBackHWnd(const HWND hWnd)
{
	this->m_CameraCallBackWnd = hWnd;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::ResetCameraRingBuffer()//覆歸相機環型記憶體列表
{
#ifndef OFFLINE_VERSION
	if ( this->CheckCameraRingBuffer() == false )
	{	return false; }

	size_t i=0;			
	for ( i=0; i<m_CameraRingBufferListSize; i++ )
	{	
		this->m_CameraRingBufferList[i] = NULL;		
		m_CameraRingBufferStateList[i] = CAMERA_RING_BUFFER_STATES_NONE;
	}

	this->m_CameraRingBufferIndex = 0;
	this->m_CameraKeepBufferPtr = NULL;
	this->m_CameraKeepBufferIndex = -1;
#endif//OFFLINE_VERSION
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CheckCameraRingBuffer()//確認相機影像記憶體位址
{
	if ( NULL == this->m_CameraRingBufferList || NULL==m_CameraRingBufferStateList ) 
	{ 
		this->m_ErrorString.Format(_T("Error, Camera m_CameraRingBufferList is NULL")); 
		return false; 
	}
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CreateCameraRingBuffer()//建立相機影像記憶體位址
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	const size_t CameraImageBufferCount = GetCameraImageBufferCount();	
	this->ReleaseCameraRingBuffer();	
	this->m_CameraRingBufferList = new IMAGE_PTR[CameraImageBufferCount];//記錄相機影像的記憶體位址
	m_CameraRingBufferStateList = new CAMERA_RING_BUFFER_STATES[CameraImageBufferCount];
	if ( this->CheckCameraRingBuffer() == false )
	{	return false; }
	this->m_CameraRingBufferListSize = CameraImageBufferCount;//影像指標Buffer最大可以儲存的數量
	for ( i=0; i<m_CameraRingBufferListSize; i++ )
	{	
		this->m_CameraRingBufferList[i] = NULL;	
		m_CameraRingBufferStateList[i] = CAMERA_RING_BUFFER_STATES_NONE;
	}
#endif//OFFLINE_VERSION
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::ReleaseCameraRingBuffer()//釋放相機影像記憶體位址
{
#ifndef OFFLINE_VERSION	
	size_t i=0;
	if ( this->CheckCameraRingBuffer() == false )	
	{
		this->m_CameraRingBufferIndex = 0;
		this->m_CameraRingBufferListSize = 0;
		return true; 
	}
	delete[] this->m_CameraRingBufferList;
	delete[] m_CameraRingBufferStateList;
	this->m_CameraRingBufferList = NULL;
	m_CameraRingBufferStateList = NULL;
	this->m_CameraRingBufferIndex = 0;
	this->m_CameraRingBufferListSize = 0;
#endif//OFFLINE_VERSION
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::KeepCameraTempRingBuffer()//暫存相機環指標
{
#ifndef OFFLINE_VERSION		
	if ( this->CheckCameraRingBuffer() == false )
	{	return false; }
	const long index = this->m_CameraRingBufferIndex%m_CameraRingBufferListSize;
	this->m_CameraKeepBufferIndex = index;
	this->m_CameraKeepBufferPtr = this->m_CameraRingBufferList[index];	
	m_CameraRingBufferStateList[index] = CAMERA_RING_BUFFER_STATES_DONE;
#endif//OFFLINE_VERSION
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::FreeCameraTempRingBuffer()//歸還相機環指標現今
{
#ifndef OFFLINE_VERSION		
	if ( this->CheckCameraRingBuffer() == false )
	{	return false; }
	if ( this->m_CameraKeepBufferIndex >= m_CameraRingBufferListSize )
	{	return false; }
	if ( m_CameraRingBufferList[m_CameraKeepBufferIndex] == m_CameraKeepBufferPtr )
	{
		m_CameraRingBufferList[m_CameraKeepBufferIndex] = NULL;
		m_CameraRingBufferStateList[m_CameraKeepBufferIndex] = CAMERA_RING_BUFFER_STATES_NONE;
		m_CameraKeepBufferIndex = -1;		
		return true;
	}

	size_t i=0;
	for ( i=0; i<m_CameraRingBufferListSize; i++ )
	{
		if ( m_CameraRingBufferList[i] == NULL ) { continue; }
		if ( m_CameraRingBufferList[i] == m_CameraKeepBufferPtr ) 
		{ 
			m_CameraRingBufferList[i] = NULL;
			m_CameraRingBufferStateList[i] = CAMERA_RING_BUFFER_STATES_NONE;
			m_CameraKeepBufferIndex = -1;
			break;
		}
	}	
#endif//OFFLINE_VERSION
	return true;
}
//------------------------------------------------------------------------------//
BAYER_PATTERN_MODE CCamera_Basic::GetCameraBayerPattern(int StartX, int StartY)//根據起點的X, Y來知道解碼的方式
{
	BAYER_PATTERN_MODE BayerPatternORG = GetCameraBayerPattern();
	BAYER_PATTERN_MODE BayerPatternNew = GetCameraBayerPattern();
	int RandX = StartX%2;
	int RandY = StartY%2;	
	
	switch ( BayerPatternORG )
	{
	case BAYER_PATTERN_RGGB:
		//寬度為偶數，且高度為偶數(RGGB), 寬度為奇數，且高度為偶數(GRBG)
		//寬度為偶數，且高度為奇數(GBRG), 寬度為奇數，且高度為偶數(BGGR)
		if ( RandX==0 )
		{
			if ( RandY == 0 )
			{	BayerPatternNew = BAYER_PATTERN_RGGB;	}
			else
			{	BayerPatternNew = BAYER_PATTERN_GBRG;	}
		}
		else
		{
			if ( RandY == 0 )
			{	BayerPatternNew = BAYER_PATTERN_GRBG;	}
			else
			{	BayerPatternNew = BAYER_PATTERN_BGGR;	}
		}
		break;
	case BAYER_PATTERN_GRBG:
		if ( RandX==0 )
		{
			if ( RandY == 0 )
			{	BayerPatternNew = BAYER_PATTERN_GRBG;	}
			else
			{	BayerPatternNew = BAYER_PATTERN_BGGR;	}
		}
		else
		{
			if ( RandY == 0 )
			{	BayerPatternNew = BAYER_PATTERN_RGGB;	}
			else
			{	BayerPatternNew = BAYER_PATTERN_GBRG;	}
		}
		break;
	case BAYER_PATTERN_GBRG:
		if ( RandX==0 )
		{
			if ( RandY == 0 )
			{	BayerPatternNew = BAYER_PATTERN_GBRG;	}
			else
			{	BayerPatternNew = BAYER_PATTERN_RGGB;	}
		}
		else
		{
			if ( RandY == 0 )
			{	BayerPatternNew = BAYER_PATTERN_BGGR;	}
			else
			{	BayerPatternNew = BAYER_PATTERN_GRBG;	}
		}
		break;
	case BAYER_PATTERN_BGGR:
		if ( RandX==0 )
		{
			if ( RandY == 0 )
			{	BayerPatternNew = BAYER_PATTERN_BGGR;	}
			else
			{	BayerPatternNew = BAYER_PATTERN_GRBG;	}
		}
		else
		{
			if ( RandY == 0 )
			{	BayerPatternNew = BAYER_PATTERN_GBRG;	}
			else
			{	BayerPatternNew = BAYER_PATTERN_RGGB;	}
		}
		break;
	}
	return BayerPatternNew;
}
//------------------------------------------------------------------------------//
CAMERA_SHUTTER_MODE CCamera_Basic::GetCameraShutterMode() const//相機快門模式
{
	return m_CameraShutterMode;
}
//------------------------------------------------------------------------------//
CAMERA_EXPOSURE_MODE CCamera_Basic::GetCameraExposureMode() const//相機曝光模式
{	
	return m_CameraExposureMode;
}	
//------------------------------------------------------------------------------//
unsigned int CCamera_Basic::GetCameraImageBufferCount() const//相機影像緩存數量
{
	return m_CameraImageBufferCount;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::GetSaveCameraAddRingBufferLog() const//取得儲存增加相機影像列表資訊
{
	return m_SaveCameraAddRingBufferLog;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetSaveCameraAddRingBufferLog(bool val)//設定儲存增加相機影像列表資訊
{
	m_SaveCameraAddRingBufferLog = val;
}
//------------------------------------------------------------------------------//
CAMERA_IMAGE_MODE CCamera_Basic::GetCameraImageMode() const
{
	return m_CameraImageMode;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraImageMode(CAMERA_IMAGE_MODE val)
{
	m_CameraImageMode = val;
}
//------------------------------------------------------------------------------//
BAYER_PATTERN_MODE CCamera_Basic::GetCameraBayerPattern() const
{
	return m_CameraBayerType;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraBayerPattern(BAYER_PATTERN_MODE val)
{
	m_CameraBayerType = val;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::LockCamera()
{
	::EnterCriticalSection(&m_csCamera);
}
//------------------------------------------------------------------------------//
void CCamera_Basic::UnlockCamera()
{
	::LeaveCriticalSection(&m_csCamera);
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraConstructed(bool val)
{
	m_CameraConstructed = val;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::GetCameraConstructed() const
{
	return m_CameraConstructed;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraModelID(int Model)
{
	m_CameraModelID = Model;
}
//------------------------------------------------------------------------------//
int CCamera_Basic::GetCameraModelID() const
{
	return m_CameraModelID;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraModelName(LPCTSTR  Model)
{
	m_CameraModelName = Model;
}
//------------------------------------------------------------------------------//
LPCTSTR CCamera_Basic::GetCameraModelName() const
{
	return m_CameraModelName;
}
//------------------------------------------------------------------------------//
CString CCamera_Basic::GetCameraErrorKeyName() const
{
	CString Key=_T("Camera");	
	return Key;
}
//------------------------------------------------------------------------------//
int CCamera_Basic::GetPeriodTime() const//取得最少周期時間
{
	//return this->m_PeriodTim_us;
	int PeriodTim_us = m_PeriodTim_us;
	const double CameraFPS = CCamera_Basic::GetCameraFPS()-1;
	const int    CameraMinPeriod = (int)(1000000/CameraFPS)+1;
	if ( PeriodTim_us < CameraMinPeriod ) 
	{	 PeriodTim_us = CameraMinPeriod; }
	return PeriodTim_us;
}
//------------------------------------------------------------------------------//
int CCamera_Basic::GetExposureTime() const
{
	return this->m_ExposureTime_us;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::FreeCameraImagePtr(void *ImagePtr)//釋放相機內的影像
{
	if ( this->CheckCameraInited() == false ) { return false; }
#ifndef CAMERA_OBJ_DISABLE	
	if ( this->CheckCameraRingBuffer() == false )
	{	return false; }

	CCamera_Basic::SaveCameraProcess(_T("FreeCameraImagePtr"), MSG_LEVEL_HIGH);
	size_t i = 0;	
	for ( i=0; i<m_CameraRingBufferListSize; i++ )
	{
		if ( m_CameraRingBufferList[i] == ImagePtr )
		{	
			m_CameraRingBufferList[i] = NULL; 
			m_CameraRingBufferStateList[i] = CAMERA_RING_BUFFER_STATES_NONE;
			break;
		}
	}
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//------------------------------------------------------------------------------//
double CCamera_Basic::ReadCameraTemperature()//讀取相機溫度
{
	return 0;
}
//------------------------------------------------------------------------------------------//
bool CCamera_Basic::SetCameraApplyGamma(bool Apply)
{	
	this->m_CameraApplyGamma = Apply;
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::SendCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//送訊息給回傳視窗 
{
	if ( GetCameraToSendCallback()==FALSE ) { return true; }
	return AOIDataCollect.SendCallbackWndMessage(message, wParam, lParam);
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::PostCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam)//送訊息給回傳視窗
{
	if ( GetCameraToSendCallback()==FALSE ) { return true; }
	return AOIDataCollect.PostCallbackWndMessage(message, wParam, lParam);	
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::AddCameraRingBufferList(void *ImagePtr)
{	
	if ( this->CheckCameraRingBuffer() == false )
	{	return false; }

	if ( NULL == ImagePtr ) 
	{
		this->m_ErrorString.Format(_T("Error, NULL == RawImage"));
		return false; 
	}

	long CurrentRingBufferIndex = 0;//m_CameraRingBufferIndex+1;
	long Index = 0;//(m_CameraRingBufferIndex)%m_CameraRingBufferListSize;
	if ( -1 == m_CameraRingBufferIndex )
	{	
		Index = 0; 
		CurrentRingBufferIndex = 1;
	}
	else
	{
		CurrentRingBufferIndex = m_CameraRingBufferIndex+1;
		Index = (m_CameraRingBufferIndex)%m_CameraRingBufferListSize;
	}	

	this->m_CameraRingBufferList[Index] = (IMAGE_PTR)ImagePtr; 	//直接指向FrameGrabber的Buffer位址
	m_CameraRingBufferStateList[Index] = CAMERA_RING_BUFFER_STATES_NEW;
	this->m_CameraRingBufferIndex = CurrentRingBufferIndex;
	//this->m_CameraRingBufferIndex = m_CameraRingBufferIndex+1;
	this->m_CameraImagePtr = (IMAGE_PTR)ImagePtr;
	this->IncrementCountForImageCallback();
	
	if ( true == m_SaveCameraAddRingBufferLog )
	{
		CString str;
		str.Format(_T("AddCameraRingBufferList[%d]"), m_CameraRingBufferIndex); 
		CCamera_Basic::SaveCameraProcess(str, MSG_LEVEL_HIGH);	
	}
	
	if ( CheckCameraGrabFinishEvent() == false )
	{	return false; }	
	return true;
}
//------------------------------------------------------------------------------//
inline bool CCamera_Basic::RemoveCameraRingBufferByPtr(void *ImagePtr)//釋放相機回傳影像
{
	if ( NULL == ImagePtr ) { return true; }
	long i=0;
	for ( i=0; i<m_CameraRingBufferListSize; i++ )
	{	
		if ( m_CameraRingBufferList[i] == ImagePtr )
		{
			m_CameraRingBufferList[i] = NULL;
			m_CameraRingBufferStateList[i] = CAMERA_RING_BUFFER_STATES_NONE;
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------//
inline bool CCamera_Basic::RemoveCameraRingBufferByIndex(long index)//移除相機回傳影像-依引數移除
{
	if ( index >= m_CameraRingBufferListSize ) { return false; }		
	m_CameraRingBufferList[index] = NULL;	
	m_CameraRingBufferStateList[index] = CAMERA_RING_BUFFER_STATES_NONE;
	return true;
}
//------------------------------------------------------------------------------//
long CCamera_Basic::GetCameraRingBufferListSize() const//取得目前相機環型指標數量
{
	return this->m_CameraRingBufferListSize;
}
//------------------------------------------------------------------------------//
long CCamera_Basic::GetCameraRingBufferCurrentIndex() const//取得目前相機環型指標引數
{	
	return this->m_CameraRingBufferIndex;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CheckCameraRingBufferStateDone()//確認相機環型記憶體狀態-已完成
{
	for ( long i=0; i<m_CameraRingBufferListSize; i++ )
	{
		if ( CAMERA_RING_BUFFER_STATES_NONE == m_CameraRingBufferStateList[i] )
		{	continue; }
		if ( CAMERA_RING_BUFFER_STATES_DONE != m_CameraRingBufferStateList[i] )
		{	return false; }
	}
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::ReSortCameraRingBufferImage(const TSliceParam &SliceParam)
{
	bool  IsOK=true;
	if ( this->CheckCameraRingBuffer() == false ) { return false; }	
	if ( SliceParam.SliceCameraID != m_CameraID ) { return true; }
	if ( LIGHT_DLP != SliceParam.SliceLightTable.LightType ) { return true; }
	SLICE_FUNC_MODE SliceFuncMode = SliceParam.SliceFuncMode;
	const int DLPLightCnt=AOIDataCollect.CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
	if ( 1 == DLPLightCnt )
	{	IsOK = ReSortCameraRingBufferImage_SepareDLPTable(SliceParam);	}
	else
	{	IsOK = ReSortCameraRingBufferImage_2ndLightDLPTable(SliceParam);	}	
	return IsOK;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::ReSortCameraRingBufferImage_SepareDLPTable(const TSliceParam &SliceParam)//重新排序相機環型指標引數
{
	//return true;
	long  i=0, j=0;
	bool  bNeedReSort=false;
	int   TriggerCount[DLP_CAST_COUNT]={0};
	if ( this->CheckCameraRingBuffer() == false ) { return false; }	
	if ( SliceParam.SliceCameraID != m_CameraID ) { return true; }
	if ( LIGHT_DLP != SliceParam.SliceLightTable.LightType ) { return true; }
	
	for ( i=0; i<DLP_CAST_COUNT; i++ )
	{
		TriggerCount[i] = 0;
		if ( FN_ENABLE == SliceParam.SliceLightTable.DLPCast[i].OnOffState )
		{	TriggerCount[i] = SliceParam.SliceLightTable.DLPCast[i].TriggerCount; }		
		if ( DLP_PATTERN_SEQUENCE_NONE == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_WHITE == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_RGB == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_4_4_1 == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_4_4_2 == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_4_4_M == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		bNeedReSort = true;
		//break;
	}
	if ( false == bNeedReSort ) { return true; }
	
	long  index=0, RingIndex=0;
	const long ImageCallbackCnt= GetCountForImageCallback();
	const long TotalImageCount=SliceParam.SliceCameraFrames;
	const long TotalImageCount2=SliceParam.SliceCameraFrames/2;
	const long CameraRingBufferListSize=m_CameraRingBufferListSize;	
	const int  IndexStart=m_CameraRingBufferIndex-TotalImageCount;
	const int  IndexMiddle=IndexStart+TotalImageCount2;
	std::vector<IMAGE_PTR>  CastBufferList[DLP_CAST_COUNT];	
	std::vector<CAMERA_RING_BUFFER_STATES>  CastBufferStateList[DLP_CAST_COUNT];

	if ( TotalImageCount > ImageCallbackCnt )
	{	
		m_ErrorString.Format(_T("Error, Camera ReSortCameraRingBufferImage Fault"));
		return false; 
	}

	index = IndexStart;
	for ( j=0; j<DLP_CAST_COUNT; j++ )
	{
		if ( 0 == TriggerCount[j] ) { continue; }
		for ( i=0; i<TriggerCount[j]/2; i++ )
		{	
			RingIndex = index%CameraRingBufferListSize;
			CastBufferList[j].push_back(m_CameraRingBufferList[RingIndex]); 
			CastBufferStateList[j].push_back(m_CameraRingBufferStateList[RingIndex]);
			index ++;
		}
	}
	for ( j=0; j<DLP_CAST_COUNT; j++ )
	{
		if ( 0 == TriggerCount[j] ) { continue; }
		for ( i=0; i<TriggerCount[j]/2; i++ )
		{	
			RingIndex = index%CameraRingBufferListSize;
			CastBufferList[j].push_back(m_CameraRingBufferList[RingIndex]); 
			CastBufferStateList[j].push_back(m_CameraRingBufferStateList[RingIndex]);
			index ++;
		}
	}
	if ( index != m_CameraRingBufferIndex )
	{	
		m_ErrorString.Format(_T("Error, Camera ReSortCameraRingBufferImage Fault"));
		return false;
	}

	long    CastBufferCount=0;
	index = IndexStart;
	for ( j=0; j<DLP_CAST_COUNT; j++ )
	{
		if ( 0 == TriggerCount[j] ) { continue; }
		CastBufferCount = (long)(CastBufferList[j].size());
		for ( i=0; i<CastBufferCount; i++ )
		{	
			RingIndex = index%CameraRingBufferListSize;
			m_CameraRingBufferList[RingIndex] = CastBufferList[j][i];
			m_CameraRingBufferStateList[RingIndex] = CastBufferStateList[j][i];
			index ++;
		}		
	}
	if ( index != m_CameraRingBufferIndex )
	{
		m_ErrorString.Format(_T("Error, Camera ReSortCameraRingBufferImage Fault"));
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::ReSortCameraRingBufferImage_2ndLightDLPTable(const TSliceParam &SliceParam)//重新排序相機環型指標引數
{
	long  i=0, j=0;
	bool  bNeedReSort=false;
	int   TriggerCount[DLP_CAST_COUNT]={0};
	if ( this->CheckCameraRingBuffer() == false ) { return false; }	
	if ( SliceParam.SliceCameraID != m_CameraID ) { return true; }
	if ( LIGHT_DLP != SliceParam.SliceLightTable.LightType ) { return true; }
	SLICE_FUNC_MODE SliceFuncMode = SliceParam.SliceFuncMode;
	const int DLPLightCnt=AOIDataCollect.CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
	if ( 1 == DLPLightCnt ) 
	{	return true; }

	for ( i=0; i<DLP_CAST_COUNT; i++ )
	{
		TriggerCount[i] = 0;
		if ( FN_ENABLE == SliceParam.SliceLightTable.DLPCast[i].OnOffState )
		{	TriggerCount[i] = SliceParam.SliceLightTable.DLPCast[i].TriggerCount; }		
		if ( DLP_PATTERN_SEQUENCE_NONE == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_WHITE == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_RGB == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_4_4GC_M2 == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_4_5GC_M2 == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_4_6GC_M2 == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_4_2_M_2 == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }
		if ( DLP_PATTERN_SEQUENCE_4_4_M_2 == SliceParam.SliceLightTable.DLPCast[i].PhasePatMode )
		{	continue; }		
		bNeedReSort = true;
		//break;
	}
	if ( false == bNeedReSort ) { return true; }
	
	long  index=0, RingIndex=0;
	const long ImageCallbackCnt= GetCountForImageCallback();
	const long TotalImageCount=SliceParam.SliceCameraFrames*2;
	const long TotalImageCount2=SliceParam.SliceCameraFrames;
	const long CameraRingBufferListSize=m_CameraRingBufferListSize;	
	const int  IndexStart=m_CameraRingBufferIndex-TotalImageCount;
	const int  IndexMiddle=IndexStart+TotalImageCount2;
	std::vector<IMAGE_PTR>  CastBufferList[DLP_CAST_COUNT];	
	std::vector<CAMERA_RING_BUFFER_STATES>  CastBufferStateList[DLP_CAST_COUNT];	
	
	if ( TotalImageCount > ImageCallbackCnt )
	{	
		m_ErrorString.Format(_T("Error, Camera ReSortCameraRingBufferImage Fault"));
		return false; 
	}

	index = IndexStart;
	for ( j=0; j<DLP_CAST_COUNT; j++ )
	{
		if ( 0 == TriggerCount[j] ) { continue; }
		for ( i=0; i<TriggerCount[j]; i++ )
		{	
			RingIndex = index%CameraRingBufferListSize;
			CastBufferList[j].push_back(m_CameraRingBufferList[RingIndex]); 
			CastBufferStateList[j].push_back(m_CameraRingBufferStateList[RingIndex]); 
			index ++;
		}
	}
	for ( j=0; j<DLP_CAST_COUNT; j++ )
	{
		if ( 0 == TriggerCount[j] ) { continue; }
		for ( i=0; i<TriggerCount[j]; i++ )
		{	
			RingIndex = index%CameraRingBufferListSize;
			CastBufferList[j].push_back(m_CameraRingBufferList[RingIndex]); 
			CastBufferStateList[j].push_back(m_CameraRingBufferStateList[RingIndex]); 
			index ++;
		}
	}
	if ( index != m_CameraRingBufferIndex )
	{	
		m_ErrorString.Format(_T("Error, Camera ReSortCameraRingBufferImage Fault"));
		return false;
	}

	long    CastBufferCount=0;
	index = IndexStart;
	for ( j=0; j<DLP_CAST_COUNT; j++ )
	{
		if ( 0 == TriggerCount[j] ) { continue; }
		CastBufferCount = (long)(CastBufferList[j].size());
		for ( i=0; i<CastBufferCount; i++ )
		{	
			RingIndex = index%CameraRingBufferListSize;
			m_CameraRingBufferList[RingIndex] = CastBufferList[j][i]; 
			m_CameraRingBufferStateList[RingIndex] = CastBufferStateList[j][i];
			index ++;
		}		
	}
	if ( index != m_CameraRingBufferIndex )
	{
		m_ErrorString.Format(_T("Error, Camera ReSortCameraRingBufferImage Fault"));
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::GetCameraRingBufferImage(long idx, unsigned int &ImageW, unsigned int &ImageH, unsigned int &ImageStep, IMAGE_PTR &pDst)//取得環型指標內的影像
{
	if ( this->CheckCameraRingBuffer() == false ) { return false; }	
	const long index = idx%m_CameraRingBufferListSize;
	IMAGE_PTR pSrc = m_CameraRingBufferList[index];
	if ( NULL == pSrc )
	{
		this->m_ErrorString.Format(_T("Error, m_CameraRingBufferList index(%d) is null"), index);
		return false;
	}

	ImageW = this->GetCameraImageW();
	ImageH = this->GetCameraImageH();
	ImageStep = this->GetImageStep();	
	pDst = pSrc;
	m_CameraRingBufferStateList[index] = CAMERA_RING_BUFFER_STATES_DONE;
	return true;
}
//------------------------------------------------------------------------------//
const IMAGE_PTR CCamera_Basic::GetCurrentRawImage()
{
	if ( this->CheckCameraInited() == false ) { return NULL; }
	return this->m_ClonedRawImagePtr;	
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::GetImageFromCamera()
{
	if ( this->CheckCameraInited() == false ) { return false; }
	if ( this->m_CameraImagePtr == NULL ) { return false; }
	if ( this->m_ClonedRawImagePtr == NULL ) { return false; }	
//	AOIDataCollect.ImageAdjust(m_CameraImagePtr, m_ClonedRawImagePtr, CAMERA_5, m_CameraApplyGamma);
	return true;	
}
//-----------------------------------------------------------------------------------------//
bool CCamera_Basic::CloneCurrentRawImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_PTR pDst)
{
	if ( this->CheckCameraInited() == false ) 
	{	return false; }
	if ( pDst == NULL ) 
	{
		this->m_ErrorString.Format(_T("Error, CCamera_Basic::pDest == NULL"));
		return false; 
	}

	ImageW = this->GetCameraImageW();
	ImageH = this->GetCameraImageH();
	ImageStep = this->GetImageStep();
	const size_t ImageRawSize = this->GetImageRawSize();
	::memcpy(pDst, this->m_ClonedRawImagePtr, sizeof(IMAGE_DATA)*ImageRawSize);
	return true;
}
//-----------------------------------------------------------------------------------------//
bool CCamera_Basic::CloneCurrentCameraImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR pDst)
{
	if ( this->CheckCameraInited() == false ) 
	{	return false; }
	if ( NULL == m_CameraImagePtr ) 
	{
		this->m_ErrorString.Format(_T("Error, CCamera_Basic::m_CameraImagePtr == NULL"));
		return false; 
	}
	if ( NULL == pDst ) 
	{ 
		this->m_ErrorString.Format(_T("Error, CCamera_Basic::pDst == NULL"));
		return false; 
	}

	const size_t ImageRawSize = this->GetImageRawSize();
	ImageW = GetCameraImageW();
	ImageH = GetCameraImageH();
	ImageStep = GetImageStep();
	BitCount = GetBitCount();
	::memcpy(pDst, this->m_CameraImagePtr, sizeof(IMAGE_DATA)*ImageRawSize);

	if ( this->CheckCameraRingBuffer() == true )
	{
		const long index = this->m_CameraRingBufferIndex%m_CameraRingBufferListSize;		
		this->m_CameraKeepBufferIndex = index;
		this->m_CameraKeepBufferPtr = m_CameraRingBufferList[index];		
		m_CameraRingBufferStateList[index] = CAMERA_RING_BUFFER_STATES_DONE;
	}	
	return true;
}
//-----------------------------------------------------------------------------------------//
bool CCamera_Basic::CloneCurrentColorImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_PTR pDst)
{
#ifndef OFFLINE_VERSION
	if ( this->CheckCameraInited() == false ) 
	{	return false;	}	
	if ( NULL == pDst ) 
	{ 
		this->m_ErrorString.Format(_T("Error, CCamera_Basic::pDest == NULL"));
		return false; 
	}
	
	bool IsOK = false;
	IMAGE_SIZE ColorStep=0;
	BAYER_PATTERN_MODE BayerMode = GetCameraBayerPattern();
	IMAGE_DEBAYER_MODE DebayerMode = AOIDataCollect.GetSystemParameter().m_DebayerMode;
	ImageW = GetCameraImageW();
	ImageH = GetCameraImageH();
	ImageStep = GetImageStep();	
	switch ( DebayerMode )
	{
	case IMAGE_DEBAYER_OPEN_CV:		
		IsOK = ImageAPI.DebayerColorImage3(ImageW, ImageH, ImageStep, m_ClonedRawImagePtr, BayerMode, ColorStep, pDst);
		break;
	case IMAGE_DEBAYER_RAW_COLOR:		
	//	IsOK = ImageServer.DoDebayer_RawColor(ImageFullW, ImageFullH, this->m_ClonedRawImagePtr, pSrc, m_CameraBayerType);
		break;
	case IMAGE_DEBAYER_RAW_MONO:
		IsOK = ImageAPI.DebayerGrayImage3(ImageW, ImageH, ImageStep, m_ClonedRawImagePtr, BayerMode, pDst);
		break;
	default:
		IsOK = ImageAPI.DebayerColorImage3(ImageW, ImageH, ImageStep, m_ClonedRawImagePtr, BayerMode, ColorStep, pDst);	
		break;
	}	
	if ( false == IsOK )
	{	this->m_ErrorString = ImageAPI.GetImageApiErrorString();	}
	return IsOK;
#endif	
	return true;
}
//-----------------------------------------------------------------------------------------//
bool CCamera_Basic::FillCameraImage(IMAGE_DISPLAY_MODE ImageMode, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr)//依據顯示模式來取回影像資料
{
	const char fnName[] = "CCamera_Basic::FillCameraImage";
	ImageW = CCamera_Basic::m_CameraImageW;
	ImageH = CCamera_Basic::m_CameraImageH;	
	if ( IMAGE_DISPLAY_COLOR == ImageMode )
	{	BitCount = 24;	}
	else
	{	BitCount = 8;	}
	ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t ImageBuffer = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	JetMemory.free_func(ImagePtr);
	if ( JetMemory.alloc_func(ImageBuffer, ImagePtr, fnName, "ImagePtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}
	if ( CCamera_Basic::FillCameraImage3(ImageMode, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{
		JetMemory.free_func(ImagePtr);
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------------------//
bool CCamera_Basic::FillCameraImage3(IMAGE_DISPLAY_MODE ImageMode, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR ImagePtr)//依據顯示模式來取回影像資料
{
	ImageW = CCamera_Basic::m_CameraImageW;
	ImageH = CCamera_Basic::m_CameraImageH;	
	if ( IMAGE_DISPLAY_COLOR == ImageMode )
	{
		long idx=0;
		IMAGE_PTR pR=NULL;
		IMAGE_PTR pG=NULL;
		IMAGE_PTR pB=NULL;				
		const long ImageCount = CCamera_Basic::m_CountForImageCallback;
		const IMAGE_SIZE GrayStep = CCamera_Basic::m_CameraImageStep;
		const long RingIndex = (long)(CCamera_Basic::m_CameraRingBufferIndex);
		const long RingListSize = (long)(CCamera_Basic::m_CameraRingBufferListSize);
		if ( ImageCount < 3 ) 
		{
			this->m_ErrorString.Format(_T("Error, Camera Ring Grab Count too small"));
			return false; 
		}

		idx = RingIndex;
		idx = idx - ImageCount;
		if ( idx < 0 ) { idx += RingListSize;  }
		BitCount = 24;
		ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);

		idx = idx%RingListSize;
		pR = CCamera_Basic::m_CameraRingBufferList[idx];
		m_CameraRingBufferStateList[idx] = CAMERA_RING_BUFFER_STATES_DONE;
		idx++;

		idx = idx%RingListSize;
		pG = CCamera_Basic::m_CameraRingBufferList[idx];
		m_CameraRingBufferStateList[idx] = CAMERA_RING_BUFFER_STATES_DONE;
		idx++;

		idx = idx%RingListSize;
		pB = CCamera_Basic::m_CameraRingBufferList[idx];
		m_CameraRingBufferStateList[idx] = CAMERA_RING_BUFFER_STATES_DONE;
		idx++;
		if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, GrayStep, pR, pG, pB, ImageStep, ImagePtr, false) == false )
		{
			this->m_ErrorString = ImageAPI.GetImageApiErrorString();
			return false; 
		}
	}
	else
	{
		BitCount = 8;
		ImageStep=CCamera_Basic::m_CameraImageStep;
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		::memcpy(ImagePtr, m_CameraImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	}
	return true;
}
//-----------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraGrabMode(CAMERA_GRAB_MODE val)
{
	m_CameraGrabMode = val;
}
//-----------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraShutterMode(CAMERA_SHUTTER_MODE val)
{
	m_CameraShutterMode = val;
}
//-----------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraExposureMode(CAMERA_EXPOSURE_MODE val)
{
	m_CameraExposureMode = val;
}
//-----------------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraImageBufferCount(unsigned int val)//相機影像緩存數量
{
	m_CameraImageBufferCount = val;
}
//-----------------------------------------------------------------------------------------//
CString CCamera_Basic::GetCameraINIFileName()
{
	CString Filename;	
	CString ShortName;	
#ifdef TB_SYSTEM_ONLY_BOT
	ShortName = _T("Camera_Bot.INI");
#else
	ShortName = _T("Camera.INI");
#endif//TB_SYSTEM_ONLY_BOT
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);
	return Filename;
}
//-----------------------------------------------------------------------------------------//
CString CCamera_Basic::GetCameraSectionName()
{
	return m_CameraModelName;
}
//------------------------------------------------------------------------------//
CString CCamera_Basic::GetCameraBasicSection()
{
	return _T("Camera_Basic");
}
//------------------------------------------------------------------------------//
int CCamera_Basic::CalcCameraImageOffset(int SensorSize, int ImageSize, int OffsetInc) const//計算影像尺寸變化時的偏移值
{
	int Offset=0;
	if ( SensorSize < ImageSize ) { return Offset; }
	int PadSize=SensorSize-ImageSize;
	if ( PadSize < OffsetInc ) { return Offset; }

	Offset = (PadSize)/2;
	Offset = (Offset+OffsetInc-1)/OffsetInc;
	Offset *= OffsetInc;			
	return Offset;
}
//-----------------------------------------------------------------------------------------//
bool CCamera_Basic::SaveCameraProcess(LPCTSTR fnName, int Level)
{
	CString str;
	if ( GetCameraConstructed() == false ) { return true; }
	str.Format(_T("Camera[%d]::%s"), m_CameraID, fnName);
	if ( AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_CAMERA, Level, str) == false )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------------------------//
bool CCamera_Basic::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Basic::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCamera_Basic::SetExposureTime(const int ExposureTime)
{
	if ( this->CheckCameraInited() == false ) { return false; }
	this->m_ExposureTime_us = ExposureTime;	
	return true;
}
//------------------------------------------------------------------------------//
CAMERA_GRAB_MODE CCamera_Basic::GetCameraGrabMode() const
{
	return m_CameraGrabMode;
}
//------------------------------------------------------------------------------//
/*
bool CCamera_Basic::SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode)
{
	if ( this->CheckCameraInited() == false ) { return false; }
	if ( this->StopCameraGrab() == false ) { return false; }	
	switch ( Mode )
	{
	case CAMERA_GRAB_FREE_RUN:
	case CAMERA_GRAB_EXTERNAL_TRIGGER:
	case CAMERA_GRAB_SOFTWARE_TRIGGER:
		this->m_CameraGrabMode = Mode;		
		break;
	default:
		this->m_ErrorString.Format(_T("Error, Camera Grab Mode Excpetion"));
		return false;
		break;
	}	
	return true;	 
}
//------------------------------------------------------------------------------//
*/
bool CCamera_Basic::DeleteCameraGrabFinishEvent()
{
	if ( this->m_CameraGrabFinishEvent == NULL ) { return true; }
	::CloseHandle(this->m_CameraGrabFinishEvent);
	this->m_CameraGrabFinishEvent = NULL;	
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CreateCameraGrabFinishEvent()
{
	this->DeleteCameraGrabFinishEvent();
	this->m_CameraGrabFinishEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Camera Grab Finish Event"));
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::WaitForCameraGrabFinishEvent()
{
#ifndef OFFLINE_VERSION
	if ( this->m_CameraGrabFinishEvent == NULL ) { return true; }
	
	const double CameraFPS = this->GetCameraFPS();
	DWORD  Res=0;
	DWORD  WaitTime = 0;
	DWORD  ExposureTime = this->m_ExposureTime_us;
	DWORD  FPS_Time     = (unsigned int)(1000000/CameraFPS);		
	DWORD  TotalWaitTime = 1000*2;//2 Sec
	DWORD  MaxWaitCount = 1;//TotalTime = 5Sec;
	DWORD  i            = 0;

#ifdef _DEBUG	
	TotalWaitTime = 5*60*1000;//5 min	
#else
	TotalWaitTime = 2*GetCameraGrabTimeout();
#endif

#ifdef SAVE_LOG_MSG_SYNC_USE
	TotalWaitTime *= 4;
#endif//SAVE_LOG_MSG_SYNC_USE

	if ( ExposureTime > FPS_Time )
	{	WaitTime = ExposureTime; }
	else
	{	WaitTime = FPS_Time; }
	WaitTime = WaitTime*50;//us
	WaitTime = WaitTime/1000;//ms

	MaxWaitCount = TotalWaitTime/WaitTime;
	if ( MaxWaitCount <= 0 ) { MaxWaitCount = 1; }

	for ( i=0; i<MaxWaitCount; i++ )
	{
		if ( AOIDataCollect.GetIsSystemException() == true ) 
		{	break; }
		
		Res = ::WaitForSingleObject(this->m_CameraGrabFinishEvent, WaitTime);
		if ( Res == WAIT_OBJECT_0 )
		{	break; }
	}
	if ( i == MaxWaitCount )
	{
		this->m_ErrorString.Format(_T("Error, Wait for m_CameraGrabFinishEvent too long [%dms]"), TotalWaitTime);
		return false;
	}
#endif
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::SetCameraGrabFinishEvent()
{
	if ( this->m_CameraGrabFinishEvent == NULL ) { return true; }
	::SetEvent(this->m_CameraGrabFinishEvent);
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::ResetCameraGrabFinishEvent()
{
	if ( this->m_CameraGrabFinishEvent == NULL ) { return true; }
	::ResetEvent(this->m_CameraGrabFinishEvent);
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CheckCameraGrabFinishEvent()//確認相機取像完成事件
{	
	const long cntBatchGrab = GetCountForBatchGrab();
	const long cntImageBack = GetCountForImageCallback();	
	CAMERA_CALLBACK_TIMMING CallbackTimming = GetCameraCallbackTimming();
	THREAD_GRAB_MODE  ThreadGrabMode = AOIDataCollect.GetThreadGrabMode();
	
	WPARAM wParam = GetCameraWParam();
	if ( CAMERA_CALLBACK_FREE_FRAME == CallbackTimming )
	{
		//SetCameraExposureFinishEvent();
		SetCameraGrabFinishEvent();
		if ( THREAD_GRAB_NONE==ThreadGrabMode && GetCameraToSendCallback()==TRUE )
		{	PostCallbackWndMessage(MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK);	}
	}
	else if ( CAMERA_CALLBACK_BATCH_GRAB_DONE==CallbackTimming )
	{
		if ( cntBatchGrab<0 || (cntImageBack%cntBatchGrab) == 0 )
		{
			//SetCameraExposureFinishEvent();
			SetCameraGrabFinishEvent();
			if ( THREAD_GRAB_NONE == ThreadGrabMode )
			{
				//::SendMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK); 
				PostCallbackWndMessage(MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK);					
			}
		}
		else
		{	
			if ( THREAD_GRAB_NONE == ThreadGrabMode )
			{
				//::SendMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_BYPASS_CALLBACK); 
				PostCallbackWndMessage(MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_BYPASS_CALLBACK);					
			}
		}
	}
	else //CAMERA_CALLBACK_EACH_FRAME
	{
		if ( THREAD_GRAB_NONE == ThreadGrabMode )
		{
			//SetCameraExposureFinishEvent();
			SetCameraGrabFinishEvent();
			//::SendMessage(hWndCallback, MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK);	
			PostCallbackWndMessage(MSG_CAMERA_CALLBACK, wParam, LPARAM_CAMERA_FRAME_CALLBACK);				
		}
	}
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::DeleteCameraExposureFinishEvent()
{
	if ( this->m_CameraExposureFinishEvent == NULL ) { return true; }
	::CloseHandle(this->m_CameraExposureFinishEvent);
	this->m_CameraExposureFinishEvent = NULL;	
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CreateCameraExposureFinishEvent()
{
	this->DeleteCameraExposureFinishEvent();
	this->m_CameraExposureFinishEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Camera Exposure Finish Event"));
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::SetCameraExposureFinishEvent()
{
	if ( this->m_CameraExposureFinishEvent == NULL ) { return true; }
	::SetEvent(this->m_CameraExposureFinishEvent);
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::ResetCameraExposureFinishEvent()
{
	if ( this->m_CameraExposureFinishEvent == NULL ) { return true; }
	::ResetEvent(this->m_CameraExposureFinishEvent);
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CheckCameraExposuredEndEvent()//確認相機曝光完成事件
{
	CAMERA_CALLBACK_TIMMING CallbackTimming = GetCameraCallbackTimming();
	if ( CAMERA_CALLBACK_BATCH_GRAB_DONE==CallbackTimming )
	{
		const long cntBatchGrab = GetCountForBatchGrab();
		const long cntExposureEnd = GetCountForCameraExposuredEnd();
		if ( cntBatchGrab<0 || (cntExposureEnd%cntBatchGrab) == 0 )
		{	SetCameraExposureFinishEvent(); }
	}
	else
	{	SetCameraExposureFinishEvent(); }
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::SetCameraGrabbing(bool Grabbing)
{
	if (m_CameraGrabbing == Grabbing )
	{	return true; }
	LockCamera();	
	SetCameraGrabbing_Unlock(Grabbing);	
	UnlockCamera();
	return true;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraGrabbing_Unlock(bool Grabbing)
{
	m_CameraGrabbing = Grabbing;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::GetCameraGrabbing()
{
	return m_CameraGrabbing;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::WaitForCameraExposureFinishEvent()
{
#ifndef OFFLINE_VERSION
	if ( this->m_CameraExposureFinishEvent == NULL ) { return true; }	

	const double CameraFPS = this->GetCameraFPS();
	DWORD MinTime = GetCameraGrabTimeout();
	DWORD FPS_Time = (DWORD)(1000.0/CameraFPS);
	DWORD ExposureTime = m_ExposureTime_us/1000;
	DWORD GrabImageCount=GetCountForFramesToGrab();

	FPS_Time     *= GrabImageCount;
	ExposureTime *= GrabImageCount;

	if ( MinTime < ExposureTime )
	{	MinTime = ExposureTime;	}
	if ( MinTime < FPS_Time )
	{	MinTime = FPS_Time;	}
#ifdef SAVE_LOG_MSG_SYNC_USE
	MinTime *= 4;
#endif//SAVE_LOG_MSG_SYNC_USE

	DWORD Res=0;
	Res = ::WaitForSingleObject(this->m_CameraExposureFinishEvent, MinTime);
	if ( Res == WAIT_TIMEOUT )
	{
		this->m_ErrorString.Format(_T("Error, Wait for m_CameraExposureFinishEvent too long [%dms]"), MinTime);
		return false;
	}
#endif	
	return true;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraFunctionStartTime()//設定相機函數起始時間
{
	QueryPerformanceCounter(&m_CameraFnStartTime);	
}
//------------------------------------------------------------------------------//
void CCamera_Basic::SetCameraFunctionEndTime()  //設定相機函數結束時間
{
	QueryPerformanceCounter(&m_CameraFnEndTime);	
}
//------------------------------------------------------------------------------//
double CCamera_Basic::GetCameraFunctionElapseTime()//取得相機函數經過時間
{
	double Time = (double)((m_CameraFnEndTime.QuadPart - m_CameraFnStartTime.QuadPart) * 1000.0/m_CameraFrequnce.QuadPart);
	return Time;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::CreateBMPInfo(BITMAPINFO *&pBMPInfo, bool IsColor)//建立一個本系統可用的BMP Infor
{
	DWORD bitmapInfoSize;
	delete[] pBMPInfo; pBMPInfo=NULL;
	const int NPattle = 256;

	const IMAGE_SIZE ImageW = this->GetCameraImageW();
	const IMAGE_SIZE ImageH = this->GetCameraImageH();

	if ( IsColor == true )
	{
		bitmapInfoSize = sizeof(BITMAPINFO);
		pBMPInfo = (BITMAPINFO*)new BYTE[bitmapInfoSize];
		if ( pBMPInfo == NULL ) 
		{	
			this->m_ErrorString.Format(_T("Error, Memory Allocate Fault"));	
			return false;	
		}

		pBMPInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBMPInfo->bmiHeader.biPlanes = 1;
		pBMPInfo->bmiHeader.biBitCount = 24;
		pBMPInfo->bmiHeader.biCompression = BI_RGB;
		pBMPInfo->bmiHeader.biSizeImage = ImageW*ImageH;
		pBMPInfo->bmiHeader.biXPelsPerMeter = 0;
		pBMPInfo->bmiHeader.biYPelsPerMeter = 0;
		pBMPInfo->bmiHeader.biClrUsed = 0;
		pBMPInfo->bmiHeader.biClrImportant = 0;

		pBMPInfo->bmiHeader.biWidth = ImageW;//2448
		pBMPInfo->bmiHeader.biHeight = ImageH;//2050
		pBMPInfo->bmiHeader.biHeight = -pBMPInfo->bmiHeader.biHeight;
	}
	else
	{
		bitmapInfoSize = sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD);
		pBMPInfo = (BITMAPINFO*)new BYTE[bitmapInfoSize];
		if ( pBMPInfo == NULL ) 
		{	
			this->m_ErrorString.Format(_T("Error, Memory Allocate Fault"));	
			return false;	
		}

		pBMPInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pBMPInfo->bmiHeader.biPlanes = 1;
		pBMPInfo->bmiHeader.biBitCount = 8;
		pBMPInfo->bmiHeader.biCompression = BI_RGB;
		pBMPInfo->bmiHeader.biSizeImage = ImageW*ImageH;
		pBMPInfo->bmiHeader.biXPelsPerMeter = 0;
		pBMPInfo->bmiHeader.biYPelsPerMeter = 0;
		pBMPInfo->bmiHeader.biClrUsed = 0;
		pBMPInfo->bmiHeader.biClrImportant = 0;

		pBMPInfo->bmiHeader.biWidth = ImageW;//2448
		pBMPInfo->bmiHeader.biHeight = ImageH;//2050
		pBMPInfo->bmiHeader.biHeight = -pBMPInfo->bmiHeader.biHeight;

		int i=0; 
		for ( i=0; i<256; i++ )
		{
			pBMPInfo->bmiColors[i].rgbRed      = i;
			pBMPInfo->bmiColors[i].rgbGreen    = i;
			pBMPInfo->bmiColors[i].rgbBlue     = i;
			pBMPInfo->bmiColors[i].rgbReserved = 0;
		}
	}
	return true;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::DrawColorImageToDC(HDC hDC, const unsigned char *pColoeImage, RECT &ImageWndRect, POINT &OffsetPts, double RealScaleX, double RealScaleY, double ZoomScale)
{
	if ( pColoeImage == NULL ) { return false; }

	BITMAPINFO *pImageInfo = this->GetColorBitmapInfo();
	
	const double sizeX = (ImageWndRect.right-ImageWndRect.left)/2.0;
	const double sizeY = (ImageWndRect.bottom-ImageWndRect.top)/2.0;

	const IMAGE_SIZE ImageFullW = this->GetCameraImageW();
	const IMAGE_SIZE ImageFullH = this->GetCameraImageH();
	const IMAGE_SIZE ImageHalfW = ImageFullW/2;
	const IMAGE_SIZE ImageHalfH = ImageFullH/2;
	
	RECT SrcRect;
	POINT pt;
	pt.x = (ImageWndRect.left+ImageWndRect.right)/2;
	pt.y = (ImageWndRect.top+ImageWndRect.bottom)/2;
	SrcRect.left   = (long)(ImageHalfW - sizeX/RealScaleX);
	SrcRect.top    = (long)(ImageHalfH - sizeY/RealScaleY);
	SrcRect.right  = (long)(ImageHalfW + sizeX/RealScaleX);
	SrcRect.bottom = (long)(ImageHalfH + sizeY/RealScaleY);	

	const int BltMode = AOIDataCollect.GetSystemParameter().m_StretchBltMode;
	if ( BltMode != HALFTONE )
	{
		HBRUSH hBrush = ::CreateSolidBrush(0x000000);
		if ( hBrush != NULL )
		{
			::FillRect(hDC, &ImageWndRect, hBrush);
			::DeleteObject(hBrush); hBrush=NULL;
		}
	}
	if ( ZoomScale < 0   )
	{
		SrcRect.left   = (long)0;
		SrcRect.top    = (long)0;
		SrcRect.right  = (long)ImageFullW;
		SrcRect.bottom = (long)ImageFullH;		
		int OldMode = ::SetStretchBltMode(hDC, BltMode);		//m_pBinaryImage
		int FrameW =  ImageWndRect.right-ImageWndRect.left;
		int FrameH = ImageWndRect.bottom-ImageWndRect.top;

		double ScaleX = (double)FrameW/(double)ImageFullW;
		double ScaleY = (double)FrameH/(double)ImageFullH;
		double TempScale = (ScaleX>ScaleY)?ScaleY:ScaleX;

		int ImageW = (int)(ImageFullW*TempScale);		
		int ImageH = (int)(ImageFullH*TempScale);

		RECT DestRect = ImageWndRect;
		DestRect.right = DestRect.left+ImageW;
		DestRect.bottom = DestRect.top+ImageH;
		::StretchDIBits (
			hDC,	// handle of device context
			DestRect.left,	// x-coordinate of upper-left corner of dest. rect. 
			DestRect.top,	// y-coordinate of upper-left corner of dest. rect. 
			DestRect.right,	// width of destination rectangle 
			DestRect.bottom,	// height of destination rectangle 
			SrcRect.left,	// x-coordinate of lower-left corner of source rect. 
			SrcRect.top,	// y-coordinate of lower-left corner of source rect. 
			SrcRect.right,	// source rectangle width 
			SrcRect.bottom,	// source rectangle height 
			pColoeImage,	// address of array with DIB bits 
			pImageInfo,	// address of structure with bitmap info. 
			DIB_RGB_COLORS,	// RGB or palette indices 
			SRCCOPY		// raster operation code
		);
		::SetStretchBltMode(hDC, OldMode);
	}
	else
	{
		int OldMode = ::SetStretchBltMode(hDC, BltMode);		//m_pBinaryImage
		SIZE size;
		size.cx =  ImageWndRect.right-ImageWndRect.left;
		size.cy = ImageWndRect.bottom-ImageWndRect.top;

		size.cx = (long)(size.cx*ZoomScale);
		size.cy = (long)(size.cy*ZoomScale);
		POINT SP;
		SP.x = ImageHalfW-OffsetPts.x-size.cx/2;		
		SP.y = ImageHalfH+OffsetPts.y-size.cy/2;

		::StretchDIBits (
			hDC,	// handle of device context
			ImageWndRect.left,	// x-coordinate of upper-left corner of dest. rect. 
			ImageWndRect.top,	// y-coordinate of upper-left corner of dest. rect. 
			ImageWndRect.right,	// width of destination rectangle 
			ImageWndRect.bottom,	// height of destination rectangle 
			SP.x,	// x-coordinate of lower-left corner of source rect. 
			SP.y,	// y-coordinate of lower-left corner of source rect. 
			size.cx,	// source rectangle width 
			size.cy,	// source rectangle height 
			pColoeImage,	// address of array with DIB bits 
			pImageInfo,	// address of structure with bitmap info. 
			DIB_RGB_COLORS,	// RGB or palette indices 
			SRCCOPY		// raster operation code
		);
		::SetStretchBltMode(hDC, OldMode);	
	}
	return true;	
}
//------------------------------------------------------------------------------------------//
bool CCamera_Basic::SaveCameraINIFile()
{
	CCamera_Basic::SaveCameraProcess(_T("SaveCameraINIFile"), MSG_LEVEL_HIGH);		
	
	bool    IsOK = true;	
	CString Section;
	CString KeyName;
	CString KeyString;	
	CString FileName = CCamera_Basic::GetCameraINIFileName();	

	Section = CCamera_Basic::GetCameraBasicSection();	
	KeyName.Format(_T("Camera Model ID")); KeyString.Format(_T("%d"), m_CameraModelID);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Camera Model Name")); KeyString = GetCameraSectionName();
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

#if CAMERA_OBJ_MODE != CAMERA_OBJ_GENERAL_OBJECT
	KeyName.Format(_T("Camera Class Type")); KeyString.Format(_T("%d"), m_CameraModelID);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
#endif//CAMERA_OBJ_MODE

	Section = CCamera_Basic::GetCameraSectionName();	
	KeyName.Format(_T("Camera Model ID")); KeyString.Format(_T("%d"), m_CameraModelID);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Camera Size W")); KeyString.Format(_T("%d"),m_CameraImageW);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Camera Size H")); KeyString.Format(_T("%d"),m_CameraImageH);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Camera Bit Count")); KeyString.Format(_T("%d"),m_CameraBitCount);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Camera FPS")); KeyString.Format(_T("%.2f"),m_CameraFPS);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
	
	KeyName.Format(_T("Camera Min Period Time")); KeyString.Format(_T("%d"), m_PeriodTim_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }
	return IsOK;
}
//------------------------------------------------------------------------------------------//
bool CCamera_Basic::LoadCameraINIFile()
{
	CCamera_Basic::SaveCameraProcess(_T("LoadCameraINIFile"), MSG_LEVEL_HIGH);
	
	CString KeyName;
	CString Section;
	CString KeyString;	
	CString FileName = CCamera_Basic::GetCameraINIFileName();
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];

	Section = CCamera_Basic::GetCameraBasicSection();	
	KeyName.Format(_T("Camera Model ID")); KeyString.Format(_T("%d"),m_CameraModelID);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraModelID = ::_ttoi(ReturnString); }

	Section = CCamera_Basic::GetCameraSectionName();	
	KeyName.Format(_T("Camera Model ID")); KeyString.Format(_T("%d"),m_CameraModelID);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraModelID = ::_ttoi(ReturnString); }

	KeyName.Format(_T("Camera Size W")); KeyString.Format(_T("%d"),m_CameraImageW);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraImageW = ::_ttoi(ReturnString); }

	KeyName.Format(_T("Camera Size H")); KeyString.Format(_T("%d"),m_CameraImageH);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraImageH = ::_ttoi(ReturnString); }

	KeyName.Format(_T("Camera Bit Count")); KeyString.Format(_T("%d"),m_CameraBitCount);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraBitCount = ::_ttoi(ReturnString); }

	KeyName.Format(_T("Camera FPS")); KeyString.Format(_T("%.2f"),m_CameraFPS);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_CameraFPS = ::_ttof(ReturnString); }

	KeyName.Format(_T("Camera Min Period Time")); KeyString.Format(_T("%d"),m_PeriodTim_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_PeriodTim_us = ::_ttoi(ReturnString); }

	CalcCameraImageSize();
	return true;
}
//------------------------------------------------------------------------------------------//
void CCamera_Basic::ClearCountAll()//清除所有計數器
{
	this->m_CountForCameraCallback = 0;//相機回傳-Step01
	this->m_CountForCameraExposuredEnd = 0;//曝光結束回傳-Step02
	this->m_CountForImageCallback = 0;//影像回傳-Step03
	this->m_CountForBufferCopyToHost = 0;//影像複製-Step04
}
//------------------------------------------------------------------------------------------//
void  CCamera_Basic::IncrementCountForCameraCallback()//疊加相機回傳-Step01
{	
	//m_CountForCameraCallback ++;//相機回傳-Step01	
	::InterlockedIncrement(&m_CountForCameraCallback);
}
//------------------------------------------------------------------------------------------//
void CCamera_Basic::IncrementCountForCameraExposuredEnd()//疊加曝光結束回傳-Step02
{
	//m_CountForCameraExposuredEnd ++;//曝光結束回傳-Step02	
	::InterlockedIncrement(&m_CountForCameraExposuredEnd);
}
//------------------------------------------------------------------------------------------//
void CCamera_Basic::IncrementCountForImageCallback()//疊加影像回傳-Step03
{
	//m_CountForImageCallback ++;//影像回傳-Step03	
	::InterlockedIncrement(&m_CountForImageCallback);
}
//------------------------------------------------------------------------------------------//
void CCamera_Basic::IncrementCountForBufferCopyToHost()//疊加影像複製-Step04	
{
	//m_CountForBufferCopyToHost ++;//影像複製-Step04		
	::InterlockedIncrement(&m_CountForBufferCopyToHost);
}
//------------------------------------------------------------------------------------------//
long CCamera_Basic::GetCountForCameraCallback()//疊加相機回傳-Step01
{
	return m_CountForCameraCallback;//相機回傳-Step01
}
//------------------------------------------------------------------------------------------//
long CCamera_Basic::GetCountForCameraExposuredEnd()//疊加曝光結束回傳-Step02
{	
	return m_CountForCameraExposuredEnd;//曝光結束回傳-Step02
}
//------------------------------------------------------------------------------------------//
long  CCamera_Basic::GetCountForImageCallback()//疊加影像回傳-Step03
{	
	return m_CountForImageCallback;//影像回傳-Step03
}
//------------------------------------------------------------------------------------------//
long CCamera_Basic::GetCountForBufferCopyToHost()//疊加影像複製-Step04
{	
	return m_CountForBufferCopyToHost;//影像複製-Step04
}
//------------------------------------------------------------------------------------------//
long CCamera_Basic::GetCountForFramesToGrab()//取得多少張數要去取
{
	return this->m_CountForFramesToGrab;
}
//------------------------------------------------------------------------------------------//
bool CCamera_Basic::SetCountForFramesToGrab(long num)//設定多少張數要去取
{
	this->m_CountForFramesToGrab = num;
	return true;
}
//------------------------------------------------------------------------------//
long CCamera_Basic::GetCountForBatchGrab()//取得批次取像張數
{
	return this->m_CountForGrabLoop;
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::SetCountForBatchGrab(long loop)//設定批次取像張數
{
	this->m_CountForGrabLoop = loop;
	return true;
}
//------------------------------------------------------------------------------//
DWORD CCamera_Basic::GetCameraGrabTimeout() const//取得相機取像逾時時間-ms
{
	return 5000;//5sec
}
//------------------------------------------------------------------------------//
bool CCamera_Basic::WaitForCameraImageCallbackCount(long ImageCount)//等待相機影像回來
{	
	long  RepeatCount=0;
	long  NowImageCount=0;
	long  LastImageCount=0;	
	const long MaxRepeatCount=100;
	DWORD TickCountGap=0;
	DWORD TickCount=GetTickCount();
	while ( true )
	{	
		NowImageCount = GetCountForImageCallback();
		if ( NowImageCount >= ImageCount ) 
		{	break; }
		if ( NowImageCount != LastImageCount )
		{	
			RepeatCount = 0; 
			TickCount=GetTickCount();
		}
		else
		{	RepeatCount ++; }

		TickCountGap=GetTickCount()-TickCount;
		//if ( RepeatCount >  MaxRepeatCount )
		if ( TickCountGap > 1000 )
		{	
			m_ErrorString.Format(_T("Error, WaitForCameraImageCallbackCount Fault [Grab Count:%d, Need Count%d]"), NowImageCount, ImageCount);			
			return false;
		}
		LastImageCount = NowImageCount;
		::Sleep(0);
		//::Sleep(10);
	};
	return true;
}
//------------------------------------------------------------------------------//
void CCamera_Basic::InitialGammaLUT()
{
	this->m_CameraUsingGamma = false;

	const int MaxI = 256;
	int i=0;
	for ( i=0; i<MaxI; i++ )
	{	this->m_CameraGammaLUT[i] = i; }
}
//------------------------------------------------------------------------------------------//
void CCamera_Basic::BuildGammaLUT(const double gamma)
{
	const int MaxI = 256;
	int i=0;
	this->InitialGammaLUT();
	
	CString Gamma_File_Path;
	CString MainFolder = AOIDataCollect.GetAOIDirectory();
	Gamma_File_Path.Format(_T("%s\\GLUT.txt"), MainFolder);
	FILE * Gamma_File;
	Gamma_File = ::_tfopen(Gamma_File_Path, _T("r+"));
	if ( fabs(gamma-1.00)<0.001 && Gamma_File == NULL) { return; }	

	double value = 0;
	double NewValue = 0;
	double MaxValue = 255.0;
	double TempV = 0;
	for ( i=0; i<MaxI; i++ )
	{
		value = i;
		TempV = value/MaxValue;
		TempV = pow(TempV, gamma);
		NewValue = TempV*MaxValue;
		this->m_CameraGammaLUT[i] = (int)(NewValue);
	}

	if(Gamma_File != NULL)
	{
		int Gamma_Value;
		for( i =0 ; i < MaxI; i++)
		{
			::_ftscanf(Gamma_File , _T("%d\n"), &Gamma_Value);
			this->m_CameraGammaLUT[i] = Gamma_Value;
		}
		::fclose(Gamma_File);
		Gamma_File = NULL;
	}

	this->m_CameraUsingGamma = true;
#ifdef _DEBUG
	CString str;	
	FILE *pfile = NULL;	
	str.Format(_T("%s\\GammaLUT.TXT"), AOIDataCollect.GetAOITempDirectory());
	pfile = ::_tfopen(str, _T("w+"));
	if ( pfile != NULL )
	{
		for ( i=0; i<MaxI; i++ )
		{
			::_ftprintf(pfile, _T("%d, %d\n"), i, this->m_CameraGammaLUT[i]);
		}
		::fclose(pfile);
		pfile = NULL;
	}
#endif
}
//------------------------------------------------------------------------------------------//
unsigned char* CCamera_Basic::GetGammaLUTPtr()
{
	return this->m_CameraGammaLUT;
}
//------------------------------------------------------------------------------------------//
BOOL CCamera_Basic::GetUsingGamma()
{
	return this->m_CameraUsingGamma;
}
//------------------------------------------------------------------------------------------//
