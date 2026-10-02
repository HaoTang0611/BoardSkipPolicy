// AOIFrame.cpp: implementation of the CAOIFrame class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIFrame.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CRITICAL_SECTION CAOIFrame::m_csFrame;//同步機制-關鍵區間
size_t CAOIFrame::m_FrameShareMaskSize=0;//Frame共享遮罩大小
size_t CAOIFrame::m_FrameShareImageSize=0;//Frame共享影像大小
size_t CAOIFrame::m_FrameShareSpaceSize=0;//Frame共享空間大小
MASK_PTR CAOIFrame::m_FrameShareMaskPtr=NULL;//Frame空間遮罩
IMAGE_PTR CAOIFrame::m_FrameShareImagePtr=NULL;//Frame影像記憶體區塊
SPACE_PTR CAOIFrame::m_FrameShareSpacePtr=NULL;//Frame空間記憶體區塊
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIFrame, CAOIObj)
//-------------------------------------------------------------------------------------//
void CAOIFrame::InitialFrameLock()//初始化影像的關鍵區間
{
	::InitializeCriticalSection(&m_csFrame);
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::DeleteFrameLock()//刪除影像的關鍵區間
{
	::DeleteCriticalSection(&m_csFrame);
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::LockFrame()//進入影像的關鍵區間
{
	::EnterCriticalSection(&m_csFrame);
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::UnlockFrame()//離開影像的關鍵區間
{
	::LeaveCriticalSection(&m_csFrame);
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ReleaseFrameShareBuffer()//釋放影像的共享記憶體
{
	if ( NULL != m_FrameShareMaskPtr )
	{	JetMemory.free_func(m_FrameShareMaskPtr); }
	if ( NULL != m_FrameShareImagePtr )
	{	JetMemory.free_func(m_FrameShareImagePtr); }
	if ( NULL != m_FrameShareSpacePtr )
	{	JetMemory.free_func(m_FrameShareSpacePtr); }
	m_FrameShareMaskSize = 0;
	m_FrameShareImageSize = 0;
	m_FrameShareSpaceSize = 0;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::AllocateFrameShareBuffer()//建立影像的共享記憶體
{
	size_t MaxSize=0;	
	std::vector<CAMERA_ID> CameraIDList;
	CAMERA_ID MaxCameraID=PRIMARY_CAMERA_ID;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageWH=0;	
	AOIDataCollect.BuildCameraIDList(CameraIDList);
	const size_t CameraIDCount=CameraIDList.size();
	for ( int i=0; i<CameraIDCount; i++ )
	{
		CAMERA_ID CameraID=CameraIDList[i];
		ImageW=AOIDataCollect.GetCameraImageW(CameraID);
		ImageH=AOIDataCollect.GetCameraImageH(CameraID);
		ImageWH=ImageW*ImageH;
		if ( 0==i || MaxSize<ImageWH )
		{	
			MaxSize = ImageWH;
			MaxCameraID = CameraID;
		}
	}
	const int nAlign = 4;	
	ImageW=AOIDataCollect.GetCameraImageW(MaxCameraID);
	ImageH=AOIDataCollect.GetCameraImageH(MaxCameraID);
	IMAGE_SIZE GrayBit=8;
	IMAGE_SIZE ColorBit=24;
	IMAGE_SIZE GrayStep=JetAPI::GetBMPImagePixelsPerLine(ImageW, GrayBit, nAlign);
	IMAGE_SIZE ColorStep=JetAPI::GetBMPImagePixelsPerLine(ImageW, ColorBit, nAlign);
	const size_t GraySize=ImageAPI.CalcBufferSize(GrayStep, ImageH);
	const size_t ColorSize=ImageAPI.CalcBufferSize(ColorStep, ImageH);
	if ( AllocateFrameShareBuffer(ColorSize, GraySize) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::AllocateFrameShareBuffer(size_t szImage, size_t szSpace)//建立影像的共享記憶體
{
	const char fnName[]="CAOIFrame::AllocateFrameShareBuffer";
	ReleaseFrameShareBuffer();	
	
	MASK_PTR  ShareMaskPtr = NULL;
	SPACE_PTR ShareSpacePtr = NULL;
	if ( JetMemory.alloc_func(szSpace, ShareMaskPtr, fnName, "ShareMaskPtr") == false )
	{	return false;	}

	if ( JetMemory.alloc_func(szSpace, ShareSpacePtr, fnName, "ShareSpacePtr") == false )
	{
		JetMemory.free_func(ShareMaskPtr);
		return false; 
	}	
	//::memset(ShareMaskPtr, 0x00, sizeof(MASK_DATA)*szSpace);
	::memset(ShareMaskPtr, PHASE_MASK_NOISE_ONLY, sizeof(MASK_DATA)*szSpace);
	::memset(ShareSpacePtr, 0x00, sizeof(SPACE_DATA)*szSpace);
	
	IMAGE_PTR ShareImagePtr = NULL;	
	if ( JetMemory.alloc_func(szImage, ShareImagePtr, fnName, "ShareImagePtr") == false )
	{	
		JetMemory.free_func(ShareMaskPtr);
		JetMemory.free_func(ShareSpacePtr);
		return false; 
	}
	::memset(ShareImagePtr, 0x00, sizeof(IMAGE_DATA)*szImage);

	m_FrameShareMaskSize = szSpace;
	m_FrameShareImageSize = szImage;
	m_FrameShareSpaceSize = szSpace;
	m_FrameShareMaskPtr = ShareMaskPtr;
	m_FrameShareSpacePtr = ShareSpacePtr;
	m_FrameShareImagePtr = ShareImagePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIFrame::CAOIFrame():CAOIObj(AOI_OBJ_FRAME)
{
	PreInitFrame();
	InitialFrame();
}
//-------------------------------------------------------------------------------------//
CAOIFrame::CAOIFrame(const CAOIFrame &frame):CAOIObj(frame)
{
	PreInitFrame();
	CloneFrame(frame);
}
//-------------------------------------------------------------------------------------//
CAOIFrame::~CAOIFrame()
{
	ClearFrameBuffer();	
}
//-------------------------------------------------------------------------------------//
CAOIFrame& CAOIFrame::operator=(const CAOIFrame &frame)
{
	if ( this == &frame ) { return *this; }
	CAOIObj::operator=(frame);
	CloneFrame(frame);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::PreInitFrame()
{	
	m_FrameMaskPtr = NULL;
	m_FrameImagePtr = NULL;
	m_FrameSpacePtr = NULL;
	m_FrameImageStep = 0;
	m_FrameImageBitCount = 0;
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::InitialFrame()
{
	m_FrameType = FRAME_NULL;//Frame影像樣式	
	m_FrameIndex = -1;//Frame引數編號
	m_FrameUniqueID = -1;//Frame唯一碼
	m_FrameHeightID = 0;//Frame高度編號
	m_FrameIndexOffline = -1;//Frame引數編號-Offline
	m_FrameSliceFuncMode = SLICE_FUNC_2D_IMAGE_GRAY;

	m_FrameFovIdx = -1;//Frame所屬的FOV引數編號
	m_FrameFovPtr = NULL;//Frame所屬的視野指標

	m_FrameFieldIdx = -1;//Frame所屬的區塊引數編號
	m_FrameFieldPtr = NULL;//Frame所屬的區塊指標

	m_FrameCameraID = PRIMARY_CAMERA_ID;//Frame的相機編號
	m_FrameLightMode = DEFAULT_LIGHT_MODE;//Frame的燈源模式		
	m_FrameDistrictID = DISTRICT_ID_A;//Frame的多段編號
	m_FrameRgnCount = 0;//Frame的區域數量
	m_FrameLinkPointer = false;//連結指標, 如果是的話不要刪除

	m_FrameTempInt = 0;//Frame暫存編號
	m_FrameFileName = _T("");//Frame的檔案名稱
	m_FrameFileFolder = _T("");//Frame的檔案資料夾

	m_FrameFileNameOffline = _T("");//Frame的檔案名稱-Offline
	m_FrameFileFolderOffline = _T("");//Frame的檔案資料夾-Offline

	m_FrameMergeTime = 0.0;//Frame合併花費時間
	m_FrameCalcState = FRAME_CALC_NONE;//影像計算狀態
	m_FrameMergeState = FRAME_MERGE_NONE;	
	m_FrameImageSaved = false;//Frame影像是否存檔過了
	m_FrameResolutionX = 1.0;//Frame影像解析度-X
	m_FrameResolutionY = 1.0;//Frame影像解析度-Y

	m_FrameSaturationRed = 1.0;//Frame飽和度調整-紅色
	m_FrameSaturationGreen = 1.0;//Frame飽和度調整-綠色
	m_FrameSaturationBlue = 1.0;//Frame飽和度調整-藍色

	m_FrameUseShareMaskBuf = false;
	m_FrameUseShareImageBuf = false;
	m_FrameUseShareSpaceBuf = false;

	m_FrameImageValid = false;	
	m_FrameImageIndex = -1;
	m_FrameImageW = 0; //Frame影像寬度
	m_FrameImageH = 0; //Frame影像長度
	m_FrameBayerPattern = BAYER_PATTERN_NONE;
	m_FraemSpaceOffset = 0.0;
	ClearFrameBuffer();	

	m_FrameSlicePtrList.clear();		
	m_FrameSaveRawImageDone = false;
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::ClearFrameBuffer()
{	
	//Frame影像記憶體區塊
	if ( true == GetFrameUseShareMaskBuf() )
	{	m_FrameMaskPtr = NULL;	}
	else
	{	JetMemory.free_func(m_FrameMaskPtr);	}

	if ( true == GetFrameUseShareImageBuf() )
	{	m_FrameImagePtr = NULL; }
	else
	{	JetMemory.free_func(this->m_FrameImagePtr);	}

	if ( true == GetFrameUseShareSpaceBuf() )
	{	m_FrameSpacePtr = NULL;		}
	else
	{	JetMemory.free_func(this->m_FrameSpacePtr);	}
		
	SetFrameImageValid(false);	
	SetFrameUseShareMaskBuf(false);
	SetFrameUseShareImageBuf(false);
	SetFrameUseShareSpaceBuf(false);
	this->m_FrameImageW = 0;//Frame影像寬度
	this->m_FrameImageH = 0;//Frame影像長度	
	this->m_FrameImageStep = 0;//Frame影像每條的位元組數量
	this->m_FrameImageBitCount = 0;	
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::CloneFrame(const CAOIFrame &frame)
{
	m_FrameType = frame.m_FrameType;
	m_FrameIndex = frame.m_FrameIndex;	
	m_FrameUniqueID = frame.m_FrameUniqueID;	
	m_FrameHeightID = frame.m_FrameHeightID;		
	m_FrameIndexOffline = frame.m_FrameIndexOffline;//Frame引數編號-Offline
	m_FrameSliceFuncMode = frame.m_FrameSliceFuncMode;

	m_FrameFovIdx = frame.m_FrameFovIdx;//Frame所屬的FOV引數編號
	m_FrameFovPtr = frame.m_FrameFovPtr;//Frame所屬的視野指標

	m_FrameFieldIdx = frame.m_FrameFieldIdx;//Frame所屬的區塊引數編號
	m_FrameFieldPtr = frame.m_FrameFieldPtr;//Frame所屬的區塊指標

	m_FrameLinkPointer = frame.m_FrameLinkPointer;//連結指標, 如果是的話不要刪除

	m_FrameCameraID = frame.m_FrameCameraID;//Frame的相機編號
	m_FrameLightMode = frame.m_FrameLightMode;//Frame的燈源模式
	m_FrameDistrictID = frame.m_FrameDistrictID;//Frame的多段編號	
	m_FrameRgnCount = frame.m_FrameRgnCount;//Frame的區域數量
	m_FrameTempInt = frame.m_FrameTempInt;//Frame暫存編號
	m_FrameFileName = frame.m_FrameFileName;//Frame的檔案名稱
	m_FrameFileFolder = frame.m_FrameFileFolder;//Frame的檔案資料夾
	m_FrameFileNameOffline = frame.m_FrameFileNameOffline;//Frame的檔案名稱-Offline
	m_FrameFileFolderOffline = frame.m_FrameFileFolderOffline;//Frame的檔案資料夾-Offline

	m_FrameMergeTime = frame.m_FrameMergeTime;//Frame合併花費時間
	m_FrameImageSaved = frame.m_FrameImageSaved;//Frame影像是否存檔過了
	m_FrameResolutionX = frame.m_FrameResolutionX;//Frame影像解析度-X
	m_FrameResolutionY = frame.m_FrameResolutionY;//Frame影像解析度-Y	
	m_FrameSaturationRed = frame.m_FrameSaturationRed;//Frame飽和度調整-紅色
	m_FrameSaturationGreen = frame.m_FrameSaturationGreen;//Frame飽和度調整-綠色
	m_FrameSaturationBlue = frame.m_FrameSaturationBlue;//Frame飽和度調整-藍色

	m_FrameSlicePtrList = frame.m_FrameSlicePtrList;		

	m_FrameUseShareMaskBuf = frame.m_FrameUseShareMaskBuf;
	m_FrameUseShareImageBuf = frame.m_FrameUseShareImageBuf;
	m_FrameUseShareSpaceBuf = frame.m_FrameUseShareSpaceBuf;	

	m_FrameImageValid = frame.m_FrameImageValid;
	m_FrameImageIndex = frame.m_FrameImageIndex;
	m_FrameImageW = frame.m_FrameImageW;
	m_FrameImageH = frame.m_FrameImageH;
	m_FrameBayerPattern = frame.m_FrameBayerPattern;
	m_FraemSpaceOffset = frame.m_FraemSpaceOffset;	
	CloneFrameBuffer(frame);
	
	m_FrameMergeState = frame.m_FrameMergeState;//影像合併步驟	
	m_FrameCalcState = frame.m_FrameCalcState;//影像計算狀態

	m_FrameSaveRawImageDone = frame.m_FrameSaveRawImageDone;
	return;
}
//-------------------------------------------------------------------------------------//
CAOIFrame* CAOIFrame::CloneFrameObj() const//建立且複製一個影像
{
	CAOIFrame *ObjPtr = AOIObjManager.CreateFrameObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::CloneFrameBuffer(const CAOIFrame &frame)
{
	CAOIFrame::ClearFrameBuffer();
	if ( 0 == frame.m_FrameImageStep ) { return true; }

	const char fnName[] = ("CAOIFrame::CloneFrameBuffer");
	const size_t BufferSize = ImageAPI.CalcBufferSize(frame.m_FrameImageStep, frame.m_FrameImageH);
	if ( NULL != frame.m_FrameImagePtr )
	{
		if ( JetMemory.alloc_func(BufferSize, m_FrameImagePtr, fnName, "m_FrameImagePtr") == false )
		{	return false; }
		::memcpy(m_FrameImagePtr, frame.m_FrameImagePtr, sizeof(IMAGE_DATA)*BufferSize);		
	}
	if ( NULL != frame.m_FrameSpacePtr )
	{
		if ( JetMemory.alloc_func(BufferSize, m_FrameSpacePtr, fnName, "m_FrameSpacePtr") == false )
		{	return false; }
		::memcpy(m_FrameSpacePtr, frame.m_FrameSpacePtr, sizeof(SPACE_DATA)*BufferSize);		
	}
	
	if ( NULL != frame.m_FrameMaskPtr )
	{
		if ( JetMemory.alloc_func(BufferSize, m_FrameMaskPtr, fnName, "m_FrameMaskPtr") == false )
		{	return false; }
		::memcpy(m_FrameMaskPtr, frame.m_FrameMaskPtr, sizeof(MASK_DATA)*BufferSize);
	}
	this->m_FrameImageW = frame.m_FrameImageW;
	this->m_FrameImageH = frame.m_FrameImageH;
	this->m_FrameImageStep = frame.m_FrameImageStep;	
	this->m_FrameImageBitCount = frame.m_FrameImageBitCount;
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::ResetFrameRgnCount()
{
	CAOIFrame::m_FrameRgnCount = 0;
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::IncreaseFrameRgnCount()
{
	CAOIFrame::m_FrameRgnCount ++;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::GetFrameUniFrame(TUNI_FRAME &UniFrame) const
{
	UniFrame.ImageW = m_FrameImageW;
	UniFrame.ImageH = m_FrameImageH;
	UniFrame.ImageStep = m_FrameImageStep;
	UniFrame.BitCount = m_FrameImageBitCount;
	UniFrame.ImagePtr = m_FrameImagePtr;
	UniFrame.MaskPtr = m_FrameMaskPtr;
	UniFrame.SpacePtr = m_FrameSpacePtr;
	UniFrame.PhasePtr = NULL;
	UniFrame.FrameUniqueID = FRAME_UNIQUE_ID_NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::SetFrameImagePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool bClone)
{
	CAOIFrame::ClearFrameBuffer();
	if ( NULL == ImagePtr ) { return false; }

	IMAGE_PTR NewImagePtr = NULL;
	if ( false == bClone )
	{	NewImagePtr = ImagePtr;	}
	else
	{
		const char fnName[] = "CAOIFrame::SetFrameImagePtr";
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, NewImagePtr, fnName, "NewImagePtr") == false )
		{	return false;	}
		::memcpy(NewImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
	}
	m_FrameImageW = ImageW;
	m_FrameImageH = ImageH;
	m_FrameImageStep = ImageStep;
	m_FrameImageBitCount = BitCount;
	m_FrameImagePtr = NewImagePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::GetFrameImagePtr(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr) const	
{
	ImageW = m_FrameImageW;
	ImageH = m_FrameImageH;
	ImageStep = m_FrameImageStep;
	BitCount = m_FrameImageBitCount;;
	ImagePtr = m_FrameImagePtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::SetFrameSpacePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, bool bClone)
{
	CAOIFrame::ClearFrameBuffer();
	if ( NULL==MaskPtr || NULL==SpacePtr ) { return false; }
	MASK_PTR  NewMaskPtr = NULL;
	SPACE_PTR NewSpacePtr = NULL;
	if ( false == bClone )
	{	
		NewMaskPtr = MaskPtr;	
		NewSpacePtr = SpacePtr;	
	}
	else
	{
		const char fnName[] = "CAOIFrame::SetFrameSpacePtr";
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, NewMaskPtr, fnName, "NewMaskPtr")==false ||
			 JetMemory.alloc_func(BufferSize, NewSpacePtr, fnName, "NewSpacePtr")==false )
		{	return false;	}
		::memcpy(NewMaskPtr, MaskPtr, sizeof(MASK_DATA)*BufferSize);
		::memcpy(NewSpacePtr, SpacePtr, sizeof(SPACE_DATA)*BufferSize);
	}
	CAOIFrame::m_FrameImageW = ImageW;
	CAOIFrame::m_FrameImageH = ImageH;
	CAOIFrame::m_FrameImageStep = ImageStep;
	CAOIFrame::m_FrameImageBitCount = BitCount;
	CAOIFrame::m_FrameMaskPtr = NewMaskPtr;
	CAOIFrame::m_FrameSpacePtr = NewSpacePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::SetFrameSpacePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, bool bClone)
{
	CAOIFrame::ClearFrameBuffer();
	if ( NULL==ImagePtr || NULL==MaskPtr || NULL==SpacePtr ) { return false; }

	MASK_PTR  NewMaskPtr = NULL;
	IMAGE_PTR NewImaegPtr = NULL;
	SPACE_PTR NewSpacePtr = NULL;
	if ( false == bClone )
	{	
		NewMaskPtr = MaskPtr;	
		NewImaegPtr = ImagePtr;
		NewSpacePtr = SpacePtr;	
	}
	else
	{
		const char fnName[] = "CAOIFrame::SetFrameSpacePtr";
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, NewMaskPtr, fnName, "NewMaskPtr")==false ||
			 JetMemory.alloc_func(BufferSize, NewImaegPtr, fnName, "NewImaegPtr")==false ||
			 JetMemory.alloc_func(BufferSize, NewSpacePtr, fnName, "NewSpacePtr")==false )
		{	return false;	}
		::memcpy(NewMaskPtr, MaskPtr, sizeof(MASK_DATA)*BufferSize);
		::memcpy(NewImaegPtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);		
		::memcpy(NewSpacePtr, SpacePtr, sizeof(SPACE_DATA)*BufferSize);
	}

	RECT rcRoi;
	rcRoi.left = 0;
	rcRoi.top = 0;
	rcRoi.right = ImageW;
	rcRoi.bottom = ImageH;
	if ( NULL != NewMaskPtr )
	{
		double Ave=0.0;
		ImageAPI.CalcGrayImageAverage(ImageW, ImageH, ImageStep, NewMaskPtr, rcRoi, Ave);
		if ( Ave > 10 )
		{	Ave = Ave; }
	}


	CAOIFrame::m_FrameImageW = ImageW;
	CAOIFrame::m_FrameImageH = ImageH;
	CAOIFrame::m_FrameImageStep = ImageStep;
	CAOIFrame::m_FrameImageBitCount = BitCount;	
	CAOIFrame::m_FrameMaskPtr = NewMaskPtr;
	CAOIFrame::m_FrameImagePtr = NewImaegPtr;
	CAOIFrame::m_FrameSpacePtr = NewSpacePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::GetFrameSpacePtr(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr) const
{
	ImageW = CAOIFrame::m_FrameImageW;
	ImageH = CAOIFrame::m_FrameImageH;
	ImageStep = CAOIFrame::m_FrameImageStep;
	BitCount = CAOIFrame::m_FrameImageBitCount;
	MaskPtr = CAOIFrame::m_FrameMaskPtr;
	SpacePtr = CAOIFrame::m_FrameSpacePtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ReleaseCastParam(TCastParam &CastParam)//釋放投光參數
{
	if ( NULL != CastParam.PtrMask )
	{	JetMemory.free_func(CastParam.PtrMask); }
	if ( NULL != CastParam.PtrSpace )
	{	JetMemory.free_func(CastParam.PtrSpace); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::BuildCastParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, int ImgPtrCount, int SliceFuncMode, IMAGE_PTR ImgPtr[], LIGHT_3D_CAST_ID CastID, TCastParam &CastParam)//建立投光參數
{
	int PhaseMode = 0;
	const char fnName[] = "CAOIFrame::BuildCastParam";
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();	
	const int DLPExposureTime_us = CaliParam.m_DLPExposureTime_us;
	const int DLPExposureTime2_us = CaliParam.m_DLPExposureTime2_us;

	CastParam.ExpCount = 1;
	CastParam.PerA = SysParam.m_PhasePeriod1;
	CastParam.PerB = SysParam.m_PhasePeriod2;
	
	CastParam.ImageW = ImageW;
	CastParam.ImageH = ImageH;
	CastParam.ImageStep = ImageStep;
	CastParam.ImageCount = ImgPtrCount;

	CastParam.CastID = CastID;
	CastParam.ExpTimeA = DLPExposureTime_us;
	CastParam.ExpTimeB = DLPExposureTime2_us;	
	CastParam.HeightBuildMode = SysParam.m_PhaseConvertHeightMode;

	switch ( ImgPtrCount )
	{
	case 3:			
		CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];
		PhaseMode = LIGHT3D_PHASE_3_3_1;
		CastParam.DecodeMode = DECODE_PHASE_3STEP_1;
		break;
	case 4:
		CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
		PhaseMode = LIGHT3D_PHASE_4_4_1;
		CastParam.DecodeMode = DECODE_PHASE_4STEP_1;
		break;
	case 5:
		if ( SLICE_FUNC_3D_2STEP_2STEP_1EXP == SliceFuncMode )
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];
			CastParam.PtrB1 = ImgPtr[2];	CastParam.PtrB2 = ImgPtr[3];	CastParam.PtrB3 = ImgPtr[4];//Average
			PhaseMode = LIGHT3D_PHASE_2_2_M;
			CastParam.DecodeMode = DECODE_PHASE_2_2STEP_2;
		}
		else
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];	CastParam.PtrA5 = ImgPtr[4];
			PhaseMode = LIGHT3D_PHASE_5_5_1;
			CastParam.DecodeMode = DECODE_PHASE_5STEP_1;
		}
		break;
	case 6:
		if ( SLICE_FUNC_3D_4STEP_2STEP_1EXP == SliceFuncMode)
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
			CastParam.PtrB1 = ImgPtr[4];	CastParam.PtrB2 = ImgPtr[5];		
			PhaseMode = LIGHT3D_PHASE_4_2_M;
			CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
		}
		else
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];
			CastParam.PtrB1 = ImgPtr[3];	CastParam.PtrB2 = ImgPtr[4];	CastParam.PtrB3 = ImgPtr[5];		
			PhaseMode = LIGHT3D_PHASE_3_3_M;
			CastParam.DecodeMode = DECODE_PHASE_3_3STEP_2;
		}			
		break;
	case 8:
		CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
		CastParam.PtrB1 = ImgPtr[4];	CastParam.PtrB2 = ImgPtr[5];	CastParam.PtrB3 = ImgPtr[6];	CastParam.PtrB4 = ImgPtr[7];
		if ( SLICE_FUNC_3D_4STEP_4GC_1EXP == SliceFuncMode)
		{
			PhaseMode = LIGHT3D_PHASE_4_4GC_M;
			CastParam.DecodeMode = DECODE_PHASE_4STEP_4GC_2;	
		}
		else
		{
			PhaseMode = LIGHT3D_PHASE_4_4_M;
			CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;
		}
		break;
	case 9:
		CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
		CastParam.PtrB1 = ImgPtr[4];	CastParam.PtrB2 = ImgPtr[5];	CastParam.PtrB3 = ImgPtr[6];	CastParam.PtrB4 = ImgPtr[7];	CastParam.PtrB5 = ImgPtr[8];
		PhaseMode = LIGHT3D_PHASE_4_5GC_M;
		CastParam.DecodeMode = DECODE_PHASE_4STEP_5GC_2;		
		break;
	case 10:
		if ( SLICE_FUNC_3D_4STEP_6GC_1EXP == SliceFuncMode)
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
			CastParam.PtrB1 = ImgPtr[4];	CastParam.PtrB2 = ImgPtr[5];	CastParam.PtrB3 = ImgPtr[6];	CastParam.PtrB4 = ImgPtr[7];	CastParam.PtrB5 = ImgPtr[8];	CastParam.PtrB6 = ImgPtr[9];
			PhaseMode = LIGHT3D_PHASE_4_6GC_M;
			CastParam.DecodeMode = DECODE_PHASE_4STEP_6GC_2;	
		}
		else
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];	CastParam.PtrA5 = ImgPtr[4];
			CastParam.PtrB1 = ImgPtr[5];	CastParam.PtrB2 = ImgPtr[6];	CastParam.PtrB3 = ImgPtr[7];	CastParam.PtrB4 = ImgPtr[8];	CastParam.PtrB5 = ImgPtr[9];
			PhaseMode = LIGHT3D_PHASE_5_5_M;
			CastParam.DecodeMode = DECODE_PHASE_5_5STEP_2;
		}
		break;
	default:
		return false;
		break;
	}

	TPhaseNoiseParam NoiseParam;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);		
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);	
	const int   nSmoothFilterSize = NoiseParam.PhaseSmoothFilter;
	const BOOL  bCheckSmoothFilter = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER);
	if ( FALSE!=bCheckSmoothFilter && nSmoothFilterSize>0 )
	{
		IMAGE_PTR    SrcPtr = NULL;
		IMAGE_PTR    BufferPtr = NULL;		
		const int    KerSize = nSmoothFilterSize*2+1;
		switch ( PhaseMode )
		{
		case LIGHT3D_PHASE_2_2_M:
			break;
		case LIGHT3D_PHASE_4_4_M:
			if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
			{
				SrcPtr = CastParam.PtrB1;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrB2;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrB3;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrB4;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrB5;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				JetMemory.free_func(BufferPtr);
			}
			break;
		}
	}
	
	PHASE_PTR BasePhasePtr = NULL;
	IMAGE_SIZE PhaseW=0, PhaseH=0, PhaseStep=0;
	const int DLPLEDColor = AOIDataCollect.GetSystemDlpLedColor();
	if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, PhaseW, PhaseH, PhaseStep, BasePhasePtr) == false )
	{	return false;	}
	if ( ImageW!=PhaseW || ImageH!=PhaseH || ImageStep!=PhaseStep )
	{	return false; }

	IMAGE_SIZE SpaceW=0, SpaceH=0, SpaceStep=0;
	SPACE_PTR KPtr = NULL;		
	if ( Light3DCtrl.GetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, SpaceW, SpaceH, SpaceStep, KPtr) == false )
	{	return false;	}
	if ( ImageW!=SpaceW || ImageH!=SpaceH || ImageStep!=SpaceStep )
	{	return false; }

	CastParam.ZeroPhasePtr = BasePhasePtr;
	CastParam.HeightFactorPtr = KPtr;

	if ( JetMemory.alloc_func(BufferSize, CastParam.PtrMask, fnName, "CastParam.PtrMask") == false || 
		 JetMemory.alloc_func(BufferSize, CastParam.PtrSpace, fnName, "CastParam.PtrSpace") == false )
	{
		JetMemory.free_func(CastParam.PtrMask);
		JetMemory.free_func(CastParam.PtrSpace);
		return false;
	}

	double Gamma = 1.0;
	if ( Light3DCtrl.GetLight3DImageGamma(CastID, Gamma) == false )
	{
		JetMemory.free_func(CastParam.PtrMask);
		JetMemory.free_func(CastParam.PtrSpace);
		return false;	
	}
	CastParam.Gamma = Gamma;

	//高度係數
	if ( Light3DCtrl.GetLight3DHeightFactor(CastID, CastParam.HeightFactor0, CastParam.HeightFactor1, CastParam.HeightFactor2) == false )
	{
		JetMemory.free_func(CastParam.PtrMask);
		JetMemory.free_func(CastParam.PtrSpace);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::BuildCastParam2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, int ImgPtrCount, int SliceFuncMode, IMAGE_PTR ImgPtr[], LIGHT_3D_CAST_ID CastID, TCastParam &CastParam)//建立投光參數
{
	int PhaseMode = 0;
	const char fnName[] = "CAOIFrame::BuildCastParam2Exp";
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	
	const TCalibrationParameter &CaliParam = AOIDataCollect.GetCalibrationParameter();	
	const int DLPExposureTime_us = CaliParam.m_DLPExposureTime_us;
	const int DLPExposureTime2_us = CaliParam.m_DLPExposureTime2_us;

	CastParam.ExpCount = 2;
	CastParam.PerA = SysParam.m_PhasePeriod1;
	CastParam.PerB = SysParam.m_PhasePeriod2;
	
	CastParam.ImageW = ImageW;
	CastParam.ImageH = ImageH;
	CastParam.ImageStep = ImageStep;
	CastParam.ImageCount = ImgPtrCount;

	CastParam.CastID = CastID;
	CastParam.ExpTimeA = DLPExposureTime_us;
	CastParam.ExpTimeB = DLPExposureTime2_us;	
	CastParam.ExpTimeC = DLPExposureTime_us;
	CastParam.ExpTimeD = DLPExposureTime2_us;	
	CastParam.HeightBuildMode = SysParam.m_PhaseConvertHeightMode;

	switch ( ImgPtrCount )
	{
	case 6:			
		CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];
		CastParam.PtrC1 = ImgPtr[3];	CastParam.PtrC2 = ImgPtr[4];	CastParam.PtrC3 = ImgPtr[5];
		PhaseMode = LIGHT3D_PHASE_3_3_1_2;
		CastParam.DecodeMode = DECODE_PHASE_3STEP_1;
		break;
	case 8:
		CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
		CastParam.PtrC1 = ImgPtr[4];	CastParam.PtrC2 = ImgPtr[5];	CastParam.PtrC3 = ImgPtr[6];	CastParam.PtrC4 = ImgPtr[7];
		PhaseMode = LIGHT3D_PHASE_4_4_1_2;
		CastParam.DecodeMode = DECODE_PHASE_4STEP_1;
		break;
	case 10:
		if (SLICE_FUNC_3D_2STEP_2STEP_1EXP == SliceFuncMode)//SLICE_FUNC_3D_2_2STEP_2EXP
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];			
			CastParam.PtrB1 = ImgPtr[2];	CastParam.PtrB2 = ImgPtr[3];	CastParam.PtrB3 = ImgPtr[4];
			CastParam.PtrC1 = ImgPtr[5];	CastParam.PtrC2 = ImgPtr[6];
			CastParam.PtrD1 = ImgPtr[7];	CastParam.PtrD2 = ImgPtr[8];	CastParam.PtrD3 = ImgPtr[9];
			PhaseMode = LIGHT3D_PHASE_2_2_M_2;
			CastParam.DecodeMode = DECODE_PHASE_2_2STEP_2;
		}
		else
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];	CastParam.PtrA5 = ImgPtr[4];
			CastParam.PtrC1 = ImgPtr[5];	CastParam.PtrC2 = ImgPtr[6];	CastParam.PtrC3 = ImgPtr[7];	CastParam.PtrC4 = ImgPtr[8];	CastParam.PtrC5 = ImgPtr[9];
			PhaseMode = LIGHT3D_PHASE_5_5_1_2;
			CastParam.DecodeMode = DECODE_PHASE_5STEP_1;
		}
		break;
	case 12:
		if ( SLICE_FUNC_3D_4STEP_2STEP_2EXP == SliceFuncMode )
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
			CastParam.PtrB1 = ImgPtr[4];	CastParam.PtrB2 = ImgPtr[5];
			CastParam.PtrC1 = ImgPtr[6];	CastParam.PtrC2 = ImgPtr[7];	CastParam.PtrC3 = ImgPtr[8];	CastParam.PtrC4 = ImgPtr[9];
			CastParam.PtrD1 = ImgPtr[10];	CastParam.PtrD2 = ImgPtr[11];			
			PhaseMode = LIGHT3D_PHASE_4_2_M_2;
			CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;		
		}
		else
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];
			CastParam.PtrB1 = ImgPtr[3];	CastParam.PtrB2 = ImgPtr[4];	CastParam.PtrB3 = ImgPtr[5];
			CastParam.PtrC1 = ImgPtr[6];	CastParam.PtrC2 = ImgPtr[7];	CastParam.PtrC3 = ImgPtr[8];
			CastParam.PtrD1 = ImgPtr[9];	CastParam.PtrD2 = ImgPtr[10];	CastParam.PtrD3 = ImgPtr[11];
			PhaseMode = LIGHT3D_PHASE_3_3_M_2;
			CastParam.DecodeMode = DECODE_PHASE_3_3STEP_2;		
		}
		break;
	case 16:
		CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
		CastParam.PtrB1 = ImgPtr[4];	CastParam.PtrB2 = ImgPtr[5];	CastParam.PtrB3 = ImgPtr[6];	CastParam.PtrB4 = ImgPtr[7];
		CastParam.PtrC1 = ImgPtr[8];	CastParam.PtrC2 = ImgPtr[9];	CastParam.PtrC3 = ImgPtr[10];	CastParam.PtrC4 = ImgPtr[11];
		CastParam.PtrD1 = ImgPtr[12];	CastParam.PtrD2 = ImgPtr[13];	CastParam.PtrD3 = ImgPtr[14];	CastParam.PtrD4 = ImgPtr[15];
		if ( SLICE_FUNC_3D_4STEP_4GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_4GC_2LIGHT==SliceFuncMode )
		{
			PhaseMode = LIGHT3D_PHASE_4_4GC_M_2;
			CastParam.DecodeMode = DECODE_PHASE_4STEP_4GC_2;
		}
		else
		{
			PhaseMode = LIGHT3D_PHASE_4_4_M_2;
			CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;	
		}
		break;
	case 18:
		CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
		CastParam.PtrB1 = ImgPtr[4];	CastParam.PtrB2 = ImgPtr[5];	CastParam.PtrB3 = ImgPtr[6];	CastParam.PtrB4 = ImgPtr[7];	CastParam.PtrB5 = ImgPtr[8];
		CastParam.PtrC1 = ImgPtr[9];	CastParam.PtrC2 = ImgPtr[10];	CastParam.PtrC3 = ImgPtr[11];	CastParam.PtrC4 = ImgPtr[12];
		CastParam.PtrD1 = ImgPtr[13];	CastParam.PtrD2 = ImgPtr[14];	CastParam.PtrD3 = ImgPtr[15];	CastParam.PtrD4 = ImgPtr[16];	CastParam.PtrD5 = ImgPtr[17];
		if ( SLICE_FUNC_3D_4STEP_5GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_5GC_2LIGHT==SliceFuncMode )
		{
			PhaseMode = LIGHT3D_PHASE_4_5GC_M_2;
			CastParam.DecodeMode = DECODE_PHASE_4STEP_5GC_2;
		}		
		break;
	case 20:
		if ( SLICE_FUNC_3D_4STEP_6GC_2EXP==SliceFuncMode || SLICE_FUNC_3D_4STEP_6GC_2LIGHT==SliceFuncMode )
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];
			CastParam.PtrB1 = ImgPtr[4];	CastParam.PtrB2 = ImgPtr[5];	CastParam.PtrB3 = ImgPtr[6];	CastParam.PtrB4 = ImgPtr[7];	CastParam.PtrB5 = ImgPtr[8];	CastParam.PtrB6 = ImgPtr[9];
			CastParam.PtrC1 = ImgPtr[10];	CastParam.PtrC2 = ImgPtr[11];	CastParam.PtrC3 = ImgPtr[12];	CastParam.PtrC4 = ImgPtr[13];
			CastParam.PtrD1 = ImgPtr[14];	CastParam.PtrD2 = ImgPtr[15];	CastParam.PtrD3 = ImgPtr[16];	CastParam.PtrD4 = ImgPtr[17];	CastParam.PtrD5 = ImgPtr[18];	CastParam.PtrD6 = ImgPtr[19];
			PhaseMode = LIGHT3D_PHASE_4_6GC_M_2;
			CastParam.DecodeMode = DECODE_PHASE_4STEP_6GC_2;
		}
		else
		{
			CastParam.PtrA1 = ImgPtr[0];	CastParam.PtrA2 = ImgPtr[1];	CastParam.PtrA3 = ImgPtr[2];	CastParam.PtrA4 = ImgPtr[3];	CastParam.PtrA5 = ImgPtr[4];
			CastParam.PtrB1 = ImgPtr[5];	CastParam.PtrB2 = ImgPtr[6];	CastParam.PtrB3 = ImgPtr[7];	CastParam.PtrB4 = ImgPtr[8];	CastParam.PtrB5 = ImgPtr[9];
			CastParam.PtrC1 = ImgPtr[10];	CastParam.PtrC2 = ImgPtr[11];	CastParam.PtrC3 = ImgPtr[12];	CastParam.PtrC4 = ImgPtr[13];	CastParam.PtrC5 = ImgPtr[14];
			CastParam.PtrD1 = ImgPtr[15];	CastParam.PtrD2 = ImgPtr[16];	CastParam.PtrD3 = ImgPtr[17];	CastParam.PtrD4 = ImgPtr[18];	CastParam.PtrD5 = ImgPtr[19];
			PhaseMode = LIGHT3D_PHASE_5_5_M_2;
			CastParam.DecodeMode = DECODE_PHASE_5_5STEP_2;
		}
		break;
	default:
		return false;
		break;
	}

	TPhaseNoiseParam NoiseParam;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);		
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);
	const int   nSmoothFilterSize = NoiseParam.PhaseSmoothFilter;
	const BOOL  bCheckSmoothFilter = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER);
	if ( FALSE!=bCheckSmoothFilter && nSmoothFilterSize>0 )
	{
		IMAGE_PTR    SrcPtr = NULL;
		IMAGE_PTR    BufferPtr = NULL;		
		const int    KerSize = nSmoothFilterSize*2+1;
		switch ( PhaseMode )
		{
		case LIGHT3D_PHASE_2_2_M_2:
			break;
		case LIGHT3D_PHASE_4_2_M_2:
			break;
		case LIGHT3D_PHASE_4_4_M_2:
			if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == true )
			{
				SrcPtr = CastParam.PtrB1;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrB2;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrB3;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrB4;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrB5;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				JetMemory.free_func(BufferPtr);

				SrcPtr = CastParam.PtrD1;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrD2;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrD3;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrD4;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				SrcPtr = CastParam.PtrD5;
				if ( NULL != SrcPtr )
				{
					if ( ImageAPI.SmoothGrayImage3(ImageW, ImageH, ImageStep, SrcPtr, KerSize, BufferPtr) == true )
					{	::memcpy(SrcPtr, BufferPtr, BufferSize);	}
				}
				JetMemory.free_func(BufferPtr);				
			}
			break;
		case LIGHT3D_PHASE_4_4GC_M_2:
			break;
		case LIGHT3D_PHASE_4_5GC_M_2:
			break;
		case LIGHT3D_PHASE_4_6GC_M_2:
			break;
		}
	}
	
	PHASE_PTR BasePhasePtr = NULL;
	IMAGE_SIZE PhaseW=0, PhaseH=0, PhaseStep=0;
	const int DLPLEDColor = AOIDataCollect.GetSystemDlpLedColor();
	if ( Light3DCtrl.GetLight3DPhaseZero(CastID, PhaseMode, DLPLEDColor, PhaseW, PhaseH, PhaseStep, BasePhasePtr) == false )
	{	return false;	}
	if ( ImageW!=PhaseW || ImageH!=PhaseH || ImageStep!=PhaseStep )
	{	return false; }

	IMAGE_SIZE SpaceW=0, SpaceH=0, SpaceStep=0;
	SPACE_PTR KPtr = NULL;		
	if ( Light3DCtrl.GetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, SpaceW, SpaceH, SpaceStep, KPtr) == false )
	{	return false;	}
	if ( ImageW!=SpaceW || ImageH!=SpaceH || ImageStep!=SpaceStep )
	{	return false; }

	CastParam.ZeroPhasePtr = BasePhasePtr;
	CastParam.HeightFactorPtr = KPtr;

	if ( JetMemory.alloc_func(BufferSize, CastParam.PtrMask, fnName, "CastParam.PtrMask") == false || 		 
		 JetMemory.alloc_func(BufferSize, CastParam.PtrSpace, fnName, "CastParam.PtrSpace") == false )
	{
		JetMemory.free_func(CastParam.PtrMask);		
		JetMemory.free_func(CastParam.PtrSpace);		
		return false;
	}

	double Gamma = 1.0;
	if ( Light3DCtrl.GetLight3DImageGamma(CastID, Gamma) == false )
	{
		JetMemory.free_func(CastParam.PtrMask);
		JetMemory.free_func(CastParam.PtrSpace);
		return false;	
	}
	CastParam.Gamma = Gamma;

	//高度係數
	if ( Light3DCtrl.GetLight3DHeightFactor(CastID, CastParam.HeightFactor0, CastParam.HeightFactor1, CastParam.HeightFactor2) == false )
	{
		JetMemory.free_func(CastParam.PtrMask);
		JetMemory.free_func(CastParam.PtrSpace);
		return false;	
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ReSortFrameSliceList()//重新排序Frame影像列表
{
	bool bIsOK = true;
	CAOIFrame::LockFrame();
	bIsOK = ReSortFrameSliceList_Kernel(m_FrameSlicePtrList);
	CAOIFrame::UnlockFrame();	
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::AddFrameSlicePtr(CAOISlice *Ptr)//增加影像的區域指標
{
	if ( NULL != Ptr )
	{	Ptr->SetSliceFrameIdx(GetFrameIndex());	}
	m_FrameSlicePtrList.push_back(Ptr);
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIFrame::GetFrameSliceCount() const //取得影像的區域數量
{
	return m_FrameSlicePtrList.size();
}
//-------------------------------------------------------------------------------------//
CAOISlice* CAOIFrame::GetFrameSlicePtr(size_t index, bool check)//取得影像的區域指標
{
	if ( true == check ) 
	{
		const size_t count = this->m_FrameSlicePtrList.size();
		if ( index >= count ) 
		{	return NULL; }
	}
	return m_FrameSlicePtrList[index];
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::RemoveFrameAllSlices()//移除影像的所有區域
{
	this->m_FrameSlicePtrList.clear();	
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::CheckFrameSliceFilled()//確認影像畫面內的區域都取到影像
{
	size_t       i = 0;	
	CAOISlice   *SlicePtr = NULL;			
	SLICE_FILL_STATE  SliceFillState=SLICE_FILL_NONE;	
	const size_t SliceCount = this->GetFrameSliceCount();
	
	for ( i=0; i<SliceCount; i++ )
	{
		SlicePtr = this->GetFrameSlicePtr(i, false);
		if ( NULL == SlicePtr ) { continue; }
		SliceFillState = SlicePtr->GetSliceFillState();		
		if ( SLICE_FILL_DONE != SliceFillState )
		{	return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::CheckFrameFieldFilled()//確認影像畫面內的區域都取到影像
{
	return CheckFrameSliceFilled();
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIFrame::ExecFrameMerge()//影像合併區域影像
{
	return ExecFrameMergeSlice();
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIFrame::ExecFrameLoad()//影像載入圖檔
{	
	CAOIField  *FieldPtr = CAOIFrame::GetFrameFieldPtr();
	if ( CAOIFrame::CheckFrameNeedToLoad() == false ) { return true; }

	size_t i=0;
	bool IsOK = false;
	IsOK = ExecFrameLoadImage();
	SetFrameImageValid(IsOK);
	if ( false == IsOK )
	{	IsOK = AllocateFrameBuffer();	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//	
bool CAOIFrame::ExecFrameSave()//影像儲存圖檔
{
	CAOIFrame *FramePtr = this;
	if ( NULL == FramePtr ) { return false; }

	bool       IsOK=true;
	CString    Folder;
	CString    FileName;
	CString    PathName;
	FRAME_TYPE FrameType;
	TUNI_FRAME UniFrame;
	TUNI_FRAME UniFrameTmp;

	FramePtr->GetFrameUniFrame(UniFrame);
			
	FrameType = FramePtr->GetFrameType();
	Folder = FramePtr->GetFrameFileFolder();
	FileName = FramePtr->GetFrameFileName();			
	PathName.Format(_T("%s\\%s"), Folder, FileName);
	UniFrameTmp = UniFrame;

	IsOK=true;	
	if ( FRAME_SPACE != FrameType )
	{
		if ( NULL != UniFrame.ImagePtr )
		{	
			if ( ImageAPI.SaveImage(PathName, UniFrame, true) == false )
			{	IsOK = false; }
		}
	}
	else
	{
		if ( NULL != UniFrame.SpacePtr )
		{
			if ( ImageAPI.SaveSpaceGrayImage(PathName, UniFrame, true) == false )
			{	IsOK = false; }
		}

		if ( NULL != UniFrame.MaskPtr )
		{
			FileName = PathName;
			PathName = AOIDataDefine.GetFrameMaskName(FileName);
			if ( ImageAPI.SaveMaskGrayImage(PathName, UniFrame, true) == false )
			{	IsOK = false; }
		}
	}
	FramePtr->SetFrameImageSaved(true);
	return IsOK;
}
//-------------------------------------------------------------------------------------//	
bool CAOIFrame::ExecFrameSave_Thread()//影像儲存圖檔-執行緒內
{
	if ( GetFrameImageSaved() == true ) { return true; }
	CAOIFrame::LockFrame();
	if ( GetFrameImageSaved() == true )
	{
		CAOIFrame::UnlockFrame();
		return true;
	}
	SetFrameImageSaved(true);
	CAOIFrame::UnlockFrame();
	return ExecFrameSave();
}
//-------------------------------------------------------------------------------------//	
bool CAOIFrame::ExecFrameLoadImage()//影像載入圖檔
{	
	bool IsOK = false;	
	CString str;
	CString ImageFileName;
	CString FileName = GetFrameFileName();
	CString FileFolder = GetFrameFileFolder();
	CString FileNameOffline = GetFrameFileNameOffline();
	CString FileFolderOffline = GetFrameFileFolderOffline();
	const size_t FrameIndex = GetFrameIndex();
	const size_t FrameIndexOffline = GetFrameIndexOffline();
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	FRAME_TYPE   FrameType = GetFrameType();	

	CAOIFrame::ClearFrameBuffer();
	ImageFileName.Format(_T("%s\\%s"), FileFolder, FileName);
	if ( FileNameOffline!=FileName || FileFolderOffline!=FileFolder || FrameIndexOffline!=FrameIndex )
	{	IsOK = IsOK;	}

	if ( FRAME_SPACE == FrameType )
	{	
		IsOK = ImageAPI.LoadSpaceGrayImage(ImageFileName, m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameSpacePtr, true);		
		if ( false == IsOK ) 
		{	return false;	}

		CString ImageMaskName = AOIDataDefine.GetFrameMaskName(ImageFileName);	
		IsOK = ImageAPI.LoadMaskGrayImage(ImageMaskName, m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameMaskPtr, true);
		if ( false == IsOK )
		{	
			CAOIFrame::ClearFrameBuffer();
			return false; 
		}			
		m_FrameImageBitCount = 8;
	}
	else
	{
		IsOK = ImageAPI.LoadImage(ImageFileName, m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameImageBitCount, m_FrameImagePtr, 4, true);		
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//	
bool CAOIFrame::SaveFrameImage_Offline()//影像儲存圖檔
{
	const size_t FieldIndex = GetFrameFieldIndex();
	CString Folder;
	CString FileName;
	CString PathName;		
	RECT    RoiRect={0};
	const IMAGE_SIZE ImageW = m_FrameImageW;
	const IMAGE_SIZE ImageH = m_FrameImageH;
	const IMAGE_SIZE ImageStep = m_FrameImageStep;
	const IMAGE_SIZE BitCount = m_FrameImageBitCount;
	FRAME_TYPE       FrameType = GetFrameType();
	DISTRICT_ID      DistrictID = GetFrameDistrictID();
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();

	RoiRect.left = 0;
	RoiRect.top = 0;
	RoiRect.right = (int)(ImageW);
	RoiRect.bottom = (int)(ImageH);
	FileName = AOIDataDefine.GetFrameShortName(m_FrameIndex, FrameType, false, DistrictID);
	PathName.Format(_T("%s\\%s"), AOIDataCollect.GetOfflineFolder(), FileName);		
	FileName = GetFrameFileName();
	if ( 0 == FileName.GetLength() )
	{	return false; }
	
	PathName.Format(_T("%s\\%s"), GetFrameFileFolder(), FileName);
	if ( FRAME_SPACE == FrameType )
	{	
		if ( NULL!=m_FrameSpacePtr && NULL!=m_FrameMaskPtr )
		{					
			IMAGE_PTR ImagePtr = NULL;
			if ( ImageAPI.SpaceGrayImageConvertToGray(ImageW, ImageH, ImageStep, m_FrameSpacePtr, m_FrameMaskPtr, RoiRect, ImageStep, ImagePtr, SpaceRatio, false) == true )
			{
				CString PathName2;
				CString FileName2 = AOIDataDefine.GetFrameShortName(m_FrameIndex, FRAME_GRAY, false, DistrictID);					
				PathName2.Format(_T("%s\\%s"), AOIDataCollect.GetOfflineFolder(), FileName2);		
				ImageAPI.SavePNGImage(PathName2, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);	
				JetMemory.free_func(ImagePtr);
			}
		}

		if ( NULL != m_FrameSpacePtr )
		{	
			ImageAPI.SaveSpaceGrayImage(PathName, ImageW, ImageH, ImageStep, m_FrameSpacePtr, true);	
			CAOIFrame::SetFrameImageSaved(true);
		}
		if ( NULL != m_FrameMaskPtr )
		{
			//FileName = AOIDataDefine.GetFrameShortName(m_FrameIndex, FrameType, true);
			//PathName.Format(_T("%s\\%s"), AOIDataCollect.GetOfflineFolder(), FileName);				
			FileName = PathName;
			PathName = AOIDataDefine.GetFrameMaskName(FileName);
			ImageAPI.SaveMaskGrayImage(PathName, ImageW, ImageH, ImageStep, m_FrameMaskPtr, true);
			CAOIFrame::SetFrameImageSaved(true);
		}			
	}
	else
	{
		if ( NULL != m_FrameImagePtr)
		{	
			ImageAPI.SavePNGImage(PathName, ImageW, ImageH, ImageStep, BitCount, m_FrameImagePtr, true);	
			CAOIFrame::SetFrameImageSaved(true);
		}			
	}			
	return true;
}
//-------------------------------------------------------------------------------------//	
inline bool CAOIFrame::CheckFrameNeedToLoad()//確認影像需要載入
{	
	CAOIField  *FieldPtr = CAOIFrame::GetFrameFieldPtr();
	if ( NULL == FieldPtr ) { return false; }
	if ( FieldPtr->GetFieldMustToLoad() == true ) { return true; }

	const size_t RgnCount = FieldPtr->GetFieldRgnPtrCount();
	if ( 0 == RgnCount ) { return false; }

	size_t i=0;
	CAOIRgn *RgnPtr = NULL;
	for ( i=0; i<RgnCount; i++ )
	{
		RgnPtr = FieldPtr->GetFieldRgnPtr(i, false);
		if ( NULL == RgnPtr ) { continue; }		
		if ( RgnPtr->GetRgnNeedToCalculate() == true )
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//	
inline bool CAOIFrame::ExecFrameMergeSlice()//影像合併區域影像
{
	bool IsOK = true;	
	ExecFrameSaveRawImage();
	switch ( m_FrameType )
	{
	case FRAME_GRAY:
		IsOK = ExecFrameMergeSlice_Gray();
		break;
	case FRAME_BAYER:
		IsOK = ExecFrameMergeSlice_Bayer();
		break;
	case FRAME_COLOR:
		IsOK = ExecFrameMergeSlice_Color();
		break;
	case FRAME_SPACE:
		IsOK = ExecFrameMergeSlice_Space();		
		break;
	}
	SetFrameImageValid(IsOK);
	if ( false == IsOK ) 
	{	return false; }		
	return IsOK;
}
//-------------------------------------------------------------------------------------//	
inline bool CAOIFrame::AllocateFrameBuffer()//建立影像資料
{
	const char fnName[] = "CAOIFrame::AllocateFrameBuffer";
	CAMERA_ID    CameraID = CAOIFrame::GetFrameCameraID();
	FRAME_TYPE   FrameType = CAOIFrame::GetFrameType();		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	IMAGE_SIZE   BitCount = 0;
	MASK_PTR     FrameMaskPtr = NULL;//Frame空間遮罩
	IMAGE_PTR    FrameImagePtr = NULL;//Frame影像記憶體區塊
	SPACE_PTR    FrameSpacePtr = NULL;//Frame空間記憶體區塊
	ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	CAOIFrame::ClearFrameBuffer();
	switch ( FrameType )
	{
	case FRAME_GRAY:	BitCount = 8;	break;
	case FRAME_BAYER:	BitCount = 24;	break;
	case FRAME_COLOR:	BitCount = 24;	break;
	case FRAME_SPACE:	BitCount = 8;	break;
	default:
		BitCount = 0;
		break;
	}	
	if ( 0 == BitCount ) { return false; }
	ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( FRAME_SPACE == FrameType )
	{	
		if ( BufferSize > m_FrameShareMaskSize )
		{
			if ( JetMemory.alloc_func(BufferSize, FrameMaskPtr, fnName, "FrameMaskPtr") == false )
			{	return false;	}
			//::memset(FrameMaskPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
			::memset(FrameMaskPtr, PHASE_MASK_NOISE_ONLY, sizeof(MASK_DATA)*BufferSize);
		}
		else
		{
			SetFrameUseShareMaskBuf(true);
			FrameMaskPtr=m_FrameShareMaskPtr;
		}

		if ( BufferSize > m_FrameShareSpaceSize )
		{
			if ( JetMemory.alloc_func(BufferSize, FrameSpacePtr, fnName, "FrameSpacePtr") == false )
			{
				if ( GetFrameUseShareMaskBuf() == false )
				{	JetMemory.free_func(FrameMaskPtr); }
				return false; 
			}
			::memset(FrameSpacePtr, 0x00, sizeof(SPACE_DATA)*BufferSize);			
		}
		else
		{	
			SetFrameUseShareSpaceBuf(true);
			FrameSpacePtr=m_FrameShareSpacePtr;			
		}	

		if ( BufferSize > m_FrameShareImageSize )
		{	FrameImagePtr = NULL;	}
		else
		{
			SetFrameUseShareImageBuf(true);
			FrameImagePtr=m_FrameShareImagePtr;	
		}
	}
	else
	{
		if ( BufferSize > m_FrameShareImageSize )
		{
			if ( JetMemory.alloc_func(BufferSize, FrameImagePtr, fnName, "FrameImagePtr") == false )
			{	return false; }
			::memset(FrameImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
		}
		else
		{
			SetFrameUseShareImageBuf(true);
			FrameImagePtr=m_FrameShareImagePtr;	
		}
	}
	
	m_FrameImageW = ImageW;
	m_FrameImageH = ImageH;
	m_FrameImageStep = ImageStep;
	m_FrameImageBitCount = BitCount;	
	m_FrameMaskPtr = FrameMaskPtr;//Frame空間遮罩
	m_FrameImagePtr = FrameImagePtr;//Frame影像記憶體區塊
	m_FrameSpacePtr = FrameSpacePtr;//Frame空間記憶體區塊
	return true;
}
//-------------------------------------------------------------------------------------//	
inline bool CAOIFrame::ExecFrameMergeSlice_Gray()//影像合併區域影像-灰階
{
	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	if ( 1 != SliceCount ) { return false; }

	CAOISlice *SlicePtr = CAOIFrame::GetFrameSlicePtr(0, false);
	if ( NULL == SlicePtr ) { return false; }
	if ( SlicePtr->GetSliceImageBuffer(m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameImageBitCount, m_FrameImagePtr) == false )
	{	return false; }
	SlicePtr->ReleaseSliceImageBuffer();//釋放對記憶體的佔住
	SlicePtr->SetSliceFillState(SLICE_FILL_CLEAR);
	return true;
}
//-------------------------------------------------------------------------------------//	
inline bool CAOIFrame::ExecFrameMergeSlice_Bayer()//影像合併區域影像-彩色-原圖
{
	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	if ( 1 != SliceCount ) { return false; }

	CAOISlice *SlicePtr = CAOIFrame::GetFrameSlicePtr(0, false);
	if ( NULL == SlicePtr ) { return false; }
	if ( SlicePtr->GetSliceImageBuffer(m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameImageBitCount, m_FrameImagePtr) == false )
	{	return false; }
	SlicePtr->ReleaseSliceImageBuffer();//釋放對記憶體的佔住
	SlicePtr->SetSliceFillState(SLICE_FILL_CLEAR);
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergeSlice_Color()//影像合併區域影像-彩色-RGB合併
{
	bool IsOK = false;
	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	switch ( SliceCount )
	{
	case 3:		IsOK = CAOIFrame::ExecFrameMergeSlice_Color03();	break;
	case 6:		IsOK = CAOIFrame::ExecFrameMergeSlice_Color06();	break;
	case 9:		IsOK = CAOIFrame::ExecFrameMergeSlice_Color09();	break;
	case 12:	IsOK = CAOIFrame::ExecFrameMergeSlice_Color12();	break;
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergeSlice_Color03()//影像合併區域影像-彩色	-RGBx1
{
	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	if ( 3 != SliceCount ) { return false; }

	CString str;	
	CAOISlice *SlicePtr1 = CAOIFrame::GetFrameSlicePtr(0, false);//R
	CAOISlice *SlicePtr2 = CAOIFrame::GetFrameSlicePtr(1, false);//G
	CAOISlice *SlicePtr3 = CAOIFrame::GetFrameSlicePtr(2, false);//B
	if ( NULL==SlicePtr1 || NULL==SlicePtr2 || NULL==SlicePtr3 )
	{
		//刪除影像記憶體
		if ( NULL != SlicePtr1 )
		{	SlicePtr1->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr2 )
		{	SlicePtr2->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr3 )
		{	SlicePtr3->ClearSliceImageBuffer(); }
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}

	IMAGE_SIZE    ImageW[3]={0};
	IMAGE_SIZE    ImageH[3]={0};
	IMAGE_SIZE    ImageStep[3]={0};
	IMAGE_SIZE    BitCount[3]={0};
	IMAGE_PTR ImagePtr[3]={NULL};
	if ( SlicePtr1->GetSliceImageBuffer(ImageW[0], ImageH[0], ImageStep[0], BitCount[0], ImagePtr[0])==false ||
		 SlicePtr2->GetSliceImageBuffer(ImageW[1], ImageH[1], ImageStep[1], BitCount[1], ImagePtr[1])==false ||	
		 SlicePtr3->GetSliceImageBuffer(ImageW[2], ImageH[2], ImageStep[2], BitCount[2], ImagePtr[2])==false )
	{	
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}

#ifdef _DEBUG
	if ( ImageW[0]!=ImageW[1] || ImageW[1]!=ImageW[2] ||
		 ImageH[0]!=ImageH[1] || ImageH[1]!=ImageH[2] ||
		 ImageStep[0]!=ImageStep[1] || ImageStep[1]!=ImageStep[2] || 
		 BitCount[0]!=BitCount[1] || BitCount[1]!=BitCount[2] ) 
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}
#endif//_DEBUG

	IMAGE_SIZE   RGBBitCount = 24;
	const IMAGE_SIZE RGBImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW[0], RGBBitCount, 4);		
	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[0], ImagePtr[1], ImagePtr[2], RGBImageStep, m_FrameImagePtr, false) == false )
	//if ( JetMemory.alloc_func(RGBImageStep*ImageH[0], m_FrameImagePtr, "CAOIFrame::ExecFrameMergeSlice_Color", "m_FrameImagePtr") == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		return false;
	}	
	//::memset(m_FrameImagePtr, 0xFF, sizeof(IMAGE_DATA)*RGBImageStep*ImageH[0]);

	const double Precision = DBL_PRECISION;
	const double FrameSatRed = GetFrameSaturationRed();
	const double FrameSatGrn = GetFrameSaturationGreen();
	const double FrameSatBlu = GetFrameSaturationBlue();
	if ( fabs(FrameSatRed-1.0)>Precision || fabs(FrameSatGrn-1.0)>Precision || fabs(FrameSatBlu-1.0)>Precision )
	{	ImageAPI.SaturateColorImage3(ImageW[0], ImageH[0], RGBImageStep, m_FrameImagePtr, m_FrameImagePtr, FrameSatRed, FrameSatBlu);	}
	
	m_FrameImageW = ImageW[0];
	m_FrameImageH = ImageH[0];
	m_FrameImageStep = RGBImageStep;
	m_FrameImageBitCount = 24;	

	SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
#ifdef _DEBUG	
	str.Format(_T("%s\\Frame#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), this->GetFrameIndex()+1);
	//ImageAPI.SaveImage(str, m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameImageBitCount, m_FrameImagePtr, false);
	//CAOIFrame::ClearFrameBuffer();
#endif	
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergeSlice_Color06()//影像合併區域影像-彩色	-RGBx2
{
	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	if ( 6 != SliceCount ) { return false; }

	CAOISlice *SlicePtr1 = CAOIFrame::GetFrameSlicePtr(0, false);//R
	CAOISlice *SlicePtr2 = CAOIFrame::GetFrameSlicePtr(1, false);//G
	CAOISlice *SlicePtr3 = CAOIFrame::GetFrameSlicePtr(2, false);//B
	CAOISlice *SlicePtr4 = CAOIFrame::GetFrameSlicePtr(3, false);//R
	CAOISlice *SlicePtr5 = CAOIFrame::GetFrameSlicePtr(4, false);//G
	CAOISlice *SlicePtr6 = CAOIFrame::GetFrameSlicePtr(5, false);//B
	if ( NULL==SlicePtr1 || NULL==SlicePtr2 || NULL==SlicePtr3 || NULL==SlicePtr4 || NULL==SlicePtr5 || NULL==SlicePtr6 )
	{
		//刪除影像記憶體
		if ( NULL != SlicePtr1 )
		{	SlicePtr1->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr2 )
		{	SlicePtr2->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr3 )
		{	SlicePtr3->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr4 )
		{	SlicePtr4->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr5 )
		{	SlicePtr5->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr6 )
		{	SlicePtr6->ClearSliceImageBuffer(); }

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}

	size_t    i=0;
	IMAGE_SIZE    ImageW[6]={0};
	IMAGE_SIZE    ImageH[6]={0};
	IMAGE_SIZE    ImageStep[6]={0};
	IMAGE_SIZE    BitCount[6]={0};
	IMAGE_PTR ImagePtr[6]={NULL};
	if ( SlicePtr1->GetSliceImageBuffer(ImageW[0], ImageH[0], ImageStep[0], BitCount[0], ImagePtr[0])==false ||
		 SlicePtr2->GetSliceImageBuffer(ImageW[1], ImageH[1], ImageStep[1], BitCount[1], ImagePtr[1])==false ||	
		 SlicePtr3->GetSliceImageBuffer(ImageW[2], ImageH[2], ImageStep[2], BitCount[2], ImagePtr[2])==false ||
		 SlicePtr4->GetSliceImageBuffer(ImageW[3], ImageH[3], ImageStep[3], BitCount[3], ImagePtr[3])==false ||
		 SlicePtr5->GetSliceImageBuffer(ImageW[4], ImageH[4], ImageStep[4], BitCount[4], ImagePtr[4])==false ||	
		 SlicePtr6->GetSliceImageBuffer(ImageW[5], ImageH[5], ImageStep[5], BitCount[5], ImagePtr[5])==false )
	{	
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}

#ifdef _DEBUG
	if ( ImageW[0]!=ImageW[1] || ImageW[0]!=ImageW[2] || ImageW[0]!=ImageW[3] || ImageW[0]!=ImageW[4] || ImageW[0]!=ImageW[5] ||
		 ImageH[0]!=ImageH[1] || ImageH[0]!=ImageH[2] || ImageH[0]!=ImageH[3] || ImageH[0]!=ImageH[4] || ImageH[0]!=ImageH[5] ||
		 ImageStep[0]!=ImageStep[1] || ImageStep[0]!=ImageStep[2] || ImageStep[0]!=ImageStep[3] || ImageStep[0]!=ImageStep[4] || ImageStep[0]!=ImageStep[5] || 
		 BitCount[0]!=BitCount[1] || BitCount[0]!=BitCount[2] || BitCount[0]!=BitCount[3] || BitCount[0]!=BitCount[4] || BitCount[0]!=BitCount[5] ) 
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}
#endif//_DEBUG

	IMAGE_PTR    RGBImagePtr1 = NULL;
	IMAGE_PTR    RGBImagePtr2 = NULL;
	IMAGE_SIZE   RGBBitCount = 24;
	const IMAGE_SIZE RGBImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW[0], RGBBitCount, 4);		
	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[0], ImagePtr[1], ImagePtr[2], RGBImageStep, RGBImagePtr1, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		return false;
	}	

	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[3], ImagePtr[4], ImagePtr[5], RGBImageStep, RGBImagePtr2, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		JetMemory.free_func(RGBImagePtr1);
		return false;
	}	
	//::memset(m_FrameImagePtr, 0xFF, sizeof(IMAGE_DATA)*RGBImageStep*ImageH[0]);
	
	//if ( ImageAPI.AverageImage2Frame(ImageW[0], ImageH[0], RGBImageStep, 24, RGBImagePtr1, RGBImagePtr2, m_FrameImagePtr) == false )
	if ( ImageAPI.MaxValueImage2Frame(ImageW[0], ImageH[0], RGBImageStep, 24, RGBImagePtr1, RGBImagePtr2, m_FrameImagePtr) == false )
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		JetMemory.free_func(RGBImagePtr1);
		JetMemory.free_func(RGBImagePtr2);
		return false;
	}
	
	const double Precision = DBL_PRECISION;
	const double FrameSatRed = GetFrameSaturationRed();
	const double FrameSatGrn = GetFrameSaturationGreen();
	const double FrameSatBlu = GetFrameSaturationBlue();
	if ( fabs(FrameSatRed-1.0)>Precision || fabs(FrameSatGrn-1.0)>Precision || fabs(FrameSatBlu-1.0)>Precision )
	{	ImageAPI.SaturateColorImage3(ImageW[0], ImageH[0], RGBImageStep, m_FrameImagePtr, m_FrameImagePtr, FrameSatRed, FrameSatBlu);	}

	JetMemory.free_func(RGBImagePtr1);
	JetMemory.free_func(RGBImagePtr2);
	m_FrameImageW = ImageW[0];
	m_FrameImageH = ImageH[0];
	m_FrameImageStep = RGBImageStep;
	m_FrameImageBitCount = 24;	

	SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);

#ifdef _DEBUG
	//CString str;
	//str.Format(_T("%s\\Frame#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), this->GetFrameIndex()+1);
	//ImageAPI.SaveImage(str, m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameImageBitCount, m_FrameImagePtr, false);
	//CAOIFrame::ClearFrameBuffer();
#endif	
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergeSlice_Color09()//影像合併區域影像-彩色-RGBx3
{
	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	if ( 9 != SliceCount ) { return false; }

	CAOISlice *SlicePtr1 = CAOIFrame::GetFrameSlicePtr(0, false);//R
	CAOISlice *SlicePtr2 = CAOIFrame::GetFrameSlicePtr(1, false);//G
	CAOISlice *SlicePtr3 = CAOIFrame::GetFrameSlicePtr(2, false);//B
	CAOISlice *SlicePtr4 = CAOIFrame::GetFrameSlicePtr(3, false);//R
	CAOISlice *SlicePtr5 = CAOIFrame::GetFrameSlicePtr(4, false);//G
	CAOISlice *SlicePtr6 = CAOIFrame::GetFrameSlicePtr(5, false);//B
	CAOISlice *SlicePtr7 = CAOIFrame::GetFrameSlicePtr(6, false);//R
	CAOISlice *SlicePtr8 = CAOIFrame::GetFrameSlicePtr(7, false);//G
	CAOISlice *SlicePtr9 = CAOIFrame::GetFrameSlicePtr(8, false);//B
	if ( NULL==SlicePtr1 || NULL==SlicePtr2 || NULL==SlicePtr3 || NULL==SlicePtr4 || NULL==SlicePtr5 || NULL==SlicePtr6 || NULL==SlicePtr7 || NULL==SlicePtr8 || NULL==SlicePtr9)
	{
		//刪除影像記憶體
		if ( NULL != SlicePtr1 )
		{	SlicePtr1->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr2 )
		{	SlicePtr2->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr3 )
		{	SlicePtr3->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr4 )
		{	SlicePtr4->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr5 )
		{	SlicePtr5->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr6 )
		{	SlicePtr6->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr7 )
		{	SlicePtr7->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr8 )
		{	SlicePtr8->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr9 )
		{	SlicePtr9->ClearSliceImageBuffer(); }

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}

	size_t        i=0;
	IMAGE_SIZE    ImageW[9]={0};
	IMAGE_SIZE    ImageH[9]={0};
	IMAGE_SIZE    ImageStep[9]={0};
	IMAGE_SIZE    BitCount[9]={0};
	IMAGE_PTR     ImagePtr[9]={NULL};
	if ( SlicePtr1->GetSliceImageBuffer(ImageW[0], ImageH[0], ImageStep[0], BitCount[0], ImagePtr[0])==false ||
		 SlicePtr2->GetSliceImageBuffer(ImageW[1], ImageH[1], ImageStep[1], BitCount[1], ImagePtr[1])==false ||	
		 SlicePtr3->GetSliceImageBuffer(ImageW[2], ImageH[2], ImageStep[2], BitCount[2], ImagePtr[2])==false ||
		 SlicePtr4->GetSliceImageBuffer(ImageW[3], ImageH[3], ImageStep[3], BitCount[3], ImagePtr[3])==false ||
		 SlicePtr5->GetSliceImageBuffer(ImageW[4], ImageH[4], ImageStep[4], BitCount[4], ImagePtr[4])==false ||	
		 SlicePtr6->GetSliceImageBuffer(ImageW[5], ImageH[5], ImageStep[5], BitCount[5], ImagePtr[5])==false ||
		 SlicePtr7->GetSliceImageBuffer(ImageW[6], ImageH[6], ImageStep[6], BitCount[6], ImagePtr[6])==false ||	
		 SlicePtr8->GetSliceImageBuffer(ImageW[7], ImageH[7], ImageStep[7], BitCount[7], ImagePtr[7])==false ||	
		 SlicePtr9->GetSliceImageBuffer(ImageW[8], ImageH[8], ImageStep[8], BitCount[8], ImagePtr[8])==false )
	{	
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}

#ifdef _DEBUG
	if ( ImageW[0]!=ImageW[1] || ImageW[0]!=ImageW[2] || ImageW[0]!=ImageW[3] || ImageW[0]!=ImageW[4] || ImageW[0]!=ImageW[5] || ImageW[0]!=ImageW[6] || ImageW[0]!=ImageW[7] || ImageW[0]!=ImageW[8] ||
		 ImageH[0]!=ImageH[1] || ImageH[0]!=ImageH[2] || ImageH[0]!=ImageH[3] || ImageH[0]!=ImageH[4] || ImageH[0]!=ImageH[5] || ImageH[0]!=ImageH[6] || ImageH[0]!=ImageH[7] || ImageH[0]!=ImageH[8] || 
		 ImageStep[0]!=ImageStep[1] || ImageStep[0]!=ImageStep[2] || ImageStep[0]!=ImageStep[3] || ImageStep[0]!=ImageStep[4] || ImageStep[0]!=ImageStep[5] || ImageStep[0]!=ImageStep[6] || ImageStep[0]!=ImageStep[7] || ImageStep[0]!=ImageStep[8] ||
		 BitCount[0]!=BitCount[1] || BitCount[0]!=BitCount[2] || BitCount[0]!=BitCount[3] || BitCount[0]!=BitCount[4] || BitCount[0]!=BitCount[5] || BitCount[0]!=BitCount[6] || BitCount[0]!=BitCount[7] || BitCount[0]!=BitCount[8] ) 
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}
#endif//_DEBUG

	IMAGE_PTR    RGBImagePtr1 = NULL;
	IMAGE_PTR    RGBImagePtr2 = NULL;
	IMAGE_PTR    RGBImagePtr3 = NULL;	
	IMAGE_SIZE   RGBBitCount = 24;
	const IMAGE_SIZE RGBImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW[0], RGBBitCount, 4);		
	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[0], ImagePtr[1], ImagePtr[2], RGBImageStep, RGBImagePtr1, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		return false;
	}	

	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[3], ImagePtr[4], ImagePtr[5], RGBImageStep, RGBImagePtr2, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);

		JetMemory.free_func(RGBImagePtr1);
		return false;
	}	

	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[6], ImagePtr[7], ImagePtr[8], RGBImageStep, RGBImagePtr3, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);

		JetMemory.free_func(RGBImagePtr1);
		JetMemory.free_func(RGBImagePtr2);
		return false;
	}	
	//::memset(m_FrameImagePtr, 0xFF, sizeof(IMAGE_DATA)*RGBImageStep*ImageH[0]);
	
	//if ( ImageAPI.AverageImage2Frame(ImageW[0], ImageH[0], RGBImageStep, 24, RGBImagePtr1, RGBImagePtr2, m_FrameImagePtr) == false )
	if ( ImageAPI.MaxValueImage3Frame(ImageW[0], ImageH[0], RGBImageStep, 24, RGBImagePtr1, RGBImagePtr2, RGBImagePtr3, m_FrameImagePtr) == false )
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		JetMemory.free_func(RGBImagePtr1);
		JetMemory.free_func(RGBImagePtr2);
		JetMemory.free_func(RGBImagePtr3);
		return false;
	}
	
	const double Precision = DBL_PRECISION;
	const double FrameSatRed = GetFrameSaturationRed();
	const double FrameSatGrn = GetFrameSaturationGreen();
	const double FrameSatBlu = GetFrameSaturationBlue();
	if ( fabs(FrameSatRed-1.0)>Precision || fabs(FrameSatGrn-1.0)>Precision || fabs(FrameSatBlu-1.0)>Precision )
	{	ImageAPI.SaturateColorImage3(ImageW[0], ImageH[0], RGBImageStep, m_FrameImagePtr, m_FrameImagePtr, FrameSatRed, FrameSatBlu);	}

	JetMemory.free_func(RGBImagePtr1);
	JetMemory.free_func(RGBImagePtr2);
	JetMemory.free_func(RGBImagePtr3);
	m_FrameImageW = ImageW[0];
	m_FrameImageH = ImageH[0];
	m_FrameImageStep = RGBImageStep;
	m_FrameImageBitCount = 24;	

	SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);

#ifdef _DEBUG
	//CString str;
	//str.Format(_T("%s\\Frame#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), this->GetFrameIndex()+1);
	//ImageAPI.SaveImage(str, m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameImageBitCount, m_FrameImagePtr, false);
	//CAOIFrame::ClearFrameBuffer();
#endif		
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergeSlice_Color12()//影像合併區域影像-彩色	-RGBx4
{
	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	if ( 12 != SliceCount ) { return false; }

	CAOISlice *SlicePtr1 = CAOIFrame::GetFrameSlicePtr(0, false);//R
	CAOISlice *SlicePtr2 = CAOIFrame::GetFrameSlicePtr(1, false);//G
	CAOISlice *SlicePtr3 = CAOIFrame::GetFrameSlicePtr(2, false);//B
	CAOISlice *SlicePtr4 = CAOIFrame::GetFrameSlicePtr(3, false);//R
	CAOISlice *SlicePtr5 = CAOIFrame::GetFrameSlicePtr(4, false);//G
	CAOISlice *SlicePtr6 = CAOIFrame::GetFrameSlicePtr(5, false);//B
	CAOISlice *SlicePtr7 = CAOIFrame::GetFrameSlicePtr(6, false);//R
	CAOISlice *SlicePtr8 = CAOIFrame::GetFrameSlicePtr(7, false);//G
	CAOISlice *SlicePtr9 = CAOIFrame::GetFrameSlicePtr(8, false);//B
	CAOISlice *SlicePtr10 = CAOIFrame::GetFrameSlicePtr(9, false);//R
	CAOISlice *SlicePtr11 = CAOIFrame::GetFrameSlicePtr(10, false);//G
	CAOISlice *SlicePtr12 = CAOIFrame::GetFrameSlicePtr(11, false);//B
	if ( NULL==SlicePtr1 || NULL==SlicePtr2 || NULL==SlicePtr3 || NULL==SlicePtr4 || NULL==SlicePtr5 || NULL==SlicePtr6 || NULL==SlicePtr7 || NULL==SlicePtr8 || NULL==SlicePtr9 || NULL==SlicePtr10 || NULL==SlicePtr11 || NULL==SlicePtr12)
	{
		//刪除影像記憶體
		if ( NULL != SlicePtr1 )
		{	SlicePtr1->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr2 )
		{	SlicePtr2->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr3 )
		{	SlicePtr3->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr4 )
		{	SlicePtr4->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr5 )
		{	SlicePtr5->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr6 )
		{	SlicePtr6->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr7 )
		{	SlicePtr7->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr8 )
		{	SlicePtr8->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr9 )
		{	SlicePtr9->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr10 )
		{	SlicePtr10->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr11 )
		{	SlicePtr11->ClearSliceImageBuffer(); }
		if ( NULL != SlicePtr12 )
		{	SlicePtr12->ClearSliceImageBuffer(); }

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}

	size_t        i=0;
	IMAGE_SIZE    ImageW[12]={0};
	IMAGE_SIZE    ImageH[12]={0};
	IMAGE_SIZE    ImageStep[12]={0};
	IMAGE_SIZE    BitCount[12]={0};
	IMAGE_PTR     ImagePtr[12]={NULL};
	if ( SlicePtr1->GetSliceImageBuffer(ImageW[0], ImageH[0], ImageStep[0], BitCount[0], ImagePtr[0])==false ||
		 SlicePtr2->GetSliceImageBuffer(ImageW[1], ImageH[1], ImageStep[1], BitCount[1], ImagePtr[1])==false ||	
		 SlicePtr3->GetSliceImageBuffer(ImageW[2], ImageH[2], ImageStep[2], BitCount[2], ImagePtr[2])==false ||
		 SlicePtr4->GetSliceImageBuffer(ImageW[3], ImageH[3], ImageStep[3], BitCount[3], ImagePtr[3])==false ||
		 SlicePtr5->GetSliceImageBuffer(ImageW[4], ImageH[4], ImageStep[4], BitCount[4], ImagePtr[4])==false ||	
		 SlicePtr6->GetSliceImageBuffer(ImageW[5], ImageH[5], ImageStep[5], BitCount[5], ImagePtr[5])==false ||
		 SlicePtr7->GetSliceImageBuffer(ImageW[6], ImageH[6], ImageStep[6], BitCount[6], ImagePtr[6])==false ||	
		 SlicePtr8->GetSliceImageBuffer(ImageW[7], ImageH[7], ImageStep[7], BitCount[7], ImagePtr[7])==false ||	
		 SlicePtr9->GetSliceImageBuffer(ImageW[8], ImageH[8], ImageStep[8], BitCount[8], ImagePtr[8])==false ||
		 SlicePtr10->GetSliceImageBuffer(ImageW[9], ImageH[9], ImageStep[9], BitCount[9], ImagePtr[9])==false ||	
		 SlicePtr11->GetSliceImageBuffer(ImageW[10], ImageH[10], ImageStep[10], BitCount[10], ImagePtr[10])==false ||	
		 SlicePtr12->GetSliceImageBuffer(ImageW[11], ImageH[11], ImageStep[11], BitCount[11], ImagePtr[11])==false)
	{	
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr10->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr11->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr12->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}

#ifdef _DEBUG
	if ( ImageW[0]!=ImageW[1] || ImageW[0]!=ImageW[2] || ImageW[0]!=ImageW[3] || ImageW[0]!=ImageW[4] || ImageW[0]!=ImageW[5] || ImageW[0]!=ImageW[6] || ImageW[0]!=ImageW[7] || ImageW[0]!=ImageW[8] || ImageW[0]!=ImageW[9] || ImageW[0]!=ImageW[10] || ImageW[0]!=ImageW[11] ||
		 ImageH[0]!=ImageH[1] || ImageH[0]!=ImageH[2] || ImageH[0]!=ImageH[3] || ImageH[0]!=ImageH[4] || ImageH[0]!=ImageH[5] || ImageH[0]!=ImageH[6] || ImageH[0]!=ImageH[7] || ImageH[0]!=ImageH[8] || ImageH[0]!=ImageH[9] || ImageH[0]!=ImageH[10] || ImageH[0]!=ImageH[11] ||
		 ImageStep[0]!=ImageStep[1] || ImageStep[0]!=ImageStep[2] || ImageStep[0]!=ImageStep[3] || ImageStep[0]!=ImageStep[4] || ImageStep[0]!=ImageStep[5] || ImageStep[0]!=ImageStep[6] || ImageStep[0]!=ImageStep[7] || ImageStep[0]!=ImageStep[8] || ImageStep[0]!=ImageStep[9] || ImageStep[0]!=ImageStep[10] || ImageStep[0]!=ImageStep[11] ||
		 BitCount[0]!=BitCount[1] || BitCount[0]!=BitCount[2] || BitCount[0]!=BitCount[3] || BitCount[0]!=BitCount[4] || BitCount[0]!=BitCount[5] || BitCount[0]!=BitCount[6] || BitCount[0]!=BitCount[7] || BitCount[0]!=BitCount[8] || BitCount[0]!=BitCount[9] || BitCount[0]!=BitCount[10] || BitCount[0]!=BitCount[11]) 
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr10->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr11->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr12->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);
		return false; 
	}
#endif//_DEBUG

	IMAGE_PTR    RGBImagePtr1 = NULL;
	IMAGE_PTR    RGBImagePtr2 = NULL;
	IMAGE_PTR    RGBImagePtr3 = NULL;	
	IMAGE_PTR    RGBImagePtr4 = NULL;
	IMAGE_SIZE   RGBBitCount = 24;
	const IMAGE_SIZE RGBImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW[0], RGBBitCount, 4);
	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[0], ImagePtr[1], ImagePtr[2], RGBImageStep, RGBImagePtr1, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr10->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr11->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr12->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);
		return false;
	}	

	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[3], ImagePtr[4], ImagePtr[5], RGBImageStep, RGBImagePtr2, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr10->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr11->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr12->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);

		JetMemory.free_func(RGBImagePtr1);
		return false;
	}	

	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[6], ImagePtr[7], ImagePtr[8], RGBImageStep, RGBImagePtr3, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr10->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr11->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr12->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);

		JetMemory.free_func(RGBImagePtr1);
		JetMemory.free_func(RGBImagePtr2);
		return false;
	}	

	if ( ImageAPI.RGBImageToColorImage(ImageW[0], ImageH[0], ImageStep[0], ImagePtr[9], ImagePtr[10], ImagePtr[11], RGBImageStep, RGBImagePtr4, false) == false )	
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr10->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr11->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr12->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);

		JetMemory.free_func(RGBImagePtr1);
		JetMemory.free_func(RGBImagePtr2);
		JetMemory.free_func(RGBImagePtr3);
		return false;
	}	
	//::memset(m_FrameImagePtr, 0xFF, sizeof(IMAGE_DATA)*RGBImageStep*ImageH[0]);
	
	//if ( ImageAPI.AverageImage2Frame(ImageW[0], ImageH[0], RGBImageStep, 24, RGBImagePtr1, RGBImagePtr2, m_FrameImagePtr) == false )
	if ( ImageAPI.MaxValueImage4Frame(ImageW[0], ImageH[0], RGBImageStep, 24, RGBImagePtr1, RGBImagePtr2, RGBImagePtr3, RGBImagePtr4, m_FrameImagePtr) == false )
	{
		SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr10->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr11->ClearSliceImageBuffer();//刪除影像記憶體
		SlicePtr12->ClearSliceImageBuffer();//刪除影像記憶體

		SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
		SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);
		JetMemory.free_func(RGBImagePtr1);
		JetMemory.free_func(RGBImagePtr2);
		JetMemory.free_func(RGBImagePtr3);
		JetMemory.free_func(RGBImagePtr4);
		return false;
	}
	
	const double Precision = DBL_PRECISION;
	const double FrameSatRed = GetFrameSaturationRed();
	const double FrameSatGrn = GetFrameSaturationGreen();
	const double FrameSatBlu = GetFrameSaturationBlue();
	if ( fabs(FrameSatRed-1.0)>Precision || fabs(FrameSatGrn-1.0)>Precision || fabs(FrameSatBlu-1.0)>Precision )
	{	ImageAPI.SaturateColorImage3(ImageW[0], ImageH[0], RGBImageStep, m_FrameImagePtr, m_FrameImagePtr, FrameSatRed, FrameSatBlu);	}

	JetMemory.free_func(RGBImagePtr1);
	JetMemory.free_func(RGBImagePtr2);
	JetMemory.free_func(RGBImagePtr3);
	JetMemory.free_func(RGBImagePtr4);
	m_FrameImageW = ImageW[0];
	m_FrameImageH = ImageH[0];
	m_FrameImageStep = RGBImageStep;
	m_FrameImageBitCount = 24;	

	SlicePtr1->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr2->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr3->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr4->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr5->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr6->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr7->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr8->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr9->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr10->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr11->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr12->ClearSliceImageBuffer();//刪除影像記憶體
	SlicePtr1->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr2->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr3->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr4->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr5->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr6->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr7->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr8->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr9->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr10->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr11->SetSliceFillState(SLICE_FILL_CLEAR);
	SlicePtr12->SetSliceFillState(SLICE_FILL_CLEAR);

