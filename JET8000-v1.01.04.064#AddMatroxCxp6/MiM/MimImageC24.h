// MimImageC24.h: interface for the CMimImageC24 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIMIMAGEC24_H__9A9BEBEB_4904_4100_9024_C60AAE14F56F__INCLUDED_)
#define AFX_MIMIMAGEC24_H__9A9BEBEB_4904_4100_9024_C60AAE14F56F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "MimLibDef.h"
//-------------------------------------------------------------------------------------//
class CMimImageC24  
{
	//---------------------------------------------------------------------------------//
	static int                CalcRowPitch(int ImageW);
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//		
	char                       m_ErrorString[128];		
#ifdef MIM_LIB_USE
	LONG_PTR                   m_ImageC24Ptr;//實際物件	
	E_iVision_ERRORS           m_MimErrCode;
#endif//MIM_LIB_USE
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitImageC24();
	void                       InitialImageC24();
	void                       CloneImageC24(const CMimImageC24 &Img);
	//---------------------------------------------------------------------------------//		
	bool                       CheckMimImage();	
	bool                       CheckMimImage() const;	
	bool                       GetIsMimError();	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//		
	CMimImageC24();
	CMimImageC24(const CMimImageC24 &Img);
	virtual ~CMimImageC24();
	//---------------------------------------------------------------------------------//		
	CMimImageC24& operator=(const CMimImageC24 &Img);
	//---------------------------------------------------------------------------------//
#ifdef MIM_LIB_USE
	LONG_PTR                   GetImageC24Ptr();
	bool                       CheckImageC24Ptr();
#endif//EVISION_USE
	//---------------------------------------------------------------------------------//
	const char*                GetErrorString() const;
	//---------------------------------------------------------------------------------//
	bool                       LoadImage(const char *pFileName);
	bool                       SaveImage(const char *pFileName);
	bool                       SetImagePtr(const unsigned char *pImage, int ImageW, int ImageH, int BytesPerLine, bool reAlloc);
	//---------------------------------------------------------------------------------//
	int                        GetImageW()  const;
	int                        GetImageH()  const;
	int                        GetImageRowPitch();
	void*                      GetImagePtr(int &ImageW, int &ImageH, int &RowPitch, int &ColPitch, int &BytesPerLine);
	//---------------------------------------------------------------------------------//
};

#endif // !defined(AFX_MIMIMAGEC24_H__9A9BEBEB_4904_4100_9024_C60AAE14F56F__INCLUDED_)
