// Barcode_Device_Module.cpp: implementation of the CBarcode_Device_Module class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Barcode_Device_Module.h"
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
CBarcode_Device_Module::CBarcode_Device_Module()
{
	PreInitBarcodeDevice_Module();
	InitialBarcodeDevice_Module();
}
//-------------------------------------------------------------------------------------//
CBarcode_Device_Module::CBarcode_Device_Module(BARCODE_DEVICE_TYPE DeviceType)
{
	SetBarcodeDeviceType(DeviceType);	
	PreInitBarcodeDevice_Module();
	InitialBarcodeDevice_Module();	
}
//-------------------------------------------------------------------------------------//
CBarcode_Device_Module::CBarcode_Device_Module(const CBarcode_Device_Module &device):CBarcode_Basic(device)
{
	PreInitBarcodeDevice_Module();
	CloneBarcodeDevice_Module(device);
}
//-------------------------------------------------------------------------------------//
CBarcode_Device_Module::~CBarcode_Device_Module()
{
	Disconnected();
}
//-------------------------------------------------------------------------------------//
CBarcode_Device_Module& CBarcode_Device_Module::operator=(const CBarcode_Device_Module &device)
{
	if ( this == &device ) { return *this; }
	CBarcode_Basic::operator=(device);
	CloneBarcodeDevice_Module(device);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Device_Module::PreInitBarcodeDevice_Module()
{
	BARCODE_DEVICE_TYPE  DeviceType = GetBarcodeDeviceType();
	CString ModelName = AOIDataDefine.GetBarcodeDeviceName(DeviceType);
	SetBarcodeDeviceType(DeviceType);//條碼機樣式
	SetBarcodeDeviceModel(ModelName);//條碼機型號
	/*
	::memset(m_SeparatorString, 0x00, sizeof(m_SeparatorString));
	::memset(m_HeaderString, 0x00, sizeof(m_HeaderString));
	::memset(m_TerminatorString, 0x00, sizeof(m_TerminatorString));
	::memset(m_TriggerOnString, 0x00, sizeof(m_TriggerOnString));
	::memset(m_TriggerOffString, 0x00, sizeof(m_TriggerOffString));
	::memset(m_DeviceNoRead, 0x00, sizeof(m_DeviceNoRead));
	*/
}
//-------------------------------------------------------------------------------------//
void CBarcode_Device_Module::InitialBarcodeDevice_Module()
{
	m_WaitTimeoutMs = 30000;//愈時時間ms	
	m_BarcodeDeviceModuleID = -1;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Device_Module::CloneBarcodeDevice_Module(const CBarcode_Device_Module &device)
{	
	m_WaitTimeoutMs = device.m_WaitTimeoutMs;
	m_BarcodeDeviceModuleID = device.m_BarcodeDeviceModuleID;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Device_Module::SetBarcodeDeviceModuleID(int val)//條碼機模組編號
{
	m_BarcodeDeviceModuleID = val;
}
//-------------------------------------------------------------------------------------//
int CBarcode_Device_Module::GetBarcodeDeviceModuleID() const//條碼機模組編號
{
	return m_BarcodeDeviceModuleID;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::CheckBarcodeDeviceModuleDefine()//確認條碼裝置模組定義
{	
#ifndef BARCODE_DEVICE_MODULE
	m_ErrorString = _T("Error, No Define Barcode Device Module");
	return false;
#endif//BARCODE_DEVICE_MODULE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::LoadBarcodeINIFile()
{
	if ( CBarcode_Basic::LoadBarcodeINIFile() == false ) { return false; }

	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen]=_T("");		
	CString strDeviceName = GetBarcodeDeviceFullName();
	
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("JET8000.INI"));
	Section.Format(_T("%s-Module"), strDeviceName);	
	
	//逾時時間
	KeyName.Format(_T("Wait Timeout ms"));
	Default.Format(_T("%d"), m_WaitTimeoutMs);
	if ( JetAPI::LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	m_WaitTimeoutMs = ::_ttoi(String); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::SaveBarcodeINIFile()
{
	if ( CBarcode_Basic::SaveBarcodeINIFile() == false ) { return false; }

	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString String = _T("");
	CString strDeviceName = GetBarcodeDeviceFullName();
	
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("JET8000.INI"));
	Section.Format(_T("%s-Module"), strDeviceName);	
	
	//逾時時間
	KeyName.Format(_T("Wait Timeout ms"));
	String.Format(_T("%d"), m_WaitTimeoutMs);
	if ( JetAPI::SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CBarcode_Device_Module::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	return CBarcode_Basic::LoadMultiLanguageString(KeyName, Default);
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::CheckConnected()//確認是否連線
{
	return this->GetBarcodeDeviceConnected();	
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::ConnectToBarcodeDevice(bool Reset)//連線至裝置
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return false; }
#ifdef BARCODE_DEVICE_MODULE		
	const auto Folder = AOIDataCollect.GetAOIDirectoryA();
	CString strPort = GetBarcodeDevicePort();
	CBarcodeParameter   BarcodeParam;

	//載入參數檔案
	LoadBarcodeINIFile();
	ClearResultBuffer();	
	ClearResultBackup();

	const int BarcodeID = GetBarcodeDeviceID();
	BARCODE_DEVICE_TYPE DeviceType = GetBarcodeDeviceType();	
	BarcodeParam.m_COMPort = ::_ttoi(strPort);
	BarcodeParam.m_BarcodeID = 0;
	BarcodeParam.m_BarcodeType = GetBarcodeDeviceType();
	BarcodeParam.m_WaitTimeoutMs = m_WaitTimeoutMs;	
	BarcodeParam.m_InitializeSW = Reset;

	switch ( BarcodeID )
	{
	case BARCODE_DEVICE_ID_01:	BarcodeParam.m_BarcodeID = 0;	break;
	case BARCODE_DEVICE_ID_02:	BarcodeParam.m_BarcodeID = 1;	break;
	case BARCODE_DEVICE_ID_03:	BarcodeParam.m_BarcodeID = 2;	break;
	case BARCODE_DEVICE_ID_04:	BarcodeParam.m_BarcodeID = 3;	break;
	case BARCODE_DEVICE_ID_05:	BarcodeParam.m_BarcodeID = 4;	break;
	case BARCODE_DEVICE_ID_06:	BarcodeParam.m_BarcodeID = 5;	break;
	case BARCODE_DEVICE_ID_07:	BarcodeParam.m_BarcodeID = 6;	break;
	case BARCODE_DEVICE_ID_08:	BarcodeParam.m_BarcodeID = 7;	break;
	default:	
		BarcodeParam.m_BarcodeID = -1;
		return false;		
	}
	if ( -1 == BarcodeParam.m_BarcodeID )
	{
		m_ErrorString.Format(_T("Error, Barcode ID Exception (%d)"), BarcodeID);
		return false;
	}

	switch ( DeviceType )
	{
	case BARCODE_DEVICE_MICROSCAN_MS3:	BarcodeParam.m_BarcodeType = BARCODE_MICROSCAN_MS3;	break;
	case BARCODE_DEVICE_MICROSCAN_MINI:	BarcodeParam.m_BarcodeType = BARCODE_MICROSCAN_MINI;	break;
	case BARCODE_DEVICE_MICROSCAN_MINI_VELOCITY:	BarcodeParam.m_BarcodeType = BARCODE_MICROSCAN_MINI_VELOCITY;	break;
	case BARCODE_DEVICE_MICROSCAN_MINI3:	BarcodeParam.m_BarcodeType = BARCODE_MICROSCAN_MINI3;	break;
	case BARCODE_DEVICE_MICROSCAN_MINI_HAWK:	BarcodeParam.m_BarcodeType = BARCODE_MICROSCAN_MINI_HAWK;	break;

	case BARCODE_DEVICE_DATALOGIC_M1000:	BarcodeParam.m_BarcodeType = BARCODE_DATALOGIC_M1000;	break;
	case BARCODE_DEVICE_DATALOGIC_MATRIX_200:	BarcodeParam.m_BarcodeType = BARCODE_DATALOGIC_MATRIX_200;	break;
	case BARCODE_DEVICE_DATALOGIC_MATRIX_210:	BarcodeParam.m_BarcodeType = BARCODE_DATALOGIC_MATRIX_210;	break;
	case BARCODE_DEVICE_DATALOGIC_GFS4400:	BarcodeParam.m_BarcodeType = BARCODE_DATALOGIC_GFS4400;	break;
	
	case BARCODE_DEVICE_HONEYWELL_3310GHD:	BarcodeParam.m_BarcodeType = BARCODE_HONEYWELL_3310GHD;	break;
	case BARCODE_DEVICE_HONEYWELL_1900:	BarcodeParam.m_BarcodeType = BARCODE_HONEYWELL_1900;	break;	

	case BARCODE_DEVICE_SICK_442:	BarcodeParam.m_BarcodeType = BARCODE_SICK_442;	break;	
	case BARCODE_DEVICE_SICK_ICR840:	BarcodeParam.m_BarcodeType = BARCODE_SICK_ICR840;	break;	
	default:
		BarcodeParam.m_BarcodeType = 0;
		break;
	}
	if ( 0 == BarcodeParam.m_BarcodeType )
	{
		m_ErrorString.Format(_T("Error, Barcode Type Exception (%d)"), DeviceType);
		return false;
	}

	BarcodeParam.m_iniPath = std::string(Folder) + std::string("\\BarcodeDeviceModule.INI");

	const int ResBarcodeID = theBarcodeUnitCtrl().CreateBarcodeObject(BarcodeParam);
	if ( ResBarcodeID < 0 ) 
	{
		SetBarcodeDeviceConnected(false);	
		const std::string *strMsg = theBarcodeUnitCtrl().GetErrorString(BarcodeParam.m_BarcodeID);
		m_ErrorString = strMsg->c_str();
		return false;
	}	
	SetBarcodeDeviceModuleID(BarcodeParam.m_BarcodeID);
	SetBarcodeDeviceConnected(true);	
	SaveBarcodeINIFile();
	return true;
#endif//BARCODE_DEVICE_MODULE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::ConnectToDevice()//連線至裝置
{
	bool IsOK = true;
	IsOK = ConnectToBarcodeDevice(false);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::Disconnected()//斷線
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return false; }
#ifdef BARCODE_DEVICE_MODULE
	if ( GetBarcodeDeviceConnected() == false ) { return true; }
	const int nBarcodeID = GetBarcodeDeviceModuleID();
	SetBarcodeDeviceModuleID(-1);
	SetBarcodeDeviceConnected(false);		
	if ( theBarcodeUnitCtrl().DelectBarcodeObject(nBarcodeID) == false ) 
	{
		const std::string *strMsg = theBarcodeUnitCtrl().GetErrorString(nBarcodeID);
		m_ErrorString = strMsg->c_str();
		return false;
	}	
	return true;
#endif//BARCODE_DEVICE_MODULE	
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CBarcode_Device_Module::StartToReadBarcodeDevice()
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return false; }
#ifdef BARCODE_DEVICE_MODULE
	const int nBarcodeID = GetBarcodeDeviceModuleID();	
	CBarcode_Basic::ClearResultBuffer();
	if ( theBarcodeUnitCtrl().ClearBarcodeData(nBarcodeID) == false )
	{
		const std::string *strMsg = theBarcodeUnitCtrl().GetErrorString(nBarcodeID);
		m_ErrorString = strMsg->c_str();
		return false;
	}
	if ( theBarcodeUnitCtrl().TriggerBarcode(nBarcodeID) == false )
	{
		const std::string *strMsg = theBarcodeUnitCtrl().GetErrorString(nBarcodeID);
		m_ErrorString = strMsg->c_str();
		return false;
	}
#endif//BARCODE_DEVICE_MODULE
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CBarcode_Device_Module::EndReadingBarcodeDevice()
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return false; }
#ifdef BARCODE_DEVICE_MODULE
	const int nBarcodeID = GetBarcodeDeviceModuleID();
	if ( theBarcodeUnitCtrl().TurnOffBarcode(nBarcodeID) == false )
	{
		const std::string *strMsg = theBarcodeUnitCtrl().GetErrorString(nBarcodeID);
		m_ErrorString = strMsg->c_str();
		return false;
	}	
#endif//BARCODE_DEVICE_MODULE
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CBarcode_Device_Module::RetrieveCodeBarcodeDevice()
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return false; }
#ifdef BARCODE_DEVICE_MODULE
	std::vector<std::string> rBarcodeList;
	const int nBarcodeID = GetBarcodeDeviceModuleID();
	//bool WaitBarcodeReading(int BarcodeId);//等待條碼機讀值
	ClearResultBackup();
	if ( theBarcodeUnitCtrl().GetBarcodeData(nBarcodeID, rBarcodeList) == false )
	{
		const std::string *strMsg = theBarcodeUnitCtrl().GetErrorString(nBarcodeID);
		m_ErrorString = strMsg->c_str();
		return false;
	}

	size_t       i=0;
	size_t       CodeLen=0;
	size_t       BufferLen=0;
	std::string  SingleCode;
	const size_t BufferSize=1024;
	char         Buffer[BufferSize]="";
	char         Buffer2[BufferSize]="";
	const size_t StrCnt = rBarcodeList.size();
	if ( 0 == StrCnt )
	{
		m_ErrorString = _T("Error, Barcode No Read");
		return false;
	}

	BufferLen=0;
	for ( i=0; i<StrCnt; i++ )
	{
		SingleCode = rBarcodeList[i];
		CodeLen = SingleCode.length();
		if ( BufferSize < (BufferLen+CodeLen+1) )
		{	
			m_ErrorString.Format(_T("Error, Barocode Code Number out of setting [%d/%d]"), (BufferLen+CodeLen+1), BufferSize);
			return false;	
		}
		if ( strlen(Buffer) == 0 ) 
		{	::strcpy(Buffer, SingleCode.c_str());	}
		else
		{
			::sprintf(Buffer2, "%s+%s", Buffer, SingleCode.c_str());	
			::strcpy(Buffer, Buffer2);
		}		
		BufferLen += (CodeLen+1);
	}
	SetResultBuffer(Buffer);	
	AnalysisResultBuffer();
	BackupResultBuffer();
	m_ErrorString = _T("");
#endif//BARCODE_DEVICE_MODULE
	return true;
}
//-------------------------------------------------------------------------------------//	
void CBarcode_Device_Module::ClearBuffer()
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return ; }
#ifdef BARCODE_DEVICE_MODULE	
	const int nBarcodeID = GetBarcodeDeviceModuleID();
	if ( theBarcodeUnitCtrl().ClearBarcodeData(nBarcodeID) == false )
	{
		const std::string *strMsg = theBarcodeUnitCtrl().GetErrorString(nBarcodeID);
		m_ErrorString = strMsg->c_str();
		return ;
	}	
#endif//BARCODE_DEVICE_MODULE
	return ;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::Initialize()
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return false; }
#ifdef BARCODE_DEVICE_MODULE
	Disconnected();
	if ( ConnectToBarcodeDevice(true) == false ) 
	{	return false; }
