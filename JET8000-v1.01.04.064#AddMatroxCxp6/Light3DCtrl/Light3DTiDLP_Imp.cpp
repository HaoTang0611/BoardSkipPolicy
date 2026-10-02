// Light3DTiDLP_Imp.cpp: implementation of the CLight3DTiDLP_Imp class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP_Imp.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_IMP_USE
#define DLP_LED_CURRENT_MIN					0		//最低亮度
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp::CLight3DTiDLP_Imp():m_PreInitCount(0)
{
	PreInitTiDlp(0, LIGHT_3D_CAST_00);	
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp::CLight3DTiDLP_Imp(int CtrlID, LIGHT_3D_CAST_ID CastID):m_PreInitCount(0)
{
	PreInitTiDlp(CtrlID, CastID);
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp::~CLight3DTiDLP_Imp()
{
	ClearPhaseZeroBuffer();	
	ClearPhaseFactorBuffer();	
	m_PreInitCount = 0;
	::DeleteCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID CastID)
{	
	m_PreInitCount ++;
	if ( m_PreInitCount > 1 )
	{	return; }
	::InitializeCriticalSection(&m_csLight3D);

	m_dwFrmVersion = 0;	
	::memset(m_DLPFrmTag, 0x00, sizeof(m_DLPFrmTag));
	::memset(m_TiAPIversion, 0x00, sizeof(m_TiAPIversion));	
	::memset(m_DLPFrmversion, 0x00, sizeof(m_DLPFrmversion));	
	::memset(m_DLPMcuversion, 0x00, sizeof(m_DLPMcuversion));

	m_CtrlBoardID = CtrlID;
	m_Light3DCastID = CastID;	

	SetDLPImageW(0);
	SetDLPImageH(0);
	SetDeviceType(LIGHT_3D_DEVICE_NULL);	

	m_OperationMode = DLP_OPERATION_DEFAULT;

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
	
	switch ( CastID )
	{
	case LIGHT_3D_CAST_01:	m_CastName=_T("ID-01");	break;
	case LIGHT_3D_CAST_02:	m_CastName=_T("ID-02");	break;
	case LIGHT_3D_CAST_03:	m_CastName=_T("ID-03");	break;
	case LIGHT_3D_CAST_04:	m_CastName=_T("ID-04");	break;
	}
	this->InitialDLPParameter(m_DLPParam);		
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::InitialTiDlp()
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
	m_LEDColor = DLP_LED_COLOR_NO;

	m_DLPDelayTime = 20;
	m_TrigOutCount = 0;
	m_PatternSetCountInDLP = 25;

	m_Exposure_us = 0;
	m_TrigPeriod_us = 0;		
	m_Exposure2_us = 0;
	m_TrigPeriod2_us = 0;		

	::memset(m_HeightFactor0, 0x00, sizeof(m_HeightFactor0));
	::memset(m_HeightFactor1, 0x00, sizeof(m_HeightFactor1));
	::memset(m_HeightFactor2, 0x00, sizeof(m_HeightFactor2));
	return;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::LockLight3D()
{
	::EnterCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//}
void CLight3DTiDLP_Imp::UnlockLight3D()
{
	::LeaveCriticalSection(&m_csLight3D);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetDLPImageW(unsigned int val)
{
	m_DLPImageW = val;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetDLPImageH(unsigned int val)
{
	m_DLPImageH = val;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetDeviceType(LIGHT_3D_DEVICE_TYPE val)
{
	m_Light3DDevice = val;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::ReturnPatternIndex_Bit() const//回傳樣板引數-位元
{
	return -1;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ReturnNotImplement(LPCTSTR fnName)//回傳未完成函式
{
	m_ErrorString.Format(_T("Error, the Func[%s] not Implement"), fnName);
	return false;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetLCRErrorFnName(LPCTSTR LCRFnName)
{
	this->m_ErrorString.Format(_T("Error, TiDLP %s Fault"), LCRFnName);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[])
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetUSB_Number"));	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetDLPSafeCurrent(int value) const
{	
	int Max=m_LEDCurrentMax;
	int Min=MAX(DLP_LED_CURRENT_MIN, 0);	
	if ( value > Max ) { return Max; }
	if ( value < Min ) { return Min; }	
	return value;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SaveDLPCurrentProcess(LPCTSTR pContext, bool bShowMsg)//儲存現在狀態
{	
	m_ErrorString = pContext;

	CString Err = GetErrorString();	
	AOIDataCollect.SaveCurrentProcess(Err);
	if ( true == bShowMsg )
	{	JetAPI::ShowMessageBox(Err); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseZeroBufferFn()//清除平面相位	
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
bool CLight3DTiDLP_Imp::ClearPhaseFactorBufferFn()//清除平面係數
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
int CLight3DTiDLP_Imp::GetDLPTrigType(int index, bool IntTrig, bool MultiTable)
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
bool CLight3DTiDLP_Imp::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, true, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr)//載入DLP平面相位
{
#ifndef PHASE_CTRL_DISABLE
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP_Imp::LoadDLPPhaseZeroFile";
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
bool CLight3DTiDLP_Imp::SaveDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr)//儲存DLP平面相位
{
#ifndef PHASE_CTRL_DISABLE
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
#endif//PHASE_CTRL_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr)//載入DLP平面係數
{
#ifndef PHASE_CTRL_DISABLE
	if ( NULL == pfilename ) { return false; }
	const char fnName[] = "CLight3DTiDLP_Imp::LoadDLPPhaseFactorFile";
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
bool CLight3DTiDLP_Imp::SaveDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr)//儲存DLP平面係數
{
#ifndef PHASE_CTRL_DISABLE
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
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetDLPID(int ID)
{	
	m_CtrlBoardID = ID;
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP_Imp::GetDLPID() const
{
	return m_CtrlBoardID;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CAST_ID CLight3DTiDLP_Imp::GetCastID() const
{
	return m_Light3DCastID;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_DEVICE_TYPE CLight3DTiDLP_Imp::GetDeviceType() const
{
	return m_Light3DDevice;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP_Imp::GetDLPProjectName() const
{
	return this->m_CastName;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP_Imp::GetErrorString()
{	
	CString Key;
	CString CastName=GetDLPProjectName();	
	Key.Format(_T("[DLP:%s]"), CastName);
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP_Others(m_ErrorStringOut);
	return m_ErrorStringOut;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetAOIExceptionCode(DWORD Code)
{
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP(Code);
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP_Imp::GetDLPImageW() const
{
	return m_DLPImageW;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP_Imp::GetDLPImageH() const
{
	return m_DLPImageH;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::DLPConnect()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::DLPConnect"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::DLPDisconnect(int WaitTime_ms)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::DLPDisconnect"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPIsConnected()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPIsConnected"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::CheckDLPFrmForExpLut()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::CheckDLPFrmForExpLut"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SaveDLPProcess(LPCTSTR fnName, int Level)
{
	CString str;
	str.Format(_T("Light3D[%d]::%s"), m_Light3DCastID, fnName);
	if ( AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_LIGHT3D, Level, str) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPSoftwareReset(DWORD delayTime)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPSoftwareReset"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetDLPLongAxisImageFlip(bool Flip)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::SetDLPLongAxisImageFlip"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPLongAxisImageFlip()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPLongAxisImageFlip"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetDLPShortAxisImageFlip(bool Flip)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::SetDLPShortAxisImageFlip"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPShortAxisImageFlip()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPShortAxisImageFlip"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetDLPOperationMode(int Mode)
{
	m_OperationMode = Mode;
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::SetDLPOperationMode"));
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP_Imp::GetDLPOperationMode() const
{
	return m_OperationMode;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::SetDLPLEDEnable"));	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPLEDEnable"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::SetDLPLEDCurrent"));	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPLEDCurrent(int &red, int &green, int &blue)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPLEDCurrent"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetDLPLEDPWMInvert(bool bInvert)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::SetDLPLEDPWMInvert"));	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPLEDPWMInvert(bool &bInvert)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPLEDPWMInvert"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::CheckDLPStatus()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::CheckDLPStatus"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPStatus_InitDone()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPStatus_InitDone"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPStatus_ForcedSwap()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPStatus_ForcedSwap"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPStatus_BufferFreeze()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPStatus_BufferFreeze"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPStatus_SeqRunning()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPStatus_SeqRunning"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPStatus_SeqError()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPStatus_SeqError"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPStatus_SeqAbort()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPStatus_SeqAbort"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPStatus_DRCError()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPStatus_DRCError"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPStatus_DMDParked()
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::GetDLPStatus_DMDParked"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::InitialDLPParameter(TDLPParam &Param)
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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SaveDLPParameter()
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

	KeyName.Format(_T("Period Padding Time")); KeyString.Format(_T("%d"), m_DLPParam.m_PeriodPaddingTime);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}

	KeyName.Format(_T("Pattern Max Count")); KeyString.Format(_T("%d"), m_PatternSetCountInDLP);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false)
	{	IsOK = false;	}	
#endif//OFFLINE_VERSION
#endif//PHASE_CTRL_DISABLE
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::LoadDLPParameter()
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
		const int CurrentMin=10;
		const int CurrentMax=GetDLPCurrentMax();
		m_LEDCurrentMax = ::_ttoi(ReturnString);
		m_DLPParam.m_LEDCurrentMax = ::_ttoi(ReturnString);
		if (m_LEDCurrentMax < CurrentMin)
		{	m_LEDCurrentMax = CurrentMin;	}
		if (m_LEDCurrentMax > CurrentMax)
		{	m_LEDCurrentMax = CurrentMax;	}
	}

	//Image Gamma
	KeyName.Format(_T("Image Gamma")); KeyString.Format(_T("%.6f"), m_DLPParam.m_ImageGamma);
	if (  LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_ImageGamma = ::_ttof(ReturnString);	}
	m_ImageGamma = m_DLPParam.m_ImageGamma;

	KeyName.Format(_T("2nd Exposure Ratio")); KeyString.Format(_T("%.6f"), m_DLPParam.m_SecondExpRatio);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_SecondExpRatio = ::_ttof(ReturnString);	}
	m_SecondExpRatio = m_DLPParam.m_SecondExpRatio;

	KeyName.Format(_T("Period Padding Time")); KeyString.Format(_T("%d"), m_DLPParam.m_PeriodPaddingTime);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_DLPParam.m_PeriodPaddingTime = ::_ttoi(ReturnString);	}

	KeyName.Format(_T("Pattern Max Count")); KeyString.Format(_T("%d"), m_PatternSetCountInDLP);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true)
	{	m_PatternSetCountInDLP = ::_ttoi(ReturnString);	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ReadDLPParameter()//從DLP裝置讀取參數
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ReadDLPParameter"));
}
//-------------------------------------------------------------------------------------//
TDLPParam& CLight3DTiDLP_Imp::GetDLPParam()
{
	return m_DLPParam;
}
//-------------------------------------------------------------------------------------//
const TDLPParam& CLight3DTiDLP_Imp::GetDLPParam() const
{
	return m_DLPParam;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetDLPPhaseMode(int Mode)
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
		m_ErrorString.Format(_T("Error, SetDLPPhaseMode Fault[%d]"), Mode);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetDLPPhaseMode() const 
{ 
	return m_DLPParam.m_PhaseMode; 
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetDLPExposureTime() const 
{ 
	return m_DLPParam.m_ExposureTime_us; 
}	
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetDLPReadySignalEnable() const
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetDLPParamLEDCurrent(int red, int green, int blue, int CurrentID)
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
bool CLight3DTiDLP_Imp::GetDLPParamLEDCurrent(int &red, int &green, int &blue, int CurrentID)
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
const char* CLight3DTiDLP_Imp::GetDLPFrmTag()
{
	return this->m_DLPFrmTag;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP_Imp::GetTiAPIVersion()
{
	return this->m_TiAPIversion;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP_Imp::GetDLPFrmVersion()
{
	return this->m_DLPFrmversion;
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP_Imp::GetDLPMcuVersion()
{
	return this->m_DLPMcuversion;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetUseExpLut()
{
	if ( CheckDLPFrmForExpLut() == false )
	{	return false; }	
	return m_DLPParam.m_UseExpLut;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetLEDColor(int Type)
{
	m_LEDColor = Type;	
	return true;
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP_Imp::GetLEDColor() const
{
	return m_LEDColor;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP_Imp::GetImageGamma() const
{
	return m_ImageGamma;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetImageGamma(double val)
{
	m_ImageGamma = val;
	m_DLPParam.m_ImageGamma = val;
	return true;
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP_Imp::GetSecondExpRatio() const
{
	return m_SecondExpRatio;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetSecondExpRatio(double val)
{
	m_SecondExpRatio = val;
	m_DLPParam.m_SecondExpRatio = val;	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPeriodPaddingTime() const//週期外加時間-us
{
	return m_DLPParam.m_PeriodPaddingTime;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetExposurePaddingTime() const//曝光外加時間-us
{
	return m_DLPParam.m_ExposurePaddingTime;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::CalcPeriodPaddingTime(int ExpTime) const//計算週期外加時間-us
{
	return GetPeriodPaddingTime();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetUse3BitPattern() const
{
	const int PatternBitCount=GetPatternBitCount();
	if ( 3 != PatternBitCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetUse5BitPattern() const
{
	const int PatternBitCount=GetPatternBitCount();
	if ( 5 != PatternBitCount ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternBitCount() const
{
	return 0;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternBitCount(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternIndex1_3Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternIndex1_3Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternIndex2_3Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternIndex2_3Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternIndex1_5Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternIndex1_5Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternIndex1_6Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternIndex1_6Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternIndex2_6Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternIndex2_6Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternIndexGC_1Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternIndexGC_1Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternIndexBC_1Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternIndexBC_1Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternStartNumGC_1Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternStartNumGC_1Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp::GetPatternStartNumBC_1Bit() const
{
	return ReturnPatternIndex_Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPatternStartNumBC_1Bit(int val)
{
	return ;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPeriod_us(unsigned int val)
{
	m_TrigPeriod_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetExposure_us(unsigned int val)
{
	m_Exposure_us = val;	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPeriod2_us(unsigned int val)
{
	m_TrigPeriod2_us = val;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetExposure2_us(unsigned int val)
{
	m_Exposure2_us = val;		
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::InitialPatItem(TDLPPatItem &PatItem)
{
	PatItem = TDLPPatItem();	
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPPattern_Run()
{
	SaveDLPProcess(_T("ExecDLPPattern_Run"), MSG_LEVEL_HIGH);
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPPattern_Run"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPPattern_Stop()
{
	SaveDLPProcess(_T("ExecDLPPattern_Stop"), MSG_LEVEL_HIGH);
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPPattern_Stop"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPPattern_Pause()
{
	SaveDLPProcess(_T("ExecDLPPattern_Pause"), MSG_LEVEL_HIGH);
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPPattern_Pause"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::LEDSetting(int LEDCurrent, int CurrentID)
{
	SaveDLPProcess(_T("LEDSetting"), MSG_LEVEL_HIGH);
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::LEDSetting"));
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP_Imp::GetTriggerOutCount()
{
	return m_TrigOutCount;
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP_Imp::GetDLPPatCount()
{
	return m_PatternList.size();
}
//-------------------------------------------------------------------------------------//
TDLPPatItem* CLight3DTiDLP_Imp::GetDLPPatItemPtr(size_t index, bool check)
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
void CLight3DTiDLP_Imp::ClearDLPPatternList()
{
	SaveDLPProcess(_T("ClearDLPPatternList"), MSG_LEVEL_HIGH);
	m_PatternList.clear();
	m_PatternMode = DLP_PATTERN_SEQUENCE_DEBUG;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::BuildDLPPatternList"));
	m_TrigOutCount = m_PatternList.size();	
	if ( 0 == m_TrigOutCount )
	{
		m_ErrorString.Format(_T("Error, BuildDLPPatternList Fault[Mode=%d]"), Mode);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::AddDLPPPatItem(TDLPPatItem &PatItem)
{	//return ReturnNotImplement(_T("CLight3DTiDLP_Imp::AddDLPPPatItem"));
	AOIDataDefine.CalcDLPBitPosRange(PatItem.sBitDepth, PatItem.sBitNum, PatItem.sBitStart, PatItem.sBitEnd);
	m_PatternList.push_back(PatItem);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::RemoveDLPPatItem(size_t index)
{
	const size_t count = this->m_PatternList.size();
	if ( index >= count ) { return false; }

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
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPPatClear()
{
	SaveDLPProcess(_T("ExecDLPPatClear"), MSG_LEVEL_HIGH);
	m_PatternList.clear();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPPatRead"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPPatSendAll"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPPatSendOne"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPValidatePatLutData"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force)
{
	//return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPPatBuildSendValidate"));
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
bool CLight3DTiDLP_Imp::ExecDLPLightSetting(int CurrentID)
{
	return ReturnNotImplement(_T("CLight3DTiDLP_Imp::ExecDLPLightSetting"));
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::BuildPatternImage(int BitDepth, int NPeriod, int NPixelPeriod, bool bVer, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage)
{
	//G-8bit R-8bit B-8bit
	const char fnName[]="CLight3DTiDLP_Imp::BuildPatternImage";
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
CString  CLight3DTiDLP_Imp::GetDLPPhaseModeTextForBinFile(int Mode)
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
	}
	return AOIDataDefine.GetDLPPhaseModeText(Mode);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::CheckPhaseZeroRed()
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
bool CLight3DTiDLP_Imp::CheckPhaseZeroGrn()
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
bool CLight3DTiDLP_Imp::CheckPhaseZeroBlu()
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
bool CLight3DTiDLP_Imp::CheckPhaseZeroWhite()
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
bool CLight3DTiDLP_Imp::CheckPhaseZeroDebug()
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
bool CLight3DTiDLP_Imp::LoadPhaseZero()
{
	bool  IsOK = true;
	const int LEDColor = GetDLPParam().m_LEDColor;
	LockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:		IsOK = LoadPhaseZeroRed();	break;
	case DLP_LED_COLOR_GREEN:	IsOK = LoadPhaseZeroGrn();	break;
	case DLP_LED_COLOR_BLUE:	IsOK = LoadPhaseZeroBlu();	break;
	case DLP_LED_COLOR_DEBUG:	IsOK = LoadPhaseZeroDebug();break;
	default:
	case DLP_LED_COLOR_WHITE:	IsOK = LoadPhaseZeroWhite();break;
	}	
	UnlockLight3D();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::LoadPhaseZeroRed()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseZeroRed";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;
	
	FILE   *pFile = NULL;
	CString Filename;
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Red.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);

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
bool CLight3DTiDLP_Imp::LoadPhaseZeroGrn()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseZeroGrn";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;
	
	FILE   *pFile = NULL;
	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Green.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);

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
bool CLight3DTiDLP_Imp::LoadPhaseZeroBlu()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseZeroBlu";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;
	
	FILE   *pFile = NULL;
	CString Filename;
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Blue.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);		

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
bool CLight3DTiDLP_Imp::LoadPhaseZeroWhite()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseZeroWhite";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;
	
	FILE   *pFile = NULL;
	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_White.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);		

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
bool CLight3DTiDLP_Imp::LoadPhaseZeroDebug()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseZeroDebug";
	bool    IsOK = true;		
	IMAGE_SIZE   ImageW = 0;
	IMAGE_SIZE   ImageH = 0;
	IMAGE_SIZE   ImageStep = 0;
	PHASE_PTR    ImagePtr = NULL;
	size_t  NReads = 0;
	
	FILE   *pFile = NULL;
	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Debug.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);		

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
bool CLight3DTiDLP_Imp::SavePhaseZero()
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
bool CLight3DTiDLP_Imp::SavePhaseZeroRed()
{
	if ( CheckPhaseZeroRed() == false )
	{	return true; }	

	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Red.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SavePhaseZeroGrn()
{
	if ( CheckPhaseZeroGrn() == false )
	{	return true; }

	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Green.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SavePhaseZeroBlu()
{
	if ( CheckPhaseZeroBlu() == false )
	{	return true; }

	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Blue.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SavePhaseZeroWhite()
{
	if ( CheckPhaseZeroWhite() == false )
	{	return true; }

	CString Filename;
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_White.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SavePhaseZeroDebug()
{
	if ( CheckPhaseZeroDebug() == false )
	{	return true; }

	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseZeroPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Debug.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPZero"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseZeroFile(Filename, m_PhaseZeroW, m_PhaseZeroH, m_PhaseZeroStep, m_PhaseZeroPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseZeroBuffer()
{
	ClearPhaseZeroBufferRed();
	ClearPhaseZeroBufferGrn();
	ClearPhaseZeroBufferBlu();
	ClearPhaseZeroBufferWhite();
	ClearPhaseZeroBufferDebug();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseZeroBufferRed()
{
	return ClearPhaseZeroBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseZeroBufferGrn()
{
	return ClearPhaseZeroBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseZeroBufferBlu()
{
	return ClearPhaseZeroBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseZeroBufferWhite()
{
	return ClearPhaseZeroBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseZeroBufferDebug()
{
	return ClearPhaseZeroBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr)
{
	const char fnName[] = "CLight3DTiDLP_Imp::SetPhaseZero";	
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
bool CLight3DTiDLP_Imp::GetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)
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
		default:					
		case DLP_LED_COLOR_WHITE:	IsOK=LoadPhaseZeroWhite();	break;
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
bool CLight3DTiDLP_Imp::ClonePhaseZero(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)
{
	IMAGE_SIZE PhaseZeroW=0;
	IMAGE_SIZE PhaseZeroH=0;
	IMAGE_SIZE PhaseZeroStep=0;
	PHASE_PTR  PhaseZeroPtr=NULL;
	const int PhaseMode = m_PhaseZeroPhaseMode;		
	if ( GetPhaseZero(PhaseMode, LEDColor, PhaseZeroW, PhaseZeroH, PhaseZeroStep, PhaseZeroPtr) == false )
	{	return false; }

	JetMemory.free_func(Ptr);
	const char fnName[] = "CLight3DTiDLP_Imp::ClonePhaseZero";	
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
bool CLight3DTiDLP_Imp::CheckPhaseFactorRed()
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_RED!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Red]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::CheckPhaseFactorGrn()
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_GREEN!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Green]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::CheckPhaseFactorBlu()
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_BLUE!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Blue]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::CheckPhaseFactorWhite()
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_WHITE!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[White]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::CheckPhaseFactorDebug()
{
	if ( NULL==m_PhaseFactorPtr || DLP_LED_COLOR_DEBUG!=m_PhaseFactorLEDColor)
	{
		this->m_ErrorString.Format(_T("Error, Phase Factor Ptr is NULL[Debug]"));
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::LoadPhaseFactor()
{
	bool  IsOK = true;
	const int LEDColor = GetDLPParam().m_LEDColor;
	LockLight3D();
	switch ( LEDColor )
	{
	case DLP_LED_COLOR_RED:		IsOK = LoadPhaseFactorRed();	break;
	case DLP_LED_COLOR_GREEN:	IsOK = LoadPhaseFactorGrn();	break;
	case DLP_LED_COLOR_BLUE:	IsOK = LoadPhaseFactorBlu();	break;
	case DLP_LED_COLOR_DEBUG:	IsOK = LoadPhaseFactorDebug();	break;
	default:
	case DLP_LED_COLOR_WHITE:
		IsOK = LoadPhaseFactorWhite();
		break;
	}
	UnlockLight3D();
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::LoadPhaseFactorRed()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseFactorRed";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	
	
	FILE   *pFile = NULL;
	CString Filename;
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Red.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
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
bool CLight3DTiDLP_Imp::LoadPhaseFactorGrn()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseFactorGrn";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	
	
	FILE   *pFile = NULL;
	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Green.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
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
bool CLight3DTiDLP_Imp::LoadPhaseFactorBlu()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseFactorBlu";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	
	
	FILE   *pFile = NULL;
	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Blue.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
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
bool CLight3DTiDLP_Imp::LoadPhaseFactorWhite()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseFactorWhite";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	
	
	FILE   *pFile = NULL;
	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_White.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
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
bool CLight3DTiDLP_Imp::LoadPhaseFactorDebug()
{
	const char fnName[] = "CLight3DTiDLP_Imp::LoadPhaseFactorDebug";
	bool    IsOK = true;		
	IMAGE_SIZE    ImageW = 0;
	IMAGE_SIZE    ImageH = 0;
	IMAGE_SIZE    ImageStep = 0;
	SPACE_PTR     ImagePtr = NULL;
	size_t   NReads = 0;	
	
	FILE   *pFile = NULL;
	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Debug.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
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
bool CLight3DTiDLP_Imp::SavePhaseFactor()
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
bool CLight3DTiDLP_Imp::SavePhaseFactorRed()
{
	if ( CheckPhaseFactorRed() == false )
	{	return true; }	
	
	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Red.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);	
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SavePhaseFactorGrn()
{
	if ( CheckPhaseFactorGrn() == false )
	{	return true; }

	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Green.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SavePhaseFactorBlu()
{
	if ( CheckPhaseFactorBlu() == false )
	{	return true; }

	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Blue.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SavePhaseFactorWhite()
{
	if ( CheckPhaseFactorWhite() == false )
	{	return true; }

	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_White.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SavePhaseFactorDebug()
{
	if ( CheckPhaseFactorDebug() == false )
	{	return true; }

	CString Filename;		
	CString ProjectName = this->GetDLPProjectName();	
	CString PhaseMode = GetDLPPhaseModeTextForBinFile(m_PhaseFactorPhaseMode);
	Filename.Format(_T("%s\\%s%s_%s_Debug.BIN"), AOIDataCollect.GetAOIDirectory(), _T("TiDLPFactor"), PhaseMode, ProjectName);		
	if ( SaveDLPPhaseFactorFile(Filename, m_PhaseFactorW, m_PhaseFactorH, m_PhaseFactorStep, m_PhaseFactorPtr) == false ) 
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseFactorBuffer()
{
	ClearPhaseFactorBufferRed();
	ClearPhaseFactorBufferGrn();
	ClearPhaseFactorBufferBlu();
	ClearPhaseFactorBufferWhite();
	ClearPhaseFactorBufferDebug();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseFactorBufferRed()
{
	return ClearPhaseFactorBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseFactorBufferGrn()
{
	return ClearPhaseFactorBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseFactorBufferBlu()
{
	return ClearPhaseFactorBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseFactorBufferWhite()
{
	return ClearPhaseFactorBufferFn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::ClearPhaseFactorBufferDebug()
{
	return ClearPhaseFactorBufferFn();
}
//-------------------------------------------------------------------------------------//
double  CLight3DTiDLP_Imp::GetPhaseFactorMin() const
{
	return m_DLPParam.m_PhaseFactorMin;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPhaseFactorMin(double val)
{
	m_DLPParam.m_PhaseFactorMin = val;
}
//-------------------------------------------------------------------------------------//
double  CLight3DTiDLP_Imp::GetPhaseFactorMax() const
{
	return m_DLPParam.m_PhaseFactorMax;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::SetPhaseFactorMax(double val)
{
	m_DLPParam.m_PhaseFactorMax = val;
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP_Imp::GetLEDCurrentMax() const
{
	return m_LEDCurrentMax;
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP_Imp::GetDLPCurrentMax() const//取得DLP電流上限
{
	return 255;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetLEDColorUsed_Red() const//取得LED顏色使用-紅色
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetLEDColorUsed_Grn() const//取得LED顏色使用-綠色
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::GetLEDColorUsed_Blu() const//取得LED顏色使用-藍色
{
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp::ClearHeightFactor()
{
	::memset(m_HeightFactor0, 0x00, sizeof(m_HeightFactor0));
	::memset(m_HeightFactor1, 0x00, sizeof(m_HeightFactor1));
	::memset(m_HeightFactor2, 0x00, sizeof(m_HeightFactor2));
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SortHeightFactorTableList()
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
bool CLight3DTiDLP_Imp::SaveHeightFactorTableList()
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
	LIGHT_3D_CAST_ID CastID = GetCastID();	
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
bool CLight3DTiDLP_Imp::LoadHeightFactorTableList()
{
#ifndef PHASE_CTRL_DISABLE
	int   i=0;
	CString  filename;	
	int      FileNo=0;
	const bool bTemp = false;	
	const int MaxFileNo = MAX_HEIGHT_TARGET_COUNT;
	LIGHT_3D_CAST_ID CastID = GetCastID();	

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
bool CLight3DTiDLP_Imp::RestoreHeightFactorTableList()
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
void CLight3DTiDLP_Imp::ClearHeightFactorTableList(bool bIncludeFiles)
{	
	LIGHT_3D_CAST_ID CastID = GetCastID();
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
bool CLight3DTiDLP_Imp::BuildHeightFactorMappingParam(bool bRecv)
{
#ifndef PHASE_CTRL_DISABLE
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		CString filename;
		TPhaseFactorGrid Grid;		
		LIGHT_3D_CAST_ID CastID=GetCastID();
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
bool CLight3DTiDLP_Imp::CalcHeightFactorMappingParam(std::vector<std::vector<TPhaseFactorGrid>> &GridListArray, std::vector<std::vector<double>> &ParamListArray)
{
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		CString filename;		
		LIGHT_3D_CAST_ID CastID=GetCastID();
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
bool CLight3DTiDLP_Imp::VerifyHeightFactorMappingParam(const std::vector<std::vector<double>> &ParamListArray, std::vector<TPhaseFactorGrid> &GridList, double &Error)
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
bool CLight3DTiDLP_Imp::GetHeightFactorMappingParam(double T0[], double T1[], double T2[])
{
	::memcpy(T0, m_HeightFactor0, sizeof(m_HeightFactor0));
	::memcpy(T1, m_HeightFactor1, sizeof(m_HeightFactor1));
	::memcpy(T2, m_HeightFactor2, sizeof(m_HeightFactor2));		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP_Imp::SetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr)
{
	const char fnName[] = "CLight3DTiDLP_Imp::SetPhaseFactor";	
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
	case DLP_LED_COLOR_WHITE:
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
bool CLight3DTiDLP_Imp::GetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)
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
		default:					
		case DLP_LED_COLOR_WHITE:	IsOK=LoadPhaseFactorWhite();	break;
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
bool CLight3DTiDLP_Imp::ClonePhaseFactor(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)
{
	IMAGE_SIZE PhaseFactorW=0;
	IMAGE_SIZE PhaseFactorH=0;
	IMAGE_SIZE PhaseFactorStep=0;
	SPACE_PTR  PhaseFactorPtr=NULL;
	const int PhaseMode = m_PhaseFactorPhaseMode;			
	if ( GetPhaseFactor(PhaseMode, LEDColor, PhaseFactorW, PhaseFactorH, PhaseFactorStep, PhaseFactorPtr) == false )
	{	return false; }

	const char fnName[] = "CLight3DTiDLP_Imp::ClonePhaseFactor";
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
bool CLight3DTiDLP_Imp::SetHeightFactorTable(int TargetNo, const TPhaseFactorTable &GridTable)
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
bool CLight3DTiDLP_Imp::CloneHeightFactorTable(int TargetNo, TPhaseFactorTable &GridTable) const
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
void CLight3DTiDLP_Imp::DLPSettingDelay() const//DLP設定時要先延遲一段時間
{
	if ( m_DLPDelayTime > 0 )
	{	::Sleep(m_DLPDelayTime); }
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//