// CameraObj.h: interface for the CCamera_Basic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_BASIC_H__4FB43C19_B429_4D9C_9354_CB60945D7F30__INCLUDED_)
#define AFX_CAMERA_BASIC_H__4FB43C19_B429_4D9C_9354_CB60945D7F30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Camera_Define.h"
//-------------------------------------------------------------------------------------//
//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
class CCamera_Basic : public CObject  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CCamera_Basic)
	//---------------------------------------------------------------------------------//	
private:	
	//---------------------------------------------------------------------------------//
	CRITICAL_SECTION           m_csCamera;//相機的關鍵區間
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CCamera_Basic(const CCamera_Basic &camera);
	CCamera_Basic& operator=(const CCamera_Basic &camera);
	//---------------------------------------------------------------------------------//		
	CString                    m_ErrorString;
	CString                    m_ErrorStringOut;
	bool                       m_CameraConstructed;//建構已完成
	int                        m_CameraModelID;//相機型號
	bool                       m_CameraInited; //相機是否初始化
	CAMERA_ID                  m_CameraID;     //相機編號	
	CAMERA_GRAB_MODE           m_CameraGrabMode;;//取像模式-外部觸發或者是內部直接取像	
	CAMERA_IMAGE_MODE          m_CameraImageMode;//相機影像模式-Gray, Bayer, Color
	HWND                       m_CameraCallBackWnd;//相機回傳視窗
	BAYER_PATTERN_MODE         m_CameraBayerType;//相機Bayer樣式
	CAMERA_SHUTTER_MODE        m_CameraShutterMode;//相機快門模式
	CAMERA_EXPOSURE_MODE       m_CameraExposureMode;//相機曝光模式
	CAMERA_CALLBACK_TIMMING    m_CameraCallbackTimming;//相機回傳時機
	CString                    m_CameraModelName;//相機機型名稱
	unsigned int               m_CameraLogIndex;//相機的訊息檔案引數
	unsigned int               m_CameraImageBufferCount;//相機影像緩存數量

	int                        m_PeriodTim_us;//FPS下的時間
	int                        m_ExposureTime_us; //曝光時間-us
	int                        m_ExposureTimeMin_us;//曝光時間-Min-us
	int                        m_ExposureTimeMax_us;//曝光時間-Max-us
	int                        m_TriggerDelay;//觸發延遲時間-us

	IMAGE_SIZE                 m_CameraBitCount;
	IMAGE_SIZE                 m_CameraImageStep;//相機間距
	IMAGE_SIZE                 m_CameraImageW;//相機影像寬度
	IMAGE_SIZE                 m_CameraImageH;//相機影像高度		
	IMAGE_SIZE                 m_CameraSizeRaw;//相機尺寸
	IMAGE_SIZE                 m_CameraSizeColor;//相機尺寸彩色	
	double                     m_CameraFPS;//相機每秒張數

	WPARAM                     m_CameraWParam;//影像回傳參數
	LPARAM                     m_CameraLParam;//影像回傳參數
	BOOL                       m_CameraToSendCallback;//影像是否傳送callback
	
	IMAGE_PTR                  m_CameraImagePtr;//指向相機或者擷取卡內的圖像指標
	IMAGE_PTR                  m_ClonedRawImagePtr;//複製出的原始影像
	IMAGE_PTR                  m_ClonedColorImagePtr;//32 bit Color Image Data

	IMAGE_PTR                  m_CameraKeepBufferPtr;//暫存住的環指標
	long                       m_CameraKeepBufferIndex;//暫存住的環指標引數

	long                       m_CameraRingBufferIndex;//目前最新的影像影像指標
	long                       m_CameraRingBufferListSize;//影像指標Buffer最大可以儲存的數量
	IMAGE_PTR*                 m_CameraRingBufferList;//記錄相機影像的記憶體位址	
	CAMERA_RING_BUFFER_STATES* m_CameraRingBufferStateList;//記錄相機影像的狀態列表
	bool                       m_SaveCameraAddRingBufferLog;//儲存增加相機影像列表資訊

	bool                       CheckCameraRingBuffer();//確認相機影像記憶體位址
	bool                       CreateCameraRingBuffer();//建立相機影像記憶體位址
	bool                       ReleaseCameraRingBuffer();//釋放相機影像記憶體位址;
	//------------------------------------------------------------------------------//
	BOOL                       m_CameraUsingGamma;
	unsigned char              m_CameraGammaLUT[256];//Gamma用的Lookup Table
	//------------------------------------------------------------------------------//
	BITMAPINFO                *m_pColorInfo;//Color
	BITMAPINFO                *m_pMonoInfo;//Raw	
	//------------------------------------------------------------------------------//		
	long                       m_CountForCameraCallback;//相機回傳-Step01
	long                       m_CountForCameraExposuredEnd;//曝光結束回傳-Step02
	long                       m_CountForImageCallback;//影像回傳-Step03
	long                       m_CountForBufferCopyToHost;//影像複製-Step04
	long                       m_CountForFramesToGrab;//相機要取像張數	
	long                       m_CountForGrabLoop;//相機週期數量
	//------------------------------------------------------------------------------------------//
	bool                       m_CameraGrabbing;//取像中
	HANDLE                     m_CameraGrabFinishEvent;//取像結束的事件
	HANDLE                     m_CameraExposureFinishEvent;//曝光結束的事件
	//------------------------------------------------------------------------------------------//
	LARGE_INTEGER              m_CameraFrequnce;//相機函數的頻率
	LARGE_INTEGER              m_CameraFnStartTime;//相機函數的起始時間
	LARGE_INTEGER              m_CameraFnEndTime;//相機函數的結束時間
	//------------------------------------------------------------------------------------------//	
	bool                       m_CameraApplyGamma;//因校正若使用Gamma會因線性度問題而失準, 所以額外開後門關閉
	//------------------------------------------------------------------------------------------//
	virtual void               PreInitCamera();	
	//------------------------------------------------------------------------------//
	void                       LockCamera();
	void                       UnlockCamera();
	//------------------------------------------------------------------------------//	
	void                       SetCameraConstructed(bool val);
	bool                       GetCameraConstructed() const;
	//------------------------------------------------------------------------------//
	void                       SetCameraModelID(int Model);
	int                        GetCameraModelID() const;
	//------------------------------------------------------------------------------//		
	void                       SetCameraModelName(LPCTSTR  Model);
	LPCTSTR                    GetCameraModelName() const;	
	//------------------------------------------------------------------------------//
	CString                    GetCameraErrorKeyName() const;
	//------------------------------------------------------------------------------//	
	void                       SetCameraGrabMode(CAMERA_GRAB_MODE val);
	//------------------------------------------------------------------------------//
	void                       SetCameraShutterMode(CAMERA_SHUTTER_MODE val);
	//------------------------------------------------------------------------------//
	void                       SetCameraExposureMode(CAMERA_EXPOSURE_MODE val);
	//------------------------------------------------------------------------------//
	void                       SetCameraImageBufferCount(unsigned int val);//相機影像緩存數量
	//------------------------------------------------------------------------------//
	CString                    GetCameraINIFileName();	
	CString                    GetCameraSectionName();	
	CString                    GetCameraBasicSection();
	//------------------------------------------------------------------------------//	
	int                        CalcCameraImageOffset(int SensorSize, int ImageSize, int OffsetInc=1) const;//計算影像尺寸變化時的偏移值
	//------------------------------------------------------------------------------//	
	bool                       SaveCameraProcess(LPCTSTR fnName, int Level);
	//------------------------------------------------------------------------------//
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CCamera_Basic();
	virtual ~CCamera_Basic();
	//---------------------------------------------------------------------------------//		
	bool                       CheckCameraInited();//確認相機是否初始化正常
	void                       SetCameraInited(bool val);
	//------------------------------------------------------------------------------//	
	bool                       RegisterCameraCurrentProcessFile();
	bool                       SaveCameraCurrentProcess(const char *String);
	bool                       SaveCameraCurrentProcess(const wchar_t *String);
	//------------------------------------------------------------------------------//	
	void                       SetCameraErrorString(LPCSTR lpszText);
	void                       SetCameraErrorString(LPCWSTR lpszText);
	LPCTSTR                    GetCameraErrorString();
	//------------------------------------------------------------------------------//	
	CAMERA_ID                  GetCameraID() const;//取得相機編號	
	virtual void               SetCameraID(CAMERA_ID value);//設定相機編號
	//------------------------------------------------------------------------------//		
	BITMAPINFO*                GetColorBitmapInfo();
	BITMAPINFO*                GetMonoBitmapInfo();
	//------------------------------------------------------------------------------//	
	HWND                       GetCameraCallBackHWnd();	
	virtual void               SetCameraCallBackHWnd(const HWND hWnd);
	//------------------------------------------------------------------------------//			
	CAMERA_IMAGE_MODE          GetCameraImageMode() const;
	virtual void               SetCameraImageMode(CAMERA_IMAGE_MODE val);
	//------------------------------------------------------------------------------//		
	BAYER_PATTERN_MODE         GetCameraBayerPattern() const;
	virtual void               SetCameraBayerPattern(BAYER_PATTERN_MODE val);
	BAYER_PATTERN_MODE         GetCameraBayerPattern(int StartX, int StartY);//根據起點的X, Y來知道解碼的方式
	//------------------------------------------------------------------------------//
	CAMERA_SHUTTER_MODE        GetCameraShutterMode() const;//相機快門模式
	//------------------------------------------------------------------------------//
	CAMERA_EXPOSURE_MODE       GetCameraExposureMode() const;//相機曝光模式
	//------------------------------------------------------------------------------//
	unsigned int               GetCameraImageBufferCount() const;//相機影像緩存數量
	//------------------------------------------------------------------------------//
	bool                       GetSaveCameraAddRingBufferLog() const;//取得儲存增加相機影像列表資訊
	virtual void               SetSaveCameraAddRingBufferLog(bool val);//設定儲存增加相機影像列表資訊
	//------------------------------------------------------------------------------//
	virtual bool               SetCameraApplyGamma(bool Apply);
	//------------------------------------------------------------------------------//	
	const IMAGE_PTR            GetCurrentRawImage();	
	bool                       GetImageFromCamera();	
	//------------------------------------------------------------------------------//
	CAMERA_GRAB_MODE           GetCameraGrabMode() const;
	virtual bool               SwitchCameraGrabMode(CAMERA_GRAB_MODE Mode=CAMERA_GRAB_FREE_RUN)=0;
	//------------------------------------------------------------------------------//
	bool                       DeleteCameraGrabFinishEvent();//刪除相機取像完成事件
	bool                       CreateCameraGrabFinishEvent();//建立相機取像完成事件
	bool                       WaitForCameraGrabFinishEvent();//等待相機取像完成事件
	bool                       SetCameraGrabFinishEvent();//設定相機取像完成事件
	bool                       ResetCameraGrabFinishEvent();//復歸相機取像完成事件
	bool                       CheckCameraGrabFinishEvent();//確認相機取像完成事件
	//------------------------------------------------------------------------------//
	void                       SetCameraFunctionStartTime();//設定相機函數起始時間
	void                       SetCameraFunctionEndTime();  //設定相機函數結束時間
	double                     GetCameraFunctionElapseTime();//取得相機函數經過時間
	//------------------------------------------------------------------------------//
	bool                       DeleteCameraExposureFinishEvent();//刪除相機曝光完成事件
	bool                       CreateCameraExposureFinishEvent();//建立相機曝光完成事件
	bool                       WaitForCameraExposureFinishEvent();//等待相機曝光完成事件
	bool                       SetCameraExposureFinishEvent();//設定相機曝光完成事件
	bool                       ResetCameraExposureFinishEvent();//取得相機曝光完成事件
	bool                       CheckCameraExposuredEndEvent();//確認相機曝光完成事件
	//------------------------------------------------------------------------------//
	virtual bool               SetCameraGrabbing(bool Grabbing);
	virtual void               SetCameraGrabbing_Unlock(bool Grabbing);
	bool                       GetCameraGrabbing(); 
	//------------------------------------------------------------------------------//
	bool                       SendCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//送訊息給回傳視窗 
	bool                       PostCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam);//送訊息給回傳視窗
	//---------------------------------------------------------------------------------//
	bool                       AddCameraRingBufferList(void *ImagePtr);//增加相機回傳影像
	virtual bool               RemoveCameraRingBufferByPtr(void *ImagePtr);//移除相機回傳影像-依指標移除
	virtual bool               RemoveCameraRingBufferByIndex(long index);//移除相機回傳影像-依引數移除
	virtual long               GetCameraRingBufferListSize() const;//取得目前相機環型指標數量	
	virtual long               GetCameraRingBufferCurrentIndex() const;//取得目前相機環型指標引數 		
	virtual bool               CheckCameraRingBufferStateDone();//確認相機環型記憶體狀態-已完成
	virtual bool               ReSortCameraRingBufferImage(const TSliceParam &SliceParam);
	virtual bool               ReSortCameraRingBufferImage_SepareDLPTable(const TSliceParam &SliceParam);//重新排序相機環型指標引數
	virtual bool               ReSortCameraRingBufferImage_2ndLightDLPTable(const TSliceParam &SliceParam);//重新排序相機環型指標引數
	virtual bool               GetCameraRingBufferImage(long idx, unsigned int &ImageW, unsigned int &ImageH, unsigned int &ImageStep, IMAGE_PTR &pDst);//取得環型指標內的影像	
	//------------------------------------------------------------------------------//
	virtual bool               ResetCameraRingBuffer();//覆歸相機環型記憶體列表
	virtual bool               FreeCameraTempRingBuffer();//歸還相機環指標現今
	virtual bool               KeepCameraTempRingBuffer();//暫存相機環指標現今	
	//------------------------------------------------------------------------------//
	virtual bool               CreateBMPInfo(BITMAPINFO *&pBMPInfo, bool IsColor);//建立一個本系統可用的BMP Infor
	virtual bool               DrawColorImageToDC(HDC hDC, const unsigned char *pColoeImage, RECT &ImageWndRect, POINT &OffsetPts, double RealScaleX, double RealScaleY, double ZoomScale);
	//------------------------------------------------------------------------------//	
	virtual bool               CloneCurrentRawImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_PTR pDst);//複製原始影像
	virtual bool               CloneCurrentCameraImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR pDst);//複製相機內部原始影像
	virtual bool               CloneCurrentColorImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_PTR pDst);//複製彩色影像

	virtual bool               FillCameraImage(IMAGE_DISPLAY_MODE ImageMode, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);//依據顯示模式來取回影像資料
	virtual bool               FillCameraImage3(IMAGE_DISPLAY_MODE ImageMode, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR ImagePtr);//依據顯示模式來取回影像資料
	//------------------------------------------------------------------------------//
	virtual bool               InitialCamera()=0;
	virtual bool               ReleaseCamera()=0;
	virtual bool               ResetCamera()=0;//復歸相機

	IMAGE_SIZE                 GetBitCount() const;//取得影像灰階深度
	IMAGE_SIZE                 GetImageStep() const;//取得整張影像每條記憶體寬度像素
	IMAGE_SIZE                 GetCameraImageW() const;//取得整張影像寬度像素
	IMAGE_SIZE                 GetCameraImageH() const;//取得整張影像長度像素		
	IMAGE_SIZE                 GetImageRawSize() const;//取得整張原始影像像素
	IMAGE_SIZE                 GetImageColorSize() const;//取得整張彩色影像像素
	double                     GetCameraFPS() const;//取得相機的FPS
	bool                       CalcCameraImageSize();//計算相機影像尺寸

	void                       SetCameraWParam(WPARAM wParam);//設定相機回傳參數-W
	WPARAM                     GetCameraWParam() const;//取得相機回傳參數-W
	void                       SetCameraLParam(LPARAM lParam);//設定相機回傳參數-L
	LPARAM                     GetCameraLParam() const;//取得相機回傳參數-L	
	void                       SetCameraToSendCallback(BOOL val);//設定影像是否傳送callback
	BOOL                       GetCameraToSendCallback() const;//取得影像是否傳送callback
	bool                       SetCameraCallbackTimming(CAMERA_CALLBACK_TIMMING val);//設定相機回傳時機
	CAMERA_CALLBACK_TIMMING    GetCameraCallbackTimming() const;//取得相機回傳時機

	int                        GetPeriodTime() const;//取得最少周期時間
	int                        GetExposureTime() const;//取得曝光時間;
	virtual bool               SetExposureTime(const int ExposureTime)=0;
	virtual bool               FreeCameraImagePtr(void *ImagePtr);//釋放相機內的影像
	virtual bool               StartCameraGrab()=0;//只取一張影像-內部觸發用
	virtual bool               StartCameraLiveGrab()=0;//連續取多張影像-外部觸發用
	virtual bool               StopCameraGrab()=0;//停止取像
	virtual bool               FireSoftwareTrigger()=0;//發射軟體觸發訊號
	virtual bool               WaitforCameraReadytoTrigger()=0;//等待相機準備好可以觸發
	virtual bool               ResetCameraReadyTriggerEvent()=0;//復歸相機準備好了的事件
	virtual bool               DoCameraDebayer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const unsigned char *pRaw, unsigned char *pResult, BAYER_PATTERN_MODE BayerPattern)=0;
	virtual bool               GetWhiteBalanceParams(double &WBR, double &WBG, double &WBB)=0;
	virtual bool               SetWhiteBalanceParams(double WBR, double WBG, double WBB)=0;
	virtual bool               CalcWhiteBalanceParams()=0;
	virtual bool               ResetWhiteBalanceParams()=0;
	virtual double             ReadCameraTemperature();//讀取相機溫度
	//------------------------------------------------------------------------------//
	virtual bool               WriteCameraParameterToDevice()=0;//儲存目前相機的參數至相機內部的韌體上	
	virtual bool               SaveCameraINIFile();
	virtual bool               LoadCameraINIFile();		
	//------------------------------------------------------------------------------//		
	void                       ClearCountAll();//清除所有計數器

	void                       IncrementCountForCameraCallback();//疊加相機回傳-Step01
	void                       IncrementCountForCameraExposuredEnd();//疊加曝光結束回傳-Step02
	void                       IncrementCountForImageCallback();//疊加影像回傳-Step03
	void                       IncrementCountForBufferCopyToHost();//疊加影像複製-Step04	

	long                       GetCountForCameraCallback();//疊加相機回傳-Step01
	long                       GetCountForCameraExposuredEnd();//疊加曝光結束回傳-Step02
	long                       GetCountForImageCallback();//疊加影像回傳-Step03	
	long                       GetCountForBufferCopyToHost();//疊加影像複製-Step04	

	long                       GetCountForFramesToGrab();//取得多少張數要去取
	bool                       SetCountForFramesToGrab(long num);//設定多少張數要去取

	long                       GetCountForBatchGrab();//取得批次取像張數
	bool                       SetCountForBatchGrab(long loop);//設定批次取像張數

	DWORD                      GetCameraGrabTimeout() const;//取得相機取像逾時時間-ms
	bool                       WaitForCameraImageCallbackCount(long ImageCount);//等待相機影像回來	
	//------------------------------------------------------------------------------//			
	void                       InitialGammaLUT();
	void                       BuildGammaLUT(const double gamma);
	unsigned char*             GetGammaLUTPtr();
	BOOL                       GetUsingGamma();
	//------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_CAMERA_BASIC_H__4FB43C19_B429_4D9C_9354_CB60945D7F30__INCLUDED_)

