// CameraCtrl.h: interface for the CCameraCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERACTRL_H__529A7909_86FD_4836_BB02_D3577755A608__INCLUDED_)
#define AFX_CAMERACTRL_H__529A7909_86FD_4836_BB02_D3577755A608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
#include "Light3DCtrl.h"
#include "Camera_Basic.h"
//-------------------------------------------------------------------------------------//
//class CCamera_Basic;
//-------------------------------------------------------------------------------------//
#define BATCH_GRAB_LIGHT_00   0x00000000//燈盤 關閉 - BATCH_GRAB_2D_FRAME_DISABLE
#define BATCH_GRAB_LIGHT_01   0x00000001//燈盤 1    - BATCH_GRAB_2D_FRAME_01
#define BATCH_GRAB_LIGHT_02   0x00000002//燈盤 2    - BATCH_GRAB_2D_FRAME_02
#define BATCH_GRAB_LIGHT_03   0x00000004//燈盤 3
#define BATCH_GRAB_LIGHT_04   0x00000008//燈盤 4
#define BATCH_GRAB_LIGHT_05   0x00000010//燈盤 5
#define BATCH_GRAB_LIGHT_06   0x00000020//燈盤 6
#define BATCH_GRAB_LIGHT_07   0x00000040//燈盤 7
#define BATCH_GRAB_LIGHT_08   0x00000080//燈盤 8
#define BATCH_GRAB_LIGHT_09   0x00000100//燈盤 9
#define BATCH_GRAB_LIGHT_10   0x00000200//燈盤10
#define BATCH_GRAB_LIGHT_11   0x00000400//燈盤11
#define BATCH_GRAB_LIGHT_12   0x00000800//燈盤12
#define BATCH_GRAB_LIGHT_ALL  0x00000FFF//燈盤全
#define BATCH_GRAB_LIGHT_RGB  BATCH_GRAB_LIGHT_01|BATCH_GRAB_LIGHT_02|BATCH_GRAB_LIGHT_03

#define BATCH_GRAB_3D_CAST_01 0x00001000//A投光 - BATCH_GRAB_3D_CAST_01
#define BATCH_GRAB_3D_CAST_02 0x00002000//B投光 - BATCH_GRAB_3D_CAST_02
#define BATCH_GRAB_3D_CAST_03 0x00004000//C投光 - BATCH_GRAB_3D_CAST_03
#define BATCH_GRAB_3D_CAST_04 0x00008000//D投光 - BATCH_GRAB_3D_CAST_04
#define BATCH_GRAB_3D_CAST_AB BATCH_GRAB_3D_CAST_01|BATCH_GRAB_3D_CAST_02// - BATCH_GRAB_3D_CAST_12
#define BATCH_GRAB_3D_CAST_CD BATCH_GRAB_3D_CAST_03|BATCH_GRAB_3D_CAST_04// - BATCH_GRAB_3D_CAST_34
#define BATCH_GRAB_3D_CAST_ABCD  BATCH_GRAB_3D_CAST_01|BATCH_GRAB_3D_CAST_02|BATCH_GRAB_3D_CAST_03|BATCH_GRAB_3D_CAST_04// - BATCH_GRAB_3D_CAST_1234

#define BATCH_GRAB_STEP_1     0x00100000//1步-單純白光
#define BATCH_GRAB_STEP_4     0x00200000//4步

#define BATCH_GRAB_PHASE_1    0x01000000//相位1
#define BATCH_GRAB_PHASE_2    0x02000000//相位2
#define BATCH_GRAB_PHASE_3    0x04000000//相位3-RGB使用
#define BATCH_GRAB_PHASE_4    0x08000000//相位4-RGB使用
#define BATCH_GRAB_PHASE_M    BATCH_GRAB_PHASE_3//合成週期
#define BATCH_GRAB_PHASE_M2   BATCH_GRAB_PHASE_4//合成週期-2個曝光

#define BATCH_GRAB_PERIOD_1   0x10000000//1週期
#define BATCH_GRAB_PERIOD_2   0x20000000//2週期
//-------------------------------------------------------------------------------------//
enum BATCH_GRAB_STEP
{
	BATCH_GRAB_STEP_NULL       =    0,//4步1週期-AB投光	

