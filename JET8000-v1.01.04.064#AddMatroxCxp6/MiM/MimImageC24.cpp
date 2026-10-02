// MimImageC24.cpp: implementation of the CMimImageC24 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MimImageC24.h"
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
int  CMimImageC24::CalcRowPitch(int ImageW)
{
	int AlignByte = 4;	//4 for eVision, 32, for Open eVision	
	int RealImageX = ImageW*3;
	int RowPitch = RealImageX;
	if ( (RealImageX%AlignByte) == 0 )
	{	RowPitch = RealImageX; }
	else
	{	RowPitch = ((RealImageX/AlignByte)+1)*AlignByte;	}	
	return RowPitch;
}
//------------------------------------------------------------------//
CMimImageC24::CMimImageC24()
{
	CMimImageC24::PreInitImageC24();
	CMimImageC24::InitialImageC24();
}
//-------------------------------------------------------------------------------------//
CMimImageC24::CMimImageC24(const CMimImageC24 &Img)
{
	CMimImageC24::PreInitImageC24();
	CMimImageC24::CloneImageC24(Img);
}
//-------------------------------------------------------------------------------------//
CMimImageC24::~CMimImageC24()
{	
#ifdef MIM_LIB_USE
	DestroyiImage(m_ImageC24Ptr);
	m_ImageC24Ptr = NULL;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
CMimImageC24& CMimImageC24::operator=(const CMimImageC24 &Img)
{
	if ( this == &Img ) { return *this; }
	CMimImageC24::CloneImageC24(Img);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CMimImageC24::PreInitImageC24()
{
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));		
#ifdef MIM_LIB_USE
	m_ImageC24Ptr = CreateColoriImage();
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
void CMimImageC24::InitialImageC24()
{
#ifdef MIM_LIB_USE
	m_MimErrCode = E__OK;
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
void CMimImageC24::CloneImageC24(const CMimImageC24 &Img)
{
	::strcpy(m_ErrorString, Img.m_ErrorString);
#ifdef MIM_LIB_USE
	CMimImageC24::m_MimErrCode = Img.m_MimErrCode;
	m_MimErrCode = iImageCopy(m_ImageC24Ptr, Img.m_ImageC24Ptr);	
#endif//MIM_LIB_USE
}
//-------------------------------------------------------------------------------------//
inline bool CMimImageC24::CheckMimImage()
{
#ifdef MIM_LIB_USE	
	m_MimErrCode = iImageIsNULL(m_ImageC24Ptr);
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
inline bool CMimImageC24::CheckMimImage() const
{
#ifdef MIM_LIB_USE	
	E_iVision_ERRORS Err = iImageIsNULL(m_ImageC24Ptr);
	if ( E_NULL == Err ) 
	{	return false; }
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CMimImageC24::GetIsMimError()
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
LONG_PTR  CMimImageC24::GetImageC24Ptr()
{
	return m_ImageC24Ptr;	
}
//-------------------------------------------------------------------------------------//
bool CMimImageC24::CheckImageC24Ptr()
{
	return CMimImageC24::CheckMimImage();
}
//-------------------------------------------------------------------------------------//
#endif//MIM_LIB_USE
//-------------------------------------------------------------------------------------//
const char* CMimImageC24::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CMimImageC24::SetImagePtr(const unsigned char *pImage, int ImageW, int ImageH, int BytesPerLine, bool reAlloc)
{
	const char fnName[] = "CMimImageC24::SetImagePtr";	
#ifdef MIM_LIB_USE
	if ( CMimImageC24::CheckMimImage() == false ) { return false; }
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
	
	m_MimErrCode = iImageResize(m_ImageC24Ptr, ImageW, ImageH);
	if ( GetIsMimError() == true ) 
	{	return false;	}		
	
	m_MimErrCode = iPointerToiImage(m_ImageC24Ptr, (LONG_PTR)pImage, BytesPerLine, ImageH);
	if ( GetIsMimError() == true ) 
	{	return false;	}	

#ifdef _DEBUG
	int nRowPitch = 0;
	int nColPitch = 0;
	iGetCameraAlignSize(m_ImageC24Ptr, &nRowPitch, &nColPitch);		
#endif
	return true;
#endif//MIM_LIB_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMimImageC24::LoadImage(const char *pFileName)
{
#ifdef MIM_LIB_USE
	if ( CMimImageC24::CheckMimImage() == false ) { return false; }
	char filename[MAX_JET_PATH]="";
	::strcpy(filename, pFileName);	
	m_MimErrCode = iReadImage(m_ImageC24Ptr, filename);
	if ( GetIsMimError() == true ) 
	{	return false; }	
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMimImageC24::SaveImage(const char *pFileName)
{
#ifdef MIM_LIB_USE
	if ( CMimImageC24::CheckMimImage() == false ) { return false; }
	char filename[MAX_JET_PATH]="";
	::strcpy(filename, pFileName);	
	m_MimErrCode = iSaveImage(m_ImageC24Ptr, filename);
	if ( GetIsMimError() == true ) 
	{	return false; }
#endif//MIM_LIB_USE
	return true;
}
//-------------------------------------------------------------------------------------//
int CMimImageC24::GetImageW() const
{
#ifdef MIM_LIB_USE
	if ( CMimImageC24::CheckMimImage() == false ) { return false; }
	return GetWidth(m_ImageC24Ptr);	
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
int CMimImageC24::GetImageH() const
{
#ifdef MIM_LIB_USE
	if ( CMimImageC24::CheckMimImage() == false ) { return false; }
	return GetHeight(m_ImageC24Ptr);	
#endif//MIM_LIB_USE
	return 0;
}
//-------------------------------------------------------------------------------------//
int CMimImageC24::GetImageRowPitch()
{
#ifdef MIM_LIB_USE
	const int ImageW = GetImageW();
	return CMimImageC24::CalcRowPitch(ImageW);
#endif//MIM_LIB_USE
	return 0;
}
//-------------------------------------------------------------------------------------//
void* CMimImageC24::GetImagePtr(int &ImageW, int &ImageH, int &RowPitch, int &ColPitch, int &BytesPerLine)
{
#ifdef MIM_LIB_USE
	if ( CMimImageC24::CheckMimImage() == false ) 
	{ 
		ImageW = ImageH = RowPitch = ColPitch = BytesPerLine = 0;
		return NULL; 
	}
	void *ImagePtr = NULL;//iVarPtr(m_ImageC24Ptr);
	ImageW = GetWidth(m_ImageC24Ptr);
	ImageH = GetHeight(m_ImageC24Ptr);
	m_MimErrCode = iGetCameraAlignSize(m_ImageC24Ptr, &RowPitch, &ColPitch);	
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
