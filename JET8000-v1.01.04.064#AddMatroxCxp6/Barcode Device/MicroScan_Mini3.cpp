// MicroScan_Mini3.cpp: implementation of the MicroScan_Mini3 class.
//
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
#include "stdafx.h"
#include "MicroScan_Mini3.h"
#include "MicroScanGroupDef.h"

//------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CHECK_COMPORT_SUCC() \
		if (false == IsComportSucc() ) \
			return false;

//------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
MicroScan_Mini3::MicroScan_Mini3()
{
	m_CodeLength = -1;
	m_SectionName = "MicroScan_MINI3_2D";

	m_HeaderChar = "<";
	m_TerminatorChar = ">";
	m_NoRead = "NO READ";

	BarCode_CS::m_IsSupportCalibration = true;
	BarCode_CS::m_BarcodeType = BARCODE_MICROSCAN_MINI3;

	BarCode_CS::m_MinCodes = 1;
	BarCode_CS::m_MaxCodes = 6;
	BarCode_CS::m_NCodes   = 1;
}
//------------------------------------------------------------------//
MicroScan_Mini3::~MicroScan_Mini3()
{
	SetScanner(false);
	SetTarget(false);
	if ( this->m_RS232COM.IsOpened() )
	{
		this->m_RS232COM.Close();
	}
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::ChangeRecipe()
{
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::Initialize()
{
	if (OnInitialize()) return true;
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::Trigger()
{
	if(OnTrigger()) return true;
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::ReadData()
{
	//if(OnGetData()) return true;
	if(GetData()) return true;
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::DoCalibration()	//chia044
{
	if(OnDoCalibration()) return true;
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::TrunONCalibrationMode()	//chia044
{
	//this->StartCalibrationMode();
	this->SetTarget(true);
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::TrunOFFCalibrationMode()	//chia044
{
	//this->EndCalibrationMode();
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::Connected()
{
	this->Import();
	this->Export();
	
	const int nBaud = 115200;//115200;
	const int nByteSize = 8;
	const int nParity = 0;
	const int nStopBits = 1;
	
	if ( this->m_RS232COM.IsOpened() )
	{
		this->m_RS232COM.Close();
	}
	//if ( m_RS232COM.Open(nPort, nBaud, nByteSize, nParity, nStopBits)==false  )//nByteSize
	if ( m_RS232COM.Open(m_Port, nBaud, nByteSize, 0, 0)==false  )//nByteSize
	{		
		SetErrorMsg("Open COM Port Fault (%d)", m_Port);
		return false;
	}
	if( this->GetIsBarcodeConnect() == true ) { return true; }
		
	const int Size = 7;

	int BaudList[Size] = {9600, 14400, 19200, 38400, 57600, 115200, 128000};

	for(int i=0 ; i<Size ; i++ )
	{
		this->m_RS232COM.Close();
		Sleep(10);
		if ( m_RS232COM.Open(m_Port, BaudList[i], nByteSize, 0, 0)==false  )//nByteSize
		{
			SetErrorMsg("Can not connect to barcode device!");
			return false;
		}	
		if( this->GetIsBarcodeConnect() == true ) { return true; }
	}

	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetComPort(int port)
{
	//Export INI
	char FileName[256]="",Section[128]="",KeyName[128]="",Text[8]="";
	bool Result = 0x00;	
	strcpy(FileName, this->m_INIPath.c_str() );
	strcpy(KeyName,"Port");
	sprintf(Text,"%i",port);
	m_Port = port;
	strcpy(Section,m_SectionName.c_str() );
	if ( WritePrivateProfileStringA(Section, KeyName, Text, FileName) == TRUE )
	{
		Result = true;
	}
	else { Result = false; }
	return this->Connected();	
}
//------------------------------------------------------------------//
int MicroScan_Mini3::GetComPort()
{
	return (m_Port);
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::OnInitialize() 
{
	// TODO: Add your control notification handler code here

	BarCode_CS::m_IsInitialSuccessed = false;
	if(!SetHostPortConnections())	{m_RS232COM.Close();return false; }
	if(!SetHostPortProtocol())		{m_RS232COM.Close();return false; }
	if(!SetCommandEchoStatus())		{m_RS232COM.Close();return false; }
	if(!SetScanner(false))			{m_RS232COM.Close();return false; }
	if(!SetSTXETXChar())			{m_RS232COM.Close();return false; }
	if(!SetTriggerMode())			{m_RS232COM.Close();return false; }
	if(!SetCaptureMode())			{m_RS232COM.Close();return false; }
	if(!SetEndCycleMode())			{m_RS232COM.Close();return false; }  //0 = Timeout 1 = New Trigger 2 = Timeout & New Trigger 
	if(!SetCheckTimes())			{m_RS232COM.Close();return false; } 
	if(!SetMultiCodeLength(m_CodeLength))   {m_RS232COM.Close();return false; } 
	if(!SetImageOutput(false))		{m_RS232COM.Close();return false; }
	if(!SetBackgroundColor())		{m_RS232COM.Close();return false; }
	if(!SetSymbolNum(BarCode_CS::m_NCodes))			{m_RS232COM.Close();return false; }
	if(!SetDataOutputStatus())		{m_RS232COM.Close();return false; } //0 = Disabled  1 = Match  2 = Mismatch  3 = Good Read
	if(!SetMessage())				{m_RS232COM.Close();return false; } 
	if(!SetTriggerChar())			{m_RS232COM.Close();return false; }
	if(!SetScanner(true))			{m_RS232COM.Close();return false; } 
	if(!SetEZButton())				{m_RS232COM.Close();return false; } 	//chia044
	if(!SetTarget(true))			{m_RS232COM.Close();return false; } 

	//CString str;	
	//str="<>";
	//str.Insert(1, DISABLE_LASER_SCANNINT);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand(DISABLE_LASER_SCANNINT);

	//確認條碼機存在
	if( GetIsBarcodeConnect() == false ) 
	{ return false; }

	SaveCurrentSetting();
	Sleep(2000);

	BarCode_CS::m_IsInitialSuccessed = true;
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::GetIsBarcodeConnect()	//chia047
{
	//確認條碼機存在	//chia005
	CHECK_COMPORT_SUCC();

	//CString str = "";
	//str="<?>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("?");

	char text = '0';
	std::string DefaultChar="<?/";
	int ErrorCounts = 0;
	int index = 0;	
	do
	{
		this->m_RS232COM.ReadSingleChar(text);
		if ( text == DefaultChar[index] )
		{ index++; }
		else
		{ index = 0; }
		if ( ErrorCounts > 100 )
		{ 
		  ClearBuffer(); 
		  SetErrorMsg("Barcode Connect Fault!");
		  return false; 
		}
		Sleep(10);
		ErrorCounts ++;
	}while(index<=2);
	ClearBuffer(); 	

	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SaveCurrentSetting()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<Z>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("Z");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetHostPortConnections()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K100,8,0,0,1>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K100,8,0,0,1");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetCommandEchoStatus()	//chia044
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K701,0,0,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K701,0,0,0");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetHostPortProtocol()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K140,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K140,0");

	//str="<K102,0>";
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K102,0");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetScanner(bool S)
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//if(S) str="<l1>";	
	//else  str="<l0>";
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand(S ? "11" : "10");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetSTXETXChar()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K141,1,>";
	//str.Insert(8,STX1);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K141,1," + STX1);

	//str="<K142,1,>";
	//str.Insert(8,ETX1);
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K142,1," + ETX1);
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetCaptureMode()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<241,1,1,0,1>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("241,1,1,0,1");

	//str="<245,5000>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("245,5000");

	//str="<244,0>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("244,0");

	//str="<233,0>";
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("233,0");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetTriggerMode()
{
	CHECK_COMPORT_SUCC();

	std::string str;
	switch(TRIGGERMODE[0])
	{
	case CONTINUE_READ:		//Continuous Read
		{
			//str="<K200,0>";
			//m_RS232COM.SendData(str, str.GetLength());
			SendCommand("K200,0");
			break;
		}
	case CONTINUE_READ_1:		//Continuous Read 1 Output
		{
			//str="<K200,1>";
			//m_RS232COM.SendData(str, str.GetLength());
			SendCommand("K200,1");
			break;
		}
	case SERIAL_DATA:		//serial Data
		{
			//str="<K200,4>";
			//m_RS232COM.SendData(str, str.GetLength());
			SendCommand("K200,4");

			//str="<K201,>";
			//str.Insert(6,TRIGGERCHAR);			
			//m_RS232COM.SendData(str, str.GetLength());
			SendCommand("K201," + std::string(TRIGGERCHAR) );
			break;
		}

	}
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetEndCycleMode()
{
	CHECK_COMPORT_SUCC();

	std::string str;
    switch(ENDCYCLEMODE[0])
	{
	case TIMEOUT:
		//str="<K220,0,>";
		str = "K220,0,";
		break;
	case NEW_TRIGGER:
		//str="<K220,1,>";
		str = "K220,1,";
		break;
	case TIMEOUT_TRIGGER:
		//str="<K220,2,>";
		str = "K220,2,";
		break;		
	}
	
    //str.Insert(8,READTIME);
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand(str + READTIME);
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetCheckTimes()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K221,1>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K221,1");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetSymbolNum(int Symbol)
{
	CHECK_COMPORT_SUCC();
	
	//char aa='2';	
	if(Symbol < m_MinCodes || Symbol>m_MaxCodes)
	{
		SetErrorMsg("SymbolNum Error");
		return false;
	}
	//CString str;
	//str="<K222,,>";
	//str.Insert(7,SEPARATOR);
	//str.Insert(6,SYMBOLNUM);
	//m_RS232COM.SendData(str, str.GetLength());
	if (false == SendCommandF("K222,%d,%s", Symbol, g_SEPARATOR.c_str()) )
		return false;

	BarCode_CS::m_NCodes = Symbol;
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetDataOutputStatus()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K705,,0>";
	//str.Insert(6,DATAOUTPUTSTATUS);	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommandF("K705,%s,0", DATAOUTPUTSTATUS);
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetMessage()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//no read
	//str="<K714,1,NO READ>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K714,1,NO READ");	
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetTriggerChar()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K201,>";
	//str.Insert(6,TRIGGERCHAR);
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K201," + std::string(TRIGGERCHAR) );
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::OnTrigger() 
{
	BarCode_CS::ClearResultBuffer();

	CHECK_COMPORT_SUCC();

	std::string str;
	switch(TRIGGERMODE[0])
	{
	case CONTINUE_READ:		//Continuous Read
		{
			break;
		}
	case CONTINUE_READ_1:		//Continuous Read 1 Output
		{
			break;
		}
	case SERIAL_DATA:		//serial Data
		{
			//str="<>";
			//str.Insert(1,TRIGGERCHAR);
			str = m_HeaderChar + TRIGGERCHAR + m_TerminatorChar;
			break;
		}
	}

	return (StartRead(str.c_str(),str.length() ) );
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::StartRead(const char *key, int size)
{
	CHECK_COMPORT_SUCC();

	//CString str="<>";
	//str.Insert(1,ENABLE_LASER_SCANNINT);
	//size = (int)strlen(str);
	//if ( m_RS232COM.SendData(str, str.GetLength()) == FALSE ) { return false; }
	if (false == SendCommand(std::string(ENABLE_LASER_SCANNINT) ) )
		return false;

	char Msg[128] = "";	
	for(int i=0;i<size;i++)
	{
		Msg[i]=key[i];
	}	
	 m_IsTrigger=SendMsg(Msg, size);
     return m_IsTrigger;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SendMsg(char pMsg[], int size)
{
	CHECK_COMPORT_SUCC();

	int length = (int)strlen(pMsg);
	if ( length == 0 )
	{
		SetErrorMsg("No Data to Send");
		return false;
	}

	this->m_RS232COM.SendData(pMsg,size);
	return true;	
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::OnGetData()
{
	CHECK_COMPORT_SUCC();

	if(!m_IsTrigger)
	{
		SetErrorMsg("Have not scan trigger");
		return false;
	}
	int i=0;
	const int length = 1024;
	int MaxWaitTime;
	//MaxWaitTime=atoi(READTIME);
	MaxWaitTime = 50;
	MaxWaitTime = atoi(READTIME)+100;
	char Msg[length+32]="";
	memset(Msg, 0x00, length*sizeof(char));
	while(!m_RS232COM.ReadDataWaiting())
	{	
		if(i>MaxWaitTime)
		{
			SetErrorMsg("Read TimeOut");
			return false;
		}
		Sleep(10);
		i++;
	}
	for(i=0;i<=20;i++)
	{
		if ( this->ReadMsg(Msg, length) == false )
		{ return false; }

		if(strlen(Msg)!=0) break;
		if(i==20) 
		{ 
			SetErrorMsg("Read data fault"); 
			return false; 
		} 
      
		Sleep(10);
	}
	m_IsTrigger=false;

	const int WaitTime = 10;
	if ( WaitTime > 0 ) 
	{
		::Sleep(WaitTime);
	}

	const int SymbolSize=256;
	char SingleSymbol[SymbolSize]="";
	const int NSymbols = BarCode_CS::m_NCodes;
	//CString barcode;//Kai
	for ( i=0; i<NSymbols; i++ )
	{
		if ( this->GetCode_N(i, SingleSymbol, SymbolSize) == false )
		{	return false;	}

		//barcode = SingleSymbol;
		//barcode.MakeUpper();
		_strupr(SingleSymbol);
		if (0 == m_NoRead.compare(SingleSymbol) ) 
		{	return false; }
	}
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::ReadMsg(char pMsg[], int size)
{
	memset(pMsg, 0x00, size*sizeof(char));
//	memset(m_Buffer, 0x00, sizeof(m_Buffer));

//	int readTime=atoi(READTIME);

	int idx=0,ErrorCounts=0;
	int MaxErrorCounts = 40;
	this->m_RS232COM.ReadSingleChar(pMsg[0]);

	while (pMsg[0] != STX1[0])
	{	
		if ( ErrorCounts > MaxErrorCounts )
		{ 
		  SetErrorMsg("Can't find STX");
		  ClearBuffer(); 
		  return false; 
		}

		Sleep(10);
		ErrorCounts ++;
		this->m_RS232COM.ReadSingleChar(pMsg[0]);
	}
	
	Sleep(20);
	ErrorCounts = 0;
	idx=1;

	for(;;)
	{
		if (this->m_RS232COM.ReadSingleChar(pMsg[idx]) == 0)
		{
			if (ErrorCounts>MaxErrorCounts)
			{
				SetErrorMsg("Waiting too long"); 
			    ClearBuffer();
			    return false;  
			}

            ErrorCounts ++; 
			Sleep(10); 
			continue;
		}

        if(pMsg[idx] == STX1[0]) 
		{	
			pMsg[idx]='\0';
			break;
		}

        idx++;

		if (idx > size ) 
		{ 
		  SetErrorMsg("Out of Memery"); 
		  ClearBuffer();
		  return false; 
		}

	} 
	int i=0;
	
	for ( i=0;i<idx; i++ )  pMsg[i] = pMsg[i+1];
	pMsg[idx] = '\0';

	BarCode_CS::SetResultBuffer(pMsg);
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::GetCode_N(int N,char* str,int BufferSize)
{	
	if ( N >= BarCode_CS::m_NCodes)
	{	SetErrorMsg("Codes out fo Max "); return false; }
	else if ( N <0 )
	{	SetErrorMsg("Codes out fo Min "); return false; }

	char ResultBuffer[1024] = "";
	strcpy(ResultBuffer, BarCode_CS::GetResultBuffer());

	const int len=(const int)strlen(ResultBuffer);
	if ( len == false ) { SetErrorMsg("Have not data! "); return false; }
	int i=0, j=0, s=0, t=0, tmpeSize=0;
	char tempS[10][256];
	memset(tempS, 0x00, sizeof(tempS));
	do
	{			
			tempS[j][s] = ResultBuffer[t];
			t++;	s++;
			if ( t == len )
			{	tempS[j][s] = '\0';	break; }
			else if ( ResultBuffer[t] == SEPARATOR[0] )
			{	
				tempS[j][s] = '\0';	
				j++;	s=0; t++;
				if ( j >= BarCode_CS::m_NCodes ) { break; }
			}
	} while (1);

	if ( (unsigned int)BufferSize <= ::strlen(tempS[N]) )
	{		
		SetErrorMsg("Out of Buffer size (%i)", ::strlen(tempS[N]) );
		return false;	
	}
	strcpy(str, tempS[N]);
	return true;
}
//------------------------------------------------------------------//
void MicroScan_Mini3::ClearBuffer()
{
	char temp;	
	Sleep(50);
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::Export()//Kai
{
	unsigned long BufferSize = 32;
    DWORD Result = 0x00;
		
    char FileName[128]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName, m_INIPath.c_str() );
	strcpy(Section,m_SectionName.c_str() );
	
	strcpy(KeyName,"TRIGGERCHAR");
	strcpy(String,TRIGGERCHAR);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);
		
	strcpy(KeyName,"TRIGGERMODE");
	strcpy(String,TRIGGERMODE);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);
				
	strcpy(KeyName,"ENDCYCLEMODE");
	strcpy(String,ENDCYCLEMODE);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);
		
	strcpy(KeyName,"READTIME");
	strcpy(String,READTIME);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);	
		
	strcpy(KeyName,"CHECKTIME");
	strcpy(String,CHECKTIME);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);	
		
    strcpy(KeyName,"SYMBOLNUM");
	sprintf(String,"%d", BarCode_CS::m_NCodes);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);	
		
	strcpy(KeyName,"DATAOUTPUTSTATUS");
	strcpy(String,DATAOUTPUTSTATUS);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);	
		
	strcpy(KeyName,"CodeLength");
	sprintf(String,"%d", m_CodeLength);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);	

	strcpy(KeyName,"Port");
	sprintf(String,"%d", m_Port);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);	
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::Import()
{
	//parameter

    unsigned long BufferSize = 8;
    DWORD Result = 0x00;
		
	char FileName[128]="",Section[128]="",KeyName[128]="",DefaultString[8]="",ReturnedString[8]="";
		
	strcpy(FileName, m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );
	
	strcpy(KeyName,"TRIGGERCHAR");
	strcpy(DefaultString,"=");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
    strcpy(TRIGGERCHAR,ReturnedString);
	
	strcpy(KeyName,"TRIGGERMODE");
	strcpy(DefaultString,"4");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
    strcpy(TRIGGERMODE,ReturnedString);

		
	strcpy(KeyName,"ENDCYCLEMODE");
	strcpy(DefaultString,"1");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
    strcpy(ENDCYCLEMODE,ReturnedString);

		
	strcpy(KeyName,"READTIME");
	strcpy(DefaultString,"300");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
    strcpy(READTIME,ReturnedString);

		
	strcpy(KeyName,"CHECKTIME");
	strcpy(DefaultString,"3");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
    strcpy(CHECKTIME,ReturnedString);

		
	strcpy(KeyName,"SYMBOLNUM");
	strcpy(DefaultString,"1");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
	BarCode_CS::m_NCodes = atoi(ReturnedString);

		
	strcpy(KeyName,"DATAOUTPUTSTATUS");
	strcpy(DefaultString,"3");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
    strcpy(DATAOUTPUTSTATUS,ReturnedString);
		
	strcpy(KeyName,"CodeLength");
	strcpy(DefaultString,"-1");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
	//	strcpy(CodeLengthStr,ReturnedString);
  	m_CodeLength=atoi(ReturnedString);

	strcpy(KeyName,"Port");
	strcpy(DefaultString,"1");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
    m_Port = atoi(ReturnedString);

	return true;
} 
//------------------------------------------------------------------//
bool MicroScan_Mini3::EndRead()
{
	CHECK_COMPORT_SUCC();
	
	//CString str="<>";
	//str.Insert(1,DISABLE_LASER_SCANNINT);	
	//int size = 0;	
	//size = (int)strlen(str);	
	//bool IsOK = false;
	//if ( m_RS232COM.SendData(str, str.GetLength()) == 0 ) 
	//{	IsOK = false;	}
	//else { IsOK = true; }
	bool IsOK = SendCommand(DISABLE_LASER_SCANNINT);

	m_IsTrigger=false;

	return IsOK;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetCodeLength(int len)
{
	CHECK_COMPORT_SUCC();

	//Export INI
	m_CodeLength=len;
	char FileName[256]="",Section[128]="",KeyName[128]="",Text[8]="";
	bool Result = 0x00;	
	strcpy(FileName, this->m_INIPath.c_str() );	//Chia2
	strcpy(KeyName,"CodeLength");
	sprintf(Text,"%i",m_CodeLength);
	strcpy(Section,m_SectionName.c_str() );
	if ( WritePrivateProfileStringA(Section, KeyName, Text, FileName) == TRUE )
	{
		Result = true;
	}
	else { Result = false; }


	return SetMultiCodeLength(m_CodeLength);	
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetImageOutput(const bool IsStore)	//chia044
{
//<K739,image output mode,communication port,file format,JPEG quality> 
//image output mod
//0 = Disabled 
//1 = Good Read 
//2 = No Read 
//3 = No Read or Good Read 
//4 = Mismatch 
//communication port
//0 = Host  1 = Auxiliary  2 = USB  
//file format
//0 = Bitmap  1 = JPEG  2 = Binary  
//JPEG quality
//1 to 100 
	//CString str;	
	//if(IsStore)
	//{ str="<K739,3,0,1,20>"; }	//0:disable  1:Store on No Read 
	//else
	//{ str="<K739,0,0,1,20>"; }
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand(IsStore ? "K739,3,0,1,20" : "K739,0,0,1,20");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetBackgroundColor()	//chia044
{
	//CString str;
	//str="<K451,0>";		//0:White  , 1:Black
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K451,0");
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetEZButton()	//chia044
{
	CString str;

	//<K770,global status,default on power-on,load configuration database, save for power-on>
	//str="<K770,1,1,1,1>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K770,1,1,1,1");

	//<K771,position 1 mode,position 2 mode,position 3 mode,position 4 mode> 
	//str="<K771,7,2,1,0>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K771,7,2,1,0");

	//Beep 1 TargetON
	//Beep 2 Calibration
	//Beep 3 Read Ratio
	//Beep 4 Disable
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetTarget(bool IsON)	//chia044
{
	//CString str;
	//if(IsON==true)
	//{ str="<l1>"; }
	//else
	//{ str="<l0>"; }
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand(IsON ? "11" : "10"); 
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetMultiCodeLength(int len)
{
	//CString str;
	//<K479,ECC 200 status,ECC 000 status,ECC 050 status,ECC 080 status, ECC 100 status,ECC 140 status,ECC 120 status,ECC 130 status>
	//str="<K479,1,1,1,1,1,1,1,1>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K479,1,1,1,1,1,1,1,1"); 

	//Aztec Code 
	//str="<K458,1>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K458,1");

	//QR Code
	//str="<K480,1>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K480,1");

	//Micro QR Code 
	//str="<K459,1>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K459,1");

	char status;
	if(len<=0)
	{
		len = 0;
		status='0';
	}
	else
	{
		status='1';
	}

	//char Len[4]="";
	//memset(Len, '\0', sizeof(Len));
	//sprintf(Len, "%d", len);	
	//Code 39  
	//str="<K470,1,0,0,0,,,0>";	
	//str.Insert(15,Len);
	//str.Insert(14,status);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K470,1,0,0,0,%c,%d,0", status, len);

	//Code 128  
	//str="<K474,1,,,0,0,0,,,0,0>";	
	//str.Insert(9,Len);
	//str.Insert(8,status);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K474,1,%c,%d,0,0,0,,,0,0", status, len);

	//BC412
	//<K481,status,check character output,fixed symbol length status,fixed symbol length> 
	//str="<K481,1,0,,>";	
	//str.Insert(11,Len);
	//str.Insert(10,status);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K481,1,0,%c,%d", status, len);

	//Interleaved 2 of 5 
	//str="<K472,1,0,0,,,0,>";
	//str.Insert(16,status);
	//str.Insert(13,Len);
	//str.Insert(12,Len);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K472,1,0,0,%d,%d,0,%c", len, len, status);

	//Codabar 
	//str="<K471,1,1,1,0,,,0,0>";
	//str.Insert(15,Len);
	//str.Insert(14,status);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K471,1,1,1,0,%c,%d,0,0", status, len);

	//UPC/EAN 
	//str="<K473,1,1,0,0,,,0,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K473,1,1,0,0,,,0,0");

	//Code 93 
	//str="<K475,1,,>";	
	//str.Insert(9,Len);
	//str.Insert(8,status);
	//m_RS232COM.SendData(str, str.GetLength());	
	SendCommandF("K475,1,%c,%d", status, len);

	//Pharmacode 
	//str="<K477,0,0,10,4,0,0,400>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K477,0,0,10,4,0,0,400");

	//Postal
	//<K460,postal symbology type,POSTNET status,PLANET status, USPS4CB status,POSTNET allow B and B' fields,Australia Post allow 0 FCC> 
	//str="<K460,0,1,1,1,0,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K460,0,1,1,1,0,0");

	//GS1 DataBar (RSS) 
	//<K484,status,fixed symbol length status,fixed symbol length> 
	//str="<K484,1,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K484,1,0");

	//DataBar Limited (RSS Limited) 
	//str="<K483,1>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K483,1");

	//DataBar-14 (RSS-14) 
	//str="<K482,1>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K482,1");

	//PDF417
	// <K476,status,[unused],fixed symbol length status,fixed symbol length,[unused],codeword collection>  
	//str="<K476,1,,0,10,,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K476,1,,0,10,,0");

	//MicroPDF417
	//<K485,status,[unused],fixed symbol length status,fixed symbol length> 
	//str="<K485,1,,0,10>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K485,1,,0,10");

	//Composite
	//<K453,mode,separator status,separator> 
	//str="<K453,0,0,,>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K453,0,0,,");

	//Narrow Margins/Symbology Identifier 
	//<K450,narrow margins,symbology identifier status> 
	//str="<K450,2,0>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K450,2,0");

	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::OnDoCalibration()	//chia044
{
	CHECK_COMPORT_SUCC();

	SetMultiCodeLength(-1);

	//CString str;
	//str="<@CAL>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("@CAL");

	Sleep(50);

	m_IsTrigger = true;
	int i = 0;
	for( i=0 ; i<20 ; i++)
	{ OnGetData(); }
	m_IsTrigger = false;
	
	char c = '\0';
	char pMsg[256];
	std::string str;
	memset(pMsg, 0x00, 10*sizeof(char));

	int idx=0,ErrorCounts=0;
	int MaxErrorCounts = 3000;
	
	Sleep(3000);
	ErrorCounts = 0;
	idx=0;

	for(;;)
	{
		if (this->m_RS232COM.ReadSingleChar(c) == 0)
		{
			if (ErrorCounts>MaxErrorCounts)
			{
				SetErrorMsg("Waiting too long"); 
			    ClearBuffer();
				//BarCode_CS::SetResultBuffer(ErrMsg.c_str() );
				break;
			}

            ErrorCounts ++; 
			Sleep(30); 
			continue;
		}

		if( c == '\r')
		{ 
			pMsg[idx] = '\0';
			str = pMsg;			
			if(std::string::npos != str.find("Calibration") )
			{ 
				ClearBuffer();
				break; 
			}
			idx = 0;
			continue;
		}

		pMsg[idx] = c;

        idx++;
		if(idx == 256)
		{ idx = 0; }

	} 

	pMsg[0] = '\0';
	BarCode_CS::SetResultBuffer(str.c_str() );

	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::StartCalibrationMode()	//chia044
{
//End of Read Cycle
//0 = Timeout 
//1 = New Trigger 
//2 = Timeout or new Trigger 
//3 = Last Frame 
//4 = Last Frame or New Trigger 
/*
	CString str;
	str="<K220,3>";
	m_RS232COM.SendData(str, str.GetLength());

	this->SetImageOutput(true);
*/
	return true; 
}
bool MicroScan_Mini3::EndCalibrationMode()	//chia044
{
//End of Read Cycle
//0 = Timeout 
//1 = New Trigger 
//2 = Timeout or new Trigger 
//3 = Last Frame 
//4 = Last Frame or New Trigger 
/*
	CString str;
	str="<K220,0>";
	m_RS232COM.SendData(str, str.GetLength());

	this->SetImageOutput(false);
*/
	return true; 
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::StartDevice()
{
	CHECK_COMPORT_SUCC();

	//CString str="<>";
	//str.Insert(1,ENABLE_LASER_SCANNINT);
	//unsigned int size = (unsigned int)(strlen(str));
	//if ( m_RS232COM.SendData(str, str.GetLength()) == FALSE ) { return false; }	
	return SendCommand(ENABLE_LASER_SCANNINT);
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SetNMultiCodes(int NCodes)//設定條碼數目
{
	if(NCodes<m_MinCodes || NCodes>m_MaxCodes)
	{
		SetErrorMsg("SymbolNum Error");
		return false;
	}
	
	return this->SetSymbolNum(NCodes);
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_Mini3::UpdateCodeSetting()
{
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SendCommandF(char *format, ...)
{
	char strCmd[128];

	int nBytes = vsprintf(strCmd, format, ( (char *)&format)+sizeof(char *) );

	return SendCommand(strCmd);
}
//------------------------------------------------------------------//
bool MicroScan_Mini3::SendCommand(const std::string &command)
{
	char str[512];
	sprintf(str, "%s%s%s", m_HeaderChar.c_str(), command.c_str(), m_TerminatorChar.c_str() );
	return (SendMsg(str, strlen(str) ) );
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_Mini3::ReadMsg(std::string &rData)	//從RS232讀取
{
	const int length = 1024;
	char pMsg[length]="";

	int nInQ = 0;
	std::string strMsg("");
	int nCount = 0;
 	while(nInQ = m_RS232COM.ReadDataWaiting() )
	{
		memset(pMsg, 0x00, length*sizeof(char));
		if (m_RS232COM.ReadData(pMsg, length) == 0)
		{
			break;		
		}
		strMsg += std::string(pMsg);		

		Sleep(1);
	};

	if (0 == strMsg.length() )
	{
		//any data is not readed		
		return false;		
	}
	else
	{
		//find header string and drop that
		if (false == FindStringDrop(strMsg, STX1, false) )
			return false;

		//find terminator string and drop that
		if (false == FindStringDrop(strMsg, ETX1, true) )
			return false;
	}

	rData = strMsg;

	BarCode_CS::SetResultBuffer(strMsg.c_str() );

	return true;
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_Mini3::GetData()//取得條碼
{
	CHECK_COMPORT_SUCC();

	if(!m_IsTrigger)
	{
		SetErrorMsg("Have not scan!");
		return false;
	}

	const int length = 1024;
	char Msg[length]="";	

	std::string strMsg;
	if ( false == ReadMsg(strMsg) )
		return false;
	
	m_IsTrigger = false;

	strcpy(Msg, strMsg.c_str());
	_strupr(Msg);
	if (0 == m_NoRead.compare(Msg) )
	{ 
		SetErrorMsg("No Read!");
		return false;
	}	
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_Mini3::IsComportSucc()
{
	if(!m_RS232COM.IsOpened())
	{ 
		SetErrorMsg("No connet");
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------------------------------//