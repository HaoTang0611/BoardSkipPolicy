// AOISlice.cpp: implementation of the CAOISlice class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOISlice.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CRITICAL_SECTION  CAOISlice::m_csSlice;//同步機制-關鍵區間
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOISlice, CAOIObj)
//-------------------------------------------------------------------------------------//
void CAOISlice::InitialSliceLock()//初始化相機內區域的關鍵區間
{
	::InitializeCriticalSection(&m_csSlice);
}
//-------------------------------------------------------------------------------------//
void CAOISlice::DeleteSliceLock() //刪除相機內區域的關鍵區間
{
	::DeleteCriticalSection(&m_csSlice);
}
//-------------------------------------------------------------------------------------//
void CAOISlice::LockSlice()//進入相機內區域的關鍵區間
{
	::EnterCriticalSection(&m_csSlice);
}
//-------------------------------------------------------------------------------------//
void CAOISlice::UnlockSlice()//離開相機內區域的關鍵區間
{
	::LeaveCriticalSection(&m_csSlice);
}
//-------------------------------------------------------------------------------------//
CAOISlice::CAOISlice():CAOIObj(AOI_OBJ_SLICE)
{
	PreInitSlice();
	InitialSlice();
}
//-------------------------------------------------------------------------------------//
CAOISlice::CAOISlice(const CAOISlice &slice):CAOIObj(slice)
{
	PreInitSlice();
	CloneSlice(slice);
}
//-------------------------------------------------------------------------------------//
CAOISlice::~CAOISlice()
{
	ClearSliceImageBuffer();
}
//-------------------------------------------------------------------------------------//
CAOISlice& CAOISlice::operator=(const CAOISlice &slice)
{
	if ( this == &slice ) { return *this; }
	CAOIObj::operator=(slice);
	CloneSlice(slice);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOISlice::PreInitSlice()
{	
	CAOISlice::m_SliceImagePtr = NULL;//相機內區域的影像指標
}
//-------------------------------------------------------------------------------------//
inline void CAOISlice::InitialSlice()
{
	m_SliceIndex = -1;//相機內區域的引數編號		

	m_SliceFovIdx = -1;//相機內區域的視野編號
	m_SliceFovPtr = NULL;//相機內區域的視野指標

	m_SliceFrameIdx = -1;
	m_SliceCameraID = PRIMARY_CAMERA_ID;//相機內區域的相機編號	
	m_SliceLightMode = LIGHT_MODE_A;//相機內區域的燈源編號	
	m_SliceLight3DCastID = LIGHT_3D_CAST_00;//相機內區域的樣板投光編號

	m_SliceGainValue = 1.0;//相機內區域的亮度增益比例

	m_SliceCameraFrameIdx=-1;
	m_SliceCameraFrameRect.left=m_SliceCameraFrameRect.right=0;//相機內區域在相機圖像的區域
	m_SliceCameraFrameRect.top=m_SliceCameraFrameRect.bottom=0;//相機內區域在相機圖像的區域
	m_SliceCadPos = TPOINT2D();//相機內區域在Cad的位置
	m_SliceStagePos = TPOINT3D();//相機內區域在Stage的位置

	m_SliceImageType = SLICE_IMAGE_GRAY;
	m_SliceFillState = SLICE_FILL_NONE;
	CAOISlice::ClearSliceImageBuffer();
}
//-------------------------------------------------------------------------------------//
inline void CAOISlice::CloneSlice(const CAOISlice &slice)
{
	CAOISlice::CloneSliceImage(slice);

	m_SliceIndex = slice.m_SliceIndex;//相機內區域的引數編號	

	m_SliceFovIdx = slice.m_SliceFovIdx;//相機內區域的視野編號
	m_SliceFovPtr = slice.m_SliceFovPtr;//相機內區域的視野指標

	m_SliceFrameIdx = slice.m_SliceFrameIdx;
	m_SliceCameraID = slice.m_SliceCameraID;//相機內區域的相機編號	
	m_SliceLightMode = slice.m_SliceLightMode;//相機內區域的燈源編號
	m_SliceLight3DCastID = slice.m_SliceLight3DCastID;//相機內區域的樣板投光編號

	m_SliceGainValue = slice.m_SliceGainValue;//相機內區域的亮度增益比例

	m_SliceCameraFrameIdx = slice.m_SliceCameraFrameIdx;//相機內區域的相機圖像引數編號		
	m_SliceCameraFrameRect = slice.m_SliceCameraFrameRect;//相機內區域在相機圖像的區域	
	m_SliceCadPos = slice.m_SliceCadPos;//相機內區域在Cad的位置	
	m_SliceStagePos = slice.m_SliceStagePos;//相機內區域在Stage的位置	
	m_SliceImageType = slice.m_SliceImageType;//相機內區域的影像格式
	m_SliceFillState = slice.m_SliceFillState;//相機內區域計算狀態
}
//-------------------------------------------------------------------------------------//
CAOISlice* CAOISlice::CloneSliceObj() const//建立且複製一個相機內區域
{
	CAOISlice *ObjPtr = AOIObjManager.CreateSliceObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
void CAOISlice::CloneSliceImage(const CAOISlice &slice)//複製相機內區域影像記憶體區塊
{
	CAOISlice::ClearSliceImageBuffer();
	if ( NULL == slice.m_SliceImagePtr )
	{	return; }
	CAOISlice::SetSliceImageBuffer(slice.m_SliceImageW, slice.m_SliceImageH,slice.m_SliceImageStep, slice.m_SliceImageBitCount, slice.m_SliceImagePtr, true);
}
//-------------------------------------------------------------------------------------//
void CAOISlice::ClearSliceImageBuffer()//清除相機內區域影像記憶體區塊
{
	if ( NULL != m_SliceImagePtr )
	{	JetMemory.free_func(m_SliceImagePtr); }

	m_SliceImageW=0;//相機內區域的影像寬度
	m_SliceImageH=0;//相機內區域的影像高度
	m_SliceImageStep=0;//相機內區域的影像步長
	m_SliceImageBitCount=8;//相機內區域的影像位元數	
}
//-------------------------------------------------------------------------------------//
void CAOISlice::ReleaseSliceImageBuffer()//釋放相機內區域影像記憶體區塊
{
	m_SliceImageW=0;//相機內區域的影像寬度
	m_SliceImageH=0;//相機內區域的影像高度
	m_SliceImageStep=0;//相機內區域的影像步長
	m_SliceImageBitCount=8;//相機內區域的影像位元數	
	m_SliceImagePtr = NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOISlice::SetSliceImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool Clone)//設定區塊圖的影像資料
{
	CAOISlice::ClearSliceImageBuffer();
	if ( true == Clone )
	{
		const char fnName[] = "CAOISlice::SetSliceImageBuffer";
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, m_SliceImagePtr, fnName, "m_SliceImagePtr") == false )
		{	return false; }
		::memcpy(m_SliceImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	}
	else
	{	m_SliceImagePtr = ImagePtr; }
	m_SliceImageW = ImageW;//相機內區域的影像寬度
	m_SliceImageH = ImageH;//相機內區域的影像高度
	m_SliceImageStep = ImageStep;//相機內區域的影像步長
	m_SliceImageBitCount = BitCount;//相機內區域的影像位元數	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOISlice::GetSliceImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr)//取得區塊圖的影像資料
{
	ImageW = m_SliceImageW;//相機內區域的影像寬度
	ImageH = m_SliceImageH;//相機內區域的影像高度
	ImageStep = m_SliceImageStep;//相機內區域的影像步長
	BitCount = m_SliceImageBitCount;//相機內區域的影像位元數
	ImagePtr = m_SliceImagePtr;//相機內區域的影像指標
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOISlice::CloneSliceImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr)//取得區塊圖的影像資料
{
	JetMemory.free_func(ImagePtr);
	if ( NULL == m_SliceImagePtr ) { return false; }
	const char fnName[] = "CAOISlice::CloneSliceImageBuffer";
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_SliceImageStep, m_SliceImageH);
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
	{	return false; }
	::memcpy(ImagePtr, m_SliceImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	ImageW = m_SliceImageW;//相機內區域的影像寬度
	ImageH = m_SliceImageH;//相機內區域的影像高度
	ImageStep = m_SliceImageStep;//相機內區域的影像步長
	BitCount = m_SliceImageBitCount;//相機內區域的影像位元數	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOISlice::CheckSliceCameaGrabbed()//確認畫面的相機有取到影像
{
	const unsigned int CameraRingBufferIndex = (unsigned int)(CameraCtrl.GetCameraRingBufferCurrentIndex(this->m_SliceCameraID));
//	if ( -1 == CameraRingBufferIndex ) { return false; }
	if ( m_SliceCameraFrameIdx >= CameraRingBufferIndex ) { return false; }
//	if ( 0 == m_SliceCameraFrameIdx )
//	{	::Sleep(100);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOISlice::ExecSliceFill()//填滿畫面影像
{
	IMAGE_PTR  CameraImagePtr = NULL;
	IMAGE_SIZE CameraImageW = 0;
	IMAGE_SIZE CameraImageH = 0;
	IMAGE_SIZE CameraImageStep = 0;
	IMAGE_SIZE CameraImageBitCount = 8;	
	if ( CameraCtrl.GetCameraRingBufferImage(m_SliceCameraID, m_SliceCameraFrameIdx, CameraImageW, CameraImageH, CameraImageStep, CameraImagePtr) == false )
	{	return false; }
	
	const IMAGE_SIZE ImageW = m_SliceCameraFrameRect.right-m_SliceCameraFrameRect.left;
	const IMAGE_SIZE ImageH = m_SliceCameraFrameRect.bottom-m_SliceCameraFrameRect.top;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, CameraImageBitCount, 4);
	CAOISlice::ClearSliceImageBuffer();
	if ( 8 == CameraImageBitCount )
	{		
		if ( ImageW!=CameraImageW || ImageH!=CameraImageH || ImageStep!=CameraImageStep )
		{
			if ( ImageAPI.ExtractGrayRoiImage(CameraImageW, CameraImageH, CameraImageStep, CameraImagePtr, m_SliceCameraFrameRect, ImageStep, m_SliceImagePtr, false) == false )
			{	return false; }
		}
		else
		{
			if ( ImageAPI.CloneGrayImage(CameraImageW, CameraImageH, CameraImageStep, CameraImagePtr, m_SliceImagePtr, false) == false )
			{	return false; }
		}
	}
	else if ( 24 == CameraImageBitCount )
	{
		if ( ImageW!=CameraImageW || ImageH!=CameraImageH || ImageStep!=CameraImageStep )
		{
			if ( ImageAPI.ExtractColorRoiImage(CameraImageW, CameraImageH, CameraImageStep, CameraImagePtr, m_SliceCameraFrameRect, ImageStep, m_SliceImagePtr, false) == false )
			{	return false; }
		}
		else
		{
			if ( ImageAPI.CloneColorImage(CameraImageW, CameraImageH, CameraImageStep, CameraImagePtr, m_SliceImagePtr, false) == false )
			{	return false; }
		}
	}
	m_SliceImageW=ImageW;//相機內區域的影像寬度
	m_SliceImageH=ImageH;//相機內區域的影像高度
	m_SliceImageStep=ImageStep;//相機內區域的影像步長
	m_SliceImageBitCount=CameraImageBitCount;//相機內區域的影像位元數	

	if ( ExecSliceGainImage() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOISlice::ExecSliceGainImage()//增益畫面影像
{
	if ( NULL == m_SliceImagePtr ) { return false; }
	if ( ::fabs(m_SliceGainValue-1.0) < 0.001 ) { return true; }

	size_t        i=0;
	int           val=0, val2=0;
	const size_t  szTable=256;
	unsigned char Table[szTable];
	const size_t  BufferSize = m_SliceImageStep*m_SliceImageH;

	val=0;
	for ( i=0; i<szTable; i++ )
	{
		val2 = (int)((val*m_SliceGainValue)+0.5);
		if ( val2 > 255 ) 
		{	Table[i] = 255;	}
		else if ( val2 < 0 )
		{	Table[i] = 0;	}
		else
		{	Table[i] = (unsigned char)(val2); }
		val ++;
	}

	for ( i=0; i<BufferSize; i++ )
	{	m_SliceImagePtr[i] = Table[m_SliceImagePtr[i]];	}
	return true;
}
//-------------------------------------------------------------------------------------//
