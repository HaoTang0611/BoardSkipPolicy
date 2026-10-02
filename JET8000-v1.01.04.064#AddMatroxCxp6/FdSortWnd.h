#if !defined(AFX_FDSORTWND_H__37FAB498_EE57_4C01_9B74_DCB4A7EDEAB3__INCLUDED_)
#define AFX_FDSORTWND_H__37FAB498_EE57_4C01_9B74_DCB4A7EDEAB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FdSortWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
//-------------------------------------------------------------------------------------//
enum FD_SORT_SCOPE_MODE//定位點排序範疇模式
{
	FD_SORT_SCOPE_PANEL = 1,//定位點排序範疇-整板
	FD_SORT_SCOPE_BOARD = 2,//定位點排序範疇-單板
	FD_SORT_SCOPE_RETURN
};
//-------------------------------------------------------------------------------------//
enum FD_SORT_SEARCH_MODE//定位點排序尋找排序模式
{
	FD_SORT_SEARCH_NONE,
	FD_SORT_SEARCH_LEFT,   //最左邊
	FD_SORT_SEARCH_TOP,    //最上面
	FD_SORT_SEARCH_RIGHT,  //最左邊
	FD_SORT_SEARCH_BOTTOM, //最下面
	FD_SORT_SEARCH_RETURN
};
//-------------------------------------------------------------------------------------//
enum FD_SORT_PICK_MODE
{
	FD_SORT_PICK_NONE, 
	FD_SORT_PICK_ENABLE,
	FD_SORT_PICK_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagFdSort
{	
	CAOIFd        *FdPtr;
	unsigned int   uFdIndex;	
	bool           bCounted;//數過
	bool           bVisibled;//顯示
	bool           bSelected;//選取
	bool           bDelected;//刪除
	bool           bIncluded;//納入計算
	TRECT4D        rcFdMapRect;	
	int            nFdSortIndex;//定位點排序編號
	int            nFdSortIndexBefore;//定位點排序編號
	tagFdSort()
	{
		FdPtr = NULL;		
		uFdIndex = -1;
		bCounted = false;
		bVisibled = true;
		bSelected = false;
		bDelected = false;
		bIncluded = false;
		rcFdMapRect = TRECT4D();	
		nFdSortIndex = 0;//定位點排序編號		
		nFdSortIndexBefore = 0;//定位點排序編號		
	}
} TFdSort, *PFdSort;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdSortWnd dialog
//-------------------------------------------------------------------------------------//
class CFdSortWnd : public CBaseDialog
{
// Construction
public:
	CFdSortWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFdSortWnd)
	enum { IDD = IDD_FD_SORT_WND };
	CStatic	m_ImageWnd;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFdSortWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr, FD_SORT_SCOPE_MODE Mode);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	double                     m_ZoomScale;	
	TPOINT2D                   m_ViewOffset;
	RECT                       m_ImageWndRect;
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;
	//---------------------------------------------------------------------------------//		
	bool                       m_Modified;
	bool                       m_ShowSelectLine;
	CAOIProject               *m_ProjectPtr;
	int                        m_FdSortIndex;	
	FD_SORT_PICK_MODE          m_FdSortPickMode;
	FD_SORT_SCOPE_MODE         m_FdSortScopeMode;
	std::vector<TFdSort>       m_FdSortList;
	//---------------------------------------------------------------------------------//
	POINT                      m_LastPos;	
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;	
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	//---------------------------------------------------------------------------------//
	unsigned int               m_ImageIndex;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;
	IMAGE_PTR                  m_ImagePtr;
	size_t                     m_ImageSize;
	TPOINT2D                   m_ImageRes;
	TREGION4D                  m_ImageStageRgn;
	BITMAPINFO                *m_ImageInfoPtr;
	//---------------------------------------------------------------------------------//		
	void                       SetModified(bool val);//m_Modified
	bool                       GetModified() const;
	//---------------------------------------------------------------------------------//
	void                       SetShowSelectLine(bool val);
	bool                       GetShowSelectLine() const;
	//---------------------------------------------------------------------------------//	
	CAOIProject               *GetActiveProjectPtr();
	//---------------------------------------------------------------------------------//	
	FD_SORT_PICK_MODE          GetFdSortPickMode() const;
	void                       SetFdSortPickMode(FD_SORT_PICK_MODE Mode);	
	//---------------------------------------------------------------------------------//	
	FD_SORT_SCOPE_MODE         GetFdSortScopeMode() const;
	void                       SetFdSortScopeMode(FD_SORT_SCOPE_MODE Mode);	
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);	
	//---------------------------------------------------------------------------------//
	bool                       BuildFdSortList();	
	size_t                     GetFdSortCount();
	TFdSort*                   GetFdSortPtr(size_t idx, bool bCheck);	
	bool                       ResetFdSortList(bool bResetAll);
	bool                       ResetFdSortListSelected();
	bool                       FillInFdSortList();//填入尚未設定的單板排序 
	bool                       RestoreFdSortList();	
	bool                       IncludeVisibledFdSortList();
	bool                       IncludeSelectedFdSortList();
	//---------------------------------------------------------------------------------//
	void                       UpdateFdSortCount(int val);
	bool                       ExecAutoFdSortFn(FD_SORT_SEARCH_MODE Mode, FD_SORT_SEARCH_MODE SubMode);
	TFdSort*                   SearchNextFdSortPtr(TFdSort *FdSortPtr, FD_SORT_SEARCH_MODE Mode);
	TFdSort*                   SearchLimitFdSortPtr(TFdSort *FdSortPtr, FD_SORT_SEARCH_MODE Mode);
	TFdSort*                   SearchFirstFdSortPtr(FD_SORT_SEARCH_MODE Mode, FD_SORT_SEARCH_MODE SubMode);
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();	
	void                       CreateBKImage();
	void                       DrawSelectRect(HDC hDC);	
	void                       DrawCameraRgn(HDC hDC);	
	void                       DrawProjectFd(HDC hDC);	
	void                       DrawProjectPanel(HDC hDC);	
	void                       DrawFdSortList(HDC hDC);	
	void                       DrawProjectComponent(HDC hDC);
	//---------------------------------------------------------------------------------//
	bool                       ClearImageBuffer();
	bool                       CreateImageBuffer();
	bool                       SwitchShowImage();
	bool                       BuildShowImage(size_t index);
	//---------------------------------------------------------------------------------//	
	bool                       ChangeFdSortRadio(UINT ActID);	
	bool                       ExecUpdateFdSort();//更新定位點排序	
	bool                       ExecPickFdSort(POINT Pt);
	bool                       ExecRectFdSort(RECT Rect);
	size_t                     CheckPickFdSort(POINT Pt);
	bool                       SelectFdSort(size_t index);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFdSortWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFdSortResetBtn();
	afx_msg void OnFdSortSetBtn();
	afx_msg void OnFdSortStopBtn();
	afx_msg void OnFdSortRestoreRdo();
	afx_msg void OnFdSortRunBtn();
	afx_msg void OnFdSortLRTBRdo();
	afx_msg void OnFdSortRLTBRdo();
	afx_msg void OnFdSortLRBTRdo();
	afx_msg void OnFdSortRLBTRdo();
	afx_msg void OnFdSortTBLRRdo();
	afx_msg void OnFdSortBTLRRdo();
	afx_msg void OnFdSortTBRLRdo();
	afx_msg void OnFdSortBTRLRdo();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDSORTWND_H__37FAB498_EE57_4C01_9B74_DCB4A7EDEAB3__INCLUDED_)
