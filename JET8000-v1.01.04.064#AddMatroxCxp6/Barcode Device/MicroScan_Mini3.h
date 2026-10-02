// MicroScan_Mini3.h: interface for the MicroScan_Mini3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROSCAN_MINI3_H__393EEDD4_2B00_4FDE_960E_D6158355583F__INCLUDED_)
#define AFX_MICROSCAN_MINI3_H__393EEDD4_2B00_4FDE_960E_D6158355583F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarCode_CS.h"

class MicroScan_Mini3 : public BarCode_CS  
{
public:
	MicroScan_Mini3();
	virtual ~MicroScan_Mini3();	

	bool Initialize();
	bool Trigger();
	bool ReadData();
	bool Connected();
	bool GetCode_N(int ,char*,int); 
	bool SetComPort(int);
	int GetComPort();
	bool SetCodeLength(int len);
	bool StartDevice();
	bool EndRead();
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

	//
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
	bool OnInitialize();
	bool OnTrigger();
	bool OnGetData();

	bool SetHostPortConnections();
	bool SetHostPortProtocol();
	bool SetScanner(bool);
	bool SetSTXETXChar();
	bool SetTriggerMode();
	bool SetCaptureMode();
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
	bool SetImageOutput(const bool IsStore);
	bool SetEZButton();
	bool SetTarget(bool IsON);

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
	bool GetData();
	bool ReadMsg(std::string &rData);	//從RS232讀取條碼
	bool IsComportSucc();
};

#endif // !defined(AFX_MICROSCAN_MINI3_H__393EEDD4_2B00_4FDE_960E_D6158355583F__INCLUDED_)
