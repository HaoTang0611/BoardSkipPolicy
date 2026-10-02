// Barcode_General.h: interface for the CBarcode_General class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_GENERAL_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_)
#define AFX_BARCODE_GENERAL_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "JetSerial.h"   // RS232C application program
#include "Barcode_Basic.h"
//-------------------------------------------------------------------------------------//
class CBarcode_General : public CBarcode_Basic  
{
private:	
	//---------------------------------------------------------------------------------//
	int                        m_DeviceID;//裝置編號
	CJetSerial                 m_RS232COM;//RS232	
	std::string                m_HexTriggerOn;//16進位-啟動
	std::string                m_HexTriggerOff;//16進位-關閉	
	std::string                m_HexCodePrefix;//16進位-條碼前綴
	std::string                m_HexCodeSuffix;//16進位-條碼後綴
	std::string                m_HexCodeSeparator;//16進位-條碼分隔碼	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CBarcode_General(const CBarcode_General &device);
	CBarcode_General& operator=(const CBarcode_General &device);
	void                       CloneBarcodeDevice_General(const CBarcode_General &device);	
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcodeDevice_General();
	void                       InitialBarcodeDevice_General();
	//---------------------------------------------------------------------------------//
	bool                       ConnectDevice_General();//連線
	bool                       Disconnect_General();//斷線
	//---------------------------------------------------------------------------------//	
	bool                       StartToRead_General();			//開始讀取(Trigger ON)
	bool                       EndReading_General(bool clrbuf);			//關閉讀取(Trigger OFF)
	bool                       WaitForDataInQuene_General();	//等待有資料進來
	bool                       RetrieveCode_General();			//接收裝置內的條碼
	bool                       ExtractBarcodeContent_General(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容(去除Header、Termintory字串)		
	//---------------------------------------------------------------------------------//	
	bool                       SendData_General(const char *buffer, const char* response);//傳送資料	
	bool                       ConvertHexToString(const char *buffer, char Buf[], size_t BufSize, size_t &Count);//建立傳送字串	
	bool                       CheckResponse_General(const char *response);//確認回傳資料
	//---------------------------------------------------------------------------------//
	bool                       HexStringToByte(const char buf[], unsigned char &val);//16進位文字轉成位元祖
	bool                       ByteToHexString(unsigned char val, char buf[], size_t size);//位元祖轉成16進位的文字	
	//---------------------------------------------------------------------------------//
	bool                       ExtractSubCode(const char *Barcode, const char *Separator, std::vector<std::string> &CodeList);
	bool                       RemoveOtherWords(const char *Code, const char *Prefix, const char *Suffix, std::string &NewCode);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CBarcode_General(int DevicdID);
	virtual ~CBarcode_General();
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
	//16進位-開始觸發
	const char*                GetHexTriggerOn() const;
	void                       SetHexTriggerOn(const char *val);	
	//---------------------------------------------------------------------------------//
	//16進位-結束觸發
	const char*                GetHexTriggerOff() const;
	void                       SetHexTriggerOff(const char *val);	
	//---------------------------------------------------------------------------------//
	//16進位-條碼前綴
	const char*                GetHexCodePrefix() const;
	void                       SetHexCodePrefix(const char *val);	
	//---------------------------------------------------------------------------------//
	//16進位-條碼後綴
	const char*                GetHexCodeSuffix() const;
	void                       SetHexCodeSuffix(const char *val);	
	//---------------------------------------------------------------------------------//
	//16進位-條碼分隔碼
	const char*                GetHexCodeSeparator() const;
	void                       SetHexCodeSeparator(const char *val);	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODE_GENERAL_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_)
