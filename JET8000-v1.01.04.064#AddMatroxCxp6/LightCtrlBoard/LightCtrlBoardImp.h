// LightCtrlBoardFpga.h: interface for the CLightCtrlBoardImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCTRLBOARDIMP_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
#define AFX_LIGHTCTRLBOARDIMP_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "LightCtrlBoardDef.h"
//-------------------------------------------------------------------------------------//
//燈源控制板
class CLightCtrlBoardImp  
{
protected:
	//---------------------------------------------------------------------------------//	
	int                        m_usbBoardID;	
	CString                    m_FullVersion;
	CString                    m_FPGAVersion;	
	int                        m_FPGAMode;	
	int                        m_TableMaxCount;//表格數量上限
	int                        m_TableRunCount;//表格執行數量
	int                        m_CameraCount;
	int                        m_DLPCastCount;//DLP投光數量	
	int                        m_LEDChannelCount;
	int                        m_DLPTableStartIndex;//DLP表格起始引數	
	LIGHT_CTRL_BOARD_TYPE      m_LightCtrlBoardType;
	//---------------------------------------------------------------------------------//
	bool                       m_SupportMoreThan64Table;//支援超過64個表格
	bool                       m_SupportModifyTriggerFirstIndex;//支援變更觸發開始引數
	//---------------------------------------------------------------------------------//		
	bool                       m_DLPMultiTableEnabled;//DLP多表格模式		
	int                        m_TableRecheckEnabled;//表格重複確認模式
	int                        m_DLPCallbackTimeus;//DLP回傳時間
	int                        m_DLPPulseWidthTimeus;//DLP脈波寬度
	int                        m_TableMinBetweenTimeus;//表格最短間距時間
	int                        m_PreCheckTableListEnabled;//提前確認表格
	int                        m_DLPChannelMap[DLP_CHANNEL_COUNT];//DLP的編號映射
	//---------------------------------------------------------------------------------//
	CString                    m_ErrorString;	
	//---------------------------------------------------------------------------------//
	std::vector<TLCB_TRIG_TABLE> m_LCBTableList;//表格記錄, 用來減少不必要重複的設定
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
	CLightCtrlBoardImp(const CLightCtrlBoardImp &CtrlBoard);
	CLightCtrlBoardImp& operator=(const CLightCtrlBoardImp &CtrlBoard);
	//---------------------------------------------------------------------------------//		
	bool                       ReturnNotImplement(LPCTSTR fnName);//回傳未完成函式
	//---------------------------------------------------------------------------------//	
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//	
	void                       SetTableMaxCount(int val);//表格數量上限
	virtual bool               UpdateSupportFuncByVersion();//依據版本號更新支援功能
	CString                    GetLCBIniFileName() const;//取得控制板Ini的檔案名稱
	CString                    GetLCBIniSectionName(LIGHT_CTRL_BOARD_TYPE Type) const;//取得控制板Ini的Section名稱
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoardImp();
	virtual ~CLightCtrlBoardImp();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();
	//---------------------------------------------------------------------------------//
	bool                       SaveLightCtrlBoardProcess(LPCTSTR fnName);
	//---------------------------------------------------------------------------------//
	virtual bool               GetUseUSBCtrl() const=0;
	virtual bool               GetUseSocketCtrl() const=0;
	virtual LIGHT_CTRL_BOARD_IMP_CLS GetLightCtrlBoardClass() const=0; 	
	//---------------------------------------------------------------------------------//
	virtual bool               Connect();	
	virtual bool               Connect(const char *IPAddress, int IPPort);//連接到控制板-網址
	virtual bool               Connect(int BoardID, char BoardName[], char BoardCode[]);//連接到控制板-USB	
	virtual bool               GetIsConnected();	
	virtual bool               CheckIsConnected();		
	virtual bool               DisConnect();	
	//---------------------------------------------------------------------------------//	
	void                       SetUSBBoardID(int val);
	int                        GetUSBBoardID() const;	
	//---------------------------------------------------------------------------------//	
	int                        GetCameraCount() const;//取得相機數量
	int                        GetDLPCastCount() const;//取得DLP投光數量			
	int                        GetLEDChannelCount() const;//取得LED通道數量	
	//---------------------------------------------------------------------------------//	
	LIGHT_CTRL_BOARD_TYPE      GetLightCtrlBoardType() const;//取得控制板樣式
	CString                    GetLightCtrlBoardTypeText() const;//取得控制板樣式文字	
	bool                       WriteLightCtrlBoardType(LIGHT_CTRL_BOARD_TYPE Type);//寫入控制板樣式	
	//---------------------------------------------------------------------------------//	
	virtual bool               UpdateLightCtrlBoardType();//更新控制板樣式
	//---------------------------------------------------------------------------------//
	virtual bool               LoopBackTestAll();		
	virtual bool               LoopBackTest(int data, char DataWS[], char DataRA[], char DataRB[]);		
	virtual bool               LoopBackTestA(int data, char DataWS[], char DataR[]);
	virtual bool               LoopBackTestB(int data, char DataWS[], char DataR[]);	
	//---------------------------------------------------------------------------------//
	virtual bool               ReadLightCtrlBorad(const std::string &Address, std::string &Data);
	virtual bool               WriteLightCtrlBorad(const std::string &Address, const std::string &Data);	
	//---------------------------------------------------------------------------------//
	virtual bool               SaveLightCtrlBoardINI();//寫參數至檔案
	virtual bool               LoadLightCtrlBoardINI();//從檔案讀取參數	
	//---------------------------------------------------------------------------------//
	virtual bool               ReadRAMData(char data[]);
	virtual bool               ReadRAMAddress(char data[]);	
	//---------------------------------------------------------------------------------//	
	virtual bool               ReadErrorString(CString &Error);	
	//---------------------------------------------------------------------------------//	
	virtual bool               DecodeErrorCodeText(int ErrorCode, CString &ErrorStr);		
	//---------------------------------------------------------------------------------//
	virtual bool               GetTriggerCountText(CString &TrigCount);		
	//---------------------------------------------------------------------------------//
	virtual bool               CheckDLPNotReadySignalFault(int DlpIdx) const;//確認DLP沒有準備訊號
	virtual bool               CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr);	
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetFPGAVersion() const;
	LPCTSTR                    GetFullVersion() const;	
	virtual bool               ReadFPGAVersion(CString &Version);
	//---------------------------------------------------------------------------------//
	virtual bool               GetIsWorkingNow(bool &Working);		
	virtual bool               GetIsFPGAOK(bool &IsOK);	
	virtual bool               SetDLPEnable(UINT DLPChannel);		
	//---------------------------------------------------------------------------------//
	virtual bool               ReadBoardAllCount();
	virtual bool               GetPCtoFPGATrigCount(UINT &count);		
	virtual bool               GetFPGAtoCCDTrigCount(UINT &count);		
	virtual bool               GetFPGAtoCCDTrigCount(UINT CameraID, UINT &count);			
	virtual bool               GetFPGAtoDLPTrigCount(UINT DLPChannel, UINT &count);		
	virtual bool               GetDLPtoFPGATrigCount(UINT DLPChannel, UINT &count);		
	//---------------------------------------------------------------------------------//
