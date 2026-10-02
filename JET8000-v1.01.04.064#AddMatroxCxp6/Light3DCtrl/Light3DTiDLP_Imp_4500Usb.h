// Light3DTiDLP_Imp_4500Usb.h: interface for the CLight3DTiDLP_Imp_4500Usb class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP_IMP_4500USB_H__C50FE2B4_03A5_4D2D_A671_76303C18FB03__INCLUDED_)
#define AFX_LIGHT3DTIDLP_IMP_4500USB_H__C50FE2B4_03A5_4D2D_A671_76303C18FB03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
#include "..\\JET8000_Library\\TiDLP\\hidapi.h"
//-------------------------------------------------------------------------------------//
#ifndef DLPC350_USB_MAX_PACKET_SIZE
	#define DLPC350_USB_MAX_PACKET_SIZE 64
#endif//DLPC350_USB_MAX_PACKET_SIZE
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP_Imp_4500Usb
{
private:
	//---------------------------------------------------------------------------------//	
	hid_device *m_DeviceHandle;	//Handle to write
	//In/Out buffers equal to HID endpoint size + 1
	//First byte is for Windows internal use and it is always 0
	unsigned char m_OutputBuffer[DLPC350_USB_MAX_PACKET_SIZE+1];//USB_MAX_PACKET_SIZE
	unsigned char m_InputBuffer[DLPC350_USB_MAX_PACKET_SIZE+1];//USB_MAX_PACKET_SIZE
	int  m_USBConnected;      //Boolean true when device is connected
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP_Imp_4500Usb(const CLight3DTiDLP_Imp_4500Usb &usb);
	CLight3DTiDLP_Imp_4500Usb& operator=(const CLight3DTiDLP_Imp_4500Usb &usb);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP_Imp_4500Usb();
	virtual ~CLight3DTiDLP_Imp_4500Usb();
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
#endif//LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHT3DTIDLP_IMP_4500USB_H__C50FE2B4_03A5_4D2D_A671_76303C18FB03__INCLUDED_)
