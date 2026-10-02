// DataLogic_GFS4400.cpp: implementation of the DataLogic_1000 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataLogic_GFS4400.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
DataLogic_GFS4400::DataLogic_GFS4400()
{	
	this->m_SectionName = "DataLogic_GFS4400";
	BarCode_CS::m_BarcodeType = BARCODE_DATALOGIC_GFS4400;
	PreInitial();
}
//------------------------------------------------------------------//
DataLogic_GFS4400::~DataLogic_GFS4400()
{
	ClearBuffer();
	if (IsConnectedSucc() )
	{
		this->m_RS232COM.Close();
	}
}
//------------------------------------------------------------------//
void DataLogic_GFS4400::ClearBuffer()	//睲埃RS232 Buffer
{
	char temp;	
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::Initialize()
{
	if ( Initial_BarcodeReader() == false )	//兵絏诀﹍砞﹚
	{ return false; }
	
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::Connected()
{
	if ( this->OnConnect() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::DoCalibration()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::TrunONCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::TrunOFFCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::Trigger()
{
	if ( this->OnTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::ReadData()
{
	if ( this->GetData() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
/*
bool DataLogic_GFS4400::GetCode_N(int N,char* str, int BufferSize)
{
	if ( N >= atoi(SYMBOLNUM)  )
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
				if ( j>=atoi(SYMBOLNUM) ) { break; }
			}
	} while (1);

	if ( (unsigned int)BufferSize <= ::strlen(tempS[N]) )
	{	
		ErrMsg.Format(_T("%s (%i)"), "Out of Buffer size", ::strlen(tempS[N]));
		return false;
	
	}
	strcpy(str, tempS[N]);
	return true;
}
*/
//------------------------------------------------------------------//
bool DataLogic_GFS4400::SetComPort(int nPort)
{
	//Export INI
	char FileName[256]="",Section[128]="",KeyName[128]="",Text[8]="";
	bool Result = 0x00;	
	strcpy(FileName, m_INIPath.c_str() );
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
int DataLogic_GFS4400::GetComPort()
{
	return m_Port;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::SetCodeLength(int len)
{
	//AOIDataCollect.JETMessage(0, 1, 1, 1,"Not Support!");
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::EndRead()
{
	if ( this->OFFTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::ChangeRecipe()
{
	return true;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void DataLogic_GFS4400::PreInitial()	
{
	m_Port = -1;
	m_IsTrigger = false;
	m_StartCommand = 0x24;	//$
	m_EndCommand = 0x0D;    //<CR>

	m_TerminatorChar = "+";//"<E>";
	m_NoRead = "NOREAD";
	m_TriggerChar = "<ST>";
	m_TriggerOffChar = "<ET>";
	m_Recipe = 0;
	m_RecipeMax = 3;	
	BarCode_CS::m_MinCodes = 1;
	BarCode_CS::m_MaxCodes = 100;
	BarCode_CS::m_NCodes = 1;
	m_CodeLength = -1;	//chia005
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::OnConnect()
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
		if ( IsConnectedSucc() )
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
			if ( IsConnectedSucc() )
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
bool DataLogic_GFS4400::Export()
{
	//parameter	
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName, m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	strcpy(KeyName,"Port");
	sprintf(String, "%d",m_Port);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);
    
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::Import()
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
bool DataLogic_GFS4400::Disconnect()	//兵絏诀耞絬
{
	if (IsConnectedSucc() )
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
bool DataLogic_GFS4400::OnTrigger()	//Trigger ON
{
	BarCode_CS::ClearResultBuffer();

	if (false == IsConnectedSucc() )
		return false;	

	//m_RS232COM.ClearQueue();	
	this->ClearBuffer();	
	if(m_RS232COM.SendData(m_TriggerChar.c_str(), m_TriggerChar.length() ) )
	{
		m_IsTrigger = true;
		return true;
	}
		
	return false;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::OFFTrigger()	//Trigger OFF
{
	if(!m_RS232COM.IsOpened())
	{ 
		SetErrorMsg("No connet");
		return false;
	}

	//m_RS232COM.ClearQueue();	
	this->ClearBuffer();	
	bool IsOK = false;
	if ( m_RS232COM.SendData(m_TriggerOffChar.c_str(), m_TriggerOffChar.length() ) == 0 ) 
	{	IsOK = false; }
	else 
	{ IsOK = true; }
	
	m_IsTrigger = false;	
	return IsOK;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::Initial_BarcodeReader()	//兵絏诀﹍砞﹚
{
	BarCode_CS::m_IsInitialSuccessed = false;

	if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
	Sleep(400);
	
	//巨家Α 0: one shot, 1: Continue, 2: Phase Mode
	if ( SetParam("CSNRM01") == false )
		return false;						//砞﹚把计

	//砞﹚Trigger ON じ <ST>
	if ( SetParam("CSTON3C53543E00000000000000000000000000000000") == false )
		return false;						//砞﹚把计
	

	//砞﹚Trigger OFF じ <ET>
	if( SetParam("CSTOF3C45543E00000000000000000000000000000000") == false )
		return false;						//砞﹚把计

	//砞﹚ソ絏じ
	if( SetParam("CLFSU2B00000000000000000000000000000000000000") == false ) { return false; }	

	if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α

	BarCode_CS::m_IsInitialSuccessed = true;
//	AOIDataCollect.JETMessage(0, 1, 1, 1,"OK");
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::Initial_Communication()	//兵絏诀硈絬把计砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::Initial_MISCELLANEOUS()
{
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::Initial_Calibration()	//兵絏诀タ把计砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::Initial_2DCode()	//兵絏摸砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::Initial_1DCode()	//兵絏摸砞﹚
{
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::SetParam(const std::string &Commond)	//砞﹚兵絏诀把计
{	//砞﹚把计玡ゲ斗秈把计砞﹚家Α

	if( SendCommand(Commond) == false ){ return false; }		//糶把计
	if( ExitSingleProgram() == false ){ return false;}	//虫把计糶挡
	
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::SendCommand(const std::string &commend)
{
	char str[128]="";	
	sprintf(str, "%c%s%c", m_StartCommand, commend.c_str() , m_EndCommand);
	return SendData(str);
}

//---------------------------------------------------------------------//
bool DataLogic_GFS4400::SendData(const std::string & commend )	//糶RS232
{
	if(m_RS232COM.SendData(commend.c_str(), commend.length() ) )
	{
		Sleep(50);
		return true;
	}

	SetErrorMsg("Send Data Error, %s! ", commend.c_str() );
	return false;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::ChangeToProgramMode()	//ち传把计砞﹚家Α
{
	if (false == IsConnectedSucc() )
		return false;

	this->ClearBuffer();

	//Enter Host Mode
	return SendCommand("S");

}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::ExitProgramMode()		//瞒秨把计砞﹚家Α
{
	if (false == IsConnectedSucc() )
		return false;

	this->ClearBuffer();
	
	//Exit Programming Mode and Data Storage
	return SendCommand("Ar");

}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::ExitSingleProgram()	//虫把计糶挡
{
	char str[128] = "";
	sprintf(str, "%c%s", m_StartCommand, "Ar");
	return SendData(str);
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::IsBarcodeConnected()	//Barcode硈絬絋粄
{
	if (false == IsConnectedSucc() )
		return false;

	int MaxErrorCounts = 40;
	int ErrorCount = 0;
	int i=0;
	const int length = 1024;
	std::string strMsg(length, '\0');

 	while(!m_RS232COM.ReadDataWaiting())
	{
		Sleep(10);
		ErrorCount++;
		if ( ErrorCount > MaxErrorCounts )
		{
			SetErrorMsg("ReadMsg Error");
			return false;
		}
	};

	char str[length] = "";
	char tmpCR = 0x0D;
	char tmpLF = 0x0A;
	char tempChar = 0x00;
	bool IsMsg = false;
	int Index = 0;
	int idx = 0;
	ErrorCount=0;

	for(;;)
	{
		if (this->m_RS232COM.ReadSingleChar(tempChar) == 0)
		{
			if (ErrorCount>MaxErrorCounts)
			{	
				SetErrorMsg("Waiting too long");
			    ClearBuffer();
			    return false;  
			}

            ErrorCount ++; 
			Sleep(10); 
			continue;
		}

		idx++;
		if (idx > length ) 
		{ 
			SetErrorMsg("Out of Memery");
			ClearBuffer();
			return false; 
		}


		if ( IsMsg == true )
		{ strMsg[Index] = tempChar; }

        if(tempChar == m_StartCommand) 
		{	
			IsMsg = true;
			Index = 0;
			continue;
		}

        if(tempChar == tmpCR && IsMsg == true ) 
		{	
			strMsg[Index] = '\0';
			break;
		}

		Index++;
	}

	
	if (strMsg.compare("H")  == 0)
	{		
		return true;
	}	
	return false;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::GetData()//眔兵絏
{
	if (false == IsConnectedSucc() )
		return false;

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

bool DataLogic_GFS4400::ReadMsg(std::string &rData)	//眖RS232弄
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

	if (strMsg.length() )
	{
		//find terminator string and drop that
		if (false == FindStringDrop(strMsg, m_TerminatorChar, true) )
			return false;
	}
	else
	{
		//any data is not readed		
		return false;
	}

	rData = strMsg;

	BarCode_CS::SetResultBuffer(strMsg.c_str() );

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::FindStringDrop(std::string &rSrcDec, const std::string &rObject, bool Foreward)
{
	size_t pos = 0;

	if (Foreward)
		pos = rSrcDec.find(rObject);
	else
		pos = rSrcDec.rfind(rObject);

	if (pos == std::string::npos)
	{
		//waining for not found the object string
		return false;
	}

	//drop string
	if (Foreward)		
		rSrcDec = rSrcDec.substr(0, pos);
	else
		rSrcDec = rSrcDec.substr(pos + rObject.length());

	return true;
}
//---------------------------------------------------------------------//
/*
bool DataLogic_GFS4400::ReadMsg(char pMsg[], int size)//眖RS232弄兵絏
{
	memset(pMsg, 0x00, size*sizeof(char));

	int idx=0;
	int ErrorCounts=0;
	int EndIdx = 0;
	int MaxErrorCounts = 40;	
	
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
				ErrMsg= "Waiting too long"; 
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
			ErrMsg =  "Out of Memery"; 
			ClearBuffer();
			return false; 
		}

	} 
	int i=0;
	
	pMsg[EndIdx] = '\0';
	
	return true;
}
*/
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::SetToNewRecipe(int No)
{
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_GFS4400::StartDevice()
{
	return true;
}
//-----------------------------------------------------------------//
bool DataLogic_GFS4400::SetNMultiCodes(int NCodes)//砞﹚兵絏计ヘ
{
	m_FullErrMsg = "Not Support!";	
	return false;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::UpdateCodeSetting()
{
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool DataLogic_GFS4400::ChangeAutoMode()
{
	if(!this->ChangeToProgramMode()){return false;}
	Sleep(200);

	if (false == SendCommand("CSNRM02") )
		return false;

	Sleep(200);

	if(!this->ExitProgramMode()){return false;}
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool DataLogic_GFS4400::ChangeTriggerMode()
{
	if(!this->ChangeToProgramMode()){return false;}

	Sleep(200);

	if (false == SendCommand("CSNRM01") )
		return false;

	Sleep(200);

	if(!this->ExitProgramMode()){return false;}
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_GFS4400::IsConnectedSucc()
{
	if(false == m_RS232COM.IsOpened() )
	{ 
		SetErrorMsg("RS232 open Fail!!!");
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------------------------//