#ifdef _DEBUG
	//CString str;
	//str.Format(_T("%s\\Frame#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), this->GetFrameIndex()+1);
	//ImageAPI.SaveImage(str, m_FrameImageW, m_FrameImageH, m_FrameImageStep, m_FrameImageBitCount, m_FrameImagePtr, false);
	//CAOIFrame::ClearFrameBuffer();
#endif	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ExecFrameMergeSlice_Space()//影像合併區域影像-3D資料
{	
	SLICE_FUNC_MODE SliceFuncMode = GetFrameSliceFuncMode();		
	const bool bSepareDLP2ExpTable = AOIDataCollect.CheckUseSeparateDLP2ExpTable(SliceFuncMode);

	if ( true == bSepareDLP2ExpTable )
	{
		if ( ReSortFrameSliceList() == false )
		{	return false; }
	}	

	//return CAOIFrame::ExecFrameMergeSlice_Space_Separate();
	return CAOIFrame::ExecFrameMergeSlice_Space_Combine();
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ExecFrameMergeSlice_Space_Combine()//影像合併區域影像-3D資料
{
	const char fnName[] = "CAOIFrame::ExecFrameMergeSlice_Space_Combine";
	bool   bOK = true;
	CString str;
	BOOL    bSave = FALSE;
	IMAGE_SIZE   i = 0;
	IMAGE_SIZE   PhaseCastCount = 0;	
	const size_t Ptr3DCount = 30;	
	const size_t FrameIndex = GetFrameIndex();	
	const size_t FieldIndex = GetFrameFieldIndex();	
	const int    LevelMode = 0;	
	SLICE_FUNC_MODE SliceFuncMode = GetFrameSliceFuncMode();
	const int  FrameExpCount = AOIDataCollect.CheckFrameExpCountBySliceFuncMode(SliceFuncMode);
	const int  FrameLightCount = AOIDataCollect.CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
	LIGHT_3D_CAST_ID Light3DID=LIGHT_3D_CAST_00;

	CAOISlice    *SlicePtr = NULL; 
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	IMAGE_SIZE    BitCount = 0;
	int           PtrCountA=0, PtrCountB=0, PtrCountC=0, PtrCountD=0;	
	IMAGE_PTR     ImagePtr = NULL;		
	IMAGE_PTR     PtrA[Ptr3DCount]={NULL};//第1投燈, 最多3週期x5步x2曝光, 共30張
	IMAGE_PTR     PtrB[Ptr3DCount]={NULL};//第2投燈, 最多3週期x5步x2曝光, 共30張
	IMAGE_PTR     PtrC[Ptr3DCount]={NULL};//第3投燈, 最多3週期x5步x2曝光, 共30張
	IMAGE_PTR     PtrD[Ptr3DCount]={NULL};//第4投燈, 最多3週期x5步x2曝光, 共30張	
	IMAGE_PTR     MaskPtrA=NULL;
	IMAGE_PTR     MaskPtrB=NULL;
	IMAGE_PTR     MaskPtrC=NULL;
	IMAGE_PTR     MaskPtrD=NULL;
	PHASE_PTR     PhasePtrA=NULL;
	PHASE_PTR     PhasePtrB=NULL;
	PHASE_PTR     PhasePtrC=NULL;
	PHASE_PTR     PhasePtrD=NULL;
	SPACE_PTR     SpacePtrA=NULL;
	SPACE_PTR     SpacePtrB=NULL;
	SPACE_PTR     SpacePtrC=NULL;
	SPACE_PTR     SpacePtrD=NULL;
	IMAGE_SIZE    ImageWA[Ptr3DCount]={0};
	IMAGE_SIZE    ImageWB[Ptr3DCount]={0};
	IMAGE_SIZE    ImageWC[Ptr3DCount]={0};
	IMAGE_SIZE    ImageWD[Ptr3DCount]={0};
	IMAGE_SIZE    ImageHA[Ptr3DCount]={0};
	IMAGE_SIZE    ImageHB[Ptr3DCount]={0};
	IMAGE_SIZE    ImageHC[Ptr3DCount]={0};
	IMAGE_SIZE    ImageHD[Ptr3DCount]={0};
	IMAGE_SIZE    ImageStepA[Ptr3DCount]={0};
	IMAGE_SIZE    ImageStepB[Ptr3DCount]={0};
	IMAGE_SIZE    ImageStepC[Ptr3DCount]={0};
	IMAGE_SIZE    ImageStepD[Ptr3DCount]={0};
	TPhaseNoiseParam NoiseParam;
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);

	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	for ( i=0; i<SliceCount; i++ )
	{
		SlicePtr = this->GetFrameSlicePtr(i, false);
		if ( NULL == SlicePtr ) { continue; }

		Light3DID  = SlicePtr->GetSliceLight3DCastID();
		if ( LIGHT_3D_CAST_00 == Light3DID ) { continue; }
		if ( SlicePtr->GetSliceImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false ) { continue; }
		if ( NULL == ImagePtr ) { continue; }
	
		switch ( Light3DID )
		{
		case LIGHT_3D_CAST_01:
			if ( PtrCountA < Ptr3DCount )
			{	
				PtrA[PtrCountA] = ImagePtr;	
				ImageWA[PtrCountA] = ImageW;	
				ImageHA[PtrCountA] = ImageH;	
				ImageStepA[PtrCountA] = ImageStep;	
				PtrCountA ++;
			}
			break;
		case LIGHT_3D_CAST_02:
			if ( PtrCountB < Ptr3DCount )
			{	
				PtrB[PtrCountB] = ImagePtr;	
				ImageWB[PtrCountB] = ImageW;	
				ImageHB[PtrCountB] = ImageH;	
				ImageStepB[PtrCountB] = ImageStep;	
				PtrCountB ++;
			}
			break;
		case LIGHT_3D_CAST_03:
			if ( PtrCountC < Ptr3DCount )
			{	
				PtrC[PtrCountC] = ImagePtr;	
				ImageWC[PtrCountC] = ImageW;	
				ImageHC[PtrCountC] = ImageH;	
				ImageStepC[PtrCountC] = ImageStep;	
				PtrCountC ++;
			}
			break;
		case LIGHT_3D_CAST_04:
			if ( PtrCountD < Ptr3DCount )
			{	
				PtrD[PtrCountD] = ImagePtr;	
				ImageWD[PtrCountD] = ImageW;	
				ImageHD[PtrCountD] = ImageH;	
				ImageStepD[PtrCountD] = ImageStep;	
				PtrCountD ++;
			}
			break;
		case LIGHT_3D_CAST_05:
			break;
		case LIGHT_3D_CAST_06:
			break;
		case LIGHT_3D_CAST_07:
			break;
		case LIGHT_3D_CAST_08:
			break;
		}		
	}	
	
	bool bIsOK = true;
	unsigned int CastIdx = 0;
	const size_t MaxCastCount = 8;
	TCastParam CastParam[MaxCastCount];	
	CastIdx = 0;
	if ( PtrCountA > 0 )
	{	
		LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_01;
		if ( 2 == FrameExpCount )
		{	bIsOK = BuildCastParam2Exp(ImageWA[0], ImageHA[0], ImageStepA[0], PtrCountA, SliceFuncMode, PtrA, CastID, CastParam[CastIdx]);	}
		else
		{	bIsOK = BuildCastParam(ImageWA[0], ImageHA[0], ImageStepA[0], PtrCountA, SliceFuncMode, PtrA, CastID, CastParam[CastIdx]);	}
		if ( false == bIsOK )
		{
			for ( i=0; i<CastIdx; i++ )
			{	ReleaseCastParam(CastParam[i]);	}
			return false;
		}
		CastIdx ++;
	}
	if ( PtrCountB > 0 )
	{	
		LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_02;
		if ( 2 == FrameExpCount )
		{	bIsOK = BuildCastParam2Exp(ImageWB[0], ImageHB[0], ImageStepB[0], PtrCountB, SliceFuncMode, PtrB, CastID, CastParam[CastIdx]);	}
		else
		{	bIsOK = BuildCastParam(ImageWB[0], ImageHB[0], ImageStepB[0], PtrCountB, SliceFuncMode, PtrB, CastID, CastParam[CastIdx]);	}
		if ( false == bIsOK )
		{
			for ( i=0; i<CastIdx; i++ )
			{	ReleaseCastParam(CastParam[i]);	}
			return false;
		}
		CastIdx ++;
	}
	if ( PtrCountC > 0 )
	{	
		LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_03;
		if ( 2 == FrameExpCount )
		{	bIsOK = BuildCastParam2Exp(ImageWC[0], ImageHC[0], ImageStepC[0], PtrCountC, SliceFuncMode, PtrC, CastID, CastParam[CastIdx]);	}
		else
		{	bIsOK = BuildCastParam(ImageWC[0], ImageHC[0], ImageStepC[0], PtrCountC, SliceFuncMode, PtrC, CastID, CastParam[CastIdx]);	}
		if ( false == bIsOK )
		{
			for ( i=0; i<CastIdx; i++ )
			{	ReleaseCastParam(CastParam[i]);	}
			return false;
		}
		CastIdx ++;
	}
	if ( PtrCountD > 0 )
	{	
		LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_04;
		if ( 2 == FrameExpCount )
		{	bIsOK = BuildCastParam2Exp(ImageWD[0], ImageHD[0], ImageStepD[0], PtrCountD, SliceFuncMode, PtrD, CastID, CastParam[CastIdx]);	}
		else
		{	bIsOK = BuildCastParam(ImageWD[0], ImageHD[0], ImageStepD[0], PtrCountD, SliceFuncMode, PtrD, CastID, CastParam[CastIdx]);	}
		if ( false == bIsOK )
		{
			for ( i=0; i<CastIdx; i++ )
			{	ReleaseCastParam(CastParam[i]);	}
			return false;
		}
		CastIdx ++;
	}
/*
	//測試用第三個Cast
	if ( PtrCountA > 0 )
	{	
		LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_01;
		if ( BuildCastParam(ImageWA[0], ImageHA[0], ImageStepA[0], PtrCountA, SliceFuncMode, PtrA, CastID, CastParam[CastIdx]) == false )
		{	return false; }
		CastIdx ++;
	}
	//測試用第四個Cast
	if ( PtrCountB > 0 )
	{	
		LIGHT_3D_CAST_ID CastID = LIGHT_3D_CAST_02;
		if ( BuildCastParam(ImageWB[0], ImageHB[0], ImageStepB[0], PtrCountB, SliceFuncMode, PtrB, CastID, CastParam[CastIdx]) == false )
		{	return false; }
		CastIdx ++;
	}
	*/
	if ( 0 == CastIdx ) 
	{	return false; }
	
	if ( CastIdx > 1 )
	{
		for ( i=0; i<CastIdx-1; i++ )
		{
			if ( CastParam[i].ImageW != CastParam[i+1].ImageW ) 
			{
				bIsOK = false;
				break;
			}
			if ( CastParam[i].ImageH != CastParam[i+1].ImageH ) 
			{
				bIsOK = false;
				break;
			}
			if ( CastParam[i].ImageStep != CastParam[i+1].ImageStep ) 
			{
				bIsOK = false;
				break;
			}
		}
	}
	if ( false == bIsOK )
	{
		for ( i=0; i<MaxCastCount; i++ )
		{	ReleaseCastParam(CastParam[i]);	}		
		return false;
	}
	
	bool bOpenMP = false;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, m_FrameMaskPtr, fnName, "m_FrameMaskPtr") == false || 
	     JetMemory.alloc_func(BufferSize, m_FrameSpacePtr, fnName, "m_FrameSpacePtr") == false )
	{
		for ( i=0; i<MaxCastCount; i++ )
		{	ReleaseCastParam(CastParam[i]);	}
		JetMemory.free_func(m_FrameMaskPtr);
		JetMemory.free_func(m_FrameSpacePtr);
		return false;
	}

	if ( 2 == FrameExpCount )
	{
		switch ( CastIdx )
		{
		case 1:	bIsOK = ImageAPI.PatternCast1ToSpace2Exp3(ImageW, ImageH, ImageStep, CastParam[0], NoiseParam, bOpenMP, m_FrameMaskPtr, m_FrameSpacePtr);	break;
		case 2:	bIsOK = ImageAPI.PatternCast2ToSpace2Exp3(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], NoiseParam, bOpenMP, m_FrameMaskPtr, m_FrameSpacePtr);	break;
		case 3:	bIsOK = ImageAPI.PatternCast3ToSpace2Exp3(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], CastParam[2], NoiseParam, bOpenMP, m_FrameMaskPtr, m_FrameSpacePtr);	break;
		case 4:	bIsOK = ImageAPI.PatternCast4ToSpace2Exp3(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], CastParam[2], CastParam[3], NoiseParam, bOpenMP, m_FrameMaskPtr, m_FrameSpacePtr);	break;	
		default:
			return false;
			break;
		}
	}
	else
	{
		switch ( CastIdx )
		{
		case 1:	bIsOK = ImageAPI.PatternCast1ToSpace3(ImageW, ImageH, ImageStep, CastParam[0], NoiseParam, bOpenMP, m_FrameMaskPtr, m_FrameSpacePtr);	break;
		case 2:	bIsOK = ImageAPI.PatternCast2ToSpace3(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], NoiseParam, bOpenMP, m_FrameMaskPtr, m_FrameSpacePtr);	break;
		case 3:	bIsOK = ImageAPI.PatternCast3ToSpace3(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], CastParam[2], NoiseParam, bOpenMP, m_FrameMaskPtr, m_FrameSpacePtr);	break;
		case 4:	bIsOK = ImageAPI.PatternCast4ToSpace3(ImageW, ImageH, ImageStep, CastParam[0], CastParam[1], CastParam[2], CastParam[3], NoiseParam, bOpenMP, m_FrameMaskPtr, m_FrameSpacePtr);	break;	
		default:
			return false;
			break;
		}
	}
	if ( false == bIsOK )
	{
		for ( i=0; i<MaxCastCount; i++ )
		{	ReleaseCastParam(CastParam[i]);	}
		JetMemory.free_func(m_FrameMaskPtr);
		JetMemory.free_func(m_FrameSpacePtr);
		return false; 
	}

	m_FrameImageW = ImageW;//Frame影像寬度
	m_FrameImageH = ImageH;//Frame影像長度
	m_FrameImageStep = ImageStep;//Frame影像步長
	m_FrameImageBitCount = 8;//Frame影像位元數

	const float SpaceOffset=GetFraemSpaceOffset();
	if ( fabs(SpaceOffset) > 0.001 )
	{
		for ( i=0; i<BufferSize; i++ )
		{	m_FrameSpacePtr[i] += SpaceOffset;	}
	}

	for ( i=0; i<MaxCastCount; i++ )
	{	ReleaseCastParam(CastParam[i]);	}	
	ExecFrameMergeSlice_SpaceEnd();	
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		IMAGE_PTR  SpaceImagePtr = NULL;
		if ( NULL != m_FrameSpacePtr )
		{
			RECT RoiRect={0, 0, 0, 0};
			const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
			RoiRect.left = 0;
			RoiRect.top = 0;
			RoiRect.right = (int)(ImageW);
			RoiRect.bottom = (int)(ImageH);
			str.Format(_T("%s\\%s#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), _T("Frame3D[Full]"), FrameIndex+1);
			ImageAPI.SpaceGrayImageConvertToGray(ImageW, ImageW, ImageStep, m_FrameSpacePtr, m_FrameMaskPtr, RoiRect, ImageStep, SpaceImagePtr, SpaceRatio, false);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, SpaceImagePtr, true);
			JetMemory.free_func(SpaceImagePtr);
		}
	//	if ( NULL != m_FrameMaskPtr )
	//	{
	//		str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("FrameMsk[Full]"), FrameIndex+1);
	//		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, 8, m_FrameMaskPtr, true);
	//	}
	}	
