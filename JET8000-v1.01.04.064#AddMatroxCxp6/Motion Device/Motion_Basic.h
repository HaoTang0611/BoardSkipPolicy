// Motion_Basic.h: interface for the CMotion_Basic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTION_BASIC_H__03B1DAF8_99AE_4A79_98A6_3B41EE7E8870__INCLUDED_)
#define AFX_MOTION_BASIC_H__03B1DAF8_99AE_4A79_98A6_3B41EE7E8870__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//MOTION_OBJ_DISABLE
#include <vector>
#include "MotionAxis.h"
#include "Motion_Define.h"
//-------------------------------------------------------------------------------------//
class CMotion_Basic : public CObject  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CMotion_Basic)
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	static bool                SetMotionParameterStringByID(MOTION_PARAM_ID ParamID, TMotionParameter &MotionParam, LPCTSTR String);//設定運動參數
	static bool                GetMotionParameterStringByID(MOTION_PARAM_ID ParamID, const TMotionParameter &MotionParam, CString &String);//取得運動參數	
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	CRITICAL_SECTION           m_csMotion;
	CRITICAL_SECTION           m_csMotion_X;
	CRITICAL_SECTION           m_csMotion_Y;
	CRITICAL_SECTION           m_csMotion_Z;	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	std::vector<TScanPath>     m_ScanPathList;
	//---------------------------------------------------------------------------------//	
	MOTION_CARD_TYPE           m_CardType;
	MACHINE_MODEL_TYPE         m_MachineType;
	int                        m_MaxAxisCount;//最多軸數
	int                        m_UsedAxisCount;//使用軸數
	std::vector<CMotionNode>   m_MotionNodeList;
	std::vector<CMotionAxis>   m_MotionAxisList;
	CMotionAxis               *m_MotionAxisPtrX;
	CMotionAxis               *m_MotionAxisPtrY;
	CMotionAxis               *m_MotionAxisPtrZ;
	//---------------------------------------------------------------------------------//
	HWND                       m_MotionCallbackHWnd;	
	bool                       m_IsInitialize;	
	bool                       m_IsJogMode;
	bool                       m_IsSupportGantry;	
	CString                    m_ErrorString;//錯誤字串
	CString                    m_ErrorStringOut;//錯誤字串	
	CString                    m_MotionStatus;//運動系統的狀況
	CString                    m_MotionName;//運動系統名稱
	CString                    m_MotionIOStats;//運動IO狀態
	bool                       m_IsMotionRelease;
	int                        m_TriggerYMaxCounts;
	int                        m_TriggerRepeatCounts;
	int                        m_TriggerYCounts;
	DWORD                      m_WaitForDoneDelayTime;//等待移動完成的延遲時間-ms
	LANE_ID                    m_XYCaliLaneID;//XY校正表軌道編號
	bool                       m_StopXYCalibration;//停止XY校正
	std::vector<TXYCali>       m_XYCaliList;  //XY校正列表	
	TMotionParameter           m_MotionParameter;	
	//---------------------------------------------------------------------------------//	
	//觸發時的參數，除錯用
	MOTION_TRIGGER_MODE        m_TriggerStatus;//觸發行程的狀態
	int                        m_TriggerMaxRepeatCounts;	
	double                     m_TriggerStartPos; //移動的起點
	double                     m_TriggerEndPos;   //移動的終點
	double                     m_TriggerPosY;     //移動時的Y軸
	double                     m_TriggerFirstOnePos;    //第一個觸發點
	double                     m_TriggerLastOnePos;      //最後的觸發點
	double                     m_TriggerInterval;        //觸發的間距
	int                        m_TriggerNTriggers; //多少個處發點
	bool                       m_IsMotionStop;     //設定是否要機台停止
	int                        m_ORGX, m_ORGY, m_ORGZ;     //進行移動後的回歸位置
	bool                       m_IsScanForInspectionMode;       //是否為檢測掃描模式
	//---------------------------------------------------------------------------------//		
	THREAD_COMMAND_MODE        m_MotionThreadCmd_GoStop;
	THREAD_STATE_MODE          m_MotionThreadState_GoStop;	
	//---------------------------------------------------------------------------------//	
	bool                       m_MotionCtrlThreadDeleted;
	//----------------------------------------------------------------------------------//	
	unsigned int               m_MotionLogIndex;//運動的訊息檔案引數
	//----------------------------------------------------------------------------------//	
	bool                       m_CommandOffline;

	double                     m_FreeRunVel_X;
	double                     m_FreeRunVel_Y;
	double                     m_FreeRunVel_Z;
	//---------------------------------------------------------------------------------//		
	void                       SetInitialize(bool bInit);
	//----------------------------------------------------------------------------------//	
	int                        GetMaxAxisCount() const;
	void                       SetMaxAxisCount(int val);
	//---------------------------------------------------------------------------------//
	int                        GetUsedAxisCount() const;
	void                       SetUsedAxisCount(int val);
	//---------------------------------------------------------------------------------//
	virtual bool               BuildMotionNodeList();//建立運動點列表
	virtual bool               BuildMotionAxisList();//建立運動軸列表		
	virtual bool               CheckMotionAxisValid(CMotionAxis &Ref);//確認運動軸	
	virtual bool               CheckMotionNodeValid(const CMotionNode &Ref);//確認軸控節點	
	virtual bool               UpdateMotionAxisDriverModel(CMotionAxis &Ref);//更新運動軸的驅動器型號
	virtual bool               BuildMotionAxisListFn(const std::vector<CMotionNode> &NodeList, std::vector<CMotionAxis> &AxisList);//建立運動軸列表	
	//----------------------------------------------------------------------------------//	
	bool                       GetIsSupportGantry();
	void                       SetIsSupportGantry(bool bVal);		
	bool                       CheckNeedMoveXY(double PosX, double PosY, bool OfflineMode);
	bool                       CheckNeedMoveXYZ(double PosX, double PosY, double PosZ, bool OfflineMode);
	//---------------------------------------------------------------------------------//	
	CMotion_Basic(const CMotion_Basic &motion);
	CMotion_Basic& operator=(const CMotion_Basic &motion);
	//---------------------------------------------------------------------------------//
	void                       LockMotion(int Axis=-1);
	void                       UnlockMotion(int Axis=-1);
	//---------------------------------------------------------------------------------//	
	void                       SetMotionName(LPCTSTR val);//設定運動系統名稱
	void                       SetMotionCardType(MOTION_CARD_TYPE Type);//設定軸控卡樣式
	//---------------------------------------------------------------------------------//
	bool                       SaveMotionCardType(); 
	bool                       LoadMotionCardType(MOTION_CARD_TYPE &val);
	bool                       SaveMotionCardType(MOTION_CARD_TYPE val); 
	//---------------------------------------------------------------------------------//
	bool                       ExecLoadMotionParameter(LPCTSTR filename, TMotionParameter &Param);//載入運動參數
	bool                       ExecSaveMotionParameter(LPCTSTR filename, const TMotionParameter &Param);//讀取運動參數
	//---------------------------------------------------------------------------------//
	bool                       SaveMotionProcess(int Axis, LPCTSTR fnName, int Level);
	//---------------------------------------------------------------------------------//
	double                     GetAxisFreeRunVelocity(int Axis);//取得軸自由移動的速度
	double                     GetAxisFreeRunVelocity(CMotionAxis *MotionAxisPtr);//取得軸自由移動的速度
	//---------------------------------------------------------------------------------//
	bool                       CheckBitMask(int val, int mask);//確認位元比較
	//---------------------------------------------------------------------------------//
	bool                       ReturnNoSupportFunc(LPCTSTR fnName);
	//---------------------------------------------------------------------------------//
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
public:
	//----------------------------------------------------------------------------------//
	CMotion_Basic();
	virtual ~CMotion_Basic();
	//---------------------------------------------------------------------------------//		
	bool                       GetInint() const;
	bool                       CheckInit();	
	LPCTSTR                    GetMotionName() const;//取得運動系統名稱
	//---------------------------------------------------------------------------------//
	void                       SetMotionExceptionCode_Param(LPCTSTR Err=NULL);
	void                       SetMotionExceptionCode_FileRead(LPCTSTR Err=NULL);
	void                       SetMotionExceptionCode_FileWrite(LPCTSTR Err=NULL);
	void                       SetMotionExceptionCode(DWORD Code, LPCTSTR Err=NULL);	
	//---------------------------------------------------------------------------------//	
	MOTION_CARD_TYPE           GetMotionCardType() const;//取得軸控卡樣式
	//---------------------------------------------------------------------------------//
	MACHINE_MODEL_TYPE         GetMotionMachineType() const;//運動系統的機台樣式
	void                       SetMotionMachineType(MACHINE_MODEL_TYPE Type);//運動系統的機台樣式
	//---------------------------------------------------------------------------------//	
	CString                    GetMotionParamSectionName() const;//取得運動參數的區間名稱
	//---------------------------------------------------------------------------------//	
	bool                       LoadMotionParameter();//載入運動參數
	bool                       SaveMotionParameter();//讀取運動參數
	virtual bool               UpdateMotionParamter();//更新運動參數
	bool                       UpdateMotionParameterToMotionAxis();//更新運動參數至軸內
	bool                       LoadMotionParameter(LPCTSTR filename, TMotionParameter &Param);//載入運動參數
	bool                       SaveMotionParameter(LPCTSTR filename, const TMotionParameter &Param);//讀取運動參數
	//---------------------------------------------------------------------------------//
	void                       SetMotionParameter(const TMotionParameter &Param) { m_MotionParameter = Param; }
	virtual TMotionParameter&  GetMotionParameter() { return m_MotionParameter; }
	virtual const TMotionParameter&  GetMotionParameter() const { return m_MotionParameter; }
	TMotionParameter*          GetMotionParameterPtr() { return &m_MotionParameter; }
	//---------------------------------------------------------------------------------//	
	bool                       LoadMotionNodeList();
	bool                       SaveMotionNodeList();		
	CString                    GetMotionNodeListFilename() const;
	//---------------------------------------------------------------------------------//	
	std::vector<CMotionNode>&  GetMotionNodeList();
	std::vector<CMotionAxis>&  GetMotionAxisList();	
	//---------------------------------------------------------------------------------//	
	void                       ResetMotionAxisPtr();	
	bool                       CheckMotionNodePtr(const CMotionNode *Ptr);
	bool                       CheckMotionAxisPtr(CMotionAxis *Ptr);
	bool                       CheckMotionAxisPtr(const CMotionAxis *Ptr) const;
	CMotionAxis*               GetMotionAxisPtr(int Axis);	
	const CMotionAxis*         GetMotionAxisPtr(int Axis) const;	
	void                       SetMotionAxisPtr(int Axis, CMotionAxis *Ptr);
	//---------------------------------------------------------------------------------//
	int                        GetGantryAxis();//取得龍門軸
	//---------------------------------------------------------------------------------//
	virtual bool               PreInitMotion();//預先初始化
	virtual bool               InitialMotion()=0;//初始化
	virtual bool               ReleaseMotion()=0;//釋放掉		
	
	virtual bool               SaveMotionParamInternal()=0;//儲存運動內部參數
	virtual bool               GetMotionIsReady()=0;//取得運動系統是否正常 
	virtual bool               Home(int Axis)=0;//歸零
	virtual bool               Enable(int Axis)=0;//啟動
	virtual bool               Disable(int Axis)=0;//取消啟動
	virtual bool               SetIsWaitForInPosition(int Axis, bool Iswait)=0;//設定是否等待定位停止
	virtual bool               WaitForDone(int Axis, int MaxPreCounts=2000)=0;//等待停止	
	virtual bool               MoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity=true)=0;//移動至哪裡
	virtual bool               TriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity=true)=0;//觸發移動至哪裡
	virtual bool               XYMoveTo(double PosX, double PosY, bool Offline=false, MOTION_MOVING_MODE MovingMode=MOTION_MOVING_NORMAL, bool IsModifyVelocity=true)=0;//2軸移動，但非同動唷
	virtual bool               XYZMoveTo(double PosX, double PosY, double PosZ, bool Offline=false, MOTION_MOVING_MODE MovingMode=MOTION_MOVING_NORMAL, bool IsModifyVelocity=true)=0;//3軸移動，但非同動唷
	virtual bool               GetEncode(int Axis, double &Encode, bool offline=false)=0;//取得該軸的光學尺座標
	virtual bool               GetCurrentPos(double &X, double &Y, bool offline=false)=0;//取得目前機台位置
	virtual bool               GetCurrentPos(double &X, double &Y, double &Z, bool offline=false)=0;//取得目前機台位置
	virtual bool               GetCurrentRawPos(double &X, double &Y, double &Z, bool offline=false)=0;//取得目前機台原始位置
	virtual bool               StartFreeRun(int Axis, bool Dir)=0;//Dir +為正方向, -為負方向
	virtual bool               StopFreeRun(int Axis)=0;//停止FreeRun
	virtual bool               ForwardTriggerProcess()=0;//正向移動
	virtual bool               BackwardTriggerProcess()=0;//逆向移動
	virtual bool               EnableCompareTrigger(bool IsEnable)=0;//是否啟動同步比較送外部觸發訊號	
	virtual bool               StopCompareTrigger(bool IsStop)=0;//是否停止同步比較送外部觸發訊號
	virtual void               SetTriggerParameter(double SP, double EP, double Start, double End, double Interval, int RepeatCounts, int YMaxCounts, double YOffset, int ORGX, int ORGY, int ORGZ)=0;
	
	virtual bool               GetIsEnable(int Axis)=0;//該軸是否為Serve ON, 也就是有送電來積磁
	virtual bool               GetIsReady(int Axis)=0;//Driver傳回該軸是否為RDY狀態
	virtual bool               GetIsAlarm(int Axis)=0;//Driver傳回該軸是否為Alarm狀態
	virtual bool               GetIsERCActive(int Axis)=0;//ERC Active
	virtual bool               GetIsInPosition(int Axis)=0;//Driver傳回該軸是否為In Position
	virtual bool               GetIsNLimit(int Axis)=0;//該軸的是否碰觸到副極限
	virtual bool               GetIsPLimit(int Axis)=0;//該軸的是否碰觸到正極限	
	virtual bool               GetIsORG(int Axis)=0;//該軸是否在原點位置
	virtual bool               GetIsEmergencyOn(int Axis)=0;//該軸是否收到急停訊號
	bool                       GetIsLimit(int Axis);
	virtual bool               ConfigTriggerTable(bool IsReBuild)=0;
	virtual bool               DoFaultAck(int Axis)=0;
	virtual const TCHAR*       GetAxisStatus(int Axis)=0;
	virtual bool               ResetMotionDriver()=0;//重新復歸運動的Driver
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系

	bool                       CheckAxisBypass(int Axis);//確認軸跳過
	bool                       CheckAxisBypass(const CMotionAxis *MotionAxisPtr);//確認軸跳過
	bool                       CheckNodeBypass(const CMotionNode *MotionNodePtr);//確認軸跳過

	void                       SetIsJogMode(const bool Is);//設定是否可以為Jog模式
	bool                       GetIsJogMode() const;

	void                       SetErrorString(LPCTSTR str);//設定錯誤字串
	LPCTSTR                    GetErrorString();//取得錯誤字串
	LPCTSTR                    GetMotionStatus() const;//取得運動系統狀況
	
	void                       SetMotionCallbackHWnd(const HWND hWnd);
	HWND                       GetMotionCallbackHWnd() const;	
	bool                       PostMotionCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam);
	bool                       SendMotionCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam);
	
	double                     GetTriggerPosY();//取得觸發的起點的Y值
	void                       SetTriggerFirstPosY(const double SPY);	
	int                        GetTriggerProcess() const;  //取得觸發的狀態
	double                     GetTriggerStartPos() const; //取得觸發時的起點
	double                     GetTriggerEndPos() const;   //取得觸發時的終點
	double                     GetTriggerFirstOnePos() const;    //取得第一個觸發時的位置
	double                     GetTriggerLastOnePos() const;      //取得最後一個觸發時的位置
	double                     GetTriggerInterval() const; //取得觸發的間隔
	int                        GetNTriggers() const;       //取得有多少個觸發點
	void                       SetMotionIsStop(const bool IsStop);//設定是否運動停止
	bool                       GetMotionIsStop() const;    //取得是否運動停止
	bool                       GetIsTriggerRepeatFinish() const;//判斷是否應該要結束重複行走觸發		
	//---------------------------------------------------------------------------//	
	bool                       CheckIsHomed(int Axis); //確認是否歸零過
	bool                       CheckIsHomed(const CMotionAxis *MotionAxisPtr); //確認是否歸零過
	bool                       CheckIsEnabled(int Axis); //確認是否啟用過	
	bool                       CheckIsEnabled(const CMotionAxis *MotionAxisPtr); //確認是否啟用過		
	bool                       GetIsConvertSignPositive();//取得是否內部方向性轉換
	//---------------------------------------------------------------------------------//
	bool                       CheckFreeRunMoving(CMotionAxis *MotionAxisPtr);//確認FreeRun移動中	
	//---------------------------------------------------------------------------------//
	double                     ConvertAxisPos(int Axis, double Pos) const;//轉換軸位置	
	double                     ConvertAxisPos(const CMotionAxis *MotionAxisPtr, double Pos) const;//轉換軸位置	
	//---------------------------------------------------------------------------------//
	//離線命令
	bool                       GetCommandOffline();
	void                       SetCommandOffline(bool val);

	//命令位置
	double                     GetCommandPosX();
	void                       SetCommandPosX(double val);
	double                     GetCommandPosY();
	void                       SetCommandPosY(double val);
	double                     GetCommandPosZ();
	void                       SetCommandPosZ(double val);		
	double                     GetCommandPos(int Axis);
	void                       SetCommandPos(int Axis, double val);
	void                       SetCommandPos(double X, double Y, double Z);//設定機台命令位置
	bool                       GetCommandPos(double &X, double &Y, double &Z);//取得機台命令位置

	//位置誤差
	double                     GetPosTolerance(int Axis);
	double                     GetPosToleranceX();
	double                     GetPosToleranceY();
	double                     GetPosToleranceZ();

	//命令速度
	double                     GetCommandMaxVelocity_X();
	void                       SetCommandMaxVelocity_X(double val);
	double                     GetCommandMaxVelocity_Y();
	void                       SetCommandMaxVelocity_Y(double val);
	double                     GetCommandMaxVelocity_Z();
	void                       SetCommandMaxVelocity_Z(double val);	
	double                     GetCommandMaxVelocity(int Axis);
	void                       SetCommandMaxVelocity(int Axis, double val);	
	double                     GetMotionAxisVelocity(CMotionAxis *MotionAxisPtr, MOTION_MOVING_MODE Mode);

	//命令加速度時間
	double                     GetCommandAccelerationTim_X();
	void                       SetCommandAccelerationTim_X(double val);
	double                     GetCommandAccelerationTim_Y();
	void                       SetCommandAccelerationTim_Y(double val);
	double                     GetCommandAccelerationTim_Z();
	void                       SetCommandAccelerationTim_Z(double val);		
	double                     GetCommandAccelerationTime(int Axis);
	void                       SetCommandAccelerationTime(int Axis, double val);	
	//---------------------------------------------------------------------------//		
	virtual int                GetIOStatus(int AxisNo, LPTSTR Str)=0;//回傳I/O狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字	
	//----------------------------------------------------------------------------------//			
	bool                       CreateMotionCtrlThread();//建立運動控制執行緒
	bool                       DeleteMotionCtrlThread();//刪除運動控制執行緒
	bool                       GetMotionCtrlThreadDeleted() const;//取得是否刪除運動控制執行緒
	//----------------------------------------------------------------------------------//	
	bool                       StartMotionThreadStats_GoStop(bool WaitOn);//開始走停取像執行緒
	void                       SetMotionThreadState_GoStop(THREAD_STATE_MODE State);//設定走停取像執行緒狀態
	THREAD_STATE_MODE          GetMotionThreadState_GoStop() const;//取得走停取像執行緒狀態
	void                       SetMotionThreadCmd_GoStop(THREAD_COMMAND_MODE Cmd);//設定走停取像執行緒命令
	THREAD_COMMAND_MODE        GetMotionThreadCmd_GoStop() const;//取得走停取像執行緒命令
	bool                       ExecMotionThreadFn_GoStop();//執行走停取像執行緒
	bool                       ExecMotionThreadFn_GoStopFn();//執行走停取像執行緒
	bool                       WaitMotionThreadDone_GoStop(size_t MoveCount);//等待走取像停執行緒
	//----------------------------------------------------------------------------------//		
	virtual bool               FireSingleTrigger()=0;//送出單一觸發訊號(包含燈源與相機)
	//----------------------------------------------------------------------------------//		
	virtual bool               EnableSoftwareLimit(int Axis)=0;//啟用軟體極限
	virtual bool               DisableSoftwareLimit(int Axis)=0;//關閉軟體極限
	virtual bool               SetSoftwareLimit(int Axis, double Min, double Max, bool Auto)=0;//設定軟體極限
	//----------------------------------------------------------------------------------//
	virtual bool               CorrectGantryOffset(int Axis);//修正龍門偏差
	virtual bool               CalibrateGantryOffset(int Axis);//校正龍門偏差
	bool                       SetGantryStdOffset(int Axis, double Offset);//設定龍門的標準偏移值
	bool                       GetGantryStdOffset(int Axis, double &Offset);//取得龍門的標準偏移值
	virtual bool               FetchGantryOffset(int Axis, double &Offset);//取得龍門的偏移值	
	//----------------------------------------------------------------------------------//
	bool                       RegisterMotionCurrentProcessFile();//註冊運動的訊息檔案
	bool                       SaveMotionCurrentProcess(const char *String);
	bool                       SaveMotionCurrentProcess(const wchar_t *String);
	//----------------------------------------------------------------------------------//
	bool                       WaitForMotionStop(bool bDelay=true);//等運動系統停下來
	//----------------------------------------------------------------------------------//
	bool                       ExecLimit(int AxisNo);//確認極限範圍並且設定成軟體極限
	bool                       ExecLimitFn(int AxisNo);//確認極限範圍並且設定成軟體極限
	bool                       ExecLimitSearch(int AxisNo, double &MinLimit, double &MaxLimit);//確認極限範圍並且設定成軟體極限
	//----------------------------------------------------------------------------------//
	bool                       ExecJogMSG(UINT message, WPARAM wParam, LPARAM lParam);//執行Jog訊息
	//----------------------------------------------------------------------------------//
	bool                       ExecHomeAll(bool MoveToStartPos);//執行全部歸零
	//----------------------------------------------------------------------------------//
	bool                       CheckMotionReady(bool bChkEnb);//確認運動系統準備好	
	bool                       ExecCheckMotionReady(bool bChkEnb);//確認運動系統準備好	
	//----------------------------------------------------------------------------------//
	bool                       MoveToBeforePCBInPos();//移動至進板前位置
	//----------------------------------------------------------------------------------//
	//等待完畢延遲時間-ms
	void                       SetWaitForDoneDelayTime(DWORD Time_ms) { m_WaitForDoneDelayTime = Time_ms; }
	DWORD                      GetWaitForDoneDelayTime() const { return m_WaitForDoneDelayTime; }
	//----------------------------------------------------------------------------------//
	double                     GetMotionPCBStopPosX(LANE_ID LaneID, bool RightSide) const;//PCB停板位置
	void                       SetMotionPCBStopPosX(LANE_ID LaneID, bool RightSide, double val);//PCB停板位置
	double                     GetMotionPCBStopPosY(LANE_ID LaneID, bool RightSide) const;//PCB停板位置
	void                       SetMotionPCBStopPosY(LANE_ID LaneID, bool RightSide, double val);//PCB停板位置
	double                     GetMotionPCBStopPosZ(LANE_ID LaneID, bool RightSide) const;//PCB停板位置
	void                       SetMotionPCBStopPosZ(LANE_ID LaneID, bool RightSide, double val);//PCB停板位置
	//---------------------------------------------------------------------------------//
	double                     GetMotionLaneLedStopPosX(LANE_ID LaneID, bool RightSide);//軌道LED停板位置
	void                       SetMotionLaneLedStopPosX(LANE_ID LaneID, bool RightSide, double val);//軌道LED停板位置
	double                     GetMotionLaneLedStopPosY(LANE_ID LaneID, bool RightSide);//軌道LED停板位置
	void                       SetMotionLaneLedStopPosY(LANE_ID LaneID, bool RightSide, double val);//軌道LED停板位置
	double                     GetMotionLaneLedStopPosZ(LANE_ID LaneID, bool RightSide);//軌道LED停板位置
	void                       SetMotionLaneLedStopPosZ(LANE_ID LaneID, bool RightSide, double val);//軌道LED停板位置
	//---------------------------------------------------------------------------------//
	double                     GetMotionLaneLedSlowPosX(LANE_ID LaneID, bool RightSide);//軌道LED減速位置
	void                       SetMotionLaneLedSlowPosX(LANE_ID LaneID, bool RightSide, double val);//軌道LED減速位置
	double                     GetMotionLaneLedSlowPosY(LANE_ID LaneID, bool RightSide);//軌道LED減速位置
	void                       SetMotionLaneLedSlowPosY(LANE_ID LaneID, bool RightSide, double val);//軌道LED減速位置
	double                     GetMotionLaneLedSlowPosZ(LANE_ID LaneID, bool RightSide);//軌道LED減速位置
	void                       SetMotionLaneLedSlowPosZ(LANE_ID LaneID, bool RightSide, double val);//軌道LED減速位置
	//---------------------------------------------------------------------------------//	
	LANE_ID                    GetMotionXYCaliLaneID() const;//取得XY校正表軌道編號
	void                       SetMotionXYCaliLaneID(LANE_ID val);//設XY校正表軌道編號
	//---------------------------------------------------------------------------------//
	bool                       GetStopXYCalibration() const;//取得停止XY校正
	void                       SetStopXYCalibration(bool val);//設定停止XY校正
	//---------------------------------------------------------------------------------//
	virtual bool               SaveMotionXYCali();//儲存運動系統的XY校正表
	virtual bool               LoadMotionXYCali();//載入運動系統的XY校正表
	bool                       SaveMotionXYCali(LPCTSTR filename, std::vector<TXYCali> &XYCaliList);//儲存運動系統的XY校正表
	bool                       LoadMotionXYCali(LPCTSTR filename, std::vector<TXYCali> &XYCaliList);//儲存運動系統的XY校正表
	bool                       ChangeMotionXYCali();//變更運動系統的XY校正表
	bool                       InitMotionXYCali(TXYCali &XYCali);//初始化XY參數
	bool                       RearrangeMotionXYCali(TXYCali &XYCali);//排序XY內的四個端點
	bool                       CalcMotionXYCaliLimit(TXYCali &XYCali);//計算XY內的極限範圍
	bool                       CalcMotionXYCaliTransform(TXYCali &XYCali);//計算XY內的轉換公式
	virtual bool               SetMotionXYYCaliList(const std::vector<TXYCali> &XYCaliList, bool Rebuild);
	bool                       StageToCali(double PosX, double PosY, double &CaliX, double &CaliY);//理論座標轉成校正後的座標
	bool                       CaliToStage(double CaliX, double CaliY, double &PosX, double &PosY);//校正後座標轉成理論的座標
	bool                       MoveMotionXYCali(double OffsetX, double OffsetY, std::vector<TXYCali> &XYCaliList);//移動運動系統的XY校正表
	bool                       CaliValue(const TXYCali &Cali, bool forward, double PosX, double PosY, double &ValX, double &ValY);//座標轉換		
	bool                       CaliValue_Matrix(const TXYCali &Cali, bool forward, double PosX, double PosY, double &ValX, double &ValY);//投影矩陣	
	//----------------------------------------------------------------------------------//	
	bool                       CheckStagePosValidX(double Pos);//確認機台座標有效
	bool                       CheckStagePosValidY(double Pos);//確認機台座標有效
	bool                       CheckStagePosValidZ(double Pos);//確認機台座標有效	
	//----------------------------------------------------------------------------------//	
	bool                       SetFocusPosZ(double PosZ, LANE_ID LaneID);//設定焦距位置
	//----------------------------------------------------------------------------------//
	bool                       ModifyORGPosition(double PosX, double PosY, double PosZ);//修改原點位置
	//----------------------------------------------------------------------------------//
	int                        GetAxisSignPositive(int Axis);//取得軸控的軸方向	
	//----------------------------------------------------------------------------------//
	double                     AdjustAccValue(double AccVal, double Dist, double RefDist);//調整加減速數值
	double                     AdjustAccTime(double AccTime, double AccMinTime, ACC_TIME_ADJUST_MODE Mode);//調整加減速時間
	//----------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CMotion_Basic *MotionCtrlPtr;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_MOTION_BASIC_H__03B1DAF8_99AE_4A79_98A6_3B41EE7E8870__INCLUDED_)
