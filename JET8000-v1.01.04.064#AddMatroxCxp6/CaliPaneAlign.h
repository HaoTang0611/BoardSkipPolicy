#if !defined(AFX_CALIPANEALIGN_H__2963833E_1B50_4137_8D17_E966D4C7345D__INCLUDED_)
#define AFX_CALIPANEALIGN_H__2963833E_1B50_4137_8D17_E966D4C7345D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaliPaneAlign.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "Draw3DWnd.h"
#include "MotionCtrlWnd.h"
#include "ImagePhaseWnd.h"
#include "ImageMaskWnd.h"
//-------------------------------------------------------------------------------------//
enum CALIALIGN_MANIPULATE_MODE
{
	CALIALIGN_MANIPULATE_NONE = 0,		
	CALIALIGN_MANIPULATE_RECT,
	CALIALIGN_MANIPULATE_GRID, 
	CALIALIGN_MANIPULATE_PHASE_LINE,
	CALIALIGN_MANIPULATE_FACTOR_GRID
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneAlign dialog
//-------------------------------------------------------------------------------------//
class CCaliPaneAlign : public CDialog
{
// Construction
public:
	CCaliPaneAlign(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCaliPaneAlign)
	enum { IDD = IDD_CALIBRATION_PANE_ALIGN };
	CComboBox   m_NoiseDefineModeCombox;
	CComboBox   m_PhaseToHeightCombox;
	CComboBox   m_HeightFactorNumCombox;
	CComboBox   m_3DCastCurrentIDCombox;
	CComboBox   m_SaveRawExtNameCombox;
	CComboBox   m_SpaceMergeModeCombox;
	CComboBox   m_SpaceMergeBestModeCombox;
	CComboBox   m_SpaceMergeIntensityModeCombox;
	CComboBox   m_CastSpaceFilterModeCombox;
	CComboBox   m_FirstFilterModeCombox;
	CComboBox   m_OverLowModeCombox;
	CComboBox   m_HeightVarModeCombox;
	CComboBox   m_FinalFilterModeCombox;
	CComboBox   m_FinalFilterModeCombox2;
	CComboBox	m_SliceCombo;
	CComboBox	m_FovResCombox;
	CListBox	m_LogListBox;
	CStatic	m_ImageTargetWnd;
	CComboBox	m_PhaseIDCombox;
	CComboBox	m_PhaseLEDCombox;
	CComboBox	m_ManipulateCombox;
	CComboBox	m_3DCastIDCombox;
	CComboBox	m_LightCombox;
	CComboBox	m_CameraCombox;
	CStatic	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaliPaneAlign)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	bool                       ExecOnOK();
	//---------------------------------------------------------------------------------//
	void                       SetModifiedCaliParam(bool val);
	bool                       GetModifiedCaliParam() const;	
	//---------------------------------------------------------------------------------//
	void                       SetSpaceBuffer(size_t Size, SPACE_PTR Buffer, SPACE_PTR Buffer1, SPACE_PTR Buffer2);
	void                       SetShowBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1);
	void                       SetImageBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1, IMAGE_PTR Buffer2, PHASE_PTR PhaseBuffer, PHASE_PTR PhaseBuffer1, PHASE_PTR PhaseBuffer2, MASK_PTR MaskBuffer, MASK_PTR MaskBuffer1, MASK_PTR MaskBuffer2);
	//---------------------------------------------------------------------------------//		
