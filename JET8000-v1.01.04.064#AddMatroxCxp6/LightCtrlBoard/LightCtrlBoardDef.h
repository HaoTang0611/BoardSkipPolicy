// LightCtrlBoard.h: interface for the CLightCtrlBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCTRLBOARDDEF_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
#define AFX_LIGHTCTRLBOARDDEF_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//#define LIGHT_CTRL_BOARD_USE_FPGA       1
#define LIGHT_CTRL_BOARD_USE_ARDUINO    2
//#define LIGHT_CTRL_BOARD_USE_IMP_CLS    3
//-------------------------------------------------------------------------------------//
//FPGA MODE
#define FPGA_MODE_UNDEFINED 0
#define FPGA_MODE_FPGA		1
#define FPGA_MODE_WRITE		2
#define FPGA_MODE_READ		3
#define FPGA_MODE_ASSIGN	4

#define TABLE_TYPE_NONE		0
#define TABLE_TYPE_LED		1
#define TABLE_TYPE_DLP		2

//#define MAX_TABLE_COUNT		54//For DLP Exposure x 1
//#define MAX_TABLE_COUNT		84//For DLP Exposure x 2
#define MAX_TABLE_COUNT		96//For DLP Exposure x 2 //6GC

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
//Light Ctrl Board Update List
//8DA1:初版
//8DA2:支援12個通道, 8個相機
//8DA3:支援超過64個表格
//8DA4:支援指定觸發時表格的起點
//-------------------------------------------------------------------------------------//
enum LIGHT_CTRL_BOARD_IMP_CLS//Light Ctrl Board Imp Class
{
	LIGHT_CTRL_BOARD_IMP_NULL       = 0,//未定義
	LIGHT_CTRL_BOARD_IMP_FPGA       = 1,//銀龍的控制器
	LIGHT_CTRL_BOARD_IMP_ARDUINO    = 2,//偉舜的控制器
	LIGHT_CTRL_BOARD_IMP_RETURN
};
//-------------------------------------------------------------------------------------//
enum LIGHT_CTRL_BOARD_TYPE
{
	LIGHT_CTRL_BOARD_NULL     =  0,

	LIGHT_CTRL_BOARD_3DA6     =  1,//FGPA-1
	LIGHT_CTRL_BOARD_8DA1     =  2,//FGPA-2

	LIGHT_CTRL_BOARD_ARDUINO  = 11,//ARDUINO-1

	LIGHT_CTRL_BOARD_RETURN
};
const LIGHT_CTRL_BOARD_TYPE LIGHT_CTRL_BOARD_DEFAULT=LIGHT_CTRL_BOARD_8DA1;
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

	UINT sDLPReadySignalEnable; //DLP是否啟用準備訊號
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

	TLCB_LED_ITEM sPWM13;			//(bit7~0) //for ARM
	TLCB_LED_ITEM sPWM14;			//(bit15~8)

	TLCB_LED_ITEM sPWM15;			//(bit7~0) //for ARM
	TLCB_LED_ITEM sPWM16;			//(bit15~8)

//(13-v2)
	UINT          sCameraEnable;    //(bit7~0) //for 8DA1

//(14-v2)
	UINT sDLPPulseTime;			//DLP Pulse Width的時間(us)
//(15-v2)(16-v2)
	UINT sDLPCallbackTime;		//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常
//(17-v2)
	UINT sDLPCCDExpTime;		//DLP的CCD觸光時間(us)

	UINT sDLPReadySignalDelayTime; //DLP準備訊號延遲時間

} TLCB_TRIG_TABLE, *PLCB_TRIG_TABLE;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHTCTRLBOARDDEF_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
