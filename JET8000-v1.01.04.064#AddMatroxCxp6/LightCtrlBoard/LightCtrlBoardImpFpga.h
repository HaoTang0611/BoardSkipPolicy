// LightCtrlBoardFpga.h: interface for the CLightCtrlBoardImpFpga class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCTRLBOARDFPGA_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
#define AFX_LIGHTCTRLBOARDFPGA_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "JetUSBCtrl.h"
#include "LightCtrlBoardImp.h"
//-------------------------------------------------------------------------------------//
//燈源控制板
class CLightCtrlBoardImpFpga : public CLightCtrlBoardImp 
{
private:
	//---------------------------------------------------------------------------------//
	CJetUSBCtrl                m_USB;
	CRITICAL_SECTION           m_csUSB;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoardImpFpga(const CLightCtrlBoardImpFpga &CtrlBoard);
	CLightCtrlBoardImpFpga& operator=(const CLightCtrlBoardImpFpga &CtrlBoard);
	//---------------------------------------------------------------------------------//	
	virtual bool               UpdateSupportFuncByVersion();//依據版本號更新支援功能	
	//---------------------------------------------------------------------------------//
	bool                       ExecUSBRead(char Address[], char Data[]);
	bool                       ExecUSBWrite(char Address[], char Data[]);		
	//---------------------------------------------------------------------------------//
	bool                       ReadErrorCode(int &Error);	
	bool                       ReadDLPErrCH(CString &ErrDlp);
	bool                       ReadDLPTrigStepInfo(CString &DLPTrigInfo);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLightCtrlBoardImpFpga();
	virtual ~CLightCtrlBoardImpFpga();
	//---------------------------------------------------------------------------------//	
	bool                       GetUseUSBCtrl() const { return true; }
	bool                       GetUseSocketCtrl() const { return false; }
	LIGHT_CTRL_BOARD_IMP_CLS   GetLightCtrlBoardClass() const { return LIGHT_CTRL_BOARD_IMP_FPGA; }
	//---------------------------------------------------------------------------------//	
	bool                       Connect();	
	bool                       Connect(int BoardID, char BoardName[], char BoardCode[]);
	bool                       GetIsConnected();	
	bool                       CheckIsConnected();
	bool                       DisConnect();	
	//---------------------------------------------------------------------------------//	
	void                       SetUSBBoardID(int val);
	int                        GetUSBBoardID() const;	
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
	//---------------------------------------------------------------------------------//		
	bool                       DecodeErrorCodeText(int ErrorCode, CString &ErrorStr);
	bool                       GetTriggerCountText(CString &TrigCount);		
	//---------------------------------------------------------------------------------//
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
//--USER
	int                        GetCurFPGAMode() const;	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHTCTRLBOARDFPGA_H__87B3AF9C_6DE4_4809_841C_6894B966D536__INCLUDED_)
