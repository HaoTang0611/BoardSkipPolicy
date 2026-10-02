// Motion_PCE_M114GL.cpp: implementation of the CMotion_PCE_M114GL class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Motion_PCE_M114GL.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#if MOTION_DERIVE_MODE == MOTION_PCE_M114GL
//-------------------------------------------------------------------------------------//
CMotion_PCE_M114GL  Motion_PCE_M114GL;
//-------------------------------------------------------------------------------------//
#define MAX_MOTION_AXIS        4//運動控制卡支援運動軸數
#define MAX_USED_AXIS          3//系統使用的運動軸數
//-------------------------------------------------------------------------------------//
#define HOME_START_VELOCITY    1000
#define HOME_MAX_VELOCITY      50000
#define HOME_ACCELERATE_TIME   0.1
#define HOME_PRE_MOVE_DIST     100000
//-------------------------------------------------------------------------------------//
#define PCE_M114GL_IO_RDY      0x0001//RDY pin input-0
#define PCE_M114GL_IO_ALM      0x0002//Alam Signal-1
#define PCE_M114GL_IO_PEL      0x0004//Positive Limit Switch-2
#define PCE_M114GL_IO_NEL      0x0008//Negative Limit Switch-3
#define PCE_M114GL_IO_ORG      0x0010//Origin Switch-4
#define PCE_M114GL_IO_DIR      0x0020//Dir Output-5-Reserved
#define PCE_M114GL_IO_EMG      0x0040//Emergency signal input-6
#define PCE_M114GL_IO_PCS      0x0080//PCS signal input-7-Reserved
#define PCE_M114GL_IO_ERC      0x0100//ERC signal input-8
#define PCE_M114GL_IO_EZ       0x0200//Index signal-9-Reserved
#define PCE_M114GL_IO_CLR      0x0400//Clear counter signal-10-Reserved
#define PCE_M114GL_IO_LATCH    0x0800//Latch Signal Input-11-Reserved
#define PCE_M114GL_IO_SD       0x1000//Slow Down signal input-12
#define PCE_M114GL_IO_INP      0x2000//In-Position signal input-13
#define PCE_M114GL_IO_SVON     0x4000//Servo-ON output status-14
#define PCE_M114GL_IO_RALM     0x8000//Reset Alarm output status-15
//---------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CMotion_PCE_M114GL, CMotion_Basic)
//-------------------------------------------------------------------------------------//
CMotion_PCE_M114GL::CMotion_PCE_M114GL()
{
	CMotion_PCE_M114GL::PreInitMotion();
	SetInitialize(false);	
	this->m_MotionStatus = _T("");
	this->m_ErrorString = _T("");
	this->m_IsJogMode = false;
	this->m_TriggerNTriggers = 0;
	SetIsSupportGantry(false);
#ifdef OFFLINE_VERSION
	BuildMotionAxisList();
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
CMotion_PCE_M114GL::~CMotion_PCE_M114GL()
{
	this->m_MotionCallbackHWnd = NULL;	
	m_IsMotionRelease = true;
	if ( this->ReleaseMotion() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
	}
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::SetCallBackFunction(void ( __stdcall *callbackAddr)(I16 IntAxisNoInCard))
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(-1, _T("SetCallBackFunction"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetCallBackFunction Start"));
	/*
	I16 ErrorStatus = ERR_NoError;
	ErrorStatus = _8164_link_interrupt(0, callbackAddr);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		if ( ErrorStatus == ERR_LinkIntError )
		{	::strcpy(m_ErrorString, "Error, _8164_link_interrupt, ERR_LinkIntError"); }
		else if ( ErrorStatus == ERR_CardNoError )
		{::strcpy(ErrorStatus, "Error, _8164_link_interrupt, ERR_CardNoError");	}
		else if ( ErrorStatus == ERR_EventNotEnableYet )
		{::strcpy(ErrorStatus, "Error, _8164_link_interrupt, ERR_EventNotEnableYet"); }
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetCallBackFunction NG-1 End"));
		return false;
	}
	*/
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetCallBackFunction OK End"));
#endif
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::InitCallbackFunction()
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(-1, _T("InitCallbackFunction"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::InitCallbackFunction Start"));	
	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::InitCallbackFunction OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::InitializeForJET7300()//JET7300的初始化	
{	
	this->ReleaseMotionCard();
#ifndef MOTION_OBJ_DISABLE
	int i=0;	
	U16 TotalCard=0;	
	I16 ErrorStatus = ERR_NoError;
	SetCardNo(-1);
	ErrorStatus = _m114_open(&TotalCard);
	if ( CheckReturnOK(ErrorStatus) == false )	//Card initial
	{
		m_ErrorString.Format(_T("Error, _m114_open(&TotalCard)"));
		return false;
	}

	if ( TotalCard == 0 )
	{
		m_ErrorString.Format(_T("Error, No PCE_M114_GL Inside IPC"));
		return false; 
	}

	//_M114GL_initial	
	for ( i=0; i<8; i++ )
	{
		ErrorStatus = _m114_initial(i);
		if ( CheckReturnOK(ErrorStatus) == true )
		{
			SetCardNo(i);
			break;
		}		
	}
	const int nCardNo = (int)(GetCardNo());
	if ( nCardNo < 0 ) 
	{
		m_ErrorString.Format(_T("Error, _m114_initial(CardNO)"));
		return false;
	}

	U8 CardType = 0;
	CString CardTypeName;
	ErrorStatus = _m114_get_card_type(nCardNo, &CardType);	
	if ( CheckReturnOK(ErrorStatus) == false )	//Card initial
	{	m_ErrorString.Format(_T("Error, _m114_get_card_type(CardNo, &CardType)"));	}
	CardTypeName = GetCardTypeText(CardType);
	
	if ( BuildMotionAxisList() == false )
	{	return false; }

	U16 Axis=0;
	U16 CardNo=0;
	U16 CardAxis=0;	
	std::vector<CMotionAxis> &MotionAxisList=GetMotionAxisList();
	const size_t MotionAxisCount=MotionAxisList.size();
	for(i=0;i<MotionAxisCount;i++)
	{	
		CMotionAxis &MotionAxisRef=MotionAxisList[i];		
		if ( CheckAxisBypass(&MotionAxisRef) == true ) { continue; }

		Axis = MotionAxisRef.GetAxisID();
		CardNo = MotionAxisRef.GetCardID();
		CardAxis = MotionAxisRef.GetNodeID();

		//set pulse command output mode		
		if ( Axis == AXIS_Z )
		{	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 7); }
		else
		{	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 7); }
		//1->OUT/DIROUT Rising edge, DIR+ is high level
		//7->AB Phase, B phase leads A
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_pls_outmode(CardNo, AXIS, 7)"));
			return false;
		}

		//set encoder input mode
		ErrorStatus = _m114_set_pls_iptmode(CardNo, CardAxis, 2, 0);
		if ( CheckReturnOK(ErrorStatus) == false )  //2->4X A/B, 1->Inverse direction
		{
			m_ErrorString.Format(_T("Error, _m114_set_pls_iptmode(CardNo,AXIS,2,0)"));
			return false;
		}

		//set counter input source //注意一定要這樣設定		
		ErrorStatus = _m114_set_feedback_src(CardNo, CardAxis,0);
		if ( CheckReturnOK(ErrorStatus) == false )  //0->External Feedback
		{
			m_ErrorString.Format(_T("Error, _m114_set_feedback_src(CardNo,AXIS,0)"));
			return false;
		}
		
		//set abs position reference//注意一定要這樣設定		
		//_m114_set_abs_reference(U16 SwitchCardNo, U16 AxisNo, I16 Ref);
		ErrorStatus = _m114_set_abs_reference(CardNo, CardAxis, 0);//0->Encode, 1->Command, 2->Target
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_abs_reference(CardNo,AXIS,0)"));
			return false;
		}

		//Set Home Config						
		//_m114_set_home_config(U16 SwitchCardNo, U16 AxisNo, I16 home_mode, I16 org_logic, I16 ez_logic, I16 ez_count, I16 erc_out);
		//ErrorStatus = _m114_set_home_config(CardNo, CardAxis, HOME_MODE, HOME_ORG_LOGIC, HOME_EZ_LOGIC, HOME_EZ_COUNT, HOME_ERC_OUT);
		//ErrorStatus = _m114_set_home_config(CardNo, CardAxis, 0, 1, 1, 3, 0);
		ErrorStatus = _m114_set_home_config(CardNo, CardAxis, 0, 1, 1, 3, 1);
		if ( CheckReturnOK(ErrorStatus) == false ) 
		{
			this->m_ErrorString.Format(_T("Error, _m114_set_home_config Fault"));
			return false; 
		}

		//set alarm logic = Low//_m114_set_alm(U16 SwitchCardNo, U16 AxisNo, I16 alm_logic, I16 alm_mode)
		ErrorStatus = _m114_set_alm(CardNo,CardAxis,0,0);
		if ( CheckReturnOK(ErrorStatus) == false ) //0->Active at Low, 0->Immediate stop
		{
			m_ErrorString.Format(_T("Error, _m114_set_alm(CardNo,AXIS,0,0)"));
			return false;
		}

		//set end limit logic 
		ErrorStatus = _m114_set_ell(CardNo,CardAxis, 1, 0);//1->high, 0->Immediate stop		
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_ell(CardNo,AXIS,1, 0)"));
			return false;
		}

		//set servo on logic = low for J2S 		
		ErrorStatus = _m114_set_servo(CardNo,CardAxis,0);
		if ( CheckReturnOK(ErrorStatus) == false ) //0->Inactive, 1->Active
		{
			m_ErrorString.Format(_T("Error, _m114_set_servo(CardNo,AXIS,0)"));
			return false;
		}

		//set in-position logic
		//_m114_set_inp(U16 SwitchCardNo, U16 AxisNo, I16 inp_enable, I16 inp_logic);
		ErrorStatus = _m114_set_inp(CardNo,CardAxis, 1, 1);	
		if ( CheckReturnOK(ErrorStatus) == false ) //0->Disable, 0->active at low
		{
			m_ErrorString.Format(_T("Error, _m114_set_inp(CardNo,AXIS, 1, 1)"));
			return false;
		}

		//set move ratio (command/feedback)//No this Function
		//ErrorStatus = _M114GL_set_move_ratio(CardNo,CardAxis, 1); 
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_move_ratio(CardNo,AXIS, 1)"));
		//	return false;
		//}				
	}
	//set call back function
	U32 factor=0x0000;
	//No this Function
	//_m114_set_int_factor(CardNo, AXIS_X, factor);//0-AxisNO	
	//_m114_set_int_factor(CardNo, AXIS_Y, factor);//1-AxisNO
	//_m114L_set_int_factor(CardNo, AXIS_Z, factor);//2-AxisNO

	this->DisableSoftwareLimit(AXIS_X);
	this->DisableSoftwareLimit(AXIS_Y);	
	this->DisableSoftwareLimit(AXIS_Z);	
#endif//MOTION_OBJ_DISABLE
	SetInitialize(true);
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::InitializeForJET6500()//JET6500的初始化
{	
	this->ReleaseMotionCard();
#ifndef MOTION_OBJ_DISABLE
	int i=0;	
	U16 TotalCard=0;	
	I16 ErrorStatus = ERR_NoError;
	SetCardNo(-1);
	ErrorStatus = _m114_open(&TotalCard);
	if ( CheckReturnOK(ErrorStatus) == false )	//Card initial
	{
		m_ErrorString.Format(_T("Error, _m114_open(&TotalCard)"));
		return false;
	}

	if ( TotalCard == 0 )
	{
		m_ErrorString.Format(_T("Error, No PCE_M114_GL Inside IPC"));
		return false; 
	}
	
	//_M114GL_initial	
	for ( i=0; i<8; i++ )
	{
		ErrorStatus = _m114_initial(i);
		if ( CheckReturnOK(ErrorStatus) == true )
		{
			SetCardNo(-1);			
			break;
		}		
	}
	const int nCardNo = (int)(GetCardNo());
	if ( nCardNo < 0 ) 
	{
		m_ErrorString.Format(_T("Error, _m114_initial(CardNO)"));
		return false;
	}

	U8 CardType = 0;
	CString CardTypeName;
	ErrorStatus = _m114_get_card_type(nCardNo, &CardType);	
	if ( CheckReturnOK(ErrorStatus) == false )	//Card initial
	{	m_ErrorString.Format(_T("Error, _m114_get_card_type(CardNo, &CardType)"));	}
	CardTypeName = GetCardTypeText(CardType);
	
	if ( BuildMotionAxisList() == false )
	{	return false; }

	U16 Axis=0;
	U16 CardNo=0;
	U16 CardAxis=0;	
	std::vector<CMotionAxis> &MotionAxisList=GetMotionAxisList();
	const size_t MotionAxisCount=MotionAxisList.size();
	for(i=0;i<MotionAxisCount;i++)
	{	
		CMotionAxis &MotionAxisRef=MotionAxisList[i];		
		if ( CheckAxisBypass(&MotionAxisRef) == true ) { continue; }

		Axis = MotionAxisRef.GetAxisID();
		CardNo = MotionAxisRef.GetCardID();
		CardAxis = MotionAxisRef.GetNodeID();

		//set pulse command output mode		
		switch ( Axis )
		{
		case AXIS_X:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 4);	break;
		case AXIS_Y:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 7);	break;
		case AXIS_Z:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 5);	break;
		default:	    ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 4);	break;
		}		
		if ( CheckReturnOK(ErrorStatus) == false )	//1->OUT/DIROUT Rising edge, DIR+ is high level
		{
			m_ErrorString.Format(_T("Error, _m114_set_pls_outmode(CardNo, AXIS, 7)"));
			return false;
		}

		//set encoder input mode
		ErrorStatus = _m114_set_pls_iptmode(CardNo, CardAxis, 2, 1);
		if ( CheckReturnOK(ErrorStatus) == false )  //2->4X A/B, 1->Inverse direction
		{
			m_ErrorString.Format(_T("Error, _m114_set_pls_iptmode(CardNo,AXIS,2,1)"));
			return false;
		}

		//set counter input source //注意一定要這樣設定		
		ErrorStatus = _m114_set_feedback_src(CardNo, CardAxis,0);//0->External Feedback
		if ( CheckReturnOK(ErrorStatus) == false )  
		{
			m_ErrorString.Format(_T("Error, _m114_set_feedback_src(CardNo,AXIS,0)"));
			return false;
		}
				
		//set abs position reference//注意一定要這樣設定		
		//_m114_set_abs_reference(U16 SwitchCardNo, U16 AxisNo, I16 Ref);
		ErrorStatus = _m114_set_abs_reference(CardNo, CardAxis, 0);//0->Encode, 1->Command, 2->Target
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_abs_reference(CardNo,AXIS,0)"));
			return false;
		}
		//Set Home Config					
		I16 home_mode = 0;//0, or 6
		//_m114_set_home_config(SelectCard= 3,SelectAxis= 0,home_mode= 0,org_logic= 1,ez_logic= 1,ez_count= 3,ERC_output= 0);		
		ErrorStatus = _m114_set_home_config(CardNo, CardAxis, home_mode, 1, 1, 3, 1);
		if ( CheckReturnOK(ErrorStatus) == false ) 
		{
			this->m_ErrorString.Format(_T("Error, _m114_set_home_config Fault"));
			return false; 
		}

		//set alarm logic = Low				
		ErrorStatus = _m114_set_alm(CardNo,CardAxis, 0, 0);
		if ( CheckReturnOK(ErrorStatus) == false ) //0->Active at Low, 0->Immediate stop
		{
			m_ErrorString.Format(_T("Error, _m114_set_alm(CardNo,AXIS,0,0)"));
			return false;
		}

		//set end limit logic 
		//ErrorStatus = _m114_set_ell(CardNo,AXIS, 1, 0);//1->high
		ErrorStatus = _m114_set_ell(CardNo,CardAxis, 0, 1);//0->Low, //0->Immediate stop,1->Slow to Stop		
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_ell(CardNo,AXIS,1,0)"));
			return false;
		}

		//set servo on logic = low for J2S 		
		ErrorStatus = _m114_set_servo(CardNo,CardAxis,0);
		if ( CheckReturnOK(ErrorStatus) == false ) //1->turn off
		{
			m_ErrorString.Format(_T("Error, _m114_set_servo(CardNo,AXIS,0)"));
			return false;
		}

		//set in-position logic			
		ErrorStatus = _m114_set_inp(CardNo,CardAxis, 1, 1);	
		if ( CheckReturnOK(ErrorStatus) == false ) //0->Disable, 0->active at low
		{
			m_ErrorString.Format(_T("Error, _m114_set_inp(CardNo,AXIS, 1, 1)"));
			return false;
		}

		//set move ratio (command/feedback)//No this Function
		//ErrorStatus = _M114GL_set_move_ratio(CardNo,CardAxis, 1); 
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_move_ratio(CardNo,AXIS, 1)"));
		//	return false;
		//}

		//turn off ERC signal
		ErrorStatus = _m114_set_erc_on(CardNo,CardAxis, 0);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_erc_on(CardNo,AXIS, 0)"));			
			return false;
		}

		//set ERC turn on mode (0~6=>time, 7=level output)
		ErrorStatus = _m114_set_erc(CardNo,CardAxis, 0, 7);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_erc(CardNo,AXIS, 0, 7)"));			
			return false;
		}

		//set SD(slow down) configuration
		//I16 status= _m114_set_sd(U16 SwitchCardNo, U16 AxisNo, I16 enable,I16 sd_logic, I16 sd_latch, I16 sd_mode)
		ErrorStatus = _m114_set_sd(CardNo,CardAxis, 1, 1, 0, 1);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_sd(CardNo,AXIS, 1, 1, 0, 1)"));			
			return false;
		}

		//set INT(interrupt) mode//No this Function
		//ErrorStatus = _M114GL_set_int_factor(CardNo,AXIS, 0x0000);//normal stop
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_int_factor(CardNo,AXIS, 0)"));
		//	return false;
		//}

		//disable software limit
		ErrorStatus = _m114_disable_soft_limit(CardNo, CardAxis);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_disable_soft_limit(CardNo,AXIS)"));			
			return false;
		}		
	}	
