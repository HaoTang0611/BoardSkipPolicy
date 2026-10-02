// LightCtrlBoard.h: interface for the CLightCtrlBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCTRLBOARD_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
#define AFX_LIGHTCTRLBOARD_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "LightCtrlBoardDef.h"
//-------------------------------------------------------------------------------------//
class CLightCtrlBoardImp;
//-------------------------------------------------------------------------------------//
//燈源控制板
class CLightCtrlBoard  
{
public:
	static bool                BuildTableTypeCombox(CComboBox &Combox);//建立表格樣式
	static bool                BuildTable3DCastCombox(CComboBox &Combox);//建立3D投光編號
	static int                 GetTableDLPChannelMask(int DLPChannel);//取得表格內DLP通道的遮罩碼
private:
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;	
	CString                    m_ErrorStringOut;
	//---------------------------------------------------------------------------------//
	CLightCtrlBoardImp        *_Imp;//實際實現的指標
	bool                       GetImp() const;//取得_Imp指標
	bool                       CheckImp();//確認_Imp指標	
	bool                       CreateImp(LIGHT_CTRL_BOARD_IMP_CLS Cls);//建立_Imp指標	
	bool                       CreateImp_Fpga();//建立_Imp指標	
	bool                       CreateImp_Arduino();//建立_Imp指標	
	bool                       SwitchImp();//切換_Imp指標
	bool                       ReleaseImp();//釋放_Imp指標
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoard(const CLightCtrlBoard &CtrlBoard);
	CLightCtrlBoard& operator=(const CLightCtrlBoard &CtrlBoard);
	//---------------------------------------------------------------------------------//	
	CString                    GetErrorKeyName() const;	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoard();
	virtual ~CLightCtrlBoard();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();
	void                       SetErrorString(LPCTSTR Err);
	//---------------------------------------------------------------------------------//
	void                       SetLightCtrlBoardExceptionCode_Param(LPCTSTR Err=NULL);//設定系統錯誤代碼
	void                       SetLightCtrlBoardExceptionCode_FileRead(LPCTSTR Err=NULL);//設定系統錯誤代碼
	void                       SetLightCtrlBoardExceptionCode_FileWrite(LPCTSTR Err=NULL);//設定系統錯誤代碼
	void                       SetLightCtrlBoardExceptionCode(DWORD Code, LPCTSTR Err=NULL);//設定系統錯誤代碼
	//---------------------------------------------------------------------------------//
	LIGHT_CTRL_BOARD_IMP_CLS   GetLightCtrlBoardClass();	
	//---------------------------------------------------------------------------------//		
	bool                       Connect();
	bool                       Connect(int BoardID, char BoardName[], char BoardCode[]);
	bool                       GetIsConnected();
	bool                       CheckIsConnected();
	bool                       DisConnect();
	//---------------------------------------------------------------------------------//	
	int                        GetUSBBoardID();
	//---------------------------------------------------------------------------------//	
	int                        GetCameraCount() const;//取得相機數量
	int                        GetDLPCastCount() const;//取得DLP投光數量	
	int                        GetLEDChannelCount() const;//取得LED通道數量
	LIGHT_CTRL_BOARD_TYPE      GetLightCtrlBoardType() const;//取得控制板樣式
	CString                    GetLightCtrlBoardTypeText() const;//取得控制板樣式文字	
	bool                       ChangeLightCtrlBoardType(LIGHT_CTRL_BOARD_TYPE Type);//變更控制板樣式
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
	bool                       ReadErrorString(CString &Error);
	bool                       DecodeErrorCodeText(int ErrorCode, CString &ErrorStr);
	bool                       GetTriggerCountText(CString &TrigCount);
	bool                       CheckDLPNotReadySignalFault(int DlpIdx);//確認DLP沒有準備訊號
	bool                       CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr);
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetFPGAVersion() const;
	LPCTSTR                    GetFullVersion() const;
	bool                       ReadFPGAVersion(CString &Version);
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
	bool                       TriggerStart();	//TriggerStart
	bool                       ClearAllCount();	//ClearAllCount
	bool                       ClearAll();		//ClearAll
	//---------------------------------------------------------------------------------//
//--04 SET
	bool                       SwitchToFPGA();
	bool                       SwitchToRead();
	bool                       SwitchToWrite();	
	bool                       SwitchToAssign();
	bool                       SetMode_FPGA();		//FPGA Run
	bool                       SetMode_PCWrite();		//寫入
	bool                       SetMode_PCRead();		//讀取
	bool                       SetMode_PCAssign();	//指定位址

	bool                       WriteTableRunCount(int nTable);	//Trigger後要跑幾張TABLE
	bool                       ReadTableRunCount(int &nTable);
	int                        GetTableRunCount() const;//

	bool                       SetCCDTrigEdge(int TrigEdge=CCD_TRIG_EDGE_L);	//CCD Trigger Edge
	bool                       EnableDLPChannel(int DLPMask);//新增加的DLP開啟功能
	
	bool                       GetCCDTrigEdge(int &TrigEdge);

	bool                       SetTriggerFirstIndex(UINT Index);//處發表格第1張引數
	//---------------------------------------------------------------------------------//
	int                        GetDLPTableStartIndex() const;
	void                       SetDLPTableStartIndex(int val);	
	//---------------------------------------------------------------------------------//
	bool                       GetDLPInteralTrigger() const;
	bool                       GetDLPMultiTableEnabled() const;
	void                       SetDLPMultiTableEnabled(bool val);	
	//---------------------------------------------------------------------------------//
	//支援超過64個表格
	bool                       GetSupportMoreThan64Table() const;	
	//---------------------------------------------------------------------------------//
	//支援變更觸發開始引數
	bool                       GetSupportModifyTriggerFirstIndex() const;	
	//---------------------------------------------------------------------------------//
	//表格重複確認模式
	void                       SetTableRecheckEnabled(int val);
	int                        GetTableRecheckEnabled() const;
	//---------------------------------------------------------------------------------//
	void                       SetDLPCallbackTimeus(int val);
	int                        GetDLPCallbackTimeus() const;
	//---------------------------------------------------------------------------------//
	void                       SetDLPPulseWidthTimeus(int val);
	int                        GetDLPPulseWidthTimeus() const;
	//---------------------------------------------------------------------------------//
	void                       SetTableMinBetweenTimeus(int val);
	int                        GetTableMinBetweenTimeus() const;
	//---------------------------------------------------------------------------------//
	void                       SetPreCheckTableListEnabled(int val);
	int                        GetPreCheckTableListEnabled() const;
	//---------------------------------------------------------------------------------//
