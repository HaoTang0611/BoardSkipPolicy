// Motion_PCI_M114GL.cpp: implementation of the CMotion_PCI_M114GL class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Motion_PCI_M114GL.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#if MOTION_DERIVE_MODE == MOTION_PCI_M114GL
//-------------------------------------------------------------------------------------//
CMotion_PCI_M114GL  Motion_PCI_M114GL;
//-------------------------------------------------------------------------------------//
#define MAX_MOTION_AXIS        4//運動控制卡支援運動軸數
#define MAX_USED_AXIS          3//系統使用的運動軸數
//-------------------------------------------------------------------------------------//
#define HOME_START_VELOCITY    1000
#define HOME_MAX_VELOCITY     -50000
#define HOME_ACCELERATE_TIME   0.1
#define HOME_PRE_MOVE_DIST     100000
//-------------------------------------------------------------------------------------//
#define PCI_M114GL_IO_RDY      0x0001//RDY pin input
#define PCI_M114GL_IO_ALM      0x0002//Alam Signal
#define PCI_M114GL_IO_PEL      0x0004//Positive Limit Switch
#define PCI_M114GL_IO_NEL      0x0008//Negative Limit Switch
#define PCI_M114GL_IO_ORG      0x0010//Origin Switch
#define PCI_M114GL_IO_DIR      0x0020//Dir Output
#define PCI_M114GL_IO_EMG      0x0040//Emergency signal input
#define PCI_M114GL_IO_PCS      0x0080//PCS signal input
#define PCI_M114GL_IO_ERC      0x0100//ERC signal input
#define PCI_M114GL_IO_EZ       0x0200//Index signal
#define PCI_M114GL_IO_CLR      0x0400//Clear counter signal
#define PCI_M114GL_IO_LATCH    0x0800//Latch Signal Input
#define PCI_M114GL_IO_SD       0x1000//Slow Down signal input
#define PCI_M114GL_IO_INP      0x2000//In-Position signal input
#define PCI_M114GL_IO_SVON     0x4000//Servo-ON output status
#define PCI_M114GL_IO_RALM     0x8000//Reset Alarm output status
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CMotion_PCI_M114GL, CMotion_Basic)
//-------------------------------------------------------------------------------------//
CMotion_PCI_M114GL::CMotion_PCI_M114GL()
{
	CMotion_PCI_M114GL::PreInitMotion();
	SetInitialize(false);
	this->m_hEvent_PCI_M114GL = NULL;
	this->m_ErrorStatus = ERR_NoError;
	this->m_MotionStatus = _T("");
	this->m_ErrorString = _T("");
	this->m_IsJogMode = false;
	this->m_TriggerNTriggers = 0;
	SetIsSupportGantry(false);
}
//-------------------------------------------------------------------------------------//
CMotion_PCI_M114GL::~CMotion_PCI_M114GL()
{
	this->m_MotionCallbackHWnd = NULL;
	this->m_ErrorStatus = ERR_NoError;	
	m_IsMotionRelease = true;
	if ( this->ReleaseMotion() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
	}
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::SetCallBackFunction(void ( __stdcall *callbackAddr)(I16 IntAxisNoInCard))
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(-1, _T("SetCallBackFunction"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetCallBackFunction Start"));
	/*
	m_ErrorStatus = _8164_link_interrupt(0, callbackAddr);
	if ( m_ErrorStatus != ERR_NoError )
	{
		
		if ( m_ErrorStatus == ERR_LinkIntError )
		{	::strcpy(m_ErrorString, "Error, _8164_link_interrupt, ERR_LinkIntError"); }
		else if ( m_ErrorStatus == ERR_CardNoError )
		{::strcpy(m_ErrorString, "Error, _8164_link_interrupt, ERR_CardNoError");	}
		else if ( m_ErrorStatus == ERR_EventNotEnableYet )
		{::strcpy(m_ErrorString, "Error, _8164_link_interrupt, ERR_EventNotEnableYet"); }
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetCallBackFunction NG-1 End"));
		return false;
	}
	*/
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetCallBackFunction OK End"));
#endif
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::InitCallbackFunction()
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(-1, _T("InitCallbackFunction"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::InitCallbackFunction Start"));
	const int CardNo = this->m_CardNo;
	U32 factor=0x0000;
	int i=0;
	for ( i=0; i<MAX_USED_AXIS; i++ )
	{
		//m_ErrorStatus = _8164_set_int_factor(i, factor);//0->AxisNo, factor->Mask
		m_ErrorStatus = _M114GL_set_int_factor(CardNo, i, factor);//0->AxisNo, factor->Mask		
		if ( m_ErrorStatus != ERR_NoError )
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_int_factor(CardNo, %d, factor)"), i);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::InitCallbackFunction NG-1 End"));
			return false;
		}
	}		
	m_ErrorStatus =	_M114GL_int_enable(CardNo,&m_hEvent_PCI_M114GL);//0->CardNo, 
	if ( m_ErrorStatus != ERR_NoError )
	{
		m_ErrorString.Format(_T("Error, _M114GL_int_enable() fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::InitCallbackFunction NG-2 End"));
		return false;
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::InitCallbackFunction OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::InitializeForJET7300()//JET7300的初始化	
{	
	this->ReleaseMotionCard();
#ifndef MOTION_OBJ_DISABLE
	int i=0;
	this->m_CardNo = -1;
	I16 TotalCard=0;
	I16 AXIS=0;
	m_ErrorStatus = _M114GL_open(&TotalCard);
	if ( m_ErrorStatus != ERR_NoError )	//Card initial
	{
		m_ErrorString.Format(_T("Error, _M114GL_open(&TotalCard)"));
		return false;
	}

	if ( TotalCard == 0 )
	{
		m_ErrorString.Format(_T("Error, No PCI_M114_GL Inside IPC"));
		return false; 
	}

	//_M114GL_initial
	this->m_CardNo = -1;
	for ( i=0; i<8; i++ )
	{
		m_ErrorStatus = _M114GL_initial(i);
		if ( m_ErrorStatus == ERR_NoError )
		{
			this->m_CardNo = i;
			break;
		}		
	}
	if ( this->m_CardNo < 0 ) 
	{
		m_ErrorString.Format(_T("Error, _M114GL_initial(CardNO)"));
		return false;
	}

	for(AXIS=0;AXIS<MAX_MOTION_AXIS;AXIS++)
	{
		//set pulse command output mode
		//m_ErrorStatus = _8164_set_pls_outmode(AXIS,1);
		//m_ErrorStatus = _M114GL_set_pls_outmode(m_CardNo, AXIS, 3);
		if ( AXIS == AXIS_Z )
		{	m_ErrorStatus = _M114GL_set_pls_outmode(m_CardNo, AXIS, 7); }
		else
		{	m_ErrorStatus = _M114GL_set_pls_outmode(m_CardNo, AXIS, 7); }
		if ( m_ErrorStatus != ERR_NoError )	//1->OUT/DIROUT Rising edge, DIR+ is high level
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_pls_outmode(m_CardNo, AXIS, 7)"));
			return false;
		}

		//set encoder input mode
		m_ErrorStatus = _M114GL_set_pls_iptmode(m_CardNo, AXIS, 2, 0);
		if ( m_ErrorStatus != ERR_NoError )  //2->4X A/B, 1->Inverse direction
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_pls_iptmode(m_CardNo,AXIS,2,0)"));
			return false;
		}

		//set counter input source //注意一定要這樣設定		
		m_ErrorStatus = _M114GL_set_feedback_src(m_CardNo, AXIS,0);
		if ( m_ErrorStatus != ERR_NoError )  //0->External Feedback
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_feedback_src(m_CardNo,AXIS,0)"));
			return false;
		}
				
		//Set Home Config						
		//_m114GL_set_home_config(SelectCard= 3,SelectAxis= 0,home_mode= 0,org_logic= 1,ez_logic= 1,ez_count= 3,ERC_output= 0);
		//m_ErrorStatus = _M114GL_set_home_config(m_CardNo, AXIS, HOME_MODE, HOME_ORG_LOGIC, HOME_EZ_LOGIC, HOME_EZ_COUNT, HOME_ERC_OUT);
		//m_ErrorStatus = _M114GL_set_home_config(m_CardNo, AXIS, 0, 1, 1, 3, 0);
		m_ErrorStatus = _M114GL_set_home_config(m_CardNo, AXIS, 0, 1, 1, 3, 1);
		if ( m_ErrorStatus != ERR_NoError) 
		{
			this->m_ErrorString.Format(_T("Error, _M114GL_set_home_config Fault"));
			return false; 
		}

		//set alarm logic = Low				
		m_ErrorStatus = _M114GL_set_alm(m_CardNo,AXIS,0,0);
		if ( m_ErrorStatus != ERR_NoError ) //0->Active at Low, 0->Immediate stop
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_alm(m_CardNo,AXIS,0,0)"));
			return false;
		}

		//set end limit logic 
		m_ErrorStatus = _M114GL_set_ell(m_CardNo,AXIS,1);//1->high
		if ( m_ErrorStatus != ERR_NoError )  //0->Immediate stop
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_ell(m_CardNo,AXIS,1)"));
			return false;
		}		

		//set end limit active				
		m_ErrorStatus = _M114GL_set_el(m_CardNo,AXIS,0);
		if ( m_ErrorStatus != ERR_NoError )  //0->Immediate stop, 1->Slow Down
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_el(m_CardNo,AXIS,0)"));
			return false;
		}

		//set servo on logic = low for J2S 		
		m_ErrorStatus = _M114GL_set_servo(m_CardNo,AXIS,0);
		if ( m_ErrorStatus != ERR_NoError ) //1->turn off
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_servo(m_CardNo,AXIS,1)"));
			return false;
		}

		//set in-position logic			
		m_ErrorStatus = _M114GL_set_inp(m_CardNo,AXIS, 1, 1);	
		if ( m_ErrorStatus != ERR_NoError ) //0->Disable, 0->active at low
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_inp(m_CardNo,AXIS, 1, 1)"));
			return false;
		}

		//set move ratio (command/feedback)				
		m_ErrorStatus = _M114GL_set_move_ratio(m_CardNo,AXIS, 1); 
		if ( m_ErrorStatus != ERR_NoError )
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_move_ratio(m_CardNo,AXIS, 1)"));
			return false;
		}				
	}
	//set call back function
	U32 factor=0x0000;
	//_8164_set_int_factor(AXIS_X, factor);//0-AxisNO
	_M114GL_set_int_factor(m_CardNo, AXIS_X, factor);//0-AxisNO	
	_M114GL_set_int_factor(m_CardNo, AXIS_Y, factor);//1-AxisNO
	_M114GL_set_int_factor(m_CardNo, AXIS_Z, factor);//2-AxisNO

	this->DisableSoftwareLimit(AXIS_X);
	this->DisableSoftwareLimit(AXIS_Y);	
	this->DisableSoftwareLimit(AXIS_Z);	
