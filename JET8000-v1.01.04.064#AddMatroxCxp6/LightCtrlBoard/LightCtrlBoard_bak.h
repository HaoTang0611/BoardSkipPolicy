// LightCtrlBoard.h: interface for the CLightCtrlBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCTRLBOARD_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
#define AFX_LIGHTCTRLBOARD_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "JetUSBCtrl.h"
//-------------------------------------------------------------------------------------//
// Address(Write)
#define W_LOOPBACKA				0x01
#define W_LOOPBACKB				0x02
#define W_WRITE					0x03
#define W_MODE_SET				0x04
#define W_TABLE_SET				0x05
#define W_READ_RAM_ADD			0x06


#define W_DLP_ENABLE			0x09

// Address(Read)
#define R_LOOPBACKA				0x01
#define R_LOOPBACKB				0x02
#define R_RAM_DATA				0x06
#define R_RAM_ADDRESS			0x07
#define R_ERROR					0x08

#define R_WORKING_NOW			0x09
#define R_FPGA_OK				0x09

#define R_DLP_ERR_CH            0x0A

#define R_PC2FPGA_TRIGCOUNT		0x0B
#define R_FPGA2CCD_TRIGCOUNT	0x0C

#define R_FPGA2DLP_TRIGCOUNT	0x0D
//#define R_FPGA2DLP_TRIGCOUNT	0x0E
//#define R_FPGA2DLP_TRIGCOUNT	0x0F
//#define R_FPGA2DLP_TRIGCOUNT	0x10
//#define R_FPGA2DLP_TRIGCOUNT	0x11
//#define R_FPGA2DLP_TRIGCOUNT	0x12
//#define R_FPGA2DLP_TRIGCOUNT	0x13
//#define R_FPGA2DLP_TRIGCOUNT	0x14

#define R_DLP2FPGA_TRIGCOUNT	0x15
//#define R_DLP2FPGA_TRIGCOUNT	0x16
//#define R_DLP2FPGA_TRIGCOUNT	0x17
//#define R_DLP2FPGA_TRIGCOUNT	0x18
//#define R_DLP2FPGA_TRIGCOUNT	0x19
//#define R_DLP2FPGA_TRIGCOUNT	0x1A
//#define R_DLP2FPGA_TRIGCOUNT	0x1B
//#define R_DLP2FPGA_TRIGCOUNT	0x1C

#define R_DLP_TRIG_STEP         0x38
#define R_FPGA_VERSION			0x3F


//SET DATA
#define SET_DATA_FPGA_MODE					0x0000	//0000 0000 0000 0000 
#define SET_DATA_PC_WRITE_MODE				0x0004	//0000 0000 0000 0100
#define SET_DATA_PC_READ_MODE				0x0002	//0000 0000 0000 0010 
#define SET_DATA_PC_ADDRESS_ASSIGN_MODE		0x0006	//0000 0000 0000 0110 
 
//FPGA MODE
#define FPGA_MODE_UNDEFINED 0
#define FPGA_MODE_FPGA		1
#define FPGA_MODE_WRITE		2
#define FPGA_MODE_READ		3
#define FPGA_MODE_ASSIGN	4

#define FPGA_TABLE_SIZE		17//10進制

#define TABLE_TYPE_NONE		0
#define TABLE_TYPE_LED		1
#define TABLE_TYPE_DLP		2

//#define MAX_TABLE_COUNT		54//For DLP Exposure x 1
#define MAX_TABLE_COUNT		84//For DLP Exposure x 2

#define DLP_CHANNEL_1		1
#define DLP_CHANNEL_2		2
#define DLP_CHANNEL_3		3
#define DLP_CHANNEL_4		4
#define DLP_CHANNEL_5		5
#define DLP_CHANNEL_6		6
#define DLP_CHANNEL_7		7
#define DLP_CHANNEL_8		8

#define DLP_CHANNEL_COUNT   9