//--03
	//---------------------------------------------------------------------------------//
	virtual bool               TriggerStart();//開始觸發	
	//---------------------------------------------------------------------------------//
	virtual bool               ClearAllCount();	//清除所有數量	
	//---------------------------------------------------------------------------------//
	virtual bool               ClearAll();//清除全部資料	
	//---------------------------------------------------------------------------------//
//--04 SET
	//---------------------------------------------------------------------------------//
	virtual bool               SwitchToFPGA();		
	virtual bool               SwitchToRead();	
	virtual bool               SwitchToWrite();	
	virtual bool               SwitchToAssign();	
	//---------------------------------------------------------------------------------//
	virtual bool               SetMode_FPGA();//FPGA Run		
	virtual bool               SetMode_PCWrite();//寫入		
	virtual bool               SetMode_PCRead();//讀取		
	virtual bool               SetMode_PCAssign();//指定位址	
	//---------------------------------------------------------------------------------//
	virtual bool               WriteTableRunCount(int nTable);	//Trigger後要跑幾張TABLE	
	virtual bool               ReadTableRunCount(int &nTable);		
	void                       SetTableRunCount(int val) { m_TableRunCount = val; }
	int                        GetTableRunCount() const { return m_TableRunCount; }//	
	//---------------------------------------------------------------------------------//
	virtual bool               SetCCDTrigEdge(int TrigEdge=CCD_TRIG_EDGE_L);	//CCD Trigger Edge
	//---------------------------------------------------------------------------------//
	virtual bool               EnableDLPChannel(int DLPMask);//新增加的DLP開啟功能	
	//---------------------------------------------------------------------------------//	
	virtual bool               GetCCDTrigEdge(int &TrigEdge);	
	//---------------------------------------------------------------------------------//
	virtual bool               SetTriggerFirstIndex(UINT Index);//處發表格第1張引數	
	//---------------------------------------------------------------------------------//
	int                        GetTableMaxCount() const;//表格數量上限
	//---------------------------------------------------------------------------------//
	int                        GetDLPTableStartIndex() const;	
	void                       SetDLPTableStartIndex(int val);		
	//---------------------------------------------------------------------------------//
	bool                       GetDLPInteralTrigger() const;
	//---------------------------------------------------------------------------------//
	bool                       GetDLPMultiTableEnabled() const;	
	void                       SetDLPMultiTableEnabled(bool val);	
	//---------------------------------------------------------------------------------//	
	//支援超過64個表格
	bool                       GetSupportMoreThan64Table() const;	
	void                       SetSupportMoreThan64Table(bool val);	
	//---------------------------------------------------------------------------------//
	//支援變更觸發開始引數
	bool                       GetSupportModifyTriggerFirstIndex() const;	
	void                       SetSupportModifyTriggerFirstIndex(bool val);	
	//---------------------------------------------------------------------------------//
	//表格重複確認模式
	void                       SetTableRecheckEnabled(int val) { m_TableRecheckEnabled = val; }
	int                        GetTableRecheckEnabled() const { return m_TableRecheckEnabled; }	
	//---------------------------------------------------------------------------------//
	//DLP回傳逾時時間-us
	void                       SetDLPCallbackTimeus(int val) { m_DLPCallbackTimeus = val; }
	int                        GetDLPCallbackTimeus() const { return m_DLPCallbackTimeus; }	
	//---------------------------------------------------------------------------------//
	//DLP脈波寬度時間-us
	void                       SetDLPPulseWidthTimeus(int val) { m_DLPPulseWidthTimeus = val; }
	int                        GetDLPPulseWidthTimeus() const { return m_DLPPulseWidthTimeus; }	
	//---------------------------------------------------------------------------------//
	//表格最小間距時間-us
	void                       SetTableMinBetweenTimeus(int val) { m_TableMinBetweenTimeus = val; }
	int                        GetTableMinBetweenTimeus() const { return m_TableMinBetweenTimeus; }	
	//---------------------------------------------------------------------------------//
	//預先確認表格列表
	void                       SetPreCheckTableListEnabled(int val) { m_PreCheckTableListEnabled = val; }
	int                        GetPreCheckTableListEnabled() const { return m_PreCheckTableListEnabled; }
	//---------------------------------------------------------------------------------//
