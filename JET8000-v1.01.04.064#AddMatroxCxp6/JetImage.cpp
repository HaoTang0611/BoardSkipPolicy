// JetImage.cpp: implementation of the CJetImage class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "JetImage.h"
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
CJetImage::CJetImage()
{
	CJetImage::PreInitImage();
}
//-------------------------------------------------------------------------------------//
CJetImage::CJetImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE Bits, IMAGE_PTR Ptr, bool bClone)
{
	CJetImage::PreInitImage();
	CJetImage::SetImage(W, H, Step, Bits, Ptr, bClone);	
}
//-------------------------------------------------------------------------------------//
CJetImage::CJetImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_PTR PtrR, IMAGE_PTR PtrG, IMAGE_PTR PtrB)
{
	CJetImage::PreInitImage();	
	CJetImage::SetRGB(W, H, Step, PtrR, PtrG, PtrB);	
}
//-------------------------------------------------------------------------------------//
CJetImage::CJetImage(const CJetImage &Image)
{
	CJetImage::PreInitImage();	
	CJetImage::SetImage(Image.m_Width, Image.m_Height, Image.m_Step, Image.m_BitCount, Image.m_Ptr, true);
}
//-------------------------------------------------------------------------------------//
CJetImage::CJetImage(const CvMat *ImagePtr)
{
	CJetImage::PreInitImage();
	CJetImage::SetImage(ImagePtr);	
}
//-------------------------------------------------------------------------------------//
CJetImage::CJetImage(const IplImage *ImagePtr)
{
	CJetImage::PreInitImage();
	CJetImage::SetImage(ImagePtr);
}
//-------------------------------------------------------------------------------------//
CJetImage::~CJetImage()
{
	ReleaseImage();
}
//-------------------------------------------------------------------------------------//
CJetImage& CJetImage::operator=(const CJetImage &Image)
{
	if ( this == &Image ) { return *this; }	
	CJetImage::SetImage(Image.m_Width, Image.m_Height, Image.m_Step, Image.m_BitCount, Image.m_Ptr, true);
	//m_Width    = Image.m_Width;
	//m_Height   = Image.m_Height;
	//m_Step     = Image.m_Step;
	//m_BitCount = Image.m_BitCount;
	//m_Ptr      = Image.m_Ptr;
	return *this;
}
//-------------------------------------------------------------------------------------//