#define	CCD_TRIG_EDGE_L		0
#define	CCD_TRIG_EDGE_H		1
//-------------------------------------------------------------------------------------//
enum LIGHT_CTRL_BOARD_TYPE
{
	LIGHT_CTRL_BOARD_NULL = 0,
	LIGHT_CTRL_BOARD_3DA6 = 1,
	LIGHT_CTRL_BOARD_8DA1 = 2,
	LIGHT_CTRL_BOARD_RETURN
};
//-------------------------------------------------------------------------------------//
//LCB->Light Control Board
typedef struct _LCB_LED_ITEM
{
	bool sIsON;
	BYTE sPower;	//%
	_LCB_LED_ITEM()
	{
		sIsON  = false;
		sPower = 0;
	}
} TLCB_LED_ITEM, *PLED_ITEM;
//-------------------------------------------------------------------------------------//
typedef struct _LCB_TRIG_TABLE
{
	int  sTableID;

//(1)
	BYTE sTableType;			//(bit15~13)	//TABLE_TYPE_LED、TABLE_TYPE_DLP
	BYTE sDLPTrigOutNumber;		//(bit12~8)		//DLP觸發數量
	BYTE sDLPActiveChannel;		//(bit7~0)		//8個DLP每個Table最多只能啟動一個DLP
	UINT sDLPPhasePatMode;                      //DLP 相位樣板模式-Kai

//(2)(3)
	UINT sNextTableTime;		//(2)(bit15~0) (3)(bit31~16)	//Table間距時間, 第一個Table要設0

//(4)(5)
	UINT sLEDTableTotalTime;	//(4)(bit15~0) (5)(bit31~16)	//LED Table總時間 (CCD延遲時間+CCD曝光時間)

//(6)
	UINT sCCDDelayTime;			//燈亮至相機觸發的時間(us)	//DLP Type時，CCD觸發訊號的延遲時間(us)

//(7)
	TLCB_LED_ITEM sPWM1;			//(bit7~0)	 bit7=>ON/OFF, 6~0=>Power(%)
	TLCB_LED_ITEM sPWM2;			//(bit15~8)	 bit15=>ON/OFF, 14~8=>Power(%)
//(8)
	TLCB_LED_ITEM sPWM3;			//(bit7~0)
	TLCB_LED_ITEM sPWM4;			//(bit15~8)
//(9)
	TLCB_LED_ITEM sPWM5;			//(bit7~0)
	TLCB_LED_ITEM sPWM6;			//(bit15~8)
//(10)
	TLCB_LED_ITEM sPWM7;			//(bit7~0)
	TLCB_LED_ITEM sPWM8;			//(bit15~8)
//(11-v2)
	TLCB_LED_ITEM sPWM9;			//(bit7~0) //for 8DA1
	TLCB_LED_ITEM sPWM10;			//(bit15~8)
//(12-v2)
	TLCB_LED_ITEM sPWM11;			//(bit7~0) //for 8DA1
	TLCB_LED_ITEM sPWM12;			//(bit15~8)
//(13-v2)
	UINT          sCameraEnable;    //(bit7~0) //for 8DA1

//(14-v2)
	UINT sDLPPulseTime;			//DLP Pulse Width的時間(us)
//(15-v2)(16-v2)
	UINT sDLPCallbackTime;		//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常
//(17-v2)
	UINT sDLPCCDExpTime;		//DLP的CCD觸光時間(us)

} TLCB_TRIG_TABLE, *PLCB_TRIG_TABLE;
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
	CJetUSBCtrl                m_USB;	
	int                        m_usbBoardID;
	CString                    m_usbBoardName;
	CString                    m_usbBoardCode;
	CString                    m_FullVersion;
	CString                    m_FPGAVersion;	
	int                        m_FPGAMode;	
	int                        m_TableRunCount;	
	int                        m_CameraCount;
	int                        m_DLPCastCount;//DLP投光數量	
	int                        m_LEDChannelCount;
	int                        m_DLPTableStartIndex;//DLP表格起始引數
	CRITICAL_SECTION           m_csLightCtrlBoard;
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
protected:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoard(const CLightCtrlBoard &CtrlBoard);
	CLightCtrlBoard& operator=(const CLightCtrlBoard &CtrlBoard);
	//---------------------------------------------------------------------------------//
	bool                       UpdateSupportFuncByVersion();//依據版本號更新支援功能
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoard();
	virtual ~CLightCtrlBoard();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();
	//---------------------------------------------------------------------------------//		
	bool                       Connect(int BoardID);
	bool                       Connect(int BoardID, char BoardName[], char BoardCode[]);
	bool                       GetIsConnected();
	bool                       DisConnect();
	//---------------------------------------------------------------------------------//
	void                       SetUSBBoardID(int val);
	int                        GetUSBBoardID() const;
	//---------------------------------------------------------------------------------//	
	int                        GetCameraCount() const;//取得相機數量
	int                        GetDLPCastCount() const;//取得DLP投光數量	
	int                        GetLEDChannelCount() const;//取得LED通道數量
	LIGHT_CTRL_BOARD_TYPE      GetLightCtrlBoardType() const;//取得控制板樣式
	CString                    GetLightCtrlBoardTypeText() const;//取得控制板樣式文字
	bool                       UpdateLightCtrlBoardType();//更新控制板樣式
	//---------------------------------------------------------------------------------//
	bool                       LoopBackTestAll();
	bool                       LoopBackTest(int data, char DataWS[], char DataRA[], char DataRB[]);
	bool                       LoopBackTestA(int data, char DataWS[], char DataR[]);
	bool                       LoopBackTestB(int data, char DataWS[], char DataR[]);
	//---------------------------------------------------------------------------------//
	bool                       ExecUSBRead(char Address[], char Data[]);
	bool                       ExecUSBWrite(char Address[], char Data[]);
	//---------------------------------------------------------------------------------//
	bool                       SaveLightCtrlBoardINI();//寫參數至檔案
	bool                       LoadLightCtrlBoardINI();//從檔案讀取參數	
	//---------------------------------------------------------------------------------//
	bool                       ReadRAMData(char data[]);
	bool                       ReadRAMAddress(char data[]);	
	bool                       ReadErrorCode(int &Error);
	bool                       ReadDLPErrCH(CString &ErrDlp);
	bool                       ReadDLPTrigStepInfo(CString &DLPTrigInfo);
	bool                       DecodeErrorCodeText(int ErrorCode, CString &ErrorStr);
	bool                       GetTriggerCountText(CString &TrigCount);
	bool                       CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr);
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetFPGAVersion() const;
	LPCTSTR                    GetFullVersion() const;
	bool                       ReadFPGAVersion(CString &Version);
	bool                       GetIsWorkingNow(bool &Working);
	bool                       GetIsFPGAOK(bool &IsOK);
	bool                       GetPCtoFPGATrigCount(UINT &count);
	bool                       GetFPGAtoCCDTrigCount(UINT &count);
	bool                       GetFPGAtoCCDTrigCount(UINT CameraID, UINT &count);
	bool                       SetDLPEnable(UINT DLPChannel);
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

	bool                       SetTableRunCount(int nTable);	//Trigger後要跑幾張TABLE
	int                        GetTableRunCount() const { return m_TableRunCount; }//

	bool                       SetCCDTrigEdge(int TrigEdge=CCD_TRIG_EDGE_L);	//CCD Trigger Edge
	bool                       EnableDLPChannel(int DLPMask);//新增加的DLP開啟功能

	bool                       GetTableRunCount(int &nTable);
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
	void                       SetDLPCallbackTimeus(int val) { m_DLPCallbackTimeus = val; }
	int                        GetDLPCallbackTimeus() const { return m_DLPCallbackTimeus; }
	//---------------------------------------------------------------------------------//
	void                       SetDLPPulseWidthTimeus(int val) { m_DLPPulseWidthTimeus = val; }
	int                        GetDLPPulseWidthTimeus() const { return m_DLPPulseWidthTimeus; }
	//---------------------------------------------------------------------------------//
	void                       SetTableMinBetweenTimeus(int val) { m_TableMinBetweenTimeus = val; }
	int                        GetTableMinBetweenTimeus() const { return m_TableMinBetweenTimeus; }
	//---------------------------------------------------------------------------------//
	void                       SetPreCheckTableListEnabled(int val) { m_PreCheckTableListEnabled = val; }
	int                        GetPreCheckTableListEnabled() const { return m_PreCheckTableListEnabled; }
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
	bool                       TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList);
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
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CLightCtrlBoard LightCtrlBoard;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHTCTRLBOARD_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
