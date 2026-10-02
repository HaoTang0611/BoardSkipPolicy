#if !defined(AFX_BARCODECONFIRMWND_H__745E6019_8BAF_4C2D_8F3D_71B2E703D0C1__INCLUDED_)
#define AFX_BARCODECONFIRMWND_H__745E6019_8BAF_4C2D_8F3D_71B2E703D0C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarcodeConfirmWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "ProjectMapWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CBarcodeConfirmWnd dialog
//-------------------------------------------------------------------------------------//
class CBarcodeConfirmWnd : public CBaseDialog
{
// Construction
public:
	CBarcodeConfirmWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBarcodeConfirmWnd)
	enum { IDD = IDD_BARCODE_CONFIRM_WND };
	CImageWnd	m_ImageWnd;
	CComboBox	m_UniFrameCombox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarcodeConfirmWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
public:
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetBarcodeContext() const;
	void                       SetBarcodeContext(LPCSTR val);
	void                       SetImageIndex(unsigned int val);	
	void                       SetActiveProject(CAOIProject *Ptr);
	void                       SetActiveBarcodePtr(CAOIBarcode *Ptr);
	void                       SetActiveComponentPtr(CAOIComponent *Ptr);
	bool                       SetUniFrameList(unsigned int FrameIndex, const std::vector<TUNI_FRAME> &UniFrameList);
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//				
	CString                    m_Name;
	unsigned int               m_ImageIndex;
	unsigned int               m_ImageIndexDefault;
	unsigned int               m_PanelIndex;
	unsigned int               m_BoardIndex;		
	CAOIProject               *m_ProjectPtr;
	CAOIBarcode               *m_BarcodePtr;
	CAOIComponent             *m_ComponentPtr;
	CString                    m_BarcodeContext;
	std::vector<TUNI_FRAME>    m_BarcodeUniFrameList;	
	CProjectMapWnd             m_ProjectMapWnd;
	//---------------------------------------------------------------------------------//		
	IMAGE_PTR                  m_ShowImagePtr;
	IMAGE_SIZE                 m_ShowBufferSize;
	//---------------------------------------------------------------------------------//		
	CAOIProject*               GetActiveProject();
	CAOIBarcode*               GetActiveBarcode();	
	CAOIComponent*             GetActiveComponent();	
	//---------------------------------------------------------------------------------//		
	void                       BuildUniFrameCombox();
	void                       AdjustCtrlWndPosition();
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	bool                       CreateShowBuffer();
	bool                       ReleaseShowImageBuffer();
	//---------------------------------------------------------------------------------//		
	void                       RedrawWnd();
	void                       DrawWndBoxRect();
	bool                       UpdateFrameImage();	
	//---------------------------------------------------------------------------------//	
	void                       EnableCloseBtn(BOOL bEnable);//±Ò¥ÎÃö³¬«ö¶s
	//---------------------------------------------------------------------------------//
	bool                       ExecSaveImageBtn(LPCTSTR  filename);
	bool                       ExecSaveImageBtn_v1(LPCTSTR filename);
	bool                       ExecSaveImageBtn_v2(LPCTSTR filename);
	bool                       ExecSaveImageBtnFn(unsigned int Index, LPCTSTR filename);
	//---------------------------------------------------------------------------------//
	bool                       ExecBarcodeRecognize();
	bool                       ExecBarcodeRecognize(CAOIBarcode *BarcodePtr);
	//---------------------------------------------------------------------------------//
	bool                       CheckBarcodeContextValid(LPCTSTR Code);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarcodeConfirmWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeUniFrameCombo();
	afx_msg void OnPCBBackBtn();
	afx_msg void OnPCBOutBtn();		
	virtual void OnOK();
	afx_msg void OnShowProjectMapBtn();
	afx_msg void OnSaveImageBtn();
	afx_msg void OnRecognizeBtn();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARCODECONFIRMWND_H__745E6019_8BAF_4C2D_8F3D_71B2E703D0C1__INCLUDED_)
