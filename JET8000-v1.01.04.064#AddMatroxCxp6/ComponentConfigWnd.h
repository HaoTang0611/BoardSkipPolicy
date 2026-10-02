#if !defined(AFX_COMPONENTCONFIGWND_H__4D493E5B_6A8E_4C86_84CB_50EB4C5D6E19__INCLUDED_)
#define AFX_COMPONENTCONFIGWND_H__4D493E5B_6A8E_4C86_84CB_50EB4C5D6E19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComponentConfigWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#define CThisListCtrl_06     CListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
enum COMPONENT_CONFIG_MODE
{
	COMPONENT_CONFIG_NONE,
	COMPONENT_CONFIG_BOARD_ASSIGN,//單板指定
	COMPONENT_CONFIG_RETURN
};
//-------------------------------------------------------------------------------------//
enum COMPONENT_CONFIG_IMAGE_MODE
{
	COMPONENT_CONFIG_IMAGE_NONE,
	COMPONENT_CONFIG_IMAGE_MAP,//底圖
	COMPONENT_CONFIG_IMAGE_TMP,//底圖
	COMPONENT_CONFIG_IMAGE_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagComponentConfig
{
	CAOIPanel     *PanelPtr;
	CAOIBoard     *BoardPtr;
	CAOIComponent *ComponentPtr;
	unsigned int   uPanelIndex;
	unsigned int   uBoardIndex;	
	unsigned int   uComponentIndex;	
	bool           bVisibled;//顯示
	bool           bSelected;//選取
	bool           bDelected;//刪除
	bool           bIncluded;//納入計算
	bool           bCalculated;//已經計算
	unsigned int   uBoardIndexNew;	
	TRECT4D        rcComponentMapRect;	
	TREGION4D      rgnComponentCadRgn;
	CString        strComponentName;

	tagComponentConfig()
	{
		PanelPtr = NULL;
		BoardPtr = NULL;
		ComponentPtr = NULL;
		uPanelIndex = -1;
		uBoardIndex = -1;
		uComponentIndex = -1;
		bVisibled = true;
		bSelected = false;
		bDelected = false;
		bIncluded = false;
		bCalculated = false;
		uBoardIndexNew = -1;		
		rcComponentMapRect = TRECT4D();		
		rgnComponentCadRgn = TREGION4D();
		strComponentName = _T("");
	}
} TComponentConfig, *PComponentConfig;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CComponentConfigWnd dialog
//-------------------------------------------------------------------------------------//
class CComponentConfigWnd : public CBaseDialog
{
// Construction
public:
	CComponentConfigWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComponentConfigWnd)
	enum { IDD = IDD_COMPONENT_CONFIG_WND };
	CStatic	m_ImageWnd;	
	CThisListCtrl_06 m_PanelListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponentConfigWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	bool                       GetModified() const;
	void                       SetProjectPtr(CAOIProject *Ptr);
	void                       SetLockPanelListWnd(bool bLock);
	void                       SetComponentConfigMode(COMPONENT_CONFIG_MODE Mode);
	void                       SetComponentConfigImageMode(COMPONENT_CONFIG_IMAGE_MODE Mode);
	//---------------------------------------------------------------------------------//
	void                       CloneBoardRectList(std::vector<TBoardRect> &List);
	void                       CloneComponentConfigList(std::vector<TComponentConfig> &List);
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
	bool                       m_LockPanelListWnd;
	CAOIProject               *m_ProjectPtr;	
	COMPONENT_CONFIG_MODE      m_ComponentConfigMode;
	COMPONENT_CONFIG_IMAGE_MODE m_ComponentConfigImageMode;
	std::vector<TBoardRect>    m_BoardRectList;
	std::vector<TComponentConfig> m_ComponentConfigList;
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
	bool                       GetLockPanelListWnd() const;
	COMPONENT_CONFIG_MODE      GetComponentConfigMode() const;
	COMPONENT_CONFIG_IMAGE_MODE GetComponentConfigImageMode() const;
	//---------------------------------------------------------------------------------//	
	void                       SetModified(bool val);//m_Modified	
	//---------------------------------------------------------------------------------//
	void                       SetShowSelectLine(bool val);
	bool                       GetShowSelectLine() const;
	//---------------------------------------------------------------------------------//
	CAOIPanel                 *GetActivePanelPtr();
	CAOIProject               *GetActiveProjectPtr();	
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);	
	//---------------------------------------------------------------------------------//
	bool                       BuildBoardRectList();	
	size_t                     GetBoardRectCount();
	TBoardRect*                GetBoardRectPtr(size_t idx, bool bCheck);	
	bool                       UpdateBoardRectList(CAOIPanel *PanelPtr);
	bool                       RemoveVisibleBoardRect();
	bool                       RemoveNoComponentBoardRect();//移除單板區域
	//---------------------------------------------------------------------------------//
	bool                       BuildComponentConfigList();	
	size_t                     GetComponentConfigCount();
	TComponentConfig*          GetComponentConfigPtr(size_t idx, bool bCheck);	
	bool                       ResetComponentConfigListSelected();
	bool                       IncludeVisibledComponentConfigList();
	bool                       IncludeSelectedComponentConfigList();
	bool                       ResortComponentConfigListBoardIndex();
	bool                       UpdateComponentConfigList(CAOIPanel *PanelPtr);
	bool                       ResetComponentConfigListBoardIndex(bool bSelected);		
	//---------------------------------------------------------------------------------//
	bool                       BuildPanelListWndHeader();
	bool                       BuildPanelListWnd();
	bool                       ExecChangePanelListItem(int nItem);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();	
	void                       CreateBKImage();
	void                       DrawSelectRect(HDC hDC);	
	void                       DrawBoardRectList(HDC hDC);	
	void                       DrawComponentConfigList(HDC hDC);	
	//---------------------------------------------------------------------------------//
	bool                       ClearImageBuffer();
	bool                       CreateImageBuffer();
	bool                       CreateImageBuffer_Map();
	bool                       CreateImageBuffer_Tmp();
	bool                       SwitchShowImage();
	bool                       BuildShowImage(size_t index);
	bool                       BuildShowImage_Map(size_t index);
	bool                       BuildShowImage_Tmp(size_t index);
	//---------------------------------------------------------------------------------//	
	bool                       ExecUpdateComponentBoard(CString &ErrorString);
	bool                       ExecSetComponentSelectedBoard();//設定選取的到零件單板
	bool                       ExecApplyToOtherComponentBoard();//套用至其他零件的單板
	bool                       ExecRectComponentConfig(RECT Rect);
	//---------------------------------------------------------------------------------//
	bool                       CheckComponentConfigFinish_BoardAssign(CString &ErrorString);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComponentConfigWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetBoardBtn();
	afx_msg void OnApplyBoardBtn();
	afx_msg void OnClearBoardBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPONENTCONFIGWND_H__4D493E5B_6A8E_4C86_84CB_50EB4C5D6E19__INCLUDED_)
