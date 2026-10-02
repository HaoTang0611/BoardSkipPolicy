#if !defined(AFX_IMAGEWND_H__53B79DCF_238E_4927_9F3D_B3E39EC2B9F6__INCLUDED_)
#define AFX_IMAGEWND_H__53B79DCF_238E_4927_9F3D_B3E39EC2B9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
enum EDIT_GRID_MODE
{
	EDIT_GRID_NULL=0,
	EDIT_GRID_INSIDE,
	EDIT_GRID_LEFT,
	EDIT_GRID_RIGHT,
	EDIT_GRID_TOP,
	EDIT_GRID_BOTTOM,
	EDIT_GRID_CENTER_LEFT,
	EDIT_GRID_CENTER_RIGH,
	EDIT_GRID_CENTER_TOP,
	EDIT_GRID_CENTER_BOTTOM,
	EDIT_GRID_LEFT_TOP,
	EDIT_GRID_RIGHT_TOP,
	EDIT_GRID_LEFT_BOTTOM,
	EDIT_GRID_RIGHT_BOTTOM,	
	EDIT_GRID_OUTSIDE,
	EDIT_GRID_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMAGE_RBTN_CLICK_MODE
{
	IMAGE_RBTN_CLICK_NULL,//右鍵功能-無
	IMAGE_RBTN_CLICK_CTRL_VIEW,//右鍵功能-顯示操作
	IMAGE_RBTN_CLICK_MOVE_PROJECT,//右鍵功能-移動專案
	IMAGE_RBTN_CLICK_MOVE_PANEL,//右鍵功能-移動整板
	IMAGE_RBTN_CLICK_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMAGE_LBTN_CLICK_MODE
{
	IMAGE_LBTN_CLICK_NULL,//左鍵功能-無
	IMAGE_LBTN_CLICK_EDIT_BOX,//左鍵功能-編輯框
	IMAGE_LBTN_CLICK_MOVE_PROJECT,//左鍵功能-移動專案
	IMAGE_LBTN_CLICK_MOVE_PANEL,//左鍵功能-移動整板	
	IMAGE_LBTN_CLICK_MOVE_PANEL_STAGE,//左鍵功能-移動整板的機台座標
	IMAGE_LBTN_CLICK_MOVE_PROJECT_STAGE,//左鍵功能-移動專案的機台座標
	IMAGE_LBTN_CLICK_SELECT_COMPONENT,//左鍵功能-選取零件
	IMAGE_LBTN_CLICK_MEASURE,//左鍵功能-量測
	IMAGE_LBTN_CLICK_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMAGE_RBTN_DBCLICK_MODE
{
	IMAGE_RBTN_DBCLICK_NULL,//右鍵雙擊功能-無
	IMAGE_RBTN_DBCLICK_FIT_ZOOM,//右鍵雙擊功能-全景檢視
	IMAGE_RBTN_DBCLICK_POPUP_MENU,//右鍵雙擊功能-蹦跳選單
	IMAGE_RBTN_DBCLICK_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMAGE_LBTN_DBCLICK_MODE
{
	IMAGE_LBTN_DBCLICK_NULL,//左鍵雙擊功能-無
	IMAGE_LBTN_DBCLICK_FIT_ZOOM,//左鍵雙擊功能-全景檢視
	IMAGE_LBTN_DBCLICK_STAGE_MOVE_TO,//左鍵雙擊功能-移動至
	IMAGE_LBTN_DBCLICK_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMAGE_RBTN_UP_MODE//右鍵放開
{
	IMAGE_RBTN_UP_NULL,//右鍵放開功能-無
	IMAGE_RBTN_UP_MOVE_STAGE,//右鍵放開功能-移動機台
	IMAGE_RBTN_UP_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMAGE_LBTN_UP_MODE//左鍵放開
{
	IMAGE_LBTN_UP_NULL,//左鍵放開功能-無	
	IMAGE_LBTN_UP_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMAGE_WND_DRAW_PROJECT_MODE//影像視窗繪製專案模式
{	
	IMAGE_WND_DRAW_PROJECT_NORMAL     = 1,//影像視窗繪製專案模式-正常
	IMAGE_WND_DRAW_PROJECT_INSPECTING = 2, //影像視窗繪製專案模式-檢測中		
	IMAGE_WND_DRAW_PROJECT_RETURN
};
//-------------------------------------------------------------------------------------//
enum IMAGE_DATA_SRC//影像資料來源
{
	IMAGE_DATA_NONE, //影像資料來源-未定義
	IMAGE_DATA_FOV,  //影像資料來源-視野
	IMAGE_DATA_MAP,  //影像資料來源-底圖
	IMAGE_DATA_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct _EditGrid
{
	RECT            sEditGrid;
	EDIT_GRID_MODE  sEditMode;	
} TEditGrid, *PEditGrid;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageWnd window
//-------------------------------------------------------------------------------------//
class CImageWnd : public CStatic
{
// Construction
public:
	CImageWnd();

// Attributes
public:

// Operations
public:
	//---------------------------------------------------------------------------------//		
	void                       SetShowLaneID(LANE_ID val);
	LANE_ID                    GetShowLaneID() const;
	//---------------------------------------------------------------------------------//		
	void                       SetProjectPtr(CAOIProject *ProjectPtr);		
	void                       SetImageInfo(CAMERA_ID CameraID, const TREGION4D &StageRgn, const TPOINT2D &Res, IMAGE_DATA_SRC SrcMode);//影像在機台位置
	//---------------------------------------------------------------------------------//	
	void                       ResetLBtnPos();//復歸滑鼠左鍵
	void                       ResetRBtnPos();//復歸滑鼠右鍵
	//---------------------------------------------------------------------------------//
	IMAGE_RBTN_UP_MODE         GetRBtnUpMode() const;
	void                       SetRBtnUpMode(IMAGE_RBTN_UP_MODE value);//右鍵放開功能	
	//---------------------------------------------------------------------------------//
	IMAGE_LBTN_UP_MODE         GetLBtnUpMode() const;
	void                       SetLBtnUpMode(IMAGE_LBTN_UP_MODE value);//左鍵放開功能
	//---------------------------------------------------------------------------------//
	IMAGE_RBTN_CLICK_MODE      GetRBtnClickMode() const;//取得右鍵控制模式
	void                       SetRBtnClickMode(IMAGE_RBTN_CLICK_MODE value);//設定右鍵控制模式
	//---------------------------------------------------------------------------------//
	IMAGE_LBTN_CLICK_MODE      GetLBtnClickMode() const;//取得左鍵控制模式	
	void                       SetLBtnClickMode(IMAGE_LBTN_CLICK_MODE value);//設定左鍵控制模式	
	//---------------------------------------------------------------------------------//
	IMAGE_RBTN_DBCLICK_MODE    GetRBtnDbClickMode() const;//取得右鍵雙擊模式	
	void                       SetRBtnDbClickMode(IMAGE_RBTN_DBCLICK_MODE value);//設定右鍵雙擊模式	
	//---------------------------------------------------------------------------------//
	IMAGE_LBTN_DBCLICK_MODE    GetLBtnDbClickMode() const;//取得左鍵雙擊模式	
	void                       SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_MODE value);//設定左鍵雙擊模式	
	//---------------------------------------------------------------------------------//		
	void                       SetBKColor(COLORREF bkColor);	
	void                       SetBKHatch(int Type=HS_API_MAX, COLORREF Color=0xFFFFFF);
	void                       RedrawWnd(BOOL bRedrawBK);//視窗重繪		
	//---------------------------------------------------------------------------------//	
	void                       SetShowAll(bool bShow);//顯示全部
	//---------------------------------------------------------------------------------//	
	bool                       GetShowLBtnPos() const;//啟用滑鼠左鍵		
	void                       SetShowLBtnPos(bool value);//啟用滑鼠左鍵		
	//---------------------------------------------------------------------------------//
	bool                       GetShowEditLine() const;//顯示編輯線段
	void                       SetShowEditLine(bool value);//顯示編輯線段
	//---------------------------------------------------------------------------------//		
	bool                       GetShowEditPosText() const;//顯示編輯位置文字
	void                       SetShowEditPosText(bool value);//顯示編輯位置文字
	//---------------------------------------------------------------------------------//		
	bool                       GetShowCursorInfo() const;//顯示滑鼠訊息
	void                       SetShowCursorInfo(bool value);//顯示滑鼠訊息
	//---------------------------------------------------------------------------------//	
	bool                       GetShowPanel() const;
	void                       SetShowPanel(bool bShow);//顯示專案整板	
	//---------------------------------------------------------------------------------//	
	bool                       GetShowBoard() const;
	void                       SetShowBoard(bool bShow);//顯示專案單板	
	//---------------------------------------------------------------------------------//	
	bool                       GetShowMark() const;
	void                       SetShowMark(bool bShow);//顯示專案特徵點
	//---------------------------------------------------------------------------------//	
	bool                       GetShowFiducial() const;
	void                       SetShowFiducial(bool bShow);//顯示專案定位點	
	//---------------------------------------------------------------------------------//		
	bool                       GetShowSystem() const;//顯示系統
	void                       SetShowSystem(bool bShow);//顯示系統
	//---------------------------------------------------------------------------------//	
	bool                       GetShowBarcode() const;//顯示專案條碼
	void                       SetShowBarcode(bool bShow);//顯示專案條碼
	//---------------------------------------------------------------------------------//	
	bool                       GetShowComponent() const;//顯示專案零件
	void                       SetShowComponent(bool bShow);//顯示專案零件
	//---------------------------------------------------------------------------------//	
	bool                       GetShowDistrictRect() const;//顯示分段區間
	void                       SetShowDistrictRect(bool bShow);//顯示分段區間
	//---------------------------------------------------------------------------------//		
	bool                       GetShowComponentName() const;//顯示專案零件名稱	
	void                       SetShowComponentName(bool bShow);//顯示專案零件名稱	
	//---------------------------------------------------------------------------------//	
	bool                       GetShowComponentDefectOnly() const;//顯示專案瑕疵零件
	void                       SetShowComponentDefectOnly(bool bShow);//顯示專案瑕疵零件
	//---------------------------------------------------------------------------------//
	bool                       GetShowFieldRgn() const;//設定區域框線
	void                       SetShowFieldRgn(bool bShow, COLORREF clr=0xFF8080);//設定區域框線	
	//---------------------------------------------------------------------------------//	
	bool                       GetShowCameraPos() const;//顯示相機中心位置	
	void                       SetShowCameraPos(bool bShow, COLORREF clr=0xFF8080);//顯示相機中心位置	
	//---------------------------------------------------------------------------------//	
	bool                       GetShowCameraRgn() const;//顯示相機位置	
	void                       SetShowCameraRgn(bool bShow, COLORREF clr=0xFF8080);//顯示相機位置	
	//---------------------------------------------------------------------------------//	
	bool                       GetShowCursorLine() const;//顯示鼠標線	
	void                       SetShowCursorLine(bool bShow, COLORREF clr=0x20A0A0);//顯示鼠標線	
	//---------------------------------------------------------------------------------//	
	bool                       GetShowWndCenterLine() const;//顯示視窗十字線
	void                       SetShowWndCenterLine(bool bShow, COLORREF clr=0x8FFF8F);//顯示視窗十字線
	//---------------------------------------------------------------------------------//	
	bool                       GetShowEditCenterLine() const;//設定編輯的中心線
	void                       SetShowEditCenterLine(bool bShow, COLORREF clr=0x000080);//設定編輯的中心線
	//---------------------------------------------------------------------------------//	
	bool                       GetShowImageCenterLine() const;//顯示影像十字線
	void                       SetShowImageCenterLine(bool bShow, COLORREF clr=0x8F8FFF);//顯示影像十字線
	//---------------------------------------------------------------------------------//		
	bool                       GetShowStagePos() const;//顯示機台座標
	void                       SetShowStagePos(bool bShow);//顯示機台座標
	//---------------------------------------------------------------------------------//		
	bool                       GetShowMoveToMsg() const;//顯示移動至訊息
	void                       SetShowMoveToMsg(bool bShow);//顯示移動至訊息
	//---------------------------------------------------------------------------------//
	void                       SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_MODE Mode);//顯示專案模式	
	//---------------------------------------------------------------------------------//
	void                       CalcImageShowRect(RECT &Rect);//取得影像顯示區域-INT
	void                       CalcImageShowRect(TRECT4D &Rect);//取得影像顯示區域-DBL
	//---------------------------------------------------------------------------------//
	void                       SetImageEditExtendSize(POINT &ExtendSize);//設定影像編輯外擴
	void                       GetImageEditRect(RECT &Rect);//取得影像編輯區域-INT
	void                       SetImageEditRect(const RECT &Rect);//設定影像編輯區域-INT
	void                       GetImageEditRect(TRECT4D &Rect);//取得影像編輯區域-DBL
	void                       SetImageEditRect(const TRECT4D &Rect);//設定影像編輯區域-DBL
	//---------------------------------------------------------------------------------//
	void                       ShowFittedZoom(bool bRedraw=true);//顯示合適的縮放比
	void                       MoveViewOffset(double nX, double nY);//移動顯示偏移量
	//---------------------------------------------------------------------------------//
	TPOINT2D                   CalcStagePos(const POINT &Pt);//計算機台座標
	TREGION4D                  CalcStageRegion(const TREGION4D &Region);//計算機台座標
	TPOINT2D                   GetStagePosAtWndCenterPos();//取得機台座標-視窗中心
	bool                       MoveViewToStageRgn(const TREGION4D &Rgn, bool bForceMove);//將畫面移至機台位置
	bool                       MoveViewToStagePos(double PosX, double PosY, bool bForceMove);//將畫面移至機台位置
	//---------------------------------------------------------------------------------//
	void                       UpdateProjectMap(bool bTestMap=false);//更新專案底圖
	void                       UpdateProjectTestMap();//更新專案檢測底圖	
	bool                       CheckShowProjectTestMpa() const;//確認顯示專案檢測底圖
	void                       ResetUpdateTestMapTickCount();//清除更新專案檢測底圖時間點
	//---------------------------------------------------------------------------------//
	bool                       LoadImageFile(LPCTSTR pfilename, bool bEnhance);//載入圖檔	
	//---------------------------------------------------------------------------------//
	void                       ReleaseImageBuffer();		
	void                       SetImageText(LPCTSTR str, bool bShow);//是否顯示影像文字
	bool                       BuildSystemRegion();//建立系統區域的影像
	bool                       SetImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *Ptr, bool Clone, bool ResetView, bool Redraw=true);
	bool                       SetImageRawBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ShowStep, IMAGE_SIZE ShowBit, unsigned char *ShowPtr, unsigned char *RawPtr, bool Clone, bool ResetView, bool Redraw=true);
	bool                       SetImageRawBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ShowStep, IMAGE_SIZE ShowBit, unsigned char *ShowPtr, IMAGE_SIZE RawStep, IMAGE_SIZE RawBit, unsigned char *RawPtr, bool Clone, bool ResetView, bool Redraw=true);
	//---------------------------------------------------------------------------------//
	POINT&                     GetLBtnUpPos() { return m_LBtnUpPos; }
	POINT&                     GetLBtnDownPos() { return m_LBtnDownPos; }
	POINT&                     GetRBtnUpPos() { return m_RBtnUpPos; }
	POINT&                     GetRBtnDownPos() { return m_RBtnDownPos; }
	POINT&                     GetMouseMovingPos() { return m_MovingPos; }	
	//---------------------------------------------------------------------------------//	
	double                     m_ImageEditRectAngle;
	void                       SetImageEditRectAngle(double angle) { m_ImageEditRectAngle = angle; }
	double                     GetImageEditRectAngle() { return m_ImageEditRectAngle; }
	//---------------------------------------------------------------------------------//	
	void                       SetTempObjList(std::vector<TBOX_DRAW_PARAM> ObjList) { m_TempObjList = ObjList; }
	//---------------------------------------------------------------------------------//	
	double                     GetImageZoom() { return m_ImageZoom; }
	TPOINT2D                   GetImageOffset() { return m_ImageOffset; }
	//---------------------------------------------------------------------------------//	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageWnd)
	public:
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageWnd();

protected:
	//---------------------------------------------------------------------------------//		
	LANE_ID                    m_ShowLaneID;
	CAMERA_ID                  m_CameraID;
	CAOIProject               *m_ProjectPtr;
	unsigned int               m_ProjectMapIndex;
	POINT                      m_ExtendSize;
	TPOINT2D                   m_ImageResolution;//影像解析度
	TREGION4D                  m_ImageStageRgn;//影像在機台的邊界
	//---------------------------------------------------------------------------------//	
	IMAGE_DATA_SRC             m_ImageDataSrc;//資料來源
	IMAGE_RBTN_UP_MODE         m_RBtnUpMode;//右鍵放開功能模式
	IMAGE_LBTN_UP_MODE         m_LBtnUpMode;//左鍵放開功能模式
	IMAGE_RBTN_CLICK_MODE      m_RBtnClickMode;//右鍵功能模式
	IMAGE_LBTN_CLICK_MODE      m_LBtnClickMode;//左鍵功能模式	
	IMAGE_RBTN_DBCLICK_MODE    m_RBtnDbClickMode;//右鍵雙擊模式	
	IMAGE_LBTN_DBCLICK_MODE    m_LBtnDbClickMode;//左鍵雙擊模式		
	//---------------------------------------------------------------------------------//
	CString                    m_strCursorInfo;
	CString                    m_strImageText;
	bool                       m_ShowLBtnPos;		
	bool                       m_ShowEditLine;
	bool                       m_ShowEditPosText;
	bool                       m_ShowCursorInfo;
	bool                       m_ShowFiducial;
	bool                       m_ShowPanel;
	bool                       m_ShowBoard;
	bool                       m_ShowMark;
	bool                       m_ShowSystem;
	bool                       m_ShowBarcode;
	bool                       m_ShowComponent;
	bool                       m_ShowDistrictRect;
	bool                       m_ShowComponentName;	
	bool                       m_ShowComponentDefectOnly;
	bool                       m_ShowCameraPos;//顯示相機中心位置	
	bool                       m_ShowImageText;//顯示畫面文字
	bool                       m_ShowMoveToMsg;//顯示移動至訊息
	//---------------------------------------------------------------------------------//		
	bool                       m_ShowCameraRgn;//顯示相機位置
	COLORREF                   m_clrCameraRgn;
	//---------------------------------------------------------------------------------//		
	bool                       m_ShowFieldRgn;//顯示區域位置
	COLORREF                   m_clrFieldRgn;
	//---------------------------------------------------------------------------------//		
	bool                       m_ShowWndCenterLine;//顯示視窗中心線
	COLORREF                   m_clrWndCenterLine;
	//---------------------------------------------------------------------------------//		
	bool                       m_ShowImgCenterLine;//顯示圖像中心線
	COLORREF                   m_clrImgCenterLine;
	//---------------------------------------------------------------------------------//		
	bool                       m_ShowCursorLine;//顯示鼠標線
	COLORREF                   m_clrCursorLine;
	//---------------------------------------------------------------------------------//		
	bool                       m_ShowEditGridLine;//顯示編輯線
	COLORREF                   m_clrEditGridLine;
	//---------------------------------------------------------------------------------//		
	bool                       m_ShowEditCenterLine;//顯示區域的中心線
	COLORREF                   m_clrEditCenterLine;
	//---------------------------------------------------------------------------------//		
	bool                       m_ShowMeasureLine;//顯示量測線
	COLORREF                   m_clrMeasureLine;
	//---------------------------------------------------------------------------------//			
	int                        m_EditGridW;
	int                        m_EditGridH;
	std::vector<TEditGrid>     m_EditGridList;
	//---------------------------------------------------------------------------------//		
	IMAGE_WND_DRAW_PROJECT_MODE m_DrawProjectMode;
	size_t                     m_FieldFinishCount;
	size_t                     m_ComponentFinishCount;		
	DWORD                      m_UpdateTestMapTickCount;//更新檢測底圖時間點
	//---------------------------------------------------------------------------------//
	int                        m_HatchType;//背景紋理
	COLORREF                   m_HatchColor;//紋理顏色
	COLORREF                   m_BkColor;
	CJetMemDC                  m_MemDC1;
	CJetMemDC                  m_MemDC2;
	CJetMemDC                  m_MemDC3;
	RECT                       m_WndRect;	
	double                     m_ImageZoom;		
	TPOINT2D                   m_ImageOffset;
	HFONT                      m_InspectionFont;	
	//---------------------------------------------------------------------------------//			
	POINT                      m_MovingPos;	
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;	
	TPOINT2D                   m_WndLBtnPt1;
	TPOINT2D                   m_WndLBtnPt2;	
	//---------------------------------------------------------------------------------//				
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	TPOINT2D                   m_WndRBtnPt1;
	TPOINT2D                   m_WndRBtnPt2;	
	//---------------------------------------------------------------------------------//					
	BOOL                       GetIsPressVRKey(int VK);
	BOOL                       CalcImageWndZoom(double OldZoom, double NewZoom, TPOINT2D &OffsetPt);
	BOOL                       CalcImageWndFitZoom(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const double Ratio, double &Zoom);
	BOOL                       MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt);
	BOOL                       MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt);
	//---------------------------------------------------------------------------------//			
	LPTSTR                     m_CursorID;
	EDIT_GRID_MODE             m_EditRectMode;
	TRECT4D                    m_ImageEditRect;
	TPOINT2D                   m_ImageEditPt1;
	TPOINT2D                   m_ImageEditPt2;	
	TPOINT2D                   m_ImageEditLast;	
	POINT                      m_ImageWndEditPt1;
	POINT                      m_ImageWndEditPt2;
	RECT                       m_ImageWndEditRect;
	bool                       m_ImageEditRectRotated;
	void                       ExecEditRect();
	void                       ExecEditRectExceptionAngle();
	void                       UpdateCursor(const POINT &pt);
	void                       UpdateShowEditLine();
	void                       UpdateShowMeasureLine();
	//---------------------------------------------------------------------------------//	
	bool                       m_Cloned;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_SIZE                 m_BitCountRaw;
	IMAGE_SIZE                 m_ImageStepRaw;
	unsigned char *            m_ImagePtr;
	unsigned char *            m_ImagePtrRaw;
	//---------------------------------------------------------------------------------//		
	std::vector<TBOX_DRAW_PARAM> m_TempObjList;
	//---------------------------------------------------------------------------------//		
	void                       SwitchProjectMap();//切換專案底圖	
	//---------------------------------------------------------------------------------//
	void                       DrawImage();	
	void                       CalcImageWndLBtn();
	void                       CalcImageWndLBtn_ExpectAngle();
	void                       ShowCursorInfo(POINT point);	
	void                       DrawPie(HDC hDC, const RECT &Rect);
	void                       DrawRect(HDC hDC, const RECT &Rect);	
	void                       DrawRectLine(HDC hDC, const POINT pt[], size_t num);
	bool                       DrawRectRoughLine(HDC hDC, const RECT &Rect, COLORREF color, int Gap=4);
	void                       DrawCircleLine(HDC hDC, const RECT &Rect);
	void                       DrawSystem(HDC hDC);//繪製系統
	void                       DrawMesStats(HDC hDC);//繪製MES
	void                       DrawProject(HDC hDC);//繪製專案	
	bool                       DrawProjectDistrictRect(HDC hDC);//繪製專案段落	
	void                       DrawProjectNormal(HDC hDC);//繪製專案-一般模式
	void                       DrawProjectInspecting(HDC hDC);//繪製專案-檢測模式	
	void                       DrawProjectFdList(HDC hDC);//繪製專案定位點
	void                       DrawProjectFieldList(HDC hDC, bool ShowRect);//繪製專案區域
	void                       DrawProjectPanelList(HDC hDC, bool ShowName);//繪製專案整板
	void                       DrawProjectBoardList(HDC hDC, bool ShowName);//繪製專案單板
	void                       DrawProjectMarkList(HDC hDC, bool ShowName);//繪製專案特徵點
	void                       DrawProjectBarcodeList(HDC hDC, bool ShowName);//繪製專案條碼
	void                       DrawProjectComponentList(HDC hDC, bool ShowRect, bool ShowName);//繪製專案零件	
	void                       DrawProjectTempObjList(HDC hDC);//繪製暫存物件
	void                       DrawCameraPos(HDC hDC);//顯示相機中心位置
	void                       DrawCameraRgn(HDC hDC);//繪製相機位置
	void                       DrawImageCenterLine(HDC hDC);//繪製影像中心線		
	//---------------------------------------------------------------------------------//
	void                       ExecMovePanel(const POINT &pt);//移動整板	
	void                       ExecMoveProject(const POINT &pt);//移動專案	
	void                       ExecMovePanelStage(const POINT &pt);//移動整板機台座標
	void                       ExecMoveProjectStage(const POINT &pt);//移動專案
	void                       ExecStageMoveTo(const POINT &Pt);//移動機台至	
	void                       ExecSelectComponent(const TRECT4D &Rect);//選取零件
	bool                       CheckOutOfImageRgn();//確認是否超過影像範圍
	//---------------------------------------------------------------------------------//
	BOOL                       SendParentMessage(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL                       PostParentMessage(UINT message, WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//	
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//	
	// Generated message map functions
protected:
	//{{AFX_MSG(CImageWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMoveTo();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);		
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEWND_H__53B79DCF_238E_4927_9F3D_B3E39EC2B9F6__INCLUDED_)

