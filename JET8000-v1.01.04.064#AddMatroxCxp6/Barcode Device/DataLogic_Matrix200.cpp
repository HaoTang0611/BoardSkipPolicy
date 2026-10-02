#include "stdafx.h"
#include "DataLogic_Matrix200.h"
#include "DataLogicGroupDef.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------//
const int g_kGainMaxCnt = 3;
const int g_kExposureTimeMaxCnt = 3;

DataLogic_Matrix200::DataLogic_Matrix200()
{		
	BarCode_CS::m_SectionName = "DataLogic_Matrix200";
	BarCode_CS::m_BarcodeType = BARCODE_DATALOGIC_MATRIX_200;

	const int pCodeId[] = {BARCODE_CODE_CODE39, BARCODE_CODE_CODE128,
							BARCODE_CODE_BC412, BARCODE_CODE_INTERLEAVED_2OF5,
							BARCODE_CODE_CODABAR, BARCODE_CODE_UPC_EAN,
							BARCODE_CODE_CODE93, 
							BARCODE_CODE_DATAMATRIX, BARCODE_CODE_QRCODE,
							BARCODE_CODE_MICRO_QRCODE, BARCODE_CODE_AZTEC_CODE,
							BARCODE_CODE_PDF417, BARCODE_CODE_MICRO_PDF417
							};
		
	const bool pSupperCode[] = {true, true, false, true, true, true, true,
									true, true, true, true, true, true};

	for (int j=0;j < sizeof(pCodeId)/sizeof(pCodeId[0]);++j)
	{
		BarCode_CS::SetDecoder(pCodeId[j], pSupperCode[j]);
		BarCode_CS::EnableCode(pCodeId[j]);		
	}	
	
	PreInitial();
}
//------------------------------------------------------------------//
DataLogic_Matrix200::~DataLogic_Matrix200()
{
	ClearBuffer();
	if ( this->m_RS232COM.IsOpened() )
	{
		this->m_RS232COM.Close();
	}
}
//------------------------------------------------------------------//
void DataLogic_Matrix200::ClearBuffer()	//睲埃RS232 Buffer
{
	char temp;	
	while(this->m_RS232COM.ReadSingleChar(temp) != 0)
	{}
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::Initialize()
{
	if ( DoInitial_BarcodeReader() == false )	//兵絏诀﹍砞﹚
	{ return false; }
	
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::Connected()
{
	if ( this->OnConnect() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::DoCalibration()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::TrunONCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::TrunOFFCalibrationMode()	//chia044
{
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::Trigger()
{
	if ( this->OnTrigger() == false )
	{ return false; }	
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::ReadData()
{
	if ( this->GetData() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::SetComPort(int nPort)
{
	//Export INI
	char FileName[256]="",Section[128]="",KeyName[128]="",Text[8]="";
	bool Result = 0x00;	
	strcpy(FileName, this->m_INIPath.c_str() );
	strcpy(KeyName,"Port");
	sprintf(Text,"%i",nPort);
	this->m_Port = nPort;
	strcpy(Section, m_SectionName.c_str() );
	if ( WritePrivateProfileStringA(Section, KeyName, Text, FileName) == TRUE )
	{
		Result = true;
	}
	else { Result = false; }
	return this->Connected();
}
//------------------------------------------------------------------//
int  DataLogic_Matrix200::GetComPort()
{
	return (m_Port);
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::SetCodeLength(int len)
{
	//::AfxMessageBox("Not Support!");
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::EndRead()
{
	if ( this->OFFTrigger() == false )
	{ return false; }

	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::ChangeRecipe()	//Chia2
{
	if ( this->OFFTrigger() == false ) { return false; }

	m_Recipe++;
	if(m_Recipe > m_RecipeMax || m_Recipe < 1)
	{m_Recipe = 1;}
	if ( SetToNewRecipe(m_Recipe) == false ) 
	{
		SetErrorMsg("Recipe Set error!");
		return false;
	}
	Sleep(50);

	if ( this->OnTrigger() == false ) { return false; }

	return true;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void DataLogic_Matrix200::PreInitial()	
{
	m_Port = -1;	
	m_vGain.resize(g_kGainMaxCnt, "");
	m_vExposureTime.resize(g_kExposureTimeMaxCnt, "");

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

	this->m_ProgramMode = DATALOGIC_MODE_LEVEL_NONE;
	this->m_DelayTime = 100;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::OnConnect()
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

	bool IsConnected = false;

	for(int i=0 ; i<7 ; i++ )
	{
		if ( this->m_RS232COM.IsOpened() )
		{ this->m_RS232COM.Close(); }

		nBaud = Baud[i];
		if ( m_RS232COM.Open(nPort, nBaud, nByteSize, SERIES_PARITY_NONE, 0) == false )
		{
			SetErrorMsg("RS232 Connect Fault (Comport = %d)!", nPort);
			continue; 
		}
		
		if(!m_RS232COM.IsOpened()) 
		{ 
			SetErrorMsg("RS232 No Connet"); 
			continue; 
		}

		//弄兵絏诀肚
		this->ClearBuffer();		
		if (false == (IsConnected = SendDataCmd("[C", "H") ) )
		{
			//Exit Host Mode			
			if (false == (IsConnected = SendDataCmd("[A", "X") ) )
			{
				//Exit Programming Mode and Data Storage
				if (false == (IsConnected = SendDataCmd("IA!", "K") ) )
				{
					if ( this->m_RS232COM.IsOpened() )
					{ this->m_RS232COM.Close(); }

					SetErrorMsg("Barcode Connect fault!"); 
					continue;
				}
				else
				{
					//Exit Host Mode
					IsConnected = SendDataCmd("[A", "X");
				}
			}
		}
		else
		{
			//Exit Host Mode
			SendDataCmd("[A", "X");
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
bool DataLogic_Matrix200::Export()
{
	//parameter	
	DWORD Result = 0x00;	
	char FileName[256]="",Section[128]="",KeyName[128]="",String[32]="";
		
	strcpy(FileName, m_INIPath.c_str() );
	strcpy(Section, m_SectionName.c_str());

	strcpy(KeyName,"Port");
	sprintf(String, "%d", m_Port);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);

    strcpy(KeyName,"SYMBOLNUM");
	sprintf(String, "%d", BarCode_CS::m_NCodes);
	::WritePrivateProfileStringA(Section, KeyName, String, FileName);	

	strcpy(KeyName,"Delay Time");
	::sprintf(String, "%d", this->m_DelayTime);
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
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Import()
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
		
	strcpy(KeyName,"SYMBOLNUM");
	strcpy(DefaultString,"1");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);    
	BarCode_CS::m_NCodes = ::atoi(ReturnedString);

	//m_LaggerTime
	strcpy(KeyName,"Delay Time");
	strcpy(DefaultString,"100");
    Result = GetPrivateProfileStringA(Section, KeyName, DefaultString, ReturnedString, BufferSize, FileName);    
	this->m_DelayTime = ::atoi(ReturnedString);
	if ( this->m_DelayTime < 0 ) { this->m_DelayTime = 100; }

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
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Disconnect()	//兵絏诀耞絬
{
	if ( IsConnectedSucc() )
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
bool DataLogic_Matrix200::OnTrigger()	//Trigger ON
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
bool DataLogic_Matrix200::OFFTrigger()	//Trigger OFF
{
	if (false == IsConnectedSucc() )
		return false;

	//m_RS232COM.ClearQueue();		
	this->ClearBuffer();	
	bool IsOK = false;
	if ( m_RS232COM.SendData(m_TriggerOffChar.c_str(), m_TriggerOffChar.length() ) == 0 )
	{	IsOK = false; }
	else
	{	IsOK = true; }

	m_nTrigger = 1;	
	return IsOK;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::DoInitial_BarcodeReader()//兵絏诀﹍砞﹚
{
	bool IsOK1 = false;
	bool IsOK2 = false;
	std::string ErrS;
	BarCode_CS::m_IsInitialSuccessed = false;
	
	if( ChangeToProgramMode() == false ) { return false; }		//ち传把计砞﹚家Α

	IsOK1 = Initial_BarcodeReader();//﹍て兵絏
	
	IsOK2 = ExitProgramMode();//ち传把计砞﹚家Α
	if ( IsOK1 == false )
	{	
		return false;
	}
	else if ( IsOK2 == false ) 
	{
		return false;
	}
	
	BarCode_CS::m_IsInitialSuccessed = true;
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Initial_BarcodeReader()	//兵絏诀﹍砞﹚
{	
	//巨家Α 0: one shot, 1: Continue, 2: Phase Mode
	if(false == SetParamCmd("AA", 2) )
		return false;

	//Phase ON-Main Port String(4)+Auxiliary Port String(8)	
	if(false == SetParamCmd("AB", 12) )
		return false;

	//Phase Mode, 1: continue
	if(false == SetParamCmd("AC", 1) )
		return false;	

	//Reading Phase OFF, Main Port String(4)+Aux Port String(8)
	if(false == SetParamCmd("AD", 12) )
		return false;	

	//Phase Time Out, 35-60000
	if(false == SetParamCmd("AE", 1000) )
		return false;	

	//First Acquisition Setting Used, 01 
	if(false == SetParamCmd("AH", 0) )
		return false;	

	//Number of Image slots, 1-20
	if(false == SetParamCmd("AL", 2) )
		return false;	

	//Code Filter Depth, 0-5
	if(false == SetParamCmd("AI", 0) )
		return false;	

	//Good Read Threshold, 0-100000
	if(false == SetParamCmd("AJ", 0) )
		return false;	

	//No Read Threshold, 0-100000
	if(false == SetParamCmd("AK", 0) )
		return false;	

	//TRIGGER DELAY Status, 0 = Disabled, 1 = Enabled
	if(false == SetParamCmd("n1", 0) )
		return false;	

	//Delay Time, 0 to 100000
	if(false == SetParamCmd("o1", 0) )
		return false;	


	if ( Initial_Communication() == false ) { return false; }	//兵絏诀硈絬把计砞﹚
	if ( Initial_Calibration() == false ) { return false; }		//兵絏诀タ把计砞﹚
	if ( Initial_2DCode_Status() == false ) { return false; }			//兵絏摸砞﹚
	if ( Initial_1DCode_Status() == false ) { return false; }			//兵絏摸砞﹚
	if ( Initial_MISCELLANEOUS() == false ) { return false; }


	//IMAGE PROCESSING
	//Processing Mode, 0 = Standard,
	if(false == SetParamCmd("FA", 0) )
		return false;

	//Identical Codes Decoding, 0 = Disabled, 1 = Enabled
	if(false == SetParamCmd("FB", 0) )
		return false;

	//Image Lighting Quality, 0 = Disabled, 1 = Enabled
	if(false == SetParamCmd("FC", 0) )
		return false;

	//Image Mirroring, 0 = Disabled, 1 = Enabled
	if(false == SetParamCmd("FD", 0) )
		return false;

	//Image Processing Timeout, 0-30000
	if(false == SetParamCmd("FF", 0) ) 
		return false;

	//POSTAL CODES
	//Status, 0: Disable
	if(false == SetParamCmd("IA", 0) )
		return false;

	//Local No-Read Message
	//Code Collection Mode, 1 = Within a Phase
	if(false == SetParamCmd("JA", 1) )
		return false;	

	if(false == SetSymbolNum(BarCode_CS::m_NCodes, false) )
		return false;

	if(false == SetSymbolSeparator(false) )
		return false;

	//Code Collection Filters, 0 = Disabled
	if(false == SetParamCmd("JC", 0) )
		return false;

	//Status, 0 = Disabled
	if(false == SetParamCmd("p1", 0) )
		return false;

	//Justification, 0 = Disabled
	if(false == SetParamCmd("KA", 0) )
		return false;

	//Code Field Cutting, 0 = None
	if(false == SetParamCmd("KD", 0) )
		return false;

	//No Read message
	if(false == SetParamCmd("KF" + m_NoRead) )
		return false;
	
	//Phase-Overrun Message	
	if(false == SetParamCmd("KI") )
		return false;

	//Partial Read TX, 1=Enable
	if(false == SetParamCmd("KL", 1) )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Initial_Communication()	//兵絏诀硈絬把计砞﹚
{
	//Main Port Setting
	//Handshake, 0 = None
	if(false == SetParamCmd("BJ", 0) )
		return false;

	//Header
	if(false == SetParamCmd("BK" + m_HeaderChar) )
		return false;

	//Terminator
	if(false == SetParamCmd("BL" + m_TerminatorChar) )
		return false;

	//Phase ON String
	if(false == SetParamCmd("BM" + m_TriggerChar) )
		return false;

	//Phase OFF String
	if(false == SetParamCmd("BO" + m_TriggerOffChar) )
		return false;

	//Auxiliary Port Setting
	//Communication Handshake, 0 = None
	if(false == SetParamCmd("DF", 0) )
		return false;

	//Header
	if(false == SetParamCmd("DG" + m_HeaderChar) )
		return false;

	//Terminator
	if(false == SetParamCmd("DH" + m_TerminatorChar) )
		return false;

	//Phase ON String
	if(false == SetParamCmd("DI" + m_TriggerChar) )
		return false;

	//Phase OFF String
	if(false == SetParamCmd("DK" + m_TriggerOffChar) )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Initial_MISCELLANEOUS()
{
	//MISCELLANEOUS
	//IMAGE BUFFER
	//Status, 0 = Disabled, 1 = Enabled
	if(false == SetParamCmd("QG", 0) )
		return false;

	//VISISET IMAGE SAVING
	//Status, 0 = Disabled, 1 = Enabled
	if(false == SetParamCmd("QB", 0) )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Initial_CalibrationRecipe(int Id)
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
bool DataLogic_Matrix200::Initial_Calibration()	//兵絏诀タ把计砞﹚
{
	//Calibrate

	//Recipe1/////////////////////////////////
	Initial_CalibrationRecipe(1);

	//Recipe2/////////////////////////////////
	Initial_CalibrationRecipe(2);	

	//Recipe3/////////////////////////////////
	Initial_CalibrationRecipe(3);


	//IMAGE PROCESSING SETUP
	//Self Tuning, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("FO", 0) )
		return false;

	//Self Tuning Mode
	/*
	0 = Symbologies Only
	1 = Processing Modes Only
	3 = Code Contrast Levels Only
	4 = Image Mirroring Only
	5 = General Purpose
	*/
	if (false == SetParamCmd("FP", 0) )
		return false;

	//Self Tuning Timeout, 0 to 180000(0 = Disabled)
	if (false == SetParamCmd("FQ", 5000) )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Initial_2DCode_Status()	//兵絏摸砞﹚
{	
	bool IsUseCode = false;

	//2D CODES
//D G E //L M N //O P U //X Z
	//Code Color, 0 = Black, 1 = White, 2 = Both Colors
	if (false == SetParamCmd("GC", 0) )
		return false;

	//Code Contrast, 0 = High, 1 = Standard, 2 = Low, 3 = Very Low
	if (false == SetParamCmd("FE", 1) )
		return false;

	//DATA MATRIX ECC200	
	//Status, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("GA", BarCode_CS::GetIsEnableCode(BARCODE_CODE_DATAMATRIX) ) )
		return false;

	//QR CODE
	//Status, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("GO", BarCode_CS::GetIsEnableCode(BARCODE_CODE_QRCODE) ) )
		return false;

	//AZTEC CODE
	//Status, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("GQ", BarCode_CS::GetIsEnableCode(BARCODE_CODE_AZTEC_CODE) ) )
		return false;

	//MAXICODE CODE
	//Status, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("GS", 0) )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Initial_2DCode()	//兵絏摸砞﹚
{
	//2D CODES
	//D G E //L M N //O P U //X Z
	//Code Color, 0 = Black, 1 = White, 2 = Both Colors
	if (false == SetParamCmd("GC", 0) )
		return false;

	//Code Contrast, 0 = High, 1 = Standard, 2 = Low, 3 = Very Low
	if (false == SetParamCmd("FE", 1) )
		return false;

	//DATA MATRIX ECC200	
	//Status, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("GA", BarCode_CS::GetIsEnableCode(BARCODE_CODE_DATAMATRIX) ) )
		return false;

	//QR CODE
	//Status, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("GA", BarCode_CS::GetIsEnableCode(BARCODE_CODE_QRCODE) ) )
		return false;

	//Decode Method, 0 = Standard, 1 = Direct Marking
	if (false == SetParamCmd("GY", 0) )
		return false;

	//AZTEC CODE
	//Status, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("GQ", BarCode_CS::GetIsEnableCode(BARCODE_CODE_AZTEC_CODE) ) )
		return false;

	//MAXICODE CODE
	//Status, 0 = Disabled, 1 = Enabled
	if (false == SetParamCmd("GS", 0) )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Initial_1DCode_Status()	//兵絏摸砞﹚
{	
	//1D Codes Setup
	//Minimum code, Height(mm), 1-500	
	if (false == SetParamCmd("FI", 1) )
		return false;

	//code Aspect Ratio, 0:Standard, 1:Low	
	if (false == SetParamCmd("FK", 0) )
		return false;

	//code contrast, 0:Standard, 1:Low	
	if (false == SetParamCmd("FL", 0) )
		return false;

	//PDF417 (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("HA", BarCode_CS::GetIsEnableCode(BARCODE_CODE_PDF417) ) )
		return false;

	//MICRO PDF417 (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("Hy", BarCode_CS::GetIsEnableCode(BARCODE_CODE_MICRO_PDF417) ) )
		return false;

	//CODE 128 (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("HC", BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE128) ) )
		return false;

	//EAN 128 (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("HE", 0) )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("HF", 1) )
		return false;

	//CODE 39 (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("HG", BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE39) ) )
		return false;

	//INTERLEAVED 2 OF 5 (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("HN", BarCode_CS::GetIsEnableCode(BARCODE_CODE_INTERLEAVED_2OF5) ) )
		return false;
	
	//PHARMACODE (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("HV", 0) )
		return false;

	//UPC/EAN FAMILY (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("Hd", BarCode_CS::GetIsEnableCode(BARCODE_CODE_UPC_EAN) ) )
		return false;

	//CODABAR (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("Hi", BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODABAR) ) )
		return false;

	//CODE 93 (Status, 0:Disabled, 1:Enabled)	
	if (false == SetParamCmd("Hl", BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE93) ) )
		return false;

	//GS1 DATABAR EXPANDED (RSS EXPANDED)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Hn", 0) )
		return false;

	//GS1 DATABAR EXPANDED STACKED (RSS EXPANDED STACKED)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Hx", 0) )
		return false;

	//GS1 DATABAR LIMITED (RSS LIMITED)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Ho", 0) )
		return false;

	//GS1 DATABAR (RSS 14)
	//GS1 DATABAR TRUNCATED (RSS 14 TRUNCATED)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Hp", 1) )
		return false;

	//GS1 DATABAR STACKED (RSS 14 STACKED)
	//GS1 DATABAR STACKED OMNIDIRECTIONAL (RSS 14 STACKED OMNIDIRECTIONAL)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Hq", 0) )
		return false;

	//COMPOSITE
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Hr", 1) )
		return false;

	return true;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::Initial_1DCode()	//兵絏摸砞﹚
{	
	//1D Codes Setup
	//Minimum code, Height(mm), 1-500
	if (false == SetParamCmd("FI", 1) )
		return false;

	//code Aspect Ratio, 0:Standard, 1:Low
	if (false == SetParamCmd("FK", 0) )
		return false;
	//code contrast, 0:Standard, 1:Low
	if (false == SetParamCmd("FL", 0) )
		return false;

	//PDF417
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HA", BarCode_CS::GetIsEnableCode(BARCODE_CODE_PDF417) ) )
		return false;

	//MICRO PDF417
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hy", BarCode_CS::GetIsEnableCode(BARCODE_CODE_MICRO_PDF417) ) )
		return false;

	//CODE 128 (Status, 0:Disabled, 1:Enabled)
	if (false == SetParamCmd("HC", BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE128) ) )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HD", 1) )
		return false;

	//EAN 128
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HE", 0) )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HF", 1) )
		return false;

	//CODE 39
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HG", BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE39) ) )
		return false;

	//Character Set, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HI", 1) )
		return false;

	//Check Digit Status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HJ", 1) )
		return false;

	//Check Digit Transmission, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HK", 1) )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HL", 1) )
		return false;

	//INTERLEAVED 2 OF 5
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HN", BarCode_CS::GetIsEnableCode(BARCODE_CODE_INTERLEAVED_2OF5) ) )
		return false;

	//Check Digit Status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HO", 1) )
		return false;

	//Check Digit Transmission, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HP", 1) )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("HQ", 1) )
		return false;

	//Minimum Number of Characters, 0:>4, 1:4, 2:2
	if (false == SetParamCmd("HM", 1) )
		return false;

	//PHARMACODE (Status, 0:Disabled, 1:Enabled)
	if (false == SetParamCmd("HV", 0) )
		return false;

	//UPC/EAN FAMILY (Status, 0:Disabled, 1:Enabled)
	if (false == SetParamCmd("Hd", BarCode_CS::GetIsEnableCode(BARCODE_CODE_UPC_EAN) ) )
		return false;

	//ADDON 2 and 5 Status , 0 = Disabled, 1 = Enabled, 2 = Enabled (+AddOn 2 No Quiet Zone)
	if (false == SetParamCmd("He", 1) )
		return false;

	//Expand UPC E0-E1 symbols , 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hf", 1) )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hg", 1) )
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
	if (false == SetParamCmd("Hw", 1) )
		return false;

	//CODABAR
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hi", BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODABAR) ) )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hj", 1) )
		return false;

	//Check Digit Status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hh", 1) )
		return false;

	//Check Digit Transmission, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hk", 1) )
		return false;

	//CODE 93
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hl", BarCode_CS::GetIsEnableCode(BARCODE_CODE_CODE39) ) )
		return false;

	//Narrow Margins, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hm", 1) )
		return false;

	//GS1 DATABAR EXPANDED (RSS EXPANDED)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Hn", 0) )
		return false;

	//GS1 DATABAR EXPANDED STACKED (RSS EXPANDED STACKED)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Hx", 0) )
		return false;

	//GS1 DATABAR LIMITED (RSS LIMITED)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Ho", 0) )
		return false;

	//GS1 DATABAR (RSS 14)
	//GS1 DATABAR TRUNCATED (RSS 14 TRUNCATED)
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hp", 1) )
		return false;

	//GS1 DATABAR STACKED (RSS 14 STACKED)
	//GS1 DATABAR STACKED OMNIDIRECTIONAL (RSS 14 STACKED OMNIDIRECTIONAL)
	//status, 0:Disabled, 1:Enabled	
	if (false == SetParamCmd("Hq", 0) )
		return false;

	//COMPOSITE
	//status, 0:Disabled, 1:Enabled
	if (false == SetParamCmd("Hr", 1) )
		return false;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::SetParamCmd(const std::string &rHd)
{
	char str[512] = "";	
	sprintf(str, "%c%s", m_MyEsc, rHd.c_str() );
	return SetParam(str);
}

//---------------------------------------------------------------------//
bool DataLogic_Matrix200::SetParamCmd(const std::string &rHd, int SharpN, int Data)
{
	char str[512] = "";	
	sprintf(str, "%c%s%d%d", m_MyEsc, rHd.c_str(), SharpN, Data);

	return SetParam(str);
}

//---------------------------------------------------------------------//
bool DataLogic_Matrix200::SetParamCmd(const std::string &rHd, int Value)
{
	char str[512] = "";	
	sprintf(str, "%c%s%d", m_MyEsc, rHd.c_str(), Value);
	return SetParam(str);
}

//---------------------------------------------------------------------//
bool DataLogic_Matrix200::SetParamCmd(const std::string &rHd, bool IsUseCode)
{	
	return SetParamCmd(rHd, (IsUseCode ? 1 : 0) );
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::SetParam(const std::string &Commond)	//砞﹚兵絏诀把计
{	
	//砞﹚把计玡ゲ斗秈把计砞﹚家Α
	const int SleepTime = 50;
	int i=0;
	int MaxCounts = 10; 
	for ( i=0; i<MaxCounts; i++ )
	{	
		if( SendData(Commond, "") == false )//糶把计	
		{
			::Sleep(SleepTime);
			continue;
		}

		if( ExitSingleProgram() == false ) //虫把计糶挡	
		{ 
			::Sleep(SleepTime);
			continue;
		}	  
		break;
	}
	if ( i == MaxCounts ) 
	{		
		return false;	
	}
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::SendDataCmd(const std::string &rHd, const std::string &Replay)
{
	char str[512] = "";	
	sprintf(str, "%c%s", m_MyEsc, rHd.c_str());
	return SendData(str, Replay);
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::SendData(const std::string &commend , const std::string &Replay)	//糶RS232
{		
	if( m_RS232COM.SendData(commend.c_str(), commend.length() ) == 0 )
	{
		SetErrorMsg("Send Data Error, %s! ", commend.c_str() );
		return false;				
	}
	Sleep(m_DelayTime);
	if ( Replay.length() > 0 ) 
	{
		if ( this->CheckReply(Replay) == false )
		{
			SetErrorMsg("Send (%s) is OK, but fail to replay (%s)!",
							commend.c_str(), Replay.c_str() );
			return false;
		}
		::Sleep(m_DelayTime);
	}
	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::ChangeToProgramMode()	//ち传把计砞﹚家Α
{
	this->m_ProgramMode = DATALOGIC_MODE_LEVEL_NONE;

	if (false == IsConnectedSucc() )
		return false;
	
	this->ClearBuffer();

	//Enter Host Mode	
	if (false == SendDataCmd("[C", "H") )
		return false;

	this->m_ProgramMode = DATALOGIC_MODE_LEVEL_HOST;

	//Enter Programming Mode
	if (false == SendDataCmd("[B", "Q") )
		return false;

	this->m_ProgramMode = DATALOGIC_MODE_LEVEL_PROGRAM;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::ExitProgramMode()		//瞒秨把计砞﹚家Α
{
	if (false == IsConnectedSucc() )
		return false;

	this->ClearBuffer();	

	if ( this->m_ProgramMode == DATALOGIC_MODE_LEVEL_PROGRAM )
	{
		//Exit Programming Mode and Data Storage		
		this->m_ProgramMode = DATALOGIC_MODE_LEVEL_HOST;		
		if ( false == SendDataCmd("IA!", "K") )
		{
			//Exit Host Mode			
			SetParamCmd("[A");
			this->m_ProgramMode = DATALOGIC_MODE_LEVEL_NONE;
			return false;
		}
	}

	if ( this->m_ProgramMode == DATALOGIC_MODE_LEVEL_HOST )
	{
		//Exit Host Mode		
		this->m_ProgramMode = DATALOGIC_MODE_LEVEL_NONE;
		if ( false == SendDataCmd("[A!", "X") )
			return false;
	}
	this->m_ProgramMode = DATALOGIC_MODE_LEVEL_NONE;

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::ExitSingleProgram()	//虫把计糶挡
{
	return SendDataCmd("IA#", "K");
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::IsBarcodeConnected()	//Barcode硈絬絋粄
{
	if(false == IsConnectedSucc() )
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
	
	if (0== pMsg.compare("H") )
	{		
		return true;
	}

	return false;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::GetData()//眔兵絏
{
	if(false == IsConnectedSucc() )
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
bool DataLogic_Matrix200::ReadMsg(std::string &rData)
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
bool DataLogic_Matrix200::SetToNewRecipe(int No)
{
	if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
		
	for (int i=1 ; i<=m_RecipeMax ; i++ )
	{
		//Status, 0 = Disabled, 1 = Enabled		
		if (false == SetParamCmd("a", i, (i == No) ? 1 : 0) )
			return false;
	}	

	if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α

	return true;
}
//---------------------------------------------------------------------//
bool DataLogic_Matrix200::StartDevice()
{
	return true;
}
//-----------------------------------------------------------------//
bool DataLogic_Matrix200::SetNMultiCodes(int NCodes)//砞﹚兵絏计ヘ
{
	if ( false == CheckNMultiCodes(NCodes) )
		return false;

	if(false == IsConnectedSucc() )
		return false;
	
	BarCode_CS::m_NCodes = NCodes;

	return this->SetSymbolNum(NCodes, true);
}
//-----------------------------------------------------------------//
bool DataLogic_Matrix200::SetSymbolNum(int SymbolNum, bool IsChangeToProgramMode)
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
bool DataLogic_Matrix200::SetSymbolSeparator(bool IsChangeToProgramMode)
{
	if( IsChangeToProgramMode == true )
	{	
		if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
	}

	if ( m_RS232COM.IsOpened() == false )
	{
		SetErrorMsg("No Connected");
		return false;
	}

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
void DataLogic_Matrix200::ExportCodeId(int CodeId)
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
void DataLogic_Matrix200::ImportCodeId(int CodeID)
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
//------------------------------------------------------------------//
bool DataLogic_Matrix200::CheckNMultiCodes(int CodeCnt)
{
	if ( (CodeCnt >= m_MaxCodes ) || ( CodeCnt < m_MinCodes ) )
	{	
		SetErrorMsg("Number of Multi-code is out of Range!!!");
		return false;
	}
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::IsConnectedSucc()
{
	if(false == m_RS232COM.IsOpened() )
	{ 
		SetErrorMsg("RS232 open Fail!!!");
		return false;
	}
	return true;
}
//------------------------------------------------------------------//
bool DataLogic_Matrix200::FindStringDrop(std::string &rSrcDec, const std::string &rObject, bool Foreward)
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
bool DataLogic_Matrix200::UpdateCodeSetting()
{
	if( ChangeToProgramMode() == false ) { return false; }	//ち传把计砞﹚家Α
	if ( this->Initial_1DCode_Status() == false ) { return false; }
	if ( this->Initial_2DCode_Status() == false ) { return false; }
	if( ExitProgramMode() == false ) { return false; }		//瞒秨把计砞﹚家Α
	return true;
}
//---------------------------------------------------------------------------------------------------//
bool DataLogic_Matrix200::CheckReply(const std::string &Replay) //絋粄肚じ
{
	//肚絏<ESC> ???? <CR><LF>
	char TempChar = 0x00;
	
	int   ContinueCount = 0;
	int   BuferID = 0;
	const int BufferSize = 1024;
	char Buffer[BufferSize]="";	
	char Buffer2[BufferSize]="";	

	bool Started = false;
	while ( true)
	{
		ContinueCount ++;
		if ( ContinueCount >= BufferSize ) { break; }

		this->m_RS232COM.ReadSingleChar(TempChar);	
		if ( TempChar == 0x00 ) { break; }
		if ( Started == false )
		{
			if ( TempChar != DATALOGIC_REPLAY_ESC ) { continue; }
			Started = true;
		}		
		Buffer[BuferID] = TempChar;
		BuferID ++;
		if ( BuferID >= BufferSize ) { break; }		
		if ( Started == true ) 
		{
			if ( TempChar == DATALOGIC_REPLAY_LF ) { break; }
		}
	};

	if ( Started == false ) { return false; }
	if ( BuferID < 2 ) { return false; }

	if ( Buffer[BuferID-1] != DATALOGIC_REPLAY_LF ) { return false; }
	if ( Buffer[BuferID-2] != DATALOGIC_REPLAY_CR ) { return false; }	
	
	int i=0, j=0;
	for ( i=1; i<BuferID-2; i++ )
	{
		Buffer2[j] = Buffer[i];
		j++;
	}
	Buffer2[j] = '\0';
	
	if (0 != Replay.compare(Buffer2) ) 
	{ 
		return false; 
	}
	return true;
}
//---------------------------------------------------------------------------------------------------//