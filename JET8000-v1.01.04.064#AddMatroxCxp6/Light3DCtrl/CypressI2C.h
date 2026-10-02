// CypressI2C.h: interface for the CCypressI2C class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CYPRESSI2C_H__064B89B6_8E9F_4A72_BE90_4CA80015CDD4__INCLUDED_)
#define AFX_CYPRESSI2C_H__064B89B6_8E9F_4A72_BE90_4CA80015CDD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdbool.h"
#include "stdint.h"
#include "DLPC_API\\cyusbserial\\CyUSBSerial.h"

#define DLP4710_MCU_USE

class CCypressI2C  
{
protected:
	static int                m_RefI2C;
	static CRITICAL_SECTION   m_csI2C;
	bool                      LockI2C();
	bool                      UnlockI2C();

protected:
	char               m_Error[256];
	CY_HANDLE          s_Handle;
	CY_I2C_DATA_CONFIG s_DataConfig;

protected:
	bool GetCyI2CHandle(CY_HANDLE* Handle, char *DeviceSN);	

public:
	CCypressI2C();
	virtual ~CCypressI2C();
	const char* GetErrorString() const;

	bool CheckIsConnected();
	bool GetIsConnected() const;
	

	bool CYPRESS_I2C_RequestI2CBusAccess();
	bool CYPRESS_I2C_RelinquishI2CBusAccess();
	bool CYPRESS_I2C_WriteI2C(uint32_t WriteDataLength, uint8_t* WriteData);
	bool CYPRESS_I2C_ReadI2C(uint32_t ReadDataLength, uint8_t* ReadData);
	bool CYPRESS_I2C_CloseCyI2C();
	bool CYPRESS_I2C_ConnectToCyI2C(char *DeviceSN=NULL);
	bool CYPRESS_I2C_ConnectToCyI2C_MCU(char *DeviceSN=NULL);//20240702
	bool CYPRESS_I2C_ConnectToCyI2C_eeprom(char *DeviceSN=NULL);//20240702
	bool CYPRESS_I2C_GetCyGpio(uint8_t GpioNum, uint8_t* Value);
	bool CYPRESS_I2C_SetCyGpio(uint8_t GpioNum, uint8_t Value);

};
#endif // !defined(AFX_CYPRESSI2C_H__064B89B6_8E9F_4A72_BE90_4CA80015CDD4__INCLUDED_)
