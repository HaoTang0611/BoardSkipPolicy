// Light3DTiDLP_Imp_4500Usb.cpp: implementation of the CLight3DTiDLP_Imp_4500Usb class.
//
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP_Imp_4500Usb.h"
//------------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4500
//------------------------------------------------------------------------------------------//
#include "DLPC350_3_1_0\\dlpc350_usb.h"
//------------------------------------------------------------------------------------------//
#ifdef _X64
	#pragma comment(lib,"..\\JET8000_Library\\TiDLP\\x64\\hidapi(x64).lib")
#else
	#pragma comment(lib,"..\\JET8000_Library\\TiDLP\\x32\\hidapi(x32).lib")
#endif//_X64
//------------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4500Usb::CLight3DTiDLP_Imp_4500Usb()
{
	PreInitial();
}
//------------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4500Usb::~CLight3DTiDLP_Imp_4500Usb()
{
	ReleaseAll();
}
//------------------------------------------------------------------------------------------//
unsigned char* CLight3DTiDLP_Imp_4500Usb::GetOutputBuffer()
{
	return m_OutputBuffer;
}
//------------------------------------------------------------------------------------------//
unsigned char* CLight3DTiDLP_Imp_4500Usb::GetInputBuffer()
{
	return m_InputBuffer;
}
//------------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500Usb::ReleaseAll()
{
	USB_Close();
}
//------------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500Usb::PreInitial()
{
	m_DeviceHandle = NULL;
    m_USBConnected = 0;
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_IsConnected()
{
    return m_USBConnected;
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_Init(void)
{
    return hid_init();
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_Exit(void)
{
    return hid_exit();
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_Open(const wchar_t *serial_number)
{
	//return USB_Open_Ti(serial_number);
	return USB_Open_JET(serial_number);    
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_Open_Ti(const wchar_t *serial_number)
{
	// Open the device using the VID, PID,
    // and optionally the Serial number.
	m_DeviceHandle = hid_open(MY_VID, MY_PID, serial_number);
    if(m_DeviceHandle == NULL)
    {
        m_USBConnected = 0;
        return -1;
    }
    m_USBConnected = 1;
    return 0;
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_Open_JET(const wchar_t *serial_number)
{
	// Open the device using the VID, PID,
	// and optionally the Serial number.
	struct hid_device_info *hid_info;
	hid_info = hid_enumerate(MY_VID, MY_PID);
	m_USBConnected = 0;
	m_DeviceHandle = NULL;	
	struct hid_device_info *hid_head_info=hid_info;
	while ( hid_info )
	{
		if ( 0 == hid_info->interface_number )
		{
			if ( 0 == wcscmp(hid_info->serial_number, serial_number) )
			{
				m_DeviceHandle = hid_open_path(hid_info->path);
				break;
			}
		}
		hid_info=hid_info->next;
	};	
	hid_free_enumeration(hid_head_info);

	if(m_DeviceHandle == NULL)
	{
		m_USBConnected = 0;
		return -1;
	}
	m_USBConnected = 1;
	return 0;
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_Write()
{
	int bytesWritten;
    if(m_DeviceHandle == NULL)  { return -1; }
	if((bytesWritten = hid_write(m_DeviceHandle, m_OutputBuffer, USB_MIN_PACKET_SIZE+1)) == -1)    
	{
		//USB_Close();
		return -1; 
	}
	return bytesWritten;
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_Read()
{
	int bytesRead;
    if(m_DeviceHandle == NULL)  { return -1; }
	//clear out the input buffer
    memset((void*)&m_InputBuffer[0],0x00,USB_MIN_PACKET_SIZE+1);
	if((bytesRead = hid_read_timeout(m_DeviceHandle, m_InputBuffer, USB_MIN_PACKET_SIZE+1, 2000)) == -1)    
	{
		//USB_Close();
		return -1; 
	}
	return bytesRead;
}
//------------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Usb::USB_Close()
{
	if( m_DeviceHandle != NULL )
	{ hid_close(m_DeviceHandle); }
	m_DeviceHandle = NULL;
	m_USBConnected = 0;
	return 0;
}
//------------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_USE_IMP_4500