protected:	
	//---------------------------------------------------------------------------------//		
	void                       BuildManipulateCombox(CComboBox &Combox);
	void                       BuildSaveRawExtNameCombox(CComboBox &Combox);
	//---------------------------------------------------------------------------------//		
	CMotionCtrlWnd             m_MotionCtrlWnd;
	//---------------------------------------------------------------------------------//
	int                        m_CameraExposureTime_us;
	int                        m_CameraExposureTimeBackup_us;		
	CAMERA_ID                  m_CameraID;	
	DWORD                      m_LightNum;
	TSliceParam                m_SliceParam;
	DWORD                      m_PatternStep;
	DWORD                      m_PhaseID;	
	SLICE_FUNC_MODE            m_SliceFuncMode;//影像設定上主要3D的模式
	bool                       m_Multi3DCastID;//多個3D投光使用
	LIGHT_3D_CAST_ID           m_Light3DCastID;	
	DWORD                      m_PatternProjectMode;//批次取像樣板模式
	DWORD                      m_CameraBatchGrabMode;//批次取像模式	
	double                     m_ZeroPhaseOffsetPosZ;
	BOOL                       m_bShowCenterLine;
	BOOL                       m_bShowHorizontalLine;
	BOOL                       m_bShowVerticalLine;
	bool                       m_ModifiedCaliParam;//修正過相位校正
	TNoiseFilterParam          m_NoiseFilterParam;	
	DWORD                      m_ExtraDelayTime;
	int                        m_SaveRawImageTimes;
	//---------------------------------------------------------------------------------//	
	double                     m_AveRoiR;
	double                     m_AveRoiG;
	double                     m_AveRoiB;	
	double                     m_AveGrayR;
	double                     m_AveGrayG;
	double                     m_AveGrayB;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;	
	CALIBRATION_MODE           m_CalibrationMode;
	CALIALIGN_MANIPULATE_MODE  m_ManipulateMode;
	long                       m_CameraImageReceieveCount;//已從相機接收多少的相機影像
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_ShowBuffer;
	IMAGE_PTR                  m_ShowBuffer1;	
	IMAGE_SIZE                 m_ShowStep;
	IMAGE_SIZE                 m_ShowBitCount;	

	MASK_PTR                   m_MaskBuffer;
	MASK_PTR                   m_MaskBuffer1;
	MASK_PTR                   m_MaskBuffer2;

	PHASE_PTR                  m_PhaseBuffer;
	PHASE_PTR                  m_PhaseBuffer1;
	PHASE_PTR                  m_PhaseBuffer2;	
	IMAGE_PTR                  m_ImageBuffer;
	IMAGE_PTR                  m_ImageBuffer1;
	IMAGE_PTR                  m_ImageBuffer2;	

	SPACE_PTR                  m_SpaceBuffer;
	SPACE_PTR                  m_SpaceBuffer1;//空間記憶體指標	
	SPACE_PTR                  m_SpaceBuffer2;//空間記憶體指標	

	size_t                     m_ShowBufferSize;
	size_t                     m_ImageBufferSize;
	size_t                     m_SpaceBufferSize;
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ImgTargetW;
	IMAGE_SIZE                 m_ImgTargetH;
	IMAGE_SIZE                 m_ImgTargetStep;	
	//---------------------------------------------------------------------------------//
	POINT                      m_MovingPos;
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;	
	
	TRECT4D                    m_ImageRect4D;
	TPOINT2D                   m_ImagePt1;
	TPOINT2D                   m_ImagePt2;	

	TPOINT2D                   m_ImageHorPt;
	TPOINT2D                   m_ImageVerPt;	

	TPOINT2D                   m_ImageWndPt1;
	TPOINT2D                   m_ImageWndPt2;
	//---------------------------------------------------------------------------------//
	size_t                     m_PhaseCount;//相位數量
	double                     m_PhaseAngle;//相位角度
	double                     m_PhaseRange;//相位範圍
	double                     m_PhaseHeight;//相位範圍
	TPOINT2D                   m_PhaseLinePt1;//相位線段
	TPOINT2D                   m_PhaseLinePt2;//相位線段
	TPOINT2D                   m_PhaseNormPt1;//相位線段-法線
	TPOINT2D                   m_PhaseNormPt2;//相位線段-法線
	std::vector<TPIXEL_GRY>    m_PhasePtList;//相位點群
	//---------------------------------------------------------------------------------//	
	double                     m_3DCastFocusLT;//3D投光焦距-左上
	double                     m_3DCastFocusRT;//3D投光焦距-右上
	double                     m_3DCastFocusLB;//3D投光焦距-左下
	double                     m_3DCastFocusRB;//3D投光焦距-右下
	double                     m_3DCastFocusCC;//3D投光焦距-中央
	double                     m_3DCastFocusCCMax;//3D投光焦距-中央
	//---------------------------------------------------------------------------------//
	unsigned int               m_ImageGridRows;
	unsigned int               m_ImageGridCols;
	std::vector<TImageStat>    m_ImageGridList;
	//---------------------------------------------------------------------------------//	
	int                        m_PhaseFactorID_Z;
	unsigned int               m_PhaseFactorIndex;
	unsigned int               m_PhaseFactorRows;
	unsigned int               m_PhaseFactorCols;
	std::vector<TPhaseFactorGrid> m_PhaseFactorList;	
	//---------------------------------------------------------------------------------//
	double                     m_AutoFocustScalePosZ;//自動對焦Z值的放大比例
	double                     m_AutoFocustMaxPosZ;//自動對焦最大Z值
	double                     m_AutoFocustMinPosZ;//自動對焦最小Z值
	double                     m_AutoFocustLastPosZ;//自動對焦上一次的位置
	double                     m_AutoFocustBestPosZ;//自動對焦最好的位置
	double                     m_AutoFocustBestStd;//自動對焦最好的數值
	double                     m_AutoFocusPitch;//自動對焦的步長
	std::vector<TPOINT2D>      m_AutoFocusReadingList;//自動對焦的讀值列表
	//---------------------------------------------------------------------------------//		
	bool                       m_CalibrateAllCannel;//調適全部2D燈源
	bool                       m_CalibrateAllCastID;//調適全部3D投光
	//---------------------------------------------------------------------------------//		
	int                        m_2DLEDCurrent;//2DLED電流	
	float                      m_2DLEDCurrentGray;//2DLED電流影像灰階	
	std::vector<CString>       m_2DLEDCurrentAlarmList;//2DLED電流警報
	std::vector<T2D_CURRENT>   m_2DLEDCurrentReadingList;//2DLED電流讀值
	//---------------------------------------------------------------------------------//		
	int                        m_3DCastCurrent;
	int                        m_3DCastCurRed;//3D投光電流-Red
	int                        m_3DCastCurGrn;//3D投光電流-Grn
	int                        m_3DCastCurBlu;//3D投光電流-Blu	
	int                        m_3DCastCurrentGray;//3D投光電流影像灰階
	unsigned int               m_3DCastExposureTimeus;//3D投光電流校正的曝光時間
	//---------------------------------------------------------------------------------//
	int                        m_GrayTarget;//灰階目標	
	double                     m_GrayMinRatio;//灰階誤差範圍		
	//---------------------------------------------------------------------------------//
	double                     m_MultiZeroPlaneGapRatio;//多個相平面高度差比例
	double                     m_MultiZeroPlaneGapThreshold;//多個相平面高度差閥值
	//---------------------------------------------------------------------------------//
	COLORREF                   m_BKColor;
	double                     m_ImageZoom;		
	TPOINT2D                   m_ImageOffset;	
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC1;	
	CJetMemDC                  m_ImageWndMemDC2;	
	//---------------------------------------------------------------------------------//	
	double                     m_ImageTargetZoom;	
	TPOINT2D                   m_ImageTargetOffset;	
	RECT                       m_ImageTargetWndRect;
	CJetMemDC                  m_ImageTargetWndMemDC;		
	//---------------------------------------------------------------------------------//
	CDraw3DWnd                 m_Draw3DWnd;//3D繪圖視窗
	CImageMaskWnd              m_MaskImageWnd;//遮造圖像視窗
	CImagePhaseWnd             m_PhaseImageWnd;//相位圖像視窗	
	//---------------------------------------------------------------------------------//
	double                     m_ImageToStageScaleX;//影像轉機台的比例-X
	double                     m_ImageToStageScaleY;//影像轉機台的比例-Y
	//---------------------------------------------------------------------------------//
	double                     m_StageToImageScaleX;//機台轉影像的比例-X
	double                     m_StageToImageScaleY;//機台轉影像的比例-Y
	//---------------------------------------------------------------------------------//
	double                     m_StagePosX;
	double                     m_StagePosY;
	double                     m_StagePosZ;
	//---------------------------------------------------------------------------------//
	SLICE_FUNC_MODE            GetGrabSliceFuncMode() const;//取得實際取像的函式模式
	//---------------------------------------------------------------------------------//
	CALIBRATION_MODE           GetCalibrationMode() const;
	void                       SetCalibrationMode(CALIBRATION_MODE Mode);	
	//---------------------------------------------------------------------------------//	
	bool                       GetCalibrateAllCannel() const;
	void                       SetCalibrateAllCannel(bool bAll);
	//---------------------------------------------------------------------------------//	
	bool                       GetCalibrateAllCastID() const;
	void                       SetCalibrateAllCastID(bool bAll);
	//---------------------------------------------------------------------------------//	
	LIGHT_3D_CAST_ID           GetLight3DCastID() const;
	void                       SetLight3DCastID(LIGHT_3D_CAST_ID CastID);
	//---------------------------------------------------------------------------------//
	int                        GetDLPLEDCurrentID() const;
	//---------------------------------------------------------------------------------//	
	void                       ExtraDelayTime();
	void                       ExtraDelayTimeKernel();	
	bool                       CheckNeedExtraDelayTime();
	//---------------------------------------------------------------------------------//	
	void                       LockUIWnd(bool bLock);//鎖住視窗		
	bool                       EnableEditWnd(UINT CtrlID, BOOL bEnable);//啟用編輯控制項
	bool                       MoveToBeforeStagePosition();//移動至機台位置
	bool                       RetrieveStagePosition(bool UpdatePos=true);//取得機台位置
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	bool                       UpdateStagePosition();//更新機台座標
	bool                       UpdateStagePositionKernel(double PosX, double PosY, double PosZ);//更新機台座標
	bool                       Update3DCastCurrentToUI();//更新3D投光的電流值
	bool                       Update3DCastCurrentToUIKernel(LIGHT_3D_CAST_ID Light3DID);//更新3D投光的電流值
	//---------------------------------------------------------------------------------//	
	bool                       CreateTempFolder();//建立暫存資料夾
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();//重繪視窗
	void                       DrawHorLine(HDC hDC);//繪製水平線
	void                       DrawVerLine(HDC hDC);//繪製垂直線
	void                       DrawRectLine(HDC hDC);//繪區域直線
	void                       DrawGridLine(HDC hDC);//繪區域格子
	void                       DrawHFactorLine(HDC hDC);//繪區高度係數
	void                       DrawCrossLine(HDC hDC);//繪製十字線
	void                       DrawPhaseLine(HDC hDC);//繪製相位線
	void                       Draw3DCastFocus(HDC hDC);//繪製3D投光焦距數值
	void                       DrawImageFrame(HDC hDC);//繪製影像外框
	bool                       PtInControlWnd(const POINT &pt, UINT ControlID, POINT &pt2);	
	void                       DrawImageWndMemDC();//建立影像的記憶體圖像		
	
	void                       DrawWorstGridLine(CWnd *pWnd, HDC hDC);//繪區域格子	
	void                       DrawBigInfoText(CWnd *pWnd, HDC hDC, CString str);//影像灰階
	void                       DrawBigInfoWnd();//繪製大訊息視窗
	void                       Draw3DCastCurrentInfo(CWnd *pWnd, HDC hDC);//3D投光電流訊息
	void                       DrawImageTargetWnd();//繪製目標影像視窗
	void                       CreateImageTargetWndMemDC();//繪製目標影像的記憶體圖像	
	//---------------------------------------------------------------------------------//
	void                       ClearLogListBox();//清除紀錄列表視窗
	void                       AddLogListBox(LPCTSTR str);//加入紀錄列表視窗
	//---------------------------------------------------------------------------------//
	bool                       Check3DCastID(bool bShowMsg);//確認3D投光編號	
	bool                       CheckHeightFactorPhaseID();//確認高度比例的平面編號
	bool                       ConfigGrabParam(CALIBRATION_MODE Mode);//組態取像參數
	bool                       UpdatePhaseNoiseParamToUI();//更新相位雜訊定義	
	bool                       UpdatePhaseNoiseParamFromUI();//更新相位雜訊定義	
	bool                       CalibrateNext3DCastID(CALIBRATION_MODE Mode);//校正下一個3D投光
	bool                       GetFirstCali3DCastID(LIGHT_3D_CAST_ID &CastID);//取得第1個校正的3D投光
	bool                       GetPhaseNoiseParam(TPhaseNoiseParam &NoiseParam);//更新相位雜訊定義	
	SLICE_FUNC_MODE            CheckSliceFuncModeByBatchGrabPhaseMode(SLICE_FUNC_MODE SlicFuncMode, int BatchGrabPhaseMode) const;//依據SliceFuncMode與相位模式來取得對應的SliceFuncMode
	//---------------------------------------------------------------------------------//
	bool                       UpdateSpaceNoiseFilterParamToUI();//更新空間雜訊過濾參數
	bool                       UpdateSpaceNoiseFilterParamFromUI();//更新空間雜訊過濾參數
	bool                       GetSpaceNoiseFilterParam(TNoiseFilterParam &FilterParam);//更新空間雜訊過濾參數		
	//---------------------------------------------------------------------------------//
	bool                       ShowPixelInfo();
	bool                       ShowPixelInfo(const POINT &WndPt);
	bool                       GetCurrentDefaultRoi(int ImageW, int ImageH, TPOINT2D &Pt1, TPOINT2D &Pt2) const;
	//---------------------------------------------------------------------------------//		
	bool                       ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam);
	bool                       ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt);
	//---------------------------------------------------------------------------------//
	bool                       StartReGrab(BOOL ReStart);//開始取下一個像
	bool                       ExecReGrab(WPARAM wParam);//取下一個像
	//---------------------------------------------------------------------------------//
	void                       UpdateSliceParamToUI();
	bool                       ExecGrabFirst();//執行第一次取像
	bool                       ExecGrabNext();//執行下一次取像
	bool                       CheckNeedWaitForMoveDone();
	//---------------------------------------------------------------------------------//
	bool                       FocusToEditCtrl();
	//---------------------------------------------------------------------------------//		
	bool                       MoveToPhaseZeroPlane();//移動至相平面高度
	bool                       ExecPhaseZeroPlaneBtn();
	//---------------------------------------------------------------------------------//	
	bool                       ExecFOVWidth(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecFOVHeight(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecCameraAlignHor(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecCameraAlignVer(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       CalcCameraAlignHor3D(const TUNI_FRAME &UniFrame, double &HeightDif, double &HeightDifT, double &HeightDifB);
	bool                       CalcCameraAlignVer3D(const TUNI_FRAME &UniFrame, double &HeightDif, double &HeightDifL, double &HeightDifR);
	bool                       ExecCameraAlignHor3D(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecCameraAlignVer3D(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecImageFocus(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecImageResolution(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecImageFocusAuto(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       Exec2DLightAlign(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       Exec2DLightCurrent(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);	
	bool                       Exec3DCastAlign(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       Exec3DCastFocus(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       Exec3DCastCurrent(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);	
	bool                       ExecPhaseZeroPlane(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);	
	bool                       Exec3DModelTest(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       Exec3DModelTestMultiCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       Exec3DModelTestSingleCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       Exec3DModelTestSingleCastID_2(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       Exec3DModelDataMultiCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, TUNI_FRAME &DstUniFrame);
	bool                       Exec3DModelDataMultiCastID_1(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, TUNI_FRAME &DstUniFrame);
	bool                       Exec3DModelDataMultiCastID_2(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, TUNI_FRAME &DstUniFrame);
	bool                       Exec3DModelDataSingleCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, TUNI_FRAME &DstUniFrame);		
	bool                       Exec3DCastParamByCastID(CALIBRATION_MODE &CalMode, LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR &ImagePtr, TCastParam &CastParam);	
	bool                       Exec3DSpaceByCastID(CALIBRATION_MODE &CalMode, LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, bool bUseRoi, IMAGE_PTR &ImagePtr, TUNI_FRAME &RoiUniFrame);		
	bool                       ExecShow3DModelWnd(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, const TUNI_FRAME &ModelUniFrame);	

	bool                       ExecVerifyZeroPlaneData(int CastID, const TUNI_FRAME &UniFrame, CString &Str);		
	bool                       ExecVerifyZeroPlaneDataList(const TUNI_FRAME UniFrameList[], size_t UniFrameCount, CString &Str);	
	bool                       CalcMeanStdMaxMin(size_t Size, const SPACE_PTR Ptr, double &Mean, double &Std, double &Min, double &Max);
	bool                       ExecVerifyZeroPlane(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecVerifyZeroPlaneSingleCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecVerifyZeroPlaneMultiCastID(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);

	bool                       ExecPhaseHeightFactor_DOT(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecPhaseHeightFactor_FOV(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecPhaseHeightFactor_FOV_Z(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecPhaseHeightFactor_MultiFOV(CALIBRATION_MODE &CalMode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecPhaseHeightFactor_MultiFOVFunc();
	bool                       CheckHeightFactor(const TPhaseFactorGrid &Grid);//計算單格的局部高度比例平均
	bool                       ExecHeightFactor(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR ImagePtr, MASK_PTR MaskPtr, const PHASE_PTR Ptr, TPhaseFactorGrid &Grid);//計算單格的高度比例
	bool                       SaveHeightFactorList();
	bool                       CalcImageHeightFactor(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, SPACE_PTR Ptr);//計算整張的圖片參數
	bool                       TestImageHeightFactor2();
	bool                       MapGridPhaseToImage(const std::vector<TPhaseFactorGrid> &GridList, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, SPACE_PTR Ptr);//將Grid內插出影像數值(相位)	
	//---------------------------------------------------------------------------------//	
	bool                       SaveAutoFocusReading(const std::vector<TPOINT2D> &ReadingList);
	bool                       CalcBest2DCurrent(const std::vector<T2D_CURRENT> &ReadingList, double TargetGray, T2D_CURRENT &Best) const;
	bool                       Save2DCurrentReading(const TSliceParam &SliceParam, CALIBRATION_MODE CalMode, const std::vector<T2D_CURRENT> &ReadingList);
	//---------------------------------------------------------------------------------//	
	bool                       CalcPhaseNormLine();//計算相位法向量
	bool                       SetFovSize(double FOVW, double FOVH);//設定視野大小
	bool                       GetFovSize(double &FOVW, double &FOVH);//取得視野大小
	bool                       CalcFOVSizeResolution();//計算FOV尺寸的解析度
	bool                       ApplyFovResolutionToSystem();
	//---------------------------------------------------------------------------------//	
	bool                       SaveRawImageList(LPCTSTR Name, LIGHT_3D_CAST_ID CastID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtrList[], int ImageCount, int SaveTimes);
	bool                       CalcPhasePeriod1(LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, int PatternStep, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR ZeroPhasePtr, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, IMAGE_PTR ImagePtr, bool SaveRaw, TCastParam *CastParamPtr=NULL, int SaveTimes=0);//計算相位-周期x1
	bool                       CalcPhasePeriod2(LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, int PatternStep, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR ZeroPhasePtr, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, IMAGE_PTR ImagePtr, bool SaveRaw, TCastParam *CastParamPtr=NULL, int SaveTimes=0);//計算相位-周期x2
	bool                       CalcPhasePeriod3(LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, int PatternStep, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR ZeroPhasePtr, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, IMAGE_PTR ImagePtr, bool SaveRaw, int SaveTimes=0);//計算相位-周期x3
	bool                       CalcPhasePeriod2Exp2(LIGHT_3D_CAST_ID CastID, CAMERA_ID CameraID, int PatternStep, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, PHASE_PTR ZeroPhasePtr, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, IMAGE_PTR ImagePtr, bool SaveRaw, TCastParam *CastParamPtr=NULL, int SaveTimes=0);//計算相位-周期x2 曝光x2
	//---------------------------------------------------------------------------------//
	bool                       CheckUseRawPhaseData() const;//確認使用原始相位值
	bool                       InputPhaseZeroPlaneCorrect();//輸入相位-相平面修正
	bool                       InputPhaseHeightFactorCorrect();//輸入相位高度係數修正
	void                       ExecPhaseHeightFactor_DOT();
	bool                       ExecPhaseHeightFactor_DOT(LIGHT_3D_CAST_ID CastID);	
	void                       ExecPhaseHeightFactor_FOV();
	void                       ExecPhaseHeightFactor_MultiFOV();
	bool                       ExecPhaseHeightFactor_MultiFOV(LIGHT_3D_CAST_ID CastID);
	//---------------------------------------------------------------------------------//		
	bool                       GetSaveRawImage() const;	
	bool                       CreateRawImageFolder();//建立原圖資料夾	
	int                        GetSaveRawImageTimes() const;
	void                       SetSaveRawImageTimes(int val);
	CString                    GetSaveRawImageFolder() const;
	CString                    GetSaveRawImageExtName();
	//---------------------------------------------------------------------------------//
	bool                       ExecCaliAlignWndMsg(WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//
	bool                       CalcObject(IMAGE_SIZE ModelW, IMAGE_SIZE ModelH, IMAGE_SIZE ModelStep, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, RECT &ObjRect, float &ObjH);//求得物體的資料
	//---------------------------------------------------------------------------------//
	bool                       ExecSelchange3DCastCurrentIDCombo();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCaliPaneAlign)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCameraCombo();
	afx_msg void OnGrabBtn();
	afx_msg void OnFOVWidthBtn();
	afx_msg void OnFOVHeightBtn();
	afx_msg void OnCameraAlignHorBtn();
	afx_msg void OnCameraAlignVerBtn();	
	afx_msg void OnImageResolutionCalcBtn();
	afx_msg void OnImageFocusAutoBtn();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLightAlignBtn();
	afx_msg void On3DCastAlignBtn();
	afx_msg void On3DCastFocusBtn();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeManipulateCombo();
	afx_msg void OnGrabRepeatChk();
	afx_msg void OnLightCurrentBtn();
	afx_msg void On3DCastCurrentBtn();
	afx_msg void OnSelchangeLightCombo();
	afx_msg void OnSelchange3DCastIDCombo();
	afx_msg void OnPhasePeriodBtn();
	afx_msg void OnPatternZeroPlaneBtn();
	afx_msg void OnPatternHeightFactorBtn();
	afx_msg void OnViewResetBtn();
	afx_msg void OnTargetGridGoBtn();
	afx_msg void OnTargetGridSetBtn();
	afx_msg void OnTargetGridExpBtn();
	afx_msg void OnTargetWhiteGoBtn();
	afx_msg void OnTargetWhiteSetBtn();
	afx_msg void OnTargetWhiteExpBtn();
	afx_msg void OnTargetHeightGoBtn();
	afx_msg void OnTargetHeightSetBtn();
	afx_msg void OnTargetHeightExpBtn();
	afx_msg void OnPatternPhaseMeasureChk();
	afx_msg void OnSaveImageBtn();
	afx_msg void OnPatternHeightFactorShowBtn();
	afx_msg void OnPatternImageBtn();
	afx_msg void OnVerifyZeroPlaneBtn();	
	afx_msg void OnPatternZeroPlaneShowBtn();
	afx_msg void OnMotionWndBtn();
	afx_msg void OnDeltaposMotionZPitchSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnXYZOrgBtn();
	afx_msg void OnPCBInBtn();
	afx_msg void OnPCBOutBtn();
	afx_msg void OnPCBBackBtn();
	afx_msg void OnPCBClampOnBtn();
	afx_msg void OnPCBClampOffBtn();
	afx_msg void OnTargetCapOnBtn();
	afx_msg void OnTargetCapOffBtn();
	afx_msg void OnSelchangeFovResolutionCombo();
	afx_msg void OnViewAllBtn();
	afx_msg void OnView1x1Btn();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCameraExposureTimeBtn();	
	afx_msg void OnShowCenterLineChk();
	afx_msg void OnShowHorLineChk();
	afx_msg void OnShowVerLineChk();	
	afx_msg void OnHideLineBtn();
	afx_msg void OnSelchangeSliceCombo();
	afx_msg void OnDLPExposureTimeBtn();
	afx_msg void On3DCastCurrentSetBtn();
	afx_msg void OnLightCurrentSetBtn();
	afx_msg void OnSelchangePhaseLedCombox();
	afx_msg void OnBasePlaneParamBtn();
	afx_msg void OnSpaceNoiseFilterBtn();
	afx_msg void OnPhaseHeightFactorSetBtn();
	afx_msg void OnPhaseHeightFactorClearBtn();
	afx_msg void OnPhaseHeightFactorShowTableBtn();
	afx_msg void OnParamFileReloadBtn();
	afx_msg void OnSelchange3DCastCurrentIDCombo();
	afx_msg void OnSelchangeParamPhaseToHeightCombo();
	afx_msg void OnShowRawImageFolderBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIPANEALIGN_H__2963833E_1B50_4137_8D17_E966D4C7345D__INCLUDED_)
