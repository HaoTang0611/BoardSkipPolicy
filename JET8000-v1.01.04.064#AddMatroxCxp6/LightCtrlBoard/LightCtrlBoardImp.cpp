// LightCtrlBoardFpga.cpp: implementation of the CLightCtrlBoardImp class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "LightCtrlBoardImp.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLightCtrlBoardImp::CLightCtrlBoardImp()
{	
#ifndef LIGHT_CTRL_DISABLE
	int   i=0;	
	m_usbBoardID = 0;
	m_FPGAMode = FPGA_MODE_UNDEFINED;	
	m_FullVersion = _T("");
	m_FPGAVersion = _T("");	
		
	m_CameraCount = 8;
	m_DLPCastCount = 8;		
	m_LEDChannelCount = 12;
	m_TableMaxCount = 0;
	m_TableRunCount = 0;	
	m_DLPTableStartIndex=-1;
	m_DLPMultiTableEnabled = true;	
	m_SupportMoreThan64Table = false;
	m_SupportModifyTriggerFirstIndex = false;
	m_TableRecheckEnabled = FN_DISABLE;//表格重複確認模式
	m_DLPCallbackTimeus = 65535;
	m_DLPPulseWidthTimeus = 20;//DLP脈波寬度
	m_TableMinBetweenTimeus = 50;//us
	m_PreCheckTableListEnabled = FN_DISABLE;
	m_LightCtrlBoardType = LIGHT_CTRL_BOARD_NULL;

	//DLP的編號映射
	const int DLPCount = DLP_CHANNEL_COUNT;
	for ( i=0; i<DLPCount; i++ )
	{	m_DLPChannelMap[i] = i;	}
#endif//LIGHT_CTRL_DISABLE
}
//-------------------------------------------------------------------------------------//
CLightCtrlBoardImp::~CLightCtrlBoardImp()
{
#ifndef LIGHT_CTRL_DISABLE	
#endif//LIGHT_CTRL_DISABLE		
}
//-------------------------------------------------------------------------------------//
LPCTSTR  CLightCtrlBoardImp::GetErrorString()
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SaveLightCtrlBoardProcess(LPCTSTR fnName)
{
	CString str;
	str.Format(_T("LightCtrlBoard::%s"), fnName);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReturnNotImplement(LPCTSTR fnName)//回傳未完成函式
{
	m_ErrorString.Format(_T("Error, the Func[%s] not Implement"), fnName);
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImp::SetTableMaxCount(int val)//表格數量上限
{
	m_TableMaxCount = val;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::UpdateSupportFuncByVersion()//依據版本號更新支援功能
{
	TCHAR str8DA1[]=_T("8DA1");
	TCHAR str8DA2[]=_T("8DA2");
	TCHAR str8DA3[]=_T("8DA3");
	TCHAR str8DA4[]=_T("8DA4");
	CString Version = m_FPGAVersion;
	bool bMoreThan64Table=false;
	bool bModifyTriggerFirstIndex = false;

	if ( Version.CompareNoCase(str8DA1) == 0 )
	{	
		bMoreThan64Table = false;
		bModifyTriggerFirstIndex = false;
	}
	else if ( Version.CompareNoCase(str8DA2) == 0 )
	{
		bMoreThan64Table = false;
		bModifyTriggerFirstIndex = false;
	}
	else if ( Version.CompareNoCase(str8DA3) == 0 )
	{
		bMoreThan64Table = true;
		bModifyTriggerFirstIndex = false;
	}
	else if ( Version.CompareNoCase(str8DA4) == 0 )
	{
		bMoreThan64Table = true;
		bModifyTriggerFirstIndex = true;
	}
	else //其餘更新的版本
	{
		bMoreThan64Table = true;
		bModifyTriggerFirstIndex = true;
	}
	SetTableMaxCount(80);
	SetSupportMoreThan64Table(bMoreThan64Table);
	SetSupportModifyTriggerFirstIndex(bModifyTriggerFirstIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLightCtrlBoardImp::GetLCBIniFileName() const//取得控制板Ini的檔案名稱
{
	CString FileName;
	CString ShortName;	
#ifdef TB_SYSTEM_ONLY_BOT
	ShortName = _T("LightCtrlBoard_Bot.ini");	
#else
	ShortName = _T("LightCtrlBoard.ini");	
#endif//TB_SYSTEM_ONLY_BOT
	FileName.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);	
	return FileName;
}
//-------------------------------------------------------------------------------------//
CString CLightCtrlBoardImp::GetLCBIniSectionName(LIGHT_CTRL_BOARD_TYPE Type) const//取得控制板Ini的Section名稱
{	
	CString Section;
	switch ( Type )
	{
	case LIGHT_CTRL_BOARD_3DA6:	Section = _T("3DA6"); break;
	case LIGHT_CTRL_BOARD_8DA1:	Section = _T("8DA1"); break;
	case LIGHT_CTRL_BOARD_ARDUINO:	Section = _T("ARDUINO"); break;		
	case LIGHT_CTRL_BOARD_NULL: Section = _T("Light Ctrl Board"); break;
	default: Section=_T(""); break;
	}		
	return Section;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::Connect()
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::Connect"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::Connect(const char *IPAddress, int IPPort)//連接到控制板-網址
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::Connect"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::Connect(int BoardID, char BoardName[], char BoardCode[])
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::Connect"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetIsConnected()
{	
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetIsConnected"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::CheckIsConnected()
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::CheckIsConnected"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::DisConnect()
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::DisConnect"));
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImp::SetUSBBoardID(int val)
{
	m_usbBoardID = val;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImp::GetUSBBoardID() const
{
	return m_usbBoardID;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImp::GetCameraCount() const//取得相機數量
{
	return m_CameraCount;	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImp::GetDLPCastCount() const//取得DLP投光數量
{
	return m_DLPCastCount;	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImp::GetLEDChannelCount() const
{
	return m_LEDChannelCount;	
}
//-------------------------------------------------------------------------------------//
LIGHT_CTRL_BOARD_TYPE CLightCtrlBoardImp::GetLightCtrlBoardType() const
{
	return m_LightCtrlBoardType;
}
//-------------------------------------------------------------------------------------//
CString CLightCtrlBoardImp::GetLightCtrlBoardTypeText() const//取得控制板樣式文字
{
	CString Text;
	switch ( m_LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_NULL: Text=_T("NULL"); break;
	case LIGHT_CTRL_BOARD_3DA6: Text=_T("3DA6"); break;
	case LIGHT_CTRL_BOARD_8DA1: Text=_T("8DA1"); break;
	case LIGHT_CTRL_BOARD_ARDUINO: Text=_T("Arduino"); break;
	default: 
		Text = _T("Undefined");
		break;
	}
	return Text;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::WriteLightCtrlBoardType(LIGHT_CTRL_BOARD_TYPE Type)//寫入控制板樣式
{
	CString KeyName, KeyString;
	CString FileName = GetLCBIniFileName();	
	CString Section = GetLCBIniSectionName(LIGHT_CTRL_BOARD_NULL);	
	KeyName.Format(_T("Light Ctrl Board Type")); KeyString.Format(_T("%d"), Type);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	return false; }
	m_LightCtrlBoardType = Type;

	KeyName.Format(_T("Light Ctrl Board Name")); KeyString=GetLCBIniSectionName(Type);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::UpdateLightCtrlBoardType()//更新控制板USB命令文字
{	
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();//取得控制板樣式	
	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6:
		m_CameraCount = 0;
		m_DLPCastCount = 8;		
		m_LEDChannelCount = 8;
		break;

	case LIGHT_CTRL_BOARD_8DA1:
		m_CameraCount = 8;
		m_DLPCastCount = 8;		
		m_LEDChannelCount = 12;
		break;

	case LIGHT_CTRL_BOARD_ARDUINO:
		m_CameraCount = 8;
		m_DLPCastCount = 8;		
		m_LEDChannelCount = 16;
		break;

	default://LIGHT_CTRL_BOARD_NULL
		m_CameraCount = 0;
		m_DLPCastCount = 0;		
		m_LEDChannelCount = 0;
		break;
	}

	if ( m_DLPCastCount > DLP_CAST_COUNT )
	{	m_DLPCastCount = DLP_CAST_COUNT; }
	if ( m_LEDChannelCount > LED_CHANNEL_COUNT )
	{	m_LEDChannelCount = LED_CHANNEL_COUNT; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::LoopBackTestAll()
{	
	return ReturnNotImplement(_T("CLightCtrlBoardImp::LoopBackTestAll"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::LoopBackTest(int data, char DataWS[], char DataRA[], char DataRB[])
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::LoopBackTest"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::LoopBackTestA(int data, char DataWS[], char DataR[])
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::LoopBackTestA"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::LoopBackTestB(int data, char DataWS[], char DataR[])
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::LoopBackTestB"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReadLightCtrlBorad(const std::string &Address, std::string &Data)
{	
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ReadLightCtrlBorad"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::WriteLightCtrlBorad(const std::string &Address, const std::string &Data)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::WriteLightCtrlBorad"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SaveLightCtrlBoardINI()//寫參數至檔案
{
	int     i=0;
	bool    IsOK = true;
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section;	
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	if ( LIGHT_CTRL_BOARD_NULL == LightCtrlBoardType ) { return true; }
#ifndef LIGHT_CTRL_BOARD_USE_IMP_CLS
	WriteLightCtrlBoardType(LightCtrlBoardType);
#endif//LIGHT_CTRL_BOARD_USE_IMP_CLS

	FileName = GetLCBIniFileName();
	Section = GetLCBIniSectionName(LightCtrlBoardType);

	//表格重複確認模式
	KeyName.Format(_T("Table Recheck")); KeyString.Format(_T("%d"), m_TableRecheckEnabled);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("DLP Enable Multi Table")); KeyString.Format(_T("%d"), m_DLPMultiTableEnabled);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("DLP Callback Time")); KeyString.Format(_T("%d"), m_DLPCallbackTimeus);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("DLP Pulse Width Time")); KeyString.Format(_T("%d"), m_DLPPulseWidthTimeus);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("Table Min Between Time")); KeyString.Format(_T("%d"), m_TableMinBetweenTimeus);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Table Pre-Check Enabled")); KeyString.Format(_T("%d"), m_PreCheckTableListEnabled);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	for ( i=0; i<DLP_CHANNEL_COUNT; i++ )
	{
		KeyName.Format(_T("DLP Channel %d"), i); KeyString.Format(_T("%d"), m_DLPChannelMap[i]);
		if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
		{	IsOK = false; }	
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::LoadLightCtrlBoardINI()//從檔案讀取參數
{
	int     i=0;
	int     nValue=0;	
	CString FileName;
	CString TempStr;
	CString KeyName;
	CString KeyString;
	CString Section;	
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];	
	LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	FileName = GetLCBIniFileName();
	Section = GetLCBIniSectionName(LIGHT_CTRL_BOARD_NULL);	

	KeyName.Format(_T("Light Ctrl Board Type")); KeyString.Format(_T("%d"), LightCtrlBoardType);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 
		switch ( nValue )
		{
		case LIGHT_CTRL_BOARD_NULL:
		case LIGHT_CTRL_BOARD_3DA6:
		case LIGHT_CTRL_BOARD_8DA1:
		case LIGHT_CTRL_BOARD_ARDUINO:
			LightCtrlBoardType = (LIGHT_CTRL_BOARD_TYPE)nValue;
			m_LightCtrlBoardType = LightCtrlBoardType;
			break;
		}
	#ifdef LIGHT_CTRL_BOARD_USE_FPGA
		if ( LIGHT_CTRL_BOARD_ARDUINO == LightCtrlBoardType )
		{
			LightCtrlBoardType = LIGHT_CTRL_BOARD_8DA1;
			m_LightCtrlBoardType = LightCtrlBoardType;
		}
	#endif//LIGHT_CTRL_BOARD_USE_FPGA

	#ifdef LIGHT_CTRL_BOARD_USE_ARDUINO
		LightCtrlBoardType = LIGHT_CTRL_BOARD_ARDUINO;
		m_LightCtrlBoardType = LightCtrlBoardType;
	#endif//LIGHT_CTRL_BOARD_USE_ARDUINO
	}	
	if ( LIGHT_CTRL_BOARD_NULL == LightCtrlBoardType )
	{	
		LightCtrlBoardType = LIGHT_CTRL_BOARD_DEFAULT;	
		m_LightCtrlBoardType = LightCtrlBoardType;
		WriteLightCtrlBoardType(LightCtrlBoardType);
	}

	Section = GetLCBIniSectionName(LightCtrlBoardType);	
	//表格重複確認模式
	KeyName.Format(_T("Table Recheck")); KeyString.Format(_T("%d"), m_TableRecheckEnabled);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 
		switch ( nValue )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			m_TableRecheckEnabled = nValue;
			break;
		}		
	}

	KeyName.Format(_T("DLP Enable Multi Table")); KeyString.Format(_T("%d"), m_DLPMultiTableEnabled);	
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 
		if ( 0 == nValue ) { m_DLPMultiTableEnabled = false; }
		else { m_DLPMultiTableEnabled = true; }
	}

	KeyName.Format(_T("DLP Callback Time")); KeyString.Format(_T("%d"),m_DLPCallbackTimeus);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPCallbackTimeus = ::_ttoi(ReturnString); }

	KeyName.Format(_T("DLP Pulse Width Time")); KeyString.Format(_T("%d"), m_DLPPulseWidthTimeus);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPPulseWidthTimeus = ::_ttoi(ReturnString); }

	KeyName.Format(_T("Table Min Between Time")); KeyString.Format(_T("%d"),m_TableMinBetweenTimeus);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_TableMinBetweenTimeus = ::_ttoi(ReturnString); }

	KeyName.Format(_T("Table Pre-Check Enabled")); KeyString.Format(_T("%d"), m_PreCheckTableListEnabled);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_PreCheckTableListEnabled = ::_ttoi(ReturnString); }

	for ( i=0; i<DLP_CHANNEL_COUNT; i++ )
	{
		KeyName.Format(_T("DLP Channel %d"), i); KeyString.Format(_T("%d"), m_DLPChannelMap[i]);
		if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
		{	m_DLPChannelMap[i] = ::_ttoi(ReturnString); }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReadRAMData(char data[])
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ReadRAMData"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReadRAMAddress(char data[])
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ReadRAMAddress"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::CheckDLPNotReadySignalFault(int DlpIdx) const//確認DLP沒有準備訊號
{	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::CheckLightCtrlBoardError"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReadErrorString(CString &Error)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ReadErrorString"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::DecodeErrorCodeText(int ErrorCode, CString &ErrorStr)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::DecodeErrorCodeText"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetTriggerCountText(CString &TrigCount)
{	
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetTriggerCountText"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReadFPGAVersion(CString &Version)
{	
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ReadFPGAVersion"));
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLightCtrlBoardImp::GetFPGAVersion() const
{
	return m_FPGAVersion;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLightCtrlBoardImp::GetFullVersion() const
{
	return m_FullVersion;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetIsWorkingNow(bool &Working)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetIsWorkingNow"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetIsFPGAOK(bool &IsOK)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetIsFPGAOK"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReadBoardAllCount()
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ReadBoardAllCount"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetPCtoFPGATrigCount(UINT &count)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetPCtoFPGATrigCount"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetFPGAtoCCDTrigCount(UINT &count)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetFPGAtoCCDTrigCount"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetFPGAtoCCDTrigCount(UINT CameraID, UINT &count)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetFPGAtoCCDTrigCount"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SetDLPEnable(UINT DLPChannel)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SetDLPEnable"));	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetFPGAtoDLPTrigCount(UINT DLPChannel, UINT &count)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetFPGAtoDLPTrigCount"));	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetDLPtoFPGATrigCount(UINT DLPChannel, UINT &count)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetDLPtoFPGATrigCount"));	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TriggerStart()//開始觸發
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TriggerStart"));	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ClearAllCount()//清除所有數量
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ClearAllCount"));	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ClearAll()//清除全部資料
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ClearAll"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SwitchToFPGA()
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SwitchToFPGA"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SwitchToRead()
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SwitchToRead"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SwitchToWrite()
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SwitchToWrite"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SwitchToAssign()
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SwitchToAssign"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SetMode_FPGA()//FPGA Run
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SetMode_FPGA"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SetMode_PCWrite()//寫入
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SetMode_PCWrite"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SetMode_PCRead()//讀取
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SetMode_PCRead"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SetMode_PCAssign()//指定位址
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SetMode_PCAssign"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::WriteTableRunCount(int nTable)//Trigger後要跑幾張TABLE
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::WriteTableRunCount"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReadTableRunCount(int &nTable)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ReadTableRunCount"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SetCCDTrigEdge(int TrigEdge)//CCD Trigger Edge
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SetCCDTrigEdge"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::EnableDLPChannel(int DLPMask)//新增加的DLP開啟功能
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::EnableDLPChannel"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetCCDTrigEdge(int &TrigEdge)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::GetCCDTrigEdge"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SetTriggerFirstIndex(UINT Index)//處發表格第1張引數
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::SetTriggerFirstIndex"));
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImp::GetTableMaxCount() const//表格數量上限
{
	return m_TableMaxCount;
}
//-------------------------------------------------------------------------------------//
int  CLightCtrlBoardImp::GetDLPTableStartIndex() const
{
	return m_DLPTableStartIndex;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImp::SetDLPTableStartIndex(int val)
{
	m_DLPTableStartIndex = val;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetDLPInteralTrigger() const
{
	bool InteralTrig = true;
#ifndef LIGHT_CTRL_DISABLE
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();
	switch ( LightCtrlBoardType )
	{
	case LIGHT_CTRL_BOARD_3DA6: InteralTrig = false; break;
	case LIGHT_CTRL_BOARD_8DA1: InteralTrig = false; break;
	case LIGHT_CTRL_BOARD_ARDUINO: InteralTrig = false; break; 
	default: InteralTrig = true; break;
	}	
#else
	InteralTrig = true;
#endif//LIGHT_CTRL_DISABLE
	return InteralTrig;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetDLPMultiTableEnabled() const
{ 
	return m_DLPMultiTableEnabled; 
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImp::SetDLPMultiTableEnabled(bool val)
{	
	m_DLPMultiTableEnabled = val; 
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetSupportMoreThan64Table() const
{
	return m_SupportMoreThan64Table;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImp::SetSupportMoreThan64Table(bool val)
{
	m_SupportMoreThan64Table = val;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetSupportModifyTriggerFirstIndex() const
{ 	
	return m_SupportModifyTriggerFirstIndex; 
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImp::SetSupportModifyTriggerFirstIndex(bool val)
{ 
	m_SupportModifyTriggerFirstIndex = val; 
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableRAMAssign(UINT AssignAddress)//指定RAM位址
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TableRAMAssign"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableAssign(UINT TableIndex)//指定RAM位址至某個Table的起始位址
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TableAssign"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableRAMAddressAdd()//RAM位址加1
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TableRAMAddressAdd"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableSingleWrite(TLCB_TRIG_TABLE &Table)//寫入一個Table
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TableSingleWrite"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableSingleRead(TLCB_TRIG_TABLE &Table)//讀出一個Table
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TableSingleRead"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableCompare(const TLCB_TRIG_TABLE &Table1, const TLCB_TRIG_TABLE &Table2)//比較2個Table
{
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	if ( Table1.sTableID != Table2.sTableID ) { return false; }

	if ( Table1.sTableType != Table2.sTableType ) { return false; }
	if ( Table1.sDLPTrigOutNumber != Table2.sDLPTrigOutNumber ) { return false; }
	if ( Table1.sDLPActiveChannel != Table2.sDLPActiveChannel ) { return false; }
	if ( Table1.sDLPPhasePatMode != Table2.sDLPPhasePatMode ) { return false; }

	if ( Table1.sNextTableTime != Table2.sNextTableTime ) { return false; }

	if ( Table1.sLEDTableTotalTime != Table2.sLEDTableTotalTime ) { return false; }


	if ( Table1.sCCDDelayTime != Table2.sCCDDelayTime ) { return false; }

	if ( Table1.sPWM1.sIsON != Table2.sPWM1.sIsON ) { return false; }
	if ( Table1.sPWM1.sPower != Table2.sPWM1.sPower ) { return false; }
	if ( Table1.sPWM2.sIsON != Table2.sPWM2.sIsON ) { return false; }
	if ( Table1.sPWM2.sPower != Table2.sPWM2.sPower ) { return false; }
	if ( Table1.sPWM3.sIsON != Table2.sPWM3.sIsON ) { return false; }
	if ( Table1.sPWM3.sPower != Table2.sPWM3.sPower ) { return false; }
	if ( Table1.sPWM4.sIsON != Table2.sPWM4.sIsON ) { return false; }
	if ( Table1.sPWM4.sPower != Table2.sPWM4.sPower ) { return false; }
	if ( Table1.sPWM5.sIsON != Table2.sPWM5.sIsON ) { return false; }
	if ( Table1.sPWM5.sPower != Table2.sPWM5.sPower ) { return false; }
	if ( Table1.sPWM6.sIsON != Table2.sPWM6.sIsON ) { return false; }
	if ( Table1.sPWM6.sPower != Table2.sPWM6.sPower ) { return false; }
	if ( Table1.sPWM7.sIsON != Table2.sPWM7.sIsON ) { return false; }
	if ( Table1.sPWM7.sPower != Table2.sPWM7.sPower ) { return false; }
	if ( Table1.sPWM8.sIsON != Table2.sPWM8.sIsON ) { return false; }
	if ( Table1.sPWM8.sPower != Table2.sPWM8.sPower ) { return false; }

	if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
	{
		if ( Table1.sPWM9.sIsON != Table2.sPWM9.sIsON ) { return false; }
		if ( Table1.sPWM9.sPower != Table2.sPWM9.sPower ) { return false; }
		if ( Table1.sPWM10.sIsON != Table2.sPWM10.sIsON ) { return false; }
		if ( Table1.sPWM10.sPower != Table2.sPWM10.sPower ) { return false; }
		if ( Table1.sPWM11.sIsON != Table2.sPWM11.sIsON ) { return false; }
		if ( Table1.sPWM11.sPower != Table2.sPWM11.sPower ) { return false; }
		if ( Table1.sPWM12.sIsON != Table2.sPWM12.sIsON ) { return false; }
		if ( Table1.sPWM12.sPower != Table2.sPWM12.sPower ) { return false; }

		if ( Table1.sCameraEnable != Table2.sCameraEnable ) { return false; }	
	}
	
	if ( LIGHT_CTRL_BOARD_ARDUINO == LightCtrlBoardType )
	{
		if ( Table1.sPWM9.sIsON != Table2.sPWM9.sIsON ) { return false; }
		if ( Table1.sPWM9.sPower != Table2.sPWM9.sPower ) { return false; }
		if ( Table1.sPWM10.sIsON != Table2.sPWM10.sIsON ) { return false; }
		if ( Table1.sPWM10.sPower != Table2.sPWM10.sPower ) { return false; }
		if ( Table1.sPWM11.sIsON != Table2.sPWM11.sIsON ) { return false; }
		if ( Table1.sPWM11.sPower != Table2.sPWM11.sPower ) { return false; }
		if ( Table1.sPWM12.sIsON != Table2.sPWM12.sIsON ) { return false; }
		if ( Table1.sPWM12.sPower != Table2.sPWM12.sPower ) { return false; }

		if ( Table1.sPWM13.sIsON != Table2.sPWM13.sIsON ) { return false; }
		if ( Table1.sPWM13.sPower != Table2.sPWM13.sPower ) { return false; }
		if ( Table1.sPWM14.sIsON != Table2.sPWM14.sIsON ) { return false; }
		if ( Table1.sPWM14.sPower != Table2.sPWM14.sPower ) { return false; }
		if ( Table1.sPWM15.sIsON != Table2.sPWM15.sIsON ) { return false; }
		if ( Table1.sPWM15.sPower != Table2.sPWM15.sPower ) { return false; }
		if ( Table1.sPWM16.sIsON != Table2.sPWM16.sIsON ) { return false; }
		if ( Table1.sPWM16.sPower != Table2.sPWM16.sPower ) { return false; }

		if ( Table1.sCameraEnable != Table2.sCameraEnable ) { return false; }	
	}

	if ( Table1.sDLPPulseTime != Table2.sDLPPulseTime ) { return false; }

	if ( Table1.sDLPCallbackTime != Table2.sDLPCallbackTime ) { return false; }
	if ( Table1.sDLPCCDExpTime != Table2.sDLPCCDExpTime ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableListRead(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TableListRead"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TableListWrite"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableListCompare(const std::vector<TLCB_TRIG_TABLE> &TableList)
{	
	const size_t TableCount1 = TableList.size();
	const size_t TableCount2 = m_LCBTableList.size();
	if ( TableCount1 != TableCount2 ) { return false; }

	CString         str;
	size_t          i=0;
	TLCB_TRIG_TABLE Table1;
	TLCB_TRIG_TABLE Table2;
	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;

	str.Format(_T("CLightCtrlBoardImp::TableListCompare[%d] Start"), TableCount1);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	JetAPI::SetFuncTimeStart(fnStart);

	for ( i=0; i<TableCount2; i++ )
	{
		Table1 = TableList[i];
		Table2 = m_LCBTableList[i];

		if ( TableCompare(Table1, Table2) == false )
		{	return false; }
	}

	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("CLightCtrlBoardImp::TableListCompare[%d] Time=%.3f ms"), TableCount1, fnTime);
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ReMapTableList_ReadToWrite(const std::vector<TLCB_TRIG_TABLE> &TableListRead, std::vector<TLCB_TRIG_TABLE> &TableListWrite)
{	//因為DLP的Channel是查表的所以要反查回去
	int       j=0;
	size_t    i=0;
	int       DLPChannel=0;
	TLCB_TRIG_TABLE Table;
	const size_t ReadTableCount = TableListRead.size();

	TableListWrite.clear();
	for ( i=0; i<ReadTableCount; i++ )
	{
		Table = (TableListRead[i]);
		if ( Table.sDLPActiveChannel > 0 ) 
		{ 
			DLPChannel = Table.sDLPActiveChannel;
			for ( j=1; j<DLP_CHANNEL_COUNT; j++ )
			{
				if ( DLPChannel == m_DLPChannelMap[j] ) 
				{	
					DLPChannel = j;
					break; 
				}
			}
			Table.sDLPActiveChannel = DLPChannel;
		}
		TableListWrite.push_back(Table);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::TableReset(int nTable)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::TableReset"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ClearLCBTableList()//清空內部的列表
{
	m_LCBTableList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImp::GetLCBTableCount() const
{
	return (int)(m_LCBTableList.size());
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::AddLCBTable(const TLCB_TRIG_TABLE &rTable)//加入內部表格
{
	m_LCBTableList.push_back(rTable);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::SetLCBTableList(const std::vector<TLCB_TRIG_TABLE> &rList)
{
	m_LCBTableList = rList;
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImp::GetCurFPGAMode() const
{
	return m_FPGAMode;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::DefaultTable(TLCB_TRIG_TABLE &Table)
{
	TLCB_LED_ITEM DefaultPWM;
	DefaultPWM.sIsON = false;
	DefaultPWM.sPower = 0;

	Table.sTableID = -1;
//(1)
	Table.sTableType = TABLE_TYPE_LED;	//TABLE_TYPE_LED、TABLE_TYPE_DLP
	Table.sDLPTrigOutNumber = 0;			//DLP觸發數量
	Table.sDLPActiveChannel = 0;			//8個DLP每個Table最多只能啟動一個DLP	(DLP_CHANNEL_1~8)
	Table.sDLPPhasePatMode = 0;

	Table.sDLPReadySignalEnable = 0;    //DLP是否啟用準備訊號

//(2)(3)
	Table.sNextTableTime = 0;				//Table間距時間, 第一個Table要設0

//(4)(5)
	Table.sLEDTableTotalTime=0;		//LED Table總時間 (CCD延遲時間+CCD曝光時間)

//(6)
	Table.sCCDDelayTime=0;				//燈亮至相機觸發的時間(us)	//DLP Type時，CCD觸發訊號的延遲時間(us)

//(7)
	Table.sPWM1=DefaultPWM;				//(bit7~0)	 bit7=>ON/OFF, 6~0=>Power(%)
	Table.sPWM2=DefaultPWM;				//(bit15~8)	 bit15=>ON/OFF, 14~8=>Power(%)
//(8)
	Table.sPWM3=DefaultPWM;				//(bit7~0)
	Table.sPWM4=DefaultPWM;				//(bit15~8)
//(9)
	Table.sPWM5=DefaultPWM;				//(bit7~0)
	Table.sPWM6=DefaultPWM;				//(bit15~8)
//(10)
	Table.sPWM7=DefaultPWM;				//(bit7~0)
	Table.sPWM8=DefaultPWM;				//(bit15~8)

//(11-v2)
	Table.sPWM9=DefaultPWM;				//(bit7~0)
	Table.sPWM10=DefaultPWM;			//(bit15~8)
//(12-v2)
	Table.sPWM11=DefaultPWM;			//(bit7~0)
	Table.sPWM12=DefaultPWM;			//(bit15~8)
//(13-v2)
	Table.sCameraEnable=0x01;			    //(bit7~0)

//(11-v1), (14-v2)
	Table.sDLPPulseTime=GetDLPPulseWidthTimeus();			//DLP Pulse Width的時間(us)-20
//(12-v1)(13-v1), (15-v1)(16-v1)
	Table.sDLPCallbackTime=65535;	//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常 - 20000
//(14-v1), (17-v1)
	Table.sDLPCCDExpTime=6000;		//DLP的CCD曝光時間(us)-6000

	Table.sDLPReadySignalDelayTime = 30000;//DLP準備訊號延遲時間//3000
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::GetTableInfoText(TLCB_TRIG_TABLE &Table, CString &Info)
{
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	Info = _T("");
	CString tmpStr = _T("");
	tmpStr.Format(_T("ID%d"), Table.sTableID);
	Info += tmpStr;
//Type
	int TableType = Table.sTableType;
	if( TableType == TABLE_TYPE_LED )		{ tmpStr = _T("LED"); }
	else if( TableType == TABLE_TYPE_DLP )	{ tmpStr = _T("DLP"); }
	else									{ tmpStr = _T("???"); return false; }
	Info += _T(" , ");
	Info += tmpStr;

//CCD Delay
	tmpStr.Format(_T("CCD Delay(%5d)"), Table.sCCDDelayTime);	
	Info += _T(" , ");
	Info += tmpStr;
//CCD Exp
	if( TableType == TABLE_TYPE_LED )
	{ tmpStr.Format(_T("Exp(%5d)"), Table.sLEDTableTotalTime-Table.sCCDDelayTime); }
	else if( TableType == TABLE_TYPE_DLP ) 
	{ tmpStr.Format(_T("Exp(%5d)"), Table.sDLPCCDExpTime); }
	Info += _T(" , ");
	Info += tmpStr;
//Next T
	tmpStr.Format(_T("NextT(%5d)"), Table.sNextTableTime);
	Info += _T(" , ");
	Info += tmpStr;

	tmpStr = _T("");
	if( TableType == TABLE_TYPE_LED )
	{
		Info += _T(" , PWM ");

		int nLED = 0;
		if( Table.sPWM1.sIsON == true ) { tmpStr.Format(_T(",01(%2d)"), Table.sPWM1.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM2.sIsON == true ) { tmpStr.Format(_T(",02(%2d)"), Table.sPWM2.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM3.sIsON == true ) { tmpStr.Format(_T(",03(%2d)"), Table.sPWM3.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM4.sIsON == true ) { tmpStr.Format(_T(",04(%2d)"), Table.sPWM4.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM5.sIsON == true ) { tmpStr.Format(_T(",05(%2d)"), Table.sPWM5.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM6.sIsON == true ) { tmpStr.Format(_T(",06(%2d)"), Table.sPWM6.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM7.sIsON == true ) { tmpStr.Format(_T(",07(%2d)"), Table.sPWM7.sPower); Info += tmpStr; nLED++; }
		if( Table.sPWM8.sIsON == true ) { tmpStr.Format(_T(",08(%2d)"), Table.sPWM8.sPower); Info += tmpStr; nLED++; }

		if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
		{
			if( Table.sPWM9.sIsON == true ) { tmpStr.Format(_T(",09(%2d)"), Table.sPWM9.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM10.sIsON == true ) { tmpStr.Format(_T(",10(%2d)"), Table.sPWM10.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM11.sIsON == true ) { tmpStr.Format(_T(",11(%2d)"), Table.sPWM11.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM12.sIsON == true ) { tmpStr.Format(_T(",12(%2d)"), Table.sPWM12.sPower); Info += tmpStr; nLED++; }
		}	

		if ( LIGHT_CTRL_BOARD_ARDUINO == LightCtrlBoardType )
		{			
			if( Table.sPWM9.sIsON == true ) { tmpStr.Format(_T(",09(%2d)"), Table.sPWM9.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM10.sIsON == true ) { tmpStr.Format(_T(",10(%2d)"), Table.sPWM10.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM11.sIsON == true ) { tmpStr.Format(_T(",11(%2d)"), Table.sPWM11.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM12.sIsON == true ) { tmpStr.Format(_T(",12(%2d)"), Table.sPWM12.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM13.sIsON == true ) { tmpStr.Format(_T(",13(%2d)"), Table.sPWM13.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM14.sIsON == true ) { tmpStr.Format(_T(",14(%2d)"), Table.sPWM14.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM15.sIsON == true ) { tmpStr.Format(_T(",15(%2d)"), Table.sPWM15.sPower); Info += tmpStr; nLED++; }
			if( Table.sPWM16.sIsON == true ) { tmpStr.Format(_T(",16(%2d)"), Table.sPWM16.sPower); Info += tmpStr; nLED++; }
		}	

		if( nLED == 0 )
		{ Info += _T("EMPTY"); }
	}
	else
	{
//DLP CH
		tmpStr.Format(_T("CH(%02d)"), Table.sDLPActiveChannel);
		Info += _T(" , ");
		Info += tmpStr;
//DLP nTrig.
		tmpStr.Format(_T("nTrig.(%d)"), Table.sDLPTrigOutNumber);
		Info += _T(" , ");
		Info += tmpStr;
//DLP Time
		tmpStr.Format(_T("DLPT(%5d)"), Table.sDLPPulseTime);
		Info += _T(" , ");
		Info += tmpStr;
//Over Time
		tmpStr.Format(_T("OverT(%5d)"), Table.sDLPCallbackTime);
		Info += _T(" , ");
		Info += tmpStr;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::CheckTableListEqually(const std::vector<TLCB_TRIG_TABLE> &TableListIn, const std::vector<TLCB_TRIG_TABLE> &TableListOut, std::vector<int> &NGTableList)//確認兩個表格是否相等
{
	int  i=0;
	CString ErrorStr;
	TLCB_TRIG_TABLE tmpTableIn;
	TLCB_TRIG_TABLE tmpTableOut;
	const int TableCountIn = (int)(TableListIn.size());
	const int TableCountOut = (int)(TableListOut.size());
	NGTableList.clear();
	//if ( TableCountIn != TableCountOut )
	//{
	//	m_ErrorString.Format(_T("Error, In/Out Table Count Exception (%d, %d)"), TableCountIn, TableCountOut);
	//	return false;
	//}
	const int TableCount = MIN(TableCountIn, TableCountOut);
	for ( i=0; i<TableCount; i++ )
	{
		tmpTableIn = TableListIn[i];
		tmpTableOut = TableListOut[i];
		if ( CheckTableEqually(i, tmpTableIn, tmpTableOut) == false )
		{
			if ( 0 == NGTableList.size() )
			{	ErrorStr = GetErrorString();	}
			NGTableList.push_back(i); 
		}			
	}	
	const size_t NGTableCount = NGTableList.size();
	if ( NGTableCount > 0 ) 
	{	
		m_ErrorString = ErrorStr;
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::CheckTableEqually(size_t idx, const TLCB_TRIG_TABLE &TableIn, const TLCB_TRIG_TABLE &TableOut)//確認兩個表格是否相等
{
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	idx = idx+1;
	if ( TableIn.sTableID != TableOut.sTableID ) 
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] ID Exception (%d, %d)"), idx, TableIn.sTableID, TableOut.sTableID);
		return false; 
	}
	if ( TableIn.sTableType != TableOut.sTableType )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] Type Exception (%d, %d)"), idx, TableIn.sTableType, TableOut.sTableType);
		return false; 
	}
	if ( TableIn.sDLPTrigOutNumber != TableOut.sDLPTrigOutNumber )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPTrigOutNumber Exception (%d, %d)"), idx, TableIn.sDLPTrigOutNumber, TableOut.sDLPTrigOutNumber);
		return false; 
	}
	
	int DLPChannelA = TableIn.sDLPActiveChannel;
	if ( DLPChannelA < DLP_CHANNEL_COUNT )
	{	DLPChannelA = m_DLPChannelMap[DLPChannelA];	}//因為寫入的DLP編號會跳號
	if ( DLPChannelA != TableOut.sDLPActiveChannel )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPActiveChannel Exception (%d, %d)"), idx, DLPChannelA, TableOut.sDLPActiveChannel);
		return false; 
	}
	//if ( TableIn.sDLPPhasePatMode != TableOut.sDLPPhasePatMode )//並非寫入CtrlBoard內
	//{ 
	//	m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPPhasePatMode Exception (%d, %d)"), idx, TableIn.sDLPPhasePatMode, TableOut.sDLPPhasePatMode);
	//	return false; 
	//}
	if ( TableIn.sNextTableTime != TableOut.sNextTableTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] NextTableTime Exception (%d, %d)"), idx, TableIn.sNextTableTime, TableOut.sNextTableTime);
		return false; 
	}
	if ( TableIn.sLEDTableTotalTime != TableOut.sLEDTableTotalTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] LEDTableTotalTime Exception (%d, %d)"), idx, TableIn.sLEDTableTotalTime, TableOut.sLEDTableTotalTime);
		return false; 
	}
	if ( TableIn.sCCDDelayTime != TableOut.sCCDDelayTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] CCDDelayTime Exception (%d, %d)"), idx, TableIn.sCCDDelayTime, TableOut.sCCDDelayTime);
		return false; 
	}

	if ( TableIn.sPWM1.sIsON != TableOut.sPWM1.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM1.sIsON Exception (%d, %d)"), idx, TableIn.sPWM1.sIsON, TableOut.sPWM1.sIsON);
		return false; 
	}
	if ( TableIn.sPWM1.sPower != TableOut.sPWM1.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM1.sPower Exception (%d, %d)"), idx, TableIn.sPWM1.sPower, TableOut.sPWM1.sPower);
		return false; 
	}
	
	if ( TableIn.sPWM2.sIsON != TableOut.sPWM2.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM2.sIsON Exception (%d, %d)"), idx, TableIn.sPWM2.sIsON, TableOut.sPWM2.sIsON);
		return false; 
	}
	if ( TableIn.sPWM2.sPower != TableOut.sPWM2.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM2.sPower Exception (%d, %d)"), idx, TableIn.sPWM2.sPower, TableOut.sPWM2.sPower);
		return false; 
	}

	if ( TableIn.sPWM3.sIsON != TableOut.sPWM3.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM3.sIsON Exception (%d, %d)"), idx, TableIn.sPWM3.sIsON, TableOut.sPWM3.sIsON);
		return false; 
	}
	if ( TableIn.sPWM3.sPower != TableOut.sPWM3.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM3.sPower Exception (%d, %d)"), idx, TableIn.sPWM3.sPower, TableOut.sPWM3.sPower);
		return false; 
	}

	if ( TableIn.sPWM4.sIsON != TableOut.sPWM4.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM4.sIsON Exception (%d, %d)"), idx, TableIn.sPWM4.sIsON, TableOut.sPWM4.sIsON);
		return false; 
	}
	if ( TableIn.sPWM4.sPower != TableOut.sPWM4.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM4.sPower Exception (%d, %d)"), idx, TableIn.sPWM4.sPower, TableOut.sPWM4.sPower);
		return false; 
	}

	if ( TableIn.sPWM5.sIsON != TableOut.sPWM5.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM5.sIsON Exception (%d, %d)"), idx, TableIn.sPWM5.sIsON, TableOut.sPWM5.sIsON);
		return false; 
	}
	if ( TableIn.sPWM5.sPower != TableOut.sPWM5.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM5.sPower Exception (%d, %d)"), idx, TableIn.sPWM5.sPower, TableOut.sPWM5.sPower);
		return false; 
	}

	if ( TableIn.sPWM6.sIsON != TableOut.sPWM6.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM6.sIsON Exception (%d, %d)"), idx, TableIn.sPWM6.sIsON, TableOut.sPWM6.sIsON);
		return false; 
	}
	if ( TableIn.sPWM6.sPower != TableOut.sPWM6.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM6.sPower Exception (%d, %d)"), idx, TableIn.sPWM6.sPower, TableOut.sPWM6.sPower);
		return false; 
	}

	if ( TableIn.sPWM7.sIsON != TableOut.sPWM7.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM7.sIsON Exception (%d, %d)"), idx, TableIn.sPWM7.sIsON, TableOut.sPWM7.sIsON);
		return false; 
	}
	if ( TableIn.sPWM7.sPower != TableOut.sPWM7.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM7.sPower Exception (%d, %d)"), idx, TableIn.sPWM7.sPower, TableOut.sPWM7.sPower);
		return false; 
	}

	if ( TableIn.sPWM8.sIsON != TableOut.sPWM8.sIsON )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM8.sIsON Exception (%d, %d)"), idx, TableIn.sPWM8.sIsON, TableOut.sPWM8.sIsON);
		return false; 
	}
	if ( TableIn.sPWM8.sPower != TableOut.sPWM8.sPower )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] PWM8.sPower Exception (%d, %d)"), idx, TableIn.sPWM8.sPower, TableOut.sPWM8.sPower);
		return false; 
	}

	if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
	{
		if ( TableIn.sPWM9.sIsON != TableOut.sPWM9.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM9.sIsON Exception (%d, %d)"), idx, TableIn.sPWM9.sIsON, TableOut.sPWM9.sIsON);
			return false; 
		}
		if ( TableIn.sPWM9.sPower != TableOut.sPWM9.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM9.sPower Exception (%d, %d)"), idx, TableIn.sPWM9.sPower, TableOut.sPWM9.sPower);
			return false; 
		}

		if ( TableIn.sPWM10.sIsON != TableOut.sPWM10.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM10.sIsON Exception (%d, %d)"), idx, TableIn.sPWM10.sIsON, TableOut.sPWM10.sIsON);
			return false; 
		}
		if ( TableIn.sPWM10.sPower != TableOut.sPWM10.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM10.sPower Exception (%d, %d)"), idx, TableIn.sPWM10.sPower, TableOut.sPWM10.sPower);
			return false; 
		}

		if ( TableIn.sPWM11.sIsON != TableOut.sPWM11.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM11.sIsON Exception (%d, %d)"), idx, TableIn.sPWM11.sIsON, TableOut.sPWM11.sIsON);
			return false; 
		}
		if ( TableIn.sPWM11.sPower != TableOut.sPWM11.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM11.sPower Exception (%d, %d)"), idx, TableIn.sPWM11.sPower, TableOut.sPWM11.sPower);
			return false; 
		}

		if ( TableIn.sPWM12.sIsON != TableOut.sPWM12.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM12.sIsON Exception (%d, %d)"), idx, TableIn.sPWM12.sIsON, TableOut.sPWM12.sIsON);
			return false; 
		}
		if ( TableIn.sPWM12.sPower != TableOut.sPWM12.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM12.sPower Exception (%d, %d)"), idx, TableIn.sPWM12.sPower, TableOut.sPWM12.sPower);
			return false; 
		}

		if ( TableIn.sCameraEnable != TableOut.sCameraEnable )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sCameraEnable Exception (%d, %d)"), idx, TableIn.sCameraEnable, TableOut.sCameraEnable);
			return false; 
		}
	}
	
	if ( LIGHT_CTRL_BOARD_ARDUINO == LightCtrlBoardType )
	{
		if ( TableIn.sPWM9.sIsON != TableOut.sPWM9.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM9.sIsON Exception (%d, %d)"), idx, TableIn.sPWM9.sIsON, TableOut.sPWM9.sIsON);
			return false; 
		}
		if ( TableIn.sPWM9.sPower != TableOut.sPWM9.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM9.sPower Exception (%d, %d)"), idx, TableIn.sPWM9.sPower, TableOut.sPWM9.sPower);
			return false; 
		}

		if ( TableIn.sPWM10.sIsON != TableOut.sPWM10.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM10.sIsON Exception (%d, %d)"), idx, TableIn.sPWM10.sIsON, TableOut.sPWM10.sIsON);
			return false; 
		}
		if ( TableIn.sPWM10.sPower != TableOut.sPWM10.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM10.sPower Exception (%d, %d)"), idx, TableIn.sPWM10.sPower, TableOut.sPWM10.sPower);
			return false; 
		}

		if ( TableIn.sPWM11.sIsON != TableOut.sPWM11.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM11.sIsON Exception (%d, %d)"), idx, TableIn.sPWM11.sIsON, TableOut.sPWM11.sIsON);
			return false; 
		}
		if ( TableIn.sPWM11.sPower != TableOut.sPWM11.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM11.sPower Exception (%d, %d)"), idx, TableIn.sPWM11.sPower, TableOut.sPWM11.sPower);
			return false; 
		}

		if ( TableIn.sPWM12.sIsON != TableOut.sPWM12.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM12.sIsON Exception (%d, %d)"), idx, TableIn.sPWM12.sIsON, TableOut.sPWM12.sIsON);
			return false; 
		}
		if ( TableIn.sPWM12.sPower != TableOut.sPWM12.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM12.sPower Exception (%d, %d)"), idx, TableIn.sPWM12.sPower, TableOut.sPWM12.sPower);
			return false; 
		}

		if ( TableIn.sPWM13.sIsON != TableOut.sPWM13.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM13.sIsON Exception (%d, %d)"), idx, TableIn.sPWM13.sIsON, TableOut.sPWM13.sIsON);
			return false; 
		}
		if ( TableIn.sPWM13.sPower != TableOut.sPWM13.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM13.sPower Exception (%d, %d)"), idx, TableIn.sPWM13.sPower, TableOut.sPWM13.sPower);
			return false; 
		}

		if ( TableIn.sPWM14.sIsON != TableOut.sPWM14.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM14.sIsON Exception (%d, %d)"), idx, TableIn.sPWM14.sIsON, TableOut.sPWM14.sIsON);
			return false; 
		}
		if ( TableIn.sPWM14.sPower != TableOut.sPWM14.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM14.sPower Exception (%d, %d)"), idx, TableIn.sPWM14.sPower, TableOut.sPWM14.sPower);
			return false; 
		}

		if ( TableIn.sPWM15.sIsON != TableOut.sPWM15.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM15.sIsON Exception (%d, %d)"), idx, TableIn.sPWM15.sIsON, TableOut.sPWM15.sIsON);
			return false; 
		}
		if ( TableIn.sPWM15.sPower != TableOut.sPWM15.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM15.sPower Exception (%d, %d)"), idx, TableIn.sPWM15.sPower, TableOut.sPWM15.sPower);
			return false; 
		}

		if ( TableIn.sPWM16.sIsON != TableOut.sPWM16.sIsON )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM16.sIsON Exception (%d, %d)"), idx, TableIn.sPWM16.sIsON, TableOut.sPWM16.sIsON);
			return false; 
		}
		if ( TableIn.sPWM16.sPower != TableOut.sPWM16.sPower )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sPWM16.sPower Exception (%d, %d)"), idx, TableIn.sPWM16.sPower, TableOut.sPWM16.sPower);
			return false; 
		}

		if ( TableIn.sCameraEnable != TableOut.sCameraEnable )
		{ 
			m_ErrorString.Format(_T("Error, In/Out Table[%d] sCameraEnable Exception (%d, %d)"), idx, TableIn.sCameraEnable, TableOut.sCameraEnable);
			return false; 
		}
	}

	if ( TableIn.sDLPPulseTime != TableOut.sDLPPulseTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPPulseTime Exception (%d, %d)"), idx, TableIn.sDLPPulseTime, TableOut.sDLPPulseTime);
		return false; 
	}
	if ( TableIn.sDLPCallbackTime != TableOut.sDLPCallbackTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPCallbackTime Exception (%d, %d)"), idx, TableIn.sDLPCallbackTime, TableOut.sDLPCallbackTime);
		return false; 
	}
	if ( TableIn.sDLPCCDExpTime != TableOut.sDLPCCDExpTime )
	{ 
		m_ErrorString.Format(_T("Error, In/Out Table[%d] DLPCCDExpTime Exception (%d, %d)"), idx, TableIn.sDLPCCDExpTime, TableOut.sDLPCCDExpTime);
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::WriteTableListToFile(LPCTSTR filename, std::vector<TLCB_TRIG_TABLE> &TableList, std::vector<int> &NGTableList)//將表格列表寫至檔案中
{
	bool   IsNGTable=false;
	unsigned int i=0, j=0;
	FILE *pfile = NULL;
	char  CameraEnable[16]="";
	TLCB_TRIG_TABLE tmpTable;	
	const unsigned int TableCount = (int)(TableList.size());
	const unsigned int NGTableCount = (int)(NGTableList.size());
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	pfile = ::_tfopen(filename, _T("w+"));
	if ( NULL == pfile ) 
	{
		m_ErrorString.Format(_T("Error, Open File to Write Fault(%s)"), filename);
		return false;
	}
	for ( i=0; i<TableCount; i++ )
	{
		IsNGTable=false;
		for ( j=0; j<NGTableCount; j++ )
		{
			if ( i == NGTableList[j] )
			{	
				IsNGTable=true;
				break;
			}
		}

		tmpTable = TableList[i];	
		if ( false == IsNGTable )
		{	::fprintf(pfile, "[Table %d]\n", i+1); }
		else
		{	::fprintf(pfile, "[Table %d -- NG]\n", i+1); }

		::fprintf(pfile, "TableID:%d\n", tmpTable.sTableID);
		::fprintf(pfile, "TableType:%d\n", tmpTable.sTableType);
		::fprintf(pfile, "DLPTrigOutNumber:%d\n", tmpTable.sDLPTrigOutNumber);
		::fprintf(pfile, "DLPActiveChannel:%d\n", tmpTable.sDLPActiveChannel);

		::fprintf(pfile, "NextTableTime:%d\n", tmpTable.sNextTableTime);
		::fprintf(pfile, "LEDTableTotalTime:%d\n", tmpTable.sLEDTableTotalTime);
		::fprintf(pfile, "CCDDelayTime:%d\n", tmpTable.sCCDDelayTime);

		::fprintf(pfile, "LED PWN1.IsON:%d\n", tmpTable.sPWM1.sIsON);
		::fprintf(pfile, "LED PWN1.Power:%d\n", tmpTable.sPWM1.sPower);
		::fprintf(pfile, "LED PWN2.IsON:%d\n", tmpTable.sPWM2.sIsON);
		::fprintf(pfile, "LED PWN2.Power:%d\n", tmpTable.sPWM2.sPower);
		::fprintf(pfile, "LED PWN3.IsON:%d\n", tmpTable.sPWM3.sIsON);
		::fprintf(pfile, "LED PWN3.Power:%d\n", tmpTable.sPWM3.sPower);
		::fprintf(pfile, "LED PWN4.IsON:%d\n", tmpTable.sPWM4.sIsON);
		::fprintf(pfile, "LED PWN4.Power:%d\n", tmpTable.sPWM4.sPower);
		::fprintf(pfile, "LED PWN5.IsON:%d\n", tmpTable.sPWM5.sIsON);
		::fprintf(pfile, "LED PWN5.Power:%d\n", tmpTable.sPWM5.sPower);
		::fprintf(pfile, "LED PWN6.IsON:%d\n", tmpTable.sPWM6.sIsON);
		::fprintf(pfile, "LED PWN6.Power:%d\n", tmpTable.sPWM6.sPower);
		::fprintf(pfile, "LED PWN7.IsON:%d\n", tmpTable.sPWM7.sIsON);
		::fprintf(pfile, "LED PWN7.Power:%d\n", tmpTable.sPWM7.sPower);
		::fprintf(pfile, "LED PWN8.IsON:%d\n", tmpTable.sPWM8.sIsON);
		::fprintf(pfile, "LED PWN8.Power:%d\n", tmpTable.sPWM8.sPower);

		if ( LIGHT_CTRL_BOARD_8DA1 == LightCtrlBoardType )
		{
			::fprintf(pfile, "LED sPWM9.IsON:%d\n", tmpTable.sPWM9.sIsON);
			::fprintf(pfile, "LED sPWM9.Power:%d\n", tmpTable.sPWM9.sPower);
			::fprintf(pfile, "LED sPWM10.IsON:%d\n", tmpTable.sPWM10.sIsON);
			::fprintf(pfile, "LED sPWM10.Power:%d\n", tmpTable.sPWM10.sPower);
			::fprintf(pfile, "LED sPWM11.IsON:%d\n", tmpTable.sPWM11.sIsON);
			::fprintf(pfile, "LED sPWM11.Power:%d\n", tmpTable.sPWM11.sPower);
			::fprintf(pfile, "LED sPWM12.IsON:%d\n", tmpTable.sPWM12.sIsON);
			::fprintf(pfile, "LED sPWM12.Power:%d\n", tmpTable.sPWM12.sPower);

			JetAPI::IntToBin(tmpTable.sCameraEnable, 8, CameraEnable);
			::fprintf(pfile, "LED Camera Enable:%s\n", CameraEnable);
		}	

		if ( LIGHT_CTRL_BOARD_ARDUINO == LightCtrlBoardType )
		{
			::fprintf(pfile, "LED sPWM9.IsON:%d\n", tmpTable.sPWM9.sIsON);
			::fprintf(pfile, "LED sPWM9.Power:%d\n", tmpTable.sPWM9.sPower);
			::fprintf(pfile, "LED sPWM10.IsON:%d\n", tmpTable.sPWM10.sIsON);
			::fprintf(pfile, "LED sPWM10.Power:%d\n", tmpTable.sPWM10.sPower);
			::fprintf(pfile, "LED sPWM11.IsON:%d\n", tmpTable.sPWM11.sIsON);
			::fprintf(pfile, "LED sPWM11.Power:%d\n", tmpTable.sPWM11.sPower);
			::fprintf(pfile, "LED sPWM12.IsON:%d\n", tmpTable.sPWM12.sIsON);
			::fprintf(pfile, "LED sPWM12.Power:%d\n", tmpTable.sPWM12.sPower);

			::fprintf(pfile, "LED sPWM13.IsON:%d\n", tmpTable.sPWM13.sIsON);
			::fprintf(pfile, "LED sPWM13.Power:%d\n", tmpTable.sPWM13.sPower);
			::fprintf(pfile, "LED sPWM14.IsON:%d\n", tmpTable.sPWM14.sIsON);
			::fprintf(pfile, "LED sPWM14.Power:%d\n", tmpTable.sPWM14.sPower);
			::fprintf(pfile, "LED sPWM15.IsON:%d\n", tmpTable.sPWM15.sIsON);
			::fprintf(pfile, "LED sPWM15.Power:%d\n", tmpTable.sPWM15.sPower);
			::fprintf(pfile, "LED sPWM16.IsON:%d\n", tmpTable.sPWM16.sIsON);
			::fprintf(pfile, "LED sPWM16.Power:%d\n", tmpTable.sPWM16.sPower);

			JetAPI::IntToBin(tmpTable.sCameraEnable, 8, CameraEnable);
			::fprintf(pfile, "LED Camera Enable:%s\n", CameraEnable);
		}	

		::fprintf(pfile, "DLPPulseTime:%d\n", tmpTable.sDLPPulseTime);
		::fprintf(pfile, "DLPCallbackTime:%d\n", tmpTable.sDLPCallbackTime);
		::fprintf(pfile, "DLPCCDExpTime:%d\n", tmpTable.sDLPCCDExpTime);		
	}
	::fclose(pfile); pfile=NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ExecConveyorMotorStop(LANE_ID LaneID)//執行軌道停止
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ExecConveyorMotorStop"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImp::ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow)//執行軌道運轉	
{
	return ReturnNotImplement(_T("CLightCtrlBoardImp::ExecConveyorMotorRunning"));
}
//-------------------------------------------------------------------------------------//