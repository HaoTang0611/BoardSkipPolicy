#pragma once
#include "stdafx.h"
#include "jet8000.h"
#include "FingerprintDefine.h"
#include "Fingerprint_Base.h"
#include "FingerprintWnd.h"
//---------------------------------------------------------------------------------//
bool Fingerprint_Base::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR * pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString & Error)
{
	if (JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false)
	{	return false;	}
	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_Base::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString & Error)
{
	if (JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false)
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_Base::LoadFPSINIFile()
{
	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("FingerPrintDevice");
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen] = _T("");
	//CString strDeviceName = m_DeviceName;
	//Filename = AOIDataCollect.GetSystemParamFilename();
	Filename = AOIDataDefine.GetFingerPrintConfigFilename();

	//KeyName.Format(_T("FPS Device Type"));
	//Default.Format(_T("%d"), m_FPSDeviceType);
	//if (LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true)
	//{	m_FPSDeviceType = (FPS_DEVICE)(JetAPI::StrToInt(String));	}

	//等待資料的次數
	KeyName.Format(_T("Wait for Data Count"));
	Default.Format(_T("%d"), m_FPSDeviceWaitDataCount);
	if (LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true)
	{	m_FPSDeviceWaitDataCount = JetAPI::StrToInt(String);	}

	//等待資料的延遲時間-ms
	KeyName.Format(_T("Wait for Data Dwell Time"));
	Default.Format(_T("%d"), m_FPSDeviceWaitDataDwellTime);
	if (LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true)
	{	m_FPSDeviceWaitDataDwellTime = JetAPI::StrToInt(String);	}

	//讀取資料前延遲時間-ms
	KeyName.Format(_T("Before Read Data Delay Time"));
	Default.Format(_T("%d"), m_FPSDeviceReadDataDelayTime);
	if (LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true)
	{	m_FPSDeviceReadDataDelayTime = JetAPI::StrToInt(String);	}

	//讀取資料前延遲時間-ms
	KeyName.Format(_T("Baud Rate"));
	Default.Format(_T("%d"), m_FPSDeviceBaudRate);
	if (LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true)
	{	m_FPSDeviceBaudRate = JetAPI::StrToInt(String);	}
	return true;
}
//---------------------------------------------------------------------------------//
bool Fingerprint_Base::SaveFPSINIFile()
{
	CString Filename;
	CString Section = _T("FingerPrintDevice");
	CString KeyName = _T("");
	CString String = _T("");

	Filename = AOIDataDefine.GetFingerPrintConfigFilename();
	
	//等待資料的次數
	KeyName.Format(_T("Wait for Data Count"));
	String.Format(_T("%d"), m_FPSDeviceWaitDataCount);
	if (SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false)
	{	return false;	}

	//等待資料的延遲時間-ms
	KeyName.Format(_T("Wait for Data Dwell Time"));
	String.Format(_T("%d"), m_FPSDeviceWaitDataDwellTime);
	if (SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false)
	{	return false;	}

	//讀取資料前延遲時間-ms
	KeyName.Format(_T("Before Read Data Delay Time"));
	String.Format(_T("%d"), m_FPSDeviceReadDataDelayTime);
	if (SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false)
	{	return false;	}

	//BaudRate
	KeyName.Format(_T("Baud Rate"));
	String.Format(_T("%d"), m_FPSDeviceBaudRate);
	if (SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false)
	{	return false;	}

	return true;
}
//---------------------------------------------------------------------------------//
CString Fingerprint_Base::GetErrorString() const
{
	return m_ErrorString;
}
//---------------------------------------------------------------------------------//
void Fingerprint_Base::SetErrorString(CString str)
{
	m_ErrorString = str;
}
//---------------------------------------------------------------------------------//