#endif//MOTION_OBJ_DISABLE
	SetInitialize(true);
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::InitializeForJET6500()//JET6500的初始化
{	
	this->ReleaseMotionCard();
#ifndef MOTION_OBJ_DISABLE
	int i=0;
	this->m_CardNo = -1;
	I16 TotalCard=0;
	I16 AXIS=0;
	m_ErrorStatus = _M114GL_open(&TotalCard);
	if ( m_ErrorStatus != ERR_NoError )	//Card initial
	{
		m_ErrorString.Format(_T("Error, _M114GL_open(&TotalCard)"));
		return false;
	}

	if ( TotalCard == 0 )
	{
		m_ErrorString.Format(_T("Error, No PCI_M114_GL Inside IPC"));
		return false; 
	}

	//_M114GL_initial
	this->m_CardNo = -1;
	for ( i=0; i<8; i++ )
	{
		m_ErrorStatus = _M114GL_initial(i);
		if ( m_ErrorStatus == ERR_NoError )
		{
			this->m_CardNo = i;
			break;
		}		
	}
	if ( this->m_CardNo < 0 ) 
	{
		m_ErrorString.Format(_T("Error, _M114GL_initial(CardNO)"));
		return false;
	}

	for(AXIS=0;AXIS<MAX_MOTION_AXIS;AXIS++)
	{
		//set pulse command output mode		
		switch ( AXIS )
		{
		case AXIS_X:	m_ErrorStatus = _M114GL_set_pls_outmode(m_CardNo, AXIS, 4);	break;
		case AXIS_Y:	m_ErrorStatus = _M114GL_set_pls_outmode(m_CardNo, AXIS, 7);	break;
		case AXIS_Z:	m_ErrorStatus = _M114GL_set_pls_outmode(m_CardNo, AXIS, 5);	break;
		default:	    m_ErrorStatus = _M114GL_set_pls_outmode(m_CardNo, AXIS, 4);	break;
		}		
		if ( m_ErrorStatus != ERR_NoError )	//1->OUT/DIROUT Rising edge, DIR+ is high level
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_pls_outmode(m_CardNo, AXIS, 7)"));
			return false;
		}

		//set encoder input mode
		m_ErrorStatus = _M114GL_set_pls_iptmode(m_CardNo, AXIS, 2, 1);
		if ( m_ErrorStatus != ERR_NoError )  //2->4X A/B, 1->Inverse direction
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_pls_iptmode(m_CardNo,AXIS,2,0)"));
			return false;
		}

		//set counter input source //注意一定要這樣設定		
		m_ErrorStatus = _M114GL_set_feedback_src(m_CardNo, AXIS,0);
		if ( m_ErrorStatus != ERR_NoError )  //0->External Feedback
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_feedback_src(m_CardNo,AXIS,0)"));
			return false;
		}
				
		//Set Home Config						
		//_m114GL_set_home_config(SelectCard= 3,SelectAxis= 0,home_mode= 0,org_logic= 1,ez_logic= 1,ez_count= 3,ERC_output= 0);		
		m_ErrorStatus = _M114GL_set_home_config(m_CardNo, AXIS, 0, 1, 1, 3, 1);
		if ( m_ErrorStatus != ERR_NoError) 
		{
			this->m_ErrorString.Format(_T("Error, _M114GL_set_home_config Fault"));
			return false; 
		}

		//set alarm logic = Low				
		m_ErrorStatus = _M114GL_set_alm(m_CardNo,AXIS,0,0);
		if ( m_ErrorStatus != ERR_NoError ) //0->Active at Low, 0->Immediate stop
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_alm(m_CardNo,AXIS,0,0)"));
			return false;
		}

		//set end limit logic 
		m_ErrorStatus = _M114GL_set_ell(m_CardNo,AXIS,1);//1->high
		if ( m_ErrorStatus != ERR_NoError )  //0->Immediate stop
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_ell(m_CardNo,AXIS,1)"));
			return false;
		}
		

		//set end limit active				
		m_ErrorStatus = _M114GL_set_el(m_CardNo,AXIS,0);
		if ( m_ErrorStatus != ERR_NoError )  //0->Immediate stop, 1->Slow Down
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_el(m_CardNo,AXIS,0)"));
			return false;
		}

		//set servo on logic = low for J2S 		
		m_ErrorStatus = _M114GL_set_servo(m_CardNo,AXIS,0);
		if ( m_ErrorStatus != ERR_NoError ) //1->turn off
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_servo(m_CardNo,AXIS,1)"));
			return false;
		}

		//set in-position logic			
		m_ErrorStatus = _M114GL_set_inp(m_CardNo,AXIS, 1, 1);	
		if ( m_ErrorStatus != ERR_NoError ) //0->Disable, 0->active at low
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_inp(m_CardNo,AXIS, 1, 1)"));
			return false;
		}

		//set move ratio (command/feedback)				
		m_ErrorStatus = _M114GL_set_move_ratio(m_CardNo,AXIS, 1); 
		if ( m_ErrorStatus != ERR_NoError )
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_move_ratio(m_CardNo,AXIS, 1)"));
			return false;
		}

		//turn off ERC signal
		m_ErrorStatus = _M114GL_set_erc_on(m_CardNo,AXIS, 0);
		if ( m_ErrorStatus != ERR_NoError )
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_erc_on(m_CardNo,AXIS, 0)"));			
			return false;
		}

		//set ERC turn on mode (0~6=>time, 7=level output)
		m_ErrorStatus = _M114GL_set_erc(m_CardNo,AXIS, 0, 7);
		if ( m_ErrorStatus != ERR_NoError )
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_erc(m_CardNo,AXIS, 0, 7)"));			
			return false;
		}

		//set SD(slow down) configuration
		//I16 status= _M114GL_set_sd(U16 SwitchCardNo, U16 AxisNo, I16 enable,I16 sd_logic, I16 sd_latch, I16 sd_mode)
		m_ErrorStatus = _M114GL_set_sd(m_CardNo,AXIS, 1, 1, 0, 1);
		if ( m_ErrorStatus != ERR_NoError )
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_sd(m_CardNo,AXIS, 1, 1, 0, 1)"));			
			return false;
		}

		//set INT(interrupt) mode
		m_ErrorStatus = _M114GL_set_int_factor(m_CardNo,AXIS, 0x0000);//normal stop
		if ( m_ErrorStatus != ERR_NoError )
		{
			m_ErrorString.Format(_T("Error, _M114GL_set_int_factor(m_CardNo,AXIS, 0)"));
			return false;
		}

		//disable software limit
		m_ErrorStatus = _M114GL_disable_soft_limit(m_CardNo, AXIS);
		if ( m_ErrorStatus != ERR_NoError )
		{
			m_ErrorString.Format(_T("Error, _M114GL_disable_soft_limit(m_CardNo,AXIS)"));			
			return false;
		}		
	}	