#endif//_DEBUG

	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergeSlice_Space_Separate()//影像合併區域影像-3D資料
{
	const char fnName[] = "CAOIFrame::ExecFrameMergeSlice_Space_Separate";
	bool   bOK = true;
	CString str;
	BOOL    bSave = FALSE;
	IMAGE_SIZE   i = 0;	
	IMAGE_SIZE   PhaseCastCount = 0;	
	int PhaseMode = LIGHT3D_PHASE_4_4_M;
	const size_t Ptr3DCount = 30;	
	const size_t FrameIndex = GetFrameIndex();	
	const size_t FieldIndex = GetFrameFieldIndex();	
	const int    LevelMode = 0;
	LIGHT_3D_CAST_ID Light3DID=LIGHT_3D_CAST_00;

	CAOISlice    *SlicePtr = NULL; 
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	IMAGE_SIZE    BitCount = 0;
	unsigned int  PtrCountA=0, PtrCountB=0, PtrCountC=0, PtrCountD=0;	
	IMAGE_PTR     ImagePtr = NULL;		
	IMAGE_PTR     PtrA[Ptr3DCount]={NULL};//第1投燈, 最多3週期x5步, 共15張
	IMAGE_PTR     PtrB[Ptr3DCount]={NULL};//第2投燈, 最多3週期x5步, 共15張
	IMAGE_PTR     PtrC[Ptr3DCount]={NULL};//第3投燈, 最多3週期x5步, 共15張
	IMAGE_PTR     PtrD[Ptr3DCount]={NULL};//第4投燈, 最多3週期x5步, 共15張	
	IMAGE_PTR     MaskPtrA=NULL;
	IMAGE_PTR     MaskPtrB=NULL;
	IMAGE_PTR     MaskPtrC=NULL;
	IMAGE_PTR     MaskPtrD=NULL;
	PHASE_PTR     PhasePtrA=NULL;
	PHASE_PTR     PhasePtrB=NULL;
	PHASE_PTR     PhasePtrC=NULL;
	PHASE_PTR     PhasePtrD=NULL;
	SPACE_PTR     SpacePtrA=NULL;
	SPACE_PTR     SpacePtrB=NULL;
	SPACE_PTR     SpacePtrC=NULL;
	SPACE_PTR     SpacePtrD=NULL;
	IMAGE_SIZE    ImageWA[Ptr3DCount]={0};
	IMAGE_SIZE    ImageWB[Ptr3DCount]={0};
	IMAGE_SIZE    ImageWC[Ptr3DCount]={0};
	IMAGE_SIZE    ImageWD[Ptr3DCount]={0};
	IMAGE_SIZE    ImageHA[Ptr3DCount]={0};
	IMAGE_SIZE    ImageHB[Ptr3DCount]={0};
	IMAGE_SIZE    ImageHC[Ptr3DCount]={0};
	IMAGE_SIZE    ImageHD[Ptr3DCount]={0};
	IMAGE_SIZE    ImageStepA[Ptr3DCount]={0};
	IMAGE_SIZE    ImageStepB[Ptr3DCount]={0};
	IMAGE_SIZE    ImageStepC[Ptr3DCount]={0};
	IMAGE_SIZE    ImageStepD[Ptr3DCount]={0};	
	TPhaseNoiseParam NoiseParam;	
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);

	const int   nSmoothFilterSize = NoiseParam.PhaseSmoothFilter;
	const BOOL  bCheckSmoothFilter = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_SMOOTH_FILTER);

	const size_t SliceCount = CAOIFrame::GetFrameSliceCount();
	for ( i=0; i<SliceCount; i++ )
	{
		SlicePtr = this->GetFrameSlicePtr(i, false);
		if ( NULL == SlicePtr ) { continue; }

		Light3DID  = SlicePtr->GetSliceLight3DCastID();
		if ( LIGHT_3D_CAST_00 == Light3DID ) { continue; }
		if ( SlicePtr->GetSliceImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false ) { continue; }
		if ( NULL == ImagePtr ) { continue; }
	
		switch ( Light3DID )
		{
		case LIGHT_3D_CAST_01:
			if ( PtrCountA < Ptr3DCount )
			{	
				PtrA[PtrCountA] = ImagePtr;	
				ImageWA[PtrCountA] = ImageW;	
				ImageHA[PtrCountA] = ImageH;	
				ImageStepA[PtrCountA] = ImageStep;	
				PtrCountA ++;
			}
			break;
		case LIGHT_3D_CAST_02:
			if ( PtrCountB < Ptr3DCount )
			{	
				PtrB[PtrCountB] = ImagePtr;	
				ImageWB[PtrCountB] = ImageW;	
				ImageHB[PtrCountB] = ImageH;	
				ImageStepB[PtrCountB] = ImageStep;	
				PtrCountB ++;
			}
			break;
		case LIGHT_3D_CAST_03:
			if ( PtrCountC < Ptr3DCount )
			{	
				PtrC[PtrCountC] = ImagePtr;	
				ImageWC[PtrCountC] = ImageW;	
				ImageHC[PtrCountC] = ImageH;	
				ImageStepC[PtrCountC] = ImageStep;	
				PtrCountC ++;
			}
			break;
		case LIGHT_3D_CAST_04:
			if ( PtrCountD < Ptr3DCount )
			{	
				PtrD[PtrCountD] = ImagePtr;	
				ImageWD[PtrCountD] = ImageW;	
				ImageHD[PtrCountD] = ImageH;	
				ImageStepD[PtrCountD] = ImageStep;	
				PtrCountD ++;
			}
			break;
		case LIGHT_3D_CAST_05:
			break;
		case LIGHT_3D_CAST_06:
			break;
		case LIGHT_3D_CAST_07:
			break;
		case LIGHT_3D_CAST_08:
			break;
		}		
	}	
	
