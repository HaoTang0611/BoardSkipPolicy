// MicroScan_Velocity.cpp: implementation of the MicroScan_Velocity class.
//
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
#include "stdafx.h"
#include "MicroScan_Velocity.h"
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
MicroScan_Velocity::MicroScan_Velocity()
{
	m_CodeLength = -1;	
	m_SectionName = "MicroScan_MINI_Velocity_2D";
	BarCode_CS::m_IsSupportCalibration = true;
	BarCode_CS::m_BarcodeType = BARCODE_MICROSCAN_MINI_VELOCITY;

	m_HeaderChar = "<";
	m_TerminatorChar = ">";
	m_NoRead = "NO READ";

	BarCode_CS::m_MinCodes = 1;
	BarCode_CS::m_MaxCodes = 100;
	BarCode_CS::m_NCodes   = 1;

	const int pCodeId[] = {BARCODE_CODE_CODE39, BARCODE_CODE_CODE128,
							BARCODE_CODE_BC412, BARCODE_CODE_INTERLEAVED_2OF5,
							BARCODE_CODE_CODABAR, BARCODE_CODE_UPC_EAN,
							BARCODE_CODE_CODE93, 
							BARCODE_CODE_DATAMATRIX, BARCODE_CODE_QRCODE,
							BARCODE_CODE_MICRO_QRCODE, BARCODE_CODE_AZTEC_CODE,
							BARCODE_CODE_PDF417, BARCODE_CODE_MICRO_PDF417
							};
		
	const bool pSupperCode[] = {true, true, true, true, true, true, true,
									true, true, true, true, true, true};

	for (int j=0;j < sizeof(pCodeId)/sizeof(pCodeId[0]);++j)
	{
		BarCode_CS::SetDecoder(pCodeId[j], pSupperCode[j]);
		BarCode_CS::EnableCode(pCodeId[j]);		
	}
}
//------------------------------------------------------------------//
MicroScan_Velocity::~MicroScan_Velocity()
{
	SetScanner(false);
	SetTarget(false);
	if ( this->m_RS232COM.IsOpened() )
	{
		this->m_RS232COM.Close();
	}
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::ChangeRecipe()
{
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::Initialize()
{
	if (OnInitialize()) return true;
	
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::Trigger()
{
	if(OnTrigger()) return true;
	
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::ReadData()
{
	if(OnGetData()) return true;
	
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::DoCalibration()	//chia044
{
	if(OnDoCalibration()) return true;
	
	return false;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::TrunONCalibrationMode()	//chia044
{
	//this->StartCalibrationMode();
	this->SetTarget(true);
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::TrunOFFCalibrationMode()	//chia044
{
	//this->EndCalibrationMode();
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::Connected()
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
bool MicroScan_Velocity::SetComPort(int port)
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
int MicroScan_Velocity::GetComPort()
{
	return (m_Port);
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::OnInitialize() 
{
	// TODO: Add your control notification handler code here

	BarCode_CS::m_IsInitialSuccessed = false;
	if(!SetHostPortConnections())	{m_RS232COM.Close();return false; }
	if(!SetHostPortProtocol())		{m_RS232COM.Close();return false; }
	if(!SetCommandEchoStatus())		{m_RS232COM.Close();return false; }	//chia044
	if(!SetScanner(false))			{m_RS232COM.Close();return false; }
	if(!SetSTXETXChar())			{m_RS232COM.Close();return false; }
	if(!SetTriggerMode())			{m_RS232COM.Close();return false; }
	if(!SetExtTriggerEdge())		{m_RS232COM.Close();return false; }	//chia 1000329
	if(!SetCaptureMode())			{m_RS232COM.Close();return false; }	//chia044
	if(!SetEndCycleMode())			{m_RS232COM.Close();return false; }  //0 = Timeout 1 = New Trigger 2 = Timeout & New Trigger 
	if(!SetCheckTimes())			{m_RS232COM.Close();return false; } 
	if(!SetMultiCodeLength(m_CodeLength))   {m_RS232COM.Close();return false; } 
	if(!SetImageOutput(false))		{m_RS232COM.Close();return false; }	//chia044
	if(!SetBackgroundColor())		{m_RS232COM.Close();return false; } 	//chia044
	if(!SetSymbolNum(BarCode_CS::m_NCodes))				{m_RS232COM.Close();return false; }
	if(!SetDataOutputStatus())		{m_RS232COM.Close();return false; } //0 = Disabled  1 = Match  2 = Mismatch  3 = Good Read
	if(!SetMessage())				{m_RS232COM.Close();return false; } 
	if(!SetTriggerChar())			{m_RS232COM.Close();return false; }
	if(!SetScanner(true))			{m_RS232COM.Close();return false; } 
	if(!SetEZButton())				{m_RS232COM.Close();return false; } 	//chia044
	if(!SetTarget(true))			{m_RS232COM.Close();return false; } 
	if(!SetOutputIO())				{m_RS232COM.Close();return false; }	//chia 1000329

	//CString str;
	//str="<>";
	//str.Insert(1, DISABLE_LASER_SCANNINT);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand(DISABLE_LASER_SCANNINT);

	//確認條碼機存在	//chia047
	if( GetIsBarcodeConnect() == false ) 
	{ return false; }

	SaveCurrentSetting();	//chia047
	Sleep(2000);	//chia047

	BarCode_CS::m_IsInitialSuccessed = true;
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::GetIsBarcodeConnect()	//chia047
{
	//確認條碼機存在	//chia005
	CHECK_COMPORT_SUCC();

	//CString str = "";
	//str="<?>";	//chia005
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
bool MicroScan_Velocity::SaveCurrentSetting()	//chia047
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<Z>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("Z");
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetHostPortConnections()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K100,8,0,0,1>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K100,8,0,0,1");

}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetCommandEchoStatus()	//chia044
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K701,0,0,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K701,0,0,0");
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetHostPortProtocol()
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
bool MicroScan_Velocity::SetScanner(bool S)
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//if(S) str="<l1>";	
	//else  str="<l0>";
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand(S ? "11" : "10");
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetSTXETXChar()
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
bool MicroScan_Velocity::SetCaptureMode()	//chia044
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
bool MicroScan_Velocity::SetExtTriggerEdge()	//chia 1000329
{
	CHECK_COMPORT_SUCC();

	//CString str;			
	//str="<K202,0>";
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K202,0");

}
bool MicroScan_Velocity::SetTriggerMode()
{
	CHECK_COMPORT_SUCC();

	//CString str;

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
	case SERIAL_DATA_AND_EDGE:		//serial Data and Edge
		{
			//str="<K200,5>";
			//m_RS232COM.SendData(str, str.GetLength());
			SendCommand("K200,5");
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
bool MicroScan_Velocity::SetEndCycleMode()
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
    
	return SendCommand(str + READTIME);

}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetCheckTimes()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K221,1>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K221,1");
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetSymbolNum(int Symbol)
{
	CHECK_COMPORT_SUCC();	
	
	if(Symbol<m_MinCodes || Symbol>m_MaxCodes)
	{
		SetErrorMsg("SymbolNum Error");
		return false;
	}
	//CString str;
	//str="<K222,,>";
	//str.Insert(7,SEPARATOR);
	//str.Insert(6,SYMBOLNUM);
	//m_RS232COM.SendData(str, str.GetLength());
	if (false == SendCommandF("K222,%d,%s", Symbol, g_SEPARATOR.c_str() ) )
		return false;

	BarCode_CS::m_NCodes = Symbol;
	return true;

}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetDataOutputStatus()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K705,,0>";
	//str.Insert(6,DATAOUTPUTSTATUS);	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommandF("K705,%s,0", DATAOUTPUTSTATUS);
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetMessage()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//no read
	//str="<K714,1,NO READ>";	
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K714,1,NO READ");
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetTriggerChar()
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K201,>";
	//str.Insert(6,TRIGGERCHAR);
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K201," + std::string(TRIGGERCHAR) );
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::OnTrigger() 
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
		break;
	case SERIAL_DATA_AND_EDGE:
		if ( this->m_IsExternalTrigger == true )
		{
			m_IsTrigger = true;
			return true;
		}
		else
		{
			//str="<>";
			//str.Insert(1,TRIGGERCHAR);
			str = m_HeaderChar + TRIGGERCHAR + m_TerminatorChar;
		}
		break;
	}

	return (StartRead(str.c_str(),str.length() ) );
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::StartRead(const char *key, int size)
{
	if ( this->StartDevice() == false ) { return false; }

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
bool MicroScan_Velocity::SendMsg(char pMsg[], int size)
{
	CHECK_COMPORT_SUCC();

	int length = (int)strlen(pMsg);
	if ( length == 0 )
	{
		SetErrorMsg("No Data to Send");
		return false;
	}
//this->m_RS232COM.SendData("<K201,=>",8);
//Sleep(80);
	this->m_RS232COM.SendData(pMsg,size);
	return true;	
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::OnGetData()
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
	for ( i=0; i<NSymbols; i++ )
	{
		if ( this->GetCode_N(i, SingleSymbol, SymbolSize) == false )
		{	return false;	}

		_strupr(SingleSymbol);		
		if (0 == m_NoRead.compare(SingleSymbol) )
		{	return false; }
	}
	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::ReadMsg(char pMsg[], int size)
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
bool MicroScan_Velocity::GetCode_N(int N,char* str,int BufferSize)
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
				if ( j >= BarCode_CS::m_NCodes ) { break; }
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
void MicroScan_Velocity::ClearBuffer()
{
	char temp;	
	Sleep(50);
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::Export()//Kai
{		
    char FileName[128]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName, this->m_INIPath.c_str() );
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
	sprintf(String, "%d", BarCode_CS::m_NCodes);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);

	strcpy(KeyName,"DATAOUTPUTSTATUS");
	strcpy(String,DATAOUTPUTSTATUS);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);
	
	strcpy(KeyName,"CodeLength");
	sprintf(String,"%d", m_CodeLength);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);		

	strcpy(KeyName,"Port");
	sprintf(String, "%d", m_Port);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);


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
//------------------------------------------------------------------//
bool MicroScan_Velocity::Import()
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
//------------------------------------------------------------------//
bool MicroScan_Velocity::EndRead()
{	
	CHECK_COMPORT_SUCC();

	if ( this->m_IsExternalTrigger == true ) { return true; }
	
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
bool MicroScan_Velocity::SetCodeLength(int len)
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
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetImageOutput(const bool IsStore)	//chia044
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
bool MicroScan_Velocity::SetBackgroundColor()	//chia044
{
	//CString str;
	//str="<K451,0>";		//0:White  , 1:Black
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K451,0");
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetOutputIO()	//chia 1000329
{
	CHECK_COMPORT_SUCC();

	//CString str;
	//str="<K810,1,0,1000,0>";
	//m_RS232COM.SendData(str, str.GetLength());
	return SendCommand("K810,1,0,1000,0");
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetEZButton()	//chia044
{
	//CString str;
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
bool MicroScan_Velocity::SetTarget(bool IsON)	//chia044
{
	/*CString str;
	if(IsON==true)
	{ str="<l1>"; }
	else
	{ str="<l0>"; }
	m_RS232COM.SendData(str, str.GetLength());*/
	return SendCommand(IsON ? "11" : "10"); 
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetMultiCodeLength(int len)
{
	//CString str;
	//char Len[4]="";
	//memset(Len, '\0', sizeof(Len));
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
	//sprintf(Len, "%d", len);
	

	//<K479,ECC 200 status,ECC 000 status,ECC 050 status,ECC 080 status, ECC 100 status,ECC 140 status,ECC 120 status,ECC 130 status>
	//BarCode_CS::GetIsEnableDataMatrix();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_DATAMATRIX);
	//if ( IsUseCode == true )
	//{	str="<K479,1,0,0,0,0,0,0,0>"; }
	//else
	//{	str="<K479,0,0,0,0,0,0,0,0>"; }
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand(IsUseCode ? "K479,1,0,0,0,0,0,0,0" : "K479,0,0,0,0,0,0,0,0");

	//Aztec Code 
	//IsUseCode = BarCode_CS::GetIsEnableAztecCode();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_AZTEC_CODE);
	//if ( IsUseCode == true ) 
	//{	str="<K458,1>";	}
	//else
	//{	str="<K458,0>";	}
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand(IsUseCode ? "K458,1" : "K458,0");

	//QR Code
	//IsUseCode = BarCode_CS::GetIsEnableQRCode();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_QRCODE);
	//if ( IsUseCode == true ) 
	//{	str="<K480,1>"; }
	//else
	//{	str="<K480,0>"; }
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand(IsUseCode ? "K480,1" : "K480,0" );

	//Micro QR Code 
	//IsUseCode = BarCode_CS::GetIsEnableMicroQRCode();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_MICRO_QRCODE);
	//if ( IsUseCode == true ) 
	//{	str="<K459,1>"; }
	//else
	//{	str="<K459,0>"; }
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand(IsUseCode ? "K459,1" : "K459,0");	
	
	//Code 39  
	//IsUseCode = BarCode_CS::GetIsEnableCode39();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE39);
	//if ( IsUseCode == true ) 
	//{	str="<K470,1,0,0,0,,,0>";	}
	//else
	//{	str="<K470,0,0,0,0,,,0>";	}
	//str.Insert(15,Len);
	//str.Insert(14,status);
	//m_RS232COM.SendData(str, str.GetLength());	
	SendCommandF("K470,%d,0,0,0,%c,%d,0", (IsUseCode ? 1 : 0), status, len);

	//Code 128  
	//IsUseCode = BarCode_CS::GetIsEnableCode128();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE128);
	//if ( IsUseCode == true ) 
	//{	str="<K474,1,,,0,0,0,,,0,0>";	}
	//else
	//{	str="<K474,0,,,0,0,0,,,0,0>";	}
	//str.Insert(9,Len);
	//str.Insert(8,status);
	//m_RS232COM.SendData(str, str.GetLength());	
	SendCommandF("K474,%d,%c,%d,0,0,0,,,0,0", (IsUseCode ? 1 : 0), status, len);

	//BC412
	//<K481,status,check character output,fixed symbol length status,fixed symbol length> 
	//IsUseCode = BarCode_CS::GetIsEnableBC412();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_BC412);
	//if ( IsUseCode == true ) 
	//{	str="<K481,1,0,,>";	}
	//else
	//{	str="<K481,0,0,,>";	}
	//str.Insert(11,Len);
	//str.Insert(10,status);
	//m_RS232COM.SendData(str, str.GetLength());	
	SendCommandF("K481,%d,0,%c,%d", (IsUseCode ? 1 : 0), status, len);

	//Interleaved 2 of 5 
	//IsUseCode = BarCode_CS::GetIsEnableInterleaved_2of5();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_INTERLEAVED_2OF5);
	//if ( IsUseCode == true ) 
	//{	str="<K472,1,0,0,,,0,>"; }
	//else
	//{	str="<K472,0,0,0,,,0,>"; }
	//str.Insert(16,status);
	//str.Insert(13,Len);
	//str.Insert(12,Len);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K472,%d,0,0,%d,%d,0,%c", (IsUseCode ? 1 : 0), len, len, status);

	//Codabar 
	//IsUseCode = BarCode_CS::GetIsEnableCodabar();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODABAR);
	//if ( IsUseCode == true ) 
	//{	str="<K471,1,1,1,0,,,0,0>"; }
	//else
	//{	str="<K471,0,1,1,0,,,0,0>"; }
	//str.Insert(15,Len);
	//str.Insert(14,status);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K471,%d,1,1,0,%c,%d,0,0", (IsUseCode ? 1 : 0), status, len);
	

	//UPC/EAN 
	//IsUseCode = BarCode_CS::GetIsEnableUPC_EAN();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_UPC_EAN);
	//if ( IsUseCode == true )
	//{	str="<K473,1,1,0,0,,,0,0>";	}
	//else
	//{	str="<K473,0,0,0,0,,,0,0>";	}
	//m_RS232COM.SendData(str, str.GetLength());	
	SendCommandF("K473,%d,1,0,0,,,0,0", (IsUseCode ? 1 : 0) );
	

	//Code 93 
	//IsUseCode = BarCode_CS::GetIsEnableCode93();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE93);
	//if ( IsUseCode == true ) 
	//{	str="<K475,1,,>";	}
	//else
	//{	str="<K475,0,,>";	}
	//str.Insert(9,Len);
	//str.Insert(8,status);
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K475,%d,%c,%d", (IsUseCode ? 1 : 0), status, len);
 
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
	//str="<K484,0,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K484,0,0");

	//DataBar Limited (RSS Limited)	
	//str="<K483,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K483,0");

	//DataBar-14 (RSS-14) 
	//str="<K482,0>";	
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K482,0");

	//PDF417
	// <K476,status,[unused],fixed symbol length status,fixed symbol length,[unused],codeword collection>  
	//IsUseCode = BarCode_CS::GetIsEnablePDF417();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_PDF417);
	//if ( IsUseCode == true ) 
	//{	str="<K476,1,,0,10,,0>";	}
	//else
	//{	str="<K476,0,,0,10,,0>";	}
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K476,%d,,0,10,,0", (IsUseCode ? 1 : 0) );

	//MicroPDF417
	//<K485,status,[unused],fixed symbol length status,fixed symbol length>	
	//IsUseCode = BarCode_CS::GetIsEnableMicroPDF417();
	IsUseCode = BarCode_CS::GetIsEnableCode(BARCODE_CODE_MICRO_PDF417);
	//if ( IsUseCode == true ) 
	//{	str="<K485,1,,0,10>";	}
	//else
	//{	str="<K485,0,,0,10>";	}
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommandF("K485,%d,,0,10", (IsUseCode ? 1 : 0) );

	//Composite
	//<K453,mode,separator status,separator> 	
	//str="<K453,0,0,,>";
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K453,0,0,,");	

	//Narrow Margins/Symbology Identifier 
	//<K450,narrow margins,symbology identifier status> 
	//str="<K450,1,0>";	//0 = Disabled  1 = Enabled  2 = 2D Enhanced  	//chia 1000329
	//m_RS232COM.SendData(str, str.GetLength());
	SendCommand("K450,1,0");

	return true;
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::OnDoCalibration()	//chia044
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

	int Index = -1;
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
				break;
			}

            ErrorCounts ++; 
			Sleep(10); 
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
bool MicroScan_Velocity::StartCalibrationMode()	//chia044
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
//------------------------------------------------------------------//
bool MicroScan_Velocity::EndCalibrationMode()	//chia044
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
bool MicroScan_Velocity::StartDevice()
{
	CHECK_COMPORT_SUCC();

	//CString str="<>";
	//str.Insert(1,ENABLE_LASER_SCANNINT);
	//unsigned int size = (unsigned int)(strlen(str));
	//if ( m_RS232COM.SendData(str, str.GetLength()) == FALSE ) { return false; }	
	return SendCommand(ENABLE_LASER_SCANNINT);
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SetNMultiCodes(int NCodes)//設定條碼數目
{
	if(NCodes<m_MinCodes || NCodes>m_MaxCodes)
	{
		SetErrorMsg("SymbolNum Error");
		return false;
	}

	return this->SetSymbolNum(NCodes);
}
//---------------------------------------------------------------------------------------------------//
bool MicroScan_Velocity::UpdateCodeSetting()
{
	return this->SetMultiCodeLength(m_CodeLength);
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SendCommandF(char *format, ...)
{
	char strCmd[128];

	int nBytes = vsprintf(strCmd, format, ( (char *)&format)+sizeof(char *) );

	return SendCommand(strCmd);
}
//------------------------------------------------------------------//
bool MicroScan_Velocity::SendCommand(const std::string &command)
{
	char str[512];
	sprintf(str, "%s%s%s", m_HeaderChar.c_str(), command.c_str(), m_TerminatorChar.c_str() );
	return (SendMsg(str, strlen(str) ) );
}
//------------------------------------------------------------------//
void MicroScan_Velocity::ExportCodeId(int CodeId)
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
void MicroScan_Velocity::ImportCodeId(int CodeID)
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
bool MicroScan_Velocity::ReadMsg(std::string &rData)
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
bool MicroScan_Velocity::GetData()
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
bool MicroScan_Velocity::IsComportSucc()
{
	if(!m_RS232COM.IsOpened())
	{ 
		SetErrorMsg("No connet");
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------------------------------//