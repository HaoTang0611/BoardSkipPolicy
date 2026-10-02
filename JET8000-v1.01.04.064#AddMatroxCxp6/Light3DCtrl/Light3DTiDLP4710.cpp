// Light3DTiDLP4710.cpp: implementation of the CLight3DTiDLP4710 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP4710.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_4710_USE
//-------------------------------------------------------------------------------------//
#define DELAY_I2C                        3000
//-------------------------------------------------------------------------------------//
#define DLP_LED_CURRENT_MAX				 1023		//最高亮度
#define DLP_LED_CURRENT_MIN					0		//最低亮度
//-------------------------------------------------------------------------------------//
#define DLP_PATTERN_INDEX_MODE_WAVE     1
#define DLP_PATTERN_INDEX_MODE_GC_BC    2
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLight3DTiDLP4710::CLight3DTiDLP4710()
{
	PreInitTiDlp(0, LIGHT_3D_CAST_00);	
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP4710::CLight3DTiDLP4710(int CtrlID, LIGHT_3D_CAST_ID Light3DID)
{
	PreInitTiDlp(CtrlID, Light3DID);
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP4710::~CLight3DTiDLP4710()
{		
	ClearPhaseZeroBuffer();	
	ClearPhaseFactorBuffer();
	DLPDisconnect();	
	::DeleteCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP4710::PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID Light3DID)
{	
	::InitializeCriticalSection(&m_csLight3D);

	m_dwFrmVersion = 0;	
	::memset(m_DLPFrmTag, 0x00, sizeof(m_DLPFrmTag));
	::memset(m_TiAPIversion, 0x00, sizeof(m_TiAPIversion));	
	::memset(m_DLPFrmversion, 0x00, sizeof(m_DLPFrmversion));	
	::memset(m_DLPMcuversion, 0x00, sizeof(m_DLPMcuversion));

	m_CtrlBoardID = CtrlID;
	m_Light3DCastID = Light3DID;
	m_Light3DDevice = LIGHT_3D_DEVICE_DLP4710;
	m_TrigOutCount = 0;	
	m_OperationMode = DLP_OPERATION_DEFAULT;	

	m_HWStatus = 0;
	m_SysStatus = 0;
	m_MainStatus = 0;	

	m_EnableTemperatureMonitor = true;
	
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroPtr = NULL;//平面相位指標
	m_PhaseZeroLEDColor = DLP_LED_COLOR_RED;

	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorPtr = NULL;//平面係數指標
	m_PhaseFactorLEDColor = DLP_LED_COLOR_RED;

	s_Index = 0;
	::memset(s_HorizontalPatternData, 0x00, sizeof(s_HorizontalPatternData));
	::memset(s_VerticalPatternData, 0x00, sizeof(s_VerticalPatternData));

	::memset(s_Patterns, 0x00, sizeof(s_Patterns));
	::memset(s_PatternSets, 0x00, sizeof(s_PatternSets));
	::memset(s_PatternOrderTable, 0x00, sizeof(s_PatternOrderTable));

	::memset(s_WriteBuffer, 0x00, sizeof(s_WriteBuffer));
	::memset(s_ReadBuffer, 0x00, sizeof(s_ReadBuffer));

	::memset(s_FlashProgramBuffer, 0x00, sizeof(s_FlashProgramBuffer));	

	s_StartProgramming = false;
	s_FlashProgramBufferPtr = NULL;
	s_FilePointer = NULL;

	//dlpc_common.h	
	s_WriteBufferSize = sizeof(s_WriteBuffer);
	s_WriteBufferIndex = 0;	
	s_ReadBufferSize = sizeof(s_ReadBuffer);
	s_ReadBufferIndex = 0;	
	::memset(&s_ProtocolData, 0x00, sizeof(s_ProtocolData));	

	switch ( Light3DID )
	{
	case LIGHT_3D_CAST_01:	m_CastName=_T("ID-01");	break;
	case LIGHT_3D_CAST_02:	m_CastName=_T("ID-02");	break;
	case LIGHT_3D_CAST_03:	m_CastName=_T("ID-03");	break;
	case LIGHT_3D_CAST_04:	m_CastName=_T("ID-04");	break;
	}
	this->InitialDLPParameter(m_DLPParam);
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP4710::InitialTiDlp()
{
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
	m_LEDCurrentR = -1;
	m_LEDCurrentG = -1;
	m_LEDCurrentB = -1;
	m_LEDCurrentMax = 1023;
	m_InvertPWM = false;
	m_LEDEnabled_R = true;
	m_LEDEnabled_G = true;
	m_LEDEnabled_B = true;
	m_LEDEnabled_Auto = true;
	m_ImageGamma = 1.0;
	m_SecondExpRatio = 1.0;	 
	m_Exposure_us = 0;
	m_TrigPeriod_us = 0;		
	m_Exposure2_us = 0;
	m_TrigPeriod2_us = 0;		
	m_DLPDelayTime = 20;
	m_LEDColor = DLP_LED_COLOR_NO;
	
	m_PatternBitCount = 6;
	m_PatternSetCountInDLP = 25;
	m_PatternDarkIndex_8Bit = 8;
	m_PatternGrayIndex_8Bit = 9;
	m_PatternWhiteIndex_8Bit = 10;	
	m_PatternIndex1_3Bit = -1;
	m_PatternIndex2_3Bit = -1;
	m_PatternIndex1_5Bit = -1;
	m_PatternIndex1_6Bit = 0;
	m_PatternIndex2_6Bit = 1;
	m_PatternIndex1_8Bit = 0;
	m_PatternIndex2_8Bit = 4;
	m_PatternIndexGC_1Bit= -1;
	m_PatternIndexBC_1Bit= -1;
	m_PatternIndexGC_8Bit= 12;
	m_PatternIndexBC_8Bit= 19;
	m_PatternStartNumGC_1Bit = -1;
	m_PatternStartNumBC_1Bit = -1;
	m_PatternStartNumGC_8Bit = -1;
	m_PatternStartNumBC_8Bit = -1;

	::memset(m_HeightFactor0, 0x00, sizeof(m_HeightFactor0));
	::memset(m_HeightFactor1, 0x00, sizeof(m_HeightFactor1));
	::memset(m_HeightFactor2, 0x00, sizeof(m_HeightFactor2));	
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP4710::LockLight3D()
{
	::EnterCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//}
inline void CLight3DTiDLP4710::UnlockLight3D()
{
	::LeaveCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDlpMonoMode() const
{
	return true;
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP4710::SetLCRErrorFnName(LPCTSTR LCRFnName)
{
	this->m_ErrorString.Format(_T("Error, TiDLP %s Fault"), LCRFnName);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetUSB_Number(LIGHT_3D_CAST_ID CastID, char USB_Number[])
{
#ifdef TB_SYSTEM_ONLY_BOT	
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::sprintf(USB_Number, "DLPB");	break;
	case LIGHT_3D_CAST_03:	::sprintf(USB_Number, "DLPC");	break;
	case LIGHT_3D_CAST_04:	::sprintf(USB_Number, "DLPD");	break;
	default:			    ::sprintf(USB_Number, "DLPA");	break;
	}		
#else
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::sprintf(USB_Number, "DLP2");	break;
	case LIGHT_3D_CAST_03:	::sprintf(USB_Number, "DLP3");	break;
	case LIGHT_3D_CAST_04:	::sprintf(USB_Number, "DLP4");	break;
	default:			    ::sprintf(USB_Number, "DLP1");	break;
	}			
#endif//TB_SYSTEM_ONLY_BOT
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[])
{
#ifdef TB_SYSTEM_ONLY_BOT	
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::wsprintfW(USB_Number, L"DLPB");	break;
	case LIGHT_3D_CAST_03:	::wsprintfW(USB_Number, L"DLPC");	break;
	case LIGHT_3D_CAST_04:	::wsprintfW(USB_Number, L"DLPD");	break;
	default:			    ::wsprintfW(USB_Number, L"DLPA");	break;
	}		
#else	
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::wsprintfW(USB_Number, L"DLP2");	break;
	case LIGHT_3D_CAST_03:	::wsprintfW(USB_Number, L"DLP3");	break;
	case LIGHT_3D_CAST_04:	::wsprintfW(USB_Number, L"DLP4");	break;
	default:			    ::wsprintfW(USB_Number, L"DLP1");	break;
	}		
#endif//TB_SYSTEM_ONLY_BOT
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetDLPID(int ID)
{	
	m_CtrlBoardID = ID;
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP4710::GetDLPID() const
{
	return m_CtrlBoardID;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CAST_ID CLight3DTiDLP4710::GetCastID() const
{
	return m_Light3DCastID;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_DEVICE_TYPE CLight3DTiDLP4710::GetDeviceType() const
{
	return m_Light3DDevice;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ChangeDeviceType(LIGHT_3D_DEVICE_TYPE Type)//變更裝置型號
{
	if ( GetDeviceType() != Type )
	{
		m_ErrorString=_T("Error, CLight3DTiDLP4710 can not change Device Type");
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP4710::GetDLPProjectName() const
{
	return this->m_CastName;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP4710::GetErrorString()
{	
	CString Key;
	CString CastName=GetDLPProjectName();	
	Key.Format(_T("[DLP:%s]"), CastName);
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP_Others(m_ErrorStringOut);
	return m_ErrorStringOut;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetDLPExceptionCode(DWORD Code, LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP(Code, str);
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP4710::GetDLPImageW() const
{
	return 1920;//912;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP4710::GetDLPImageH() const
{
	return 1080;//1140;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::DLPConnect()
{
	if ( DLPConnectFn() == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_CONNECT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::DLPConnectFn()
{
	SaveDLPProcess(_T("DLPConnect"), MSG_LEVEL_HIGH);

	bool SLmode = 0;
	const int DLPID = 0;
	char  USB_Number[256] = { 0 };
	if (m_I2C.GetIsConnected() == true)
	{
		SaveDLPProcess(_T("DLPConnect.BeforeCloseOldI2C"), MSG_LEVEL_HIGH);
		m_I2C.CYPRESS_I2C_CloseCyI2C();
		SaveDLPProcess(_T("DLPConnect.AfterCloseOldI2C"), MSG_LEVEL_HIGH);
	}

	SaveDLPProcess(_T("DLPConnect.BeforeGetUSBNumber"), MSG_LEVEL_HIGH);
	if (GetUSB_Number(m_Light3DCastID, USB_Number) == false)
	{
		//SetJetExceptionCode(JET_EXCEPTION_DLP_CTRL_CONNECT);
		return false;
	}
	SaveDLPProcess(_T("DLPConnect.AfterGetUSBNumber"), MSG_LEVEL_HIGH);

#ifdef DLP4710_MCU_USE
	SaveDLPProcess(_T("DLPConnect.BeforeConnectMCU"), MSG_LEVEL_HIGH);
	const bool McuConnected = m_I2C.CYPRESS_I2C_ConnectToCyI2C_MCU(USB_Number);
	SaveDLPProcess(_T("DLPConnect.AfterConnectMCU"), MSG_LEVEL_HIGH);

	if (McuConnected == true)
	{
		uint8_t datatemp[29], Datalength = 0;

		SaveDLPProcess(_T("DLPConnect.BeforeReadMCUVersion"), MSG_LEVEL_HIGH);
		const uint32_t McuVersionStatus = DLPC34xx_AH_ReadMcuVersion(datatemp, &Datalength);
		SaveDLPProcess(_T("DLPConnect.AfterReadMCUVersion"), MSG_LEVEL_HIGH);

		if (McuVersionStatus == SUCCESS)
		{
			::memcpy(m_DLPMcuversion, datatemp, sizeof(datatemp[0]) * 29);
		}
	}
	if (false == m_I2C.CYPRESS_I2C_CloseCyI2C()) {
		this->m_ErrorString.Format(_T("%s"), m_I2C.GetErrorString());
		return false;
	}
#endif//DLP4710_MCU_USE

	//::wsprintfW(USB_Number, L"LCR2");    
	SaveDLPProcess(_T("DLPConnect.BeforeConnectDLP"), MSG_LEVEL_HIGH);
	const bool DlpConnected = m_I2C.CYPRESS_I2C_ConnectToCyI2C(USB_Number);
	SaveDLPProcess(_T("DLPConnect.AfterConnectDLP"), MSG_LEVEL_HIGH);

	if (DlpConnected == false)
	{
		//SetJetExceptionCode(JET_EXCEPTION_DLP_CTRL_CONNECT);
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault"));
		return false;
	}

	uint16_t PatchVersion = 0;
	uint8_t  MinorVersion = 0, MajorVersion = 0;
	SaveDLPProcess(_T("DLPConnect.BeforeReadSystemSoftwareVersion"), MSG_LEVEL_HIGH);
	if (ReadSystemSoftwareVersion(&PatchVersion, &MinorVersion, &MajorVersion) == true)
	{
		sprintf(m_TiAPIversion, "%d.%d.%d", MajorVersion, MinorVersion, PatchVersion);
	}
	SaveDLPProcess(_T("DLPConnect.AfterReadSystemSoftwareVersion"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeReadFirmwareBuildVersion"), MSG_LEVEL_HIGH);
	if (ReadFirmwareBuildVersion(&PatchVersion, &MinorVersion, &MajorVersion) == true)
	{
		sprintf(m_DLPFrmversion, "%d.%d.%d", MajorVersion, MinorVersion, PatchVersion);
	}
	SaveDLPProcess(_T("DLPConnect.AfterReadFirmwareBuildVersion"), MSG_LEVEL_HIGH);

	::strcat(m_DLPFrmTag, "DLP4710-DLPC347x_Dual");//No Tag Support	

	unsigned int numImgInFlash = 0;
	//Retrieve the total number of Images in the firmware info, m_numImgInFlash=0


	bool ShowMsg = false;
	bool ResetFinish = true;
	SaveDLPProcess(_T("DLPConnect.BeforeResetLEDDisable"), MSG_LEVEL_HIGH);
	ResetLEDDisable(ResetFinish, ShowMsg);
	SaveDLPProcess(_T("DLPConnect.AfterResetLEDDisable"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeCheckDLPStatus"), MSG_LEVEL_HIGH);
	this->CheckDLPStatus();
	SaveDLPProcess(_T("DLPConnect.AfterCheckDLPStatus"), MSG_LEVEL_HIGH);

	//Check SL Mode
	DLPC34XX_DUAL_OperatingMode_e OperatingMode;
	SaveDLPProcess(_T("DLPConnect.BeforeReadOperatingMode"), MSG_LEVEL_HIGH);
	if (ReadOperatingModeSelect(&OperatingMode) == false)
	{
		m_OperationMode = DLP_OPERATION_DEFAULT;
	}
	else
	{
		m_OperationMode = MapOperatingModeSelect(OperatingMode);
	}
	SaveDLPProcess(_T("DLPConnect.AfterReadOperatingMode"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeSetOperatingMode"), MSG_LEVEL_HIGH);
	SetDLPOperationMode(m_OperationMode);
	SaveDLPProcess(_T("DLPConnect.AfterSetOperatingMode"), MSG_LEVEL_HIGH);

	//Check LED Parameters
	SaveDLPProcess(_T("DLPConnect.BeforeLoadDLPParameter"), MSG_LEVEL_HIGH);
	SaveDeviceType();
	LoadDLPParameter();
	SaveDLPProcess(_T("DLPConnect.AfterLoadDLPParameter"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeReadLEDSetting"), MSG_LEVEL_HIGH);
	GetDLPLEDPWMInvert(m_InvertPWM);//m_InvertPWM);	
	GetDLPLEDEnable(m_LEDEnabled_Auto, m_LEDEnabled_R, m_LEDEnabled_G, m_LEDEnabled_B);
	GetDLPLEDCurrent(m_LEDCurrentR, m_LEDCurrentG, m_LEDCurrentB);
	SaveDLPProcess(_T("DLPConnect.AfterReadLEDSetting"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeWriteLEDSetting"), MSG_LEVEL_HIGH);
	ExecDLPLightSetting(DLP_LED_CURRENT_ID_01);
	SaveDLPProcess(_T("DLPConnect.AfterWriteLEDSetting"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeWriteTriggerIn"), MSG_LEVEL_HIGH);
	if (WriteTriggerInConfiguration(DLPC34XX_DUAL_TE_ENABLE, DLPC34XX_DUAL_TP_ACTIVE_LOW) == false)
	{
		return false;
	}
	SaveDLPProcess(_T("DLPConnect.AfterWriteTriggerIn"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeWritePatternReady"), MSG_LEVEL_HIGH);
	if (WritePatternReadyConfiguration(DLPC34XX_DUAL_TE_ENABLE, DLPC34XX_DUAL_TP_ACTIVE_LOW) == false)
	{
		return false;
	}
	SaveDLPProcess(_T("DLPConnect.AfterWritePatternReady"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeWriteTriggerOut1"), MSG_LEVEL_HIGH);
	if (WriteTriggerOutConfiguration(DLPC34XX_DUAL_TT_TRIGGER1, DLPC34XX_DUAL_TE_ENABLE, DLPC34XX_DUAL_TI_INVERTED, 0) == false)
	{
		return false;
	}
	SaveDLPProcess(_T("DLPConnect.AfterWriteTriggerOut1"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.BeforeWriteTriggerOut2"), MSG_LEVEL_HIGH);
	if (WriteTriggerOutConfiguration(DLPC34XX_DUAL_TT_TRIGGER2, DLPC34XX_DUAL_TE_ENABLE, DLPC34XX_DUAL_TI_INVERTED, 0) == false)
	{
		return false;
	}
	SaveDLPProcess(_T("DLPConnect.AfterWriteTriggerOut2"), MSG_LEVEL_HIGH);

	SaveDLPProcess(_T("DLPConnect.Complete"), MSG_LEVEL_HIGH);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::DLPDisconnect(int WaitTime_ms)
{
	if ( DLPDisconnectFn(WaitTime_ms) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_DISCONNECT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::DLPDisconnectFn(int WaitTime_ms)
{
	CLight3DTiDLP4710::ExecDLPPattern_Stop();

	SaveDLPProcess(_T("DLPDisconnect"), MSG_LEVEL_HIGH);

	m_I2C.CYPRESS_I2C_CloseCyI2C();

	m_dwFrmVersion = 0;
	::memset(m_DLPFrmTag, 0x00, sizeof(m_DLPFrmTag));
	::memset(m_TiAPIversion, 0x00, sizeof(m_TiAPIversion));	
	::memset(m_DLPFrmversion, 0x00, sizeof(m_DLPFrmversion));	
	::memset(m_DLPMcuversion, 0x00, sizeof(m_DLPMcuversion));

	m_HWStatus = 0;
	m_SysStatus = 0;
	m_MainStatus = 0;
	//m_numImgInFlash = 0;

	if ( WaitTime_ms > 0 )
	{	::Sleep(WaitTime_ms);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPIsConnected()
{
	if ( m_I2C.CheckIsConnected() == false )
	{
		this->m_ErrorString.Format(_T("Error, TiDLP did not connect"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLPIsConnected()
{
	return GetDLPIsConnected();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLP4710McuIsConnected()//確認DLP4710的MCU可連線
{
	if ( 0 == m_DLPMcuversion[0] )
	{
		this->m_ErrorString.Format(_T("Error, Check DLP4710 MCU Connected Fault"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLPFrmForExpLut()//確認DLP韌體支援Exposure Lut
{
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SaveDLPProcess(LPCTSTR fnName, int Level)
{
	CString str;
	str.Format(_T("Light3D[%d]::%s"), m_Light3DCastID, fnName);
	if ( AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_LIGHT3D, Level, str) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPSoftwareReset(DWORD delayTime)
{
	if ( ExecDLPSoftwareResetFn(delayTime) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_RESET);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPSoftwareResetFn(DWORD delayTime)
{	
	if ( GetDLPIsConnected() == false )
	{	return false; }
	if ( CheckDLP4710McuIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ExecDLPSoftwareReset"), MSG_LEVEL_HIGH);	
	
	if ( ResetDLP4710() == false )
	{
		m_I2C.CYPRESS_I2C_CloseCyI2C();
		return false;
	}
	delayTime = MAX(delayTime, DELAY_I2C);
	::Sleep(delayTime);//暫停10秒
	m_I2C.CYPRESS_I2C_CloseCyI2C();

	if ( DLPConnect() == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetDLPLongAxisImageFlip(bool Flip)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPLongAxisImageFlip"), MSG_LEVEL_HIGH);

	DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, ShortAxisImageFlip;
	if ( ReadDisplayImageOrientation(&LongAxisImageFlip, &ShortAxisImageFlip) == false )
	{	return false; }

	if ( true == Flip ) { LongAxisImageFlip=DLPC34XX_DUAL_IF_IMAGE_FLIPPED; }
	else { LongAxisImageFlip=DLPC34XX_DUAL_IF_IMAGE_NOT_FLIPPED; }

	if ( WriteDisplayImageOrientation(LongAxisImageFlip, ShortAxisImageFlip) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPLongAxisImageFlip()
{
	DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, ShortAxisImageFlip;
	if ( ReadDisplayImageOrientation(&LongAxisImageFlip, &ShortAxisImageFlip) == false )
	{	return false; }	
	if ( DLPC34XX_DUAL_IF_IMAGE_NOT_FLIPPED == LongAxisImageFlip )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetDLPShortAxisImageFlip(bool Flip)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPShortAxisImageFlip"), MSG_LEVEL_HIGH);

	DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, ShortAxisImageFlip;
	if ( ReadDisplayImageOrientation(&LongAxisImageFlip, &ShortAxisImageFlip) == false )
	{	return false; }

	if ( true == Flip ) { ShortAxisImageFlip=DLPC34XX_DUAL_IF_IMAGE_FLIPPED; }
	else { ShortAxisImageFlip=DLPC34XX_DUAL_IF_IMAGE_NOT_FLIPPED; }

	if ( WriteDisplayImageOrientation(LongAxisImageFlip, ShortAxisImageFlip) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPShortAxisImageFlip()
{
	DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, ShortAxisImageFlip;
	if ( ReadDisplayImageOrientation(&LongAxisImageFlip, &ShortAxisImageFlip) == false )
	{	return false; }	
	if ( DLPC34XX_DUAL_IF_IMAGE_NOT_FLIPPED == ShortAxisImageFlip )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetDLPOperationMode(int Mode)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPOperationMode"), MSG_LEVEL_HIGH);
	DLPC34XX_DUAL_OperatingMode_e OperatingMode=MapOperatingModeSelect(Mode);
	if ( WriteOperatingModeSelect(OperatingMode) == false )
	{	return false; }
	m_OperationMode = Mode;	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetDLPOperationMode() const
{	
	return this->m_OperationMode;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_LEDEnabled_R!=bRed || m_LEDEnabled_G!=bGreen || m_LEDEnabled_B!=bBlue )//m_LEDEnabled_Auto!=bSeqCtrl
	{
		SaveDLPProcess(_T("SetDLPLEDEnable"), MSG_LEVEL_HIGH);
		if ( WriteRgbLedEnable(bRed, bGreen, bBlue) == false )		
		{	return false;	}
	}	

	m_LEDEnabled_R = bRed;
	m_LEDEnabled_G = bGreen;
	m_LEDEnabled_B = bBlue;
	m_LEDEnabled_Auto = bSeqCtrl;
	m_DLPParam.m_LEDEnabled_Auto = bSeqCtrl;
	m_DLPParam.m_LEDEnabled_R = bRed;
	m_DLPParam.m_LEDEnabled_G = bGreen;
	m_DLPParam.m_LEDEnabled_B = bBlue;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }
	
	if ( ReadRgbLedEnable(&bRed, &bGreen, &bBlue) == false )		
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP4710::GetPatternSetCountInDLP()
{
	return m_PatternSetCountInDLP;
}
//-------------------------------------------------------------------------------------//
inline int CLight3DTiDLP4710::GetDLPSafeCurrent(int value)
{
	int Max=m_LEDCurrentMax;//1024
	int Min=MAX(DLP_LED_CURRENT_MIN, 0);	
	if ( value > Max ) { return Max; }
	if ( value < Min ) { return Min; }	
	return static_cast<int>(value);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SaveDLPCurrentProcess(LPCTSTR pContext, bool bShowMsg)//儲存現在狀態
{	
	m_ErrorString = pContext;

	CString Err = GetErrorString();	
	AOIDataCollect.SaveCurrentProcess(Err);
	if ( true == bShowMsg )
	{	JetAPI::ShowMessageBox(Err); }
	return true;
}
//-------------------------------------------------------------------------------------//
inline int CLight3DTiDLP4710::GetDLPTrigType(int index, bool IntTrig, bool MultiTable)
{  
	int TrigType = DLP_TRIG_TYPE_INTERNAL;
	if ( true == IntTrig )
	{	TrigType = DLP_TRIG_TYPE_INTERNAL;	}
	else
	{
		if ( 0 == index )	
		{	TrigType = DLP_TRIG_TYPE_EXT_NEG; }
		else
		{
			if ( true == MultiTable )
			{	TrigType = DLP_TRIG_TYPE_EXT_NEG;	}
			else
			{	TrigType = DLP_TRIG_TYPE_INTERNAL;	}		
		}
	}
	return TrigType;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SaveDeviceType()
{	
	bool IsOK=true;
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section=_T("TiDLP");	
	LIGHT_3D_DEVICE_TYPE Type=GetDeviceType();
	CString ProjectName = this->GetDLPProjectName();	

	FileName = AOIDataCollect.GetLight3DParamFilename();
	Section.Format(_T("TiDLP-%s"), ProjectName);		

	KeyName.Format(_T("Device Type")); KeyString.Format(_T("%d"),Type);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_LEDCurrentR!=red || m_LEDCurrentG!=green || m_LEDCurrentB!=blue )
	{	
		SaveDLPProcess(_T("SetDLPLEDCurrent"), MSG_LEVEL_HIGH);
		
		int r = GetDLPSafeCurrent(red);
		int g = GetDLPSafeCurrent(green);
		int b = GetDLPSafeCurrent(blue);
		if ( WriteRgbLedCurrent(r, g, b) == false )
		{	return false;	}				
	}
	
	m_LEDCurrentR = red;
	m_LEDCurrentG = green;
	m_LEDCurrentB = blue;
	//if ( true == Update )//要此過濾再校正電流會有問題
	{
		SetDLPParamLEDCurrent(red, green, blue, CurrentID);		
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPLEDCurrent(int &red, int &green, int &blue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	uint16_t r=0, g=0, b=0;
    if ( ReadRgbLedCurrent(&r, &g, &b) == false )  
	{	return false;	}	
	red   = r;
	green = g;
	blue  = b;
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetDLPLEDPWMInvert(bool bInvert)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_InvertPWM != bInvert )
	{
		SaveDLPProcess(_T("SetDLPLEDPWMInvert"), MSG_LEVEL_HIGH);
		//if ( DLPC350_SetLEDPWMInvert(bInvert) < 0 )//Not Support
	}
	m_InvertPWM = bInvert;
	this->m_DLPParam.m_InvertPWM = bInvert;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPLEDPWMInvert(bool &bInvert)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }
    //if ( DLPC350_GetLEDPWMInvert(&bInvert) == -1 )//Not Support	
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLPStatus()
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( ReadShortStatus(&m_ShortStatus) == false )
	{	return false; }
	if ( ReadSystemStatus(&m_SystemStatus) == false )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPStatus_InitDone()
{
	if ( DLPC34XX_DUAL_SI_NOT_COMPLETE == m_ShortStatus.SystemInitialized )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPStatus_ForcedSwap()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPStatus_BufferFreeze()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPStatus_SeqRunning()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPStatus_SeqError()
{
	if ( DLPC34XX_DUAL_E_NO_ERROR == m_SystemStatus.SequenceError )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPStatus_SeqAbort()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPStatus_DRCError()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPStatus_DMDParked()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::InitialDLPParameter(TDLPParam &Param)
{	
	Param.m_LEDColor		=	DLP_LED_COLOR_WHITE;
	Param.m_TrigType		=	DLP_LED_TRIGGER_INTERNAL;
	Param.m_PatternBitDepth	=	6;
	Param.m_TrigInterval_us	=	10300;//10ms
	Param.m_PatternExp_us	=	10000;//10ms
	Param.m_PhaseMode   	=	LIGHT3D_PHASE_4_4_M;

	Param.m_LEDCurrentR_1	=	100;
	Param.m_LEDCurrentG_1	=	100;
	Param.m_LEDCurrentB_1	=	100;

	Param.m_LEDCurrentR_2	=	100;
	Param.m_LEDCurrentG_2	=	100;
	Param.m_LEDCurrentB_2	=	100;

	Param.m_InvertPWM		=	false;
	Param.m_LEDEnabled_Auto	=	true;
	Param.m_LEDEnabled_R	=	true;
	Param.m_LEDEnabled_G	=	true;
	Param.m_LEDEnabled_B	=	true;
	Param.m_FlipLong		=	false;
	Param.m_FlipShort		=	false;

//	bool	InvertPat = false;
//	bool	InsertBlack = true;
//	bool	TrigOutPrev = false;
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4710::SaveDLPParameter()
{	
	bool    IsOK = true;	
#ifndef PHASE_CTRL_DISABLE
#ifndef OFFLINE_VERSION
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section=_T("TiDLP");	
	CString ProjectName = this->GetDLPProjectName();

	FileName = AOIDataCollect.GetLight3DParamFilename();
	Section.Format(_T("TiDLP-%s"), ProjectName);		
	
	KeyName.Format(_T("LED Color")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDColor);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("Trig Type")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigType);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("Bit Depth")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternBitDepth);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Trig Interval")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigInterval_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Pattern Exp")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternExp_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Phase Mode")); KeyString.Format(_T("%d"),m_DLPParam.m_PhaseMode);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Period Time")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Exposure Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("Period Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime2_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Exposure Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime2_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("LED Current R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_1);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("LED Current G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_1);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_1);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current R 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_2);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("LED Current G 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_2);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current B 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_2);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		

	KeyName.Format(_T("Invert PWM")); KeyString.Format(_T("%d"),m_DLPParam.m_InvertPWM);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled Auto")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_Auto);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_R);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_G);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("LED Enabled B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_B);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Flip Long")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipLong);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Flip Short")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipShort);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Validate Delay Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ValidateDelayTime);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Use Exposure Pattern")); KeyString.Format(_T("%d"),m_DLPParam.m_UseExpLut);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Phase Factor Min")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMin);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		 

	KeyName.Format(_T("Phase Factor Max")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMax);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("LED Current Max")); KeyString.Format(_T("%d"), m_DLPParam.m_LEDCurrentMax);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Image Gamma")); KeyString.Format(_T("%.6f"), m_DLPParam.m_ImageGamma);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("2nd Exposure Ratio")); KeyString.Format(_T("%.6f"), m_DLPParam.m_SecondExpRatio);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Period Padding Time")); KeyString.Format(_T("%d"), m_DLPParam.m_PeriodPaddingTime);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Pattern Max Count")); KeyString.Format(_T("%d"), m_PatternSetCountInDLP);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}
#endif//OFFLINE_VERSION
#endif//PHASE_CTRL_DISABLE
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadDLPParameter()
{	
#ifndef PHASE_CTRL_DISABLE
	CString FileName;
	CString TempStr;
	CString KeyName;
	CString KeyString;
	CString Section=_T("DLP");
	CString ProjectName = this->GetDLPProjectName();
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];

	FileName = AOIDataCollect.GetLight3DParamFilename();
	Section.Format(_T("TiDLP-%s"),ProjectName);
	
	//LED Color	
	KeyName.Format(_T("LED Color")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDColor);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDColor = ::_ttoi(ReturnString); }
	m_PhaseZeroLEDColor = m_DLPParam.m_LEDColor;
	m_PhaseFactorLEDColor = m_DLPParam.m_LEDColor;
		
	//Trig Type	
	KeyName.Format(_T("Trig Type")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigType);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_TrigType = ::_ttoi(ReturnString);  }

	//Bit Depth	
	KeyName.Format(_T("Bit Depth")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternBitDepth);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PatternBitDepth = ::_ttoi(ReturnString); }

	//TrigInterval_us	
	KeyName.Format(_T("Trig Interval")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigInterval_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_TrigInterval_us = ::_ttoi(ReturnString); }

	//PatternExp_us	
	KeyName.Format(_T("Pattern Exp")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternExp_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PatternExp_us = ::_ttoi(ReturnString); }

	//Phase Mode
	KeyName.Format(_T("Phase Mode")); KeyString.Format(_T("%d"),m_DLPParam.m_PhaseMode);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PhaseMode = ::_ttoi(ReturnString); }
	m_PhaseZeroPhaseMode = m_DLPParam.m_PhaseMode;
	m_PhaseFactorPhaseMode = m_DLPParam.m_PhaseMode;
		
	//Period
	KeyName.Format(_T("Period Time")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PeriodTime_us = ::_ttoi(ReturnString); }

	//LED Exposure Time
	KeyName.Format(_T("Exposure Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_ExposureTime_us = ::_ttoi(ReturnString); }
	
	m_DLPParam.m_PeriodTime2_us = m_DLPParam.m_PeriodTime_us;
	m_DLPParam.m_ExposureTime2_us = m_DLPParam.m_ExposureTime_us;
	//Period 2
	KeyName.Format(_T("Period Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime2_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PeriodTime2_us = ::_ttoi(ReturnString); }

	//LED Exposure Time 2
	KeyName.Format(_T("Exposure Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime2_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_ExposureTime2_us = ::_ttoi(ReturnString); }	

	if ( m_DLPParam.m_PeriodTime_us < m_DLPParam.m_ExposureTime_us )
	{	m_DLPParam.m_PeriodTime_us = m_DLPParam.m_ExposureTime_us; }

	if ( m_DLPParam.m_PeriodTime2_us < m_DLPParam.m_ExposureTime2_us )
	{	m_DLPParam.m_PeriodTime2_us = m_DLPParam.m_ExposureTime2_us; }

	//LEDCurrent_R	
	KeyName.Format(_T("LED Current R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_1);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentR_1 = ::_ttoi(ReturnString); }

	//LEDCurrent_G	
	KeyName.Format(_T("LED Current G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_1);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{	m_DLPParam.m_LEDCurrentG_1 = ::_ttoi(ReturnString); }
		
	//LEDCurrent_B	
	KeyName.Format(_T("LED Current B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_1);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentB_1 = ::_ttoi(ReturnString); }

	//LEDCurrent_R_2
	KeyName.Format(_T("LED Current R 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_2);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentR_2 = ::_ttoi(ReturnString); }

	//LEDCurrent_G_2	
	KeyName.Format(_T("LED Current G 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_2);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{	m_DLPParam.m_LEDCurrentG_2 = ::_ttoi(ReturnString); }
		
	//LEDCurrent_B_2
	KeyName.Format(_T("LED Current B 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_2);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentB_2 = ::_ttoi(ReturnString); }	

	//InvertPWM	
	KeyName.Format(_T("Invert PWM")); KeyString.Format(_T("%d"),m_DLPParam.m_InvertPWM);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_InvertPWM=true;	}
		else
		{	m_DLPParam.m_InvertPWM=false;	}	
	}

	//LEDEnabled_Auto	
	KeyName.Format(_T("LED Enabled Auto")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_Auto);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_Auto=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_Auto=false;	}	
	}
		
	//LEDEnabled_R	
	KeyName.Format(_T("LED Enabled R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_R);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_R=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_R=false;	}	
	}

	//LEDEnabled_G
	KeyName.Format(_T("LED Enabled G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_G);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_G=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_G=false;	}	
	}

	//LEDEnabled_B	
	KeyName.Format(_T("LED Enabled B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_B);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_B=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_B=false;	}	
	}

	//FlipLong	
	KeyName.Format(_T("Flip Long")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipLong);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_FlipLong=true;	}
		else
		{	m_DLPParam.m_FlipLong=false;	}	
	}

	//FlipShort
	KeyName.Format(_T("Flip Short")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipShort);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_FlipShort=true;	}
		else
		{	m_DLPParam.m_FlipShort=false;	}	
	}

	KeyName.Format(_T("Validate Delay Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ValidateDelayTime);	
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_ValidateDelayTime = ::_ttoi(ReturnString);
		if( m_DLPParam.m_ValidateDelayTime < 0 )
		{	m_DLPParam.m_ValidateDelayTime=0;	}		
	}

	KeyName.Format(_T("Use Exposure Pattern")); KeyString.Format(_T("%d"),m_DLPParam.m_UseExpLut);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_UseExpLut=true;	}
		else
		{	m_DLPParam.m_UseExpLut=false;	}	
	}

	KeyName.Format(_T("Phase Factor Min")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMin);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_PhaseFactorMin = ::_ttof(ReturnString);		
		if ( m_DLPParam.m_PhaseFactorMin < 0 ) 
		{	m_DLPParam.m_PhaseFactorMin = 0; } 
	}	

	KeyName.Format(_T("Phase Factor Max")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMax);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_PhaseFactorMax = ::_ttof(ReturnString);		
		if ( m_DLPParam.m_PhaseFactorMax < 0 ) 
		{	m_DLPParam.m_PhaseFactorMax = 0; } 
	}

	KeyName.Format(_T("LED Current Max")); KeyString.Format(_T("%d"), m_DLPParam.m_LEDCurrentMax);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{
		m_LEDCurrentMax = ::_ttoi(ReturnString);
		m_DLPParam.m_LEDCurrentMax = ::_ttoi(ReturnString);
		if (m_LEDCurrentMax < 10)
		{	m_LEDCurrentMax = 10;	}
		if (m_LEDCurrentMax > 1023)
		{	m_LEDCurrentMax = 1023;	}
	}

	//Image Gamma
	KeyName.Format(_T("Image Gamma")); KeyString.Format(_T("%.6f"), m_DLPParam.m_ImageGamma);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_ImageGamma = ::_ttof(ReturnString);	}
	m_ImageGamma = m_DLPParam.m_ImageGamma;

	KeyName.Format(_T("2nd Exposure Ratio")); KeyString.Format(_T("%.6f"), m_DLPParam.m_SecondExpRatio);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_SecondExpRatio = ::_ttof(ReturnString);	}
	m_SecondExpRatio = m_DLPParam.m_SecondExpRatio;

	KeyName.Format(_T("Period Padding Time")); KeyString.Format(_T("%d"), m_DLPParam.m_PeriodPaddingTime);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_PeriodPaddingTime = ::_ttoi(ReturnString);	}

	KeyName.Format(_T("Pattern Max Count")); KeyString.Format(_T("%d"), m_PatternSetCountInDLP);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_PatternSetCountInDLP = ::_ttoi(ReturnString);	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadDLPParameter()//從DLP裝置讀取參數
{
#ifndef PHASE_CTRL_DISABLE
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ReadDLPParameter"), MSG_LEVEL_HIGH);

	//重新取回參數
	bool SLmode=false;	
	DLPC34XX_DUAL_OperatingMode_e OperatingMode;
	m_OperationMode = DLP_OPERATION_DEFAULT;
	if ( ReadOperatingModeSelect(&OperatingMode) == false )
	{	return false; }
	m_OperationMode=MapOperatingModeSelect(OperatingMode);


	bool bSeqCtrl = false;
	bool bRed=false, bGreen=false, bBlue=false;
	if ( ReadRgbLedEnable(&bRed, &bGreen, &bBlue) == false )
	{	return false;	}

	m_DLPParam.m_LEDEnabled_Auto = bSeqCtrl;
	m_DLPParam.m_LEDEnabled_R = bRed;
	m_DLPParam.m_LEDEnabled_G = bGreen;
	m_DLPParam.m_LEDEnabled_B = bBlue;

	uint16_t cRed=0, cGreen=0, cBlue=0;	
	if ( ReadRgbLedCurrent(&cRed, &cGreen, &cBlue) == false )    
	{	return false;	}

	m_LEDCurrentR = cRed;
	m_LEDCurrentG = cGreen;
	m_LEDCurrentB = cBlue;

	//m_DLPParam.m_LEDCurrentR_1 = cRed;
	//m_DLPParam.m_LEDCurrentG_1 = cGreen;
	//m_DLPParam.m_LEDCurrentB_1 = cBlue;
	
	bool bInvert=false;
	//if ( DLPC350_GetLEDPWMInvert(&bInvert) < 0 ) //Not Support	
	m_InvertPWM = bInvert;
	this->m_DLPParam.m_InvertPWM = bInvert;
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
TDLPParam& CLight3DTiDLP4710::GetDLPParam()
{
	return m_DLPParam;
}
//-------------------------------------------------------------------------------------//
const TDLPParam& CLight3DTiDLP4710::GetDLPParam() const
{
	return m_DLPParam;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetDLPPhaseMode(int Mode)
{
	bool IsOK = true;
	switch ( Mode )
	{
	case LIGHT3D_PHASE_2_2_M:
	case LIGHT3D_PHASE_4_4_1:
	case LIGHT3D_PHASE_4_4_2:
	case LIGHT3D_PHASE_4_4_M:
	case LIGHT3D_PHASE_4_4_M_2:
		m_DLPParam.m_PhaseMode = Mode;
		break;
	default:
		IsOK = false;
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetDLPParamLEDCurrent(int red, int green, int blue, int CurrentID)
{
	switch ( CurrentID )
	{
	case DLP_LED_CURRENT_ID_02:
		m_DLPParam.m_LEDCurrentR_2 = red;
		m_DLPParam.m_LEDCurrentG_2 = green;
		m_DLPParam.m_LEDCurrentB_2 = blue;
		break;
	default:
	case DLP_LED_CURRENT_ID_01:
		m_DLPParam.m_LEDCurrentR_1 = red;
		m_DLPParam.m_LEDCurrentG_1 = green;
		m_DLPParam.m_LEDCurrentB_1 = blue;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetDLPParamLEDCurrent(int &red, int &green, int &blue, int CurrentID)
{
	switch ( CurrentID )
	{
	case DLP_LED_CURRENT_ID_02:
		red   = m_DLPParam.m_LEDCurrentR_2;
		green = m_DLPParam.m_LEDCurrentG_2;
		blue  = m_DLPParam.m_LEDCurrentB_2;
		break;
	default:
	case DLP_LED_CURRENT_ID_01:
		red   = m_DLPParam.m_LEDCurrentR_1;
		green = m_DLPParam.m_LEDCurrentG_1;
		blue  = m_DLPParam.m_LEDCurrentB_1;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP4710::GetDLPFrmTag()
{
	return this->m_DLPFrmTag;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP4710::GetTiAPIVersion()
{
	return this->m_TiAPIversion;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP4710::GetDLPFrmVersion()
{
	return this->m_DLPFrmversion;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP4710::GetDLPMcuVersion()
{
	return this->m_DLPMcuversion;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetUseExpLut()
{
	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	
	return m_DLPParam.m_UseExpLut;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetLEDColor(int Type)
{
	m_LEDColor = Type;	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetLEDColor() const
{
	return m_LEDColor;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP4710::GetImageGamma() const
{
	//return 1.00f;
	//return 1.0f;
	return m_ImageGamma;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetImageGamma(double val)
{
	m_ImageGamma = val;
	m_DLPParam.m_ImageGamma = val;
	return true;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP4710::GetSecondExpRatio() const//取得第2次曝光比例
{
	//return 1.0;
	return m_SecondExpRatio;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetSecondExpRatio(double val)
{
	m_SecondExpRatio = val;
	m_DLPParam.m_SecondExpRatio = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPeriodPaddingTime() const//週期外加時間-us
{
	return m_DLPParam.m_PeriodPaddingTime;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetExposurePaddingTime() const//曝光外加時間-us
{
	return m_DLPParam.m_ExposurePaddingTime;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::CalcPeriodPaddingTime(int ExpTime) const//計算週期外加時間-us
{
	float MinExp=2100;//us
	const float MinPreExp=171;//us
	const float MinPostExp=31;//us
	const bool bMonoMode=GetDlpMonoMode();
	if ( true == bMonoMode )
	{	MinExp = 2100; }
	else
	{	MinExp = 6500; }
	const float ExpRatio=(ExpTime/MinExp);
	const int PreExp=(int)((MinPreExp*ExpRatio)+0.5);
	const int PostExp=(int)((MinPostExp*ExpRatio)+0.5);
	const int PaddingTime=(PreExp+PostExp)+10;
	return MAX(PaddingTime, GetPeriodPaddingTime());
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetUse3BitPattern() const//取得使用3Bit樣板圖
{
	const int PatternBitCount=GetPatternBitCount();
	if ( 3 != PatternBitCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetUse5BitPattern() const//取得使用5Bit樣板圖
{
	const int PatternBitCount=GetPatternBitCount();
	if ( 5 != PatternBitCount ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternBitCount() const//取得樣板圖位元數
{
	return m_PatternBitCount;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternBitCount(int val)//設定樣板圖位元數
{
	m_PatternBitCount = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternDarkIndex_8Bit() const//取得使用8Bit黑畫面引數-1
{
	return m_PatternDarkIndex_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternDarkIndex_8Bit(int val)//設定使用8Bit黑畫面引數-1	
{
	m_PatternDarkIndex_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternGrayIndex_8Bit() const//取得使用8Bit灰畫面引數-1
{
	return m_PatternGrayIndex_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternGrayIndex_8Bit(int val)//設定使用8Bit灰畫面引數-1	
{
	m_PatternGrayIndex_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternWhiteIndex_8Bit() const//取得使用8Bit白畫面引數-1
{
	return m_PatternWhiteIndex_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternWhiteIndex_8Bit(int val)//設定使用8Bit白畫面引數-1	
{
	m_PatternWhiteIndex_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndex1_3Bit() const//取得使用3Bit樣板引數-1
{
	return m_PatternIndex1_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndex1_3Bit(int val)//設定使用3Bit樣板引數-1
{
	m_PatternIndex1_3Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndex2_3Bit() const//取得使用3Bit樣板引數-2
{
	return m_PatternIndex2_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndex2_3Bit(int val)//設定使用3Bit樣板引數-2
{
	m_PatternIndex2_3Bit = val;	
}
//-------------------------------------------------------------------------------------//															 
int CLight3DTiDLP4710::GetPatternIndex1_5Bit() const//取得使用5Bit樣板引數-1
{
	return m_PatternIndex1_5Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndex1_5Bit(int val)//設定使用5Bit樣板引數-1
{
	m_PatternIndex1_5Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndex1_6Bit() const//取得使用6Bit樣板引數-1
{
	return m_PatternIndex1_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndex1_6Bit(int val)//設定使用6Bit樣板引數-1
{
	m_PatternIndex1_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndex2_6Bit() const//取得使用6Bit樣板引數-2
{
	return m_PatternIndex2_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndex2_6Bit(int val)//設定使用6Bit樣板引數-2
{
	m_PatternIndex2_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndex1_8Bit() const//取得使用8Bit樣板引數-1
{
	return m_PatternIndex1_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndex1_8Bit(int val)//設定使用8Bit樣板引數-1
{
	m_PatternIndex1_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndex2_8Bit() const//取得使用8Bit樣板引數-2
{
	return m_PatternIndex2_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndex2_8Bit(int val)//設定使用8Bit樣板引數-2
{
	m_PatternIndex2_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndexGC_1Bit() const//取得使用1Bit-GrayCode引數-1
{
	return m_PatternIndexGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndexGC_1Bit(int val)//設定使用1Bit-GrayCode引數-1
{
	m_PatternIndexGC_1Bit = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndexBC_1Bit() const//取得使用1Bit-BinaryCode引數-1
{
	return m_PatternIndexBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndexBC_1Bit(int val)//設定使用1Bit-BinaryCode引數-1
{
	m_PatternIndexBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndexGC_8Bit() const//取得使用8Bit-GrayCode引數-1
{	
	return m_PatternIndexGC_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndexGC_8Bit(int val)//設定使用8Bit-GrayCode引數-1
{
	m_PatternIndexGC_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternIndexBC_8Bit() const//取得使用8Bit-BinaryCode引數-1
{
	return m_PatternIndexBC_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternIndexBC_8Bit(int val)//設定使用8Bit-BinaryCode引數-1
{
	m_PatternIndexBC_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternStartNumGC_1Bit() const//取得使用1Bit-GrayCode起始張數-1
{
	return m_PatternStartNumGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternStartNumGC_1Bit(int val)//設定使用1Bit-GrayCode起始張數-1
{
	m_PatternStartNumGC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetPatternStartNumBC_1Bit() const//取得使用1Bit-BinaryCode起始張數-1
{
	return m_PatternStartNumBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPatternStartNumBC_1Bit(int val)//設定使用1Bit-BinaryCode起始張數-1
{
	m_PatternStartNumBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//	
int CLight3DTiDLP4710::GetPatternStartNumGC_8Bit() const//取得使用8Bit-GrayCode起始張數-1
{
	return m_PatternStartNumGC_8Bit;
}
//-------------------------------------------------------------------------------------//	
void CLight3DTiDLP4710::SetPatternStartNumGC_8Bit(int val)//設定使用8Bit-GrayCode起始張數-1
{
	m_PatternStartNumGC_8Bit = val;
}
//-------------------------------------------------------------------------------------//	
int CLight3DTiDLP4710::GetPatternStartNumBC_8Bit() const//取得使用8Bit-BinaryCode起始張數-1
{
	return m_PatternStartNumBC_8Bit;
}
//-------------------------------------------------------------------------------------//	
void CLight3DTiDLP4710::SetPatternStartNumBC_8Bit(int val)//設定使用8Bit-BinaryCode起始張數-1
{
	m_PatternStartNumBC_8Bit = val;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPeriod_us(unsigned int val)
{
	m_TrigPeriod_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetExposure_us(unsigned int val)
{
	m_Exposure_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPeriod2_us(unsigned int val)
{
	m_TrigPeriod2_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetExposure2_us(unsigned int val)
{
	m_Exposure2_us = val;		
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::InitialPatItem(TDLPPatItem &PatItem)
{
	PatItem = TDLPPatItem();	
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPatClear()//清除樣版內容
{
	SaveDLPProcess(_T("ExecDLPPatClear"), MSG_LEVEL_HIGH);

	this->m_PatternList.clear();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{
	if ( ExecDLPPatReadFn(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_READ);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPatReadFn(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{	
	if ( this->GetDLPIsConnected() == false )
	{	return false; }			

	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	

	SaveDLPProcess(_T("ExecDLPPatRead"), MSG_LEVEL_HIGH);

	
	int i=0;
	TDLPPatItem  PatItem;
	int Pat_Exposure = 0, Pat_Period = 0;
	int firstItem=32, lastItem=0, index=0;
	bool Invert_Pat=false, Insert_Black=false, Buf_Swap=false, TrigOutPrev=false;    
	int trig_type=0, Pat_Num=0, Bit_Depth=0, LED_Select=0, Frame_Index=0, Fresh_Index=0;    	
	unsigned int numLutEntries = GetPatternSetCountInDLP();

	Bit_Depth = 8;
	m_PatternList.clear();	
    for(i=0; i<numLutEntries; i++)
    {		
		uint8_t PatternOrderTableEntryIndex=i;
		DLPC34XX_DUAL_PatternOrderTableEntry_s PatternOrderTableEntry;
		if ( ReadPatternOrderTableEntry(PatternOrderTableEntryIndex, &PatternOrderTableEntry) == false )
		{	continue; }
		
		Pat_Exposure = PatternOrderTableEntry.IlluminationTime;
		Pat_Period = PatternOrderTableEntry.PreIlluminationDarkTime+PatternOrderTableEntry.IlluminationTime+PatternOrderTableEntry.PostIlluminationDarkTime;

		LED_Select = DLP_LED_COLOR_NO;		
		if ( DLPC34XX_DUAL_IE_ENABLE == PatternOrderTableEntry.RedIlluminator ) 
		{ 
			if ( DLP_LED_COLOR_NO == LED_Select ) { LED_Select = DLP_LED_COLOR_RED; }
			else { LED_Select = DLP_LED_COLOR_WHITE; }			
		}
		if ( DLPC34XX_DUAL_IE_ENABLE == PatternOrderTableEntry.GreenIlluminator )
		{
			if ( DLP_LED_COLOR_NO == LED_Select ) { LED_Select = DLP_LED_COLOR_GREEN; }
			else { LED_Select = DLP_LED_COLOR_WHITE; }
		}
		if ( DLPC34XX_DUAL_IE_ENABLE == PatternOrderTableEntry.BlueIlluminator )
		{
			if ( DLP_LED_COLOR_NO == LED_Select ) { LED_Select = DLP_LED_COLOR_BLUE; }
			else { LED_Select = DLP_LED_COLOR_WHITE; }			
		}

		PatItem.sFlashIndex = PatternOrderTableEntry.PatSetIndex;
		PatItem.sBitDepth = Bit_Depth;
		PatItem.sColorIndex = LED_Select;
		PatItem.sTrigType = trig_type;
		PatItem.sBitNum  = Pat_Num;
		PatItem.sBitStart = firstItem;
		PatItem.sBitEnd = lastItem;
		PatItem.sInvertPattern = Invert_Pat;
		PatItem.sInsertBlack = Insert_Black;
		PatItem.sTrigOutPrev = TrigOutPrev;
		PatItem.sBufSwap = Buf_Swap;
		PatItem.nExposure = Pat_Exposure;
		PatItem.nPeriod = Pat_Period;
		this->m_PatternList.push_back(PatItem);			

		if ( 0 == Exposure_us )
		{	Exposure_us = Pat_Exposure;	}
		if ( 0 == TrigPeriod_us )
		{	TrigPeriod_us = Pat_Period;	}		
    }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{
	if ( ExecDLPPatSendAllFn(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_SEND_ALL);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPatSendAllFn(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{	
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ExecDLPPatSendAll"), MSG_LEVEL_HIGH);

	int i=0;  
	CString str;
	CString ItemText;	
	TDLPPatItem PatItem;
	const int PatItemCount = (int)(this->m_PatternList.size());	

	//Make sure the Pattern Exposure and Pattern Period timings are within the spec

    //Pattern Exposure > Pattern Period not a valid settings
    if( Exposure_us > TrigPeriod_us )
    {
        m_ErrorString = _T("Pattern exposure setting voilation, it should be, Pattern Exposure = Pattern Period or (Pattern Period - Pattern Exposure) > 230us");
        return false;
    }
	
	uint32_t ExposureTime = Exposure_us;	
	DLPC34XX_DUAL_ValidateExposureTime_s ValidateExposureTime;
	DLPC34XX_DUAL_PatternMode_e PatternMode=DLPC34XX_DUAL_PM_INTERNAL;
	DLPC34XX_DUAL_SequenceType_e eBitDepth=DLPC34XX_DUAL_ST_EIGHT_BIT_RGB;		

	int LedUsdCnt=0;
	const bool bMonoMode = GetDlpMonoMode();
	DLPC34XX_DUAL_IlluminatorEnable_e RedEnb, GrnEnb, BluEnb;
	DLPC34XX_DUAL_WriteControl_e WriteControl;
	DLPC34XX_DUAL_PatternOrderTableEntry_s PatternOrderTableEntry;
    for (i=0; i<PatItemCount; i++)
    {
		PatItem = m_PatternList[i];
		if ( 0 == i )
		{	WriteControl = DLPC34XX_DUAL_WC_START;	}
		else
		{	WriteControl = DLPC34XX_DUAL_WC_CONTINUE;	}
		::memset(&PatternOrderTableEntry, 0x00, sizeof(PatternOrderTableEntry));

		LedUsdCnt=0;
		RedEnb = GrnEnb = BluEnb = DLPC34XX_DUAL_IE_DISABLE;		
		switch ( PatItem.sColorIndex )
		{
		case DLP_LED_COLOR_RED:    RedEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_GREEN:  GrnEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_YELLOW: RedEnb = GrnEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_BLUE:   BluEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_MAGENTA: RedEnb = BluEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_CYAN:    GrnEnb = BluEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_WHITE:  RedEnb = GrnEnb = BluEnb = DLPC34XX_DUAL_IE_ENABLE;	break;			
			break;		
		}
		if (DLP_LED_COLOR_YELLOW==PatItem.sColorIndex || DLP_LED_COLOR_MAGENTA==PatItem.sColorIndex || DLP_LED_COLOR_CYAN==PatItem.sColorIndex || DLP_LED_COLOR_WHITE==PatItem.sColorIndex )
		{
			if (true == bMonoMode)
			{
				BluEnb = DLPC34XX_DUAL_IE_ENABLE;
				RedEnb = GrnEnb = DLPC34XX_DUAL_IE_DISABLE;
			}
		}
		
		if ( DLPC34XX_DUAL_IE_ENABLE== RedEnb ) { LedUsdCnt++; }
		if ( DLPC34XX_DUAL_IE_ENABLE== GrnEnb ) { LedUsdCnt++; }
		if ( DLPC34XX_DUAL_IE_ENABLE== BluEnb ) { LedUsdCnt++; }
		if ( LedUsdCnt > 1 )
		{	eBitDepth=DLPC34XX_DUAL_ST_EIGHT_BIT_RGB;	}
		else
		{	eBitDepth=DLPC34XX_DUAL_ST_EIGHT_BIT_MONO;	}

		ExposureTime = PatItem.nExposure;
		if ( ReadValidateExposureTime(PatternMode, eBitDepth, ExposureTime, &ValidateExposureTime) == false )
		{	return false;	}
		if ( ExposureTime < ValidateExposureTime.MinimumExposureTime )
		{
			  m_ErrorString.Format(_T("Pattern[%02d] exposure time[%d us] less than Cast minimum exposure time[%d]"), i+1, ExposureTime, ValidateExposureTime.MinimumExposureTime);
			  return false;
		}
		const uint32_t PreExpPostTime=ValidateExposureTime.PreExposureDarkTime+ValidateExposureTime.MinimumExposureTime+ValidateExposureTime.PostExposureDarkTime;
		if ( PatItem.nPeriod < PreExpPostTime )
		{
			  m_ErrorString.Format(_T("Pattern[%02d] period time[%d us] less than Cast minimum period time[%d]"), i+1, PatItem.nPeriod, PreExpPostTime);
			  return false;
		}    	

		PatternOrderTableEntry.PatSetIndex=PatItem.sFlashIndex;
		PatternOrderTableEntry.NumberOfPatternsToDisplay = 1;

		PatternOrderTableEntry.RedIlluminator   = RedEnb;
		PatternOrderTableEntry.GreenIlluminator = GrnEnb;
		PatternOrderTableEntry.BlueIlluminator  = BluEnb;		 

		PatternOrderTableEntry.PatternInvertLsword = 0;
		PatternOrderTableEntry.PatternInvertMsword = 0;
		PatternOrderTableEntry.IlluminationTime = ExposureTime;
		PatternOrderTableEntry.PreIlluminationDarkTime  = ValidateExposureTime.PreExposureDarkTime;
		PatternOrderTableEntry.PostIlluminationDarkTime = ValidateExposureTime.PostExposureDarkTime;
		if ( WritePatternOrderTableEntry(WriteControl, &PatternOrderTableEntry) == false )
		{	return false; }
    }	    
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( ExecDLPPatSendOneFn(bExpLut, index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_SEND_ONE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPatSendOneFn(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }
	SaveDLPProcess(_T("ExecDLPPatSendOne_Lut"), MSG_LEVEL_HIGH);
	return ReturnNotSupportFunc(_T("CLight3DTiDLP4710::ExecDLPPatSendOne"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep)//套用樣板列表資料
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	Status = 0;
	SaveDLPProcess(_T("ExecDLPValidatePatLutData"), MSG_LEVEL_HIGH);		
	if ( true == Sleep )
	{
		if ( m_DLPParam.m_ValidateDelayTime > 0 ) 
		{	::Sleep(m_DLPParam.m_ValidateDelayTime); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force)//建立樣板, 傳送樣板以及驗證
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }	

	if ( Periodus < exposure_us )
	{	Periodus = exposure_us; }
	const unsigned int Exposure_us = exposure_us;
	const unsigned int TrigPeriod_us = Periodus;	

	bool HasSetBefore = true;
	if ( true == Force )//強迫更新
	{	HasSetBefore = false;	}
	else
	{
		if ( Mode==DLP_PATTERN_SEQUENCE_DEBUG || m_PatternMode!=Mode || m_Exposure_us!=Exposure_us || m_TrigPeriod_us!=TrigPeriod_us || m_LEDColor!=LEDColor )
		{	HasSetBefore = false;	}
	}
	if ( true == HasSetBefore )
	{	return true; }	

	TDLPParam &DLPParam = GetDLPParam();	
	DLPParam.m_PeriodTime_us = TrigPeriod_us;
	DLPParam.m_ExposureTime_us = Exposure_us;	
	if ( this->BuildDLPPatternList(Mode, IntTrig, MultiTable, LEDColor) == false )
	{	return false;	}

	const bool bPatFrmVideo = false;
	const bool bTrigIntExt = true;
#ifndef LIGHT_CTRL_DISABLE
	const bool bRepeat = true;
#else
	const bool bRepeat = false;
#endif//LIGHT_CTRL_DISABLE
	const bool bExpLut = GetUseExpLut();
	SaveDLPProcess(_T("ExecDLPPatBuildSendValidate"), MSG_LEVEL_HIGH);
	if ( this->ExecDLPPatSendAll(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{	return false;	}

	unsigned int Status=0;
	if ( this->ExecDLPValidatePatLutData(Status, Sleep) == false )
	{	return false;	}
	if ( Status != 0 )
	{
		this->m_ErrorString = _T("");
		if ( (Status&BIT0)==BIT0 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Exposure and Period OOR)");	}
		else if ( (Status&BIT1)==BIT1 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Pattern Number OOR)");	}
		else if ( (Status&BIT2)==BIT2 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Count trigger out ovelaps black)");	}
		else if ( (Status&BIT3)==BIT3 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Black vector missing)");	}
		else if ( (Status&BIT4)==BIT4 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Exposure, Period diff < 230 )");	}
		else
		{	this->m_ErrorString.Format(_T("Error, TIDLP Exception (%d)"), Status); }
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_VALIDATE);
		return false; 
	}
	m_PatternMode = Mode;
	SetLEDColor(LEDColor);
	SetPeriod_us(TrigPeriod_us);
	SetExposure_us(Exposure_us);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPLightSetting(int CurrentID)//執行DLP的LED設定-依據目前的設定
{
	const bool bUpdate = false;
	int CurRed=0, CurGrn=0, CurBlu= 0;
	CLight3DTiDLP4710 *Light3DPtr = this;
	Light3DPtr->GetDLPParamLEDCurrent(CurRed, CurGrn, CurBlu, CurrentID);
	if ( Light3DPtr->SetDLPLEDEnable(true, true, true, true) == false )
	{	return false;	}		
	if ( Light3DPtr->SetDLPLEDCurrent(CurRed, CurGrn, CurBlu, bUpdate, CurrentID) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPattern_Run()
{
	if ( ExecDLPPattern_RunFn() == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_PLAY);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPattern_RunFn()
{
	if ( this->GetDLPIsConnected() == false ) { return false; }	
/*
	bool repeat;
	unsigned int numLutEntries, numPatsForTrigOut2, numSplash;    
	if( DLPC350_GetPatternConfig(&numLutEntries, &repeat, &numPatsForTrigOut2, &numSplash) == -1 )
	{
		this->m_ErrorString.Format("Get Pat Config Error!");
		return false;
	}
*/
	SaveDLPProcess(_T("ExecDLPPattern_Run"), MSG_LEVEL_HIGH);
	//0xFF=Continue
	if ( WriteInternalPatternControl(DLPC34XX_DUAL_PC_START, 0xFF) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPattern_Stop()
{
	if ( ExecDLPPattern_StopFn() == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_STOP);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPattern_StopFn()
{
	if ( this->GetDLPIsConnected() == false ) { return false; }	

	SaveDLPProcess(_T("ExecDLPPattern_Stop"), MSG_LEVEL_HIGH);
	if ( WriteInternalPatternControl(DLPC34XX_DUAL_PC_STOP) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPattern_Pause()
{
	if ( ExecDLPPattern_PauseFn() == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_PAUSE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ExecDLPPattern_PauseFn()
{
	if ( this->GetDLPIsConnected() == false ) { return false; }	

	SaveDLPProcess(_T("ExecDLPPattern_Pause"), MSG_LEVEL_HIGH);
	if ( WriteInternalPatternControl(DLPC34XX_DUAL_PC_PAUSE) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LEDSetting(int LEDCurrent, int CurrentID)
{
	bool InvertPWM = false;
	const bool bUpdate = false;
	int LEDCurrent_R=150, LEDCurrent_G=150, LEDCurrent_B=150;
	bool LEDEnabled_Auto=true, LEDEnabled_R=true, LEDEnabled_G=true, LEDEnabled_B=true;	

	InvertPWM = this->m_DLPParam.m_InvertPWM;
	LEDEnabled_Auto = this->m_DLPParam.m_LEDEnabled_Auto;
	LEDEnabled_R = this->m_DLPParam.m_LEDEnabled_R;
	LEDEnabled_G = this->m_DLPParam.m_LEDEnabled_G;
	LEDEnabled_B = this->m_DLPParam.m_LEDEnabled_B;
	GetDLPParamLEDCurrent(LEDCurrent_R, LEDCurrent_G, LEDCurrent_B, CurrentID);	
	SaveDLPProcess(_T("LEDSetting"), MSG_LEVEL_HIGH);

	if( SetDLPLEDEnable(LEDEnabled_Auto, LEDEnabled_R, LEDEnabled_G , LEDEnabled_B) == false ) { return false; }
	if( SetDLPLEDCurrent(LEDCurrent_R, LEDCurrent_G, LEDCurrent_B, bUpdate, CurrentID) == false ) { return false; }
	if( SetDLPLEDPWMInvert(InvertPWM) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP4710::GetTriggerOutCount()
{
	return m_TrigOutCount;
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP4710::GetDLPPatCount()
{
	return m_PatternList.size();
}
//-------------------------------------------------------------------------------------//
TDLPPatItem* CLight3DTiDLP4710::GetDLPPatItemPtr(size_t index, bool check)
{
	if ( true == check )
	{
		const size_t size = m_PatternList.size();
		if ( index >= size ) 
		{	return NULL; }
	}
	return &(m_PatternList[index]);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::ClearDLPPatternList()//清除m_PatternList
{
	SaveDLPProcess(_T("ClearDLPPatternList"), MSG_LEVEL_HIGH);
	m_PatternList.clear();
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLPPatternIndex(int index, unsigned int Count, int Mode)
{
	if ( index<0 || index>=Count )
	{
		if ( DLP_PATTERN_INDEX_MODE_GC_BC == Mode )
		{	m_ErrorString.Format(_T("Error, DLP Pattern Index Exception [%d/%d] (GC/BC)"), index, Count);	}
		else
		{	m_ErrorString.Format(_T("Error, DLP Pattern Index Exception [%d/%d]"), index, Count); }
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表 
{	
	bool IsOK = true;
	SaveDLPProcess(_T("BuildDLPPatternList"), MSG_LEVEL_HIGH);	
	BuildDLPPatternList_TestGC(IntTrig, MultiTable, LEDColor);
	m_PatternList.clear();
	switch ( Mode )
	{
	case DLP_PATTERN_SEQUENCE_WHITE:	IsOK=BuildDLPPatternList_White(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_RGB:		IsOK=BuildDLPPatternList_RGB(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_2_2_M:	IsOK=BuildDLPPatternList_2X2_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4_1:	IsOK=BuildDLPPatternList_4X4_1(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4_2:	IsOK=BuildDLPPatternList_4X4_2(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_2_M:	IsOK=BuildDLPPatternList_4X2_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4_M:	IsOK=BuildDLPPatternList_4X4_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4GC_M:	IsOK=BuildDLPPatternList_4X4GC_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_5GC_M:	IsOK=BuildDLPPatternList_4X5GC_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_6GC_M:	IsOK=BuildDLPPatternList_4X6GC_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4GC_M2: IsOK=BuildDLPPatternList_4X4GC_M2(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_5GC_M2: IsOK=BuildDLPPatternList_4X5GC_M2(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_6GC_M2: IsOK=BuildDLPPatternList_4X6GC_M2(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_2_M_2:	IsOK=BuildDLPPatternList_4X2_M2(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4_M_2:	IsOK=BuildDLPPatternList_4X4_M2(IntTrig, MultiTable, LEDColor); break;
	default:
		IsOK = true;
		break;
	}
	if ( false == IsOK )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_SET_FUNC);
		return false; 
	}	
	m_TrigOutCount = m_PatternList.size();	
	if ( 0 == m_TrigOutCount )
	{
		m_ErrorString.Format(_T("Error, BuildDLPPatternList Fault[Mode=%d]"), Mode);
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_SET_FUNC);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_TestGC(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-測試GC
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;
	
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	
	const int UseWaveCode=0;
	const int UseGrayCode=1;
	const int UseBinaryCode=2;
	const int UseCodeMode=UseWaveCode;
	if ( UseGrayCode==UseCodeMode )
	{	//0~9:2+4+8+16+32+64+128+256+512+1024
		bool bFirst=true;
		const int nStart=2;
		const int UseCnt=8;
		const int FlashIndexCode=18;
		if ( CheckDLPPatternIndex(FlashIndexCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == true )
		{
			for ( int i=nStart; i<nStart+UseCnt; i++ )
			{
				if ( true == bFirst )
				{
					bFirst = false;
					PatItem.sFlashIndex = FlashIndexCode;
					PatItem.sBitDepth = DLP_BIT_DEPTH_1;
					PatItem.sColorIndex = DLPLEDColor;
					PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
					PatItem.sBitNum = i;
					PatItem.sInvertPattern = false;
					PatItem.sInsertBlack = true;
					PatItem.sTrigOutPrev = false;
					PatItem.sBufSwap = true;
					PatItem.nPeriod = PatPeriod1;
					PatItem.nExposure = PatExposure1;
					this->AddDLPPPatItem(PatItem);
					index++;
					continue;
				}
				PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
				PatItem.sBitNum = i;//4
				PatItem.sBufSwap = false;
				this->AddDLPPPatItem(PatItem);
				index++;
			}
		}
		return true;
	}
	if ( UseBinaryCode==UseCodeMode )
	{	//10~19:2+4+8+16+32+64+128+256+512+1024
		bool bFirst=true;
		const int nStart=12;
		const int UseCnt=8;
		const int FlashIndexCode=18;
		if ( CheckDLPPatternIndex(FlashIndexCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == true )
		{
			for ( int i=nStart; i<nStart+UseCnt; i++ )
			{
				if ( true == bFirst )
				{
					bFirst = false;
					PatItem.sFlashIndex = FlashIndexCode;
					PatItem.sBitDepth = DLP_BIT_DEPTH_1;
					PatItem.sColorIndex = DLPLEDColor;
					PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
					PatItem.sBitNum = i;
					PatItem.sInvertPattern = false;
					PatItem.sInsertBlack = true;
					PatItem.sTrigOutPrev = false;
					PatItem.sBufSwap = true;
					PatItem.nPeriod = PatPeriod1;
					PatItem.nExposure = PatExposure1;
					this->AddDLPPPatItem(PatItem);
					index++;
					continue;
				}
				PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
				PatItem.sBitNum = i;//4
				PatItem.sBufSwap = false;
				this->AddDLPPPatItem(PatItem);
				index++;
			}
		}
		return true;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_White(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-白燈
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexD = GetPatternDarkIndex_8Bit();
	const int FlashIndexG = GetPatternGrayIndex_8Bit();
	const int FlashIndexW = GetPatternWhiteIndex_8Bit();

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8	
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	const int FlashIndex = FlashIndexW;//FlashIndexW;//FlashIndexA, 8
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 24;	
	PatItem.sInvertPattern = true;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_RGB(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-紅綠藍燈
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;	

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLP_LED_COLOR_RED;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 24;	
	PatItem.sInvertPattern = true;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;

	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sColorIndex = DLP_LED_COLOR_GREEN;
	PatItem.sBitNum = 24;	
	PatItem.sBufSwap = false;		
	this->AddDLPPPatItem(PatItem);
	index ++;

	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sColorIndex = DLP_LED_COLOR_BLUE;
	PatItem.sBitNum = 24;	
	PatItem.sBufSwap = false;		
	this->AddDLPPPatItem(PatItem);
	index ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_2X2_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-22M
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;	

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	//6x(2+2+1)=30
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index++;
	FlashIndex++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;//000
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//PatItem.sFlashIndex = FlashIndex;
	//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	//PatItem.sBitNum = 1;//270	
	//PatItem.sBufSwap = false;
	//this->AddDLPPPatItem(PatItem);
	//index ++;
	//FlashIndex++;

	//PatItem.sFlashIndex = FlashIndex;
	//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	//PatItem.sBitNum = 2;//180
	//PatItem.sBufSwap = false;
	//this->AddDLPPPatItem(PatItem);
	//index ++;
	//FlashIndex++;

	//PatItem.sFlashIndex = FlashIndex;
	//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	//PatItem.sBitNum = 3;//090
	//PatItem.sBufSwap = false;
	//this->AddDLPPPatItem(PatItem);
	//index ++;
	//FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X4_1(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-421
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8	
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X4_2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-422
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexB;	
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X2_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-42M
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	//6x4+6x2=32
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;//000
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	/*
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;//270	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;//180
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
	*/

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;//090
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X4_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-44M
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;	

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	//const int FlashIndexA = 4;
	//const int FlashIndexB = 8;//0;//1,7,8
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	//6x4x2=48 
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
		
	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X4GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X4GCM
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }
	
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }
	
	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex=FlashIndexGrayCode;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		

	const int IndexCount=FlashIndex-FlashIndexGrayCode;
	FlashIndex=FlashIndexBinaryCode+IndexCount+1;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	FlashIndex ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X5GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X5GCM
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }
	
	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex=FlashIndexGrayCode;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	const int IndexCount=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCount;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X6GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X6GCM
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }

	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex = FlashIndexGrayCode;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+4;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	const int IndexCount=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCount;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X4GC_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X4GCM2
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const double Exp2Ratio=GetSecondExpRatio();	
	int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
	int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
	int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
	int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	
	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;	

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();		
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }

	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex = FlashIndexGrayCode;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex++;

	const int IndexCountAB=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCountAB;	
	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//third
	FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1_2;
	PatItem.nExposure = PatExposure1_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//forth
	FlashIndex = FlashIndexGrayCode;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2_2;
	PatItem.nExposure = PatExposure2_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex++;

	const int IndexCountCD=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCountCD;
	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X5GC_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X5GCM2
{
	return ReturnNotSupportFunc(_T("CLight3DTiDLP4710::BuildDLPPatternList_4X5GC_M2"));	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X6GC_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X6GCM2
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const double Exp2Ratio=GetSecondExpRatio();	
	int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
	int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
	int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
	int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }

	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex=FlashIndexGrayCode;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	const int IndexCountAB=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCountAB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//Third
	FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1_2;
	PatItem.nExposure = PatExposure1_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//forth			
	FlashIndex=FlashIndexGrayCode;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2_2;
	PatItem.nExposure = PatExposure2_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	const int IndexCountCD=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCountCD;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X2_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-42M2
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;	

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const double Exp2Ratio=GetSecondExpRatio();
	const bool   Use3BitPattern=GetUse3BitPattern();
	int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
	int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
	int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
	int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	
	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8	
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	/*
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	*/
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex++;

	//增加以下樣板, 會讓取像間隔拉大至20 ms
	//third
	FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1_2;
	PatItem.nExposure = PatExposure1_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//forth	
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2_2;
	PatItem.nExposure = PatExposure2_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	/*
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	*/

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildDLPPatternList_4X4_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-44M2
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const double Exp2Ratio=GetSecondExpRatio();	
	int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
	int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
	int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
	int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sColorIndex = DLPLEDColor;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sInvertPattern = false;
	PatItem.sInsertBlack = true;
	PatItem.sTrigOutPrev = false;
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1;
	PatItem.nExposure = PatExposure1;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex++;

	//增加以下樣板, 會讓取像間隔拉大至20 ms
	//third
	FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1_2;
	PatItem.nExposure = PatExposure1_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//forth
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2_2;
	PatItem.nExposure = PatExposure2_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::AddDLPPPatItem(TDLPPatItem &PatItem)//增加樣板項目
{
	AOIDataDefine.CalcDLPBitPosRange(PatItem.sBitDepth, PatItem.sBitNum, PatItem.sBitStart, PatItem.sBitEnd);
	m_PatternList.push_back(PatItem);	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::RemoveDLPPatItem(size_t index)//移除樣板項目
{
	const size_t count = this->m_PatternList.size();
	if ( index >= count ) { return; }

	SaveDLPProcess(_T("RemoveDLPPatItem"), MSG_LEVEL_HIGH);

	size_t i = 0;
	std::vector<TDLPPatItem>  PatternList = m_PatternList;//偵錯用
	m_PatternList.clear();
	for ( i=0; i<count; i++ )
	{
		if ( i == index ) { continue; }
		m_PatternList.push_back(PatternList[i]);
	}
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildPatternImage(int BitDepth, int NPeriod, int NPixelPeriod, bool bVer, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage)
{
	//G-8bit R-8bit B-8bit
	const char fnName[]="CLight3DTiDLP4710::BuildPatternImage";
	const int TotalBit = 24;//24bit
	if ( (BitDepth*NPeriod) > TotalBit )
	{
		this->m_ErrorString.Format(_T("Error, TiDLP Pattern Bit Depth and Period out of range(%d)"), TotalBit);
		return false;
	}
	int   val[64]={0};
	int   TotalVal=0;
	int   PaddingShift = 0;
	const size_t MaxPixelPeriod = 128;	
	unsigned char PatternBuffer[24][MaxPixelPeriod]={0};
	const double Phase_DegUnit = 360.0/NPixelPeriod;//每個像素為度
	const size_t PixelMaxBit = 8;
	const int    PixelShift = PixelMaxBit-BitDepth;
	const size_t BitRange = 256;
	const double MaxGraydbl = (double)(BitRange-1);

	if ( (TotalBit%BitDepth) != 0 ) 
	{	PaddingShift = 1; }

	size_t i=0, j=0, k=0, idx=0;
	int    Gray=0;
	int    PatternIdx=0;
	
	double GrayDbl=0;
	double Phase_Deg=0;
	double Phase_Rad=0;	
	unsigned char r=0, g=0, b=0;
	
	ImageW = this->GetDLPImageW();
	ImageH = this->GetDLPImageH();
	IMAGE_SIZE BitCount = TotalBit;
	ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	JetMemory.free_func(pImage);
	if ( JetMemory.alloc_func(BufferSize, pImage, fnName, "pImage") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false;
	}
  
	if ( 3 == NPeriod)//0, 120, 240,
	{
		for ( i=0; i<NPixelPeriod; i++ )
		{
			//0
			Phase_Deg = (i*Phase_DegUnit);
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[0][i] = static_cast<unsigned char>(Gray);

			//120
			Phase_Deg = (i*Phase_DegUnit)+120.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[1][i] = static_cast<unsigned char>(Gray);

			//240
			Phase_Deg = (i*Phase_DegUnit)+240.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[2][i] = static_cast<unsigned char>(Gray);
		}
		
		if ( true == bVer )
		{
			for ( i=0; i<ImageH; i++ )
			{
				idx = i*ImageStep;
				for ( j=0; j<ImageW; j++ )
				{
					PatternIdx = j%NPixelPeriod;
				
					val[0] = PatternBuffer[0][PatternIdx];//000
					val[1] = PatternBuffer[1][PatternIdx];//120
					val[2] = PatternBuffer[2][PatternIdx];//240

					if ( PixelShift > 0 ) //重新取樣, 將8bit取成?bit
					{
						val[0] = val[0] >> PixelShift;
						val[1] = val[1] >> PixelShift;
						val[2] = val[2] >> PixelShift;
					}

					if ( PaddingShift > 0 ) //不整除時會提高單一位元
					{
						val[0] = val[0] << PaddingShift;
						val[1] = val[1] << PaddingShift;
						val[2] = val[2] << PaddingShift;
					}

					//合成一個大數值-Int				
					val[10] = val[0];
					val[11] = val[1] << ((BitDepth+PaddingShift));
					val[12] = val[2] << ((BitDepth+PaddingShift)*2);

					TotalVal = val[10]+val[11]+val[12];

					//轉除成G, R , B
					val[20] = TotalVal&0xff0000;
					val[21] = TotalVal&0x00ff00;
					val[22] = TotalVal&0x0000ff;

					val[30] = val[20]>>16;
					val[31] = val[21]>>8;
					val[32] = val[22];

					b = static_cast<unsigned char>(val[30]);
					r = static_cast<unsigned char>(val[31]);
					g = static_cast<unsigned char>(val[32]);

					pImage[idx] = b; idx++;
					pImage[idx] = g; idx++;
					pImage[idx] = r; idx++;
				}
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				idx = i*ImageStep;
				for ( j=0; j<ImageW; j++ )
				{
					PatternIdx = i%NPixelPeriod;
				
					val[0] = PatternBuffer[0][PatternIdx];//000
					val[1] = PatternBuffer[1][PatternIdx];//120
					val[2] = PatternBuffer[2][PatternIdx];//240

					if ( PixelShift > 0 ) //重新取樣, 將8bit取成?bit
					{
						val[0] = val[0] >> PixelShift;
						val[1] = val[1] >> PixelShift;
						val[2] = val[2] >> PixelShift;
					}

					if ( PaddingShift > 0 ) //不整除時會提高單一位元
					{
						val[0] = val[0] << PaddingShift;
						val[1] = val[1] << PaddingShift;
						val[2] = val[2] << PaddingShift;
					}

					//合成一個大數值-Int				
					val[10] = val[0];
					val[11] = val[1] << ((BitDepth+PaddingShift));
					val[12] = val[2] << ((BitDepth+PaddingShift)*2);

					TotalVal = val[10]+val[11]+val[12];

					//轉除成G, R , B
					val[20] = TotalVal&0xff0000;
					val[21] = TotalVal&0x00ff00;
					val[22] = TotalVal&0x0000ff;

					val[30] = val[20]>>16;
					val[31] = val[21]>>8;
					val[32] = val[22];

					b = static_cast<unsigned char>(val[30]);
					r = static_cast<unsigned char>(val[31]);
					g = static_cast<unsigned char>(val[32]);

					pImage[idx] = b; idx++;
					pImage[idx] = g; idx++;
					pImage[idx] = r; idx++;
				}
			}
		}
	}
	else if ( 4 == NPeriod )//0, 90, 180, 240
	{
		for ( i=0; i<NPixelPeriod; i++ )
		{
			//000
			Phase_Deg = (i*Phase_DegUnit);
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[0][i] = static_cast<unsigned char>(Gray);

			//090
			Phase_Deg = (i*Phase_DegUnit)+90.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[1][i] = static_cast<unsigned char>(Gray);

			//180
			Phase_Deg = (i*Phase_DegUnit)+180.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[2][i] = static_cast<unsigned char>(Gray);

			//270
			Phase_Deg = (i*Phase_DegUnit)+270.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[3][i] = static_cast<unsigned char>(Gray);
		}

		if ( true == bVer )
		{
			for ( i=0; i<ImageH; i++ )
			{
				idx = i*ImageStep;
				for ( j=0; j<ImageW; j++ )
				{				
					PatternIdx = j%NPixelPeriod;
				
					val[0] = PatternBuffer[0][PatternIdx];//000				
					val[1] = PatternBuffer[1][PatternIdx];//090
					val[2] = PatternBuffer[2][PatternIdx];//180				
					val[3] = PatternBuffer[3][PatternIdx];//270

					if ( PixelShift > 0 ) //重新取樣, 將8bit取成?bit
					{
						val[0] = val[0] >> PixelShift;
						val[1] = val[1] >> PixelShift;
						val[2] = val[2] >> PixelShift;
						val[3] = val[3] >> PixelShift;
					}

					if ( PaddingShift > 0 ) //不整除時會提高單一位元
					{
						val[0] = val[0] << PaddingShift;
						val[1] = val[1] << PaddingShift;
						val[2] = val[2] << PaddingShift;
						val[3] = val[3] << PaddingShift;
					}

					//合成一個大數值-Int
					val[10] = val[0];
					val[11] = val[1] << (BitDepth+PaddingShift);
					val[12] = val[2] << ((BitDepth+PaddingShift)*2);
					val[13] = val[3] << ((BitDepth+PaddingShift)*3);

					TotalVal = val[10]+val[11]+val[12]+val[13];

					//轉除成G, R , B
					val[20] = TotalVal&0xff0000;
					val[21] = TotalVal&0x00ff00;
					val[22] = TotalVal&0x0000ff;

					val[30] = val[20]>>16;
					val[31] = val[21]>>8;
					val[32] = val[22];

					b = static_cast<unsigned char>(val[30]);
					r = static_cast<unsigned char>(val[31]);
					g = static_cast<unsigned char>(val[32]);

					pImage[idx] = b; idx++;
					pImage[idx] = g; idx++;
					pImage[idx] = r; idx++;
				}
			}		
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				idx = i*ImageStep;
				for ( j=0; j<ImageW; j++ )
				{				
					PatternIdx = i%NPixelPeriod;
				
					val[0] = PatternBuffer[0][PatternIdx];//000				
					val[1] = PatternBuffer[1][PatternIdx];//090
					val[2] = PatternBuffer[2][PatternIdx];//180				
					val[3] = PatternBuffer[3][PatternIdx];//270

					if ( PixelShift > 0 ) //重新取樣, 將8bit取成?bit
					{
						val[0] = val[0] >> PixelShift;
						val[1] = val[1] >> PixelShift;
						val[2] = val[2] >> PixelShift;
						val[3] = val[3] >> PixelShift;
					}

					if ( PaddingShift > 0 ) //不整除時會提高單一位元
					{
						val[0] = val[0] << PaddingShift;
						val[1] = val[1] << PaddingShift;
						val[2] = val[2] << PaddingShift;
						val[3] = val[3] << PaddingShift;
					}

					//合成一個大數值-Int
					val[10] = val[0];
					val[11] = val[1] << (BitDepth+PaddingShift);
					val[12] = val[2] << ((BitDepth+PaddingShift)*2);
					val[13] = val[3] << ((BitDepth+PaddingShift)*3);

					TotalVal = val[10]+val[11]+val[12]+val[13];

					//轉除成G, R , B
					val[20] = TotalVal&0xff0000;
					val[21] = TotalVal&0x00ff00;
					val[22] = TotalVal&0x0000ff;

					val[30] = val[20]>>16;
					val[31] = val[21]>>8;
					val[32] = val[22];

					b = static_cast<unsigned char>(val[30]);
					r = static_cast<unsigned char>(val[31]);
					g = static_cast<unsigned char>(val[32]);

					pImage[idx] = b; idx++;
					pImage[idx] = g; idx++;
					pImage[idx] = r; idx++;
				}
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseZeroWhite()//確認平面相位
{
	if ( DLP_LED_COLOR_WHITE != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL[White]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseZeroDebug()//確認平面相位
{
	if ( DLP_LED_COLOR_DEBUG != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseZeroRed()//確認平面相位
{
	if ( DLP_LED_COLOR_RED != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL[Red]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseZeroGrn()//確認平面相位
{
	if ( DLP_LED_COLOR_GREEN != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL[Green]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseZeroBlu()//確認平面相位
{
	if ( DLP_LED_COLOR_BLUE != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL[Blue]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4710::GetDLPPhaseZeroBinShortName(int LedClr)//取得相平面的校正短名
{
	CString ShortName;	
	CString KeyName=_T("TiDLPZero");	
	CString ProjectName = GetDLPProjectName();
	CString ColorName = GetDLPLedColorTextForBinFile(LedClr);
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);

#ifdef TB_SYSTEM_ONLY_BOT
	ShortName.Format(_T("%s%s_%s_%s_Bot.BIN"), KeyName, PhaseMode, ProjectName, ColorName);
#else
	ShortName.Format(_T("%s%s_%s_%s.BIN"), KeyName, PhaseMode, ProjectName, ColorName);		
#endif//TB_SYSTEM_ONLY_BOT
	return ShortName;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4710::GetDLPPhaseFactorBinShortName(int LedClr)//取相位比例的校正短名
{
	CString ShortName;
	CString KeyName=_T("TiDLPFactor");	
	CString ProjectName = GetDLPProjectName();
	CString ColorName = GetDLPLedColorTextForBinFile(LedClr);
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);

#ifdef TB_SYSTEM_ONLY_BOT
	ShortName.Format(_T("%s%s_%s_%s_Bot.BIN"), KeyName, PhaseMode, ProjectName, ColorName);
#else
	ShortName.Format(_T("%s%s_%s_%s.BIN"), KeyName, PhaseMode, ProjectName, ColorName);		
#endif//TB_SYSTEM_ONLY_BOT
	return ShortName;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4710::GetDLPPhaseZeroBinFilename(int LedClr)//取得相平面的校正檔名
{
	CString Filename;	
	CString ShortName=GetDLPPhaseZeroBinShortName(LedClr);		
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);
	return Filename;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4710::GetDLPLedColorTextForBinFile(int Color)//取得燈源模式的文字	
{
	CString ColorText;
	switch ( Color )
	{
	case DLP_LED_COLOR_RED:		ColorText=_T("Red"); break;
	case DLP_LED_COLOR_GREEN:	ColorText=_T("Green"); break;
	case DLP_LED_COLOR_BLUE:	ColorText=_T("Blue"); break;
	case DLP_LED_COLOR_WHITE:	ColorText=_T("White"); break;
	case DLP_LED_COLOR_DEBUG:	ColorText=_T("Debug"); break;
	default:
		ColorText=_T("LED");
		break;
	}
	return ColorText;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4710::GetDLPPhaseModeTextForBinFile(int Mode)//取得相位模式的文字
{
	switch ( Mode )
	{	
	case LIGHT3D_PHASE_2_2_M:
		Mode = LIGHT3D_PHASE_2_2_M;
		break;
	case LIGHT3D_PHASE_4_2_M:
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_4GC_M:
		Mode = LIGHT3D_PHASE_4_4GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_5GC_M:
		Mode = LIGHT3D_PHASE_4_5GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_6GC_M:
		Mode = LIGHT3D_PHASE_4_6GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_2_M_2:
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_4_M_2:
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_4GC_M_2:
		Mode = LIGHT3D_PHASE_4_4GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_5GC_M_2:
		Mode = LIGHT3D_PHASE_4_5GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_6GC_M_2:
		Mode = LIGHT3D_PHASE_4_6GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	}
	return AOIDataDefine.GetDLPPhaseModeText(Mode);
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4710::GetDLPPhaseFactorBinFilename(int LedClr)//取相位比例的校正檔名
{
	CString Filename;	
	CString ShortName=GetDLPPhaseFactorBinShortName(LedClr);	
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);
	return Filename;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseZero()//載入平面相位
{
	bool  IsOK = true;
	const int LEDColor = GetDLPParam().m_LEDColor;
	LockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		IsOK = LoadPhaseZeroRed();
		break;
	case DLP_LED_COLOR_GREEN:
		IsOK = LoadPhaseZeroGrn();
		break;
	case DLP_LED_COLOR_BLUE:
		IsOK = LoadPhaseZeroBlu();
		break;
	case DLP_LED_COLOR_DEBUG:
		IsOK = LoadPhaseZeroDebug();
		break;
	default:
		IsOK = LoadPhaseZeroWhite();
		break;
	}	
	UnlockLight3D();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseZeroRed()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_RED;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);

	ClearPhaseZeroBufferRed();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseZeroGrn()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_GREEN;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);

	ClearPhaseZeroBufferGrn();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseZeroBlu()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_BLUE;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);

	ClearPhaseZeroBufferBlu();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseZeroWhite()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_WHITE;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);

	ClearPhaseZeroBufferWhite();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseZeroDebug()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_DEBUG;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);

	ClearPhaseZeroBufferDebug();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		//if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr)//載入DLP平面相位
{
#ifndef PHASE_CTRL_DISABLE
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP4710::LoadDLPPhaseZeroFile";
	bool    IsOK = true;		
	IMAGE_SIZE   TempW = 0;
	IMAGE_SIZE   TempH = 0;
	IMAGE_SIZE   TempStep = 0;
	PHASE_PTR    TempPhase = NULL;
	size_t  NReads = 0;
	
	FILE   *pFile = NULL;
	CString Filename = pfilename;

	pFile = ::_tfopen(Filename, _T("rb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return true;
	}
	NReads = ::fread(&TempW, sizeof(TempW), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(&TempH, sizeof(TempH), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	NReads = ::fread(&TempStep, sizeof(TempStep), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( 0 == BufferSize ) 
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BuferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	
	if ( JetMemory.alloc_func(BufferSize, TempPhase, fnName, "m_PhaseZeroPtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(TempPhase, sizeof(PHASE_DATA), BufferSize, pFile);
	if ( BufferSize != NReads )
	{
		JetMemory.free_func(TempPhase);
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);
	pFile = NULL;

	PhaseW = TempW;
	PhaseH = TempH;
	PhaseStep = TempStep;
	PhasePtr = TempPhase;
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadDLPPhaseZeroFile2(int LedClr, LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr)//載入DLP平面相位
{
#ifdef MUST_BACKUP_SYSTEM_FILE_USE
	CString Filename2;
	CString Filename = pfilename;
	CString ShortName=GetDLPPhaseZeroBinShortName(LedClr);		
	Filename2.Format(_T("%s\\%s"), AOIDataCollect.GetSystemBackupFolder(), ShortName);
	if ( JetAPI::IsFileExist(Filename2) == false )
	{	return false; }		
	::DeleteFile(Filename);
	JetMemory.free_func(PhasePtr);
	::CopyFile(Filename2, Filename, FALSE);
	if ( LoadDLPPhaseZeroFile(Filename, PhaseW, PhaseH, PhaseStep, PhasePtr) == false ) 
	{	return false; }
	return true;
#endif//MUST_BACKUP_SYSTEM_FILE_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseZero()//儲存平面相位
{
	bool IsOK = true;
	if ( SavePhaseZeroRed() == false )
	{	IsOK = false;	}
	if ( SavePhaseZeroGrn() == false )
	{	IsOK = false;	}
	if ( SavePhaseZeroBlu() == false )
	{	IsOK = false;	}
	if ( SavePhaseZeroWhite() == false )
	{	IsOK = false;	}
	if ( SavePhaseZeroDebug() == false )
	{	IsOK = false;	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseZeroRed()//儲存平面相位
{
	if ( CheckPhaseZeroRed() == false )
	{	return true; }	

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);	
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseZeroGrn()//儲存平面相位
{
	if ( CheckPhaseZeroGrn() == false )
	{	return true; }

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseZeroBlu()//儲存平面相位
{
	if ( CheckPhaseZeroBlu() == false )
	{	return true; }

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseZeroWhite()//儲存平面相位
{
	if ( CheckPhaseZeroWhite() == false )
	{	return true; }

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseZeroDebug()//儲存平面相位
{
	if ( CheckPhaseZeroDebug() == false )
	{	return true; }

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLPPhaseZeroData(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr)//確認DLP平面相位資料
{
	if ( 0==PhaseW || 0==PhaseH || 0==PhaseStep )
	{
		m_ErrorString.Format(_T("Error, DLP Phase Zero Data Exception(W=%d, H=%d, Step=%d)"), PhaseW, PhaseH, PhaseStep);
		return false;	
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SaveDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr)//儲存DLP平面相位
{
#ifndef PHASE_CTRL_DISABLE
#ifndef OFFLINE_VERSION
	if ( NULL == pfilename ) { return false; }
	if ( CheckDLPPhaseZeroData(PhaseW, PhaseH, PhaseStep, PhasePtr) == false )
	{	return false; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename = pfilename;

	IMAGE_SIZE PhaseZeroW = PhaseW;
	IMAGE_SIZE PhaseZeroH = PhaseH;
	IMAGE_SIZE PhaseZeroStep = PhaseStep;
	const PHASE_PTR  PhaseZeroPtr = PhasePtr;
	if ( NULL == PhaseZeroPtr ) 
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroPtr"));
		return false;
	}

	pFile = ::_tfopen(Filename, _T("wb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return false;
	}
	NWrites = ::fwrite(&PhaseZeroW, sizeof(PhaseZeroW), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NWrites = ::fwrite(&PhaseZeroH, sizeof(PhaseZeroH), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NWrites = ::fwrite(&PhaseZeroStep, sizeof(PhaseZeroStep), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	const size_t BufferSize = PhaseZeroH*PhaseZeroStep;
	NWrites = ::fwrite(PhaseZeroPtr, sizeof(PHASE_DATA), BufferSize, pFile);
	if ( BufferSize != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);
	pFile = NULL;
#endif//OFFLINE_VERSION
#endif//PHASE_CTRL_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseZeroBuffer()//清除平面相位
{
	ClearPhaseZeroBufferRed();
	ClearPhaseZeroBufferGrn();
	ClearPhaseZeroBufferBlu();
	ClearPhaseZeroBufferWhite();
	ClearPhaseZeroBufferDebug();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseZeroBufferRed()//清除平面相位	
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseZeroBufferGrn()//清除平面相位	
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseZeroBufferBlu()//清除平面相位	
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseZeroBufferWhite()//清除平面相位	
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseZeroBufferDebug()//清除平面相位
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep)//確認DLP平面相位檔案
{
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP4710::CheckDLPPhaseZeroFile";
	bool    IsOK = true;		
	IMAGE_SIZE   TempW = 0;
	IMAGE_SIZE   TempH = 0;
	IMAGE_SIZE   TempStep = 0;
	PHASE_PTR    TempPhase = NULL;
	size_t  NReads = 0;
	
	FILE   *pFile = NULL;
	CString Filename = pfilename;

	pFile = ::_tfopen(Filename, _T("rb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return true;
	}
	NReads = ::fread(&TempW, sizeof(TempW), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(&TempH, sizeof(TempH), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	NReads = ::fread(&TempStep, sizeof(TempStep), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( 0 == BufferSize ) 
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BuferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);	pFile = NULL;

	if ( PhaseW!=TempW || PhaseH!=TempH || PhaseStep!=TempStep )
	{
		this->m_ErrorString.Format(_T("Error, Check Bin Data Size Fault(%s::W:%d, H:%d, Step:%d)"), Filename, TempW, TempH, TempStep);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr)//設定平面相位
{
	const char fnName[] = "CLight3DTiDLP4710::SetPhaseZero";	
	if ( NULL == Ptr ) { return false; }

	PHASE_PTR PhasePtr=NULL;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageSetp, ImageH);
	if ( JetMemory.alloc_func(BufferSize, PhasePtr, fnName, "PhasePtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}	
	::memcpy(PhasePtr, Ptr, sizeof(PHASE_DATA)*BufferSize);
	
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		ClearPhaseZeroBufferRed();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_GREEN:
		ClearPhaseZeroBufferGrn();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_BLUE:
		ClearPhaseZeroBufferBlu();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_DEBUG:
		ClearPhaseZeroBufferDebug();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;		
	default:
		ClearPhaseZeroBufferWhite();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;
	}
	m_PhaseZeroPhaseMode = PhaseMode;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)//取得平面相位
{
	bool IsOK = true;
	bool ReloadFile = false;	 
	LockLight3D();
	if ( PhaseMode!=m_PhaseZeroPhaseMode || m_PhaseZeroLEDColor!=LEDColor || NULL==m_PhaseZeroPtr )
	{
		m_PhaseZeroPhaseMode = PhaseMode;
		ReloadFile = true;
	}	
	if ( true == ReloadFile )
	{	
		switch ( LEDColor )
		{
		case DLP_LED_COLOR_RED:		IsOK=LoadPhaseZeroRed();	break;
		case DLP_LED_COLOR_GREEN:	IsOK=LoadPhaseZeroGrn();	break;
		case DLP_LED_COLOR_BLUE:	IsOK=LoadPhaseZeroBlu();	break;
		case DLP_LED_COLOR_DEBUG:	IsOK=LoadPhaseZeroDebug();	break;
		default:					IsOK=LoadPhaseZeroWhite();	break;
		}		
		if ( false == IsOK )
		{
			UnlockLight3D();
			return false; 
		}
	}
	UnlockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	case DLP_LED_COLOR_GREEN:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	case DLP_LED_COLOR_BLUE:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	case DLP_LED_COLOR_DEBUG:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	default:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClonePhaseZero(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)//複製平面相位
{
	IMAGE_SIZE PhaseZeroW=0;
	IMAGE_SIZE PhaseZeroH=0;
	IMAGE_SIZE PhaseZeroStep=0;
	PHASE_PTR  PhaseZeroPtr=NULL;
	const int PhaseMode = m_PhaseZeroPhaseMode;		
	if ( GetPhaseZero(PhaseMode, LEDColor, PhaseZeroW, PhaseZeroH, PhaseZeroStep, PhaseZeroPtr) == false )
	{	return false; }

	JetMemory.free_func(Ptr);
	const char fnName[] = "CLight3DTiDLP4710::ClonePhaseZero";	
	const size_t BufferSize = ImageAPI.CalcBufferSize(PhaseZeroStep, PhaseZeroH);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}

	::memcpy(Ptr, PhaseZeroPtr, sizeof(PHASE_DATA)*BufferSize);
	ImageW = PhaseZeroW;
	ImageH = PhaseZeroH;
	ImageSetp = PhaseZeroStep;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseFactorRed()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_RED!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Red]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseFactorGrn()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_GREEN!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Green]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseFactorBlu()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_BLUE!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Blue]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseFactorWhite()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_WHITE!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[White]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckPhaseFactorDebug()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_DEBUG!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Debug]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseFactor()//載入平面係數
{	
	bool  IsOK = true;
	const int LEDColor = GetDLPParam().m_LEDColor;
	LockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		IsOK = LoadPhaseFactorRed();
		break;
	case DLP_LED_COLOR_GREEN:
		IsOK = LoadPhaseFactorGrn();
		break;
	case DLP_LED_COLOR_BLUE:
		IsOK = LoadPhaseFactorBlu();
		break;
	case DLP_LED_COLOR_DEBUG:
		IsOK = LoadPhaseFactorDebug();
		break;
	default:
		IsOK = LoadPhaseFactorWhite();
		break;
	}
	UnlockLight3D();
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseFactorRed()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_RED;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);		

	ClearPhaseFactorBufferRed();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseFactorGrn()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_GREEN;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);		

	ClearPhaseFactorBufferGrn();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseFactorBlu()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_BLUE;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);		

	ClearPhaseFactorBufferBlu();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseFactorWhite()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_WHITE;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);		

	ClearPhaseFactorBufferWhite();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadPhaseFactorDebug()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_DEBUG;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);		

	ClearPhaseFactorBufferDebug();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		//if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr)//載入DLP平面係數
{
#ifndef PHASE_CTRL_DISABLE
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP4710::LoadDLPPhaseFactorFile";
	bool    IsOK = true;		
	IMAGE_SIZE   TempW = 0;
	IMAGE_SIZE   TempH = 0;
	IMAGE_SIZE   TempStep = 0;
	SPACE_PTR    TempSpace = NULL;
	size_t   NReads = 0;	
	
	FILE   *pFile = NULL;
	CString Filename = pfilename;

	pFile = ::_tfopen(Filename, _T("rb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return true;
	}
	NReads = ::fread(&TempW, sizeof(TempW), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(&TempH, sizeof(TempH), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	NReads = ::fread(&TempStep, sizeof(TempStep), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( 0 == BufferSize ) 
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BufferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	if ( JetMemory.alloc_func(BufferSize, TempSpace, fnName, "m_PhaseFactorPtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(TempSpace, sizeof(SPACE_DATA), BufferSize, pFile);
	if ( BufferSize != NReads )
	{
		JetMemory.free_func(TempSpace);
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);
	pFile = NULL;
	SpaceW = TempW;
	SpaceH = TempH;
	SpaceStep = TempStep;	
	SpacePtr = TempSpace;
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadDLPPhaseFactorFile2(int LedClr, LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr)//載入DLP平面係數
{
#ifdef MUST_BACKUP_SYSTEM_FILE_USE
	CString Filename2;
	CString Filename = pfilename;
	CString ShortName=GetDLPPhaseFactorBinShortName(LedClr);		
	Filename2.Format(_T("%s\\%s"), AOIDataCollect.GetSystemBackupFolder(), ShortName);
	if ( JetAPI::IsFileExist(Filename2) == false )
	{	return false; }	
	::DeleteFile(Filename);
	JetMemory.free_func(SpacePtr);
	::CopyFile(Filename2, Filename, FALSE);
	if ( LoadDLPPhaseFactorFile(Filename, SpaceW, SpaceH, SpaceStep, SpacePtr) == false ) 
	{	return false; }
	return true;
#endif//MUST_BACKUP_SYSTEM_FILE_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseFactor()//儲存平面係數
{
	bool IsOK = true;
	if ( SavePhaseFactorRed() == false )
	{	IsOK = false;	}
	if ( SavePhaseFactorGrn() == false )
	{	IsOK = false;	}
	if ( SavePhaseFactorBlu() == false )
	{	IsOK = false;	}
	if ( SavePhaseFactorWhite() == false )
	{	IsOK = false;	}
	if ( SavePhaseFactorDebug() == false )
	{	IsOK = false;	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseFactorRed()//儲存平面係數
{
	if ( CheckPhaseFactorRed() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseFactorGrn()//儲存平面係數
{
	if ( CheckPhaseFactorGrn() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseFactorBlu()//儲存平面係數
{
	if ( CheckPhaseFactorBlu() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseFactorWhite()//儲存平面係數
{
	if ( CheckPhaseFactorWhite() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SavePhaseFactorDebug()//儲存平面係數
{
	if ( CheckPhaseFactorDebug() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLPPhaseFactorData(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr)//確認DLP平面係數
{
	if ( 0==SpaceW || 0==SpaceH || 0==SpaceStep )
	{
		m_ErrorString.Format(_T("Error, DLP Phase Factor Data Exception(W=%d, H=%d, Step=%d)"), SpaceW, SpaceH, SpaceStep);
		return false;	
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SaveDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr)//儲存DLP平面係數
{
#ifndef PHASE_CTRL_DISABLE
#ifndef OFFLINE_VERSION
	if ( NULL == pfilename ) { return false; }
	if ( CheckDLPPhaseFactorData(SpaceW, SpaceH, SpaceStep, SpacePtr) == false )
	{	return false; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename = pfilename;	

	IMAGE_SIZE PhaseFactorW = SpaceW;
	IMAGE_SIZE PhaseFactorH = SpaceH;
	IMAGE_SIZE PhaseFactorStep = SpaceStep;
	const SPACE_PTR  PhaseFactorPtr = SpacePtr;

	if ( NULL == PhaseFactorPtr )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorPtr"));
		return false;		
	};	

	pFile = ::_tfopen(Filename, _T("wb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return false;
	}
	NWrites = ::fwrite(&PhaseFactorW, sizeof(PhaseFactorW), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NWrites = ::fwrite(&PhaseFactorH, sizeof(PhaseFactorH), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NWrites = ::fwrite(&PhaseFactorStep, sizeof(PhaseFactorStep), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = PhaseFactorH*PhaseFactorStep;
	NWrites = ::fwrite(PhaseFactorPtr, sizeof(SPACE_DATA), BufferSize, pFile);
	if ( BufferSize != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}		
	::fclose(pFile);
	pFile = NULL;
#endif//OFFLINE_VERSION
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseFactorBuffer()//清除平面係數
{
	ClearPhaseFactorBufferRed();
	ClearPhaseFactorBufferGrn();
	ClearPhaseFactorBufferBlu();
	ClearPhaseFactorBufferWhite();
	ClearPhaseFactorBufferDebug();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseFactorBufferRed()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseFactorBufferGrn()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseFactorBufferBlu()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseFactorBufferWhite()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClearPhaseFactorBufferDebug()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CheckDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep)//確認DLP平面係數檔案
{
#ifndef PHASE_CTRL_DISABLE
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP4710::CheckDLPPhaseFactorFile";
	bool    IsOK = true;		
	IMAGE_SIZE   TempW = 0;
	IMAGE_SIZE   TempH = 0;
	IMAGE_SIZE   TempStep = 0;
	SPACE_PTR    TempSpace = NULL;
	size_t   NReads = 0;	
	
	FILE   *pFile = NULL;
	CString Filename = pfilename;

	pFile = ::_tfopen(Filename, _T("rb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return true;
	}
	NReads = ::fread(&TempW, sizeof(TempW), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(&TempH, sizeof(TempH), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	NReads = ::fread(&TempStep, sizeof(TempStep), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( 0 == BufferSize ) 
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BufferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);	pFile = NULL;
	
	if ( PhaseW!=TempW || PhaseH!=TempH || PhaseStep!=TempStep )
	{		
		this->m_ErrorString.Format(_T("Error, Check Bin Data Size Fault(%s::W:%d, H:%d, Step:%d)"), Filename, TempW, TempH, TempStep);
		return false;
	}
#endif//PHASE_CTRL_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP4710::GetPhaseFactorMin() const//取得平面係數下限
{
	return m_DLPParam.m_PhaseFactorMin;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPhaseFactorMin(double val)//設定平面係數下限
{
	m_DLPParam.m_PhaseFactorMin = val;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP4710::GetPhaseFactorMax() const//取得平面係數上限
{
	return m_DLPParam.m_PhaseFactorMax;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::SetPhaseFactorMax(double val)//設定平面係數上限
{
	m_DLPParam.m_PhaseFactorMax = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::GetLEDCurrentMax() const//取得LED電流上限
{
	return m_LEDCurrentMax;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetLEDColorUsed_Red() const//取得LED顏色使用-紅色
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetLEDColorUsed_Grn() const//取得LED顏色使用-綠色
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetLEDColorUsed_Blu() const//取得LED顏色使用-藍色
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::ClearHeightFactor()//清除高度參數T0, T1, T3
{
	::memset(m_HeightFactor0, 0x00, sizeof(m_HeightFactor0));
	::memset(m_HeightFactor1, 0x00, sizeof(m_HeightFactor1));
	::memset(m_HeightFactor2, 0x00, sizeof(m_HeightFactor2));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SortHeightFactorTableList()//排序高度係數列表
{
	std::vector<TPhaseFactorTable> TempFactorTableList=m_FactorTableList;	
	const size_t TempTableCount = TempFactorTableList.size();
	if ( 0==TempTableCount || 1==TempTableCount ) { return true; }	

	size_t   i=0;
	int      TargetNo=0;
	double   TargetHeight=0;
	size_t   TableIndex=0;	
	CSortObj SortObj;
	std::vector<CSortObj> SortList;
	SortObj.SetSortMode(SORT_BY_INT);//For No
	SortObj.SetSortMode(SORT_BY_DBL);//For Height
	for ( i=0; i<TempTableCount; i++ )
	{		
		const TPhaseFactorTable &FactorTable=TempFactorTableList[i];		
		TargetNo = FactorTable.nTargetNo;
		TargetHeight = FactorTable.dHeight;
		SortObj.SetID(i);
		SortObj.SetValueInt(TargetNo);
		SortObj.SetValueDbl(TargetHeight);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount=SortList.size();
	m_FactorTableList.clear();
	for ( i=0; i<SortCount; i++ )
	{		
		SortObj = SortList[i];		
		TableIndex = SortObj.GetID();
		m_FactorTableList.push_back(TempFactorTableList[TableIndex]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SaveHeightFactorTableList()//儲存高度係數列表檔案
{
#ifndef PHASE_CTRL_DISABLE
#ifndef OFFLINE_VERSION
	if ( SortHeightFactorTableList() == false ) { return false; }

	const std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	
	const size_t TableCount = FactorTableList.size();
	if ( 0 == TableCount ) { return true; }

	size_t   i=0;
	CString  folder;
	CString  filename;
	int      FileNo=0;
	const bool bTemp = false;
	LIGHT_3D_CAST_ID CastID=m_Light3DCastID;	
	const int MaxTargetNo = MAX_HEIGHT_TARGET_COUNT;

	folder = AOIDataCollect.GetPhaseFactorFolder(bTemp);
	::CreateDirectory(folder, NULL);
	for ( i=0; i<MaxTargetNo; i++ )
	{
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp);
		::DeleteFile(filename);
	}
	for ( i=0; i<TableCount; i++ )
	{		
		const TPhaseFactorTable &GridTable = FactorTableList[i];		
		const size_t GridCount = GridTable.GridList.size();
		if ( 0 == GridCount ) { continue; }
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp);
		AOIDataCollect.SavePhaseFactorTableFile(filename, GridTable);
	}	
#endif//OFFLINE_VERSION
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::LoadHeightFactorTableList()//載入高度係數列表檔案
{
#ifndef PHASE_CTRL_DISABLE
	int   i=0;
	CString  filename;	
	int      FileNo=0;
	const bool bTemp = false;	
	const int MaxFileNo = MAX_HEIGHT_TARGET_COUNT;
	LIGHT_3D_CAST_ID CastID=m_Light3DCastID;	

	m_FactorTableList.clear();
	for ( i=0; i<MaxFileNo; i++ )
	{
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp);
		if ( JetAPI::IsFileExist(filename) == false ) { continue; }

		TPhaseFactorTable GridTable;
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable);
		if ( GridTable.GridList.size() == 0 ) { continue; }
		if ( 0 == GridTable.nTargetNo ) { continue; }
		m_FactorTableList.push_back(GridTable);
	}	
	if ( SortHeightFactorTableList() == false )
	{	return false; }
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::RestoreHeightFactorTableList()//復原高度係數列表
{
	size_t    i=0, j=0;
	std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	
	size_t TableCount = FactorTableList.size();
	for ( i=0; i<TableCount; i++ )
	{		
		TPhaseFactorTable &Table=FactorTableList[i];
		const size_t GridCount = Table.GridList.size();
		for ( j=0; j<GridCount; j++ )
		{
			TPhaseFactorGrid &Grid=Table.GridList[j];
			Grid.m_PhaseBaseCalc = Grid.m_PhaseBase;
			Grid.m_PhaseTargetCalc = Grid.m_PhaseTarget;
			Grid.m_HeightBaseCalc = Grid.m_HeightBase;
			Grid.m_HeightTargetCalc = Grid.m_HeightTarget;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::ClearHeightFactorTableList(bool bIncludeFiles)//清除高度係數列表
{	
	LIGHT_3D_CAST_ID CastID=m_Light3DCastID;
	m_FactorTableList.clear();		
#ifndef OFFLINE_VERSION
	if ( true == bIncludeFiles )
	{
		int      i=0;
		int      TargetNo=0;
		CString  filename;	
		const bool bTemp = false;		
		const int MaxTargetNo = MAX_HEIGHT_TARGET_COUNT;
		for ( i=0; i<MaxTargetNo; i++ )
		{
			TargetNo = i+1;
			filename = AOIDataCollect.GetPhaseFactorFilename(CastID, TargetNo, bTemp);
			::DeleteFile(filename);
		}
	}
#endif//OFFLINE_VERSION
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::BuildHeightFactorMappingParam(bool bRecv)//建立高度參數T0, T1, T3
{
#ifndef PHASE_CTRL_DISABLE
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		CString filename;
		TPhaseFactorGrid Grid;		
		LIGHT_3D_CAST_ID CastID=m_Light3DCastID;
		if ( LIGHT_3D_CAST_04 == CastID )
		{	CastID = CastID; }
		std::vector<double> T;		
		std::vector<TPhaseFactorGrid> GridList_0000;
		std::vector<std::vector<double>> ParamListArray_Z;
		std::vector<std::vector<TPhaseFactorGrid>> GridListArray;//校正用
		std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	

		double TargetH=0;
		int CountX=0, CountY=0;
		const bool bTemp = false;
		double AveErr=0, MaxErr=0, RatioErr=0, MaxRatioErr=0, MaxErrHeight=0;
		const size_t TableListCount = FactorTableList.size();
		if ( 0 == TableListCount ) { return true; }

		GridList_0000 = FactorTableList[0].GridList;
		for ( size_t i=0; i<GridList_0000.size(); i++ )
		{
			GridList_0000[i].m_Phase=GridList_0000[i].m_PhaseBase=GridList_0000[i].m_PhaseTarget=GridList_0000[i].m_PhaseOffset=GridList_0000[i].m_PhaseBaseCalc=GridList_0000[i].m_PhaseTargetCalc=0;
			GridList_0000[i].m_Height=GridList_0000[i].m_HeightBase=GridList_0000[i].m_HeightTarget=GridList_0000[i].m_HeightOffset=GridList_0000[i].m_HeightBaseCalc=GridList_0000[i].m_HeightTargetCalc=0;			 
		}		
		//Reset Height Factor
		ClearHeightFactor();

		//校正用
		if ( GridList_0000.size() > 0 )	{	GridListArray.push_back(GridList_0000); }//0um		
		for ( int i=0; i<TableListCount; i++ )
		{	GridListArray.push_back(FactorTableList[i].GridList);	}		
		const size_t GridListCount = GridListArray.size();
		if ( 0 == GridListCount )
		{
			CString ProjectName = GetDLPProjectName();
			m_ErrorString.Format(_T("Error, Build Height Factor Grid List [%s]"), ProjectName);
			return false;
		}
		//初始化參數
		for ( int i=0; i<GridListCount; i++ )
		{
			for ( int j=0; j<GridListArray[i].size(); j++ )
			{
				GridListArray[i][j].m_Phase  = GridListArray[i][j].m_PhaseTargetCalc;
				GridListArray[i][j].m_Height = GridListArray[i][j].m_HeightTargetCalc;				
			}
		}

		int  nRecuCnt = 0;
		double MapError=0.0;
		double SumError=0.0;
		double LastError=DBL_MAX-10.0;
		bool bFinish = false;
		bool bFinishAll = false;
		const int nMaxRecuCnt = 32;
		while ( true )
		{
			if ( CalcHeightFactorMappingParam(GridListArray, ParamListArray_Z) == false )
			{	return false; }
			if ( false == bRecv )
			{	break; }
			nRecuCnt ++;
			if ( nRecuCnt >= nMaxRecuCnt )
			{	break; }

			//重新驗證高度
			SumError = 0.0;
			bFinish = true;
			bFinishAll = true;
			std::vector<std::vector<TPhaseFactorGrid>> TempGridListArray=GridListArray;//驗證用
			const size_t TempGridListCount=TempGridListArray.size();
			for ( int i=1; i<TempGridListCount; i++ )//剃除第1個, 階高塊為0
			{
				VerifyHeightFactorMappingParam(ParamListArray_Z, TempGridListArray[i], MapError);
				SumError += MapError;
			}			
			if ( GridListCount>1 )
			{	SumError /= (GridListCount-1);	}
			if ( SumError > (LastError-0.1) )
			{	break; }			
			LastError = SumError;
			GridListArray = TempGridListArray;
		};
		
		if ( true == bRecv )
		{
			if ( GridListCount > TableListCount )
			{
				for ( int i=0; i<TableListCount; i++ )
				{	FactorTableList[i].GridList = GridListArray[i+1];	}		
			}
			if ( GridListCount == TableListCount )
			{
				for ( int i=0; i<TableListCount; i++ )
				{	FactorTableList[i].GridList = GridListArray[i];	}		
			}
		}
		const size_t KListCnt_Z = ParamListArray_Z.size();

		if ( KListCnt_Z > 0 )
		{
			const std::vector<double> &Param=ParamListArray_Z[0];			
			const size_t ParamCnt = Param.size();
			const size_t FactorCnt = sizeof(m_HeightFactor0)/sizeof(m_HeightFactor0[0]);
			for ( int i=0; i<ParamCnt; i++ )
			{
				if ( i>= FactorCnt) { break; }
				m_HeightFactor0[i] = Param[i];
			}			
		}
		if ( KListCnt_Z > 1 )
		{
			const std::vector<double> &Param=ParamListArray_Z[1];
			const size_t ParamCnt = Param.size();
			const size_t FactorCnt = sizeof(m_HeightFactor1)/sizeof(m_HeightFactor0[1]);
			for ( int i=0; i<ParamCnt; i++ )
			{
				if ( i>= FactorCnt) { break; }
				m_HeightFactor1[i] = Param[i];
			}			
		}
		if ( KListCnt_Z > 2 )
		{
			const std::vector<double> &Param=ParamListArray_Z[2];
			const size_t ParamCnt = Param.size();
			const size_t FactorCnt = sizeof(m_HeightFactor2)/sizeof(m_HeightFactor2[1]);
			for ( int i=0; i<ParamCnt; i++ )
			{
				if ( i>= FactorCnt) { break; }
				m_HeightFactor2[i] = Param[i];
			}			
		}
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		m_ErrorString = msg_e;
		return false;
	}
	return true;
#else
	m_ErrorString = _T("Error, Cast Build Height Factor Need OpenCV Enabled");
	return false;
#endif//OPENCV_DISABLE
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CalcHeightFactorMappingParam(std::vector<std::vector<TPhaseFactorGrid>> &GridListArray, std::vector<std::vector<double>> &ParamListArray)//計算高度參數T0, T1, T3
{
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		CString filename;		
		LIGHT_3D_CAST_ID CastID=m_Light3DCastID;
		std::vector<double> T;
		const size_t GridListCount = GridListArray.size();
		if ( 0 == GridListCount )
		{
			CString ProjectName = GetDLPProjectName();
			m_ErrorString.Format(_T("Error, Load Height Factor Grid File Fault [%s]"), ProjectName);
			return false;
		}

		//測試同XY下不同相位的高度值
		std::vector<std::vector<double>> KListArray_Z;
		const size_t PArrayCount=GridListArray[0].size();
		std::vector<std::vector<TPhaseFactorGrid>> GridListPArray(PArrayCount);//For MultiLayer Phase
		for ( int i=0; i<GridListCount; i++ )
		{			
			int idx = 0;
			const int PhaseMappingMode=7;//建立相位Mapping Func的參數
			std::vector<double> KList_Phase;	
			const std::vector<TPhaseFactorGrid> &GridList = GridListArray[i];			
			AOIDataCollect.CalcPhaseFactorXYPhaseMappingFunc(PhaseMappingMode, GridList, KList_Phase);		
			const int KCount_Phase=KList_Phase.size();			
			const size_t ListCnt=GridList.size();

			T.resize(KCount_Phase);
			for ( int j=0; j<ListCnt; j++ )
			{				
				idx = j;
				if ( idx >= PArrayCount ) { continue; }
				TPhaseFactorGrid Grid=GridList[idx];
				//使用第1組的XY影像座標
				if ( GridListPArray[idx].size() > 0 ) 
				{
					Grid.m_ImgX = GridListPArray[idx][0].m_ImgX;
					Grid.m_ImgY = GridListPArray[idx][0].m_ImgY;
				}			
				AOIDataCollect.GetHeightFactorXYParam(KCount_Phase, Grid, T);
				Grid.m_Phase = AOIDataCollect.Calc2ParamVecotr(KList_Phase, T);				
				if ( fabs(Grid.m_Phase) > 0.1 )
				{	Grid.m_Factor = Grid.m_Height/Grid.m_Phase; }
				else
				{	Grid.m_Factor = 0.0; }
				GridListPArray[idx].push_back(Grid);
			}
		}

		const size_t GridListPhaseCount = GridListPArray.size();
		if ( 0 == GridListPhaseCount )
		{
			CString ProjectName = GetDLPProjectName();
			m_ErrorString.Format(_T("Error, Build GridListPArray Fault [%s]"), ProjectName);
			return false;
		}

		std::vector<std::vector<double>> ParamListArray_Z;							
		for ( int i=0; i<GridListPhaseCount; i++ )
		{
			if ( GridListPArray[i].size() == 0 ) { continue; }

			int ZMappingMode = 3;//建立高度Mapping Func的參數
			std::vector<double> KList_Z;
			const int ZCount=(int)(GridListPArray[i].size());
			switch ( ZCount )
			{
			case 1:	ZMappingMode = 1;	break;
			case 2:	ZMappingMode = 2;	break;			
			default:
				ZMappingMode = 3;
				break;
			}
			AOIDataCollect.CalcPhaseFactorZMappingFunc(ZMappingMode, GridListPArray[i], KList_Z);		
			KListArray_Z.push_back(KList_Z);
		}

		const size_t KListArrayCount = KListArray_Z.size();
		if ( 0 == KListArrayCount )
		{
			CString ProjectName = GetDLPProjectName();
			m_ErrorString.Format(_T("Error, Build KListArray_Z Fault [%s]"), ProjectName);
			return false;
		}			

		//建立T參數Mapping Func的參數
		const int TMappingMode = 7;//3-1階, 4,5,6-2階, 7,10-3階					
		const size_t KListCnt_Z = KListArray_Z[0].size();
		for ( int i=0; i<KListCnt_Z; i++ )
		{	
			std::vector<TPhaseFactorGrid> ParamList;
			for ( int j=0; j<PArrayCount; j++ )
			{
				int idx = j;						
				TPhaseFactorGrid PhaseFactorGrid=GridListPArray[idx][0];						
				PhaseFactorGrid.m_Height = KListArray_Z[idx][i];
				PhaseFactorGrid.m_Phase = KListArray_Z[idx][i];		
				ParamList.push_back(PhaseFactorGrid);
			}			
			std::vector<double> KList_Param;				
			AOIDataCollect.CalcPhaseFactorXYPhaseMappingFunc(TMappingMode, ParamList, KList_Param);	
			ParamListArray_Z.push_back(KList_Param);
		}
		ParamListArray = ParamListArray_Z;		
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		m_ErrorString = msg_e;
		return false;
	}
	return true;
#else
	m_ErrorString = _T("Error, Cast Build Height Factor Need OpenCV Enabled");
	return false;
#endif//OPENCV_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::VerifyHeightFactorMappingParam(const std::vector<std::vector<double>> &ParamListArray, std::vector<TPhaseFactorGrid> &GridList, double &Error)//驗證高度參數T0, T1, T3
{		
	const int GridCount = (int)(GridList.size());
	const int ParamListCount = (int)(ParamListArray.size());
	if ( 0==GridCount || 0==ParamListCount )
	{	return true; }
	
	TPhaseFactorGrid Grid;		
	double AveError=0.0;	
	double MaxErrorBase=0.0;	
	double HeightBaseAfter=0;
	double HeightBaseBefore=0;	
	double HeightTargetAfter=0;
	for ( int i=0; i<GridCount; i++ )
	{
		Grid = GridList[i];		
		std::vector<double> HeightFactor;
		double PhaseBase   = Grid.m_PhaseBaseCalc;
		double PhaseTarget = Grid.m_PhaseTargetCalc;		
		double HeightOffset= Grid.m_HeightOffset;		
		for ( int j=0; j<ParamListCount; j++ )
		{	
			const std::vector<double> &ParamList = ParamListArray[j];
			const int ParamCount = ParamList.size();
			std::vector<double> T(ParamCount);	
			AOIDataCollect.GetHeightFactorXYParam(ParamCount, Grid, T);
			double FactorParam=AOIDataCollect.Calc2ParamVecotr(ParamList, T);
			HeightFactor.push_back(FactorParam);
		}
		const int HeightFactorCount=(int)(HeightFactor.size());
		std::vector<double> T_Base(HeightFactorCount);
		std::vector<double> T_Target(HeightFactorCount);		
		AOIDataCollect.GetHeightFactorPhaseParam(HEIGHT_FACTOR_PHASE_BASE, HeightFactorCount, Grid, T_Base);
		AOIDataCollect.GetHeightFactorPhaseParam(HEIGHT_FACTOR_PHASE_TARGET, HeightFactorCount, Grid, T_Target);
		double HeightBase   = AOIDataCollect.Calc2ParamVecotr(HeightFactor, T_Base);
		double HeightTarget = AOIDataCollect.Calc2ParamVecotr(HeightFactor, T_Target);
		double ErrorBase = fabs(HeightBase-Grid.m_HeightBase);
		double ErrorTarget = fabs(HeightTarget-Grid.m_HeightTarget);		
		if ( ErrorBase > MaxErrorBase ){ MaxErrorBase = ErrorBase; }
		HeightBaseAfter += HeightBase;		
		HeightTargetAfter += HeightTarget;
		AveError += fabs(HeightTarget-HeightBase-HeightOffset);	
	}
	AveError /= GridCount;	
	HeightBaseAfter /= GridCount;//求得新的平均基準面
	HeightTargetAfter /= GridCount;//求得新的平均階高塊

	HeightBaseBefore=0;//求得之前平均基準面
	for ( int i=0; i<GridCount; i++ )
	{	HeightBaseBefore += GridList[i].m_HeightBaseCalc; }
	HeightBaseBefore /= GridCount;
	
	double HeightBase = HeightBaseAfter;
	double ErrorBase = fabs(HeightBase-HeightBaseBefore);
	double ErrorTarget = fabs(HeightTargetAfter-HeightBaseAfter-Grid.m_HeightOffset);
	//if ( ErrorBase > 5 )
	//if ( ErrorTarget > 1 )
	//if ( AveError > 10 )
	//{	bFinish = false; }
	Error = AveError;

	//修正原始的基準面與階高塊的高度值
	for ( int i=0; i<GridCount; i++ )
	{	
		GridList[i].m_HeightBaseCalc = HeightBase; 
		GridList[i].m_HeightTargetCalc = HeightBase+GridList[i].m_HeightOffset; 
		GridList[i].m_Height = GridList[i].m_HeightTargetCalc;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetHeightFactorMappingParam(double T0[], double T1[], double T2[])//取得高度參數
{
	::memcpy(T0, m_HeightFactor0, sizeof(m_HeightFactor0));
	::memcpy(T1, m_HeightFactor1, sizeof(m_HeightFactor1));
	::memcpy(T2, m_HeightFactor2, sizeof(m_HeightFactor2));		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr)//設定平面係數
{
	const char fnName[] = "CLight3DTiDLP4710::SetPhaseFactor";	
	if ( NULL == Ptr ) { return false; }

	SPACE_PTR    PhaseFactorPtr = NULL;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageSetp, ImageH);
	if ( JetMemory.alloc_func(BufferSize, PhaseFactorPtr, fnName, "m_PhaseFactorPtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}
	::memcpy(PhaseFactorPtr, Ptr, sizeof(SPACE_DATA)*BufferSize);

	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		ClearPhaseFactorBufferRed();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_GREEN:
		ClearPhaseFactorBufferGrn();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_BLUE:
		ClearPhaseFactorBufferBlu();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_DEBUG:
		ClearPhaseFactorBufferDebug();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	default:
		ClearPhaseFactorBufferWhite();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	}
	m_PhaseFactorPhaseMode = PhaseMode;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)//取得平面係數
{
	bool IsOK = true;
	bool ReloadFile = false;
	LockLight3D();
	if ( PhaseMode!=m_PhaseFactorPhaseMode || m_PhaseFactorLEDColor!=LEDColor || NULL==m_PhaseFactorPtr )
	{
		m_PhaseFactorPhaseMode = PhaseMode;
		ReloadFile = true;
	}
	if ( true == ReloadFile )
	{	
		switch ( LEDColor )
		{
		case DLP_LED_COLOR_RED:		IsOK=LoadPhaseFactorRed();	break;
		case DLP_LED_COLOR_GREEN:	IsOK=LoadPhaseFactorGrn();	break;
		case DLP_LED_COLOR_BLUE:	IsOK=LoadPhaseFactorBlu();	break;
		case DLP_LED_COLOR_DEBUG:	IsOK=LoadPhaseFactorDebug();	break;
		default:					IsOK=LoadPhaseFactorWhite();	break;
		}		
		if ( false == IsOK )
		{
			UnlockLight3D();
			return false; 
		}
	}
	UnlockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	case DLP_LED_COLOR_GREEN:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	case DLP_LED_COLOR_BLUE:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	case DLP_LED_COLOR_DEBUG:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	default:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ClonePhaseFactor(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)//複製平面係數
{
	IMAGE_SIZE PhaseFactorW=0;
	IMAGE_SIZE PhaseFactorH=0;
	IMAGE_SIZE PhaseFactorStep=0;
	SPACE_PTR  PhaseFactorPtr=NULL;
	const int PhaseMode = m_PhaseFactorPhaseMode;			
	if ( GetPhaseFactor(PhaseMode, LEDColor, PhaseFactorW, PhaseFactorH, PhaseFactorStep, PhaseFactorPtr) == false )
	{	return false; }

	const char fnName[] = "CLight3DTiDLP4710::ClonePhaseFactor";
	JetMemory.free_func(Ptr);
	const size_t BufferSize = ImageAPI.CalcBufferSize(PhaseFactorStep, PhaseFactorH);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}

	::memcpy(Ptr, PhaseFactorPtr, sizeof(SPACE_DATA)*BufferSize);
	ImageW = PhaseFactorW;
	ImageH = PhaseFactorH;
	ImageSetp = PhaseFactorStep;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetHeightFactorTable(int TargetNo, const TPhaseFactorTable &GridTable)//設定高度係數格點列表	
{
	if ( 0 == TargetNo ) { return true; }
	size_t i=0;
	std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	
	const size_t TableCount = FactorTableList.size();
	for ( i=0; i<TableCount; i++ )
	{
		const TPhaseFactorTable &FactorTable=FactorTableList[i];		
		if ( TargetNo != FactorTable.nTargetNo ) { continue; }									
		FactorTableList[i] = GridTable; 		
		return true;
	}	
	FactorTableList.push_back(GridTable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::CloneHeightFactorTable(int TargetNo, TPhaseFactorTable &GridTable) const//複製高度係數格點列表
{
	if ( 0 == TargetNo ) { return false; }
	size_t i=0;
	const std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	
	const size_t TableCount = FactorTableList.size();
	for ( i=0; i<TableCount; i++ )
	{
		const TPhaseFactorTable &FactorTable=FactorTableList[i];		
		if ( TargetNo != FactorTable.nTargetNo ) { continue; }
		GridTable = FactorTableList[i];
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ResetLEDDisable(bool &ResetFinish, bool ShowMsg)
{
	if( false == m_EnableTemperatureMonitor ) { ResetFinish=true; return true; }	//不使用溫度監控
	return ReturnNotSupportFunc(_T("CLight3DTiDLP4710::ResetLEDDisable"));	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetGPIOStatus(UINT PinNum, bool &Status)//取得GPIO pin 狀態
{
	return ReturnNotSupportFunc(_T("CLight3DTiDLP4710::GetGPIOStatus"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetGPIOStatus(UINT PinNum, bool Status)//取得GPIO pin output 狀態
{
	return ReturnNotSupportFunc(_T("CLight3DTiDLP4710::SetGPIOStatus"));	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetGPIOTemperatureOver(bool &IsOver, bool ShowMsg)//偵測溫度狀態。		GPIO11 input狀態。high高溫/low低溫
{
	//偵測溫度狀態。	GPIO11 input狀態。high高溫/low低溫
	double Temperature=0.0;
	if ( ReadSystemTemperature(&Temperature) == false )
	{	
		SaveDLPCurrentProcess(m_ErrorString, ShowMsg);
		return false; 
	}

	IsOver = false;
	if ( Temperature > 100.0 )
	{
		IsOver = true;
		if( true == m_EnableTemperatureMonitor )
		{			
			m_ErrorString.Format(_T("Error, DLP high temperature[%.2f]"), Temperature);
			SaveDLPCurrentProcess(m_ErrorString, ShowMsg);
		}
	} 
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::GetGPIOLEDDisable(bool &IsDisable, bool ShowMsg)//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除
{	//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除			
	UINT pinNum = 5;
	bool Status = false;	//0=LOW; 1=HIGH
	if ( GetGPIOStatus(pinNum, Status) == false )
	{
		m_ErrorString.Format(_T("Error, Get LED status fault.(GPIO 5)"));
		SaveDLPCurrentProcess(m_ErrorString, ShowMsg);
		return false;
	}
	if( false == Status )
	{ IsDisable = false; }
	else
	{ 
		IsDisable = true; 		
		if( true == m_EnableTemperatureMonitor )
		{	
			m_ErrorString.Format(_T("Error, DLP disable"));
			SaveDLPCurrentProcess(m_ErrorString, ShowMsg);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::SetGPIOLEDEnable()//LED disable狀態解除。	GPIO6  output狀態。 low->hi	
{
	//LED disable狀態解除。	GPIO6  output狀態。 low->hi			
	UINT pinNum = 6;
	if( SetGPIOStatus(pinNum, false) == false ) 
	{ 
		this->m_ErrorString.Format(_T("Error, Set GPIO low fault.(GPIO 6)"));
		SaveDLPCurrentProcess(m_ErrorString, true);
		return false; 
	}
	DLPSettingDelay();
	if( SetGPIOStatus(pinNum, true) == false ) 
	{ 
		this->m_ErrorString.Format(_T("Error, Set GPIO high fault.(GPIO 6)"));
		SaveDLPCurrentProcess(m_ErrorString, true);
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4710::DLPSettingDelay()//DLP設定時要先延遲一段時間
{
	if ( m_DLPDelayTime > 0 )
	{	::Sleep(m_DLPDelayTime); }	
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_4710_USE
//-------------------------------------------------------------------------------------//