#ifdef _DEBUG		
	BOOL bSaveRaw = FALSE;
	if ( TRUE == bSaveRaw )
	{		
		for ( i=0; i<PtrCountA; i++ )
		{
			if ( NULL == PtrA[i] ) { continue; }			
			str.Format(_T("%s\\Frame#%dPatternA[%d].BMP"), AOIDataCollect.GetAOITempDirectory(), FrameIndex+1, i);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, PtrA[i], true);					
		}
		for ( i=0; i<PtrCountB; i++ )
		{
			if ( NULL == PtrB[i] ) { continue; }						
			str.Format(_T("%s\\Frame#%dPatternB[%d].BMP"), AOIDataCollect.GetAOITempDirectory(), FrameIndex+1, i);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, PtrB[i], true);			
		}
		for ( i=0; i<PtrCountC; i++ )
		{
			if ( NULL == PtrC[i] ) { continue; }			
			str.Format(_T("%s\\Frame#%dPatternC[%d].JPG"), AOIDataCollect.GetAOITempDirectory(), FrameIndex+1, i);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, PtrC[i], true);			
		}
		for ( i=0; i<PtrCountD; i++ )
		{
			if ( NULL == PtrD[i] ) { continue; }			
			str.Format(_T("%s\\Frame#%dPatternD[%d].JPG"), AOIDataCollect.GetAOITempDirectory(), FrameIndex+1, i);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, PtrD[i], true);			
		}
	}	