CJetImage& CJetImage::operator=(const CvMat *ImagePtr)
{
	CJetImage::ReleaseImage();	
	CJetImage::SetImage(ImagePtr);	
	return *this;
}
//-------------------------------------------------------------------------------------//
CJetImage& CJetImage::operator=(const IplImage *ImagePtr)
{
	CJetImage::ReleaseImage();	
	CJetImage::SetImage(ImagePtr);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CJetImage::PreInitImage()
{
	m_Width = 0;
	m_Height = 0;
	m_Step = 0;
	m_BitCount = 0;
	m_Ptr = NULL;	
}
//-------------------------------------------------------------------------------------//
void CJetImage::ReleaseImage()
{
	JetMemory.free_func(m_Ptr);
	m_Width = 0;
	m_Height = 0;
	m_Step = 0;
	m_BitCount = 0;
}
//-------------------------------------------------------------------------------------//
inline bool CJetImage::CheckSelfImage() const
{
	if ( m_Ptr==NULL || m_Width<=0 || m_Height<=0 || m_Step<=0 || m_BitCount<=0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CJetImage::CheckImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, const IMAGE_PTR Ptr) const
{
	if ( Ptr==NULL || W<=0 || H<=0 || Step<=0 || BitCount<=0 )
	{	return false; }
	IMAGE_SIZE RealW = CJetImage::GetImageRealWidth(W, BitCount);
	if ( Step < RealW ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CJetImage::CheckRGBImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, const IMAGE_PTR PtrR, const IMAGE_PTR PtrG, const IMAGE_PTR PtrB) const
{
	if ( PtrR==NULL || PtrG==NULL || PtrB==NULL || W<=0 || H<=0 || Step<=0 )
	{	return false; }
	IMAGE_SIZE RealW = CJetImage::GetImageRealWidth(W, 8);
	if ( Step < RealW ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline int CJetImage::GetImageChannels(IMAGE_SIZE BitCount) const//取得影像通道數	
{
	if ( 8 == BitCount ) { return 1; }
	if ( 24 == BitCount ) { return 3; }
	return 0;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CJetImage::GetImageRealWidth(IMAGE_SIZE ImageW, IMAGE_SIZE BitCount) const//取得影像實際寬度
{
	if ( 8 == BitCount ) { return ImageW; }
	if ( 24 == BitCount ) { return ImageW*3; }
	return 0;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CJetImage::GetImageAlignedWidth(IMAGE_SIZE ImageW, IMAGE_SIZE BitCount, int Align) const//取得影像對齊寬度
{
	assert((Align & (Align - 1)) == 0); // Align is a power of 2
	switch ( BitCount )
	{
	case 8:  ImageW = ImageW; break;
	case 24: ImageW = ImageW*3; break;
	case 32: ImageW = ImageW*4; break;
	default: ImageW = 0; break;
	}
    return (ImageW + Align-1) & -Align;
}
//-------------------------------------------------------------------------------------//
void CJetImage::ClearImage()
{
	CJetImage::ReleaseImage();
}
//-------------------------------------------------------------------------------------//
bool CJetImage::SetImage(const CvMat *ImagePtr)
{
	if ( NULL == ImagePtr ) { return false; }
	IMAGE_SIZE BitCount=0;
	const int nType = CV_MAT_TYPE(ImagePtr->type);
	const int nDepth = CV_MAT_DEPTH(ImagePtr->type);
	const int nChannels = CV_MAT_CN(ImagePtr->type);	
	switch ( nType )
	{
	case CV_8UC1:	BitCount = 8; break;
	case CV_8SC1:	BitCount = 8; break;
	case CV_8UC3:	BitCount = 24; break;
	case CV_8SC3:	BitCount = 24; break;
	}
	if ( 0 == BitCount ) { return false; }
	IMAGE_PTR Ptr = (IMAGE_PTR)ImagePtr->data.ptr;
	CJetImage::SetImage(ImagePtr->width, ImagePtr->height, ImagePtr->step, BitCount, Ptr, true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::SetImage(const IplImage *ImagePtr)
{
	if ( NULL == ImagePtr ) { return false; }
	IMAGE_SIZE BitCount=0;
	switch ( ImagePtr->nChannels )
	{
	case 1:	BitCount = 8; break;
	case 3:	BitCount = 24; break;
	}
	if ( 0 == BitCount ) { return false; }
	IMAGE_PTR Ptr = (IMAGE_PTR)ImagePtr->imageData;
	CJetImage::SetImage(ImagePtr->width, ImagePtr->height, ImagePtr->widthStep, BitCount, Ptr, true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::GetImage(IMAGE_SIZE &W, IMAGE_SIZE &H, IMAGE_SIZE &Step, IMAGE_SIZE &Bits, IMAGE_PTR &Ptr)
{
	W = m_Width;
	H = m_Height;
	Step = m_Step;
	Bits = m_BitCount;
	Ptr = m_Ptr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::SetImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE Bits, const IMAGE_PTR Ptr, bool bClone)
{
	if ( CJetImage::CheckImage(W, H, Step, Bits, Ptr) == false )
	{	return false; }
	CJetImage::ReleaseImage();
	if ( true == bClone )
	{
		const size_t BufferSize = (size_t)(Step)*(size_t)(H);
		if ( JetMemory.alloc_func(BufferSize, m_Ptr, "CJetImage::SetImage", "m_Ptr") == false ) 
		{	return false; }
		::memcpy(m_Ptr, Ptr, sizeof(IMAGE_DATA)*(BufferSize));
	}
	else
	{	m_Ptr = (IMAGE_PTR)Ptr; }
	m_Width = W;
	m_Height = H;
	m_Step = Step;
	m_BitCount = Bits;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::SetRGB(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, const IMAGE_PTR PtrR, const IMAGE_PTR PtrG, const IMAGE_PTR PtrB)
{
	if ( CJetImage::CheckRGBImage(W, H, Step, PtrR, PtrG, PtrB) == false )
	{	return false; }

	CJetImage::ReleaseImage();
	const IMAGE_SIZE ClrStep = CJetImage::GetImageAlignedWidth(W, 24, 4);
	const IMAGE_SIZE BufferSize = ImageAPI.CalcBufferSize(ClrStep, H);
	if ( JetMemory.alloc_func(BufferSize, m_Ptr, "CJetImage::SetRGB", "m_Ptr") == false ) 
	{	return false; }
	
	IMAGE_SIZE i=0, j=0;
	IMAGE_SIZE srcIdx=0, dstIdx=0;
	for ( i=0; i<H; i++ )
	{
		srcIdx = i*Step;
		dstIdx = i*ClrStep;
		for ( j=0; j<W; j++ )
		{
			m_Ptr[dstIdx] = PtrB[srcIdx];	dstIdx++;
			m_Ptr[dstIdx] = PtrG[srcIdx];	dstIdx++;
			m_Ptr[dstIdx] = PtrR[srcIdx];	dstIdx++;
			srcIdx ++;
		}
	}	
	m_Width = W;
	m_Height = H;
	m_Step = ClrStep;
	m_BitCount = 24;
	return true;
}
//-------------------------------------------------------------------------------------//
IMAGE_PTR CJetImage::GetImagePtr() const
{
	return m_Ptr;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CJetImage::GetImageW() const
{	
	return m_Width;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CJetImage::GetImageH() const
{
	return m_Height;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CJetImage::GetImageStep() const
{
	return m_Step;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CJetImage::GetBitCount() const
{
	return m_BitCount;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::AlignImage(int Align)//重新對齊影像指標
{
	if ( CJetImage::CheckSelfImage() == false ) { return false; }
	const IMAGE_SIZE NewStep = CJetImage::GetImageAlignedWidth(m_Width, m_BitCount, Align);
	if ( NewStep == m_Step ) { return true; }

	IMAGE_PTR Ptr = NULL;
	const IMAGE_SIZE BufferSize = ImageAPI.CalcBufferSize(NewStep, m_Height);
	if ( JetMemory.alloc_func(BufferSize, Ptr, "CJetImage::AlignImage", "Ptr") == false ) 
	{	return false; }

	IMAGE_SIZE i=0, j=0;
	IMAGE_SIZE srcIdx=0, dstIdx=0;
	const IMAGE_SIZE RealWidth = CJetImage::GetImageRealWidth(m_Width, m_BitCount);
	for ( i=0; i<m_Height; i++ )
	{
		srcIdx = i*m_Step;
		dstIdx = i*NewStep;
		::memcpy(&(Ptr[dstIdx]), &(m_Ptr[srcIdx]), sizeof(IMAGE_DATA)*RealWidth);	
	}
	JetMemory.free_func(m_Ptr);
	m_Ptr = Ptr;
	m_Step = NewStep;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::CloneImage(CJetImage &Image) const
{	
	if ( CJetImage::CheckSelfImage() == false ) { return false; }	
	IMAGE_PTR Ptr = NULL;
	const IMAGE_SIZE BufferSize = ImageAPI.CalcBufferSize(m_Step, m_Height);
	if ( JetMemory.alloc_func(BufferSize, Ptr, "CJetImage::CloneImage", "Ptr") == false ) 
	{	return false; }
	
	::memcpy(Ptr, m_Ptr, sizeof(IMAGE_DATA)*BufferSize);
	Image.SetImage(m_Width, m_Height, m_Step, m_BitCount, Ptr, false);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::SaveImage(LPCTSTR pfilename)
{
	const size_t chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);	

	CvMat *cvMatPtr=NULL;
	if ( CJetImage::CloneImage(cvMatPtr) == false ) { return false; }
	int Res = ::cvSaveImage(filename, cvMatPtr);
	::cvReleaseMat(&cvMatPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::LoadImage(LPCTSTR pfilename)
{
	const size_t chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);	

	CJetImage::ReleaseImage();
	CvMat *cvMatPtr=::cvLoadImageM(filename, CV_LOAD_IMAGE_ANYCOLOR);
	if ( NULL == cvMatPtr ) { return false; }
	CJetImage::SetImage(cvMatPtr);
	::cvReleaseMat(&cvMatPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::CloneImage(CvMat *&ImagePtr) const
{
	if ( CJetImage::CheckSelfImage() == false ) { return false; }
	const int  nChannels = CJetImage::GetImageChannels(m_BitCount);
	ImagePtr = ::cvCreateMat(m_Height, m_Width, CV_MAKETYPE(CV_8U,nChannels));	
	if ( NULL == ImagePtr ) {	return false;	}
	IMAGE_SIZE i=0, j=0;
	IMAGE_SIZE srcIdx=0, dstIdx=0;	
	if ( m_Step != ImagePtr->step )
	{
		IMAGE_SIZE RealImageW = CJetImage::GetImageRealWidth(m_Width, m_BitCount);
		const IMAGE_SIZE CopyLen = sizeof(unsigned char)*RealImageW;
		for ( i=0; i<m_Height; i++ )
		{
			srcIdx = i*m_Step;
			dstIdx = i*ImagePtr->step;
			::memcpy(&(ImagePtr->data.ptr[dstIdx]), &(m_Ptr[srcIdx]), CopyLen);
		}
	}
	else
	{
		const IMAGE_SIZE CopyLen = sizeof(unsigned char)*m_Step*m_Height;
		::memcpy(ImagePtr->data.ptr, m_Ptr, CopyLen);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetImage::CloneImage(IplImage *&ImagePtr) const
{	
	if ( CJetImage::CheckSelfImage() == false ) { return false; }
	const int  nChannels = CJetImage::GetImageChannels(m_BitCount);
	ImagePtr = ::cvCreateImage(cvSize(m_Width, m_Height), IPL_DEPTH_8U, nChannels);
	if ( NULL == ImagePtr ) {	return false;	}
	IMAGE_SIZE i=0, j=0;
	IMAGE_SIZE srcIdx=0, dstIdx=0;	
	if ( m_Step != ImagePtr->widthStep )
	{
		IMAGE_SIZE RealImageW = CJetImage::GetImageRealWidth(m_Width, m_BitCount);	
		const IMAGE_SIZE CopyLen = sizeof(unsigned char)*RealImageW;
		for ( i=0; i<m_Height; i++ )
		{
			srcIdx = i*m_Step;
			dstIdx = i*ImagePtr->widthStep;
			::memcpy(&(ImagePtr->imageData[dstIdx]), &(m_Ptr[srcIdx]), CopyLen);
		}
	}
	else
	{
		const IMAGE_SIZE CopyLen = sizeof(unsigned char)*m_Step*m_Height;
		::memcpy(ImagePtr->imageData, m_Ptr, CopyLen);
	}
	return true;
}
//-------------------------------------------------------------------------------------//