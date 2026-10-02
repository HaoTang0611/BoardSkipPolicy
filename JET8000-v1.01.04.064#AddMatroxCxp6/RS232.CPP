#include "stdafx.h"
#include "RS232.h"
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::CRS232()                                                        ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
   
CRS232::CRS232( )   
{   
    m_hCom = 0;   
    m_lpszErrorMessage[0] = '\0';   
    ZeroMemory(&m_ov, sizeof(m_ov));   
}   
   
  
   
   
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::~CRS232()                                                       ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
   
CRS232::~CRS232() {   
   Close();   
}   
   
   
   
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::GetErrorMessage                                                ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
   
LPSTR   CRS232::GetErrorMessage( void ) {   
   return m_lpszErrorMessage;   
}   
   
   
   
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::open( )                                                        ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////   
bool CRS232::Open(int nPort, int nBaud, int nByteSize, int nParity, int nStopBits)
{   
	Close();
    DCB  dcb;  //串口設備控制塊 Device Control Block 
    BOOL bSuccess = FALSE;   

	CString szPort;
	if(nPort>9)
	{ szPort.Format(_T("\\\\.\\COM%d"), nPort );  }
	else
	{ szPort.Format(_T("COM%d"), nPort );  }
   
   // m_hCom即為函數返回的串口的控制碼   
    m_hCom = CreateFile( szPort,            // pointer to name of the file
                         GENERIC_READ|GENERIC_WRITE, // 允許讀寫。  
                         0,                     // 通訊設備必須以獨佔方式打開    
                         NULL,                  // 無安全屬性，表示該串口不可被副程式繼承。      
                         OPEN_EXISTING,         // 通訊設備已存在。 
                         FILE_FLAG_OVERLAPPED,  // 使用非同步方式 overlapped I/O。  
                         NULL);                 // 通訊設備不能用範本打開
    if ( m_hCom == INVALID_HANDLE_VALUE ) {   
       CRS232::ErrorToString("CRS232::open()   CreateFile() failed, invalid handle value");   
        return FALSE;   
    }   
   
   // 與串口相關的參數非常多，當需要設置串口參數時，通常是先取得串口 
   // 的參數結構，修改部分參數後再將參數結構寫入   
    bSuccess = GetCommState(m_hCom, &dcb);   
    if ( !bSuccess ) {   
        CRS232::Close();   
        CRS232::ErrorToString("CRS232::open()   GetCommState() failed");   
        return FALSE;   
    }   
	dcb.BaudRate = nBaud;
	dcb.ByteSize = nByteSize;
    dcb.Parity = nParity;
    dcb.StopBits = nStopBits;
//	dcb.EvtChar = 62;
	bSuccess = SetCommState( m_hCom, &dcb );
    if ( !bSuccess ) {   
        CRS232::Close();   
        CRS232::ErrorToString("CRS232::open()   SetCommState() failed");   
        return FALSE;   
    }   

//	bSuccess = SetupComm( m_hCom, 10000, 10000 );
	bSuccess = SetupComm( m_hCom, 2048, 1024 );
    if ( !bSuccess ) {   
        CRS232::Close();   
        CRS232::ErrorToString("CRS232::open()   SetCommState() failed");   
        return FALSE;   
    }   

	//fang 
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 0;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 5000;
	bSuccess = SetCommTimeouts( m_hCom, &CommTimeOuts );	
	if ( !bSuccess ) {   
        CRS232::Close();   
        CRS232::ErrorToString("CRS232::open()   SetCommState() failed");   
        return FALSE;   
	} 
   
    return TRUE;   
}   
//----------------------------------------------------------------------------------//
bool CRS232::SetRWTimeouts(COMMTIMEOUTS CommTimeOuts)
{
    BOOL bSuccess = FALSE;   
	bSuccess = SetCommTimeouts( m_hCom, &CommTimeOuts );	
	if ( !bSuccess ) {   
        CRS232::Close();   
        CRS232::ErrorToString("CRS232::open()   SetCommState() failed");   
        return false;   
	}
	return true;
}
//----------------------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::SetEventChar()                                                    ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    

bool CRS232::SetEventChar(char EvtChar)
{
	BOOL bSuccess = false;
	DCB  dcb;  //串口設備控制塊 Device Control Block 
	bSuccess = GetCommState(m_hCom, &dcb);   
	if ( !bSuccess ) 
	{   
		CRS232::Close();
		CRS232::ErrorToString("CRS232::SetEventChar()   GetCommState() failed");   
		return FALSE;   
    }   

//	dcb.EvtChar = 62;
	dcb.EvtChar = EvtChar;

	bSuccess = SetCommState( m_hCom, &dcb );
    if ( !bSuccess ) {   
        CRS232::Close();   
        CRS232::ErrorToString("CRS232::SetEventChar()   SetCommState() failed");   
        return FALSE;   
    }   
	return true;
}
   
   
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::SendData()                                                    ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
   
