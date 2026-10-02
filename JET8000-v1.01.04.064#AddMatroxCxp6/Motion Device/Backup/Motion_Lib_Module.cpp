// CMotion_Lib_Module.cpp: implementation of the CMotion_Lib_Module class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Motion_Lib_Module.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#if MOTION_DERIVE_MODE == MOTION_LIB_MODULE
#include "MotionModule\\Include\\MotionUnit.h"
//-------------------------------------------------------------------------------------//
CMotion_Lib_Module  Motion_Lib_Module;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CMotion_Lib_Module, CMotion_Basic)
//-------------------------------------------------------------------------------------//
CMotion_Lib_Module::CMotion_Lib_Module()
{
	PreInitMotion();
	SetInitialize(false);
	m_MotionStatus = _T("");
	m_ErrorString = _T("");
	m_IsJogMode = false;
	m_TriggerNTriggers = 0;
	SetIsSupportGantry(false);	
}
//-------------------------------------------------------------------------------------//
CMotion_Lib_Module::~CMotion_Lib_Module()
{	
	m_IsMotionRelease = true;
	m_MotionCallbackHWnd = NULL;
	if ( this->ReleaseMotion() == false )
	{	JetAPI::ShowMessageBox(this->m_ErrorString);	}

#ifndef MOTION_OBJ_DISABLE	
	//mouFreeDLL();	
#endif//MOTION_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::PreInitMotion()//預先初始化
{
	CMotion_Basic::m_MotionName = _T("Motion-Unit");
	m_LoadMotionDll = false;
	m_Axis[SWAT_X] = MOU_AXIS_X;
	m_Axis[SWAT_Y] = MOU_AXIS_Y;
	m_Axis[SWAT_Z] = MOU_AXIS_Z;
	m_Axis[SWAT_T] = MOU_AXIS_T;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::InitialMotion()//初始化	
{
bool IsOK = true;

#ifdef MOTION_OBJ_DISABLE
	IsOK = true;
	SetInitialize(true);
#else
	CMotion_Basic::SaveMotionProcess(-1, _T("InitialMotion"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::InitialMotion Start"));
	
	std::wstring strDllPath(L"MotionUnit.dll");
	std::wstring strIniPath(L"C:\\JETAOI3D\\MotionUnit.ini");

#ifdef _DEBUG
	strDllPath = L"MotionUnitD.dll";
#else
	strDllPath = L"MotionUnit.dll";
#endif//#_DEBUG
	//strDllPath = L"C:\\JETAOI3D\\Runtime_x64\\MotionUnitD.dll";	
	//::SetCurrentDirectory(_T("C:\\JETAOI3D\\Runtime_x64"));
	IsOK = InitMotionInstance(strDllPath.c_str(), strIniPath.c_str());	
	if ( IsOK == false ) 
	{	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::InitialMotion NG End"));	}
	else
	{	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::InitialMotion OK End"));	}
#endif//MOTION_OBJ_DISABLE	
	return IsOK;		
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::LoadMotionDll(const wchar_t *pDll)
{
#ifndef MOTION_OBJ_DISABLE
	FreeMotionDll();
	bool bval = mouLoadDLL(pDll);
	if ( false == bval ) { return false; }
	this->m_LoadMotionDll = true;
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::FreeMotionDll()
{
#ifndef MOTION_OBJ_DISABLE
	if ( false == m_LoadMotionDll ) { return true; }
	mouFreeDLL();
	this->m_LoadMotionDll = false;
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::InitMotionInstance(const wchar_t *pDll, const wchar_t *pDllIni)
{
#ifndef MOTION_OBJ_DISABLE
	//bool bval = mouLoadDLL(pDll);
	bool bval = LoadMotionDll(pDll);
	CMotion_Basic::m_MotionName = _T("Motion-Unit");
	if ( bval == false )
	{
		wchar_t strMsg[128];
		swprintf_s(strMsg, L"Cannot load %s", pDll);			
		MessageBoxW(NULL, strMsg,  L"Attention", MB_APPLMODAL | MB_ICONERROR| MB_OK);
		return (false);
	}	
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	if ( MOU_ERR_SUCCESS != (ErrCode = mouOpen(pDllIni, &m_hMotion) ) )
	{	return false; }

	if ( MOU_ERR_SUCCESS != (ErrCode = mouInitialize(m_hMotion) ))
	{	return  false;	}	
		
	char ModuleName[128]="";
	CString strModuleName;
	ErrCode = mouGetModuleName(m_hMotion, ModuleName);
	strModuleName = ModuleName;

	CMotion_Basic::m_MotionName.Format(_T("%s [%s]"), _T("Motion-Unit"), strModuleName);
	SetInitialize(true);
	
	return (true);	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::ReleaseMotion()//釋放資源
{
	return ReleaseMotionCard();
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::ReleaseMotionCard()
{
	if ( CheckInit() == false ) 
	{
		FreeMotionDll();
		return true; 
	}	

	this->DisableSoftwareLimit(AXIS_X);
	this->DisableSoftwareLimit(AXIS_Y);
	this->DisableSoftwareLimit(AXIS_Z);

#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::ReleaseMotionCard Start"));
	this->Disable(AXIS_X);
	this->Disable(AXIS_Y);
	this->Disable(AXIS_Z);
	this->Disable(AXIS_X_SLAVE);	
	if ( NULL != m_hMotion )
	{	
		mouUninitialize(m_hMotion);		
		m_hMotion = NULL;
	}		
	FreeMotionDll();
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::ReleaseMotionCard OK End"));
#endif//MOTION_OBJ_DISABLE

	SetInitialize(false);	
	SetIsSupportGantry(false);

	CMotion_Basic::SetIsHomed(AXIS_X, false);
	CMotion_Basic::SetIsHomed(AXIS_Y, false);
	CMotion_Basic::SetIsHomed(AXIS_Z, false);
	CMotion_Basic::SetIsEnabled(AXIS_X, false);
	CMotion_Basic::SetIsEnabled(AXIS_Y, false);
	CMotion_Basic::SetIsEnabled(AXIS_Z, false);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::CheckAxis(int axis, TCHAR *pAxisS)
{
	CString str;
	if ( axis < 0 ) 
	{	
		str = _T("Error, Axis Exception");
		str = LoadMultiLanguageString(str, str);
		this->m_ErrorString.Format(_T("%s (%d)"), str, axis);
		return false; 
	}
#ifdef STAND_ALONE_VERSION
	if ( axis!=AXIS_X && axis!=AXIS_Y && axis!=AXIS_Z  && axis!=AXIS_4  )
	{
		str = _T("Error, Axis Exception");
		str = LoadMultiLanguageString(str, str);
		this->m_ErrorString.Format(_T("%s (%d)"), str, axis);
		return false; 
	}
#else
	if ( axis!=AXIS_X && axis!=AXIS_Y && axis!=AXIS_Z )
	{
		if ( this->GetIsSupportGantry() == true )
		{
			if ( axis != AXIS_X_SLAVE )
			{
				str = _T("Error, Axis Exception");
				str = LoadMultiLanguageString(str, str);
				this->m_ErrorString.Format(_T("%s (%d)"), str, axis);
				return false; 
			}
		}
		else
		{
			str = _T("Error, Axis Exception");
			str = LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s (%d)"), str, axis);
			return false; 
		}
	}
#endif//STAND_ALONE_VERSION
	if ( pAxisS == NULL ) { return true; }
	
	switch ( axis )
	{
	case AXIS_X:	::_tcscpy(pAxisS, _T("X Axis"));	break; 
	case AXIS_Y:	::_tcscpy(pAxisS, _T("Y Axis"));	break; 
	case AXIS_Z:	::_tcscpy(pAxisS, _T("Z Axis"));	break; 
	case AXIS_X_SLAVE: ::_tcscpy(pAxisS, _T("X-Slave Axis"));	break; 
	default: ::_stprintf(pAxisS, _T("%d Axis"), axis);	break; 
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Lib_Module::SaveMotionParamInternal()//儲存運動內部參數
{
#ifndef MOTION_OBJ_DISABLE
	const int CmdLen = 128;	
	int ResLen = CmdLen;
	char Cmd[CmdLen]="";
	char Response[CmdLen]="";
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;

	strcpy(Cmd, "SaveINI");
	ErrCode = mouSendCommand(m_hMotion, Cmd, CmdLen, Response, &ResLen);
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Home NG-2 End"));
		return false;
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetMotionIsReady()//取得運動系統是否正常
{
	if ( CheckInit() == false ) { return false; }
	this->m_ErrorString = _T("");
	
	//先判斷是否Enable	
	int  AxisNo = AXIS_X;
	CString Text;
	if ( this->GetIsEnable(AXIS_X) == false )
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("X Axis didn't Enabled. (%s)"), Text);
		return false;
	}

	if ( this->GetIsEnable(AXIS_Y) == false )
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Y Axis didn't Enabled. (%s)"), Text);
		return false;
	}

	if ( this->GetIsEnable(AXIS_Z) == false )
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Z Axis didn't Enabled. (%s)"), Text);
		return false;
	}
	
	//確認是否為ALARM狀態	
	if ( this->GetIsAlarm(AXIS_X) == true ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("X Axis is Alarm. (%s)"), Text);
		return false;
	}
	if ( this->GetIsAlarm(AXIS_Y) == true ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Y Axis is Alarm. (%s)"), Text);
		return false;
	}
	if ( this->GetIsAlarm(AXIS_Z) == true ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Z Axis is Alarm. (%s)"), Text);
		return false;
	}

	//確認是否為Ready狀態	
	if ( this->GetIsReady(AXIS_X) == false ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("X Axis is not Ready. (%s)"), Text);
		return false;
	}
	if ( this->GetIsReady(AXIS_Y) == false ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Y Axis is not Ready. (%s)"), Text);
		return false;
	}
	if ( this->GetIsReady(AXIS_Z) == false ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Z Axis is not Ready. (%s)"), Text);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::Home(const int Axis)
{
#ifndef MOTION_OBJ_DISABLE		
	if ( this->CheckInit() == false ) { return false; }		
	if ( this->CheckAxis(Axis) == false ) 
	{	return false; }
	
	JOG_Data  JogData;
	HOME_Data HomeData;
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	MOU_AXIS_DIRECTION AxisDir=MOU_AXIS_FORWARD;	
	const TMotionParameter &MotionParam = GetMotionParameter();	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Home Start"));	

	::memset(&JogData, 0x00, sizeof(JogData));	
	::memset(&HomeData, 0x00, sizeof(HomeData));	
	const bool bConvert=GetIsConvertSignPositive();
	switch ( Axis )
	{
	case AXIS_X:
		JogData.MinVelocity = 10000;
		JogData.MaxVelocity = 500000;
		HomeData.Velocity = MotionParam.m_HomeVelocityX;
		HomeData.OrgOffset = MotionParam.m_HomeOrgOffsetX;		
		HomeData.PreMovingDist = MotionParam.m_HomePreMoveDisX;
		if ( MACHINE_MODEL_6500 == SystemParam.m_MachineModelType )
		{
			if ( FN_ENABLE == MotionParam.m_SignPositiveX )
			{	AxisDir=MOU_AXIS_FORWARD; }
			else 
			{	AxisDir=MOU_AXIS_BACKWARD; }		
		}
		else
		{
			if ( FN_ENABLE == MotionParam.m_SignPositiveX )
			{	AxisDir=MOU_AXIS_BACKWARD; }
			else 
			{	AxisDir=MOU_AXIS_FORWARD; }		
		}
		break;
	case AXIS_Y:
		JogData.MinVelocity = 10000;
		JogData.MaxVelocity = 500000;
		HomeData.Velocity = MotionParam.m_HomeVelocityY;
		HomeData.OrgOffset = MotionParam.m_HomeOrgOffsetY;		
		HomeData.PreMovingDist = MotionParam.m_HomePreMoveDisY;		
		if ( MACHINE_MODEL_6500 == SystemParam.m_MachineModelType )
		{
			if ( FN_ENABLE == MotionParam.m_SignPositiveY )
			{	AxisDir=MOU_AXIS_FORWARD; }
			else 
			{	AxisDir=MOU_AXIS_BACKWARD; }
		}
		else
		{
			if ( FN_ENABLE == MotionParam.m_SignPositiveY )
			{	AxisDir=MOU_AXIS_BACKWARD; }
			else 
			{	AxisDir=MOU_AXIS_FORWARD; }
		}
		break;
	case AXIS_Z:
		JogData.MinVelocity = 10000;
		JogData.MaxVelocity = 100000;
		HomeData.Velocity = MotionParam.m_HomeVelocityZ;
		HomeData.OrgOffset = MotionParam.m_HomeOrgOffsetZ;		
		HomeData.PreMovingDist = MotionParam.m_HomePreMoveDisZ;
		if ( MACHINE_MODEL_6500 == SystemParam.m_MachineModelType )
		{
			if ( FN_ENABLE == MotionParam.m_SignPositiveZ )
			{	AxisDir=MOU_AXIS_BACKWARD; }
			else 
			{	AxisDir=MOU_AXIS_FORWARD; }		
		}
		else
		{
			if ( FN_ENABLE == MotionParam.m_SignPositiveZ )
			{	AxisDir=MOU_AXIS_FORWARD; }
			else 
			{	AxisDir=MOU_AXIS_BACKWARD; }		
		}
		break;
	}
	const int  bSign=GetAxisSignPositive(Axis);
	if ( true==bConvert && FN_DISABLE==bSign )
	{	
		HomeData.OrgOffset = -HomeData.OrgOffset;	
		HomeData.PreMovingDist = -HomeData.PreMovingDist;
	}
	JogData.AccelerationTime = MotionParam.m_JogAccelerationTime;
	JogData.DecelerationTime = MotionParam.m_JogDecelerationTime;

	ErrCode = mouSetHomeData(m_hMotion, m_Axis[Axis], HomeData);
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Home NG-1 End"));
		return false;
	}
	
	ErrCode = mouSetJogData(m_hMotion, m_Axis[Axis], JogData);
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Home NG-2 End"));
		return false; 
	}
	
	//ErrCode = mouExecHome(m_hMotion, m_Axis[Axis] );
	ErrCode = mouExecHomeEx(m_hMotion, m_Axis[Axis], AxisDir);	
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Home NG-3 End"));
		return false;
	}
	
	if ( m_MotionParameter.m_UsingMotionSoftwareLimit == FN_ENABLE )
	{
		double Max = 0;
		double Min = 0;
		switch ( Axis )
		{
		case AXIS_X:
			Max = m_MotionParameter.m_LimitMaxX;
			Min = m_MotionParameter.m_LimitMinX;
			this->SetSoftwareLimit(Axis, Min, Max, false);
			break;
		case AXIS_Y:
			Max = m_MotionParameter.m_LimitMaxY;
			Min = m_MotionParameter.m_LimitMinY;
			this->SetSoftwareLimit(Axis, Min, Max, false);
			break;
		case AXIS_Z:
			Max = m_MotionParameter.m_LimitMaxZ;
			Min = m_MotionParameter.m_LimitMinZ;
			this->SetSoftwareLimit(Axis, Min, Max, false);
			break;
		}		
		this->EnableSoftwareLimit(Axis);
	}
	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Home OK End"));

	CMotion_Lib_Module::SaveMotionParamInternal();	
#endif//MOTION_OBJ_DISABLE	
	CMotion_Basic::SetIsHomed(Axis, true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::Enable(const int Axis)
{
#ifndef MOTION_OBJ_DISABLE			
	if ( this->CheckInit() == false ) { return false; }		
	if ( this->CheckAxis(Axis) == false ) 
	{	return false; }

	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Enable Start"));	
	ErrCode = mouEnable(m_hMotion, m_Axis[Axis] );
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Enable NG-1 End"));
		return false;
	}

	if ( GetIsSupportGantry() == true )                                             
	{
		if ( Axis == AXIS_X )
		{
			if ( this->Enable(AXIS_X_SLAVE) == false )
			{	return false;	}
		}
	}	

	int HomeAxis = Axis;
	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( Axis == AXIS_X_SLAVE )
		{	HomeAxis = AXIS_X;	}
	}
	const bool bHomed = CMotion_Basic::GetIsHomed(HomeAxis);
	if ( true == bHomed )
	{
		if ( ResetMotionCardCommandPos(Axis) == false )
		{	CMotion_Basic::SetIsHomed(HomeAxis, false);	}		 
	}
	CMotion_Basic::SetIsEnabled(Axis, true);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Enable OK End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::Disable(const int Axis)
{
#ifndef MOTION_OBJ_DISABLE		
	if ( this->CheckInit() == false ) { return false; }		
	if ( this->CheckAxis(Axis) == false ) 
	{	return false; }

	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Disable Start"));	
	ErrCode = mouDisable(m_hMotion, m_Axis[Axis] );
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Disable NG-1 End"));
		return false;
	}
	CMotion_Basic::SetIsEnabled(Axis, false);	
	if ( GetIsSupportGantry() == true )                                             
	{
		if ( Axis == AXIS_X )
		{
			if ( this->Disable(AXIS_X_SLAVE) == false )
			{	return false;	}
		}
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::Disable OK End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::SetIsWaitForInPosition(const int Axis, const bool Iswait)//設定是否等待定位停止
{	
#ifndef MOTION_OBJ_DISABLE		
	if ( this->CheckInit() == false ) { return false; }		
	if ( this->CheckAxis(Axis) == false ) 
	{	return false; }

	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::SetIsWaitForInPosition Start"));	
	if ( true == Iswait )
	{	ErrCode = mouEnableINP(m_hMotion, m_Axis[Axis] ); }
	else
	{	ErrCode = mouDisableINP(m_hMotion, m_Axis[Axis] ); }
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::SetIsWaitForInPosition NG-1 End"));
		return false;
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::SetIsWaitForInPosition OK End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::WaitForDone(const int Axis, const int MaxPreCounts)
{
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }		
	if ( this->CheckAxis(Axis) == false ) 
	{	return false; }

	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::WaitForDone Start"));	
	ErrCode = mouWaitForEndOfMove(m_hMotion, m_Axis[Axis] );
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::WaitForDone NG-1 End"));
		return false;
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::WaitForDone OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::OneAxisMoveTo(int AxisNo, double Dist, double StrVel, double MaxVel,double Tacc,double Tdec, MOVE_CURVE_MODE VelCurve, MOVE_COORDINATE_MODE CoordMode, double SVacc, double SVdec)
{
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) 
	{	return false; }

	CString str;	
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	int nSign = FN_ENABLE;
	double Acc = 3000000;//加速度mm/s/s
	double MinTimeAcc = 0.14;//最慢的時間為0.05 sec
	double MinTimeDec = 0.14;//最慢的時間為0.05 sec
	ACC_TIME_ADJUST_MODE AccTimeAdjustMode=ACC_TIME_ADJUST_OFF;
	const TMotionParameter &MotionParam=m_MotionParameter;

	if ( CMotion_Basic::CheckIsEnabled(AxisNo) == false )
	{	return false; }
	if ( CMotion_Basic::CheckIsHomed(AxisNo) == false )
	{	return false; }

	CMotion_Basic::SaveMotionProcess(AxisNo, _T("OneAxisMoveTo"), MSG_LEVEL_HIGH);

	if ( CMotion_Lib_Module::WaitForDone(AxisNo) == false )
	{	return false; }

	double Encode=0;
	double TargetPos = Dist;
	GetEncode(AxisNo, Encode);
	if ( MOVE_COORDINATE_INS == CoordMode ) 
	{	TargetPos = Encode+Dist;	}
	
	if ( AxisNo == AXIS_X ) 
	{ 		
		str.Format(_T("CMotion_Lib_Module::OneAxisMoveTo(X, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), TargetPos, MaxVel, Tacc, Tdec);
		CMotion_Basic::SaveMotionCurrentProcess(str);		
		if ( TargetPos>m_MotionParameter.m_LimitMaxX || TargetPos<m_MotionParameter.m_LimitMinX )
		{
			str = _T("Error, X Axis Out of Stage Limit");
			str = CMotion_Basic::LoadMultiLanguageString(str, str);
			m_ErrorString.Format(_T("%s (%.0f ~ %.0f, Pos=%.0f)"), str, m_MotionParameter.m_LimitMinX, m_MotionParameter.m_LimitMaxX, TargetPos);
			return false;
		}			
		nSign = MotionParam.m_SignPositiveX;
		Acc = MotionParam.m_AccelerationValueX;
		MinTimeAcc = MotionParam.m_AccelerationTimeX;	
		MinTimeDec = MotionParam.m_DecelerationTimeX;
		AccTimeAdjustMode = MotionParam.m_AccTimeAdjustModeX;

		SetCommandMaxVelocity_X(0);
		SetCommandAccelerationTim_X(0);		
	}
	if ( AxisNo == AXIS_Y ) 
	{ 
		str.Format(_T("CMotion_Lib_Module::OneAxisMoveTo(Y, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), TargetPos, MaxVel, Tacc, Tdec);
		CMotion_Basic::SaveMotionCurrentProcess(str);		
		if ( TargetPos>m_MotionParameter.m_LimitMaxY || TargetPos<m_MotionParameter.m_LimitMinY )
		{
			str = _T("Error, Y Axis Out of Stage Limit");
			str = CMotion_Basic::LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s (%.0f ~ %.0f, Pos=%.0f)"), str, m_MotionParameter.m_LimitMinY, m_MotionParameter.m_LimitMaxY, TargetPos);
			return false;
		}				
		nSign = MotionParam.m_SignPositiveY;
		Acc = MotionParam.m_AccelerationValueY;	
		MinTimeAcc = MotionParam.m_AccelerationTimeY;	
		MinTimeDec = MotionParam.m_DecelerationTimeY;
		AccTimeAdjustMode = MotionParam.m_AccTimeAdjustModeY;

		SetCommandMaxVelocity_Y(0);
		SetCommandAccelerationTim_Y(0);			
	}

	if ( AxisNo == AXIS_Z ) 
	{ 
		str.Format(_T("CMotion_Lib_Module::OneAxisMoveTo(Z, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), TargetPos, MaxVel, Tacc, Tdec);
		CMotion_Basic::SaveMotionCurrentProcess(str);		
		if ( TargetPos>m_MotionParameter.m_LimitMaxZ || TargetPos<m_MotionParameter.m_LimitMinZ )
		{
			str = _T("Error, Z Axis Out of Stage Limit");
			str = CMotion_Basic::LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s (%.0f ~ %.0f, Pos=%.0f)"), str, m_MotionParameter.m_LimitMinZ, m_MotionParameter.m_LimitMaxZ, TargetPos);
			return false;
		}
		nSign = MotionParam.m_SignPositiveZ;
		Acc = MotionParam.m_AccelerationValueZ;	
		MinTimeAcc = MotionParam.m_AccelerationTimeZ;	
		MinTimeDec = MotionParam.m_DecelerationTimeZ;
		AccTimeAdjustMode = MotionParam.m_AccTimeAdjustModeZ;

		SetCommandMaxVelocity_Z(0);
		SetCommandAccelerationTim_Z(0);	
	}	

	double Dis = Dist;
	double MaxVelocity = MaxVel;	
	if ( CoordMode == MOVE_COORDINATE_ABS)
	{	Dis = Dist - Encode;	}
	
	const bool bConvert=GetIsConvertSignPositive();//注意順序
	if ( true == bConvert )
	{
		if ( FN_DISABLE == nSign )
		{	Dist = -Dist;	}
	}

	if ( Dis < 0 ) { Dis = -Dis; }
	//MaxVelocity = sqrt(Acc*Dis/2.0);//平行四邊形
	MaxVelocity = sqrt(Acc*Dis);//三角形
	//MaxVelocity = sqrt(2.0*Acc*Dis/3.0);//三角形
	//MaxVelocity = sqrt(2.0*Acc*Dis/3.0);//三角形
	if ( MaxVelocity > MaxVel ) { MaxVelocity = MaxVel; }
	Tacc = ::fabs(MaxVelocity/Acc);
	Tdec = ::fabs(MaxVelocity/Acc);
	Tacc = AdjustAccTime(Tacc, MinTimeAcc, AccTimeAdjustMode);
	Tdec = AdjustAccTime(Tdec, MinTimeDec, AccTimeAdjustMode);

	double NewAcc = Dis/(Tacc*Tacc);
	if ( NewAcc > Acc ) { NewAcc = Acc; }
	MaxVelocity = Tacc*NewAcc;
	if ( MaxVelocity > MaxVel ) 
	{
		MaxVelocity = MaxVel;
		Acc = MaxVelocity/Tacc;
	}
	MaxVel = MaxVelocity;
	if ( FN_DISABLE == m_MotionParameter.m_AccelerationAdjust )
	{
		MaxVel = MaxVelocity;
		Tacc = MinTimeAcc;
		Tdec = MinTimeDec;		
	}
#ifndef MOTION_OBJ_DISABLE
	if ( Dis < 5 ) { return true; }

	switch ( AxisNo ) 
	{
	case AXIS_X:		
		SetCommandMaxVelocity_X(MaxVel);
		SetCommandAccelerationTim_X(Tacc);
		break;
	case AXIS_Y:
		SetCommandMaxVelocity_Y(MaxVel);
		SetCommandAccelerationTim_Y(Tacc);
		break;
	case AXIS_Z:
		SetCommandMaxVelocity_Z(MaxVel);
		SetCommandAccelerationTim_Z(Tacc);
		break;
	}
	
	if ( CheckEnableAxis(AxisNo) == false )
	{	return true; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo Start"));
	ErrCode = mouSetAccDecTime(m_hMotion, m_Axis[AxisNo], Tacc, Tdec);
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-0 (mouSetAccDecTime) End"));
		return false;
	}
	if ( VelCurve ==  MOVE_CURVE_T )
	{	
		ErrCode = mouSetCurveMode(m_hMotion, m_Axis[AxisNo], MOU_T_CURVE);		
		if ( MOU_ERR_SUCCESS != ErrCode )
		{
			GetErrorCodeText(ErrCode, m_ErrorString);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-1 (mouSetCurveMode) End"));
			return false; 
		}
		if ( CoordMode == MOVE_COORDINATE_ABS )
		{				
			ErrCode = mouMoveAbsolute(m_hMotion, m_Axis[AxisNo], MaxVel, Dist);
			if ( MOU_ERR_SUCCESS != ErrCode )
			{	
				GetErrorCodeText(ErrCode, str);
				m_ErrorString.Format(_T("Error, mouMoveAbsolute Fault.(%s)"), str);

				int axis = AxisNo;
				double dist = Dist;
				double startvel = StrVel;
				double maxvel = MaxVel;
				double acc = Tacc;
				double dec = Tdec;
				str.Format(_T("Axis: %d, Dist: %.2f,  StartVel: %.2f, MaxVel: %.2f, TAcc: %.2f, TDec: %.2f"), axis, dist, startvel, maxvel, acc, dec);

				CMotion_Basic::SaveMotionCurrentProcess(m_ErrorString);
				CMotion_Basic::SaveMotionCurrentProcess(str);
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-2 (mouMoveAbsolute) End"));
				return false; 
			}
		}
		else if ( CoordMode == MOVE_COORDINATE_INS )
		{			
			ErrCode = mouMoveRelative(m_hMotion, m_Axis[AxisNo], MaxVel, Dist);
			if ( MOU_ERR_SUCCESS != ErrCode )
			{
				GetErrorCodeText(ErrCode, m_ErrorString);
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-3 (mouMoveRelative) End"));
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Coordinate Mode Exception (%d)"), CoordMode);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-4 End"));
			return false; 
		}		
	}
	else if ( VelCurve == MOVE_CURVE_S )
	{
		SVacc = ::fabs((MaxVel-StrVel)/3);
		SVdec = ::fabs((MaxVel-StrVel)/3);
		ErrCode = mouSetCurveMode(m_hMotion, m_Axis[AxisNo], MOU_S_CURVE);		
		if ( MOU_ERR_SUCCESS != ErrCode )
		{
			GetErrorCodeText(ErrCode, m_ErrorString);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-5 (mouSetCurveMode) End"));
			return false; 
		}
		ErrCode = mouSetSVaccVdec(m_hMotion, m_Axis[AxisNo],SVacc, SVdec);
		if ( MOU_ERR_SUCCESS != ErrCode )
		{
			GetErrorCodeText(ErrCode, m_ErrorString);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-6 (mouSetSVaccVdec) End"));
			return false; 
		}
		if ( CoordMode == MOVE_COORDINATE_ABS )
		{
			ErrCode = mouMoveAbsolute(m_hMotion, m_Axis[AxisNo], MaxVel, Dist);
			if ( MOU_ERR_SUCCESS != ErrCode )
			{
				GetErrorCodeText(ErrCode, m_ErrorString);
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-7 (mouMoveAbsolute) End"));
				return false; 
			}
		}
		else if ( CoordMode == MOVE_COORDINATE_INS )
		{
			ErrCode = mouMoveRelative(m_hMotion, m_Axis[AxisNo], MaxVel, Dist);
			if ( MOU_ERR_SUCCESS != ErrCode )
			{
				GetErrorCodeText(ErrCode, m_ErrorString);
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-8 (mouMoveRelative) End"));
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Coordinate Mode Exception (%d)"), CoordMode);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-9 End"));
			return false; 
		}		
	}
	else
	{
		this->m_ErrorString.Format(_T("Error, Velocity Curve Exception (%d)"), CoordMode);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo NG-10 End"));
		return false; 
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::OneAxisMoveTo OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::MoveTo(const int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)
{
#ifndef MOTION_OBJ_DISABLE		
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(Axis) == false ) 
	{	return false; }	

	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	MOVE_CURVE_MODE MoveCurveMode=MOVE_CURVE_T;
	double StrVel=0, MaxVel=300000, Tacc=0.1, Tdec=0.1;
	if ( Axis == AXIS_X ) 
	{
		switch ( MovingMode )
		{
		case MOTION_MOVING_HOME:	MaxVel = m_MotionParameter.m_HomeVelocityX;	break;
		case MOTION_MOVING_SCAN:	MaxVel = m_MotionParameter.m_TriggerMaxVelocity;	break;
		case MOTION_MOVING_GO_STOP:	MaxVel = m_MotionParameter.m_GrabbingVelocityX;	break;
		case MOTION_MOVING_PCB_IN:	MaxVel = m_MotionParameter.m_BoardInVelocityX;		break;
		case MOTION_MOVING_PCB_OUT:	MaxVel = m_MotionParameter.m_BoardOutVelocityX;		break;
		default:					MaxVel = m_MotionParameter.m_MaxVelocityX;	break;
		}
		SetCommandPosX(TargetPos);		
		Tacc = m_MotionParameter.m_AccelerationTimeX;
		Tdec = m_MotionParameter.m_DecelerationTimeX;
		MoveCurveMode = m_MotionParameter.m_MovingCurveModeX;
	}
	if ( Axis == AXIS_Y ) 
	{ 
		StrVel = 0;
		switch ( MovingMode )
		{
		case MOTION_MOVING_HOME:	MaxVel = m_MotionParameter.m_HomeVelocityY;	break;
		case MOTION_MOVING_SCAN:	MaxVel = m_MotionParameter.m_TriggerMaxVelocity;	break;
		case MOTION_MOVING_GO_STOP:	MaxVel = m_MotionParameter.m_GrabbingVelocityY;	break;
		case MOTION_MOVING_PCB_IN:	MaxVel = m_MotionParameter.m_BoardInVelocityY;		break;
		case MOTION_MOVING_PCB_OUT:	MaxVel = m_MotionParameter.m_BoardOutVelocityY;		break;
		default:					MaxVel = m_MotionParameter.m_MaxVelocityY;	break;
		}
		SetCommandPosY(TargetPos);
		Tacc = m_MotionParameter.m_AccelerationTimeY;
		Tdec = m_MotionParameter.m_DecelerationTimeY;
		MoveCurveMode = m_MotionParameter.m_MovingCurveModeY;
	}
	if ( Axis == AXIS_Z ) 
	{ 
		StrVel = 0;
		switch ( MovingMode )
		{
		case MOTION_MOVING_HOME:	MaxVel = m_MotionParameter.m_HomeVelocityZ;	break;
		case MOTION_MOVING_SCAN:	MaxVel = m_MotionParameter.m_TriggerMaxVelocity;	break;
		case MOTION_MOVING_GO_STOP:	MaxVel = m_MotionParameter.m_GrabbingVelocityZ;	break;
		case MOTION_MOVING_PCB_IN:	MaxVel = m_MotionParameter.m_BoardInVelocityZ;		break;
		case MOTION_MOVING_PCB_OUT:	MaxVel = m_MotionParameter.m_BoardOutVelocityZ;		break;
		default:					MaxVel = m_MotionParameter.m_MaxVelocityZ;	break;
		}
		SetCommandPosZ(TargetPos);
		Tacc = m_MotionParameter.m_AccelerationTimeZ;
		Tdec = m_MotionParameter.m_DecelerationTimeZ;
		MoveCurveMode = m_MotionParameter.m_MovingCurveModeZ;
	}
	//MoveCurveMode=MOVE_CURVE_T;
//	return this->OneAxisMoveTo(Axis, Dist, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_S, MOVE_COORDINATE_ABS);
	return this->OneAxisMoveTo(Axis, TargetPos, StrVel, MaxVel, Tacc, Tdec, MoveCurveMode, MOVE_COORDINATE_ABS);
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::TriggerMoveTo(const int AxisNo, double TargetPos, bool IsModifyVelocity)//觸發移動至哪裡
{
#ifndef MOTION_OBJ_DISABLE			
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) 
	{	return false; }	

	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	double StrVel=0, MaxVel=300000, Tacc=0.1, Tdec=0.1;	
	if ( AxisNo == AXIS_X ) 
	{ 
		StrVel = 0;
		MaxVel = m_MotionParameter.m_TriggerMaxVelocity;		
		Tacc = m_MotionParameter.m_AccelerationTimeX;
		Tdec = m_MotionParameter.m_DecelerationTimeX;
	}
	if ( AxisNo == AXIS_Y ) 
	{ 
		StrVel = 0;
		MaxVel = m_MotionParameter.m_TriggerMaxVelocity;	
		Tacc = m_MotionParameter.m_AccelerationTimeY;
		Tdec = m_MotionParameter.m_DecelerationTimeY;
	}
	if ( AxisNo == AXIS_Z ) 
	{ 
		StrVel = 0;
		MaxVel = m_MotionParameter.m_TriggerMaxVelocity;	
		Tacc = m_MotionParameter.m_AccelerationTimeZ;
		Tdec = m_MotionParameter.m_DecelerationTimeZ;
	}
//	return this->OneAxisMoveTo(AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_S, MOVE_COORDINATE_ABS);
	return this->OneAxisMoveTo(AxisNo, TargetPos, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_T, MOVE_COORDINATE_ABS);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::XYMoveTo(double PosX, double PosY, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//2軸移動，但非同動唷
{
	if ( CheckNeedMoveXY(PosX, PosY, Offline) == false )//與之前的移動位置相同
	{	return true; }

	SetCommandPosX(PosX);
	SetCommandPosY(PosY);	
	SetCommandOffline(Offline);	
	if ( true == Offline )
	{	return true; }
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	const double ToleranceX=3;//誤差值
	const double ToleranceY=3;//誤差值
	const double ToleranceZ=3;//誤差值
	double EncodeX=0;
	double EncodeY=0;
	double EncodeZ=0;
	//GetCurrentPos(EncodeX, EncodeY, EncodeZ);//已校正的座標
	GetEncode(AXIS_X, EncodeX);//要取未校正的座標
	GetEncode(AXIS_Y, EncodeY);//要取未校正的座標
	GetEncode(AXIS_Z, EncodeZ);//要取未校正的座標

	double CaliX=0, CaliY=0;
	StageToCali(PosX, PosY, CaliX, CaliY);
	//XY座標會因為另一軌位置不同而變更, 以最後軸座標來比較
	if ( fabs(CaliX-EncodeX) > ToleranceX ) 
	{	
		if ( MoveTo(AXIS_X, CaliX, MovingMode, IsModifyVelocity) == false )
		{	return false; }		
	}
	if ( fabs(CaliY-EncodeY) > ToleranceY ) 
	{
		if ( MoveTo(AXIS_Y, CaliY, MovingMode, IsModifyVelocity) == false ) 
		{	return false; }
	}	
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::XYZMoveTo(double PosX, double PosY, double PosZ, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//3軸移動，但非同動唷
{
	if ( CheckNeedMoveXYZ(PosX, PosY, PosZ, Offline) == false )//與之前的移動位置相同
	{	return true; }

	SetCommandPosX(PosX);
	SetCommandPosY(PosY);	
	SetCommandPosZ(PosZ);	
	SetCommandOffline(Offline);
	if ( true == Offline )
	{	return true; }

#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	const double ToleranceX=3;//誤差值
	const double ToleranceY=3;//誤差值
	const double ToleranceZ=3;//誤差值
	double EncodeX=0;
	double EncodeY=0;
	double EncodeZ=0;
	//GetCurrentPos(EncodeX, EncodeY, EncodeZ);//已校正的座標
	GetEncode(AXIS_X, EncodeX);//要取未校正的座標
	GetEncode(AXIS_Y, EncodeY);//要取未校正的座標
	GetEncode(AXIS_Z, EncodeZ);//要取未校正的座標

	double CaliX=0, CaliY=0;
	StageToCali(PosX, PosY, CaliX, CaliY);
	//XY座標會因為另一軌位置不同而變更, 以最後軸座標來比較
	if ( fabs(CaliX-EncodeX) > ToleranceX ) 
	{	
		if ( MoveTo(AXIS_X, CaliX, MovingMode, IsModifyVelocity) == false )
		{	return false; }		
	}

	if ( fabs(CaliY-EncodeY) > ToleranceY ) 
	{
		if ( MoveTo(AXIS_Y, CaliY, MovingMode, IsModifyVelocity) == false )
		{	return false; }
	}
	
	if ( fabs(PosZ-EncodeZ) > ToleranceZ )
	{	
		if ( MoveTo(AXIS_Z, PosZ, MovingMode, IsModifyVelocity) == false ) 
		{	return false; }
	}
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetEncode(const int AxisNo, double &Encode, bool offline)//取得該軸的光學尺座標
{
	Encode = 0;	
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) 
	{	return false; }

	if ( true == offline )
	{
		Encode = GetCommandPos(AxisNo);		
		return true;
	}
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetEncode Start"));
	ErrCode = mouGetPosition(m_hMotion, m_Axis[AxisNo], Encode);
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(AxisNo);		
		if ( FN_DISABLE == nSign )
		{	Encode = -Encode;	}
	}
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetEncode NG End"));
		return false; 
	}
//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetEncode OK End"));
#else
	Encode = GetCommandPos(AxisNo);	
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetCurrentPos(double &X, double &Y, bool offline)//取得目前機台位置
{
	double PosZ=0;
	return GetCurrentPos(X, Y, PosZ, offline);
}
//----------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetCurrentPos(double &X, double &Y, double &Z, bool offline)//取得目前機台位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }	
	if ( true == offline )
	{
		GetCommandPos(X, Y, Z);
		return true;
	}
	double EncodeX=0;
	double EncodeY=0;
	double EncodeZ=0;
	if ( GetEncode(AXIS_X, EncodeX, offline) == false ) { return false; }
	if ( GetEncode(AXIS_Y, EncodeY, offline) == false ) { return false; }
	if ( GetEncode(AXIS_Z, EncodeZ, offline) == false ) { return false; }
	X = EncodeX;
	Y = EncodeY;
	Z = EncodeZ;	

	double PosX=0, PosY=0;
	CaliToStage(EncodeX, EncodeY, PosX, PosY);
	X = PosX;
	Y = PosY;	
#else
	GetCommandPos(X, Y, Z);	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::StartFreeRun(const int AxisNo, bool Dir)//Dir +為正方向, -為負方向
{	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("StartFreeRun"), MSG_LEVEL_HIGH);

	if ( CMotion_Basic::CheckIsEnabled(AxisNo) == false )
	{	return false; }	
#ifndef MOTION_OBJ_DISABLE
	MOU_AXIS_DIRECTION JogDir;
	const bool BotMode = GetViewBotMode();
	double MaxV = (int)(GetAxisFreeRunVelocity(AxisNo));
	if ( true == Dir )
	{	JogDir = MOU_AXIS_FORWARD;	}
	else
	{	JogDir = MOU_AXIS_BACKWARD; }
	switch ( AxisNo )
	{
	case AXIS_X:		
		m_FreeRunVel_X  = MaxV;		
		if ( m_JogDirection_X < 0 ) 
		{
			if ( MOU_AXIS_FORWARD == JogDir ) { JogDir = MOU_AXIS_BACKWARD; }
			else { JogDir = MOU_AXIS_FORWARD; }
		}
		if ( true == BotMode )
		{	
			if ( MOU_AXIS_FORWARD == JogDir ) { JogDir = MOU_AXIS_BACKWARD; }
			else { JogDir = MOU_AXIS_FORWARD; }
		}
		break;
	case AXIS_Y:		
		m_FreeRunVel_Y  = MaxV;
		if ( m_JogDirection_Y < 0 ) 
		{
			if ( MOU_AXIS_FORWARD == JogDir ) { JogDir = MOU_AXIS_BACKWARD; }
			else { JogDir = MOU_AXIS_FORWARD; }
		}
		break;
	case AXIS_Z:		
		m_FreeRunVel_Z  = MaxV;
		if ( m_JogDirection_Z < 0 ) 
		{
			if ( MOU_AXIS_FORWARD == JogDir ) { JogDir = MOU_AXIS_BACKWARD; }
			else { JogDir = MOU_AXIS_FORWARD; }
		}
		break;
	default:
		this->m_ErrorString.Format(_T("Error, Axis Exception"));
		return false;
	}

	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::StartFreeRun Start"));
	ErrCode = mouStartJog(m_hMotion, m_Axis[AxisNo], JogDir, MaxV);
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::StartFreeRun NG-2 End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::StartFreeRun OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::StopFreeRun(const int AxisNo)//停止FreeRun
{
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("StopFreeRun"), MSG_LEVEL_HIGH);
#ifndef MOTION_OBJ_DISABLE
	switch ( AxisNo )
	{
	case AXIS_X:	m_FreeRunVel_X = 0;	break;
	case AXIS_Y:	m_FreeRunVel_Y = 0;	break;
	case AXIS_Z:	m_FreeRunVel_Z = 0;	break;
	}

	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::StopFreeRun Start"));		
	switch ( AxisNo )
	{
	case AXIS_Z:		
		ErrCode = mouStopJog(m_hMotion, m_Axis[AxisNo]);
		break;
	default:
		ErrCode = mouStopJog(m_hMotion, m_Axis[AxisNo]);
		break;
	}	
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::StopFreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::StopFreeRun OK End"));	
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::ForwardTriggerProcess()//正向移動
{
#ifndef MOTION_OBJ_DISABLE	
	m_ErrorString = _T("Error, Not Support ForwardTriggerProcess Fn");
	JetAPI::ShowMessageBox(m_ErrorString);
	return false;
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::BackwardTriggerProcess()//逆向移動
{
#ifndef MOTION_OBJ_DISABLE	
	m_ErrorString = _T("Error, Not Support BackwardTriggerProcess Fn");
	JetAPI::ShowMessageBox(m_ErrorString);
	return false;
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::EnableCompareTrigger(const bool IsEnable)//是否啟動同步比較送外部觸發訊號
{
#ifndef MOTION_OBJ_DISABLE
	m_ErrorString = _T("Error, Not Support EnableCompareTrigger Fn");
	JetAPI::ShowMessageBox(m_ErrorString);
	return false;
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::StopCompareTrigger(bool IsStop)//是否停止同步比較送外部觸發訊號
{
#ifndef MOTION_OBJ_DISABLE	
	m_ErrorString = _T("Error, Not Support StopCompareTrigger Fn");
	JetAPI::ShowMessageBox(m_ErrorString);
	return false;
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotion_Lib_Module::SetTriggerParameter(double SP, double EP, double Start, double End, double Interval, int RepeatCounts, int YMaxCounts, double YOffset, int ORGX, int ORGY, int ORGZ)
{
#ifndef MOTION_OBJ_DISABLE	
	m_ErrorString = _T("Error, Not Support SetTriggerParameter Fn");
	JetAPI::ShowMessageBox(m_ErrorString);	
#endif//MOTION_OBJ_DISABLE	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::ConfigTriggerTable(const bool IsReBuild)
{
#ifndef MOTION_OBJ_DISABLE	
	m_ErrorString = _T("Error, Not Support ConfigTriggerTable Fn");
	JetAPI::ShowMessageBox(m_ErrorString);	
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CMotion_Lib_Module::GetIsEnable(const int AxisNo)//該軸是否為Serve ON, 也就是有送電來積磁
{
	bool bResult = true;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsEnable Start"));
	
	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsEnable NG End"));
		return false; 
	}	
	bResult = CheckBitMask(AxisIO, MOU_AXIS_SERVO_ON);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsEnable OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetIsReady(const int AxisNo)//Driver傳回該軸是否為RDY狀態
{
	bool bResult = true;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsReady Start"));

	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsReady NG End"));
		return false; 
	}	
	bResult = CheckBitMask(AxisIO, MOU_AXIS_READY);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsReady OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetIsAlarm(const int AxisNo)//Driver傳回該軸是否為Alarm狀態
{
	bool bResult = false;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsAlarm Start"));

	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsAlarm NG End"));
		return false; 
	}	
	bResult = CheckBitMask(AxisIO, MOU_AXIS_ALARM);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsAlarm OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetIsERCActive(const int AxisNo)//ERC Active
{
	bool bResult = false;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsERCActive Start"));

	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsERCActive NG End"));
		return false; 
	}	
	bResult = CheckBitMask(AxisIO, MOU_AXIS_ERC);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsERCActive OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetIsInPosition(const int AxisNo)//Driver傳回該軸是否為In Position
{
	bool bResult = true;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsInPosition Start"));

	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsInPosition NG End"));
		return false; 
	}	
	bResult = CheckBitMask(AxisIO, MOU_AXIS_IN_POSITION);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsInPosition OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetIsNLimit(const int AxisNo)//該軸的是否碰觸到副極限
{
	bool bResult = false;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsNLimit Start"));

	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsNLimit NG End"));
		return false; 
	}	
	int Filter= MOU_AXIS_BACKWARD_LIMITSWITCH;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(AxisNo);	
		if ( FN_DISABLE == nSign )
		{	Filter = MOU_AXIS_FORWARD_LIMITSWITCH;	}
	}
	bResult = CheckBitMask(AxisIO, Filter);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsNLimit OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetIsPLimit(const int AxisNo)//該軸的是否碰觸到正極限
{
	bool bResult = false;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsPLimit Start"));

	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsPLimit NG End"));
		return false; 
	}	
	int Filter= MOU_AXIS_FORWARD_LIMITSWITCH;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(AxisNo);	
		if ( FN_DISABLE == nSign )
		{	Filter = MOU_AXIS_BACKWARD_LIMITSWITCH;	}
	}
	bResult = CheckBitMask(AxisIO, Filter);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsPLimit OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetIsORG(const int AxisNo)//該軸是否在原點位置
{
	bool bResult = false;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsORG Start"));

	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsORG NG End"));
		return false; 
	}	
	bResult = CheckBitMask(AxisIO, MOU_AXIS_IN_ORG);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsORG OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::GetIsEmergencyOn(const int AxisNo)//該軸是否收到急停訊號
{
	bool bResult = false;
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsEmergencyOn Start"));

	int AxisIO = GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsEmergencyOn NG End"));
		return false; 
	}	
	bResult = CheckBitMask(AxisIO, MOU_AXIS_EMERGENCY_STOP);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIsEmergencyOn OK-1 End"));
#endif//MOTION_OBJ_DISABLE
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::ResetMotionCardCommandPos(int Axis)//重設軸控卡的命令位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(Axis) == false ) { return false; }
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;	
	ErrCode = mouResetAlarm(m_hMotion, m_Axis[Axis]);	
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);		
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::ResetMotionCardCommandPos NG End"));
		return false;
	}
	
	double encode=0.0;
	ErrCode = mouGetPosition(m_hMotion, m_Axis[Axis], encode);
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);		
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::ResetMotionCardCommandPos NG End"));
		return false;
	}
	CMotion_Basic::SetCommandPos(Axis, encode);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::DoFaultAck(const int AxisNo)
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	if ( CheckAxis(AxisNo, NULL) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("DoFaultAck"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::DoFaultAck Start"));	
	
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;	
	ErrCode = mouResetAlarm(m_hMotion, m_Axis[AxisNo]);	
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);		
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::DoFaultAck NG End"));
		return false;
	}	
	
	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( AxisNo == AXIS_X )
		{
			if ( DoFaultAck(AXIS_X_SLAVE) == false )
			{	return false;	}
		}
	}

	int HomeAxis = AxisNo;
	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( AxisNo == AXIS_X_SLAVE )
		{	HomeAxis = AXIS_X;	}
	}
	const bool bHomed = CMotion_Basic::GetIsHomed(HomeAxis);
	if ( true == bHomed )
	{
		if ( ResetMotionCardCommandPos(AxisNo) == false )
		{	CMotion_Basic::SetIsHomed(HomeAxis, false);	}		 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::DoFaultAck OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotion_Lib_Module::GetErrorCodeText(MOU_ERROR status, CString &str)//取得錯誤描述
{
	switch ( status )
	{
	case MOU_ERR_SUCCESS:	str=_T("MOU_ERR_SUCCESS");	break;

	case MOU_ERR_FAILED:	str=_T("MOU_ERR_FAILED");	break;
	case MOU_ERR_NOT_IMPLEMENTED:	str=_T("MOU_ERR_NOT_IMPLEMENTED");	break;
	case MOU_ERR_DEVICE_NOT_OPEN:	str=_T("MOU_ERR_DEVICE_NOT_OPEN");	break;
	case MOU_ERR_INVALID_HANDLE:	str=_T("MOU_ERR_INVALID_HANDLE");	break;
	case MOU_ERR_AXIS_INVALID:	str=_T("MOU_ERR_AXIS_INVALID");	break;
	case MOU_ERR_INITIALIZE_FAIL:	str=_T("MOU_ERR_INITIALIZE_FAIL");	break;
	case MOU_ERR_MODULE_TYPE_FAIL:	str=_T("MOU_ERR_MODULE_TYPE_FAIL");	break;

	case MOU_ERR_COMMAND_INVALID:	str=_T("MOU_ERR_COMMAND_INVALID");	break;
	case MOU_ERR_COMMAND_PARAMETER_INVALID:	str=_T("MOU_ERR_COMMAND_PARAMETER_INVALID");	break;
	case MOU_ERR_COMMAND_TIME_OUT:	str=_T("MOU_ERR_COMMAND_TIME_OUT");	break;

	case MOU_ERR_LOAD_CFG_FILE_FAIL:	str=_T("MOU_ERR_LOAD_CFG_FILE_FAIL");	break;

	case MOU_ERR_INIT_FAILED_AXIS_X:	str=_T("MOU_ERR_INIT_FAILED_AXIS_X");	break;
	case MOU_ERR_INIT_FAILED_AXIS_Y:	str=_T("MOU_ERR_INIT_FAILED_AXIS_Y");	break;
	case MOU_ERR_INIT_FAILED_AXIS_Z:	str=_T("MOU_ERR_INIT_FAILED_AXIS_Z");	break;
	case MOU_ERR_INIT_FAILED_AXIS_T:	str=_T("MOU_ERR_INIT_FAILED_AXIS_T");	break;

	case MOU_ERR_INIT_FAILED_AXIS_A:	str=_T("MOU_ERR_INIT_FAILED_AXIS_A");	break;
	case MOU_ERR_INIT_FAILED_AXIS_B:	str=_T("MOU_ERR_INIT_FAILED_AXIS_B");	break;
	case MOU_ERR_INIT_FAILED_AXIS_C:	str=_T("MOU_ERR_INIT_FAILED_AXIS_C");	break;
	case MOU_ERR_INIT_FAILED_AXIS_D:	str=_T("MOU_ERR_INIT_FAILED_AXIS_D");	break;

	case MOU_ERR_ENABEL_FAILED_AXIS_X:	str=_T("MOU_ERR_ENABEL_FAILED_AXIS_X");	break;
	case MOU_ERR_ENABEL_FAILED_AXIS_Y:	str=_T("MOU_ERR_ENABEL_FAILED_AXIS_Y");	break;
	case MOU_ERR_ENABEL_FAILED_AXIS_Z:	str=_T("MOU_ERR_ENABEL_FAILED_AXIS_Z");	break;
	case MOU_ERR_ENABEL_FAILED_AXIS_T:	str=_T("MOU_ERR_ENABEL_FAILED_AXIS_T");	break;
	case MOU_ERR_ENABEL_FAILED_AXIS_A:	str=_T("MOU_ERR_ENABEL_FAILED_AXIS_A");	break;
	case MOU_ERR_ENABEL_FAILED_AXIS_B:	str=_T("MOU_ERR_ENABEL_FAILED_AXIS_B");	break;
	case MOU_ERR_ENABEL_FAILED_AXIS_C:	str=_T("MOU_ERR_ENABEL_FAILED_AXIS_C");	break;
	case MOU_ERR_ENABEL_FAILED_AXIS_D:	str=_T("MOU_ERR_ENABEL_FAILED_AXIS_D");	break;
	case MOU_ERR_DRIVER_ALARM_AXIS_X:	str=_T("MOU_ERR_DRIVER_ALARM_AXIS_X");	break;
	case MOU_ERR_DRIVER_ALARM_AXIS_Y:	str=_T("MOU_ERR_DRIVER_ALARM_AXIS_Y");	break;
	case MOU_ERR_DRIVER_ALARM_AXIS_Z:	str=_T("MOU_ERR_DRIVER_ALARM_AXIS_Z");	break;
	case MOU_ERR_DRIVER_ALARM_AXIS_T:	str=_T("MOU_ERR_DRIVER_ALARM_AXIS_T");	break;

	case MOU_ERR_DRIVER_ALARM_AXIS_A:	str=_T("MOU_ERR_DRIVER_ALARM_AXIS_A");	break;
	case MOU_ERR_DRIVER_ALARM_AXIS_B:	str=_T("MOU_ERR_DRIVER_ALARM_AXIS_B");	break;
	case MOU_ERR_DRIVER_ALARM_AXIS_C:	str=_T("MOU_ERR_DRIVER_ALARM_AXIS_C");	break;
	case MOU_ERR_DRIVER_ALARM_AXIS_D:	str=_T("MOU_ERR_DRIVER_ALARM_AXIS_D");	break;

	case MOU_ERR_ENCODER_FAILED_AXIS_X:	str=_T("MOU_ERR_ENCODER_FAILED_AXIS_X");	break;
	case MOU_ERR_ENCODER_FAILED_AXIS_Y:	str=_T("MOU_ERR_ENCODER_FAILED_AXIS_Y");	break;
	case MOU_ERR_ENCODER_FAILED_AXIS_Z:	str=_T("MOU_ERR_ENCODER_FAILED_AXIS_Z");	break;
	case MOU_ERR_ENCODER_FAILED_AXIS_T:	str=_T("MOU_ERR_ENCODER_FAILED_AXIS_T");	break;
	case MOU_ERR_ENCODER_FAILED_AXIS_A:	str=_T("MOU_ERR_ENCODER_FAILED_AXIS_A");	break;
	case MOU_ERR_ENCODER_FAILED_AXIS_B:	str=_T("MOU_ERR_ENCODER_FAILED_AXIS_B");	break;
	case MOU_ERR_ENCODER_FAILED_AXIS_C:	str=_T("MOU_ERR_ENCODER_FAILED_AXIS_C");	break;
	case MOU_ERR_ENCODER_FAILED_AXIS_D:	str=_T("MOU_ERR_ENCODER_FAILED_AXIS_D");	break;

	default:
		str.Format(_T("Undefine MOU_ERR[%d]"), status);
		break;
	}
}
//-------------------------------------------------------------------------------------//
int CMotion_Lib_Module::GetAxisStatus(int AxisNo, CString &str)//取得錯誤描述
{
	if ( this->CheckInit() == false ) { return -1; }
	if ( this->CheckAxis(AxisNo) == false ) { return -1; }
	
	str = _T("");
#ifndef MOTION_OBJ_DISABLE
	MOU_AXIS_STATUS_MT eStatus;
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;	
	ErrCode = mouGetStatusMT(m_hMotion, m_Axis[AxisNo], eStatus);	
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);		
		return -1;
	}
	
	int nMask = 0;		
	const int nStatus = eStatus;
	nMask = MOU_AXIS_STOP;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_OVER_CURRENT;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_WAIT_CSTA;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_WAIT_IN_SYN_SNG;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_WAITING_IN_SYN_SNG;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_WAIT_ERC_FNH;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_WAIT_DIR_CHG;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_BACKlash_COMP;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_WAIT_PA_PB;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_IN_HOME_SPD_MOTION;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_IN_START_VEL_MOTION;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_IN_ACCLERATION;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_IN_MAX_VEL_MOTION;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}
	
	nMask = MOU_AXIS_IN_DECELERATION;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}

	nMask = MOU_AXIS_WAIT_INP;
	if ( CheckBitMask(nStatus, nMask) == true )
	{	
		str = GetStatusText(nMask);
		return nStatus;
	}
	return nStatus;
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
const TCHAR* CMotion_Lib_Module::GetAxisStatus(const int Axis)
{
	GetAxisStatus(Axis, m_MotionStatus);
	return m_MotionStatus;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::ResetMotionDriver()//重新復歸運動的Driver
{
#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::ResetMotionDriver Start"));
	DoFaultAck(AXIS_X);
	DoFaultAck(AXIS_Y);
	DoFaultAck(AXIS_Z);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::ResetMotionDriver OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
CString CMotion_Lib_Module::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("MOTION_LIB_MODULE");
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CMotion_Lib_Module::GetIOText(int IO)//取得IO的文字
{
	switch ( IO )
	{
	case MOU_AXIS_READY:	        m_MotionIOStats = _T("Ready");	break;
	case MOU_AXIS_ALARM:	        m_MotionIOStats = _T("Alarm signal");	break;
	case MOU_AXIS_FORWARD_LIMITSWITCH:	m_MotionIOStats = _T("Positive Limit");	break;
	case MOU_AXIS_BACKWARD_LIMITSWITCH:	m_MotionIOStats = _T("Negative Limit");	break;
	case MOU_AXIS_IN_ORG:	            m_MotionIOStats = _T("Origin signal");	break;
	case MOU_AXIS_DIR:	        m_MotionIOStats = _T("DIR output");	break;
	case MOU_AXIS_EMERGENCY_STOP:	m_MotionIOStats = _T("PCS signal input");	break;
	case MOU_AXIS_PCS:	        m_MotionIOStats = _T("ERC pin output");	break;
	case MOU_AXIS_ERC:	        m_MotionIOStats = _T("EZ index signalt");	break;
	case MOU_AXIS_EZ:	        m_MotionIOStats = _T("EZ index signalt");	break;
	case MOU_AXIS_CLR:	        m_MotionIOStats = _T("CLR signalt");	break;
	case MOU_AXIS_LATCH:	    m_MotionIOStats = _T("Latch signal input");	break;
	case MOU_AXIS_SLOW_DOWN:	m_MotionIOStats = _T("Slow down signal input");	break;
	case MOU_AXIS_IN_POSITION:	m_MotionIOStats = _T("In-Position signal input");	break;
	case MOU_AXIS_SERVO_ON:	    m_MotionIOStats = _T("Servo-ON output status");	break;
	case MOU_AXIS_RALM:	        m_MotionIOStats = _T("Servo-ON output status");	break;
	default:
		m_MotionIOStats = _T("No defined");
		break;
	}
	return m_MotionIOStats;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CMotion_Lib_Module::GetStatusText(int Stats)//取得狀態的文字
{
	switch ( Stats )
	{
	case MOU_AXIS_STOP:				   m_MotionStatus = _T("Stop");	break;
	case MOU_AXIS_OVER_CURRENT:	       m_MotionStatus = _T("Over Current");	break;
	case MOU_AXIS_WAIT_CSTA:	       m_MotionStatus = _T("Wait CSTA (Synchronous start signal)");	break;
	case MOU_AXIS_WAIT_IN_SYN_SNG:	   m_MotionStatus = _T("Wait Internal sync. signal");	break;
	case MOU_AXIS_WAITING_IN_SYN_SNG:  m_MotionStatus = _T("Waiting Internal sync. signal");	break;	
	case MOU_AXIS_WAIT_ERC_FNH:	       m_MotionStatus = _T("Wait ERC finished");	break;
	case MOU_AXIS_WAIT_DIR_CHG:	       m_MotionStatus = _T("Wait DIR Change");	break;
	case MOU_AXIS_BACKlash_COMP:	   m_MotionStatus = _T("Backlash compensating");	break;
	case MOU_AXIS_WAIT_PA_PB:	       m_MotionStatus = _T("Wait PA/PB");	break;
	case MOU_AXIS_IN_HOME_SPD_MOTION:  m_MotionStatus = _T("In home special speed motion");	break;
	case MOU_AXIS_IN_START_VEL_MOTION: m_MotionStatus = _T("In start velocity motion");	break;
	case MOU_AXIS_IN_ACCLERATION:	   m_MotionStatus = _T("In acceleration");	break;
	case MOU_AXIS_IN_MAX_VEL_MOTION:   m_MotionStatus = _T("In Max velocity motion");	break;
	case MOU_AXIS_IN_DECELERATION:	   m_MotionStatus = _T("In deceleration");	break;
	case MOU_AXIS_WAIT_INP:	           m_MotionStatus = _T("Wait INP");	break;
	default:
		m_MotionStatus.Format(_T("Not Define[%d]"), Stats);
		break;
	}
	return m_MotionStatus;
}
//-------------------------------------------------------------------------------------//
int CMotion_Lib_Module::GetIOStatus(int AxisNo, LPTSTR Str)//回傳I/O狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字	
{
	int io_sts = 0;
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIOStatus Start"));		
	
	MOU_AXIS_STATUS_IO eStatus;
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;	
	ErrCode = mouGetStatusIO(m_hMotion, m_Axis[AxisNo], eStatus);	
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIOStatus NG-1 End"));
		return -1;
	}
	io_sts = eStatus;

	if ( NULL != Str )
	{
		int IoID = 0;
		CString Status;
		CString stringbuff;		
		IoID = MOU_AXIS_READY;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}
	
		IoID = MOU_AXIS_ALARM;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}
	
		IoID = MOU_AXIS_FORWARD_LIMITSWITCH;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_BACKWARD_LIMITSWITCH;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}


		IoID = MOU_AXIS_IN_ORG;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_DIR;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_EMERGENCY_STOP;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_PCS;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_ERC;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}
	
		IoID = MOU_AXIS_EZ;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_CLR;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_LATCH;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_SLOW_DOWN;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_IN_POSITION;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_SERVO_ON;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}

		IoID = MOU_AXIS_RALM;
		if ( CheckBitMask(io_sts, IoID) == true )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }	
		}
		_tcscpy(Str, Status);		
	}
#endif//MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::GetIOStatus OK-2 End"));
	return io_sts;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::FireSingleTrigger()//送出單一觸發訊號(包含燈源與相機)
{
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	const double TriggerBetweenTime = TRIGGER_CCD_BETWEEN_LED;//ms	
	//--------------------------------------------------------------------------------------------------------------//	
	this->m_ErrorString.Format(_T("Error, JET-Module does not support Fire single trigger mode"));
	JetAPI::ShowMessageBox(m_ErrorString);
	return false;
	//--------------------------------------------------------------------------------------------------------------//	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::EnableSoftwareLimit(int AxisNo)//啟用軟體極限
{
#ifndef MOTION_OBJ_DISABLE	
	TCHAR AxisS[8]=_T("");
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	if ( this->CheckInit() == false ) { return false; }		
	if ( this->CheckAxis(AxisNo, AxisS) == false ) 
	{	return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("EnableSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::EnableSoftwareLimit Start"));
	ErrCode = mouEnableSWLimit(m_hMotion, m_Axis[AxisNo] );
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::EnableSoftwareLimit NG End"));
		return false;
	}
	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( AxisNo == AXIS_X )
		{
		//	if ( EnableSoftwareLimit(AXIS_X_SLAVE) == false )
		//	{	return false;	}			
		}
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::EnableSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::DisableSoftwareLimit(int AxisNo)//關閉軟體極限
{
#ifndef MOTION_OBJ_DISABLE
	TCHAR AxisS[8]=_T("");
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	if ( this->CheckInit() == false ) { return false; }		
	if ( this->CheckAxis(AxisNo, AxisS) == false ) 
	{	return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("DisableSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::DisableSoftwareLimit Start"));

	ErrCode = mouDisableSWLimit(m_hMotion, m_Axis[AxisNo]);
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::DisableSoftwareLimit NG End"));
		return false;
	}
	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( AxisNo == AXIS_X )
		{
			if ( DisableSoftwareLimit(AXIS_X_SLAVE) == false )
			{	return false;	}
		}
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::DisableSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Lib_Module::SetSoftwareLimit(int AxisNo, double Min, double Max, bool Auto)//設定軟體極限
{
#ifndef MOTION_OBJ_DISABLE
	TCHAR AxisS[8]=_T("");
	MOU_ERROR ErrCode = MOU_ERR_SUCCESS;
	if ( this->CheckInit() == false ) { return false; }		
	if ( this->CheckAxis(AxisNo, AxisS) == false ) 
	{	return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("SetSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::SetSoftwareLimit Start"));

	if ( Auto == true ) 
	{	
		if ( DisableSoftwareLimit(AxisNo) == false )
		{	return false; }
	}
	
	double Gap = 10;
	double dMin=Min;
	double dMax=Max;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(AxisNo);
		if ( FN_DISABLE == nSign )
		{
			dMin = MIN(-Min, -Max);
			dMax = MAX(-Min, -Max);
		}
	}
	ErrCode = mouSetSWLimit(m_hMotion, m_Axis[AxisNo], dMax+Gap, dMin-Gap);
	if ( MOU_ERR_SUCCESS != ErrCode )
	{
		GetErrorCodeText(ErrCode, m_ErrorString);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::SetSoftwareLimit NG End"));
		return false;
	}	

	double Max2=0, Min2=0;
	ErrCode = mouGetSWLimit(m_hMotion, m_Axis[AxisNo], Max2, Min2);
	if ( Auto == true ) 
	{		
		if ( EnableSoftwareLimit(AxisNo) == false ) 
		{	return false; }
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_Lib_Module::SetSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
#endif//MOTION_DERIVE_MODE