#endif//MOTION_OBJ_DISABLE
	SetInitialize(true);
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::InitializeForJET8000()//JET8000的初始化
{
	this->ReleaseMotionCard();	
#ifndef MOTION_OBJ_DISABLE
	int i=0;	
	U16 TotalCard=0;	
	I16 ErrorStatus = ERR_NoError;
	SetCardNo(-1);
	ErrorStatus = _m114_open(&TotalCard);
	if ( CheckReturnOK(ErrorStatus) == false )	//Card initial
	{
		m_ErrorString.Format(_T("Error, _m114_open(&TotalCard)"));
		return false;
	}

	if ( TotalCard == 0 )
	{
		m_ErrorString.Format(_T("Error, No PCE_M114_GL Inside IPC"));
		return false; 
	}
	
	//_M114GL_initial	
	for ( i=0; i<8; i++ )
	{
		ErrorStatus = _m114_initial(i);
		if ( CheckReturnOK(ErrorStatus) == true )
		{
			SetCardNo(i);
			break;
		}		
	}
	const int nCardNo = (int)(GetCardNo());
	if ( nCardNo < 0 ) 
	{
		m_ErrorString.Format(_T("Error, _m114_initial(CardNO)"));
		return false;
	}

	U8 CardType = 0;
	CString CardTypeName;
	ErrorStatus = _m114_get_card_type(nCardNo, &CardType);	
	if ( CheckReturnOK(ErrorStatus) == false )	//Card initial
	{	m_ErrorString.Format(_T("Error, _m114_get_card_type(CardNo, &CardType)"));	}
	CardTypeName = GetCardTypeText(CardType);
	
	if ( BuildMotionAxisList() == false )
	{	return false; }

	U16 Axis=0;
	U16 CardNo=0;
	U16 CardAxis=0;	
	std::vector<CMotionAxis> &MotionAxisList=GetMotionAxisList();
	const size_t MotionAxisCount=MotionAxisList.size();
	for(i=0;i<MotionAxisCount;i++)
	{	
		CMotionAxis &MotionAxisRef=MotionAxisList[i];		
		if ( CheckAxisBypass(&MotionAxisRef) == true ) { continue; }

		Axis = MotionAxisRef.GetAxisID();
		CardNo = MotionAxisRef.GetCardID();
		CardAxis = MotionAxisRef.GetNodeID();
		//set pulse command output mode		
		//_m114_set_pls_outmode(U16 SwitchCardNo, U16 AxisNo, I16 pls_outmode);//pls_outmode:6,7(A/B Phase)
		switch ( Axis )
		{
		case AXIS_X:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 7);	break;
		case AXIS_Y:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 6);	break;
		case AXIS_Z:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 7);	break;
		default:	    ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 4);	break;
		}		
		if ( CheckReturnOK(ErrorStatus) == false )	//1->OUT/DIROUT Rising edge, DIR+ is high level
		{
			m_ErrorString.Format(_T("Error, _m114_set_pls_outmode(CardNo, AXIS, 7)"));
			return false;
		}

		//set encoder input mode
		ErrorStatus = _m114_set_pls_iptmode(CardNo, CardAxis, 2, 0);//2->4X A/B, 1->Inverse direction
		if ( CheckReturnOK(ErrorStatus) == false )  
		{
			m_ErrorString.Format(_T("Error, _m114_set_pls_iptmode(CardNo,AXIS,2,1)"));
			return false;
		}

		//set counter input source //注意一定要這樣設定		
		ErrorStatus = _m114_set_feedback_src(CardNo, CardAxis,0);//0->External Feedback
		if ( CheckReturnOK(ErrorStatus) == false )  
		{
			m_ErrorString.Format(_T("Error, _m114_set_feedback_src(CardNo,AXIS,0)"));
			return false;
		}
				
		//set abs position reference//注意一定要這樣設定		
		//_m114_set_abs_reference(U16 SwitchCardNo, U16 AxisNo, I16 Ref);
		ErrorStatus = _m114_set_abs_reference(CardNo, CardAxis, 0);//0->Encode, 1->Command, 2->Target
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_abs_reference(CardNo,AXIS,0)"));
			return false;
		}
		//Set Home Config					
		I16 home_mode = 0;//0, or 6
		//_m114_set_home_config(SelectCard= 3,SelectAxis= 0,home_mode= 0,org_logic= 1,ez_logic= 1,ez_count= 3,ERC_output= 0);		
		int org_logic = 1;
		int ez_logic = 0;
		int ez_count = 0;
		int ERC_output = 0;
		ErrorStatus = _m114_set_home_config(CardNo, CardAxis, home_mode, org_logic, ez_logic, ez_count, ERC_output);
		if ( CheckReturnOK(ErrorStatus) == false ) 
		{
			this->m_ErrorString.Format(_T("Error, _m114_set_home_config Fault"));
			return false; 
		}

		//set alarm logic = Low				
		ErrorStatus = _m114_set_alm(CardNo,CardAxis, 0, 0);//0->Active at Low, 0->Immediate stop
		if ( CheckReturnOK(ErrorStatus) == false ) 
		{
			m_ErrorString.Format(_T("Error, _m114_set_alm(CardNo,AXIS,0,0)"));
			return false;
		}

		//set end limit logic 		
		ErrorStatus = _m114_set_ell(CardNo,CardAxis, 0, 0);//0->Low, //0->Immediate stop,1->Slow to Stop		
		if ( CheckReturnOK(ErrorStatus) == false )  
		{
			m_ErrorString.Format(_T("Error, _m114_set_ell(CardNo,AXIS,1,0)"));
			return false;
		}

		//set servo on logic = low for J2S 		
		ErrorStatus = _m114_set_servo(CardNo,CardAxis,0);
		if ( CheckReturnOK(ErrorStatus) == false ) //1->turn off
		{
			m_ErrorString.Format(_T("Error, _m114_set_servo(CardNo,AXIS,0)"));
			return false;
		}

		//set in-position logic			
		ErrorStatus = _m114_set_inp(CardNo,CardAxis, 1, 1);	//1->Enable, (0->active at low, 1->active at high)
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_inp(CardNo,AXIS, 1, 1)"));
			return false;
		}

		//set move ratio (command/feedback)//No this Function
		//ErrorStatus = _M114GL_set_move_ratio(CardNo,CardAxis, 1); 
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_move_ratio(CardNo,AXIS, 1)"));
		//	return false;
		//}

		//turn off ERC signal
		ErrorStatus = _m114_set_erc_on(CardNo,CardAxis, 0);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_erc_on(CardNo,AXIS, 0)"));			
			return false;
		}

		//set ERC turn on mode (0~6=>time, 7=level output)
		ErrorStatus = _m114_set_erc(CardNo,CardAxis, 0, 7);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_erc(CardNo,AXIS, 0, 7)"));			
			return false;
		}

		//set SD(slow down) configuration
		//I16 status= _m114_set_sd(U16 SwitchCardNo, U16 AxisNo, I16 enable,I16 sd_logic, I16 sd_latch, I16 sd_mode)
		ErrorStatus = _m114_set_sd(CardNo,CardAxis, 1, 1, 0, 1);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_sd(CardNo,AXIS, 1, 1, 0, 1)"));			
			return false;
		}

		//set INT(interrupt) mode//No this Function
		//ErrorStatus = _M114GL_set_int_factor(CardNo,CardAxis, 0x0000);//normal stop
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_int_factor(CardNo,AXIS, 0)"));
		//	return false;
		//}

		//disable software limit
		ErrorStatus = _m114_disable_soft_limit(CardNo, CardAxis);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_disable_soft_limit(CardNo,AXIS)"));			
			return false;
		}		
	}	