#endif//MOTION_OBJ_DISABLE
	SetInitialize(true);
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::Stop(I16 AxisNO)
{	
#ifndef MOTION_OBJ_DISABLE	
	const double Tdec = 0.15f;
	if ( this->CheckInit() == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(AxisNO, _T("Stop"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Stop Start"));
	//m_ErrorStatus = _8164_sd_stop(AxisNO,Tdec);	
	m_ErrorStatus = _M114GL_sd_stop(m_CardNo,AxisNO,Tdec);	
	if ( m_ErrorStatus != ERR_NoError )
	{
		m_ErrorString.Format(_T("Error, _M114GL_sd_stop"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Stop NG End"));
		return false;
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Stop OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
int  CMotion_PCI_M114GL::GetIOStatus(int AxisNo, LPTSTR Str)//回傳錯誤狀態
{	
	U16 io_sts = 0;
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return 0;	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIOStatus Start"));
	//m_ErrorStatus = _8164_get_io_status(AxisNo, &io_sts);
	m_ErrorStatus = _M114GL_get_io_status(m_CardNo, AxisNo, &io_sts);
	if ( m_ErrorStatus != ERR_NoError )
	{
		m_ErrorString.Format(_T("Error, _M114GL_get_io_status"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIOStatus NG-1 End"));
		return -1;
	}	
	
	if ( NULL != Str )
	{
		int IoID = 0;
		CString Status;
		CString stringbuff;			
	
		IoID = PCI_M114GL_IO_RDY;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_ALM;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_PEL;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_NEL;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_ORG;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_DIR;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}

		//0x40->reserve
	
		IoID = PCI_M114GL_IO_PCS;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_ERC;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_EZ;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}

		//0x400 - Reserved
		IoID = PCI_M114GL_IO_LATCH;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_SD;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_INP;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			stringbuff = this->GetIOText(IoID);
			if ( Status.GetLength() == 0 )
			{	Status = stringbuff; }
			else
			{	Status = Status + _T(", ") + stringbuff; }		
		}
	
		IoID = PCI_M114GL_IO_SVON;
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
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIOStatus OK-2 End"));
	return io_sts;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::CheckAxis(I16 axis, TCHAR *pAxisS)
{
	if ( axis < 0 ) 
	{	
		this->m_ErrorString.Format(_T("Error, Axis Exception (%d)"), axis);
		return false; 
	}
#ifdef STAND_ALONE_VERSION
	if ( axis!=AXIS_X && axis!=AXIS_Y && axis!=AXIS_Z  && axis!=AXIS_4  )
	{
		::sprintf(this->m_ErrorString, "Error, Axis Exception (%d)", axis);
		return false; 
	}
#else
	if ( axis!=AXIS_X && axis!=AXIS_Y && axis!=AXIS_Z )
	{
		if ( this->GetIsSupportGantry() == true )
		{
			if ( axis != AXIS_X_SLAVE )
			{
				this->m_ErrorString.Format(_T("Error, Axis Exception (%d)"), axis);
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Axis Exception (%d)"), axis);
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
bool CMotion_PCI_M114GL::FreeRun(I16 axis, F64 StrVel, F64 MaxVel, F64 Tacc)
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(axis) == false ) 
	{	return false; }
	if ( CheckEnableAxis(axis) == false )
	{	return true;	}

	CMotion_Basic::SaveMotionProcess(axis, _T("FreeRun"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::FreeRun Start"));
	//m_ErrorStatus = _8164_tv_move(axis, StrVel, MaxVel, Tacc);
	m_ErrorStatus = _M114GL_tv_move(m_CardNo, axis, StrVel, MaxVel, Tacc);	
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("_M114GL_tv_move(axis, StrVel, MaxVel, Tacc"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::FreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::FreeRun OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::OneAxisMoveTo(I16 AxisNo, F64 Dist, F64 StrVel, F64 MaxVel,F64 Tacc,F64 Tdec, MOVE_CURVE_MODE VelCurve, MOVE_COORDINATE_MODE CoordMode, F64 SVacc, F64 SVdec)
{
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) 
	{	return false; }
	CString str;
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

	if ( CMotion_PCI_M114GL::WaitForDone(AxisNo) == false )
	{	return false; }

	if ( AxisNo == AXIS_X ) 
	{ 
		str.Format(_T("CMotion_PCI_M114GL::OneAxisMoveTo(X, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec);
		CMotion_Basic::SaveMotionCurrentProcess(str);		
		if ( Dist>m_MotionParameter.m_LimitMaxX || Dist<m_MotionParameter.m_LimitMinX )
		{
			str = _T("Error, X Axis Out of Stage Limit");
			str = CMotion_Basic::LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s (%.0f ~ %.0f, Pos=%.0f)"), str, m_MotionParameter.m_LimitMinX, m_MotionParameter.m_LimitMaxX, Dist);
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
		str.Format(_T("CMotion_PCI_M114GL::OneAxisMoveTo(Y, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec);
		CMotion_Basic::SaveMotionCurrentProcess(str);		
		if ( Dist>m_MotionParameter.m_LimitMaxY || Dist<m_MotionParameter.m_LimitMinY )
		{
			str = _T("Error, Y Axis Out of Stage Limit");
			str = CMotion_Basic::LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s (%.0f ~ %.0f, P=%.0f)"), str, m_MotionParameter.m_LimitMinY, m_MotionParameter.m_LimitMaxY, Dist);
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
		str.Format(_T("CMotion_PCI_M114GL::OneAxisMoveTo(Z, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec);
		CMotion_Basic::SaveMotionCurrentProcess(str);		
		if ( Dist>m_MotionParameter.m_LimitMaxZ || Dist<m_MotionParameter.m_LimitMinZ )
		{
			str = _T("Error, Z Axis Out of Stage Limit");
			str = CMotion_Basic::LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s (%.0f ~ %.0f, P=%.0f)"), str, m_MotionParameter.m_LimitMinZ, m_MotionParameter.m_LimitMaxZ, Dist);
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
	double Encode=0;
	this->GetEncode(AxisNo, Encode);
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

	//Tdec = Tdec*2;
	if ( CheckEnableAxis(AxisNo) == false )
	{	return true; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo Start"));
	if ( VelCurve ==  MOVE_CURVE_T )
	{	
		CString strInfo;
		strInfo.Format(_T("T-Curve Dist(%.2f), StrVel(%.2f), MaxVel(%.2f), Tacc(%.4f), Tdec(%.4f)"), Dist, StrVel, MaxVel, Tacc, Tdec);
		if ( FN_ENABLE == m_MotionParameter.m_SaveMotionCardParam  )
		{	SaveMotionProcess(AxisNo, strInfo, MSG_LEVEL_HIGH); }
		if ( CoordMode == MOVE_COORDINATE_ABS )
		{	
			//m_ErrorStatus = _8164_start_ta_move(AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec);
			m_ErrorStatus = _M114GL_start_ta_move(m_CardNo, AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec);
			if ( m_ErrorStatus != ERR_NoError )
			{
				CString str = "";
				GetErrorCodeText(m_ErrorStatus, str);
				this->m_ErrorString.Format(_T("Error, _M114GL_start_ta_move Fault.(%s)"), str);

				int axis = AxisNo;
				double dist = Dist;
				double startvel = StrVel;
				double maxvel = MaxVel;
				double acc = Tacc;
				double dec = Tdec;
				str.Format(_T("Axis: %d, Dist: %.2f,  StartVel: %.2f, MaxVel: %.2f, TAcc: %.2f, TDec: %.2f"), axis, dist, startvel, maxvel, acc, dec);				
				CMotion_Basic::SaveMotionCurrentProcess(m_ErrorString);				
				CMotion_Basic::SaveMotionCurrentProcess(str);
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo NG-1 End"));
				return false; 
			}
		}
		else if ( CoordMode == MOVE_COORDINATE_INS )
		{
			//m_ErrorStatus = _8164_start_tr_move(AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec);
			m_ErrorStatus = _M114GL_start_tr_move(m_CardNo, AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec);
			if ( m_ErrorStatus != ERR_NoError) 
			{
				this->m_ErrorString.Format(_T("Error, _M114GL_start_tr_move Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo NG-2 End"));
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Coordinate Mode Exception (%d)"), CoordMode);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo NG-3 End"));
			return false; 
		}		
	}
	else if ( VelCurve == MOVE_CURVE_S )
	{		
		SVacc = (MaxVel-StrVel)/3;
		SVdec = (MaxVel-StrVel)/3;
		const double DifVel = (MaxVel-StrVel);
		const double Ratio = m_MotionParameter.m_SCurveVelRatio/100.0;
		SVacc = Ratio*DifVel*Tdec/(Tacc+Tdec);
		SVdec = Ratio*DifVel*Tacc/(Tacc+Tdec);

		CString strInfo;
		strInfo.Format(_T("S-Curve Dist(%.2f), StrVel(%.2f), MaxVel(%.2f), Tacc(%.4f), Tdec(%.4f), SVacc(%.2f), SVdec(%.2f)"), Dist, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec);
		if ( FN_ENABLE == m_MotionParameter.m_SaveMotionCardParam  )
		{	SaveMotionProcess(AxisNo, strInfo, MSG_LEVEL_HIGH); }
		if ( CoordMode == MOVE_COORDINATE_ABS )
		{
			//MaxVel = -MaxVel;
			//m_ErrorStatus = _8164_start_sa_move(AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec);
			m_ErrorStatus = _M114GL_start_sa_move(m_CardNo, AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec);
			if ( m_ErrorStatus != ERR_NoError) 
			{
				this->m_ErrorString.Format(_T("Error, _M114GL_start_sa_move Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo NG-4 End"));
				return false; 
			}
		}
		else if ( CoordMode == MOVE_COORDINATE_INS )
		{
			//m_ErrorStatus = _8164_start_sr_move(AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec);
			m_ErrorStatus = _M114GL_start_sr_move(m_CardNo, AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec, SVacc, SVdec);
			if ( m_ErrorStatus != ERR_NoError) 
			{
				this->m_ErrorString.Format(_T("Error, _M114GL_start_sr_move Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo NG-5 End"));
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Coordinate Mode Exception (%d)"), CoordMode);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo NG-6 End"));
			return false; 
		}		
	}
	else
	{
		this->m_ErrorString.Format(_T("Error, Velocity Curve Exception (%d)"), CoordMode);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo NG-7 End"));
		return false; 
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::OneAxisMoveTo OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::SetORGAll()
{
	if ( this->CheckInit() == false ) { return false; }
	if ( this->SetORG(AXIS_X) == false ) 
	{	return false; }	
	if ( this->SetORG(AXIS_Y) == false ) 
	{	return false; }	
	if ( this->SetORG(AXIS_Z) == false ) 
	{	return false; }	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::SetORG(I16 AxisNo)
{	
#ifndef MOTION_OBJ_DISABLE
	F64 pos = 0;
	I32 cmd = 0;
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }

	CMotion_Basic::SaveMotionProcess(AxisNo, _T("SetORG"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetORG Start"));
	
	m_ErrorStatus = _M114GL_set_position(m_CardNo, AxisNo, pos);//feedback
	if ( m_ErrorStatus != ERR_NoError) 
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_position Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetORG NG-1 End"));
		return false; 
	}	
	m_ErrorStatus = _M114GL_set_command(m_CardNo, AxisNo, cmd);//command pulse
	if ( m_ErrorStatus != ERR_NoError) 
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_ command Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetORG NG-2 End"));
		return false; 
	}	
	m_ErrorStatus = _M114GL_reset_error_counter(m_CardNo, AxisNo);
	if ( m_ErrorStatus != ERR_NoError) 
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_reset_error_counter Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetORG NG-3 End"));
		return false; 
	}	
	m_ErrorStatus = _M114GL_set_target_pos(m_CardNo, AxisNo, pos);//end position
	if ( m_ErrorStatus != ERR_NoError) 
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_target_pos Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetORG NG-4 End"));
		return false; 
	}

	//I16 status= _M114GL_set_axis_counter (U16 SwitchCardNo, U16 AxisCounterNo, U16 CntMode, U16 CntDir, I32 SetValue)
	m_ErrorStatus = _M114GL_set_axis_counter(m_CardNo, AxisNo, 0, 0, 0);//end position
	if ( m_ErrorStatus != ERR_NoError) 
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_target_pos Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetORG NG-4 End"));
		return false; 
	}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetORG OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
LPCTSTR CMotion_PCI_M114GL::GetIOText(int IO)//取得IO的文字
{	
	switch ( IO )
	{
	case PCI_M114GL_IO_RDY:	m_MotionIOStats = _T("RDY pin input");	break;
	case PCI_M114GL_IO_ALM:	m_MotionIOStats = _T("Alarm signal");	break;
	case PCI_M114GL_IO_PEL:	m_MotionIOStats = _T("Positive Limit");	break;
	case PCI_M114GL_IO_NEL:	m_MotionIOStats = _T("Negative Limit");	break;
	case PCI_M114GL_IO_ORG:	m_MotionIOStats = _T("Origin signal");	break;
	case PCI_M114GL_IO_DIR:	m_MotionIOStats = _T("DIR output");	break;
	case PCI_M114GL_IO_PCS:	m_MotionIOStats = _T("PCS signal input");	break;
	case PCI_M114GL_IO_ERC:	m_MotionIOStats = _T("ERC pin output");	break;
	case PCI_M114GL_IO_EZ:	m_MotionIOStats = _T("EZ index signalt");	break;
	case PCI_M114GL_IO_CLR:	m_MotionIOStats = _T("CLR signalt");	break;
	case PCI_M114GL_IO_LATCH:	m_MotionIOStats = _T("Latch signal input");	break;
	case PCI_M114GL_IO_SD:	m_MotionIOStats = _T("Slow down signal input");	break;
	case PCI_M114GL_IO_INP:	m_MotionIOStats = _T("In-Position signal input");	break;
	case PCI_M114GL_IO_SVON:	m_MotionIOStats = _T("Servo-ON output status");	break;
	default:
		m_MotionIOStats = _T("No defined");
		break;
	}
	return m_MotionIOStats;
}
//----------------------------------------------------------------------------------//
int CMotion_PCI_M114GL::GetAxisStatus(I16 AxisNO, CString &str)//回傳該軸狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字
{
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNO) == false ) { return -1; }
	
#ifndef MOTION_OBJ_DISABLE
	//I16 m_sts=_8164_motion_done(AxisNO);
	I16 m_sts=_M114GL_motion_done(m_CardNo, AxisNO);	

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
		this->m_ErrorString.Format(_T("Error, _M114GL_motion_done Exception"));
		str = m_ErrorString;
		return -1;
	}
	return (int)m_sts;
#else
	return 0;
#endif
	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::ReleaseMotionCard()
{	
	if ( CheckInit() == false ) { return true; }	

	this->DisableSoftwareLimit(AXIS_X);
	this->DisableSoftwareLimit(AXIS_Y);
	this->DisableSoftwareLimit(AXIS_Z);

#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::ReleaseMotionCard Start"));
	for(I16 AxisNo=0;AxisNo<MAX_MOTION_AXIS;AxisNo++)
	{	
		m_ErrorStatus = _M114GL_set_servo(m_CardNo,AxisNo,0);	
		if ( m_ErrorStatus != ERR_NoError) 
		{
			this->m_ErrorString.Format(_T("Error, _M114GL_set_servo(m_CardNo,AxisNo,1) Fault"));
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::ReleaseMotionCard NG-1 End"));
			return false; 
		}
	}	
	m_ErrorStatus = _M114GL_close();
	if ( m_ErrorStatus != ERR_NoError) 
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_close Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::ReleaseMotionCard NG-2 End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::ReleaseMotionCard OK End"));
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
//----------------------------------------------------------------------------------//
void CMotion_PCI_M114GL::SetTriggerParameter(double SP, double EP, double Start, double End, double Interval, int MaxRepeatCounts, int YMaxCounts, double YOffset, int ORGX, int ORGY, int ORGZ)
{
//	this->SetSystemDefaultMotionParameter();	
	this->m_TriggerStartPos = SP;			//移動的起點	(含加速距離)
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
bool CMotion_PCI_M114GL::ForwardTriggerProcess_JET7000S()//正向移動
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
	//1. Disable Auto Trigger  //_M114GL_start_auto_trigger (U16 SwitchCardNo, U16 CompareNo, U16 On_Off)
	m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_LED, 0);//LED Trigger
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}
	m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_CCD, 0);//Camera Trigger
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}
		
	//2. Set Axis Count  _M114GL_set_axis_counter (U16 SwitchCardNo, U16 AxisCounterNo, U16 CntMode, U16 CntDir, I32 SetValue)//AxisCounterValue
	m_ErrorStatus = _M114GL_set_axis_counter(m_CardNo, TriggerAxis, CntMode, CntDir, AxisCounter);//Axis Counter
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}

	//3. Set Trigger Output pulse width  _M114GL_set_trigger_pulsewidth(SwitchCardNo, CompareNo, PulseWidth) 184=>166us
	m_ErrorStatus = _M114GL_set_trigger_pulsewidth(m_CardNo, CompareNo_LED, 184);//LED Trigger 
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_trigger_pulsewidth Fault"));
		return false;
	}
	m_ErrorStatus = _M114GL_set_trigger_pulsewidth(m_CardNo, CompareNo_CCD, 184);//Camera Trigger 
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_trigger_pulsewidth Fault"));
		return false;
	}

	//4.Set Auto Trigger parameter  _M114GL_set_auto_trigger_comparator(SwitchCardNo, CompareNo, AxisCounterNo, Dir, StartPos, Interval, TriggerNum)
	m_ErrorStatus = _M114GL_set_auto_trigger_comparator(m_CardNo, CompareNo_LED, TriggerAxis, Dir, Start1, TriggerInterval, NElems);//LED Trigger 
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_auto_trigger_comparator Fault"));
		return false;
	}

	m_ErrorStatus = _M114GL_set_auto_trigger_comparator(m_CardNo, CompareNo_CCD, TriggerAxis, Dir, Start2, TriggerInterval, NElems);//Camera Trigger 
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_auto_trigger_comparator Fault"));
		return false;
	}

	//5.Enable Auto Trigger
	m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_LED, 1);//Camera Trigger
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}
	m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_CCD, 1);//Camera Trigger
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}	
	delete[] pArray; pArray=NULL;
	
	m_ErrorStatus = _M114GL_start_ta_move(m_CardNo, TRIGGER_AXIS,EndPosX, m_MotionParameter.m_TriggerStartVelocity ,m_MotionParameter.m_TriggerMaxVelocity,m_MotionParameter.m_TriggerAccelerationTime,m_MotionParameter.m_TriggerDecelerationTime);
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_ta_move Fault in Trigger"));
		return false; 
	}
	
#endif//MOTION_OBJ_DISABLE
	this->m_TriggerRepeatCounts ++;	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::BackwardTriggerProcess_JET7000S()//逆向移動
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
	//1. Disable Auto Trigger  //_M114GL_start_auto_trigger (U16 SwitchCardNo, U16 CompareNo, U16 On_Off)
	m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_LED, 0);//LED Trigger
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}
	m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_CCD, 0);//Camera Trigger
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}
	
	//2. Set Axis Count  _M114GL_set_axis_counter (U16 SwitchCardNo, U16 AxisCounterNo, U16 CntMode, U16 CntDir, I32 SetValue)	//set axis counter value
	m_ErrorStatus = _M114GL_set_axis_counter(m_CardNo, TriggerAxis, CntMode, CntDir, AxisCounter);//Axis Counter
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}

	//3. Set Trigger Output pulse width  _M114GL_set_trigger_pulsewidth(SwitchCardNo, CompareNo, PulseWidth) 184=>166us
	m_ErrorStatus = _M114GL_set_trigger_pulsewidth(m_CardNo, CompareNo_LED, 184);//LED Trigger 
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_trigger_pulsewidth Fault"));
		return false;
	}
	m_ErrorStatus = _M114GL_set_trigger_pulsewidth(m_CardNo, CompareNo_CCD, 184);//Camera Trigger 
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_trigger_pulsewidth Fault"));
		return false;
	}

	//4.Set Auto Trigger parameter  _M114GL_set_auto_trigger_comparator(SwitchCardNo, CompareNo, AxisCounterNo, Dir, StartPos, Interval, TriggerNum)
	m_ErrorStatus = _M114GL_set_auto_trigger_comparator(m_CardNo, CompareNo_LED, TriggerAxis, Dir, Start1, TriggerInterval, NElems);//LED Trigger 
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_auto_trigger_comparator Fault"));
		return false;
	}

	m_ErrorStatus = _M114GL_set_auto_trigger_comparator(m_CardNo, CompareNo_CCD, TriggerAxis, Dir, Start2, TriggerInterval, NElems);//Camera Trigger 
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_auto_trigger_comparator Fault"));
		return false;
	}

	//5.Enable Auto Trigger
	m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_LED, 1);//Camera Trigger
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}
	m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_CCD, 1);//Camera Trigger
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
		return false;
	}
	delete[] pArray; pArray=NULL;

	m_ErrorStatus = _M114GL_start_ta_move(m_CardNo, TRIGGER_AXIS,StartPosX, m_MotionParameter.m_TriggerStartVelocity ,m_MotionParameter.m_TriggerMaxVelocity,m_MotionParameter.m_TriggerAccelerationTime,m_MotionParameter.m_TriggerDecelerationTime);	
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_ta_move Fault in Trigger"));
		return false;
	}
	
