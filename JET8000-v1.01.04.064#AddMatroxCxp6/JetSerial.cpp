// Serial.cpp
//----------------------------------------------------------------------------//
#include "stdafx.h"
#include "JetSerial.h"
//----------------------------------------------------------------------------//
CJetSerial::CJetSerial()
{
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );
	m_hIDComDev = NULL;
	m_bOpened = FALSE;	
	m_LoopSleeipTime = 0;
}
//----------------------------------------------------------------------------//
CJetSerial::~CJetSerial()
{
	Close();
}
//----------------------------------------------------------------------------//
bool CJetSerial::Open( int nPort, int nBaud , int nByteSize, int nParity, int nStopBits)
{
	if ( IsStopBitsOK(nStopBits) == false ) { return false; }
	if ( IsParityOK(nParity) == false ) { return false; }
	if ( IsByteSizeOK(nByteSize) == false ) { return false; }

	if( m_bOpened ) return( TRUE );
	CString szPort;
	
	//wsprintf( szPort, "COM%d", nPort );//只適用小於10以下的COM Port
	szPort.Format(_T("\\\\.\\COM%d"), nPort );//可使用於大於10以上的COM Port

	m_hIDComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
	if( m_hIDComDev == NULL ) return( FALSE );
	
	memset( &m_OverlappedRead, 0, sizeof( OVERLAPPED ) );
 	memset( &m_OverlappedWrite, 0, sizeof( OVERLAPPED ) );

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 5;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 500;//ReadTotal Time = ReadTotalTimeoutMultiplier * nChar + ReadTotalTimeoutConstant;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;	
	
	/*
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;	
	*/
	if ( SetCommTimeouts( m_hIDComDev, &CommTimeOuts ) == FALSE )
	{	return FALSE; }

	DWORD MaskAll = EV_BREAK|EV_CTS|EV_DSR|EV_ERR|EV_RING|EV_RLSD|EV_RXCHAR|EV_RXFLAG|EV_TXEMPTY;
	if ( SetCommMask( m_hIDComDev, MaskAll ) == FALSE )  //specifies a set of events to be monitored for a communications device
	{	return FALSE; }

	m_OverlappedRead.hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );
	m_OverlappedWrite.hEvent = ::CreateEvent( NULL, TRUE, FALSE, NULL );

	m_DCB.DCBlength = sizeof( DCB );
	GetCommState( m_hIDComDev, &m_DCB );
	m_DCB.BaudRate = nBaud;
	m_DCB.ByteSize = nByteSize;
    m_DCB.Parity = nParity;
    m_DCB.StopBits = nStopBits;	
	
	unsigned char ucSet;
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_DTRDSR ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_RTSCTS ) != 0 );
	ucSet = (unsigned char) ( ( FC_RTSCTS & FC_XONXOFF ) != 0 );	

	if( !SetCommState( m_hIDComDev, &m_DCB ) ||
		!SetupComm( m_hIDComDev, 10000, 10000 ) ||
		//!SetupComm( m_hIDComDev, 2048, 1024 ) ||
		m_OverlappedRead.hEvent == NULL ||
		m_OverlappedWrite.hEvent == NULL )
	{
		DWORD dwError = GetLastError();
		if( m_OverlappedRead.hEvent != NULL ) 
		{	CloseHandle( m_OverlappedRead.hEvent );	}
		if( m_OverlappedWrite.hEvent != NULL ) 
		{	CloseHandle( m_OverlappedWrite.hEvent ); }
		CloseHandle( m_hIDComDev );
		return( FALSE );
	}

	m_bOpened = TRUE;

	return( m_bOpened );

}
//----------------------------------------------------------------------------//
bool CJetSerial::Close()
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
//----------------------------------------------------------------------------//
bool CJetSerial::WriteCommByte( unsigned char ucByte )
{
	BOOL bWriteStat;
	DWORD dwBytesWritten;

	//GetOverlappedResult
	bWriteStat = WriteFile( m_hIDComDev, (LPSTR) &ucByte, 1, &dwBytesWritten, &m_OverlappedWrite );
	if( !bWriteStat && ( GetLastError() == ERROR_IO_PENDING ) )
	{
		if( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) 
		{	dwBytesWritten = 0;	}
		else
		{
			GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, TRUE );
			m_OverlappedWrite.Offset += dwBytesWritten;
		}
	}
	return (true);

}
//----------------------------------------------------------------------------//
int CJetSerial::WriteCommBytes( const char *Buffer, int nBuffers )// send character
{
	BOOL bWriteStat;
	DWORD dwBytesWritten=0;
	bWriteStat = WriteFile( m_hIDComDev,	// handle to file to write to      
                          Buffer,			// pointer to data to write to file     
                          nBuffers,			// number of bytes to write     
                          &dwBytesWritten,	// pointer to number of bytes written     
                          &m_OverlappedWrite );// pointer to structure needed for overlapped I/O    
  
	if( bWriteStat==FALSE )
	{
		if (  GetLastError() == ERROR_IO_PENDING ) 
		{
			if ( WaitForSingleObject( m_OverlappedWrite.hEvent, 1000 ) ) 
			{	dwBytesWritten = 0;	}
			else
			{
				GetOverlappedResult( m_hIDComDev, &m_OverlappedWrite, &dwBytesWritten, TRUE );
				m_OverlappedWrite.Offset += dwBytesWritten;
			}
		}
	}

	return dwBytesWritten;
}
//----------------------------------------------------------------------------//
int CJetSerial::SendData( const char *buffer, int size )
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	//--------------------------------------------------------------//
	DWORD  dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( ComStat.cbInQue ) 
	{	::PurgeComm(m_hIDComDev,PURGE_RXCLEAR); }
	//--------------------------------------------------------------//
	return WriteCommBytes(buffer, size);
	//--------------------------------------------------------------//
}
//----------------------------------------------------------------------------//
int CJetSerial::ReadDataWaiting( void )
{

	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );

	return( (int) ComStat.cbInQue );

}
//----------------------------------------------------------------------------//
int CJetSerial::ReadData_Syn( void *buffer, int limit )// Read string from COM port 
{
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesReadNeed, dwBytesReadGet, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue )
	{	return( 0 ); }

	dwBytesReadNeed = (DWORD) ComStat.cbInQue;
   	if( limit < (int) dwBytesReadNeed ) 
	{	dwBytesReadNeed = (DWORD) limit; }
	
	bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesReadNeed, &dwBytesReadGet, &m_OverlappedRead );
	//bReadStatus = ReadFile( m_hIDComDev, buffer, 255, &(unsigned long)limit, &m_OverlappedRead );
	if( !bReadStatus )
	{
		dwErrorFlags = GetLastError();
		if( ERROR_IO_PENDING == dwErrorFlags )
		{
			DWORD dwBytesReadGet2=0;
			 BOOL bResult = GetOverlappedResult(m_hIDComDev, &m_OverlappedRead, &dwBytesReadGet2, FALSE); 			
			return( (int) dwBytesReadGet2 );
		}
		return( dwBytesReadGet );
	}

	return( (int) dwBytesReadGet );
}
//----------------------------------------------------------------------------//
int CJetSerial::ReadDataKernal( void *buffer, int limit , bool &bRXFLAG, bool CareRXFLAG)// Read string from COM port 
{	
	if( !m_bOpened || m_hIDComDev == NULL ) return( 0 );

	CString str;
	const int dwMaxWait = 1000;
	BOOL  bReadStatus;   
	DWORD dwBytesReadOL=0;
	DWORD dwBytesRead, dwErrorFlags, dwErrorFlags2;
	DWORD dwBytesNeed = limit;
    DWORD result = 0,   
          read   = 0, // num read bytes    
          Mask   = 0; // a 32-bit variable that receives a mask     
                      // indicating the type of event that occurred    
	/*
	COMSTAT ComStat;
	ClearCommError( m_hIDComDev, &dwErrorFlags, &ComStat );
	this->TraceText("ClearCommError\n");
	if( !ComStat.cbInQue )
	{	return( 0 );	}
	dwBytesRead = (DWORD) ComStat.cbInQue;
   	if( limit < (int) dwBytesRead ) 
	{	dwBytesRead = (DWORD) limit; }		
	*/

	dwBytesRead = 0;
	dwBytesNeed = (DWORD) limit;
	
	// Specify here the event to be enabled    	
	//EV_BREAK// A break was detected on input. 
	//EV_CTS //The CTS (clear-to-send) signal changed state. 
	//EV_DSR //The DSR (data-set-ready) signal changed state. 
	//EV_ERR //A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
	//EV_RING //A ring indicator was detected. 
	//EV_RLSD //The RLSD (receive-line-signal-detect) signal changed state. 
	//EV_RXCHAR //A character was received and placed in the input buffer. 
	//EV_RXFLAG //The event character was received and placed in the input buffer. The event character is specified in the device's DCB structure, which is applied to a serial port by using the SetCommState function. 
	//EV_TXEMPTY//The last character in the output buffer was sent 
    //bReadStatus = SetCommMask( m_hIDComDev, EV_RXFLAG );   

	DWORD MaskAll = EV_BREAK|EV_CTS|EV_DSR|EV_ERR|EV_RING|EV_RLSD|EV_RXCHAR|EV_RXFLAG|EV_TXEMPTY;
	bReadStatus = SetCommMask( m_hIDComDev, MaskAll);   
    if ( ! bReadStatus ) 
	{	return -1;	 }

   // WaitForSingleObject    
    bReadStatus = WaitCommEvent(m_hIDComDev, &Mask, &m_OverlappedRead);
    if ( !bReadStatus ) 
	{   
		dwErrorFlags2 = GetLastError();   
		if ( dwErrorFlags2 == ERROR_IO_PENDING ) 
		{   
			dwErrorFlags2 = WaitForSingleObject(m_OverlappedRead.hEvent, dwMaxWait);// milli seconds before returning    
            if ( result == WAIT_FAILED ) 
			{	return -1;	}   
        }   
    }   
	
	if ( Mask == 0 ) 
	{	return -1;	}	

  // The specified event occured?    
	str.Format(_T("Need:%d\n"), dwBytesNeed);
	this->TraceText(str);
	if ( Mask&EV_BREAK )    
    {
		this->TraceText(_T("EV_BREAK\n"));
		return -1;
	}
	if ( Mask&EV_CTS )    
    {
		this->TraceText(_T("EV_CTS\n"));
	}
	if ( Mask&EV_DSR )    
    {
		this->TraceText(_T("EV_DSR\n"));
	}
	if ( Mask&EV_ERR )    
    {
		this->TraceText(_T("EV_ERR\n"));
	}
	if ( Mask&EV_RING )    
    {
		this->TraceText(_T("EV_RING\n"));
	}
	if ( Mask&EV_RLSD )    
    {
		this->TraceText(_T("EV_RLSD\n"));
	}		
	if ( Mask&EV_RXCHAR )    
    {
		this->TraceText(_T("EV_RXCHAR\n"));

		if ( bRXFLAG==true || CareRXFLAG==false)
		{
			this->TraceText(_T("RXCHAR::ReadFile\n"));
			bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesNeed, &dwBytesRead, &m_OverlappedRead );   
			if ( bReadStatus == FALSE ) 
			{	
				dwErrorFlags = GetLastError();   
				if ( dwErrorFlags == ERROR_IO_PENDING ) 
				{
					bReadStatus = GetOverlappedResult(m_hIDComDev, &m_OverlappedRead, &dwBytesRead, TRUE);
					if ( bReadStatus == FALSE ) 
					{	return -1; }
				}
				else
				{	return -1; }
			}			
			else
			{
				bReadStatus = GetOverlappedResult(m_hIDComDev, &m_OverlappedRead, &dwBytesRead, TRUE);
				if ( bReadStatus == FALSE ) 
				{	return -1; }
			}
			bRXFLAG = true;
		}
	}
	if ( Mask&EV_RXFLAG )    
    {   
		this->TraceText(_T("EV_RXFLAG\n"));
		this->TraceText(_T("RXFLAG::ReadFile\n"));
        bReadStatus = ReadFile( m_hIDComDev, buffer, dwBytesNeed, &dwBytesRead, &m_OverlappedRead );   
		if ( bReadStatus == FALSE ) 
		{	
			dwErrorFlags = GetLastError();   
			if ( dwErrorFlags == ERROR_IO_PENDING ) 
			{
				bReadStatus = GetOverlappedResult(m_hIDComDev, &m_OverlappedRead, &dwBytesRead, TRUE);
				if ( bReadStatus == FALSE ) 
				{	return -1; }
			}
			else
			{	return -1;	}
		}
		else
		{
			bReadStatus = GetOverlappedResult(m_hIDComDev, &m_OverlappedRead, &dwBytesRead, TRUE);
			if ( bReadStatus == FALSE ) 
			{	return -1; }
		}
		bRXFLAG = true;		
    }   
	if ( Mask&EV_TXEMPTY )    
    {
		this->TraceText(_T("EV_TXEMPTY\n"));
	}
	return dwBytesRead;
}
//----------------------------------------------------------------------------//
int CJetSerial::ReadDataByNumber( void *buffer, int limit, int Needs) // Read string from COM port 	
{
	int Res = 0;
	bool bRXFLAG=false;	
	int CurrentNReads = 0;	
	const int MaxI = 10000;	
	int TotalNReads = 0;
	int i = 0;
	char *pBuffer = (char*)buffer;

	TotalNReads = 0;
	bRXFLAG=false;	
	this->TraceText(_T("CJetSerial::ReadData Loop Start\n"));
	do 
	{		
		CurrentNReads = this->ReadDataKernal(&(pBuffer[TotalNReads]), limit-TotalNReads, bRXFLAG, true);
		if ( CurrentNReads < 0 ) 
		{	return -1;	}
		TotalNReads = TotalNReads+CurrentNReads;

		if ( TotalNReads >= Needs ) 
		{	break;	}
		
		i ++ ;
		if ( i > MaxI ) 
		{	return -1;	}

		if ( m_LoopSleeipTime > 0 ) 
		{	::Sleep(m_LoopSleeipTime); }
	} while ( true );	
	
	this->TraceText(_T("CJetSerial::ReadData Loop End\n"));
	return TotalNReads;
}
//----------------------------------------------------------------------------//
int CJetSerial::ReadDataByEndChar( void *buffer, int limit)// Read string from COM port 	
{
	int Res = 0;
	bool bRXFLAG=false;	
	int CurrentNReads = 0;	
	const int MaxI = 10000;
	int TotalNReads = 0;	
	int i = 0;
	char *pBuffer = (char*)buffer;

	TotalNReads = 0;
	bRXFLAG=false;		
	this->TraceText(_T("CJetSerial::ReadData Loop Start\n"));
	do 
	{		
		CurrentNReads = this->ReadDataKernal(&(pBuffer[TotalNReads]), limit-TotalNReads, bRXFLAG, false);
		if ( CurrentNReads < 0 ) 
		{	return -1;	}
		TotalNReads = TotalNReads+CurrentNReads;

		if ( bRXFLAG == true )
		{	break;	}

		i ++ ;
		if ( i > MaxI ) 
		{	return -1;	}

		if ( m_LoopSleeipTime > 0 ) 
		{	::Sleep(m_LoopSleeipTime); }
	} while ( true );	
	
	this->TraceText(_T("CJetSerial::ReadData Loop End\n"));
	return TotalNReads;
}
//----------------------------------------------------------------------------//
bool CJetSerial::IsByteSizeOK(int nByteSize)
{
	if ( nByteSize<4 || nByteSize>8) { return false; }
	return true;
}
//----------------------------------------------------------------------------//
bool CJetSerial::IsParityOK(int nParity)
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
//----------------------------------------------------------------------------//
bool CJetSerial::IsStopBitsOK(int nStopBits)
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
//----------------------------------------------------------------------------//
int CJetSerial::ReadSingleChar(char &ch)
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
//----------------------------------------------------------------------------//
void CJetSerial::TraceText(LPCTSTR Text)
{
#ifndef _UNICODE 
	TRACE0(Text);
#endif
}
//----------------------------------------------------------------------------//
void CJetSerial::ClearBuffer()
{
	char temp;	
	int  count=0;
	while(this->ReadSingleChar(temp) != 0)
	{
		count ++;
	};

	if ( count > 0 ) 
	{
		count = count;
	}
}
//----------------------------------------------------------------------------//
bool CJetSerial::SetEventChar(char EvtChar)
{
	if( !m_bOpened || m_hIDComDev == NULL ) 
	{	return( false ); }
	
	if ( ::GetCommState(m_hIDComDev, &m_DCB) == FALSE ) 
	{	return false; }

	m_DCB.EvtChar  = EvtChar;
	if ( SetCommState( m_hIDComDev, &m_DCB ) == FALSE )
	{	return false; }

	return true;
}
//----------------------------------------------------------------------------//
void CJetSerial::SetLoopSleeipTime(DWORD Time_ms)//迴圈的時間延遲
{
	this->m_LoopSleeipTime = Time_ms;
}
//----------------------------------------------------------------------------//