//--05 Table
	virtual bool               TableRAMAssign(UINT AssignAddress);//指定RAM位址	
	virtual bool               TableAssign(UINT TableIndex);	//指定RAM位址至某個Table的起始位址	
	virtual bool               TableRAMAddressAdd();			//RAM位址加1	
	//---------------------------------------------------------------------------------//
	virtual bool               TableSingleWrite(TLCB_TRIG_TABLE &Table);//寫入一個Table		
	virtual bool               TableSingleRead(TLCB_TRIG_TABLE &Table);//讀出一個Table	
	//---------------------------------------------------------------------------------//
	virtual bool               TableCompare(const TLCB_TRIG_TABLE &Table1, const TLCB_TRIG_TABLE &Table2);//比較2個Table
	//---------------------------------------------------------------------------------//
	virtual bool               TableListRead(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable);		
	virtual bool               TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList);	
	//---------------------------------------------------------------------------------//
	virtual bool               TableListCompare(const std::vector<TLCB_TRIG_TABLE> &TableList);	
	//---------------------------------------------------------------------------------//
	virtual bool               TableReset(int nTable);
	//---------------------------------------------------------------------------------//	
	bool                       ClearLCBTableList();//清空內部的列表	
	int                        GetLCBTableCount() const;	
	bool                       AddLCBTable(const TLCB_TRIG_TABLE &rTable);//加入內部表格	
	bool                       SetLCBTableList(const std::vector<TLCB_TRIG_TABLE> &rList);
	//---------------------------------------------------------------------------------//
	bool                       ReMapTableList_ReadToWrite(const std::vector<TLCB_TRIG_TABLE> &TableListRead, std::vector<TLCB_TRIG_TABLE> &TableListWrite);		
	//---------------------------------------------------------------------------------//
//--USER
	virtual int                GetCurFPGAMode() const;	
	//---------------------------------------------------------------------------------//
	virtual bool               DefaultTable(TLCB_TRIG_TABLE &Table);	
	//---------------------------------------------------------------------------------//
	virtual bool               GetTableInfoText(TLCB_TRIG_TABLE &Table, CString &Info);	
	virtual bool               CheckTableEqually(size_t idx, const TLCB_TRIG_TABLE &TableIn, const TLCB_TRIG_TABLE &TableOut);//確認兩個表格是否相等	
	virtual bool               CheckTableListEqually(const std::vector<TLCB_TRIG_TABLE> &TableListIn, const std::vector<TLCB_TRIG_TABLE> &TableListOut, std::vector<int> &NGTableList);//確認兩個表格列表是否相等		
	//---------------------------------------------------------------------------------//	
	virtual bool               WriteTableListToFile(LPCTSTR filename, std::vector<TLCB_TRIG_TABLE> &TableList, std::vector<int> &NGTableList);//將表格列表寫至檔案中	
	//---------------------------------------------------------------------------------//	
	virtual bool               ExecConveyorMotorStop(LANE_ID LaneID);//執行軌道停止
	virtual bool               ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow);//執行軌道運轉	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHTCTRLBOARDIMP_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
