// Serial.cpp


#include "stdafx.h"
#include "Serial.h"

CSerial::CSerial()
{
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
	m_hIDComDev = NULL;
	m_bOpened = FALSE;
}

CSerial::~CSerial()
{

	Close();

}

bool CSerial::Open( int nPort, int nBaud , int nByteSize, int nParity, int nStopBits )
{
	if ( IsStopBitsOK(nStopBits) == false ) { return false; }
	if ( IsParityOK(nParity) == false ) { return false; }
	if ( IsByteSizeOK(nByteSize) == false ) { return false; }

	if( m_bOpened ) return( TRUE );
	TCHAR szPort[32];
	
	DCB dcb;	
	
	//wsprintf( szPort, "COM%d", nPort );//只適用小於10以下的COM Port
	wsprintf( szPort, _T("\\\\.\\COM%d"), nPort );//可使用於大於10以上的COM Port

	m_hIDComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	if( m_hIDComDev == NULL ) return( FALSE );

	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	SetCommTimeouts( m_hIDComDev, &CommTimeOuts );	

	m_OverlappedRead.hEvent = ::CreateEventA( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = ::CreateEventA( NULL, TRUE, FALSE, NULL );

	dcb.DCBlength = sizeof( DCB );
	GetCommState( m_hIDComDev, &dcb );
	dcb.BaudRate = nBaud;
	dcb.ByteSize = nByteSize;
    dcb.Parity = nParity;
    dcb.StopBits = nStopBits;	
	
	unsigned char ucSet;
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_DTRDSR ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_RTSCTS ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_XONXOFF ) != 0 );	

	if( !SetCommState( m_hIDComDev, &dcb ) ||
		!SetupComm( m_hIDComDev, 10000, 10000 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL ){
		DWORD dwError = GetLastError();
		if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
		if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
		CloseHandle( m_hIDComDev );
		return( FALSE );
		}

	m_bOpened = TRUE;

	return( m_bOpened );

}

bool CSerial::Close()
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( TRUE );

	if( m_OverlappedRead.hEvent != NULL ) CloseHandle( m_OverlappedRead.hEvent );
	if( m_OverlappedWrite.hEvent != NULL ) CloseHandle( m_OverlappedWrite.hEvent );
	CloseHandle( m_hIDComDev );
	m_bOpened = FALSE;
	m_hIDComDev = NULL;
	m_OverlappedRead.hEvent = NULL;
	m_OverlappedWrite.hEvent = NULL;

	return( TRUE );

}

bool CSerial::WriteCommByte( unsigned char ucByte )
{
	BOOL bWriteStat;
	DWORD dwBytesWritten;

	bWriteStat = WriteFile( m_hIDComDev, (LPSTR) &ucByte, 1, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) ){
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) dwBytesWritten = 0;
		else{
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, FALSE );
			m_OverlappedWrite.Offset += dwBytesWritten;
			}
		}

	return( TRUE );

}

int CSerial::SendData( const char *buffer, int size )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	//--------------------------------------------------------------//
	DWORD  dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( ComStat.cbInQue ) 
	{	::PurgeComm(m_hIDComDev,PURGE_RXCLEAR); }

	//--------------------------------------------------------------//

	DWORD dwBytesWritten = 0;
	int i;
	for( i=0; i<size; i++ ){
		WriteCommByte( buffer[i] );
		dwBytesWritten++;
		}

	return( (int) dwBytesWritten );

}

int CSerial::ReadDataWaiting( void )
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );

	return( (int) ComStat.cbInQue );

}

int CSerial::ReadData( void *buffer, int limit )
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	dwBytesRead = (DWORD) ComStat.cbInQue;
    	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;

     	bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
     //   bReadStatus = ReadFile( m_hIDComDev, buffer, 255, &(unsigned long)limit, &m_OverlappedRead );
	if( !bReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return( (int) dwBytesRead );
			}
		return( 0 );
		}

	return( (int) dwBytesRead );

}

bool CSerial::IsByteSizeOK(int nByteSize)
{
	if ( nByteSize<4 || nByteSize>8) { return false; }
	return true;
}

bool CSerial::IsParityOK(int nParity)
{
	switch ( nParity ) 
	{
	case SERIES_PARITY_NONE:
		return true;
	case SERIES_PARITY_ODD:
		return true;
	case SERIES_PARITY_EVEN:
		return true;
	case SERIES_PARITY_MASK:
		return true;
	case SERIES_PARITY_SPACE:
		return true;
	default:
		return false;
	}
}

bool CSerial::IsStopBitsOK(int nStopBits)
{
	
	switch ( nStopBits )
	{
	case SERIES_STOPBITS_10:
		return true;
	case SERIES_STOPBITS_15:
		return true;
	case SERIES_STOPBITS_20:
		return true;
	default:
		return false;
	}
}

int CSerial::ReadSingleChar(char &ch)
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( false );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	//dwBytesRead = (DWORD) ComStat.cbInQue;
    dwBytesRead = 1;

    bReadStatus = ReadFile( m_hIDComDev, &ch, dwBytesRead, &dwBytesRead, &m_OverlappedRead );
     //   bReadStatus = ReadFile( m_hIDComDev, buffer, 255, &(unsigned long)limit, &m_OverlappedRead );
	if( !bReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( m_OverlappedRead.hEvent, 2000 );
			return( (int) dwBytesRead );
			}
		return( 0 );
		}

	return( (int)dwBytesRead);
}
