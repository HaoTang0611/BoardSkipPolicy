// Barcode_DataLogic_Matrix210N.h: interface for the CBarcode_DataLogic_Matrix210N class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_DATALOGIC_MATRIX210N_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_)
#define AFX_BARCODE_DATALOGIC_MATRIX210N_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JetSerial.h"   // RS232C application program
#include "Barcode_Basic.h"

class CBarcode_DataLogic_Matrix210N : public CBarcode_Basic  
{
private:
	char                       m_ESCChar;                //ESC 字元	
	char                       m_SeparatorString[32];    //分割字元
	char                       m_HeaderString[32];       //起頭 字元	
	char                       m_TerminatorString[32];   //結束 字元	
	char                       m_TriggerOnString[32];    //Trigger 字元	
	char                       m_TriggerOffString[32];	 //Trigger Off 字元	
	char                       m_DeviceNoRead[32];		 //No Read訊息
	CJetSerial                 m_RS232COM;				 //RS232	
protected:
	CBarcode_DataLogic_Matrix210N(const CBarcode_DataLogic_Matrix210N &device);
	CBarcode_DataLogic_Matrix210N& operator=(const CBarcode_DataLogic_Matrix210N &device);
	void                       CloneBarcodeDevice_Matrix210N(const CBarcode_DataLogic_Matrix210N &device);	
#pragma region Initialze
	void                       PreInitBarcodeDevice_Matrix210N();
	void                       InitialBarcodeDevice_Matrix210N();
#pragma endregion
#pragma region Connect/Disconnect RS232
	bool                       ConnectDevice_DataLogic_Matrix210N();//連線
	bool                       Disconnect_DataLogic_Matrix210N();//斷線
#pragma endregion
#pragma region Loading Barcode Data
	bool                       StartToRead_DataLogic_Matrix210N();			//開始讀取(Trigger ON)
	bool                       EndReading_DataLogic_Matrix210N();			//關閉讀取(Trigger OFF)
	bool                       WaitForDataInQuene_DataLogic_Matrix210N();	//等待有資料進來
	bool                       RetrieveCode_DataLogic_Matrix210N();			//接收裝置內的條碼
	bool                       ExtractBarcodeContent_DataLogic_Matrix210N(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容(去除Header、Termintory字串)
	bool                       ExtractBarcodeContent_DataLogic_Matrix210N_I(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容(去除Header、Termintory字串)
	bool                       ExtractBarcodeContent_DataLogic_Matrix210N_II(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容(去除Header、Termintory字串)
	bool                       ExtractBarcodeContent_DataLogic_Matrix210N_III(const char *src, char *buffer, size_t bufferSize);//萃取條碼內容(去除Header、Termintory字串)
#pragma endregion
#pragma region Send Command/Set Param
	bool                       SetParam_DataLogic_Matrix210N(const char *buffer);						//傳送資料
	bool                       SendData_DataLogic_Matrix210N(const char *buffer, const char* response);	//傳送資料
	bool                       CheckResponse_DataLogic_Matrix210N(const char *response);				//確認回傳資料
	bool                       ChangeToProgramMode();	//切換至參數設定模式
	bool                       ExitProgramMode();		//離開參數設定模式
	bool                       ExitSingleProgram();		//單一參數寫完	
#pragma endregion	
public:
	CBarcode_DataLogic_Matrix210N();
	virtual ~CBarcode_DataLogic_Matrix210N();
#pragma region Function of RS232
	virtual void			   SetCOMPort(unsigned int);
	virtual bool               CheckConnected();	//確認是否連線
	virtual bool               ConnectToDevice();	//連線至裝置
	virtual bool               Disconnected();		//斷線
	virtual void               ClearBuffer();
#pragma endregion
#pragma region Function of BarcodeReader
	virtual bool               LoadBarcodeINIFile();
	virtual bool               SaveBarcodeINIFile();
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
	//-------------------------------------------------------------------------------------------------------//
	virtual bool               Initialize();
	virtual bool               StartToRead();				//Trigger ON
	virtual bool               EndReading();				//Trigger OFF
	virtual bool               RetrieveCode();				//接收裝置內的條碼
	virtual bool               AnalysisResultBuffer();		//分析結果字串	
	virtual bool               CloneResultBuffer(char *Buffer, size_t BufferSize);//複製結果暫存區
#pragma endregion

};

#endif // !defined(AFX_BARCODE_DATALOGIC_MATRIX210N_H__CDC890E0_5810_4042_B744_88BC0173CA87__INCLUDED_)
