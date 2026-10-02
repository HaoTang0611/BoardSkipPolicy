// LightCtrlBoard.cpp: implementation of the CLightCtrlBoard class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "LightCtrlBoard.h"
#include "LightCtrlBoardImp.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CLightCtrlBoard LightCtrlBoard;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::BuildTableTypeCombox(CComboBox &Combox)//建立表格樣式
{
	CString str;
	int     idx=0;	
	int     Param=0;

	JetAPI::ClearCombox(Combox);

	str = _T("LED");
	Param=TABLE_TYPE_LED;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("3D");
	Param=TABLE_TYPE_DLP;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::BuildTable3DCastCombox(CComboBox &Combox)//建立3D投光編號
{
	CString str;
	int     idx=0;	
	int     Param=0;

	JetAPI::ClearCombox(Combox);

	int i=0;
	const int Max3DCast = 8;
	for ( i=0; i<=Max3DCast; i++ )
	{
		if ( 0 == i )
		{	str = _T("None"); }
		else
		{	str.Format(_T("%d"), i); }
		Param = i;
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Param);
		idx ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetTableDLPChannelMask(int DLPChannel)//取得表格內DLP引數的遮罩碼
{
	int Mask = 0;
	switch ( DLPChannel ) 
	{
	case 1:	Mask = 0x01;	break;
	case 2:	Mask = 0x02;	break;
	case 3:	Mask = 0x04;	break;
	case 4:	Mask = 0x08;	break;
	case 5:	Mask = 0x10;	break;
	case 6:	Mask = 0x20;	break;
	case 7:	Mask = 0x40;	break;
	case 8:	Mask = 0x80;	break;
	}
	return Mask;
}
//-------------------------------------------------------------------------------------//
CLightCtrlBoard::CLightCtrlBoard()
{	
	_Imp = NULL;
	LIGHT_CTRL_BOARD_IMP_CLS ImpCls=LIGHT_CTRL_BOARD_IMP_FPGA;
#ifdef LIGHT_CTRL_BOARD_USE_ARDUINO
	ImpCls=LIGHT_CTRL_BOARD_IMP_ARDUINO;	
#endif//LIGHT_CTRL_BOARD_USE_ARDUINO

	CreateImp(ImpCls);
	SwitchImp();
}
//-------------------------------------------------------------------------------------//
CLightCtrlBoard::~CLightCtrlBoard()
{
	ReleaseImp();	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetImp() const//取得_Imp指標
{
	if ( NULL == _Imp )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckImp()//確認_Imp指標
{
	if ( NULL == _Imp )
	{
		m_ErrorString=_T("Error, CLightCtrlBoard::_Imp is NULL");
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_OTHERS);
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CreateImp(LIGHT_CTRL_BOARD_IMP_CLS Cls)//建立_Imp指標	
{
	bool IsOK = false;
	switch ( Cls )
	{
	case LIGHT_CTRL_BOARD_IMP_FPGA:		IsOK=CreateImp_Fpga();	break;
	case LIGHT_CTRL_BOARD_IMP_ARDUINO:	IsOK=CreateImp_Arduino();	break;
	default:
		m_ErrorString=_T("Error, CLightCtrlBoard CreateImp Fault");
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_OTHERS);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchImp()//切換_Imp指標
{	
#ifndef LIGHT_CTRL_BOARD_USE_IMP_CLS
	return true;
#endif//LIGHT_CTRL_BOARD_USE_IMP_CLS
	if ( CheckImp() == false ) { return false; }	
	LIGHT_CTRL_BOARD_IMP_CLS nClass;	
	LIGHT_CTRL_BOARD_TYPE Type=_Imp->GetLightCtrlBoardType();	
	LIGHT_CTRL_BOARD_IMP_CLS eClass=_Imp->GetLightCtrlBoardClass();	
	
	switch ( Type )
	{
	case LIGHT_CTRL_BOARD_3DA6:
	case LIGHT_CTRL_BOARD_8DA1:
		nClass = LIGHT_CTRL_BOARD_IMP_FPGA;
		break;
	case LIGHT_CTRL_BOARD_ARDUINO:	
		nClass = LIGHT_CTRL_BOARD_IMP_ARDUINO;		
		break;
	default:
		nClass = LIGHT_CTRL_BOARD_IMP_RETURN;
		break;
	}
	if ( LIGHT_CTRL_BOARD_IMP_RETURN==nClass )
	{
		m_ErrorString=_T("Error, CLightCtrlBoard SwitchImp Fault");
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_OTHERS);
		return false;
	}
	if ( nClass == eClass )
	{	return true; }
	
	
	ReleaseImp();
	if ( CreateImp(nClass) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReleaseImp()//釋放_Imp指標
{
	if ( NULL == _Imp ) { return true; }
	delete _Imp; 
	_Imp = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLightCtrlBoard::GetErrorKeyName() const
{
	CString Key=_T("[LightCtrlBoard]");	
	return Key;
}
//-------------------------------------------------------------------------------------//
LPCTSTR  CLightCtrlBoard::GetErrorString()
{
	//if ( CheckImp() == true )
	//{	m_ErrorString = _Imp->GetErrorString();	}	
	CString Key=GetErrorKeyName();
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_LightCtrlBoard_Others(m_ErrorStringOut);	
	return m_ErrorStringOut;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetErrorString(LPCTSTR Err)
{
	m_ErrorString = Err;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetLightCtrlBoardExceptionCode_Param(LPCTSTR Err)//設定系統錯誤代碼
{
	CString Key=GetErrorKeyName();	
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	CString strOut=JetAPI::AddKeyToErrorString(str, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Param(strOut);
	//SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_PARAM, strOut);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetLightCtrlBoardExceptionCode_FileRead(LPCTSTR Err)//設定系統錯誤代碼
{
	CString Key=GetErrorKeyName();	
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	CString strOut=JetAPI::AddKeyToErrorString(str, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileRead(strOut);	
	//SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_FILE_READ, strOut);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetLightCtrlBoardExceptionCode_FileWrite(LPCTSTR Err)//設定系統錯誤代碼
{
	CString Key=GetErrorKeyName();	
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	CString strOut=JetAPI::AddKeyToErrorString(str, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileWrite(strOut);
	//SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_FILE_WRITE, strOut);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetLightCtrlBoardExceptionCode(DWORD Code, LPCTSTR Err)//設定系統錯誤代碼
{
	CString Key=GetErrorKeyName();	
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	CString strOut=JetAPI::AddKeyToErrorString(str, Key);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_LightCtrlBoard(Code, strOut);	
	return;
}
//-------------------------------------------------------------------------------------//
LIGHT_CTRL_BOARD_IMP_CLS CLightCtrlBoard::GetLightCtrlBoardClass()
{
#ifndef LIGHT_CTRL_DISABLE	
	if ( CheckImp() == false ) { return LIGHT_CTRL_BOARD_IMP_NULL; }
	return _Imp->GetLightCtrlBoardClass();	
#endif//LIGHT_CTRL_DISABLE
	return LIGHT_CTRL_BOARD_IMP_NULL;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::Connect()
{
#ifndef LIGHT_CTRL_DISABLE	
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->Connect() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_CONNECT);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::Connect(int BoardID, char BoardName[], char BoardCode[])
{	
#ifndef LIGHT_CTRL_DISABLE		
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->Connect(BoardID, BoardName, BoardCode) == false )	
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_CONNECT);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetIsConnected()
{	
#ifndef LIGHT_CTRL_DISABLE		
	if ( CheckImp() == false ) { return false; }
	return _Imp->GetIsConnected();		
#endif//LIGHT_CTRL_DISABLE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckIsConnected()
{
#ifndef LIGHT_CTRL_DISABLE		
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->CheckIsConnected() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_CONNECT);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::DisConnect()
{
#ifndef LIGHT_CTRL_DISABLE	
	if ( CheckImp() == false ) { return true; }
	if ( _Imp->DisConnect() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_RELEASE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetUSBBoardID()
{
	if ( CheckImp() == false ) { return 0; }
	return _Imp->GetUSBBoardID();	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetCameraCount() const//取得相機數量
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetCameraCount();
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetDLPCastCount() const//取得DLP投光數量
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetDLPCastCount();
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetLEDChannelCount() const
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetLEDChannelCount();
}
//-------------------------------------------------------------------------------------//
LIGHT_CTRL_BOARD_TYPE CLightCtrlBoard::GetLightCtrlBoardType() const
{
	if ( GetImp() == false ) { return LIGHT_CTRL_BOARD_NULL; }
	return _Imp->GetLightCtrlBoardType();	
}
//-------------------------------------------------------------------------------------//
CString CLightCtrlBoard::GetLightCtrlBoardTypeText() const//取得控制板樣式文字
{
	if ( GetImp() == false ) { return CString(_T("NULL")); }
	return _Imp->GetLightCtrlBoardTypeText();
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ChangeLightCtrlBoardType(LIGHT_CTRL_BOARD_TYPE Type)//變更控制板樣式	
{
	if ( GetImp() == false ) { return false; }
	if ( _Imp->WriteLightCtrlBoardType(Type) == false )
	{	return false; }
	if ( SwitchImp() == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoopBackTestAll()
{	
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->LoopBackTestAll() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_LOOP_BACK);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoopBackTest(int data, char DataWS[], char DataRA[], char DataRB[])
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->LoopBackTest(data, DataWS, DataRA, DataRB) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_LOOP_BACK);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoopBackTestA(int data, char DataWS[], char DataR[])
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->LoopBackTestA(data, DataWS, DataR) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_LOOP_BACK);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoopBackTestB(int data, char DataWS[], char DataR[])
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->LoopBackTestB(data, DataWS, DataR) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_LOOP_BACK);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadLightCtrlBorad(const std::string &Address, std::string &Data)
{	
#ifndef LIGHT_CTRL_DISABLE	
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ReadLightCtrlBorad(Address, Data) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::WriteLightCtrlBorad(const std::string &Address, const std::string &Data)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->WriteLightCtrlBorad(Address, Data) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SaveLightCtrlBoardINI()//寫參數至檔案
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SaveLightCtrlBoardINI() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::LoadLightCtrlBoardINI()//從檔案讀取參數
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->LoadLightCtrlBoardINI() == false )
	{ 
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode_FileRead();
		return false; 
	}
	if ( SwitchImp() == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadRAMData(char data[])
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ReadRAMData(data) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadRAMAddress(char data[])
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ReadRAMAddress(data) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckDLPNotReadySignalFault(int DlpIdx)//確認DLP沒有準備訊號
{
	if ( CheckImp() == false ) { return false; }
	return _Imp->CheckDLPNotReadySignalFault(DlpIdx);	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr)
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->CheckLightCtrlBoardError(IsError, ErrorStr) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadErrorString(CString &Error)
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ReadErrorString(Error) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::DecodeErrorCodeText(int ErrorCode, CString &ErrorStr)
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->DecodeErrorCodeText(ErrorCode, ErrorStr) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetTriggerCountText(CString &TrigCount)
{	
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetTriggerCountText(TrigCount) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadFPGAVersion(CString &Version)
{	
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ReadFPGAVersion(Version) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLightCtrlBoard::GetFPGAVersion() const
{
	if ( GetImp() == false ) { return CString(_T("")); }
	return _Imp->GetFPGAVersion();	
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLightCtrlBoard::GetFullVersion() const
{
	if ( GetImp() == false ) { return CString(_T("")); }
	return _Imp->GetFullVersion();		
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetIsWorkingNow(bool &Working)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetIsWorkingNow(Working) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetIsFPGAOK(bool &IsOK)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetIsFPGAOK(IsOK) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetDLPEnable(UINT DLPChannel)
{	
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SetDLPEnable(DLPChannel) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadBoardAllCount()
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ReadBoardAllCount() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetPCtoFPGATrigCount(UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetPCtoFPGATrigCount(count) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_GET_FUNC);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetFPGAtoCCDTrigCount(UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetFPGAtoCCDTrigCount(count) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_GET_FUNC);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetFPGAtoCCDTrigCount(UINT CameraID, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetFPGAtoCCDTrigCount(CameraID, count) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_GET_FUNC);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetFPGAtoDLPTrigCount(UINT DLPChannel, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetFPGAtoDLPTrigCount(DLPChannel, count) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_GET_FUNC);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetDLPtoFPGATrigCount(UINT DLPChannel, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetDLPtoFPGATrigCount(DLPChannel, count) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_GET_FUNC);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TriggerStart()//TriggerStart
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	_Imp->SaveLightCtrlBoardProcess(_T("TriggerStart()"));
	if ( _Imp->TriggerStart() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TRIGGER);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ClearAllCount()//ClearAllCount
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ClearAllCount() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_CLEAR);
		return false;
	}
	_Imp->SaveLightCtrlBoardProcess(_T("ClearAllCount()"));
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ClearAll()//ClearAll
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ClearAll() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_CLEAR);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchToFPGA()
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SwitchToFPGA() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchToRead()
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SwitchToRead() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchToWrite()
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SwitchToWrite() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SwitchToAssign()
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SwitchToAssign() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetMode_FPGA()//FPGA Run
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SetMode_FPGA() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetMode_PCWrite()//寫入
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SetMode_PCWrite() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetMode_PCRead()//讀取
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SetMode_PCRead() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetMode_PCAssign()//指定位址
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SetMode_PCAssign() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::WriteTableRunCount(int nTable)//Trigger後要跑幾張TABLE
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->WriteTableRunCount(nTable) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReadTableRunCount(int &nTable)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( GetImp() == false ) { return false; }
	if ( _Imp->ReadTableRunCount(nTable) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetTableRunCount() const
{ 
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetTableRunCount();
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetCCDTrigEdge(int TrigEdge)//CCD Trigger Edge
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SetCCDTrigEdge(TrigEdge) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::EnableDLPChannel(int DLPMask)//新增加的DLP開啟功能
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->EnableDLPChannel(DLPMask) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetCCDTrigEdge(int &TrigEdge)
{
#ifndef LIGHT_CTRL_DISABLE	
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetCCDTrigEdge(TrigEdge) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::SetTriggerFirstIndex(UINT Index)//處發表格第1張引數
{
#ifndef LIGHT_CTRL_DISABLE	
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->SetTriggerFirstIndex(Index) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int  CLightCtrlBoard::GetDLPTableStartIndex() const
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetDLPTableStartIndex();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetDLPTableStartIndex(int val)
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return ; }
	return _Imp->SetDLPTableStartIndex(val);	
#endif//LIGHT_CTRL_DISABLE
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetDLPInteralTrigger() const
{
	bool InteralTrig = true;
#ifndef LIGHT_CTRL_DISABLE
	if ( GetImp() == false ) { return InteralTrig; }
	InteralTrig=_Imp->GetDLPInteralTrigger();	
#else
	InteralTrig = true;
#endif//LIGHT_CTRL_DISABLE
	return InteralTrig;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetDLPMultiTableEnabled() const
{ 
	if ( GetImp() == false ) { return true; }
	return _Imp->GetDLPMultiTableEnabled();	
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetDLPMultiTableEnabled(bool val)
{	
	if ( CheckImp() == false ) { return ; }
	return _Imp->SetDLPMultiTableEnabled(val);	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetSupportMoreThan64Table() const
{
	if ( GetImp() == false ) { return false; }
	return _Imp->GetSupportMoreThan64Table();
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetSupportModifyTriggerFirstIndex() const
{ 	
	if ( GetImp() == false ) { return false; }
	return _Imp->GetSupportModifyTriggerFirstIndex();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetTableRecheckEnabled(int val)
{
	if ( CheckImp() == false ) { return ; }
	_Imp->SetTableRecheckEnabled(val);	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetTableRecheckEnabled() const
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetTableRecheckEnabled();
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetDLPCallbackTimeus(int val)
{
	if ( CheckImp() == false ) { return ; }
	_Imp->SetDLPCallbackTimeus(val);
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetDLPCallbackTimeus() const
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetDLPCallbackTimeus();	
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetDLPPulseWidthTimeus(int val)
{
	if ( CheckImp() == false ) { return ; }
	_Imp->SetDLPPulseWidthTimeus(val);	 
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetDLPPulseWidthTimeus() const
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetDLPPulseWidthTimeus();	
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetTableMinBetweenTimeus(int val)
{
	if ( CheckImp() == false ) { return ; }
	_Imp->SetTableMinBetweenTimeus(val);
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetTableMinBetweenTimeus() const
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetTableMinBetweenTimeus();	
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoard::SetPreCheckTableListEnabled(int val)
{
	if ( CheckImp() == false ) { return ; }
	_Imp->SetPreCheckTableListEnabled(val);	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetPreCheckTableListEnabled() const
{
	if ( GetImp() == false ) { return 0; }
	return _Imp->GetPreCheckTableListEnabled();	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableRAMAssign(UINT AssignAddress)//指定RAM位址
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->TableRAMAssign(AssignAddress) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableAssign(UINT TableIndex)//指定RAM位址至某個Table的起始位址
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->TableAssign(TableIndex) == false )	
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableRAMAddressAdd()//RAM位址加1
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->TableRAMAddressAdd() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableSingleWrite(TLCB_TRIG_TABLE &Table)//寫入一個Table
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->TableSingleWrite(Table) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_WRITE);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableSingleRead(TLCB_TRIG_TABLE &Table)//讀出一個Table
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->TableSingleRead(Table) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_READ);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableCompare(const TLCB_TRIG_TABLE &Table1, const TLCB_TRIG_TABLE &Table2)//比較2個Table
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->TableCompare(Table1, Table2) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_COMPARE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableListRead(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable)
{
	if ( TableListReadFn(TableList, nTable) == false )
	{
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_READ);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableListReadFn(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable)
{
	TableList.clear();
#ifndef LIGHT_CTRL_DISABLE
	int             i=0;
	CString         str;
	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;
	TLCB_TRIG_TABLE tmpTable;
		
	JetAPI::SetFuncTimeStart(fnStart);
	if ( CheckImp() == false ) { return false; }
	str.Format(_T("TableListRead[%d] Start"), nTable);
	_Imp->SaveLightCtrlBoardProcess(str);
	if ( _Imp->TableListRead(TableList, nTable) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		return false; 
	}

	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("TableListRead[%d] Time=%.3f ms"), nTable, fnTime);
	_Imp->SaveLightCtrlBoardProcess(str);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList)
{
	if ( TableListWriteFn(TableList) == false )
	{
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_WRITE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableListWriteFn(const std::vector<TLCB_TRIG_TABLE> &TableList)
{
#ifndef LIGHT_CTRL_DISABLE
	const int PreCheckTableList = GetPreCheckTableListEnabled();	
	if ( FN_ENABLE == PreCheckTableList )
	{
		if ( TableListCompare(TableList) == true ) 
		{	return true; }
	}

	CString str;
	size_t  i=0;
	int     tmpData=0;
	DWORD   DLPEnableMask=0;	
	TLCB_TRIG_TABLE tmpTable;
	TLCB_TRIG_TABLE tmpTableA;
	TLCB_TRIG_TABLE tmpTableB;	
	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;
	std::vector<TLCB_TRIG_TABLE>  TableListTotal = TableList;
	const size_t nTable = TableList.size();		

	JetAPI::SetFuncTimeStart(fnStart);	
	if ( CheckImp() == false ) { return false; }
	str.Format(_T("TableListWrite[%d] Start"), nTable);
	_Imp->SaveLightCtrlBoardProcess(str);
	if ( _Imp->TableListWrite(TableList) == false ) 
	{
		SetErrorString(_Imp->GetErrorString());
		return false; 
	}

	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("TableListWrite[%d] Time=%.3f ms"), nTable, fnTime);
	_Imp->SaveLightCtrlBoardProcess(str);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableListCompare(const std::vector<TLCB_TRIG_TABLE> &TableList)
{	
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->TableListCompare(TableList) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_COMPARE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ReMapTableList_ReadToWrite(const std::vector<TLCB_TRIG_TABLE> &TableListRead, std::vector<TLCB_TRIG_TABLE> &TableListWrite)
{	//因為DLP的Channel是查表的所以要反查回去
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ReMapTableList_ReadToWrite(TableListRead, TableListWrite) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_CONVERT);
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::TableReset(int nTable)
{
#ifndef LIGHT_CTRL_DISABLE
	if( nTable <= 0 )  { return true; }
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->TableReset(nTable) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_WRITE);
		return false; 
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ClearLCBTableList()//清空內部的列表
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ClearLCBTableList() == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_WRITE);
		return false; 
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoard::GetCurFPGAMode()
{
	if ( CheckImp() == false ) { return FPGA_MODE_UNDEFINED; }
	return _Imp->GetCurFPGAMode();
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::DefaultTable(TLCB_TRIG_TABLE &Table)
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->DefaultTable(Table) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::GetTableInfoText(TLCB_TRIG_TABLE &Table, CString &Info)
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->GetTableInfoText(Table, Info) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_READ);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckTableListEqually(const std::vector<TLCB_TRIG_TABLE> &TableListIn, const std::vector<TLCB_TRIG_TABLE> &TableListOut, std::vector<int> &NGTableList)//確認兩個表格是否相等
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->CheckTableListEqually(TableListIn, TableListOut, NGTableList) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_COMPARE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CheckTableEqually(size_t idx, const TLCB_TRIG_TABLE &TableIn, const TLCB_TRIG_TABLE &TableOut)//確認兩個表格是否相等
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->CheckTableEqually(idx, TableIn, TableOut) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_COMPARE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::WriteTableListToFile(LPCTSTR filename, std::vector<TLCB_TRIG_TABLE> &TableList, std::vector<int> &NGTableList)//將表格列表寫至檔案中
{
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->WriteTableListToFile(filename, TableList, NGTableList) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::BuildLCBTableFromSliceParamList(const std::vector<TSliceParam> &SliceParamList)//由SliceParm列表來建立燈盤表格列表
{
	if ( BuildLCBTableFromSliceParamListFn(SliceParamList) == false )
	{
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_CONVERT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::BuildLCBTableFromSliceParamListFn(const std::vector<TSliceParam> &SliceParamList)//由SliceParm列表來建立燈盤表格列表	
{
#ifndef LIGHT_CTRL_DISABLE	
	int i=0;
	int DLPIndex = -1;
	int DLPChannel=0;
	int DLPChannelMask = 0;
	int DLPMask = 0;//0x01|0x02|0x04|0x08;
	int DLPPeriod_us = 0;//us
	int DLPExposure_us = 0;//us
	int DLPLEDColor = DLP_LED_COLOR_WHITE;
	TLCB_TRIG_TABLE LCBTable;
	std::vector<TLCB_TRIG_TABLE> TableList;
	const bool bForce = false;
	const bool bSleep = false;	
	const bool DLPMultiTable = GetDLPMultiTableEnabled();
	const bool DLPInternalTrigger = GetDLPInteralTrigger();
	LIGHT_3D_CLS_PTR PhasePtr = NULL;
	LIGHT_3D_CAST_ID Light3DID = LIGHT_3D_CAST_00;
	
	SetDLPTableStartIndex(-1);
	if( ClearAll() == false )	
	{	return false;	}
	if ( ConvertSliceParamListToLCBTableList(SliceParamList, TableList) == false )
	{	return false;	}	
	
	int   DLPStartIndex = -1;
	const int TriggerTableCount = (int)(TableList.size());
	for ( i=0; i<TriggerTableCount; i++ )
	{
		LCBTable = TableList[i];
		if ( LIGHT_DLP != LCBTable.sTableType )	{	continue; }
		if ( -1 == DLPStartIndex )
		{	DLPStartIndex = (int)(i); }
		DLPChannelMask = CLightCtrlBoard::GetTableDLPChannelMask(LCBTable.sDLPActiveChannel);
		if ( 0 == (DLPMask&DLPChannelMask) ) 
		{	DLPMask |= DLPChannelMask;	}
		else//開啟過了
		{	continue;	}

		Light3DID = CLight3DCtrl::GetLight3DCastIDByDLPChannel(LCBTable.sDLPActiveChannel);		
		PhasePtr = Light3DCtrl.GetLight3DCastPtr(Light3DID);
		if ( NULL == PhasePtr ) 
		{
			m_ErrorString = Light3DCtrl.GetErrorString();
			return false;
		}		
		DLPIndex = LCBTable.sDLPActiveChannel-1;		
		const int NFrames = LCBTable.sDLPTrigOutNumber;
		const int PhasePatMode = LCBTable.sDLPPhasePatMode;
		//const int DLPCameraExpTime = CameraExposureTime_us;
		const int DLPCameraExpTime = LCBTable.sDLPCCDExpTime;
		if ( DLP_PATTERN_SEQUENCE_DEBUG == PhasePatMode )	
		{
			m_ErrorString.Format(_T("Error, Phase Pattern Sequence Exception"));
			SetLightCtrlBoardExceptionCode_Param();	
			return false;
		}		
		DLPPeriod_us = PhasePtr->GetDLPParam().m_PeriodTime_us;
		DLPExposure_us = PhasePtr->GetDLPParam().m_ExposureTime_us;		
		DLPLEDColor = PhasePtr->GetDLPParam().m_LEDColor;
		if ( DLPCameraExpTime < DLPExposure_us )
		{
			m_ErrorString.Format(_T("Error, Camera Exp. Time[%d] < DLP Exp. Time[%d]"), DLPCameraExpTime, DLPExposure_us);
			return false;
		}
		if ( DLPCameraExpTime < DLPPeriod_us )
		{
			m_ErrorString.Format(_T("Error, Camera Exp. Time[%d] < DLP Period Time[%d]"), DLPCameraExpTime, DLPPeriod_us);
			return false;
		}
		if ( PhasePtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )		
		{
			m_ErrorString = PhasePtr->GetErrorString();
			return false;
		}
		
		//Run
		if ( PhasePtr->ExecDLPPattern_Run()  == false )
		{
			m_ErrorString = PhasePtr->GetErrorString();
			return false;
		}					
	}	
	SetDLPTableStartIndex(DLPStartIndex);
	if ( false == bSleep )//如果DLP的設定沒有延遲時間的話
	{	JetAPI::TimeDelay_TickCount(50);	}
	
	if ( TableListWrite(TableList) == false )
	{	return false;	}	

	const int CheckTable = GetTableRecheckEnabled();
	if ( FN_ENABLE == CheckTable )
	{
		::Sleep(50);		
		std::vector<int>             NGTableList;
		std::vector<TLCB_TRIG_TABLE> TableListOut;
		std::vector<TLCB_TRIG_TABLE> TableListIn=TableList;
		const size_t TableCountIn = TableListIn.size();
		if ( TableListRead(TableListOut, TableCountIn) == false ) 
		{	return false;	}
		if ( CheckTableListEqually(TableListIn, TableListOut, NGTableList) == false )
		{
			CString tmpfile;
			tmpfile.Format(_T("%s\\LightCtrlTableFault.TXT"), AOIDataCollect.GetAOITempDirectory());
			WriteTableListToFile(tmpfile, TableListOut, NGTableList);
			::ShellExecute(NULL, _T("open"), tmpfile, NULL, NULL, SW_SHOW);			
			return false; 
		}
	}

	if ( WriteTableRunCount(TriggerTableCount) == false )
	{	return false;	}		
	
	if( SetMode_FPGA() == false )	
	{	return false;	}
#endif//LIGHT_CTRL_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ConvertSliceParamListToLCBTableList(const std::vector<TSliceParam> &SliceParamList, std::vector<TLCB_TRIG_TABLE> &LCBTableList)//將Slice Param轉成控制板的表格
{
	const bool bUsing3D = true;
	if ( ConvertSliceParamListToLCBTableList(SliceParamList, bUsing3D, LCBTableList) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ConvertSliceParamListToLCBTableList(const std::vector<TSliceParam> &SliceParamList, bool bUsing3D, std::vector<TLCB_TRIG_TABLE> &LCBTableList)//將Slice Param轉成控制板的表格
{
	if ( ConvertSliceParamListToLCBTableListFn(SliceParamList, bUsing3D, LCBTableList) == false )
	{
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_TABLE_CONVERT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ConvertSliceParamListToLCBTableListFn(const std::vector<TSliceParam> &SliceParamList, bool bUsing3D, std::vector<TLCB_TRIG_TABLE> &LCBTableList)//將Slice Param轉成控制板的表格	
{
	bool IsOK = true;	
	const bool DLPMultiTable = GetDLPMultiTableEnabled();
	if ( true == bUsing3D )
	{
		if ( false == DLPMultiTable )//DLP使用單張表格
		{	IsOK = ConvertSliceParamListToLCBTableList_DLPSingle(SliceParamList, LCBTableList);	}
		else//DLP使用多張表格
		{	IsOK = ConvertSliceParamListToLCBTableList_DLPMultiple(SliceParamList, LCBTableList);	}
		return IsOK;
	}
	else
	{
		size_t i=0;
		std::vector<TSliceParam> SliceParamListTmp;
		const size_t Cnt=SliceParamList.size();
		for ( i=0; i<Cnt; i++ )
		{
			const TSliceParam &SliceParamRef=SliceParamList[i];
			if ( SLICE_UNIQUE_ID_DLP == SliceParamRef.SliceUniqueID ) { continue; }
			SliceParamListTmp.push_back(SliceParamRef);
		}
		if ( false == DLPMultiTable )//DLP使用單張表格
		{	IsOK = ConvertSliceParamListToLCBTableList_DLPSingle(SliceParamListTmp, LCBTableList);	}
		else//DLP使用多張表格
		{	IsOK = ConvertSliceParamListToLCBTableList_DLPMultiple(SliceParamListTmp, LCBTableList);	}
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ConvertSliceParamListToLCBTableList_DLPSingle(const std::vector<TSliceParam> &SliceParamList, std::vector<TLCB_TRIG_TABLE> &LCBTableList)//將Slice Param轉成控制板的表格
{
	CString str;
	size_t           i=0, j=0;
	int              TempI = 0;	
	TLEDTable        LEDTable;
	CAMERA_ID        CameraID = PRIMARY_CAMERA_ID;
	TSliceParam      SliceParam;	
	TLCB_LED_ITEM    LCBPWM, *pLCBPWM=NULL;		
	TLCB_TRIG_TABLE  LCBTable;
	const size_t     SliceParamCount = SliceParamList.size();	
	
	int TableID = 0;
	int TableType = 0;
	int DLPIndex = 0;
	int CameraDelayTime = 0;
	int CameraExpTime = 0;//(int)(SliceParam.SliceCameraExpTimeus);
	int       CameraFrameTimeus = 0;
	int       LastTableTimeus = 0;
	int       LastTableBtTimeus = 0;
	int       TablePeriodTimeus = 0;		
	const int DLPCallbackTime = GetDLPCallbackTimeus();
	const int MinTabeBetweenTimeus = GetTableMinBetweenTimeus();		

	for ( i=0; i<SliceParamCount; i++ )
	{
		SliceParam = SliceParamList[i];
		switch ( SliceParam.SliceLightTable.LightType )
		{
		case LIGHT_LED:	TableType =  TABLE_TYPE_LED;	break;
		case LIGHT_DLP:	TableType =  TABLE_TYPE_DLP;	break;
		default:        TableType =  TABLE_TYPE_NONE;	break;
		}
		if ( TABLE_TYPE_NONE == TableType )
		{
			this->m_ErrorString.Format(_T("Error, Slice Param Ligth Type Exception [Index:%d, Value:%d]"), i+1, SliceParam.SliceLightTable.LightType);
			return false;
		}
	#ifdef PHASE_CTRL_DISABLE
		if ( TABLE_TYPE_DLP == TableType )
		{	continue; }
	#endif//PHASE_CTRL_DISABLE

		CameraID = SliceParam.SliceCameraID;		
		CameraFrameTimeus = (int)(CameraCtrl.GetCameraMinPeriod(CameraID));//us		
		CameraExpTime = (int)(SliceParam.SliceCameraExpTimeus);
		CameraDelayTime = (int)(SliceParam.SliceCameraDelayTimeus);
		TablePeriodTimeus = CameraFrameTimeus;
		if ( LIGHT_LED == TableType )
		{	
			TableID = (int)(LCBTableList.size());		
			DefaultTable(LCBTable);
			LCBTable.sTableID = TableID;		
			LCBTable.sTableType = TableType;
			LCBTable.sDLPTrigOutNumber = 0;	//DLP觸發數量
			LCBTable.sDLPActiveChannel = 0;	//DLP通道
			LCBTable.sDLPReadySignalEnable = FN_DISABLE;
		
			for ( j=0; j<LED_CHANNEL_COUNT; j++ )
			{			
				switch ( j )
				{
				case 0:	pLCBPWM = &LCBTable.sPWM1;	break;
				case 1:	pLCBPWM = &LCBTable.sPWM2;	break;
				case 2:	pLCBPWM = &LCBTable.sPWM3;	break;
				case 3:	pLCBPWM = &LCBTable.sPWM4;	break;
				case 4:	pLCBPWM = &LCBTable.sPWM5;	break;
				case 5:	pLCBPWM = &LCBTable.sPWM6;	break;
				case 6:	pLCBPWM = &LCBTable.sPWM7;	break;
				case 7:	pLCBPWM = &LCBTable.sPWM8;	break;			
				case 8:	pLCBPWM = &LCBTable.sPWM9;	break;
				case 9:	pLCBPWM = &LCBTable.sPWM10;	break;
				case 10: pLCBPWM = &LCBTable.sPWM11;	break;
				case 11: pLCBPWM = &LCBTable.sPWM12;	break;			
				default:pLCBPWM = NULL;	break;
				}
				if ( NULL == pLCBPWM )	
				{	break;	}

				LEDTable = SliceParam.SliceLightTable.LEDChannel[j];
				if ( LEDTable.PowerValue >= 255 ) 
				{	pLCBPWM->sPower = 100; }
				else
				{
					if ( LEDTable.PowerValue > 100 ) { pLCBPWM->sPower = 100; }
					else if ( LEDTable.PowerValue < 0 ) { pLCBPWM->sPower = 0; }
					else
					{	pLCBPWM->sPower = (BYTE)(LEDTable.PowerValue);	}					
				}
				if ( LEDTable.OnOffState == FN_ENABLE )
				{	pLCBPWM->sIsON = true; }
				else
				{	pLCBPWM->sIsON = false; }
			}
			//unsigned int LEDTurnOnTimeus;//LED開啟時間-us		
			LCBTable.sLEDTableTotalTime=CameraDelayTime+CameraExpTime;//LED Table總時間 (相機延遲時間+相機曝光時間)
			LCBTable.sCCDDelayTime=CameraDelayTime;//燈亮至相機觸發的時間(us)	//DLP Type時，相機觸發訊號的延遲時間(us)		
			LCBTable.sDLPCCDExpTime=0;//DLP的相機曝光時間(us)

			//Table間距時間, 第一個Table要設0	
			LCBTable.sNextTableTime = 0;
			if ( 0 < TableID )
			{
				if ( LastTableTimeus < TablePeriodTimeus )
				{	LCBTable.sNextTableTime = TablePeriodTimeus-LastTableTimeus;	}

				const int NextMinTabeBetweenTimeus=MAX(MinTabeBetweenTimeus, LastTableBtTimeus);
				LCBTable.sNextTableTime = MAX(LCBTable.sNextTableTime, NextMinTabeBetweenTimeus);
			}			
			LCBTableList.push_back(LCBTable);
			LastTableTimeus = CameraExpTime;//LCBTable.sLEDTableTotalTime;
			LastTableBtTimeus = SliceParam.SliceNextGrabBtTimeus;
		}
		else if ( LIGHT_DLP == TableType )
		{	
			bool DLPAdded = false;
			CameraDelayTime = SliceParam.SliceCameraDelayTimeus;
			for ( j=0; j<DLP_CAST_COUNT; j++ )
			{
				if ( SliceParam.SliceLightTable.DLPCast[j].OnOffState == FN_DISABLE ) { continue; }

				TableID = (int)(LCBTableList.size());		
				DefaultTable(LCBTable);
				LCBTable.sTableID = TableID;		
				LCBTable.sTableType = TableType;

				DLPIndex = (int)(j);
				LCBTable.sDLPActiveChannel = DLPIndex+1;//DLP通道
				LCBTable.sDLPTrigOutNumber = SliceParam.SliceLightTable.DLPCast[DLPIndex].TriggerCount;	//DLP觸發數量
				LCBTable.sDLPPhasePatMode = SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode;//DLP相位樣板模式				
				LCBTable.sDLPPulseTime=GetDLPPulseWidthTimeus();//DLP Pulse Width的時間(us)
				LCBTable.sDLPCallbackTime=GetDLPCallbackTimeus();//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常

				LCBTable.sLEDTableTotalTime=CameraDelayTime+CameraExpTime;//LED Table總時間 (相機延遲時間+相機曝光時間)
				LCBTable.sCCDDelayTime=CameraDelayTime;//燈亮至相機觸發的時間(us)	//DLP Type時，相機觸發訊號的延遲時間(us)		
				LCBTable.sDLPCCDExpTime=CameraExpTime;		//DLP的相機曝光時間(us)
			
				LIGHT_3D_CAST_ID CastID = CLight3DCtrl::GetLight3DCastIDByDLPChannel(LCBTable.sDLPActiveChannel);
				LIGHT_3D_CLS_PTR CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
				if ( NULL==CastPtr || false==CastPtr->GetDLPReadySignalEnable() )
				{	LCBTable.sDLPReadySignalEnable = FN_DISABLE;	}
				else
				{	LCBTable.sDLPReadySignalEnable = FN_ENABLE;	}

				LCBTable.sNextTableTime = 0;//Table間距時間, 第一個Table要設0	
				if ( 0 < TableID )
				{
					if ( LastTableTimeus < TablePeriodTimeus )
					{	LCBTable.sNextTableTime = TablePeriodTimeus-LastTableTimeus;	}

					const int NextMinTabeBetweenTimeus=MAX(MinTabeBetweenTimeus, LastTableBtTimeus);
					LCBTable.sNextTableTime = MAX(LCBTable.sNextTableTime, NextMinTabeBetweenTimeus);
				}

				LCBTableList.push_back(LCBTable);
				LastTableTimeus = LCBTable.sLEDTableTotalTime;
				LastTableBtTimeus = SliceParam.SliceNextGrabBtTimeus;

				DLPAdded = true;
			}
			if ( false == DLPAdded )
			{
				this->m_ErrorString.Format(_T("Error, SliceParam Light Type is DLP, But no DLP Cast ID in Used"));
				return false;
			}
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ConvertSliceParamListToLCBTableList_DLPMultiple(const std::vector<TSliceParam> &SliceParamList, std::vector<TLCB_TRIG_TABLE> &LCBTableList)//將Slice Param轉成控制板的表格	
{
	CString str;
	size_t           i=0, j=0, k=0;
	int              TempI = 0;	
	TLEDTable        LEDTable;
	CAMERA_ID        CameraID = PRIMARY_CAMERA_ID;
	TSliceParam      SliceParam;	
	TLCB_LED_ITEM    LCBPWM, *pLCBPWM=NULL;		
	TLCB_TRIG_TABLE  LCBTable;
	const size_t     SliceParamCount = SliceParamList.size();	
	
	int TableID = 0;
	int TableType = 0;
	int DLPIndex = 0;
	int CameraDelayTime = 0;
	int CameraExpTime = 0;//(int)(SliceParam.SliceCameraExpTimeus);
	int       CameraFrameTimeus = 0;
	int       LastTableTimeus = 0;
	int       LastTableBtTimeus = 0;
	int       TablePeriodTimeus = 0;
	bool      FirstDLPTable = true;
	const bool bSepareDLP2ExpTable = AOIDataCollect.GetSeparateDLP2ExpTable();
	const int DLPCallbackTime = GetDLPCallbackTimeus();
	const int MinTabeBetweenTimeus = GetTableMinBetweenTimeus();

	bool      FixTableTime = false;//由於Coaxlink不支援動態的曝光周期, 因此找最大的曝光時間來調整-無效用
	int       MaxCameraExpTime = -1;
	if ( true == FixTableTime )
	{
		for ( i=0; i<SliceParamCount; i++ )
		{
			SliceParam = SliceParamList[i];
			switch ( SliceParam.SliceLightTable.LightType )
			{
			case LIGHT_LED:	TableType =  TABLE_TYPE_LED;	break;
			case LIGHT_DLP:	TableType =  TABLE_TYPE_DLP;	break;
			default:        TableType =  TABLE_TYPE_NONE;	break;
			}
			if ( TABLE_TYPE_NONE == TableType )
			{
				this->m_ErrorString.Format(_T("Error, Slice Param Ligth Type Exception [Index:%d, Value:%d]"), i+1, SliceParam.SliceLightTable.LightType);
				return false;
			}	
		#ifdef PHASE_CTRL_DISABLE
			if ( TABLE_TYPE_DLP == TableType )
			{	continue; }
		#endif//PHASE_CTRL_DISABLE
			CameraFrameTimeus = (int)(CameraCtrl.GetCameraMinPeriod(CameraID));//us		
			CameraExpTime = (int)(SliceParam.SliceCameraExpTimeus);
			MaxCameraExpTime = MAX(MaxCameraExpTime, CameraExpTime);
			MaxCameraExpTime = MAX(MaxCameraExpTime, CameraFrameTimeus);
		}
	}
	
	for ( i=0; i<SliceParamCount; i++ )
	{
		SliceParam = SliceParamList[i];
		switch ( SliceParam.SliceLightTable.LightType )
		{
		case LIGHT_LED:	TableType =  TABLE_TYPE_LED;	break;
		case LIGHT_DLP:	TableType =  TABLE_TYPE_DLP;	break;
		default:        TableType =  TABLE_TYPE_NONE;	break;
		}
		if ( TABLE_TYPE_NONE == TableType )
		{
			this->m_ErrorString.Format(_T("Error, Slice Param Ligth Type Exception [Index:%d, Value:%d]"), i+1, SliceParam.SliceLightTable.LightType);
			return false;
		}
	#ifdef PHASE_CTRL_DISABLE
		if ( TABLE_TYPE_DLP == TableType )
		{	continue; }
	#endif//PHASE_CTRL_DISABLE
		//if ( TABLE_TYPE_LED == TableType ) 
		//{	continue; }

		CameraID = SliceParam.SliceCameraID;		
		CameraFrameTimeus = (int)(CameraCtrl.GetCameraMinPeriod(CameraID));//us		
		CameraExpTime = (int)(SliceParam.SliceCameraExpTimeus);
		CameraDelayTime = SliceParam.SliceCameraDelayTimeus;
		TablePeriodTimeus = CameraFrameTimeus;
		if ( true == FixTableTime )
		{	TablePeriodTimeus = MaxCameraExpTime;	}
		if ( LIGHT_LED == TableType )
		{	
			TableID = (int)(LCBTableList.size());		
			DefaultTable(LCBTable);
			LCBTable.sTableID = TableID;		
			LCBTable.sTableType = TableType;
			LCBTable.sDLPTrigOutNumber = 0;	//DLP觸發數量
			LCBTable.sDLPActiveChannel = 0;	//DLP通道
			LCBTable.sDLPReadySignalEnable = FN_DISABLE;
		
			for ( j=0; j<LED_CHANNEL_COUNT; j++ )
			{			
				switch ( j )
				{
				case 0:	pLCBPWM = &LCBTable.sPWM1;	break;
				case 1:	pLCBPWM = &LCBTable.sPWM2;	break;
				case 2:	pLCBPWM = &LCBTable.sPWM3;	break;
				case 3:	pLCBPWM = &LCBTable.sPWM4;	break;
				case 4:	pLCBPWM = &LCBTable.sPWM5;	break;
				case 5:	pLCBPWM = &LCBTable.sPWM6;	break;
				case 6:	pLCBPWM = &LCBTable.sPWM7;	break;
				case 7:	pLCBPWM = &LCBTable.sPWM8;	break;			
				case 8:	pLCBPWM = &LCBTable.sPWM9;	break;
				case 9:	pLCBPWM = &LCBTable.sPWM10;	break;
				case 10: pLCBPWM = &LCBTable.sPWM11;	break;
				case 11: pLCBPWM = &LCBTable.sPWM12;	break;			
				default:pLCBPWM = NULL;	break;
				}
				if ( NULL == pLCBPWM )	
				{	break;	}

				LEDTable = SliceParam.SliceLightTable.LEDChannel[j];
				if ( LEDTable.PowerValue >= 255 ) 
				{	pLCBPWM->sPower = 100; }
				else
				{
					if ( LEDTable.PowerValue > 100 ) { pLCBPWM->sPower = 100; }
					else if ( LEDTable.PowerValue < 0 ) { pLCBPWM->sPower = 0; }
					else
					{	pLCBPWM->sPower = (BYTE)(LEDTable.PowerValue);	}					
				}
				if ( LEDTable.OnOffState == FN_ENABLE )
				{	pLCBPWM->sIsON = true; }
				else
				{	pLCBPWM->sIsON = false; }
			}
			//unsigned int LEDTurnOnTimeus;//LED開啟時間-us		
			LCBTable.sLEDTableTotalTime=CameraDelayTime+CameraExpTime;//LED Table總時間 (相機延遲時間+相機曝光時間)
			LCBTable.sCCDDelayTime=CameraDelayTime;//燈亮至相機觸發的時間(us)	//DLP Type時，相機觸發訊號的延遲時間(us)		
			LCBTable.sDLPCCDExpTime=0;//DLP的相機曝光時間(us)

			//Table間距時間, 第一個Table要設0	
			LCBTable.sNextTableTime = 0;
			if ( 0 < TableID )
			{	
				if ( LastTableTimeus < TablePeriodTimeus )
				{	LCBTable.sNextTableTime = TablePeriodTimeus-LastTableTimeus;	}

				const int NextMinTabeBetweenTimeus=MAX(MinTabeBetweenTimeus, LastTableBtTimeus);
				LCBTable.sNextTableTime = MAX(LCBTable.sNextTableTime, NextMinTabeBetweenTimeus);
			}			
			LCBTableList.push_back(LCBTable);
			LastTableTimeus = CameraExpTime;//LCBTable.sLEDTableTotalTime;
			LastTableBtTimeus = SliceParam.SliceNextGrabBtTimeus;
		}
		else if ( LIGHT_DLP == TableType )
		{	
			bool DLPAdded = false;
			bool TableSepared = false;//分開表格
			CameraDelayTime = SliceParam.SliceCameraDelayTimeus;
			if ( false == bSepareDLP2ExpTable )
			{	TableSepared = false;	}
			else
			{
				for ( j=0; j<DLP_CAST_COUNT; j++ )
				{
					DLPIndex = (int)(j);
					if ( SliceParam.SliceLightTable.DLPCast[j].OnOffState == FN_DISABLE ) { continue; }
					if ( DLP_PATTERN_SEQUENCE_4_4_M_2 != SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode && 
						 DLP_PATTERN_SEQUENCE_4_2_M_2 != SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode &&
						 DLP_PATTERN_SEQUENCE_4_4GC_M2 != SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode && 
						 DLP_PATTERN_SEQUENCE_4_5GC_M2 != SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode &&
						 DLP_PATTERN_SEQUENCE_4_6GC_M2 != SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode )
					{	continue; }
					TableSepared = true;
					break;
				}
			}
			if ( false == TableSepared )			
			{
				DLPAdded = false;
				for ( j=0; j<DLP_CAST_COUNT; j++ )
				{			
					DLPIndex = (int)(j);
					if ( SliceParam.SliceLightTable.DLPCast[j].OnOffState == FN_DISABLE ) { continue; }				
					for ( k=0; k<SliceParam.SliceLightTable.DLPCast[j].TriggerCount; k++ )
					{
						TableID = (int)(LCBTableList.size());		
						DefaultTable(LCBTable);
						LCBTable.sTableID = TableID;		
						LCBTable.sTableType = TableType;
					
						LCBTable.sDLPActiveChannel = DLPIndex+1;//DLP通道
						LCBTable.sDLPTrigOutNumber = 1;//DLP觸發數量--不同點
						LCBTable.sDLPPhasePatMode = SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode;//DLP相位樣板模式				
						LCBTable.sDLPPulseTime=GetDLPPulseWidthTimeus();//DLP Pulse Width的時間(us)
						LCBTable.sDLPCallbackTime=GetDLPCallbackTimeus();//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常

						LCBTable.sLEDTableTotalTime=CameraDelayTime+CameraExpTime;//LED Table總時間 (相機延遲時間+相機曝光時間)
						LCBTable.sCCDDelayTime=CameraDelayTime;//燈亮至相機觸發的時間(us)	//DLP Type時，相機觸發訊號的延遲時間(us)		
						LCBTable.sDLPCCDExpTime=CameraExpTime;	//DLP的相機曝光時間(us)
					
						LIGHT_3D_CAST_ID CastID = CLight3DCtrl::GetLight3DCastIDByDLPChannel(LCBTable.sDLPActiveChannel);
						LIGHT_3D_CLS_PTR CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
						if ( NULL==CastPtr || false==CastPtr->GetDLPReadySignalEnable() )
						{	LCBTable.sDLPReadySignalEnable = FN_DISABLE;	}
						else
						{	LCBTable.sDLPReadySignalEnable = FN_ENABLE;	}

						LCBTable.sNextTableTime = 0;//Table間距時間, 第一個Table要設0	
						if ( 0 < TableID )
						{	
							if ( LastTableTimeus < TablePeriodTimeus )
							{	LCBTable.sNextTableTime = TablePeriodTimeus-LastTableTimeus;	}

							const int NextMinTabeBetweenTimeus=MAX(MinTabeBetweenTimeus, LastTableBtTimeus);
							LCBTable.sNextTableTime = MAX(LCBTable.sNextTableTime, NextMinTabeBetweenTimeus);
						}
						LCBTableList.push_back(LCBTable);
						LastTableTimeus = LCBTable.sLEDTableTotalTime;
						LastTableBtTimeus = SliceParam.SliceNextGrabBtTimeus;
						DLPAdded = true;
					}
				}
				if ( false == DLPAdded )
				{
					this->m_ErrorString.Format(_T("Error, SliceParam Light Type is DLP, But no DLP Cast ID in Used"));
					return false;
				}
			}
			else
			{				
				int ExpCnt=0;
				int ExpMaxCnt=2;
				int PaddingTime_us=0;
				int CameraExpTime2 = 0;
				int ExpMaxTriggerCnt=0;
				LIGHT_3D_CAST_ID CastID;
				LIGHT_3D_CLS_PTR Light3DPtr=NULL;
				int LastTrigger[DLP_CAST_COUNT]={0};
				int NextTrigger[DLP_CAST_COUNT]={0};
				int DlpPeriodTime[DLP_CAST_COUNT]={0};
				double SecondExpRatio[DLP_CAST_COUNT]={1};				

				DLPAdded = false;				
				for ( j=0; j<DLP_CAST_COUNT; j++ )
				{	
					SecondExpRatio[j] = 1.0;
					DlpPeriodTime[j] = 4000;
					LastTrigger[j] = NextTrigger[j] = 0; 
					CastID = CLight3DCtrl::GetLight3DCastIDByIndex(j);
					Light3DPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
					if ( NULL != Light3DPtr)
					{	
						DlpPeriodTime[j] = Light3DPtr->GetDLPPeriodTime();
						SecondExpRatio[j] = Light3DPtr->GetSecondExpRatio();
					}
				}
				for ( ExpCnt=0; ExpCnt<ExpMaxCnt; ExpCnt++ )
				{						
					for ( j=0; j<DLP_CAST_COUNT; j++ )
					{			
						DLPIndex = (int)(j);
						if ( SliceParam.SliceLightTable.DLPCast[j].OnOffState == FN_DISABLE ) { continue; }						
						if ( 0 == ExpCnt )
						{	CameraExpTime2 = CameraExpTime; }
						else
						{	
							if ( CameraExpTime < DlpPeriodTime[j] )
							{	PaddingTime_us = 0; }
							else 
							{	PaddingTime_us = CameraExpTime-DlpPeriodTime[j]; }
							CameraExpTime2 = CameraExpTime*SecondExpRatio[j];	
							CameraExpTime2 = (DlpPeriodTime[j]*SecondExpRatio[j])+PaddingTime_us;	
							CameraExpTime2 = MAX(CameraExpTime, CameraExpTime2);
						}

						switch ( SliceParam.SliceLightTable.DLPCast[j].PhasePatMode )
						{
						case DLP_PATTERN_SEQUENCE_4_4GC_M2:	ExpMaxTriggerCnt = 8;  break;
						case DLP_PATTERN_SEQUENCE_4_5GC_M2:	ExpMaxTriggerCnt = 9;  break;
						case DLP_PATTERN_SEQUENCE_4_6GC_M2:	ExpMaxTriggerCnt =10;  break;
						case DLP_PATTERN_SEQUENCE_4_2_M_2:	ExpMaxTriggerCnt = 6;  break;						
						case DLP_PATTERN_SEQUENCE_4_4_M_2:	ExpMaxTriggerCnt = 8;  break;
						default:
							ExpMaxTriggerCnt = 0;
							break;
						}

						LastTrigger[j] = NextTrigger[j];
						NextTrigger[j] = MIN((ExpCnt+1)*ExpMaxTriggerCnt, SliceParam.SliceLightTable.DLPCast[j].TriggerCount);
						for ( k=LastTrigger[j]; k<NextTrigger[j]; k++ )
						{
							TableID = (int)(LCBTableList.size());		
							DefaultTable(LCBTable);
							LCBTable.sTableID = TableID;		
							LCBTable.sTableType = TableType;
					
							LCBTable.sDLPActiveChannel = DLPIndex+1;//DLP通道
							LCBTable.sDLPTrigOutNumber = 1;//DLP觸發數量--不同點
							LCBTable.sDLPPhasePatMode = SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode;//DLP相位樣板模式				
							LCBTable.sDLPPulseTime=GetDLPPulseWidthTimeus();//DLP Pulse Width的時間(us)
							LCBTable.sDLPCallbackTime=GetDLPCallbackTimeus();//(12)(bit15~0) (13)(bit31~16)	//等待DLP Callback的時間(us)，超時會出現異常

							LCBTable.sLEDTableTotalTime=CameraDelayTime+CameraExpTime2;//LED Table總時間 (相機延遲時間+相機曝光時間)
							LCBTable.sCCDDelayTime=CameraDelayTime;//燈亮至相機觸發的時間(us)	//DLP Type時，相機觸發訊號的延遲時間(us)		
							LCBTable.sDLPCCDExpTime=CameraExpTime2;	//DLP的相機曝光時間(us)

							LCBTable.sNextTableTime = 0;//Table間距時間, 第一個Table要設0	
							if ( 0 < TableID )
							{	
								if ( LastTableTimeus < TablePeriodTimeus )
								{	LCBTable.sNextTableTime = TablePeriodTimeus-LastTableTimeus;	}

								const int NextMinTabeBetweenTimeus=MAX(MinTabeBetweenTimeus, LastTableBtTimeus);
								LCBTable.sNextTableTime = MAX(LCBTable.sNextTableTime, NextMinTabeBetweenTimeus);
							}
							LCBTableList.push_back(LCBTable);
							LastTableTimeus = LCBTable.sLEDTableTotalTime;
							LastTableBtTimeus = SliceParam.SliceNextGrabBtTimeus;
							DLPAdded = true;
						}
					}					
				}	
				if ( false == DLPAdded )
				{
					this->m_ErrorString.Format(_T("Error, SliceParam Light Type is DLP, But no DLP Cast ID in Used"));
					return false;
				}
			}
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ExecConveyorMotorStop(LANE_ID LaneID)//執行軌道停止
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ExecConveyorMotorStop(LaneID) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_CONVEYOR_FUNC);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow)//執行軌道運轉	
{
#ifndef LIGHT_CTRL_DISABLE
	if ( CheckImp() == false ) { return false; }
	if ( _Imp->ExecConveyorMotorRunning(LaneID, On, bPositive, Slow) == false )
	{
		SetErrorString(_Imp->GetErrorString());
		SetLightCtrlBoardExceptionCode(AOI_EXCEPTION_LIGHT_CTRL_CONVEYOR_FUNC);
		return false;
	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//