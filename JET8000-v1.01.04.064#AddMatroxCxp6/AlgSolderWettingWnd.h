#if !defined(AFX_ALGSOLDERWETTINGWND_H__7BF116AA_8DB0_4BDA_A2CF_8AFFD3BBC82B__INCLUDED_)
#define AFX_ALGSOLDERWETTINGWND_H__7BF116AA_8DB0_4BDA_A2CF_8AFFD3BBC82B__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgSolderWettingWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_68     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgSolderWettingWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgSolderWettingWnd : public CBaseDialog
{
// Construction
public:
	CAlgSolderWettingWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgSolderWettingWnd)
	enum { IDD = IDD_ALG_SOLDER_WETTING_WND };
	CStatic	m_ImageWnd;	
	CEdit	m_EditCtrl;	
	CButton	m_BtnCtrl;
	CComboBox	m_ComboxCtrl;
	CThisListCtrl_68	m_ParamListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgSolderWettingWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	bool                       SetWndPtr(CAOIWnd *WndPtr);
	//---------------------------------------------------------------------------------//		
	void                       GetSolderWettingParam(TALG_PARAM_SOLDER_WETTING &Param) const;	
	//---------------------------------------------------------------------------------//		
	bool                       SetBinaryParam(CAlgBinaryParam &BinParam);
	bool                       SetUniFrameList(unsigned int FrameIndex, const std::vector<TUNI_FRAME> &UniFrameList);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	CAOIWnd                   *m_WndPtr;
	CAlgBinaryParam            m_BinaryParam;  
	unsigned int               m_ImageIndex;	
	std::vector<TUNI_FRAME>    m_WndUniFrameList;
	TALG_PARAM_SOLDER_WETTING  m_SolderWettingParam;
	//---------------------------------------------------------------------------------//	
	CParamUni                 *m_ParamActPtr;
	CParamList                 m_ParamList;
	bool                       m_StopParamListBeSelected;
	//---------------------------------------------------------------------------------//
	IMAGE_PTR                  m_ImagePtr;
	IMAGE_SIZE                 m_ImageW;
	IMAGE_SIZE                 m_ImageH;
	IMAGE_SIZE                 m_ImageStep;
	IMAGE_SIZE                 m_ImageBitCount;
	//---------------------------------------------------------------------------------//
	bool                       m_ImageEnhanced;
	//---------------------------------------------------------------------------------//	
	IMAGE_PTR                  m_ShowPtr;
	IMAGE_SIZE                 m_ShowW;
	IMAGE_SIZE                 m_ShowH;
	IMAGE_SIZE                 m_ShowStep;
	IMAGE_SIZE                 m_ShowSize;   
	IMAGE_SIZE                 m_ShowBitCount;
	BITMAPINFO                *m_BitmapInfoPtr;
	//---------------------------------------------------------------------------------//
	double                     m_ViewZoom;
	COLORREF                   m_BkColor;
	CJetMemDC                  m_ImageMemDC;
	CJetMemDC                  m_ImageMemDC2;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();	
	bool                       SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID=true);
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       UpdateResultToUI();
	//---------------------------------------------------------------------------------//
	void                       ReleaseImageBuffer();	
	bool                       LoadImageFile(LPCTSTR filename);
	bool                       SwitchImageBuffer(unsigned int ImageIndex, std::vector<TUNI_FRAME> &UniFrameList);
	IMAGE_PTR                  GetImageBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount);	
	//---------------------------------------------------------------------------------//	
	bool                       ReleaseShowBuffer();
	IMAGE_PTR                  GetShowBuffer(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount);	
	bool                       BuildShowBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);		
	//---------------------------------------------------------------------------------//	
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//	
	bool                       BuildParamList();
	void                       ClearParamListWnd();
	bool                       BuildParamListWnd();	
	bool                       UpdateParamListWnd();	
	bool                       BuildParamListWndHeader();
	//---------------------------------------------------------------------------------//
	void                       RedrawWnd();	
	void                       DrawImageWnd();
	void                       CreateBKImageWnd();
	void                       DrawImage(HDC hDC, const RECT &Rect);	
	void                       DrawCircleAngle(HDC hDC, const RECT &Rect);	
	//---------------------------------------------------------------------------------//	
	void                       TestWnd();
	bool                       ExecTestBtn(CAOIWnd *WndPtr);
	//---------------------------------------------------------------------------------//		
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_68 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_68 &ListCtrl, int nItem, int nSubItem);
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseParamCtrl();	
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//	
	void                       SetDescriptionText(const CParamUni *Ptr);	
	bool                       SetSolderWettingParamStringByID(UINT ParamID, TALG_PARAM_SOLDER_WETTING &swParam, LPCTSTR String);//設定焊接參數
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgSolderWettingWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnTestBtn();
	afx_msg void OnShowCircleAngleChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_ALGSOLDERWETTINGWND_H__7BF116AA_8DB0_4BDA_A2CF_8AFFD3BBC82B__INCLUDED_)
