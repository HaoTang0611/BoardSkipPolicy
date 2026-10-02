#if !defined(AFX_CALIPANESTAGE_H__6A79DF7D_8528_4C75_984F_3F32CE3EA005__INCLUDED_)
#define AFX_CALIPANESTAGE_H__6A79DF7D_8528_4C75_984F_3F32CE3EA005__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaliPaneStage.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "MotionCtrlWnd.h"
#include "JETListCtrl.h"
#include "MapCoordinate.h"
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
enum DOT_ALIGN_MODE
{
	DOT_ALIGN_BLOB,
	DOT_ALIGN_MATCH,
	DOT_ALIGN_HOUGH_CIRCLE,
	DOT_ALIGN_RETURN
};
//-------------------------------------------------------------------------------------//
enum DOT_CORNER_ID
{
	DOT_CORNER_ORG = 0,
	DOT_CORNER_1  = DOT_CORNER_ORG,
	DOT_CORNER_2  = 1,
	DOT_CORNER_3  = 2,
	DOT_CORNER_4  = 3,
	DOT_CORNER_TOTAL
};
//-------------------------------------------------------------------------------------//
enum CALIBRATION_STAGE_MODE
{
	CALIBRATION_STAGE_STOP,
	CALIBRATION_STAGE_DOT_ALIGN,//對齊第1點

	CALIBRATION_STAGE_DOT_CAMERA,//對齊相機

	CALIBRATION_STAGE_X_POS_ALIGN,//對齊X軸
	CALIBRATION_STAGE_X_POS_ALIGN_1,//對齊X軸第1點
	CALIBRATION_STAGE_X_POS_ALIGN_2,//對齊X軸第2點
	CALIBRATION_STAGE_X_POS_VERIFY_1,//驗證X軸第1點
	CALIBRATION_STAGE_X_POS_VERIFY_2,//驗證X軸第2點	
	CALIBRATION_STAGE_X_POS_VERIFY_EACH,//驗證X軸每一點

	CALIBRATION_STAGE_Y_POS_ALIGN,//對齊X軸
	CALIBRATION_STAGE_Y_POS_ALIGN_1,//對齊X軸第1點
	CALIBRATION_STAGE_Y_POS_ALIGN_2,//對齊X軸第2點
	CALIBRATION_STAGE_Y_POS_VERIFY_1,//驗證Y軸第1點
	CALIBRATION_STAGE_Y_POS_VERIFY_2,//驗證Y軸第2點
	CALIBRATION_STAGE_Y_POS_VERIFY_EACH,//驗證Y軸每一點

	CALIBRATION_STAGE_ALIGN_CORNER,//驗證四角落
	CALIBRATION_STAGE_ALIGN_CORNER_1,//驗證四角落第1點
	CALIBRATION_STAGE_ALIGN_CORNER_2,//驗證四角落第2點
	CALIBRATION_STAGE_ALIGN_CORNER_3,//驗證四角落第3點
	CALIBRATION_STAGE_ALIGN_CORNER_4,//驗證四角落第4點

	CALIBRATION_STAGE_CALIBRATE_DOT_NODE,//校正黑點

