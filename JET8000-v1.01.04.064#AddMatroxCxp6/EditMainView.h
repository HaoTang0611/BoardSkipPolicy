#if !defined(AFX_EDITMAINVIEW_H__633F354A_9C8B_4858_A9A6_E6CC6E66502D__INCLUDED_)
#define AFX_EDITMAINVIEW_H__633F354A_9C8B_4858_A9A6_E6CC6E66502D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMainView.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "ProjectMarkWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditMainView view
//-------------------------------------------------------------------------------------//
class CEditMainView : public CView
{
protected:
	CEditMainView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditMainView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditMainView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CEditMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//---------------------------------------------------------------------------------//	
	CProjectMarkWnd            m_ProjectMarkWnd;
	//---------------------------------------------------------------------------------//	
	int                        GetEditLineSize();//取得編輯線的尺寸
	int                        GetEditCheckSize();//取得編輯確認的尺寸
	bool                       PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2);
	BOOL                       MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt);
	BOOL                       MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt);
	//-------------------------------------------------------------------------------------//
	COLORREF                   m_BkColor;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMapDC;//專案底圖的DC
	CJetMemDC                  m_ImageWndMemDC;	
	CJetMemDC                  m_ImageWndMemDC2;	
	//-------------------------------------------------------------------------------------//	
	COLORREF                   m_clrFdLine;
	COLORREF                   m_clrFdText;
	COLORREF                   m_clrFdLineExt;	
	COLORREF                   m_clrSBLine;
	COLORREF                   m_clrPanelLine;
	COLORREF                   m_clrPanelText;
	COLORREF                   m_clrBoardLine;
	COLORREF                   m_clrBoardText;
	COLORREF                   m_clrComponentLine;
	COLORREF                   m_clrComponentText;	
	COLORREF                   m_clrSelected;
	//-------------------------------------------------------------------------------------//
	CAOIProject               *m_ProjectPtr;	
	CAOIBoard                 *m_BoardPtr_Add;
	CString                    m_ComponentName;
	CString                    m_PartNumberName;
	//-------------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_MapImageW;
	IMAGE_SIZE                 m_MapImageH;
	IMAGE_SIZE                 m_MapImageStep;
	IMAGE_SIZE                 m_MapBitCount;
	IMAGE_PTR                  m_MapImagePtr;
	TPOINT2D                   m_MapResolution;//影像解析度			
	TREGION4D                  m_MapCadRgn;
	TREGION4D                  m_MapStageRgn;	
	//-------------------------------------------------------------------------------------//
	double                     m_FrameRatio;
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE                 m_ShowImageStep;
	IMAGE_SIZE                 m_ShowBitCount;	
	IMAGE_PTR                  m_ShowImagePtr;
	TPOINT2D                   m_FrameResolution;//影像解析度		
	TREGION4D                  m_FrameStageRgn;
	TUNI_FRAME                 m_UniFrameList[FRAME_MAX_COUNT];	
	//-------------------------------------------------------------------------------------//
	double                     m_MapZoom;
	double                     m_ImageZoom;	
	TPOINT2D                   m_ImageOffset;
	bool                       m_ModifyPanelPos;
	bool                       m_ModifyBoardPos;
	bool                       m_ModifyComponentPos;
	bool                       m_DrawAddRect;	
	bool                       m_DrawPanelList;
	bool                       m_DrawBoardList;	
	bool                       m_DrawComponent;
	bool                       m_DrawFieldList;	
	bool                       m_DrawComponentMode;
	bool                       m_ShowComponentName;
	bool                       m_LoadOfflineParam;//是否載入離線編程
	DWORD                      m_UpdateTestMapTickCount;//更新底圖時間點
	//-------------------------------------------------------------------------------------//
	std::vector<TPanelRect>    m_SelPanelList;
	std::vector<TBoardRect>    m_SelBoardList;
	std::vector<TComponentRect> m_SelComponentList;
	//-------------------------------------------------------------------------------------//
	std::vector<CAOIPanel*>    m_ClonePanelList;
	std::vector<CAOIBoard*>    m_CloneBoardList;
	std::vector<CAOIComponent*> m_CloneComponentList;
	//-------------------------------------------------------------------------------------//
	void                       SetDrawAddRect(bool Draw);
	//-------------------------------------------------------------------------------------//
	void                       CloseProject(bool ResetData);	
	void                       SwitchProject();//切換專案	
	CAOIProject*               GetActiveProject();
	void                       SwitchProjectDistrictID(bool bRedraw);//切換專案
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------------------//
	UINT                       m_MainMode;
	POINT                      m_MousePosLast;//滑鼠座標-上一個
	POINT                      m_MousePosFirst;//滑鼠座標-第1個
	POINT                      m_MousePosCurrent;//滑鼠座標-現今
	POINT                      m_MousePosImageWnd;//滑鼠座標-圖像視窗
	CURSOR_POS_MODE            m_MousePosMode;//滑鼠座標模式
	bool                       m_ShowPopupMenu;
	bool                       CheckMousePosMoved() const;//確認滑鼠移動過
	//---------------------------------------------------------------------------------//		
	void                       SwitchImage(bool NextMap);//切換畫面
	void                       SwitchMapImage(bool NextMap);//切換專案底圖
	void                       BackupViewParam();//備份顯示參數
	void                       RestoreViewParam();//恢復顯示參數
	void                       CalcFovPosition();//計算FOV的位置
	void                       CalcImageOffset();//計算影像顯示移動值	
	void                       ResetImageOffset();//復歸顯示移動值	
	void                       BuildObjectSelected();//更新選到的物件
	void                       BuildPanelSelected();//更新選到的整板
	void                       BuildBoardSelected();//更新選到的單板
	void                       BuildComponentSelected();//更新選到的零件	
	CURSOR_POS_MODE            CheckCursorPosMode(POINT pt);//確認鼠標座標模式
	CURSOR_POS_MODE            CheckCursorPosMode_Panel(POINT pt);//確認鼠標座標模式
	CURSOR_POS_MODE            CheckCursorPosMode_Board(POINT pt);//確認鼠標座標模式
	CURSOR_POS_MODE            CheckCursorPosMode_Component(POINT pt);//確認鼠標座標模式
	const TPOINT2D&            GetImageResolution() const;//取得影像解析度
	void                       GetImageInfo(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, TPOINT2D &ImageRes, TPOINT2D  &StageCp);//取得目前畫面資訊
	void                       GetImageInfo(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, TPOINT2D &ImageRes, TREGION4D &StageRgn);//取得目前畫面資訊
	//---------------------------------------------------------------------------------//		
	void                       RedrawWnd();
	void                       CreateBKImage();	
	void                       CreateMapImage(bool bTestMap=false);	
	void                       CreateTestMapImage();
	void                       DrawImage(HDC hDC);
	void                       DrawProjectMap(HDC hDC, bool bTestMap);
	void                       DrawAddRect(HDC hDC);
	void                       DrawComponent(HDC hDC);
	void                       DrawBoardList(HDC hDC);
	void                       DrawPanelList(HDC hDC);
	void                       DrawFieldList(HDC hDC);
	void                       DrawCrossLine(HDC hDC);
	void                       DrawCrosshair(HDC hDC);//十字線
	void                       DrawRect(HDC hDC, const RECT &Rect);
	void                       DrawRectLine(HDC hDC, const POINT pt[], size_t num);	
	bool                       DrawRectRoughLine(HDC hDC, const RECT &Rect, COLORREF color, int Gap);
	void                       DrawCircleLine(HDC hDC, const RECT &Rect);		
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 GetMapImageW() const;
	IMAGE_SIZE                 GetMapImageH() const;
	bool                       GetCurrentImage(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);
	bool                       GetStageSelectRegion(TREGION4D &StageRgn);//取得選取區域-機台座標
	//---------------------------------------------------------------------------------//		
	int                        GetMaxFrameCount();
	IMAGE_SIZE                 GetFrameImageW() const;
	IMAGE_SIZE                 GetFrameImageH() const;
	bool                       AdjustCurrentFrames();
	bool                       FillCurrentFrames(double Ratio);		
	//---------------------------------------------------------------------------------//	
	void                       SwitchFrameImage(bool NextMap);
	void                       UpdateFrameImage();
	//---------------------------------------------------------------------------------//	
	void                       PreInitUniFrameBuffer();//預先影像記憶體
	void                       ReleaseUniFrameBuffer();//釋放影像記憶體	
	void                       BuildShowImageBuffer();//建立顯示的影像記憶體
	void                       ReleaseShowImageBuffer();//釋放顯示影像記憶體	
	//---------------------------------------------------------------------------------//		
	bool                       GetShowPopupMenu() const;
	void                       SetShowPopupMenu(bool val);	
	bool                       ExecPopupMenu(POINT point, UINT menuID);	
	//---------------------------------------------------------------------------------//		
	bool                       ExecMoveSelectedObject();
	bool                       ExecMoveSelectedObjectKernel(int nWndPx, int nWndPy);
	bool                       ExecMoveSelectedPanel();
	bool                       ExecMoveSelectedPanelKernel(int nWndPx, int nWndPy);
	bool                       ExecMoveSelectedBoard();
	bool                       ExecMoveSelectedBoardKernel(int nWndPx, int nWndPy);
	bool                       ExecMoveSelectedComponent();
	bool                       ExecMoveSelectedComponentKernel(int nWndPx, int nWndPy);

	bool                       ExecMoveSelectedObjectFinish();
	bool                       ExecMoveSelectedPanelFinish();
	bool                       ExecMoveSelectedBoardFinish();
	bool                       ExecMoveSelectedComponentFinish();

	bool                       ExecMainSelectObject();
	bool                       ExecMainSelectPanel();
	bool                       ExecMainSelectBoard();	
	bool                       ExecMainSelectComponent();

	bool                       ExecMoveToStage();
	bool                       ExecShowWndPosition();
	bool                       ExecGrabFov(double PosX, double PosY, double PosZ);
	bool                       ExecUpdateFov(double PosX, double PosY, double PosZ);
	bool                       ExecUpdateFov_Map(double PosX, double PosY, double PosZ);
	bool                       ExecUpdateFov_Frame(double PosX, double PosY, double PosZ);

	bool                       ExecRotateObj(double Angle, bool Inverse);
	bool                       ExecMove(double dX, double dY);
	bool                       ExecMirrorPosX();
	bool                       ExecMirrorPosY();

	bool                       ExecSelectInvertBoard();
	bool                       ExecSelectInvertPanel();
	bool                       ExecSelectInvertProject();

	bool                       ExecDeleteSelected();
	bool                       ExecDeleteSelectedPanel();
	bool                       ExecDeleteSelectedBoard();
	bool                       ExecDeleteSelectedComponent();

	bool                       ExecMainCloneObject();
	bool                       ExecMainClonePanel();
	bool                       ExecMainCloneBoard();
	bool                       ExecMainCloneComponent();

	bool                       ExecMainAddObject();
	bool                       ExecMainAddPanel();
	bool                       ExecMainAddBoard();
	bool                       ExecMainAddComponent();	

	bool                       ExecMainPasteObject();
	bool                       ExecMainPastePanel();
	bool                       ExecMainPasteBoard();
	bool                       ExecMainPasteComponent();

	bool                       ExecMainBypassObject();
	bool                       ExecMainBypassPanel();
	bool                       ExecMainBypassBoard();
	bool                       ExecMainBypassComponent();

	bool                       ExecMainBypass3DComponent();

	bool                       ExecMainProjectMark();
	
	bool                       ExecInspection_Finish();
	bool                       ExecOnlineInspection_Finish();
	bool                       ExecInspectionFinishKernel(bool bOnline);
	bool                       ExecMainCaptureProjectMap();
	bool                       ExecMainDivideDistrictWnd();

	bool                       ExecMainSetBarcodeDeviceIndexPanel();
	bool                       ExecMainSetBarcodeDeviceIndexBoard();
	bool                       ExecMainSetBarcodeDeviceCodeIndexPanel();
	bool                       ExecMainSetBarcodeDeviceCodeIndexBoard();
	//---------------------------------------------------------------------------------//	
	void                       ExecMainEditSaveComponentSample(SAVE_SPC_PART_IMAGE_MODE SaveMode);
	//---------------------------------------------------------------------------------//	
	bool                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//	
	bool                       GetOfflineMode() const;
	//---------------------------------------------------------------------------------//	
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	//---------------------------------------------------------------------------------//
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditMainView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMainEditRotate090();
	afx_msg void OnUpdateMainEditRotate090(CCmdUI* pCmdUI);
	afx_msg void OnMainEditRotate180();
	afx_msg void OnUpdateMainEditRotate180(CCmdUI* pCmdUI);
	afx_msg void OnMainEditRotate270();
	afx_msg void OnUpdateMainEditRotate270(CCmdUI* pCmdUI);
	afx_msg void OnMainEditRotateOthers();
	afx_msg void OnUpdateMainEditRotateOthers(CCmdUI* pCmdUI);
	afx_msg void OnMainEditRotateReverse();
	afx_msg void OnUpdateMainEditRotateReverse(CCmdUI* pCmdUI);
	afx_msg void OnMainEditMirrorPosX();
	afx_msg void OnUpdateMainEditMirrorPosX(CCmdUI* pCmdUI);
	afx_msg void OnMainEditMirrorPosY();
	afx_msg void OnUpdateMainEditMirrorPosY(CCmdUI* pCmdUI);	
	afx_msg void OnMainEditMove();
	afx_msg void OnUpdateMainEditMove(CCmdUI* pCmdUI);
	afx_msg void OnMainEditDeleteSelected();
	afx_msg void OnUpdateMainEditDeleteSelected(CCmdUI* pCmdUI);
	afx_msg void OnMainEditDeleteUnselected();
	afx_msg void OnUpdateMainEditDeleteUnselected(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSetPartNumber();
	afx_msg void OnUpdateMainEditSetPartNumber(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSelectInvertBoard();
	afx_msg void OnUpdateMainEditSelectInvertBoard(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSelectInvertPanel();
	afx_msg void OnUpdateMainEditSelectInvertPanel(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSelectInvertProject();
	afx_msg void OnUpdateMainEditSelectInvertProject(CCmdUI* pCmdUI);
	afx_msg void OnMainEditModeComponent();
	afx_msg void OnUpdateMainEditModeComponent(CCmdUI* pCmdUI);
	afx_msg void OnMainEditModeBoard();
	afx_msg void OnUpdateMainEditModeBoard(CCmdUI* pCmdUI);
	afx_msg void OnMainEditModePanel();
	afx_msg void OnUpdateMainEditModePanel(CCmdUI* pCmdUI);		
	afx_msg void OnMainEditView1x1();
	afx_msg void OnUpdateMainEditView1x1(CCmdUI* pCmdUI);
	afx_msg void OnMainEditViewAll();
	afx_msg void OnUpdateMainEditViewAll(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSwitchOnlineView();
	afx_msg void OnUpdateMainEditSwitchOnlineViewl(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSwitchEditModel();
	afx_msg void OnUpdateMainEditSwitchEditModel(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSwitchEditBarcode();
	afx_msg void OnUpdateMainEditSwitchEditBarcode(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSwitchEditFd();
	afx_msg void OnUpdateMainEditSwitchEditFd(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiSelect();
	afx_msg void OnUpdateMainEditManiSelect(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiSelectXPos();
	afx_msg void OnUpdateMainEditManiSelectXPos(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiSelectYPos();
	afx_msg void OnUpdateMainEditManiSelectYPos(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiSelectRowCol();
	afx_msg void OnUpdateMainEditManiSelectRowCol(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiCopyToFd();
	afx_msg void OnUpdateMainEditManiCopyToFd(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiAdd();
	afx_msg void OnUpdateMainEditManiAdd(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiPaste();
	afx_msg void OnUpdateMainEditManiPaste(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiPasteArray();
	afx_msg void OnUpdateMainEditManiPasteArray(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiClone();
	afx_msg void OnUpdateMainEditManiClone(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSetBypass();
	afx_msg void OnUpdateMainEditSetBypass(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSetBypass3D();
	afx_msg void OnUpdateMainEditSetBypass3D(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSetModelIsolated();
	afx_msg void OnUpdateMainEditSetModelIsolated(CCmdUI* pCmdUI);	
	afx_msg void OnMainEditSetMaskBaseColorLinkIndex();
	afx_msg void OnUpdateMainEditSetMaskBaseColorLinkIndex(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSetComponentAlarmAOI();
	afx_msg void OnUpdateMainEditSetComponentAlarmAOI(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSetComponentSaveReportARS();
	afx_msg void OnUpdateMainEditSetComponentSaveReportARS(CCmdUI* pCmdUI);
	afx_msg void OnMainEditCaptureProjectMap();
	afx_msg void OnUpdateMainEditCaptureProjectMap(CCmdUI* pCmdUI);	
	afx_msg void OnMainEditDivideDistrictWnd();
	afx_msg void OnUpdateMainEditDivideDistrictWnd(CCmdUI* pCmdUI);
	afx_msg void OnMainEditAlignFiducial();
	afx_msg void OnUpdateMainEditAlignFiducial(CCmdUI* pCmdUI);
	afx_msg void OnMainEditViewProjectMap();
	afx_msg void OnUpdateMainEditViewProjectMap(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiPasteToOtherBoards();
	afx_msg void OnUpdateMainEditManiPasteToOtherBoards(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiPasteToOtherPanels();
	afx_msg void OnUpdateMainEditManiPasteToOtherPanels(CCmdUI* pCmdUI);
	afx_msg void OnMainEditManiProjectMark();
	afx_msg void OnUpdateMainEditManiProjectMark(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSetBarcodeDeviceIndex();
	afx_msg void OnUpdateMainEditSetBarcodeDeviceIndex(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSetBarcodeDeviceCodeIndex();
	afx_msg void OnUpdateMainEditSetBarcodeDeviceCodeIndex(CCmdUI* pCmdUI);
	afx_msg void OnMainEditSaveComponentSample();
	afx_msg void OnUpdateMainEditSaveComponentSample(CCmdUI* pCmdUI);
	afx_msg void OnMainEditAddComponentSample();
	afx_msg void OnUpdateMainEditAddComponentSample(CCmdUI* pCmdUI);
	afx_msg void OnMainEditReplaceComponentSample();
	afx_msg void OnUpdateMainEditReplaceComponentSample(CCmdUI* pCmdUI);
	afx_msg void OnMainEditCopyComponentSample();
	afx_msg void OnUpdateMainEditCopyComponentSample(CCmdUI* pCmdUI);
	afx_msg void OnMainEditProjectMapMask();
	afx_msg void OnUpdateMainEditProjectMapMask(CCmdUI* pCmdUI);	
	afx_msg void OnMainEditProjectMapMaskRegion();
	afx_msg void OnUpdateMainEditProjectMapMaskRegion(CCmdUI* pCmdUI);
	afx_msg void OnMainEditProjectCompare();
	afx_msg void OnUpdateMainEditProjectCompare(CCmdUI* pCmdUI);	
	afx_msg void OnMainEditComponentCompare();
	afx_msg void OnUpdateMainEditComponentCompare(CCmdUI* pCmdUI);	
	afx_msg void OnMainEditFullMapComponentCreate();
	afx_msg void OnUpdateMainEditFullMapComponentCreate(CCmdUI* pCmdUI);	
	afx_msg void OnMainEditFullMapComponentClear();
	afx_msg void OnUpdateMainEditFullMapComponentClear(CCmdUI* pCmdUI);	
	afx_msg void OnMainEditSetBoardOrder();
	afx_msg void OnUpdateMainEditSetBoardOrder(CCmdUI* pCmdUI);		
	afx_msg void OnMainEditAutoAddComponet();
	afx_msg void OnUpdateMainEditAutoAddComponet(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMAINVIEW_H__633F354A_9C8B_4858_A9A6_E6CC6E66502D__INCLUDED_)
