#if !defined(AFX_EDITMODELVIEW_H__80BBFA84_4333_4423_B24D_796BA654D93F__INCLUDED_)
#define AFX_EDITMODELVIEW_H__80BBFA84_4333_4423_B24D_796BA654D93F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditModelView.h : header file
//
//-------------------------------------------------------------------------------------//
#include "AOIModel.h"
#include "JetMemDC.h"
#include "ColorRGBVWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditModelView view
//-------------------------------------------------------------------------------------//
class CEditModelView : public CView
{
protected:
	CEditModelView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditModelView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditModelView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
// Implementation
protected:
	virtual ~CEditModelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//---------------------------------------------------------------------------------//	
	bool                       PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2);
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;
	CAOIModel                 *m_ModelPtr;
	double                     m_MapZoom;
	double                     m_ImageZoom;		
	TPOINT2D                   m_ImageOffset;
	bool                       m_DrawAddRect;
	bool                       m_KeepImageOffset;
	bool                       m_ChangeComponentSelected;		
	CAOIBox                    m_ActiveBox;
	TActiveObj                 m_ActiveObj;
	std::vector<TActiveObj>    m_ActiveObjList;
	COLORREF                   m_BkColor;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMapDC;//專案底圖的DC
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;	
	
	double                     m_FovRatio;//FOV的比例
	unsigned int               m_ImageIndex;		
	DWORD                      m_UpdateTestMapTickCount;//更新底圖時間點
	//---------------------------------------------------------------------------------//
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
	POINT                      m_MousePosLast;//滑鼠座標-上一個
	POINT                      m_MousePosFirst;//滑鼠座標-第1個
	POINT                      m_MousePosCurrent;//滑鼠座標-現今
	POINT                      m_MousePosImageWnd;//滑鼠座標-圖像視窗
	CURSOR_POS_MODE            m_MousePosMode;//滑鼠座標模式
	bool                       m_ShowPopupMenu;
	bool                       CheckMousePosMoved() const;//確認滑鼠移動過
	//---------------------------------------------------------------------------------//		
	void                       CloseProject();	
	void                       SwitchProject();
	CAOIProject*               GetActiveProject();
	void                       SwitchFrameImage();
	void                       UpdateFrameImage();		
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	void                       SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	void                       SendOutUpdatePartList(int UpdateList=MSG_MODE_UPDATE, int UpdateWnd=MSG_MODE_UPDATE);
	BOOL                       CheckInEditMode(bool bCheckType);//確認是否可以編輯	
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	void                       CreateBKImage();
	void                       CreateMapImage(bool bTestMap=false);
	void                       CreateTestMapImage();
	void                       DrawModel(HDC hDC);	
	void                       DrawImage(HDC hDC);
	void                       DrawProjectMap(HDC hDC, bool bTestMap);
	void                       DrawAddRect(HDC hDC);
	void                       DrawBoxInfo(HDC hDC);
	void                       DrawComponent(HDC hDC);	
	void                       DrawTempModel(HDC hDC);	
	void                       DrawCrosshair(HDC hDC);//十字線
	void                       DrawObjectList(HDC hDC);		
	void                       DrawModelActivedLine(HDC hDC);//繪製選取交線		
	int                        GetEditLineSize();//取得編輯線的尺寸
	int                        GetEditCheckSize();//取得編輯線比較的尺寸
	DRAW_MODEL_MODE            GetDrawModelMode() const;
	void                       GetModelDrawParam(TMODEL_DRAW_PARAM &DrawParam);	
	BOOL                       MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt);
	BOOL                       MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt);
	//---------------------------------------------------------------------------------//	
	bool                       ExecModelAddWnd();
	bool                       ExecModelAddLand(LAND_TYPE LandType);
	bool                       ExecModelAutoAddLand(int direc);
	bool                       ExecModelRegionSelect();
	bool                       ExecModelComponentSelect();
	//---------------------------------------------------------------------------------//	
	void                       ExecModelEditWndBandID();
	void                       ExecModelEditLandAlignID();	
	void                       ExecModelEditLandAlignID_v1();	
	void                       ExecModelEditLandAlignID_v2();	
	//---------------------------------------------------------------------------------//	
	void                       ExecModelEditWndGroupID();
	void                       ExecModelEditLandGroupID();
	//---------------------------------------------------------------------------------//	
	void                       ExecModelEditLandIncludePadAlign();	
	void                       ExecModelEditLandIncludePartAlign();	
	//---------------------------------------------------------------------------------//	
	bool                       BuildModel();//建立模組;
	CAOIWnd*                   GetWndPtr();
	CAOIModel*                 GetModelPtr(); 	
	CAOIWndMask*               GetWndMaskPtr();
	bool                       ResetModel();
	void                       SetModel(CAOIModel *Ptr);
	bool                       CreateModelROI(TUNI_FRAME UniFrameList[], const TPOINT2D &StageCP, const TPOINT2D &ImageRes, CAOIModel *ModelPtr, RECT &rect);
	bool                       UpdateComponentSelected();
	bool                       CheckIsLinkMode() const;
	void                       UpdateModelStats();
	void                       ResetActiveObjPosFocus();
	void                       ResetActiveObjPosSelect();
	void                       CheckActiveObjFocus(TActiveObj &Obj);
	void                       CheckActiveObjFocus(TActiveObj &Obj, CAOIBox &Box);
	bool                       ExecModifyActiveObjPos();//執行選中物件的座標
	bool                       ExecModifyActiveObjPosKernel(int nWndPx, int nWndPy);//執行選中物件的座標
	bool                       ExecModifyActiveObjSize(CURSOR_POS_MODE CursorMode);//執行選中物件的尺寸
	//---------------------------------------------------------------------------------//	
	void                       ExecModelEditAddAllWnd(MDW_VERSION Version, UINT WndCmd);
	//---------------------------------------------------------------------------------//	
	void                       ExecModelEditLandCount(int nCountMode);	
	bool                       CalcModelLandCount(CAOIModel *ModelPtr, std::vector<TPOINT2D> &BoxResultList, double &Pitch, double MinScore, int nCountMode);//計算影像中的焊盤數量
	bool                       CalcModelLandCount_Array(CAOIModel *ModelPtr, std::vector<TPOINT2D> &BoxResultList, double &Pitch, double MinScore);//計算影像中的焊盤數量
	bool                       CalcModelLandCount_BGA_DIP(CAOIModel *ModelPtr, std::vector<TPOINT2D> &BoxResultList, double &Pitch, double MinScore);//計算影像中的焊盤數量
	bool                       CalcModelLandCount_Normal(CAOIModel *ModelPtr, std::vector<TPOINT2D> &BoxResultList, double &Pitch, double MinScore);//計算影像中的焊盤數量
	//---------------------------------------------------------------------------------//	
	bool                       GetShowPopupMenu() const;
	void                       SetShowPopupMenu(bool val);	
	bool                       ExecPopupMenu_Auto(POINT point);
	bool                       ExecPopupMenu_Add(POINT point);
	bool                       ExecPopupMenu_Edit(POINT point);
	bool                       ExecPopupMenu(POINT point, UINT menuID);
	//---------------------------------------------------------------------------------//
	MANIPULATE_MODEL_MODE      GetManiModelMode() const;
	void                       SwitchManiModelMode(MANIPULATE_MODEL_MODE ManiMode);	
	//---------------------------------------------------------------------------------//
	void                       UpdateActiveObjList();
	void                       BuildActiveObjList(CAOIModel *ModelPtr, bool ActiveOnly);	
	void                       AddActiveObject(const TActiveObj &ActiveObj, bool Check);
	CURSOR_POS_MODE            CheckCursorPosMode(POINT pt);//確認鼠標座標模式
	//---------------------------------------------------------------------------------//
	bool                       ExecMoveToStage();
	bool                       ExecMoveToComponentStage();
	bool                       ExecShowWndPosition();
	bool                       ExecGrabFov(double PosX, double PosY, double PosZ);
	bool                       ExecUpdateFov(double PosX, double PosY, double PosZ);
	//---------------------------------------------------------------------------------//		
	void                       PreInitUniFrameBuffer();//預先影像記憶體
	void                       ReleaseUniFrameBuffer();//釋放影像記憶體	
	//---------------------------------------------------------------------------------//	
	bool                       GetKeepImageOffset() const;	//取得是否保持影像偏移值
	void                       SetKeepImageOffset(bool val);//設定是否保持影像偏移值	
	//---------------------------------------------------------------------------------//	
	bool                       GetCurrentFrame(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//取得目前影像
	bool                       GetFrameImage(unsigned int Index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//取得目前影像
	int                        GetMaxFrameCount();
	bool                       AdjustCurrentFrames();
	bool                       FillCurrentFrames(double Ratio);		
	void                       BackupViewParam();//備份顯示參數
	void                       RestoreViewParam();//恢復顯示參數
	void                       CalcFovPosition();
	void                       ResetImageOffset();
	bool                       UpdateImageByAlgParam();//依據演算法更新畫面		
	bool                       ExecCalcWndColor();//計算檢測框顏色
	bool                       ExecExtractWndColorFilter();//取得檢測框抽色參數
	bool                       ExecGetWndColorFilter(CColorRGBV &rgbv);//取得檢測框抽色參數
	bool                       ExecGatherColorFilter(bool CombineColorMode);//吸取抽色參數	
	void                       ExecAlgImage(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam);	
	void                       ExecAlgImage_Field(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam);	
	void                       ExecAlgImage_Model(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam);	
	bool                       ExecModelWndInspection(bool UpdateUI);
	bool                       ExecModelComponentInspect();
	bool                       ExecComponentModelToLibrary();//將零件模組更新至資料庫內	
	bool                       BuildModelUniFrameList(CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign, bool bClone, bool bNoFilter);
	bool                       BuildModelWndUniFrameList(CAOIModel *ModelPtr,CAOIWnd *WndPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign);
	//---------------------------------------------------------------------------------//
	void                       BuildShowImageBuffer();//建立顯示的影像記憶體
	void                       ReleaseShowImageBuffer();//釋放顯示影像記憶體	
	//---------------------------------------------------------------------------------//	
	bool                       ExecInspection_Finish();
	bool                       ExecOnlineInspection_Finish();
	bool                       ExecInspectionFinishKernel(bool bOnline);
	//---------------------------------------------------------------------------------//
	bool                       LockUIWnd(bool bLock);//鎖住視窗
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	//---------------------------------------------------------------------------------//
	bool                       ExecSwitchFrame(int FrameIndex);//切換畫面
	//---------------------------------------------------------------------------------//
	bool                       ExecUpdateToLibrary(bool bArrange);
	//---------------------------------------------------------------------------------//
	bool                       ExecSaveDefaultModel();//儲存預設模組 	
	//---------------------------------------------------------------------------------//
	bool                       ExecShowLibraryWnd(bool bShow);//顯示資料庫視窗
	//---------------------------------------------------------------------------------//		
	void                       ExecToggleEnhanceImageMode();
	bool                       ExecModelEditWndShapeMode(BOX_SHAPE_MODE BoxShapeMode);
	bool                       ExecModelEditMaskBoxShapeMode(BOX_SHAPE_MODE BoxShapeMode);
	//---------------------------------------------------------------------------------//
	bool                       ExecSaveLogLButtonUp();
	//---------------------------------------------------------------------------------//
	void                       OpenChart(int type);
	//---------------------------------------------------------------------------------//
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditModelView)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);	
	afx_msg void OnModelEditSwitchOnlineView();
	afx_msg void OnUpdateModelEditSwitchOnlineView(CCmdUI* pCmdUI);
	afx_msg void OnModelEditSwitchEditMain();
	afx_msg void OnUpdateModelEditSwitchEditMain(CCmdUI* pCmdUI);
	afx_msg void OnShowFrame01();
	afx_msg void OnUpdateShowFrame01(CCmdUI* pCmdUI);
	afx_msg void OnShowFrame02();
	afx_msg void OnUpdateShowFrame02(CCmdUI* pCmdUI);
	afx_msg void OnShowFrame03();
	afx_msg void OnUpdateShowFrame03(CCmdUI* pCmdUI);
	afx_msg void OnShowFrame04();
	afx_msg void OnUpdateShowFrame04(CCmdUI* pCmdUI);
	afx_msg void OnShowFrame05();
	afx_msg void OnUpdateShowFrame05(CCmdUI* pCmdUI);
	afx_msg void OnShowFrame06();
	afx_msg void OnUpdateShowFrame06(CCmdUI* pCmdUI);
	afx_msg void OnShowFrame07();
	afx_msg void OnUpdateShowFrame07(CCmdUI* pCmdUI);
	afx_msg void OnShowFrame08();
	afx_msg void OnUpdateShowFrame08(CCmdUI* pCmdUI);
	afx_msg void OnEditModelAdd();
	afx_msg void OnUpdateEditModelAdd(CCmdUI* pCmdUI);
	afx_msg void OnEditModelEdit();
	afx_msg void OnUpdateEditModelEdit(CCmdUI* pCmdUI);
	afx_msg void OnEditModelSelect();
	afx_msg void OnUpdateEditModelSelect(CCmdUI* pCmdUI);
	afx_msg void OnEditModelAutoAdd();
	afx_msg void OnUpdateEditModelAutoAdd(CCmdUI* pCmdUI);	
	afx_msg void OnModelAddLandElectrode();
	afx_msg void OnUpdateModelAddLandElectrode(CCmdUI* pCmdUI);	
	afx_msg void OnModelAddLandICLead();	
	afx_msg void OnUpdateModelAddLandICLead(CCmdUI* pCmdUI);	
	afx_msg void OnModelAddLandConLead();	
	afx_msg void OnUpdateModelAddLandConLead(CCmdUI* pCmdUI);	
	afx_msg void OnModelAddLandPad();
	afx_msg void OnUpdateModelAddLandPad(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditLandCount();
	afx_msg void OnUpdateModelEditLandCount(CCmdUI* pCmdUI);
	afx_msg void OnModelEditLandCount2D();
	afx_msg void OnUpdateModelEditLandCount2D(CCmdUI* pCmdUI);
	afx_msg void OnModelEditLandArray();
	afx_msg void OnUpdateModelEditLandArray(CCmdUI* pCmdUI);
	afx_msg void OnModelEditLandPitch();
	afx_msg void OnUpdateModelEditLandPitch(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditLandAlign();
	afx_msg void OnUpdateModelEditLandAlign(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditLandIncludePadAlign();
	afx_msg void OnUpdateModelEditLandIncludePadAlign(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditLandIncludePartAlign();
	afx_msg void OnUpdateModelEditLandIncludePartAlign(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditClonePaste();
	afx_msg void OnUpdateModelEditClonePaste(CCmdUI* pCmdUI);
	afx_msg void OnModelEditCloneRotate090();
	afx_msg void OnUpdateModelEditCloneRotate090(CCmdUI* pCmdUI);
	afx_msg void OnModelEditCloneRotate180();
	afx_msg void OnUpdateModelEditCloneRotate180(CCmdUI* pCmdUI);
	afx_msg void OnModelEditCloneRotate270();
	afx_msg void OnUpdateModelEditCloneRotate270(CCmdUI* pCmdUI);
	afx_msg void OnModelEditCloneMirrorXPos();
	afx_msg void OnUpdateModelEditCloneMirrorXPos(CCmdUI* pCmdUI);
	afx_msg void OnModelEditCloneMirrorYPos();
	afx_msg void OnUpdateModelEditCloneMirrorYPos(CCmdUI* pCmdUI);
	afx_msg void OnModelEditCloneDiagonal();
	afx_msg void OnUpdateModelEditCloneDiagonal(CCmdUI* pCmdUI);
	afx_msg void OnModelEditCloneCorner4();	
	afx_msg void OnUpdateModelEditCloneCorner4(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDeleteSelect();
	afx_msg void OnUpdateModelEditDeleteSelect(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDeleteOthers();
	afx_msg void OnUpdateModelEditDeleteOthers(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDeleteGroup();
	afx_msg void OnUpdateModelEditDeleteGroup(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDeleteAll();
	afx_msg void OnUpdateModelEditDeleteAll(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDeleteAllWnd();
	afx_msg void OnUpdateModelEditDeleteAllWnd(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDeleteDerivative();
	afx_msg void OnUpdateModelEditDeleteDerivative(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDeleteComponent();
	afx_msg void OnUpdateModelEditDeleteComponent(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDeleteGroupLibrary();
	afx_msg void OnUpdateModelEditDeleteGroupLibrary(CCmdUI* pCmdUI);
	afx_msg void OnModelEditModifyPos();
	afx_msg void OnUpdateModelEditModifyPos(CCmdUI* pCmdUI);
	afx_msg void OnModelEditModifySize();
	afx_msg void OnUpdateModelEditModifySize(CCmdUI* pCmdUI);
	afx_msg void OnModelEditRotate090();
	afx_msg void OnUpdateModelEditRotate090(CCmdUI* pCmdUI);
	afx_msg void OnModelEditRotate180();
	afx_msg void OnUpdateModelEditRotate180(CCmdUI* pCmdUI);
	afx_msg void OnModelEditRotate270();
	afx_msg void OnUpdateModelEditRotate270(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMirrorXPos();
	afx_msg void OnUpdateModelEditMirrorXPos(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMirrorYPos();
	afx_msg void OnUpdateModelEditMirrorYPos(CCmdUI* pCmdUI);
	afx_msg void OnModelEditAlignCenterPos();
	afx_msg void OnUpdateModelEditAlignCenterPos(CCmdUI* pCmdUI);
	afx_msg void OnModelEditAlignCenterPosU();
	afx_msg void OnUpdateModelEditAlignCenterPosU(CCmdUI* pCmdUI);
	afx_msg void OnModelEditAlignCenterPosV();
	afx_msg void OnUpdateModelEditAlignCenterPosV(CCmdUI* pCmdUI);
	afx_msg void OnModelEditWndShapeRectangle();
	afx_msg void OnUpdateModelEditWndShapeRectangle(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditWndShapeRoundRect();
	afx_msg void OnUpdateModelEditWndShapeRoundRect(CCmdUI* pCmdUI);
	afx_msg void OnModelEditWndShapeEllipse();
	afx_msg void OnUpdateModelEditWndShapeEllipse(CCmdUI* pCmdUI);
	afx_msg void OnModelEditWndShapeCapsule();
	afx_msg void OnUpdateModelEditWndShapeCapsule(CCmdUI* pCmdUI);
	afx_msg void OnModelEditWndShapeBullet();
	afx_msg void OnUpdateModelEditWndShapeBullet(CCmdUI* pCmdUI);
	afx_msg void OnModelEditWndShapeHalfRoundRect();
	afx_msg void OnUpdateModelEditWndShapeHalfRoundRect(CCmdUI* pCmdUI);
	afx_msg void OnModelEditWndShapeTShape();
	afx_msg void OnUpdateModelEditWndShapeTShape(CCmdUI* pCmdUI);
	afx_msg void OnModelEditWndShapeParam();
	afx_msg void OnUpdateModelEditWndShapeParam(CCmdUI* pCmdUI);
	afx_msg void OnModelEditWndShapeParam2();
	afx_msg void OnUpdateModelEditWndShapeParam2(CCmdUI* pCmdUI);
	afx_msg void OnModelEditGroupID();
	afx_msg void OnUpdateModelEditGroupID(CCmdUI* pCmdUI);
	afx_msg void OnModelEditBandID();
	afx_msg void OnUpdateModelEditBandID(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditAddAllWnd();
	afx_msg void OnUpdateModelEditAddAllWnd(CCmdUI* pCmdUI);
	afx_msg void OnModelEditAddAllWndv2();
	afx_msg void OnUpdateModelEditAddAllWndv2(CCmdUI* pCmdUI);
	afx_msg void OnModelEditAddAllWndGroup();
	afx_msg void OnUpdateModelEditAddAllWndGroup(CCmdUI* pCmdUI);
	afx_msg void OnTuneAlignFiducial();	
	afx_msg void OnUpdateTuneAlignFiducial(CCmdUI* pCmdUI);		
	afx_msg void OnTuneInspection();
	afx_msg void OnUpdateTuneInspection(CCmdUI* pCmdUI);
	afx_msg void OnTuneSelectedComponent();
	afx_msg void OnUpdateTuneSelectedComponent(CCmdUI* pCmdUI);
	afx_msg void OnTuneSelectedModel();
	afx_msg void OnUpdateTuneSelectedModel(CCmdUI* pCmdUI);
	afx_msg void OnModelEditUpdateToLibrary();
	afx_msg void OnUpdateModelEditUpdateToLibrary(CCmdUI* pCmdUI);
	afx_msg void OnModelEditArrangeModel();
	afx_msg void OnUpdateModelEditArrangeModel(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditShowModelActiveLine();
	afx_msg void OnUpdateModelEditShowModelActiveLine(CCmdUI* pCmdUI);
	afx_msg void OnModelEditUpdateFromLibrary();
	afx_msg void OnUpdateModelEditUpdateFromLibrary(CCmdUI* pCmdUI);
	afx_msg void OnModelEditUpdateToLibraryGroup();
	afx_msg void OnUpdateModelEditUpdateToLibraryGroup(CCmdUI* pCmdUI);
	afx_msg void OnModelEditSaveDefaultModel();
	afx_msg void OnUpdateModelEditSaveDefaultModel(CCmdUI* pCmdUI);
	afx_msg void OnModelEditProjecSave();
	afx_msg void OnUpdateModelEditProjecSave(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditShowAgentList();
	afx_msg void OnUpdateModelEditShowAgentList(CCmdUI* pCmdUI);
	afx_msg void OnModelInspectComponent();
	afx_msg void OnUpdateModelInspectComponent(CCmdUI* pCmdUI);	
	afx_msg void OnShowModelEditMode();
	afx_msg void OnUpdateShowModelEditMode(CCmdUI* pCmdUI);
	afx_msg void OnShowModelResultMode();
	afx_msg void OnUpdateShowModelResultMode(CCmdUI* pCmdUI);
	afx_msg void OnShowModelLands();
	afx_msg void OnUpdateShowModelLands(CCmdUI* pCmdUI);
	afx_msg void OnHideModelLands();
	afx_msg void OnUpdateHideModelLands(CCmdUI* pCmdUI);
	afx_msg void OnShowAllComponents();
	afx_msg void OnUpdateShowAllComponents(CCmdUI* pCmdUI);
	afx_msg void OnShowModelWndIndex();
	afx_msg void OnUpdateShowModelWndIndex(CCmdUI* pCmdUI);
	afx_msg void OnShowModelLandIndex();
	afx_msg void OnUpdateShowModelLandIndex(CCmdUI* pCmdUI);
	afx_msg void OnEditModelBkImage();
	afx_msg void OnUpdateEditModelBkImage(CCmdUI* pCmdUI);
	afx_msg void OnEditModelBkImageAll();
	afx_msg void OnUpdateEditModelBkImageAll(CCmdUI* pCmdUI);
	afx_msg void OnModelEditCloneArrayPaste();
	afx_msg void OnUpdateModelEditCloneArrayPaste(CCmdUI* pCmdUI);
	afx_msg void OnShowModelActivedLine();
	afx_msg void OnUpdateShowModelActivedLine(CCmdUI* pCmdUI);
	afx_msg void OnModelLinkLandWndPos();
	afx_msg void OnUpdateModelLinkLandWndPos(CCmdUI* pCmdUI);
	afx_msg void OnModelLinkLandWndSize();
	afx_msg void OnUpdateModelLinkLandWndSize(CCmdUI* pCmdUI);
	afx_msg void OnModelLinkLandPos();
	afx_msg void OnUpdateModelLinkLandPos(CCmdUI* pCmdUI);
	afx_msg void OnModelLinkLandSize();
	afx_msg void OnUpdateModelLinkLandSize(CCmdUI* pCmdUI);	
	afx_msg void OnTuneRepeateInspection();
	afx_msg void OnUpdateTuneRepeateInspection(CCmdUI* pCmdUI);
	afx_msg void OnTuneSelectedPartNumber();
	afx_msg void OnUpdateTuneSelectedPartNumber(CCmdUI* pCmdUI);
	afx_msg void OnTuneSelectedModelGroup();
	afx_msg void OnUpdateTuneSelectedModelGroup(CCmdUI* pCmdUI);
	afx_msg void OnTuneInspectionGroup();
	afx_msg void OnUpdateTuneInspectionGroup(CCmdUI* pCmdUI);
	afx_msg void OnModelEditModify();
	afx_msg void OnUpdateModelEditModify(CCmdUI* pCmdUI);
	afx_msg void OnModelEditClone();
	afx_msg void OnUpdateModelEditClone(CCmdUI* pCmdUI);
	afx_msg void OnModelEditDelete();
	afx_msg void OnUpdateModelEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxAdd();
	afx_msg void OnUpdateModelEditMaskBoxAdd(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxRotate090();
	afx_msg void OnUpdateModelEditMaskBoxRotate090(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxRotate180();
	afx_msg void OnUpdateModelEditMaskBoxRotate180(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxRotate270();
	afx_msg void OnUpdateModelEditMaskBoxRotate270(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxShapeRectangle();
	afx_msg void OnUpdateModelEditMaskBoxShapeRectangle(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditMaskBoxShapeRoundRect();
	afx_msg void OnUpdateModelEditMaskBoxShapeRoundRect(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxShapeEllipse();
	afx_msg void OnUpdateModelEditMaskBoxShapeEllipse(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxShapeCapsule();
	afx_msg void OnUpdateModelEditMaskBoxShapeCapsule(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxShapeBullet();
	afx_msg void OnUpdateModelEditMaskBoxShapeBullet(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxShapeHalfRoundRect();
	afx_msg void OnUpdateModelEditMaskBoxShapeHalfRoundRect(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxShapeTShape();
	afx_msg void OnUpdateModelEditMaskBoxShapeTShape(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxShapeParam();
	afx_msg void OnUpdateModelEditMaskBoxShapeParam(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxShapeParam2();
	afx_msg void OnUpdateModelEditMaskBoxShapeParam2(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxErase();
	afx_msg void OnUpdateModelEditMaskBoxErase(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxDelete();
	afx_msg void OnUpdateModelEditMaskBoxDelete(CCmdUI* pCmdUI);
	afx_msg void OnModelEditMaskBoxClear();
	afx_msg void OnUpdateModelEditMaskBoxClear(CCmdUI* pCmdUI);
	afx_msg void OnModelEditGlobalCloneWnd();
	afx_msg void OnUpdateModelEditGlobalCloneWnd(CCmdUI* pCmdUI);
	afx_msg void OnModelEditGlobalPasteWnd();
	afx_msg void OnUpdateModelEditGlobalPasteWnd(CCmdUI* pCmdUI);
	afx_msg void OnModelEditShowLibraryWnd();
	afx_msg void OnUpdateModelEditShowLibraryWnd(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditPropertyWnd();
	afx_msg void OnUpdateModelEditPropertyWnd(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditEnableAllWnds();
	afx_msg void OnUpdateModelEditEnableAllWnds(CCmdUI* pCmdUI);	
	afx_msg void OnModelEditDisableAllWnds();
	afx_msg void OnUpdateModelEditDisableAllWnds(CCmdUI* pCmdUI);	
	afx_msg void OnModelAutoAddLandVer();
	afx_msg void OnModelAutoAddLandHor();
	afx_msg void OnModelAutoAddLandBoth();
	afx_msg void OnUpdateModelAutoAddLand(CCmdUI* pCmdUI);
	afx_msg void OnModelSmartChart();
	afx_msg void OnUpdateModelSmartChart(CCmdUI* pCmdUI);
	afx_msg void OnModelDataStatic();
	afx_msg void OnScanDataStatic();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMODELVIEW_H__80BBFA84_4333_4423_B24D_796BA654D93F__INCLUDED_)
