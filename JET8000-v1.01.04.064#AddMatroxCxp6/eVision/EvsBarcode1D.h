// EvsBarcode1D.h: interface for the CEvsBarcode1D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVSBARCODE1D_H__D8EE98C1_4B0D_4817_9D20_EF2D04935FDC__INCLUDED_)
#define AFX_EVSBARCODE1D_H__D8EE98C1_4B0D_4817_9D20_EF2D04935FDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "EvsRoiBW8.h"
//-------------------------------------------------------------------------------------//
class CEvsBarcode1D  
{
private:
	//---------------------------------------------------------------------------------//
	int                        m_ErrorCode;
	char                       m_ErrorString[128];
#ifdef EVISION_1D_BARCODE_USE
	EVS_BARCODE_1D             m_Barcode1D;//實際物件的指標
#endif//EVISION_1D_BARCODE_USE
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitBarcode1D();
	void                       InitialBarcode1D();
	void                       CloneBarcode1D(const CEvsBarcode1D &Barcode);
	//---------------------------------------------------------------------------------//	
	CEvsBarcode1D(const CEvsBarcode1D &Barcode);
	//---------------------------------------------------------------------------------//
	CEvsBarcode1D& operator=(const CEvsBarcode1D &Barcode);	
	//---------------------------------------------------------------------------------//	
	bool                       GetIsEVisionError();		
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CEvsBarcode1D();	
	virtual ~CEvsBarcode1D();
	//---------------------------------------------------------------------------------//	
#ifdef EVISION_1D_BARCODE_USE
	EVS_BARCODE_1D*            GetBarcode1DPtr();
#endif//EVISION_1D_BARCODE_USE
	const char*                GetErrorString() const;	
	//---------------------------------------------------------------------------------//	
	void                       SetEBarCodeDefaultParam();
	//---------------------------------------------------------------------------------//
	void                       SetKnownLocation(BOOL IsKnown);	
	//---------------------------------------------------------------------------------//
	void                       SetCenter(float Cx, float Cy);
	//---------------------------------------------------------------------------------//
	void                       SetSize(float W, float H);
	//---------------------------------------------------------------------------------//
	void                       SetAngle(float Angle);
	//---------------------------------------------------------------------------------//
	void                       SetReadingCenter(float Cx, float Cy);
	//---------------------------------------------------------------------------------//
	void                       SetReadingSize(float W, float H);
	//---------------------------------------------------------------------------------//	
	void                       SetVerifyChecksum(BOOL IsCheckSum);		
	//---------------------------------------------------------------------------------//
	void                       SetAdditionalSymbologies(int Symbologies);	
	//---------------------------------------------------------------------------------//
	bool                       Read(CEvsRoiBW8 *pRoi, char* BarcodeText, size_t BarcodeTextLen);
	//---------------------------------------------------------------------------------//
	bool                       Read(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen);//讀取
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_EVSBARCODE1D_H__D8EE98C1_4B0D_4817_9D20_EF2D04935FDC__INCLUDED_)
