#ifndef _RS232_H_
#define _RS232_H_


class CRS232 
{   
public:    
    CRS232( void );   
    ~CRS232();   
    LPSTR   GetErrorMessage( void );   

	// init COM port
	bool Open(int nPort = 1, int nBaud = CBR_9600 , int nByteSize = 8, int nParity = NOPARITY, int nStopBits = ONESTOPBIT);
	bool SetRWTimeouts(COMMTIMEOUTS CommTimeOuts);
	bool SetEventChar(char EvtChar);
	bool Close();									// close COM port

    bool    SendData( LPCVOID pdata, DWORD len );							    // Send string to COM port
    DWORD   ReadData( LPVOID  pdest, DWORD len, DWORD dwMaxWait = 500 );        //Read Data From ComPort , if Anychar Receive(EV_RXCHAR)
    DWORD   ReadData_Event( LPVOID  pdest, DWORD len, DWORD dwMaxWait = 500 );  //Read Data From ComPort , if Receive Event Char(EV_RXFLAG)
   
private:   
   
    VOID    ErrorToString( LPCSTR lpszMessage );   
    BOOL    IsNT( void );   
    char        m_lpszErrorMessage[256];   
    HANDLE      m_hCom;   // 串口控制碼    
    OVERLAPPED  m_ov;     // 包含非同步輸入輸出操作資訊的結構    
}; 

#endif
