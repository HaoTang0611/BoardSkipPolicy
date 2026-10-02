#if !defined(AFX_IMAGEPHASEALLWND_H__0881AD9C_1A79_4879_A16B_1A09B599107F__INCLUDED_)
#define AFX_IMAGEPHASEALLWND_H__0881AD9C_1A79_4879_A16B_1A09B599107F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImagePhaseAllWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "Draw3DWnd.h"
#include "ImagePhaseWnd.h"
//-------------------------------------------------------------------------------------//
#define MAX_PAHSE_COUNT     4
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CImagePhaseAllWnd dialog
//-------------------------------------------------------------------------------------//
class CImagePhaseAllWnd : public CBaseDialog
{
// Construction
public:
	CImagePhaseAllWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImagePhaseAllWnd)
	enum { IDD = IDD_IMAGE_PHASE_ALL_WND };
	CComboBox	m_DLPLEDCombox;
	CComboBox   m_OpenMPCombox;
	CComboBox   m_SpaceMergeModeCombox;
	CComboBox   m_SpaceMergeBestModeCombox;
	CComboBox   m_SpaceMergeIntensityModeCombox;
	CComboBox   m_NoiseDefineModeCombox;	
	CComboBox	m_MergeRecursionModeCombox;	
	CComboBox	m_FirstFilterModeCombox;
	CComboBox	m_OverLowModeCombox;	
	CComboBox	m_HeightUnexpectedModeCombox;	
	CComboBox	m_FinalFilterModeCombox;
	CComboBox	m_FinalFilterModeCombox2;	
	CStatic	m_ImageWnd4;
	CStatic	m_ImageWnd3;
	CStatic	m_ImageWnd2;
	CStatic	m_ImageWnd1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImagePhaseAllWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CImagePhaseWnd             m_PhaseWnd;
	CDraw3DWnd                 m_Draw3DWnd;//3D繪圖視窗
	//---------------------------------------------------------------------------------//	
	UINT                       m_ShowModeID;
	SLICE_FUNC_MODE            m_SliceFuncMode;
	TPhaseNoiseParam           m_PhaseNoiseParam;
	TNoiseFilterParam          m_NoiseFilterParam;
	//---------------------------------------------------------------------------------//	
	POINT                      m_MovingPos;
	POINT                      m_RBtnUpPos;
	POINT                      m_RBtnDownPos;
	POINT                      m_LBtnUpPos;
	POINT                      m_LBtnDownPos;	
	double                     m_ImageZoom;	
	TPOINT2D                   m_ImageOffset;	
	TPOINT2D                   m_ImagePt;
	COLORREF                   m_BkColor;
	//---------------------------------------------------------------------------------//		
	TPOINT2D                   m_ImageWndPt1;
	TPOINT2D                   m_ImageWndPt2;	
	//---------------------------------------------------------------------------------//	
	TPOINT2D                   m_ImagePt1;
	TPOINT2D                   m_ImagePt2;	
	//---------------------------------------------------------------------------------//	
	TPOINT2D                   m_ImagePtMark1;
	TPOINT2D                   m_ImagePtMark2;	
	//---------------------------------------------------------------------------------//	
	RECT                       m_ImageWndRect1;
	CJetMemDC                  m_ImageWndMemDC1;
	//---------------------------------------------------------------------------------//	
	RECT                       m_ImageWndRect2;
	CJetMemDC                  m_ImageWndMemDC2;
	//---------------------------------------------------------------------------------//	
	RECT                       m_ImageWndRect3;
	CJetMemDC                  m_ImageWndMemDC3;
	//---------------------------------------------------------------------------------//	
	RECT                       m_ImageWndRect4;
	CJetMemDC                  m_ImageWndMemDC4;
	//---------------------------------------------------------------------------------//	
	TCastParam                 m_CastParam[MAX_PAHSE_COUNT];
	CString                    m_PhaseFileName[MAX_PAHSE_COUNT];
	double                     m_EllapseTime[MAX_PAHSE_COUNT];
	BOOL                       m_bClonePhase[MAX_PAHSE_COUNT];
	BOOL                       m_bCloneSpace[MAX_PAHSE_COUNT];
	IMAGE_SIZE                 m_ImageW[MAX_PAHSE_COUNT];
	IMAGE_SIZE                 m_ImageH[MAX_PAHSE_COUNT];	
	IMAGE_SIZE                 m_ShowStep[MAX_PAHSE_COUNT];
	IMAGE_SIZE                 m_ImageStep[MAX_PAHSE_COUNT];
	IMAGE_SIZE                 m_PhaseStep[MAX_PAHSE_COUNT];
	IMAGE_SIZE                 m_SpaceStep[MAX_PAHSE_COUNT];
	IMAGE_SIZE                 m_BitCount[MAX_PAHSE_COUNT];	
	MASK_PTR                   m_MaskBuffer[MAX_PAHSE_COUNT];
	IMAGE_PTR                  m_ShowBuffer[MAX_PAHSE_COUNT];
	IMAGE_PTR                  m_ImageBuffer[MAX_PAHSE_COUNT];
	PHASE_PTR                  m_PhaseBuffer[MAX_PAHSE_COUNT];
	SPACE_PTR                  m_SpaceBuffer[MAX_PAHSE_COUNT];
	double                     m_PhaseMax[MAX_PAHSE_COUNT];
	double                     m_PhaseMin[MAX_PAHSE_COUNT];
	double                     m_PhaseAve[MAX_PAHSE_COUNT];	
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_ImageW_M;
	IMAGE_SIZE                 m_ImageH_M;
	IMAGE_SIZE                 m_BitCount_M;
	IMAGE_SIZE                 m_ImageStep_M;
	IMAGE_SIZE                 m_PhaseStep_M;
	IMAGE_SIZE                 m_SpaceStep_M;
	MASK_PTR                   m_MaskBuffer_M;
	IMAGE_PTR                  m_ImageBuffer_M;
	PHASE_PTR                  m_PhaseBuffer_M;
	SPACE_PTR                  m_SpaceBuffer_M;
	//---------------------------------------------------------------------------------//	
	void                       ReleaseBuffer_M();
	void                       ReleaseBuffer(size_t idx);	
	//---------------------------------------------------------------------------------//
	void                       AdjustCtrlWnd(int cx=-1, int cy=-1);	
	void                       DrawImageWndMemDC(size_t idx);
	void                       UpdateShowImage(UINT ID, size_t idx);	
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	void                       UpdateInfoEdit(UINT EditID, TPOINT2D ImagePt);
	//---------------------------------------------------------------------------------//
	SLICE_FUNC_MODE            GetDlpSliceFuncMode() const;
	//---------------------------------------------------------------------------------//
	void                       ExecLoadFileBtn(size_t idx, LPCTSTR pfilename);
	void                       ExecLoadPhaseBtn(size_t idx);
	void                       ExecMergeSpace();//合併不同投光的空間資料
	void                       ExecMergePhase();//合併相同投光的相位資料
	//---------------------------------------------------------------------------------//	
	bool                       ExecMouseWheelMSG(UINT message, WPARAM wParam, LPARAM lParam);
	bool                       ExecMouseWheelEvent(UINT nFlags, short zDelta, CPoint pt);
	//---------------------------------------------------------------------------------//
	bool                       ExecPhaseCompare_JET6500();	
	bool                       ExecMedianFilter_Debug();
	bool                       CreateMedianFilterSample_Debug();
	bool                       ExecConvertPhaseFactor_JET6500();	
	bool                       SaveKValeFile_JET6500(LPCTSTR filename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, float *Ptr);
	bool                       SavePhaseFile_JET6500(LPCTSTR filename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, short *Ptr);
	bool                       SavePhaseImage_Debug(LPCTSTR filename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, PHASE_PTR Ptr);
	bool                       SavePhaseMask_Debug(LPCTSTR filename, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR Ptr, int Mask);
	//---------------------------------------------------------------------------------//
	bool                       SaveRectParam();
	bool                       LoadRectParam();
	//---------------------------------------------------------------------------------//
	void                       UpdateSpaceNoiseFilterParamToUI(const TNoiseFilterParam &NoiseFilterParam);
	void                       UpdateSpaceNoiseFilterParamFromUI(TNoiseFilterParam &NoiseFilterParam) const;
	//---------------------------------------------------------------------------------//	
	bool                       TestDLPZeroBinFile();
	bool                       TestDLPFactorBinFile();
	bool                       LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr);//載入DLP平面相位
	bool                       LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr);//載入DLP平面係數
	//---------------------------------------------------------------------------------//
	bool                       CheckNeedReloadRawFile(const TPhaseNoiseParam &NewParam);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImagePhaseAllWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual void OnOK();
	afx_msg void OnLoadPhaseBtn1();
	afx_msg void OnLoadPhaseBtn2();
	afx_msg void OnLoadPhaseBtn3();
	afx_msg void OnLoadPhaseBtn4();
	afx_msg void OnMergePhaseBtn();
	afx_msg void OnCalculateAllBtn();
	afx_msg void OnShowImageRadio();
	afx_msg void OnShowPhaseRadio();
	afx_msg void OnShowSpaceRadio();
	afx_msg void OnEnhanceImageChk();
	afx_msg void OnCompareSpaceBtn();
	afx_msg void OnNFDisableBtn();
	afx_msg void OnConvertPhaseFactorBtn();
	afx_msg void OnPhaseCompareBtn();
	afx_msg void OnMedianDebugBtn();
	afx_msg void OnUseCudaChk();	
	afx_msg void OnLockRectBtn();
	afx_msg void OnBasePlaneParamBtn();
	afx_msg void OnSpaceNoiseFilterBtn();
	afx_msg void OnLoadParamBtn();
	afx_msg void OnSaveImageBtn();
	afx_msg void OnSetPatternTypeBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPHASEALLWND_H__0881AD9C_1A79_4879_A16B_1A09B599107F__INCLUDED_)