bool CRS232::SendData( LPCVOID pdata, DWORD   len ) 
{   
	if( m_hCom == NULL )
	{
		sprintf( m_lpszErrorMessage, "m_Com = NULL!\n" );   
		return false;
	}
  	DWORD  dwErrorFlags;
	COMSTAT ComStat;
	ClearCommError( m_hCom, &dwErrorFlags, &ComStat );
	if( ComStat.cbInQue ) 
	{
		::PurgeComm(m_hCom,PURGE_RXCLEAR);
	}

    BOOL  bSuccess;   
    DWORD written = 0;   
   
    if ( len < 1 )   { return false; }
    // create event for overlapped I/O    
    m_ov.hEvent = CreateEvent( NULL,   // pointer to security attributes     
                               FALSE,  // flag for manual-reset event     
                               FALSE,  // flag for initial state     
                               _T(""));    // pointer to event-object name     
    if ( m_ov.hEvent == INVALID_HANDLE_VALUE ) {   
        CRS232::ErrorToString( "CRS232::SendData()   CreateEvent() failed" );   
        return false;   
   }   
    bSuccess = WriteFile( m_hCom,   // handle to file to write to      
                          pdata,    // pointer to data to write to file     
                          len,      // number of bytes to write     
                          &written, // pointer to number of bytes written     
                          &m_ov );  // pointer to structure needed for overlapped I/O    
  // 如果函數執行成功的話檢查written的值為寫入的位元組數，WriteFile函數執行完畢後    
  // 自行填充的，利用此變數的填充值可以用來檢查該函數是否將所有的資料成功寫入串口   
    if ( CRS232::IsNT() ) {   
        bSuccess = GetOverlappedResult( m_hCom, &m_ov, &written, TRUE );   
        if ( !bSuccess ) {   
            CloseHandle( m_ov.hEvent );   
            CRS232::ErrorToString( "CRS232::SendData()   GetOverlappedResult() failed" );   
            return false;   
        }   
    }   
    else if ( len != written ) {   
        CloseHandle( m_ov.hEvent );   
        CRS232::ErrorToString( "CRS232::SendData()   WriteFile() failed" );   
        return false;   
    }   
    CloseHandle( m_ov.hEvent );   
    return true;   
}   
     
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::ReadData()                                                    ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
   
DWORD CRS232::ReadData( LPVOID pdest, DWORD  len, DWORD  dwMaxWait ) 
{   
	if( m_hCom == NULL )
	{
		sprintf( m_lpszErrorMessage, "m_Com = NULL!\n" );   
		return false;
	}
	BOOL  bSuccess;   
    DWORD result = 0,   
          read   = 0, // num read bytes    
          mask   = 0; // a 32-bit variable that receives a mask     
                      // indicating the type of event that occurred    
    if ( len < 1 )  { return(0); }
   // create event for overlapped I/O    
    m_ov.hEvent = CreateEvent( NULL,    // pointer to security attributes     
                               FALSE,   // flag for manual-reset event     
                               FALSE,   // flag for initial state     
                               _T("") );    // pointer to event-object name     
    if ( m_ov.hEvent == INVALID_HANDLE_VALUE ) {   
        ErrorToString( "CRS232::ReadData()   CreateEvent() failed" );   
        return -1;   
    }   
   // Specify here the event to be enabled    
    bSuccess = SetCommMask( m_hCom, EV_RXCHAR );   
    if ( ! bSuccess ) {   
        CloseHandle(m_ov.hEvent);   
        ErrorToString("CRS232::ReadData()   SetCommMask() failed");   
        return -1;   
    }   
   // WaitForSingleObject    
    bSuccess = WaitCommEvent(m_hCom, &mask, &m_ov);   
    if ( !bSuccess ) {   
      int err = GetLastError();   
        if ( err == ERROR_IO_PENDING ) {   
           result = WaitForSingleObject(m_ov.hEvent, dwMaxWait);  //wait dwMaxWait    
                                                // milli seconds before returning    
            if ( result == WAIT_FAILED ) {   
                CloseHandle(m_ov.hEvent);   
                ErrorToString( "CRS232::ReadData()   WaitForSingleObject() failed" );   
            return -1;   
            }   
        }   
    }   


  // The specified event occured?    
   if ( mask & EV_RXCHAR )    
    {   
        bSuccess = ReadFile( m_hCom, // handle of file to read     
                           pdest,  // address of buffer that receives data     
                            len,    // number of bytes to read     
                             &read,  // address of number of bytes read     
                              &m_ov); // address of structure for data     
        if ( CRS232::IsNT() ) {   
           bSuccess = GetOverlappedResult(m_hCom, &m_ov, &read, TRUE);
         if ( !bSuccess ) {   
                CloseHandle( m_ov.hEvent );   
                ErrorToString( "CRS232::ReadData()   GetOverlappedResult() failed" );   
            return -1;   
            }   
        }   
        else if ( !bSuccess ) {   
         CloseHandle(m_ov.hEvent);   
            ErrorToString( "CRS232::ReadData()   ReadFile() failed" );   
         return -1;   
        }   
    }   
    else {   
        CloseHandle(m_ov.hEvent);   
        sprintf( m_lpszErrorMessage, "CRS232::ReadData()   No EV_RXCHAR occured\n" );   
      return -1;   
    }   
    CloseHandle(m_ov.hEvent);   
    return read;   
}  
 
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::ReadData_Event()                                              ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
 
