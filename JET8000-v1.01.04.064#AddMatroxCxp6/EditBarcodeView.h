#if !defined(AFX_EDITBARCODEVIEW_H__F1247969_4A49_4FAE_9E15_AD9CA8F36C6F__INCLUDED_)
#define AFX_EDITBARCODEVIEW_H__F1247969_4A49_4FAE_9E15_AD9CA8F36C6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditBarcodeView.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditBarcodeView view

class CEditBarcodeView : public CView
{
protected:
	CEditBarcodeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditBarcodeView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditBarcodeView)
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
	CAOIBox                    m_ActiveBox;
	TActiveObj                 m_ActiveObj;
	std::vector<TActiveObj>    m_ActiveObjList;
	//-------------------------------------------------------------------------------------//	
	void                       CloseProject();	
	void                       SwitchProject();//切換專案
	CAOIProject*               GetActiveProject();	
	//-------------------------------------------------------------------------------------//
	CAOIModel*                 GetModelPtr();		
	CAOIBarcode*               GetModelBarcodePtr();
	//-------------------------------------------------------------------------------------//	
	bool                       UpdateBarcodeSelected();
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
	void                       PreInitUniFrameBuffer();//預先影像記憶體
	void                       ReleaseUniFrameBuffer();//釋放影像記憶體	
	void                       BuildShowImageBuffer();//建立顯示的影像記憶體
	void                       ReleaseShowImageBuffer();//釋放顯示影像記憶體	
	//-------------------------------------------------------------------------------------//	
	bool                       GetKeepImageOffset() const;	//取得是否保持影像偏移值
	void                       SetKeepImageOffset(bool val);//設定是否保持影像偏移值	
	//---------------------------------------------------------------------------------//	
	MANIPULATE_MODEL_MODE      GetManipulateModelModeDefault(); //操作模組模式	- 預設
	//-------------------------------------------------------------------------------------//		
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
	void                       DrawModel(HDC hDC);
	void                       DrawImage(HDC hDC);
	void                       DrawProjectMap(HDC hDC, bool bTestMap);
	void                       DrawAddRect(HDC hDC);	
	void                       DrawBoxInfo(HDC hDC);
	void                       DrawBarcode(HDC hDC);	
	void                       DrawCrosshair(HDC hDC);//十字線
	void                       DrawObjectList(HDC hDC);	
	void                       DrawModelActivedLine(HDC hDC);//繪製選取交線
	int                        GetEditLineSize();//取得編輯線的尺寸
	int                        GetEditCheckSize();//取得編輯線比較的尺寸
	DRAW_MODEL_MODE            GetDrawModelMode() const;
	//-------------------------------------------------------------------------------------//
	void                       SetDrawAddRect(bool Draw);
	void                       SetDrawEditRect(bool Draw);
	CURSOR_POS_MODE            CheckCursorPosModeBarcode(POINT pt);//確認鼠標座標模式
	//-------------------------------------------------------------------------------------//
	bool                       ExecAddBarcode();
	//-------------------------------------------------------------------------------------//
	bool                       ResetBarcodeModel();
	void                       SetModel(CAOIModel *Ptr);
	void                       UpdateBarcodeModelStats();
	void                       SendOutUpdatePartList(int UpdateList=MSG_MODE_UPDATE, int UpdateWnd=MSG_MODE_UPDATE);	
	//-------------------------------------------------------------------------------------//
	bool                       GetShowPopupMenu() const;
	void                       SetShowPopupMenu(bool val);	
	//-------------------------------------------------------------------------------------//
	bool                       ExecSelectBarcode();		
	bool                       ExecDeleteBarcodeAll();
	bool                       ExecDeleteBarcodeOthers();
	bool                       ExecDeleteBarcodeOtherWnds();
	bool                       ExecDeleteBarcodeSelected();	
	bool                       ExecDeleteBarcodeSelectedWnd();	
	bool                       ExecPasteBarcodeToOtherBoards();
	bool                       ExecPasteBarcodeToOtherPanels();
	bool                       ExecAddBarcodeWnd();
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
	virtual ~CEditBarcodeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditBarcodeView)
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
	afx_msg void OnSoftwareBarcodeAddMode();
	afx_msg void OnUpdateSoftwareBarcodeAddMode(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeEditMode();
	afx_msg void OnUpdateSoftwareBarcodeEditMode(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeDelSelected();
	afx_msg void OnUpdateSoftwareBarcodeDelSelected(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeDelSelectedWnd();
	afx_msg void OnUpdateSoftwareBarcodeDelSelectedWnd(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeDelOthers();
	afx_msg void OnUpdateSoftwareBarcodeDelOthers(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeDelOtherWnds();
	afx_msg void OnUpdateSoftwareBarcodeDelOtherWnds(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodePasteToOtherBoards();
	afx_msg void OnUpdateSoftwareBarcodePasteToOtherBoards(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodePasteToOtherPanels();
	afx_msg void OnUpdateSoftwareBarcodePasteToOtherPanels(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeAddBarcodeWnd();
	afx_msg void OnUpdateSoftwareBarcodeAddBarcodeWnd(CCmdUI* pCmdUI);		
	afx_msg void OnSoftwareBarcodeInspectAll();
	afx_msg void OnUpdateSoftwareBarcodeInspectAll(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeInspectSelected();
	afx_msg void OnUpdateSoftwareBarcodeInspectSelected(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeDelAll();
	afx_msg void OnUpdateSoftwareBarcodeDelAll(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodeAlignFiducial();
	afx_msg void OnUpdateSoftwareBarcodeAlignFiducial(CCmdUI* pCmdUI);
	afx_msg void OnSoftwareBarcodePropertyWnd();
	afx_msg void OnUpdateSoftwareBarcodePropertyWnd(CCmdUI* pCmdUI);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITBARCODEVIEW_H__F1247969_4A49_4FAE_9E15_AD9CA8F36C6F__INCLUDED_)
