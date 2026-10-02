// Light3DTiDLP_Imp_4710.cpp: implementation of the CLight3DTiDLP_Imp_4710 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP.h"
#include "Light3DTiDLP_Imp_4710.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//
bool  CLight3DTiDLP::CreateImp_DLP4710(int CtrlID, LIGHT_3D_CAST_ID CastID)//建立Imp的指標
{
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4710
	m_Imp = new CLight3DTiDLP_Imp_4710(CtrlID, CastID);
#endif//LIGHT_3D_TI_DLP_USE_IMP_4710
	if ( NULL == m_Imp )
	{
		m_ErrorString = _T("Error, CLight3DTiDLP::CreateImp_DLP4710 Fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4710
//-------------------------------------------------------------------------------------//
#define DLP_LED_CURRENT_MAX				 1023		//最高亮度
#define DLP_LED_CURRENT_MIN					0		//最低亮度
//-------------------------------------------------------------------------------------//
#define DLP_PATTERN_INDEX_MODE_WAVE     1
#define DLP_PATTERN_INDEX_MODE_GC_BC    2
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4710::CLight3DTiDLP_Imp_4710():CLight3DTiDLP_Imp()
{
	PreInitTiDlp(0, LIGHT_3D_CAST_00);	
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4710::CLight3DTiDLP_Imp_4710(int CtrlID, LIGHT_3D_CAST_ID CastID):CLight3DTiDLP_Imp(CtrlID, CastID)
{
	PreInitTiDlp(CtrlID, CastID);
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4710::~CLight3DTiDLP_Imp_4710()
{	
	DLPDisconnect();
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP_Imp_4710::PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID CastID)
{	
	CLight3DTiDLP_Imp::PreInitTiDlp(CtrlID, CastID);
	
	SetDLPImageW(1920);
	SetDLPImageH(1080);
	SetDeviceType(LIGHT_3D_DEVICE_DLP4710);	
	
	m_EnableTemperatureMonitor = true;	

	s_Index = 0;
	::memset(s_HorizontalPatternData, 0x00, sizeof(s_HorizontalPatternData));
	::memset(s_VerticalPatternData, 0x00, sizeof(s_VerticalPatternData));

	::memset(s_Patterns, 0x00, sizeof(s_Patterns));
	::memset(s_PatternSets, 0x00, sizeof(s_PatternSets));
	::memset(s_PatternOrderTable, 0x00, sizeof(s_PatternOrderTable));

	::memset(s_WriteBuffer, 0x00, sizeof(s_WriteBuffer));
	::memset(s_ReadBuffer, 0x00, sizeof(s_ReadBuffer));

	::memset(s_FlashProgramBuffer, 0x00, sizeof(s_FlashProgramBuffer));	

	s_StartProgramming = false;
	s_FlashProgramBufferPtr = NULL;
	s_FilePointer = NULL;

	//dlpc_common.h	
	s_WriteBufferSize = sizeof(s_WriteBuffer);
	s_WriteBufferIndex = 0;	
	s_ReadBufferSize = sizeof(s_ReadBuffer);
	s_ReadBufferIndex = 0;	
	::memset(&s_ProtocolData, 0x00, sizeof(s_ProtocolData));	

	this->InitialDLPParameter(m_DLPParam);
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP_Imp_4710::InitialTiDlp()
{
	CLight3DTiDLP_Imp::InitialTiDlp();

	m_PatternBitCount = 8;
	m_PatternDarkIndex_8Bit = 8;
	m_PatternGrayIndex_8Bit = 9;
	m_PatternWhiteIndex_8Bit = 10;	
	m_PatternIndex1_3Bit = -1;
	m_PatternIndex2_3Bit = -1;
	m_PatternIndex1_5Bit = -1;
	m_PatternIndex1_6Bit = 0;
	m_PatternIndex2_6Bit = 1;
	m_PatternIndex1_8Bit = 0;
	m_PatternIndex2_8Bit = 4;
	m_PatternIndexGC_1Bit= -1;
	m_PatternIndexBC_1Bit= -1;
	m_PatternIndexGC_8Bit= 12;
	m_PatternIndexBC_8Bit= 19;
	m_PatternStartNumGC_1Bit = -1;
	m_PatternStartNumBC_1Bit = -1;
	return ;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDlpMonoMode() const
{
	return true;
}
//-------------------------------------------------------------------------------------//
inline void CLight3DTiDLP_Imp_4710::SetLCRErrorFnName(LPCTSTR LCRFnName)
{
	this->m_ErrorString.Format(_T("Error, TiDLP %s Fault"), LCRFnName);
}
//-------------------------------------------------------------------------------------//
inline bool CLight3DTiDLP_Imp_4710::GetUSB_Number(LIGHT_3D_CAST_ID CastID, char USB_Number[])
{
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::sprintf(USB_Number, "DLP2");	break;
	case LIGHT_3D_CAST_03:	::sprintf(USB_Number, "DLP3");	break;
	case LIGHT_3D_CAST_04:	::sprintf(USB_Number, "DLP4");	break;
	default:			    ::sprintf(USB_Number, "DLP1");	break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLight3DTiDLP_Imp_4710::GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[])
{
	switch (CastID)
	{
	case LIGHT_3D_CAST_02:	::wsprintfW(USB_Number, L"DLP2");	break;
	case LIGHT_3D_CAST_03:	::wsprintfW(USB_Number, L"DLP3");	break;
	case LIGHT_3D_CAST_04:	::wsprintfW(USB_Number, L"DLP4");	break;
	default:			    ::wsprintfW(USB_Number, L"DLP1");	break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP_Imp_4710::GetDLPProjectName() const
{
	return this->m_CastName;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::DLPConnect()
{
	SaveDLPProcess(_T("DLPConnect"), MSG_LEVEL_HIGH);

	bool SLmode=0;
	const int DLPID = 0;
	char  USB_Number[256]={0};	

	if ( m_I2C.GetIsConnected() == true )
	{	m_I2C.CYPRESS_I2C_CloseCyI2C(); }
	
	LIGHT_3D_CAST_ID CastID = GetCastID();
	if( GetUSB_Number(CastID, USB_Number) == false)
	{
		SetAOIExceptionCode(AOI_EXCEPTION_DLP_CTRL_CONNECT);
		return false;	
	}
	//::wsprintfW(USB_Number, L"LCR2");    
	if ( m_I2C.CYPRESS_I2C_ConnectToCyI2C(USB_Number) == false )	
	{
		SetAOIExceptionCode(AOI_EXCEPTION_DLP_CTRL_CONNECT);
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault"));
		return false; 
	}	

	uint16_t PatchVersion=0;
	uint8_t  MinorVersion=0, MajorVersion=0;
	if ( ReadSystemSoftwareVersion(&PatchVersion, &MinorVersion, &MajorVersion) == true )
	{	sprintf(m_TiAPIversion, "%d.%d.%d", MajorVersion, MinorVersion, PatchVersion);	}    
	if ( ReadFirmwareBuildVersion(&PatchVersion, &MinorVersion, &MajorVersion) == true )
	{	sprintf(m_DLPFrmversion, "%d.%d.%d", MajorVersion, MinorVersion, PatchVersion);	}    
	
	::strcat(m_DLPFrmTag, "DLP4710-DLPC347x_Dual");//No Tag Support	

	unsigned int numImgInFlash = 0;
	//Retrieve the total number of Images in the firmware info, m_numImgInFlash=0


	bool ShowMsg = false;
	bool ResetFinish = true;
	ResetLEDDisable(ResetFinish, ShowMsg);

	this->CheckDLPStatus();

	//Check SL Mode
	DLPC34XX_DUAL_OperatingMode_e OperatingMode;
	if ( ReadOperatingModeSelect(&OperatingMode) == false )
	{	m_OperationMode = DLP_OPERATION_DEFAULT;	}
	else
	{	m_OperationMode=MapOperatingModeSelect(OperatingMode);	}
	
	SetDLPOperationMode(m_OperationMode);
	//Check LED Parameters
	LoadDLPParameter();
	GetDLPLEDPWMInvert(m_InvertPWM);//m_InvertPWM);	
	GetDLPLEDEnable(m_LEDEnabled_Auto, m_LEDEnabled_R, m_LEDEnabled_G, m_LEDEnabled_B);	
	GetDLPLEDCurrent(m_LEDCurrentR, m_LEDCurrentG, m_LEDCurrentB);	
	ExecDLPLightSetting(DLP_LED_CURRENT_ID_01);

	if (WriteTriggerInConfiguration(DLPC34XX_DUAL_TE_ENABLE, DLPC34XX_DUAL_TP_ACTIVE_LOW) == false)
	{	return false;	}
	if (WritePatternReadyConfiguration(DLPC34XX_DUAL_TE_ENABLE, DLPC34XX_DUAL_TP_ACTIVE_LOW) == false)
	{	return false;	}
	if (WriteTriggerOutConfiguration(DLPC34XX_DUAL_TT_TRIGGER1, DLPC34XX_DUAL_TE_ENABLE, DLPC34XX_DUAL_TI_INVERTED, 0) == false)
	{	return false;	}
	if (WriteTriggerOutConfiguration(DLPC34XX_DUAL_TT_TRIGGER2, DLPC34XX_DUAL_TE_ENABLE, DLPC34XX_DUAL_TI_INVERTED, 0) == false)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::DLPDisconnect(int WaitTime_ms)
{
	CLight3DTiDLP_Imp_4710::ExecDLPPattern_Stop();

	SaveDLPProcess(_T("DLPDisconnect"), MSG_LEVEL_HIGH);

	m_I2C.CYPRESS_I2C_CloseCyI2C();

	m_dwFrmVersion = 0;
	::memset(m_DLPFrmTag, 0x00, sizeof(m_DLPFrmTag));
	::memset(m_TiAPIversion, 0x00, sizeof(m_TiAPIversion));	
	::memset(m_DLPFrmversion, 0x00, sizeof(m_DLPFrmversion));	
	::memset(m_DLPMcuversion, 0x00, sizeof(m_DLPMcuversion));

	//m_numImgInFlash = 0;

	if ( WaitTime_ms > 0 )
	{	::Sleep(WaitTime_ms);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPIsConnected()
{
	if ( m_I2C.CheckIsConnected() == false )
	{
		this->m_ErrorString.Format(_T("Error, TiDLP did not connect"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::CheckDLPFrmForExpLut()//確認DLP韌體支援Exposure Lut
{
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ExecDLPSoftwareReset(DWORD delayTime)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }
	
	SaveDLPProcess(_T("ExecDLPSoftwareReset"), MSG_LEVEL_HIGH);
	
	m_I2C.CYPRESS_I2C_CloseCyI2C();
	::Sleep(delayTime);//暫停10秒
	char USB_Number[256] = { 0 };
	LIGHT_3D_CAST_ID CastID = GetCastID();	
	if ( GetUSB_Number(CastID, USB_Number) == false)
	{	return false;	}

	if ( m_I2C.CYPRESS_I2C_ConnectToCyI2C(USB_Number) == false )	
	{
		this->m_ErrorString.Format(_T("Error, TiDLP USB Open Fault"));
		return false;
	}

	this->ReadDLPParameter();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::SetDLPLongAxisImageFlip(bool Flip)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPLongAxisImageFlip"), MSG_LEVEL_HIGH);

	DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, ShortAxisImageFlip;
	if ( ReadDisplayImageOrientation(&LongAxisImageFlip, &ShortAxisImageFlip) == false )
	{	return false; }

	if ( true == Flip ) { LongAxisImageFlip=DLPC34XX_DUAL_IF_IMAGE_FLIPPED; }
	else { LongAxisImageFlip=DLPC34XX_DUAL_IF_IMAGE_NOT_FLIPPED; }

	if ( WriteDisplayImageOrientation(LongAxisImageFlip, ShortAxisImageFlip) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPLongAxisImageFlip()
{
	DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, ShortAxisImageFlip;
	if ( ReadDisplayImageOrientation(&LongAxisImageFlip, &ShortAxisImageFlip) == false )
	{	return false; }	
	if ( DLPC34XX_DUAL_IF_IMAGE_NOT_FLIPPED == LongAxisImageFlip )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::SetDLPShortAxisImageFlip(bool Flip)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPShortAxisImageFlip"), MSG_LEVEL_HIGH);

	DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, ShortAxisImageFlip;
	if ( ReadDisplayImageOrientation(&LongAxisImageFlip, &ShortAxisImageFlip) == false )
	{	return false; }

	if ( true == Flip ) { ShortAxisImageFlip=DLPC34XX_DUAL_IF_IMAGE_FLIPPED; }
	else { ShortAxisImageFlip=DLPC34XX_DUAL_IF_IMAGE_NOT_FLIPPED; }

	if ( WriteDisplayImageOrientation(LongAxisImageFlip, ShortAxisImageFlip) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPShortAxisImageFlip()
{
	DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, ShortAxisImageFlip;
	if ( ReadDisplayImageOrientation(&LongAxisImageFlip, &ShortAxisImageFlip) == false )
	{	return false; }	
	if ( DLPC34XX_DUAL_IF_IMAGE_NOT_FLIPPED == ShortAxisImageFlip )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::SetDLPOperationMode(int Mode)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("SetDLPOperationMode"), MSG_LEVEL_HIGH);
	DLPC34XX_DUAL_OperatingMode_e OperatingMode=MapOperatingModeSelect(Mode);
	if ( WriteOperatingModeSelect(OperatingMode) == false )
	{	return false; }
	m_OperationMode = Mode;	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetDLPOperationMode() const
{	
	return this->m_OperationMode;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_LEDEnabled_R!=bRed || m_LEDEnabled_G!=bGreen || m_LEDEnabled_B!=bBlue )//m_LEDEnabled_Auto!=bSeqCtrl
	{
		SaveDLPProcess(_T("SetDLPLEDEnable"), MSG_LEVEL_HIGH);
		if ( WriteRgbLedEnable(bRed, bGreen, bBlue) == false )		
		{	return false;	}
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
bool CLight3DTiDLP_Imp_4710::GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }
	
	if ( ReadRgbLedEnable(&bRed, &bGreen, &bBlue) == false )		
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP_Imp_4710::GetPatternSetCountInDLP()
{
	return m_PatternSetCountInDLP;
}
//-------------------------------------------------------------------------------------//
inline int CLight3DTiDLP_Imp_4710::GetDLPTrigType(int index, bool IntTrig, bool MultiTable)
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
bool CLight3DTiDLP_Imp_4710::SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_LEDCurrentR!=red || m_LEDCurrentG!=green || m_LEDCurrentB!=blue )
	{	
		SaveDLPProcess(_T("SetDLPLEDCurrent"), MSG_LEVEL_HIGH);
		
		int r = GetDLPSafeCurrent(red);
		int g = GetDLPSafeCurrent(green);
		int b = GetDLPSafeCurrent(blue);
		if ( WriteRgbLedCurrent(r, g, b) == false )
		{	return false;	}				
	}
	
	m_LEDCurrentR = red;
	m_LEDCurrentG = green;
	m_LEDCurrentB = blue;
	//if ( true == Update )//要此過濾再校正電流會有問題
	{
		SetDLPParamLEDCurrent(red, green, blue, CurrentID);		
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPLEDCurrent(int &red, int &green, int &blue)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	uint16_t r=0, g=0, b=0;
    if ( ReadRgbLedCurrent(&r, &g, &b) == false )  
	{	return false;	}	
	red   = r;
	green = g;
	blue  = b;
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::SetDLPLEDPWMInvert(bool bInvert)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( m_InvertPWM != bInvert )
	{
		SaveDLPProcess(_T("SetDLPLEDPWMInvert"), MSG_LEVEL_HIGH);
		//if ( DLPC350_SetLEDPWMInvert(bInvert) < 0 )//Not Support
	}
	m_InvertPWM = bInvert;
	this->m_DLPParam.m_InvertPWM = bInvert;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPLEDPWMInvert(bool &bInvert)
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }
    //if ( DLPC350_GetLEDPWMInvert(&bInvert) == -1 )//Not Support	
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::CheckDLPStatus()
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	if ( ReadShortStatus(&m_ShortStatus) == false )
	{	return false; }
	if ( ReadSystemStatus(&m_SystemStatus) == false )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPStatus_InitDone()
{
	if ( DLPC34XX_DUAL_SI_NOT_COMPLETE == m_ShortStatus.SystemInitialized )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPStatus_ForcedSwap()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPStatus_BufferFreeze()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPStatus_SeqRunning()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPStatus_SeqError()
{
	if ( DLPC34XX_DUAL_E_NO_ERROR == m_SystemStatus.SequenceError )
	{	return false; }
	return true;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPStatus_SeqAbort()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPStatus_DRCError()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPStatus_DMDParked()
{
	return false;
}
//-----------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ReadDLPParameter()//從DLP裝置讀取參數
{
#ifndef PHASE_CTRL_DISABLE
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ReadDLPParameter"), MSG_LEVEL_HIGH);

	//重新取回參數
	bool SLmode=false;	
	DLPC34XX_DUAL_OperatingMode_e OperatingMode;
	m_OperationMode = DLP_OPERATION_DEFAULT;
	if ( ReadOperatingModeSelect(&OperatingMode) == false )
	{	return false; }
	m_OperationMode=MapOperatingModeSelect(OperatingMode);


	bool bSeqCtrl = false;
	bool bRed=false, bGreen=false, bBlue=false;
	if ( ReadRgbLedEnable(&bRed, &bGreen, &bBlue) == false )
	{	return false;	}

	m_DLPParam.m_LEDEnabled_Auto = bSeqCtrl;
	m_DLPParam.m_LEDEnabled_R = bRed;
	m_DLPParam.m_LEDEnabled_G = bGreen;
	m_DLPParam.m_LEDEnabled_B = bBlue;

	uint16_t cRed=0, cGreen=0, cBlue=0;	
	if ( ReadRgbLedCurrent(&cRed, &cGreen, &cBlue) == false )    
	{	return false;	}

	m_LEDCurrentR = cRed;
	m_LEDCurrentG = cGreen;
	m_LEDCurrentB = cBlue;

	//m_DLPParam.m_LEDCurrentR_1 = cRed;
	//m_DLPParam.m_LEDCurrentG_1 = cGreen;
	//m_DLPParam.m_LEDCurrentB_1 = cBlue;
	
	bool bInvert=false;
	//if ( DLPC350_GetLEDPWMInvert(&bInvert) < 0 ) //Not Support	
	m_InvertPWM = bInvert;
	this->m_DLPParam.m_InvertPWM = bInvert;
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetDLPReadySignalEnable() const
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetUseExpLut()
{
	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	
	return m_DLPParam.m_UseExpLut;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternBitCount() const//取得樣板圖位元數
{
	return m_PatternBitCount;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternBitCount(int val)//設定樣板圖位元數
{
	m_PatternBitCount = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternDarkIndex_8Bit() const//取得使用8Bit黑畫面引數-1
{
	return m_PatternDarkIndex_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternDarkIndex_8Bit(int val)//設定使用8Bit黑畫面引數-1	
{
	m_PatternDarkIndex_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternGrayIndex_8Bit() const//取得使用8Bit灰畫面引數-1
{
	return m_PatternGrayIndex_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternGrayIndex_8Bit(int val)//設定使用8Bit灰畫面引數-1	
{
	m_PatternGrayIndex_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternWhiteIndex_8Bit() const//取得使用8Bit白畫面引數-1
{
	return m_PatternWhiteIndex_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternWhiteIndex_8Bit(int val)//設定使用8Bit白畫面引數-1	
{
	m_PatternWhiteIndex_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndex1_3Bit() const//取得使用3Bit樣板引數-1
{
	return m_PatternIndex1_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndex1_3Bit(int val)//設定使用3Bit樣板引數-1
{
	m_PatternIndex1_3Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndex2_3Bit() const//取得使用3Bit樣板引數-2
{
	return m_PatternIndex2_3Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndex2_3Bit(int val)//設定使用3Bit樣板引數-2
{
	m_PatternIndex2_3Bit = val;	
}
//-------------------------------------------------------------------------------------//															 
int CLight3DTiDLP_Imp_4710::GetPatternIndex1_5Bit() const//取得使用5Bit樣板引數-1
{
	return m_PatternIndex1_5Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndex1_5Bit(int val)//設定使用5Bit樣板引數-1
{
	m_PatternIndex1_5Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndex1_6Bit() const//取得使用6Bit樣板引數-1
{
	return m_PatternIndex1_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndex1_6Bit(int val)//設定使用6Bit樣板引數-1
{
	m_PatternIndex1_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndex2_6Bit() const//取得使用6Bit樣板引數-2
{
	return m_PatternIndex2_6Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndex2_6Bit(int val)//設定使用6Bit樣板引數-2
{
	m_PatternIndex2_6Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndex1_8Bit() const//取得使用8Bit樣板引數-1
{
	return m_PatternIndex1_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndex1_8Bit(int val)//設定使用8Bit樣板引數-1
{
	m_PatternIndex1_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndex2_8Bit() const//取得使用8Bit樣板引數-2
{
	return m_PatternIndex2_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndex2_8Bit(int val)//設定使用8Bit樣板引數-2
{
	m_PatternIndex2_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndexGC_1Bit() const//取得使用1Bit-GrayCode引數-1
{
	return m_PatternIndexGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndexGC_1Bit(int val)//設定使用1Bit-GrayCode引數-1
{
	m_PatternIndexGC_1Bit = val;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndexBC_1Bit() const//取得使用1Bit-BinaryCode引數-1
{
	return m_PatternIndexBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndexBC_1Bit(int val)//設定使用1Bit-BinaryCode引數-1
{
	m_PatternIndexBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndexGC_8Bit() const//取得使用8Bit-GrayCode引數-1
{	
	return m_PatternIndexGC_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndexGC_8Bit(int val)//設定使用8Bit-GrayCode引數-1
{
	m_PatternIndexGC_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternIndexBC_8Bit() const//取得使用8Bit-BinaryCode引數-1
{
	return m_PatternIndexBC_8Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternIndexBC_8Bit(int val)//設定使用8Bit-BinaryCode引數-1
{
	m_PatternIndexBC_8Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternStartNumGC_1Bit() const//取得使用1Bit-GrayCode起始張數-1
{
	return m_PatternStartNumGC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternStartNumGC_1Bit(int val)//設定使用1Bit-GrayCode起始張數-1
{
	m_PatternStartNumGC_1Bit = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetPatternStartNumBC_1Bit() const//取得使用1Bit-BinaryCode起始張數-1
{
	return m_PatternStartNumBC_1Bit;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4710::SetPatternStartNumBC_1Bit(int val)//設定使用1Bit-BinaryCode起始張數-1
{
	m_PatternStartNumBC_1Bit = val;
}
//-------------------------------------------------------------------------------------//	
bool CLight3DTiDLP_Imp_4710::ExecDLPPatClear()//清除樣版內容
{
	SaveDLPProcess(_T("ExecDLPPatClear"), MSG_LEVEL_HIGH);

	this->m_PatternList.clear();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)//讀取DLP樣版內容
{	
	if ( this->GetDLPIsConnected() == false )
	{	return false; }			

	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	

	SaveDLPProcess(_T("ExecDLPPatRead"), MSG_LEVEL_HIGH);

	
	int i=0;
	TDLPPatItem  PatItem;
	int Pat_Exposure = 0, Pat_Period = 0;
	int firstItem=32, lastItem=0, index=0;
	bool Invert_Pat=false, Insert_Black=false, Buf_Swap=false, TrigOutPrev=false;    
	int trig_type=0, Pat_Num=0, Bit_Depth=0, LED_Select=0, Frame_Index=0, Fresh_Index=0;    	
	unsigned int numLutEntries = GetPatternSetCountInDLP();

	Bit_Depth = 8;
	m_PatternList.clear();	
    for(i=0; i<numLutEntries; i++)
    {		
		uint8_t PatternOrderTableEntryIndex=i;
		DLPC34XX_DUAL_PatternOrderTableEntry_s PatternOrderTableEntry;
		if ( ReadPatternOrderTableEntry(PatternOrderTableEntryIndex, &PatternOrderTableEntry) == false )
		{	continue; }
		
		Pat_Exposure = PatternOrderTableEntry.IlluminationTime;
		Pat_Period = PatternOrderTableEntry.PreIlluminationDarkTime+PatternOrderTableEntry.IlluminationTime+PatternOrderTableEntry.PostIlluminationDarkTime;

		LED_Select = DLP_LED_COLOR_NO;		
		if ( DLPC34XX_DUAL_IE_ENABLE == PatternOrderTableEntry.RedIlluminator ) 
		{ 
			if ( DLP_LED_COLOR_NO == LED_Select ) { LED_Select = DLP_LED_COLOR_RED; }
			else { LED_Select = DLP_LED_COLOR_WHITE; }			
		}
		if ( DLPC34XX_DUAL_IE_ENABLE == PatternOrderTableEntry.GreenIlluminator )
		{
			if ( DLP_LED_COLOR_NO == LED_Select ) { LED_Select = DLP_LED_COLOR_GREEN; }
			else { LED_Select = DLP_LED_COLOR_WHITE; }
		}
		if ( DLPC34XX_DUAL_IE_ENABLE == PatternOrderTableEntry.BlueIlluminator )
		{
			if ( DLP_LED_COLOR_NO == LED_Select ) { LED_Select = DLP_LED_COLOR_BLUE; }
			else { LED_Select = DLP_LED_COLOR_WHITE; }			
		}

		PatItem.sFlashIndex = PatternOrderTableEntry.PatSetIndex;
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
bool CLight3DTiDLP_Imp_4710::ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新樣板至DLP
{	
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	SaveDLPProcess(_T("ExecDLPPatSendAll"), MSG_LEVEL_HIGH);

	int i=0;  
	CString str;
	CString ItemText;	
	TDLPPatItem PatItem;
	const int PatItemCount = (int)(this->m_PatternList.size());	

	//Make sure the Pattern Exposure and Pattern Period timings are within the spec

    //Pattern Exposure > Pattern Period not a valid settings
    if( Exposure_us > TrigPeriod_us )
    {
        m_ErrorString = _T("Pattern exposure setting voilation, it should be, Pattern Exposure = Pattern Period or (Pattern Period - Pattern Exposure) > 230us");
        return false;
    }
	
	uint32_t ExposureTime = Exposure_us;	
	DLPC34XX_DUAL_ValidateExposureTime_s ValidateExposureTime;
	DLPC34XX_DUAL_PatternMode_e PatternMode=DLPC34XX_DUAL_PM_INTERNAL;
	DLPC34XX_DUAL_SequenceType_e eBitDepth=DLPC34XX_DUAL_ST_EIGHT_BIT_RGB;	
	
	int LedUsdCnt=0;
	const bool bMonoMode = GetDlpMonoMode();
	DLPC34XX_DUAL_IlluminatorEnable_e RedEnb, GrnEnb, BluEnb;
	DLPC34XX_DUAL_WriteControl_e WriteControl;
	DLPC34XX_DUAL_PatternOrderTableEntry_s PatternOrderTableEntry;
    for (i=0; i<PatItemCount; i++)
    {
		PatItem = m_PatternList[i];
		if ( 0 == i )
		{	WriteControl = DLPC34XX_DUAL_WC_START;	}
		else
		{	WriteControl = DLPC34XX_DUAL_WC_CONTINUE;	}
		::memset(&PatternOrderTableEntry, 0x00, sizeof(PatternOrderTableEntry));

		LedUsdCnt=0;
		RedEnb = GrnEnb = BluEnb = DLPC34XX_DUAL_IE_DISABLE;		
		switch ( PatItem.sColorIndex )
		{
		case DLP_LED_COLOR_RED:    RedEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_GREEN:  GrnEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_YELLOW: RedEnb = GrnEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_BLUE:   BluEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_MAGENTA: RedEnb = BluEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_CYAN:    GrnEnb = BluEnb = DLPC34XX_DUAL_IE_ENABLE; break;
		case DLP_LED_COLOR_WHITE:  RedEnb = GrnEnb = BluEnb = DLPC34XX_DUAL_IE_ENABLE;	break;			
			break;		
		}
		if (DLP_LED_COLOR_YELLOW==PatItem.sColorIndex || DLP_LED_COLOR_MAGENTA==PatItem.sColorIndex || DLP_LED_COLOR_CYAN==PatItem.sColorIndex || DLP_LED_COLOR_WHITE==PatItem.sColorIndex )
		{
			if (true == bMonoMode)
			{
				BluEnb = DLPC34XX_DUAL_IE_ENABLE;
				RedEnb = GrnEnb = DLPC34XX_DUAL_IE_DISABLE;
			}
		}
		
		if ( DLPC34XX_DUAL_IE_ENABLE== RedEnb ) { LedUsdCnt++; }
		if ( DLPC34XX_DUAL_IE_ENABLE== GrnEnb ) { LedUsdCnt++; }
		if ( DLPC34XX_DUAL_IE_ENABLE== BluEnb ) { LedUsdCnt++; }
		if ( LedUsdCnt > 1 )
		{	eBitDepth=DLPC34XX_DUAL_ST_EIGHT_BIT_RGB;	}
		else
		{	eBitDepth=DLPC34XX_DUAL_ST_EIGHT_BIT_MONO;	}

		ExposureTime = PatItem.nExposure;
		if ( ReadValidateExposureTime(PatternMode, eBitDepth, ExposureTime, &ValidateExposureTime) == false )
		{	return false;	}
		if ( ExposureTime < ValidateExposureTime.MinimumExposureTime )
		{
			  m_ErrorString.Format(_T("Pattern[%02d] exposure time[%d us] less than Cast minimum exposure time[%d]"), i+1, ExposureTime, ValidateExposureTime.MinimumExposureTime);
			  return false;
		}
		const uint32_t PreExpPostTime=ValidateExposureTime.PreExposureDarkTime+ValidateExposureTime.MinimumExposureTime+ValidateExposureTime.PostExposureDarkTime;
		if ( PatItem.nPeriod < PreExpPostTime )
		{
			  m_ErrorString.Format(_T("Pattern[%02d] period time[%d us] less than Cast minimum period time[%d]"), i+1, PatItem.nPeriod, PreExpPostTime);
			  return false;
		}    	

		PatternOrderTableEntry.PatSetIndex=PatItem.sFlashIndex;
		PatternOrderTableEntry.NumberOfPatternsToDisplay = 1;

		PatternOrderTableEntry.RedIlluminator   = RedEnb;
		PatternOrderTableEntry.GreenIlluminator = GrnEnb;
		PatternOrderTableEntry.BlueIlluminator  = BluEnb;		 

		PatternOrderTableEntry.PatternInvertLsword = 0;
		PatternOrderTableEntry.PatternInvertMsword = 0;
		PatternOrderTableEntry.IlluminationTime = ExposureTime;
		PatternOrderTableEntry.PreIlluminationDarkTime  = ValidateExposureTime.PreExposureDarkTime;
		PatternOrderTableEntry.PostIlluminationDarkTime = ValidateExposureTime.PostExposureDarkTime;
		if ( WritePatternOrderTableEntry(WriteControl, &PatternOrderTableEntry) == false )
		{	return false; }
    }	    
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)//更新單一樣板至DLP
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }
	SaveDLPProcess(_T("ExecDLPPatSendOne_Lut"), MSG_LEVEL_HIGH);
	return ReturnNotSupportFunc(_T("CLight3DTiDLP_Imp_4710::ExecDLPPatSendOne"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep)//套用樣板列表資料
{
	if ( this->GetDLPIsConnected() == false )
	{	return false; }

	Status = 0;
	SaveDLPProcess(_T("ExecDLPValidatePatLutData"), MSG_LEVEL_HIGH);		
	if ( true == Sleep )
	{
		if ( m_DLPParam.m_ValidateDelayTime > 0 ) 
		{	::Sleep(m_DLPParam.m_ValidateDelayTime); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force)//建立樣板, 傳送樣板以及驗證
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
bool CLight3DTiDLP_Imp_4710::ExecDLPLightSetting(int CurrentID)//執行DLP的LED設定-依據目前的設定
{
	const bool bUpdate = false;
	int CurRed=0, CurGrn=0, CurBlu= 0;
	CLight3DTiDLP_Imp_4710 *Light3DPtr = this;
	Light3DPtr->GetDLPParamLEDCurrent(CurRed, CurGrn, CurBlu, CurrentID);
	if ( Light3DPtr->SetDLPLEDEnable(true, true, true, true) == false )
	{	return false;	}		
	if ( Light3DPtr->SetDLPLEDCurrent(CurRed, CurGrn, CurBlu, bUpdate, CurrentID) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::CalcPeriodPaddingTime(int ExpTime) const//計算週期外加時間-us
{	
	float MinExp=2100;//us
	const float MinPreExp=171;//us
	const float MinPostExp=31;//us
	const bool bMonoMode=GetDlpMonoMode();
	if ( true == bMonoMode )
	{	MinExp = 2100; }
	else
	{	MinExp = 6500; }
	const float ExpRatio=(ExpTime/MinExp);
	const int PreExp=(int)((MinPreExp*ExpRatio)+0.5);
	const int PostExp=(int)((MinPostExp*ExpRatio)+0.5);
	const int PaddingTime=(PreExp+PostExp)+10;
	return MAX(PaddingTime, GetPeriodPaddingTime());
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ExecDLPPattern_Run()
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
	//0xFF=Continue
	if ( WriteInternalPatternControl(DLPC34XX_DUAL_PC_START, 0xFF) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ExecDLPPattern_Stop()
{
	if ( this->GetDLPIsConnected() == false ) { return false; }	

	SaveDLPProcess(_T("ExecDLPPattern_Stop"), MSG_LEVEL_HIGH);
	if ( WriteInternalPatternControl(DLPC34XX_DUAL_PC_STOP) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ExecDLPPattern_Pause()
{
	if ( this->GetDLPIsConnected() == false ) { return false; }	

	SaveDLPProcess(_T("ExecDLPPattern_Pause"), MSG_LEVEL_HIGH);
	if ( WriteInternalPatternControl(DLPC34XX_DUAL_PC_PAUSE) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::LEDSetting(int LEDCurrent, int CurrentID)
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
void CLight3DTiDLP_Imp_4710::ClearDLPPatternList()//清除m_PatternList
{
	SaveDLPProcess(_T("ClearDLPPatternList"), MSG_LEVEL_HIGH);
	m_PatternList.clear();
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::CheckDLPPatternIndex(int index, unsigned int Count, int Mode)
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
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表 
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
	case DLP_PATTERN_SEQUENCE_4_5GC_M2: IsOK=BuildDLPPatternList_4X5GC_M2(IntTrig, MultiTable, LEDColor); break;
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
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_TestGC(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-測試GC
{
	int index = 0;
	TDLPPatItem  PatItem;	
	const TDLPParam &DLPParam = GetDLPParam();	
	int PatPeriod1 = (int)(DLPParam.m_PeriodTime_us);
	int PatExposure1 = (int)(DLPParam.m_ExposureTime_us);
	int PatPeriod2 = (int)(DLPParam.m_PeriodTime2_us);
	int PatExposure2 = (int)(DLPParam.m_ExposureTime2_us);	
	const int DLPLEDColor = LEDColor;//DLP_LED_COLOR_WHITE;
	
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	
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
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_White(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-白燈
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
	const int FlashIndexD = GetPatternDarkIndex_8Bit();
	const int FlashIndexG = GetPatternGrayIndex_8Bit();
	const int FlashIndexW = GetPatternWhiteIndex_8Bit();	

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8	
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	const int FlashIndex = FlashIndexW;//FlashIndexW;//FlashIndexA, 8
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_RGB(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-紅綠藍燈
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
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_2X2_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-22M
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
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	//6x(2+2+1)=30
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index++;
	FlashIndex++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;//000
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//PatItem.sFlashIndex = FlashIndex;
	//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	//PatItem.sBitNum = 1;//270	
	//PatItem.sBufSwap = false;
	//this->AddDLPPPatItem(PatItem);
	//index ++;
	//FlashIndex++;

	//PatItem.sFlashIndex = FlashIndex;
	//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	//PatItem.sBitNum = 2;//180
	//PatItem.sBufSwap = false;
	//this->AddDLPPPatItem(PatItem);
	//index ++;
	//FlashIndex++;

	//PatItem.sFlashIndex = FlashIndex;
	//PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	//PatItem.sBitNum = 3;//090
	//PatItem.sBufSwap = false;
	//this->AddDLPPPatItem(PatItem);
	//index ++;
	//FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X4_1(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-421
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
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8	
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X4_2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-422
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
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexB;	
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X2_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-42M
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
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	//6x4+6x2=32
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;//000
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	/*
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;//270	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;//180
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
	*/

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;//090
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X4_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-44M
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
	//const int FlashIndexA = 4;
	//const int FlashIndexB = 8;//0;//1,7,8
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	//6x4x2=48 
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex ++;
		
	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X4GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X4GCM
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
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }
	
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex ++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }
	
	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex=FlashIndexGrayCode;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		

	const int IndexCount=FlashIndex-FlashIndexGrayCode;
	FlashIndex=FlashIndexBinaryCode+IndexCount+1;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	FlashIndex ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X5GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X5GCM
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
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }
	
	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex=FlashIndexGrayCode;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	const int IndexCount=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCount;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X6GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X6GCM
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
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }

	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex = FlashIndexGrayCode;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+4;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	const int IndexCount=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCount;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X4GC_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X4GCM2
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
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const double Exp2Ratio=GetSecondExpRatio();	
	int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
	int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
	int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
	int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	
	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex++;	

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();		
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }

	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex = FlashIndexGrayCode;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex++;

	const int IndexCountAB=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCountAB;	
	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//third
	FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//forth
	FlashIndex = FlashIndexGrayCode;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2_2;
	PatItem.nExposure = PatExposure2_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex++;

	const int IndexCountCD=FlashIndex-FlashIndexGrayCode;
	FlashIndex = FlashIndexBinaryCode+IndexCountCD;
	PatItem.sFlashIndex = FlashIndex;	
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X5GC_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X5GCM2
{
	return ReturnNotSupportFunc(_T("CLight3DTiDLP4710::BuildDLPPatternList_4X5GC_M2"));		
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X43GC_M(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-4X4GCM
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
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }
	
	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex ++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	//second		
	const int FlashIndexGrayCode=GetPatternIndexGC_8Bit();
	const int FlashIndexBinaryCode=GetPatternIndexBC_8Bit();		
	const int GrayCodeStartNum=GetPatternStartNumGC_8Bit();//3
	const int BinaryCodeStartNum=GetPatternStartNumBC_8Bit();//17
	if ( CheckDLPPatternIndex(FlashIndexGrayCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false || CheckDLPPatternIndex(FlashIndexBinaryCode, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_GC_BC) == false )
	{	return false; }
	
	//GrayCode  :8+16+32+64+128+256+512
	//BinaryCode:8+16+32+64+128+256+512
	FlashIndex=FlashIndexGrayCode;	
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);		
	PatItem.sBitNum = GrayCodeStartNum;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex ++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = GrayCodeStartNum+2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex ++;

	const int IndexCount=FlashIndex-FlashIndexGrayCode;
	FlashIndex=FlashIndexBinaryCode+IndexCount+1;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-1;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	FlashIndex --;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = BinaryCodeStartNum-2;
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	FlashIndex --;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X2_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-42M2
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
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const double Exp2Ratio=GetSecondExpRatio();
	const bool   Use3BitPattern=GetUse3BitPattern();
	int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
	int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
	int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
	int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	
	
	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8	
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex++;
	
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	/*
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	*/
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex++;

	//增加以下樣板, 會讓取像間隔拉大至20 ms
	//third
	FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1_2;
	PatItem.nExposure = PatExposure1_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//forth	
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2_2;
	PatItem.nExposure = PatExposure2_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	/*
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;
	*/

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::BuildDLPPatternList_4X4_M2(bool IntTrig, bool MultiTable, int LEDColor)//建立樣板列表-44M2
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
	unsigned int NumImagesInFlash=GetPatternSetCountInDLP();

	const double Exp2Ratio=GetSecondExpRatio();	
	int PatPeriod1_2 = (int)(DLPParam.m_PeriodTime_us*Exp2Ratio);	
	int PatExposure1_2 = (int)(DLPParam.m_ExposureTime_us*Exp2Ratio);
	int PatPeriod2_2 = (int)(DLPParam.m_PeriodTime2_us*Exp2Ratio);
	int PatExposure2_2 = (int)(DLPParam.m_ExposureTime2_us*Exp2Ratio);	

	const int FlashIndexA = GetPatternIndex1_8Bit();//3;//0,6,0
	const int FlashIndexB = GetPatternIndex2_8Bit();//0;//1,7,8
	if ( CheckDLPPatternIndex(FlashIndexA, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false || CheckDLPPatternIndex(FlashIndexB, NumImagesInFlash, DLP_PATTERN_INDEX_MODE_WAVE) == false )
	{	return false; }

	int FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sBitDepth = DLP_BIT_DEPTH_8;
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
	FlashIndex++;
		
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//second
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2;
	PatItem.nExposure = PatExposure2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;		
	FlashIndex++;

	//增加以下樣板, 會讓取像間隔拉大至20 ms
	//third
	FlashIndex = FlashIndexA;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod1_2;
	PatItem.nExposure = PatExposure1_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	//forth
	FlashIndex = FlashIndexB;
	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 0;	
	PatItem.sBufSwap = true;
	PatItem.nPeriod = PatPeriod2_2;
	PatItem.nExposure = PatExposure2_2;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 1;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 2;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;
	FlashIndex++;

	PatItem.sFlashIndex = FlashIndex;
	PatItem.sTrigType = GetDLPTrigType(index, IntTrig, MultiTable);
	PatItem.sBitNum = 3;	
	PatItem.sBufSwap = false;
	this->AddDLPPPatItem(PatItem);
	index ++;	
	FlashIndex++;
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4710::GetDLPCurrentMax() const//取得DLP電流上限
{
	return DLP_LED_CURRENT_MAX;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetLEDColorUsed_Red() const//取得LED顏色使用-紅色
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetLEDColorUsed_Grn() const//取得LED顏色使用-綠色
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetLEDColorUsed_Blu() const//取得LED顏色使用-藍色
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::ResetLEDDisable(bool &ResetFinish, bool ShowMsg)
{
	if( false == m_EnableTemperatureMonitor ) { ResetFinish=true; return true; }	//不使用溫度監控
	return ReturnNotSupportFunc(_T("CLight3DTiDLP_Imp_4710::ResetLEDDisable"));	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetGPIOStatus(UINT PinNum, bool &Status)//取得GPIO pin 狀態
{
	return ReturnNotSupportFunc(_T("CLight3DTiDLP_Imp_4710::GetGPIOStatus"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::SetGPIOStatus(UINT PinNum, bool Status)//取得GPIO pin output 狀態
{
	return ReturnNotSupportFunc(_T("CLight3DTiDLP_Imp_4710::SetGPIOStatus"));	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetGPIOTemperatureOver(bool &IsOver, bool ShowMsg)//偵測溫度狀態。		GPIO11 input狀態。high高溫/low低溫
{
	//偵測溫度狀態。	GPIO11 input狀態。high高溫/low低溫
	double Temperature=0.0;
	if ( ReadSystemTemperature(&Temperature) == false )
	{	
		SaveDLPCurrentProcess(m_ErrorString, ShowMsg);		
		return false; 
	}

	IsOver = false;
	if ( Temperature > 100.0 )
	{
		IsOver = true;
		if( true == m_EnableTemperatureMonitor )
		{			
			m_ErrorString.Format(_T("Error, DLP high temperature[%.2f]"), Temperature);
			SaveDLPCurrentProcess(m_ErrorString, ShowMsg);			
		}
	} 
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp_4710::GetGPIOLEDDisable(bool &IsDisable, bool ShowMsg)//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除
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
bool CLight3DTiDLP_Imp_4710::SetGPIOLEDEnable()//LED disable狀態解除。	GPIO6  output狀態。 low->hi	
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
#endif//LIGHT_3D_TI_DLP_USE_IMP_4710
//-------------------------------------------------------------------------------------//