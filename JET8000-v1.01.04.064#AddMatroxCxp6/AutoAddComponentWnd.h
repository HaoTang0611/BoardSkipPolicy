#pragma once


// AutoAddComponentWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "JETListCtrl.h"
#include "ImageWnd.h"
#include "EditLibraryWnd.h"
#include "EditImageProcessPage.h"
#include "EditImageBlobWnd.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_72     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAutoAddComponentWnd dialog
//-------------------------------------------------------------------------------------//
class CAutoAddComponentWnd : public CBaseDialog
{
	DECLARE_DYNAMIC(CAutoAddComponentWnd)
	typedef enum {
		AAC_WND_IMAGE_MAP,
		AAC_WND_IMAGE_FOV
	}AAC_WND_IMAGE;

public:
	CAutoAddComponentWnd(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CAutoAddComponentWnd();

	enum { IDD = IDD_AUTO_ADD_COMPONENT_WND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()


public:
	//---------------------------------------------------------------------------------//	
	void                       SetProjectPtr(CAOIProject *Ptr);
protected:
	CEditImageColorFilterWnd  *m_wndColorFilter;
	//CTabCtrl                   m_BlobTab;
	CThisListCtrl_72	       m_BlobListCtrl;
	//---------------------------------------------------------------------------------//
	HWND                       m_ParentWnd;
	//---------------------------------------------------------------------------------//
	CImageWnd                  m_ImageWnd;
	CStatic	                   m_ComponentImageWnd;
	CDib                       m_Dib;
	CSpinButtonCtrl	           m_IndexSpin;
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_ProjectPtr;
	//---------------------------------------------------------------------------------//	
	RECT                       m_EditRect;
	TREGION4D                  m_StageRegion;
	//---------------------------------------------------------------------------------//	
	TPOINT2D                   m_PointAPos;
	TPOINT2D                   m_PointBPos;
	//---------------------------------------------------------------------------------//	
	CAOIWnd                   *m_WndPtr;
	CAOIModel                 *m_ModelPtr;
	CAlgParam                  m_AlgParam;
	CAlgBinaryParam            m_BinaryParam;
	//---------------------------------------------------------------------------------//	
	int                        m_PatternIndex;
	int                        m_PatternCount;
	CPatternParam              m_PatternParam;
	std::vector<TPATTERN_ROI>  m_PatternRoiList;
	//---------------------------------------------------------------------------------//	
	float                      m_PatternSimilarity;
	float                      m_PatternScaleUSL;
	float                      m_PatternScaleLSL;
	//---------------------------------------------------------------------------------//	
	bool                       m_bPatternFinding;
	//---------------------------------------------------------------------------------//	
	AAC_WND_IMAGE              m_AAC_WND_IMAGE;
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ProjectMapW;
	IMAGE_SIZE                 m_ProjectMapH;
	//---------------------------------------------------------------------------------//	
	COLORREF                   m_BkColor;
	RECT                       m_ImageWndRect;
	double                     m_ImageZoom;
	TPOINT2D                   m_ImageOffset;
	CJetMemDC                  m_ImageWndMemDC;
	CJetMemDC                  m_ImageWndMemDC2;
	CJetMemDC                  m_ConponentImageWndMemDC;
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ShowBitCount;
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE                 m_ShowImageStep;
	size_t                     m_ShowImageSize;
	IMAGE_PTR                  m_ShowImagePtr;
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ShowBufferW;
	IMAGE_SIZE                 m_ShowBufferH;
	IMAGE_SIZE                 m_ShowBufferStep;
	size_t                     m_ShowBufferSize;
	IMAGE_PTR                  m_ShowBufferPtr;
	//---------------------------------------------------------------------------------//	
	TPOINT2D                   m_FrameResolution;//影像解析度		
	TREGION4D                  m_FrameStageRgn;
	FRAME_TYPE                 m_FrameType;
	unsigned int               m_FrameIndex;
	unsigned int               m_FrameUniqueID;//取像畫面的唯一碼
	//---------------------------------------------------------------------------------//	
	POINT                      m_CurrentPt;
	//---------------------------------------------------------------------------------//	
	bool                       m_ResetView;
	double                     m_FovStageX;
	double                     m_FovStageY;
	//---------------------------------------------------------------------------------//
	//std::vector<RECT>          m_FoundRectList;
	//std::vector<TComponentRect> m_FoundRectList;
	std::vector<TBOX_DRAW_PARAM> m_FoundRectList;
	//---------------------------------------------------------------------------------//
	CComboBox                  m_PanelIndexComobx;
	CComboBox                  m_BoardIndexComobx;
	CComboBox                  m_BlobConnectivityComobx;
	//---------------------------------------------------------------------------------//
	CEditLibraryWnd           *m_EditLibraryWnd;
	//---------------------------------------------------------------------------------//
	std::vector<UINT>          m_ImageMatchUI_IDList;
	std::vector<UINT>          m_ImageBlobUI_IDList;
	std::vector<UINT>          m_ImageFoundUI_IDList;
	//---------------------------------------------------------------------------------//
	MANIPULATE_MODEL_MODE      m_ManiMode;
	//---------------------------------------------------------------------------------//
	std::vector<TPOINT2D>      m_ImageSearchPoint;
	int                        m_ImageSearchIndex;
	//---------------------------------------------------------------------------------//
	bool                       m_bLocked;
	//---------------------------------------------------------------------------------//
	HPEN                       m_hPen;
	HPEN                       m_hPenSel;
	//---------------------------------------------------------------------------------//


	//---------------------------------------------------------------------------------//	
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//	
	bool                       InitImageWnd();
	bool                       InitWndElementDefault();
	bool                       SetMethodUIGroupUIDList();
	//---------------------------------------------------------------------------------//	
	void                       ClearShowImageBuffer();
	bool                       CreateShowImageBuffer(IMAGE_SIZE ImageW,IMAGE_SIZE ImageH);
	bool                       ModifiedShowImageSize();
	//---------------------------------------------------------------------------------//	
	void                       RedrewWnd();
	//---------------------------------------------------------------------------------//	
	bool                       SwitchImageToCamara();
	bool                       SwitchImageToMap();
	//---------------------------------------------------------------------------------//	
	bool                       ExecGrabImage(bool bUseMessage = true);
	bool                       UpdateFovImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);
	bool                       UpdateFovImageToFind(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage);
	bool                       RetrieveCameraUniFrame(WPARAM wParam, LPARAM lParam, bool bCameraCallBack, bool &bGetImage);
	bool                       LoadProgramOfflineImage(WPARAM wParam, LPARAM lParam, bool &bGetImage);
	//---------------------------------------------------------------------------------//	
	BOOL                       CreateBKDC(bool bResetView);//建立背景DC	
	bool                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd();
	//---------------------------------------------------------------------------------//	
	bool                       ExecRegionSetting();
	bool                       ExecRegionSettingAPoint();
	bool                       ExecRegionSettingBPoint();
	bool                       ExecRegionMoveToPoint(TPOINT2D point, bool bUseMessage = true);
	bool                       ExecRegionMoveToPoint();
	//---------------------------------------------------------------------------------//	
	bool                       ExecAddPattern();
	bool                       ExecEditPattern();
	bool                       ExecDelPattern();
	bool                       ExecClearPattern();
	//---------------------------------------------------------------------------------//
	bool                       ExecFind();
	bool                       ExecFind_kn(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);
	bool                       ExecFindPattern(TREGION4D SearchRegion);
	bool                       ExecFindBlob(TREGION4D SearchRegion);
	bool                       ExecFindBlob_CurrentFov();
	bool                       ExecExtractImage(TPOINT2D SearchStartPt,TREGION4D &ExtractRegion);
	bool                       CalcExtractRegion(TPOINT2D SearchStartPt, TREGION4D &ExtractRegion);
	//---------------------------------------------------------------------------------//
	void                       DrawImage(HDC hDC, const RECT &Rect);
	void                       CreateBKImage();
	void                       UpdateParamToUI(UINT FromCtrlID);
	//---------------------------------------------------------------------------------//	
	bool                       SwitchPatternImage(int index, bool UpdateUI = true);
	bool                       ResetPatternParam();//復歸樣板參數
	//---------------------------------------------------------------------------------//	
	bool                       BuildPanelComboxSel();
	bool                       BuildBoardComboxSel();
	bool                       BuildConnectivityComboxSel();
	//---------------------------------------------------------------------------------//	
	bool                       ExecOpenModelLibrary();
	//---------------------------------------------------------------------------------//	
	bool                       ExecAddComponent();
	bool                       UpdateModelPtr();
	//---------------------------------------------------------------------------------//	
	bool                       MoveMethodGroupUI();
	bool                       MoveMethodGroupUI(UINT* IDArray, size_t Count, UINT Anchor);
	//---------------------------------------------------------------------------------//
	bool                       SetMethodGroupUIVisible();
	bool                       SetMethodGroupUIVisible(UINT* IDArray, size_t Count, bool bVisible);
	//---------------------------------------------------------------------------------//
	bool                       ExecOpenImageProcessWnd();
	bool                       InvisibleImageProcessWndElement();
	//---------------------------------------------------------------------------------//
	bool                       ExecGatherColor();
	bool                       ExecSelectTempObj();
	bool                       OnSelChangeColorWndList();
	bool                       BuildColorFilterImage(CAlgBinaryParam *BinParamPtr);
	//---------------------------------------------------------------------------------//
	bool                       MergeOverlappingRects();
	bool                       ReBuildFoundList();
	bool                       SetImageWndTempObj();
	//---------------------------------------------------------------------------------//
	bool                       BuildFoundListWndHeader();
	bool                       BuildFoundListWndHeader_Blob();
	bool                       BuildFoundListWndHeader_Pattern();
	bool                       ClearFoundListWnd();
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ExecClearTempObj();
	bool                       MarkSelectTempObj();

private:
	void                       SetPatternFinding(bool value) { m_bPatternFinding = value; };
	bool                       GetPatternFinding() { return m_bPatternFinding; }
	// Implementation
protected:
	//---------------------------------------------------------------------------------//	
	void                       OnDBlickedImageWnd();
	void                       OnLButtonUpImageWnd();
	//---------------------------------------------------------------------------------//	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRegionSettingBtn();
	afx_msg void OnRegionMoveToABtn();
	afx_msg void OnRegionMoveToBBtn();
	afx_msg void OnRegionSetupABtn();
	afx_msg void OnRegionSetupBBtn();
	afx_msg void OnPatternAddBtn();
	afx_msg void OnPatternDeleteBtn();
	afx_msg void OnPatternClearBtn();
	afx_msg void OnPatternEditBtn();
	afx_msg void OnFindBtn();
	afx_msg void OnBnClickedImageMatchRadio();
	afx_msg void OnBnClickedBlobRadio();
	afx_msg void OnDeltaposIndexSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangePanelIndexCombo();
	afx_msg void OnOpenModelLibraryBtn();
	afx_msg void OnOpenImageProcessBtn();
	afx_msg void OnNMDblclkFoundList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickFoundList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFoundListAddBtn();
	afx_msg void OnFoundListDeleteBtn();
	afx_msg void OnFoundListClearBtn();
	afx_msg void OnAddBtn();
	
};
