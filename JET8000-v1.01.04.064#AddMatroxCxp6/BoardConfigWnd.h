#if !defined(AFX_BOARDCONFIGWND_H__9E06039C_81E5_4A26_BAFC_87B39798A04B__INCLUDED_)
#define AFX_BOARDCONFIGWND_H__9E06039C_81E5_4A26_BAFC_87B39798A04B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoardConfigWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_05     CListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
enum BOARD_CONFIG_MODE
{
	BOARD_CONFIG_NONE,
	BOARD_CONFIG_ORDER,//單板排序
	BOARD_CONFIG_RETURN
};
//-------------------------------------------------------------------------------------//
enum BOARD_CONFIG_PICK_MODE
{
	BOARD_CONFIG_PICK_NONE, 
	BOARD_CONFIG_PICK_ORDER,//新增單板排序
	BOARD_CONFIG_PICK_RETURN
};
//-------------------------------------------------------------------------------------//
enum BOARD_ORDER_SEARCH_MODE//單板尋找排序模式
{
	BOARD_ORDER_SEARCH_NONE,
	BOARD_ORDER_SEARCH_LEFT,   //最左邊
	BOARD_ORDER_SEARCH_TOP,    //最上面
	BOARD_ORDER_SEARCH_RIGHT,  //最左邊
	BOARD_ORDER_SEARCH_BOTTOM, //最下面
	BOARD_ORDER_SEARCH_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagBoardConfig
{
	CAOIPanel     *PanelPtr;
	CAOIBoard     *BoardPtr;
	unsigned int   uPanelIndex;
	unsigned int   uBoardIndex;	
	bool           bCounted;//數過
	bool           bVisibled;//顯示
	bool           bSelected;//選取
	bool           bDelected;//刪除
	bool           bIncluded;//納入計算	
	TRECT4D        rcBoardMapRect;	
	int            nBoardOrderIndex;//單板排序編號
	int            nBoardOrientationMode;//單板極性
	int            nBarcodeID;
	int            nBarcodeCodeID;
	int            nBarcodeAutoExtendID_A;//條碼自動展開代碼-A組
	int            nBarcodeAutoExtendID_B;//條碼自動展開代碼-B組	
	int            nBoardPosIndex_X;//單板位置編號-X
	int            nBoardPosIndex_Y;//單板位置編號-Y
	int            nBoardOrderIndex_Backup;//單板排序編號-備份

	tagBoardConfig()
	{
		PanelPtr = NULL;
		BoardPtr = NULL;
		uPanelIndex = -1;
		uBoardIndex = -1;
		bCounted = false;
		bVisibled = true;
		bSelected = false;
		bDelected = false;
		bIncluded = false;
		rcBoardMapRect = TRECT4D();	
		nBoardOrderIndex = -1;//單板排序編號
		nBoardOrientationMode;//單板極性
		nBarcodeID = 0;
		nBarcodeCodeID = 0;
		nBarcodeAutoExtendID_A = 0;//條碼自動展開代碼-A組
		nBarcodeAutoExtendID_B = 0;//條碼自動展開代碼-B組
		nBoardPosIndex_X = -1;
		nBoardPosIndex_Y = -1;
		nBoardOrderIndex_Backup = -1;
	}
} TBoardConfig, *PBoardConfig;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBoardConfigWnd dialog

class CBoardConfigWnd : public CBaseDialog
{
// Construction
public:
	CBoardConfigWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBoardConfigWnd)
	enum { IDD = IDD_BOARD_CONFIG_WND };
	CStatic	m_ImageWnd;	
	CThisListCtrl_05 m_PanelListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoardConfigWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	void                       SetProjectPtr(CAOIProject *Ptr);
	void                       SetBoardConfigMode(BOARD_CONFIG_MODE Mode);
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
	bool                       m_MatrixMode;//每行列數相等
	bool                       m_ShowSelectLine;
	CAOIProject               *m_ProjectPtr;
	int                        m_BoardOrderIndex;
	BOARD_CONFIG_MODE          m_BoardConfigMode;
	BOARD_CONFIG_PICK_MODE     m_BoardConfigPickMode;
	std::vector<TBoardConfig>  m_BoardConfigList;	
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
	CAOIPanel                 *GetActivePanelPtr();
	CAOIProject               *GetActiveProjectPtr();	
	BOARD_CONFIG_MODE          GetBoardConfigMode() const;
	//---------------------------------------------------------------------------------//
	BOARD_CONFIG_PICK_MODE     GetBoardConfigPickMode() const;
	void                       SetBoardConfigPickMode(BOARD_CONFIG_PICK_MODE Mode);	
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);	
	//---------------------------------------------------------------------------------//
	bool                       GetSPath() const;//是否使用S路徑
	bool                       GetUseMatrixMode() const;//使用陣列模式(行列等數)
	//---------------------------------------------------------------------------------//
	bool                       BuildBoardConfigList();	
	bool                       BuildBoardConfigListPosIndex();
	size_t                     GetBoardConfigCount();
	TBoardConfig*              GetBoardConfigPtr(size_t idx, bool bCheck);
	bool                       UpdateBoardConfigList(CAOIPanel *PanelPtr);			
	bool                       ResetBoardConfigList_Order(bool bResetAll);
	bool                       ResetBoardConfigListSelected();
	bool                       FillInBoardConfigList_Order();//填入尚未設定的單板排序 
	bool                       RestoreBoardConfigList_Order();	
	bool                       IncludeVisibledBoardConfigList();
	bool                       IncludeSelectedBoardConfigList();
	//---------------------------------------------------------------------------------//
	void                       UpdateBoardOrderCount(int val);
	TRECT4D                    GetBoardFilterRect(TBoardConfig *RefPtr);
	bool                       CalcBoardPitch(TBoardConfig *RefPtr, double &PitchX, double &PitchY);
	BOARD_ORDER_SEARCH_MODE    GetOppositeDirectionMode(BOARD_ORDER_SEARCH_MODE Mode) const;//取得反方向
	bool                       ExecAutoBoardOrderFn(BOARD_ORDER_SEARCH_MODE Mode, BOARD_ORDER_SEARCH_MODE SubMode);
	TBoardConfig*              SearchNextBoardConfigPtr(TBoardConfig *BoardConfigPtr, BOARD_ORDER_SEARCH_MODE Mode);
	TBoardConfig*              SearchLimitBoardConfigPtr(TBoardConfig *BoardConfigPtr, BOARD_ORDER_SEARCH_MODE Mode);
	TBoardConfig*              SearchFirstBoardConfigPtr(BOARD_ORDER_SEARCH_MODE Mode, BOARD_ORDER_SEARCH_MODE SubMode);
	//---------------------------------------------------------------------------------//
	bool                       BuildPanelListWndHeader();
	bool                       BuildPanelListWnd();
	bool                       ExecChangePanelListItem(int nItem);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();	
	void                       CreateBKImage();
	void                       DrawSelectRect(HDC hDC);	
	void                       DrawProjectFd(HDC hDC);	
	void                       DrawProjectBoard(HDC hDC);
	void                       DrawProjectBarcode(HDC hDC);
	void                       DrawBoardConfigList(HDC hDC);	
	void                       DrawProjectComponent(HDC hDC);
	//---------------------------------------------------------------------------------//
	bool                       ClearImageBuffer();
	bool                       CreateImageBuffer();
	bool                       SwitchShowImage();
	bool                       BuildShowImage(size_t index);
	//---------------------------------------------------------------------------------//	
	bool                       ChangeBoardOrderRadio(UINT ActID);	
	bool                       ExecUpdateBoardOrder();//更新單板排序
	bool                       ExecUpdateBoardOrder_All();//更新單板排序
	bool                       ExecUpdateBoardOrder_Panel();//更新單板排序
	bool                       ExecPickBoardConfig(POINT Pt);
	bool                       ExecRectBoardConfig(RECT Rect);
	size_t                     CheckPickBoardConfig(POINT Pt, BOARD_CONFIG_PICK_MODE PickMode);
	bool                       SelectBoardConfig(size_t index);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBoardConfigWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBoardSortSetBtn();
	afx_msg void OnBoardSortLRTBRdo();
	afx_msg void OnBoardSortRLTBRdo();
	afx_msg void OnBoardSortLRBTRdo();
	afx_msg void OnBoardSortRLBTRdo();
	afx_msg void OnBoardSortTBLRRdo();
	afx_msg void OnBoardSortBTLRRdo();
	afx_msg void OnBoardSortTBRLRdo();
	afx_msg void OnBoardSortBTRLRdo();
	afx_msg void OnDblclkPanelListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnBoardSortRestoreRdo();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBoardSortStopBtn();
	afx_msg void OnAllPanelModeChk();
	afx_msg void OnUserSelectModeChk();
	afx_msg void OnBoardSortResetBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOARDCONFIGWND_H__9E06039C_81E5_4A26_BAFC_87B39798A04B__INCLUDED_)
