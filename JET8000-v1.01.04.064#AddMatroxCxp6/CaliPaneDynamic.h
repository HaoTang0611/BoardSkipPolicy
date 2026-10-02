#if !defined(AFX_CALIPANEDYNAMIC_H__2F1D66A2_8D66_4EF6_A784_8C35BB3E06CA__INCLUDED_)
#define AFX_CALIPANEDYNAMIC_H__2F1D66A2_8D66_4EF6_A784_8C35BB3E06CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaliPaneDynamic.h : header file
//
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "JETListCtrl.h"
#include "MotionCtrlWnd.h"
#include "CaliWndDynamicTune.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl4     CListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
enum CALIBRATION_DYNAMIC_MODE
{
	CALIBRATION_DYNAMIC_STOP,
	CALIBRATION_DYNAMIC_TEST,//對齊第1點
	CALIBRATION_DYNAMIC_TUNE,//調適參數

	CALIBRATION_DYNAMIC_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagDynamicNode
{	
	IMAGE_SIZE   ImageW;
	IMAGE_SIZE   ImageH;
	IMAGE_SIZE   ImageStep;
	IMAGE_SIZE   BitCount;
	IMAGE_PTR    ImagePtr;
	size_t       BufferSize;

	DWORD        Time;
	double       Score;
	double       OffsetX;
	double       OffsetY;
	TRECT4D      ResultRect;	
	tagDynamicNode()
	{
		ImageW = 0;
		ImageH = 0;
		ImageStep = 0;
		BitCount = 0;
		ImagePtr = NULL;
		BufferSize = 0;

		Time = 0;
		Score = 0;
		OffsetX = 0;
		OffsetY = 0;
		ResultRect = TRECT4D();
	}
} TDynamicNode, *PDynamicNode;
//-------------------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////////
// CCaliPaneDynamic dialog
//-------------------------------------------------------------------------------------//
class CCaliPaneDynamic : public CDialog
{
// Construction
public:
	CCaliPaneDynamic(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCaliPaneDynamic)
	enum { IDD = IDD_CALIBRATION_PANE_DYNAMIC };
	CThisListCtrl4 m_ResultListWnd;
	CComboBox	m_SliceCombox;
	CImageWnd	m_ImageWnd;
	//}}AFX_DATA

public:
	//---------------------------------------------------------------------------------//
	void                       SetSpaceBuffer(size_t Size, SPACE_PTR Buffer, SPACE_PTR Buffer1);
	void                       SetShowBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1);
	void                       SetImageBuffer(size_t Size, IMAGE_PTR Buffer, IMAGE_PTR Buffer1, IMAGE_PTR Buffer2, PHASE_PTR PhaseBuffer, PHASE_PTR PhaseBuffer1, PHASE_PTR PhaseBuffer2, MASK_PTR MaskBuffer);
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//	
	COLORREF                   m_BKColor;
	double                     m_ImageZoom;
	RECT                       m_ImageWndRect;		
	TPOINT2D                   m_ImageResolution;
	CALIBRATION_DYNAMIC_MODE   m_CalibrationMode;
	bool                       m_StopResultListBeSelected;
	long                       m_CameraImageReceieveCount;//已從相機接收多少的相機影像
	//---------------------------------------------------------------------------------//	
	int                        m_LastAxisIndex;//上一次設定的軸
	double                     m_LastAxisOffset;//上一次設定的偏移量
	CMotionCtrlWnd             m_MotionCtrlWnd;
	//---------------------------------------------------------------------------------//
	size_t                     m_ImageIndex;
	CAMERA_ID                  m_CameraID;	
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_BitCount;	
	TPOINT3D                   m_StageOrg;
	TSliceParam                m_SliceParam;	
	//---------------------------------------------------------------------------------//		
	LARGE_INTEGER              m_TestEnd;	
	LARGE_INTEGER              m_TestStart;		
	LARGE_INTEGER              m_TuneStart;	
	LARGE_INTEGER              m_TestSetup;			
	TMotionParameter           m_MotionParam;	
	unsigned int               m_DynamicTuneIndex;
	TDynamicTuneParam          m_DynamicTuneParam;	
	std::vector<TDynamicNode>  m_DynamicNodeList;
	std::vector<TDynamicTuneParam> m_DynamicTuneList;
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_ShowBuffer;
	IMAGE_PTR                  m_ShowBuffer1;	
	//---------------------------------------------------------------------------------//
	MASK_PTR                   m_MaskBuffer;
	PHASE_PTR                  m_PhaseBuffer;
	PHASE_PTR                  m_PhaseBuffer1;
	PHASE_PTR                  m_PhaseBuffer2;	
	IMAGE_PTR                  m_ImageBuffer;
	IMAGE_PTR                  m_ImageBuffer1;
	IMAGE_PTR                  m_ImageBuffer2;	
	//---------------------------------------------------------------------------------//
	SPACE_PTR                  m_SpaceBuffer;
	SPACE_PTR                  m_SpaceBuffer1;//空間記憶體指標	
	//---------------------------------------------------------------------------------//
	size_t                     m_ShowBufferSize;
	size_t                     m_ImageBufferSize;
	size_t                     m_SpaceBufferSize;
	//---------------------------------------------------------------------------------//	
	int                        m_PatternX;
	int                        m_PatternY;	
	IMAGE_SIZE                 m_PatternW;
	IMAGE_SIZE                 m_PatternH;
	IMAGE_SIZE                 m_PatternStep;
	IMAGE_SIZE                 m_PatternBitCnt;
	IMAGE_PTR                  m_PatternPtr;
	TRECT4D                    m_PatternRect4D;
	//---------------------------------------------------------------------------------//
	bool                       CreateTempFolder();//建立暫存資料夾
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	void                       CreateBKImage();	
	//---------------------------------------------------------------------------------//
	CAMERA_ID                  GetCameraID();
	UINT                       GetImageCount();
	DWORD                      GetMoveDownDelyTime();//移動完成延遲時間	
	//---------------------------------------------------------------------------------//	
	bool                       LockUIWnd(bool bLock);
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	//---------------------------------------------------------------------------------//	
	void                       SetCalibrationMode(CALIBRATION_DYNAMIC_MODE Mode);
	CALIBRATION_DYNAMIC_MODE   GetCalibrationMode() const;
	//---------------------------------------------------------------------------------//	
	void                       ClearLogListBox();//清除紀錄列表視窗
	void                       AddLogListBox(LPCTSTR str);//加入紀錄列表視窗
	//---------------------------------------------------------------------------------//
	bool                       RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack);//取得相機影像	
	//---------------------------------------------------------------------------------//		
	bool                       ConfigGrabParam(CALIBRATION_DYNAMIC_MODE Mode);		
	bool                       FocusToEditCtrl();
	//---------------------------------------------------------------------------------//
	bool                       ExecGrabFirst();//執行第一次取像
	bool                       ExecGrabNext();//執行下一次取像
	//---------------------------------------------------------------------------------//
	bool                       CreateImageBufferList(size_t BufferSize, size_t ImageCount);
	bool                       CloneImageBufferList();
	bool                       ReleaseImageBufferList();
	//---------------------------------------------------------------------------------//
	bool                       ReleasePatternImage();
	//---------------------------------------------------------------------------------//
	bool                       ClearResultListWnd();
	bool                       BuildResultListWnd();
	bool                       BuildResultListWndHeader();
	//---------------------------------------------------------------------------------//
	bool                       AnalysizeDynamicOffset();//分析動態變化	
	//---------------------------------------------------------------------------------//
	void                       SetInfoText(LPCTSTR Text);
	void                       AddInfoText(LPCTSTR Text);
	//---------------------------------------------------------------------------------//
	bool                       ExecStopGrab();
	bool                       ExecDynamicTest(double PosX1, double PosY1, double PosX2, double PosY2);	
	//---------------------------------------------------------------------------------//
	bool                       ExecDynamicTuneFirst();
	bool                       ExecDynamicTuneNext();
	bool                       AnalysizeDynamicTuneOffset();//分析動態變化
	bool                       AnalysizeDynamicTuneResult();//分析動態變化	
	bool                       SaveDynamicTuneParamList(LPCTSTR filename, const std::vector<TDynamicTuneParam> &TuneList);	
	bool                       SaveDynamicTuneParamList(LPCTSTR filename, const std::vector<TDynamicTuneParam> &BestList, const std::vector<TDynamicTuneParam> &GroupList);	
	bool                       GroupTuneParamList(const std::vector<TDynamicTuneParam> &SrcList, std::vector<TDynamicTuneParam> &DstList);//群組化列表
	bool                       FindBestTuneParamList(const std::vector<TDynamicTuneParam> &SrcList, std::vector<TDynamicTuneParam> &DstList);//找最好化列表
	//---------------------------------------------------------------------------------//
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaliPaneDynamic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCaliPaneDynamic)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnPosGoBtnA();	
	afx_msg void OnPosGoBtnB();	
	afx_msg void OnPosGoBtnC();	
	afx_msg void OnBuildPatternBtn();
	afx_msg void OnTestBtnAA();
	afx_msg void OnTestBtnAB();
	afx_msg void OnTestBtnAC();
	afx_msg void OnGrabBtn();
	afx_msg void OnSelchangeSliceCombo();
	afx_msg void OnItemchangedResultListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPosGetBtnA();
	afx_msg void OnPosGetBtnB();
	afx_msg void OnPosGetBtnC();	
	afx_msg void OnMotionWndBtn();
	afx_msg void OnJogChk();
	afx_msg void OnSetBCPosBtn();
	afx_msg void OnAutoTuneBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIPANEDYNAMIC_H__2F1D66A2_8D66_4EF6_A784_8C35BB3E06CA__INCLUDED_)
