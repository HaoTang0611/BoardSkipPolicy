#if !defined(AFX_ARRAYPASTEWND_H__041EACCF_71A3_4ED3_B98B_C1D2A5326876__INCLUDED_)
#define AFX_ARRAYPASTEWND_H__041EACCF_71A3_4ED3_B98B_C1D2A5326876__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArrayPasteWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "ImageWnd.h"
#include "ProjectMapWnd.h"
//-------------------------------------------------------------------------------------//
#define ARRAY_PASTE_PANEL              1
#define ARRAY_PASTE_BOARD              2
#define ARRAY_PASTE_COMPONENT          3
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CArrayPasteWnd dialog
//-------------------------------------------------------------------------------------//
class CArrayPasteWnd : public CBaseDialog
{
// Construction
public:
	CArrayPasteWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CArrayPasteWnd)
	enum { IDD = IDD_ARRAY_PASTE_WND };
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CArrayPasteWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	double                    GetColPitch() const;
	double                    GetRowPitch() const;
	int                       GetNameMode() const;
	void                      SetNameMode(int Mode);
	bool                      GetChangeSelName() const;
	void                      SetArrayPasteMode(int Mode);	
	void                      GetIdxList(std::vector<POINT> &IdxList);
	void                      GetPosList(std::vector<TPOINT2D> &PosList);
	void                      SetProjectPtr(CAOIProject *Ptr);	
	void                      SetSelectedRgn(const TREGION4D &Rgn);	
	void                      SetMapCoordinate(CMapCoordinate *MapPtrCTS, CMapCoordinate *MapPtrSTC);	
	//---------------------------------------------------------------------------------//				
protected:
	//---------------------------------------------------------------------------------//	
	double                     m_ColPitch;
	double                     m_RowPitch;
	int                        m_NameMode;
	bool                       m_ChangeSelName;
	int                        m_ArrayPasteMode;
	std::vector<POINT>         m_IdxList;
	std::vector<TPOINT2D>      m_PosList;
	bool                       m_SetRgn;	
	TREGION4D                  m_SelectedRgn;	
	//---------------------------------------------------------------------------------//		
	CAOIProject               *m_ProjectPtr;
	CMapCoordinate             m_CadToStageMap;	
	CMapCoordinate             m_StageToCadMap;	
	CProjectMapWnd             m_ProjectMapWnd;
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ImageIndex;
	IMAGE_SIZE                 m_ShowImageW;
	IMAGE_SIZE                 m_ShowImageH;
	IMAGE_SIZE                 m_ShowImageStep;
	IMAGE_SIZE                 m_ShowBitCount;	
	IMAGE_PTR                  m_ShowImagePtr;
	TPOINT2D                   m_FrameResolution;//影像解析度		
	TREGION4D                  m_FrameStageRgn;
	TUNI_FRAME                 m_UniFrameList[FRAME_MAX_COUNT];	
	//-------------------------------------------------------------------------------------//
	CAOIProject*               GetActiveProject();
	//-------------------------------------------------------------------------------------//
	int                        GetMaxFrameCount() const;
	void                       PreInitUniFrameBuffer();//預先影像記憶體
	void                       ReleaseUniFrameBuffer();//釋放影像記憶體	
	void                       BuildShowImageBuffer(bool ResetView);//建立顯示的影像記憶體
	void                       ReleaseShowImageBuffer();//釋放顯示影像記憶體
	void                       SwitchFrameImage();
	bool                       FillCurrentFrames(double Ratio);		
	//-------------------------------------------------------------------------------------//
	void                       AdjustCtrlWndPosition();
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();
	void                       CreateBKImage();	
	//---------------------------------------------------------------------------------//
	bool                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//		
	bool                       ExecMoveToStage();
	bool                       ExecShowWndPosition();
	bool                       ExecGrabFov(double PosX, double PosY, double PosZ);
	bool                       ExecUpdateFov(double PosX, double PosY, double PosZ);
	//---------------------------------------------------------------------------------//		
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像
	//---------------------------------------------------------------------------------//
	bool                       OnImageWndNotify(WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//		
	bool                       BuildPosList(bool bAlignMapPos);//建立位置列表
	bool                       PitchPosList();//等間距位置列表
	bool                       AlignPosList();//對齊位置列表
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CArrayPasteWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnShowMapBtn();
	afx_msg void OnPaint();
	afx_msg void OnRegionPosToBtn1();
	afx_msg void OnRegionPosToBtn2();
	afx_msg void OnRegionPosGetBtn1();
	afx_msg void OnRegionPosGetBtn2();
	afx_msg void OnRegionCalcPitch();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMotionJogChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARRAYPASTEWND_H__041EACCF_71A3_4ED3_B98B_C1D2A5326876__INCLUDED_)