#endif//MOTION_OBJ_DISABLE
	SetInitialize(true);
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::InitializeForJET7500TB()//JET7500TB的初始化
{
	this->ReleaseMotionCard();	
#ifndef MOTION_OBJ_DISABLE
	int i=0;	
	U16 TotalCard=0;	
	I16 ErrorStatus = ERR_NoError;
	SetCardNo(-1);
	ErrorStatus = _m114_open(&TotalCard);
	if ( CheckReturnOK(ErrorStatus) == false )	//Card initial
	{
		m_ErrorString.Format(_T("Error, _m114_open(&TotalCard)"));
		return false;
	}

	if ( TotalCard == 0 )
	{
		m_ErrorString.Format(_T("Error, No PCE_M114_GL Inside IPC"));
		return false; 
	}

	//_M114GL_initial	
	for ( i=0; i<8; i++ )
	{
		ErrorStatus = _m114_initial(i);
		if ( CheckReturnOK(ErrorStatus) == true )
		{
			SetCardNo(i);
			break;
		}		
	}
	const int nCardNo = (int)(GetCardNo());
	if ( nCardNo < 0 ) 
	{
		m_ErrorString.Format(_T("Error, _m114_initial(CardNO)"));
		return false;
	}

	U8 CardType = 0;
	CString CardTypeName;
	ErrorStatus = _m114_get_card_type(nCardNo, &CardType);	
	if ( CheckReturnOK(ErrorStatus) == false )	//Card initial
	{	m_ErrorString.Format(_T("Error, _m114_get_card_type(CardNo, &CardType)"));	}
	CardTypeName = GetCardTypeText(CardType);

	if ( BuildMotionAxisList() == false )
	{	return false; }

	U16 Axis=0;
	U16 CardNo=0;
	U16 CardAxis=0;	
	std::vector<CMotionAxis> &MotionAxisList=GetMotionAxisList();
	const size_t MotionAxisCount=MotionAxisList.size();
	for(i=0;i<MotionAxisCount;i++)
	{	
		CMotionAxis &MotionAxisRef=MotionAxisList[i];		
		if ( CheckAxisBypass(&MotionAxisRef) == true ) { continue; }

		Axis = MotionAxisRef.GetAxisID();
		CardNo = MotionAxisRef.GetCardID();
		CardAxis = MotionAxisRef.GetNodeID();
		//set pulse command output mode		
		//_m114_set_pls_outmode(U16 SwitchCardNo, U16 AxisNo, I16 pls_outmode);//pls_outmode:6,7(A/B Phase)
		switch ( Axis )
		{
		case AXIS_X:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 7);	break;
		case AXIS_Y:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 6);	break;
		case AXIS_Z:	ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 7);	break;
		default:	    ErrorStatus = _m114_set_pls_outmode(CardNo, CardAxis, 7);	break;
		}		
		if ( CheckReturnOK(ErrorStatus) == false )	//1->OUT/DIROUT Rising edge, DIR+ is high level
		{
			m_ErrorString.Format(_T("Error, _m114_set_pls_outmode(CardNo, AXIS, 7)"));
			return false;
		}

		//set encoder input mode
		switch ( Axis )
		{
		case AXIS_X:	ErrorStatus = _m114_set_pls_iptmode(CardNo, CardAxis, 2, 0);	break;
		case AXIS_Y:	ErrorStatus = _m114_set_pls_iptmode(CardNo, CardAxis, 2, 1);	break;
		case AXIS_Z:	ErrorStatus = _m114_set_pls_iptmode(CardNo, CardAxis, 2, 1);	break;
		default:	    ErrorStatus = _m114_set_pls_iptmode(CardNo, CardAxis, 2, 1);	break;
		}	
		if ( CheckReturnOK(ErrorStatus) == false )  //2->4X A/B, 1->Inverse direction
		{
			m_ErrorString.Format(_T("Error, _m114_set_pls_iptmode(CardNo,AXIS,2,1)"));
			return false;
		}

		//set counter input source //注意一定要這樣設定		
		ErrorStatus = _m114_set_feedback_src(CardNo, CardAxis,0);//0->External Feedback
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_feedback_src(CardNo,AXIS,0)"));
			return false;
		}

		//set abs position reference//注意一定要這樣設定		
		//_m114_set_abs_reference(U16 SwitchCardNo, U16 AxisNo, I16 Ref);
		ErrorStatus = _m114_set_abs_reference(CardNo, CardAxis, 0);//0->Encode, 1->Command, 2->Target
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_abs_reference(CardNo,AXIS,0)"));
			return false;
		}
		//Set Home Config					
		I16 home_mode = 0;//0, or 6
						  //_m114_set_home_config(SelectCard= 3,SelectAxis= 0,home_mode= 0,org_logic= 1,ez_logic= 1,ez_count= 3,ERC_output= 0);		
		int org_logic = 1;
		int ez_logic = 0;
		int ez_count = 0;
		int ERC_output = 0;
		ErrorStatus = _m114_set_home_config(CardNo, CardAxis, home_mode, org_logic, ez_logic, ez_count, ERC_output);
		if ( CheckReturnOK(ErrorStatus) == false ) 
		{
			this->m_ErrorString.Format(_T("Error, _m114_set_home_config Fault"));
			return false; 
		}

		//set alarm logic = Low				
		ErrorStatus = _m114_set_alm(CardNo,CardAxis, 0, 0);//0->Active at Low, 0->Immediate stop
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_alm(CardNo,AXIS,0,0)"));
			return false;
		}

		//set end limit logic 		
		ErrorStatus = _m114_set_ell(CardNo,CardAxis, 0, 0);//0->Low, //0->Immediate stop,1->Slow to Stop		
		if ( CheckReturnOK(ErrorStatus) == false )  
		{
			m_ErrorString.Format(_T("Error, _m114_set_ell(CardNo,AXIS,1,0)"));
			return false;
		}

		//set servo on logic = low for J2S 		
		ErrorStatus = _m114_set_servo(CardNo,CardAxis,0);
		if ( CheckReturnOK(ErrorStatus) == false ) //1->turn off
		{
			m_ErrorString.Format(_T("Error, _m114_set_servo(CardNo,AXIS,0)"));
			return false;
		}

		//set in-position logic			
		ErrorStatus = _m114_set_inp(CardNo,CardAxis, 1, 1);	//1->Enable, (0->active at low, 1->active at high)
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_inp(CardNo,AXIS, 1, 1)"));
			return false;
		}

		//set move ratio (command/feedback)//No this Function
		//ErrorStatus = _M114GL_set_move_ratio(CardNo,CardAxis, 1); 
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_move_ratio(CardNo,AXIS, 1)"));
		//	return false;
		//}

		//turn off ERC signal
		ErrorStatus = _m114_set_erc_on(CardNo,CardAxis, 0);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_erc_on(CardNo,AXIS, 0)"));			
			return false;
		}

		//set ERC turn on mode (0~6=>time, 7=level output)
		ErrorStatus = _m114_set_erc(CardNo,CardAxis, 0, 7);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_erc(CardNo,AXIS, 0, 7)"));			
			return false;
		}

		//set SD(slow down) configuration
		//I16 status= _m114_set_sd(U16 SwitchCardNo, U16 AxisNo, I16 enable,I16 sd_logic, I16 sd_latch, I16 sd_mode)
		ErrorStatus = _m114_set_sd(CardNo,CardAxis, 1, 1, 0, 1);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_set_sd(CardNo,AXIS, 1, 1, 0, 1)"));			
			return false;
		}

		//set INT(interrupt) mode//No this Function
		//ErrorStatus = _M114GL_set_int_factor(CardNo,CardAxis, 0x0000);//normal stop
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_int_factor(CardNo,AXIS, 0)"));
		//	return false;
		//}

		//disable software limit
		ErrorStatus = _m114_disable_soft_limit(CardNo, CardAxis);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _m114_disable_soft_limit(CardNo,AXIS)"));			
			return false;
		}		
	}	
#endif//MOTION_OBJ_DISABLE
	SetInitialize(true);
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::Stop(U16 Axis)
{	
#ifndef MOTION_OBJ_DISABLE	
	const double Tdec = 0.15f;
	if ( CheckInit() == false ) { return false; }		
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return true; }
	
	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("Stop"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Stop Start"));
	//ErrorStatus = _m114_sd_stop(Axis,Tdec);	
	ErrorStatus = _m114_sd_stop(CardNo,CardAxis,Tdec);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		m_ErrorString.Format(_T("Error, _m114_sd_stop"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Stop NG End"));
		return false;
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Stop OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
int  CMotion_PCE_M114GL::GetIOStatus(int Axis, LPTSTR Str)//回傳錯誤狀態
{	
	U16 io_sts = 0;
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return -1; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return -1; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return 0; }
	
	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIOStatus Start"));	
	ErrorStatus = _m114_get_io_status(CardNo, CardAxis, &io_sts);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		m_ErrorString.Format(_T("Error, _m114_get_io_status"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIOStatus NG-1 End"));
		return -1;
	}	
	
	if ( NULL != Str )
	{
		int IoID = 0;
		CString Status;
		CString stringbuff;			
	
		IoID = PCE_M114GL_IO_RDY;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_ALM;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_PEL;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_NEL;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_ORG;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_DIR;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}

		//0x40->reserve
	
		IoID = PCE_M114GL_IO_PCS;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_ERC;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_EZ;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}

		//0x400 - Reserved
		IoID = PCE_M114GL_IO_LATCH;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_SD;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_INP;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCE_M114GL_IO_SVON;
		if ( (io_sts&IoID)!= 0x00 )
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
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIOStatus OK-2 End"));
	return io_sts;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::FreeRun(U16 Axis, F64 StrVel, F64 MaxVel, F64 Tacc)
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return true; }

	U8 JogDir = 0;//0->Neg, 1->Pos
	U32 uStarVel = 0;
	U32 uMaxVel = 0;
	F32 fTacc = 0.0;

	if ( StrVel < 0 ) { uStarVel = (U32)(-StrVel); }
	else { uStarVel = (U32)(StrVel); }
	if ( MaxVel < 0 ) 
	{
		JogDir = 0;
		uMaxVel = (U32)(-MaxVel);
	}
	else
	{
		JogDir = 1;
		uMaxVel = (U32)(MaxVel);
	}
	fTacc = (F32)(Tacc);

	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("FreeRun"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::FreeRun Start"));
	//_m114_tv_move(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc)
	ErrorStatus = _m114_tv_move(CardNo, CardAxis, JogDir, uStarVel, uMaxVel, fTacc);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("_m114_tv_move(axis, StrVel, MaxVel, Tacc"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::FreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::FreeRun OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::OneAxisMoveTo(U16 Axis, F64 Dist, F64 StrVel, F64 MaxVel,F64 Tacc,F64 Tdec, MOVE_CURVE_MODE VelCurve, MOVE_COORDINATE_MODE CoordMode, F64 SVacc, F64 SVdec)
{
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return false; }
	if ( CheckIsHomed(MotionAxisPtr) == false )	{	return false; }

	CString str;
	int nSign = FN_ENABLE;
	bool bTriangleCorrection = false;
	double Acc = 3000000;//加速度mm/s/s	
	double MinTimeAcc = 0.14;//最慢的時間為0.05 sec
	double MinTimeDec = 0.14;//最慢的時間為0.05 sec
	double PosTolerance = 1.0;
	ACC_TIME_ADJUST_MODE AccTimeAdjustMode=ACC_TIME_ADJUST_OFF;
	const TMotionParameter &MotionParam=m_MotionParameter;
	
	if ( CMotion_PCE_M114GL::WaitForDone(Axis) == false )
	{	return false; }

	str.Format(_T("OneAxisMoveTo[%.0f]"), Dist);
	CMotion_Basic::SaveMotionProcess(Axis, str, MSG_LEVEL_HIGH);
	//CMotion_Basic::SaveMotionProcess(Axis, _T("OneAxisMoveTo"), MSG_LEVEL_HIGH);

	nSign = MotionAxisPtr->GetSignPositive();
	Acc = MotionAxisPtr->GetAccelerationValue();
	MinTimeAcc = MotionAxisPtr->GetAccelerationTime();	
	MinTimeDec = MotionAxisPtr->GetDecelerationTime();		
	PosTolerance = MotionAxisPtr->GetPosTolerance();
	AccTimeAdjustMode = MotionAxisPtr->GetAccTimeAdjustMode();
	bTriangleCorrection = MotionAxisPtr->GetTriangleCorrection();
		
	MotionAxisPtr->SetCommandMaxVelocity(0);
	MotionAxisPtr->SetCommandAccelerationTime(0);
	const double LimitMin = MotionAxisPtr->GetLimitMin();
	const double LimitMax = MotionAxisPtr->GetLimitMax();
	switch ( Axis )
	{
	default: str=_T(""); break;
	case AXIS_X: str.Format(_T("CMotion_PCE_M114GL::OneAxisMoveTo(X, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec); break;
	case AXIS_Y: str.Format(_T("CMotion_PCE_M114GL::OneAxisMoveTo(Y, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec); break;
	case AXIS_Z: str.Format(_T("CMotion_PCE_M114GL::OneAxisMoveTo(Z, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec); break;
	}
	if ( str.GetLength() > 0 )
	{	CMotion_Basic::SaveMotionCurrentProcess(str); }

	if ( Dist>LimitMax || Dist<LimitMin )
	{
		switch ( Axis )
		{
		default: str=_T("Axis"); break;
		case AXIS_X:	str = _T("Error, X Axis Out of Stage Limit"); break;
		case AXIS_Y:	str = _T("Error, Y Axis Out of Stage Limit"); break;
		case AXIS_Z:	str = _T("Error, Z Axis Out of Stage Limit"); break;
		}
		str = CMotion_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.0f ~ %.0f, Pos=%.0f)"), str, LimitMin, LimitMax, Dist);
		return false;
	}	

	double Dis = Dist;
	double MaxVelocity = MaxVel;
	double Encode=0;
	this->GetEncode(Axis, Encode);
	if ( CoordMode == MOVE_COORDINATE_ABS)
	{	Dis = Dist - Encode;	}

	const bool bConvert=GetIsConvertSignPositive();//注意順序
	if ( true == bConvert )
	{
		if ( FN_DISABLE == nSign )
		{	Dist = -Dist;	}
	}

	if ( Dis < 0 ) { Dis = -Dis; }
	const double AdjustAccDist=MotionAxisPtr->GetAdjustAccclerationDist();
	Acc = AdjustAccValue(Acc, Dis, AdjustAccDist);

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
	if ( FN_DISABLE == m_MotionParameter.m_AccelerationAdjust )
	{
		Tacc = MinTimeAcc;
		Tdec = MinTimeDec;		
	}
	else
	{	MaxVel = MaxVelocity;	}
#ifndef MOTION_OBJ_DISABLE
	if ( Dis < PosTolerance ) { return true; }
	MotionAxisPtr->SetCommandMaxVelocity(MaxVel);
	MotionAxisPtr->SetCommandAccelerationTime(Tacc);

	//以下確認是避免算到負號時異常動作
	U32 uSVacc=0, uSVdec = 0;
	U32 uStrVel=0, uMaxVel=0;	
	if ( StrVel < 0 ) { uStrVel = (U32)(-StrVel); }
	else { uStrVel = (U32)(StrVel); }
	if ( MaxVel < 0 ) { uMaxVel = (U32)(-MaxVel); }
	else { uMaxVel = (U32)(MaxVel); }
	if ( SVacc < 0 ) { uSVacc = (U32)(-SVacc); }
	else { uSVacc = (U32)(SVacc); }
	if ( SVdec < 0 ) { uSVdec = (U32)(-SVdec); }
	else { uSVdec = (U32)(SVdec); }

	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	//Set Triangle Correction
	ErrorStatus = _m114_triangle_correction(CardNo, CardAxis, bTriangleCorrection);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CString str = "";
		GetErrorCodeText(ErrorStatus, str);
		this->m_ErrorString.Format(_T("Error, _m114_triangle_correction Fault.(%s)"), str);
		return false;
	}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo Start"));
	if ( VelCurve ==  MOVE_CURVE_T )
	{	
		CString strInfo;
		strInfo.Format(_T("T-Curve Dist(%.2f), StrVel(%d), MaxVel(%d), Tacc(%.4f), Tdec(%.4f)"), Dist, uStrVel, uMaxVel, Tacc, Tdec);
		if ( FN_ENABLE == m_MotionParameter.m_SaveMotionCardParam  )
		{	SaveMotionProcess(Axis, strInfo, MSG_LEVEL_HIGH); }
		if ( CoordMode == MOVE_COORDINATE_ABS )
		{	
			//_m114_start_ta_move(U16 SwitchCardNo, U16 AxisNo, I32 Pos, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
			ErrorStatus = _m114_start_ta_move(CardNo, CardAxis, Dist, uStrVel, uMaxVel, Tacc, Tdec);
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				CString str = "";
				GetErrorCodeText(ErrorStatus, str);
				this->m_ErrorString.Format(_T("Error, _m114_start_ta_move Fault.(%s)"), str);

				int axis = Axis;
				double dist = Dist;
				double startvel = StrVel;
				double maxvel = MaxVel;
				double acc = Tacc;
				double dec = Tdec;
				str.Format(_T("Axis: %d, Dist: %.2f,  StartVel: %.2f, MaxVel: %.2f, TAcc: %.2f, TDec: %.2f"), axis, dist, startvel, maxvel, acc, dec);

				CMotion_Basic::SaveMotionCurrentProcess(m_ErrorString);
				CMotion_Basic::SaveMotionCurrentProcess(str);
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo NG-1 End"));
				return false; 
			}
		}
		else if ( CoordMode == MOVE_COORDINATE_INS )
		{
			//_m114_start_tr_move(U16 SwitchCardNo, U16 AxisNo, I32 Dist, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec);
			ErrorStatus = _m114_start_tr_move(CardNo, CardAxis, Dist, uStrVel, uMaxVel, Tacc, Tdec);
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				this->m_ErrorString.Format(_T("Error, _m114_start_tr_move Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo NG-2 End"));
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Coordinate Mode Exception (%d)"), CoordMode);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo NG-3 End"));
			return false; 
		}		
	}
	else if ( VelCurve == MOVE_CURVE_S )
	{
		uSVacc = (uMaxVel-uStrVel)/3;
		uSVdec = (uMaxVel-uStrVel)/3;
		const double DifVel = (uMaxVel-uStrVel);
		const double Ratio = m_MotionParameter.m_SCurveVelRatio/100.0;
		uSVacc = (U32)(Ratio*0.5*DifVel);
		uSVdec = (U32)(Ratio*0.5*DifVel);
		//uSVacc = (U32)(Ratio*DifVel*Tdec/(Tacc+Tdec));
		//uSVdec = (U32)(Ratio*DifVel*Tacc/(Tacc+Tdec));		
		CString strInfo;
		strInfo.Format(_T("S-Curve Dist(%.2f), StrVel(%d), MaxVel(%d), Tacc(%.4f), Tdec(%.4f), SVacc(%d), SVdec(%d)"), Dist, uStrVel, uMaxVel, Tacc, Tdec, uSVacc, uSVdec);		
		if ( FN_ENABLE == m_MotionParameter.m_SaveMotionCardParam  )
		{	SaveMotionProcess(Axis, strInfo, MSG_LEVEL_HIGH); }
		if ( CoordMode == MOVE_COORDINATE_ABS )
		{
			//MaxVel = -MaxVel;
			//_m114_start_sa_move_ex(U16 SwitchCardNo, U16 AxisNo, I32 Pos, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec, U32 SVacc, U32 SVdec);
			ErrorStatus = _m114_start_sa_move_ex(CardNo, CardAxis, Dist, uStrVel, uMaxVel, Tacc, Tdec, uSVacc, uSVdec);
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				this->m_ErrorString.Format(_T("Error, _m114_start_sa_move_ex Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo NG-4 End"));
				return false; 
			}
		}
		else if ( CoordMode == MOVE_COORDINATE_INS )
		{
			//_m114_start_sr_move_ex(U16 SwitchCardNo, U16 AxisNo, I32 Dist, U32 StrVel, U32 MaxVel, F32 Tacc, F32 Tdec, U32 SVacc, U32 SVdec);
			ErrorStatus = _m114_start_sr_move_ex(CardNo, CardAxis, Dist, uStrVel, uMaxVel, Tacc, Tdec, uSVacc, uSVdec);
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				this->m_ErrorString.Format(_T("Error, _m114_start_sr_move_ex Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo NG-5 End"));
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Coordinate Mode Exception (%d)"), CoordMode);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo NG-6 End"));
			return false; 
		}		
	}
	else
	{
		this->m_ErrorString.Format(_T("Error, Velocity Curve Exception (%d)"), CoordMode);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo NG-7 End"));
		return false; 
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::OneAxisMoveTo OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::SetORGAll()
{
	if ( CheckInit() == false ) { return false; }
	if ( SetORG(AXIS_X) == false ) 
	{	return false; }	
	if ( SetORG(AXIS_Y) == false ) 
	{	return false; }	
	if ( SetORG(AXIS_Z) == false ) 
	{	return false; }	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::SetORG_Internal(U16 CardNo, U16 CardAxis)
{
#ifndef MOTION_OBJ_DISABLE
	I32 pos = 0;
	I32 cmd = 0;
	if ( CheckInit() == false ) { return false; }

	CString   Err;
	I16   ErrorStatus=0;	

	//_m114_set_position(U16 SwitchCardNo, U16 AxisNo, I32 pos)
	ErrorStatus = _m114_set_position(CardNo, CardAxis, pos);//feedback
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_position Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetORG NG-1 End"));
		return false; 
	}	
	//_m114_set_command(U16 SwitchCardNo, U16 AxisNo, I32 cmd)
	ErrorStatus = _m114_set_command(CardNo, CardAxis, cmd);//command pulse
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_command Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetORG NG-2 End"));
		return false; 
	}	
	//_m114_reset_error_counter(U16 SwitchCardNo, U16 AxisNo)
	ErrorStatus = _m114_reset_error_counter(CardNo, CardAxis);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_reset_error_counter Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetORG NG-3 End"));
		return false; 
	}	
	//_m114_set_target_pos(U16 SwitchCardNo, U16 AxisNo, I32 pos)
	ErrorStatus = _m114_set_target_pos(CardNo, CardAxis, pos);//end position
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_target_pos Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetORG NG-4 End"));
		return false; 
	}

	//I16 status= _M114GL_set_axis_counter (U16 SwitchCardNo, U16 AxisCounterNo, U16 CntMode, U16 CntDir, I32 SetValue)
	//ErrorStatus = _M114GL_set_axis_counter(CardNo, CardAxis, 0, 0, 0);//end position//No this Function
	//if ( CheckReturnOK(ErrorStatus) == false )
	//{
	//	this->m_ErrorString.Format(_T("Error, _M114GL_set_target_pos Fault"));
	//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetORG NG-4 End"));
	//	return false; 
	//}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetORG OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::SetORG(U16 Axis)
{	
#ifndef MOTION_OBJ_DISABLE
	I32 pos = 0;	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }	
	
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("SetORG"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetORG Start"));
	if ( SetORG_Internal(CardNo, CardAxis) == false )
	{	return false;	}

	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *ModelNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == ModelNodePtr ) { continue; }
		const U16 SlaveCardNo = ModelNodePtr->GetCardID();
		const U16 SlaveCardAxis = ModelNodePtr->GetNodeID();
		SetORG_Internal(SlaveCardNo, SlaveCardAxis);
	}
	
	MotionAxisPtr->SetCommandPos(pos);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetORG OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
