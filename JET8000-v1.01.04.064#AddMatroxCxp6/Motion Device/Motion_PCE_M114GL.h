// Motion_PCE_M114GL.h: interface for the CMotion_PCE_M114GL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTION_PCE_M114GL_H__7EB8874B_52CE_4705_B5A4_B02E5F557185__INCLUDED_)
#define AFX_MOTION_PCE_M114GL_H__7EB8874B_52CE_4705_B5A4_B02E5F557185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Motion_Basic.h"
//-------------------------------------------------------------------------------------//
#if MOTION_DERIVE_MODE == MOTION_PCE_M114GL
//使用Syn Tech PCI_M114GL軸控卡, 所有的單位皆為pulse
//---------------------------------------------------------------------------------------//
#ifndef MOTION_OBJ_DISABLE
	#ifdef _X64
		#pragma comment(lib, "..\\JET8000_Library\\PCE_M114GL\\x64\\Lib\\PCI_M114_x64.lib")
	#else
		#pragma comment(lib, "..\\JET8000_Library\\PCE_M114GL\\x32\\Lib\\PCI_M114.lib")
	#endif//_X64
#endif

#include "..\\JET8000_Library\\PCE_M114GL\\Include\\type_def.h"
#include "..\\JET8000_Library\\PCE_M114GL\\Include\\PCI_M114.h"
//---------------------------------------------------------------------------------------//
class CMotion_PCE_M114GL : public CMotion_Basic  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CMotion_PCE_M114GL)
	//---------------------------------------------------------------------------------//	
protected:	
	//----------------------------------------------------------------------------------//
	U16                        m_CardNo;	
	//----------------------------------------------------------------------------------//	
	U16                        GetCardNo() const;
	void                       SetCardNo(U16 val);
	//----------------------------------------------------------------------------------//
	virtual bool               BuildMotionNodeList();//建立運動點列表
	virtual bool               BuildMotionAxisList();//建立運動軸列表	
	//----------------------------------------------------------------------------------//
	bool                       CheckReturnOK(I16 ret);
	//----------------------------------------------------------------------------------//
	bool                       ReleaseMotionCard();
	CString                    GetCardTypeText(int val);	
	bool                       InitializeForJET7300();//JET7300的初始化
	bool                       InitializeForJET6500();//JET6500的初始化
	bool                       InitializeForJET8000();//JET8000的初始化
	bool                       InitializeForJET7500TB();//JET7500TB的初始化
	//----------------------------------------------------------------------------------//
	bool                       InitCallbackFunction();
	bool                       SetCallBackFunction(void ( __stdcall *callbackAddr)(I16 IntAxisNoInCard));
	//----------------------------------------------------------------------------------//
	bool                       OneAxisMoveTo(U16 Axis, F64 Dist, F64 StrVel, F64 MaxVel,F64 Tacc,F64 Tdec, MOVE_CURVE_MODE VelCurve, MOVE_COORDINATE_MODE CoordMode, F64 SVacc=-1, F64 SVdec=-1);
	//----------------------------------------------------------------------------------//	
	bool                       FreeRun(U16 Axis, F64 StrVel, F64 MaxVel, F64 Tacc);
	//----------------------------------------------------------------------------------//	
	bool                       SetORG(U16 Axis);
	bool                       SetORGAll();	
	bool                       SetORG_Internal(U16 CardNo, U16 CardAxis);
	//----------------------------------------------------------------------------------//
	bool                       Enable_Internal(U16 CardNo, U16 CardAxis);
	bool                       Disable_Internal(U16 CardNo, U16 CardAxis);	
	//----------------------------------------------------------------------------------//
	bool                       DoFaultAck_Internal(U16 CardNo, U16 CardAxis);
	//----------------------------------------------------------------------------------//
	bool                       EnableSoftwareLimit_Internal(U16 CardNo, U16 CardAxis);//啟用軟體極限
	bool                       DisableSoftwareLimit_Internal(U16 CardNo, U16 CardAxis);//關閉軟體極限
	//----------------------------------------------------------------------------------//
	bool                       Stop(U16 Axis);
	//----------------------------------------------------------------------------------//
	void                       GetErrorCodeText(int status, CString &str);//取得錯誤描述
	LPCTSTR                    GetIOText(int IO);//取得IO的文字
	int                        GetAxisStatus(U16 Axis, CString &str);//回傳該軸狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字	
	//----------------------------------------------------------------------------------//
	bool                       ForwardTriggerProcess_JET7000S();//正向移動
	bool                       BackwardTriggerProcess_JET7000S();//逆向移動	
	//----------------------------------------------------------------------------------//
	bool                       ResetMotionCardCommandPos(int Axis);//重設軸控卡的命令位置
	//----------------------------------------------------------------------------------//		
	bool                       ExecHome(int Axis);
	bool                       ExecEnable(int Axis);
	bool                       ExecDisable(int Axis);
	bool                       ExecSetIsWaitForInPosition(int Axis, bool Iswait);
	bool                       ExecWaitForDone(int Axis, int MaxPreCounts);	
	bool                       ExecMoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity);
	bool                       ExecTriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity);//觸發移動至哪裡
	bool                       ExecXYMoveTo(double PosX, double PosY, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity);//2軸移動，但非同動唷
	bool                       ExecXYZMoveTo(double PosX, double PosY, double PosZ, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity);//3軸移動，但非同動唷
	bool                       ExecDoFaultAck(int Axis);
	//----------------------------------------------------------------------------------//