	CALIBRATION_STAGE_RETURN
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliPaneStage dialog
//-------------------------------------------------------------------------------------//
class CCaliPaneStage : public CDialog
{
// Construction
public:
	CCaliPaneStage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCaliPaneStage)
	enum { IDD = IDD_CALIBRATION_PANE_STAGE };
	CComboBox	m_AlignModeCombox;
	CComboBox	m_AlignCountCombox;	
	CThisListCtrl m_DotListCtrl;
	CComboBox	m_SliceCombox;
	CComboBox	m_LaneIDCombox;
	CStatic	m_ImageWnd;
	CStatic	m_DotImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaliPaneStage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//		
	void                       SetSpaceBuffer(size_t Size, SPACE_PTR Buffer, SPACE_PTR Buffer1);
	void                       SetShowBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1);
	void                       SetImageBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1, IMAGE_PTR Buffer2, PHASE_PTR PhaseBuffer, PHASE_PTR PhaseBuffer1, PHASE_PTR PhaseBuffer2, MASK_PTR MaskBuffer);
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//
	bool                       m_LockUIWnd;
	bool                       m_LockUIAlign;//鎖住對齊
	bool                       m_LockUIAlignHor;//鎖住水平調整
	bool                       m_LockUIAlignVer;//鎖住垂直確認
	bool                       m_LockUIAlignCorner;//鎖住四角落確認
	bool                       m_LockUIAlignDotNode;//鎖住玻璃點校正
	//---------------------------------------------------------------------------------//
	CMotionCtrlWnd             m_MotionCtrlWnd;	
	//---------------------------------------------------------------------------------//
	TFuncTickCount             m_FuncTime;
	TFuncTickCount             m_FuncTimeFd;		
	//---------------------------------------------------------------------------------//	
	COLORREF                   m_BKColor;
	double                     m_ImageZoom;		
	TPOINT2D                   m_ImageOffset;	
	RECT                       m_ImageWndRect;	
	CJetMemDC                  m_ImageWndMemDC1;	
	CJetMemDC                  m_ImageWndMemDC2;	
	int                        m_ImageFontHeight;
	//---------------------------------------------------------------------------------//			
	COLORREF                   m_DotBKColor;
	double                     m_DotImageZoom;		
	TPOINT2D                   m_DotImageOffset;	
	RECT                       m_DotImageWndRect;
	CJetMemDC                  m_DotImageWndMemDC1;
	//---------------------------------------------------------------------------------//
	bool                       SaveParamFile();//儲存參數檔案
	bool                       LoadParamFile();//載入參數檔案	
	//---------------------------------------------------------------------------------//
	bool                       SaveImageFile();//存圖
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	IMAGE_PTR                  m_DotImagePtr;
	IMAGE_SIZE                 m_DotImageW;
	IMAGE_SIZE                 m_DotImageH;
	IMAGE_SIZE                 m_DotImageStep;
	IMAGE_SIZE                 m_DotImageBitCount;	
	//---------------------------------------------------------------------------------//	
	CTime                      m_DotDateTime;//校正時間
	int                        m_DotCountX;
	int                        m_DotCountY;
	int                        m_DotSkipX;
	int                        m_DotSkipY;
	double                     m_DotPitchX;
	double                     m_DotPitchY;		
	int                        m_DotWidth;//um
	int                        m_DotHeight;//um
	int                        m_DotSizeW;//Pixel
	int                        m_DotSizeH;//Pixel
	int                        m_DotMarginW;
	int                        m_DotMarginH;	
	int                        m_DotPatternW;
	int                        m_DotPatternH;
	bool                       m_DotWhiteMode;
	SIZE                       m_DotRoiSize;
	DOT_ALIGN_MODE             m_DotAlignMode;	
	double                     m_ErrorOffsetX;//X偏差誤差上限
	double                     m_ErrorOffsetY;//Y偏差誤差上限
	double                     m_AlignTolerance_DOT;//原點對齊誤差上限
	double                     m_AlignTolerance_HOR;//水平對齊誤差上限
	double                     m_AlignTolerance_Ver;//垂直對齊誤差上限
	//---------------------------------------------------------------------------------//		
	TPOINT3D                   m_StagePos;
	int                        m_Threshold;
	double                     m_DotHorGapY;
	double                     m_DotVerGapX;
	TPOINT3D                   m_DotOrgStagePos;//基準點位置
	TPOINT3D                   m_DotOrgOffsetPos;//基準點偏差
	TPOINT3D                   m_DotStagePosHor;
	TPOINT3D                   m_DotStagePosVer;
	TPOINT3D                   m_DotCornerPos[DOT_CORNER_TOTAL];
	TPOINT3D                   m_DotMatchedCornerPos[DOT_CORNER_TOTAL];		
	bool                       m_DotNodeCanBeXYTable;	
	int                        m_DotNodeIndex;
	std::vector<TDotNode>      m_DotNodeList;
	std::vector<TDotNode>      m_DotHorLineList;
	std::vector<TDotNode>      m_DotVerLineList;	
	//---------------------------------------------------------------------------------//
	LANE_ID                    m_LaneID;
	CAMERA_ID                  m_CameraID;	
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;		
	CALIBRATION_STAGE_MODE     m_CaliStageMode;
	TSliceParam                m_SliceParam;
	bool                       m_ShowRoiRect;
	bool                       m_ShowCrossLine;
	bool                       m_ShowCursorLine;
	bool                       m_ShowDotMatched;	
	int                        m_CaliRepeatCount;	
	CString                    m_strImageValue;
	CString                    m_strCaliStep;
	bool                       m_ShowCaliStep;//顯示校正步驟
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_ShowBuffer;
	IMAGE_PTR                  m_ShowBuffer1;	
	//---------------------------------------------------------------------------------//
	MASK_PTR                   m_MaskBuffer;
	PHASE_PTR                  m_PhaseBuffer;
	PHASE_PTR                  m_PhaseBuffer1;
	PHASE_PTR                  m_PhaseBuffer2;	
	IMAGE_PTR                  m_ImageBuffer;
	IMAGE_PTR                  m_ImageBuffer1;
	IMAGE_PTR                  m_ImageBuffer2;	
	//---------------------------------------------------------------------------------//
	SPACE_PTR                  m_SpaceBuffer;
	SPACE_PTR                  m_SpaceBuffer1;//空間記憶體指標	
	//---------------------------------------------------------------------------------//
	size_t                     m_ShowBufferSize;
	size_t                     m_ImageBufferSize;
	size_t                     m_SpaceBufferSize;
	//---------------------------------------------------------------------------------//
	POINT                      m_MovingPos;
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;		
	//---------------------------------------------------------------------------------//		
	bool                       m_DotMatched;
	TSIZE2D                    m_DotMatchedSz;
	TPOINT2D                   m_DotMatchedPt;	
	TPOINT2D                   m_DotMatchedOffset;//Stage Offset
	TPOINT2D                   m_DotMatchedScale;	
	double                     m_DotMatchedScore;	
	std::vector<TPOINT2D>      m_DotMatchedPtList;
	double                     GetDotMatchedPtX() const;
	double                     GetDotMatchedPtY() const;
	double                     GetDotMatchedOffsetX() const;
	double                     GetDotMatchedOffsetY() const;
	//---------------------------------------------------------------------------------//	
	CMapCoordinate             m_AlignedMap;//玻璃板座標對齊轉換
	int                        m_AlignedCount;//使用玻璃板座標對齊數量-FD
	bool                       m_AlignedMapUsed;//使用玻璃板座標對齊轉換
	std::vector<TPOINT2D>      m_AlignedFdCadList;//使用玻璃板座標對齊對位點Cad座標
	std::vector<TPOINT2D>      m_AlignedFdResList;//使用玻璃板座標對齊對位點結果座標
	void                       ClearAlignedFdList();
	void                       SetAlignedMapUsed(bool val);//使用玻璃板座標對齊轉換
	bool                       GetAlignedMapUsed() const;//使用玻璃板座標對齊轉換
	//---------------------------------------------------------------------------------//
	CAMERA_ID                  GetCameraID();
	COLORREF                   GetColorNG();//取得NG顏色
	COLORREF                   GetColorOK();//取得OK顏色
	COLORREF                   GetColorSel();//取得選取顏色
	COLORREF                   GetColorNone();//取得未處理顏色
	int                        GetThreshold();//取得2值化閥值
	double                     GetUnitRatio();//取得單位比例		
	double                     GetErrorOffsetX();
	double                     GetErrorOffsetY();
	double                     GetHorTolerance();//取得水平公差範圍
	double                     GetVerTolerance();//取得水平公差範圍	
	double                     GetAlignTolerance();//取得對齊公差範圍	
	BOX_SHAPE_MODE             GetDotShapeMode();	
	DWORD                      GetMoveDownDelyTime();//移動完成延遲時間	
	//---------------------------------------------------------------------------------//
	bool                       BuildDotAlignModeCombox();
	bool                       BuildDotAlignCountCombox();
	//---------------------------------------------------------------------------------//
	bool                       BuildDotListWndHeader();
	bool                       BuildDotListWnd();
	bool                       UpdateDotListWnd();
	bool                       EnsureVisibleDotListData(int nData);
	//---------------------------------------------------------------------------------//
	bool                       BuildDotImage();
	bool                       ReleaseDotImage();
	//---------------------------------------------------------------------------------//	
	bool                       StartReGrab(BOOL ReStart);//開始取下一個像
	bool                       ExecReGrab(WPARAM wParam);//取下一個像
	bool                       FocusToEditCtrl();
	//---------------------------------------------------------------------------------//
	bool                       ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam);
	bool                       ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt);
	//---------------------------------------------------------------------------------//
	bool                       ExecGrabFirst();//執行第一次取像
	bool                       ExecGrabNext();//執行下一次取像
	bool                       ConfigGrabParam(CALIBRATION_STAGE_MODE Mode);//組態取像參數
	//---------------------------------------------------------------------------------//
	void                       SetShowCaliStep(bool bShow);
	//---------------------------------------------------------------------------------//
	LANE_ID                    GetActiveLaneID() const;
	void                       SetActiveLaneID(LANE_ID LaneID);
	//---------------------------------------------------------------------------------//
	CALIBRATION_STAGE_MODE     GetCalibrationMode() const;
	void                       SetCalibrationMode(CALIBRATION_STAGE_MODE Mode);	
	//---------------------------------------------------------------------------------//	
	bool                       ExecXYMoveTo(CALIBRATION_STAGE_MODE Mode, double PosX, double PosY);//2軸移動，但非同動唷	
	//---------------------------------------------------------------------------------//
	void                       SetLockUIAlign(bool bLock);//鎖住對齊			
	void                       SetLockUIAlignHor(bool bLock);//鎖住水平調整
	void                       SetLockUIAlignVer(bool bLock);//鎖住垂直確認
	void                       SetLockUIAlignCorner(bool bLock);//鎖住四角落確認
	void                       SetLockUIAlignDotNode(bool bLock);//鎖住玻璃點校正	
	//---------------------------------------------------------------------------------//	
	bool                       GetLockUIWnd() const;
	void                       LockUIWnd(bool bLock);//鎖住視窗	
	void                       SetRepeatGrabBtnCheck(bool bCheck);//啟用重複取像
	bool                       GetRepeatGrabBtnChecked();
	bool                       RetrieveStagePosition(bool UpdatePos=true);//取得機台位置
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像	
	//---------------------------------------------------------------------------------//	
	void                       ClearLogListBox();//清除紀錄列表視窗
	void                       AddLogListBox(LPCTSTR str);//加入紀錄列表視窗
	//---------------------------------------------------------------------------------//	
	void                       SetInfoText(LPCTSTR Text);
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	void                       DrawDotImageWnd();
	void                       DrawDotImage(HDC hDC);	
	//---------------------------------------------------------------------------------//	
	void                       DrawImageWnd();
	void                       DrawImageWndMemDC();	
	void                       DrawRoiRect(HDC hDC);//繪製搜尋範圍
	void                       DrawCrossLine(HDC hDC);//繪製十字線
	void                       DrawCursorLine(HDC hDC);//繪製鼠標線
	void                       DrawDotMatched(HDC hDC);//繪製點找到的位置
	void                       DrawDotMatched(HDC hDC, double ImgPosX, double ImgPosY);//繪製點找到的位置
	//---------------------------------------------------------------------------------//	
	void                       DrawDotMap(HDC hDC);//繪製點結果	
	//---------------------------------------------------------------------------------//		
	bool                       CreateTempFolder();//建立暫存資料夾
	//---------------------------------------------------------------------------------//	
	int                        GetDotCountX() const;
	int                        GetDotCountY() const;
	double                     GetDotPitchX() const;
	double                     GetDotPitchY() const;
	bool                       GetDotRoiSize(int &W, int &H);
	int                        GetDotPeriod(int DotSkip) const;
	int                        CalcDotCount(int DotSkip, int DotCount) const;
	CString                    GetDebugFolder() const;
	//---------------------------------------------------------------------------------//		
	void                       ResetDotSearch();//重置點搜尋	
	bool                       ExecDotSearch();//執行點搜尋
	bool                       ExecDotSearch(double DotX, double DotY);//執行點搜尋
	bool                       ExecDotSearch_Blob(double DotX, double DotY);//執行點區塊搜尋
	bool                       ExecDotSearch_Match(double DotX, double DotY);//執行點匹配	
	bool                       ExecDotSearch_HoughCircle(double DotX, double DotY);//執行點搜尋霍夫圓形
	//---------------------------------------------------------------------------------//		
	bool                       SavePatImage();
	bool                       ExecDotSaveRoiImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);//執行儲存區域影像
	//---------------------------------------------------------------------------------//	
	bool                       ExecOtherDotSearch();//執行其他點搜尋
	//---------------------------------------------------------------------------------//		
	double                     GetDotOrgStagePosX() const;
	double                     GetDotOrgStagePosY() const;
	void                       SetDotOrgStagePos(double PosX, double PosY);
	//---------------------------------------------------------------------------------//		
	double                     GetDotOrgOffsetPosX() const;
	double                     GetDotOrgOffsetPosY() const;
	void                       SetDotOrgOffsetPos(double OffsetX, double OffsetY);
	//---------------------------------------------------------------------------------//		
	bool                       BuildDotNodeList();//建立每個點的XY校正表
	bool                       UpdateDotHorVerPos();//計算點的水平與垂直位置		
	bool                       UpdateDotParamFromUI();//更新UI至點參數
	bool                       ExecOrgDotAlign(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecDotCameraAlign(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	//---------------------------------------------------------------------------------//
	bool                       ExecXVerifyX1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecXVerifyX2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecXVerifyXEach(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecXAlignHorLineX1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecXAlignHorLineX2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	//---------------------------------------------------------------------------------//
	bool                       ExecYVerifyY1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecYVerifyY2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecYVerifyYEach(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecYAlignVerLineY1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecYAlignVerLineY2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	//---------------------------------------------------------------------------------//
	bool                       AnalyzeCornerPos();//分析四角落的點座標
	bool                       ExecAlignCornerPos1(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecAlignCornerPos2(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecAlignCornerPos3(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	bool                       ExecAlignCornerPos4(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	//---------------------------------------------------------------------------------//	
	bool                       BuildStageXYCaliTable();//建立機台XY校正表		
	bool                       SaveStageDotCompareFile();//輸出機台Dot比較檔案
	bool                       ExecCalibrateDotNode(CALIBRATION_STAGE_MODE Mode, CAMERA_ID CameraID, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool &bFinish);
	//---------------------------------------------------------------------------------//
	bool                       SaveOffsetFile(bool bXaxis, std::vector<TPOINT2D> &List);
	bool                       SaveDotNodeFile(std::vector<TDotNode> &List, bool bShowFd);
	bool                       SaveDotNodeFile(LPCTSTR filename, std::vector<TDotNode> &List, bool bShowFd, bool bShowFile=true);
	//---------------------------------------------------------------------------------//
	bool                       PickDotNodeList(POINT pt);
	void                       UpdateImageValue(POINT pt);
	//---------------------------------------------------------------------------------//
	int                        GetGantryAxis() const;
	//---------------------------------------------------------------------------------//
	bool                       CheckCornerLimit();
	bool                       ExecCalibrateDotBtn();	
	bool                       AnalyzeAlignedMap();//分析座標轉換-玻璃板角度偏差
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCaliPaneStage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDotBuildBtn();
	afx_msg void OnGrabBtn();
	afx_msg void OnSelchangeSliceCombo();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDotMatchBtn();
	afx_msg void OnShowCenterLineChk();
	afx_msg void OnShowCursorLineChk();
	afx_msg void OnMotionWndBtn();
	afx_msg void OnDotAlignBtn();
	afx_msg void OnAlignX12Btn();	
	afx_msg void OnMoveToX1Btn();
	afx_msg void OnMoveToX2Btn();	
	afx_msg void OnVerifyXBtn();
	afx_msg void OnAlignY12Btn();
	afx_msg void OnMoveToY1Btn();
	afx_msg void OnMoveToY2Btn();
	afx_msg void OnVerifyYBtn();
	afx_msg void OnAlignCornerBtn();
	afx_msg void OnCalibrateDotBtn();
	afx_msg void OnBuildXYDotTableBtn();
	afx_msg void OnEnableXYCaliChk();
	afx_msg void OnItemchangedDotListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDotListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkDotListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetTolBtn();
	afx_msg void OnShowDotMapChk();
	afx_msg void OnDotErrSetBtn();
	afx_msg void OnMoveToCornerPosBtn1();
	afx_msg void OnMoveToCornerPosBtn2();
	afx_msg void OnMoveToCornerPosBtn3();
	afx_msg void OnMoveToCornerPosBtn4();
	afx_msg void OnAnalzeCornerBtn();
	afx_msg void OnDotSaveBtn();
	afx_msg void OnSaveImageBtn();
	afx_msg void OnGantryFetchOffsetBtn();
	afx_msg void OnGantrySetStdOffsetBtn();	
	afx_msg void OnAlignCameraBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIPANESTAGE_H__6A79DF7D_8528_4C75_984F_3F32CE3EA005__INCLUDED_)
