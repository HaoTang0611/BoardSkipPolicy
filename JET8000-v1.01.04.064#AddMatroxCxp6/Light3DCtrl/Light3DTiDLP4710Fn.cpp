// Light3DTiDLP4710Fn.cpp: implementation of the CLight3DTiDLP4710 class.
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
bool CLight3DTiDLP4710::ReturnNotSupportFunc(const TCHAR *fnName)
{
	m_ErrorString.Format(_T("Error, ReturnNotSupportFunc [%s]"), fnName);
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ResetDLP4710()
{
#ifdef DLP4710_MCU_USE	
	if ( GetDLPIsConnected() == false )
	{	return false; }
	if ( CheckDLP4710McuIsConnected() == false )
	{	return false; }
	if (false == m_I2C.CYPRESS_I2C_CloseCyI2C()) {
		this->m_ErrorString.Format(_T("%s"), m_I2C.GetErrorString());
		return false;
	}
	char  USB_Number[256]={0};
	uint8_t datatemp[29], Datalength=0;	
	const LIGHT_3D_CAST_ID CastID = GetCastID();
	if( GetUSB_Number(CastID, USB_Number) == false)
	{	return false;	}
	if ( m_I2C.CYPRESS_I2C_ConnectToCyI2C_MCU(USB_Number) == false )
	{
		this->m_ErrorString.Format(_T("Error, TiDLP I2C Connect to MCU Fault"));
		return false;
	}	
	if ( MCU_Write_MCU_I2Ccontrol(3, 10) != SUCCESS )
	{	return false;	}
#endif//DLP4710_MCU_USE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadSystemTemperature(double *Temperature)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadSystemTemperature(Temperature);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadSystemTemperature Fault[%d]"), Status);		
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadShortStatus(DLPC34XX_DUAL_ShortStatus_s *ShortStatus)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadShortStatus(ShortStatus);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadShortStatus Fault[%d]"), Status);		
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadSystemStatus(DLPC34XX_DUAL_SystemStatus_s *SystemStatus)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadSystemStatus(SystemStatus);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadSystemStatus Fault[%d]"), Status);		
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadSystemSoftwareVersion(uint16_t *PatchVersion, uint8_t *MinorVersion, uint8_t *MajorVersion)
{	
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadSystemSoftwareVersion(PatchVersion, MinorVersion, MajorVersion);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadSystemSoftwareVersion Fault[%d]"), Status);		
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadFirmwareBuildVersion(uint16_t *PatchVersion, uint8_t *MinorVersion, uint8_t *MajorVersion)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadFirmwareBuildVersion(PatchVersion, MinorVersion, MajorVersion);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadFirmwareBuildVersion Fault[%d]"), Status);		
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
DLPC34XX_DUAL_OperatingMode_e CLight3DTiDLP4710::MapOperatingModeSelect(int OperatingMode)
{
	DLPC34XX_DUAL_OperatingMode_e Mode=DLPC34XX_DUAL_OM_STANDBY;
	switch ( OperatingMode )
	{
	case DLP_OPERATION_VIDEO: Mode=DLPC34XX_DUAL_OM_EXTERNAL_VIDEO_PORT; break;
	//case DLP_OPERATION_DEFAULT: Mode=DLPC34XX_DUAL_OM_TEST_PATTERN_GENERATOR; break;
	//case DLP_OPERATION_DEFAULT: Mode=DLPC34XX_DUAL_OM_SPLASH_SCREEN; break;
	case DLP_OPERATION_PATTERN: Mode=DLPC34XX_DUAL_OM_SENS_EXTERNAL_PATTERN; break;
	case DLP_OPERATION_PATTERN_EXP: Mode=DLPC34XX_DUAL_OM_SENS_INTERNAL_PATTERN; break;
	//case DLP_OPERATION_DEFAULT: Mode=DLPC34XX_DUAL_OM_SENS_SPLASH_PATTERN; break;
	case DLP_OPERATION_STANDBY: Mode=DLPC34XX_DUAL_OM_STANDBY; break;
	}
	return Mode;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4710::MapOperatingModeSelect(DLPC34XX_DUAL_OperatingMode_e OperatingMode)
{
	int Mode=DLP_OPERATION_DEFAULT;
	switch ( OperatingMode )
	{
	case DLPC34XX_DUAL_OM_EXTERNAL_VIDEO_PORT: Mode=DLP_OPERATION_VIDEO; break;
	//case DLPC34XX_DUAL_OM_TEST_PATTERN_GENERATOR: Mode=DLP_OPERATION_DEFAULT; break;
	case DLPC34XX_DUAL_OM_SPLASH_SCREEN: Mode=DLP_OPERATION_DEFAULT; break;
	case DLPC34XX_DUAL_OM_SENS_EXTERNAL_PATTERN: Mode=DLP_OPERATION_PATTERN; break;
	case DLPC34XX_DUAL_OM_SENS_INTERNAL_PATTERN: Mode=DLP_OPERATION_PATTERN_EXP; break;
	//case DLPC34XX_DUAL_OM_SENS_SPLASH_PATTERN: Mode=DLP_OPERATION_DEFAULT; break;
	case DLPC34XX_DUAL_OM_STANDBY: Mode=DLP_OPERATION_STANDBY; break;
	}
	return Mode;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadOperatingModeSelect(DLPC34XX_DUAL_OperatingMode_e *OperatingMode)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadOperatingModeSelect(OperatingMode);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadOperatingModeSelect Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WriteOperatingModeSelect(DLPC34XX_DUAL_OperatingMode_e OperatingMode)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WriteOperatingModeSelect(OperatingMode);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WriteOperatingModeSelect Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadDisplayImageOrientation(DLPC34XX_DUAL_ImageFlip_e *LongAxisImageFlip, DLPC34XX_DUAL_ImageFlip_e *ShortAxisImageFlip)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadDisplayImageOrientation(LongAxisImageFlip, ShortAxisImageFlip);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadDisplayImageOrientation Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WriteDisplayImageOrientation(DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, DLPC34XX_DUAL_ImageFlip_e ShortAxisImageFlip)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WriteDisplayImageOrientation(LongAxisImageFlip, ShortAxisImageFlip);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WriteDisplayImageOrientation Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadLedOutputControlMethod(DLPC34XX_DUAL_LedControlMethod_e *LedControlMethod)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadLedOutputControlMethod(LedControlMethod);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadLedOutputControlMethod Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WriteLedOutputControlMethod(DLPC34XX_DUAL_LedControlMethod_e LedControlMethod)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WriteLedOutputControlMethod(LedControlMethod);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WriteLedOutputControlMethod Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadRgbLedEnable(bool *RedLedEnable, bool *GreenLedEnable, bool *BlueLedEnable)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadRgbLedEnable(RedLedEnable, GreenLedEnable, BlueLedEnable);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadRgbLedEnable Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WriteRgbLedEnable(bool RedLedEnable, bool GreenLedEnable, bool BlueLedEnable)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WriteRgbLedEnable(RedLedEnable, GreenLedEnable, BlueLedEnable);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WriteRgbLedEnable Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadRgbLedCurrent(uint16_t *RedLedCurrent, uint16_t *GreenLedCurrent, uint16_t *BlueLedCurrent)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadRgbLedCurrent(RedLedCurrent, GreenLedCurrent, BlueLedCurrent);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadRgbLedCurrent Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WriteRgbLedCurrent(uint16_t RedLedCurrent, uint16_t GreenLedCurrent, uint16_t BlueLedCurrent)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WriteRgbLedCurrent(RedLedCurrent, GreenLedCurrent, BlueLedCurrent);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WriteRgbLedCurrent Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WriteTriggerInConfiguration(DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e TriggerPolarity)
{
	if (GetDLPIsConnected() == false) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WriteTriggerInConfiguration(TriggerEnable, TriggerPolarity);
	if (0 != Status)
	{
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WriteTriggerInConfiguration Fault[%d]"), Status);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadTriggerInConfiguration(DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e *TriggerPolarity)
{
	if (GetDLPIsConnected() == false) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadTriggerInConfiguration(TriggerEnable, TriggerPolarity);
	if (0 != Status)
	{
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadTriggerInConfiguration Fault[%d]"), Status);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WritePatternReadyConfiguration(DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e TriggerPolarity)
{
	if (GetDLPIsConnected() == false) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WritePatternReadyConfiguration(TriggerEnable, TriggerPolarity);
	if (0 != Status)
	{
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WritePatternReadyConfiguration Fault[%d]"), Status);
		return false;
	}
	return true;		
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadPatternReadyConfiguration(DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e *TriggerPolarity)
{
	if (GetDLPIsConnected() == false) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadPatternReadyConfiguration(TriggerEnable, TriggerPolarity);
	if (0 != Status)
	{
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadPatternReadyConfiguration Fault[%d]"), Status);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WriteTriggerOutConfiguration(DLPC34XX_DUAL_TriggerType_e TriggerType, DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerInversion_e TriggerInversion, int32_t Delay)
{
	if (GetDLPIsConnected() == false) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WriteTriggerOutConfiguration(TriggerType, TriggerEnable, TriggerInversion, Delay);
	if (0 != Status)
	{
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WriteTriggerOutConfiguration Fault[%d]"), Status);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadTriggerOutConfiguration(DLPC34XX_DUAL_TriggerType_e TriggerType, DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerInversion_e *TriggerInversion, int32_t *Delay)
{
	if (GetDLPIsConnected() == false) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadTriggerOutConfiguration(TriggerType, TriggerEnable, TriggerInversion, Delay);
	if (0 != Status)
	{
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadTriggerOutConfiguration Fault[%d]"), Status);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WriteInternalPatternControl(DLPC34XX_DUAL_PatternControl_e PatternControl, uint8_t RepeatCount)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WriteInternalPatternControl(PatternControl, RepeatCount);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WriteInternalPatternControl Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadPatternOrderTableEntry(uint8_t PatternOrderTableEntryIndex, DLPC34XX_DUAL_PatternOrderTableEntry_s *PatternOrderTableEntry)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadPatternOrderTableEntry(PatternOrderTableEntryIndex, PatternOrderTableEntry);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadPatternOrderTableEntry Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::WritePatternOrderTableEntry(DLPC34XX_DUAL_WriteControl_e WriteControl, DLPC34XX_DUAL_PatternOrderTableEntry_s *PatternOrderTableEntry)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_WritePatternOrderTableEntry(WriteControl, PatternOrderTableEntry);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_WritePatternOrderTableEntry Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4710::ReadValidateExposureTime(DLPC34XX_DUAL_PatternMode_e PatternMode, DLPC34XX_DUAL_SequenceType_e BitDepth, uint32_t ExposureTime, DLPC34XX_DUAL_ValidateExposureTime_s *ValidateExposureTime)
{
	if ( GetDLPIsConnected() == false ) { return false; }
	uint32_t Status = DLPC34XX_DUAL_ReadValidateExposureTime(PatternMode, BitDepth, ExposureTime, ValidateExposureTime);
	if ( 0 != Status )
	{	
		m_ErrorString.Format(_T("Exec DLPC34XX_DUAL_ReadValidateExposureTime Fault[%d]"), Status);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_4710_USE