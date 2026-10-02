// CameraCtrl.cpp: implementation of the CCameraCtrl class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "CameraCtrl.h"
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_GRABLINKFULL_CSC6M100BMP11_P100
	#include "Camera_GrabLinkFull_CSC6M100BMP11.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_GRABLINKFULL_Q12A65FM
	#include "Camera_GrabLinkFull_Q12A65Fm.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_COAXLINK_Q_12A180_FM
	#include "Camera_CoaxLink_Q_12A180F.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_COAXLINK_VC_12MX_M180
	#include "Camera_Coaxlink_VC_12MX_M180.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_COAXLINK_QUAD_G3_CAMERA
	#include "Camera_CoaxlinkQuadG3_Camera.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_COAXLINK_QUAD_CXP12_CAMERA
	#include "Camera_CoaxLinkQuadCXP12_Camera.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_TELI_BU1203MC
	#include "Camera_Teli_BU1203MC.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_TELI_BU1207MCF
	#include "Camera_Teli_BU1207MCF.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_MATROX_RAPIXO_CXP12_CAMERA
	#include "Camera_MatroxRapixoCXP12_Camera.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_MATROX_RAPIXO_CXP6_CAMERA
#include "Camera_MatroxRapixoCXP6_Camera.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE==CAMERA_OBJ_DYNAMIC_MODULE
	#include "Camera_DynamicModule.h"
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CCameraCtrl CameraCtrl;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//	
IMPLEMENT_DYNAMIC(CCameraCtrl, CObject)
//-------------------------------------------------------------------------------------//
int CCameraCtrl::GetPhaseMode(DWORD PatternStep, DWORD PhaseNum)//取得相位模式
{
	int Mode = 0;
	switch ( PatternStep )
	{	
	case BATCH_GRAB_STEP_4:
		switch ( PhaseNum )
		{
		case BATCH_GRAB_PHASE_1: Mode = LIGHT3D_PHASE_4_4_1;	break;			
		case BATCH_GRAB_PHASE_2: Mode = LIGHT3D_PHASE_4_4_2;	break;		
		case BATCH_GRAB_PHASE_M: Mode = LIGHT3D_PHASE_4_4_M;	break;	
		case BATCH_GRAB_PHASE_M2: Mode= LIGHT3D_PHASE_4_4_M_2;  break;			
		}
		break;	
	}
	return Mode;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BuildCameraIDCombox(CComboBox &Combox)//建立相機編號列表
{
	int     idx=0;
	CString str;
	CAMERA_ID CameraID = CAMERA_ID_1;
	JetAPI::ClearCombox(Combox);
	idx = 0;

	CameraID = CAMERA_ID_1;
	str = CCameraCtrl::GetCameraIDText(CameraID);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, CameraID);
	idx ++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BuildCameraGrabModeCombox(CComboBox &Combox)//建立相機取像模式列表
{
	int     idx=0;
	CString str;
	CAMERA_GRAB_MODE GrabMode = CAMERA_GRAB_FREE_RUN;
	JetAPI::ClearCombox(Combox);
	idx = 0;

	GrabMode = CAMERA_GRAB_FREE_RUN;
	str = CCameraCtrl::GetCameraGrabModeText(GrabMode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, GrabMode);
	idx ++;

	GrabMode = CAMERA_GRAB_EXTERNAL_TRIGGER;
	str = CCameraCtrl::GetCameraGrabModeText(GrabMode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, GrabMode);
	idx ++;

	GrabMode = CAMERA_GRAB_SOFTWARE_TRIGGER;
	str = CCameraCtrl::GetCameraGrabModeText(GrabMode);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, GrabMode);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BuildCameraCallbackTimmingCombox(CComboBox &Combox)//建立相機回傳模式列表
{
	int     idx=0;
	CString str;
	CAMERA_CALLBACK_TIMMING Timming = CAMERA_CALLBACK_EACH_FRAME;
	JetAPI::ClearCombox(Combox);
	idx = 0;

	Timming = CAMERA_CALLBACK_EACH_FRAME;
	str = CCameraCtrl::GetCameraCallbackTimmingText(Timming);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Timming);
	idx ++;

	Timming = CAMERA_CALLBACK_FREE_FRAME;
	str = CCameraCtrl::GetCameraCallbackTimmingText(Timming);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Timming);
	idx ++;

	Timming = CAMERA_CALLBACK_BATCH_GRAB_DONE;
	str = CCameraCtrl::GetCameraCallbackTimmingText(Timming);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Timming);
	idx ++;

	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BuildBatchGrabLightNumCombox(CComboBox &Combox)//建立批次取像燈源數量列表
{
	int     idx=0;
	CString str;
	DWORD   Param=0;
	JetAPI::ClearCombox(Combox);
	idx = 0;

	str = _T(" 1");
	Param = BATCH_GRAB_LIGHT_01;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T(" 2");
	Param = BATCH_GRAB_LIGHT_02;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T(" 3");
	Param = BATCH_GRAB_LIGHT_03;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T(" 4");
	Param = BATCH_GRAB_LIGHT_04;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T(" 5");
	Param = BATCH_GRAB_LIGHT_05;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T(" 6");
	Param = BATCH_GRAB_LIGHT_06;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T(" 7");
	Param = BATCH_GRAB_LIGHT_07;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T(" 8");
	Param = BATCH_GRAB_LIGHT_08;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T(" 9");
	Param = BATCH_GRAB_LIGHT_09;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("10");
	Param = BATCH_GRAB_LIGHT_10;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("11");
	Param = BATCH_GRAB_LIGHT_11;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("12");
	Param = BATCH_GRAB_LIGHT_12;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("Close");
	Param = BATCH_GRAB_LIGHT_00;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BuildBatchGrabPhaseStepCombox(CComboBox &Combox)//建立批次取像相位步數列表
{
	int     idx=0;
	CString str;
	DWORD   Param=0;
	JetAPI::ClearCombox(Combox);
	idx = 0;
	
	str = _T("1");
	Param = BATCH_GRAB_STEP_1;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("4");
	Param = BATCH_GRAB_STEP_4;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BuildBatchGrabPhasePeriodCombox(CComboBox &Combox)//建立批次取像相位步數列表
{
	int     idx=0;
	CString str;
	DWORD   Param=0;
	JetAPI::ClearCombox(Combox);
	idx = 0;
	
	str = _T("1");
	Param = BATCH_GRAB_PERIOD_1;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("2");
	Param = BATCH_GRAB_PERIOD_2;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BuildBatchGrabPhaseCastCombox(CComboBox &Combox)//建立批次取像相位投射列表
{
	int     idx=0;
	CString str;
	DWORD   Param=0;
	JetAPI::ClearCombox(Combox);
	idx = 0;
	
	str = _T("1");
	Param = BATCH_GRAB_3D_CAST_01;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("2");
	Param = BATCH_GRAB_3D_CAST_02;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("3");
	Param = BATCH_GRAB_3D_CAST_03;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("4");
	Param = BATCH_GRAB_3D_CAST_04;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("AB");
	Param = BATCH_GRAB_3D_CAST_AB;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("CD");
	Param = BATCH_GRAB_3D_CAST_CD;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("ABCD");
	Param = BATCH_GRAB_3D_CAST_ABCD;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("Close");
	Param = NULL;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BuildBatchGrabPhaseIDCombox(CComboBox &Combox)//建立相位編號視窗
{
	CString str;
	int     idx=0;	
	DWORD   Param=0;

	JetAPI::ClearCombox(Combox);

	str = _T("1-Phase");	
	Param=BATCH_GRAB_PHASE_1;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("2-Phase");	
	Param=BATCH_GRAB_PHASE_2;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("M-Phase");	
	Param=BATCH_GRAB_PHASE_M;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;

	str = _T("M-Phase-2");	
	Param=BATCH_GRAB_PHASE_M2;
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CCameraCtrl::GetCameraIDText(CAMERA_ID CameraID)//取得相機編號文字
{
	CString str;
	switch ( CameraID )
	{
	case CAMERA_ID_1:	str = _T("Camera 1");	break;
	case CAMERA_ID_2:	str = _T("Camera 2");	break;
	case CAMERA_ID_3:	str = _T("Camera 3");	break;
	case CAMERA_ID_4:	str = _T("Camera 4");	break;
	case CAMERA_ID_5:	str = _T("Camera 5");	break;
	default:            str = _T("Camera 0");	break;	
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CCameraCtrl::GetCameraGrabModeText(CAMERA_GRAB_MODE GrabMode)//取得相機取像模式
{
	CString str;
	switch ( GrabMode )
	{
	case CAMERA_GRAB_FREE_RUN:	str = _T("Free Run");	break;
	case CAMERA_GRAB_EXTERNAL_TRIGGER:	str = _T("Ext. Trigger");	break;
	case CAMERA_GRAB_SOFTWARE_TRIGGER:	str = _T("Sof. Trigger");	break;
	default:            str = _T("Not Defined");	break;	
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CString CCameraCtrl::GetCameraCallbackTimmingText(CAMERA_CALLBACK_TIMMING Timming)//取得相機回傳模式列表
{
	CString str;
	switch ( Timming )
	{
	case CAMERA_CALLBACK_EACH_FRAME:	  str = _T("Each Frame");	break;
	case CAMERA_CALLBACK_FREE_FRAME:	  str = _T("Free Frame");	break;
	case CAMERA_CALLBACK_BATCH_GRAB_DONE: str = _T("Batch Grab");	break;
	default:            str = _T("Not Defined");	break;	
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CAMERA_ID CCameraCtrl::GetCaemraIDFromWParam(WPARAM wParam)//取得相機編號
{
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	switch ( wParam )
	{
	case WPARAM_CAMERA_1_CALLBACK:
		CameraID = CAMERA_ID_1;
		break;
	case WPARAM_CAMERA_2_CALLBACK:
		CameraID = CAMERA_ID_2;
		break;
	case WPARAM_CAMERA_3_CALLBACK:
		CameraID = CAMERA_ID_3;
		break;
	case WPARAM_CAMERA_4_CALLBACK:
		CameraID = CAMERA_ID_4;
		break;
	case WPARAM_CAMERA_5_CALLBACK:
		CameraID = CAMERA_ID_5;
		break;
	default:
		CameraID = PRIMARY_CAMERA_ID;
		break;
	}
	return CameraID;
}
//-------------------------------------------------------------------------------------//
CCameraCtrl::CCameraCtrl()
{	
	PreInitCameraCtrl();
	InitialCameraCtrl();
	InitialCamreaPtrList();
}
//-------------------------------------------------------------------------------------//
CCameraCtrl::~CCameraCtrl()
{
	DeleteBatchGrabFinishEvent();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::PreInitCameraCtrl()
{	
	m_BatchGrabFinishEvent = NULL;
	CreateBatchGrabFinishEvent();
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::InitialCameraCtrl()
{
	m_BatchGrabbing = false;
	m_BatchGrabExposure_us = 3000;
	m_BatchGrabCameaID = CAMERA_ID_1;
	m_BatchGrabMode = BATCH_GRAB_LIGHT_RGB;//批次取像模式
	m_BatchGrabStep = BATCH_GRAB_STEP_NULL;//批次取像階段
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::InitialCamreaPtrList()
{
	m_CameraPtrList.clear();
#ifdef CAMERA_LIST_USE	
	#if CAMERA_OBJ_MODE == CAMERA_OBJ_GRABLINKFULL_CSC6M100BMP11_P100
		if ( AddCameraPtr(&Camera_GrabLinkFull_CSC6M100BMP11) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_GRABLINKFULL_Q12A65FM
		if ( AddCameraPtr(&Camera_GrabLinkFull_Q12A65Fm) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_COAXLINK_Q_12A180_FM
		if ( AddCameraPtr(&Camera_CoaxLink_Q_12A180F) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_COAXLINK_VC_12MX_M180
		if ( AddCameraPtr(&Camera_CoaxLink_VC_12MX_M180) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_COAXLINK_QUAD_G3_CAMERA
		if ( AddCameraPtr(&Camera_CoaxLinkQuadG3_Camera) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_COAXLINK_QUAD_CXP12_CAMERA
		if ( AddCameraPtr(&Camera_CoaxLinkQuadCXP12_Camera) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_TELI_BU1203MC
		if ( AddCameraPtr(&Camera_Teli_BU1203MC) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_TELI_BU1207MCF
		if ( AddCameraPtr(&Camera_Teli_BU1207MCF) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_MATROX_RAPIXO_CXP12_CAMERA
		if ( AddCameraPtr(&Camera_MatroxRapixoCXP12_Camera) == false )
		{	return false;	}	
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_MATROX_RAPIXO_CXP6_CAMERA
		if (AddCameraPtr(&Camera_MatroxRapixoCXP6_Camera) == false)
		{	return false;	}
	#elif CAMERA_OBJ_MODE == CAMERA_OBJ_DYNAMIC_MODULE
		if ( AddCameraPtr(&Camera_DynamicModule) == false )
		{	return false;	}		
	#endif//CAMERA_OBJ_MODE
#endif//CAMERA_LIST_USE
	return true;	
}
//------------------------------------------------------------------------------//
bool CCameraCtrl::AddCameraPtr(CCamera_Basic *Ptr)
{
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	const size_t Count=m_CameraPtrList.size();
	if ( NULL == Ptr )
	{	
		m_ErrorString = _T("Error, Add Camera Ptr Fault [Ptr==NULL]");
		return false; 
	}
	CAMERA_ID CameraID;
	CAMERA_ID RefCameraID = Ptr->GetCameraID();
	for ( i=0; i<Count; i++ )
	{
		if ( Ptr == m_CameraPtrList[i] ) { continue; }
		if ( NULL == m_CameraPtrList[i] ) { continue; }
		CameraID = m_CameraPtrList[i]->GetCameraID();
		if ( CameraID == RefCameraID )
		{
			m_ErrorString = _T("Error, Add Camera Ptr Fault [CameraID Repeat]");
			return false;
		}
	}
	m_CameraPtrList.push_back(Ptr);	
#endif//CAMERA_LIST_USE
	return true;
}
//------------------------------------------------------------------------------//
bool CCameraCtrl::CheckCameraPtr(CCamera_Basic *Ptr)
{
	if ( NULL == Ptr )
	{
		m_ErrorString = _T("Error, Camera Ptr is NULL");
		return false;
	}
	return true;
}
//------------------------------------------------------------------------------//
size_t CCameraCtrl::GetCameraPtrCount()
{
	return m_CameraPtrList.size();
}
//------------------------------------------------------------------------------//
CCamera_Basic* CCameraCtrl::GetCameraPtr(size_t index, bool bCheck)
{
	if ( true == bCheck )
	{
		const size_t Count = m_CameraPtrList.size();
		if ( index >= Count )
		{	return NULL; }
	}
	return m_CameraPtrList[index];
}
//------------------------------------------------------------------------------//
CCamera_Basic* CCameraCtrl::GetCameraPtrByCameraID(CAMERA_ID CameraID)
{
	size_t i=0;
	const size_t Count = m_CameraPtrList.size();
	for ( i=0; i<Count; i++ )
	{
		if ( NULL == m_CameraPtrList[i] ) { continue; }
		if ( CameraID != m_CameraPtrList[i]->GetCameraID() )
		{	continue; }
		return m_CameraPtrList[i];
	}
	return NULL;
}
//------------------------------------------------------------------------------//
WPARAM CCameraCtrl::MapCameraIDToWParam(CAMERA_ID CamreaID)
{
	WPARAM Ret=0;
	switch ( CamreaID )
	{
	case CAMERA_ID_1:	Ret=WPARAM_CAMERA_1_CALLBACK;	break;
	case CAMERA_ID_2:	Ret=WPARAM_CAMERA_2_CALLBACK;	break;
	case CAMERA_ID_3:	Ret=WPARAM_CAMERA_3_CALLBACK;	break;
	case CAMERA_ID_4:	Ret=WPARAM_CAMERA_4_CALLBACK;	break;
	case CAMERA_ID_5:	Ret=WPARAM_CAMERA_5_CALLBACK;	break;
	}
	return Ret;
}
//------------------------------------------------------------------------------//
void CCameraCtrl::InitialErrorString()//清除錯誤訊息
{
	this->m_ErrorString = _T("");
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetCameraExceptionCode_Param(LPCTSTR Err)
{	
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Param(str);
	//SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_PARAM, Err);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetCameraExceptionCode_FileRead(LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileRead(str);
	//SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_FILE_READ, Err);	return;		
	return;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetCameraExceptionCode_FileWrite(LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileWrite(str);
	//SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_FILE_WRITE, Err);	return;		
	return;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetCameraExceptionCode(DWORD Code, LPCTSTR Err)
{	
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Camera(Code, str);	
	return;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CCameraCtrl::GetErrorString()//取得錯誤訊息
{
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Camera_Others(m_ErrorString);
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ReturnNoUseCameraList()
{
	m_ErrorString = _T("Error, No Define CAMERA_LIST_USE");
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Camera_Others(m_ErrorString);
	return false;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::ResetBatchGrabbing()
{
	SetBatchGrabbing(false);
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::GetBatchGrabbing() const
{	
	return m_BatchGrabbing;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetBatchGrabbing(bool val)
{
	m_BatchGrabbing = val;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::ClearAllCameraCount()//復歸所有相機觸發次數
{
#ifdef CAMERA_LIST_USE
	size_t i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount=GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		CameraPtr->ClearCountAll();
	}
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GeCameraCallbackCount(CAMERA_ID CameraID)//疊加相機回傳
{
	long Count = 0;
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	Count = CameraPtr->GetCountForCameraCallback();		
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return Count;
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GetCameraExposuredEndCount(CAMERA_ID CameraID)//疊加曝光結束回傳
{
	long Count = 0;
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	Count = CameraPtr->GetCountForCameraExposuredEnd();		
	return Count;
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return Count;
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GetImageCallbackCount(CAMERA_ID CameraID)//疊加影像回傳
{
	long Count = 0;
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	Count = CameraPtr->GetCountForImageCallback();		
	return Count;
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return Count;
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GetBufferCopyToHostCount(CAMERA_ID CameraID)//疊加影像複製
{
	long Count = 0;
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	Count = CameraPtr->GetCountForBufferCopyToHost();		
	return Count;
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::IncrementCameraCopyToHostCount(CAMERA_ID CameraID)//疊加影像複製
{
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	CameraPtr->IncrementCountForBufferCopyToHost();
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return true;
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GetCameraNFramesToGrab(CAMERA_ID CameraID)//取得相機多少張數要去取	
{
	long val = 0;
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	val = CameraPtr->GetCountForFramesToGrab();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return val;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetCameraNFramesToGrab(CAMERA_ID CameraID, long num)//設定相機多少張數要去取
{
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->SetCountForFramesToGrab(num) == false )
	{
		m_ErrorString = CameraPtr->GetCameraErrorString(); 
		return false;
	}
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return true;
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GetCameraBatchGrabCount(CAMERA_ID CameraID)//取得相機取像周期
{
	long val = 0;
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	val = CameraPtr->GetCountForBatchGrab();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return val;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetCameraBatchGrabCount(CAMERA_ID CameraID, long val)//設定相機取像周期
{
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->SetCountForBatchGrab(val) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString(); 
		return false;
	}
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetCameraToSendCallback(CAMERA_ID CameraID, BOOL val)//設定影像是否傳送callback
{
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	CameraPtr->SetCameraToSendCallback(val);
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::GetCameraToSendCallback(CAMERA_ID CameraID, BOOL &val)//取得影像是否傳送callback
{	
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	val = CameraPtr->GetCameraToSendCallback();
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return true;
}
//-------------------------------------------------------------------------------------//
CAMERA_EXPOSURE_MODE CCameraCtrl::GetCameraExposureMode(CAMERA_ID CameraID)//取得相機曝光模式
{
	CAMERA_EXPOSURE_MODE CameraExpMode=CAMERA_EXPOSURE_UNDEFINED;
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return CameraExpMode; }
	CameraExpMode = CameraPtr->GetCameraExposureMode();	
#endif//CAMERA_LIST_USE
	return CameraExpMode;
}
//-------------------------------------------------------------------------------------//
unsigned int CCameraCtrl::GetCameraImageBufferCount(CAMERA_ID CameraID)//取得相機影像暫存數量
{
	unsigned int CameraImageBufferCount = CAMERA_IMAGE_BUFFER_COUNT_3D;
	#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return CameraImageBufferCount; }
	CameraImageBufferCount = CameraPtr->GetCameraImageBufferCount();	
#endif//CAMERA_LIST_USE
	return CameraImageBufferCount;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetCameraCallbackTimming(CAMERA_ID CameraID, CAMERA_CALLBACK_TIMMING val)//設定相機回傳時機
{
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->SetCameraCallbackTimming(val) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		return false;
	}
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return true;
}
//-------------------------------------------------------------------------------------//
CAMERA_CALLBACK_TIMMING  CCameraCtrl::GetCameraCallbackTimming(CAMERA_ID CameraID)//取得相機回傳時機
{
	CAMERA_CALLBACK_TIMMING CallbackTimming = CAMERA_CALLBACK_EACH_FRAME;
#ifdef CAMERA_LIST_USE	
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return CallbackTimming; }
	CallbackTimming = CameraPtr->GetCameraCallbackTimming();	
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return CallbackTimming;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetAllSaveCameraAddRingBufferLog(bool val)//設定儲存增加相機影像列表資訊
{
#ifdef CAMERA_LIST_USE	
	size_t         i=0;
	CCamera_Basic *CameraPtr = NULL;
	const size_t   CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++  )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		CameraPtr->SetSaveCameraAddRingBufferLog(val);
	}	
#endif//CAMERA_LIST_USE
	return;
}
//-------------------------------------------------------------------------------------//
unsigned int CCameraCtrl::GetMaxGrayImageSize()//取得最大黑白影像尺寸
{
	unsigned int ImageSize = 0;
	unsigned int MaxImageSize = ImageSize;
#ifdef CAMERA_LIST_USE
	size_t i=0;
	CCamera_Basic *CameraPtr=NULL;
	size_t CameraCount = GetCameraPtrCount();	
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		ImageSize = CameraPtr->GetImageRawSize();
		if ( MaxImageSize < ImageSize )
		{	MaxImageSize = ImageSize;	}
	}
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return MaxImageSize;
}
//-------------------------------------------------------------------------------------//
unsigned int CCameraCtrl::GetMaxColorImageSize()//取得最大彩色影像尺寸
{
	unsigned int ImageSize = 0;//預設1MB尺寸
	unsigned int MaxImageSize = ImageSize;
#ifdef CAMERA_LIST_USE
	size_t i=0;
	CCamera_Basic *CameraPtr=NULL;
	size_t CameraCount = GetCameraPtrCount();	
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		ImageSize = CameraPtr->GetImageColorSize();
		if ( MaxImageSize < ImageSize )
		{	MaxImageSize = ImageSize;	}
	}
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return MaxImageSize;
}
//-------------------------------------------------------------------------------------//
double CCameraCtrl::GetCameraMinPeriod(CAMERA_ID CameraID)//取得相機最短需要時間
{
	double MinPeriod = 1000;//us
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	MinPeriod = CameraPtr->GetPeriodTime();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return MinPeriod;
}
//-------------------------------------------------------------------------------------//
double CCameraCtrl::GetCameraFramePerSecond(CAMERA_ID CameraID)//取得相機每秒取像張數
{
	double FPS = 1;
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	FPS = CameraPtr->GetCameraFPS();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return FPS;
}
//-------------------------------------------------------------------------------------//
CAMERA_IMAGE_MODE CCameraCtrl::GetCameraImageMode(CAMERA_ID CameraID)//取得相機影像模式
{
	CAMERA_IMAGE_MODE CameraImageMode=CAMERA_IMAGE_GRAY;
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return CameraImageMode; }
	CameraImageMode = CameraPtr->GetCameraImageMode();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return CameraImageMode;
}
//-------------------------------------------------------------------------------------//
unsigned int CCameraCtrl::GetCameraImageSizeW(CAMERA_ID CameraID)//取得相機影像寬度
{
	unsigned int ImageW = 0;
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return ImageW; }
	ImageW = CameraPtr->GetCameraImageW();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return ImageW;
}
//-------------------------------------------------------------------------------------//
unsigned int CCameraCtrl::GetCameraImageSizeH(CAMERA_ID CameraID)//取得相機影像長度
{
	unsigned int ImageH = 0;
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return ImageH; }
	ImageH = CameraPtr->GetCameraImageH();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return ImageH;
}
//-------------------------------------------------------------------------------------//
unsigned int CCameraCtrl::GetCameraImageStep(CAMERA_ID CameraID)//取得相機影像每條寬度
{
	unsigned int ImageStep = 0;
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return ImageStep; }
	ImageStep = CameraPtr->GetImageStep();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return ImageStep;
}
//-------------------------------------------------------------------------------------//
unsigned int CCameraCtrl::GetCameraGrayImageSize(CAMERA_ID CameraID)//取得相機黑白影像尺寸
{
	unsigned int ImageSize=0;
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return ImageSize; }
	ImageSize = CameraPtr->GetImageRawSize();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return ImageSize;
}
//-------------------------------------------------------------------------------------//
unsigned int CCameraCtrl::GetCameraColorImageSize(CAMERA_ID CameraID)//取得相機彩色影像尺寸
{
	unsigned int ImageSize=0;
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return ImageSize; }
	ImageSize = CameraPtr->GetImageColorSize();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
	return ImageSize;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::InitialAllCamera()//初始化
{
	bool IsOK = true;
	CString ErrorString;
	this->InitialErrorString();
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t         i=0;
	WPARAM         wParam;
	CAMERA_ID      CameraID;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();	
	ResetBatchGrabbing();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		CameraID = CameraPtr->GetCameraID();
		wParam = MapCameraIDToWParam(CameraID);
		CameraPtr->SetCameraWParam(wParam);
		if ( CameraPtr->InitialCamera() == true ) 
		{	continue; }
		
		IsOK = false;
		if ( this->m_ErrorString.GetLength() > 0 )
		{
			ErrorString = this->m_ErrorString;
			this->m_ErrorString.Format(_T("%s, %s"), ErrorString, CameraPtr->GetCameraErrorString());
		}
		else
		{	this->m_ErrorString = CameraPtr->GetCameraErrorString(); }
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_CONNECT);
	}	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ReleaseAllCamera()//釋放
{
	bool IsOK = true;
	CString ErrorString;
	this->InitialErrorString();
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();	
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->ReleaseCamera() == true ) 
		{	continue; }

		IsOK = false;
		if ( this->m_ErrorString.GetLength() > 0 )
		{
			ErrorString = this->m_ErrorString;
			this->m_ErrorString.Format(_T("%s, %s"), ErrorString, CameraPtr->GetCameraErrorString());
		}
		else
		{	this->m_ErrorString = CameraPtr->GetCameraErrorString(); }
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RELEASE);
	}	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetAllCamera()//復歸
{
	bool IsOK = true;
	CString ErrorString;
	this->InitialErrorString();
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();	
	ResetBatchGrabbing();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->ResetCamera() == true ) 
		{	continue; }

		IsOK = false;
		if ( this->m_ErrorString.GetLength() > 0 )
		{
			ErrorString = this->m_ErrorString;
			this->m_ErrorString.Format(_T("%s, %s"), ErrorString, CameraPtr->GetCameraErrorString());
		}
		else
		{	this->m_ErrorString = CameraPtr->GetCameraErrorString(); }
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RESET);
	}	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::RegisterAllCameraCurrentProcessFile()//註冊所有相機的訊息檔案
{
	bool IsOK = true;
	CString ErrorString;
	this->InitialErrorString();
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();	
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->RegisterCameraCurrentProcessFile() == true ) 
		{	continue; }

		IsOK = false;
		if ( this->m_ErrorString.GetLength() > 0 )
		{
			ErrorString = this->m_ErrorString;
			this->m_ErrorString.Format(_T("%s, %s"), ErrorString, CameraPtr->GetCameraErrorString());
		}
		else
		{	this->m_ErrorString = CameraPtr->GetCameraErrorString(); }
	}	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CCameraCtrl::GetCameraErrorString(CAMERA_ID CameraID)//取得相機錯誤訊息
{
	this->m_ErrorString = _T("");
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	m_ErrorString = CameraPtr->GetCameraErrorString();
#else
	ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StopAllCameraGrab()//停止所有相機取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();	
	ResetBatchGrabbing();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->StopCameraGrab() == true ) 
		{	continue; }
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_STOP);
		return false;
	}	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StopCameraGrab(CAMERA_ID CameraID)//停止相機取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->StopCameraGrab() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_STOP);
		return false;	
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartCameraGrab(CAMERA_ID CameraID)//開始相機取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->StartCameraGrab() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_START);
		return false;	
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetCameraExposureTime(CAMERA_ID CameraID, const int ExposureTime)//設定相機曝光時間
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->SetExposureTime(ExposureTime) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_EXP_TIME_FUNC);
		return false;	
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
int CCameraCtrl::GetCameraExposureTime(CAMERA_ID CameraID)//取得相機曝光時間
{
	int ExposureTime = 0;
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	ExposureTime = CameraPtr->GetExposureTime();
#else
	ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return ExposureTime;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::FireCameraSoftwareTrigger(CAMERA_ID CameraID)//發送相機軟體觸發
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->FireSoftwareTrigger() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_SOFT_TRIG_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartAllCameraGrab()//開始所有相機取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->StartCameraGrab() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_START);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned char* CCameraCtrl::GetCameraGammaLookUpTable(CAMERA_ID CameraID)//取得相機的Gamma表
{
	unsigned char * TablePtr = NULL;
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	TablePtr = CameraPtr->GetGammaLUTPtr();	
#else
	ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return TablePtr;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetAllCameraApplyGamma(bool Apply)//設定所有相機是否使用Gamma
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SetCameraApplyGamma(Apply) == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_SET_FUNC);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetAllCameraExposureTime(const int ExposureTime)//設定所有相機曝光時間
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SetExposureTime(ExposureTime) == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_EXP_TIME_FUNC);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetAllCameraInternalTrigger()//設定相機內部觸發-立即取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SwitchCameraGrabMode(CAMERA_GRAB_FREE_RUN) == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_MODE);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetAllCameraExternalTrigger()//設定相機外部觸發
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SwitchCameraGrabMode(CAMERA_GRAB_EXTERNAL_TRIGGER) == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_MODE);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetAllCameraSoftwareTrigger()//設定相機軟體取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SwitchCameraGrabMode(CAMERA_GRAB_SOFTWARE_TRIGGER) == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_MODE);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetAllCameraGrabMode(CAMERA_GRAB_MODE Mode)//設定相機取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SwitchCameraGrabMode(Mode) == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_MODE);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
CAMERA_GRAB_MODE CCameraCtrl::GetCameraGrabMode(CAMERA_ID CameraID)//取得相機取像
{
	CAMERA_GRAB_MODE GrabMode = CAMERA_GRAB_FREE_RUN;
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return GrabMode; }
	GrabMode = CameraPtr->GetCameraGrabMode();
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return GrabMode;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetCameraGrabMode(CAMERA_ID CameraID, CAMERA_GRAB_MODE Mode)//設定相機取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->SwitchCameraGrabMode(Mode) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_MODE);
		return false;
	}
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::GetCameraCtrlReady()//取回相機控制是否正常
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->CheckCameraInited() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_CONNECT);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartCameraLiveGrab(CAMERA_ID CameraID)//開始相機連續取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->StartCameraLiveGrab() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_START);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartAllCameraLiveGrab()//開始所有相機連續取像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->StartCameraLiveGrab() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_START);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartCameraGrabbing(CAMERA_ID CameraID)//開始相機取像	
{
#ifndef CAMERA_OBJ_DISABLE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
BAYER_PATTERN_MODE CCameraCtrl::GetCameraBayerPattern(CAMERA_ID CameraID)//取得相機Bayer樣板
{
	BAYER_PATTERN_MODE BayerPattern=BAYER_PATTERN_NONE;
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return BayerPattern; }
	BayerPattern = CameraPtr->GetCameraBayerPattern();	
#else
	ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return BayerPattern;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ClearCameraCount(CAMERA_ID CameraID)//清除相機的次數紀錄
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	CameraPtr->ClearCountAll();	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::GetCameraCount(CAMERA_ID CameraID, long &cntCameraBak, long &cntExpBak, long &cntImageBak, long &cntImageCpy)//取得相機的次數紀錄
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	cntCameraBak = CameraPtr->GetCountForCameraCallback();
	cntExpBak = CameraPtr->GetCountForCameraExposuredEnd();
	cntImageBak = CameraPtr->GetCountForImageCallback();
	cntImageCpy = CameraPtr->GetCountForBufferCopyToHost();
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::GetCameraImage3(CAMERA_ID CameraID, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR pImage)//取得相機影像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->CloneCurrentCameraImage(ImageW, ImageH, ImageStep, BitCount, pImage) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GET_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::GetCameraRawImage(CAMERA_ID CameraID, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_PTR pImage)//取得相機原始影像
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->CloneCurrentRawImage(ImageW, ImageH, ImageStep, pImage) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GET_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetAllCameraRingBuffer()//覆歸所有相機環型記憶體列表
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->ResetCameraRingBuffer() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RING_BUFFER);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetCameraRingBuffer(CAMERA_ID CameraID)//覆歸相機環型記憶體列表
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->ResetCameraRingBuffer() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RING_BUFFER);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::KeepCameraTempRingBuffer(CAMERA_ID CameraID)//佔住相機環影像指標
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->KeepCameraTempRingBuffer() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RING_BUFFER);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::FreeCameraTempRingBuffer(CAMERA_ID CameraID)//釋放相機環影像指標
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->FreeCameraTempRingBuffer() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RING_BUFFER);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GetCameraRingBufferListSize(CAMERA_ID CameraID)//得目前相機環型指標數量
{
	long size=0;
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	size = CameraPtr->GetCameraRingBufferListSize();
#else
	ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return size;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::CheckCameraRingBufferStateDone(CAMERA_ID CameraID)//確認相機環型記憶影像狀態-已完成
{	
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	return CameraPtr->CheckCameraRingBufferStateDone();
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GetCameraRingBufferCurrentIndex(CAMERA_ID CameraID)//得目前相機環型指標引數 
{
	long index=0;
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	index = CameraPtr->GetCameraRingBufferCurrentIndex();
#else
	ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return index;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ReSortCameraRingBufferImage(CAMERA_ID CameraID, const TSliceParam &SliceParam)//重新排序相機環型指標引數
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->ReSortCameraRingBufferImage(SliceParam) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RING_BUFFER);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::GetCameraRingBufferImage(CAMERA_ID CameraID, long index, unsigned int &ImageW, unsigned int &ImageH, unsigned int &ImageStep, IMAGE_PTR &pImage)//取得相機原始影像	
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->GetCameraRingBufferImage(index, ImageW, ImageH, ImageStep, pImage) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RING_BUFFER);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetAllCameraExposureFinishEvent()//設定所有相機曝光結束事件
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SetCameraExposureFinishEvent() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_EXP_DONE_FUNC);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetAllCameraExposureFinishEvent()//復歸所有相機曝光結束事件
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->ResetCameraExposureFinishEvent() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_EXP_DONE_FUNC);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetCameraExposureFinishEvent(CAMERA_ID CameraID)//復歸相機曝光結束事件
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->ResetCameraExposureFinishEvent() == false )
	{
		m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_EXP_DONE_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartCameraExposureFinishThread(CAMERA_ID CameraID, bool WaitOn)//開始相機的曝光執行緒
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	//if ( CameraPtr->ResetCameraExposureFinishEvent() == false )
	//{
	//	m_ErrorString = CameraPtr->GetCameraErrorString();
	//	return false;
	//}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::WaitForCameraExposureFinishThreadStart(CAMERA_ID CameraID)//等待相機曝光結束執行緒起來
{
#ifndef CAMERA_OBJ_DISABLE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartAllCameraExposureFinishThread(bool WaitOn)//開始所有相機的曝光執行緒
{
#ifndef CAMERA_OBJ_DISABLE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::WaitForCameraExposureFinish(CAMERA_ID CameraID)//等帶相機曝光結束
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->WaitForCameraExposureFinishEvent() == false )
	{
		m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_EXP_DONE_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetAllCameraGrabFinishEvent()//設定所有相機取像結束事件
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SetCameraGrabFinishEvent() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_DONE_FUNC);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetAllCameraGrabFinishEvent()//復歸所有相機取像結束事件
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->ResetCameraGrabFinishEvent() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_DONE_FUNC);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetCameraGrabFinishEvent(CAMERA_ID CameraID)//復歸相機取像結束事件
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->ResetCameraGrabFinishEvent() == false )
	{
		m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_DONE_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartCameraGrabFinishThread(CAMERA_ID CameraID, bool WaitOn)//開始相機的取像結束執行緒	
{
#ifndef CAMERA_OBJ_DISABLE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::WaitForCameraGrabFinishThreadStart(CAMERA_ID CameraID)//等待相機的取像結束執行緒起來
{
#ifndef CAMERA_OBJ_DISABLE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::StartAllCameraGrabFinishThread(bool WaitOn)//開始所有相機的取像結束執行緒
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->WaitForCameraGrabFinishEvent() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_DONE_FUNC);
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::WaitForCameraGrabFinish(CAMERA_ID CameraID, int TimeOut)//等帶相機取像結束
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->WaitForCameraGrabFinishEvent() == false )
	{
		m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_DONE_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::WaitforCameraReadytoTrigger(CAMERA_ID CameraID)//等待相機可以觸發
{
#ifndef CAMERA_OBJ_DISABLE
	//SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_READY_TRIG_FUNC);
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetCameraReadytoTriggerEvent(CAMERA_ID CameraID)//復歸相機可以觸發事件
{
#ifndef CAMERA_OBJ_DISABLE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
double CCameraCtrl::ReadCameraTemperature(CAMERA_ID CameraID)//讀取相機溫度	
{
	double Temperature=0.0;
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return Temperature; }
	Temperature = CameraPtr->ReadCameraTemperature();	
#else
	return Temperature;	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return Temperature;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetCameraFunctionStartTime(CAMERA_ID CameraID)//設定相機函數起始時間
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return ; }
	CameraPtr->SetCameraFunctionStartTime();
#else
	ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetCameraFunctionEndTime(CAMERA_ID CameraID)//設定相機函數結束時間
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return ; }
	CameraPtr->SetCameraFunctionEndTime();
#else
	ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
}
//-------------------------------------------------------------------------------------//
double CCameraCtrl::GetCameraFunctionElapseTime(CAMERA_ID CameraID)//取得相機函數經過時間
{
	double Time = 0.0;
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return Time; }
	Time = CameraPtr->GetCameraFunctionElapseTime();
#else
	ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return Time;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::LoadAllCamerasINIFile()//載入所有相機參數
{
//#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->LoadCameraINIFile() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode_FileRead();
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
//#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SaveAllCamerasINIFile()//儲存所有相機參數
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	size_t  i=0;
	CCamera_Basic *CameraPtr=NULL;
	const size_t CameraCount = GetCameraPtrCount();
	for ( i=0; i<CameraCount; i++ )
	{
		CameraPtr = GetCameraPtr(i, false);
		if ( NULL == CameraPtr ) { continue; }
		if ( CameraPtr->SaveCameraINIFile() == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode_FileWrite();
			return false;
		}
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::LoadCameraINIFile(CAMERA_ID CameraID)//載入相機參數
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->LoadCameraINIFile() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode_FileRead();
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SaveCameraINIFile(CAMERA_ID CameraID)//儲存相機參數
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->SaveCameraINIFile() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode_FileWrite();
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetCameraWhiteBalance(CAMERA_ID CameraID, double WBR, double WBG, double WBB)//設定相機白平衡參數
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->SetWhiteBalanceParams(WBR, WBG, WBB) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_WHITE_BALANCE_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::GetCameraWhiteBalance(CAMERA_ID CameraID, double &WBR, double &WBG, double &WBB)//取得相機白平衡參數
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->GetWhiteBalanceParams(WBR, WBG, WBB) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_WHITE_BALANCE_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::CalcCameraWhiteBalance(CAMERA_ID CameraID)//計算相機白平衡
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->CalcWhiteBalanceParams() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_WHITE_BALANCE_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetCameraWhiteBalance(CAMERA_ID CameraID)//復歸相機白平衡
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->ResetWhiteBalanceParams() == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_WHITE_BALANCE_FUNC);
		return false;
	}
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::LoadCameraParameterFFCByLEDName(const char *LEDName)//取得相機白平衡
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::DeleteBatchGrabFinishEvent()//刪除批量取像完成事件
{
	if ( this->m_BatchGrabFinishEvent == NULL ) { return true; }
	::CloseHandle(this->m_BatchGrabFinishEvent);
	this->m_BatchGrabFinishEvent = NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::CreateBatchGrabFinishEvent()//建立批量取像完成事件
{
	this->DeleteBatchGrabFinishEvent();
	this->m_BatchGrabFinishEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Batch Grab Finish Event"));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::WaitForBatchGrabFinishEvent()//等待批量取像完成事件
{
#ifndef OFFLINE_VERSION
	if ( this->m_BatchGrabFinishEvent == NULL ) { return true; }	

	const double CameraFPS = 1000;
	const int NFrames = 10;
	const int Exposure_us = 1000;
	DWORD MinTime = 1000;
	DWORD ExposureTime = Exposure_us*20*NFrames;
	DWORD FPS_Time = 1000000/CameraFPS*10;

	if ( MinTime < ExposureTime )
	{	MinTime = ExposureTime;	}
	if ( MinTime < FPS_Time )
	{	MinTime = FPS_Time;	}

	DWORD Res=0;
	Res = ::WaitForSingleObject(this->m_BatchGrabFinishEvent, MinTime);
	if ( Res == WAIT_TIMEOUT )
	{
		this->m_ErrorString.Format(_T("Error, Wait for m_BatchGrabFinishEvent too long"));
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_DONE_FUNC);
		return false;
	}
#endif	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetBatchGrabFinishEvent()//設定批量取像完成事件
{
	if ( this->m_BatchGrabFinishEvent == NULL ) { return true; }
	::SetEvent(this->m_BatchGrabFinishEvent);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::ResetBatchGrabFinishEvent()//復歸批量取像完成事件
{
	if ( this->m_BatchGrabFinishEvent == NULL ) { return true; }
	::ResetEvent(this->m_BatchGrabFinishEvent);
	return true;
}
//-------------------------------------------------------------------------------------//
long CCameraCtrl::GetBatchGrabFrames(DWORD GrabMode, BATCH_GRAB_STEP GrabStep)//取得批量取像的張數
{
	long frames = 0;	
	long Step = 0;
	long PhaseN = 0;	
	switch ( GrabStep )
	{
	case BATCH_GRAB_STEP_3D_CAST_01:		
	case BATCH_GRAB_STEP_3D_CAST_02:		
	case BATCH_GRAB_STEP_3D_CAST_03:		
	case BATCH_GRAB_STEP_3D_CAST_04:
		if ( (GrabMode&BATCH_GRAB_STEP_1)==BATCH_GRAB_STEP_1 )
		{	Step = 1; }		
		else if ( (GrabMode&BATCH_GRAB_STEP_4)==BATCH_GRAB_STEP_4 )
		{	Step = 4; }		
		
		if ( (GrabMode&BATCH_GRAB_PHASE_1)==BATCH_GRAB_PHASE_1 )
		{	PhaseN = 1; }		
		else if ( (GrabMode&BATCH_GRAB_PHASE_2)==BATCH_GRAB_PHASE_2 )
		{	PhaseN = 2; }						
		else if ( (GrabMode&BATCH_GRAB_PHASE_3)==BATCH_GRAB_PHASE_3 )
		{	PhaseN = 3; }

		if ( 1 == Step )
		{	
			if ( 3 == PhaseN )
			{	frames = 3; }
			else
			{	frames = 1; }
		}
		else
		{	
			switch ( PhaseN )
			{
			case 1:
			case 2:
				frames = Step; 
				break;
			case 3:
				frames = 2*Step; 
				break;
			}			
		}
		break;
	case BATCH_GRAB_STEP_LIGHT_01:
	case BATCH_GRAB_STEP_LIGHT_02:
	case BATCH_GRAB_STEP_LIGHT_03:
	case BATCH_GRAB_STEP_LIGHT_04:
	case BATCH_GRAB_STEP_LIGHT_05:
	case BATCH_GRAB_STEP_LIGHT_06:
	case BATCH_GRAB_STEP_LIGHT_07:
	case BATCH_GRAB_STEP_LIGHT_08:
	case BATCH_GRAB_STEP_LIGHT_09:
	case BATCH_GRAB_STEP_LIGHT_10:
	case BATCH_GRAB_STEP_LIGHT_11:
	case BATCH_GRAB_STEP_LIGHT_12:
		frames = 1;
		break;
	}	
	return frames;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CLS_PTR CCameraCtrl::GetBatchGrabLightCtrlPtr(BATCH_GRAB_STEP Step)//取得批量取像的投光
{
	LIGHT_3D_CLS_PTR Ptr = NULL;
	switch ( Step )
	{
	case BATCH_GRAB_STEP_3D_CAST_01:
		Ptr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_01);
		break;
	case BATCH_GRAB_STEP_3D_CAST_02:
		Ptr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_02);
		break;
	case BATCH_GRAB_STEP_3D_CAST_03:
		Ptr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_03);
		break;
	case BATCH_GRAB_STEP_3D_CAST_04:
		Ptr = Light3DCtrl.GetLight3DCastPtr(LIGHT_3D_CAST_04);
		break;
	default:
		break;
	}	

	if ( NULL == Ptr )
	{	this->m_ErrorString.Format(_T("Error, CCameraCtrl get batch grab project ctrl fault")); }
	return Ptr;
}
//-------------------------------------------------------------------------------------//
BATCH_GRAB_STEP CCameraCtrl::GetBatchGrabFirstStep(DWORD GrabMode)//取得第一個步驟
{
	BATCH_GRAB_STEP Step = BATCH_GRAB_STEP_NULL;
	if ( (GrabMode&BATCH_GRAB_LIGHT_01) == BATCH_GRAB_LIGHT_01 )
	{	Step = BATCH_GRAB_STEP_LIGHT_01;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_02) == BATCH_GRAB_LIGHT_02 )
	{	Step = BATCH_GRAB_STEP_LIGHT_02;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_03) == BATCH_GRAB_LIGHT_03 )
	{	Step = BATCH_GRAB_STEP_LIGHT_03;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_04) == BATCH_GRAB_LIGHT_04 )
	{	Step = BATCH_GRAB_STEP_LIGHT_04;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_05) == BATCH_GRAB_LIGHT_05 )
	{	Step = BATCH_GRAB_STEP_LIGHT_05;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_06) == BATCH_GRAB_LIGHT_06 )
	{	Step = BATCH_GRAB_STEP_LIGHT_06;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_07) == BATCH_GRAB_LIGHT_07 )
	{	Step = BATCH_GRAB_STEP_LIGHT_07;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_08) == BATCH_GRAB_LIGHT_08 )
	{	Step = BATCH_GRAB_STEP_LIGHT_08;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_09) == BATCH_GRAB_LIGHT_09 )
	{	Step = BATCH_GRAB_STEP_LIGHT_09;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_10) == BATCH_GRAB_LIGHT_10 )
	{	Step = BATCH_GRAB_STEP_LIGHT_10;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_11) == BATCH_GRAB_LIGHT_11 )
	{	Step = BATCH_GRAB_STEP_LIGHT_11;	}
	else if ( (GrabMode&BATCH_GRAB_LIGHT_12) == BATCH_GRAB_LIGHT_12 )
	{	Step = BATCH_GRAB_STEP_LIGHT_12;	}
	else if ( (GrabMode&BATCH_GRAB_3D_CAST_01) == BATCH_GRAB_3D_CAST_01 )
	{	Step = BATCH_GRAB_STEP_3D_CAST_01;	}
	else if ( (GrabMode&BATCH_GRAB_3D_CAST_02) == BATCH_GRAB_3D_CAST_02 )
	{	Step = BATCH_GRAB_STEP_3D_CAST_02;	}
	else if ( (GrabMode&BATCH_GRAB_3D_CAST_03) == BATCH_GRAB_3D_CAST_03 )
	{	Step = BATCH_GRAB_STEP_3D_CAST_03;	}
	else if ( (GrabMode&BATCH_GRAB_3D_CAST_04) == BATCH_GRAB_3D_CAST_04 )
	{	Step = BATCH_GRAB_STEP_3D_CAST_04;	}	
	return Step;
}
//-------------------------------------------------------------------------------------//
BATCH_GRAB_STEP CCameraCtrl::GetBatchGrabNextStep(DWORD GrabMode, BATCH_GRAB_STEP CurStep)//取得下一個步驟
{	
	DWORD NextLight = BATCH_GRAB_LIGHT_01;
	BATCH_GRAB_STEP Step = BATCH_GRAB_STEP_NULL;
	BATCH_GRAB_STEP NexStep = BATCH_GRAB_STEP_NULL;
	switch ( CurStep )
	{
	case BATCH_GRAB_STEP_LIGHT_01:		
		NextLight = BATCH_GRAB_LIGHT_02;
		NexStep = BATCH_GRAB_STEP_LIGHT_02;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_01;
		break;
	case BATCH_GRAB_STEP_LIGHT_02:			
		NextLight = BATCH_GRAB_LIGHT_03;
		NexStep = BATCH_GRAB_STEP_LIGHT_03;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_02;
		break;
	case BATCH_GRAB_STEP_LIGHT_03:			
		NextLight = BATCH_GRAB_LIGHT_04;
		NexStep = BATCH_GRAB_STEP_LIGHT_04;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_03;
		break;
	case BATCH_GRAB_STEP_LIGHT_04:	
		NextLight = BATCH_GRAB_LIGHT_05;
		NexStep = BATCH_GRAB_STEP_LIGHT_05;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_04;
		break;
	case BATCH_GRAB_STEP_LIGHT_05:	
		NextLight = BATCH_GRAB_LIGHT_06;
		NexStep = BATCH_GRAB_STEP_LIGHT_06;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_05;
		break;
	case BATCH_GRAB_STEP_LIGHT_06:	
		NextLight = BATCH_GRAB_LIGHT_07;
		NexStep = BATCH_GRAB_STEP_LIGHT_07;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_06;
		break;
	case BATCH_GRAB_STEP_LIGHT_07:	
		NextLight = BATCH_GRAB_LIGHT_08;
		NexStep = BATCH_GRAB_STEP_LIGHT_08;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_07;
		break;
	case BATCH_GRAB_STEP_LIGHT_08:	
		NextLight = BATCH_GRAB_LIGHT_09;
		NexStep = BATCH_GRAB_STEP_LIGHT_09;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_08;
		break;
	case BATCH_GRAB_STEP_LIGHT_09:	
		NextLight = BATCH_GRAB_LIGHT_10;
		NexStep = BATCH_GRAB_STEP_LIGHT_10;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_09;
		break;
	case BATCH_GRAB_STEP_LIGHT_10:	
		NextLight = BATCH_GRAB_LIGHT_11;
		NexStep = BATCH_GRAB_STEP_LIGHT_11;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_10;
		break;
	case BATCH_GRAB_STEP_LIGHT_11:	
		NextLight = BATCH_GRAB_LIGHT_12;
		NexStep = BATCH_GRAB_STEP_LIGHT_12;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_11;
		break;
	case BATCH_GRAB_STEP_LIGHT_12:	
		NextLight = BATCH_GRAB_3D_CAST_01;	
		NexStep = BATCH_GRAB_STEP_3D_CAST_01;
		GrabMode = GrabMode-BATCH_GRAB_LIGHT_12;
		break;
	case BATCH_GRAB_STEP_3D_CAST_01:	
		NextLight = BATCH_GRAB_3D_CAST_02;
		NexStep = BATCH_GRAB_STEP_3D_CAST_02;
		GrabMode = GrabMode-BATCH_GRAB_3D_CAST_01;
		break;
	case BATCH_GRAB_STEP_3D_CAST_02:	
		NextLight = BATCH_GRAB_3D_CAST_03;	
		NexStep = BATCH_GRAB_STEP_3D_CAST_03;
		if ( (GrabMode&BATCH_GRAB_3D_CAST_01) == BATCH_GRAB_3D_CAST_01 )
		{	GrabMode = GrabMode-BATCH_GRAB_3D_CAST_01; }
		GrabMode = GrabMode-BATCH_GRAB_3D_CAST_02;
		break;
	case BATCH_GRAB_STEP_3D_CAST_03:	
		NextLight = BATCH_GRAB_3D_CAST_04;	
		NexStep = BATCH_GRAB_STEP_3D_CAST_04;
		GrabMode = GrabMode-BATCH_GRAB_3D_CAST_03;
		break;
	case BATCH_GRAB_STEP_3D_CAST_04:	
		NextLight = 0;	
		NexStep = BATCH_GRAB_STEP_NULL;
		GrabMode = GrabMode-BATCH_GRAB_3D_CAST_04;
		break;
	}
	if ( 0 == NextLight )
	{
		Step = NexStep;
		return Step;
	}

	if ( (GrabMode&NextLight)==NextLight )
	{	Step = NexStep; }
	else if ( (GrabMode&BATCH_GRAB_3D_CAST_01)==BATCH_GRAB_3D_CAST_01 )
	{	Step = BATCH_GRAB_STEP_3D_CAST_01; }
	else if ( (GrabMode&BATCH_GRAB_3D_CAST_02)==BATCH_GRAB_3D_CAST_02 )
	{	Step = BATCH_GRAB_STEP_3D_CAST_02; }
	else if ( (GrabMode&BATCH_GRAB_3D_CAST_03)==BATCH_GRAB_3D_CAST_03 )
	{	Step = BATCH_GRAB_STEP_3D_CAST_03; }
	else if ( (GrabMode&BATCH_GRAB_3D_CAST_04)==BATCH_GRAB_3D_CAST_04 )
	{	Step = BATCH_GRAB_STEP_3D_CAST_04; }	
	return Step;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabModeEncode(int LightNum, bool ProjA, bool ProjB, bool ProjC, bool ProjD, int Step, int nPhase, DWORD &Mode)
{
	Mode = 0;
	switch ( LightNum )
	{
	case 0:
		break;
	case  1:
		Mode = BATCH_GRAB_LIGHT_01;
		break;
	case  2:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02;
		break;
	case  3:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03;
		break;
	case  4:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04;
		break;
	case  5:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04|BATCH_GRAB_LIGHT_05;
		break;
	case  6:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04|BATCH_GRAB_LIGHT_05|BATCH_GRAB_LIGHT_06;
		break;
	case  7:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04|BATCH_GRAB_LIGHT_05|BATCH_GRAB_LIGHT_06|BATCH_GRAB_LIGHT_07;
		break;
	case  8:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04|BATCH_GRAB_LIGHT_05|BATCH_GRAB_LIGHT_06|BATCH_GRAB_LIGHT_07|BATCH_GRAB_LIGHT_08;
		break;
	case  9:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04|BATCH_GRAB_LIGHT_05|BATCH_GRAB_LIGHT_06|BATCH_GRAB_LIGHT_07|BATCH_GRAB_LIGHT_08|BATCH_GRAB_LIGHT_09;
		break;
	case 10:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04|BATCH_GRAB_LIGHT_05|BATCH_GRAB_LIGHT_06|BATCH_GRAB_LIGHT_07|BATCH_GRAB_LIGHT_08|BATCH_GRAB_LIGHT_09|BATCH_GRAB_LIGHT_10;
		break;
	case 11:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04|BATCH_GRAB_LIGHT_05|BATCH_GRAB_LIGHT_06|BATCH_GRAB_LIGHT_07|BATCH_GRAB_LIGHT_08|BATCH_GRAB_LIGHT_09|BATCH_GRAB_LIGHT_10|BATCH_GRAB_LIGHT_11;
		break;
	case 12:
		Mode = BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03|BATCH_GRAB_LIGHT_04|BATCH_GRAB_LIGHT_05|BATCH_GRAB_LIGHT_06|BATCH_GRAB_LIGHT_07|BATCH_GRAB_LIGHT_08|BATCH_GRAB_LIGHT_09|BATCH_GRAB_LIGHT_10|BATCH_GRAB_LIGHT_11|BATCH_GRAB_LIGHT_12;
		break;
	default:
		this->m_ErrorString.Format(_T("Error, Batch Grab Mode Encode Fault (Light Num:%d)"), LightNum);
		return false;		
	}	

	if ( true == ProjA )
	{	Mode = Mode|BATCH_GRAB_3D_CAST_01; }
	if ( true == ProjB )
	{	Mode = Mode|BATCH_GRAB_3D_CAST_02; }
	if ( true == ProjC )
	{	Mode = Mode|BATCH_GRAB_3D_CAST_03; }
	if ( true == ProjD )
	{	Mode = Mode|BATCH_GRAB_3D_CAST_04; }

	switch ( Step )
	{
	case 0:	break;
	case 1:	Mode = Mode|BATCH_GRAB_STEP_1;	break;	
	case 4:	Mode = Mode|BATCH_GRAB_STEP_4;	break;	
	default:
		this->m_ErrorString.Format(_T("Error, Batch Grab Mode Encode Fault (Step:%d)"), Step);
		return false;
	}	

	switch ( nPhase )
	{
	case 0:	break;
	case 1: Mode = Mode|BATCH_GRAB_PHASE_1;	break;	
	case 2: Mode = Mode|BATCH_GRAB_PHASE_2;	break;	
	case 3: Mode = Mode|BATCH_GRAB_PHASE_M;	break;	
	case 4: Mode = Mode|BATCH_GRAB_PHASE_M2; break;
	default:
		this->m_ErrorString.Format(_T("Error, Batch Grab Mode Encode Fault (Phase:%d)"), nPhase);
		return false;	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabModeDecode(DWORD Mode, int &LightNum, bool &ProjA, bool &ProjB, bool &ProjC, bool &ProjD, int &Step, int &nPhase)
{
	LightNum = 0;
	if ( (Mode&BATCH_GRAB_LIGHT_12)==BATCH_GRAB_LIGHT_12 )
	{	LightNum = 12; }
	else if ( (Mode&BATCH_GRAB_LIGHT_11)==BATCH_GRAB_LIGHT_11 )
	{	LightNum = 11; }
	else if ( (Mode&BATCH_GRAB_LIGHT_10)==BATCH_GRAB_LIGHT_10 )
	{	LightNum = 11; }
	else if ( (Mode&BATCH_GRAB_LIGHT_09)==BATCH_GRAB_LIGHT_09 )
	{	LightNum =  9; }
	else if ( (Mode&BATCH_GRAB_LIGHT_08)==BATCH_GRAB_LIGHT_08 )
	{	LightNum =  8; }
	else if ( (Mode&BATCH_GRAB_LIGHT_07)==BATCH_GRAB_LIGHT_07 )
	{	LightNum =  7; }
	else if ( (Mode&BATCH_GRAB_LIGHT_06)==BATCH_GRAB_LIGHT_06 )
	{	LightNum =  6; }
	else if ( (Mode&BATCH_GRAB_LIGHT_05)==BATCH_GRAB_LIGHT_05 )
	{	LightNum =  5; }
	else if ( (Mode&BATCH_GRAB_LIGHT_04)==BATCH_GRAB_LIGHT_04 )
	{	LightNum =  4; }
	else if ( (Mode&BATCH_GRAB_LIGHT_03)==BATCH_GRAB_LIGHT_03 )
	{	LightNum =  3; }
	else if ( (Mode&BATCH_GRAB_LIGHT_02)==BATCH_GRAB_LIGHT_02 )
	{	LightNum =  2; }
	else if ( (Mode&BATCH_GRAB_LIGHT_01)==BATCH_GRAB_LIGHT_01 )
	{	LightNum =  1; }

	if ( (Mode&BATCH_GRAB_3D_CAST_01)==BATCH_GRAB_3D_CAST_01 )
	{	ProjA = true; }
	else
	{	ProjA = false; }
	if ( (Mode&BATCH_GRAB_3D_CAST_02)==BATCH_GRAB_3D_CAST_02 )
	{	ProjB = true; }
	else
	{	ProjB = false; }
	if ( (Mode&BATCH_GRAB_3D_CAST_03)==BATCH_GRAB_3D_CAST_03 )
	{	ProjC = true; }
	else
	{	ProjC = false; }
	if ( (Mode&BATCH_GRAB_3D_CAST_04)==BATCH_GRAB_3D_CAST_04 )
	{	ProjD = true; }
	else
	{	ProjD = false; }

	Step = 0;
	if ( (Mode&BATCH_GRAB_STEP_1)==BATCH_GRAB_STEP_1 )
	{	Step = 1; }	
	else if ( (Mode&BATCH_GRAB_STEP_4)==BATCH_GRAB_STEP_4 )
	{	Step = 4; }	

	nPhase = 0;
	if ( (Mode&BATCH_GRAB_PHASE_1)==BATCH_GRAB_PHASE_1 )
	{	nPhase = 1; }	
	else if ( (Mode&BATCH_GRAB_PHASE_2)==BATCH_GRAB_PHASE_2 )
	{	nPhase = 2; }	
	else if ( (Mode&BATCH_GRAB_PHASE_M)==BATCH_GRAB_PHASE_M )
	{	nPhase = 3; }	
	else if ( (Mode&BATCH_GRAB_PHASE_M2)==BATCH_GRAB_PHASE_M2 )
	{	nPhase = 4; }		
	return true;
}
//-------------------------------------------------------------------------------------//
CAMERA_ID CCameraCtrl::GetBatchGrabCameraID() const//取得批次取像相機
{
	return this->m_BatchGrabCameaID;
}
//-------------------------------------------------------------------------------------//
DWORD CCameraCtrl::GetBatchGrabMode() const//取得批次取像模式
{
	return this->m_BatchGrabMode;
}
//-------------------------------------------------------------------------------------//
void CCameraCtrl::SetBatchGrabStep(BATCH_GRAB_STEP val)//設定目前步驟
{
	this->m_BatchGrabStep = val;
}
//-------------------------------------------------------------------------------------//
BATCH_GRAB_STEP CCameraCtrl::GetBatchGrabStep() const//取得目前步驟
{
	return this->m_BatchGrabStep;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabPrepare(CAMERA_ID CameraID, DWORD GrabMode, int Period_us, int Exposure_us)//準備批量取像
{
#ifndef OFFLINE_VERSION
	int CameraPeriodus = 30000;
	int DLPPeriod_us = Period_us;//us
	int DLPExposure_us = Exposure_us;//us
	int DLPLEDColor = DLP_LED_COLOR_WHITE;
	const int CameraExposureTime_us = Exposure_us;
	//GetTriggerPeriodMinTime
#ifdef CAMERA_LIST_USE
		CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
		if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
		CameraPtr->SetCameraCallbackTimming(CAMERA_CALLBACK_BATCH_GRAB_DONE);
		CameraPtr->ClearCountAll();
		CameraPeriodus = CameraPtr->GetPeriodTime();			
		if ( CameraPtr->SetExposureTime(CameraExposureTime_us) == false )
		{	
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_EXP_TIME_FUNC);
			return false;
		}
		if ( CameraPtr->SwitchCameraGrabMode(CAMERA_GRAB_EXTERNAL_TRIGGER) == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_MODE);
			return false;
		}
		/*
		if ( CameraPtr->ResetCameraRingBuffer() == false )
		{	
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RING_BUFFER);
			return false;
		}
		*/
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
	//相位投光設定		
	this->m_BatchGrabCameaID = CameraID;
	this->m_BatchGrabMode = GrabMode;
	this->m_BatchGrabExposure_us = CameraExposureTime_us;
	this->m_BatchGrabStep = this->GetBatchGrabFirstStep(GrabMode);
	if ( Light3DCtrl.ExecAllLight3DSequenceStop() == false )
	{	
		this->m_ErrorString = Light3DCtrl.GetErrorString();
		return false; 
	}	

	const bool bForce = false;
	const bool bSleep = true;	
	const bool DLPMultiTable = LightCtrlBoard.GetDLPMultiTableEnabled();
	const bool DLPInternalTrigger = LightCtrlBoard.GetDLPInteralTrigger();
	int LightNum = 0, Step=0, PhaseNum=0, NFrames=0;
	bool Cast3D_01=false, Cast3D_02=false, Cast3D_03=false, Cast3D_04=false;
	int PhasePatMode = DLP_PATTERN_SEQUENCE_DEBUG;
	if ( this->BatchGrabModeDecode(GrabMode, LightNum, Cast3D_01, Cast3D_02, Cast3D_03, Cast3D_04, Step, PhaseNum) == false )
	{	return false; }	
	switch ( Step )
	{
	case 0:
		NFrames = 1;
		PhasePatMode = DLP_PATTERN_SEQUENCE_NONE;	
		break;
	case 1:
		switch ( PhaseNum )
		{
		case 3:	
			NFrames = 3;
			PhasePatMode = DLP_PATTERN_SEQUENCE_RGB;	
			break;
		default:
			NFrames = 1;
			PhasePatMode = DLP_PATTERN_SEQUENCE_WHITE;	
			break;
		}		
		break;	
	case 4:
		switch ( PhaseNum )
		{
		case 1:	
			NFrames = 4;
			PhasePatMode = DLP_PATTERN_SEQUENCE_4_4_1;	
			break;			
		case 2:	
			NFrames = 4;
			PhasePatMode = DLP_PATTERN_SEQUENCE_4_4_2;	
			break;		
		case 3:	
			NFrames = 8;
			PhasePatMode = DLP_PATTERN_SEQUENCE_4_4_M;
			break;		
		}
		break;		
	}	
	if ( DLP_PATTERN_SEQUENCE_DEBUG == PhasePatMode )
	{
		this->m_ErrorString.Format(_T("Error, Phase Pattern Sequence Exception"));
		SetCameraExceptionCode_Param();
		return false;
	}

	if ( true == Cast3D_01 )
	{		
		LIGHT_3D_CLS_PTR Light3DPtr = this->GetBatchGrabLightCtrlPtr(BATCH_GRAB_STEP_3D_CAST_01);
		if ( NULL==Light3DPtr)
		{	return false;	}
		DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
		DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;	
		DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
		if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
		if ( Light3DPtr->ExecDLPPattern_Run() == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
	}
	if ( true == Cast3D_02 )
	{
		LIGHT_3D_CLS_PTR Light3DPtr = this->GetBatchGrabLightCtrlPtr(BATCH_GRAB_STEP_3D_CAST_02);
		if ( NULL==Light3DPtr)
		{	return false;	}
		DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
		DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;	
		DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
		if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
		if ( Light3DPtr->ExecDLPPattern_Run() == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
	}

	if ( true == Cast3D_03 )
	{
		LIGHT_3D_CLS_PTR Light3DPtr = this->GetBatchGrabLightCtrlPtr(BATCH_GRAB_STEP_3D_CAST_03);
		if ( NULL==Light3DPtr)
		{	return false;	}
		DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
		DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;
		DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
		if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
		if ( Light3DPtr->ExecDLPPattern_Run() == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
	}

	if ( true == Cast3D_04 )
	{
		LIGHT_3D_CLS_PTR Light3DPtr = this->GetBatchGrabLightCtrlPtr(BATCH_GRAB_STEP_3D_CAST_04);
		if ( NULL==Light3DPtr)
		{	return false;	}
		DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
		DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;
		DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
		if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
		if ( Light3DPtr->ExecDLPPattern_Run() == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
	}
#endif	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabStart(bool &bFinish)//開始批量取像
{
#ifndef OFFLINE_VERSION
	int       CameraExposureTimeus = 0;
	CAMERA_ID CameraID = this->m_BatchGrabCameaID;
	DWORD GrabMode = this->m_BatchGrabMode;
	BATCH_GRAB_STEP GrabStep = this->m_BatchGrabStep;
	if ( BATCH_GRAB_STEP_NULL == GrabStep ) 
	{	
		bFinish = true;
		return true; 
	}
	const long NFrames = this->GetBatchGrabFrames(GrabMode, GrabStep);
	LIGHT_3D_CLS_PTR PhasePtr = this->GetBatchGrabLightCtrlPtr(GrabStep);
	if ( 0 == NFrames )
	{	return false;	}
	
	CCameraCtrl::ResetBatchGrabFinishEvent();
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	CameraExposureTimeus = CameraPtr->GetExposureTime();
	CameraPtr->SetCountForFramesToGrab(NFrames);
	CameraPtr->SetCameraCallbackTimming(CAMERA_CALLBACK_BATCH_GRAB_DONE);
	if ( CameraPtr->SetCountForBatchGrab(NFrames) == false )
	{	
		this->m_ErrorString = CameraPtr->GetCameraErrorString(); 
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_SET_FUNC);
		return false;
	}
	if ( CameraPtr->StartCameraLiveGrab() == false )
	{	
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_START);
		return false;
	}	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE

	this->m_BatchGrabStep = this->GetBatchGrabNextStep(this->m_BatchGrabMode, this->m_BatchGrabStep);
	if ( BATCH_GRAB_STEP_NULL == m_BatchGrabStep ) 
	{	bFinish = true;	}

	if ( NULL != PhasePtr )
	{
		if ( PhasePtr->ExecDLPPattern_Run()  == false )
		{
			this->m_ErrorString = PhasePtr->GetErrorString();
			return false;
		}
	}
#endif	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::FillCameraImage(CAMERA_ID CameraID, IMAGE_DISPLAY_MODE ImageMode, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr)//依據顯示模式來取回影像資料
{
#ifndef OFFLINE_VERSION
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->FillCameraImage(ImageMode, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_SET_FUNC);
		return false;
	}	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::FillCameraImage3(CAMERA_ID CameraID, IMAGE_DISPLAY_MODE ImageMode, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR ImagePtr)//依據顯示模式來取回影像資料
{
#ifndef OFFLINE_VERSION
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->FillCameraImage3(ImageMode, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_SET_FUNC);
		return false;
	}	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchIndexReset()//復歸批量取像
{
	m_BatchSliceParamIndex = 0;
	m_BatchSliceParamIndex_Next3D= 0;	
	return true;
}
//-------------------------------------------------------------------------------------//
const std::vector<TSliceParam>& CCameraCtrl::GetBatchParamList()//取回批量取像參數
{	
	return m_BatchSliceParamList;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabPrepare2(const std::vector<TSliceParam> &ParamList, bool ResetCamera)//準備批量取像
{
	size_t i=0;
	m_BatchSliceParamIndex = -1;
	m_BatchSliceParamIndex_Next3D= -1;
	m_BatchSliceParamList_Next3D.clear();
	m_BatchSliceParamList = ParamList;////批次取像列表
	std::vector<TSliceParam>  BatchSliceParamList=m_BatchSliceParamList;
	const size_t ParamCount = BatchSliceParamList.size();
	if ( 0 == ParamCount ) { return true; }	
	BatchIndexReset();

	SLICE_FUNC_MODE   SliceFuncMode;
	TSliceParam      *SliceParamPtr = NULL;
	TSliceParam       SliceParam = BatchSliceParamList[0];
	int Period_us   = SliceParam.SliceCameraExpTimeus;
	int Exposure_us = SliceParam.SliceCameraExpTimeus;
	const CAMERA_ID CameraID = SliceParam.SliceCameraID;
	int MaxExposure_us[MAX_CAMERA_COUNT] = {0};	
	
	::memset(MaxExposure_us, 0x00, sizeof(MaxExposure_us));	
	for ( i=0; i<ParamCount; i++ )
	{
		SliceParamPtr = &(BatchSliceParamList[i]);
		if ( NULL == SliceParamPtr ) { continue; }
		//if ( MaxExposure_us > SliceParamPtr->SliceCameraExpTimeus ) { continue; }//20240624
		const int ExpTime = SliceParamPtr->SliceCameraExpTimeus;
		if ( MaxExposure_us[SliceParamPtr->SliceCameraID] < ExpTime )
		{	MaxExposure_us[SliceParamPtr->SliceCameraID] = ExpTime;	}	

		//加入第2打光的參數
		SliceFuncMode = SliceParamPtr->SliceFuncMode;
		const int DLPLightCnt=AOIDataCollect.CheckFrameLightCountBySliceFuncMode(SliceFuncMode);
		if ( 1 != DLPLightCnt )
		{	m_BatchSliceParamList_Next3D.push_back(BatchSliceParamList[i]);	}
	}

#ifndef LIGHT_CTRL_DISABLE	
	//Period_us = MaxExposure_us;
#endif//LIGHT_CTRL_DISABLE	

#ifndef OFFLINE_VERSION
	int CameraPeriodus = 30000;
	int DLPPeriod_us = Period_us;//us
	int DLPExposure_us = Exposure_us;//us
	int DLPLEDColor = DLP_LED_COLOR_WHITE;
	int CameraExposureTime_us = Exposure_us;		
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	CAMERA_EXPOSURE_MODE CameraExposureMode=CameraPtr->GetCameraExposureMode();
	if ( CAMERA_EXPOSURE_TIMED == CameraExposureMode )
	{	CameraExposureTime_us = MaxExposure_us[CameraID];	}
	CameraPtr->SetCameraCallbackTimming(CAMERA_CALLBACK_BATCH_GRAB_DONE);
	CameraPtr->ClearCountAll();
	CameraPeriodus = CameraPtr->GetPeriodTime();

	if ( true==ResetCamera || CAMERA_EXPOSURE_TIMED==CameraExposureMode )
	{
		if ( CameraPtr->SetExposureTime(CameraExposureTime_us) == false )
		{	
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_EXP_TIME_FUNC);
			return false;
		}
	}
	if ( true == ResetCamera )
	{
		if ( CameraPtr->SwitchCameraGrabMode(CAMERA_GRAB_EXTERNAL_TRIGGER) == false )
		{
			this->m_ErrorString = CameraPtr->GetCameraErrorString();
			SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_MODE);
			return false;
		}
	}
	/*
	if ( Camera_CoaxLink_Q_12A180F.ResetCameraRingBuffer() == false )
	{	
		this->m_ErrorString = Camera_CoaxLink_Q_12A180F.GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_RING_BUFFER);
		return false;
	}
	*/	
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE

#ifndef LIGHT_CTRL_DISABLE	
	/*
	TLCB_TRIG_TABLE LCBTable;
	std::vector<TLCB_TRIG_TABLE> TableList;
	int DLPIndex = -1;
	int DLPChannel=0;
	int DLPChannelMask = 0;
	int DLPMask = 0;//0x01|0x02|0x04|0x08;
	const bool bForce = false;
	const bool bSleep = false;	
	const bool DLPMultiTable = LightCtrlBoard.GetDLPMultiTableEnabled();
	const bool DLPInternalTrigger = LightCtrlBoard.GetDLPInteralTrigger();
	LIGHT_3D_CLS_PTR PhasePtr = NULL;
	LIGHT_3D_CAST_ID Light3DID = LIGHT_3D_CAST_00;

	if( LightCtrlBoard.ClearAll() == false )	
	{
		this->m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}
	if ( LightCtrlBoard.ConvertSliceParamListToLCBTableList(BatchSliceParamList, TableList, DLPMultiTable) == false )
	{
		this->m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}
	const int TriggerTableCount = (int)(TableList.size());
	for ( i=0; i<TriggerTableCount; i++ )
	{
		LCBTable = TableList[i];
		if ( LIGHT_DLP != LCBTable.sTableType )	{	continue; }
		DLPChannelMask = CLightCtrlBoard::GetTableDLPChannelMask(LCBTable.sDLPActiveChannel);
		if ( 0 == (DLPMask&DLPChannelMask) ) 
		{	DLPMask |= DLPChannelMask;	}
		else//開啟過了
		{	continue;	}

		Light3DID = CLight3DCtrl::GetLight3DCastIDByDLPChannel(LCBTable.sDLPActiveChannel);		
		PhasePtr = Light3DCtrl.GetLight3DCastPtr(Light3DID);
		if ( NULL == PhasePtr ) 
		{
			this->m_ErrorString = Light3DCtrl.GetErrorString();
			return false;
		}		
		DLPIndex = LCBTable.sDLPActiveChannel-1;		
		const int NFrames = LCBTable.sDLPTrigOutNumber;
		const int PhasePatMode = LCBTable.sDLPPhasePatMode;		
		if ( DLP_PATTERN_SEQUENCE_DEBUG == PhasePatMode )	
		{
			this->m_ErrorString.Format(_T("Error, Phase Pattern Sequence Exception"));
			SetCameraExceptionCode_Param();
			return false;
		}		
		DLPPeriod_us = PhasePtr->GetDLPParam().m_PeriodTime_us;
		DLPExposure_us = PhasePtr->GetDLPParam().m_ExposureTime_us;		
		DLPLEDColor = PhasePtr->GetDLPParam().m_LEDColor;
		if ( PhasePtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )		
		{
			this->m_ErrorString = PhasePtr->GetErrorString();
			return false;
		}
		
		//Run
		if ( PhasePtr->ExecDLPPattern_Run()  == false )
		{
			this->m_ErrorString = PhasePtr->GetErrorString();
			return false;
		}					
	}	
	if ( false == bSleep )//如果DLP的設定沒有延遲時間的話
	{	JetAPI::TimeDelay_TickCount(50);	}
	
	if ( LightCtrlBoard.TableListWrite(TableList) == false )
	{
		this->m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}	

	const int CheckTable = LightCtrlBoard.GetTableRecheckEnabled();
	if ( FN_ENABLE == CheckTable )
	{
		::Sleep(50);		
		std::vector<int>             NGTableList;
		std::vector<TLCB_TRIG_TABLE> TableListOut;
		std::vector<TLCB_TRIG_TABLE> TableListIn=TableList;
		const size_t TableCountIn = TableListIn.size();
		if ( LightCtrlBoard.TableListRead(TableListOut, TableCountIn) == false ) 
		{
			this->m_ErrorString = LightCtrlBoard.GetErrorString();
			return false; 
		}
		if ( LightCtrlBoard.CheckTableListEqually(TableListIn, TableListOut, NGTableList) == false )
		{
			CString tmpfile;
			tmpfile.Format(_T("%s\\LightCtrlTableFault.TXT"), AOIDataCollect.GetAOITempDirectory());
			LightCtrlBoard.WriteTableListToFile(tmpfile, TableListOut, NGTableList);
			::ShellExecute(NULL, _T("open"), tmpfile, NULL, NULL, SW_SHOW);
			this->m_ErrorString = LightCtrlBoard.GetErrorString();
			return false; 
		}
	}

	if ( LightCtrlBoard.WriteTableRunCount(TriggerTableCount) == false )
	{
		this->m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}		
	
	if( LightCtrlBoard.SetMode_FPGA() == false )	
	{
		this->m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}
	*/
	if ( LightCtrlBoard.BuildLCBTableFromSliceParamList(BatchSliceParamList) == false )
	{
		m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}
	m_BatchSliceParamIndex = ParamCount-1;
#else
	//相位投光設定			
	if ( Light3DCtrl.ExecAllLight3DSequenceStop() == false )
	{	
		this->m_ErrorString = Light3DCtrl.GetErrorString();
		return false; 
	}
#endif//LIGHT_CTRL_DISABLE

#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabKernel2(bool bFirst, bool bBuildLCB, int nTriggerStart, bool bUsing3D, bool &bFinish, std::vector<TSliceParam> &SliceParamList, unsigned int &SliceParamIndex)//開始批量取像	
{
#ifndef OFFLINE_VERSION	
	size_t  i=0;	
	CString str;		
	const size_t ParamCount = SliceParamList.size();
	if ( 0==ParamCount || SliceParamIndex>=ParamCount ) 
	{
		bFinish = true;
		return true; 
	}
	long        NFrames = 0;		
	int         CameraExposureTimeus = 0;
	TSliceParam SliceParam = SliceParamList[0];
	const CAMERA_ID CameraID = SliceParam.SliceCameraID;	
#ifndef LIGHT_CTRL_DISABLE	
	NFrames = 0;
	for ( i=0; i<ParamCount; i++ )
	{
		const TSliceParam &SliceParamRef = SliceParamList[i];
		if ( false == bUsing3D )
		{	
			if ( SLICE_UNIQUE_ID_DLP == SliceParamRef.SliceUniqueID ) 
			{	continue; }
		}
		NFrames += SliceParamRef.SliceCameraFrames;
	}	
#else
	NFrames = SliceParam.SliceCameraFrames;
#endif//LIGHT_CTRL_DISABLE
	if ( 0 == NFrames )
	{	return false;	}
	
	CCameraCtrl::ResetBatchGrabFinishEvent();
#ifdef CAMERA_LIST_USE
	const long NFramesNew=NFrames;
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->WaitforCameraReadytoTrigger() == false )
	{	
		this->m_ErrorString = CameraPtr->GetCameraErrorString(); 
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_READY_TRIG_FUNC);
		return false;
	}
	if ( false == bFirst )
	{	NFrames += CameraPtr->GetCountForFramesToGrab();	}
	CameraExposureTimeus = CameraPtr->GetExposureTime();
	CameraPtr->SetCountForFramesToGrab(NFrames);
	CameraPtr->SetCameraCallbackTimming(CAMERA_CALLBACK_BATCH_GRAB_DONE);
	if ( CameraPtr->SetCountForBatchGrab(NFrames) == false )
	{	
		this->m_ErrorString = CameraPtr->GetCameraErrorString(); 
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_SET_FUNC);
		return false;
	}
	if ( CameraPtr->StartCameraLiveGrab() == false )
	{	
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_START);
		return false;
	}			
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#ifndef LIGHT_CTRL_DISABLE

	if ( true == bBuildLCB )
	{	
		if ( LightCtrlBoard.BuildLCBTableFromSliceParamList(SliceParamList) == false )
		{	
			m_ErrorString = LightCtrlBoard.GetErrorString();
			return false; 
		}
	}

	CString strCnt;	
	if ( LightCtrlBoard.ClearAllCount() == false )
	{
		this->m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}
	LightCtrlBoard.GetTriggerCountText(strCnt);
	if ( false == bBuildLCB )
	{
		const int TableRunCount = LightCtrlBoard.GetTableRunCount();
		if ( (NFramesNew!=TableRunCount) || (0!=nTriggerStart) )
		{	
			if ( LightCtrlBoard.SetTriggerFirstIndex(nTriggerStart) == false )
			{
				m_ErrorString = LightCtrlBoard.GetErrorString();
				return false;
			}
			//if ( LightCtrlBoard.WriteTableRunCount(NFrames) == false )
			if ( LightCtrlBoard.WriteTableRunCount(NFramesNew) == false )				
			{
				this->m_ErrorString = LightCtrlBoard.GetErrorString();
			}
			if( LightCtrlBoard.SetMode_FPGA() == false )	
			{
				this->m_ErrorString = LightCtrlBoard.GetErrorString();
				return false;
			}
		}
	}
	bool bDebug = false;
	if ( true == bDebug )
	{
		std::vector<TLCB_TRIG_TABLE> TableList;	
		//if ( LightCtrlBoard.ConvertSliceParamListToLCBTableList(SliceParamList, TableList) == false )
		if ( LightCtrlBoard.ConvertSliceParamListToLCBTableList(SliceParamList, bUsing3D, TableList) == false )
		{
			this->m_ErrorString = LightCtrlBoard.GetErrorString();
			return false;
		}
		const int TriggerTableCount = (int)(TableList.size());	
		const int TableRunCount = LightCtrlBoard.GetTableRunCount();
		if ( TableRunCount != TriggerTableCount )
		{
			if ( LightCtrlBoard.WriteTableRunCount(TriggerTableCount) == false )
			{
				this->m_ErrorString = LightCtrlBoard.GetErrorString();
				return false;
			}	
			if( LightCtrlBoard.SetMode_FPGA() == false )	
			{
				this->m_ErrorString = LightCtrlBoard.GetErrorString();
				return false;
			}
		}
	}	
	if ( LightCtrlBoard.TriggerStart() == false )
	{
		this->m_ErrorString = LightCtrlBoard.GetErrorString();
		return false;
	}	
	SliceParamIndex = ParamCount;
	bFinish = true;
#else	
	LIGHT_3D_CAST_ID Light3DCastID = LIGHT_3D_CAST_00;
	LIGHT_3D_CLS_PTR Light3DPtr = NULL;

	int DLPIndex = -1;
	for ( i=0; i<DLP_CAST_COUNT; i++ )
	{
		if ( FN_DISABLE == SliceParam.SliceLightTable.DLPCast[i].OnOffState )
		{	continue;	}
		DLPIndex = (int)(i);
		Light3DCastID = CLight3DCtrl::GetLight3DCastIDByIndex(i);		
		break;
	}
	Light3DPtr = Light3DCtrl.GetLight3DCastPtr(Light3DCastID);
	if ( NULL != Light3DPtr )
	{	
		int DLPPeriod_us = 0;
		int DLPExposure_us = 0;
		int DLPLEDColor = DLP_LED_COLOR_WHITE;
		const bool bForce = false;
		const bool bSleep = true;
		const bool DLPMultiTable = LightCtrlBoard.GetDLPMultiTableEnabled();
		const bool DLPInternalTrigger = LightCtrlBoard.GetDLPInteralTrigger();		
		const int PhasePatMode = SliceParam.SliceLightTable.DLPCast[DLPIndex].PhasePatMode;				
		if ( DLP_PATTERN_SEQUENCE_DEBUG == PhasePatMode )	
		{
			this->m_ErrorString.Format(_T("Error, Phase Pattern Sequence Exception"));
			SetCameraExceptionCode_Param();
			return false;
		}		
		DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
		DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;		
		DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
		if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )		
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
		
		//Run
		if ( Light3DPtr->ExecDLPPattern_Run()  == false )
		{
			this->m_ErrorString = Light3DPtr->GetErrorString();
			return false;
		}
	}
	SliceParamIndex ++;	
	if ( SliceParamIndex == ParamCount )
	{	bFinish = true;	}
#endif//LIGHT_CTRL_DISABLE
	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabStart2(bool &bFinish)//開始批量取像	
{
	const bool bUsing3D=true;
	if ( BatchGrabStart2(bFinish, bUsing3D) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabStart2(bool &bFinish, bool bUsing3D)//開始批量取像	
{
#ifndef OFFLINE_VERSION	
	bool         bBuildLCB=true;
	int          nTriggerStart = 0;	
	const bool   bFirst = true;
	const bool   bNeedGrabNext3DImage = CheckNeedGrabNext3DImage();
	const bool   bCanModifyTriggerStartIndex = LightCtrlBoard.GetSupportModifyTriggerFirstIndex();
	if ( true==bCanModifyTriggerStartIndex || false==bNeedGrabNext3DImage )
	{	bBuildLCB = false;	}
	else
	{	bBuildLCB = true;	}
	if ( true == bNeedGrabNext3DImage )
	{
		if ( Light3DCtrl.SetAllLight3DLEDCurrentID(DLP_LED_CURRENT_ID_01) == false )
		{
			m_ErrorString = Light3DCtrl.GetErrorString();
			return false;
		}
	}
	if ( BatchGrabKernel2(bFirst, bBuildLCB, nTriggerStart, bUsing3D, bFinish, m_BatchSliceParamList, m_BatchSliceParamIndex) == false )
	{	return false; }	
	SetBatchGrabbing(true);
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::BatchGrabNext3DImage(bool &bFinish)//下一次批量取像	
{
#ifndef OFFLINE_VERSION	
	bool         bBuildLCB=true;
	const bool   bUsing3D =true;
	const bool   bFirst = false;
	const bool   bNeedGrabNext3DImage = CheckNeedGrabNext3DImage();
	const int    nTriggerStart = LightCtrlBoard.GetDLPTableStartIndex();
	const bool   bCanModifyTriggerStartIndex = LightCtrlBoard.GetSupportModifyTriggerFirstIndex();	
	if ( false == bNeedGrabNext3DImage )
	{
		bFinish = true;
		return true;
	}
	if ( true == bCanModifyTriggerStartIndex )
	{	bBuildLCB = false;	}
	else
	{	bBuildLCB = true;	}
	if ( Light3DCtrl.SetAllLight3DLEDCurrentID(DLP_LED_CURRENT_ID_02) == false )
	{
		m_ErrorString = Light3DCtrl.GetErrorString();
		return false;
	}

	if ( SetBatchGrabExpourseTime(m_BatchSliceParamList_Next3D) == false )//CAMERA_EXPOSURE_TIMED
	{	return false; }
	if ( BatchGrabKernel2(bFirst, bBuildLCB, nTriggerStart, bUsing3D, bFinish, m_BatchSliceParamList_Next3D, m_BatchSliceParamIndex_Next3D) == false )
	{	return false; }
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::CheckNeedGrabNext3DImage()//確定是否需要取下一次批量
{
#ifndef OFFLINE_VERSION	
	const size_t Index = m_BatchSliceParamIndex_Next3D;
	const size_t Count = m_BatchSliceParamList_Next3D.size();
	if ( 0==Count || Index>=Count )
	{	return false; }
	return true;
#endif//OFFLINE_VERSION
	return false;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::RetrieveCameraImageCallback(CAMERA_ID CameraID, bool &bReturn)//接收相機影像回傳, 程式整理
{
#ifndef OFFLINE_VERSION	
	bReturn = false;
	SetCameraToSendCallback(CameraID, FALSE);
	KeepCameraTempRingBuffer(CameraID);
	IncrementCameraCopyToHostCount(CameraID);	
	SetCameraToSendCallback(CameraID, TRUE);
	FreeCameraTempRingBuffer(CameraID);
	if ( CheckNeedGrabNext3DImage() == true )//2次打光
	{
		bool bFinish = false;
		if ( BatchGrabNext3DImage(bFinish) == false )
		{	return false;	}
		bReturn=true;
		return true; 		
	}
	ResetBatchGrabbing();
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::SetBatchGrabExpourseTime(const std::vector<TSliceParam> &ParamList)//設定批量取像曝光時間
{	//return true;
#ifndef OFFLINE_VERSION
	int MaxExpouseTime[MAX_CAMERA_COUNT]={0};
	const size_t ParamCount = ParamList.size();
	::memset(MaxExpouseTime, 0x00, sizeof(MaxExpouseTime));
	for ( size_t i=0; i<ParamCount; i++ )
	{
		const TSliceParam &rParam=ParamList[i];
		CAMERA_ID CameraID = rParam.SliceCameraID;
		int ExpTime = rParam.SliceCameraExpTimeus;
		if ( MaxExpouseTime[CameraID] < ExpTime )
		{	MaxExpouseTime[CameraID] = ExpTime;	}
	}

	bool bSucc = true;
	CString ErrString;
	for ( size_t i=0; i<MAX_CAMERA_COUNT; i++ )
	{		
		const int ExpTime = MaxExpouseTime[i];
		if ( 0 == ExpTime ) { continue; }
		CAMERA_ID CameraID = (CAMERA_ID)(i);		
		CAMERA_EXPOSURE_MODE CameraExposureMode=GetCameraExposureMode(CameraID);
		if ( CAMERA_EXPOSURE_TIMED != CameraExposureMode ) { continue; }
		if ( ExpTime == GetCameraExposureTime(CameraID) )
		{	continue; }
		if ( SetCameraExposureTime(CameraID, ExpTime) == false )
		{	
			bSucc = false;	
			ErrString = m_ErrorString;
		}
	}

	if ( false == bSucc )
	{
		m_ErrorString = ErrString;
		return false;
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CCameraCtrl::WaitForCameraImageCallbackCount(CAMERA_ID CameraID, long ImageCount)//等待相機影像回來
{
#ifndef CAMERA_OBJ_DISABLE
#ifdef CAMERA_LIST_USE
	CCamera_Basic *CameraPtr=GetCameraPtrByCameraID(CameraID);
	if ( CheckCameraPtr(CameraPtr) == false ) { return false; }
	if ( CameraPtr->WaitForCameraImageCallbackCount(ImageCount) == false )
	{
		this->m_ErrorString = CameraPtr->GetCameraErrorString();
		SetCameraExceptionCode(AOI_EXCEPTION_CAMERA_GRAB_DONE_FUNC);
		return false;
	}
	ResetBatchGrabbing();
#else
	return ReturnNoUseCameraList();	
#endif//CAMERA_LIST_USE
#endif//CAMERA_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
