// EvsImageBW8.h: interface for the CEvsImageBW8 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVSIMAGEBW8_H__777D3773_15BE_4D03_B95E_0737D9A1DAB4__INCLUDED_)
#define AFX_EVSIMAGEBW8_H__777D3773_15BE_4D03_B95E_0737D9A1DAB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "EVisionLibDef.h"
//-------------------------------------------------------------------------------------//
class CEvsImageBW8  
{
	//---------------------------------------------------------------------------------//
	static unsigned long      g_ImageBW8UniqueIDCount;//唯一碼數量
	static int                CalcRowPitch(int ImageW);
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//	
	unsigned long              m_UniqueID;
	int                        m_ErrorCode;
	char                       m_ErrorString[128];	
	unsigned char*             m_BufferPtr;
	size_t                     m_BufferSize;
#ifdef EVISION_USE
	EVS_IMAGE_BW8              m_ImageBW8;//實際物件	
#endif//EVISION_USE
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitImageBW8();
	void                       InitialImageBW8();
	void                       CloneImageBW8(const CEvsImageBW8 &Img);
	//---------------------------------------------------------------------------------//
	void                       ReleaseBuffer();
	//---------------------------------------------------------------------------------//
	bool                       GetIsEVisionError();	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CEvsImageBW8();
	CEvsImageBW8(const CEvsImageBW8 &Img);
	virtual ~CEvsImageBW8();
	//---------------------------------------------------------------------------------//	
	CEvsImageBW8& operator=(const CEvsImageBW8 &Img);
	//---------------------------------------------------------------------------------//		
#ifdef EVISION_USE
	EVS_IMAGE_BW8*             GetImageBW8Ptr();	
#endif//EVISION_USE
	const char*                GetErrorString() const;
	//---------------------------------------------------------------------------------//
	bool                       LoadImage(const char *pFileName);
	bool                       SaveImage(const char *pFileName);
	bool                       SetImagePtr(const unsigned char *pImage, int ImageW, int ImageH, int BytesPerLine, bool reAlloc, bool byFile=false);
	//---------------------------------------------------------------------------------//
	int                        GetImageW();
	int                        GetImageH();
	int                        GetImageRowPitch();
	void*                      GetImagePtr(int &ImageW, int &ImageH, int &RowPitch, int &ColPitch, int &BytesPerLine);
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//

#endif // !defined(AFX_EVSIMAGEBW8_H__777D3773_15BE_4D03_B95E_0737D9A1DAB4__INCLUDED_)
