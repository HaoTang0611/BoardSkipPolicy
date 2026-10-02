// Light3DTiDLP4500.cpp: implementation of the CLight3DTiDLP4500 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP4500.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_4500_USE
//-------------------------------------------------------------------------------------//
#define DLP_LED_CURRENT_MAX					255		//最高亮度
#define DLP_LED_CURRENT_MIN					0		//最低亮度
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CmdFormat CmdList[255] =
{
    {   0x07,  0x1C,  0x1C   },      //VID_SIG_STAT,
    {   0x1A,  0x00,  0x01   },      //SOURCE_SEL,
    {   0x1A,  0x02,  0x01   },      //PIXEL_FORMAT,
    {   0x1A,  0x03,  0x01   },      //CLK_SEL,
    {   0x1A,  0x37,  0x01   },      //CHANNEL_SWAP,
    {   0x1A,  0x04,  0x01   },      //FPD_MODE,
    {   0,  0,  0   },      //CURTAIN_COLOR,
    {   0x02,  0x00,  0x01   },      //POWER_CONTROL,
    {   0x10,  0x08,  0x01   },      //FLIP_LONG,
    {   0x10,  0x09,  0x01   },      //FLIP_SHORT,
    {   0x12,  0x03,  0x01   },      //TPG_SEL,
    {   0x1A,  0x05,  0x01   },      //PWM_INVERT,
    {   0x1A,  0x07,  0x01   },      //LED_ENABLE,
    {   0x02,  0x05,  0x00   },      //GET_VERSION,
    {   0x1A,  0xFF,  0x00   },      //GET_FIRMWAE_TAG_INFO
    {   0x08,  0x02,  0x00   },      //SW_RESET,
    {   0,  0,  0   },      //DMD_PARK,
    {   0x10,  0x0A,  0x01   },      //BUFFER_FREEZE,
    {   0x1A,  0x0A,  0x00   },      //STATUS_HW,
    {   0x1A,  0x0B,  0x00   },      //STATUS_SYS,
    {   0x1A,  0x0C,  0x00   },      //STATUS_MAIN,
    {   0,  0,  0   },      //CSC_DATA,
    {   0,  0,  0   },      //GAMMA_CTL,
    {   0,  0,  0   },      //BC_CTL,
    {   0x1A,  0x10,  0x01   },      //PWM_ENABLE,
    {   0x1A,  0x11,  0x06   },      //PWM_SETUP,
    {   0x1A,  0x12,  0x05   },      //PWM_CAPTURE_CONFIG,
    {   0x1A,  0x38,  0x02   },      //GPIO_CONFIG,
    {   0x0B,  0x01,  0x03   },      //LED_CURRENT,
    {   0x10,  0x00,  0x10   },      //DISP_CONFIG,
    {   0,  0,  0   },      //TEMP_CONFIG,
    {   0,  0,  0   },      //TEMP_READ,
    {   0x1A,  0x16,  0x09   },      //MEM_CONTROL,
    {   0,  0,  0   },      //I2C_CONTROL,
    {   0x1A,  0x1A,  0x01   },      //LUT_VALID,
    {   0x1A,  0x1B,  0x01   },      //DISP_MODE,
    {   0x1A,  0x1D,  0x03   },      //TRIG_OUT1_CTL,
    {   0x1A,  0x1E,  0x02   },      //TRIG_OUT2_CTL,
    {   0x1A,  0x1F,  0x02   },      //RED_STROBE_DLY,
    {   0x1A,  0x20,  0x02   },      //GRN_STROBE_DLY,
    {   0x1A,  0x21,  0x02   },      //BLU_STROBE_DLY,
    {   0x1A,  0x22,  0x01   },      //PAT_DISP_MODE,
    {   0x1A,  0x23,  0x01   },      //PAT_TRIG_MODE,
    {   0x1A,  0x24,  0x01   },      //PAT_START_STOP,
    {   0,  0,  0   },      //BUFFER_SWAP,
    {   0,  0,  0   },      //BUFFER_WR_DISABLE,
    {   0,  0,  0   },      //CURRENT_RD_BUFFER,
    {   0x1A,  0x29,  0x08   },      //PAT_EXPO_PRD,
    {   0x1A,  0x30,  0x01   },      //INVERT_DATA,
    {   0x1A,  0x31,  0x04   },      //PAT_CONFIG,
    {   0x1A,  0x32,  0x01   },      //MBOX_ADDRESS,
    {   0x1A,  0x33,  0x01   },      //MBOX_CONTROL,
    {   0x1A,  0x34,  0x00   },      //MBOX_DATA,
    {   0x1A,  0x35,  0x04   },      //TRIG_IN1_DELAY,
    {   0x1A,  0x36,  0x01   },      //TRIG_IN2_CONTROL,
    {   0x1A,  0x39,  0x01   },      //IMAGE_LOAD,
    {   0x1A,  0x3A,  0x02   },      //IMAGE_LOAD_TIMING,
    {   0x1A,  0x3B,  0x00   },      //I2C0_CTRL,
    {   0x1A,  0x3E,  0x0C   },      //MBOX_EXP_DATA,
    {   0x1A,  0x3F,  0x02   },      //MBOX_EXP_ADDRESS,
    {   0x1A,  0x40,  0x06   },      //EXP_PAT_CONFIG
    {   0x1A,  0x42,  0x01   },      //NUM_IMAGE_IN_FLASH,
    {   0x1A,  0x43,  0x01   },      //I2C0_STAT,
    {   0x08,  0x07,  0x03   },      //GPCLK_CONFIG,
    {   0,  0,  0   },      //PULSE_GPIO_23,
    {   0,  0,  0   },      //ENABLE_DLPC350_DEBUG,
    {   0x12,  0x04,  0x0C   },      //TPG_COLOR,
    {   0x1A,  0x13,  0x05   },     //PWM_CAPTURE_READ,
    {   0x30,  0x01,  0x00   },     //PROG_MODE,
    {   0x00,  0x00,  0x00   },     //BL_STATUS
    {   0x00,  0x23,  0x01   },     //BL_SPL_MODE
    {   0x00,  0x15,  0x01   },     //BL_GET_MANID,
    {   0x00,  0x15,  0x01   },     //BL_GET_DEVID,
    {   0x00,  0x15,  0x01   },     //BL_GET_CHKSUM,
    {   0x00,  0x29,  0x04   },     //BL_SETSECTADDR,
    {   0x00,  0x28,  0x00   },     //BL_SECT_ERASE,
    {   0x00,  0x2C,  0x04   },     //BL_SET_DNLDSIZE,
    {   0x00,  0x25,  0x00   },     //BL_DNLD_DATA,
    {   0x00,  0x2F,  0x01   },     //BL_FLASH_TYPE,
    {   0x00,  0x26,  0x00   },     //BL_CALC_CHKSUM,
    {   0x00,  0x30,  0x01   }     //BL_PROG_MODE,
};
//-------------------------------------------------------------------------------------//
/*
unsigned int BitPlanes[24][8] =
{
    { 0x01, 0x03, 0x07, 0x0F, 0x3E, 0x3F, 0xFE, 0xFF },  //G0 Bit-depth 1-8
    { 0x02, 0x03, 0x07, 0x0F, 0x3E, 0x3F, 0xFE, 0xFF },  //G1 Bit-depth 1-8
    { 0x04, 0x0C, 0x07, 0x0F, 0x3E, 0x3F, 0xFE, 0xFF },  //G2 Bit-depth 1-8
    { 0x08, 0x0C, 0x38, 0x0F, 0x3E, 0x3F, 0xFE, 0xFF },  //G3 Bit-depth 1-8
    { 0x10, 0x30, 0x38, 0xF0, 0x3E, 0x3F, 0xFE, 0xFF },  //G4 Bit-depth 1-8
    { 0x20, 0x30, 0x38, 0xF0, 0x3E, 0x3F, 0xFE, 0xFF },  //G5 Bit-depth 1-8
    { 0x40, 0xC0, 0x1C0, 0xF0, 0xF80, 0xFC0,0xFE, 0xFF },  //G6 Bit-depth 1-8
    { 0x80, 0xC0, 0x1C0, 0xF0, 0xF80, 0xFC0,0xFE, 0xFF }, //G7 Bit-depth 1-8


    { 0x0100, 0x0300, 0x1C0, 0xF00, 0xF80,     0xFC0,   0xFE00, 0xFF00 },  //R0 Bit-depth 1-8
    { 0x0200, 0x0300, 0xE00, 0xF00, 0xF80,     0xFC0,   0xFE00, 0xFF00 },  //R1 Bit-depth 1-8
    { 0x0400, 0x0C00, 0xE00, 0xF00, 0xF80,     0xFC0,   0xFE00, 0xFF00 },  //R2 Bit-depth 1-8
    { 0x0800, 0x0C00, 0xE00, 0xF00, 0xF80,     0xFC0,   0xFE00, 0xFF00 },  //R3 Bit-depth 1-8
    { 0x1000, 0x3000, 0x7000, 0xF000, 0x3E000,        0x3F000, 0xFE00, 0xFF00 },  //R4 Bit-depth 1-8
    { 0x2000, 0x3000, 0x7000, 0xF000, 0x3E000,  0x3F000, 0xFE00, 0xFF00 },  //R5 Bit-depth 1-8
    { 0x4000, 0xC000, 0x7000, 0xF000, 0x3E000, 0x3F000, 0xFE00, 0xFF00 },  //R6 Bit-depth 1-8
    { 0x8000, 0xC000, 0x38000, 0xF000, 0x3E000, 0x3F000, 0xFE00, 0xFF00 }, //R7 Bit-depth 1-8

    { 0x010000, 0x030000, 0x38000, 0xF0000,   0x3E000,  0x3F000,  0xFE0000, 0xFF0000 },  //B0 Bit-depth 1-8
    { 0x020000, 0x030000, 0x38000, 0xF0000,   0x3E000,  0x3F000,  0xFE0000, 0xFF0000 },  //B1 Bit-depth 1-8
    { 0x040000, 0x0C0000, 0x1C0000, 0xF0000,  0xF80000,   0xFC0000, 0xFE0000, 0xFF0000 },  //B2 Bit-depth 1-8
    { 0x080000, 0x0C0000, 0x1C0000, 0xF0000,  0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B3 Bit-depth 1-8
    { 0x100000, 0x300000, 0x1C0000, 0xF00000, 0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B4 Bit-depth 1-8
    { 0x200000, 0x300000, 0xE00000, 0xF00000, 0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B5 Bit-depth 1-8
    { 0x400000, 0xC00000, 0xE00000, 0xF00000, 0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B6 Bit-depth 1-8
    { 0x800000, 0xC00000, 0xE00000, 0xF00000, 0xF80000, 0xFC0000, 0xFE0000, 0xFF0000 },  //B7 Bit-depth 1-8
};
*/
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLight3DTiDLP4500::CLight3DTiDLP4500()
{
	PreInitTiDlp(0, LIGHT_3D_CAST_00);	
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP4500::CLight3DTiDLP4500(int CtrlID, LIGHT_3D_CAST_ID Light3DID)
{
	PreInitTiDlp(CtrlID, Light3DID);
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP4500::~CLight3DTiDLP4500()
{		
	ClearPhaseZeroBuffer();	
	ClearPhaseFactorBuffer();
	DLPDisconnect();
	m_TiUSB.USB_Exit();
	::DeleteCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP4500::PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID Light3DID)
{	
	::InitializeCriticalSection(&m_csLight3D);

	m_dwFrmVersion = 0;
	::memset(m_PatLut, 0x00, sizeof(m_PatLut));	
	::memset(m_ExpLut, 0x00, sizeof(m_ExpLut));		
	::memset(m_DLPFrmTag, 0x00, sizeof(m_DLPFrmTag));	
	::memset(m_TiAPIversion, 0x00, sizeof(m_TiAPIversion));	
	::memset(m_DLPFrmversion, 0x00, sizeof(m_DLPFrmversion));	
	::memset(m_DLPMcuversion, 0x00, sizeof(m_DLPMcuversion));

	m_CtrlBoardID = CtrlID;
	m_Light3DCastID = Light3DID;
	m_Light3DDevice = LIGHT_3D_DEVICE_DLP4500;
	m_TrigOutCount = 0;
	m_seqNum = 0;
	m_numImgInFlash = 0;
	m_PatLutIndex = 0;		
	m_ExpLutIndex = 0;
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
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP4500::InitialTiDlp()
{	
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
	m_LEDCurrentR = -1;
	m_LEDCurrentG = -1;
	m_LEDCurrentB = -1;
	m_LEDCurrentMax = 180;
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
	m_PatternIndex1_8Bit =-1;
	m_PatternIndex2_8Bit =-1;
	m_PatternIndexGC_1Bit= -1;
	m_PatternIndexBC_1Bit= -1;
	m_PatternStartNumGC_1Bit = -1;
	m_PatternStartNumBC_1Bit = -1;
	m_PatternIndexGC_8Bit= -1;
	m_PatternIndexBC_8Bit= -1;
	m_PatternStartNumGC_8Bit = -1;
	m_PatternStartNumBC_8Bit = -1;

	::memset(m_HeightFactor0, 0x00, sizeof(m_HeightFactor0));
	::memset(m_HeightFactor1, 0x00, sizeof(m_HeightFactor1));
	::memset(m_HeightFactor2, 0x00, sizeof(m_HeightFactor2));	
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP4500::LockLight3D()
{
	::EnterCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//}
inline void CLight3DTiDLP4500::UnlockLight3D()
{
	::LeaveCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4500::GetDLPErrorKeyName() const
{
	CString Key;
	CString CastName=GetDLPProjectName();	
	Key.Format(_T("[DLP:%s]"), CastName);
	return Key;
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP4500::SetLCRErrorFnName(LPCTSTR LCRFnName)
{
	this->m_ErrorString.Format(_T("Error, TiDLP %s Fault"), LCRFnName);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[])
{
#ifdef TB_SYSTEM_ONLY_BOT
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::wsprintfW(USB_Number, L"LCRB");	break;
	case LIGHT_3D_CAST_03:	::wsprintfW(USB_Number, L"LCRC");	break;
	case LIGHT_3D_CAST_04:	::wsprintfW(USB_Number, L"LCRD");	break;
	default:			    ::wsprintfW(USB_Number, L"LCRA");	break;
	}
#else
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::wsprintfW(USB_Number, L"LCR2");	break;
	case LIGHT_3D_CAST_03:	::wsprintfW(USB_Number, L"LCR3");	break;
	case LIGHT_3D_CAST_04:	::wsprintfW(USB_Number, L"LCR4");	break;
	default:			    ::wsprintfW(USB_Number, L"LCR1");	break;
	}
#endif//TB_SYSTEM_ONLY_BOT
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetDLPID(int ID)
{	
	m_CtrlBoardID = ID;
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP4500::GetDLPID() const
{
	return m_CtrlBoardID;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CAST_ID CLight3DTiDLP4500::GetCastID() const
{
	return m_Light3DCastID;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_DEVICE_TYPE CLight3DTiDLP4500::GetDeviceType() const
{
	return m_Light3DDevice;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ChangeDeviceType(LIGHT_3D_DEVICE_TYPE Type)//變更裝置型號
{
	if ( GetDeviceType() != Type )
	{
		m_ErrorString=_T("Error, CLight3DTiDLP4500 can not change Device Type");
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP4500::GetDLPProjectName() const
{
	return this->m_CastName;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP4500::GetErrorString()
{
	CString Key=GetDLPErrorKeyName();	
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP_Others(m_ErrorStringOut);
	return m_ErrorStringOut;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetDLPExceptionCode(DWORD Code, LPCTSTR Err)
{	
	CString Key=GetDLPErrorKeyName();	
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	CString strOut = JetAPI::AddKeyToErrorString(str, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP(Code, strOut);
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP4500::GetDLPImageW() const
{
	return 912;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP4500::GetDLPImageH() const
{
	return 1140;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::DLPConnect()
{
	if ( DLPConnectFn() == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_CONNECT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::DLPConnectFn()
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
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault"));
		return false; 
	}	

    // Display GUI Version #
    if (DLPC350_GetVersion(&App_ver, &API_ver, &SWConfig_ver, &SeqConfig_ver) == 0)
    {	sprintf(m_TiAPIversion, "%d.%d.%d", (API_ver >> 24), ((API_ver << 8) >> 24), ((API_ver << 16) >> 16));	}

	sprintf(m_TiAPIversion, "%d.%d.%d", GUI_VERSION_MAJOR, GUI_VERSION_MINOR, GUI_VERSION_BUILD);
	if ( API_ver >= 0x03000000 )
	{	
		m_dwFrmVersion = DLP_BIN_VERSION_3_0_0;
		sprintf(m_DLPFrmversion, "%d.%d.%d", (API_ver >> 24), ((API_ver << 8) >> 24), ((API_ver << 16) >> 16));			
	}
	else
	{		
		if ( DLPC350_MemRead(0xF902C000, &FW_ver) == 0)
		{
			FW_ver &= 0xFFFFFF;
			m_dwFrmVersion = DLP_BIN_VERSION_2_0_0;
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
	CLight3DTiDLP4500::ResetLEDDisable(ResetFinish, ShowMsg);

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
	SaveDeviceType();
	LoadDLPParameter();
	GetDLPLEDPWMInvert(m_InvertPWM);//m_InvertPWM);	
	GetDLPLEDEnable(m_LEDEnabled_Auto, m_LEDEnabled_R, m_LEDEnabled_G, m_LEDEnabled_B);	
	GetDLPLEDCurrent(m_LEDCurrentR, m_LEDCurrentG, m_LEDCurrentB);	
	ExecDLPLightSetting(DLP_LED_CURRENT_ID_01);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::DLPDisconnect(int WaitTime_ms)
{
	if ( DLPDisconnectFn(WaitTime_ms) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_DISCONNECT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::DLPDisconnectFn(int WaitTime_ms)
{
	CLight3DTiDLP4500::ExecDLPPattern_Stop();

	SaveDLPProcess(_T("DLPDisconnect"), MSG_LEVEL_HIGH);

	m_TiUSB.USB_Close();	

	m_dwFrmVersion = 0;
	::memset(m_DLPFrmTag, 0x00, sizeof(m_DLPFrmTag));	
	::memset(m_TiAPIversion, 0x00, sizeof(m_TiAPIversion));	
	::memset(m_DLPFrmversion, 0x00, sizeof(m_DLPFrmversion));	
	::memset(m_DLPMcuversion, 0x00, sizeof(m_DLPMcuversion));

	m_HWStatus = 0;
	m_SysStatus = 0;
	m_MainStatus = 0;
	m_numImgInFlash = 0;

	if ( WaitTime_ms > 0 )
	{	::Sleep(WaitTime_ms);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPIsConnected()
{
	if ( m_TiUSB.USB_IsConnected() == false )
	{
		this->m_ErrorString.Format(_T("Error, TiDLP did not connect"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CheckDLPIsConnected()
{
	return GetDLPIsConnected();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CheckDLPFrmForExpLut()//確認DLP韌體支援Exposure Lut
{
	if ( m_dwFrmVersion < DLP_BIN_VERSION_3_0_0 )
	{
		CString strFrm = m_DLPFrmversion;
		m_ErrorString.Format(_T("Error, TiDLP Firmware does not support Exp. Lut (Frm:%s)."), strFrm);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SaveDLPProcess(LPCTSTR fnName, int Level)
{
	CString str;
	str.Format(_T("Light3D[%d]::%s"), m_Light3DCastID, fnName);
	if ( AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_LIGHT3D, Level, str) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPSoftwareReset(DWORD delayTime)
{
	if ( ExecDLPSoftwareResetFn(delayTime) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_RESET);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPSoftwareResetFn(DWORD delayTime)
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
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault"));
		return false;
	}

	this->ReadDLPParameter();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetDLPLongAxisImageFlip(bool Flip)
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
bool CLight3DTiDLP4500::GetDLPLongAxisImageFlip()
{
	return DLPC350_GetLongAxisImageFlip();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetDLPShortAxisImageFlip(bool Flip)
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
bool CLight3DTiDLP4500::GetDLPShortAxisImageFlip()
{
	return DLPC350_GetShortAxisImageFlip();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetDLPOperationMode(int Mode)
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
int CLight3DTiDLP4500::GetDLPOperationMode() const
{	
	return this->m_OperationMode;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue)
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

	CLight3DTiDLP4500::m_LEDEnabled_R = bRed;
	CLight3DTiDLP4500::m_LEDEnabled_G = bGreen;
	CLight3DTiDLP4500::m_LEDEnabled_B = bBlue;
	CLight3DTiDLP4500::m_LEDEnabled_Auto = bSeqCtrl;
	this->m_DLPParam.m_LEDEnabled_Auto = bSeqCtrl;
	this->m_DLPParam.m_LEDEnabled_R = bRed;
	this->m_DLPParam.m_LEDEnabled_G = bGreen;
	this->m_DLPParam.m_LEDEnabled_B = bBlue;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue)
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
inline unsigned char CLight3DTiDLP4500::GetDLPSafeCurrent(int value)
{	
	int Max=m_LEDCurrentMax;
	int Min=MAX(DLP_LED_CURRENT_MIN, 0);	
	if ( value > Max ) { return Max; }
	if ( value < Min ) { return Min; }	
	return static_cast<unsigned char>(value);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SaveDLPCurrentProcess(LPCTSTR pContext, bool bShowMsg)//儲存現在狀態
{	
	m_ErrorString = pContext;

	CString Err = GetErrorString();	
	AOIDataCollect.SaveCurrentProcess(Err);
	if ( true == bShowMsg )
	{	JetAPI::ShowMessageBox(Err); }
	return true;
}
//-------------------------------------------------------------------------------------//
inline int CLight3DTiDLP4500::GetDLPTrigType(int index, bool IntTrig, bool MultiTable)
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
bool CLight3DTiDLP4500::SaveDeviceType()
{	
	bool IsOK=true;
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section=_T("TiDLP");	
	LIGHT_3D_DEVICE_TYPE Type=GetDeviceType();
	CString ProjectName = this->GetDLPProjectName();	

	FileName = AOIDataCollect.GetLight3DParamFilename();
	Section.Format(_T("TiDLP-%s"), ProjectName);		

	KeyName.Format(_T("Device Type")); KeyString.Format(_T("%d"),Type);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID)
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
			this->m_ErrorString.Format(_T("Error, TiDLP DLPC350_SetLedCurrents Fault [R:%d, G:%d, B:%d]"), red, green, blue);
			return false; 
		}	
	}
	
	CLight3DTiDLP4500::m_LEDCurrentR = red;
	CLight3DTiDLP4500::m_LEDCurrentG = green;
	CLight3DTiDLP4500::m_LEDCurrentB = blue;
	//if ( true == Update )//要此過濾再校正電流會有問題
	{
		SetDLPParamLEDCurrent(red, green, blue, CurrentID);		
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPLEDCurrent(int &red, int &green, int &blue)
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
bool CLight3DTiDLP4500::SetDLPLEDPWMInvert(bool bInvert)
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
	CLight3DTiDLP4500::m_InvertPWM = bInvert;
	this->m_DLPParam.m_InvertPWM = bInvert;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPLEDPWMInvert(bool &bInvert)
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
bool CLight3DTiDLP4500::CheckDLPStatus()
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
bool CLight3DTiDLP4500::GetDLPStatus_InitDone()
{
	return bool(m_HWStatus&BIT0);
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPStatus_ForcedSwap()
{
	if ( (m_HWStatus&BIT3) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPStatus_BufferFreeze()
{
	if ( (m_MainStatus&BIT2) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPStatus_SeqRunning()
{
	if ( (m_MainStatus&BIT1) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPStatus_SeqError()
{
	if ( (m_HWStatus&BIT7) == 0 ) 
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPStatus_SeqAbort()
{
	if ( (m_HWStatus&BIT6) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPStatus_DRCError()
{
	if ( (m_HWStatus&BIT2) == 0 ) 
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetDLPStatus_DMDParked()
{
	if ( (m_MainStatus&BIT0) == 0 )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP4500::InitialDLPParameter(TDLPParam &Param)
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
bool CLight3DTiDLP4500::SaveDLPParameter()
{	
	bool    IsOK = true;	
#ifndef PHASE_CTRL_DISABLE
#ifndef OFFLINE_VERSION
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section=_T("TiDLP");	
	CString ProjectName = this->GetDLPProjectName();

	FileName = AOIDataCollect.GetLight3DParamFilename();
	Section.Format(_T("TiDLP-%s"), ProjectName);

	KeyName.Format(_T("LED Color")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDColor);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("Trig Type")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigType);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("Bit Depth")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternBitDepth);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Trig Interval")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigInterval_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Pattern Exp")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternExp_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Phase Mode")); KeyString.Format(_T("%d"),m_DLPParam.m_PhaseMode);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Period Time")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Exposure Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("Period Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime2_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Exposure Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime2_us);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("LED Current R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_1);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("LED Current G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_1);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_1);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current R 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_2);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("LED Current G 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_2);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Current B 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_2);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		

	KeyName.Format(_T("Invert PWM")); KeyString.Format(_T("%d"),m_DLPParam.m_InvertPWM);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled Auto")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_Auto);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_R);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("LED Enabled G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_G);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	
		
	KeyName.Format(_T("LED Enabled B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_B);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Flip Long")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipLong);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Flip Short")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipShort);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Validate Delay Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ValidateDelayTime);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Use Exposure Pattern")); KeyString.Format(_T("%d"),m_DLPParam.m_UseExpLut);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }	

	KeyName.Format(_T("Phase Factor Min")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMin);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }		 

	KeyName.Format(_T("Phase Factor Max")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMax);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	KeyName.Format(_T("LED Current Max")); KeyString.Format(_T("%d"), m_DLPParam.m_LEDCurrentMax);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Image Gamma")); KeyString.Format(_T("%.6f"), m_DLPParam.m_ImageGamma);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("2nd Exposure Ratio")); KeyString.Format(_T("%.6f"), m_DLPParam.m_SecondExpRatio);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}
#endif//OFFLINE_VERSION
#endif//PHASE_CTRL_DISABLE
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadDLPParameter()
{	
#ifndef PHASE_CTRL_DISABLE
	CString FileName;
	CString TempStr;
	CString KeyName;
	CString KeyString;
	CString Section=_T("DLP");
	CString ProjectName = this->GetDLPProjectName();
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];

	FileName = AOIDataCollect.GetLight3DParamFilename();
	Section.Format(_T("TiDLP-%s"),ProjectName);
	
	//LED Color	
	KeyName.Format(_T("LED Color")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDColor);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDColor = ::_ttoi(ReturnString); }
	m_PhaseZeroLEDColor = m_DLPParam.m_LEDColor;
	m_PhaseFactorLEDColor = m_DLPParam.m_LEDColor;
		
	//Trig Type	
	KeyName.Format(_T("Trig Type")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigType);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_TrigType = ::_ttoi(ReturnString);  }

	//Bit Depth	
	KeyName.Format(_T("Bit Depth")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternBitDepth);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PatternBitDepth = ::_ttoi(ReturnString); }

	//TrigInterval_us	
	KeyName.Format(_T("Trig Interval")); KeyString.Format(_T("%d"),m_DLPParam.m_TrigInterval_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_TrigInterval_us = ::_ttoi(ReturnString); }

	//PatternExp_us	
	KeyName.Format(_T("Pattern Exp")); KeyString.Format(_T("%d"),m_DLPParam.m_PatternExp_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PatternExp_us = ::_ttoi(ReturnString); }

	//Phase Mode
	KeyName.Format(_T("Phase Mode")); KeyString.Format(_T("%d"),m_DLPParam.m_PhaseMode);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PhaseMode = ::_ttoi(ReturnString); }
	m_PhaseZeroPhaseMode = m_DLPParam.m_PhaseMode;
	m_PhaseFactorPhaseMode = m_DLPParam.m_PhaseMode;
		
	//Period
	KeyName.Format(_T("Period Time")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PeriodTime_us = ::_ttoi(ReturnString); }

	//LED Exposure Time
	KeyName.Format(_T("Exposure Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_ExposureTime_us = ::_ttoi(ReturnString); }
	
	m_DLPParam.m_PeriodTime2_us = m_DLPParam.m_PeriodTime_us;
	m_DLPParam.m_ExposureTime2_us = m_DLPParam.m_ExposureTime_us;
	//Period 2
	KeyName.Format(_T("Period Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_PeriodTime2_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_PeriodTime2_us = ::_ttoi(ReturnString); }

	//LED Exposure Time 2
	KeyName.Format(_T("Exposure Time 2")); KeyString.Format(_T("%d"),m_DLPParam.m_ExposureTime2_us);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_ExposureTime2_us = ::_ttoi(ReturnString); }	

	if ( m_DLPParam.m_PeriodTime_us < m_DLPParam.m_ExposureTime_us )
	{	m_DLPParam.m_PeriodTime_us = m_DLPParam.m_ExposureTime_us; }

	if ( m_DLPParam.m_PeriodTime2_us < m_DLPParam.m_ExposureTime2_us )
	{	m_DLPParam.m_PeriodTime2_us = m_DLPParam.m_ExposureTime2_us; }

	//LEDCurrent_R	
	KeyName.Format(_T("LED Current R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_1);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentR_1 = ::_ttoi(ReturnString); }

	//LEDCurrent_G	
	KeyName.Format(_T("LED Current G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_1);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{	m_DLPParam.m_LEDCurrentG_1 = ::_ttoi(ReturnString); }
		
	//LEDCurrent_B	
	KeyName.Format(_T("LED Current B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_1);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentB_1 = ::_ttoi(ReturnString); }

	//LEDCurrent_R_2
	KeyName.Format(_T("LED Current R 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentR_2);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentR_2 = ::_ttoi(ReturnString); }

	//LEDCurrent_G_2	
	KeyName.Format(_T("LED Current G 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentG_2);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{	m_DLPParam.m_LEDCurrentG_2 = ::_ttoi(ReturnString); }
		
	//LEDCurrent_B_2
	KeyName.Format(_T("LED Current B 2")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDCurrentB_2);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	m_DLPParam.m_LEDCurrentB_2 = ::_ttoi(ReturnString); }	


	//InvertPWM	
	KeyName.Format(_T("Invert PWM")); KeyString.Format(_T("%d"),m_DLPParam.m_InvertPWM);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_InvertPWM=true;	}
		else
		{	m_DLPParam.m_InvertPWM=false;	}	
	}

	//LEDEnabled_Auto	
	KeyName.Format(_T("LED Enabled Auto")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_Auto);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_Auto=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_Auto=false;	}	
	}
		
	//LEDEnabled_R	
	KeyName.Format(_T("LED Enabled R")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_R);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_R=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_R=false;	}	
	}

	//LEDEnabled_G
	KeyName.Format(_T("LED Enabled G")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_G);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_G=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_G=false;	}	
	}

	//LEDEnabled_B	
	KeyName.Format(_T("LED Enabled B")); KeyString.Format(_T("%d"),m_DLPParam.m_LEDEnabled_B);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_LEDEnabled_B=true;	}
		else
		{	m_DLPParam.m_LEDEnabled_B=false;	}	
	}

	//FlipLong	
	KeyName.Format(_T("Flip Long")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipLong);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_FlipLong=true;	}
		else
		{	m_DLPParam.m_FlipLong=false;	}	
	}

	//FlipShort
	KeyName.Format(_T("Flip Short")); KeyString.Format(_T("%d"),m_DLPParam.m_FlipShort);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_FlipShort=true;	}
		else
		{	m_DLPParam.m_FlipShort=false;	}	
	}

	KeyName.Format(_T("Validate Delay Time")); KeyString.Format(_T("%d"),m_DLPParam.m_ValidateDelayTime);	
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_ValidateDelayTime = ::_ttoi(ReturnString);
		if( m_DLPParam.m_ValidateDelayTime < 0 )
		{	m_DLPParam.m_ValidateDelayTime=0;	}		
	}

	KeyName.Format(_T("Use Exposure Pattern")); KeyString.Format(_T("%d"),m_DLPParam.m_UseExpLut);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		if( ::_ttoi(ReturnString)>0 )
		{	m_DLPParam.m_UseExpLut=true;	}
		else
		{	m_DLPParam.m_UseExpLut=false;	}	
	}

	KeyName.Format(_T("Phase Factor Min")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMin);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_PhaseFactorMin = ::_ttof(ReturnString);		
		if ( m_DLPParam.m_PhaseFactorMin < 0 ) 
		{	m_DLPParam.m_PhaseFactorMin = 0; } 
	}	

	KeyName.Format(_T("Phase Factor Max")); KeyString.Format(_T("%.lf"),m_DLPParam.m_PhaseFactorMax);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )
	{
		m_DLPParam.m_PhaseFactorMax = ::_ttof(ReturnString);		
		if ( m_DLPParam.m_PhaseFactorMax < 0 ) 
		{	m_DLPParam.m_PhaseFactorMax = 0; } 
	}

	KeyName.Format(_T("LED Current Max")); KeyString.Format(_T("%d"), m_DLPParam.m_LEDCurrentMax);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{
		m_LEDCurrentMax = ::_ttoi(ReturnString);
		m_DLPParam.m_LEDCurrentMax = ::_ttoi(ReturnString);
		if (m_LEDCurrentMax < 10)
		{	m_LEDCurrentMax = 10;	}
		if (m_LEDCurrentMax > 255)
		{	m_LEDCurrentMax = 255;	}
	}

	//Image Gamma
	KeyName.Format(_T("Image Gamma")); KeyString.Format(_T("%.6f"), m_DLPParam.m_ImageGamma);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_ImageGamma = ::_ttof(ReturnString);	}
	m_ImageGamma = m_DLPParam.m_ImageGamma;

	KeyName.Format(_T("2nd Exposure Ratio")); KeyString.Format(_T("%.6f"), m_DLPParam.m_SecondExpRatio);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_SecondExpRatio = ::_ttof(ReturnString);	}
	m_SecondExpRatio = m_DLPParam.m_SecondExpRatio;
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ReadDLPParameter()//從DLP裝置讀取參數
{
#ifndef PHASE_CTRL_DISABLE
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
	this->m_DLPParam.m_InvertPWM = bInvert;
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
TDLPParam& CLight3DTiDLP4500::GetDLPParam()
{
	return m_DLPParam;
}
//-------------------------------------------------------------------------------------//
const TDLPParam& CLight3DTiDLP4500::GetDLPParam() const
{
	return m_DLPParam;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetDLPPhaseMode(int Mode)
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
bool CLight3DTiDLP4500::SetDLPParamLEDCurrent(int red, int green, int blue, int CurrentID)
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
bool CLight3DTiDLP4500::GetDLPParamLEDCurrent(int &red, int &green, int &blue, int CurrentID)
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
const char* CLight3DTiDLP4500::GetDLPFrmTag()
{
	return this->m_DLPFrmTag;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP4500::GetTiAPIVersion()
{
	return this->m_TiAPIversion;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP4500::GetDLPFrmVersion()
{
	return this->m_DLPFrmversion;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP4500::GetDLPMcuVersion()
{
	return this->m_DLPMcuversion;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetUseExpLut()
{
	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	
	return m_DLPParam.m_UseExpLut;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetLEDColor(int Type)
{
	m_LEDColor = Type;	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetLEDColor() const
{
	return m_LEDColor;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP4500::GetImageGamma() const
{
	//return 1.00f;
	//return 1.0f;
	return m_ImageGamma;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetImageGamma(double val)
{
	m_ImageGamma = val;
	m_DLPParam.m_ImageGamma = val;
	return true;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP4500::GetSecondExpRatio() const//取得第2次曝光比例
{
	return m_SecondExpRatio;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetSecondExpRatio(double val)
{
	m_SecondExpRatio = val;
	m_DLPParam.m_SecondExpRatio = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPeriodPaddingTime() const//週期外加時間-us
{
	return m_DLPParam.m_PeriodPaddingTime;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetExposurePaddingTime() const//曝光外加時間-us
{
	return m_DLPParam.m_ExposurePaddingTime;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::CalcPeriodPaddingTime(int ExpTime) const//計算週期外加時間-us
{
	return GetPeriodPaddingTime();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetUse3BitPattern() const//取得使用3Bit樣板圖
{
	const int PatternBitCount=GetPatternBitCount();
	if ( 3 != PatternBitCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetUse5BitPattern() const//取得使用5Bit樣板圖
{
	const int PatternBitCount=GetPatternBitCount();
	if ( 5 != PatternBitCount ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternBitCount() const//取得樣板圖位元數
{
	return m_PatternBitCount;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternBitCount(int val)//設定樣板圖位元數
{
	m_PatternBitCount = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndex1_3Bit() const//取得使用3Bit樣板引數-1
{
	return m_PatternIndex1_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndex1_3Bit(int val)//設定使用3Bit樣板引數-1
{
	m_PatternIndex1_3Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndex2_3Bit() const//取得使用3Bit樣板引數-2
{
	return m_PatternIndex2_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndex2_3Bit(int val)//設定使用3Bit樣板引數-2
{
	m_PatternIndex2_3Bit = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndex1_5Bit() const//取得使用5Bit樣板引數-1
{
	return m_PatternIndex1_5Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndex1_5Bit(int val)//設定使用5Bit樣板引數-1
{
	m_PatternIndex1_5Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndex1_6Bit() const//取得使用6Bit樣板引數-1
{
	return m_PatternIndex1_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndex1_6Bit(int val)//設定使用6Bit樣板引數-1
{
	m_PatternIndex1_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndex2_6Bit() const//取得使用6Bit樣板引數-2
{
	return m_PatternIndex2_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndex2_6Bit(int val)//設定使用6Bit樣板引數-2
{
	m_PatternIndex2_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndex1_8Bit() const//取得使用8Bit樣板引數-1
{
	return m_PatternIndex1_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndex1_8Bit(int val)//設定使用8Bit樣板引數-1
{
	m_PatternIndex1_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndex2_8Bit() const//取得使用8Bit樣板引數-2
{
	return m_PatternIndex2_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndex2_8Bit(int val)//設定使用8Bit樣板引數-2
{
	m_PatternIndex2_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndexGC_1Bit() const//取得使用1Bit-GrayCode引數-1
{
	return m_PatternIndexGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndexGC_1Bit(int val)//設定使用1Bit-GrayCode引數-1
{
	m_PatternIndexGC_1Bit = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternIndexBC_1Bit() const//取得使用1Bit-BinaryCode引數-1
{
	return m_PatternIndexBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternIndexBC_1Bit(int val)//設定使用1Bit-BinaryCode引數-1
{
	m_PatternIndexBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternStartNumGC_1Bit() const//取得使用1Bit-GrayCode起始張數-1
{
	return m_PatternStartNumGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternStartNumGC_1Bit(int val)//設定使用1Bit-GrayCode起始張數-1
{
	m_PatternStartNumGC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetPatternStartNumBC_1Bit() const//取得使用1Bit-BinaryCode起始張數-1
{
	return m_PatternStartNumBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPatternStartNumBC_1Bit(int val)//設定使用1Bit-BinaryCode起始張數-1
{
	m_PatternStartNumBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//	
int CLight3DTiDLP4500::GetPatternIndexGC_8Bit() const//取得使用8Bit-GrayCode引數-1
{
	return m_PatternIndexGC_8Bit;
}
//-------------------------------------------------------------------------------------//	
void CLight3DTiDLP4500::SetPatternIndexGC_8Bit(int val)//設定使用8Bit-GrayCode引數-1
{
	m_PatternIndexGC_8Bit = val;
}
//-------------------------------------------------------------------------------------//	
int CLight3DTiDLP4500::GetPatternIndexBC_8Bit() const//取得使用8Bit-BinaryCode引數-1
{
	return m_PatternIndexBC_8Bit;
}
//-------------------------------------------------------------------------------------//	
void CLight3DTiDLP4500::SetPatternIndexBC_8Bit(int val)//設定使用8Bit-BinaryCode引數-1
{
	m_PatternIndexBC_8Bit = val;
}
//-------------------------------------------------------------------------------------//	
int CLight3DTiDLP4500::GetPatternStartNumGC_8Bit() const//取得使用8Bit-GrayCode起始張數-1
{
	return m_PatternStartNumGC_8Bit;
}
//-------------------------------------------------------------------------------------//	
void CLight3DTiDLP4500::SetPatternStartNumGC_8Bit(int val)//設定使用8Bit-GrayCode起始張數-1
{
	m_PatternStartNumGC_8Bit = val;
}
//-------------------------------------------------------------------------------------//	
int CLight3DTiDLP4500::GetPatternStartNumBC_8Bit() const//取得使用8Bit-BinaryCode起始張數-1
{
	return m_PatternStartNumBC_8Bit;
}
//-------------------------------------------------------------------------------------//	
void CLight3DTiDLP4500::SetPatternStartNumBC_8Bit(int val)//設定使用8Bit-BinaryCode起始張數-1
{
	m_PatternStartNumBC_8Bit = val;
}
//-------------------------------------------------------------------------------------//	
void CLight3DTiDLP4500::SetPeriod_us(unsigned int val)
{
	m_TrigPeriod_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetExposure_us(unsigned int val)
{
	m_Exposure_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPeriod2_us(unsigned int val)
{
	m_TrigPeriod2_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetExposure2_us(unsigned int val)
{
	m_Exposure2_us = val;		
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::InitialPatItem(TDLPPatItem &PatItem)
{
	PatItem = TDLPPatItem();	
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPatClear()//清除樣版內容
{
	SaveDLPProcess(_T("ExecDLPPatClear"), MSG_LEVEL_HIGH);

	this->m_PatternList.clear();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{
	if ( ExecDLPPatReadFn(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_READ);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPatReadFn(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{
	if ( true == bExpLut )
	{	return ExecDLPPatRead_ExpLut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat); }
	return ExecDLPPatRead_Lut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPatRead_Lut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
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
bool CLight3DTiDLP4500::ExecDLPPatRead_ExpLut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
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
bool CLight3DTiDLP4500::ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{
	if ( ExecDLPPatSendAllFn(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_SEND_ALL);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPatSendAllFn(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{
	if ( true == bExpLut )
	{	return ExecDLPPatSendAll_ExpLut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat); }
	return ExecDLPPatSendAll_Lut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPatSendAll_Lut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
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
bool CLight3DTiDLP4500::ExecDLPPatSendAll_ExpLut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
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
bool CLight3DTiDLP4500::ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( ExecDLPPatSendOneFn(bExpLut, index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_SEND_ONE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPatSendOneFn(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( true == bExpLut )
	{	return ExecDLPPatSendOne_ExpLut(index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	}
	return ExecDLPPatSendOne_Lut(index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPatSendOne_Lut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
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
bool CLight3DTiDLP4500::ExecDLPPatSendOne_ExpLut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
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
bool CLight3DTiDLP4500::ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep)//套用樣板列表資料
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ExecDLPValidatePatLutData"), MSG_LEVEL_HIGH);

	if ( DLPC350_ValidatePatLutData(&Status) < 0 )
    {
        this->SetLCRErrorFnName(_T("DLPC350_ValidatePatLutData"));
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_VALIDATE);
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
bool CLight3DTiDLP4500::ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force)//建立樣板, 傳送樣板以及驗證
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
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_VALIDATE);
		return false; 
	}
	m_PatternMode = Mode;
	SetLEDColor(LEDColor);
	SetPeriod_us(TrigPeriod_us);
	SetExposure_us(Exposure_us);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPLightSetting(int CurrentID)//執行DLP的LED設定-依據目前的設定
{
	const bool bUpdate = false;
	int CurRed=0, CurGrn=0, CurBlu= 0;
	CLight3DTiDLP4500 *Light3DPtr = this;
	Light3DPtr->GetDLPParamLEDCurrent(CurRed, CurGrn, CurBlu, CurrentID);
	if ( Light3DPtr->SetDLPLEDEnable(true, true, true, true) == false )
	{	return false;	}		
	if ( Light3DPtr->SetDLPLEDCurrent(CurRed, CurGrn, CurBlu, bUpdate, CurrentID) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPattern_Run()
{
	if ( ExecDLPPattern_RunFn() == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_PLAY);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPattern_RunFn()
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
bool CLight3DTiDLP4500::ExecDLPPattern_Stop()
{
	if ( ExecDLPPattern_StopFn() == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_STOP);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPattern_StopFn()
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
bool CLight3DTiDLP4500::ExecDLPPattern_Pause()
{
	if ( ExecDLPPattern_PauseFn() == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_PATTERN_PAUSE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ExecDLPPattern_PauseFn()
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
bool CLight3DTiDLP4500::LEDSetting(int LEDCurrent, int CurrentID)
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
size_t CLight3DTiDLP4500::GetTriggerOutCount()
{
	return m_TrigOutCount;
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP4500::GetDLPPatCount()
{
	return m_PatternList.size();
}
//-------------------------------------------------------------------------------------//
TDLPPatItem* CLight3DTiDLP4500::GetDLPPatItemPtr(size_t index, bool check)
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
void CLight3DTiDLP4500::ClearDLPPatternList()//清除m_PatternList
{
	SaveDLPProcess(_T("ClearDLPPatternList"), MSG_LEVEL_HIGH);
	m_PatternList.clear();
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表 
{
	if ( BuildDLPPatternListFn(Mode, IntTrig, MultiTable, LEDColor) == false )
	{
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_SET_FUNC);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::BuildDLPPatternListFn(int Mode, bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表 
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

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:15P, 9:220P, 10:224P+127Gray
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
void CLight3DTiDLP4500::AddDLPPPatItem(TDLPPatItem &PatItem)//增加樣板項目
{
	AOIDataDefine.CalcDLPBitPosRange(PatItem.sBitDepth, PatItem.sBitNum, PatItem.sBitStart, PatItem.sBitEnd);
	m_PatternList.push_back(PatItem);	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::RemoveDLPPatItem(size_t index)//移除樣板項目
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
bool CLight3DTiDLP4500::BuildPatternImage(int BitDepth, int NPeriod, int NPixelPeriod, bool bVer, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage)
{
	//G-8bit R-8bit B-8bit
	const char fnName[]="CLight3DTiDLP4500::BuildPatternImage";
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
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
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
bool CLight3DTiDLP4500::CheckPhaseZeroWhite()//確認平面相位
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
bool CLight3DTiDLP4500::CheckPhaseZeroDebug()//確認平面相位
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
bool CLight3DTiDLP4500::CheckPhaseZeroRed()//確認平面相位
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
bool CLight3DTiDLP4500::CheckPhaseZeroGrn()//確認平面相位
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
bool CLight3DTiDLP4500::CheckPhaseZeroBlu()//確認平面相位
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
CString CLight3DTiDLP4500::GetDLPPhaseZeroBinShortName(int LedClr)//取得相平面的校正短名
{
	CString ShortName;	
	CString KeyName=_T("TiDLPZero");	
	CString ProjectName = GetDLPProjectName();
	CString ColorName = GetDLPLedColorTextForBinFile(LedClr);
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);

#ifdef TB_SYSTEM_ONLY_BOT
	ShortName.Format(_T("%s%s_%s_%s_Bot.BIN"), KeyName, PhaseMode, ProjectName, ColorName);
#else
	ShortName.Format(_T("%s%s_%s_%s.BIN"), KeyName, PhaseMode, ProjectName, ColorName);		
#endif//TB_SYSTEM_ONLY_BOT
	return ShortName;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4500::GetDLPPhaseFactorBinShortName(int LedClr)//取相位比例的校正短名
{
	CString ShortName;
	CString KeyName=_T("TiDLPFactor");	
	CString ProjectName = GetDLPProjectName();
	CString ColorName = GetDLPLedColorTextForBinFile(LedClr);
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);

#ifdef TB_SYSTEM_ONLY_BOT
	ShortName.Format(_T("%s%s_%s_%s_Bot.BIN"), KeyName, PhaseMode, ProjectName, ColorName);
#else
	ShortName.Format(_T("%s%s_%s_%s.BIN"), KeyName, PhaseMode, ProjectName, ColorName);		
#endif//TB_SYSTEM_ONLY_BOT
	return ShortName;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4500::GetDLPPhaseZeroBinFilename(int LedClr)//取得相平面的校正檔名
{
	CString Filename;	
	CString ShortName=GetDLPPhaseZeroBinShortName(LedClr);		
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);
	return Filename;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4500::GetDLPLedColorTextForBinFile(int Color)//取得燈源模式的文字	
{
	CString ColorText;
	switch ( Color )
	{
	case DLP_LED_COLOR_RED:		ColorText=_T("Red"); break;
	case DLP_LED_COLOR_GREEN:	ColorText=_T("Green"); break;
	case DLP_LED_COLOR_BLUE:	ColorText=_T("Blue"); break;
	case DLP_LED_COLOR_WHITE:	ColorText=_T("White"); break;
	case DLP_LED_COLOR_DEBUG:	ColorText=_T("Debug"); break;
	default:
		ColorText=_T("LED");
		break;
	}
	return ColorText;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4500::GetDLPPhaseModeTextForBinFile(int Mode)//取得相位模式的文字
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
	case LIGHT3D_PHASE_4_5GC_M_2:
		Mode = LIGHT3D_PHASE_4_5GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	case LIGHT3D_PHASE_4_6GC_M_2:
		Mode = LIGHT3D_PHASE_4_6GC_M;
		Mode = LIGHT3D_PHASE_4_4_M;
		break;
	}
	return AOIDataDefine.GetDLPPhaseModeText(Mode);
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP4500::GetDLPPhaseFactorBinFilename(int LedClr)//取相位比例的校正檔名
{
	CString Filename;	
	CString ShortName=GetDLPPhaseFactorBinShortName(LedClr);	
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);
	return Filename;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseZero()//載入平面相位
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
bool CLight3DTiDLP4500::LoadPhaseZeroRed()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_RED;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);
	
	ClearPhaseZeroBufferRed();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseZeroGrn()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_GREEN;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);
	
	ClearPhaseZeroBufferGrn();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseZeroBlu()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_BLUE;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);
	
	ClearPhaseZeroBufferBlu();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseZeroWhite()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_WHITE;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);
	
	ClearPhaseZeroBufferWhite();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseZeroDebug()//載入平面相位
{
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	const int    LedClr = DLP_LED_COLOR_DEBUG;
	CString Filename = GetDLPPhaseZeroBinFilename(LedClr);
	
	ClearPhaseZeroBufferDebug();
	if ( LoadDLPPhaseZeroFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		//if ( LoadDLPPhaseZeroFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseZeroW = ImageW;
	m_PhaseZeroH = ImageH;
	m_PhaseZeroStep = ImageStep;
	m_PhaseZeroPtr = ImagePtr;
	m_PhaseZeroLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr)//載入DLP平面相位
{
#ifndef PHASE_CTRL_DISABLE
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP4500::LoadDLPPhaseZeroFile";
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

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
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
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadDLPPhaseZeroFile2(int LedClr, LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr)//載入DLP平面相位
{
#ifdef MUST_BACKUP_SYSTEM_FILE_USE
	CString Filename2;
	CString Filename = pfilename;
	CString ShortName=GetDLPPhaseZeroBinShortName(LedClr);		
	Filename2.Format(_T("%s\\%s"), AOIDataCollect.GetSystemBackupFolder(), ShortName);
	if ( JetAPI::IsFileExist(Filename2) == false )
	{	return false; }		
	::DeleteFile(Filename);
	JetMemory.free_func(PhasePtr);
	::CopyFile(Filename2, Filename, FALSE);
	if ( LoadDLPPhaseZeroFile(Filename, PhaseW, PhaseH, PhaseStep, PhasePtr) == false ) 
	{	return false; }
	return true;
#endif//MUST_BACKUP_SYSTEM_FILE_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseZero()//儲存平面相位
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
bool CLight3DTiDLP4500::SavePhaseZeroRed()//儲存平面相位
{
	if ( CheckPhaseZeroRed() == false )
	{	return true; }	

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseZeroGrn()//儲存平面相位
{
	if ( CheckPhaseZeroGrn() == false )
	{	return true; }

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseZeroBlu()//儲存平面相位
{
	if ( CheckPhaseZeroBlu() == false )
	{	return true; }

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseZeroWhite()//儲存平面相位
{
	if ( CheckPhaseZeroWhite() == false )
	{	return true; }

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseZeroDebug()//儲存平面相位
{
	if ( CheckPhaseZeroDebug() == false )
	{	return true; }

	CString Filename = GetDLPPhaseZeroBinFilename(m_PhaseZeroLEDColor);
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CheckDLPPhaseZeroData(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr)//確認DLP平面相位資料
{
	if ( 0==PhaseW || 0==PhaseH || 0==PhaseStep )
	{
		m_ErrorString.Format(_T("Error, DLP Phase Zero Data Exception(W=%d, H=%d, Step=%d)"), PhaseW, PhaseH, PhaseStep);
		return false;	
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SaveDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr)//儲存DLP平面相位
{
#ifndef PHASE_CTRL_DISABLE
#ifndef OFFLINE_VERSION
	if ( NULL == pfilename ) { return false; }
	if ( CheckDLPPhaseZeroData(PhaseW, PhaseH, PhaseStep, PhasePtr) == false )
	{	return false; }

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
#endif//PHASE_CTRL_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ClearPhaseZeroBuffer()//清除平面相位
{
	ClearPhaseZeroBufferRed();
	ClearPhaseZeroBufferGrn();
	ClearPhaseZeroBufferBlu();
	ClearPhaseZeroBufferWhite();
	ClearPhaseZeroBufferDebug();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ClearPhaseZeroBufferRed()//清除平面相位	
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
bool CLight3DTiDLP4500::ClearPhaseZeroBufferGrn()//清除平面相位	
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
bool CLight3DTiDLP4500::ClearPhaseZeroBufferBlu()//清除平面相位	
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
bool CLight3DTiDLP4500::ClearPhaseZeroBufferWhite()//清除平面相位	
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
bool CLight3DTiDLP4500::ClearPhaseZeroBufferDebug()//清除平面相位
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
bool CLight3DTiDLP4500::CheckDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep)//確認DLP平面相位檔案
{
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP4500::CheckDLPPhaseZeroFile";
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

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( 0 == BufferSize ) 
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BuferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);	pFile = NULL;

	if ( PhaseW!=TempW || PhaseH!=TempH || PhaseStep!=TempStep )
	{
		this->m_ErrorString.Format(_T("Error, Check Bin Data Size Fault(%s::W:%d, H:%d, Step:%d)"), Filename, TempW, TempH, TempStep);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr)//設定平面相位
{
	const char fnName[] = "CLight3DTiDLP4500::SetPhaseZero";	
	if ( NULL == Ptr ) { return false; }

	PHASE_PTR PhasePtr=NULL;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageSetp, ImageH);
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
bool CLight3DTiDLP4500::GetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)//取得平面相位
{
	bool IsOK = true;
	bool ReloadFile = false;	 
	LockLight3D();
	if ( PhaseMode!=m_PhaseZeroPhaseMode || m_PhaseZeroLEDColor!=LEDColor || NULL==m_PhaseZeroPtr )
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
bool CLight3DTiDLP4500::ClonePhaseZero(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)//複製平面相位
{
	IMAGE_SIZE PhaseZeroW=0;
	IMAGE_SIZE PhaseZeroH=0;
	IMAGE_SIZE PhaseZeroStep=0;
	PHASE_PTR  PhaseZeroPtr=NULL;
	const int PhaseMode = m_PhaseZeroPhaseMode;		
	if ( GetPhaseZero(PhaseMode, LEDColor, PhaseZeroW, PhaseZeroH, PhaseZeroStep, PhaseZeroPtr) == false )
	{	return false; }

	JetMemory.free_func(Ptr);
	const char fnName[] = "CLight3DTiDLP4500::ClonePhaseZero";	
	const size_t BufferSize = ImageAPI.CalcBufferSize(PhaseZeroStep, PhaseZeroH);
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
bool CLight3DTiDLP4500::CheckPhaseFactorRed()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_RED!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Red]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CheckPhaseFactorGrn()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_GREEN!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Green]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CheckPhaseFactorBlu()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_BLUE!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Blue]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CheckPhaseFactorWhite()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_WHITE!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[White]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CheckPhaseFactorDebug()//確認平面係數
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_DEBUG!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Debug]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseFactor()//載入平面係數
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
bool CLight3DTiDLP4500::LoadPhaseFactorRed()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_RED;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);
	
	ClearPhaseFactorBufferRed();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseFactorGrn()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_GREEN;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);
	
	ClearPhaseFactorBufferGrn();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseFactorBlu()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_BLUE;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);
	
	ClearPhaseFactorBufferBlu();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseFactorWhite()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_WHITE;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);
	
	ClearPhaseFactorBufferWhite();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadPhaseFactorDebug()//載入平面係數
{
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	const int     LedClr = DLP_LED_COLOR_DEBUG;
	CString Filename = GetDLPPhaseFactorBinFilename(LedClr);
	
	ClearPhaseFactorBufferDebug();
	if ( LoadDLPPhaseFactorFile(Filename, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
	{
		//if ( LoadDLPPhaseFactorFile2(LedClr, Filename, ImageW, ImageH, ImageStep, ImagePtr) == false )
		{	return false;	}
	}
	if ( NULL == ImagePtr )
	{	return true; }
	m_PhaseFactorW = ImageW;
	m_PhaseFactorH = ImageH;
	m_PhaseFactorStep = ImageStep;
	m_PhaseFactorPtr = ImagePtr;
	m_PhaseFactorLEDColor = LedClr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr)//載入DLP平面係數
{
#ifndef PHASE_CTRL_DISABLE
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP4500::LoadDLPPhaseFactorFile";
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

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
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
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadDLPPhaseFactorFile2(int LedClr, LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr)//載入DLP平面係數
{
#ifdef MUST_BACKUP_SYSTEM_FILE_USE
	CString Filename2;
	CString Filename = pfilename;
	CString ShortName=GetDLPPhaseFactorBinShortName(LedClr);		
	Filename2.Format(_T("%s\\%s"), AOIDataCollect.GetSystemBackupFolder(), ShortName);
	if ( JetAPI::IsFileExist(Filename2) == false )
	{	return false; }	
	::DeleteFile(Filename);
	JetMemory.free_func(SpacePtr);
	::CopyFile(Filename2, Filename, FALSE);
	if ( LoadDLPPhaseFactorFile(Filename, SpaceW, SpaceH, SpaceStep, SpacePtr) == false ) 
	{	return false; }
	return true;
#endif//MUST_BACKUP_SYSTEM_FILE_USE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseFactor()//儲存平面係數
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
bool CLight3DTiDLP4500::SavePhaseFactorRed()//儲存平面係數
{
	if ( CheckPhaseFactorRed() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);	
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseFactorGrn()//儲存平面係數
{
	if ( CheckPhaseFactorGrn() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);	
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseFactorBlu()//儲存平面係數
{
	if ( CheckPhaseFactorBlu() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);	
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseFactorWhite()//儲存平面係數
{
	if ( CheckPhaseFactorWhite() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);	
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SavePhaseFactorDebug()//儲存平面係數
{
	if ( CheckPhaseFactorDebug() == false )
	{	return true; }

	CString Filename = GetDLPPhaseFactorBinFilename(m_PhaseFactorLEDColor);	
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CheckDLPPhaseFactorData(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr)//確認DLP平面係數
{
	if ( 0==SpaceW || 0==SpaceH || 0==SpaceStep )
	{
		m_ErrorString.Format(_T("Error, DLP Phase Factor Data Exception(W=%d, H=%d, Step=%d)"), SpaceW, SpaceH, SpaceStep);
		return false;	
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SaveDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr)//儲存DLP平面係數
{
#ifndef PHASE_CTRL_DISABLE
#ifndef OFFLINE_VERSION
	if ( NULL == pfilename ) { return false; }
	if ( CheckDLPPhaseFactorData(SpaceW, SpaceH, SpaceStep, SpacePtr) == false )
	{	return false; }

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
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ClearPhaseFactorBuffer()//清除平面係數
{
	ClearPhaseFactorBufferRed();
	ClearPhaseFactorBufferGrn();
	ClearPhaseFactorBufferBlu();
	ClearPhaseFactorBufferWhite();
	ClearPhaseFactorBufferDebug();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::ClearPhaseFactorBufferRed()//清除平面係數
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
bool CLight3DTiDLP4500::ClearPhaseFactorBufferGrn()//清除平面係數
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
bool CLight3DTiDLP4500::ClearPhaseFactorBufferBlu()//清除平面係數
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
bool CLight3DTiDLP4500::ClearPhaseFactorBufferWhite()//清除平面係數
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
bool CLight3DTiDLP4500::ClearPhaseFactorBufferDebug()//清除平面係數
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
bool CLight3DTiDLP4500::CheckDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep)//確認DLP平面係數檔案
{
#ifndef PHASE_CTRL_DISABLE
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP4500::CheckDLPPhaseFactorFile";
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

	const size_t BufferSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( 0 == BufferSize ) 
	{
		this->m_ErrorString.Format(_T("Error, Read File Fault(%s::%s)"), Filename, _T("BufferSize"));
		::fclose(pFile);	pFile = NULL;
		return false;
	}
	::fclose(pFile);	pFile = NULL;
	
	if ( PhaseW!=TempW || PhaseH!=TempH || PhaseStep!=TempStep )
	{		
		this->m_ErrorString.Format(_T("Error, Check Bin Data Size Fault(%s::W:%d, H:%d, Step:%d)"), Filename, TempW, TempH, TempStep);
		return false;
	}
#endif//PHASE_CTRL_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP4500::GetPhaseFactorMin() const//取得平面係數下限
{
	return m_DLPParam.m_PhaseFactorMin;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPhaseFactorMin(double val)//設定平面係數下限
{
	m_DLPParam.m_PhaseFactorMin = val;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP4500::GetPhaseFactorMax() const//取得平面係數上限
{
	return m_DLPParam.m_PhaseFactorMax;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::SetPhaseFactorMax(double val)//設定平面係數上限
{
	m_DLPParam.m_PhaseFactorMax = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::GetLEDCurrentMax() const//取得LED電流上限
{
	return m_LEDCurrentMax;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetLEDColorUsed_Red() const//取得LED顏色使用-紅色
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetLEDColorUsed_Grn() const//取得LED顏色使用-綠色
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetLEDColorUsed_Blu() const//取得LED顏色使用-藍色
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::ClearHeightFactor()//清除高度參數T0, T1, T3
{
	::memset(m_HeightFactor0, 0x00, sizeof(m_HeightFactor0));
	::memset(m_HeightFactor1, 0x00, sizeof(m_HeightFactor1));
	::memset(m_HeightFactor2, 0x00, sizeof(m_HeightFactor2));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SortHeightFactorTableList()//排序高度係數列表
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
bool CLight3DTiDLP4500::SaveHeightFactorTableList()//儲存高度係數列表檔案
{
#ifndef PHASE_CTRL_DISABLE
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
	const int MaxTargetNo = MAX_HEIGHT_TARGET_COUNT;

	folder = AOIDataCollect.GetPhaseFactorFolder(bTemp);
	::CreateDirectory(folder, NULL);
	for ( i=0; i<MaxTargetNo; i++ )
	{
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp);
		::DeleteFile(filename);
	}
	for ( i=0; i<TableCount; i++ )
	{		
		const TPhaseFactorTable &GridTable = FactorTableList[i];		
		const size_t GridCount = GridTable.GridList.size();
		if ( 0 == GridCount ) { continue; }
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp);
		AOIDataCollect.SavePhaseFactorTableFile(filename, GridTable);
	}	
#endif//OFFLINE_VERSION
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::LoadHeightFactorTableList()//載入高度係數列表檔案
{
#ifndef PHASE_CTRL_DISABLE
	int   i=0;
	CString  filename;	
	int      FileNo=0;
	const bool bTemp = false;	
	const int MaxFileNo = MAX_HEIGHT_TARGET_COUNT;
	LIGHT_3D_CAST_ID CastID=m_Light3DCastID;

	m_FactorTableList.clear();
	for ( i=0; i<MaxFileNo; i++ )
	{
		FileNo = i+1;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, FileNo, bTemp);
		if ( JetAPI::IsFileExist(filename) == false ) { continue; }

		TPhaseFactorTable GridTable;
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable);
		if ( GridTable.GridList.size() == 0 ) { continue; }
		if ( 0 == GridTable.nTargetNo ) { continue; }
		m_FactorTableList.push_back(GridTable);
	}	
	if ( SortHeightFactorTableList() == false )
	{	return false; }
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::RestoreHeightFactorTableList()//復原高度係數列表
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
void CLight3DTiDLP4500::ClearHeightFactorTableList(bool bIncludeFiles)//清除高度係數列表
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
		const int MaxTargetNo = MAX_HEIGHT_TARGET_COUNT;
		for ( i=0; i<MaxTargetNo; i++ )
		{
			TargetNo = i+1;
			filename = AOIDataCollect.GetPhaseFactorFilename(CastID, TargetNo, bTemp);
			::DeleteFile(filename);
		}
	}
#endif//OFFLINE_VERSION
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::BuildHeightFactorMappingParam(bool bRecv)//建立高度參數T0, T1, T3
{
#ifndef PHASE_CTRL_DISABLE
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
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::CalcHeightFactorMappingParam(std::vector<std::vector<TPhaseFactorGrid>> &GridListArray, std::vector<std::vector<double>> &ParamListArray)//計算高度參數T0, T1, T3
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
bool CLight3DTiDLP4500::VerifyHeightFactorMappingParam(const std::vector<std::vector<double>> &ParamListArray, std::vector<TPhaseFactorGrid> &GridList, double &Error)//驗證高度參數T0, T1, T3
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
bool CLight3DTiDLP4500::GetHeightFactorMappingParam(double T0[], double T1[], double T2[])//取得高度參數
{
	::memcpy(T0, m_HeightFactor0, sizeof(m_HeightFactor0));
	::memcpy(T1, m_HeightFactor1, sizeof(m_HeightFactor1));
	::memcpy(T2, m_HeightFactor2, sizeof(m_HeightFactor2));		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr)//設定平面係數
{
	const char fnName[] = "CLight3DTiDLP4500::SetPhaseFactor";	
	if ( NULL == Ptr ) { return false; }

	SPACE_PTR    PhaseFactorPtr = NULL;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageSetp, ImageH);
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
bool CLight3DTiDLP4500::GetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)//取得平面係數
{
	bool IsOK = true;
	bool ReloadFile = false;
	LockLight3D();
	if ( PhaseMode!=m_PhaseFactorPhaseMode || m_PhaseFactorLEDColor!=LEDColor || NULL==m_PhaseFactorPtr )
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
bool CLight3DTiDLP4500::ClonePhaseFactor(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)//複製平面係數
{
	IMAGE_SIZE PhaseFactorW=0;
	IMAGE_SIZE PhaseFactorH=0;
	IMAGE_SIZE PhaseFactorStep=0;
	SPACE_PTR  PhaseFactorPtr=NULL;
	const int PhaseMode = m_PhaseFactorPhaseMode;			
	if ( GetPhaseFactor(PhaseMode, LEDColor, PhaseFactorW, PhaseFactorH, PhaseFactorStep, PhaseFactorPtr) == false )
	{	return false; }

	const char fnName[] = "CLight3DTiDLP4500::ClonePhaseFactor";
	JetMemory.free_func(Ptr);
	const size_t BufferSize = ImageAPI.CalcBufferSize(PhaseFactorStep, PhaseFactorH);
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
bool CLight3DTiDLP4500::SetHeightFactorTable(int TargetNo, const TPhaseFactorTable &GridTable)//設定高度係數格點列表	
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
bool CLight3DTiDLP4500::CloneHeightFactorTable(int TargetNo, TPhaseFactorTable &GridTable) const//複製高度係數格點列表
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
bool CLight3DTiDLP4500::ResetLEDDisable(bool &ResetFinish, bool ShowMsg)
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
				m_ErrorString.Format(_T("The DLP high temperature, can not reset led"));				
				SaveDLPCurrentProcess(m_ErrorString, ShowMsg);	
			}
			return true;
		}
	}
	m_ErrorString.Format(_T("DLP LED Reset"));
	SaveDLPCurrentProcess(m_ErrorString, false);	
	
	if( SetGPIOLEDEnable() == false ) { return false; }
	ResetFinish = true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetGPIOStatus(UINT PinNum, bool &Status)//取得GPIO pin 狀態
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
bool CLight3DTiDLP4500::SetGPIOStatus(UINT PinNum, bool Status)//取得GPIO pin output 狀態
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
bool CLight3DTiDLP4500::GetGPIOTemperatureOver(bool &IsOver, bool ShowMsg)//偵測溫度狀態。		GPIO11 input狀態。high高溫/low低溫
{
	//偵測溫度狀態。	GPIO11 input狀態。high高溫/low低溫			
	UINT pinNum = 11;
	bool Status = false;	//0=LOW; 1=HIGH
	if ( GetGPIOStatus(pinNum, Status) == false )
	{		
		m_ErrorString.Format(_T("Error, Get temperature fault.(GPIO 11)"));
		SaveDLPCurrentProcess(m_ErrorString, ShowMsg);	
		return false;
	}
	if( Status == 0 )
	{	IsOver = false; }
	else
	{ 
		IsOver = true;
		if( true == m_EnableTemperatureMonitor )
		{			
			m_ErrorString.Format(_T("Error, DLP high temperature"));
			SaveDLPCurrentProcess(m_ErrorString, ShowMsg);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::GetGPIOLEDDisable(bool &IsDisable, bool ShowMsg)//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除
{	//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除			
	UINT pinNum = 5;
	bool Status = false;	//0=LOW; 1=HIGH
	if ( GetGPIOStatus(pinNum, Status) == false )
	{
		m_ErrorString.Format(_T("Error, Get LED status fault.(GPIO 5)"));
		SaveDLPCurrentProcess(m_ErrorString, ShowMsg);	
		return false;
	}
	if( false == Status )
	{ IsDisable = false; }
	else
	{ 
		IsDisable = true; 		
		if( true == m_EnableTemperatureMonitor )
		{	
			m_ErrorString.Format(_T("Error, DLP disable"));
			SaveDLPCurrentProcess(m_ErrorString, ShowMsg);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP4500::SetGPIOLEDEnable()//LED disable狀態解除。	GPIO6  output狀態。 low->hi	
{
	//LED disable狀態解除。	GPIO6  output狀態。 low->hi			
	UINT pinNum = 6;
	if( SetGPIOStatus(pinNum, false) == false ) 
	{ 
		this->m_ErrorString.Format(_T("Error, Set GPIO low fault.(GPIO 6)"));
		SaveDLPCurrentProcess(m_ErrorString, true);	
		return false; 
	}
	DLPSettingDelay();
	if( SetGPIOStatus(pinNum, true) == false ) 
	{ 
		this->m_ErrorString.Format(_T("Error, Set GPIO high fault.(GPIO 6)"));
		SaveDLPCurrentProcess(m_ErrorString, true);	
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP4500::DLPSettingDelay()//DLP設定時要先延遲一段時間
{
	if ( m_DLPDelayTime > 0 )
	{	::Sleep(m_DLPDelayTime); }	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP4500::DLPC350_Write(bool ackRequired)
{
    int ret_val;
    hidMessageStruct *pMsg;

    if(ackRequired)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        pMsg = (hidMessageStruct *)g_InputBuffer;
        if((ret_val = m_TiUSB.USB_Write()) > 0)
        {
            //Check for ACK or NACK response
            if(m_TiUSB.USB_Read() > 0)
            {
                if(pMsg->head.flags.nack == 1)
				{
					m_ErrorString.Format(_T("Error, DLPC350_Write Fault"));
					SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_WRITE);
                    return -2;
				}
                else
                    return ret_val;
            }
        }
    }
    else
    {
       ret_val = m_TiUSB.USB_Write();
    }
	if ( ret_val < 0 )
	{
		m_ErrorString.Format(_T("Error, DLPC350_Write Fault"));
		SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_WRITE);	
	}
    return ret_val;
}

int CLight3DTiDLP4500::DLPC350_Read()
/**
 * This function is private to this file. This function is called to write the read control command and then read back 64 bytes over USB
 * to g_InputBuffer.
 *
 * @return  number of bytes read
 *          -2 = nack from target
 *          -1 = error reading
 *
 */
{
    int ret_val;
	unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
    hidMessageStruct *pMsg = (hidMessageStruct *)g_InputBuffer;
    if(m_TiUSB.USB_Write() > 0)
    {
        ret_val =  m_TiUSB.USB_Read();

        if((pMsg->head.flags.nack == 1) || (pMsg->head.length == 0))
		{
			m_ErrorString.Format(_T("Error, DLPC350_Read Fault"));
			SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_READ);
            return -2;
		}
        else
            return ret_val;
    }
	m_ErrorString.Format(_T("Error, DLPC350_Read Fault"));
	SetDLPExceptionCode(AOI_EXCEPTION_DLP_CTRL_READ);
    return -1;
}

int CLight3DTiDLP4500::DLPC350_ContinueRead()
{
    return m_TiUSB.USB_Read();
}

int CLight3DTiDLP4500::DLPC350_SendMsg(hidMessageStruct *pMsg, bool ackRequired)
/**
 * This function is private to this file. This function is called to send a message over USB; in chunks of 64 bytes.
 *
 * @return  number of bytes sent
 *          -1 = FAIL
 *
 */
{
    int maxDataSize = USB_MAX_PACKET_SIZE-sizeof(pMsg->head);
    int dataBytesSent = MIN(pMsg->head.length, maxDataSize);    //Send all data or max possible

    // Default the DLPC350_PrepWriteCmd() update write message for ACK
    // if user not expecting adjust accordingly
    if(!ackRequired)
        pMsg->head.flags.reply = 0;

	unsigned char* g_OutputBuffer = m_TiUSB.GetOutputBuffer();
    g_OutputBuffer[0]=0; // First byte is the report number
    memcpy(&g_OutputBuffer[1], pMsg, (sizeof(pMsg->head) + dataBytesSent));

    //Single packet transaction
    if(dataBytesSent >= pMsg->head.length)
    {
        if(DLPC350_Write(ackRequired) < 0)
            return -1;
    }
    else
    {
        //Send ACK request only for the last packet
        if(DLPC350_Write(0) < 0)
            return -1;

        while(dataBytesSent < pMsg->head.length)
        {
            memcpy(&g_OutputBuffer[1], &pMsg->text.data[dataBytesSent], USB_MAX_PACKET_SIZE);

            if((dataBytesSent+USB_MAX_PACKET_SIZE) >= (pMsg->head.length))
            {
                //last packet request for ACK
                if(DLPC350_Write(ackRequired) < 0)
                    return -1;
            }
            else
            {
                //middle packet
                if(DLPC350_Write(0) < 0)
                    return -1;
            }

            dataBytesSent += USB_MAX_PACKET_SIZE;
        }
    }

    return dataBytesSent+sizeof(pMsg->head);
}

int CLight3DTiDLP4500::DLPC350_PrepReadCmd(DLPC350_CMD cmd)
/**
 * This function is private to this file. Prepares the read-control command packet for the given command code and copies it to g_OutputBuffer.
 *
 * @param   cmd  - I - USB command code.
 *
 * @return  0 = PASS
 *          -1 = FAIL
 *
 */
{
    hidMessageStruct msg;

    msg.head.flags.rw = 1; //Read
    msg.head.flags.reply = 1; //Host wants a reply from device
    msg.head.flags.dest = 0; //Projector Control Endpoint
    msg.head.flags.reserved = 0;
    msg.head.flags.nack = 0;
    msg.head.seq = 0;

    msg.text.cmd = (CmdList[cmd].CMD2 << 8) | CmdList[cmd].CMD3;
    msg.head.length = 2;

    if(cmd == BL_GET_MANID)
    {
        msg.text.data[2] = 0x0C;
        msg.head.length += 1;
    }
    else if (cmd == BL_GET_DEVID)
    {
        msg.text.data[2] = 0x0D;
        msg.head.length += 1;
    }
    else if (cmd == BL_GET_CHKSUM)
    {
        msg.text.data[2] = 0x00;
        msg.head.length += 1;
    }

	unsigned char* g_OutputBuffer = m_TiUSB.GetOutputBuffer();
    g_OutputBuffer[0]=0; // First byte is the report number
    memcpy(&g_OutputBuffer[1], &msg, (sizeof(msg.head)+sizeof(msg.text.cmd) + msg.head.length));
    return 0;
}

int CLight3DTiDLP4500::DLPC350_PrepReadCmdWithParam(DLPC350_CMD cmd, unsigned char param)
/**
 * This function is private to this file. Prepares the read-control command packet for the given command code and parameter and copies it to g_OutputBuffer.
 *
 * @param   cmd  - I - USB command code.
 * @param   param - I - parameter to be used for tis read command.
 *
 * @return  0 = PASS
 *          -1 = FAIL
 *
 */
{
    hidMessageStruct msg;

    msg.head.flags.rw = 1; //Read
    msg.head.flags.reply = 1; //Host wants a reply from device
    msg.head.flags.dest = 0; //Projector Control Endpoint
    msg.head.flags.reserved = 0;
    msg.head.flags.nack = 0;
    msg.head.seq = 0;

    msg.text.cmd = (CmdList[cmd].CMD2 << 8) | CmdList[cmd].CMD3;
    msg.head.length = 3;

    msg.text.data[2] = param;

	unsigned char* g_OutputBuffer = m_TiUSB.GetOutputBuffer();
    g_OutputBuffer[0]=0; // First byte is the report number
    memcpy(&g_OutputBuffer[1], &msg, (sizeof(msg.head)+sizeof(msg.text.cmd) + msg.head.length));
    return 0;
}

int CLight3DTiDLP4500::DLPC350_PrepMemReadCmd(unsigned int addr)
/**
 * This function is private to this file. Prepares the memory read command packet with the given address and copies it to g_OutputBuffer.
 *
 * @param   addr  - I - memory address in controller to be read.
 *
 * @return  0 = PASS
 *          -1 = FAIL
 *
 */
{
    hidMessageStruct msg;

    msg.head.flags.rw = 1; //Read
    msg.head.flags.reply = 1; //Host wants a reply from device
    msg.head.flags.dest = 0; //Projector Control Endpoint
    msg.head.flags.reserved = 0;
    msg.head.flags.nack = 0;
    msg.head.seq = 0;

    msg.text.cmd = (CmdList[MEM_CONTROL].CMD2 << 8) | CmdList[MEM_CONTROL].CMD3;
    msg.head.length = 6;

    msg.text.data[2] = addr;
    msg.text.data[3] = addr >>8;
    msg.text.data[4] = addr >>16;
    msg.text.data[5] = addr >>24;

	unsigned char* g_OutputBuffer = m_TiUSB.GetOutputBuffer();
    g_OutputBuffer[0]=0; // First byte is the report number
    memcpy(&g_OutputBuffer[1], &msg, (sizeof(msg.head)+sizeof(msg.text.cmd) + msg.head.length));
    return 0;
}

int CLight3DTiDLP4500::DLPC350_PrepWriteCmd(hidMessageStruct *pMsg, DLPC350_CMD cmd)
/**
 * This function is private to this file. Prepares the write command packet with given command code in the message structure pointer passed.
 *
 * @param   cmd  - I - USB command code.
 * @param   pMsg - I - Pointer to the message.
 *
 * @return  0 = PASS
 *          -1 = FAIL
 *
 */
{
    pMsg->head.flags.rw = 0; //Write
    pMsg->head.flags.reply = 1; //Host wants a reply from device
    pMsg->head.flags.dest = 0; //Projector Control Endpoint
    pMsg->head.flags.reserved = 0;
    pMsg->head.flags.nack = 0;
    pMsg->head.seq = m_seqNum++;

    pMsg->text.cmd = (CmdList[cmd].CMD2 << 8) | CmdList[cmd].CMD3;
    pMsg->head.length = CmdList[cmd].len + 2;

    return 0;
}

int CLight3DTiDLP4500::DLPC350_GetFirmwareVersion(unsigned int *pFW_ver)
/**
 * This command reads the version information of the DLPC350 firmware.
 *
 * @param   pFW_ver  - O - Firmware Revision BITS 0:15 PATCH NUMBER, BITS 16:23 MINOR REVISION, BIS 24:31 MAJOR REVISION
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
	//Firware revision is loaded at this location in flash in v2.0.0 F/W it is @ 0xF902C000
	//in v3.0.0 F/W it is @ 0xF9093400   
	return DLPC350_MemRead(0xF9093400, pFW_ver); 
}

int CLight3DTiDLP4500::DLPC350_GetVersion(unsigned int *pApp_ver, unsigned int *pAPI_ver, unsigned int *pSWConfig_ver, unsigned int *pSeqConfig_ver)
/**
 * This command reads the version information of the various components of DLPC350 firmware.
 * (I2C: 0x11)
 * (USB: CMD2: 0x02, CMD3: 0x05)
 *
 * @param   pApp_ver  - O - Application Software Revision BITS 0:15 PATCH NUMBER, BITS 16:23 MINOR REVISION, BIS 24:31 MAJOR REVISION
 * @param   pAPI_ver  - O - API Software Revision BITS 0:15 PATCH NUMBER, BITS 16:23 MINOR REVISION, BIS 24:31 MAJOR REVISION
 * @param   pSWConfig_ver  - O - Software Configuration Revision BITS 0:15 PATCH NUMBER, BITS 16:23 MINOR REVISION, BIS 24:31 MAJOR REVISION
 * @param   pSeqConfig_ver  - O - Sequence Configuration Revision BITS 0:15 PATCH NUMBER, BITS 16:23 MINOR REVISION, BIS 24:31 MAJOR REVISION
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(GET_VERSION);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *pApp_ver = *(unsigned int *)&msg.text.data[0];
        *pAPI_ver = *(unsigned int *)&msg.text.data[4];
        *pSWConfig_ver = *(unsigned int *)&msg.text.data[8];
        *pSeqConfig_ver = *(unsigned int *)&msg.text.data[12];
        return 0;
    }
    return -1;
}


int CLight3DTiDLP4500::DLPC350_GetFirmwareTagInfo(unsigned char *pFwTagInfo)
/**
 * This command reads the firmware tag information of the DLPC350 firmware.
 * (I2C: 0x5E)
 * (USB: CMD2: 0x1A, CMD3: 0xFF)
 *
 * @param   pFwTagInfo  - O - Firmware Tag information as string of characters.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    int i = 0;
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(GET_FIRMWAE_TAG_INFO);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        while((msg.text.data[i] != '\0') && (i < 32))
        {
            *pFwTagInfo = msg.text.data[i];
            pFwTagInfo ++;
            i++;
        }

        *pFwTagInfo = '\0';

        return 0;
    }

    return -1;
}

int CLight3DTiDLP4500::DLPC350_GetLedEnables(bool *pSeqCtrl, bool *pRed, bool *pGreen, bool *pBlue)
/**
 * This command reads back the state of LED control method as well as the enabled/disabled status of all LEDs.
 * (I2C: 0x10)
 * (USB: CMD2: 0x1A, CMD3: 0x07)
 *
 * @param   pSeqCtrl  - O - 1 - All LED enables are controlled by the Sequencer and ignore the other LED enable settings.
 *                          0 - All LED enables are controlled by pRed, pGreen and pBlue seetings and ignore Sequencer control
 * @param   pRed  - O - 0 - Red LED is disabled
 *                      1 - Red LED is enabled
 * @param   pGreen  - O - 0 - Green LED is disabled
 *                      1 - Green LED is enabled
 * @param   pBlue  - O - 0 - Blue LED is disabled
 *                      1 - Blue LED is enabled]
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(LED_ENABLE);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        if(msg.text.data[0] & BIT0)
            *pRed = true;
        else
            *pRed = false;

        if(msg.text.data[0] & BIT1)
            *pGreen = true;
        else
            *pGreen = false;

        if(msg.text.data[0] & BIT2)
            *pBlue = true;
        else
            *pBlue = false;

        if(msg.text.data[0] & BIT3)
            *pSeqCtrl = true;
        else
            *pSeqCtrl = false;
        return 0;
    }
    return -1;
}


int CLight3DTiDLP4500::DLPC350_SetLedEnables(bool SeqCtrl, bool Red, bool Green, bool Blue)
/**
 * This command sets the state of LED control method as well as the enabled/disabled status of all LEDs.
 * (I2C: 0x10)
 * (USB: CMD2: 0x1A, CMD3: 0x07)
 *
 * @param   pSeqCtrl  - I - 1 - All LED enables are controlled by the Sequencer and ignore the other LED enable settings.
 *                          0 - All LED enables are controlled by pRed, pGreen and pBlue seetings and ignore Sequencer control
 * @param   pRed  - I - 0 - Red LED is disabled
 *                      1 - Red LED is enabled
 * @param   pGreen  - I - 0 - Green LED is disabled
 *                      1 - Green LED is enabled
 * @param   pBlue  - I - 0 - Blue LED is disabled
 *                      1 - Blue LED is enabled]
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    unsigned char Enable=0;

    if(SeqCtrl)
        Enable |= BIT3;
    if(Red)
        Enable |= BIT0;
    if(Green)
        Enable |= BIT1;
    if(Blue)
        Enable |= BIT2;

    msg.text.data[2] = Enable;
    DLPC350_PrepWriteCmd(&msg, LED_ENABLE);

    return DLPC350_SendMsg(&msg, true);
}

int CLight3DTiDLP4500::DLPC350_GetLedCurrents(unsigned char *pRed, unsigned char *pGreen, unsigned char *pBlue)
/**
 * (I2C: 0x4B)
 * (USB: CMD2: 0x0B, CMD3: 0x01)
 * This parameter controls the pulse duration of the specific LED PWM modulation output pin. The resolution
 * is 8 bits and corresponds to a percentage of the LED current. The PWM value can be set from 0 to 100%
 * in 256 steps . If the LED PWM polarity is set to normal polarity, a setting of 0xFF gives the maximum
 * PWM current. The LED current is a function of the specific LED driver design.
 *
 * @param   pRed  - O - Red LED PWM current control Valid range, assuming normal polarity of PWM signals, is:
 *                      0x00 (0% duty cycle ??Red LED driver generates no current
 *                      0xFF (100% duty cycle ??Red LED driver generates maximum current))
 *                      The current level corresponding to the selected PWM duty cycle is a function of the specific LED driver design and thus varies by design.
 * @param   pGreen  - O - Green LED PWM current control Valid range, assuming normal polarity of PWM signals, is:
 *                      0x00 (0% duty cycle ??Red LED driver generates no current
 *                      0xFF (100% duty cycle ??Red LED driver generates maximum current))
 *                      The current level corresponding to the selected PWM duty cycle is a function of the specific LED driver design and thus varies by design.
 * @param   pBlue  - O - Blue LED PWM current control Valid range, assuming normal polarity of PWM signals, is:
 *                      0x00 (0% duty cycle ??Red LED driver generates no current
 *                      0xFF (100% duty cycle ??Red LED driver generates maximum current))
 *                      The current level corresponding to the selected PWM duty cycle is a function of the specific LED driver design and thus varies by design.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(LED_CURRENT);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *pRed = msg.text.data[0];
        *pGreen = msg.text.data[1];
        *pBlue = msg.text.data[2];

        return 0;
    }
    return -1;
}


int CLight3DTiDLP4500::DLPC350_SetLedCurrents(unsigned char RedCurrent, unsigned char GreenCurrent, unsigned char BlueCurrent)
/**
 * (I2C: 0x4B)
 * (USB: CMD2: 0x0B, CMD3: 0x01)
 * This parameter controls the pulse duration of the specific LED PWM modulation output pin. The resolution
 * is 8 bits and corresponds to a percentage of the LED current. The PWM value can be set from 0 to 100%
 * in 256 steps . If the LED PWM polarity is set to normal polarity, a setting of 0xFF gives the maximum
 * PWM current. The LED current is a function of the specific LED driver design.
 *
 * @param   RedCurrent  - I - Red LED PWM current control Valid range, assuming normal polarity of PWM signals, is:
 *                      0x00 (0% duty cycle ??Red LED driver generates no current
 *                      0xFF (100% duty cycle ??Red LED driver generates maximum current))
 *                      The current level corresponding to the selected PWM duty cycle is a function of the specific LED driver design and thus varies by design.
 * @param   GreenCurrent  - I - Green LED PWM current control Valid range, assuming normal polarity of PWM signals, is:
 *                      0x00 (0% duty cycle ??Red LED driver generates no current
 *                      0xFF (100% duty cycle ??Red LED driver generates maximum current))
 *                      The current level corresponding to the selected PWM duty cycle is a function of the specific LED driver design and thus varies by design.
 * @param   BlueCurrent  - I - Blue LED PWM current control Valid range, assuming normal polarity of PWM signals, is:
 *                      0x00 (0% duty cycle ??Red LED driver generates no current
 *                      0xFF (100% duty cycle ??Red LED driver generates maximum current))
 *                      The current level corresponding to the selected PWM duty cycle is a function of the specific LED driver design and thus varies by design.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = RedCurrent;
    msg.text.data[3] = GreenCurrent;
    msg.text.data[4] = BlueCurrent;

    DLPC350_PrepWriteCmd(&msg, LED_CURRENT);

    return DLPC350_SendMsg(&msg, true);
}

bool CLight3DTiDLP4500::DLPC350_GetLongAxisImageFlip(void)
/**
 * (I2C: 0x08)
 * (USB: CMD2: 0x10, CMD3: 0x08)
 * The Long-Axis Image Flip defines whether the input image is flipped across the long axis of the DMD. If
 * this parameter is changed while displaying a still image, the input still image should be re-sent. If the
 * image is not re-sent, the output image might be slightly corrupted. In Structured Light mode, the image
 * flip will take effect on the next bit-plane, image, or video frame load.
 *
 * @return  TRUE = Image flipped along long axis    <BR>
 *          FALSE = Image not flipped  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(FLIP_LONG);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        if ((msg.text.data[0] & BIT0) == BIT0)
            return true;
        else
            return false;
    }
    return false;
}

bool CLight3DTiDLP4500::DLPC350_GetShortAxisImageFlip(void)
/**
 * (I2C: 0x09)
 * (USB: CMD2: 0x10, CMD3: 0x09)
 * The Short-Axis Image Flip defines whether the input image is flipped across the short axis of the DMD. If
 * this parameter is changed while displaying a still image, the input still image should be re-sent. If the
 * image is not re-sent, the output image might be slightly corrupted. In Structured Light mode, the image
 * flip will take effect on the next bit-plane, image, or video frame load.
 *
 * @return  TRUE = Image flipped along short axis    <BR>
 *          FALSE = Image not flipped  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(FLIP_SHORT);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        if ((msg.text.data[0] & BIT0) == BIT0)
            return true;
        else
            return false;
    }
    return false;
}


int CLight3DTiDLP4500::DLPC350_SetFreeze(bool Freeze)
/**
 * (I2C: 0x7C)
 * (USB: CMD2: 0x10, CMD3: 0x0A)
 * The freeze command disables swapping the memory buffers. When frozen the last
 * image streamed to the DMD continuese to be displayed.
 * @param   Freeze -I TRUE = Freeze display buffer <BR>
 *                  FALSE = No freeze. Display buffer swap keeps happening <BR>
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    if(Freeze)
        msg.text.data[2] = BIT0;
    else
        msg.text.data[2] = 0;

    DLPC350_PrepWriteCmd(&msg, BUFFER_FREEZE);

    return DLPC350_SendMsg(&msg, true);
}

int CLight3DTiDLP4500::DLPC350_SetLongAxisImageFlip(bool Flip)
/**
 * (I2C: 0x08)
 * (USB: CMD2: 0x10, CMD3: 0x08)
 * The Long-Axis Image Flip defines whether the input image is flipped across the long axis of the DMD. If
 * this parameter is changed while displaying a still image, the input still image should be re-sent. If the
 * image is not re-sent, the output image might be slightly corrupted. In Structured Light mode, the image
 * flip will take effect on the next bit-plane, image, or video frame load.
 *
 * @param   Flip -I TRUE = Image flipped along long axis enable    <BR>
 *                  FALSE = Do not flip image <BR>
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    if(Flip)
        msg.text.data[2] = BIT0;
    else
        msg.text.data[2] = 0;

    DLPC350_PrepWriteCmd(&msg, FLIP_LONG);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_SetShortAxisImageFlip(bool Flip)
/**
 * (I2C: 0x09)
 * (USB: CMD2: 0x10, CMD3: 0x09)
 * The Long-Axis Image Flip defines whether the input image is flipped across the long axis of the DMD. If
 * this parameter is changed while displaying a still image, the input still image should be re-sent. If the
 * image is not re-sent, the output image might be slightly corrupted. In Structured Light mode, the image
 * flip will take effect on the next bit-plane, image, or video frame load.
 *
 * @param   Flip -I TRUE = Image flipped along long axis enable    <BR>
 *                  FALSE = Do not flip image <BR>
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    if(Flip)
        msg.text.data[2] = BIT0;
    else
        msg.text.data[2] = 0;

    DLPC350_PrepWriteCmd(&msg, FLIP_SHORT);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_EnterProgrammingMode()
/**
 * This function is to be called to put the unit in programming mode. Only programming mode APIs will work once
 * in this mode.
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = 1;

    DLPC350_PrepWriteCmd(&msg, PROG_MODE);

    return DLPC350_SendMsg(&msg,false);
}

int CLight3DTiDLP4500::DLPC350_ExitProgrammingMode(void)
/**
 * This function works only in prorgamming mode.
 * This function is to be called to exit programming mode and resume normal operation with the new downloaded firmware.
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = 2;
    DLPC350_PrepWriteCmd(&msg, BL_PROG_MODE);

    return DLPC350_SendMsg(&msg,false);
}

int CLight3DTiDLP4500::DLPC350_GetFlashManID(unsigned short *pManID)
/**
 * This function works only in prorgamming mode.
 * This function returns the manufacturer ID of the flash part interfaced with the controller.
 *
 * @param pManID - O - Manufacturer ID of the flash part
 *
 * @return 0 PASS <BR>
 *         -1 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(BL_GET_MANID);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *pManID = msg.text.data[6];
        *pManID |= (unsigned short)msg.text.data[7] << 8;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_GetFlashDevID(unsigned long long *pDevID)
/**
 * This function works only in prorgamming mode.
 * This function returns the device ID of the flash part interfaced with the controller.
 *
 * @param pDevID - O - Device ID of the flash part
 *
 * @return 0 PASS <BR>
 *         -1 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(BL_GET_DEVID);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *pDevID = msg.text.data[6];
        *pDevID |= (unsigned long long)msg.text.data[7] << 8;
        *pDevID |= (unsigned long long)msg.text.data[8] << 16;
        *pDevID |= (unsigned long long)msg.text.data[9] << 24;
        *pDevID |= (unsigned long long)msg.text.data[12] << 32;
        *pDevID |= (unsigned long long)msg.text.data[13] << 40;
        *pDevID |= (unsigned long long)msg.text.data[14] << 48;
        *pDevID |= (unsigned long long)msg.text.data[15] << 56;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_GetBLStatus(unsigned char *BL_Status)
/**
 * This function works only in prorgamming mode.
 * This function returns the device ID of the flash part interfaced with the controller.
 *
 * @param BL_Status - O - BIT3 of the status byte when set indicates that the program is busy
 *                        with exectuing the previous command. When BIT3 is reset, it means the
 *                        program is ready for the next command.
 *
 * @return 0 PASS <BR>
 *         -1 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    /* For some reason BL_STATUS readback is not working properly.
     * However, after going through the bootloader code, I have ascertained that any
     * readback is fine - Byte 0 is always the bootloader status */
    DLPC350_PrepReadCmd(BL_GET_CHKSUM);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *BL_Status = msg.text.data[0];
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetFlashAddr(unsigned int Addr)
/**
 * This function works only in prorgamming mode.
 * This function is to be called to set the address prior to calling DLPC350_FlashSectorErase or DLPC350_DownloadData APIs.
 *
 * @param Addr - I - 32-bit absolute address.
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = Addr;
    msg.text.data[3] = Addr >> 8;
    msg.text.data[4] = Addr >> 16;
    msg.text.data[5] = Addr >> 24;

    DLPC350_PrepWriteCmd(&msg, BL_SET_SECTADDR);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_FlashSectorErase(void)
/**
  * This function works only in prorgamming mode.
  * This function is to be called to erase a sector of flash. The address of the sector to be erased
  * is to be set by using the DLPC350_SetFlashAddr() API
  *
  * @return >=0 PASS <BR>
  *         <0 FAIL <BR>
  *
  */
{
    hidMessageStruct msg;

    DLPC350_PrepWriteCmd(&msg, BL_SECT_ERASE);
    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_SetUploadSize(unsigned long int dataLen)
/**
 * This function works only in prorgamming mode.
 * This function is to be called to set the payload size of data to be sent using DLPC350_DownloadData API.
 *
 * @param dataLen -I - length of download data payload in bytes.
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = dataLen;
    msg.text.data[3] = dataLen >> 8;
    msg.text.data[4] = dataLen >> 16;
    msg.text.data[5] = dataLen >> 24;

    DLPC350_PrepWriteCmd(&msg, BL_SET_DNLDSIZE);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_UploadData(unsigned char *pByteArray, unsigned int dataLen)
/**
 * This function works only in prorgamming mode.
 * This function sends one payload of data to the controller at a time. takes the total size of payload
 * in the parameter dataLen and returns the actual number of bytes that was sent in the return value.
 * This function needs to be called multiple times until all of the desired bytes are sent.
 *
 * @param pByteArray - I - Pointer to where the data to be downloaded is to be fetched from
 * @param dataLen -I - length in bytes of the total payload data to download.
 *
 * @return number of bytes actually downloaded <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;
    int retval;
    unsigned int sendSize;

    //The last -2 is to workaround a bug in bootloader.
    sendSize = HID_MESSAGE_MAX_SIZE - sizeof(msg.head)- sizeof(msg.text.cmd) - 2;

    if(dataLen > sendSize)
        dataLen = sendSize;

    CmdList[BL_DNLD_DATA].len = dataLen;
    memcpy(&msg.text.data[2], pByteArray, dataLen);

    DLPC350_PrepWriteCmd(&msg, BL_DNLD_DATA);

    retval = DLPC350_SendMsg(&msg,false);
    if(retval > 0)
        return dataLen;

    return -1;
}

void CLight3DTiDLP4500::DLPC350_WaitForFlashReady()
/**
 * This function works only in prorgamming mode.
 * This function polls the status bit and returns only when the controller is ready for next command.
 *
 */
{
    unsigned char BLstatus=STAT_BIT_FLASH_BUSY;

    do
    {
        DLPC350_GetBLStatus(&BLstatus);
    }
    while((BLstatus & STAT_BIT_FLASH_BUSY) == STAT_BIT_FLASH_BUSY);//Wait for flash busy flag to go off
}

int CLight3DTiDLP4500::DLPC350_SetFlashType(unsigned char Type)
/**
 * This function works only in prorgamming mode.
 * This function is to be used to set the programming type of the flash device attached to the controller.
 *
 * @param Type - I - Type of the flash device.
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = Type;

    DLPC350_PrepWriteCmd(&msg, BL_FLASH_TYPE);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_CalculateFlashChecksum(void)
/**
 * This function works only in prorgamming mode.
 * This function is to be issued to instruct the controller to calculate the flash checksum.
 * DLPC350_WaitForFlashReady() is then to be called to ensure that the controller is done and then call
 * DLPC350_GetFlashChecksum() API to retrieve the actual checksum from the controller.
 *
 * @return 0 = PASS <BR>
 *         -1 = FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepWriteCmd(&msg, BL_CALC_CHKSUM);

    if(DLPC350_SendMsg(&msg,false) <= 0)
        return -1;

    return 0;

}

int CLight3DTiDLP4500::DLPC350_GetFlashChecksum(unsigned int*checksum)
/**
 * This function works only in prorgamming mode.
 * This function is to be used to retrieve the flash checksum from the controller.
 * DLPC350_CalculateFlashChecksum() and DLPC350_WaitForFlashReady() must be called before using this API.
 *
 * @param checksum - O - variable in which the flash checksum is to be returned
 *
 * @return >=0 PASS <BR>
 *         <0 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;
#if 0
    DLPC350_PrepWriteCmd(&msg, BL_CALC_CHKSUM);

    if(DLPC350_SendMsg(&msg,true) <= 0)
        return -1;

    DLPC350_WaitForFlashReady();
#endif
    DLPC350_PrepReadCmd(BL_GET_CHKSUM);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *checksum = msg.text.data[6];
        *checksum |= (unsigned int)msg.text.data[7] << 8;
        *checksum |= (unsigned int)msg.text.data[8] << 16;
        *checksum |= (unsigned int)msg.text.data[9] << 24;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_GetStatus(unsigned char *pHWStatus, unsigned char *pSysStatus, unsigned char *pMainStatus)
/**
 * This function is to be used to check the various status indicators from the controller.
 * Refer to DLPC350 Programmer's guide section 2.1 "DLPC350 Status Commands" for detailed description of each byte.
 *
 * @param pHWStatus - O - provides status information on the DLPC350's sequencer, DMD controller and initialization.
 * @param pSysStatus - O - provides DLPC350 status on internal memory tests..
 * @param pMainStatus - O - provides DMD park status and DLPC350 sequencer, frame buffer, and gamma correction status.
 *
 * @return 0 PASS <BR>
 *         -1 FAIL <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(STATUS_HW);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *pHWStatus = msg.text.data[0];
    }
    else
        return -1;

    DLPC350_PrepReadCmd(STATUS_SYS);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *pSysStatus = msg.text.data[0];
    }
    else
        return -1;

    DLPC350_PrepReadCmd(STATUS_MAIN);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        *pMainStatus = msg.text.data[0];
    }
    else
        return -1;

    return 0;
}

int CLight3DTiDLP4500::DLPC350_SoftwareReset(void)
/**
 * Use this API to reset the controller
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepWriteCmd(&msg, SW_RESET);

    return DLPC350_SendMsg(&msg,false);
}

int CLight3DTiDLP4500::DLPC350_SetMode(bool SLmode)
/**
 * The Display Mode Selection Command enables the DLPC350 internal image processing functions for
 * video mode or bypasses them for pattern display mode. This command selects between video or pattern
 * display mode of operation.
 *
 * @param   SLmode  - I - TRUE = Pattern Display mode. Assumes a 1-bit through 8-bit image with a pixel
 *                              resolution of 912 x 1140 and bypasses all the image processing functions of DLPC350
 *                          FALSE = Video Display mode. Assumes streaming video image from the 30-bit
 *                              RGB or FPD-link interface with a pixel resolution of up to 1280 x 800 up to 120 Hz.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = SLmode;
    DLPC350_PrepWriteCmd(&msg, DISP_MODE);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetMode(bool *pMode)
/**
 * The Display Mode Selection Command enables the DLPC350 internal image processing functions for
 * video mode or bypasses them for pattern display mode. This command selects between video or pattern
 * display mode of operation.
 *
 * @param   SLmode  - O - TRUE = Pattern Display mode. Assumes a 1-bit through 8-bit image with a pixel
 *                              resolution of 912 x 1140 and bypasses all the image processing functions of DLPC350
 *                        FALSE = Video Display mode. Assumes streaming video image from the 30-bit
 *                              RGB or FPD-link interface with a pixel resolution of up to 1280 x 800 up to 120 Hz.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(DISP_MODE);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pMode = (msg.text.data[0] != 0);
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetPowerMode(bool Standby)
/**
 * (I2C: 0x07)
 * (USB: CMD2: 0x02, CMD3: 0x00)
 * The Power Control places the DLPC350 in a low-power state and powers down the DMD interface.
 * Standby mode should only be enabled after all data for the last frame to be displayed has been
 * transferred to the DLPC350. Standby mode must be disabled prior to sending any new data.
 *
 * @param   Standby  - I - TRUE = Standby mode. Places DLPC350 in low power state and powers down the DMD interface
 *                         FALSE = Normal operation. The selected external source will be displayed
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = Standby;
    DLPC350_PrepWriteCmd(&msg, POWER_CONTROL);

    return DLPC350_SendMsg(&msg,true);
}


int CLight3DTiDLP4500::DLPC350_GetPowerMode(bool *Standby)
/**
 * (I2C: 0x07)
 * (USB: CMD2: 0x02, CMD3: 0x00)
 * The Power Control places the DLPC350 in a low-power state and powers down the DMD interface.
 * Standby mode should only be enabled after all data for the last frame to be displayed has been
 * transferred to the DLPC350. Standby mode must be disabled prior to sending any new data.
 *
 * @param   Standby  - 0 - TRUE = Standby mode. Places DLPC350 in low power state and powers down the DMD interface
 *                         FALSE = Normal operation. The selected external source will be displayed
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(POWER_CONTROL);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        //bit1:0 - show Power On/Standby state
        if(msg.text.data[0] & 0x03)
        {
            *Standby = true;
        }
        else
        {
            *Standby = false;
        }
    }
    else
    {
        return -1;
    }

    return 0;
}


int CLight3DTiDLP4500::DLPC350_SetRedLEDStrobeDelay(unsigned char rising, unsigned char falling)
/**
 * (I2C: 0x6C)
 * (USB: CMD2: 0x1A, CMD3: 0x1F)
 * The Red LED Enable Delay Control command sets the rising and falling edge delay of the Red LED enable signal.
 *
 * @param   rising  - I - Red LED enable rising edge delay control. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 * @param   falling  - I - Red LED enable falling edge delay control. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = rising;
    msg.text.data[3] = falling;

    DLPC350_PrepWriteCmd(&msg, RED_STROBE_DLY);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_SetGreenLEDStrobeDelay(unsigned char rising, unsigned char falling)
/**
 * (I2C: 0x6D)
 * (USB: CMD2: 0x1A, CMD3: 0x20)
 * The Green LED Enable Delay Control command sets the rising and falling edge delay of the Green LED enable signal.
 *
 * @param   rising  - I - Green LED enable rising edge delay control. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 * @param   falling  - I - Green LED enable falling edge delay control. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = rising;
    msg.text.data[3] = falling;

    DLPC350_PrepWriteCmd(&msg, GRN_STROBE_DLY);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_SetBlueLEDStrobeDelay(unsigned char rising, unsigned char falling)
/**
 * (I2C: 0x6E)
 * (USB: CMD2: 0x1A, CMD3: 0x21)
 * The Blue LED Enable Delay Control command sets the rising and falling edge delay of the Blue LED enable signal.
 *
 * @param   rising  - I - Blue LED enable rising edge delay control. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 * @param   falling  - I - Blue LED enable falling edge delay control. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = rising;
    msg.text.data[3] = falling;

    DLPC350_PrepWriteCmd(&msg, BLU_STROBE_DLY);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetRedLEDStrobeDelay(unsigned char *pRising, unsigned char *pFalling)
/**
 * (I2C: 0x6C)
 * (USB: CMD2: 0x1A, CMD3: 0x1F)
 * This command reads back the rising and falling edge delay of the Red LED enable signal.
 *
 * @param   pRising  - O - Red LED enable rising edge delay value. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 * @param   pFalling  - O - Red LED enable falling edge delay value. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(RED_STROBE_DLY);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pRising = msg.text.data[0];
        *pFalling = msg.text.data[1];
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_GetGreenLEDStrobeDelay(unsigned char *pRising, unsigned char *pFalling)
/**
 * (I2C: 0x6D)
 * (USB: CMD2: 0x1A, CMD3: 0x20)
 * This command reads back the rising and falling edge delay of the Green LED enable signal.
 *
 * @param   pRising  - O - Green LED enable rising edge delay value. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 * @param   pFalling  - O - Green LED enable falling edge delay value. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(GRN_STROBE_DLY);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pRising = msg.text.data[0];
        *pFalling = msg.text.data[1];
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_GetBlueLEDStrobeDelay(unsigned char *pRising, unsigned char *pFalling)
/**
 * (I2C: 0x6E)
 * (USB: CMD2: 0x1A, CMD3: 0x21)
 * This command reads back the rising and falling edge delay of the Blue LED enable signal.
 *
 * @param   pRising  - O - Blue LED enable rising edge delay value. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 * @param   pFalling  - O - Blue LED enable falling edge delay value. Each bit adds 107.2 ns.
 *                        0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xFE = +7.1828 弮s, 0xFF = +7.29 弮s
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(BLU_STROBE_DLY);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pRising = msg.text.data[0];
        *pFalling = msg.text.data[1];
        return 0;
    }
    return -1;
}


int  CLight3DTiDLP4500::DLPC350_GetVideoSignalStatus(VideoSigStatus *pVidSigStat)
/**
 * (I2C: 0x01)
 * (USB: CMD2: 0x07, CMD3: 0x1C)
 * This command reads back the incoming video signal timing infomration.
 *
 * @param *pVidSigStat - O - The VideoSigStatus structure contains the following
 *          parameters to describe the area to be cropped:
 *              - Status <BR>
 *              - HRes <BR>
 *              - VRes <BR>
 *              - RSVD <BR>
 *              - HSyncPol <BR>
 *              - VSyncPol <BR>
 *              - PixClock <BR>
 *              - HFreq <BR>
 *              - VFreq <BR>
 *              - TotPixPerLine <BR>
 *              - TotLinPerFrame <BR>
 *              - ActvPixPerLine <BR>
 *              - ActvLinePerFrame <BR>
 *              - FirstActvPix <BR>
 *              - FirstActvLine <BR>
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(VID_SIG_STAT);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);

        //Copy data into structure
        pVidSigStat->Status = (msg.text.data[0] & 0x03);
        pVidSigStat->HRes = (unsigned int)((msg.text.data[2] << 8) | (msg.text.data[1]));
        pVidSigStat->VRes = (unsigned int)((msg.text.data[4] << 8) | (msg.text.data[3]));
        pVidSigStat->RSVD = (msg.text.data[5]);
        pVidSigStat->HSyncPol = (msg.text.data[6]);
        pVidSigStat->VSyncPol = (msg.text.data[7]);
        pVidSigStat->PixClock = (unsigned long int)((msg.text.data[11] << 24) | (msg.text.data[10] << 16) | (msg.text.data[9] << 8)  | (msg.text.data[8]));
        pVidSigStat->HFreq = (unsigned int)((msg.text.data[13] << 8) | (msg.text.data[12]));
        pVidSigStat->VFreq = (unsigned int)((msg.text.data[15] << 8) | (msg.text.data[14]));
        pVidSigStat->TotPixPerLine = (unsigned int)((msg.text.data[17] << 8) | (msg.text.data[16]));
        pVidSigStat->TotLinPerFrame = (unsigned int)((msg.text.data[19] << 8) | (msg.text.data[18]));
        pVidSigStat->ActvPixPerLine = (unsigned int)((msg.text.data[21] << 8) | (msg.text.data[20]));
        pVidSigStat->ActvLinePerFrame = (unsigned int)((msg.text.data[23] << 8) | (msg.text.data[22]));
        pVidSigStat->FirstActvPix = (unsigned int)((msg.text.data[25] << 8) | (msg.text.data[24]));
        pVidSigStat->FirstActvLine = (unsigned int)((msg.text.data[27] << 8) | (msg.text.data[26]));
        return 0;
    }

    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetInputSource(unsigned int source, unsigned int portWidth)
/**
 * (I2C: 0x00)
 * (USB: CMD2: 0x1A, CMD3: 0x00)
 * The Input Source Selection command selects the input source to be displayed by the DLPC350: 30-bit
 * Parallel Port, Internal Test Pattern, Flash memory, or FPD-link interface.
 *
 * @param   source  - I - Select the input source and interface mode:
 *                        0 = Parallel interface with 8-bit, 16-bit, 20-bit, 24-bit, or 30-bit RGB or YCrCb data formats
 *                        1 = Internal test pattern; Use DLPC350_SetTPGSelect() API to select pattern
 *                        2 = Flash. Images are 24-bit single-frame, still images stored in flash that are uploaded on command.
 *                        3 = FPD-link interface
 * @param   portWidth  - I - Parallel Interface bit depth
 *                           0 = 30-bits
 *                           1 = 24-bits
 *                           2 = 20-bits
 *                           3 = 16-bits
 *                           4 = 10-bits
 *                           5 = 8-bits
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = source;
    msg.text.data[2] |= portWidth << 3;
    DLPC350_PrepWriteCmd(&msg, SOURCE_SEL);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetInputSource(unsigned int *pSource, unsigned int *pPortWidth)
/**
 * (I2C: 0x00)
 * (USB: CMD2: 0x1A, CMD3: 0x00)
 * Thisn command reads back the input source to be displayed by the DLPC350
 *
 * @param   pSource  - O - Input source and interface mode:
 *                        0 = Parallel interface with 8-bit, 16-bit, 20-bit, 24-bit, or 30-bit RGB or YCrCb data formats
 *                        1 = Internal test pattern; Use DLPC350_SetTPGSelect() API to select pattern
 *                        2 = Flash. Images are 24-bit single-frame, still images stored in flash that are uploaded on command.
 *                        3 = FPD-link interface
 * @param   pPortWidth  - O - Parallel Interface bit depth
 *                           0 = 30-bits
 *                           1 = 24-bits
 *                           2 = 20-bits
 *                           3 = 16-bits
 *                           4 = 10-bits
 *                           5 = 8-bits
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(SOURCE_SEL);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pSource = msg.text.data[0] & (BIT0 | BIT1 | BIT2);
        *pPortWidth = msg.text.data[0] >> 3;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetPatternDisplayMode(bool external)
/**
 * The Pattern Display Data Input Source command selects the source of the data for pattern display:
 * streaming through the 24-bit RGB/FPD-link interface or stored data in the flash image memory area from
 * external Flash. Before executing this command, stop the current pattern sequence. After executing this
 * command, send the Validation command (I2C: 0x7D or USB: 0x1A1A) once before starting the pattern
 * sequence.
 *
 * @param   external  - I - TRUE = Pattern Display Data is streamed through the 24-bit RGB/FPD-link interface
 *                          FALSE = Pattern Display Data is fetched from flash memory
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    if(external)
        msg.text.data[2] = 0;
    else
        msg.text.data[2] = 3;

    DLPC350_PrepWriteCmd(&msg, PAT_DISP_MODE);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetPatternDisplayMode(bool *external)
/**
 * The Pattern Display Data Input Source command selects the source of the data for pattern display:
 * streaming through the 24-bit RGB/FPD-link interface or stored data in the flash image memory area from
 * external Flash. Before executing this command, stop the current pattern sequence. After executing this
 * command, send the Validation command (I2C: 0x7D or USB: 0x1A1A) once before starting the pattern
 * sequence.
 *
 * @param   external  - O - TRUE = Pattern Display Data is streamed through the 24-bit RGB/FPD-link interface
 *                          FALSE = Pattern Display Data is fetched from flash memory
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(PAT_DISP_MODE);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        if(msg.text.data[0] == 0)
            *external = true;
        else
            *external = false;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetPixelFormat(unsigned int format)
/**
 * (I2C: 0x02)
 * (USB: CMD2: 0x1A, CMD3: 0x02)
 * This API defines the pixel data format input into the DLPC350.Refer to programmer's guide for supported pixel formats
 * for each source type.
 *
 * @param   format  - I - Select the pixel data format:
 *                        0 = RGB 4:4:4 (30-bit)
 *                        1 = YCrCb 4:4:4 (30-bit)
 *                        2 = YCrCb 4:2:2
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = format;
    DLPC350_PrepWriteCmd(&msg, PIXEL_FORMAT);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetPixelFormat(unsigned int *pFormat)
/**
 * (I2C: 0x02)
 * (USB: CMD2: 0x1A, CMD3: 0x02)
 * This API returns the defined the pixel data format input into the DLPC350.Refer to programmer's guide for supported pixel formats
 * for each source type.
 *
 * @param   pFormat  - O - Pixel data format:
 *                        0 = RGB 4:4:4 (30-bit)
 *                        1 = YCrCb 4:4:4 (30-bit)
 *                        2 = YCrCb 4:2:2
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(PIXEL_FORMAT);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pFormat = msg.text.data[0] & (BIT0 | BIT1 | BIT2);
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetPortClock(unsigned int clock)
/**
 * (I2C: 0x03)
 * (USB: CMD2: 0x1A, CMD3: 0x03)
 * This API selects the Port 1 clock for the parallel interface. For the FPD-Link, the Port Clock is
 * automatically set to Port 2.
 *
 * @param   clock  - I - Selects the port input clock:
 *                        0 = Port1, clock A
 *                        1 = Port1, clock B
 *                        2 = Port1, clock C
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = clock;
    DLPC350_PrepWriteCmd(&msg, CLK_SEL);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetPortClock(unsigned int *pClock)
/**
 * (I2C: 0x03)
 * (USB: CMD2: 0x1A, CMD3: 0x03)
 * This API reads the Port 1 clock for the parallel interface.
 *
 * @param   pClock  - O - Selected port input clock:
 *                        0 = Port1, clock A
 *                        1 = Port1, clock B
 *                        2 = Port1, clock C
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(CLK_SEL);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pClock = msg.text.data[0] & (BIT0 | BIT1 | BIT2);
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetDataChannelSwap(unsigned int port, unsigned int swap)
/**
 * (I2C: 0x04)
 * (USB: CMD2: 0x1A, CMD3: 0x37)
 * This API configures the specified input data port and map the data subchannels.
 * The DLPC350 interprets Channel A as Green, Channel B as Red, and Channel C as Blue.
 *
 * @param   port  - I - Selects the port:
 *                        0 = Port1, parallel interface
 *                        1 = Port2, FPD-link interface
 * @param   swap - I - Swap Data Sub-Channel:
 *                     0 - ABC = ABC, No swapping of data sub-channels
 *                     1 - ABC = CAB, Data sub-channels are right shifted and circularly rotated
 *                     2 - ABC = BCA, Data sub-channels are left shifted and circularly rotated
 *                     3 - ABC = ACB, Data sub-channels B and C are swapped
 *                     4 - ABC = BAC, Data sub-channels A and B are swapped
 *                     5 - ABC = CBA, Data sub-channels A and C are swapped
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = port << 7;
    msg.text.data[2] |= swap & (BIT0 | BIT1 | BIT2);
    DLPC350_PrepWriteCmd(&msg, CHANNEL_SWAP);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetDataChannelSwap(unsigned int *pPort, unsigned int *pSwap)
/**
 * (I2C: 0x04)
 * (USB: CMD2: 0x1A, CMD3: 0x37)
 * This API reads the data subchannel mapping for the specified input data port and map the data subchannels
 *
 * @param   *pPort  - O - Selected port:
 *                        0 = Port1, parallel interface
 *                        1 = Port2, FPD-link interface
 * @param   *pSwap - O - Swap Data Sub-Channel:
 *                     0 - ABC = ABC, No swapping of data sub-channels
 *                     1 - ABC = CAB, Data sub-channels are right shifted and circularly rotated
 *                     2 - ABC = BCA, Data sub-channels are left shifted and circularly rotated
 *                     3 - ABC = ACB, Data sub-channels B and C are swapped
 *                     4 - ABC = BAC, Data sub-channels A and B are swapped
 *                     5 - ABC = CBA, Data sub-channels A and C are swapped
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(CHANNEL_SWAP);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pSwap = msg.text.data[0] & (BIT0 | BIT1 | BIT2);
        if(msg.text.data[0] & BIT7)
            *pPort = 1;
        else
            *pPort = 0;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetFPD_Mode_Field(unsigned int PixelMappingMode, bool SwapPolarity, unsigned int FieldSignalSelect)
/**
 * (I2C: 0x05)
 * (USB: CMD2: 0x1A, CMD3: 0x04)
 * The FPD-Link Mode and Field Select command configures the FPD-link pixel map, polarity, and signal select.
 *
 * @param   PixelMappingMode  - I - FPD-link Pixel Mapping Mode: See table 2-21 in programmer's guide for more details
 *                                  0 = Mode 1
 *                                  1 = Mode 2
 *                                  2 = Mode 3
 *                                  3 = Mode 4
 * @param   SwapPolarity - I - Polarity select
 *                             true = swap polarity
 *                             false = do not swap polarity
 *
 * @param   FieldSignalSelect -I -  Field Signal Select
 *                              0 - Map FPD-Link output from CONT1 onto Field Signal for FPD-link interface port
 *                              1 - Map FPD-Link output from CONT2 onto Field Signal for FPD-link interface port
 *                              2 - Force 0 onto Field Signal for FPD-link interface port
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = PixelMappingMode << 6;
    msg.text.data[2] |= FieldSignalSelect & (BIT0 | BIT1 | BIT2);
    if(SwapPolarity)
        msg.text.data[2] |= BIT3;
    DLPC350_PrepWriteCmd(&msg, FPD_MODE);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetFPD_Mode_Field(unsigned int *pPixelMappingMode, bool *pSwapPolarity, unsigned int *pFieldSignalSelect)
/**
 * (I2C: 0x05)
 * (USB: CMD2: 0x1A, CMD3: 0x04)
 * This command reads back the configuration of FPD-link pixel map, polarity, and signal select.
 *
 * @param   PixelMappingMode  - O - FPD-link Pixel Mapping Mode: See table 2-21 in programmer's guide for more details
 *                                  0 = Mode 1
 *                                  1 = Mode 2
 *                                  2 = Mode 3
 *                                  3 = Mode 4
 * @param   SwapPolarity - O - Polarity select
 *                             true = swap polarity
 *                             false = do not swap polarity
 *
 * @param   FieldSignalSelect - O -  Field Signal Select
 *                              0 - Map FPD-Link output from CONT1 onto Field Signal for FPD-link interface port
 *                              1 - Map FPD-Link output from CONT2 onto Field Signal for FPD-link interface port
 *                              2 - Force 0 onto Field Signal for FPD-link interface port
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(FPD_MODE);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pFieldSignalSelect = msg.text.data[0] & (BIT0 | BIT1 | BIT2);
        if(msg.text.data[0] & BIT3)
            *pSwapPolarity = 1;
        else
            *pSwapPolarity = 0;
        *pPixelMappingMode = msg.text.data[0] >> 6;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetTPGSelect(unsigned int pattern)
/**
 * (I2C: 0x0A)
 * (USB: CMD2: 0x12, CMD3: 0x03)
 * When the internal test pattern is the selected input, the Internal Test Patterns Select defines the test
 * pattern displayed on the screen. These test patterns are internally generated and injected into the
 * beginning of the DLPC350 image processing path. Therefore, all image processing is performed on the
 * test images. All command registers should be set up as if the test images are input from an RGB 8:8:8
 * external source.
 *
 * @param   pattern  - I - Selects the internal test pattern:
 *                         0x0 = Solid Field
 *                         0x1 = Horizontal Ramp
 *                         0x2 = Vertical Ramp
 *                         0x3 = Horizontal Lines
 *                         0x4 = Diagonal Lines
 *                         0x5 = Vertical Lines
 *                         0x6 = Grid
 *                         0x7 = Checkerboard
 *                         0x8 = RGB Ramp
 *                         0x9 = Color Bars
 *                         0xA = Step Bars
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = pattern;
    DLPC350_PrepWriteCmd(&msg, TPG_SEL);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetTPGSelect(unsigned int *pPattern)
/**
 * (I2C: 0x0A)
 * (USB: CMD2: 0x12, CMD3: 0x03)
 * This command reads back the selected internal test pattern.
 *
 * @param   pattern  - O - Selected internal test pattern:
 *                         0x0 = Solid Field
 *                         0x1 = Horizontal Ramp
 *                         0x2 = Vertical Ramp
 *                         0x3 = Horizontal Lines
 *                         0x4 = Diagonal Lines
 *                         0x5 = Vertical Lines
 *                         0x6 = Grid
 *                         0x7 = Checkerboard
 *                         0x8 = RGB Ramp
 *                         0x9 = Color Bars
 *                         0xA = Step Bars
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(TPG_SEL);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pPattern = msg.text.data[0] & (BIT0 | BIT1 | BIT2 | BIT3);
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_LoadImageIndex(unsigned int index)
/**
 * (I2C: 0x7F)
 * (USB: CMD2: 0x1A, CMD3: 0x39)
 * This command loads an image from flash memory and then performs a buffer swap to display the loaded
 * image on the DMD.
 *
 * @param   index  - I - Image Index. Loads the image at this index from flash.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = index;
    DLPC350_PrepWriteCmd(&msg, IMAGE_LOAD);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetImageIndex(unsigned int *pIndex)
/**
 * (I2C: 0x7F)
 * (USB: CMD2: 0x1A, CMD3: 0x39)
 * This command loads reads back the index that was loaded most recently via DLPC350_LoadImageIndex() API.
 *
 * @param   *pIndex  - O - Image Index. Image at this index is loaded from flash.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(IMAGE_LOAD);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pIndex = msg.text.data[0];
        return 0;
    }
    return -1;
}

int  CLight3DTiDLP4500::DLPC350_GetNumImagesInFlash(unsigned int *pNumImgInFlash)
/**
 * (I2C: 0x0C)
 * (USB: CMD2: 0x1A, CMD3: 0x42)
 * This command reads number of images in the firmware running in DLPC350 controller.
 *
 * @param   *pNumImgInFlash  - O - Number of Images in the Flash.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(NUM_IMAGE_IN_FLASH);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pNumImgInFlash = (msg.text.data[0]&0xFF);
        return 0;
    }

    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetDisplay(rectangle2 croppedArea, rectangle2 displayArea)
/**
 * (I2C: 0x7E)
 * (USB: CMD2: 0x10, CMD3: 0x00)
 * The Input Display Resolution command defines the active input resolution and active output (displayed)
 * resolution. The maximum supported input and output resolutions for the DLP4500 0.45 WXGA DMD is
 * 1280 pixels (columns) by 800 lines (rows). This command provides the option to define a subset of active
 * input frame data using pixel (column) and line (row) counts relative to the source-data enable signal
 * (DATEN). In other words, this feature allows the source image to be cropped as the first step in the
 * processing chain.
 *
 * @param croppedArea - I - The rectagle structure contains the following
 *          parameters to describe the area to be cropped:
 *              - FirstPixel <BR>
 *              - FirstLine <BR>
 *              - PixelsPerLine <BR>
 *              - LinesPerFrame <BR>
 * @param displayArea - I - The rectagle structure contains the following
 *          parameters to describe the display area:
 *              - FirstPixel <BR>
 *              - FirstLine <BR>
 *              - PixelsPerLine <BR>
 *              - LinesPerFrame <BR>
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = croppedArea.firstPixel & 0xFF;
    msg.text.data[3] = croppedArea.firstPixel >> 8;
    msg.text.data[4] = croppedArea.firstLine & 0xFF;
    msg.text.data[5] = croppedArea.firstLine >> 8;
    msg.text.data[6] = croppedArea.pixelsPerLine & 0xFF;
    msg.text.data[7] = croppedArea.pixelsPerLine >> 8;
    msg.text.data[8] = croppedArea.linesPerFrame & 0xFF;
    msg.text.data[9] = croppedArea.linesPerFrame >> 8;
    msg.text.data[10] = displayArea.firstPixel & 0xFF;
    msg.text.data[11] = displayArea.firstPixel >> 8;
    msg.text.data[12] = displayArea.firstLine & 0xFF;
    msg.text.data[13] = displayArea.firstLine >> 8;
    msg.text.data[14] = displayArea.pixelsPerLine & 0xFF;
    msg.text.data[15] = displayArea.pixelsPerLine >> 8;
    msg.text.data[16] = displayArea.linesPerFrame & 0xFF;
    msg.text.data[17] = displayArea.linesPerFrame >> 8;

    DLPC350_PrepWriteCmd(&msg, DISP_CONFIG);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetDisplay(rectangle2 *pCroppedArea, rectangle2 *pDisplayArea)
/**
 * (I2C: 0x7E)
 * (USB: CMD2: 0x10, CMD3: 0x00)
 * This command reads back the active input resolution and active output (displayed) resolution.
 *
 * @param *pCroppedArea - O - The rectagle structure contains the following
 *          parameters to describe the area to be cropped:
 *              - FirstPixel <BR>
 *              - FirstLine <BR>
 *              - PixelsPerLine <BR>
 *              - LinesPerFrame <BR>
 * @param *pDisplayArea - O - The rectagle structure contains the following
 *          parameters to describe the display area:
 *              - FirstPixel <BR>
 *              - FirstLine <BR>
 *              - PixelsPerLine <BR>
 *              - LinesPerFrame <BR>
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(DISP_CONFIG);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        pCroppedArea->firstPixel = msg.text.data[0] | msg.text.data[1] << 8;
        pCroppedArea->firstLine = msg.text.data[2] | msg.text.data[3] << 8;
        pCroppedArea->pixelsPerLine = msg.text.data[4] | msg.text.data[5] << 8;
        pCroppedArea->linesPerFrame = msg.text.data[6] | msg.text.data[7] << 8;
        pDisplayArea->firstPixel = msg.text.data[8] | msg.text.data[9] << 8;
        pDisplayArea->firstLine = msg.text.data[10] | msg.text.data[11] << 8;
        pDisplayArea->pixelsPerLine = msg.text.data[12] | msg.text.data[13] << 8;
        pDisplayArea->linesPerFrame = msg.text.data[14] | msg.text.data[15] << 8;

        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetTPGColor(unsigned short redFG, unsigned short greenFG, unsigned short blueFG, unsigned short redBG, unsigned short greenBG, unsigned short blueBG)
/**
 * (I2C: 0x1A)
 * (USB: CMD2: 0x12, CMD3: 0x04)
 * When the internal test pattern is the selected input, the Internal Test Patterns Color Control defines the
 * colors of the test pattern displayed on the screen. The foreground color setting affects all test patterns. The background color
 * setting affects those test patterns that have a foreground and background component, such as, Horizontal
 * Lines, Diagonal Lines, Vertical Lines, Grid, and Checkerboard.
 *
 * @param   redFG  - I - Red Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Red Foreground color intensity
 *                       0x3FF = Full Red Foreground color intensity
 * @param   greenFG  - I - Green Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Green Foreground color intensity
 *                       0x3FF = Full Green Foreground color intensity
 * @param   blueFG  - I - Blue Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Blue Foreground color intensity
 *                       0x3FF = Full Blue Foreground color intensity
 * @param   redBG  - I - Red Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Red Foreground color intensity
 *                       0x3FF = Full Red Foreground color intensity
 * @param   greenBG  - I - Green Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Green Foreground color intensity
 *                       0x3FF = Full Red Foreground color intensity
 * @param   blueBG  - I - Red Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Blue Foreground color intensity
 *                       0x3FF = Full Blue Foreground color intensity
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = (char)redFG;
    msg.text.data[3] = (char)(redFG >> 8);
    msg.text.data[4] = (char)greenFG;
    msg.text.data[5] = (char)(greenFG >> 8);
    msg.text.data[6] = (char)blueFG;
    msg.text.data[7] = (char)(blueFG >> 8);
    msg.text.data[8] = (char)redBG;
    msg.text.data[9] = (char)(redBG >> 8);
    msg.text.data[10] = (char)greenBG;
    msg.text.data[11] = (char)(greenBG >> 8);
    msg.text.data[12] = (char)blueBG;
    msg.text.data[13] = (char)(blueBG >> 8);

    DLPC350_PrepWriteCmd(&msg, TPG_COLOR);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetTPGColor(unsigned short *pRedFG, unsigned short *pGreenFG, unsigned short *pBlueFG, unsigned short *pRedBG, unsigned short *pGreenBG, unsigned short *pBlueBG)
/**
 * (I2C: 0x1A)
 * (USB: CMD2: 0x12, CMD3: 0x04)
 * When the internal test pattern is the selected input, the Internal Test Patterns Color Control defines the
 * colors of the test pattern displayed on the screen. The foreground color setting affects all test patterns. The background color
 * setting affects those test patterns that have a foreground and background component, such as, Horizontal
 * Lines, Diagonal Lines, Vertical Lines, Grid, and Checkerboard.
 *
 * @param   *pRedFG  - O - Red Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Red Foreground color intensity
 *                       0x3FF = Full Red Foreground color intensity
 * @param   *pGreenFG  - O - Green Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Green Foreground color intensity
 *                       0x3FF = Full Green Foreground color intensity
 * @param   *pBlueFG  - O - Blue Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Blue Foreground color intensity
 *                       0x3FF = Full Blue Foreground color intensity
 * @param   *pRedBG  - O - Red Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Red Foreground color intensity
 *                       0x3FF = Full Red Foreground color intensity
 * @param   *pGreenBG  - O - Green Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Green Foreground color intensity
 *                       0x3FF = Full Red Foreground color intensity
 * @param   *pBlueBG  - O - Red Foreground Color intensity in a scale from 0 to 1023
 *                       0x0 = No Blue Foreground color intensity
 *                       0x3FF = Full Blue Foreground color intensity
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(TPG_COLOR);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pRedFG = msg.text.data[0] | msg.text.data[1] << 8;
        *pGreenFG = msg.text.data[2] | msg.text.data[3] << 8;
        *pBlueFG = msg.text.data[4] | msg.text.data[5] << 8;
        *pRedBG = msg.text.data[6] | msg.text.data[7] << 8;
        *pGreenBG = msg.text.data[8] | msg.text.data[9] << 8;
        *pBlueBG = msg.text.data[10] | msg.text.data[11] << 8;

        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_ClearPatLut(void)
/**
 * This API does not send any commands to the controller.It clears the locally (in the GUI program) stored pattern LUT.
 * See table 2-65 in programmer's guide for detailed desciprtion of pattern LUT entries.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    m_PatLutIndex = 0;
    return 0;
}

int CLight3DTiDLP4500::DLPC350_ClearExpLut(void)
/**
 * This API does not send any commands to the controller.It clears the locally (in the GUI program) stored exposure LUT.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    m_ExpLutIndex = 0;
    return 0;
}

int CLight3DTiDLP4500::DLPC350_AddToPatLut(int TrigType, int PatNum,int BitDepth,int LEDSelect,bool InvertPat, bool InsertBlack,bool BufSwap, bool trigOutPrev)
/**
 * This API does not send any commands to the controller.
 * It makes an entry (appends) in the locally stored (in the GUI program) pattern LUT as per the input arguments passed to this function.
 * See table 2-65 in programmer's guide for detailed desciprtion of pattern LUT entries.
 *
 * @param   TrigType  - I - Select the trigger type for the pattern
 *                          0 = Internal trigger
 *                          1 = External positive
 *                          2 = External negative
 *                          3 = No Input Trigger (Continue from previous; Pattern still has full exposure time)
 *                       0x3FF = Full Red Foreground color intensity
 * @param   PatNum  - I - Pattern number (0 based index). For pattern number 0x3F, there is no
 *                          pattern display. The maximum number supported is 24 for 1 bit-depth
 *                          patterns. Setting the pattern number to be 25, with a bit-depth of 1 will insert
 *                          a white-fill pattern. Inverting this pattern will insert a black-fill pattern. These w
 *                          patterns will have the same exposure time as defined in the Pattern Display
 *                          Exposure and Frame Period command. Table 2-66 in the programmer's guide illustrates which bit
 *                          planes are illuminated by each pattern number.
 * @param   BitDepth  - I - Select desired bit-depth
 *                          0 = Reserved
 *                          1 = 1-bit
 *                          2 = 2-bit
 *                          3 = 3-bit
 *                          4 = 4-bit
 *                          5 = 5-bit
 *                          6 = 6-bit
 *                          7 = 7-bit
 *                          8 = 8-bit
 * @param   LEDSelect  - I -  Choose the LEDs that are on: b0 = Red, b1 = Green, b2 = Blue
 *                          0 = No LED (Pass Through)
 *                          1 = Red
 *                          2 = Green
 *                          3 = Yellow (Green + Red)
 *                          4 = Blue
 *                          5 = Magenta (Blue + Red)
 *                          6 = Cyan (Blue + Green)
 *                          7 = White (Red + Blue + Green)
 * @param   InvertPat  - I - true = Invert pattern
 *                           false = do not invert pattern
 * @param   InsertBlack  - I - true = Insert black-fill pattern after current pattern. This setting requires 230 弮s
 *                                      of time before the start of the next pattern
 *                           false = do not insert any post pattern
 * @param   BufSwap  - I - true = perform a buffer swap
 *                           false = do not perform a buffer swap
 * @param   trigOutPrev  - I - true = Trigger Out 1 will continue to be high. There will be no falling edge
 *                                       between the end of the previous pattern and the start of the current pattern. w
 *                                       Exposure time is shared between all patterns defined under a common
 *                                       trigger out). This setting cannot be combined with the black-fill pattern
 *                           false = Trigger Out 1 has a rising edge at the start of a pattern, and a falling
 *                                       edge at the end of the pattern
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    unsigned long int lutWord = 0;

    lutWord = TrigType & 3;
    if(PatNum > 24)
        return -1;

    lutWord |= ((PatNum & 0x3F) << 2);
    if( (BitDepth > 8) || (BitDepth <= 0))
        return -1;
    lutWord |= ((BitDepth & 0xF) << 8);
    if(LEDSelect > 7)
        return -1;
    lutWord |= ((LEDSelect & 0x7) << 12);
    if(InvertPat)
        lutWord |= BIT16;
    if(InsertBlack)
        lutWord |= BIT17;
    if(BufSwap)
        lutWord |= BIT18;
    if(trigOutPrev)
        lutWord |= BIT19;

    m_PatLut[m_PatLutIndex++] = lutWord;
    return 0;
}

int CLight3DTiDLP4500::DLPC350_AddToExpLut(int TrigType, int PatNum,int BitDepth,int LEDSelect,bool InvertPat, bool InsertBlack,bool BufSwap, bool trigOutPrev, unsigned int exp_time_us, unsigned int ptn_frame_period_us)
/**
 * This API does not send any commands to the controller.
 * It makes an entry (appends) in the locally stored (in the GUI program) pattern LUT as per the input arguments passed to this function.
 * See table 2-65 in programmer's guide for detailed desciprtion of pattern LUT entries.
 *
 * @param   TrigType  - I - Select the trigger type for the pattern
 *                          0 = Internal trigger
 *                          1 = External positive
 *                          2 = External negative
 *                          3 = No Input Trigger (Continue from previous; Pattern still has full exposure time)
 *                       0x3FF = Full Red Foreground color intensity
 * @param   PatNum  - I - Pattern number (0 based index). For pattern number 0x3F, there is no
 *                          pattern display. The maximum number supported is 24 for 1 bit-depth
 *                          patterns. Setting the pattern number to be 25, with a bit-depth of 1 will insert
 *                          a white-fill pattern. Inverting this pattern will insert a black-fill pattern. These w
 *                          patterns will have the same exposure time as defined in the Pattern Display
 *                          Exposure and Frame Period command. Table 2-66 in the programmer's guide illustrates which bit
 *                          planes are illuminated by each pattern number.
 * @param   BitDepth  - I - Select desired bit-depth
 *                          0 = Reserved
 *                          1 = 1-bit
 *                          2 = 2-bit
 *                          3 = 3-bit
 *                          4 = 4-bit
 *                          5 = 5-bit
 *                          6 = 6-bit
 *                          7 = 7-bit
 *                          8 = 8-bit
 * @param   LEDSelect  - I -  Choose the LEDs that are on: b0 = Red, b1 = Green, b2 = Blue
 *                          0 = No LED (Pass Through)
 *                          1 = Red
 *                          2 = Green
 *                          3 = Yellow (Green + Red)
 *                          4 = Blue
 *                          5 = Magenta (Blue + Red)
 *                          6 = Cyan (Blue + Green)
 *                          7 = White (Red + Blue + Green)
 * @param   InvertPat  - I - true = Invert pattern
 *                           false = do not invert pattern
 * @param   InsertBlack  - I - true = Insert black-fill pattern after current pattern. This setting requires 230 弮s
 *                                      of time before the start of the next pattern
 *                           false = do not insert any post pattern
 * @param   BufSwap  - I - true = perform a buffer swap
 *                           false = do not perform a buffer swap
 * @param   trigOutPrev  - I - true = Trigger Out 1 will continue to be high. There will be no falling edge
 *                                       between the end of the previous pattern and the start of the current pattern. w
 *                                       Exposure time is shared between all patterns defined under a common
 *                                       trigger out). This setting cannot be combined with the black-fill pattern
 *                           false = Trigger Out 1 has a rising edge at the start of a pattern, and a falling
 *                                       edge at the end of the pattern
 * @param   exp_time_us     -I expsoure time in microseconds for this pattern
 * @param   ptn_frame_period_us     -I frame time in microseconds for this pattern
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    unsigned long int lutWord = 0;

    lutWord = TrigType & 3;
    if(PatNum > 24)
        return -1;

    lutWord |= ((PatNum & 0x3F) << 2);
    if( (BitDepth > 8) || (BitDepth <= 0))
        return -1;
    lutWord |= ((BitDepth & 0xF) << 8);
    if(LEDSelect > 7)
        return -1;
    lutWord |= ((LEDSelect & 0x7) << 12);
    if(InvertPat)
        lutWord |= BIT16;
    if(InsertBlack)
        lutWord |= BIT17;
    if(BufSwap)
        lutWord |= BIT18;
    if(trigOutPrev)
        lutWord |= BIT19;

    m_ExpLut[m_ExpLutIndex++] = lutWord;
    m_ExpLut[m_ExpLutIndex++] = exp_time_us;
    m_ExpLut[m_ExpLutIndex++] = ptn_frame_period_us;
    return 0;
}

int CLight3DTiDLP4500::DLPC350_GetPatLutItem(int index, int *pTrigType, int *pPatNum,int *pBitDepth,int *pLEDSelect,bool *pInvertPat, bool *pInsertBlack,bool *pBufSwap, bool *pTrigOutPrev)
/**
 * This API does not send any commands to the controller.
 * It reads back an entry at the specified index from the locally stored (in the GUI program) pattern LUT and populates the input arguments passed to this function.
 * See table 2-65 in programmer's guide for detailed desciprtion of pattern LUT entries.
 *
 * @param   index  - I - Entry at this index from pattern LUT to be read back.
 * @param   *pTrigType  - O - Select the trigger type for the pattern
 *                          0 = Internal trigger
 *                          1 = External positive
 *                          2 = External negative
 *                          3 = No Input Trigger (Continue from previous; Pattern still has full exposure time)
 *                       0x3FF = Full Red Foreground color intensity
 * @param   *pPatNum  - O - Pattern number (0 based index). For pattern number 0x3F, there is no
 *                          pattern display. The maximum number supported is 24 for 1 bit-depth
 *                          patterns. Setting the pattern number to be 25, with a bit-depth of 1 will insert
 *                          a white-fill pattern. Inverting this pattern will insert a black-fill pattern. These w
 *                          patterns will have the same exposure time as defined in the Pattern Display
 *                          Exposure and Frame Period command. Table 2-66 in the programmer's guide illustrates which bit
 *                          planes are illuminated by each pattern number.
 * @param   *pBitDepth  - O - Select desired bit-depth
 *                          0 = Reserved
 *                          1 = 1-bit
 *                          2 = 2-bit
 *                          3 = 3-bit
 *                          4 = 4-bit
 *                          5 = 5-bit
 *                          6 = 6-bit
 *                          7 = 7-bit
 *                          8 = 8-bit
 * @param   *pLEDSelect  - O -  Choose the LEDs that are on: b0 = Red, b1 = Green, b2 = Blue
 *                          0 = No LED (Pass Through)
 *                          1 = Red
 *                          2 = Green
 *                          3 = Yellow (Green + Red)
 *                          4 = Blue
 *                          5 = Magenta (Blue + Red)
 *                          6 = Cyan (Blue + Green)
 *                          7 = White (Red + Blue + Green)
 * @param   *pInvertPat  - O - true = Invert pattern
 *                           false = do not invert pattern
 * @param   *pInsertBlack  - O - true = Insert black-fill pattern after current pattern. This setting requires 230 弮s
 *                                      of time before the start of the next pattern
 *                           false = do not insert any post pattern
 * @param   *pBufSwap  - O - true = perform a buffer swap
 *                           false = do not perform a buffer swap
 * @param   *pTrigOutPrev  - O - true = Trigger Out 1 will continue to be high. There will be no falling edge
 *                                       between the end of the previous pattern and the start of the current pattern. w
 *                                       Exposure time is shared between all patterns defined under a common
 *                                       trigger out). This setting cannot be combined with the black-fill pattern
 *                           false = Trigger Out 1 has a rising edge at the start of a pattern, and a falling
 *                                       edge at the end of the pattern
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    unsigned int lutWord;

    lutWord = m_PatLut[index];

    *pTrigType = lutWord & 3;
    *pPatNum = (lutWord >> 2) & 0x3F;
    *pBitDepth = (lutWord >> 8) & 0xF;
    *pLEDSelect = (lutWord >> 12) & 7;
    *pInvertPat = ((lutWord & BIT16) == BIT16);
    *pInsertBlack = ((lutWord & BIT17) == BIT17);
    *pBufSwap = ((lutWord & BIT18) == BIT18);
    *pTrigOutPrev = ((lutWord & BIT19) == BIT19);

    return 0;
}

int CLight3DTiDLP4500::DLPC350_GetVarExpPatLutItem(int index, int *pTrigType, int *pPatNum,int *pBitDepth,int *pLEDSelect,bool *pInvertPat, bool *pInsertBlack,bool *pBufSwap, bool *pTrigOutPrev, int *pPatExp, int *pPatPeriod)
/**
 * This API does not send any commands to the controller.
 * It reads back an entry at the specified index from the locally stored (in the GUI program) pattern LUT and populates the input arguments passed to this function.
 * See table 2-65 in programmer's guide for detailed desciprtion of pattern LUT entries.
 *
 * @param   index  - I - Entry at this index from pattern LUT to be read back.
 * @param   *pTrigType  - O - Select the trigger type for the pattern
 *                          0 = Internal trigger
 *                          1 = External positive
 *                          2 = External negative
 *                          3 = No Input Trigger (Continue from previous; Pattern still has full exposure time)
 *                       0x3FF = Full Red Foreground color intensity
 * @param   *pPatNum  - O - Pattern number (0 based index). For pattern number 0x3F, there is no
 *                          pattern display. The maximum number supported is 24 for 1 bit-depth
 *                          patterns. Setting the pattern number to be 25, with a bit-depth of 1 will insert
 *                          a white-fill pattern. Inverting this pattern will insert a black-fill pattern. These w
 *                          patterns will have the same exposure time as defined in the Pattern Display
 *                          Exposure and Frame Period command. Table 2-66 in the programmer's guide illustrates which bit
 *                          planes are illuminated by each pattern number.
 * @param   *pBitDepth  - O - Select desired bit-depth
 *                          0 = Reserved
 *                          1 = 1-bit
 *                          2 = 2-bit
 *                          3 = 3-bit
 *                          4 = 4-bit
 *                          5 = 5-bit
 *                          6 = 6-bit
 *                          7 = 7-bit
 *                          8 = 8-bit
 * @param   *pLEDSelect  - O -  Choose the LEDs that are on: b0 = Red, b1 = Green, b2 = Blue
 *                          0 = No LED (Pass Through)
 *                          1 = Red
 *                          2 = Green
 *                          3 = Yellow (Green + Red)
 *                          4 = Blue
 *                          5 = Magenta (Blue + Red)
 *                          6 = Cyan (Blue + Green)
 *                          7 = White (Red + Blue + Green)
 * @param   *pInvertPat  - O - true = Invert pattern
 *                           false = do not invert pattern
 * @param   *pInsertBlack  - O - true = Insert black-fill pattern after current pattern. This setting requires 230 弮s
 *                                      of time before the start of the next pattern
 *                           false = do not insert any post pattern
 * @param   *pBufSwap  - O - true = perform a buffer swap
 *                           false = do not perform a buffer swap
 * @param   *pTrigOutPrev  - O - true = Trigger Out 1 will continue to be high. There will be no falling edge
 *                                       between the end of the previous pattern and the start of the current pattern. w
 *                                       Exposure time is shared between all patterns defined under a common
 *                                       trigger out). This setting cannot be combined with the black-fill pattern
 *                           false = Trigger Out 1 has a rising edge at the start of a pattern, and a falling
 *                                       edge at the end of the pattern
 * @param   *pPatExp  - O - Pattern Expsoure Time in us
 *
 * @param   *pPatPeriod - O - Total Pattern Period in us
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    unsigned long int lutWord;

    lutWord = m_ExpLut[(index*3)];

    *pTrigType = lutWord & 3;
    *pPatNum = (lutWord >> 2) & 0x3F;
    *pBitDepth = (lutWord >> 8) & 0xF;
    *pLEDSelect = (lutWord >> 12) & 7;
    *pInvertPat = ((lutWord & BIT16) == BIT16);
    *pInsertBlack = ((lutWord & BIT17) == BIT17);
    *pBufSwap = ((lutWord & BIT18) == BIT18);
    *pTrigOutPrev = ((lutWord & BIT19) == BIT19);
    *pPatExp = m_ExpLut[(index*3)+1];
    *pPatPeriod = m_ExpLut[(index*3)+2];

    return 0;
}

int CLight3DTiDLP4500::DLPC350_OpenMailbox(int MboxNum)
/**
 * (I2C: 0x77)
 * (USB: CMD2: 0x1A, CMD3: 0x33)
 * This API opens the specified Mailbox within the DLPC350 controller. This API must be called
 * before sending data to the mailbox/LUT using DLPC350_SendPatLut() or DLPC350_SendImageLut() APIs.
 *
 * @param MboxNum - I - 1 = Open the mailbox for image index configuration
 *                      2 = Open the mailbox for pattern definition.
 *                      3 = Open the mailbox for the Variable Exposure
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = MboxNum;
    DLPC350_PrepWriteCmd(&msg, MBOX_CONTROL);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_CloseMailbox(void)
/**
 * (I2C: 0x77)
 * (USB: CMD2: 0x1A, CMD3: 0x33)
 * This API is internally used by other APIs within this file. There is no need for user application to
 * call this API separately.
 * This API closes all the Mailboxes within the DLPC350 controller.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = 0;
    DLPC350_PrepWriteCmd(&msg, MBOX_CONTROL);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_MailboxSetAddr(int Addr)
/**
 * (I2C: 0x76)
 * (USB: CMD2: 0x1A, CMD3: 0x32)
 * This API defines the offset location within the DLPC350 mailboxes to write data into or to read data from
 *
 * @param Addr - I - 0-127 - Defines the offset within the selected (opened) LUT to write/read data to/from.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    if(Addr > 127)
        return -1;

    msg.text.data[2] = Addr;
    DLPC350_PrepWriteCmd(&msg, MBOX_ADDRESS);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_SetVarExpMboxAddr(int Addr)
/**
 * (I2C: 0x5D)
 * (USB: CMD2: 0x1A, CMD3: 0x32)
 * This API defines the offset location within the DLPC350 mailboxes to write data into or to read data from
 *
 * @param Addr - I - 0-1823 - Defines the offset within the selected (opened) LUT to write/read data to/from.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    if(Addr > 1823)
        return -1;

    msg.text.data[2] = Addr;
    msg.text.data[3] = Addr >> 8;
    DLPC350_PrepWriteCmd(&msg, MBOX_EXP_ADDRESS);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_SendVarExpPatLut(void)
/**
 * (I2C: 0x5c)
 * (USB: CMD2: 0x1A, CMD3: 0x3E)
 * This API sends the pattern LUT created by calling DLPC350_AddToExpLut() API to the DLPC350 controller.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    unsigned int i;

#if 0
    printf("VarExpPatLut Send\n");
    for(i=0;i<m_ExpLutIndex;i++)
    {
        printf("VarExpPatLut[%04d] = 0x%X\n",i,m_ExpLut[i]);
    }
#endif

    if(DLPC350_OpenMailbox(3) < 0)
        return -1;

    DLPC350_PrepWriteCmd(&msg, MBOX_EXP_DATA);

    for(i=0; i<m_ExpLutIndex; i+=3)
    {
        if(DLPC350_SetVarExpMboxAddr(i/3) < 0)
            return -1;

        msg.text.data[2] = m_ExpLut[i];
        msg.text.data[3] = m_ExpLut[i]>>8;
        msg.text.data[4] = m_ExpLut[i]>>16;
        msg.text.data[5] = m_ExpLut[i]>>24;

        msg.text.data[6] = m_ExpLut[i+1];
        msg.text.data[7] = m_ExpLut[i+1]>>8;
        msg.text.data[8] = m_ExpLut[i+1]>>16;
        msg.text.data[9] = m_ExpLut[i+1]>>24;

        msg.text.data[10] = m_ExpLut[i+2];
        msg.text.data[11] = m_ExpLut[i+2]>>8;
        msg.text.data[12] = m_ExpLut[i+2]>>16;
        msg.text.data[13] = m_ExpLut[i+2]>>24;
        DLPC350_SendMsg(&msg,true);
    }

    DLPC350_CloseMailbox();

    return 0;
}


int CLight3DTiDLP4500::DLPC350_SendVarExpImageLut(unsigned char *lutEntries, unsigned int numEntries)
/**
 * (I2C: )
 * (USB: CMD2:  CMD3: )
 * This API sends the image LUT to the DLPC350 controller.
 *
 * @param   *lutEntries - I - Pointer to the array in which LUT entries to be sent are stored
 *
 * @param   numEntries - I - number of entries to be sent to the controller
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    unsigned int i;
    unsigned int bytes_sent = 0;

    if(numEntries < 1 || numEntries > MAX_VAR_EXP_IMAGE_LUT_ENTRIES)
        return -1;

#if 0
    printf("VarExpImageLut Send\n");
    for(i=0;i<numEntries;i++)
    {
        printf("varExpImageLut[%02d] = %d\n",i,lutEntries[i]);
    }
#endif

    DLPC350_OpenMailbox(1);
    DLPC350_SetVarExpMboxAddr(0);

    // Check for special case of 2 entries
    if( numEntries == 2)
    {
        msg.text.data[2] = lutEntries[1];
        msg.text.data[3] = lutEntries[0];
    }
    else
    {
        for(i=0; i<numEntries; i++)
        {
            msg.text.data[2+i] = lutEntries[i];
        }
    }

    CmdList[MBOX_DATA].len = numEntries;
    DLPC350_PrepWriteCmd(&msg, MBOX_DATA);
    bytes_sent = DLPC350_SendMsg(&msg,true);
    DLPC350_CloseMailbox();

    return bytes_sent;
}



int CLight3DTiDLP4500::DLPC350_SendPatLut(void)
/**
 * (I2C: 0x78)
 * (USB: CMD2: 0x1A, CMD3: 0x34)
 * This API sends the pattern LUT created by calling DLPC350_AddToPatLut() API to the DLPC350 controller.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    int bytesToSend=m_PatLutIndex*3;
    unsigned int i;

#if 0
    printf("PatLut Send\n");
    for(i=0;i<m_PatLutIndex;i++)
    {
        printf("PatLut[%03d] = 0x%X\n",i,m_PatLut[i]);
    }
#endif

    if(DLPC350_OpenMailbox(2) < 0)
        return -1;
    DLPC350_MailboxSetAddr(0);

    CmdList[MBOX_DATA].len = bytesToSend;
    DLPC350_PrepWriteCmd(&msg, MBOX_DATA);

    for(i=0; i<m_PatLutIndex; i++)
    {
        msg.text.data[2+3*i] = m_PatLut[i];
        msg.text.data[2+3*i+1] = m_PatLut[i]>>8;
        msg.text.data[2+3*i+2] = m_PatLut[i]>>16;
    }

    DLPC350_SendMsg(&msg,true);
    DLPC350_CloseMailbox();
    return 0;
}

int CLight3DTiDLP4500::DLPC350_SendImageLut(unsigned char *lutEntries, unsigned int numEntries)
/**
 * (I2C: 0x78)
 * (USB: CMD2: 0x1A, CMD3: 0x34)
 * This API sends the image LUT to the DLPC350 controller.
 *
 * @param   *lutEntries - I - Pointer to the array in which LUT entries to be sent are stored
 *
 * @param   numEntries - I - number of entries to be sent to the controller
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    unsigned int i;
    unsigned int bytes_sent = 0;

#if 0
    printf("ImageLut Send\n");
    for(i=0;i<numEntries;i++)
    {
        printf("ImageLut[%03d] = %d\n",i,lutEntries[i]);
    }
#endif

    if(numEntries < 1 || numEntries > MAX_IMAGE_LUT_ENTRIES)
        return -1;

    DLPC350_OpenMailbox(1);
    DLPC350_MailboxSetAddr(0);

    // Check for special case of 2 entries
    if( numEntries == 2)
    {
        msg.text.data[2+0] = lutEntries[1];
        msg.text.data[2+1] = lutEntries[0];
    }
    else
    {
        for(i=0; i < numEntries; i++)
        {
            msg.text.data[2+i] = lutEntries[i];
        }
    }

    CmdList[MBOX_DATA].len = numEntries;
    DLPC350_PrepWriteCmd(&msg, MBOX_DATA);
    bytes_sent = DLPC350_SendMsg(&msg,true);
    DLPC350_CloseMailbox();

    return bytes_sent;
}

int CLight3DTiDLP4500::DLPC350_GetPatLut(int numEntries)
/**
 * (I2C: 0x78)
 * (USB: CMD2: 0x1A, CMD3: 0x34)
 * This API reads the pattern LUT from the DLPC350 controller and stores it in the local array.
 * The pattern LUT entries could be queried using the API DLPC350_GetPatLutItem().
 *
 * @param   numEntries - I - Number of entries expected in pattern LUT.
 *
 * @return  number of bytes actually read from the controller LUT.
 *
 */
{
    hidMessageStruct msg;
    unsigned int lutWord = 0;
    int numBytes, i;
    unsigned char *readBuf;

    if(numEntries > 128)
        return -1;

    if(DLPC350_OpenMailbox(2) < 0)
        return -1;

    if(DLPC350_MailboxSetAddr(0) < 0)
        return -1;

    numBytes = sizeof(msg.head)+numEntries*3;
    readBuf = (unsigned char *)&msg;
    DLPC350_PrepReadCmd(MBOX_DATA);


    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(readBuf, g_InputBuffer, MIN(numBytes,64));
        readBuf+=64;
        numBytes -=64;
    }
    else
    {
        DLPC350_CloseMailbox();
        return -1;
    }
    /* If packet is greater than 64 bytes, continue to read */
    while(numBytes > 0)
    {
        if(DLPC350_ContinueRead() < 0)
            return -1;
		
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(readBuf, g_InputBuffer, MIN(numBytes,64));
        readBuf+=64;
        numBytes -=64;
    }

    DLPC350_ClearPatLut();

    for(i=0; i<numEntries*3; i+=3)
    {
        lutWord = msg.text.data[i] | msg.text.data[i+1] << 8 | msg.text.data[i+2] << 16;
        m_PatLut[m_PatLutIndex++] = lutWord;
    }

    if(DLPC350_CloseMailbox() < 0)
        return -1;

#if 0
    printf("PatLut Receive\n");
    for(unsigned i=0;i<m_PatLutIndex;i++)
    {
        printf("PatLut[%03d] = 0x%X\n",i,m_PatLut[i]);
    }
#endif

    return (int)msg.head.length;
}

int CLight3DTiDLP4500::DLPC350_GetVarExpPatLut(int numEntries)
/**
 * (I2C: 0x5D)
 * (USB: CMD2: 0x1A, CMD3: 0x3E)
 * This API reads the pattern LUT from the DLPC350 controller and stores it in the local array.
 * The pattern LUT entries could be queried using the API DLPC350_GetVarExpPatLutItem().
 *
 * @param   numEntries - I - Number of entries expected in pattern LUT.
 *
 * @return  0 = PASS   <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    int numBytes, i;
    unsigned char *readBuf;
    int numLutEntrRead = 0;

    if(numEntries > MAX_VAR_EXP_PAT_LUT_ENTRIES)
        return -1;

    if(DLPC350_OpenMailbox(3) < 0)
        return -1;

    DLPC350_ClearExpLut();

    i = 0;
    readBuf = (unsigned char *)&msg;
    numBytes = sizeof(msg.head)+12;
    do
    {
        if(DLPC350_SetVarExpMboxAddr(i) < 0)
            return -1;

        DLPC350_PrepReadCmd(MBOX_EXP_DATA);

        if(DLPC350_Read() > 0)
        {
			unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
            memcpy(readBuf, g_InputBuffer, numBytes);
            m_ExpLut[m_ExpLutIndex++] = (msg.text.data[0] | msg.text.data[1] << 8 | msg.text.data[2] << 16 | msg.text.data[3] << 24);
            m_ExpLut[m_ExpLutIndex++] = (msg.text.data[4] | msg.text.data[5] << 8 | msg.text.data[6] << 16 | msg.text.data[7] << 24); //Pattern Exposure
            m_ExpLut[m_ExpLutIndex++] = (msg.text.data[8] | msg.text.data[9] << 8 | msg.text.data[10] << 16 | msg.text.data[11] << 24);; //Total Pattern Period
            numLutEntrRead++;
        }

        if(numLutEntrRead == numEntries)
        {
            break;
        }
        else
        {
            i++;
        }

    }while(1);

    if(DLPC350_CloseMailbox() < 0)
        return -1;

#if 0
    printf("VarExpPatLut Receive\n");
    for(unsigned i=0;i<m_ExpLutIndex;i++)
    {
        printf("varExpPatLut[%04d] = 0x%X\n",i,m_ExpLut[i]);
    }
#endif

    return 0;
}


int CLight3DTiDLP4500::DLPC350_GetImageLut(unsigned char *pLut, int numEntries)
/**
 * (I2C: 0x78)
 * (USB: CMD2: 0x1A, CMD3: 0x34)
 * This API reads the image LUT from the DLPC350 controller.
 *
 * @param   *pLut - I - Pointer to the array in which the read entries should be stored
 *
 * @param   numEntries - I - Number of image LUT entries to be read from the controller
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    int retval;

    if(DLPC350_OpenMailbox(1) < 0)
        return -1;

    if(DLPC350_MailboxSetAddr(0) < 0)
        return -1;

    DLPC350_PrepReadCmd(MBOX_DATA);

    if((retval = DLPC350_Read()) > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        hidMessageStruct *pMsg = (hidMessageStruct *)g_InputBuffer;
        if(pMsg != NULL)
        {
            memcpy(pLut, g_InputBuffer+sizeof(pMsg->head), MIN((unsigned int)numEntries,64-sizeof(pMsg->head)));
            pLut+= (64-sizeof(pMsg->head));
            numEntries -= (64-sizeof(pMsg->head));
        }
    }
    else
    {
        DLPC350_CloseMailbox();
        return retval;
    }

    /* If packet is greater than 64 bytes, continue to read */
    while(numEntries > 0)
    {
        DLPC350_ContinueRead();
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(pLut, g_InputBuffer, MIN(numEntries,64));
        pLut+=64;
        numEntries -= 64;
    }

    if(DLPC350_CloseMailbox() < 0)
        return -1;

    return 0;
}

int CLight3DTiDLP4500::DLPC350_GetvarExpImageLut(unsigned char *pLut, int numEntries)
/**
 * (I2C: 0x78)
 * (USB: CMD2: 0x1A, CMD3: 0x34)
 * This API reads the image LUT from the DLPC350 controller.
 *
 * @param   *pLut - I - Pointer to the array in which the read entries should be stored
 *
 * @param   numEntries - I - Number of image LUT entries to be read from the controller
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    int retval;

    if(DLPC350_OpenMailbox(1) < 0)
        return -1;

    if(DLPC350_SetVarExpMboxAddr(0) < 0)
        return -1;

    DLPC350_PrepReadCmd(MBOX_DATA);

    if((retval = DLPC350_Read()) > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        hidMessageStruct *pMsg = (hidMessageStruct *)g_InputBuffer;
        if(pMsg != NULL)
        {
			unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
            memcpy(pLut, g_InputBuffer+sizeof(pMsg->head), MIN((unsigned int)numEntries,64-sizeof(pMsg->head)));
            pLut+= (64-sizeof(pMsg->head));
            numEntries -= (64-sizeof(pMsg->head));
        }
    }
    else
    {
        DLPC350_CloseMailbox();
        return retval;
    }

    /* If packet is greater than 64 bytes, continue to read */
    while(numEntries > 0)
    {
        DLPC350_ContinueRead();
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(pLut, g_InputBuffer, MIN(numEntries,64));
        pLut+=64;
        numEntries -= 64;
    }

    if(DLPC350_CloseMailbox() < 0)
        return -1;

    return 0;
}

int CLight3DTiDLP4500::DLPC350_SetPatternTriggerMode(int trigMode)
/**
 * The Pattern Trigger Mode Selection command selects between one of the three pattern Trigger Modes.
 * Before executing this command, stop the current pattern sequence. After executing this command, send
 * the Validation command (I2C: 0x7D or USB: 0x1A1A) once before starting the pattern sequence.
 *
 * @param   trigMode  - I - 0 = Pattern Trigger Mode 0: VSYNC serves to trigger the pattern display sequence.
 *                          1 = Pattern Trigger Mode 1: Internally or Externally (through TRIG_IN1 and TRIG_IN2) generated trigger.
 *                          2 = Pattern Trigger Mode 2: TRIG_IN_1 alternates between two patterns,while TRIG_IN_2 advances to the next pair of patterns.
 *                          3 = Pattern Trigger Mode 3: Internally or externally generated trigger for Variable Exposure display sequence.
 *                          4 = Pattern Trigger Mode 4: VSYNC triggered for Variable Exposure display sequence.
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = trigMode;
    DLPC350_PrepWriteCmd(&msg, PAT_TRIG_MODE);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetPatternTriggerMode(int *trigMode)
/**
 * The Pattern Trigger Mode Selection command selects between one of the three pattern Trigger Modes.
 *
 * @param   trigMode  - O - 0 = Pattern Trigger Mode 0: VSYNC serves to trigger the pattern display sequence.
 *                          1 = Pattern Trigger Mode 1: Internally or Externally (through TRIG_IN1 and TRIG_IN2) generated trigger.
 *                          2 = Pattern Trigger Mode 2: TRIG_IN_1 alternates between two patterns,while TRIG_IN_2 advances to the next pair of patterns.
 *                          3 = Pattern Trigger Mode 3: Internally or externally generated trigger for Variable Exposure display sequence.
 *                          4 = Pattern Trigger Mode 4: VSYNC triggered for Variable Exposure display sequence.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{    hidMessageStruct msg;

     DLPC350_PrepReadCmd(PAT_TRIG_MODE);

      if(DLPC350_Read() > 0)
      {
		  unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
          memcpy(&msg, g_InputBuffer, 65);
          *trigMode = (msg.text.data[0] & 0xFF);
          return 0;
      }
      return -1;
}


int CLight3DTiDLP4500::DLPC350_PatternDisplay(unsigned int Action)
/**
 * (I2C: 0x65)
 * (USB: CMD2: 0x1A, CMD3: 0x24)
 * This API starts or stops the programmed patterns sequence.
 *
 * @param   Action - I - Pattern Display Start/Stop Pattern Sequence
 *                          0 = Stop Pattern Display Sequence. The next "Start" command will
 *                              restart the pattern sequence from the beginning.
 *                          1 = Pause Pattern Display Sequence. The next "Start" command will
 *                              start the pattern sequence by re-displaying the current pattern in the sequence.
 *                          2 = Start Pattern Display Sequence
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = (Action&0x03);
    DLPC350_PrepWriteCmd(&msg, PAT_START_STOP);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetPatternDisplay(unsigned int *pAction)
/**
 * (I2C: 0x65)
 * (USB: CMD2: 0x1A, CMD3: 0x24)
 * This API starts or stops the programmed patterns sequence.
 *
 * @param   *pAction - O - Returns the state of the Pattern Display
 *                          0 = Stop Pattern Display Sequence. The next "Start" command will
 *                              restart the pattern sequence from the beginning.
 *                          1 = Pause Pattern Display Sequence. The next "Start" command will
 *                              start the pattern sequence by re-displaying the current pattern in the sequence.
 *                          2 = Start Pattern Display Sequence
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

     DLPC350_PrepReadCmd(PAT_START_STOP);

      if(DLPC350_Read() > 0)
      {
		  unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
          memcpy(&msg, g_InputBuffer, 65);
          *pAction = (msg.text.data[0] & 0xFF);
          return 0;
      }
      return -1;
}


int CLight3DTiDLP4500::DLPC350_SetPatternConfig(unsigned int numLutEntries, bool repeat, unsigned int numPatsForTrigOut2, unsigned int numImages)
/**
 * (I2C: 0x75)
 * (USB: CMD2: 0x1A, CMD3: 0x31)
 * This API controls the execution of patterns stored in the lookup table.
 * Before using this API, stop the current pattern sequence using DLPC350_PatternDisplay() API
 * After calling this API, send the Validation command using the API DLPC350_ValidatePatLutData() before starting the pattern sequence
 *
 * @param   numLutEntries - I - Number of LUT entries
 * @param   repeat - I - 0 = execute the pattern sequence once; 1 = repeat the pattern sequnce.
 * @param   numPatsForTrigOut2 - I - Number of patterns to display(range 1 through 256).
 *                                   If in repeat mode, then this value dictates how often TRIG_OUT_2 is generated.
 * @param   numImages - I - Number of Image Index LUT Entries(range 1 through 64).
 *                          This Field is irrelevant for Pattern Display Data Input Source set to a value other than internal.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = numLutEntries-1;         /* -1 because the firmware command takes 0-based indices (0 means 1) */
    msg.text.data[3] = repeat;
    msg.text.data[4] = numPatsForTrigOut2 - 1;  /* -1 because the firmware command takes 0-based indices (0 means 1) */
    msg.text.data[5] = numImages - 1;           /* -1 because the firmware command takes 0-based indices (0 means 1) */
    DLPC350_PrepWriteCmd(&msg, PAT_CONFIG);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_SetVarExpPatternConfig(unsigned int numLutEntries, unsigned int numPatsForTrigOut2, unsigned int numImages, bool repeat)
/**
 * (I2C: 0x5B)
 * (USB: CMD2: 0x1A, CMD3: 0x40)
 * This API controls the execution of patterns stored in the lookup table.
 * Before using this API, stop the current pattern sequence using DLPC350_PatternDisplay() API
 * After calling this API, send the Validation command using the API DLPC350_ValidatePatLutData() before starting the pattern sequence
 *
 * @param   numLutEntries - I - Number of LUT entries
 * @param   repeat - I - 0 = execute the pattern sequence once; 1 = repeat the pattern sequnce.
 * @param   numPatsForTrigOut2 - I - Number of patterns to display(range 1 through 256).
 *                                   If in repeat mode, then this value dictates how often TRIG_OUT_2 is generated.
 * @param   numImages - I - Number of Image Index LUT Entries(range 1 through 64).
 *                          This Field is irrelevant for Pattern Display Data Input Source set to a value other than internal.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = numLutEntries-1;             /* -1 because the firmware command takes 0-based indices (0 means 1) */
    msg.text.data[3] = (numLutEntries-1)>>8;
    msg.text.data[4] = numPatsForTrigOut2 - 1;      /* -1 because the firmware command takes 0-based indices (0 means 1) */
    msg.text.data[5] = (numPatsForTrigOut2-1)>>8;
    msg.text.data[6] = (numImages - 1) & 0xFF;               /* -1 because the firmware command takes 0-based indices (0 means 1) */
    (repeat == true) ? (msg.text.data[7] = 0x01) : (msg.text.data[7] = 0x00);
    DLPC350_PrepWriteCmd(&msg, EXP_PAT_CONFIG);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetVarExpPatternConfig(unsigned int *pNumLutEntries, unsigned int *pNumPatsForTrigOut2, unsigned int *pNumImages,  bool *pRepeat)
/**
 * (I2C: 0x5B)
 * (USB: CMD2: 0x1A, CMD3: 0x40)
 * This API controls the execution of patterns stored in the lookup table.
 * Before using this API, stop the current pattern sequence using DLPC350_PatternDisplay() API
 * After calling this API, send the Validation command using the API DLPC350_ValidatePatLutData() before starting the pattern sequence
 *
 * @param   numLutEntries - I - Number of LUT entries
 * @param   repeat - I - 0 = execute the pattern sequence once; 1 = repeat the pattern sequnce.
 * @param   numPatsForTrigOut2 - I - Number of patterns to display(range 1 through 256).
 *                                   If in repeat mode, then this value dictates how often TRIG_OUT_2 is generated.
 * @param   numImages - I - Number of Image Index LUT Entries(range 1 through 64).
 *                          This Field is irrelevant for Pattern Display Data Input Source set to a value other than internal.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(EXP_PAT_CONFIG);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pNumLutEntries = ((unsigned int)msg.text.data[1] << 8) + msg.text.data[0] + 1; /* +1 because the firmware gives 0-based indices (0 means 1) */
        *pNumPatsForTrigOut2 = ((unsigned int)msg.text.data[3] << 8) + msg.text.data[2] + 1; /* +1 because the firmware gives 0-based indices (0 means 1) */
        *pNumImages = msg.text.data[4]+1;     /* +1 because the firmware gives 0-based indices (0 means 1) */
        *pRepeat = (msg.text.data[5] != 0);
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_GetPatternConfig(unsigned int *pNumLutEntries, bool *pRepeat, unsigned int *pNumPatsForTrigOut2, unsigned int *pNumImages)
/**
 * (I2C: 0x75)
 * (USB: CMD2: 0x1A, CMD3: 0x31)
 * This API controls the execution of patterns stored in the lookup table.
 * Before using this API, stop the current pattern sequence using DLPC350_PatternDisplay() API
 * After calling this API, send the Validation command using the API DLPC350_ValidatePatLutData() before starting the pattern sequence
 *
 * @param   *pNumLutEntries - O - Number of LUT entries
 * @param   *pRepeat - O - 0 = execute the pattern sequence once; 1 = repeat the pattern sequnce.
 * @param   *pNumPatsForTrigOut2 - O - Number of patterns to display(range 1 through 256).
 *                                   If in repeat mode, then this value dictates how often TRIG_OUT_2 is generated.
 * @param   *pNumImages - O - Number of Image Index LUT Entries(range 1 through 64).
 *                          This Field is irrelevant for Pattern Display Data Input Source set to a value other than internal.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(PAT_CONFIG);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pNumLutEntries = msg.text.data[0] + 1; /* +1 because the firmware gives 0-based indices (0 means 1) */
        *pRepeat = (msg.text.data[1] != 0);
        *pNumPatsForTrigOut2 = msg.text.data[2]+1;    /* +1 because the firmware gives 0-based indices (0 means 1) */
        *pNumImages = msg.text.data[3]+1;     /* +1 because the firmware gives 0-based indices (0 means 1) */
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetExposure_FramePeriod(unsigned int exposurePeriod, unsigned int framePeriod)
/**
 * (I2C: 0x66)
 * (USB: CMD2: 0x1A, CMD3: 0x29)
 * The Pattern Display Exposure and Frame Period dictates the time a pattern is exposed and the frame
 * period. Either the exposure time must be equivalent to the frame period, or the exposure time must be
 * less than the frame period by 230 microseconds. Before executing this command, stop the current pattern
 * sequence. After executing this command, call DLPC350_ValidatePatLutData() API before starting the pattern sequence.
 *
 * @param   exposurePeriod - I - Exposure time in microseconds.
 * @param   framePeriod - I - Frame period in microseconds.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = exposurePeriod;
    msg.text.data[3] = exposurePeriod>>8;
    msg.text.data[4] = exposurePeriod>>16;
    msg.text.data[5] = exposurePeriod>>24;
    msg.text.data[6] = framePeriod;
    msg.text.data[7] = framePeriod>>8;
    msg.text.data[8] = framePeriod>>16;
    msg.text.data[9] = framePeriod>>24;
    DLPC350_PrepWriteCmd(&msg, PAT_EXPO_PRD);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetExposure_FramePeriod(unsigned int *pExposure, unsigned int *pFramePeriod)
/**
 * (I2C: 0x66)
 * (USB: CMD2: 0x1A, CMD3: 0x29)
 * This API reads back the exposure time and frame period settings from the controller.
 *
 * @param   exposurePeriod - O - Exposure time in microseconds.
 * @param   framePeriod - O - Frame period in microseconds.
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(PAT_EXPO_PRD);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pExposure = msg.text.data[0] | msg.text.data[1] << 8 | msg.text.data[2] << 16 | msg.text.data[3] << 24;
        *pFramePeriod = msg.text.data[4] | msg.text.data[5] << 8 | msg.text.data[6] << 16 | msg.text.data[7] << 24;
        return 0;
    }
    return -1;
}


int CLight3DTiDLP4500::DLPC350_SetTrigOutConfig(unsigned int trigOutNum, bool invert, unsigned int rising, unsigned int falling)
/**
 * (I2C: 0x6A)
 * (USB: CMD2: 0x1A, CMD3: 0x1D)
 * This API sets the polarity, rising edge delay, and falling edge delay of the DLPC350's TRIG_OUT_1 or TRIG_OUT_2 signal.
 * The delays are compared to when the pattern is displayed on the DMD. Before executing this command,
 * stop the current pattern sequence. After executing this command, call DLPC350_ValidatePatLutData() API before starting the pattern sequence.
 *
 * @param   trigOutNum - I - 1 = TRIG_OUT_1; 2 = TRIG_OUT_2
 * @param   invert - I - 0 = active High signal; 1 = Active Low signal
 * @param   rising - I - rising edge delay control. Each bit adds 107.2 ns
 *                      0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xD4 = +2.68 弮s, 0xD5 = +2.787 弮s
 * @param   falling- I - falling edge delay control. Each bit adds 107.2 ns (This field is not applcable for TRIG_OUT_2)
 *                      0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xD4 = +2.68 弮s, 0xD5 = +2.787 弮s
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    if(trigOutNum == 1)
    {
        msg.text.data[2] = invert;
        msg.text.data[3] = rising;
        msg.text.data[4] = falling;
        DLPC350_PrepWriteCmd(&msg, TRIG_OUT1_CTL);
    }
    else if(trigOutNum==2)
    {
        msg.text.data[2] = invert;
        msg.text.data[3] = rising;
        DLPC350_PrepWriteCmd(&msg, TRIG_OUT2_CTL);
    }
    else
        return -1;

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetTrigOutConfig(unsigned int trigOutNum, bool *pInvert,unsigned int *pRising, unsigned int *pFalling)
/**
 * (I2C: 0x6A)
 * (USB: CMD2: 0x1A, CMD3: 0x1D)
 * This API readsback the polarity, rising edge delay, and falling edge delay of the DLPC350's TRIG_OUT_1 or TRIG_OUT_2 signal.
 * The delays are compared to when the pattern is displayed on the DMD.
 *
 * @param   trigOutNum - I - 1 = TRIG_OUT_1; 2 = TRIG_OUT_2
 * @param   *pInvert - O - 0 = active High signal; 1 = Active Low signal
 * @param   *pRising - O - rising edge delay control. Each bit adds 107.2 ns
 *                      0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xD4 = +2.68 弮s, 0xD5 = +2.787 弮s
 * @param   *pFalling- O - falling edge delay control. Each bit adds 107.2 ns (This field is not applcable for TRIG_OUT_2)
 *                      0x00 = -20.05 弮s, 0x01 = -19.9428 弮s, ......0xBB=0.00 弮s, ......, 0xD4 = +2.68 弮s, 0xD5 = +2.787 弮s
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    if(trigOutNum == 1)
        DLPC350_PrepReadCmd(TRIG_OUT1_CTL);
    else if(trigOutNum==2)
        DLPC350_PrepReadCmd(TRIG_OUT2_CTL);
    else
        return -1;

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        if(trigOutNum == 1)
        {
            *pInvert = (msg.text.data[0] != 0);
            *pRising = msg.text.data[1];
            *pFalling = msg.text.data[2];
        }
        else
        {
            *pInvert = (msg.text.data[0] != 0);
            *pRising = msg.text.data[1];
            *pFalling = 0;
        }

        return 0;
    }
    return -1;
}


int CLight3DTiDLP4500::DLPC350_ValidatePatLutData(unsigned int *pStatus)
{
    hidMessageStruct msg;

    DLPC350_PrepWriteCmd(&msg, LUT_VALID);
    if(DLPC350_SendMsg(&msg,true) < 0)
        return -1;

    DLPC350_PrepReadCmd(LUT_VALID);

    //Poll for completion of validation
    do
    {
        if(DLPC350_Read() > 0)
        {
			unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
            memcpy(&msg, g_InputBuffer, 65);
            if(((uint8)msg.text.data[0]&0x80) == 0)
            {
                *pStatus = msg.text.data[0];
                break;
            }
            else
            {
                continue;
            }
        }
        else
            return -1;

    } while(1);

    return 0;
}


int CLight3DTiDLP4500::DLPC350_StartPatLutValidate()
/**
 * (I2C: 0x7D)
 * (USB: CMD2: 0x1A, CMD3: 0x1A)
 * This API checks the programmed pattern display modes and indicates any invalid settings.
 * This command needs to be executed after all pattern display configurations have been completed.
 *
 * @param   *pStatus - O
 *                      BIT0 = Validity of exposure or frame period settings
 *                             1 = Selected exposure or frame period settings are invalid
 *                             0 =Selected exposure or frame period settings are valid
 *                      BIT1 = Validity of pattern numbers in lookup table (LUT)
 *                             1 = Selected pattern numbers in LUT are invalid
 *                             0 = Selected pattern numbers in LUT are valid
 *                      BIT2 = Status of Trigger Out1
 *                             1 = Warning, continuous Trigger Out1 request or overlapping black sectors
 *                             0 = Trigger Out1 settings are valid
 *                      BIT3 = Status of post sector settings
 *                             1 = Warning, post vector was not inserted prior to external triggered vector
 *                             0 = Post vector settings are valid
 *                      BIT4 = Status of frame period and exposure difference
 *                             1 = Warning, frame period or exposure difference is less than 230usec
 *                             0 = Frame period or exposure difference is valid
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepWriteCmd(&msg, LUT_VALID);
    if(DLPC350_SendMsg(&msg,true) < 0)
        return -1;

    //    DLPC350_PrepReadCmd(LUT_VALID);

    //    //Poll for completion of validation
    //    do
    //    {
    //        if(DLPC350_Read() > 0)
    //        {
    //            memcpy(&msg, InputBuffer, 65);
    //            if(((uint8)msg.text.data[0]&0x80) == 0)
    //            {
    //                *pStatus = msg.text.data[0];
    //                break;
    //            }
    //            else
    //                continue;
    //        }
    //        else{
    //            return -1;
    //        }
    //    } while(1);

    return 0;
}


int CLight3DTiDLP4500::DLPC350_CheckPatLutValidate(bool *ready, unsigned int *pStatus)
/**
 * (I2C: 0x7D)
 * (USB: CMD2: 0x1A, CMD3: 0x1A)
 * This API checks the programmed pattern display modes and indicates any invalid settings.
 * This command needs to be executed after all pattern display configurations have been completed.
 *
 * @param   *pStatus - O
 *                      BIT0 = Validity of exposure or frame period settings
 *                             1 = Selected exposure or frame period settings are invalid
 *                             0 =Selected exposure or frame period settings are valid
 *                      BIT1 = Validity of pattern numbers in lookup table (LUT)
 *                             1 = Selected pattern numbers in LUT are invalid
 *                             0 = Selected pattern numbers in LUT are valid
 *                      BIT2 = Status of Trigger Out1
 *                             1 = Warning, continuous Trigger Out1 request or overlapping black sectors
 *                             0 = Trigger Out1 settings are valid
 *                      BIT3 = Status of post sector settings
 *                             1 = Warning, post vector was not inserted prior to external triggered vector
 *                             0 = Post vector settings are valid
 *                      BIT4 = Status of frame period and exposure difference
 *                             1 = Warning, frame period or exposure difference is less than 230usec
 *                             0 = Frame period or exposure difference is valid
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(LUT_VALID);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        if(((uint8)msg.text.data[0]&0x80) == 0)
        {
            *pStatus = msg.text.data[0];
            *ready = true;
        }
        else
        {
            *ready = false;
        }
    }
    else{
        return -1;
    }


    return 0;
}

int CLight3DTiDLP4500::DLPC350_SetTrigIn1Delay(unsigned int Delay)
/**
 * (I2C: 0x79)
 * (USB: CMD2: 0x1A, CMD3: 0x35)
 * This API sets the rising edge delay of the DLPC350's TRIG_OUT_1 signal compared to when the pattern is displayed on the DMD.
 * The polarity of TRIG_IN_1 is set in the lookup table of the pattern sequence.Before executing this command,
 * stop the current pattern sequence. After executing this command, call DLPC350_ValidatePatLutData() API before starting the pattern sequence.
 *
 * @param   Delay - I - rising edge delay control. 0=0ns. Each bit adds 107.2 ns
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    //BIT18:0 is Valid data
    Delay &= 0x7FFFF;

    msg.text.data[2] = Delay;
    msg.text.data[3] = Delay >> 8;
    msg.text.data[4] = Delay >> 16;
    msg.text.data[5] = Delay >> 24;
    DLPC350_PrepWriteCmd(&msg, TRIG_IN1_DELAY);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetTrigIn1Delay(unsigned int *pDelay)
/**
 * (I2C: 0x79)
 * (USB: CMD2: 0x1A, CMD3: 0x35)
 * This API reads back the rising edge delay of the DLPC350's TRIG_OUT_1 signal compared to when the pattern is displayed on the DMD.
 *
 * @param   *pDelay - O - rising edge delay control. 0=0ns. Each bit adds 107.2 ns
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(TRIG_IN1_DELAY);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pDelay = msg.text.data[0] | msg.text.data[1]<<8 | msg.text.data[2]<<16 | msg.text.data[3]<<24;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetTrigIn2Pol(bool isFallingEdge)
/**
 * (I2C: 0x7A)
 * (USB: CMD2: 0x1A, CMD3: 0x36)
 * This API sets the TRIG_IN_2 intepreation for pattern advancement.It can be configured to advace on raising or falling edge.
 * Before executing this command,stop the current pattern sequence. After executing this command, call DLPC350_ValidatePatLutData() API before starting the pattern sequence.
 * NOTE: This option is applicable only in Trigger Mode = 2.
 * @param   isFallingEdge - I - TRUE  - Advance pattern on falling edge
 *                              FALSE - Advance pattern on rising edge
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    if(isFallingEdge)
        msg.text.data[2] = 0x01;
    else
        msg.text.data[2] = 0x00;

    DLPC350_PrepWriteCmd(&msg, TRIG_IN2_CONTROL);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetTrigIn2Pol(bool *pIsFallingEdge)
/**
 * (I2C: 0x7A)
 * (USB: CMD2: 0x1A, CMD3: 0x36)
 * This API reads back the rising/falling edge configuration setting for advacing the pattern.
 *
 * @param   *pIsFallingEdge - O - TRUE: Falling edge
 *                                FALSE: Rising edge
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(TRIG_IN2_CONTROL);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        if(msg.text.data[0])
        {
            *pIsFallingEdge = true;
        }
        else
        {
            *pIsFallingEdge = false;
        }
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetInvertData(bool invert)
/**
 * (I2C: 0x74)
 * (USB: CMD2: 0x1A, CMD3: 0x30)
 * This API dictates how the DLPC350 interprets a value of 0 or 1 to control mirror position for displayed patterns.
 * Before executing this command, stop the current pattern sequence. After executing this command, call
 * DLPC350_ValidatePatLutData() API before starting the pattern sequence.
 *
 * @param   invert - I - Pattern Display Invert Data
 *                      0 = Normal operation. A data value of 1 will flip the mirror to output light,
 *                          while a data value of 0 will flip the mirror to block light
 *                      1 = Inverted operation. A data value of 0 will flip the mirror to output light,
 *                          while a data value of 1 will flip the mirror to block light
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = invert;
    DLPC350_PrepWriteCmd(&msg, INVERT_DATA);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_SetPWMConfig(unsigned int channel, unsigned int pulsePeriod, unsigned int dutyCycle)
/**
 * (I2C: 0x41)
 * (USB: CMD2: 0x1A, CMD3: 0x11)
 * This API sets the clock period and duty cycle of the specified PWM channel. The PWM
 * frequency and duty cycle is derived from an internal 18.67MHz clock. To calculate the desired PWM
 * period, divide the desired clock frequency from the internal 18.67Mhz clock. For example, a PWM
 * frequency of 2kHz, requires pulse period to be set to 18666667 / 2000 = 9333.
 *
 * @param   channel - I - PWM Channel Select
 *                      0 - PWM channel 0 (GPIO_0)
 *                      1 - Reserved
 *                      2 - PWM channel 2 (GPIO_2)
 *
 * @param   pulsePeriod - I - Clock Period in increments of 53.57ns. Clock Period = (value + 1) * 53.5ns
 *
 * @param   dutyCycle - I - Duty Cycle = (value + 1)% Value range is 1%-99%
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = channel;
    msg.text.data[3] = pulsePeriod;
    msg.text.data[4] = pulsePeriod >> 8;
    msg.text.data[5] = pulsePeriod >> 16;
    msg.text.data[6] = pulsePeriod >> 24;
    msg.text.data[7] = dutyCycle;

    DLPC350_PrepWriteCmd(&msg, PWM_SETUP);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetPWMConfig(unsigned int channel, unsigned int *pPulsePeriod, unsigned int *pDutyCycle)
/**
 * (I2C: 0x41)
 * (USB: CMD2: 0x1A, CMD3: 0x11)
 * This API reads back the clock period and duty cycle of the specified PWM channel. The PWM
 * frequency and duty cycle is derived from an internal 18.67MHz clock. To calculate the desired PWM
 * period, divide the desired clock frequency from the internal 18.67Mhz clock. For example, a PWM
 * frequency of 2kHz, requires pulse period to be set to 18666667 / 2000 = 9333.
 *
 * @param   channel - I - PWM Channel Select
 *                      0 - PWM channel 0 (GPIO_0)
 *                      1 - Reserved
 *                      2 - PWM channel 2 (GPIO_2)
 *
 * @param   *pPulsePeriod - O - Clock Period in increments of 53.57ns. Clock Period = (value + 1) * 53.5ns
 *
 * @param   *pDutyCycle - O - Duty Cycle = (value + 1)% Value range is 1%-99%
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmdWithParam(PWM_SETUP, (unsigned char)channel);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pPulsePeriod = msg.text.data[1] | msg.text.data[2] << 8 | msg.text.data[3] << 16 | msg.text.data[4] << 24;
        *pDutyCycle = msg.text.data[5];
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetPWMEnable(unsigned int channel, bool Enable)
/**
 * (I2C: 0x40)
 * (USB: CMD2: 0x1A, CMD3: 0x10)
 * After the PWM Setup command configures the clock period and duty cycle, the PWM Enable command
 * activates the PWM signals.
 *
 * @param   channel - I - PWM Channel Select
 *                      0 - PWM channel 0 (GPIO_0)
 *                      1 - Reserved
 *                      2 - PWM channel 2 (GPIO_2)
 *
 * @param   Enable - I - PWM Channel enable 0=disable; 1=enable
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    unsigned char value = 0;

    if(Enable)
        value = BIT7;

    if(channel == 2)
        value |= 2;
    else if (channel != 0)
        return -1;

    msg.text.data[2] = value;
    DLPC350_PrepWriteCmd(&msg, PWM_ENABLE);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetPWMEnable(unsigned int channel, bool *pEnable)
/**
 * (I2C: 0x40)
 * (USB: CMD2: 0x1A, CMD3: 0x10)
 * Reads back the enabled/disabled status of the given PWM channel.
 *
 * @param   channel - I - PWM Channel Select
 *                      0 - PWM channel 0 (GPIO_0)
 *                      1 - Reserved
 *                      2 - PWM channel 2 (GPIO_2)
 *
 * @param   *pEnable - O - PWM Channel enable 0=disable; 1=enable
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmdWithParam(PWM_ENABLE, (unsigned char)channel);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        if(msg.text.data[0] & BIT7)
            *pEnable =  true;
        else
            *pEnable = false;

        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetPWMCaptureConfig(unsigned int channel, bool enable, unsigned int sampleRate)
/**
 * (I2C: 0x43)
 * (USB: CMD2: 0x1A, CMD3: 0x12)
 * This API samples the specified PWM input signals and returns the PWM clock period.
 *
 * @param   channel - I - PWM Capture Port
 *                      0 - PWM input channel 0 (GPIO_5)
 *                      1 - PWM input channel 1 (GPIO_6)
 *
 * @param   enable - I - PWM Channel enable 0=disable; 1=enable
 *
 * @param   sampleRate - I - PWM Sample Rate (285 Hz to 18,666,667 Hz) - Sample Rate = Pulse Frequency / Duty Cycle
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    unsigned char value = 0;

    value = channel & 1;

    if(enable)
        value |= BIT7;

    msg.text.data[2] = value;
    msg.text.data[3] = sampleRate;
    msg.text.data[4] = sampleRate >> 8;
    msg.text.data[5] = sampleRate >> 16;
    msg.text.data[6] = sampleRate >> 24;
    DLPC350_PrepWriteCmd(&msg, PWM_CAPTURE_CONFIG);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetPWMCaptureConfig(unsigned int channel, bool *pEnabled, unsigned int *pSampleRate)
/**
 * (I2C: 0x43)
 * (USB: CMD2: 0x1A, CMD3: 0x12)
 * This API reads back the configuration of the specified PWM capture channel.
 *
 * @param   channel - I - PWM Capture Port
 *                      0 - PWM input channel 0 (GPIO_5)
 *                      1 - PWM input channel 1 (GPIO_6)
 *
 * @param   *pEnabled - O - PWM Channel enable 0=disable; 1=enable
 *
 * @param   *pSampleRate - O - PWM Sample Rate (285 Hz to 18,666,667 Hz) - Sample Rate = Pulse Frequency / Duty Cycle
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmdWithParam(PWM_CAPTURE_CONFIG, (unsigned char)channel);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        if(msg.text.data[0] & BIT7)
            *pEnabled =  true;
        else
            *pEnabled = false;

        *pSampleRate = msg.text.data[1] | msg.text.data[2] << 8 | msg.text.data[3] << 16 | msg.text.data[4] << 24;

        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_PWMCaptureRead(unsigned int channel, unsigned int *pLowPeriod, unsigned int *pHighPeriod)
/**
 * (I2C: 0x4E)
 * (USB: CMD2: 0x1A, CMD3: 0x13)
 * This API returns both the number of clock cycles the signal was low and high.
 *
 * @param   channel - I - PWM Capture Port
 *                      0 - PWM input channel 0 (GPIO_5)
 *                      1 - PWM input channel 1 (GPIO_6)
 *
 * @param   *pLowPeriod - O - indicates how many samples were taken during a low signal
 *
 * @param   *pHighPeriod - O - indicates how many samples were taken during a high signal
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmdWithParam(PWM_CAPTURE_READ, (unsigned char)channel);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pLowPeriod = msg.text.data[1] | msg.text.data[2] << 8;
        *pHighPeriod = msg.text.data[3] | msg.text.data[4] << 8;
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetGPIOConfig(unsigned int pinNum, bool enAltFunc, bool altFunc1, bool dirOutput, bool outTypeOpenDrain, bool pinState)
/**
 * (I2C: 0x44)
 * (USB: CMD2: 0x1A, CMD3: 0x38)
 *
 * This API enables GPIO functionality on a specific set of DLPC350 pins. The
 * command sets their direction, output buffer type, and output state.
 *
 * @param   pinNum - I - GPIO selection. See Table 2-38 in the programmer's guide for description of available pins
 *
 * @param   enAltFunc - I - 0=disable alternative function; enable GPIO; 1=enable alternative function; disable GPIO
 *
 * @param   altFunc1 - I - must be set to false
 *
 * @param   dirOutput - I - 0=input; 1=output
 *
 * @param   outTypeOpenDrain - I - 0=Standard buffer (drives high or low); 1=open drain buffer (drives low only)
 *
 * @param   pinState - I - 0=LOW; 1=HIGH
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;
    unsigned char value = 0;

    if(enAltFunc)
        value |= BIT7;
    if(altFunc1)
        value |= BIT6;
    if(dirOutput)
        value |= BIT5;
    if(outTypeOpenDrain)
        value |= BIT4;
    if(pinState)
        value |= BIT3;

    msg.text.data[2] = pinNum;
    msg.text.data[3] = value;
    DLPC350_PrepWriteCmd(&msg, GPIO_CONFIG);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetGPIOConfig(unsigned int pinNum, bool *pEnAltFunc, bool *pAltFunc1, bool *pDirOutput, bool *pOutTypeOpenDrain, bool *pState)
/**
 * (I2C: 0x44)
 * (USB: CMD2: 0x1A, CMD3: 0x38)
 *
 * This API reads back the GPIO configuration on a specific set of DLPC350 pins. The
 * command reads back their direction, output buffer type, and  state.
 *
 * @param   pinNum - I - GPIO selection. See Table 2-38 in the programmer's guide for description of available pins
 *
 * @param   *pEnAltFunc - O - 0=disable alternative function; enable GPIO; 1=enable alternative function; disable GPIO
 *
 * @param   *pAltFunc1 - O - must be set to false
 *
 * @param   *pDirOutput - O - 0=input; 1=output
 *
 * @param   *pOutTypeOpenDrain - O - 0=Standard buffer (drives high or low); 1=open drain buffer (drives low only)
 *
 * @param   *pState - O - 0=LOW; 1=HIGH
 *
 * @return  0 = PASS    <BR>
 *          -1 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmdWithParam(GPIO_CONFIG, (unsigned char)pinNum);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pEnAltFunc = ((msg.text.data[1] & BIT7) == BIT7);
        *pAltFunc1 = ((msg.text.data[1] & BIT6) == BIT6);
        *pDirOutput = ((msg.text.data[1] & BIT5) == BIT5);
        *pOutTypeOpenDrain = ((msg.text.data[1] & BIT4) == BIT4);
        if(*pDirOutput)
            *pState = ((msg.text.data[1] & BIT3) == BIT3);
        else
            *pState = ((msg.text.data[1] & BIT2) == BIT2);
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetGeneralPurposeClockOutFreq(unsigned int clkId, bool enable, unsigned int clkDivider)
/**
 * (I2C: 0x48)
 * (USB: CMD2: 0x08, CMD3: 0x07)
 *
 * DLPC350 supports two pins with clock output capabilities: GPIO_11 and GPIO_12.
 * This API enables the clock output functionality and sets the clock frequency.
 *
 * @param   clkId - I - Clock selection. 1=GPIO_11; 2=GPIO_12
 *
 * @param   enable - I - 0=disable clock functionality on selected pin; 1=enable clock functionality on selected pin
 *
 * @param   clkDivider - I - Allowed values in the range of 2 to 127. Output frequency = 96MHz / (Clock Divider)
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = clkId;
    msg.text.data[3] = enable;
    msg.text.data[4] = clkDivider;
    DLPC350_PrepWriteCmd(&msg, GPCLK_CONFIG);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetGeneralPurposeClockOutFreq(unsigned int clkId, bool *pEnabled, unsigned int *pClkDivider)
/**
 * (I2C: 0x48)
 * (USB: CMD2: 0x08, CMD3: 0x07)
 *
 * DLPC350 supports two pins with clock output capabilities: GPIO_11 and GPIO_12.
 * This API reads back the clock output enabled status and the clock frequency.
 *
 * @param   clkId - I - Clock selection. 1=GPIO_11; 2=GPIO_12
 *
 * @param   *pEnabled - O - 0=disable clock functionality on selected pin; 1=enable clock functionality on selected pin
 *
 * @param   *pClkDivider - O - Allowed values in the range of 2 to 127. Output frequency = 96MHz / (Clock Divider)
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmdWithParam(GPCLK_CONFIG, (unsigned char)clkId);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pEnabled = (msg.text.data[0] != 0);
        *pClkDivider = msg.text.data[1];
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_SetLEDPWMInvert(bool invert)
/**
 * (I2C: 0x0B)
 * (USB: CMD2: 0x1A, CMD3: 0x05)
 *
 * This API sets the polarity of all LED PWM signals. This API must be called before powering up the LED drivers.
 *
 * @param   invert - I - 0 = Normal polarity, PWM 0 value corresponds to no current while PWM 255 value corresponds to maximum current
 *                       1 = Inverted polarity. PWM 0 value corresponds to maximum current while PWM 255 value corresponds to no current.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = invert;
    DLPC350_PrepWriteCmd(&msg, PWM_INVERT);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_GetLEDPWMInvert(bool *inverted)
/**
 * (I2C: 0x0B)
 * (USB: CMD2: 0x1A, CMD3: 0x05)
 *
 * This API reads the polarity of all LED PWM signals.
 *
 * @param   invert - O - 0 = Normal polarity, PWM 0 value corresponds to no current while PWM 255 value corresponds to maximum current
 *                       1 = Inverted polarity. PWM 0 value corresponds to maximum current while PWM 255 value corresponds to no current.
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(PWM_INVERT);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *inverted = (msg.text.data[0] != 0);
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_MemRead(unsigned int addr, unsigned int *readWord)
/**
 *
 * This API reads back the content at a specified memory location from the controller.
 *
 * @param   addr - I - address from which to read contents
 *
 * @param   readWord - O - 32-bit word read from the given address
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    DLPC350_PrepMemReadCmd(addr);
    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *readWord = msg.text.data[0] | msg.text.data[1] << 8 | msg.text.data[2] << 16 | msg.text.data[3] << 24;
        //*readWord = msg.text.data[3] | msg.text.data[2] << 8 | msg.text.data[1] << 16 | msg.text.data[0] << 24; //MSB first
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_MemWrite(unsigned int addr, unsigned int data)
/**
 *
 * This API writes the given content at a specified memory location from the controller.
 *
 * @param   addr - I - address to write to
 *
 * @param   data - I - 32-bit word to be written at given address
 *
 * @return  >=0 = PASS    <BR>
 *          <0 = FAIL  <BR>
 *
 */
{
    hidMessageStruct msg;

    msg.text.data[2] = 0; //absolute write
    msg.text.data[6] = addr >> 24;
    msg.text.data[5] = addr >> 16;
    msg.text.data[4] = addr >> 8;
    msg.text.data[3] = addr;  //LSB first
    msg.text.data[10] = data >> 24;
    msg.text.data[9] = data >> 16;
    msg.text.data[8] = data >> 8;
    msg.text.data[7] = data;  //LSB first
    DLPC350_PrepWriteCmd(&msg, MEM_CONTROL);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_MeasureImageLoadTiming(unsigned int startIndex, unsigned int numFlash)
/**
  * This API instructs the controller to measure the load time for the image(s) stored starting at specified index.
  * The result of measuement can be read back using the DLPC350_ReadImageLoadTiming() API.
  *
  * @param   startIndex - I - index of the first image whose load time is to be measured
  *
  * @param   numFlash - I - number of images for which load time is to be measured.
  *
  * @return  >=0 = PASS    <BR>
  *          <0 = FAIL  <BR>
  *
  */
{
    hidMessageStruct msg;

    msg.text.data[2] = startIndex;
    msg.text.data[3] = numFlash;
    DLPC350_PrepWriteCmd(&msg, IMAGE_LOAD_TIMING);

    return DLPC350_SendMsg(&msg,true);
}

int CLight3DTiDLP4500::DLPC350_ReadImageLoadTiming(unsigned int *pTimingData)
/**
  * This API reads back the mesasured load time for the image specified by DLPC350_MeasureImageLoadTiming() API.
  *
  * @param   *pTimingData - I - time taken to load the specified image in milliseconds = value/18667.
  *
  * @return  >=0 = PASS    <BR>
  *          <0 = FAIL  <BR>
  *
  */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(IMAGE_LOAD_TIMING);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pTimingData = (msg.text.data[0] | msg.text.data[1] << 8 | msg.text.data[2] << 16 | msg.text.data[3] << 24);
        return 0;
    }
    return -1;
}

int CLight3DTiDLP4500::DLPC350_I2C0WriteData(bool is7Bit, unsigned int sclClk, unsigned int devAddr, unsigned int numWriteBytes, unsigned char *pWData)
/**
  * This API makes a I2C Master write transcation via the I2C0 port of DLPC350.
  *
  * @param   is7Bit - I - true - For 7bit device addressing
  *                       false - For 10bit device addressing
  *
  * @param   sclClk - I - I2C master clock setting 18194 to 400000
  *
  * @param   devAddress - I - I2C slave device address to which message to be sent.
  *
  * @param   numWriteBytes  - I - Number of data bytes to be sent in write transcation
  *
  * @param   *pWdata - I - pointer to data bytes array to be sent
  *
  *
  * @return  >=0 = PASS    <BR>
  *          <0 = FAIL  <BR>
  */
{
    unsigned int i;
    hidMessageStruct msg;

    msg.text.data[2] = (is7Bit == true) ? 0x00 : 0x01;
    msg.text.data[3] = sclClk;  //LSB first
    msg.text.data[4] = sclClk >> 8;
    msg.text.data[5] = sclClk >> 16;
    msg.text.data[6] = sclClk >> 24;
    //RFU
    msg.text.data[7] = 0x00;
    msg.text.data[8] = 0x00;
    msg.text.data[9] = 0x00;
    msg.text.data[10] = 0x00;

    msg.text.data[11] = devAddr;  //LSB first
    msg.text.data[12] = devAddr >> 8;
    msg.text.data[13] = numWriteBytes;  //LSB first
    msg.text.data[14] = numWriteBytes >> 8;

    for(i=0;i<numWriteBytes;i++)
        msg.text.data[15+i] = pWData[i];

    m_seqNum = 0;
    DLPC350_PrepWriteCmd(&msg, I2C0_CTRL);
    msg.head.length += (13 + numWriteBytes);
    if(DLPC350_SendMsg(&msg,true) < 0)
        return -1;

    return 0;
}

int CLight3DTiDLP4500::DLPC350_I2C0ReadData(bool is7Bit, unsigned int sclClk, unsigned int devAddr, unsigned int numWriteBytes, unsigned int numReadBytes, unsigned char *pWData, unsigned char *pRdata)
/**
  * This API makes a I2C Master write transcation via the I2C0 port of DLPC350.
  *
  * @param   is7Bit - I - true - For 7bit device addressing
  *                       false - For 10bit device addressing
  *
  * @param   sclClk - I - I2C master clock setting 18194 to 400000
  *
  * @param   devAddress - I - I2C slave device address to which message to be sent.
  *
  * @param   numWriteBytes  - I - Number of data bytes to be sent as part of write before reading the response
  *
  * @param   numReadBytes  - I - Number of data bytes to be read as part of read transcation
  *
  * @param   *pWdata - I - pointer to data bytes array to be sent
  *
  * @param   *pRdata - I - pointer to data bytes array where the response is copied
  *
  * @return  >=0 = PASS    <BR>
  *          <0 = FAIL  <BR>
  */
{
    unsigned int i;
    unsigned int tmpUIntVar;

    hidMessageStruct msg;
    msg.head.flags.rw = 1; //Read
    msg.head.flags.reply = 1; //Host wants a reply from device
    msg.head.flags.dest = 0; //Projector Control Endpoint
    msg.head.flags.reserved = 0;
    msg.head.flags.nack = 0;
    msg.head.seq = 0;
    msg.text.cmd = (CmdList[I2C0_CTRL].CMD2 << 8) | CmdList[I2C0_CTRL].CMD3;

    msg.text.data[2] = (is7Bit == true) ? 0x00 : 0x01;
    msg.text.data[3] = sclClk;  //LSB first
    msg.text.data[4] = sclClk >> 8;
    msg.text.data[5] = sclClk >> 16;
    msg.text.data[6] = sclClk >> 24;

    //RFU
    msg.text.data[7] = 0x00;
    msg.text.data[8] = 0x00;
    msg.text.data[9] = 0x00;
    msg.text.data[10] = 0x00;

    msg.text.data[11] = devAddr;  //LSB first
    msg.text.data[12] = devAddr >> 8;
    msg.text.data[13] = numWriteBytes;  //LSB first
    msg.text.data[14] = numWriteBytes >> 8;
    msg.text.data[15] = numReadBytes;  //LSB first
    msg.text.data[16] = numReadBytes >> 8;

    //Copy number of bytes to written before reading
    for(i=0;i<numWriteBytes;i++)
    {
        msg.text.data[17+i] = pWData[i];
    }
    msg.head.length = (2 + 15 + numWriteBytes);

    int maxDataSize = USB_MAX_PACKET_SIZE-sizeof(msg.head);
    int dataBytesSent = MIN(msg.head.length, maxDataSize);

	unsigned char* g_OutputBuffer = m_TiUSB.GetOutputBuffer();
    g_OutputBuffer[0]=0; // First byte is the report number
    memcpy(&g_OutputBuffer[1], &msg, (sizeof(msg.head) + dataBytesSent));

    //Check if it is single or multiple packet transaction
    if(dataBytesSent < msg.head.length)
    {
        //Send first packet
        if(m_TiUSB.USB_Write() < 0)
            return -1;

        //Send all intermediate packets
        while(dataBytesSent < msg.head.length)
        {
            memcpy(&g_OutputBuffer[1], &msg.text.data[dataBytesSent], USB_MAX_PACKET_SIZE);

            if((dataBytesSent+USB_MAX_PACKET_SIZE) >= (msg.head.length))
            {
                //last packet
                break;
            }
            else
            {
                if(m_TiUSB.USB_Write() < 0)
                    return -1;
            }

            dataBytesSent += USB_MAX_PACKET_SIZE;
        }
    }

    //Begin reading the response

    tmpUIntVar = numReadBytes;

    if(DLPC350_Read() < 0)
        return -1;

	unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
    hidMessageStruct *pMsg = (hidMessageStruct *)g_InputBuffer;
    if(pMsg != NULL)
    {
        memcpy(pRdata, g_InputBuffer+sizeof(pMsg->head), MIN((unsigned int)tmpUIntVar,64-sizeof(pMsg->head)));
        pRdata+= (64-sizeof(pMsg->head));
        if(tmpUIntVar > 64)
            tmpUIntVar -= (64-sizeof(pMsg->head));
        else
           tmpUIntVar = 0;
    }

    /* If number of bytes to be read in response is > 64 bytes */
    while(tmpUIntVar > 0)
    {
        DLPC350_ContinueRead();
        memcpy(pRdata, g_InputBuffer, MIN(tmpUIntVar,64));
        pRdata+=64;
        tmpUIntVar -= 64;
    }

    return 0;
}

int CLight3DTiDLP4500::DLPC350_I2C0TranStat(unsigned char *pStat)
/**
  * This API returns I2C0 Master write transcation status on I2C0 port of DLPC350.
  *
  * @param   *pStats - I - pointer to the status
  *                        0x00 - NO Error occured
  *                        0x01 - NO ACK Received from slave
  *                        0x02 - Arbitration lost
  *                        0x04 - Write timeout error
  *                        0x08 - Read timeout error
  *                        0x10 - Reserved
  *                        0x20 - I2C0 Internal error occurred
  *
  * @return  >=0 = PASS    <BR>
  *          <0 = FAIL  <BR>
  */
{
    hidMessageStruct msg;

    DLPC350_PrepReadCmd(I2C0_STAT);

    if(DLPC350_Read() > 0)
    {
		unsigned char* g_InputBuffer = m_TiUSB.GetInputBuffer();
        memcpy(&msg, g_InputBuffer, 65);
        *pStat = msg.text.data[0];
        return 0;
    }

    return -1;
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_4500_USE
//-------------------------------------------------------------------------------------//