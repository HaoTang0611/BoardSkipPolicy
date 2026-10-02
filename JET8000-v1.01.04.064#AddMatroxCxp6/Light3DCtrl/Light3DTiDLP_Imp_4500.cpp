// Light3DTiDLP_Imp_4500.cpp: implementation of the CLight3DTiDLP_Imp_4500 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP.h"
#include "Light3DTiDLP_Imp_4500.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//
bool  CLight3DTiDLP::CreateImp_DLP4500(int CtrlID, LIGHT_3D_CAST_ID CastID)//建立Imp的指標
{
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4500
	m_Imp = new CLight3DTiDLP_Imp_4500(CtrlID, CastID);
#endif//LIGHT_3D_TI_DLP_USE_IMP_4500
	if ( NULL == m_Imp )
	{
		m_ErrorString = _T("Error, CLight3DTiDLP::CreateImp_DLP4500 Fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
#define DLP_LED_CURRENT_MAX				  255		//最高亮度
#define DLP_LED_CURRENT_MIN					0		//最低亮度
//-------------------------------------------------------------------------------------//
#define DLPC350_BIN_VERSION_2_0_0                   0x00200000
#define DLPC350_BIN_VERSION_3_0_0                   0x00300000
#define DLPC350_BIN_VERSION_4_0_0                   0x00400000
//-------------------------------------------------------------------------------------//
#include "DLPC350_3_1_0\\dlpc350_version.h"
#include "DLPC350_3_1_0\\dlpc350_firmware.h"
//-------------------------------------------------------------------------------------//
#define DLP_PATTERN_INDEX_MODE_WAVE     1
#define DLP_PATTERN_INDEX_MODE_GC_BC    2
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4500::CLight3DTiDLP_Imp_4500():CLight3DTiDLP_Imp()
{
	PreInitTiDlp(0, LIGHT_3D_CAST_00);	
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4500::CLight3DTiDLP_Imp_4500(int CtrlID, LIGHT_3D_CAST_ID CastID):CLight3DTiDLP_Imp(CtrlID, CastID)
{
	PreInitTiDlp(CtrlID, CastID);
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4500::~CLight3DTiDLP_Imp_4500()
{
	DLPDisconnect();	
	m_TiUSB.USB_Exit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID CastID)
{
	CLight3DTiDLP_Imp::PreInitTiDlp(CtrlID, CastID);
	
	SetDLPImageW(912);
	SetDLPImageH(1140);
	SetDeviceType(LIGHT_3D_DEVICE_DLP4500);	

	m_HWStatus = 0;
	m_SysStatus = 0;
	m_MainStatus = 0;	

	m_seqNum = 0;
	m_numImgInFlash = 0;
	m_PatLutIndex = 0;		
	m_ExpLutIndex = 0;
	::memset(m_PatLut, 0x00, sizeof(m_PatLut));	
	::memset(m_ExpLut, 0x00, sizeof(m_ExpLut));		

	m_EnableTemperatureMonitor = true;

	m_TiUSB.USB_Init();
	return;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::InitialTiDlp()
{
	CLight3DTiDLP_Imp::InitialTiDlp();

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
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[])
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
bool CLight3DTiDLP_Imp_4500::DLPConnect()
{
	SaveDLPProcess(_T("DLPConnect"), MSG_LEVEL_HIGH);

	bool SLmode=0;		
    unsigned int API_ver, App_ver, SWConfig_ver, SeqConfig_ver;
    unsigned int FW_ver;
	const int DLPID = 0;
	wchar_t USB_Number[256]={0};
	const LIGHT_3D_CAST_ID CastID = GetCastID();	

    if( m_TiUSB.USB_IsConnected()==1 )
	{	this->DLPDisconnect();	}
	
	if( GetUSB_Number(CastID, USB_Number) == false)
	{
		SetAOIExceptionCode(AOI_EXCEPTION_DLP_CTRL_CONNECT);
		return false;	
	}
	//::wsprintfW(USB_Number, L"LCR2");    
	if ( m_TiUSB.USB_Open(USB_Number) < 0 ) 
	{
		SetAOIExceptionCode(AOI_EXCEPTION_DLP_CTRL_CONNECT);
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault"));
		return false; 
	}	

    // Display GUI Version #
    if (DLPC350_GetVersion(&App_ver, &API_ver, &SWConfig_ver, &SeqConfig_ver) == 0)
    {	sprintf(m_TiAPIversion, "%d.%d.%d", (API_ver >> 24), ((API_ver << 8) >> 24), ((API_ver << 16) >> 16));	}

	sprintf(m_TiAPIversion, "%d.%d.%d", GUI_VERSION_MAJOR, GUI_VERSION_MINOR, GUI_VERSION_BUILD);
	if ( API_ver >= 0x03000000 )
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
	ResetLEDDisable(ResetFinish, ShowMsg);

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
			if ( DLPC350_GetPowerMode(&Standby) == 0 )
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
bool CLight3DTiDLP_Imp_4500::DLPDisconnect(int WaitTime_ms)
{
	ExecDLPPattern_Stop();

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
bool CLight3DTiDLP_Imp_4500::GetDLPIsConnected()
{
	if ( m_TiUSB.USB_IsConnected() == false )
	{
		this->m_ErrorString.Format(_T("Error, TiDLP did not connect"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::CheckDLPFrmForExpLut()//確認DLP韌體支援Exposure Lut
{
	if ( m_dwFrmVersion < DLPC350_BIN_VERSION_3_0_0 )
	{
		CString strFrm = m_DLPFrmversion;
		m_ErrorString.Format(_T("Error, TiDLP Firmware does not support Exp. Lut (Frm:%s)."), strFrm);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ExecDLPSoftwareReset(DWORD delayTime)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }
	
	SaveDLPProcess(_T("ExecDLPSoftwareReset"), MSG_LEVEL_HIGH);

	DLPC350_SoftwareReset();
	m_TiUSB.USB_Close();	
	::Sleep(delayTime);//暫停10秒
	wchar_t USB_Number[256] = { 0 };
	const LIGHT_3D_CAST_ID CastID = GetCastID();	
	if ( GetUSB_Number(CastID, USB_Number) == false )
	{
		SetAOIExceptionCode(AOI_EXCEPTION_DLP_CTRL_CONNECT);
		return false;	
	}

	if (m_TiUSB.USB_Open(USB_Number) < 0 )
	{
		SetAOIExceptionCode(AOI_EXCEPTION_DLP_CTRL_CONNECT);
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault"));
		return false;
	}
	this->ReadDLPParameter();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::SetDLPLongAxisImageFlip(bool Flip)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPLongAxisImageFlip"), MSG_LEVEL_HIGH);

	if ( DLPC350_SetLongAxisImageFlip(Flip) == -1 ) 
	{
		SetLCRErrorFnName(_T("DLPC350_SetLongAxisImageFlip"));		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPLongAxisImageFlip()
{
	return DLPC350_GetLongAxisImageFlip();
}
//-------------------------------------------------------------------------------------//	
bool CLight3DTiDLP_Imp_4500::SetDLPShortAxisImageFlip(bool Flip)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPShortAxisImageFlip"), MSG_LEVEL_HIGH);

	if( DLPC350_SetShortAxisImageFlip(Flip) == -1 ) 
	{
		SetLCRErrorFnName(_T("DLPC350_SetShortAxisImageFlip"));		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPShortAxisImageFlip()
{
	return DLPC350_GetShortAxisImageFlip();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::SetDLPOperationMode(int Mode)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	bool SLMode = true;
	bool Standby = false;
	int  OperMode=m_OperationMode;
	SaveDLPProcess(_T("SetDLPOperationMode"), MSG_LEVEL_HIGH);
	switch ( Mode )
	{
	case DLP_OPERATION_PATTERN:
	case DLP_OPERATION_PATTERN_EXP:
		SLMode = true;
		Standby = false;
		if ( DLPC350_SetPowerMode(Standby) < 0 ) 
		{	return false; }
		if ( DLPC350_SetMode(SLMode) < 0 ) 
		{	return false; }
		OperMode = Mode;
		break;
	case DLP_OPERATION_VIDEO:
		SLMode = false;
		Standby = false;
		if ( this->DLPC350_SetPowerMode(Standby) < 0 ) 
		{	return false; }
		if ( this->DLPC350_SetMode(SLMode) < 0 ) 
		{	return false; }
		OperMode = Mode;
		break;
	case DLP_OPERATION_STANDBY:
		SLMode = false;
		Standby = true;
		if ( this->DLPC350_SetMode(SLMode) < 0 ) 
		{	return false; }
		if ( this->DLPC350_SetPowerMode(Standby) < 0 ) 
		{	return false; }
		OperMode = Mode;
		break;
	}	
	m_OperationMode=OperMode;
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetDLPOperationMode() const
{
	return m_OperationMode;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	if ( m_LEDEnabled_R!=bRed || m_LEDEnabled_G!=bGreen || m_LEDEnabled_B!=bBlue || m_LEDEnabled_Auto!=bSeqCtrl )
	{
		SaveDLPProcess(_T("SetDLPLEDEnable"), MSG_LEVEL_HIGH);
		if ( DLPC350_SetLedEnables(bSeqCtrl, bRed, bGreen, bBlue) < 0 ) 
		{	
			SetLCRErrorFnName(_T("DLPC350_SetLedEnables"));		
			return false; 
		}
	}

	m_LEDEnabled_R = bRed;
	m_LEDEnabled_G = bGreen;
	m_LEDEnabled_B = bBlue;
	m_LEDEnabled_Auto = bSeqCtrl;
	m_DLPParam.m_LEDEnabled_Auto = bSeqCtrl;
	m_DLPParam.m_LEDEnabled_R = bRed;
	m_DLPParam.m_LEDEnabled_G = bGreen;
	m_DLPParam.m_LEDEnabled_B = bBlue;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	if ( DLPC350_GetLedEnables(&bSeqCtrl, &bRed, &bGreen, &bBlue) == -1 )  
	{
		SetLCRErrorFnName(_T("DLPC350_GetLedEnables"));		
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CLight3DTiDLP_Imp_4500::SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	if ( m_LEDCurrentR!=red || m_LEDCurrentG!=green || m_LEDCurrentB!=blue )
	{	
		SaveDLPProcess(_T("SetDLPLEDCurrent"), MSG_LEVEL_HIGH);
		unsigned char r = static_cast<unsigned char>(GetDLPSafeCurrent(red));
		unsigned char g = static_cast<unsigned char>(GetDLPSafeCurrent(green));
		unsigned char b = static_cast<unsigned char>(GetDLPSafeCurrent(blue));
		//注意要反向!?
		r = 255-r;
		g = 255-g;
		b = 255-b;
		if ( DLPC350_SetLedCurrents(r, g, b) < 0 ) 
		{		
			m_ErrorString.Format(_T("Error, TiDLP DLPC350_SetLedCurrents Fault [R:%d, G:%d, B:%d]"), red, green, blue);
			return false; 
		}	
	}
	
	m_LEDCurrentR = red;
	m_LEDCurrentG = green;
	m_LEDCurrentB = blue;
	//if ( true == Update )//要此過濾再校正電流會有問題
	{	SetDLPParamLEDCurrent(red, green, blue, CurrentID);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPLEDCurrent(int &red, int &green, int &blue)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	unsigned char r=0, g=0, b=0;
    if ( DLPC350_GetLedCurrents(&r, &g, &b) == -1)  
	{
		SetLCRErrorFnName(_T("DLPC350_GetLedCurrents"));		
		return false;
	}	
	red = 255-r;
	green = 255-g;
	blue = 255-b;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::SetDLPLEDPWMInvert(bool bInvert)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	if ( m_InvertPWM != bInvert )
	{
		SaveDLPProcess(_T("SetDLPLEDPWMInvert"), MSG_LEVEL_HIGH);

		if ( DLPC350_SetLEDPWMInvert(bInvert) < 0 ) 
		{	
			SetLCRErrorFnName(_T("DLPC350_SetLEDPWMInvert"));		
			return false; 
		}
	}
	m_InvertPWM = bInvert;
	m_DLPParam.m_InvertPWM = bInvert;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPLEDPWMInvert(bool &bInvert)
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

    if ( DLPC350_GetLEDPWMInvert(&bInvert) == -1 )
	{
		SetLCRErrorFnName(_T("DLPC350_GetLEDPWMInvert"));		
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::CheckDLPStatus()
{
	if ( GetDLPIsConnected() == false )
	{	return false; }

	if ( DLPC350_GetStatus(&m_HWStatus, &m_SysStatus, &m_MainStatus) == -1) 
	{ 
		SetLCRErrorFnName(_T("DLPC350_GetStatus"));		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPStatus_InitDone()
{
	return bool(m_HWStatus&BIT0);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPStatus_ForcedSwap()
{
	if ( (m_HWStatus&BIT3) == 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPStatus_BufferFreeze()
{
	if ( (m_MainStatus&BIT2) == 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPStatus_SeqRunning()
{
	if ( (m_MainStatus&BIT1) == 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPStatus_SeqError()
{
	if ( (m_HWStatus&BIT7) == 0 ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPStatus_SeqAbort()
{
	if ( (m_HWStatus&BIT6) == 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPStatus_DRCError()
{
	if ( (m_HWStatus&BIT2) == 0 ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetDLPStatus_DMDParked()
{
	if ( (m_MainStatus&BIT0) == 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ReadDLPParameter()//從DLP裝置讀取參數
{
#ifndef PHASE_CTRL_DISABLE
	if ( GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ReadDLPParameter"), MSG_LEVEL_HIGH);

	//重新取回參數
	bool SLmode=false;	
	m_OperationMode = DLP_OPERATION_DEFAULT;
	if ( DLPC350_GetMode(&SLmode) < 0  )
	{
		SetLCRErrorFnName(_T("DLPC350_GetMode"));		
		return false;
	}
	if ( SLmode == true )
	{	m_OperationMode = DLP_OPERATION_PATTERN;	}
	else
	{	m_OperationMode = DLP_OPERATION_VIDEO;	}		
	if ( DLPC350_SetPowerMode(false) < 0 )
	{
		SetLCRErrorFnName(_T("DLPC350_SetPowerMode"));		
		return false;
	}

	bool bSeqCtrl = false;
	bool bRed=false, bGreen=false, bBlue=false;
	if ( DLPC350_GetLedEnables(&bSeqCtrl, &bRed, &bGreen, &bBlue) < 0 )
	{
		SetLCRErrorFnName(_T("DLPC350_GetLedEnables"));		
		return false;
	}
	m_DLPParam.m_LEDEnabled_Auto = bSeqCtrl;
	m_DLPParam.m_LEDEnabled_R = bRed;
	m_DLPParam.m_LEDEnabled_G = bGreen;
	m_DLPParam.m_LEDEnabled_B = bBlue;

	unsigned char cRed=0, cGreen=0, cBlue=0;	
    if ( DLPC350_GetLedCurrents(&cRed, &cGreen, &cBlue) < 0 )
	{
		SetLCRErrorFnName(_T("DLPC350_GetLedCurrents"));		
		return false;
	}

	m_LEDCurrentR = 255-cRed;
	m_LEDCurrentG = 255-cGreen;
	m_LEDCurrentB = 255-cBlue;

	//m_DLPParam.m_LEDCurrentR_1 = 255-cRed;
	//m_DLPParam.m_LEDCurrentG_1 = 255-cGreen;
	//m_DLPParam.m_LEDCurrentB_1 = 255-cBlue;
	
	bool bInvert=false;
	if ( DLPC350_GetLEDPWMInvert(&bInvert) < 0 ) 
	{
		SetLCRErrorFnName(_T("DLPC350_GetLEDPWMInvert"));		
		return false;
	}
	m_InvertPWM = bInvert;
	m_DLPParam.m_InvertPWM = bInvert;
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternBitCount() const//取得樣板圖位元數
{
	return m_PatternBitCount;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternBitCount(int val)//設定樣板圖位元數
{
	m_PatternBitCount = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternIndex1_3Bit() const//取得使用3Bit樣板引數-1
{
	return m_PatternIndex1_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternIndex1_3Bit(int val)//設定使用3Bit樣板引數-1
{
	m_PatternIndex1_3Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternIndex2_3Bit() const//取得使用3Bit樣板引數-2
{
	return m_PatternIndex2_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternIndex2_3Bit(int val)//設定使用3Bit樣板引數-2
{
	m_PatternIndex2_3Bit = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternIndex1_5Bit() const//取得使用5Bit樣板引數-1
{
	return m_PatternIndex1_5Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternIndex1_5Bit(int val)//設定使用5Bit樣板引數-1
{
	m_PatternIndex1_5Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternIndex1_6Bit() const//取得使用6Bit樣板引數-1
{
	return m_PatternIndex1_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternIndex1_6Bit(int val)//設定使用6Bit樣板引數-1
{
	m_PatternIndex1_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternIndex2_6Bit() const//取得使用6Bit樣板引數-2
{
	return m_PatternIndex2_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternIndex2_6Bit(int val)//設定使用6Bit樣板引數-2
{
	m_PatternIndex2_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternIndexGC_1Bit() const//取得使用1Bit-GrayCode引數-1
{
	return m_PatternIndexGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternIndexGC_1Bit(int val)//設定使用1Bit-GrayCode引數-1
{
	m_PatternIndexGC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternIndexBC_1Bit() const//取得使用1Bit-BinaryCode引數-1
{
	return m_PatternIndexBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternIndexBC_1Bit(int val)//設定使用1Bit-BinaryCode引數-1
{
	m_PatternIndexBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternStartNumGC_1Bit() const//取得使用1Bit-GrayCode起始張數-1
{
	return m_PatternStartNumGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternStartNumGC_1Bit(int val)//設定使用1Bit-GrayCode起始張數-1
{
	m_PatternStartNumGC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500::GetPatternStartNumBC_1Bit() const//取得使用1Bit-BinaryCode起始張數-1
{
	return m_PatternStartNumBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500::SetPatternStartNumBC_1Bit(int val)//設定使用1Bit-BinaryCode起始張數-1
{
	m_PatternStartNumBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ExecDLPPattern_Run()
{
	if ( GetDLPIsConnected() == false ) { return false; }	
/*
	bool repeat;
	unsigned int numLutEntries, numPatsForTrigOut2, numSplash;    
	if( DLPC350_GetPatternConfig(&numLutEntries, &repeat, &numPatsForTrigOut2, &numSplash) == -1 )
	{
		m_ErrorString.Format("Get Pat Config Error!");
		return false;
	}
*/
	SaveDLPProcess(_T("ExecDLPPattern_Run"), MSG_LEVEL_HIGH);

	if( DLPC350_PatternDisplay(2) < 0 )
	{ 
		SetLCRErrorFnName(_T("DLPC350_PatternDisplay(2)"));		
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ExecDLPPattern_Stop()
{
	if ( GetDLPIsConnected() == false ) { return false; }	

	SaveDLPProcess(_T("ExecDLPPattern_Stop"), MSG_LEVEL_HIGH);

	if ( DLPC350_PatternDisplay(0) < 0 ) 
	{
		SetLCRErrorFnName(_T("DLPC350_PatternDisplay(0)"));		
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ExecDLPPattern_Pause()
{
	if ( GetDLPIsConnected() == false ) { return false; }	

	SaveDLPProcess(_T("ExecDLPPattern_Pause"), MSG_LEVEL_HIGH);
	if ( DLPC350_PatternDisplay(1) < 0 ) 
	{
		SetLCRErrorFnName(_T("DLPC350_PatternDisplay(1)"));		
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::LEDSetting(int LEDCurrent, int CurrentID)
{
	bool InvertPWM = false;
	const bool bUpdate = false;
	const TDLPParam &Param=m_DLPParam;
	int LEDCurrent_R=150, LEDCurrent_G=150, LEDCurrent_B=150;
	bool LEDEnabled_Auto=true, LEDEnabled_R=true, LEDEnabled_G=true, LEDEnabled_B=true;	

	InvertPWM = Param.m_InvertPWM;
	LEDEnabled_Auto = Param.m_LEDEnabled_Auto;
	LEDEnabled_R = Param.m_LEDEnabled_R;
	LEDEnabled_G = Param.m_LEDEnabled_G;
	LEDEnabled_B = Param.m_LEDEnabled_B;
	GetDLPParamLEDCurrent(LEDCurrent_R, LEDCurrent_G, LEDCurrent_B, CurrentID);	
	SaveDLPProcess(_T("LEDSetting"), MSG_LEVEL_HIGH);

	if( SetDLPLEDEnable(LEDEnabled_Auto, LEDEnabled_R, LEDEnabled_G , LEDEnabled_B) == false ) { return false; }
	if( SetDLPLEDCurrent(LEDCurrent_R, LEDCurrent_G, LEDCurrent_B, bUpdate, CurrentID) == false ) { return false; }
	if( SetDLPLEDPWMInvert(InvertPWM) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表 
{
	bool IsOK = true;
	SaveDLPProcess(_T("BuildDLPPatternList"), MSG_LEVEL_HIGH);	
	BuildDLPPatternList_TestGC(IntTrig, MultiTable, LEDColor);
	m_PatternList.clear();
	switch ( Mode )
	{
	case DLP_PATTERN_SEQUENCE_WHITE:	IsOK=BuildDLPPatternList_White(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_RGB:		IsOK=BuildDLPPatternList_RGB(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_2_2_M:	IsOK=BuildDLPPatternList_2X2_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4_1:	IsOK=BuildDLPPatternList_4X4_1(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4_2:	IsOK=BuildDLPPatternList_4X4_2(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_2_M:	IsOK=BuildDLPPatternList_4X2_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4_M:	IsOK=BuildDLPPatternList_4X4_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4GC_M:	IsOK=BuildDLPPatternList_4X4GC_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_5GC_M:	IsOK=BuildDLPPatternList_4X5GC_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_6GC_M:	IsOK=BuildDLPPatternList_4X6GC_M(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4GC_M2: IsOK=BuildDLPPatternList_4X4GC_M2(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_2_M_2:	IsOK=BuildDLPPatternList_4X2_M2(IntTrig, MultiTable, LEDColor); break;
	case DLP_PATTERN_SEQUENCE_4_4_M_2:	IsOK=BuildDLPPatternList_4X4_M2(IntTrig, MultiTable, LEDColor); break;
	//case DLP_PATTERN_SEQUENCE_4_43GC_M:	IsOK=BuildDLPPatternList_4X43GC_M(IntTrig, MultiTable, LEDColor); break;
	default:
		IsOK = true;
		break;
	}
	if ( false == IsOK )
	{	return false; }	
	m_TrigOutCount = m_PatternList.size();	
	if ( 0 == m_TrigOutCount )
	{
		m_ErrorString.Format(_T("Error, BuildDLPPatternList Fault[Mode=%d]"), Mode);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::CheckDLPPatternIndex(int index, unsigned int Count, int Mode)
{
	if ( index<0 || index>=Count )
	{
		if ( DLP_PATTERN_INDEX_MODE_GC_BC == Mode )
		{	m_ErrorString.Format(_T("Error, DLP Pattern Index Exception [%d/%d] (GC/BC)"), index, Count);	}
		else
		{	m_ErrorString.Format(_T("Error, DLP Pattern Index Exception [%d/%d]"), index, Count); }
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_TestGC(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-測試GC
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;
	
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	
	const int UseWaveCode=0;
	const int UseGrayCode=1;
	const int UseBinaryCode=2;
	const int UseCodeMode=UseWaveCode;
	if ( UseGrayCode==UseCodeMode )
	{	//0~9:2+4+8+16+32+64+128+256+512+1024
		bool bFirst=true;
		const int nStart=2;
		const int UseCnt=8;
		const int FlashIndexCode=18;
		if ( CheckDLPPatternIndex(FlashIndexCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == true )
		{
			for ( int i=nStart; i<nStart+UseCnt; i++ )
			{
				if ( true == bFirst )
				{
					bFirst = false;
					PatItem.sFlashIndex = FlashIndexCode;
					PatItem.sBitDepth = DLP_BIT_DEPTH_1;
					PatItem.sColorIndex = DLPLEDColor;
					PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
					PatItem.sBitNum = i;
					PatItem.sInvertPattern = false;
					PatItem.sInsertBlack = true;
					PatItem.sTrigOutPrev = false;
					PatItem.sBufSwap = true;
					PatItem.nPeriod = PatPeriod1;
					PatItem.nExposure = PatExposure1;
					this->AddDLPPPatItem(PatItem);
					index++;
					continue;
				}
				PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
				PatItem.sBitNum = i;//4
				PatItem.sBufSwap = false;
				this->AddDLPPPatItem(PatItem);
				index++;
			}
		}
		return true;
	}
	if ( UseBinaryCode==UseCodeMode )
	{	//10~19:2+4+8+16+32+64+128+256+512+1024
		bool bFirst=true;
		const int nStart=12;
		const int UseCnt=8;
		const int FlashIndexCode=18;
		if ( CheckDLPPatternIndex(FlashIndexCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == true )
		{
			for ( int i=nStart; i<nStart+UseCnt; i++ )
			{
				if ( true == bFirst )
				{
					bFirst = false;
					PatItem.sFlashIndex = FlashIndexCode;
					PatItem.sBitDepth = DLP_BIT_DEPTH_1;
					PatItem.sColorIndex = DLPLEDColor;
					PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
					PatItem.sBitNum = i;
					PatItem.sInvertPattern = false;
					PatItem.sInsertBlack = true;
					PatItem.sTrigOutPrev = false;
					PatItem.sBufSwap = true;
					PatItem.nPeriod = PatPeriod1;
					PatItem.nExposure = PatExposure1;
					this->AddDLPPPatItem(PatItem);
					index++;
					continue;
				}
				PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
				PatItem.sBitNum = i;//4
				PatItem.sBufSwap = false;
				this->AddDLPPPatItem(PatItem);
				index++;
			}
		}
		return true;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_White(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-白燈
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_RGB(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-紅綠藍燈
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;	

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_2X2_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-22M
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;	

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X4_1(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-441
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X4_2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-442
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X2_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-42M
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X4_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-44M
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;	

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X4GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X4GCM
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);

	const bool Use5BitPattern=GetUse5BitPattern();
	if ( true == Use5BitPattern )
	{
		//Test 5*4+4=24
		const int nFlashIndex1=GetPatternIndex1_5Bit();		
		if ( CheckDLPPatternIndex(nFlashIndex1, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
		{	return false; }
		
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
		if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
		{	return false; }

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
		const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
		const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
		if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
		{	return false; }

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
		if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
		{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X5GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X5GCM
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);

	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }
	
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X6GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X6GCM
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);

	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X4GC_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X4GCM2
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);

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
		if ( CheckDLPPatternIndex(nFlashIndex1, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
		{	return false; }

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
		if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
		{	return false; }

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
		const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
		const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
		if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
		{	return false; }

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
		if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
		{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X43GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X4-3GCM
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);
	
	const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

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
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }

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

	PatItem.sFlashIndex = FlashIndexBinaryCode;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-2;//BinaryCode3
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X2_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-42M2
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;	

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray	
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);

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
		if ( CheckDLPPatternIndex(nFlashIndex1, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(nFlashIndex2, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
		{	return false; }		
			
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
		const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
		const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8	
		if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
		{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::BuildDLPPatternList_4X4_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-44M2
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;		

	//0:16P, 1:224P, 2:16P+224P(3Bit), 3:8P, 4:20P, 5:22P, 6:30P, 7:32P, 8:224P+127Gray		
	unsigned int NumImagesInFlash = 0;
	DLPC350_GetNumImagesInFlash(&NumImagesInFlash);

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
		if ( CheckDLPPatternIndex(nFlashIndex1, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(nFlashIndex2, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
		{	return false; }
			
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
		const int FlashIndexA = GetPatternIndex1_6Bit();//3;//0,6,0
		const int FlashIndexB = GetPatternIndex2_6Bit();//0;//1,7,8
		if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
		{	return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{
	if ( true == bExpLut )
	{	return ExecDLPPatRead_ExpLut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat); }
	return ExecDLPPatRead_Lut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ExecDLPPatRead_Lut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
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
bool CLight3DTiDLP_Imp_4500::ExecDLPPatRead_ExpLut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
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
bool CLight3DTiDLP_Imp_4500::ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{
	if ( true == bExpLut )
	{	return ExecDLPPatSendAll_ExpLut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat); }
	return ExecDLPPatSendAll_Lut(TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ExecDLPPatSendAll_Lut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
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
bool CLight3DTiDLP_Imp_4500::ExecDLPPatSendAll_ExpLut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
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
bool CLight3DTiDLP_Imp_4500::ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( true == bExpLut )
	{	return ExecDLPPatSendOne_ExpLut(index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	}
	return ExecDLPPatSendOne_Lut(index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ExecDLPPatSendOne_Lut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
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
bool CLight3DTiDLP_Imp_4500::ExecDLPPatSendOne_ExpLut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
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
bool CLight3DTiDLP_Imp_4500::ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep)//套用樣板列表資料
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
bool CLight3DTiDLP_Imp_4500::ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force)//建立樣板, 傳送樣板以及驗證
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
bool CLight3DTiDLP_Imp_4500::ExecDLPLightSetting(int CurrentID)//執行DLP的LED設定-依據目前的設定
{
	const bool bUpdate = false;
	int CurRed=0, CurGrn=0, CurBlu= 0;	
	if ( GetDLPParamLEDCurrent(CurRed, CurGrn, CurBlu, CurrentID) == false )
	{	return false; }
	if ( SetDLPLEDEnable(true, true, true, true) == false )
	{	return false;	}		
	if ( SetDLPLEDCurrent(CurRed, CurGrn, CurBlu, bUpdate, CurrentID) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::ResetLEDDisable(bool &ResetFinish, bool ShowMsg)
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4500::GetGPIOStatus(UINT PinNum, bool &Status)//取得GPIO pin 狀態
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
bool CLight3DTiDLP_Imp_4500::SetGPIOStatus(UINT PinNum, bool Status)//取得GPIO pin output 狀態
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
bool CLight3DTiDLP_Imp_4500::GetGPIOTemperatureOver(bool &IsOver, bool ShowMsg)//偵測溫度狀態。		GPIO11 input狀態。high高溫/low低溫
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
bool CLight3DTiDLP_Imp_4500::GetGPIOLEDDisable(bool &IsDisable, bool ShowMsg)//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除
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
bool CLight3DTiDLP_Imp_4500::SetGPIOLEDEnable()//LED disable狀態解除。	GPIO6  output狀態。 low->hi	
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
#endif//LIGHT_3D_TI_DLP_USE_IMP_4500