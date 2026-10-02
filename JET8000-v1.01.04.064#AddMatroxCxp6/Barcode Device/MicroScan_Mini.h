// MicroScan_Mini.h: interface for the MicroScan_Mini class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROSCAN_MINI_H__D2B42055_7C96_4301_9865_D531C105398D__INCLUDED_)
#define AFX_MICROSCAN_MINI_H__D2B42055_7C96_4301_9865_D531C105398D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarCode_CS.h"

class MicroScan_Mini : public BarCode_CS  
{
public:
	MicroScan_Mini();
	virtual ~MicroScan_Mini();

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

	char TRIGGERCHAR[8];

	char TRIGGERMODE[8];	
	//0=Continuous Read ,1=Continuous Read 1 Output ,4=serial Data
	char ENDCYCLEMODE[8];		
	//0 = Timeout 1 = New Trigger 2 = Timeout & New Trigger 

	char READTIME[8];
	//100=1sec	
	char CHECKTIME[8];
	//char SYMBOLNUM[8];
	char DATAOUTPUTSTATUS[8];	
/////////////////////////////////////////////
	//CString ErrMsg;
	//CString *m_Buffer;

	
	bool OnInitialize();
	bool OnTrigger();
	bool SetHostPortConnections();
	bool SetHostPortProtocol();
	bool SetScanner(bool);
	bool SetSTXETXChar();
	bool SetTriggerMode();
	bool SetExtTriggerEdge();	//chia 1000329
	bool SetCaptureMode();	//chia044
	bool SetEndCycleMode();
	bool SetCheckTimes();
	bool SetSymbolNum(int SymbolNum);
	bool SetDataOutputStatus();
	bool SetMessage();
	bool SetTriggerChar();
	bool SetMultiCodeLength(int len);
	bool StartRead( const char *buffer, int size);
	bool SetBackgroundColor();	//chia044
	bool SetCommandEchoStatus();	//chia044
	bool SetImageOutput(const bool IsStore);	//chia044
	bool SetEZButton();	//chia044	
	bool SetTarget(bool IsON);	//chia044
	bool SetOutputIO();	//chia 1000329

	bool StartCalibrationMode();	//chia044
	bool EndCalibrationMode();		//chia044
	bool OnDoCalibration();		//chia044
	bool SaveCurrentSetting();	//chia047
	bool GetIsBarcodeConnect();	//chia047
	
	bool SendMsg(const char pMsg[], int size);
	bool ReadMsg(char pMsg[], int size);	
	void ClearBuffer();

	bool m_IsTrigger;

	CSerial m_RS232COM;
protected:	
	bool Import();
	bool Export();//Kai

private:
	bool SendCommandF(char *format, ...);
	bool SendCommand(const std::string &command);
	bool OnGetData();
	bool GetData();
	bool ReadMsg(std::string &rData);	//從RS232讀取條碼
	bool IsComportSucc();
};

#endif // !defined(AFX_MICROSCAN_MINI_H__D2B42055_7C96_4301_9865_D531C105398D__INCLUDED_)
