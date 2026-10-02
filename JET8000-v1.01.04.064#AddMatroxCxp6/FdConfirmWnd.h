#if !defined(AFX_FDCONFIRMWND_H__14EE376B_8BEF_4E05_99DF_95946A7A2055__INCLUDED_)
#define AFX_FDCONFIRMWND_H__14EE376B_8BEF_4E05_99DF_95946A7A2055__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FdConfirmWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "AOI_Kernel\\AOIFd.h"
#include "ImageWnd.h"
#include "ProjectMapWnd.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CFdConfirmWnd dialog
//-------------------------------------------------------------------------------------//
class CFdConfirmWnd : public CBaseDialog
{
// Construction
public:
	CFdConfirmWnd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFdConfirmWnd)
	enum { IDD = IDD_FD_CONFIRM_WND };
	CImageWnd	m_ImageWnd;
	CImageWnd	m_PatternWnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFdConfirmWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	//---------------------------------------------------------------------------------//		
	bool                       GetFdModified() const;	
	//---------------------------------------------------------------------------------//
	void                       SetFdPtr(CAOIFd *FdPtr);	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//		
	CAOIFd                    *m_FdPtr;
	CAOIWnd                   *m_FdWndPtr;
	CAOIProject               *m_ProjectPtr;
	CAOIFd*                    GetFdPtr();
	CAOIWnd*                   GetFdWndPtr();
	double                     m_FdScore;
	CString                    m_FdString;
	bool                       m_FdModified;
	//---------------------------------------------------------------------------------//	
	TSIZE2D                    m_FdSize;
	TRECT4D                    m_FdImageRect4d;
	TPOINT2D                   m_FdTeachPos;
	TPOINT2D                   m_FdStagePos;
	TPOINT2D                   m_FdImageRes;
	TPOINT2D                   m_FdImageOffset;	
	unsigned int               m_FrameIndex;
	std::vector<TUNI_FRAME>    m_UniFrameList;
	std::vector<TUNI_FRAME>    m_PatternFrameList;
	CProjectMapWnd             m_ProjectMapWnd;
	//---------------------------------------------------------------------------------//	
	IMAGE_PTR                  m_BufferPtr;
	size_t                     m_BufferSize;
	void                       ReleaseBuffer();
	//---------------------------------------------------------------------------------//	
	void                       SwitchMultiLanguage();	
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);	
	//---------------------------------------------------------------------------------//
	void                       SetFdModified(double val);
	//---------------------------------------------------------------------------------//
	bool                       SwitchFdUniFrameList();
	bool                       SwitchFdUniFrameList(unsigned int FrameIndex);
	//---------------------------------------------------------------------------------//	
	bool                       OnImageWndNotify(WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//	
	void                       RedrawWnd();
	void                       DrawRoiImage();
	void                       DrawPatternImage();
	void                       DrawRoiImage(HDC hDC, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);
	void                       DrawPatternImage(HDC hDC, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr);
	//---------------------------------------------------------------------------------//	
	bool                       ExecSaveImageBtn(LPCTSTR filename);
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFdConfirmWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnPaint();
	virtual void OnOK();
	afx_msg void OnShowProjectMapBtn();
	afx_msg void OnPatternAddBtn();
	afx_msg void OnParamSetBtn();
	afx_msg void OnSaveImageBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FDCONFIRMWND_H__14EE376B_8BEF_4E05_99DF_95946A7A2055__INCLUDED_)
