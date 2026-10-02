// Honeywell_3310GHD.cpp: implementation of the Honeywell_3310GHD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Honeywell_1900.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CHECK_COMPORT_SUCC() \
		if (false == IsComportSucc() ) \
			return false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
Honeywell_1900::Honeywell_1900()
{	
	this->m_SectionName = "Honeywell_1900";
	BarCode_CS::m_BarcodeType = BARCODE_HONEYWELL_1900;
	PreInitial();
}
//------------------------------------------------------------------//
Honeywell_1900::~Honeywell_1900()
{
	ClearBuffer();
	if ( this->m_RS232COM.IsOpened() )
	{
		this->m_RS232COM.Close();
	}
}
//------------------------------------------------------------------//
void Honeywell_1900::ClearBuffer()	//睲埃RS232 Buffer
{
	char temp;	
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//------------------------------------------------------------------//
bool Honeywell_1900::Initialize()
{
	if ( Initial_BarcodeReader() == false )	//兵絏诀﹍砞﹚
	{ return false; }
	
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::Connected()
{
	if ( this->OnConnect() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::DoCalibration()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::TrunONCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::TrunOFFCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::Trigger()
{
	if ( this->OnTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::ReadData()
{
	if ( this->GetData() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
/*
bool Honeywell_1900::GetCode_N(int N,char* str, int BufferSize)
{
	if ( N >= BarCode_CS::m_MaxCodes  )
	{	ErrMsg= "Codes out fo Max "; return false; }
	else if ( N <0 )
	{	ErrMsg="Codes out fo Min "; return false; }
	
	char ResultBuffer[1024] = "";
	strcpy(ResultBuffer, BarCode_CS::GetResultBuffer());

	const int len=(const int)strlen(ResultBuffer);
	if ( len == false ) { ErrMsg="Have not data! "; return false; }
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
				if ( j>=BarCode_CS::m_MaxCodes ) { break; }
			}
	} while (1);

	if ( (unsigned int)BufferSize <= ::strlen(tempS[N]) )
	{	
		ErrMsg.Format("%s (%i)", "Out of Buffer size", ::strlen(tempS[N]));
		return false;
	
	}
	strcpy(str, tempS[N]);
	return true;
}
*/
//------------------------------------------------------------------//
bool Honeywell_1900::SetComPort(int nPort)
{
	//Export INI
	char FileName[256]="",Section[128]="",KeyName[128]="",Text[8]="";
	bool Result = 0x00;	
	strcpy(FileName, this->m_INIPath.c_str() );
	strcpy(KeyName,"Port");
	sprintf(Text,"%i",nPort);
	m_Port = nPort;
	strcpy(Section, m_SectionName.c_str() );
	if ( WritePrivateProfileStringA(Section, KeyName, Text, FileName) == TRUE )
	{
		Result = true;
	}
	else { Result = false; }
	return this->Connected();
}
//------------------------------------------------------------------//
int  Honeywell_1900::GetComPort()
{	
	return m_Port;
}
//------------------------------------------------------------------//
bool Honeywell_1900::SetCodeLength(int len)
{
	//AOIDataCollect.JETMessage(0, 1, 1, 1,"Not Support!");
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::EndRead()
{
	if ( this->OFFTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::ChangeRecipe()
{
	return true;
}
//------------------------------------------------------------------//
void Honeywell_1900::PreInitial()	
{
	m_Port = -1;

	m_IsTrigger = FALSE;	
	
	m_StartCommand = 0x16;	//$
	m_EndCommand = 0x0D;    //<CR>
	m_TerminatorChar = "+";//"<E>";
	m_NoRead = "NOREAD";
	m_TriggerChar = 0x54;
	m_TriggerOffChar = 0x55;
	m_Recipe = 0;
	m_RecipeMax = 3;	
	BarCode_CS::m_MinCodes = 1;
	BarCode_CS::m_MaxCodes = 10;
	BarCode_CS::m_NCodes = 1;
	this->m_NSymbolRead = 0;
	m_CodeLength = -1;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::OnConnect()
{
	if ( this->m_RS232COM.IsOpened() )
	{ this->m_RS232COM.Close(); }

	this->Import();
	this->Export();

	const int nPort = m_Port;
	int nBaud = 115200;
	const int nByteSize = 8;
	const int nParity = SERIES_PARITY_NONE;
	const int nStopBits = 0;//SERIES_STOPBITS_15;
	
	int Baud[7] = {115200, 57600, 38400, 19200, 9600, 4800, 2400};

	bool IsConnected = true;	
	
	for(int i=0 ; i<7 ; i++ )
	{
		if ( this->m_RS232COM.IsOpened() )
		{ this->m_RS232COM.Close(); }

		nBaud = Baud[i];
		if ( m_RS232COM.Open(nPort, nBaud, nByteSize, SERIES_PARITY_NONE, 0) == false )
		{
			IsConnected = false;
			SetErrorMsg("RS232 Connect Fault (Com Port = %d)!", nPort);
			continue; 
		}
		
		if(!m_RS232COM.IsOpened()) 
		{ 
			IsConnected = false;
			SetErrorMsg("RS232 No Connet"); 
			continue; 
		}

		//弄兵絏诀肚
		IsConnected = true;
		this->ClearBuffer();	
		if ( IsConnected == false )
		{
			if ( this->m_RS232COM.IsOpened() )
			{ this->m_RS232COM.Close(); }

			SetErrorMsg("Barcode Connect fault!"); 
			continue;
		}	
		break;
	}

	if ( IsConnected == true )
	{
		ClearErrorMsg();
	}

	return IsConnected;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Export()
{
	//parameter	
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName, this->m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	strcpy(KeyName,"Port");
	sprintf(String, "%d", m_Port);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);

	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Import()
{
	//parameter
	unsigned long BufferSize = 8;
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",DefaultString[8]="",ReturnedString[8]="";
		
	strcpy(FileName, m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	strcpy(KeyName,"Port");
	strcpy(DefaultString,"4");
	Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
	m_Port = atoi(ReturnedString);		

	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Disconnect()	//兵絏诀耞絬
{
	if ( this->m_RS232COM.IsOpened() )
	{
		if( ExitProgramMode() == false )
		{
			return false;
		}
		this->m_RS232COM.Close();
	}

	SetErrorMsg("Disconnected!");
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::OnTrigger()	//Trigger ON
{
	BarCode_CS::ClearResultBuffer();
	if(!m_RS232COM.IsOpened()) { SetErrorMsg("No connet"); return false;}

	//m_RS232COM.ClearQueue();	
	this->ClearBuffer();
	char str[128];
	this->m_NSymbolRead = 0;
	sprintf(str, "%c%c%c", m_StartCommand, m_TriggerChar, m_EndCommand);
	if(m_RS232COM.SendData(str, strlen(str) ) )
	{
		m_IsTrigger = true;
		return true;
	}
		
	return false;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::OFFTrigger()	//Trigger OFF
{
	if(!m_RS232COM.IsOpened()) { SetErrorMsg("No connet"); return false;}

	//m_RS232COM.ClearQueue();	
	this->ClearBuffer();
	char str[128];
	sprintf(str, "%c%c%c", m_StartCommand, m_TriggerOffChar, m_EndCommand);
	bool IsOK = false;
	if ( m_RS232COM.SendData(str, strlen(str) ) == 0 ) 
	{	IsOK = false; }
	else 
	{ IsOK = true; }
	
	m_IsTrigger = false;	
	return IsOK;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Initial_BarcodeReader()	//兵絏诀﹍砞﹚
{
	BarCode_CS::m_IsInitialSuccessed = false;
	
	Sleep(400);

	//巨家Α 0: one shot, 1: Continue, 2: Phase Mode
	if( SetParamCmd("CSNRM01") == false )
		return false;

	//砞﹚Trigger ON じ <ST>
	if( SetParamCmd("CSTON3C53543E00000000000000000000000000000000") == false )
		return false;	

	//砞﹚Trigger OFF じ <ET>
	if( SetParamCmd("CSTOF3C45543E00000000000000000000000000000000") == false )
		return false;

	//砞﹚ソ絏じ
	if( SetParamCmd("CLFSU2B00000000000000000000000000000000000000") == false )
		return false;

	if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α

	BarCode_CS::m_IsInitialSuccessed = true;
	this->m_NSymbolRead = 0;
//	AOIDataCollect.JETMessage(0, 1, 1, 1,"OK");
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Initial_Communication()	//兵絏诀硈絬把计砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Initial_MISCELLANEOUS()
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Initial_Calibration()	//兵絏诀タ把计砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Initial_2DCode()	//兵絏摸砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::Initial_1DCode()	//兵絏摸砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::SetParamCmd(const std::string &Commond)
{
	char str[512]="";	
	sprintf(str, "%c%s%c", m_StartCommand, Commond.c_str(), m_EndCommand);
	return SetParam(str);
}
//---------------------------------------------------------------------//
bool Honeywell_1900::SetParam(const std::string & Commond)	//砞﹚兵絏诀把计
{	//砞﹚把计玡ゲ斗秈把计砞﹚家Α

	if( SendData(Commond) == false ){ return false; }		//糶把计
	if( ExitSingleProgram() == false ){ return false;}	//虫把计糶挡
	
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::SendData(const std::string & commend )	//糶RS232
{
	if(m_RS232COM.SendData(commend.c_str(), commend.length() ) )
	{
		Sleep(50);
		return true;
	}

	SetErrorMsg("Send Data Error, %s!", commend.c_str() );
	return false;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::ExitProgramMode()		//瞒秨把计砞﹚家Α
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::ExitSingleProgram()	//虫把计糶挡
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::GetData()//眔兵絏
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

	_strupr(Msg);
	if( 0 == m_NoRead.compare(Msg))
	{ 	
		SetErrorMsg("No Read!");
		return false;
	}
	
	ClearErrorMsg();
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::ReadMsg(char pMsg[], int size)//眖RS232弄兵絏
{
	memset(pMsg, 0x00, size*sizeof(char));

	int idx=0;
	int ErrorCounts=0;
	int EndIdx = 0;
	int MaxErrorCounts = 40;

	if ( GetHeaderChar() == false ) { return false; }
	
	ErrorCounts = 0;
	idx=0;

	char TerminatorChar = 0x00;
	int TerminatorLen = 0;
	int Index = 0;
	bool IsFirstTerminate = true;
	TerminatorLen = (int)m_TerminatorChar.length();
	TerminatorChar = m_TerminatorChar[Index];

	for(;;)
	{
		if (this->m_RS232COM.ReadSingleChar(pMsg[idx]) == 0)
		{
			if (ErrorCounts>MaxErrorCounts)
			{	
				break;
				SetErrorMsg("Waiting too long"); 
			    ClearBuffer();
			    return false;  
			}

            ErrorCounts ++; 
			Sleep(10); 
			continue;
		}


        if(pMsg[idx] == TerminatorChar) 
		{	
			if ( IsFirstTerminate == true)
			{ 
				EndIdx = idx; 
				IsFirstTerminate = false;
			}
			Index++;
			if(Index >= TerminatorLen) { break; }
			TerminatorChar = m_TerminatorChar[Index];
			break;
		}
		else
		{
			if ( IsFirstTerminate == false )
			{
				Index = 0;
				TerminatorChar = m_TerminatorChar[Index];
				if(pMsg[idx] == TerminatorChar) 
				{	
					EndIdx = Index; 
					Index++;
					TerminatorChar = m_TerminatorChar[Index];
					break;
				}
				else
				{
					EndIdx = 0;
					IsFirstTerminate = true;				
				}
			}
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
	
	pMsg[EndIdx] = '\0';
	
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::ReadMsg(std::string &rData)
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

	if (false == strMsg.length() )
	{
		//any data is not readed		
		return false;		
	}
	else
	{
		//find header string and drop that
		
		//find terminator string and drop that
	}

	rData = strMsg;

	BarCode_CS::SetResultBuffer(strMsg.c_str() );

	return true;

}
//---------------------------------------------------------------------//
bool Honeywell_1900::GetHeaderChar()	//絋粄繷絏
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::SetToNewRecipe(int No)
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_1900::StartDevice()
{
	return true;
}
//-----------------------------------------------------------------//
bool Honeywell_1900::SetNMultiCodes(int NCodes)//砞﹚兵絏计ヘ
{	
	if ( NCodes >= m_MaxCodes )
	{
		SetErrorMsg("Codes out fo Max ");
		return false;
	}
	else if ( NCodes <m_MinCodes )
	{
		SetErrorMsg("Codes out fo Min ");
		return false;
	}
	BarCode_CS::m_NCodes = NCodes;
	return true;
}
//-----------------------------------------------------------------//
bool Honeywell_1900::SetSymbolNum(bool IsChangeToProgramMode)
{
/*	if( IsChangeToProgramMode == true )
	{
		if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
	}
*/
	CHECK_COMPORT_SUCC();

	int Symbol=1;//SYMBOLNUM
	//Symbol=atoi(SYMBOLNUM);
	if(Symbol<m_MinCodes || Symbol>m_MaxCodes)
	{
		SetErrorMsg("SymbolNum Error");
		return false;
	}
	if(Symbol == 1)
	{this->ChangeTriggerMode();}
	else{this->ChangeAutoMode();}
/*
	CString str = "";
	str.Format("%c%s%d", m_StartCommand, "JB", Symbol);	//Number of Codes, Range: 1 to 100
	if( SetParam(str) == false ) { return false; }	
*/
	BarCode_CS::m_NCodes = Symbol;
/*
	if( IsChangeToProgramMode == true )
	{
		if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α
	}*/
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::SetSymbolSeparator(bool IsChangeToProgramMode)
{
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_1900::UpdateCodeSetting()
{
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool Honeywell_1900::ChangeAutoMode()
{
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool Honeywell_1900::ChangeTriggerMode()
{
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool Honeywell_1900::IsComportSucc()
{
	if(!m_RS232COM.IsOpened())
	{ 
		SetErrorMsg("No connet");
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------------------------------//