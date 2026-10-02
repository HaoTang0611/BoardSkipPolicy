// Light3DTiDLP4500Usb.h: interface for the CLight3DTiDLP4500Usb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP4500_USB_H__C50FE2B4_03A5_4D2D_A671_76303C18FB03__INCLUDED_)
#define AFX_LIGHT3DTIDLP4500_USB_H__C50FE2B4_03A5_4D2D_A671_76303C18FB03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_4500_USE
//-------------------------------------------------------------------------------------//
#include "..\\JET8000_Library\\TiDLP\\hidapi.h"
//-------------------------------------------------------------------------------------//
#ifdef _X64
	#pragma comment(lib,"..\\JET8000_Library\\TiDLP\\x64\\hidapi(x64).lib")
#else
	#pragma comment(lib,"..\\JET8000_Library\\TiDLP\\x32\\hidapi(x32).lib")
#endif//_X64
//-------------------------------------------------------------------------------------//
#define USB_MIN_PACKET_SIZE 64
#define USB_MAX_PACKET_SIZE 64
//-------------------------------------------------------------------------------------//
//定義裝置號碼
#define MY_VID 0x0451
#define MY_PID 0x6401
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP4500Usb  
{
private:
	//---------------------------------------------------------------------------------//	
	hid_device *m_DeviceHandle;	//Handle to write
	//In/Out buffers equal to HID endpoint size + 1
	//First byte is for Windows internal use and it is always 0
	unsigned char m_OutputBuffer[USB_MAX_PACKET_SIZE+1];
	unsigned char m_InputBuffer[USB_MAX_PACKET_SIZE+1];
	int  m_USBConnected;      //Boolean true when device is connected
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP4500Usb(const CLight3DTiDLP4500Usb &usb);
	CLight3DTiDLP4500Usb& operator=(const CLight3DTiDLP4500Usb &usb);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP4500Usb();
	virtual ~CLight3DTiDLP4500Usb();
	//---------------------------------------------------------------------------------//		
	unsigned char *GetOutputBuffer();
	unsigned char *GetInputBuffer();	
	//---------------------------------------------------------------------------------//	
	void	PreInitial();
	void	ReleaseAll();
	//---------------------------------------------------------------------------------//	
	int 	USB_Open(const wchar_t *serial_number);
	int 	USB_Open_Ti(const wchar_t *serial_number);
	int 	USB_Open_JET(const wchar_t *serial_number);
	int		USB_IsConnected();
	int 	USB_Write();
	int 	USB_Read();
	int 	USB_Close();
	int		USB_Init();
	int		USB_Exit();
	//---------------------------------------------------------------------------------//
};
#endif//LIGHT_3D_TI_DLP_4500_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHT3DTIDLP4500_USB_H__C50FE2B4_03A5_4D2D_A671_76303C18FB03__INCLUDED_)