	BATCH_GRAB_STEP_3D_CAST_01 =    1,//A投光
	BATCH_GRAB_STEP_3D_CAST_02 =    2,//B投光
	BATCH_GRAB_STEP_3D_CAST_03 =    3,//C投光
	BATCH_GRAB_STEP_3D_CAST_04 =    4,//D投光

	BATCH_GRAB_STEP_LIGHT_01   = 101,//燈盤-1
	BATCH_GRAB_STEP_LIGHT_02   = 102,//燈盤-2
	BATCH_GRAB_STEP_LIGHT_03   = 103,//燈盤-3
	BATCH_GRAB_STEP_LIGHT_04   = 104,//燈盤-4
	BATCH_GRAB_STEP_LIGHT_05   = 105,//燈盤-5
	BATCH_GRAB_STEP_LIGHT_06   = 106,//燈盤-6
	BATCH_GRAB_STEP_LIGHT_07   = 107,//燈盤-7
	BATCH_GRAB_STEP_LIGHT_08   = 108,//燈盤-8
	BATCH_GRAB_STEP_LIGHT_09   = 109,//燈盤-9
	BATCH_GRAB_STEP_LIGHT_10   = 210,//燈盤-10
	BATCH_GRAB_STEP_LIGHT_11   = 211,//燈盤-11
	BATCH_GRAB_STEP_LIGHT_12   = 212,//燈盤-12

	BATCH_GRAB_STEP_DUMMY     = 9999//無用	
};
//-------------------------------------------------------------------------------------//
class CCameraCtrl : public CObject  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CCameraCtrl)
	//---------------------------------------------------------------------------------//	
	static int                 GetPhaseMode(DWORD PatternStep, DWORD PhaseNum);//取得相位模式	
	static bool                BuildCameraIDCombox(CComboBox &Combox);//建立相機編號列表
	static bool                BuildCameraGrabModeCombox(CComboBox &Combox);//建立相機取像模式列表
	static bool                BuildCameraCallbackTimmingCombox(CComboBox &Combox);//建立相機回傳模式列表
	static bool                BuildBatchGrabLightNumCombox(CComboBox &Combox);//建立批次取像燈源數量列表
	static bool                BuildBatchGrabPhaseStepCombox(CComboBox &Combox);//建立批次取像相位步數列表
	static bool                BuildBatchGrabPhasePeriodCombox(CComboBox &Combox);//建立批次取像相位步數列表
	static bool                BuildBatchGrabPhaseCastCombox(CComboBox &Combox);//建立批次取像相位投射列表
	static bool                BuildBatchGrabPhaseIDCombox(CComboBox &Combox);//建立相位編號視窗		

	static CString             GetCameraIDText(CAMERA_ID CameraID);//取得相機編號文字
	static CString             GetCameraGrabModeText(CAMERA_GRAB_MODE GrabMode);//取得相機取像模式
	static CString             GetCameraCallbackTimmingText(CAMERA_CALLBACK_TIMMING Timming);//取得相機回傳模式列表
	static CAMERA_ID           GetCaemraIDFromWParam(WPARAM wParam);//取得相機編號
