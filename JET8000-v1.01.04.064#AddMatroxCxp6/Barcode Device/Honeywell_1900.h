// DataLogic_GFS4400.h: interface for the DataLogic_GFS4400 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HONETWELL_1900_H)
#define AFX_HONETWELL_1900_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarCode_CS.h"
#define SEPARATOR				 "+"
//---------------------------------------------------------------------------------//
//#define TRIGGER_MODE_ONE		0
//#define TRIGGER_MODE_CONTINUE	0

//---------------------------------------------------------------------------------//
class Honeywell_1900 : public BarCode_CS  
{
public:
	Honeywell_1900();
	virtual ~Honeywell_1900();	

	bool SetComPort(int nPort);
	int  GetComPort();
	bool Connected();
	bool Initialize();
	bool Trigger();
	bool ReadData();
	//bool GetCode_N(int ,char*,int);
	bool SetCodeLength(int len);
	bool EndRead();	
	bool StartDevice();	
	void ClearBuffer();				//清除RS232 Buffer
	bool ChangeRecipe();	

	bool TrunONCalibrationMode();	//chia044
	bool TrunOFFCalibrationMode();	//chia044
	bool DoCalibration();	//chia044

	virtual bool SetNMultiCodes(int NCodes);//設定條碼數目
	virtual bool  UpdateCodeSetting();
protected:
	bool Import();
	bool Export();
private:
	int m_Port;

	CSerial m_RS232COM;

	bool m_IsTrigger;				//是否有Trigger

	//特殊字元設定	在PreInitial中設定
	char m_StartCommand;			//$ 0x24
	char m_EndCommand;				//<CR> 0x0D	
	std::string m_TerminatorChar;			//未碼
	char m_TriggerChar;			//Trigger 字元	
	char m_TriggerOffChar;			//Trigger Off 字元	
	std::string m_NoRead;					//No Read訊息
	int   m_NSymbolRead;            //N Symbol Read

	int m_Recipe;
	int m_RecipeMax;	

	void PreInitial();	

	bool OnConnect();
	bool Disconnect();				//條碼機斷線
	bool OnTrigger();				//Trigger ON
	bool OFFTrigger();				//Trigger OFF
	bool Initial_BarcodeReader();	//條碼機初始設定
	bool Initial_Communication();	//條碼機連線參數設定
	bool Initial_MISCELLANEOUS();	
	bool Initial_Calibration();		//條碼機校正參數設定
	bool Initial_2DCode();			//條碼類型設定
	bool Initial_1DCode();			//條碼類型設定
	bool SetParamCmd(const std::string &Commond);
	bool SetParam(const std::string &Commond);	//設定條碼機參數	
	bool SendData(const std::string &commend);	//寫入至RS232	
	bool ExitProgramMode();			//離開參數設定模式
	bool ExitSingleProgram();		//單一參數寫入結束
	bool IsBarcodeConnected();		//Barcode連線確認
	bool GetData();					//取得條碼
	bool ReadMsg(char pMsg[], int size);	//從RS232讀取條碼
	bool ReadMsg(std::string &rData);
	bool GetHeaderChar();				//確認頭碼
	bool SetToNewRecipe(int No);

	bool SetSymbolNum(bool IsChangeToProgramMode=true);
	bool SetSymbolSeparator(bool IsChangeToProgramMode=true);

	bool ChangeAutoMode();
	bool ChangeTriggerMode();
	bool IsComportSucc();
};
#endif // !defined(AFX_HONETWELL_3310GHD_H)