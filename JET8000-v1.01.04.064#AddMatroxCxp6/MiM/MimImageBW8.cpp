// MimImageBW8.cpp: implementation of the CMimImageBW8 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MimImageBW8.h"
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
int  CMimImageBW8::CalcRowPitch(int ImageW)
{
	int AlignByte = 4;	//4 for eVision, 32, for Open eVision	
	int RealImageX = ImageW;
	int RowPitch = RealImageX;
	if ( (RealImageX%AlignByte) == 0 )
	{	RowPitch = RealImageX; }
	else
	{	RowPitch = ((RealImageX/AlignByte)+1)*AlignByte;	}	
	return RowPitch;
}
//------------------------------------------------------------------//
CMimImageBW8::CMimImageBW8()
{
	CMimImageBW8::PreInitImageBW8();
	CMimImageBW8::InitialImageBW8();
}
//-------------------------------------------------------------------------------------//
CMimImageBW8::CMimImageBW8(const CMimImageBW8 &Img)
{
	CMimImageBW8::PreInitImageBW8();
	CMimImageBW8::CloneImageBW8(Img);
}
//-------------------------------------------------------------------------------------//
CMimImageBW8::~CMimImageBW8()
{	
#ifdef MIM_LIB_USE
	DestroyiImage(m_ImageBW8Ptr);
	m_ImageBW8Ptr = NULL;	
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
CMimImageBW8& CMimImageBW8::operator=(const CMimImageBW8 &Img)
{
	if ( this == &Img ) { return *this; }
	CMimImageBW8::CloneImageBW8(Img);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CMimImageBW8::PreInitImageBW8()
{
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));		
#ifdef MIM_LIB_USE
	m_ImageBW8Ptr = CreateGrayiImage();	
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
void CMimImageBW8::InitialImageBW8()
{
#ifdef MIM_LIB_USE
	m_MimErrCode = E__OK;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
void CMimImageBW8::CloneImageBW8(const CMimImageBW8 &Img)
{
	::strcpy(m_ErrorString, Img.m_ErrorString);
#ifdef MIM_LIB_USE
	CMimImageBW8::m_MimErrCode = Img.m_MimErrCode;
	m_MimErrCode = iImageCopy(m_ImageBW8Ptr, Img.m_ImageBW8Ptr);	
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
inline bool CMimImageBW8::CheckMimImage()
{
#ifdef MIM_LIB_USE	
	m_MimErrCode = iImageIsNULL(m_ImageBW8Ptr);
	if ( E_NULL == m_MimErrCode ) 
	{
		char *pBuf = iGetErrorText(E_NULL);
		::strcpy(m_ErrorString, pBuf);
		return false; 
	}
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CMimImageBW8::CheckMimImage() const
{
#ifdef MIM_LIB_USE		
	E_iVision_ERRORS Err = iImageIsNULL(m_ImageBW8Ptr);
	if ( E_NULL == Err ) 
	{	return false; }
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMimImageBW8::GetIsMimError()
{
#ifdef MIM_LIB_USE
	if ( E__OK != m_MimErrCode ) 
	{
		char *pBuf = iGetErrorText(m_MimErrCode);
		::strcpy(m_ErrorString, pBuf);
		return true;
	}
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
#ifdef MIM_LIB_USE
//-------------------------------------------------------------------------------------//
LONG_PTR  CMimImageBW8::GetImageBW8Ptr()
{
	return m_ImageBW8Ptr;	
}
//-------------------------------------------------------------------------------------//
bool CMimImageBW8::CheckImageBW8Ptr()
{
	return CMimImageBW8::CheckMimImage();
}
//-------------------------------------------------------------------------------------//
#endif//MIM_LIB_USE
//-------------------------------------------------------------------------------------//
const char* CMimImageBW8::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CMimImageBW8::SetImagePtr(const unsigned char *pImage, int ImageW, int ImageH, int BytesPerLine, bool reAlloc)
{
	const char fnName[] = "CMimImageBW8::SetImagePtr";	
#ifdef MIM_LIB_USE
	if ( CMimImageBW8::CheckMimImage() == false ) { return false; }
	if( NULL == pImage )
	{
		sprintf(this->m_ErrorString, "Set Image = NULL !");
		return false;
	}
	if( (ImageW<=0) || (ImageH<=0) )	
	{
		sprintf(this->m_ErrorString, "Image Size <= 0 !");
		return false;
	}	
	
	m_MimErrCode = iImageResize(m_ImageBW8Ptr, ImageW, ImageH);
	if ( GetIsMimError() == true ) 
	{	return false;	}		
	
	m_MimErrCode = iPointerToiImage(m_ImageBW8Ptr, (LONG_PTR)pImage, BytesPerLine, ImageH);
	if ( GetIsMimError() == true ) 
	{	return false;	}	

#ifdef _DEBUG
	int nRowPitch = 0;
	int nColPitch = 0;
	iGetCameraAlignSize(m_ImageBW8Ptr, &nRowPitch, &nColPitch);		
#endif
	return true;
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimImageBW8::LoadImage(const char *pFileName)
{
#ifdef MIM_LIB_USE
	if ( CMimImageBW8::CheckMimImage() == false ) { return false; }
	char filename[MAX_JET_PATH]="";
	::strcpy(filename, pFileName);	
	m_MimErrCode = iReadImage(m_ImageBW8Ptr, filename);
	if ( GetIsMimError() == true ) 
	{	return false; }	
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMimImageBW8::SaveImage(const char *pFileName)
{
#ifdef MIM_LIB_USE
	if ( CMimImageBW8::CheckMimImage() == false ) { return false; }
	char filename[MAX_JET_PATH]="";
	::strcpy(filename, pFileName);	
	m_MimErrCode = iSaveImage(m_ImageBW8Ptr, filename);
	if ( GetIsMimError() == true ) 
	{	return false; }
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
int CMimImageBW8::GetImageW() const
{
#ifdef MIM_LIB_USE
	if ( CMimImageBW8::CheckMimImage() == false ) { return false; }
	return GetWidth(m_ImageBW8Ptr);	
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
int CMimImageBW8::GetImageH() const
{
#ifdef MIM_LIB_USE
	if ( CMimImageBW8::CheckMimImage() == false ) { return false; }
	return GetHeight(m_ImageBW8Ptr);	
#endif//MIM_LIB_USE
	return 0;
}
//-------------------------------------------------------------------------------------//
int CMimImageBW8::GetImageRowPitch()
{
#ifdef MIM_LIB_USE
	const int ImageW = GetImageW();
	return CMimImageBW8::CalcRowPitch(ImageW);
#endif//MIM_LIB_USE
	return 0;
}
//-------------------------------------------------------------------------------------//
void* CMimImageBW8::GetImagePtr(int &ImageW, int &ImageH, int &RowPitch, int &ColPitch, int &BytesPerLine)
{
#ifdef MIM_LIB_USE
	if ( CMimImageBW8::CheckMimImage() == false ) 
	{ 
		ImageW = ImageH = RowPitch = ColPitch = BytesPerLine = 0;
		return NULL; 
	}
	void *ImagePtr = NULL;//iVarPtr(m_ImageBW8Ptr);
	ImageW = GetWidth(m_ImageBW8Ptr);
	ImageH = GetHeight(m_ImageBW8Ptr);
	m_MimErrCode = iGetCameraAlignSize(m_ImageBW8Ptr, &RowPitch, &ColPitch);	
	if ( GetIsMimError() == true ) 
	{	return NULL; }
	int AlignedBits = 0;
	m_MimErrCode = iGetAlignedBits(&AlignedBits);
	BytesPerLine = RowPitch;	
	return ImagePtr;
#endif//MIM_LIB_USE
	return NULL;	
}
//-------------------------------------------------------------------------------------//
