#ifndef _JET_USB_CTRL_V2_H_
#define _JET_USB_CTRL_V2_H_
//------------------------------------------------------------------------------//
#include "Setupapi.h"
#pragma comment(lib, "Setupapi.lib")
//------------------------------------------------------------------------------//
#include "..\\JET8000_Library\\CyUsb\\include\\CyAPI.h"
#include "..\\JET8000_Library\\CyUsb\\include\\cyioctl.h"
//------------------------------------------------------------------------------//
#define ADDRESS_W				0
#define ADDRESS_R				1

#define JET_USB_CTRL_ADDRESS_SIZE	    	2
#define JET_USB_CTRL_DATA_SIZE	        	4
#define JET_USB_CTRL_ADDRESS_BIT_SIZE		8
#define JET_USB_CTRL_DATA_BIT_SIZE	    	16
//------------------------------------------------------------------------------//
#define MAX_USB_RESET_COUNT           2
#define ENABLE_USB_DEVICE_RESET
//------------------------------------------------------------------------------//
#define JET_USB_TEXT_SIZE       64
//------------------------------------------------------------------------------//
class CJetUSBCtrl
{
public:
	//---------------------------------------------------------------------------//
	CJetUSBCtrl();
	virtual ~CJetUSBCtrl();
	//---------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------//
	LPCTSTR     GetErrorString();
	bool        ConnectUSB(char BoardName[], char BoardCode[]);
	void        DisConnectUSB();	
	bool        CheckUSBDevice();
	//---------------------------------------------------------------------------//
	bool        GetIsConnected();
	//---------------------------------------------------------------------------//	
	bool        USBRead(char Address[], char DataS[], const int status=0);
	bool        USBWrite(char Address[], char DataS[]);
	bool        TestReadGetID(int DeviceIndex, char strBoardID[]);	
	//---------------------------------------------------------------------------//
	//fang 1011212
	void        SetBoardID(int BoardID);
	int         GetBoardID();
	bool        CheckData(char data[], size_t len);
	//---------------------------------------------------------------------------//
	void        SetCmdTextReadData(char Cmd[]);//設定電腦讀取資料命令
	void        SetCmdTextWriteData(char Cmd[]);//設定電腦寫入資料命令
	void        SetCmdTextReadAddress(char Cmd[]);//設定電腦讀取位址命令
	void        SetCmdTextWriteAddress(char Cmd[]);//設定電腦寫入位址命令	
	//---------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------//	
	CString     m_USBDebugFile;
	BOOL        m_bOverLapped;
	DWORD       m_USBTimeOut;	
	OVERLAPPED *m_pWriteOvLap;
	OVERLAPPED *m_pReadOvLap;
	LARGE_INTEGER m_SystemFreq;//系統計數頻率
	//---------------------------------------------------------------------------//
	//讀寫命令
	char        m_CmdTextReadData[32];//電腦讀取資料命令
	char        m_CmdTextWriteData[32];//電腦寫入資料命令
	char        m_CmdTextReadAddress[32];//電腦讀取位址命令
	char        m_CmdTextWriteAddress[32];//電腦寫入位址命令
	//---------------------------------------------------------------------------//
	HANDLE      m_hDevice;
	int         m_DeviceIndex;
	int         m_BoardID;//fang 1011212

	CString     m_ErrorString;
	bool        m_IsConnected;
	bool        m_SaveLogMsg;

	void        PreInitialUSB();
	void        ReleaseUSBControl();
	void        ASCII_to_Int_Fn(char *Data,int *D_i,int Len_i);
	void        IntToHex(int Value, int Digits, char str[]);	
	bool        OpenDriver(int DeviceIndex);

	bool        PipeNumToAddress(int PipeNum, UCHAR &Address);

	bool        USB_READ_Fn(char* Bufer_cr,int len_i,int pipeNum);
	bool        USB_READ_Fn_Direct(char* Bufer_cr,int len_i,int pipeNum);
	bool        USB_READ_Fn_Transfer(char* Bufer_cr,int len_i,int pipeNum);

	bool        USB_WRITE_Fn(char *Bufer_cr,int len_i,int pipeNum);
	bool        USB_WRITE_Fn_Direct(char *Bufer_cr,int len_i,int pipeNum);
	bool        USB_WRITE_Fn_Transfer(char *Bufer_cr,int len_i,int pipeNum);

	bool        CheckInit();
	bool        CheckDeviceHandle(HANDLE hDevice);

	bool        WriteAddress(char Address[], const int AddressMode);
	bool        ReadData(char DataS[]);
	bool        WriteData(char DataS[]);
	bool        Delay(DWORD Delayus);

	bool        GetReadDadaCmd(char Cmd[]);
	bool        GetWriteDadaCmd(char Cmd[]);
	bool        GetReadAddressCmd(char Cmd[]);
	bool        GetWriteAddressCmd(char Cmd[]);

	bool        DoDeviceReset();
	//---------------------------------------------------------------------------//
	bool        ReleaseOverLapped();
	bool        CreateOverLapped();
	//---------------------------------------------------------------------------//
	bool        USB_ResetParentPort(HANDLE hDevice);
	bool        USB_AbortPipe(HANDLE hDevice, UCHAR Address);
	bool        USB_CyclePort(HANDLE hDevice);
	bool        USB_GetDeviceName(HANDLE hDevice, char Name[]);
	bool        USB_GetDriverVersion(HANDLE hDevice, ULONG &version);
	bool        USB_GetFriendlyName(HANDLE hDevice, char Name[]);
	bool        USB_GetNumberPoints(HANDLE hDevice, UCHAR &endPts);
	bool        USB_GetTransferSize(HANDLE hDevice, UCHAR Address, LONG &transferSz);
	bool        USB_GetUSBDIVersion(HANDLE hDevice, ULONG &version);
	bool        USB_IO_Control_Direct(HANDLE hDevice, UCHAR Address, char *Bufer_cr, int len_i, OVERLAPPED *ov);
	bool        USB_IO_Control_Transfer(HANDLE hDevice, UCHAR Address, char *Bufer_cr, int len_i, OVERLAPPED *ov);
	//---------------------------------------------------------------------------//
	void        SetSaveLogMsg(bool Save);
	bool        SaveUSBLogMessage(LPCTSTR Info);
	//---------------------------------------------------------------------------//
};
//------------------------------------------------------------------------------//
#endif