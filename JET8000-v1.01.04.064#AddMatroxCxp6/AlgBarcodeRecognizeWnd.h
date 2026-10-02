#if !defined(AFX_ALGBARCODERECOGNIZEWND_H__F22FC1B3_0CCA_4E23_803B_5E6B6456157F__INCLUDED_)
#define AFX_ALGBARCODERECOGNIZEWND_H__F22FC1B3_0CCA_4E23_803B_5E6B6456157F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgBarcodeRecognizeWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JetMemDC.h"
#include "ParamUni.h"
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_01     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBarcodeRecognizeWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgBarcodeRecognizeWnd : public CBaseDialog
{
// Construction
public:
	CAlgBarcodeRecognizeWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgBarcodeRecognizeWnd)
	enum { IDD = IDD_ALG_BARCODE_RECOGNIZE_WND };
	CEdit	m_EditCtrl;	
	CButton	m_BtnCtrl;
	CComboBox	m_ComboxCtrl;
	CThisListCtrl_01	m_ParamListCtrl;
	CStatic	m_ImageWnd;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgBarcodeRecognizeWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	void                       SetBarcodeRecognizeParam(const TALG_PARAM_BARCODE_RECOGNIZE &Param);
	void                       GetBarcodeRecognizeParam(TALG_PARAM_BARCODE_RECOGNIZE &Param) const;
	//---------------------------------------------------------------------------------//	
	bool                       SetBinaryParam(CAlgBinaryParam &BinParam);
	bool                       SetUniFrameList(unsigned int FrameIndex, const std::vector<TUNI_FRAME> &UniFrameList);
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	CString                    m_WndTitle;
	CString                    m_BarcodeText;
	CString                    m_LastImageExtName;
	//---------------------------------------------------------------------------------//	
	unsigned int               m_ImageIndex;
	std::vector<TUNI_FRAME>    m_BarcodeUniFrameList;
	//---------------------------------------------------------------------------------//	
	CAlgBinaryParam              m_BinaryParam;  
	TALG_PARAM_BARCODE_RECOGNIZE m_BarcodeParam;
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
	COLORREF                   m_BkColor;
	CJetMemDC                  m_ImageMemDC;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
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
	bool                       BuildBarcodeImage(CThisListCtrl_01 &ListCtrl, int nItem);
	bool                       ExecBuildBarcodeImage(int ParamID);
	bool                       BuildBarcodeImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr, int FinalStep);
	//---------------------------------------------------------------------------------//	
	CParamUni*                 GetActParamUni();
	void                       SetActParamUni(CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//
	bool                       BuildParamList();
	bool                       BuildParamListWnd();
	bool                       BuildParamListWndHeader();
	//---------------------------------------------------------------------------------//
	void                       SetDescriptionText(const CParamUni *Ptr);	
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();	
	void                       DrawImageWnd();
	void                       CreateBKImageWnd();
	void                       DrawImage(HDC hDC, const RECT &Rect);	
	//---------------------------------------------------------------------------------//
	bool                       ExecBarcodeRecogine();
	bool                       ShowBarcodeLetterList();
	//---------------------------------------------------------------------------------//
	bool                       ExecReleaseParamCtrl();
	//---------------------------------------------------------------------------------//
	bool                       ExecItemchangedParamListWnd(CThisListCtrl_01 &ListCtrl, int nItem);
	bool                       ExecDblclkParamListWnd(CThisListCtrl_01 &ListCtrl, int nItem, int nSubItem);
	bool                       ExecUpdateParamByEdit();
	bool                       ExecUpdateParamByCombox();
	//---------------------------------------------------------------------------------//
	void                       SetBarcodeResultText(LPCTSTR Text);
	bool                       SetBarcodeParameterStringByID(UINT ParamID, TALG_PARAM_BARCODE_RECOGNIZE &BarcodeParam, LPCTSTR String);//設定條碼參數
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgBarcodeRecognizeWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnLoadImageBtn();
	afx_msg void OnBarcodeRecognizeBtn();
	afx_msg void OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkParamListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusParamEdit();
	afx_msg void OnSelchangeParamCombo();
	afx_msg void OnKillfocusParamCombo();
	afx_msg void OnSaveImageBtn();
	afx_msg void OnShowLetterListBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGBARCODERECOGNIZEWND_H__F22FC1B3_0CCA_4E23_803B_5E6B6456157F__INCLUDED_)
