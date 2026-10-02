// Light3DTiDLP_DLPC350_v4.cpp: implementation of the CLight3DTiDLP_DLPC350_v4 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP_DLPC350_v4.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP
//-------------------------------------------------------------------------------------//
#include "DLPC350_4_0_0\\dlpc350_version.h"
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_DLPC350_v4::CLight3DTiDLP_DLPC350_v4()
{
	PreInitTiDlp(0, LIGHT_3D_CAST_00, TB_SYSTEM_TOP);	
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_DLPC350_v4::CLight3DTiDLP_DLPC350_v4(int CtrlID, LIGHT_3D_CAST_ID Light3DID, TB_SYSTEM_ID SysID)
{
	PreInitTiDlp(CtrlID, Light3DID, SysID);
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_DLPC350_v4::~CLight3DTiDLP_DLPC350_v4()
{
	ClearPhaseZeroBuffer();	
	ClearPhaseFactorBuffer();
	DLPDisconnect();
	m_TiUSB.USB_Exit();
	::DeleteCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP_DLPC350_v4::PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID Light3DID, TB_SYSTEM_ID SysID)
{	
	::InitializeCriticalSection(&m_csLight3D);

	m_dwFrmVersion = 0;
	::memset(g_PatLut, 0x00, sizeof(g_PatLut));	
	::memset(g_ExpLut, 0x00, sizeof(g_ExpLut));		
	::memset(m_DLPFrmTag, 0x00, sizeof(m_DLPFrmTag));
	::memset(m_TiAPIversion, 0x00, sizeof(m_TiAPIversion));	
	::memset(m_DLPFrmversion, 0x00, sizeof(m_DLPFrmversion));	

	m_DLPSystemID = SysID;
	m_CtrlBoardID = CtrlID;
	m_Light3DCastID = Light3DID;
	m_TrigOutCount = 0;
	g_SeqNum = 0;
	m_numImgInFlash = 0;
	g_PatLutIndex = 0;		
	g_ExpLutIndex = 0;
	m_OperationMode = DLP_OPERATION_DEFAULT;	

	m_HWStatus = 0;
	m_SysStatus = 0;
	m_MainStatus = 0;	

	m_EnableTemperatureMonitor = true;

	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroPtr = NULL;//平面相位指標
	m_PhaseZeroLEDColor = DLP_LED_COLOR_RED;

	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorPtr = NULL;//平面係數指標
	m_PhaseFactorLEDColor = DLP_LED_COLOR_RED;

	this->m_numExtraSplashLutEntries = 0;		
	::memset(m_ExtraSplashLutEntries, 0x00, sizeof(m_ExtraSplashLutEntries));	

	switch ( Light3DID )
	{
	case LIGHT_3D_CAST_01:	m_CastName=_T("ID-01");	break;
	case LIGHT_3D_CAST_02:	m_CastName=_T("ID-02");	break;
	case LIGHT_3D_CAST_03:	m_CastName=_T("ID-03");	break;
	case LIGHT_3D_CAST_04:	m_CastName=_T("ID-04");	break;
	}

	this->InitialDLPParameter(m_DLPParam);	
	//Before connect
	m_TiUSB.USB_Init();
	g_InputBuffer = m_TiUSB.GetInputBuffer();//指向Ti-USB內部輸入記憶體
	g_OutputBuffer = m_TiUSB.GetOutputBuffer();//指向Ti-USB內部輸出記憶體
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP_DLPC350_v4::InitialTiDlp()
{
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
	m_LEDCurrentR = -1;
	m_LEDCurrentG = -1;
	m_LEDCurrentB = -1;
	m_InvertPWM = false;
	m_LEDEnabled_R = true;
	m_LEDEnabled_G = true;
	m_LEDEnabled_B = true;
	m_LEDEnabled_Auto = true;
	m_ImageGamma = 1.0;
	m_SecondExpRatio = 1.0;	 
	m_Exposure_us = 0;
	m_TrigPeriod_us = 0;		
	m_Exposure2_us = 0;
	m_TrigPeriod2_us = 0;		
	m_DLPDelayTime = 20;
	m_LEDColor = DLP_LED_COLOR_NO;

	m_PatternBitCount = 6;
	m_PatternIndex1_3Bit = -1;
	m_PatternIndex2_3Bit = -1;
	m_PatternIndex1_5Bit = -1;
	m_PatternIndex1_6Bit = 0;
	m_PatternIndex2_6Bit = 1;
	m_PatternIndexGC_1Bit= -1;
	m_PatternIndexBC_1Bit= -1;
	m_PatternStartNumGC_1Bit = -1;
	m_PatternStartNumBC_1Bit = -1;

	::memset(m_HeightFactor0, 0x00, sizeof(m_HeightFactor0));
	::memset(m_HeightFactor1, 0x00, sizeof(m_HeightFactor1));
	::memset(m_HeightFactor2, 0x00, sizeof(m_HeightFactor2));	
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP_DLPC350_v4::LockLight3D()
{
	::EnterCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//}
inline void CLight3DTiDLP_DLPC350_v4::UnlockLight3D()
{
	::LeaveCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP_DLPC350_v4::SetLCRErrorFnName(LPCTSTR LCRFnName)
{
	this->m_ErrorString.Format(_T("Error, TiDLP %s Fault (%s)"), LCRFnName, this->m_CastName);
}
//-------------------------------------------------------------------------------------//
inline bool CLight3DTiDLP_DLPC350_v4::GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[])
{
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::wsprintfW(USB_Number, L"LCR2");	break;
	case LIGHT_3D_CAST_03:	::wsprintfW(USB_Number, L"LCR3");	break;
	case LIGHT_3D_CAST_04:	::wsprintfW(USB_Number, L"LCR4");	break;
	default:			    ::wsprintfW(USB_Number, L"LCR1");	break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetDLPID(int ID)
{	
	m_CtrlBoardID = ID;
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP_DLPC350_v4::GetDLPID() const
{
	return m_CtrlBoardID;
}
//-------------------------------------------------------------------------------------//
TB_SYSTEM_ID CLight3DTiDLP_DLPC350_v4::GetDLPSystemID() const
{
	return m_DLPSystemID;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckDLPInSystemID(TB_SYSTEM_ID SysID)//確認DLP在此系統編號
{
	return m_DLPSystemID==SysID;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CAST_ID CLight3DTiDLP_DLPC350_v4::GetCastID() const
{
	return m_Light3DCastID;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP_DLPC350_v4::GetDLPProjectName()
{
	return this->m_CastName;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP_DLPC350_v4::GetErrorString()
{
#ifdef TB_SYSTEM_ENABLE	
	TB_SYSTEM_ID SysID=GetDLPSystemID();
	switch ( SysID )
	{
	case TB_SYSTEM_BOT:	m_ErrorString_TB.Format(_T("%s [BOT]"), m_ErrorString);	break;
	default:
	case TB_SYSTEM_TOP: m_ErrorString_TB.Format(_T("%s [TOP]"), m_ErrorString);	break;		
	}
	return m_ErrorString_TB;
#endif//TB_SYSTEM_ENABLE
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP_DLPC350_v4::GetDLPImageW() const
{
	return 912;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP_DLPC350_v4::GetDLPImageH() const
{
	return 1140;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::DLPConnect()
{
	SaveDLPProcess(_T("DLPConnect"), MSG_LEVEL_HIGH);

	bool SLmode=0;		
	unsigned int API_ver, App_ver, SWConfig_ver, SeqConfig_ver;
	unsigned int FW_ver;
	const int DLPID = 0;
	wchar_t USB_Number[256]={0};

	if( m_TiUSB.USB_IsConnected()==1 )
	{	this->DLPDisconnect();	}

	if( GetUSB_Number(m_Light3DCastID, USB_Number) == false)
	{	return false;	}
	//::wsprintfW(USB_Number, L"LCR2");    
	if ( m_TiUSB.USB_Open(USB_Number) < 0 ) 
	{
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault (%s)"), this->m_CastName);
		return false; 
	}	

	// Display GUI Version #
	if (DLPC350_GetVersion(&App_ver, &API_ver, &SWConfig_ver, &SeqConfig_ver) == 0)
	{	sprintf(m_TiAPIversion, "%d.%d.%d", (API_ver >> 24), ((API_ver << 8) >> 24), ((API_ver << 16) >> 16));	}

	sprintf(m_TiAPIversion, "%d.%d.%d", GUI_VERSION_MAJOR, GUI_VERSION_MINOR, GUI_VERSION_BUILD);
	
	if ( API_ver >= 0x04000000 )
	{	
		m_dwFrmVersion = DLPC350_BIN_VERSION_4_0_0;
		sprintf(m_DLPFrmversion, "%d.%d.%d", (API_ver >> 24), ((API_ver << 8) >> 24), ((API_ver << 16) >> 16));			
	}
	else if ( API_ver >= 0x03000000 )
	{	
		m_dwFrmVersion = DLPC350_BIN_VERSION_3_0_0;
		sprintf(m_DLPFrmversion, "%d.%d.%d", (API_ver >> 24), ((API_ver << 8) >> 24), ((API_ver << 16) >> 16));			
	}
	else
	{		
		if ( DLPC350_MemRead(0xF902C000, &FW_ver) == 0)
		{
			FW_ver &= 0xFFFFFF;
			m_dwFrmVersion = DLPC350_BIN_VERSION_2_0_0;
			sprintf(m_DLPFrmversion, "%d.%d.%d", (FW_ver >> 16), ((FW_ver << 16) >> 24), ((FW_ver << 24) >> 24));//FWAPI_version            			
			// When GUI is first opened, check if old firmware is present & prompt user to upload new version if it is
			//if (FW_ver < RELEASE_FW_VERSION)
			//{	::AfxMessageBox(_T("WARNING: Old version of Firmware detected."));	}        
		}
	}

	//Read firmware tag information
	CString strFrmTag;
	unsigned char firmwareTag[33]="";
	if ( DLPC350_GetFirmwareTagInfo(&firmwareTag[0]) == 0)
	{	
		strFrmTag = firmwareTag;	
		::strcpy(m_DLPFrmTag, (const char*)(firmwareTag));
	}

	unsigned int numImgInFlash = 0;
	//Retrieve the total number of Images in the firmware info
	if ( DLPC350_GetNumImagesInFlash(&numImgInFlash) == 0)
	{	m_numImgInFlash = numImgInFlash;	}

	bool ShowMsg = false;
	bool ResetFinish = true;
	CLight3DTiDLP_DLPC350_v4::ResetLEDDisable(ResetFinish, ShowMsg);

	this->CheckDLPStatus();

	//Check SL Mode
	m_OperationMode = DLP_OPERATION_DEFAULT;	
	if ( DLPC350_GetMode(&SLmode) ==0  )
	{
		if ( SLmode == true )
		{
			int trigMode=0;			
			if ( DLPC350_GetPatternTriggerMode(&trigMode) == 0 )
			{
				if ( 1==trigMode || 2==trigMode )
				{	m_OperationMode = DLP_OPERATION_PATTERN;	}
				if ( 3==trigMode || 4==trigMode )
				{	m_OperationMode = DLP_OPERATION_PATTERN_EXP;	}
			}
		}
		else
		{
			bool Standby = false;
			if ( DLPC350_SetPowerMode(&Standby) == 0 )
			{
				if ( false == Standby )
				{	m_OperationMode = DLP_OPERATION_VIDEO;	}
				else
				{	m_OperationMode = DLP_OPERATION_STANDBY;	}
			}
		}
	}	
	//DLPC350_SetPowerMode(false);	
	SetDLPOperationMode(m_OperationMode);	

	//Check LED Parameters
	LoadDLPParameter();
	GetDLPLEDPWMInvert(m_InvertPWM);//m_InvertPWM);	
	GetDLPLEDEnable(m_LEDEnabled_Auto, m_LEDEnabled_R, m_LEDEnabled_G, m_LEDEnabled_B);	
	GetDLPLEDCurrent(m_LEDCurrentR, m_LEDCurrentG, m_LEDCurrentB);	
	ExecDLPLightSetting(DLP_LED_CURRENT_ID_01);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::DLPDisconnect(int WaitTime_ms)
{
	CLight3DTiDLP_DLPC350_v4::ExecDLPPattern_Stop();

	SaveDLPProcess(_T("DLPDisconnect"), MSG_LEVEL_HIGH);

	m_TiUSB.USB_Close();	

	m_dwFrmVersion = 0;
	::memset(m_DLPFrmTag, 0x00, sizeof(m_DLPFrmTag));
	::memset(m_TiAPIversion, 0x00, sizeof(m_TiAPIversion));	
	::memset(m_DLPFrmversion, 0x00, sizeof(m_DLPFrmversion));	

	m_HWStatus = 0;
	m_SysStatus = 0;
	m_MainStatus = 0;
	m_numImgInFlash = 0;

	if ( WaitTime_ms > 0 )
	{	::Sleep(WaitTime_ms);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPIsConnected()
{
	if ( m_TiUSB.USB_IsConnected() == false )
	{
		this->m_ErrorString.Format(_T("Error, TiDLP did not connect (P:%s)."), this->m_CastName);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckDLPFrmForExpLut()//確認DLP韌體支援Exposure Lut
{
	if ( m_dwFrmVersion < DLPC350_BIN_VERSION_3_0_0 )
	{
		CString strFrm = m_DLPFrmversion;
		m_ErrorString.Format(_T("Error, TiDLP Firmware does not support Exp. Lut (P:%s, Frm:%s)."), m_CastName, strFrm);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SaveDLPProcess(LPCTSTR fnName, int Level)
{
	CString str;
	str.Format(_T("Light3D[%d]::%s"), m_Light3DCastID, fnName);
	if ( AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_LIGHT3D, Level, str) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPSoftwareReset(DWORD delayTime)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ExecDLPSoftwareReset"), MSG_LEVEL_HIGH);

	DLPC350_SoftwareReset();
	m_TiUSB.USB_Close();	
	::Sleep(delayTime);//暫停10秒
	wchar_t USB_Number[256] = { 0 };
	if (GetUSB_Number(m_Light3DCastID, USB_Number) == false)
	{	return false;	}

	if (m_TiUSB.USB_Open(USB_Number) < 0)
	{
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault (%s)"), this->m_CastName);
		return false;
	}

	this->ReadDLPParameter();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetDLPLongAxisImageFlip(bool Flip)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPLongAxisImageFlip"), MSG_LEVEL_HIGH);

	if ( DLPC350_SetLongAxisImageFlip(Flip) == -1 ) 
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetLongAxisImageFlip"));		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPLongAxisImageFlip()
{
	return DLPC350_GetLongAxisImageFlip();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetDLPShortAxisImageFlip(bool Flip)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPShortAxisImageFlip"), MSG_LEVEL_HIGH);

	if( DLPC350_SetShortAxisImageFlip(Flip) == -1 ) 
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetShortAxisImageFlip"));		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPShortAxisImageFlip()
{
	return DLPC350_GetShortAxisImageFlip();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetDLPOperationMode(int Mode)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	bool SLMode = true;
	bool Standby = false;

	SaveDLPProcess(_T("SetDLPOperationMode"), MSG_LEVEL_HIGH);

	switch ( Mode )
	{
	case DLP_OPERATION_PATTERN:
	case DLP_OPERATION_PATTERN_EXP:
		SLMode = true;
		Standby = false;

		if ( this->DLPC350_SetPowerMode(Standby) < 0 ) 
		{	return false; }

		if ( this->DLPC350_SetMode(SLMode) < 0 ) 
		{	return false; }

		this->m_OperationMode = Mode;
		break;
	case DLP_OPERATION_VIDEO:
		SLMode = false;
		Standby = false;

		if ( this->DLPC350_SetPowerMode(Standby) < 0 ) 
		{	return false; }

		if ( this->DLPC350_SetMode(SLMode) < 0 ) 
		{	return false; }

		this->m_OperationMode = Mode;
		break;
	case DLP_OPERATION_STANDBY:
		SLMode = false;
		Standby = true;

		if ( this->DLPC350_SetMode(SLMode) < 0 ) 
		{	return false; }

		if ( this->DLPC350_SetPowerMode(Standby) < 0 ) 
		{	return false; }

		this->m_OperationMode = Mode;
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetDLPOperationMode() const
{	
	return this->m_OperationMode;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_LEDEnabled_R!=bRed || m_LEDEnabled_G!=bGreen || m_LEDEnabled_B!=bBlue || m_LEDEnabled_Auto!=bSeqCtrl )
	{
		SaveDLPProcess(_T("SetDLPLEDEnable"), MSG_LEVEL_HIGH);
		if ( DLPC350_SetLedEnables(bSeqCtrl, bRed, bGreen, bBlue) < 0 ) 
		{	
			this->SetLCRErrorFnName(_T("DLPC350_SetLedEnables"));		
			return false; 
		}
	}

	CLight3DTiDLP_DLPC350_v4::m_LEDEnabled_R = bRed;
	CLight3DTiDLP_DLPC350_v4::m_LEDEnabled_G = bGreen;
	CLight3DTiDLP_DLPC350_v4::m_LEDEnabled_B = bBlue;
	CLight3DTiDLP_DLPC350_v4::m_LEDEnabled_Auto = bSeqCtrl;
	this->m_DLPParam.m_LEDEnabled_Auto = bSeqCtrl;
	this->m_DLPParam.m_LEDEnabled_R = bRed;
	this->m_DLPParam.m_LEDEnabled_G = bGreen;
	this->m_DLPParam.m_LEDEnabled_B = bBlue;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( DLPC350_GetLedEnables(&bSeqCtrl, &bRed, &bGreen, &bBlue) == -1 )  
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetLedEnables"));		
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline unsigned char CLight3DTiDLP_DLPC350_v4::GetDLPSafeCurrent(int value)
{	
	int Min=MAX(DLP_LED_CURRENT_MIN, 0);
	int Max=MIN(DLP_LED_CURRENT_MAX, m_DLPParam.m_LEDCurrentMax);
	if ( value > Max ) { return Max; }
	if ( value < Min ) { return Min; }	
	return static_cast<unsigned char>(value);
}
//-------------------------------------------------------------------------------------//
inline int CLight3DTiDLP_DLPC350_v4::GetDLPTrigType(int index, bool IntTrig, bool MultiTable)
{  
	int TrigType = DLP_TRIG_TYPE_INTERNAL;
	if ( true == IntTrig )
	{	TrigType = DLP_TRIG_TYPE_INTERNAL;	}
	else
	{
		if ( 0 == index )	
		{	TrigType = DLP_TRIG_TYPE_EXT_NEG; }
		else
		{
			if ( true == MultiTable )
			{	TrigType = DLP_TRIG_TYPE_EXT_NEG;	}
			else
			{	TrigType = DLP_TRIG_TYPE_INTERNAL;	}		
		}
	}
	return TrigType;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_LEDCurrentR!=red || m_LEDCurrentG!=green || m_LEDCurrentB!=blue )
	{	
		SaveDLPProcess(_T("SetDLPLEDCurrent"), MSG_LEVEL_HIGH);
		unsigned char r = this->GetDLPSafeCurrent(red);
		unsigned char g = this->GetDLPSafeCurrent(green);
		unsigned char b = this->GetDLPSafeCurrent(blue);
		//注意要反向!?
		r = 255-r;
		g = 255-g;
		b = 255-b;
		if ( DLPC350_SetLedCurrents(r, g, b) < 0 ) 
		{		
			this->m_ErrorString.Format(_T("Error, TiDLP DLPC350_SetLedCurrents Fault [R:%d, G:%d, B:%d] (P:%s)"), red, green, blue, this->m_CastName);
			return false; 
		}	
	}

	CLight3DTiDLP_DLPC350_v4::m_LEDCurrentR = red;
	CLight3DTiDLP_DLPC350_v4::m_LEDCurrentG = green;
	CLight3DTiDLP_DLPC350_v4::m_LEDCurrentB = blue;
	//if ( true == Update )//要此過濾再校正電流會有問題
	{
		SetDLPParamLEDCurrent(red, green, blue, CurrentID);		
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPLEDCurrent(int &red, int &green, int &blue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	unsigned char r=0, g=0, b=0;
	if ( DLPC350_GetLedCurrents(&r, &g, &b) == -1)  
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetLedCurrents"));		
		return false;
	}	
	red = 255-r;
	green = 255-g;
	blue = 255-b;
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetDLPLEDPWMInvert(bool bInvert)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_InvertPWM != bInvert )
	{
		SaveDLPProcess(_T("SetDLPLEDPWMInvert"), MSG_LEVEL_HIGH);

		if ( DLPC350_SetLEDPWMInvert(bInvert) < 0 ) 
		{	
			this->SetLCRErrorFnName(_T("DLPC350_SetLEDPWMInvert"));		
			return false; 
		}
	}
	CLight3DTiDLP_DLPC350_v4::m_InvertPWM = bInvert;
	this->m_DLPParam.m_InvertPWM = bInvert;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPLEDPWMInvert(bool &bInvert)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( DLPC350_GetLEDPWMInvert(&bInvert) == -1 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetLEDPWMInvert"));		
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckDLPStatus()
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( DLPC350_GetStatus(&m_HWStatus, &m_SysStatus, &m_MainStatus) == -1) 
	{ 
		this->SetLCRErrorFnName(_T("DLPC350_GetStatus"));		
		return false; 
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPStatus_InitDone()
{
	return bool(m_HWStatus&BIT0);
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPStatus_ForcedSwap()
{
	if ( (m_HWStatus&BIT3) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPStatus_BufferFreeze()
{
	if ( (m_MainStatus&BIT2) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPStatus_SeqRunning()
{
	if ( (m_MainStatus&BIT1) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPStatus_SeqError()
{
	if ( (m_HWStatus&BIT7) == 0 ) 
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPStatus_SeqAbort()
{
	if ( (m_HWStatus&BIT6) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPStatus_DRCError()
{
	if ( (m_HWStatus&BIT2) == 0 ) 
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPStatus_DMDParked()
{
	if ( (m_MainStatus&BIT0) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::InitialDLPParameter(TDLPParam &Param)
{	
	Param.m_LEDColor		=	DLP_LED_COLOR_WHITE;
	Param.m_TrigType		=	DLP_LED_TRIGGER_INTERNAL;
	Param.m_PatternBitDepth	=	6;
	Param.m_TrigInterval_us	=	10300;//10ms
	Param.m_PatternExp_us	=	10000;//10ms
	Param.m_PhaseMode   	=	LIGHT3D_PHASE_4_4_M;

	Param.m_LEDCurrentR_1	=	100;
	Param.m_LEDCurrentG_1	=	100;
	Param.m_LEDCurrentB_1	=	100;

	Param.m_LEDCurrentR_2	=	100;
	Param.m_LEDCurrentG_2	=	100;
	Param.m_LEDCurrentB_2	=	100;

	Param.m_InvertPWM		=	false;
	Param.m_LEDEnabled_Auto	=	true;
	Param.m_LEDEnabled_R	=	true;
	Param.m_LEDEnabled_G	=	true;
	Param.m_LEDEnabled_B	=	true;
	Param.m_FlipLong		=	false;
	Param.m_FlipShort		=	false;

	//	bool	InvertPat = false;
	//	bool	InsertBlack = true;
	//	bool	TrigOutPrev = false;
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SaveDLPParameter()
{	
	bool    IsOK = true;	
#ifndef OFFLINE_VERSION
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section=_T("TiDLP");	
	CString ProjectName = this->GetDLPProjectName();

	FileName.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("Light3D_DLP.ini"));
	Section.Format(_T("TiDLP-%s"), ProjectName);		

	KeyName.Format(_T("LED Color")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDColor);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Trig Type")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigType);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Bit Depth")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternBitDepth);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Trig Interval")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigInterval_us);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Pattern Exp")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternExp_us);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Phase Mode")); KeyString.Format(_T("%d"),m_DLPParam.m_PhaseMode);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Period Time")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime_us);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Exposure Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime_us);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("Period Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime2_us);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Exposure Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime2_us);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("LED Current R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_1);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_1);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_1);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current R 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_2);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current G 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_2);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current B 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_2);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		

	KeyName.Format(_T("Invert PWM")); KeyString.Format(_T("%d"),m_DLPParam.m_InvertPWM);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled Auto")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_Auto);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_R);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_G);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_B);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Flip Long")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipLong);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Flip Short")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipShort);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Validate Delay Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ValidateDelayTime);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Use Exposure Pattern")); KeyString.Format(_T("%d"),m_DLPParam.m_UseExpLut);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Phase Factor Min")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMin);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		 

	KeyName.Format(_T("Phase Factor Max")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMax);
	if ( JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("LED Current Max")); KeyString.Format(_T("%d"), m_DLPParam.m_LEDCurrentMax);
	if (JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Image Gamma")); KeyString.Format(_T("%.6f"), m_DLPParam.m_ImageGamma);
	if (JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("2nd Exposure Ratio")); KeyString.Format(_T("%.6f"), m_DLPParam.m_SecondExpRatio);
	if (JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}
#endif//OFFLINE_VERSION
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadDLPParameter()
{	
	CString FileName;
	CString TempStr;
	CString KeyName;
	CString KeyString;
	CString Section=_T("DLP");
	CString ProjectName = this->GetDLPProjectName();
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];

	FileName.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("Light3D_DLP.ini"));
	Section.Format(_T("TiDLP-%s"),ProjectName);

	//LED Color	
	KeyName.Format(_T("LED Color")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDColor);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDColor = ::_ttoi(ReturnString); }
	m_PhaseZeroLEDColor = m_DLPParam.m_LEDColor;
	m_PhaseFactorLEDColor = m_DLPParam.m_LEDColor;

	//Trig Type	
	KeyName.Format(_T("Trig Type")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigType);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_TrigType = ::_ttoi(ReturnString);  }

	//Bit Depth	
	KeyName.Format(_T("Bit Depth")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternBitDepth);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PatternBitDepth = ::_ttoi(ReturnString); }

	//TrigInterval_us	
	KeyName.Format(_T("Trig Interval")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigInterval_us);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_TrigInterval_us = ::_ttoi(ReturnString); }

	//PatternExp_us	
	KeyName.Format(_T("Pattern Exp")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternExp_us);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PatternExp_us = ::_ttoi(ReturnString); }

	//Phase Mode
	KeyName.Format(_T("Phase Mode")); KeyString.Format(_T("%d"),m_DLPParam.m_PhaseMode);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PhaseMode = ::_ttoi(ReturnString); }
	m_PhaseZeroPhaseMode = m_DLPParam.m_PhaseMode;
	m_PhaseFactorPhaseMode = m_DLPParam.m_PhaseMode;

	//Period
	KeyName.Format(_T("Period Time")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime_us);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PeriodTime_us = ::_ttoi(ReturnString); }

	//LED Exposure Time
	KeyName.Format(_T("Exposure Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime_us);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_ExposureTime_us = ::_ttoi(ReturnString); }

	m_DLPParam.m_PeriodTime2_us = m_DLPParam.m_PeriodTime_us;
	m_DLPParam.m_ExposureTime2_us = m_DLPParam.m_ExposureTime_us;
	//Period 2
	KeyName.Format(_T("Period Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime2_us);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PeriodTime2_us = ::_ttoi(ReturnString); }

	//LED Exposure Time 2
	KeyName.Format(_T("Exposure Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime2_us);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_ExposureTime2_us = ::_ttoi(ReturnString); }	

	if ( m_DLPParam.m_PeriodTime_us < m_DLPParam.m_ExposureTime_us )
	{	m_DLPParam.m_PeriodTime_us = m_DLPParam.m_ExposureTime_us; }

	if ( m_DLPParam.m_PeriodTime2_us < m_DLPParam.m_ExposureTime2_us )
	{	m_DLPParam.m_PeriodTime2_us = m_DLPParam.m_ExposureTime2_us; }

	//LEDCurrent_R	
	KeyName.Format(_T("LED Current R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_1);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentR_1 = ::_ttoi(ReturnString); }

	//LEDCurrent_G	
	KeyName.Format(_T("LED Current G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_1);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{	m_DLPParam.m_LEDCurrentG_1 = ::_ttoi(ReturnString); }

	//LEDCurrent_B	
	KeyName.Format(_T("LED Current B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_1);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentB_1 = ::_ttoi(ReturnString); }

	//LEDCurrent_R_2
	KeyName.Format(_T("LED Current R 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_2);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentR_2 = ::_ttoi(ReturnString); }

	//LEDCurrent_G_2	
	KeyName.Format(_T("LED Current G 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_2);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{	m_DLPParam.m_LEDCurrentG_2 = ::_ttoi(ReturnString); }

	//LEDCurrent_B_2
	KeyName.Format(_T("LED Current B 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_2);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentB_2 = ::_ttoi(ReturnString); }	


	//InvertPWM	
	KeyName.Format(_T("Invert PWM")); KeyString.Format(_T("%d"),m_DLPParam.m_InvertPWM);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_InvertPWM=true;	}
		else
		{	m_DLPParam.m_InvertPWM=false;	}	
	}

	//LEDEnabled_Auto	
	KeyName.Format(_T("LED Enabled Auto")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_Auto);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_Auto=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_Auto=false;	}	
	}

	//LEDEnabled_R	
	KeyName.Format(_T("LED Enabled R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_R);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_R=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_R=false;	}	
	}

	//LEDEnabled_G
	KeyName.Format(_T("LED Enabled G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_G);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_G=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_G=false;	}	
	}

	//LEDEnabled_B	
	KeyName.Format(_T("LED Enabled B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_B);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_B=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_B=false;	}	
	}

	//FlipLong	
	KeyName.Format(_T("Flip Long")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipLong);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_FlipLong=true;	}
		else
		{	m_DLPParam.m_FlipLong=false;	}	
	}

	//FlipShort
	KeyName.Format(_T("Flip Short")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipShort);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_FlipShort=true;	}
		else
		{	m_DLPParam.m_FlipShort=false;	}	
	}

	KeyName.Format(_T("Validate Delay Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ValidateDelayTime);	
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_ValidateDelayTime = ::_ttoi(ReturnString);
		if( m_DLPParam.m_ValidateDelayTime < 0 )
		{	m_DLPParam.m_ValidateDelayTime=0;	}		
	}

	KeyName.Format(_T("Use Exposure Pattern")); KeyString.Format(_T("%d"),m_DLPParam.m_UseExpLut);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_UseExpLut=true;	}
		else
		{	m_DLPParam.m_UseExpLut=false;	}	
	}

	KeyName.Format(_T("Phase Factor Min")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMin);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_PhaseFactorMin = ::_ttof(ReturnString);		
		if ( m_DLPParam.m_PhaseFactorMin < 0 ) 
		{	m_DLPParam.m_PhaseFactorMin = 0; } 
	}	

	KeyName.Format(_T("Phase Factor Max")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMax);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_PhaseFactorMax = ::_ttof(ReturnString);		
		if ( m_DLPParam.m_PhaseFactorMax < 0 ) 
		{	m_DLPParam.m_PhaseFactorMax = 0; } 
	}

	KeyName.Format(_T("LED Current Max")); KeyString.Format(_T("%d"), m_DLPParam.m_LEDCurrentMax);
	if (JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{
		m_DLPParam.m_LEDCurrentMax = ::_ttoi(ReturnString);
		if (m_DLPParam.m_LEDCurrentMax < 10)
		{	m_DLPParam.m_LEDCurrentMax = 10;	}
		if (m_DLPParam.m_LEDCurrentMax > 256)
		{	m_DLPParam.m_LEDCurrentMax = 256;	}
	}

	//Image Gamma
	KeyName.Format(_T("Image Gamma")); KeyString.Format(_T("%.6f"), m_DLPParam.m_ImageGamma);
	if (JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_ImageGamma = ::_ttof(ReturnString);	}
	m_ImageGamma = m_DLPParam.m_ImageGamma;

	KeyName.Format(_T("2nd Exposure Ratio")); KeyString.Format(_T("%.6f"), m_DLPParam.m_SecondExpRatio);
	if (JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_SecondExpRatio = ::_ttof(ReturnString);	}
	m_SecondExpRatio = m_DLPParam.m_SecondExpRatio;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ReadDLPParameter()//從DLP裝置讀取參數
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ReadDLPParameter"), MSG_LEVEL_HIGH);

	//重新取回參數
	bool SLmode=false;	
	m_OperationMode = DLP_OPERATION_DEFAULT;
	if ( DLPC350_GetMode(&SLmode) < 0  )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetMode"));		
		return false;
	}
	if ( SLmode == true )
	{	m_OperationMode = DLP_OPERATION_PATTERN;	}
	else
	{	m_OperationMode = DLP_OPERATION_VIDEO;	}		
	if ( DLPC350_SetPowerMode(false) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPowerMode"));		
		return false;
	}

	bool bSeqCtrl = false;
	bool bRed=false, bGreen=false, bBlue=false;
	if ( DLPC350_GetLedEnables(&bSeqCtrl, &bRed, &bGreen, &bBlue) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetLedEnables"));		
		return false;
	}
	this->m_DLPParam.m_LEDEnabled_Auto = bSeqCtrl;
	this->m_DLPParam.m_LEDEnabled_R = bRed;
	this->m_DLPParam.m_LEDEnabled_G = bGreen;
	this->m_DLPParam.m_LEDEnabled_B = bBlue;

	unsigned char cRed=0, cGreen=0, cBlue=0;	
	if ( DLPC350_GetLedCurrents(&cRed, &cGreen, &cBlue) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetLedCurrents"));		
		return false;
	}

	m_LEDCurrentR = 255-cRed;
	m_LEDCurrentG = 255-cGreen;
	m_LEDCurrentB = 255-cBlue;

	//this->m_DLPParam.m_LEDCurrentR_1 = 255-cRed;
	//this->m_DLPParam.m_LEDCurrentG_1 = 255-cGreen;
	//this->m_DLPParam.m_LEDCurrentB_1 = 255-cBlue;

	bool bInvert=false;
	if ( DLPC350_GetLEDPWMInvert(&bInvert) < 0 ) 
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetLEDPWMInvert"));		
		return false;
	}
	m_InvertPWM = bInvert;
	this->m_DLPParam.m_InvertPWM = bInvert;
	return true;
}
//-------------------------------------------------------------------------------------//
TDLPParam& CLight3DTiDLP_DLPC350_v4::GetDLPParam()
{
	return m_DLPParam;
}
//-------------------------------------------------------------------------------------//
const TDLPParam& CLight3DTiDLP_DLPC350_v4::GetDLPParam() const
{
	return m_DLPParam;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetDLPPhaseMode(int Mode)
{
	bool IsOK = true;
	switch ( Mode )
	{
	case LIGHT3D_PHASE_2_2_M:
	case LIGHT3D_PHASE_4_4_1:
	case LIGHT3D_PHASE_4_4_2:
	case LIGHT3D_PHASE_4_4_M:
	case LIGHT3D_PHASE_4_4_M_2:
		m_DLPParam.m_PhaseMode = Mode;
		break;
	default:
		IsOK = false;
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetDLPParamLEDCurrent(int red, int green, int blue, int CurrentID)
{
	switch ( CurrentID )
	{
	case DLP_LED_CURRENT_ID_02:
		m_DLPParam.m_LEDCurrentR_2 = red;
		m_DLPParam.m_LEDCurrentG_2 = green;
		m_DLPParam.m_LEDCurrentB_2 = blue;
		break;
	default:
	case DLP_LED_CURRENT_ID_01:
		m_DLPParam.m_LEDCurrentR_1 = red;
		m_DLPParam.m_LEDCurrentG_1 = green;
		m_DLPParam.m_LEDCurrentB_1 = blue;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetDLPParamLEDCurrent(int &red, int &green, int &blue, int CurrentID)
{
	switch ( CurrentID )
	{
	case DLP_LED_CURRENT_ID_02:
		red   = m_DLPParam.m_LEDCurrentR_2;
		green = m_DLPParam.m_LEDCurrentG_2;
		blue  = m_DLPParam.m_LEDCurrentB_2;
		break;
	default:
	case DLP_LED_CURRENT_ID_01:
		red   = m_DLPParam.m_LEDCurrentR_1;
		green = m_DLPParam.m_LEDCurrentG_1;
		blue  = m_DLPParam.m_LEDCurrentB_1;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP_DLPC350_v4::GetDLPFrmTag()
{
	return this->m_DLPFrmTag;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP_DLPC350_v4::GetTiAPIVersion()
{
	return this->m_TiAPIversion;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP_DLPC350_v4::GetDLPFrmVersion()
{
	return this->m_DLPFrmversion;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetUseExpLut()
{
	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	
	return m_DLPParam.m_UseExpLut;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetLEDColor(int Type)
{
	m_LEDColor = Type;	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetLEDColor() const
{
	return m_LEDColor;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP_DLPC350_v4::GetImageGamma() const
{
	//return 1.00f;
	//return 1.0f;
	return m_ImageGamma;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetImageGamma(double val)
{
	m_ImageGamma = val;
	m_DLPParam.m_ImageGamma = val;
	return true;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP_DLPC350_v4::GetSecondExpRatio() const//取得第2次曝光比例
{
	return m_SecondExpRatio;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetSecondExpRatio(double val)
{
	m_SecondExpRatio = val;
	m_DLPParam.m_SecondExpRatio = val;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetUse3BitPattern() const//取得使用3Bit樣板圖
{
	const int PatternBitCount=GetPatternBitCount();
	if ( 3 != PatternBitCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetUse5BitPattern() const//取得使用5Bit樣板圖
{
	const int PatternBitCount=GetPatternBitCount();
	if ( 5 != PatternBitCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternBitCount() const//取得樣板圖位元數
{
	return m_PatternBitCount;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternBitCount(int val)//設定樣板圖位元數
{
	m_PatternBitCount = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternIndex1_3Bit() const//取得使用3Bit樣板引數-1
{
	return m_PatternIndex1_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternIndex1_3Bit(int val)//設定使用3Bit樣板引數-1
{
	m_PatternIndex1_3Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternIndex2_3Bit() const//取得使用3Bit樣板引數-2
{
	return m_PatternIndex2_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternIndex2_3Bit(int val)//設定使用3Bit樣板引數-2
{
	m_PatternIndex2_3Bit = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternIndex1_5Bit() const//取得使用5Bit樣板引數-1
{
	return m_PatternIndex1_5Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternIndex1_5Bit(int val)//設定使用5Bit樣板引數-1
{
	m_PatternIndex1_5Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternIndex1_6Bit() const//取得使用6Bit樣板引數-1
{
	return m_PatternIndex1_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternIndex1_6Bit(int val)//設定使用6Bit樣板引數-1
{
	m_PatternIndex1_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternIndex2_6Bit() const//取得使用6Bit樣板引數-2
{
	return m_PatternIndex2_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternIndex2_6Bit(int val)//設定使用6Bit樣板引數-2
{
	m_PatternIndex2_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternIndexGC_1Bit() const//取得使用1Bit-GrayCode引數-1
{
	return m_PatternIndexGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternIndexGC_1Bit(int val)//設定使用1Bit-GrayCode引數-1
{
	m_PatternIndexGC_1Bit = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternIndexBC_1Bit() const//取得使用1Bit-BinaryCode引數-1
{
	return m_PatternIndexBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternIndexBC_1Bit(int val)//設定使用1Bit-BinaryCode引數-1
{
	m_PatternIndexBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternStartNumGC_1Bit() const//取得使用1Bit-GrayCode起始張數-1
{
	return m_PatternStartNumGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternStartNumGC_1Bit(int val)//設定使用1Bit-GrayCode起始張數-1
{
	m_PatternStartNumGC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetPatternStartNumBC_1Bit() const//取得使用1Bit-BinaryCode起始張數-1
{
	return m_PatternStartNumBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPatternStartNumBC_1Bit(int val)//設定使用1Bit-BinaryCode起始張數-1
{
	m_PatternStartNumBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//	
void CLight3DTiDLP_DLPC350_v4::SetPeriod_us(unsigned int val)
{
	m_TrigPeriod_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetExposure_us(unsigned int val)
{
	m_Exposure_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPeriod2_us(unsigned int val)
{
	m_TrigPeriod2_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetExposure2_us(unsigned int val)
{
	m_Exposure2_us = val;		
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::InitialPatItem(TDLPPatItem &PatItem)
{
	PatItem = TDLPPatItem();	
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatClear()//清除樣版內容
{
	SaveDLPProcess(_T("ExecDLPPatClear"), MSG_LEVEL_HIGH);

	this->m_PatternList.clear();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{
	if ( true == bExpLut )
	{	return ExecDLPPatRead_ExpLut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat); }
	return ExecDLPPatRead_Lut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatRead_Lut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }			

	SaveDLPProcess(_T("ExecDLPPatRead_Lut"), MSG_LEVEL_HIGH);

	CString str;		
	unsigned int i=0, j=0;	    
	unsigned int numLutEntries=0, numPatsForTrigOut2=0, numSplash=0;        
	int firstItem=32, lastItem=0, index=0;        
	bool Invert_Pat=false, Insert_Black=false, Buf_Swap=false, TrigOutPrev=false;    
	int trig_type=0, Pat_Num=0, Bit_Depth=0, LED_Select=0, Frame_Index=0, Fresh_Index=0;    
	bool patFromVideo=false;
	int patLutBytesRead=0, numLUTEntriesRead=0;    
	unsigned char splashLut[DLP_SPLASH_LUT_MAX];
	TDLPPatItem  PatItem;

	this->m_PatternList.clear();
	::memset(splashLut, 0x00, sizeof(splashLut));    
	int  nTrigIntExt=0;
	if ( DLPC350_GetPatternTriggerMode(&nTrigIntExt) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetPatternTriggerMode"));		
		return false;
	}
	if ( nTrigIntExt > 2 ) 
	{
		this->m_ErrorString = _T("System is configured in Variable Exposure Pattern Sequence Mode");
		return false;
	}
	if ( 0 == nTrigIntExt ) { bTrigIntExt = false; }
	else { bTrigIntExt = true; }

	if ( DLPC350_GetExposure_FramePeriod(&Exposure_us, &TrigPeriod_us) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetExposure_FramePeriod"));		
		return false;
	}
	if ( DLPC350_GetPatternDisplayMode(&bPatFrmVideo) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetPatternDisplayMode"));		
		return false;
	}	
	if ( DLPC350_GetPatternConfig(&numLutEntries, &bRepeat, &numPatsForTrigOut2, &numSplash) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetPatternConfig"));		
		return false;
	}	
	patLutBytesRead = DLPC350_GetPatLut(numLutEntries);
	if ( patLutBytesRead < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetPatLut"));		
		return false;
	}	
	if(patLutBytesRead % 3 == 0)
	{	numLUTEntriesRead = patLutBytesRead/3+1; }
	else
	{	numLUTEntriesRead = patLutBytesRead/3; }

	if (numLUTEntriesRead != numLutEntries)
	{
		str.Format(_T("Only %d pattern LUT entries read back correctly. This issue will be fixed in the next release of firmware"), numLUTEntriesRead);
		JetAPI::ShowMessageBox(str);			
		numLutEntries = numLUTEntriesRead;        
	}
	if ( DLPC350_GetImageLut(&splashLut[0], numSplash) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetImageLut"));		
		return false;
	}

	// Values read correctly so check for special 2 numSplash case and adjust so GUI displays correctly
	if ( numSplash == 2 )
	{
		unsigned char temp_val = splashLut[0];
		splashLut[0] = splashLut[1];
		splashLut[1] = temp_val;
	}

	Frame_Index = -1;
	patFromVideo = bPatFrmVideo;
	for(i=0; i<numLutEntries; i++)
	{
		if ( DLPC350_GetPatLutItem(i, &trig_type, &Pat_Num, &Bit_Depth, &LED_Select, &Invert_Pat, &Insert_Black, &Buf_Swap, &TrigOutPrev) < 0 )
		{
			this->SetLCRErrorFnName(_T("DLPC350_GetPatLutItem"));
			return false;
		}

		if ( Bit_Depth<DLP_BIT_DEPTH_1 || Bit_Depth>DLP_BIT_DEPTH_8)
		{
			this->m_ErrorString.Format(_T("Error, TiDLP Received unexpected value for Bit depth %d"), Bit_Depth); 
			return false;
		}        
		if ( LED_Select<DLP_LED_COLOR_RED || LED_Select>DLP_LED_COLOR_WHITE )
		{
			this->m_ErrorString.Format(_T("Error, TiDLP Received unexpected value for Color selection %d"), LED_Select);
			return false;
		}

		if ( (Buf_Swap) || (i==0) )
		{	Frame_Index++; }
		AOIDataDefine.CalcDLPBitPosRange(Bit_Depth, Pat_Num, firstItem, lastItem);

		//PatItem
		if ( true == patFromVideo ) 
		{	Fresh_Index = Frame_Index; }
		else
		{	Fresh_Index = splashLut[Frame_Index]; }

		PatItem.sFlashIndex = Fresh_Index;
		PatItem.sBitDepth = Bit_Depth;
		PatItem.sColorIndex = LED_Select;
		PatItem.sTrigType = trig_type;
		PatItem.sBitNum  = Pat_Num;
		PatItem.sBitStart = firstItem;
		PatItem.sBitEnd = lastItem;
		PatItem.sInvertPattern = Invert_Pat;
		PatItem.sInsertBlack = Insert_Black;
		PatItem.sTrigOutPrev = TrigOutPrev;
		PatItem.sBufSwap = Buf_Swap;
		PatItem.nPeriod = TrigPeriod_us;
		PatItem.nExposure = Exposure_us;
		this->m_PatternList.push_back(PatItem);			
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatRead_ExpLut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }			

	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	

	SaveDLPProcess(_T("ExecDLPPatRead_ExpLut"), MSG_LEVEL_HIGH);

	CString str;		
	unsigned int i=0, j=0;	    
	unsigned int numLutEntries=0, numPatsForTrigOut2=0, numSplash=0;        
	int firstItem=32, lastItem=0, index=0;        
	bool Invert_Pat=false, Insert_Black=false, Buf_Swap=false, TrigOutPrev=false;    
	int trig_type=0, Pat_Num=0, Bit_Depth=0, LED_Select=0, Frame_Index=0, Fresh_Index=0;    
	bool patFromVideo=false;
	int patLutBytesRead=0, numLUTEntriesRead=0;    
	unsigned char splashLut[DLP_SPLASH_EXP_LUT_MAX];
	TDLPPatItem  PatItem;

	this->m_PatternList.clear();
	::memset(splashLut, 0x00, sizeof(splashLut));
	int  nTrigIntExt=0;
	if ( DLPC350_GetPatternTriggerMode(&nTrigIntExt) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetPatternTriggerMode"));		
		return false;
	}
	if ( nTrigIntExt < 3 )
	{
		m_ErrorString = _T("System is configured in legacy Pattern Sequence Mode");
		return false;
	}

	if ( 4 == nTrigIntExt ) { bTrigIntExt = false; }
	else { bTrigIntExt = true; }
	if ( DLPC350_GetPatternDisplayMode(&bPatFrmVideo) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetPatternDisplayMode"));		
		return false;
	}	

	if ( DLPC350_GetVarExpPatternConfig(&numLutEntries, &numPatsForTrigOut2, &numSplash, &bRepeat) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetVarExpPatternConfig"));		
		return false;
	}	

	patLutBytesRead = DLPC350_GetVarExpPatLut(numLutEntries);
	if ( patLutBytesRead < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetPatLut"));		
		return false;
	}

	if ( DLPC350_GetvarExpImageLut(&splashLut[0], numSplash) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_GetvarExpImageLut"));		
		return false;
	}

	// Values read correctly so check for special 2 numSplash case and adjust so GUI displays correctly
	if ( numSplash == 2 )
	{
		unsigned char temp_val = splashLut[0];
		splashLut[0] = splashLut[1];
		splashLut[1] = temp_val;
	}


	Frame_Index = -1;
	patFromVideo = bPatFrmVideo;
	int Pat_Exposure = 0;
	int Pat_Period = 0;
	for(i=0; i<numLutEntries; i++)
	{		
		if ( DLPC350_GetVarExpPatLutItem(i, &trig_type, &Pat_Num, &Bit_Depth, &LED_Select, &Invert_Pat, &Insert_Black, &Buf_Swap, &TrigOutPrev, &Pat_Exposure, &Pat_Period) < 0 )
		{
			this->SetLCRErrorFnName(_T("DLPC350_GetVarExpPatLutItem"));
			return false;
		}

		if ( Bit_Depth<DLP_BIT_DEPTH_1 || Bit_Depth>DLP_BIT_DEPTH_8)
		{
			this->m_ErrorString.Format(_T("Error, TiDLP Received unexpected value for Bit depth %d"), Bit_Depth); 
			return false;
		}        
		if ( LED_Select<DLP_LED_COLOR_RED || LED_Select>DLP_LED_COLOR_WHITE )
		{
			this->m_ErrorString.Format(_T("Error, TiDLP Received unexpected value for Color selection %d"), LED_Select);
			return false;
		}

		if ( (Buf_Swap) || (i==0) )
		{	Frame_Index++; }
		AOIDataDefine.CalcDLPBitPosRange(Bit_Depth, Pat_Num, firstItem, lastItem);

		//PatItem
		if ( true == patFromVideo ) 
		{	Fresh_Index = Frame_Index; }
		else
		{	Fresh_Index = splashLut[Frame_Index]; }

		PatItem.sFlashIndex = Fresh_Index;
		PatItem.sBitDepth = Bit_Depth;
		PatItem.sColorIndex = LED_Select;
		PatItem.sTrigType = trig_type;
		PatItem.sBitNum  = Pat_Num;
		PatItem.sBitStart = firstItem;
		PatItem.sBitEnd = lastItem;
		PatItem.sInvertPattern = Invert_Pat;
		PatItem.sInsertBlack = Insert_Black;
		PatItem.sTrigOutPrev = TrigOutPrev;
		PatItem.sBufSwap = Buf_Swap;
		PatItem.nExposure = Pat_Exposure;
		PatItem.nPeriod = Pat_Period;
		this->m_PatternList.push_back(PatItem);			

		if ( 0 == Exposure_us )
		{	Exposure_us = Pat_Exposure;	}
		if ( 0 == TrigPeriod_us )
		{	TrigPeriod_us = Pat_Period;	}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{
	if ( true == bExpLut )
	{	return ExecDLPPatSendAll_ExpLut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat); }
	return ExecDLPPatSendAll_Lut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatSendAll_Lut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ExecDLPPatSendAll_Lut"), MSG_LEVEL_HIGH);

	CString str;
	CString ItemText;
	int i=0, numLutEntries=0;
	unsigned int status=0;        
	int numSplashLutEntries = 0;
	int num_pats_in_exposure=1;    
	int worstCaseBitDepth = 0;
	unsigned int min_pat_exposure[8] = {235, 700, 1570, 1700, 2000, 2500, 4500, 8333};
	unsigned int numPatterns;
	unsigned int total_exposure_time=0;
	unsigned int rogueExposureTime=0;
	unsigned int exposure_us = Exposure_us;
	unsigned char splashLut[DLP_SPLASH_LUT_MAX];
	bool displayTimeLoadTimeMismatch=false;

	int TriggerType = 0;
	int FlashIdx = 0;
	int PatNum = 0;
	int BitDepth = 0;
	int LEDIndex = 0;
	bool InvertPattern = false;
	bool InsertBlack = false;
	bool bufSwap = false;
	bool TrigOutPrev = false;	

	TDLPPatItem PatItem;
	const int PatItemCount = (int)(this->m_PatternList.size());	

	//Make sure the Pattern Exposure and Pattern Period timings are within the spec

	//Pattern Exposure > Pattern Period not a valid settings
	if( Exposure_us > TrigPeriod_us )
	{
		m_ErrorString = _T("Pattern exposure setting voilation, it should be, Pattern Exposure = Pattern Period or (Pattern Period - Pattern Exposure) > 230us");
		return false;
	}

	//If Pattern Exposure != Pattern Period then (Pattern Period - Pattern Exposure) > 230us
	if( (Exposure_us!=TrigPeriod_us) && ((TrigPeriod_us-Exposure_us) <= 230))
	{
		m_ErrorString = _T("Pattern exposure setting voilation, it should be, Pattern Exposure = Pattern Period or (Pattern Period - Pattern Exposure) > 230us");
		return false;
	}

	if ( DLPC350_ClearPatLut() < 0 )
	{	
		this->SetLCRErrorFnName(_T("DLPC350_ClearPatLut"));		
		return false;
	}
	::memset(splashLut, 0x00, sizeof(splashLut));
	for (i=0; i<PatItemCount; i++)
	{
		PatItem = this->m_PatternList[i];
		if ( 0==i && PatItem.sTrigType==DLP_TRIG_TYPE_NO_TRIG )
		{
			m_ErrorString = _T("First Item must be triggered. Please select a Trigger_In_Type other than No Trigger");			
			return false;
		}

		TriggerType = PatItem.sTrigType;
		FlashIdx = PatItem.sFlashIndex;
		PatNum = PatItem.sBitNum;
		BitDepth = PatItem.sBitDepth;
		LEDIndex = PatItem.sColorIndex;
		InvertPattern = PatItem.sInvertPattern;
		InsertBlack = PatItem.sInsertBlack;
		bufSwap = PatItem.sBufSwap;
		TrigOutPrev = PatItem.sTrigOutPrev;		

		//Expsosure time validation logic begin/	
		//If trigOut = NEW
		if ( TrigOutPrev == false )
		{
			if ( num_pats_in_exposure!=1)
			{
				//Check if expsoure time is above the minimum requirement
				if( (exposure_us/num_pats_in_exposure) < min_pat_exposure[worstCaseBitDepth] )
				{	
					m_ErrorString.Format(_T("Exposure time %d < Minimum Exposure time %d for bit depth %d"), exposure_us/num_pats_in_exposure, min_pat_exposure[worstCaseBitDepth], worstCaseBitDepth+1);					
					return false;
				}
			}
			if( (BitDepth-1) > worstCaseBitDepth)
			{	worstCaseBitDepth = BitDepth-1;	}
			num_pats_in_exposure=1;
			worstCaseBitDepth = 0;
		}
		else //if trigOut = PREV
		{
			num_pats_in_exposure++;
			if ( BitDepth-1 > worstCaseBitDepth)
			{	worstCaseBitDepth = BitDepth-1; }
		}
		//Expsosure time validation logic End//		

		if ( DLPC350_AddToPatLut(PatItem.sTrigType, PatItem.sBitNum, PatItem.sBitDepth, PatItem.sColorIndex, PatItem.sInvertPattern, PatItem.sInsertBlack, PatItem.sBufSwap, PatItem.sTrigOutPrev) < 0 )
		{
			this->SetLCRErrorFnName(_T("DLPC350_AddToPatLut"));			
			return false;
		}

		//If there is a buffer swap or if this is the first pattern
		if( true==bufSwap || (numSplashLutEntries==0))
		{
			if (numSplashLutEntries >= DLP_SPLASH_LUT_MAX )
			{
				this->m_ErrorString = _T("Image LUT entries(64) reached maximum. Will not add anymore entries\n");				
				return false;
			}
			else
			{	splashLut[numSplashLutEntries++] = FlashIdx;	}
		}
		numLutEntries++;		
	}	
	if ( m_numExtraSplashLutEntries > 0 )
	{
		for ( i=0; i<m_numExtraSplashLutEntries; i++)
		{
			if (numSplashLutEntries >= DLP_SPLASH_LUT_MAX)
			{
				this->m_ErrorString = _T("Image LUT entries(64) reached maximum. Will not add anymore entries\n");				
				return false;
			}
			splashLut[numSplashLutEntries++] = m_ExtraSplashLutEntries[i];
		}
	}

	if ( num_pats_in_exposure!=1 )
	{
		//Check if expsoure time is above the minimum requirement
		if ( (exposure_us/num_pats_in_exposure) < min_pat_exposure[worstCaseBitDepth])
		{
			this->m_ErrorString.Format(_T("Exposure time %d < Minimum Exposure time %d for bit depth %d"), exposure_us/num_pats_in_exposure, min_pat_exposure[worstCaseBitDepth], worstCaseBitDepth+1);            
			return false;
		}
	}

	if ( DLPC350_SetPatternDisplayMode(bPatFrmVideo) < 0 ) //from vedio?
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternDisplayMode"));		
		return false;
	}


	//if play once is selected
	if( false == bRepeat )//one
	{	numPatterns = numLutEntries;	}
	else//repeat
	{   
		numPatterns = 1;//??
	}

	if ( DLPC350_SetPatternConfig(numLutEntries, bRepeat, numPatterns, numSplashLutEntries) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternConfig"));
		return false;
	}	

	if ( DLPC350_SetExposure_FramePeriod(Exposure_us, TrigPeriod_us) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetExposure_FramePeriod"));
		return false;
	}

	int trigMode = 0;
	if ( true == bTrigIntExt )//Internal trigger
	{	trigMode = 1; }
	else //VSync trigger
	{	trigMode = 0; }
	if ( DLPC350_SetPatternTriggerMode(trigMode) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternTriggerMode"));
		return false;
	}

	if ( DLPC350_SendPatLut() < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SendPatLut"));
		return false;
	}	

	if ( DLPC350_SendImageLut(&splashLut[0], numSplashLutEntries) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SendImageLut"));
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatSendAll_ExpLut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	

	SaveDLPProcess(_T("ExecDLPPatSendAll_ExpLut"), MSG_LEVEL_HIGH);

	CString str;
	CString ItemText;
	int i=0, numLutEntries=0;
	unsigned int status=0;        
	int numSplashLutEntries = 0;
	int num_pats_in_exposure=1;    
	int worstCaseBitDepth = 0;
	unsigned int min_pat_exposure[8] = {235, 700, 1570, 1700, 2000, 2500, 4500, 8333};
	unsigned int numPatterns;
	unsigned int total_exposure_time=0;
	unsigned int rogueExposureTime=0;
	unsigned int exposure_us = Exposure_us;
	unsigned char splashLut[DLP_SPLASH_EXP_LUT_MAX];
	bool displayTimeLoadTimeMismatch=false;

	int TriggerType = 0;
	int FlashIdx = 0;
	int PatNum = 0;
	int BitDepth = 0;
	int LEDIndex = 0;
	bool InvertPattern = false;
	bool InsertBlack = false;
	bool bufSwap = false;
	bool TrigOutPrev = false;	

	TDLPPatItem PatItem;
	const int PatItemCount = (int)(this->m_PatternList.size());	
	if ( DLPC350_ClearExpLut() < 0 )
	{	
		this->SetLCRErrorFnName(_T("DLPC350_ClearExpLut"));		
		return false;
	}
	::memset(splashLut, 0x00, sizeof(splashLut));
	for (i=0; i<PatItemCount; i++)
	{
		PatItem = this->m_PatternList[i];
		if ( 0==i && PatItem.sTrigType==DLP_TRIG_TYPE_NO_TRIG )
		{
			m_ErrorString = _T("First Item must be triggered. Please select a Trigger_In_Type other than No Trigger");			
			return false;
		}

		TriggerType = PatItem.sTrigType;
		FlashIdx = PatItem.sFlashIndex;
		PatNum = PatItem.sBitNum;
		BitDepth = PatItem.sBitDepth;
		LEDIndex = PatItem.sColorIndex;
		InvertPattern = PatItem.sInvertPattern;
		InsertBlack = PatItem.sInsertBlack;
		bufSwap = PatItem.sBufSwap;
		TrigOutPrev = PatItem.sTrigOutPrev;		

		//Expsosure time validation logic begin/	
		//If trigOut = NEW
		if ( TrigOutPrev == false )
		{
			if ( num_pats_in_exposure!=1)
			{
				//Check if expsoure time is above the minimum requirement
				if( (exposure_us/num_pats_in_exposure) < min_pat_exposure[worstCaseBitDepth] )
				{	
					m_ErrorString.Format(_T("Exposure time %d < Minimum Exposure time %d for bit depth %d"), exposure_us/num_pats_in_exposure, min_pat_exposure[worstCaseBitDepth], worstCaseBitDepth+1);					
					return false;
				}
			}
			if( (BitDepth-1) > worstCaseBitDepth)
			{	worstCaseBitDepth = BitDepth-1;	}
			num_pats_in_exposure=1;
			worstCaseBitDepth = 0;
		}
		else //if trigOut = PREV
		{
			num_pats_in_exposure++;
			if ( BitDepth-1 > worstCaseBitDepth)
			{	worstCaseBitDepth = BitDepth-1; }
		}
		//Expsosure time validation logic End//		

		if ( DLPC350_AddToExpLut(PatItem.sTrigType, PatItem.sBitNum, PatItem.sBitDepth, PatItem.sColorIndex, PatItem.sInvertPattern, PatItem.sInsertBlack, PatItem.sBufSwap, PatItem.sTrigOutPrev, PatItem.nExposure, PatItem.nPeriod) < 0 )
		{
			this->SetLCRErrorFnName(_T("DLPC350_AddToExpLut"));			
			return false;
		}

		//If there is a buffer swap or if this is the first pattern
		if( true==bufSwap || (numSplashLutEntries==0))
		{
			if (numSplashLutEntries >= DLP_SPLASH_EXP_LUT_MAX )
			{
				this->m_ErrorString = _T("Image LUT entries(256) reached maximum. Will not add anymore entries\n");				
				return false;
			}
			else
			{	splashLut[numSplashLutEntries++] = FlashIdx;	}
		}
		numLutEntries++;		
	}	   

	//Set Pattern Mode - Video or Flash
	if ( DLPC350_SetPatternDisplayMode(bPatFrmVideo) < 0 ) //from vedio?
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternDisplayMode"));		
		return false;
	}	

	//if play once is selected
	if( false == bRepeat )//one
	{	numPatterns = numLutEntries;	}
	else//repeat
	{   
		numPatterns = 1;//??
	}

	//Pattern Sequence Configuration, DLPC350_SetPatternConfig
	if ( DLPC350_SetVarExpPatternConfig(numLutEntries, numPatterns, numSplashLutEntries, bRepeat) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetVarExpPatternConfig"));
		return false;
	}			

	int trigMode = 0;
	if ( true == bTrigIntExt )//Internal trigger
	{	trigMode = 3; }
	else //VSync trigger
	{	trigMode = 4; }
	//Configure Trigger Mode - 3 or 4 //Applicable for Variable Exposure pat sequence
	if ( DLPC350_SetPatternTriggerMode(trigMode) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternTriggerMode"));
		return false;
	}

	//Send Variable Exposure pattern LUT
	if ( DLPC350_SendVarExpPatLut() < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SendVarExpPatLut"));
		return false;
	}	

	if ( DLPC350_SendVarExpImageLut(&splashLut[0], numSplashLutEntries) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SendVarExpImageLut"));
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( true == bExpLut )
	{	return ExecDLPPatSendOne_ExpLut(index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	}
	return ExecDLPPatSendOne_Lut(index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatSendOne_Lut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ExecDLPPatSendOne_Lut"), MSG_LEVEL_HIGH);

	CString str;
	CString ItemText;
	int i=0, numLutEntries=0;
	unsigned int status=0;        
	int numSplashLutEntries = 0;
	int num_pats_in_exposure=1;    
	int worstCaseBitDepth = 0;
	unsigned int min_pat_exposure[8] = {235, 700, 1570, 1700, 2000, 2500, 4500, 8333};
	unsigned int numPatterns;
	unsigned int total_exposure_time=0;
	unsigned int rogueExposureTime=0;
	unsigned int exposure_us = Exposure_us;
	unsigned char splashLut[DLP_SPLASH_LUT_MAX];
	bool displayTimeLoadTimeMismatch=false;

	int TriggerType = 0;
	int FlashIdx = 0;
	int PatNum = 0;
	int BitDepth = 0;
	int LEDIndex = 0;
	bool InvertPattern = false;
	bool InsertBlack = false;
	bool bufSwap = false;
	bool TrigOutPrev = false;	

	TDLPPatItem PatItem;
	const int PatItemCount = (int)(this->m_PatternList.size());	
	if ( DLPC350_ClearPatLut() < 0 )
	{	
		this->SetLCRErrorFnName(_T("DLPC350_ClearPatLut"));		
		return false;
	}
	if ( index<0 || index>=PatItemCount )
	{
		this->m_ErrorString.Format(_T("Error, index out of pattern list range (%d)"), PatItemCount);
		return false;
	}
	::memset(splashLut, 0x00, sizeof(splashLut));    
	PatItem = this->m_PatternList[index];
	PatItem.sTrigType = DLP_TRIG_TYPE_INTERNAL;
	PatItem.sBufSwap = true;
	if (PatItem.sTrigType==DLP_TRIG_TYPE_NO_TRIG )
	{
		m_ErrorString = _T("First Item must be triggered. Please select a Trigger_In_Type other than No Trigger");			
		return false;
	}

	TriggerType = PatItem.sTrigType;
	FlashIdx = PatItem.sFlashIndex;
	PatNum = PatItem.sBitNum;
	BitDepth = PatItem.sBitDepth;
	LEDIndex = PatItem.sColorIndex;
	InvertPattern = PatItem.sInvertPattern;
	InsertBlack = PatItem.sInsertBlack;
	bufSwap = PatItem.sBufSwap;
	TrigOutPrev = PatItem.sTrigOutPrev;		

	//Expsosure time validation logic begin/	
	//If trigOut = NEW
	if ( TrigOutPrev == false )
	{
		if ( num_pats_in_exposure!=1)
		{
			//Check if expsoure time is above the minimum requirement
			if( (exposure_us/num_pats_in_exposure) < min_pat_exposure[worstCaseBitDepth] )
			{	
				m_ErrorString.Format(_T("Exposure time %d < Minimum Exposure time %d for bit depth %d"), exposure_us/num_pats_in_exposure, min_pat_exposure[worstCaseBitDepth], worstCaseBitDepth+1);					
				return false;
			}
		}
		if( (BitDepth-1) > worstCaseBitDepth)
		{	worstCaseBitDepth = BitDepth-1;	}
		num_pats_in_exposure=1;
		worstCaseBitDepth = 0;
	}
	else //if trigOut = PREV
	{
		num_pats_in_exposure++;
		if ( BitDepth-1 > worstCaseBitDepth)
		{	worstCaseBitDepth = BitDepth-1; }
	}
	//Expsosure time validation logic End//		

	if ( DLPC350_AddToPatLut(PatItem.sTrigType, PatItem.sBitNum, PatItem.sBitDepth, PatItem.sColorIndex, PatItem.sInvertPattern, PatItem.sInsertBlack, PatItem.sBufSwap, PatItem.sTrigOutPrev) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_AddToPatLut"));			
		return false;
	}

	//If there is a buffer swap or if this is the first pattern
	if( true==bufSwap || (numSplashLutEntries==0))
	{
		if (numSplashLutEntries >= DLP_SPLASH_LUT_MAX )
		{
			this->m_ErrorString = _T("Image LUT entries(64) reached maximum. Will not add anymore entries\n");				
			return false;
		}
		else
		{	splashLut[numSplashLutEntries++] = FlashIdx;	}
	}
	numLutEntries++;		

	if ( m_numExtraSplashLutEntries > 0 )
	{
		for ( i=0; i<m_numExtraSplashLutEntries; i++)
		{
			if (numSplashLutEntries >= DLP_SPLASH_LUT_MAX)
			{
				this->m_ErrorString = _T("Image LUT entries(64) reached maximum. Will not add anymore entries\n");				
				return false;
			}
			splashLut[numSplashLutEntries++] = m_ExtraSplashLutEntries[i];
		}
	}

	if ( num_pats_in_exposure!=1 )
	{
		//Check if expsoure time is above the minimum requirement
		if ( (exposure_us/num_pats_in_exposure) < min_pat_exposure[worstCaseBitDepth])
		{
			this->m_ErrorString.Format(_T("Exposure time %d < Minimum Exposure time %d for bit depth %d"), exposure_us/num_pats_in_exposure, min_pat_exposure[worstCaseBitDepth], worstCaseBitDepth+1);            
			return false;
		}
	}

	if ( DLPC350_SetPatternDisplayMode(bPatFrmVideo) < 0 ) //from vedio?
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternDisplayMode"));		
		return false;
	}	

	//if play once is selected
	if( false == bRepeat )//one
	{	numPatterns = numLutEntries;	}
	else//repeat
	{   
		numPatterns = 1;//??
	}

	if ( DLPC350_SetPatternConfig(numLutEntries, bRepeat, numPatterns, numSplashLutEntries) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternConfig"));
		return false;
	}	

	if ( DLPC350_SetExposure_FramePeriod(Exposure_us, TrigPeriod_us) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetExposure_FramePeriod"));
		return false;
	}

	int trigMode = 0;
	if ( true == bTrigIntExt )//Internal trigger
	{	trigMode = 1; }
	else //VSync trigger
	{	trigMode = 0; }
	if ( DLPC350_SetPatternTriggerMode(trigMode) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternTriggerMode"));
		return false;
	}

	if ( DLPC350_SendPatLut() < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SendPatLut"));
		return false;
	}

	if ( DLPC350_SendImageLut(&splashLut[0], numSplashLutEntries) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SendImageLut"));
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatSendOne_ExpLut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	

	SaveDLPProcess(_T("ExecDLPPatSendOne_ExpLut"), MSG_LEVEL_HIGH);

	CString str;
	CString ItemText;
	int i=0, numLutEntries=0;
	unsigned int status=0;        
	int numSplashLutEntries = 0;
	int num_pats_in_exposure=1;    
	int worstCaseBitDepth = 0;
	unsigned int min_pat_exposure[8] = {235, 700, 1570, 1700, 2000, 2500, 4500, 8333};
	unsigned int numPatterns;
	unsigned int total_exposure_time=0;
	unsigned int rogueExposureTime=0;
	unsigned int exposure_us = Exposure_us;
	unsigned char splashLut[DLP_SPLASH_EXP_LUT_MAX];
	bool displayTimeLoadTimeMismatch=false;

	int TriggerType = 0;
	int FlashIdx = 0;
	int PatNum = 0;
	int BitDepth = 0;
	int LEDIndex = 0;
	bool InvertPattern = false;
	bool InsertBlack = false;
	bool bufSwap = false;
	bool TrigOutPrev = false;	

	TDLPPatItem PatItem;
	const int PatItemCount = (int)(this->m_PatternList.size());	
	if ( DLPC350_ClearExpLut() < 0 )
	{	
		this->SetLCRErrorFnName(_T("DLPC350_ClearExpLut"));		
		return false;
	}
	if ( index<0 || index>=PatItemCount )
	{
		this->m_ErrorString.Format(_T("Error, index out of pattern list range (%d)"), PatItemCount);
		return false;
	}
	::memset(splashLut, 0x00, sizeof(splashLut));    
	PatItem = this->m_PatternList[index];
	PatItem.sTrigType = DLP_TRIG_TYPE_INTERNAL;
	PatItem.sBufSwap = true;
	if (PatItem.sTrigType==DLP_TRIG_TYPE_NO_TRIG )
	{
		m_ErrorString = _T("First Item must be triggered. Please select a Trigger_In_Type other than No Trigger");			
		return false;
	}

	TriggerType = PatItem.sTrigType;
	FlashIdx = PatItem.sFlashIndex;
	PatNum = PatItem.sBitNum;
	BitDepth = PatItem.sBitDepth;
	LEDIndex = PatItem.sColorIndex;
	InvertPattern = PatItem.sInvertPattern;
	InsertBlack = PatItem.sInsertBlack;
	bufSwap = PatItem.sBufSwap;
	TrigOutPrev = PatItem.sTrigOutPrev;		

	//Expsosure time validation logic begin/	
	//If trigOut = NEW
	if ( TrigOutPrev == false )
	{
		if ( num_pats_in_exposure!=1)
		{
			//Check if expsoure time is above the minimum requirement
			if( (exposure_us/num_pats_in_exposure) < min_pat_exposure[worstCaseBitDepth] )
			{	
				m_ErrorString.Format(_T("Exposure time %d < Minimum Exposure time %d for bit depth %d"), exposure_us/num_pats_in_exposure, min_pat_exposure[worstCaseBitDepth], worstCaseBitDepth+1);					
				return false;
			}
		}
		if( (BitDepth-1) > worstCaseBitDepth)
		{	worstCaseBitDepth = BitDepth-1;	}
		num_pats_in_exposure=1;
		worstCaseBitDepth = 0;
	}
	else //if trigOut = PREV
	{
		num_pats_in_exposure++;
		if ( BitDepth-1 > worstCaseBitDepth)
		{	worstCaseBitDepth = BitDepth-1; }
	}
	//Expsosure time validation logic End//		

	if ( DLPC350_AddToExpLut(PatItem.sTrigType, PatItem.sBitNum, PatItem.sBitDepth, PatItem.sColorIndex, PatItem.sInvertPattern, PatItem.sInsertBlack, PatItem.sBufSwap, PatItem.sTrigOutPrev, PatItem.nExposure, PatItem.nPeriod) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_AddToExpLut"));			
		return false;
	}

	//If there is a buffer swap or if this is the first pattern
	if( true==bufSwap || (numSplashLutEntries==0))
	{
		if (numSplashLutEntries >= DLP_SPLASH_EXP_LUT_MAX )
		{
			this->m_ErrorString = _T("Image LUT entries(256) reached maximum. Will not add anymore entries\n");				
			return false;
		}
		else
		{	splashLut[numSplashLutEntries++] = FlashIdx;	}
	}
	numLutEntries++;

	//if play once is selected
	if( false == bRepeat )//one
	{	numPatterns = numLutEntries;	}
	else//repeat
	{   
		numPatterns = 1;//??
	}

	//Pattern Sequence Configuration, DLPC350_SetPatternConfig
	if ( DLPC350_SetVarExpPatternConfig(numLutEntries, numPatterns, numSplashLutEntries, bRepeat) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetVarExpPatternConfig"));
		return false;
	}		

	int trigMode = 0;
	if ( true == bTrigIntExt )//Internal trigger
	{	trigMode = 3; }
	else //VSync trigger
	{	trigMode = 4; }
	//Configure Trigger Mode - 3 or 4 //Applicable for Variable Exposure pat sequence
	if ( DLPC350_SetPatternTriggerMode(trigMode) < 0)
	{
		this->SetLCRErrorFnName(_T("DLPC350_SetPatternTriggerMode"));
		return false;
	}

	//Send Variable Exposure pattern LUT
	if ( DLPC350_SendVarExpPatLut() < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SendVarExpPatLut"));
		return false;
	}	

	if ( DLPC350_SendVarExpImageLut(&splashLut[0], numSplashLutEntries) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_SendVarExpImageLut"));
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep)//套用樣板列表資料
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ExecDLPValidatePatLutData"), MSG_LEVEL_HIGH);

	if ( DLPC350_ValidatePatLutData(&Status) < 0 )
	{
		this->SetLCRErrorFnName(_T("DLPC350_ValidatePatLutData"));
		return false;
	}

	if ( true == Sleep )
	{
		if ( m_DLPParam.m_ValidateDelayTime > 0 ) 
		{	::Sleep(m_DLPParam.m_ValidateDelayTime); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force)//建立樣板, 傳送樣板以及驗證
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }	

	if ( Periodus < exposure_us )
	{	Periodus = exposure_us; }
	const unsigned int Exposure_us = exposure_us;
	const unsigned int TrigPeriod_us = Periodus;	

	bool HasSetBefore = true;
	if ( true == Force )//強迫更新
	{	HasSetBefore = false;	}
	else
	{
		if ( Mode==DLP_PATTERN_SEQUENCE_DEBUG || m_PatternMode!=Mode || m_Exposure_us!=Exposure_us || m_TrigPeriod_us!=TrigPeriod_us || m_LEDColor!=LEDColor )
		{	HasSetBefore = false;	}
	}
	if ( true == HasSetBefore )
	{	return true; }	

	TDLPParam &DLPParam = GetDLPParam();	
	DLPParam.m_PeriodTime_us = TrigPeriod_us;
	DLPParam.m_ExposureTime_us = Exposure_us;	
	if ( this->BuildDLPPatternList(Mode, IntTrig, MultiTable, LEDColor) == false )
	{	return false;	}

	const bool bPatFrmVideo = false;
	const bool bTrigIntExt = true;
#ifndef LIGHT_CTRL_DISABLE
	const bool bRepeat = true;
#else
	const bool bRepeat = false;
#endif//LIGHT_CTRL_DISABLE
	const bool bExpLut = GetUseExpLut();
	SaveDLPProcess(_T("ExecDLPPatBuildSendValidate"), MSG_LEVEL_HIGH);
	if ( this->ExecDLPPatSendAll(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{	return false;	}

	unsigned int Status=0;
	if ( this->ExecDLPValidatePatLutData(Status, Sleep) == false )
	{	return false;	}
	if ( Status != 0 )
	{
		this->m_ErrorString = _T("");
		if ( (Status&BIT0)==BIT0 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Exposure and Period OOR)");	}
		else if ( (Status&BIT1)==BIT1 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Pattern Number OOR)");	}
		else if ( (Status&BIT2)==BIT2 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Count trigger out ovelaps black)");	}
		else if ( (Status&BIT3)==BIT3 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Black vector missing)");	}
		else if ( (Status&BIT4)==BIT4 )
		{	this->m_ErrorString = _T("Error, TIDLP Exception (Exposure, Period diff < 230 )");	}
		else
		{	this->m_ErrorString.Format(_T("Error, TIDLP Exception (%d)"), Status); }
		return false; 
	}
	m_PatternMode = Mode;
	SetLEDColor(LEDColor);
	SetPeriod_us(TrigPeriod_us);
	SetExposure_us(Exposure_us);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPLightSetting(int CurrentID)//執行DLP的LED設定-依據目前的設定
{
	const bool bUpdate = false;
	int CurRed=0, CurGrn=0, CurBlu= 0;
	CLight3DTiDLP_DLPC350_v4 *Light3DPtr = this;
	Light3DPtr->GetDLPParamLEDCurrent(CurRed, CurGrn, CurBlu, CurrentID);
	if ( Light3DPtr->SetDLPLEDEnable(true, true, true, true) == false )
	{	return false;	}		
	if ( Light3DPtr->SetDLPLEDCurrent(CurRed, CurGrn, CurBlu, bUpdate, CurrentID) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPattern_Run()
{
	if ( this->GetDLPIsConnected() == false ) { return false; }	
	/*
	bool repeat;
	unsigned int numLutEntries, numPatsForTrigOut2, numSplash;    
	if( DLPC350_GetPatternConfig(&numLutEntries, &repeat, &numPatsForTrigOut2, &numSplash) == -1 )
	{
	this->m_ErrorString.Format("Get Pat Config Error!");
	return false;
	}
	*/
	SaveDLPProcess(_T("ExecDLPPattern_Run"), MSG_LEVEL_HIGH);

	if( DLPC350_PatternDisplay(2) < 0 )
	{ 
		this->SetLCRErrorFnName(_T("DLPC350_PatternDisplay(2)"));		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPattern_Stop()
{
	if ( this->GetDLPIsConnected() == false ) { return false; }	

	SaveDLPProcess(_T("ExecDLPPattern_Stop"), MSG_LEVEL_HIGH);

	if ( DLPC350_PatternDisplay(0) < 0 ) 
	{
		this->SetLCRErrorFnName(_T("DLPC350_PatternDisplay(0)"));		
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ExecDLPPattern_Pause()
{
	if ( this->GetDLPIsConnected() == false ) { return false; }	

	SaveDLPProcess(_T("ExecDLPPattern_Pause"), MSG_LEVEL_HIGH);
	if ( DLPC350_PatternDisplay(1) < 0 ) 
	{
		this->SetLCRErrorFnName(_T("DLPC350_PatternDisplay(1)"));		
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LEDSetting(int LEDCurrent, int CurrentID)
{
	bool InvertPWM = false;
	const bool bUpdate = false;
	int LEDCurrent_R=150, LEDCurrent_G=150, LEDCurrent_B=150;
	bool LEDEnabled_Auto=true, LEDEnabled_R=true, LEDEnabled_G=true, LEDEnabled_B=true;	

	InvertPWM = this->m_DLPParam.m_InvertPWM;
	LEDEnabled_Auto = this->m_DLPParam.m_LEDEnabled_Auto;
	LEDEnabled_R = this->m_DLPParam.m_LEDEnabled_R;
	LEDEnabled_G = this->m_DLPParam.m_LEDEnabled_G;
	LEDEnabled_B = this->m_DLPParam.m_LEDEnabled_B;
	GetDLPParamLEDCurrent(LEDCurrent_R, LEDCurrent_G, LEDCurrent_B, CurrentID);	
	SaveDLPProcess(_T("LEDSetting"), MSG_LEVEL_HIGH);

	if( SetDLPLEDEnable(LEDEnabled_Auto, LEDEnabled_R, LEDEnabled_G , LEDEnabled_B) == false ) { return false; }
	if( SetDLPLEDCurrent(LEDCurrent_R, LEDCurrent_G, LEDCurrent_B, bUpdate, CurrentID) == false ) { return false; }
	if( SetDLPLEDPWMInvert(InvertPWM) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP_DLPC350_v4::GetTriggerOutCount()
{
	return m_TrigOutCount;
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP_DLPC350_v4::GetDLPPatCount()
{
	return m_PatternList.size();
}
//-------------------------------------------------------------------------------------//
TDLPPatItem* CLight3DTiDLP_DLPC350_v4::GetDLPPatItemPtr(size_t index, bool check)
{
	if ( true == check )
	{
		const size_t size = m_PatternList.size();
		if ( index >= size ) 
		{	return NULL; }
	}
	return &(m_PatternList[index]);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::ClearDLPPatternList()//清除m_PatternList
{
	SaveDLPProcess(_T("ClearDLPPatternList"), MSG_LEVEL_HIGH);
	m_PatternList.clear();
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表 
{		
	SaveDLPProcess(_T("BuildDLPPatternList"), MSG_LEVEL_HIGH);

	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	m_PatternList.clear();
	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8

	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	if ( FlashIndexA<0 || FlashIndexB<0 || FlashIndexA>=NumImagesInFlash || FlashIndexB>=NumImagesInFlash )
	{
		m_ErrorString = _T("Error, DLP Pattern Index Exception");
		return false;
	}

	if ( DLP_PATTERN_SEQUENCE_WHITE == Mode )
	{
		PatItem.sFlashIndex = FlashIndexA;
		PatItem.sBitDepth = DLP_BIT_DEPTH_1;
		PatItem.sColorIndex = DLPLEDColor;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 24;	
		PatItem.sInvertPattern = true;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod1;
		PatItem.nExposure = PatExposure1;
		this->AddDLPPPatItem(PatItem);
		index ++;
	}
	else if ( DLP_PATTERN_SEQUENCE_RGB == Mode )
	{
		PatItem.sFlashIndex = FlashIndexA;
		PatItem.sBitDepth = DLP_BIT_DEPTH_1;
		PatItem.sColorIndex = DLP_LED_COLOR_RED;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 24;	
		PatItem.sInvertPattern = true;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod1;
		PatItem.nExposure = PatExposure1;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sColorIndex = DLP_LED_COLOR_GREEN;
		PatItem.sBitNum = 24;	
		PatItem.sBufSwap = false;		
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sColorIndex = DLP_LED_COLOR_BLUE;
		PatItem.sBitNum = 24;	
		PatItem.sBufSwap = false;		
		this->AddDLPPPatItem(PatItem);
		index ++;
	}
	else if ( DLP_PATTERN_SEQUENCE_2_2_M == Mode )
	{
		//6x(2+2+1)=30
		PatItem.sFlashIndex = FlashIndexA;
		PatItem.sBitDepth = DLP_BIT_DEPTH_6;
		PatItem.sColorIndex = DLPLEDColor;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;
		PatItem.sInvertPattern = false;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod1;
		PatItem.nExposure = PatExposure1;
		this->AddDLPPPatItem(PatItem);
		index++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index++;

		//second
		PatItem.sFlashIndex = FlashIndexB;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;//000
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod2;
		PatItem.nExposure = PatExposure2;
		this->AddDLPPPatItem(PatItem);
		index ++;

		//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		//PatItem.sBitNum = 1;//270	
		//PatItem.sBufSwap = false;
		//this->AddDLPPPatItem(PatItem);
		//index ++;

		//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		//PatItem.sBitNum = 2;//180
		//PatItem.sBufSwap = false;
		//this->AddDLPPPatItem(PatItem);
		//index ++;

		//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		//PatItem.sBitNum = 3;//090
		//PatItem.sBufSwap = false;
		//this->AddDLPPPatItem(PatItem);
		//index ++;
	}
	else if ( DLP_PATTERN_SEQUENCE_4_4_1 == Mode )
	{
		//6x4=24
		PatItem.sFlashIndex = FlashIndexA;
		PatItem.sBitDepth = DLP_BIT_DEPTH_6;
		PatItem.sColorIndex = DLPLEDColor;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;	
		PatItem.sInvertPattern = false;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod1;
		PatItem.nExposure = PatExposure1;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;
	}
	else if ( DLP_PATTERN_SEQUENCE_4_4_2 == Mode )
	{
		//6x4=24
		PatItem.sFlashIndex = FlashIndexB;
		PatItem.sBitDepth = DLP_BIT_DEPTH_6;
		PatItem.sColorIndex = DLPLEDColor;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;	
		PatItem.sInvertPattern = false;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod2;
		PatItem.nExposure = PatExposure2;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;
	}	
	else if ( DLP_PATTERN_SEQUENCE_4_2_M == Mode )
	{
		//6x4+6x2=32
		PatItem.sFlashIndex = FlashIndexA;
		PatItem.sBitDepth = DLP_BIT_DEPTH_6;
		PatItem.sColorIndex = DLPLEDColor;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;	
		PatItem.sInvertPattern = false;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod1;
		PatItem.nExposure = PatExposure1;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		//second
		PatItem.sFlashIndex = FlashIndexB;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;//000
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod2;
		PatItem.nExposure = PatExposure2;
		this->AddDLPPPatItem(PatItem);
		index ++;
		/*
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;//270	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;//180
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;
		*/
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;//090
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;
	}	
	else if ( DLP_PATTERN_SEQUENCE_4_4_M == Mode )
	{
		//6x4x2=48 
		PatItem.sFlashIndex = FlashIndexA;
		PatItem.sBitDepth = DLP_BIT_DEPTH_6;
		PatItem.sColorIndex = DLPLEDColor;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;	
		PatItem.sInvertPattern = false;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod1;
		PatItem.nExposure = PatExposure1;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		//second
		PatItem.sFlashIndex = FlashIndexB;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;	
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod2;
		PatItem.nExposure = PatExposure2;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;
	}	
	else if ( DLP_PATTERN_SEQUENCE_4_4GC_M == Mode )
	{
		const bool Use5BitPattern=GetUse5BitPattern();
		if ( true == Use5BitPattern )
		{
			//Test 5*4+4=24
			const int nFlashIndex1=GetPatternIndex1_5Bit();			
			if ( nFlashIndex1<0 || nFlashIndex1>=NumImagesInFlash )
			{
				m_ErrorString = _T("Error, DLP Pattern Index Exception");
				return false;
			}
			PatItem.sFlashIndex = nFlashIndex1;
			PatItem.sBitDepth = DLP_BIT_DEPTH_5;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1;
			PatItem.nExposure = PatExposure1;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//second		
			const int FlashIndexGrayCode=nFlashIndex1;
			const int FlashIndexBinaryCode=nFlashIndex1;			
			//const int FlashIndexGrayCode=GetPatternIndexGC_1Bit();		
			//const int FlashIndexBinaryCode=GetPatternIndexBC_1Bit();		
			//const int GrayCodeStartNum=GetPatternStartNumGC_1Bit();//3
			//const int BinaryCodeStartNum=GetPatternStartNumBC_1Bit();//17
			if ( FlashIndexGrayCode<0 || FlashIndexBinaryCode<0 || FlashIndexGrayCode>=NumImagesInFlash || FlashIndexBinaryCode>=NumImagesInFlash )
			{
				m_ErrorString = _T("Error, DLP Pattern Index Exception (GC/BC)");
				return false;
			}
			//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024
			//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
			PatItem.sBitDepth = DLP_BIT_DEPTH_1;
			PatItem.sFlashIndex = FlashIndexGrayCode;//nFlashIndex1
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = false;
			PatItem.nPeriod = PatPeriod2;
			PatItem.nExposure = PatExposure2;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 6;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 12;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			PatItem.sFlashIndex = FlashIndexBinaryCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 18;
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
		}
		else		
		{
			//6x4x2=48 
			PatItem.sFlashIndex = FlashIndexA;
			PatItem.sBitDepth = DLP_BIT_DEPTH_6;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1;
			PatItem.nExposure = PatExposure1;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//second		
			const int FlashIndexGrayCode=GetPatternIndexGC_1Bit();		
			const int FlashIndexBinaryCode=GetPatternIndexBC_1Bit();		
			const int GrayCodeStartNum=GetPatternStartNumGC_1Bit();//3
			const int BinaryCodeStartNum=GetPatternStartNumBC_1Bit();//17
			if ( FlashIndexGrayCode<0 || FlashIndexBinaryCode<0 || FlashIndexGrayCode>=NumImagesInFlash || FlashIndexBinaryCode>=NumImagesInFlash )
			{
				m_ErrorString = _T("Error, DLP Pattern Index Exception (GC/BC)");
				return false;
			}
			//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024
			//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
			PatItem.sBitDepth = DLP_BIT_DEPTH_1;
			PatItem.sFlashIndex = FlashIndexGrayCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
			PatItem.sBitNum = GrayCodeStartNum;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod2;
			PatItem.nExposure = PatExposure2;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = GrayCodeStartNum+1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = GrayCodeStartNum+2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			PatItem.sFlashIndex = FlashIndexBinaryCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = BinaryCodeStartNum-1;
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
		}
	}
	else if ( DLP_PATTERN_SEQUENCE_4_5GC_M == Mode )
	{
		//6x4x2=48 
		PatItem.sFlashIndex = FlashIndexA;
		PatItem.sBitDepth = DLP_BIT_DEPTH_6;
		PatItem.sColorIndex = DLPLEDColor;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;	
		PatItem.sInvertPattern = false;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod1;
		PatItem.nExposure = PatExposure1;
		this->AddDLPPPatItem(PatItem);
		index ++;
		
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		//second		
		const int FlashIndexGrayCode=GetPatternIndexGC_1Bit();		
		const int FlashIndexBinaryCode=GetPatternIndexBC_1Bit();		
		const int GrayCodeStartNum=GetPatternStartNumGC_1Bit();//3
		const int BinaryCodeStartNum=GetPatternStartNumBC_1Bit();//17
		if ( FlashIndexGrayCode<0 || FlashIndexBinaryCode<0 || FlashIndexGrayCode>=NumImagesInFlash || FlashIndexBinaryCode>=NumImagesInFlash )
		{
			m_ErrorString = _T("Error, DLP Pattern Index Exception (GC/BC)");
			return false;
		}
		//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024
		//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
		PatItem.sBitDepth = DLP_BIT_DEPTH_1;
		PatItem.sFlashIndex = FlashIndexGrayCode;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
		PatItem.sBitNum = GrayCodeStartNum;	
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod2;
		PatItem.nExposure = PatExposure2;
		this->AddDLPPPatItem(PatItem);
		index ++;
		
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = GrayCodeStartNum+1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = GrayCodeStartNum+2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = GrayCodeStartNum+3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sFlashIndex = FlashIndexBinaryCode;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = BinaryCodeStartNum;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;
	}
	else if ( DLP_PATTERN_SEQUENCE_4_6GC_M == Mode )
	{
		//6x4x2=48 
		PatItem.sFlashIndex = FlashIndexA;
		PatItem.sBitDepth = DLP_BIT_DEPTH_6;
		PatItem.sColorIndex = DLPLEDColor;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 0;	
		PatItem.sInvertPattern = false;
		PatItem.sInsertBlack = true;
		PatItem.sTrigOutPrev = false;
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod1;
		PatItem.nExposure = PatExposure1;
		this->AddDLPPPatItem(PatItem);
		index ++;
		
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = 3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		//second		
		const int FlashIndexGrayCode=GetPatternIndexGC_1Bit();		
		const int FlashIndexBinaryCode=GetPatternIndexBC_1Bit();		
		const int GrayCodeStartNum=GetPatternStartNumGC_1Bit();//3
		const int BinaryCodeStartNum=GetPatternStartNumBC_1Bit();//17
		if ( FlashIndexGrayCode<0 || FlashIndexBinaryCode<0 || FlashIndexGrayCode>=NumImagesInFlash || FlashIndexBinaryCode>=NumImagesInFlash )
		{
			m_ErrorString = _T("Error, DLP Pattern Index Exception (GC/BC)");
			return false;
		}
		//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024
		//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
		PatItem.sBitDepth = DLP_BIT_DEPTH_1;
		PatItem.sFlashIndex = FlashIndexGrayCode;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
		PatItem.sBitNum = GrayCodeStartNum;	
		PatItem.sBufSwap = true;
		PatItem.nPeriod = PatPeriod2;
		PatItem.nExposure = PatExposure2;
		this->AddDLPPPatItem(PatItem);
		index ++;
		
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = GrayCodeStartNum+1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = GrayCodeStartNum+2;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = GrayCodeStartNum+3;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = GrayCodeStartNum+4;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;

		PatItem.sFlashIndex = FlashIndexBinaryCode;
		PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
		PatItem.sBitNum = BinaryCodeStartNum+1;	
		PatItem.sBufSwap = false;
		this->AddDLPPPatItem(PatItem);
		index ++;
	}
	else if ( DLP_PATTERN_SEQUENCE_4_4GC_M2 == Mode )
	{
		const double Exp2Ratio=GetSecondExpRatio();
		const bool   Use5BitPattern=GetUse5BitPattern();
		int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
		int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
		int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
		int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	
		if ( true == Use5BitPattern )
		{
			//Test 5*4+4=24
			const int nFlashIndex1=GetPatternIndex1_5Bit();			
			if ( nFlashIndex1<0 || nFlashIndex1>=NumImagesInFlash )
			{
				m_ErrorString = _T("Error, DLP Pattern Index Exception");
				return false;
			}
			PatItem.sFlashIndex = nFlashIndex1;
			PatItem.sBitDepth = DLP_BIT_DEPTH_5;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1;
			PatItem.nExposure = PatExposure1;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//second		
			const int FlashIndexGrayCode=nFlashIndex1;
			const int FlashIndexBinaryCode=nFlashIndex1;
			const int GrayCodeStartNum=21;//3
			const int BinaryCodeStartNum=24;//17
			//const int FlashIndexGrayCode=GetPatternIndexGC_1Bit();		
			//const int FlashIndexBinaryCode=GetPatternIndexBC_1Bit();		
			//const int GrayCodeStartNum=GetPatternStartNumGC_1Bit();//3
			//const int BinaryCodeStartNum=GetPatternStartNumBC_1Bit();//17
			if ( FlashIndexGrayCode<0 || FlashIndexBinaryCode<0 || FlashIndexGrayCode>=NumImagesInFlash || FlashIndexBinaryCode>=NumImagesInFlash )
			{
				m_ErrorString = _T("Error, DLP Pattern Index Exception (GC/BC)");
				return false;
			}
			//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024
			//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
			PatItem.sBitDepth = DLP_BIT_DEPTH_1;
			PatItem.sFlashIndex = FlashIndexGrayCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = false;//true
			PatItem.nPeriod = PatPeriod2;
			PatItem.nExposure = PatExposure2;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 6;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 12;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			PatItem.sFlashIndex = FlashIndexBinaryCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 18;
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//增加以下樣板, 會讓取像間隔拉大至20 ms			
			//third
			PatItem.sFlashIndex = nFlashIndex1;
			PatItem.sBitDepth = DLP_BIT_DEPTH_5;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1_2;
			PatItem.nExposure = PatExposure1_2;
			this->AddDLPPPatItem(PatItem);
			index ++;		
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//forth			
			//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024
			//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
			PatItem.sBitDepth = DLP_BIT_DEPTH_1;
			PatItem.sFlashIndex = FlashIndexGrayCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = false;//true
			PatItem.nPeriod = PatPeriod2_2;
			PatItem.nExposure = PatExposure2_2;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 6;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 12;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			PatItem.sFlashIndex = FlashIndexBinaryCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 18;
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
		}
		else
		{
			//6x4x2=48 
			PatItem.sFlashIndex = FlashIndexA;
			PatItem.sBitDepth = DLP_BIT_DEPTH_6;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1;
			PatItem.nExposure = PatExposure1;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//second		
			const int FlashIndexGrayCode=GetPatternIndexGC_1Bit();		
			const int FlashIndexBinaryCode=GetPatternIndexBC_1Bit();		
			const int GrayCodeStartNum=GetPatternStartNumGC_1Bit();//3
			const int BinaryCodeStartNum=GetPatternStartNumBC_1Bit();//17
			if ( FlashIndexGrayCode<0 || FlashIndexBinaryCode<0 || FlashIndexGrayCode>=NumImagesInFlash || FlashIndexBinaryCode>=NumImagesInFlash )
			{
				m_ErrorString = _T("Error, DLP Pattern Index Exception (GC/BC)");
				return false;
			}
			//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024
			//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
			PatItem.sBitDepth = DLP_BIT_DEPTH_1;
			PatItem.sFlashIndex = FlashIndexGrayCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
			PatItem.sBitNum = GrayCodeStartNum;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod2;
			PatItem.nExposure = PatExposure2;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = GrayCodeStartNum+1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = GrayCodeStartNum+2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			PatItem.sFlashIndex = FlashIndexBinaryCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = BinaryCodeStartNum-1;
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//增加以下樣板, 會讓取像間隔拉大至20 ms			
			//third
			PatItem.sFlashIndex = FlashIndexA;
			PatItem.sBitDepth = DLP_BIT_DEPTH_6;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1_2;
			PatItem.nExposure = PatExposure1_2;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//forth
			PatItem.sBitDepth = DLP_BIT_DEPTH_1;
			PatItem.sFlashIndex = FlashIndexGrayCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
			PatItem.sBitNum = GrayCodeStartNum;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod2_2;
			PatItem.nExposure = PatExposure2_2;
			this->AddDLPPPatItem(PatItem);
			index ++;
		
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = GrayCodeStartNum+1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = GrayCodeStartNum+2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			PatItem.sFlashIndex = FlashIndexBinaryCode;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = BinaryCodeStartNum-1;
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
		}
	}
	else if ( DLP_PATTERN_SEQUENCE_4_2_M_2 == Mode )
	{
		const double Exp2Ratio=GetSecondExpRatio();
		const bool   Use3BitPattern=GetUse3BitPattern();
		int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
		int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
		int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
		int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	

		if ( true == Use3BitPattern )
		{
			//Test 3*4*2*2=48
			const int nFlashIndex1=GetPatternIndex1_3Bit();
			const int nFlashIndex2=GetPatternIndex2_3Bit();
			if ( nFlashIndex1<0 || nFlashIndex2<0 || nFlashIndex1>=NumImagesInFlash || nFlashIndex2>=NumImagesInFlash )
			{
				m_ErrorString = _T("Error, DLP Pattern Index Exception");
				return false;
			}
			
			PatItem.sFlashIndex = nFlashIndex1;
			PatItem.sBitDepth = DLP_BIT_DEPTH_3;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1;
			PatItem.nExposure = PatExposure1;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//second
			PatItem.sFlashIndex = nFlashIndex2;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 4;	
			PatItem.sBufSwap = false;
			PatItem.nPeriod = PatPeriod2;
			PatItem.nExposure = PatExposure2;
			this->AddDLPPPatItem(PatItem);
			index ++;
			/*
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 5;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;			

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 6;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
			*/

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 7;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			//增加以下樣板, 會讓取像間隔拉大至20 ms
			//third
			PatItem.sFlashIndex = nFlashIndex1;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1_2;
			PatItem.nExposure = PatExposure1_2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//forth
			PatItem.sFlashIndex = nFlashIndex2;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 4;	
			PatItem.sBufSwap = false;
			PatItem.nPeriod = PatPeriod2_2;
			PatItem.nExposure = PatExposure2_2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			/*
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 5;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 6;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
			*/

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 7;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
		}
		else
		{	//Use 6Bit Image			
			//6x4x2=48 
			PatItem.sFlashIndex = FlashIndexA;
			PatItem.sBitDepth = DLP_BIT_DEPTH_6;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1;
			PatItem.nExposure = PatExposure1;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//second
			PatItem.sFlashIndex = FlashIndexB;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod2;
			PatItem.nExposure = PatExposure2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			/*
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
			*/
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			//增加以下樣板, 會讓取像間隔拉大至20 ms
			//third
			PatItem.sFlashIndex = FlashIndexA;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1_2;
			PatItem.nExposure = PatExposure1_2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//forth
			PatItem.sFlashIndex = FlashIndexB;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod2_2;
			PatItem.nExposure = PatExposure2_2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			/*
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
			*/

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
		}
	}
	else if ( DLP_PATTERN_SEQUENCE_4_4_M_2 == Mode )
	{		
		const double Exp2Ratio=GetSecondExpRatio();
		const bool   Use3BitPattern=GetUse3BitPattern();
		int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
		int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
		int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
		int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	

		if ( true == Use3BitPattern )
		{
			//Test 3*4*2*2=48
			const int nFlashIndex1=GetPatternIndex1_3Bit();
			const int nFlashIndex2=GetPatternIndex2_3Bit();
			if ( nFlashIndex1<0 || nFlashIndex2<0 || nFlashIndex1>=NumImagesInFlash || nFlashIndex2>=NumImagesInFlash )
			{
				m_ErrorString = _T("Error, DLP Pattern Index Exception");
				return false;
			}
			
			PatItem.sFlashIndex = nFlashIndex1;
			PatItem.sBitDepth = DLP_BIT_DEPTH_3;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1;
			PatItem.nExposure = PatExposure1;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//second
			PatItem.sFlashIndex = nFlashIndex2;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 4;	
			PatItem.sBufSwap = false;
			PatItem.nPeriod = PatPeriod2;
			PatItem.nExposure = PatExposure2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 5;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 6;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 7;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			//增加以下樣板, 會讓取像間隔拉大至20 ms
			//third
			PatItem.sFlashIndex = nFlashIndex1;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1_2;
			PatItem.nExposure = PatExposure1_2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//forth
			PatItem.sFlashIndex = nFlashIndex2;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 4;	
			PatItem.sBufSwap = false;
			PatItem.nPeriod = PatPeriod2_2;
			PatItem.nExposure = PatExposure2_2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 5;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 6;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 7;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
		}
		else
		{	//Use 6Bit Image			
			//6x4x2=48 
			PatItem.sFlashIndex = FlashIndexA;
			PatItem.sBitDepth = DLP_BIT_DEPTH_6;
			PatItem.sColorIndex = DLPLEDColor;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sInvertPattern = false;
			PatItem.sInsertBlack = true;
			PatItem.sTrigOutPrev = false;
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1;
			PatItem.nExposure = PatExposure1;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//second
			PatItem.sFlashIndex = FlashIndexB;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod2;
			PatItem.nExposure = PatExposure2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;		

			//增加以下樣板, 會讓取像間隔拉大至20 ms
			//third
			PatItem.sFlashIndex = FlashIndexA;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod1_2;
			PatItem.nExposure = PatExposure1_2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			//forth
			PatItem.sFlashIndex = FlashIndexB;
			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 0;	
			PatItem.sBufSwap = true;
			PatItem.nPeriod = PatPeriod2_2;
			PatItem.nExposure = PatExposure2_2;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 1;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 2;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;

			PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
			PatItem.sBitNum = 3;	
			PatItem.sBufSwap = false;
			this->AddDLPPPatItem(PatItem);
			index ++;
		}
	}	
	m_TrigOutCount = m_PatternList.size();	
	if ( 0 == m_TrigOutCount )
	{
		m_ErrorString.Format(_T("Error, BuildDLPPatternList Fault[Mode=%d]"), Mode);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::AddDLPPPatItem(TDLPPatItem &PatItem)//增加樣板項目
{
	AOIDataDefine.CalcDLPBitPosRange(PatItem.sBitDepth, PatItem.sBitNum, PatItem.sBitStart, PatItem.sBitEnd);
	m_PatternList.push_back(PatItem);	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::RemoveDLPPatItem(size_t index)//移除樣板項目
{
	const size_t count = this->m_PatternList.size();
	if ( index >= count ) { return; }

	SaveDLPProcess(_T("RemoveDLPPatItem"), MSG_LEVEL_HIGH);

	size_t i = 0;
	std::vector<TDLPPatItem>  PatternList = m_PatternList;//偵錯用
	m_PatternList.clear();
	for ( i=0; i<count; i++ )
	{
		if ( i == index ) { continue; }
		m_PatternList.push_back(PatternList[i]);
	}
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::BuildPatternImage(int BitDepth, int NPeriod, int NPixelPeriod, bool bVer, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage)
{
	//G-8bit R-8bit B-8bit
	const char fnName[]="CLight3DTiDLP_DLPC350_v4::BuildPatternImage";
	const int TotalBit = 24;//24bit
	if ( (BitDepth*NPeriod) > TotalBit )
	{
		this->m_ErrorString.Format(_T("Error, TiDLP Pattern Bit Depth and Period out of range(%d)"), TotalBit);
		return false;
	}
	int   val[64]={0};
	int   TotalVal=0;
	int   PaddingShift = 0;
	const size_t MaxPixelPeriod = 128;	
	unsigned char PatternBuffer[24][MaxPixelPeriod]={0};
	const double Phase_DegUnit = 360.0/NPixelPeriod;//每個像素為度
	const size_t PixelMaxBit = 8;
	const int    PixelShift = PixelMaxBit-BitDepth;
	const size_t BitRange = 256;
	const double MaxGraydbl = (double)(BitRange-1);

	if ( (TotalBit%BitDepth) != 0 ) 
	{	PaddingShift = 1; }

	size_t i=0, j=0, k=0, idx=0;
	int    Gray=0;
	int    PatternIdx=0;

	double GrayDbl=0;
	double Phase_Deg=0;
	double Phase_Rad=0;	
	unsigned char r=0, g=0, b=0;

	ImageW = this->GetDLPImageW();
	ImageH = this->GetDLPImageH();
	IMAGE_SIZE BitCount = TotalBit;
	ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t BufferSize = ImageStep*ImageH;
	JetMemory.free_func(pImage);
	if ( JetMemory.alloc_func(BufferSize, pImage, fnName, "pImage") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false;
	}

	if ( 3 == NPeriod)//0, 120, 240,
	{
		for ( i=0; i<NPixelPeriod; i++ )
		{
			//0
			Phase_Deg = (i*Phase_DegUnit);
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[0][i] = static_cast<unsigned char>(Gray);

			//120
			Phase_Deg = (i*Phase_DegUnit)+120.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[1][i] = static_cast<unsigned char>(Gray);

			//240
			Phase_Deg = (i*Phase_DegUnit)+240.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[2][i] = static_cast<unsigned char>(Gray);
		}

		if ( true == bVer )
		{
			for ( i=0; i<ImageH; i++ )
			{
				idx = i*ImageStep;
				for ( j=0; j<ImageW; j++ )
				{
					PatternIdx = j%NPixelPeriod;

					val[0] = PatternBuffer[0][PatternIdx];//000
					val[1] = PatternBuffer[1][PatternIdx];//120
					val[2] = PatternBuffer[2][PatternIdx];//240

					if ( PixelShift > 0 ) //重新取樣, 將8bit取成?bit
					{
						val[0] = val[0] >> PixelShift;
						val[1] = val[1] >> PixelShift;
						val[2] = val[2] >> PixelShift;
					}

					if ( PaddingShift > 0 ) //不整除時會提高單一位元
					{
						val[0] = val[0] << PaddingShift;
						val[1] = val[1] << PaddingShift;
						val[2] = val[2] << PaddingShift;
					}

					//合成一個大數值-Int				
					val[10] = val[0];
					val[11] = val[1] << ((BitDepth+PaddingShift));
					val[12] = val[2] << ((BitDepth+PaddingShift)*2);

					TotalVal = val[10]+val[11]+val[12];

					//轉除成G, R , B
					val[20] = TotalVal&0xff0000;
					val[21] = TotalVal&0x00ff00;
					val[22] = TotalVal&0x0000ff;

					val[30] = val[20]>>16;
					val[31] = val[21]>>8;
					val[32] = val[22];

					b = static_cast<unsigned char>(val[30]);
					r = static_cast<unsigned char>(val[31]);
					g = static_cast<unsigned char>(val[32]);

					pImage[idx] = b; idx++;
					pImage[idx] = g; idx++;
					pImage[idx] = r; idx++;
				}
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				idx = i*ImageStep;
				for ( j=0; j<ImageW; j++ )
				{
					PatternIdx = i%NPixelPeriod;

					val[0] = PatternBuffer[0][PatternIdx];//000
					val[1] = PatternBuffer[1][PatternIdx];//120
					val[2] = PatternBuffer[2][PatternIdx];//240

					if ( PixelShift > 0 ) //重新取樣, 將8bit取成?bit
					{
						val[0] = val[0] >> PixelShift;
						val[1] = val[1] >> PixelShift;
						val[2] = val[2] >> PixelShift;
					}

					if ( PaddingShift > 0 ) //不整除時會提高單一位元
					{
						val[0] = val[0] << PaddingShift;
						val[1] = val[1] << PaddingShift;
						val[2] = val[2] << PaddingShift;
					}

					//合成一個大數值-Int				
					val[10] = val[0];
					val[11] = val[1] << ((BitDepth+PaddingShift));
					val[12] = val[2] << ((BitDepth+PaddingShift)*2);

					TotalVal = val[10]+val[11]+val[12];

					//轉除成G, R , B
					val[20] = TotalVal&0xff0000;
					val[21] = TotalVal&0x00ff00;
					val[22] = TotalVal&0x0000ff;

					val[30] = val[20]>>16;
					val[31] = val[21]>>8;
					val[32] = val[22];

					b = static_cast<unsigned char>(val[30]);
					r = static_cast<unsigned char>(val[31]);
					g = static_cast<unsigned char>(val[32]);

					pImage[idx] = b; idx++;
					pImage[idx] = g; idx++;
					pImage[idx] = r; idx++;
				}
			}
		}
	}
	else if ( 4 == NPeriod )//0, 90, 180, 240
	{
		for ( i=0; i<NPixelPeriod; i++ )
		{
			//000
			Phase_Deg = (i*Phase_DegUnit);
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[0][i] = static_cast<unsigned char>(Gray);

			//090
			Phase_Deg = (i*Phase_DegUnit)+90.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[1][i] = static_cast<unsigned char>(Gray);

			//180
			Phase_Deg = (i*Phase_DegUnit)+180.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[2][i] = static_cast<unsigned char>(Gray);

			//270
			Phase_Deg = (i*Phase_DegUnit)+270.0;
			Phase_Rad = Phase_Deg*DEG_TO_RAD_DBL;
			GrayDbl = ::sin(Phase_Rad);//-1 ~ 1, signed char = -127 ~ 127
			GrayDbl += 1.0;//移至0 ~ 2, unsigned char = 0 ~ 255			
			Gray = (int)(GrayDbl*MaxGraydbl/2.0);
			PatternBuffer[3][i] = static_cast<unsigned char>(Gray);
		}

		if ( true == bVer )
		{
			for ( i=0; i<ImageH; i++ )
			{
				idx = i*ImageStep;
				for ( j=0; j<ImageW; j++ )
				{				
					PatternIdx = j%NPixelPeriod;

					val[0] = PatternBuffer[0][PatternIdx];//000				
					val[1] = PatternBuffer[1][PatternIdx];//090
					val[2] = PatternBuffer[2][PatternIdx];//180				
					val[3] = PatternBuffer[3][PatternIdx];//270

					if ( PixelShift > 0 ) //重新取樣, 將8bit取成?bit
					{
						val[0] = val[0] >> PixelShift;
						val[1] = val[1] >> PixelShift;
						val[2] = val[2] >> PixelShift;
						val[3] = val[3] >> PixelShift;
					}

					if ( PaddingShift > 0 ) //不整除時會提高單一位元
					{
						val[0] = val[0] << PaddingShift;
						val[1] = val[1] << PaddingShift;
						val[2] = val[2] << PaddingShift;
						val[3] = val[3] << PaddingShift;
					}

					//合成一個大數值-Int
					val[10] = val[0];
					val[11] = val[1] << (BitDepth+PaddingShift);
					val[12] = val[2] << ((BitDepth+PaddingShift)*2);
					val[13] = val[3] << ((BitDepth+PaddingShift)*3);

					TotalVal = val[10]+val[11]+val[12]+val[13];

					//轉除成G, R , B
					val[20] = TotalVal&0xff0000;
					val[21] = TotalVal&0x00ff00;
					val[22] = TotalVal&0x0000ff;

					val[30] = val[20]>>16;
					val[31] = val[21]>>8;
					val[32] = val[22];

					b = static_cast<unsigned char>(val[30]);
					r = static_cast<unsigned char>(val[31]);
					g = static_cast<unsigned char>(val[32]);

					pImage[idx] = b; idx++;
					pImage[idx] = g; idx++;
					pImage[idx] = r; idx++;
				}
			}		
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				idx = i*ImageStep;
				for ( j=0; j<ImageW; j++ )
				{				
					PatternIdx = i%NPixelPeriod;

					val[0] = PatternBuffer[0][PatternIdx];//000				
					val[1] = PatternBuffer[1][PatternIdx];//090
					val[2] = PatternBuffer[2][PatternIdx];//180				
					val[3] = PatternBuffer[3][PatternIdx];//270

					if ( PixelShift > 0 ) //重新取樣, 將8bit取成?bit
					{
						val[0] = val[0] >> PixelShift;
						val[1] = val[1] >> PixelShift;
						val[2] = val[2] >> PixelShift;
						val[3] = val[3] >> PixelShift;
					}

					if ( PaddingShift > 0 ) //不整除時會提高單一位元
					{
						val[0] = val[0] << PaddingShift;
						val[1] = val[1] << PaddingShift;
						val[2] = val[2] << PaddingShift;
						val[3] = val[3] << PaddingShift;
					}

					//合成一個大數值-Int
					val[10] = val[0];
					val[11] = val[1] << (BitDepth+PaddingShift);
					val[12] = val[2] << ((BitDepth+PaddingShift)*2);
					val[13] = val[3] << ((BitDepth+PaddingShift)*3);

					TotalVal = val[10]+val[11]+val[12]+val[13];

					//轉除成G, R , B
					val[20] = TotalVal&0xff0000;
					val[21] = TotalVal&0x00ff00;
					val[22] = TotalVal&0x0000ff;

					val[30] = val[20]>>16;
					val[31] = val[21]>>8;
					val[32] = val[22];

					b = static_cast<unsigned char>(val[30]);
					r = static_cast<unsigned char>(val[31]);
					g = static_cast<unsigned char>(val[32]);

					pImage[idx] = b; idx++;
					pImage[idx] = g; idx++;
					pImage[idx] = r; idx++;
				}
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseZeroWhite()//確認平面相位
{
	if ( DLP_LED_COLOR_WHITE != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL[White]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseZeroDebug()//確認平面相位
{
	if ( DLP_LED_COLOR_DEBUG != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseZeroRed()//確認平面相位
{
	if ( DLP_LED_COLOR_RED != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL[Red]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseZeroGrn()//確認平面相位
{
	if ( DLP_LED_COLOR_GREEN != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL[Green]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseZeroBlu()//確認平面相位
{
	if ( DLP_LED_COLOR_BLUE != m_PhaseZeroLEDColor )
	{	return false; }
	if ( NULL == m_PhaseZeroPtr )
	{	
		this->m_ErrorString.Format(_T("Error, Phase Zero is NULL[Blue]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP_DLPC350_v4::GetDLPPhaseModeTextForBinFile(int Mode)//取得相位模式的文字
{
	switch ( Mode )
	{	
	case LIGHT3D_PHASE_2_2_M:
		Mode = LIGHT3D_PHASE_2_2_M;
		break;
	case LIGHT3D_PHASE_4_2_M:
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_4GC_M:
		Mode = LIGHT3D_PHASE_4_4GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_5GC_M:
		Mode = LIGHT3D_PHASE_4_5GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_6GC_M:
		Mode = LIGHT3D_PHASE_4_6GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_2_M_2:
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_4_M_2:
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_4GC_M_2:
		Mode = LIGHT3D_PHASE_4_4GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	}
	return AOIDataDefine.GetDLPPhaseModeText(Mode);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseZero()//載入平面相位
{
	bool  IsOK = true;
	const int LEDColor = GetDLPParam().m_LEDColor;
	LockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		IsOK = LoadPhaseZeroRed();
		break;
	case DLP_LED_COLOR_GREEN:
		IsOK = LoadPhaseZeroGrn();
		break;
	case DLP_LED_COLOR_BLUE:
		IsOK = LoadPhaseZeroBlu();
		break;
	case DLP_LED_COLOR_DEBUG:
		IsOK = LoadPhaseZeroDebug();
		break;
	default:
		IsOK = LoadPhaseZeroWhite();
		break;
	}	
	UnlockLight3D();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroRed()//載入平面相位
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroRed";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Red.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Red-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	}

	ClearPhaseZeroBufferRed();

	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = DLP_LED_COLOR_RED;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroGrn()//載入平面相位
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroGrn";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Green.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Green-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	}

	ClearPhaseZeroBufferGrn();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = DLP_LED_COLOR_GREEN;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroBlu()//載入平面相位
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroBlu";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Blue.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Blue-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	}

	ClearPhaseZeroBufferBlu();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = DLP_LED_COLOR_BLUE;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroWhite()//載入平面相位
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroWhite";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_White.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_White-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	}

	ClearPhaseZeroBufferWhite();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = DLP_LED_COLOR_WHITE;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroDebug()//載入平面相位
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseZeroDebug";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Debug.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Debug-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	}

	ClearPhaseZeroBufferDebug();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = DLP_LED_COLOR_DEBUG;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr)//載入DLP平面相位
{
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadDLPPhaseZeroFile";
	bool    IsOK = true;		
	IMAGE_SIZE   TempW = 0;
	IMAGE_SIZE   TempH = 0;
	IMAGE_SIZE   TempStep = 0;
	PHASE_PTR    TempPhase = NULL;
	size_t  NReads = 0;

	FILE   *pFile = NULL;
	CString Filename = pfilename;

	pFile = ::_tfopen(Filename, _T("rb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return true;
	}
	NReads = ::fread(&TempW, sizeof(TempW), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(&TempH, sizeof(TempH), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	NReads = ::fread(&TempStep, sizeof(TempStep), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = TempStep*TempH;
	if ( 0 == BufferSize ) 
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BuferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	if ( JetMemory.alloc_func(BufferSize, TempPhase, fnName, "m_PhaseZeroPtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(TempPhase, sizeof(PHASE_DATA), BufferSize, pFile);
	if ( BufferSize != NReads )
	{
		JetMemory.free_func(TempPhase);
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseZeroPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);
	pFile = NULL;

	PhaseW = TempW;
	PhaseH = TempH;
	PhaseStep = TempStep;
	PhasePtr = TempPhase;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseZero()//儲存平面相位
{
	bool IsOK = true;
	if ( SavePhaseZeroRed() == false )
	{	IsOK = false;	}
	if ( SavePhaseZeroGrn() == false )
	{	IsOK = false;	}
	if ( SavePhaseZeroBlu() == false )
	{	IsOK = false;	}
	if ( SavePhaseZeroWhite() == false )
	{	IsOK = false;	}
	if ( SavePhaseZeroDebug() == false )
	{	IsOK = false;	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseZeroRed()//儲存平面相位
{
	if ( CheckPhaseZeroRed() == false )
	{	return true; }	

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Red.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Red-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	 }
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseZeroGrn()//儲存平面相位
{
	if ( CheckPhaseZeroGrn() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Green.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Green-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	 }
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseZeroBlu()//儲存平面相位
{
	if ( CheckPhaseZeroBlu() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Blue.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Blue-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	 }
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseZeroWhite()//儲存平面相位
{
	if ( CheckPhaseZeroWhite() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_White.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_White-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	 }
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseZeroDebug()//儲存平面相位
{
	if ( CheckPhaseZeroDebug() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Debug.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Debug-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);	 }
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SaveDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr)//儲存DLP平面相位
{
#ifndef OFFLINE_VERSION
	if ( NULL == pfilename ) { return false; }
	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename = pfilename;

	IMAGE_SIZE PhaseZeroW = PhaseW;
	IMAGE_SIZE PhaseZeroH = PhaseH;
	IMAGE_SIZE PhaseZeroStep = PhaseStep;
	const PHASE_PTR  PhaseZeroPtr = PhasePtr;
	if ( NULL == PhaseZeroPtr ) 
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroPtr"));
		return false;
	}

	pFile = ::_tfopen(Filename, _T("wb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return false;
	}
	NWrites = ::fwrite(&PhaseZeroW, sizeof(PhaseZeroW), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NWrites = ::fwrite(&PhaseZeroH, sizeof(PhaseZeroH), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NWrites = ::fwrite(&PhaseZeroStep, sizeof(PhaseZeroStep), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	const size_t BufferSize = PhaseZeroH*PhaseZeroStep;
	NWrites = ::fwrite(PhaseZeroPtr, sizeof(PHASE_DATA), BufferSize, pFile);
	if ( BufferSize != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseZeroPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);
	pFile = NULL;
#endif//OFFLINE_VERSION
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseZeroBuffer()//清除平面相位
{
	ClearPhaseZeroBufferRed();
	ClearPhaseZeroBufferGrn();
	ClearPhaseZeroBufferBlu();
	ClearPhaseZeroBufferWhite();
	ClearPhaseZeroBufferDebug();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseZeroBufferRed()//清除平面相位	
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseZeroBufferGrn()//清除平面相位	
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseZeroBufferBlu()//清除平面相位	
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseZeroBufferWhite()//清除平面相位	
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseZeroBufferDebug()//清除平面相位
{
	if ( NULL != m_PhaseZeroPtr )
	{	JetMemory.free_func(m_PhaseZeroPtr);	}
	m_PhaseZeroW = 0;//平面相位寬度
	m_PhaseZeroH = 0;//平面相位高度
	m_PhaseZeroStep = 0;//平面相位步長
	m_PhaseZeroLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr)//設定平面相位
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::SetPhaseZero";	
	if ( NULL == Ptr ) { return false; }

	PHASE_PTR PhasePtr=NULL;
	const size_t BufferSize = ImageSetp*ImageH;
	if ( JetMemory.alloc_func(BufferSize, PhasePtr, fnName, "PhasePtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}	
	::memcpy(PhasePtr, Ptr, sizeof(PHASE_DATA)*BufferSize);

	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		ClearPhaseZeroBufferRed();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_GREEN:
		ClearPhaseZeroBufferGrn();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_BLUE:
		ClearPhaseZeroBufferBlu();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_DEBUG:
		ClearPhaseZeroBufferDebug();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;		
	default:
		ClearPhaseZeroBufferWhite();
		m_PhaseZeroW = ImageW;
		m_PhaseZeroH = ImageH;
		m_PhaseZeroStep = ImageSetp;
		m_PhaseZeroPtr = PhasePtr;
		m_PhaseZeroLEDColor = LEDColor;
		break;
	}
	m_PhaseZeroPhaseMode = PhaseMode;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)//取得平面相位
{
	bool IsOK = true;
	bool ReloadFile = false;	 
	LockLight3D();
	if ( PhaseMode!=m_PhaseZeroPhaseMode || m_PhaseZeroLEDColor!=LEDColor )
	{
		m_PhaseZeroPhaseMode = PhaseMode;
		ReloadFile = true;
	}	
	if ( true == ReloadFile )
	{
		switch ( LEDColor )
		{
		case DLP_LED_COLOR_RED:		IsOK=LoadPhaseZeroRed();	break;
		case DLP_LED_COLOR_GREEN:	IsOK=LoadPhaseZeroGrn();	break;
		case DLP_LED_COLOR_BLUE:	IsOK=LoadPhaseZeroBlu();	break;
		case DLP_LED_COLOR_DEBUG:	IsOK=LoadPhaseZeroDebug();	break;
		default:					IsOK=LoadPhaseZeroWhite();	break;
		}
		if ( false == IsOK )
		{
			UnlockLight3D();
			return false; 
		}
	}
	UnlockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	case DLP_LED_COLOR_GREEN:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	case DLP_LED_COLOR_BLUE:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	case DLP_LED_COLOR_DEBUG:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	default:
		ImageW = m_PhaseZeroW;
		ImageH = m_PhaseZeroH;
		ImageSetp = m_PhaseZeroStep;
		Ptr = m_PhaseZeroPtr;
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClonePhaseZero(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)//複製平面相位
{
	IMAGE_SIZE PhaseZeroW=0;
	IMAGE_SIZE PhaseZeroH=0;
	IMAGE_SIZE PhaseZeroStep=0;
	PHASE_PTR  PhaseZeroPtr=NULL;
	const int PhaseMode = m_PhaseZeroPhaseMode;		
	if ( GetPhaseZero(PhaseMode, LEDColor, PhaseZeroW, PhaseZeroH, PhaseZeroStep, PhaseZeroPtr) == false )
	{	return false; }

	JetMemory.free_func(Ptr);
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::ClonePhaseZero";	
	const size_t BufferSize = PhaseZeroStep*PhaseZeroH;
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}

	::memcpy(Ptr, PhaseZeroPtr, sizeof(PHASE_DATA)*BufferSize);
	ImageW = PhaseZeroW;
	ImageH = PhaseZeroH;
	ImageSetp = PhaseZeroStep;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseFactorRed()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_RED!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Red]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseFactorGrn()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_GREEN!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Green]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseFactorBlu()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_BLUE!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Blue]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseFactorWhite()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_WHITE!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[White]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CheckPhaseFactorDebug()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_DEBUG!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Debug]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseFactor()//載入平面係數
{	
	bool  IsOK = true;
	const int LEDColor = GetDLPParam().m_LEDColor;
	LockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		IsOK = LoadPhaseFactorRed();
		break;
	case DLP_LED_COLOR_GREEN:
		IsOK = LoadPhaseFactorGrn();
		break;
	case DLP_LED_COLOR_BLUE:
		IsOK = LoadPhaseFactorBlu();
		break;
	case DLP_LED_COLOR_DEBUG:
		IsOK = LoadPhaseFactorDebug();
		break;
	default:
		IsOK = LoadPhaseFactorWhite();
		break;
	}	
	UnlockLight3D();
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorRed()//載入平面係數
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorRed";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Red.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Red-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	}
	ClearPhaseFactorBufferRed();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = DLP_LED_COLOR_RED;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorGrn()//載入平面係數
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorGrn";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Green.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Green-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	}
	ClearPhaseFactorBufferGrn();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = DLP_LED_COLOR_GREEN;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorBlu()//載入平面係數
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorBlu";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Blue.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Blue-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	}
	ClearPhaseFactorBufferBlu();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = DLP_LED_COLOR_BLUE;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorWhite()//載入平面係數
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorWhite";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_White.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_White-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	}
	ClearPhaseFactorBufferWhite();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = DLP_LED_COLOR_WHITE;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorDebug()//載入平面係數
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadPhaseFactorDebug";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	

	FILE   *pFile = NULL;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Debug.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Debug-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	}
	ClearPhaseFactorBufferDebug();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{	return false; }
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = DLP_LED_COLOR_DEBUG;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr)//載入DLP平面係數
{
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::LoadDLPPhaseFactorFile";
	bool    IsOK = true;		
	IMAGE_SIZE   TempW = 0;
	IMAGE_SIZE   TempH = 0;
	IMAGE_SIZE   TempStep = 0;
	SPACE_PTR    TempSpace = NULL;
	size_t   NReads = 0;	

	FILE   *pFile = NULL;
	CString Filename = pfilename;

	pFile = ::_tfopen(Filename, _T("rb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return true;
	}
	NReads = ::fread(&TempW, sizeof(TempW), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(&TempH, sizeof(TempH), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	NReads = ::fread(&TempStep, sizeof(TempStep), 1, pFile);
	if ( 1 != NReads )
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = TempStep*TempH;
	if ( 0 == BufferSize ) 
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BufferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	if ( JetMemory.alloc_func(BufferSize, TempSpace, fnName, "m_PhaseFactorPtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NReads = ::fread(TempSpace, sizeof(SPACE_DATA), BufferSize, pFile);
	if ( BufferSize != NReads )
	{
		JetMemory.free_func(TempSpace);
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("m_PhaseFactorPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);
	pFile = NULL;
	SpaceW = TempW;
	SpaceH = TempH;
	SpaceStep = TempStep;	
	SpacePtr = TempSpace;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseFactor()//儲存平面係數
{
	bool IsOK = true;
	if ( SavePhaseFactorRed() == false )
	{	IsOK = false;	}
	if ( SavePhaseFactorGrn() == false )
	{	IsOK = false;	}
	if ( SavePhaseFactorBlu() == false )
	{	IsOK = false;	}
	if ( SavePhaseFactorWhite() == false )
	{	IsOK = false;	}
	if ( SavePhaseFactorDebug() == false )
	{	IsOK = false;	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseFactorRed()//儲存平面係數
{
	if ( CheckPhaseFactorRed() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Red.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Red-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName); }
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseFactorGrn()//儲存平面係數
{
	if ( CheckPhaseFactorGrn() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Green.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Green-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName); }
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseFactorBlu()//儲存平面係數
{
	if ( CheckPhaseFactorBlu() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Blue.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Blue-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName); }
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseFactorWhite()//儲存平面係數
{
	if ( CheckPhaseFactorWhite() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_White.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_White-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName); }
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SavePhaseFactorDebug()//儲存平面係數
{
	if ( CheckPhaseFactorDebug() == false )
	{	return true; }

	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename;	
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Debug.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( TB_SYSTEM_BOT == SysID )
	{	Filename.Format(_T("%s\\%s%s_%s_Debug-2.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName); }
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SaveDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr)//儲存DLP平面係數
{
#ifndef OFFLINE_VERSION
	if ( NULL == pfilename ) { return false; }
	bool    IsOK = true;	
	FILE   *pFile = NULL;
	size_t  NWrites = 0;
	CString Filename = pfilename;	

	IMAGE_SIZE PhaseFactorW = SpaceW;
	IMAGE_SIZE PhaseFactorH = SpaceH;
	IMAGE_SIZE PhaseFactorStep = SpaceStep;
	const SPACE_PTR  PhaseFactorPtr = SpacePtr;

	if ( NULL == PhaseFactorPtr )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorPtr"));
		return false;		
	};	

	pFile = ::_tfopen(Filename, _T("wb"));
	if ( NULL == pFile )
	{
		this->m_ErrorString.Format(_T("Error, Open File Fault(%s)"), Filename);
		return false;
	}
	NWrites = ::fwrite(&PhaseFactorW, sizeof(PhaseFactorW), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorW"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NWrites = ::fwrite(&PhaseFactorH, sizeof(PhaseFactorH), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorH"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	NWrites = ::fwrite(&PhaseFactorStep, sizeof(PhaseFactorStep), 1, pFile);
	if ( 1 != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorStep"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}

	const size_t BufferSize = PhaseFactorH*PhaseFactorStep;
	NWrites = ::fwrite(PhaseFactorPtr, sizeof(SPACE_DATA), BufferSize, pFile);
	if ( BufferSize != NWrites )
	{
		this->m_ErrorString.Format(_T("Error, Write File Fault(%s::%s)"), Filename, _T("m_PhaseFactorPtr"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}		
	::fclose(pFile);
	pFile = NULL;
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseFactorBuffer()//清除平面係數
{
	ClearPhaseFactorBufferRed();
	ClearPhaseFactorBufferGrn();
	ClearPhaseFactorBufferBlu();
	ClearPhaseFactorBufferWhite();
	ClearPhaseFactorBufferDebug();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseFactorBufferRed()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseFactorBufferGrn()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseFactorBufferBlu()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseFactorBufferWhite()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClearPhaseFactorBufferDebug()//清除平面係數
{
	if ( NULL != m_PhaseFactorPtr )
	{	JetMemory.free_func(m_PhaseFactorPtr);	}
	m_PhaseFactorW = 0;//平面係數寬度
	m_PhaseFactorH = 0;//平面係數高度
	m_PhaseFactorStep = 0;//平面係數步長
	m_PhaseFactorLEDColor = DLP_LED_COLOR_NO;
	return true;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP_DLPC350_v4::GetPhaseFactorMin() const//取得平面係數下限
{
	return m_DLPParam.m_PhaseFactorMin;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPhaseFactorMin(double val)//設定平面係數下限
{
	m_DLPParam.m_PhaseFactorMin = val;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP_DLPC350_v4::GetPhaseFactorMax() const//取得平面係數上限
{
	return m_DLPParam.m_PhaseFactorMax;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::SetPhaseFactorMax(double val)//設定平面係數上限
{
	m_DLPParam.m_PhaseFactorMax = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_DLPC350_v4::GetLEDCurrentMax() const//取得LED電流上限
{
	return m_DLPParam.m_LEDCurrentMax;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::ClearHeightFactor()//清除高度參數T0, T1, T3
{
	::memset(m_HeightFactor0, 0x00, sizeof(m_HeightFactor0));
	::memset(m_HeightFactor1, 0x00, sizeof(m_HeightFactor1));
	::memset(m_HeightFactor2, 0x00, sizeof(m_HeightFactor2));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SortHeightFactorTableList()//排序高度係數列表
{
	std::vector<TPhaseFactorTable> TempFactorTableList=m_FactorTableList;	
	const size_t TempTableCount = TempFactorTableList.size();
	if ( 0==TempTableCount || 1==TempTableCount ) { return true; }	

	size_t   i=0;
	int      TargetNo=0;
	double   TargetHeight=0;
	size_t   TableIndex=0;	
	CSortObj SortObj;
	std::vector<CSortObj> SortList;
	SortObj.SetSortMode(SORT_BY_INT);//For No
	SortObj.SetSortMode(SORT_BY_DBL);//For Height
	for ( i=0; i<TempTableCount; i++ )
	{		
		const TPhaseFactorTable &FactorTable=TempFactorTableList[i];		
		TargetNo = FactorTable.nTargetNo;
		TargetHeight = FactorTable.dHeight;
		SortObj.SetID(i);
		SortObj.SetValueInt(TargetNo);
		SortObj.SetValueDbl(TargetHeight);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount=SortList.size();
	m_FactorTableList.clear();
	for ( i=0; i<SortCount; i++ )
	{		
		SortObj = SortList[i];		
		TableIndex = SortObj.GetID();
		m_FactorTableList.push_back(TempFactorTableList[TableIndex]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SaveHeightFactorTableList()//儲存高度係數列表檔案
{
#ifndef OFFLINE_VERSION
	if ( SortHeightFactorTableList() == false ) { return false; }

	const std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	
	const size_t TableCount = FactorTableList.size();
	if ( 0 == TableCount ) { return true; }

	size_t   i=0;
	CString  folder;
	CString  filename;
	int      FileNo=0;
	const bool bTemp = false;
	LIGHT_3D_CAST_ID CastID=m_Light3DCastID;
	const TB_SYSTEM_ID SysID = GetDLPSystemID();
	const int MaxTargetNo = MAX_HEIGHT_TARGET_COUNT;

	folder = AOIDataCollect.GetPhaseFactorFolder(bTemp);
	::CreateDirectory(folder, NULL);
	for ( i=0; i<MaxTargetNo; i++ )
	{
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp, SysID);
		::DeleteFile(filename);
	}
	for ( i=0; i<TableCount; i++ )
	{		
		const TPhaseFactorTable &GridTable = FactorTableList[i];		
		const size_t GridCount = GridTable.GridList.size();
		if ( 0 == GridCount ) { continue; }
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp, SysID);
		AOIDataCollect.SavePhaseFactorTableFile(filename, GridTable);
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::LoadHeightFactorTableList()//載入高度係數列表檔案
{
	int   i=0;
	CString  filename;	
	int      FileNo=0;
	const bool bTemp = false;	
	const int MaxFileNo = MAX_HEIGHT_TARGET_COUNT;
	LIGHT_3D_CAST_ID CastID=m_Light3DCastID;
	const TB_SYSTEM_ID SysID = GetDLPSystemID();

	m_FactorTableList.clear();
	for ( i=0; i<MaxFileNo; i++ )
	{
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp, SysID);
		if ( JetAPI::IsFileExist(filename) == false ) { continue; }

		TPhaseFactorTable GridTable;
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable);
		if ( GridTable.GridList.size() == 0 ) { continue; }
		if ( 0 == GridTable.nTargetNo ) { continue; }
		m_FactorTableList.push_back(GridTable);
	}	
	if ( SortHeightFactorTableList() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::RestoreHeightFactorTableList()//復原高度係數列表
{
	size_t    i=0, j=0;
	std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	
	size_t TableCount = FactorTableList.size();
	for ( i=0; i<TableCount; i++ )
	{		
		TPhaseFactorTable &Table=FactorTableList[i];
		const size_t GridCount = Table.GridList.size();
		for ( j=0; j<GridCount; j++ )
		{
			TPhaseFactorGrid &Grid=Table.GridList[j];
			Grid.m_PhaseBaseCalc = Grid.m_PhaseBase;
			Grid.m_PhaseTargetCalc = Grid.m_PhaseTarget;
			Grid.m_HeightBaseCalc = Grid.m_HeightBase;
			Grid.m_HeightTargetCalc = Grid.m_HeightTarget;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::ClearHeightFactorTableList(bool bIncludeFiles)//清除高度係數列表
{	
	LIGHT_3D_CAST_ID CastID=m_Light3DCastID;
	m_FactorTableList.clear();		
#ifndef OFFLINE_VERSION
	if ( true == bIncludeFiles )
	{
		int      i=0;
		int      TargetNo=0;
		CString  filename;	
		const bool bTemp = false;
		const TB_SYSTEM_ID SysID = GetDLPSystemID();
		const int MaxTargetNo = MAX_HEIGHT_TARGET_COUNT;
		for ( i=0; i<MaxTargetNo; i++ )
		{
			TargetNo = i+1;
			filename = AOIDataCollect.GetPhaseFactorFilename(CastID, TargetNo, bTemp, SysID);
			::DeleteFile(filename);
		}
	}
#endif//OFFLINE_VERSION
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::BuildHeightFactorMappingParam(bool bRecv)//建立高度參數T0, T1, T3
{
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		CString filename;
		TPhaseFactorGrid Grid;		
		LIGHT_3D_CAST_ID CastID=m_Light3DCastID;
		if ( LIGHT_3D_CAST_04 == CastID )
		{	CastID = CastID; }
		std::vector<double> T;		
		std::vector<TPhaseFactorGrid> GridList_0000;
		std::vector<std::vector<double>> ParamListArray_Z;
		std::vector<std::vector<TPhaseFactorGrid>> GridListArray;//校正用
		std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	

		double TargetH=0;
		int CountX=0, CountY=0;
		const bool bTemp = false;
		double AveErr=0, MaxErr=0, RatioErr=0, MaxRatioErr=0, MaxErrHeight=0;
		const size_t TableListCount = FactorTableList.size();
		if ( 0 == TableListCount ) { return true; }

		GridList_0000 = FactorTableList[0].GridList;
		for ( size_t i=0; i<GridList_0000.size(); i++ )
		{
			GridList_0000[i].m_Phase=GridList_0000[i].m_PhaseBase=GridList_0000[i].m_PhaseTarget=GridList_0000[i].m_PhaseOffset=GridList_0000[i].m_PhaseBaseCalc=GridList_0000[i].m_PhaseTargetCalc=0;
			GridList_0000[i].m_Height=GridList_0000[i].m_HeightBase=GridList_0000[i].m_HeightTarget=GridList_0000[i].m_HeightOffset=GridList_0000[i].m_HeightBaseCalc=GridList_0000[i].m_HeightTargetCalc=0;			 
		}		
		//Reset Height Factor
		ClearHeightFactor();

		//校正用
		if ( GridList_0000.size() > 0 )	{	GridListArray.push_back(GridList_0000); }//0um		
		for ( int i=0; i<TableListCount; i++ )
		{	GridListArray.push_back(FactorTableList[i].GridList);	}		
		const size_t GridListCount = GridListArray.size();
		if ( 0 == GridListCount )
		{
			CString ProjectName = GetDLPProjectName();
			m_ErrorString.Format(_T("Error, Build Height Factor Grid List [%s]"), ProjectName);
			return false;
		}
		//初始化參數
		for ( int i=0; i<GridListCount; i++ )
		{
			for ( int j=0; j<GridListArray[i].size(); j++ )
			{
				GridListArray[i][j].m_Phase  = GridListArray[i][j].m_PhaseTargetCalc;
				GridListArray[i][j].m_Height = GridListArray[i][j].m_HeightTargetCalc;				
			}
		}

		int  nRecuCnt = 0;
		double MapError=0.0;
		double SumError=0.0;
		double LastError=DBL_MAX-10.0;
		bool bFinish = false;
		bool bFinishAll = false;
		const int nMaxRecuCnt = 32;
		while ( true )
		{
			if ( CalcHeightFactorMappingParam(GridListArray, ParamListArray_Z) == false )
			{	return false; }
			if ( false == bRecv )
			{	break; }
			nRecuCnt ++;
			if ( nRecuCnt >= nMaxRecuCnt )
			{	break; }

			//重新驗證高度
			SumError = 0.0;
			bFinish = true;
			bFinishAll = true;
			std::vector<std::vector<TPhaseFactorGrid>> TempGridListArray=GridListArray;//驗證用
			const size_t TempGridListCount=TempGridListArray.size();
			for ( int i=1; i<TempGridListCount; i++ )//剃除第1個, 階高塊為0
			{
				VerifyHeightFactorMappingParam(ParamListArray_Z, TempGridListArray[i], MapError);
				SumError += MapError;
			}			
			if ( GridListCount>1 )
			{	SumError /= (GridListCount-1);	}
			if ( SumError > (LastError-0.1) )
			{	break; }			
			LastError = SumError;
			GridListArray = TempGridListArray;
		};

		if ( true == bRecv )
		{
			if ( GridListCount > TableListCount )
			{
				for ( int i=0; i<TableListCount; i++ )
				{	FactorTableList[i].GridList = GridListArray[i+1];	}		
			}
			if ( GridListCount == TableListCount )
			{
				for ( int i=0; i<TableListCount; i++ )
				{	FactorTableList[i].GridList = GridListArray[i];	}		
			}
		}
		const size_t KListCnt_Z = ParamListArray_Z.size();

		if ( KListCnt_Z > 0 )
		{
			const std::vector<double> &Param=ParamListArray_Z[0];			
			const size_t ParamCnt = Param.size();
			const size_t FactorCnt = sizeof(m_HeightFactor0)/sizeof(m_HeightFactor0[0]);
			for ( int i=0; i<ParamCnt; i++ )
			{
				if ( i>= FactorCnt) { break; }
				m_HeightFactor0[i] = Param[i];
			}			
		}
		if ( KListCnt_Z > 1 )
		{
			const std::vector<double> &Param=ParamListArray_Z[1];
			const size_t ParamCnt = Param.size();
			const size_t FactorCnt = sizeof(m_HeightFactor1)/sizeof(m_HeightFactor0[1]);
			for ( int i=0; i<ParamCnt; i++ )
			{
				if ( i>= FactorCnt) { break; }
				m_HeightFactor1[i] = Param[i];
			}			
		}
		if ( KListCnt_Z > 2 )
		{
			const std::vector<double> &Param=ParamListArray_Z[2];
			const size_t ParamCnt = Param.size();
			const size_t FactorCnt = sizeof(m_HeightFactor2)/sizeof(m_HeightFactor2[1]);
			for ( int i=0; i<ParamCnt; i++ )
			{
				if ( i>= FactorCnt) { break; }
				m_HeightFactor2[i] = Param[i];
			}			
		}
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		m_ErrorString = msg_e;
		return false;
	}
	return true;
#else
	m_ErrorString = _T("Error, Cast Build Height Factor Need OpenCV Enabled");
	return false;
#endif//OPENCV_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CalcHeightFactorMappingParam(std::vector<std::vector<TPhaseFactorGrid>> &GridListArray, std::vector<std::vector<double>> &ParamListArray)//計算高度參數T0, T1, T3
{
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		CString filename;		
		LIGHT_3D_CAST_ID CastID=m_Light3DCastID;
		std::vector<double> T;
		const size_t GridListCount = GridListArray.size();
		if ( 0 == GridListCount )
		{
			CString ProjectName = GetDLPProjectName();
			m_ErrorString.Format(_T("Error, Load Height Factor Grid File Fault [%s]"), ProjectName);
			return false;
		}

		//測試同XY下不同相位的高度值
		std::vector<std::vector<double>> KListArray_Z;
		const size_t PArrayCount=GridListArray[0].size();
		std::vector<std::vector<TPhaseFactorGrid>> GridListPArray(PArrayCount);//For MultiLayer Phase
		for ( int i=0; i<GridListCount; i++ )
		{			
			int idx = 0;
			const int PhaseMappingMode=7;//建立相位Mapping Func的參數
			std::vector<double> KList_Phase;	
			const std::vector<TPhaseFactorGrid> &GridList = GridListArray[i];			
			AOIDataCollect.CalcPhaseFactorXYPhaseMappingFunc(PhaseMappingMode, GridList, KList_Phase);		
			const int KCount_Phase=KList_Phase.size();			
			const size_t ListCnt=GridList.size();

			T.resize(KCount_Phase);
			for ( int j=0; j<ListCnt; j++ )
			{				
				idx = j;
				if ( idx >= PArrayCount ) { continue; }
				TPhaseFactorGrid Grid=GridList[idx];
				//使用第1組的XY影像座標
				if ( GridListPArray[idx].size() > 0 ) 
				{
					Grid.m_ImgX = GridListPArray[idx][0].m_ImgX;
					Grid.m_ImgY = GridListPArray[idx][0].m_ImgY;
				}			
				AOIDataCollect.GetHeightFactorXYParam(KCount_Phase, Grid, T);
				Grid.m_Phase = AOIDataCollect.Calc2ParamVecotr(KList_Phase, T);				
				if ( fabs(Grid.m_Phase) > 0.1 )
				{	Grid.m_Factor = Grid.m_Height/Grid.m_Phase; }
				else
				{	Grid.m_Factor = 0.0; }
				GridListPArray[idx].push_back(Grid);
			}
		}

		const size_t GridListPhaseCount = GridListPArray.size();
		if ( 0 == GridListPhaseCount )
		{
			CString ProjectName = GetDLPProjectName();
			m_ErrorString.Format(_T("Error, Build GridListPArray Fault [%s]"), ProjectName);
			return false;
		}

		std::vector<std::vector<double>> ParamListArray_Z;							
		for ( int i=0; i<GridListPhaseCount; i++ )
		{
			if ( GridListPArray[i].size() == 0 ) { continue; }

			int ZMappingMode = 3;//建立高度Mapping Func的參數
			std::vector<double> KList_Z;
			const int ZCount=(int)(GridListPArray[i].size());
			switch ( ZCount )
			{
			case 1:	ZMappingMode = 1;	break;
			case 2:	ZMappingMode = 2;	break;			
			default:
				ZMappingMode = 3;
				break;
			}
			AOIDataCollect.CalcPhaseFactorZMappingFunc(ZMappingMode, GridListPArray[i], KList_Z);		
			KListArray_Z.push_back(KList_Z);
		}

		const size_t KListArrayCount = KListArray_Z.size();
		if ( 0 == KListArrayCount )
		{
			CString ProjectName = GetDLPProjectName();
			m_ErrorString.Format(_T("Error, Build KListArray_Z Fault [%s]"), ProjectName);
			return false;
		}			

		//建立T參數Mapping Func的參數
		const int TMappingMode = 7;//3-1階, 4,5,6-2階, 7,10-3階					
		const size_t KListCnt_Z = KListArray_Z[0].size();
		for ( int i=0; i<KListCnt_Z; i++ )
		{	
			std::vector<TPhaseFactorGrid> ParamList;
			for ( int j=0; j<PArrayCount; j++ )
			{
				int idx = j;						
				TPhaseFactorGrid PhaseFactorGrid=GridListPArray[idx][0];						
				PhaseFactorGrid.m_Height = KListArray_Z[idx][i];
				PhaseFactorGrid.m_Phase = KListArray_Z[idx][i];		
				ParamList.push_back(PhaseFactorGrid);
			}			
			std::vector<double> KList_Param;				
			AOIDataCollect.CalcPhaseFactorXYPhaseMappingFunc(TMappingMode, ParamList, KList_Param);	
			ParamListArray_Z.push_back(KList_Param);
		}
		ParamListArray = ParamListArray_Z;		
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		m_ErrorString = msg_e;
		return false;
	}
	return true;
#else
	m_ErrorString = _T("Error, Cast Build Height Factor Need OpenCV Enabled");
	return false;
#endif//OPENCV_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::VerifyHeightFactorMappingParam(const std::vector<std::vector<double>> &ParamListArray, std::vector<TPhaseFactorGrid> &GridList, double &Error)//驗證高度參數T0, T1, T3
{		
	const int GridCount = (int)(GridList.size());
	const int ParamListCount = (int)(ParamListArray.size());
	if ( 0==GridCount || 0==ParamListCount )
	{	return true; }

	TPhaseFactorGrid Grid;		
	double AveError=0.0;	
	double MaxErrorBase=0.0;	
	double HeightBaseAfter=0;
	double HeightBaseBefore=0;	
	double HeightTargetAfter=0;
	for ( int i=0; i<GridCount; i++ )
	{
		Grid = GridList[i];		
		std::vector<double> HeightFactor;
		double PhaseBase   = Grid.m_PhaseBaseCalc;
		double PhaseTarget = Grid.m_PhaseTargetCalc;		
		double HeightOffset= Grid.m_HeightOffset;		
		for ( int j=0; j<ParamListCount; j++ )
		{	
			const std::vector<double> &ParamList = ParamListArray[j];
			const int ParamCount = ParamList.size();
			std::vector<double> T(ParamCount);	
			AOIDataCollect.GetHeightFactorXYParam(ParamCount, Grid, T);
			double FactorParam=AOIDataCollect.Calc2ParamVecotr(ParamList, T);
			HeightFactor.push_back(FactorParam);
		}
		const int HeightFactorCount=(int)(HeightFactor.size());
		std::vector<double> T_Base(HeightFactorCount);
		std::vector<double> T_Target(HeightFactorCount);		
		AOIDataCollect.GetHeightFactorPhaseParam(HEIGHT_FACTOR_PHASE_BASE, HeightFactorCount, Grid, T_Base);
		AOIDataCollect.GetHeightFactorPhaseParam(HEIGHT_FACTOR_PHASE_TARGET, HeightFactorCount, Grid, T_Target);
		double HeightBase   = AOIDataCollect.Calc2ParamVecotr(HeightFactor, T_Base);
		double HeightTarget = AOIDataCollect.Calc2ParamVecotr(HeightFactor, T_Target);
		double ErrorBase = fabs(HeightBase-Grid.m_HeightBase);
		double ErrorTarget = fabs(HeightTarget-Grid.m_HeightTarget);		
		if ( ErrorBase > MaxErrorBase ){ MaxErrorBase = ErrorBase; }
		HeightBaseAfter += HeightBase;		
		HeightTargetAfter += HeightTarget;
		AveError += fabs(HeightTarget-HeightBase-HeightOffset);	
	}
	AveError /= GridCount;	
	HeightBaseAfter /= GridCount;//求得新的平均基準面
	HeightTargetAfter /= GridCount;//求得新的平均階高塊

	HeightBaseBefore=0;//求得之前平均基準面
	for ( int i=0; i<GridCount; i++ )
	{	HeightBaseBefore += GridList[i].m_HeightBaseCalc; }
	HeightBaseBefore /= GridCount;

	double HeightBase = HeightBaseAfter;
	double ErrorBase = fabs(HeightBase-HeightBaseBefore);
	double ErrorTarget = fabs(HeightTargetAfter-HeightBaseAfter-Grid.m_HeightOffset);
	//if ( ErrorBase > 5 )
	//if ( ErrorTarget > 1 )
	//if ( AveError > 10 )
	//{	bFinish = false; }
	Error = AveError;

	//修正原始的基準面與階高塊的高度值
	for ( int i=0; i<GridCount; i++ )
	{	
		GridList[i].m_HeightBaseCalc = HeightBase; 
		GridList[i].m_HeightTargetCalc = HeightBase+GridList[i].m_HeightOffset; 
		GridList[i].m_Height = GridList[i].m_HeightTargetCalc;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetHeightFactorMappingParam(double T0[], double T1[], double T2[])//取得高度參數
{
	::memcpy(T0, m_HeightFactor0, sizeof(m_HeightFactor0));
	::memcpy(T1, m_HeightFactor1, sizeof(m_HeightFactor1));
	::memcpy(T2, m_HeightFactor2, sizeof(m_HeightFactor2));		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr)//設定平面係數
{
	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::SetPhaseFactor";	
	if ( NULL == Ptr ) { return false; }

	SPACE_PTR    PhaseFactorPtr = NULL;
	const size_t BufferSize = ImageSetp*ImageH;
	if ( JetMemory.alloc_func(BufferSize, PhaseFactorPtr, fnName, "m_PhaseFactorPtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}
	::memcpy(PhaseFactorPtr, Ptr, sizeof(SPACE_DATA)*BufferSize);

	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		ClearPhaseFactorBufferRed();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_GREEN:
		ClearPhaseFactorBufferGrn();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_BLUE:
		ClearPhaseFactorBufferBlu();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	case DLP_LED_COLOR_DEBUG:
		ClearPhaseFactorBufferDebug();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	default:
		ClearPhaseFactorBufferWhite();		
		m_PhaseFactorW = ImageW;
		m_PhaseFactorH = ImageH;
		m_PhaseFactorStep = ImageSetp;
		m_PhaseFactorPtr = PhaseFactorPtr;
		m_PhaseFactorLEDColor = LEDColor;
		break;
	}
	m_PhaseFactorPhaseMode = PhaseMode;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)//取得平面係數
{
	bool IsOK = true;
	bool ReloadFile = false;
	LockLight3D();
	if ( PhaseMode!=m_PhaseFactorPhaseMode || m_PhaseFactorLEDColor!=LEDColor )
	{
		m_PhaseFactorPhaseMode = PhaseMode;
		ReloadFile = true;
	}
	if ( true == ReloadFile )
	{
		switch ( LEDColor )
		{
		case DLP_LED_COLOR_RED:		IsOK=LoadPhaseFactorRed();	break;
		case DLP_LED_COLOR_GREEN:	IsOK=LoadPhaseFactorGrn();	break;
		case DLP_LED_COLOR_BLUE:	IsOK=LoadPhaseFactorBlu();	break;
		case DLP_LED_COLOR_DEBUG:	IsOK=LoadPhaseFactorDebug();	break;
		default:					IsOK=LoadPhaseFactorWhite();	break;
		}
		if ( false == IsOK )
		{
			UnlockLight3D();
			return false; 
		}
	}
	UnlockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	case DLP_LED_COLOR_GREEN:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	case DLP_LED_COLOR_BLUE:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	case DLP_LED_COLOR_DEBUG:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	default:
		ImageW = m_PhaseFactorW;//平面係數寬度
		ImageH = m_PhaseFactorH;//平面係數高度
		ImageSetp = m_PhaseFactorStep;//平面係數步長
		Ptr = m_PhaseFactorPtr;//平面係數指標
		break;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ClonePhaseFactor(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)//複製平面係數
{
	IMAGE_SIZE PhaseFactorW=0;
	IMAGE_SIZE PhaseFactorH=0;
	IMAGE_SIZE PhaseFactorStep=0;
	SPACE_PTR  PhaseFactorPtr=NULL;
	const int PhaseMode = m_PhaseFactorPhaseMode;			
	if ( GetPhaseFactor(PhaseMode, LEDColor, PhaseFactorW, PhaseFactorH, PhaseFactorStep, PhaseFactorPtr) == false )
	{	return false; }

	const char fnName[] = "CLight3DTiDLP_DLPC350_v4::ClonePhaseFactor";
	JetMemory.free_func(Ptr);
	const size_t BufferSize = PhaseFactorStep*PhaseFactorH;
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "Ptr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false; 
	}

	::memcpy(Ptr, PhaseFactorPtr, sizeof(SPACE_DATA)*BufferSize);
	ImageW = PhaseFactorW;
	ImageH = PhaseFactorH;
	ImageSetp = PhaseFactorStep;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetHeightFactorTable(int TargetNo, const TPhaseFactorTable &GridTable)//設定高度係數格點列表	
{
	if ( 0 == TargetNo ) { return true; }
	size_t i=0;
	std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	
	const size_t TableCount = FactorTableList.size();
	for ( i=0; i<TableCount; i++ )
	{
		const TPhaseFactorTable &FactorTable=FactorTableList[i];		
		if ( TargetNo != FactorTable.nTargetNo ) { continue; }									
		FactorTableList[i] = GridTable; 		
		return true;
	}	
	FactorTableList.push_back(GridTable);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::CloneHeightFactorTable(int TargetNo, TPhaseFactorTable &GridTable) const//複製高度係數格點列表
{
	if ( 0 == TargetNo ) { return false; }
	size_t i=0;
	const std::vector<TPhaseFactorTable> &FactorTableList=m_FactorTableList;	
	const size_t TableCount = FactorTableList.size();
	for ( i=0; i<TableCount; i++ )
	{
		const TPhaseFactorTable &FactorTable=FactorTableList[i];		
		if ( TargetNo != FactorTable.nTargetNo ) { continue; }
		GridTable = FactorTableList[i];
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::ResetLEDDisable(bool &ResetFinish, bool ShowMsg)
{
	if( false == m_EnableTemperatureMonitor ) { ResetFinish=true; return true; }	//不使用溫度監控

	ResetFinish = false;
	bool IsDisable = false;
	if ( GetGPIOLEDDisable(IsDisable, ShowMsg) == false ) { return false; }
	if ( IsDisable == false ) { return true; }

	CString str = "";
	bool IsOverT = false;
	if ( GetGPIOTemperatureOver(IsOverT, ShowMsg) == false ) { return false; }
	if ( IsOverT == true ) 
	{//溫度還沒降下來，不能Reset		
		Sleep(2000);	//等2秒再偵測一次
		if( GetGPIOTemperatureOver(IsOverT, ShowMsg) == false ) { return false; }
		if( true == IsOverT )
		{
			if( true == ShowMsg ) 
			{ 
				m_ErrorString.Format(_T("The DLP high temperature, can not reset led. (%s)"), m_CastName);				
				JetAPI::ShowMessageBox(str);				
			}
			return true;
		}
	}
	m_ErrorString.Format(_T("DLP LED Reset. (%s)"), m_CastName);
	AOIDataCollect.SaveCurrentProcess(m_ErrorString);	

	if( SetGPIOLEDEnable() == false ) { return false; }
	ResetFinish = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetGPIOStatus(UINT PinNum, bool &Status)//取得GPIO pin 狀態
{
	//取得GPIO pin 狀態
	bool EnAltFunc = false;
	bool AltFunc1 = false;
	bool DirOutput = false;	//0=input; 1=output
	bool OutTypeOpenDrain = false;
	Status = false;	//0=LOW; 1=HIGH
	if( DLPC350_SetGPIOConfig(PinNum, EnAltFunc, AltFunc1, DirOutput, OutTypeOpenDrain, Status) <0 )
	{	return false;	}

	DLPSettingDelay();

	if( DLPC350_GetGPIOConfig(PinNum, &EnAltFunc, &AltFunc1, &DirOutput, &OutTypeOpenDrain, &Status) <0 )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetGPIOStatus(UINT PinNum, bool Status)//取得GPIO pin output 狀態
{
	//取得GPIO pin output 狀態
	bool EnAltFunc = false;
	bool AltFunc1 = false;
	bool DirOutput = true;	//0=input; 1=output
	bool OutTypeOpenDrain = false;
	if( DLPC350_SetGPIOConfig(PinNum, EnAltFunc, AltFunc1, DirOutput, OutTypeOpenDrain, Status) <0 )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetGPIOTemperatureOver(bool &IsOver, bool ShowMsg)//偵測溫度狀態。		GPIO11 input狀態。high高溫/low低溫
{
	//偵測溫度狀態。	GPIO11 input狀態。high高溫/low低溫			
	UINT pinNum = 11;
	bool Status = false;	//0=LOW; 1=HIGH
	if ( GetGPIOStatus(pinNum, Status) == false )
	{		
		m_ErrorString.Format(_T("Error, Get temperature fault.(GPIO 11) (%s)"), m_CastName);
		AOIDataCollect.SaveCurrentProcess(m_ErrorString);
		if ( true == ShowMsg )
		{	JetAPI::ShowMessageBox(m_ErrorString); }
		return false;
	}
	if( Status == 0 )
	{	IsOver = false; }
	else
	{ 
		IsOver = true;
		if( true == m_EnableTemperatureMonitor )
		{			
			m_ErrorString.Format(_T("Error, DLP high temperature. (%s)"), m_CastName);
			AOIDataCollect.SaveCurrentProcess(m_ErrorString);
			if ( true == ShowMsg )
			{	JetAPI::ShowMessageBox(m_ErrorString); }
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::GetGPIOLEDDisable(bool &IsDisable, bool ShowMsg)//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除
{	//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除			
	UINT pinNum = 5;
	bool Status = false;	//0=LOW; 1=HIGH
	if ( GetGPIOStatus(pinNum, Status) == false )
	{
		m_ErrorString.Format(_T("Error, Get LED status fault.(GPIO 5) (%s)"), m_CastName);
		AOIDataCollect.SaveCurrentProcess(m_ErrorString);
		JetAPI::ShowMessageBox(m_ErrorString);
		return false;
	}
	if( false == Status )
	{ IsDisable = false; }
	else
	{ 
		IsDisable = true; 		
		if( true == m_EnableTemperatureMonitor )
		{	
			m_ErrorString.Format(_T("Error, DLP disable. (%s)"), m_CastName);
			AOIDataCollect.SaveCurrentProcess(m_ErrorString);
			if ( true == ShowMsg )
			{	JetAPI::ShowMessageBox(m_ErrorString); }
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_DLPC350_v4::SetGPIOLEDEnable()//LED disable狀態解除。	GPIO6  output狀態。 low->hi	
{
	//LED disable狀態解除。	GPIO6  output狀態。 low->hi			
	UINT pinNum = 6;
	if( SetGPIOStatus(pinNum, false) == false ) 
	{ 
		this->m_ErrorString.Format(_T("Error, Set GPIO low fault.(GPIO 6) (%s)"), m_CastName);
		AOIDataCollect.SaveCurrentProcess(m_ErrorString);
		JetAPI::ShowMessageBox(m_ErrorString);
		return false; 
	}
	DLPSettingDelay();
	if( SetGPIOStatus(pinNum, true) == false ) 
	{ 
		this->m_ErrorString.Format(_T("Error, Set GPIO high fault.(GPIO 6) (%s)"), m_CastName);
		AOIDataCollect.SaveCurrentProcess(m_ErrorString);
		JetAPI::ShowMessageBox(m_ErrorString);
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_DLPC350_v4::DLPSettingDelay()//DLP設定時要先延遲一段時間
{
	if ( m_DLPDelayTime > 0 )
	{	::Sleep(m_DLPDelayTime); }	
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_USE_IMP
//-------------------------------------------------------------------------------------//