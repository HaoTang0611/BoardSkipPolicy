// Barcode_Device_Module.h: interface for the CBarcode_Device_Module class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_DEVICE_MODULE_H__1AD7905C_0051_4845_99CF_E746D24DFC90__INCLUDED_)
#define AFX_BARCODE_DEVICE_MODULE_H__1AD7905C_0051_4845_99CF_E746D24DFC90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Barcode_Basic.h"
//-------------------------------------------------------------------------------------//
class CBarcode_Device_Module : public CBarcode_Basic  
{
private:
	//---------------------------------------------------------------------------------//	
	int                        m_BarcodeDeviceModuleID;//條碼機模組編號
	DWORD                      m_WaitTimeoutMs;//愈時時間ms
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CBarcode_Device_Module(const CBarcode_Device_Module &device);
	CBarcode_Device_Module& operator=(const CBarcode_Device_Module &device);
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcodeDevice_Module();
	void                       InitialBarcodeDevice_Module();
	void                       CloneBarcodeDevice_Module(const CBarcode_Device_Module &device);
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeDeviceModuleID(int val);//條碼機模組編號
	int                        GetBarcodeDeviceModuleID() const;//條碼機模組編號
	//---------------------------------------------------------------------------------//	
	bool                       CheckBarcodeDeviceModuleDefine();//確認條碼裝置模組定義
	//---------------------------------------------------------------------------------//
	bool                       ConnectToBarcodeDevice(bool Reset);//連線至裝置
	//---------------------------------------------------------------------------------//
	bool                       StartToReadBarcodeDevice();
	bool                       EndReadingBarcodeDevice();
	bool                       RetrieveCodeBarcodeDevice();//接收裝置內的條碼
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CBarcode_Device_Module();
	CBarcode_Device_Module(BARCODE_DEVICE_TYPE DeviceType);	
	virtual ~CBarcode_Device_Module();
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
//---------------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODE_DEVICE_MODULE_H__1AD7905C_0051_4845_99CF_E746D24DFC90__INCLUDED_)
