#ifndef _DATALOGIC_MATRIX210_H_
#define _DATALOGIC_MATRIX210_H_
//---------------------------------------------------------------------------------//
#include "BarCode_CS.h"
//---------------------------------------------------------------------------------//
class DataLogic_Matrix210 : public BarCode_CS
{
public:
	DataLogic_Matrix210();
	virtual ~DataLogic_Matrix210();
	
	bool SetComPort(int nPort);
	int	 GetComPort();
	bool Connected();
	bool Initialize();
	bool Trigger();
	bool ReadData();	
	bool SetCodeLength(int len);
	bool EndRead();	
	bool StartDevice();	
	void ClearBuffer();				//清除RS232 Buffer
	bool ChangeRecipe();	

	bool TrunONCalibrationMode();	//chia044
	bool TrunOFFCalibrationMode();	//chia044
	bool DoCalibration();	//chia044

	virtual bool SetNMultiCodes(int NCodes);
	virtual bool  UpdateCodeSetting();
protected:
	bool Import();
	bool Export();
private:
	int m_Port;	
	std::vector<std::string> m_vGain;	
	std::vector<std::string> m_vExposureTime;

	CSerial m_RS232COM;	
	//是否有Trigger
	//(0: trun-off and buffer is empty; 1: trun-off wait read buffer; 2: trun-on)
	int m_nTrigger;
	int  m_ProgramMode;             //是否為編成模式
	int  m_DelayTime;              //通訊的延遲時間

	//特殊字元設定	在PreInitial中設定
	char m_MyEsc;					//ESC
	std::string m_HeaderChar;				//頭碼
	std::string m_TerminatorChar;			//未碼
	std::string m_TriggerChar;			//Trigger 字元
	std::string m_TriggerOffChar;			//Trigger Off 字元
	std::string m_NoRead;					//No Read訊息

	int m_Recipe;
	int m_RecipeMax;	

	void PreInitial();	

	bool OnConnect();
	bool Disconnect();				//條碼機斷線
	bool OnTrigger();				//Trigger ON
	bool OFFTrigger();				//Trigger OFF
	bool Initial_BarcodeReader();	//條碼機初始設定
	bool DoInitial_BarcodeReader(); //條碼機初始設定
	bool Initial_Communication();	//條碼機連線參數設定
	bool Initial_MISCELLANEOUS();
	bool Initial_CalibrationRecipe(int RecipeId);//條碼機校正參數設定
	bool Initial_Calibration();		//條碼機校正參數設定
	bool Initial_2DCode();			//條碼類型設定
	bool Initial_1DCode();			//條碼類型設定
	bool Initial_1DCode_Status();	//條碼類型設定
	bool Initial_2DCode_Status();	//條碼類型設定
	inline bool SetParamCmd(const std::string &rHd);
	inline bool SetParamCmd(const std::string &rHd, int SharpN, int Data);
	inline bool SetParamCmd(const std::string &rHd, int V);
	inline bool SetParamCmd(const std::string &rHd, bool Enable);
	bool SetParam(std::string commond);	//設定條碼機參數
	inline bool SendDataCmd(const std::string &rHd, const std::string &Replay);
	bool SendData(const std::string &commend, const std::string &Replay);	//寫入至RS232
	bool CheckReply(const std::string &Replay); //確認回傳字元
	bool ChangeToProgramMode();		//切換至參數設定模式
	bool ExitProgramMode();			//離開參數設定模式
	bool ExitSingleProgram();		//單一參數寫入結束
	bool IsBarcodeConnected();		//Barcode連線確認
	bool GetData();					//取得條碼	
	bool ReadMsg(std::string &rData);	//從RS232讀取條碼	
	bool SetToNewRecipe(int No);
	
	bool SetSymbolNum(int CodeCnt, bool IsChangeToProgramMode);
	bool SetSymbolSeparator(bool IsChangeToProgramMode=true);
	void ExportCodeId(int CodeId);
	void ImportCodeId(int CodeId);
	bool CheckNMultiCodes(int CodeCnt);
	void CloseComport();
	bool IsConnectedSucc();
	bool FindStringDrop(std::string &rSrcDec, const std::string &rObject, bool FindForeward);
};
//---------------------------------------------------------------------------------//

#endif