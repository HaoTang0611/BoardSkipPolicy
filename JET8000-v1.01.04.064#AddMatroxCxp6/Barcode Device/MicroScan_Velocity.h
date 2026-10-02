// MicroScan_Velocity.h: interface for the MicroScan_Velocity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROSCAN_VELOCITY_H__0082ED62_E4C1_4B45_826E_9EB938E95F6F__INCLUDED_)
#define AFX_MICROSCAN_VELOCITY_H__0082ED62_E4C1_4B45_826E_9EB938E95F6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarCode_CS.h"

class MicroScan_Velocity : public BarCode_CS  
{

public:
	MicroScan_Velocity();
	virtual ~MicroScan_Velocity();	

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

	virtual bool SetNMultiCodes(int NCodes);//設定條碼數目
	virtual bool  UpdateCodeSetting();//更新條碼解碼的設定

	bool TrunONCalibrationMode();	//chia044
	bool TrunOFFCalibrationMode();	//chia044
	bool DoCalibration();	//chia044
	
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
	
	char DATAOUTPUTSTATUS[8];	
/////////////////////////////////////////////
	//CString ErrMsg;
	//CString *m_Buffer;

	
	bool OnInitialize();
	bool OnTrigger();
	bool OnGetData();

	bool SetHostPortConnections();
	bool SetHostPortProtocol();
	bool SetScanner(bool);
	bool SetSTXETXChar();
	bool SetTriggerMode();
	bool SetExtTriggerEdge();	//chia 1000329
	bool SetCaptureMode();	//chia044
	bool SetEndCycleMode();
	bool SetCheckTimes();
	bool SetSymbolNum(int Symbol);
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

	bool StartCalibrationMode();
	bool EndCalibrationMode();
	bool OnDoCalibration();
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

#endif // !defined(AFX_MICROSCAN_VELOCITY_H__0082ED62_E4C1_4B45_826E_9EB938E95F6F__INCLUDED_)