#endif//MOTION_OBJ_DISABLE
	this->m_TriggerRepeatCounts ++;
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::ForwardTriggerProcess()
{
	bool IsOK = false;
	IsOK = this->ForwardTriggerProcess_JET7000S();	
	return IsOK;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::BackwardTriggerProcess()
{	
	bool IsOK = false;
	IsOK = this->BackwardTriggerProcess_JET7000S();
	return IsOK;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::StopCompareTrigger(bool IsStop)//是否停止同步比較送外部觸發訊號
{	
#ifndef MOTION_OBJ_DISABLE
	const int CompareNo_LED = 0;
	const int CompareNo_CCD = 1;	
	CMotion_Basic::SaveMotionProcess(-1, _T("StopCompareTrigger"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::StopCompareTrigger Start"));
	if ( IsStop == true ) 
	{
		m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_LED, 0);//LED Trigger
		if ( m_ErrorStatus != ERR_NoError )
		{
			this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
			return false;
		}
		m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, CompareNo_CCD, 0);//Camera Trigger
		if ( m_ErrorStatus != ERR_NoError )
		{
			this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
			return false;
		}
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::StopCompareTrigger End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::EnableCompareTrigger(const bool IsEnable)
{		
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }	
	CMotion_Basic::SaveMotionProcess(-1, _T("EnableCompareTrigger"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::EnableCompareTrigger Start"));
	if ( IsEnable == true )
	{	
		//m_ErrorStatus = _8164_set_auto_compare(TRIGGER_AXIS, TRIGGER_DEVICE); 
		/*
		m_ErrorStatus = _M114GL_start_auto_trigger(m_CardNo, TRIGGER_AXIS, TRIGGER_DEVICE); 
		if ( m_ErrorStatus != ERR_NoError )
		{
			::strcpy(this->m_ErrorString, "Error, _M114GL_start_auto_trigger Fault");
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::EnableCompareTrigger NG-1 End"));
			return false; 
		}
		
		//I16 status= _M114GL_set_trigger_pulsewidth (U16 SwitchCardNo, U16 CompareNo, U16 PulseWidth)		
		*/
	}
	else
	{	
		this->StopCompareTrigger(true);
	}	//取消Trigger送出	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::EnableCompareTrigger OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
void CMotion_PCI_M114GL::GetErrorCodeText(const int status, CString &str)
{	
	switch ( status )
	{
	case ERR_NoError: str=_T("ERR_NoError"); break;
	case ERR_NoCardFound: str=_T("ERR_NoCardFound"); break;
	case ERR_lioReadIntMask: str=_T("ERR_lioReadIntMask"); break;
	case ERR_RingNo: str=_T("ERR_RingNo"); break;
	case ERR_CommSpeed: str=_T("ERR_CommSpeed"); break;
	case ERR_EventAllocateFailed: str=_T("ERR_EventAllocateFailed"); break;
	case ERR_OffsetOutOfRange: str=_T("ERR_OffsetOutOfRange"); break;
	case ERR_IncorrectCardNo: str=_T("ERR_IncorrectCardNo"); break;
	case ERR_IncorrectAxisNo: str=_T("ERR_IncorrectAxisNo"); break;
	case ERR_FailToWrite6045Register: str=_T("ERR_FailToWrite6045Register"); break;
	case ERR_FailToRead6045Register: str=_T("ERR_FailToRead6045Register"); break;
	case ERR_IncorrectMemorySpace: str=_T("ERR_IncorrectMemorySpace"); break;
	case ERR_FailToOpenCard: str=_T("ERR_FailToOpenCard"); break;
	case ERR_FailToGetSwitchId: str=_T("ERR_FailToGetSwitchId"); break;
	case ERR_SetRingConfig: str=_T("ERR_SetRingConfig"); break;
	case ERR_FailToWriteAxisCommand: str=_T("ERR_FailToWriteAxisCommand"); break;
	case ERR_FailToInitialCard: str=_T("ERR_FailToInitialCard"); break;
	case ERR_ReadGpioStatus: str=_T("ERR_ReadGpioStatus"); break;
	case ERR_PosOutOfRange: str=_T("ERR_PosOutOfRange"); break;
	case ERR_SetValueOutOfRange: str=_T("ERR_SetValueOutOfRange"); break;
	case ERR_FailToSetTRmvRegister: str=_T("ERR_FailToSetTRmvRegister"); break;
	case ERR_FailToSetFaRegister: str=_T("ERR_FailToSetFaRegister"); break;
	case ERR_FailToSetTAccTime: str=_T("ERR_FailToSetTAccTime"); break;
	case ERR_FailToSetSpeed: str=_T("ERR_FailToSetSpeed"); break;
	case ERR_FailToCheckRmvRegister: str=_T("ERR_FailToCheckRmvRegister"); break;
	case ERR_FailToSetRdpRegister: str=_T("ERR_FailToSetRdpRegister"); break;
	case ERR_FailToSetSAcceleration: str=_T("ERR_FailToSetSAcceleration"); break;
	case ERR_FailToSetSRmvRegister: str=_T("ERR_FailToSetSRmvRegister"); break;
	case ERR_FailToSetSRdpRegister: str=_T("ERR_FailToSetSRdpRegister"); break;
	case ERR_AxisAlreadyStop: str=_T("ERR_AxisAlreadyStop"); break;
	case ERR_ChannelNotCorrect: str=_T("ERR_ChannelNotCorrect"); break;		
	case ERR_ConfigFileOpenError: str=_T("ERR_ConfigFileOpenError"); break;
	case ERR_MoveRatioError: str=_T("ERR_MoveRatioError"); break;
	case ERR_FailToSetLatchNo: str=_T("ERR_FailToSetLatchNo"); break;
	case ERR_FailToSetCompareNo: str=_T("ERR_FailToSetCompareNo"); break;
	case ERR_FailToSetCompareMethod: str=_T("ERR_FailToSetCompareMethod"); break;
	case ERR_PChangeSlowDownPointError: str=_T("ERR_PChangeSlowDownPointError"); break;
	case ERR_SpeedChange: str=_T("ERR_SpeedChange"); break;
	case ERR_SlowDownPointError: str=_T("ERR_SlowDownPointError"); break;
	case ERR_FpgaControlPageNoNotCorrect: str=_T("ERR_FpgaControlPageNoNotCorrect"); break;
	case ERR_FailToSetPageNo: str=_T("ERR_FailToSetPageNo"); break;
	case ERR_FailToGetPageNo: str=_T("ERR_FailToGetPageNo"); break;
	case ERR_CompareSrcNotCorrect: str=_T("ERR_CompareSrcNotCorrect"); break;
	case ERR_CompareNoNotCorrect: str=_T("ERR_CompareNoNotCorrect"); break;
	case ERR_FailToWriteCmpMpcRegister: str=_T("ERR_FailToWriteCmpMpcRegister"); break;
	case ERR_FailToReadCmpMpcRegister: str=_T("ERR_FailToReadCmpMpcRegister"); break;
	case ERR_FailToWriteMpcCntRegister: str=_T("ERR_FailToWriteMpcCntRegister"); break;
	case ERR_FailToReadMpcCntRegister: str=_T("ERR_FailToReadMpcCntRegister"); break;
	case ERR_IncorrectIntType: str=_T("ERR_IncorrectIntType"); break;
	case ERR_FunctionNotSupportThisAsic: str=_T("ERR_FunctionNotSupportThisAsic"); break;
	case ERR_LinkIntError: str=_T("ERR_LinkIntError"); break;
	case ERR_FailToEnableInt: str=_T("ERR_FailToEnableInt"); break;
	case ERR_AxisArrayError: str=_T("ERR_AxisArrayError"); break;
	case ERR_SpeedError: str=_T("ERR_SpeedError"); break;
	case ERR_FailToCopyFH_FL: str=_T("ERR_FailToCopyFH_FL"); break;
	case ERR_Err3PointsInput: str=_T("ERR_Err3PointsInput"); break;
	case ERR_ErrValueOfCeterOutRange: str=_T("ERR_ErrValueOfCeterOutRange"); break;
	case ERR_FailToSetRipRegister: str=_T("ERR_FailToSetRipRegister"); break;
	case ERR_FailToSetRciRegister: str=_T("ERR_FailToSetRciRegister"); break;
	case ERR_InvalidMutexObject: str=_T("ERR_InvalidMutexObject"); break;
	case ERR_CardSwitchNOoutStrip: str=_T("ERR_CardSwitchNOoutStrip"); break;
	case ERR_CompareNo: str=_T("ERR_CompareNo"); break;
	case ERR_FailToReadFIFOCntlRegister: str=_T("ERR_FailToReadFIFOCntlRegister"); break;
	case ERR_GetLatchDataTimeOut: str=_T("ERR_GetLatchDataTimeOut"); break;
	case ERR_FailToGetLatchData: str=_T("ERR_FailToGetLatchData"); break;
	case ERR_LatchBufferFull: str=_T("ERR_LatchBufferFull"); break;
	case ERR_ReadMainStatus: str=_T("ERR_ReadMainStatus"); break;
	case ERR_FailToRestorIntFactor: str=_T("ERR_FailToRestorIntFactor"); break;
	case ERR_FailToCreateMutexObject: str=_T("ERR_FailToCreateMutexObject"); break;
	case ERR_FailToOpenMnet: str=_T("ERR_FailToOpenMnet"); break;
	case ERR_FailToOpenCMnetDll: str=_T("ERR_FailToOpenCMnetDll"); break;
	case ERR_SecurityFifoBusy: str=_T("ERR_SecurityFifoBusy"); break;
	case ERR_FailToReadSecurityFifo: str=_T("ERR_FailToReadSecurityFifo"); break;
	case ERR_IncorrectSecurityPage: str=_T("ERR_IncorrectSecurityPage"); break;
	case ERR_IncorrectPageNo: str=_T("ERR_IncorrectPageNo"); break;
	case ERR_FailToReadSecurityNum: str=_T("ERR_FailToReadSecurityNum"); break;
	case ERR_GetCenter: str=_T("ERR_GetCenter"); break;
	case ERR_FailToOpenEvent: str=_T("ERR_FailToOpenEvent"); break;
	case ERR_OpenMnetRpt: str=_T("ERR_OpenMnetRpt"); break;
	case ERR_OpenCardRpt: str=_T("ERR_OpenCardRpt"); break;
	case ERR_NotSetContinuousMove: str=_T("ERR_NotSetContinuousMove"); break;
	default:
		str.Format(_T("EERR_NOT_DEFINED(%d)"), status);
		break;	
	}
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::PreInitMotion()//預先初始化
{
//	CMotion_Basic::PreInitMotion();
	CMotion_Basic::m_MotionName = _T("PCI-M114GL");
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::InitialMotion()//初始化
{	
	bool IsOK = true;
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	SetMotionMachineType(SystemParam.m_MachineModelType);
#ifdef MOTION_OBJ_DISABLE
	IsOK = true;
	SetInitialize(true);
#else
	CMotion_Basic::SaveMotionProcess(-1, _T("InitialMotion"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::InitialMotion Start"));
	//IsOK = this->InitializeForJET7300();	
	IsOK = this->InitializeForJET6500();		
	if ( IsOK == false ) 
	{	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::InitialMotion NG End"));	}
	else
	{	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::InitialMotion OK End"));	}
#endif//MOTION_OBJ_DISABLE	
	return IsOK;	
	
}
//----------------------------------------------------------------------------------//	
bool CMotion_PCI_M114GL::ReleaseMotion()
{
	return this->ReleaseMotionCard();	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::Home(const int Axis)
{
#ifndef MOTION_OBJ_DISABLE	
	CString str;
	const I16 AxisNo = Axis;
	if ( this->CheckInit() == false ) { return false; }
	TCHAR AxisS[8]=_T("");
	if ( this->CheckAxis(AxisNo, AxisS) == false ) 
	{	return false; }
	if ( CheckEnableAxis(Axis) == false )
	{	return true; }
	str.Format(_T("Home %s"), AxisS);
	CMotion_Basic::SaveMotionProcess(Axis, _T("Home"), MSG_LEVEL_HIGH);	
	AOIDataCollect.SaveLogMessage(str);		

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home Start"));

	//if ( m_MotionParameter.m_UsingMotionSoftwareLimit == FN_ENABLE )
	{	this->DisableSoftwareLimit(Axis);	}

	if ( this->GetIsEnable(AxisNo) == false ) 
	{ 
		this->m_ErrorString.Format(_T("%s axis not enabled, Please Enable it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-1 End"));
		return false; 
	}
	if ( this->GetIsAlarm(AxisNo) == true )
	{
		this->m_ErrorString.Format(_T("%s axis was alarm, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-2 End"));
		return false; 
	}
	CMotion_Basic::SetIsHomed(Axis, false);	
	
	double HomeVelocity = 0;
	double ORGOffset = 10000;
	double ORGVelocity  = 50000;
	double HomePreMove = HOME_PRE_MOVE_DIST;
	const bool bConvert=GetIsConvertSignPositive();
	MACHINE_MODEL_TYPE	MachineType = GetMotionMachineType();
	switch ( Axis )
	{
	case AXIS_X:		
		str.Format(_T("Home X axis"));
		ORGOffset = m_MotionParameter.m_HomeOrgOffsetX;
		ORGVelocity = m_MotionParameter.m_HomeVelocityX;
		HomePreMove = m_MotionParameter.m_HomePreMoveDisX;		
		HomeVelocity = ORGVelocity;
		if ( false==bConvert && MACHINE_MODEL_6500==MachineType )
		{
			if ( HomeVelocity > 0 ) { HomeVelocity = -HomeVelocity; }
		}
		break;
	case AXIS_Y:		
		str.Format(_T("Home Y axis"));
		ORGOffset = m_MotionParameter.m_HomeOrgOffsetY;
		ORGVelocity = m_MotionParameter.m_HomeVelocityY;
		HomePreMove = m_MotionParameter.m_HomePreMoveDisY;
		if ( false==bConvert && MACHINE_MODEL_6500==MachineType )
		{
			if ( HomePreMove > 0 ) 
			{	HomePreMove = -HomePreMove; }		
		}
		HomeVelocity = ORGVelocity;
		if ( false==bConvert && MACHINE_MODEL_6500==MachineType )
		{
			if ( HomeVelocity < 0 ) { HomeVelocity = -HomeVelocity; }
		}
		break;
	case AXIS_Z:		
		str.Format(_T("Home Z axis"));
		ORGOffset = m_MotionParameter.m_HomeOrgOffsetZ;
		ORGVelocity = m_MotionParameter.m_HomeVelocityZ;
		HomePreMove = m_MotionParameter.m_HomePreMoveDisZ;		
		if ( false==bConvert && MACHINE_MODEL_6500==MachineType )
		{
			if ( HomePreMove > 0 ) 
			{	HomePreMove = -HomePreMove; }		
		}
		HomeVelocity = ORGVelocity;//注意Y軸歸零的方向
		if ( false==bConvert && MACHINE_MODEL_6500==MachineType )
		{
			if ( HomeVelocity < 0 ) { HomeVelocity = -HomeVelocity; }
		}
		break;
	}
	const int  bSign=GetAxisSignPositive(Axis);
	if ( true==bConvert && FN_DISABLE==bSign )
	{	
		ORGOffset = -ORGOffset;	
		HomePreMove = -HomePreMove;
	}

	//晚後移動, 避免撞機(低速模式)	
	m_ErrorStatus = _M114GL_start_tr_move(m_CardNo, AxisNo, HomePreMove, 0, HOME_MAX_VELOCITY, HOME_ACCELERATE_TIME, HOME_ACCELERATE_TIME);
	if ( m_ErrorStatus != ERR_NoError) 
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_start_tr_move Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-3 End"));
		return false; 
	}

	//----------等待歸零結束---------------------------------------
	if ( this->WaitForDone(AxisNo) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-4 End"));
		return false; 
	}
	
	if ( this->GetIsEmergencyOn(AxisNo) == true )
	{ 
		this->m_ErrorString.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-5 End"));
		return false; 
	}

	::Sleep(250);

	//高速模式的歸零, 但是不準	
	double HomeAccTime = 1.0;

	//低速模式的歸零, 準但很慢
	HomeAccTime = 0.10;
	//m_ErrorStatus = _M114GL_set_el(m_CardNo,AxisNo,1);//啟動SlowDown
	m_ErrorStatus = _M114GL_home_move(m_CardNo, AxisNo, 0, HomeVelocity, HomeAccTime);	
	if ( m_ErrorStatus != ERR_NoError) 
	{	
		//m_ErrorStatus = _M114GL_set_el(m_CardNo,AxisNo,0);//關閉SlowDown
		this->m_ErrorString.Format(_T("Error, _M114GL_home_move Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-9 End"));
		return false; 
	}

	//----------等待歸零結束---------------------------------------
	if ( this->WaitForDone(AxisNo) == false ) 
	{ 
		//m_ErrorStatus = _M114GL_set_el(m_CardNo,AxisNo,0);//關閉SlowDown
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-10 End"));
		return false; 
	}
	
	if ( this->GetIsEmergencyOn(AxisNo) == true )
	{ 
		this->m_ErrorString.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-6 End"));
		return false; 
	}

	//----------重設原點位置----------------------------------------//
	::Sleep(500);

	//m_ErrorStatus = _M114GL_set_el(m_CardNo,AxisNo,0);//關閉SlowDown
	if ( this->SetORG(AxisNo) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-13 End"));
		return false; 
	}

	if ( this->GetIsEmergencyOn(AxisNo) == true )
	{ 
		this->m_ErrorString.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-13 End"));
		return false; 
	}

	double Vel = HOME_MAX_VELOCITY;
	//double Vel = ORGVelocity;
	if ( Vel < 0 ) { Vel = -Vel; }		
	m_ErrorStatus = _M114GL_start_tr_move(m_CardNo, AxisNo, ORGOffset, 0, Vel, HOME_ACCELERATE_TIME, HOME_ACCELERATE_TIME);	
	if ( m_ErrorStatus != ERR_NoError) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-14 End"));
		return false; 
	}

	if ( this->WaitForDone(AxisNo) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-15 End"));
		return false; 
	}

	if ( this->GetIsEmergencyOn(AxisNo) == true )
	{ 
		this->m_ErrorString.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-7 End"));
		return false; 
	}

	if ( this->SetORG(AxisNo) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home NG-16 End"));
		return false; 
	}

	if ( this->GetIsSupportGantry() == true )
	{
		if ( Axis == AXIS_X )
		{	this->SetORG(AXIS_X_SLAVE);	}
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
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Home OK End"));
#endif//MOTION_OBJ_DISABLE
	
	CMotion_Basic::SetIsHomed(Axis, true);		
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::Enable(const int Axis)
{	
#ifndef MOTION_OBJ_DISABLE
	const I16 axis = Axis;
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(axis) == false ) { return false; }	
	if ( CheckEnableAxis(Axis) == false )
	{	return true; }
	CMotion_Basic::SaveMotionProcess(Axis, _T("Enable"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Enable Start"));		
	m_ErrorStatus = _M114GL_set_servo(m_CardNo, axis,1);
	if ( m_ErrorStatus != ERR_NoError ) //0->turn on
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_servo(m_CardNo, axis,1)"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Enable NG End"));
		return false;
	}

	if ( this->GetIsSupportGantry() == true )                                             
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
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Enable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::Disable(const int Axis)
{	
#ifndef MOTION_OBJ_DISABLE
	const I16 axis = Axis;
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(axis) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(Axis, _T("Disable"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Disable Start"));	
	m_ErrorStatus = _M114GL_set_servo(m_CardNo, axis,0);
	if ( m_ErrorStatus != ERR_NoError ) //1->turn off
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_servo(m_CardNo, axis,0)"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Disable NG End"));
		return false;
	}

	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( Axis == AXIS_X )
		{
			if ( this->Disable(AXIS_X_SLAVE) == false )
			{	return false;	}
		}
	}
	CMotion_Basic::SetIsEnabled(Axis, false);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::Disable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::WaitForDone(const int Axis, const int MaxPreCounts)
{		
#ifndef MOTION_OBJ_DISABLE	
	TCHAR AxisS[8]=_T("");
	I16 AxisNo = Axis;	
	if ( this->CheckInit() == false ) 
	{	return false;	}
	if ( this->CheckAxis(AxisNo, AxisS) == false ) 
	{	return false;	}
	if ( CheckEnableAxis(AxisNo) == false )
	{	return true;	}

	CString   str;
	int       IoID = 0;	
	U16       io_sts = 0;	
	DWORD     TickCnt1=0;
	DWORD     TickCnt2=0;
	DWORD     TickCntD=0;
	const int SeelpTime = 1;//10ms
	const DWORD MaxTickCount=20000;//20 sec
	CMotion_Basic::SaveMotionProcess(Axis, _T("WaitForDone"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::WaitForDone Start"));	

	TickCnt1 = ::GetTickCount();
	while ( _M114GL_motion_done(m_CardNo, AxisNo) != 0 )
	{		
		TickCnt2 = ::GetTickCount();
		TickCntD = TickCnt2-TickCnt1;
		if ( TickCntD > MaxTickCount )
		{
			str = _T("Error, wait for done too long");
			str = LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::WaitForDone NG End"));
			return false; 
		}

		_M114GL_get_io_status(m_CardNo, AxisNo, &io_sts);
		IoID = PCI_M114GL_IO_ALM;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			str = _T("Error, motion is alarm");
			//str = LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::WaitForDone NG End"));
			return false; 
		}
	
		IoID = PCI_M114GL_IO_EMG;
		if ( (io_sts&IoID)!= 0x00 )
		{	
			str = _T("Error, motion is ALM");
			//str = LoadMultiLanguageString(str, str);
			this->m_ErrorString.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::WaitForDone NG End"));
			return false; 
		}
		if ( SeelpTime > 0 )
		{	::Sleep(SeelpTime); }		
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::WaitForDone OK End"));
	return true;	
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::TriggerMoveTo(const int Axis, double TargetPos, bool IsModifyVelocity)//觸發移動至哪裡
{
#ifndef MOTION_OBJ_DISABLE
	I16 AxisNo = Axis;
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) 
	{	return false; }	
	F64 StrVel=0, MaxVel=300000, Tacc=0.1, Tdec=0.1;	
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
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::MoveTo(const int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)
{
#ifndef MOTION_OBJ_DISABLE
	I16 AxisNo = Axis;
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) 
	{	return false; }	
	MOVE_CURVE_MODE MoveCurveMode=MOVE_CURVE_T;
	F64 StrVel=0, MaxVel=300000, Tacc=0.1, Tdec=0.1;

	if ( AxisNo == AXIS_X ) 
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
	if ( AxisNo == AXIS_Y ) 
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
	if ( AxisNo == AXIS_Z ) 
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
//	return this->OneAxisMoveTo(AxisNo, Dist, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_S, MOVE_COORDINATE_ABS);
	return this->OneAxisMoveTo(AxisNo, TargetPos, StrVel, MaxVel, Tacc, Tdec, MoveCurveMode, MOVE_COORDINATE_ABS);
#endif
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::XYMoveTo(double PosX, double PosY, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//2軸移動，但非同動唷
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
		if ( this->MoveTo(AXIS_X, CaliX, MovingMode, IsModifyVelocity) == false ) { return false; }		
	}

	if ( fabs(CaliY-EncodeY) > ToleranceY ) 
	{
		if ( this->MoveTo(AXIS_Y, CaliY, MovingMode, IsModifyVelocity) == false ) { return false; }
	}	

#ifdef _DEBUG
	double PosX2=0, PosY2=0;
	CaliToStage(CaliX, CaliY, PosX2, PosY2);
	const double dPosX2=PosX2-PosX;
	const double dPosY2=PosY2-PosY;
	PosX2 = PosX2;
#endif//_DEBUG
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::XYZMoveTo(double PosX, double PosY, double PosZ, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//3軸移動，但非同動唷
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
		if ( this->MoveTo(AXIS_X, CaliX, MovingMode, IsModifyVelocity) == false ) { return false; }		
	}

	if ( fabs(CaliY-EncodeY) > ToleranceY ) 
	{
		if ( this->MoveTo(AXIS_Y, CaliY, MovingMode, IsModifyVelocity) == false ) { return false; }
	}
	
	if ( fabs(PosZ-EncodeZ) > ToleranceZ )
	{	
		if ( this->MoveTo(AXIS_Z, PosZ, MovingMode, IsModifyVelocity) == false ) { return false; }
	}
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetEncode(const int Axis, double &Encode, bool offline)//取得該軸的光學尺座標
{
	Encode = 0;	
#ifndef MOTION_OBJ_DISABLE
	I16 AxisNo = Axis;
	F64 encode = 0;
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	if ( true == offline )
	{
		Encode = GetCommandPos(Axis);		
		return true;
	}
	if ( CheckEnableAxis(Axis) == false )
	{	
		Encode = 0.0; 
		return true; 
	}
//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetEncode Start"));
	//m_ErrorStatus = _8164_get_position(AxisNo, &encode);
	m_ErrorStatus = _M114GL_get_position(m_CardNo, AxisNo, &encode);
	Encode = encode;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(Axis);		
		if ( FN_DISABLE == nSign )
		{	Encode = -Encode;	}
	}
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_get_position Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetEncode NG End"));
		return false; 
	}
//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetEncode OK End"));
#else
	Encode = GetCommandPos(Axis);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetCurrentPos(double &X, double &Y, bool offline)//取得目前機台位置
{
	double PosZ=0;
	return GetCurrentPos(X, Y, PosZ, offline);
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetCurrentPos(double &X, double &Y, double &Z, bool offline)//取得目前機台位置
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
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
bool CMotion_PCI_M114GL::GetIsEnable(const int AxisNo)//該軸是否為Serve ON, 也就是有送電來積磁
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return true;	}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEnable Start"));	
	int AxisIO = this->GetIOStatus(AxisNo, NULL);	
	if ( AxisIO < 0 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEnable NG End"));
		return false; 
	}	

	if ( (AxisIO & PCI_M114GL_IO_SVON)!= 0x00 )
	{	
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEnable OK-1 End"));
		return true;	
	}
	else
	{	
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEnable OK-2 End"));
		return false;	
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEnable OK-3 End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetIsReady(const int AxisNo)//Driver傳回該軸是否為RDY狀態
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo, NULL) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return false;	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsReady Start"));	
	int AxisStatus = this->GetIOStatus(AxisNo, NULL);
	if ( AxisStatus == -1 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsReady NG End"));
		return false; 
	}
	if ( (AxisStatus & PCI_M114GL_IO_RDY) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsReady OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsReady OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::ResetMotionCardCommandPos(int Axis)//重設軸控卡的命令位置
{
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(Axis, NULL) == false ) { return false; }
	F64 encode=0;
	m_ErrorStatus = _M114GL_get_position(m_CardNo, Axis, &encode);
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_get_position(m_CardNo, Axis, &encode)"));
		return false;
	}
	m_ErrorStatus = _M114GL_set_command(m_CardNo, Axis, (int)(encode));
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_command(m_CardNo, Axis, (int)(encode))"));
		return false;
	}
	CMotion_Basic::SetCommandPos(Axis, encode);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::DoFaultAck(const int Axis)
{//Alarm Reset	
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(Axis, NULL) == false ) { return false; }
	if ( CheckEnableAxis(Axis) == false )
	{	return true;	}
	CMotion_Basic::SaveMotionProcess(Axis, _T("DoFaultAck"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::DoFaultAck Start"));
	//設定ERC為Hi	
	m_ErrorStatus = _M114GL_set_ralm(m_CardNo, Axis, 1);
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_erc(m_CardNo, AXIS, 1, 0)"));
		return false;
	}
	::Sleep(500);
	//設定ERC為Low	
	m_ErrorStatus = _M114GL_set_ralm(m_CardNo, Axis, 0);
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_erc(m_CardNo, AXIS, 0, 0)"));
		return false;
	}	

	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( Axis == AXIS_X )
		{
			if ( this->DoFaultAck(AXIS_X_SLAVE) == false )
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
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::DoFaultAck OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
const TCHAR* CMotion_PCI_M114GL::GetAxisStatus(const int Axis)
{
	CMotion_PCI_M114GL::GetAxisStatus(Axis, m_MotionStatus);
	return this->m_MotionStatus;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetIsERCActive(const int AxisNo)//Driver傳回該軸是否為ERC Active狀態
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo, NULL) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return false;	}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsAlarm Start"));	
	int AxisStatus = this->GetIOStatus(AxisNo, NULL);
	if ( AxisStatus == -1 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsAlarm NG-1 End"));
		return false; 
	}
	if ( (AxisStatus & PCI_M114GL_IO_ERC) != 0x00 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsAlarm OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsAlarm OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false; 	
}
//-----------------------------------------------------------------------
bool CMotion_PCI_M114GL::GetIsEmergencyOn(const int AxisNo)//該軸是否收到急停訊號
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo, NULL) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return false;	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEmergencyOn Start"));
	
	int AxisStatus = this->GetIOStatus(AxisNo, NULL);
	if ( AxisStatus == -1 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEmergencyOn NG-1 End"));
		return false; 
	}
	if ( (AxisStatus & PCI_M114GL_IO_EMG) != 0x00 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEmergencyOn OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsEmergencyOn OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false; 	
}
//-----------------------------------------------------------------------
bool CMotion_PCI_M114GL::GetIsAlarm(const int AxisNo)//Driver傳回該軸是否為Alarm狀態
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo, NULL) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return false;	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsAlarm Start"));
	
	int AxisStatus = this->GetIOStatus(AxisNo, NULL);
	if ( AxisStatus == -1 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsAlarm NG-1 End"));
		return false; 
	}
	if ( (AxisStatus & PCI_M114GL_IO_ALM) != 0x00 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsAlarm OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsAlarm OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false; 	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetIsInPosition(const int AxisNo)//Driver傳回該軸是否為In Position
{
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo, NULL) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return true;	}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsInPosition Start"));	
	int AxisStatus = this->GetIOStatus(AxisNo, NULL);
	if ( AxisStatus == -1 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsInPosition NG-1 End"));
		return false; 
	}
	if ( (AxisStatus & PCI_M114GL_IO_INP) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsInPosition OK End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsInPosition OK End"));
#endif//MOTION_OBJ_DISABLE
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetIsNLimit(const int AxisNo)//該軸的是否碰觸到副極限
{
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo, NULL) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return false;	}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsNLimit Start"));	
	int AxisStatus = this->GetIOStatus(AxisNo, NULL);
	if ( AxisStatus == -1 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsNLimit NG End"));
		return false; 
	}
	int Filter= PCI_M114GL_IO_NEL;	
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(AxisNo);		
		if ( FN_DISABLE == nSign )
		{	Filter = PCI_M114GL_IO_PEL;	}
	}
	if ( (AxisStatus & Filter) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsNLimit OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsNLimit OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetIsORG(const int AxisNo)//該軸是否在原點位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo, NULL) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return false;	}

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsORG Start"));	
	int AxisStatus = this->GetIOStatus(AxisNo, NULL);
	if ( AxisStatus == -1 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsORG NG End"));
		return false; 
	}
	if ( (AxisStatus & PCI_M114GL_IO_ORG) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsORG OK End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsORG OK End"));
#endif//MOTION_OBJ_DISABLE
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetIsPLimit(const int AxisNo)//該軸的是否碰觸到正極限	
{
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo, NULL) == false ) { return false; }
	if ( CheckEnableAxis(AxisNo) == false )
	{	return false;	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsPLimit Start"));	
	int AxisStatus = this->GetIOStatus(AxisNo, NULL);
	if ( AxisStatus == -1 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsPLimit NG End"));
		return false; 
	}
	int Filter= PCI_M114GL_IO_PEL;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(AxisNo);	
		if ( FN_DISABLE == nSign )
		{	Filter = PCI_M114GL_IO_NEL;	}
	}
	if ( (AxisStatus & Filter) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsPLimit OK End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::GetIsPLimit OK End"));
#endif//MOTION_OBJ_DISABLE
	return false;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::StartFreeRun(const int AxisNo, bool Dir)//Dir +為正方向, -為負方向
{
	if ( this->CheckInit() == false ) { return false; }
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("StartFreeRun"), MSG_LEVEL_HIGH);
	
	if ( CMotion_Basic::CheckIsEnabled(AxisNo) == false )
	{	return false; }	

	const bool BotMode = GetViewBotMode();
	int MaxV = (int)(GetAxisFreeRunVelocity(AxisNo));	
	if ( Dir == false ) { MaxV = (int)(-MaxV); }	
	switch ( AxisNo )
	{
	case AXIS_X:
		MaxV = MaxV*this->m_JogDirection_X;
		if ( true == BotMode )
		{	MaxV = -MaxV;	}
		m_FreeRunVel_X  = MaxV;		
		break;
	case AXIS_Y:
		MaxV = MaxV*this->m_JogDirection_Y;	
		m_FreeRunVel_Y  = MaxV;
		break;
	case AXIS_Z:
		MaxV = MaxV*this->m_JogDirection_Z;	
		m_FreeRunVel_Z  = MaxV;
		break;
	default:
		this->m_ErrorString.Format(_T("Error, Axis Exception"));
		return false;
	}
#ifndef MOTION_OBJ_DISABLE	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::StartFreeRun Start"));	
	m_ErrorStatus = _M114GL_tv_move(m_CardNo, AxisNo, m_MotionParameter.m_JogStartVelocity, MaxV, m_MotionParameter.m_JogAccelerationTime);
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_tv_move Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::StartFreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::StartFreeRun OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::StopFreeRun(const int AxisNo)//停止FreeRun
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
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::StopFreeRun Start"));	
	
	switch ( AxisNo )
	{
	case AXIS_Z:
		m_ErrorStatus = _M114GL_emg_stop(m_CardNo, AxisNo);
		break;
	default:
		m_ErrorStatus = _M114GL_sd_stop(m_CardNo, AxisNo, m_MotionParameter.m_JogDecelerationTime);
		break;
	}
	if ( m_ErrorStatus != ERR_NoError )
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_sd_stop Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::StopFreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::StopFreeRun OK End"));	
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::SaveMotionParamInternal()//儲存運動內部參數
{
#ifndef MOTION_OBJ_DISABLE
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::GetMotionIsReady()//取得運動系統是否正常
{
	if ( CheckInit() == false ) { return false; }
	this->m_ErrorString = _T("");
	
	//先判斷是否Enable	
	I16 AxisNo = AXIS_X;
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
bool CMotion_PCI_M114GL::SetIsWaitForInPosition(const int Axis, const bool Iswait)//設定是否等待定位停止
{
	if ( this->CheckInit() == false ) { return false; }
	
#ifndef MOTION_OBJ_DISABLE
	bool Wait = Iswait;
	if ( this->CheckAxis(Axis) == false ) { return false; }
	
	CMotion_Basic::SaveMotionProcess(Axis, _T("SetIsWaitForInPosition"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetIsWaitForInPosition Start"));
	int status = 1;	
	if ( Wait == true )
	{	
		m_ErrorStatus = _M114GL_set_inp(m_CardNo, Axis, 1, status);	//chia 0109
	}	
	else
	{	
		m_ErrorStatus = _M114GL_set_inp(m_CardNo, Axis, 0, status);	//chia 0109
	}	
		
	if ( m_ErrorStatus != ERR_NoError ) //0->Disable, 0->active at low
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_inp()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetIsWaitForInPosition NG End"));
		return false;
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetIsWaitForInPosition OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::ConfigTriggerTable(const bool IsReBuild)
{
#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::ConfigTriggerTable Start"));
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::ConfigTriggerTable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::ResetMotionDriver()//重新復歸運動的Driver
{	
#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::ResetMotionDriver Start"));	
	DoFaultAck(AXIS_X);
	DoFaultAck(AXIS_Y);
	DoFaultAck(AXIS_Z);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::ResetMotionDriver OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
CString CMotion_PCI_M114GL::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("MOTION_PCI_M114GL");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::FireSingleTrigger()//送出單一觸發訊號(包含燈源與相機)
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }
	const double TriggerBetweenTime = TRIGGER_CCD_BETWEEN_LED;//ms
	//Total Time, On Time, NTriggers
	//Total Time:166us, On Time:40us, NTriggers=1
	//Total Time: TRIGGER_CCD_BETWEEN_LED*2
	//On Time:40
	//NTrigger = 2

	int TotalTime = (int)(TriggerBetweenTime*1000);
	int OnTime = 40;
	int NTrigger = 1;
	
	//--------------------------------------------------------------------------------------------------------------//	
	this->m_ErrorString.Format(_T("Error, PIC-8164 does not support Fire single trigger mode"));
	return false;
	//--------------------------------------------------------------------------------------------------------------//	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::SetSoftwareLimit(int AxisNo, double Min, double Max, bool Auto)//設定軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("SetSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetSoftwareLimit Start"));

	I32 MinI = (I32)(Min);
	I32 MaxI = (I32)(Max);	

	if ( Auto == true ) 
	{
		if ( this->DisableSoftwareLimit(AxisNo) == false ) { return false; }
	}
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(AxisNo);
		if ( FN_DISABLE == nSign )
		{
			MinI = (I32)MIN(-Min, -Max);
			MaxI = (I32)MAX(-Min, -Max);
		}
	}
	//設定軟體極限
	m_ErrorStatus = _M114GL_set_soft_limit(m_CardNo, AxisNo,MaxI, MinI);
	if ( m_ErrorStatus != ERR_NoError ) //0->Disable, 0->active at low
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_set_soft_limit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetSoftwareLimit NG End"));
		return false;
	}	

	if ( Auto == true ) 
	{		
		if ( this->EnableSoftwareLimit(AxisNo) == false ) { return false; }
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::SetSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::EnableSoftwareLimit(int AxisNo)//啟用軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("EnableSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::EnableSoftwareLimit Start"));
	//開啟軟體極限, //0->INT, 1->Immediately stop, 2->Slow Down then stop, 3->Reserved
	m_ErrorStatus = _M114GL_enable_soft_limit(m_CardNo, AxisNo, 2);
	if ( m_ErrorStatus != ERR_NoError ) 
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_enable_soft_limit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::EnableSoftwareLimit NG End"));
		return false;
	}

	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( AxisNo == AXIS_X )
		{
		//	if ( this->EnableSoftwareLimit(AXIS_X_SLAVE) == false )
		//	{
		//		return false;
		//	}
			
		}
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::EnableSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCI_M114GL::DisableSoftwareLimit(int AxisNo)//關閉軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckAxis(AxisNo) == false ) { return false; }
	CMotion_Basic::SaveMotionProcess(AxisNo, _T("DisableSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::DisableSoftwareLimit Start"));

	//關閉軟體極限
	m_ErrorStatus = _M114GL_disable_soft_limit(m_CardNo, AxisNo);
	if ( m_ErrorStatus != ERR_NoError ) //0->Disable, 0->active at low
	{
		this->m_ErrorString.Format(_T("Error, _M114GL_disable_soft_limit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::DisableSoftwareLimit NG End"));
		return false;
	}

	if ( this->GetIsSupportGantry() == true ) 
	{
		if ( AxisNo == AXIS_X )
		{
			if ( this->DisableSoftwareLimit(AXIS_X_SLAVE) == false )
			{
				return false;
			}			
		}
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCI_M114GL::DisableSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
#endif//MOTION_DERIVE_MODE