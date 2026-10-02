// CypressI2C.cpp: implementation of the CCypressI2C class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CypressI2C.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

#pragma comment(lib,"Light3DCtrl\\DLPC_API\\cyusbserial\\cyusbserial.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define REQUEST_I2C_ACCESS_GPIO    5
#define I2C_ACCESS_GRANTED_GPIO    6
#define START_I2C_TRANSACTION_GPIO 9
#ifndef DLP4710_MCU_USE
	#define I2C_CLOCK_FREQUENCY_HZ     100000
	#define DLP_I2C_SLAVE_ADDRESS      (0X36 >> 1)
#else
	#define I2C_CLOCK_FREQUENCY_HZ     80000//100000
	#define DLP_I2C_SLAVE_ADDRESS      (0X36 >> 1)
	#define MCU_I2C_SLAVE_ADDRESS      (0X3A >> 1)
	#define EEPROM_I2C_SLAVE_ADDRESS      (0XA8 >> 1)
#endif//DLP4710_MCU_USE
#define I2C_TIMEOUT_MILLISECONDS   500   

int CCypressI2C::m_RefI2C = 0;
CRITICAL_SECTION  CCypressI2C::m_csI2C;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCypressI2C::CCypressI2C()
{	
	if ( 0 == m_RefI2C )
	{	::InitializeCriticalSection(&m_csI2C);	}
	m_RefI2C ++;

	s_Handle = NULL;
	memset(m_Error, 0x00, sizeof(m_Error));	
	::memset(&s_DataConfig, 0x00, sizeof(s_DataConfig));
}

CCypressI2C::~CCypressI2C()
{
	CYPRESS_I2C_CloseCyI2C();

	m_RefI2C --;
	if ( 0 == m_RefI2C )
	{	::DeleteCriticalSection(&m_csI2C);	}
}

bool CCypressI2C::LockI2C()
{
	::EnterCriticalSection(&m_csI2C);
	return true;
}

bool CCypressI2C::UnlockI2C()
{
	::LeaveCriticalSection(&m_csI2C);
	return true;
}

const char* CCypressI2C::GetErrorString() const
{
	return m_Error;
}

bool CCypressI2C::GetCyI2CHandle(CY_HANDLE* Handle, char *DeviceSN)
{
	CY_RETURN_STATUS Status;
    CY_DEVICE_INFO   DeviceInfo;
    uint8_t          NumDevices = 0;
    uint8_t          DeviceIdx;
    uint8_t          InterfaceIdx;
	size_t           szDeviceSN=0;
	if ( NULL != DeviceSN )
	{	szDeviceSN=::strlen(DeviceSN); }

    Status = CyGetListofDevices(&NumDevices);	
    if ((Status != CY_SUCCESS) || (NumDevices == 0))
    {	 return false;	}

    for (DeviceIdx = 0; DeviceIdx < NumDevices; DeviceIdx++)
    {
		::memset(&DeviceInfo, 0x00, sizeof(DeviceInfo));
		LockI2C();
        Status = CyGetDeviceInfo(DeviceIdx, &DeviceInfo);		
		UnlockI2C();
        if (Status != CY_SUCCESS)
        {	continue;	}

        for (InterfaceIdx = 0; InterfaceIdx < DeviceInfo.numInterfaces; InterfaceIdx++)
        {
			if ( szDeviceSN > 0 )
			{
				if ( strcmp((char*)(DeviceInfo.serialNum), DeviceSN) != 0)
				{	continue; }
			}
            if (DeviceInfo.deviceType[InterfaceIdx] == CY_TYPE_I2C)
            {	
				LockI2C();
                Status = CyOpen(DeviceIdx, InterfaceIdx, Handle);
				UnlockI2C();
                if (Status == CY_SUCCESS)
                {	return true;	}
            }
        }
    }

	sprintf(m_Error, "Get I2C Handle Error %d!!! \n", Status);
	return false;
}

bool CCypressI2C::CheckIsConnected()
{
	if ( GetIsConnected() == false )
	{
		sprintf(m_Error, "I2C Not Connected");
		return false;
	}
	return true;
}

bool CCypressI2C::GetIsConnected() const
{
	if ( NULL == s_Handle )
	{	return false; }
	return true;
}

