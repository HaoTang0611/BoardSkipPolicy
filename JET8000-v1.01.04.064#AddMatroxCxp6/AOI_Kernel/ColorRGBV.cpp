// ColorRGBV.cpp: implementation of the CColorRGBV class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ColorRGBV.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CColorRGBV::CColorRGBV()
{
	CColorRGBV::PreInitColorRGBV();
	CColorRGBV::InitialColorRGBV();
}
//-------------------------------------------------------------------------------------//
CColorRGBV::CColorRGBV(const CColorRGBV &rgbv)
{
	CColorRGBV::PreInitColorRGBV();
	CColorRGBV::CloneColorRGBV(rgbv);
}
//-------------------------------------------------------------------------------------//
CColorRGBV::~CColorRGBV()
{

}
//-------------------------------------------------------------------------------------//
CColorRGBV& CColorRGBV::operator=(const CColorRGBV &rgbv)
{
	if ( &rgbv == this ) { return *this; }
	CColorRGBV::CloneColorRGBV(rgbv);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CColorRGBV::PreInitColorRGBV()
{	
}
//-------------------------------------------------------------------------------------//
inline void CColorRGBV::InitialColorRGBV()
{
	m_RGBVColorMode = COLOR_RGBV_RED;            //色彩模式
	m_RGBVLogicMode = COLOR_LOGIC_INCLUDE;       //邏輯模式	
	//---------------------------------------------------------------------------------//
	m_RGBVRedMax = 255;                 //紅色上限
	m_RGBVRedMin = 0;                 //紅色下限 
	m_RGBVRedEnabled = false;             //紅色啟用

	m_RGBVGreenMax = 255;               //綠色上限
	m_RGBVGreenMin = 0;               //綠色下限 
	m_RGBVGreenEnabled = false;           //綠色啟用

	m_RGBVBlueMax = 255;                //藍色上限
	m_RGBVBlueMin = 0;                //藍色下限 
	m_RGBVBlueEnabled = false;            //藍色啟用

	m_RGBVValueMax = 255;                //灰色上限
	m_RGBVValueMin = 0;                //灰色下限 
	m_RGBVValueEnabled = false;            //灰色啟用

	m_RGBVUsed = false;                //是否使用
	m_RGBVActived = false;
	m_RGBVShowColor = 0xFFFFFF;//顯示的顏色
}
//-------------------------------------------------------------------------------------//
inline void CColorRGBV::CloneColorRGBV(const CColorRGBV &rgbv)
{
	m_RGBVColorMode = rgbv.m_RGBVColorMode;
	m_RGBVLogicMode = rgbv.m_RGBVLogicMode;	
	m_RGBVActived = rgbv.m_RGBVActived;
	CopyColorRGBV(rgbv);
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::WriteColorRGBVFile(CAOIFileIO &FileIO)
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	CColorRGBV *rgbvPtr = this;

	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_COLOR_MODE, rgbvPtr->GetColorMode()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_LOGIC_MODE, rgbvPtr->GetLogicMode()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_RED_MAX, rgbvPtr->GetRedMax()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_RED_MIN, rgbvPtr->GetRedMin()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_RED_ENABLED, rgbvPtr->GetRedEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_GREEN_MAX, rgbvPtr->GetGreenMax()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_GREEN_MIN, rgbvPtr->GetGreenMin()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_GREEN_ENABLED, rgbvPtr->GetGreenEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_BLUE_MAX, rgbvPtr->GetBlueMax()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_BLUE_MIN, rgbvPtr->GetBlueMin()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_BLUE_ENABLED, rgbvPtr->GetBlueEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_VALUE_MAX, rgbvPtr->GetValueMax()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_VALUE_MIN, rgbvPtr->GetValueMin()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_VALUE_ENABLED, rgbvPtr->GetValueEnabled()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::ReadColorRGBVFile(CAOIFileIO &FileIO)
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int     index = 0;	
	CColorRGBV *rgbvPtr = this;
	while ( FileIO.CheckFileEnd() == false )
	{
		if ( FileIO.LoadChunk(index) == false ) 
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_COLOR_RGBV_START://RGBV顏色參數-起點
			break;
		case FILE_IO_COLOR_RGBV_END://RGBV顏色參數-終點
			rgbvPtr->CheckUsed();
			rgbvPtr->CalcShowColor();
			return true;
			break;
		case FILE_IO_COLOR_RGBV_COLOR_MODE://RGBV顏色參數-色彩模式
			rgbvPtr->SetColorMode((COLOR_RGBV_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_COLOR_RGBV_LOGIC_MODE://RGBV顏色參數-色彩邏輯
			rgbvPtr->SetLogicMode((COLOR_LOGIC_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_COLOR_RGBV_RED_MAX://RGBV顏色參數-紅色上限
			rgbvPtr->SetRedMax((FileIO.GetData_INT()));
			break;
		case FILE_IO_COLOR_RGBV_RED_MIN://RGBV顏色參數-紅色下限
			rgbvPtr->SetRedMin((FileIO.GetData_INT()));
			break;
		case FILE_IO_COLOR_RGBV_RED_ENABLED://RGBV顏色參數-紅色啟用
			rgbvPtr->SetRedEnabled(FileIO.GetData_BOL());			
			break;
		case FILE_IO_COLOR_RGBV_GREEN_MAX://RGBV顏色參數-綠色上限
			rgbvPtr->SetGreenMax(FileIO.GetData_INT());
			break;
		case FILE_IO_COLOR_RGBV_GREEN_MIN://RGBV顏色參數-綠色下限
			rgbvPtr->SetGreenMin(FileIO.GetData_INT());
			break;
		case FILE_IO_COLOR_RGBV_GREEN_ENABLED://RGBV顏色參數-綠色啟用
			rgbvPtr->SetGreenEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_COLOR_RGBV_BLUE_MAX://RGBV顏色參數-藍色上限
			rgbvPtr->SetBlueMax(FileIO.GetData_INT());
			break;
		case FILE_IO_COLOR_RGBV_BLUE_MIN://RGBV顏色參數-藍色下限
			rgbvPtr->SetBlueMin(FileIO.GetData_INT());
			break;
		case FILE_IO_COLOR_RGBV_BLUE_ENABLED://RGBV顏色參數-藍色啟用
			rgbvPtr->SetBlueEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_COLOR_RGBV_VALUE_MAX://RGBV顏色參數-灰色上限
			rgbvPtr->SetValueMax(FileIO.GetData_INT());
			break;
		case FILE_IO_COLOR_RGBV_VALUE_MIN://RGBV顏色參數-灰色下限
			rgbvPtr->SetValueMin(FileIO.GetData_INT());
			break;
		case FILE_IO_COLOR_RGBV_VALUE_ENABLED://RGBV顏色參數-灰色啟用
			rgbvPtr->SetValueEnabled(FileIO.GetData_BOL());
			break;
		default:
			break;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::SetColorMax(COLOR_RGBV_MODE Mode, int value)
{
	switch ( Mode )
	{
	case COLOR_RGBV_RED:	CColorRGBV::m_RGBVRedMax = value; break;
	case COLOR_RGBV_GREEN:	CColorRGBV::m_RGBVGreenMax = value; break;
	case COLOR_RGBV_BLUE:	CColorRGBV::m_RGBVBlueMax = value; break;
	}
}
//-------------------------------------------------------------------------------------//
int CColorRGBV::GetColorMax(COLOR_RGBV_MODE Mode) const
{
	int value=COLOR_RGBV_MAX;
	switch ( Mode )
	{
	case COLOR_RGBV_RED:	value = CColorRGBV::m_RGBVRedMax; break;
	case COLOR_RGBV_GREEN:	value = CColorRGBV::m_RGBVGreenMax; break;
	case COLOR_RGBV_BLUE:	value = CColorRGBV::m_RGBVBlueMax; break;
	}
	return value;
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::SetColorMin(COLOR_RGBV_MODE Mode, int value)
{
	switch ( Mode )
	{
	case COLOR_RGBV_RED:	CColorRGBV::m_RGBVRedMin = value; break;
	case COLOR_RGBV_GREEN:	CColorRGBV::m_RGBVGreenMin = value; break;
	case COLOR_RGBV_BLUE:	CColorRGBV::m_RGBVBlueMin = value; break;
	}
}
//-------------------------------------------------------------------------------------//
int CColorRGBV::GetColorMin(COLOR_RGBV_MODE Mode) const
{
	int value=COLOR_RGBV_MIN;
	switch ( Mode )
	{
	case COLOR_RGBV_RED:	value = CColorRGBV::m_RGBVRedMin; break;
	case COLOR_RGBV_GREEN:	value = CColorRGBV::m_RGBVGreenMin; break;
	case COLOR_RGBV_BLUE:	value = CColorRGBV::m_RGBVBlueMin; break;
	}
	return value;
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::SetColorEnabled(COLOR_RGBV_MODE Mode, bool value)
{
	switch ( Mode )
	{
	case COLOR_RGBV_RED:	CColorRGBV::m_RGBVRedEnabled = value; break;
	case COLOR_RGBV_GREEN:	CColorRGBV::m_RGBVGreenEnabled = value; break;
	case COLOR_RGBV_BLUE:	CColorRGBV::m_RGBVBlueEnabled = value; break;
	}
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::GetColorEnabled(COLOR_RGBV_MODE Mode) const
{
	bool value=false;
	switch ( Mode )
	{
	case COLOR_RGBV_RED:	value = CColorRGBV::m_RGBVRedEnabled; break;
	case COLOR_RGBV_GREEN:	value = CColorRGBV::m_RGBVGreenEnabled; break;
	case COLOR_RGBV_BLUE:	value = CColorRGBV::m_RGBVBlueEnabled; break;
	}
	return value;
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::CopyColorRGBV(const CColorRGBV &rgbv)
{
	m_RGBVRedMax = rgbv.m_RGBVRedMax;
	m_RGBVRedMin = rgbv.m_RGBVRedMin;
	m_RGBVRedEnabled = rgbv.m_RGBVRedEnabled;

	m_RGBVGreenMax = rgbv.m_RGBVGreenMax;
	m_RGBVGreenMin = rgbv.m_RGBVGreenMin;
	m_RGBVGreenEnabled = rgbv.m_RGBVGreenEnabled;

	m_RGBVBlueMax = rgbv.m_RGBVBlueMax;
	m_RGBVBlueMin = rgbv.m_RGBVBlueMin;
	m_RGBVBlueEnabled = rgbv.m_RGBVBlueEnabled;

	m_RGBVValueMax = rgbv.m_RGBVValueMax;
	m_RGBVValueMin = rgbv.m_RGBVValueMin;
	m_RGBVValueEnabled = rgbv.m_RGBVValueEnabled;

	m_RGBVUsed = rgbv.m_RGBVUsed;	
	m_RGBVShowColor = rgbv.m_RGBVShowColor;
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::MoveColorBrightness(int value)
{
	int nV1=0, nV2=0;
	int nMax=0, nMin=0;
	if ( true == CColorRGBV::m_RGBVValueEnabled )
	{
		nV1 = CColorRGBV::m_RGBVValueMin + value;
		nV2 = CColorRGBV::m_RGBVValueMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVValueMin = nMin;
		CColorRGBV::m_RGBVValueMax = nMax;
	}
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::MoveColorRGBV(int value, bool OnlyColor)
{
	int nV1=0, nV2=0;
	int nMax=0, nMin=0;
	if ( true == CColorRGBV::m_RGBVRedEnabled )
	{
		nV1 = CColorRGBV::m_RGBVRedMin + value;
		nV2 = CColorRGBV::m_RGBVRedMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVRedMin = nMin;
		CColorRGBV::m_RGBVRedMax = nMax;
	}

	if ( true == CColorRGBV::m_RGBVGreenEnabled )
	{
		nV1 = CColorRGBV::m_RGBVGreenMin + value;
		nV2 = CColorRGBV::m_RGBVGreenMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVGreenMin = nMin;
		CColorRGBV::m_RGBVGreenMax = nMax;
	}

	if ( true == CColorRGBV::m_RGBVBlueEnabled )
	{
		nV1 = CColorRGBV::m_RGBVBlueMin + value;
		nV2 = CColorRGBV::m_RGBVBlueMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVBlueMin = nMin;
		CColorRGBV::m_RGBVBlueMax = nMax;
	}

	if ( false==OnlyColor && true == CColorRGBV::m_RGBVValueEnabled )
	{
		nV1 = CColorRGBV::m_RGBVValueMin + value;
		nV2 = CColorRGBV::m_RGBVValueMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVValueMin = nMin;
		CColorRGBV::m_RGBVValueMax = nMax;
	}
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::ExpandColorBrightness(int value)
{
	int nV1=0, nV2=0;
	int nMax=0, nMin=0;
	if ( true == CColorRGBV::m_RGBVValueEnabled )
	{
		nV1 = CColorRGBV::m_RGBVValueMin - value;
		nV2 = CColorRGBV::m_RGBVValueMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVValueMin = nMin;
		CColorRGBV::m_RGBVValueMax = nMax;
	}
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::ExpandColorRGBV(int value, bool OnlyColor)
{
	int nV1=0, nV2=0;
	int nMax=0, nMin=0;
	if ( true == CColorRGBV::m_RGBVRedEnabled )
	{
		nV1 = CColorRGBV::m_RGBVRedMin - value;
		nV2 = CColorRGBV::m_RGBVRedMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVRedMin = nMin;
		CColorRGBV::m_RGBVRedMax = nMax;
	}

	if ( true == CColorRGBV::m_RGBVGreenEnabled )
	{
		nV1 = CColorRGBV::m_RGBVGreenMin - value;
		nV2 = CColorRGBV::m_RGBVGreenMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVGreenMin = nMin;
		CColorRGBV::m_RGBVGreenMax = nMax;
	}

	if ( true == CColorRGBV::m_RGBVBlueEnabled )
	{
		nV1 = CColorRGBV::m_RGBVBlueMin - value;
		nV2 = CColorRGBV::m_RGBVBlueMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVBlueMin = nMin;
		CColorRGBV::m_RGBVBlueMax = nMax;
	}

	if ( false==OnlyColor && true == CColorRGBV::m_RGBVValueEnabled )
	{
		nV1 = CColorRGBV::m_RGBVValueMin - value;
		nV2 = CColorRGBV::m_RGBVValueMax + value;
		nMin = MIN(nV1, nV2);
		nMax = MAX(nV1, nV2);

		if ( nMin < COLOR_RGBV_MIN ) { nMin = COLOR_RGBV_MIN; }
		if ( nMin > COLOR_RGBV_MAX ) { nMin = COLOR_RGBV_MAX; }
		if ( nMax < COLOR_RGBV_MIN ) { nMax = COLOR_RGBV_MIN; }
		if ( nMax > COLOR_RGBV_MAX ) { nMax = COLOR_RGBV_MAX; }
		CColorRGBV::m_RGBVValueMin = nMin;
		CColorRGBV::m_RGBVValueMax = nMax;
	}
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::GetUsed()
{
	return m_RGBVUsed;
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::CheckUsed()
{
	if ( true == m_RGBVRedEnabled || 
		 true == m_RGBVGreenEnabled || 
		 true == m_RGBVBlueEnabled || 
		 true == m_RGBVValueEnabled )
	{	m_RGBVUsed = true;	}
	else
	{	m_RGBVUsed = false; }
	return m_RGBVUsed;
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::ResetColor()
{
	m_RGBVRedMax = 255;                 //紅色上限
	m_RGBVRedMin = 0;                 //紅色下限 
	m_RGBVRedEnabled = false;             //紅色啟用

	m_RGBVGreenMax = 255;               //綠色上限
	m_RGBVGreenMin = 0;               //綠色下限 
	m_RGBVGreenEnabled = false;           //綠色啟用

	m_RGBVBlueMax = 255;                //藍色上限
	m_RGBVBlueMin = 0;                //藍色下限 
	m_RGBVBlueEnabled = false;            //藍色啟用

	m_RGBVValueMax = 255;                //灰色上限
	m_RGBVValueMin = 0;                //灰色下限 
	m_RGBVValueEnabled = false;            //灰色啟用

	m_RGBVUsed = false;                //是否使用
	m_RGBVShowColor = 0xFFFFFF;        //顯示顏色
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::CheckColorInside(int IR, int IG, int IB, int IV) const//確認顏色在範圍內
{
	if ( true == m_RGBVRedEnabled )
	{
		if ( IR>m_RGBVRedMax || IR<m_RGBVRedMin ) { return false; }
	}
	if ( true == m_RGBVGreenEnabled )
	{
		if ( IG>m_RGBVGreenMax || IG<m_RGBVGreenMin ) { return false; }
	}
	if ( true == m_RGBVBlueEnabled )
	{
		if ( IB>m_RGBVBlueMax || IB<m_RGBVBlueMin ) { return false; }
	}
	if ( true == m_RGBVValueEnabled )
	{
		if ( IV>m_RGBVValueMax || IV<m_RGBVValueMin ) { return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::GrayColor()//灰階顏色
{
	const int Mid = 85;
	const int Max = Mid+20;
	const int Min = Mid-20;

	SetRedMax(Max);
	SetRedMin(Min);
	SetRedEnabled(true);

	SetGreenMax(Max);
	SetGreenMin(Min);
	SetGreenEnabled(true);

	SetBlueMax(Max);
	SetBlueMin(Min);
	SetBlueEnabled(true);

	SetUsed(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::CheckMerge()//確認是否可以合併
{
	bool bMerge=true;
	if ( false == CColorRGBV::m_RGBVRedEnabled || 
		 false == CColorRGBV::m_RGBVGreenEnabled || 
		 false == CColorRGBV::m_RGBVBlueEnabled || 
		 false == CColorRGBV::m_RGBVValueEnabled )
	{	bMerge = false;	}
	else
	{	bMerge = true; }
	return bMerge;
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::MergeColor(bool chkMerge, const CColorRGBV *rgbvPtr)//合併顏色	
{
	if ( NULL == rgbvPtr ) { return false; }	
	if ( true == chkMerge )
	{
		if ( GetLogicMode() != rgbvPtr->GetLogicMode() ) { return false; }
		if ( CheckMergeColor(rgbvPtr) == false ) { return false; }
	}

	int nMax1=0, nMin1=0;
	int nMax2=0, nMin2=0;
	if ( true==m_RGBVRedEnabled && true==rgbvPtr->m_RGBVRedEnabled )
	{
		nMin1 = rgbvPtr->m_RGBVRedMin;
		nMax1 = rgbvPtr->m_RGBVRedMax;
		nMin2 = CColorRGBV::m_RGBVRedMin;
		nMax2 = CColorRGBV::m_RGBVRedMax;		
		CColorRGBV::m_RGBVRedMin = MIN(nMin1, nMin2);
		CColorRGBV::m_RGBVRedMax = MAX(nMax1, nMax2);
	}
	if ( true==m_RGBVGreenEnabled && true==rgbvPtr->m_RGBVGreenEnabled )
	{	
		nMin1 = rgbvPtr->m_RGBVGreenMin;
		nMax1 = rgbvPtr->m_RGBVGreenMax;
		nMin2 = CColorRGBV::m_RGBVGreenMin;
		nMax2 = CColorRGBV::m_RGBVGreenMax;		
		CColorRGBV::m_RGBVGreenMin = MIN(nMin1, nMin2);
		CColorRGBV::m_RGBVGreenMax = MAX(nMax1, nMax2);
	}
	if ( true==m_RGBVBlueEnabled && true==rgbvPtr->m_RGBVBlueEnabled )
	{
		nMin1 = rgbvPtr->m_RGBVBlueMin;
		nMax1 = rgbvPtr->m_RGBVBlueMax;
		nMin2 = CColorRGBV::m_RGBVBlueMin;
		nMax2 = CColorRGBV::m_RGBVBlueMax;		
		CColorRGBV::m_RGBVBlueMin = MIN(nMin1, nMin2);
		CColorRGBV::m_RGBVBlueMax = MAX(nMax1, nMax2);
	}
	if ( true==m_RGBVValueEnabled && true==rgbvPtr->m_RGBVValueEnabled )
	{
		nMin1 = rgbvPtr->m_RGBVValueMin;
		nMax1 = rgbvPtr->m_RGBVValueMax;
		nMin2 = CColorRGBV::m_RGBVValueMin;
		nMax2 = CColorRGBV::m_RGBVValueMax;		
		CColorRGBV::m_RGBVValueMin = MIN(nMin1, nMin2);
		CColorRGBV::m_RGBVValueMax = MAX(nMax1, nMax2);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::CheckMergeColor(const CColorRGBV *rgbvPtr)//確認是否為可以合併顏色	
{
	if ( NULL == rgbvPtr ) { return false; }
	if ( GetLogicMode() != rgbvPtr->GetLogicMode() ) { return false; }

	int nMax1=0, nMin1=0;
	int nMax2=0, nMin2=0;

	//Red
	if ( false==m_RGBVRedEnabled || false==rgbvPtr->m_RGBVRedEnabled )
	{	return false; }

	nMin1 = rgbvPtr->m_RGBVRedMin;
	nMax1 = rgbvPtr->m_RGBVRedMax;
	nMin2 = CColorRGBV::m_RGBVRedMin;
	nMax2 = CColorRGBV::m_RGBVRedMax;
	if ( nMin1>nMax2 || nMax1<nMin2 ) 
	{	return false; }
	
	//Green
	if ( false==m_RGBVGreenEnabled || false==rgbvPtr->m_RGBVGreenEnabled )
	{	return false; }

	nMin1 = rgbvPtr->m_RGBVGreenMin;
	nMax1 = rgbvPtr->m_RGBVGreenMax;
	nMin2 = CColorRGBV::m_RGBVGreenMin;
	nMax2 = CColorRGBV::m_RGBVGreenMax;
	if ( nMin1>nMax2 || nMax1<nMin2 ) 
	{	return false; }
	
	//Blue
	if ( false==m_RGBVBlueEnabled || false==rgbvPtr->m_RGBVBlueEnabled )
	{	return false; }
	
	nMin1 = rgbvPtr->m_RGBVBlueMin;
	nMax1 = rgbvPtr->m_RGBVBlueMax;
	nMin2 = CColorRGBV::m_RGBVBlueMin;
	nMax2 = CColorRGBV::m_RGBVBlueMax;		
	if ( nMin1>nMax2 || nMax1<nMin2 ) 
	{	return false; }

	//Value
	if ( false==m_RGBVValueEnabled || false==rgbvPtr->m_RGBVValueEnabled )
	{	return false; }

	nMin1 = rgbvPtr->m_RGBVValueMin;
	nMax1 = rgbvPtr->m_RGBVValueMax;
	nMin2 = CColorRGBV::m_RGBVValueMin;
	nMax2 = CColorRGBV::m_RGBVValueMax;		
	if ( nMin1>nMax2 || nMax1<nMin2 ) 
	{	return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CColorRGBV::CalcShowColor()//計算出顯示的顏色 
{
	unsigned char Red=0, Grn=0, Blu=0;
	int nRed=0, nGrn=0, nBlu=0, nVal=0;	

	if ( true == m_RGBVRedEnabled || 
		 true == m_RGBVGreenEnabled ||
		 true == m_RGBVBlueEnabled || 
		 true == m_RGBVValueEnabled )
	{
		if ( true == m_RGBVRedEnabled || 
			 true == m_RGBVGreenEnabled ||
			 true == m_RGBVBlueEnabled )
		{
			if ( false == m_RGBVRedEnabled )
			{	nRed = 0;	}
			else
			{	nRed = ((m_RGBVRedMax*3)+m_RGBVRedMin)/4;	}

			if ( false == m_RGBVGreenEnabled )
			{	nGrn = 0;	}
			else
			{	nGrn = ((m_RGBVGreenMax*3)+m_RGBVGreenMin)/4;	}

			if ( false == m_RGBVBlueEnabled )
			{	nBlu = 0;	}
			else
			{	nBlu = ((m_RGBVBlueMax*3)+m_RGBVBlueMin)/4;	}
			nVal = 255;
			ImageAPI.RGBVConvertToRGB(nRed, nGrn, nBlu, nVal, Red, Grn, Blu);
		}
		else
		{
			nRed = nGrn = nBlu = 255;			
			nVal = ((m_RGBVValueMax*3)+m_RGBVValueMin)/4;			
			ImageAPI.RGBVConvertToRGB(nRed, nGrn, nBlu, nVal, Red, Grn, Blu);
		}
	}
	else
	{	Red = Grn = Blu = 255; }
	m_RGBVShowColor = RGB(Red, Grn, Blu);
	return true;
}
//-------------------------------------------------------------------------------------//
COLORREF CColorRGBV::GetShowColor() const//取得顯示的顏色
{
	return m_RGBVShowColor;
}
//-------------------------------------------------------------------------------------//
void CColorRGBV::SetShowColor(COLORREF clr)//設定顯示的顏色
{
	m_RGBVShowColor = clr;
}
//-------------------------------------------------------------------------------------//