LPCTSTR CMotion_PCE_M114GL::GetIOText(int IO)//取得IO的文字
{	
	switch ( IO )
	{
	case PCE_M114GL_IO_RDY:	m_MotionIOStats = _T("RDY pin input");	break;
	case PCE_M114GL_IO_ALM:	m_MotionIOStats = _T("Alarm signal");	break;
	case PCE_M114GL_IO_PEL:	m_MotionIOStats = _T("Positive Limit");	break;
	case PCE_M114GL_IO_NEL:	m_MotionIOStats = _T("Negative Limit");	break;
	case PCE_M114GL_IO_ORG:	m_MotionIOStats = _T("Origin signal");	break;
	case PCE_M114GL_IO_DIR:	m_MotionIOStats = _T("DIR output");	break;
	case PCE_M114GL_IO_PCS:	m_MotionIOStats = _T("PCS signal input");	break;
	case PCE_M114GL_IO_ERC:	m_MotionIOStats = _T("ERC pin output");	break;
	case PCE_M114GL_IO_EZ:	m_MotionIOStats = _T("EZ index signalt");	break;
	case PCE_M114GL_IO_CLR:	m_MotionIOStats = _T("CLR signalt");	break;
	case PCE_M114GL_IO_LATCH:	m_MotionIOStats = _T("Latch signal input");	break;
	case PCE_M114GL_IO_SD:	m_MotionIOStats = _T("Slow down signal input");	break;
	case PCE_M114GL_IO_INP:	m_MotionIOStats = _T("In-Position signal input");	break;
	case PCE_M114GL_IO_SVON:	m_MotionIOStats = _T("Servo-ON output status");	break;
	default:
		m_MotionIOStats = _T("No defined");
		break;
	}
	return m_MotionIOStats;
}
//----------------------------------------------------------------------------------//
int CMotion_PCE_M114GL::GetAxisStatus(U16 Axis, CString &str)//回傳該軸狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字
{
	if ( CheckInit() == false ) { return -1; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return -1; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return 0; }
	
#ifndef MOTION_OBJ_DISABLE
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	//_m114_motion_done(U16 SwitchCardNo, U16 AxisNo)
	I16 m_sts=_m114_motion_done(CardNo, CardAxis);	

	switch ( m_sts )
	{
	case 0:	str.Format(_T("Stop"));	break;
	case 1:	str.Format(_T("Reserved"));	break;
	case 2:	str.Format(_T("Wait CSTA (Synchronous start signal)"));	break;
	case 3:	str.Format(_T("Wait Internal sync. signal"));	break;
	case 4:	str.Format(_T("Reserved"));	break;
	case 5:	str.Format(_T("Wait ERC finished"));	break;
	case 6:	str.Format(_T("Wait DIR Change"));	break;
	case 7:	str.Format(_T("Backlash compensating"));	break;
	case 8:	str.Format(_T("Wait PA/PB"));	break;
	case 9:	str.Format(_T("In home special speed motion"));	break;
	case 10: str.Format(_T("In start velocity motion"));	break;
	case 11: str.Format(_T("In acceleration"));	break;
	case 12: str.Format(_T("In Max velocity motion"));	break;
	case 13: str.Format(_T("In deceleration"));	break;
	case 14: str.Format(_T("Wait INP"));	break;
	case 15: str.Format(_T("Reserved"));	break;
	default:
		this->m_ErrorString.Format(_T("Error, _m114_motion_done Exception"));
		str = m_ErrorString;
		return -1;
	}
	return (int)m_sts;
#else
	return 0;
#endif
	
}
//----------------------------------------------------------------------------------//
U16 CMotion_PCE_M114GL::GetCardNo() const
{
	return m_CardNo;
}
//----------------------------------------------------------------------------------//
void CMotion_PCE_M114GL::SetCardNo(U16 val)
{
	m_CardNo = val;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::BuildMotionNodeList()//建立運動點列表
{	
	if ( LoadMotionNodeList() == false ) { return false; }	
	std::vector<CMotionNode> &MotionNodeListRef=GetMotionNodeList();
	const size_t MotionNodeCount = MotionNodeListRef.size();
	if ( MotionNodeCount > 0 ) { return true; }	
	
	const int MaxAxis=GetUsedAxisCount();	
	for ( int i=0; i<MaxAxis; i++ )
	{
		const int AxisID=i;		
		const int CardNo = GetCardNo();		
		const int CardAxis = i;

		CMotionNode MotionNode;
		MotionNode.SetAxisID(AxisID);//軸號-AXIS_X, AXIS_Y, AXIS_Z;
		MotionNode.SetCardID(CardNo);//卡號
		MotionNode.SetNodeID(CardAxis);//通道或站號		

		MotionNode.SetGantryID(-1);//龍門邊號
		MotionNodeListRef.push_back(MotionNode); 			
	}	
	SaveMotionNodeList();	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::BuildMotionAxisList()//建立運動軸列表		
{
	ResetMotionAxisPtr();
	if ( BuildMotionNodeList() == false ) { return false; }	
	std::vector<CMotionAxis> &MotionAxisListRef=GetMotionAxisList();
	const std::vector<CMotionNode> &MotionNodeListRef=GetMotionNodeList();
	if ( BuildMotionAxisListFn(MotionNodeListRef, MotionAxisListRef) == false )
	{	return false; }
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::CheckReturnOK(I16 ret)
{
	if ( ERR_NoError != ret )
	{
		CString Err;
		GetErrorCodeText(ret, Err);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ReleaseMotionCard()
{	
	if ( GetInint() == false ) { return true; }	

	DisableSoftwareLimit(AXIS_X);
	DisableSoftwareLimit(AXIS_Y);
	DisableSoftwareLimit(AXIS_Z);

	size_t i=0;
	std::vector<CMotionAxis> &MotionAxisList=GetMotionAxisList();
	const size_t MotionAxisCount=MotionAxisList.size();
#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::ReleaseMotionCard Start"));
	U16 Axis=0;
	U16 CardNo=0;
	U16 CardAxis=0;
	I16 ErrorStatus = ERR_NoError;	
	for(i=0;i<MotionAxisCount;i++)
	{	
		CMotionAxis &MotionAxisRef=MotionAxisList[i];		
		if ( CheckAxisBypass(&MotionAxisRef) == true ) { continue; }

		Axis = MotionAxisRef.GetAxisID();
		CardNo = MotionAxisRef.GetCardID();
		CardAxis = MotionAxisRef.GetNodeID();

		//_m114_set_servo(U16 SwitchCardNo, U16 AxisNo, I16 on_off)
		ErrorStatus = _m114_set_servo(CardNo,CardAxis,0);	
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			this->m_ErrorString.Format(_T("Error, _m114_set_servo(CardNo,AxisNo,0) Fault"));
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::ReleaseMotionCard NG-1 End"));
			return false; 
		}
	}	
	ErrorStatus = _m114_close();
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_close Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::ReleaseMotionCard NG-2 End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::ReleaseMotionCard OK End"));
#endif//MOTION_OBJ_DISABLE

	SetInitialize(false);
	SetIsSupportGantry(false);
	for(i=0;i<MotionAxisCount;i++)
	{	
		CMotionAxis &MotionAxisRef=MotionAxisList[i];		
		if ( CheckAxisBypass(&MotionAxisRef) == true ) { continue; }
		MotionAxisRef.SetIsHomed(false);
		MotionAxisRef.SetIsEnabled(false);
	}	
	return true;
}
//----------------------------------------------------------------------------------//
CString CMotion_PCE_M114GL::GetCardTypeText(int val)
{
	CString str;
	switch ( val )
	{
	case CARD_PCI_M114:		str=_T("CARD_PCI_M114");	break;
	case CARD_PCI_M114GH:	str=_T("CARD_PCI_M114GH");	break;
	case CARD_PCI_M114GM:	str=_T("CARD_PCI_M114GM");	break;
	case CARD_PCI_M114GL:	str=_T("CARD_PCI_M114GL");	break;
	case CARD_PCE_M114:		str=_T("CARD_PCE_M114");	break;
	case CARD_PCE_M114GH:	str=_T("CARD_PCE_M114GH");	break;
	case CARD_PCE_M114GM:	str=_T("CARD_PCE_M114GM");	break;
	case CARD_PCE_M114GL:	str=_T("CARD_PCE_M114GL");	break;
	default:
		str = _T("CARD_NOT_DEFINED");
		break;
	}
	return str;	
}
//----------------------------------------------------------------------------------//
void CMotion_PCE_M114GL::SetTriggerParameter(double SP, double EP, double Start, double End, double Interval, int MaxRepeatCounts, int YMaxCounts, double YOffset, int ORGX, int ORGY, int ORGZ)
{
//	this->SetSystemDefaultMotionParameter();	
	this->m_TriggerStartPos = SP;			//移動的起點	(含加速距離)	//chia031
	this->m_TriggerEndPos = EP;				//移動的終點	(含減速距離)	
	this->m_TriggerFirstOnePos = Start;		//第一個觸發點
	this->m_TriggerLastOnePos = End;		//最後的觸發點
	this->m_TriggerInterval = Interval;		//觸發的間距
	this->m_TriggerMaxRepeatCounts = MaxRepeatCounts;
	this->m_TriggerYMaxCounts = YMaxCounts;
	this->m_ORGX = ORGX;
	this->m_ORGY = ORGY;
	this->m_ORGZ = ORGZ;
	//計算出每掃一條的觸發次數
	m_TriggerNTriggers=(int)((m_TriggerFirstOnePos-m_TriggerLastOnePos)/m_TriggerInterval);
	if ( m_TriggerNTriggers < 0 ) { m_TriggerNTriggers = -m_TriggerNTriggers; }	
	m_TriggerNTriggers = m_TriggerNTriggers+1;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ForwardTriggerProcess_JET7000S()//正向移動
{	
	this->m_TriggerStatus = MOTION_TRIGGER_FORWARD;	
#ifndef MOTION_OBJ_DISABLE	
	const int CurrentFOVY = 0;//AOIDataCollect.GetCurrentMovingScanIndex();
	const size_t MaxScanCount = this->m_ScanPathList.size();
	if ( CurrentFOVY<0 || CurrentFOVY>=MaxScanCount ) 
	{	return false; }

	//此範圍沒有檢測框	
	if ( m_ScanPathList[CurrentFOVY].m_FOVStartIDX_C < 0 ) 
	{
		this->m_TriggerRepeatCounts ++;	
		return true; 
	}
	const float EndOffset=0.5f;	
	const int PreTriggerStart = m_ScanPathList[CurrentFOVY].m_TriggerStartPosX;	//第一個觸發點
	const int PreTriggerEnd = m_ScanPathList[CurrentFOVY].m_TriggerEndPosX;		//最後一個觸發點
	const int TriggerStart = (int)(PreTriggerStart-m_MotionParameter.m_TriggerForwardOffset);		//第一個觸發點(含Trigger Offset)
	const int TriggerEnd = (int)(PreTriggerEnd-m_MotionParameter.m_TriggerForwardOffset+EndOffset);	//最後一個觸發點(含Trigger Offset)
	const int StartPosX = m_ScanPathList[CurrentFOVY].m_StartPosX;		//掃描起始點(含加速距離)
	const int EndPosX = m_ScanPathList[CurrentFOVY].m_EndPosX;			//掃描結束點(含減速距離)
	const int CurrentPosY = m_ScanPathList[CurrentFOVY].m_CurrentPosY;	//當前的Y Stage

	this->m_TriggerStartPos = StartPosX;
	this->m_TriggerEndPos = EndPosX;

	this->m_TriggerFirstOnePos = TriggerStart;
	this->m_TriggerLastOnePos = TriggerEnd;	


	//建立比對的陣列
	F64 *pArray=0;
	I16 ArraySize = (int)((TriggerEnd-TriggerStart)/m_TriggerInterval);
	//避免有正負號的問題
	if ( ArraySize < 0 ) 
	{	ArraySize = -ArraySize+1;	}
	else
	{	ArraySize = ArraySize+1;	}

	int LEDTriggerDis = TRIGGER_CCD_BETWEEN_LED;
	LEDTriggerDis = (int)(TRIGGER_CCD_BETWEEN_LED*0.001*m_MotionParameter.m_TriggerMaxVelocity);	//時間換算成距離
	ArraySize = ArraySize*2;	//兩倍Trigger數量  LED + CCD
	int i=0;
	int LEDindex = 0;

	bool IsTest = false;	
	if( IsTest == true )
	{ pArray = new F64[ArraySize-1]; }
	else
	{ pArray = new F64[ArraySize]; }

	for ( i=0; i<ArraySize; i+=2 )
	{	
		pArray[i] = (int)(TriggerStart + LEDindex*m_TriggerInterval)-LEDTriggerDis;	//LED Trigger
		if( IsTest == true && i == ArraySize-2)
		{ break; }
		pArray[i+1] = pArray[i] + LEDTriggerDis;									//CCD Trigger
		LEDindex++;
	}

	I16 ErrorStatus = ERR_NoError;	
	const U16 CardNo = GetCardNo();
	const int TriggerAxis = TRIGGER_AXIS;	
	const int CompareNo_LED = 0;
	const int CompareNo_CCD = 1;
	const int Dir = 1;//CCW:0, CW:1;
	const int CntMode = 0;//0:A/B phase, 1:CW/CCW
	const int CntDir  = 0;//0:Normal, 1:Inverse
	const int NElems = (int)((TriggerEnd-TriggerStart)/m_TriggerInterval)+1;
	double CurPos = 0;
	this->GetEncode(TRIGGER_AXIS, CurPos);
	const long AxisCounter = (long)(CurPos);
	const U16 TriggerInterval = (U16)(m_TriggerInterval);
	const I32 Start1 = (I32)(pArray[0]);
	const I32 Start2 = (I32)(pArray[1]);
	
	//LED、相機分別觸發
	//1. Disable Auto Trigger  //_m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
	ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_LED, 0);//LED Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_CCD, 0);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}		
	
	//2. Set Axis Count  
	//ErrorStatus = _M114GL_set_axis_counter(CardNo, TriggerAxis, CntMode, CntDir, AxisCounter);//Axis Counter
	//if ( CheckReturnOK(ErrorStatus) == false )
	//{
	//	this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
	//	return false;
	//}

	//設定觸發軸與比較軸, _m114_set_auto_compare_source(U16 SwitchCardNo, U16 AxisNo, U16 SrcAxisNo)	
	ErrorStatus = _m114_set_auto_compare_source(CardNo, CompareNo_LED, TriggerAxis);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_source(LED) Fault"));
		return false;
	}
	ErrorStatus = _m114_set_auto_compare_source(CardNo, CompareNo_CCD, TriggerAxis);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_source(CCD) Fault"));
		return false;
	}
	//座標同步化// _m114_set_auto_compare_encoder (U16 SwitchCardNo, U16 AxisNo, I32 EncPos)//
	ErrorStatus = _m114_set_auto_compare_encoder(CardNo, TriggerAxis, AxisCounter);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_encoder Fault"));
		return false;
	}	
	
	//3. Set Trigger Output pulse width  //_m114_set_auto_compare_trigger(U16 SwitchCardNo, U16 AxisNo, U16 Level, U16 Width);
	U16 Level = 0;//0->Normal Low, 1->Normal Hight
	ErrorStatus = _m114_set_auto_compare_trigger(CardNo, CompareNo_LED, Level, 184);//LED Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_trigger Fault"));
		return false;
	}
	ErrorStatus = _m114_set_auto_compare_trigger(CardNo, CompareNo_CCD, Level, 184);//Camera Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_trigger Fault"));
		return false;
	}

	//4.Set Auto Trigger parameter //_m114_set_auto_compare_function(U16 SwitchCardNo, U16 AxisNo, U8 Dir, I32 StrPos, I32 Interval, U16 TrgCnt);
	ErrorStatus = _m114_set_auto_compare_function(CardNo, CompareNo_LED, Dir, Start1, TriggerInterval, NElems);//LED Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_function Fault"));
		return false;
	}

	ErrorStatus = _m114_set_auto_compare_function(CardNo, CompareNo_CCD, Dir, Start2, TriggerInterval, NElems);//Camera Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_function Fault"));
		return false;
	}

	//5.Enable Auto Trigger
	//_m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
	ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_LED, 1);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_CCD, 1);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}	
	delete[] pArray; pArray=NULL;
	
	U32 uMaxVel = 0;
	U32 uStrVel = 0;
	F32 fTacc = 0.0f;
	F32 fTdec = 0.0f;
	if ( m_MotionParameter.m_TriggerStartVelocity < 0 ) { uStrVel = (U32)(-m_MotionParameter.m_TriggerStartVelocity); }
	else { uStrVel = (U32)(m_MotionParameter.m_TriggerStartVelocity); }
	if ( m_MotionParameter.m_TriggerMaxVelocity < 0 ) { uMaxVel = (U32)(-m_MotionParameter.m_TriggerMaxVelocity); }
	else { uMaxVel = (U32)(m_MotionParameter.m_TriggerMaxVelocity); }
	fTacc = (F32)(m_MotionParameter.m_TriggerAccelerationTime);
	fTdec = (F32)(m_MotionParameter.m_TriggerDecelerationTime);
	ErrorStatus = _m114_start_ta_move(CardNo, TriggerAxis,EndPosX, uStrVel ,uMaxVel, fTacc, fTdec);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_ta_move Fault in Trigger"));
		return false; 
	}
	
