// MS3_CS.cpp: implementation of the MS3_CS class.
//
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
#include "stdafx.h"
#include "MicroScan_MS3.h"
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
MicroScan_MS3::MicroScan_MS3()
{
	m_CodeLength = -1;	//chia005
	this->m_SectionName = "BARCODE_MICROSCAN_MS3";
	BarCode_CS::m_BarcodeType = BARCODE_MICROSCAN_MS3;

	BarCode_CS::m_MinCodes = 1;
	BarCode_CS::m_MaxCodes = 6;
	BarCode_CS::m_NCodes   = 1;

	m_Port = 1;

	m_HeaderChar = "<";
	m_TerminatorChar = ">";
	m_NoRead = "NO READ";

	const int pCodeId[] = {BARCODE_CODE_CODE39, BARCODE_CODE_CODE128,
							BARCODE_CODE_INTERLEAVED_2OF5,
							BARCODE_CODE_CODABAR, BARCODE_CODE_UPC_EAN,
							BARCODE_CODE_CODE93,							
							};
		
	const bool pSupperCode[] = {true, true, true, true, true, true};

	for (int j=0;j < sizeof(pCodeId)/sizeof(pCodeId[0]);++j)
	{
		BarCode_CS::SetDecoder(pCodeId[j], pSupperCode[j]);
		BarCode_CS::EnableCode(pCodeId[j]);
	}	
}
//------------------------------------------------------------------//
MicroScan_MS3::~MicroScan_MS3()
{
	SetMotor(FALSE);
	if ( this->m_RS232COM.IsOpened() )
	{
		this->m_RS232COM.Close();
	}
}
//------------------------------------------------------------------//
void MicroScan_MS3::Show()
{
	//AOIDataCollect.JETMessage(0, 1, 1, 1,m_SectionName.c_str() );
}
//------------------------------------------------------------------//
bool MicroScan_MS3::ChangeRecipe()
{
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::Initialize()
{
	if (OnInitialize()) return true;
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::Trigger()
{
	if(OnTrigger()) return true;
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::ReadData()
{
	//if(OnGetData()) return true;
	if(GetData()) return true;
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::DoCalibration()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::TrunONCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::TrunOFFCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::Connected()
{

	//Open( int nPort, int nBaud , int nByteSize, int nParity, int nStopBits )
/*	const int nPort = 3;
	const int nBaud = 9600;
	const int nByteSize = 8;
	const int nParity = SERIES_PARITY_EVEN;
	const int nStopBits = SERIES_STOPBITS_10;;
*/
	this->Import();
	this->Export();
	
	const int nBaud = 115200;
	const int nByteSize = 8;
	const int nParity = SERIES_PARITY_NONE;
	const int nStopBits = SERIES_STOPBITS_10;
	
	if ( this->m_RS232COM.IsOpened() )
	{
		this->m_RS232COM.Close();
	}

	if ( m_RS232COM.Open(m_Port, nBaud, nByteSize, nParity, nStopBits)==false  )//nByteSize
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
bool MicroScan_MS3::SetComPort(int port)
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
int MicroScan_MS3::GetComPort()
{
	return m_Port;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::OnInitialize() 
{
	// TODO: Add your control notification handler code here
//	 TriggerMode=4;
//	 TimeOutMode=0;
//	 CheckTime=3;
//	 SymbolNum=1;
//	 DataOutputStatus=3;
	BarCode_CS::m_IsInitialSuccessed = false;

	if(!SetHostPortConnections()){m_RS232COM.Close();return false; }
	if(!SetHostPortProtocol()){m_RS232COM.Close();return false; }
	if(!SetCommandEchoStatus())		{m_RS232COM.Close();return false; }	//chia044
	if(!SetMotor(FALSE)) {m_RS232COM.Close();return false; }
	if(!SetSTXETXChar())  {m_RS232COM.Close();return false; }
	if(!SetTriggerMode())  {m_RS232COM.Close();return false; }
	if(!SetExtTriggerEdge())  {m_RS232COM.Close();return false; }	//chia 1000329
	if(!SetEndCycleMode())  {m_RS232COM.Close();return false; }  //0 = Timeout 1 = New Trigger 2 = Timeout & New Trigger 
	if(!SetCheckTimes())    {m_RS232COM.Close();return false; } 
	if(!SetMultiCodeLength(m_CodeLength))   {m_RS232COM.Close();return false; } 
	if(!SetBackgroundColor())   {m_RS232COM.Close();return false; } 	//chia044
	if(!SetSymbolNum(BarCode_CS::m_NCodes))    {m_RS232COM.Close();return false; }
	if(!SetDataOutputStatus()) {m_RS232COM.Close();return false; } //0 = Disabled  1 = Match  2 = Mismatch  3 = Good Read
	if(!SetMessage())  {m_RS232COM.Close();return false; }
	if(!SetTriggerChar())   {m_RS232COM.Close();return false; }
	if(!SetOutputIO())   {m_RS232COM.Close();return false; }	//chia 1000329
	if(!SetMotor(TRUE))   {m_RS232COM.Close();return false; } 			

	//str="<>";
	//str.Insert(1, DISABLE_LASER_SCANNINT);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand(DISABLE_LASER_SCANNINT);	

	//str="<K702,1>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K702,1");

	//確認條碼機存在
	if( GetIsBarcodeConnect() == false ) 
	{ return false; }

	SaveCurrentSetting();
	Sleep(2000);
	this->EndRead();

	BarCode_CS::m_IsInitialSuccessed = true;
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SaveCurrentSetting()	//chia 1000329
{
	CHECK_COMPORT_SUCC();	
	
	return SendCommand("Z");
}
//------------------------------------------------------------------//
bool MicroScan_MS3::GetIsBarcodeConnect()	//chia047
{
	//確認條碼機存在	//chia005
	CHECK_COMPORT_SUCC();
	
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
bool MicroScan_MS3::SetHostPortConnections()
{
	CHECK_COMPORT_SUCC();
	
	return SendCommand("K100,8,0,0,1");
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetCommandEchoStatus()	//chia044
{
	CHECK_COMPORT_SUCC();
	
	return SendCommand("K701,0,0,0");
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetHostPortProtocol()
{
	CHECK_COMPORT_SUCC();

	SendCommand("K140,0");
	
	return SendCommand("K102,0");
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetMotor(bool S)
{
	CHECK_COMPORT_SUCC();
	
	return SendCommand(S ? "KE" : "KF");
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetSTXETXChar()
{
	CHECK_COMPORT_SUCC();
	
	SendCommand("K141,1," + STX1);
	
	return (SendCommand("K142,1," + ETX1) );
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetExtTriggerEdge()	//chia 1000329
{
	CHECK_COMPORT_SUCC();
	
	return (SendCommand("K202,0") );
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetTriggerMode()
{
	CHECK_COMPORT_SUCC();

	switch(TRIGGERMODE[0])
	{
	case CONTINUE_READ:		//Continuous Read
		{			
			SendCommand("K200,0");
			break;
		}
	case CONTINUE_READ_1:		//Continuous Read 1 Output
		{			
			SendCommand("K200,1");
			break;
		}
	case SERIAL_DATA:		//serial Data
		{
			SendCommand("K200,4");
			
			SendCommand("K201," + std::string(TRIGGERCHAR));
			break;
		}
	case SERIAL_DATA_AND_EDGE:			
			SendCommand("K200,5");
	break;

	}
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetEndCycleMode()
{
	CHECK_COMPORT_SUCC();

	std::string str;

    switch(ENDCYCLEMODE[0])
	{
	case TIMEOUT:		
		str="K220,0,";
		break;
	case NEW_TRIGGER:		
		str="K220,1,";
		break;
	case TIMEOUT_TRIGGER:		
		str="K220,2,";
		break;		
	}	
   
	return SendCommand(str + std::string(READTIME) );
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetCheckTimes()
{
	CHECK_COMPORT_SUCC();
	
	return SendCommandF("K221,%s,0", CHECKTIME);
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetOutputIO()	//chia 1000329
{
	CHECK_COMPORT_SUCC();
	
	return SendCommand("K810,1,0,1000,0");
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetSymbolNum(int Symbol)
{
	CHECK_COMPORT_SUCC();
	
	if(Symbol<m_MinCodes || Symbol>m_MaxCodes)
	{
		SetErrorMsg("SymbolNum Error");
		return false;
	}
	
	if (false == SendCommandF("K222,%d,%s", Symbol, g_SEPARATOR.c_str() ) )
		return false;

	BarCode_CS::m_NCodes = Symbol;
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetDataOutputStatus()
{
	CHECK_COMPORT_SUCC();
	
	return SendCommandF("K705,%s,0", DATAOUTPUTSTATUS);
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetMessage()
{
	CHECK_COMPORT_SUCC();

	//no read	
	return SendCommandF("K714,1,%s", m_NoRead.c_str() );
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SetTriggerChar()
{
	CHECK_COMPORT_SUCC();
	
	return SendCommand("K201," + std::string(TRIGGERCHAR) );
}
//------------------------------------------------------------------//
bool MicroScan_MS3::OnTrigger() 
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
	case CONTINUE_READ_1:	//Continuous Read 1 Output
		{
			break;
		}
	case SERIAL_DATA:		//serial Data
		{			
			str = m_HeaderChar + TRIGGERCHAR + m_TerminatorChar;
			break;
		}
	case SERIAL_DATA_AND_EDGE:
		if ( this->m_IsExternalTrigger == true )
		{
			m_IsTrigger = true;
			return true;
		}
		else
		{			
			str = m_HeaderChar + TRIGGERCHAR + m_TerminatorChar;
		}
		break;
	}

	return (StartRead(str.c_str(),str.length() ) );
}
//------------------------------------------------------------------//
bool MicroScan_MS3::StartRead(const char *key, int size)
{
	CHECK_COMPORT_SUCC();
	
	if ( false == SendCommand(ENABLE_LASER_SCANNINT) )
		return false;

	Sleep(50);
	char Msg[128] = "";
	int i=0;
	for(i=0;i<size;i++)
	{
		Msg[i]=key[i];
	}	
	 m_IsTrigger=SendMsg(Msg, size);
     return m_IsTrigger;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SendMsg(char pMsg[], int size)
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
bool MicroScan_MS3::OnGetData()
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
	
	MaxWaitTime = 50;
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
	
	const int NSymbols = BarCode_CS::m_NCodes;
	
	for ( i=0; i<NSymbols; i++ )
	{
		const int SymbolSize=256;
		char SingleSymbol[SymbolSize]="";
		if ( this->GetCode_N(i, SingleSymbol, SymbolSize) == false )
		{	return false;	}

		_strupr(SingleSymbol);		
		if (0 == m_NoRead.compare(SingleSymbol) )
		{	return false; }
	}
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_MS3::ReadMsg(char pMsg[], int size)
{
	memset(pMsg, 0x00, size*sizeof(char));

	int idx,ErrorCounts=0;
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
	
	Sleep(50);
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

        if(pMsg[idx] == ETX1[0]) 
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
bool MicroScan_MS3::GetCode_N(int N,char* str,int BufferSize)
{	
	if ( N >= BarCode_CS::m_NCodes )
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
				if ( j >= BarCode_CS::m_NCodes) { break; }
			}
	} while (1);

	if ( (unsigned int)BufferSize <= ::strlen(tempS[N]) )
	{	
		SetErrorMsg("Out of Buffer size (%d)", ::strlen(tempS[N]) );
		return false;
	
	}
	strcpy(str, tempS[N]);
	return true;
}
//------------------------------------------------------------------//
void MicroScan_MS3::ClearBuffer()
{
	char temp;	
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//------------------------------------------------------------------//
bool MicroScan_MS3::Export()
{
	unsigned long BufferSize = 32;
    DWORD Result = 0x00;
		
    char FileName[128]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName, this->m_INIPath.c_str() );
	strcpy(Section,m_SectionName.c_str() );
	
	strcpy(KeyName,"TRIGGERCHAR");
	strcpy(String,TRIGGERCHAR);
    WritePrivateProfileStringA(Section, KeyName, String, FileName);
        		
	strcpy(KeyName,"TRIGGERMODE");
	strcpy(String,TRIGGERMODE);
    WritePrivateProfileStringA(Section, KeyName, String, FileName);
       		
	strcpy(KeyName,"ENDCYCLEMODE");
	strcpy(String,ENDCYCLEMODE);
    WritePrivateProfileStringA(Section, KeyName, String, FileName);        
	
	strcpy(KeyName,"READTIME");
	strcpy(String,READTIME);
	WritePrivateProfileStringA(Section, KeyName, String, FileName);     
		
	strcpy(KeyName,"CHECKTIME");
	strcpy(String,READTIME);
    WritePrivateProfileStringA(Section, KeyName, String, FileName);        
		
	strcpy(KeyName,"SYMBOLNUM");
	sprintf(String, "%d", BarCode_CS::m_NCodes);
    WritePrivateProfileStringA(Section, KeyName, String, FileName);      
		
	strcpy(KeyName,"DATAOUTPUTSTATUS");
	strcpy(String,DATAOUTPUTSTATUS);
    WritePrivateProfileStringA(Section, KeyName, String, FileName);        
		
	strcpy(KeyName,"CodeLength");
	sprintf(String, "%d", m_CodeLength);
    WritePrivateProfileStringA(Section, KeyName, String, FileName);
//-----------------------------------------------------------------------
	
	strcpy(KeyName,"Port");
	sprintf(String,"%d", m_Port);
    WritePrivateProfileStringA(Section, KeyName, String, FileName);
     
	ExportCodeId(BARCODE_CODE_CODE39);

	ExportCodeId(BARCODE_CODE_CODE128);

	ExportCodeId(BARCODE_CODE_BC412);

	ExportCodeId(BARCODE_CODE_INTERLEAVED_2OF5);

	ExportCodeId(BARCODE_CODE_CODABAR);

	ExportCodeId(BARCODE_CODE_UPC_EAN);

	ExportCodeId(BARCODE_CODE_CODE93);

	ExportCodeId(BARCODE_CODE_PHARMACODE);

	ExportCodeId(BARCODE_CODE_DATAMATRIX);
	
	ExportCodeId(BARCODE_CODE_QRCODE);

	ExportCodeId(BARCODE_CODE_MICRO_QRCODE);

	ExportCodeId(BARCODE_CODE_AZTEC_CODE);

	ExportCodeId(BARCODE_CODE_PDF417);

	ExportCodeId(BARCODE_CODE_MICRO_PDF417);
	
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::Import()
{    
	//parameter
    unsigned long BufferSize = 8;
    DWORD Result = 0x00;
		
	char FileName[128]="",Section[128]="",KeyName[128]="",DefaultString[8]="",ReturnedString[8]="";
		
	strcpy(FileName, this->m_INIPath.c_str() );
	strcpy(Section,m_SectionName.c_str() );
	
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
	m_CodeLength=atoi(ReturnedString);
		
	strcpy(KeyName,"Port");
	strcpy(DefaultString,"1");
	Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
	m_Port = atoi(ReturnedString);	
	

	ImportCodeId(BARCODE_CODE_CODE39);
	
	ImportCodeId(BARCODE_CODE_CODE128);

	ImportCodeId(BARCODE_CODE_BC412);

	ImportCodeId(BARCODE_CODE_INTERLEAVED_2OF5);

	ImportCodeId(BARCODE_CODE_CODABAR);

	ImportCodeId(BARCODE_CODE_UPC_EAN);

	ImportCodeId(BARCODE_CODE_CODE93);

	ImportCodeId(BARCODE_CODE_DATAMATRIX);
	
	ImportCodeId(BARCODE_CODE_QRCODE);

	ImportCodeId(BARCODE_CODE_MICRO_QRCODE);
    
    ImportCodeId(BARCODE_CODE_AZTEC_CODE);

    ImportCodeId(BARCODE_CODE_PDF417);

	ImportCodeId(BARCODE_CODE_MICRO_PDF417);

	return true;
} 
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::EndRead()
{	
	CHECK_COMPORT_SUCC();

	if ( this->m_IsExternalTrigger == true ) { return true; }	

	bool IsOK = SendCommand(DISABLE_LASER_SCANNINT);
	m_IsTrigger=false;
	return IsOK;
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::SetCodeLength(int len)
{
	CHECK_COMPORT_SUCC();

	//Export INI
	m_CodeLength=len;
	char FileName[256]="",Section[128]="",KeyName[128]="",Text[8]="";
	bool Result = 0x00;	
	strcpy(FileName, this->m_INIPath.c_str() );
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
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::SetBackgroundColor()	//chia044
{	
	return SendCommand("K451,0");
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::SetMultiCodeLength(int len)
{
	bool IsUseCode = false;
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

	//Code 39 
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE39);
	SendCommandF("K470,%d,0,0,0,%c,%d,0", (IsUseCode ? 1 : 0), status, len);	

	//Code 128  
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE128);
	SendCommandF("K474,%d,%c,%d,0,0,0,,,0,0", (IsUseCode ? 1 : 0), status, len);
	
	//Interleaved 2 of 5 
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_INTERLEAVED_2OF5);
	SendCommandF("K472,%d,0,0,%d,%d,0,%c", (IsUseCode ? 1 : 0), len, len, status);
	
	//Codabar 
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODABAR);
	SendCommandF("K471,%d,1,1,0,%c,%d,0,0", (IsUseCode ? 1 : 0), status, len);	

	//UPC/EAN
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_UPC_EAN);
	SendCommandF("K473,%d,1,0,0,,,0,0", (IsUseCode ? 1 : 0) );
	
	//Code 93 
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE93);
	SendCommandF("K475,%d,%c,%d", (IsUseCode ? 1 : 0), status, len);

	//Pharmacode
	SendCommand("K477,0,0,10,4,0,0,400");


	//Narrow Margins/Symbology Identifier 
	//<K450,narrow margins,symbology identifier status>		//chia 1000329
	SendCommand("K450,1,0");

	return true;
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::StartDevice()
{
	CHECK_COMPORT_SUCC();
	
	return SendCommand(ENABLE_LASER_SCANNINT);
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::SetNMultiCodes(int NCodes)//設定條碼數目
{
	if(NCodes<m_MinCodes || NCodes>m_MaxCodes)
	{
		SetErrorMsg("SymbolNum Error");
		return false;
	}

	return this->SetSymbolNum(NCodes);
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::UpdateCodeSetting()
{
	return this->SetMultiCodeLength(m_CodeLength);	
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SendCommandF(char *format, ...)
{
	char strCmd[128];

	int nBytes = vsprintf(strCmd, format, ( (char *)&format)+sizeof(char *) );

	return SendCommand(strCmd);
}
//------------------------------------------------------------------//
bool MicroScan_MS3::SendCommand(const std::string &command)
{
	char str[512];
	sprintf(str, "%s%s%s", m_HeaderChar.c_str(), command.c_str(), m_TerminatorChar.c_str() );
	return (SendMsg(str, strlen(str) ) );
}
//------------------------------------------------------------------//
void MicroScan_MS3::ExportCodeId(int CodeId)
{
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName, m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	sprintf(KeyName,"%s Enable", Barcode_API::GetCodeName(CodeId).c_str() );
	if ( BarCode_CS::GetDecoder(CodeId) == true ) 
	{
		sprintf(String,"%d", (true == BarCode_CS::GetIsEnableCode(CodeId) ) ? 1 : 0);
	}
	else
	{
		sprintf(String,"%d", 0);
	}
	
	WritePrivateProfileStringA(Section, KeyName, String, FileName);
}
//------------------------------------------------------------------//
void MicroScan_MS3::ImportCodeId(int CodeID)
{
	//parameter
	unsigned long BufferSize = 8;
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",DefaultString[8]="",ReturnedString[8]="";
		
	strcpy(FileName, m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	if ( BarCode_CS::GetDecoder(CodeID) == true ) 
	{
		sprintf(KeyName,"%s Enable", Barcode_API::GetCodeName(CodeID).c_str() );
		strcpy(DefaultString,"1");
		DWORD Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
		if ( ReturnedString[0] == '1' )
		{ 
			BarCode_CS::EnableCode(CodeID);
		}
		else
		{ 
			BarCode_CS::DisableCode(CodeID);			
		}
	}
	else
	{
		BarCode_CS::DisableCode(CodeID);
	}
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_MS3::ReadMsg(std::string &rData)
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
bool MicroScan_MS3::GetData()
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
bool MicroScan_MS3::IsComportSucc()
{
	if(!m_RS232COM.IsOpened())
	{ 
		SetErrorMsg("No connet");
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------------------------------//