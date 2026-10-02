#if !defined(AFX_IMAGECOMBINEWND_H__325953AE_7450_4993_9ACF_CF22D81CCF91__INCLUDED_)
#define AFX_IMAGECOMBINEWND_H__325953AE_7450_4993_9ACF_CF22D81CCF91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageCombineWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
enum COMBINE_MAP_MODE
{
	COMBINE_MAP_BY_RIGHT = 1,
	COMBINE_MAP_BY_LEFT  = 2,
	COMBINE_MAP_RETURN
};
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImageCombineWnd dialog
//-------------------------------------------------------------------------------------//
class CImageCombineWnd : public CBaseDialog
{
// Construction
public:
	CImageCombineWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageCombineWnd)
	enum { IDD = IDD_IMAGE_COMBINE_WND };
	CStatic	m_ImageWnd;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageCombineWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	bool                       BuildMapBuffer();
	void                       ClearMapBuffer();
	void                       GetMapResultRect(RECT &Rect);
	void                       GetMapResultRect_R(RECT &Rect);
	void                       GetMapResultRect_L(RECT &Rect);
	COMBINE_MAP_MODE           GetCombineMapMode() const;
	void                       SetCombineMapMode(COMBINE_MAP_MODE Mode);	
	bool                       GetMapBuffer(IMAGE_SIZE W[], IMAGE_SIZE H[], IMAGE_SIZE Step[], IMAGE_SIZE Bit[], IMAGE_PTR Ptr[], size_t Cnt);
	bool                       SetMapInfo_R(TREGION4D &Rgn, TPOINT2D &Res, IMAGE_SIZE W[], IMAGE_SIZE H[], IMAGE_SIZE Step[], IMAGE_SIZE Bit[], IMAGE_PTR Ptr[], size_t Cnt);
	bool                       SetMapInfo_L(TREGION4D &Rgn, TPOINT2D &Res, IMAGE_SIZE W[], IMAGE_SIZE H[], IMAGE_SIZE Step[], IMAGE_SIZE Bit[], IMAGE_PTR Ptr[], size_t Cnt);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	size_t                     GetMaxCount() const;		
	void                       ClearShowBuffer();
	void                       ClearMapBuffer_R();
	void                       ClearMapBuffer_L();
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//		
	COMBINE_MAP_MODE           m_CombineMapMode;
	//---------------------------------------------------------------------------------//		
	double                     m_ZoomScale;	
	TPOINT2D                   m_ViewOffset;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;
	//---------------------------------------------------------------------------------//
	POINT                      m_LastPos;
	POINT                      m_MovingPos;
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;	
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	//---------------------------------------------------------------------------------//
	unsigned int               m_MapIndex;
	IMAGE_SIZE                 m_MapW[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_MapH[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_BitCount[FRAME_MAX_COUNT];	
	IMAGE_SIZE                 m_MapStep[FRAME_MAX_COUNT];
	IMAGE_PTR                  m_MapBuffer[FRAME_MAX_COUNT];	
	size_t                     m_MapSize[FRAME_MAX_COUNT];
	RECT                       m_MapResultRect;//結果位置
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ShowW;
	IMAGE_SIZE                 m_ShowH;
	IMAGE_SIZE                 m_ShowStep;
	IMAGE_SIZE                 m_ShowBitCnt;	
	IMAGE_PTR                  m_ShowBuffer;
	size_t                     m_ShowSize;
	BITMAPINFO                *m_ShowInfoPtr;
	//---------------------------------------------------------------------------------//	
	bool                       m_MapLoad_R;
	POINT                      m_MapPos_R;
	RECT                       m_MapRect_R;
	TREGION4D                  m_MapStageRgn_R;////專案檢測底圖範圍
	TPOINT2D                   m_MapResolution_R;
	RECT                       m_MapResultRect_R;//結果位置
	IMAGE_SIZE                 m_MapW_R[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_MapH_R[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_BitCount_R[FRAME_MAX_COUNT];	
	IMAGE_SIZE                 m_MapStep_R[FRAME_MAX_COUNT];
	IMAGE_PTR                  m_MapBuffer_R[FRAME_MAX_COUNT];	
	size_t                     m_MapSize_R[FRAME_MAX_COUNT];	
	//---------------------------------------------------------------------------------//
	bool                       m_MapLoad_L;
	POINT                      m_MapPos_L;
	RECT                       m_MapRect_L;
	TREGION4D                  m_MapStageRgn_L;////專案檢測底圖範圍
	TPOINT2D                   m_MapResolution_L;
	RECT                       m_MapResultRect_L;//結果位置
	IMAGE_SIZE                 m_MapW_L[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_MapH_L[FRAME_MAX_COUNT];
	IMAGE_SIZE                 m_BitCount_L[FRAME_MAX_COUNT];	
	IMAGE_SIZE                 m_MapStep_L[FRAME_MAX_COUNT];
	IMAGE_PTR                  m_MapBuffer_L[FRAME_MAX_COUNT];	
	size_t                     m_MapSize_L[FRAME_MAX_COUNT];		
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ExecSaveMapR();
	bool                       ExecSaveMapL();
	bool                       ExecLoadMapR();
	bool                       ExecLoadMapL();
	bool                       ExecAutoMatch();
	bool                       ExecMoveMap(int dx, int dy);
	bool                       ExecMoveMapR(int dx, int dy);
	bool                       ExecMoveMapL(int dx, int dy);
	//---------------------------------------------------------------------------------//
	bool                       ExecMergeImage();
	bool                       PasteToMap(size_t index, IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, IMAGE_PTR Ptr, const POINT &Pos, RECT &Rect);
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();	
	void                       CreateBKImage();
	void                       BuildShowImage(size_t index);
	//---------------------------------------------------------------------------------//
	void                       OnImageWndNotify(WPARAM wParam, LPARAM lParam);
	void                       OnImageWndMouseMove();
	void                       OnImageWndLButtonUp();
	void                       OnImageWndLButtonDown();	
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImageCombineWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();	
	afx_msg void OnCombineImageBtn();
	afx_msg void OnLoadImageBtn1();
	afx_msg void OnLoadImageBtn2();
	afx_msg void OnAutoMatchImageBtn();
	afx_msg void OnShowRectLineChk();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_IMAGECOMBINEWND_H__325953AE_7450_4993_9ACF_CF22D81CCF91__INCLUDED_)

