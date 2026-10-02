// Light3DTiDLP.cpp: implementation of the CLight3DTiDLP class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLight3DTiDLP::CLight3DTiDLP():m_Imp(NULL)
{	
	PreInitTiDlp(0, LIGHT_3D_CAST_00);	
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP::CLight3DTiDLP(int CtrlID, LIGHT_3D_CAST_ID CastID):m_Imp(NULL)
{	
	PreInitTiDlp(CtrlID, CastID);
	InitialTiDlp();
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP::~CLight3DTiDLP()
{
	ReleaseImp();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID CastID)
{	
	if ( CreateImp(CtrlID, CastID) == false ) { return; }
	if ( CheckImp() == false ) { return; }
	//m_Imp->PreInitTiDlp(CtrlID, CastID);	//建構子已經呼叫
	return;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::InitialTiDlp()
{	
	if ( CheckImp() == false ) { return; }
	m_Imp->InitialTiDlp();	
	return;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::LockLight3D()
{
	if ( CheckImp() == false ) { return; }
	m_Imp->LockLight3D();	
}
//-------------------------------------------------------------------------------------//}
void CLight3DTiDLP::UnlockLight3D()
{
	if ( CheckImp() == false ) { return; }
	m_Imp->UnlockLight3D();
}
//-------------------------------------------------------------------------------------//
const CLight3DTiDLP_Imp* CLight3DTiDLP::GetImp() const//取得Imp的指標	
{
	return m_Imp;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckImp()//確認Imp的指標
{
	if ( NULL == GetImp() )
	{
		m_ErrorString=_T("Error, CLight3DTiDLP Imp Exception");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckImp() const//確認Imp的指標
{
	if ( NULL == GetImp() )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CreateImp(int CtrlID, LIGHT_3D_CAST_ID CastID)//建立Imp的指標
{
	ReleaseImp();	
	bool  bIsOK=false;	
	LIGHT_3D_DEVICE_TYPE DeviceType=LoadDeviceType(CastID);	
	switch ( DeviceType )
	{
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4500
	case LIGHT_3D_DEVICE_DLP4500: bIsOK = CreateImp_DLP4500(CtrlID, CastID); break;
#endif//LIGHT_3D_TI_DLP_USE_IMP_4500

#ifdef LIGHT_3D_TI_DLP_USE_IMP_4710
	case LIGHT_3D_DEVICE_DLP4710: bIsOK = CreateImp_DLP4710(CtrlID, CastID); break;
#endif//LIGHT_3D_TI_DLP_USE_IMP_4710

	default:
		m_ErrorString.Format(_T("Error, CLight3DTiDLP::CreateImp Fault[%d]"), DeviceType);
		break;
	}	
	return bIsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ReleaseImp()//釋放Imp的指標	
{
	if ( NULL == m_Imp ) { return true; }
	delete m_Imp; 
	m_Imp = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLight3DTiDLP::GetIniSectionName(LIGHT_3D_CAST_ID CastID)
{
	CString Section, CastName;	
	switch ( CastID )
	{	
	case LIGHT_3D_CAST_01:	CastName=_T("ID-01");	break;
	case LIGHT_3D_CAST_02:	CastName=_T("ID-02");	break;
	case LIGHT_3D_CAST_03:	CastName=_T("ID-03");	break;
	case LIGHT_3D_CAST_04:	CastName=_T("ID-04");	break;
	default: CastName.Format(_T("ID-%02d"), CastID); break;
	}
	Section.Format(_T("TiDLP-%s"), CastName);
	return Section;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_DEVICE_TYPE CLight3DTiDLP::LoadDeviceType(LIGHT_3D_CAST_ID CastID)//載入裝置型號
{	
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];
	CString KeyName, KeyString;
	CString Section = GetIniSectionName(CastID);
	CString FileName = AOIDataCollect.GetLight3DParamFilename();	

	//Device Type
	LIGHT_3D_DEVICE_TYPE Type = LIGHT_3D_DEVICE_DLP4500;
	KeyName.Format(_T("Device Type")); KeyString.Format(_T("%d"),Type);
	if ( JetAPI::LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, m_ErrorString) == true )	
	{	Type = (LIGHT_3D_DEVICE_TYPE)(::_ttoi(ReturnString)); }
	else
	{	JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString); }
	return Type;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[])
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetUSB_Number(CastID, USB_Number);	
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetDLPSafeCurrent(int value)
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPSafeCurrent(value);		
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseZeroBufferFn()//清除平面相位	
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseZeroBufferFn();
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetDLPTrigType(int index, bool IntTrig, bool MultiTable)
{  
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPTrigType(index, IntTrig, MultiTable);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr)//載入DLP平面相位
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadDLPPhaseZeroFile(pfilename, PhaseW, PhaseH, PhaseStep, PhasePtr);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SaveDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr)//儲存DLP平面相位
{
#ifndef OFFLINE_VERSION
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SaveDLPPhaseZeroFile(pfilename, PhaseW, PhaseH, PhaseStep, PhasePtr);
#endif//OFFLINE_VERSION
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr)//載入DLP平面係數
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadDLPPhaseFactorFile(pfilename, SpaceW, SpaceH, SpaceStep, SpacePtr);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SaveDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr)//儲存DLP平面係數
{
#ifndef OFFLINE_VERSION
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SaveDLPPhaseFactorFile(pfilename, SpaceW, SpaceH, SpaceStep, SpacePtr);
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetDLPID(int ID)
{	
	if ( CheckImp() == false ) { return; }
	m_Imp->SetDLPID(ID);	
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP::GetDLPID()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPID();	
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CAST_ID CLight3DTiDLP::GetCastID()
{
	if ( CheckImp() == false ) { return LIGHT_3D_CAST_00; }
	return m_Imp->GetCastID();	
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_DEVICE_TYPE CLight3DTiDLP::GetDeviceType()
{
	if ( CheckImp() == false ) { return LIGHT_3D_DEVICE_NULL; }
	return m_Imp->GetDeviceType();	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ChangeDeviceType(LIGHT_3D_DEVICE_TYPE Type)//變更裝置型號
{
	if ( CheckImp() == false ) { return false; }
	const int CtrlID = m_Imp->GetDLPID();
	LIGHT_3D_CAST_ID CastID = m_Imp->GetCastID();		

	CString KeyName, KeyString;
	CString Section = GetIniSectionName(CastID);
	CString FileName = AOIDataCollect.GetLight3DParamFilename();	

	//Device Type
	KeyName.Format(_T("Device Type")); KeyString.Format(_T("%d"),Type);
	JetAPI::SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString);

	if ( CreateImp(CtrlID, CastID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP::GetDLPProjectName()
{
	if ( CheckImp() == false ) { return NULL; }
	return m_Imp->GetDLPProjectName();
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DTiDLP::GetErrorString()
{		
	if ( CheckImp() == false )
	{
		AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP_Others(m_ErrorString);
		return m_ErrorString; 
	}
	m_ErrorString = m_Imp->GetErrorString();
	AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP_Others(m_ErrorString);
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP::GetDLPImageW()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPImageW();
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP::GetDLPImageH()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPImageH();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::DLPConnect()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->DLPConnect();	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::DLPDisconnect(int WaitTime_ms)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->DLPDisconnect();	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPIsConnected()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPIsConnected();	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckDLPFrmForExpLut()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckDLPFrmForExpLut();	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SaveDLPProcess(LPCTSTR fnName, int Level)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SaveDLPProcess(fnName, Level);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPSoftwareReset(DWORD delayTime)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPSoftwareReset(delayTime);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetDLPLongAxisImageFlip(bool Flip)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetDLPLongAxisImageFlip(Flip);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPLongAxisImageFlip()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPLongAxisImageFlip();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetDLPShortAxisImageFlip(bool Flip)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetDLPShortAxisImageFlip(Flip);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPShortAxisImageFlip()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPShortAxisImageFlip();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetDLPOperationMode(int Mode)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetDLPOperationMode(Mode);
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP::GetDLPOperationMode()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPOperationMode();	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen, bool bBlue)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetDLPLEDEnable(bSeqCtrl, bRed, bGreen, bBlue);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPLEDEnable(bSeqCtrl, bRed, bGreen, bBlue);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetDLPLEDCurrent(red, green, blue, Update, CurrentID);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPLEDCurrent(int &red, int &green, int &blue)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPLEDCurrent(red, green, blue);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetDLPLEDPWMInvert(bool bInvert)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetDLPLEDPWMInvert(bInvert);	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPLEDPWMInvert(bool &bInvert)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPLEDPWMInvert(bInvert);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckDLPStatus()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckDLPStatus();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPStatus_InitDone()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPStatus_InitDone();	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPStatus_ForcedSwap()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPStatus_ForcedSwap();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPStatus_BufferFreeze()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPStatus_BufferFreeze();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPStatus_SeqRunning()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPStatus_SeqRunning();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPStatus_SeqError()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPStatus_SeqError();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPStatus_SeqAbort()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPStatus_SeqAbort();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPStatus_DRCError()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPStatus_DRCError();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPStatus_DMDParked()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPStatus_DMDParked();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::InitialDLPParameter(TDLPParam &Param)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->InitialDLPParameter(Param);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SaveDLPParameter()
{
	bool    IsOK = true;	
#ifndef OFFLINE_VERSION
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SaveDLPParameter();
#endif//OFFLINE_VERSION
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadDLPParameter()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadDLPParameter();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ReadDLPParameter()//從DLP裝置讀取參數
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ReadDLPParameter();
}
//-------------------------------------------------------------------------------------//
TDLPParam& CLight3DTiDLP::GetDLPParam()
{
	if ( CheckImp() == false ) { return m_DLPParamDummy; }
	return m_Imp->GetDLPParam();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetDLPPhaseMode(int Mode)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetDLPPhaseMode(Mode);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetDLPPhaseMode() 
{ 
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPPhaseMode();
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetDLPExposureTime() 
{ 
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPExposureTime();
}	
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPReadySignalEnable()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPReadySignalEnable();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetDLPParamLEDCurrent(int red, int green, int blue, int CurrentID)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetDLPParamLEDCurrent(red, green, blue, CurrentID);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetDLPParamLEDCurrent(int &red, int &green, int &blue, int CurrentID)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetDLPParamLEDCurrent(red, green, blue, CurrentID);
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP::GetDLPFrmTag()
{
	if ( CheckImp() == false ) { return NULL; }
	return m_Imp->GetDLPFrmTag();
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP::GetTiAPIVersion()
{
	if ( CheckImp() == false ) { return NULL; }
	return m_Imp->GetTiAPIVersion();
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP::GetDLPFrmVersion()
{
	if ( CheckImp() == false ) { return NULL; }
	return m_Imp->GetDLPFrmVersion();
}
//-------------------------------------------------------------------------------------//
const char* CLight3DTiDLP::GetDLPMcuVersion()
{
	if ( CheckImp() == false ) { return NULL; }
	return m_Imp->GetDLPMcuVersion();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetUseExpLut()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetUseExpLut();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetLEDColor(int Type)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetLEDColor(Type);
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP::GetLEDColor()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetLEDColor();
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP::GetImageGamma()
{
	if ( CheckImp() == false ) { return 1.0; }
	return m_Imp->GetImageGamma();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetImageGamma(double val)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetImageGamma(val);
}
//-------------------------------------------------------------------------------------//
double CLight3DTiDLP::GetSecondExpRatio()
{
	if ( CheckImp() == false ) { return 1.0; }
	return m_Imp->GetSecondExpRatio();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetSecondExpRatio(double val)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetSecondExpRatio(val);
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP::GetPeriodPaddingTime()//週期外加時間-us
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetPeriodPaddingTime();
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetExposurePaddingTime()//曝光外加時間-us
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetExposurePaddingTime();
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::CalcPeriodPaddingTime(int ExpTime)//計算週期外加時間-us
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->CalcPeriodPaddingTime(ExpTime);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetUse3BitPattern()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetUse3BitPattern();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetUse5BitPattern()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetUse5BitPattern();
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternBitCount()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetPatternBitCount();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternBitCount(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternBitCount(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternIndex1_3Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternIndex1_3Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternIndex1_3Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternIndex1_3Bit(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternIndex2_3Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternIndex2_3Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternIndex2_3Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternIndex2_3Bit(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternIndex1_5Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternIndex1_5Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternIndex1_5Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternIndex1_5Bit(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternIndex1_6Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternIndex1_6Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternIndex1_6Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternIndex1_6Bit(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternIndex2_6Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternIndex2_6Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternIndex2_6Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternIndex2_6Bit(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternIndexGC_1Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternIndexGC_1Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternIndexGC_1Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternIndexGC_1Bit(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternIndexBC_1Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternIndexBC_1Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternIndexBC_1Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternIndexBC_1Bit(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternStartNumGC_1Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternStartNumGC_1Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternStartNumGC_1Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternStartNumGC_1Bit(val);
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP::GetPatternStartNumBC_1Bit()
{
	if ( CheckImp() == false ) { return -1; }
	return m_Imp->GetPatternStartNumBC_1Bit();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPatternStartNumBC_1Bit(int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPatternStartNumBC_1Bit(val);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPeriod_us(unsigned int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPeriod_us(val);	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetExposure_us(unsigned int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetExposure_us(val);	
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPeriod2_us(unsigned int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPeriod2_us(val);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetExposure2_us(unsigned int val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetExposure2_us(val);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::InitialPatItem(TDLPPatItem &PatItem)
{
	if ( CheckImp() == false ) { return ; }
	m_Imp->InitialPatItem(PatItem);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPPattern_Run()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPPattern_Run();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPPattern_Stop()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPPattern_Stop();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPPattern_Pause()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPPattern_Pause();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LEDSetting(int LEDCurrent, int CurrentID)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LEDSetting(LEDCurrent, CurrentID);
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP::GetTriggerOutCount()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetTriggerOutCount();
}
//-------------------------------------------------------------------------------------//
size_t CLight3DTiDLP::GetDLPPatCount()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetDLPPatCount();
}
//-------------------------------------------------------------------------------------//
TDLPPatItem* CLight3DTiDLP::GetDLPPatItemPtr(size_t index, bool check)
{
	if ( CheckImp() == false ) { return NULL; }
	return m_Imp->GetDLPPatItemPtr(index, check);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::ClearDLPPatternList()
{
	if ( CheckImp() == false ) { return; }
	return m_Imp->ClearDLPPatternList();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->BuildDLPPatternList(Mode, IntTrig, MultiTable, LEDColor);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::AddDLPPPatItem(TDLPPatItem &PatItem)
{	
	if ( CheckImp() == false ) { return false; }
	return m_Imp->AddDLPPPatItem(PatItem);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::RemoveDLPPatItem(size_t index)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->RemoveDLPPatItem(index);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPPatClear()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPPatClear();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPPatRead(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPPatSendAll(bExpLut, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPPatSendOne(bExpLut, index, TrigPeriod_us, Exposure_us, bPatFrmVideo, bTrigIntExt, bRepeat);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPValidatePatLutData(Status, Sleep);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPPatBuildSendValidate(Mode, IntTrig, MultiTable, Periodus, exposure_us, LEDColor, Sleep, Force);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ExecDLPLightSetting(int CurrentID)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ExecDLPLightSetting(CurrentID);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::BuildPatternImage(int BitDepth, int NPeriod, int NPixelPeriod, bool bVer, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->BuildPatternImage(BitDepth, NPeriod, NPixelPeriod, bVer, ImageW, ImageH, ImageStep, pImage);
}
//-------------------------------------------------------------------------------------//
CString  CLight3DTiDLP::GetDLPPhaseModeTextForBinFile(int Mode)
{
	if ( CheckImp() == false ) { return CString(_T("")); }
	return m_Imp->GetDLPPhaseModeTextForBinFile(Mode);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseZeroRed()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseZeroRed();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseZeroGrn()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseZeroGrn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseZeroBlu()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseZeroBlu();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseZeroWhite()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseZeroWhite();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseZeroDebug()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseZeroDebug();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseZero()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseZero();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseZeroRed()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseZeroRed();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseZeroGrn()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseZeroGrn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseZeroBlu()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseZeroBlu();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseZeroWhite()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseZeroWhite();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseZeroDebug()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseZeroDebug();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseZero()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseZero();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseZeroRed()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseZeroRed();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseZeroGrn()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseZeroGrn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseZeroBlu()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseZeroBlu();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseZeroWhite()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseZeroWhite();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseZeroDebug()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseZeroDebug();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseZeroBuffer()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseZeroBuffer();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseZeroBufferRed()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseZeroBufferRed();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseZeroBufferGrn()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseZeroBufferGrn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseZeroBufferBlu()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseZeroBufferBlu();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseZeroBufferWhite()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseZeroBufferWhite();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseZeroBufferDebug()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseZeroBufferDebug();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetPhaseZero(PhaseMode, LEDColor, ImageW, ImageH, ImageSetp, Ptr);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetPhaseZero(PhaseMode, LEDColor, ImageW, ImageH, ImageSetp, Ptr);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClonePhaseZero(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClonePhaseZero(LEDColor, ImageW, ImageH, ImageSetp, Ptr);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseFactorRed()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseFactorRed();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseFactorGrn()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseFactorGrn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseFactorBlu()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseFactorBlu();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseFactorWhite()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseFactorWhite();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CheckPhaseFactorDebug()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CheckPhaseFactorDebug();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseFactor()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseFactor();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseFactorRed()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseFactorRed();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseFactorGrn()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseFactorGrn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseFactorBlu()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseFactorBlu();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseFactorWhite()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseFactorWhite();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadPhaseFactorDebug()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadPhaseFactorDebug();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseFactor()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseFactor();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseFactorRed()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseFactorRed();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseFactorGrn()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseFactorGrn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseFactorBlu()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseFactorBlu();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseFactorWhite()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseFactorWhite();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SavePhaseFactorDebug()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SavePhaseFactorDebug();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseFactorBuffer()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseFactorBuffer();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseFactorBufferRed()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseFactorBufferRed();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseFactorBufferGrn()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseFactorBufferGrn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseFactorBufferBlu()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseFactorBufferBlu();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseFactorBufferWhite()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseFactorBufferWhite();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClearPhaseFactorBufferDebug()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClearPhaseFactorBufferDebug();
}
//-------------------------------------------------------------------------------------//
double  CLight3DTiDLP::GetPhaseFactorMin()
{
	if ( CheckImp() == false ) { return 0.0; }
	return m_Imp->GetPhaseFactorMin();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPhaseFactorMin(double val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPhaseFactorMin(val);
}
//-------------------------------------------------------------------------------------//
double  CLight3DTiDLP::GetPhaseFactorMax()
{
	if ( CheckImp() == false ) { return 0.0; }
	return m_Imp->GetPhaseFactorMax();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::SetPhaseFactorMax(double val)
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->SetPhaseFactorMax(val);
}
//-------------------------------------------------------------------------------------//
int  CLight3DTiDLP::GetLEDCurrentMax()
{
	if ( CheckImp() == false ) { return 0; }
	return m_Imp->GetLEDCurrentMax();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetLEDColorUsed_Red()//取得LED顏色使用-紅色
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetLEDColorUsed_Red();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetLEDColorUsed_Grn()//取得LED顏色使用-綠色
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetLEDColorUsed_Grn();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetLEDColorUsed_Blu()//取得LED顏色使用-藍色
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetLEDColorUsed_Blu();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::ClearHeightFactor()
{
	if ( CheckImp() == false ) { return ; }
	return m_Imp->ClearHeightFactor();	
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SortHeightFactorTableList()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SortHeightFactorTableList();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SaveHeightFactorTableList()
{
#ifndef OFFLINE_VERSION
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SaveHeightFactorTableList();
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::LoadHeightFactorTableList()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->LoadHeightFactorTableList();
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::RestoreHeightFactorTableList()
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->RestoreHeightFactorTableList();
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP::ClearHeightFactorTableList(bool bIncludeFiles)
{		
	if ( CheckImp() == false ) { return ; }
	return m_Imp->ClearHeightFactorTableList(bIncludeFiles);
	return;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::BuildHeightFactorMappingParam(bool bRecv)
{
#ifndef OPENCV_DISABLE
	if ( CheckImp() == false ) { return false; }
	return m_Imp->BuildHeightFactorMappingParam(bRecv);
#else
	m_ErrorString = _T("Error, Cast Build Height Factor Need OpenCV Enabled");
	return false;
#endif//OPENCV_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CalcHeightFactorMappingParam(std::vector<std::vector<TPhaseFactorGrid>> &GridListArray, std::vector<std::vector<double>> &ParamListArray)
{
#ifndef OPENCV_DISABLE
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CalcHeightFactorMappingParam(GridListArray, ParamListArray);
#else
	m_ErrorString = _T("Error, Cast Build Height Factor Need OpenCV Enabled");
	return false;
#endif//OPENCV_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::VerifyHeightFactorMappingParam(const std::vector<std::vector<double>> &ParamListArray, std::vector<TPhaseFactorGrid> &GridList, double &Error)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->VerifyHeightFactorMappingParam(ParamListArray, GridList, Error);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetHeightFactorMappingParam(double T0[], double T1[], double T2[])
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetHeightFactorMappingParam(T0, T1, T2);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetPhaseFactor(PhaseMode, LEDColor, ImageW, ImageH, ImageSetp, Ptr);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::GetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->GetPhaseFactor(PhaseMode, LEDColor, ImageW, ImageH, ImageSetp, Ptr);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::ClonePhaseFactor(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->ClonePhaseFactor(LEDColor, ImageW, ImageH, ImageSetp, Ptr);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::SetHeightFactorTable(int TargetNo, const TPhaseFactorTable &GridTable)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->SetHeightFactorTable(TargetNo, GridTable);
}
//-------------------------------------------------------------------------------------//
bool CLight3DTiDLP::CloneHeightFactorTable(int TargetNo, TPhaseFactorTable &GridTable)
{
	if ( CheckImp() == false ) { return false; }
	return m_Imp->CloneHeightFactorTable(TargetNo, GridTable);
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//