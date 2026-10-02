#if !defined(AFX_EDITFDVIEW_H__4AC51E10_360B_4A9A_A6D4_C6A1235451DE__INCLUDED_)
#define AFX_EDITFDVIEW_H__4AC51E10_360B_4A9A_A6D4_C6A1235451DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditFdView.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
//-------------------------------------------------------------------------------------//
enum ALIGN_FD_MODE
{
	ALIGN_FD_PANEL,
	ALIGN_FD_BOARD
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditFdView view

class CEditFdView : public CView
{
protected:
	CEditFdView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditFdView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditFdView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//---------------------------------------------------------------------------------//	
	bool                       PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2);
	BOOL                       MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt);
	BOOL                       MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt);
	BOOL                       MapImageRgnToWndRgn_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TREGION4D &ImageRgn, TREGION4D &WndRgn);
	//-------------------------------------------------------------------------------------//
	COLORREF                   m_BkColor;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMapDC;	
	CJetMemDC                  m_ImageWndMemDC;	
	CJetMemDC                  m_ImageWndMemDC2;		
	//-------------------------------------------------------------------------------------//
	bool                       m_DrawAddRect;
	bool                       m_DrawEditRect;
	bool                       m_KeepImageOffset;
	double                     m_MapZoom;
	double                     m_ImageZoom;	
	TPOINT2D                   m_ImageOffset;
	DWORD                      m_UpdateTestMapTickCount;//更新底圖時間點
	//-------------------------------------------------------------------------------------//
	double                     m_FovRatio;//FOV的比例
	unsigned int               m_ImageIndex;
	//-------------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE                 m_ShowImageStep;
	IMAGE_SIZE                 m_ShowBitCount;
	IMAGE_PTR                  m_ShowImagePtr;
	TPOINT2D                   m_FOVPosStage;	
	TREGION4D                  m_FrameStageRgn;//影像在機台的邊界
	TPOINT2D                   m_FrameResolution;//影像解析度	
	TUNI_FRAME                 m_UniFrameList[FRAME_MAX_COUNT];	
	IMAGE_SIZE                 GetFrameImageW_2() const;
	IMAGE_SIZE                 GetFrameImageH_2() const;
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 GetImageW() const;
	IMAGE_SIZE                 GetImageH() const;
	const TPOINT2D&            GetImageResolution() const;
	const TREGION4D&           GetImageStageRgn() const;
	const TPOINT2D             GetImageStageRgnCp() const;
	//---------------------------------------------------------------------------------//
	bool                       GetShowProjectMapMode() const;
	//---------------------------------------------------------------------------------//
	TREGION4D                  m_EditRegion;
	POINT                      m_MousePosLast;//滑鼠座標-上一個
	POINT                      m_MousePosFirst;//滑鼠座標-第1個
	POINT                      m_MousePosCurrent;//滑鼠座標-現今
	POINT                      m_MousePosImageWnd;//滑鼠座標-圖像視窗
	CURSOR_POS_MODE            m_MousePosMode;//滑鼠座標模式
	bool                       m_ShowPopupMenu;
	bool                       CheckMousePosMoved() const;//確認滑鼠移動過
	//---------------------------------------------------------------------------------//	
	CAOIModel                 *m_ModelPtr;
	CAOIProject               *m_ProjectPtr;		
	std::vector<TFdRect>       m_SelFdList;
	bool                       m_ModifyFdPos;
	CAOIBox                    m_ActiveBox;
	TActiveObj                 m_ActiveObj;
	std::vector<TActiveObj>    m_ActiveObjList;
	ALIGN_FD_MODE              m_AlignFdMode;
	int                        m_CaliIndex;
	size_t                     m_CaliComponentIdx[4];//校正的零件引數
	double                     m_CaliComponentCadPosX[4];//校正零件的座標-X-Cad
	double                     m_CaliComponentCadPosY[4];//校正零件的座標-Y-Cad
	double                     m_CaliComponentStagePosX[4];//校正零件的座標-X-Stage
	double                     m_CaliComponentStagePosY[4];//校正零件的座標-Y-Stage
	TREGION4D                  m_CaliComponentRegion[4];
	//-------------------------------------------------------------------------------------//
	void                       CloseProject();	
	void                       SwitchProject();//切換專案
	CAOIModel*                 GetModelPtr();	
	CAOIProject*               GetActiveProject();
	void                       BuildFdSelected();//更新選到的定位點
	bool                       CheckCalibrationMode();//確認校正基板模式
	void                       ResetCaliComponentList();//復歸校正零件列表		
	//-------------------------------------------------------------------------------------//
	bool                       UpdateFdSelected();
	void                       UpdateActiveObjList();
	void                       BuildActiveObjList(CAOIModel *ModelPtr, bool ActiveOnly);	
	void                       AddActiveObject(const TActiveObj &ActiveObj, bool Check);
	//-------------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//-------------------------------------------------------------------------------------//
	void                       PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	void                       SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	//-------------------------------------------------------------------------------------//
	void                       PreInitImageBuffer();//預先影像記憶體
	void                       ReleaseImageBuffer();//釋放影像記憶體	
	void                       BuildShowImageBuffer();//建立顯示的影像記憶體
	void                       ReleaseShowImageBuffer();//釋放顯示影像記憶體	
	//-------------------------------------------------------------------------------------//	
	bool                       GetKeepImageOffset() const;	//取得是否保持影像偏移值
	void                       SetKeepImageOffset(bool val);//設定是否保持影像偏移值	
	//---------------------------------------------------------------------------------//	
	MANIPULATE_MODEL_MODE      GetManipulateModelModeDefault(); //操作模組模式	- 預設
	//---------------------------------------------------------------------------------//	
	bool                       GetStageSelectRegion(TREGION4D &StageRgn);//取得選取區域-機台座標
	bool                       GetCurrentFrame(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//取得目前影像
	bool                       GetFrameImage(unsigned int Index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//取得目前影像
	int                        GetMaxFrameCount();
	bool                       AdjustCurrentFrames();
	bool                       FillCurrentFrames(double Ratio);	
	void                       BackupViewParam();//備份顯示參數
	void                       RestoreViewParam();//恢復顯示參數
	void                       CalcFovPosition();
	void                       ResetImageOffset();//復歸顯示移動值	
	//-------------------------------------------------------------------------------------//	
	void                       SwitchFrameImage();
	void                       UpdateFrameImage();
	//-------------------------------------------------------------------------------------//	
	bool                       ExecMoveToStage();
	bool                       ExecShowWndPosition();
	bool                       ExecGrabFov(double PosX, double PosY, double PosZ);
	bool                       ExecUpdateFov(double PosX, double PosY, double PosZ);	
	//-------------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	void                       CreateBKImage();	
	void                       CreateMapImage(bool bTestMap=false);
	void                       CreateTestMapImage();
	void                       DrawFd(HDC hDC);	
	void                       DrawModel(HDC hDC);
	void                       DrawImage(HDC hDC);	
	void                       DrawProjectMap(HDC hDC, bool bTestMap);	
	void                       DrawAddRect(HDC hDC);	
	void                       DrawEditRect(HDC hDC);	
	void                       DrawBoxInfo(HDC hDC);
	void                       DrawCrosshair(HDC hDC);//十字線
	void                       DrawObjectList(HDC hDC);	
	void                       DrawModelActivedLine(HDC hDC);//繪製選取交線
	int                        GetEditLineSize();//取得編輯線的尺寸
	int                        GetEditCheckSize();//取得編輯線比較的尺寸
	int                        GetEditCheckStageSize();//取得編輯線比較的尺寸
	DRAW_MODEL_MODE            GetDrawModelMode() const;
	//-------------------------------------------------------------------------------------//
	void                       SetDrawAddRect(bool Draw);
	void                       SetDrawEditRect(bool Draw);
	CURSOR_POS_MODE            CheckCursorPosModeFd(POINT pt);//確認鼠標座標模式
	CURSOR_POS_MODE            CheckCursorPosModeEdit(POINT pt);//確認鼠標座標模式
	//-------------------------------------------------------------------------------------//
	bool                       ExecAddFd();
	bool                       ExecAddFd2();	
	//-------------------------------------------------------------------------------------//
	bool                       ResetFdModel();
	void                       SetModel(CAOIModel *Ptr);
	void                       UpdateFdModelStats();
	void                       SendOutUpdatePartList(int UpdateList=MSG_MODE_UPDATE, int UpdateWnd=MSG_MODE_UPDATE);	
	//-------------------------------------------------------------------------------------//
	bool                       GetShowPopupMenu() const;
	void                       SetShowPopupMenu(bool val);	
	//-------------------------------------------------------------------------------------//
	bool                       ExecSelectFd();
	bool                       ExecDeleteFdSelected();
	bool                       ExecMoveToComponent(CAOIComponent *ComponentPtr);
	bool                       ExecCoordinateCalibration(int nComponents);//執行座標校正	

	bool                       ExecMoveSelectedFd();
	bool                       ExecResizeSelectedFd(CURSOR_POS_MODE CursorMode);
	bool                       ExecModifySelectedFdFinish();	

	bool                       ExecMoveSelectedEdit();
	bool                       ExecResizeSelectedEdit(CURSOR_POS_MODE CursorMode);
	
	void                       ResetActiveObjPosFocus();
	void                       ResetActiveObjPosSelect();
	void                       CheckActiveObjFocus(TActiveObj &Obj);
	void                       CheckActiveObjFocus(TActiveObj &Obj, CAOIBox &Box);
	bool                       ExecModifyActiveObjPos();//執行選中物件的座標
	bool                       ExecModifyActiveObjPosKernel(int nWndPx, int nWndPy);//執行選中物件的座標
	bool                       ExecModifyActiveObjSize(CURSOR_POS_MODE CursorMode);//執行選中物件的尺寸

	bool                       ExecInspection_Finish();
	bool                       ExecOnlineInspection_Finish();
	bool                       ExecInspectionFinishKernel(bool bOnline);
	//-------------------------------------------------------------------------------------//	
	bool                       UpdateImageByAlgParam();//依據演算法更新畫面
	bool                       ExecCalcWndColor();//計算檢測框顏色
	bool                       ExecExtractWndColorFilter();//取得檢測框抽色參數
	bool                       ExecGetWndColorFilter(CColorRGBV &rgbv);//取得檢測框抽色參數
	bool                       ExecGatherColorFilter(bool CombineColorMode);//吸取抽色參數
	void                       ExecAlgImage(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam);	
	void                       ExecAlgImage_Field(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam);	
	void                       ExecAlgImage_Model(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam);	
	bool                       ExecModelWndInspection(bool UpdateUI);
	bool                       ExecModelBarcodeInspect();		
	bool                       BuildModelUniFrameList(CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign, bool bClone, bool bNoFilter);
	bool                       BuildModelWndUniFrameList(CAOIModel *ModelPtr,CAOIWnd *WndPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign);
	//-------------------------------------------------------------------------------------//		
	bool                       LockUIWnd(bool bLock);//鎖住視窗
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	//---------------------------------------------------------------------------------//
	void                       ExecToggleEnhanceImageMode();
	//---------------------------------------------------------------------------------//
	bool                       ExecSaveLogLButtonUp();
	//---------------------------------------------------------------------------------//
// Implementation
protected:
	virtual ~CEditFdView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditFdView)
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
	afx_msg void OnFiducialEditAddMode();
	afx_msg void OnUpdateFiducialEditAddMode(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditEditMode();
	afx_msg void OnUpdateFiducialEditEditMode(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditCalibration();
	afx_msg void OnUpdateFiducialEditCalibration(CCmdUI* pCmdUI);
	afx_msg void OnFiducialPasteToOtherBoards();
	afx_msg void OnUpdateFiducialPasteToOtherBoards(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditDelSelected();
	afx_msg void OnUpdateFiducialEditDelSelected(CCmdUI* pCmdUI);	
	afx_msg void OnFiducialEditAlignPanel();
	afx_msg void OnUpdateFiducialEditAlignPanel(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignPanelCombo();
	afx_msg void OnUpdateFiducialEditAlignPanelCombo(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignBoard();
	afx_msg void OnUpdateFiducialEditAlignBoard(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignBoardCombo();
	afx_msg void OnUpdateFiducialEditAlignBoardCombo(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetBoardCombo1();
	afx_msg void OnUpdateFiducialEditAlignSetBoardCombo1(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetBoardCombo2();
	afx_msg void OnUpdateFiducialEditAlignSetBoardCombo2(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetBoardCombo3();
	afx_msg void OnUpdateFiducialEditAlignSetBoardCombo3(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetBoardCombo4();
	afx_msg void OnUpdateFiducialEditAlignSetBoardCombo4(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetComponentCombo1();
	afx_msg void OnUpdateFiducialEditAlignSetComponentCombo1(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetComponentCombo2();
	afx_msg void OnUpdateFiducialEditAlignSetComponentCombo2(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetComponentCombo3();
	afx_msg void OnUpdateFiducialEditAlignSetComponentCombo3(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetComponentCombo4();
	afx_msg void OnUpdateFiducialEditAlignSetComponentCombo4(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetComponent1();
	afx_msg void OnUpdateFiducialEditAlignSetComponent1(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetComponent2();
	afx_msg void OnUpdateFiducialEditAlignSetComponent2(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetComponent3();
	afx_msg void OnUpdateFiducialEditAlignSetComponent3(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignSetComponent4();
	afx_msg void OnUpdateFiducialEditAlignSetComponent4(CCmdUI* pCmdUI);	
	afx_msg void OnFiducialEditAlign();
	afx_msg void OnUpdateFiducialEditAlign(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignFdAll();
	afx_msg void OnUpdateFiducialEditAlignFdAll(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditAlignFdSelected();
	afx_msg void OnUpdateFiducialEditAlignFdSelected(CCmdUI* pCmdUI);
	afx_msg void OnFiducialEditPropertyWnd();
	afx_msg void OnUpdateFiducialEditPropertyWnd(CCmdUI* pCmdUI);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITFDVIEW_H__4AC51E10_360B_4A9A_A6D4_C6A1235451DE__INCLUDED_)