#endif//MOTION_OBJ_DISABLE
	this->m_TriggerRepeatCounts ++;	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::BackwardTriggerProcess_JET7000S()//逆向移動
{	
	this->m_TriggerStatus = MOTION_TRIGGER_BACKWARD;
#ifndef MOTION_OBJ_DISABLE	
	const int CurrentFOVY = 0;//AOIDataCollect.GetCurrentMovingScanIndex();
	const size_t MaxScanCount = this->m_ScanPathList.size();
	if ( CurrentFOVY<0 || CurrentFOVY>=MaxScanCount ) 
	{	return false; }

	//此範圍沒有檢測框
	if ( m_ScanPathList[CurrentFOVY].m_FOVStartIDX_C < 0 ) 
	{ 
		this->m_TriggerRepeatCounts ++;	
		return true; 
	}	
	const float EndOffset=-0.5f;
	const int PreTriggerStart = m_ScanPathList[CurrentFOVY].m_TriggerStartPosX;	//第一個觸發點
	const int PreTriggerEnd = m_ScanPathList[CurrentFOVY].m_TriggerEndPosX;		//最後一個觸發點
	const int TriggerStart = (int)(m_MotionParameter.m_TriggerBackwardOffset+PreTriggerEnd);			//第一個觸發點(含Trigger Offset)
	const int TriggerEnd  = (int)(m_MotionParameter.m_TriggerBackwardOffset+PreTriggerStart+EndOffset);	//最後一個觸發點(含Trigger Offset)
	const int StartPosX = m_ScanPathList[CurrentFOVY].m_StartPosX;				//掃描起始點(含加速距離)
	const int EndPosX = m_ScanPathList[CurrentFOVY].m_EndPosX;					//掃描結束點(含減速距離)

	this->m_TriggerStartPos = StartPosX;
	this->m_TriggerEndPos = EndPosX;
	this->m_TriggerFirstOnePos = TriggerStart;
	this->m_TriggerLastOnePos = TriggerEnd;

	//建立比對的陣列
	F64 *pArray=0;
	I16 ArraySize = (int)((TriggerEnd-TriggerStart)/m_TriggerInterval);	
	
	int LEDTriggerDis = TRIGGER_CCD_BETWEEN_LED;
	LEDTriggerDis = (int)(TRIGGER_CCD_BETWEEN_LED*0.001*m_MotionParameter.m_TriggerMaxVelocity);	//時間換算成距離	

	int i=0;
	int LEDindex = 0;
	
	//避免有正負號的問題
	if ( ArraySize < 0 )
	{
		ArraySize = -ArraySize+1;
		ArraySize = ArraySize*2;	//兩倍Trigger數量  LED + CCD
		pArray = new F64[ArraySize];
		for ( i=0; i<ArraySize; i+=2 )
		{
			pArray[i] = (int)(TriggerStart - LEDindex*m_TriggerInterval) + LEDTriggerDis;	//LED Trigger
			pArray[i+1] = pArray[i] - LEDTriggerDis;										//CCD Trigger
			LEDindex++;
		}
	}
	else
	{
		ArraySize = ArraySize+1;
		ArraySize = ArraySize*2;	//兩倍Trigger數量  LED + CCD
		pArray = new F64[ArraySize];
		for ( i=0; i<ArraySize; i+=2 )
		{	
			pArray[i] = (int)(TriggerStart + LEDindex*m_TriggerInterval) + LEDTriggerDis;	//LED Trigger
			pArray[i+1] = pArray[i] - LEDTriggerDis;										//CCD Trigger
			LEDindex++;	
		}
	}
	
	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = GetCardNo();
	const int TriggerAxis = TRIGGER_AXIS;
	const int CompareNo_LED = 0;
	const int CompareNo_CCD = 1;
	const int Dir = 0;//CCW:0, CW:1;
	const int CntMode = 0;//0:A/B phase, 1:CW/CCW
	const int CntDir  = 0;//0:Normal, 1:Inverse
	const int NElems = (-(int)((TriggerEnd-TriggerStart)/m_TriggerInterval))+1;
	double CurPos = 0;
	this->GetEncode(TRIGGER_AXIS, CurPos);
	const long AxisCounter = (long)(CurPos);
	const U16 TriggerInterval = (U16)(m_TriggerInterval);
	const I32 Start1 = (I32)(pArray[0]);
	const I32 Start2 = (I32)(pArray[1]);
		
	//LED、相機分別觸發
	//1. Disable Auto Trigger  //_m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
	ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_LED, 0);//LED Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_CCD, 0);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	
	//2. Set Axis Count
	//ErrorStatus = _M114GL_set_axis_counter(CardNo, TriggerAxis, CntMode, CntDir, AxisCounter);//Axis Counter
	//if ( CheckReturnOK(ErrorStatus) == false )
	//{
	//	this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
	//	return false;
	//}
	//設定觸發軸與比較軸, _m114_set_auto_compare_source(U16 SwitchCardNo, U16 AxisNo, U16 SrcAxisNo)	
	ErrorStatus = _m114_set_auto_compare_source(CardNo, CompareNo_LED, TriggerAxis);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_source(LED) Fault"));
		return false;
	}
	ErrorStatus = _m114_set_auto_compare_source(CardNo, CompareNo_CCD, TriggerAxis);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_source(CCD) Fault"));
		return false;
	}
	//座標同步化// _m114_set_auto_compare_encoder (U16 SwitchCardNo, U16 AxisNo, I32 EncPos)//
	ErrorStatus = _m114_set_auto_compare_encoder(CardNo, TriggerAxis, AxisCounter);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_encoder Fault"));
		return false;
	}	

	//3. Set Trigger Output pulse width
	//_m114_set_auto_compare_trigger(U16 SwitchCardNo, U16 AxisNo, U16 Level, U16 Width);
	U16 Level = 0;//0->Normal Low, 1->Normal Hight
	ErrorStatus = _m114_set_auto_compare_trigger(CardNo, CompareNo_LED, Level, 184);//LED Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_trigger Fault"));
		return false;
	}
	ErrorStatus = _m114_set_auto_compare_trigger(CardNo, CompareNo_CCD, Level, 184);//Camera Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_trigger Fault"));
		return false;
	}

	//4.Set Auto Trigger parameter  //_m114_set_auto_compare_function(U16 SwitchCardNo, U16 AxisNo, U8 Dir, I32 StrPos, I32 Interval, U16 TrgCnt);
	ErrorStatus = _m114_set_auto_compare_function(CardNo, CompareNo_LED, Dir, Start1, TriggerInterval, NElems);//LED Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_function Fault"));
		return false;
	}

	ErrorStatus = _m114_set_auto_compare_function(CardNo, CompareNo_CCD, Dir, Start2, TriggerInterval, NElems);//Camera Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_function Fault"));
		return false;
	}

	//5.Enable Auto Trigger
	//_m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
	ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_LED, 1);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_CCD, 1);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	delete[] pArray; pArray=NULL;

	U32 uMaxVel = 0;
	U32 uStrVel = 0;
	F32 fTacc = 0.0f;
	F32 fTdec = 0.0f;
	if ( m_MotionParameter.m_TriggerStartVelocity < 0 ) { uStrVel = (U32)(-m_MotionParameter.m_TriggerStartVelocity); }
	else { uStrVel = (U32)(m_MotionParameter.m_TriggerStartVelocity); }
	if ( m_MotionParameter.m_TriggerMaxVelocity < 0 ) { uMaxVel = (U32)(-m_MotionParameter.m_TriggerMaxVelocity); }
	else { uMaxVel = (U32)(m_MotionParameter.m_TriggerMaxVelocity); }
	fTacc = (F32)(m_MotionParameter.m_TriggerAccelerationTime);
	fTdec = (F32)(m_MotionParameter.m_TriggerDecelerationTime);
	ErrorStatus = _m114_start_ta_move(CardNo, TriggerAxis,StartPosX, uStrVel, uMaxVel, fTacc, fTdec);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_ta_move Fault in Trigger"));
		return false;
	}
	
