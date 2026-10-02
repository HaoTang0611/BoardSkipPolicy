// Honeywell_3310GHD.cpp: implementation of the Honeywell_3310GHD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Honeywell_3310GHD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _SELF_INI_EXPORT (1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
Honeywell_3310GHD::Honeywell_3310GHD()
{	
	BarCode_CS::m_SectionName = "Honeywell_3310GHD";
	BarCode_CS::m_BarcodeType = BARCODE_HONEYWELL_3310GHD;	
	PreInitial();
}

//------------------------------------------------------------------//
Honeywell_3310GHD::~Honeywell_3310GHD()
{
	ClearBuffer();
	Disconnect();
	/*if ( this->m_RS232COM.IsOpened() )
	{
		this->m_RS232COM.Close();
	}*/
}
//------------------------------------------------------------------//
void Honeywell_3310GHD::ClearBuffer()	//睲埃RS232 Buffer
{
	char temp;	
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::Initialize()
{
	if ( Initial_BarcodeReader() == false )	//兵絏诀﹍砞﹚
	{ return false; }
	
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::Connected()
{
	if ( this->OnConnect() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::DoCalibration()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::TrunONCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::TrunOFFCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::Trigger()
{
	if ( this->OnTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::ReadData()
{
	if ( this->GetData() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::SetComPort(int nPort)
{
#if (_SELF_INI_EXPORT)
	//Export INI
	char FileName[256]="",Section[128]="",KeyName[128]="",Text[8]="";
	bool Result = 0x00;	
	strcpy(FileName,m_INIPath.c_str() );
	strcpy(KeyName,"Port");
	m_Port = nPort;
	sprintf(Text,"%i",nPort);
	strcpy(Section, m_SectionName.c_str() );
	if ( WritePrivateProfileStringA(Section, KeyName, Text, FileName) == TRUE )
	{
		Result = true;
	}
	else { Result = false; }
#else
	m_Port = nPort;
#endif
	return this->Connected();
}
//------------------------------------------------------------------//
int Honeywell_3310GHD::GetComPort()
{
	return m_Port;
}

//------------------------------------------------------------------//
bool Honeywell_3310GHD::SetCodeLength(int len)
{
	//AOIDataCollect.JETMessage(0, 1, 1, 1,"Not Support!");
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::EndRead()
{
	if ( this->OFFTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::ChangeRecipe()	//Chia2
{
	return true;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void Honeywell_3310GHD::PreInitial()	
{
	m_Port = 0;
	m_IsTrigger = FALSE;
	m_StartCommand = 0x16;	//$ (SYN symbol)
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
	m_CodeLength = -1;	//chia005
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::OnConnect()
{
	if ( this->m_RS232COM.IsOpened() )
	{ this->m_RS232COM.Close(); }

#if (_SELF_INI_EXPORT)
	this->Import();
	this->Export();
#endif

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
		if ( m_RS232COM.Open(m_Port, nBaud, nByteSize, SERIES_PARITY_NONE, 0) == false )
		{
			IsConnected = false;			
			this->SetErrorMsg("RS232 Connect Fault (Com Port = %d)!", m_Port);
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
bool Honeywell_3310GHD::Export()
{
	//parameter	
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName,m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	strcpy(KeyName,"Port");
	sprintf(String, "%d", m_Port);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);

	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::Import()
{
	//parameter
	unsigned long BufferSize = 8;
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",DefaultString[8]="",ReturnedString[8]="";
		
	strcpy(FileName,m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	strcpy(KeyName,"Port");
	strcpy(DefaultString,"4");
	Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
	m_Port = atoi(ReturnedString);

	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::Disconnect()	//兵絏诀耞絬
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
bool Honeywell_3310GHD::OnTrigger()	//Trigger ON
{
	BarCode_CS::ClearResultBuffer();
	
	if(!m_RS232COM.IsOpened())
	{
		SetErrorMsg("No connet");
		return false;
	}	

	//m_RS232COM.ClearQueue();	
	this->ClearBuffer();
	char str[1024];
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
bool Honeywell_3310GHD::OFFTrigger()	//Trigger OFF
{
	if(!m_RS232COM.IsOpened() )
	{
		SetErrorMsg("No connet");
		return false;
	}

	//m_RS232COM.ClearQueue();	
	this->ClearBuffer();
	char str[1024];
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
bool Honeywell_3310GHD::Initial_BarcodeReader()	//兵絏诀﹍砞﹚
{
	BarCode_CS::m_IsInitialSuccessed = false;

	if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
	Sleep(200);
	
	char str[1024]="";
	
	sprintf(str, "%c%s%c", m_StartCommand, "CSNRM01", m_EndCommand);	//巨家Α 0: one shot, 1: Continue, 2: Phase Mode		
	if( SetParam(str) == false ) { return false; }					//砞﹚把计	

	sprintf(str, "%c%s%c", m_StartCommand, "CSTON3C53543E00000000000000000000000000000000", m_EndCommand);	//砞﹚Trigger ON じ <ST>
	if( SetParam(str) == false ) { return false; }						//砞﹚把计
	

	sprintf(str, "%c%s%c", m_StartCommand, "CSTOF3C45543E00000000000000000000000000000000", m_EndCommand);	//砞﹚Trigger OFF じ <ET>
	if( SetParam(str) == false ) { return false; }						//砞﹚把计

	sprintf(str, "%c%s%c", m_StartCommand, "CLFSU2B00000000000000000000000000000000000000", m_EndCommand);	//砞﹚ソ絏じ
	if( SetParam(str) == false ) { return false; }	
	
	if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α
	
	BarCode_CS::m_IsInitialSuccessed = true;
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::Initial_Communication()	//兵絏诀硈絬把计砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::Initial_MISCELLANEOUS()
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::Initial_Calibration()	//兵絏诀タ把计砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::Initial_2DCode()	//兵絏摸砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::Initial_1DCode()	//兵絏摸砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::SetParam(const std::string &Commond)	//砞﹚兵絏诀把计
{	//砞﹚把计玡ゲ斗秈把计砞﹚家Α

	if( SendData(Commond) == false ){ return false; }		//糶把计
	if( ExitSingleProgram() == false ){ return false;}	//虫把计糶挡

	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::SendData(const std::string &commend )	//糶RS232
{
	if(m_RS232COM.SendData(commend.c_str(), commend.length()))
	{
		Sleep(50);
		return true;
	}			
	
	SetErrorMsg("Send Data Error, %s! ", commend.c_str() );
	return false;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::ChangeToProgramMode()	//ち传把计砞﹚家Α
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::ExitProgramMode()		//瞒秨把计砞﹚家Α
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::ExitSingleProgram()	//虫把计糶挡
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::GetData()//眔兵絏
{
	if (!m_RS232COM.IsOpened())
	{
		SetErrorMsg("No connet");
		return false;
	}

	if (!m_IsTrigger)
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
/*
bool Honeywell_3310GHD::GetData()//眔兵絏
{
	if (!m_RS232COM.IsOpened())
	{
		ErrMsg=_T("No connet");
		return false;
	}

	if (!m_IsTrigger)
	{
		ErrMsg=_T("Have not scan!");
		return false;
	}

	char str[1024];
	const int MaxErrorCounts = 20;
	int ErrorCount = 0;
	int i=0;
	const int length = 1024;
	char Msg[length+32]="";
	memset(Msg, 0x00, length*sizeof(char));	

 	while(!m_RS232COM.ReadDataWaiting())
	{
		Sleep(10);
		ErrorCount++;
		if ( ErrorCount > MaxErrorCounts )
		{
			this->ErrMsg=_T("ReadMsg Error");
			return false;
		}
	};

	int nReLen = 0;
	for(i=0;i<=20;i++)
	{
		if ( 0 == (nReLen = m_RS232COM.ReadData(Msg, length) ) )
		{
			return false;
		}

		if(strlen(Msg)!=0)
			break;

		if(i==20) 
		{
			this->ErrMsg=_T("ReadMsg Error"); 
			return false; 
		} 

		Sleep(5);
	}

	if ( this->m_NSymbolRead > 0 )	
	{		
		sprintf(str,"%s%c%s", this->m_ResultBuffer, SEPARATOR[0], Msg);
		::strcpy(Msg, str);
	}

	BarCode_CS::SetResultBuffer(Msg);
	
	this->m_NSymbolRead ++;

	if ( this->m_NSymbolRead < BarCode_CS::m_NCodes )
	{
		return false;
	}

	m_IsTrigger = false;

	_strupr(Msg);
	if( 0 == m_NoRead.compare(Msg))
	{ 	
		this->ErrMsg=_T("No Read!");
		return false;
	}

	char tmpCR = 0x0D;
	char tmpLF = 0x0A;
	sprintf(str, "%cH%c%c", this->m_StartCommand, tmpCR, tmpLF);
	if ( strcmp(m_ResultBuffer, str) == 0)
	{
		//AOIDataCollect.JETMessage(0, 1, 1, 1,"Success!");
	}
	this->ErrMsg.Empty();
	return true;
}
*/
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::ReadMsg(char pMsg[], int size)//眖RS232弄兵絏
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
	
	pMsg[EndIdx] = '\0';
	
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::ReadMsg(std::string &rData)
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
		
		//find terminator string and drop that
	}

	rData = strMsg;

	BarCode_CS::SetResultBuffer(strMsg.c_str() );

	return true;

}

//---------------------------------------------------------------------//
bool Honeywell_3310GHD::GetHeaderChar()	//絋粄繷絏
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::SetToNewRecipe(int No)
{
	return true;
}
//---------------------------------------------------------------------//
bool Honeywell_3310GHD::StartDevice()
{
	return true;
}
//-----------------------------------------------------------------//
bool Honeywell_3310GHD::SetNMultiCodes(int NCodes)//砞﹚兵絏计ヘ
{
	/*
	if ( NCodes >= m_MaxCodes )
	{
		ErrMsg= _T("Codes out fo Max ");
		return false;
	}
	else if ( NCodes <m_MinCodes )
	{
		ErrMsg="Codes out fo Min ";
		return false;
	}
	BarCode_CS::m_NCodes = NCodes;
	return true;
	*/
	SetErrorMsg("No Support Multi-Code Method");
	return false;
}
//-----------------------------------------------------------------//
bool Honeywell_3310GHD::SetSymbolNum(bool IsChangeToProgramMode)
{
	if ( m_RS232COM.IsOpened() == false )
	{
		SetErrorMsg("No Connected");
		return false;
	}
	int Symbol=1;//SYMBOLNUM

	if(Symbol<m_MinCodes || Symbol>m_MaxCodes)
	{
		SetErrorMsg("SymbolNum Error");
		return false;
	}

	if(Symbol == 1)
	{
		this->ChangeTriggerMode();
	}
	else
	{
		this->ChangeAutoMode();
	}

	BarCode_CS::m_NCodes = Symbol;

	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::SetSymbolSeparator(bool IsChangeToProgramMode)
{
	return true;
}
//------------------------------------------------------------------//
bool Honeywell_3310GHD::UpdateCodeSetting()
{
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool Honeywell_3310GHD::ChangeAutoMode()
{
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool Honeywell_3310GHD::ChangeTriggerMode()
{
	return true;
}
//---------------------------------------------------------------------------------------------------//