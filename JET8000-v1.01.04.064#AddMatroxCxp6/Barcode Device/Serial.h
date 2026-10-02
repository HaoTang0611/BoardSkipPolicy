// Serial.h
#include <windows.h>
#ifndef __SERIAL_H__
#define __SERIAL_H__

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13
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
class CSerial
{

public:
	
	CSerial();
	~CSerial();

	// init COM port
	bool Open(int nPort, int nBaud , int nByteSize, int nParity, int nStopBits);
	bool Close();                             // close COM port

	int ReadSingleChar(char &ch);             // read a character from COM port
	int ReadData( void *, int );              // Read string from COM port 
	int SendData( const char *, int );        // Send string to COM port
	int ReadDataWaiting( void );              // return read buffer number

	bool IsOpened(){ return( m_bOpened ); }   // Is open COM port

protected:

	bool IsStopBitsOK(int nStopBits);         // Is set Stop-bit
	bool IsParityOK(int nParity);             // Is set parity
	bool WriteCommByte( unsigned char );      // send character
	bool IsByteSizeOK(int nByteSize);         // Is Set Byte size

	HANDLE m_hIDComDev;                       // COM headle
	OVERLAPPED m_OverlappedRead, m_OverlappedWrite;  
	bool m_bOpened;                           // Is COM open?

};

#endif