#endif//_DEBUG

	PhaseCastCount = 0;	
	const int DLPLEDColor = AOIDataCollect.GetSystemDlpLedColor();
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();	
	const int PhaseConvertHeightMode = SysParam.m_PhaseConvertHeightMode;	
	//第1個投光
	if ( PtrCountA > 0 )
	{
		int    PatternID= 0;
		double PeriodA  = 0;
		double PeriodA1 = SysParam.m_PhasePeriod1;
		double PeriodA2 = SysParam.m_PhasePeriod2;
		double PeriodA3 = SysParam.m_PhasePeriod3;
		ImageW = ImageWA[0];
		ImageH = ImageHA[0];
		ImageStep = ImageStepA[0];
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, MaskPtrA, fnName, "MaskPtrA")==false ||
		     JetMemory.alloc_func(BufferSize, PhasePtrA, fnName, "PhasePtrA") == false )
		{
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			return false;
		}
		//::memset(MaskPtrA, 0x00, sizeof(MASK_DATA)*BufferSize);
		PHASE_PTR PtrBasePhase = NULL;
		IMAGE_SIZE PhaseW=0, PhaseH=0, PhaseStep=0;
		Light3DCtrl.GetLight3DPhaseZero(LIGHT_3D_CAST_01, PhaseMode, DLPLEDColor, PhaseW, PhaseH, PhaseStep, PtrBasePhase);		

		double Gamma=1.0;
		Light3DCtrl.GetLight3DImageGamma(LIGHT_3D_CAST_01, Gamma);
		//ImageAPI.SwapImagePtr(PtrA, PtrCountA, false);//將圖像次序相反	
		switch ( PtrCountA )
		{
		case 3://1週期3步長			
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage3FrameToPhase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA);
			break;
		case 4://1週期4步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage4FrameToPhase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PtrA[3], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA);
			break;
		case 5://1週期5步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage5FrameToPhase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PtrA[3], PtrA[4], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA);
			break;
		case 6://2週期3步長
			bOK = ImageAPI.GrayImage3FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PeriodA1, PtrA[3], PtrA[4], PtrA[5], PeriodA2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA, PeriodA);
			break;
		case 8://2週期4步長
			bOK = ImageAPI.GrayImage4FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PtrA[3], PeriodA1, PtrA[4], PtrA[5], PtrA[6], PtrA[7], PeriodA2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA, PeriodA);
			break;
		case 9://3週期3步長
			bOK = ImageAPI.GrayImage3FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PeriodA1, PtrA[3], PtrA[4], PtrA[5], PeriodA2, PtrA[6], PtrA[7], PtrA[8], PeriodA3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA, PeriodA);
			break;
		case 10://2週期5步長
			bOK = ImageAPI.GrayImage5FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PtrA[3], PtrA[4], PeriodA1, PtrA[5], PtrA[6], PtrA[7], PtrA[8], PtrA[9], PeriodA2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA, PeriodA);
			break;
		case 12://3週期4步長
			bOK = ImageAPI.GrayImage4FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PtrA[3], PeriodA1, PtrA[4], PtrA[5], PtrA[6], PtrA[7], PeriodA2, PtrA[8], PtrA[9], PtrA[10], PtrA[11], PeriodA3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA, PeriodA);
			break;
		case 15://3週期5步長
			bOK = ImageAPI.GrayImage5FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrA[0], PtrA[1], PtrA[2], PtrA[3], PtrA[4], PeriodA1, PtrA[5], PtrA[6], PtrA[7], PtrA[8], PtrA[9], PeriodA2, PtrA[10], PtrA[11], PtrA[12], PtrA[13], PtrA[14], PeriodA3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrA, PhasePtrA, PeriodA);
			break;
		}
		if ( false == bOK )
		{	
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			return false;
		}

		//將相位影像轉成空間相對位置
		if ( ExecFramePhaseToSpace(PhaseConvertHeightMode, LIGHT_3D_CAST_01, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, PhasePtrA, MaskPtrA, SpacePtrA) == false )
		{
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			return false;
		}		
		PhaseCastCount ++;
	}

	//第2個投光
	if ( PtrCountB > 0 )
	{
		int    PatternID= 0;
		double PeriodB  = 0;
		double PeriodB1 = SysParam.m_PhasePeriod1;
		double PeriodB2 = SysParam.m_PhasePeriod2;
		double PeriodB3 = SysParam.m_PhasePeriod3;
		ImageW = ImageWB[0];
		ImageH = ImageHB[0];
		ImageStep = ImageStepB[0];
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, MaskPtrB, fnName, "MaskPtrB") == false || 
		     JetMemory.alloc_func(BufferSize, PhasePtrB, fnName, "PhasePtrB") == false )
		{
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			return false;
		}
		//::memset(MaskPtrB, 0x00, sizeof(MASK_DATA)*BufferSize);

		PHASE_PTR PtrBasePhase = NULL;
		IMAGE_SIZE PhaseW=0, PhaseH=0, PhaseStep=0;
		Light3DCtrl.GetLight3DPhaseZero(LIGHT_3D_CAST_02, PhaseMode, DLPLEDColor, PhaseW, PhaseH, PhaseStep, PtrBasePhase);

		double Gamma=1.0;
		Light3DCtrl.GetLight3DImageGamma(LIGHT_3D_CAST_02, Gamma);
		//ImageAPI.SwapImagePtr(PtrB, PtrCountB, false);//將圖像次序相反	
		switch ( PtrCountB )
		{
		case 3://1週期3步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage3FrameToPhase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB);
			break;
		case 4://1週期4步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage4FrameToPhase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PtrB[3], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB);
			break;
		case 5://1週期5步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage5FrameToPhase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PtrB[3], PtrB[4], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB);
			break;
		case 6://2週期3步長
			bOK = ImageAPI.GrayImage3FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PeriodB1, PtrB[3], PtrB[4], PtrB[5], PeriodB2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB, PeriodB);
			break;
		case 8://2週期4步長
			bOK = ImageAPI.GrayImage4FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PtrB[3], PeriodB1, PtrB[4], PtrB[5], PtrB[6], PtrB[7], PeriodB2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB, PeriodB);
			break;
		case 9://3週期3步長
			bOK = ImageAPI.GrayImage3FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PeriodB1, PtrB[3], PtrB[4], PtrB[5], PeriodB2, PtrB[6], PtrB[7], PtrB[8], PeriodB3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB, PeriodB);
			break;
		case 10://2週期5步長
			bOK = ImageAPI.GrayImage5FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PtrB[3], PtrB[4], PeriodB1, PtrB[5], PtrB[6], PtrB[7], PtrB[8], PtrB[9], PeriodB2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB, PeriodB);
			break;
		case 12://3週期4步長
			bOK = ImageAPI.GrayImage4FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PtrB[3], PeriodB1, PtrB[4], PtrB[5], PtrB[6], PtrB[7], PeriodB2, PtrB[8], PtrB[9], PtrB[10], PtrB[11], PeriodB3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB, PeriodB);
			break;
		case 15://3週期5步長
			bOK = ImageAPI.GrayImage5FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrB[0], PtrB[1], PtrB[2], PtrB[3], PtrB[4], PeriodB1, PtrB[5], PtrB[6], PtrB[7], PtrB[8], PtrB[9], PeriodB2, PtrB[10], PtrB[11], PtrB[12], PtrB[13], PtrB[14], PeriodB3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrB, PhasePtrB, PeriodB);
			break;
		}
		if ( false == bOK )
		{	
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			return false;
		}

		//將相位影像轉成空間相對位置
		if ( ExecFramePhaseToSpace(PhaseConvertHeightMode, LIGHT_3D_CAST_02, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, PhasePtrB, MaskPtrB, SpacePtrB) == false )
		{
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			return false;
		}
		PhaseCastCount ++;
	}

	//第3個投光
	if ( PtrCountC > 0 )
	{
		int    PatternID= 0;
		double PeriodC  = 0;
		double PeriodC1 = SysParam.m_PhasePeriod1;
		double PeriodC2 = SysParam.m_PhasePeriod2;
		double PeriodC3 = SysParam.m_PhasePeriod3;
		ImageW = ImageWC[0];
		ImageH = ImageHC[0];
		ImageStep = ImageStepC[0];
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, MaskPtrC, fnName, "MaskPtrC") == false ||
		     JetMemory.alloc_func(BufferSize, PhasePtrC, fnName, "PhasePtrC") == false )
		{
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			JetMemory.free_func(SpacePtrB);
			JetMemory.free_func(MaskPtrC);
			JetMemory.free_func(PhasePtrC);
			return false;
		}
		//::memset(MaskPtrC, 0x00, sizeof(MASK_DATA)*BufferSize);

		PHASE_PTR PtrBasePhase = NULL;
		IMAGE_SIZE PhaseW=0, PhaseH=0, PhaseStep=0;
		Light3DCtrl.GetLight3DPhaseZero(LIGHT_3D_CAST_03, PhaseMode, DLPLEDColor, PhaseW, PhaseH, PhaseStep, PtrBasePhase);

		double Gamma=1.0;
		Light3DCtrl.GetLight3DImageGamma(LIGHT_3D_CAST_03, Gamma);
		//ImageAPI.SwapImagePtr(PtrC, PtrCountC, false);//將圖像次序相反	
		switch ( PtrCountC )
		{
		case 3://1週期3步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage3FrameToPhase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC);
			break;
		case 4://1週期4步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage4FrameToPhase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PtrC[3], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC);
			break;
		case 5://1週期5步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage5FrameToPhase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PtrC[3], PtrC[4], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC);
			break;
		case 6://2週期3步長
			bOK = ImageAPI.GrayImage3FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PeriodC1, PtrC[3], PtrC[4], PtrC[5], PeriodC2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC, PeriodC);
			break;
		case 8://2週期4步長
			bOK = ImageAPI.GrayImage4FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PtrC[3], PeriodC1, PtrC[4], PtrC[5], PtrC[6], PtrC[7], PeriodC2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC, PeriodC);
			break;
		case 9://3週期3步長
			bOK = ImageAPI.GrayImage3FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PeriodC1, PtrC[3], PtrC[4], PtrC[5], PeriodC2, PtrC[6], PtrC[7], PtrC[8], PeriodC3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC, PeriodC);
			break;
		case 10://2週期5步長
			bOK = ImageAPI.GrayImage5FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PtrC[3], PtrC[4], PeriodC1, PtrC[5], PtrC[6], PtrC[7], PtrC[8], PtrC[9], PeriodC2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC, PeriodC);
			break;
		case 12://3週期4步長
			bOK = ImageAPI.GrayImage4FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PtrC[3], PeriodC1, PtrC[4], PtrC[5], PtrC[6], PtrC[7], PeriodC2, PtrC[8], PtrC[9], PtrC[10], PtrC[11], PeriodC3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC, PeriodC);
			break;
		case 15://3週期5步長
			bOK = ImageAPI.GrayImage5FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrC[0], PtrC[1], PtrC[2], PtrC[3], PtrC[4], PeriodC1, PtrC[5], PtrC[6], PtrC[7], PtrC[8], PtrC[9], PeriodC2, PtrC[10], PtrC[11], PtrC[12], PtrC[13], PtrC[14], PeriodC3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrC, PhasePtrC, PeriodC);
			break;
		}
		if ( false == bOK )
		{	
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			JetMemory.free_func(SpacePtrB);
			JetMemory.free_func(MaskPtrC);
			JetMemory.free_func(PhasePtrC);
			return false;
		}
		//將相位影像轉成空間相對位置
		if ( ExecFramePhaseToSpace(PhaseConvertHeightMode, LIGHT_3D_CAST_03, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, PhasePtrC, MaskPtrC, SpacePtrC) == false )
		{
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			JetMemory.free_func(SpacePtrB);
			JetMemory.free_func(MaskPtrC);
			JetMemory.free_func(PhasePtrC);
			return false;
		}		
		PhaseCastCount ++;
	}

	//第4個投光
	if ( PtrCountD > 0 )
	{
		int    PatternID= 0;
		double PeriodD  = 0;
		double PeriodD1 = SysParam.m_PhasePeriod1;
		double PeriodD2 = SysParam.m_PhasePeriod2;
		double PeriodD3 = SysParam.m_PhasePeriod3;
		ImageW = ImageWD[0];
		ImageH = ImageHD[0];
		ImageStep = ImageStepD[0];
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, MaskPtrD, fnName, "MaskPtrD") == false ||
		     JetMemory.alloc_func(BufferSize, PhasePtrD, fnName, "PhasePtrD") == false )
		{
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			JetMemory.free_func(SpacePtrB);
			JetMemory.free_func(MaskPtrC);
			JetMemory.free_func(PhasePtrC);
			JetMemory.free_func(SpacePtrC);
			JetMemory.free_func(MaskPtrD);
			JetMemory.free_func(PhasePtrD);
			return false;
		}
		//::memset(MaskPtrD, 0x00, sizeof(MASK_DATA)*BufferSize);

		PHASE_PTR PtrBasePhase = NULL;
		IMAGE_SIZE PhaseW=0, PhaseH=0, PhaseStep=0;
		Light3DCtrl.GetLight3DPhaseZero(LIGHT_3D_CAST_04, PhaseMode, DLPLEDColor, PhaseW, PhaseH, PhaseStep, PtrBasePhase);

		double Gamma=1.0;
		Light3DCtrl.GetLight3DImageGamma(LIGHT_3D_CAST_04, Gamma);
		//ImageAPI.SwapImagePtr(PtrD, PtrCountD, false);//將圖像次序相反	
		switch ( PtrCountD )
		{
		case 3://1週期3步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage3FrameToPhase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD);
			break;
		case 4://1週期4步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage4FrameToPhase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PtrD[3], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD);
			break;
		case 5://1週期5步長
			PatternID = PHASE_PATTERN_A;
			bOK = ImageAPI.GrayImage5FrameToPhase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PtrD[3], PtrD[4], PatternID, Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD);
			break;
		case 6://2週期3步長
			bOK = ImageAPI.GrayImage3FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PeriodD1, PtrD[3], PtrD[4], PtrD[5], PeriodD2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD, PeriodD);
			break;
		case 8://2週期4步長
			bOK = ImageAPI.GrayImage4FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PtrD[3], PeriodD1, PtrD[4], PtrD[5], PtrD[6], PtrD[7], PeriodD2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD, PeriodD);
			break;
		case 9://3週期3步長
			bOK = ImageAPI.GrayImage3FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PeriodD1, PtrD[3], PtrD[4], PtrD[5], PeriodD2, PtrD[6], PtrD[7], PtrD[8], PeriodD3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD, PeriodD);
			break;
		case 10://2週期5步長
			bOK = ImageAPI.GrayImage5FrameTo2Phase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PtrD[3], PtrD[4], PeriodD1, PtrD[5], PtrD[6], PtrD[7], PtrD[8], PtrD[9], PeriodD2,  Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD, PeriodD);
			break;
		case 12://3週期4步長
			bOK = ImageAPI.GrayImage4FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PtrD[3], PeriodD1, PtrD[4], PtrD[5], PtrD[6], PtrD[7], PeriodD2, PtrD[8], PtrD[9], PtrD[10], PtrD[11], PeriodD3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD, PeriodD);
			break;
		case 15://3週期5步長
			bOK = ImageAPI.GrayImage5FrameTo3Phase3(ImageW, ImageH, ImageStep, PtrD[0], PtrD[1], PtrD[2], PtrD[3], PtrD[4], PeriodD1, PtrD[5], PtrD[6], PtrD[7], PtrD[8], PtrD[9], PeriodD2, PtrD[10], PtrD[11], PtrD[12], PtrD[13], PtrD[14], PeriodD3,  Gamma, PtrBasePhase, NoiseParam, MaskPtrD, PhasePtrD, PeriodD);
			break;
		}
		if ( false == bOK )
		{	
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			JetMemory.free_func(SpacePtrB);
			JetMemory.free_func(MaskPtrC);
			JetMemory.free_func(PhasePtrC);
			JetMemory.free_func(SpacePtrC);
			JetMemory.free_func(MaskPtrD);
			JetMemory.free_func(PhasePtrD);
			return false;
		}

		//將相位影像轉成空間相對位置
		if ( ExecFramePhaseToSpace(PhaseConvertHeightMode, LIGHT_3D_CAST_04, PhaseMode, DLPLEDColor, ImageW, ImageH, ImageStep, PhasePtrD, MaskPtrD, SpacePtrD) == false )
		{
			JetMemory.free_func(MaskPtrA);
			JetMemory.free_func(PhasePtrA);
			JetMemory.free_func(SpacePtrA);
			JetMemory.free_func(MaskPtrB);
			JetMemory.free_func(PhasePtrB);
			JetMemory.free_func(SpacePtrB);
			JetMemory.free_func(MaskPtrC);
			JetMemory.free_func(PhasePtrC);
			JetMemory.free_func(SpacePtrC);
			JetMemory.free_func(MaskPtrD);
			JetMemory.free_func(PhasePtrD);
			return false;
		}		
		PhaseCastCount ++;
	}	
	JetMemory.free_func(PhasePtrA);		
	JetMemory.free_func(PhasePtrB);		
	JetMemory.free_func(PhasePtrC);		
	JetMemory.free_func(PhasePtrD);	
	
	//提前歸還Slice的指標, 所以後面不能再使用PtrA, PtrB, PtrC, PtrD的影像
	ExecFrameMergeSlice_SpaceEnd();	

