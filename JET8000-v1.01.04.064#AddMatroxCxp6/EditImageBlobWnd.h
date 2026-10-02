#if !defined(AFX_EDITIMAGEBLOBWND_H__025E8738_C62A_4403_8DAC_D04208FFA57F__INCLUDED_)
#define AFX_EDITIMAGEBLOBWND_H__025E8738_C62A_4403_8DAC_D04208FFA57F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditImageBlobWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "JetBlob.h"
#include "JetMemDC.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
typedef struct tagBlobObj
{
	int     BlobID;
	int     BlobPixels;
	RECT    BlobRect;
	double  BlobGCPosX;
	double  BlobGCPosY;
	double  BlobSizeW;
	double  BlobSizeH;
	double  BlobSizeD;
	double  BlobSizeA;
	double  BlobAspectRatio;	
	double  BlobFillRatio;
	double  BlobLongShortRatio;
	bool    BlobVisibled;
	bool    BlobSelected;
	
	bool    BlobMatchSizeW;
	bool    BlobMatchSizeH;
	bool    BlobMatchSizeD;
	bool    BlobMatchSizeA;
	bool    BlobMatchAspectRatio;	
	bool    BlobMatchFillRatio;	
	bool    BlobMatchLongShortRatio;
	bool    BlobMatchResult;

	tagBlobObj()
	{
		BlobID = -1;
		BlobPixels = 0;
		BlobRect.left = BlobRect.top = BlobRect.bottom = BlobRect.right = -1;
		BlobGCPosX = 0;
		BlobGCPosY = 0;
		BlobSizeW = 0.0;
		BlobSizeH = 0.0;
		BlobSizeD = 0.0;
		BlobSizeA = 0.0;
		BlobAspectRatio = 1.0;
		BlobFillRatio = 1.0;
		BlobLongShortRatio = 100.0;

		BlobVisibled = true;
		BlobSelected = false;
		
		BlobMatchSizeW = true;
		BlobMatchSizeH = true;
		BlobMatchSizeD = true;
		BlobMatchSizeA = true;
		BlobMatchAspectRatio = true;
		BlobMatchFillRatio = true;
		BlobMatchLongShortRatio = true;
		BlobMatchResult = true;
		BlobMatchFillRatio = true;
	}	
} TBlobObj, *PBlobObj;
//-------------------------------------------------------------------------------------//
//#define CThisListCtrl_08     CListCtrl//目前使用的列表控制類別	
#define CThisListCtrl_08     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageBlobWnd dialog
//-------------------------------------------------------------------------------------//
class CEditImageBlobWnd : public CDialog
{
// Construction
public:
	CEditImageBlobWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditImageBlobWnd)
	enum { IDD = IDD_EDIT_IMAGE_BLOB_WND };
	CStatic	m_ImageWnd;
	CThisListCtrl_08 m_ListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditImageBlobWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//
	int                        CompareBlobListWnd(size_t index1, size_t index2);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CAOIWnd                   *m_WndPtr;	
	CAOIModel                 *m_ModelPtr;
	CAOIProject               *m_ProjectPtr;
	BOX_TOWARD                 m_BlobToward;
	TALG_PARAM_BLOB_COUNT      m_BlobParam;
	//---------------------------------------------------------------------------------//	
	CJetMemDC                  m_MemDC1;
	CJetMemDC                  m_MemDC2;
	RECT                       m_WndRect;
	COLORREF                   m_BKColor;
	double                     m_ImageZoom;		
	TPOINT2D                   m_ImageOffset;
	double                     m_ImageResX;
	double                     m_ImageResY;
	POINT                      m_LastCursorPos;
	POINT                      m_CurrentCursorPos;
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_ImagePtr;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_ImageBitCount;	
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_MaskPtr;
	IMAGE_SIZE                 m_MaskW;
	IMAGE_SIZE                 m_MaskH;
	IMAGE_SIZE                 m_MaskStep;
	IMAGE_SIZE                 m_MaskBitCount;	
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_ShowPtr;
	IMAGE_SIZE                 m_ShowW;
	IMAGE_SIZE                 m_ShowH;
	IMAGE_SIZE                 m_ShowStep;	
	IMAGE_SIZE                 m_ShowBitCount;	
	//---------------------------------------------------------------------------------//
	std::vector<TBlobObj>      m_BlobList;	
	bool                       ClearBlobList();
	bool                       BuildBlobList();	
	bool                       SelectBlobObj(int Index);
	bool                       PickBlobObj(const POINT &pt);
	bool                       SetActBlobInfo(const TBlobObj *BlobPtr);
	//---------------------------------------------------------------------------------//
	int                        m_BlobListSortMode;
	int                        m_BlobListWndColID;
	bool                       m_StopBlobListBeSelected;
	bool                       BuildListWnd();	
	bool                       ClearListWnd();
	bool                       BuildListWndHeader();
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();	
	void                       CreateBKImage();
	void                       DrawImage(HDC hDC, const RECT &Rect);
	void                       DrawBlobList(HDC hDC);
	//---------------------------------------------------------------------------------//
	void                       CloseProject();	
	void                       CloseProjectKernel();	
	CAOIProject*               GetActiveProject();	
	void                       UpdateWndSelected();
	void                       UpdateWndSelectedKernel();
	//---------------------------------------------------------------------------------//		
	bool                       DestroyShowBuffer();
	bool                       DestroyMaskBuffer();
	bool                       DestroyImageBuffer();	
	bool                       BuildShowBuffer();
	bool                       BuildImageBuffer(CAOIModel *ModelPtr, CAOIWnd *WndPtr);		
	//---------------------------------------------------------------------------------//
	bool                       ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam);
	bool                       ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditImageBlobWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnItemchangedListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowRawImageChk();
	afx_msg void OnUpdateWndSelectedBtn();
	afx_msg void OnColumnclickListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);	
	afx_msg void OnShowAllChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITIMAGEBLOBWND_H__025E8738_C62A_4403_8DAC_D04208FFA57F__INCLUDED_)
