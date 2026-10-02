#if !defined(AFX_MODELPROPERTYWND_H__28FE56D9_98BF_44B8_A0D1_9C8B8A24353E__INCLUDED_)
#define AFX_MODELPROPERTYWND_H__28FE56D9_98BF_44B8_A0D1_9C8B8A24353E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelPropertyWnd.h : header file
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_13     CListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CModelPropertyWnd dialog
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
class CModelPropertyWnd : public CBaseDialog
{
// Construction
public:
	CModelPropertyWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelPropertyWnd)
	enum { IDD = IDD_MODEL_PROPERTY_WND };
	CStatic   m_ImageWnd;
	CComboBox m_LandGroupCombox;
	CComboBox m_LandTypeCombox;
	CThisListCtrl_13 m_LandListCtrl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelPropertyWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


public:
	//---------------------------------------------------------------------------------//
	void                       SetModelPtr(CAOIModel *Ptr);
	bool                       SetUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList);	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	double                     m_ImageZoom;
	TPOINT2D                   m_ImageOffset;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;
	TPOINT2D                   m_ImageResolution;//影像解析度	
	TPOINT2D                   m_ModelImagePosStage;//影像中心在機台的位置
	//---------------------------------------------------------------------------------//
	unsigned int               m_ImageIndex;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;	
	TUNI_FRAME                 m_UniFrameList[FRAME_MAX_COUNT];	
	unsigned int               GetMaxFrameCount();
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_SpaceW;
	IMAGE_SIZE                 m_SpaceH;
	IMAGE_SIZE                 m_SpaceStep;
	IMAGE_SIZE                 m_SpaceBitCnt;	
	MASK_PTR                   m_MaskPtr;
	SPACE_PTR                  m_SpacePtr;	
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
	bool                       m_StopLandListBeSelected;
	//---------------------------------------------------------------------------------//
	CAOIBox                    m_ActiveBox;
	TActiveObj                 m_ActiveObj;
	std::vector<TActiveObj>    m_ActiveObjList;
	//---------------------------------------------------------------------------------//
	CAOIModel                 *m_ModelPtr;	
	CAOIModel                 *GetModelPtr();
	//---------------------------------------------------------------------------------//	
	double                     m_ExtendX;
	double                     m_ExtendY;
	bool                       m_ExtendAutoAdjust;
	//---------------------------------------------------------------------------------//	
	double                     m_BodySizeX;
	double                     m_BodySizeY;
	double                     m_BodyHeight;
	TLandProperty              m_LandProperty;
	std::vector<TLandProperty> m_LandPropertyList;
	//---------------------------------------------------------------------------------//			
	RECT                       m_RoiRect;
	RECT                       m_ImageRoiRect;
	bool                       m_DrawRoiRect;	
	bool                       m_BuidlRoiRect;
	double                     m_RoiRectHeight;
	POINT                      m_MousePosLast;//滑鼠座標-上一個
	POINT                      m_MousePosFirst;//滑鼠座標-第1個
	POINT                      m_MousePosCurrent;//滑鼠座標-現今
	POINT                      m_MousePosImageWnd;//滑鼠座標-圖像視窗
	CURSOR_POS_MODE            m_MousePosMode;//滑鼠座標模式
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       CreateShowBuffer();
	bool                       ReleaseShowImageBuffer();
	bool                       SwitchFrameImage();
	bool                       UpdateFrameImage();			
	bool                       BuildShowImageBuffer();
	//---------------------------------------------------------------------------------//	
	bool                       PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2);
	MANIPULATE_MODEL_MODE      GetManiModelMode() const;
	int                        GetEditLineSize();//取得編輯線的尺寸
	int                        GetEditCheckSize();//取得編輯線比較的尺寸
	//---------------------------------------------------------------------------------//	
	bool                       BuildModelProperty();	
	bool                       BuildLandGroupCombox();
	bool                       UpdateModelPropertyToUI();
	bool                       ExecSelchangeLandGroupCombo();
	bool                       UpdateLandUIVisible(size_t LandCount, LAND_TYPE LandType);
	bool                       UpdateLandPropertyToUI(const TLandProperty &LandProperty);
	bool                       UpdateLandPropertyToKernel(const TLandProperty &LandProperty);
	//---------------------------------------------------------------------------------//	
	bool                       BuildLandListWndHeader();
	bool                       ClearLandListWnd();
	bool                       BuildLandListWnd(int LandGroupID);
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
	bool                       CreateBKImage();
	bool                       DrawRect(HDC hDC);
	bool                       ShowCursorInfo(HDC hDC);
	bool                       DrawModel(HDC hDC, RECT &WndRect);	
	//---------------------------------------------------------------------------------//	
	bool                       CalcRoiRectHeight();
	bool                       GetRoiRectParam(double &SizeX, double &SizeY, double &SizeZ);
	bool                       ExecSelectModelLand(unsigned int LandIndex);
	//---------------------------------------------------------------------------------//	
	bool                       ExecSaveLogLButtonUp();
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelPropertyWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSelchangeLandGroupCombo();
	afx_msg void OnPaint();
	afx_msg void OnItemchangedLandListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);		
	virtual void OnOK();
	afx_msg void OnBodySizeXYZBtn();
	afx_msg void OnLeadSizeXYZBtn();
	afx_msg void OnLeadTipSizeXYZBtn();
	afx_msg void OnLeadShoulderSizeXYZBtn();
	afx_msg void OnKillfocusBodySizeXEdit();
	afx_msg void OnKillfocusBodySizeYEdit();
	afx_msg void OnKillfocusBodySizeZEdit();
	afx_msg void OnKillfocusLeadSizeXEdit();
	afx_msg void OnKillfocusLeadSizeYEdit();
	afx_msg void OnKillfocusLeadSizeZEdit();
	afx_msg void OnKillfocusLeadTipSizeXEdit();
	afx_msg void OnKillfocusLeadTipSizeYEdit();
	afx_msg void OnKillfocusLeadTipSizeZEdit();
	afx_msg void OnKillfocusLeadShoulderSizeXEdit();
	afx_msg void OnKillfocusLeadShoulderSizeYEdit();
	afx_msg void OnKillfocusLeadShoulderSizeZEdit();
	afx_msg void OnShowLandLineChk();
	afx_msg void OnKillfocusExtendXEdit();
	afx_msg void OnKillfocusExtendYEdit();
	afx_msg void OnExtendAutoAdjustChk();
	afx_msg void OnLandPadAlignChk();	
	afx_msg void OnLandPartAlignChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELPROPERTYWND_H__28FE56D9_98BF_44B8_A0D1_9C8B8A24353E__INCLUDED_)