#ifdef _DEBUG		
	if ( TRUE == bSave )
	{
		RECT RoiRect={0, 0, 0, 0};			
		IMAGE_PTR  SpaceImagePtr = NULL;		
		RoiRect.left = 0;
		RoiRect.top = 0;
		RoiRect.right = (int)(ImageW);
		RoiRect.bottom = (int)(ImageH);
		if ( NULL != SpacePtrA )
		{	
			str.Format(_T("%s\\%s#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), _T("Frame3D[1]"), FrameIndex+1);
			ImageAPI.SpaceGrayImageConvertToGray(ImageW, ImageW, ImageStep, SpacePtrA, MaskPtrA, RoiRect, ImageStep, SpaceImagePtr, SpaceRatio, false);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, SpaceImagePtr, true);
			JetMemory.free_func(SpaceImagePtr);
		}
		if ( NULL != MaskPtrA )
		{
			str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("FrameMsk[1]"), FrameIndex+1);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, MaskPtrA, true);
		}
		if ( NULL != SpacePtrB )
		{
			str.Format(_T("%s\\%s#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), _T("Frame3D[2]"), FrameIndex+1);		
			ImageAPI.SpaceGrayImageConvertToGray(ImageW, ImageW, ImageStep, SpacePtrB, MaskPtrB, RoiRect, ImageStep, SpaceImagePtr, SpaceRatio, false);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, SpaceImagePtr, true);
			JetMemory.free_func(SpaceImagePtr);
		}
		if ( NULL != MaskPtrB )
		{
			str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("FrameMsk[2]"), FrameIndex+1);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, MaskPtrB, true);
		}		
		if ( NULL != SpacePtrC )
		{
			str.Format(_T("%s\\%s#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), _T("Frame3D[3]"), FrameIndex+1);
			ImageAPI.SpaceGrayImageConvertToGray(ImageW, ImageW, ImageStep, SpacePtrC, MaskPtrC, RoiRect, ImageStep, SpaceImagePtr, SpaceRatio, false);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, SpaceImagePtr, true);
			JetMemory.free_func(SpaceImagePtr);
		}
		if ( NULL != MaskPtrC )
		{
			str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("FrameMsk[3]"), FrameIndex+1);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, MaskPtrC, true);
		}
		if ( NULL != SpacePtrD )
		{
			str.Format(_T("%s\\%s#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), _T("Frame3D[4]"), FrameIndex+1);
			ImageAPI.SpaceGrayImageConvertToGray(ImageW, ImageW, ImageStep, SpacePtrD, MaskPtrD, RoiRect, ImageStep, SpaceImagePtr, SpaceRatio, false);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, SpaceImagePtr, true);
			JetMemory.free_func(SpaceImagePtr);
		}
		if ( NULL != MaskPtrD )
		{
			str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("FrameMsk[4]"), FrameIndex+1);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, MaskPtrD, true);
		}
	}	
