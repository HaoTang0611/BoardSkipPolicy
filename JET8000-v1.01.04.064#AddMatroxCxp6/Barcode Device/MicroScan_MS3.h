// MS3_CS.h: interface for the MS3_CS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MS3_CS_H__20AD60F4_1317_4DBF_AA67_5B21D919E07E__INCLUDED_)
#define AFX_MS3_CS_H__20AD60F4_1317_4DBF_AA67_5B21D919E07E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BarCode_CS.h"

//MS3 parameter
/*
#define TIMEOUT                 '0'
#define NEW_TRIGGER				'1'	
#define TIMEOUT_TRIGGER			'2'
#define CONTINUE_READ			'0'
#define CONTINUE_READ_1			'1'
#define SERIAL_DATA			    '4'
#define SERIAL_DATA_AND_EDGE	'5'

#define ENABLE_LASER_SCANNINT    'H'
#define DISABLE_LASER_SCANNINT   'I'

#define STX1                     "!"
#define ETX1                     "*"
#define SEPARATOR				 "+"
*/

class MicroScan_MS3 : public BarCode_CS  
{
public:
	MicroScan_MS3();
	virtual ~MicroScan_MS3();
	void Show();

	bool Initialize();
	bool Trigger();
	bool ReadData();
	bool Connected();
	bool GetCode_N(int ,char*,int); 
	bool SetComPort(int);
	int GetComPort();
	bool SetCodeLength(int len);
	bool EndRead();
	bool StartDevice();
	bool ChangeRecipe();

	bool TrunONCalibrationMode();	//chia044
	bool TrunOFFCalibrationMode();	//chia044
	bool DoCalibration();	//chia044
	
	virtual bool SetNMultiCodes(int NCodes);//設定條碼數目
	virtual bool  UpdateCodeSetting();//更新條碼解碼的設定

/////////////////////////////////////////////
	int m_Port;

	std::string m_HeaderChar;				//頭碼
	std::string m_TerminatorChar;			//未碼
	std::string m_NoRead;					//No Read訊息

	//char BAUDRATE[8];
	/*0 = 600 3 = 4800 6 = 38.4 K 

	1 = 1200 4 = 9600 7 = 57.6 K 

	2 = 2400 5 = 19.2 K 8 = 115.2 K 
	*/
	//char PARITY[8];
	//0 = None 1 = Even 2 = Odd 
	//char STOPBITS[8];
	//0 = One 1 = Two 
	//char DATABITS[8];
	//0 = Seven 1 = Eight 
	//char PROTOCOL[8];
	
	char TRIGGERCHAR[8];

	char TRIGGERMODE[8];	
	//0=Continuous Read ,1=Continuous Read 1 Output ,4=serial Data
	char ENDCYCLEMODE[8];		
	//0 = Timeout 1 = New Trigger 2 = Timeout & New Trigger 

	char READTIME[8];
	//100=1sec
	char CHECKTIME[8];
	
	char DATAOUTPUTSTATUS[8];
	
	bool OnInitialize();
	bool OnTrigger();
	bool OnGetData();

	bool SetHostPortConnections();
	bool SetHostPortProtocol();
	bool SetMotor(bool);
	bool SetSTXETXChar();
	bool SetTriggerMode();
	bool SetExtTriggerEdge();	//chia 1000329
	bool SetEndCycleMode();
	bool SetCheckTimes();
	bool SetSymbolNum(int Symbol);
	bool SetDataOutputStatus();
	bool SetMessage();
	bool SetTriggerChar();
	bool SetMultiCodeLength(int len);
	bool StartRead( const char *buffer, int size);
	bool SetBackgroundColor();
	bool SetCommandEchoStatus();
	bool SetOutputIO();
	bool SaveCurrentSetting();
	bool GetIsBarcodeConnect();

	bool SendMsg(char pMsg[], int size);
	bool ReadMsg(char pMsg[], int size);
	void ClearBuffer();	

	bool m_IsTrigger;
	CSerial m_RS232COM;

protected:	
	bool Import();
	bool Export();

private:
	bool SendCommandF(char *format, ...);
	bool SendCommand(const std::string &command);
	void ExportCodeId(int CodeId);
	void ImportCodeId(int CodeId);
	bool GetData();
	bool ReadMsg(std::string &rData);	//從RS232讀取
	bool IsComportSucc();	
};


#endif // !defined(AFX_MS3_CS_H__20AD60F4_1317_4DBF_AA67_5B21D919E07E__INCLUDED_)
