// Serial.h
#include <windows.h>
#ifndef _JET_SERIAL_H__
#define _JET_SERIAL_H__

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
//-------------------------------------------------------------//
#define ASCII_STX		0x02
#define ASCII_ETX		0x03
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13
#define ASCII_ESC       0x1B
//-------------------------------------------------------------//
#define SERIES_PARITY_NONE  0
#define SERIES_PARITY_ODD   1
#define SERIES_PARITY_EVEN  2
#define SERIES_PARITY_MASK  3
#define SERIES_PARITY_SPACE 4
//-------------------------------------------------------------//
#define SERIES_STOPBITS_10  0
#define SERIES_STOPBITS_15  1
#define SERIES_STOPBITS_20  2
//-------------------------------------------------------------//
class CJetSerial
{
public:	
	CJetSerial();
	~CJetSerial();

	// init COM port
	bool       Open(int nPort, int nBaud , int nByteSize, int nParity, int nStopBits);
	bool       Close();                             // close COM port

	int        ReadSingleChar(char &ch);             // read a character from COM port
	int        ReadDataByNumber(void *, int, int); // Read string from COM port 	
	int        ReadDataByEndChar(void *, int); // Read string from COM port 	
	int        ReadData_Syn(void *, int );          // Read string from COM port 
	int        ReadDataKernal(void *, int, bool &, bool CareRXFLAG);          // Read string from COM port 
	int        SendData(const char *, int );        // Send string to COM port
	int        ReadDataWaiting( void );              // return read buffer number

	bool       IsOpened(){ return( m_bOpened ); }   // Is open COM port
	void       TraceText(LPCTSTR Text);

	void       ClearBuffer();
	bool       SetEventChar(char EvtChar);
	void       SetLoopSleeipTime(DWORD Time_ms);//°j°éªº®É¶¡©µ¿ð
protected:

	bool       IsStopBitsOK(int nStopBits);         // Is set Stop-bit
	bool       IsParityOK(int nParity);             // Is set parity
	bool       WriteCommByte( unsigned char );      // send character
	int        WriteCommBytes( const char *Buffer, int nBuffers );      // send character
	bool       IsByteSizeOK(int nByteSize);         // Is Set Byte size

	DCB        m_DCB;
	HANDLE     m_hIDComDev;                       // COM headle
	OVERLAPPED m_OverlappedRead, m_OverlappedWrite;  
	bool       m_bOpened;                           // Is COM open?		
	DWORD      m_LoopSleeipTime;
};

#endif