#endif //_DEBUG

	if ( 0 == PhaseCastCount )
	{
		JetMemory.free_func(MaskPtrA);
		JetMemory.free_func(SpacePtrA);
		JetMemory.free_func(MaskPtrB);
		JetMemory.free_func(SpacePtrB);
		JetMemory.free_func(MaskPtrC);
		JetMemory.free_func(SpacePtrC);
		JetMemory.free_func(MaskPtrD);
		JetMemory.free_func(SpacePtrD);
		return false;
	}
	if ( 1 == PhaseCastCount )
	{
		if ( NULL != SpacePtrA )
		{
			m_FrameMaskPtr = MaskPtrA;
			m_FrameSpacePtr = SpacePtrA;
		}
		if ( NULL != SpacePtrB )
		{
			m_FrameMaskPtr = MaskPtrB;
			m_FrameSpacePtr = SpacePtrB;
		}
		if ( NULL != SpacePtrC )
		{
			m_FrameMaskPtr = MaskPtrC;
			m_FrameSpacePtr = SpacePtrC;
		}
		if ( NULL != SpacePtrD )
		{
			m_FrameMaskPtr = MaskPtrD;
			m_FrameSpacePtr = SpacePtrD;
		}
		m_FrameImageW = ImageW;//Frame影像寬度
		m_FrameImageH = ImageH;//Frame影像長度
		m_FrameImageStep = ImageStep;//Frame影像步長
		m_FrameImageBitCount = 8;//Frame影像位元數			
		return true;
	}

	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, m_FrameMaskPtr, fnName, "m_FrameMaskPtr") == false || 
	     JetMemory.alloc_func(BufferSize, m_FrameSpacePtr, fnName, "m_FrameSpacePtr") == false )
	{
		JetMemory.free_func(MaskPtrA);
		JetMemory.free_func(SpacePtrA);
		JetMemory.free_func(MaskPtrB);
		JetMemory.free_func(SpacePtrB);
		JetMemory.free_func(MaskPtrC);
		JetMemory.free_func(SpacePtrC);
		JetMemory.free_func(MaskPtrD);
		JetMemory.free_func(SpacePtrD);

		JetMemory.free_func(m_FrameMaskPtr);
		JetMemory.free_func(m_FrameSpacePtr);
		return false;
	}

	size_t idx = 0;	
	MASK_PTR  TmpMaskPtr[4]={NULL};
	SPACE_PTR TmpSpacePtr[4]={NULL};

	idx = 0;	
	if ( NULL != SpacePtrA )
	{
		TmpMaskPtr[idx] = MaskPtrA;
		TmpSpacePtr[idx] = SpacePtrA;
		idx ++;
	}
	if ( NULL != SpacePtrB )
	{
		TmpMaskPtr[idx] = MaskPtrB;
		TmpSpacePtr[idx] = SpacePtrB;
		idx ++;
	}
	if ( NULL != SpacePtrC )
	{
		TmpMaskPtr[idx] = MaskPtrC;
		TmpSpacePtr[idx] = SpacePtrC;
		idx ++;
	}
	if ( NULL != SpacePtrD )
	{
		TmpMaskPtr[idx] = MaskPtrD;
		TmpSpacePtr[idx] = SpacePtrD;
		idx ++;
	}

	const BOOL  bCheckVoidExpaned = (BOOL)(NoiseParam.PhaseNoiseDef&PHASE_NOSIE_VOID_EXPAND);
	const int   nVoidExpandSize = NoiseParam.PhaseExtendVoid*2+1;	
	if ( FALSE!=bCheckVoidExpaned && nVoidExpandSize > 0 )
	{		
		const int nVoidExpandIterCount = 1;
		const size_t MaskBufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		for ( i=0; i<PhaseCastCount; i++ )
		{
			if ( NULL == TmpMaskPtr[i] ) { continue; }
			if ( ImageAPI.DilateNoiseMaskImage3(ImageW, ImageH, ImageStep, TmpMaskPtr[i], nVoidExpandSize, nVoidExpandIterCount, m_FrameMaskPtr) == false )
			{	continue;	}
			::memcpy(TmpMaskPtr[i], m_FrameMaskPtr, sizeof(MASK_DATA)*MaskBufferSize);			
		}
	}

	bool bOpenMP = false;
	switch ( PhaseCastCount )
	{
	case 2:
		bOK = ImageAPI.Merge2SpaceImage3(ImageW, ImageH, ImageStep, TmpSpacePtr[0], TmpMaskPtr[0], TmpSpacePtr[1], TmpMaskPtr[1], bOpenMP, m_FrameSpacePtr, m_FrameMaskPtr);
		break;
	case 3:
		bOK = ImageAPI.Merge3SpaceImage3(ImageW, ImageH, ImageStep, TmpSpacePtr[0], TmpMaskPtr[0], TmpSpacePtr[1], TmpMaskPtr[1], TmpSpacePtr[2], TmpMaskPtr[2], bOpenMP, m_FrameSpacePtr, m_FrameMaskPtr);
		break;
	case 4:
		bOK = ImageAPI.Merge4SpaceImage3(ImageW, ImageH, ImageStep, TmpSpacePtr[0], TmpMaskPtr[0], TmpSpacePtr[1], TmpMaskPtr[1], TmpSpacePtr[2], TmpMaskPtr[2], TmpSpacePtr[3], TmpMaskPtr[3], bOpenMP, m_FrameSpacePtr, m_FrameMaskPtr);
		break;
	}
	if ( false == bOK )
	{
		JetMemory.free_func(MaskPtrA);
		JetMemory.free_func(SpacePtrA);
		JetMemory.free_func(MaskPtrB);
		JetMemory.free_func(SpacePtrB);
		JetMemory.free_func(MaskPtrC);
		JetMemory.free_func(SpacePtrC);
		JetMemory.free_func(MaskPtrD);
		JetMemory.free_func(SpacePtrD);

		JetMemory.free_func(m_FrameMaskPtr);
		JetMemory.free_func(m_FrameSpacePtr);
		return false;
	}	

	JetMemory.free_func(MaskPtrA);
	JetMemory.free_func(SpacePtrA);
	JetMemory.free_func(MaskPtrB);
	JetMemory.free_func(SpacePtrB);
	JetMemory.free_func(MaskPtrC);
	JetMemory.free_func(SpacePtrC);
	JetMemory.free_func(MaskPtrD);
	JetMemory.free_func(SpacePtrD);

	m_FrameImageW = ImageW;//Frame影像寬度
	m_FrameImageH = ImageH;//Frame影像長度
	m_FrameImageStep = ImageStep;//Frame影像步長
	m_FrameImageBitCount = 8;//Frame影像位元數

	const float SpaceOffset=GetFraemSpaceOffset();
	if ( fabs(SpaceOffset) > 0.001 )
	{
		for ( i=0; i<BufferSize; i++ )
		{	m_FrameSpacePtr[i] += SpaceOffset;	}
	}