bool CCypressI2C::CYPRESS_I2C_RequestI2CBusAccess()
{
	uint8_t Value     = 0;
    time_t  StartTime = time(NULL);

    if (!CYPRESS_I2C_SetCyGpio(REQUEST_I2C_ACCESS_GPIO, 1))
    {
		sprintf(m_Error, "Request I2C Start Error \n");
		return false;
    }

    while ((time(NULL) - StartTime) < I2C_TIMEOUT_MILLISECONDS)
    {
        if (!CYPRESS_I2C_GetCyGpio(I2C_ACCESS_GRANTED_GPIO, &Value))
        {
            break;
        }

        if (Value == 1)
        {
            if (!CYPRESS_I2C_SetCyGpio(START_I2C_TRANSACTION_GPIO, 1))
            {
                break;
            }

            CyI2cReset(s_Handle, false);
            CyI2cReset(s_Handle, true);

            return true;
        }
    }

	sprintf(m_Error, "Request I2C End Error \n");
	return false;
}

bool CCypressI2C::CYPRESS_I2C_RelinquishI2CBusAccess()
{
	return CYPRESS_I2C_SetCyGpio(REQUEST_I2C_ACCESS_GPIO, 0) 
        && CYPRESS_I2C_SetCyGpio(START_I2C_TRANSACTION_GPIO, 0);
}

bool CCypressI2C::CYPRESS_I2C_WriteI2C(uint32_t WriteDataLength, uint8_t* WriteData)
{
	if ( CheckIsConnected() == false ) { return false; }

	CY_DATA_BUFFER   WriteBuffer;
    CY_RETURN_STATUS Status;

    WriteBuffer.buffer        = WriteData;
    WriteBuffer.length        = WriteDataLength;
    WriteBuffer.transferCount = 0;
    
    Status = CyI2cWrite(s_Handle, 
                        &s_DataConfig,
                        &WriteBuffer,
                        I2C_TIMEOUT_MILLISECONDS);
    if (Status != CY_SUCCESS)
    {		
		sprintf(m_Error, "Write I2C Error %d!!! \n", Status);
		CyI2cReset(s_Handle, false);
        CyI2cReset(s_Handle, true);
		return false;
    }
    
#ifdef DLP4710_MCU_USE
	//if (s_DataConfig.slaveAddress == MCU_I2C_SLAVE_ADDRESS) 
	{
		CyI2cReset(s_Handle, false);
		CyI2cReset(s_Handle, true);
	}
#endif//DLP4710_MCU_USE
    return true;
}

bool CCypressI2C::CYPRESS_I2C_ReadI2C(uint32_t ReadDataLength, uint8_t* ReadData)
{
	if ( CheckIsConnected() == false ) { return false; }

	CY_DATA_BUFFER   ReadBuffer;
    CY_RETURN_STATUS Status;

    ReadBuffer.buffer        = ReadData;
    ReadBuffer.length        = ReadDataLength;
    ReadBuffer.transferCount = 0;

    Status = CyI2cRead(s_Handle,
                       &s_DataConfig,
                       &ReadBuffer,
                       I2C_TIMEOUT_MILLISECONDS);
    if ((Status != CY_SUCCESS) && (Status != CY_ERROR_IO_TIMEOUT))
    {
		sprintf(m_Error, "Read I2C Error %d!!! \n", Status);
        CyI2cReset(s_Handle, false);
		CyI2cReset(s_Handle, true);
		return false;
    }

    return true;
}

bool CCypressI2C::CYPRESS_I2C_CloseCyI2C()
{
	CY_RETURN_STATUS Status;	
	if ( GetIsConnected() == false ) { return true; }	
	//Status = CyClose(s_Handle);
	Status = CY_SUCCESS;
	s_Handle = NULL;
	if ( Status != CY_SUCCESS )
	{
		sprintf(m_Error, "Disconnect to I2C Error %d!!! \n", Status);
		return false;
	}	
	return true;
}

