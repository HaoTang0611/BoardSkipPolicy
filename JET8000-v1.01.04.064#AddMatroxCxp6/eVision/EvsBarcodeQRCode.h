// EvsBarcodeQRCode.h: interface for the CEvsBarcodeQRCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVSBARCODEQRCODE_H__3F0A17E0_1593_45FB_B097_DC31FA24D5AA__INCLUDED_)
#define AFX_EVSBARCODEQRCODE_H__3F0A17E0_1593_45FB_B097_DC31FA24D5AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "EvsRoiBW8.h"
//-------------------------------------------------------------------------------------//
class CEvsBarcodeQRCode  
{
private:
	//---------------------------------------------------------------------------------//
	int                        m_ErrorCode;
	size_t                     m_ErrorStringLen;
	char                       m_ErrorString[128];	
	int                        m_EvsDecodeTimeout;
	int                        m_EvsDecodeTimeoutDefault;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitQRCode();
	void                       InitialQRCode();
	void                       CloneQRCode(const CEvsBarcodeQRCode &QRCode);
	//---------------------------------------------------------------------------------//	
	CEvsBarcodeQRCode(const CEvsBarcodeQRCode &QRCode);
	CEvsBarcodeQRCode& operator=(const CEvsBarcodeQRCode &QRCode);	
	//---------------------------------------------------------------------------------//
	bool                       GetIsEVisionError();		
	bool                       ReturnNoSupportQRCode();
	//---------------------------------------------------------------------------------//
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
	bool                       SaveQRCodeIniFile();//纗QRCode把计郎
	bool                       LoadQRCodeIniFile();//更QRCode把计郎	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CEvsBarcodeQRCode();
	virtual ~CEvsBarcodeQRCode();
	//---------------------------------------------------------------------------------//	
	const char*                GetErrorString() const;	
	//---------------------------------------------------------------------------------//	
	void                       SetEvsDecodeTimeout(int Timeoutms) { m_EvsDecodeTimeout = Timeoutms; }
	//---------------------------------------------------------------------------------//	
	bool                       Read(CEvsRoiBW8 &Roi, char* BarcodeText, size_t BarcodeTextLen);
	//---------------------------------------------------------------------------------//	
	bool                       Read(int ImageW, int ImageH, int ImageStep, unsigned char* pImage, char* BarcodeText, size_t BarcodeTextLen);
	//---------------------------------------------------------------------------------//		

};

#endif // !defined(AFX_EVSBARCODEQRCODE_H__3F0A17E0_1593_45FB_B097_DC31FA24D5AA__INCLUDED_)