#ifdef _DEBUG	
	bSave = TRUE;
	if ( TRUE == bSave )
	{
		RECT RoiRect={0, 0, 0, 0};			
		IMAGE_PTR  SpaceImagePtr = NULL;
		RoiRect.left = 0;
		RoiRect.top = 0;
		RoiRect.right = (int)(ImageW);
		RoiRect.bottom = (int)(ImageH);
		if ( NULL != m_FrameSpacePtr )
		{
			str.Format(_T("%s\\%s#%d.JPG"), AOIDataCollect.GetAOITempDirectory(), _T("Frame3D[Full]"), FrameIndex+1);
			ImageAPI.SpaceGrayImageConvertToGray(ImageW, ImageW, ImageStep, m_FrameSpacePtr, m_FrameMaskPtr, RoiRect, ImageStep, SpaceImagePtr, SpaceRatio, false);
			ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, SpaceImagePtr, true);
			JetMemory.free_func(SpaceImagePtr);
		}
		if ( NULL != m_FrameMaskPtr )
		{
			str.Format(_T("%s\\%s#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), _T("FrameMsk[Full]"), FrameIndex+1);
		//	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, m_FrameMaskPtr, true);
		}
	}
#endif //_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//	
inline bool CAOIFrame::ExecFrameMergeSlice_SpaceEnd()//影像合併區域影像-歸還Field的影像資料
{
	size_t i = 0;
	CAOISlice *SlicePtr = NULL;
	const size_t SliceCount = GetFrameSliceCount();
	for ( i=0; i<SliceCount; i++ )
	{
		SlicePtr = this->GetFrameSlicePtr(i, false);
		if ( NULL == SlicePtr ) { continue; }
		SlicePtr->ClearSliceImageBuffer();
		SlicePtr->SetSliceFillState(SLICE_FILL_CLEAR);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergePatch()//影像合併區域影像-灰階
{
	bool IsOK = true;	
	switch ( m_FrameType )
	{
	case FRAME_GRAY:
		IsOK = CAOIFrame::ExecFrameMergePatch_Gray();
		break;
	case FRAME_BAYER:
		IsOK = CAOIFrame::ExecFrameMergePatch_Bayer();
		break;
	case FRAME_COLOR:
		IsOK = CAOIFrame::ExecFrameMergePatch_Color();
		break;
	case FRAME_SPACE:
		IsOK = CAOIFrame::ExecFrameMergePatch_Space();
		if ( false == IsOK )
		{	CAOIFrame::ExecFrameMergePatch_SpaceEnd(); }
		break;
	}
	SetFrameImageValid(IsOK);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergePatch_Gray()//影像合併區域影像-灰階
{
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergePatch_Bayer()//影像合併區域影像-彩色
{
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergePatch_Color()//影像合併區域影像-彩色	
{
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergePatch_Space()//影像合併區域影像-3D資料
{
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFrame::ExecFrameMergePatch_SpaceEnd()//影像合併區域影像-3D資料
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ReSortFrameSliceList_Kernel(std::vector<CAOISlice*> &List)//重新排序Frame影像列表
{
	size_t       i=0, j=0;
	LIGHT_3D_CAST_ID CastID;
	CAOISlice   *SlicePtr=NULL;	
	std::vector<CAOISlice*> TempSlicePtrList;
	const size_t FrameSliceCount=List.size();
	for ( j=0; j<DLP_CAST_COUNT; j++ )
	{
		CastID = CLight3DCtrl::GetLight3DCastIDByIndex(j);
		if ( LIGHT_3D_CAST_00 == CastID ) { continue; }

		for ( i=0; i<FrameSliceCount; i++ )
		{
			SlicePtr = List[i];
			if ( NULL == SlicePtr ) { continue; }
			if ( CastID != SlicePtr->GetSliceLight3DCastID() ) { continue; }
			TempSlicePtrList.push_back(SlicePtr);
		}
	}
	const size_t TempSliceCount=TempSlicePtrList.size();	
	if ( TempSliceCount != FrameSliceCount )
	{	return false; }	
	List = TempSlicePtrList;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ExecFrameSaveRawImage()//影像儲存原始圖檔
{
	SetFrameSaveRawImageDone(false);
	if ( AOIDataCollect.GetSaveProjectRawImage() == false )
	{	return true; }

	CString RawFolder=AOIDataCollect.GetSaveProjectRawImageFolder();
	if ( RawFolder.GetLength() == 0 )
	{	return true; }	
	ONLINE_STATE_MODE OnlineStateMode = AOIDataCollect.GetOnlineStateMode();
	if ( ONLINE_STATE_PROJECT_MAP == OnlineStateMode ) { return true; }
	if ( ONLINE_STATE_PROJECT_MARK == OnlineStateMode ) { return true; }
	if ( ONLINE_STATE_PROJECT_OPEN_CODE == OnlineStateMode ) { return true; }
	if ( ONLINE_STATE_INSPECT_FD_PANEL == OnlineStateMode ) { return true; }
	if ( ONLINE_STATE_INSPECT_FD_BOARD == OnlineStateMode ) { return true; }
	if ( ONLINE_STATE_INSPECT_BARCODE == OnlineStateMode ) { return true; }
	
	CString FovFolder=RawFolder;	
	CAOIField *FieldPtr=GetFrameFieldPtr();
	if ( NULL == FieldPtr ) { return true; }	
	CString FieldName=FieldPtr->GetFieldIndexName();		
	FovFolder.Format(_T("%s\\%s"), RawFolder, FieldName);
	::CreateDirectory(FovFolder, NULL);
	if ( DumpFrameSliceImageList(FovFolder, true) == false )
	{	return true;	}
	SetFrameSaveRawImageDone(true);
	return true;
}
bool CAOIFrame::WriteFrameFile(CAOIFileIO &FileIO)//儲存區域影像檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	unsigned int FieldIndex = 0;
	CString    str;
	CAOIFrame *FramePtr = this;
	char       uuidStr[MAX_JET_PATH]="";	
	wchar_t    uuidWStr[MAX_JET_PATH]=L"";	
	UUID       uuid = FramePtr->GetObjUuid();
	FileIO.SetFnName(_T("CAOIFrame::WriteFrameFile"));	
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//	
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_START, 0) == false ) { return false; }

	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_FRAME_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_FRAME_OBJ_UUID, uuidStr) == false ) { return false; } }
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_INDEX, FramePtr->GetFrameIndex()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_FIELD_INDEX, FramePtr->GetFrameFieldIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_TYPE, FramePtr->GetFrameType()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_CAMERA_ID, FramePtr->GetFrameCameraID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_LIGHT_MODE, FramePtr->GetFrameLightMode()) == false ) { return false; }

	str = FramePtr->GetFrameFileName();	
	if ( FileIO.GetSaveWStr() == true ) 
	{	
		JetAPI::TCHAR2wchar(str, uuidWStr, MAX_JET_PATH);		
		if ( FileIO.SaveChunk_STR(FILE_IO_FRAME_FILE_NAME, uuidWStr) == false ) { return false; } 
	}
	else
	{	
		JetAPI::TCHAR2char(str, uuidStr, MAX_JET_PATH);
		if ( FileIO.SaveChunk_STR(FILE_IO_FRAME_FILE_NAME, uuidStr) == false ) { return false; } 
	}
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_DISTRICT_ID, FramePtr->GetFrameDistrictID()) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_FRAME_SATURATION_RED, FramePtr->GetFrameSaturationRed()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FRAME_SATURATION_GREEN, FramePtr->GetFrameSaturationGreen()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FRAME_SATURATION_BLUE, FramePtr->GetFrameSaturationBlue()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_BAYER_PATTERN, FramePtr->GetFrameBayerPattern()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_UNIQUE_ID, FramePtr->GetFrameUniqueID()) == false ) { return false; }		
	
	if ( FileIO.SaveChunk_INT(FILE_IO_FRAME_END, 0) == false ) { return false; }
	//----------------------------------------------------------------------------------------//		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ReadFrameFile(CAOIFileIO &FileIO)//載入區域影像檔案
{	
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	//----------------------------------------------------------------------------------------//
	UUID       uuid;
	CString    str;
	int        index = 0;
	int        nValue=0;
	int        PanelIndex = 0;		
	double     dValue = 0;	
	CAOIFrame *FramePtr = this;	
	FileIO.SetFnName(_T("CAOIFrame::ReadFrameFile"));
	//----------------------------------------------------------------------------------------//	
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_FRAME_START://區域影像參數-起點
			break;
		case FILE_IO_FRAME_END://區域影像參數-終點			
			return true;
			break;

		case FILE_IO_FRAME_OBJ_UUID://區域影像參數-的OBJ-UUID
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	FramePtr->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	FramePtr->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_FRAME_INDEX://區域影像參數-編號
			nValue = FileIO.GetData_INT();
			FramePtr->SetFrameIndex(nValue);
			FramePtr->SetFrameIndexOffline(nValue);
			break;
		case FILE_IO_FRAME_FIELD_INDEX://區域影像參數-區域編號
			FramePtr->SetFrameFieldIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_FRAME_TYPE://區域影像參數-影像樣式
			nValue = FileIO.GetData_INT();
			FramePtr->SetFrameType((FRAME_TYPE)nValue);
			break;
		case FILE_IO_FRAME_CAMERA_ID://區域影像參數-相機編號
			nValue = FileIO.GetData_INT();
			FramePtr->SetFrameCameraID((CAMERA_ID)nValue);
			break;
		case FILE_IO_FRAME_LIGHT_MODE://區域影像參數-燈源模式
			nValue = FileIO.GetData_INT();
			FramePtr->SetFrameLightMode((LIGHT_MODE)nValue);
			break;
		case FILE_IO_FRAME_FILE_NAME://區域影像參數-檔案名稱
			if ( FileIO.GetLoadWStr()==true )
			{	str = FileIO.GetData_WSTR();	}
			else
			{	str = FileIO.GetData_STR(); }
			FramePtr->SetFrameFileName(str);
			FramePtr->SetFrameFileNameOffline(str);
			break;
		case FILE_IO_FRAME_DISTRICT_ID://區域影像參數-分段編號
			FramePtr->SetFrameDistrictID((DISTRICT_ID)(FileIO.GetData_INT()));
			break;
		case FILE_IO_FRAME_SATURATION_RED://區域影像參數-飽和調整-紅色
			FramePtr->SetFrameSaturationRed(FileIO.GetData_DBL());
			break;
		case FILE_IO_FRAME_SATURATION_GREEN://區域影像參數-飽和調整-綠色
			FramePtr->SetFrameSaturationGreen(FileIO.GetData_DBL());			
			break;
		case FILE_IO_FRAME_SATURATION_BLUE://區域影像參數-飽和調整-藍色
			FramePtr->SetFrameSaturationBlue(FileIO.GetData_DBL());
			break;			
		case FILE_IO_FRAME_BAYER_PATTERN://區域影像參數-Bayer樣板
			FramePtr->SetFrameBayerPattern((BAYER_PATTERN_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_FRAME_UNIQUE_ID://區域影像參數-唯一碼
			FramePtr->SetFrameUniqueID(FileIO.GetData_INT());
			break;
		default:
			break;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::ExecFramePhaseToSpace(int CvtMode, LIGHT_3D_CAST_ID CastID, int PhaseMode, int DLPLEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, SPACE_PTR SpacePtr)
{	
	if ( PHASE_CONVERT_HEIGHT_SCALE == CvtMode )
	{		
		IMAGE_SIZE  PhaseW=0;
		IMAGE_SIZE  PhaseH=0;
		IMAGE_SIZE  PhaseStep=0;		
		SPACE_PTR KPtr = NULL;	
		Light3DCtrl.GetLight3DPhaseFactor(CastID, PhaseMode, DLPLEDColor, PhaseW, PhaseH, PhaseStep, KPtr);
		if ( ImageAPI.PhaseImageToSpaceImage(ImageW, ImageH, ImageStep, PhasePtr, KPtr, MaskPtr, SpacePtr) == false )
		{	return false;	}
	}
	else
	{		
		const int SX=0, SY=0;
		double HeightFactor0[HEIGHT_FACTOR_PARAM_COUNT];
		double HeightFactor1[HEIGHT_FACTOR_PARAM_COUNT];
		double HeightFactor2[HEIGHT_FACTOR_PARAM_COUNT];
		Light3DCtrl.GetLight3DHeightFactor(CastID, HeightFactor0, HeightFactor1, HeightFactor2);	
		if ( ImageAPI.PhaseImageToSpaceImageByMapping(CvtMode, ImageW, ImageH, ImageStep, PhasePtr, SX, SY, HeightFactor0, HeightFactor1, HeightFactor2, MaskPtr, SpacePtr) == false )
		{	return false;	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::DumpFrameSliceImageList(LPCTSTR Folder, bool bUseDlpName)//匯出影像單張影像列表
{
	size_t       i=0;
	CString      str;
	CString      strFolder;
	IMAGE_SIZE   ImageW=0;
	IMAGE_SIZE   ImageH=0;
	IMAGE_SIZE   BitCount=0;
	IMAGE_SIZE   ImageStep=0;
	IMAGE_PTR    ImagePtr=NULL;
	CAOISlice   *SlicePtr=NULL;		
	int          CastIndex=0;
	LIGHT_3D_CAST_ID CastID;
	const size_t FrameIndex=GetFrameIndex();
	const size_t SliceCount=GetFrameSliceCount();	
	int          DlpImageCount[DLP_CAST_COUNT]={0};
	
	strFolder.Format(_T("%s\\Frame_%04d"), Folder, FrameIndex+1);
	::CreateDirectory(strFolder, NULL);
	::memset(DlpImageCount, 0x00, sizeof(DlpImageCount));

	for ( i=0; i<SliceCount; i++ )
	{
		SlicePtr = GetFrameSlicePtr(i, false);
		if ( NULL == SlicePtr ) { continue; }
		if ( SlicePtr->GetSliceImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
		{	continue; }
		if ( NULL == ImagePtr ) { continue; }
		//str.Format(_T("%s\\Frame_%04d#%02d.PNG"), Folder, FrameIndex+1, i+1);
		str.Format(_T("%s\\Slice_%04d.PNG"), strFolder, i+1);
		if ( true == bUseDlpName )
		{
			CastID = SlicePtr->GetSliceLight3DCastID();		
			if ( LIGHT_3D_CAST_00 != CastID )		
			{
				CastIndex = Light3DCtrl.GetLight3DIndexByCastID(CastID);
				if ( CastIndex>=0 && CastIndex<DLP_CAST_COUNT )
				{	
					str.Format(_T("%s\\DLP%02d_%04d.PNG"), strFolder, CastIndex+1, DlpImageCount[CastIndex]+1);	
					DlpImageCount[CastIndex] ++;
				}
			}
		}
		ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIFrame::GetFrameIndexName() const//取得Frame引數名稱
{
	CString IndexName;
	IndexName.Format(_T("Frame_%04d"), GetFrameIndex()+1);
	return IndexName;
}
//-------------------------------------------------------------------------------------//
bool CAOIFrame::GetFrameSaveRawImageDone() const//取得影像畫面原圖以儲存
{
	return m_FrameSaveRawImageDone;
}
//-------------------------------------------------------------------------------------//
void CAOIFrame::SetFrameSaveRawImageDone(bool val)//設定影像畫面原圖以儲存
{
	m_FrameSaveRawImageDone = val;
}
//-------------------------------------------------------------------------------------//