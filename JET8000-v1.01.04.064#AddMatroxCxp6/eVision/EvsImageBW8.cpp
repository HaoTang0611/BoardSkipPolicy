// EvsImageBW8.cpp: implementation of the CEvsImageBW8 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EvsImageBW8.h"
//-------------------------------------------------------------------------------------//
#include "EVisionLibDef.h"
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
unsigned long CEvsImageBW8::g_ImageBW8UniqueIDCount=0;//唯一碼數量
//-------------------------------------------------------------------------------------//
int  CEvsImageBW8::CalcRowPitch(int ImageW)
{
	int AlignByte = 32;	//4 for eVision, 32, for Open eVision	
	int RealImageX = ImageW;
	int RowPitch = RealImageX;
	if ( (RealImageX%AlignByte) == 0 )
	{	RowPitch = RealImageX; }
	else
	{	RowPitch = ((RealImageX/AlignByte)+1)*AlignByte;	}	
	return RowPitch;
}
//------------------------------------------------------------------//
CEvsImageBW8::CEvsImageBW8()
{
	CEvsImageBW8::PreInitImageBW8();
	CEvsImageBW8::InitialImageBW8();
}
//-------------------------------------------------------------------------------------//
CEvsImageBW8::CEvsImageBW8(const CEvsImageBW8 &Img)
{
	CEvsImageBW8::PreInitImageBW8();
	CEvsImageBW8::CloneImageBW8(Img);
}
//-------------------------------------------------------------------------------------//
CEvsImageBW8::~CEvsImageBW8()
{
	CEvsImageBW8::ReleaseBuffer();
}
//-------------------------------------------------------------------------------------//
CEvsImageBW8& CEvsImageBW8::operator=(const CEvsImageBW8 &Img)
{
	if ( this == &Img ) { return *this; }
	CEvsImageBW8::CloneImageBW8(Img);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CEvsImageBW8::PreInitImageBW8()
{
	m_UniqueID=::InterlockedIncrement(&g_ImageBW8UniqueIDCount);
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
	m_ErrorCode = 0;
	m_BufferPtr = NULL;
	m_BufferSize = 0;
}
//-------------------------------------------------------------------------------------//
void CEvsImageBW8::InitialImageBW8()
{
}
//-------------------------------------------------------------------------------------//
void CEvsImageBW8::CloneImageBW8(const CEvsImageBW8 &Img)
{
	::strcpy(m_ErrorString, Img.m_ErrorString);
	this->m_ErrorCode = Img.m_ErrorCode;

#ifdef EVISION_USE
	this->m_ImageBW8 = Img.m_ImageBW8;
	CEvsImageBW8::ReleaseBuffer();
	CEvsImageBW8::m_BufferSize = Img.m_BufferSize;
	if ( CEvsImageBW8::m_BufferSize > 0 ) 
	{
		if ( JetMemory.alloc_func(m_BufferSize, CEvsImageBW8::m_BufferPtr, "CEvsImageBW8::CloneImageBW8", "m_BufferPtr") == false )		
		{	
			CEvsImageBW8::m_BufferSize = 0;	
			this->m_ImageBW8.SetImagePtr(0, 0, NULL, 0);			
		}
		else
		{
			int ImageW = this->m_ImageBW8.GetWidth();
			int ImageH = this->m_ImageBW8.GetHeight();
			const int PerLineByte = CEvsImageBW8::CalcRowPitch(ImageW);	
			const int BitsPerLine = PerLineByte*8;
			::memcpy(m_BufferPtr, Img.m_BufferPtr, sizeof(unsigned char)*m_BufferSize);
			this->m_ImageBW8.SetImagePtr(ImageW, ImageH, (void*)m_BufferPtr, BitsPerLine);
		}
	}	
#endif//EVISION_USE
}
//-------------------------------------------------------------------------------------//
void CEvsImageBW8::ReleaseBuffer()
{
	if ( NULL != m_BufferPtr )
	{	JetMemory.free_func(m_BufferPtr);	}	
	m_BufferSize = 0;
}
//-------------------------------------------------------------------------------------//
#ifdef EVISION_USE
EVS_IMAGE_BW8*  CEvsImageBW8::GetImageBW8Ptr()
{
	return &m_ImageBW8;	
}
#endif//EVISION_USE
//-------------------------------------------------------------------------------------//
const char* CEvsImageBW8::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CEvsImageBW8::GetIsEVisionError()
{
#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		if (EGetError( ) != E_OK)
		{
			sprintf(this->m_ErrorString, "%s", EGetErrorText());
			EOk();
			return true;
		}
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		if ( this->m_ErrorCode != EError_Ok )
		{	return true;	}		
	#endif	
	return false;
#endif//EVISION_USE	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsImageBW8::LoadImage(const char *pFileName)
{
#ifdef EVISION_USE		
	#if EVISION_MODE == EVISION_MODE_EVISION
		m_ImageBW8.Load(pFileName);
		if( this->GetIsEVisionError() == true )
		{	return false; }
		this->ReleaseBuffer();
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			this->m_ImageBW8.Load(pFileName);	
			this->ReleaseBuffer();
			return true;	
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEvsImageBW8::SetImagePtr(const unsigned char *pImage, int ImageW, int ImageH, int BytesPerLine, bool reAlloc, bool byFile)
{
	const char fnName[] = "CEvsImageBW8::SetImagePtr";	
#ifdef EVISION_USE
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
	if ( true == byFile )
	{
		CString sFile;
		CString sFolder=AOIDataCollect.GetAOITempDirectory();
		sFile.Format(_T("%s\\%s#%08d.PNG"), sFolder, _T("EvsImageBW8"), m_UniqueID);		
		if ( ImageAPI.SaveImage(sFile, ImageW, ImageH, BytesPerLine, 8, pImage, true) == false )
		{
			::strcpy(m_ErrorString, "Error, Save Image Fault");
			return false;
		}

		std::string Filename;
		JetAPI::TCHAR2string(sFile, Filename);
		if ( LoadImage(Filename.c_str()) == false )
		{	return false; }		
		::DeleteFile(sFile);		
		return true;
	}

	unsigned char *TempImage = NULL;	
	const int PerLineByte = CEvsImageBW8::CalcRowPitch(ImageW);		
	if( reAlloc == true )	//需要重新排列為32byte格式
	{	
		int   RealImageW = ImageW;
		const size_t szImageH=ImageH;
		const size_t szPerLineByte=PerLineByte;
		const size_t Size = szPerLineByte*szImageH;		
		RealImageW = ImageW;
		if ( Size > m_BufferSize )
		{
			CEvsImageBW8::ReleaseBuffer();
			if ( JetMemory.alloc_func(Size, this->m_BufferPtr, fnName, "m_BufferPtr") == false )			
			{
				::sprintf(this->m_ErrorString, "Eorror, Memory Allocate Fault");
				return false;
			}
			this->m_BufferSize = Size;
		}
		TempImage = m_BufferPtr;	
		

		int i=0;
		unsigned char *pSrcImg = (unsigned char *)pImage;
		unsigned char *pDestImg = (unsigned char *)TempImage;
		const int BitsPerLine = PerLineByte*8;

		if ( PerLineByte == BytesPerLine )
		{	::memcpy(pDestImg, pSrcImg, sizeof(unsigned char)*Size);	}
		else
		{
			for( i=0 ; i<ImageH ; i++ )
			{
				::memcpy(pDestImg, pSrcImg, sizeof(unsigned char)*RealImageW);
				pDestImg += PerLineByte;
				pSrcImg += BytesPerLine;
			}
		}
		
	#ifdef _DEBUG
		int nRowPitch = 0;
	#endif		
		this->m_ImageBW8.SetImagePtr(ImageW, ImageH, (void*)TempImage, BitsPerLine);
		//m_ROIBW8.SetPlacement(0, 0, ImageW, ImageH);
	#ifdef _DEBUG
		nRowPitch = this->m_ImageBW8.GetRowPitch();
	#endif
	}
	else
	{		
		if ( BytesPerLine != PerLineByte )
		{
			sprintf(this->m_ErrorString, "Eorror, Image Memory Size Exception (W:%d, H:%d, W2:%d, BytePerLine:%d)", ImageW, ImageH, BytesPerLine, PerLineByte);
			return false;
		}

	#ifdef _DEBUG
		int nRowPitch = 0;
	#endif
		TempImage = (unsigned char *)pImage; 		
		const int BitsPerLine = BytesPerLine*8;		
		this->m_ImageBW8.SetImagePtr(ImageW, ImageH, (void*)TempImage, BitsPerLine);				
		CEvsImageBW8::ReleaseBuffer();
	#ifdef _DEBUG
		nRowPitch = this->m_ImageBW8.GetRowPitch();
	#endif	
	}
	return true;
#endif//endif EVISION_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsImageBW8::SaveImage(const char *pFileName)
{
#ifdef EVISION_USE
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_ImageBW8.Save(pFileName);
		if( this->GetIsEVisionError() == true )
		{	return false; }
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		try
		{
			this->m_ErrorCode = EError_Ok;			
			this->m_ImageBW8.Save(pFileName);
			return true;	
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
int CEvsImageBW8::GetImageW()
{
#ifdef EVISION_USE
	return this->m_ImageBW8.GetWidth();		
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
int CEvsImageBW8::GetImageH()
{
#ifdef EVISION_USE
	return this->m_ImageBW8.GetHeight();	
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
int CEvsImageBW8::GetImageRowPitch()
{
#ifdef EVISION_USE	
	return this->m_ImageBW8.GetRowPitch();
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
void* CEvsImageBW8::GetImagePtr(int &ImageW, int &ImageH, int &RowPitch, int &ColPitch, int &BytesPerLine)
{
#ifdef EVISION_USE	
	void *ImagePtr = this->m_ImageBW8.GetImagePtr(ImageW, ImageH);
	ImageW = this->m_ImageBW8.GetWidth();
	ImageH = this->m_ImageBW8.GetHeight();
	RowPitch = this->m_ImageBW8.GetRowPitch();
	ColPitch = this->m_ImageBW8.GetColPitch();
	BytesPerLine = this->m_ImageBW8.GetBitsPerPixel();		
	return ImagePtr;
#endif//endif EVISION_USE
	return NULL;	
}
//-------------------------------------------------------------------------------------//
