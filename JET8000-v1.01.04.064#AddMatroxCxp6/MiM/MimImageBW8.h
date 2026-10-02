// MimImageBW8.h: interface for the CMimImageBW8 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIMIMAGEBW8_H__7CBAD8F1_4C46_47DA_9B78_4148409805A9__INCLUDED_)
#define AFX_MIMIMAGEBW8_H__7CBAD8F1_4C46_47DA_9B78_4148409805A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "MimLibDef.h"
//-------------------------------------------------------------------------------------//
class CMimImageBW8  
{
	//---------------------------------------------------------------------------------//
	static int                CalcRowPitch(int ImageW);
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//		
	char                       m_ErrorString[128];		
#ifdef MIM_LIB_USE
	LONG_PTR                   m_ImageBW8Ptr;//實際物件	
	E_iVision_ERRORS           m_MimErrCode;
#endif//MIM_LIB_USE
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitImageBW8();
	void                       InitialImageBW8();
	void                       CloneImageBW8(const CMimImageBW8 &Img);
	//---------------------------------------------------------------------------------//		
	bool                       CheckMimImage();	
	bool                       CheckMimImage() const;	
	bool                       GetIsMimError();	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//		
	CMimImageBW8();
	CMimImageBW8(const CMimImageBW8 &Img);
	virtual ~CMimImageBW8();
	//---------------------------------------------------------------------------------//		
	CMimImageBW8& operator=(const CMimImageBW8 &Img);
	//---------------------------------------------------------------------------------//
#ifdef MIM_LIB_USE		
	LONG_PTR                   GetImageBW8Ptr();
	bool                       CheckImageBW8Ptr();
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

#endif // !defined(AFX_MIMIMAGEBW8_H__7CBAD8F1_4C46_47DA_9B78_4148409805A9__INCLUDED_)
