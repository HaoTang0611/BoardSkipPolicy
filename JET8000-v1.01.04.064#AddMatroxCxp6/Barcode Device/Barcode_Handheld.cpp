// Barcode_Handheld.cpp: implementation of the CBarcode_Handheld class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Barcode_Handheld.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CBarcode_Handheld::CBarcode_Handheld()
{
	PreInitBarcode_Handheld();
	InitialBarcode_Handheld();
}
//-------------------------------------------------------------------------------------//
CBarcode_Handheld::CBarcode_Handheld(const CBarcode_Handheld &barcode)
{
	PreInitBarcode_Handheld();
	CloneBarcode_Handheld(barcode);
}
//-------------------------------------------------------------------------------------//
CBarcode_Handheld::~CBarcode_Handheld()
{

}
//-------------------------------------------------------------------------------------//
CBarcode_Handheld& CBarcode_Handheld::operator=(const CBarcode_Handheld &barcode)
{
	if ( this == &barcode ) { return *this; }
	CloneBarcode_Handheld(barcode);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Handheld::PreInitBarcode_Handheld()
{
}
//-------------------------------------------------------------------------------------//
void CBarcode_Handheld::InitialBarcode_Handheld()
{
	m_BarcodeProjectPtr = NULL;
	m_BarcodeInfoList.clear();
}
//-------------------------------------------------------------------------------------//
void CBarcode_Handheld::CloneBarcode_Handheld(const CBarcode_Handheld &barcode)
{
	m_BarcodeProjectPtr = barcode.m_BarcodeProjectPtr;
	m_BarcodeInfoList = barcode.m_BarcodeInfoList;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CBarcode_Handheld::GetProjectPtr()
{
	return m_BarcodeProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Handheld::SetProjectPtr(CAOIProject *Ptr)
{
	m_BarcodeProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
void CBarcode_Handheld::ClearBarcodeInfoList()
{
	m_BarcodeInfoList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CBarcode_Handheld::GetBarcodeInfoCount() const
{
	return m_BarcodeInfoList.size();
}
//-------------------------------------------------------------------------------------//
TBarcodeInfo* CBarcode_Handheld::GetBarcodeInfoPtr(size_t idx, bool bCheck)
{
	if ( true == bCheck )
	{
		const size_t Count = m_BarcodeInfoList.size();
		if ( idx >= Count ) 
		{	return NULL; }
	}
	return &m_BarcodeInfoList[idx];
}
//-------------------------------------------------------------------------------------//
void CBarcode_Handheld::AddBarcodeInfo(const TBarcodeInfo &BarcodeInfo)
{
	size_t        i=0;
	TBarcodeInfo *BarcodeInfoPtr=NULL;
	const size_t  BarcodeInfoCount = GetBarcodeInfoCount();

	for ( i=0; i<BarcodeInfoCount; i++ )
	{
		BarcodeInfoPtr = GetBarcodeInfoPtr(i, false);
		if ( NULL == BarcodeInfoPtr ) { continue; }
		if ( BarcodeInfoPtr->pPanel != BarcodeInfo.pPanel ) { continue; }
		if ( BarcodeInfoPtr->pBoard != BarcodeInfo.pBoard ) { continue; }
		if ( BarcodeInfoPtr->nPanelIndex != BarcodeInfo.nPanelIndex ) { continue; }
		if ( BarcodeInfoPtr->nBoardIndex != BarcodeInfo.nBoardIndex ) { continue; }
		if ( BarcodeInfoPtr->wsBarcode != BarcodeInfo.wsBarcode ) { continue; }		
		return;
	}
	m_BarcodeInfoList.push_back(BarcodeInfo);
}
//-------------------------------------------------------------------------------------//
void CBarcode_Handheld::CloneBarcodeInfoList(std::vector<TBarcodeInfo> &InfoList)
{
	InfoList = m_BarcodeInfoList;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Handheld::CheckBarcodeRepeated(const char *Barcode)//確認條碼是否重複
{
	if ( NULL == Barcode ) { return true; }

	size_t        i=0;
	std::wstring  wsBarcode;
	TBarcodeInfo *BarcodeInfoPtr=NULL;
	const size_t  BarcodeInfoCount = GetBarcodeInfoCount();

	JetAPI::char2wstring(Barcode, wsBarcode);
	for ( i=0; i<BarcodeInfoCount; i++ )
	{
		BarcodeInfoPtr = GetBarcodeInfoPtr(i, false);
		if ( NULL == BarcodeInfoPtr ) { continue; }
		if ( BarcodeInfoPtr->wsBarcode == wsBarcode ) 
		{	return true; }
		
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CBarcode_Handheld::CheckBarcodeRepeated(const wchar_t *Barcode)//確認條碼是否重複
{
	if ( NULL == Barcode ) { return true; }

	size_t        i=0;
	std::wstring  wsBarcode = Barcode;
	TBarcodeInfo *BarcodeInfoPtr=NULL;
	const size_t  BarcodeInfoCount = GetBarcodeInfoCount();

	for ( i=0; i<BarcodeInfoCount; i++ )
	{
		BarcodeInfoPtr = GetBarcodeInfoPtr(i, false);
		if ( NULL == BarcodeInfoPtr ) { continue; }
		if ( BarcodeInfoPtr->wsBarcode == wsBarcode ) 
		{	return true; }
		
	}
	return false;
}
//-------------------------------------------------------------------------------------//