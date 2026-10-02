#pragma once
#include "Barcode_Basic.h"
#include "JetSerial.h"   // RS232C application program

class CBarcode_Keyence_SR751 : public CBarcode_Basic
{
#define ESC		0x1B
#define STX		0x02
#define ETX		0x03
#define LF		0x0A
#define CR		0x0d

#define SERIAL_PORT_COUNT	1		// Number of COM ports used
#define BAUDRATE_COUNT		5		// Number of Baudrate Rate used
#define RECV_DATA_MAX		10240	// Length of Receive Data

	HANDLE m_rs232cHandle[SERIAL_PORT_COUNT];	// Array to store COM port handles used
												// Name of COM port used
												// Be careful to specify serial ports larger than COM9

	int Baud[BAUDRATE_COUNT] = { 115200, 57600, 38400, 19200, 9600 };
	bool m_binaryDataMode = false;	// Whether using binary data mode
private:
	char                        m_ESCChar;                //ESC 字元	
	char						m_SeparatorString[32];    //分割字元
	char						m_HeaderString[32];       //起頭 字元	Send
	char						m_TerminatorString[32];   //結束 字元	Send
	char						m_TriggerOnString[32];    //Trigger 字元	
	char						m_TriggerOffString[32];	  //Trigger Off 字元	
	char						m_DeviceNoRead[32];		  //No Read訊息
	CJetSerial					m_RS232COM;
	CString						m_portName[SERIAL_PORT_COUNT];
protected:
	CBarcode_Keyence_SR751(const CBarcode_Keyence_SR751 &device);
	CBarcode_Keyence_SR751& operator=(const CBarcode_Keyence_SR751 &device);
	void                        CloneBarcodeDevice_Keyence_SR751(const CBarcode_Keyence_SR751 &device);
#pragma region Initialze
	void                        PreInitBarcodeDevice_Keyence_SR751();
	void                        InitialBarcodeDevice_Keyence_SR751();
#pragma endregion
	bool 						ConnectDevice_Keyence_SR751();
	bool						Disconnect_Keyence_SR751();
	bool						StartToRead_Keyence_SR751();//Timing On
	bool 						EndReading_Keyence_SR751();//Timing Off
	bool 						WaitForDataInQuene_Keyence_SR751();
	bool 						RetrieveCode_Keyence_SR751();//Receive Data
	bool                        ExtractBarcodeContent_Keyence_SR751(const char *src, char *buffer, size_t bufferSize);
#pragma region Send Command/Set Param

	bool                        CheckResponse_Keyence_SR751(const char *response, int length);					//確認回傳資料
#pragma endregion
public:
	CBarcode_Keyence_SR751();
	virtual ~CBarcode_Keyence_SR751();
	virtual bool				CheckConnected();	//確認是否連線
	virtual bool				ConnectToDevice();	//連線至裝置
	virtual bool				Disconnected();		//斷線
	virtual void				ClearBuffer();

	virtual bool				LoadBarcodeINIFile();
	virtual bool				SaveBarcodeINIFile();
	virtual CString				LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	virtual bool				Initialize();
	virtual bool				StartToRead();				//Trigger ON
	virtual bool				EndReading();				//Trigger OFF
	virtual bool				RetrieveCode();				//接收裝置內的條碼
	virtual bool				AnalysisResultBuffer();		//分析結果字串	
	virtual bool				CloneResultBuffer(char *Buffer, size_t BufferSize);//複製結果暫存區	
	bool                        SendData_Keyence_SR751(const char *buffer, const char* response = NULL, int length = 0);	//傳送資料
};

