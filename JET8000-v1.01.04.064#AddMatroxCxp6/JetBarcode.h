// JetBarcode.h: interface for the CJetBarcode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETBARCODE_H__3C71C429_C27C_4C36_99D6_F29A14E770C4__INCLUDED_)
#define AFX_JETBARCODE_H__3C71C429_C27C_4C36_99D6_F29A14E770C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "DtkBarcode.h"
#include "HonBarcode.h"
#include "EvsBarcode1D.h"
#include "EvsBarcodeQRCode.h"
#include "EvsBarcodeDataMatrix.h"
//-------------------------------------------------------------------------------------//
#define   BARCODE_CONTENT_SIZE    128//條碼內容尺寸-128
//-------------------------------------------------------------------------------------//
enum JET_BARCODE_LIB_TYPE
{
	JET_BARCODE_LIB_NONE = 0,
	JET_BARCODE_LIB_EVS  = 1,
	JET_BARCODE_LIB_DTK  = 2,
	JET_BARCODE_LIB_HON  = 3 //Honeywell
};
//-------------------------------------------------------------------------------------//
enum JET_BARCODE_DIRECTION
{
	BARCODE_DIRECTION_NONE = 0x00,
	BARCODE_DIRECTION_HOR  = 0x01,
	BARCODE_DIRECTION_VER  = 0x02,
	BARCODE_DIRECTION_ALL  = 0XFF
};
//-------------------------------------------------------------------------------------//
class CJetBarcode  
{
	//---------------------------------------------------------------------------------//
	static CString            GetBarcodeLibTypeText(JET_BARCODE_LIB_TYPE Type);
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	CString                    m_ErrorString;//錯誤訊息
	int                        m_DecodeTimeout;//解碼逾時
	bool                       m_EnableCheckSum;//啟用CheckSum
	JET_BARCODE_DIRECTION      m_BarcodeDirection;//條碼方向-1D	
	JET_BARCODE_LIB_TYPE       m_Barcode1DLibType;//解碼函式庫樣式
	JET_BARCODE_LIB_TYPE       m_QRCodeLibType;//解碼函式庫樣式
	JET_BARCODE_LIB_TYPE       m_DataMatrixLibType;//解碼函式庫樣式	
	//---------------------------------------------------------------------------------//
	CEvsBarcodeQRCode          m_EvsQRCode;//Euresys QRCode
	CEvsBarcode1D              m_EvsBarcode1D;//Euresys Barcode
	CEvsBarcodeDataMatrix      m_EvsDataMatrix;//Euresys DataMatrix
	CDtkBarcode                m_DtkBarcodeDecoder;//DTK Barcode
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CJetBarcode(const CJetBarcode &barcode);
	//---------------------------------------------------------------------------------//
	CJetBarcode& operator=(const CJetBarcode &barcode);
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcode();
	void                       InitialBarcode();
	//---------------------------------------------------------------------------------//
	JET_BARCODE_LIB_TYPE       MapDecoderToLibType(BARCODE_DECODER_TYPE val) const;
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CJetBarcode();
	virtual ~CJetBarcode();
	//---------------------------------------------------------------------------------//
	bool SetQRCodeLibType(JET_BARCODE_LIB_TYPE val);//設定條碼函式庫-2D
	bool SetBarcode1DLibType(JET_BARCODE_LIB_TYPE val);//設定條碼函式庫-1D
	bool SetDataMatrixLibType(JET_BARCODE_LIB_TYPE val);//設定條碼函式庫-2D
	//---------------------------------------------------------------------------------//
	bool SetQRCodeDecoder(BARCODE_DECODER_TYPE val);//設定條碼解碼器-2D
	bool SetBarcode1DDecoder(BARCODE_DECODER_TYPE val);//設定條碼解碼器-1D
	bool SetDataMatrixDecoder(BARCODE_DECODER_TYPE val);//設定條碼解碼器-2D
	//---------------------------------------------------------------------------------//
	void SetDecodeTimeout(int val);//設定條碼解碼逾時時間
	void SetEnableCheckSum(bool val);//設定啟用CheckSum	
	void SetBarcodeDirection(JET_BARCODE_DIRECTION CodeDirection);//設定條碼方向-節省時間	
	//---------------------------------------------------------------------------------//	
	bool DecodeBarcode1D(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, char BarcodeContent[], size_t BarcodeSize);
	bool DecodeBarcodeDataMatrix(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, char BarcodeContent[], size_t BarcodeSize);
	bool DecodeBarcodeQRCode(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, char BarcodeContent[], size_t BarcodeSize);
	//---------------------------------------------------------------------------------//
	bool FilterBarcodeForJSON(char BarcodeContent[]);//過濾條碼內容-為了JSON格式
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_JETBARCODE_H__3C71C429_C27C_4C36_99D6_F29A14E770C4__INCLUDED_)
