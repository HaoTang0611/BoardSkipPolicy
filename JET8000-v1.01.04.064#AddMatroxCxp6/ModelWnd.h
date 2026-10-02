#if !defined(AFX_MODELWND_H__30B39B13_374F_49FB_9D66_7F4648911AE4__INCLUDED_)
#define AFX_MODELWND_H__30B39B13_374F_49FB_9D66_7F4648911AE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "EditWndView.h"
#include "EditImageView.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelWnd dialog
//-------------------------------------------------------------------------------------//
class CModelWnd : public CBaseDialog
{
// Construction
public:
	CModelWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelWnd)
	enum { IDD = IDD_MODEL_WND };
	CStatic	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelWnd)
	public:	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//		
	void                       SetModelPtr(CAOIModel *Ptr);
	void                       SetActiveProject(CAOIProject *Ptr);
	bool                       SetUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList);	
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	CAOIModel                 *m_ModelPtr;
	CAOIModel                 *GetModelPtr();
	//---------------------------------------------------------------------------------//	
	CAOIProject               *m_ProjectPtr;
	CAOIProject               *GetActiveProject();
	//---------------------------------------------------------------------------------//
	HWND                       m_MainFrameWnd;	
	CEditWndView               m_ViewModelWnd;
	CEditImageView             m_ViewModelImage;
	//---------------------------------------------------------------------------------//	
	CAOIBox                    m_ActiveBox;
	TActiveObj                 m_ActiveObj;
	std::vector<TActiveObj>    m_ActiveObjList;
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ImageIndex;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;	
	
	TUNI_FRAME                 m_UniFrameList[FRAME_MAX_COUNT];	
	unsigned int               GetMaxFrameCount();
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE	               m_ShowImageStep;
	IMAGE_SIZE                 m_ShowBitCount;
	IMAGE_PTR                  m_ShowImagePtr;
	size_t                     m_ShowBufferSize;
	IMAGE_SIZE                 GetFrameImageW() const;
	IMAGE_SIZE                 GetFrameImageH() const;
	//---------------------------------------------------------------------------------//	
	double                     m_ImageZoom;
	TPOINT2D                   m_ImageOffset;
	COLORREF                   m_BkColor;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC1;	
	CJetMemDC                  m_ImageWndMemDC2;		
	TPOINT2D                   m_ImageResolution;//影像解析度	
	TPOINT2D                   m_ModelImagePosStage;//影像中心在機台的位置
	//---------------------------------------------------------------------------------//
	bool                       m_DrawAddRect;
	POINT                      m_MousePosLast;//滑鼠座標-上一個
	POINT                      m_MousePosFirst;//滑鼠座標-第1個
	POINT                      m_MousePosCurrent;//滑鼠座標-現今
	POINT                      m_MousePosImageWnd;//滑鼠座標-圖像視窗
	CURSOR_POS_MODE            m_MousePosMode;//滑鼠座標模式
	//---------------------------------------------------------------------------------//
	void                       AdjustPaneWndPosition();
	bool                       PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2);	
	MANIPULATE_MODEL_MODE      GetManiModelMode() const;
	int                        GetEditLineSize();//取得編輯線的尺寸
	int                        GetEditCheckSize();//取得編輯線比較的尺寸
	void                       SwitchMultiLanguage();
	//---------------------------------------------------------------------------------//	
	bool                       CreateShowBuffer();
	bool                       ReleaseShowImageBuffer();
	bool                       SwitchFrameImage();
	bool                       UpdateFrameImage();			
	bool                       BuildShowImageBuffer();
	//---------------------------------------------------------------------------------//	
	bool                       ExecShowWndPosition();
	//---------------------------------------------------------------------------------//	
	void                       UpdateActiveObjList();
	void                       BuildActiveObjList(CAOIModel *ModelPtr, bool ActiveOnly);	
	void                       AddActiveObject(const TActiveObj &ActiveObj, bool Check);
	CURSOR_POS_MODE            CheckCursorPosMode(POINT pt);//確認鼠標座標模式
	void                       ResetActiveObjPosFocus();
	void                       ResetActiveObjPosSelect();
	void                       CheckActiveObjFocus(TActiveObj &Obj);
	void                       CheckActiveObjFocus(TActiveObj &Obj, CAOIBox &Box);
	bool                       ExecModifyActiveObjPos();//執行選中物件的座標
	bool                       ExecModifyActiveObjPosKernel(int nWndPx, int nWndPy);//執行選中物件的座標
	bool                       ExecModifyActiveObjSize(CURSOR_POS_MODE CursorMode);//執行選中物件的尺寸
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	void                       DrawModel(HDC hDC);
	void                       DrawAddRect(HDC hDC);
	void                       CreateBKImage();
	//---------------------------------------------------------------------------------//
	bool                       ExecModelWndInspection(bool UpdateUI);
	bool                       ExecModelComponentInspect();
	void                       UpdateImageByAlgParam();//依據演算法更新畫面
	bool                       ExecCalcWndColor();//計算檢測框顏色
	bool                       ExecExtractWndColorFilter();//取得檢測框抽色參數
	bool                       ExecGetWndColorFilter(CColorRGBV &rgbv);//取得檢測框抽色參數
	bool                       ExecGatherColorFilter(bool CombineColorMode);//吸取抽色參數	
	void                       ExecAlgImage(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam);
	bool                       BuildModelUniFrameList(CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign, bool bClone, bool bNoFilter);
	bool                       GetCurrentFrame(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//取得目前影像
	bool                       GetFrameImage(unsigned int Index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//取得目前影像
	//---------------------------------------------------------------------------------//	
	bool                       ExecSaveLogLButtonUp();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTestModeBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELWND_H__30B39B13_374F_49FB_9D66_7F4648911AE4__INCLUDED_)
