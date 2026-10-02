#pragma once
// ProjectMapSpecRegionWnd.h : header file
//
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#include "ImageWnd.h"
#include "JetMemDC.h"
#include "ColorGroup.h"
#include "JETListCtrl.h"
#include "ColorRGBVWnd.h"
#include "ProjectMapMaskWnd.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_21     CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CProjectMapMaskWnd dialog
//-------------------------------------------------------------------------------------//
class CProjectMapSpecRegionWnd : public CProjectMapMaskWnd
{
	// Construction
public:
	CProjectMapSpecRegionWnd(CWnd* pParent = NULL);   // standard constructor
	~CProjectMapSpecRegionWnd();
	enum { IDD = IDD_PROJECT_MAP_SPEC_REGION_WND };
private:
	virtual BOOL               OnInitDialog();
	virtual void               RedrewWnd() override;
	virtual bool               BuildShowImage() override;
	virtual void               BuildColorFilterImage(CColorRGBV *rgbvPtr) override;
	void                       SwitchMultiLanguage();
	void                       UpdateCursor(const POINT &pt);
	void                       SetComboMaskIndex(int Index);
	void                       DrawPie(HDC hDC, const RECT &Rect);
	void                       DrawSpecRng(HDC hDC);
	void                       DrawEditGrid(HDC hDC);
	bool                       CalcSpecStageRegion();
	bool                       RebuildProjectSpecComponent();
	bool                       LoadProjectSpecComponent();
	bool                       RefreshEditGrid();
	bool                       ExecEditRect(const POINT pt);
	bool                       ModifyMaskEditImageRect();
	bool                       ExecAddRegion(TREGION4D NewRegion);
	bool                       ModifyColorMaskSpecRegion();
protected:
	TREGION4D m_StageTotalRegion;
	std::vector<CAOIComponent*> m_SpecComponentList;

public:
	bool                       GetSpecTotalStageRegion(TREGION4D &StageRegion);
private:

	int                        m_EditGridW;
	int                        m_EditGridH;
	std::vector<TEditGrid>     m_EditGridList;
	EDIT_GRID_MODE             m_EditRectMode;
	int                        m_EditRectIndex;
	bool                       m_EditingRect;
	CComboBox                  m_RegionIndexComobx;
	int                        m_RegionActiveIndex;
	std::vector<TREGION4D>     m_SpecRegionList;
	TREGION4D                  m_SpecTotalRegion;
	IMAGE_PTR                  m_SpecRegionMaskImagePtr;
	//std::vector<CColorGroup>   m_SpecRegionColorGroupList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX) override;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point) ;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRegionAddBtn() ;
	afx_msg void OnRegionEraseBtn();
	afx_msg void OnRegionClearBtn();
	afx_msg void OnRegionColorBtn();
	afx_msg void OnSelchangeRegionIndexCombo();
	afx_msg void OnSelchangeMaskIndexCombo();
	afx_msg void OnOK();
	DECLARE_MESSAGE_MAP()

};
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//

