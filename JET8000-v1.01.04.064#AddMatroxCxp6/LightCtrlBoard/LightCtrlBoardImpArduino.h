// LightCtrlBoardFpga.h: interface for the CLightCtrlBoardImpArduino class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCTRLBOARDIMPARDUINO_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
#define AFX_LIGHTCTRLBOARDIMPARDUINO_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "JetSocket.h"
#include "JetUSBCtrl.h"
#include "LightCtrlBoardImp.h"
//-------------------------------------------------------------------------------------//
#define ARDUINO_MAX_CCD_COUNT        9
#define ARDUINO_MAX_DLP_COUNT        DLP_CHANNEL_COUNT
//-------------------------------------------------------------------------------------//
#define ARDUINO_SUB_BOARD_COUNT      4//控制板-子板數量
#define ARDUINO_MAX_LED_COUNT       16//控制板-LED數量
//-------------------------------------------------------------------------------------//
//燈源控制板
class CLightCtrlBoardImpArduino : public CLightCtrlBoardImp 
{
private:
	//---------------------------------------------------------------------------------//
	CRITICAL_SECTION           m_csSocket;
	int                        m_IPPort;//網路埠
	std::string                m_IPAdress;//網址	
	CJetSocketClient           m_Socket;	
	std::string                m_AllTableStringW;//寫入的表格字串	
	unsigned int               m_SocketTimeout;//網路逾時(ms)
	unsigned int               m_TriggerTableCount;//觸發表格數量
	unsigned int               m_TriggerTableIndex;//開始觸發引數		
	//---------------------------------------------------------------------------------//
	int                        m_ConveyerDirection;
	bool                       m_ConveyerSpeedSlow;	
	//---------------------------------------------------------------------------------//
	bool                       m_SubBoardValid[ARDUINO_SUB_BOARD_COUNT];//子板是否有效
	bool                       m_LedChannelValid[ARDUINO_MAX_LED_COUNT];//LED通道是否有效
	//---------------------------------------------------------------------------------//
	bool                       m_DLPNotReadySignal[ARDUINO_MAX_DLP_COUNT];//DLP沒有準備訊號
	//---------------------------------------------------------------------------------//
	UINT                       m_PCtoFPGATrigCount;//PC送給控制板觸發次數
	UINT                       m_FPGAtoCCDTrigCount[ARDUINO_MAX_CCD_COUNT];//控制板送給相機觸發次數
	UINT                       m_FPGAtoDLPTrigCount[ARDUINO_MAX_DLP_COUNT];//控制板送給DLP觸發次數
	UINT                       m_DLPtoFPGATrigCount[ARDUINO_MAX_DLP_COUNT];//DLP送給控制板觸發次數
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoardImpArduino(const CLightCtrlBoardImpArduino &CtrlBoard);
	CLightCtrlBoardImpArduino& operator=(const CLightCtrlBoardImpArduino &CtrlBoard);
	//---------------------------------------------------------------------------------//	
	CString                    GetSocketErrorString();
	//---------------------------------------------------------------------------------//
	bool                       LockBoard();//同步化
	bool                       UnlockBoard();//同步化
	//---------------------------------------------------------------------------------//
	unsigned int               GetSocketTimeout() const;//網路逾時(ms)
	void                       SetSocketTimeout(unsigned int val);//網路逾時(ms)
	//---------------------------------------------------------------------------------//
	void                       ResetDLPNotReadySignal();//復歸DLP沒有準備訊號		
	void                       DecodeDLPNotReadySignal(const std::string &Str);//解碼DLP沒有準備訊號
	//---------------------------------------------------------------------------------//
	bool                       ShowTrace(const std::string &Str);//顯示訊息	
	bool                       CheckSocketTimeout(long time);//確認Socket逾時
	bool                       CheckStringValid(const char *Str);//確認字串是否有效	
	bool                       CheckStringValid(const std::string &Str);//確認字串是否有效	
	bool                       CheckStringLen(size_t Pos, size_t Len);//確認字串長度
	bool                       CheckStringLen(const std::string &Str, size_t Pos);//確認字串長度
	int                        HexStrToInt(const std::string &Str) const;//16進位制字串轉成整數
	std::string                IntToBinStr(int Val, int Cnt, char profix='0') const;//整數轉成02進位制字串	
	std::string                IntToHexStr(int Val, int Cnt, char profix='0') const;//整數轉成16進位制字串	
	size_t                     FindString(const std::string &str, const std::string &Sub, int Num=1, int Start=0);//尋找子字串		
	bool                       PacketCommandString(const std::string &Str, std::string &Packet);//封裝成命令字串//Cmd--<Cmd>
	bool                       UnpacketCommandString(const std::string &Str, std::string &Packet);//解封成命令字串//<Cmd>--Cmd
	bool                       PacketUploadEndString(const std::string &Str, std::string &Packet);//封裝成上傳結束字串//[Table1][Table2]...[TableN]@	
	bool                       CopySubString(const char *Str, size_t Len, size_t Pos, size_t SubLen, char *Buf);//複製子字串	
	bool                       CopySubString(const std::string &Str, size_t Pos, size_t SubLen, std::string &Buf);//複製子字串
	//---------------------------------------------------------------------------------//
	bool                       SocketWaitAck();//等待板子回應
	bool                       SocketRead(std::string &Result);//讀取Socket		
	bool                       CheckFaultString(const std::string &Result);//確認回傳錯誤	
	bool                       SocketWriteCmd(const std::string &Str, bool bWaitAck);//寫入命令至Socket	
	bool                       SocketWriteData(const std::string &Str, bool bWaitAck);//寫入資料至Socket	
	bool                       SocketWriteRaw(const std::string &Str, bool bWaitAck);//寫入至Socket
	//---------------------------------------------------------------------------------//
	bool                       ExecBoardCmd(const std::string &Cmd);//執行控制板命令	
	bool                       ExecBoardCmdFn(const std::string &Cmd);//執行控制板命令	
	bool                       ReadBoardData(const std::string &Cmd, std::string &Result);//讀取控制板資料
	bool                       ReadBoardDataFn(const std::string &Cmd, std::string &Result);//讀取控制板資料
	bool                       WriteBoardData(const std::string &Cmd, const std::string &Result);//寫入控制板資料	
	bool                       WriteBoardAllTable();//寫入控制板全部表格
	bool                       WriteBoardAllTable(const std::string &Data);//寫入控制板全部表格
	bool                       WriteBoardAllTableFn(const std::string &Data);//寫入控制板全部表格
	bool                       ReadBoardAllChannel();//讀取控制板所有通道
	bool                       ReadBoardAllTable(std::vector<TLCB_TRIG_TABLE> &rList);//讀取控制板所有表格	
	bool                       ReadBoardAllTableFn(std::vector<TLCB_TRIG_TABLE> &rList);//讀取控制板所有表格	
	bool                       ReadBoardTableCount(int &Count);//讀取控制板的表格數
	bool                       ReadBoardTableCountFn(int &Count);//讀取控制板的表格數
	//---------------------------------------------------------------------------------//	
	bool                       ParserTableCount(const std::string &str, int &Count);//剖析數量	
	bool                       ParserBoardError(const std::string &Str, std::string &Error, bool &IsError);//剖析子板錯誤
	bool                       DecodeBoardError(const std::string &Str, std::string &Error, bool &IsError, size_t &EndPos);//剖析子板錯誤
	bool                       DecodeOtherError(const std::string &Str, size_t LastPos, std::string &Error, bool &IsError);//剖析其餘錯誤
	int                        ParserStringCount(int Mode, int Channel, const std::string &str);//剖析數量	
	bool                       ParserTableString(const std::string &Str, std::vector<TLCB_TRIG_TABLE> &rTableList);//解析表格字串
	bool                       Parser2LedItemString(const std::string &Str, TLCB_LED_ITEM &PWM1, TLCB_LED_ITEM &PWM2);//解析兩個LED字串
	//---------------------------------------------------------------------------------//	
	unsigned int               GetTriggerTableCount() const;//觸發表格數量
	void                       SetTriggerTableCount(unsigned int val);//觸發表格數量
	unsigned int               GetTriggerTableIndex() const;//觸發表格起始引數
	void                       SetTriggerTableIndex(unsigned int val);//觸發表格起始引數	
	bool                       ClearAllCountValue();//清除所有計數
	//---------------------------------------------------------------------------------//		
	virtual bool               UpdateSupportFuncByVersion();//依據版本號更新支援功能	
	bool                       ResetSubBoardLedChannelValid();//清除子板與LED通道有效	
	//---------------------------------------------------------------------------------//	
	bool                       CheckUploadData(const std::string &Str, char ch='*');//確認上傳資料未填滿
	unsigned int               CombineLedItem2(const TLCB_LED_ITEM &PWM1, const TLCB_LED_ITEM &PWM2);//合併兩個LED電源參數	
	//---------------------------------------------------------------------------------/
	void                       ClearAllTableStringW();//清除表格文字-寫入
	const std::string&         GetAllTableStringW() const;//取得表格文字-寫入
	void                       AddAllTableStringW(const std::string &Str);//加入新的表格文字-寫入
	//---------------------------------------------------------------------------------/	
	bool                       Table2String(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//表格轉字串
	bool                       Table2String_Type(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//D:DLP, Others:LED
	bool                       Table2String_DlpTrgCnt(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//DLP Trigger Count:[1]
	bool                       Table2String_DlpChanne(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//DLP Channel:[1~8]
	bool                       Table2String_DlpEnbRdy(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//DLP Ready Mode[1,0];
	bool                       Table2String_NextTableTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//Table Between Time
	bool                       Table2String_LedOnTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//LED Turn On Time
	bool                       Table2String_CcdDelayTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//CCD Delay Time
	bool                       Table2String_LedChPwr(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//LED Channel Power
	bool                       Table2String_CameraEnable(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//Camera Enable
	bool                       Table2String_DlpPulseTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//DLP Pulse Time
	bool                       Table2String_DlpCallbackTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//DLP Callback Time
	bool                       Table2String_DlpExposureTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//DLP Exposure Time
	bool                       Table2String_DlpReadyTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr);//DLP Wait Ready Time
	//---------------------------------------------------------------------------------//
	bool                       String2Table(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//字串轉表格
	bool                       String2Table_Type(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//D:DLP, Others:LED
	bool                       String2Table_DlpTrgCnt(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//DLP Trigger Count:[1]
	bool                       String2Table_DlpChannel(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//DLP Channel:[1~8]
	bool                       String2Table_DlpEnbRdy(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//DLP Ready Mode[1,0];
	bool                       String2Table_NextTableTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//Table Between Time
	bool                       String2Table_LedOnTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//LED Turn On Time
	bool                       String2Table_CcdDelayTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//CCD Delay Time
	bool                       String2Table_LedChPwr(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//LED Channel Power
	bool                       String2Table_CameraEnable(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//Camera Enable
	bool                       String2Table_DlpPulseTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//DLP Pulse Time
	bool                       String2Table_DlpCallbackTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//DLP Callback Time
	bool                       String2Table_DlpExposureTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//DLP Exposure Time
	bool                       String2Table_DlpReadyTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable);//DLP Wait Ready Time
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoardImpArduino();
	virtual ~CLightCtrlBoardImpArduino();
	//---------------------------------------------------------------------------------//	
	bool                       GetUseUSBCtrl() const { return false; }
	bool                       GetUseSocketCtrl() const { return true; }
	LIGHT_CTRL_BOARD_IMP_CLS   GetLightCtrlBoardClass() const { return LIGHT_CTRL_BOARD_IMP_ARDUINO; }
	//---------------------------------------------------------------------------------//	
	bool                       Connect();	
	bool                       Connect(const char *IPAddress, int IPPort);//連接到控制板-網址
	bool                       GetIsConnected();
	bool                       CheckIsConnected();
	bool                       DisConnect();	
	//---------------------------------------------------------------------------------//	
	void                       SetIPPort(int val);
	int                        GetIPPort() const;	
	//---------------------------------------------------------------------------------//	
	void                       SetIPAddress(const char *str);
	const char*                GetIPAddress() const;	
	//---------------------------------------------------------------------------------//	
	int                        GetCameraCount() const;//取得相機數量
	int                        GetDLPCastCount() const;//取得DLP投光數量
	int                        GetLEDChannelCount() const;//取得LED通道數量	
	//---------------------------------------------------------------------------------//	
	bool                       UpdateLightCtrlBoardType();//更新控制板樣式
	//---------------------------------------------------------------------------------//
	bool                       LoopBackTestAll();
	bool                       LoopBackTest(int data, char DataWS[], char DataRA[], char DataRB[]);
	bool                       LoopBackTestA(int data, char DataWS[], char DataR[]);	
	bool                       LoopBackTestB(int data, char DataWS[], char DataR[]);	
	//---------------------------------------------------------------------------------//	
	bool                       ReadLightCtrlBorad(const std::string &Address, std::string &Data);
	bool                       WriteLightCtrlBorad(const std::string &Address, const std::string &Data);	
	//---------------------------------------------------------------------------------//
	bool                       SaveLightCtrlBoardINI();//寫參數至檔案
	bool                       LoadLightCtrlBoardINI();//從檔案讀取參數	
	//---------------------------------------------------------------------------------//
	bool                       ReadRAMData(char data[]);
	bool                       ReadRAMAddress(char data[]);	
	//---------------------------------------------------------------------------------//	
	bool                       ReadErrorString(CString &Error);	
	bool                       ReadErrorString(std::string &Err);
	//---------------------------------------------------------------------------------//	
	bool                       DecodeErrorCodeText(int ErrorCode, CString &ErrorStr);	
	bool                       GetTriggerCountText(CString &TrigCount);		
	//---------------------------------------------------------------------------------//
	bool                       CheckDLPNotReadySignalFault(int DlpIdx) const;//確認DLP沒有準備訊號
	bool                       CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr);	
	//---------------------------------------------------------------------------------//	
	bool                       ReadFPGAVersion(CString &Version);
	//---------------------------------------------------------------------------------//
	bool                       GetIsWorkingNow(bool &Working);
	bool                       GetIsFPGAOK(bool &IsOK);	
	bool                       SetDLPEnable(UINT DLPChannel);	
	//---------------------------------------------------------------------------------//
	bool                       ReadBoardAllCount();
	bool                       GetPCtoFPGATrigCount(UINT &count);	
	bool                       GetFPGAtoCCDTrigCount(UINT &count);	
	bool                       GetFPGAtoCCDTrigCount(UINT CameraID, UINT &count);		
	bool                       GetFPGAtoDLPTrigCount(UINT DLPChannel, UINT &count);	
	bool                       GetDLPtoFPGATrigCount(UINT DLPChannel, UINT &count);		
	//---------------------------------------------------------------------------------//
//--03
	//---------------------------------------------------------------------------------//
	bool                       TriggerStart();//開始觸發
	bool                       ClearAllCount();	//清除所有數量
	bool                       ClearAll();//清除全部資料	
	//---------------------------------------------------------------------------------//
//--04 SET
	bool                       SwitchToFPGA();
	bool                       SwitchToRead();
	bool                       SwitchToWrite();
	bool                       SwitchToAssign();	
	//---------------------------------------------------------------------------------//
	bool                       SetMode_FPGA();//FPGA Run
	bool                       SetMode_PCWrite();//寫入
	bool                       SetMode_PCRead();//讀取
	bool                       SetMode_PCAssign();//指定位址	
	//---------------------------------------------------------------------------------//
	bool                       WriteTableRunCount(int nTable);	//Trigger後要跑幾張TABLE	
	bool                       ReadTableRunCount(int &nTable);		
	//---------------------------------------------------------------------------------//
	bool                       SetCCDTrigEdge(int TrigEdge=CCD_TRIG_EDGE_L);	//CCD Trigger Edge	
	//---------------------------------------------------------------------------------//
	bool                       EnableDLPChannel(int DLPMask);//新增加的DLP開啟功能	
	//---------------------------------------------------------------------------------//	
	bool                       GetCCDTrigEdge(int &TrigEdge);	
	//---------------------------------------------------------------------------------//
	bool                       SetTriggerFirstIndex(UINT Index);//處發表格第1張引數	
	//---------------------------------------------------------------------------------//
//--05 Table
	bool                       TableRAMAssign(UINT AssignAddress);//指定RAM位址	
	bool                       TableAssign(UINT TableIndex);	//指定RAM位址至某個Table的起始位址	
	bool                       TableRAMAddressAdd();			//RAM位址加1	
	//---------------------------------------------------------------------------------//
	bool                       TableSingleWrite(TLCB_TRIG_TABLE &Table);//寫入一個Table
	bool                       TableSingleRead(TLCB_TRIG_TABLE &Table);//讀出一個Table	
	//---------------------------------------------------------------------------------//	
	bool                       TableListRead(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable);		
	bool                       TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList);	
	//---------------------------------------------------------------------------------//	
	bool                       TableReset(int nTable);		
	//---------------------------------------------------------------------------------//
	bool                       ExecConveyorMotorStop(LANE_ID LaneID);//執行軌道停止
	bool                       ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow);//執行軌道運轉	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHTCTRLBOARDIMPARDUINO_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
