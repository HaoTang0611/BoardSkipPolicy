// Barcode_Handheld.h: interface for the CBarcode_Handheld class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_HANDHELD_H__EDD6069D_12E2_448F_AD5F_071DA8B05CEC__INCLUDED_)
#define AFX_BARCODE_HANDHELD_H__EDD6069D_12E2_448F_AD5F_071DA8B05CEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
class CAOIProject;
//-------------------------------------------------------------------------------------//
class CBarcode_Handheld  
{
private:
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_BarcodeProjectPtr;
	std::vector<TBarcodeInfo>  m_BarcodeInfoList;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitBarcode_Handheld();
	void                       InitialBarcode_Handheld();
	void                       CloneBarcode_Handheld(const CBarcode_Handheld &barcode);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CBarcode_Handheld();
	CBarcode_Handheld(const CBarcode_Handheld &barcode);
	virtual ~CBarcode_Handheld();
	CBarcode_Handheld& operator=(const CBarcode_Handheld &barcode);
	//---------------------------------------------------------------------------------//	
	CAOIProject*               GetProjectPtr();
	void                       SetProjectPtr(CAOIProject *Ptr);
	//---------------------------------------------------------------------------------//
	void                       ClearBarcodeInfoList();
	size_t                     GetBarcodeInfoCount() const;
	TBarcodeInfo*              GetBarcodeInfoPtr(size_t idx, bool bCheck);
	void                       AddBarcodeInfo(const TBarcodeInfo &BarcodeInfo);
	void                       CloneBarcodeInfoList(std::vector<TBarcodeInfo> &InfoList);
	bool                       CheckBarcodeRepeated(const char *Barcode);//確認條碼是否重複
	bool                       CheckBarcodeRepeated(const wchar_t *Barcode);//確認條碼是否重複
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODE_HANDHELD_H__EDD6069D_12E2_448F_AD5F_071DA8B05CEC__INCLUDED_)