bool CCypressI2C::CYPRESS_I2C_ConnectToCyI2C(char *DeviceSN)
{
	CY_RETURN_STATUS Status;	
    CY_I2C_CONFIG I2CConfig;
        
    if (!GetCyI2CHandle(&s_Handle, DeviceSN))
    {
        return false;
    }

    I2CConfig.frequency      = I2C_CLOCK_FREQUENCY_HZ;
#ifndef DLP4710_MCU_USE
    I2CConfig.slaveAddress   = 0x30;
#else
	I2CConfig.slaveAddress	 = 0x60;//for MCU slave
#endif//DLP4710_MCU_USE
    I2CConfig.isMaster       = true;
    I2CConfig.isClockStretch = false;
    
    Status = CySetI2cConfig(s_Handle, &I2CConfig);
    if (Status != CY_SUCCESS)
    {
		sprintf(m_Error, "Connect to I2C Error %d!!! \n", Status);
        return false;
    }

#ifndef DLP4710_MCU_USE
    s_DataConfig.isNakBit     = true;
#else
	s_DataConfig.isNakBit     = false;
#endif//DLP4710_MCU_USE
    s_DataConfig.isStopBit    = true;
    s_DataConfig.slaveAddress = DLP_I2C_SLAVE_ADDRESS;

    return true;
}

bool CCypressI2C::CYPRESS_I2C_ConnectToCyI2C_MCU(char *DeviceSN)//20240702
{
	CY_RETURN_STATUS Status;
	CY_I2C_CONFIG I2CConfig;

	if (!GetCyI2CHandle(&s_Handle, DeviceSN))
	{
		return false;
	}

	//Status = CyGetI2cConfig(s_Handle, &I2CConfig);

	I2CConfig.frequency = I2C_CLOCK_FREQUENCY_HZ;
#ifndef DLP4710_MCU_USE
	I2CConfig.slaveAddress   = 0x30;
#else
	I2CConfig.slaveAddress = 0x60;//for MCU slave
#endif//DLP4710_MCU_USE
	I2CConfig.isMaster = true;
	I2CConfig.isClockStretch = false;

	Status = CySetI2cConfig(s_Handle, &I2CConfig);
	if (Status != CY_SUCCESS)
	{
		//printf("Connect to I2C Error %d!!! \n", Status);
		return false;
	}

	s_DataConfig.isNakBit     = false;
	s_DataConfig.isStopBit    = true;
	s_DataConfig.slaveAddress = MCU_I2C_SLAVE_ADDRESS;
	//s_DataConfig.slaveAddress = DLP_I2C_SLAVE_ADDRESS;

	return true;
}

bool CCypressI2C::CYPRESS_I2C_ConnectToCyI2C_eeprom(char *DeviceSN)//20240702
{
	CY_RETURN_STATUS Status;
	CY_I2C_CONFIG I2CConfig;

	if (!GetCyI2CHandle(&s_Handle, DeviceSN))
	{
		return false;
	}

	//Status = CyGetI2cConfig(s_Handle, &I2CConfig);

	I2CConfig.frequency = I2C_CLOCK_FREQUENCY_HZ;
#ifndef DLP4710_MCU_USE
	I2CConfig.slaveAddress   = 0x30;
#else
	I2CConfig.slaveAddress = 0x60;//for MCU slave
#endif//DLP4710_MCU_USE
	I2CConfig.isMaster = true;
	I2CConfig.isClockStretch = false;

	Status = CySetI2cConfig(s_Handle, &I2CConfig);
	if (Status != CY_SUCCESS)
	{
		//printf("Connect to I2C Error %d!!! \n", Status);
		return false;
	}

	s_DataConfig.isNakBit = false;
	s_DataConfig.isStopBit = true;
	s_DataConfig.slaveAddress = EEPROM_I2C_SLAVE_ADDRESS;
	//s_DataConfig.slaveAddress = DLP_I2C_SLAVE_ADDRESS;

	return true;
}

bool CCypressI2C::CYPRESS_I2C_GetCyGpio(uint8_t GpioNum, uint8_t* Value)
{
	return CyGetGpioValue(s_Handle, GpioNum, Value) == CY_SUCCESS;
}

bool CCypressI2C::CYPRESS_I2C_SetCyGpio(uint8_t GpioNum, uint8_t Value)
{
	return CySetGpioValue(s_Handle, GpioNum, Value) == CY_SUCCESS;
}