// Barcode_DataLogic_Matrix210.h: interface for the CBarcode_DataLogic_Matrix210 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_DATALOGIC_MATRIX210_H__F085DBF7_903F_4981_BD46_6C5F1647E00B__INCLUDED_)
#define AFX_BARCODE_DATALOGIC_MATRIX210_H__F085DBF7_903F_4981_BD46_6C5F1647E00B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "JetSerial.h"   // RS232C application program
#include "Barcode_Basic.h"
//-------------------------------------------------------------------------------------//
class CBarcode_DataLogic_Matrix210 : public CBarcode_Basic  
{
private:
	//---------------------------------------------------------------------------------//	 
	char                       m_ESCChar;                //ESC 字元	
	char                       m_SeparatorString[32];    //分割字元
	char                       m_HeaderString[32];       //起頭 字元	
	char                       m_TerminatorString[32];   //結束 字元	
	char                       m_TriggerOnString[32];    //Trigger 字元	
	char                       m_TriggerOffString[32];	 //Trigger Off 字元	
	char                       m_DeviceNoRead[32];		 //No Read訊息
	//---------------------------------------------------------------------------------//
	CJetSerial                 m_RS232COM;
	char                       m_BarcodeName[64];
	char                       m_BarcodeModel[64];
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CBarcode_DataLogic_Matrix210(const CBarcode_DataLogic_Matrix210 &device);
	CBarcode_DataLogic_Matrix210& operator=(const CBarcode_DataLogic_Matrix210 &device);
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcodeDevice_Matrix210();
	void                       InitialBarcodeDevice_Matrix210();
	void                       CloneBarcodeDevice_Matrix210(const CBarcode_DataLogic_Matrix210 &device);
	//---------------------------------------------------------------------------------//
	bool                       ConnectDevice_DataLogic_Matrix210();//連線
	bool                       Disconnect_DataLogic_Matrix210();//斷線
	bool                       CheckDevice_DataLogic_Matrix210();//確認硬體	
	bool                       CheckDevice_DataLogic_Matrix210_Kernel();//確認硬體	
	//---------------------------------------------------------------------------------//
	bool                       StartToRead_DataLogic_Matrix210();//開始讀取
	bool                       EndReading_DataLogic_Matrix210();//關閉讀取
	bool                       WaitForDataInQuene_DataLogic_Matrix210();//等待有資料進來
	bool                       RetrieveCode_DataLogic_Matrix210();//接收裝置內的條碼
	//---------------------------------------------------------------------------------//
	bool                       SetParam_DataLogic_Matrix210(const char *buffer);//傳送資料
	bool                       SendData_DataLogic_Matrix210(const char *buffer, const char* response);//傳送資料
	bool                       CheckResponse_DataLogic_Matrix210(const char *response);//確認回傳資料
	//---------------------------------------------------------------------------------//	
	bool                       ChangeToHostMode();//切換至主控設定模式
	bool                       ChangeToProgramMode();//切換至參數設定模式
	bool                       ExitHostMode();//離開Host模式
	bool                       ExitProgramMode();//離開參數設定模式
	bool                       ExitSingleProgram();//單一參數寫完	
	bool                       ExtractBarcodeContent_DataLogic_Matrix210(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容
	bool                       ExtractBarcodeContent_DataLogic_Matrix210_I(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容
	bool                       ExtractBarcodeContent_DataLogic_Matrix210_II(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容
	//---------------------------------------------------------------------------------//	
public:
	CBarcode_DataLogic_Matrix210();
	virtual ~CBarcode_DataLogic_Matrix210();
	//---------------------------------------------------------------------------------//
	//條碼機參數檔
	virtual bool               LoadBarcodeINIFile();
	virtual bool               SaveBarcodeINIFile();
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	//---------------------------------------------------------------------------------//
	virtual bool               CheckConnected();//確認是否連線
	virtual bool               ConnectToDevice();//連線至裝置
	virtual bool               Disconnected();//斷線
	//---------------------------------------------------------------------------------//
	virtual void               ClearBuffer();
	virtual bool               Initialize();
	//---------------------------------------------------------------------------------//
	virtual bool               StartToRead();
	virtual bool               EndReading();
	virtual bool               RetrieveCode();//接收裝置內的條碼
	//---------------------------------------------------------------------------------//	
	virtual bool               AnalysisResultBuffer();//分析結果字串	
	virtual bool               CloneResultBuffer(char *Buffer, size_t BufferSize);//複製結果暫存區
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODE_DATALOGIC_MATRIX210_H__F085DBF7_903F_4981_BD46_6C5F1647E00B__INCLUDED_)
