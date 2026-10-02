#pragma once


// CJETRibbonPanel

class CJETRibbonPanel : public CMFCRibbonPanel
{
	DECLARE_DYNAMIC(CJETRibbonPanel)

public:
	CJETRibbonPanel(LPCTSTR lpszName = NULL, HICON hIcon = NULL);
	CJETRibbonPanel(CMFCRibbonGallery* pPaletteButton);
	virtual ~CJETRibbonPanel();
	
	void SetName(LPCTSTR lpszName);
	
protected:
	DECLARE_MESSAGE_MAP()
};


