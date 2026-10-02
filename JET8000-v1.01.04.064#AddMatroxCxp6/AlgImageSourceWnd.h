#if !defined(AFX_ALGIMAGESOURCEWND_H__863DF7AE_AF83_4473_8F41_2B98F220F7B3__INCLUDED_)
#define AFX_ALGIMAGESOURCEWND_H__863DF7AE_AF83_4473_8F41_2B98F220F7B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlgImageSourceWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "JETListCtrl.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_37     CJETListCtrl//目前使用的列表控制類別	
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgImageSourceWnd dialog
//-------------------------------------------------------------------------------------//
class CAlgImageSourceWnd : public CBaseDialog
{
// Construction
public:
	CAlgImageSourceWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlgImageSourceWnd)
	enum { IDD = IDD_ALG_IMAGE_SOURCE_WND };
	CComboBox	m_IconSizeCombox;	
	CThisListCtrl_37	m_IconListWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlgImageSourceWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//	
	unsigned int               GetUniFrameIndex() const;
	void                       SetUniFrameIndex(unsigned int val);	
	//---------------------------------------------------------------------------------//	
	IMAGE_SRC_MODE             GetImageSourceMode() const;
	void                       SetImageSourceMode(IMAGE_SRC_MODE val);		
	//---------------------------------------------------------------------------------//
	EDGE_ENHANCE_MODE          GetEdgeEnhanceMode() const;
	TBINARY_FILTER             GetEdgeEnhanceFilter1() const;
	TBINARY_FILTER             GetEdgeEnhanceFilter2() const;
	//---------------------------------------------------------------------------------//
	void                       SetUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList);
	void                       SetFrameUniqueIDList(const std::vector<unsigned int> &FrameUniqueIDList);
	//---------------------------------------------------------------------------------//
	void                       SetAlgBinaryParam(const CAlgBinaryParam &BinaryParam);		
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//		
	unsigned int               m_UniFrameIndex;	
	IMAGE_SRC_MODE             m_ImageSourceMode;
	CAlgBinaryParam            m_AlgBinaryParam;
	//---------------------------------------------------------------------------------//	
	std::vector<TUNI_FRAME>    m_UniFrameList;
	std::vector<unsigned int>  m_FrameUniqueIDList;
	CImageList                 m_IconImageList;
	bool                       m_StopIconListBeSelected;
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       BuildIconSizeCombox();
	//---------------------------------------------------------------------------------//
	void                       InitFrameRadionBtn();
	//---------------------------------------------------------------------------------//
	CString                    GetSaveImageFolder() const;
	bool                       ClearImageSourceListCtrl(CThisListCtrl_37 &ListCtrl);
	bool                       BuildImageSourceListCtrl(int index, CThisListCtrl_37 &ListCtrl);
	//---------------------------------------------------------------------------------//
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlgImageSourceWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeIconSizeCombo();
	afx_msg void OnFrameIDRad01();
	afx_msg void OnFrameIDRad02();
	afx_msg void OnFrameIDRad03();
	afx_msg void OnFrameIDRad04();
	afx_msg void OnFrameIDRad05();
	afx_msg void OnFrameIDRad06();
	afx_msg void OnFrameIDRad07();
	afx_msg void OnFrameIDRad08();
	afx_msg void OnClickIconListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedIconListWnd(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEdgeEnhanceBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALGIMAGESOURCEWND_H__863DF7AE_AF83_4473_8F41_2B98F220F7B3__INCLUDED_)
