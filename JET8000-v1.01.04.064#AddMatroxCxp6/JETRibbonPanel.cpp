// JETRibbonPanel.cpp : 實作檔
//

#include "stdafx.h"
#include "JETRibbonPanel.h"


// CJETRibbonPanel

IMPLEMENT_DYNAMIC(CJETRibbonPanel, CMFCRibbonPanel)

CJETRibbonPanel::CJETRibbonPanel(LPCTSTR lpszName, HICON hIcon):CMFCRibbonPanel(lpszName, hIcon)
{

}

CJETRibbonPanel::CJETRibbonPanel(CMFCRibbonGallery* pPaletteButton):CMFCRibbonPanel(pPaletteButton)
{
}

CJETRibbonPanel::~CJETRibbonPanel()
{
}

void CJETRibbonPanel::SetName(LPCTSTR lpszName)
{
	CMFCRibbonPanel::m_strName = lpszName;
}

BEGIN_MESSAGE_MAP(CJETRibbonPanel, CMFCRibbonPanel)
END_MESSAGE_MAP()



// CJETRibbonPanel 訊息處理常式


