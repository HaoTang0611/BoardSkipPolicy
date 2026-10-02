// Barcode_Honeywell_3310GHD.h: interface for the CBarcode_Honeywell_3310GHD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_HONEYWELL_3310GHD_H__B578E791_DE37_4A34_82B1_9C7EE5F1150A__INCLUDED_)
#define AFX_BARCODE_HONEYWELL_3310GHD_H__B578E791_DE37_4A34_82B1_9C7EE5F1150A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "JetSerial.h"   // RS232C application program
#include "Barcode_Basic.h"
//-------------------------------------------------------------------------------------//
class CBarcode_Honeywell_3310GHD : public CBarcode_Basic  
{
private:
	//---------------------------------------------------------------------------------//
	char                       m_StartCommand; //$ 0x24
	char                       m_EndCommand;	   //<CR> 0x0D	
	char                       m_SeparatorChar;//未碼
	char                       m_TriggerChar;    //Trigger 字元	
	char                       m_TriggerOffChar;	//Trigger Off 字元	
	char                       m_DeviceNoRead[32];		//No Read訊息	
	//---------------------------------------------------------------------------------//
	CJetSerial                 m_RS232COM;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CBarcode_Honeywell_3310GHD(const CBarcode_Honeywell_3310GHD &device);
	CBarcode_Honeywell_3310GHD& operator=(const CBarcode_Honeywell_3310GHD &device);
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcodeDevice_3310GHD();
	void                       InitialBarcodeDevice_3310GHD();
	void                       CloneBarcodeDevice_3310GHD(const CBarcode_Honeywell_3310GHD &device);
	//---------------------------------------------------------------------------------//
	bool                       ConnectDevice_Honeywell_3310GHD();//連線
	bool                       Disconnect_Honeywell_3310GHD();//斷線
	//---------------------------------------------------------------------------------//
	bool                       StartToRead_Honeywell_3310GHD();//開始讀取
	bool                       EndReading_Honeywell_3310GHD();//關閉讀取
	bool                       WaitForDataInQuene_Honeywell_3310GHD();//等待有資料進來
	bool                       RetrieveCode_Honeywell_3310GHD();//接收裝置內的條碼
	//---------------------------------------------------------------------------------//
	bool                       SendData_Honeywell_3310GHD(const char *buffer);//傳送資料
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CBarcode_Honeywell_3310GHD();
	virtual ~CBarcode_Honeywell_3310GHD();
	//---------------------------------------------------------------------------------//
	//條碼機參數檔
	virtual bool               LoadBarcodeINIFile();
	virtual bool               SaveBarcodeINIFile();
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	//---------------------------------------------------------------------------------//
	virtual bool               CheckConnected();//確認是否連線
	virtual bool               ConnectToDevice();//連線至裝置
	virtual bool               Disconnected();//斷線
	//---------------------------------------------------------------------------------//
	virtual void               ClearBuffer();
	virtual bool               Initialize();
	//---------------------------------------------------------------------------------//
	virtual bool               StartToRead();
	virtual bool               EndReading();
	virtual bool               RetrieveCode();//接收裝置內的條碼
	//---------------------------------------------------------------------------------//	
	virtual bool               AnalysisResultBuffer();//分析結果字串	
	virtual bool               CloneResultBuffer(char *Buffer, size_t BufferSize);//複製結果暫存區
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODE_HONEYWELL_3310GHD_H__B578E791_DE37_4A34_82B1_9C7EE5F1150A__INCLUDED_)