public:
	CMotion_PCE_M114GL();
	virtual ~CMotion_PCE_M114GL();
	//----------------------------------------------------------------------------------//
	virtual  bool              PreInitMotion();//預先初始化
	virtual  bool              InitialMotion();//初始化	
	virtual  bool              ReleaseMotion();//釋放資源

	virtual bool               SaveMotionParamInternal();//儲存運動內部參數
	virtual bool               GetMotionIsReady();//取得運動系統是否正常
	virtual bool               Home(int Axis);
	virtual bool               Enable(int Axis);
	virtual bool               Disable(int Axis);
	virtual bool               SetIsWaitForInPosition(int Axis, bool Iswait);//設定是否等待定位停止
	virtual bool               WaitForDone(int Axis, int MaxPreCounts=2000);	
	virtual bool               MoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity=true);
	virtual bool               TriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity=true);//觸發移動至哪裡
	virtual bool               XYMoveTo(double PosX, double PosY, bool Offline=false, MOTION_MOVING_MODE MovingMode=MOTION_MOVING_NORMAL, bool IsModifyVelocity=true);//2軸移動，但非同動唷
	virtual bool               XYZMoveTo(double PosX, double PosY, double PosZ, bool Offline=false, MOTION_MOVING_MODE MovingMode=MOTION_MOVING_NORMAL, bool IsModifyVelocity=true);//3軸移動，但非同動唷
	virtual bool               GetEncode(int Axis, double &Encode, bool offline=false);//取得該軸的光學尺座標
	virtual bool               GetCurrentPos(double &X, double &Y, bool offline=false);//取得目前機台位置
	virtual bool               GetCurrentPos(double &X, double &Y, double &Z, bool offline=false);//取得目前機台位置
	virtual bool               GetCurrentRawPos(double &X, double &Y, double &Z, bool offline=false);//取得目前機台原始位置
	virtual bool               StartFreeRun(int Axis, bool Dir);//Dir +為正方向, -為負方向
	virtual bool               StopFreeRun(int Axis);//停止FreeRun
	virtual bool               ForwardTriggerProcess();//正向移動
	virtual bool               BackwardTriggerProcess();//逆向移動
	virtual bool               EnableCompareTrigger(bool IsEnable);//是否啟動同步比較送外部觸發訊號
	virtual bool               StopCompareTrigger(bool IsStop);//是否停止同步比較送外部觸發訊號
	virtual void               SetTriggerParameter(double SP, double EP, double Start, double End, double Interval, int RepeatCounts, int YMaxCounts, double YOffset, int ORGX, int ORGY, int ORGZ);
	virtual bool               ConfigTriggerTable(bool IsReBuild);
	
	virtual bool               GetIsEnable(int Axis);//該軸是否為Serve ON, 也就是有送電來積磁
	virtual bool               GetIsReady(int Axis);//Driver傳回該軸是否為RDY狀態
	virtual bool               GetIsAlarm(int Axis);//Driver傳回該軸是否為Alarm狀態
	virtual bool               GetIsERCActive(int Axis);//ERC Active
	virtual bool               GetIsInPosition(int Axis);//Driver傳回該軸是否為In Position
	virtual bool               GetIsNLimit(int Axis);//該軸的是否碰觸到副極限
	virtual bool               GetIsPLimit(int Axis);//該軸的是否碰觸到正極限		
	virtual bool               GetIsORG(int Axis);//該軸是否在原點位置
	virtual bool               GetIsEmergencyOn(int Axis);//該軸是否收到急停訊號
	virtual bool               DoFaultAck(int Axis);
	virtual const TCHAR*       GetAxisStatus(int Axis);
	virtual bool               ResetMotionDriver();//重新復歸運動的Driver
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
			
	virtual int                GetIOStatus(int Axis, LPTSTR Str);//回傳I/O狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字	

	virtual bool               FireSingleTrigger();//送出單一觸發訊號(包含燈源與相機)

	virtual bool               EnableSoftwareLimit(int Axis);//啟用軟體極限
	virtual bool               DisableSoftwareLimit(int Axis);//關閉軟體極限
	virtual bool               SetSoftwareLimit(int Axis, double Min, double Max, bool Auto);//設定軟體極限	
};
//-------------------------------------------------------------------------------------//
extern CMotion_PCE_M114GL  Motion_PCE_M114GL;
//-------------------------------------------------------------------------------------//
#endif//MOTION_DERIVE_MODE
#endif // !defined(AFX_MOTION_PCE_M114GL_H__7EB8874B_52CE_4705_B5A4_B02E5F557185__INCLUDED_)
