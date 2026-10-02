// DataLogic_1000.cpp: implementation of the DataLogic_1000 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataLogic_1000.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
const int g_kGainMaxCnt = 3;
const int g_kExposureTimeMaxCnt = 3;

DataLogic_1000::DataLogic_1000()
{	
	m_SectionName = "DataLogic_M1000";
	BarCode_CS::m_BarcodeType = BARCODE_DATALOGIC_M1000;
	PreInitial();
}
//------------------------------------------------------------------//
DataLogic_1000::~DataLogic_1000()
{
	ClearBuffer();
	if (IsConnectedSucc() )
	{
		this->m_RS232COM.Close();
	}
}
//------------------------------------------------------------------//
void DataLogic_1000::ClearBuffer()	//睲埃RS232 Buffer
{
	char temp;	
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//------------------------------------------------------------------//
bool DataLogic_1000::Initialize()
{
	if ( Initial_BarcodeReader() == false )	//兵絏诀﹍砞﹚
	{ return false; }
	
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::Connected()
{
	if ( this->OnConnect() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::DoCalibration()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::TrunONCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::TrunOFFCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::Trigger()
{
	if ( this->OnTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::ReadData()
{
	if ( this->GetData() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
/*
bool DataLogic_1000::GetCode_N(int N,char* str, int BufferSize)
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
		ErrMsg.Format("%s (%i)", "Out of Buffer size", ::strlen(tempS[N]));
		return false;
	
	}
	strcpy(str, tempS[N]);
	return true;
}
*/
//------------------------------------------------------------------//
bool DataLogic_1000::SetComPort(int Port)
{
	//Export INI
	char FileName[256]="",Section[128]="",KeyName[128]="",Text[8]="";
	bool Result = 0x00;	
	strcpy(FileName, this->m_INIPath.c_str() );
	strcpy(KeyName,"Port");
	sprintf(Text,"%i",Port);
	m_Port = Port;
	strcpy(Section, m_SectionName.c_str() );
	if ( WritePrivateProfileStringA(Section, KeyName, Text, FileName) == TRUE )
	{
		Result = true;
	}
	else { Result = false; }
	return this->Connected();
}
//------------------------------------------------------------------//
int DataLogic_1000::GetComPort()
{
	return m_Port;
}
//------------------------------------------------------------------//
bool DataLogic_1000::SetCodeLength(int len)
{
	//AOIDataCollect.JETMessage(0, 1, 1, 1,"Not Support!");
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::EndRead()
{
	if ( this->OFFTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::ChangeRecipe()	//Chia2
{
	if ( this->OFFTrigger() == false ) { return false; }

	m_Recipe++;
	if(m_Recipe > m_RecipeMax || m_Recipe < 1)
	{m_Recipe = 1;}
	if ( SetToNewRecipe(m_Recipe) == false ) 
	{
		//AOIDataCollect.JETMessage(0, 1, 1, 1,"Recipe Set error!");
		return false;
	}
	Sleep(50);

	if ( this->OnTrigger() == false ) { return false; }

	return true;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void DataLogic_1000::PreInitial()	
{
	m_nTrigger = 0;	
	m_MyEsc = 0x1B;	//"ESC"
	m_HeaderChar = "<S>";
	m_TerminatorChar = "<E>";
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
bool DataLogic_1000::OnConnect()
{
	if (IsConnectedSucc() )
	{ this->m_RS232COM.Close(); }

	this->Import();
	this->Export();

	const int nPort = m_Port;
	int nBaud = 115200;
	const int nByteSize = 8;
	const int nParity = SERIES_PARITY_NONE;
	const int nStopBits = 0;//SERIES_STOPBITS_15;	

	int Baud[7] = {115200, 57600, 38400, 19200, 9600, 4800, 2400};

	bool IsConnected = false;	
	
	for(int i=0 ; i<7 ; i++ )
	{
		if ( IsConnectedSucc() )
		{ this->m_RS232COM.Close(); }

		nBaud = Baud[i];
		if ( m_RS232COM.Open(nPort, nBaud, nByteSize, SERIES_PARITY_NONE, 0) == false )
		{
			SetErrorMsg("RS232 Connect Fault ( Comport = %d)!", nPort);
			continue; 
		}
		
		if(!IsConnectedSucc()) 
		{ 
			SetErrorMsg("RS232 No Connet"); 
			continue; 
		}

		//弄兵絏诀肚
		this->ClearBuffer();
		//Enter Host Mode
		SendCommand("[C");		
		IsConnected = IsBarcodeConnected();
		if ( IsConnected == false )
		{
			if (IsConnectedSucc() )
			{ this->m_RS232COM.Close(); }

			SetErrorMsg("Barcode Connect fault!"); 
			continue;
		}
		//Exit Host Mode
		SendCommand("[A");
		break;
	}

	if ( IsConnected == true )
	{
		ClearErrorMsg();
	}

	return IsConnected;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Export()
{
	//parameter	
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName, m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	strcpy(KeyName,"Port");
	sprintf(String,"%d", m_Port);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);


    strcpy(KeyName,"SYMBOLNUM");
	sprintf(String,"%d", BarCode_CS::m_NCodes);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);	

	int j=0;
	//write exposure time
	for (j=0;j < g_kExposureTimeMaxCnt; ++j)
	{		
		sprintf(KeyName,"Exposure %02d", j);
		strcpy(String, m_vExposureTime[j].c_str() );
		::WritePrivateProfileStringA(Section, KeyName, String, FileName);
	}

	//write gain value
	for (j=0;j < g_kGainMaxCnt; ++j)
	{
		sprintf(KeyName,"Gain %02d", j);
		strcpy(String, m_vGain[j].c_str() );
		::WritePrivateProfileStringA(Section, KeyName, String, FileName);
	}

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Import()
{
	//parameter
	unsigned long BufferSize = 8;
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",DefaultString[8]="",ReturnedString[8]="";
		
	strcpy(FileName, this->m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str() );

	strcpy(KeyName,"Port");
	strcpy(DefaultString,"4");
	Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
	m_Port = atoi(ReturnedString);
	
		
	strcpy(KeyName,"SYMBOLNUM");
	strcpy(DefaultString,"1");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);    
	BarCode_CS::m_NCodes = ::atoi(ReturnedString);

	int j = 0;
	const std::string pExpTimeDef[g_kExposureTimeMaxCnt] ={"50", "300", "3000"};
	//read exoposure time
	for (j=0;j < g_kExposureTimeMaxCnt; ++j)
	{
		sprintf(KeyName,"Exposure %02d", j);
		strcpy(DefaultString, pExpTimeDef[j].c_str() );
		Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
		m_vExposureTime[j] = std::string(ReturnedString);
	}
	
	//read gain value
	for (j=0;j < g_kGainMaxCnt; ++j)
	{
		sprintf(KeyName,"Gain %02d", j);
		strcpy(DefaultString,"450");
		Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);
		m_vGain[j] = std::string(ReturnedString);
	}

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Disconnect()	//兵絏诀耞絬
{
	if (IsConnectedSucc())
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
bool DataLogic_1000::OnTrigger()	//Trigger ON
{
	BarCode_CS::ClearResultBuffer();
	
	if (false == IsConnectedSucc() )
		return false;

	//m_RS232COM.ClearQueue();	
	this->ClearBuffer();	
	if(m_RS232COM.SendData(m_TriggerChar.c_str(), m_TriggerChar.length() ) )
	{
		m_nTrigger = 2;
		return true;
	}
		
	return false;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::OFFTrigger()	//Trigger OFF
{
	if (false == IsConnectedSucc() )
		return false;

	//m_RS232COM.ClearQueue();	
	this->ClearBuffer();	
	bool IsOK = false;
	if ( m_RS232COM.SendData(m_TriggerOffChar.c_str(), m_TriggerOffChar.length()) == 0 ) 
	{	IsOK = false; }
	else 
	{ IsOK = true; }

	m_nTrigger = 1;	
	return IsOK;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::SetParamCmd(const std::string &rHd)
{
	char str[512] = "";	
	sprintf(str, "%c%s", m_MyEsc, rHd.c_str() );
	return SetParam(str);
}

//---------------------------------------------------------------------//
bool DataLogic_1000::SetParamCmd(const std::string &rHd, int SharpN, int Data)
{
	char str[512] = "";	
	sprintf(str, "%c%s%d%d", m_MyEsc, rHd.c_str(), SharpN, Data);

	return SetParam(str);
}

//---------------------------------------------------------------------//
bool DataLogic_1000::SetParamCmd(const std::string &rHd, int Value)
{
	char str[512] = "";	
	sprintf(str, "%c%s%d", m_MyEsc, rHd.c_str(), Value);
	return SetParam(str);
}

//---------------------------------------------------------------------//
bool DataLogic_1000::SetParamCmd(const std::string &rHd, bool IsUseCode)
{	
	return SetParamCmd(rHd, (IsUseCode ? 1 : 0) );
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Initial_BarcodeReader()	//兵絏诀﹍砞﹚
{
	BarCode_CS::m_IsInitialSuccessed = false;

	if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α

	
	//巨家Α 0: one shot, 1: Continue, 2: Phase Mode
	if ( SetParamCmd("AA", 2) == false )
		return false;						//砞﹚把计

	//Phase ON
	if ( SetParamCmd("AB", 8) == false )
		return false;	

	//Phase Mode, 1: continue
	if ( SetParamCmd("AC", 1) == false )
		return false;	

	//Reading Phase OFF, 8: Aux Port String, 16: Timeout
	if ( SetParamCmd("AD", 8) == false )
		return false;	

	//Phase Time Out, 35-60000
	if ( SetParamCmd("AE", 1000) == false )
		return false;	

	//First Acquisition Setting Used, 01 
	if ( SetParamCmd("AH", 0) == false )
		return false;	

	//Number of Image slots, 1-20
	if ( SetParamCmd("AL", 2) == false )
		return false;	

	//Code Filter Depth, 0-5
	if ( SetParamCmd("AI", 0) == false )
		return false;	

	//Good Read Threshold, 0-100000
	if ( SetParamCmd("AJ", 0) == false )
		return false;	

	//No Read Threshold, 0-100000
	if ( SetParamCmd("AK", 0) == false )
		return false;	

	//TRIGGER DELAY Status, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("n1", 0) == false )
		return false;	

	//Delay Time, 0 to 100000
	if( SetParamCmd("o1", 0) == false )
		return false;

	if ( Initial_Communication() == false ) { return false; }	//兵絏诀硈絬把计砞﹚
	if ( Initial_Calibration() == false ) { return false; }		//兵絏诀タ把计砞﹚
	if ( Initial_2DCode() == false ) { return false; }			//兵絏摸砞﹚
	if ( Initial_1DCode() == false ) { return false; }			//兵絏摸砞﹚
	if ( Initial_MISCELLANEOUS() == false ) { return false; }


	//IMAGE PROCESSING
	//Processing Mode, 0 = Standard,
	if ( SetParamCmd("FA", 0) == false )
		return false;

	//Identical Codes Decoding, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("FB", 0) == false )
		return false;

	//Image Lighting Quality, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("FC", 0) == false )
		return false;

	//Image Mirroring, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("FD", 0) == false )
		return false;

	//Image Processing Timeout, 0-30000
	if ( SetParamCmd("FF", 0) == false )
		return false;

	//POSTAL CODES
	//Status, 0: Disable
	if ( SetParamCmd("IA", 0) == false )
		return false;

	//Local No-Read Message
	//Code Collection Mode, 1 = Within a Phase
	if ( SetParamCmd("JA", 1) == false )
		return false;
	
	if ( SetSymbolNum(BarCode_CS::m_NCodes, false) == false )
		return false;

	if ( SetSymbolSeparator(false) == false )
		return false;

	//Code Collection Filters, 0 = Disabled
	if ( SetParamCmd("JC", 0) == false )
		return false;

	//Status, 0 = Disabled
	if ( SetParamCmd("p1", 0) == false )
		return false;

	//Justification, 0 = Disabled
	if ( SetParamCmd("KA", 0) == false )
		return false;

	//Code Field Cutting, 0 = None
	if ( SetParamCmd("KD", 0) == false )
		return false;

	//No Read message
	if ( SetParamCmd("KF" + m_NoRead) == false )
		return false;

	//Phase-Overrun Message
	if ( SetParamCmd("KI") == false )
		return false;

	if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α

	BarCode_CS::m_IsInitialSuccessed = true;
//	AOIDataCollect.JETMessage(0, 1, 1, 1,"OK");
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Initial_Communication()	//兵絏诀硈絬把计砞﹚
{	
	//Communication Handshake, 0 = None
	if ( SetParamCmd("DF", 0) == false )
		return false;

	//Header
	if ( SetParamCmd("DG" + m_HeaderChar) == false )
		return false;

	//Terminator
	if ( SetParamCmd("DH" + m_TerminatorChar) == false )
		return false;

	//Phase ON String
	if ( SetParamCmd("DI" + m_TriggerChar) == false )
		return false;

	//Phase OFF String
	if( SetParamCmd("DK" + m_TriggerOffChar) == false )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Initial_MISCELLANEOUS()
{
	//MISCELLANEOUS
	//IMAGE BUFFER
	//Status, 0 = Disabled, 1 = Enabled
	if( SetParamCmd("QG", 0) == false )
		return false;

	//VISISET IMAGE SAVING
	//Status, 0 = Disabled, 1 = Enabled
	if( SetParamCmd("QB", 0) == false )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Initial_CalibrationRecipe(int Id)
{	
	//Calibrate
	int Gain[g_kGainMaxCnt] ={0}, ExposureTime[g_kExposureTimeMaxCnt] = {0};

	int j=0;
	//get exposure time
	for (j=0;j < g_kExposureTimeMaxCnt; ++j)
	{
		ExposureTime[j] = atoi(m_vExposureTime[j].c_str() );
	}
	//get gain value
	for (j=0;j < g_kGainMaxCnt;++j)
	{
		Gain[j] = atoi(m_vGain[j].c_str() );
	}

	//Status
	if (false == SetParamCmd("a", Id, 1) )
		return false;

	//Self Tuning, 0 = Disabled, 1 = Enabled	
	if (false == SetParamCmd("G", Id, 1) )
		return false;

	//Self Tuning Mode, 0 = Gain Only, 1 = Exposure Time Only, 2 = Exposure Time And Gain
	if (false == SetParamCmd("H", Id, 2) )
		return false;

	//Selft Tuning Timeout, 1-10000	
	if (false == SetParamCmd("I", Id, 0) )
		return false;

	//Internal Lighting Mode, 3 = High-Power Strobed	
	if (false == SetParamCmd("j", Id, 2) )
		return false;

	//Image Filter, 0 = None, 1 = Erode, 2 = Dilate, 3 = Open, 4 = Close	
	if (false == SetParamCmd("l", Id, 0) )
		return false;

	//Exposure Time Very High-Power, 1-50	
	if (false == SetParamCmd("b", Id, ExposureTime[0]) )
		return false;

	//Exposure Time High-Power, 1-300	
	if (false == SetParamCmd("c", Id, ExposureTime[1]) )
		return false;

	//Exposure Time Medium-Power, 1-32000	
	if (false == SetParamCmd("d", Id, ExposureTime[2]) )
		return false;

	//Gain, 0-800	
	if (false == SetParamCmd("f", Id, 450) )
		return false;

	//Gain Increasing, 0 = X4, 1 = X2, 2 = X1	
	if (false == SetParamCmd("h", Id, 2) )
		return false;

	//Image Polarity Inversion, 0 = Disabled, 1 = Enabled	
	if (false == SetParamCmd("i", Id, 0) )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Initial_Calibration()	//兵絏诀タ把计砞﹚
{
	//Recipe1/////////////////////////////////
	Initial_CalibrationRecipe(1);

	//Recipe2/////////////////////////////////
	Initial_CalibrationRecipe(2);	

	//Recipe3/////////////////////////////////
	Initial_CalibrationRecipe(3);

	//IMAGE PROCESSING SETUP
	//Self Tuning, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("FO", 0) == false )
		return false;

	//Self Tuning Mode
	/*
	0 = Symbologies Only
	1 = Processing Modes Only
	3 = Code Contrast Levels Only
	4 = Image Mirroring Only
	5 = General Purpose
	*/
	if( SetParamCmd("FP", 0) == false )
		return false;

	//Self Tuning Timeout, 0 to 180000(0 = Disabled)
	if( SetParamCmd("FQ", 5000) == false )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Initial_2DCode()	//兵絏摸砞﹚
{
	//2D CODES
	//D G E //L M N //O P U //X Z
	//Code Color, 0 = Black, 1 = White, 2 = Both Colors
	if ( SetParamCmd("GC", 0) == false )
		return false;

	//Code Contrast, 0 = High, 1 = Standard, 2 = Low, 3 = Very Low
	if ( SetParamCmd("FE", 1) == false )
		return false;

	//DATA MATRIX ECC200
	//Status, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("GA", 1) == false )
		return false;

	//QR CODE
	//Status, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("GO", 1) == false )
		return false;

	//Code Color, 0 = Black, 1 = White, 2 = Both Colors
	if ( SetParamCmd("GP", 0) == false )
		return false;

	//AZTEC CODE
	//Status, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("GQ", 1) == false )
		return false;

	//MAXICODE CODE
	//Status, 0 = Disabled, 1 = Enabled
	if ( SetParamCmd("GS", 1) == false )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::Initial_1DCode()	//兵絏摸砞﹚
{
	//1D Codes Setup
	//Minimum code, Height(mm), 1-500
	if ( SetParamCmd("FI") == false )
		return false;

	//code Aspect Ratio, 0:Standard, 1:Low
	if ( SetParamCmd("FK", 0) == false )
		return false;

	//code contrast, 0:Standard, 1:Low
	if ( SetParamCmd("FL", 0) == false )
		return false;

	//PDF417
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HA", 1) == false )
		return false;

	//MICRO PDF417
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hy", 1) == false )
		return false;

	//CODE 128
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HC", 1) == false )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HD", 1) == false )
		return false;

	//EAN 128
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HE", 1) == false )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HF", 1) == false )
		return false;

	//CODE 39
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HG", 1) == false )
		return false;

	//Character Set, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HI", 1) == false )
		return false;

	//Check Digit Status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HJ", 1) == false )
		return false;

	//Check Digit Transmission, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HK", 1) == false )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HL", 1) == false )
		return false;

	//INTERLEAVED 2 OF 5
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HN", 1) == false )
		return false;

	//Check Digit Status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HO", 1) == false )
		return false;

	//Check Digit Transmission, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HP", 1) == false )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HQ", 1) == false )
		return false;

	//Minimum Number of Characters, 0:>4, 1:4, 2:2
	if ( SetParamCmd("HM", 1) == false )
		return false;

	//PHARMACODE
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("HV", 0) == false )
		return false;

	//UPC/EAN FAMILY
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hd", 1) == false )
		return false;

	//ADDON 2 and 5 Status , 0 = Disabled, 1 = Enabled, 2 = Enabled (+AddOn 2 No Quiet Zone)
	if ( SetParamCmd("He", 1) == false )
		return false;

	//Expand UPC E0-E1 symbols , 0:Disabled, 1:Enabled
	if( SetParamCmd("Hf", 1) == false )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if( SetParamCmd("Hg", 1) == false )
		return false;

	//Margin Size (%)(% of narrow module size), 
	/*
		0 = 200
		1 = 250
		2 = 300
		3 = 350
		4 = 400
		5 = 450
	*/
	if ( SetParamCmd("Hw", 1) == false )
		return false;

	//CODABAR
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hi", 1) == false )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hj", 1) == false )
		return false;

	//Check Digit Status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hh", 1) == false )
		return false;

	//Check Digit Transmission, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hk", 1) == false )
		return false;

	//CODE 93
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("H1", 1) == false )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hm", 1) == false )
		return false;

	//GS1 DATABAR EXPANDED (RSS EXPANDED)
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hn", 1) == false )
		return false;

	//GS1 DATABAR EXPANDED STACKED (RSS EXPANDED STACKED)
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hx", 1) == false )
		return false;

	//GS1 DATABAR LIMITED (RSS LIMITED)
		//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Ho", 1) == false )
		return false;

	//GS1 DATABAR (RSS 14)
	//GS1 DATABAR TRUNCATED (RSS 14 TRUNCATED)
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hp", 1) == false )
		return false;

	//GS1 DATABAR STACKED (RSS 14 STACKED)
	//GS1 DATABAR STACKED OMNIDIRECTIONAL (RSS 14 STACKED OMNIDIRECTIONAL)
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hq", 1) == false )
		return false;

	//COMPOSITE
	//status, 0:Disabled, 1:Enabled
	if ( SetParamCmd("Hr", 1) == false )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::SetParam(const std::string &Commond)	//砞﹚兵絏诀把计
{	//砞﹚把计玡ゲ斗秈把计砞﹚家Α

	if( SendData(Commond) == false ){ return false; }		//糶把计
	if( ExitSingleProgram() == false ){ return false;}	//虫把计糶挡
	
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::SendCommand(const std::string &commond)
{
	char str[128];
	sprintf(str, "%c%s", m_MyEsc, commond.c_str() );
	return (SendData(str) );
}
//---------------------------------------------------------------------//
bool DataLogic_1000::SendData(const std::string &commend )	//糶RS232
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
bool DataLogic_1000::ChangeToProgramMode()	//ち传把计砞﹚家Α
{
	if (false == IsConnectedSucc() )
		return false;

	this->ClearBuffer();	
	
	//Enter Host Mode
	SendCommand("[C");
	
	//Enter Programming Mode
	return SendCommand("[B");;

}
//---------------------------------------------------------------------//
bool DataLogic_1000::ExitProgramMode()		//瞒秨把计砞﹚家Α
{
	if (false == IsConnectedSucc() )
		return false;

	this->ClearBuffer();
	
	//Exit Programming Mode and Data Storage
	SendCommand("IA!");

	//Exit Host Mode
	return SendCommand("[A");

}
//---------------------------------------------------------------------//
bool DataLogic_1000::ExitSingleProgram()	//虫把计糶挡
{	
	return SendCommand("IA#");
}
//---------------------------------------------------------------------//
bool DataLogic_1000::IsBarcodeConnected()	//Barcode硈絬絋粄
{
	if (false == IsConnectedSucc() )
		return false;

	int MaxErrorCounts = 40;
	int ErrorCount = 0;
	int i=0;	
	const int length = 1024;	
	std::string pMsg(length, '\0');

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
		{ pMsg[Index] = tempChar; }

        if(tempChar == m_MyEsc) 
		{	
			IsMsg = true;
			Index = 0;
			continue;
		}

        if(tempChar == tmpCR && IsMsg == true ) 
		{	
			pMsg[Index] = '\0';
			break;
		}

		Index++;
	}
	
	if ( pMsg.compare("H") == 0)
	{		
		return true;
	}	
	return false;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::GetData()//眔兵絏
{
	if (false == IsConnectedSucc() )
		return false;

	if(!m_nTrigger)
	{
		SetErrorMsg("Have not scan!");
		return false;
	}

	const int length = 1024;
	char Msg[length]="";	

	std::string strMsg;
	if ( false == ReadMsg(strMsg) )
		return false;

	m_nTrigger = 0;

	m_nTrigger = 0;

	strcpy(Msg, strMsg.c_str());
	_strupr(Msg);
	if (0 == m_NoRead.compare(Msg) )
	{ 
		SetErrorMsg("No Read!");
		return false;
	}
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::ReadMsg(std::string &rData)
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
		//find header string and drop that
		if (false == FindStringDrop(strMsg, m_HeaderChar, false) )
			return false;

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
//---------------------------------------------------------------------//
/*
bool DataLogic_1000::ReadMsg(char pMsg[], int size)//眖RS232弄兵絏
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
	TerminatorLen = (int)strlen(m_TerminatorChar);
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
/*
bool DataLogic_1000::GetHeaderChar()	//絋粄繷絏
{
	char TempChar = 0x00;
	char HeaderChar = 0x00;
	int HeaderLen = 0;
	int index = 0;

	int ErrorCounts=0;
	int MaxErrorCounts = 40;
	
	HeaderLen = (int)strlen(m_HeaderChar);
	if ( HeaderLen <=0 ) { return true; }

	HeaderChar = m_HeaderChar[index];

	do
	{
		this->m_RS232COM.ReadSingleChar(TempChar);	
		if ( TempChar == HeaderChar ) 
		{ 
			index++; 
			if ( index >= HeaderLen ) { break; }
			HeaderChar = m_HeaderChar[index];
		}
		else
		{
			index = 0;
			HeaderChar = m_HeaderChar[index];
			if ( TempChar == HeaderChar ) 
			{ 
				index++; 
				if ( index >= HeaderLen ) { break; }
				HeaderChar = m_HeaderChar[index];
			}
		}
		
		if ( ErrorCounts > MaxErrorCounts )
		{ 
			ErrMsg= "No HeaderChar"; 
			ClearBuffer(); 
			return false; 
		}

		Sleep(10);
		ErrorCounts ++;
		TempChar = 0x00;

	}while( index < HeaderLen );

	return true;
}
*/
//---------------------------------------------------------------------//
bool DataLogic_1000::SetToNewRecipe(int No)
{
	if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α	

	int i = 0; 
	for ( i=1 ; i<=m_RecipeMax ; i++ )
	{
		if (false == SetParamCmd("a", i, (i == No) ? 1 : 0) )
			return false;
	}
	

	if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_1000::StartDevice()
{
	return true;
}
//-----------------------------------------------------------------//
bool DataLogic_1000::SetNMultiCodes(int NCodes)//砞﹚兵絏计ヘ
{
	if ( NCodes >= m_MaxCodes )
	{	SetErrorMsg("Codes out fo Max "); return false; }
	else if ( NCodes <m_MinCodes )
	{	SetErrorMsg("Codes out fo Min "); return false; }


	if(false == IsConnectedSucc() )
		return false;

	return this->SetSymbolNum(NCodes, true);
}
//-----------------------------------------------------------------//
bool DataLogic_1000::SetSymbolNum(int SymbolNum, bool IsChangeToProgramMode)
{
	if( IsChangeToProgramMode == true )
	{
		if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
	}

	//Number of Codes, Range: 1 to 100	
	if (false == SetParamCmd("JB", SymbolNum) )
		return false;

	BarCode_CS::m_NCodes = SymbolNum;

	if( IsChangeToProgramMode == true )
	{
		if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α
	}

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::SetSymbolSeparator(bool IsChangeToProgramMode)
{
	if( IsChangeToProgramMode == true )
	{
		if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
	}

	if(false == IsConnectedSucc() )
		return false;

	//Data Packet Separator string	
	if (false == SetParamCmd(std::string("KG") + SEPARATOR) )
		return false;

	//Data Packet Format (//2 = Code Data)	
	if (false == SetParamCmd("KJ%2") )
		return false;

	if( IsChangeToProgramMode == true )
	{
		if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α
	}
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::IsConnectedSucc()
{
	if(false == m_RS232COM.IsOpened() )
	{ 
		SetErrorMsg("RS232 open Fail!!!");
		return false;
	}
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_1000::FindStringDrop(std::string &rSrcDec, const std::string &rObject, bool Foreward)
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
//------------------------------------------------------------------//
bool DataLogic_1000::UpdateCodeSetting()
{
	if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
	if ( this->Initial_1DCode() == false ) { return false; }
	if ( this->Initial_2DCode() == false ) { return false; }
	if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α
	return true;
}
//---------------------------------------------------------------------------------------------------//