// EvsBarcode1D.cpp: implementation of the CEvsBarcode1D class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "EvsBarcode1D.h"
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
CEvsBarcode1D::CEvsBarcode1D()
{
	CEvsBarcode1D::PreInitBarcode1D();
	CEvsBarcode1D::InitialBarcode1D();
}
//-------------------------------------------------------------------------------------//
CEvsBarcode1D::CEvsBarcode1D(const CEvsBarcode1D &Barcode)
{
	CEvsBarcode1D::PreInitBarcode1D();
	CEvsBarcode1D::CloneBarcode1D(Barcode);
}
//-------------------------------------------------------------------------------------//
CEvsBarcode1D::~CEvsBarcode1D()
{
}
//-------------------------------------------------------------------------------------//
CEvsBarcode1D& CEvsBarcode1D::operator=(const CEvsBarcode1D &Barcode)
{
	if ( this == &Barcode ) { return *this; }
	CEvsBarcode1D::CloneBarcode1D(Barcode);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::PreInitBarcode1D()
{
	m_ErrorCode = 0;
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::InitialBarcode1D()
{
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::CloneBarcode1D(const CEvsBarcode1D &Barcode)
{
	this->m_ErrorCode = Barcode.m_ErrorCode;
	::strcpy(m_ErrorString, Barcode.m_ErrorString);
//	this->m_Barcode1D = Barcode.m_Barcode1D;	
}
//-------------------------------------------------------------------------------------//
#ifdef EVISION_1D_BARCODE_USE
EVS_BARCODE_1D* CEvsBarcode1D::GetBarcode1DPtr()
{
	return &m_Barcode1D;	
}
#endif//EVISION_1D_BARCODE_USE
//-------------------------------------------------------------------------------------//
const char* CEvsBarcode1D::GetErrorString() const
{
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcode1D::GetIsEVisionError()
{
#ifdef EVISION_1D_BARCODE_USE	
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
#endif//EVISION_1D_BARCODE_USE	
	return false;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetEBarCodeDefaultParam()
{	
#ifdef EVISION_1D_BARCODE_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Barcode1D.SetKnownLocation(TRUE);
		this->m_Barcode1D.SetCenter(0.0f, 0.0f);
		this->m_Barcode1D.SetVerifyChecksum(FALSE);
		this->m_Barcode1D.SetAdditionalSymbologies(0x0FFFFFFE);
		this->m_Barcode1D.SetSize(0.0f, 0.0f);
		this->m_Barcode1D.SetAngle(0.0f);
		this->m_Barcode1D.SetReadingCenter(0.0f, 0.0f);
		this->m_Barcode1D.SetReadingSize(0.0f, 0.0f);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION		
		try
		{	
			this->m_ErrorCode = EError_Ok;
			this->m_Barcode1D.SetKnownLocation(TRUE);
			this->m_Barcode1D.SetCenterXY(0.0f, 0.0f);
			this->m_Barcode1D.SetVerifyChecksum(FALSE);
			this->m_Barcode1D.SetAdditionalSymbologies(0x0FFFFFFE);
			this->m_Barcode1D.SetSize(0.0f, 0.0f);
			this->m_Barcode1D.SetAngle(0.0f);
			this->m_Barcode1D.SetReadingCenter(0.0f, 0.0f);
			this->m_Barcode1D.SetReadingSize(0.0f, 0.0f);
			return;
		}
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return ;
		}			
	#endif//EVISION_MODE
#endif//EVISION_1D_BARCODE_USE
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetKnownLocation(BOOL IsKnown)
{
#ifdef EVISION_1D_BARCODE_USE
	this->m_Barcode1D.SetKnownLocation(IsKnown);
#endif//EVISION_1D_BARCODE_USE
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetCenter(float Cx, float Cy)
{	
#ifdef EVISION_1D_BARCODE_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		this->m_Barcode1D.SetCenter(Cx, Cy);
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION	
		this->m_Barcode1D.SetCenterXY(Cx, Cy);
	#endif//EVISION_MODE
#endif//EVISION_1D_BARCODE_USE
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetSize(float W, float H)
{
#ifdef EVISION_1D_BARCODE_USE	
	this->m_Barcode1D.SetSize(W, H);	
#endif//EVISION_1D_BARCODE_USE
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetAngle(float Angle)
{	
#ifdef EVISION_1D_BARCODE_USE	
	this->m_Barcode1D.SetAngle(Angle);
#endif//EVISION_1D_BARCODE_USE
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetReadingCenter(float Cx, float Cy)
{	
#ifdef EVISION_1D_BARCODE_USE	
	this->m_Barcode1D.SetReadingCenter(Cx, Cy);
#endif//EVISION_1D_BARCODE_USE
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetReadingSize(float W, float H)
{	
#ifdef EVISION_1D_BARCODE_USE	
	this->m_Barcode1D.SetReadingSize(W, H);
#endif//EVISION_1D_BARCODE_USE
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetVerifyChecksum(BOOL IsCheckSum)
{	
#ifdef EVISION_1D_BARCODE_USE		
	this->m_Barcode1D.SetVerifyChecksum(IsCheckSum);	
#endif//EVISION_1D_BARCODE_USE
	return ;
}
//-------------------------------------------------------------------------------------//
void CEvsBarcode1D::SetAdditionalSymbologies(int Symbologies)
{
#ifdef EVISION_1D_BARCODE_USE
	this->m_Barcode1D.SetVerifyChecksum(Symbologies);	
#endif//EVISION_1D_BARCODE_USE
	return ;
			
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcode1D::Read(CEvsRoiBW8 *pRoi, char* BarcodeText, size_t BarcodeTextLen)
{		
#ifdef EVISION_1D_BARCODE_USE		

	#if EVISION_MODE == EVISION_MODE_EVISION	
		this->m_Barcode1D.Read(pRoi->GetRoiBW8Ptr(), BarcodeText, BarcodeTextLen);
		if ( this->GetIsEVisionError() == true )
		{	return false; }		
		return true;
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION		
		try
		{	
			this->m_ErrorCode = EError_Ok;			
			std::string str = this->m_Barcode1D.Read(pRoi->GetRoiBW8Ptr());
			const size_t len = (size_t)(str.length());
			if ( len == 0  )
			{		
				::strcpy(BarcodeText, "");
				::sprintf(this->m_ErrorString, "Error, Length of EMatrixCode DecodedString Exception (%d)", len);
				return true;	
			}

			if ( len >= BarcodeTextLen )
			{
				::memcpy(BarcodeText, str.c_str(), sizeof(char)*(BarcodeTextLen-1));
				BarcodeText[BarcodeTextLen-1] = '\0';
			}
			else
			{	::strcpy(BarcodeText, str.c_str()); }
			return true;
		}		
		catch(EException exc)
		{	
			m_ErrorCode = exc.GetError();
			sprintf(this->m_ErrorString, "%s", exc.What().c_str());
			return false;
		}
	#endif//EVISION_MODE
#endif//EVISION_1D_BARCODE_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEvsBarcode1D::Read(int ImageW, int ImageH, int ImageStep, unsigned char *pImage, char* BarcodeText, size_t BarcodeTextLen)//Åª¨ú
{
#ifdef EVISION_1D_BARCODE_USE	
	CEvsRoiBW8   RoiBW8;
	CEvsImageBW8 ImageBW8;
	if ( ImageBW8.SetImagePtr(pImage, ImageW, ImageH, ImageStep, true) == false ) 
	{	return false; }

	RoiBW8.Attach(&ImageBW8);
	RoiBW8.SetPlacement(0, 0, ImageW, ImageH);
	if ( Read(&RoiBW8, BarcodeText, BarcodeTextLen) == false )
	{
		RoiBW8.Detach();
		return false; 
	}
	RoiBW8.Detach();
	return true;
#endif//EVISION_1D_BARCODE_USE
	return false;
}
//-------------------------------------------------------------------------------------//