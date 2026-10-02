// DataLogic_1000.h: interface for the DataLogic_1000 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATALOGIC_1000_H__8C180E62_0DE5_4E93_81A7_33094C89100C__INCLUDED_)
#define AFX_DATALOGIC_1000_H__8C180E62_0DE5_4E93_81A7_33094C89100C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BarCode_CS.h"
#define SEPARATOR				 "+"
//---------------------------------------------------------------------------------//
//#define TRIGGER_MODE_ONE		0
//#define TRIGGER_MODE_CONTINUE	0

//---------------------------------------------------------------------------------//
class DataLogic_1000 : public BarCode_CS  
{
public:
	DataLogic_1000();
	virtual ~DataLogic_1000();	

	bool SetComPort(int nPort);
	int GetComPort();
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
	std::vector<std::string> m_vGain;	
	std::vector<std::string> m_vExposureTime;

	CSerial m_RS232COM;	
	int m_nTrigger;				//是否有Trigger

	//特殊字元設定	在PreInitial中設定
	char m_MyEsc;					//ESC
	std::string m_HeaderChar;			//頭碼
	std::string m_TerminatorChar;		//未碼
	std::string m_TriggerChar;			//Trigger 字元	
	std::string m_TriggerOffChar;		//Trigger Off 字元	
	std::string m_NoRead;				//No Read訊息

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
	bool Initial_CalibrationRecipe(int Id);
	bool Initial_Calibration();		//條碼機校正參數設定
	bool Initial_2DCode();			//條碼類型設定
	bool Initial_1DCode();			//條碼類型設定
	inline bool SetParamCmd(const std::string &rHd);
	inline bool SetParamCmd(const std::string &rHd, int SharpN, int Data);
	inline bool SetParamCmd(const std::string &rHd, int V);
	inline bool SetParamCmd(const std::string &rHd, bool Enable);

	bool SetParam(const std::string &commond);	//設定條碼機參數
	bool SendCommand(const std::string &commond);
	bool SendData(const std::string &commend);	//寫入至RS232
	bool ChangeToProgramMode();		//切換至參數設定模式
	bool ExitProgramMode();			//離開參數設定模式
	bool ExitSingleProgram();		//單一參數寫入結束
	bool IsBarcodeConnected();		//Barcode連線確認
	bool GetData();					//取得條碼
	bool ReadMsg(std::string &rData);		//從RS232讀取條碼	
	//bool ReadMsg(char pMsg[], int size);	//從RS232讀取條碼
	//bool GetHeaderChar();					//確認頭碼
	bool SetToNewRecipe(int No);

	bool SetSymbolNum(int CodeCnt, bool IsChangeToProgramMode);
	bool SetSymbolSeparator(bool IsChangeToProgramMode=true);

	bool IsConnectedSucc();
	bool FindStringDrop(std::string &rSrcDec, const std::string &rObject, bool FindForeward);
};
#endif // !defined(AFX_DATALOGIC_1000_H__8C180E62_0DE5_4E93_81A7_33094C89100C__INCLUDED_)
