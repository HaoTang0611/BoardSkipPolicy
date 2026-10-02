// EvsImageC24.h: interface for the CEvsImageC24 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVSIMAGEC24_H__9FEC1564_1802_4097_9FAC_76D91EF0C09A__INCLUDED_)
#define AFX_EVSIMAGEC24_H__9FEC1564_1802_4097_9FAC_76D91EF0C09A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "EVisionLibDef.h"
//-------------------------------------------------------------------------------------//
class CEvsImageC24  
{
	//---------------------------------------------------------------------------------//
	static unsigned long       g_ImageC24UniqueIDCount;//唯一碼數量
	static int                 CalcRowPitch(int ImageW);
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//	
	unsigned long              m_UniqueID;
	int                        m_ErrorCode;
	char                       m_ErrorString[128];	
	unsigned char*             m_BufferPtr;
	size_t                     m_BufferSize;
#ifdef EVISION_USE
	EVS_IMAGE_C24              m_ImageC24;//實際物件
#endif//EVISION_USE
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitImageC24();
	void                       InitialImageC24();
	void                       CloneImageC24(const CEvsImageC24 &Img);
	//---------------------------------------------------------------------------------//
	void                       ReleaseBuffer();
	//---------------------------------------------------------------------------------//
	bool                       GetIsEVisionError();	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CEvsImageC24();
	CEvsImageC24(const CEvsImageC24 &Img);
	virtual ~CEvsImageC24();
	//---------------------------------------------------------------------------------//
	CEvsImageC24& operator=(const CEvsImageC24 &Img);
	//---------------------------------------------------------------------------------//
#ifdef EVISION_USE
	EVS_IMAGE_C24*             GetImageC24Ptr();	
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
#endif // !defined(AFX_EVSIMAGEC24_H__9FEC1564_1802_4097_9FAC_76D91EF0C09A__INCLUDED_)