//--05 Table
	bool                       TableRAMAssign(UINT AssignAddress);//指定RAM位址	
	bool                       TableAssign(UINT TableIndex);	//指定RAM位址至某個Table的起始位址	
	bool                       TableRAMAddressAdd();			//RAM位址加1	
	bool                       TableSingleWrite(TLCB_TRIG_TABLE &Table);	//寫入一個Table
	bool                       TableSingleRead(TLCB_TRIG_TABLE &Table);		//讀出一個Table
	bool                       TableCompare(const TLCB_TRIG_TABLE &Table1, const TLCB_TRIG_TABLE &Table2);//比較2個Table
	//---------------------------------------------------------------------------------//
	bool                       TableListRead(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable);
	bool                       TableListReadFn(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable);
	bool                       TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList);
	bool                       TableListWriteFn(const std::vector<TLCB_TRIG_TABLE> &TableList);
	bool                       TableListCompare(const std::vector<TLCB_TRIG_TABLE> &TableList);	
	bool                       TableReset(int nTable);	
	bool                       ClearLCBTableList();//清空內部的列表
	bool                       ReMapTableList_ReadToWrite(const std::vector<TLCB_TRIG_TABLE> &TableListRead, std::vector<TLCB_TRIG_TABLE> &TableListWrite);	
	//---------------------------------------------------------------------------------//
//--USER
	int                        GetCurFPGAMode();
	bool                       DefaultTable(TLCB_TRIG_TABLE &Table);
	bool                       GetTableInfoText(TLCB_TRIG_TABLE &Table, CString &Info);	
	bool                       CheckTableEqually(size_t idx, const TLCB_TRIG_TABLE &TableIn, const TLCB_TRIG_TABLE &TableOut);//確認兩個表格是否相等
	bool                       CheckTableListEqually(const std::vector<TLCB_TRIG_TABLE> &TableListIn, const std::vector<TLCB_TRIG_TABLE> &TableListOut, std::vector<int> &NGTableList);//確認兩個表格列表是否相等	
	//---------------------------------------------------------------------------------//	
	bool                       WriteTableListToFile(LPCTSTR filename, std::vector<TLCB_TRIG_TABLE> &TableList, std::vector<int> &NGTableList);//將表格列表寫至檔案中
	//---------------------------------------------------------------------------------//
	bool                       BuildLCBTableFromSliceParamList(const std::vector<TSliceParam> &SliceParamList);//由SliceParm列表來建立燈盤表格列表
	bool                       BuildLCBTableFromSliceParamListFn(const std::vector<TSliceParam> &SliceParamList);//由SliceParm列表來建立燈盤表格列表	
	//---------------------------------------------------------------------------------//
	bool                       ConvertSliceParamListToLCBTableList(const std::vector<TSliceParam> &SliceParamList, std::vector<TLCB_TRIG_TABLE> &LCBTableList);//將Slice Param轉成控制板的表格
	bool                       ConvertSliceParamListToLCBTableList(const std::vector<TSliceParam> &SliceParamList, bool bUsing3D, std::vector<TLCB_TRIG_TABLE> &LCBTableList);//將Slice Param轉成控制板的表格	
	bool                       ConvertSliceParamListToLCBTableListFn(const std::vector<TSliceParam> &SliceParamList, bool bUsing3D, std::vector<TLCB_TRIG_TABLE> &LCBTableList);//將Slice Param轉成控制板的表格	
	bool                       ConvertSliceParamListToLCBTableList_DLPSingle(const std::vector<TSliceParam> &SliceParamList, std::vector<TLCB_TRIG_TABLE> &LCBTableList);//將Slice Param轉成控制板的表格
	bool                       ConvertSliceParamListToLCBTableList_DLPMultiple(const std::vector<TSliceParam> &SliceParamList, std::vector<TLCB_TRIG_TABLE> &LCBTableList);//將Slice Param轉成控制板的表格	
	//---------------------------------------------------------------------------------//
	bool                       ExecConveyorMotorStop(LANE_ID LaneID);//執行軌道停止
	bool                       ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow);//執行軌道運轉	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CLightCtrlBoard LightCtrlBoard;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHTCTRLBOARD_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
