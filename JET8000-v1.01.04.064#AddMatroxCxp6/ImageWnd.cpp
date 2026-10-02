// ImageWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "ImageWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageWnd
//-------------------------------------------------------------------------------------//
typedef struct tagLaneRgn
{
	LANE_ID       eLaneID;
	TPOINT2D      sLanePos1;
	TPOINT2D      sLanePos2;
} TLaneRgn, *PLaneRgn;
//-------------------------------------------------------------------------------------//
CImageWnd::CImageWnd()
{
	m_ShowLaneID = LANE_ID_BOTH;
	m_ProjectPtr = NULL;
	m_ProjectMapIndex = 0;

	m_CursorID = NULL;	
	m_ShowLBtnPos = false;	
	m_ShowEditLine = false;
	m_ShowEditPosText = true;
	m_ShowCursorInfo = false;
	m_ShowFiducial = true;
	m_ShowPanel = false;
	m_ShowBoard = false;
	m_ShowMark = true;
	m_ShowSystem = false;
	m_ShowBarcode = true;
	m_ShowComponent = true;
	m_ShowDistrictRect = false;
	m_ShowComponentName = true;	
	m_ShowComponentDefectOnly = false;
	m_EditRectMode = EDIT_GRID_NULL;

	m_ImageDataSrc = IMAGE_DATA_FOV;
	m_RBtnUpMode = IMAGE_RBTN_UP_NULL;
	m_LBtnUpMode = IMAGE_LBTN_UP_NULL;
	m_RBtnClickMode = IMAGE_RBTN_CLICK_CTRL_VIEW;
	m_LBtnClickMode = IMAGE_LBTN_CLICK_NULL;
	m_RBtnDbClickMode = IMAGE_RBTN_DBCLICK_FIT_ZOOM;
	m_LBtnDbClickMode = IMAGE_LBTN_DBCLICK_STAGE_MOVE_TO;	
	
	m_ShowImageText = false;	
	m_ShowMoveToMsg = false;
	m_ShowCameraPos = false;//顯示相機中心位置
	m_ShowCameraRgn = false;//顯示相機位置
	m_clrCameraRgn = 0xFF8080;

	m_ShowFieldRgn = false;
	m_clrFieldRgn = 0xFF8080;
	
	m_ShowWndCenterLine = false;
	m_clrWndCenterLine = 0x8FFF8F;

	m_ShowImgCenterLine = false;//顯示圖像中心線
	m_clrImgCenterLine = 0x8F8FFF;	

	m_ShowCursorLine = true;
	m_clrCursorLine = 0x20A0A0;

	m_ShowEditGridLine = true;
	m_clrEditGridLine = 0xFFFF00;
	m_clrEditGridLine = 0x0000FF;

	m_ShowMeasureLine = false;
	m_clrMeasureLine = 0x0000FF;

	m_ShowEditCenterLine = false;//顯示區域的中心線
	m_clrEditCenterLine = 0x000080;
	
	m_HatchType = HS_API_MAX;
	m_HatchColor = 0xFFFFFF;
	m_BkColor = 0x000000;

	m_Cloned = false;	
	m_ImageW = 1024;
	m_ImageH = 1024;
	m_ImageStep = 0;
	m_BitCount = 0;	
	m_ImagePtr = NULL;
	m_BitCountRaw = 0;
	m_ImageStepRaw = 0;
	m_ImagePtrRaw = NULL;
	m_CameraID = PRIMARY_CAMERA_ID;
	m_FieldFinishCount = 0;
	m_ComponentFinishCount = 0;
	m_UpdateTestMapTickCount = 0;

	m_DrawProjectMode = IMAGE_WND_DRAW_PROJECT_NORMAL;
	m_ExtendSize.x = m_ExtendSize.y = 0;
	m_ImageEditRectAngle = 0;
	m_ImageEditRectRotated = false;

	this->m_EditGridW = 8;
	this->m_EditGridH = 8;
	this->m_ImageZoom = 1;		
	this->m_MovingPos.x = this->m_MovingPos.y = 0;
	this->m_RBtnUpPos.x = this->m_RBtnUpPos.y = -1;
	this->m_RBtnDownPos.x = this->m_RBtnDownPos.y = -1;
	this->m_LBtnUpPos.x = this->m_LBtnUpPos.y = -1;
	this->m_LBtnDownPos.x = this->m_LBtnDownPos.y = -1;	
	
	m_InspectionFont = NULL;


	UpdateShowEditLine();
}
//-------------------------------------------------------------------------------------//
CImageWnd::~CImageWnd()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CImageWnd, CStatic)
	//{{AFX_MSG_MAP(CImageWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()	
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(MENU_IMAGE_MOVE_TO, OnMoveTo)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageWnd message handlers
//-------------------------------------------------------------------------------------//
void CImageWnd::ResetLBtnPos()//復歸滑鼠左鍵
{
	m_LBtnUpPos.x = -1;
	m_LBtnUpPos.y = -1;
	m_LBtnDownPos.x = -1;	
	m_LBtnDownPos.y = -1;	
	m_WndLBtnPt1.x = -1;
	m_WndLBtnPt1.y = -1;
	m_WndLBtnPt2.x = -1;	
	m_WndLBtnPt2.y = -1;	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ResetRBtnPos()//復歸滑鼠右鍵
{
	m_RBtnUpPos.x = -1;
	m_RBtnUpPos.y = -1;
	m_RBtnDownPos.x = -1;	
	m_RBtnDownPos.y = -1;	
	m_WndRBtnPt1.x = -1;
	m_WndRBtnPt1.y = -1;
	m_WndRBtnPt2.x = -1;	
	m_WndRBtnPt2.y = -1;	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowAll(bool bShow)//顯示全部
{	
	SetShowFiducial(bShow);
	SetShowPanel(bShow);
	SetShowBoard(bShow);
	SetShowBarcode(bShow);
	SetShowComponent(bShow);
	SetShowCameraRgn(bShow);
	SetShowCursorInfo(bShow);
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowLBtnPos() const//啟用滑鼠左鍵		
{
	return m_ShowLBtnPos;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowLBtnPos(bool value)//啟用滑鼠左鍵
{
	m_ShowLBtnPos = value;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowEditLine() const//顯示編輯線段
{
	return m_ShowEditLine;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowEditLine(bool value)//顯示編輯線段
{
	m_ShowEditLine = value;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowEditPosText() const//顯示編輯位置文字
{
	return m_ShowEditPosText;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowEditPosText(bool value)//顯示編輯位置文字
{
	m_ShowEditPosText = value;
}
//-------------------------------------------------------------------------------------//
IMAGE_RBTN_UP_MODE CImageWnd::GetRBtnUpMode() const
{
	return m_RBtnUpMode;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetRBtnUpMode(IMAGE_RBTN_UP_MODE value)//右鍵放開功能
{
	m_RBtnUpMode = value;
}
//-------------------------------------------------------------------------------------//
IMAGE_LBTN_UP_MODE CImageWnd::GetLBtnUpMode() const
{
	return m_LBtnUpMode;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetLBtnUpMode(IMAGE_LBTN_UP_MODE value)//左鍵放開功能
{
	m_LBtnUpMode = value;
}
//-------------------------------------------------------------------------------------//
IMAGE_RBTN_CLICK_MODE CImageWnd::GetRBtnClickMode() const//取得右鍵控制模式
{
	return m_RBtnClickMode;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetRBtnClickMode(IMAGE_RBTN_CLICK_MODE value)//設定右鍵控制模式
{
	m_RBtnClickMode = value;
}
//-------------------------------------------------------------------------------------//	
IMAGE_LBTN_CLICK_MODE  CImageWnd::GetLBtnClickMode() const//取得左鍵控制模式	
{
	return m_LBtnClickMode;
}
//-------------------------------------------------------------------------------------//	
void CImageWnd::SetLBtnClickMode(IMAGE_LBTN_CLICK_MODE value)//設定左鍵控制模式
{
	m_LBtnClickMode = value;
	UpdateShowEditLine();
	UpdateShowMeasureLine();
}
//-------------------------------------------------------------------------------------//
IMAGE_RBTN_DBCLICK_MODE CImageWnd::GetRBtnDbClickMode() const//取得右鍵雙擊模式	
{
	return m_RBtnDbClickMode;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetRBtnDbClickMode(IMAGE_RBTN_DBCLICK_MODE value)//設定右鍵雙擊模式	
{
	m_RBtnDbClickMode = value;
}
//-------------------------------------------------------------------------------------//
IMAGE_LBTN_DBCLICK_MODE CImageWnd::GetLBtnDbClickMode() const//取得左鍵雙擊模式	
{
	return m_LBtnDbClickMode;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetLBtnDbClickMode(IMAGE_LBTN_DBCLICK_MODE value)//設定左鍵雙擊模式	
{
	m_LBtnDbClickMode = value;
}
//-------------------------------------------------------------------------------------//
bool  CImageWnd::GetShowCursorInfo() const//顯示滑鼠訊息
{
	return m_ShowCursorInfo;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowCursorInfo(bool value)
{
	m_ShowCursorInfo = value;	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_MODE Mode)//顯示專案模式
{
	m_DrawProjectMode = Mode;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowPanel() const
{
	return m_ShowPanel;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowPanel(bool bShow)//顯示專案整板
{
	m_ShowPanel = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowBoard() const
{
	return m_ShowBoard;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowBoard(bool bShow)//顯示專案單板	
{
	m_ShowBoard = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowFiducial() const
{
	return m_ShowFiducial;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowFiducial(bool bShow)//顯示專案定位點
{
	m_ShowFiducial = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowMark() const
{
	return m_ShowMark;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowMark(bool bShow)//顯示專案特徵點
{
	m_ShowMark = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowSystem() const//顯示系統
{
	return m_ShowSystem;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowSystem(bool bShow)//顯示系統
{
	m_ShowSystem = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowBarcode() const//顯示專案條碼
{
	return m_ShowBarcode;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowBarcode(bool bShow)//顯示專案條碼
{
	m_ShowBarcode = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowComponent() const//顯示專案零件
{
	return m_ShowComponent;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowComponent(bool bShow)//顯示專案零件
{
	m_ShowComponent = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowDistrictRect() const//顯示分段區間
{
	return m_ShowDistrictRect;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowDistrictRect(bool bShow)//顯示分段區間
{
	m_ShowDistrictRect = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowComponentName() const//顯示專案零件名稱	
{
	return m_ShowComponentName;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowComponentName(bool bShow)//顯示專案零件名稱
{
	m_ShowComponentName = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowComponentDefectOnly() const//顯示專案瑕疵零件
{
	return m_ShowComponentDefectOnly;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowComponentDefectOnly(bool bShow)//顯示專案瑕疵零件
{
	m_ShowComponentDefectOnly = bShow;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowFieldRgn() const//設定區域框線
{
	return m_ShowFieldRgn;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowFieldRgn(bool bShow, COLORREF clr)//設定是否顯示區域框線
{
	m_ShowFieldRgn = bShow;
	m_clrFieldRgn = clr;	
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowCameraPos() const//顯示相機中心位置	
{
	return m_ShowCameraPos;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowCameraRgn() const//顯示相機位置	
{
	return m_ShowCameraRgn;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowCameraPos(bool bShow, COLORREF clr)//顯示相機中心位置	
{
	m_ShowCameraPos = bShow;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowCameraRgn(bool bShow, COLORREF clr)//顯示相機位置
{
	m_ShowCameraRgn = bShow;
	m_clrCameraRgn = clr;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowCursorLine() const//顯示鼠標線	
{
	return m_ShowCursorLine;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowCursorLine(bool bShow, COLORREF clr)//顯示鼠標線
{
	m_ShowCursorLine = bShow;
	m_clrCursorLine = clr;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowWndCenterLine() const//顯示視窗十字線
{
	return m_ShowWndCenterLine;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowWndCenterLine(bool bShow, COLORREF clr)//顯示十字線
{
	m_ShowWndCenterLine = bShow;
	m_clrWndCenterLine = clr;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowEditCenterLine() const//設定編輯的中心線
{
	return m_ShowEditCenterLine;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowEditCenterLine(bool bShow, COLORREF clr)//設定編輯的中心線
{
	m_ShowEditCenterLine = bShow;//顯示區域的中心線
	m_clrEditCenterLine = clr;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowImageCenterLine() const//顯示影像十字線
{
	return m_ShowImgCenterLine;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowImageCenterLine(bool bShow, COLORREF clr)//顯示影像十字線
{
	m_ShowImgCenterLine = bShow;
	m_clrImgCenterLine = clr;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetShowMoveToMsg() const//顯示移動至訊息
{
	return m_ShowMoveToMsg;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowMoveToMsg(bool bShow)//顯示移動至訊息
{
	m_ShowMoveToMsg = bShow;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetImageEditExtendSize(POINT &ExtendSize)//設定影像編輯外擴
{
	this->m_ExtendSize = ExtendSize;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::CalcImageShowRect(RECT &Rect)//取得影像顯示區域-INT
{	
	TPOINT2D WndPtLT, WndPtRB;
	TPOINT2D ImagePtLT, ImagePtRB;
	WndPtLT.x = m_WndRect.left;
	WndPtLT.y = m_WndRect.top;
	WndPtRB.x = m_WndRect.right;
	WndPtRB.y = m_WndRect.bottom;
	CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPtLT, ImagePtLT);
	CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPtRB, ImagePtRB);
	Rect.left = JetAPI::Floor(ImagePtLT.x);
	Rect.top = JetAPI::Floor(ImagePtLT.y);
	Rect.right = JetAPI::Floor(ImagePtRB.x);
	Rect.bottom = JetAPI::Floor(ImagePtRB.y);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::CalcImageShowRect(TRECT4D &Rect)//取得影像顯示區域-DBL
{
	TPOINT2D WndPtLT, WndPtRB;
	TPOINT2D ImagePtLT, ImagePtRB;
	WndPtLT.x = m_WndRect.left;
	WndPtLT.y = m_WndRect.top;
	WndPtRB.x = m_WndRect.right;
	WndPtRB.y = m_WndRect.bottom;
	MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPtLT, ImagePtLT);
	MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPtRB, ImagePtRB);
	Rect.left = ImagePtLT.x;
	Rect.top = ImagePtLT.y;
	Rect.right = ImagePtRB.x;
	Rect.bottom = ImagePtRB.y;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::GetImageEditRect(RECT &Rect)//取得影像編輯區域-INT
{
	Rect.left   = JetAPI::Floor(m_ImageEditRect.left);
	Rect.top    = JetAPI::Floor(m_ImageEditRect.top);
	Rect.right  = JetAPI::Floor(m_ImageEditRect.right);
	Rect.bottom = JetAPI::Floor(m_ImageEditRect.bottom);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetImageEditRect(const RECT &Rect)//設定影像編輯區域-INT
{
	m_ImageEditRect = Rect;
	m_ImageEditPt1.x = Rect.left;
	m_ImageEditPt1.y = Rect.top;
	m_ImageEditPt2.x = Rect.right;
	m_ImageEditPt2.y = Rect.bottom;
	CalcImageWndLBtn();
}
//-------------------------------------------------------------------------------------//
void CImageWnd::GetImageEditRect(TRECT4D &Rect)//取得影像編輯區域-DBL
{
	Rect = m_ImageEditRect;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetImageEditRect(const TRECT4D &Rect)//設定影像編輯區域-DBL
{
	m_ImageEditRect = Rect;
	m_ImageEditPt1.x = Rect.left;
	m_ImageEditPt1.y = Rect.top;
	m_ImageEditPt2.x = Rect.right;
	m_ImageEditPt2.y = Rect.bottom;
	CalcImageWndLBtn();
	return;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ShowFittedZoom(bool bRedraw)//顯示合適的縮放比
{
	const IMAGE_SIZE ImageW = this->m_ImageW;
	const IMAGE_SIZE ImageH = this->m_ImageH;
	this->m_ImageOffset.x = this->m_ImageOffset.y = 0;
	CalcImageWndFitZoom(ImageW, ImageH, this->m_WndRect, 1.1, this->m_ImageZoom);
	DrawImage();
	if ( true == bRedraw )
	{	RedrawWnd(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CImageWnd::MoveViewOffset(double nX, double nY)//移動顯示偏移量
{
	m_ImageOffset.x += nX;
	m_ImageOffset.y += nY;
	CalcImageWndLBtn();
	DrawImage();
}
//-------------------------------------------------------------------------------------//
TPOINT2D CImageWnd::GetStagePosAtWndCenterPos()//取得機台座標-視窗中心
{
	POINT WndCp;
	WndCp.x = (m_WndRect.left+m_WndRect.right)/2;
	WndCp.y = (m_WndRect.top+m_WndRect.bottom)/2;
	return CalcStagePos(WndCp);	
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::MoveViewToStageRgn(const TREGION4D &Rgn, bool bForceMove)//將畫面移至機台位置
{
	if ( IMAGE_DATA_MAP != m_ImageDataSrc )
	{	return false; }
			
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt1={0}, Pt2={0};	
	POINT        WndCp={0};
	RECT         Rect={0,0,0,0};
	TPOINT2D     WndPt1, WndPt2;	
	TPOINT2D     ImagePt1, ImagePt2;
	TREGION4D    ImageRgn;
	TREGION4D    CameraRgn;
	TPOINT2D     ImageStagePos;
	TPOINT2D     ImageRes = this->m_ImageResolution;
	
	JetAPI::GetRectCenterPos(m_WndRect, WndCp);
	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();
	CameraRgn = Rgn;	
	AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, CameraRgn, ImageStagePos, ImageRgn);
	ImagePt1.x = ImageRgn.minX;	ImagePt1.y = ImageRgn.minY;
	ImagePt2.x = ImageRgn.maxX;	ImagePt2.y = ImageRgn.maxY;
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt1, WndPt1);	
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt2, WndPt2);	

	JetAPI::Point2DToPoint(WndPt1, Pt1);
	JetAPI::Point2DToPoint(WndPt2, Pt2);
	if ( true==bForceMove || ::PtInRect(&m_WndRect, Pt1)==FALSE || ::PtInRect(&m_WndRect, Pt2)==FALSE )
	{
		POINT Pt;
		Pt.x = (Pt1.x+Pt2.x)/2;
		Pt.y = (Pt1.y+Pt2.y)/2;
		m_ImageOffset.x = (WndCp.x-Pt.x)+m_ImageOffset.x;
		m_ImageOffset.y = (WndCp.y-Pt.y)+m_ImageOffset.y;
		RedrawWnd(TRUE);
	}
	else
	{	RedrawWnd(FALSE); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::MoveViewToStagePos(double PosX, double PosY, bool bForceMove)//將畫面移至機台位置
{
	if ( IMAGE_DATA_MAP != m_ImageDataSrc )
	{	return false; }
			
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt={0};	
	POINT        WndCp={0};
	RECT         Rect={0,0,0,0};
	TPOINT2D     WndPt;	
	TPOINT2D     ImagePt;
	TPOINT2D     CameraPt;
	TPOINT2D     ImageStagePos;
	TPOINT2D     ImageRes = this->m_ImageResolution;

	CameraPt.x = PosX;
	CameraPt.y = PosY;
	JetAPI::GetRectCenterPos(m_WndRect, WndCp);
	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();		
	AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, CameraPt, ImageStagePos, ImagePt);
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt, WndPt);	

	JetAPI::Point2DToPoint(WndPt, Pt);
	if ( true==bForceMove || ::PtInRect(&m_WndRect, Pt)==FALSE )
	{
		m_ImageOffset.x = (WndCp.x-Pt.x)+m_ImageOffset.x;
		m_ImageOffset.y = (WndCp.y-Pt.y)+m_ImageOffset.y;
		RedrawWnd(TRUE);
	}
	else
	{	RedrawWnd(FALSE); }
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::UpdateProjectMap(bool bTestMap)//更新專案底圖
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	
	TPOINT2D   ImageRes;	
	IMAGE_PTR  ImagePtr=NULL;			
	TREGION4D  RgnCad, RgnStage;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	unsigned int NextMapIndex = m_ProjectMapIndex;
	ProjectPtr->GetProjectMapPtr(NextMapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	ProjectPtr->CreateProjectMapShowPtr(NextMapIndex, ImagePtr, true, bTestMap);		
	m_strImageText = ProjectPtr->GetProjectMapIndexName();
	if ( IMAGE_DATA_MAP == m_ImageDataSrc )
	{	SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, false, true); }	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::UpdateProjectTestMap()//更新專案檢測底圖
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	DWORD TestMapTickCount=ProjectPtr->GetProjectTestMapTickCount(0);
	if ( TestMapTickCount < m_UpdateTestMapTickCount )
	{	return; }
	m_UpdateTestMapTickCount=GetTickCount();
	UpdateProjectMap(true);
	m_UpdateTestMapTickCount=GetTickCount();
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::CheckShowProjectTestMpa() const//確認顯示專案檢測底圖
{
	if ( 0 == m_UpdateTestMapTickCount ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ResetUpdateTestMapTickCount()//清除更新專案檢測底圖時間點
{
	m_UpdateTestMapTickCount = 0;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::LoadImageFile(LPCTSTR pfilename, bool bEnhance)//載入圖檔
{
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount  = 0;
	IMAGE_PTR  ImagePtr = NULL;
	ReleaseImageBuffer();	
	if ( ImageAPI.LoadImage(pfilename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
	{	
		RedrawWnd(TRUE);
		return false; 
	}
	if ( true == bEnhance )
	{
		if ( AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImagePtr) == false )
		{
			JetMemory.free_func(ImagePtr);			
			RedrawWnd(TRUE);
			return false;
		}
	}
	SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, true, false);
	m_Cloned = true;
	ShowFittedZoom();
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetImageText(LPCTSTR str, bool bShow)//是否顯示影像文字
{
	m_strImageText = str;
	m_ShowImageText = bShow;
	return;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::BuildSystemRegion()//建立系統區域的影像
{
#ifndef OFFLINE_VERSION
	TPOINT2D  SystemRes;
	TREGION4D SystemRgn;	
	IMAGE_DATA_SRC SrcMode=IMAGE_DATA_MAP;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();

	SystemRes.x = 100;
	SystemRes.y = 100;
	SystemRgn.minX = MotionParam.m_LimitMinX;
	SystemRgn.minY = MotionParam.m_LimitMinY;
	SystemRgn.maxX = MotionParam.m_LimitMaxX;
	SystemRgn.maxY = MotionParam.m_LimitMaxY;

	double       ImageZoom = 1.0;
	const RECT  &WndRect = m_WndRect;
	const double RgnSizeX=SystemRgn.GetSizeX();
	const double RgnSizeY=SystemRgn.GetSizeY();
	IMAGE_SIZE   ImageW=(int)((RgnSizeX/SystemRes.x)+0.5);
	IMAGE_SIZE   ImageH=(int)((RgnSizeY/SystemRes.y)+0.5);	
	ReleaseImageBuffer();	
	CalcImageWndFitZoom(ImageW, ImageH, WndRect, 1.1, ImageZoom);

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageZoom = ImageZoom;
	SetImageInfo(CameraID, SystemRgn, SystemRes, SrcMode);
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::SetImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *Ptr, bool Clone, bool ResetView, bool Redraw)
{
	double ImageZoomBefor = m_ImageZoom;
	ReleaseImageBuffer();
	if ( true == Clone )
	{
		const char fnName[] = "CImageWnd::SetImageBuffer";
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, m_ImagePtr, fnName, "m_ImagePtr") == false )
		{	return FALSE; }
		::memcpy(m_ImagePtr, Ptr, sizeof(unsigned char)*BufferSize);		
	}
	else
	{	m_ImagePtr = Ptr;	}

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_BitCount = BitCount;
	m_Cloned = Clone;
	m_ImageEditPt1.x = m_ImageEditPt1.y = -1;
	m_ImageEditPt2.x = m_ImageEditPt2.y = -1;
	m_ImageEditRect.left = m_ImageEditRect.right = -1;	
	m_ImageEditRect.top  = m_ImageEditRect.bottom = -1;	
	if ( true == ResetView )
	{	m_ImageOffset.x = m_ImageOffset.y = 0; }
	else
	{	m_ImageZoom = ImageZoomBefor; }
	CalcImageWndLBtn();
	if ( true == Redraw )
	{
		DrawImage();
		RedrawWnd(FALSE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::SetImageRawBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ShowStep, IMAGE_SIZE ShowBit, unsigned char *ShowPtr, unsigned char *RawPtr, bool Clone, bool ResetView, bool Redraw)
{
	bool IsOK = true;
	IsOK = SetImageRawBuffer(ImageW, ImageH, ShowStep, ShowBit, ShowPtr, ShowStep, ShowBit, RawPtr, Clone, ResetView, Redraw);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::SetImageRawBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ShowStep, IMAGE_SIZE ShowBit, unsigned char *ShowPtr, IMAGE_SIZE RawStep, IMAGE_SIZE RawBit, unsigned char *RawPtr, bool Clone, bool ResetView, bool Redraw)
{
	double ImageZoomBefor = m_ImageZoom;
	ReleaseImageBuffer();
	if ( true == Clone )
	{
		const char fnName[] = "CImageWnd::SetImageRawBuffer";
		const size_t RawBufferSize = ImageAPI.CalcBufferSize(RawStep, ImageH);
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(ShowStep, ImageH);
		if ( JetMemory.alloc_func(ShowBufferSize, m_ImagePtr, fnName, "m_ImagePtr") == false ||
			 JetMemory.alloc_func(RawBufferSize, m_ImagePtrRaw, fnName, "m_ImagePtrRaw") == false )
		{	
			JetMemory.free_func(m_ImagePtr);
			JetMemory.free_func(m_ImagePtrRaw);
			return false; 
		}		
		::memcpy(m_ImagePtr, ShowPtr, sizeof(unsigned char)*ShowBufferSize);		
		::memcpy(m_ImagePtrRaw, RawPtr, sizeof(unsigned char)*RawBufferSize);
	}
	else
	{	
		m_ImagePtr = ShowPtr;	
		m_ImagePtrRaw = RawPtr;
	}

	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ShowStep;
	m_BitCount = ShowBit;
	m_ImageStepRaw = RawStep;
	m_BitCountRaw = RawBit;
	m_Cloned = Clone;
	m_ImageEditPt1.x = m_ImageEditPt1.y = -1;
	m_ImageEditPt2.x = m_ImageEditPt2.y = -1;
	m_ImageEditRect.left = m_ImageEditRect.right = -1;	
	m_ImageEditRect.top  = m_ImageEditRect.bottom = -1;	
	
	if ( true == ResetView )
	{	m_ImageOffset.x = m_ImageOffset.y = 0; }
	else
	{	m_ImageZoom = ImageZoomBefor; }
	CalcImageWndLBtn();
	if ( true == Redraw )
	{
		DrawImage();
		RedrawWnd(FALSE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ReleaseImageBuffer()
{
	if ( true == m_Cloned )
	{
		if ( NULL != m_ImagePtr )
		{	JetMemory.free_func(m_ImagePtr); }
		if ( NULL != m_ImagePtrRaw )
		{	JetMemory.free_func(m_ImagePtrRaw); }
	}

	m_Cloned = false;
	m_ImageW = 0;
	m_ImageH = 0;
	m_ImageStep = 0;
	m_BitCount = 0;
	m_ImagePtr = NULL;	

	m_ImageStepRaw = 0;
	m_BitCountRaw = 0;
	m_ImagePtrRaw = NULL;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here	
	GetClientRect(&m_WndRect);
	m_MemDC1.CreateMemDC(GetSafeHwnd(), m_BkColor);
	m_MemDC2.CreateMemDC(GetSafeHwnd(), m_BkColor);
	m_MemDC3.CreateMemDC(GetSafeHwnd(), m_BkColor);	
	DrawImage();
	RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// TODO: Add your message handler code here
	if ( NULL != m_InspectionFont )
	{	::DeleteObject(m_InspectionFont);	m_InspectionFont=NULL;	}
	CImageWnd::ReleaseImageBuffer();
}
//-------------------------------------------------------------------------------------//
void CImageWnd::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	GetClientRect(&m_WndRect);
	m_MemDC1.CreateMemDC(GetSafeHwnd(), m_BkColor);
	m_MemDC2.CreateMemDC(GetSafeHwnd(), m_BkColor);
	m_MemDC3.CreateMemDC(GetSafeHwnd(), m_BkColor);	
	CStatic::PreSubclassWindow();
}
//-------------------------------------------------------------------------------------//
BOOL CImageWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CStatic::OnEraseBkgnd(pDC);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
	CImageWnd::RedrawWnd(FALSE);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetBKColor(COLORREF bkColor)
{
	m_BkColor = bkColor;	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetBKHatch(int Type, COLORREF Color)
{
	switch ( Type ) 
	{
	case HS_HORIZONTAL:
	case HS_VERTICAL:
	case HS_FDIAGONAL:
	case HS_BDIAGONAL:
	case HS_CROSS:
	case HS_DIAGCROSS:
		m_HatchType = Type;
		break;
	default:
		m_HatchType=HS_API_MAX;
		break;	
	}
	m_HatchColor = Color;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::RedrawWnd(BOOL bRedrawBK)
{
	if ( CImageWnd::GetSafeHwnd() == NULL ) { return; }
	CString  str;	
	int      nTextTop=0;
	CClientDC dc(this);		
	const int nTextH = 24;
	HDC hDC = dc.GetSafeHdc();	
	HDC hBKDC1 = m_MemDC1.GetSafeHdc();
	HDC hBKDC2 = m_MemDC2.GetSafeHdc();
	HDC hBKDC3 = m_MemDC3.GetSafeHdc();	
	POINT WndCP={(m_WndRect.left+m_WndRect.right)/2, (m_WndRect.top+m_WndRect.bottom)/2 };

	const double ComponentAngle = GetImageEditRectAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	double ImageAngle = JetAPI::MapCadAngleToImageAngle(ComponentAngle);

	if ( TRUE == bRedrawBK )
	{	DrawImage();	}

	::IntersectClipRect(hDC, this->m_WndRect.left, this->m_WndRect.top, m_WndRect.right, m_WndRect.bottom);	

	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	::BitBlt(hBKDC2, 0, 0, m_WndRect.right, m_WndRect.bottom, hBKDC1, 0, 0, SRCCOPY );
	if ( true == m_ShowSystem )
	{	DrawSystem(hBKDC2); }

	const int OldBkColor = ::SetBkColor(hBKDC2, ::GetBkColor(hDC));
	const int OldTextColor = ::SetTextColor(hBKDC2, ::GetTextColor(hDC));
	const int OldBkMode = ::SetBkMode(hBKDC2, TRANSPARENT);
	if ( true==m_ShowCursorInfo && m_strCursorInfo.GetLength() > 0 ) 
	{
		::SetTextColor(hBKDC2, 0xFFFFFF);
		::TextOut(hBKDC2, 0, nTextTop, m_strCursorInfo, m_strCursorInfo.GetLength());			
		//::SetTextColor(hBKDC2, OldBkColor);
		nTextTop  += nTextH;
	}

	if ( true==m_ShowImageText && m_strImageText.GetLength()>0 )
	{
		::SetTextColor(hBKDC2, 0xFFFFFF);
		::TextOut(hBKDC2, 0, nTextTop, m_strImageText, m_strImageText.GetLength());			
		//::SetTextColor(hBKDC2, OldBkColor);
		nTextTop  += nTextH;
	}

	//顯示視窗中心線
	if ( true == m_ShowWndCenterLine )
	{
		HPEN hPen = ::CreatePen(PS_DASH, 1, m_clrWndCenterLine);
		HPEN hOldPen = (HPEN)(::SelectObject(hBKDC2, hPen));
		::MoveToEx(hBKDC2, m_WndRect.left, WndCP.y, NULL);
		::LineTo(hBKDC2, m_WndRect.right, WndCP.y);
		::MoveToEx(hBKDC2, WndCP.x, m_WndRect.top, NULL);
		::LineTo(hBKDC2, WndCP.x, m_WndRect.bottom);
		::SelectObject(hBKDC2, hOldPen);
		::DeleteObject(hPen); hPen = NULL;
	}
	
	if ( true==m_ShowCursorLine && IMAGE_WND_DRAW_PROJECT_NORMAL==m_DrawProjectMode )
	{		
		HPEN hPen = ::CreatePen(PS_DOT, 1, m_clrCursorLine);
		HPEN hOldPen = (HPEN)(::SelectObject(hBKDC2, hPen));
		::MoveToEx(hBKDC2, m_WndRect.left, m_MovingPos.y, NULL);
		::LineTo(hBKDC2, m_WndRect.right, m_MovingPos.y);
		::MoveToEx(hBKDC2, m_MovingPos.x, m_WndRect.top, NULL);
		::LineTo(hBKDC2, m_MovingPos.x, m_WndRect.bottom);
		::SelectObject(hBKDC2, hOldPen);
		::DeleteObject(hPen); hPen = NULL;
	}

	if ( true==m_ShowLBtnPos || true==m_ShowEditLine || IMAGE_LBTN_CLICK_EDIT_BOX==m_LBtnClickMode)
	{
		CString tmpStr;
		if ( true == m_ShowEditPosText ) 
		{
			tmpStr.Format(_T("(%d, %d), (%d, %d)"), m_ImageWndEditPt1.x, m_ImageWndEditPt1.y, m_ImageWndEditPt2.x, m_ImageWndEditPt2.y);
			::TextOut(hBKDC2, 0, nTextTop, tmpStr, tmpStr.GetLength());
			nTextTop += nTextH;
		}


		HPEN hPen, hOldPen;
		if (false == IsExceptionAngle)
		{
			hPen = ::CreatePen(PS_SOLID, 1, 0xFF00FF);
			hOldPen = (HPEN)(::SelectObject(hBKDC2, hPen));
			::MoveToEx(hBKDC2, m_ImageWndEditPt1.x - m_ExtendSize.x, m_ImageWndEditPt1.y - m_ExtendSize.y, NULL);
			::LineTo(hBKDC2, m_ImageWndEditPt2.x + m_ExtendSize.x, m_ImageWndEditPt1.y - m_ExtendSize.y);
			::LineTo(hBKDC2, m_ImageWndEditPt2.x + m_ExtendSize.x, m_ImageWndEditPt2.y + m_ExtendSize.y);
			::LineTo(hBKDC2, m_ImageWndEditPt1.x - m_ExtendSize.x, m_ImageWndEditPt2.y + m_ExtendSize.y);
			::LineTo(hBKDC2, m_ImageWndEditPt1.x - m_ExtendSize.x, m_ImageWndEditPt1.y - m_ExtendSize.y);
			::SelectObject(hBKDC2, hOldPen);
			::DeleteObject(hPen); hPen = NULL;
			hPen = ::CreatePen(PS_SOLID, 1, 0x0000FF);
			hOldPen = (HPEN)(::SelectObject(hBKDC2, hPen));
			::MoveToEx(hBKDC2, m_ImageWndEditPt1.x, m_ImageWndEditPt1.y, NULL);
			::LineTo(hBKDC2, m_ImageWndEditPt2.x, m_ImageWndEditPt1.y);
			::LineTo(hBKDC2, m_ImageWndEditPt2.x, m_ImageWndEditPt2.y);
			::LineTo(hBKDC2, m_ImageWndEditPt1.x, m_ImageWndEditPt2.y);
			::LineTo(hBKDC2, m_ImageWndEditPt1.x, m_ImageWndEditPt1.y);
			::SelectObject(hBKDC2, hOldPen);
			::DeleteObject(hPen); hPen = NULL;
		}
		else
		{
			hPen = ::CreatePen(PS_SOLID, 2, 0x0000FF);
			hOldPen = (HPEN)(::SelectObject(hBKDC2, hPen));
			TPOINT2D Cp, dPt1, dPt2;
			TPOINT2D CornerPoint[4];
			dPt1 = m_ImageWndEditPt1;
			dPt2 = m_ImageWndEditPt2;
			Cp.x = (dPt1.x + dPt2.x)*0.5;
			Cp.y = (dPt1.y + dPt2.y)*0.5;
			JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt1);
			JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt2);
			CornerPoint[0].x = dPt1.x;	CornerPoint[0].y = dPt1.y;
			CornerPoint[1].x = dPt2.x;	CornerPoint[1].y = dPt1.y;
			CornerPoint[2].x = dPt2.x;	CornerPoint[2].y = dPt2.y;
			CornerPoint[3].x = dPt1.x;	CornerPoint[3].y = dPt2.y;
			JetAPI::RotateCornerPos(ImageAngle, Cp.x, Cp.y, CornerPoint);
			ImageAPI.DrawPolyLine(hBKDC2, CornerPoint, 4);
			::SelectObject(hBKDC2, hOldPen);
			::DeleteObject(hPen); hPen = NULL;
		}
		
		if ( true == m_ShowEditCenterLine )
		{
			POINT CenterPos;
			hPen = ::CreatePen(PS_DASHDOT, 1, m_clrEditCenterLine);
			hOldPen = (HPEN)(::SelectObject(hBKDC2, hPen));
			CenterPos.x = (m_ImageWndEditPt1.x+m_ImageWndEditPt2.x)/2;
			CenterPos.y = (m_ImageWndEditPt1.y+m_ImageWndEditPt2.y)/2;
			::MoveToEx(hBKDC2, m_ImageWndEditPt1.x, CenterPos.y, NULL);
			::LineTo(hBKDC2, m_ImageWndEditPt2.x, CenterPos.y);
			::MoveToEx(hBKDC2, CenterPos.x, m_ImageWndEditPt1.y, NULL);
			::LineTo(hBKDC2, CenterPos.x, m_ImageWndEditPt2.y);
			::SelectObject(hBKDC2, hOldPen);
			::DeleteObject(hPen); hPen = NULL;
		}
	}

	//顯示影像中心線
	if ( true == m_ShowImgCenterLine )
	{	DrawImageCenterLine(hBKDC2); }

	//顯示相機位置
	if ( true == m_ShowCameraRgn )
	{	DrawCameraRgn(hBKDC2);	}

	//顯示相機中心位置
	if ( true == m_ShowCameraPos )
	{	DrawCameraPos(hBKDC2);	}

	if ( true == m_ShowEditGridLine )
	{
		RECT rcGrid={0};
		size_t i = 0;
		const size_t rcCount = m_EditGridList.size();
		HPEN hPen = ::CreatePen(PS_SOLID, 1, m_clrEditGridLine);
		HPEN hOldPen = (HPEN)(::SelectObject(hBKDC2, hPen));		
		HBRUSH hBrush = ::CreateSolidBrush(m_clrEditGridLine);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hBKDC2, hBrush);	
		for ( i=0; i<rcCount; i++ )
		{	
			switch ( m_EditGridList[i].sEditMode )
			{
			case EDIT_GRID_CENTER_LEFT:
			case EDIT_GRID_CENTER_RIGH:
			case EDIT_GRID_CENTER_TOP:
			case EDIT_GRID_CENTER_BOTTOM:
			case EDIT_GRID_LEFT_TOP:
			case EDIT_GRID_RIGHT_TOP:
			case EDIT_GRID_LEFT_BOTTOM:
			case EDIT_GRID_RIGHT_BOTTOM:
				//this->DrawRect(hBKDC2, m_EditGridList[i].sEditGrid);
				this->DrawPie(hBKDC2, m_EditGridList[i].sEditGrid);
				break;
			//case EDIT_GRID_LEFT:
			//case EDIT_GRID_RIGHT:
			//case EDIT_GRID_TOP:
			//case EDIT_GRID_BOTTOM:
			//	this->DrawRect(hBKDC2, m_EditGridList[i].sEditGrid);
			//	break;
			}	
		}		
		::SelectObject(hBKDC2, hOldBrush);
		::DeleteObject(hBrush); hBrush = NULL;	
		::SelectObject(hBKDC2, hOldPen);
		::DeleteObject(hPen); hPen = NULL;	
	}

	if ( true == m_ShowMeasureLine )
	{			
		HPEN hPen = ::CreatePen(PS_SOLID, 1, m_clrMeasureLine);
		HPEN hOldPen = (HPEN)(::SelectObject(hBKDC2, hPen));		
		HBRUSH hBrush = ::CreateSolidBrush(m_clrMeasureLine);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hBKDC2, hBrush);	

		::MoveToEx(hBKDC2, m_ImageWndEditPt1.x, m_ImageWndEditPt1.y, NULL);
		::LineTo(hBKDC2, m_ImageWndEditPt2.x, m_ImageWndEditPt1.y);
		::LineTo(hBKDC2, m_ImageWndEditPt2.x, m_ImageWndEditPt2.y);
		::LineTo(hBKDC2, m_ImageWndEditPt1.x, m_ImageWndEditPt2.y);
		::LineTo(hBKDC2, m_ImageWndEditPt1.x, m_ImageWndEditPt1.y);				

		::SelectObject(hBKDC2, hOldBrush);
		::DeleteObject(hBrush); hBrush = NULL;	
		::SelectObject(hBKDC2, hOldPen);
		::DeleteObject(hPen); hPen = NULL;	
	}

	::SetBkColor(hBKDC2, OldBkColor);
	::SetTextColor(hBKDC2, OldTextColor);
	::SetBkMode(hBKDC2, OldBkMode);	
	
	DrawProject(hBKDC2);	
	if ( DrawProjectDistrictRect(hBKDC3) == true ) //PATINVERT, PATPAINT, SRCAND, SRCCOPY, SRCERASE, SRCINVERT, SRCPAINT
	{	::BitBlt(hBKDC2, 0, 0, m_WndRect.right, m_WndRect.bottom, hBKDC3, 0, 0, SRCPAINT);	}

	//MSG_IMAGE_WND_PARENT_DRAWING
	if ( this->GetParent()!=NULL && this->GetParent()->GetSafeHwnd()!=NULL )
	{	SendParentMessage(MSG_IMAGE_WND_DRAW_NEXT, (WPARAM)(this->GetDlgCtrlID()), (LPARAM)hBKDC2);		}

	//Copy m_MemHDC Image to MemHDC1 use in Back ground
	::BitBlt(hDC, 0, 0, m_WndRect.right, m_WndRect.bottom, hBKDC2, 0, 0, SRCCOPY );	
}
//-------------------------------------------------------------------------------------//
inline void CImageWnd::DrawPie(HDC hDC, const RECT &Rect)
{
	POINT Pt;
	int HalfSize = (Rect.right-Rect.left)/2;
	Pt.x = (Rect.left+Rect.right)/2;
	Pt.y = (Rect.top+Rect.bottom)/2;
	::Pie(hDC, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x+HalfSize, Pt.y+HalfSize, Pt.x-HalfSize, Pt.y-HalfSize, Pt.x-HalfSize, Pt.y-HalfSize);
}
//-------------------------------------------------------------------------------------//
inline void CImageWnd::DrawRect(HDC hDC, const RECT &Rect)
{
	::MoveToEx(hDC, Rect.left, Rect.top, NULL);
	::LineTo(hDC, Rect.right, Rect.top);
	::LineTo(hDC, Rect.right, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.bottom);
	::LineTo(hDC, Rect.left, Rect.top);
}
//-------------------------------------------------------------------------------------//
inline void CImageWnd::DrawRectLine(HDC hDC, const POINT pt[], size_t num)
{
	if ( 4 == num )
	{
		::MoveToEx(hDC, pt[0].x, pt[0].y, NULL);		
		::LineTo(hDC, pt[1].x, pt[1].y);
		::LineTo(hDC, pt[2].x, pt[2].y);
		::LineTo(hDC, pt[3].x, pt[3].y);
		::LineTo(hDC, pt[0].x, pt[0].y);
	}
	else
	{
		size_t i=0;
		for ( i=0; i<num; i++ )
		{
			if ( 0 == i ) 
			{	::MoveToEx(hDC, pt[i].x, pt[i].y, NULL);	}
			else
			{	::LineTo(hDC, pt[i].x, pt[i].y);	}
		}
		if ( 1 == num )
		{	::LineTo(hDC, pt[0].x, pt[0].y);	}
	}
}
//-------------------------------------------------------------------------------------//
inline bool CImageWnd::DrawRectRoughLine(HDC hDC, const RECT &Rect, COLORREF color, int Gap)
{
	bool bDraw=true;	
	const int RectW=Rect.right-Rect.left;
	const int RectH=Rect.bottom-Rect.top;
	const int RectX=(Rect.right+Rect.left)/2;
	const int RectY=(Rect.bottom+Rect.top)/2;
	if ( RectW < Gap )
	{
		if ( RectH < Gap )
		{	::SetPixel(hDC, RectX, RectY, color);	}
		else
		{
			::MoveToEx(hDC, RectX, Rect.top, NULL);
			::LineTo(hDC, RectX, Rect.bottom);
		}
	}
	else
	{
		if ( RectH < Gap )
		{
			::MoveToEx(hDC, Rect.left, RectY, NULL);
			::LineTo(hDC, Rect.right, RectY);
		}
		else
		{	bDraw = false;	}
	}
	return bDraw;
}
//-------------------------------------------------------------------------------------//
inline void CImageWnd::DrawCircleLine(HDC hDC, const RECT &Rect)
{
	int x = (Rect.left);
	int y = (Rect.top+Rect.bottom)/2;
	::MoveToEx(hDC, x, y, NULL);		
	::ArcTo(hDC, Rect.left, Rect.top, Rect.right, Rect.bottom, x, y, x, y);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawSystem(HDC hDC)//繪製專案-系統模式
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr ) { return ; }

	CString      str;	
	size_t       i = 0;
	CAMERA_ID    CameraID = m_CameraID;	
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	if ( 0==ImageW || 0==ImageH ) { return ; }

	TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const bool Fixed14Lane = PlcCtrlPtr->GetLaneAdjustFixed14Lane();	
	const bool bSignX = AOIDataCollect.GetStageSignPositiveX();
	const bool bSignY = AOIDataCollect.GetStageSignPositiveY();

	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};	
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes = m_ImageResolution;
	TREGION4D    ImageStageRgn = m_ImageStageRgn;	
	RESULT_ID    ResultID = RESULT_ID_NONE;
	LANE_WORK_MODE LaneWorkMode_LA = SystemParam.m_LaneWorkMode_LA;
	LANE_WORK_MODE LaneWorkMode_LB = SystemParam.m_LaneWorkMode_LB;

	TLaneRgn     LaneRgn;
	double       LandWidth=0;	
	std::vector<TLaneRgn> LaneRgnList;
	const LANE_ID LaneID = GetShowLaneID();
	const double  LaneSizeY = 20*1000;//mm;
	const double  LaneSizeX = fabs(MotionParam.m_PCBStopRPosX_LA-MotionParam.m_PCBStopLPosX_LA);		
	const bool    LaneAdjustDisableBtn = PlcCtrlPtr->GetLaneAdjustDisableBtn(LaneID);

	if ( LANE_ID_A == LaneID || (LANE_ID_BOTH==LaneID && LANE_WORK_DISABLE!=LaneWorkMode_LA) )
	{					
		StagePos.x = (MotionParam.m_PCBStopRPosX_LA+MotionParam.m_PCBStopLPosX_LA)*0.5;
		StagePos.y = (MotionParam.m_PCBStopRPosY_LA+MotionParam.m_PCBStopLPosY_LA)*0.5;

		LaneRgn.eLaneID = LANE_ID_A;
		LaneRgn.sLanePos1 = StagePos;
		if ( false == LaneAdjustDisableBtn )			
		{
			LandWidth = PlcCtrlPtr->GetLaneAdjustCurrentPos_LA()*1000;			
			if ( true == bSignY )
			{	StagePos.y += LandWidth;	}
			else
			{	StagePos.y -= LandWidth;	}				
		}
		LaneRgn.sLanePos2 = StagePos;
		LaneRgnList.push_back(LaneRgn);
	}
	if ( LANE_ID_B == LaneID || (LANE_ID_BOTH==LaneID && LANE_WORK_DISABLE!=LaneWorkMode_LB) )
	{		
		StagePos.x = (MotionParam.m_PCBStopRPosX_LB+MotionParam.m_PCBStopLPosX_LB)*0.5;
		StagePos.y = (MotionParam.m_PCBStopRPosY_LB+MotionParam.m_PCBStopLPosY_LB)*0.5;

		LaneRgn.eLaneID = LANE_ID_B;
		LaneRgn.sLanePos1 = StagePos;
		if ( false == LaneAdjustDisableBtn )
		{
			LandWidth = PlcCtrlPtr->GetLaneAdjustCurrentPos_LB()*1000;
			if ( true == Fixed14Lane )
			{
				if ( true == bSignY )
				{	StagePos.y -= LandWidth;	}
				else
				{	StagePos.y += LandWidth;	}
			}
			else
			{
				if ( true == bSignY )
				{	StagePos.y += LandWidth;	}
				else
				{	StagePos.y -= LandWidth;	}
			}
		}
		LaneRgn.sLanePos2 = StagePos;
		LaneRgnList.push_back(LaneRgn);		
	}
	
	const bool      bCheckInsideWnd=false;
	const size_t    LaneCount = LaneRgnList.size();		
	const COLORREF  clrLane  = 0xE8A200;//0xE8A200//0x0080FF//0xEAD999
	const COLORREF  clrLane1 = 0x8080FF;//0xE8A200
	const COLORREF  clrLane2 = 0xEAD999;//0xE8A200
	const COLORREF  clrLimit = 0xFFFFFF;
	const COLORREF  clrText  = 0xFFFFFF;	
	CString         strLane = AOIDataDefine.GetLaneText();

	HPEN hPenLane  = ::CreatePen(PS_SOLID, 1, clrLane);	
	HPEN hPenLimit = ::CreatePen(PS_DOT, 1, clrLimit);	
	HBRUSH hBrush1  = ::CreateHatchBrush(HS_DIAGCROSS, clrLane1);
	HBRUSH hBrush2  = ::CreateHatchBrush(HS_DIAGCROSS, clrLane2);

	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPenLane));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		

	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();	
	::SelectObject(hDC, hPenLane);
	for ( i=0; i<LaneCount; i++ )
	{			
		LaneRgn  = LaneRgnList[i];
		StagePos = LaneRgn.sLanePos1;
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }

		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( true == bCheckInsideWnd )
		{
			if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }
		}

		StgCornerPos[0].x = StagePos.x-(LaneSizeX*0.5);	StgCornerPos[0].y = StagePos.y-(LaneSizeY*0.5);
		StgCornerPos[1].x = StagePos.x+(LaneSizeX*0.5);	StgCornerPos[1].y = StagePos.y-(LaneSizeY*0.5);
		StgCornerPos[2].x = StagePos.x+(LaneSizeX*0.5);	StgCornerPos[2].y = StagePos.y+(LaneSizeY*0.5);
		StgCornerPos[3].x = StagePos.x-(LaneSizeX*0.5);	StgCornerPos[3].y = StagePos.y+(LaneSizeY*0.5);
		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);		
		if ( JetAPI::CheckCornerInRect(CornerPos, m_WndRect)==false && true==bCheckInsideWnd )
		{	continue; }

		JetAPI::CornerPtToRect(CornerPos, Rect);		
		::FillRect(hDC, &Rect, hBrush1);
		DrawRectLine(hDC, CornerPos, 4);

		switch ( LaneRgn.eLaneID )
		{		
		case LANE_ID_B:	str.Format(_T("%s-B"), strLane);	break;
		default:
		case LANE_ID_A:	str.Format(_T("%s-A"), strLane);	break;			
		}				
		//::TextOut(hDC, Pt.x, Pt.y-10, str, str.GetLength());
		if ( LANE_ID_B == LaneRgn.eLaneID )
		{
			if ( true == Fixed14Lane )
			{	::TextOut(hDC, Pt.x, Rect.bottom+8, str, str.GetLength());	}
			else
			{	::TextOut(hDC, Pt.x, Rect.top-24, str, str.GetLength());	}
		}
		else
		{	::TextOut(hDC, Pt.x, Rect.top-24, str, str.GetLength()); }

		//第2片
		if ( false == LaneAdjustDisableBtn )
		{
			StagePos = LaneRgn.sLanePos2;
			if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
			if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }

			AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
			MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

			JetAPI::Point2DToPoint(WndPt, Pt);
			if ( true == bCheckInsideWnd )
			{	
				if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }
			}

			StgCornerPos[0].x = StagePos.x-(LaneSizeX*0.5);	StgCornerPos[0].y = StagePos.y-(LaneSizeY*0.5);
			StgCornerPos[1].x = StagePos.x+(LaneSizeX*0.5);	StgCornerPos[1].y = StagePos.y-(LaneSizeY*0.5);
			StgCornerPos[2].x = StagePos.x+(LaneSizeX*0.5);	StgCornerPos[2].y = StagePos.y+(LaneSizeY*0.5);
			StgCornerPos[3].x = StagePos.x-(LaneSizeX*0.5);	StgCornerPos[3].y = StagePos.y+(LaneSizeY*0.5);
		
			AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
			MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
			MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
			MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
			MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

			JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
			if ( JetAPI::CheckCornerInRect(CornerPos, m_WndRect)==false && true==bCheckInsideWnd )
			{	continue; }

			JetAPI::CornerPtToRect(CornerPos, Rect);		
			::FillRect(hDC, &Rect, hBrush2);
			DrawRectLine(hDC, CornerPos, 4);
		}
	}
	
	::SelectObject(hDC, hPenLimit);
	StgCornerPos[0].x = ImageStageRgn.minX;	StgCornerPos[0].y = ImageStageRgn.minY;
	StgCornerPos[1].x = ImageStageRgn.maxX;	StgCornerPos[1].y = ImageStageRgn.minY;
	StgCornerPos[2].x = ImageStageRgn.maxX;	StgCornerPos[2].y = ImageStageRgn.maxY;
	StgCornerPos[3].x = ImageStageRgn.minX;	StgCornerPos[3].y = ImageStageRgn.maxY;

	AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
	MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
	MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
	MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
	MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

	JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
	if ( JetAPI::CheckCornerInRect(CornerPos, m_WndRect)==true || false==bCheckInsideWnd )
	{	DrawRectLine(hDC, CornerPos, 4);	}			

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hBrush1); hBrush1 = NULL;
	::DeleteObject(hBrush2); hBrush2 = NULL;
	::DeleteObject(hPenLane); hPenLane = NULL;	
	::DeleteObject(hPenLimit); hPenLimit = NULL;	
	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);
	return;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawMesStats(HDC hDC)//繪製MES
{	return;
#ifndef MES_DISABLE
	if ( NULL == hDC ) { return; }
	CString str;
	POINT   TextPt;	
	int     FontSize = 18;
	RECT    WndRect = m_WndRect;	
	COLORREF OldTextColor=::SetTextColor(hDC, 0xFFFFFF);

	TextPt.y = 4;
	TextPt.x = WndRect.right-80;
	switch ( AOIDataCollect.GetMES_EqpCtrlStateMode() )
	{
	case MES_EQP_CTRL_STATE_NONE:	str=_T("None"); break;
	case MES_EQP_CTRL_STATE_OFFLINE:str=_T("Offline"); break;	
	case MES_EQP_CTRL_STATE_LOCAL:	str=_T("Online-Local"); break;
	case MES_EQP_CTRL_STATE_REMOTE:	str=_T("Online-Remote"); break;
	}
	::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength()); TextPt.y+=FontSize;

	if ( true == AOIDataCollect.GetMES_RemoteCtrlOn() )
	{	str = _T("RCMD:On");	}
	else
	{	str = _T("RCMD:Off");	}	
	::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength()); TextPt.y+=FontSize;

	::SetTextColor(hDC, OldTextColor);
#endif//MES_DISABLE
	return ;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProject(HDC hDC)//繪製專案
{
	DrawMesStats(hDC);
	switch ( m_DrawProjectMode )
	{	
	case IMAGE_WND_DRAW_PROJECT_NORMAL:	    DrawProjectNormal(hDC);	break;
	case IMAGE_WND_DRAW_PROJECT_INSPECTING:	DrawProjectInspecting(hDC);	break;
	}
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectFdList(HDC hDC)//繪製專案定位點
{
	//return;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	CString      str;
	size_t       i = 0;
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;
	TSIZE2D      FdPatExtend, FdRoiExtend;
	TPOINT2D     ImageRes = this->m_ImageResolution;
	TREGION4D    ImageStageRgn = this->m_ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;
	RESULT_ID    ResultID = RESULT_ID_NONE;

	CAOIFd        *FdPtr = NULL;			
	const LANE_ID  LaneID = GetShowLaneID();
	const size_t   FdCount = ProjectPtr->GetProjectFdCount();	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_FdColor1;
	const COLORREF  clr2 = SystemParam.m_FdColor2;
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrText = SystemParam.m_FdTextColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;
	const DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, clr1);	
	HPEN hPen2 = ::CreatePen(PS_SOLID, 1, clr2);
	HPEN hPenOK = ::CreatePen(PS_SOLID, 1, clrOK);
	HPEN hPenNG = ::CreatePen(PS_SOLID, 1, clrNG);
	HPEN hPenSkip = ::CreatePen(PS_SOLID, 1, clrSkip);
	HPEN hPenBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	HPEN hPenException = ::CreatePen(PS_SOLID, 1, clrException);
	
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		

	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();	
	for ( i=0; i<FdCount; i++ )
	{	
		if ( ProjectPtr->CheckProjectLocking() == true )
		{	break; }
		FdPtr = ProjectPtr->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }
		ResultID = FdPtr->GetFdResultID_AOI_Lane(LaneID);
		StagePos.x = FdPtr->GetFdStagePosX();
		StagePos.y = FdPtr->GetFdStagePosY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }

		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }

		FdPtr->GetFdBodyStageCornerPos(StgCornerPos);		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, m_WndRect) == false )
		{	continue; }

		switch ( ResultID )
		{
		case RESULT_ID_NONE:	::SelectObject(hDC, hPen);	break;
		case RESULT_ID_OK:		::SelectObject(hDC, hPenOK);break;
		case RESULT_ID_NG:		::SelectObject(hDC, hPenNG);break;
		case RESULT_ID_SKIP:	::SelectObject(hDC, hPenSkip);break;
		case RESULT_ID_BYPASS:	::SelectObject(hDC, hPenBypass);break;		
		case RESULT_ID_EXCEPTION:
			::SelectObject(hDC, hPenException);
			break;
		default:
			::SelectObject(hDC, hPen);
			break;
		}
		DrawRectLine(hDC, CornerPos, 4);		

		//Roi Extend Range;			
		FdPtr->GetFdRoiStageCornerPos(StgCornerPos);
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, m_WndRect) == false )
		{	continue; }

		::SelectObject(hDC, hPen2);				
		JetAPI::PointsToRect(CornerPos, 4, Rect);
		DrawCircleLine(hDC, Rect);
		//CImageWnd::DrawRectLine(hDC, CornerPos, 4);		

		str.Format(_T("Fd-%d"), FdPtr->GetFdIndex_Panel()+1);		
		::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	
	::DeleteObject(hPen2); hPen2 = NULL;		
	::DeleteObject(hPenOK); hPenOK=NULL;
	::DeleteObject(hPenNG); hPenNG=NULL;
	::DeleteObject(hPenSkip); hPenSkip = NULL;
	::DeleteObject(hPenBypass); hPenBypass = NULL;
	::DeleteObject(hPenException); hPenException = NULL;
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectFieldList(HDC hDC, bool ShowRect)//繪製專案區域
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	CString      str;	
	size_t       i = 0;
	size_t       FieldFinishCount=0;
	size_t       ComponentFinishCount=0;
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes = this->m_ImageResolution;
	TREGION4D    ImageStageRgn = this->m_ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;	
	size_t       FieldIdx=0;
	CAOIFov     *FovPtr = NULL;
	CAOIField   *FieldPtr = NULL;	
	FIELD_MERGE_STATE FieldMergeState;
	const size_t FieldCount = ProjectPtr->GetProjectFieldCount();
	const DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, 0x804080);
	HPEN hPen2 = ::CreatePen(PS_SOLID, 1, 0x00D0D0);
	HPEN hPen3 = ::CreatePen(PS_SOLID, 1, 0x4040D0);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrText = ::SetTextColor(hDC, 0x2200A0);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);

	FieldFinishCount = 0;	
	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();	
	for ( i=0; i<FieldCount; i++ )
	{	
		if ( ProjectPtr->CheckProjectLocking() == true )
		{	break; }

		FieldIdx = FieldCount-i-1;
		FieldPtr = ProjectPtr->GetProjectFieldPtr(FieldIdx, true);			
		if ( NULL == FieldPtr ) { continue; }	
		if ( DistrictID!= FieldPtr->GetFieldDistrictID() ) { continue; }

		FieldMergeState = FieldPtr->GetFieldMergeState();
		switch ( FieldMergeState )
		{
		case FIELD_MERGE_DONE:
		case FIELD_MERGE_CLEAR:
			FieldFinishCount ++;
			break;
		}
		if ( false == ShowRect ) { continue; }

		FieldPtr->GetFieldStageRgn_Inner(ObjStageRgn);
		StagePos.x = ObjStageRgn.GetCpX();
		StagePos.y = ObjStageRgn.GetCpY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);
		
		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }		
		
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, ObjStageRgn, ImageStagePos, ObjImageRgn);
		ImgCornerPos[0].x = ObjImageRgn.minX;	ImgCornerPos[0].y = ObjImageRgn.minY;
		ImgCornerPos[1].x = ObjImageRgn.maxX;	ImgCornerPos[1].y = ObjImageRgn.maxY;
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);	
		
		CornerPos[0].x = JetAPI::Floor(WndCornerPos[0].x);	CornerPos[0].y = JetAPI::Floor(WndCornerPos[0].y);
		CornerPos[1].x = JetAPI::Floor(WndCornerPos[1].x);	CornerPos[1].y = JetAPI::Floor(WndCornerPos[1].y);			
		if ( CornerPos[0].x<0 || CornerPos[0].y<0 || CornerPos[0].x>m_WndRect.right || CornerPos[0].y>m_WndRect.bottom ) { continue; }
		if ( CornerPos[1].x<0 || CornerPos[1].y<0 || CornerPos[1].x>m_WndRect.right || CornerPos[1].y>m_WndRect.bottom ) { continue; }

		switch ( FieldMergeState )
		{		
		case FIELD_MERGE_CLEAR:	::SelectObject(hDC, hPen2);	break;
		case FIELD_MERGE_DONE:	::SelectObject(hDC, hPen3);	break;
		default:				::SelectObject(hDC, hPen);	break;			
		}
		Rect.left   = MIN(CornerPos[0].x, CornerPos[1].x);
		Rect.right  = MAX(CornerPos[0].x, CornerPos[1].x);
		Rect.top    = MIN(CornerPos[0].y, CornerPos[1].y);
		Rect.bottom = MAX(CornerPos[0].y, CornerPos[1].y);
		DrawRect(hDC, Rect);				
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	
	::DeleteObject(hPen2); hPen2 = NULL;			
	::DeleteObject(hPen3); hPen3 = NULL;			
	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrText);
	m_FieldFinishCount = FieldFinishCount;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectPanelList(HDC hDC, bool ShowName)//繪製專案整板
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	CString      str;
	CString      strPanel;	
	size_t       i = 0;	
	size_t       ComponentFinishCount=0;
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	RECT         Rect={0};	
	POINT        Pt={0}, CornerPos[4];	
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes = this->m_ImageResolution;
	TREGION4D    ImageStageRgn = this->m_ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;	
	RESULT_ID    ResultID = RESULT_ID_NONE;
	
	CAOIPanel    *PanelPtr = NULL;
	const LANE_ID LaneID = GetShowLaneID();
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, 0x40B0B0);	
	HPEN hPenSel = ::CreatePen(PS_SOLID, 1, 0xFF00FF);
	HPEN hPenSkip = ::CreatePen(PS_SOLID, 1, clrSkip);
	HPEN hPenBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	HPEN hPenException = ::CreatePen(PS_SOLID, 1, clrException);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrText = ::SetTextColor(hDC, 0x2200A0);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);				
	
	strPanel = AOIDataDefine.GetPanelText();
	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();
	for ( i=0; i<PanelCount; i++ )
	{	
		if ( ProjectPtr->CheckProjectLocking() == true )
		{	break; }

		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		ObjStageRgn = PanelPtr->GetPanelRgnStage(DistrictID);
		StagePos.x = ObjStageRgn.GetCpX();
		StagePos.y = ObjStageRgn.GetCpY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);
		
		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }		
		
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, ObjStageRgn, ImageStagePos, ObjImageRgn);
		ImgCornerPos[0].x = ObjImageRgn.minX;	ImgCornerPos[0].y = ObjImageRgn.minY;
		ImgCornerPos[1].x = ObjImageRgn.maxX;	ImgCornerPos[1].y = ObjImageRgn.maxY;
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);			

		CornerPos[0].x = JetAPI::Floor(WndCornerPos[0].x);	CornerPos[0].y = JetAPI::Floor(WndCornerPos[0].y);
		CornerPos[1].x = JetAPI::Floor(WndCornerPos[1].x);	CornerPos[1].y = JetAPI::Floor(WndCornerPos[1].y);			
		if ( CornerPos[0].x<0 || CornerPos[0].y<0 || CornerPos[0].x>m_WndRect.right || CornerPos[0].y>m_WndRect.bottom ) { continue; }
		if ( CornerPos[1].x<0 || CornerPos[1].y<0 || CornerPos[1].x>m_WndRect.right || CornerPos[1].y>m_WndRect.bottom ) { continue; }
		
		Rect.left   = MIN(CornerPos[0].x, CornerPos[1].x);
		Rect.right  = MAX(CornerPos[0].x, CornerPos[1].x);
		Rect.top    = MIN(CornerPos[0].y, CornerPos[1].y);
		Rect.bottom = MAX(CornerPos[0].y, CornerPos[1].y);

		if ( PanelPtr->GetPanelSelected() == true ) 
		{	::SelectObject(hDC, hPenSel);	}
		else
		{
			ResultID = PanelPtr->GetPanelResultID_AOI_Lane(LaneID);
			switch ( ResultID )
			{
			case RESULT_ID_SKIP:	::SelectObject(hDC, hPenSkip);	break;
			case RESULT_ID_BYPASS:	::SelectObject(hDC, hPenBypass);	break;
			default:				::SelectObject(hDC, hPen);			break;
			}
		}
		DrawRect(hDC, Rect);
		if ( true==ShowName )
		{
			str.Format(_T("%s:%d"), strPanel, i+1);			
			::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
		}
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	::DeleteObject(hPenSel); hPenSel = NULL;
	::DeleteObject(hPenSkip); hPenSkip = NULL;
	::DeleteObject(hPenBypass); hPenBypass = NULL;	
	::DeleteObject(hPenException); hPenException = NULL;
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrText);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectBoardList(HDC hDC, bool ShowName)//繪製專案單板
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	CString      str;
	CString      strBoard;	
	size_t       i = 0;	
	size_t       ComponentFinishCount=0;
	CAMERA_ID    CameraID = m_CameraID;	
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	RECT         Rect={0};	
	POINT        Pt={0}, CornerPos[4];	
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes = this->m_ImageResolution;
	TREGION4D    ImageStageRgn = this->m_ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;	
	RESULT_ID    ResultID = RESULT_ID_NONE;	
	CAOIBoard   *BoardPtr = NULL;	
	unsigned int PanelIndex=0;
	unsigned int BoardIndex=0;
	
	const LANE_ID LaneID = GetShowLaneID();
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();		
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_BoardColor1;
	const COLORREF  clr2 = SystemParam.m_BoardColor2;
	const COLORREF  clrText = SystemParam.m_BoardTextColor;	
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;	
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;	
	const COLORREF  clrSelected = SystemParam.m_BoardSelectedColor;
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;	
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	LOGFONT LogFont;
	HFONT   hFont = NULL;
	HFONT   hOldFont = NULL;
	const int FontSize = 64;//32	
	const int FontSizeX = FontSize/2;
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = FontSize;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));	
	hFont = CreateFontIndirect(&LogFont);
	hOldFont = (HFONT)::SelectObject(hDC, hFont);

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, clr1);	
	HPEN hPenSel = ::CreatePen(PS_SOLID, 1, clrSelected);
	HPEN hPenSkip = ::CreatePen(PS_SOLID, 1, clrSkip);
	HPEN hPenBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	HPEN hPenException = ::CreatePen(PS_SOLID, 1, clrException);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);				
	
	strBoard = AOIDataDefine.GetBoardText();
	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();
	for ( i=0; i<BoardCount; i++ )
	{	
		if ( ProjectPtr->CheckProjectLocking() == true )
		{	break; }
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		PanelIndex = BoardPtr->GetBoardPanelIndex_Project();
		BoardIndex = BoardPtr->GetBoardIndex_Panel();
		ObjStageRgn = BoardPtr->GetBoardRgnStage(DistrictID);
		StagePos.x = ObjStageRgn.GetCpX();
		StagePos.y = ObjStageRgn.GetCpY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }		
		
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, ObjStageRgn, ImageStagePos, ObjImageRgn);
		ImgCornerPos[0].x = ObjImageRgn.minX;	ImgCornerPos[0].y = ObjImageRgn.minY;
		ImgCornerPos[1].x = ObjImageRgn.maxX;	ImgCornerPos[1].y = ObjImageRgn.maxY;
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);			

		CornerPos[0].x = JetAPI::Floor(WndCornerPos[0].x);	CornerPos[0].y = JetAPI::Floor(WndCornerPos[0].y);
		CornerPos[1].x = JetAPI::Floor(WndCornerPos[1].x);	CornerPos[1].y = JetAPI::Floor(WndCornerPos[1].y);			
		if ( CornerPos[0].x<0 || CornerPos[0].y<0 || CornerPos[0].x>m_WndRect.right || CornerPos[0].y>m_WndRect.bottom ) { continue; }
		if ( CornerPos[1].x<0 || CornerPos[1].y<0 || CornerPos[1].x>m_WndRect.right || CornerPos[1].y>m_WndRect.bottom ) { continue; }
		
		Rect.left   = MIN(CornerPos[0].x, CornerPos[1].x);
		Rect.right  = MAX(CornerPos[0].x, CornerPos[1].x);
		Rect.top    = MIN(CornerPos[0].y, CornerPos[1].y);
		Rect.bottom = MAX(CornerPos[0].y, CornerPos[1].y);

		if ( BoardPtr->GetBoardSelected() == true ) 
		{	::SelectObject(hDC, hPenSel);	}
		else
		{
			ResultID = BoardPtr->GetBoardResultID_AOI_Lane(LaneID);
			switch ( ResultID )
			{
			case RESULT_ID_SKIP:	::SelectObject(hDC, hPenSkip);	break;
			case RESULT_ID_BYPASS:	::SelectObject(hDC, hPenBypass);	break;
			default:				::SelectObject(hDC, hPen);			break;
			}
		}

		DrawRect(hDC, Rect);
		if ( true==ShowName )
		{				
			const int MinW=8;
			const int MinH=8;			
			const int RectW=Rect.right-Rect.left;
			const int RectH=Rect.bottom-Rect.top;
			if ( RectW>MinW && RectH>MinH )
			{
				RECT BackupRect=Rect;
				::InflateRect(&Rect, -(MinW/2), -(MinH/2));
				//str.Format(_T("%s:%d"), strBoard, i+1);			
				if ( 1 == PanelCount )
				{	str.Format(_T("%d"), BoardIndex+1);	}
				else
				{	str.Format(_T("%d-%d"), PanelIndex+1, BoardIndex+1);	}
			
				int Len = str.GetLength();
				int LenSizeX = Len*FontSizeX;			
				//::TextOut(hDC, posX, posY, str, Len);
				if ( LenSizeX < (Rect.right-Rect.left) )			
				{	
					int posX = (Rect.left + Rect.right - LenSizeX) / 2;
					int posY = (Rect.top + Rect.bottom - FontSize) / 2;
					//::TextOut(hDC, Rect.left+4, Rect.top, str, Len);	
					::TextOut(hDC, posX, posY, str, Len);	
				}
				else
				{
					str.Format(_T("%d"), BoardIndex+1);
					Len = str.GetLength();
					LenSizeX = Len*FontSizeX;
					if ( LenSizeX < (Rect.right-Rect.left) )			
					{	
						int posX = (Rect.left + Rect.right - LenSizeX) / 2;
						int posY = (Rect.top + Rect.bottom - FontSize) / 2;
						::TextOut(hDC, posX, posY, str, Len);	
					}
				}
				Rect=BackupRect;
			}
		}
	}		
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont); hFont = NULL;
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	::DeleteObject(hPenSel); hPenSel = NULL;
	::DeleteObject(hPenSkip); hPenSkip = NULL;	
	::DeleteObject(hPenBypass); hPenBypass = NULL;	
	::DeleteObject(hPenException); hPenException = NULL;		
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectMarkList(HDC hDC, bool ShowName)//繪製專案特徵點
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	CString      str;
	int          Len=0;
	int          LenSizeX=0;
	size_t       i = 0;
	COLORREF     Color=0;
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes = this->m_ImageResolution;
	TREGION4D    ImageStageRgn = this->m_ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;	
	RESULT_ID    ResultID = RESULT_ID_NONE;	
	CAOIMark    *MarkPtr = NULL;	
	const LANE_ID      LaneID = GetShowLaneID();	
	const DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	const size_t MarkCount = ProjectPtr->GetProjectMarkCount();
	const bool  bDrawRoughLine = ProjectPtr->CheckProjectMarkUseDrawRoughLine();

	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_BarcodeColor1;
	const COLORREF  clr2 = SystemParam.m_BarcodeColor2;
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;	
	const COLORREF  clrText = SystemParam.m_BarcodeTextColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;	
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, clr1);
	HPEN hPen2 = ::CreatePen(PS_SOLID, 1, clr2);	
	HPEN hPenOK = ::CreatePen(PS_SOLID, 1, clrOK);
	HPEN hPenNG = ::CreatePen(PS_SOLID, 1, clrNG);
	HPEN hPenSkip = ::CreatePen(PS_SOLID, 1, clrSkip);
	HPEN hPenBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	HPEN hPenException = ::CreatePen(PS_SOLID, 1, clrException);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		

	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();		
	for ( i=0; i<MarkCount; i++ )
	{	
		if ( ProjectPtr->CheckProjectLocking() == true )
		{	break; }
		MarkPtr = ProjectPtr->GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }
		ResultID = MarkPtr->GetMarkResultID_AOI_Lane(LaneID);

		StagePos.x = MarkPtr->GetMarkStagePosX();
		StagePos.y = MarkPtr->GetMarkStagePosY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);
		
		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }
		
		MarkPtr->GetMarkBodyStageCornerPos(StgCornerPos);
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, m_WndRect) == false )
		{	continue; }
		JetAPI::CornerPtToRect(CornerPos, Rect);
		switch ( ResultID )
		{
		case RESULT_ID_NONE:	::SelectObject(hDC, hPen);	 Color=clr1;	break;
		case RESULT_ID_OK:		::SelectObject(hDC, hPenOK); Color=clrOK;	break;
		case RESULT_ID_NG:		::SelectObject(hDC, hPenNG); Color=clrNG;	break;
		case RESULT_ID_SKIP:	::SelectObject(hDC, hPenSkip); Color=clrSkip;	break;
		case RESULT_ID_BYPASS:	::SelectObject(hDC, hPenBypass); Color=clrBypassed;	break;
		case RESULT_ID_EXCEPTION:
			Color=clrException;	
			::SelectObject(hDC, hPenException);
			break;
		default:
			Color=clr2;	
			::SelectObject(hDC, hPen2);
			break;
		}
		if ( true == bDrawRoughLine )
		{
			if ( DrawRectRoughLine(hDC, Rect, Color) == true )		
			{	 continue; }
		}
		DrawRectLine(hDC, CornerPos, 4);
		if ( TRUE==ShowName )
		{	
			str.Format(_T("Mark %d"), MarkPtr->GetMarkIndex_Project()+1);
			Len = str.GetLength();
			LenSizeX = Len*6;
			if ( LenSizeX < (Rect.right-Rect.left) )
			{
				//::TextOut(hDC, Rect.left, Rect.top, str, str.GetLength());
				::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
			}
		}
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	
	::DeleteObject(hPen2); hPen2 = NULL;
	::DeleteObject(hPenOK); hPenOK=NULL;
	::DeleteObject(hPenNG); hPenNG=NULL;
	::DeleteObject(hPenSkip); hPenSkip = NULL;
	::DeleteObject(hPenBypass); hPenBypass = NULL;
	::DeleteObject(hPenException); hPenException = NULL;
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectBarcodeList(HDC hDC, bool ShowName)//繪製專案條碼
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	CString      str;
	int          Len=0;
	int          LenSizeX=0;
	size_t       i = 0;
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes = this->m_ImageResolution;
	TREGION4D    ImageStageRgn = this->m_ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;	
	RESULT_ID    ResultID = RESULT_ID_NONE;	
	CAOIBarcode   *BarcodePtr = NULL;	
	const LANE_ID      LaneID = GetShowLaneID();
	const DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	const size_t BarcodeCount = ProjectPtr->GetProjectBarcodeCount();
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_BarcodeColor1;
	const COLORREF  clr2 = SystemParam.m_BarcodeColor2;
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;	
	const COLORREF  clrText = SystemParam.m_BarcodeTextColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;	
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, clr1);
	HPEN hPen2 = ::CreatePen(PS_SOLID, 1, clr2);	
	HPEN hPenOK = ::CreatePen(PS_SOLID, 1, clrOK);
	HPEN hPenNG = ::CreatePen(PS_SOLID, 1, clrNG);
	HPEN hPenSkip = ::CreatePen(PS_SOLID, 1, clrSkip);
	HPEN hPenBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	HPEN hPenException = ::CreatePen(PS_SOLID, 1, clrException);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		

	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();		
	for ( i=0; i<BarcodeCount; i++ )
	{	
		if ( ProjectPtr->CheckProjectLocking() == true )
		{	break; }
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }
		ResultID = BarcodePtr->GetBarcodeResultID_AOI_Lane(LaneID);

		StagePos.x = BarcodePtr->GetBarcodeStagePosX();
		StagePos.y = BarcodePtr->GetBarcodeStagePosY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);
		
		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }
		
		BarcodePtr->GetBarcodeBodyStageCornerPos(StgCornerPos);
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, m_WndRect) == false )
		{	continue; }
			
		switch ( ResultID )
		{
		case RESULT_ID_NONE:	::SelectObject(hDC, hPen);	break;
		case RESULT_ID_OK:		::SelectObject(hDC, hPenOK);break;
		case RESULT_ID_NG:		::SelectObject(hDC, hPenNG);break;
		case RESULT_ID_SKIP:	::SelectObject(hDC, hPenSkip);break;
		case RESULT_ID_BYPASS:	::SelectObject(hDC, hPenBypass);break;
		case RESULT_ID_EXCEPTION:
			::SelectObject(hDC, hPenException);
			break;
		default:
			::SelectObject(hDC, hPen2);
			break;
		}
		DrawRectLine(hDC, CornerPos, 4);
		if ( TRUE==ShowName )
		{
			JetAPI::CornerPtToRect(CornerPos, Rect);
			str.Format(_T("Code %d"), BarcodePtr->GetBarcodeIndex_Project()+1);
			Len = str.GetLength();
			LenSizeX = Len*6;
			if ( LenSizeX < (Rect.right-Rect.left) )
			{
				//::TextOut(hDC, Rect.left, Rect.top, str, str.GetLength());
				::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
			}
		}		
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	
	::DeleteObject(hPen2); hPen2 = NULL;
	::DeleteObject(hPenOK); hPenOK=NULL;
	::DeleteObject(hPenNG); hPenNG=NULL;
	::DeleteObject(hPenSkip); hPenSkip = NULL;
	::DeleteObject(hPenBypass); hPenBypass = NULL;
	::DeleteObject(hPenException); hPenException = NULL;
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectComponentList(HDC hDC, bool ShowRect, bool ShowName)//繪製專案零件
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	CString      str;
	int          Len=0;
	int          LenSizeX=0;
	size_t       i = 0;	
	COLORREF     Color=0;
	size_t       ComponentFinishCount=0;
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};		
	TPOINT2D     StagePos, StageOffset, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;	
	TPOINT2D     ImageRes = this->m_ImageResolution;
	TREGION4D    ImageStageRgn = this->m_ImageStageRgn;
	TREGION4D    ObjStageRgn, ObjImageRgn;
	REGION_CALC_STATE  RgnCalcState;
	RESULT_ID ResultID = RESULT_ID_NONE;
	COMPONENT_TYPE ComponentType;
	CAOIComponent *ComponentPtr = NULL;
	const LANE_ID      LaneID = GetShowLaneID();
	const bool ShowDefectOnly = GetShowComponentDefectOnly();
	const DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();	
	const bool  bDrawRoughLine = ProjectPtr->CheckProjectComponentUseDrawRoughLine();

	const DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();	
	const COLORREF  clr1 = SystemParam.m_ComponentColor1;
	const COLORREF  clr2 = SystemParam.m_ComponentColor2;
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrText = SystemParam.m_ComponentTextColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;
	const COLORREF  clrSelected=SystemParam.m_ComponentSelectedColor;
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, clr1);
	HPEN hPen2 = ::CreatePen(PS_SOLID, 1, clr2);	
	HPEN hPenOK = ::CreatePen(PS_SOLID, 1, clrOK);
	HPEN hPenNG = ::CreatePen(PS_SOLID, 1, clrNG);
	HPEN hPenSel = ::CreatePen(PS_SOLID, 3, clrSelected);
	HPEN hPenSkip = ::CreatePen(PS_SOLID, 1, clrSkip);
	HPEN hPenBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	HPEN hPenException = ::CreatePen(PS_SOLID, 1, clrException);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);			
	
	ComponentFinishCount = 0;
	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();		
	for ( i=0; i<ComponentCount; i++ )
	{	
		if ( ProjectPtr->CheckProjectLocking() == true )
		{	break; }
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
		ComponentPtr = ComponentPtr->GetComponentResultPtr();

		ComponentType = ComponentPtr->GetComponentType();
		RgnCalcState = ComponentPtr->GetRgnCalcState();
		ResultID = ComponentPtr->GetComponentResultID_AOI_Lane(LaneID);
		if ( RESULT_ID_NONE != ResultID )
		{	ComponentFinishCount ++;	}
		else
		{
			if ( ComponentPtr->CheckComponentModelEnabled() == false )
			{	ComponentFinishCount ++;	}
		}
		if ( AOIDataCollect.CheckComponentTypeVisible(ComponentType) == false )
		{	continue; }
		/*
		switch ( RgnCalcState )
		{		
		case REGION_CALC_DONE:
		case REGION_CALC_CLEAR:			
			ComponentFinishCount ++;
			break;
		}*/
		if ( false == ShowRect ) { continue; }
		if ( true == ShowDefectOnly )
		{
			bool bDraw=false;				
			switch ( ResultID )
			{			
			case RESULT_ID_NG:
			case RESULT_ID_EXCEPTION:
				bDraw=true;
				break;
			}
			if ( false == bDraw )
			{	continue; }
		}
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();
		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{
			StageOffset.x = ComponentPtr->GetComponentStageOffsetX();
			StageOffset.y = ComponentPtr->GetComponentStageOffsetY();
			StagePos.x += StageOffset.x;
			StagePos.y += StageOffset.y;
		}
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);
		
		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>m_WndRect.right || Pt.y>m_WndRect.bottom ) { continue; }
		
		ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);
		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{
			StgCornerPos[0].x += StageOffset.x;	StgCornerPos[0].y += StageOffset.y;
			StgCornerPos[1].x += StageOffset.x;	StgCornerPos[1].y += StageOffset.y;
			StgCornerPos[2].x += StageOffset.x;	StgCornerPos[2].y += StageOffset.y;
			StgCornerPos[3].x += StageOffset.x;	StgCornerPos[3].y += StageOffset.y;
		}
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, m_WndRect) == false )
		{	continue; }
		JetAPI::CornerPtToRect(CornerPos, Rect);
		if ( ComponentPtr->GetComponentSelected() == true ) 
		{
			::SelectObject(hDC, hPenSel);
			if ( true == bDrawRoughLine )
			{
				if ( DrawRectRoughLine(hDC, Rect, clrSelected) == true )
				{	continue; }
			}
			CImageWnd::DrawRectLine(hDC, CornerPos, 4);			
		}
		
		switch ( ResultID )
		{
		case RESULT_ID_NONE:	::SelectObject(hDC, hPen);	 Color = clr1;	break;
		case RESULT_ID_OK:		::SelectObject(hDC, hPenOK); Color = clrOK;	break;
		case RESULT_ID_NG:		::SelectObject(hDC, hPenNG); Color = clrNG;	break;
		case RESULT_ID_SKIP:	::SelectObject(hDC, hPenSkip); Color = clrSkip;	break;
		case RESULT_ID_BYPASS:	::SelectObject(hDC, hPenBypass); Color = clrBypassed;	break;
		case RESULT_ID_EXCEPTION:
			Color = clrException;
			::SelectObject(hDC, hPenException);
			break;
		default:
			Color = clr2;
			::SelectObject(hDC, hPen2);
			break;
		}
		if ( true == bDrawRoughLine )
		{
			if ( DrawRectRoughLine(hDC, Rect, Color) == true )		
			{	 continue; }
		}
		DrawRectLine(hDC, CornerPos, 4);
		if ( true==ShowName )
		{	
			str = ComponentPtr->GetComponentName();
			Len = str.GetLength();
			LenSizeX = Len*6;
			if ( LenSizeX < (Rect.right-Rect.left) )
			{
				//::TextOut(hDC, Rect.left, Rect.top, str, str.GetLength());
				::TextOut(hDC, Pt.x, Pt.y, str, Len);
			}
		}
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	
	::DeleteObject(hPen2); hPen2 = NULL;	
	::DeleteObject(hPenSel); hPenSel=NULL;
	::DeleteObject(hPenOK); hPenOK=NULL;
	::DeleteObject(hPenNG); hPenNG=NULL;
	::DeleteObject(hPenSkip); hPenSkip=NULL;
	::DeleteObject(hPenBypass); hPenBypass=NULL;
	::DeleteObject(hPenException); hPenException=NULL;
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);

	m_ComponentFinishCount = ComponentFinishCount;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectTempObjList(HDC hDC)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return; }
	if (ProjectPtr->CheckProjectLocking() == true) { return; }
	if (m_TempObjList.empty()) { return; }

	size_t       i = 0;
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	TPOINT2D     ImageRes = this->m_ImageResolution;
	TREGION4D    ImageStageRgn = this->m_ImageStageRgn;
	TPOINT2D     ImageStagePos;
	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();

	const DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_ComponentColor1;
	const COLORREF  clr2 = SystemParam.m_ComponentColor2;
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrText = SystemParam.m_ComponentTextColor;
	const COLORREF  clrSkip = SystemParam.m_InspectedResultSkipColor;
	const COLORREF  clrSelected = SystemParam.m_ComponentSelectedColor;
	const COLORREF  clrBypassed = SystemParam.m_InspectedResultBypassColor;
	const COLORREF  clrException = SystemParam.m_InspectedResultExceptionColor;

	HPEN hPen = ::CreatePen(PS_SOLID, 1, clr1);
	HPEN hPen2 = ::CreatePen(PS_SOLID, 1, clr2);
	HPEN hPenOK = ::CreatePen(PS_SOLID, 1, clrOK);
	HPEN hPenNG = ::CreatePen(PS_SOLID, 1, clrNG);
	HPEN hPenSel = ::CreatePen(PS_SOLID, 3, clrSelected);
	HPEN hPenSkip = ::CreatePen(PS_SOLID, 1, clrSkip);
	HPEN hPenBypass = ::CreatePen(PS_SOLID, 1, clrBypassed);
	HPEN hPenException = ::CreatePen(PS_SOLID, 1, clrException);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);
	TBOX_DRAW_PARAM TempObj;
	TRECT4D StageRect, ImageRect;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];
	POINT        Pt = { 0 }, CornerPos[4];
	for (i = 0; i < m_TempObjList.size(); i++) {
		TempObj = m_TempObjList.at(i);
		StageRect = TempObj.WndRect;
		AOIDataCollect.MapStageRectToCamera(ImageW, ImageH, ImageRes, StageRect, ImageStagePos, ImageRect);
		JetAPI::Rect4DToCornerPt(ImageRect, ImgCornerPos);
		//AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, TempObj.CornerPts, ImageStagePos, ImgCornerPos);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);
		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if (JetAPI::CheckCornerInRect(CornerPos, m_WndRect) == false) { continue; }
		if (TempObj.hPenNull != NULL) {
			::SelectObject(hDC, TempObj.hPenNull);
		}
		else {
			::SelectObject(hDC, hPen2);
		}
		CImageWnd::DrawRectLine(hDC, CornerPos, 4);
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	::DeleteObject(hPen2); hPen2 = NULL;
	::DeleteObject(hPenSel); hPenSel = NULL;
	::DeleteObject(hPenOK); hPenOK = NULL;
	::DeleteObject(hPenNG); hPenNG = NULL;
	::DeleteObject(hPenSkip); hPenSkip = NULL;
	::DeleteObject(hPenBypass); hPenBypass = NULL;
	::DeleteObject(hPenException); hPenException = NULL;
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::DrawProjectDistrictRect(HDC hDC)//繪製專案段落
{
	if ( NULL == hDC ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return true; }

	TPOINT2D     WndPt1;
	TPOINT2D     WndPt2;
	TPOINT2D     ImagePt1;
	TPOINT2D     ImagePt2;	
	IMAGE_SIZE   ImageW = m_ImageW;
	IMAGE_SIZE   ImageH = m_ImageH;
	RECT         Rect={0,0,0,0};	
	RECT         Rect2={0,0,0,0};	
	RECT         MapRectOff={0,0,0,0};
	RECT         MapRectOff2={0,0,0,0};
	const bool   bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
	const bool   ShowDistrictRect=GetShowDistrictRect();	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();	

	if ( false==bMultiDistrictMode || false==ShowDistrictRect )
	{	return false; }
	ProjectPtr->GetProjectMapLocRectOff(MapRectOff, MapRectOff2);

	ImagePt1.x = MapRectOff.left;
	ImagePt1.y = MapRectOff.top;
	ImagePt2.x = MapRectOff.right;
	ImagePt2.y = MapRectOff.bottom;
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt1, WndPt1);	
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt2, WndPt2);	
	Rect.left   = JetAPI::Floor(WndPt1.x);	
	Rect.top    = JetAPI::Floor(WndPt1.y);	
	Rect.right  = JetAPI::Floor(WndPt2.x);	
	Rect.bottom = JetAPI::Floor(WndPt2.y);
	
	ImagePt1.x = MapRectOff2.left;
	ImagePt1.y = MapRectOff2.top;
	ImagePt2.x = MapRectOff2.right;
	ImagePt2.y = MapRectOff2.bottom;
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt1, WndPt1);	
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt2, WndPt2);	
	Rect2.left   = JetAPI::Floor(WndPt1.x);	
	Rect2.top    = JetAPI::Floor(WndPt1.y);	
	Rect2.right  = JetAPI::Floor(WndPt2.x);	
	Rect2.bottom = JetAPI::Floor(WndPt2.y);
	
	HBRUSH hBrush = NULL;
	hBrush = ::CreateSolidBrush(0x000000);
	if ( NULL == hBrush ) 
	{	return false; }
	::FillRect(hDC, &m_WndRect, hBrush);
	::DeleteObject(hBrush); hBrush=NULL;	

	//hBrush = ::CreateSolidBrush(0xEAD999);
	//hBrush = ::CreateHatchBrush(HS_DIAGCROSS, 0xEAD999);
	//hBrush = ::CreateHatchBrush(HS_DIAGCROSS, 0xB0E4EF);
	hBrush = ::CreateHatchBrush(HS_DIAGCROSS, SystemParam.m_DistrictColor1);	
	if ( NULL == hBrush ) 
	{	return false; }
	::FillRect(hDC, &Rect, hBrush);
	::DeleteObject(hBrush); hBrush=NULL;

	//hBrush = ::CreateHatchBrush(HS_DIAGCROSS, 0x627BDF);
	hBrush = ::CreateHatchBrush(HS_DIAGCROSS, SystemParam.m_DistrictColor2);
	if ( NULL == hBrush ) 
	{	return false; }
	::FillRect(hDC, &Rect2, hBrush);
	::DeleteObject(hBrush); hBrush=NULL;	
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectNormal(HDC hDC)//繪製專案-一般模式
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	if ( true == m_ShowPanel )
	{	DrawProjectPanelList(hDC, true);	}

	if ( true == m_ShowBoard )
	{	DrawProjectBoardList(hDC, true);	}

	if ( true == m_ShowFiducial )
	{	DrawProjectFdList(hDC);	}	
	
	if ( true == m_ShowMark )
	{	DrawProjectMarkList(hDC, m_ShowComponentName);	}

	if ( true == m_ShowBarcode )
	{	DrawProjectBarcodeList(hDC, m_ShowComponentName);	}
	
	if ( true == m_ShowComponent )
	{	DrawProjectComponentList(hDC, true, m_ShowComponentName);	}	
	DrawProjectTempObjList(hDC);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawProjectInspecting(HDC hDC)//繪製專案-檢測模式
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	if ( ProjectPtr->CheckProjectLocking() == true ) { return ; }

	CString      str, str2;	
	CString      strLaneID;
	CString      strDistrictID;
	size_t       i = 0;
	size_t       FieldCount = 0;
	size_t       ComponentCount = 0;
	size_t       FieldFinishCount=0;			
	size_t       ComponentFinishCount=0;
	LANE_ID      UILaneID = GetShowLaneID();
	LANE_ID      ActLaneID = ProjectPtr->GetProjectActLaneID();
	DISTRICT_ID  DistrictID = ProjectPtr->GetProjectActDistrictID();	
	const bool   bMultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();		
	const ONLINE_STATE_MODE OnlineStateGUI = AOIDataCollect.GetOnlineStateMode_GUI_Lane(UILaneID);		
	const LANE_WORK_MODE LaneWorkMode_LA=AOIDataCollect.GetSystemParameter().m_LaneWorkMode_LA;
	const LANE_WORK_MODE LaneWorkMode_LB=AOIDataCollect.GetSystemParameter().m_LaneWorkMode_LB;	

	DrawProjectFdList(hDC);
	DrawProjectMarkList(hDC, true);
	DrawProjectBarcodeList(hDC, true);
	DrawProjectComponentList(hDC, true, false);
	DrawProjectFieldList(hDC, m_ShowFieldRgn);
	DrawProjectBoardList(hDC, true);//繪製專案單板
	
	FieldFinishCount = m_FieldFinishCount;
	ComponentFinishCount = m_ComponentFinishCount;

	LOGFONT LogFont;
	HFONT   hFont = NULL;	
	CString strRatio;
	CString strOnlineNode;
	CString strOnlinestate;
	const int FontSize = 32;	
	CString strGrab = AOIDataDefine.GetGrabText();
	CString strCalc = AOIDataDefine.GetCalculateText();	
	::memset(&LogFont, 0x00, sizeof(LogFont));
	LogFont.lfHeight = FontSize;	
	LogFont.lfWeight = FW_BOLD;
	LogFont.lfCharSet = DEFAULT_CHARSET;
	::_tcscpy(LogFont.lfFaceName, _T("Cambria"));//Calibri
	hFont = ::CreateFontIndirect(&LogFont);
	HFONT    hOldFont = (HFONT)::SelectObject(hDC, hFont);
	COLORREF TextColor = 0xFFFFFF;
	COLORREF OldTextClr = ::SetTextColor(hDC, TextColor);	
	const int BkMode = ::SetBkMode(hDC, TRANSPARENT);
	double ratioField = (double)(FieldFinishCount);
	double ratioComponent = (double)(ComponentFinishCount);

	POINT TextPt;
	TextPt.x = 4;
	TextPt.y = 4;

	if ( false == bMultiDistrictMode )
	{			
		FieldCount = ProjectPtr->CalcProjectFieldCountGrab(); 		
		ComponentCount = ProjectPtr->GetProjectComponentCount();	
	}
	else
	{	
		FieldCount = ProjectPtr->CalcProjectFieldCountGrab(DistrictID); 
		ComponentCount = ProjectPtr->GetProjectComponentCount(DistrictID);			
	}
	if ( FieldCount > 0 ) 
	{	ratioField = ratioField*100.0/FieldCount;	}
	else
	{	ratioField = 0.0; }	
	if ( ComponentCount > 0 ) 
	{	ratioComponent = ratioComponent*100.0/ComponentCount;	}
	else
	{	ratioComponent = 0.0; }

	strOnlinestate = AOIDataDefine.GetOnlineStateText(OnlineStateGUI);		
	if ( false == bMultiDistrictMode ) 
	{	strRatio.Format(_T("%s=%.2f%%(%d/%d), %s=%.2f%%"), strGrab, ratioField, FieldFinishCount, FieldCount, strCalc, ratioComponent); }
	else
	{
		strDistrictID = AOIDataDefine.GetDistrictIDText(DistrictID);
		strRatio.Format(_T("[%s] %s=%.2f%%(%d/%d), %s=%.2f%%"), strDistrictID, strGrab, ratioField, FieldFinishCount, FieldCount, strCalc, ratioComponent);	
	}

	if ( LANE_ID_BOTH != UILaneID )
	{	strOnlineNode = AOIDataCollect.GetOnlineStateNode(UILaneID);  }
	else
	{	strOnlineNode = AOIDataCollect.GetOnlineStateNode(ActLaneID);  }	
	if ( strOnlineNode.GetLength() > 0 )
	{
		str.Format(_T("%s(%s)"), strOnlinestate, strOnlineNode);
		strOnlinestate = str;
	}

	if ( LANE_ID_BOTH != UILaneID )
	{	strLaneID = AOIDataDefine.GetLaneIDText(UILaneID);  }
	else
	{	strLaneID = AOIDataDefine.GetLaneIDText(ActLaneID);  }
	
	str.Format(_T("%s-%s"), strOnlinestate, strLaneID);
	if ( ProjectPtr->GetProjectOnlineTuningEnable() )
	{	
		str += _T("-Tuning");	
		const int MaxCount=AOIDataCollect.GetOnlineTuningSavedMaxCount();		
		const int SavedCount=(int)(ProjectPtr->GetProjectOnlineTuningSavedCount());		
		if ( MaxCount > 0 )
		{
			str2.Format(_T("(%d/%d)"), SavedCount, MaxCount);
			str += str2;
		}
	}
	::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength());		
	TextPt.y += FontSize;

	if ( LANE_ID_BOTH==UILaneID || ActLaneID==UILaneID)
	{
		str.Format(_T("%s"), strRatio);
		::TextOut(hDC, TextPt.x, TextPt.y, str, str.GetLength());		
		TextPt.y += FontSize;
	}
	
	//顯示忽略安全檢知
	const bool   bSaftyBypass = PlcCtrlPtr->GetSaftyBypass();
	if ( true == bSaftyBypass )
	{
		::SetTextColor(hDC, 0x0000FF);
		str = AOIDataDefine.GetSaftyBypassText();
		::TextOut(hDC, TextPt.x, TextPt.y+FontSize, str, str.GetLength());
		TextPt.y += FontSize;
	}

	//顯示螢幕鎖住
	const bool bLockScreen = AOIDataCollect.GetLockScreenEnabled();
	if ( true == bLockScreen )
	{
		::SetTextColor(hDC, 0x0000FF);
		str = AOIDataDefine.GetLockScreenText();
		::TextOut(hDC, TextPt.x, TextPt.y+FontSize, str, str.GetLength());
		TextPt.y += FontSize;
	}

	//自動重測
	const int    AutoRetryCount = AOIDataCollect.GetAutoRetryCount();	
	if ( AutoRetryCount > 0 ) 
	{
		const int    AutoRetryMaxCount = AOIDataCollect.GetSystemParameter().m_AutoRetryMaxCount;
		::SetTextColor(hDC, 0xFFFFFF);
		str.Format(_T("Auto Retry %d/%d"), AutoRetryCount, AutoRetryMaxCount);
		::TextOut(hDC, TextPt.x, TextPt.y+FontSize, str, str.GetLength());
		TextPt.y += FontSize;
	}

	DEFECT_HANDLE_MODE DefectHandleMode = ProjectPtr->GetProjectParameter().m_DefectHandleMode;
	if ( LANE_WORK_RUN == LaneWorkMode_LA )
	{
		CString strWaitFor;
		CString strDateTime;
		char    DateTime[64]="";		
		LANE_ID LaneID = LANE_ID_A;		
		if ( LANE_ID_BOTH==UILaneID || LaneID==UILaneID )
		{
			CString strLane = AOIDataDefine.GetLaneIDText(LaneID);
			if ( DEFECT_HANDLE_WAIT_FOR_REPAIR == DefectHandleMode )
			{			
				if ( AOIDataCollect.GetRepairDateTimeToCheck(LaneID, DateTime) == true ) 
				{
					strDateTime = DateTime;
					strWaitFor = AOIDataDefine.GetWaitForRepairText();
					str.Format(_T("%s %s [%s]"), strLane, strWaitFor, strDateTime);
					::SetTextColor(hDC, 0xEF4F4F);
					::TextOut(hDC, TextPt.x, TextPt.y+FontSize, str, str.GetLength());
					TextPt.y += FontSize;
				}
			}
		
			if ( DEFECT_HANDLE_CONTROL_CENTER == DefectHandleMode )
			{
				if ( AOIDataCollect.GetCCSDateTimeToCheck(LaneID, DateTime) == true ) 
				{
					strDateTime = DateTime;
					strWaitFor = AOIDataDefine.GetWaitForCCSText();
					str.Format(_T("%s %s [%s]"), strLane, strWaitFor, strDateTime);
					::SetTextColor(hDC, 0x4F4FEF);
					::TextOut(hDC, TextPt.x, TextPt.y+FontSize, str, str.GetLength());
					TextPt.y += FontSize;
				}		
			}
		}
	}

	if ( LANE_WORK_RUN == LaneWorkMode_LB )
	{
		CString strWaitFor;
		CString strDateTime;
		char    DateTime[64]="";
		LANE_ID LaneID = LANE_ID_B;		
		if ( LANE_ID_BOTH==UILaneID || LaneID==UILaneID )
		{
			CString strLane = AOIDataDefine.GetLaneIDText(LaneID);
			if ( DEFECT_HANDLE_WAIT_FOR_REPAIR == DefectHandleMode )
			{
				if ( AOIDataCollect.GetRepairDateTimeToCheck(LaneID, DateTime) == true ) 
				{
					strDateTime = DateTime;
					strWaitFor = AOIDataDefine.GetWaitForRepairText();
					str.Format(_T("%s %s [%s]"), strLane, strWaitFor, strDateTime);
					::SetTextColor(hDC, 0xEF4F4F);
					::TextOut(hDC, TextPt.x, TextPt.y+FontSize, str, str.GetLength());
					TextPt.y += FontSize;
				}
			}
		
			if ( DEFECT_HANDLE_CONTROL_CENTER == DefectHandleMode )
			{
				if ( AOIDataCollect.GetCCSDateTimeToCheck(LaneID, DateTime) == true ) 
				{
					strDateTime = DateTime;
					strWaitFor = AOIDataDefine.GetWaitForCCSText();
					str.Format(_T("%s %s [%s]"), strLane, strWaitFor, strDateTime);
					::SetTextColor(hDC, 0x4F4FEF);
					::TextOut(hDC, TextPt.x, TextPt.y+FontSize, str, str.GetLength());
					TextPt.y += FontSize;
				}		
			}	
		}
	}	

	::SetBkMode(hDC, BkMode);
	::SetTextColor(hDC, OldTextClr);	
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hFont); hFont=NULL;	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawCameraPos(HDC hDC)//顯示相機中心位置
{
	if ( true == m_ShowCameraPos ) { return; }
	
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt={0};	
	RECT         Rect={0,0,0,0};
	TPOINT2D     WndPt;	
	TPOINT2D     ImagePt;
	TPOINT2D     CameraPt;
	TPOINT2D     ImageStagePos;
	TPOINT2D     ImageRes = this->m_ImageResolution;	
	double       CameraPosX=0;
	double       CameraPosY=0;
	double       CameraPosZ=0;
	const int    nSize=10;
	const bool   OfflineMode = AOIDataCollect.GetOfflineMode();		

	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();
	MotionCtrlPtr->GetCurrentPos(CameraPosX, CameraPosY, CameraPosZ, OfflineMode);
	CameraPt.x = CameraPosX;
	CameraPt.y = CameraPosY;
	AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, CameraPt, ImageStagePos, ImagePt);

	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt, WndPt);	
	JetAPI::Point2DToPoint(WndPt, Pt);	
	Rect.left = Pt.x-nSize-1;
	Rect.right = Pt.x+nSize;
	Rect.top = Pt.y-nSize-1;
	Rect.bottom = Pt.y+nSize;

	COLORREF Color = 0x0000FF;	
	HPEN hPen = ::CreatePen(PS_SOLID, 2, Color);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	//DrawPie(hDC, Rect);
	::MoveToEx(hDC, Rect.left, Pt.y, NULL);
	::LineTo(hDC, Rect.right, Pt.y);
	::MoveToEx(hDC, Pt.x, Rect.top, NULL);
	::LineTo(hDC, Pt.x, Rect.bottom);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;		
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawCameraRgn(HDC hDC)//繪製相機位置
{
	if ( false == m_ShowCameraRgn ) { return; }

	TREGION4D    CameraRgn, ImageRgn;	
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt1={0}, Pt2={0};
	TPOINT2D     ImageStagePos;
	TPOINT2D     WndPt1, WndPt2;
	TPOINT2D     ImagePt1, ImagePt2;
	TPOINT2D     ImageRes = this->m_ImageResolution;	
	double       CameraPosX=0;
	double       CameraPosY=0;
	double       CameraPosZ=0;
	const bool   OfflineMode = AOIDataCollect.GetOfflineMode();	
	const double FovSizeW = AOIDataCollect.GetFovSizeRealW();
	const double FovSizeH = AOIDataCollect.GetFovSizeRealH();

	ImageStagePos.x = m_ImageStageRgn.GetCpX();
	ImageStagePos.y = m_ImageStageRgn.GetCpY();
	MotionCtrlPtr->GetCurrentPos(CameraPosX, CameraPosY, CameraPosZ, OfflineMode);
	CameraRgn.minX = CameraPosX-(FovSizeW*0.5);
	CameraRgn.maxX = CameraRgn.minX+(FovSizeW);
	CameraRgn.minY = CameraPosY-(FovSizeH*0.5);
	CameraRgn.maxY = CameraRgn.minY+(FovSizeH);	
	AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, CameraRgn, ImageStagePos, ImageRgn);

	ImagePt1.x = ImageRgn.minX;
	ImagePt1.y = ImageRgn.minY;
	ImagePt2.x = ImageRgn.maxX;
	ImagePt2.y = ImageRgn.maxY;
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt1, WndPt1);
	MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt2, WndPt2);	

	JetAPI::Point2DToPoint(WndPt1, Pt1);
	JetAPI::Point2DToPoint(WndPt2, Pt2);
	
	HPEN hPen = ::CreatePen(PS_SOLID, 2, m_clrCameraRgn);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	::MoveToEx(hDC, Pt1.x, Pt1.y, NULL);
	::LineTo(hDC, Pt2.x, Pt1.y);
	::LineTo(hDC, Pt2.x, Pt2.y);
	::LineTo(hDC, Pt1.x, Pt2.y);
	::LineTo(hDC, Pt1.x, Pt1.y);		
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	

	if ( IMAGE_WND_DRAW_PROJECT_INSPECTING==m_DrawProjectMode )
	{
		POINT PtC, PtMin, PtMax;
		PtC.x = (Pt1.x+Pt2.x)/2;
		PtC.y = (Pt1.y+Pt2.y)/2;
		PtMin.x = MIN(Pt1.x, Pt2.x);
		PtMin.y = MIN(Pt1.y, Pt2.y);
		PtMax.x = MAX(Pt1.x, Pt2.x);
		PtMax.y = MAX(Pt1.y, Pt2.y);
		hPen = ::CreatePen(PS_DOT, 1, m_clrCursorLine);
		hOldPen = (HPEN)(::SelectObject(hDC, hPen));
		::MoveToEx(hDC, m_WndRect.left, PtC.y, NULL);
		::LineTo(hDC, PtMin.x, PtC.y);
		::MoveToEx(hDC, m_WndRect.right, PtC.y, NULL);
		::LineTo(hDC, PtMax.x, PtC.y);

		::MoveToEx(hDC, PtC.x, m_WndRect.top, NULL);
		::LineTo(hDC, PtC.x, PtMin.y);
		::MoveToEx(hDC, PtC.x, m_WndRect.bottom, NULL);
		::LineTo(hDC, PtC.x, PtMax.y);
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen = NULL;
	}
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawImageCenterLine(HDC hDC)//繪製影像中心線
{
	if ( false == m_ShowImgCenterLine ) { return; }

	TREGION4D    CameraRgn, ImageRgn;	
	CAMERA_ID    CameraID = this->m_CameraID;	
	IMAGE_SIZE   ImageW = this->m_ImageW;
	IMAGE_SIZE   ImageH = this->m_ImageH;
	POINT        Pt={0};	
	RECT         Rect={0};	
	TPOINT2D     WndPt1, WndPt2;
	TPOINT2D     ImagePt1, ImagePt2;
	
	ImagePt1.x = 0;
	ImagePt1.y = 0;
	ImagePt2.x = ImageW;
	ImagePt2.y = ImageH;
	this->MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt1, WndPt1);
	this->MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt2, WndPt2);
	Pt.x = JetAPI::Floor((WndPt1.x+WndPt2.x)*0.5);
	Pt.y = JetAPI::Floor((WndPt1.y+WndPt2.y)*0.5);
	Rect.left = JetAPI::Floor(WndPt1.x);
	Rect.top = JetAPI::Floor(WndPt1.y);
	Rect.right = JetAPI::Floor(WndPt2.x);
	Rect.bottom = JetAPI::Floor(WndPt2.y);		
	
	HPEN hPen = ::CreatePen(PS_DOT, 1, m_clrImgCenterLine);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	::MoveToEx(hDC, Rect.left, Pt.y, NULL);
	::LineTo(hDC, Rect.right, Pt.y);
	::MoveToEx(hDC, Pt.x, Rect.top, NULL);
	::LineTo(hDC, Pt.x, Rect.bottom);	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( ::PtInRect(&m_WndRect, pt) == FALSE )
	{	
		CStatic::OnLButtonDown(nFlags, point);
		return;
	}

	this->m_LBtnUpPos = point;
	this->m_LBtnDownPos = this->m_MovingPos = this->m_LBtnUpPos;	
	this->m_WndLBtnPt2 = this->m_WndLBtnPt1 = pt;

	CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, m_WndLBtnPt1, m_ImageEditPt1);
	m_ImageEditLast = m_ImageEditPt2 = m_ImageEditPt1;
	if ( IMAGE_LBTN_CLICK_NULL != m_LBtnClickMode )
	{
		if ( EDIT_GRID_NULL == m_EditRectMode )
		{
			if ( true==m_ShowLBtnPos )
			{
				m_ImageEditRect.left = m_ImageEditRect.right = m_ImageEditPt1.x;
				m_ImageEditRect.top = m_ImageEditRect.bottom = m_ImageEditPt1.y;
				this->CalcImageWndLBtn();
			}
		}
		else
		{	m_ShowEditGridLine = false;	}
	}
	else
	{	m_ShowEditGridLine = false;	}
	m_ShowEditGridLine = false;//放開後才會繪製編輯點
	this->RedrawWnd(FALSE);
	
	this->SetCapture();
	this->SetFocus();	
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_LBUTTON_DOWN, NULL);
	CStatic::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();	
	POINT pt = point;		
	this->m_LBtnUpPos = this->m_MovingPos = point;		
	if ( ::PtInRect(&m_WndRect, pt) == TRUE )
	{	
		POINT WndDP, ImgDP;
		this->m_WndLBtnPt2 = pt;		
		CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, m_WndLBtnPt2, m_ImageEditPt2);	
		WndDP.x = ::abs(m_MovingPos.x-m_LBtnDownPos.x);
		WndDP.y = ::abs(m_MovingPos.y-m_LBtnDownPos.y);		
		ImgDP.x = ::abs(m_ImageEditPt2.x-m_ImageEditPt1.x);
		ImgDP.y = ::abs(m_ImageEditPt2.y-m_ImageEditPt1.y);		
		CalcImageWndLBtn();
		if ( IMAGE_LBTN_CLICK_MEASURE == m_LBtnClickMode )
		{
			CString str;
			const double GapX=ImgDP.x*m_ImageResolution.x;
			const double GapY=ImgDP.y*m_ImageResolution.y;
			str.Format(_T("Wnd (%d, %d) pixel\nImage (%d, %d) pixel\nStage (%.0f, %.0f) um"), WndDP.x, WndDP.y, ImgDP.x, ImgDP.y, GapX, GapY);			
			JetAPI::ShowMessageBox(str);
		}
		else if ( IMAGE_LBTN_CLICK_EDIT_BOX == m_LBtnClickMode )
		{
			if ( EDIT_GRID_NULL == m_EditRectMode )
			{	
				if ( WndDP.x<=2 || WndDP.y<=2 )
				{	
					m_ImageWndEditPt1.x = m_ImageWndEditPt1.y = -1;
					m_ImageWndEditPt2.x = m_ImageWndEditPt2.y = -1;

					m_ImageEditPt1.x = m_ImageEditPt1.y = -1;
					m_ImageEditPt2.x = m_ImageEditPt2.y = -1;
					m_ImageEditRect.left = m_ImageEditRect.right = -1;
					m_ImageEditRect.top = m_ImageEditRect.bottom = -1;
					this->m_EditGridList.clear();
				}
				else
				{					
					const double ComponentAngle = GetImageEditRectAngle();
					const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
					if (true == IsExceptionAngle) {
						if ((m_ImageEditPt1.x > m_ImageEditPt2.x) ^ (m_ImageEditPt1.y > m_ImageEditPt2.y)) {
							m_ImageEditRectRotated = true;
						}
						else {
							m_ImageEditRectRotated = false;
						}
					}
					m_ImageEditRect.left = MIN(m_ImageEditPt1.x, m_ImageEditPt2.x);
					m_ImageEditRect.right = MAX(m_ImageEditPt1.x, m_ImageEditPt2.x);
					m_ImageEditRect.top = MIN(m_ImageEditPt1.y, m_ImageEditPt2.y);
					m_ImageEditRect.bottom = MAX(m_ImageEditPt1.y, m_ImageEditPt2.y);
					this->CalcImageWndLBtn();
				}
			}
		}
		else if ( IMAGE_LBTN_CLICK_SELECT_COMPONENT == m_LBtnClickMode )
		{	
			m_ImageEditRect.left   = MIN(m_ImageEditPt1.x, m_ImageEditPt2.x);
			m_ImageEditRect.right  = MAX(m_ImageEditPt1.x, m_ImageEditPt2.x);
			m_ImageEditRect.top    = MIN(m_ImageEditPt1.y, m_ImageEditPt2.y);
			m_ImageEditRect.bottom = MAX(m_ImageEditPt1.y, m_ImageEditPt2.y);
			CalcImageWndLBtn();
			
			TRECT4D WndRect=m_ImageWndEditRect;
			ExecSelectComponent(WndRect);
		}
		else
		{	ExecEditRect();	}		
	}	
	UpdateShowEditLine();
	this->RedrawWnd(FALSE);
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_LBUTTON_UP, NULL);
	CStatic::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt = point;
	if ( ::PtInRect(&m_WndRect, pt) == FALSE )
	{	
		CStatic::OnRButtonDown(nFlags, point);
		return;
	}

	this->m_RBtnUpPos = point;
	this->m_RBtnDownPos = this->m_MovingPos = this->m_RBtnUpPos;
	this->m_WndRBtnPt2 = this->m_WndRBtnPt1 = point;
	this->SetCapture();
	this->SetFocus();
	this->RedrawWnd(FALSE);
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_RBUTTON_DOWN, NULL);
	CStatic::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	::ReleaseCapture();
	POINT dp;
	POINT pt = point;
	if ( ::PtInRect(&m_WndRect, pt) == TRUE )		
	{	this->m_WndRBtnPt2 = pt;	}

	this->m_RBtnUpPos = this->m_MovingPos = point;	
	dp.x = m_RBtnUpPos.x-m_RBtnDownPos.x;
	dp.y = m_RBtnUpPos.y-m_RBtnDownPos.y;

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{	
		if ( abs(dp.x)<2 && abs(dp.y)<2 )
		{			
			SwitchProjectMap();			
			CStatic::OnRButtonUp(nFlags, point);
			return;
		}
	}

	if ( IMAGE_RBTN_UP_MOVE_STAGE == m_RBtnUpMode )
	{	
		if ( CheckOutOfImageRgn()==true )
		{
			POINT WndCp;
			WndCp.x = (m_WndRect.left+m_WndRect.right)/2;
			WndCp.y = (m_WndRect.top+m_WndRect.bottom)/2;
			CImageWnd::ExecStageMoveTo(WndCp); 
		}
	}
	this->RedrawWnd(FALSE);
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_RBUTTON_UP, NULL);
	CStatic::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( true == m_ShowCursorInfo )
	{	ShowCursorInfo(point);	}		

	if ( this != GetCapture() ) 
	{
		this->UpdateCursor(point);
		if ( true==m_ShowCursorLine || m_strCursorInfo.GetLength() > 0 ) 
		{	RedrawWnd(FALSE); }
		m_MovingPos = point;
		CStatic::OnMouseMove(nFlags, point);
		return; 
	}	

	BOOL  bRedraw = FALSE;
	POINT pt  = point;

	if ( true == m_ShowCursorLine )
	{	bRedraw = TRUE; }

	if ( nFlags&MK_LBUTTON )
	{
		if ( ::PtInRect(&m_WndRect, pt) == TRUE )		
		{	
			switch ( m_LBtnClickMode )
			{
			case IMAGE_LBTN_CLICK_EDIT_BOX:				
				bRedraw = TRUE;
				this->m_WndLBtnPt2 = point;
				CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, m_WndLBtnPt2, m_ImageEditPt2);	
				if ( EDIT_GRID_NULL == m_EditRectMode )
				{				
					const double ComponentAngle = GetImageEditRectAngle();
					const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
					if (true == IsExceptionAngle) {
						if ((m_ImageEditPt1.x > m_ImageEditPt2.x) ^ (m_ImageEditPt1.y > m_ImageEditPt2.y)) {
							m_ImageEditRectRotated = true;
						}
						else {
							m_ImageEditRectRotated = false;
						}
					}
					m_ImageEditRect.left = MIN(m_ImageEditPt1.x, m_ImageEditPt2.x);
					m_ImageEditRect.right = MAX(m_ImageEditPt1.x, m_ImageEditPt2.x);
					m_ImageEditRect.top = MIN(m_ImageEditPt1.y, m_ImageEditPt2.y);
					m_ImageEditRect.bottom = MAX(m_ImageEditPt1.y, m_ImageEditPt2.y);
					this->CalcImageWndLBtn();
				}
				else
				{	ExecEditRect();	}
				break;			
			case IMAGE_LBTN_CLICK_MOVE_PANEL:
				bRedraw = TRUE;
				pt.x = point.x-m_MovingPos.x;
				pt.y = point.y-m_MovingPos.y;
				ExecMovePanel(pt);
				break;
			case IMAGE_LBTN_CLICK_MOVE_PROJECT:
				bRedraw = TRUE;
				pt.x = point.x-m_MovingPos.x;
				pt.y = point.y-m_MovingPos.y;
				ExecMoveProject(pt);
				break;
			case IMAGE_LBTN_CLICK_MOVE_PANEL_STAGE:
				bRedraw = TRUE;
				pt.x = point.x-m_MovingPos.x;
				pt.y = point.y-m_MovingPos.y;
				ExecMovePanelStage(pt);
				break;
			case IMAGE_LBTN_CLICK_MOVE_PROJECT_STAGE:
				bRedraw = TRUE;
				pt.x = point.x-m_MovingPos.x;
				pt.y = point.y-m_MovingPos.y;
				ExecMoveProjectStage(pt);
				break;
			case IMAGE_LBTN_CLICK_MEASURE:
			case IMAGE_LBTN_CLICK_SELECT_COMPONENT:
				bRedraw = TRUE;
				this->m_WndLBtnPt2 = point;
				CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, m_WndLBtnPt2, m_ImageEditPt2);									
				m_ImageEditRect.left   = MIN(m_ImageEditPt1.x, m_ImageEditPt2.x);
				m_ImageEditRect.right  = MAX(m_ImageEditPt1.x, m_ImageEditPt2.x);
				m_ImageEditRect.top    = MIN(m_ImageEditPt1.y, m_ImageEditPt2.y);
				m_ImageEditRect.bottom = MAX(m_ImageEditPt1.y, m_ImageEditPt2.y);	
				CalcImageWndLBtn();
				break;
			}
		}
		if ( true == m_ShowLBtnPos )
		{	
			bRedraw = TRUE; 
			if ( IMAGE_LBTN_CLICK_EDIT_BOX != m_LBtnClickMode )
			{
				this->m_WndLBtnPt2 = point;			
				CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, m_WndLBtnPt2, m_ImageEditPt2);	
				m_ImageEditRect.left   = MIN(m_ImageEditPt1.x, m_ImageEditPt2.x);
				m_ImageEditRect.right  = MAX(m_ImageEditPt1.x, m_ImageEditPt2.x);
				m_ImageEditRect.top    = MIN(m_ImageEditPt1.y, m_ImageEditPt2.y);
				m_ImageEditRect.bottom = MAX(m_ImageEditPt1.y, m_ImageEditPt2.y);
				CalcImageWndLBtn();
			}			
		}
	}
	else if ( nFlags&MK_RBUTTON )
	{
		switch ( m_RBtnClickMode )
		{
		case IMAGE_RBTN_CLICK_CTRL_VIEW:
			bRedraw = TRUE;
			m_ImageOffset.x += point.x-m_MovingPos.x;
			m_ImageOffset.y += point.y-m_MovingPos.y;			
			CalcImageWndLBtn();
			DrawImage();
			break;
		case IMAGE_RBTN_CLICK_MOVE_PROJECT:			
			bRedraw = TRUE;
			pt.x = point.x-m_MovingPos.x;
			pt.y = point.y-m_MovingPos.y;
			ExecMoveProject(pt);			
			break;
		case IMAGE_RBTN_CLICK_MOVE_PANEL:			
			bRedraw = TRUE;
			pt.x = point.x-m_MovingPos.x;
			pt.y = point.y-m_MovingPos.y;
			ExecMovePanel(pt);			
			break;
		}		
	}

	if ( TRUE == bRedraw )
	{	RedrawWnd(FALSE); }

	m_MovingPos = point;
	SendParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_MOUSE_MOVE, NULL);
	CStatic::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CImageWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if ( IMAGE_RBTN_CLICK_NULL != m_RBtnClickMode )
	{
		double NextImageZoom = m_ImageZoom;
		if ( zDelta > 0 ) 
		{	NextImageZoom *= 1.1;	}
		else
		{	NextImageZoom /= 1.1;	}
		double ZoomMin = AOIDataCollect.GetImageZoomMin();
		double ZoomMax = AOIDataCollect.GetImageZoomMax();
		if ( IMAGE_DATA_MAP == m_ImageDataSrc )
		{
			ZoomMin = 0.0001;
			ZoomMax = 1000.0;
		}
		if ( NextImageZoom>ZoomMin && NextImageZoom<ZoomMax )
		{
			CImageWnd::CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
			m_ImageZoom = NextImageZoom;
		}
		POINT point = pt;
		CWnd::ScreenToClient(&point);
		ShowCursorInfo(point);
		CalcImageWndLBtn();
		DrawImage();
		RedrawWnd(FALSE);
	}
	SendParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_MOUSE_WHEEL, NULL);
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::DrawImage()
{
	HDC hDC = m_MemDC1.GetSafeHdc();	
	if ( NULL==hDC ) { return; }	
	COLORREF clrBK = m_BkColor;//0xAFFFFF;
	//if ( NULL == m_ImagePtr )
	{
		HBRUSH hBrush = NULL;
		if ( HS_API_MAX == m_HatchType )
		{	hBrush = ::CreateSolidBrush(clrBK);	}
		else
		{	hBrush = ::CreateHatchBrush(m_HatchType, m_HatchColor);	}		
		if ( NULL != hBrush )
		{
			::FillRect(hDC, &m_WndRect, hBrush);
			::DeleteObject(hBrush);
		}
	}
	//else
	{	ImageAPI.DrawImageToDC(hDC, m_ImageW, m_ImageH, m_ImageStep, m_BitCount, m_ImagePtr, m_WndRect, m_ImageOffset, m_ImageZoom, clrBK); }
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	POINT dp;
	dp.x = ::abs(m_RBtnUpPos.x-m_RBtnDownPos.x);
	dp.y = ::abs(m_RBtnUpPos.y-m_RBtnDownPos.y);
	if ( dp.x>2 || dp.y>2 )
	{	return; }

	if ( IMAGE_RBTN_DBCLICK_POPUP_MENU == m_RBtnDbClickMode )
	{
		CMenu Menu;		
		UINT  MenuID = IDR_IMAGE_POPUP_MENU;	
		if ( MenuID == 0 ) { return ; }
		VERIFY(Menu.LoadMenu(MenuID));
		AOIDataCollect.SwitchMultiLanguageMenu(Menu, MenuID);

		CMenu* pPopup = Menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);

		CWnd* pWndPopupOwner = this;
		while (pWndPopupOwner->GetStyle() & WS_CHILD)
		{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	else
	{	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_CONTEXT_MENU, NULL);	}
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnMoveTo() 
{
	// TODO: Add your command handler code here
	CImageWnd::ExecStageMoveTo(m_RBtnDownPos);	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//會被蹦跳選單吃掉
	switch ( m_RBtnDbClickMode )
	{
	case IMAGE_RBTN_DBCLICK_FIT_ZOOM:
		CImageWnd::ShowFittedZoom();
		break;
	}	
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_RBUTTON_DBCLICK, NULL);
	CStatic::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	switch ( m_LBtnDbClickMode )
	{
	case IMAGE_LBTN_DBCLICK_FIT_ZOOM:
		CImageWnd::ShowFittedZoom();
		break;
	case IMAGE_LBTN_DBCLICK_STAGE_MOVE_TO:
		CImageWnd::ExecStageMoveTo(point);
		break;
	}
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_RBUTTON_DBCLICK, NULL);
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_LBUTTON_DBCLICK, NULL);
	CStatic::OnLButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::CalcImageWndLBtn_ExpectAngle()
{
	TPOINT2D dImgPt1, dImgPt2;
	TPOINT2D dWndPt1, dWndPt2;

	dImgPt1.x = m_ImageEditRect.left;
	dImgPt1.y = m_ImageEditRect.top;
	dImgPt2.x = m_ImageEditRect.right;
	dImgPt2.y = m_ImageEditRect.bottom;

	m_EditGridList.clear();
	MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, dImgPt1, dWndPt1);
	MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, dImgPt2, dWndPt2);
	JetAPI::Point2DToPoint(dWndPt1, m_ImageWndEditPt1);
	JetAPI::Point2DToPoint(dWndPt2, m_ImageWndEditPt2);
	m_ImageWndEditRect.left   = MIN(m_ImageWndEditPt1.x, m_ImageWndEditPt2.x);
	m_ImageWndEditRect.right  = MAX(m_ImageWndEditPt1.x, m_ImageWndEditPt2.x);
	m_ImageWndEditRect.top    = MIN(m_ImageWndEditPt1.y, m_ImageWndEditPt2.y);
	m_ImageWndEditRect.bottom = MAX(m_ImageWndEditPt1.y, m_ImageWndEditPt2.y);

	if ( IMAGE_LBTN_CLICK_EDIT_BOX == m_LBtnClickMode )
	{
		TEditGrid EditGrid;
		const int GridW = m_EditGridW;
		const int GridH = m_EditGridH;
		POINT CP={0};
		RECT  Rect={0};
		RECT  RectMax = this->m_ImageWndEditRect;
		RECT  RectMin = this->m_ImageWndEditRect;
		CP.x = (m_ImageWndEditRect.left+m_ImageWndEditRect.right)/2;
		CP.y = (m_ImageWndEditRect.top+m_ImageWndEditRect.bottom)/2;

		::InflateRect(&RectMax, GridW, GridH);
		::InflateRect(&RectMin, -GridW, -GridH);

		EditGrid.sEditMode = EDIT_GRID_OUTSIDE;
		EditGrid.sEditGrid = RectMax;
		m_EditGridList.push_back(EditGrid);

		EditGrid.sEditMode = EDIT_GRID_INSIDE;
		EditGrid.sEditGrid = RectMin;
		m_EditGridList.push_back(EditGrid);
		
		//C-Left
		Rect.left   = RectMax.left;
		Rect.right  = RectMin.left;
		Rect.top    = CP.y-GridH;
		Rect.bottom = CP.y+GridH;
		EditGrid.sEditMode = EDIT_GRID_CENTER_LEFT;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);
		
		//C-Right
		Rect.left   = RectMin.right;
		Rect.right  = RectMax.right;
		Rect.top    = CP.y-GridH;
		Rect.bottom = CP.y+GridH;
		EditGrid.sEditMode = EDIT_GRID_CENTER_RIGH;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);		

		//C-Top
		Rect.top    = RectMax.top;
		Rect.bottom = RectMin.top;
		Rect.left   = CP.x-GridW;
		Rect.right  = CP.x+GridW;
		EditGrid.sEditMode = EDIT_GRID_CENTER_TOP;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);		
		
		//Bottom
		Rect.top    = RectMin.bottom;
		Rect.bottom = RectMax.bottom;
		Rect.left   = CP.x-GridW;
		Rect.right  = CP.x+GridW;
		EditGrid.sEditMode = EDIT_GRID_CENTER_BOTTOM;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);		


		//left
		Rect.left   = RectMax.left;
		Rect.right  = RectMin.left;
		Rect.top    = RectMin.top;
		Rect.bottom = RectMin.bottom;
		EditGrid.sEditMode = EDIT_GRID_LEFT;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);				

		//right
		Rect.left   = RectMin.right;
		Rect.right  = RectMax.right;
		Rect.top    = RectMin.top;
		Rect.bottom = RectMin.bottom;
		EditGrid.sEditMode = EDIT_GRID_RIGHT;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);		

		//Top
		Rect.top    = RectMax.top;
		Rect.bottom = RectMin.top;
		Rect.left   = RectMin.left;
		Rect.right  = RectMin.right;	
		EditGrid.sEditMode = EDIT_GRID_TOP;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);

		//Bottom
		Rect.top    = RectMin.bottom;
		Rect.bottom = RectMax.bottom;
		Rect.left   = RectMin.left;
		Rect.right  = RectMin.right;	
		EditGrid.sEditMode = EDIT_GRID_BOTTOM;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);		

		//left-Top
		Rect.left   = RectMax.left;
		Rect.right  = RectMin.left;
		Rect.top    = RectMax.top;
		Rect.bottom = RectMin.top;
		EditGrid.sEditMode = EDIT_GRID_LEFT_TOP;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);

		//right-Top
		Rect.left   = RectMin.right;
		Rect.right  = RectMax.right;
		Rect.top    = RectMax.top;
		Rect.bottom = RectMin.top;
		EditGrid.sEditMode = EDIT_GRID_RIGHT_TOP;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);		

		//left-Bottom
		Rect.left   = RectMax.left;
		Rect.right  = RectMin.left;
		Rect.top    = RectMin.bottom;
		Rect.bottom = RectMax.bottom;
		EditGrid.sEditMode = EDIT_GRID_LEFT_BOTTOM;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);

		//right-Bottom
		Rect.left   = RectMin.right;
		Rect.right  = RectMax.right;
		Rect.top    = RectMin.bottom;
		Rect.bottom = RectMax.bottom;
		EditGrid.sEditMode = EDIT_GRID_RIGHT_BOTTOM;
		EditGrid.sEditGrid = Rect;
		m_EditGridList.push_back(EditGrid);
	}	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::CalcImageWndLBtn()
{
	if (m_ImageEditRect.GetWidth() == 0 || m_ImageEditRect.GetHeight() == 0) { 
		m_ShowEditGridLine = false;
	}
	//m_ImageEditRectAngle
	const double ComponentAngle = GetImageEditRectAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	double ImageAngle = JetAPI::MapCadAngleToImageAngle(ComponentAngle);

	if (false == IsExceptionAngle) {
		CalcImageWndLBtn_ExpectAngle();
		return;
	}
	//#define EDITRECT_DEBUG
#ifdef EDITRECT_DEBUG
	CalcImageWndLBtn_old();
#else
	m_EditGridList.clear();
#endif // EDITRECT_DEBUG
	TPOINT2D dImgPt1, dImgPt2;
	TPOINT2D dWndPt1, dWndPt2;

	if (true == m_ImageEditRectRotated) {
		dImgPt1.x = m_ImageEditRect.left;
		dImgPt1.y = m_ImageEditRect.bottom;
		dImgPt2.x = m_ImageEditRect.right;
		dImgPt2.y = m_ImageEditRect.top;
	}
	else {
		dImgPt1.x = m_ImageEditRect.right;
		dImgPt1.y = m_ImageEditRect.bottom;
		dImgPt2.x = m_ImageEditRect.left;
		dImgPt2.y = m_ImageEditRect.top;
	}


	m_EditGridList.clear();
	MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, dImgPt1, dWndPt1);
	MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, dImgPt2, dWndPt2);
	JetAPI::Point2DToPoint(dWndPt1, m_ImageWndEditPt1);
	JetAPI::Point2DToPoint(dWndPt2, m_ImageWndEditPt2);
	m_ImageWndEditRect.left = MIN(m_ImageWndEditPt1.x, m_ImageWndEditPt2.x);
	m_ImageWndEditRect.right = MAX(m_ImageWndEditPt1.x, m_ImageWndEditPt2.x);
	m_ImageWndEditRect.top = MIN(m_ImageWndEditPt1.y, m_ImageWndEditPt2.y);
	m_ImageWndEditRect.bottom = MAX(m_ImageWndEditPt1.y, m_ImageWndEditPt2.y);

	if (IMAGE_LBTN_CLICK_EDIT_BOX == m_LBtnClickMode)
	{

		double CP_x = (dWndPt1.x + dWndPt2.x)*0.5;
		double CP_y = (dWndPt1.y + dWndPt2.y)*0.5;

		JetAPI::RotatePos(-ImageAngle, CP_x, CP_y, dWndPt1);
		JetAPI::RotatePos(-ImageAngle, CP_x, CP_y, dWndPt2);

		double dx = fabs(dWndPt1.x - CP_x);
		double dy = fabs(dWndPt1.y - CP_y);

		TPOINT2D EditPoint[8];
		EditPoint[0] = { CP_x - dx, CP_y };        // CENTER_LEFT
		EditPoint[1] = { CP_x + dx, CP_y };        // CENTER_RIGHT
		EditPoint[2] = { CP_x,      CP_y - dy };   // CENTER_TOP
		EditPoint[3] = { CP_x,      CP_y + dy };   // CENTER_BOTTOM
		EditPoint[4] = { CP_x - dx, CP_y - dy };   // LEFT_TOP
		EditPoint[5] = { CP_x + dx, CP_y - dy };   // RIGHT_TOP
		EditPoint[6] = { CP_x - dx, CP_y + dy };   // LEFT_BOTTOM
		EditPoint[7] = { CP_x + dx, CP_y + dy };   // RIGHT_BOTTOM

		JetAPI::RotateCornerPos(ImageAngle, CP_x, CP_y, EditPoint);
		JetAPI::RotateCornerPos(ImageAngle, CP_x, CP_y, EditPoint + 4);

		m_ImageWndEditRect.left = CP_x;
		m_ImageWndEditRect.right = CP_x;
		m_ImageWndEditRect.top = CP_y;
		m_ImageWndEditRect.bottom = CP_y;

		for (int i = 0; i < 4; i++) {
			TPOINT2D EditPointTemp = EditPoint[i];
			POINT ImageWndEditPt;
			JetAPI::Point2DToPoint(EditPointTemp, ImageWndEditPt);
			m_ImageWndEditRect.left = MIN(m_ImageWndEditRect.left, ImageWndEditPt.x);
			m_ImageWndEditRect.right = MAX(m_ImageWndEditRect.right, ImageWndEditPt.x);
			m_ImageWndEditRect.top = MIN(m_ImageWndEditRect.top, ImageWndEditPt.y);
			m_ImageWndEditRect.bottom = MAX(m_ImageWndEditRect.bottom, ImageWndEditPt.y);
		}

		TEditGrid EditGrid;
		const int GridW = m_EditGridW;
		const int GridH = m_EditGridH;

		RECT  Rect = { 0 };
		POINT CP = { 0 };
		RECT  RectMax, RectMin;
		{
			RectMax.left = MIN(EditPoint[4].x, EditPoint[6].x) - GridW;
			RectMax.right = MAX(EditPoint[5].x, EditPoint[7].x) + GridW;
			RectMax.top = MIN(EditPoint[4].y, EditPoint[5].y) - GridH;
			RectMax.bottom = MAX(EditPoint[6].y, EditPoint[7].y) + GridH;
			RectMin.left = MAX(EditPoint[4].x, EditPoint[6].x) + GridW;
			RectMin.right = MIN(EditPoint[5].x, EditPoint[7].x) - GridW;
			RectMin.top = MAX(EditPoint[4].y, EditPoint[5].y) + GridH;
			RectMin.bottom = MIN(EditPoint[6].y, EditPoint[7].y) - GridH;
		}

		CP.x = (m_ImageWndEditRect.left + m_ImageWndEditRect.right) / 2;
		CP.y = (m_ImageWndEditRect.top + m_ImageWndEditRect.bottom) / 2;

		EditGrid.sEditMode = EDIT_GRID_OUTSIDE;
		EditGrid.sEditGrid = RectMax;
		m_EditGridList.push_back(EditGrid);

		EditGrid.sEditMode = EDIT_GRID_INSIDE;
		EditGrid.sEditGrid = RectMin;
		m_EditGridList.push_back(EditGrid);

		for (int i = 0; i < 8; i++) {
			Rect.left = EditPoint[i].x - GridW;
			Rect.right = EditPoint[i].x + GridW;
			Rect.top = EditPoint[i].y - GridH;
			Rect.bottom = EditPoint[i].y + GridH;
			EditGrid.sEditMode = (EDIT_GRID_MODE)(i + 6);
			EditGrid.sEditGrid = Rect;
			m_EditGridList.push_back(EditGrid);
		}
		for (int i = 0; i < 4; i++) {
			TPOINT2D StartPoint, EndPoint, TempPoint;
			switch (EDIT_GRID_MODE(i + 2))
			{
			case EDIT_GRID_LEFT:
				StartPoint = EditPoint[4];// LEFT_TOP
				EndPoint = EditPoint[6];// LEFT_BOTTOM
				break;
			case EDIT_GRID_RIGHT:
				StartPoint = EditPoint[5];// RIGHT_TOP
				EndPoint = EditPoint[7];// RIGHT_BOTTOM
				break;
			case EDIT_GRID_TOP:
				StartPoint = EditPoint[4];// LEFT_TOP
				EndPoint = EditPoint[5];// RIGHT_TOP
				break;
			case EDIT_GRID_BOTTOM:
				StartPoint = EditPoint[6];// LEFT_BOTTOM
				EndPoint = EditPoint[7];// RIGHT_BOTTOM
				break;
			}
			int RectCount;
			double MarginX = EndPoint.x - StartPoint.x;
			double MarginY = EndPoint.y - StartPoint.y;
			TempPoint = StartPoint;
			RectCount = ((MarginX / GridW > MarginY / GridH) ? MarginX / GridW : MarginY / GridH) + 1;
			dx = MarginX / RectCount;
			dy = MarginY / RectCount;
			for (int j = 0; j < RectCount; j++) {
				Rect.left = TempPoint.x - GridW;
				Rect.right = TempPoint.x + GridW;
				Rect.top = TempPoint.y - GridH;
				Rect.bottom = TempPoint.y + GridH;
				EditGrid.sEditMode = (EDIT_GRID_MODE)(i + 2);
				EditGrid.sEditGrid = Rect;
				m_EditGridList.push_back(EditGrid);
				TempPoint.x += dx;
				TempPoint.y += dy;
			}
		}
	}
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ShowCursorInfo(POINT point)
{
	POINT WndPt = point;
	m_strCursorInfo = _T("");
	if ( ::PtInRect(&m_WndRect, WndPt) == TRUE )	
	{		
		size_t   idx=0;
		double   value=0;
		double   ZoomRatio = 100.0/m_ImageZoom;
		int      R=0, G=0, B=0;		
		unsigned char uR=0, uG=0, uB=0;
		unsigned char IR=0, IG=0, IB=0, IV=0;
		POINT    ImagePos={0};			
		TPOINT2D StageCp;
		TPOINT2D StagePos;		
		TPOINT2D ImagePt=WndPt;
		TPOINT2D WndPt2 =WndPt;		
		IMAGE_PTR  ImagePtr=m_ImagePtrRaw;
		const size_t ImageW = m_ImageW;
		const size_t ImageH = m_ImageH;
		const int nImageW = (int)(ImageW);
		const int nImageH = (int)(ImageH);		
		size_t BitCount=m_BitCountRaw;
		size_t ImageStep=m_ImageStepRaw;
		if ( NULL == ImagePtr ) 
		{	
			ImagePtr = m_ImagePtr; 
			BitCount = m_BitCount; 
			ImageStep = m_ImageStep;
		}
		StageCp.x = m_ImageStageRgn.GetCpX();
		StageCp.y = m_ImageStageRgn.GetCpY();
		MapWndPtToImagePt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);
		MapImagePtToWndPt_DBL(ImageW, ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt, WndPt2);
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, m_ImageResolution, ImagePt, StageCp, StagePos);			
		
		JetAPI::Point2DToPoint(ImagePt, ImagePos);
		if ( NULL==ImagePtr || ImagePos.x<0 || ImagePos.y<0 || ImagePos.x>=nImageW || ImagePos.y>=nImageH )
		{	m_strCursorInfo.Format(_T("Zoom:%.2f%%, Pos(%d, %d), Image(%.2f, %.2f), Stage(%.2f, %.2f)"), ZoomRatio, WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, StagePos.x, StagePos.y); }
		else if ( 8 == BitCount )
		{	
			idx = (ImagePos.y*ImageStep)+(ImagePos.x);
			R = G = B = ImagePtr[idx];
			m_strCursorInfo.Format(_T("Zoom:%.2f%%, Pos(%d, %d), Image(%.2f, %.2f), RGB=(%d, %d, %d), Stage=(%.2f, %.2f)"), ZoomRatio, WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, R, G, B, StagePos.x, StagePos.y);				
		}
		else if ( 24 == BitCount )
		{	
			idx = (ImagePos.y*ImageStep)+(ImagePos.x*3);
			uB = ImagePtr[idx]; 
			uG = ImagePtr[idx+1]; 
			uR = ImagePtr[idx+2];
			ImageAPI.RGBConvertToRGBV(uR, uG, uB, IR, IG, IB, IV);
			m_strCursorInfo.Format(_T("Zoom:%.2f%%, Pos(%d, %d), Image(%.2f, %.2f), RGB=(%d, %d, %d), RGBV=(%d, %d, %d, %d), Stage=(%.2f, %.2f)"), ZoomRatio, WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, uR, uG, uB, IR, IG, IB, IV, StagePos.x, StagePos.y);				
		}
		else
		{	m_strCursorInfo.Format(_T("Zoom:%.2f%%, Pos(%d, %d), Image(%.2f, %.2f), Stage=(%.2f, %.2f)"), ZoomRatio, WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, StagePos.x, StagePos.y); }		
	}	
	return ;
}
//-------------------------------------------------------------------------------------//
BOOL CImageWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if ( NULL != m_CursorID )
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(m_CursorID));
		return TRUE;
	}	
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::UpdateCursor(const POINT &pt)
{
	m_CursorID = NULL;	
	m_EditRectMode = EDIT_GRID_NULL;
	if ( IMAGE_LBTN_CLICK_EDIT_BOX != m_LBtnClickMode ) { return; }
	
	size_t       i = 0;
	BOOL         bPtInRect=FALSE;
	TEditGrid   *EditGridPtr=NULL;
	const size_t EditGridCount = this->m_EditGridList.size();

	for ( i=0; i<EditGridCount; i++ )
	{
		EditGridPtr = &(m_EditGridList[i]);
		bPtInRect = ::PtInRect(&(EditGridPtr->sEditGrid), pt);
		if ( EDIT_GRID_OUTSIDE==EditGridPtr->sEditMode && FALSE==bPtInRect )
		{	return;	}
	//	if ( EDIT_GRID_INSIDE==EditGridPtr->sEditMode && TRUE==bPtInRect )
	//	{	return;	}

		if ( FALSE == bPtInRect ) { continue; }
#ifdef CURSOR_DEBUG
		m_ShowCursorInfo = true;
		m_strCursorInfo = _T("滑鼠離開區域！");
		if (FALSE == bPtInRect) { continue; }
		m_strCursorInfo = _T("滑鼠進入區域！");
#else
		if (FALSE == bPtInRect) { continue; }
#endif // CURSOR_DEBUG
		switch ( EditGridPtr->sEditMode )
		{
		case EDIT_GRID_LEFT:
		case EDIT_GRID_RIGHT:
		case EDIT_GRID_TOP:
		case EDIT_GRID_BOTTOM:
			m_CursorID = IDC_SIZEALL;
			break;
		case EDIT_GRID_CENTER_LEFT:
		case EDIT_GRID_CENTER_RIGH:
			m_CursorID = IDC_SIZEWE;
			break;
		case EDIT_GRID_CENTER_TOP:
		case EDIT_GRID_CENTER_BOTTOM:
			m_CursorID = IDC_SIZENS;
			break;
		case EDIT_GRID_LEFT_TOP:
		case EDIT_GRID_RIGHT_BOTTOM:
			m_CursorID = IDC_SIZENWSE;
			break;
		case EDIT_GRID_RIGHT_TOP:
		case EDIT_GRID_LEFT_BOTTOM:
			m_CursorID = IDC_SIZENESW;
			break;
		}	
		m_EditRectMode = EditGridPtr->sEditMode;
		if ( NULL != m_CursorID ) 
		{	return;		}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::UpdateShowEditLine()
{
	if ( IMAGE_LBTN_CLICK_EDIT_BOX == m_LBtnClickMode )	
	{	m_ShowEditGridLine = true; }
	else
	{	m_ShowEditGridLine = false; }
}
//-------------------------------------------------------------------------------------//
void CImageWnd::UpdateShowMeasureLine()
{
	if ( IMAGE_LBTN_CLICK_MEASURE == m_LBtnClickMode )	
	{	m_ShowMeasureLine = true; }
	else
	{	m_ShowMeasureLine = false; }
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ExecEditRect()
{
	const double ComponentAngle = GetImageEditRectAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if (true == IsExceptionAngle) {
		ExecEditRectExceptionAngle();
		return;
	}

	TPOINT2D dp;
	BOOL bDbSide = FALSE;
	BOOL bCtrl = GetIsPressVRKey(VK_CONTROL);	
	if ( TRUE == bCtrl ) 
	{	bDbSide = FALSE; }
	else
	{	bDbSide = TRUE; }
	dp.x = m_ImageEditPt2.x-m_ImageEditLast.x;
	dp.y = m_ImageEditPt2.y-m_ImageEditLast.y;
	switch ( m_EditRectMode )
	{
	case EDIT_GRID_INSIDE:
		break;
	case EDIT_GRID_LEFT:
	case EDIT_GRID_RIGHT:
	case EDIT_GRID_TOP:
	case EDIT_GRID_BOTTOM:
		m_ImageEditRect.left  += dp.x;
		m_ImageEditRect.right += dp.x;
		m_ImageEditRect.top    += dp.y;
		m_ImageEditRect.bottom += dp.y;
		break;		
	case EDIT_GRID_CENTER_LEFT:
		m_ImageEditRect.left  += dp.x;
		if ( TRUE == bDbSide )
		{	m_ImageEditRect.right -= dp.x; }
		break;
	case EDIT_GRID_CENTER_RIGH:
		
		m_ImageEditRect.right += dp.x;
		if ( TRUE == bDbSide )
		{	m_ImageEditRect.left  -= dp.x; }
		break;
	case EDIT_GRID_CENTER_TOP:
		m_ImageEditRect.top    += dp.y;
		if ( TRUE == bDbSide )
		{	m_ImageEditRect.bottom -= dp.y;}
		break;
	case EDIT_GRID_CENTER_BOTTOM:		
		m_ImageEditRect.bottom += dp.y;
		if ( TRUE == bDbSide )
		{	m_ImageEditRect.top    -= dp.y; }
		break;
	case EDIT_GRID_LEFT_TOP:
		m_ImageEditRect.left  += dp.x;		
		m_ImageEditRect.top    += dp.y;
		if ( TRUE == bDbSide )
		{
			m_ImageEditRect.right -= dp.x;
			m_ImageEditRect.bottom -= dp.y;
		}		
		break;
	case EDIT_GRID_RIGHT_TOP:		
		m_ImageEditRect.right += dp.x;
		m_ImageEditRect.top    += dp.y;
		if ( TRUE == bDbSide )
		{
			m_ImageEditRect.left  -= dp.x;
			m_ImageEditRect.bottom -= dp.y;
		}		
		break;
	case EDIT_GRID_LEFT_BOTTOM:
		m_ImageEditRect.left  += dp.x;		
		m_ImageEditRect.bottom += dp.y;
		if ( TRUE == bDbSide )
		{
			m_ImageEditRect.right -= dp.x;
			m_ImageEditRect.top    -= dp.y;
		}
		break;
	case EDIT_GRID_RIGHT_BOTTOM:		
		m_ImageEditRect.right += dp.x;		
		m_ImageEditRect.bottom += dp.y;
		if ( TRUE == bDbSide )
		{
			m_ImageEditRect.left  -= dp.x;
			m_ImageEditRect.top    -= dp.y;
		}
		break;
	}		
	//ExecEditRect();
	//m_ImageEditLast
	m_ImageEditLast = m_ImageEditPt2;
	this->CalcImageWndLBtn();
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ExecEditRectExceptionAngle()
{
	const double ComponentAngle = GetImageEditRectAngle();
	const double ImageAngle = JetAPI::MapCadAngleToImageAngle(ComponentAngle);
	const double COS = cos(ImageAngle*DEG_TO_RAD_DBL);
	const double SIN = sin(ImageAngle*DEG_TO_RAD_DBL);
	TPOINT2D dp;
	BOOL bDbSide = FALSE;
	BOOL bCtrl = GetIsPressVRKey(VK_CONTROL);
	if (TRUE == bCtrl)
	{
		bDbSide = FALSE;
	}
	else
	{
		bDbSide = TRUE;
	}
	dp.x = m_ImageEditPt2.x - m_ImageEditLast.x;
	dp.y = m_ImageEditPt2.y - m_ImageEditLast.y;
	double dtx = dp.x*COS + dp.y*SIN;
	double dty = dp.x*(-SIN) + dp.y*COS;
	double dtx_x = dtx*COS;	double dtx_y = dtx*SIN;
	double dty_x = dty*(-SIN);	double dty_y = dty*COS;
	EDIT_GRID_MODE EditRectMode = m_EditRectMode;
	double *left, *right, *top, *bottom;
	if (true == m_ImageEditRectRotated) {
		left = &m_ImageEditRect.left;
		right = &m_ImageEditRect.right;
		top = &m_ImageEditRect.bottom;
		bottom = &m_ImageEditRect.top;
	}
	else {
		left = &m_ImageEditRect.left;
		right = &m_ImageEditRect.right;
		top = &m_ImageEditRect.top;
		bottom = &m_ImageEditRect.bottom;
	}
	if (true == m_ImageEditRectRotated) {
		switch (EditRectMode)
		{
		case EDIT_GRID_CENTER_TOP:
			EditRectMode = EDIT_GRID_CENTER_BOTTOM;
			break;
		case EDIT_GRID_CENTER_BOTTOM:
			EditRectMode = EDIT_GRID_CENTER_TOP;
			break;
		case EDIT_GRID_LEFT_TOP:
			EditRectMode = EDIT_GRID_LEFT_BOTTOM;
			break;
		case EDIT_GRID_RIGHT_TOP:
			EditRectMode = EDIT_GRID_RIGHT_BOTTOM;
			break;
		case EDIT_GRID_LEFT_BOTTOM:
			EditRectMode = EDIT_GRID_LEFT_TOP;
			break;
		case EDIT_GRID_RIGHT_BOTTOM:
			EditRectMode = EDIT_GRID_RIGHT_TOP;
			break;
		}
	}
	switch (EditRectMode)
	{
	case EDIT_GRID_INSIDE:
		break;
	case EDIT_GRID_LEFT:
	case EDIT_GRID_RIGHT:
	case EDIT_GRID_TOP:
	case EDIT_GRID_BOTTOM:
		*left += dp.x;
		*right += dp.x;
		*top += dp.y;
		*bottom += dp.y;
		break;
	case EDIT_GRID_CENTER_LEFT:
		*left += dtx_x;
		*top += dtx_y;
		if (TRUE == bDbSide)
		{
			*right -= dtx_x;
			*bottom -= dtx_y;
		}
		break;
	case EDIT_GRID_CENTER_RIGH:
		*right += dtx_x;
		*bottom += dtx_y;
		if (TRUE == bDbSide)
		{
			*left -= dtx_x;
			*top -= dtx_y;
		}
		break;
	case EDIT_GRID_CENTER_TOP:
		*left += dty_x;
		*top += dty_y;
		if (TRUE == bDbSide)
		{
			*right -= dty_x;
			*bottom -= dty_y;
		}
		break;
	case EDIT_GRID_CENTER_BOTTOM:
		*right += dty_x;
		*bottom += dty_y;
		if (TRUE == bDbSide)
		{
			*left -= dty_x;
			*top -= dty_y;
		}
		break;
	case EDIT_GRID_LEFT_TOP:
		*left += dtx_x + dty_x;
		*top += dtx_y + dty_y;
		if (TRUE == bDbSide)
		{
			*right -= dtx_x + dty_x;
			*bottom -= dtx_y + dty_y;
		}
		break;
	case EDIT_GRID_RIGHT_TOP:
		*left += dty_x;
		*top += dty_y;
		*right += dtx_x;
		*bottom += dtx_y;
		if (TRUE == bDbSide)
		{
			*left -= dtx_x;
			*top -= dtx_y;
			*right -= dty_x;
			*bottom -= dty_y;
		}
		break;
	case EDIT_GRID_LEFT_BOTTOM:
		*left += dtx_x;
		*top += dtx_y;
		*right += dty_x;
		*bottom += dty_y;
		if (TRUE == bDbSide)
		{
			*right -= dtx_x;
			*bottom -= dtx_y;
			*left -= dty_x;
			*top -= dty_y;
		}
		break;
	case EDIT_GRID_RIGHT_BOTTOM:
		*right += dtx_x + dty_x;
		*bottom += dtx_y + dty_y;
		if (TRUE == bDbSide)
		{
			*left -= dtx_x + dty_x;
			*top -= dtx_y + dty_y;
		}
		break;
	}
	m_ImageEditLast = m_ImageEditPt2;

	//if()
	this->CalcImageWndLBtn();
}
//-------------------------------------------------------------------------------------//
inline BOOL CImageWnd::GetIsPressVRKey(int VK)
{
	switch ( VK )
	{
	case VK_CONTROL:
		VK = VK;
		break;
	case VK_SHIFT:
		VK = VK;
		break;
	}
	if ( ::GetKeyState(VK) < 0 ) { return TRUE; }
	return FALSE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CImageWnd::CalcImageWndZoom(double OldZoom, double NewZoom, TPOINT2D &OffsetPt)
{
	ImageAPI.CalcImageWndZoom(OldZoom, NewZoom, OffsetPt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CImageWnd::CalcImageWndFitZoom(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const double Ratio, double &Zoom)
{
	ImageAPI.CalcImageWndFitZoom(ImageW, ImageH, WndRect, Ratio, Zoom);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CImageWnd::MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt)
{
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndPt, ImagePt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline BOOL CImageWnd::MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt)
{
	ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImagePt, WndPt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetImageInfo(CAMERA_ID CameraID, const TREGION4D &StageRgn, const TPOINT2D &Res, IMAGE_DATA_SRC SrcMode)//影像在機台位置
{	
	m_CameraID = CameraID;
	m_ImageStageRgn = StageRgn;	
	m_ImageResolution = Res;
	m_ImageDataSrc = SrcMode;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetShowLaneID(LANE_ID val)
{
	m_ShowLaneID = val;
}
//-------------------------------------------------------------------------------------//
LANE_ID CImageWnd::GetShowLaneID() const
{
	return m_ShowLaneID;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SetProjectPtr(CAOIProject *ProjectPtr)
{
	m_ProjectPtr = ProjectPtr;
	m_ProjectMapIndex = 0;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::SwitchProjectMap()//切換專案底圖
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	
	bool       bForce=false;
	TPOINT2D   ImageRes;	
	IMAGE_PTR  ImagePtr=NULL;			
	TREGION4D  RgnCad, RgnStage;
	const bool bTestMap = CheckShowProjectTestMpa();
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	unsigned int NextMapIndex = ProjectPtr->GetProjectMapIndexNext(m_ProjectMapIndex);
	if ( true == bTestMap ) { bForce = true; }
	ProjectPtr->GetProjectMapInfo(ImageRes, RgnCad, RgnStage);
	ProjectPtr->GetProjectMapCalcRgn(RgnStage);
	ProjectPtr->GetProjectMapPtr(NextMapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
	ProjectPtr->CreateProjectMapShowPtr(NextMapIndex, ImagePtr, bForce, bTestMap);
	if ( NULL == ImagePtr )
	{	
		NextMapIndex = 0; 
		ProjectPtr->GetProjectMapPtr(NextMapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr);
		ProjectPtr->CreateProjectMapShowPtr(NextMapIndex, ImagePtr, bForce, bTestMap);
	}
	m_ProjectMapIndex = NextMapIndex;
	ProjectPtr->SetProjectMapIndex(NextMapIndex);
	m_strImageText = ProjectPtr->GetProjectMapIndexName();
	if ( IMAGE_DATA_MAP == m_ImageDataSrc )
	{	SetImageBuffer(ImageW, ImageH, ImageStep, BitCount, ImagePtr, false, false, true); }
	AOIDataCollect.PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SWITCH_FRAME_IMAGE, NULL);
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ExecMovePanel(const POINT &pt)//移動整板
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) { return; }

	size_t         i=0;
	double         dX=0, dY=0;
	CAMERA_ID       CameraID = this->m_CameraID;			
	CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;		
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t    BoardCount = PanelPtr->GetPanelBoardCount();	
	const size_t    ComponentCount = PanelPtr->GetPanelComponentCount();

	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	dX = ResX*pt.x*this->m_ImageZoom;
	dY = ResY*pt.y*this->m_ImageZoom;
	dX =  dX;//Cad
	dY = -dY;//Cad
	PanelPtr->MovePanelPos(dX, dY);	
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_MODIFY_STAGE_POS, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ExecMovePanelStage(const POINT &pt)//移動整板機台座標
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel   *PanelPtr = ProjectPtr->GetProjectPanelPtrBySelected();
	if ( NULL == PanelPtr ) { return; }

	size_t          i=0;
	double          dX=0, dY=0;
	double          dX2=0, dY2=0;
	CAMERA_ID       CameraID = m_CameraID;			
	CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;		
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t    BoardCount = PanelPtr->GetPanelBoardCount();
	DISTRICT_ID     DistrictID = ProjectPtr->GetProjectActDistrictID();
	const size_t    ComponentCount = PanelPtr->GetPanelComponentCount();

	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	dX = ResX*pt.x*m_ImageZoom;
	dY = ResY*pt.y*m_ImageZoom;
	dX =  dX;//Cad
	dY = -dY;//Cad
	AOIDataCollect.MapCadOffsetPtToStage(dX, dY, dX2, dY2);
	PanelPtr->MovePanelStagePos(dX2, dY2, DistrictID);	
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_MODIFY_STAGE_POS, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ExecMoveProject(const POINT &pt)//移動專案
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	
	size_t         i=0;
	double         dX=0, dY=0;
	CAMERA_ID      CameraID = this->m_CameraID;
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	dX = ResX*pt.x*m_ImageZoom;
	dY = ResY*pt.y*m_ImageZoom;
	dX =  dX;//Cad
	dY = -dY;//Cad
	ProjectPtr->MoveProjectPos(dX, dY);	
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_MODIFY_STAGE_POS, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ExecMoveProjectStage(const POINT &pt)//移動專案
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	
	size_t         i=0;
	double         dX=0, dY=0;
	double         dX2=0, dY2=0;
	CAMERA_ID      CameraID = this->m_CameraID;
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	dX = ResX*pt.x*m_ImageZoom;
	dY = ResY*pt.y*m_ImageZoom;
	dX =  dX;//Cad
	dY = -dY;//Cad
	AOIDataCollect.MapCadOffsetPtToStage(dX, dY, dX2, dY2);
	ProjectPtr->MoveProjectStagePos(dX2, dY2);	
	PostParentMessage(MSG_IMAGE_WND_NOTIFY_EVENT, WPARAM_MODIFY_STAGE_POS, NULL);
	return;
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ExecStageMoveTo(const POINT &Pt)//移動機台至
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	const bool bRemoteState = AOIDataCollect.CheckMES_CtrlState(MES_EQP_CTRL_STATE_REMOTE);
	if ( true == bRemoteState ) { return; }

	TPOINT2D StagePt;	
	const bool ShowAsk = GetShowMoveToMsg();
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	StagePt = CalcStagePos(Pt);
	if ( false==OfflineMode && true==ShowAsk )
	{
		CString str, str2;
		str = "Do you want to move stage";
		str = AOIDataCollect.LoadMultiLanguageString(str, str);
		str2.Format(_T("%s (%.0f, %.0f) ?"), str, StagePt.x, StagePt.y);
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) != IDYES )
		{	return;	}
	}
	if ( MotionCtrlPtr->XYMoveTo(StagePt.x, StagePt.y, OfflineMode) == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return;
	}
	//m_ImageOffset.x = m_ImageOffset.y = 0;
	AOIDataCollect.PostCallbackWndMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);	
}
//-------------------------------------------------------------------------------------//
void CImageWnd::ExecSelectComponent(const TRECT4D &Rect)//選取零件
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CAOIComponent *ComponentPtr=NULL;
	const bool bResultMode=false;
	TREGION4D  StageRgn=CalcStageRegion(Rect);	
	std::vector<CAOIComponent*> ComponentList;

	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByStage(StageRgn, bResultMode, ComponentList);	

	size_t       i=0;
	const size_t SelCount = ComponentList.size();
	if ( SelCount > 0 ) 
	{
		for ( i=0; i<SelCount; i++ )
		{
			ComponentPtr = ComponentList[i];
			if ( NULL == ComponentPtr ) { continue; }
			ComponentPtr->SetComponentSelected(true);
		}
		ComponentPtr = ComponentList[0];
		ProjectPtr->SetProjectActiveComponent(ComponentPtr);
	}
}
//-------------------------------------------------------------------------------------//
TPOINT2D CImageWnd::CalcStagePos(const POINT &Pt)//計算機台座標
{
	TPOINT2D StagePt;
	TPOINT2D StageRgnCp;
	TPOINT2D ImagePt;
	TPOINT2D WndPtr = Pt;	
	StageRgnCp.x = m_ImageStageRgn.GetCpX();
	StageRgnCp.y = m_ImageStageRgn.GetCpY();
	CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPtr, ImagePt);
	//AOIDataCollect.MapCameraPtToStage(m_CameraID, ImagePt, StageRgnCp, StagePt);
	AOIDataCollect.MapCameraPtToStage(m_ImageW, m_ImageH, m_ImageResolution, ImagePt, StageRgnCp, StagePt);	
	return StagePt;
}
//-------------------------------------------------------------------------------------//
TREGION4D CImageWnd::CalcStageRegion(const TREGION4D &Region)//計算機台座標
{	
	TPOINT2D WndPt1;	
	TPOINT2D WndPt2;	
	TPOINT2D ImagePt1;
	TPOINT2D ImagePt2;
	TPOINT2D StagePt1;
	TPOINT2D StagePt2;		
	TPOINT2D StageRgnCp;	
	TREGION4D  StageRgn;

	WndPt1.x = Region.minX;
	WndPt1.y = Region.minY;
	WndPt2.x = Region.maxX;
	WndPt2.y = Region.maxY;
	StageRgnCp.x = m_ImageStageRgn.GetCpX();
	StageRgnCp.y = m_ImageStageRgn.GetCpY();	
	CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPt1, ImagePt1);
	CImageWnd::MapWndPtToImagePt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt2);
	//AOIDataCollect.MapCameraPtToStage(m_CameraID, ImagePt, StageRgnCp, StagePt);
	AOIDataCollect.MapCameraPtToStage(m_ImageW, m_ImageH, m_ImageResolution, ImagePt1, StageRgnCp, StagePt1);	
	AOIDataCollect.MapCameraPtToStage(m_ImageW, m_ImageH, m_ImageResolution, ImagePt2, StageRgnCp, StagePt2);	
	StageRgn.minX = MIN(StagePt1.x, StagePt2.x);
	StageRgn.minY = MIN(StagePt1.y, StagePt2.y);
	StageRgn.maxX = MAX(StagePt1.x, StagePt2.x);
	StageRgn.maxY = MAX(StagePt1.y, StagePt2.y);
	return StageRgn;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::CheckOutOfImageRgn()//確認是否超過影像範圍
{
	TPOINT2D WndPt1;
	TPOINT2D WndPt2;
	TPOINT2D ImagePt1;
	TPOINT2D ImagePt2;

	ImagePt1.x = 0;
	ImagePt1.y = 0;
	ImagePt2.x = m_ImageW;
	ImagePt2.y = m_ImageH;
	CImageWnd::MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt1, WndPt1);
	CImageWnd::MapImagePtToWndPt_DBL(m_ImageW, m_ImageH, m_WndRect, m_ImageOffset, m_ImageZoom, ImagePt2, WndPt2);
	
	const int MarginX = 100;
	const int MarginY = 100;
	const int MinX = (int)(MIN(WndPt1.x, WndPt2.x)+0.5);
	const int MinY = (int)(MIN(WndPt1.y, WndPt2.y)+0.5);
	const int MaxX = (int)(MAX(WndPt1.x, WndPt2.x)+0.5);
	const int MaxY = (int)(MAX(WndPt1.y, WndPt2.y)+0.5);

	RECT  CheckRect = m_WndRect;
	CheckRect.left    -= MarginX;
	CheckRect.top     -= MarginY;
	CheckRect.right   += MarginX;
	CheckRect.bottom  += MarginY;

	if ( MinX>CheckRect.left || MinY>CheckRect.top || MaxX<CheckRect.right || MaxY<CheckRect.bottom ) 
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
BOOL CImageWnd::SendParentMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( NULL==this || NULL==this->GetSafeHwnd() ) { return FALSE; }
	CWnd *Parent = CWnd::GetParent();
	if ( NULL==Parent || NULL==Parent->GetSafeHwnd() ) { return FALSE; }
	::SendMessage(Parent->GetSafeHwnd(), message, wParam, lParam);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CImageWnd::PostParentMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( NULL==this || NULL==this->GetSafeHwnd() ) { return FALSE; }
	CWnd *Parent = CWnd::GetParent();
	if ( NULL==Parent || NULL==Parent->GetSafeHwnd() ) { return FALSE; }
	::PostMessage(Parent->GetSafeHwnd(), message, wParam, lParam);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CImageWnd::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
bool CImageWnd::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
