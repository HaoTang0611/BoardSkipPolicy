// JetBarcode.cpp: implementation of the CJetBarcode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JetBarcode.h"
//--------------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//--------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------//
CString CJetBarcode::GetBarcodeLibTypeText(JET_BARCODE_LIB_TYPE Type)
{
	CString str;
	switch ( Type )
	{
	case JET_BARCODE_LIB_EVS: str = _T("Open eVision Barcode");		break;
	case JET_BARCODE_LIB_DTK: str = _T("DTK Library Barcode");		break;
	case JET_BARCODE_LIB_HON: str = _T("Honeywell SwiftDecoder");	break;
	default:
		str = _T("Undefined Barcode Library");
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CJetBarcode::CJetBarcode()
{
	CJetBarcode::PreInitBarcode();
	CJetBarcode::InitialBarcode();
}
//-------------------------------------------------------------------------------------//
CJetBarcode::~CJetBarcode()
{

}
//-------------------------------------------------------------------------------------//
void CJetBarcode::PreInitBarcode()
{
	m_ErrorString = _T("");	
	m_QRCodeLibType = JET_BARCODE_LIB_NONE;
	m_Barcode1DLibType = JET_BARCODE_LIB_NONE;	
	m_DataMatrixLibType = JET_BARCODE_LIB_NONE;

#ifdef EVISION_1D_BARCODE_USE
	m_Barcode1DLibType = JET_BARCODE_LIB_EVS;
#endif//EVISION_1D_BARCODE_USE

#ifdef EVISION_DATA_MATRIX_USE
	m_DataMatrixLibType = JET_BARCODE_LIB_EVS;
#endif//EVISION_DATA_MATRIX_USE

#ifdef EVISION_QRCODE_USE
	m_QRCodeLibType = JET_BARCODE_LIB_EVS;
#endif//EVISION_QRCODE_USE

#ifdef DTK_BARCODE_USE
	m_QRCodeLibType = JET_BARCODE_LIB_DTK;
	m_Barcode1DLibType = JET_BARCODE_LIB_DTK;	
	m_DataMatrixLibType = JET_BARCODE_LIB_DTK;	
#endif//DTK_BARCODE_USE
}
//-------------------------------------------------------------------------------------//
void CJetBarcode::InitialBarcode()
{
	m_ErrorString = _T("");
	m_DecodeTimeout = 0;
	m_EnableCheckSum = true;
	m_BarcodeDirection = BARCODE_DIRECTION_ALL;	
}
//-------------------------------------------------------------------------------------//
JET_BARCODE_LIB_TYPE CJetBarcode::MapDecoderToLibType(BARCODE_DECODER_TYPE val) const
{
	JET_BARCODE_LIB_TYPE Lib;
	switch ( val )
	{
	case BARCODE_DECODER_EVS: Lib=JET_BARCODE_LIB_EVS;	break;
	case BARCODE_DECODER_DTK: Lib=JET_BARCODE_LIB_DTK;	break;
	case BARCODE_DECODER_HON: Lib=JET_BARCODE_LIB_HON;	break;
	default:
		Lib=JET_BARCODE_LIB_NONE;
		break;
	}
	return Lib;
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::SetBarcode1DLibType(JET_BARCODE_LIB_TYPE val)//設定條碼函式庫
{
	switch ( val )
	{
	case JET_BARCODE_LIB_EVS:
	#ifdef EVISION_1D_BARCODE_USE
		m_Barcode1DLibType = val;
	#endif//EVISION_1D_BARCODE_USE
		break;
	case JET_BARCODE_LIB_DTK:
	#ifdef DTK_BARCODE_USE
		m_Barcode1DLibType = val;
	#endif//DTK_BARCODE_USE
		break;
	case JET_BARCODE_LIB_HON:
	#ifdef HON_BARCODE_USE
		m_Barcode1DLibType = val;
	#endif//HON_BARCODE_USE
		break;
	}
	if ( m_Barcode1DLibType != val ) 
	{
		this->m_ErrorString.Format(_T("Error, Barcode 1D Not Support %s"), CJetBarcode::GetBarcodeLibTypeText(val) );
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::SetQRCodeLibType(JET_BARCODE_LIB_TYPE val)//設定條碼函式庫-2D
{
	switch ( val )
	{
	case JET_BARCODE_LIB_EVS:
	#ifdef EVISION_QRCODE_USE
		m_QRCodeLibType = val;
	#endif//EVISION_QRCODE_USE
		break;
	case JET_BARCODE_LIB_DTK:
	#ifdef DTK_BARCODE_USE
		m_QRCodeLibType = val;
	#endif//DTK_BARCODE_USE
		break;
	case JET_BARCODE_LIB_HON:
	#ifdef HON_BARCODE_USE
		m_QRCodeLibType = val;
	#endif//HON_BARCODE_USE
		break;
	}
	if ( m_QRCodeLibType != val ) 
	{
		this->m_ErrorString.Format(_T("Error, QRCode Not Support %s"), CJetBarcode::GetBarcodeLibTypeText(val) );
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::SetDataMatrixLibType(JET_BARCODE_LIB_TYPE val)
{
	switch ( val )
	{
	case JET_BARCODE_LIB_EVS:
	#ifdef EVISION_DATA_MATRIX_USE
		m_DataMatrixLibType = val;
	#endif//EVISION_DATA_MATRIX_USE
		break;
	case JET_BARCODE_LIB_DTK:
	#ifdef DTK_BARCODE_USE
		m_DataMatrixLibType = val;
	#endif//DTK_BARCODE_USE
		break;
	case JET_BARCODE_LIB_HON:
	#ifdef HON_BARCODE_USE
		m_DataMatrixLibType = val;
	#endif//HON_BARCODE_USE
		break;
	}
	if ( m_DataMatrixLibType != val ) 
	{
		this->m_ErrorString.Format(_T("Error, DataMatrix Not Support %s"), CJetBarcode::GetBarcodeLibTypeText(val) );
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::SetQRCodeDecoder(BARCODE_DECODER_TYPE val)//設定條碼解碼器-2D
{
	return SetQRCodeLibType(MapDecoderToLibType(val));	
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::SetBarcode1DDecoder(BARCODE_DECODER_TYPE val)//設定條碼解碼器-1D
{
	return SetBarcode1DLibType(MapDecoderToLibType(val));
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::SetDataMatrixDecoder(BARCODE_DECODER_TYPE val)//設定條碼解碼器-2D
{
	return SetDataMatrixLibType(MapDecoderToLibType(val));
}
//-------------------------------------------------------------------------------------//
void CJetBarcode::SetDecodeTimeout(int val)//設定條碼解碼逾時時間
{ 
	m_DecodeTimeout = val; 
}
//-------------------------------------------------------------------------------------//
void CJetBarcode::SetEnableCheckSum(bool val)//設定啟用CheckSum	
{
	m_EnableCheckSum = val;
}
//-------------------------------------------------------------------------------------//
void CJetBarcode::SetBarcodeDirection(JET_BARCODE_DIRECTION CodeDirection)//設定條碼方向-節省時間
{
	m_BarcodeDirection = CodeDirection;
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::DecodeBarcode1D(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, char BarcodeContent[], size_t BarcodeSize)
{
	DWORD   dwRes = 0;
	CString filename;	
	int     BarcodeOrient=0;	
	::memset(BarcodeContent, 0x00, sizeof(char)*BarcodeSize);
	switch ( m_Barcode1DLibType )
	{
	case JET_BARCODE_LIB_EVS:
		m_EvsBarcode1D.SetEBarCodeDefaultParam();		
		m_EvsBarcode1D.SetKnownLocation(FALSE);
		m_EvsBarcode1D.SetVerifyChecksum(m_EnableCheckSum);
		if ( m_EvsBarcode1D.Read(ImageW, ImageH, ImageStep, ImagePtr, BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = m_EvsBarcode1D.GetErrorString();
			return false;
		}
		break;
	case JET_BARCODE_LIB_DTK:		
		m_DtkBarcodeDecoder.LockDTKBarcode();		
		filename = m_DtkBarcodeDecoder.BuildLoadImage_DTK(AOIDataCollect.GetAOITempDirectory(), _T("Barcode_1D_DTK"));
		if ( ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, 8, ImagePtr, true) == false )
		{
			this->m_ErrorString = ImageAPI.GetImageApiErrorString();
			m_DtkBarcodeDecoder.UnlockDTKBarcode();
			return false;
		}		
		dwRes = (BARCODE_DIRECTION_HOR&m_BarcodeDirection);
		if ( 0 != dwRes )
		{
			BarcodeOrient |= BO_LeftToRight;
			BarcodeOrient |= BO_RightToLeft;			
		}

		dwRes = (BARCODE_DIRECTION_VER&m_BarcodeDirection);
		if ( 0 != dwRes )
		{	
			BarcodeOrient |= BO_BottomToTop;
			BarcodeOrient |= BO_TopToBottom;			
		}
		m_DtkBarcodeDecoder.SetBarcodeDirection_DTK(BarcodeOrient);	
		
		m_DtkBarcodeDecoder.SetLoadImage_DTK(filename);		
		m_DtkBarcodeDecoder.SetVerifyChecksum_DTK(m_EnableCheckSum);
		m_DtkBarcodeDecoder.SetRecognitionTimeout_DTK(m_DecodeTimeout);
		if ( m_DtkBarcodeDecoder.ExecDecode1D_DTK(BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = m_DtkBarcodeDecoder.GetErrorString();
			m_DtkBarcodeDecoder.UnlockDTKBarcode();		
			return false;
		}
		::DeleteFile(filename);	
		m_DtkBarcodeDecoder.UnlockDTKBarcode();		
		break;
	case JET_BARCODE_LIB_HON:
		HoneywellBarcodeSdk.LockHonBarcode();
		HoneywellBarcodeSdk.SetVerifyChecksum_Hon(m_EnableCheckSum);
		if ( HoneywellBarcodeSdk.ExecDecode1D_Hon(ImageW, ImageH, ImageStep, ImagePtr, BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = HoneywellBarcodeSdk.GetErrorString();
			HoneywellBarcodeSdk.UnlockHonBarcode();
			return false;
		}
		HoneywellBarcodeSdk.UnlockHonBarcode();
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::DecodeBarcodeDataMatrix(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, char BarcodeContent[], size_t BarcodeSize)
{
	CString filename;	
	::memset(BarcodeContent, 0x00, sizeof(char)*BarcodeSize);		
	//::sprintf(BarcodeContent, "BarcodeTest"); return true;//For Test 20251025
	switch ( m_DataMatrixLibType )
	{
	case JET_BARCODE_LIB_EVS:
		m_EvsDataMatrix.SetEvsDecodeTimeout(m_DecodeTimeout);
		if ( m_EvsDataMatrix.Read(ImageW, ImageH, ImageStep, ImagePtr, BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = m_EvsDataMatrix.GetErrorString();
			return false;
		}
		break;
	case JET_BARCODE_LIB_DTK:		
		m_DtkBarcodeDecoder.LockDTKBarcode();		
		filename = m_DtkBarcodeDecoder.BuildLoadImage_DTK(AOIDataCollect.GetAOITempDirectory(), _T("Barcode_DataMatrix_DTK"));
		if ( ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, 8, ImagePtr, true) == false )
		{
			this->m_ErrorString = ImageAPI.GetImageApiErrorString();
			m_DtkBarcodeDecoder.UnlockDTKBarcode();
			return false;
		}		
		m_DtkBarcodeDecoder.SetLoadImage_DTK(filename);
		m_DtkBarcodeDecoder.SetRecognitionTimeout_DTK(m_DecodeTimeout);
		if ( m_DtkBarcodeDecoder.ExecDecodeDataMatrix_DTK(BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = m_DtkBarcodeDecoder.GetErrorString();
			m_DtkBarcodeDecoder.UnlockDTKBarcode();		
			return false;
		}
		::DeleteFile(filename);	
		m_DtkBarcodeDecoder.UnlockDTKBarcode();		
		break;
	case JET_BARCODE_LIB_HON:
		HoneywellBarcodeSdk.LockHonBarcode();
		if ( HoneywellBarcodeSdk.ExecDecodeDataMatrix_Hon(ImageW, ImageH, ImageStep, ImagePtr, BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = HoneywellBarcodeSdk.GetErrorString();
			HoneywellBarcodeSdk.UnlockHonBarcode();
			return false;
		}
		HoneywellBarcodeSdk.UnlockHonBarcode();
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::DecodeBarcodeQRCode(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, char BarcodeContent[], size_t BarcodeSize)
{
	CString filename;	
	::memset(BarcodeContent, 0x00, sizeof(char)*BarcodeSize);	
	switch ( m_QRCodeLibType )
	{
	case JET_BARCODE_LIB_EVS:
		m_EvsQRCode.SetEvsDecodeTimeout(m_DecodeTimeout);
		if ( m_EvsQRCode.Read(ImageW, ImageH, ImageStep, ImagePtr, BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = m_EvsQRCode.GetErrorString();
			return false;
		}
		FilterBarcodeForJSON(BarcodeContent);
		break;
	case JET_BARCODE_LIB_DTK:
		m_DtkBarcodeDecoder.LockDTKBarcode();		
		filename = m_DtkBarcodeDecoder.BuildLoadImage_DTK(AOIDataCollect.GetAOITempDirectory(), _T("Barcode_QRCode_DTK"));
		if ( ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, 8, ImagePtr, true) == false )
		{
			this->m_ErrorString = ImageAPI.GetImageApiErrorString();
			m_DtkBarcodeDecoder.UnlockDTKBarcode();
			return false;
		}
		m_DtkBarcodeDecoder.SetLoadImage_DTK(filename);
		m_DtkBarcodeDecoder.SetRecognitionTimeout_DTK(m_DecodeTimeout);
		if ( m_DtkBarcodeDecoder.ExecDecodeQRCode_DTK(BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = m_DtkBarcodeDecoder.GetErrorString();
			m_DtkBarcodeDecoder.UnlockDTKBarcode();		
			return false;
		}
		::DeleteFile(filename);	
		m_DtkBarcodeDecoder.UnlockDTKBarcode();		
		break;
	case JET_BARCODE_LIB_HON:
		HoneywellBarcodeSdk.LockHonBarcode();
		if ( HoneywellBarcodeSdk.ExecDecodeQRCode_Hon(ImageW, ImageH, ImageStep, ImagePtr, BarcodeContent, BarcodeSize) == false )
		{
			this->m_ErrorString = HoneywellBarcodeSdk.GetErrorString();
			HoneywellBarcodeSdk.UnlockHonBarcode();
			return false;
		}
		HoneywellBarcodeSdk.UnlockHonBarcode();
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBarcode::FilterBarcodeForJSON(char BarcodeContent[])//過濾條碼內容-為了JSON格式
{
	size_t i=0, j=0;
	const size_t len=::strlen(BarcodeContent);

	j = 0;
	for ( i=0; i<len; i++ )
	{
		if ( '\n' == BarcodeContent[i] ) { continue; }//JSON不支援換行字元
		BarcodeContent[j] = BarcodeContent[i];
		j ++;
	}
	BarcodeContent[j] = '\0';
	return true;
}
//-------------------------------------------------------------------------------------//