private:
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;//錯誤訊息
	//---------------------------------------------------------------------------------//
	int                        m_BatchGrabExposure_us;//批次取像曝光時間
	//---------------------------------------------------------------------------------//	
	bool                       m_BatchGrabbing;
	CAMERA_ID                  m_BatchGrabCameaID;//批次取像相機
	DWORD                      m_BatchGrabMode;//批次取像模式
	BATCH_GRAB_STEP            m_BatchGrabStep;//批次取像階段
	//---------------------------------------------------------------------------------//	
	unsigned int               m_BatchSliceParamIndex;
	unsigned int               m_BatchSliceParamIndex_Next3D;	
	std::vector<TSliceParam>   m_BatchSliceParamList;////批次取像列表
	std::vector<TSliceParam>   m_BatchSliceParamList_Next3D;////批次取像列表
	//---------------------------------------------------------------------------------//		
	LARGE_INTEGER              m_GrabFrequnce;//取像函數的頻率
	LARGE_INTEGER              m_GrabFnStartTime;//取像函數的起始時間
	LARGE_INTEGER              m_GrabFnEndTime;//取像函數的結束時間	
	//---------------------------------------------------------------------------------//	
	HANDLE                     m_BatchGrabFinishEvent;//循環取像結束的事件
	//---------------------------------------------------------------------------------//	
	std::vector<CCamera_Basic*> m_CameraPtrList;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	CCameraCtrl(const CCameraCtrl &CameraCtrl);
	CCameraCtrl& operator=(const CCameraCtrl &CameraCtrl);
	//---------------------------------------------------------------------------------//
	void                       PreInitCameraCtrl();
	void                       InitialCameraCtrl();
	//---------------------------------------------------------------------------------//
	inline void                InitialErrorString();//清除錯誤訊息
	//---------------------------------------------------------------------------------//		
	bool                       InitialCamreaPtrList();
	bool                       AddCameraPtr(CCamera_Basic *Ptr);	
	bool                       CheckCameraPtr(CCamera_Basic *Ptr);
	size_t                     GetCameraPtrCount();
	CCamera_Basic             *GetCameraPtr(size_t index, bool bCheck);
	CCamera_Basic             *GetCameraPtrByCameraID(CAMERA_ID CameraID);
	//---------------------------------------------------------------------------------//
	WPARAM                     MapCameraIDToWParam(CAMERA_ID CamreaID);
	//---------------------------------------------------------------------------------//
	bool                       BatchGrabKernel2(bool bFirst, bool bBuildLCB, int nTriggerStart, bool bUsing3D, bool &bFinish, std::vector<TSliceParam> &SliceParamList, unsigned int &SliceParamIndex);//開始批量取像	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CCameraCtrl();
	virtual ~CCameraCtrl();
	//---------------------------------------------------------------------------------//	
	void                       SetCameraExceptionCode_Param(LPCTSTR Err=NULL);
	void                       SetCameraExceptionCode_FileRead(LPCTSTR Err=NULL);
	void                       SetCameraExceptionCode_FileWrite(LPCTSTR Err=NULL);
	void                       SetCameraExceptionCode(DWORD Code, LPCTSTR Err=NULL);
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();//取得錯誤訊息
	bool                       ReturnNoUseCameraList();
	//---------------------------------------------------------------------------------//	
	void                       ResetBatchGrabbing();
	bool                       GetBatchGrabbing() const;
	void                       SetBatchGrabbing(bool val);
	//---------------------------------------------------------------------------------//	
	void                       ClearAllCameraCount();//復歸所有相機次數			
	bool                       ClearCameraCount(CAMERA_ID CameraID);//清除相機的次數紀錄	
	bool                       GetCameraCount(CAMERA_ID CameraID, long &cntCameraBak, long &cntExpBak, long &cntImageBak, long &cntImageCpy);//取得相機的次數紀錄	
	long                       GeCameraCallbackCount(CAMERA_ID CameraID);//取得疊加相機回傳的次數
	long                       GetCameraExposuredEndCount(CAMERA_ID CameraID);//取得疊加曝光結束回傳的次數
	long                       GetImageCallbackCount(CAMERA_ID CameraID);//取得疊加影像回傳的次數	
	long                       GetBufferCopyToHostCount(CAMERA_ID CameraID);//取得疊加影像複製的次數		

	long                       GetCameraNFramesToGrab(CAMERA_ID CameraID);//取得相機多少張數要去取	
	bool                       SetCameraNFramesToGrab(CAMERA_ID CameraID, long num);//設定相機多少張數要去取	

	long                       GetCameraBatchGrabCount(CAMERA_ID CameraID);//取得相機取像周期
	bool                       SetCameraBatchGrabCount(CAMERA_ID CameraID, long val);//設定相機取像周期
	
	bool                       IncrementCameraCopyToHostCount(CAMERA_ID CameraID);//疊加影像複製	
	//---------------------------------------------------------------------------------//	
	bool                       SetCameraToSendCallback(CAMERA_ID CameraID, BOOL val);//設定影像是否傳送callback
	bool                       GetCameraToSendCallback(CAMERA_ID CameraID, BOOL &val);//取得影像是否傳送callback
	//---------------------------------------------------------------------------------//	
	CAMERA_EXPOSURE_MODE       GetCameraExposureMode(CAMERA_ID CameraID);//取得相機曝光模式
	//---------------------------------------------------------------------------------//	
	unsigned int               GetCameraImageBufferCount(CAMERA_ID CameraID);//取得相機影像暫存數量
	//---------------------------------------------------------------------------------//	
	bool                       SetCameraCallbackTimming(CAMERA_ID CameraID, CAMERA_CALLBACK_TIMMING val);//設定相機回傳時機
	CAMERA_CALLBACK_TIMMING    GetCameraCallbackTimming(CAMERA_ID CameraID);//取得相機回傳時機
	//---------------------------------------------------------------------------------//		
	void                       SetAllSaveCameraAddRingBufferLog(bool val);//設定儲存增加相機影像列表資訊
	//------------------------------------------------------------------------------//
	unsigned int               GetMaxGrayImageSize();//取得最大黑白影像尺寸
	unsigned int               GetMaxColorImageSize();//取得最大彩色影像尺寸
	//---------------------------------------------------------------------------------//	
	double                     GetCameraMinPeriod(CAMERA_ID CameraID=PRIMARY_CAMERA_ID);//取得相機最短需要時間
	double                     GetCameraFramePerSecond(CAMERA_ID CameraID=PRIMARY_CAMERA_ID);//取得相機每秒取像張數	
	CAMERA_IMAGE_MODE          GetCameraImageMode(CAMERA_ID CameraID=PRIMARY_CAMERA_ID);//取得相機影像模式
	unsigned int               GetCameraImageSizeW(CAMERA_ID CameraID=PRIMARY_CAMERA_ID);//取得相機影像寬度
	unsigned int               GetCameraImageSizeH(CAMERA_ID CameraID=PRIMARY_CAMERA_ID);//取得相機影像長度	
	unsigned int               GetCameraImageStep(CAMERA_ID CameraID=PRIMARY_CAMERA_ID);//取得相機影像每條寬度
	unsigned int               GetCameraGrayImageSize(CAMERA_ID CameraID=PRIMARY_CAMERA_ID);//取得相機黑白影像尺寸
	unsigned int               GetCameraColorImageSize(CAMERA_ID CameraID=PRIMARY_CAMERA_ID);//取得相機彩色影像尺寸	
	bool                       GetCameraImage3(CAMERA_ID CameraID, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR pImage);//取得相機影像	
	bool                       GetCameraRawImage(CAMERA_ID CameraID, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_PTR pImage);//取得相機原始影像	
	//---------------------------------------------------------------------------------//	
	bool                       ResetAllCameraRingBuffer();//覆歸所有相機環型記憶體列表
	bool                       ResetCameraRingBuffer(CAMERA_ID CameraID);//覆歸相機環型記憶體列表
	bool                       KeepCameraTempRingBuffer(CAMERA_ID CameraID);//佔住相機環影像指標
	bool                       FreeCameraTempRingBuffer(CAMERA_ID CameraID);//釋放相機環影像指標
	long                       GetCameraRingBufferListSize(CAMERA_ID CameraID);//取得目前相機環型指標數量
	bool                       CheckCameraRingBufferStateDone(CAMERA_ID CameraID);//確認相機環型記憶影像狀態-已完成
	long                       GetCameraRingBufferCurrentIndex(CAMERA_ID CameraID);//取得目前相機環型指標引數
	bool                       ReSortCameraRingBufferImage(CAMERA_ID CameraID, const TSliceParam &SliceParam);//重新排序相機環型指標引數	
	bool                       GetCameraRingBufferImage(CAMERA_ID CameraID, long index, unsigned int &ImageW, unsigned int &ImageH, unsigned int &ImageStep, IMAGE_PTR &pImage);//取得相機原始影像	
	//---------------------------------------------------------------------------------//	
	bool                       InitialAllCamera();//初始化
	bool                       ReleaseAllCamera();//釋放	
	bool                       ResetAllCamera();//復歸
	bool                       RegisterAllCameraCurrentProcessFile();//註冊所有相機的訊息檔案
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetCameraErrorString(CAMERA_ID CameraID);//取得相機錯誤訊息
	bool                       StopAllCameraGrab();//停止所有相機取像	
	bool                       StopCameraGrab(CAMERA_ID CameraID);//停止相機取像	
	bool                       StartCameraGrab(CAMERA_ID CameraID);//開始相機取像
	bool                       SetCameraExposureTime(CAMERA_ID CameraID, const int ExposureTime);//設定相機曝光時間
	int                        GetCameraExposureTime(CAMERA_ID CameraID);//取得相機曝光時間
	bool                       FireCameraSoftwareTrigger(CAMERA_ID CameraID);//發送相機軟體觸發
	bool                       StartAllCameraGrab();//開始所有相機取像
	unsigned char *            GetCameraGammaLookUpTable(CAMERA_ID CameraID);//取得相機的Gamma表
	bool                       SetAllCameraApplyGamma(bool Apply);//設定所有相機是否使用Gamma
	bool                       SetAllCameraExposureTime(const int ExposureTime);//設定所有相機曝光時間
	bool                       SetAllCameraInternalTrigger();//設定相機內部觸發-立即取像
	bool                       SetAllCameraExternalTrigger();//設定相機外部觸發
	bool                       SetAllCameraSoftwareTrigger();//設定相機軟體取像
	bool                       SetAllCameraGrabMode(CAMERA_GRAB_MODE Mode);//設定相機取像
	CAMERA_GRAB_MODE           GetCameraGrabMode(CAMERA_ID CameraID);//取得相機取像
	bool                       SetCameraGrabMode(CAMERA_ID CameraID, CAMERA_GRAB_MODE Mode);//設定相機取像
	bool                       GetCameraCtrlReady();//取回相機控制是否正常
	bool                       StartCameraLiveGrab(CAMERA_ID CameraID);//開始相機連續取像
	bool                       StartAllCameraLiveGrab();//開始所有相機連續取像
	bool                       StartCameraGrabbing(CAMERA_ID CameraID);//開始相機取像		
	BAYER_PATTERN_MODE         GetCameraBayerPattern(CAMERA_ID CameraID);//取得相機Bayer樣板
	//---------------------------------------------------------------------------------//	
	bool                       SetAllCameraExposureFinishEvent();//設定所有相機曝光結束事件
	bool                       ResetAllCameraExposureFinishEvent();//復歸所有相機曝光結束事件
	bool                       ResetCameraExposureFinishEvent(CAMERA_ID CameraID);//復歸相機曝光結束事件
	bool                       StartCameraExposureFinishThread(CAMERA_ID CameraID, bool WaitOn);//開始相機的曝光執行緒
	bool                       WaitForCameraExposureFinishThreadStart(CAMERA_ID CameraID);//等待相機曝光結束執行緒起來
	bool                       StartAllCameraExposureFinishThread(bool WaitOn);//開始所有相機的曝光執行緒
	bool                       WaitForCameraExposureFinish(CAMERA_ID CameraID);//等帶相機曝光結束
	//---------------------------------------------------------------------------------//
	bool                       SetAllCameraGrabFinishEvent();//設定所有相機取像結束事件
	bool                       ResetAllCameraGrabFinishEvent();//復歸所有相機取像結束事件
	bool                       ResetCameraGrabFinishEvent(CAMERA_ID CameraID);//復歸相機取像結束事件
	bool                       StartCameraGrabFinishThread(CAMERA_ID CameraID, bool WaitOn);//開始相機的取像結束執行緒	
	bool                       WaitForCameraGrabFinishThreadStart(CAMERA_ID CameraID);//等待相機的取像結束執行緒起來
	bool                       StartAllCameraGrabFinishThread(bool WaitOn);//開始所有相機的取像結束執行緒
	bool                       WaitForCameraGrabFinish(CAMERA_ID CameraID, int TimeOut=1000);//等帶相機取像結束
	//---------------------------------------------------------------------------------//	
	bool                       WaitforCameraReadytoTrigger(CAMERA_ID CameraID);//等待相機可以觸發
	bool                       ResetCameraReadytoTriggerEvent(CAMERA_ID CameraID);//復歸相機可以觸發事件
	//---------------------------------------------------------------------------------//	
	double                     ReadCameraTemperature(CAMERA_ID CameraID);//讀取相機溫度	
	//---------------------------------------------------------------------------------//	
	void                       SetCameraFunctionStartTime(CAMERA_ID CameraID);//設定相機函數起始時間
	void                       SetCameraFunctionEndTime(CAMERA_ID CameraID);  //設定相機函數結束時間
	double                     GetCameraFunctionElapseTime(CAMERA_ID CameraID);//取得相機函數經過時間
	//------------------------------------------------------------------------------//
	bool                       LoadAllCamerasINIFile();//載入所有相機參數
	bool                       SaveAllCamerasINIFile();//儲存所有相機參數
	//------------------------------------------------------------------------------------------//		
	bool                       LoadCameraINIFile(CAMERA_ID CameraID);//載入相機參數
	bool                       SaveCameraINIFile(CAMERA_ID CameraID);//儲存相機參數
	//------------------------------------------------------------------------------------------//	
	bool                       SetCameraWhiteBalance(CAMERA_ID CameraID, double WBR, double WBG, double WBB);//設定相機白平衡參數
	bool                       GetCameraWhiteBalance(CAMERA_ID CameraID, double &WBR, double &WBG, double &WBB);//取得相機白平衡參數
	bool                       CalcCameraWhiteBalance(CAMERA_ID CameraID);//計算相機白平衡
	bool                       ResetCameraWhiteBalance(CAMERA_ID CameraID);//復歸相機白平衡
	bool                       LoadCameraParameterFFCByLEDName(const char *LEDName);//取得相機白平衡
	//------------------------------------------------------------------------------------------//
	bool                       DeleteBatchGrabFinishEvent();//刪除批量取像完成事件
	bool                       CreateBatchGrabFinishEvent();//建立批量取像完成事件
	bool                       WaitForBatchGrabFinishEvent();//等待批量取像完成事件
	bool                       SetBatchGrabFinishEvent();//設定批量取像完成事件
	bool                       ResetBatchGrabFinishEvent();//復歸批量取像完成事件
	//------------------------------------------------------------------------------//
	long                       GetBatchGrabFrames(DWORD GrabMode, BATCH_GRAB_STEP GrabStep);//取得批量取像的張數
	LIGHT_3D_CLS_PTR           GetBatchGrabLightCtrlPtr(BATCH_GRAB_STEP Step);//取得批量取像的投光
	BATCH_GRAB_STEP            GetBatchGrabFirstStep(DWORD GrabMode);//取得第一個步驟
	BATCH_GRAB_STEP            GetBatchGrabNextStep(DWORD GrabMode, BATCH_GRAB_STEP CurStep);//取得下一個步驟
	//------------------------------------------------------------------------------//
	bool                       BatchGrabModeEncode(int LightNum, bool ProjA, bool ProjB, bool ProjC, bool ProjD, int Step, int nPhase, DWORD &Mode);
	bool                       BatchGrabModeDecode(DWORD Mode, int &LightNum, bool &ProjA, bool &ProjB, bool &ProjC, bool &ProjD, int &Step, int &nPhase);
	//------------------------------------------------------------------------------//
	CAMERA_ID                  GetBatchGrabCameraID() const;//取得批次取像相機
	DWORD                      GetBatchGrabMode() const;//取得批次取像模式
	void                       SetBatchGrabStep(BATCH_GRAB_STEP val);//設定目前步驟
	BATCH_GRAB_STEP            GetBatchGrabStep() const;//取得目前步驟
	//------------------------------------------------------------------------------//
	bool                       BatchGrabPrepare(CAMERA_ID CameraID, DWORD GrabMode, int Period_us, int Exposure_us);//準備批量取像
	bool                       BatchGrabStart(bool &bFinish);//開始批量取像	
	//------------------------------------------------------------------------------//
	bool                       FillCameraImage(CAMERA_ID CameraID, IMAGE_DISPLAY_MODE ImageMode, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);//依據顯示模式來取回影像資料
	bool                       FillCameraImage3(CAMERA_ID CameraID, IMAGE_DISPLAY_MODE ImageMode, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR ImagePtr);//依據顯示模式來取回影像資料
	//------------------------------------------------------------------------------//
	bool                       BatchIndexReset();//復歸批量取像
	const std::vector<TSliceParam>& GetBatchParamList();//取回批量取像參數
	bool                       BatchGrabPrepare2(const std::vector<TSliceParam> &ParamList, bool ResetCamera);//準備批量取像
	bool                       BatchGrabStart2(bool &bFinish);//開始批量取像	
	bool                       BatchGrabStart2(bool &bFinish, bool bUsing3D);//開始批量取像	
	bool                       BatchGrabNext3DImage(bool &bFinish);//下一次批量取像		
	bool                       CheckNeedGrabNext3DImage();//確定是否需要取下一次批量	
	bool                       RetrieveCameraImageCallback(CAMERA_ID CameraID, bool &bReturn);//接收相機影像回傳, 程式整理
	bool                       SetBatchGrabExpourseTime(const std::vector<TSliceParam> &ParamList);//設定批量取像曝光時間
	//------------------------------------------------------------------------------//		
	bool                       WaitForCameraImageCallbackCount(CAMERA_ID CameraID, long ImageCount);//等待相機影像回來
	//------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CCameraCtrl CameraCtrl;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_CAMERACTRL_H__529A7909_86FD_4836_BB02_D3577755A608__INCLUDED_)
