// Barcode_Basic.cpp: implementation of the CBarcode_Basic class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Barcode_Basic.h"
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
CBarcode_Basic::CBarcode_Basic()
{
	PreInitBarcodeDevice();
	InitialBarcodeDevice();
}
//-------------------------------------------------------------------------------------//
CBarcode_Basic::CBarcode_Basic(const CBarcode_Basic &device)
{
	PreInitBarcodeDevice();
	CloneBarcodeDevice(device);
}
//-------------------------------------------------------------------------------------//
CBarcode_Basic::~CBarcode_Basic()
{

}
//-------------------------------------------------------------------------------------//
CBarcode_Basic& CBarcode_Basic::operator=(const CBarcode_Basic &device)
{
	if ( this == &device ) { return *this; }
	CloneBarcodeDevice(device);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Basic::PreInitBarcodeDevice()
{	
	ClearResultBuffer();	
	ClearResultBackup();
}
//-------------------------------------------------------------------------------------//
void CBarcode_Basic::InitialBarcodeDevice()
{	
	m_BarcodeDeviceID = -1;
	m_BarcodeDeviceType = BARCODE_DEVICE_NULL;//條碼機樣式
	m_BarcodeDeviceModel = _T("Barcode");//條碼機型號	

	m_BarcodeDevicePort = _T("1");
	m_BarcodeDeviceParity = SERIES_PARITY_NONE;
	m_BarcodeDeviceStopBits = SERIES_STOPBITS_10;
	m_BarcodeDeviceBaudRate = CBR_9600;

	m_BarcodeDeviceActived = false;
	m_BarcodeDeviceConnected = false;
	m_BarcodeDeviceCommDelayTime = 50;//通訊延遲時間
	m_BarcodeDeviceWaitDataCount = 100;//等待資料的次數
	m_BarcodeDeviceWaitDataDwellTime = 10;//等待資料的延遲時間-ms
	m_BarcodeDeviceReadDataDelayTime = 50;//讀取資料前延遲時間-ms
}
//-------------------------------------------------------------------------------------//
void CBarcode_Basic::CloneBarcodeDevice(const CBarcode_Basic &device)
{
	m_BarcodeDeviceID = device.m_BarcodeDeviceID;
	m_BarcodeDeviceType = device.m_BarcodeDeviceType;//條碼機樣式
	m_BarcodeDeviceModel = device.m_BarcodeDeviceModel;//條碼機型號	
	
	m_ErrorString = device.m_ErrorString;	
	m_BarcodeDevicePort = device.m_BarcodeDevicePort;
	m_BarcodeDeviceParity = device.m_BarcodeDeviceParity;
	m_BarcodeDeviceStopBits = device.m_BarcodeDeviceStopBits;
	m_BarcodeDeviceBaudRate = device.m_BarcodeDeviceBaudRate;
	m_BarcodeDeviceActived = device.m_BarcodeDeviceActived;
	m_BarcodeDeviceConnected = device.m_BarcodeDeviceConnected;
	m_BarcodeDeviceCommDelayTime = device.m_BarcodeDeviceCommDelayTime;//通訊延遲時間
	m_BarcodeDeviceWaitDataCount = device.m_BarcodeDeviceWaitDataCount;//等待資料的次數
	m_BarcodeDeviceWaitDataDwellTime = device.m_BarcodeDeviceWaitDataDwellTime;//等待資料的延遲時間-ms
	m_BarcodeDeviceReadDataDelayTime = device.m_BarcodeDeviceReadDataDelayTime;//讀取資料前延遲時間-ms

	::memcpy(m_ResultBuffer, device.m_ResultBuffer, sizeof(m_ResultBuffer));
	::memcpy(m_ResultBufferSubList, device.m_ResultBufferSubList, sizeof(m_ResultBufferSubList));

	::memcpy(m_ResultBackup, device.m_ResultBackup, sizeof(m_ResultBackup));
	::memcpy(m_ResultBackupSubList, device.m_ResultBackupSubList, sizeof(m_ResultBackupSubList));	
}
//-------------------------------------------------------------------------------------//
CString CBarcode_Basic::GetBarcodeDeviceFullName() const
{
	CString FullName;
	FullName.Format(_T("%s_%s"), _T("Barcode"), m_BarcodeDeviceModel);
	return FullName;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Basic::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Basic::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CBarcode_Basic::GetErrorString()
{ 	
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, _T("[HWBarcode]"));		
	AOIExceptionCodeCtrl.SetAOIExceptionCode_BarcodeDevice_Others(m_ErrorStringOut);
	return m_ErrorStringOut;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Basic::LoadBarcodeINIFile()
{	
	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen]=_T("");		
	CString strDeviceName = GetBarcodeDeviceFullName();	
	
	Filename = AOIDataCollect.GetSystemParamFilename();	
	Section.Format(_T("%s"), strDeviceName);	
	
	//通訊延遲時間
	KeyName.Format(_T("Communication Delay Time"));
	Default.Format(_T("%d"), m_BarcodeDeviceCommDelayTime);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	m_BarcodeDeviceCommDelayTime = JetAPI::StrToInt(String); }

	//等待資料的次數
	KeyName.Format(_T("Wait for Data Count"));
	Default.Format(_T("%d"), m_BarcodeDeviceWaitDataCount);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	m_BarcodeDeviceWaitDataCount = JetAPI::StrToInt(String); }

	//等待資料的延遲時間-ms
	KeyName.Format(_T("Wait for Data Dwell Time"));
	Default.Format(_T("%d"), m_BarcodeDeviceWaitDataDwellTime);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	m_BarcodeDeviceWaitDataDwellTime = JetAPI::StrToInt(String); }	 

	//讀取資料前延遲時間-ms
	KeyName.Format(_T("Before Read Data Delay Time"));
	Default.Format(_T("%d"), m_BarcodeDeviceReadDataDelayTime);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	m_BarcodeDeviceReadDataDelayTime = JetAPI::StrToInt(String); }	 

	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Basic::SaveBarcodeINIFile()
{
	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString String = _T("");
	CString strDeviceName = GetBarcodeDeviceFullName();
	
	Filename = AOIDataCollect.GetSystemParamFilename();	
	Section.Format(_T("%s"), strDeviceName);

	//通訊延遲時間
	KeyName.Format(_T("Communication Delay Time"));
	String.Format(_T("%d"), m_BarcodeDeviceCommDelayTime);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//等待資料的次數
	KeyName.Format(_T("Wait for Data Count"));
	String.Format(_T("%d"), m_BarcodeDeviceWaitDataCount);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//等待資料的延遲時間-ms
	KeyName.Format(_T("Wait for Data Dwell Time"));
	String.Format(_T("%d"), m_BarcodeDeviceWaitDataDwellTime);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//讀取資料前延遲時間-ms
	KeyName.Format(_T("Before Read Data Delay Time"));
	String.Format(_T("%d"), m_BarcodeDeviceReadDataDelayTime);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Basic::SaveBarcodeMovingTimeMsg(const char *pContext)//儲存條碼機移動時間訊息
{
#ifndef BARCODE_DEVICE_DISABLE
	const bool bSucc = AOIDataCollect.SaveMovingTimeMsg(pContext);	
	return bSucc;
#endif//BARCODE_DEVICE_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Basic::SaveBarcodeMovingTimeMsg(const wchar_t *pContext)//儲存條碼機移動時間訊
{
#ifndef BARCODE_DEVICE_DISABLE
	const bool bSucc = AOIDataCollect.SaveMovingTimeMsg(pContext);	
	return bSucc;
#endif//BARCODE_DEVICE_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
CString CBarcode_Basic::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("BARCODE_DEVICE_BASIC");
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Basic::ClearResultBuffer()
{
	::memset(m_ResultBuffer, 0x00, sizeof(m_ResultBuffer));
	::memset(m_ResultBufferSubList, 0x00, sizeof(m_ResultBufferSubList));	
}
//-------------------------------------------------------------------------------------//
void CBarcode_Basic::SetResultBuffer(const char* String)
{
	if ( NULL == String ) { return; }
	const size_t StrLen=::strlen(String);
	const size_t BufLen=sizeof(m_ResultBuffer)-1;
	const size_t CpyLen=MIN(StrLen, BufLen);	
	if ( StrLen > 0 )
	{
		CString ss;
		CString fnName=_T("CBarcode_Basic::SetResultBuffer#Input");
		fnName += CString(_T("["));
		for ( size_t i=0; i<StrLen; i++ )
		{	
			ss.Format(_T("0x%02x"), String[i]);
			if ( 0 == i )
			{	fnName += ss;	}
			else
			{	fnName += CString(_T(" "))+ss;	}
		}
		fnName += CString(_T("]"));
		SaveBarcodeMovingTimeMsg(fnName);
	}

	//::strcpy(m_ResultBuffer, String);	
	if ( ASCII_STX != String[0] )
	{	::memcpy(m_ResultBuffer, String, sizeof(char)*(CpyLen+1));	}
	else
	{	::memcpy(m_ResultBuffer, &(String[1]), sizeof(char)*(CpyLen));	}	

	if ( StrLen > 0 )
	{
		CString fnName=_T("CBarcode_Basic::SetResultBuffer");
		fnName += CString(_T("["))+CString(m_ResultBuffer)+CString(_T("]"));
		SaveBarcodeMovingTimeMsg(fnName);
	}
	return;
}
//-------------------------------------------------------------------------------------//
const char* CBarcode_Basic::GetResultBuffer() const
{
	return m_ResultBuffer;
}
//-------------------------------------------------------------------------------------//
int CBarcode_Basic::GetResultSubCount() const
{
	return MAX_BARCODE_DEVICE_CODE_COUNT;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Basic::SetResultSubBuffer(int idx, const char* String)
{
	if ( idx<0 || idx>=BARCODE_SUB_COUNT ) 
	{	return false; }
	::strcpy(m_ResultBufferSubList[idx], String);		

	if ( NULL != String )
	{
		CString fnName;
		fnName.Format(_T("%s#%02d"), _T("CBarcode_Basic::SetResultSubBuffer"), idx+1);
		fnName += CString(_T("["))+CString(m_ResultBufferSubList[idx])+CString(_T("]"));
		SaveBarcodeMovingTimeMsg(fnName);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
const char* CBarcode_Basic::GetResultSubBuffer(int idx) const
{
	if ( idx<0 || idx>=BARCODE_SUB_COUNT ) 
	{	return NULL; }
	return m_ResultBufferSubList[idx];	
}
//-------------------------------------------------------------------------------------//
void CBarcode_Basic::BackupResultBuffer()//備份條碼機內容
{
	::memcpy(m_ResultBackup, m_ResultBuffer, sizeof(m_ResultBackup));
	::memcpy(m_ResultBackupSubList, m_ResultBufferSubList, sizeof(m_ResultBufferSubList));	
}
//-------------------------------------------------------------------------------------//
void CBarcode_Basic::ClearResultBackup()
{
	::memset(m_ResultBackup, 0x00, sizeof(m_ResultBackup));
	::memset(m_ResultBackupSubList, 0x00, sizeof(m_ResultBackupSubList));	
}
//-------------------------------------------------------------------------------------//
const char* CBarcode_Basic::GetResultSubBackup(int idx) const
{
	if ( idx<0 || idx>=BARCODE_SUB_COUNT ) 
	{	return NULL; }
	return m_ResultBackupSubList[idx];		
}
//-------------------------------------------------------------------------------------//