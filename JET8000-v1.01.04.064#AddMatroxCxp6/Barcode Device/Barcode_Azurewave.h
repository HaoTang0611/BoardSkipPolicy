// Barcode_Azurewave.h: interface for the CBarcode_Azurewave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_AZUREWAVE_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_)
#define AFX_BARCODE_AZUREWAVE_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "JetSerial.h"   // RS232C application program
#include "Barcode_Basic.h"
//-------------------------------------------------------------------------------------//
class CBarcode_Azurewave : public CBarcode_Basic  
{
private:	
	//---------------------------------------------------------------------------------//
	CJetSerial                 m_RS232COM;				 //RS232	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CBarcode_Azurewave(const CBarcode_Azurewave &device);
	CBarcode_Azurewave& operator=(const CBarcode_Azurewave &device);
	void                       CloneBarcodeDevice_Azurewave(const CBarcode_Azurewave &device);	
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcodeDevice_Azurewave();
	void                       InitialBarcodeDevice_Azurewave();
	//---------------------------------------------------------------------------------//
	bool                       ConnectDevice_Azurewave();//連線
	bool                       Disconnect_Azurewave();//斷線
	//---------------------------------------------------------------------------------//
	bool                       StartToRead_Azurewave();			//開始讀取(Trigger ON)
	bool                       EndReading_Azurewave(bool clrbuf);			//關閉讀取(Trigger OFF)
	bool                       WaitForDataInQuene_Azurewave();	//等待有資料進來
	bool                       RetrieveCode_Azurewave();			//接收裝置內的條碼
	bool                       ExtractBarcodeContent_Azurewave(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容(去除Header、Termintory字串)		
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CBarcode_Azurewave();
	virtual ~CBarcode_Azurewave();
	//---------------------------------------------------------------------------------//
	virtual void			   SetCOMPort(unsigned int);
	virtual bool               CheckConnected();	//確認是否連線
	virtual bool               ConnectToDevice();	//連線至裝置
	virtual bool               Disconnected();		//斷線
	virtual void               ClearBuffer();
	//---------------------------------------------------------------------------------//
	virtual bool               LoadBarcodeINIFile();
	virtual bool               SaveBarcodeINIFile();
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	//-------------------------------------------------------------------------------------------------------//
	virtual bool               Initialize();
	virtual bool               StartToRead();				//Trigger ON
	virtual bool               EndReading();				//Trigger OFF
	virtual bool               RetrieveCode();				//接收裝置內的條碼
	virtual bool               AnalysisResultBuffer();		//分析結果字串	
	virtual bool               CloneResultBuffer(char *Buffer, size_t BufferSize);//複製結果暫存區
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODE_AZUREWAVE_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_)