#endif//BARCODE_DEVICE_MODULE
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CBarcode_Device_Module::StartToRead()
{
	if ( StartToReadBarcodeDevice() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::EndReading()
{
	if ( EndReadingBarcodeDevice() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::RetrieveCode()
{
	if ( RetrieveCodeBarcodeDevice() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CBarcode_Device_Module::AnalysisResultBuffer()//分析結果字串	
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return false; }
#ifdef BARCODE_DEVICE_MODULE
	std::vector<std::string> rBarcodeList;
	const int nBarcodeID = GetBarcodeDeviceModuleID();
	if ( theBarcodeUnitCtrl().GetBarcodeData(nBarcodeID, rBarcodeList) == false )
	{
		const std::string *strMsg = theBarcodeUnitCtrl().GetErrorString(nBarcodeID);
		m_ErrorString = strMsg->c_str();
		return false;
	}

	size_t       i=0;
	int          SubCount=0;
	size_t       CodeLen=0;	
	std::string  SingleCode;
	const size_t SubBufferSize=BARCODE_SIZE;	
	const size_t StrCnt = rBarcodeList.size();
	
	SubCount=0;
	for ( i=0; i<StrCnt; i++ )
	{
		SingleCode = rBarcodeList[i];
		CodeLen = SingleCode.length();
		if ( CodeLen > SubBufferSize ) 
		{
			m_ErrorString.Format(_T("Error, Barocode Sub-Code Number out of setting [%d/%d]"), (CodeLen), SubBufferSize);
			return false;	
		}
		SetResultSubBuffer(SubCount, SingleCode.c_str());		
		SubCount ++;
	}

#endif//BARCODE_DEVICE_MODULE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Device_Module::CloneResultBuffer(char *Buffer, size_t BufferSize)//複製結果暫存區
{
	if ( CheckBarcodeDeviceModuleDefine() == false ) { return false; }

	size_t i=0, j=0, k=0;		
	char ContextBuffer[1024]="";
	strcpy(ContextBuffer, GetResultBuffer());
	const size_t ContextLen = ::strlen(ContextBuffer);

	const size_t MinLen = MIN(ContextLen, BufferSize-1);
	::memcpy(Buffer, ContextBuffer, sizeof(char)*MinLen);
	Buffer[MinLen] = '\0';	
	return true;
}
//-------------------------------------------------------------------------------------//