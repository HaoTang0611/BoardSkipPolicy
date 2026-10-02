// Barcode_Basic.h: interface for the CBarcode_Basic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_BASIC_H__91D0BF9D_9745_45EE_9DBB_C19D95AB68E2__INCLUDED_)
#define AFX_BARCODE_BASIC_H__91D0BF9D_9745_45EE_9DBB_C19D95AB68E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Barcode_Define.h"
//-------------------------------------------------------------------------------------//
class CBarcode_Basic  
{
private:
	//---------------------------------------------------------------------------------//
	int                        m_BarcodeDeviceID;//條碼機編號
	BARCODE_DEVICE_TYPE        m_BarcodeDeviceType;//條碼機樣式
	CString                    m_BarcodeDeviceModel;//條碼機型號	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	bool                       m_BarcodeDeviceConnected;//裝置連線中
	bool                       m_BarcodeDeviceActived;//裝置啟用中
	//---------------------------------------------------------------------------------//
	CString                    m_BarcodeDevicePort;
	int                        m_BarcodeDeviceParity;
	int                        m_BarcodeDeviceStopBits;
	int                        m_BarcodeDeviceBaudRate;
	DWORD                      m_BarcodeDeviceCommDelayTime;//通訊延遲時間
	//---------------------------------------------------------------------------------//
	CString                    m_ErrorString;
	CString                    m_ErrorStringOut;//錯誤字串	
	//---------------------------------------------------------------------------------//	
	int                        m_BarcodeDeviceWaitDataCount;//等待資料的次數
	DWORD                      m_BarcodeDeviceWaitDataDwellTime;//等待資料的延遲時間-ms
	DWORD                      m_BarcodeDeviceReadDataDelayTime;//讀取資料前延遲時間-ms
	//---------------------------------------------------------------------------------//	
	//目前讀取到的條碼內容
	char                       m_ResultBuffer[MAX_BARCODE_DEVICE_RECEIVE_SIZE];
	char                       m_ResultBufferSubList[BARCODE_SUB_COUNT][BARCODE_SIZE];	
	//---------------------------------------------------------------------------------//
	//備份上一次讀取到的條碼內容
	char                       m_ResultBackup[MAX_BARCODE_DEVICE_RECEIVE_SIZE];
	char                       m_ResultBackupSubList[BARCODE_SUB_COUNT][BARCODE_SIZE];
	//---------------------------------------------------------------------------------//
	CBarcode_Basic(const CBarcode_Basic &device);
	CBarcode_Basic& operator=(const CBarcode_Basic &device);
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcodeDevice();
	void                       InitialBarcodeDevice();
	void                       CloneBarcodeDevice(const CBarcode_Basic &device);
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeDeviceConnected(bool val) { m_BarcodeDeviceConnected = val; }
	//---------------------------------------------------------------------------------//
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CBarcode_Basic();
	virtual ~CBarcode_Basic();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();
	//---------------------------------------------------------------------------------//
	bool                       GetBarcodeDeviceConnected() const { return m_BarcodeDeviceConnected; }
	//---------------------------------------------------------------------------------//	
	//條碼機編號
	void                       SetBarcodeDeviceID(int val) { m_BarcodeDeviceID = val; }
	int                        GetBarcodeDeviceID() const { return m_BarcodeDeviceID; }
	//---------------------------------------------------------------------------------//
	//條碼機樣式
	void                       SetBarcodeDeviceType(BARCODE_DEVICE_TYPE val) { m_BarcodeDeviceType = val; }
	BARCODE_DEVICE_TYPE        GetBarcodeDeviceType() const { return m_BarcodeDeviceType; }
	//---------------------------------------------------------------------------------//
	//條碼機型號
	void                       SetBarcodeDeviceModel(LPCTSTR val) { m_BarcodeDeviceModel = val; }
	LPCTSTR                    GetBarcodeDeviceModel() const { return m_BarcodeDeviceModel; }
	//---------------------------------------------------------------------------------//	
	CString                    GetBarcodeDeviceFullName() const;	
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeDevicePort(LPCTSTR val) { m_BarcodeDevicePort = val; }
	LPCTSTR                    GetBarcodeDevicePort() const { return m_BarcodeDevicePort; }
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeDeviceParity(int val) { m_BarcodeDeviceParity = val; }
	int                        GetBarcodeDeviceParity() const { return m_BarcodeDeviceParity; }
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeDeviceStopBits(int val) { m_BarcodeDeviceStopBits = val; }
	int                        GetBarcodeDeviceStopBits() const { return m_BarcodeDeviceStopBits; }
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeDeviceBaudRate(int val) { m_BarcodeDeviceBaudRate = val; }
	int                        GetBarcodeDeviceBaudRate() const { return m_BarcodeDeviceBaudRate; }
	//---------------------------------------------------------------------------------//
	//裝置啟用中
	void                       SetBarcodeDeviceActived(bool val) { m_BarcodeDeviceActived = val; }
	bool                       GetBarcodeDeviceActived() const { return m_BarcodeDeviceActived; }
	//---------------------------------------------------------------------------------//
	//條碼機參數檔
	virtual bool               LoadBarcodeINIFile();
	virtual bool               SaveBarcodeINIFile();

	virtual bool               SaveBarcodeMovingTimeMsg(const char *pContext);//儲存條碼機移動時間訊息
	virtual bool               SaveBarcodeMovingTimeMsg(const wchar_t *pContext);//儲存條碼機移動時間訊

	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	//---------------------------------------------------------------------------------//
	virtual bool               CheckConnected()=0;//確認是否連線
	virtual bool               ConnectToDevice()=0;//連線至裝置
	virtual bool               Disconnected()=0;//斷線
	//---------------------------------------------------------------------------------//
	virtual void               ClearBuffer()=0;//清除緩衝區
	virtual bool               Initialize()=0;//初始化條碼機
	//---------------------------------------------------------------------------------//
	virtual bool               StartToRead()=0;//開始讀取條碼
	virtual bool               EndReading()=0;//停止讀取條碼
	virtual bool               RetrieveCode()=0;//接收裝置內的條碼
	//---------------------------------------------------------------------------------//	
	void                       ClearResultBuffer();    
	void                       SetResultBuffer(const char* String);
	const char*                GetResultBuffer() const;	
	int                        GetResultSubCount() const; 
	bool                       SetResultSubBuffer(int idx, const char* String);
	const char*                GetResultSubBuffer(int idx) const;	
	virtual bool               AnalysisResultBuffer()=0;//分析結果字串	
	virtual bool               CloneResultBuffer(char *Buffer, size_t BufferSize)=0;//複製結果暫存區
	//---------------------------------------------------------------------------------//
	void                       BackupResultBuffer();//備份條碼機內容
	void                       ClearResultBackup();//清除備份條碼機內容	
	const char*                GetResultSubBackup(int idx) const;//取得備份條碼機內容
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODE_BASIC_H__91D0BF9D_9745_45EE_9DBB_C19D95AB68E2__INCLUDED_)