#endif//MOTION_OBJ_DISABLE
	this->m_TriggerRepeatCounts ++;
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ForwardTriggerProcess()
{
	bool IsOK = false;
	IsOK = this->ForwardTriggerProcess_JET7000S();	
	if ( false == IsOK )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return IsOK;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::BackwardTriggerProcess()
{	
	bool IsOK = false;
	IsOK = this->BackwardTriggerProcess_JET7000S();
	if ( false == IsOK )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return IsOK;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::StopCompareTrigger(bool IsStop)//是否停止同步比較送外部觸發訊號
{	
#ifndef MOTION_OBJ_DISABLE
	const U16 CardNo = GetCardNo();
	const int CompareNo_LED = 0;
	const int CompareNo_CCD = 1;
	I16 ErrorStatus = ERR_NoError;
	CMotion_Basic::SaveMotionProcess(-1, _T("StopCompareTrigger"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::StopCompareTrigger Start"));
	if ( IsStop == true ) 
	{
		ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_LED, 0);//LED Trigger
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
			SetMotionExceptionCode(AOI_EXCEPTION_MOTION_EXEC_FUNC);
			return false;
		}
		ErrorStatus = _m114_start_auto_compare(CardNo, CompareNo_CCD, 0);//Camera Trigger
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
			SetMotionExceptionCode(AOI_EXCEPTION_MOTION_EXEC_FUNC);
			return false;
		}
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::StopCompareTrigger End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::EnableCompareTrigger(bool IsEnable)
{		
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = GetCardNo();
	CMotion_Basic::SaveMotionProcess(-1, _T("EnableCompareTrigger"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::EnableCompareTrigger Start"));
	if ( IsEnable == true )
	{	
		//ErrorStatus = _8164_set_auto_compare(TRIGGER_AXIS, TRIGGER_DEVICE); 
		/*
		ErrorStatus = _M114GL_start_auto_trigger(CardNo, TRIGGER_AXIS, TRIGGER_DEVICE); 
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			::strcpy(this->m_ErrorString, "Error, _M114GL_start_auto_trigger Fault");
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::EnableCompareTrigger NG-1 End"));
			return false; 
		}
		
		//I16 status= _M114GL_set_trigger_pulsewidth (U16 SwitchCardNo, U16 CompareNo, U16 PulseWidth)		
		*/
	}
	else
	{	
		this->StopCompareTrigger(true);
	}	//取消Trigger送出	
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_EXEC_FUNC);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::EnableCompareTrigger OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
void CMotion_PCE_M114GL::GetErrorCodeText(int status, CString &str)
{	
	switch ( status )
	{
	case ERR_NoError: str = _T("ERR_NoError"); break;
	case ERR_OpenCardFailed: str = _T("ERR_OpenCardFailed"); break;
	case ERR_MapMemoryFailed: str = _T("ERR_MapMemoryFailed"); break;
	case ERR_CardNumberRepeated: str = _T("ERR_CardNumberRepeated"); break;
	case ERR_CardNotExist: str = _T("ERR_CardNotExist"); break;
	case ERR_CardNotInitYet: str = _T("ERR_CardNotInitYet"); break;
	case ERR_LoadLibraryFailed: str = _T("ERR_LoadLibraryFailed"); break;
	case ERR_OpenMNetFailed: str = _T("ERR_OpenMNetFailed"); break;
	case ERR_InvalidCardNumber: str = _T("ERR_InvalidCardNumber"); break;
	case ERR_InvalidAxisNumber: str = _T("ERR_InvalidAxisNumber"); break;
	case ERR_InvalidParameter1: str = _T("ERR_InvalidParameter1"); break;
	case ERR_InvalidParameter2: str = _T("ERR_InvalidParameter2"); break;
	case ERR_InvalidParameter3: str = _T("ERR_InvalidParameter3"); break;
	case ERR_InvalidParameter4: str = _T("ERR_InvalidParameter4"); break;
	case ERR_InvalidParameter5: str = _T("ERR_InvalidParameter5"); break;
	case ERR_InvalidParameter6: str = _T("ERR_InvalidParameter6"); break;
	case ERR_InvalidParameter7: str = _T("ERR_InvalidParameter7"); break;
	case ERR_InvalidParameter8: str = _T("ERR_InvalidParameter8"); break;
	case ERR_InvalidParameter9: str = _T("ERR_InvalidParameter9"); break;
	case ERR_InvalidParameter10: str = _T("ERR_InvalidParameter10"); break;
	case ERR_InvalidParameter11: str = _T("ERR_InvalidParameter11"); break;
	case ERR_InvalidParameter12: str = _T("ERR_InvalidParameter12"); break;
	case ERR_SlowDownPointError: str = _T("ERR_SlowDownPointError"); break;
	case ERR_Err3PointsInput: str = _T("ERR_Err3PointsInput"); break;
	case ERR_GetCenterFailed: str = _T("ERR_GetCenterFailed"); break;
	case ERR_CompareBufferFull: str = _T("ERR_CompareBufferFull"); break;
	case ERR_AxisNotStoppedYet: str = _T("ERR_AxisNotStoppedYet"); break;
	case ERR_ObsoleteFunction: str = _T("ERR_ObsoleteFunction"); break;
	case ERR_GetSecureIdFailed: str = _T("ERR_GetSecureIdFailed"); break;
	case ERR_GenAesKeyFailed: str = _T("ERR_GenAesKeyFailed"); break;
	case ERR_NotSupported: str = _T("ERR_NotSupported"); break;
	case ERR_SetAutoCmpFifoFailed: str = _T("ERR_SetAutoCmpFifoFailed"); break;
	case ERR_InvalidBoardID: str = _T("ERR_InvalidBoardID"); break;
	default:
		str.Format(_T("ERR_NotDefeined (%d)"), status);
		break;
	}	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::PreInitMotion()//預先初始化
{
//	CMotion_Basic::PreInitMotion();
	SetMotionName(_T("PCE-M114GL"));
	SetMotionCardType(MOTION_CARD_PCE_M114GL);
	m_MaxAxisCount = MAX_MOTION_AXIS;
	m_UsedAxisCount = MAX_USED_AXIS;
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::InitialMotion()//初始化
{	
	bool IsOK = true;

#ifdef MOTION_OBJ_DISABLE
	IsOK = true;
	SetInitialize(true);
#else
	CMotion_Basic::SaveMotionProcess(-1, _T("InitialMotion"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::InitialMotion Start"));
	//IsOK = this->InitializeForJET7300();	
	//IsOK = this->InitializeForJET6500();		
	IsOK = this->InitializeForJET8000();
	//IsOK = this->InitializeForJET7500TB();
	if ( IsOK == true )
	{	SaveMotionCardType();	}	
	if ( IsOK == false ) 
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_CONNECT);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::InitialMotion NG End"));	
	}
	else
	{	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::InitialMotion OK End"));	}
#endif//MOTION_OBJ_DISABLE	
	return IsOK;	
	
}
//----------------------------------------------------------------------------------//	
bool CMotion_PCE_M114GL::ReleaseMotion()
{	
	if ( ReleaseMotionCard() == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_RELEASE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecHome(int Axis)
{
#ifndef MOTION_OBJ_DISABLE	
	CString str;
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	CString AxisS = MotionAxisPtr->GetAxisName();

	str.Format(_T("Home %s"), AxisS);	
	CMotion_Basic::SaveMotionProcess(Axis, _T("Home"), MSG_LEVEL_HIGH);	
	AOIDataCollect.SaveLogMessage(str);		
	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home Start"));

	//if ( m_MotionParameter.m_UsingMotionSoftwareLimit == FN_ENABLE )
	{	this->DisableSoftwareLimit(Axis);	}

	if ( this->GetIsEnable(Axis) == false ) 
	{ 
		this->m_ErrorString.Format(_T("%s axis not enabled, Please Enable it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-1 End"));
		return false; 
	}
	if ( this->GetIsAlarm(Axis) == true )
	{
		this->m_ErrorString.Format(_T("%s axis was alarm, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-2 End"));
		return false; 
	}
	MotionAxisPtr->SetIsHomed(false);	
	
	U8     HomeDir=1;//0:neg, 1:post
	int    SignPositive=0;
	double HomeVelocity = 0;
	double ORGOffset = 10000;
	double ORGVelocity  = 50000;
	double HomePreMove = HOME_PRE_MOVE_DIST;
	const bool bConvert=GetIsConvertSignPositive();
	switch ( Axis )
	{
	case AXIS_X:	str.Format(_T("Home X axis"));	break;
	case AXIS_Y:	str.Format(_T("Home Y axis"));	break;
	case AXIS_Z:	str.Format(_T("Home Z axis"));	break;
	}
	I16   ErrorStatus=0;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();

	ORGOffset = MotionAxisPtr->GetHomeOrgOffset();
	ORGVelocity = MotionAxisPtr->GetHomeVelocity();
	HomePreMove = MotionAxisPtr->GetHomePreMoveDis();		
	SignPositive = MotionAxisPtr->GetSignPositive();		
	HomeVelocity = ::fabs(ORGVelocity);

	//注意歸零的方向
	HomeDir = 0;//目前都是負向搜尋
	if ( true==bConvert && FN_DISABLE==SignPositive )
	{	
		ORGOffset = -ORGOffset;	
		HomePreMove = -HomePreMove;
	}

	//晚後移動, 避免撞機(低速模式)	
	U32 uStrVel = 0;
	U32 uMaxVel = 0;	
	const F32 fTacc = (F32)(HOME_ACCELERATE_TIME); 
	const F32 fTdec = (F32)(HOME_ACCELERATE_TIME); 
	
	uStrVel = 0;
	if ( HOME_MAX_VELOCITY < 0 ) { uMaxVel =(U32)(-HOME_MAX_VELOCITY); }
	else { uMaxVel = (U32)(HOME_MAX_VELOCITY); }
	uStrVel = uMaxVel/10;
	ErrorStatus = _m114_start_tr_move(CardNo, CardAxis, HomePreMove, uMaxVel, uMaxVel, fTacc, fTdec);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_tr_move Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-3 End"));
		return false; 
	}	
	//----------等待歸零結束---------------------------------------
	if ( this->WaitForDone(Axis) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-4 End"));
		return false; 
	}
	
	if ( this->GetIsEmergencyOn(Axis) == true )
	{ 
		this->m_ErrorString.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-5 End"));
		return false; 
	}
	::Sleep(250);

	//高速模式的歸零, 但是不準	
	double HomeAccTime = 0.25;

	//低速模式的歸零, 準但很慢	
	//HomeAccTime = 1.10;	
	//I16 home_mode = 0;	
	//I16 ell_logic = 0;
	//I16 ell_mode = 1;//slow down(on:1, off:0);	
	//ErrorStatus = _m114_set_sd(CardNo,CardAxis, 0, 1, 0, 1);//_m114_set_sd(U16 SwitchCardNo, U16 AxisNo, I16 enable, I16 sd_logic, I16 sd_latch, I16 sd_mode);
	//ErrorStatus = _m114_set_ell(CardNo, CardAxis, ell_logic, ell_mode);//啟動SlowDown
	//ErrorStatus = _m114_set_home_config(CardNo, CardAxis, home_mode, 0, 0, 0, 0);//無效

	//_m114_home_move(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc);	
	ErrorStatus = _m114_home_move(CardNo, CardAxis, HomeDir, 0, HomeVelocity, HomeAccTime);
	//ErrorStatus = _m114_home_search(CardNo, CardAxis, HomeDir, 0, HomeVelocity, HomeAccTime, 0);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{	
		//ErrorStatus = _M114GL_set_el(CardNo,AxisNo,0);//關閉SlowDown
		this->m_ErrorString.Format(_T("Error, _m114_home_move Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-9 End"));
		return false; 
	}

	//----------等待歸零結束---------------------------------------
	if ( this->WaitForDone(Axis) == false ) 
	{ 
		//ErrorStatus = _M114GL_set_el(CardNo,AxisNo,0);//關閉SlowDown
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-10 End"));
		return false; 
	}
	
	if ( this->GetIsEmergencyOn(Axis) == true )
	{ 
		this->m_ErrorString.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-6 End"));
		return false; 
	}

	//----------重設原點位置----------------------------------------//
	::Sleep(50);

	//ErrorStatus = _M114GL_set_el(CardNo,CardAxis,0);//關閉SlowDown
	if ( this->SetORG(Axis) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-13 End"));
		return false; 
	}

	if ( this->GetIsEmergencyOn(Axis) == true )
	{ 
		this->m_ErrorString.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-13 End"));
		return false; 
	}

	double Vel = HOME_MAX_VELOCITY;
	//double Vel = ORGVelocity;
	if ( Vel < 0 ) { uMaxVel = (U32)(-Vel); }		
	else { uMaxVel = (U32)(Vel); }
	ErrorStatus = _m114_start_tr_move(CardNo, CardAxis, ORGOffset, 0, uMaxVel, fTacc, fTdec);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-14 End"));
		return false; 
	}

	if ( this->WaitForDone(Axis) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-15 End"));
		return false; 
	}

	if ( this->GetIsEmergencyOn(Axis) == true )
	{ 
		this->m_ErrorString.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-7 End"));
		return false; 
	}

	if ( this->SetORG(Axis) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home NG-16 End"));
		return false; 
	}

	if ( m_MotionParameter.m_UsingMotionSoftwareLimit == FN_ENABLE )
	{
		const double Max = MotionAxisPtr->GetLimitMax();
		const double Min = MotionAxisPtr->GetLimitMin();
		SetSoftwareLimit(Axis, Min, Max, false);		
		EnableSoftwareLimit(Axis);
	}
	MotionAxisPtr->SetIsHomed(true);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Home OK End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::Home(int Axis)
{
	if ( ExecHome(Axis) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_HOME);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::Enable_Internal(U16 CardNo, U16 CardAxis)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	I16 ErrorStatus = _m114_set_servo(CardNo, CardAxis,1);
	if ( CheckReturnOK(ErrorStatus) == false ) //0->turn on
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_servo(CardNo, axis,1)"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Enable NG End"));
		return false;
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecEnable(int Axis)
{	
#ifndef MOTION_OBJ_DISABLE
	CString Err;	
	const I16 axis = Axis;
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("Enable"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Enable Start"));	
	if ( Enable_Internal(CardNo, CardAxis) == false )
	{	return false; }	
	MotionAxisPtr->SetIsEnabled(true);

	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *ModelNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == ModelNodePtr ) { continue; }
		const U16 SlaveCardNo = ModelNodePtr->GetCardID();
		const U16 SlaveCardAxis = ModelNodePtr->GetNodeID();
		Enable_Internal(SlaveCardNo, SlaveCardAxis);
		ModelNodePtr->SetIsEnabled(true);
	}		
	
	const bool bHomed = MotionAxisPtr->GetIsHomed();
	if ( true == bHomed )
	{
		if ( ResetMotionCardCommandPos(Axis) == false )
		{	MotionAxisPtr->SetIsHomed(false);	}
	}		
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Enable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::Enable(int Axis)
{
	if ( ExecEnable(Axis) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_ENABLE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::Disable_Internal(U16 CardNo, U16 CardAxis)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	I16 ErrorStatus = _m114_set_servo(CardNo, CardAxis,0);
	if ( CheckReturnOK(ErrorStatus) == false ) //1->turn off
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_servo(CardNo, axis,0)"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Disable NG End"));
		return false;
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecDisable(int Axis)
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("Disable"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Disable Start"));	
	if ( Disable_Internal(CardNo, CardAxis) == false )
	{	return false; }
	MotionAxisPtr->SetIsEnabled(false);

	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *ModelNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == ModelNodePtr ) { continue; }
		const U16 SlaveCardNo = ModelNodePtr->GetCardID();
		const U16 SlaveCardAxis = ModelNodePtr->GetNodeID();
		Disable_Internal(SlaveCardNo, SlaveCardAxis);
		ModelNodePtr->SetIsEnabled(false);
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::Disable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::Disable(int Axis)
{
	if ( ExecDisable(Axis) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_DISABLE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecWaitForDone(int Axis, int MaxPreCounts)	
{		
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false;	}	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	CString AxisS = MotionAxisPtr->GetAxisName();
	
	CString   str;
	int       IoID = 0;
	U16       io_sts = 0;
	DWORD     TickCnt1=0;
	DWORD     TickCnt2=0;
	DWORD     TickCntD=0;	
	I16 ErrorStatus = ERR_NoError;
	//const int SeelpTime = 1;//10ms
	const DWORD MaxTickCount=20000;//20 sec
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	const int SeelpTime = GetMotionParameter().m_WaitForDoneDwellTime;	
	CMotion_Basic::SaveMotionProcess(Axis, _T("WaitForDone"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::WaitForDone Start"));	

	//if ( this->GetIsEnable(Axis) == false )
	//{	return true; }

	TickCnt1 = ::GetTickCount();
	while ( _m114_motion_done(CardNo, CardAxis) != 0 )
	{
		TickCnt2 = ::GetTickCount();
		TickCntD = TickCnt2-TickCnt1;
		if ( TickCntD > MaxTickCount )
		{
			str = _T("Error, wait for done too long");
			str = LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::WaitForDone NG End"));
			return false; 
		}

		_m114_get_io_status(CardNo, CardAxis, &io_sts);
		IoID = PCE_M114GL_IO_ALM;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			str = _T("Error, motion is alarm");
			//str = LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::WaitForDone NG End"));
			return false;
		}
		IoID = PCE_M114GL_IO_EMG;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			str = _T("Error, motion is EMG");
			//str = LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::WaitForDone NG End"));
			return false;
		}

		if ( SeelpTime > 0 )
		{	::Sleep(SeelpTime); }
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::WaitForDone OK End"));
	return true;	
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::WaitForDone(int Axis, int MaxPreCounts)
{
	if ( ExecWaitForDone(Axis, MaxPreCounts) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE_DONE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecTriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity)//觸發移動至哪裡
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }	

	F64 StrVel=0, MaxVel=300000, Tacc=0.1, Tdec=0.1;	
	StrVel = 0;
	MaxVel = m_MotionParameter.m_TriggerMaxVelocity;
	Tacc = MotionAxisPtr->GetAccelerationTime();
	Tdec = MotionAxisPtr->GetDecelerationTime();
//	return this->OneAxisMoveTo(Axis, Dist, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_S, MOVE_COORDINATE_ABS);
	return this->OneAxisMoveTo(Axis, TargetPos, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_T, MOVE_COORDINATE_ABS);
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::TriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity)//觸發移動至哪裡
{
	if ( ExecTriggerMoveTo(Axis, TargetPos, IsModifyVelocity) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecMoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }	

	MOVE_CURVE_MODE MoveCurveMode=MOVE_CURVE_T;
	F64 StrVel=0, MaxVel=300000, Tacc=0.1, Tdec=0.1;

	Tacc = MotionAxisPtr->GetAccelerationTime();
	Tdec = MotionAxisPtr->GetDecelerationTime();
	MoveCurveMode = MotionAxisPtr->GetMovingCurveMode();
	MaxVel = GetMotionAxisVelocity(MotionAxisPtr, MovingMode);	

	MotionAxisPtr->SetCommandPos(TargetPos);
//	return this->OneAxisMoveTo(Axis, Dist, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_S, MOVE_COORDINATE_ABS);
	return this->OneAxisMoveTo(Axis, TargetPos, StrVel, MaxVel, Tacc, Tdec, MoveCurveMode, MOVE_COORDINATE_ABS);
#endif
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::MoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)
{
	if ( ExecMoveTo(Axis, TargetPos, MovingMode, IsModifyVelocity) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::XYMoveTo(double PosX, double PosY, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//2軸移動，但非同動唷
{
	if ( ExecXYMoveTo(PosX, PosY, Offline, MovingMode, IsModifyVelocity) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecXYMoveTo(double PosX, double PosY, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//2軸移動，但非同動唷
{
	if ( CheckNeedMoveXY(PosX, PosY, Offline) == false )//與之前的移動位置相同
	{	return true; }

	SetCommandOffline(Offline);
	CMotionAxis *MotionAxisPtrX=GetMotionAxisPtr(AXIS_X);
	CMotionAxis *MotionAxisPtrY=GetMotionAxisPtr(AXIS_Y);	
	if ( CheckMotionAxisPtr(MotionAxisPtrX) == true )
	{	MotionAxisPtrX->SetCommandPos(PosX); }
	if ( CheckMotionAxisPtr(MotionAxisPtrY) == true )
	{	MotionAxisPtrY->SetCommandPos(PosY); }
	if ( true == Offline )
	{	return true; }
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	const double ToleranceX=GetPosToleranceX();//誤差值
	const double ToleranceY=GetPosToleranceY();//誤差值
	const double ToleranceZ=GetPosToleranceZ();//誤差值
	double EncodeX=0;
	double EncodeY=0;
	double EncodeZ=0;
	//GetCurrentPos(EncodeX, EncodeY, EncodeZ);//已校正的座標
	GetEncode(AXIS_X, EncodeX);//要取未校正的座標
	GetEncode(AXIS_Y, EncodeY);//要取未校正的座標	

	double CaliX=0, CaliY=0;
	StageToCali(PosX, PosY, CaliX, CaliY);
	//XY座標會因為另一軌位置不同而變更, 以最後軸座標來比較
	if ( fabs(CaliX-EncodeX) > ToleranceX ) 
	{	
		if ( this->MoveTo(AXIS_X, CaliX, MovingMode, IsModifyVelocity) == false )
		{	return false; }		
		if ( CheckMotionAxisPtr(MotionAxisPtrX) == true )
		{	MotionAxisPtrX->SetCommandPos(PosX); }
	}

	if ( fabs(CaliY-EncodeY) > ToleranceY ) 
	{
		if ( this->MoveTo(AXIS_Y, CaliY, MovingMode, IsModifyVelocity) == false )
		{	return false; }
		if ( CheckMotionAxisPtr(MotionAxisPtrY) == true )
		{	MotionAxisPtrY->SetCommandPos(PosY);	}
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecXYZMoveTo(double PosX, double PosY, double PosZ, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//3軸移動，但非同動唷
{	
	if ( CheckNeedMoveXYZ(PosX, PosY, PosZ, Offline) == false )//與之前的移動位置相同
	{	return true; }

	SetCommandOffline(Offline);
	CMotionAxis *MotionAxisPtrX=GetMotionAxisPtr(AXIS_X);
	CMotionAxis *MotionAxisPtrY=GetMotionAxisPtr(AXIS_Y);	
	CMotionAxis *MotionAxisPtrZ=GetMotionAxisPtr(AXIS_Z);
	if ( CheckMotionAxisPtr(MotionAxisPtrX) == true )
	{	MotionAxisPtrX->SetCommandPos(PosX); }
	if ( CheckMotionAxisPtr(MotionAxisPtrY) == true )
	{	MotionAxisPtrY->SetCommandPos(PosY); }
	if ( CheckMotionAxisPtr(MotionAxisPtrZ) == true )
	{	MotionAxisPtrZ->SetCommandPos(PosZ);	}	
	if ( true == Offline )
	{	return true; }

#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	const double ToleranceX=GetPosToleranceX();//誤差值
	const double ToleranceY=GetPosToleranceY();//誤差值
	const double ToleranceZ=GetPosToleranceZ();//誤差值
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
		if ( this->MoveTo(AXIS_X, CaliX, MovingMode, IsModifyVelocity) == false )
		{	return false; }		
		if ( CheckMotionAxisPtr(MotionAxisPtrX) == true )
		{	MotionAxisPtrX->SetCommandPos(PosX); }
	}

	if ( fabs(CaliY-EncodeY) > ToleranceY ) 
	{
		if ( this->MoveTo(AXIS_Y, CaliY, MovingMode, IsModifyVelocity) == false )
		{	return false; }
		if ( CheckMotionAxisPtr(MotionAxisPtrY) == true )
		{	MotionAxisPtrY->SetCommandPos(PosY);	}
	}
	
	if ( fabs(PosZ-EncodeZ) > ToleranceZ )
	{	
		if ( this->MoveTo(AXIS_Z, PosZ, MovingMode, IsModifyVelocity) == false )
		{	return false; }
		if ( CheckMotionAxisPtr(MotionAxisPtrZ) == true )
		{	MotionAxisPtrZ->SetCommandPos(PosZ); }
	}
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::XYZMoveTo(double PosX, double PosY, double PosZ, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//3軸移動，但非同動唷
{
	if ( ExecXYZMoveTo(PosX, PosY, PosZ, Offline, MovingMode, IsModifyVelocity) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetEncode(int Axis, double &Encode, bool offline)//取得該軸的光學尺座標
{
	Encode = GetCommandPos(Axis);
#ifndef MOTION_OBJ_DISABLE
	I32 encode = 0;
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }	
	if ( true == offline )	{	return true;	}

	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetEncode Start"));
	//_m114_get_position(U16 SwitchCardNo, U16 AxisNo, I32 *pos)
	ErrorStatus = _m114_get_position(CardNo, CardAxis, &encode);
	Encode = encode;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = MotionAxisPtr->GetSignPositive();	
		if ( FN_DISABLE == nSign )
		{	Encode = -Encode;	}
	}

	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_get_position Fault"));
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_READ_ENCODE);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetEncode NG End"));
		return false; 
	}
//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetEncode OK End"));
#else
	Encode = GetCommandPos(Axis);	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetCurrentPos(double &X, double &Y, bool offline)//取得目前機台位置
{
	double PosZ=0;
	return GetCurrentPos(X, Y, PosZ, offline);
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetCurrentPos(double &X, double &Y, double &Z, bool offline)//取得目前機台位置
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	if ( true == offline )
	{
		GetCommandPos(X, Y, Z);
		return true;
	}
	F64 EncodeX=0;
	F64 EncodeY=0;
	F64 EncodeZ=0;
	if ( this->GetEncode(AXIS_X, EncodeX, offline) == false ) { return false; }
	if ( this->GetEncode(AXIS_Y, EncodeY, offline) == false ) { return false; }
	if ( this->GetEncode(AXIS_Z, EncodeZ, offline) == false ) { return false; }
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
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetCurrentRawPos(double &X, double &Y, double &Z, bool offline)//取得目前機台原始位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	if ( true == offline )
	{
		GetCommandPos(X, Y, Z);
		return true;
	}
	F64 EncodeX=0;
	F64 EncodeY=0;
	F64 EncodeZ=0;
	if ( this->GetEncode(AXIS_X, EncodeX, offline) == false ) { return false; }
	if ( this->GetEncode(AXIS_Y, EncodeY, offline) == false ) { return false; }
	if ( this->GetEncode(AXIS_Z, EncodeZ, offline) == false ) { return false; }
	X = EncodeX;
	Y = EncodeY;
	Z = EncodeZ;
#else
	GetCommandPos(X, Y, Z);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetIsEnable(int Axis)//該軸是否為Serve ON, 也就是有送電來積磁
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEnable Start"));
	
	int AxisIO = GetIOStatus(Axis, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEnable NG End"));
		return false; 
	}	

	if ( (AxisIO & PCE_M114GL_IO_SVON)!= 0x00 )
	{	
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEnable OK-1 End"));
		return true;	
	}
	else
	{	
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEnable OK-2 End"));
		return false;	
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEnable OK-3 End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetIsReady(int Axis)//Driver傳回該軸是否為RDY狀態
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsReady Start"));
	
	int AxisStatus = GetIOStatus(Axis, NULL);
	if ( AxisStatus == -1 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsReady NG End"));
		return false; 
	}
	if ( (AxisStatus & PCE_M114GL_IO_RDY) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsReady OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsReady OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ResetMotionCardCommandPos(int Axis)//重設軸控卡的命令位置
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }	

	I32 encode = 0;
	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	ErrorStatus = _m114_get_position(CardNo, CardAxis, &encode);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_get_position(CardNo, Axis, &encode)"));
		return false;
	}
	ErrorStatus = _m114_set_command(CardNo, CardAxis, (int)(encode));
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_command(CardNo, Axis, (int)(encode))"));
		return false;
	}
	double ComdPos=ConvertAxisPos(MotionAxisPtr, encode);
	MotionAxisPtr->SetCommandPos(ComdPos);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::DoFaultAck_Internal(U16 CardNo, U16 CardAxis)
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	
	I16 ErrorStatus = ERR_NoError;	
	//設定ERC為Hi	
	//_m114_set_ralm(U16 SwitchCardNo, U16 AxisNo, I16 on_off)
	ErrorStatus = _m114_set_ralm(CardNo, CardAxis, 1);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_ralm(CardNo, AXIS, 1, 0)"));
		return false;
	}
	::Sleep(500);
	//設定ERC為Low	
	ErrorStatus = _m114_set_ralm(CardNo, CardAxis, 0);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_ralm(CardNo, AXIS, 0, 0)"));
		return false;
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecDoFaultAck(int Axis)
{//Alarm Reset	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }		
	
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("DoFaultAck"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::DoFaultAck Start"));

	if ( DoFaultAck_Internal(CardNo, CardAxis) == false )
	{	return false; }

	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *ModelNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == ModelNodePtr ) { continue; }
		const U16 SlaveCardNo = ModelNodePtr->GetCardID();
		const U16 SlaveCardAxis = ModelNodePtr->GetNodeID();
		DoFaultAck_Internal(SlaveCardNo, SlaveCardAxis);		
	}

	const bool bHomed = MotionAxisPtr->GetIsHomed();
	if ( true == bHomed )
	{
		if ( ResetMotionCardCommandPos(Axis) == false )
		{	MotionAxisPtr->SetIsHomed(false);	}		 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::DoFaultAck OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::DoFaultAck(int Axis)
{
	if ( ExecDoFaultAck(Axis) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_FAULT_ACK);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
const TCHAR* CMotion_PCE_M114GL::GetAxisStatus(int Axis)
{
	m_MotionStatus = _T("");
	CMotion_PCE_M114GL::GetAxisStatus(Axis, m_MotionStatus);
	return this->m_MotionStatus;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetIsERCActive(int Axis)//Driver傳回該軸是否為ERC Active狀態
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsERCActive Start"));
	
	int AxisStatus = GetIOStatus(Axis, NULL);
	if ( AxisStatus == -1 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsERCActive NG-1 End"));
		return false; 
	}
	if ( (AxisStatus & PCE_M114GL_IO_ERC) != 0x00 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsERCActive OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsERCActive OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false; 	
}
//-----------------------------------------------------------------------
bool CMotion_PCE_M114GL::GetIsEmergencyOn(int Axis)//該軸是否收到急停訊號
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEmergencyOn Start"));
	
	int AxisStatus = GetIOStatus(Axis, NULL);
	if ( AxisStatus == -1 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEmergencyOn NG-1 End"));
		return false; 
	}
	if ( (AxisStatus & PCE_M114GL_IO_EMG) != 0x00 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEmergencyOn OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsEmergencyOn OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false; 	
}
//-----------------------------------------------------------------------
bool CMotion_PCE_M114GL::GetIsAlarm(int Axis)//Driver傳回該軸是否為Alarm狀態
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsAlarm Start"));
	
	int AxisStatus = GetIOStatus(Axis, NULL);
	if ( AxisStatus == -1 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsAlarm NG-1 End"));
		return false; 
	}
	if ( (AxisStatus & PCE_M114GL_IO_ALM) != 0x00 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsAlarm OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsAlarm OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false; 	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetIsInPosition(int Axis)//Driver傳回該軸是否為In Position
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsInPosition Start"));
	
	int AxisStatus = GetIOStatus(Axis, NULL);
	if ( AxisStatus == -1 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsInPosition NG-1 End"));
		return false; 
	}
	if ( (AxisStatus & PCE_M114GL_IO_INP) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsInPosition OK End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsInPosition OK End"));
#endif//MOTION_OBJ_DISABLE
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetIsNLimit(int Axis)//該軸的是否碰觸到副極限
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsNLimit Start"));
	
	int AxisStatus = GetIOStatus(Axis, NULL);
	if ( AxisStatus == -1 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsNLimit NG End"));
		return false; 
	}
	int Filter = PCE_M114GL_IO_NEL;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(Axis);		
		if ( FN_DISABLE == nSign )
		{	Filter = PCE_M114GL_IO_PEL;	}
	}
	if ( (AxisStatus & Filter) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsNLimit OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsNLimit OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetIsORG(int Axis)//該軸是否在原點位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsORG Start"));
	
	int AxisStatus = GetIOStatus(Axis, NULL);
	if ( AxisStatus == -1 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsORG NG End"));
		return false; 
	}
	if ( (AxisStatus & PCE_M114GL_IO_ORG) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsORG OK End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsORG OK End"));
#endif//MOTION_OBJ_DISABLE
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetIsPLimit(int Axis)//該軸的是否碰觸到正極限	
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsPLimit Start"));
	
	int AxisStatus = GetIOStatus(Axis, NULL);
	if ( AxisStatus == -1 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsPLimit NG End"));
		return false; 
	}
	int Filter = PCE_M114GL_IO_PEL;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(Axis);		
		if ( FN_DISABLE == nSign )
		{	Filter = PCE_M114GL_IO_NEL;	}
	}
	if ( (AxisStatus & Filter) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsPLimit OK End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::GetIsPLimit OK End"));
#endif//MOTION_OBJ_DISABLE
	return false;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::StartFreeRun(int Axis, bool Dir)//Dir +為正方向, -為負方向
{
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }	
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(Axis, _T("StartFreeRun"), MSG_LEVEL_HIGH);
	
	const int JogDirection = MotionAxisPtr->GetJogDirection();
	int MaxV = (int)(GetAxisFreeRunVelocity(MotionAxisPtr));
	
	MaxV = ::abs(MaxV);
	U8  uDir = 1;
	U32 uMaxVel = (U32)(MaxV);
	if ( Dir == true ) { uDir = 1; }
	else { uDir = 0; }

	MotionAxisPtr->SetFreeRunVel(uMaxVel);
	if ( JogDirection > 0 ) { uDir = uDir; }
	else 
	{
		if ( uDir == 1 ) { uDir = 0; }
		else { uDir = 1; }			
	}
#ifndef MOTION_OBJ_DISABLE
	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	U32 uStrVel = (U32)(m_MotionParameter.m_JogStartVelocity);
	const F32 Tacc = (F32)(m_MotionParameter.m_JogAccelerationTime);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::StartFreeRun Start"));	
	//_m114_tv_move(U16 SwitchCardNo, U16 AxisNo, U8 Dir, U32 StrVel, U32 MaxVel, F32 Tacc)
	ErrorStatus = _m114_tv_move(CardNo, CardAxis, uDir, uStrVel, uMaxVel, Tacc);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_tv_move Fault"));
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_JOG_FUNC);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::StartFreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::StartFreeRun OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::StopFreeRun(int Axis)//停止FreeRun
{
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }		

	CMotion_Basic::SaveMotionProcess(Axis, _T("StopFreeRun"), MSG_LEVEL_HIGH);
#ifndef MOTION_OBJ_DISABLE
	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	MotionAxisPtr->SetFreeRunVel(0);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::StopFreeRun Start"));	
	
	switch ( Axis )
	{
	case AXIS_Z:
		//_m114_imd_stop(U16 SwitchCardNo, U16 AxisNo);
		ErrorStatus = _m114_imd_stop(CardNo, CardAxis);
		break;
	default:
		//_m114_sd_stop(U16 SwitchCardNo, U16 AxisNo, F32 Tdec)
		ErrorStatus = _m114_sd_stop(CardNo, CardAxis, m_MotionParameter.m_JogDecelerationTime);
		break;
	}	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_sd_stop Fault"));
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_JOG_FUNC);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::StopFreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::StopFreeRun OK End"));	
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::SaveMotionParamInternal()//儲存運動內部參數
{
#ifndef MOTION_OBJ_DISABLE
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::GetMotionIsReady()//取得運動系統是否正常
{
	if ( CheckInit() == false ) { return false; }
	this->m_ErrorString = _T("");
	
	//先判斷是否Enable	
	U16 AxisNo = AXIS_X;
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
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ExecSetIsWaitForInPosition(int Axis, bool Iswait)//設定是否等待定位停止
{
	if ( CheckInit() == false ) { return false; }
	
#ifndef MOTION_OBJ_DISABLE
	bool Wait = Iswait;
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }		
	
	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("SetIsWaitForInPosition"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetIsWaitForInPosition Start"));
	int status = 1;	
	//_m114_set_inp(U16 SwitchCardNo, U16 AxisNo, I16 inp_enable, I16 inp_logic)
	if ( Wait == true )
	{	ErrorStatus = _m114_set_inp(CardNo, CardAxis, 1, status);	}	
	else
	{	ErrorStatus = _m114_set_inp(CardNo, CardAxis, 0, status);	}
	if ( CheckReturnOK(ErrorStatus) == false ) //0->Disable, 0->active at low
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_inp()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetIsWaitForInPosition NG End"));
		return false;
	}
	MotionAxisPtr->SetIsWaitForInp(Iswait);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetIsWaitForInPosition OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::SetIsWaitForInPosition(int Axis, bool Iswait)//設定是否等待定位停止
{
	if ( ExecSetIsWaitForInPosition(Axis, Iswait) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_INP_FUNC);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ConfigTriggerTable(bool IsReBuild)
{
#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::ConfigTriggerTable Start"));
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_SET_FUNC);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::ConfigTriggerTable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::ResetMotionDriver()//重新復歸運動的Driver
{	
#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::ResetMotionDriver Start"));
	DoFaultAck(AXIS_X);
	DoFaultAck(AXIS_Y);
	DoFaultAck(AXIS_Z);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::ResetMotionDriver OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
CString CMotion_PCE_M114GL::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("MOTION_PCE_M114GL");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::FireSingleTrigger()//送出單一觸發訊號(包含燈源與相機)
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	const double TriggerBetweenTime = TRIGGER_CCD_BETWEEN_LED;//ms
	//Total Time, On Time, NTriggers
	//Total Time:166us, On Time:40us, NTriggers=1
	//Total Time: TRIGGER_CCD_BETWEEN_LED*2
	//On Time:40
	//NTrigger = 2

	int TotalTime = (int)(TriggerBetweenTime*1000);
	int OnTime = 40;
	int NTrigger = 1;
	//_m114_force_trigger_output(U16 SwitchCardNo, U16 AxisNo);
	//--------------------------------------------------------------------------------------------------------------//	
	this->m_ErrorString.Format(_T("Error, PCE-M114GL does not support Fire single trigger mode"));
	return false;
	//--------------------------------------------------------------------------------------------------------------//	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::SetSoftwareLimit(int Axis, double Min, double Max, bool Auto)//設定軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }		

	I16 ErrorStatus = ERR_NoError;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("SetSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetSoftwareLimit Start"));

	I32 MinI = (I32)(Min);
	I32 MaxI = (I32)(Max);	

	if ( Auto == true ) 
	{
		if ( this->DisableSoftwareLimit(Axis) == false ) { return false; }
	}
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(Axis);
		if ( FN_DISABLE == nSign )
		{
			MinI = (I32)MIN(-Min, -Max);
			MaxI = (I32)MAX(-Min, -Max);
		}
	}
	//設定軟體極限
	//_m114_set_soft_limit(U16 SwitchCardNo, U16 AxisNo, I32 PLimit, I32 NLimit)
	ErrorStatus = _m114_set_soft_limit(CardNo, CardAxis,MaxI, MinI);
	if ( CheckReturnOK(ErrorStatus) == false ) //0->Disable, 0->active at low
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_soft_limit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetSoftwareLimit NG End"));
		return false;
	}	

	if ( Auto == true ) 
	{		
		if ( this->EnableSoftwareLimit(Axis) == false ) { return false; }
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::SetSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::EnableSoftwareLimit_Internal(U16 CardNo, U16 CardAxis)//啟用軟體極限
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	//開啟軟體極限, //0->INT, 1->Immediately stop, 2->Slow Down then stop, 3->Reserved
	//_m114_enable_soft_limit(U16 SwitchCardNo, U16 AxisNo, I16 Action)
	I16 ErrorStatus = _m114_enable_soft_limit(CardNo, CardAxis, 2);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_enable_soft_limit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::EnableSoftwareLimit NG End"));
		return false;
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::EnableSoftwareLimit(int Axis)//啟用軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("EnableSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::EnableSoftwareLimit Start"));
	if ( EnableSoftwareLimit_Internal(CardNo, CardAxis) == false )
	{	return false; }	

	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *ModelNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == ModelNodePtr ) { continue; }
		const U16 SlaveCardNo = ModelNodePtr->GetCardID();
		const U16 SlaveCardAxis = ModelNodePtr->GetNodeID();
		EnableSoftwareLimit_Internal(SlaveCardNo, SlaveCardAxis);		
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::EnableSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::DisableSoftwareLimit_Internal(U16 CardNo, U16 CardAxis)//關閉軟體極限
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	//關閉軟體極限
	//_m114_disable_soft_limit(U16 SwitchCardNo, U16 AxisNo);
	I16 ErrorStatus = _m114_disable_soft_limit(CardNo, CardAxis);
	if ( CheckReturnOK(ErrorStatus) == false ) //0->Disable, 0->active at low
	{
		this->m_ErrorString.Format(_T("Error, _m114_disable_soft_limit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::DisableSoftwareLimit NG End"));
		return false;
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCE_M114GL::DisableSoftwareLimit(int Axis)//關閉軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 CardAxis = MotionAxisPtr->GetNodeID();
	CMotion_Basic::SaveMotionProcess(Axis, _T("DisableSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::DisableSoftwareLimit Start"));

	if ( DisableSoftwareLimit_Internal(CardNo, CardAxis) == false )
	{	return false;	}

	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *ModelNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == ModelNodePtr ) { continue; }
		const U16 SlaveCardNo = ModelNodePtr->GetCardID();
		const U16 SlaveCardAxis = ModelNodePtr->GetNodeID();
		DisableSoftwareLimit_Internal(SlaveCardNo, SlaveCardAxis);		
	}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCE_M114GL::DisableSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
#endif//MOTION_DERIVE_MODE