DWORD CRS232::ReadData_Event( LPVOID  pdest, DWORD len, DWORD dwMaxWait )  //Read Data From ComPort , if Receive Event Char(EV_RXFLAG)
{
	if( m_hCom == NULL )
	{
		sprintf( m_lpszErrorMessage, "m_Com = NULL!\n" );   
		return false;
	}
	BOOL  bSuccess;   
    DWORD result = 0,   
          read   = 0, // num read bytes    
          mask   = 0; // a 32-bit variable that receives a mask     
                      // indicating the type of event that occurred    
    if ( len < 1 )   { return(0); }
   // create event for overlapped I/O    
    m_ov.hEvent = CreateEvent( NULL,    // pointer to security attributes     
                               FALSE,   // flag for manual-reset event     
                               FALSE,   // flag for initial state     
                               _T("") );    // pointer to event-object name     
    if ( m_ov.hEvent == INVALID_HANDLE_VALUE ) {   
        ErrorToString( "CRS232::ReadData()   CreateEvent() failed" );   
        return -1;   
    }   
   // Specify here the event to be enabled    
    bSuccess = SetCommMask( m_hCom, EV_RXFLAG );   
    if ( ! bSuccess ) {   
        CloseHandle(m_ov.hEvent);   
        ErrorToString("CRS232::ReadData()   SetCommMask() failed");   
        return -1;   
    }   
   // WaitForSingleObject    
    bSuccess = WaitCommEvent(m_hCom, &mask, &m_ov);   
    if ( !bSuccess ) {   
      int err = GetLastError();   
        if ( err == ERROR_IO_PENDING ) {   
           result = WaitForSingleObject(m_ov.hEvent, dwMaxWait);  //wait dwMaxWait    
                                                // milli seconds before returning    
            if ( result == WAIT_FAILED ) {   
                CloseHandle(m_ov.hEvent);   
                ErrorToString( "CRS232::ReadData()   WaitForSingleObject() failed" );   
            return -1;   
            }   
        }   
    }   


  // The specified event occured?    
	if ( mask & EV_RXFLAG )    
    {   
        bSuccess = ReadFile( m_hCom, // handle of file to read     
                           pdest,  // address of buffer that receives data     
                            len,    // number of bytes to read     
                             &read,  // address of number of bytes read     
                              &m_ov); // address of structure for data     
        if ( CRS232::IsNT() ) {   
           bSuccess = GetOverlappedResult(m_hCom, &m_ov, &read, TRUE);
         if ( !bSuccess ) {   
                CloseHandle( m_ov.hEvent );   
                ErrorToString( "CRS232::ReadData()   GetOverlappedResult() failed" );   
            return -1;   
            }   
        }   
        else if ( !bSuccess ) {   
         CloseHandle(m_ov.hEvent);   
            ErrorToString( "CRS232::ReadData()   ReadFile() failed" );   
         return -1;   
        }   
    }   
    else {   
        CloseHandle(m_ov.hEvent);   
        sprintf( m_lpszErrorMessage, "ReadData()   No EV_RXCHAR occured\n" );   
      return -1;   
    }   
    CloseHandle(m_ov.hEvent);   
	char *pChar = (char *)pdest;
    return read;
}
  
   
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::Close()                                                        ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
   
bool CRS232::Close( void ) {   
   if ( m_hCom > 0 )   
       CloseHandle( m_hCom );   
   m_hCom = 0;    
   return true;
}   
   
   
   
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::IsNT                                                           ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
   
VOID CRS232::ErrorToString( LPCSTR lpszMessage ) {   
    LPVOID lpMessageBuffer;   
    DWORD  error = GetLastError();   
   
    FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |   
                   FORMAT_MESSAGE_FROM_SYSTEM,      // source and processing options    
                   NULL,                            // pointer to message source    
                   error,                           // requested message identifie    
                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // the user default language.    
                   ( LPTSTR ) &lpMessageBuffer,     // pointer to message buffer    
                   0,                               // maximum size of message buffer    
                   NULL);                           // address of array of message inserts     
   
    // and copy it in our error string    
    ::sprintf(m_lpszErrorMessage,"%s: (%d) %s\n", lpszMessage, error, (LPCSTR)lpMessageBuffer);
   
    LocalFree(lpMessageBuffer);   
}   
   
   
   
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  CRS232::IsNT                                                           ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////    
   
BOOL CRS232::IsNT( void ) 
{   
   OSVERSIONINFO osvi;   
       
   osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );   
   GetVersionEx( &osvi );   
   if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )   
      return TRUE;   
   else   
      return FALSE;   
}   
   
   
   
////////////////////////////////////////////////////////////////////////////////    
////                                                                        ////    
////  The End of CRS232                                                      ////    
////                                                                        ////    
////////////////////////////////////////////////////////////////////////////////  
