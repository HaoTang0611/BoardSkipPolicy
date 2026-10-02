// AOIModel.cpp: implementation of the CAOIModel class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIModel.h"
//-------------------------------------------------------------------------------------//
#include "AOIFd.h"
#include "AOIMark.h"
#include "AOIFileIO.h"
#include "AOIProject.h"
#include "AOIBarcode.h"
#include "AOIComponent.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
bool CAOIModel::g_AIServerIsReady = false;
DWORD CAOIModel::g_AIServerTimeout = 10000;
CString CAOIModel::g_AIServerFolderSend=AOI3D_FOLDER("\\AiModel\\AOI2AI");//_T("C:\\JETAOI3D\\AiModel\\AOI2AI");
CString CAOIModel::g_AIServerFolderRecv=AOI3D_FOLDER("\\AiModel\\AI2AOI");//_T("C:\\JETAOI3D\\AiModel\\AI2AOI");
//-------------------------------------------------------------------------------------//
HPEN CAOIModel::hNullPen = NULL;
HPEN CAOIModel::hComPen = NULL;
HPEN CAOIModel::hPadPen = NULL;
HPEN CAOIModel::hPadPen2 = NULL;
HPEN CAOIModel::hLeadPen = NULL;
HPEN CAOIModel::hLeadPen2 = NULL;
HPEN CAOIModel::hShoulderPen = NULL;
HPEN CAOIModel::hShoulderPen2 = NULL;
HPEN CAOIModel::hLeadTipPen = NULL;
HPEN CAOIModel::hLeadTipPen2 = NULL;
HPEN CAOIModel::hSelPenEdit = NULL;
HPEN CAOIModel::hSelPenEditW;
HPEN CAOIModel::hSelPenResult = NULL;
HPEN CAOIModel::hSelPenResultW = NULL;
HPEN CAOIModel::hLandPen = NULL;
HPEN CAOIModel::hLandPen2 = NULL;
HPEN CAOIModel::hWndPen = NULL;
HPEN CAOIModel::hNGPen = NULL;
HPEN CAOIModel::hOKPen = NULL;
HPEN CAOIModel::hBoxPen = NULL;
HPEN CAOIModel::hExtendPen = NULL;
HPEN CAOIModel::hSubBoxPen1 = NULL;
HPEN CAOIModel::hSubBoxPen2 = NULL;
HPEN CAOIModel::hSubBoxPen3 = NULL;
HPEN CAOIModel::hSubBoxPen4 = NULL;
HPEN CAOIModel::hMaskBoxPen1 = NULL;
HPEN CAOIModel::hMaskBoxPen2 = NULL;
HPEN CAOIModel::hIntervalPen = NULL;
HPEN CAOIModel::hNGSubBoxPen1 = NULL;
HPEN CAOIModel::hOKSubBoxPen1 = NULL;
HPEN CAOIModel::hNGSubBoxPen2 = NULL;
HPEN CAOIModel::hOKSubBoxPen2 = NULL;
HBRUSH CAOIModel::hEditBrush = NULL;
HBRUSH CAOIModel::hMaskBoxBrush = NULL;
HBRUSH CAOIModel::hMaskBoxBrushErase = NULL;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIModel, CAOIObj)
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetAIServerIsReady()
{
	return g_AIServerIsReady;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetAIServerIsReady(bool val)
{	
	g_AIServerIsReady = val;
}
//-------------------------------------------------------------------------------------//
DWORD CAOIModel::GetAIServerTimeout()
{
	return g_AIServerTimeout;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetAIServerTimeout(DWORD val)
{
	g_AIServerTimeout = val;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIModel::GetAIServerFolderSend()
{
	return g_AIServerFolderSend;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetAIServerFolderSend(LPCTSTR val)
{
	g_AIServerFolderSend = val;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIModel::GetAIServerFolderRecv()
{
	return g_AIServerFolderRecv;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetAIServerFolderRecv(LPCTSTR val)
{
	g_AIServerFolderRecv = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckAIServerIsReady()
{	
#ifdef AI_MODEL_USE
	CString strFilename;
	CString strFolderSend=GetAIServerFolderSend();
	CString strFolderRecv=GetAIServerFolderRecv();	
	const bool bAiModelDebug=GetAIServerDebugMode();
	if ( true == bAiModelDebug )
	{
		SetAIServerIsReady(true);
		return true;
	}

	SetAIServerIsReady(false);	
	//clear old file
	strFilename.Format(_T("%s\\%s"), strFolderRecv, _T("AIReady.sync"));
	::DeleteFile(strFilename);

	strFilename.Format(_T("%s\\%s"), strFolderSend, _T("IsAIReady.sync"));
	if ( JetAPI::CreateSyncFile(strFilename) == false )
	{	return false;	}

	::Sleep(0);
	size_t i=0;
	const size_t Timeout = GetAIServerTimeout();
	const size_t MaxCount = MAX(100, Timeout/10);
	strFilename.Format(_T("%s\\%s"), strFolderRecv, _T("AIReady.sync"));
	for ( i=0; i<MaxCount; i++ )
	{
		if ( JetAPI::IsFileExist(strFilename) == true )
		{	break; }
		::Sleep(10);		
	}	
	if ( MaxCount == i )
	{	return false; }
	::DeleteFile(strFilename);
	SetAIServerIsReady(true);
#endif//AI_MODEL_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetAIServerDebugMode()
{	//return true;
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::WaitForAIServerSyncFile(LPCTSTR SyncFile)
{	
	size_t i=0;
	const size_t Timeout=GetAIServerTimeout();
	const size_t MaxWaitCount=(100, Timeout/10);	
	for ( i=0; i<MaxWaitCount; i++ )
	{
		if ( GetAIServerIsReady() == false )
		{	return false;	}
		if ( JetAPI::IsFileExist(SyncFile) == true )
		{	break;	}
		::Sleep(10);
	}
	if ( MaxWaitCount == i )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CreateModelPen()
{
	const int BaseSize = 2;
	const int ExtendSize = BaseSize*2;
	int PenSize1 = ((BaseSize/2)*2)+1;
	int PenSize2 = PenSize1+ExtendSize;
	int PenSize3 = PenSize1+(2*ExtendSize);
	COLORREF clr=0x0000000;
	CAOIModel::DestroyModelPen();
	
	clr = 0x000000;
	CAOIModel::hNullPen = ::CreatePen(PS_NULL, 1, clr);

	clr = 0xFF00FF;
	//clr = AOIDataCollect.GetColorComponent();//0xFF00FF	
	CAOIModel::hComPen = ::CreatePen(PS_SOLID, PenSize1, clr);

	clr = 0x80A0FF;
	//clr = AOIDataCollect.GetColorPad();//0x80A0FF	
	CAOIModel::hPadPen = ::CreatePen(PS_SOLID, PenSize1, clr);

	clr = 0x80A0FF;
	//clr = AOIDataCollect.GetColorPad();//0x80A0FF
	CAOIModel::hPadPen2 = ::CreatePen(PS_DASH, 1, clr);

	clr = 0xF0A080;
	//clr = AOIDataCollect.GetColorElectrode();//0xF0A080
	CAOIModel::hLeadPen = ::CreatePen(PS_SOLID, PenSize1, clr);

	clr = 0xF0A080;
	//clr = AOIDataCollect.GetColorElectrode();//0xF0A080
	CAOIModel::hLeadPen2 = ::CreatePen(PS_DASH, 1, clr);

	clr = 0xFF4020;
	//clr = AOIDataCollect.GetColorLeadShoulder();//0xFF4020
	CAOIModel::hShoulderPen = ::CreatePen(PS_SOLID, PenSize1, clr);

	clr = 0xFF4020;
	//clr = AOIDataCollect.GetColorLeadShoulder();//0xFF4020
	CAOIModel::hShoulderPen2 = ::CreatePen(PS_DASH, 1, clr);

	clr = 0xFF8040;
	//clr = AOIDataCollect.GetColorLeadTip();//0xFF8040
	CAOIModel::hLeadTipPen = ::CreatePen(PS_SOLID, PenSize1, clr);

	clr = 0xFF8040;
	//clr = AOIDataCollect.GetColorLeadTip();//0xFF8040
	CAOIModel::hLeadTipPen2 = ::CreatePen(PS_DASH, 1, clr);
	
	clr = 0x00FF00;
	//clr = AOIDataCollect.GetColorLand();//0x00FF00
	CAOIModel::hLandPen = ::CreatePen(PS_SOLID, PenSize1, clr);

	clr = 0x00FF00;
	//clr = AOIDataCollect.GetColorLand();//0x00FF00
	CAOIModel::hLandPen2 = ::CreatePen(PS_DASH, 1, clr);

	clr = 0xFFFF00;
	//clr = AOIDataCollect.GetColorWnd();//0xFFFF00
	CAOIModel::hWndPen = ::CreatePen(PS_SOLID, PenSize1, clr);
	CAOIModel::hExtendPen = ::CreatePen(PS_DASH, 1, clr);

	clr = 0x8F8F2F;
	//clr = AOIDataCollect.GetColorBox();//0x8F8F2F
	CAOIModel::hBoxPen = ::CreatePen(PS_SOLID, PenSize1, clr);
	CAOIModel::hSubBoxPen1 = ::CreatePen(PS_SOLID, PenSize1, clr);
	CAOIModel::hSubBoxPen2 = ::CreatePen(PS_SOLID, PenSize2, clr);
	CAOIModel::hSubBoxPen3 = ::CreatePen(PS_DASH, 1, clr);
	clr = 0x2F2F8F;
	CAOIModel::hSubBoxPen4 = ::CreatePen(PS_SOLID, PenSize1, clr);

	clr = 0xBE9270;
	clr = 0xE7BFC8;	
	CAOIModel::hMaskBoxPen1 = ::CreatePen(PS_SOLID, PenSize1, clr);
	CAOIModel::hMaskBoxPen2 = ::CreatePen(PS_SOLID, PenSize1, 0xB0E4EF);

	//CAOIModel::hSelPen = ::CreatePen(PS_SOLID, PenSize1, 0x00FFFF);	
	CAOIModel::hSelPenEdit = ::CreatePen(PS_SOLID, PenSize1, 0x0000FF);	
	CAOIModel::hSelPenEditW = ::CreatePen(PS_SOLID, PenSize2, 0xFFFFFF);
	CAOIModel::hSelPenResult = ::CreatePen(PS_SOLID, PenSize1, 0x0000FF);	
	CAOIModel::hSelPenResultW = ::CreatePen(PS_SOLID, PenSize2, 0xFFFFFF);		
	CAOIModel::hNGPen = ::CreatePen(PS_SOLID, PenSize1, 0x0000FF);
	CAOIModel::hOKPen = ::CreatePen(PS_SOLID, PenSize1, 0x00FF00);	
	CAOIModel::hIntervalPen = ::CreatePen(PS_DOT, PenSize1, 0x8F8F2F);
	CAOIModel::hNGSubBoxPen1 = ::CreatePen(PS_SOLID, PenSize3, 0x2020AF);
	CAOIModel::hOKSubBoxPen1 = ::CreatePen(PS_SOLID, PenSize3, 0x20AF20);
	CAOIModel::hNGSubBoxPen2 = ::CreatePen(PS_SOLID, PenSize2, 0x2020AF);
	CAOIModel::hOKSubBoxPen2 = ::CreatePen(PS_SOLID, PenSize2, 0x20AF20);

	CAOIModel::hEditBrush = ::CreateSolidBrush(0x0000FF);	
	CAOIModel::hMaskBoxBrush = ::CreateHatchBrush(HS_DIAGCROSS, 0xE7BFC8);
	CAOIModel::hMaskBoxBrushErase = ::CreateHatchBrush(HS_DIAGCROSS, 0xB0E4EF);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DestroyModelPen()
{	
	::DeleteObject(CAOIModel::hNullPen); CAOIModel::hNullPen=NULL;
	::DeleteObject(CAOIModel::hComPen); CAOIModel::hComPen=NULL;
	::DeleteObject(CAOIModel::hPadPen); CAOIModel::hPadPen=NULL;
	::DeleteObject(CAOIModel::hPadPen2); CAOIModel::hPadPen2=NULL;
	::DeleteObject(CAOIModel::hLeadPen); CAOIModel::hLeadPen=NULL;
	::DeleteObject(CAOIModel::hLeadPen2); CAOIModel::hLeadPen2=NULL;
	::DeleteObject(CAOIModel::hShoulderPen); CAOIModel::hShoulderPen=NULL;
	::DeleteObject(CAOIModel::hShoulderPen2); CAOIModel::hShoulderPen2=NULL;
	::DeleteObject(CAOIModel::hLeadTipPen); CAOIModel::hLeadTipPen=NULL;
	::DeleteObject(CAOIModel::hLeadTipPen2); CAOIModel::hLeadTipPen2=NULL;
	::DeleteObject(CAOIModel::hSelPenEdit); CAOIModel::hSelPenEdit=NULL;
	::DeleteObject(CAOIModel::hSelPenEditW); CAOIModel::hSelPenEditW=NULL;	
	::DeleteObject(CAOIModel::hSelPenResult); CAOIModel::hSelPenResult=NULL;	
	::DeleteObject(CAOIModel::hSelPenResultW); CAOIModel::hSelPenResultW=NULL;		
	::DeleteObject(CAOIModel::hLandPen); CAOIModel::hLandPen=NULL;
	::DeleteObject(CAOIModel::hLandPen2); CAOIModel::hLandPen2=NULL;
	::DeleteObject(CAOIModel::hWndPen); CAOIModel::hWndPen=NULL;
	::DeleteObject(CAOIModel::hNGPen); CAOIModel::hNGPen=NULL;
	::DeleteObject(CAOIModel::hOKPen); CAOIModel::hOKPen=NULL;
	::DeleteObject(CAOIModel::hBoxPen); CAOIModel::hBoxPen=NULL;
	::DeleteObject(CAOIModel::hExtendPen); CAOIModel::hExtendPen=NULL;
	::DeleteObject(CAOIModel::hSubBoxPen1); CAOIModel::hSubBoxPen1=NULL;
	::DeleteObject(CAOIModel::hSubBoxPen2); CAOIModel::hSubBoxPen2=NULL;
	::DeleteObject(CAOIModel::hSubBoxPen3); CAOIModel::hSubBoxPen3=NULL;
	::DeleteObject(CAOIModel::hSubBoxPen4); CAOIModel::hSubBoxPen4=NULL;
	::DeleteObject(CAOIModel::hMaskBoxPen1); CAOIModel::hMaskBoxPen1=NULL;
	::DeleteObject(CAOIModel::hMaskBoxPen2); CAOIModel::hMaskBoxPen2=NULL;
	::DeleteObject(CAOIModel::hIntervalPen); CAOIModel::hIntervalPen=NULL;
	::DeleteObject(CAOIModel::hNGSubBoxPen1); CAOIModel::hNGSubBoxPen1=NULL;
	::DeleteObject(CAOIModel::hOKSubBoxPen1); CAOIModel::hOKSubBoxPen1=NULL;
	::DeleteObject(CAOIModel::hNGSubBoxPen2); CAOIModel::hNGSubBoxPen2=NULL;
	::DeleteObject(CAOIModel::hOKSubBoxPen2); CAOIModel::hOKSubBoxPen2=NULL;

	::DeleteObject(CAOIModel::hEditBrush); CAOIModel::hEditBrush=NULL;
	::DeleteObject(CAOIModel::hMaskBoxBrush); CAOIModel::hMaskBoxBrush=NULL;	
	::DeleteObject(CAOIModel::hMaskBoxBrushErase); CAOIModel::hMaskBoxBrushErase=NULL;		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelTypeEnabled(MODEL_TYPE Type)
{
	if ( MODEL_TYPE_NULL == Type )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTypeText(MODEL_TYPE Type, char Text[])
{
	bool IsOK = true;
	switch ( Type )
	{
	case MODEL_TYPE_CHIP:	
		::strcpy(Text, "Chip");
		//::strcpy(Text, "被動元件");		
		break;
	case MODEL_TYPE_CHIP_C:	
		::strcpy(Text, "Chip-C");
		//::strcpy(Text, "電容");			
		break;
	case MODEL_TYPE_CHIP_R: 
		::strcpy(Text, "Chip-R");
		//::strcpy(Text, "電阻");
		break;
	case MODEL_TYPE_CHIP_L: 
		::strcpy(Text, "Chip-L");
		//::strcpy(Text, "電感");		
		break;
	case MODEL_TYPE_CHIP_LED:
		::strcpy(Text, "LED");
		//::strcpy(Text, "二極發光體");
		break;
	case MODEL_TYPE_MELF:   
		::strcpy(Text, "MELF");
		//::strcpy(Text, "MELF");		
		break;
	case MODEL_TYPE_ELECTRODE:	
		::strcpy(Text, "Electrode");
		//::strcpy(Text, "電極元件");		
		break;
	case MODEL_TYPE_TANTALUM_CONDENSER:
		::strcpy(Text, "Tantalum");
		//::strcpy(Text, "鉭質電容");		
		break;
	case MODEL_TYPE_CAPACITY_ARRAY:  
		::strcpy(Text, "CA");
		//::strcpy(Text, "排容");		
		break;	
	case MODEL_TYPE_RESISTOR_ARRAY:
		::strcpy(Text, "RA");
		//::strcpy(Text, "排阻");		
		break;	
	case MODEL_TYPE_TRANSISTOR:  
	case MODEL_TYPE_LEAD_TRANSISTOR:
		::strcpy(Text, "Transistor");
		//::strcpy(Text, "三腳晶體");		
		break;	
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
		::strcpy(Text, "Elec. Cap.");
		//::strcpy(Text, "電解電容");		
		break;	
	case MODEL_TYPE_LED_ARRAY:
		::strcpy(Text, "LED Array");
		//::strcpy(Text, "多排二極發光體");		
		break;		
	case MODEL_TYPE_NO_LEAD_COMPONENT:
		::strcpy(Text, "No Lead");
		//::strcpy(Text, "No Lead");
		break;		
	case MODEL_TYPE_NO_LEAD_DFN:
		::strcpy(Text, "DFN");
		//::strcpy(Text, "DFN");		
		break;
	case MODEL_TYPE_NO_LEAD_QFN:
		::strcpy(Text, "QFN");
		//::strcpy(Text, "QFN");		
		break;	
	case MODEL_TYPE_NO_LEAD_OSC:
		::strcpy(Text, "OSC");
		//::strcpy(Text, "振盪器");		
		break;			
	case MODEL_TYPE_LEAD_COMPONENT:
		::strcpy(Text, "Lead Component");
		//::strcpy(Text, "Lead元件");		
		break;
	case MODEL_TYPE_LEAD_SOP:
		::strcpy(Text, "SOP");
		//::strcpy(Text, "SOP");		
		break;
	case MODEL_TYPE_LEAD_QFP:
		::strcpy(Text, "QFP");
		//::strcpy(Text, "QFP");		
		break;
	case MODEL_TYPE_JLEAD_COMPONENT:
		::strcpy(Text, "J-Lead Component");
		//::strcpy(Text, "Lead元件");		
		break;
	case MODEL_TYPE_JLEAD_SOJ:
		::strcpy(Text, "SOJ");
		//::strcpy(Text, "SOJ");		
		break;
	case MODEL_TYPE_JLEAD_PLCC:
		::strcpy(Text, "PLCC");
		//::strcpy(Text, "PLCC");		
		break;
	case MODEL_TYPE_COMPOSITE_COMPONENT:
		::strcpy(Text, "Composite Component");
		//::strcpy(Text, "複合元件");		
		break;
	case MODEL_TYPE_POWER_TRANSISTOR:
		::strcpy(Text, "Power Transistor");
		//::strcpy(Text, "功率電晶體");		
		break;
	case MODEL_TYPE_CONNECTOR:
		::strcpy(Text, "Connector");
		//::strcpy(Text, "連接器");		
		break;
	
	case MODEL_TYPE_BGA:
		::strcpy(Text, "BGA");
		//::strcpy(Text, "BGA");
		break;
	case MODEL_TYPE_FD:
		::strcpy(Text, "Fd");
		break;
	case MODEL_TYPE_BARCODE:
		::strcpy(Text, "Barcode");
		break;

	case MODEL_TYPE_PAD_COMPONENT:
		::strcpy(Text, "Pad");
		//::strcpy(Text, "焊盤元件");		
		break;
	case MODEL_TYPE_GOLD_FINGER:
		::strcpy(Text, "Gold Finger");
		//::strcpy(Text, "金手指");		
		break;

	case MODEL_TYPE_DIP_LEAD:
		::strcpy(Text, "DIP Lead");
		//::strcpy(Text, "DIP Lead");		
		break;
	case MODEL_TYPE_OTHERS:
		::strcpy(Text, "Others");
		break;	
	case MODEL_TYPE_NULL:
		::strcpy(Text, "Undefined");
		//::strcpy(Text, "Undefined");
		break;
	default://MODEL_TYPE_NULL		
		IsOK = false;
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTypeText(MODEL_TYPE Type, wchar_t Text[])
{
	bool IsOK = true;
	switch ( Type )
	{
	case MODEL_TYPE_CHIP:	
		::wcscpy(Text, L"Chip");
		//::wcscpy(Text, L"被動元件");		
		break;
	case MODEL_TYPE_CHIP_C:	
		::wcscpy(Text, L"Chip-C");
		//::wcscpy(Text, L"電容");			
		break;
	case MODEL_TYPE_CHIP_R: 
		::wcscpy(Text, L"Chip-R");
		//::wcscpy(Text, L"電阻");
		break;
	case MODEL_TYPE_CHIP_L: 
		::wcscpy(Text, L"Chip-L");
		//::wcscpy(Text, L"電感");		
		break;
	case MODEL_TYPE_CHIP_LED:
		::wcscpy(Text, L"LED");
		//::wcscpy(Text, L"二極發光體");
		break;
	case MODEL_TYPE_MELF:   
		::wcscpy(Text, L"MELF");
		//::wcscpy(Text, L"MELF");		
		break;
	case MODEL_TYPE_ELECTRODE:	
		::wcscpy(Text, L"Electrode");
		//::wcscpy(Text, L"電極元件");		
		break;
	case MODEL_TYPE_TANTALUM_CONDENSER:
		::wcscpy(Text, L"Tantalum");
		//::wcscpy(Text, L"鉭質電容");		
		break;
	case MODEL_TYPE_CAPACITY_ARRAY:  
		::wcscpy(Text, L"CA");
		//::wcscpy(Text, L"排容");		
		break;	
	case MODEL_TYPE_RESISTOR_ARRAY:
		::wcscpy(Text, L"RA");
		//::wcscpy(Text, L"排阻");		
		break;	
	case MODEL_TYPE_TRANSISTOR:  
	case MODEL_TYPE_LEAD_TRANSISTOR:
		::wcscpy(Text, L"Transistor");
		//::wcscpy(Text, L"三腳晶體");		
		break;	
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
		::wcscpy(Text, L"Elec. Cap.");
		//::wcscpy(Text, L"電解電容");		
		break;	
	case MODEL_TYPE_LED_ARRAY:
		::wcscpy(Text, L"LED Array");
		//::wcscpy(Text, L"多排二極發光體");		
		break;		
	case MODEL_TYPE_NO_LEAD_COMPONENT:
		::wcscpy(Text, L"No Lead");
		//::wcscpy(Text, L"No Lead");
		break;		
	case MODEL_TYPE_NO_LEAD_DFN:
		::wcscpy(Text, L"DFN");
		//::wcscpy(Text, L"DFN");		
		break;
	case MODEL_TYPE_NO_LEAD_QFN:
		::wcscpy(Text, L"QFN");
		//::wcscpy(Text, L"QFN");		
		break;	
	case MODEL_TYPE_NO_LEAD_OSC:
		::wcscpy(Text, L"OSC");
		//::wcscpy(Text, L"振盪器");		
		break;			
	case MODEL_TYPE_LEAD_COMPONENT:
		::wcscpy(Text, L"Lead Component");
		//::wcscpy(Text, L"Lead元件");		
		break;
	case MODEL_TYPE_LEAD_SOP:
		::wcscpy(Text, L"SOP");
		//::wcscpy(Text, L"SOP");		
		break;
	case MODEL_TYPE_LEAD_QFP:
		::wcscpy(Text, L"QFP");
		//::wcscpy(Text, L"QFP");		
		break;
	case MODEL_TYPE_JLEAD_COMPONENT:
		::wcscpy(Text, L"J-Lead Component");
		//::wcscpy(Text, L"J-Lead元件");		
		break;
	case MODEL_TYPE_JLEAD_SOJ:
		::wcscpy(Text, L"SOJ");
		//::wcscpy(Text, L"SOJ");		
		break;
	case MODEL_TYPE_JLEAD_PLCC:
		::wcscpy(Text, L"PLCC");
		//::wcscpy(Text, L"PLCC");		
		break;
	case MODEL_TYPE_COMPOSITE_COMPONENT:
		::wcscpy(Text, L"Composite Component");
		//::wcscpy(Text, L"複合元件");		
		break;
	case MODEL_TYPE_POWER_TRANSISTOR:
		::wcscpy(Text, L"Power Transistor");
		//::wcscpy(Text, L"功率電晶體");		
		break;
	case MODEL_TYPE_CONNECTOR:
		::wcscpy(Text, L"Connector");
		//::wcscpy(Text, L"連接器");		
		break;	
	case MODEL_TYPE_BGA:
		::wcscpy(Text, L"BGA");
		//::wcscpy(Text, L"BGA");
		break;
	case MODEL_TYPE_FD:
		::wcscpy(Text, L"Fd");
		break;
	case MODEL_TYPE_BARCODE:
		::wcscpy(Text, L"Barcode");
		break;

	case MODEL_TYPE_PAD_COMPONENT:
		::wcscpy(Text, L"Pad");
		//::wcscpy(Text, L"焊盤元件");		
		break;
	case MODEL_TYPE_GOLD_FINGER:
		::wcscpy(Text, L"Gold Finger");
		//::wcscpy(Text, L"金手指");		
		break;

	case MODEL_TYPE_DIP_LEAD:
		::wcscpy(Text, L"DIP Lead");
		//::wcscpy(Text, L"DIP Lead");		
		break;
	case MODEL_TYPE_OTHERS:
		::wcscpy(Text, L"Others");
		break;	
	case MODEL_TYPE_NULL:
		::wcscpy(Text, L"Undefined");
		//::wcscpy(Text, L"Undefined");
		break;
	default://MODEL_TYPE_NULL		
		IsOK = false;
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTypeList(std::vector<MODEL_TYPE> &List)//取得模組樣式表
{
	List.clear();

	//Chipe Type
	List.push_back(MODEL_TYPE_CHIP_C);
	List.push_back(MODEL_TYPE_CHIP_R);
	List.push_back(MODEL_TYPE_CHIP_L);
	List.push_back(MODEL_TYPE_CHIP_LED);
	List.push_back(MODEL_TYPE_MELF);

	//Electrode Type
	List.push_back(MODEL_TYPE_TANTALUM_CONDENSER);
	List.push_back(MODEL_TYPE_CAPACITY_ARRAY);
	List.push_back(MODEL_TYPE_RESISTOR_ARRAY);	
	List.push_back(AOIDataCollect.GetModelDefaultTransistorType());	
	List.push_back(MODEL_TYPE_ELECTROLYTIC_CAPACITOR);
	List.push_back(MODEL_TYPE_LED_ARRAY);

	//No Lead Type
	List.push_back(MODEL_TYPE_NO_LEAD_COMPONENT);
	//List.push_back(MODEL_TYPE_NO_LEAD_DFN);
	//List.push_back(MODEL_TYPE_NO_LEAD_QFN);
	List.push_back(MODEL_TYPE_NO_LEAD_OSC);	

	//IC Lead Type
	List.push_back(MODEL_TYPE_LEAD_COMPONENT);
	//List.push_back(MODEL_TYPE_LEAD_SOP);
	//List.push_back(MODEL_TYPE_LEAD_QFP);
	
	//IC J-Lead Type	
	//List.push_back(MODEL_TYPE_JLEAD_COMPONENT);
	//List.push_back(MODEL_TYPE_JLEAD_SOJ);
	List.push_back(MODEL_TYPE_JLEAD_PLCC);

	//More thand One Lead Type	
	//List.push_back(MODEL_TYPE_COMPOSITE_COMPONENT);
	List.push_back(MODEL_TYPE_POWER_TRANSISTOR);
	List.push_back(MODEL_TYPE_CONNECTOR);

	//NO Pad Type
	List.push_back(MODEL_TYPE_BGA);
	//List.push_back(MODEL_TYPE_FD);
	//List.push_back(MODEL_TYPE_BARCODE);

	//No Lead Type
	List.push_back(MODEL_TYPE_GOLD_FINGER);

	//DIP Lead Type
	List.push_back(MODEL_TYPE_DIP_LEAD);

	//Others
	List.push_back(MODEL_TYPE_OTHERS);

	//No Defined
	List.push_back(MODEL_TYPE_NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIModel::GetModelDefaultFilename(LPCTSTR GroupName)//取得預設模組名稱
{
	CString filename;	
	CString    DefaultModelFolder=AOIDataCollect.GetAOIDefaultModelDirectory();	
	filename.Format(_T("%s\\%s.MDL"), DefaultModelFolder, GroupName);
	return filename;
}
//-------------------------------------------------------------------------------------//
CString CAOIModel::GetModelDefaultImageFilename(LPCTSTR GroupName)//取得預設模組影像名稱
{
	CString filename;	
	CString DefaultModelFolder=AOIDataCollect.GetAOIDefaultModelDirectory();	
	filename.Format(_T("%s\\%s.PNG"), DefaultModelFolder, GroupName);
	return filename;
}
//-------------------------------------------------------------------------------------//
CString CAOIModel::GetModelChipSizeModeText(CHIP_SIZE_MODE Mode)//取得模組被動元件尺寸等級文字
{
	CString str;
	switch ( Mode )
	{
	case CHIP_SIZE_NONE:    str=_T("None");	break;	
	case CHIP_SIZE_008_004:	str=_T("008004");	break;
	case CHIP_SIZE_030_015_METRIC: str=_T("03015-Metric"); break;
	case CHIP_SIZE_010_005:	str=_T("01005");	break;
	case CHIP_SIZE_020_010:	str=_T("0201");	break;
	case CHIP_SIZE_040_020:	str=_T("0402");	break;
	case CHIP_SIZE_060_030:	str=_T("0603");	break;
	case CHIP_SIZE_080_050:	str=_T("0805");	break;
	case CHIP_SIZE_120_060:	str=_T("1206");	break;
	case CHIP_SIZE_120_100:	str=_T("1210");	break;
	case CHIP_SIZE_180_120: str=_T("1812"); break;
	case CHIP_SIZE_200_100:	str=_T("2010");	break;
	case CHIP_SIZE_250_120:	str=_T("2512");	break;
	case CHIP_SIZE_OTHERS:	str=_T("Others");	break;
	default:
		str = _T("Undefined");
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelTypeUseBody(MODEL_TYPE ModelType)//確認模組樣式使用零件本體
{
	if ( MODEL_TYPE_GOLD_FINGER == ModelType ) { return false; }
	if ( MODEL_TYPE_DIP_LEAD == ModelType ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelTypUseChipSizeMode(MODEL_TYPE ModelType)//確認模組樣式適合被動元件尺寸等級
{
	//if ( MODEL_TYPE_CHIP == ModelType ) { return true; }
	if ( MODEL_TYPE_CHIP_C == ModelType ) { return true; }
	if ( MODEL_TYPE_CHIP_R == ModelType ) { return true; }
	if ( MODEL_TYPE_CHIP_L == ModelType ) { return true; }
	//if ( MODEL_TYPE_CHIP_LED == ModelType ) { return true; }
	//if ( MODEL_TYPE_MELF == ModelType ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelTypeUseTwoElectrode(MODEL_TYPE ModelType)//確認模組樣式有2個電極端
{
	bool bUseTwoElectrode=false;
	switch ( ModelType )
	{
	case MODEL_TYPE_CHIP:
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_R:
	case MODEL_TYPE_CHIP_L:
	case MODEL_TYPE_CHIP_LED:
	case MODEL_TYPE_MELF:
		bUseTwoElectrode = true;
		break;
	case MODEL_TYPE_TANTALUM_CONDENSER:	
		bUseTwoElectrode = true;
		break;
	}
	return bUseTwoElectrode;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelChipSize(CHIP_SIZE_MODE Mode, double &Width, double &Length, double &Height)//取得模組被動元件尺寸
{
	bool IsOK=true;
	switch ( Mode )
	{	
	case CHIP_SIZE_008_004:
		Width  =  250;//um
		Length =  125;//um
		Height =  120;
		break;
	case CHIP_SIZE_030_015_METRIC:
		Width  =  300;//um
		Length =  150;//um
		Height =  100;
		break;
	case CHIP_SIZE_010_005:
		Width  =  400;//um
		Length =  200;//um
		Height =  180;
		break;
	case CHIP_SIZE_020_010:
		Width  =  600;//um
		Length =  300;//um
		Height =  230;
		break;
	case CHIP_SIZE_040_020:
		Width  = 1000;//um
		Length =  500;//um
		Height =  300;
		break;
	case CHIP_SIZE_060_030:
		Width  = 1600;//um
		Length =  800;//um
		Height =  400;
		break;
	case CHIP_SIZE_080_050:
		Width  = 2000;//um
		Length = 1250;//um
		Height =  500;
		break;
	case CHIP_SIZE_120_060:
		Width  = 3200;//um
		Length = 1600;//um
		Height =  550;
		break;
	case CHIP_SIZE_120_100:
		Width  = 3200;//um
		Length = 2500;//um
		Height =  550;
		break;
	case CHIP_SIZE_180_120:
		Width  = 4500;//um
		Length = 3200;//um
		Height =  550;
		break;
	case CHIP_SIZE_200_100:
		Width  = 5000;//um
		Length = 2500;//um
		Height =  550;
		break;
	case CHIP_SIZE_250_120:
		Width  = 6400;//um
		Length = 3200;//um
		Height =  550;
		break;	
	default:
	case CHIP_SIZE_NONE:
	case CHIP_SIZE_OTHERS:
		Width  = 1000;//um
		Length = 1000;//um
		Height = 600;
		IsOK = false;
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelChipSizeGap(CHIP_SIZE_MODE Mode, double &WidthGap, double &LengthGap, double &HeightGap)//取得模組被動元件尺寸公差
{
	bool IsOK=true;
	switch ( Mode )
	{	
	case CHIP_SIZE_008_004:
		WidthGap  =   60;//250um
		LengthGap =   30;//125um
		HeightGap =   30;
		break;
	case CHIP_SIZE_030_015_METRIC:
		WidthGap  =   50;//400um
		LengthGap =   40;//200um
		HeightGap =   30;
		break;
	case CHIP_SIZE_010_005:
		WidthGap  =  100;//400um
		LengthGap =   80;//200um
		HeightGap =   30;
		break;
	case CHIP_SIZE_020_010:
		WidthGap  =  150;//600um
		LengthGap =  100;//300um
		HeightGap =   50;
		break;
	case CHIP_SIZE_040_020:
		WidthGap  =  300;//1000um
		LengthGap =  200;// 500um
		HeightGap = 75;
		break;
	case CHIP_SIZE_060_030:
		WidthGap  =  300;//1600um
		LengthGap =  300;// 800um
		HeightGap = 100;
		break;
	case CHIP_SIZE_080_050:
		WidthGap  =  300;//2000um
		LengthGap =  300;//1250um
		HeightGap = 100;
		break;
	case CHIP_SIZE_120_060:
		WidthGap  =  500;//3200um
		LengthGap =  300;//1600um
		HeightGap = 100;
		break;
	case CHIP_SIZE_120_100:
		WidthGap  =  500;//3200um
		LengthGap =  400;//2500um
		HeightGap = 100;
		break;
	case CHIP_SIZE_180_120:
		WidthGap  =  500;//3200um
		LengthGap =  400;//2500um
		HeightGap = 100;
		break;
	case CHIP_SIZE_200_100:
		WidthGap  =  600;//5000um
		LengthGap =  400;//2500um
		HeightGap = 100;
		break;
	case CHIP_SIZE_250_120:
		WidthGap  =  600;//6400um
		LengthGap =  500;//3200um
		HeightGap = 100;
		break;	
	default:
	case CHIP_SIZE_NONE:
	case CHIP_SIZE_OTHERS:
		WidthGap  = 200;//um
		LengthGap = 200;//um
		HeightGap = 100;
		IsOK = false;
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CHIP_SIZE_MODE  CAOIModel::FindModelChipSizeMode(MODEL_TYPE ModelType, const TREGION4D &BodyRgn)//尋找模組被動元件尺寸模式
{
	const bool bUseChipSizeLevel = CheckModelTypUseChipSizeMode(ModelType);
	if ( false == bUseChipSizeLevel ) 
	{	return CHIP_SIZE_OTHERS;	}
	
	int    i=0, j=0;
	int    level=0;	
	double MinW=0, MaxW=0;	
	double MinH=0, MaxH=0;	
	double ErrorW=0, ErrorH=0;
	double Error=0, MinError=0;
	double MinErrorW=0, MinErrorH=0;
	CHIP_SIZE_MODE ChipSizeMode;	
	CHIP_SIZE_MODE ChipSizeModeAct;	
	double ChipW=0, ChipH=0, ChipT=0, ChipL=0;				
	const double width = BodyRgn.GetWidth();
	const double height = BodyRgn.GetHeight();	
	const double BodyW = MAX(width, height);
	const double BodyH = MIN(width, height);
	const double diagonal = sqrt((width*width)+(height*height));

	MinError = DBL_MAX;
	ChipSizeModeAct = CHIP_SIZE_OTHERS;
	for ( i=CHIP_SIZE_OTHERS-1; i!=CHIP_SIZE_NONE; i-- )
	{
		ChipSizeMode = (CHIP_SIZE_MODE)(i);		
		CAOIModel::GetModelChipSize(ChipSizeMode, ChipW, ChipH, ChipT);		
		ErrorW = fabs(BodyW-ChipW);
		ErrorH = fabs(BodyH-ChipH);
		Error = ErrorW+ErrorH;
		if ( Error > MinError )
		{	continue;	}

		MinError = Error;
		MinErrorW = ErrorW;
		MinErrorH = ErrorH;
		ChipSizeModeAct = ChipSizeMode;		
	}
	if ( MinErrorW > 250 || MinErrorH > 250 )
	{	ChipSizeModeAct = CHIP_SIZE_OTHERS; }
	return ChipSizeModeAct;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelSimilarTypeList(MODEL_TYPE Type, std::vector<MODEL_TYPE> &TypeList)//取得相同模組樣式的樣式列表
{	
	TypeList.clear();
	switch ( Type )
	{
	//case MODEL_TYPE_CHIP://被動元件-總類
	case MODEL_TYPE_CHIP_C://被動元件-電容
	case MODEL_TYPE_CHIP_R://被動元件-電阻
	case MODEL_TYPE_CHIP_L://被動元件-電感	
	case MODEL_TYPE_CHIP_LED://LED雙腳
	case MODEL_TYPE_MELF://被動元件				

	//case MODEL_TYPE_ELECTRODE://電極元件
	case MODEL_TYPE_TANTALUM_CONDENSER://鉭質電容
	case MODEL_TYPE_CAPACITY_ARRAY://排容
	case MODEL_TYPE_RESISTOR_ARRAY://排阻
	case MODEL_TYPE_TRANSISTOR://三腳晶體
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR://電解電容
	case MODEL_TYPE_LED_ARRAY://LED多腳	
		//TypeList.push_back(MODEL_TYPE_CHIP);
		TypeList.push_back(MODEL_TYPE_CHIP_C);
		TypeList.push_back(MODEL_TYPE_CHIP_R);
		TypeList.push_back(MODEL_TYPE_CHIP_L);
		TypeList.push_back(MODEL_TYPE_CHIP_LED);
		TypeList.push_back(MODEL_TYPE_MELF);
		
		//TypeList.push_back(MODEL_TYPE_ELECTRODE);
		TypeList.push_back(MODEL_TYPE_TANTALUM_CONDENSER);
		TypeList.push_back(MODEL_TYPE_CAPACITY_ARRAY);
		TypeList.push_back(MODEL_TYPE_RESISTOR_ARRAY);
		TypeList.push_back(MODEL_TYPE_TRANSISTOR);
		TypeList.push_back(MODEL_TYPE_ELECTROLYTIC_CAPACITOR);
		TypeList.push_back(MODEL_TYPE_LED_ARRAY);
		break;

	case MODEL_TYPE_NO_LEAD_COMPONENT://無腳元件
	case MODEL_TYPE_NO_LEAD_DFN://無腳元件-DFN
	case MODEL_TYPE_NO_LEAD_QFN://無腳元件-QFN
	case MODEL_TYPE_NO_LEAD_OSC://無腳元件-振盪器
		TypeList.push_back(Type);
		break;

	case MODEL_TYPE_LEAD_COMPONENT://Lead元件
	case MODEL_TYPE_LEAD_SOP://Lead元件-SOP
	case MODEL_TYPE_LEAD_QFP://Lead元件-QFP	
	case MODEL_TYPE_LEAD_TRANSISTOR://Lead-三腳晶體
		TypeList.push_back(Type);
		break;

	case MODEL_TYPE_JLEAD_COMPONENT://J-Lead元件
	case MODEL_TYPE_JLEAD_SOJ://J-Lead元件-SOJ
	case MODEL_TYPE_JLEAD_PLCC://J-Lead元件-QFJ	
		TypeList.push_back(Type);
		break;

	case MODEL_TYPE_COMPOSITE_COMPONENT://複合元件
	case MODEL_TYPE_POWER_TRANSISTOR://複合元件-功率電晶體	
	case MODEL_TYPE_CONNECTOR://複合元件-連接器	
		TypeList.push_back(Type);
		break;

	case MODEL_TYPE_BGA://BGA元件
		TypeList.push_back(Type);
		break;

	case MODEL_TYPE_FD://Fd元件
	case MODEL_TYPE_BARCODE://Barcode元件
	case MODEL_TYPE_PAD_COMPONENT://焊盤元件	
	case MODEL_TYPE_GOLD_FINGER://焊盤元件-金手指	
	case MODEL_TYPE_DIP_LEAD://標準DIP引腳
	case MODEL_TYPE_OTHERS://其餘的
		TypeList.push_back(Type);
		break;
	case MODEL_TYPE_NULL:
	default:
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelWndRoiCanBeDeleted(ALG_TYPE Type)//確認該演算法的子框能不能被刪除
{
	bool bDeleted=true;
	switch ( Type )
	{	
	case ALG_BRIGHT_RATIO:	bDeleted = false;	break;
	case ALG_ANGLE_MEASURE:	bDeleted = false;	break;
	case ALG_MEASURE_CONNECTOR: bDeleted = false; break;
	}
	return bDeleted;
}
//-------------------------------------------------------------------------------------//
int CAOIModel::ObtainModelAlgDefaultWndRoiCount(ALG_TYPE Type)//取得模組演算法預設子框數量
{
	int WndRoiCount = 0;
	switch ( Type )
	{
	case ALG_COLOR_CODE:	WndRoiCount = 5;	break;
	case ALG_CHAR_VERIFY:	WndRoiCount = 3;	break;
	case ALG_ANGLE_MEASURE:	WndRoiCount = 2;	break;
	case ALG_PIXEL_COMPARE: WndRoiCount = 3;    break;
	case ALG_MEASURE_SIP_DISTANCE:	WndRoiCount = 5;	break;
	case ALG_MEASURE_CONNECTOR:	WndRoiCount = 8;	break;
	}
	return WndRoiCount;
}
//-------------------------------------------------------------------------------------//
WND_SYNC_MOVE_MODE CAOIModel::ObtainModelDefaultWndSyncMoveMode(MODEL_TYPE ModelType)//取得模組預設檢測框同步移動模式
{
	WND_SYNC_MOVE_MODE SyncMoveMode=WND_SYNC_MOVE_ROTATE;
	switch ( ModelType )
	{
	case MODEL_TYPE_NO_LEAD_OSC:
		SyncMoveMode=WND_SYNC_MOVE_SYMMETRY;
		break;
	}
	return SyncMoveMode;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelWndRoiAutoAdd(ALG_TYPE Type, CAOILand *LandPtr)//確認該演算法的子框能不能自動建立
{
	bool bFit=false;
	switch ( Type )
	{	
	case ALG_CHAR_VERIFY:	bFit = true;	break;
	case ALG_PIXEL_COMPARE: bFit = true;	break;
	}
	if ( false == bFit ) { return false; }
	if ( NULL != LandPtr ) { return false; }
	//if ( NULL == WndPtr ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
WND_RGN_LINK_MODE CAOIModel::ObtainModelDefaultWndRegionLinkMode(MODEL_TYPE ModelType, WND_DEFECT_ID WndDefectID)
{
	WND_RGN_LINK_MODE WndRgnLinkMode=WND_RGN_LINK_NONE;	
	switch ( ModelType )
	{	
	case MODEL_TYPE_CHIP:		
	case MODEL_TYPE_CHIP_C:				
	case MODEL_TYPE_CHIP_R:		
	case MODEL_TYPE_CHIP_L:		
	case MODEL_TYPE_CHIP_LED:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;//WND_RGN_LINK_BODY;			
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}		
		break;
	case MODEL_TYPE_MELF:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;		
		break;
	case MODEL_TYPE_ELECTRODE:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;		
		break;
	case MODEL_TYPE_TANTALUM_CONDENSER:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;		
		break;
	case MODEL_TYPE_CAPACITY_ARRAY:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:			
			WndRgnLinkMode = WND_RGN_LINK_BODY;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;
	case MODEL_TYPE_RESISTOR_ARRAY:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:			
			WndRgnLinkMode = WND_RGN_LINK_LEAD;//WND_RGN_LINK_BODY;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;
	case MODEL_TYPE_TRANSISTOR:	
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:			
			WndRgnLinkMode = WND_RGN_LINK_LEAD;//WND_RGN_LINK_BODY;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;
	case MODEL_TYPE_NO_LEAD_COMPONENT:
	case MODEL_TYPE_NO_LEAD_DFN:
	case MODEL_TYPE_NO_LEAD_QFN:
	case MODEL_TYPE_NO_LEAD_OSC:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_BODY;
			break;
		}
		break;
	case MODEL_TYPE_LEAD_COMPONENT:
	case MODEL_TYPE_LEAD_SOP:
	case MODEL_TYPE_LEAD_QFP:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_LEAD_TIP_SHOULDER;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD_TIP_SHOULDER;//WND_RGN_LINK_LEAD_TIP;
			break;
		}
		break;
	case MODEL_TYPE_LEAD_TRANSISTOR:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PART_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_LEAD_SHOULDER;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD_SHOULDER;
			break;
		}
		break;
	case MODEL_TYPE_JLEAD_COMPONENT:
	case MODEL_TYPE_JLEAD_SOJ:
	case MODEL_TYPE_JLEAD_PLCC:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_BODY;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;
	case MODEL_TYPE_COMPOSITE_COMPONENT:
	case MODEL_TYPE_POWER_TRANSISTOR:	
	case MODEL_TYPE_CONNECTOR:	
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:
			//WndRgnLinkMode = WND_RGN_LINK_BODY;
			WndRgnLinkMode = WND_RGN_LINK_LEAD_SHOULDER;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD_TIP_SHOULDER;//WND_RGN_LINK_LEAD_TIP;
			break;
		}
		break;

	case MODEL_TYPE_PAD_COMPONENT://焊盤元件	
	case MODEL_TYPE_GOLD_FINGER://焊盤元件-金手指	
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:			
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PART_ALIGN:			
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_BODY;
			break;
		}
		break;
	case MODEL_TYPE_DIP_LEAD://插件引腳
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PART_ALIGN:			
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;
	default:
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_PAD;
			break;
		case WND_DEFECT_PAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_PAD_TIP;
			break;
		case WND_DEFECT_PART_ALIGN:
			WndRgnLinkMode = WND_RGN_LINK_BODY;
			break;
		case WND_DEFECT_LEAD_ADJUST:
			WndRgnLinkMode = WND_RGN_LINK_LEAD;
			break;
		}
		break;
	}
	return WndRgnLinkMode;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelAlgWndRoiSelfFrameEnabled(ALG_TYPE Type)
{
	bool bEnabled = false;
	switch (Type)
	{
	case(ALG_MEASURE_SIP_DISTANCE):
	case(ALG_MEASURE_CONNECTOR):
		bEnabled = true;
		break;
	default:
		bEnabled = false;
		break;
	}
	return bEnabled;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelBoxRegionRect(const TREGION4D &BoxRgn, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, RECT &RoiRect)//計算檢測框檢測區域
{	
	const int nImageH=(int)(ImageH);
	double dL = (BoxRgn.minX-RgnCp.x)*Scale.x+ImageCp.x;
	double dT = (BoxRgn.minY-RgnCp.y)*Scale.y+ImageCp.y;
	double dR = (BoxRgn.maxX-RgnCp.x)*Scale.x+ImageCp.x;
	double dB = (BoxRgn.maxY-RgnCp.y)*Scale.y+ImageCp.y;
	//const int nL = JetAPI::Floor(dL);
	//const int nT = JetAPI::Floor(dT);
	//const int nR = JetAPI::Floor(dR);
	//const int nB = JetAPI::Floor(dB);
	const int nL = (int)(dL+0.5);
	const int nT = (int)(dT+0.5);
	const int nR = (int)(dR+0.5);
	const int nB = (int)(dB+0.5);

	RoiRect.left   = nL;
	RoiRect.top    = nT;
	RoiRect.right  = nR;
	RoiRect.bottom = nB;

	RoiRect.top    = nImageH-nB;
	RoiRect.bottom = nImageH-nT;
	if ( ImageAPI.CheckRoiRect(ImageW, ImageH, RoiRect) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelBoxCornerPts(const TPOINT2D CornerPts[], IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, TPOINT2D ResultPts[])//計算檢測框檢測區域
{
	ResultPts[0].x = (CornerPts[0].x-RgnCp.x)*Scale.x+ImageCp.x;
	ResultPts[0].y = (CornerPts[0].y-RgnCp.y)*Scale.y+ImageCp.y;
	ResultPts[1].x = (CornerPts[1].x-RgnCp.x)*Scale.x+ImageCp.x;
	ResultPts[1].y = (CornerPts[1].y-RgnCp.y)*Scale.y+ImageCp.y;
	ResultPts[2].x = (CornerPts[2].x-RgnCp.x)*Scale.x+ImageCp.x;
	ResultPts[2].y = (CornerPts[2].y-RgnCp.y)*Scale.y+ImageCp.y;
	ResultPts[3].x = (CornerPts[3].x-RgnCp.x)*Scale.x+ImageCp.x;
	ResultPts[3].y = (CornerPts[3].y-RgnCp.y)*Scale.y+ImageCp.y;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelBoxPtPoint(const TPOINT2D &Pt, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, TPOINT2D &ResultPt)//計算檢測框檢測位置
{
	const int nImageH=(int)(ImageH);
	double x = Pt.x;
	double y = nImageH-Pt.y;
	ResultPt.x = ((x-ImageCp.x)/Scale.x)+RgnCp.x;
	ResultPt.y = ((y-ImageCp.y)/Scale.y)+RgnCp.y;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelBoxRectRegion(const RECT &BoxRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &Scale, const TPOINT2D &ImageCp, TREGION4D &RoiRegion)//計算檢測框模組區域
{
	const int nImageH=(int)(ImageH);
	double dL = BoxRect.left;
	double dT = nImageH-BoxRect.top;
	double dR = BoxRect.right;
	double dB = nImageH-BoxRect.bottom;

	dL = ((dL-ImageCp.x)/Scale.x)+RgnCp.x;
	dT = ((dT-ImageCp.y)/Scale.y)+RgnCp.y;
	dR = ((dR-ImageCp.x)/Scale.x)+RgnCp.x;
	dB = ((dB-ImageCp.y)/Scale.y)+RgnCp.y;	

	RoiRegion.minX = MIN(dL, dR);
	RoiRegion.minY = MIN(dT, dB);
	RoiRegion.maxX = MAX(dL, dR);
	RoiRegion.maxY = MAX(dT, dB);
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIModel::CAOIModel():CAOIObj(AOI_OBJ_MODEL)
{
	PreInitModel();
	InitialModel();
}
//-------------------------------------------------------------------------------------//
CAOIModel::CAOIModel(const CAOIModel &Model):CAOIObj(Model)
{
	PreInitModel();
	CloneModel(Model);
}
//-------------------------------------------------------------------------------------//
CAOIModel::~CAOIModel()
{	
	ClearModelAllObjList();	
	RemoveModelImageFolder();
	ReleaseModelUniFrameList();
}
//-------------------------------------------------------------------------------------//
CAOIModel& CAOIModel::operator=(const CAOIModel &Model)
{
	if ( &Model == this ) { return *this; }
	CAOIObj::operator=(Model);
	CloneModel(Model);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIModel::PreInitModel()
{	
}
//-------------------------------------------------------------------------------------//
inline void CAOIModel::InitialModel()
{	
	m_ModelIndex = -1;
	m_ModelType = MODEL_TYPE_NULL;
	m_ModelChipSizeMode = CHIP_SIZE_NONE;
	m_ModelName = L"Model";
	m_ModelGroupName = L"Group";	
	m_ModelLandDirection = MODEL_LAND_COUNTER_CLOCKWISE;
	m_ModelPadAdjustMode = MODEL_LAND_ADJUST_INDIVIDUAL;
	m_ModelLeadAdjustMode = MODEL_LAND_ADJUST_INDIVIDUAL;
	m_ModelWndSyncMoveMode = WND_SYNC_MOVE_ROTATE;

	m_ModelClassID = MODEL_CLASS_ID_NONE;
	m_ModelActClassID = 0;
	m_ModelSelected = false;
	m_ModelUsing3D = true;
	m_ModelBypass3D = false;
	m_ModelIsolated = false;	
	m_ModelSelfTest = true;
	m_ModelSaveLeadReport = false;
	m_ModelBodyLinkChipLead = false;
	m_ModelTempInt[0] = 0;
	m_ModelTempInt[1] = 0;
	m_ModelTempInt[2] = 0;
	m_ModelTempInt[3] = 0;
	m_ModelResultID = RESULT_ID_NONE;
	m_ModelResultID_Alarm = RESULT_ID_NONE;
	m_ModelNeedSaveFiles = true;
	m_ModelInspectedTime = 0.0;	
	m_ModelModifiedDateTime = 0;
	//m_ModelWndLinkMode = MODEL_LINK_REGION_ALL;
	//m_ModelWndLinkMode = MODEL_LINK_REGION_NO_POS;	
	m_ModelWndLinkMode = MODEL_LINK_REGION_NO_LAND_POS;		
	m_ModelOpenMPCount = 0;
	m_ModelImageScale.x = m_ModelImageScale.y = 0.01;
	m_ModelModifiedCount = false;
	m_ModelDefectItemTest.SetAll(1);
	m_ModelDefectItemAlarm.SetAll(0);	
	m_ModelDefectItemEssential.SetAll(0);
	m_ModelDefectItemRecheck_ARS.SetAll(0);
	m_ModelAutoDeleteImageFolder = true;

	m_ModelUsedCount = 0;
	m_ModelBKImageIndex = 0;
	m_ModelBKImageNeedToGrab = true;
	
	m_ModelExtendRange.cx = 250;
	m_ModelExtendRange.cy = 250;
	m_ModelExtendAutoAdjust = true;
	m_ModelTotalRgn = TREGION4D();
	m_ModelTotalRgnRaw = TREGION4D();
	m_ModelTotalRgnCad = TREGION4D();
	m_ModelTotalRgnStage = TREGION4D();	
	m_ModelImageSize_um = TSIZE2D();
	m_ModelImageCadOffset_um = TPOINT2D();

	m_ModelTotalCornerPts[0] = TPOINT2D();
	m_ModelTotalCornerPts[1] = TPOINT2D();
	m_ModelTotalCornerPts[2] = TPOINT2D();
	m_ModelTotalCornerPts[3] = TPOINT2D();
	m_ModelTotalCornerPtsCad[0] = TPOINT2D();
	m_ModelTotalCornerPtsCad[1] = TPOINT2D();
	m_ModelTotalCornerPtsCad[2] = TPOINT2D();
	m_ModelTotalCornerPtsCad[3] = TPOINT2D();
	m_ModelTotalCornerPtsStage[0] = TPOINT2D();
	m_ModelTotalCornerPtsStage[1] = TPOINT2D();
	m_ModelTotalCornerPtsStage[2] = TPOINT2D();
	m_ModelTotalCornerPtsStage[3] = TPOINT2D();		

	m_ModelBodyLandRgn = TREGION4D();	
	m_ModelBodyLandCornerPts[0] = TPOINT2D();
	m_ModelBodyLandCornerPts[1] = TPOINT2D();
	m_ModelBodyLandCornerPts[2] = TPOINT2D();
	m_ModelBodyLandCornerPts[3] = TPOINT2D();

	m_ModelAttachedPtr = NULL;
	m_ModelAttachedAngle = 0;
	m_ModelAttachedCadPos = TPOINT2D();
	m_ModelAttachedStagePos = TPOINT2D();		
	
	m_ModelBodyBox = CAOIBox();		
	m_ModelBodySizeX = 0;
	m_ModelBodySizeY = 0;
	m_ModelBodyHeight = 0;	
	::memset(&m_ModelBodyImageRect_Raw, 0x00, sizeof(m_ModelBodyImageRect_Raw));
	
	AOIDataCollect.GetSpaceNoiseFilterParam(m_ModelSpaceNoiseFilterParam);
	m_ModelPanelBasePlane = 0;
	m_ModelMaskEnable_Base = false;//是否啟用
	m_ModelMaskFrameIndex_Base = 0;//影像序號
	m_ModelMaskFrameUniqueID_Base = FRAME_UNIQUE_ID_DEFAULT;//影像唯一碼
	m_ModelMaskColorGroupLinkIndex = PROJECT_COLOR_ID_BOARD_BEGIN;//彩色過濾的連動編號

	m_ModelDataModelEnabled = false;
	m_ModelDataModelLevelID = 3;

	InitialModelColorGroup();
	ClearModelAllObjList();	
	SetModelModifiedCount(false);	
	return;
}
//-------------------------------------------------------------------------------------//
void  CAOIModel::InitialModelColorGroup()
{	
	m_ModelBodyColorGroup.BuildColorGroup_Test();
	m_ModelBodyColorGroup.SetColorGroupFrameIndex(0);
	m_ModelBodyColorGroup.SetColorGroupFrameUniqueID(FRAME_UNIQUE_ID_LOW);
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::CloneModel(const CAOIModel &Model)
{
	m_ModelIndex = Model.m_ModelIndex;
	m_ModelType = Model.m_ModelType;	
	m_ModelName = Model.m_ModelName;	
	m_ModelGroupName = Model.m_ModelGroupName;		
	m_ModelChipSizeMode = Model.m_ModelChipSizeMode;
	m_ModelLandDirection = Model.m_ModelLandDirection;	
	m_ModelPadAdjustMode = Model.m_ModelPadAdjustMode;	
	m_ModelLeadAdjustMode = Model.m_ModelLeadAdjustMode;
	m_ModelWndSyncMoveMode = Model.m_ModelWndSyncMoveMode;
	m_ModelClassID = Model.m_ModelClassID;
	m_ModelActClassID = Model.m_ModelActClassID;	
	m_ModelSelected = Model.m_ModelSelected;
	m_ModelUsing3D = Model.m_ModelUsing3D;
	m_ModelBypass3D = Model.m_ModelBypass3D;
	m_ModelIsolated = Model.m_ModelIsolated;	
	m_ModelSelfTest = Model.m_ModelSelfTest;
	m_ModelSaveLeadReport = Model.m_ModelSaveLeadReport;		
	m_ModelBodyLinkChipLead = Model.m_ModelBodyLinkChipLead;
	m_ModelTempInt[0] = Model.m_ModelTempInt[0];
	m_ModelTempInt[1] = Model.m_ModelTempInt[1];
	m_ModelTempInt[2] = Model.m_ModelTempInt[2];
	m_ModelTempInt[3] = Model.m_ModelTempInt[3];
	m_ModelResultID = Model.m_ModelResultID;
	m_ModelResultID_Alarm = Model.m_ModelResultID_Alarm;
	m_ModelNeedSaveFiles = Model.m_ModelNeedSaveFiles;
	m_ModelInspectedTime = Model.m_ModelInspectedTime;	
	m_ModelModifiedDateTime = Model.m_ModelModifiedDateTime;
	m_ModelWndLinkMode = Model.m_ModelWndLinkMode;
	m_ModelOpenMPCount = Model.m_ModelOpenMPCount;	
	m_ModelImageScale = Model.m_ModelImageScale;	
	m_ModelModifiedCount = Model.m_ModelModifiedCount;	
	m_ModelDefectItemTest = Model.m_ModelDefectItemTest;
	m_ModelDefectItemAlarm = Model.m_ModelDefectItemAlarm;
	m_ModelDefectItemEssential = Model.m_ModelDefectItemEssential;
	m_ModelDefectItemRecheck_ARS = Model.m_ModelDefectItemRecheck_ARS;
	m_ModelAutoDeleteImageFolder = Model.m_ModelAutoDeleteImageFolder;

	m_ModelUsedCount = Model.m_ModelUsedCount;
	m_ModelBKImageIndex = Model.m_ModelBKImageIndex;
	m_ModelBKImageNeedToGrab = Model.m_ModelBKImageNeedToGrab;	
	
	m_ModelExtendRange = Model.m_ModelExtendRange;
	m_ModelExtendAutoAdjust = Model.m_ModelExtendAutoAdjust;
	m_ModelTotalRgn = Model.m_ModelTotalRgn;
	m_ModelTotalRgnRaw = Model.m_ModelTotalRgnRaw;
	m_ModelTotalRgnCad = Model.m_ModelTotalRgnCad;
	m_ModelTotalRgnStage = Model.m_ModelTotalRgnStage;
	m_ModelImageSize_um = Model.m_ModelImageSize_um;
	m_ModelImageCadOffset_um = Model.m_ModelImageCadOffset_um;

	m_ModelTotalCornerPts[0] = Model.m_ModelTotalCornerPts[0];
	m_ModelTotalCornerPts[1] = Model.m_ModelTotalCornerPts[1];
	m_ModelTotalCornerPts[2] = Model.m_ModelTotalCornerPts[2];
	m_ModelTotalCornerPts[3] = Model.m_ModelTotalCornerPts[3];

	m_ModelTotalCornerPtsCad[0] = Model.m_ModelTotalCornerPtsCad[0];
	m_ModelTotalCornerPtsCad[1] = Model.m_ModelTotalCornerPtsCad[1];
	m_ModelTotalCornerPtsCad[2] = Model.m_ModelTotalCornerPtsCad[2];
	m_ModelTotalCornerPtsCad[3] = Model.m_ModelTotalCornerPtsCad[3];

	m_ModelTotalCornerPtsStage[0] = Model.m_ModelTotalCornerPtsStage[0];
	m_ModelTotalCornerPtsStage[1] = Model.m_ModelTotalCornerPtsStage[1];
	m_ModelTotalCornerPtsStage[2] = Model.m_ModelTotalCornerPtsStage[2];
	m_ModelTotalCornerPtsStage[3] = Model.m_ModelTotalCornerPtsStage[3];		

	m_ModelBodyLandRgn = Model.m_ModelBodyLandRgn;
	m_ModelBodyLandCornerPts[0] = Model.m_ModelBodyLandCornerPts[0];
	m_ModelBodyLandCornerPts[1] = Model.m_ModelBodyLandCornerPts[1];
	m_ModelBodyLandCornerPts[2] = Model.m_ModelBodyLandCornerPts[2];
	m_ModelBodyLandCornerPts[3] = Model.m_ModelBodyLandCornerPts[3];	

	m_ModelAttachedPtr = Model.m_ModelAttachedPtr;
	m_ModelAttachedAngle = Model.m_ModelAttachedAngle;
	m_ModelAttachedCadPos = Model.m_ModelAttachedCadPos;
	m_ModelAttachedStagePos = Model.m_ModelAttachedStagePos;	

	m_ModelBodyBox = Model.m_ModelBodyBox;
	m_ModelBodySizeX = Model.m_ModelBodySizeX;
	m_ModelBodySizeY = Model.m_ModelBodySizeY;
	m_ModelBodyHeight = Model.m_ModelBodyHeight;
	m_ModelBodyImageRect_Raw = Model.m_ModelBodyImageRect_Raw;
	m_ModelBodyColorGroup = Model.m_ModelBodyColorGroup;
	m_ModelFolderModel = Model.m_ModelFolderModel;	
	m_ModelFolderComponent = Model.m_ModelFolderComponent;		
	
	m_ModelSpaceNoiseFilterParam = Model.m_ModelSpaceNoiseFilterParam;
	m_ModelPanelBasePlane = Model.m_ModelPanelBasePlane;
	m_ModelMaskEnable_Base = Model.m_ModelMaskEnable_Base;		
	m_ModelMaskFrameIndex_Base = Model.m_ModelMaskFrameIndex_Base;		
	m_ModelMaskFrameUniqueID_Base = Model.m_ModelMaskFrameUniqueID_Base;		
	m_ModelMaskColorGroupLinkIndex = Model.m_ModelMaskColorGroupLinkIndex;		

	m_ModelDefectWndUUIDList  = Model.m_ModelDefectWndUUIDList ;
	m_ModelDefectWndGroupIDList  = Model.m_ModelDefectWndGroupIDList ;

	m_ModelDataModelEnabled = Model.m_ModelDataModelEnabled;
	m_ModelDataModelLevelID = Model.m_ModelDataModelLevelID;	

	CloneModelObjectList(Model);	
	CloneModelUniFrameList(Model);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::CloneModelObjectList(const CAOIModel &Model)
{
	ClearModelAllObjList();	
	
	size_t       i=0, j=0, k=0;	
	size_t       WndIndexMapSize=0;
	size_t       LogicIndexMapSize=0;
	std::vector<size_t> ChildWndIndexMap;
	std::vector<size_t> LandLogicIndexMap;		

	size_t       LandIndex=0;
	CAOILand    *OldLandPtr = NULL;
	CAOILand    *NewLandPtr = NULL;
	std::vector<size_t>   LandIndexMap;
	size_t LandCount = Model.GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandIndexMap.push_back(-1);
		OldLandPtr = Model.GetModelLandPtr(i, false);		
		if ( NULL == OldLandPtr ) { continue; }
		
		//NewLandPtr = AddModelLandPtr(OldLandPtr, true);		
		NewLandPtr = OldLandPtr->CloneLandObj();
		if ( NULL == NewLandPtr ) { continue; }		
		AddModelLandPtr_Direct(NewLandPtr);
		NewLandPtr->SetLandModelPtr(this);
		NewLandPtr->SetLandIndex(LandIndex);		
		LandIndexMap[i] = NewLandPtr->GetLandIndex();
		LandIndex ++;
	}
	const size_t LandIndexCount = LandIndexMap.size();

	size_t       WndIndex=0;
	CAOIWnd     *OldWndPtr = NULL;
	CAOIWnd     *NewWndPtr = NULL;
	std::vector<size_t>   WndIndexMap;
	size_t WndCount = Model.m_ModelWndList.size();	
	for ( i=0; i<WndCount; i++ )
	{
		WndIndexMap.push_back(-1);
		OldWndPtr = Model.m_ModelWndList[i];		
		if ( NULL == OldWndPtr ) { continue; }
		
		//NewWndPtr = CAOIModel::AddModelWndPtr(OldWndPtr, true);		
		NewWndPtr = OldWndPtr->CloneWndObj();
		if ( NULL == NewWndPtr ) { continue; }
		AddModelWndPtr_Direct(NewWndPtr);
		NewWndPtr->SetWndModelPtr(this);
		NewWndPtr->SetWndIndex(WndIndex);	
		WndIndexMap[i] = NewWndPtr->GetWndIndex();
		WndIndex ++;
	}
	const size_t WndIndexCount = WndIndexMap.size();
	
	size_t         LogicIndex=0;
	CAOILogic     *OldLogicPtr = NULL;
	CAOILogic     *NewLogicPtr = NULL;
	std::vector<size_t>   LogicIndexMap;
	size_t LogicCount = Model.GetModelLogicCount_Inline();	
	for ( i=0; i<LogicCount; i++ )
	{
		LogicIndexMap.push_back(-1);
		OldLogicPtr = Model.GetModelLogicPtr_Inline(i);		
		if ( NULL == OldLogicPtr ) { continue; }
		
		//NewLogicPtr = CAOIModel::AddModelLogicPtr(OldLogicPtr, true);		
		NewLogicPtr = OldLogicPtr->CloneLogicObj();
		if ( NULL == NewLogicPtr ) { continue; }
		CAOIModel::AddModelLogicPtr_Inline(NewLogicPtr);
		NewLogicPtr->SetLogicModelPtr(this);
		NewLogicPtr->SetLogicIndex(LogicIndex);
		LogicIndexMap[i] = NewLogicPtr->GetLogicIndex();
		LogicIndex ++;
	}
	const size_t LogicIndexCount = LogicIndexMap.size();	
	
	//利用Index將指標整個重新連接起來	
	//Model Logic
	size_t LogicWndCount = 0;
	LogicCount = CAOIModel::GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		NewLogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);
		if ( NULL == NewLogicPtr ) { continue; }

		ChildWndIndexMap.clear();		
		LogicWndCount = NewLogicPtr->GetLogicWndPtrCount();	
		for ( j=0; j<LogicWndCount; j++ )
		{
			OldWndPtr = NewLogicPtr->GetLogicWndPtr(j, false);			
			if ( NULL == OldWndPtr ) { continue; }
			WndIndex = OldWndPtr->GetWndIndex();
			ChildWndIndexMap.push_back(WndIndex);
		}
		NewLogicPtr->RemoveLogicWndPtrList();
		WndIndexMapSize = ChildWndIndexMap.size();
		for ( j=0; j<WndIndexMapSize; j++ )
		{
			WndIndex = ChildWndIndexMap[j];			
			if ( (WndIndex<0) || (WndIndex>=WndIndexCount) ) { continue; }
			WndIndex = WndIndexMap[WndIndex];
			NewWndPtr = GetModelWndPtr(WndIndex, false);
			NewLogicPtr->AddLogicWndPtr(NewWndPtr);
		}
	}	

	//Model Land
	size_t LandWndCount = 0;
	size_t LandLogicCount = 0;
	LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		NewLandPtr = GetModelLandPtr(i, false);		
		if ( NULL == NewLandPtr ) { continue; }

		ChildWndIndexMap.clear();		
		LandWndCount = NewLandPtr->GetLandWndCount();	
		for ( j=0; j<LandWndCount; j++ )
		{
			OldWndPtr = NewLandPtr->GetLandWndPtr(j, false);			
			if ( OldWndPtr == NULL ) { continue; }			
			WndIndex = OldWndPtr->GetWndIndex();
			ChildWndIndexMap.push_back(WndIndex);
		}

		LandLogicIndexMap.clear();		
		LandLogicCount = NewLandPtr->GetLandLogicCount();	
		for ( j=0; j<LandLogicCount; j++ )
		{
			OldLogicPtr = NewLandPtr->GetLandLogicPtr(j, false);
			if ( NULL == OldLogicPtr ) { continue; }			
			LogicIndex = OldLogicPtr->GetLogicIndex();
			LandLogicIndexMap.push_back(LogicIndex);
		}

		NewLandPtr->RemoveLandWndList();
		NewLandPtr->RemoveLandLogicList();

		LogicIndexMapSize = LandLogicIndexMap.size();
		for ( j=0; j<LogicIndexMapSize; j++ )
		{
			LogicIndex = LandLogicIndexMap[j];			
			if ( (LogicIndex<0) || (LogicIndex>=LogicIndexCount) ) { continue; }
			LogicIndex = LogicIndexMap[LogicIndex];
			NewLogicPtr = CAOIModel::GetModelLogicPtr_Inline(LogicIndex);
			NewLandPtr->AddLandLogicPtr(NewLogicPtr);
		}

		WndIndexMapSize = ChildWndIndexMap.size();
		for ( j=0; j<WndIndexMapSize; j++ )
		{
			WndIndex = ChildWndIndexMap[j];			
			if ( (WndIndex<0) || (WndIndex>=WndIndexCount) ) { continue; }
			WndIndex = WndIndexMap[WndIndex];
			NewWndPtr = GetModelWndPtr(WndIndex, false);
			NewLandPtr->AddLandWndPtr(NewWndPtr);
		}
	}	

#ifdef _DEBUG
	const size_t SrcModelWndCount = Model.GetModelWndCount();
	const size_t DstModelWndCount = GetModelWndCount();
	if ( DstModelWndCount != SrcModelWndCount )
	{
		return;
	}
#endif
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::RemoveModelImageFolder()
{
	if ( false == m_ModelAutoDeleteImageFolder ) { return; }
	ClearModelFolder(MODEL_CLEAR_FOLDER_PATTERNS);
	return;

	//移除對應的樣版資料夾
	size_t   i=0, j=0;	
	size_t   RemovedCount=0;
	int      AlgGroupID = 0;	
	CAOIWnd *WndPtr = NULL;
	CString  ImageFolder, FullImageFolder;
	std::vector<int> RemovedAlgGroupIDList;
	CString  ModelFolder = GetModelFolder();		
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndAlgParam().GetAlgPatternFileUsed() == false ) { continue; }

		AlgGroupID = WndPtr->GetWndAlgGroupID();		
		RemovedCount = RemovedAlgGroupIDList.size();
		for ( j=0; j<RemovedCount; j++ )
		{
			if ( AlgGroupID == RemovedAlgGroupIDList[j] ) 
			{	break; }
		}
		if ( j != RemovedCount ) { continue; }
		RemovedAlgGroupIDList.push_back(AlgGroupID);
		ImageFolder = AOIDataDefine.GetAlgPatternFolder(AlgGroupID);
		FullImageFolder.Format(_T("%s\\%s"), ModelFolder, ImageFolder);
		JetAPI::RemoveFolder(FullImageFolder);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ClearModelFolder(DWORD dwClearFlags)//清除模組資料夾
{	
	int        i = 0;
	CAOIWnd   *WndPtr = NULL;
	CString    FullFolder;
	CString    PatternFolder;
	CString    ModelBkImageName;
	CString    ModelFolder = GetModelFolder();	
	const int  MaxAlgFreeGroupID = GetModelAlgFreeGroupID();
	DWORD      dwClearPattern = dwClearFlags&MODEL_CLEAR_FOLDER_PATTERNS;
	DWORD      dwClearBKImages = dwClearFlags&MODEL_CLEAR_FOLDER_BK_IMAGES;
	if ( 0 != dwClearPattern )
	{
		for ( i=0; i<MaxAlgFreeGroupID; i++ )
		{
			PatternFolder = AOIDataDefine.GetAlgPatternFolder(i);
			FullFolder.Format(_T("%s\\%s"), ModelFolder, PatternFolder);			
			JetAPI::RemoveFolder(FullFolder);
		}
	}

	if ( 0 != dwClearBKImages )
	{
		for ( i=0; i<FRAME_MAX_COUNT; i++ )
		{
			ModelBkImageName = GetModelBKImageFilename(i);	
			::DeleteFile(ModelBkImageName);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIModel* CAOIModel::CloneModelObj() const//建立且複製一個模組
{
	CAOIModel *ObjPtr = AOIObjManager.CreateModelObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
bool CAOIModel::InitModelWndOrderList_Clone()
{
	const int WndCount = GetModelWndCount();
	CAOIWnd* WndPtr = NULL;
	if (!m_ModelWndOrderList.empty()) { return false; }
	m_ModelWndOrderList.resize(WndCount);
	for (int i = 0; i < WndCount; i++) {
		WndPtr = GetModelWndPtr(i, false);
		if (WndPtr == NULL) { continue; }
		int WndIndex = WndPtr->GetWndOrderIndex();
		m_ModelWndOrderList[WndIndex] = WndPtr;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIModel::CheckModelPtr(CAOIModel *Ptr)//確認模組指標
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIModel::GetModelLogicCount_Inline() const
{
	return CAOIModel::m_ModelLogicList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIModel::AddModelLogicPtr_Inline(CAOILogic *LogicPtr)
{
	CAOIModel::m_ModelLogicList.push_back(LogicPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOILogic* CAOIModel::GetModelLogicPtr_Inline(size_t index) const
{
	return (CAOIModel::m_ModelLogicList[index]);	
}
//-------------------------------------------------------------------------------------//
CWndDefectItem CAOIModel::CheckModelTestDefectItems() const//確認模組檢測瑕疵項目
{
	size_t        i=0;		
	WND_DEFECT_ID WndDefectID;
	CWndDefectItem DefectItem;
	CAOIWnd      *WndPtr = NULL;
	const size_t  WndCount = GetModelWndCount();

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		DefectItem.AddItemCount(WndDefectID);
	}
	return DefectItem;
}
//-------------------------------------------------------------------------------------//
CWndDefectItem CAOIModel::CheckModelEnabledDefectItems() const//確認模組檢測瑕疵項目
{
	size_t        i=0;		
	WND_DEFECT_ID WndDefectID;
	CWndDefectItem DefectItem;
	CAOIWnd      *WndPtr = NULL;
	const size_t  WndCount = GetModelWndCount();

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false )
		{	continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		DefectItem.AddItemCount(WndDefectID);		
	}
	return DefectItem;
}
//-------------------------------------------------------------------------------------//
CWndDefectItem CAOIModel::CheckModelBypassedDefectItems() const//確認模組不檢測瑕疵項目
{	//要先UpdateModelWndBypassed
	size_t        i=0;		
	WND_DEFECT_ID WndDefectID;
	CWndDefectItem DefectItem;
	CAOIWnd      *WndPtr = NULL;
	const size_t  WndCount = GetModelWndCount();

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndBypassed() == false )
		{	continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		DefectItem.AddItemCount(WndDefectID);		
	}
	return DefectItem;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelWndDefectItemAlarm()//更新模組檢測框瑕疵警報
{
	CAOIWnd *WndPtr = NULL;
	const size_t WndCount=GetModelWndCount();
	const CWndDefectItem &DefectItemAlarm=GetModelDefectItemAlarm();

	for ( size_t i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		if ( DefectItemAlarm.GetItemCount(WndPtr->GetWndDefectID()) > 0 )
		{	WndPtr->SetWndDefectAlarm(true);	}
		else
		{	WndPtr->SetWndDefectAlarm(false);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelDefectItemEssential(std::vector<WND_DEFECT_ID> &BadList) const
{	
	BadList.clear();
	const size_t WndCount=GetModelWndCount();
	const CWndDefectItem &ItemEssential=GetModelDefectItemEssential();
	const std::vector<WND_DEFECT_ID> &WndDefectIDList=AOIDataCollect.GetWndDefectIDList();	
	const size_t WndDefectIDCount=WndDefectIDList.size();
	for ( size_t i=0; i<WndDefectIDCount; i++ )
	{
		WND_DEFECT_ID WndDefectID=WndDefectIDList[i];
		if ( 0 == ItemEssential.GetItemCount(WndDefectID) ) { continue; }

		bool bTest=false;
		for ( size_t j=0; j<WndCount; j++ )
		{
			CAOIWnd *WndPtr=GetModelWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndDefectID() != WndDefectID ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			bTest=true;			
			break;
		}		
		if ( false==bTest )
		{	BadList.push_back(WndDefectID);	}
	}
	if ( BadList.size() > 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelName(const char* value)
{
	if ( NULL == value ) { return ; }
	JetAPI::char2wstring(value, m_ModelName);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelName(const wchar_t* value)
{
	if ( NULL == value ) { return ; }
	m_ModelName = value;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelGroupName(const char* value)
{
	if ( NULL == value ) { return ; }
	JetAPI::char2wstring(value, m_ModelGroupName);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelGroupName(const wchar_t* value)
{
	if ( NULL == value ) { return ; }
	m_ModelGroupName = value;	
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIModel::GetModelFolder() const
{
	bool bIsolated = false;;
	bIsolated = GetModelIsolated();
	if ( true == bIsolated ) 
	{	
		if ( m_ModelFolderComponent.GetLength() > 0 )
		{	return m_ModelFolderComponent; }		
	}
	return m_ModelFolderModel;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AssignModelFolder()//指派模組資料夾至旗下演算法內
{
	size_t       i=0;
	int          AlgGroupID = 0;
	int          PatternFolderIndex = 0;
	CAOIWnd     *WndPtr = NULL;	
	CString      ModelFolder;
	CString      PatternFolder;
	CString      AlgPatternFolder;	
	const size_t WndCount = GetModelWndCount();
	ModelFolder = GetModelFolder();

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();				
		AlgGroupID = AlgParam.GetAlgGroupID();
		if ( AlgParam.GetAlgPatternFileUsed() == false ) { continue; }
		
		PatternFolderIndex = AlgGroupID;	
		PatternFolder = AOIDataDefine.GetAlgPatternFolder(PatternFolderIndex);
		AlgPatternFolder.Format(_T("%s\\%s"), ModelFolder, PatternFolder);
		AlgParam.SetAlgPatternFolder(AlgPatternFolder);
		JetAPI::CreateFolder(AlgPatternFolder);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RotateModelBKImage(double Angle)//旋轉模組底圖
{
	const double Precision = DBL_PRECISION;
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	if ( true == IsExceptionAngle ) { return false; }
	if ( fabs(Angle)<Precision || fabs(Angle-360.0)<Precision ) { return true; } 

	size_t       i=0;	
	CString      strBKImage;	
	const size_t MaxFrameCount = FRAME_MAX_COUNT;
	for ( i=0; i<MaxFrameCount; i++ )
	{
		strBKImage = GetModelBKImageFilename(i);
		if ( JetAPI::IsFileExist(strBKImage) == false ) { continue; }
		if ( ImageAPI.RotateImageFile(Angle, strBKImage, strBKImage) == false ) 
		{	continue; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIModel::GetModelBKImageFilename(int UniFrameIdx) const
{	
	CString ModelName = CAOIModel::GetModelName();
	LPCTSTR ModelFolder = CAOIModel::GetModelFolder();
	return AOIDataDefine.GetModelBKImageFilename(ModelFolder, ModelName, UniFrameIdx);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelRegion(TREGION4D &Region)
{
	size_t       i = 0;	
	TREGION4D    RgnBox;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;			
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();

	m_ModelBodyBox.GetBoxRegion(Region);
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		
		BoxPtr = LandPtr->GetLandPadBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnBox);
			JetAPI::UnionRegion(RgnBox, Region, Region);			
		}

		//Electrode/Lead
		BoxPtr = LandPtr->GetLandLeadBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnBox);
			JetAPI::UnionRegion(RgnBox, Region, Region);			
		}

		//Lead Tip
		BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnBox);
			JetAPI::UnionRegion(RgnBox, Region, Region);
		}

		//Lead Shoulder
		BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnBox);
			JetAPI::UnionRegion(RgnBox, Region, Region);
		}
	}	

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		

		if ( WndPtr->GetWndExtendBoxUsed() == true )
		{	BoxPtr = WndPtr->GetWndExtendBoxPtr();	}
		else
		{	BoxPtr = WndPtr->GetWndBoxPtr();	}
		BoxPtr->GetBoxRegion(RgnBox);
		JetAPI::UnionRegion(RgnBox, Region, Region);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
double CAOIModel::GetFitScale(const RECT &WndRect)
{
	size_t i=0;	
	TREGION4D Region;	
	CAOIModel::GetModelRegion(Region);

	if ( fabs(Region.minX) < Region.maxX ) { Region.minX = -Region.maxX; }
	else { Region.maxX = -Region.minX; }

	if ( fabs(Region.minY) < Region.maxY ) { Region.minY = -Region.maxY; }
	else { Region.maxY = -Region.minY; }

	const double ResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const double Width  = (Region.maxX-Region.minX)/ResX;
	const double Height = (Region.maxY-Region.minY)/ResY;	
	const double WndW = WndRect.right-WndRect.left;
	const double WndH = WndRect.bottom-WndRect.top;

	if ( abs(Width)<0.1 || abs(Height)<0.1 ) { return 1; }

	const double ScaleX = WndW/Width;
	const double ScaleY = WndH/Height;

	const double NewWndH = Height*ScaleX;//以ScaleX為準的比例
	const double NewWndW = Width*ScaleY;//以ScaleY為準的比例

	if ( NewWndH < WndH ) 
	{	
		return (1.0/ScaleX); 
	}
	
	return (1.0/ScaleY);
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::UnSelectModel()
{
	GetModelBodyBox().SetBoxSelected(false);
	UnSelectModelWnd();
	UnSelectModelLand(false);	
	UnSelectModelLogic();
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::InvisibleModel()
{
	GetModelBodyBox().SetBoxVisibled(false);
	InvisibleModelWnd();
	InvisibleModelLand(false);		
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::MoveModel(double x, double y)
{
	size_t i=0;
	m_ModelBodyBox.MoveBox(x, y);	
	
	CAOILand *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->MoveLand(x, y);
	}

	CAOIWnd  *WndPtr  = NULL;
	const size_t WndCount = GetModelWndCount();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr ) { continue; }
		WndPtr->MoveWnd(x, y);
	}

	for ( i=0; i<4; i++ )
	{
		m_ModelTotalCornerPts[i].x += x;
		m_ModelTotalCornerPts[i].y += y;

		m_ModelBodyLandCornerPts[i].x += x;
		m_ModelBodyLandCornerPts[i].y += y;
	}
	m_ModelTotalRgn.Move(x, y);
	m_ModelTotalRgnRaw.Move(x, y);
	m_ModelBodyLandRgn.Move(x, y);
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::ScaleModel(double cx, double cy)
{
	size_t i=0;
	bool bIncludeRes=true;
	double ScaleX=1.0, ScaleY=1.0;
	double BodySizeX=0, BodySizeY=0;
	m_ModelBodyBox.GetBoxSize(BodySizeX, BodySizeY);

	ScaleX = cx/BodySizeX;
	ScaleY = cy/BodySizeY;
	ScaleModelProperty(ScaleX, ScaleY);	
	m_ModelBodyBox.ScaleBox(ScaleX, ScaleY, bIncludeRes);
	
	CAOILand *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->ScaleLand(ScaleX, ScaleY, bIncludeRes);
	}

	CAOIWnd  *WndPtr  = NULL;
	const size_t WndCount = GetModelWndCount();	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr ) { continue; }
		WndPtr->ScaleWnd(ScaleX, ScaleY, bIncludeRes);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::ScaleModelProperty(double sx, double sy)
{
	m_ModelBodySizeX *= sx;
	m_ModelBodySizeY *= sy;
	return;
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::RotateModel(double Angle, double CPX, double CPY)
{
	size_t i=0;			
	double dTemp=0;
	Angle = JetAPI::RotateAngle(0, Angle);
	const int AngleLable = JetAPI::GetAngleLabel(Angle);
	switch ( AngleLable )
	{
	case  90:
	case 270:
		JetAPI::Swap(m_ModelBodySizeX, m_ModelBodySizeY);
		JetAPI::Swap(m_ModelExtendRange.cx, m_ModelExtendRange.cy);		
		break;
	}
	m_ModelAttachedAngle = JetAPI::RotateAngle(m_ModelAttachedAngle, Angle);
	m_ModelBodyBox.RotateBox(Angle, CPX, CPY);
	
	CAOIWnd  *WndPtr  = NULL;		
	CAOILand *LandPtr = NULL;		
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->RotateLand(Angle, CPX, CPY);		
	}	

	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }	
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr ) { continue; }
		WndPtr->RotateWnd(Angle, CPX, CPY);		
	}

	TPOINT2D CadPos, StagePos;	
	GetModelAttachedPosCad(CadPos);
	GetModelAttachedPosStage(StagePos);	
	JetAPI::RotateCornerPos(Angle, CPX, CPY, m_ModelTotalCornerPts);
	JetAPI::CornerPtToRegion(m_ModelTotalCornerPts, m_ModelTotalRgn);
	m_ModelTotalRgnRaw = m_ModelTotalRgn;

	JetAPI::MoveCornerPts(m_ModelTotalCornerPts, CadPos, m_ModelTotalCornerPtsCad);
	JetAPI::CornerPtToRegion(m_ModelTotalCornerPtsCad, m_ModelTotalRgnCad);
	
	AOIDataCollect.MapCadOffsetCornerPtsToStage(m_ModelTotalCornerPts, m_ModelTotalCornerPtsStage);
	JetAPI::MoveCornerPts(m_ModelTotalCornerPtsStage, StagePos, m_ModelTotalCornerPtsStage);
	JetAPI::CornerPtToRegion(m_ModelTotalCornerPtsStage, m_ModelTotalRgnStage);

	JetAPI::RotateCornerPos(Angle, CPX, CPY, m_ModelBodyLandCornerPts);
	JetAPI::CornerPtToRegion(m_ModelBodyLandCornerPts, m_ModelBodyLandRgn);	

	//AOIDataCollect.MapCadOffsetRgnToStage(m_ModelTotalRgn, m_ModelTotalRgnStage);
	//JetAPI::MoveRegion(m_ModelTotalRgnStage, StagePos, m_ModelTotalRgnStage);	
	//JetAPI::MoveCornerPts(m_ModelTotalCornerPtsStage, StagePos, m_ModelTotalCornerPtsStage);
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::MirrorModelXAxis(double CPY)
{
	size_t i=0;	
	m_ModelAttachedAngle = JetAPI::MirrorXAxisAngle(CAOIModel::m_ModelAttachedAngle);
	m_ModelBodyBox.MirrorBoxXAxis(CPY);
	
	CAOIWnd  *WndPtr  = NULL;	
	CAOILand *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->MirrorLandXAxis(CPY);
	}

	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }	
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr ) { continue; }
		WndPtr->MirrorWndXAxis(CPY);
	}	
	return ;	
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::MirrorModelYAxis(double CPX)
{
	size_t i=0;	
	m_ModelAttachedAngle = JetAPI::MirrorYAxisAngle(CAOIModel::m_ModelAttachedAngle);
	m_ModelBodyBox.MirrorBoxYAxis(CPX);

	CAOIWnd  *WndPtr = NULL;	
	CAOILand *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->MirrorLandYAxis(CPX);
	}

	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }	
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr ) { continue; }
		WndPtr->MirrorWndYAxis(CPX);
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOIModel::DrawModel(HDC hDC, DRAW_MODEL_MODE DrawMode, const TMODEL_DRAW_PARAM &DrawParam)
{
	int OldBKMode = ::SetBkMode(hDC, TRANSPARENT);
	switch ( DrawMode )
	{
	case DRAW_MODEL_RESULT:
		DrawModelResult(hDC, DrawParam);
		break;
	case DRAW_MODEL_TEMP:
		DrawModelTemp(hDC, DrawParam);
		break;
	default://DRAW_MODEL_EDIT
		DrawModelEdit(hDC, DrawParam);
		break;
	}
	::SetBkMode(hDC, OldBKMode);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::DrawModelEdit(HDC hDC, const TMODEL_DRAW_PARAM &ShowParam)
{
	if ( NULL == hDC) { return ; }	

//	CString str;
	int i=0, j=0, k=0;	
	int u=0, v=0, w=0;		
	size_t       WndCount = 0;
	size_t       BoxCount = 0;	
	unsigned int BoxIndex = 0;
	size_t       LandCount= 0;	
	int          ActiveLandType = 0;	
	size_t       ItemCount = 0;
	size_t       AlgCount = 0;		
	size_t       BoxResCount = 0;	
	size_t       WndRoiCount = 0;
	size_t       MaskWndCount = 0;
	bool   ShowEditLine2 = false;	
	double PenScale = 1;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;	
	TBOX_DRAW_PARAM   BoxDrawParam;
	TMODEL_DRAW_PARAM DrawParam = ShowParam;

	bool         WndRoiSelected=false;
	bool         MaskBoxSelected=false;
	double       Scale = DrawParam.Scale;
	double       CornerPtX[4]={0};
	double       CornerPtY[4]={0};
	const double ComponentAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);	
	const double ResX = DrawParam.ResolutionX;
	const double ResY = DrawParam.ResolutionY;
	const double ModelCPX = 0;
	const double ModelCPY = 0;
	const int    WndCPX = (DrawParam.WndRect.left+DrawParam.WndRect.right)/2;
	const int    WndCPY = (DrawParam.WndRect.top+DrawParam.WndRect.bottom)/2;	
	const bool   ShowWndIndex = AOIDataCollect.GetShowModelWndIndex();
	const bool   ShowLandIndex = AOIDataCollect.GetShowModelLandIndex();
	const bool   ShowModelWndMaskAll = true;

	BOOL         bShowWnd = FALSE;
	BOOL         bShowLand = FALSE;
	
	CAOIWnd      *WndPtr = NULL;	
	CAOILand     *LandPtr = NULL;	
	CAOIBox      *BoxPtr = NULL;			
	CAOIWndRoi   *WndRoiPtr = NULL;
	CAOIWndMask  *MaskWndPtr = NULL;	

	BOX_SHAPE_MODE WndShapeMode=BOX_SHAPE_RECTANGLE;	
	if ( Scale < 0 ) 
	{
		Scale = CAOIModel::GetFitScale(DrawParam.WndRect);
		Scale = Scale*1.2;		
	}
	const double ZoomX = ResX*Scale;
	const double ZoomY = ResY*Scale;
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const int    LineSizeLevel = SysParam.m_EditLineSizeLevel;
	HPEN hOldPen = NULL;	
	HPEN hNullPen = CAOIModel::hNullPen;
	HPEN hComPen = CAOIModel::hComPen;
	HPEN hPadPen = CAOIModel::hPadPen;
	HPEN hLeadPen = CAOIModel::hLeadPen;
	HPEN hShoulderPen = CAOIModel::hShoulderPen;
	HPEN hLeadTipPen = CAOIModel::hLeadTipPen;
	HPEN hSelPen = CAOIModel::hSelPenEdit;
	HPEN hSelPenW = CAOIModel::hSelPenEditW;	
	HPEN hLandPen = CAOIModel::hLandPen;
	HPEN hWndPen = CAOIModel::hWndPen;
	HPEN hNGPen = CAOIModel::hNGPen;
	HPEN hOKPen = CAOIModel::hOKPen;
	HPEN hBoxPen = CAOIModel::hBoxPen;
	HPEN hExtendPen = CAOIModel::hExtendPen;	
	HPEN hSubBoxPen1 = CAOIModel::hSubBoxPen1;
	HPEN hSubBoxPen2 = CAOIModel::hSubBoxPen2;
	HPEN hSubBoxPen3 = CAOIModel::hSubBoxPen3;
	HPEN hSubBoxPen4 = CAOIModel::hSubBoxPen4;
	HPEN hMaskBoxPen1 = CAOIModel::hMaskBoxPen1;
	HPEN hMaskBoxPen2 = CAOIModel::hMaskBoxPen2;
	HPEN hIntervalPen = CAOIModel::hIntervalPen;

	HBRUSH hOldBrush = NULL;
	HBRUSH hBrush = CAOIModel::hEditBrush;	

	hOldPen = (HPEN)::SelectObject(hDC, hComPen);	
	hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);	

	PenScale = 1/Scale;

	//BoxWnd List	
	bShowWnd = FALSE;
	bShowLand = FALSE;	
	
	BoxDrawParam.Zoom.x = ZoomX;
	BoxDrawParam.Zoom.y = ZoomY;
	BoxDrawParam.ModelCP.x = ModelCPX;
	BoxDrawParam.ModelCP.y = ModelCPY;
	BoxDrawParam.ViewOffset.x = DrawParam.ViewOffsetX;
	BoxDrawParam.ViewOffset.y = DrawParam.ViewOffsetY;	
	BoxDrawParam.ComponentAngle = ComponentAngle;
	BoxDrawParam.IsExceptionAngle = IsExceptionAngle;
	BoxDrawParam.DrawEditLine = ShowEditLine2;
	BoxDrawParam.LineScale = PenScale;
	BoxDrawParam.WndCP.x = WndCPX;
	BoxDrawParam.WndCP.y = WndCPY;
	BoxDrawParam.WndRect = DrawParam.WndRect;
	BoxDrawParam.ViewCP = DrawParam.ViewCP;
	BoxDrawParam.TowardSize = JetAPI::GetTowardSize(Scale);
	BoxDrawParam.EditLineSize = JetAPI::GetEditLineSize(Scale, LineSizeLevel);	
	BoxDrawParam.hPenNull = CAOIModel::hNullPen;
	BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrush;
	if ( GetModelEditMode() == false )	
	{	DrawParam.ShowEditLine = false;	}	

	BoxPtr = GetModelBodyBoxPtr();
	if ( BoxPtr->GetBoxVisibled() == true )
	{
		if ( BoxPtr->GetBoxSelected() == true )
		{	::SelectObject(hDC, hSelPen);	}
		else
		{	::SelectObject(hDC, hComPen); }
	
		BoxDrawParam.Toward = BoxPtr->GetBoxToward();		
		if ( true == DrawParam.ShowEditLine )
		{
			//BoxPtr->SetBoxSelected(false);
			if ( (BoxPtr->GetBoxEditabled()==false) || (BoxPtr->GetBoxSelected()==false) )
			{	BoxDrawParam.DrawEditLine = false;	}
			else
			{	BoxDrawParam.DrawEditLine = true; }	
		}
		else
		{	BoxDrawParam.DrawEditLine = false;	}		
		
		BoxDrawParam.Extend.x = 0;
		BoxDrawParam.Extend.y = 0;
		if ( BoxPtr->GetBoxSelected() == false )
		{	BoxPtr->DrawBoxEdit(hDC, BoxDrawParam); }
		else
		{
			::SelectObject(hDC, hSelPenW);
			BoxPtr->DrawBoxEdit(hDC, BoxDrawParam);
			::SelectObject(hDC, hSelPen);
			BoxPtr->DrawBoxEdit(hDC, BoxDrawParam);
		}
	}
	
	//Draw Land Box
	//if ( (bShowBoxWnd==FALSE) || (bShowLandBoxWnd==TRUE) )
	if ( true == DrawParam.ShowLandBox )
	{
		bool bShowenLandIndex = false;
		BoxDrawParam.DrawBoxIndex = false;
		BoxDrawParam.strBoxIndex = _T("");
		BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;
		LandCount = GetModelLandCount();
		for ( j=0; j<LandCount; j++ )
		{					
			LandPtr = GetModelLandPtr(j, false);
			if ( LandPtr == NULL ) { continue; }

			bShowenLandIndex = false;
			//Draw Inactive Box
			::SelectObject(hDC, hLandPen);				
			for ( k=0; k<5; k++ )
			{
				switch ( k )
				{
				case 0:	
					::SelectObject(hDC, hPadPen);
					BoxPtr = LandPtr->GetLandPadBoxPtr();	
					break;
				case 1:	
					::SelectObject(hDC, hLeadPen);
					BoxPtr = LandPtr->GetLandLeadBoxPtr();	
					break;
				case 2:	
					::SelectObject(hDC, hLeadTipPen);
					BoxPtr = LandPtr->GetLandLeadTipBoxPtr();	
					break;
				case 3:	
					::SelectObject(hDC, hShoulderPen);
					BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();	
					break;
				case 4:
					::SelectObject(hDC, hComPen);
					BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();	
					break;
				default:
					BoxPtr = NULL;
					::SelectObject(hDC, hLandPen);
					break;
				}								
				if ( BoxPtr == NULL ) { continue; }
				if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
				if ( BoxPtr->GetBoxVisibled() == false ) { continue; }
				if ( BoxPtr->GetBoxSelected() == true ) { continue; }
				if ( true == ShowLandIndex )
				{	
					if ( false == bShowenLandIndex )
					{
						BoxDrawParam.DrawBoxIndex = true;
						BoxDrawParam.strBoxIndex.Format(_T("%d"), LandPtr->GetLandIndex()+1);
						bShowenLandIndex = true;
					}
				}
				BoxDrawParam.Toward = BOX_TOWARD_NULL; 					
				BoxDrawParam.DrawEditLine = false;
				BoxPtr->DrawBoxEdit(hDC, BoxDrawParam);
				BoxDrawParam.DrawBoxIndex = false;
			}

			//Draw Selected Box			
			for ( k=0; k<5; k++ )
			{
				switch ( k )
				{
				case 0:	BoxPtr = LandPtr->GetLandPadBoxPtr();	break;
				case 1:	BoxPtr = LandPtr->GetLandLeadBoxPtr();	break;
				case 2:	BoxPtr = LandPtr->GetLandLeadTipBoxPtr();	break;
				case 3:	BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();	break;
				case 4:	BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();	break;
				default: BoxPtr = NULL;	break;
				}			
				if ( BoxPtr == NULL ) { continue; }
				if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
				if ( BoxPtr->GetBoxVisibled() == false ) { continue; }
				if ( BoxPtr->GetBoxSelected() == false ) { continue; }
				
				if ( true == ShowLandIndex )
				{	
					if ( false == bShowenLandIndex )
					{
						BoxDrawParam.DrawBoxIndex = true;
						BoxDrawParam.strBoxIndex.Format(_T("%d"), LandPtr->GetLandIndex()+1);
						bShowenLandIndex = true;
					}
				}
				bShowLand = TRUE;
				BoxDrawParam.Toward = LandPtr->GetLandToward();					
				if ( (DrawParam.ShowEditLine==true) && (BoxPtr->GetBoxEditabled()==true) )
				{	BoxDrawParam.DrawEditLine = true;;	}
				else
				{	BoxDrawParam.DrawEditLine = false; }
				::SelectObject(hDC, hSelPenW);
				BoxPtr->DrawBoxEdit(hDC, BoxDrawParam);
				BoxDrawParam.DrawBoxIndex = false;
				::SelectObject(hDC, hSelPen);
				BoxPtr->DrawBoxEdit(hDC, BoxDrawParam);
			}
		}
	}		
	
	if ( true == DrawParam.ShowWndBox )
	{
		BoxDrawParam.DrawBoxIndex = false;
		BoxDrawParam.strBoxIndex = _T("");
		BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;
		WndCount = GetModelWndCount();
		for ( j=0; j<WndCount; j++ )
		{					
			WndPtr = GetModelWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }			
			if ( WndPtr->GetWndEnabled() == false ) 
			{ 
				if ( WndPtr->GetWndSelected() == false )
				{	continue;  }
			}
			if ( WndPtr->GetWndVisibled() == false ) { continue; }			

			WndRoiSelected=false;
			MaskBoxSelected=false;
			
			if ( WndPtr->GetWndSelected()==true || true==ShowModelWndMaskAll )
			{	
				//繪製子框	
				if ( WndPtr->GetWndSelected() == true )
				{					
					BoxDrawParam.Toward = BOX_TOWARD_NULL;				
					::SelectObject(hDC, hSubBoxPen1);
					WndRoiCount = WndPtr->GetWndRoiWndCount();
					for ( k=0; k<WndRoiCount; k++ )
					{
						WndRoiPtr = WndPtr->GetWndRoiWndPtr(k, false);
						if ( WndRoiPtr == NULL ) { continue; }				
						if ( WndRoiPtr->SetWndRoiBoxAlgDrawParam(hDC, BoxDrawParam) == false) { continue; }
						if ( WndRoiPtr->GetWndRoiSelected() == true ) 
						{
							WndRoiSelected = true;
							BoxDrawParam.DrawEditLine = true;
						}
						else
						{	BoxDrawParam.DrawEditLine = false; }	
						WndRoiPtr->DrawWndRoiBoxEdit(hDC, BoxDrawParam);
						if ( WndRoiPtr->CheckWndRoiBoxAlgUseExtend() == false) { continue; }
						::SelectObject(hDC, hSubBoxPen3);
						BoxDrawParam.DrawEditLine = false;
						BoxDrawParam.Extend.x = WndPtr->GetWndExtendRangeX();
						BoxDrawParam.Extend.y = WndPtr->GetWndExtendRangeY();
						WndRoiPtr->DrawWndRoiBoxEdit(hDC, BoxDrawParam);
					}					
				}
				//繪製遮罩框
				BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;
				BoxDrawParam.DrawBoxIndex = false;
				BoxDrawParam.strBoxIndex = _T("");
				BoxDrawParam.Toward = BOX_TOWARD_NULL;
				BoxDrawParam.DrawEditLine = false;
				BoxDrawParam.FillRegion = true;
				BoxDrawParam.DrawTowardFeature = false;								
				//BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrush;
				MaskWndCount = WndPtr->GetWndMaskWndCount();
				for ( k=0; k<MaskWndCount; k++ )
				{
					MaskWndPtr = WndPtr->GetWndMaskWndPtr(k, false);
					if ( MaskWndPtr == NULL ) { continue; }
					if ( MaskWndPtr->GetWndMaskSelected() == true ) 
					{	continue; }	
					if ( MaskWndPtr->GetWndMaskEraseMode() == false ) 
					{
						::SelectObject(hDC, hMaskBoxPen1);
						BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrush;	
					}
					else
					{
						::SelectObject(hDC, hMaskBoxPen2);
						BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrushErase;	
					}
					BoxDrawParam.Toward = MaskWndPtr->GetWndMaskToward();
					BoxDrawParam.ShapeMode = MaskWndPtr->GetWndMaskShapeMode();
					MaskWndPtr->DrawWndMaskEdit(hDC, BoxDrawParam);
				}
				BoxDrawParam.DrawEditLine = true;
				for ( k=0; k<MaskWndCount; k++ )
				{
					MaskWndPtr = WndPtr->GetWndMaskWndPtr(k, false);
					if ( MaskWndPtr == NULL ) { continue; }
					if ( MaskWndPtr->GetWndMaskSelected() == false ) 
					{	continue; }

					if ( MaskWndPtr->GetWndMaskEraseMode() == false ) 
					{
						::SelectObject(hDC, hMaskBoxPen1);
						BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrush;	
					}
					else
					{
						::SelectObject(hDC, hMaskBoxPen2);
						BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrushErase;	
					}

					MaskBoxSelected = true; 					
					BoxDrawParam.Toward = MaskWndPtr->GetWndMaskToward();
					BoxDrawParam.ShapeMode = MaskWndPtr->GetWndMaskShapeMode();
					MaskWndPtr->DrawWndMaskEdit(hDC, BoxDrawParam);
				}
				BoxDrawParam.FillRegion = false;
				BoxDrawParam.DrawTowardFeature = true;
				/*
				BoxResCount = WndPtr->GetWndResultBoxCount();
				BoxDrawParam.Toward = BOX_TOWARD_NULL;
				BoxDrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
				::SelectObject(hDC, hSubBoxPen1);
				for ( k=0; k<BoxResCount; k++ )
				{
					BoxPtr = WndPtr->GetWndResultBoxPtr(k, false);
					if ( NULL == BoxPtr ) { continue; }
					BoxPtr->DrawBoxResult(hDC, BoxDrawParam);
				}
				*/
			}
			
			BoxDrawParam.DrawEditLine = false;
			//BoxDrawParam.ShapeMode = WndPtr->GetWndOuterMaskShapeMode();
			BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;
			BoxDrawParam.DrawEditLine = false;
			if ( WndPtr->GetWndSelected() == false )
			{
				BoxDrawParam.Toward = WndPtr->GetWndToward();
				::SelectObject(hDC, hWndPen);					
			}
			else
			{
				BoxDrawParam.Toward = WndPtr->GetWndToward();
				if ( (DrawParam.ShowEditLine==true) && (WndPtr->GetWndEditabled()==true) && false==WndRoiSelected && false==MaskBoxSelected )
				//if ( (DrawParam.ShowEditLine==true) && (WndPtr->GetWndEditabled()==true) )
				{	BoxDrawParam.DrawEditLine = true;;	}
				::SelectObject(hDC, hSelPen);
				bShowWnd = TRUE;
			}
			BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;

			if ( true == ShowWndIndex )
			{	
				BoxDrawParam.DrawBoxIndex = true;
				BoxDrawParam.strBoxIndex.Format(_T("%d"), WndPtr->GetWndIndex()+1);
			}
			if ( false==BoxDrawParam.DrawEditLine )
			{	WndPtr->DrawWndBoxEdit(hDC, BoxDrawParam);	}
			else
			{
				::SelectObject(hDC, hSelPenW);
				WndPtr->DrawWndBoxEdit(hDC, BoxDrawParam);
				BoxDrawParam.DrawBoxIndex = false;
				::SelectObject(hDC, hSelPen);
				WndPtr->DrawWndBoxEdit(hDC, BoxDrawParam);
			}
			BoxDrawParam.DrawBoxIndex = false;
			if ( WndPtr->GetWndSelected()==true && WndPtr->GetWndExtendBoxUsed()==true )
			{				
				::SelectObject(hDC, hExtendPen);
				WndPtr->DrawWndExtBoxEdit(hDC, BoxDrawParam);				
			}
		}
	}	
	::SelectObject(hDC, hOldPen);
	::SelectObject(hDC, hOldBrush);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::DrawModelTemp(HDC hDC, const TMODEL_DRAW_PARAM &DrawParam)
{
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::DrawModelResult(HDC hDC, const TMODEL_DRAW_PARAM &DrawParam)
{
	int i=0, j=0, k=0;	
	int u=0, v=0, w=0;		
	size_t       WndCount = 0;
	size_t       BoxCount = 0;	
	unsigned int BoxIndex = 0;
	size_t       LandCount= 0;	
	int          ActiveLandType = 0;	
	size_t       ItemCount = 0;
	size_t       AlgCount = 0;
	size_t       BoxResCount = 0;
	size_t       WndRoiCount = 0;
	size_t       MaskWndCount = 0;
	bool   ShowEditLine2 = false;	
	double PenScale = 1;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;	
	TBOX_DRAW_PARAM BoxDrawParam;
	
	double       Scale = DrawParam.Scale;
	double       CornerPtX[4]={0};
	double       CornerPtY[4]={0};
	const double ComponentAngle = CAOIModel::GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);	
	const double ResX = DrawParam.ResolutionX;
	const double ResY = DrawParam.ResolutionY;
	const double ModelCPX = 0;
	const double ModelCPY = 0;
	const int    WndCPX = (DrawParam.WndRect.left+DrawParam.WndRect.right)/2;
	const int    WndCPY = (DrawParam.WndRect.top+DrawParam.WndRect.bottom)/2;	
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const int    LineSizeLevel = SysParam.m_EditLineSizeLevel;
	const bool   ShowWndIndex = AOIDataCollect.GetShowModelWndIndex();
	const bool   ShowLandIndex = AOIDataCollect.GetShowModelLandIndex();
	const bool   ShowModelWndMaskAll = true;

	BOOL         bShowWnd = FALSE;
	BOOL         bShowLand = FALSE;
	
	CAOIWnd      *WndPtr = NULL;	
	CAOILand     *LandPtr = NULL;	
	CAOIBox      *BoxPtr = NULL;		
	CAOIWndRoi   *WndRoiPtr = NULL;	
	CAOIWndMask  *MaskWndPtr = NULL;	

	COLORREF      TxtColorOK=0x00FF00;
	COLORREF      TxtColorNG=0x0000FF;
	COLORREF      TxtColorOld = GetTextColor(hDC);

	BOX_SHAPE_MODE WndShapeMode=BOX_SHAPE_RECTANGLE;
	RESULT_ID      BoxResultID=RESULT_ID_NONE;
	RESULT_ID      WndResultID=RESULT_ID_NONE;

	if ( Scale < 0 ) 
	{
		Scale = CAOIModel::GetFitScale(DrawParam.WndRect);
		Scale = Scale*1.2;		
	}
	const double ZoomX = ResX*Scale;
	const double ZoomY = ResY*Scale;	
	HPEN hOldPen = NULL;
	HPEN hNullPen = CAOIModel::hNullPen;
	HPEN hComPen = CAOIModel::hComPen;
	HPEN hPadPen = CAOIModel::hPadPen;
	HPEN hLeadPen = CAOIModel::hLeadPen;
	HPEN hShoulderPen = CAOIModel::hShoulderPen;
	HPEN hLeadTipPen = CAOIModel::hLeadTipPen;
	HPEN hSelPen = CAOIModel::hSelPenResult;
	HPEN hSelPenW = CAOIModel::hSelPenResultW;
	HPEN hLandPen = CAOIModel::hLandPen;
	HPEN hWndPen = CAOIModel::hWndPen;
	HPEN hNGPen = CAOIModel::hNGPen;
	HPEN hOKPen = CAOIModel::hOKPen;
	HPEN hBoxPen = CAOIModel::hBoxPen;
	HPEN hExtendPen = CAOIModel::hExtendPen;	
	HPEN hSubBoxPen1 = CAOIModel::hSubBoxPen1;
	HPEN hSubBoxPen2 = CAOIModel::hSubBoxPen2;
	HPEN hSubBoxPen3 = CAOIModel::hSubBoxPen3;
	HPEN hSubBoxPen4 = CAOIModel::hSubBoxPen4;
	HPEN hMaskBoxPen1 = CAOIModel::hMaskBoxPen1;
	HPEN hMaskBoxPen2 = CAOIModel::hMaskBoxPen2;
	HPEN hIntervalPen = CAOIModel::hIntervalPen;
	
	hOldPen = (HPEN)::SelectObject(hDC, hComPen);
	PenScale = 1/Scale;

	//BoxWnd List	
	bShowWnd = FALSE;
	bShowLand = FALSE;	
	
	BoxDrawParam.Zoom.x = ZoomX;
	BoxDrawParam.Zoom.y = ZoomY;
	BoxDrawParam.ModelCP.x = ModelCPX;
	BoxDrawParam.ModelCP.y = ModelCPY;
	BoxDrawParam.ViewOffset.x = DrawParam.ViewOffsetX;
	BoxDrawParam.ViewOffset.y = DrawParam.ViewOffsetY;	
	BoxDrawParam.ComponentAngle = ComponentAngle;
	BoxDrawParam.IsExceptionAngle = IsExceptionAngle;
	BoxDrawParam.DrawEditLine = ShowEditLine2;
	BoxDrawParam.LineScale = PenScale;
	BoxDrawParam.WndCP.x = WndCPX;
	BoxDrawParam.WndCP.y = WndCPY;
	BoxDrawParam.WndRect = DrawParam.WndRect;
	BoxDrawParam.ViewCP = DrawParam.ViewCP;
	BoxDrawParam.TowardSize = JetAPI::GetTowardSize(Scale);
	BoxDrawParam.EditLineSize = JetAPI::GetEditLineSize(Scale, LineSizeLevel);
	BoxDrawParam.DrawResultText = false;
	BoxDrawParam.hPenNull = CAOIModel::hNullPen;
	BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrush;

	BoxPtr = GetModelBodyBoxPtr();
	if ( BoxPtr->GetBoxVisibled() == true )
	{
		if ( BoxPtr->GetBoxSelected() == true )
		{	::SelectObject(hDC, hSelPen);	}
		else
		{	::SelectObject(hDC, hComPen); }
	
		BoxDrawParam.Toward = BoxPtr->GetBoxToward();
		BoxDrawParam.DrawEditLine = false;		
		BoxDrawParam.Extend.x = 0;
		BoxDrawParam.Extend.y = 0;
		if ( BoxPtr->GetBoxSelected() == false )
		{	BoxPtr->DrawBoxResult(hDC, BoxDrawParam);	}
		else
		{
			::SelectObject(hDC, hSelPenW);
			BoxPtr->DrawBoxResult(hDC, BoxDrawParam);
			::SelectObject(hDC, hSelPen);
			BoxPtr->DrawBoxResult(hDC, BoxDrawParam);
		}
	}
	
	//Draw Land Box
	//if ( (bShowBoxWnd==FALSE) || (bShowLandBoxWnd==TRUE) )
	if ( true == DrawParam.ShowLandBox )
	{
		bool bShowenLandIndex = false;
		BoxDrawParam.DrawBoxIndex = false;
		BoxDrawParam.strBoxIndex = _T("");
		BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;
		LandCount = GetModelLandCount();
		for ( j=0; j<LandCount; j++ )
		{					
			LandPtr = GetModelLandPtr(j, false);
			if ( LandPtr == NULL ) { continue; }
			//continue;
			//Draw Inactive Box
			bShowenLandIndex = false;
			::SelectObject(hDC, hLandPen);				
			for ( k=0; k<5; k++ )
			{
				switch ( k )
				{
				case 0:	
					::SelectObject(hDC, hPadPen);
					BoxPtr = LandPtr->GetLandPadBoxPtr();	
					break;
				case 1:	
					::SelectObject(hDC, hLeadPen);
					BoxPtr = LandPtr->GetLandLeadBoxPtr();	
					break;
				case 2:	
					::SelectObject(hDC, hLeadTipPen);
					BoxPtr = LandPtr->GetLandLeadTipBoxPtr();	
					break;
				case 3:	
					::SelectObject(hDC, hShoulderPen);
					BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();	
					break;
				case 4:
					::SelectObject(hDC, hComPen);
					BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();	
					break;
				default:
					BoxPtr = NULL;
					::SelectObject(hDC, hLandPen);
					break;
				}								
				if ( BoxPtr == NULL ) { continue; }
				if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
				if ( BoxPtr->GetBoxVisibled() == false ) { continue; }
				if ( BoxPtr->GetBoxSelected() == true ) { continue; }

				if ( true == ShowLandIndex )
				{	
					if ( false == bShowenLandIndex )
					{
						BoxDrawParam.DrawBoxIndex = true;
						BoxDrawParam.strBoxIndex.Format(_T("%d"), LandPtr->GetLandIndex()+1);
						bShowenLandIndex = true;
					}
				}
				BoxDrawParam.Toward = BOX_TOWARD_NULL; 					
				BoxDrawParam.DrawEditLine = false;
				BoxPtr->DrawBoxResult(hDC, BoxDrawParam);
				BoxDrawParam.DrawBoxIndex = false;
			}

			//Draw Selected Box			
			for ( k=0; k<5; k++ )
			{
				switch ( k )
				{
				case 0:	BoxPtr = LandPtr->GetLandPadBoxPtr();	break;
				case 1:	BoxPtr = LandPtr->GetLandLeadBoxPtr();	break;
				case 2:	BoxPtr = LandPtr->GetLandLeadTipBoxPtr();	break;
				case 3:	BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();	break;
				case 4:	BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();	break;
				default: BoxPtr = NULL;	break;
				}			
				if ( BoxPtr == NULL ) { continue; }
				if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
				if ( BoxPtr->GetBoxVisibled() == false ) { continue; }
				if ( BoxPtr->GetBoxSelected() == false ) { continue; }
				
				if ( true == ShowLandIndex )
				{	
					if ( false == bShowenLandIndex )
					{
						BoxDrawParam.DrawBoxIndex = true;
						BoxDrawParam.strBoxIndex.Format(_T("%d"), LandPtr->GetLandIndex()+1);
						bShowenLandIndex = true;
					}
				}
				bShowLand = TRUE;
				BoxDrawParam.Toward = LandPtr->GetLandToward();
				BoxDrawParam.DrawEditLine = false;
				::SelectObject(hDC, hSelPenW);
				BoxPtr->DrawBoxResult(hDC, BoxDrawParam);
				BoxDrawParam.DrawBoxIndex = false;
				::SelectObject(hDC, hSelPen);
				BoxPtr->DrawBoxResult(hDC, BoxDrawParam);
			}
		}
	}		
	
	if ( true == DrawParam.ShowWndBox )
	{
		BoxDrawParam.DrawBoxIndex = false;
		BoxDrawParam.strBoxIndex = _T("");
		BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;
		WndCount = GetModelWndCount();
		for ( j=0; j<WndCount; j++ )
		{					
			WndPtr = GetModelWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) 
			{ 
				if ( WndPtr->GetWndSelected() == false )
				{	continue;  }
			}					
			
			if ( WndPtr->GetWndVisibled() == false ) { continue; }
			
			BoxDrawParam.DrawEditLine = false;
			//BoxDrawParam.ShapeMode = WndPtr->GetWndOuterMaskShapeMode();
			BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;

			if ( WndPtr->GetWndSelected() == false )
			{
				WndResultID = WndPtr->GetWndResultID();
				switch ( WndResultID )
				{
				case RESULT_ID_NG: ::SelectObject(hDC, hNGPen); break;
				case RESULT_ID_OK: ::SelectObject(hDC, hOKPen); break;
				default:           ::SelectObject(hDC, hWndPen); break;
				}			
			}
			else
			{
				::SelectObject(hDC, hSelPen);
				bShowWnd = TRUE;
			}
			BoxDrawParam.DrawEditLine = false;
			BoxDrawParam.Toward = WndPtr->GetWndToward();				
			BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;

			if ( true == ShowWndIndex )
			{	
				BoxDrawParam.DrawBoxIndex = true;
				BoxDrawParam.strBoxIndex.Format(_T("%d"), WndPtr->GetWndIndex()+1);
			}
			if ( WndPtr->GetWndSelected() == false )
			{	WndPtr->DrawWndBoxResult(hDC, BoxDrawParam); }
			else
			{
				::SelectObject(hDC, hSelPenW);
				WndPtr->DrawWndBoxResult(hDC, BoxDrawParam);
				BoxDrawParam.DrawBoxIndex = false;
				::SelectObject(hDC, hSelPen);
				WndPtr->DrawWndBoxResult(hDC, BoxDrawParam);
			}
			BoxDrawParam.DrawBoxIndex = false;
			//if ( WndPtr->GetWndInnerMaskEnabled() == true )
			//{	BoxDrawParam.Toward = BOX_TOWARD_NULL;	}

			if ( WndPtr->GetWndSelected()==true && WndPtr->GetWndExtendBoxUsed()==true )
			{
				::SelectObject(hDC, hExtendPen);
				WndPtr->DrawWndExtBoxResult(hDC, BoxDrawParam);				
			}
			
			if ( WndPtr->GetWndSelected()==true || true==ShowModelWndMaskAll )
			{				
				/*
				//繪製子框
				if ( WndPtr->GetWndSelected() == true )
				{
					WndRoiCount = WndPtr->GetWndRoiWndCount();
					BoxDrawParam.Toward = BOX_TOWARD_NULL;
					::SelectObject(hDC, hSubBoxPen1);
					for ( k=0; k<WndRoiCount; k++ )
					{
						WndRoiPtr = WndPtr->GetWndRoiWndPtr(k, false);
						if ( WndRoiPtr == NULL ) { continue; }
						WndRoiPtr->DrawWndRoiBoxResult(hDC, BoxDrawParam);
					}
				}
				*/

				//繪製遮罩框
				BoxDrawParam.Toward = BOX_TOWARD_NULL;
				MaskWndCount = WndPtr->GetWndMaskWndCount();				
				BoxDrawParam.FillRegion = true;
				BoxDrawParam.DrawTowardFeature = false;					
				for ( k=0; k<MaskWndCount; k++ )
				{
					MaskWndPtr = WndPtr->GetWndMaskWndPtr(k, false);
					if ( MaskWndPtr == NULL ) { continue; }
					if ( MaskWndPtr->GetWndMaskEraseMode() == false ) 
					{
						::SelectObject(hDC, hMaskBoxPen1);
						BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrush;	
					}
					else
					{
						::SelectObject(hDC, hMaskBoxPen2);
						BoxDrawParam.hBrushMask = CAOIModel::hMaskBoxBrushErase;	
					}
					BoxDrawParam.Toward = MaskWndPtr->GetWndMaskToward();
					BoxDrawParam.ShapeMode = MaskWndPtr->GetWndMaskShapeMode();
					MaskWndPtr->DrawWndMaskResult(hDC, BoxDrawParam);
				}
				BoxDrawParam.FillRegion = false;
				BoxDrawParam.DrawTowardFeature = true;

				//繪製結果框
				BoxResCount = WndPtr->GetWndResultBoxCount();
				BoxDrawParam.Toward = BOX_TOWARD_NULL;
				BoxDrawParam.ShapeMode = BOX_SHAPE_RECTANGLE;
				BoxDrawParam.DrawResultText = true;
				for ( k=0; k<BoxResCount; k++ )
				{
					BoxPtr = WndPtr->GetWndResultBoxPtr(k, false);
					if ( NULL == BoxPtr ) { continue; }
					BoxResultID = BoxPtr->GetBoxResultID();
					switch ( BoxResultID )
					{
					case RESULT_ID_NG: 
						::SelectObject(hDC, hNGPen); 
						::SetTextColor(hDC, TxtColorNG);
						break;
					case RESULT_ID_OK: 
						::SelectObject(hDC, hOKPen); 
						::SetTextColor(hDC, TxtColorOK);
						break;
					default:           
						::SelectObject(hDC, hSubBoxPen1); 
						::SetTextColor(hDC, TxtColorOld);
						break;
					}					
					BoxPtr->DrawBoxResult(hDC, BoxDrawParam);					
				}
				BoxDrawParam.DrawResultText = false;
				::SetTextColor(hDC, TxtColorOld);
			}
		}
	}	
	::SelectObject(hDC, hOldPen);	
	::SetTextColor(hDC, TxtColorOld);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CopyModelProperty(const CAOIModel *RefModelPtr)
{
	if ( NULL == RefModelPtr ) { return false; }	
	int          j=0;
	size_t       i=0;
	int          AngleLabel=0;
	CAOIModel   *ModelPtr = this;
	CAOILand    *LandPtr = NULL;	
	int          LandGroupID=0;
	int          RefLandGroupID=0;		
	const size_t LandCount = ModelPtr->GetModelLandCount();
	const size_t RefLandCount = RefModelPtr->GetModelLandCount();
	BOX_TOWARD   BodyToward = ModelPtr->GetModelBodyBox().GetBoxToward();
	BOX_TOWARD   RefBodyToward = RefModelPtr->GetModelBodyBox().GetBoxToward();
	const double ModelAngle=ModelPtr->GetModelAttachedAngle();
	const double RefModelAngle=RefModelPtr->GetModelAttachedAngle();
	const int    MaxLandGroupID = ModelPtr->GetModelLandFreeGroupID();
	const int    RefMaxLandGroupID = RefModelPtr->GetModelLandFreeGroupID();

	double ExtendX = RefModelPtr->GetModelExtendRangeX();
	double ExtendY = RefModelPtr->GetModelExtendRangeY();
	bool   ExtendAutoAdjust = RefModelPtr->GetModelExtendAutoAdjust();
	double BodySizeX  = RefModelPtr->GetModelBodySizeX();
	double BodySizeY  = RefModelPtr->GetModelBodySizeY();
	double BodyHeight = RefModelPtr->GetModelBodyHeight();	

	AngleLabel = CAOIBox::CalcBoxTowardAngle(BodyToward, RefBodyToward);
	JetAPI::RotateSize(AngleLabel, ExtendX, ExtendY);
	JetAPI::RotateSize(AngleLabel, BodySizeX, BodySizeY);

	ModelPtr->SetModelModifiedCount(true);
	ModelPtr->SetModelExtendRangeX(ExtendX);
	ModelPtr->SetModelExtendRangeY(ExtendY);
	ModelPtr->SetModelExtendAutoAdjust(ExtendAutoAdjust);

	ModelPtr->SetModelBodySizeX(BodySizeX);
	ModelPtr->SetModelBodySizeY(BodySizeY);
	ModelPtr->SetModelBodyHeight(BodyHeight);

	for ( j=0; j<MaxLandGroupID; j++ )
	{
		LandGroupID = j;
		for ( i=0; i<RefLandCount; i++ )
		{
			LandPtr = RefModelPtr->GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			break;
		}
		//find nothing
		if ( i == RefLandCount ) { continue; }		
		
		BOX_TOWARD    LandToward;
		TLandProperty LandProperty;		
		TLandProperty RefLandProperty = LandPtr->GetLandProperty();
		
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandProperty = RefLandProperty;
			LandToward = LandPtr->GetLandToward();
			
			AngleLabel = CAOIBox::CalcBoxTowardAngle(LandToward, RefLandProperty.eLandToward);
			JetAPI::RotateSize(AngleLabel, LandProperty.dLeadSizeX, LandProperty.dLeadSizeY);
			JetAPI::RotateSize(AngleLabel, LandProperty.dLeadTipSizeX, LandProperty.dLeadTipSizeY);
			JetAPI::RotateSize(AngleLabel, LandProperty.dLeadShoulderSizeX, LandProperty.dLeadShoulderSizeY);
			LandPtr->SetLandProperty(LandProperty);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelBodyBoxActived(bool val)
{
	m_ModelBodyBox.SetBoxActived(val);
	if ( true == val ) 
	{			
		m_ModelBodyBox.SetBoxSelected(true); 
		m_ModelBodyBox.SetBoxVisibled(true);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelBodyRegion(const TREGION4D &Region)
{
	m_ModelBodyBox.SetBoxRegion(Region);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelBodyRegion(TREGION4D &Region)
{
	m_ModelBodyBox.GetBoxRegion(Region);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::GetModelBodyPos(TPOINT2D &Pos)
{	
	m_ModelBodyBox.GetBoxPos(Pos);	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::CalcModelBodyRegionNoLead(TREGION4D &Region)
{
	size_t       i=0;
	TREGION4D    LeadRgn;
	BOX_TOWARD   LandToward;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount();

	GetModelBodyRegion(Region);
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandLeadBox().GetBoxEnabled() == false ) { continue; }
		LandToward = LandPtr->GetLandToward();
		LandPtr->GetLandLeadBox().GetBoxRegion(LeadRgn);
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:
			if ( Region.maxY > LeadRgn.minY ) 
			{	Region.maxY = LeadRgn.minY; }
			break;
		case BOX_TOWARD_LEFT:
			if ( Region.minX < LeadRgn.maxX ) 
			{	Region.minX = LeadRgn.maxX; }
			break;
		case BOX_TOWARD_DOWN:
			if ( Region.minY < LeadRgn.maxY ) 
			{	Region.minY = LeadRgn.maxY; }
			break;
		case BOX_TOWARD_RIGHT:
			if ( Region.maxX > LeadRgn.minX ) 
			{	Region.maxX = LeadRgn.minX; }
			break;
		}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ModifyModelBodyPos(double x, double y)
{
	double PosX=0, PosY=0;
	double dPosX=0, dPosY=0;
	const double ComponentAngle =  CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if ( IsExceptionAngle == false )
	{	
		CAOIModel::m_ModelBodyBox.GetBoxPos(PosX, PosY);
		dPosX = x-PosX;
		dPosY = y-PosY;
		CAOIModel::MoveModel(dPosX, dPosY);
	}
	else
	{
		CAOIModel::RotateModel(-ComponentAngle, 0, 0);
		CAOIModel::m_ModelBodyBox.GetBoxPos(PosX, PosY);
		dPosX = x-PosX;
		dPosY = y-PosY;
		CAOIModel::MoveModel(dPosX, dPosY);
		CAOIModel::RotateModel(ComponentAngle, 0, 0);		
	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ModifyModelBodySize(double w, double h)
{
	const double ComponentAngle =  CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if ( IsExceptionAngle == false )
	{	
		CAOIModel::m_ModelBodyBox.SetBoxSize(w, h);		
	}
	else
	{
		CAOIModel::RotateModel(-ComponentAngle, 0, 0);
		CAOIModel::m_ModelBodyBox.SetBoxSize(w, h);		
		CAOIModel::RotateModel(ComponentAngle, 0, 0);		
	}	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelBodyRegion(double dMinX, double dMinY, double dMaxX, double dMaxY)
{
	bool IsOK = true;
	CAOIBox *BoxPtr = CAOIModel::GetModelBodyBoxPtr();
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( false == IsExceptionAngle )
	{	BoxPtr->ModifyBoxRegion(dMinX, dMinY, dMaxX, dMaxY);		}
	else
	{
		BoxPtr->RotateBox(-ComAngle, 0, 0);
		BoxPtr->ModifyBoxRegion(dMinX, dMinY, dMaxX, dMaxY);	
		BoxPtr->RotateBox(ComAngle, 0, 0);
	}

	UpdateModelChipLeadRgnFromBody();
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelAllObjList()
{
	ClearModelWndList();
	ClearModelLandList();
	ClearModelLogicList();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelAllWndList()
{
	size_t       i = 0;
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{	
		LandPtr = GetModelLandPtr(i, false);
		if ( LandPtr == NULL ) { continue; }
		LandPtr->RemoveLandWndList();
		LandPtr->RemoveLandLogicList();		
	}		
	ClearModelWndList();
	ClearModelLogicList();
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelAllDerivative()//刪除模組衍生物-僅保留基本項目
{
	CAOIModel *ModelPtr = this;
	if ( NULL == ModelPtr ) { return; }

	size_t       i=0;
	int          ii=0;	
	CAOILand    *LandPtr = NULL;	
	std::vector<CAOILand*> KeepLandList;
	const size_t LandCount = ModelPtr->GetModelLandCount();
	const int    LandGroupID = ModelPtr->GetModelLandFreeGroupID();	

	for ( ii=0; ii<LandGroupID; ii++ )
	{
		LandPtr = ModelPtr->GetModelLandPtrByGroupID(ii, -1);
		if ( NULL == LandPtr ) { continue; }
		KeepLandList.push_back(LandPtr);
	}

	const size_t KeepLandCount = KeepLandList.size();
	if ( KeepLandCount > 0 ) 
	{
		ModelPtr->SetModelLandSelected(true);
		for ( i=0; i<KeepLandCount; i++ )
		{	
			LandPtr = KeepLandList[i];
			LandPtr->SetLandAllBoxSelected(false);
		}
		ModelPtr->DeleteModelLandSelected();		
	}

	WND_DEFECT_ID WndDefectID;
	CAOIWnd      *WndPtr = NULL;
	std::vector<CAOIWnd*> KeepWndList;
	const size_t WndCount = ModelPtr->GetModelWndCount();
	const int    WndGroupID = ModelPtr->GetModelWndFreeGroupID();

	//先放入特徵框上的檢測框
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		if ( WND_DEFECT_SOLDER_BRIDGE==WndDefectID || WND_DEFECT_SOLDER_BEAD==WndDefectID )
		{	continue; }
		KeepWndList.push_back(WndPtr);
	}
	//再放入本體上的檢測框
	for ( ii=0; ii<WndGroupID; ii++ )
	{
		WndPtr = ModelPtr->GetModelWndPtrByGroupID(ii, -1, false);
		if ( NULL == WndPtr ) { continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr ) 
		{	continue;	}
		KeepWndList.push_back(WndPtr);
	}

	const size_t KeepWndCount = KeepWndList.size();
	if ( KeepWndCount > 0 ) 
	{
		ModelPtr->SetModelWndSelected(true);
		for ( i=0; i<KeepWndCount; i++ )
		{	
			WndPtr = KeepWndList[i];
			WndPtr->SetWndSelected(false);
		}
		ModelPtr->DeleteModelWndSelected(false);
	}

	UpdateModelBodyRgnFromChipLead();
	UpdateModelWndRgnByLinkMode();
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelWndRoiWnd(CAOIWnd *WndPtr)//新增模組檢測框內的子框
{
	bool IsOK = true;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( false == IsExceptionAngle ) 
	{	IsOK = AddModelWndRoiWndKernel(WndPtr);	}
	else
	{
		RotateModel(-AttachedAngle, 0, 0);
		IsOK = AddModelWndRoiWndKernel(WndPtr);
		RotateModel(AttachedAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelWndRoiWndKernel(CAOIWnd *RefWndPtr)//新增模組檢測框內的子框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;
	double              WndCpX = 0;
	double              WndCpY = 0;
	double              WndSizeW = 0;
	double              WndSizeH = 0;
	double              WndRoiSizeW = 0;
	double              WndRoiSizeH = 0;
	double              WndRoiSizeW2 = 0;
	double              WndRoiSizeH2 = 0;	
	TREGION4D           WndRgn;
	TREGION4D           WndRoiRgn;
	CAOIWnd            *WndPtr = NULL;	
	CAOIWndRoi         *WndRoiPtr = NULL;	
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		

		WndPtr->GetWndRegion(WndRgn);
		WndCpX = WndRgn.GetCpX();
		WndCpY = WndRgn.GetCpY();
		WndSizeW = WndRgn.GetWidth();
		WndSizeH = WndRgn.GetHeight();
		WndRoiSizeW = WndSizeW/2;
		WndRoiSizeH = WndSizeH/2;
		WndRoiSizeW2 = WndRoiSizeW/2;
		WndRoiSizeH2 = WndRoiSizeH/2;
		WndRoiRgn.minX = WndCpX-(WndRoiSizeW2);
		WndRoiRgn.minY = WndCpY-(WndRoiSizeH2);
		WndRoiRgn.maxX = WndCpX+(WndRoiSizeW2);
		WndRoiRgn.maxY = WndCpY+(WndRoiSizeH2);	
	
		WndRoiPtr = AOIObjManager.CreateWndRoiObj();
		if ( NULL == WndRoiPtr )
		{		
			JetAPI::ShowMessageBox(_T("Error, Create Wnd Roi Obj Fault"));
			return false;
		}
		WndRoiPtr->SetWndRoiSelected(true);				
		WndRoiPtr->SetWndRoiRegion(WndRoiRgn);
		WndRoiPtr->SetWndRoiToward(WndPtr->GetWndToward());

		WndPtr->SetWndAllRoiWndActived(false);
		WndPtr->SetWndAllRoiWndSelected(false);
		WndPtr->AddWndRoiWndPtr(WndRoiPtr, false);
		WndPtr->SetWndModified(true);
		WndPtr->SetWndRoiWndActived(WndRoiPtr);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelWndRoiWnd(CAOIWnd *WndPtr, const TREGION4D &RoiRgn)//新增模組檢測框內的子框
{
	bool IsOK = true;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( false == IsExceptionAngle ) 
	{	IsOK = AddModelWndRoiWndKernel(WndPtr, RoiRgn);	}
	else
	{
		RotateModel(-AttachedAngle, 0, 0);
		IsOK = AddModelWndRoiWndKernel(WndPtr, RoiRgn);
		RotateModel(AttachedAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelWndRoiWndKernel(CAOIWnd *RefWndPtr, const TREGION4D &RoiRgn)//新增模組檢測框內的子框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;
	double              WndCpX = 0;
	double              WndCpY = 0;
	double              WndSizeW = 0;
	double              WndSizeH = 0;
	double              WndRoiSizeW = 0;
	double              WndRoiSizeH = 0;
	double              WndRoiSizeW2 = 0;
	double              WndRoiSizeH2 = 0;	
	TREGION4D           WndRgn;
	TREGION4D           WndRoiRgn;
	CAOIWnd            *WndPtr = NULL;	
	CAOIWndRoi         *WndRoiPtr = NULL;	
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		

		WndPtr->GetWndRegion(WndRgn);
		WndCpX = WndRgn.GetCpX();
		WndCpY = WndRgn.GetCpY();
		WndSizeW = WndRgn.GetWidth();
		WndSizeH = WndRgn.GetHeight();
		WndRoiSizeW = WndSizeW/2;
		WndRoiSizeH = WndSizeH/2;
		WndRoiSizeW2 = WndRoiSizeW/2;
		WndRoiSizeH2 = WndRoiSizeH/2;
		WndRoiRgn.minX = WndCpX-(WndRoiSizeW2);
		WndRoiRgn.minY = WndCpY-(WndRoiSizeH2);
		WndRoiRgn.maxX = WndCpX+(WndRoiSizeW2);
		WndRoiRgn.maxY = WndCpY+(WndRoiSizeH2);	
		WndRoiRgn = RoiRgn;

		if ( WndRoiRgn.minX < WndRgn.minX ) { WndRoiRgn.minX = WndRgn.minX; }
		if ( WndRoiRgn.minY < WndRgn.minY ) { WndRoiRgn.minY = WndRgn.minY; }
		if ( WndRoiRgn.maxX > WndRgn.maxX ) { WndRoiRgn.maxX = WndRgn.maxX; }
		if ( WndRoiRgn.maxY > WndRgn.maxY ) { WndRoiRgn.maxY = WndRgn.maxY; }	
	
		WndRoiPtr = AOIObjManager.CreateWndRoiObj();
		if ( NULL == WndRoiPtr )
		{		
			JetAPI::ShowMessageBox(_T("Error, Create Wnd Roi Obj Fault"));
			return false;
		}
		WndRoiPtr->SetWndRoiSelected(true);				
		WndRoiPtr->SetWndRoiRegion(WndRoiRgn);
		WndRoiPtr->SetWndRoiToward(WndPtr->GetWndToward());

		WndPtr->SetWndAllRoiWndActived(false);
		WndPtr->SetWndAllRoiWndSelected(false);
		WndPtr->AddWndRoiWndPtr(WndRoiPtr, false);
		WndPtr->SetWndModified(true);
		WndPtr->SetWndRoiWndActived(WndRoiPtr);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelWndRoiWnd(CAOIWnd *RefWndPtr)//刪除模組檢測框內的子框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;
	size_t              WndRoiCount=0;
	CAOIWnd            *WndPtr = NULL;
	CAOIWndRoi         *WndRoiPtr = NULL;
	std::vector<size_t> WndRoiIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();	
	
	WndRoiIndexList.clear();
	WndRoiCount = RefWndPtr->GetWndRoiWndCount();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = RefWndPtr->GetWndRoiWndPtr(i, false);
		if ( NULL == WndRoiPtr ) { continue; }
		if ( WndRoiPtr->GetWndRoiSelected() == false ) { continue; }
		WndRoiIndexList.push_back(i);
	}
	const size_t WndRoiIndexCount = WndRoiIndexList.size();
	if ( 0 == WndRoiIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndPtr->SetWndAllRoiWndSelected(false);
		for ( j=0; j<WndRoiIndexCount; j++ )
		{
			Index = WndRoiIndexList[j];
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(Index, true);
			if ( NULL == WndRoiPtr ) { continue; }
			WndRoiPtr->SetWndRoiSelected(true);
		}		
		WndPtr->DestroyWndRoiWndSelected();
		WndPtr->SetWndModified(true);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ClearModelWndRoiWndList(CAOIWnd *RefWndPtr)//清除模組檢測框內的子框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	CAOIWnd            *WndPtr = NULL;	
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		
		WndPtr->ClearWndRoiWndList();
		WndPtr->SetWndModified(true);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelWndMaskWnd(CAOIWnd *WndPtr)//新增模組檢測框內的遮罩框
{
	bool IsOK = true;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( false == IsExceptionAngle ) 
	{	IsOK = AddModelWndMaskWndKernel(WndPtr);	}
	else
	{
		RotateModel(-AttachedAngle, 0, 0);
		IsOK = AddModelWndMaskWndKernel(WndPtr);
		RotateModel(AttachedAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelWndMaskWndKernel(CAOIWnd *RefWndPtr)//新增模組檢測框內的遮罩框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;
	double              WndCpX = 0;
	double              WndCpY = 0;
	double              WndSizeW = 0;
	double              WndSizeH = 0;
	double              WndRoiSizeW = 0;
	double              WndRoiSizeH = 0;
	double              WndRoiSizeW2 = 0;
	double              WndRoiSizeH2 = 0;		
	TREGION4D           WndRgn;
	TREGION4D           MaskWndRgn;
	CAOIWnd            *WndPtr = NULL;	
	CAOIWndMask        *MaskWndPtr = NULL;	
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		

		WndPtr->GetWndRegion(WndRgn);
		WndCpX = WndRgn.GetCpX();
		WndCpY = WndRgn.GetCpY();
		WndSizeW = WndRgn.GetWidth();
		WndSizeH = WndRgn.GetHeight();
		WndRoiSizeW = WndSizeW/2;
		WndRoiSizeH = WndSizeH/2;
		WndRoiSizeW2 = WndRoiSizeW/2;
		WndRoiSizeH2 = WndRoiSizeH/2;
		MaskWndRgn.minX = WndCpX-(WndRoiSizeW2);
		MaskWndRgn.minY = WndCpY-(WndRoiSizeH2);
		MaskWndRgn.maxX = WndCpX+(WndRoiSizeW2);
		MaskWndRgn.maxY = WndCpY+(WndRoiSizeH2);	
	
		MaskWndPtr = AOIObjManager.CreateWndMaskObj();
		if ( NULL == MaskWndPtr )
		{		
			JetAPI::ShowMessageBox(_T("Error, Create Wnd Mask Wnd Obj Fault"));
			return false;
		}
		MaskWndPtr->SetWndMaskSelected(true);
		MaskWndPtr->SetWndMaskRegion(MaskWndRgn);
		MaskWndPtr->SetWndMaskToward(WndPtr->GetWndToward());
		MaskWndPtr->SetWndMaskShapeMode(WndPtr->GetWndShapeMode());
		WndPtr->SetWndAllMaskWndSelected(false);		
		WndPtr->AddWndMaskWndPtr(MaskWndPtr, false);
		WndPtr->SetWndModified(true);
		WndPtr->SetWndMaskWndActived(MaskWndPtr);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SpinModelWndMaskWndSelected(CAOIWnd *RefWndPtr, double Angle)//自轉模組檢測框內的遮罩框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();	
	
	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndPtr->SetWndModified(true);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }			
			MaskWndPtr->SpinWndMask(Angle);
		}	
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RotateModelWndMaskWndSelected(CAOIWnd *RefWndPtr, double Angle)//旋轉模組檢測框內的遮罩框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	double              CpX=0, CpY=0;
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();	
	
	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndPtr->SetWndModified(true);
		WndPtr->GetWndBox().GetBoxPos(CpX, CpY);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }			
			MaskWndPtr->RotateWndMask(Angle, CpX, CpY);
		}	
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorXModelWndMaskWndSelected(CAOIWnd *RefWndPtr)//鏡射模組檢測框內的遮罩框	
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	double              CpX=0, CpY=0;
	TREGION4D           WndRgn;
	TREGION4D           MaskWndRgn;	
	TPOINT2D            WndCornerPt[4];
	TPOINT2D            MaskWndCornerPt[4];
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	BOX_TOWARD RefWndToward = RefWndPtr->GetWndToward();
	const size_t WndCount = GetModelWndCount();	
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		
		if ( false == IsExceptionAngle )
		{	WndPtr->GetWndRegion(WndRgn);	}
		else
		{
			WndPtr->GetWndCornerPos(WndCornerPt);
			JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, WndCornerPt);
			JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);			
		}
		CpX = WndRgn.GetCpX();
		CpY = WndRgn.GetCpY();
		WndPtr->SetWndModified(true);		
		BOX_TOWARD WndToward = WndPtr->GetWndToward();
		const int TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }			
			if ( true == IsExceptionAngle )			
			{	MaskWndPtr->RotateWndMask(-AttachedAngle, 0, 0);	}		
			switch ( TowardAngle )
			{
			case 90:
			case 270:	MaskWndPtr->MirrorWndMaskYAxis(CpX);	break;
			default:	MaskWndPtr->MirrorWndMaskXAxis(CpY);	break;
			}
			if ( true == IsExceptionAngle )			
			{	MaskWndPtr->RotateWndMask(AttachedAngle, 0, 0);	}	
		}	
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MirrorYModelWndMaskWndSelected(CAOIWnd *RefWndPtr)//鏡射模組檢測框內的遮罩框	
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	double              CpX=0, CpY=0;
	TREGION4D           WndRgn;
	TREGION4D           MaskWndRgn;	
	TPOINT2D            WndCornerPt[4];
	TPOINT2D            MaskWndCornerPt[4];
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	BOX_TOWARD RefWndToward = RefWndPtr->GetWndToward();
	const size_t WndCount = GetModelWndCount();	
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		if ( false == IsExceptionAngle )
		{	WndPtr->GetWndRegion(WndRgn);	}
		else
		{
			WndPtr->GetWndCornerPos(WndCornerPt);
			JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, WndCornerPt);
			JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);			
		}
		CpX = WndRgn.GetCpX();
		CpY = WndRgn.GetCpY();
		WndPtr->SetWndModified(true);		
		BOX_TOWARD WndToward = WndPtr->GetWndToward();
		const int TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }
			MaskWndPtr->GetWndMaskRegion(MaskWndRgn);			
			if ( true == IsExceptionAngle )			
			{	MaskWndPtr->RotateWndMask(-AttachedAngle, 0, 0);	}
			switch ( TowardAngle )
			{
			case 90:
			case 270:	MaskWndPtr->MirrorWndMaskXAxis(CpY);	break;
			default:	MaskWndPtr->MirrorWndMaskYAxis(CpX);	break;
			}
			if ( true == IsExceptionAngle )			
			{	MaskWndPtr->RotateWndMask(AttachedAngle, 0, 0);	}			
		}	
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::MoveModelWndMaskWndSelected(CAOIWnd *RefWndPtr, const TPOINT2D &Move)//移動模組檢測框內的遮罩框	
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	TREGION4D           WndRgn;
	TREGION4D           MaskWndRgn;	
	TPOINT2D            WndCornerPt[4];
	TPOINT2D            MaskWndCornerPt[4];
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	BOX_TOWARD RefWndToward = RefWndPtr->GetWndToward();
	const size_t WndCount = GetModelWndCount();	
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		if ( false == IsExceptionAngle )
		{	WndPtr->GetWndRegion(WndRgn);	}
		else
		{
			WndPtr->GetWndCornerPos(WndCornerPt);
			JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, WndCornerPt);
			JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);			
		}			

		TPOINT2D MovePt = Move;
		BOX_TOWARD WndToward = WndPtr->GetWndToward();
		const int TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
		JetAPI::RotatePos(TowardAngle, 0, 0, MovePt);		
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }
			if ( false == IsExceptionAngle )
			{	MaskWndPtr->GetWndMaskRegion(MaskWndRgn);	}
			else
			{				
				MaskWndPtr->GetWndMaskCornerPos(MaskWndCornerPt);
				JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, MaskWndCornerPt);
				JetAPI::CornerPtToRegion(MaskWndCornerPt, MaskWndRgn);
			}			
			MaskWndRgn.Move(MovePt.x, MovePt.y);
			if ( MaskWndRgn.minX < WndRgn.minX ||
				 MaskWndRgn.minY < WndRgn.minY ||
				 MaskWndRgn.maxX > WndRgn.maxX ||
				 MaskWndRgn.maxY > WndRgn.maxY 
				 )
			{	continue; }
			WndPtr->SetWndModified(true);
			MaskWndPtr->MoveWndMask(MovePt);
		}					
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ClonePasteModelWndMaskWnd(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr)//複製貼上模組檢測框內的遮罩框
{
	bool IsOK = true;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( false == IsExceptionAngle ) 
	{	IsOK = ClonePasteModelWndMaskWndKernel(WndPtr, WndMaskPtr);	}
	else
	{
		RotateModel(-AttachedAngle, 0, 0);
		IsOK = ClonePasteModelWndMaskWndKernel(WndPtr, WndMaskPtr);
		RotateModel(AttachedAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ClonePasteModelWndMaskWndKernel(CAOIWnd *RefWndPtr, CAOIWndMask *RefWndMaskPtr)//複製貼上模組檢測框內的遮罩框
{
	if ( NULL == RefWndPtr ) { return false;}		
	if ( NULL == RefWndMaskPtr ) { return false;}
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }
	if ( RefWndPtr->CheckWndMaskValid(RefWndMaskPtr) == false ) { return false; }
	CAOIWndMask *TempMaskPtr = RefWndMaskPtr->CloneWndMaskObj();
	if ( NULL == TempMaskPtr ) { return false;}		

	TREGION4D WndRgn;
	CAOIWnd *WndPtr = NULL;	
	const size_t WndCount = GetModelWndCount();	
	const int WndGorupID = RefWndPtr->GetWndGroupID();
	BOX_TOWARD RefWndToward = RefWndPtr->GetWndToward();
	RefWndPtr->GetWndRegion(WndRgn);
	const double RefWndRgnCpX = WndRgn.GetCpX();
	const double RefWndRgnCpY = WndRgn.GetCpY();

	TempMaskPtr->SetWndMaskActived(false);
	TempMaskPtr->SetWndMaskSelected(false);		
	for ( size_t i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGorupID ) { continue; }
		BOX_TOWARD WndToward = WndPtr->GetWndToward();
		const int Angle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
		CAOIWndMask *MaskWndPtr = TempMaskPtr->CloneWndMaskObj();
		if ( NULL == MaskWndPtr ) { continue; }		
		TPOINT2D WndPos, MovePos;
		WndPtr->GetWndBox().GetBoxPos(WndPos);
		MaskWndPtr->RotateWndMask(Angle, RefWndRgnCpX, RefWndRgnCpY);
		MovePos.x = WndPos.x-RefWndRgnCpX;
		MovePos.y = WndPos.y-RefWndRgnCpY;
		MaskWndPtr->MoveWndMask(MovePos);		

		WndPtr->SetWndAllMaskWndSelected(false);		
		WndPtr->AddWndMaskWndPtr(MaskWndPtr, false);
		WndPtr->SetWndModified(true);
		if ( WndPtr == RefWndPtr )
		{
			MaskWndPtr->SetWndMaskActived(true);
			MaskWndPtr->SetWndMaskSelected(true);
			WndPtr->SetWndMaskWndActived(MaskWndPtr);
		}
		WndPtr->SetWndModified(true);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	AOIObjManager.DestroyWndMaskObj(TempMaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndMaskWndEraseMode(CAOIWnd *RefWndPtr, bool bEraseMode)//設定模組檢測框內的遮罩框-清除遮罩模式
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();	
	
	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);		
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndPtr->SetWndModified(true);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }
			MaskWndPtr->SetWndMaskEraseMode(bEraseMode);
		}
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndMaskWndShapeMode(CAOIWnd *RefWndPtr, BOX_SHAPE_MODE BoxShapeMode)//設定模組檢測框內的遮罩框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();	
	
	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndPtr->SetWndModified(true);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }
			MaskWndPtr->SetWndMaskShapeMode(BoxShapeMode);			
		}
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndMaskWndShapeParam(CAOIWnd *RefWndPtr, double ShapeParam)//設定模組檢測框內的遮罩框外形參數
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();	
	
	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndPtr->SetWndModified(true);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }
			MaskWndPtr->SetWndMaskShapeParam(ShapeParam);
		}
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelWndMaskWndShapeParam2(CAOIWnd *RefWndPtr, double ShapeParam2)//設定模組檢測框內的遮罩框外形參數
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();	
	
	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndPtr->SetWndModified(true);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }
			MaskWndPtr->SetWndMaskShapeParam2(ShapeParam2);
		}
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelWndMaskWnd(CAOIWnd *RefWndPtr)//刪除模組檢測框內的遮罩框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	CAOIWnd            *WndPtr = NULL;
	CAOIWndMask        *MaskWndPtr = NULL;
	std::vector<size_t> MaskWndIndexList;
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();	
	
	MaskWndIndexList.clear();
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
		
		WndPtr->SetWndAllMaskWndSelected(false);
		for ( j=0; j<MaskWndIndexCount; j++ )
		{
			Index = MaskWndIndexList[j];
			MaskWndPtr = WndPtr->GetWndMaskWndPtr(Index, true);
			if ( NULL == MaskWndPtr ) { continue; }
			MaskWndPtr->SetWndMaskSelected(true);
		}		
		WndPtr->DestroyWndMaskWndSelected();
		WndPtr->SetWndModified(true);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ClearModelWndMaskWndList(CAOIWnd *RefWndPtr)//清除模組檢測框內的遮罩框
{
	if ( NULL == RefWndPtr ) { return false; }
	if ( CheckModelWndValid(RefWndPtr) == false ) { return false; }

	size_t              Index=0;
	size_t              i=0, j=0;	
	CAOIWnd            *WndPtr = NULL;	
	const int WndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }		
		WndPtr->ClearWndMaskWndList();
		WndPtr->SetWndModified(true);
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelAlgFreeGroupID() const
{
	size_t          i = 0;
	int             AlgGroupID = -1;
	int             MaxAlgGroupID = -1;
	CAOIWnd        *WndPtr = NULL;		
	const size_t   ModelWndCount = GetModelWndCount();
	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		AlgGroupID = WndPtr->GetWndAlgGroupID();
		if ( MaxAlgGroupID < AlgGroupID )
		{	MaxAlgGroupID = AlgGroupID; }
	}
	return MaxAlgGroupID+1;
}
//-------------------------------------------------------------------------------------//
size_t CAOIModel::GetModelLogicCount() const
{
	return CAOIModel::GetModelLogicCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOIModel::AddModelLogicPtr(CAOILogic *LogicPtr, bool Clone)
{
	if ( NULL == LogicPtr ) { return NULL; }	
	if ( false == Clone )
	{		
		const unsigned int LogicIndex = (unsigned int)(CAOIModel::GetModelLogicCount_Inline());
		CAOIModel::AddModelLogicPtr_Inline(LogicPtr);
		LogicPtr->SetLogicIndex(LogicIndex);	
		LogicPtr->SetLogicModelPtr(this);
		return LogicPtr;
	}

	CAOILogic *NewLogicPtr = CopyModelLogic(LogicPtr);	
	return NewLogicPtr;
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOIModel::CopyModelLogic(const CAOILogic *LogicPtr)
{
	if ( NULL == LogicPtr ) { return NULL; }
	CAOILogic *NewLogicPtr = LogicPtr->CloneLogicObj();
	if ( NULL == NewLogicPtr ) 
	{	return NULL; }
	return NewLogicPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DestroyModelLogicSelected()
{
	size_t      i=0;	
	size_t      LogicIndex=0;
	size_t      LogicCount = 0;		
	CAOILogic   *LogicPtr = NULL;	
	std::vector<CAOILogic*> ModelLogicList = CAOIModel::m_ModelLogicList;	
	
	LogicIndex=0;	
	CAOIModel::m_ModelLogicList.clear();
	LogicCount = ModelLogicList.size();
	for ( i=0; i<LogicCount; i++ )
	{	
		LogicPtr = ModelLogicList[i];
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicSelected() == true ) 
		{
			AOIObjManager.DestroyLogicObj(ModelLogicList[i]);
			continue; 
		}
		LogicPtr->SetLogicIndex(LogicIndex);		
		CAOIModel::AddModelLogicPtr_Inline(LogicPtr);
		LogicIndex ++;
	}
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOIModel::GetModelLogicPtr(int index, bool Check) const
{
	if ( Check )
	{
		const size_t size = CAOIModel::GetModelLogicCount_Inline();
		if ( index >= size ) 
		{	return NULL; }
	}
	return CAOIModel::GetModelLogicPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOIModel::GetModelLogicPtrByGroupID(int LogicGroupID, bool NoIsolatedLogic) const
{
	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = (CAOIModel::GetModelLogicPtr_Inline(i));
		if ( NULL == LogicPtr ) { continue; }
		if ( true == NoIsolatedLogic )
		{
			if ( LogicPtr->GetLogicIsolated() == true )
			{	continue; }
		}
		if ( LogicPtr->GetLogicGroupID() == LogicGroupID )
		{	return LogicPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelLogicSelected(bool val)
{
	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = (GetModelLogicPtr_Inline(i));
		if ( NULL == LogicPtr ) { continue; }
		LogicPtr->SetLogicSelected(val);
	}
	return;
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOIModel::GetModelLogicSelected() const
{
	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();	
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicSelected() == true )
		{	return LogicPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelLogicSelectedCount() const
{
	size_t       i=0;
	int          Count = 0;	
	CAOILogic   *LogicPtr = NULL;
	const size_t size = CAOIModel::GetModelLogicCount_Inline();	

	Count = 0;
	for ( i=0; i<size; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }		
		if ( LogicPtr->GetLogicSelected() == false )
		{	continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UnSelectModelLogic()
{
	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }
		LogicPtr->SetLogicSelected(false);				
		//LogicPtr->SetLogicActived(false);
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelLogicModified()
{
	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicModified() == false ) { continue; }
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelLogicModified(bool value)
{
	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }
		LogicPtr->SetLogicModified(value);
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelLogicValid(const CAOILogic *RefLogicPtr)//確認邏輯閘指標有效-屬於此模組內
{
	if ( NULL == RefLogicPtr ) { return false; }

	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }
		if ( RefLogicPtr == LogicPtr )
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelLogicList()
{
	size_t       i = 0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{	
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		AOIObjManager.DestroyLogicObj(LogicPtr);		
	}
	CAOIModel::m_ModelLogicList.clear();
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelLogicFreeGroupID() const
{
	size_t       i = 0;
	int          MaxGroupID=-1;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{	
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		if ( MaxGroupID < LogicPtr->GetLogicGroupID() )
		{	MaxGroupID = LogicPtr->GetLogicGroupID();	}
	}
	return (MaxGroupID+1);	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelLogic(CAOILogic *RefLogicPtr)
{	
	if ( NULL == RefLogicPtr ) { return false; }

	size_t       i=0, j=0;
	size_t       LogicWndCount = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOILogic   *LogicPtr = NULL;
	const size_t LandCount = GetModelLandCount();
	CAOILand  *RefLandPtr = RefLogicPtr->GetLogicLandPtr();
	const int  RefLogicGroupID = RefLogicPtr->GetLogicGroupID();

	SetModelLogicSelected(false);
	SetModelWndSelected(false);
	SetModelLandSelected(false);

	//移除在Land的關聯	
	if ( NULL != RefLandPtr )
	{
		const int RefLandGroupID = RefLandPtr->GetLandGroupID();
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
			LogicPtr = LandPtr->GetLandLogicPtrByGroupID(RefLogicGroupID);
			if ( NULL == LogicPtr ) { continue; }
			LogicPtr->SetLogicSelected(true);
			LandPtr->RemoveLandLogicSelected();
		}
	}
	else
	{	RefLogicPtr->SetLogicSelected(true);	}
	
	DestroyModelLogicSelected();
	UpdateModelLogicWndIndex();
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelLogicSelectedByLogicGroupID(int LogicGroupID, bool Selected)
{
	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();	
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicGroupID() != LogicGroupID ) { continue; }
		LogicPtr->SetLogicSelected(Selected);
	}
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOIModel::GetModelLogicSelectedByLogicGroupID(int LogicGroupID) const
{
	size_t i=0;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();	
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicGroupID() != LogicGroupID ) { continue; }
		if ( LogicPtr->GetLogicSelected() == true )
		{	return LogicPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelWndVisibledByLogicGroupID(int LogicGroupID, bool Visibled)
{
	size_t       i=0, j=0;	
	int          LogicWndCount = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILogic   *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();	
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);	
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicGroupID() != LogicGroupID ) { continue; }
		
		LogicWndCount = LogicPtr->GetLogicWndPtrCount();
		for ( j=0; j<LogicWndCount; j++ )
		{
			WndPtr = LogicPtr->GetLogicWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }			
			WndPtr->SetWndVisibled(Visibled);
		}
	}
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelLogicSelected()
{		
	unsigned int LogicIndex=0;
	int          LogicGroupID=0;
	size_t       i=0, j=0;	
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOILand    *RefLandPtr = NULL;
	CAOILogic   *LogicPtr = NULL;
	std::vector<unsigned int> LogicIndexList;	
	const size_t LandCount = GetModelLandCount();		
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicSelected() == false )
		{	continue; }

		LogicIndexList.push_back(i);
		RefLandPtr = LogicPtr->GetLogicLandPtr();
		if ( RefLandPtr != NULL )
		{
			LogicGroupID = LogicPtr->GetLogicGroupID();
			for ( j=0; j<LandCount; j++ )
			{
				LandPtr = GetModelLandPtr(j, false);
				if ( LandPtr == NULL ) { continue; }
				if ( LandPtr == RefLandPtr ) { continue; }
				if ( LandPtr->GetLandGroupID() != RefLandPtr->GetLandGroupID() ) { continue; }
				LogicPtr = LandPtr->GetLandLogicPtrByGroupID(LogicGroupID);
				if ( LogicPtr == NULL ) { continue; }
				LogicIndex = LogicPtr->GetLogicIndex();
				LogicIndexList.push_back(LogicIndex);
			}
		}		
	}	
	if ( LogicIndexList.size() == 0 ) { return true; }
	CAOIModel::DeleteModelLogicIndexList(LogicIndexList);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ApplyModelLogic(CAOILogic *RefLogicPtr)//套用邏輯閘至其他相同群組
{
	//未處理
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UpdateModelLogicWndIndex()
{
	size_t      i=0;	
	CAOILogic  *LogicPtr = NULL;		
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();		
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		LogicPtr->UpdateLogicWndIndex();
	}		
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::DeleteModelLogicIndexList(std::vector<unsigned int> &LogicIndexList)
{
	unsigned int LogicIndex = 0;
	size_t       i=0, j=0;		
	CAOILogic   *LogicPtr = NULL;
	size_t       ItemWndBoxCount = 0;
	size_t       ItemCount = 0;
	const size_t LogicIndexCount = LogicIndexList.size();
	
	SetModelLogicSelected(false);
	SetModelWndSelected(false);
	SetModelLandSelected(false);

	for ( i=0; i<LogicIndexCount; i++ )
	{
		LogicIndex = LogicIndexList[i];
		LogicPtr = CAOIModel::GetModelLogicPtr(LogicIndex, true);
		if ( NULL == LogicPtr ) { continue; }		
		LogicPtr->SetLogicSelected(true);
	}
	RemoveModelObjectByLogicObjSelected();	
	DestroyModelLogicSelected();
	UpdateModelLogicWndIndex();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::LayoutModelImageFolderIndexList()//重新計算樣板資料夾引數列表
{
	/*
	int        PatternFolderIndex = 0;
	size_t     i=0, j=0, k=0;	
	CAlgObj   *AlgPtr   = NULL;
	CAOIWnd   *WndPtr = NULL;

	const size_t WndCount = GetModelWndCount();
	const size_t FolderCount = CAOIModel::m_PatternFolderList.size();
	std::vector<BOOL> PatternFolderList = CAOIModel::m_PatternFolderList;

	for ( i=0; i<FolderCount; i++ )
	{	CAOIModel::m_PatternFolderList[i] = FALSE; }

	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr(i);
		if ( WndPtr == NULL ) { continue; }
		
		AlgPtr = WndPtr->GetWndAlgPtr();
		if ( AlgPtr == NULL ) { continue; }

		PatternFolderIndex = AlgPtr->GetAlgPatternFolderIndex();
		if ( PatternFolderIndex < 0 ) { continue; }
		if ( PatternFolderIndex >= FolderCount ) { continue; }
		CAOIModel::m_PatternFolderList[PatternFolderIndex] = TRUE;		
	}	


	CString PatternFolder, FullPatternFolder;
	CString ModelFolder = CAOIModel::GetModelFolder();
	for ( i=0; i<FolderCount; i++ )
	{	
		if ( PatternFolderList[i] == FALSE ) { continue; }
		if ( CAOIModel::m_PatternFolderList[i] == TRUE ) { continue; }
		PatternFolder = CAlgObj::ObtainAlgPatternFolder(i);
		FullPatternFolder.Format(_T("%s\\%s"), ModelFolder, PatternFolder);
		JetAPI::RemoveFolder(FullPatternFolder);
	}
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CombineModelObjectList()
{
	size_t       i=0, j=0;
	unsigned int WndIndex=0;	
	unsigned int LandIndex=0;	
	unsigned int LogicIndex=0;	
	size_t       WndLogicCount = 0;
	size_t       LogicWndCount = 0;

	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOILogic   *LogicPtr = NULL;
	std::vector<unsigned int>  IndexList;
	const size_t WndCount    = GetModelWndCount();
	const size_t LandCount   = GetModelLandCount();	
	const size_t LogicCount   = GetModelLogicCount_Inline();
	
	//移除舊有的
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->RemoveLandWndList();
	}

	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = GetModelLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		LogicPtr->SetLogicLandPtr(NULL);
	//	LogicPtr->RemoveLogicWndPtrList();//須保留Wnd Index所以不先行刪除
	}
	

	//放入新的Wnd
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		
		LandIndex = WndPtr->GetWndLandIndex();
		if ( (LandIndex>=0) && (LandIndex<LandCount) )
		{
			LandPtr = GetModelLandPtr(LandIndex, false);
			if ( NULL != LandPtr )
			{	LandPtr->AddLandWndPtr(WndPtr); }
		}		
	}	
	
	//放入新的Logic
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = GetModelLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		
		LandIndex = LogicPtr->GetLogicLandIndex();
		if ( (LandIndex>=0) && (LandIndex<LandCount) )
		{
			LandPtr = GetModelLandPtr(LandIndex, false);
			if ( NULL != LandPtr )
			{	LandPtr->AddLandLogicPtr(LogicPtr); }
		}

		IndexList.clear();
		LogicWndCount = LogicPtr->GetLogicWndIndexCount();
		for ( j=0; j<LogicWndCount; j++ )
		{
			WndIndex = LogicPtr->GetLogicWndIndex(j, false);
			IndexList.push_back(WndIndex);
		}

		LogicPtr->RemoveLogicWndPtrList();
		LogicWndCount = IndexList.size();
		for ( j=0; j<LogicWndCount; j++ )
		{
			WndIndex = IndexList[j];
			if ( (WndIndex<0) || (WndIndex>=WndCount) ) { continue; }
			WndPtr = m_ModelWndList[WndIndex];
			if ( NULL == WndPtr ) { continue; }			
			LogicPtr->AddLogicWndPtr(WndPtr);
		}
	}	
	CalcModelTotalRegionAll();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RemoveModelObjectByWndObjSelected()
{
	//未完成
	size_t       i = 0;

	//移除其於與他關聯的Land	
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->RemoveLandWndSelected();		
	}
	
	//移除其於與他關聯的Logic		
	CAOILogic    *LogicPtr = NULL;
	const size_t LogicCount = CAOIModel::GetModelLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOIModel::GetModelLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }		
		LogicPtr->RemoveLogicWndPtrSelected();			
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RemoveModelObjectByLogicObjSelected()
{
	size_t       i = 0;
	//移除其於與他關聯的Land	
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->RemoveLandLogicSelected();				
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelAttachedAngle(double Angle)
{
	size_t    i=0;
	m_ModelAttachedAngle = Angle;	
	m_ModelBodyBox.SetBoxAttachedAngle(Angle);
	
	CAOILand  *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( LandPtr == NULL ) { continue; }
		LandPtr->SetLandAttachedAngle(Angle);
	}

	CAOIWnd  *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndAttachedAngle(Angle);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelAttachedPosCad(const TPOINT2D &Pos)
{
	CAOIModel::SetModelAttachedPosCad(Pos.x, Pos.y);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelAttachedPosCad(double PosX, double PosY)
{
	size_t    i=0;
	CAOIModel::m_ModelAttachedCadPos.x = PosX;
	CAOIModel::m_ModelAttachedCadPos.y = PosY;
	CAOIModel::m_ModelBodyBox.SetBoxAttachedPosCad(m_ModelAttachedCadPos);
	
	CAOILand  *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( LandPtr == NULL ) { continue; }
		LandPtr->SetLandAttachedPosCad(m_ModelAttachedCadPos);
	}

	CAOIWnd  *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelAttachedPosStage(const TPOINT2D &Pos)
{
	CAOIModel::SetModelAttachedPosStage(Pos.x, Pos.y);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelAttachedPosStage(double PosX, double PosY)
{
	size_t    i=0;
	CAOIModel::m_ModelAttachedStagePos.x = PosX;	
	CAOIModel::m_ModelAttachedStagePos.y = PosY;	
	CAOIModel::m_ModelBodyBox.SetBoxAttachedPosStage(m_ModelAttachedStagePos);
	
	CAOILand  *LandPtr = NULL;
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( LandPtr == NULL ) { continue; }
		LandPtr->SetLandAttachedPosStage(m_ModelAttachedStagePos);
	}

	CAOIWnd  *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelType(MODEL_TYPE ModelType)//建立模組樣式
{
	bool IsOK = true;
	double BodyW = 1000;
	double BodyH = 1000;
	switch ( ModelType )
	{
	case MODEL_TYPE_NULL:				
		break;
	case MODEL_TYPE_CHIP:
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_R:
	case MODEL_TYPE_CHIP_L:
	case MODEL_TYPE_CHIP_LED:
	case MODEL_TYPE_MELF:
		BodyW = 1500;
		BodyH =  700;		
		break;
	case MODEL_TYPE_ELECTRODE:
	case MODEL_TYPE_TANTALUM_CONDENSER:
	case MODEL_TYPE_CAPACITY_ARRAY:
	case MODEL_TYPE_RESISTOR_ARRAY:
	case MODEL_TYPE_TRANSISTOR:
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
	case MODEL_TYPE_LED_ARRAY:	
		BodyW = 1500;
		BodyH =  700;		
		break;
	case MODEL_TYPE_NO_LEAD_COMPONENT:
	case MODEL_TYPE_NO_LEAD_DFN:
	case MODEL_TYPE_NO_LEAD_QFN:
	case MODEL_TYPE_NO_LEAD_OSC:
		BodyW = 1500;
		BodyH = 1500;		
		break;	
	case MODEL_TYPE_LEAD_COMPONENT:
	case MODEL_TYPE_LEAD_SOP:
	case MODEL_TYPE_LEAD_QFP:
		BodyW = 3000;
		BodyH = 3000;		
		break;
	case MODEL_TYPE_LEAD_TRANSISTOR:
		BodyW = 1500;
		BodyH =  700;	
		break;
	case MODEL_TYPE_JLEAD_COMPONENT:
	case MODEL_TYPE_JLEAD_SOJ:
	case MODEL_TYPE_JLEAD_PLCC:
		BodyW = 1500;
		BodyH =  700;		
		break;
	case MODEL_TYPE_COMPOSITE_COMPONENT:
	case MODEL_TYPE_POWER_TRANSISTOR:
	case MODEL_TYPE_CONNECTOR:			
		break;
	case MODEL_TYPE_BGA:
	case MODEL_TYPE_FD:
	case MODEL_TYPE_BARCODE:		
		break;
	case MODEL_TYPE_PAD_COMPONENT:	
	case MODEL_TYPE_GOLD_FINGER:
		BodyW = 1500;
		BodyH = 1500;		
		break;
	case MODEL_TYPE_DIP_LEAD:		
		break;
	case MODEL_TYPE_OTHERS:		
		break;
	default://MODEL_TYPE_NULL
		IsOK = false;
		break;
	}
	return BuildModelType(ModelType, BodyW, BodyH);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelType(MODEL_TYPE ModelType, double BodyW, double BodyH)//建立模組樣式
{
	bool IsOK = true;	
	switch ( ModelType )
	{
	case MODEL_TYPE_NULL:		
		IsOK = BuildModelBasic(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_CHIP:
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_R:
	case MODEL_TYPE_CHIP_L:
	case MODEL_TYPE_CHIP_LED:
	case MODEL_TYPE_MELF:		
		IsOK = BuildModelChip(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_ELECTRODE:
	case MODEL_TYPE_TANTALUM_CONDENSER:
	case MODEL_TYPE_CAPACITY_ARRAY:
	case MODEL_TYPE_RESISTOR_ARRAY:
	case MODEL_TYPE_TRANSISTOR:
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
	case MODEL_TYPE_LED_ARRAY:			
		IsOK = BuildModelArray(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_NO_LEAD_COMPONENT:
	case MODEL_TYPE_NO_LEAD_DFN:
	case MODEL_TYPE_NO_LEAD_QFN:
	case MODEL_TYPE_NO_LEAD_OSC:
		IsOK = BuildModelNoLeadComponent(ModelType, BodyW, BodyH);
		break;	
	case MODEL_TYPE_LEAD_COMPONENT:
	case MODEL_TYPE_LEAD_SOP:
	case MODEL_TYPE_LEAD_QFP:		
	case MODEL_TYPE_LEAD_TRANSISTOR:
		IsOK = BuildModelFlatLeadComponent(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_JLEAD_COMPONENT:
	case MODEL_TYPE_JLEAD_SOJ:
	case MODEL_TYPE_JLEAD_PLCC:		
		IsOK = BuildModelJLeadComponent(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_COMPOSITE_COMPONENT:
	case MODEL_TYPE_POWER_TRANSISTOR:
	case MODEL_TYPE_CONNECTOR:	
		IsOK = BuildModelBasic(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_BGA:
	case MODEL_TYPE_FD:
	case MODEL_TYPE_BARCODE:
		IsOK = BuildModelBasic(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_PAD_COMPONENT:	
	case MODEL_TYPE_GOLD_FINGER:		
		IsOK = BuildModelPadComponent(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_DIP_LEAD:
		IsOK = BuildModelBasic(ModelType, BodyW, BodyH);
		break;
	case MODEL_TYPE_OTHERS:
		IsOK = BuildModelBasic(ModelType, BodyW, BodyH);
		break;
	default://MODEL_TYPE_NULL
		IsOK = false;
		break;
	}
	if ( true == IsOK )
	{	
		m_ModelType = ModelType;	
		if ( CheckModelTypUseChipSizeMode(ModelType) == true )
		{	SetModelBodyLinkChipLead(true);	}
		else
		{	SetModelBodyLinkChipLead(false);	}		
		SetModelWndSyncMoveMode(ObtainModelDefaultWndSyncMoveMode(ModelType));
	}
	return true;  
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelBasic(MODEL_TYPE ModelType, double BodyW, double BodyH)//未定義的模組
{
	int       WndGroupID = 0;
	int       LandGroupID = 0;

	TREGION4D Region;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	ClearModelAllObjList();
	RemoveModelImageFolder();//先行清除舊有的樣版資料夾

	SetModelType(ModelType);	
	//本體尺寸
	BOX_TOWARD ModelToward = BOX_TOWARD_RIGHT;	
	BoxPtr = GetModelBodyBoxPtr();	
	BoxPtr->SetBoxToward(ModelToward);
	BoxPtr->SetBoxAngle(0);
	BoxPtr->SetBoxPos(0, 0);
	BoxPtr->SetBoxSize(BodyW, BodyH);
	BoxPtr->GetBoxRegion(Region);
	BoxPtr->SetBoxRegion(Region);	
	BoxPtr->SetBoxSelected(true);
	
	CAOIModel::SetModelAttachedAngle(0);
	CAOIModel::SetModelAttachedPosCad(m_ModelAttachedCadPos);
	CAOIModel::SetModelAttachedPosStage(m_ModelAttachedStagePos);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelChip(MODEL_TYPE ModelType, double BodyW, double BodyH)
{
	int       WndGroupID = 0;
	int       LandGroupID = 0;

	TREGION4D Region;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	ClearModelAllObjList();
	RemoveModelImageFolder();//先行清除舊有的樣版資料夾

	SetModelType(ModelType);
	//本體尺寸
	BOX_TOWARD ModelToward = BOX_TOWARD_RIGHT;	
	BoxPtr = GetModelBodyBoxPtr();	
	BoxPtr->SetBoxToward(ModelToward);
	BoxPtr->SetBoxAngle(0);
	BoxPtr->SetBoxPos(0, 0);
	BoxPtr->SetBoxSize(BodyW, BodyH);
	BoxPtr->GetBoxRegion(Region);
	BoxPtr->SetBoxRegion(Region);	
	BoxPtr->SetBoxSelected(true);
	
	CAOIModel::SetModelAttachedAngle(0);
	CAOIModel::SetModelAttachedPosCad(m_ModelAttachedCadPos);
	CAOIModel::SetModelAttachedPosStage(m_ModelAttachedStagePos);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelArray(MODEL_TYPE ModelType, double BodyW, double BodyH)//排容, 排阻
{
	int       WndGroupID = 0;
	int       LandGroupID = 0;

	TREGION4D Region;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	ClearModelAllObjList();
	RemoveModelImageFolder();//先行清除舊有的樣版資料夾

	SetModelType(ModelType);
	//本體尺寸
	BOX_TOWARD ModelToward = BOX_TOWARD_RIGHT;	
	BoxPtr = GetModelBodyBoxPtr();	
	BoxPtr->SetBoxToward(ModelToward);
	BoxPtr->SetBoxAngle(0);
	BoxPtr->SetBoxPos(0, 0);
	BoxPtr->SetBoxSize(BodyW, BodyH);
	BoxPtr->GetBoxRegion(Region);
	BoxPtr->SetBoxRegion(Region);	
	BoxPtr->SetBoxSelected(true);
	
	CAOIModel::SetModelAttachedAngle(0);
	CAOIModel::SetModelAttachedPosCad(m_ModelAttachedCadPos);
	CAOIModel::SetModelAttachedPosStage(m_ModelAttachedStagePos);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelJLeadComponent(MODEL_TYPE ModelType, double BodyW, double BodyH)//J型腳的模組
{
	int       WndGroupID = 0;
	int       LandGroupID = 0;

	TREGION4D Region;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	ClearModelAllObjList();
	RemoveModelImageFolder();//先行清除舊有的樣版資料夾

	SetModelType(ModelType);
	//本體尺寸
	BOX_TOWARD ModelToward = BOX_TOWARD_RIGHT;	
	BoxPtr = GetModelBodyBoxPtr();	
	BoxPtr->SetBoxToward(ModelToward);
	BoxPtr->SetBoxAngle(0);
	BoxPtr->SetBoxPos(0, 0);
	BoxPtr->SetBoxSize(BodyW, BodyH);
	BoxPtr->GetBoxRegion(Region);
	BoxPtr->SetBoxRegion(Region);	
	BoxPtr->SetBoxSelected(true);
	
	CAOIModel::SetModelAttachedAngle(0);
	CAOIModel::SetModelAttachedPosCad(m_ModelAttachedCadPos);
	CAOIModel::SetModelAttachedPosStage(m_ModelAttachedStagePos);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelFlatLeadComponent(MODEL_TYPE ModelType, double BodyW, double BodyH)
{
	int       WndGroupID = 0;
	int       LandGroupID = 0;

	TREGION4D Region;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	ClearModelAllObjList();
	RemoveModelImageFolder();//先行清除舊有的樣版資料夾

	SetModelType(ModelType);
	//本體尺寸
	BOX_TOWARD ModelToward = BOX_TOWARD_RIGHT;	
	BoxPtr = GetModelBodyBoxPtr();	
	BoxPtr->SetBoxToward(ModelToward);
	BoxPtr->SetBoxAngle(0);
	BoxPtr->SetBoxPos(0, 0);
	BoxPtr->SetBoxSize(BodyW, BodyH);
	BoxPtr->GetBoxRegion(Region);
	BoxPtr->SetBoxRegion(Region);	
	
	CAOIModel::SetModelAttachedAngle(0);
	CAOIModel::SetModelAttachedPosCad(m_ModelAttachedCadPos);
	CAOIModel::SetModelAttachedPosStage(m_ModelAttachedStagePos);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelNoLeadComponent(MODEL_TYPE ModelType, double BodyW, double BodyH)//無腳類的模組
{
	int       WndGroupID = 0;
	int       LandGroupID = 0;

	TREGION4D Region;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	ClearModelAllObjList();
	RemoveModelImageFolder();//先行清除舊有的樣版資料夾

	SetModelType(ModelType);
	//本體尺寸
	BOX_TOWARD ModelToward = BOX_TOWARD_RIGHT;	
	BoxPtr = GetModelBodyBoxPtr();	
	BoxPtr->SetBoxToward(ModelToward);
	BoxPtr->SetBoxAngle(0);
	BoxPtr->SetBoxPos(0, 0);
	BoxPtr->SetBoxSize(BodyW, BodyH);
	BoxPtr->GetBoxRegion(Region);
	BoxPtr->SetBoxRegion(Region);	
	BoxPtr->SetBoxSelected(true);
	
	CAOIModel::SetModelAttachedAngle(0);
	CAOIModel::SetModelAttachedPosCad(m_ModelAttachedCadPos);
	CAOIModel::SetModelAttachedPosStage(m_ModelAttachedStagePos);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelPadComponent(MODEL_TYPE ModelType, double BodyW, double BodyH)//建立焊盤元件
{
	int       WndGroupID = 0;
	int       LandGroupID = 0;

	TREGION4D Region;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	ClearModelAllObjList();
	RemoveModelImageFolder();//先行清除舊有的樣版資料夾

	SetModelType(ModelType);
	//本體尺寸
	BOX_TOWARD ModelToward = BOX_TOWARD_RIGHT;	
	BoxPtr = GetModelBodyBoxPtr();	
	BoxPtr->SetBoxToward(ModelToward);
	BoxPtr->SetBoxAngle(0);
	BoxPtr->SetBoxPos(0, 0);
	BoxPtr->SetBoxSize(BodyW, BodyH);
	BoxPtr->GetBoxRegion(Region);
	BoxPtr->SetBoxRegion(Region);	
	BoxPtr->SetBoxSelected(true);
	
	CAOIModel::SetModelAttachedAngle(0);
	CAOIModel::SetModelAttachedPosCad(m_ModelAttachedCadPos);
	CAOIModel::SetModelAttachedPosStage(m_ModelAttachedStagePos);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelActObjLinkWndRgn(const TActiveObj *ObjPtr) const//確認模組物件連動檢測框
{
	if ( NULL == ObjPtr ) { return false; }
	if ( NULL != ObjPtr->WndMaskPtr ) { return false; }
	if ( NULL != ObjPtr->WndRoiPtr ) { return false; }
	if ( NULL != ObjPtr->WndPtr ) { return false; }
	//if ( NULL != ObjPtr->LandPtr ) { return true; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelSelectedObjListForPos(TActiveObj *ObjPtr, std::vector<TActiveObj> &ActObjList, std::vector<TActiveObj*> &SelObjList)//取得模組內選用的物件
{
	if ( NULL == ObjPtr ) { return false; }
	
	std::vector<CAOIWnd*> WndList;
	std::vector<CAOILand*> LandList;
	const size_t NObjects = ActObjList.size();
	CAOIBox* BoxPtr    = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	CAOIWnd* WndPtr    = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	CAOILand* LandPtr   = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	CAOIWndRoi* WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	CAOIWndMask* WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);
	
	SelObjList.clear();
	SelObjList.push_back(ObjPtr);
	if ( NULL != WndPtr )
	{	WndList.push_back(WndPtr);	}
	if ( NULL != LandPtr )
	{	LandList.push_back(LandPtr);	}

	if ( (NULL!=WndRoiPtr) && (NULL!=WndPtr) )
	{
	}
	else if ( (NULL!=WndMaskPtr) && (NULL!=WndPtr) )
	{
		
	}
	else if ( NULL != WndPtr )
	{	
		const int WndBandID = WndPtr->GetWndBandID();
		const int WndGroupID = WndPtr->GetWndGroupID();
		for ( size_t i=0; i<NObjects; i++ )
		{
			ObjPtr = &(ActObjList[i]);
			if ( false == ObjPtr->GetSelected() ) { continue;	}
			if ( NULL == ObjPtr->WndPtr ) { continue; }

			bool bExist=false;
			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			CAOIWnd *WndTmp    = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
			for ( size_t j=0; j<WndList.size(); j++ )
			{
				WndPtr = WndList[j];
				if ( NULL == WndPtr ) { continue; }
				if ( WndTmp->CheckWndLinkPos(WndPtr) == true )
				{
					bExist = true;
					break;
				}
			}
			if ( false == bExist )
			{
				WndList.push_back(WndTmp);
				SelObjList.push_back(ObjPtr);	
			}
		}
	}
	else if ( NULL != LandPtr )
	{			
		for ( size_t i=0; i<NObjects; i++ )
		{
			ObjPtr = &(ActObjList[i]);
			if ( false == ObjPtr->GetSelected() ) { continue;	}
			if ( NULL == ObjPtr->LandPtr ) { continue; }

			bool bExist=false;
			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			CAOILand *LandTmp   = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
			for ( size_t j=0; j<LandList.size(); j++ )
			{
				LandPtr = LandList[j];
				if ( NULL == LandPtr ) { continue; }
				if ( LandTmp->CheckLandLinkPos(LandPtr) == true )
				{
					bExist = true;
					break;
				}
			}
			if ( false == bExist )
			{
				LandList.push_back(LandTmp);
				SelObjList.push_back(ObjPtr);	
			}
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelSelectedObjListForSize(TActiveObj *ObjPtr, std::vector<TActiveObj> &ActObjList, std::vector<TActiveObj*> &SelObjList)//取得模組內選用的物件
{
	if ( NULL == ObjPtr ) { return false; }
	
	std::vector<CAOIWnd*> WndList;
	std::vector<CAOILand*> LandList;
	const size_t NObjects = ActObjList.size();
	CAOIBox* BoxPtr    = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	CAOIWnd* WndPtr    = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	CAOILand* LandPtr   = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	CAOIWndRoi* WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	CAOIWndMask* WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);
	
	SelObjList.clear();
	SelObjList.push_back(ObjPtr);
	if ( NULL != WndPtr )
	{	WndList.push_back(WndPtr);	}
	if ( NULL != LandPtr )
	{	LandList.push_back(LandPtr);	}

	if ( (NULL!=WndRoiPtr) && (NULL!=WndPtr) )
	{
	}
	else if ( (NULL!=WndMaskPtr) && (NULL!=WndPtr) )
	{
		
	}
	else if ( NULL != WndPtr )
	{	
		const int WndBandID = WndPtr->GetWndBandID();
		const int WndGroupID = WndPtr->GetWndGroupID();
		for ( size_t i=0; i<NObjects; i++ )
		{
			ObjPtr = &(ActObjList[i]);
			if ( false == ObjPtr->GetSelected() ) { continue;	}
			if ( NULL == ObjPtr->WndPtr ) { continue; }

			bool bExist=false;
			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			CAOIWnd *WndTmp    = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
			for ( size_t j=0; j<WndList.size(); j++ )
			{
				WndPtr = WndList[j];
				if ( NULL == WndPtr ) { continue; }
				if ( WndTmp->CheckWndLinkSize(WndPtr) == true )
				{
					bExist = true;
					break;
				}
			}
			if ( false == bExist )
			{
				WndList.push_back(WndTmp);
				SelObjList.push_back(ObjPtr);	
			}
		}
	}
	else if ( NULL != LandPtr )
	{			
		for ( size_t i=0; i<NObjects; i++ )
		{
			ObjPtr = &(ActObjList[i]);
			if ( false == ObjPtr->GetSelected() ) { continue;	}
			if ( NULL == ObjPtr->LandPtr ) { continue; }

			bool bExist=false;
			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			CAOILand *LandTmp   = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
			for ( size_t j=0; j<LandList.size(); j++ )
			{
				LandPtr = LandList[j];
				if ( NULL == LandPtr ) { continue; }
				if ( LandTmp->CheckLandLinkSize(LandPtr) == true )
				{
					bExist = true;
					break;
				}
			}
			if ( false == bExist )
			{
				LandList.push_back(LandTmp);
				SelObjList.push_back(ObjPtr);	
			}
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelObjPos(std::vector<TActiveObj*> ObjPtrList, double dPx, double dPy, int LinkMode)//修正模組內物件的座標
{
	bool bSucc=true;
	bool bWndRgnByLinkMode=false;
	const size_t ObjPtrCount=ObjPtrList.size();
	for ( size_t i=0; i<ObjPtrCount; i++ )
	{
		TActiveObj *ObjPtr = ObjPtrList[i];
		if ( NULL == ObjPtr ) { continue; }
		CAOIBox *BoxPtr    = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
		CAOIWnd *WndPtr    = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
		CAOILand *LandPtr   = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
		CAOIWndRoi *WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
		CAOIWndMask *WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);	
		if ( CheckModelActObjLinkWndRgn(ObjPtr) == true )
		{	bWndRgnByLinkMode = true;	}
		bSucc = ModifyModelBoxPos(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dPx, dPy, LinkMode, false);
		if ( false == bSucc )
		{	break; }
	}
	if ( true == bWndRgnByLinkMode )
	{	UpdateModelWndRgnByLinkMode();	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelBoxPos(CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOILand *LandPtr, CAOIWndRoi *WndRoiPtr, CAOIWndMask *MaskWndPtr, double dPx, double dPy, int LinkMode, bool bUpdate)//修正模組內框的座標
{
	bool IsOK = true;
	bool bWndRgnByLinkMode=false;
	if ( NULL == BoxPtr ) { return false; }

	if ( NULL !=WndRoiPtr )
	{	IsOK = ModifyModelWndRoiPos(BoxPtr, WndPtr, WndRoiPtr, dPx, dPy, LinkMode, false);	}	
	else if ( NULL !=MaskWndPtr )
	{	IsOK = ModifyModelWndMaskPos(BoxPtr, WndPtr, MaskWndPtr, dPx, dPy, LinkMode, false);	}
	else if ( NULL != WndPtr )
	{	IsOK = ModifyModelWndPos(BoxPtr, WndPtr, dPx, dPy, LinkMode, false);	}
	else if ( NULL != LandPtr )
	{
		bWndRgnByLinkMode = true;
		IsOK = ModifyModelLandPos(BoxPtr, LandPtr, dPx, dPy, LinkMode, false);	
	}
	else
	{
		bWndRgnByLinkMode = true;
		MoveModel(dPx, dPy); 
	}
	if ( true == bUpdate )
	{
		if ( true == bWndRgnByLinkMode )
		{	UpdateModelWndRgnByLinkMode(); }
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelWndPos(CAOIBox *BoxPtr, CAOIWnd *RefWndPtr, double dPx, double dPy, int LinkMode, bool bUpdate)//修正模組內檢測框的座標
{
	if ( NULL == BoxPtr ) { return false; }
	if ( NULL == RefWndPtr ) { return false; }

	size_t       i = 0;
	int          TowardAngle = 0;
	double       dPx2=0, dPy2=0;	
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;
	CAOILand    *RefLandPtr = RefWndPtr->GetWndLandPtr();
	const int    RefWndBandID = RefWndPtr->GetWndBandID();
	const int    RefWndGroupID = RefWndPtr->GetWndGroupID();	
	WND_SYNC_MOVE_MODE RefWndSyncMoveMode = RefWndPtr->GetWndSyncMoveMode();
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();		
	const bool   LinkBodyWndPos = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_BODY_WND_POS);
	const bool   LinkLandWndPos = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_LAND_WND_POS);	
	if ( NULL == RefLandPtr )
	{	
		//false==LinkMode || 
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndSelected() == false ){ continue; }
			if ( WndPtr->GetWndRgnLinkMode() != WND_RGN_LINK_NONE ) { continue; }

			WndPtr->MoveWnd(dPx, dPy); 
			WndPtr->UpdateWndExtendBox();

			if ( true == LinkBodyWndPos )
			{
			}
		}
	}
	else
	{
		TPOINT2D   RefLandPos, LandPos, DotPos;
		const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
		BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
		RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefLandPos);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
			WndPtr = LandPtr->GetLandWndPtrByGroupID(RefWndGroupID, RefWndBandID);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndRgnLinkMode() != WND_RGN_LINK_NONE ) { continue; }

			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);
			if ( false == LinkLandWndPos )
			{
				if ( LandToward != RefLandToward ) 
				{	continue; }
			}

			dPx2 = dPx;
			dPy2 = dPy;
			if ( WND_SYNC_MOVE_MIRROR == RefWndSyncMoveMode )//鏡射
			{	JetAPI::MirrorPos(RefLandToward, LandToward, dPx2, dPy2);	}
			if ( WND_SYNC_MOVE_ROTATE == RefWndSyncMoveMode )//旋轉
			{
				TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);
				switch ( TowardAngle )
				{
				case  90:	dPy2 =  dPx;	dPx2 = -dPy;	break;
				case 180:	dPx2 = -dPx;	dPy2 = -dPy;	break;
				case 270:	dPy2 = -dPx;	dPx2 = dPy;		break;
				default:	dPx2 =  dPx;	dPy2 = dPy;		break;
				}
			}
			if ( WND_SYNC_MOVE_SYMMETRY == RefWndSyncMoveMode )
			{
				TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);
				switch ( TowardAngle )
				{
				case  90:
				case 270:
					DotPos.x = RefLandPos.x*LandPos.y;
					DotPos.y = RefLandPos.y*LandPos.x;
					break;
				default:
				case   0:
				case 180:
					DotPos.x = RefLandPos.x*LandPos.x;
					DotPos.y = RefLandPos.y*LandPos.y;
					break;
				}
				if ( DotPos.x < 0 )
				{	dPx2 = -dPx2;	}
				if ( DotPos.y < 0 )
				{	dPy2 = -dPy2;	}
				switch ( TowardAngle )
				{
				case  90:
				case 270:
					JetAPI::Swap(dPx2, dPy2);					
					break;
				}			
			}	
			WndPtr->MoveWnd(dPx2, dPy2);
			WndPtr->UpdateWndExtendBox();			
		}
	}
	if ( true == bUpdate )
	{
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandPos(CAOIBox *RefBoxPtr, CAOILand *RefLandPtr, double dPx, double dPy, int LinkMode, bool bUpdate)//修正模組內特徵框的座標
{
	if ( NULL == RefBoxPtr ) { return false; }
	if ( NULL == RefLandPtr ) { return false; }	

	size_t     i = 0;
	CAOILand  *LandPtr = NULL;
	bool       MoveLand = false;
	int        TowardAngle = 0;
	double     dPx1=0, dPy1=0;	
	double     dPx2=0, dPy2=0;	
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;
	LAND_TYPE  RefLandType = RefLandPtr->GetLandType();
	const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
	const int  RefLandAlignID = RefLandPtr->GetLandAlignID();
	BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
	const int  BasicBoxID = RefLandPtr->GetLandBasicBoxID(RefBoxPtr);	
	const size_t LandCount = GetModelLandCount();
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	const bool   LinkLandPos = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_LAND_POS);	

	if ( RefLandPtr->GetLandBoxPtr() == RefBoxPtr )
	{	MoveLand = true; }
	else
	{	MoveLand = false; }

	dPx1 = dPx;	dPy1 = dPy;
	if ( IsExceptionAngle == true )
	{	JetAPI::RotatePos(-ComAngle, 0, 0, dPx1, dPy1);	}
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
		if ( LandPtr->GetLandAlignID() != RefLandAlignID ) { continue; }

		if ( false == LinkLandPos )
		{
			if ( LandPtr->GetLandToward()  != RefLandToward ) { continue; }
		}
		
		LandToward = LandPtr->GetLandToward();				
		TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);		
		switch ( TowardAngle )
		{
		case  90:	dPy2 =  dPx1;	dPx2 = -dPy1;	break;
		case 180:	dPx2 = -dPx1;	dPy2 = -dPy1;	break;
		case 270:	dPy2 = -dPx1;	dPx2 =  dPy1;	break;
		default:	dPx2 =  dPx1;	dPy2 =  dPy1;	break;
		}
		if ( LandPtr->GetLandSelected() == false )
		{	
			if ( LandPtr->GetLandToward()!=RefLandToward || false==LinkLandPos )
			{
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
				case BOX_TOWARD_DOWN:
					dPx2 = 0;
					break;
				case BOX_TOWARD_LEFT:
				case BOX_TOWARD_RIGHT:
					dPy2 = 0;
					break;
				}
			}			
		}
		if ( true == IsExceptionAngle )
		{	JetAPI::RotatePos(ComAngle, 0, 0, dPx2, dPy2);	}

		if ( true == MoveLand )
		{				
			LandPtr->MoveLand(dPx2, dPy2);
			continue;
		}
		switch ( BasicBoxID )
		{
		case LAND_BOX_PAD:	
			if ( LAND_TYPE_PAD == RefLandType )
			{	LandPtr->MoveLand(dPx2, dPy2);	}
			else
			{	LandPtr->GetLandPadBox().MoveBox(dPx2, dPy2); }
			break;
		case LAND_BOX_LEAD:
		case LAND_BOX_LEAD_TIP:			
		case LAND_BOX_LEAD_SHOULDER:
			LandPtr->MoveLandLead(dPx2, dPy2);
			break;
		case LAND_BOX_BODY_EDGE:	
			LandPtr->GetLandBodyEdgeBoxPtr()->MoveBox(dPx2, dPy2);
			break;			
		}				
	}

	if ( CheckLandBasicBoxIDLinkBody(BasicBoxID) == true )
	{	UpdateModelBodyRgnFromChipLead();	}	

	if ( true == bUpdate )
	{
		UpdateModelWndRgnByLinkMode();
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelWndRoiPos(CAOIBox *BoxPtr, CAOIWnd *RefWndPtr, CAOIWndRoi *RefWndRoiPtr, double dPx, double dPy, int LinkMode, bool bUpdate)//修正模組內檢測框子框的座標
{
	if ( NULL == BoxPtr ) { return false; }
	if ( NULL == RefWndPtr ) { return false; }
	if ( NULL == RefWndRoiPtr ) { return false; }

	size_t       i=0, k=0;
	int          TowardAngle = 0;
	double       dPx2=0, dPy2=0;
	TREGION4D    WndRgn;
	TREGION4D    WndRoiRgn;	
	TPOINT2D     WndCornerPt[4];
	TPOINT2D     WndRoiCornerPt[4];
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;	
	CAOIWndRoi  *WndRoiPtr = NULL;	
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;
	CAOILand    *RefLandPtr = RefWndPtr->GetWndLandPtr();
	const int    RefWndBandID = RefWndPtr->GetWndBandID();
	const int    RefWndGroupID = RefWndPtr->GetWndGroupID();		
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();	
	const unsigned int RefWndRoiIndex = RefWndRoiPtr->GetWndRoiIndex();
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	const bool   LinkBodyWndPos = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_BODY_WND_POS);
	const bool   LinkLandWndPos = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_LAND_WND_POS);	

	unsigned int WndRoiIndex=0;
	std::vector<CAOIWndRoi*> WndRoiList;
	std::vector<unsigned int> WndRoiIndexList;
	RefWndPtr->GetWndRoiWndSelectedList(WndRoiList);
	const size_t WndRoiWndCount = WndRoiList.size();
	for ( i=0; i<WndRoiWndCount; i++ )
	{
		WndRoiPtr = WndRoiList[i];
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiIndex = WndRoiPtr->GetWndRoiIndex();
		WndRoiIndexList.push_back(WndRoiIndex);
	}
	const size_t WndRoiIndexCount = WndRoiIndexList.size();

	if ( NULL == RefLandPtr )
	{	
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndSelected() == false ){ continue; }
			for ( k=0; k<WndRoiIndexCount; k++ )
			{
				WndRoiIndex = WndRoiIndexList[k];
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(WndRoiIndex, true);
				//WndRoiPtr = WndPtr->GetWndRoiWndPtr(RefWndRoiIndex, true);
				if ( NULL == WndRoiPtr ) { continue; }
				if ( false == IsExceptionAngle )
				{	
					WndPtr->GetWndRegion(WndRgn); 
					WndRoiPtr->GetWndRoiRegion(WndRoiRgn);
				}
				else
				{
					WndPtr->GetWndCornerPos(WndCornerPt);
					WndRoiPtr->GetWndRoiCornerPos(WndRoiCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndRoiCornerPt);
					JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);
					JetAPI::CornerPtToRegion(WndRoiCornerPt, WndRoiRgn);
				}			
				WndRoiRgn.Move(dPx, dPy);
				if ( WndRoiRgn.minX < WndRgn.minX ||
					 WndRoiRgn.minY < WndRgn.minY ||
					 WndRoiRgn.maxX > WndRgn.maxX ||
					 WndRoiRgn.maxY > WndRgn.maxY 
					 )
				{	continue; }

				WndRoiPtr->MoveWndRoi(dPx, dPy); 
				if ( true == LinkBodyWndPos )
				{
				}
			}
		}
	}
	else
	{
		const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
		BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
			WndPtr = LandPtr->GetLandWndPtrByGroupID(RefWndGroupID, RefWndBandID);
			if ( NULL == WndPtr ) { continue; }
			for ( k=0; k<WndRoiIndexCount; k++ )
			{
				WndRoiIndex = WndRoiIndexList[k];
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(WndRoiIndex, true);
				//WndRoiPtr = WndPtr->GetWndRoiWndPtr(RefWndRoiIndex, true);
				if ( NULL == WndRoiPtr ) { continue; }

				LandToward = LandPtr->GetLandToward();
				if ( false == LinkLandWndPos )
				{
					if ( LandToward != RefLandToward ) 
					{	continue; }
				}
				TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);
				switch ( TowardAngle )
				{
				case  90:	dPy2 =  dPx;	dPx2 = -dPy;	break;
				case 180:	dPx2 = -dPx;	dPy2 = -dPy;	break;
				case 270:	dPy2 = -dPx;	dPx2 = dPy;		break;
				default:	dPx2 =  dPx;	dPy2 = dPy;		break;
				}

				if ( false == IsExceptionAngle )
				{	
					WndPtr->GetWndRegion(WndRgn); 
					WndRoiPtr->GetWndRoiRegion(WndRoiRgn);
				}
				else
				{
					WndPtr->GetWndCornerPos(WndCornerPt);
					WndRoiPtr->GetWndRoiCornerPos(WndRoiCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndRoiCornerPt);
					JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);
					JetAPI::CornerPtToRegion(WndRoiCornerPt, WndRoiRgn);
				}
				WndRoiRgn.Move(dPx2, dPy2);
				if ( WndRoiRgn.minX < WndRgn.minX ||
					 WndRoiRgn.minY < WndRgn.minY ||
					 WndRoiRgn.maxX > WndRgn.maxX ||
					 WndRoiRgn.maxY > WndRgn.maxY 
					 )
				{	continue; }
				WndRoiPtr->MoveWndRoi(dPx2, dPy2);
			}
		}
	}

	if ( true == bUpdate )
	{
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelWndMaskPos(CAOIBox *BoxPtr, CAOIWnd *RefWndPtr, CAOIWndMask *RefMaskWndPtr, double dPx, double dPy, int LinkMode, bool bUpdate)//修正模組內檢測框遮罩框的座標
{
	if ( NULL == BoxPtr ) { return false; }
	if ( NULL == RefWndPtr ) { return false; }
	if ( NULL == RefMaskWndPtr ) { return false; }

	size_t       i=0, j=0, k=0;
	size_t       LandWndCount = 0;
	int          TowardAngle = 0;
	double       dPx2=0, dPy2=0;
	TREGION4D    WndRgn;
	TREGION4D    MaskWndRgn;	
	TPOINT2D     WndCornerPt[4];
	TPOINT2D     MaskWndCornerPt[4];
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;	
	CAOIWndMask *MaskWndPtr = NULL;	
	BOX_TOWARD   WndToward = BOX_TOWARD_NULL;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;
	CAOILand    *RefLandPtr = RefWndPtr->GetWndLandPtr();
	const double ComAngle = GetModelAttachedAngle();
	BOX_TOWARD   RefWndToward = RefWndPtr->GetWndToward();
	const int    RefWndBandID = RefWndPtr->GetWndBandID();
	const int    RefWndGroupID = RefWndPtr->GetWndGroupID();		
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();	
	const unsigned int RefMaskBoxIndex = RefMaskWndPtr->GetWndMaskIndex();	
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	const bool   LinkBodyWndPos = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_BODY_WND_POS);
	const bool   LinkLandWndPos = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_LAND_WND_POS);	

	size_t MaskWndIndex=0;
	std::vector<size_t> MaskWndIndexList;
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	if ( NULL == RefLandPtr )
	{	
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			//if ( WndPtr->GetWndSelected() == false ){ continue; }
			if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }
			WndToward = WndPtr->GetWndToward();
			if ( false == IsExceptionAngle )
			{	WndPtr->GetWndRegion(WndRgn);	}
			else
			{
				WndPtr->GetWndCornerPos(WndCornerPt);
				JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndCornerPt);
				JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);				
			}
			TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
			switch ( TowardAngle )
			{
			case  90:	dPy2 =  dPx;	dPx2 = -dPy;	break;
			case 180:	dPx2 = -dPx;	dPy2 = -dPy;	break;
			case 270:	dPy2 = -dPx;	dPx2 = dPy;		break;
			default:	dPx2 =  dPx;	dPy2 = dPy;		break;
			}
			for ( k=0; k<MaskWndIndexCount; k++ )
			{
				MaskWndIndex = MaskWndIndexList[k];
				MaskWndPtr = WndPtr->GetWndMaskWndPtr(MaskWndIndex, true);
				//MaskWndPtr = WndPtr->GetWndMaskWndPtr(RefMaskBoxIndex, true);
				if ( NULL == MaskWndPtr ) { continue; }
				if ( false == IsExceptionAngle )
				{	MaskWndPtr->GetWndMaskRegion(MaskWndRgn);	}
				else
				{					
					MaskWndPtr->GetWndMaskCornerPos(MaskWndCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, MaskWndCornerPt);
					JetAPI::CornerPtToRegion(MaskWndCornerPt, MaskWndRgn);
				}			
				MaskWndRgn.Move(dPx2, dPy2);
				if ( MaskWndRgn.minX < WndRgn.minX ||
					 MaskWndRgn.minY < WndRgn.minY ||
					 MaskWndRgn.maxX > WndRgn.maxX ||
					 MaskWndRgn.maxY > WndRgn.maxY 
					 )
				{	continue; }

				MaskWndPtr->MoveWndMask(dPx2, dPy2);
				if ( true == LinkBodyWndPos )
				{
				}			
			}
		}
	}
	else
	{
		const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
		BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
			LandToward = LandPtr->GetLandToward();
			if ( false == LinkLandWndPos )
			{
				if ( LandToward != RefLandToward ) 
				{	continue; }
			}
			LandWndCount = LandPtr->GetLandWndCount();
			for ( j=0; j<LandWndCount; j++ )
			{
				WndPtr = LandPtr->GetLandWndPtr(j, false);
				if ( NULL == WndPtr ) { continue; }
				if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }
				WndToward = WndPtr->GetWndToward();
				if ( false == IsExceptionAngle )
				{	WndPtr->GetWndRegion(WndRgn);	}
				else
				{
					WndPtr->GetWndCornerPos(WndCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndCornerPt);
					JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);					
				}
				TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
				switch ( TowardAngle )
				{
				case  90:	dPy2 =  dPx;	dPx2 = -dPy;	break;
				case 180:	dPx2 = -dPx;	dPy2 = -dPy;	break;
				case 270:	dPy2 = -dPx;	dPx2 = dPy;		break;
				default:	dPx2 =  dPx;	dPy2 = dPy;		break;
				}
				for ( k=0; k<MaskWndIndexCount; k++ )
				{
					MaskWndIndex = MaskWndIndexList[k];
					MaskWndPtr = WndPtr->GetWndMaskWndPtr(MaskWndIndex, true);
					//MaskWndPtr = WndPtr->GetWndMaskWndPtr(RefMaskBoxIndex, true);
					if ( NULL == MaskWndPtr ) { continue; }
					if ( false == IsExceptionAngle )
					{	MaskWndPtr->GetWndMaskRegion(MaskWndRgn);	}
					else
					{
						MaskWndPtr->GetWndMaskCornerPos(MaskWndCornerPt);
						JetAPI::RotateCornerPos(-ComAngle, 0, 0, MaskWndCornerPt);
						JetAPI::CornerPtToRegion(MaskWndCornerPt, MaskWndRgn);
					}
					MaskWndRgn.Move(dPx2, dPy2);
					if ( MaskWndRgn.minX < WndRgn.minX ||
						 MaskWndRgn.minY < WndRgn.minY ||
						 MaskWndRgn.maxX > WndRgn.maxX ||
						 MaskWndRgn.maxY > WndRgn.maxY 
						 )
					{	continue; }
					MaskWndPtr->MoveWndMask(dPx2, dPy2);
				}
			}
		}
	}

	if ( true == bUpdate )
	{
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelBodySizeToAllWnds()//更新模組本體尺寸至全部檢測框
{
	bool IsOK = true;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);	
	if ( IsExceptionAngle == false )
	{	
		IsOK = UpdateModelBodySizeToAllWndsKernel();				
	}
	else
	{		
		double CPX = 0;
		double CPY = 0;		
		RotateModel(-AttachedAngle, CPX, CPY);				
		IsOK = UpdateModelBodySizeToAllWndsKernel();
		RotateModel(AttachedAngle, CPX, CPY);				
	}	
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelBodySizeToAllWndsKernel()//更新模組本體尺寸至全部檢測框
{	
	size_t i=0;
	CAOIWnd *WndPtr=NULL;
	CAOIBox *BoxPtr=NULL;
	TREGION4D BoxRgn;
	const CAOIBox &BodyBox=GetModelBodyBox();
	const size_t WndCount=GetModelWndCount();

	BodyBox.GetBoxRegion(BoxRgn);
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		BoxPtr = WndPtr->GetWndBoxPtr();
		if ( NULL == BoxPtr ) { continue; }
		BoxPtr->SetBoxRegion(BoxRgn);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelObjSize(std::vector<TActiveObj*> ObjPtrList, const TREGION4D &dPos, int LinkMode)//修正模組內物件的尺寸
{
	bool bSucc=true;
	bool bWndRgnByLinkMode=false;
	const size_t ObjPtrCount=ObjPtrList.size();
	for ( size_t i=0; i<ObjPtrCount; i++ )
	{
		TActiveObj *ObjPtr = ObjPtrList[i];
		if ( NULL == ObjPtr ) { continue; }
		CAOIBox *BoxPtr    = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
		CAOIWnd *WndPtr    = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
		CAOILand *LandPtr   = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
		CAOIWndRoi *WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
		CAOIWndMask *WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);	
		if ( CheckModelActObjLinkWndRgn(ObjPtr) == true )
		{	bWndRgnByLinkMode = true;	}
		bSucc = ModifyModelBoxSize(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dPos, LinkMode, false);
		if ( false == bSucc )
		{	break; }
	}
	if ( true == bWndRgnByLinkMode )
	{	UpdateModelWndRgnByLinkMode();	}
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
	return bSucc;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelBoxSize(CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOILand *LandPtr, CAOIWndRoi *WndRoiPtr, CAOIWndMask *MaskWndPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate)//修正模組內框的尺寸
{
	bool IsOK = true;
	bool bWndRgnByLinkMode=false;
	if ( NULL == BoxPtr ) { return false; }

	if ( NULL != WndRoiPtr )
	{	IsOK = ModifyModelWndRoiSize(BoxPtr, WndPtr, WndRoiPtr, dPos, LinkMode, false);	}
	else if ( NULL != MaskWndPtr )
	{	IsOK = ModifyModelWndMaskSize(BoxPtr, WndPtr, MaskWndPtr, dPos, LinkMode, false);	}
	else if ( NULL != WndPtr )
	{	IsOK = ModifyModelWndSize(BoxPtr, WndPtr, dPos, LinkMode, false);	}
	else if ( NULL != LandPtr )
	{
		bWndRgnByLinkMode = true;
		IsOK = ModifyModelLandSize(BoxPtr, LandPtr, dPos, LinkMode, false);	
	}
	else
	{
		bWndRgnByLinkMode = true;
		IsOK = ModifyModelBodyRegion(dPos.minX, dPos.minY, dPos.maxX, dPos.maxY); 
	}

	if ( true == bUpdate )
	{
		if ( true == bWndRgnByLinkMode )
		{	UpdateModelWndRgnByLinkMode(); }
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelWndSize(CAOIBox *RefBoxPtr, CAOIWnd *RefWndPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate)//修正模組內檢測框的尺寸
{
	if ( NULL == RefBoxPtr ) { return false; }
	if ( NULL == RefWndPtr ) { return false; }	

	const int WndBasicBoxID = RefWndPtr->GetWndBasicBoxID(RefBoxPtr);
	CAOILand *RefLandPtr = RefWndPtr->GetWndLandPtr();
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	const bool   LinkBodyWndSize = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_BODY_WND_SIZE);
	const bool   LinkLandWndSize = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_LAND_WND_SIZE);	
	
	size_t       i=0, j=0, k=0;
	size_t       LandWndCount = 0;
	size_t       WndRoiWndCount = 0;
	size_t       WndMaskBoxCount = 0;
	int          TowardAngle = 0;
	TREGION4D    dPos2;
	TREGION4D    WndRgn;
	TREGION4D    WndRoiRgn;	
	TREGION4D    MaskBoxRgn;	
	CAOIWnd     *WndPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *MaskWndPtr = NULL;
	BOX_TOWARD   WndToward = BOX_TOWARD_NULL;
	BOX_TOWARD   RefWndToward = RefWndPtr->GetWndToward();
	const int    RefWndBandID = RefWndPtr->GetWndBandID();
	const int    RefWndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();
	WND_SYNC_MOVE_MODE RefWndSyncMoveMode = RefWndPtr->GetWndSyncMoveMode();

	if ( NULL == RefLandPtr )
	{	
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }
			if ( WndPtr->GetWndRgnLinkMode() != WND_RGN_LINK_NONE ) { continue; }
			WndToward = WndPtr->GetWndToward();
			if ( false == LinkBodyWndSize )
			{
				if ( WndToward != RefWndToward ) { continue; }
			}			
			TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
			switch ( TowardAngle )
			{
			case  90:
				dPos2.minX = -dPos.maxY;
				dPos2.minY =  dPos.minX;
				dPos2.maxX = -dPos.minY;
				dPos2.maxY =  dPos.maxX;
				break;
			case 180:
				dPos2.minX = -dPos.maxX;
				dPos2.minY = -dPos.maxY;
				dPos2.maxX = -dPos.minX;
				dPos2.maxY = -dPos.minY;					
				break;
			case 270:
				dPos2.minX =  dPos.minY;
				dPos2.minY = -dPos.maxX;
				dPos2.maxX =  dPos.maxY;
				dPos2.maxY = -dPos.minX;
				break;
			default:
				dPos2 = dPos;
				break;
			}
			if ( false == IsExceptionAngle )
			{
				switch ( WndBasicBoxID )
				{
				case WND_BOX_BASIC:
					WndRoiWndCount = WndPtr->GetWndRoiWndCount();
					if ( WndRoiWndCount > 0 ) 
					{	
						WndPtr->GetWndRegion(WndRgn);
						WndPtr->CalcWndRoiWndRegion(WndRoiRgn); 
						WndRgn.Modify(dPos2);
						if ( WndRgn.minX > WndRoiRgn.minX || 
							 WndRgn.minY > WndRoiRgn.minY || 
							 WndRgn.maxX < WndRoiRgn.maxX || 
							 WndRgn.maxY < WndRoiRgn.maxY )
						{	continue;	}
					}
					WndMaskBoxCount = WndPtr->GetWndMaskWndCount();
					if ( WndMaskBoxCount > 0 ) 
					{
						WndPtr->GetWndRegion(WndRgn);
						WndPtr->CalcWndMaskWndRegion(MaskBoxRgn); 
						WndRgn.Modify(dPos2);
						if ( WndRgn.minX > MaskBoxRgn.minX || 
							 WndRgn.minY > MaskBoxRgn.minY || 
							 WndRgn.maxX < MaskBoxRgn.maxX || 
							 WndRgn.maxY < MaskBoxRgn.maxY )
						{	continue;	}
					}
					WndPtr->GetWndBoxPtr()->ModifyBoxRegion(dPos2);	
					WndPtr->UpdateWndExtendBox();					
					break;
				case WND_BOX_EXTEND:	
					WndPtr->GetWndExtendBoxPtr()->ModifyBoxRegion(dPos2);	
					break;
				}				
			}
			else
			{
				WndPtr->RotateWnd(-ComAngle, 0, 0);
				switch ( WndBasicBoxID )
				{
				case WND_BOX_BASIC:
					WndRoiWndCount = WndPtr->GetWndRoiWndCount();
					if ( WndRoiWndCount > 0 ) 
					{	
						WndPtr->GetWndRegion(WndRgn);
						WndPtr->CalcWndRoiWndRegion(WndRoiRgn); 
						WndRgn.Modify(dPos2);
						if ( WndRgn.minX > WndRoiRgn.minX || 
							 WndRgn.minY > WndRoiRgn.minY || 
							 WndRgn.maxX < WndRoiRgn.maxX || 
							 WndRgn.maxY < WndRoiRgn.maxY )
						{
							WndPtr->RotateWnd(ComAngle, 0, 0);
							continue;	
						}
					}
					WndMaskBoxCount = WndPtr->GetWndMaskWndCount();
					if ( WndMaskBoxCount > 0 ) 
					{
						WndPtr->GetWndRegion(WndRgn);
						WndPtr->CalcWndMaskWndRegion(MaskBoxRgn); 
						WndRgn.Modify(dPos2);
						if ( WndRgn.minX > MaskBoxRgn.minX || 
							 WndRgn.minY > MaskBoxRgn.minY || 
							 WndRgn.maxX < MaskBoxRgn.maxX || 
							 WndRgn.maxY < MaskBoxRgn.maxY )
						{
							WndPtr->RotateWnd(ComAngle, 0, 0);
							continue;	
						}
					}
					WndPtr->GetWndBoxPtr()->ModifyBoxRegion(dPos2);	
					WndPtr->UpdateWndExtendBox();					
					break;
				case WND_BOX_EXTEND:	
					WndPtr->GetWndExtendBoxPtr()->ModifyBoxRegion(dPos2);	
					break;
				}				
				WndPtr->RotateWnd(ComAngle, 0, 0);
			}
		}		
	}
	else
	{
		CAOILand  *LandPtr = NULL;
		TPOINT2D   RefLandPos, LandPos, DotPos;
		BOX_TOWARD LandToward = BOX_TOWARD_NULL;
		BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
		const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
		RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefLandPos);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandBoxPtr()->GetBoxPos(LandPos);
			if ( false == LinkLandWndSize )
			{
				if ( LandToward != RefLandToward ) { continue; }
			}

			LandWndCount = LandPtr->GetLandWndCount();
			for ( j=0; j<LandWndCount; j++ )
			{
				WndPtr = LandPtr->GetLandWndPtr(j, false);
				if ( NULL == WndPtr ) { continue; }
				if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }

				WndToward = WndPtr->GetWndToward();
				TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
				if ( WND_SYNC_MOVE_SYMMETRY == RefWndSyncMoveMode )
				{	
					dPos2 = dPos;
					switch ( TowardAngle )
					{
					case  90:
					case 270:
						DotPos.x = RefLandPos.x*LandPos.y;
						DotPos.y = RefLandPos.y*LandPos.x;
						break;
					default:
					case   0:
					case 180:
						DotPos.x = RefLandPos.x*LandPos.x;
						DotPos.y = RefLandPos.y*LandPos.y;
						break;
					}
					if ( DotPos.x < 0 )
					{							
						dPos2.minX = -dPos.maxX;
						dPos2.maxX = -dPos.minX;
					}
					if ( DotPos.y < 0 )
					{	
						dPos2.minY = -dPos.maxY;
						dPos2.maxY = -dPos.minY;
					}
					switch ( TowardAngle )
					{
					case  90:
					case 270:
						JetAPI::Swap(dPos2.minX, dPos2.minY);
						JetAPI::Swap(dPos2.maxX, dPos2.maxY);
						break;
					}			
				}	
				else
				{
					switch ( TowardAngle )
					{
					case  90:
						dPos2.minX = -dPos.maxY;
						dPos2.minY =  dPos.minX;
						dPos2.maxX = -dPos.minY;
						dPos2.maxY =  dPos.maxX;
						break;
					case 180:
						dPos2.minX = -dPos.maxX;
						dPos2.minY = -dPos.maxY;
						dPos2.maxX = -dPos.minX;
						dPos2.maxY = -dPos.minY;
						break;
					case 270:
						dPos2.minX =  dPos.minY;
						dPos2.minY = -dPos.maxX;
						dPos2.maxX =  dPos.maxY;
						dPos2.maxY = -dPos.minX;
						break;
					default:
						dPos2 = dPos;
						break;
					}
				}

				if ( false == IsExceptionAngle )
				{
					switch ( WndBasicBoxID )
					{
					case WND_BOX_BASIC:
						WndRoiWndCount = WndPtr->GetWndRoiWndCount();
						if ( WndRoiWndCount > 0 ) 
						{	
							WndPtr->GetWndRegion(WndRgn);
							WndPtr->CalcWndRoiWndRegion(WndRoiRgn); 
							WndRgn.Modify(dPos2);
							if ( WndRgn.minX > WndRoiRgn.minX || 
								 WndRgn.minY > WndRoiRgn.minY || 
								 WndRgn.maxX < WndRoiRgn.maxX || 
								 WndRgn.maxY < WndRoiRgn.maxY )
							{	continue;	}
						}
						WndMaskBoxCount = WndPtr->GetWndMaskWndCount();
						if ( WndMaskBoxCount > 0 ) 
						{
							WndPtr->GetWndRegion(WndRgn);
							WndPtr->CalcWndMaskWndRegion(MaskBoxRgn); 
							WndRgn.Modify(dPos2);
							if ( WndRgn.minX > MaskBoxRgn.minX || 
								 WndRgn.minY > MaskBoxRgn.minY || 
								 WndRgn.maxX < MaskBoxRgn.maxX || 
								 WndRgn.maxY < MaskBoxRgn.maxY )
							{	continue;	}
						}
						WndPtr->GetWndBoxPtr()->ModifyBoxRegion(dPos2);	
						WndPtr->UpdateWndExtendBox();						
						break;
					case WND_BOX_EXTEND:	
						WndPtr->GetWndExtendBoxPtr()->ModifyBoxRegion(dPos2);	
						break;
					}				
				}
				else
				{
					WndPtr->RotateWnd(-ComAngle, 0, 0);
					switch ( WndBasicBoxID )
					{
					case WND_BOX_BASIC:
						WndRoiWndCount = WndPtr->GetWndRoiWndCount();
						if ( WndRoiWndCount > 0 ) 
						{	
							WndPtr->GetWndRegion(WndRgn);
							WndPtr->CalcWndRoiWndRegion(WndRoiRgn); 
							WndRgn.Modify(dPos2);
							if ( WndRgn.minX > WndRoiRgn.minX || 
								 WndRgn.minY > WndRoiRgn.minY || 
								 WndRgn.maxX < WndRoiRgn.maxX || 
								 WndRgn.maxY < WndRoiRgn.maxY )
							{
								WndPtr->RotateWnd(ComAngle, 0, 0);
								continue;	
							}
						}
						WndMaskBoxCount = WndPtr->GetWndMaskWndCount();
						if ( WndMaskBoxCount > 0 ) 
						{
							WndPtr->GetWndRegion(WndRgn);
							WndPtr->CalcWndMaskWndRegion(MaskBoxRgn); 
							WndRgn.Modify(dPos2);
							if ( WndRgn.minX > MaskBoxRgn.minX || 
								 WndRgn.minY > MaskBoxRgn.minY || 
								 WndRgn.maxX < MaskBoxRgn.maxX || 
								 WndRgn.maxY < MaskBoxRgn.maxY )
							{
								WndPtr->RotateWnd(ComAngle, 0, 0);
								continue;	
							}
						}
						WndPtr->GetWndBoxPtr()->ModifyBoxRegion(dPos2);	
						WndPtr->UpdateWndExtendBox();						
						break;
					case WND_BOX_EXTEND:	
						WndPtr->GetWndExtendBoxPtr()->ModifyBoxRegion(dPos2);	
						break;
					}				
					WndPtr->RotateWnd(ComAngle, 0, 0);		
				}				
			}
		}
	}

	if ( true == bUpdate )
	{
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelLandSize(CAOIBox *RefBoxPtr, CAOILand *RefLandPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate)//修正模組特徵框的尺寸
{
	if ( NULL == RefBoxPtr ) { return false; }
	if ( NULL == RefLandPtr ) { return false; }

	LAND_TYPE RefLandType = RefLandPtr->GetLandType();
	const int BasicBoxID = RefLandPtr->GetLandBasicBoxID(RefBoxPtr);
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	const bool   LinkLandSize = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_LAND_SIZE);	

	size_t     i=0, j=0;
	int        TowardAngle = 0;
	TREGION4D  dPos2;
	CAOIBox   *BoxPtr = NULL;
	CAOILand  *LandPtr = NULL;
	BOX_TOWARD LandToward = BOX_TOWARD_NULL;		
	const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
	BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
		if ( false == LinkLandSize )
		{
			if ( LandPtr->GetLandToward() != RefLandToward ) { continue; }//單邊連動
		}		
		BoxPtr = LandPtr->GetLandBasicBoxPtr(BasicBoxID);			
		if ( NULL == BoxPtr ) { continue; }

		LandToward = LandPtr->GetLandToward();
		TowardAngle = CAOIBox::CalcBoxTowardAngle(RefLandToward, LandToward);
		switch ( TowardAngle )
		{
		case  90:
			dPos2.minX = -dPos.maxY;
			dPos2.minY =  dPos.minX;
			dPos2.maxX = -dPos.minY;
			dPos2.maxY =  dPos.maxX;
			break;
		case 180:
			dPos2.minX = -dPos.maxX;
			dPos2.minY = -dPos.maxY;
			dPos2.maxX = -dPos.minX;
			dPos2.maxY = -dPos.minY;
			break;
		case 270:
			dPos2.minX =  dPos.minY;
			dPos2.minY = -dPos.maxX;
			dPos2.maxX =  dPos.maxY;
			dPos2.maxY = -dPos.minX;
			break;
		default:
			dPos2 = dPos;
			break;
		}
				
		if ( false == IsExceptionAngle )
		{
			switch ( BasicBoxID )
			{
			case LAND_BOX_PAD:
				BoxPtr->ModifyBoxRegion(dPos2);
				break;
			case LAND_BOX_LEAD://Lead
			case LAND_BOX_LEAD_TIP://Lead Tip				
			case LAND_BOX_LEAD_SHOULDER://Lead Shoulder
				LandPtr->ModifyLandLeadSize(BoxPtr, dPos2);
				break;
			default:
				BoxPtr->ModifyBoxRegion(dPos2);
				break;
			}	
		}
		else
		{
			LandPtr->RotateLand(-ComAngle, 0, 0);
			switch ( BasicBoxID )
			{
			case LAND_BOX_PAD:
				BoxPtr->ModifyBoxRegion(dPos2);
				break;
			case LAND_BOX_LEAD://Lead
			case LAND_BOX_LEAD_TIP://Lead Tip
			case LAND_BOX_LEAD_SHOULDER://Lead Shoulder
				LandPtr->ModifyLandLeadSize(BoxPtr, dPos2);
				break;
			default:
				BoxPtr->ModifyBoxRegion(dPos2);
				break;
			}
			LandPtr->RotateLand(ComAngle, 0, 0);
		}
	}
	/*
	if ( false == LinkLandWndSize )
	{
		if ( false == IsExceptionAngle )
		{
			switch ( BasicBoxID )
			{
			case LAND_BOX_PAD:				
				RefBoxPtr->ModifyBoxRegion(dPos);
				break;
			case LAND_BOX_LEAD://Lead
			case LAND_BOX_LEAD_TIP://Lead Tip
			case LAND_BOX_LEAD_SHOULDER://Lead Shoulder
				RefLandPtr->ModifyLandLeadSize(RefBoxPtr, dPos);
				break;
			default:				
				RefBoxPtr->ModifyBoxRegion(dPos);
				break;
			}			
		}
		else
		{
			RefLandPtr->RotateLand(-ComAngle, 0, 0);
			switch ( BasicBoxID )
			{
			case LAND_BOX_PAD:
				RefBoxPtr->ModifyBoxRegion(dPos);
				break;
			case LAND_BOX_LEAD://Lead
			case LAND_BOX_LEAD_TIP://Lead Tip
			case LAND_BOX_LEAD_SHOULDER://Lead Shoulder
				RefLandPtr->ModifyLandLeadSize(RefBoxPtr, dPos);
				break;
			default:
				RefBoxPtr->ModifyBoxRegion(dPos);
				break;
			}
			RefLandPtr->RotateLand(ComAngle, 0, 0);
		}	
	}
	else
	{		
	}
	*/

	if ( CheckLandBasicBoxIDLinkBody(BasicBoxID) == true )
	{	UpdateModelBodyRgnFromChipLead();	}	

	if ( true == bUpdate )
	{
		UpdateModelWndRgnByLinkMode();
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelWndRoiSize(CAOIBox *RefBoxPtr, CAOIWnd *RefWndPtr, CAOIWndRoi *RefWndRoiPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate)//修正模組內檢測框子框的尺寸
{
	if ( NULL == RefBoxPtr ) { return false; }
	if ( NULL == RefWndPtr ) { return false; }	
	if ( NULL == RefWndRoiPtr ) { return false; }	

	CAOILand *RefLandPtr = RefWndPtr->GetWndLandPtr();
	const int WndBasicBoxID = RefWndPtr->GetWndBasicBoxID(RefBoxPtr);
	const unsigned int RefWndRoiIndex = RefWndRoiPtr->GetWndRoiIndex();
	const double ComAngle = CAOIModel::GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	const bool   LinkBodyWndSize = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_BODY_WND_SIZE);
	const bool   LinkLandWndSize = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_LAND_WND_SIZE);	
	
	size_t       i=0, j=0, k=0;
	size_t       LandWndCount = 0;
	int          TowardAngle = 0;	
	TREGION4D    dPos2;
	TREGION4D    WndRgn;
	TREGION4D    WndRoiRgn;
	TPOINT2D     WndCornerPt[4];
	TPOINT2D     WndRoiCornerPt[4];
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;	
	BOX_TOWARD   WndToward = BOX_TOWARD_NULL;	
	BOX_TOWARD   RefWndToward = RefWndPtr->GetWndToward();
	const int    RefWndBandID = RefWndPtr->GetWndBandID();
	const int    RefWndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();	

	unsigned int WndRoiIndex=0;
	std::vector<CAOIWndRoi*> WndRoiList;
	std::vector<unsigned int> WndRoiIndexList;
	RefWndPtr->GetWndRoiWndSelectedList(WndRoiList);
	const size_t WndRoiWndCount = WndRoiList.size();
	for ( i=0; i<WndRoiWndCount; i++ )
	{
		WndRoiPtr = WndRoiList[i];
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiIndex = WndRoiPtr->GetWndRoiIndex();
		WndRoiIndexList.push_back(WndRoiIndex);
	}
	const size_t WndRoiIndexCount = WndRoiIndexList.size();

	if ( NULL == RefLandPtr )
	{	
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }
			for ( k=0; k<WndRoiIndexCount; k++ )
			{
				WndRoiIndex = WndRoiIndexList[k];				
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(WndRoiIndex, true);
				//WndRoiPtr = WndPtr->GetWndRoiWndPtr(RefWndRoiIndex, true);
				if ( NULL == WndRoiPtr ) { continue; }	

				WndToward = WndPtr->GetWndToward();
				if ( false == LinkBodyWndSize )
				{
					if ( WndToward != RefWndToward ) { continue; }
				}			
				TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
				switch ( TowardAngle )
				{
				case  90:
					dPos2.minX = -dPos.maxY;
					dPos2.minY =  dPos.minX;
					dPos2.maxX = -dPos.minY;
					dPos2.maxY =  dPos.maxX;
					break;
				case 180:
					dPos2.minX = -dPos.maxX;
					dPos2.minY = -dPos.maxY;
					dPos2.maxX = -dPos.minX;
					dPos2.maxY = -dPos.minY;					
					break;
				case 270:
					dPos2.minX =  dPos.minY;
					dPos2.minY = -dPos.maxX;
					dPos2.maxX =  dPos.maxY;
					dPos2.maxY = -dPos.minX;
					break;
				default:
					dPos2 = dPos;
					break;
				}

				if ( false == IsExceptionAngle )
				{
					WndPtr->GetWndRegion(WndRgn);
					WndRoiPtr->GetWndRoiRegion(WndRoiRgn); 
				}
				else
				{
					WndPtr->GetWndCornerPos(WndCornerPt);
					WndRoiPtr->GetWndRoiCornerPos(WndRoiCornerPt);				
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndRoiCornerPt);
					JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);
					JetAPI::CornerPtToRegion(WndRoiCornerPt, WndRoiRgn);
				}
				WndRoiRgn.Modify(dPos2);
				if ( WndRoiRgn.minX < WndRgn.minX ||
					 WndRoiRgn.minY < WndRgn.minY ||
					 WndRoiRgn.maxX > WndRgn.maxX ||
					 WndRoiRgn.maxY > WndRgn.maxY 
					 )
				{	continue; }

				BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
				if ( false == IsExceptionAngle )
				{	BoxPtr->ModifyBoxRegion(dPos2);	}
				else
				{
					BoxPtr->RotateBox(-ComAngle, 0, 0);
					BoxPtr->ModifyBoxRegion(dPos2);
					BoxPtr->RotateBox(ComAngle, 0, 0);
				}
			}
		}		
	}
	else
	{
		CAOILand  *LandPtr = NULL;
		BOX_TOWARD LandToward = BOX_TOWARD_NULL;
		BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
		const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
			LandToward = LandPtr->GetLandToward();
			if ( false == LinkLandWndSize )
			{
				if ( LandToward != RefLandToward ) { continue; }
			}

			LandWndCount = LandPtr->GetLandWndCount();
			for ( j=0; j<LandWndCount; j++ )
			{
				WndPtr = LandPtr->GetLandWndPtr(j, false);
				if ( NULL == WndPtr ) { continue; }
				if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }
				for ( k=0; k<WndRoiIndexCount; k++ )
				{
					WndRoiIndex = WndRoiIndexList[k];					
					WndRoiPtr = WndPtr->GetWndRoiWndPtr(WndRoiIndex, true);
					//WndRoiPtr = WndPtr->GetWndRoiWndPtr(RefWndRoiIndex, true);
					if ( NULL == WndRoiPtr ) { continue; }			

					WndToward = WndPtr->GetWndToward();
					TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
					switch ( TowardAngle )
					{
					case  90:
						dPos2.minX = -dPos.maxY;
						dPos2.minY =  dPos.minX;
						dPos2.maxX = -dPos.minY;
						dPos2.maxY =  dPos.maxX;
						break;
					case 180:
						dPos2.minX = -dPos.maxX;
						dPos2.minY = -dPos.maxY;
						dPos2.maxX = -dPos.minX;
						dPos2.maxY = -dPos.minY;
						break;
					case 270:
						dPos2.minX =  dPos.minY;
						dPos2.minY = -dPos.maxX;
						dPos2.maxX =  dPos.maxY;
						dPos2.maxY = -dPos.minX;
						break;
					default:
						dPos2 = dPos;
						break;
					}

					if ( false == IsExceptionAngle )
					{
						WndPtr->GetWndRegion(WndRgn);
						WndRoiPtr->GetWndRoiRegion(WndRoiRgn); 
					}
					else
					{
						WndPtr->GetWndCornerPos(WndCornerPt);
						WndRoiPtr->GetWndRoiCornerPos(WndRoiCornerPt);					
						JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndCornerPt);
						JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndRoiCornerPt);
						JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);
						JetAPI::CornerPtToRegion(WndRoiCornerPt, WndRoiRgn);
					}
					WndRoiRgn.Modify(dPos2);
					if ( WndRoiRgn.minX < WndRgn.minX ||
						 WndRoiRgn.minY < WndRgn.minY ||
						 WndRoiRgn.maxX > WndRgn.maxX ||
						 WndRoiRgn.maxY > WndRgn.maxY 
						 )
					{	continue; }

					BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
					if ( false == IsExceptionAngle )
					{	BoxPtr->ModifyBoxRegion(dPos2);		}
					else
					{
						BoxPtr->RotateBox(-ComAngle, 0, 0);
						BoxPtr->ModifyBoxRegion(dPos2);
						BoxPtr->RotateBox(ComAngle, 0, 0);		
					}
				}
			}
		}
	}

	if ( true == bUpdate )
	{
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelWndMaskSize(CAOIBox *RefBoxPtr, CAOIWnd *RefWndPtr, CAOIWndMask *RefMaskWndPtr, const TREGION4D &dPos, int LinkMode, bool bUpdate)//修正模組內檢測框遮罩框的尺寸
{
	if ( NULL == RefBoxPtr ) { return false; }
	if ( NULL == RefWndPtr ) { return false; }	
	if ( NULL == RefMaskWndPtr ) { return false; }	

	const double ComAngle = GetModelAttachedAngle();
	CAOILand *RefLandPtr = RefWndPtr->GetWndLandPtr();
	const int WndBasicBoxID = RefWndPtr->GetWndBasicBoxID(RefBoxPtr);
	const unsigned int RefMaskWndIndex = RefMaskWndPtr->GetWndMaskIndex();	
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	const bool   LinkBodyWndSize = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_BODY_WND_SIZE);
	const bool   LinkLandWndSize = JetAPI::BitMask_Check(LinkMode, MODEL_LINK_LAND_WND_SIZE);	
	
	size_t       i=0, j=0, k=0;
	size_t       LandWndCount = 0;
	int          TowardAngle = 0;	
	TREGION4D    dPos2;
	TREGION4D    WndRgn;
	TREGION4D    MaskWndRgn;
	TPOINT2D     WndCornerPt[4];
	TPOINT2D     MaskWndCornerPt[4];
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOIWndMask *MaskWndPtr = NULL;
	BOX_TOWARD   WndToward = BOX_TOWARD_NULL;
	BOX_TOWARD   RefWndToward = RefWndPtr->GetWndToward();
	const int    RefWndBandID = RefWndPtr->GetWndBandID();
	const int    RefWndGroupID = RefWndPtr->GetWndGroupID();
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();

	size_t MaskWndIndex=0;
	std::vector<size_t> MaskWndIndexList;
	RefWndPtr->GetWndMaskWndSelectedList(MaskWndIndexList);	
	const size_t MaskWndIndexCount = MaskWndIndexList.size();
	if ( 0 == MaskWndIndexCount ) { return true; }

	if ( NULL == RefLandPtr )
	{	
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = GetModelWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }
			WndToward = WndPtr->GetWndToward();
			if ( false == LinkBodyWndSize )
			{
				if ( WndToward != RefWndToward ) { continue; }
			}
			if ( false == IsExceptionAngle )
			{	WndPtr->GetWndRegion(WndRgn);	}
			else
			{
				WndPtr->GetWndCornerPos(WndCornerPt);
				JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndCornerPt);
				JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);
			}
			TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
			switch ( TowardAngle )
			{
			case  90:
				dPos2.minX = -dPos.maxY;
				dPos2.minY =  dPos.minX;
				dPos2.maxX = -dPos.minY;
				dPos2.maxY =  dPos.maxX;
				break;
			case 180:
				dPos2.minX = -dPos.maxX;
				dPos2.minY = -dPos.maxY;
				dPos2.maxX = -dPos.minX;
				dPos2.maxY = -dPos.minY;					
				break;
			case 270:
				dPos2.minX =  dPos.minY;
				dPos2.minY = -dPos.maxX;
				dPos2.maxX =  dPos.maxY;
				dPos2.maxY = -dPos.minX;
				break;
			default:
				dPos2 = dPos;
				break;
			}
			for ( k=0; k<MaskWndIndexCount; k++ )
			{
				MaskWndIndex = MaskWndIndexList[k];
				MaskWndPtr = WndPtr->GetWndMaskWndPtr(MaskWndIndex, true);
				//MaskWndPtr = WndPtr->GetWndMaskWndPtr(RefMaskBoxIndex, true);
				if ( NULL == MaskWndPtr ) { continue; }	
				if ( false == IsExceptionAngle )
				{	MaskWndPtr->GetWndMaskRegion(MaskWndRgn);	}
				else
				{					
					MaskWndPtr->GetWndMaskCornerPos(MaskWndCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, MaskWndCornerPt);
					JetAPI::CornerPtToRegion(MaskWndCornerPt, MaskWndRgn);
				}
				MaskWndRgn.Modify(dPos2);
				if ( MaskWndRgn.minX < WndRgn.minX ||
					 MaskWndRgn.minY < WndRgn.minY ||
					 MaskWndRgn.maxX > WndRgn.maxX ||
					 MaskWndRgn.maxY > WndRgn.maxY 
					 )
				{	continue; }

				BoxPtr = MaskWndPtr->GetWndMaskBoxPtr();
				if ( false == IsExceptionAngle )
				{	BoxPtr->ModifyBoxRegion(dPos2);	}
				else
				{
					BoxPtr->RotateBox(-ComAngle, 0, 0);
					BoxPtr->ModifyBoxRegion(dPos2);
					BoxPtr->RotateBox(ComAngle, 0, 0);
				}
			}
		}		
	}
	else
	{
		CAOILand  *LandPtr = NULL;
		BOX_TOWARD LandToward = BOX_TOWARD_NULL;
		BOX_TOWARD RefLandToward = RefLandPtr->GetLandToward();
		const int  RefLandGroupID = RefLandPtr->GetLandGroupID();
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != RefLandGroupID ) { continue; }
			LandToward = LandPtr->GetLandToward();
			if ( false == LinkLandWndSize )
			{
				if ( LandToward != RefLandToward ) { continue; }
			}

			LandWndCount = LandPtr->GetLandWndCount();
			for ( j=0; j<LandWndCount; j++ )
			{
				WndPtr = LandPtr->GetLandWndPtr(j, false);
				if ( NULL == WndPtr ) { continue; }
				if ( WndPtr->GetWndGroupID() != RefWndGroupID ) { continue; }
				WndToward = WndPtr->GetWndToward();
				if ( false == IsExceptionAngle )
				{	WndPtr->GetWndRegion(WndRgn);	}
				else
				{
					WndPtr->GetWndCornerPos(WndCornerPt);
					JetAPI::RotateCornerPos(-ComAngle, 0, 0, WndCornerPt);
					JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);					
				}
				TowardAngle = CAOIBox::CalcBoxTowardAngle(RefWndToward, WndToward);
				switch ( TowardAngle )
				{
				case  90:
					dPos2.minX = -dPos.maxY;
					dPos2.minY =  dPos.minX;
					dPos2.maxX = -dPos.minY;
					dPos2.maxY =  dPos.maxX;
					break;
				case 180:
					dPos2.minX = -dPos.maxX;
					dPos2.minY = -dPos.maxY;
					dPos2.maxX = -dPos.minX;
					dPos2.maxY = -dPos.minY;
					break;
				case 270:
					dPos2.minX =  dPos.minY;
					dPos2.minY = -dPos.maxX;
					dPos2.maxX =  dPos.maxY;
					dPos2.maxY = -dPos.minX;
					break;
				default:
					dPos2 = dPos;
					break;
				}
				for ( k=0; k<MaskWndIndexCount; k++ )
				{
					MaskWndIndex = MaskWndIndexList[k];
					MaskWndPtr = WndPtr->GetWndMaskWndPtr(MaskWndIndex, true);
					//MaskWndPtr = WndPtr->GetWndMaskWndPtr(RefMaskBoxIndex, true);
					if ( NULL == MaskWndPtr ) { continue; }
					if ( false == IsExceptionAngle )
					{	MaskWndPtr->GetWndMaskRegion(MaskWndRgn);	}
					else
					{
						MaskWndPtr->GetWndMaskCornerPos(MaskWndCornerPt);
						JetAPI::RotateCornerPos(-ComAngle, 0, 0, MaskWndCornerPt);
						JetAPI::CornerPtToRegion(MaskWndCornerPt, MaskWndRgn);
					}
					MaskWndRgn.Modify(dPos2);
					if ( MaskWndRgn.minX < WndRgn.minX ||
						 MaskWndRgn.minY < WndRgn.minY ||
						 MaskWndRgn.maxX > WndRgn.maxX ||
						 MaskWndRgn.maxY > WndRgn.maxY 
						 )
					{	continue; }

					BoxPtr = MaskWndPtr->GetWndMaskBoxPtr();
					if ( false == IsExceptionAngle )
					{	BoxPtr->ModifyBoxRegion(dPos2);		}
					else
					{
						BoxPtr->RotateBox(-ComAngle, 0, 0);
						BoxPtr->ModifyBoxRegion(dPos2);
						BoxPtr->RotateBox(ComAngle, 0, 0);		
					}
				}
			}
		}
	}

	if ( true == bUpdate )
	{
		CalcModelTotalRegionAll();
		UpdateModelRegionToAttached();
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckLandBasicBoxIDLinkBody(int ID) const//確認特徵框基本框是否與本體有關
{
	if ( LAND_BOX_PAD == ID ) { return false; }
	//LAND_BOX_LEAD
	//LAND_BOX_LEAD_TIP
	//LAND_BOX_LEAD_SHOULDER
	//LAND_BOX_BODY_EDGE	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelBodyRgnFromChipLead()//更新模組本體尺寸-來自被動元件電極
{
	if ( CheckModelBodyLinkChipLead() == false ) { return true; }	
	const size_t LandCount=GetModelLandCount();
	if ( 0 == LandCount ) { return true; }
	
	bool bSucc=false;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( false == IsExceptionAngle )
	{	bSucc = UpdateModelBodyRgnFromChipLeadKernel();	}
	else
	{
		double CPX = 0;
		double CPY = 0;
		TREGION4D ModelRgn;
		GetModelTotalRegion(ModelRgn);
		const double RgnCpX = ModelRgn.GetCpX();
		const double RgnCpY = ModelRgn.GetCpY();		
		RotateModel(-AttachedAngle, CPX, CPY);
		bSucc = UpdateModelBodyRgnFromChipLeadKernel();
		RotateModel(AttachedAngle, CPX, CPY);
	}	
	return bSucc;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelBodyRgnFromChipLeadKernel()//更新模組本體尺寸-來自被動元件電極
{
	if ( CheckModelBodyLinkChipLead() == false ) { return true; }	
	const size_t LandCount=GetModelLandCount();
	if ( 0 == LandCount ) { return true; }
	
	BOX_TOWARD ModelToward=GetModelBodyBox().GetBoxToward();	
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( 1 == LandCount )
	{
		CAOILand *LandPtr = GetModelLandPtr(0, true);
		if ( NULL == LandPtr ) { return true; }		
		BOX_TOWARD LandToward=LandPtr->GetLandToward();				
		const int Angle=CAOIBox::CalcBoxTowardAngle(LandToward, ModelToward);		
		if ( 0 == Angle || 180==Angle )
		{	
			TREGION4D  BodyRgn, LeadRgn, TempRgn;
			GetModelBodyBox().GetBoxRegion(BodyRgn);
			LandPtr->GetLandLeadBox().GetBoxRegion(LeadRgn);		
			TempRgn = BodyRgn;
			BodyRgn = LeadRgn;
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:		BodyRgn.minY=TempRgn.minY;	break;
			case BOX_TOWARD_LEFT:	BodyRgn.maxX=TempRgn.maxX;	break;
			case BOX_TOWARD_DOWN:	BodyRgn.maxY=TempRgn.maxY;	break;
			case BOX_TOWARD_RIGHT:	BodyRgn.minX=TempRgn.minX;	break;				
			}
			GetModelBodyBox().SetBoxRegion(BodyRgn);
		}		
		return true;
	}	
	else
	{
		bool bUpdate=false;
		TREGION4D LeadRgn, BodyRgn;
		for ( size_t i=0; i<LandCount; i++ )
		{
			CAOILand *LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			BOX_TOWARD LandToward=LandPtr->GetLandToward();
			const int Angle=CAOIBox::CalcBoxTowardAngle(LandToward, ModelToward);		
			if ( 0 == Angle || 180==Angle )
			{	
				LandPtr->GetLandLeadBox().GetBoxRegion(LeadRgn);
				if ( false == bUpdate )
				{
					bUpdate = true;
					BodyRgn = LeadRgn;
					continue;
				}				
				JetAPI::UnionRegion(BodyRgn, LeadRgn, BodyRgn);
			}			
		}
		if ( true == bUpdate )
		{	GetModelBodyBox().SetBoxRegion(BodyRgn);	}		
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelChipLeadRgnFromBody()//更新模組被動元件電極尺寸-來自本體
{
	if ( CheckModelBodyLinkChipLead() == false ) { return true; }	
	const size_t LandCount=GetModelLandCount();
	if ( 0 == LandCount ) { return true; }	
	bool bSucc=false;
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( false == IsExceptionAngle )
	{	bSucc = UpdateModelChipLeadRgnFromBodyKernel();	}
	else
	{
		double CPX = 0;
		double CPY = 0;
		TREGION4D ModelRgn;
		GetModelTotalRegion(ModelRgn);
		const double RgnCpX = ModelRgn.GetCpX();
		const double RgnCpY = ModelRgn.GetCpY();		
		RotateModel(-AttachedAngle, CPX, CPY);
		bSucc = UpdateModelChipLeadRgnFromBodyKernel();
		RotateModel(AttachedAngle, CPX, CPY);
	}
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelChipLeadRgnFromBodyKernel()//更新模組被動元件電極尺寸-來自本體
{
	if ( CheckModelBodyLinkChipLead() == false ) { return true; }	
	const size_t LandCount=GetModelLandCount();
	if ( 0 == LandCount ) { return true; }		
	bool bUpdate=false;
	TSIZE2D   LaadSz;
	TPOINT2D  Offset;	
	TPOINT2D  BodyCp;	
	TPOINT2D  LeadCp;
	TREGION4D TempRgn;
	TREGION4D LeadRgn;
	TREGION4D BodyRgn;
	CAOILand *LandPtr=NULL;	
	BOX_TOWARD LandToward;
	BOX_TOWARD ModelToward=GetModelBodyBox().GetBoxToward();	
	GetModelBodyBox().GetBoxRegion(BodyRgn);	
	BodyCp.x = BodyRgn.GetCpX();
	BodyCp.y = BodyRgn.GetCpY();		
	for ( size_t i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandToward = LandPtr->GetLandToward();
		CAOIBox &LeadBox=LandPtr->GetLandLeadBox();
		LeadBox.GetBoxRegion(LeadRgn);									
		LeadCp.x = LeadRgn.GetCpX();
		LeadCp.y = LeadRgn.GetCpY();
		LaadSz.cx = LeadRgn.GetSizeX();
		LaadSz.cy = LeadRgn.GetSizeY();
		Offset.x = BodyCp.x-LeadCp.x;
		Offset.y = BodyCp.y-LeadCp.y;

		bUpdate = false;
		TempRgn = LeadRgn;
		LeadRgn = BodyRgn;
		switch ( ModelToward )
		{
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			switch ( LandToward )
			{
			case BOX_TOWARD_LEFT:
				bUpdate = true;
				Offset.x=BodyRgn.minX-TempRgn.minX;
				LeadRgn.maxX = LeadRgn.minX+LaadSz.cx;					
				break;
			case BOX_TOWARD_RIGHT:
				bUpdate = true;
				Offset.x=BodyRgn.maxX-TempRgn.maxX;
				LeadRgn.minX = LeadRgn.maxX-LaadSz.cx;					
				break;
			}
			break;
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				bUpdate = true;
				Offset.y=BodyRgn.maxY-TempRgn.maxY;
				LeadRgn.minY = LeadRgn.maxY-LaadSz.cy;					
				break;
			case BOX_TOWARD_DOWN:
				bUpdate = true;
				Offset.y=BodyRgn.minY-TempRgn.minY;
				LeadRgn.maxY = LeadRgn.minY+LaadSz.cy;					
				break;
			}				
			break;
		}
		if ( true == bUpdate )
		{				
			LandPtr->MoveLand(Offset.x, Offset.y);				
			LeadBox.SetBoxRegion(LeadRgn);
			LandPtr->UpdateLandTotalRegion();
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelExtendAutoAdjust() const//設定模組外擴自動調整
{
	return m_ModelExtendAutoAdjust;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelExtendAutoAdjust(bool val)//取得模組外擴自動調整
{	
	if ( m_ModelExtendAutoAdjust != val )
	{	
		m_ModelExtendAutoAdjust = val;
		SetModelModifiedCount(true);
		SetModelNeedSaveFiles(true);
	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelExtendRangeX(double value)
{ 
	m_ModelExtendRange.cx=value; 
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelExtendRangeY(double value)
{ 
	m_ModelExtendRange.cy=value; 
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelExtendRange(const TSIZE2D &value)
{ 
	m_ModelExtendRange=value; 
	SetModelModifiedCount(true);
	SetModelNeedSaveFiles(true);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelWndMaxExtendRange(double &ExtendX, double &ExtendY)//計算模組內檢測框最大外擴範圍
{
	size_t       i=0;
	bool         bFirst=true;
	bool         bLocateWnd=false;
	double       TempX=0;
	double       TempY=0;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;		
	WND_DEFECT_ID WndDefectID;
	const size_t WndCount = GetModelWndCount();

	bFirst = true;
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndExtendBoxUsed() == false )	{	continue;	}
		
		//只比較可以定位的檢測框		
		WndDefectID = WndPtr->GetWndDefectID();
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:
		case WND_DEFECT_PART_ALIGN:
		case WND_DEFECT_PAD_ADJUST:
		case WND_DEFECT_LEAD_ADJUST:
			bLocateWnd = true;
			break;
		default:
			bLocateWnd = false;
			break;
		}
		if ( false == bLocateWnd ) { continue; }

		TempX = WndPtr->GetWndExtendRangeX();
		TempY = WndPtr->GetWndExtendRangeY();
		if ( true == bFirst )
		{
			ExtendX = TempX;
			ExtendY = TempY;
			bFirst = false;
			continue;
		}
		if ( TempX > ExtendX ) { ExtendX = TempX; }
		if ( TempY > ExtendY ) { ExtendY = TempY; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelTotalRegionExtend(TREGION4D &Rgn)//計算模組的額外外擴範圍
{	
	const double ExtRatio = 0.4;
	const double ExtMaxW = m_ModelExtendRange.cx;
	const double ExtMaxH = m_ModelExtendRange.cy;
	const double RgnCpx = Rgn.GetCpX();
	const double RgnCpy = Rgn.GetCpY();
	const double RgnW = Rgn.GetWidth();
	const double RgnH = Rgn.GetHeight();	
	double ExtW = RgnW*ExtRatio;
	double ExtH = RgnH*ExtRatio;
	if ( ExtW > ExtMaxW ) { ExtW = ExtMaxW; } 
	if ( ExtH > ExtMaxH ) { ExtH = ExtMaxH; } 

	//至少要比檢測框的外擴範圍大	
	double MaxWndExtX=0;
	double MaxWndExtY=0;
	const bool ExtendAutoAdjust=GetModelExtendAutoAdjust();
	if ( true == ExtendAutoAdjust )
	{
		CalcModelWndMaxExtendRange(MaxWndExtX, MaxWndExtY);
		if ( ExtW<MaxWndExtX ) { ExtW = MaxWndExtX; }
		if ( ExtH<MaxWndExtY ) { ExtH = MaxWndExtY; }
	}

	const double RgnWF = RgnW+ExtW+ExtW;
	const double RgnHF = RgnH+ExtH+ExtH;
	Rgn.minX = RgnCpx-(RgnWF*0.5);
	Rgn.minY = RgnCpy-(RgnHF*0.5);
	Rgn.maxX = Rgn.minX+RgnWF;
	Rgn.maxY = Rgn.minY+RgnHF;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelTotalRegionAll()//計算模組整個範圍	
{
	bool IsOK = true;	
	const double ComponentAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	if ( IsExceptionAngle == false )
	{
		IsOK = CalcModelTotalRegionKernel();		
	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		RotateModel(-ComponentAngle, CPX, CPY);
		IsOK = CalcModelTotalRegionKernel();		
		RotateModel(ComponentAngle, CPX, CPY);
	}	

	//TSIZE2D RgnSize;
	//RgnSize.cx = m_ModelTotalRgn.GetSizeX();
	//RgnSize.cy = m_ModelTotalRgn.GetSizeY();	
	//SetModelImageSize_um(RgnSize);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelTotalRegionKernel()
{
	size_t       i=0, j=0;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    Region;
	TREGION4D    Rgn, RgnBody, RgnLand, RgnWnd;
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();

	BoxPtr = GetModelBodyBoxPtr();
	BoxPtr->GetBoxRegion(RgnBody);
	Rgn = Region = RgnBody;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }

		BoxPtr = LandPtr->GetLandPadBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegion(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}
	}
	m_ModelBodyLandRgn = Rgn;

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndExtendBoxUsed() == false )
		{	BoxPtr = WndPtr->GetWndBoxPtr();	}
		else
		{	BoxPtr = WndPtr->GetWndExtendBoxPtr(); }
		//BoxPtr = WndPtr->GetWndBoxPtr();
		BoxPtr->GetBoxRegion(RgnLand);
		JetAPI::UnionRegion(Rgn, RgnLand, Region);
		Rgn = Region;
	}
	
	CalcModelTotalRegionExtend(Rgn);
	m_ModelTotalRgn = Rgn;
	m_ModelTotalRgnRaw = Rgn;
	UpdateModelTotalRegionToAttached();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelTotalRegionToAttached()//更新模組的全區域至零件位置上
{
	TREGION4D  Rgn = m_ModelTotalRgn;
	const double RgnCpx = Rgn.GetCpX();
	const double RgnCpy = Rgn.GetCpY();
	const double RgnW = Rgn.GetWidth();
	const double RgnH = Rgn.GetHeight();
	m_ModelTotalCornerPts[0].x = RgnCpx-(RgnW/2);
	m_ModelTotalCornerPts[0].y = RgnCpy-(RgnH/2);
	m_ModelTotalCornerPts[1].x = RgnCpx+(RgnW/2);
	m_ModelTotalCornerPts[1].y = RgnCpy-(RgnH/2);
	m_ModelTotalCornerPts[2].x = RgnCpx+(RgnW/2);
	m_ModelTotalCornerPts[2].y = RgnCpy+(RgnH/2);
	m_ModelTotalCornerPts[3].x = RgnCpx-(RgnW/2);
	m_ModelTotalCornerPts[3].y = RgnCpy+(RgnH/2);

	m_ModelBodyLandRgn.GetCornerPts(m_ModelBodyLandCornerPts);	

	JetAPI::MoveRegion(Rgn, m_ModelAttachedCadPos, m_ModelTotalRgnCad);
	JetAPI::MoveCornerPts(m_ModelTotalCornerPts, m_ModelAttachedCadPos, m_ModelTotalCornerPtsCad);

	TREGION4D StageRgn;
	AOIDataCollect.MapCadOffsetRgnToStage(Rgn, StageRgn);
	AOIDataCollect.MapCadOffsetCornerPtsToStage(m_ModelTotalCornerPts, m_ModelTotalCornerPtsStage);	
	JetAPI::MoveRegion(StageRgn, m_ModelAttachedStagePos, m_ModelTotalRgnStage);
	JetAPI::MoveCornerPts(m_ModelTotalCornerPtsStage, m_ModelAttachedStagePos, m_ModelTotalCornerPtsStage);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ModifyModelTotalRegionSize(double W, double H)//修改模組整個範圍
{
	m_ModelTotalRgn.SetSize(W, H);
	m_ModelTotalRgnCad.SetSize(W, H);
	m_ModelTotalRgnStage.SetSize(W, H);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTotalRegion(TREGION4D &Region, bool bRaw) const//取得模組整個範圍
{
	if ( bRaw )
	{	Region = m_ModelTotalRgnRaw;	}
	else
	{	Region = m_ModelTotalRgn;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelTotalRegionCadKernel()
{
	size_t       i=0, j=0;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    Region;
	TREGION4D    Rgn, RgnBody, RgnLand, RgnWnd;
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();

	BoxPtr = GetModelBodyBoxPtr();
	BoxPtr->GetBoxRegionCad(RgnBody);
	Rgn = Region = RgnBody;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }

		BoxPtr = LandPtr->GetLandPadBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionCad(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionCad(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionCad(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionCad(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionCad(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}
	}

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndExtendBoxUsed() == false )
		{	BoxPtr = WndPtr->GetWndBoxPtr();	}
		else
		{	BoxPtr = WndPtr->GetWndExtendBoxPtr(); }
		//BoxPtr = WndPtr->GetWndBoxPtr();
		BoxPtr->GetBoxRegionCad(RgnLand);
		JetAPI::UnionRegion(Rgn, RgnLand, Region);
		Rgn = Region;
	}

	CalcModelTotalRegionExtend(Rgn);
	m_ModelTotalRgnCad = Rgn;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTotalRegionCad(TREGION4D &Region) const//取得模組整個範圍
{
	Region = m_ModelTotalRgnCad;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelTotalRegionStageKernel()
{
	size_t       i=0, j=0;
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;	
	TREGION4D    Region;
	TREGION4D    Rgn, RgnBody, RgnLand, RgnWnd;	
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();

	BoxPtr = GetModelBodyBoxPtr();
	BoxPtr->GetBoxRegionStage(RgnBody);
	Rgn = Region = RgnBody;
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }

		BoxPtr = LandPtr->GetLandPadBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionStage(RgnLand);			
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionStage(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionStage(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionStage(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}

		BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();
		if ( BoxPtr->GetBoxEnabled() == true )
		{
			BoxPtr->GetBoxRegionStage(RgnLand);
			JetAPI::UnionRegion(Rgn, RgnLand, Region);
			Rgn = Region;
		}
	}

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndExtendBoxUsed() == false )
		{	BoxPtr = WndPtr->GetWndBoxPtr();	}
		else
		{	BoxPtr = WndPtr->GetWndExtendBoxPtr(); }
		//BoxPtr = WndPtr->GetWndBoxPtr();
		BoxPtr->GetBoxRegionStage(RgnLand);
		JetAPI::UnionRegion(Rgn, RgnLand, Region);
		Rgn = Region;
	}
	CalcModelTotalRegionExtend(Rgn);
	m_ModelTotalRgnStage = Rgn;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTotalRegionStage(TREGION4D &Region) const//取得模組整個範圍
{
	Region = m_ModelTotalRgnStage;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTotalCornerPts(TPOINT2D CornerPts[4]) const//取得模組整個範圍四個端點
{
	CornerPts[0] = m_ModelTotalCornerPts[0];
	CornerPts[1] = m_ModelTotalCornerPts[1];
	CornerPts[2] = m_ModelTotalCornerPts[2];
	CornerPts[3] = m_ModelTotalCornerPts[3];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTotalCornerPtsCad(TPOINT2D CornerPts[4]) const//取得模組整個範圍四個端點
{
	CornerPts[0] = m_ModelTotalCornerPtsCad[0];
	CornerPts[1] = m_ModelTotalCornerPtsCad[1];
	CornerPts[2] = m_ModelTotalCornerPtsCad[2];
	CornerPts[3] = m_ModelTotalCornerPtsCad[3];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelTotalCornerPtsStage(TPOINT2D CornerPts[4]) const//取得模組整個範圍四個端點
{
	CornerPts[0] = m_ModelTotalCornerPtsStage[0];
	CornerPts[1] = m_ModelTotalCornerPtsStage[1];
	CornerPts[2] = m_ModelTotalCornerPtsStage[2];
	CornerPts[3] = m_ModelTotalCornerPtsStage[3];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelBodyLandRegion(TREGION4D &Region) const//取得模組特徵範圍
{
	Region = m_ModelBodyLandRgn;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelBodyLandCornerPts(TPOINT2D CornerPts[4]) const//取得模組特徵範圍四個端點
{
	CornerPts[0] = m_ModelBodyLandCornerPts[0];
	CornerPts[1] = m_ModelBodyLandCornerPts[1];
	CornerPts[2] = m_ModelBodyLandCornerPts[2];
	CornerPts[3] = m_ModelBodyLandCornerPts[3];
	return true;
}
//-------------------------------------------------------------------------------------//
const TSIZE2D& CAOIModel::GetModelImageSize_um() const//取得模組影像的物理尺寸-um
{
	return m_ModelImageSize_um;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelImageSize_um(const TSIZE2D &Size)
{
	m_ModelImageSize_um = Size;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D& CAOIModel::GetModelImageCadOffset_um() const//取得模組影像的Cad偏差-um
{
	return m_ModelImageCadOffset_um;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelImageCadOffset_um(const TPOINT2D &Offset)//設定模組影像的Cad偏差-um
{
	m_ModelImageCadOffset_um = Offset;
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelSupportLandTypeCount() const//取得模型支援特徵框樣式種類數量
{
	int        LandTypeCount = 0;
	MODEL_TYPE ModelType = CAOIModel::GetModelType();
	switch ( ModelType )
	{
	case MODEL_TYPE_CHIP://被動元件-總類
	case MODEL_TYPE_CHIP_C://被動元件-電容
	case MODEL_TYPE_CHIP_R://被動元件-電阻
	case MODEL_TYPE_CHIP_L://被動元件-電感	
	case MODEL_TYPE_CHIP_LED://LED雙腳
	case MODEL_TYPE_MELF://被動元件

	case MODEL_TYPE_ELECTRODE://電極元件
	case MODEL_TYPE_TANTALUM_CONDENSER://鉭質電容
	case MODEL_TYPE_CAPACITY_ARRAY://排容
	case MODEL_TYPE_RESISTOR_ARRAY://排阻
	case MODEL_TYPE_TRANSISTOR://三腳晶體
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR://電解電容
	case MODEL_TYPE_LED_ARRAY://LED多腳	

	case MODEL_TYPE_NO_LEAD_COMPONENT://無腳元件
	case MODEL_TYPE_NO_LEAD_DFN://無腳元件-DFN
	case MODEL_TYPE_NO_LEAD_QFN://無腳元件-QFN
	case MODEL_TYPE_NO_LEAD_OSC://無腳元件-振盪器

	case MODEL_TYPE_LEAD_COMPONENT://Lead元件
	case MODEL_TYPE_LEAD_SOP://Lead元件-SOP
	case MODEL_TYPE_LEAD_QFP://Lead元件-QFP	
	case MODEL_TYPE_LEAD_TRANSISTOR://Lead-三腳晶體

	case MODEL_TYPE_JLEAD_COMPONENT://J-Lead元件
	case MODEL_TYPE_JLEAD_SOJ://J-Lead元件-SOJ
	case MODEL_TYPE_JLEAD_PLCC://J-Lead元件-PLCC

	case MODEL_TYPE_BGA://BGA元件
	case MODEL_TYPE_PAD_COMPONENT://焊盤元件	
	case MODEL_TYPE_GOLD_FINGER://焊盤元件-金手指	

	case MODEL_TYPE_DIP_LEAD://標準DIP引腳	
		LandTypeCount = 1;
		break;

	case MODEL_TYPE_COMPOSITE_COMPONENT://複合元件
	case MODEL_TYPE_POWER_TRANSISTOR://複合元件-功率電晶體
	case MODEL_TYPE_CONNECTOR://複合元件-連接器	
	case MODEL_TYPE_OTHERS://其餘元件-全都可以用
		LandTypeCount = 3;
		break;
	}
	return LandTypeCount;
}
//-------------------------------------------------------------------------------------//
LAND_TYPE CAOIModel::GetModelLandTypeMaster() const//取得模型主要特徵框樣式
{
	LAND_TYPE  LandType = LAND_TYPE_NULL;
	MODEL_TYPE ModelType = GetModelType();
	switch ( ModelType )
	{
	case MODEL_TYPE_CHIP://被動元件-總類
	case MODEL_TYPE_CHIP_C://被動元件-電容
	case MODEL_TYPE_CHIP_R://被動元件-電阻
	case MODEL_TYPE_CHIP_L://被動元件-電感	
	case MODEL_TYPE_CHIP_LED://LED雙腳
	case MODEL_TYPE_MELF://被動元件

	case MODEL_TYPE_ELECTRODE://電極元件
	case MODEL_TYPE_TANTALUM_CONDENSER://鉭質電容
	case MODEL_TYPE_CAPACITY_ARRAY://排容
	case MODEL_TYPE_RESISTOR_ARRAY://排阻
	case MODEL_TYPE_TRANSISTOR://三腳晶體
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR://電解電容
	case MODEL_TYPE_LED_ARRAY://LED多腳	
		LandType = LAND_TYPE_ELECTRODE;
		break;
	case MODEL_TYPE_NO_LEAD_COMPONENT://無腳元件
	case MODEL_TYPE_NO_LEAD_DFN://無腳元件-DFN
	case MODEL_TYPE_NO_LEAD_QFN://無腳元件-QFN
	case MODEL_TYPE_NO_LEAD_OSC://無腳元件-振盪器
		LandType = LAND_TYPE_PAD;
		break;	
	case MODEL_TYPE_LEAD_COMPONENT://Lead元件
	case MODEL_TYPE_LEAD_SOP://Lead元件-SOP
	case MODEL_TYPE_LEAD_QFP://Lead元件-QFP	
	case MODEL_TYPE_LEAD_TRANSISTOR://Lead-三腳晶體
		LandType = LAND_TYPE_IC_LEAD;
		break;
	case MODEL_TYPE_JLEAD_COMPONENT://J-Lead元件
	case MODEL_TYPE_JLEAD_SOJ://J-Lead元件-SOJ
	case MODEL_TYPE_JLEAD_PLCC://J-Lead元件-PLCC
		LandType = LAND_TYPE_ELECTRODE;
		break;
	case MODEL_TYPE_BGA://BGA元件
	case MODEL_TYPE_PAD_COMPONENT://焊盤元件	
	case MODEL_TYPE_GOLD_FINGER://焊盤元件-金手指	
		LandType = LAND_TYPE_PAD;
		break;
	case MODEL_TYPE_DIP_LEAD://標準DIP引腳	
		LandType = LAND_TYPE_DIP_LEAD;
		break;

	case MODEL_TYPE_COMPOSITE_COMPONENT://複合元件
		LandType = LAND_TYPE_CON_LEAD;
		break;
	case MODEL_TYPE_POWER_TRANSISTOR://複合元件-功率電晶體
		LandType = LAND_TYPE_IC_LEAD;
		break;
	case MODEL_TYPE_CONNECTOR://複合元件-連接器	
		LandType = LAND_TYPE_CON_LEAD;
		break;
	case MODEL_TYPE_OTHERS://其餘元件
		LandType = LAND_TYPE_ELECTRODE;
		break;
	}
	return LandType;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::InitialModelProperty()
{
	size_t       i=0;
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = GetModelLandCount();
	m_ModelBodySizeX = 0;
	m_ModelBodySizeY = 0;
	m_ModelBodyHeight = 0;
	m_ModelBodyColorGroup.ResetColorGroupColorList();

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->SetLandLeadSizeX(0);
		LandPtr->SetLandLeadSizeY(0);
		LandPtr->SetLandLeadHeight(0);

		LandPtr->SetLandLeadTipSizeX(0);
		LandPtr->SetLandLeadTipSizeY(0);
		LandPtr->SetLandLeadTipHeight(0);

		LandPtr->SetLandLeadShoulderSizeX(0);
		LandPtr->SetLandLeadShoulderSizeY(0);
		LandPtr->SetLandLeadShoulderHeight(0);
		LandPtr->GetLandLeadColorGroup().ResetColorGroupColorList();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CloneModelProperty(CAOIModel *RefModelPtr)//複製模組屬性參數
{
	CAOIModel   *ModelPtr = this;
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == RefModelPtr ) { return false; }

	size_t       i=0;
	double       SizeX=0;
	double       SizeY=0;
	double       Height=0;
	CAOILand    *LandPtr = NULL;
	CAOILand    *RefLandPtr = NULL;
	BOX_TOWARD   ModelToward = ModelPtr->GetModelBodyBox().GetBoxToward();
	BOX_TOWARD   RefModelToward = RefModelPtr->GetModelBodyBox().GetBoxToward();
	const size_t LandCount = ModelPtr->GetModelLandCount();
	const size_t RefLandCount = RefModelPtr->GetModelLandCount();
	const int    BetAngle = CAOIBox::CalcBoxTowardAngle(RefModelToward, ModelToward);
	
	SizeX = RefModelPtr->m_ModelBodySizeX;
	SizeY = RefModelPtr->m_ModelBodySizeY;
	Height = RefModelPtr->m_ModelBodyHeight;
	switch ( BetAngle )
	{
	case  90:
	case 180:
		m_ModelBodySizeX = SizeY;
		m_ModelBodySizeY = SizeX;
		break;
	default:
		m_ModelBodySizeX = SizeX;
		m_ModelBodySizeY = SizeY;
		break;
	}	
	m_ModelBodyHeight = Height;
	m_ModelBodyColorGroup.CloneColorGroupColorList(RefModelPtr->m_ModelBodyColorGroup);

	if ( LandCount == RefLandCount )
	{
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(i, false);
			RefLandPtr = RefModelPtr->GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( NULL == RefLandPtr ) { continue; }

			//Lead
			SizeX = RefLandPtr->GetLandLeadSizeX();
			SizeY = RefLandPtr->GetLandLeadSizeY();
			Height = RefLandPtr->GetLandLeadHeight();
			switch ( BetAngle )
			{
			case  90:
			case 180:
				LandPtr->SetLandLeadSizeX(SizeY);
				LandPtr->SetLandLeadSizeY(SizeX);				
				break;
			default:
				LandPtr->SetLandLeadSizeX(SizeX);
				LandPtr->SetLandLeadSizeY(SizeY);
				break;
			}	
			LandPtr->SetLandLeadHeight(Height);
			
			//Lead Tip
			SizeX = RefLandPtr->GetLandLeadTipSizeX();
			SizeY = RefLandPtr->GetLandLeadTipSizeY();
			Height = RefLandPtr->GetLandLeadTipHeight();
			switch ( BetAngle )
			{
			case  90:
			case 180:
				LandPtr->SetLandLeadTipSizeX(SizeY);
				LandPtr->SetLandLeadTipSizeY(SizeX);				
				break;
			default:
				LandPtr->SetLandLeadTipSizeX(SizeX);
				LandPtr->SetLandLeadTipSizeY(SizeY);
				break;
			}	
			LandPtr->SetLandLeadTipHeight(Height);

			//Lead Shoulder
			SizeX = RefLandPtr->GetLandLeadShoulderSizeX();
			SizeY = RefLandPtr->GetLandLeadShoulderSizeY();
			Height = RefLandPtr->GetLandLeadShoulderHeight();
			switch ( BetAngle )
			{
			case  90:
			case 180:
				LandPtr->SetLandLeadShoulderSizeX(SizeY);
				LandPtr->SetLandLeadShoulderSizeY(SizeX);				
				break;
			default:
				LandPtr->SetLandLeadShoulderSizeX(SizeX);
				LandPtr->SetLandLeadShoulderSizeY(SizeY);
				break;
			}	
			LandPtr->SetLandLeadShoulderHeight(Height);

			LandPtr->GetLandLeadColorGroup().CloneColorGroupColorList(RefLandPtr->GetLandLeadColorGroup());
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AnalysisModelProperty(std::vector<TUNI_FRAME> &UniFrameList)//分析模組
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	ModifyModelTotalRegionSize(m_ModelImageSize_um.cx, m_ModelImageSize_um.cy);
	if ( IsExceptionAngle == false )
	{	IsOK = AnalysisModelProperty_Kernel(UniFrameList);	}
	else
	{
		const double CPX = 0;
		const double CPY = 0;
		TPOINT2D  ModelCornerPts[4];
		TREGION4D ModelRgn, ModelRgnRotated;
		std::vector<TUNI_FRAME> UniFrameListDst;

		GetModelTotalRegion(ModelRgn);
		RotateModel(-ComAngle, CPX, CPY);		
		GetModelTotalRegion(ModelRgnRotated);
		IsOK = RotateModelUniFrameList(-ComAngle, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListDst);
		if ( false == IsOK )
		{	
			RotateModel(ComAngle, CPX, CPY);			
			return false;
		}
		IsOK = AnalysisModelProperty_Kernel(UniFrameListDst);
		RotateModel(ComAngle, CPX, CPY);
		JetAPI::ClearUniFrameList(UniFrameListDst);		
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AnalysisModelProperty_Kernel(std::vector<TUNI_FRAME> &UniFrameList)//分析模組
{
	size_t       idx=0;
	size_t       u=0, v=0;	
	size_t       i=0, j=0, k=0;
	CAOIBox     *BoxPtr=NULL;
	TUNI_FRAME   UniFrame;
	IMAGE_SIZE   ImageW=0;
	IMAGE_SIZE   ImageH=0;
	IMAGE_SIZE   ImageStep=0;
	IMAGE_SIZE   BitCount=0;
	IMAGE_PTR    ImagePtr=NULL;
	MASK_PTR     MaskPtr=NULL;
	SPACE_PTR    SpacePtr=NULL;	
	unsigned int FrameIndex = 0;
	unsigned int FrameUniqueID = 0;
	unsigned int ColorFrameUniqueID = 0;
	CAOILand    *LandPtr = NULL;	
	RECT         ImageRect={0};
	TREGION4D    BodyRgn;	
	TREGION4D    LandRgn;
	TREGION4D    ModelRgn;
	TREGION4D    ImageRgn;
	TREGION4D    BodyNoLeadRgn;
	TPOINT2D     ModelCp;
	TPOINT2D     ImageRes;
	double       Sum=0;
	double       Ave=0;
	size_t       Count=0;
	CColorRGBV   rgbv;
	const size_t LandCount = GetModelLandCount();
	const size_t UniFrameCount = UniFrameList.size();	

	GetModelBodyRegion(BodyRgn);
	GetModelTotalRegion(ModelRgn);
	CalcModelBodyRegionNoLead(BodyNoLeadRgn);
	ModelCp.x = ModelRgn.GetCpX();
	ModelCp.y = ModelRgn.GetCpY();
	const double RgnScaleX = 0.85;
	const double RgnScaleY = 0.85;
	const double ModelSizeW = ModelRgn.GetWidth();
	const double ModelSizeH = ModelRgn.GetHeight();

	InitialModelProperty();
	//第1張彩色圖
	for ( k=0; k<UniFrameCount; k++ )
	{
		UniFrame = UniFrameList[k];
		if ( 24 != UniFrame.BitCount ) 
		{	continue; }	

		FrameIndex = k;
		FrameUniqueID = UniFrame.FrameUniqueID;
		ImageW = UniFrame.ImageW;
		ImageH = UniFrame.ImageH;
		ImageStep=UniFrame.ImageStep;
		BitCount=UniFrame.BitCount;
		ImagePtr=UniFrame.ImagePtr;
		MaskPtr=UniFrame.MaskPtr;
		SpacePtr=UniFrame.SpacePtr;

		ImageRes.x = ModelSizeW/ImageW;
		ImageRes.y = ModelSizeH/ImageH;	
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, BodyNoLeadRgn, ModelCp, ImageRgn);

		//本體顏色		
		ColorFrameUniqueID = GetModelBodyColorGroup().GetColorGroupFrameUniqueID();
		if ( ColorFrameUniqueID == FrameUniqueID )
		{
			GetModelBodyColorGroup().SetColorGroupFrameIndex(FrameIndex);
			JetAPI::ScaleRegion(ImageRgn, RgnScaleX, RgnScaleY, SCALE_REGION_BY_CENTER, ImageRgn);
			JetAPI::Region4DToRect(ImageRgn, ImageRect, false);
			if ( ImageAPI.CalcColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, ImageRect, rgbv) == true )		
			{	GetModelBodyColorGroup().SetColorGroupColor(0, rgbv);	}
		}
		
		//引腳顏色		
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = GetModelLandPtr(j, false);
			if ( NULL == LandPtr ) { continue; }

			//引腳高度-Lead
			ColorFrameUniqueID = LandPtr->GetLandLeadColorGroup().GetColorGroupFrameUniqueID();
			if ( ColorFrameUniqueID == FrameUniqueID )
			{				
				BoxPtr = LandPtr->GetLandLeadBoxPtr();
				BoxPtr->GetBoxRegion(LandRgn);			
				AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, LandRgn, ModelCp, ImageRgn);
				JetAPI::ScaleRegion(ImageRgn, RgnScaleX, RgnScaleY, SCALE_REGION_BY_CENTER, ImageRgn);
				JetAPI::Region4DToRect(ImageRgn, ImageRect, false);
				LandPtr->GetLandLeadColorGroup().SetColorGroupFrameIndex(FrameIndex);
				if ( ImageAPI.CalcColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, ImageRect, rgbv) == true )		
				{	LandPtr->GetLandLeadColorGroup().SetColorGroupColor(0, rgbv);	}
			}
		}				
	}

	//高度計算
	for ( k=0; k<UniFrameCount; k++ )
	{
		UniFrame = UniFrameList[k];
		if ( NULL==UniFrame.SpacePtr || NULL==UniFrame.MaskPtr) 
		{	continue; }		

		ImageW = UniFrame.ImageW;
		ImageH = UniFrame.ImageH;
		ImageStep=UniFrame.ImageStep;
		BitCount=UniFrame.BitCount;
		ImagePtr=UniFrame.ImagePtr;
		MaskPtr=UniFrame.MaskPtr;
		SpacePtr=UniFrame.SpacePtr;

		ImageRes.x = ModelSizeW/ImageW;
		ImageRes.y = ModelSizeH/ImageH;
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, BodyRgn, ModelCp, ImageRgn);
		
		//本體高度
		Sum = 0;
		Count = 0;
		JetAPI::ScaleRegion(ImageRgn, RgnScaleX, RgnScaleY, SCALE_REGION_BY_CENTER, ImageRgn);
		JetAPI::Region4DToRect(ImageRgn, ImageRect, false);
		if ( JetAPI::CheckImageRoi(ImageW, ImageH, ImageRect) == true )
		{
			for ( u=ImageRect.top; u<ImageRect.bottom; u++ )
			{
				idx = (u*ImageStep)+ImageRect.left-1;
				for ( v=ImageRect.left; v<ImageRect.right; v++ )
				{	
					idx ++;
					if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false)//非有效點
					{	continue;	}				
					Sum += SpacePtr[idx];
					Count ++;					
				}
			}
			if ( Count > 0 ) 
			{	
				Ave = Sum/Count; 
				SetModelBodyHeight(Ave);
			}
			const double BodySizeX = BodyRgn.GetSizeX();
			const double BodySizeY = BodyRgn.GetSizeY();
			SetModelBodySizeX(BodySizeX);
			SetModelBodySizeY(BodySizeY);
		}		

		//引腳高度
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = GetModelLandPtr(j, false);
			if ( NULL == LandPtr ) { continue; }

			//引腳高度-Lead
			BoxPtr = LandPtr->GetLandLeadBoxPtr();
			BoxPtr->GetBoxRegion(LandRgn);			
			AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, LandRgn, ModelCp, ImageRgn);			
			Sum = 0;
			Count = 0;			
			JetAPI::ScaleRegion(ImageRgn, RgnScaleX, RgnScaleY, SCALE_REGION_BY_CENTER, ImageRgn);
			JetAPI::Region4DToRect(ImageRgn, ImageRect, false);
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, ImageRect) == true )
			{
				for ( u=ImageRect.top; u<ImageRect.bottom; u++ )
				{
					idx = (u*ImageStep)+ImageRect.left-1;
					for ( v=ImageRect.left; v<ImageRect.right; v++ )
					{
						idx++;
						if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false)//非有效點
						{	continue;	}				
						Sum += SpacePtr[idx];
						Count ++;						
					}
				}
				if ( Count > 0 ) 
				{	
					Ave = Sum/Count; 					
					LandPtr->SetLandLeadHeight(Ave);
				}
				
				const double LeadSizeX = LandRgn.GetSizeX();
				const double LeadSizeY = LandRgn.GetSizeY();
				LandPtr->SetLandLeadSizeX(LeadSizeX);
				LandPtr->SetLandLeadSizeY(LeadSizeY);				
			}

			//引腳高度-Lead-Tip
			BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
			BoxPtr->GetBoxRegion(LandRgn);			
			AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, LandRgn, ModelCp, ImageRgn);			
			Sum = 0;
			Count = 0;
			JetAPI::ScaleRegion(ImageRgn, RgnScaleX, RgnScaleY, SCALE_REGION_BY_CENTER, ImageRgn);
			JetAPI::Region4DToRect(ImageRgn, ImageRect, false);
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, ImageRect) == true )
			{
				for ( u=ImageRect.top; u<ImageRect.bottom; u++ )
				{
					idx = (u*ImageStep)+ImageRect.left-1;
					for ( v=ImageRect.left; v<ImageRect.right; v++ )
					{	
						idx ++;
						if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false)//非有效點
						{	continue;	}				
						Sum += SpacePtr[idx];
						Count ++;						
					}
				}
				if ( Count > 0 ) 
				{	
					Ave = Sum/Count; 
					LandPtr->SetLandLeadTipHeight(Ave);
				}
				const double LeadTipSizeX = LandRgn.GetSizeX();
				const double LeadTipSizeY = LandRgn.GetSizeY();
				LandPtr->SetLandLeadTipSizeX(LeadTipSizeX);
				LandPtr->SetLandLeadTipSizeY(LeadTipSizeY);
			}

			//引腳高度-Lead Shoulder
			BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
			BoxPtr->GetBoxRegion(LandRgn);			
			AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, LandRgn, ModelCp, ImageRgn);			
			Sum = 0;
			Count = 0;
			JetAPI::ScaleRegion(ImageRgn, RgnScaleX, RgnScaleY, SCALE_REGION_BY_CENTER, ImageRgn);
			JetAPI::Region4DToRect(ImageRgn, ImageRect, false);		
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, ImageRect) == true )
			{
				for ( u=ImageRect.top; u<ImageRect.bottom; u++ )
				{
					idx = (u*ImageStep)+ImageRect.left-1;
					for ( v=ImageRect.left; v<ImageRect.right; v++ )
					{	
						idx ++;
						if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false)//非有效點
						{	continue;	}				
						Sum += SpacePtr[idx];
						Count ++;						
					}
				}
				if ( Count > 0 ) 
				{	
					Ave = Sum/Count; 
					LandPtr->SetLandLeadShoulderHeight(Ave);
				}
				const double LeadShoulderSizeX = LandRgn.GetSizeX();
				const double LeadShoulderSizeY = LandRgn.GetSizeY();
				LandPtr->SetLandLeadShoulderSizeX(LeadShoulderSizeX);
				LandPtr->SetLandLeadShoulderSizeY(LeadShoulderSizeY);
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ApplyModelPropertyToAlgParam()//套用模組屬相參數至演算法參數
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = ApplyModelPropertyToAlgParam_Kernel();	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = ApplyModelPropertyToAlgParam_Kernel();
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;		
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ApplyModelPropertyToAlgParam_Kernel()//套用模組屬性至演算法參數
{
	CAOIModel *ModelPtr = this;
	if ( CheckModelPtr(ModelPtr) == false ) { return false; }

	size_t         i=0;	
	ALG_TYPE       AlgType;
	WND_DEFECT_ID  WndDefectID;
	CAOIWnd       *WndPtr = NULL;
	CAOILand      *LandPtr = NULL;
	double         LeadSizeX=0;
	double         LeadSizeY=0;
	double         LeadHeight=0;
	unsigned int   FrameUniqueID=0;
	const size_t   WndCount = ModelPtr->GetModelWndCount();
	const double   ModelBodySizeX  = ModelPtr->GetModelBodySizeX();
	const double   ModelBodySizeY  = ModelPtr->GetModelBodySizeY();
	const double   ModelBodyHeight = ModelPtr->GetModelBodyHeight();
	const double   ModelBodySizeXY = ModelBodySizeX*ModelBodySizeY;
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		AlgType = WndPtr->GetWndAlgType();		
		WndDefectID = WndPtr->GetWndDefectID();
		CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
		FrameUniqueID = AlgParam.GetAlgImageBinParam().GetBinaryFrameUniqueID();

		if ( NULL == LandPtr )//Model Body
		{
			if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{	
				AlgParam.GetAlgImageBinParam().SetFixedThresholdLow(ModelBodyHeight*0.80);
				AlgParam.GetAlgParamBrightRatio().brTargetValue = ModelBodyHeight;				
				AlgParam.GetAlgParamObjectMeasure().omSizeXSpec = ModelBodySizeX;
				AlgParam.GetAlgParamObjectMeasure().omSizeYSpec = ModelBodySizeY;
				AlgParam.GetAlgParamObjectMeasure().omHeightSpec = ModelBodyHeight;
				AlgParam.GetAlgParamObjectMeasure().omAreaSpec = ModelBodySizeXY;
				AlgParam.GetAlgParamObjectMeasure().omVolumeSpec = ModelBodySizeXY*ModelBodyHeight;
			}
		}
		else
		{
			LeadSizeX = LandPtr->GetLandLeadSizeX();
			LeadSizeY = LandPtr->GetLandLeadSizeY();
			LeadHeight = LandPtr->GetLandLeadHeight();
			if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{	
				AlgParam.GetAlgImageBinParam().SetFixedThresholdLow(LeadHeight*0.80);
				AlgParam.GetAlgParamBrightRatio().brTargetValue = LeadHeight;	
				AlgParam.GetAlgParamObjectMeasure().omSizeXSpec = LeadSizeX;
				AlgParam.GetAlgParamObjectMeasure().omSizeYSpec = LeadSizeY;
				AlgParam.GetAlgParamObjectMeasure().omHeightSpec = LeadHeight;
				AlgParam.GetAlgParamObjectMeasure().omAreaSpec = LeadSizeX*LeadSizeY;
				AlgParam.GetAlgParamObjectMeasure().omVolumeSpec = LeadSizeX*LeadSizeY*LeadHeight;
			}
		}
		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWndKernal(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWndKernal(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelLandOffsetRange(BOX_TOWARD Toward, TPOINT2D &Range)//計算模組特徵框偏移範圍
{
	size_t i=0;	
	CAOILand  *LandPtr = NULL;
	size_t LandCount = GetModelLandCount();		
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		if ( Toward != LandPtr->GetLandToward() ) { continue; }
		break;
	}
	if ( i == LandCount )
	{	return false; }	
	
	double PadW=0, PadH=0;
	double LeadW=0, LeadH=0;
	double BodyW=0, BodyH=0;	
	TREGION4D  BodyRgn, PadRgn, LeadRgn;	
	LAND_TYPE  LandType=LandPtr->GetLandType();	

	GetModelBodyRegion(BodyRgn);
	LandPtr->GetLandPadBox().GetBoxRegion(PadRgn);
	LandPtr->GetLandLeadBox().GetBoxRegion(LeadRgn);

	BodyW = BodyRgn.GetWidth();
	BodyH = BodyRgn.GetHeight();			
	PadW = PadRgn.GetWidth();
	PadH = PadRgn.GetHeight();
	LeadW = LeadRgn.GetWidth();
	LeadH = LeadRgn.GetHeight();

	double RangeX=PadW;
	double RangeY=PadH;
	switch ( Toward )
	{
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		RangeY = MIN(BodyH, RangeY);
		if ( LAND_TYPE_PAD != LandType )
		{	RangeX = MIN(LeadW, RangeX); }			
		break;
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		RangeX = MIN(BodyW, RangeX);			
		if ( LAND_TYPE_PAD != LandType )
		{	RangeY = MIN(LeadH, RangeY); }			
		break;
	}	
	Range.x = RangeX;
	Range.y = RangeY;

	double RatioX=0.33;
	double RatioY=0.33;
	double PadRatioX=RatioX;
	double PadRatioY=RatioY;
	double LeadRatioX=RatioX;
	double LeadRatioY=RatioY;
	switch ( LandType )
	{
	case LAND_TYPE_ELECTRODE:
		LeadRatioX=0.50;
		LeadRatioY=0.50;
		break;
	default:
		LeadRatioX=RatioX;
		LeadRatioY=RatioY;
		break;
	}
	switch ( Toward )
	{
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		RatioX=LeadRatioX;
		RatioY=PadRatioY;
		break;
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		RatioX=PadRatioX;
		RatioY=LeadRatioY;
		break;
	}
	Range.x *= RatioX;
	Range.y *= RatioY;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelOffsetLimit(const TMODEL_DEFAULT_WND_PARAM &Param, TPOINT2D &Limit)//計算模組偏移上限
{		
	double Range=0;		
	LAND_TYPE  LandType;	
	double PadW=0, PadH=0;
	double LeadW=0, LeadH=0;
	double BodyW=0, BodyH=0;	
	TREGION4D  BodyRgn, PadRgn, LeadRgn;
	MODEL_TYPE ModelType = Param.eModelType;	
	CAOILand  *LandPtr = GetModelLandPtr(0, true);	
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int IPCLevel = AOIDataCollect.GetModelIPCLevel();	

	GetModelBodyRegion(BodyRgn);
	BodyW = BodyRgn.GetWidth();
	BodyH = BodyRgn.GetHeight();	
	if ( NULL == LandPtr )
	{
		PadW = PadH = 800;
		LeadW = LeadH = 800;		
		LandType = LAND_TYPE_PAD;
	}
	else
	{
		LandType = LandPtr->GetLandType();
		LandPtr->GetLandPadBox().GetBoxRegion(PadRgn);
		LandPtr->GetLandLeadBox().GetBoxRegion(LeadRgn);
		PadW = PadRgn.GetWidth();
		PadH = PadRgn.GetHeight();
		LeadW = LeadRgn.GetWidth();
		LeadH = LeadRgn.GetHeight();
	}

	if ( MODEL_TYPE_CHIP_C == ModelType || MODEL_TYPE_CHIP_R == ModelType || MODEL_TYPE_CHIP_L == ModelType )
	{	Range=Param.dPartAlignOffsetLimit;	}
	else
	{
		switch ( Toward )
		{
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			Range = BodyH;
			if ( LAND_TYPE_NULL != LandType )
			{
				Range = MIN(PadH, Range);
				if ( LAND_TYPE_PAD != LandType )
				{	Range = MIN(LeadH, Range); }
			}
			break;
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			Range = BodyW;
			if ( LAND_TYPE_NULL != LandType )
			{			
				Range = MIN(PadW, Range);
				if ( LAND_TYPE_PAD != LandType )
				{	Range = MIN(LeadW, Range); }
			}
			break;
		}				
		switch ( IPCLevel )
		{
		case IPC_A_610E_LEVEL_3:	
			Range = Range*0.25;
			Range = JetAPI::AdjustValue(Range, 10);
			Range = MAX(Range, 20);//最少偏移量- 20um
			Range = MIN(Range, 300);//最多偏移量-300um
			break;
		default:
			Range = Range*0.50;
			Range = JetAPI::AdjustValue(Range, 10);
			Range = MAX(Range, 20);//最少偏移量- 20um
			Range = MIN(Range, 500);//最多偏移量-500um
			break;
		}		
	}	
	Limit.x = Range;
	Limit.y = Range;		
	
	if ( MDW_VERSION_2 == Param.eVersion )
	{
		bool bFind=false;
		TPOINT2D OffsetRange;
		double RangeX=BodyW;
		double RangeY=BodyH;
		double LimitX=Param.dPartAlignOffsetLimit;
		double LimitY=Param.dPartAlignOffsetLimit;		
		if ( CalcModelLandOffsetRange(BOX_TOWARD_UP, OffsetRange) == true )
		{
			bFind = true;
			RangeX = MIN(OffsetRange.x, RangeX);
			RangeY = MIN(OffsetRange.y, RangeY);
		}
		if ( CalcModelLandOffsetRange(BOX_TOWARD_LEFT, OffsetRange) == true )
		{
			bFind = true;
			RangeX = MIN(OffsetRange.x, RangeX);
			RangeY = MIN(OffsetRange.y, RangeY);
		}
		if ( CalcModelLandOffsetRange(BOX_TOWARD_DOWN, OffsetRange) == true )
		{
			bFind = true;
			RangeX = MIN(OffsetRange.x, RangeX);
			RangeY = MIN(OffsetRange.y, RangeY);
		}
		if ( CalcModelLandOffsetRange(BOX_TOWARD_RIGHT, OffsetRange) == true )
		{
			bFind = true;
			RangeX = MIN(OffsetRange.x, RangeX);
			RangeY = MIN(OffsetRange.y, RangeY);
		}		

		switch ( ModelType )
		{		
		//case MODEL_TYPE_NULL:

		//case MODEL_TYPE_CHIP:
		//case MODEL_TYPE_CHIP_C:
		//case MODEL_TYPE_CHIP_R:
		case MODEL_TYPE_CHIP_L:
		case MODEL_TYPE_CHIP_LED:
		case MODEL_TYPE_MELF:

		case MODEL_TYPE_ELECTRODE:
		case MODEL_TYPE_TANTALUM_CONDENSER:
		case MODEL_TYPE_CAPACITY_ARRAY:
		case MODEL_TYPE_RESISTOR_ARRAY:
		case MODEL_TYPE_TRANSISTOR:
		case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
		case MODEL_TYPE_LED_ARRAY:

		//case MODEL_TYPE_NO_LEAD_COMPONENT:
		case MODEL_TYPE_NO_LEAD_DFN:
		//case MODEL_TYPE_NO_LEAD_QFN:
		case MODEL_TYPE_NO_LEAD_OSC:

		case MODEL_TYPE_LEAD_COMPONENT:
		case MODEL_TYPE_LEAD_SOP:
		case MODEL_TYPE_LEAD_QFP:
		case MODEL_TYPE_LEAD_TRANSISTOR:

		case MODEL_TYPE_JLEAD_COMPONENT:
		case MODEL_TYPE_JLEAD_SOJ:
		case MODEL_TYPE_JLEAD_PLCC:

		case MODEL_TYPE_COMPOSITE_COMPONENT:
		case MODEL_TYPE_POWER_TRANSISTOR:
		case MODEL_TYPE_CONNECTOR:

		//case MODEL_TYPE_BGA:
		//case MODEL_TYPE_FD:
		//case MODEL_TYPE_BARCODE:

		case MODEL_TYPE_PAD_COMPONENT:
		//case MODEL_TYPE_GOLD_FINGER:

		case MODEL_TYPE_DIP_LEAD:

		case MODEL_TYPE_OTHERS:

			if ( true == bFind )
			{
				LimitX = RangeX;
				LimitY = RangeY;
				LimitX = JetAPI::AdjustValue(LimitX, 5);
				LimitY = JetAPI::AdjustValue(LimitY, 5);
				LimitX = MAX(LimitX, 20);//最少偏移量- 20um
				LimitX = MIN(LimitX, 1000);//最多偏移量-300um
				LimitY = MAX(LimitY, 20);//最少偏移量- 20um
				LimitY = MIN(LimitY, 1000);//最多偏移量-300um
			}
			break;
		//case AAAAAAAAAAAAAAA:
		default:
			break;
		}

		Limit.x = LimitX;
		Limit.y = LimitY;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWndKernal(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框
{	
	CString    Filename;		
	CAOIModel  DefaultModel;	
	CString    GroupName = Param.sGroupName;
	TMODEL_DEFAULT_WND_PARAM ParamUsed = Param;
	if ( true == ParamUsed.bUseDefaultModel )
	{		
		Filename = CAOIModel::GetModelDefaultFilename(GroupName);		
		if ( DefaultModel.LoadModelParamFile(Filename) == true )
		{			
			unsigned int DefaultFrameIndex = Param.nDefaultFrameIndex;
			unsigned int DefaultFrameUniqueID = Param.nDefaultFrameUniqueID;
			std::vector<unsigned int> FrameIndexMapList = Param.FrameIndexMapList;
			DefaultModel.UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
			ParamUsed.pModel = &(DefaultModel);	
		}
	}
	SetModelNeedSaveFiles(true);
	SetModelBKImageNeedToGrab(true);	
	//SetModelGroupName(Param.sGroupName);//先不要自動設定
	SetModelChipSizeMode(ParamUsed.eChipSizeMode);	

	if ( true == ParamUsed.bDefaultModelAllDefect )
	{
		if ( AddModelDefaultWnd_DefaultModelDefect(ParamUsed) == false )
		{	return false; }
	}
	else
	{
		if ( true == ParamUsed.bPadAlign )
		{
			if ( AddModelDefaultWnd_PadAlign(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bPartAlign )
		{	
			if ( AddModelDefaultWnd_PartAlign(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bPadAdjust )
		{	
			if ( AddModelDefaultWnd_PadAdjust(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bLeadAdjust )
		{	
			if ( AddModelDefaultWnd_LeadAdjust(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bBodyMissing )
		{
			if ( AddModelDefaultWnd_BodyMissing(ParamUsed) == false )
			{	return false; }
		}	

		if ( true == ParamUsed.bBodyTilt )
		{
			if ( AddModelDefaultWnd_BodyTilt(ParamUsed) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bBodyMount )
		{
			if ( AddModelDefaultWnd_BodyMount(ParamUsed) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bPolarity )
		{
			if ( AddModelDefaultWnd_Polarity(ParamUsed) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bTextWrong )
		{
			if ( AddModelDefaultWnd_TextWrong(ParamUsed) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bBodyDamaged )
		{
			if ( AddModelDefaultWnd_BodyDamaged(ParamUsed) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bForeignBody )
		{
			if ( AddModelDefaultWnd_ForeignBody(ParamUsed) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bLeadLifted )
		{
			if ( AddModelDefaultWnd_LeadLifted(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bLeadBended )
		{
			if ( AddModelDefaultWnd_LeadBended(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bSolderOpen )
		{
			if ( AddModelDefaultWnd_SolderOpen(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bSolderPoor )
		{
			if ( AddModelDefaultWnd_SolderPoor(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bSolderPadExposed )
		{
			if ( AddModelDefaultWnd_SolderPadExposed(ParamUsed, -1) == false )
			{	return false; }
		}

		if ( true == ParamUsed.bPadScratch )
		{
			if ( AddModelDefaultWnd_PadScratch(ParamUsed, -1) == false )
			{	return false; }
		}
	
		if ( true == ParamUsed.bBridge )
		{
			int  BridgeMode = 0;
			const int BodyOuterShort=1;
			const int LandBetweenShort=2;
			const int LandOuterShort=3;		
			switch ( ParamUsed.eModelType )
			{
			case MODEL_TYPE_CHIP:
			case MODEL_TYPE_CHIP_C:
			case MODEL_TYPE_CHIP_R:
			case MODEL_TYPE_CHIP_L:
			case MODEL_TYPE_CHIP_LED:
			case MODEL_TYPE_MELF:
			case MODEL_TYPE_TANTALUM_CONDENSER:
			case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
				BridgeMode = BodyOuterShort;
				break;
			case MODEL_TYPE_DIP_LEAD:
				BridgeMode = LandOuterShort;
				break;
			default:
				BridgeMode = LandBetweenShort;
				break;
			}
		
			bool CreateBridgeOK=true;
			switch ( BridgeMode )
			{
			case BodyOuterShort:
				CreateBridgeOK = AddModelDefaultWnd_OuterShort(ParamUsed);
				break;
			case LandOuterShort:
				CreateBridgeOK = AddModelDefaultWnd_LandOuterShort(ParamUsed, -1);
				break;
			case LandBetweenShort:
			default:
				CreateBridgeOK = AddModelDefaultWnd_LandBridge(ParamUsed, -1);
				break;
			}
			if ( false == CreateBridgeOK )
			{	return false; }
		}
	}	

	//UpdateModelWndRgnByLinkModeKernel();
	UpdateModelWndRgnByLinkMode();
	CalcModelTotalRegionAll();
	UpdateModelRegionToAttached();
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIModel::ApplyModelDefaultLandWnd(CAOIWnd *WndPtr, CAOIWnd *RefWndPtr)//增加模組引腳檢測框
{
	if ( NULL == WndPtr ) { return true; }
	if ( NULL == RefWndPtr ) { return true; }

	TREGION4D  RefRgn;
	double     RefX=0, RefY=0;		
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	BOX_TOWARD RefToward = RefWndPtr->GetWndToward();
	const int  BetweenAngle = CAOIBox::CalcBoxTowardAngle(RefToward, WndToward);
	RefWndPtr->GetWndRegion(RefRgn);
	RefX = RefRgn.GetCpX();
	RefY = RefRgn.GetCpY();
	if ( 0 !=  BetweenAngle )
	{	RefWndPtr->RotateWnd(BetweenAngle, RefX, RefY);	}
	RefToward = RefWndPtr->GetWndToward();
	WndPtr->ApplyWndDefault(RefWndPtr);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ApplyModelDefaultBodyWnd(CAOIWnd *WndPtr, const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組本體檢測框
{
	if ( NULL == WndPtr ) { return true; }
	CAOIModel   *RefModelPtr = (CAOIModel*)(Param.pModel);	
	if ( NULL == RefModelPtr ) { return true; }
		
	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	const int WndDefectGroupID = WndPtr->GetWndDefectGroupID();
	CAOIWnd *RefWndPtr = RefModelPtr->GetModelWndPtrByDefectID(WndDefectID, WndDefectGroupID);	
	if ( NULL == RefWndPtr ) { return true; }

	TREGION4D  TmpRgn;
	double     TmpX=0, TmpY=0;
	CAOIWnd    TempWnd = *RefWndPtr;	
	BOX_TOWARD TmpToward = TempWnd.GetWndToward();
	BOX_TOWARD WndToward = WndPtr->GetWndToward();
	const int  BetweenAngle = CAOIBox::CalcBoxTowardAngle(TmpToward, WndToward);
	TempWnd.GetWndRegion(TmpRgn);
	TmpX = TmpRgn.GetCpX();
	TmpY = TmpRgn.GetCpY();
	if ( 0 !=  BetweenAngle )
	{	TempWnd.RotateWnd(BetweenAngle, TmpX, TmpY);	}
	TmpToward = TempWnd.GetWndToward();
	WndPtr->ApplyWndDefault(&TempWnd);
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PadAlign(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-焊盤定位
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_PadAlignKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_PadAlignKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PadAlignKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-焊盤定位
{	
	TPOINT2D  psWnd;
	TSIZE2D   szLand, szWnd, szWndExt;
	CAOIWnd   *WndPtr = NULL;		
	const double scX = 1.50;
	const double scY = 1.50;
	const double ExtendMaxX = 1000;
	const double ExtendMaxY = 1000;	
	MODEL_TYPE   ModelType = Param.eModelType;
	TREGION4D    RgnTarget, RgnWnd, RgnWndExt;
	
	GetModelPadRegion(LandGroupID, -1, RgnTarget);

	psWnd.x = (RgnTarget.minX+RgnTarget.maxX)*0.5;
	psWnd.y = (RgnTarget.minY+RgnTarget.maxY)*0.5;
	szWnd.cx = RgnTarget.maxX-RgnTarget.minX;
	szWnd.cy = RgnTarget.maxY-RgnTarget.minY;	
	szWndExt.cx = szWnd.cx*scX;
	szWndExt.cy = szWnd.cy*scY;
	if ( (szWndExt.cx-szWnd.cx) > ExtendMaxX ) 
	{	szWndExt.cx = szWnd.cx+ExtendMaxX; }
	if ( (szWndExt.cy-szWnd.cy) > ExtendMaxY ) 
	{	szWndExt.cy = szWnd.cy+ExtendMaxY; }
	
	const double MinSize = MIN(szWnd.cx, szWnd.cy);	
	int   nMinSize = (int)(MinSize/100);
	if ( nMinSize < 1 ) { nMinSize = 1; }
	nMinSize = nMinSize*10;
	if ( nMinSize < 400 ) { nMinSize = 400; }
	if ( nMinSize > 1000 ) { nMinSize = 1000; }
	 
	//const double ExtX = nMinSize;
	//const double ExtY = nMinSize;
	const double ExtX = Param.dPadAlignExtendRange;
	const double ExtY = Param.dPadAlignExtendRange;		
	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.5);
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.5);
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.5);
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.5);
	ALG_TYPE   AlgType = ALG_MODEL_MATCH;
	IMAGE_SRC_MODE ImageSrcMode=IMAGE_SRC_MAX_GRN_BLU;
	WND_RGN_LINK_MODE WndRgnLinkMode=WND_RGN_LINK_PAD;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_PAD_ALIGN;
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	//unsigned int FrameIndex = Param.nFrameIndex_Align;
	//unsigned int FrameUniqueID = Param.nFrameUniqueID_Align;
	unsigned int FrameIndex = Param.nFrameIndex_Solder;//焊錫光源
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;//焊錫光源

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WndRgnLinkMode);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndExtendRangeX(ExtX);
	WndPtr->SetWndExtendRangeY(ExtY);	
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();	
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();

	BinaryParam.SetGrayGainValue(5.0);
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);	
	BinaryParam.SetBinaryImageSourceMode(ImageSrcMode);//IMAGE_SRC_LIGHTNESS, IMAGE_SRC_MAX_GRN_BLU	, IMAGE_SRC_BLUE_RATIO
	//BinaryParam.SetBinaryMode(BINARY_COLOR_FILTER);
	//BinaryParam.SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_PAD_BEGIN);

	const double SkewLimit = 10;
	const double OffsetLimit = 1000;	
	AlgParam.SetAlgOffsetXUSL(OffsetLimit);
	AlgParam.SetAlgOffsetXLSL(-OffsetLimit);
	AlgParam.SetAlgOffsetYUSL(OffsetLimit);
	AlgParam.SetAlgOffsetYLSL(-OffsetLimit);
	AlgParam.SetAlgSkewUSL(SkewLimit);
	AlgParam.SetAlgSkewLSL(-SkewLimit);
	AlgParam.SetAlgPatternSimilarityUSL(100);
	AlgParam.SetAlgPatternSimilarityLSL(20);

	TALG_PARAM_MODEL_MATCH &mmParam = AlgParam.GetAlgParamModelMatch();
	//WndPtr->SetWndModelMaskFlag(MODEL_MASK_BODY);

	ApplyModelDefaultBodyWnd(WndPtr, Param);	
	CAOIModel::AddModelWndPtr(WndPtr, false);

	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PartAlign(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-零件定位
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_PartAlignKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_PartAlignKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PartAlignKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-零件定位
{
	bool bSucc=true;
	switch ( Param.eVersion )
	{
	case MDW_VERSION_2:	bSucc = AddModelDefaultWnd_PartAlignKernel_v2(Param, LandGroupID);	break;
	default:
	case MDW_VERSION_1:	bSucc = AddModelDefaultWnd_PartAlignKernel_v1(Param, LandGroupID);	break;		
	}
	return bSucc;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PartAlignKernel_v1(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-零件定位	
{
	CAOIWnd   *WndPtr = NULL;	
	TPOINT2D  psWnd;
	TSIZE2D   szWnd, szWndExt;	
	double    PadW=0, PadH=0;
	double    LeadW=0, LeadH=0;
	double    BodyW=0, BodyH=0;
	const double scX = 1.50;
	const double scY = 1.50;
	const double ExtendMaxX = 1000;
	const double ExtendMaxY = 1000;
	TREGION4D    RgnBody, RgnLead, RgnPad;
	TREGION4D    RgnTarget, RgnWnd, RgnWndExt;
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	MODEL_TYPE   ModelType = Param.eModelType;
	const int    PartAlignMode = Param.nPartAlignMode;
	unsigned int FrameIndex = Param.nFrameIndex_Align;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Align;	
	WND_RGN_LINK_MODE LinkMode = Param.ePartAlignLinkMode;	
	CAOILand  *LandPtr = GetModelLandPtr(0, true);	
	CAOIWnd *WndPtr_PadAlign = GetModelWndPtrByDefectID(WND_DEFECT_PAD_ALIGN);

	if ( MDW_VERSION_2 == Param.eVersion )
	{
		switch ( ModelType )
		{
		case MODEL_TYPE_CHIP_C:
			FrameIndex = Param.nFrameIndex_Solder;
			FrameUniqueID = Param.nFrameUniqueID_Solder;	
			break;
		case MODEL_TYPE_CHIP_R:
			FrameIndex = Param.nFrameIndex_High;
			FrameUniqueID = Param.nFrameUniqueID_High;	
			break;
		}
	}

	GetModelBodyRegion(RgnBody);
	BodyW = RgnBody.GetWidth();
	BodyH = RgnBody.GetHeight();	
	if ( NULL == LandPtr )
	{
		PadW = PadH = 800;
		LeadW = LeadH = 800;
		if ( MODEL_TYPE_BGA == ModelType )
		{	LandType = LAND_TYPE_PAD; }
	}
	else
	{
		LandType = LandPtr->GetLandType();
		LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
		LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
		PadW = RgnPad.GetWidth();
		PadH = RgnPad.GetHeight();
		LeadW = RgnLead.GetWidth();
		LeadH = RgnLead.GetHeight();
	}	
	if ( PART_ALIGN_MODE_BY_MODEL_MATCH_2D == PartAlignMode )
	{	
		switch ( LinkMode )
		{	
		case WND_RGN_LINK_LEAD:
			GetModelLeadRegion(LandGroupID, -1, RgnTarget);		
			break;
		case WND_RGN_LINK_LEAD_TIP:
			GetModelLeadTipRegion(LandGroupID, -1, RgnTarget);
			break;
		case WND_RGN_LINK_LEAD_SHOULDER:
			GetModelLeadShoulderRegion(LandGroupID, -1, RgnTarget);
			break;
		case WND_RGN_LINK_LEAD_TIP_SHOULDER:
			GetModelLeadShoulderTipRegion(LandGroupID, -1, RgnTarget);		
			break;
		default://WND_RGN_LINK_BODY
			GetModelBodyRegion(RgnTarget);
			FrameIndex = Param.nFrameIndex_3D;
			FrameUniqueID = Param.nFrameUniqueID_3D;
			break;
		}
	}
	if ( PART_ALIGN_MODE_BY_MODEL_MATCH_3D == PartAlignMode )
	{
		switch ( LinkMode )
		{	
		case WND_RGN_LINK_BODY:
		case WND_RGN_LINK_LEAD:			
		case WND_RGN_LINK_LEAD_TIP:			
		case WND_RGN_LINK_LEAD_SHOULDER:			
		case WND_RGN_LINK_LEAD_TIP_SHOULDER:
			LinkMode = WND_RGN_LINK_BODY;
			GetModelBodyRegion(RgnTarget);			
			break;
		default:
			LinkMode = WND_RGN_LINK_NONE;
			GetModelBodyRegion(RgnTarget);
			break;
		}
		FrameIndex = Param.nFrameIndex_3D;
		FrameUniqueID = Param.nFrameUniqueID_3D;
	}
	if ( PART_ALIGN_MODE_BY_OBJECT_MEASURE == PartAlignMode )
	{
		switch ( LinkMode )
		{	
		case WND_RGN_LINK_LEAD:			
		case WND_RGN_LINK_LEAD_TIP:			
		case WND_RGN_LINK_LEAD_SHOULDER:			
		case WND_RGN_LINK_LEAD_TIP_SHOULDER:
			LinkMode = WND_RGN_LINK_BODY;
			GetModelBodyRegion(RgnTarget);			
			break;
		default:
			LinkMode = WND_RGN_LINK_NONE;
			GetModelBodyRegion(RgnTarget);
			break;
		}
		FrameIndex = Param.nFrameIndex_3D;
		FrameUniqueID = Param.nFrameUniqueID_3D;		
	}

	psWnd.x = (RgnTarget.minX+RgnTarget.maxX)*0.5;
	psWnd.y = (RgnTarget.minY+RgnTarget.maxY)*0.5;
	szWnd.cx = RgnTarget.maxX-RgnTarget.minX;
	szWnd.cy = RgnTarget.maxY-RgnTarget.minY;
	szWndExt.cx = szWnd.cx*scX;
	szWndExt.cy = szWnd.cy*scY;
	if ( (szWndExt.cx-szWnd.cx) > ExtendMaxX ) 
	{	szWndExt.cx = szWnd.cx+ExtendMaxX; }
	if ( (szWndExt.cy-szWnd.cy) > ExtendMaxY ) 
	{	szWndExt.cy = szWnd.cy+ExtendMaxY; }
	
	int   nMinSizeX = 0;
	int   nMinSizeY = 0;
	const double MinSize = MIN(szWnd.cx, szWnd.cy);	
	const double MaxSize = MAX(szWnd.cx, szWnd.cy);	
	double ExtX = (szWndExt.cx-szWnd.cx)*0.5;
	double ExtY = (szWndExt.cy-szWnd.cy)*0.5;
	if ( NULL == WndPtr_PadAlign )
	{		
		nMinSizeX = (int)(MinSize/100);
		if ( nMinSizeX < 1 ) { nMinSizeX = 1; }
		nMinSizeX = nMinSizeX*10;
		if ( nMinSizeX < 400 ) { nMinSizeX = 400; }
		if ( nMinSizeX > 1000 ) { nMinSizeX = 1000; }		
		nMinSizeY = nMinSizeX;
	}
	else
	{		
		nMinSizeX = (int)(MinSize/100);
		if ( nMinSizeX < 1 ) { nMinSizeX = 1; }
		nMinSizeX = nMinSizeX*5;
		if ( nMinSizeX < 200 ) { nMinSizeX = 200; }
		if ( nMinSizeX > 800 ) { nMinSizeX = 800; }
		nMinSizeY = nMinSizeX;
	}
	if ( MaxSize > (MinSize*1.25) )//長條型
	{
		nMinSizeX = MaxSize*0.30;
		nMinSizeY = MaxSize*0.30;		
		nMinSizeX = nMinSizeX/50;
		nMinSizeY = nMinSizeY/50;
		if ( nMinSizeX < 1 ) { nMinSizeX = 1; }
		if ( nMinSizeY < 1 ) { nMinSizeY = 1; }
		nMinSizeX = nMinSizeX*50;
		nMinSizeY = nMinSizeY*50;
		if ( NULL == WndPtr_PadAlign )
		{
			if ( nMinSizeX <  400 ) { nMinSizeX =  400; }
			if ( nMinSizeX > 1000 ) { nMinSizeX = 1000; }
			if ( nMinSizeY <  400 ) { nMinSizeY =  400; }
			if ( nMinSizeY > 1000 ) { nMinSizeY = 1000; }
		}
		else
		{
			if ( nMinSizeX <  200 ) { nMinSizeX =  200; }
			if ( nMinSizeX >  800 ) { nMinSizeX =  800; }
			if ( nMinSizeY <  200 ) { nMinSizeY =  200; }
			if ( nMinSizeY >  800 ) { nMinSizeY =  800; }
		}
	}	

	ExtX = nMinSizeX;
	ExtY = nMinSizeY;
	ExtX = Param.dPartAlignExtendRange;
	ExtY = Param.dPartAlignExtendRange;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.5);
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.5);
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.5);
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.5);

	RgnWndExt.minX = psWnd.x - (szWndExt.cx*0.5);
	RgnWndExt.minY = psWnd.y - (szWndExt.cy*0.5);
	RgnWndExt.maxX = psWnd.x + (szWndExt.cx*0.5);
	RgnWndExt.maxY = psWnd.y + (szWndExt.cy*0.5);
	
	TREGION4D  BodyRgn;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();

	GetModelBodyBox().GetBoxRegion(BodyRgn);
	const double BodySizeX=BodyRgn.GetWidth();
	const double BodySizeY=BodyRgn.GetHeight();
	const double BodyHeight = GetModelBodyHeight();
	const double BodyArea   = BodySizeX*BodySizeY;
	const double BodyVolume = BodyArea*BodyHeight;	

	ALG_TYPE   AlgType = ALG_MODEL_MATCH;	
	switch ( PartAlignMode )
	{
	case PART_ALIGN_MODE_BY_OBJECT_MEASURE:
		AlgType = ALG_OBJECT_MEASURE;	
		break;
	default:
	case PART_ALIGN_MODE_BY_MODEL_MATCH_2D:
	case PART_ALIGN_MODE_BY_MODEL_MATCH_3D:
		AlgType = ALG_MODEL_MATCH;	
		break;
	}	
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_PART_ALIGN;	
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	
	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(LinkMode);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);
	WndPtr->SetWndExtendRangeX(ExtX);
	WndPtr->SetWndExtendRangeY(ExtY);	
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();	
	WndPtr->SetWndAlgType(AlgType);
	if ( true == Param.bUseLogic )
	{	WndPtr->SetWndLogicType(WND_LOGIC_DEFECT_ID);	}

	const double     HeightGap = Param.dPartAlignHeightTolerance;
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);
	if ( PART_ALIGN_MODE_BY_MODEL_MATCH_2D == PartAlignMode )
	{
		if ( CheckModelTypeUseTwoElectrode(ModelType) )
		{
			TALG_PARAM_RESIN_HEIGHT &hightDetectParm = AlgParam.GetAlgParamResinHeight();
			if ( false == Param.bUseLogic )
			{	hightDetectParm.enabled = true; }
			hightDetectParm.enableDoubleCheck = true;
		}
		if ( MDW_VERSION_2 == Param.eVersion )
		{
			switch ( ModelType )
			{			
			case MODEL_TYPE_CHIP_R:
				BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);		
				BinaryParam.SetFixedThresholdLow(80);
				BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_2D);				
				BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_RED);	
				break;
			default://彩色過濾
				BinaryParam.SetBinaryMode(BINARY_COLOR_FILTER);
				BinaryParam.SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_OTHERS_END);
				break;
			}			
		}
	}
	if ( PART_ALIGN_MODE_BY_MODEL_MATCH_3D == PartAlignMode )
	{
		//固定閥值
		TBINARY_FILTER BinaryFilter1;
		double BodyHeightLow = BodyHeight*0.75;
		double BodyHeightHigh = FIXED_THRESHOLD_MAX_3D;
		BinaryFilter1.FilterParam1 = 7;
		BinaryFilter1.FilterMode = NOISE_FILTER_OPEN;		
		if ( MDW_VERSION_2 == Param.eVersion )//彩色過濾
		{	
			BodyHeightLow = BodyHeight*0.80;
			BodyHeightHigh= BodyHeight*1.20;
			BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);		
		}
		BinaryParam.SetBinaryNoiseFilter1(BinaryFilter1);
		BinaryParam.SetRatioThresholdRatioLow(80);
		BinaryParam.SetRatioThresholdRatioHigh(120);
		BinaryParam.SetRatioThresholdTarget(BodyHeight);
		BinaryParam.SetFixedThresholdLow(BodyHeightLow);
		BinaryParam.SetFixedThresholdHigh(BodyHeightHigh);
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);			

		//Model Match
		AlgParam.GetAlgParamModelMatch().mmReCheckBox = FN_ENABLE;
	}
	if ( PART_ALIGN_MODE_BY_OBJECT_MEASURE == PartAlignMode )
	{	
		//固定閥值
		TBINARY_FILTER BinaryFilter1;
		const double BodyHeightLow = BodyHeight*0.85;
		BinaryFilter1.FilterParam1 = 7;
		BinaryFilter1.FilterMode = NOISE_FILTER_OPEN;
		BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);	
		BinaryParam.SetBinaryNoiseFilter1(BinaryFilter1);
		BinaryParam.SetRatioThresholdTarget(BodyHeight);
		BinaryParam.SetFixedThresholdLow(BodyHeightLow);
		BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_3D);		
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);					
	}
	//Object Measure
	TALG_PARAM_OBJECT_MEASURE &omParam = AlgParam.GetAlgParamObjectMeasure();//演算法-物件量測參數		
	omParam.omSizeXSpec = BodySizeX;
	omParam.omSizeXEnabled = true;
	omParam.omSizeYSpec = BodySizeY;
	omParam.omSizeYEnabled = true;
	omParam.omHeightSpec = BodyHeight;
	omParam.omHeightAverageMode = ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL;
	omParam.omHeightAveragePartialH = 100;
	omParam.omHeightAveragePartialL =  50;
	omParam.omHeightEnabled = true;
	omParam.omHeightDiffUSL =  HeightGap;
	omParam.omHeightDiffLSL = -HeightGap;
	omParam.omAreaSpec = BodyArea;
	omParam.omAreaEnabled = true;
	omParam.omVolumeSpec = BodyVolume;
	omParam.omVolumeEnabled = true;

	TPOINT2D Range;
	CalcModelOffsetLimit(Param, Range);	
	const double OffsetLimitX = Range.x;
	const double OffsetLimitY = Range.y;
	const double SkewLimit = Param.dPartAlignSkewLimit;
	AlgParam.SetAlgOffsetXUSL(OffsetLimitX);
	AlgParam.SetAlgOffsetXLSL(-OffsetLimitX);
	AlgParam.SetAlgOffsetYUSL(OffsetLimitY);
	AlgParam.SetAlgOffsetYLSL(-OffsetLimitY);
	AlgParam.SetAlgSkewUSL(SkewLimit);
	AlgParam.SetAlgSkewLSL(-SkewLimit);
	AlgParam.SetAlgSkewEnabled(true);
	AlgParam.SetAlgPatternSimilarityUSL(100);
	AlgParam.SetAlgPatternSimilarityLSL(50);//AK ask

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);

	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PadAdjust(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-焊盤調整
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_PadAdjustKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_PadAdjustKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PadAdjustKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-焊盤調整
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnPad;
	const double SkewLimit = 0;
	const double OffsetLimit = 1000;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;		
	
	double LinkRatio = 50;
	const double ExtX = Param.dPadAdjustExtendRange;
	const double ExtY = Param.dPadAdjustExtendRange;
	
	MODEL_TYPE   ModelType=GetModelType();
	ALG_TYPE     AlgType = ALG_MODEL_MATCH;
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_PAD_ADJUST;	
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	WND_RGN_LINK_MODE WndRgnLinkMode = CAOIModel::ObtainModelDefaultWndRegionLinkMode(ModelType, WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Solder;//nFrameIndex_Align;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;//nFrameUniqueID_Align;	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);

	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);		
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			RgnWnd = RgnPad;
			switch ( LandType )
			{
			case LAND_TYPE_PAD:
				LinkRatio = 100;
				break;
			case LAND_TYPE_ELECTRODE:
				if ( WND_RGN_LINK_PAD_TIP == WndRgnLinkMode )
				{	LinkRatio = 50;	}
				else
				{	LinkRatio = 100;	}
				break;
			case LAND_TYPE_IC_LEAD:
				if ( WND_RGN_LINK_PAD_TIP == WndRgnLinkMode )
				{	LinkRatio = 60;	}
				else
				{	LinkRatio = 100;	}
				break;
			case LAND_TYPE_CON_LEAD:
				if ( WND_RGN_LINK_PAD_TIP == WndRgnLinkMode )
				{	LinkRatio = 60;	}
				else
				{	LinkRatio = 100;	}
				break;
			case LAND_TYPE_DIP_LEAD:
				if ( WND_RGN_LINK_PAD_TIP == WndRgnLinkMode )
				{	LinkRatio = 50;	}
				else
				{	LinkRatio = 100;	}
				break;
			default:
				LinkRatio = 100;
				break;
			}

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(true);
			WndPtr->SetWndRgnLinkMode(WndRgnLinkMode);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			if ( LAND_TYPE_DIP_LEAD==LandType )
			{	WndPtr->SetWndShapeMode(BOX_SHAPE_ELLIPSE);	}
			WndPtr->SetWndFollowMode(WndFollowMode);
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendRangeX(ExtX);
			WndPtr->SetWndExtendRangeY(ExtY);
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();			

			CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				WndPtr->SetWndRgnLinkRatioY(LinkRatio);
				AlgParam.SetAlgOffsetXEnabled(true);//false
				AlgParam.SetAlgOffsetYEnabled(true);
				break;
			case BOX_TOWARD_LEFT:
			case BOX_TOWARD_RIGHT:
				WndPtr->SetWndRgnLinkRatioX(LinkRatio);
				AlgParam.SetAlgOffsetXEnabled(true);
				AlgParam.SetAlgOffsetYEnabled(true);//false
				break;
			}
			AlgParam.GetAlgImageBinParam().SetGrayGainValue(1.0);
			AlgParam.GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
			AlgParam.GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);
			AlgParam.GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_LIGHTNESS);

			AlgParam.SetAlgOffsetXUSL(OffsetLimit);
			AlgParam.SetAlgOffsetXLSL(-OffsetLimit);
			AlgParam.SetAlgOffsetYUSL(OffsetLimit);
			AlgParam.SetAlgOffsetYLSL(-OffsetLimit);
			AlgParam.SetAlgSkewUSL(SkewLimit);
			AlgParam.SetAlgSkewLSL(-SkewLimit);
			AlgParam.SetAlgPatternSimilarityUSL(100);
			AlgParam.SetAlgPatternSimilarityLSL(20);
			
			TALG_PARAM_MODEL_MATCH &mmParam = AlgParam.GetAlgParamModelMatch();			
			mmParam.mmDockMode = ALG_MATCH_DOCK_DISABLE;//ALG_MATCH_DOCK_TO_TIP;			

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			CAOIModel::AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LeadAdjust(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-引腳調整
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_LeadAdjustKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_LeadAdjustKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LeadAdjustKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-引腳調整
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	const double SkewLimit = 5;
	const double OffsetLimit = 1000;	
	TREGION4D    RgnWnd, RgnLead, RgnPad;	
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;		

	MODEL_TYPE   ModelType=GetModelType();
	ALG_TYPE     AlgType = ALG_MODEL_MATCH;
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_LEAD_ADJUST;	
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const double ExtX = Param.dLeadAdjustExtendRange;
	const double ExtY = Param.dLeadAdjustExtendRange;
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	WND_RGN_LINK_MODE WndRgnLinkMode = CAOIModel::ObtainModelDefaultWndRegionLinkMode(ModelType, WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Align;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Align;
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);		
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }			
			LandType   = LandPtr->GetLandType();
			if ( LAND_TYPE_PAD == LandType ) { continue; }
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
			RgnWnd = RgnLead;

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(true);
			WndPtr->SetWndRgnLinkMode(WndRgnLinkMode);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			WndPtr->SetWndFollowMode(WndFollowMode);
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendRangeX(ExtX);
			WndPtr->SetWndExtendRangeY(ExtY);
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();			
			
			CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				AlgParam.SetAlgOffsetXEnabled(true);//false
				AlgParam.SetAlgOffsetYEnabled(true);
				break;
			case BOX_TOWARD_LEFT:
			case BOX_TOWARD_RIGHT:
				AlgParam.SetAlgOffsetXEnabled(true);
				AlgParam.SetAlgOffsetYEnabled(true);//false
				break;
			}

			AlgParam.GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
			AlgParam.GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);
			AlgParam.SetAlgOffsetXUSL(OffsetLimit);
			AlgParam.SetAlgOffsetXLSL(-OffsetLimit);
			AlgParam.SetAlgOffsetYUSL(OffsetLimit);
			AlgParam.SetAlgOffsetYLSL(-OffsetLimit);			
			AlgParam.SetAlgSkewUSL(SkewLimit);
			AlgParam.SetAlgSkewLSL(-SkewLimit);
			AlgParam.SetAlgPatternSimilarityUSL(100);
			AlgParam.SetAlgPatternSimilarityLSL(20);

			TALG_PARAM_MODEL_MATCH &mmParam = AlgParam.GetAlgParamModelMatch();			
			switch ( WndRgnLinkMode )
			{
			case WND_RGN_LINK_LEAD_TIP:
				mmParam.mmDockMode = ALG_MATCH_DOCK_TO_SHOULDER;//靠根部
				break;
			case WND_RGN_LINK_LEAD_SHOULDER:
				mmParam.mmDockMode = ALG_MATCH_DOCK_TO_TIP;//靠前端
				break;				      
			default:
				mmParam.mmDockMode = ALG_MATCH_DOCK_DISABLE;//ALG_MATCH_DOCK_TO_SHOULDER;
				break;
			}			

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			CAOIModel::AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
		}
	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_Polarity(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-極性檢測	
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_PolarityKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_PolarityKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PolarityKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-極性檢測
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_PolarityKernel_v2(Param); }

	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	CAOIModel::GetModelBodyPos(psBody);
	CAOIModel::GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;	
	const double ExtX = Param.dPolarityExtendRange;
	const double ExtY = Param.dPolarityExtendRange;

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	double SizeU = 0;
	double SizeV = 0;
	double RatioU = 0.4;
	double RatioV = 0.25;
	const double GapU = 50;
	const double GapV = 50;
	const double MaxSizeU = 5000;
	const double MaxSizeV = 5000;
	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_POLARITY;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Text;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Text;

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }	

		RgnWnd.maxY = RgnBody.maxY-(GapV);
		RgnWnd.maxX = RgnBody.maxX-(GapU);
		RgnWnd.minX = RgnWnd.maxX-(SizeU);		
		RgnWnd.minY = RgnWnd.maxY-(SizeV);
		break;
	case BOX_TOWARD_LEFT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minX = RgnBody.minX+(GapV);
		RgnWnd.maxY = RgnBody.maxY-(GapU);
		RgnWnd.minY = RgnWnd.maxY-(SizeU);
		RgnWnd.maxX = RgnWnd.minX+(SizeV);
		break;
	case BOX_TOWARD_DOWN:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minY = RgnBody.minY+(GapV);
		RgnWnd.minX = RgnBody.minX+(GapU);
		RgnWnd.maxX = RgnWnd.minX+(SizeU);
		RgnWnd.maxY = RgnWnd.minY+(SizeV);
		break;
	case BOX_TOWARD_RIGHT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.maxX = RgnBody.maxX-(GapV);
		RgnWnd.minY = RgnBody.minY+(GapU);
		RgnWnd.maxY = RgnWnd.minY+(SizeU);
		RgnWnd.minX = RgnWnd.maxX-(SizeV);
		break;
	}	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->SetWndExtendRangeX(ExtX);
	WndPtr->SetWndExtendRangeY(ExtY);	
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();
	AlgParam.CheckAlgPatternFileUsed();
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);
	BinaryParam.SetBinaryMode(BINARY_COLOR_FILTER);

	TALG_PARAM_BRIGHT_RATIO   &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	brParam.brRatioLSL = 50.0;

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);
	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMissing(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體缺件
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_BodyMissingKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_BodyMissingKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMissingKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體缺件
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_BodyMissingKernel_v2(Param); }

	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;

	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	if ( BODY_MISSING_MODE_BY_VOLUME == Param.nBodyMissingMode )
	{	AlgType = ALG_OBJECT_MEASURE; }
	else
	{	AlgType = ALG_BRIGHT_RATIO; }

	double Size = 0;
	double SizeX = 0;
	double SizeY = 0;	
	double RatioX = 0.5;
	double RatioY = 0.5;	
	const double MaxSizeX = 1000;
	const double MaxSizeY = 1000;
	const double MinSizeX = 50;
	const double MinSizeY = 50;
	const double BodyHeight = GetModelBodyHeight();
	const double BodyArea = szWnd.cx*szWnd.cy;
	const double BodyVolume = BodyArea*BodyHeight;
	CHIP_SIZE_MODE ChipSizeMode = Param.eChipSizeMode;

	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();		
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_MISSING;	
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	
	if ( ALG_OBJECT_MEASURE == AlgType )
	{
		SizeX = szWnd.cx;
		SizeY = szWnd.cy;
	}
	else
	{
		SizeX = szWnd.cx*RatioX;
		if ( SizeX > MaxSizeX ) { SizeX = MaxSizeX; }
		if ( SizeX < MinSizeX ) { SizeX = MinSizeX; }
		SizeY = szWnd.cy*RatioY;
		if ( SizeY > MaxSizeY ) { SizeY = MaxSizeY; }	
		if ( SizeY < MinSizeY ) { SizeY = MinSizeY; }
		Size = MIN(SizeX, SizeY);	
		SizeX = SizeY = Size;
	}
	RgnWnd.minX = psBody.x-(SizeX/2.0);
	RgnWnd.minY = psBody.y-(SizeY/2.0);	
	RgnWnd.maxX = psBody.x+(SizeX/2.0);
	RgnWnd.maxY = psBody.y+(SizeY/2.0);	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	if ( true == UseExtendBox )
	{
		WndPtr->SetWndExtendRangeX(400);
		WndPtr->SetWndExtendRangeY(400);
	}
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);
	
	const double               RangeH=Param.dBodyMissingHeightTolerance;
	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	TALG_PARAM_BRIGHT_RATIO   &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	TALG_PARAM_OBJECT_MEASURE &omParam = AlgParam.GetAlgParamObjectMeasure();//演算法-物件量測參數
	TALG_PARAM_GROUP_COMPARE  &gcParam = AlgParam.GetAlgParamGroupCompare();
	
	AlgParam.CheckAlgPatternFileUsed();
	if ( FRAME_UNIQUE_ID_DLP==Param.nFrameUniqueID_3D )
	{
		unsigned int FrameIndex = Param.nFrameIndex_3D;
		unsigned int FrameUniqueID = Param.nFrameUniqueID_3D;		
		BinaryParam.SetBinaryFrameIndex(FrameIndex);
		BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID); 
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);		
		BinaryParam.SetRatioThresholdTarget(BodyHeight);
		brParam.brTargetValue = BodyHeight;				
	}
	if ( ALG_OBJECT_MEASURE == AlgType )
	{
		TBINARY_FILTER BinaryFilter1;
		const int BodyHeightLow = (int)(BodyHeight*0.85);
		AlgParam.SetAlgSkewEnabled(true);
		AlgParam.SetAlgScaleEnabled(true);
		AlgParam.SetAlgOffsetXEnabled(true);
		AlgParam.SetAlgOffsetYEnabled(true);				

		BinaryFilter1.FilterParam1 = 7;
		BinaryFilter1.FilterMode = NOISE_FILTER_OPEN;		
		BinaryParam.SetBinaryNoiseFilter1(BinaryFilter1);
		BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);
		BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_3D);
		BinaryParam.SetFixedThresholdLow(BodyHeightLow);		

		omParam.omSizeXSpec = SizeX;
		omParam.omSizeXEnabled = true;
		omParam.omSizeYSpec = SizeY;
		omParam.omSizeYEnabled = true;
		omParam.omHeightSpec = BodyHeight;
		omParam.omHeightAverageMode = ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL;
		omParam.omHeightAveragePartialH = 100;
		omParam.omHeightAveragePartialL =  50;
		omParam.omHeightEnabled = true;
		omParam.omAreaSpec = BodyArea;
		omParam.omAreaEnabled = true;
		omParam.omVolumeSpec = BodyVolume;
		omParam.omVolumeEnabled = true;
	}
	else
	{
		brParam.brAverageMode = ALG_BRIGHT_AVERAGE_PARTIAL;	
		brParam.brAveragePartialL = 10;
		brParam.brAveragePartialH = 40;

		brParam.brRatioUSL = 125;
		brParam.brRatioLSL =  75;
		brParam.brToleranceUSL =  RangeH;
		brParam.brToleranceLSL = -RangeH;
		if ( FRAME_UNIQUE_ID_DLP==Param.nFrameUniqueID_3D )
		{	
			brParam.brRatioEnabled = false;			
			brParam.brToleranceEnabled = true;
			gcParam.gc3DHeightEnabled = false;//關閉
			gcParam.gc3DHeightBaseMode = ALG_3D_BASE_HEIGHT_AVE;
		}
		else
		{	
			brParam.brRatioEnabled = true;			
			brParam.brToleranceEnabled = false;
		}
	}
	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);
	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyTilt(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-立碑檢測	
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_BodyTiltKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_BodyTiltKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyTiltKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體傾斜	
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_BodyTiltKernel_v2(Param); }	

	bool IsOK = false;	
	switch ( Param.nBodyTiltNum )
	{
	case 2:		IsOK = AddModelDefaultWnd_BodyTiltKernel_2(Param);	break;
	case 4:		IsOK = AddModelDefaultWnd_BodyTiltKernel_4(Param);	break;	
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyTiltKernel_2(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體傾斜-2框	
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }	
	
	double SizeU = 0;
	double SizeV = 0;
	double RatioU = 0.30;//0.40;
	double RatioV = 0.15;//0.25;	
	const double GapV = 50;	
	const double Theata=20;//頃斜角度
	const double MaxSizeU = 2500;
	const double MaxSizeV = 1250;		
	const double BodyHeight = GetModelBodyHeight();
	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_TILT;	
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }		
		
		RgnWnd.maxY = RgnBody.maxY-GapV;
		RgnWnd.minX = psWnd.x-SizeU;
		RgnWnd.maxX = psWnd.x+SizeU;
		RgnWnd.minY = RgnWnd.maxY-(SizeV);
		break;
	case BOX_TOWARD_LEFT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.minX = RgnBody.minX+GapV;
		RgnWnd.minY = psWnd.y-(SizeU);
		RgnWnd.maxY = psWnd.y+(SizeU);
		RgnWnd.maxX = RgnWnd.minX+(SizeV);
		break;
	case BOX_TOWARD_DOWN:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.minY = RgnBody.minY+GapV;
		RgnWnd.minX = psWnd.x-(SizeU);
		RgnWnd.maxX = psWnd.x+(SizeU);
		RgnWnd.maxY = RgnWnd.minY+(SizeV);
		break;
	case BOX_TOWARD_RIGHT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.maxX = RgnBody.maxX-GapV;
		RgnWnd.minY = psWnd.y-(SizeU);
		RgnWnd.maxY = psWnd.y+(SizeU);
		RgnWnd.minX = RgnWnd.maxX-(SizeV);
		break;
	}
	
	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	const double               GapH=Param.dBodyTiltHeightTolerance;
	const double               TolGapH=Param.dBodyMissingHeightTolerance;	
	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	TALG_PARAM_BRIGHT_RATIO  &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	TALG_PARAM_GROUP_COMPARE &gcParam = AlgParam.GetAlgParamGroupCompare();

	AlgParam.CheckAlgPatternFileUsed();
	brParam.brRatioUSL = 125;
	brParam.brRatioLSL =  75;
	brParam.brToleranceUSL =  TolGapH;
	brParam.brToleranceLSL = -TolGapH;
	if ( FRAME_UNIQUE_ID_DLP==Param.nFrameUniqueID_3D )
	{	
		unsigned int FrameIndex = Param.nFrameIndex_3D;
		unsigned int FrameUniqueID = Param.nFrameUniqueID_3D;		
		BinaryParam.SetBinaryFrameIndex(FrameIndex);
		BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID); 
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
		BinaryParam.SetRatioThresholdTarget(BodyHeight);

		brParam.brTargetValue = BodyHeight;		
		brParam.brRatioEnabled = false;			
		brParam.brToleranceEnabled = false;
		gcParam.gc3DHeightEnabled = true;
		gcParam.gc3DHeightBaseMode = ALG_3D_BASE_HEIGHT_AVE;
		gcParam.gc3DHeightUSL    =  GapH*0.5;
		gcParam.gc3DHeightLSL    = -GapH*0.5;
	}
	else
	{	
		brParam.brRatioEnabled = true;			
		brParam.brToleranceEnabled = false;
	}

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);
	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	

	//Add Other one
	CAOIWnd *WndPtr2 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr2 )
	{
		WndPtr2->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr2->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr2->MirrorWndYAxis(psBody.x);
			break;
		}
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyTiltKernel_4(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體傾斜-4框		
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	double SizeU = 0;
	double SizeV = 0;
	double RatioU = 0.30;//0.40;
	double RatioV = 0.15;//0.25;	
	const double GapU = 50;
	const double GapV = 50;
	const double MaxSizeU = 1000;
	const double MaxSizeV = 1000;
	const double BodyHeight = GetModelBodyHeight();
	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_TILT;	
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }	

		RgnWnd.maxY = RgnBody.maxY-(GapV);
		RgnWnd.maxX = RgnBody.maxX-(GapU);
		RgnWnd.minX = RgnWnd.maxX-(SizeU);		
		RgnWnd.minY = RgnWnd.maxY-(SizeV);
		break;
	case BOX_TOWARD_LEFT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minX = RgnBody.minX+(GapV);
		RgnWnd.maxY = RgnBody.maxY-(GapU);
		RgnWnd.minY = RgnWnd.maxY-(SizeU);
		RgnWnd.maxX = RgnWnd.minX+(SizeV);
		break;
	case BOX_TOWARD_DOWN:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minY = RgnBody.minY+(GapV);
		RgnWnd.minX = RgnBody.minX+(GapU);
		RgnWnd.maxX = RgnWnd.minX+(SizeU);
		RgnWnd.maxY = RgnWnd.minY+(SizeV);
		break;
	case BOX_TOWARD_RIGHT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.maxX = RgnBody.maxX-(GapV);
		RgnWnd.minY = RgnBody.minY+(GapU);
		RgnWnd.maxY = RgnWnd.minY+(SizeU);
		RgnWnd.minX = RgnWnd.maxX-(SizeV);
		break;
	}
	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	const double              GapH=Param.dBodyTiltHeightTolerance;
	const double              TolGapH=Param.dBodyMissingHeightTolerance;	
	CAlgParam                &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam          &BinaryParam = AlgParam.GetAlgImageBinParam();	
	TALG_PARAM_BRIGHT_RATIO  &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	TALG_PARAM_GROUP_COMPARE &gcParam = AlgParam.GetAlgParamGroupCompare();

	AlgParam.CheckAlgPatternFileUsed();	
	brParam.brRatioUSL = 125;
	brParam.brRatioLSL =  75;
	brParam.brToleranceUSL =  TolGapH;
	brParam.brToleranceLSL = -TolGapH;		
	if ( FRAME_UNIQUE_ID_DLP==Param.nFrameUniqueID_3D )
	{	
		unsigned int FrameIndex = Param.nFrameIndex_3D;
		unsigned int FrameUniqueID = Param.nFrameUniqueID_3D;		
		BinaryParam.SetBinaryFrameIndex(FrameIndex);
		BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID); 
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
		BinaryParam.SetRatioThresholdTarget(BodyHeight);

		brParam.brTargetValue = BodyHeight;
		brParam.brRatioEnabled = false;			
		brParam.brToleranceEnabled = false;
		gcParam.gc3DHeightEnabled = true;
		gcParam.gc3DHeightBaseMode = ALG_3D_BASE_HEIGHT_AVE;		
		gcParam.gc3DHeightUSL    =  GapH/2;
		gcParam.gc3DHeightLSL    = -GapH/2;
	}
	else
	{	
		brParam.brRatioEnabled = true;			
		brParam.brToleranceEnabled = false;
	}	

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);
	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	

	//return true;//delete every time, so only create one wnd 
	//Add Other one	
	CAOIWnd *WndPtr2 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr2 )
	{
		WndPtr2->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr2->MirrorWndYAxis(psBody.x);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr2->MirrorWndXAxis(psBody.y);
			break;
		}
	}

	CAOIWnd *WndPtr3 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr3 )
	{
		WndPtr3->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr3->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr3->MirrorWndYAxis(psBody.x);
			break;
		}
	}
	CAOIWnd *WndPtr4 = CAOIModel::CopyModelWnd(WndPtr2);
	if ( NULL != WndPtr4 )
	{
		WndPtr4->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr4->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr4->MirrorWndYAxis(psBody.x);
			break;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMount(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體錯色
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_BodyMountKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_BodyMountKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMountKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-側立檢測	
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_BodyMountKernel_v2(Param); }		

	bool IsOK = false;
	switch ( Param.nBodyMountNum )
	{
	case 2:
		IsOK = CAOIModel::AddModelDefaultWnd_BodyMountKernel_2(Param);
		break;
	case 4:
		IsOK = CAOIModel::AddModelDefaultWnd_BodyMountKernel_4(Param);
		break;
	default:
		IsOK = CAOIModel::AddModelDefaultWnd_BodyMountKernel_1(Param);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMountKernel_1(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體裝貼-1框
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	CalcModelBodyRegionNoLead(RgnBody);
	psBody.x = RgnBody.GetCpX();
	psBody.y = RgnBody.GetCpY();
	szBody.cx = RgnBody.GetWidth();
	szBody.cy = RgnBody.GetHeight();

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.25);//0.4
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.25);//0.4
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.25);//0.4
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.25);//0.4

	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_MOUNT;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	const CColorGroup &BodyColorGroup = GetModelBodyColorGroup();
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();	
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();	
	
	BinaryParam.SetBinaryMode(BINARY_COLOR_FILTER);
	BinaryParam.SetBinaryColorGroup(BodyColorGroup);
	//BinaryParam.SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_BODY_BEGIN);

	TALG_PARAM_BRIGHT_RATIO   &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	brParam.brRatioLSL = 50;
	brParam.brRatioEnabled = true;
	brParam.brToleranceEnabled = false;	

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);

	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMountKernel_2(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體裝貼-2框
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	CalcModelBodyRegionNoLead(RgnBody);
	psBody.x = RgnBody.GetCpX();
	psBody.y = RgnBody.GetCpY();
	szBody.cx = RgnBody.GetWidth();
	szBody.cy = RgnBody.GetHeight();

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }	

	double SizeU = 0;
	double SizeV = 0;
	double RatioU = 0.4;
	double RatioV = 0.25;
	const double GapV = 50;
	const double MaxSizeU = 2500;
	const double MaxSizeV = 1250;

	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const CColorGroup &BodyColorGroup = GetModelBodyColorGroup();
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_MOUNT;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }		

		RgnWnd.maxY = RgnBody.maxY-GapV;
		RgnWnd.minX = psWnd.x-SizeU;
		RgnWnd.maxX = psWnd.x+SizeU;
		RgnWnd.minY = RgnWnd.maxY-(SizeV);
		break;
	case BOX_TOWARD_LEFT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.minX = RgnBody.minX+GapV;
		RgnWnd.minY = psWnd.y-(SizeU);
		RgnWnd.maxY = psWnd.y+(SizeU);
		RgnWnd.maxX = RgnWnd.minX+(SizeV);
		break;
	case BOX_TOWARD_DOWN:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.minY = RgnBody.minY+GapV;
		RgnWnd.minX = psWnd.x-(SizeU);
		RgnWnd.maxX = psWnd.x+(SizeU);
		RgnWnd.maxY = RgnWnd.minY+(SizeV);
		break;
	case BOX_TOWARD_RIGHT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }
		RgnWnd.maxX = RgnBody.maxX-GapV;
		RgnWnd.minY = psWnd.y-(SizeU);
		RgnWnd.maxY = psWnd.y+(SizeU);
		RgnWnd.minX = RgnWnd.maxX-(SizeV);
		break;
	}
	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();	
	BinaryParam.SetBinaryMode(BINARY_COLOR_FILTER);
	BinaryParam.SetBinaryColorGroup(BodyColorGroup);
	//BinaryParam.SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_BODY_BEGIN);

	TALG_PARAM_BRIGHT_RATIO   &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數		
	brParam.brRatioLSL = 50;
	brParam.brRatioEnabled = true;
	brParam.brToleranceEnabled = false;
	
	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);
	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	

	//Add Other one
	CAOIWnd *WndPtr2 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr2 )
	{
		WndPtr2->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr2->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr2->MirrorWndYAxis(psBody.x);
			break;
		}
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyMountKernel_4(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體裝貼-4框
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	CalcModelBodyRegionNoLead(RgnBody);
	psBody.x = RgnBody.GetCpX();
	psBody.y = RgnBody.GetCpY();
	szBody.cx = RgnBody.GetWidth();
	szBody.cy = RgnBody.GetHeight();

	psWnd = psBody;
	szWnd = szBody;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	double SizeU = 0;
	double SizeV = 0;
	double RatioU = 0.4;
	double RatioV = 0.25;
	const double GapU = 50;
	const double GapV = 50;
	const double MaxSizeU = 500;
	const double MaxSizeV = 500;

	ALG_TYPE   AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const CColorGroup &BodyColorGroup = GetModelBodyColorGroup();
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_MOUNT;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }	

		RgnWnd.maxY = RgnBody.maxY-(GapV);
		RgnWnd.maxX = RgnBody.maxX-(GapU);
		RgnWnd.minX = RgnWnd.maxX-(SizeU);		
		RgnWnd.minY = RgnWnd.maxY-(SizeV);
		break;
	case BOX_TOWARD_LEFT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minX = RgnBody.minX+(GapV);
		RgnWnd.maxY = RgnBody.maxY-(GapU);
		RgnWnd.minY = RgnWnd.maxY-(SizeU);
		RgnWnd.maxX = RgnWnd.minX+(SizeV);
		break;
	case BOX_TOWARD_DOWN:
		SizeU = szWnd.cx*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cy*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.minY = RgnBody.minY+(GapV);
		RgnWnd.minX = RgnBody.minX+(GapU);
		RgnWnd.maxX = RgnWnd.minX+(SizeU);
		RgnWnd.maxY = RgnWnd.minY+(SizeV);
		break;
	case BOX_TOWARD_RIGHT:
		SizeU = szWnd.cy*RatioU;
		if ( SizeU > MaxSizeU ) { SizeU = MaxSizeU; }
		SizeV = szWnd.cx*RatioV;
		if ( SizeV > MaxSizeV ) { SizeV = MaxSizeV; }

		RgnWnd.maxX = RgnBody.maxX-(GapV);
		RgnWnd.minY = RgnBody.minY+(GapU);
		RgnWnd.maxY = RgnWnd.minY+(SizeU);
		RgnWnd.minX = RgnWnd.maxX-(SizeV);
		break;
	}
	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();	
	BinaryParam.SetBinaryMode(BINARY_COLOR_FILTER);
	BinaryParam.SetBinaryColorGroup(BodyColorGroup);
	//BinaryParam.SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_BODY_BEGIN);

	TALG_PARAM_BRIGHT_RATIO   &brParam = AlgParam.GetAlgParamBrightRatio();//演算法-亮度比例參數	
	brParam.brRatioLSL = 50;
	brParam.brRatioEnabled = true;
	brParam.brToleranceEnabled = false;	

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);
	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	

	return true;//delete every time, so only create one wnd 
	//Add Other one	
	CAOIWnd *WndPtr2 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr2 )
	{
		WndPtr2->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr2->MirrorWndYAxis(psBody.x);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr2->MirrorWndXAxis(psBody.y);
			break;
		}
	}

	CAOIWnd *WndPtr3 = CAOIModel::CopyModelWnd(WndPtr);
	if ( NULL != WndPtr3 )
	{
		WndPtr3->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr3->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr3->MirrorWndYAxis(psBody.x);
			break;
		}
	}
	CAOIWnd *WndPtr4 = CAOIModel::CopyModelWnd(WndPtr2);
	if ( NULL != WndPtr4 )
	{
		WndPtr4->SetWndSelected(false);
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
		case BOX_TOWARD_DOWN:
			WndPtr4->MirrorWndXAxis(psBody.y);
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			WndPtr4->MirrorWndYAxis(psBody.x);
			break;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_TextWrong(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-錯件檢測	
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_TextWrongKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_TextWrongKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_TextWrongKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-錯件檢測	
{
	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;
	
	CAOIModel::GetModelBodyPos(psBody);
	CAOIModel::GetModelBodyRegion(RgnBody);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;

	psWnd = psBody;
	szWnd = szBody;	

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	if ( szWnd.cx > 2000 ) { szWnd.cx = 2000; }
	if ( szWnd.cy > 2000 ) { szWnd.cy = 2000; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.25);
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.25);
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.25);
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.25);

	int       DivideX = 3;
	int       DivideY = 1;
	const int DivideU = 1;
	const int DivideV = 1;

	ALG_TYPE   AlgType = ALG_CHAR_VERIFY;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_WRONG_TEXT;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	const bool bPolarity = AOIDataDefine.GetModelTypePolarity(Param.eModelType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Text;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Text;

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		DivideX = DivideU;
		DivideY = DivideV;
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		DivideX = DivideV;
		DivideY = DivideU;
		break;
	}
	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);	
	WndPtr->SetWndRegion(RgnWnd);		
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendRangeX(400);
	WndPtr->SetWndExtendRangeY(400);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();	
	//WndPtr->BuildWndRoiWndList(DivideX, DivideY, 50, 50);
	WndPtr->SetWndAlgType(AlgType);
	if ( MDW_VERSION_2 == Param.eVersion )
	{	WndPtr->SetWndLogicType(WND_LOGIC_DEFECT_ID);	}

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();
	TALG_PARAM_CHAR_VERIFY    &cvParam = AlgParam.GetAlgParamCharVerify();
	TALG_PARAM_AI_MODEL       &aiParam = AlgParam.GetAlgParamAiModel();
	const bool bShowX = AlgParam.CheckAlgShowOffsetByAlgType(AlgType);
	const bool bShowY = AlgParam.CheckAlgShowOffsetByAlgType(AlgType);

	AlgParam.CheckAlgPatternFileUsed();
	AlgParam.SetAlgOffsetXEnabled(bShowX);
	AlgParam.SetAlgOffsetYEnabled(bShowY);
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);
	//BinaryParam.SetBinaryMode(BINARY_DYNAMIC_THRESHOLD);
	if ( false == bPolarity )
	{	AlgParam.SetAlgPatternPolarity(2); }	

	cvParam.cvCellScoreMin = 60;
	aiParam.aiModelID = ALG_AI_MODEL_OCR_01;

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);

	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyDamaged(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-本體破損
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_BodyDamagedKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_BodyDamagedKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_BodyDamagedKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-破損檢測	
{	
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_BodyDamagedKernel_v2(Param); }

	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;	
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	CalcModelBodyRegionNoLead(RgnBody);
	psBody.x = RgnBody.GetCpX();
	psBody.y = RgnBody.GetCpY();
	szBody.cx = RgnBody.GetWidth();
	szBody.cy = RgnBody.GetHeight();

	psWnd = psBody;
	szWnd = szBody;
	
	const double ScaleX=0.95;
	const double ScaleY=0.95;
	const double GapX=MIN(100, szWnd.cx*(1.0-ScaleX));
	const double GapY=MIN(100, szWnd.cy*(1.0-ScaleY));

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.50) + GapX;//*0.5
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.50) + GapY;//*0.5
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.50) - GapX;//*0.5
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.50) - GapY;//*0.5

	ALG_TYPE   AlgType = ALG_BLOB_COUNT;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();	
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_BODY_DAMAGED;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	
	unsigned int FrameIndex = Param.nFrameIndex_High;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_High;	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndRgnLinkRatioX(ScaleX*100.0);
	WndPtr->SetWndRgnLinkRatioX(ScaleY*100.0);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();	
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();	
	BinaryParam.SetBinaryInvert(true);
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);	
	BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);
	BinaryParam.SetFixedThresholdHigh(255);
	BinaryParam.SetFixedThresholdLow(80);	

	TALG_PARAM_BLOB_COUNT   &blobParam = AlgParam.GetAlgParamBlobCount();	
	blobParam.bcAreaSizeMinEnabled = true;
	blobParam.bcAreaSizeMin = 1000;
	blobParam.bcCountUSL = 0;
	blobParam.bcCountLSL = 0;

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_OuterShort(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-外接短路檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_OuterShortKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_OuterShortKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_OuterShortKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-外接短路檢測	
{
	if ( true == Param.bBridgeUse2D )
	{
		if ( AddModelDefaultWnd_OuterShortKernelFn(Param, false) == false )
		{	return false; }
	}
	if ( true == Param.bBridgeUse3D )
	{
		if ( AddModelDefaultWnd_OuterShortKernelFn(Param, true) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_OuterShortKernelFn(const TMODEL_DEFAULT_WND_PARAM &Param, bool bUse3D)//增加模組檢測框-外接短路檢測
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_OuterShortKernelFn_v2(Param, bUse3D);	}

	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd, psPad;
	TSIZE2D   szBody, szWnd, szPad;	
	TREGION4D RgnBody, RgnWnd, RgnPad;
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	GetModelPadRegion(-1, -1, RgnPad);
	szBody.cx = RgnBody.maxX-RgnBody.minX;
	szBody.cy = RgnBody.maxY-RgnBody.minY;
	szPad.cx = RgnPad.maxX-RgnPad.minX;
	szPad.cy = RgnPad.maxY-RgnPad.minY;	

	psWnd = psBody;
	szWnd = szBody;

	psWnd = psPad;
	szWnd = szPad;

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.5);
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.5);
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.5);
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.5);	
	const double ExtX = Param.dBridgeExtendRange;
	const double ExtY = Param.dBridgeExtendRange;
	const bool   bBridgeTwoSide=Param.bBridgeTwoSide;//雙邊增加檢測框

	ALG_TYPE   AlgType = ALG_OUTER_SHORT;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_BRIDGE;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;	
	int          FixedThreshold = 128;

	if ( true == bUse3D )
	{
		FrameIndex = Param.nFrameIndex_3D;
		FrameUniqueID = Param.nFrameUniqueID_3D;	
	}

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_PAD_BODY_RGN);//WND_RGN_LINK_NONE, WND_RGN_LINK_PAD_RGN
	WndPtr->SetWndDefectID(WndDefectID);	
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);	
	WndPtr->SetWndRegion(RgnWnd);		
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendRangeX(ExtX);
	WndPtr->SetWndExtendRangeY(ExtY);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();

	AlgParam.CheckAlgPatternFileUsed();
	if ( true == bBridgeTwoSide )
	{
		ALG_OUTER_SHORT_EXT_MODE ExtMode=ALG_OUTER_SHORT_EXT_BOTH;
		AlgParam.GetAlgParamOuterShort().osLine_R.olExtMode=ExtMode;
		AlgParam.GetAlgParamOuterShort().osLine_T.olExtMode=ExtMode;
		AlgParam.GetAlgParamOuterShort().osLine_L.olExtMode=ExtMode;
		AlgParam.GetAlgParamOuterShort().osLine_B.olExtMode=ExtMode;
	}
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);	
	BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);
	if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
	{
		BinaryParam.SetFixedThresholdLow(100);	
		BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_3D);
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
	}
	else
	{			
		FixedThreshold = 80;
		BinaryParam.SetFixedThresholdLow(FixedThreshold);	 
		BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_2D);
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_LIGHTNESS);
	}

	TBINARY_FILTER BinaryFilter;
	BinaryFilter.FilterMode = NOISE_FILTER_OPEN;
	BinaryFilter.FilterParam1 = 3;
	BinaryParam.SetBinaryNoiseFilter1(BinaryFilter);

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	CAOIModel::AddModelWndPtr(WndPtr, false);

	WndPtr->SetWndClassID(m_ModelActClassID);
	WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
	WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
	WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_ForeignBody(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-異物檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_ForeignBodyKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_ForeignBodyKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_ForeignBodyKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-破損檢測	
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return true; }

	CAOIWnd *WndPtr = NULL;
	TPOINT2D  psBody, psWnd;
	TSIZE2D   szBody, szWnd;	
	TREGION4D RgnBody, RgnWnd;	
	
	GetModelBodyPos(psBody);
	GetModelBodyRegion(RgnBody);
	CalcModelBodyRegionNoLead(RgnBody);
	psBody.x = RgnBody.GetCpX();
	psBody.y = RgnBody.GetCpY();
	szBody.cx = RgnBody.GetWidth();
	szBody.cy = RgnBody.GetHeight();

	psWnd = psBody;
	szWnd = szBody;
	
	const double ScaleX=0.95;
	const double ScaleY=0.95;
	const double GapX=MIN(100, szWnd.cx*(1.0-ScaleX));
	const double GapY=MIN(100, szWnd.cy*(1.0-ScaleY));

	WndPtr = AOIObjManager.CreateWndObj();
	if ( NULL == WndPtr ) { return false; }

	RgnWnd.minX = psWnd.x - (szWnd.cx*0.50) + GapX;//*0.5
	RgnWnd.minY = psWnd.y - (szWnd.cy*0.50) + GapY;//*0.5
	RgnWnd.maxX = psWnd.x + (szWnd.cx*0.50) - GapX;//*0.5
	RgnWnd.maxY = psWnd.y + (szWnd.cy*0.50) - GapY;//*0.5

	ALG_TYPE   AlgType = ALG_BLOB_COUNT;
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();	
	const int WndGouprID = GetModelWndFreeGroupID();
	const int WndBandID = GetModelWndFreeBandID(WndGouprID);
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_FOREIGN_BODY;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);	
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	
	unsigned int FrameIndex = Param.nFrameIndex_Low;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Low;	

	WndPtr->SetWndToward(Toward);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(false);
	WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
	WndPtr->SetWndRgnLinkRatioX(ScaleX*100.0);
	WndPtr->SetWndRgnLinkRatioX(ScaleY*100.0);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);	
	WndPtr->SetWndSelected(true);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();	
	WndPtr->SetWndAlgType(AlgType);

	CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();	
	AlgParam.CheckAlgPatternFileUsed();	
	BinaryParam.SetBinaryInvert(true);
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);	
	BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);
	BinaryParam.SetFixedThresholdHigh(255);
	BinaryParam.SetFixedThresholdLow(80);	

	TALG_PARAM_BLOB_COUNT   &blobParam = AlgParam.GetAlgParamBlobCount();	
	blobParam.bcAreaSizeMinEnabled = true;
	blobParam.bcAreaSizeMin = 2000;
	blobParam.bcCountUSL = 0;
	blobParam.bcCountLSL = 0;

	ApplyModelDefaultBodyWnd(WndPtr, Param);
	InitialModelWndPtr(WndPtr);
	AddModelWndPtr(WndPtr, false);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_OtherDefect(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-其餘瑕疵檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_OtherDefectKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_OtherDefectKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_OtherDefectKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-其餘瑕疵檢測
{	
	CAOIModel   *RefModelPtr = (CAOIModel*)(Param.pModel);	
	if ( NULL == RefModelPtr ) { return true; }
	CAOIWnd *WndPtr = NULL;
	WND_DEFECT_ID WndDefectID;	
	TMODEL_DEFAULT_WND_PARAM TempParam=Param;
	const double AttachedAngle = GetModelAttachedAngle();
	std::vector<WND_DEFECT_ID> SelfWndDefectIDList;//本身已存在瑕疵代碼列表
	std::vector<WND_DEFECT_ID> BodyWndDefectIDList;//本體上瑕疵檢測框列表
	std::vector<WND_DEFECT_ID> LandWndDefectIDList;//特徵框上瑕疵檢測框列表
	const size_t WndCount=GetModelWndCount();
	const size_t RefWndCount=RefModelPtr->GetModelWndCount();
	const std::vector<WND_DEFECT_ID> &WndDefectIDList=AOIDataCollect.GetWndDefectIDList();
	const size_t WndDefectIDCount=WndDefectIDList.size();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);	
	TempParam.SetAll(true);
	
	for ( size_t i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		bool bExist=false;
		for ( size_t j=0; j<SelfWndDefectIDList.size(); j++ )
		{
			if ( WndDefectID == SelfWndDefectIDList[j] )
			{
				bExist = true;
				break;
			}
		}
		if ( false == bExist )
		{	SelfWndDefectIDList.push_back(WndDefectID);	}
	}
	const size_t SelfWndDefectIDCount=SelfWndDefectIDList.size();

	for ( size_t i=0; i<RefWndCount; i++ )
	{
		WndPtr = RefModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		if ( TempParam.GetEnable(WndDefectID) == true )
		{	continue; }

		bool bExist=false;
		for ( size_t j=0; j<SelfWndDefectIDCount; j++ )
		{
			if ( SelfWndDefectIDList[j] == WndDefectID )
			{	
				bExist=true; 
				break;
			}
		}
		if ( true == bExist ) 
		{	continue; }

		if ( NULL == WndPtr->GetWndLandPtr() )
		{
			for ( size_t j=0; j<BodyWndDefectIDList.size(); j++ )
			{
				if ( BodyWndDefectIDList[j] == WndDefectID )
				{	
					bExist=true; 
					break;
				}
			}
			if ( false == bExist )
			{	BodyWndDefectIDList.push_back(WndDefectID);	 }
		}
		else
		{
			for ( size_t j=0; j<LandWndDefectIDList.size(); j++ )
			{
				if ( LandWndDefectIDList[j] == WndDefectID )
				{	
					bExist=true; 
					break;
				}
			}
			if ( false == bExist )
			{	LandWndDefectIDList.push_back(WndDefectID);	 }			
		}		
	}
	const size_t BodyWndDefectIDCount=BodyWndDefectIDList.size();
	const size_t LandWndDefectIDCount=LandWndDefectIDList.size();
	if ( 0==BodyWndDefectIDCount && 0==LandWndDefectIDCount )
	{	return true; }
	
	int WndGouprID = 0;
	int WndBandID = 0;
	int DefectGroupID = 0;		
	if ( false == IsExceptionAngle )
	{
		RefModelPtr = RefModelPtr->CloneModelObj();
		if ( NULL == RefModelPtr ) { return true; }
		RefModelPtr->RotateModel(AttachedAngle, 0, 0);
		RefModelPtr->SetModelAutoDeleteImageFolder(false);
	}
	
	CAOIWnd *RefWndPtr = NULL;		
	BOX_TOWARD Toward = GetModelBodyBox().GetBoxToward();	
	BOX_TOWARD RefToward = RefModelPtr->GetModelBodyBox().GetBoxToward();	
	for ( size_t i=0; i<BodyWndDefectIDCount; i++ )
	{
		WndDefectID = BodyWndDefectIDList[i];
		Toward=GetModelBodyBox().GetBoxToward();	
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);		
		DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);		
		CAOIWnd *RefWndPtr = RefModelPtr->GetModelWndPtrByDefectID(WndDefectID, DefectGroupID);			
		if ( NULL == RefWndPtr ) { return true; }				
		WndPtr = RefWndPtr->CloneWndObj();
		if ( NULL == WndPtr ) { return false; }				

		WndPtr->SetWndToward(Toward);
		WndPtr->SetWndBandID(WndBandID);
		WndPtr->SetWndGroupID(WndGouprID);		
		WndPtr->SetWndDefectID(WndDefectID);	
		WndPtr->SetWndDefectGroupID(DefectGroupID);				
		ApplyModelDefaultBodyWnd(WndPtr, Param);
		WndPtr->SetWndClassID(m_ModelActClassID);
		WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);	
		WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
		WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
		AddModelWndPtr(WndPtr, false);
	}
	//return true;
	
	TPOINT2D BoxPos, RefBoxPos, MovePos;
	int MaxLandGroupID=0, LandGroupID=0;	
	CAOILand *LandPtr = NULL;
	CAOILand *RefLandPtr = NULL;	
	const size_t LandCount=GetModelLandCount();
	for ( size_t j=0; j<LandWndDefectIDCount; j++ )
	{
		WndDefectID = LandWndDefectIDList[j];
		MaxLandGroupID = GetModelLandFreeGroupID();
		for ( size_t k=0; k<MaxLandGroupID; k ++ )
		{
			LandGroupID = k;
			WndGouprID = GetModelWndFreeGroupID();
			WndBandID = GetModelWndFreeBandID(WndGouprID);
			DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
			for ( size_t i=0; i<LandCount; i++ )
			{
				LandPtr = GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }			
				if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }								
				if ( RefModelPtr->GetModelLandWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID, RefLandPtr, RefWndPtr) == false ) { continue; }				
				if ( NULL == RefWndPtr ) { continue; }
				if ( NULL == RefLandPtr ) { continue; }
				WndPtr = RefWndPtr->CloneWndObj();
				if ( NULL == WndPtr ) { return false; }	
				LandPtr->GetLandBoxPtr()->GetBoxPos(BoxPos);
				RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefBoxPos);
				MovePos.x = BoxPos.x-RefBoxPos.x;
				MovePos.y = BoxPos.y-RefBoxPos.y;
				WndPtr->MoveWnd(MovePos);				

				Toward = LandPtr->GetLandToward();
				WndPtr->SetWndToward(Toward);
				WndPtr->SetWndBandID(WndBandID);
				WndPtr->SetWndGroupID(WndGouprID);		
				WndPtr->SetWndDefectID(WndDefectID);	
				WndPtr->SetWndDefectGroupID(DefectGroupID);			

				ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
				WndPtr->SetWndClassID(m_ModelActClassID);
				WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);	
				WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
				WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
				AddModelWndPtr(WndPtr, false);
				LandPtr->AddLandWndPtr(WndPtr);
			}
		}
	}	

	if ( RefModelPtr != Param.pModel )
	{	AOIObjManager.DestroyModelObj(RefModelPtr); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_DefaultModelDefect(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-預設模組全瑕疵
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_DefaultModelDefectKernel(Param);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_DefaultModelDefectKernel(Param);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_DefaultModelDefectKernel(const TMODEL_DEFAULT_WND_PARAM &Param)//增加模組檢測框-預設模組全瑕疵
{
	CAOIModel   *RefModelPtr = (CAOIModel*)(Param.pModel);	
	if ( NULL == RefModelPtr ) { return true; }		
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);	
	
	if ( GetModelWndCount() != 0 )
	{	
		ClearModelFolder(MODEL_CLEAR_FOLDER_PATTERNS);
		ClearModelAllWndList();	
	}
		
	if ( false == IsExceptionAngle )
	{
		RefModelPtr = RefModelPtr->CloneModelObj();
		if ( NULL == RefModelPtr ) { return true; }
		RefModelPtr->RotateModel(AttachedAngle, 0, 0);
		RefModelPtr->SetModelAutoDeleteImageFolder(false);
	}	
	
	ALG_TYPE AlgType;
	int FrameUniqueID=0;
	TPOINT2D BodyScale;
	WND_DEFECT_ID WndDefectID;	
	TSIZE2D BodySize, RefBodySize;	
	CAOIWnd *WndPtr = NULL, *RefWndPtr = NULL;
	CAOILand *LandPtr = NULL, *RefLandPtr = NULL;
	const double BodyHeight=GetModelBodyHeight();		
	const int FrameUniqueID_3D=Param.nFrameUniqueID_3D;	
	const CAOIBox &BodyBox=GetModelBodyBox();	
	const CAOIBox &RefBodyBox=RefModelPtr->GetModelBodyBox();
	const size_t RefWndCount=RefModelPtr->GetModelWndCount();	

	BodyBox.GetBoxSize(BodySize);
	RefBodyBox.GetBoxSize(RefBodySize);
	if ( fabs(RefBodySize.cx) < 0.00001 )
	{	BodyScale.x = 1.0;	}
	else
	{	BodyScale.x = BodySize.cx/RefBodySize.cx; }
	if ( fabs(RefBodySize.cy) < 0.00001 )
	{	BodyScale.y = 1.0;	}
	else
	{	BodyScale.y = BodySize.cy/RefBodySize.cy; }
	for ( size_t i=0; i<RefWndCount; i++ )
	{
		RefWndPtr = RefModelPtr->GetModelWndPtr(i, false);
		if ( NULL == RefWndPtr ) { continue; }		
		RefLandPtr = RefWndPtr->GetWndLandPtr();
		if ( NULL != RefLandPtr )
		{	continue;	}
		WndPtr = RefWndPtr->CloneWndObj();
		if ( NULL == WndPtr ) { return false; }
		WndDefectID = WndPtr->GetWndDefectID();
		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();

		AlgType = AlgParam.GetAlgType();
		FrameUniqueID = AlgParam.GetAlgImageBinParam().GetBinaryFrameUniqueID();
		switch ( AlgType )
		{
		case ALG_BRIGHT_RATIO:
			switch ( WndDefectID )
			{
			case WND_DEFECT_BODY_MISSING:
			case WND_DEFECT_BODY_TILT:
				if ( FrameUniqueID_3D == FrameUniqueID )
				{	AlgParam.GetAlgParamBrightRatio().brTargetValue = BodyHeight;	}
				break;
			}			
			break;
		}		
		AlgParam.SetAlgPatternCount(0);
		AlgParam.ClearAlgPatternParamList();
		AlgParam.GetAlgParamAiModel().aiOcrCharList.clear();		

		WndPtr->ClearWndRoiWndList();
		WndPtr->ScaleWnd(BodyScale.x, BodyScale.y);
		WndPtr->SetWndClassID(m_ModelActClassID);
		WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);	
		WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
		WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
		AddModelWndPtr(WndPtr, false);
	}	
	
	int LandGroupID=0;		
	double LeadHeight=0.0;
	BOX_TOWARD LandToward;
	TSIZE2D LandSize, RefLandSize;
	CAOIBox *LandBoxPtr=NULL;
	CAOIBox *RefLandBoxPtr=NULL;
	TPOINT2D BoxPos, RefBoxPos, MovePos;	
	const size_t LandCount=GetModelLandCount();	
	const int MaxLandGroupID = GetModelLandFreeGroupID();
	for ( size_t k=0; k<MaxLandGroupID; k ++ )
	{
		LandGroupID = k;		
		for ( size_t i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandToward = LandPtr->GetLandToward();			
			LandBoxPtr = LandPtr->GetLandBoxPtr();
			LeadHeight = LandPtr->GetLandLeadTipHeight();
			if ( NULL == LandBoxPtr )
			{	LandSize.cx=LandSize.cy=0;	}
			else
			{	LandBoxPtr->GetBoxSize(LandSize); }
			if ( NULL==RefLandPtr || LandGroupID!=RefLandPtr->GetLandGroupID() || LandToward!=RefLandPtr->GetLandToward() )
			{	RefLandPtr = RefModelPtr->GetModelLandPtrByGroupID(LandGroupID, -1, LandToward, true);	}
			if ( NULL == RefLandPtr ) { continue; }
			RefLandBoxPtr = RefLandPtr->GetLandBoxPtr();
			if ( NULL == RefLandBoxPtr )
			{	RefLandSize.cx=RefLandSize.cy=0;	}
			else
			{	RefLandBoxPtr->GetBoxSize(RefLandSize); }

			const size_t RefLandWndCount=RefLandPtr->GetLandWndCount();
			for ( size_t j=0; j<RefLandWndCount; j++ )
			{
				RefWndPtr = RefLandPtr->GetLandWndPtr(j, false);
				if ( NULL == RefWndPtr ) { continue; }
				WndPtr = RefWndPtr->CloneWndObj();
				if ( NULL == WndPtr ) { return false; }	
				WndDefectID = WndPtr->GetWndDefectID();
				//先過濾特殊建法
				//if ( WND_DEFECT_SOLDER_BEAD == WndDefectID ) { continue; }
				if ( WND_DEFECT_SOLDER_BRIDGE == WndDefectID ) { continue; }

				LandPtr->GetLandBoxPtr()->GetBoxPos(BoxPos);
				RefLandPtr->GetLandBoxPtr()->GetBoxPos(RefBoxPos);
				MovePos.x = BoxPos.x-RefBoxPos.x;
				MovePos.y = BoxPos.y-RefBoxPos.y;
				WndPtr->MoveWnd(MovePos);

				CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
				AlgType = AlgParam.GetAlgType();
				FrameUniqueID = AlgParam.GetAlgImageBinParam().GetBinaryFrameUniqueID();
				switch ( AlgType )
				{
				case ALG_BRIGHT_RATIO:
					switch ( WndDefectID )
					{
					case WND_DEFECT_BODY_MISSING:
					case WND_DEFECT_BODY_TILT:
					case WND_DEFECT_LEAD_LIFTED:
						if ( FrameUniqueID_3D == FrameUniqueID )
						{	AlgParam.GetAlgParamBrightRatio().brTargetValue = LeadHeight;	}
						break;
					}			
					break;
				}		
				AlgParam.SetAlgPatternCount(0);
				AlgParam.ClearAlgPatternParamList();
				AlgParam.GetAlgParamAiModel().aiOcrCharList.clear();

				WndPtr->ClearWndRoiWndList();
				WndPtr->SetWndClassID(m_ModelActClassID);
				WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);	
				WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
				WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
				AddModelWndPtr(WndPtr, false);
				LandPtr->AddLandWndPtr(WndPtr);
			}
		}
	}		
	if ( RefModelPtr != Param.pModel )
	{	AOIObjManager.DestroyModelObj(RefModelPtr); }

	AssignModelFolder();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LeadLifted(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-腳翹檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_LeadLiftedKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_LeadLiftedKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LeadLiftedKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-腳翹檢測
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	double       LeadTipHeight = 0;	
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnLeadTip;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	const double GapU=0.75;
	const double GapV=0.75;		
	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_LEAD_LIFTED;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);	
	unsigned int FrameIndex = Param.nFrameIndex_3D;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_3D;
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);

	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);
		LeadTipHeight = CalcModelLandLeadTipAverageHeight(LandGroupID);	
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandPtr->GetLandType() == LAND_TYPE_NULL ) { continue; }
			if ( LandPtr->GetLandType() == LAND_TYPE_PAD ) { continue; }
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
			LandPtr->GetLandLeadTipBox().GetBoxRegion(RgnLeadTip);
			JetAPI::ScaleRegion(RgnLeadTip, GapU, GapV, SCALE_REGION_BY_CENTER, RgnWnd);
			
			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			WndPtr->SetWndFollowMode(WndFollowMode);
			WndPtr->SetWndRegion(RgnWnd);
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

			
			CAlgBinaryParam &BinParam = WndPtr->GetWndAlgParam().GetAlgImageBinParam();
			TALG_PARAM_BRIGHT_RATIO &brParam=WndPtr->GetWndAlgParam().GetAlgParamBrightRatio();	
			TALG_PARAM_GROUP_COMPARE &gcParam = WndPtr->GetWndAlgParam().GetAlgParamGroupCompare();
			brParam.brRatioUSL = 125;
			brParam.brRatioLSL =  50;
			brParam.brToleranceUSL =   75;
			brParam.brToleranceLSL = -100;
			if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
			{				
				BinParam.SetBinaryFrameIndex(FrameIndex);
				BinParam.SetBinaryFrameUniqueID(FrameUniqueID);
				BinParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);

				brParam.brTargetValue = LeadTipHeight;
				brParam.brAverageMode = ALG_BRIGHT_AVERAGE_PARTIAL;
				brParam.brAveragePartialH = 100;
				brParam.brAveragePartialL =  50;
				brParam.brRatioEnabled = false;//比例啟用				
				brParam.brToleranceEnabled = true;
				
				gcParam.gc3DHeightEnabled = true;
				gcParam.gc3DHeightBaseMode = ALG_3D_BASE_HEIGHT_AVE;
			}
			else
			{	
				brParam.brRatioEnabled = true;//比例啟用				
				brParam.brToleranceEnabled = false;
			}

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			CAOIModel::AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LeadBended(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-腳歪檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_LeadBendedKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_LeadBendedKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LeadBendedKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-腳歪檢測
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndBandID2 = 0;
	int          WndGouprID = 0;	
	int          LandGroupID = 0;
	double       LeadTipHeight = 0;	
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;	
	double       ScaleU=0.6; 
	double       ScaleV=1.2; 
	TSIZE2D      szLeadTip, szWnd;
	TPOINT2D     OffsetWnd;
	TREGION4D    RgnWnd, RgnLeadTip;	
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;		
	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_LEAD_BENDED;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);		
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;
	const double Gap = 20;//um	
	
	TBINARY_FILTER BinaryFilter1;
	const int FixThresholdL = 150;
	const double PassLineLUSL = 50;
	BinaryFilter1.FilterMode = NOISE_FILTER_OPEN;
	BinaryFilter1.FilterParam1 = 3;	
	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}
		
		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);
		WndBandID2 = WndBandID+1;		
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandPtr->GetLandType() == LAND_TYPE_NULL ) { continue; }
			if ( LandPtr->GetLandType() == LAND_TYPE_PAD ) { continue; }
			if ( LandPtr->GetLandType() == LAND_TYPE_ELECTRODE ) { continue; }
			if ( LandPtr->GetLandType() == LAND_TYPE_DIP_LEAD ) { continue; }

			LandToward = LandPtr->GetLandToward();			
			LandPtr->GetLandLeadTipBox().GetBoxRegion(RgnLeadTip);
			OffsetWnd.x = szLeadTip.cx = RgnLeadTip.GetWidth();
			OffsetWnd.y = szLeadTip.cy = RgnLeadTip.GetHeight();
			szWnd.cx = szLeadTip.cx*ScaleU;
			szWnd.cy = szLeadTip.cy*ScaleU;
			OffsetWnd.x = (szLeadTip.cx+szWnd.cx)*0.5+Gap;
			OffsetWnd.y = (szLeadTip.cy+szWnd.cy)*0.5+Gap;
			
			//第1個
			RgnWnd = RgnLeadTip;			
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:		
				RgnWnd.Move(-OffsetWnd.x, 0);
				JetAPI::ScaleRegion(RgnWnd, ScaleU, 1.0, SCALE_REGION_BY_CENTER, RgnWnd);
				JetAPI::ScaleRegion(RgnWnd, 1.0, ScaleV, SCALE_REGION_BY_SIDE_MIN_Y, RgnWnd);
				break;
			case BOX_TOWARD_LEFT:	
				RgnWnd.Move(0, -OffsetWnd.y);	
				JetAPI::ScaleRegion(RgnWnd, 1.0, ScaleU, SCALE_REGION_BY_CENTER, RgnWnd);
				JetAPI::ScaleRegion(RgnWnd, ScaleV, 1.0, SCALE_REGION_BY_SIDE_MAX_X, RgnWnd);
				break;
			case BOX_TOWARD_DOWN:	
				RgnWnd.Move(OffsetWnd.x, 0);	
				JetAPI::ScaleRegion(RgnWnd, ScaleU, 1.0, SCALE_REGION_BY_CENTER, RgnWnd);
				JetAPI::ScaleRegion(RgnWnd, 1.0, ScaleV, SCALE_REGION_BY_SIDE_MAX_Y, RgnWnd);
				break;
			case BOX_TOWARD_RIGHT:	
				RgnWnd.Move(0, OffsetWnd.y);	
				JetAPI::ScaleRegion(RgnWnd, 1.0, ScaleU, SCALE_REGION_BY_CENTER, RgnWnd);
				JetAPI::ScaleRegion(RgnWnd, ScaleV, 1.0, SCALE_REGION_BY_SIDE_MIN_X, RgnWnd);
				break;
			}

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			WndPtr->SetWndFollowMode(WndFollowMode);
			WndPtr->SetWndRegion(RgnWnd);
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();
		
			//短路參數			
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);			
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_RED);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryMode(BINARY_FIXED_THRESHOLD);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetFixedThresholdLow(FixThresholdL);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryNoiseFilter1(BinaryFilter1);
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brXLineEnabled = true;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brYLineEnabled = true;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brXLineUSL = PassLineLUSL;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brYLineUSL = PassLineLUSL;
				break;
			case BOX_TOWARD_LEFT:					
			case BOX_TOWARD_RIGHT:	
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brYLineEnabled = true;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brXLineEnabled = true;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brYLineUSL = PassLineLUSL;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brXLineUSL = PassLineLUSL;
				break;
			}
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioEnabled = false;
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brToleranceEnabled = false;

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);

			//第2個
			RgnWnd = RgnLeadTip;
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:		
				RgnWnd.Move(OffsetWnd.x, 0);
				JetAPI::ScaleRegion(RgnWnd, ScaleU, 1.0, SCALE_REGION_BY_CENTER, RgnWnd);
				JetAPI::ScaleRegion(RgnWnd, 1.0, ScaleV, SCALE_REGION_BY_SIDE_MIN_Y, RgnWnd);
				break;
			case BOX_TOWARD_LEFT:	
				RgnWnd.Move(0, OffsetWnd.y);	
				JetAPI::ScaleRegion(RgnWnd, 1.0, ScaleU, SCALE_REGION_BY_CENTER, RgnWnd);
				JetAPI::ScaleRegion(RgnWnd, ScaleV, 1.0, SCALE_REGION_BY_SIDE_MAX_X, RgnWnd);
				break;
			case BOX_TOWARD_DOWN:	
				RgnWnd.Move(-OffsetWnd.x, 0);	
				JetAPI::ScaleRegion(RgnWnd, ScaleU, 1.0, SCALE_REGION_BY_CENTER, RgnWnd);
				JetAPI::ScaleRegion(RgnWnd, 1.0, ScaleV, SCALE_REGION_BY_SIDE_MAX_Y, RgnWnd);
				break;
			case BOX_TOWARD_RIGHT:	
				RgnWnd.Move(0, -OffsetWnd.y);	
				JetAPI::ScaleRegion(RgnWnd, 1.0, ScaleU, SCALE_REGION_BY_CENTER, RgnWnd);
				JetAPI::ScaleRegion(RgnWnd, ScaleV, 1.0, SCALE_REGION_BY_SIDE_MIN_X, RgnWnd);
				break;
			}

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID2);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			WndPtr->SetWndFollowMode(WndFollowMode);
			WndPtr->SetWndRegion(RgnWnd);
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

			//短路參數			
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_RED);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryMode(BINARY_FIXED_THRESHOLD);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetFixedThresholdLow(FixThresholdL);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryNoiseFilter1(BinaryFilter1);
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
			case BOX_TOWARD_DOWN:
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brXLineEnabled = true;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brYLineEnabled = true;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brXLineUSL = PassLineLUSL;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brYLineUSL = PassLineLUSL;
				break;
			case BOX_TOWARD_LEFT:					
			case BOX_TOWARD_RIGHT:	
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brYLineEnabled = true;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brXLineEnabled = true;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brYLineUSL = PassLineLUSL;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brXLineUSL = PassLineLUSL;
				break;
			}
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioEnabled = false;
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brToleranceEnabled = false;

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);				
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderOpen(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-空焊檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_SolderOpenKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_SolderOpenKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderOpenKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-空焊檢測
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_SolderOpenKernel_v2(Param, RefLandGroupID); }	

	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnBody, RgnPadInner;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	double       GapU=50;
	double       GapV=50;
	double       SizeU=0;
	double       SizeV=0;	
	double       StartV=0;	

	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_OPEN;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);

	GetModelBodyRegion(RgnBody);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);		
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType   = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);			
			RgnPadInner = RgnPad;
			RgnPadInner.minX += 10;
			RgnPadInner.minY += 10;
			RgnPadInner.maxX -= 10;
			RgnPadInner.maxY -= 10;

			if ( LAND_TYPE_PAD == LandType )
			{				
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:					
					if ( RgnBody.maxY>RgnPad.minY && RgnBody.maxY<RgnPad.maxY )					
					{	RgnWnd.minY = RgnBody.maxY+(GapV); }
					else
					{	RgnWnd.minY = RgnPad.minY+(GapV); }
					RgnWnd.maxY = RgnPad.maxY-(GapV);					
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					if ( RgnWnd.maxY < RgnWnd.minY )
					{	RgnWnd.maxY = RgnWnd.minY+GapV; }
					break;
				case BOX_TOWARD_LEFT:
					if ( RgnBody.minX<RgnPad.maxX && RgnBody.minX>RgnPad.minX )
					{	RgnWnd.maxX = RgnBody.minX-(GapV); }
					else
					{	RgnWnd.maxX = RgnPad.maxX-(GapV); }
					RgnWnd.minX = RgnPad.minX+(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					if ( RgnWnd.minX > RgnWnd.maxX )
					{	RgnWnd.minX = RgnWnd.maxX-GapV; }
					break;
				case BOX_TOWARD_DOWN:					
					if ( RgnBody.minY<RgnPad.maxY && RgnBody.minY>RgnPad.minY )					
					{	RgnWnd.maxY = RgnBody.minY-(GapV); }
					else
					{	RgnWnd.maxY = RgnPad.maxY-(GapV); }					
					RgnWnd.minY = RgnPad.minY+(GapV);
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					if ( RgnWnd.minY > RgnWnd.maxY )
					{	RgnWnd.minY = RgnWnd.maxY-GapV; }
					break;
				case BOX_TOWARD_RIGHT:
					if ( RgnBody.maxX>RgnPad.minX && RgnBody.maxX<RgnPad.maxX )
					{	RgnWnd.minX = RgnBody.maxX+(GapV); }
					else
					{	RgnWnd.minX = RgnPad.minX+(GapV); }
					RgnWnd.maxX = RgnPad.maxX-(GapV);					
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					if ( RgnWnd.maxX < RgnWnd.minX )
					{	RgnWnd.maxX = RgnWnd.minX+GapV; }
					break;
				}				
				//JetAPI::ScaleRegion(RgnPad, 0.5, 0.5, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else if ( LAND_TYPE_ELECTRODE == LandType )
			{					
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					GapU = RgnLead.GetWidth()*0.3;
					GapV = 10;//RgnPad.GetHeight()*0.25;
					SizeV = RgnLead.GetHeight()*0.6;
					StartV = MAX(RgnPad.minY, RgnLead.maxY);
					StartV = MAX(StartV, RgnBody.maxY);

					RgnWnd.minY = StartV+(GapV);
					RgnWnd.maxY = RgnWnd.minY+(SizeV);
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					GapU = RgnLead.GetHeight()*0.3;
					GapV = 10;//RgnPad.GetWidth()*0.25;
					SizeV = RgnLead.GetWidth()*0.6;
					StartV = MIN(RgnPad.maxX, RgnLead.minX);
					StartV = MIN(StartV, RgnBody.minX);

					RgnWnd.maxX = StartV-(GapV);
					RgnWnd.minX = RgnWnd.maxX-(SizeV);
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					GapU = RgnLead.GetWidth()*0.3;
					GapV = 10;//RgnPad.GetHeight()*0.25;
					SizeV = RgnLead.GetHeight()*0.6;
					StartV = MIN(RgnPad.maxY, RgnLead.minY);
					StartV = MIN(StartV, RgnBody.minY);
					
					RgnWnd.maxY = StartV-(GapV);
					RgnWnd.minY = RgnWnd.maxY-(SizeV);
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					GapU = RgnLead.GetHeight()*0.3;
					GapV = 10;//RgnPad.GetWidth()*0.25;
					SizeV = RgnLead.GetWidth()*0.6;
					StartV = MAX(RgnPad.minX, RgnLead.maxX);
					StartV = MAX(StartV, RgnBody.maxX);

					RgnWnd.minX = StartV+(GapV);
					RgnWnd.maxX = RgnWnd.minX+(SizeV);
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
				//JetAPI::ScaleRegion(RgnPad, 0.4, 0.4, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else if ( LAND_TYPE_DIP_LEAD == LandType )
			{	
				JetAPI::ScaleRegion(RgnPad, 0.4, 0.4, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else//if ( LAND_TYPE_IC_LEAD/LAND_TYPE_CON_LEAD == LandType )			
			{
				GapU=00;
				GapV=20;	
				SizeV = 100;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					RgnWnd.minY = RgnLead.maxY+(GapV);					
					RgnWnd.maxY = RgnWnd.minY+(SizeV);
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					if ( RgnWnd.minX < RgnPadInner.minX ) { RgnWnd.minX = RgnPadInner.minX; }
					if ( RgnWnd.maxX > RgnPadInner.maxX ) { RgnWnd.maxX = RgnPadInner.maxX; }
					break;
				case BOX_TOWARD_LEFT:					
					RgnWnd.maxX = RgnLead.minX-(GapV);
					RgnWnd.minX = RgnWnd.maxX-(SizeV);
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					if ( RgnWnd.minY < RgnPadInner.minY ) { RgnWnd.minY = RgnPadInner.minY; }
					if ( RgnWnd.maxY > RgnPadInner.maxY ) { RgnWnd.maxY = RgnPadInner.maxY; }
					break;
				case BOX_TOWARD_DOWN:					
					RgnWnd.maxY = RgnLead.minY-(GapV);
					RgnWnd.minY = RgnWnd.maxY-(SizeV);
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					if ( RgnWnd.minX < RgnPadInner.minX ) { RgnWnd.minX = RgnPadInner.minX; }
					if ( RgnWnd.maxX > RgnPadInner.maxX ) { RgnWnd.maxX = RgnPadInner.maxX; }
					break;
				case BOX_TOWARD_RIGHT:					
					RgnWnd.minX = RgnLead.maxX+(GapV);
					RgnWnd.maxX = RgnWnd.minX+(SizeV);
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					if ( RgnWnd.minY < RgnPadInner.minY ) { RgnWnd.minY = RgnPadInner.minY; }
					if ( RgnWnd.maxY > RgnPadInner.maxY ) { RgnWnd.maxY = RgnPadInner.maxY; }
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			if ( LAND_TYPE_PAD == LandType )
			{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
			else if ( LAND_TYPE_DIP_LEAD==LandType )
			{				
				WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);
				WndPtr->SetWndShapeMode(BOX_SHAPE_ELLIPSE);
			}
			else
			{	WndPtr->SetWndFollowMode(WndFollowMode); }
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->SetWndConstrainMode(WND_CONSTRAIN_PAD_RGN_MOVE);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryMode(BINARY_COLOR_FILTER);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_VOID_BEGIN);			
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioUSL = 30;
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioLSL = 00;
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioEnabled = true;//比例啟用
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brToleranceEnabled = false;

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			CAOIModel::AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderPoor(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-少焊檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_SolderPoorKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_SolderPoorKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderPoorKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-少焊檢測
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_SolderPoorKernel_v2(Param, RefLandGroupID); }		

	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnBody, RgnPadInner;
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	double GapU=50;
	double GapV=50;
	double SizeV = 200;
	double StartV = 0.0;
	double RangeU = 0.0;
	double RangeV = 0.0;
	double WndSizeW=0;
	double WndSizeH=0;
	const double MinSizeW=50;
	const double MinSizeH=50;
	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_POOR;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;
	const bool bColorFilterMode = false;	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);

	if ( false == bColorFilterMode )
	{
		FrameIndex = Param.nFrameIndex_Align;
		FrameUniqueID = Param.nFrameUniqueID_Align;
	}
	GetModelBodyRegion(RgnBody);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);		
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType   = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
			RgnPadInner = RgnPad;
			RgnPadInner.minX += 10;
			RgnPadInner.minY += 10;
			RgnPadInner.maxX -= 10;
			RgnPadInner.maxY -= 10;

			if ( LAND_TYPE_PAD == LandType )
			{
				GapU=50;
				GapV=50;
				SizeV = 200;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					if ( RgnBody.maxY>RgnPad.minY && RgnBody.maxY<RgnPad.maxY )					
					{	RgnWnd.minY = RgnBody.maxY+(GapV); }
					else
					{	RgnWnd.minY = RgnPad.minY+(GapV); }										
					RgnWnd.maxY = RgnPad.maxY-GapV;
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					if ( RgnBody.minX<RgnPad.maxX && RgnBody.minX>RgnPad.minX )
					{	RgnWnd.maxX = RgnBody.minX-(GapV); }
					else
					{	RgnWnd.maxX = RgnPad.maxX-(GapV); }					
					RgnWnd.minX = RgnPad.minX+(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					if ( RgnBody.minY<RgnPad.maxY && RgnBody.minY>RgnPad.minY )					
					{	RgnWnd.maxY = RgnBody.minY-(GapV); }
					else
					{	RgnWnd.maxY = RgnPad.maxY-(GapV); }							
					RgnWnd.minY = RgnPad.minY-(GapV);
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					if ( RgnBody.maxX>RgnPad.minX && RgnBody.maxX<RgnPad.maxX )
					{	RgnWnd.minX = RgnBody.maxX+(GapV); }
					else
					{	RgnWnd.minX = RgnPad.minX+(GapV); }					
					RgnWnd.maxX = RgnPad.maxX-(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}
			else if ( LAND_TYPE_ELECTRODE == LandType )
			{
				GapU=50;
				GapV=50;
				SizeV = 50;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					StartV = MAX(RgnPad.minY, RgnLead.maxY);
					StartV = MAX(StartV, RgnBody.maxY);

					RangeV = RgnPad.maxY-GapV-StartV;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.minY = StartV+(GapV);
					RgnWnd.maxY = RgnPad.maxY-SizeV;
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					StartV = MIN(RgnPad.maxX, RgnLead.minX);
					StartV = MIN(StartV, RgnBody.minX);

					RangeV = StartV-GapV-RgnPad.minX;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.maxX = StartV-(GapV);
					RgnWnd.minX = RgnPad.minX+SizeV;
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					StartV = MIN(RgnPad.maxY, RgnLead.minY);
					StartV = MIN(StartV, RgnBody.minY);

					RangeV = StartV-GapV-RgnPad.minY;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.maxY = StartV-(GapV);
					RgnWnd.minY = RgnPad.minY+SizeV;
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					StartV = MAX(RgnPad.minX, RgnLead.maxX);
					StartV = MAX(StartV, RgnBody.maxX);

					RangeV = RgnPad.maxX-GapV-StartV;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.minX = StartV+(GapV);
					RgnWnd.maxX = RgnPad.maxX-SizeV;
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}
			else if ( LAND_TYPE_DIP_LEAD == LandType )
			{	
				JetAPI::ScaleRegion(RgnPad, 0.6, 0.6, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else //if ( LAND_TYPE_IC_LEAD/LAND_TYPE_CON_LEAD == LandType )
			{
				GapU=-50;
				GapV=20;
				SizeV = 150;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:				
					RgnWnd.minY = RgnLead.maxY+(GapV);
					RgnWnd.maxY = RgnWnd.minY+(SizeV);
					if ( RgnWnd.maxY > RgnPad.maxY ) 
					{	RgnWnd.maxY = RgnPad.maxY; }
				
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					if ( RgnWnd.minX < RgnPadInner.minX ) { RgnWnd.minX = RgnPadInner.minX; }
					if ( RgnWnd.maxX > RgnPadInner.maxX ) { RgnWnd.maxX = RgnPadInner.maxX; }
					break;
				case BOX_TOWARD_LEFT:
					RgnWnd.maxX = RgnLead.minX-(GapV);
					RgnWnd.minX = RgnWnd.maxX-(SizeV);
					if ( RgnWnd.minX < RgnPad.minX ) 
					{	RgnWnd.minX = RgnPad.minX; }
				
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					if ( RgnWnd.minY < RgnPadInner.minY ) { RgnWnd.minY = RgnPadInner.minY; }
					if ( RgnWnd.maxY > RgnPadInner.maxY ) { RgnWnd.maxY = RgnPadInner.maxY; }
					break;
				case BOX_TOWARD_DOWN:
					RgnWnd.maxY = RgnLead.minY-(GapV);
					RgnWnd.minY = RgnWnd.maxY-(SizeV);
					if ( RgnWnd.minY < RgnPad.minY ) 
					{	RgnWnd.minY = RgnPad.minY; }

					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					if ( RgnWnd.minX < RgnPadInner.minX ) { RgnWnd.minX = RgnPadInner.minX; }
					if ( RgnWnd.maxX > RgnPadInner.maxX ) { RgnWnd.maxX = RgnPadInner.maxX; }
					break;
				case BOX_TOWARD_RIGHT:
					RgnWnd.minX = RgnLead.maxX+(GapV);
					RgnWnd.maxX = RgnWnd.minX+(SizeV);
					if ( RgnWnd.maxX > RgnPad.maxX ) 
					{	RgnWnd.maxX = RgnPad.maxX; }
				
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					if ( RgnWnd.minY < RgnPadInner.minY ) { RgnWnd.minY = RgnPadInner.minY; }
					if ( RgnWnd.maxY > RgnPadInner.maxY ) { RgnWnd.maxY = RgnPadInner.maxY; }
					break;
				}				
			}

			WndSizeW = RgnWnd.GetWidth();
			WndSizeH = RgnWnd.GetHeight();
			WndSizeW = MAX(WndSizeW, MinSizeW);
			WndSizeH = MAX(WndSizeH, MinSizeH);
			RgnWnd.SetSize(WndSizeW, WndSizeH);
	
			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			if ( LAND_TYPE_PAD == LandType )
			{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
			else if ( LAND_TYPE_DIP_LEAD==LandType )
			{
				WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);
				WndPtr->SetWndShapeMode(BOX_SHAPE_ELLIPSE);
			}
			else
			{	WndPtr->SetWndFollowMode(WndFollowMode); }			
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);			
			WndPtr->SetWndConstrainMode(WND_CONSTRAIN_PAD_RGN_MOVE);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);			
			if ( true == bColorFilterMode )
			{
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryMode(BINARY_COLOR_FILTER);
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_SOLDER_BEGIN);			
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioUSL = 100;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioLSL =  50;
			}
			else
			{
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_RED);//IMAGE_SRC_LIGHTNESS
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryMode(BINARY_FIXED_THRESHOLD);
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetFixedThresholdHigh(255);
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetFixedThresholdLow(100);
				//WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_SOLDER_BEGIN);			
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioUSL = 30;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioLSL =  0;
			}
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioEnabled = true;//比例啟用
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brToleranceEnabled = false;

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			CAOIModel::AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderPadExposed(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-焊盤露出
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_SolderPadExposedKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_SolderPadExposedKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_SolderPadExposedKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-焊盤露出
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnBody, RgnPadInner;
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	double GapU=50;
	double GapV=50;
	double SizeV = 200;
	double StartV = 0.0;
	double RangeU = 0.0;
	double RangeV = 0.0;
	double WndSizeW=0;
	double WndSizeH=0;
	const double MinSizeW=50;
	const double MinSizeH=50;
	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_PAD_EXPOSED;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;
	const bool bColorFilterMode = false;	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);

	GetModelBodyRegion(RgnBody);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);		
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType   = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
			RgnPadInner = RgnPad;
			RgnPadInner.minX += 10;
			RgnPadInner.minY += 10;
			RgnPadInner.maxX -= 10;
			RgnPadInner.maxY -= 10;

			if ( LAND_TYPE_PAD == LandType )
			{
				GapU = 50;
				GapV = 50;
				SizeV = 200;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					if ( RgnBody.maxY>RgnPad.minY && RgnBody.maxY<RgnPad.maxY )					
					{	RgnWnd.minY = RgnBody.maxY+(GapV); }
					else
					{	RgnWnd.minY = RgnPad.minY+(GapV); }										
					RgnWnd.maxY = RgnPad.maxY-GapV;
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					if ( RgnBody.minX<RgnPad.maxX && RgnBody.minX>RgnPad.minX )
					{	RgnWnd.maxX = RgnBody.minX-(GapV); }
					else
					{	RgnWnd.maxX = RgnPad.maxX-(GapV); }					
					RgnWnd.minX = RgnPad.minX+(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					if ( RgnBody.minY<RgnPad.maxY && RgnBody.minY>RgnPad.minY )					
					{	RgnWnd.maxY = RgnBody.minY-(GapV); }
					else
					{	RgnWnd.maxY = RgnPad.maxY-(GapV); }							
					RgnWnd.minY = RgnPad.minY-(GapV);
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					if ( RgnBody.maxX>RgnPad.minX && RgnBody.maxX<RgnPad.maxX )
					{	RgnWnd.minX = RgnBody.maxX+(GapV); }
					else
					{	RgnWnd.minX = RgnPad.minX+(GapV); }					
					RgnWnd.maxX = RgnPad.maxX-(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}
			else if ( LAND_TYPE_ELECTRODE == LandType )
			{
				GapU = 50;
				GapV = 50;
				SizeV = 50;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					StartV = MAX(RgnPad.minY, RgnLead.maxY);
					StartV = MAX(StartV, RgnBody.maxY);

					RgnWnd.minY = StartV+(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapV;
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					StartV = MIN(RgnPad.maxX, RgnLead.minX);
					StartV = MIN(StartV, RgnBody.minX);

					RgnWnd.maxX = StartV-(GapV);
					RgnWnd.minX = RgnPad.minX+GapV;
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					StartV = MIN(RgnPad.maxY, RgnLead.minY);
					StartV = MIN(StartV, RgnBody.minY);

					RgnWnd.maxY = StartV-(GapV);
					RgnWnd.minY = RgnPad.minY+GapV;
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					StartV = MAX(RgnPad.minX, RgnLead.maxX);
					StartV = MAX(StartV, RgnBody.maxX);

					RgnWnd.minX = StartV+(GapV);
					RgnWnd.maxX = RgnPad.maxX-GapV;
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}
			else if ( LAND_TYPE_DIP_LEAD == LandType )
			{
				JetAPI::ScaleRegion(RgnPad, 0.8, 0.8, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else //if ( LAND_TYPE_IC_LEAD/LAND_TYPE_CON_LEAD == LandType )
			{
				GapU = 50;
				GapV = 50;
				SizeV = 150;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:				
					RgnWnd.minY = RgnLead.maxY+(GapV);
					RgnWnd.maxY = RgnPad.maxY-(GapV);

					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;					
					break;
				case BOX_TOWARD_LEFT:
					RgnWnd.maxX = RgnLead.minX-(GapV);
					RgnWnd.minX = RgnPad.minX+(GapV);

					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;					
					break;
				case BOX_TOWARD_DOWN:
					RgnWnd.maxY = RgnLead.minY-(GapV);
					RgnWnd.minY = RgnPad.minY+(GapV);					

					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;					
					break;
				case BOX_TOWARD_RIGHT:
					RgnWnd.minX = RgnLead.maxX+(GapV);
					RgnWnd.maxX = RgnPad.maxX-(GapV);
				
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;					
					break;
				}				
			}

			WndSizeW = RgnWnd.GetWidth();
			WndSizeH = RgnWnd.GetHeight();
			WndSizeW = MAX(WndSizeW, MinSizeW);
			WndSizeH = MAX(WndSizeH, MinSizeH);
			RgnWnd.SetSize(WndSizeW, WndSizeH);
	
			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			if ( LAND_TYPE_PAD == LandType )
			{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
			else if ( LAND_TYPE_DIP_LEAD == LandType )
			{	
				WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	
				WndPtr->SetWndShapeMode(BOX_SHAPE_ELLIPSE);	
			}
			else
			{	WndPtr->SetWndFollowMode(WndFollowMode); }			
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);			
			WndPtr->SetWndConstrainMode(WND_CONSTRAIN_PAD_RGN_MOVE);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);			
			if ( true == bColorFilterMode )
			{
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryMode(BINARY_COLOR_FILTER);
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_VOID_BEGIN);			
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioUSL = 35;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioLSL =  0;
			}
			else
			{
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_RED);
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryMode(BINARY_FIXED_THRESHOLD);
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetFixedThresholdHigh(255);
				WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetFixedThresholdLow(100);
				//WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryColorGroupLinkIndex(PROJECT_COLOR_ID_SOLDER_BEGIN);			
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioUSL = 35;
				WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioLSL =  0;
			}
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brRatioEnabled = true;//比例啟用
			WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brToleranceEnabled = false;

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			CAOIModel::AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PadScratch(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-刮傷檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_PadScratchKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_PadScratchKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_PadScratchKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-少焊檢測
{
	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnBody;
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	double GapU=50;
	double GapV=50;
	double SizeV = 200;
	double StartV = 0.0;
	double RangeU = 0.0;
	double RangeV = 0.0;

	ALG_TYPE     AlgType = ALG_BLOB_COUNT;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_PAD_SCRATCH;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;
	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);

	GetModelBodyRegion(RgnBody);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);		
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType   = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);

			if ( LAND_TYPE_PAD == LandType )
			{
				GapU=50;
				GapV=50;
				SizeV = 200;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					if ( RgnBody.maxY>RgnPad.minY && RgnBody.maxY<RgnPad.maxY )					
					{	RgnWnd.minY = RgnBody.maxY+(GapV); }
					else
					{	RgnWnd.minY = RgnPad.minY+(GapV); }										
					RgnWnd.maxY = RgnPad.maxY-GapV;
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					if ( RgnBody.minX<RgnPad.maxX && RgnBody.minX>RgnPad.minX )
					{	RgnWnd.maxX = RgnBody.minX-(GapV); }
					else
					{	RgnWnd.maxX = RgnPad.maxX-(GapV); }					
					RgnWnd.minX = RgnPad.minX+(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					if ( RgnBody.minY<RgnPad.maxY && RgnBody.minY>RgnPad.minY )					
					{	RgnWnd.maxY = RgnBody.minY-(GapV); }
					else
					{	RgnWnd.maxY = RgnPad.maxY-(GapV); }							
					RgnWnd.minY = RgnPad.minY-(GapV);
					RgnWnd.maxX = RgnPad.maxX-GapU;
					RgnWnd.minX = RgnPad.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					if ( RgnBody.maxX>RgnPad.minX && RgnBody.maxX<RgnPad.maxX )
					{	RgnWnd.minX = RgnBody.maxX+(GapV); }
					else
					{	RgnWnd.minX = RgnPad.minX+(GapV); }					
					RgnWnd.maxX = RgnPad.maxX-(GapV);
					RgnWnd.maxY = RgnPad.maxY-GapU;
					RgnWnd.minY = RgnPad.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}
			else if ( LAND_TYPE_ELECTRODE == LandType )
			{
				GapU=50;
				GapV=50;
				SizeV = 50;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					StartV = MAX(RgnPad.minY, RgnLead.maxY);
					StartV = MAX(StartV, RgnBody.maxY);

					RangeV = RgnPad.maxY-GapV-StartV;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.minY = StartV+(GapV);
					RgnWnd.maxY = RgnPad.maxY-SizeV;
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					StartV = MIN(RgnPad.maxX, RgnLead.minX);
					StartV = MIN(StartV, RgnBody.minX);

					RangeV = StartV-GapV-RgnPad.minX;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.maxX = StartV-(GapV);
					RgnWnd.minX = RgnPad.minX+SizeV;
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					StartV = MIN(RgnPad.maxY, RgnLead.minY);
					StartV = MIN(StartV, RgnBody.minY);

					RangeV = StartV-GapV-RgnPad.minY;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.maxY = StartV-(GapV);
					RgnWnd.minY = RgnPad.minY+SizeV;
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					StartV = MAX(RgnPad.minX, RgnLead.maxX);
					StartV = MAX(StartV, RgnBody.maxX);

					RangeV = RgnPad.maxX-GapV-StartV;
					if ( RangeV > 0 )
					{	SizeV = RangeV*0.5; }
					RgnWnd.minX = StartV+(GapV);
					RgnWnd.maxX = RgnPad.maxX-SizeV;
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				}
				JetAPI::BoundaryRegion(RgnPad, RgnWnd);
			}
			else if ( LAND_TYPE_DIP_LEAD == LandType )
			{
				JetAPI::ScaleRegion(RgnPad, 1.0, 1.0, SCALE_REGION_BY_CENTER, RgnWnd);
			}
			else //if ( LAND_TYPE_IC_LEAD/LAND_TYPE_CON_LEAD == LandType )
			{
				GapU=-50;
				GapV=20;
				SizeV = 150;
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:				
					RgnWnd.minY = RgnLead.maxY+(GapV);
					RgnWnd.maxY = RgnWnd.minY+(SizeV);
					if ( RgnWnd.maxY > RgnPad.maxY ) 
					{	RgnWnd.maxY = RgnPad.maxY; }
				
					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_LEFT:
					RgnWnd.maxX = RgnLead.minX-(GapV);
					RgnWnd.minX = RgnWnd.maxX-(SizeV);
					if ( RgnWnd.minX < RgnPad.minX ) 
					{	RgnWnd.minX = RgnPad.minX; }
				
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				case BOX_TOWARD_DOWN:
					RgnWnd.maxY = RgnLead.minY-(GapV);
					RgnWnd.minY = RgnWnd.maxY-(SizeV);
					if ( RgnWnd.minY < RgnPad.minY ) 
					{	RgnWnd.minY = RgnPad.minY; }

					RgnWnd.maxX = RgnLead.maxX-GapU;
					RgnWnd.minX = RgnLead.minX+GapU;
					break;
				case BOX_TOWARD_RIGHT:
					RgnWnd.minX = RgnLead.maxX+(GapV);
					RgnWnd.maxX = RgnWnd.minX+(SizeV);
					if ( RgnWnd.maxX > RgnPad.maxX ) 
					{	RgnWnd.maxX = RgnPad.maxX; }
				
					RgnWnd.maxY = RgnLead.maxY-GapU;
					RgnWnd.minY = RgnLead.minY+GapU;
					break;
				}				
			}

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(false);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			WndPtr->SetWndFollowMode(WndFollowMode);
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);
			WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);
			WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryMode(BINARY_COLOR_FILTER);			
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcXSizeMin = 50;//um
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcXSizeMinEnabled = true;
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcYSizeMin = 50;//um
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcYSizeMinEnabled = true;
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcLSizeMin = 50;//um
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcLSizeMinEnabled = true;
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcAreaSizeMin = 2000;//um*um
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcAreaSizeMinEnabled = true;			
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcCountUSL = 0;
			WndPtr->GetWndAlgParam().GetAlgParamBlobCount().bcCountLSL = 0;			

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
			CAOIModel::AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);

			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LandBridge(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-短路檢測
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_LandBridgeKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_LandBridgeKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LandBridgeKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-短路檢測
{
	if ( true == Param.bBridgeUse2D )
	{
		if ( AddModelDefaultWnd_LandBridgeKernelFn(Param, RefLandGroupID, false) == false )
		{	return false; }
	}
	if ( true == Param.bBridgeUse3D )
	{
		if ( AddModelDefaultWnd_LandBridgeKernelFn(Param, RefLandGroupID, true) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LandBridgeKernelFn(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID, bool bUse3D)//增加模組檢測框-短路檢測
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_LandBridgeKernelFn_v2(Param, RefLandGroupID, bUse3D);	}

	int          s=0, t=0;
	size_t       i=0, j=0;
	size_t       LandPtrCount=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          WndBandID1=0, WndBandID2=0;
	int          LandGroupID = 0;
	int          LandAlignID = 0;
	int          MaxLandAlignID = 0;
	bool         GetBridgeRgn = false;
	bool         GetBridgeRgnT=false, GetBridgeRgnL=false, GetBridgeRgnR=false, GetBridgeRgnB=false;
	double       dMinX=0, dMinY=0, dMaxX=0, dMaxY=0;	
	LAND_TYPE    LandType=LAND_TYPE_NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOIWnd     *WndPtr2 = NULL;
	CAOILand    *LandPtr = NULL;
	CAOILand    *LandPtrNext = NULL;
	CAOILand    *LandPtrNext2 = NULL;
	CAOILand    *LandPtrFirst = NULL;
	CAOILand    *LandPtrLast = NULL;	
	TPOINT2D     PtPad, PtPadNext;
	TREGION4D    RgnBody;
	TREGION4D    RgnLead, RgnLeadNext;
	TREGION4D    RgnWnd1, RgnWnd2;
	TREGION4D    RgnWnd, RgnPad, RgnPadNext;
	TREGION4D    RgnWndT, RgnWndL, RgnWndR, RgnWndB;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;
	BOX_TOWARD   ModelBodyToward = GetModelBodyBox().GetBoxToward();
	std::vector<CAOILand*> LandPtrList;
	CAlgParam    *AlgPtr = NULL;	
	const double GapU= 0;
	const double GapV=50;
	const double dMoveRatio_3D=0.4;
	const double dScaleRatio_3D = 0.7;
	const MODEL_TYPE ModelType=Param.eModelType;

	size_t                SortCount=0;
	CSortObj              SortObj;
	std::vector<CSortObj> SortListTmp;
	std::vector<CSortObj> SortListToUp;
	std::vector<CSortObj> SortListToRight;
	std::vector<CSortObj> SortListToLeft;
	std::vector<CSortObj> SortListToDown;
	bool                 bSortForward=true;
	const bool           bBridgeTwoSide=Param.bBridgeTwoSide;//雙邊增加檢測框
	ALG_TYPE     AlgType = ALG_BRIGHT_RATIO;
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();		
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_BRIDGE;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	MODEL_LAND_DIRECTION ModelLandDir = GetModelLandDirection();
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;
	int          FixedThreshold = 128;
	TBINARY_FILTER BinaryFilter;
	BinaryFilter.FilterMode = NOISE_FILTER_OPEN;
	if ( LandCount < 1 ) { return true; }	
	
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);

	if ( true == bUse3D )
	{
		FrameIndex = Param.nFrameIndex_3D;
		FrameUniqueID = Param.nFrameUniqueID_3D;		
	}
	GetModelBodyRegion(RgnBody);

	for ( s=0; s<MaxLandGroupID; s ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != s ) { continue; }
		}
		LandGroupID = s;
		MaxLandAlignID = GetModelLandFreeAlignID(LandGroupID);
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);
		WndBandID1 = WndBandID+1;
		WndBandID2 = WndBandID+2;		
		for ( t=0; t<MaxLandAlignID; t++ )
		{
			LandAlignID = t;
			//要先排序個方向的腳數			
			LandPtrList.clear();	
			SortListToUp.clear();
			SortListToRight.clear();
			SortListToLeft.clear();
			SortListToDown.clear();
			SortObj.SetSortMode(SORT_BY_INT);
			for ( i=0; i<LandCount; i++ )
			{
				LandPtr = GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
				if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }

				LandToward  = LandPtr->GetLandToward();				
				LandPtr->GetLandBoxPtr()->GetBoxRegion(RgnPad);

				SortObj.SetID(i);
				SortObj.SetPtr(LandPtr);				
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:					
					SortObj.SetValueInt((int)(RgnPad.minX));
					SortListToUp.push_back(SortObj);
					break;
				case BOX_TOWARD_DOWN:					
					SortObj.SetValueInt((int)(RgnPad.minX));
					SortListToDown.push_back(SortObj);
					break;
				case BOX_TOWARD_LEFT:						
					SortObj.SetValueInt((int)(RgnPad.minY));
					SortListToLeft.push_back(SortObj);
					break;
				case BOX_TOWARD_RIGHT:					
					SortObj.SetValueInt((int)(RgnPad.minY));
					SortListToRight.push_back(SortObj);
					break;				
				}				
			}			
			//1. 先加入和模組同向的
			switch ( ModelBodyToward )
			{
			case BOX_TOWARD_UP:
				bSortForward = false;
				SortListTmp = SortListToUp;	
				break;
			case BOX_TOWARD_DOWN:
				bSortForward = true;
				SortListTmp = SortListToDown; 
				break;
			case BOX_TOWARD_LEFT: 
				bSortForward = false;
				SortListTmp = SortListToLeft; 
				break;
			case BOX_TOWARD_RIGHT: 
				bSortForward = true;
				SortListTmp = SortListToRight;
				break;
			default:	SortListTmp.clear(); break;
			}			
			SortCount = SortListTmp.size();
			if ( SortCount > 1 )
			{	
				if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
				{
					if ( true == bSortForward ) { bSortForward = false; }
					else { bSortForward = true; }
				}
				std::sort(SortListTmp.begin(), SortListTmp.end());
				SortCount = SortListTmp.size();
				for ( i=0; i<SortCount; i++ )
				{
					if ( true==bSortForward )
					{	SortObj = SortListTmp[i]; }
					else
					{	SortObj = SortListTmp[SortCount-i-1]; }
					LandPtr = (CAOILand*)(SortObj.GetPtr());
					LandPtrList.push_back(LandPtr);
				}				
			}

			//2. 先加入和模組同向的
			if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
			{	LandToward = JetAPI::RotateToward(-90, ModelBodyToward);	}
			else
			{	LandToward = JetAPI::RotateToward(90, ModelBodyToward);	}
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				bSortForward = false;
				SortListTmp = SortListToUp;
				break;
			case BOX_TOWARD_RIGHT: 
				bSortForward = true;
				SortListTmp = SortListToRight;
				break;
			case BOX_TOWARD_DOWN: 
				bSortForward = true;
				SortListTmp = SortListToDown; 
				break;
			case BOX_TOWARD_LEFT: 
				bSortForward = false;
				SortListTmp = SortListToLeft; 
				break;				
			default:	SortListTmp.clear(); break;
			}
			SortCount = SortListTmp.size();
			if ( SortCount > 1 )
			{	
				if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
				{
					if ( true == bSortForward ) { bSortForward = false; }
					else { bSortForward = true; }
				}
				std::sort(SortListTmp.begin(), SortListTmp.end());
				SortCount = SortListTmp.size();
				for ( i=0; i<SortCount; i++ )
				{
					if ( true==bSortForward )
					{	SortObj = SortListTmp[i]; }
					else
					{	SortObj = SortListTmp[SortCount-i-1]; }
					LandPtr = (CAOILand*)(SortObj.GetPtr());
					LandPtrList.push_back(LandPtr);
				}				
			}

			//3. 先加入和模組同向的
			LandToward = JetAPI::RotateToward(180, ModelBodyToward);
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				bSortForward = false;
				SortListTmp = SortListToUp;
				break;
			case BOX_TOWARD_RIGHT: 
				bSortForward = true;
				SortListTmp = SortListToRight;
				break;
			case BOX_TOWARD_DOWN: 
				bSortForward = true;
				SortListTmp = SortListToDown; 
				break;
			case BOX_TOWARD_LEFT: 
				bSortForward = false;
				SortListTmp = SortListToLeft; 
				break;				
			default:	SortListTmp.clear(); break;
			}		
			SortCount = SortListTmp.size();
			if ( SortCount > 1 )
			{
				if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
				{
					if ( true == bSortForward ) { bSortForward = false; }
					else { bSortForward = true; }
				}
				std::sort(SortListTmp.begin(), SortListTmp.end());
				SortCount = SortListTmp.size();
				for ( i=0; i<SortCount; i++ )
				{
					if ( true==bSortForward )
					{	SortObj = SortListTmp[i]; }
					else
					{	SortObj = SortListTmp[SortCount-i-1]; }
					LandPtr = (CAOILand*)(SortObj.GetPtr());
					LandPtrList.push_back(LandPtr);
				}				
			}
			//4. 先加入和模組同向的			
			if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
			{	LandToward = JetAPI::RotateToward(-270, ModelBodyToward);	}
			else
			{	LandToward = JetAPI::RotateToward(270, ModelBodyToward);	}
			switch ( LandToward )
			{
			case BOX_TOWARD_UP:
				bSortForward = false;
				SortListTmp = SortListToUp;
				break;
			case BOX_TOWARD_RIGHT: 
				bSortForward = true;
				SortListTmp = SortListToRight;
				break;
			case BOX_TOWARD_DOWN: 
				bSortForward = true;
				SortListTmp = SortListToDown; 
				break;
			case BOX_TOWARD_LEFT: 
				bSortForward = false;
				SortListTmp = SortListToLeft; 
				break;				
			default:	SortListTmp.clear(); break;
			}					
			SortCount = SortListTmp.size();
			if ( SortCount > 1 )
			{
				if ( MODEL_LAND_CLOCKWISE == ModelLandDir )
				{
					if ( true == bSortForward ) { bSortForward = false; }
					else { bSortForward = true; }
				}
				std::sort(SortListTmp.begin(), SortListTmp.end());
				SortCount = SortListTmp.size();
				for ( i=0; i<SortCount; i++ )
				{
					if ( true==bSortForward )
					{	SortObj = SortListTmp[i]; }
					else
					{	SortObj = SortListTmp[SortCount-i-1]; }
					LandPtr = (CAOILand*)(SortObj.GetPtr());
					LandPtrList.push_back(LandPtr);
				}				
			}
			LandPtrCount = LandPtrList.size();
			if ( LandPtrCount < 2) { continue; }
			
			LandPtrFirst = NULL;
			LandPtrLast  = NULL;			
			GetBridgeRgnT = GetBridgeRgnL = GetBridgeRgnR = GetBridgeRgnB = false;
			for ( i=0; i<LandPtrCount-1; i++ )
			{
				LandPtr = LandPtrList[i];
				LandPtrNext = LandPtrList[i+1];
				if ( NULL == LandPtr ) { continue; }
				if ( NULL == LandPtrNext ) { continue; }
				if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
				if ( LandPtrNext->GetLandGroupID() != LandGroupID ) { continue; }				

				LandType    = LandPtr->GetLandType();
				LandToward  = LandPtr->GetLandToward();
				LandAlignID = LandPtr->GetLandAlignID();
				if ( LAND_TYPE_DIP_LEAD == LandType ) { continue; }
				if ( LandPtrNext->GetLandToward() != LandToward ) { continue;	}
				if ( LandPtrNext->GetLandAlignID() != LandAlignID ) { continue; }

				LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
				LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);			
				LandPtrNext->GetLandPadBox().GetBoxRegion(RgnPadNext);
				LandPtrNext->GetLandLeadBox().GetBoxRegion(RgnLeadNext);

				PtPad.x = (RgnPad.minX+RgnPad.maxX)*0.5;
				PtPad.y = (RgnPad.minY+RgnPad.maxY)*0.5;
				PtPadNext.x = (RgnPadNext.minX+RgnPadNext.maxX)*0.5;
				PtPadNext.y = (RgnPadNext.minY+RgnPadNext.maxY)*0.5;
				GetBridgeRgn = false;				
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
					if ( RgnPad.maxY < RgnPadNext.maxY )
					{	RgnWnd.maxY = RgnPad.maxY-(GapV);	}
					else
					{	RgnWnd.maxY = RgnPadNext.maxY-(GapV);	}

					if ( RgnPad.minY > RgnPadNext.minY )
					{	RgnWnd.minY = RgnPad.minY+(GapV);	}
					else
					{	RgnWnd.minY = RgnPadNext.minY+(GapV);	}
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	dMaxY = MAX(RgnLead.maxY, RgnLeadNext.maxY);	}
					else if ( LAND_TYPE_IC_LEAD == LandType )
					{	dMaxY = MAX(RgnLead.minY, RgnLeadNext.minY);	}	
					else if ( LAND_TYPE_CON_LEAD == LandType )
					{	dMaxY = MAX(RgnLead.minY, RgnLeadNext.minY);	}						
					else { dMaxY = RgnWnd.minY; }
					if ( CheckModelTypeUseBody(ModelType) == true )
					{	dMaxY = MAX(dMaxY, RgnBody.maxY); }
					dMinY = MAX(RgnWnd.minY, dMaxY);
					if ( dMinY < RgnWnd.maxY )
					{	RgnWnd.minY = dMinY;	}

					if ( RgnPad.minX > RgnPadNext.maxX )
					{
						RgnWnd.maxX = RgnPad.minX-GapU;
						RgnWnd.minX = RgnPadNext.maxX+GapU;
						GetBridgeRgn = true;
					}
					else if (RgnPadNext.minX > RgnPad.maxX )
					{
						RgnWnd.maxX = RgnPadNext.minX-GapU;
						RgnWnd.minX = RgnPad.maxX+GapU;
						GetBridgeRgn = true;
					}
					if ( true == GetBridgeRgn )
					{
						if ( true == bUse3D )
						{
							double Size=RgnWnd.GetSizeY();
							double dMove=Size*dMoveRatio_3D;
							double dSize=Size*dScaleRatio_3D;
							RgnWnd.Move(0, dMove);
							RgnWnd.maxY=RgnWnd.minY+dSize;
						}
						GetBridgeRgnT = true;
						RgnWndT = RgnWnd; 
					}	
					break;
				case BOX_TOWARD_LEFT:
					if ( RgnPad.minX > RgnPadNext.minX )
					{	RgnWnd.minX = RgnPad.minX+(GapV);	}
					else
					{	RgnWnd.minX = RgnPadNext.minX+(GapV);	}

					if ( RgnPad.maxX < RgnPadNext.maxX )
					{	RgnWnd.maxX = RgnPad.maxX-(GapV);	}
					else
					{	RgnWnd.maxX = RgnPadNext.maxX-(GapV);	}
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	dMinX = MIN(RgnLead.minX, RgnLeadNext.minX);	}
					else if ( LAND_TYPE_IC_LEAD == LandType )
					{	dMinX = MIN(RgnLead.maxX, RgnLeadNext.maxX);	}
					else if ( LAND_TYPE_CON_LEAD == LandType )
					{	dMinX = MIN(RgnLead.maxX, RgnLeadNext.maxX);	}					
					else { dMinX = RgnWnd.maxX; }
					if ( CheckModelTypeUseBody(ModelType) == true )
					{	dMinX = MIN(dMinX, RgnBody.minX);	}
					dMaxX = MIN(RgnWnd.maxX, dMinX);
					if ( dMaxX > RgnWnd.minX ) 
					{	RgnWnd.maxX = dMaxX; }

					if ( RgnPad.minY > RgnPadNext.maxY )
					{
						RgnWnd.maxY = RgnPad.minY-GapU;
						RgnWnd.minY = RgnPadNext.maxY+GapU;
						GetBridgeRgn = true;
					}
					else if (RgnPadNext.minY > RgnPad.maxY )
					{
						RgnWnd.maxY = RgnPadNext.minY-GapU;
						RgnWnd.minY = RgnPad.maxY+GapU;
						GetBridgeRgn = true;
					}
					if ( true == GetBridgeRgn )
					{
						if ( true == bUse3D )
						{						
							double Size=RgnWnd.GetSizeX();
							double dMove=Size*dMoveRatio_3D;
							double dSize=Size*dScaleRatio_3D;
							RgnWnd.Move(-dMove, 0);
							RgnWnd.minX=RgnWnd.maxX-dSize;
						}
						GetBridgeRgnL = true;
						RgnWndL = RgnWnd; 
					}	
					break;
				case BOX_TOWARD_DOWN:
					if ( RgnPad.minY > RgnPadNext.minY )
					{	RgnWnd.minY = RgnPad.minY+(GapV);	}
					else
					{	RgnWnd.minY = RgnPadNext.minY+(GapV);	}

					if ( RgnPad.maxY < RgnPadNext.maxY )
					{	RgnWnd.maxY = RgnPad.maxY-(GapV);	}
					else
					{	RgnWnd.maxY = RgnPadNext.maxY-(GapV);	}				
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	dMinY = MIN(RgnLead.minY, RgnLeadNext.minY);	}
					else if ( LAND_TYPE_IC_LEAD == LandType )
					{	dMinY = MIN(RgnLead.maxY, RgnLeadNext.maxY);	}
					else if ( LAND_TYPE_CON_LEAD == LandType )
					{	dMinY = MIN(RgnLead.maxY, RgnLeadNext.maxY);	}
					else { dMinY = RgnWnd.maxY; }
					if ( CheckModelTypeUseBody(ModelType) == true )
					{	dMinY = MIN(dMinY, RgnBody.minY);	}
					dMaxY = MIN(RgnWnd.maxY, dMinY);
					if ( dMaxY > RgnWnd.minY ) 
					{	RgnWnd.maxY = dMaxY; }

					if ( RgnPad.minX > RgnPadNext.maxX )
					{
						RgnWnd.maxX = RgnPad.minX-GapU;
						RgnWnd.minX = RgnPadNext.maxX+GapU;
						GetBridgeRgn = true;
					}
					else if (RgnPadNext.minX > RgnPad.maxX )
					{
						RgnWnd.maxX = RgnPadNext.minX-GapU;
						RgnWnd.minX = RgnPad.maxX+GapU;
						GetBridgeRgn = true;
					}
					if ( true == GetBridgeRgn )
					{
						if ( true == bUse3D )
						{
							double Size=RgnWnd.GetSizeY();
							double dMove=Size*dMoveRatio_3D;
							double dSize=Size*dScaleRatio_3D;
							RgnWnd.Move(0, -dMove);
							RgnWnd.minY=RgnWnd.maxY-dSize;
						}
						GetBridgeRgnB = true;
						RgnWndB = RgnWnd; 
					}	
					break;
				case BOX_TOWARD_RIGHT:
					if ( RgnPad.maxX < RgnPadNext.maxX )
					{	RgnWnd.maxX = RgnPad.maxX-(GapV);	}
					else
					{	RgnWnd.maxX = RgnPadNext.maxX-(GapV);	}

					if ( RgnPad.minX > RgnPadNext.minX )
					{	RgnWnd.minX = RgnPad.minX+(GapV);	}
					else
					{	RgnWnd.minX = RgnPadNext.minX+(GapV);	}
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	dMaxX = MAX(RgnLead.maxX, RgnLeadNext.maxX);	}
					else if ( LAND_TYPE_IC_LEAD == LandType )
					{	dMaxX = MAX(RgnLead.minX, RgnLeadNext.minX);	}
					else if ( LAND_TYPE_CON_LEAD == LandType )
					{	dMaxX = MAX(RgnLead.minX, RgnLeadNext.minX);	}
					else {	dMaxX = RgnWnd.minX; }
					if ( CheckModelTypeUseBody(ModelType) == true )
					{	dMaxX = MAX(dMaxX, RgnBody.maxX);	}
					dMinX = MAX(RgnWnd.minX, dMaxX);
					if ( dMinX < RgnWnd.maxX )
					{	RgnWnd.minX = dMinX;	}

					if ( RgnPad.minY > RgnPadNext.maxY )
					{
						RgnWnd.maxY = RgnPad.minY-GapU;
						RgnWnd.minY = RgnPadNext.maxY+GapU;
						GetBridgeRgn = true;
					}
					else if (RgnPadNext.minY > RgnPad.maxY )
					{
						RgnWnd.maxY = RgnPadNext.minY-GapU;
						RgnWnd.minY = RgnPad.maxY+GapU;
						GetBridgeRgn = true;
					}
					if ( true == GetBridgeRgn )
					{
						if ( true == bUse3D )
						{						
							double Size=RgnWnd.GetSizeX();
							double dMove=Size*dMoveRatio_3D;
							double dSize=Size*dScaleRatio_3D;
							RgnWnd.Move(dMove, 0);
							RgnWnd.maxX=RgnWnd.minX+dSize;
						}
						GetBridgeRgnR = true;
						RgnWndR = RgnWnd; 
					}	
					break;
				}
				if ( false == GetBridgeRgn ) { continue; }

				WndPtr = AOIObjManager.CreateWndObj();
				if ( NULL == WndPtr ) { return false; }
				WndPtr->SetWndToward(LandToward);
				WndPtr->SetWndBandID(WndBandID);
				WndPtr->SetWndGroupID(WndGouprID);
				WndPtr->SetWndRgnLinkAuto(false);
				WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);
				WndPtr->SetWndDefectID(WndDefectID);
				WndPtr->SetWndDefectGroupID(DefectGroupID);
				WndPtr->SetWndFollowMode(WndFollowMode);
				WndPtr->SetWndRegion(RgnWnd);	
				WndPtr->SetWndExtendBoxUsed(UseExtendBox);
				WndPtr->UpdateWndExtendBox();	
				WndPtr->SetWndAlgType(AlgType);
				WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

				AlgPtr = WndPtr->GetWndAlgParamPtr();	
				AlgPtr->GetAlgImageBinParam().SetBinaryFrameIndex(FrameIndex);
				AlgPtr->GetAlgImageBinParam().SetBinaryFrameUniqueID(FrameUniqueID);				
				AlgPtr->GetAlgImageBinParam().SetBinaryMode(BINARY_FIXED_THRESHOLD);				
				AlgPtr->GetAlgImageBinParam().SetBinaryNoiseFilter1(BinaryFilter);
				if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
				{					
					AlgPtr->GetAlgImageBinParam().SetFixedThresholdLow(100);
					AlgPtr->GetAlgImageBinParam().SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_3D);					
					AlgPtr->GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
				}
				else
				{						
					AlgPtr->GetAlgImageBinParam().SetFixedThresholdLow(80);	 
					AlgPtr->GetAlgImageBinParam().SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_2D);
					AlgPtr->GetAlgImageBinParam().SetBinaryImageSourceMode(IMAGE_SRC_LIGHTNESS);
				}
				switch ( LandToward )
				{
				case BOX_TOWARD_UP:
				case BOX_TOWARD_DOWN:
					AlgPtr->GetAlgParamBrightRatio().brXLineEnabled = true;
					break;
				case BOX_TOWARD_LEFT:
				case BOX_TOWARD_RIGHT:
					AlgPtr->GetAlgParamBrightRatio().brYLineEnabled = true;
					break;
				}
				AlgPtr->GetAlgParamBrightRatio().brRatioEnabled = false;
				AlgPtr->GetAlgParamBrightRatio().brToleranceEnabled = false;

				if ( NULL != RefModelPtr )
				{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
				ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);
				if ( false == bBridgeTwoSide )
				{					
					AddModelWndPtr(WndPtr, false);
					LandPtr->AddLandWndPtr(WndPtr);
				}
				else
				{					
					//第1隻腳
					if ( NULL==LandPtrFirst )
					{
						WndPtr2 = WndPtr->CloneWndObj();
						if ( NULL == WndPtr2 ) { return false; }
						
						switch ( LandToward )
						{
						case BOX_TOWARD_UP:							
						case BOX_TOWARD_DOWN:							
							WndPtr2->MirrorWndYAxis(RgnPad.GetCpX());
							break;
						case BOX_TOWARD_LEFT:							
						case BOX_TOWARD_RIGHT:
							WndPtr2->MirrorWndXAxis(RgnPad.GetCpY());
							break;
						}						
						WndPtr2->SetWndBandID(WndBandID1);
						AddModelWndPtr(WndPtr2, false);
						LandPtr->AddLandWndPtr(WndPtr2);
						LandPtrFirst = LandPtr;
					}
					AddModelWndPtr(WndPtr, false);
					LandPtr->AddLandWndPtr(WndPtr);					

					//最末一隻					
					if ( i < LandPtrCount-2 )
					{
						LandPtrNext2 = LandPtrList[i+2];
						if ( NULL != LandPtrNext2 )
						{							
							if ( LandPtrNext2->GetLandGroupID() != LandGroupID ||
								 LandPtrNext2->GetLandToward() != LandToward ||
								 LandPtrNext2->GetLandAlignID() != LandAlignID)
							{	LandPtrNext2 = NULL;	}
						}						
					}
					else
					{	LandPtrNext2 = NULL; }

					if ( NULL == LandPtrNext2 )
					{
						WndPtr2 = WndPtr->CloneWndObj();
						if ( NULL == WndPtr2 ) { return false; }
						
						switch ( LandToward )
						{
						case BOX_TOWARD_UP:							
						case BOX_TOWARD_DOWN:							
							WndPtr2->MirrorWndYAxis(RgnPadNext.GetCpX());
							break;
						case BOX_TOWARD_LEFT:							
						case BOX_TOWARD_RIGHT:
							WndPtr2->MirrorWndXAxis(RgnPadNext.GetCpY());
							break;
						}
						WndPtr2->SetWndBandID(WndBandID2);
						AddModelWndPtr(WndPtr2, false);
						LandPtrNext->AddLandWndPtr(WndPtr2);						
						LandPtrFirst = NULL;
					}
				}
				WndPtr->SetWndClassID(m_ModelActClassID);
				WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);
				WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
				WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);	
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LandOuterShort(const TMODEL_DEFAULT_WND_PARAM &Param, int LandGroupID)//增加模組檢測框-焊盤外接短路檢測	
{
	bool IsOK = true;
	const double ComAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComAngle);
	if ( IsExceptionAngle == false )
	{	IsOK = AddModelDefaultWnd_LandOuterShortKernel(Param, LandGroupID);	}
	else
	{
		RotateModel(-ComAngle, 0, 0);
		IsOK = AddModelDefaultWnd_LandOuterShortKernel(Param, LandGroupID);
		RotateModel(ComAngle, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LandOuterShortKernel(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)//增加模組檢測框-焊盤外接短路檢測	
{
	if ( true == Param.bBridgeUse2D )
	{
		if ( AddModelDefaultWnd_LandOuterShortKernelFn(Param, RefLandGroupID, false) == false )
		{	return false; }
	}
	if ( true == Param.bBridgeUse3D )
	{
		if ( AddModelDefaultWnd_LandOuterShortKernelFn(Param, RefLandGroupID, true) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::AddModelDefaultWnd_LandOuterShortKernelFn(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID, bool bUse3D)//增加模組檢測框-焊盤外接短路檢測	
{
	if ( MDW_VERSION_2 == Param.eVersion )
	{	return AddModelDefaultWnd_LandOuterShortKernelFn_v2(Param, RefLandGroupID, bUse3D);	}

	int          k=0;
	size_t       i=0, j=0;
	int          WndBandID = 0;
	int          WndGouprID = 0;
	int          LandGroupID = 0;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	TREGION4D    RgnWnd, RgnLead, RgnPad, RgnBody, RgnPadInner;
	BOX_TOWARD   LandToward = BOX_TOWARD_NULL;	
	double       GapU=50;
	double       GapV=50;
	double       SizeU=0;
	double       SizeV=0;	
	double       StartV=0;	
	const double ExtX = Param.dBridgeExtendRange;
	const double ExtY = Param.dBridgeExtendRange;	

	ALG_TYPE     AlgType = ALG_OUTER_SHORT;
	LAND_TYPE    LandType=LAND_TYPE_NULL;	
	BOX_TOWARD   BodyToward = GetModelBodyBox().GetBoxToward();		
	const int    MaxLandGroupID = GetModelLandFreeGroupID();
	const size_t LandCount = GetModelLandCount();	
	const WND_DEFECT_ID WndDefectID=WND_DEFECT_SOLDER_BRIDGE;
	const int DefectGroupID = GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	unsigned int FrameIndex = Param.nFrameIndex_Solder;
	unsigned int FrameUniqueID = Param.nFrameUniqueID_Solder;	
	int          FixedThreshold = 128;
	CAOIWnd   *RefWndPtr = NULL;
	CAOIModel *RefModelPtr = (CAOIModel*)(Param.pModel);
	if ( true == bUse3D )
	{
		FrameIndex = Param.nFrameIndex_3D;
		FrameUniqueID = Param.nFrameUniqueID_3D;	
	}
	GetModelBodyRegion(RgnBody);
	for ( k=0; k<MaxLandGroupID; k ++ )
	{
		if ( RefLandGroupID >= 0 ) 
		{
			if ( RefLandGroupID != k ) { continue; }
		}

		LandGroupID = k;		
		WndGouprID = GetModelWndFreeGroupID();
		WndBandID = GetModelWndFreeBandID(WndGouprID);		
		WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
		for ( i=0; i<LandCount; i++ )
		{
			LandPtr = GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }			
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			LandType   = LandPtr->GetLandType();
			LandToward = LandPtr->GetLandToward();
			LandPtr->GetLandPadBox().GetBoxRegion(RgnPad);
			LandPtr->GetLandLeadBox().GetBoxRegion(RgnLead);
			RgnPadInner = RgnPad;
			RgnPadInner.minX += 10;
			RgnPadInner.minY += 10;
			RgnPadInner.maxX -= 10;
			RgnPadInner.maxY -= 10;

			//JetAPI::BoundaryRegion(RgnPad, RgnWnd);

			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr ) { return false; }
			WndPtr->SetWndToward(LandToward);
			WndPtr->SetWndBandID(WndBandID);
			WndPtr->SetWndGroupID(WndGouprID);
			WndPtr->SetWndRgnLinkAuto(true);
			WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_PAD);//WND_RGN_LINK_NONE, WND_RGN_LINK_PAD_RGN
			WndPtr->SetWndDefectID(WndDefectID);
			WndPtr->SetWndDefectGroupID(DefectGroupID);
			if ( LAND_TYPE_PAD == LandType )
			{	WndPtr->SetWndFollowMode(WND_FOLLOW_PAD);	}
			else
			{	WndPtr->SetWndFollowMode(WndFollowMode); }
			WndPtr->SetWndRegion(RgnWnd);	
			WndPtr->SetWndExtendRangeX(ExtX);
			WndPtr->SetWndExtendRangeY(ExtY);			
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->SetWndConstrainMode(WND_CONSTRAIN_DISABLE);
			WndPtr->UpdateWndExtendBox();	
			WndPtr->SetWndAlgType(AlgType);			

			CAlgParam                 &AlgParam = WndPtr->GetWndAlgParam();
			CAlgBinaryParam           &BinaryParam = AlgParam.GetAlgImageBinParam();			

			AlgParam.CheckAlgPatternFileUsed();			
			BinaryParam.SetBinaryFrameIndex(FrameIndex);
			BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);
			BinaryParam.SetBinaryMode(BINARY_FIXED_THRESHOLD);
			if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{
				BinaryParam.SetFixedThresholdLow(100);	
				BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_3D);
				BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
			}
			else
			{			
				FixedThreshold = 80;
				BinaryParam.SetFixedThresholdLow(FixedThreshold);	 
				BinaryParam.SetFixedThresholdHigh(FIXED_THRESHOLD_MAX_2D);
				BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_LIGHTNESS);
			}
			TBINARY_FILTER BinaryFilter;
			BinaryFilter.FilterMode = NOISE_FILTER_OPEN;
			BinaryFilter.FilterParam1 = 3;
			BinaryParam.SetBinaryNoiseFilter1(BinaryFilter);

			if ( NULL != RefModelPtr )
			{	RefWndPtr = RefModelPtr->GetModelWndPtrByLandDefectID(LandPtr, WndDefectID, DefectGroupID);	}
			ApplyModelDefaultLandWnd(WndPtr, RefWndPtr);			
			AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);
			WndPtr->SetWndClassID(m_ModelActClassID);
			WndPtr->SetWndAttachedAngle(m_ModelAttachedAngle);	
			WndPtr->SetWndAttachedPosCad(m_ModelAttachedCadPos);
			WndPtr->SetWndAttachedPosStage(m_ModelAttachedStagePos);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelEditMode() const//取得模組是否編輯模式
{
	return true;
	if ( CheckModelTypeEnabled() == false )	
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelUnsetState() const//取得模組是否為設定狀態
{
	if ( CheckModelTypeEnabled() == false )	
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UpdateModelUsing3D()
{	
	size_t       i=0;	
	bool         Using3D = true;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetModelWndCount();	

	Using3D = false;
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		if ( WndPtr->CheckWndAlgUsing3D() == false ) { continue; }		
		Using3D = true;
		break;		
	}	

	CAOIRgn *RgnPtr = GetModelAttachedPtr();	
	if ( NULL != RgnPtr  )
	{
		CAOIRgn *SubRgnPtr=NULL;
		const bool Bypass3D = RgnPtr->GetRgnBypass3D();
		const size_t SubRgnCount=RgnPtr->GetRgnSubRgnCount();		
		if ( true == Bypass3D )
		{	Using3D = false;	}

		RgnPtr->SetRgnUsing3D(Using3D);	
		for ( size_t i=0; i<SubRgnCount; i++ )
		{
			SubRgnPtr=RgnPtr->GetRgnSubRgnPtr(i, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnUsing3D(Using3D);	
		}
	}
	SetModelUsing3D(Using3D);
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::UpdateModelBypass3D()
{
	bool Bypass3D = false;
	CAOIRgn *RgnPtr = GetModelAttachedPtr();	
	if ( NULL != RgnPtr  )
	{	Bypass3D = RgnPtr->GetRgnBypass3D();	}
	SetModelBypass3D(Bypass3D);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelBodyLinkChipLead() const//確認模組本體與被動元件電極框連動
{	
	if ( CheckModelTypUseChipSizeMode(GetModelType()) == false )
	{	return false; }	
	return GetModelBodyLinkChipLead();
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelModified()//確認模組修改過參數	
{  
	if ( GetModelModifiedCount() ) { return true; }
	if ( CheckModelWndModified() ) { return true; }	
	if ( CheckModelLogicModified() ) { return true; }
	if ( CheckModelLandModified() ) { return true; }			
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetupkModelModifiedDateTime()
{
	::time(&m_ModelModifiedDateTime);	
#ifdef _DEBUG
	tm *pTime=localtime(&m_ModelModifiedDateTime);//Begin Year=1900
#endif _DEBUG
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::InitModelInspection(bool SelfTest)//初始化模組檢測
{
	size_t        i=0;
	int           WndOrder = 0;
	CAOIWnd      *WndPtr = NULL;
	CAOILand     *LandPtr = NULL;		
	WND_DEFECT_ID WndDefectID = WND_DEFECT_NONE;	
	const size_t  WndCount = GetModelWndCount();	
	const size_t  LandCount = GetModelLandCount();	
	
	SetModelSelfTest(SelfTest);
	m_ModelResultID = RESULT_ID_NONE;
	m_ModelResultID_Alarm = RESULT_ID_NONE;
	m_ModelInspectedTime = 0.0;	
	m_ModelBodyBox.ResetBoxRegionRes();	

	UpdateModelUsing3D();
	UpdateModelBypass3D();			
	ClearModelWndOrderList();
	ClearModelDefectWndUUIDList();		
	ClearModelDefectWndGroupIDList();	

	const double PanelBasePlane=GetModelPanelBasePlane();
	GetModelSpaceBasePlaneParam().PanelBasePlane = PanelBasePlane;

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->InitLandInspection();
	}

	if ( 0 == WndCount ) { return true; }
	CSortObj              SortObj;
	CSortObj             *SortObjPtr=NULL;
	std::vector<CSortObj> WndSortList(WndCount);
	
	WndSortList.clear();
	SortObj.SetSortMode(SORT_BY_INT);	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }

		WndDefectID = WndPtr->GetWndDefectID();
		WndOrder = AOIDataDefine.GetWndDefectIDOrder(WndDefectID);
		WndOrder = WndOrder+(int)(i);
		SortObj.SetValueInt(WndOrder);
		SortObj.SetPtr(WndPtr);
		WndSortList.push_back(SortObj);
		WndPtr->SetWndOrderIndex(-1);
		WndPtr->InitWndInspection(true);
		if ( WND_DEFECT_CLASS_CHECK == WndPtr->GetWndDefectID() )
		{	WndPtr->SetWndActived(false); }
	}

	std::sort(WndSortList.begin(), WndSortList.end());
	const size_t SortCount = WndSortList.size();
	for ( i=0; i<SortCount; i++ )	
	{
		SortObjPtr = &(WndSortList[i]);
		WndPtr = (CAOIWnd*)(SortObjPtr->GetPtr());
		WndPtr->SetWndOrderIndex(i);
		m_ModelWndOrderList.push_back(WndPtr);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspection(std::vector<TUNI_FRAME> &UniFrameList)
{
	bool IsOK = true;	
	LARGE_INTEGER       nStartTime;
	LARGE_INTEGER       nEndTime;	
	const size_t UniFrameCount = UniFrameList.size();
	const AOI_OBJ_TYPE  AttachedType = GetModelAttachedType();
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	ModifyModelTotalRegionSize(m_ModelImageSize_um.cx, m_ModelImageSize_um.cy);		
	QueryPerformanceCounter(&nStartTime);
	if ( IsExceptionAngle == false )
	{	
		IsOK = ExecModelInspectionKernel(UniFrameList);				
	}
	else
	{		
		double CPX = 0;
		double CPY = 0;
		TPOINT2D  ModelCornerPts[4];
		TREGION4D ModelRgn, ModelRgnRotated;
		std::vector<TUNI_FRAME> UniFrameListDst;
		GetModelTotalRegion(ModelRgn);
		const double RgnCpX = ModelRgn.GetCpX();
		const double RgnCpY = ModelRgn.GetCpY();		
		RotateModel(-AttachedAngle, CPX, CPY);		
		GetModelTotalRegion(ModelRgnRotated);
		const double RgnRotatedCpX = ModelRgnRotated.GetCpX();
		const double RgnRotatedCpY = ModelRgnRotated.GetCpY();
		//m_ModelTotalRgn.Move(-RgnRotatedCpX, -RgnRotatedCpY);//消除模組中心偏差值?
		IsOK = RotateModelUniFrameList(-AttachedAngle, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListDst);
		if ( false == IsOK )
		{
			SetModelResultID(RESULT_ID_EXCEPTION);
			SetModelResultID_Alarm(RESULT_ID_EXCEPTION);
			RotateModel(AttachedAngle, CPX, CPY);
			QueryPerformanceCounter(&nEndTime);
			m_ModelInspectedTime = (nEndTime.QuadPart-nStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			return false;
		}	
		IsOK = ExecModelInspectionKernel(UniFrameListDst);

		//m_ModelTotalRgn = ModelRgnRotated;
		RotateModel(AttachedAngle, CPX, CPY);		
		JetAPI::ClearUniFrameList(UniFrameListDst);
	}	
	if ( false == IsOK )
	{	
		SetModelResultID(RESULT_ID_EXCEPTION); 
		SetModelResultID_Alarm(RESULT_ID_EXCEPTION);
	}	
	QueryPerformanceCounter(&nEndTime);
	m_ModelInspectedTime = (nEndTime.QuadPart-nStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;

	if ( UniFrameCount > 0 )
	{
		TPOINT2D   RgnCp, ImageCp, Scale;
		const TUNI_FRAME &UniFrameRef=UniFrameList[0];
		IMAGE_SIZE ImageW=UniFrameRef.ImageW;
		IMAGE_SIZE ImageH=UniFrameRef.ImageH;		
		CalcModelInspectionParam(ImageW, ImageH, RgnCp, ImageCp, Scale);
		CalcModelInspectionImageRect_Raw(ImageW, ImageH, RgnCp, ImageCp, Scale);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspectionKernel(std::vector<TUNI_FRAME> &UniFrameList)//執行模組檢測
{		
	const size_t FrameCount = UniFrameList.size();	
	if ( FrameCount == 0 ) { return false; }	
	const size_t     WndCount = GetModelWndCount();
	if ( 0 == WndCount )
	{	//沒有檢測框視為不檢測
		SetModelResultID(RESULT_ID_BYPASS);
		SetModelResultID_Alarm(RESULT_ID_BYPASS);
		return true;
	}
	TPOINT2D        RgnCp, ImageCp, Scale;
	const TREGION4D ModelRgn = m_ModelTotalRgn;
	const IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	const IMAGE_SIZE ImageH = UniFrameList[0].ImageH;	
	CalcModelInspectionParam(ImageW, ImageH, RgnCp, ImageCp, Scale);		
	
	SetModelImageScale(Scale);	
	SetModelResultID(RESULT_ID_OK);
	SetModelResultID_Alarm(RESULT_ID_OK);

	bool  bIsOK = true;
	bool  bOpenMP = false; 
	const int OpenMapCount = GetModelOpenMPCount();
	if ( OpenMapCount > 1 ) 
	{	bOpenMP = true;	}
	else
	{	bOpenMP = false; }
	//bIsOK = ExecModelInspectionKernel_MP(UniFrameList, bOpenMP);	
	//bIsOK = ExecModelInspectionKernel_SP(UniFrameList);
	bool  bRotated = AOIDataCollect.GetSystemParameter().m_WndRotationFollowed;
	if (false == bRotated) {bIsOK = ExecModelInspectionKernel_MP(UniFrameList, bOpenMP); }
	else {	bIsOK = ExecModelInspectionKernel_MP_Rotated(UniFrameList, bOpenMP);}	
	if ( false == bIsOK )
	{	return false; }

	//AI Model Inspectoin
	const bool bAiSucc=ExecModelAIInspectionKernel(UniFrameList);

	//Group Compare
	if ( ExecModelWndGroupInspection(UniFrameList) == false )
	{	return false; }

	if ( BuildModelDefectWndGroupIDList() == false )
	{	return false; }
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspectionKernel_SP(std::vector<TUNI_FRAME> &UniFrameList)//執行模組檢測//不再使用
{
	size_t           i=0;	
	int              WndClassID=0;	
	int              ActClassID=0;	
	int              ClassCheckCnt=0;
	CAOIWnd         *WndPtr = NULL;	
	RESULT_ID        ResultID;
	WND_DEFECT_ID    WndDefectID;
	TPOINT2D         RgnCp, Scale, ImageCp;
	const TREGION4D  ModelRgn = m_ModelTotalRgn;
	const int        ModelClassID = GetModelClassID();
	const size_t     WndCount = GetModelWndOrderCount();	
	const IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	const IMAGE_SIZE ImageH = UniFrameList[0].ImageH;	
	CalcModelInspectionParam(ImageW, ImageH, RgnCp, ImageCp, Scale);

	//先計算類別確認框
	ClassCheckCnt=0;
	ActClassID = ModelClassID;//MODEL_CLASS_ID_NONE
	if ( MODEL_CLASS_ID_NONE == ActClassID )
	{
		for ( i=0; i<WndCount; i++ )
		{		
			WndPtr = GetModelWndOrderPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			if ( WndPtr->GetWndDefectID() != WND_DEFECT_CLASS_CHECK ) { continue; }
			if ( WndPtr->GetWndBypassed() == true )
			{
				ResultID = RESULT_ID_BYPASS;			
				WndPtr->SetWndResultID(ResultID);			
				WndPtr->SetWndLogicResultID(ResultID);
				WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);			
				continue; 
			}	
			if ( WndPtr->ExecWndInspection(this, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList) == false )
			{
				ResultID = RESULT_ID_EXCEPTION;
				SetModelResultID(ResultID);
				SetModelResultID_Alarm(ResultID);
				WndPtr->SetWndResultID(ResultID);
				WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);
				continue; 
			}
			if ( MODEL_CLASS_ID_NONE == ActClassID )
			{
				WndDefectID = WndPtr->GetWndDefectID();
				if ( WND_DEFECT_CLASS_CHECK == WndDefectID )
				{
					ClassCheckCnt ++;
					ResultID = WndPtr->GetWndResultID();
					WndClassID = WndPtr->GetWndClassID();
					if ( RESULT_ID_OK == ResultID )
					{	ActClassID = WndClassID; }				
				}
			}
		}
	}
	else
	{	ClassCheckCnt = 1;	}
	
	//若全部類別確認都失敗就要報錯
	if ( 0==ClassCheckCnt || MODEL_CLASS_ID_NONE==ActClassID) 
	{	ActClassID = 0; }
	else
	{
		//將其他設定成Bypass以避免影響後面判斷
		RESULT_ID    ResultID2=RESULT_ID_SKIP;	
		for ( i=0; i<WndCount; i++ )
		{		
			WndPtr = GetModelWndOrderPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			if ( WndPtr->GetWndBypassed() == true ) { continue; }
			if ( WND_DEFECT_CLASS_CHECK != WndPtr->GetWndDefectID() ) { continue; }
			ResultID = WndPtr->GetWndResultID();
			if ( RESULT_ID_OK == ResultID ) { continue; }		
			WndPtr->SetWndResultID(ResultID2);		
			WndPtr->SetWndLogicResultID(ResultID2);
			WndPtr->GetWndAlgParam().SetAlgResultID(ResultID2);
		}		
	}
	SetModelActClassID(ActClassID);

	//一般檢測
	for ( i=0; i<WndCount; i++ )
	{		
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		if ( WndPtr->GetWndDefectID() == WND_DEFECT_CLASS_CHECK ) { continue; }
		if ( WndPtr->GetWndBypassed() == true )
		{
			ResultID = RESULT_ID_BYPASS;			
			WndPtr->SetWndResultID(ResultID);			
			WndPtr->SetWndLogicResultID(ResultID);
			WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);			
			continue; 
		}
		if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false )
		{		
			ResultID = RESULT_ID_SKIP;			
			WndPtr->SetWndResultID(ResultID);			
			WndPtr->SetWndLogicResultID(ResultID);
			WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);			
			continue; 
		}
		if ( WndPtr->ExecWndInspection(this, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList) == false )
		{
			ResultID = RESULT_ID_EXCEPTION;
			SetModelResultID(ResultID);
			SetModelResultID_Alarm(ResultID);
			WndPtr->SetWndResultID(ResultID);
			WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);
			continue; 
		}
	}

	//補上XY的偏移角度檢測	
	for ( i=0; i<WndCount; i++ )
	{		
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		if ( AlgParam.GetAlgOffsetAEnabled() == false ) { continue; }
		
		RESULT_ID   ResultIDTmp;
		TSpecResult szW, szH;
		TSpecResult rH, rA, rV;
		TSpecResult sX, sY, sA, tA, oA;		
		CAOILand *LandPtr=WndPtr->GetWndLandPtr();
		CheckModelSpecResult(LandPtr, sX, sY, sA, tA, rH, rA, rV, szW, szH, oA, ResultIDTmp);			
		AlgParam.CheckAlgOffsetAngle(rH.sValue);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspectionKernel_MP(std::vector<TUNI_FRAME> &UniFrameList, bool bOpenMP)//執行模組檢測
{
	size_t           i=0;
	size_t           LastI=0;
	RESULT_ID        ResultID;
	WND_DEFECT_ID    WndDefectID;
	int              ActClassID=0;
	CAOIWnd         *WndPtr = NULL;	
	TPOINT2D         RgnCp, Scale, ImageCp;		
	TREGION4D        ModelRgn = m_ModelTotalRgn;
	const int        ModelClassID = GetModelClassID();
	const size_t     WndCount = GetModelWndOrderCount();	
	const IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	const IMAGE_SIZE ImageH = UniFrameList[0].ImageH;	
	CalcModelInspectionParam(ImageW, ImageH, RgnCp, ImageCp, Scale);	
	
	//分別先處理以下狀態-焊盤定位, 零件定位, 焊盤調整, 引腳調整
	size_t       PadAlignEnd=-1;
	size_t       PartAlignEnd=-1;
	size_t       PadAdjustEnd=-1;
	size_t       LeadAdjustEnd=-1;	
	size_t       BaseValueEnd=-1;
	size_t       ClassCheckEnd=-1;

	int          nOpenMPCount=0;	
	const size_t OpenMPUsedCnt=16;
	const int    OpenMPCount = GetModelOpenMPCount();
	for ( i=0; i<WndCount; i++ )
	{		
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		WndDefectID = WndPtr->GetWndDefectID();
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:	 PadAlignEnd = i;	break;
		case WND_DEFECT_PART_ALIGN:	 PartAlignEnd = i;	break;
		case WND_DEFECT_PAD_ADJUST:	 PadAdjustEnd = i;	break;
		case WND_DEFECT_LEAD_ADJUST: LeadAdjustEnd = i;	break;
		case WND_DEFECT_BASE_VALUE:  BaseValueEnd = i;	break;
		case WND_DEFECT_CLASS_CHECK:  ClassCheckEnd = i;	break;
			
		}   
	}
	PadAlignEnd += 1;
	PartAlignEnd += 1;
	PadAdjustEnd += 1;
	LeadAdjustEnd += 1;
	BaseValueEnd += 1;
	ClassCheckEnd += 1;

	LastI=0;	
	ActClassID = ModelClassID;//MODEL_CLASS_ID_NONE;
	//類別確認	
	if ( 0 != ClassCheckEnd )
	{
		if ( MODEL_CLASS_ID_NONE == ActClassID )
		{
			nOpenMPCount = 0;
			const size_t ClassCheckCnt=ClassCheckEnd-LastI;
			if ( false == bOpenMP ) 
			{	nOpenMPCount = 0; }
			if ( ExecModelInspectionKernel_Fn(ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, LastI, ClassCheckEnd, ActClassID, nOpenMPCount) == false )
			{	return false; }		
		}		
		if ( ExecModelInspectionKernelClassCheck_Fn(LastI, ClassCheckEnd, ActClassID) == false )
		{	return false; }
		LastI = ClassCheckEnd;		
	}
	if ( MODEL_CLASS_ID_NONE == ActClassID )
	{	ActClassID = 0; }
	SetModelActClassID(ActClassID);

	//焊盤定位	
	if ( 0 != PadAlignEnd )
	{	
		nOpenMPCount = 0;
		const size_t PadAlignCnt=PadAlignEnd-LastI;
		if ( false == bOpenMP ) 
		{	nOpenMPCount = 0; }	
		if ( ExecModelInspectionKernel_Fn(ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, LastI, PadAlignEnd, ActClassID, nOpenMPCount) == false )
		{	return false; }		
		if ( ExecModelInspectionKernelOffsetWnd_Fn(LastI, PadAlignEnd) == false )
		{	return false; }
		LastI = PadAlignEnd;
		//--------------------------------------------------------------------------------------//	
	}
	//零件定位
	if ( 0 != PartAlignEnd )
	{
		nOpenMPCount = 0;
		const size_t PartAlignCnt=PartAlignEnd-LastI;
		if ( false == bOpenMP ) 
		{	nOpenMPCount = 0; }	
		if ( ExecModelInspectionKernel_Fn(ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, LastI, PartAlignEnd, ActClassID, nOpenMPCount) == false )
		{	return false; }		
		if ( ExecModelInspectionKernelOffsetWnd_Fn(LastI, PartAlignEnd) == false )
		{	return false; }
		LastI = PartAlignEnd;
		//--------------------------------------------------------------------------------------//	
	}
	//焊盤調整
	if ( 0 != PadAdjustEnd )
	{	
		nOpenMPCount = 0;		
		const size_t PadAdjustCnt=PadAdjustEnd-LastI;
		const bool   bSupportMP = CheckModelWndOrderListSupportOpenMP(LastI, PadAdjustEnd);
		if ( true == bSupportMP )
		{
			if ( PadAdjustCnt > OpenMPUsedCnt ) { nOpenMPCount = OpenMPCount; }
		}
		if ( false == bOpenMP ) 
		{	nOpenMPCount = 0; }	
		if ( ExecModelInspectionKernel_Fn(ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, LastI, PadAdjustEnd, ActClassID, nOpenMPCount) == false )
		{	return false; }		
		if ( ExecModelInspectionKernelOffsetWnd_Fn(LastI, PadAdjustEnd) == false )
		{	return false; }
		LastI = PadAdjustEnd;
	}
	//引腳調整
	if ( 0 != LeadAdjustEnd )
	{
		nOpenMPCount = 0;
		const size_t LeadAdjustCnt=LeadAdjustEnd-LastI;
		const bool   bSupportMP = CheckModelWndOrderListSupportOpenMP(LastI, LeadAdjustEnd);
		if ( true == bSupportMP )
		{
			if ( LeadAdjustCnt > OpenMPUsedCnt ) { nOpenMPCount = OpenMPCount; }
		}
		if ( false == bOpenMP ) 
		{	nOpenMPCount = 0; }	
		if (ExecModelInspectionKernel_Fn(ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, LastI, LeadAdjustEnd, ActClassID, nOpenMPCount) == false )
		{	return false; }		
		if ( ExecModelInspectionKernelOffsetWnd_Fn(LastI, LeadAdjustEnd) == false )
		{	return false; }
		LastI = LeadAdjustEnd;		
	}
	//基準面
	if ( 0 != BaseValueEnd )
	{
		nOpenMPCount = 0;
		const size_t BasePlaneCnt=BaseValueEnd-LastI;
		const bool   bSupportMP = CheckModelWndOrderListSupportOpenMP(LastI, BaseValueEnd);
		if ( true == bSupportMP )
		{
			if ( BasePlaneCnt > OpenMPUsedCnt ) { nOpenMPCount = OpenMPCount; }
		}
		if ( false == bOpenMP ) 
		{	nOpenMPCount = 0; }	
		if (ExecModelInspectionKernel_Fn(ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, LastI, BaseValueEnd, ActClassID, nOpenMPCount) == false )
		{	return false; }		
		if ( ExecModelInspectionKernelBaseValue_Fn(LastI, BaseValueEnd) == false )
		{	return false; }
		LastI = BaseValueEnd;		
	}
	//剩下的檢測框
	nOpenMPCount = 0;
	const size_t RemainingCnt=WndCount-LastI;
	if ( RemainingCnt > OpenMPUsedCnt ) { nOpenMPCount = OpenMPCount; }		
	if ( false == bOpenMP ) 
	{	nOpenMPCount = 0; }
	if (ExecModelInspectionKernel_Fn(ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, LastI, WndCount, ActClassID, nOpenMPCount) == false )
	{	return false; }		
	LastI = WndCount;			

	//補上XY的偏移角度檢測	
	for ( i=0; i<WndCount; i++ )
	{		
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		if ( AlgParam.GetAlgOffsetAEnabled() == false ) { continue; }
		
		RESULT_ID   ResultIDTmp;
		TSpecResult szW, szH;
		TSpecResult rH, rA, rV;
		TSpecResult sX, sY, sA, tA, oA;		
		CAOILand *LandPtr=WndPtr->GetWndLandPtr();
		CheckModelSpecResult(LandPtr, sX, sY, sA, tA, rH, rA, rV, szW, szH, oA, ResultIDTmp);			
		AlgParam.CheckAlgOffsetAngle(rH.sValue);
	}

	for ( i=0; i<WndCount; i++ )
	{		
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		ResultID = WndPtr->GetWndResultID();
		if ( RESULT_ID_EXCEPTION != ResultID ) { continue; }
		SetModelResultID(ResultID);
		SetModelResultID_Alarm(ResultID);
		break;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspectionKernel_MP_Rotated(std::vector<TUNI_FRAME>& UniFrameList, bool bOpenMP)
{
	size_t           i = 0;
	size_t           LastI = 0;
	RESULT_ID        ResultID;
	WND_DEFECT_ID    WndDefectID;
	int              ActClassID = 0;
	CAOIWnd         *WndPtr = NULL;
	TPOINT2D         RgnCp, Scale, ImageCp;
	TREGION4D        ModelRgn = m_ModelTotalRgn;
	const int        ModelClassID = GetModelClassID();
	const size_t     WndCount = GetModelWndOrderCount();
	const IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	const IMAGE_SIZE ImageH = UniFrameList[0].ImageH;
	CalcModelInspectionParam(ImageW, ImageH, RgnCp, ImageCp, Scale);

	//分別先處理以下狀態-焊盤定位, 零件定位, 焊盤調整, 引腳調整
	size_t       PadAlignEnd = -1;
	size_t       PartAlignEnd = -1;
	size_t       PadAdjustEnd = -1;
	size_t       LeadAdjustEnd = -1;
	size_t       BaseValueEnd = -1;
	size_t       ClassCheckEnd = -1;

	int          nOpenMPCount = 0;
	const size_t OpenMPUsedCnt = 16;
	const int    OpenMPCount = GetModelOpenMPCount();
	for (i = 0; i<WndCount; i++)
	{
		WndPtr = GetModelWndOrderPtr(i, false);
		if (NULL == WndPtr) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		switch (WndDefectID)
		{
		case WND_DEFECT_PAD_ALIGN:	 PadAlignEnd = i;	break;
		case WND_DEFECT_PART_ALIGN:	 PartAlignEnd = i;	break;
		case WND_DEFECT_PAD_ADJUST:	 PadAdjustEnd = i;	break;
		case WND_DEFECT_LEAD_ADJUST: LeadAdjustEnd = i;	break;
		case WND_DEFECT_BASE_VALUE:  BaseValueEnd = i;	break;
		case WND_DEFECT_CLASS_CHECK:  ClassCheckEnd = i;	break;
		}
	}
	PadAlignEnd += 1;
	PartAlignEnd += 1;
	PadAdjustEnd += 1;
	LeadAdjustEnd += 1;
	BaseValueEnd += 1;
	ClassCheckEnd += 1;

	LastI = 0;
	ActClassID = ModelClassID;//MODEL_CLASS_ID_NONE;
							  //類別確認	
	if (0 != ClassCheckEnd)
	{
		if (MODEL_CLASS_ID_NONE == ActClassID)
		{
			nOpenMPCount = 0;
			const size_t ClassCheckCnt = ClassCheckEnd - LastI;
			if (false == bOpenMP)
			{
				nOpenMPCount = 0;
			}
			if (ExecModelInspectionKernel_Fn(ModelRgn, RgnCp, Scale, ImageCp, UniFrameList, LastI, ClassCheckEnd, ActClassID, nOpenMPCount) == false)
			{
				return false;
			}
		}
		if (ExecModelInspectionKernelClassCheck_Fn(LastI, ClassCheckEnd, ActClassID) == false)
		{
			return false;
		}
		LastI = ClassCheckEnd;
	}
	if (MODEL_CLASS_ID_NONE == ActClassID)
	{
		ActClassID = 0;
	}
	SetModelActClassID(ActClassID);

	//角度只會隨著焊盤與零件旋轉，焊盤與零件都會去調整ModelBodyBox的Skew，所以可以預先處理好執行的變數。
	//Skew = 0
	double AngleSkew, radius, ArcLength = 0;
	CAOIBox *ModelBoxPtr = NULL;
	std::map<WND_FOLLOW_MODE, WndInspectionParameter> WndParameterMap;
	WndInspectionParameter WndParameter;
	WndParameter.ModelCopyPtr = NULL;
	WndParameter.Setting(0, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList);
	WndParameterMap.insert(std::pair<WND_FOLLOW_MODE, WndInspectionParameter>(WND_FOLLOW_NONE, WndParameter));

	//焊盤定位	
	if ( 0 != PadAlignEnd )
	{
		nOpenMPCount = 0;
		const size_t PadAlignCnt = PadAlignEnd - LastI;
		if (false == bOpenMP)
		{
			nOpenMPCount = 0;
		}
		if (ExecModelInspectionKernel_Fn(WndParameterMap, LastI, PadAlignEnd, ActClassID, nOpenMPCount) == false)
		{ return false;	}

		if (ExecModelInspectionKernelOffsetWnd_Fn(LastI, PadAlignEnd) == false)
		{ return false;	}
		LastI = PadAlignEnd;
		//--------------------------------------------------------------------------------------//
		//Skew = PadSkew
		ModelBoxPtr = GetModelBodyBoxPtr();
		if (ModelBoxPtr == NULL) { return false; }

		AngleSkew = ModelBoxPtr->GetBoxAngleSkew();
		if (fabs(AngleSkew) > 0.001) {
			double BoxHeight, BoxWidth;
			ModelBoxPtr->GetBoxSize(BoxHeight, BoxWidth);
			radius = sqrt((BoxHeight*BoxHeight + BoxWidth * BoxWidth)) / 2;
			ArcLength = (AngleSkew*DEG_TO_RAD_DBL)*radius;
		}
		if (fabs(ArcLength) > 10) {
			TREGION4D ModelRgnRotated;
			std::vector<TUNI_FRAME> UniFrameListDst;
			TPOINT2D RgnCpDst, ScaleDst, ImageCpDst;
			WndInspectionParameter WndParameter;
			WndParameter.ModelCopyPtr = CloneModelObj();
			if (NULL == WndParameter.ModelCopyPtr) { return false; }
			WndParameter.ModelCopyPtr->InitModelWndOrderList_Clone();
			WndParameter.ModelCopyPtr->RotateModel(-AngleSkew, 0, 0);
			WndParameter.ModelCopyPtr->GetModelTotalRegion(ModelRgnRotated);
			bool IsOK = WndParameter.ModelCopyPtr->RotateModelUniFrameList(-AngleSkew, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListDst);
			if (IsOK == true) {
				const IMAGE_SIZE ImageWPad = UniFrameListDst[0].ImageW;
				const IMAGE_SIZE ImageHPad = UniFrameListDst[0].ImageH;
				WndParameter.ModelCopyPtr->CalcModelInspectionParam(ImageWPad, ImageHPad, RgnCpDst, ImageCpDst, ScaleDst);
				WndParameter.Setting(AngleSkew, ModelRgnRotated, RgnCpDst, ScaleDst, ImageCpDst, UniFrameListDst);
				WndParameterMap.insert(std::pair<WND_FOLLOW_MODE, WndInspectionParameter>(WND_FOLLOW_PAD, WndParameter));
			}
			else {
				AOIObjManager.DestroyModelObj(WndParameter.ModelCopyPtr);
			}
		}
	}
	//零件定位
	if (0 != PartAlignEnd)
	{
		nOpenMPCount = 0;
		const size_t PartAlignCnt = PartAlignEnd - LastI;
		if (false == bOpenMP)
		{
			nOpenMPCount = 0;
		}
		if (ExecModelInspectionKernel_Fn(WndParameterMap, LastI, PartAlignEnd, ActClassID, nOpenMPCount) == false)
		{
			return false;
		}
		if (ExecModelInspectionKernelOffsetWnd_Fn(LastI, PartAlignEnd) == false)
		{
			return false;
		}
		LastI = PartAlignEnd;
		//--------------------------------------------------------------------------------------//
		ModelBoxPtr = GetModelBodyBoxPtr();
		if (ModelBoxPtr == NULL) { return false; }
		AngleSkew = ModelBoxPtr->GetBoxAngleSkew();
		ArcLength = 0;
		if (fabs(AngleSkew) > 0.001) {
			double BoxHeight, BoxWidth;
			ModelBoxPtr->GetBoxSize(BoxHeight, BoxWidth);
			radius = sqrt((BoxHeight*BoxHeight + BoxWidth * BoxWidth)) / 2;
			ArcLength = (AngleSkew*DEG_TO_RAD_DBL)*radius;
		}
		if (fabs(ArcLength) > 10) {
			// 用弧長忽略不必要的旋轉。
			TREGION4D ModelRgnRotated;
			std::vector<TUNI_FRAME> UniFrameListDst;
			TPOINT2D RgnCpDst, ScaleDst, ImageCpDst;
			WndInspectionParameter WndParameter;
			WndParameter.ModelCopyPtr = CloneModelObj();
			if (NULL == WndParameter.ModelCopyPtr) { return false; }
			WndParameter.ModelCopyPtr->InitModelWndOrderList_Clone();
			WndParameter.ModelCopyPtr->RotateModel(-AngleSkew, 0, 0);
			WndParameter.ModelCopyPtr->GetModelTotalRegion(ModelRgnRotated);
			bool IsOK = WndParameter.ModelCopyPtr->RotateModelUniFrameList(-AngleSkew, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListDst);
			if (IsOK == true) {
				const IMAGE_SIZE ImageWPad = UniFrameListDst[0].ImageW;
				const IMAGE_SIZE ImageHPad = UniFrameListDst[0].ImageH;
				WndParameter.ModelCopyPtr->CalcModelInspectionParam(ImageWPad, ImageHPad, RgnCpDst, ImageCpDst, ScaleDst);
				WndParameter.Setting(AngleSkew, ModelRgnRotated, RgnCpDst, ScaleDst, ImageCpDst, UniFrameListDst);
				WndParameterMap.insert(std::pair<WND_FOLLOW_MODE, WndInspectionParameter>(WND_FOLLOW_PART, WndParameter));
			}
			else {
				AOIObjManager.DestroyModelObj(WndParameter.ModelCopyPtr);
			}
		}
	}

	//焊盤調整
	if (0 != PadAdjustEnd)
	{
		nOpenMPCount = 0;
		const size_t PadAdjustCnt = PadAdjustEnd - LastI;
		const bool   bSupportMP = CheckModelWndOrderListSupportOpenMP(LastI, PadAdjustEnd);
		if (true == bSupportMP)
		{
			if (PadAdjustCnt > OpenMPUsedCnt) { nOpenMPCount = OpenMPCount; }
		}
		if (false == bOpenMP)
		{
			nOpenMPCount = 0;
		}
		if (ExecModelInspectionKernel_Fn(WndParameterMap, LastI, PadAdjustEnd, ActClassID, nOpenMPCount) == false)
		{
			return false;
		}
		if (ExecModelInspectionKernelOffsetWnd_Fn(LastI, PadAdjustEnd) == false)
		{
			return false;
		}
		LastI = PadAdjustEnd;
	}
	//引腳調整
	if (0 != LeadAdjustEnd)
	{
		nOpenMPCount = 0;
		const size_t LeadAdjustCnt = LeadAdjustEnd - LastI;
		const bool   bSupportMP = CheckModelWndOrderListSupportOpenMP(LastI, LeadAdjustEnd);
		if (true == bSupportMP)
		{
			if (LeadAdjustCnt > OpenMPUsedCnt) { nOpenMPCount = OpenMPCount; }
		}
		if (false == bOpenMP)
		{
			nOpenMPCount = 0;
		}
		if (ExecModelInspectionKernel_Fn(WndParameterMap, LastI, LeadAdjustEnd, ActClassID, nOpenMPCount) == false)
		{ return false;	}
		if (ExecModelInspectionKernelOffsetWnd_Fn(LastI, LeadAdjustEnd) == false)
		{ return false;	}
		LastI = LeadAdjustEnd;
	}
	//基準面
	if ( 0 != BaseValueEnd )
	{
		nOpenMPCount = 0;
		const size_t BasePlaneCnt = BaseValueEnd - LastI;
		const bool   bSupportMP = CheckModelWndOrderListSupportOpenMP(LastI, BaseValueEnd);
		if ( true == bSupportMP )
		{
			if ( BasePlaneCnt > OpenMPUsedCnt) { nOpenMPCount = OpenMPCount; }
		}
		if (false == bOpenMP)
		{ nOpenMPCount = 0;	}
		if (ExecModelInspectionKernel_Fn(WndParameterMap, LastI, BaseValueEnd, ActClassID, nOpenMPCount) == false)
		{ return false;	}
		if (ExecModelInspectionKernelBaseValue_Fn(LastI, BaseValueEnd) == false)
		{ return false;	}
		LastI = BaseValueEnd;
	}
	//剩下的檢測框
	nOpenMPCount = 0;
	const size_t RemainingCnt = WndCount - LastI;
	if (RemainingCnt > OpenMPUsedCnt) { nOpenMPCount = OpenMPCount; }
	if (false == bOpenMP)
	{ nOpenMPCount = 0;	}
	if (ExecModelInspectionKernel_Fn(WndParameterMap, LastI, WndCount, ActClassID, nOpenMPCount) == false)
	{ return false; }
	LastI = WndCount;

	//補上XY的偏移角度檢測	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		if ( AlgParam.GetAlgOffsetAEnabled() == false ) { continue; }

		RESULT_ID   ResultIDTmp;
		TSpecResult szW, szH;
		TSpecResult rH, rA, rV;
		TSpecResult sX, sY, sA, tA, oA;
		CAOILand *LandPtr=WndPtr->GetWndLandPtr();
		CheckModelSpecResult(LandPtr, sX, sY, sA, tA, rH, rA, rV, szW, szH, oA, ResultIDTmp);
		AlgParam.CheckAlgOffsetAngle(rH.sValue);
	}

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		ResultID = WndPtr->GetWndResultID();
		if ( RESULT_ID_EXCEPTION != ResultID ) { continue; }
		SetModelResultID(ResultID);
		SetModelResultID_Alarm(ResultID);
		break;
	}

	for (auto &WndPara : WndParameterMap) {
		if (WndPara.first == WND_FOLLOW_NONE) { continue; }
		JetAPI::ClearUniFrameList(WndPara.second.UniFrameList);
		AOIObjManager.DestroyModelObj(WndPara.second.ModelCopyPtr);
	}
	WndParameterMap.clear();

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspectionKernel_Fn(TREGION4D &ModelRgn, TPOINT2D &RgnCp, TPOINT2D &Scale, TPOINT2D &ImageCp, std::vector<TUNI_FRAME> &UniFrameList, size_t Start, size_t End, int ActClassID, int nOpenMPCnt)//執行模組檢測	 
{	
	const size_t WndCount = GetModelWndOrderCount();
	if ( End > WndCount ) { return false; }
	if ( Start > WndCount ) { return false; }
	if ( nOpenMPCnt > 1 )
	{
		const int nEnd=(int)(End);
		const int nStart=(int)(Start);		
		const int OpenMPCount = nOpenMPCnt;		
		bool  CpuAffinity[MAX_OPEN_MP_COUNT]={false};
		DWORD_PTR AffinityMask = AOIDataCollect.GetThreadAffinityMask();
#pragma omp parallel for num_threads(OpenMPCount)
		for ( int ii=Start; ii<End; ii++ )
		{		
			int tid = omp_get_thread_num();
			if ( false == CpuAffinity[tid] )
			{
				CpuAffinity[tid] = true;
				SetThreadAffinityMask(GetCurrentThread(), AffinityMask);
			}
			CAOIWnd *WndPtr2 = GetModelWndOrderPtr(ii, true);
			if ( NULL == WndPtr2 ) { continue; }
			if ( WndPtr2->GetWndEnabled() == false ) { continue; }
			if ( WndPtr2->GetWndBypassed() == true )
			{
				RESULT_ID ResultID = RESULT_ID_BYPASS;			
				WndPtr2->SetWndResultID(ResultID);			
				WndPtr2->SetWndLogicResultID(ResultID);
				WndPtr2->GetWndAlgParam().SetAlgResultID(ResultID);			
				continue; 
			}
			if ( WndPtr2->CheckWndClassIDUsed(ActClassID) == false )	
			{
				RESULT_ID ResultID = RESULT_ID_SKIP;			
				WndPtr2->SetWndResultID(ResultID);			
				WndPtr2->SetWndLogicResultID(ResultID);
				WndPtr2->GetWndAlgParam().SetAlgResultID(ResultID);			
				continue; 
			}
			if ( WndPtr2->ExecWndInspection(this, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList) == false )
			{	
				RESULT_ID ResultID = RESULT_ID_EXCEPTION;	
				WndPtr2->SetWndResultID(RESULT_ID_EXCEPTION);
				WndPtr2->GetWndAlgParam().SetAlgResultID(RESULT_ID_EXCEPTION);
				continue; 
			}	
		}
	}
	else
	{
		size_t       i=0;	
		RESULT_ID    ResultID;
		CAOIWnd     *WndPtr = NULL;	
		for ( i=Start; i<End; i++ )
		{		
			WndPtr = GetModelWndOrderPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			if ( WndPtr->GetWndBypassed() == true )
			{
				ResultID = RESULT_ID_BYPASS;			
				WndPtr->SetWndResultID(ResultID);			
				WndPtr->SetWndLogicResultID(ResultID);
				WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);			
				continue; 
			}
			if ( WndPtr->CheckWndClassIDUsed(ActClassID) == false )	
			{
				RESULT_ID ResultID = RESULT_ID_SKIP;			
				WndPtr->SetWndResultID(ResultID);			
				WndPtr->SetWndLogicResultID(ResultID);
				WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);			
				continue; 
			}
			if ( WndPtr->ExecWndInspection(this, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList) == false )
			{	
				ResultID = RESULT_ID_EXCEPTION;	
				WndPtr->SetWndResultID(RESULT_ID_EXCEPTION);
				WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_EXCEPTION);
				continue; 
			}	
		}
	}
	return true;
}
bool CAOIModel::ExecModelInspectionKernel_Fn(std::map<WND_FOLLOW_MODE, WndInspectionParameter> WndParameterMap, size_t Start, size_t End, int ActClassID, int nOpenMPCnt)
{
	const size_t WndCount = GetModelWndOrderCount();
	CAOIComponent *ComponentPtr = GetModelComponentPtr();
	CString ComponentName;
	RESULT_ID    ResultID;
	CString str;

	if (NULL == ComponentPtr) { ComponentName = _T("Warning: lost Component Name"); }
	else { CString ComponentName = GetModelComponentPtr()->GetComponentName(); }

	if (End > WndCount) { return false; }
	if (Start > WndCount) { return false; }
	if (nOpenMPCnt > 1)
	{
		const int nEnd = (int)(End);
		const int nStart = (int)(Start);
		const int OpenMPCount = nOpenMPCnt;
		bool  CpuAffinity[MAX_OPEN_MP_COUNT] = { false };
		DWORD_PTR AffinityMask = AOIDataCollect.GetThreadAffinityMask();
#pragma omp parallel for num_threads(OpenMPCount)
		for (int ii = Start; ii<End; ii++)
		{
			CAOIWnd     *WndPtr = NULL,
				*WndPtr2 = NULL;
			int tid = omp_get_thread_num();
			if (false == CpuAffinity[tid])
			{
				CpuAffinity[tid] = true;
				SetThreadAffinityMask(GetCurrentThread(), AffinityMask);
			}
			WndPtr = GetModelWndOrderPtr(ii, true);
			if (NULL == WndPtr) { continue; }
			if (WndPtr->GetWndEnabled() == false) { continue; }
			if (WndPtr->GetWndBypassed() == true)
			{
				RESULT_ID ResultID = RESULT_ID_BYPASS;
				WndPtr2->SetWndResultID(ResultID);
				WndPtr2->SetWndLogicResultID(ResultID);
				WndPtr2->GetWndAlgParam().SetAlgResultID(ResultID);
				continue;
			}
			if (WndPtr->CheckWndClassIDUsed(ActClassID) == false)
			{
				RESULT_ID ResultID = RESULT_ID_SKIP;
				WndPtr2->SetWndResultID(ResultID);
				WndPtr2->SetWndLogicResultID(ResultID);
				WndPtr2->GetWndAlgParam().SetAlgResultID(ResultID);
				continue;
			}
			WND_FOLLOW_MODE WndFollowMode = WndPtr->GetWndFollowMode();
			while (WndParameterMap.find(WndFollowMode) == WndParameterMap.end()) {
				//找不到，向上找。
				switch (WndFollowMode)
				{
				case WND_FOLLOW_PAD:
					WndFollowMode = WND_FOLLOW_NONE;
					break;
				case WND_FOLLOW_PART:
				case WND_FOLLOW_PAD_BODY:
				case WND_FOLLOW_PAD_LEAD:
					WndFollowMode = WND_FOLLOW_PAD;
					break;
				case WND_FOLLOW_PART_BODY:
				case WND_FOLLOW_PART_LEAD:
					WndFollowMode = WND_FOLLOW_PART;
					break;
				}
			}
			double skew = WndParameterMap[WndFollowMode].skew;
			TREGION4D ModelRgn = WndParameterMap[WndFollowMode].ModelRgn;
			TPOINT2D RgnCp = WndParameterMap[WndFollowMode].RgnCp;
			TPOINT2D Scale = WndParameterMap[WndFollowMode].Scale;
			TPOINT2D ImageCp = WndParameterMap[WndFollowMode].ImageCp;
			std::vector<TUNI_FRAME> UniFrameList = WndParameterMap[WndFollowMode].UniFrameList;
			CAOIModel *ModelPtr = WndParameterMap[WndFollowMode].ModelCopyPtr;
			if (ModelPtr == NULL) {
				WndPtr2 = WndPtr;
				ModelPtr = this;
			}
			else {
				int WndIndex = WndPtr->GetWndIndex();
				WndPtr2 = ModelPtr->GetModelWndPtr(WndIndex, false);
			}
			if (WndPtr2->ExecWndInspection(ModelPtr, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList) == false)
			{
				str.Format(_T("    -CAOIModel::[%s] Wnd Inspection Failed "), ComponentName);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				ResultID = RESULT_ID_EXCEPTION;
				WndPtr->SetWndResultID(RESULT_ID_EXCEPTION);
				WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_EXCEPTION);
				continue;
			}
			if (WndPtr == WndPtr2) { continue; }

			WndPtr2->RotateWnd(skew, 0, 0);
			WndPtr->CloneWndResult(WndPtr2);

			CAOIBox * BoxPtr = NULL;
			CAOIBox * BoxPtr2 = NULL;
			BoxPtr = WndPtr->GetWndBoxPtr();
			BoxPtr2 = WndPtr2->GetWndBoxPtr();
			if (BoxPtr == NULL || BoxPtr2 == NULL) {
				str.Format(_T("    -CAOIModel::[%s] Clone result from Rotate box failed "), ComponentName);
				ResultID = RESULT_ID_EXCEPTION;
				WndPtr->SetWndResultID(RESULT_ID_EXCEPTION);
				WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_EXCEPTION);
				continue;
			}
			TREGION4D BoxRegion; TPOINT2D BoxPosRes2, BoxPosRes1;
			BoxPtr2->GetBoxRegionRes(BoxRegion);
			BoxPtr->SetBoxRegionRes(BoxRegion);
			BoxPtr2->GetBoxPosRes(BoxPosRes2);
			BoxPtr->GetBoxPosRes(BoxPosRes1);
			double MoveX = BoxPosRes2.x - BoxPosRes1.x;
			double MoveY = BoxPosRes2.y - BoxPosRes1.y;
			BoxPtr->MoveBoxRes(MoveX, MoveY);

			// Alan 0725 註解更新
			// Model 會因為零件與焊盤有偏移角度 Skew 而被我複製，所以最多會有3個 Model 同時在記憶體中。
			// 而檢測框要檢測前，會先判斷它的跟隨模式，去選取對應的 Model，檢測完後會根據演算法去使用
			// UpdateModelInspectionPosRes 改變其他零件的對應位置，編寫完這邊的程式碼後測試時，位置更
			// 新並不會傳遞到原 Model，我那時認為是因為傳遞給 ExecWndInspection 的 ModelPtr 是 Clone 
			// 後的，所以想藉由以下程式碼想要把資訊複製回來。
			CAlgParam &AlgParam = WndPtr2->GetWndAlgParam();
			double      CadOffsetX_Others = 0, CadOffsetY_Others = 0, CadSkew_Others = 0;
			double      CadOffsetX_Self = 0, CadOffsetY_Self = 0, CadSkew_Self = 0;
			const bool  bChkDefect = false;
			AlgParam.CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
			if (UpdateModelInspectionPosRes(WndPtr, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, true) == false)
			{
				str.Format(_T("    -CAOIModel::[%s] Update Model Inspection Pos by Rotate Box Failed"), ComponentName);
				ResultID = RESULT_ID_EXCEPTION;
				WndPtr->SetWndResultID(RESULT_ID_EXCEPTION);
				WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_EXCEPTION);
				continue;
			}
			// 但後來就算新增了以上程式碼依舊沒有更新，原因是 CloneModelObj() 不會把 m_ModelWndOrderList 
			// 複製，導致 Clone 的 Model 呼叫 UpdateModelInspectionPosRes 也不會有任何操作，為此我新增了
			// InitModelWndOrderList_Clone()，用來複製 m_ModelWndOrderList。
			// 所以以上程式碼有些冗餘，在原Model與Clone出的Model，位置應該都正常的更新了，
			// 理論上將 Wnd 自己的 Box 資訊從 Clone 那複製回來就好。
			const int ResultBoxCount = WndPtr2->GetWndResultBoxCount();
			for (int j = 0; j < ResultBoxCount; j++) {
				BoxPtr = WndPtr->GetWndResultBoxPtr(j, false);
				if (BoxPtr == NULL) { continue; }
				BoxPtr->SetBoxAngleSkew(skew);
			}
		}
	}
	else
	{
		CAOIWnd     *WndPtr = NULL,
			*WndPtr2 = NULL;
		size_t       i = 0;
		for (i = Start; i<End; i++)
		{
			WndPtr = GetModelWndOrderPtr(i, false);
			if (NULL == WndPtr) { continue; }
			if (WndPtr->GetWndEnabled() == false) { continue; }
			if (WndPtr->GetWndBypassed() == true)
			{
				ResultID = RESULT_ID_BYPASS;
				WndPtr->SetWndResultID(ResultID);
				WndPtr->SetWndLogicResultID(ResultID);
				WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);
				continue;
			}
			if (WndPtr->CheckWndClassIDUsed(ActClassID) == false)
			{
				RESULT_ID ResultID = RESULT_ID_SKIP;
				WndPtr->SetWndResultID(ResultID);
				WndPtr->SetWndLogicResultID(ResultID);
				WndPtr->GetWndAlgParam().SetAlgResultID(ResultID);
				continue;
			}

			WND_FOLLOW_MODE WndFollowMode = WndPtr->GetWndFollowMode();
			while (WndParameterMap.find(WndFollowMode) == WndParameterMap.end()) {
				//找不到，向上找。
				switch (WndFollowMode)
				{
				case WND_FOLLOW_NONE:
					return false;
				case WND_FOLLOW_PAD:
					WndFollowMode = WND_FOLLOW_NONE;
					break;
				case WND_FOLLOW_PART:
				case WND_FOLLOW_PAD_BODY:
				case WND_FOLLOW_PAD_LEAD:
					WndFollowMode = WND_FOLLOW_PAD;
					break;
				case WND_FOLLOW_PART_BODY:
				case WND_FOLLOW_PART_LEAD:
					WndFollowMode = WND_FOLLOW_PART;
					break;
				}
			}
			double skew = WndParameterMap[WndFollowMode].skew;
			TREGION4D ModelRgn = WndParameterMap[WndFollowMode].ModelRgn;
			TPOINT2D RgnCp = WndParameterMap[WndFollowMode].RgnCp;
			TPOINT2D Scale = WndParameterMap[WndFollowMode].Scale;
			TPOINT2D ImageCp = WndParameterMap[WndFollowMode].ImageCp;
			std::vector<TUNI_FRAME> UniFrameList = WndParameterMap[WndFollowMode].UniFrameList;
			CAOIModel *ModelPtr = WndParameterMap[WndFollowMode].ModelCopyPtr;
			if (ModelPtr == NULL) {
				WndPtr2 = WndPtr;
				ModelPtr = this;
			}
			else {
				int WndIndex = WndPtr->GetWndIndex();
				WndPtr2 = ModelPtr->GetModelWndPtr(WndIndex, false);
			}
			if (WndPtr2->ExecWndInspection(ModelPtr, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList) == false)
			{
				str.Format(_T("    -CAOIModel::[%s] Wnd Inspection Failed "), ComponentName);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				ResultID = RESULT_ID_EXCEPTION;
				WndPtr->SetWndResultID(RESULT_ID_EXCEPTION);
				WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_EXCEPTION);
				continue;
			}
			//如果WndPtr == WndPtr2 則已經修改到原Model
			if (WndPtr == WndPtr2) { continue; }

			//複製結果
			WndPtr2->RotateWnd(skew, 0, 0);
			WndPtr->CloneWndResult(WndPtr2);


			CAOIBox * BoxPtr = NULL;
			CAOIBox * BoxPtr2 = NULL;
			BoxPtr = WndPtr->GetWndBoxPtr();
			BoxPtr2 = WndPtr2->GetWndBoxPtr();
			if (BoxPtr == NULL || BoxPtr2 == NULL) {
				str.Format(_T("    -CAOIModel::[%s] Clone result from Rotate box failed "), ComponentName);
				ResultID = RESULT_ID_EXCEPTION;
				WndPtr->SetWndResultID(RESULT_ID_EXCEPTION);
				WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_EXCEPTION);
				continue;
			}

			TREGION4D BoxRegion; TPOINT2D BoxPosRes2, BoxPosRes1;
			BoxPtr2->GetBoxRegionRes(BoxRegion);
			BoxPtr->SetBoxRegionRes(BoxRegion);

			BoxPtr2->GetBoxPosRes(BoxPosRes2);
			BoxPtr->GetBoxPosRes(BoxPosRes1);
			double MoveX = BoxPosRes2.x - BoxPosRes1.x;
			double MoveY = BoxPosRes2.y - BoxPosRes1.y;
			BoxPtr->MoveBoxRes(MoveX, MoveY);

			CAlgParam &AlgParam = WndPtr2->GetWndAlgParam();
			double      CadOffsetX_Others = 0, CadOffsetY_Others = 0, CadSkew_Others = 0;
			double      CadOffsetX_Self = 0, CadOffsetY_Self = 0, CadSkew_Self = 0;
			const bool  bChkDefect = false;
			AlgParam.CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
			if (UpdateModelInspectionPosRes(WndPtr, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, true) == false)
			{
				str.Format(_T("    -CAOIModel::[%s] Update Model Inspection Pos by Rotate Box Failed"), ComponentName);
				ResultID = RESULT_ID_EXCEPTION;
				WndPtr->SetWndResultID(RESULT_ID_EXCEPTION);
				WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_EXCEPTION);
				return false;
			}
			const int ResultBoxCount = WndPtr2->GetWndResultBoxCount();
			for (int j = 0; j < ResultBoxCount; j++) {
				BoxPtr = WndPtr->GetWndResultBoxPtr(j, false);
				if (BoxPtr == NULL) { continue; }
				BoxPtr->SetBoxAngleSkew(skew);
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelWndOrderListSupportOpenMP(size_t Start, size_t End)//確認檢測框列表支援OpenMP計算
{
	size_t        i=0;		
	int           nTemp=0;	
	bool          bAlignWnd=false;
	int           nTempIndex=0;
	WND_DEFECT_ID WndDefectID;
	CAOIWnd      *WndPtr = NULL;	
	CAOILand     *LandPtr = NULL;	
	const size_t  LandCount = GetModelLandCount();

	SetModelTempInt1(0);
	SetModelTempInt2(0);
	SetModelTempInt3(0);
	SetModelTempInt4(0);
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->SetLandTempInt1(0);
		LandPtr->SetLandTempInt2(0);
		LandPtr->SetLandTempInt3(0);
		LandPtr->SetLandTempInt4(0);
	}
	for ( i=Start; i<End; i++ )
	{		
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		switch ( WndDefectID )
		{
		case WND_DEFECT_PAD_ALIGN:	nTempIndex=0;	break;
		case WND_DEFECT_PART_ALIGN:	nTempIndex=1;	break;
		case WND_DEFECT_PAD_ADJUST:	nTempIndex=2;	break;
		case WND_DEFECT_LEAD_ADJUST:	nTempIndex=3;	break;
		default:
			nTempIndex = -1;
			break;
		}		
		if ( -1 == nTempIndex ) { continue; }

		//OpenMP不可以同時跑多個座標補正
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			nTemp = GetModelTempInt(nTempIndex);			
			if ( nTemp > 0 )
			{	return false; }
			nTemp ++;
			SetModelTempInt(nTemp, nTempIndex);
		}
		else
		{	
			nTemp = LandPtr->GetLandTempInt(nTempIndex);
			if ( nTemp > 0 )
			{	return false; }
			nTemp ++;
			LandPtr->SetLandTempInt(nTemp, nTempIndex);
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspectionKernelOffsetWnd_Fn(size_t Start, size_t End)//執行模組檢測
{
	const size_t WndCount = GetModelWndOrderCount();
	if ( End > WndCount ) { return false; }
	if ( Start > WndCount ) { return false; }	

	size_t       i=0;	
	RESULT_ID    ResultID;
	CAOIWnd     *WndPtr = NULL;	
	CAOILand    *LandPtr = NULL;
	bool        bBodyOffset=false;
	const bool  bChkDefect= false;
	const bool  ApplySkewAngle=true;
	double      CadOffsetX_Self=0, CadOffsetY_Self=0, CadSkew_Self=0;
	double      CadOffsetX_Others=0, CadOffsetY_Others=0, CadSkew_Others=0;	
	WND_DEFECT_ID  WndDefectID;
	WND_LOGIC_TYPE WndLogicType;
	const int   FnEnable  = FN_ENABLE;
	const int   FnDisable = FN_DISABLE;	
	const size_t LandCount = GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{	
		LandPtr = GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		LandPtr->SetLandTempInt(FnDisable);
	}
	for ( i=Start; i<End; i++ )
	{		
		if ( i >= WndCount ) { break; }
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }				
		WndLogicType = WndPtr->GetWndLogicType();
		if ( WND_LOGIC_NONE == WndLogicType ) { continue; }
		ResultID = WndPtr->GetWndResultID();
		if ( RESULT_ID_OK != ResultID ) { continue; }
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			if ( true == bBodyOffset )
			{	continue; }
		}
		else
		{
			if ( LandPtr->GetLandTempInt() == FnEnable )
			{	continue; }
		}
		WndDefectID = WndPtr->GetWndDefectID();		
		if ( AOIDataDefine.CheckWndDefectIDCanToAlign(WndDefectID) == false ) 
		{	continue; }

		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		AlgParam.CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
		if ( UpdateModelInspectionPosRes(WndPtr, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, ApplySkewAngle) == false )
		{	return false; }
		if ( NULL != LandPtr )
		{	LandPtr->SetLandTempInt(FnEnable); }
		else
		{	bBodyOffset = true; }		
	}		

	//抓最後一個
	for ( i=End-1; i>=Start; i-- )
	{		
		if ( i >= WndCount ) { break; }
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }		
		WndLogicType = WndPtr->GetWndLogicType();
		if ( WND_LOGIC_NONE == WndLogicType ) { continue; }
		ResultID = WndPtr->GetWndResultID();
		if ( RESULT_ID_NG != ResultID ) { continue; }		
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			if ( true == bBodyOffset )
			{	continue; }
		}
		else
		{
			if ( LandPtr->GetLandTempInt() == FnEnable )
			{	continue; }
		}
		WndDefectID = WndPtr->GetWndDefectID();		
		if ( AOIDataDefine.CheckWndDefectIDCanToAlign(WndDefectID) == false ) 
		{	continue; }

		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		AlgParam.CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
		if ( UpdateModelInspectionPosRes(WndPtr, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, ApplySkewAngle) == false )
		{	return false; }		
		if ( NULL != LandPtr )
		{	LandPtr->SetLandTempInt(FnEnable); }
		else
		{	bBodyOffset = true; }	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspectionKernelBaseValue_Fn(size_t Start, size_t End)//執行模組檢測-基準值更新
{
	const size_t WndCount = GetModelWndOrderCount();
	if ( End > WndCount ) { return false; }
	if ( Start > WndCount ) { return false; }	

	size_t       i=0, j=0;		
	size_t       LandWndCount=0;
	int          BaseValueGroupID=0;
	bool         BaseValueEnabled=false;
	double       RefBaseValueReading=0;
	int          RefBaseValueGroupID=0;	
	CAOIWnd     *WndPtr = NULL;	
	CAOIWnd     *RefWndPtr = NULL;		
	CAOILand    *LandPtr = NULL;	
	CAOILand    *RefLandPtr = NULL;	
	WND_DEFECT_ID  WndDefectID;	
	const int   FnEnable  = FN_ENABLE;
	const int   FnDisable = FN_DISABLE;	
	const size_t LandCount = GetModelLandCount();	

	RECT WndRect = { 0,0,0,0 };
	double avgX = 0, avgY = 0, avgZ = 0;
	double a = 0, b = 0, c;
	double s11 = 0, s12 = 0, s13 = 0, s22 = 0, s23 = 0;
	double value =0;
	std::vector<double> WndCenterXList(WndCount);
	std::vector<double> WndCenterYList(WndCount);
	std::vector<double> WndCenterZList(WndCount);
	WndCenterXList.clear();
	WndCenterYList.clear();
	WndCenterZList.clear();
	for ( i=Start; i<End; i++ )
	{		
		if ( i >= WndCount ) { break; }
		RefWndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == RefWndPtr ) { continue; }
		if ( RefWndPtr->GetWndEnabled() == false ) { continue; }
		if ( WND_DEFECT_BASE_VALUE != RefWndPtr->GetWndDefectID() ) { continue; }
		RefWndPtr->GetWndExtendImageRect(WndRect);

		RefLandPtr = RefWndPtr->GetWndLandPtr();
		RefBaseValueGroupID = RefWndPtr->GetWndAlgParam().GetAlgBaseValueGroupID();
		if (RefWndPtr->GetWndAlgParam().GetAlgBaseValueEnabled() && RefWndPtr->GetWndAlgParam().GetAlgParamGroupCompare().gc3DHeightBaseMode == ALG_3D_BASE_HEIGHT_SQR)
		{//蒐集平面資訊
			avgZ += RefWndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brAverageReading;
			avgX += (WndRect.left + WndRect.right) / 2;
			avgY += (WndRect.bottom + WndRect.top) / 2;

			WndCenterXList.push_back((WndRect.left + WndRect.right) / 2);
			WndCenterYList.push_back((WndRect.bottom + WndRect.top) / 2);
			WndCenterZList.push_back(RefWndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brAverageReading);
			continue;
		}

		RefBaseValueReading = RefWndPtr->GetWndAlgParam().GetAlgBaseValueReading();
		RefBaseValueReading = RefWndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brAverageReading;
		
		for ( j=End; j<WndCount; j++ )			
		{	
			WndPtr = GetModelWndOrderPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			LandPtr = WndPtr->GetWndLandPtr();
			if ( LandPtr != RefLandPtr ) { continue; }

			WndDefectID = WndPtr->GetWndDefectID();			
			if ( WND_DEFECT_BASE_VALUE == WndDefectID ) { continue; }

			BaseValueGroupID = WndPtr->GetWndAlgParam().GetAlgBaseValueGroupID();
			BaseValueEnabled = WndPtr->GetWndAlgParam().GetAlgBaseValueEnabled();
			if ( false == BaseValueEnabled ) { continue; }
			if ( BaseValueGroupID != RefBaseValueGroupID ) { continue; }
			WndPtr->GetWndAlgParam().SetAlgBaseValueReading(RefBaseValueReading);
		}
	}

	if (RefWndPtr->GetWndAlgParam().GetAlgBaseValueEnabled() && 
		RefWndPtr->GetWndAlgParam().GetAlgParamGroupCompare().gc3DHeightBaseMode == ALG_3D_BASE_HEIGHT_SQR &&
		WndCenterXList.size() > 5)
	{//計算ABC用於基準
		avgZ = avgZ / WndCenterXList.size();
		avgX = avgX / WndCenterXList.size();
		avgY = avgY / WndCenterXList.size();
		s11 = 0, s12 = 0, s13 = 0, s22 = 0, s23 = 0;
		for (j = 0; j < WndCenterXList.size(); j++)
		{
			s11 += std::pow(WndCenterXList[j] - avgX, 2);
			s12 += (WndCenterXList[j] - avgX) * (WndCenterYList[j] - avgY);
			s13 += (WndCenterXList[j] - avgX) * (WndCenterZList[j] - avgZ);
			s22 += std::pow(WndCenterYList[j] - avgY, 2);
			s23 += (WndCenterYList[j] - avgY) * (WndCenterZList[j] - avgZ);
		}
		a = (s13 *s22 - s12*s23) / (s11*s22 - std::pow(s12, 2));
		b = (s11*s23 - s12*s13) / (s11*s22 - std::pow(s12, 2));
		c = avgZ - a*avgX - b*avgY;

		for (i = Start; i < End; i++)
		{ //重新計算一次平面資訊，給值
			if (i >= WndCount) { break; }
			RefWndPtr = GetModelWndOrderPtr(i, false);
			if (NULL == RefWndPtr) { continue; }
			if (RefWndPtr->GetWndEnabled() == false) { continue; }
			if (WND_DEFECT_BASE_VALUE != RefWndPtr->GetWndDefectID()) { continue; }

			RefWndPtr->GetWndAlgParam().GetAlgParamGroupCompare().gc3DHeightBasePlaneA = a;
			RefWndPtr->GetWndAlgParam().GetAlgParamGroupCompare().gc3DHeightBasePlaneB = b;
			RefWndPtr->GetWndAlgParam().GetAlgParamGroupCompare().gc3DHeightBasePlaneC = c;

			RefLandPtr = RefWndPtr->GetWndLandPtr();
			RefBaseValueGroupID = RefWndPtr->GetWndAlgParam().GetAlgBaseValueGroupID();
			for (j = End; j < WndCount; j++)
			{
				WndPtr = GetModelWndOrderPtr(j, false);
				if (NULL == WndPtr) { continue; }
				if (WndPtr->GetWndEnabled() == false) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if (LandPtr != RefLandPtr) { continue; }

				WndDefectID = WndPtr->GetWndDefectID();
				if (WND_DEFECT_BASE_VALUE == WndDefectID) { continue; }

				BaseValueGroupID = WndPtr->GetWndAlgParam().GetAlgBaseValueGroupID();
				BaseValueEnabled = WndPtr->GetWndAlgParam().GetAlgBaseValueEnabled();
				if (false == BaseValueEnabled) { continue; }
				if (BaseValueGroupID != RefBaseValueGroupID) { continue; }
				
				WndPtr->GetWndExtendImageRect(WndRect);
				value = a * ((WndRect.left + WndRect.right) / 2) + b * ((WndRect.bottom + WndRect.top) / 2) + c;
				WndPtr->GetWndAlgParam().SetAlgBaseValueReading(value);
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelInspectionKernelClassCheck_Fn(size_t Start, size_t End, int &ActClassID)//執行模組檢測-類別確認
{
	const size_t WndCount = GetModelWndOrderCount();
	if ( End > WndCount ) { return false; }
	if ( Start > WndCount ) { return false; }	

	size_t       i=0, j=0;		
	size_t       ClassCheckCnt=0;
	size_t       LandWndCount=0;	
	CAOIWnd     *WndPtr = NULL;		
	RESULT_ID    ResultID;	
	const size_t LandCount = GetModelLandCount();	
	for ( i=Start; i<End; i++ )
	{		
		if ( i >= WndCount ) { break; }
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		if ( WND_DEFECT_CLASS_CHECK != WndPtr->GetWndDefectID() ) { continue; }
		ClassCheckCnt ++;
		ResultID = WndPtr->GetWndResultID();
		if ( RESULT_ID_OK != ResultID ) { continue; }
		ActClassID = WndPtr->GetWndClassID();
		break;
	}
	
	//若全部類別確認都失敗就要報錯
	if ( 0==ClassCheckCnt || MODEL_CLASS_ID_NONE==ActClassID )
	{	return true;	}
	
	//將其他設定成Bypass以避免影響後面判斷
	RESULT_ID    ResultID2=RESULT_ID_SKIP;	
	for ( i=Start; i<End; i++ )
	{		
		if ( i >= WndCount ) { break; }
		WndPtr = GetModelWndOrderPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		if ( WND_DEFECT_CLASS_CHECK != WndPtr->GetWndDefectID() ) { continue; }
		ResultID = WndPtr->GetWndResultID();
		if ( RESULT_ID_OK == ResultID ) { continue; }		
		WndPtr->SetWndResultID(ResultID2);		
		WndPtr->SetWndLogicResultID(ResultID2);
		WndPtr->GetWndAlgParam().SetAlgResultID(ResultID2);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::RotateModelUniFrameList(double Angle, const TREGION4D &ModelRgn, const TREGION4D ModelRgnRoated, const std::vector<TUNI_FRAME> &UniFrameList, std::vector<TUNI_FRAME> &UniFrameListDst)//旋轉模組通用影像列表
{
	size_t       i = 0;	
	bool         IsOK = true;
	RECT         ModelRect={0,0,0,0};
	TPOINT2D     Scale;	
	TUNI_FRAME   UniFrame;
	TUNI_FRAME   UniFrameDst;
	MASK_PTR     MaskPtr=NULL;
	MASK_PTR     ModelMaskPtr=NULL;
	MASK_PTR     MaskPtrRotated=NULL;
	SPACE_PTR    SpacePtr=NULL;
	SPACE_PTR    ModelSpacePtr=NULL;
	SPACE_PTR    SpacePtrRotated=NULL;
	IMAGE_PTR    ImagePtr=NULL;
	IMAGE_PTR    ModelImagePtr=NULL;
	IMAGE_PTR    ImagePtrRotated=NULL;
	IMAGE_SIZE   ModelW=0, ModelH=0, ModelStep=0, ModelBitCount=0;
	IMAGE_SIZE   ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	IMAGE_SIZE   ImageWRotated=0, ImageHRotated=0, ImageStepRotated=0, BitCountRotated=0;
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	const size_t UniFrameCount = UniFrameList.size();
	UniFrameListDst.clear();
	if ( 0 == UniFrameCount ) { return false; }

	UniFrame = UniFrameList[0];
	ImageW = UniFrame.ImageW;
	ImageH = UniFrame.ImageH;
	//CAOIModel::GetModelTotalRegion(ModelRgnRoated);
	const double ModelRgnW = ModelRgn.GetWidth();
	const double ModelRgnH = ModelRgn.GetHeight();
	const double ModelWRotated = ModelRgnRoated.GetWidth();
	const double ModelHRotated = ModelRgnRoated.GetHeight();
	Scale.x = ImageW/ModelRgnW;
	Scale.y = ImageH/ModelRgnH;	
	ModelW = JetAPI::Floor(ModelWRotated*Scale.x);
	ModelH = JetAPI::Floor(ModelHRotated*Scale.y);

	if ( true == IsExceptionAngle )
	{	ImageHRotated = ImageWRotated = ImageAPI.CalcRotateImageSize(ImageW, ImageH);	}
	else
	{
		ImageWRotated = ImageW;
		ImageHRotated = ImageH;
	}
	//提前判定旋轉後的圖像尺寸
	if ( ModelW>ImageWRotated || ModelH>ImageHRotated ) 
	{	return false; }

	for ( i=0; i<UniFrameCount; i++ )
	{		
		UniFrame = UniFrameList[i];
		ImageW = UniFrame.ImageW;
		ImageH = UniFrame.ImageH;
		ImageStep = UniFrame.ImageStep;
		BitCount = UniFrame.BitCount;
		ImagePtr = UniFrame.ImagePtr;
		MaskPtr = UniFrame.MaskPtr;
		SpacePtr = UniFrame.SpacePtr;		

		if ( NULL != ImagePtr )
		{			
			ModelStep = JetAPI::GetBMPImagePixelsPerLine(ModelW, BitCount, 4);
			if ( ImageAPI.RotateImage(Angle, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageWRotated, ImageHRotated, ImageStepRotated, ImagePtrRotated) == false )
			{	break;	}			
			ModelRect.left = (ImageWRotated-ModelW)/2;
			ModelRect.right = ModelRect.left+ModelW;
			ModelRect.top = (ImageHRotated-ModelH)/2;
			ModelRect.bottom = ModelRect.top+ModelH;
			if ( ImageAPI.ExtractRoiImage(ImageWRotated, ImageHRotated, ImageStepRotated, BitCount, ImagePtrRotated, ModelRect, ModelStep, ModelImagePtr, false) == false )
			{	break;	}
			JetMemory.free_func(ImagePtrRotated);
		}		

		if ( NULL != SpacePtr )
		{
			ModelBitCount = 8;
			ModelStep = JetAPI::GetBMPImagePixelsPerLine(ModelW, ModelBitCount, 4);			
			if ( ImageAPI.RotateSpace(Angle, ImageW, ImageH, ImageStep, SpacePtr, ImageWRotated, ImageHRotated, ImageStepRotated, SpacePtrRotated) == false )
			{	break;	}
			ModelRect.left = (ImageWRotated-ModelW)/2;
			ModelRect.right = ModelRect.left+ModelW;
			ModelRect.top = (ImageHRotated-ModelH)/2;
			ModelRect.bottom = ModelRect.top+ModelH;
			if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageWRotated, ImageHRotated, ImageStepRotated, SpacePtrRotated, ModelRect, ModelStep, ModelSpacePtr, false) == false )
			{	break; }
			JetMemory.free_func(SpacePtrRotated);
		}

		if ( NULL != MaskPtr )
		{
			ModelBitCount = 8;
			ModelStep = JetAPI::GetBMPImagePixelsPerLine(ModelW, ModelBitCount, 4);			
			if ( ImageAPI.RotateMask(Angle, ImageW, ImageH, ImageStep, MaskPtr, ImageWRotated, ImageHRotated, ImageStepRotated, MaskPtrRotated) == false )
			{	break;	}
			ModelRect.left = (ImageWRotated-ModelW)/2;
			ModelRect.right = ModelRect.left+ModelW;
			ModelRect.top = (ImageHRotated-ModelH)/2;
			ModelRect.bottom = ModelRect.top+ModelH;
			if ( ImageAPI.ExtractGrayRoiImage(ImageWRotated, ImageHRotated, ImageStepRotated, MaskPtrRotated, ModelRect, ModelStep, ModelMaskPtr, false) == false )
			{	break; }
			JetMemory.free_func(MaskPtrRotated);
		}

		ModelStep = JetAPI::GetBMPImagePixelsPerLine(ModelW, BitCount, 4);
		UniFrameDst.FrameUniqueID = UniFrame.FrameUniqueID;
		UniFrameDst.ImageW = ModelW;
		UniFrameDst.ImageH = ModelH;
		UniFrameDst.ImageStep = ModelStep;
		UniFrameDst.BitCount = BitCount;
		UniFrameDst.ImagePtr = ModelImagePtr;
		UniFrameDst.MaskPtr = ModelMaskPtr;
		UniFrameDst.SpacePtr = ModelSpacePtr;
		UniFrameListDst.push_back(UniFrameDst);
		
		ModelStep = 0;
		ModelImagePtr = NULL;
		ModelSpacePtr = NULL;
		ModelMaskPtr = NULL;

		ImageWRotated = 0;
		ImageHRotated = 0;
		ImageStepRotated = 0;
		ImagePtrRotated = NULL;
		SpacePtrRotated = NULL;
		MaskPtrRotated = NULL;
	}

	if ( UniFrameCount != i )
	{
		JetMemory.free_func(ImagePtrRotated);
		JetMemory.free_func(SpacePtrRotated);
		JetMemory.free_func(MaskPtrRotated);

		JetMemory.free_func(ModelImagePtr);
		JetMemory.free_func(ModelSpacePtr);
		JetMemory.free_func(ModelMaskPtr);
		JetAPI::ClearUniFrameList(UniFrameListDst);
		return false;
	}

	BOOL bSaved=TRUE;
#ifdef _DEBUG
	CString str;
	if ( TRUE == bSaved )
	{
		for ( i=0; i<UniFrameCount; i++ )
		{		
			UniFrame = UniFrameListDst[i];
			ImageW = UniFrame.ImageW;
			ImageH = UniFrame.ImageH;
			ImageStep = UniFrame.ImageStep;
			BitCount = UniFrame.BitCount;
			ImagePtr = UniFrame.ImagePtr;
			MaskPtr = UniFrame.MaskPtr;
			SpacePtr = UniFrame.SpacePtr;

			if ( NULL != ImagePtr )
			{
				str.Format(_T("%s\\%s_%d.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("RotatedImg"), i+1);
				ImageAPI.SavePNGImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);			
			}
		}
	}
#endif //_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelWndGroupInspection(std::vector<TUNI_FRAME> &UniFrameList)//執行模組檢測檢測框群組檢測
{
	size_t       i=0, j=0, k=0;
	CAOIWnd     *WndPtr = NULL;
	CAOIWnd     *WndPtr2= NULL;
	CAOIWnd     *Min2DWndPtr = NULL;
	CAOIWnd     *Max2DWndPtr = NULL;
	CAOIWnd     *Min3DWndPtr = NULL;
	CAOIWnd     *Max3DWndPtr = NULL;
	CAOIWnd     *Ignor3DWndPtr = NULL;
	CAOIWnd     *Base3DAngleWndPtr = NULL;	
	const size_t FrameCount = UniFrameList.size();
	const size_t WndCount = GetModelWndCount();
	
	//Group Compare
	bool              bGroup3D=false;
	CString           strText;
	bool              bEnable2D=false, bEnable3D=false, bEnable3DAngle=false;
	double            dUSL2D=0, dLSL2D=0;
	double            dUSL3D=0, dLSL3D=0;	
	double            dSum2D=0, dSum3D=0;
	double            dMax3D=0, dMin3D=0;
	double            dAve2D=0, dAve3D=0;
	double            dUSL3DAngle=0, dLSL3DAngle=0;
	double            dBase2D=0, dBase3D=0, dBase3DAngle=0;
	size_t            WndGroupCount = 0;
	int               WndGroupID = 0;	
	BOX_TOWARD        WndToward = BOX_TOWARD_NULL;
	RECT              WndRect={0,0,0,0};
	RESULT_ID         ResultID=RESULT_ID_NONE;	
	ALG_GROUP_CMP_DIR_MODE eDirectionMode;
	std::vector<RECT> WndRectList(WndCount);
	std::vector<CAOIWnd*> WndPtrList(WndCount);	
	CAlgParam *AlgParamPtr = NULL;
	CAlgParam *AlgParamPtr2 = NULL;	

	double avgX = 0, avgY = 0;
	double a = 0, b = 0, c;
	double s11 = 0, s12 = 0, s13 = 0, s22 = 0, s23 = 0;
	std::vector<double> WndCenterXList(WndCount);
	std::vector<double> WndCenterYList(WndCount);
	std::vector<double> WndCenterZList(WndCount);
	WndPtrList.clear();
	WndRectList.clear();
	WndCenterXList.clear();
	WndCenterYList.clear();
	WndCenterZList.clear();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }		
		ResultID = WndPtr->GetWndResultID();		
		if ( RESULT_ID_NONE == ResultID ) { continue; }
		if ( RESULT_ID_EXCEPTION == ResultID ) { continue; }

		AlgParamPtr = WndPtr->GetWndAlgParamPtr();	
		TALG_PARAM_GROUP_COMPARE &gcParam=AlgParamPtr->GetAlgParamGroupCompare();

		//計算過了
		if ( RESULT_ID_NONE != gcParam.gcResultID ) { continue; }
		WndToward = WndPtr->GetWndToward();
		gcParam.gcResultID = RESULT_ID_OK;
		eDirectionMode = gcParam.gcDirectionMode;
		bEnable2D = gcParam.gc2DGrayEnabled; 
		bEnable3D = gcParam.gc3DHeightEnabled; 
		bEnable3DAngle = gcParam.gcTiltAngleEnabled; 		
		//if ( true==bEnable2D || true==bEnable3D || true==bEnable3DAngle )
		if ( (true==bEnable2D || true==bEnable3D || true==bEnable3DAngle) && WND_DEFECT_BASE_VALUE != WndPtr->GetWndDefectID())// 先保持原本base沒作用//Ryzen
		{
			WndPtrList.clear();
			WndRectList.clear();	
			WndCenterXList.clear();
			WndCenterYList.clear();
			WndCenterZList.clear();
			dSum2D = dSum3D = 0.0;
			dMin3D =  DBL_MAX;
			dMax3D = -DBL_MAX;
			Min2DWndPtr = NULL;
			Max2DWndPtr = NULL;
			Min3DWndPtr = NULL;
			Max3DWndPtr = NULL;
			dUSL2D = gcParam.gc2DGrayUSL;
			dLSL2D = gcParam.gc2DGrayLSL;
			dUSL3D = gcParam.gc3DHeightUSL;
			dLSL3D = gcParam.gc3DHeightLSL;
			dUSL3DAngle = gcParam.gcTiltAngleUSL;
			dLSL3DAngle = gcParam.gcTiltAngleLSL;

			WndGroupID = WndPtr->GetWndGroupID();
			for ( j=0; j<WndCount; j++ )
			{
				WndPtr2 = GetModelWndPtr(j, false);
				if ( NULL == WndPtr2 ) { continue; }
				if ( WndPtr2->GetWndEnabled() == false ) { continue; }
				if ( WndPtr2->GetWndGroupID() != WndGroupID ) { continue; }				
				if ( ALG_GROUP_CMP_DIR_ONE == eDirectionMode )
				{
					if ( WndPtr2->GetWndToward() != WndToward ) { continue;}					
				}

				WndPtr2->GetWndExtendImageRect(WndRect);
				AlgParamPtr2 = WndPtr2->GetWndAlgParamPtr();	
				TALG_PARAM_GROUP_COMPARE &gcParam2 = AlgParamPtr2->GetAlgParamGroupCompare();				

				if ( dMax3D < gcParam2.gc3DHeightValue ) 
				{ 
					Max3DWndPtr = WndPtr2;
					dMax3D = gcParam2.gc3DHeightValue; 
				}
				if ( dMin3D > gcParam2.gc3DHeightValue ) 
				{
					Min3DWndPtr = WndPtr2;
					dMin3D = gcParam2.gc3DHeightValue; 
				}				
				
				dSum2D += gcParam2.gc2DGrayValue;
				dSum3D += gcParam2.gc3DHeightValue;
				avgX += (WndRect.left + WndRect.right) / 2;
				avgY += (WndRect.bottom + WndRect.top) / 2;

				WndPtrList.push_back(WndPtr2);
				WndRectList.push_back(WndRect);
				WndCenterXList.push_back((WndRect.left + WndRect.right) / 2);
				WndCenterYList.push_back((WndRect.bottom + WndRect.top) / 2);
				WndCenterZList.push_back(gcParam2.gc3DHeightValue);
			}

			WndGroupCount = WndPtrList.size();
			if ( 0 == WndGroupCount ) { continue; }
			dAve2D = dSum2D/WndGroupCount;
			dAve3D = dSum3D/WndGroupCount;
			avgX = avgX / WndGroupCount;
			avgY = avgY / WndGroupCount;
			dBase2D = dAve2D;
			dBase3D = dMin3D;

			switch ( gcParam.gc3DHeightBaseMode )
			{
			case ALG_3D_BASE_HEIGHT_MAX:	
				dBase3D = dMax3D;	
				Ignor3DWndPtr = Max3DWndPtr;
				break;
			case ALG_3D_BASE_HEIGHT_AVE:	
				dBase3D = dAve3D;	
				break;
			case ALG_3D_BASE_HEIGHT_MID:	
				dBase3D = dAve3D;	
				break;
			case ALG_3D_BASE_HEIGHT_SQR:
				s11 = 0, s12 = 0, s13 = 0, s22 = 0, s23 = 0;
				if (WndGroupCount < 5) { dBase3D = dAve3D; break; } //點數太少，改用平均
				for (j = 0; j < WndGroupCount; j++)
				{
					s11 += std::pow(WndCenterXList[j] - avgX, 2);
					s12 += (WndCenterXList[j] - avgX) * (WndCenterYList[j] - avgY);
					s13 += (WndCenterXList[j] - avgX) * (WndCenterZList[j] - dAve3D);
					s22 += std::pow(WndCenterYList[j] - avgY, 2);
					s23 += (WndCenterYList[j] - avgY) * (WndCenterZList[j] - dAve3D);
				}
				a = (s13 *s22 - s12*s23) / (s11*s22 - std::pow(s12, 2));
				b = (s11*s23 - s12*s13) / (s11*s22 - std::pow(s12, 2));
				c = dAve3D - a*avgX - b*avgY;
				//strText.Format(_T("%d,%.3f,%.3f,%.3f"), WndGroupCount, a, b, c);
				//AOIDataCollect.SaveInitialReleaseLog(strText);
				break;
			default://ALG_3D_BASE_HEIGHT_MIN
				dBase3D = dMin3D;
				Ignor3DWndPtr = Min3DWndPtr;
				break;
			}

			Base3DAngleWndPtr = WndPtrList[0];
			if ( NULL == Base3DAngleWndPtr )
			{	dBase3DAngle = 0.0; }
			else
			{	
				TALG_PARAM_GROUP_COMPARE &gcParam2 = Base3DAngleWndPtr->GetWndAlgParam().GetAlgParamGroupCompare();				
				dBase3DAngle = gcParam2.gc3DHeightValue;
			}

			strText = _T("");
			gcParam.gc3DHeightBase = dBase3D;
			for ( j=0; j<WndGroupCount; j++ )
			{
				WndPtr2 = WndPtrList[j];
				AlgParamPtr2 = WndPtr2->GetWndAlgParamPtr();	
				TALG_PARAM_GROUP_COMPARE &gcParam2 = AlgParamPtr2->GetAlgParamGroupCompare();
				if (gcParam2.gc3DHeightBaseMode != ALG_3D_BASE_HEIGHT_SQR || WndGroupCount < 5)
				{
					gcParam2.gc3DHeightBase = dBase3D;				
					gcParam2.gc3DHeightReading = gcParam2.gc3DHeightValue-dBase3D;									
				}
				else
				{
					dBase3D = a * WndCenterXList[j] + b * WndCenterYList[j] + c;
					gcParam2.gc3DHeightBase = dBase3D;
					gcParam2.gc3DHeightReading = gcParam2.gc3DHeightValue - dBase3D;
					gcParam2.gc3DHeightBasePlaneA = a;
					gcParam2.gc3DHeightBasePlaneB = b;
					gcParam2.gc3DHeightBasePlaneC = c;
				}
				gcParam2.gc2DGrayReading = gcParam2.gc2DGrayValue-dBase2D;
				gcParam2.gcResultID = RESULT_ID_OK;				
				if ( true == bEnable2D ) 
				{
					if ( gcParam2.gc2DGrayReading > dUSL2D ) { gcParam2.gcResultID = RESULT_ID_NG; }
					if ( gcParam2.gc2DGrayReading < dLSL2D ) { gcParam2.gcResultID = RESULT_ID_NG; }
					if ( RESULT_ID_NG == gcParam2.gcResultID )
					{
						CString Key = AOIDataDefine.GetGrayText();
						strText.Format(_T("%s:%.0f (%.0f ~ %.0f)"), Key, gcParam2.gc2DGrayReading, dLSL2D, dUSL2D);
					}
				}
				if ( true == bEnable3D )
				{
					if ( WndPtr2 != Ignor3DWndPtr )//因為3D高度是相對於最低檢測框, 所以跳過該最低檢測框
					{
						if ( gcParam2.gc3DHeightReading > dUSL3D ) { gcParam2.gcResultID = RESULT_ID_NG; }
						if ( gcParam2.gc3DHeightReading < dLSL3D ) { gcParam2.gcResultID = RESULT_ID_NG; }
						if ( RESULT_ID_NG == gcParam2.gcResultID )
						{
							CString Key = AOIDataDefine.GetThicknessText();
							CString Key2 = AOIDataDefine.GetRelativeText();
							strText.Format(_T("%s-%s:%.0fum (%.0f ~ %.0f)"), Key2, Key, gcParam2.gc3DHeightReading, dLSL3D, dUSL3D);
						}
					}					
				}
				if ( true == bEnable3DAngle )
				{
					if ( WndPtr2 != Base3DAngleWndPtr )
					{
						TPOINT2D PosRes1, PosRes2;
						WndPtr2->GetWndBoxPtr()->GetBoxPosRes(PosRes2);
						Base3DAngleWndPtr->GetWndBoxPtr()->GetBoxPosRes(PosRes1);
						const double DifH = gcParam2.gc3DHeightValue-dBase3DAngle;
						const double PosL = JetAPI::CalcDistance(PosRes2.x-PosRes1.x, PosRes2.y-PosRes1.y);
						const double TiltAngle=::atan2(DifH, PosL)*RAD_TO_DEG_DBL;

						gcParam2.gcTiltAngleReading = TiltAngle;						
						if ( TiltAngle > dUSL3DAngle ) { gcParam2.gcResultID = RESULT_ID_NG; }
						if ( TiltAngle < dLSL3DAngle ) { gcParam2.gcResultID = RESULT_ID_NG; }
						if ( RESULT_ID_NG == gcParam2.gcResultID )
						{
							CString Key = AOIDataDefine.GetAngleText();							
							strText.Format(_T("%s %.2f (%.0f ~ %.0f)"), Key, TiltAngle, dLSL3DAngle, dUSL3DAngle);
						}
					}	
				}
				if ( RESULT_ID_NG == gcParam2.gcResultID )
				{	
					if ( strText.GetLength() == 0 ) 
					{	strText = _T("NG"); }
					WndPtr2->SetWndResultID(RESULT_ID_NG);
					WndPtr2->SetWndResultText(strText);
					WndPtr2->SetWndLogicResultID(RESULT_ID_NG);
					AlgParamPtr2->SetAlgResultText(strText);
					AlgParamPtr2->SetAlgResultID(RESULT_ID_NG);
				}
			}
		}
	}


	//邏輯群組		
	int            WndGroupID2 = 0;	
	int            LogicGroupID = 0;
	int            LogicGroupID2 = 0;
	CAOILand      *LandPtr = NULL;
	CAOILand      *LandPtr2 = NULL;
	WND_DEFECT_ID  DefectID  = WND_DEFECT_NONE;
	WND_DEFECT_ID  DefectID2 = WND_DEFECT_NONE;
	WND_LOGIC_TYPE LogicType =WND_LOGIC_NONE;
	WND_LOGIC_TYPE LogicType2=WND_LOGIC_NONE;
	RESULT_ID      ResultID2=RESULT_ID_NONE;
	RESULT_ID      LogicResultID=RESULT_ID_NONE;	
	RESULT_ID      LogicResultID2=RESULT_ID_NONE;
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }

		LogicResultID = WndPtr->GetWndLogicResultID();//比較過了
		if ( RESULT_ID_NONE != LogicResultID )	{	continue;	}
		ResultID = WndPtr->GetWndResultID();
		LogicType = WndPtr->GetWndLogicType();
		if ( WND_LOGIC_NONE==LogicType || RESULT_ID_BYPASS==ResultID || RESULT_ID_SKIP==ResultID || RESULT_ID_NONE==ResultID || RESULT_ID_EXCEPTION==ResultID )//無使用邏輯閘或者Bypass
		{
			WndPtr->SetWndLogicResultID(ResultID);
			continue;
		}		
		if ( RESULT_ID_NONE == ResultID ) { continue; }
		if ( RESULT_ID_EXCEPTION == ResultID ) { continue; }		
		
		LandPtr = WndPtr->GetWndLandPtr();
		DefectID = WndPtr->GetWndDefectID();
		WndGroupID = WndPtr->GetWndGroupID();
		LogicGroupID = WndPtr->GetWndLogicGroupID();

		WndPtrList.clear();		
		AlgParamPtr = WndPtr->GetWndAlgParamPtr();	
		LogicResultID = RESULT_ID_NG;
		for ( j=0; j<WndCount; j++ )
		{
			WndPtr2 = GetModelWndPtr(j, false);
			if ( NULL == WndPtr2 ) { continue; }
			if ( WndPtr2->GetWndEnabled() == false ) { continue; }
			
			LandPtr2 = WndPtr2->GetWndLandPtr();//不同隻腳
			if ( LandPtr2 != LandPtr ) { continue; }

			LogicType2 = WndPtr2->GetWndLogicType();//邏輯樣式
			if ( LogicType2 != LogicType ) { continue; }

			LogicGroupID2 = WndPtr2->GetWndLogicGroupID();//邏輯群組
			if ( LogicGroupID2 != LogicGroupID ) { continue; }

			DefectID2 = WndPtr2->GetWndDefectID();
			WndGroupID2 = WndPtr2->GetWndGroupID();
			LogicResultID2 = WndPtr2->GetWndLogicResultID();

			if ( WND_LOGIC_GROUP_ID == LogicType )
			{
				if ( WndGroupID2 != WndGroupID ) { continue; }
			}
			if ( WND_LOGIC_DEFECT_ID == LogicType)
			{
				if ( DefectID2 != DefectID ) { continue; }
			}

			WndPtrList.push_back(WndPtr2);
			AlgParamPtr2 = WndPtr2->GetWndAlgParamPtr();	
			ResultID2 = WndPtr2->GetWndResultID();
			if ( RESULT_ID_OK == ResultID2 )	
			{	LogicResultID = RESULT_ID_OK; }			
		}

		WndGroupCount = WndPtrList.size();
		if ( 0 == WndGroupCount ) { continue; }
		for ( j=0; j<WndGroupCount; j++ )
		{
			WndPtr2 = WndPtrList[j];
			WndPtr2->SetWndLogicResultID(LogicResultID);
		}
	}

	//總判斷
	//SetModelResultID(RESULT_ID_OK);
	//SetModelResultID_Alarm(RESULT_ID_OK);
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }

		LogicResultID = WndPtr->GetWndLogicResultID();//比較過了
		if ( RESULT_ID_NONE == LogicResultID ) { continue; }
		if ( RESULT_ID_OK == LogicResultID ) { continue; }
		if ( RESULT_ID_SKIP == LogicResultID ) { continue; }
		if ( RESULT_ID_BYPASS == LogicResultID ) { continue; }		
		
		//SetModelResultID(RESULT_ID_NG);	
		//SetModelResultID_Alarm(RESULT_ID_NG);	
		break;
	}

	//移除邏輯閘OK的檢測框圖檔
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndEnabled() == false ) { continue; }		
		LogicResultID = WndPtr->GetWndLogicResultID();
		if ( RESULT_ID_NG == LogicResultID ) { continue; }
		if ( RESULT_ID_EXCEPTION == LogicResultID ) { continue; }

		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		if ( AlgParam.GetAlgSaveDefectImageDone() == false ) { continue; }		
		CString Filename=AlgParam.GetAlgSaveDefectImageName();
		if ( Filename.GetLength() == 0 ) { continue; }
		::DeleteFile(Filename);
		AlgParam.SetAlgSaveDefectImageDone(false);
		AlgParam.SetAlgSaveDefectImageName(_T(""));		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
AOI_OBJ_TYPE CAOIModel::GetModelAttachedType() const
{
	CAOIRgn *RgnPtr = CAOIModel::GetModelAttachedPtr();
	if ( NULL == RgnPtr ) { return AOI_OBJ_NULL; }
	return RgnPtr->GetObjType();	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelRegionToAttached()//將模組的本體調整至附屬內
{
	bool IsOK = true;
	AOI_OBJ_TYPE AttachedType = CAOIModel::GetModelAttachedType();
	switch ( AttachedType )
	{
	case AOI_OBJ_FD:
		IsOK = UpdateModelBodyToFd();
		break;
	case AOI_OBJ_MARK:
		IsOK = UpdateModelBodyToMark();
		break;
	case AOI_OBJ_BARCODE:
		IsOK = UpdateModelBodyToBarcode();
		break;
	case AOI_OBJ_COMPONENT:
		IsOK = UpdateModelBodyToComponent();
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIFd* CAOIModel::GetModelFdPtr() const
{
	CAOIRgn *RgnPtr = CAOIModel::GetModelAttachedPtr();
	if ( NULL == RgnPtr ) { return NULL; }
	if ( RgnPtr->GetObjType() != AOI_OBJ_FD ) { return NULL; }
	return (CAOIFd*)(RgnPtr);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelFdPtr(CAOIFd *Ptr)
{
	m_ModelAttachedPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelBodyToFd()//將模組的本體調整至定位點內		
{
	CAOIFd *FdPtr = CAOIModel::GetModelFdPtr();
	if ( NULL == FdPtr ) { return true; }

	TREGION4D Region;
	TREGION4D RegionCad;
	TPOINT2D  CadOffset, StageOffset;
	LANE_ID   LaneID;
	double    cx=0, cy=0;
	double    px=0, py=0;
	double    sx=0, sy=0;
	double    sx2=0, sy2=0;
	double    Angle = FdPtr->GetFdAngle();
	double    CadPosX = FdPtr->GetFdCadPosX();
	double    CadPosY = FdPtr->GetFdCadPosY();
	double    StagePosX = FdPtr->GetFdStagePosX();
	double    StagePosY = FdPtr->GetFdStagePosY();
	double    StagePosX_LA=StagePosX;
	double    StagePosY_LA=StagePosY;
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);	
	CAOIBox *BoxPtr = CAOIModel::GetModelBodyBoxPtr();
	
	cx = cy = 0;
	BoxPtr->GetBoxPos(px, py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(-Angle, cx, cy);		}	
	//BoxPtr->GetBoxPos(px, py);
	BoxPtr->GetBoxSize(sx, sy);
	CAOIModel::GetModelTotalRegion(Region);
	const double BiasPosX = Region.GetCpX();
	const double BiasPosY = Region.GetCpY();	
	sx2 = Region.maxX-Region.minX;
	sy2 = Region.maxY-Region.minY;		

	CadOffset.x = px;
	CadOffset.y = py;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);

	CadPosX += CadOffset.x;
	CadPosY += CadOffset.y;

	StagePosX += StageOffset.x;
	StagePosY += StageOffset.y;	

	//旋轉零件的尺寸, 因為零件的寬長是以0度為基礎
	if ( false == IsExceptionAngle )
	{	
		JetAPI::RotateSize(Angle, sx, sy);	
		JetAPI::RotateSize(Angle, sx2, sy2);	
	}
	else
	{	JetAPI::RotatePos(-Angle, cx, cy, px, py);		}
		
	LaneID = FdPtr->GetFdLaneID();
	FdPtr->SetFdRoiSizeW(sx2);
	FdPtr->SetFdRoiSizeH(sy2);		
	FdPtr->SetFdBodySizeW(sx);
	FdPtr->SetFdBodySizeH(sy);	
	FdPtr->SetFdCadPosX(CadPosX);
	FdPtr->SetFdCadPosY(CadPosY);
//	FdPtr->SetFdCadBiasPosX(BiasPosX);
//	FdPtr->SetFdCadBiasPosY(BiasPosY);
	FdPtr->SetFdStagePosX(StagePosX);
	FdPtr->SetFdStagePosY(StagePosY);

	StagePosX_LA = StagePosX;
	StagePosY_LA = StagePosY;
	AOIDataCollect.MapStagePosToLaneA(StagePosX_LA, StagePosY_LA, LaneID);	
	FdPtr->SetFdTeachStagePosX(StagePosX_LA);
	FdPtr->SetFdTeachStagePosY(StagePosY_LA);	

	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	TREGION4D    WndRgn;	
	double       ExtendX=0;
	double       ExtendY=0;
	double       RoiRgnX=0;
	double       RoiRgnY=0;
	double       WndRgnSizeX=sx;
	double       WndRgnSizeY=sy;
	const size_t WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		WndPtr->GetWndRegionCad(WndRgn);
		WndRgnSizeX = WndRgn.GetWidth();
		WndRgnSizeY = WndRgn.GetHeight();
		if ( WndPtr->GetWndExtendBoxUsed() == true )
		{
			ExtendX = WndPtr->GetWndExtendRangeX();
			ExtendY = WndPtr->GetWndExtendRangeY();
		}
		else
		{	ExtendX = ExtendY = 0;	}
		WndRgnSizeX = WndRgnSizeX+ExtendX+ExtendX;
		WndRgnSizeY = WndRgnSizeY+ExtendY+ExtendY;
		if ( RoiRgnX < WndRgnSizeX ) { RoiRgnX = WndRgnSizeX; }
		if ( RoiRgnY < WndRgnSizeY ) { RoiRgnY = WndRgnSizeY; }

		//if ( WndPtr->GetWndExtendBoxUsed() == false ) { continue; }
		//ExtendX = WndPtr->GetWndExtendRangeX();
		//ExtendY = WndPtr->GetWndExtendRangeY();
		//break;
	}
	if ( i != WndCount )
	{
		//FdPtr->SetFdRoiExtendSizeW(ExtendX*2);
		//FdPtr->SetFdRoiExtendSizeH(ExtendY*2);
	}
	else
	{
		//FdPtr->SetFdRoiExtendSizeW(sx);
		//FdPtr->SetFdRoiExtendSizeH(sy);
	}
	//FdPtr->SetFdRoiExtendSizeW(RoiRgnX);
	//FdPtr->SetFdRoiExtendSizeH(RoiRgnY);

	FdPtr->CalcFdCadCornerPos();
	FdPtr->LayoutFdStageCornerPos();

	TPOINT2D   CadPos, StagePos;
	CadPos.x = CadPosX;
	CadPos.y = CadPosY;
	StagePos.x = StagePosX;
	StagePos.y = StagePosY;
	
	CAOIModel::MoveModel(-px, -py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(Angle, cx, cy);	}		
	CAOIModel::SetModelAttachedPosCad(CadPos);
	CAOIModel::SetModelAttachedPosStage(StagePos);		

	BoxPtr->GetBoxPos(px, py);
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIMark* CAOIModel::GetModelMarkPtr() const
{
	CAOIRgn *RgnPtr = CAOIModel::GetModelAttachedPtr();
	if ( NULL == RgnPtr ) { return NULL; }
	if ( RgnPtr->GetObjType() != AOI_OBJ_MARK ) { return NULL; }
	return (CAOIMark*)(RgnPtr);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelMarkPtr(CAOIMark *Ptr)
{
	m_ModelAttachedPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelBodyToMark()//將模組的本體調整至特徵內
{
	CAOIMark *MarkPtr = CAOIModel::GetModelMarkPtr();
	if ( NULL == MarkPtr ) { return true; }

	TREGION4D Region;
	TREGION4D RegionCad;
	TPOINT2D  CadOffset, StageOffset;
	double    cx=0, cy=0;
	double    px=0, py=0;
	double    sx=0, sy=0;
	double    sx2=0, sy2=0;
	double    Angle = MarkPtr->GetMarkAngle();
	double    CadPosX = MarkPtr->GetMarkCadPosX();
	double    CadPosY = MarkPtr->GetMarkCadPosY();
	double    StagePosX = MarkPtr->GetMarkStagePosX();
	double    StagePosY = MarkPtr->GetMarkStagePosY();	
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	CAOIBox *BoxPtr = CAOIModel::GetModelBodyBoxPtr();
	
	cx = cy = 0;
	BoxPtr->GetBoxPos(px, py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(-Angle, cx, cy);		}	
	//BoxPtr->GetBoxPos(px, py);
	BoxPtr->GetBoxSize(sx, sy);
	CAOIModel::GetModelTotalRegion(Region);
	const double BiasPosX = Region.GetCpX();
	const double BiasPosY = Region.GetCpY();	
	sx2 = Region.maxX-Region.minX;
	sy2 = Region.maxY-Region.minY;		

	CadOffset.x = px;
	CadOffset.y = py;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);

	CadPosX += CadOffset.x;
	CadPosY += CadOffset.y;

	StagePosX += StageOffset.x;
	StagePosY += StageOffset.y;	

	//旋轉零件的尺寸, 因為零件的寬長是以0度為基礎
	if ( false == IsExceptionAngle )
	{	
		JetAPI::RotateSize(Angle, sx, sy);	
		JetAPI::RotateSize(Angle, sx2, sy2);	
	}
	else
	{	JetAPI::RotatePos(-Angle, cx, cy, px, py);		}
		
	MarkPtr->SetMarkRoiSizeW(sx2);
	MarkPtr->SetMarkRoiSizeH(sy2);
	MarkPtr->SetMarkBodySizeW(sx);
	MarkPtr->SetMarkBodySizeH(sy);
	MarkPtr->SetMarkCadPosX(CadPosX);
	MarkPtr->SetMarkCadPosY(CadPosY);
	//MarkPtr->SetMarkCadBiasPosX(BiasPosX);
	//MarkPtr->SetMarkCadBiasPosY(BiasPosY);
	MarkPtr->SetMarkStagePosX(StagePosX);
	MarkPtr->SetMarkStagePosY(StagePosY);

	MarkPtr->CalcMarkCadCornerPos();
	MarkPtr->LayoutMarkStageCornerPos();

	TPOINT2D   CadPos, StagePos;
	CadPos.x = CadPosX;
	CadPos.y = CadPosY;
	StagePos.x = StagePosX;
	StagePos.y = StagePosY;
	
	CAOIModel::MoveModel(-px, -py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(Angle, cx, cy);	}		
	CAOIModel::SetModelAttachedPosCad(CadPos);
	CAOIModel::SetModelAttachedPosStage(StagePos);		

	BoxPtr->GetBoxPos(px, py);
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIBarcode* CAOIModel::GetModelBarcodePtr() const
{
	CAOIRgn *RgnPtr = CAOIModel::GetModelAttachedPtr();
	if ( NULL == RgnPtr ) { return NULL; }
	if ( RgnPtr->GetObjType() != AOI_OBJ_BARCODE ) { return NULL; }
	return (CAOIBarcode*)(RgnPtr);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelBarcodePtr(CAOIBarcode *Ptr)
{
	m_ModelAttachedPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelBodyToBarcode()//將模組的本體調整至軟體條碼內
{
	CAOIBarcode *BarcodePtr = CAOIModel::GetModelBarcodePtr();
	if ( NULL == BarcodePtr ) { return true; }

	TREGION4D Region;
	TREGION4D RegionCad;
	TPOINT2D  CadOffset, StageOffset;
	double    cx=0, cy=0;
	double    px=0, py=0;
	double    sx=0, sy=0;
	double    sx2=0, sy2=0;
	double    Angle = BarcodePtr->GetBarcodeAngle();
	double    CadPosX = BarcodePtr->GetBarcodeCadPosX();
	double    CadPosY = BarcodePtr->GetBarcodeCadPosY();
	double    StagePosX = BarcodePtr->GetBarcodeStagePosX();
	double    StagePosY = BarcodePtr->GetBarcodeStagePosY();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	CAOIBox *BoxPtr = CAOIModel::GetModelBodyBoxPtr();
	
	cx = cy = 0;
	BoxPtr->GetBoxPos(px, py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(-Angle, cx, cy);		}	
	//BoxPtr->GetBoxPos(px, py);
	BoxPtr->GetBoxSize(sx, sy);
	CAOIModel::GetModelTotalRegion(Region);
	const double BiasPosX = Region.GetCpX();
	const double BiasPosY = Region.GetCpY();	
	sx2 = Region.maxX-Region.minX;
	sy2 = Region.maxY-Region.minY;		

	CadOffset.x = px;
	CadOffset.y = py;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);

	CadPosX += CadOffset.x;
	CadPosY += CadOffset.y;

	StagePosX += StageOffset.x;
	StagePosY += StageOffset.y;	

	//旋轉零件的尺寸, 因為零件的寬長是以0度為基礎
	if ( false == IsExceptionAngle )
	{	
		JetAPI::RotateSize(Angle, sx, sy);	
		JetAPI::RotateSize(Angle, sx2, sy2);	
	}
	else
	{	JetAPI::RotatePos(-Angle, cx, cy, px, py);		}
		
	BarcodePtr->SetBarcodeRoiSizeW(sx2);
	BarcodePtr->SetBarcodeRoiSizeH(sy2);
	BarcodePtr->SetBarcodeBodySizeW(sx);
	BarcodePtr->SetBarcodeBodySizeH(sy);
	BarcodePtr->SetBarcodeCadPosX(CadPosX);
	BarcodePtr->SetBarcodeCadPosY(CadPosY);
	//BarcodePtr->SetBarcodeCadBiasPosX(BiasPosX);
	//BarcodePtr->SetBarcodeCadBiasPosY(BiasPosY);
	BarcodePtr->SetBarcodeStagePosX(StagePosX);
	BarcodePtr->SetBarcodeStagePosY(StagePosY);

	BarcodePtr->CalcBarcodeCadCornerPos();
	BarcodePtr->LayoutBarcodeStageCornerPos();

	TPOINT2D   CadPos, StagePos;
	CadPos.x = CadPosX;
	CadPos.y = CadPosY;
	StagePos.x = StagePosX;
	StagePos.y = StagePosY;
	
	CAOIModel::MoveModel(-px, -py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(Angle, cx, cy);	}		
	CAOIModel::SetModelAttachedPosCad(CadPos);
	CAOIModel::SetModelAttachedPosStage(StagePos);		

	BoxPtr->GetBoxPos(px, py);

	/*
	CAOIBarcode *BarcodePtr = CAOIModel::GetModelBarcodePtr();
	if ( NULL == BarcodePtr ) { return true; }

	TREGION4D Region;
	TREGION4D RegionCad;
	TPOINT2D  CadOffset, StageOffset;
	double    cx=0, cy=0;
	double    px=0, py=0;
	double    sx=0, sy=0;
	double    sx2=0, sy2=0;
	double    Angle = BarcodePtr->GetBarcodeAngle();
	double    CadPosX = BarcodePtr->GetBarcodeCadPosX();
	double    CadPosY = BarcodePtr->GetBarcodeCadPosY();
	double    StagePosX = BarcodePtr->GetBarcodeStagePosX();
	double    StagePosY = BarcodePtr->GetBarcodeStagePosY();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	CAOIBox *BoxPtr = CAOIModel::GetModelBodyBoxPtr();
	
	cx = cy = 0;
	BoxPtr->GetBoxPos(px, py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(-Angle, cx, cy);		}	
	//BoxPtr->GetBoxPos(px, py);
	BoxPtr->GetBoxSize(sx, sy);
	CAOIModel::GetModelTotalRegion(Region);
	const double BiasPosX = Region.GetCpX();
	const double BiasPosY = Region.GetCpY();	
	sx2 = Region.maxX-Region.minX;
	sy2 = Region.maxY-Region.minY;		

	CadOffset.x = px;
	CadOffset.y = py;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);

	CadPosX += CadOffset.x;
	CadPosY += CadOffset.y;

	StagePosX += StageOffset.x;
	StagePosY += StageOffset.y;	

	//旋轉零件的尺寸, 因為零件的寬長是以0度為基礎
	if ( false == IsExceptionAngle )
	{	
		JetAPI::RotateSize(Angle, sx, sy);	
		JetAPI::RotateSize(Angle, sx2, sy2);	
	}
	else
	{	JetAPI::RotatePos(-Angle, cx, cy, px, py);		}
		
	BarcodePtr->SetBarcodeRoiSizeW(sx);
	BarcodePtr->SetBarcodeRoiSizeH(sy);	
	BarcodePtr->SetBarcodeBodySizeW(sx);
	BarcodePtr->SetBarcodeBodySizeH(sy);
	BarcodePtr->SetBarcodeCadPosX(CadPosX);
	BarcodePtr->SetBarcodeCadPosY(CadPosY);
//	BarcodePtr->SetBarcodeCadBiasPosX(BiasPosX);
//	BarcodePtr->SetBarcodeCadBiasPosY(BiasPosY);
	BarcodePtr->SetBarcodeStagePosX(StagePosX);
	BarcodePtr->SetBarcodeStagePosY(StagePosY);

	BarcodePtr->CalcBarcodeCadCornerPos();
	BarcodePtr->LayoutBarcodeStageCornerPos();

	TPOINT2D   CadPos, StagePos;
	CadPos.x = CadPosX;
	CadPos.y = CadPosY;
	StagePos.x = StagePosX;
	StagePos.y = StagePosY;
	
	CAOIModel::MoveModel(-px, -py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(Angle, cx, cy);	}		
	CAOIModel::SetModelAttachedPosCad(CadPos);
	CAOIModel::SetModelAttachedPosStage(StagePos);		

	BoxPtr->GetBoxPos(px, py);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIModel::GetModelComponentPtr() const
{
	CAOIRgn *RgnPtr = CAOIModel::GetModelAttachedPtr();
	if ( NULL == RgnPtr ) { return NULL; }
	if ( RgnPtr->GetObjType() != AOI_OBJ_COMPONENT ) { return NULL; }
	return (CAOIComponent*)(RgnPtr);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelComponentPtr(CAOIComponent *Ptr)
{ 
	m_ModelAttachedPtr = Ptr; 
}	
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelBodyToComponent()//將模組的本體調整至零件內
{	
	CAOIComponent *pComponent = CAOIModel::GetModelComponentPtr();
	if ( NULL == pComponent ) { return true; }

	TREGION4D Region;
	TREGION4D RegionCad;
	TPOINT2D  CadOffset, StageOffset;
	double    cx=0, cy=0;
	double    px=0, py=0;
	double    sx=0, sy=0;
	double    sx2=0, sy2=0;
	double    Angle = pComponent->GetComponentAngle();
	double    CadPosX = pComponent->GetComponentCadPosX();
	double    CadPosY = pComponent->GetComponentCadPosY();
	double    StagePosX = pComponent->GetComponentStagePosX();
	double    StagePosY = pComponent->GetComponentStagePosY();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	CAOIBox *BoxPtr = CAOIModel::GetModelBodyBoxPtr();
	
	cx = cy = 0;
	BoxPtr->GetBoxPos(px, py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(-Angle, cx, cy);		}	
	//BoxPtr->GetBoxPos(px, py);
	BoxPtr->GetBoxSize(sx, sy);
	CAOIModel::GetModelTotalRegion(Region);
	const double BiasPosX = Region.GetCpX();
	const double BiasPosY = Region.GetCpY();	
	sx2 = Region.maxX-Region.minX;
	sy2 = Region.maxY-Region.minY;		

	CadOffset.x = px;
	CadOffset.y = py;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);

	CadPosX += CadOffset.x;
	CadPosY += CadOffset.y;

	StagePosX += StageOffset.x;
	StagePosY += StageOffset.y;	

	//旋轉零件的尺寸, 因為零件的寬長是以0度為基礎
	if ( false == IsExceptionAngle )
	{	
		JetAPI::RotateSize(Angle, sx, sy);	
		JetAPI::RotateSize(Angle, sx2, sy2);	
	}
	else
	{	JetAPI::RotatePos(-Angle, cx, cy, px, py);		}
		
	pComponent->SetComponentRoiSizeW(sx2);
	pComponent->SetComponentRoiSizeH(sy2);
	pComponent->SetComponentBodySizeW(sx);
	pComponent->SetComponentBodySizeH(sy);
	pComponent->SetComponentCadPosX(CadPosX);
	pComponent->SetComponentCadPosY(CadPosY);
	pComponent->SetComponentCadBiasPosX(BiasPosX);
	pComponent->SetComponentCadBiasPosY(BiasPosY);
	pComponent->SetComponentStagePosX(StagePosX);
	pComponent->SetComponentStagePosY(StagePosY);

	pComponent->CalcComponentCadCornerPos();
	pComponent->LayoutComponentStageCornerPos();

	TPOINT2D   CadPos, StagePos;
	CadPos.x = CadPosX;
	CadPos.y = CadPosY;
	StagePos.x = StagePosX;
	StagePos.y = StagePosY;
	
	CAOIModel::MoveModel(-px, -py);
	if ( true == IsExceptionAngle )
	{	CAOIModel::RotateModel(Angle, cx, cy);	}		
	CAOIModel::SetModelAttachedPosCad(CadPos);
	CAOIModel::SetModelAttachedPosStage(StagePos);		

	BoxPtr->GetBoxPos(px, py);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ReleaseModelUniFrameList()
{
	size_t i=0;
	const size_t UniFrameCount = m_ModelUniFrameList.size();
	for ( i=0; i<UniFrameCount; i++ )
	{
		if ( NULL != m_ModelUniFrameList[i].ImagePtr ) 
		{	JetMemory.free_func(m_ModelUniFrameList[i].ImagePtr); }

		if ( NULL != m_ModelUniFrameList[i].MaskPtr ) 
		{	JetMemory.free_func(m_ModelUniFrameList[i].MaskPtr); }

		if ( NULL != m_ModelUniFrameList[i].SpacePtr ) 
		{	JetMemory.free_func(m_ModelUniFrameList[i].SpacePtr); }

		m_ModelUniFrameList[i].ImageW = 1024;
		m_ModelUniFrameList[i].ImageH = 1024;
		m_ModelUniFrameList[i].ImageStep = 1024;
		m_ModelUniFrameList[i].BitCount = 8;
	}
	m_ModelUniFrameList.clear();
}
//-------------------------------------------------------------------------------------//
void CAOIModel::CloneModelUniFrameList(const CAOIModel &Model)
{
	CAOIModel::SetModelUniFrameList(Model.m_ModelUniFrameList, true);
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelUniFrameList(const std::vector<TUNI_FRAME> &UniFrameList, bool bClone)
{
	const char fnName[] = "CAOIModel::SetModelUniFrameList";
	CAOIModel::ReleaseModelUniFrameList();
	
	size_t i=0;
	size_t BufferSize=0;
	char   varName[64]="";
	TUNI_FRAME UniFrame;
	MASK_PTR   MaskPtr=NULL;
	MASK_PTR   MaskPtrNew=NULL;
	SPACE_PTR  SpacePtr=NULL;
	SPACE_PTR  SpacePtrNew=NULL;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_PTR  ImagePtrNew=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	IMAGE_SIZE ImageWOrg=0, ImageHOrg=0, ImageStepOrg=0, BitCountOrg=0;
	const size_t UniFrameCount = UniFrameList.size();

	for ( i=0; i<UniFrameCount; i++ )
	{
		ImageW    = UniFrameList[i].ImageW;
		ImageH    = UniFrameList[i].ImageH;
		ImageStep = UniFrameList[i].ImageStep;
		BitCount  = UniFrameList[i].BitCount;

		ImagePtr    = UniFrameList[i].ImagePtr;
		MaskPtr    = UniFrameList[i].MaskPtr;		
		SpacePtr    = UniFrameList[i].SpacePtr;
		BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);

		if ( true == bClone )
		{
			ImagePtrNew = NULL;
			MaskPtrNew = NULL;
			SpacePtrNew = NULL;

			if ( NULL != ImagePtr ) 
			{
				::sprintf(varName, "%s#%d", "ImagePtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, ImagePtrNew, fnName, varName) == false ) 
				{	
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				::memcpy(ImagePtrNew, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);			
			}

			if ( NULL != MaskPtr ) 
			{	
				::sprintf(varName, "%s#%d", "MaskPtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, MaskPtrNew, fnName, varName) == false ) 
				{
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				::memcpy(MaskPtrNew, MaskPtr, sizeof(MASK_DATA)*BufferSize);
			}

			if ( NULL != SpacePtr ) 
			{	
				::sprintf(varName, "%s#%d", "SpacePtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, SpacePtrNew, fnName, varName) == false ) 
				{
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				::memcpy(SpacePtrNew, SpacePtr, sizeof(SPACE_DATA)*BufferSize);
			}
		}
		else
		{
			ImagePtrNew = ImagePtr;
			MaskPtrNew = MaskPtr;
			SpacePtrNew = SpacePtr;
		}
		
		UniFrame.ImageW = ImageW;
		UniFrame.ImageH = ImageH;
		UniFrame.ImageStep = ImageStep;
		UniFrame.BitCount = BitCount;
		UniFrame.ImagePtr = ImagePtrNew;
		UniFrame.MaskPtr = MaskPtrNew;
		UniFrame.SpacePtr = SpacePtrNew;
		m_ModelUniFrameList.push_back(UniFrame);
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CopyModelUniFrameList(std::vector<TUNI_FRAME> &UniFrameList, bool bClone) const//複製模組的影像
{
	const char fnName[] = "CAOIModel::CopyModelUniFrameList";
	
	bool   IsOK=true;
	size_t i=0;
	size_t BufferSize=0;
	size_t UniFrameCount;
	char   varName[64]="";
	TUNI_FRAME UniFrame;
	MASK_PTR   MaskPtr=NULL;
	MASK_PTR   MaskPtrNew=NULL;
	SPACE_PTR  SpacePtr=NULL;
	SPACE_PTR  SpacePtrNew=NULL;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_PTR  ImagePtrNew=NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	IMAGE_SIZE ImageWOrg=0, ImageHOrg=0, ImageStepOrg=0, BitCountOrg=0;
	
	UniFrameCount = UniFrameList.size();
	for ( i=0; i<UniFrameCount; i++ )
	{
		JetMemory.free_func(UniFrameList[i].ImagePtr);
		JetMemory.free_func(UniFrameList[i].MaskPtr);
		JetMemory.free_func(UniFrameList[i].SpacePtr);
	}
	UniFrameList.clear();

	IsOK=true;
	UniFrameCount = m_ModelUniFrameList.size();
	for ( i=0; i<UniFrameCount; i++ )
	{
		ImageW    = m_ModelUniFrameList[i].ImageW;
		ImageH    = m_ModelUniFrameList[i].ImageH;
		ImageStep = m_ModelUniFrameList[i].ImageStep;
		BitCount  = m_ModelUniFrameList[i].BitCount;

		ImagePtr    = m_ModelUniFrameList[i].ImagePtr;
		MaskPtr    = m_ModelUniFrameList[i].MaskPtr;		
		SpacePtr    = m_ModelUniFrameList[i].SpacePtr;
		BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);

		if ( true == bClone )
		{
			ImagePtrNew = NULL;
			MaskPtrNew = NULL;
			SpacePtrNew = NULL;

			if ( NULL != ImagePtr ) 
			{
				::sprintf(varName, "%s#%d", "ImagePtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, ImagePtrNew, fnName, varName) == false ) 
				{	
					IsOK=false;
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				::memcpy(ImagePtrNew, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);			
			}

			if ( NULL != MaskPtr ) 
			{	
				::sprintf(varName, "%s#%d", "MaskPtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, MaskPtrNew, fnName, varName) == false ) 
				{
					IsOK=false;
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				::memcpy(MaskPtrNew, MaskPtr, sizeof(MASK_DATA)*BufferSize);
			}

			if ( NULL != SpacePtr ) 
			{	
				::sprintf(varName, "%s#%d", "SpacePtr", i+1);
				if ( JetMemory.alloc_func(BufferSize, SpacePtrNew, fnName, varName) == false ) 
				{
					IsOK=false;
					JetMemory.free_func(ImagePtrNew);
					JetMemory.free_func(MaskPtrNew);
					JetMemory.free_func(SpacePtrNew);
					break;
				}
				::memcpy(SpacePtrNew, SpacePtr, sizeof(SPACE_DATA)*BufferSize);
			}
		}
		else
		{
			ImagePtrNew = ImagePtr;
			MaskPtrNew = MaskPtr;
			SpacePtrNew = SpacePtr;
		}		
		
		UniFrame.ImageW = ImageW;
		UniFrame.ImageH = ImageH;
		UniFrame.ImageStep = ImageStep;
		UniFrame.BitCount = BitCount;
		UniFrame.ImagePtr = ImagePtrNew;
		UniFrame.MaskPtr = MaskPtrNew;
		UniFrame.SpacePtr = SpacePtrNew;
		UniFrameList.push_back(UniFrame);
	}

	if ( false == IsOK )
	{
		if ( true == bClone )
		{
			UniFrameCount = UniFrameList.size();		
			for ( i=0; i<UniFrameCount; i++ )
			{
				JetMemory.free_func(UniFrameList[i].ImagePtr);
				JetMemory.free_func(UniFrameList[i].MaskPtr);
				JetMemory.free_func(UniFrameList[i].SpacePtr);
			}
		}
		UniFrameList.clear();
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelDefectWndGroupIDList()
{
	size_t        i=0;
	CAOIWnd      *WndPtr = NULL;
	int           WndGroupID=0;
	RESULT_ID     WndResultID;
	WND_DEFECT_ID WndDefectID;	
	std::set<int> WndDefectIDSet;	
	int   DefectCountTest=0;	
	const size_t WndCount = GetModelWndCount();
	const CWndDefectItem &TestItem = GetModelDefectItemTest();	

	ClearModelDefectWndUUIDList();
	ClearModelDefectWndGroupIDList();
	for ( i=0; i<WndCount; i++ )
	{		
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		WndResultID = WndPtr->GetWndResultID();
		WndResultID = WndPtr->GetWndLogicResultID();		
		if ( RESULT_ID_NONE == WndResultID ) { continue; }		
		if ( RESULT_ID_SKIP == WndResultID ) { continue; }
		if ( RESULT_ID_BYPASS == WndResultID ) { continue; }
		//if ( RESULT_ID_EXCEPTION == WndResultID ) { continue; }				
		
		WndDefectID = WndPtr->GetWndDefectID();
		DefectCountTest = TestItem.GetItemCount(WndDefectID);
		if ( WND_DEFECT_ITEM_DISABLE==DefectCountTest || WND_DEFECT_ITEM_NO_SHOW==DefectCountTest )
		{
			if ( RESULT_ID_NG==WndResultID || RESULT_ID_EXCEPTION==WndResultID )
			{
				WndResultID = RESULT_ID_BYPASS;
				WndPtr->SetWndResultID(WndResultID);
				WndPtr->GetWndAlgParam().SetAlgResultID(WndResultID);
			}
			continue;
		}
		WndGroupID = WndPtr->GetWndGroupID();		

		if ( RESULT_ID_OK != WndResultID )
		{	
			RESULT_ID ModelResultID=RESULT_ID_NG;
			if ( RESULT_ID_EXCEPTION == WndResultID )
			{	ModelResultID = WndResultID;	}

			if ( RESULT_ID_EXCEPTION != GetModelResultID() )
			{	SetModelResultID(ModelResultID);	}

			if ( WndPtr->GetWndDefectAlarm() == true )
			{	SetModelResultID_Alarm(RESULT_ID_NG);	}
		
			WndDefectIDSet.insert(WndGroupID);
			m_ModelDefectWndUUIDList.push_back(WndPtr->GetObjUuid());	
		}
	}

	//模組瑕疵檢測框群組列表
	std::set<int>::iterator iter;
	for ( iter=WndDefectIDSet.begin(); iter!=WndDefectIDSet.end(); iter++ )
	{	m_ModelDefectWndGroupIDList.push_back(*iter);	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelDefectWndGroupIDList()
{
	m_ModelDefectWndGroupIDList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CAOIModel::GetModelDefectWndGroupIDCount() const
{
	return m_ModelDefectWndGroupIDList.size();
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelDefectWndGroupID(size_t idx, bool Check) const
{
	if ( true == Check )
	{		
		const size_t Count = m_ModelDefectWndUUIDList.size();
		if ( idx >= Count ) 
		{	return -1;	}
	}
	return m_ModelDefectWndGroupIDList[idx];
	
}
//-------------------------------------------------------------------------------------//
void CAOIModel::ClearModelDefectWndUUIDList()
{
	m_ModelDefectWndUUIDList.clear();
}
//-------------------------------------------------------------------------------------//
size_t  CAOIModel::GetModelDefectWndUUIDCount() const
{
	return m_ModelDefectWndUUIDList.size();
}
//-------------------------------------------------------------------------------------//
UUID CAOIModel::GetModelDefectWndUUID(size_t idx, bool Check) const
{
	if ( true == Check )
	{		
		const size_t Count = m_ModelDefectWndUUIDList.size();
		if ( idx >= Count ) 
		{
			UUID TempUUID;
			JetAPI::InitialUUID(TempUUID);
			return TempUUID; 
		}
	}
	return m_ModelDefectWndUUIDList[idx];
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SynchronousModel(const CAOIModel *RefModelPtr, bool bPartial)//同步化模組, 需要相同框數量下
{
	if ( NULL == RefModelPtr ) { return false; }
	bool IsOK = true;
	const double RefComponentAngle = RefModelPtr->GetModelAttachedAngle();
	const double CurComponentAngle = CAOIModel::GetModelAttachedAngle();
	const double BetweenComponentAnlge = CurComponentAngle-RefComponentAngle;
	if ( ::fabs(BetweenComponentAnlge)<0.001 || ::fabs(BetweenComponentAnlge-360.0)<0.001 )
	{	IsOK = SynchronousModelKernel(RefModelPtr, bPartial);	}
	else
	{
		CAOIModel::RotateModel(-BetweenComponentAnlge, 0, 0);
		IsOK = SynchronousModelKernel(RefModelPtr, bPartial);
		CAOIModel::RotateModel(BetweenComponentAnlge, 0, 0);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SynchronousModelKernel(const CAOIModel *RefModelPtr, bool bPartial)//同步化模組, 需要相同框數量下
{	
	//確認數量
	const size_t WndCount = GetModelWndCount();
	const size_t LandCount = GetModelLandCount();
	const size_t LogicCount = GetModelLogicCount_Inline();
	const size_t RefWndCount = RefModelPtr->GetModelWndCount();
	const size_t RefLandCount = RefModelPtr->GetModelLandCount();
	const size_t RefLogicCount = RefModelPtr->GetModelLogicCount_Inline();
	if ( RefWndCount != WndCount || RefLandCount != LandCount || RefLogicCount != LogicCount )
	{	return false;	}

	//確認角度
	const double RefComponentAngle = RefModelPtr->GetModelAttachedAngle();
	const double CurComponentAngle = CAOIModel::GetModelAttachedAngle();
	if ( ::fabs(RefComponentAngle-CurComponentAngle) > 0.001 ) 
	{	return false; }

	size_t     i=0, j=0, k=0;
	CAOIWnd   *WndPtr = NULL;
	CAOIWnd   *RefWndPtr = NULL;
	CAOILand  *LandPtr = NULL;
	CAOILand  *RefLandPtr = NULL;
	CAOILogic *LogicPtr = NULL;
	CAOILogic *RefLogicPtr = NULL;

	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = GetModelLandPtr(i, false);
		RefLandPtr = RefModelPtr->GetModelLandPtr(i, false);
		if ( NULL==LandPtr || NULL==RefLandPtr ) { continue; }
		if ( true == bPartial )
		{	
			if ( RefLandPtr->GetLandModified() == false ) 
			{	continue; }
		}
		LandPtr->SynchronousLand(RefLandPtr);
		LandPtr->SetLandModified(false);
	}

	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);		
		RefWndPtr = RefModelPtr->GetModelWndPtr(i, false);
		if ( NULL==WndPtr || NULL==RefWndPtr ) { continue; }
		if ( true == bPartial )
		{	
			if ( RefWndPtr->GetWndModified() == false ) 
			{	continue; }
		}
		WndPtr->SynchronousWnd(RefWndPtr);
		WndPtr->SetWndModified(false);
	}

	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = GetModelLogicPtr_Inline(i);
		RefLogicPtr = RefModelPtr->GetModelLogicPtr_Inline(i);
		if ( NULL==LogicPtr || NULL==RefLogicPtr ) { continue; }
		if ( true == bPartial )
		{	
			if ( RefLogicPtr->GetLogicModified() == false ) 
			{	continue; }
		}
		LogicPtr->SynchronousLogic(RefLogicPtr);
		LogicPtr->SetLogicModified(false);
	}

	m_ModelIndex = RefModelPtr->m_ModelIndex;
	m_ModelType = RefModelPtr->m_ModelType;
	m_ModelName = RefModelPtr->m_ModelName;
	m_ModelGroupName=RefModelPtr->m_ModelGroupName;
	m_ModelChipSizeMode = RefModelPtr->m_ModelChipSizeMode;
	m_ModelLandDirection = RefModelPtr->m_ModelLandDirection;
	m_ModelPadAdjustMode = RefModelPtr->m_ModelPadAdjustMode;
	m_ModelLeadAdjustMode = RefModelPtr->m_ModelLeadAdjustMode;
	
	//m_ModelClassID = RefModelPtr->m_ModelClassID;
	//m_ModelActClassID = RefModelPtr->m_ModelActClassID;	
	m_ModelSelected = RefModelPtr->m_ModelSelected;
	m_ModelIsolated = RefModelPtr->m_ModelIsolated;
	//m_ModelSelfTest = RefModelPtr->m_ModelSelfTest;
	m_ModelSaveLeadReport = RefModelPtr->m_ModelSaveLeadReport;
	m_ModelBodyLinkChipLead = RefModelPtr->m_ModelBodyLinkChipLead;
	m_ModelTempInt[0] = RefModelPtr->m_ModelTempInt[0];
	m_ModelTempInt[1] = RefModelPtr->m_ModelTempInt[1];
	m_ModelTempInt[2] = RefModelPtr->m_ModelTempInt[2];
	m_ModelTempInt[3] = RefModelPtr->m_ModelTempInt[3];
	m_ModelModifiedDateTime = RefModelPtr->m_ModelModifiedDateTime;
	SetModelBKImageNeedToGrab(RefModelPtr->GetModelBKImageNeedToGrab());

	m_ModelExtendAutoAdjust = RefModelPtr->m_ModelExtendAutoAdjust;
	//m_ModelImageScale = RefModelPtr->m_ModelImageScale;	
	//m_ModelModifiedCount = RefModelPtr->m_ModelModifiedCount;	
	//m_ModelAutoDeleteImageFolder = RefModelPtr->m_ModelAutoDeleteImageFolder;
	//m_ModelExtendRange = RefModelPtr->m_ModelExtendRange;
	//m_ModelTotalRgn = RefModelPtr->m_ModelTotalRgn;
	//UpdateModelTotalRegionToAttached();	
	//---------------------------------------------------------------------------------//
	m_ModelBodyBox.SynchronousBox(&(RefModelPtr->m_ModelBodyBox));
	m_ModelFolderModel = RefModelPtr->m_ModelFolderModel;	
	m_ModelBodySizeX = RefModelPtr->m_ModelBodySizeX;
	m_ModelBodySizeY = RefModelPtr->m_ModelBodySizeY;
	m_ModelBodyHeight = RefModelPtr->m_ModelBodyHeight;
	m_ModelBodyColorGroup = RefModelPtr->m_ModelBodyColorGroup;
	//m_ModelFolderComponent = RefModelPtr->m_ModelFolderComponent;			
	m_ModelModifiedCount = false;
	m_ModelNeedSaveFiles = true; 
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelInspectionParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TPOINT2D &RgnCp, TPOINT2D &ImageCp, TPOINT2D &Scale, bool bRawRgn) const//計算模組檢測參數
{
	TREGION4D ModelRgn;
	GetModelTotalRegion(ModelRgn, bRawRgn);
	const double RegionW = ModelRgn.GetWidth();
	const double RegionH = ModelRgn.GetHeight();	
	if ( ::fabs(RegionW)<1 || ::fabs(RegionH)<1 )
	{	return false;	}		
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();
	Scale.x = ImageW;
	Scale.y = ImageH;
	Scale.x = Scale.x/RegionW;
	Scale.y = Scale.y/RegionH;
	ImageCp.x = ImageW;
	ImageCp.y = ImageH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelInspectionImageRect_Raw(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TPOINT2D &RgnCp, const TPOINT2D &ImageCp, const TPOINT2D &Scale)//計算模組檢測影像位置
{
	RECT         BoxRect;
	TREGION4D    BodyRgn;
	TPOINT2D     CornerPos[4];
	const CAOIBox &BodyBox=GetModelBodyBox();

	//BodyBox.GetBoxCornerPos(CornerPos);
	BodyBox.GetBoxCornerPosRes(CornerPos);
	JetAPI::CornerPtToRegion(CornerPos, BodyRgn);	
	CalcModelBoxRegionRect(BodyRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, BoxRect);
	SetModelBodyImageRect_Raw(BoxRect);	

	CAOIWnd *WndPtr=NULL;
	const size_t WndCount=GetModelWndCount();
	for ( size_t i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		const CAOIBox &BoxRef=WndPtr->GetWndBox();
		//BoxRef.GetBoxCornerPos(CornerPos);
		BoxRef.GetBoxCornerPosRes(CornerPos);
		JetAPI::CornerPtToRegion(CornerPos, BodyRgn);	
		CalcModelBoxRegionRect(BodyRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, BoxRect);
		WndPtr->SetWndImageRect_Raw(BoxRect);
		if ( WndPtr->GetWndExtendBoxUsed() == true )
		{
			const CAOIBox &ExtendBoxRef=WndPtr->GetWndExtendBox();
			ExtendBoxRef.GetBoxCornerPosRes(CornerPos);
			JetAPI::CornerPtToRegion(CornerPos, BodyRgn);	
			CalcModelBoxRegionRect(BodyRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, BoxRect);
		}
		WndPtr->SetWndExtendImageRect_Raw(BoxRect);			
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelInspectionPosRes(CAOIWnd *RefWndPtr, double OffsetX, double OffsetY, double Skew, bool ApplySkew)//更新模組檢測的位置結果
{
	bool bSucc = true;
	bool  bRotated = AOIDataCollect.GetSystemParameter().m_WndRotationFollowed;
	if (false == bRotated) {bSucc = UpdateModelInspectionPosRes_v1(RefWndPtr, OffsetX, OffsetY, Skew, ApplySkew); }
	else { bSucc = UpdateModelInspectionPosRes_v2(RefWndPtr, OffsetX, OffsetY, Skew, ApplySkew);}
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelInspectionPosRes_v1(CAOIWnd *RefWndPtr, double OffsetX, double OffsetY, double Skew, bool ApplySkew)//更新模組檢測的位置結果
{	
	if ( NULL == RefWndPtr ) { return false; }

	size_t    i=0;	
	bool      bFollow = false;
	int       LandGroupID = -1;	
	double    SkewPosX=0.0, SkewPosY=0.0;//因角度變化而產生的位移差
	TPOINT2D  RefBoxPos, BoxPos, BoxOffset, BoxNewPos, BoxOffset2, BoxOffset3;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	CAOIBox  *RefBoxPtr = RefWndPtr->GetWndBoxPtr();
	CAOILand *RefLandPtr = RefWndPtr->GetWndLandPtr();	
	WND_DEFECT_ID WndDefectID = RefWndPtr->GetWndDefectID();
	WND_FOLLOW_MODE WndFollowMode = RefWndPtr->GetWndFollowMode();
	const size_t RefWndOrderIndex = RefWndPtr->GetWndOrderIndex();
	const size_t StartWndOrderIndex = RefWndOrderIndex+1;
	const double CadOffsetX2 = OffsetX;
	const double CadOffsetY2 = OffsetY;
	const double CadSkew2 = Skew;
	const size_t ModelLandCount = GetModelLandCount();
	const size_t ModelWndOrderCount = GetModelWndOrderCount();
	const double COS = cos(Skew*DEG_TO_RAD_DBL);
	const double SIN = sin(Skew*DEG_TO_RAD_DBL);
	bool  ApplySkewAngle = false;

	if ( true == ApplySkew )
	{
		if ( fabs(CadSkew2)>0.001 )
		{	ApplySkewAngle = true;	}
	}
	RefBoxPtr->GetBoxPosRes(RefBoxPos);

	if ( NULL != RefLandPtr ) 
	{	
		LandGroupID = RefLandPtr->GetLandGroupID(); 
		const bool LeadFollowPad = RefLandPtr->CheckLandLeadFollowPad();
		if ( WND_DEFECT_PAD_ALIGN == WndDefectID )//焊盤定位
		{
			if ( true == LeadFollowPad )
			{	RefLandPtr->MoveLandResult(CadOffsetX2, CadOffsetY2);	}
			else//只移動Pad
			{	RefLandPtr->MoveLandPadResult(CadOffsetX2, CadOffsetY2); }
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr != RefLandPtr ) { continue; }				
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();
				if ( true==LeadFollowPad )
				{
					if ( WND_FOLLOW_NONE == WndFollowMode )
					{	continue; }
					bFollow = true;
				}
				else
				{
					if ( WND_FOLLOW_PAD==WndFollowMode || WND_FOLLOW_PAD_BODY==WndFollowMode )
					{	bFollow = true; }
					else
					{	continue; }
				}
				WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2);				
			}
		}
		else if ( WND_DEFECT_PAD_ADJUST == WndDefectID )//焊盤定位
		{
			if ( true == LeadFollowPad )
			{	RefLandPtr->MoveLandResult(CadOffsetX2, CadOffsetY2); }
			else//只移動Pad
			{	RefLandPtr->MoveLandPadResult(CadOffsetX2, CadOffsetY2);	}
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr != RefLandPtr ) { continue; }
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();
				if ( true==LeadFollowPad )
				{
					if ( WND_FOLLOW_NONE == WndFollowMode )
					{	continue; }
					bFollow = true;
				}
				else
				{
					if ( WND_FOLLOW_PAD==WndFollowMode || WND_FOLLOW_PAD_LEAD==WndFollowMode )
					{	bFollow = true; }
					else
					{	continue; }
				}
				WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2);				
			}
		}
		else if ( WND_DEFECT_PART_ALIGN==WndDefectID )//本體定位
		{
			RefLandPtr->MoveLandLeadResult(CadOffsetX2, CadOffsetY2);			
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr != RefLandPtr ) { continue; }
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();
				if ( WND_FOLLOW_PART==WndFollowMode || WND_FOLLOW_PART_BODY==WndFollowMode )
				{	bFollow = true; }
				else
				{	continue; }
				WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2);				
			}
		}
		else if ( WND_DEFECT_LEAD_ADJUST==WndDefectID )//引腳定位
		{
			RefLandPtr->MoveLandLeadResult(CadOffsetX2, CadOffsetY2);			
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr != RefLandPtr ) { continue; }
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();				
				if ( WND_FOLLOW_PART==WndFollowMode || WND_FOLLOW_PART_LEAD==WndFollowMode )
				{	bFollow = true; }
				else
				{	continue; }
				WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2);				
			}
		}
	}
	else
	{
		if ( WND_DEFECT_PAD_ALIGN==WndDefectID || WND_DEFECT_PAD_ADJUST==WndDefectID )//焊盤定位
		{
			BoxPtr = GetModelBodyBoxPtr();
			if ( false == ApplySkewAngle )
			{	BoxPtr->MoveBoxRes(CadOffsetX2, CadOffsetY2); }
			else
			{
				BoxPtr->GetBoxPosRes(BoxPos);
				BoxOffset.x = BoxPos.x-RefBoxPos.x;
				BoxOffset.y = BoxPos.y-RefBoxPos.y;
				BoxNewPos.x = (COS*BoxOffset.x)-(SIN*BoxOffset.y);
				BoxNewPos.y = (SIN*BoxOffset.x)+(COS*BoxOffset.y);
				BoxOffset2.x = BoxNewPos.x-BoxOffset.x;
				BoxOffset2.y = BoxNewPos.y-BoxOffset.y;
				BoxOffset3.x = BoxOffset2.x+CadOffsetX2;
				BoxOffset3.y = BoxOffset2.y+CadOffsetY2;
				BoxPtr->MoveBoxRes(BoxOffset3);
				BoxPtr->SkewBoxAngle(CadSkew2);
			}

			for ( i=0; i<ModelLandCount; i++ )
			{
				LandPtr = GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( false == ApplySkewAngle )
				{	LandPtr->MoveLandResult(CadOffsetX2, CadOffsetY2);	}
				else
				{
					BoxPtr = LandPtr->GetLandBoxPtr();
					BoxPtr->GetBoxPosRes(BoxPos);
					BoxOffset.x = BoxPos.x-RefBoxPos.x;
					BoxOffset.y = BoxPos.y-RefBoxPos.y;
					BoxNewPos.x = (COS*BoxOffset.x)-(SIN*BoxOffset.y);
					BoxNewPos.y = (SIN*BoxOffset.x)+(COS*BoxOffset.y);
					BoxOffset2.x = BoxNewPos.x-BoxOffset.x;
					BoxOffset2.y = BoxNewPos.y-BoxOffset.y;
					BoxOffset3.x = BoxOffset2.x+CadOffsetX2;
					BoxOffset3.y = BoxOffset2.y+CadOffsetY2;
					LandPtr->MoveLandResult(BoxOffset3.x, BoxOffset3.y);
				}				
			}

			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }				
				WndFollowMode = WndPtr->GetWndFollowMode();
				if ( WND_FOLLOW_NONE == WndFollowMode ) { continue; }
				if ( false == ApplySkewAngle )
				{	WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2); }
				else
				{
					BoxPtr = WndPtr->GetWndBoxPtr();
					BoxPtr->GetBoxPosRes(BoxPos);
					BoxOffset.x = BoxPos.x-RefBoxPos.x;
					BoxOffset.y = BoxPos.y-RefBoxPos.y;
					BoxNewPos.x = (COS*BoxOffset.x)-(SIN*BoxOffset.y);
					BoxNewPos.y = (SIN*BoxOffset.x)+(COS*BoxOffset.y);
					BoxOffset2.x = BoxNewPos.x-BoxOffset.x;
					BoxOffset2.y = BoxNewPos.y-BoxOffset.y;
					BoxOffset3.x = BoxOffset2.x+CadOffsetX2;
					BoxOffset3.y = BoxOffset2.y+CadOffsetY2;
					WndPtr->MoveWndResult(BoxOffset3);
				}
			}
		}
		else if ( WND_DEFECT_PART_ALIGN==WndDefectID || WND_DEFECT_LEAD_ADJUST==WndDefectID)//本體定位
		{
			BoxPtr = (CAOIBox*)(GetModelBodyBoxPtr());
			if ( false == ApplySkewAngle )
			{	BoxPtr->MoveBoxRes(CadOffsetX2, CadOffsetY2); }
			else
			{
				BoxPtr->GetBoxPosRes(BoxPos);
				BoxOffset.x = BoxPos.x-RefBoxPos.x;
				BoxOffset.y = BoxPos.y-RefBoxPos.y;
				BoxNewPos.x = (COS*BoxOffset.x)-(SIN*BoxOffset.y);
				BoxNewPos.y = (SIN*BoxOffset.x)+(COS*BoxOffset.y);
				BoxOffset2.x = BoxNewPos.x-BoxOffset.x;
				BoxOffset2.y = BoxNewPos.y-BoxOffset.y;
				BoxOffset3.x = BoxOffset2.x+CadOffsetX2;
				BoxOffset3.y = BoxOffset2.y+CadOffsetY2;
				BoxPtr->MoveBoxRes(BoxOffset3);
				BoxPtr->SkewBoxAngle(CadSkew2);
			}
			for ( i=0; i<ModelLandCount; i++ )
			{
				LandPtr = GetModelLandPtr(i, false);
				if ( NULL == LandPtr ) { continue; }
				if ( false == ApplySkewAngle )
				{	LandPtr->MoveLandLeadResult(CadOffsetX2, CadOffsetY2); }
				else
				{
					BoxPtr = LandPtr->GetLandLeadBoxPtr();
					BoxPtr->GetBoxPosRes(BoxPos);
					BoxOffset.x = BoxPos.x-RefBoxPos.x;
					BoxOffset.y = BoxPos.y-RefBoxPos.y;
					BoxNewPos.x = (COS*BoxOffset.x)-(SIN*BoxOffset.y);
					BoxNewPos.y = (SIN*BoxOffset.x)+(COS*BoxOffset.y);
					BoxOffset2.x = BoxNewPos.x-BoxOffset.x;
					BoxOffset2.y = BoxNewPos.y-BoxOffset.y;
					BoxOffset3.x = BoxOffset2.x+CadOffsetX2;
					BoxOffset3.y = BoxOffset2.y+CadOffsetY2;
					LandPtr->MoveLandLeadResult(BoxOffset3.x, BoxOffset3.y);
				}
			}
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();
				if ( WND_FOLLOW_PART==WndFollowMode || WND_FOLLOW_PART_BODY==WndFollowMode || WND_FOLLOW_PART_LEAD==WndFollowMode )
				{	bFollow = true;}
				else
				{	continue; }
				if ( false == ApplySkewAngle )
				{	WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2); }
				else
				{
					BoxPtr = WndPtr->GetWndBoxPtr();
					BoxPtr->GetBoxPosRes(BoxPos);
					BoxOffset.x = BoxPos.x-RefBoxPos.x;
					BoxOffset.y = BoxPos.y-RefBoxPos.y;
					BoxNewPos.x = (COS*BoxOffset.x)-(SIN*BoxOffset.y);
					BoxNewPos.y = (SIN*BoxOffset.x)+(COS*BoxOffset.y);
					BoxOffset2.x = BoxNewPos.x-BoxOffset.x;
					BoxOffset2.y = BoxNewPos.y-BoxOffset.y;
					BoxOffset3.x = BoxOffset2.x+CadOffsetX2;
					BoxOffset3.y = BoxOffset2.y+CadOffsetY2;
					WndPtr->MoveWndResult(BoxOffset3);
				}						
			}
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelInspectionPosRes_v2(CAOIWnd *RefWndPtr, double OffsetX, double OffsetY, double Skew, bool ApplySkew)//更新模組檢測的位置結果
{	
	if ( NULL == RefWndPtr ) { return false; }

	size_t    i = 0, j = 0;
	bool      bFollow = false;
	int       LandGroupID = -1;	
	double    SkewPosX=0.0, SkewPosY=0.0;//因角度變化而產生的位移差
	TPOINT2D  RefBoxPos, BoxPos, BoxOffset, BoxNewPos, BoxOffset2, BoxOffset3;
	CAOIBox  *BoxPtr = NULL;
	CAOIWnd  *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	CAOIBox  *RefBoxPtr = RefWndPtr->GetWndBoxPtr();
	CAOILand *RefLandPtr = RefWndPtr->GetWndLandPtr();	
	WND_DEFECT_ID WndDefectID = RefWndPtr->GetWndDefectID();
	WND_FOLLOW_MODE WndFollowMode = RefWndPtr->GetWndFollowMode();
	const size_t RefWndOrderIndex = RefWndPtr->GetWndOrderIndex();
	const size_t StartWndOrderIndex = RefWndOrderIndex+1;
	const double CadOffsetX2 = OffsetX;
	const double CadOffsetY2 = OffsetY;
	const double CadSkew2 = Skew;
	const size_t ModelLandCount = GetModelLandCount();
	const size_t ModelWndOrderCount = GetModelWndOrderCount();
	const double COS = cos(Skew*DEG_TO_RAD_DBL);
	const double SIN = sin(Skew*DEG_TO_RAD_DBL);
	bool  ApplySkewAngle = false;

	if ( true == ApplySkew )
	{
		if ( fabs(CadSkew2)>0.001 )
		{	ApplySkewAngle = true;	}
	}
	RefBoxPtr->GetBoxPosRes(RefBoxPos);
	double RefX = RefBoxPos.x - OffsetX;
	double RefY = RefBoxPos.y - OffsetY;

	if ( NULL != RefLandPtr ) 
	{	
		LandGroupID = RefLandPtr->GetLandGroupID(); 
		const bool LeadFollowPad = RefLandPtr->CheckLandLeadFollowPad();
		if ( WND_DEFECT_PAD_ALIGN == WndDefectID )//焊盤定位
		{
			if ( true == LeadFollowPad )
			{	RefLandPtr->MoveLandResult(CadOffsetX2, CadOffsetY2);	}
			else//只移動Pad
			{	RefLandPtr->MoveLandPadResult(CadOffsetX2, CadOffsetY2); }
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr != RefLandPtr ) { continue; }				
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();
				if ( true==LeadFollowPad )
				{
					if ( WND_FOLLOW_NONE == WndFollowMode )
					{	continue; }
					bFollow = true;
				}
				else
				{
					if ( WND_FOLLOW_PAD==WndFollowMode || WND_FOLLOW_PAD_BODY==WndFollowMode )
					{	bFollow = true; }
					else
					{	continue; }
				}
				WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2);				
			}
		}
		else if ( WND_DEFECT_PAD_ADJUST == WndDefectID )//焊盤定位
		{
			if ( true == LeadFollowPad )
			{	RefLandPtr->MoveLandResult(CadOffsetX2, CadOffsetY2); }
			else//只移動Pad
			{	RefLandPtr->MoveLandPadResult(CadOffsetX2, CadOffsetY2);	}
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr != RefLandPtr ) { continue; }
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();
				if ( true==LeadFollowPad )
				{
					if ( WND_FOLLOW_NONE == WndFollowMode )
					{	continue; }
					bFollow = true;
				}
				else
				{
					if ( WND_FOLLOW_PAD==WndFollowMode || WND_FOLLOW_PAD_LEAD==WndFollowMode )
					{	bFollow = true; }
					else
					{	continue; }
				}
				WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2);	
				if (WndPtr->GetWndDefectID() == WND_DEFECT_LEAD_ADJUST) {
					RefLandPtr->MoveLandLeadResult(CadOffsetX2, CadOffsetY2);
				}
			}
		}
		else if ( WND_DEFECT_PART_ALIGN==WndDefectID )//本體定位
		{
			RefLandPtr->MoveLandLeadResult(CadOffsetX2, CadOffsetY2);			
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr != RefLandPtr ) { continue; }
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();
				if ( WND_FOLLOW_PART==WndFollowMode || WND_FOLLOW_PART_BODY==WndFollowMode )
				{	bFollow = true; }
				else
				{	continue; }
				WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2);				
			}
		}
		else if ( WND_DEFECT_LEAD_ADJUST==WndDefectID )//引腳定位
		{
			RefLandPtr->MoveLandLeadResult(CadOffsetX2, CadOffsetY2);			
			for ( i=StartWndOrderIndex; i<ModelWndOrderCount; i++ )
			{
				WndPtr = GetModelWndOrderPtr(i, false);
				if ( NULL == WndPtr ) { continue; }
				LandPtr = WndPtr->GetWndLandPtr();
				if ( NULL == LandPtr ) { continue; }
				if ( LandPtr != RefLandPtr ) { continue; }
				bFollow = false;
				WndFollowMode = WndPtr->GetWndFollowMode();				
				if ( WND_FOLLOW_PART==WndFollowMode || WND_FOLLOW_PART_LEAD==WndFollowMode )
				{	bFollow = true; }
				else
				{	continue; }
				WndPtr->MoveWndResult(CadOffsetX2, CadOffsetY2);				
			}
		}
	}
	else
	{
		if (WND_DEFECT_PAD_ALIGN == WndDefectID || WND_DEFECT_PAD_ADJUST == WndDefectID)//焊盤定位
		{
			std::vector<CAOIBox*> BoxPtrArray;
			BoxPtrArray.push_back(GetModelBodyBoxPtr());
			bool WND_DEFECT_PAD_ADJUST_FOLLOWED = true;
			bool WND_DEFECT_LEAD_ADJUST_FOLLOWED = true;
			for (i = StartWndOrderIndex; i < ModelWndOrderCount; i++) {
				WndPtr = GetModelWndOrderPtr(i, false);
				if (NULL == WndPtr) { continue; }
				WndFollowMode = WndPtr->GetWndFollowMode();
				if (WND_FOLLOW_NONE == WndFollowMode) {
					if (WndPtr->GetWndDefectID() == WND_DEFECT_PAD_ADJUST) {
						WND_DEFECT_PAD_ADJUST_FOLLOWED = false;
					}
					else if (WndPtr->GetWndDefectID() == WND_DEFECT_LEAD_ADJUST) {
						WND_DEFECT_LEAD_ADJUST_FOLLOWED = false;
					}
					continue;
				}
				//if (WndPtr->GetWndDefectID() != WND_DEFECT_PAD_ALIGN) {
				//	BoxPtrArray.push_back(WndPtr->GetWndBoxPtr());
				//}
				BoxPtrArray.push_back(WndPtr->GetWndBoxPtr());
				if (true == WndPtr->GetWndExtendBoxUsed()) {
					BoxPtrArray.push_back(WndPtr->GetWndExtendBoxPtr());
				}
				{
					CAOIWndRoi  *WndRoiPtr = NULL;
					const size_t WndRoiCount = WndPtr->GetWndRoiWndCount();
					for (j = 0; j < WndRoiCount; j++)
					{
						WndRoiPtr = WndPtr->GetWndRoiWndPtr(j, false);
						if (NULL == WndRoiPtr) { continue; }
						BoxPtrArray.push_back(WndRoiPtr->GetWndRoiBoxPtr());
					}
				}
				{
					CAOIWndMask *MaskWndPtr = NULL;
					const size_t MaskWndCount = WndPtr->GetWndMaskWndCount();
					for (j = 0; j<MaskWndCount; j++)
					{
						MaskWndPtr = WndPtr->GetWndMaskWndPtr(j, false);
						if (NULL == MaskWndPtr) { continue; }
						BoxPtrArray.push_back(MaskWndPtr->GetWndMaskBoxPtr());
					}
				}
			}
			for (i = 0; i < ModelLandCount; i++) {
				LandPtr = GetModelLandPtr(i, false);
				if (NULL == LandPtr) { continue; }
				if (WND_DEFECT_PAD_ADJUST_FOLLOWED)
					BoxPtrArray.push_back(LandPtr->GetLandPadBoxPtr());
				if (WND_DEFECT_LEAD_ADJUST_FOLLOWED) {
					BoxPtrArray.push_back(LandPtr->GetLandLeadBoxPtr());
					BoxPtrArray.push_back(LandPtr->GetLandLeadTipBoxPtr());
					BoxPtrArray.push_back(LandPtr->GetLandLeadShoulderBoxPtr());
				}
				BoxPtrArray.push_back(LandPtr->GetLandBodyEdgeBoxPtr());
			}
			for (i = 0; i < BoxPtrArray.size(); i++) {
				BoxPtr = BoxPtrArray[i];
				if (false == ApplySkewAngle) {
					BoxPtr->MoveBoxRes(CadOffsetX2, CadOffsetY2);
				}
				else {
					BoxPtr->GetBoxPosRes(BoxPos);
					BoxOffset.x = BoxPos.x - RefX;
					BoxOffset.y = BoxPos.y - RefY;
					BoxNewPos.x = (COS*BoxOffset.x) - (SIN*BoxOffset.y);
					BoxNewPos.y = (SIN*BoxOffset.x) + (COS*BoxOffset.y);
					BoxOffset2.x = BoxNewPos.x - BoxOffset.x;
					BoxOffset2.y = BoxNewPos.y - BoxOffset.y;
					BoxOffset3.x = BoxOffset2.x + CadOffsetX2;
					BoxOffset3.y = BoxOffset2.y + CadOffsetY2;
					BoxPtr->MoveBoxRes(BoxOffset3);
					BoxPtr->SkewBoxAngle(CadSkew2);
				}
			}
		}
		else if (WND_DEFECT_PART_ALIGN == WndDefectID || WND_DEFECT_LEAD_ADJUST == WndDefectID)//本體定位
		{
			std::vector<CAOIBox*> BoxPtrArray;
			BoxPtrArray.push_back(GetModelBodyBoxPtr());
			bool WND_DEFECT_LEAD_ADJUST_FOLLOWED = true;
			for (i = StartWndOrderIndex; i < ModelWndOrderCount; i++) {
				WndPtr = GetModelWndOrderPtr(i, false);
				if (NULL == WndPtr) { continue; }
				WndFollowMode = WndPtr->GetWndFollowMode();
				if (WND_FOLLOW_PART == WndFollowMode || WND_FOLLOW_PART_BODY == WndFollowMode || WND_FOLLOW_PART_LEAD == WndFollowMode)
				{
					bFollow = true;
				}
				else
				{
					if (WndPtr->GetWndDefectID() == WND_DEFECT_LEAD_ADJUST) {
						WND_DEFECT_LEAD_ADJUST_FOLLOWED = false;
					}
					continue;
				}
				BoxPtrArray.push_back(WndPtr->GetWndBoxPtr());
				if (true == WndPtr->GetWndExtendBoxUsed()) {
					BoxPtrArray.push_back(WndPtr->GetWndExtendBoxPtr());
				}
				{
					CAOIWndRoi  *WndRoiPtr = NULL;
					const size_t WndRoiCount = WndPtr->GetWndRoiWndCount();
					for (j = 0; j < WndRoiCount; j++)
					{
						WndRoiPtr = WndPtr->GetWndRoiWndPtr(j, false);
						if (NULL == WndRoiPtr) { continue; }
						BoxPtrArray.push_back(WndRoiPtr->GetWndRoiBoxPtr());
					}
				}
				{
					CAOIWndMask *MaskWndPtr = NULL;
					const size_t MaskWndCount = WndPtr->GetWndMaskWndCount();
					for (j = 0; j<MaskWndCount; j++)
					{
						MaskWndPtr = WndPtr->GetWndMaskWndPtr(j, false);
						if (NULL == MaskWndPtr) { continue; }
						BoxPtrArray.push_back(MaskWndPtr->GetWndMaskBoxPtr());
					}
				}
			}

			for (i = 0; i < ModelLandCount; i++) {
				LandPtr = GetModelLandPtr(i, false);
				if (NULL == LandPtr) { continue; }
				if (false == WND_DEFECT_LEAD_ADJUST_FOLLOWED) { continue; }
				BoxPtrArray.push_back(LandPtr->GetLandLeadBoxPtr());
				BoxPtrArray.push_back(LandPtr->GetLandLeadTipBoxPtr());
				BoxPtrArray.push_back(LandPtr->GetLandLeadShoulderBoxPtr());

			}

			for (i = 0; i < BoxPtrArray.size(); i++) {
				BoxPtr = BoxPtrArray[i];
				if (RefWndPtr->GetWndFollowMode() == WND_FOLLOW_NONE) {
					BoxPtr->GetBoxPos(BoxPos);
					BoxPtr->SetBoxPosRes(BoxPos);
					double Angle = BoxPtr->GetBoxAngle();
					BoxPtr->SetBoxAngleRes(Angle);
					BoxPtr->SetBoxAngleSkew(0);
				}
				if (false == ApplySkewAngle) {
					BoxPtr->MoveBoxRes(CadOffsetX2, CadOffsetY2);
				}
				else {
					BoxPtr->GetBoxPosRes(BoxPos);
					BoxOffset.x = BoxPos.x - RefX;
					BoxOffset.y = BoxPos.y - RefY;
					BoxNewPos.x = (COS*BoxOffset.x) - (SIN*BoxOffset.y);
					BoxNewPos.y = (SIN*BoxOffset.x) + (COS*BoxOffset.y);
					BoxOffset2.x = BoxNewPos.x - BoxOffset.x;
					BoxOffset2.y = BoxNewPos.y - BoxOffset.y;
					BoxOffset3.x = BoxOffset2.x + CadOffsetX2;
					BoxOffset3.y = BoxOffset2.y + CadOffsetY2;
					BoxPtr->MoveBoxRes(BoxOffset3);
					BoxPtr->SkewBoxAngle(CadSkew2);
				}
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CloneModelInfo(TModelInfo &ModelInfo)//複製模組訊息
{
	ModelInfo.sTempInt = 0;
	ModelInfo.sModelPtr = this;
	ModelInfo.sModelName = GetModelName();
	ModelInfo.sModifiedTime = GetModelModifiedDateTime();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::WriteModelFile(CAOIFileIO &FileIO)//儲存模組檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	//----------------------------------------------------------------------------------------//
	size_t   i=0;
	CAOIBox *BoxPtr = NULL;
	CAOIModel *pModel = this;
	char       uuidStr[MAX_JET_PATH]="";	
	wchar_t    uuidWStr[MAX_JET_PATH]=L"";	
	UUID       uuid = pModel->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIModel::WriteModelFile"));	
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	CString  ModelFolder;
	CString  ModelName = pModel->GetModelName();
	AOI_OBJ_TYPE AttachedType = pModel->GetModelAttachedType();
	switch ( AttachedType )
	{
	case AOI_OBJ_FD:
		ModelFolder.Format(_T("%s\\%s"), FileIO.GetFdFolder(), ModelName);
		break;
	case AOI_OBJ_COMPONENT:		
	default:
		ModelFolder.Format(_T("%s\\%s"), FileIO.GetLibraryFolder(), ModelName);
		break;
	}	
	pModel->SetModelFolderModel(ModelFolder);	
	//----------------------------------------------------------------------------------------//
	//模組參數
	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_TYPE, pModel->GetModelType()) == false ) { return false; }
	if ( FileIO.SaveChunk_STR(FILE_IO_MODEL_NAME, pModel->GetModelName()) == false ) { return false; }
	if ( FileIO.SaveChunk_STR(FILE_IO_MODEL_GROUP_NAME, pModel->GetModelGroupName()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_LAND_DIRECTION, pModel->GetModelLandDirection()) == false ) { return false; }
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_MODEL_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_MODEL_OBJ_UUID, uuidStr) == false ) { return false; } }
	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_BK_IMAGE_INDEX, pModel->GetModelBKImageIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_CHIP_SIZE_MODE, pModel->GetModelChipSizeMode()) == false ) { return false; }	

	if ( FileIO.SaveChunk_DBL(FILE_IO_MODEL_BODY_SIZE_X, pModel->GetModelBodySizeX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MODEL_BODY_SIZE_Y, pModel->GetModelBodySizeY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MODEL_BODY_HEIGHT, pModel->GetModelBodyHeight()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT64(FILE_IO_MODEL_MODIFIED_DATE_TIME, pModel->GetModelModifiedDateTime()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_MODEL_EXTEND_RANGE_X, pModel->GetModelExtendRangeX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MODEL_EXTEND_RANGE_Y, pModel->GetModelExtendRangeY()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_MODEL_EXTEND_AUTO_ADJUST, pModel->GetModelExtendAutoAdjust()) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_MODEL_SAVE_LEAD_REPORT, pModel->GetModelSaveLeadReport()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_MODEL_BODY_LINK_CHIP_LEAD, pModel->GetModelBodyLinkChipLead()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_WND_SYNC_MOVE_MODE, pModel->GetModelWndSyncMoveMode()) == false ) { return false; }		

	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_DEFECT_ITEM_ESSENTIAL, 0) == false ) { return false; }		
	if ( pModel->GetModelDefectItemEssential().WriteWndDefectItemFile(FileIO) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_DEFECT_ITEM_RECHECK_ARS, 0) == false ) { return false; }	
	if ( pModel->GetModelDefectItemRecheck_ARS().WriteWndDefectItemFile(FileIO) == false ) { return false; }	

	//模組本體框參數
	BoxPtr = pModel->GetModelBodyBoxPtr();
	if ( NULL != BoxPtr )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_BODY_BOX, 0) == false ) { return false; }	
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }		
	}
	//模組特徵框參數
	CAOILand    *LandPtr = NULL;
	const size_t LandCount = pModel->GetModelLandCount();
	for ( i=0; i<LandCount; i++ )
	{
		LandPtr = pModel->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_LAND_NODE, 0) == false ) { return false; }	
		if ( LandPtr->WriteLandFile(FileIO) == false ) { return false; }		
	}
	//模組檢測框參數
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = pModel->GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = pModel->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_WND_NODE, 0) == false ) { return false; }
		if ( WndPtr->WriteWndFile(FileIO) == false ) { return false; }		
	}

	if ( FileIO.SaveChunk_INT(FILE_IO_MODEL_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ReadModelFile(CAOIFileIO &FileIO)//載入模組檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	//----------------------------------------------------------------------------------------//
	UUID       uuid;
	int        index = 0;
	int        nValue=0;
	int        PanelIndex = 0;	
	double     dValue = 0;	
	CString    ModelName;
	CString    ModelFolder;
	CAOIBox   *BoxPtr = NULL;
	CAOIWnd   *WndPtr = NULL;
	CAOILand  *LandPtr = NULL;
	CAOIModel *pModel = this;
	CWndDefectItem  ModelDefectItem;
	AOI_OBJ_TYPE  AttachedType = pModel->GetModelAttachedType();
	FileIO.SetFnName(_T("CAOIModel::ReadModelFile"));
	//----------------------------------------------------------------------------------------//	
	ClearModelAllObjList();	
	//----------------------------------------------------------------------------------------//		
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_MODEL_START://模組參數-起點
			break;
		case FILE_IO_MODEL_END://模組參數-終點			
			pModel->CombineModelObjectList();
			pModel->AssignModelFolder();
			pModel->InvisibleModelWnd();
			pModel->VisibleModelLand(false);
			pModel->SetModelNeedSaveFiles(false);
			pModel->SetModelModifiedCount(false);
			pModel->SetModelBKImageNeedToGrab(false);
			pModel->UpdateModelRegionToAttached();
			return true;
			break;
		case FILE_IO_MODEL_TYPE://模組參數-模組樣式
			nValue = FileIO.GetData_INT();
			pModel->SetModelType((MODEL_TYPE)(nValue));
			pModel->SetModelWndSyncMoveMode(ObtainModelDefaultWndSyncMoveMode((MODEL_TYPE)(nValue)));
			break;
		case FILE_IO_MODEL_NAME://模組參數-模組名稱
			if ( FileIO.GetLoadWStr()==true )
			{	pModel->SetModelName(FileIO.GetData_WSTR());	}
			else
			{	pModel->SetModelName(FileIO.GetData_STR()); }
			ModelName = pModel->GetModelName();
			switch ( AttachedType )
			{
			case AOI_OBJ_FD:	ModelFolder.Format(_T("%s\\%s"), FileIO.GetFdFolder(), ModelName);	break;
			default:			ModelFolder.Format(_T("%s\\%s"), FileIO.GetLibraryFolder(), ModelName);	break;
			}
			pModel->SetModelFolderModel(ModelFolder);
			break;
		case FILE_IO_MODEL_GROUP_NAME://模組參數-模組群組名稱
			if ( FileIO.GetLoadWStr()==true )
			{	pModel->SetModelGroupName(FileIO.GetData_WSTR());	}
			else
			{	pModel->SetModelGroupName(FileIO.GetData_STR()); }
			break;
		case FILE_IO_MODEL_LAND_DIRECTION://模組參數-模組腳位方向
			pModel->SetModelLandDirection((MODEL_LAND_DIRECTION)(FileIO.GetData_INT()));
			break;
		case FILE_IO_MODEL_OBJ_UUID://模組參數-UUID
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	pModel->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	pModel->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_MODEL_BK_IMAGE_INDEX://模組參數-模組底圖影像編號
			pModel->SetModelBKImageIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_MODEL_CHIP_SIZE_MODE://模組參數-模組被動元件尺寸模式
			pModel->SetModelChipSizeMode((CHIP_SIZE_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_MODEL_BODY_SIZE_X://模組參數-模組本體尺寸X
			pModel->SetModelBodySizeX(FileIO.GetData_DBL());
			break;
		case FILE_IO_MODEL_BODY_SIZE_Y://模組參數-模組本體尺寸Y
			pModel->SetModelBodySizeY(FileIO.GetData_DBL());
			break;
		case FILE_IO_MODEL_BODY_HEIGHT://模組參數-模組本體高度
			pModel->SetModelBodyHeight(FileIO.GetData_DBL());
			break;
			
		case FILE_IO_MODEL_MODIFIED_DATE_TIME://模組參數-模組修改日期
			pModel->SetModelModifiedDateTime(FileIO.GetData_INT64());
			break;
		case FILE_IO_MODEL_EXTEND_RANGE_X://模組參數-模組外擴範圍X
			pModel->SetModelExtendRangeX(FileIO.GetData_DBL());
			break;
		case FILE_IO_MODEL_EXTEND_RANGE_Y://模組參數-模組外擴範圍Y
			pModel->SetModelExtendRangeY(FileIO.GetData_DBL());
			break;
		case FILE_IO_MODEL_EXTEND_AUTO_ADJUST://模組參數-模組外擴自動調整
			pModel->SetModelExtendAutoAdjust(FileIO.GetData_BOL());
			break;
		case FILE_IO_MODEL_SAVE_LEAD_REPORT://模組參數-模組儲存引腳報告
			pModel->SetModelSaveLeadReport(FileIO.GetData_BOL());
			break;
		case FILE_IO_MODEL_BODY_LINK_CHIP_LEAD://模組參數-模組本體連動引腳啟用
			pModel->SetModelBodyLinkChipLead(FileIO.GetData_BOL());
			break;
		case FILE_IO_MODEL_WND_SYNC_MOVE_MODE://模組參數-檢測框同步移動模式-預設
			pModel->SetModelWndSyncMoveMode((WND_SYNC_MOVE_MODE)(FileIO.GetData_INT()));
			break;

		case FILE_IO_MODEL_DEFECT_ITEM_ESSENTIAL://模組參數-模組瑕疵必要確認項目			
			ModelDefectItem.SetAll(0);
			if ( ModelDefectItem.ReadWndDefectItemFile(FileIO) == false )
			{	ModelDefectItem.SetAll(0); }
			pModel->SetModelDefectItemEssential(ModelDefectItem);			
			break;
		case FILE_IO_MODEL_DEFECT_ITEM_RECHECK_ARS://模組參數-模組瑕疵重複確認項目
			ModelDefectItem.SetAll(0);
			if ( ModelDefectItem.ReadWndDefectItemFile(FileIO) == false )
			{	ModelDefectItem.SetAll(0); }
			pModel->SetModelDefectItemRecheck_ARS(ModelDefectItem);
			break;

		case FILE_IO_MODEL_BODY_BOX://模組參數-模組本體框
			BoxPtr = pModel->GetModelBodyBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO)==false )				
				{	return false; }
			}			
			break;
		case FILE_IO_MODEL_LAND_NODE:
			LandPtr = AOIObjManager.CreateLandObj();
			if ( NULL == LandPtr )
			{
				FileIO.SetErrorString(_T("Create Land Obj Fault"));
				return false;
			}
			if ( LandPtr->ReadLandFile(FileIO) == false )			
			{
				AOIObjManager.DestroyLandObj(LandPtr);
				return false; 
			}
			pModel->AddModelLandPtr(LandPtr, false);
			break;
		case FILE_IO_MODEL_WND_NODE:
			WndPtr = AOIObjManager.CreateWndObj();
			if ( NULL == WndPtr )
			{
				FileIO.SetErrorString(_T("Create Wnd Obj Fault"));
				return false;
			}
			if ( WndPtr->ReadWndFile(FileIO) == false )			
			{
				AOIObjManager.DestroyWndObj(WndPtr);
				return false; 
			}
			pModel->AddModelWndPtr(WndPtr, false);
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::WriteModelSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr) {

	CAOIModel *ModelPtr = this;
	CAOIWnd *WndPtr = NULL;
	CAOILand *LandPtr = NULL;
	CAOIBox *BoxPtr = NULL;

	TREGION4D ModelRgn;
	TPOINT2D BoxCad;
	TSIZE2D BoxSize;
	const size_t WndCount = ModelPtr->GetModelWndCount();
	const size_t LandCount = ModelPtr->GetModelLandCount();
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";
	unsigned int i;

	BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	ModelPtr->GetModelBodyPos(BoxCad);
	BoxPtr->GetBoxRegion(ModelRgn);
	BoxPtr->GetBoxPos(BoxCad);
	BoxPtr->GetBoxSize(BoxSize);

	::fwprintf(pfile, L"    {\n");
	::wcscpy(strTag, L"ModelName");
	JetAPI::TCHAR2wchar(ModelPtr->GetModelName(), strBuffer, szBuffer);
	::fwprintf(pfile, L"      \"%s\":\"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"BodyCenterX");
	//::fwprintf(pfile, L"      \"%s\":%.3f,\n", strTag, BoxCad.x);
	::fwprintf(pfile, L"      \"%s\":%.3f,\n", strTag, ModelRgn.GetCpX());

	::wcscpy(strTag, L"BodyCenterY");
	//::fwprintf(pfile, L"      \"%s\":%.3f,\n", strTag, BoxCad.y);
	::fwprintf(pfile, L"      \"%s\":%.3f,\n", strTag, ModelRgn.GetCpY());

	::wcscpy(strTag, L"BodyWidth");
	::fwprintf(pfile, L"      \"%s\":%.3f,\n", strTag, BoxSize.cx);

	::wcscpy(strTag, L"BodyHeight");
	::fwprintf(pfile, L"      \"%s\":%.3f,\n", strTag, BoxSize.cy);

	::wcscpy(strTag, L"Wnd_List");
	::fwprintf(pfile, L"      \"%s\":[\n", strTag);
	for (i = 0; i < WndCount; i++) {
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if (NULL == WndPtr) { continue; }
		if (WndPtr->WriteWndSpcHeader_JSON_detail(pfile, ProjectPtr) == false) { return FALSE; }
		//Prevent Trailing Comma
		if (i == WndCount - 1) { ::fwprintf(pfile, L"\n"); }
		else { ::fwprintf(pfile, L",\n"); }

	}

	::fwprintf(pfile, L"      ],\n");

	::wcscpy(strTag, L"Lead_List");
	::fwprintf(pfile, L"      \"%s\":[\n", strTag);
	//todo:
	for (i = 0; i < LandCount; i++) {
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if (NULL == LandPtr) { continue; }
		if (LandPtr->WriteLandLeadSpcHeader_JSON_detail(pfile, ProjectPtr) == false) { return FALSE; }
		//Prevent Trailing Comma
		if (i == LandCount - 1) { ::fwprintf(pfile, L"\n"); }
		else { ::fwprintf(pfile, L",\n"); }
	}
	::fwprintf(pfile, L"      ],\n");


	::wcscpy(strTag, L"Pad_List");
	::fwprintf(pfile, L"      \"%s\":[\n", strTag);
	//todo:
	for (i = 0; i < LandCount; i++) {
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if (NULL == LandPtr) { continue; }
		if (LandPtr->WriteLandPadSpcHeader_JSON_detail(pfile, ProjectPtr) == false) { return FALSE; }
		//Prevent Trailing Comma
		if (i == LandCount - 1) { ::fwprintf(pfile, L"\n"); }
		else { ::fwprintf(pfile, L",\n"); }
	}
	::fwprintf(pfile, L"      ]\n");
	::fwprintf(pfile, L"    }");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelWndParamStringList(LPCTSTR Title, std::vector<CString> &strList)//建立模組檢測框列表
{
	size_t       i=0;		
	CString      strTitle=_T("");
	CString      strDisabled=_T("");
	CAOIWnd     *WndPtr = NULL;
	CString      strModel=GetModelName();
	const size_t WndCount=GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false )
		{	strDisabled = _T("Y"); }
		else
		{	strDisabled = _T("N"); }
		strTitle.Format(_T("%s, %s, %d, %s"), Title, strModel, i+1, strDisabled);
		if ( WndPtr->BuildWndParamStringList(strTitle, strList) == false )
		{	return false;	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIModel::GetModelDefaultFilename()//取得預設模組名稱
{
	CString filename;
	CAOIModel *ModelPtr = this;
	if ( NULL == ModelPtr ) { return filename; }
	CString    GroupName = ModelPtr->GetModelGroupName();
	return CAOIModel::GetModelDefaultFilename(GroupName);
}
//-------------------------------------------------------------------------------------//
CString CAOIModel::GetModelDefaultImageFilename()//取得預設模組影像名稱
{
	CString filename;
	CAOIModel *ModelPtr = this;
	if ( NULL == ModelPtr ) { return filename; }
	CString    GroupName = ModelPtr->GetModelGroupName();
	return CAOIModel::GetModelDefaultImageFilename(GroupName);	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SaveModelParamFile(LPCTSTR pfilename)//儲存模組參數檔案
{
	if ( NULL == pfilename )  { return false; }
	
	CString      str;
	CString      strError;
	CAOIFileIO   FileIO;
	bool         IsOK=true;
	CString      filename = pfilename;
	CString      FileNameDst = pfilename;	
	MODEL_TYPE   ModelType = GetModelType();
	CString      ModelName = GetModelName();
	CString      GroupName = GetModelGroupName();
	CString      ModelFolderModel = GetModelFolderModel();
	CString      ModelFolderComponent = GetModelFolderComponent();
	CString      DefaultModelFolder = AOIDataCollect.GetAOIDefaultModelDirectory();
	if ( MODEL_TYPE_NULL == ModelType ) 
	{ 
		strError = _T("Error, the model is null");
		AOIDataCollect.SetErrorString(strError);
		return false; 
	}

	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);
	
	FileIO.CreateProgressWnd();
	FileIO.SetFileName(FileNameDst);	
	FileIO.SetTotalNodes(0);
	FileIO.SetTotalModelObjs(0);
	FileIO.SetTotalComponents(0);
	FileIO.SetNNodeCount(0);
	FileIO.SetFileTarget(FILE_TARGET_DEFAULT_MODEL);	
	FileIO.SetLibraryFolder(DefaultModelFolder);

	IsOK = FileIO.OpenSaveFile(filename);	
	if ( false == IsOK )
	{
		strError = FileIO.GetErrorString();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);
		AOIDataCollect.SetErrorString(strError);
		return false;
	}	
	//設定成群組名稱
	//SetModelName(GroupName);
	IsOK = WriteModelFile(FileIO);	
	//恢復設定
	SetModelName(ModelName);
	SetModelFolderModel(ModelFolderModel);	
	SetModelFolderComponent(ModelFolderComponent);
	AssignModelFolder();//恢復模組資料夾
	::SetCursor(hOldCursor);
	if ( false == IsOK )
	{
		strError = FileIO.GetErrorString();
		FileIO.CloseFile();
		FileIO.DestroyProgressWnd();		
		AOIDataCollect.SetErrorString(strError);
		return false;
	}
	FileIO.CloseFile();
	FileIO.DestroyProgressWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::LoadModelParamFile(LPCTSTR pfilename)//儲存模組參數檔案
{
	if ( NULL == pfilename )  { return false; }
	
	HCURSOR hCursor = ::AfxGetApp()->LoadStandardCursor(IDC_WAIT);
	HCURSOR hOldCursor = ::SetCursor(hCursor);

	CString      str;
	CString      strError;
	bool         IsOK=true;
	CAOIFileIO   FileIO;
	CString      TempFolder;
	CString      FileName = pfilename;
	CString      DefaultModelFolder = AOIDataCollect.GetAOIDefaultModelDirectory();
	FileIO.CreateProgressWnd();	

	FileIO.SetFileName(FileName);
	FileIO.SetTotalNodes(0);
	FileIO.SetNNodeCount(0);
	FileIO.SetTotalModelObjs(0);
	FileIO.SetTotalComponents(0);
	FileIO.SetFileTarget(FILE_TARGET_DEFAULT_MODEL);
	FileIO.SetLibraryFolder(DefaultModelFolder);
	//----------------------------------------------------------------------------------------//	
	ClearModelAllObjList();	
	RemoveModelImageFolder();
	ReleaseModelUniFrameList();
	//----------------------------------------------------------------------------------------//	
	IsOK = FileIO.OpenLoadFile(FileName);	
	if ( false == IsOK )
	{
		strError = FileIO.GetErrorString();
		FileIO.DestroyProgressWnd();
		::SetCursor(hOldCursor);
		AOIDataCollect.SetErrorString(strError);
		return false;
	}
	IsOK = ReadModelFile(FileIO);
	::SetCursor(hOldCursor);
	if ( false == IsOK )
	{
		strError = FileIO.GetErrorString();
		FileIO.CloseFile();
		FileIO.DestroyProgressWnd();		
		AOIDataCollect.SetErrorString(strError);
		return false;
	}
	FileIO.CloseFile();
	FileIO.DestroyProgressWnd();	
	SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelTypeEnabled() const
{
	return CheckModelTypeEnabled(GetModelType());	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ArrangeModelBKImageFiles()//重新整理模組底圖
{
	CAOIModel *ModelPtr = this;
	if ( NULL == ModelPtr ) { return false; }

	std::vector<CString> ModelBkImageList;
	CString ModelName = ModelPtr->GetModelName();
	CString ModelFolder = ModelPtr->GetModelFolder();
			
	if ( JetAPI::ListFilesInFolder(ModelFolder, _T("PNG"), ModelBkImageList) == false ) 
	{	return true; }
	
	size_t i=0;
	CString ModelBkImageName;
	CString ModelBkImagePathName;
	const size_t BkImageCount=ModelBkImageList.size();
	for ( i=0; i<BkImageCount; i++ )
	{
		ModelBkImageName = ModelBkImageList[i];
		if ( JetAPI::FindTextInString(ModelName, ModelBkImageName) == true ) 
		{	continue; }
		ModelPtr->SetModelNeedSaveFiles(true);
		ModelBkImagePathName.Format(_T("%s\\%s"), ModelFolder, ModelBkImageName);
		::DeleteFile(ModelBkImagePathName);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLinkModeWndPos() const//模組檢測框位置同動
{	
	return CheckModelWndLinkMode(MODEL_LINK_BODY_WND_POS);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLinkModeWndSize() const//模組檢測框尺寸同動
{
	return CheckModelWndLinkMode(MODEL_LINK_BODY_WND_SIZE);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLinkModeLandPos() const//模組特徵框位置同動
{
	return CheckModelWndLinkMode(MODEL_LINK_LAND_POS);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLinkModeLandSize() const//模組特徵框尺寸同動
{
	return CheckModelWndLinkMode(MODEL_LINK_LAND_SIZE);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel:: GetModelLinkModeLandWndPos() const//模組特徵檢測框位置同動
{
	return CheckModelWndLinkMode(MODEL_LINK_LAND_WND_POS);
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelLinkModeLandWndSize() const//模組特徵檢測框尺寸同動
{
	return CheckModelWndLinkMode(MODEL_LINK_LAND_WND_SIZE);
}
//-------------------------------------------------------------------------------------//
void CAOIModel::EnableModelLinkModeWndPos(bool bEnable)//啟用模組檢測框位置同動
{
	if ( true == bEnable  )
	{	AddModelWndLinkMode(MODEL_LINK_BODY_WND_POS);	}
	else
	{	RemoveModelWndLinkMode(MODEL_LINK_BODY_WND_POS);	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::EnableModelLinkModeWndSize(bool bEnable)//啟用模組檢測框尺寸同動
{
	if ( true == bEnable  )
	{	AddModelWndLinkMode(MODEL_LINK_BODY_WND_SIZE);	}
	else
	{	RemoveModelWndLinkMode(MODEL_LINK_BODY_WND_SIZE);	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::EnableModelLinkModeLandPos(bool bEnable)//啟用模組特徵框位置同動
{
	if ( true == bEnable  )
	{	AddModelWndLinkMode(MODEL_LINK_LAND_POS);	}
	else
	{	RemoveModelWndLinkMode(MODEL_LINK_LAND_POS);	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::EnableModelLinkModeLandSize(bool bEnable)//啟用模組特徵框尺寸同動
{
	if ( true == bEnable  )
	{	AddModelWndLinkMode(MODEL_LINK_LAND_SIZE);	}
	else
	{	RemoveModelWndLinkMode(MODEL_LINK_LAND_SIZE);	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::EnableModelLinkModeLandWndPos(bool bEnable)//啟用模組特徵檢測框位置同動
{
	if ( true == bEnable  )
	{	AddModelWndLinkMode(MODEL_LINK_LAND_WND_POS);	}
	else
	{	RemoveModelWndLinkMode(MODEL_LINK_LAND_WND_POS);	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::EnableModelLinkModeLandWndSize(bool bEnable)//啟用模組特徵檢測框尺寸同動
{
	if ( true == bEnable  )
	{	AddModelWndLinkMode(MODEL_LINK_LAND_WND_SIZE);	}
	else
	{	RemoveModelWndLinkMode(MODEL_LINK_LAND_WND_SIZE);	}
}
//-------------------------------------------------------------------------------------//
void CAOIModel::RemoveModelWndLinkMode(int LinkMode)
{
	const int NotLinkMode = ~LinkMode;
	m_ModelWndLinkMode = m_ModelWndLinkMode&NotLinkMode;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelWndLinkMode(int LinkMode) const//確認是否連動
{
	int Res = m_ModelWndLinkMode&LinkMode;
	if ( 0 == Res ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelColorGroupLinkIndex(int Index, const CColorGroup &ColorGroup)//更新模組內的彩色過濾連動
{
	size_t         i=0, j=0;	
	size_t         WndCount=0;
	size_t         WndRoiCount=0;
	int            ColorGroupIndex=0;	
	CAOIWnd       *WndPtr = NULL;	
	CAOIWndRoi    *WndRoiPtr = NULL;
	CAlgBinaryParam *BinParamPtr=NULL;	

	WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }

		//Alg Image
		BinParamPtr = WndPtr->GetWndAlgParam().GetAlgImageBinParamPtr();
		ColorGroupIndex = BinParamPtr->GetBinaryColorGroupLinkIndex();
		if ( ColorGroupIndex == Index )		
		{	BinParamPtr->SetBinaryColorGroup(ColorGroup);	}

		//Alg Mask
		BinParamPtr = WndPtr->GetWndAlgParam().GetAlgMaskBinParamPtr();
		ColorGroupIndex = BinParamPtr->GetBinaryColorGroupLinkIndex();
		if ( ColorGroupIndex == Index )		
		{	BinParamPtr->SetBinaryColorGroup(ColorGroup);		}

		WndRoiCount = WndPtr->GetWndRoiWndCount();
		for ( j=0; j<WndRoiCount; j++ )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(j, false);
			if ( NULL == WndRoiPtr ) { continue; }
			BinParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
			ColorGroupIndex = BinParamPtr->GetBinaryColorGroupLinkIndex();
			if ( ColorGroupIndex == Index )		
			{	BinParamPtr->SetBinaryColorGroup(ColorGroup);		}
		}
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::UpdateModelColorGroupLinkIndex(const std::vector<CColorGroup> &ColorGroupList)//更新模組內的彩色過濾連動
{
	const size_t   ColorGroupCount = ColorGroupList.size();
	if ( 0 == ColorGroupCount ) { return true; }

	size_t         i=0, j=0;	
	size_t         WndCount=0;
	size_t         WndRoiCount=0;
	int            ColorGroupIndex=0;	
	unsigned int   FrameIndex = 0;
	unsigned int   FrameUniqueID = 0;
	CAOIWnd       *WndPtr = NULL;	
	CAOIWndRoi    *WndRoiPtr = NULL;
	CAlgBinaryParam *BinParamPtr=NULL;	

	WndCount = GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }

		//Alg Image
		BinParamPtr = WndPtr->GetWndAlgParam().GetAlgImageBinParamPtr();
		ColorGroupIndex = BinParamPtr->GetBinaryColorGroupLinkIndex();
		if ( ColorGroupIndex>=0 && ColorGroupIndex<ColorGroupCount )
		{	BinParamPtr->SetBinaryColorGroup(ColorGroupList[ColorGroupIndex]);		}

		//Alg Mask
		BinParamPtr = WndPtr->GetWndAlgParam().GetAlgMaskBinParamPtr();
		ColorGroupIndex = BinParamPtr->GetBinaryColorGroupLinkIndex();
		if ( ColorGroupIndex>=0 && ColorGroupIndex<ColorGroupCount )
		{	BinParamPtr->SetBinaryColorGroup(ColorGroupList[ColorGroupIndex]);		}

		WndRoiCount = WndPtr->GetWndRoiWndCount();
		for ( j=0; j<WndRoiCount; j++ )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(j, false);
			if ( NULL == WndRoiPtr ) { continue; }
			BinParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
			ColorGroupIndex = BinParamPtr->GetBinaryColorGroupLinkIndex();
			if ( ColorGroupIndex>=0 && ColorGroupIndex<ColorGroupCount )
			{	BinParamPtr->SetBinaryColorGroup(ColorGroupList[ColorGroupIndex]);		}
		}
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelPanelBasePlane(double val)
{
	m_ModelPanelBasePlane = val;
	m_ModelSpaceNoiseFilterParam.BasePlaneParam.PanelBasePlane = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelPanelBasePlaneParamValid() const//確認空間基準面參數有效
{
	bool bValid=true;
	const TBasePlaneParam &BasePlaneParamRef=GetModelSpaceBasePlaneParam();	
	switch ( BasePlaneParamRef.CalcBasePlaneMode )
	{
	case CALC_BASE_PLANE_DISABLE:
		bValid = false;
		break;
	case CALC_BASE_PLANE_PANEL:
		if ( fabs(BasePlaneParamRef.PanelBasePlane) < 0.0001 )//如果離線編程下讀取不到3D資料, 就用線上檢測的參數值
		{	bValid = false; }
		break;
	case CALC_BASE_PLANE_LOCAL:
		if ( fabs(BasePlaneParamRef.NormalZ) < 0.0001 )//如果離線編程下讀取不到3D資料, 就用線上檢測的參數值
		{	bValid = false; }			
		break;
	default:
		bValid = true;
		break;
	}
	return bValid;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelSpaceBasePlaneParam(const TBasePlaneParam& Param)
{	
	m_ModelSpaceNoiseFilterParam.BasePlaneParam = Param;		
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelSpaceNoiseFilterParam(const TNoiseFilterParam& Param)
{
	TBasePlaneParam LevelBaseParam = m_ModelSpaceNoiseFilterParam.BasePlaneParam;
	m_ModelSpaceNoiseFilterParam = Param;
	m_ModelSpaceNoiseFilterParam.BasePlaneParam = LevelBaseParam;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelSpaceLeveingParam(double nX, double nY, double nZ)
{
	m_ModelSpaceNoiseFilterParam.BasePlaneParam.NormalX = nX;
	m_ModelSpaceNoiseFilterParam.BasePlaneParam.NormalY = nY;
	m_ModelSpaceNoiseFilterParam.BasePlaneParam.NormalZ = nZ;
	m_ModelSpaceNoiseFilterParam.BasePlaneParam.GroundEquation.SetGroundNormal(nX, nY, nZ);	
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelBarcodeList(std::vector<std::wstring> &BarcodeList)//取得零件條碼列表
{
	size_t        i=0, j=0;	
	ALG_TYPE      AlgType;
	WND_DEFECT_ID WndDefectID;
	CAOIWnd      *WndPtr = NULL;
	CAOIWnd      *WndPtr2 = NULL;
	const int     FnEnable=FN_ENABLE;
	const int     FnDisable=FN_DISABLE;	
	std::vector<CAOIWnd*> BarcodeWndList;
	const size_t  WndCount = GetModelWndCount();
	
	BarcodeList.clear();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndTempInt1(FnDisable);
		AlgType = WndPtr->GetWndAlgType();
		if ( ALG_BARCODE_RECOGNIZE != AlgType )
		{	continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		if ( WND_DEFECT_BODY_WRONG_CODE != WndDefectID )
		{	continue; }
		BarcodeWndList.push_back(WndPtr);		
	}
	
	const size_t BarcodeWndCount=BarcodeWndList.size();
	if ( 0 ==  BarcodeWndCount )
	{	return true; }
	
	RESULT_ID ResultID;
	int   LogicGroupID=0;			
	std::wstring Barcode;
	WND_LOGIC_TYPE WndLogicType;
	const wchar_t BadBarcode[]=L"NoRead";

	for ( i=0; i<BarcodeWndCount; i++ )
	{
		WndPtr = BarcodeWndList[i];
		if ( NULL == WndPtr ) { continue; }
		if ( FnEnable == WndPtr->GetWndTempInt1() ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		WndLogicType = WndPtr->GetWndLogicType();		
		LogicGroupID = WndPtr->GetWndLogicGroupID();
		if ( WND_LOGIC_NONE == WndLogicType ) 
		{	
			WndPtr->SetWndTempInt1(FnEnable);
			ResultID = WndPtr->GetWndResultID();
			if ( RESULT_ID_OK != ResultID )
			{	Barcode = BadBarcode;	}
			else
			{	Barcode = WndPtr->GetWndAlgParam().GetAlgParamBarcodeRecognize().brBarcodeResult;	}
			BarcodeList.push_back(Barcode);	
			continue;
		}		
		
		Barcode.clear();
		for ( j=i; j<BarcodeWndCount; j++ )
		{
			WndPtr2 = BarcodeWndList[j];
			if ( NULL == WndPtr2 ) { continue; }
			WndLogicType = WndPtr2->GetWndLogicType();
			if ( WND_LOGIC_NONE == WndLogicType ) { continue; }
			if ( WndPtr2->GetWndLogicGroupID() != LogicGroupID ) { continue; }
			if ( WND_LOGIC_DEFECT_ID == WndLogicType )
			{
				if ( WndDefectID != WndPtr2->GetWndDefectID() ) 
				{	continue; }
			}
			WndPtr2->SetWndTempInt1(FnEnable);
			ResultID = WndPtr2->GetWndResultID();
			if ( RESULT_ID_OK != ResultID ) { continue; }
			if ( Barcode.length() == 0 ) 
			{	Barcode = WndPtr2->GetWndAlgParam().GetAlgParamBarcodeRecognize().brBarcodeResult; }			
		}
		if ( Barcode.length() == 0 )
		{	Barcode = BadBarcode;	}
		BarcodeList.push_back(Barcode);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelSpecResult(CAOILand *RefLandPtr, TSpecResult &sX, TSpecResult &sY, TSpecResult &sA, TSpecResult &tA, TSpecResult &rH, TSpecResult &rA, TSpecResult &rV, TSpecResult &szW, TSpecResult &szH, TSpecResult &oA, RESULT_ID &ResultID)
{
	CAOIModel     *ModelPtr = this;

	size_t         i=0;
	size_t         idx=0;		
	DWORD          Res=0;	
	unsigned int   WndIdx=0;
	double         dTiltA=INVALID_DOUBLE;	
	double         dSkewA=INVALID_DOUBLE;
	double         dOffsetX=INVALID_DOUBLE;
	double         dOffsetY=INVALID_DOUBLE;
	double         dOffsetA=INVALID_DOUBLE;
	double         dResultHeight=INVALID_DOUBLE;
	double         dResultArea=INVALID_DOUBLE;
	double         dResultVolume=INVALID_DOUBLE;
	double         dResultWidth=INVALID_DOUBLE;
	double         dResultLength=INVALID_DOUBLE;
	double         dResultHeightMin=INVALID_DOUBLE;
	double         dResultHeightMax=INVALID_DOUBLE;

	double         dTiltA_USL=INVALID_DOUBLE;
	double         dTiltA_LSL=INVALID_DOUBLE;
	unsigned int   uTiltA_Idx=INVALID_INDEX;
	double         dSkewA_USL=INVALID_DOUBLE;
	double         dSkewA_LSL=INVALID_DOUBLE;
	unsigned int   uSkewA_Idx=INVALID_INDEX;
	double         dOffsetX_USL=INVALID_DOUBLE;
	double         dOffsetX_LSL=INVALID_DOUBLE;
	unsigned int   uOffsetX_Idx=INVALID_INDEX;
	double         dOffsetY_USL=INVALID_DOUBLE;
	double         dOffsetY_LSL=INVALID_DOUBLE;
	unsigned int   uOffsetY_Idx=INVALID_INDEX;
	double         dOffsetA_USL=INVALID_DOUBLE;
	double         dOffsetA_LSL=INVALID_DOUBLE;
	unsigned int   uOffsetA_Idx=INVALID_INDEX;
	double         dResultHeight_USL=INVALID_DOUBLE;
	double         dResultHeight_LSL=INVALID_DOUBLE;
	double         dResultHeight_Spec=INVALID_DOUBLE;
	unsigned int   uResultHeight_Idx=INVALID_INDEX;
	double         dResultArea_USL=INVALID_DOUBLE;
	double         dResultArea_LSL=INVALID_DOUBLE;
	double         dResultArea_Spec=INVALID_DOUBLE;
	unsigned int   uResultArea_Idx=INVALID_INDEX;
	double         dResultVolume_USL=INVALID_DOUBLE;
	double         dResultVolume_LSL=INVALID_DOUBLE;
	double         dResultVolume_Spec=INVALID_DOUBLE;
	unsigned int   uResultVolume_Idx=INVALID_INDEX;
	double         dResultWidth_USL=INVALID_DOUBLE;
	double         dResultWidth_LSL=INVALID_DOUBLE;
	double         dResultWidth_Spec=INVALID_DOUBLE;
	unsigned int   uResultWidth_Idx=INVALID_INDEX;
	double         dResultLength_USL=INVALID_DOUBLE;
	double         dResultLength_LSL=INVALID_DOUBLE;
	double         dResultLength_Spec=INVALID_DOUBLE;
	unsigned int   uResultLength_Idx=INVALID_INDEX;

	ALG_TYPE       AlgType;
	RESULT_ID      WndResultID;
	WND_DEFECT_ID  WndDefectID;
	unsigned int   FrameUniqueID = 0;	
	CAOIWnd       *WndPtr = NULL;
	CAOILand      *LandPtr = NULL;		
	std::vector<CAOIWnd*>    WndList;
	TALG_PARAM_MODEL_MATCH  *ModelMatchPtr=NULL;//演算法-模板匹配參數
	TALG_PARAM_IMAGE_MATCH  *ImageMatchPtr=NULL;//演算法-影像匹配參數
	TALG_PARAM_FD_MATCH     *FdMatchPtr=NULL;//演算法-定位點搜尋參數	
	const size_t WndOrderCount = ModelPtr->GetModelWndOrderCount();

	dTiltA=INVALID_DOUBLE;
	dSkewA=INVALID_DOUBLE;
	dOffsetX=INVALID_DOUBLE;
	dOffsetY=INVALID_DOUBLE;
	dOffsetA=INVALID_DOUBLE;
	dResultHeight=INVALID_DOUBLE;
	dResultArea=INVALID_DOUBLE;
	dResultVolume=INVALID_DOUBLE;
	dResultWidth=INVALID_DOUBLE;
	dResultLength=INVALID_DOUBLE;	
	dResultHeightMin=INVALID_DOUBLE;
	dResultHeightMax=INVALID_DOUBLE;

	szW.Init(); szH.Init();
	rH.Init(); rA.Init(); rV.Init();
	sX.Init(); sY.Init(); sA.Init(); tA.Init(); oA.Init();

	if ( NULL == RefLandPtr )
	{
		for ( i=0; i<WndOrderCount; i++ )
		{
			idx = WndOrderCount-i-1;
			WndPtr = ModelPtr->GetModelWndOrderPtr(idx, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			LandPtr = WndPtr->GetWndLandPtr();
			if ( NULL != LandPtr ) { continue; }		
			WndList.push_back(WndPtr);
		}
	}
	else
	{	
		size_t LandWndCount=RefLandPtr->GetLandWndCount();
		for ( i=0; i<LandWndCount; i++ )
		{	
			WndPtr = RefLandPtr->GetLandWndPtr(i, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			WndList.push_back(WndPtr);
		}
		SortModelWndOrder(WndList);
		std::reverse(WndList.begin(), WndList.end());
	}
	
	ResultID = RESULT_ID_NONE;
	const size_t WndCount=WndList.size();
	for ( i=0; i<WndCount; i++ )
	{	
		WndPtr = WndList[i];
		if ( NULL == WndPtr ) { continue; }
		//if ( WndPtr->GetWndEnabled() == false ) { continue; }
		//LandPtr = WndPtr->GetWndLandPtr();
		//if ( LandPtr != RefLandPtr ) { continue; }

		CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		
		
		WndIdx = WndPtr->GetWndIndex();
		AlgType = AlgParam.GetAlgType();
		WndDefectID = WndPtr->GetWndDefectID();
		WndResultID = WndPtr->GetWndResultID();
		if ( RESULT_ID_NONE==ResultID || RESULT_ID_NG==WndResultID || RESULT_ID_EXCEPTION==WndResultID )
		{	ResultID = WndResultID;	}
		FrameUniqueID = AlgParam.GetAlgImageBinParam().GetBinaryFrameUniqueID();

		switch ( AlgType )
		{
		case ALG_BRIGHT_RATIO:
			if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{
				bool bUsed=false;
				if ( INVALID_DOUBLE==dResultHeight  )
				{	bUsed = true;	}
				else
				{
					if ( WND_DEFECT_BODY_MISSING == WndDefectID )
					{	bUsed = true;	}
				}
				if ( true == bUsed )
				{
					uResultHeight_Idx = (unsigned int)(WndIdx);
					double dSpec = AlgParam.GetAlgParamBrightRatio().brTargetValue;						
					dResultHeight = AlgParam.GetAlgParamBrightRatio().brAverageReading;	
					dResultHeight_Spec = AlgParam.GetAlgParamBrightRatio().brTargetValue;
					if ( AlgParam.GetAlgParamBrightRatio().brToleranceEnabled == true )
					{
						dResultHeight_USL = AlgParam.GetAlgParamBrightRatio().brToleranceUSL+dSpec;	
						dResultHeight_LSL = AlgParam.GetAlgParamBrightRatio().brToleranceLSL+dSpec;	
					}
					else if ( AlgParam.GetAlgParamBrightRatio().brRatioEnabled == true )
					{
						dResultHeight_USL = AlgParam.GetAlgParamBrightRatio().brRatioUSL*dSpec/100.0;	
						dResultHeight_LSL = AlgParam.GetAlgParamBrightRatio().brRatioLSL*dSpec/100.0;	
					}
					if ( AlgParam.GetAlgParamBrightRatio().brLimitMaxEnabled == true )
					{	dResultHeightMax = AlgParam.GetAlgParamBrightRatio().brAverageReadingMax;	}
					if ( AlgParam.GetAlgParamBrightRatio().brLimitMinEnabled == true )
					{	dResultHeightMin = AlgParam.GetAlgParamBrightRatio().brAverageReadingMin;	}
				}
			}
			break;
		case ALG_OBJECT_MEASURE:
			if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{				
				if ( true == AlgParam.GetAlgParamObjectMeasure().omHeightEnabled )
				{
					bool bUsed=false;
					if ( INVALID_DOUBLE==dResultHeight  )
					{	bUsed=true;	}
					else
					{
						if ( WND_DEFECT_BODY_MISSING == WndDefectID )
						{	bUsed=true;	}
					}
					if ( true == bUsed )
					{
						uResultHeight_Idx = (unsigned int)(WndIdx);
						double dSpec=AlgParam.GetAlgParamObjectMeasure().omHeightSpec;
						dResultHeight = AlgParam.GetAlgParamObjectMeasure().omHeightReading;
						dResultHeight_Spec = AlgParam.GetAlgParamObjectMeasure().omHeightSpec;
						switch ( AlgParam.GetAlgParamObjectMeasure().omHeightCalcUnitMode )
						{
						case ALG_CALC_UNIT_ABS://um					
						case ALG_CALC_UNIT_DIFF://um
							dResultHeight_USL = AlgParam.GetAlgParamObjectMeasure().omHeightDiffUSL+dSpec;
							dResultHeight_LSL = AlgParam.GetAlgParamObjectMeasure().omHeightDiffLSL+dSpec;
							break;
						case ALG_CALC_UNIT_RATIO://%
							dResultHeight_USL = AlgParam.GetAlgParamObjectMeasure().omHeightRatioUSL*dSpec/100.0;
							dResultHeight_LSL = AlgParam.GetAlgParamObjectMeasure().omHeightRatioLSL*dSpec/100.0;
							break;
						}
					}
				}
			}
			if ( WND_DEFECT_PART_ALIGN == WndDefectID )
			{
				if ( INVALID_DOUBLE==dSkewA && true==AlgParam.GetAlgSkewEnabled() )
				{
					uSkewA_Idx = (unsigned int)(WndIdx);
					dSkewA = AlgParam.GetAlgSkewReading();
					dSkewA_USL = AlgParam.GetAlgSkewUSL();
					dSkewA_LSL = AlgParam.GetAlgSkewLSL();
				}
				if ( INVALID_DOUBLE==dOffsetX && true==AlgParam.GetAlgOffsetXEnabled() )
				{	
					uOffsetX_Idx = (unsigned int)(WndIdx);
					dOffsetX = AlgParam.GetAlgOffsetXReading();	
					dOffsetX_USL = AlgParam.GetAlgOffsetXUSL();
					dOffsetX_LSL = AlgParam.GetAlgOffsetXLSL();
				}
				if ( INVALID_DOUBLE==dOffsetY && true==AlgParam.GetAlgOffsetYEnabled() )
				{	
					uOffsetY_Idx = (unsigned int)(WndIdx);
					dOffsetY = AlgParam.GetAlgOffsetYReading();
					dOffsetY_USL = AlgParam.GetAlgOffsetYUSL();
					dOffsetY_LSL = AlgParam.GetAlgOffsetYLSL();
				}
				if ( INVALID_DOUBLE==dOffsetA && true==AlgParam.GetAlgOffsetAEnabled() )
				{	
					uOffsetA_Idx = (unsigned int)(WndIdx);
					dOffsetA = AlgParam.GetAlgOffsetAReading();
					dOffsetA_USL = AlgParam.GetAlgOffsetAUSL();
					dOffsetA_LSL = AlgParam.GetAlgOffsetALSL();
				}
			}
			if ( true == AlgParam.GetAlgParamObjectMeasure().omAreaEnabled )
			{	
				uResultArea_Idx = (unsigned int)(WndIdx);
				double dSpec=AlgParam.GetAlgParamObjectMeasure().omAreaSpec;
				dResultArea = AlgParam.GetAlgParamObjectMeasure().omAreaReading;
				dResultArea_Spec =AlgParam.GetAlgParamObjectMeasure().omAreaSpec;
				dResultArea_USL = AlgParam.GetAlgParamObjectMeasure().omAreaUSL*dSpec/100.0;
				dResultArea_LSL = AlgParam.GetAlgParamObjectMeasure().omAreaLSL*dSpec/100.0;

			}
			if ( true == AlgParam.GetAlgParamObjectMeasure().omVolumeEnabled )
			{	
				uResultVolume_Idx = (unsigned int)(WndIdx);
				double dSpec=AlgParam.GetAlgParamObjectMeasure().omVolumeSpec;				
				dResultVolume = AlgParam.GetAlgParamObjectMeasure().omVolumeReading;
				dResultVolume_Spec = AlgParam.GetAlgParamObjectMeasure().omVolumeSpec;
				dResultVolume_USL = AlgParam.GetAlgParamObjectMeasure().omVolumeUSL*dSpec/100.0;
				dResultVolume_LSL = AlgParam.GetAlgParamObjectMeasure().omVolumeLSL*dSpec/100.0;
			}
			if ( true == AlgParam.GetAlgParamObjectMeasure().omSizeXEnabled )
			{	
				uResultWidth_Idx = (unsigned int)(WndIdx);
				double dSpec=AlgParam.GetAlgParamObjectMeasure().omSizeXSpec;
				dResultWidth = AlgParam.GetAlgParamObjectMeasure().omSizeXReading;	
				dResultWidth_Spec = AlgParam.GetAlgParamObjectMeasure().omSizeXSpec;
				switch ( AlgParam.GetAlgParamObjectMeasure().omSizeCalcUnitMode )
				{
				case ALG_CALC_UNIT_ABS://um					
				case ALG_CALC_UNIT_DIFF://um
					dResultWidth_USL = AlgParam.GetAlgParamObjectMeasure().omSizeXDiffUSL+dSpec;
					dResultWidth_LSL = AlgParam.GetAlgParamObjectMeasure().omSizeXDiffLSL+dSpec;
					break;
				case ALG_CALC_UNIT_RATIO://%
					dResultWidth_USL = AlgParam.GetAlgParamObjectMeasure().omSizeXRatioUSL*dSpec/100.0;
					dResultWidth_LSL = AlgParam.GetAlgParamObjectMeasure().omSizeXRatioLSL*dSpec/100.0;
					break;
				}
			}
			if ( true == AlgParam.GetAlgParamObjectMeasure().omSizeYEnabled )
			{	
				uResultLength_Idx = (unsigned int)(WndIdx);
				double dSpec=AlgParam.GetAlgParamObjectMeasure().omSizeYSpec;
				dResultLength = AlgParam.GetAlgParamObjectMeasure().omSizeYReading;	
				dResultLength_Spec = AlgParam.GetAlgParamObjectMeasure().omSizeYSpec;
				switch ( AlgParam.GetAlgParamObjectMeasure().omSizeCalcUnitMode )
				{
				case ALG_CALC_UNIT_ABS://um					
				case ALG_CALC_UNIT_DIFF://um
					dResultLength_USL = AlgParam.GetAlgParamObjectMeasure().omSizeYDiffUSL+dSpec;
					dResultLength_LSL = AlgParam.GetAlgParamObjectMeasure().omSizeYDiffLSL+dSpec;
					break;
				case ALG_CALC_UNIT_RATIO://%
					dResultLength_USL = AlgParam.GetAlgParamObjectMeasure().omSizeYRatioUSL*dSpec/100.0;
					dResultLength_LSL = AlgParam.GetAlgParamObjectMeasure().omSizeYRatioLSL*dSpec/100.0;
					break;
				}
			}			
			break;
		case ALG_MODEL_MATCH:			
		case ALG_IMAGE_MATCH:			
		case ALG_FD_MATCH:
		case ALG_EDGE_SEARCH:
			if ( WND_DEFECT_PART_ALIGN==WndDefectID || WND_DEFECT_LEAD_ADJUST==WndDefectID )
			{
				if ( INVALID_DOUBLE==dSkewA && true==AlgParam.GetAlgSkewEnabled() )
				{	
					uSkewA_Idx = (unsigned int)(WndIdx);
					dSkewA = AlgParam.GetAlgSkewReading();	
					dSkewA_USL = AlgParam.GetAlgSkewUSL();
					dSkewA_LSL = AlgParam.GetAlgSkewLSL();
				}
				if ( INVALID_DOUBLE==dOffsetX && true==AlgParam.GetAlgOffsetXEnabled() )
				{	
					uOffsetX_Idx = (unsigned int)(WndIdx);
					dOffsetX = AlgParam.GetAlgOffsetXReading();	
					dOffsetX_USL = AlgParam.GetAlgOffsetXUSL();
					dOffsetX_LSL = AlgParam.GetAlgOffsetXLSL();
				}
				if ( INVALID_DOUBLE==dOffsetY && true==AlgParam.GetAlgOffsetYEnabled() )
				{	
					uOffsetY_Idx = (unsigned int)(WndIdx);
					dOffsetY = AlgParam.GetAlgOffsetYReading();	
					dOffsetY_USL = AlgParam.GetAlgOffsetYUSL();
					dOffsetY_LSL = AlgParam.GetAlgOffsetYLSL();
				}
				if ( INVALID_DOUBLE==dOffsetA && true==AlgParam.GetAlgOffsetAEnabled() )
				{	
					uOffsetA_Idx = (unsigned int)(WndIdx);
					dOffsetA = AlgParam.GetAlgOffsetAReading();
					dOffsetA_USL = AlgParam.GetAlgOffsetAUSL();
					dOffsetA_LSL = AlgParam.GetAlgOffsetALSL();
				}
			}
			break;		
		}

		//Group Compare
		if ( FRAME_UNIQUE_ID_DLP==FrameUniqueID )
		{
			if ( true == AlgParam.GetAlgParamGroupCompare().gcTiltAngleEnabled )
			{
				if ( INVALID_DOUBLE==dTiltA || WND_DEFECT_BODY_TILT==WndDefectID )
				{	
					uTiltA_Idx = (unsigned int)(WndIdx);
					dTiltA = AlgParam.GetAlgParamGroupCompare().gcTiltAngleReading;	
					dTiltA_USL = AlgParam.GetAlgParamGroupCompare().gcTiltAngleUSL;	
					dTiltA_LSL = AlgParam.GetAlgParamGroupCompare().gcTiltAngleLSL;	
				}
			}
		}
	}

	if ( INVALID_DOUBLE != dTiltA )
	{			
		tA.sSpec = 0.0;
		tA.sUSL = dTiltA_USL;
		tA.sLSL = dTiltA_LSL;		
		tA.sValue = dTiltA;
		tA.sIndex = uTiltA_Idx;		
	}

	if ( INVALID_DOUBLE != dSkewA )
	{	
		sA.sSpec = 0.0;
		sA.sUSL = dSkewA_USL;
		sA.sLSL = dSkewA_LSL;
		sA.sValue = dSkewA;
		sA.sIndex = uSkewA_Idx;
	}

	if ( INVALID_DOUBLE != dOffsetX )
	{	
		sX.sSpec = 0.0;
		sX.sUSL = dOffsetX_USL;
		sX.sLSL = dOffsetX_LSL;
		sX.sValue = dOffsetX;
		sX.sIndex = uOffsetX_Idx;
	}

	if ( INVALID_DOUBLE != dOffsetY )
	{	
		sY.sSpec = 0.0;
		sY.sUSL = dOffsetY_USL;
		sY.sLSL = dOffsetY_LSL;
		sY.sValue = dOffsetY;
		sY.sIndex = uOffsetY_Idx;
	}
	
	if ( INVALID_DOUBLE != dOffsetA )
	{	
		oA.sSpec = 0.0;
		oA.sUSL = dOffsetA_USL;
		oA.sLSL = dOffsetA_LSL;
		oA.sValue = dOffsetA;
		oA.sIndex = uOffsetA_Idx;
	}

	if ( INVALID_DOUBLE != dResultHeight )
	{			
		rH.sUSL = dResultHeight_USL;
		rH.sLSL = dResultHeight_LSL;
		rH.sSpec = dResultHeight_Spec;
		rH.sValue = dResultHeight;
		rH.sIndex = uResultHeight_Idx;
	}

	if ( INVALID_DOUBLE != dResultArea )
	{		
		rA.sUSL = dResultArea_USL;
		rA.sLSL = dResultArea_LSL;
		rA.sSpec = dResultArea_Spec;
		rA.sValue = dResultArea;
		rA.sIndex = uResultArea_Idx;		
	}

	if ( INVALID_DOUBLE != dResultVolume )
	{			
		rV.sUSL = dResultVolume_USL;
		rV.sLSL = dResultVolume_LSL;
		rV.sSpec = dResultVolume_Spec;
		rV.sValue = dResultVolume;
		rV.sIndex = uResultVolume_Idx;
	}
	
	if ( INVALID_DOUBLE != dResultWidth )
	{			
		szW.sUSL = dResultWidth_USL;
		szW.sLSL = dResultWidth_LSL;
		szW.sSpec = dResultWidth_Spec;
		szW.sValue = dResultWidth;
		szW.sIndex = uResultWidth_Idx;
	}

	if ( INVALID_DOUBLE != dResultLength )
	{			
		szH.sUSL = dResultLength_USL;
		szH.sLSL = dResultLength_LSL;
		szH.sSpec = dResultLength_Spec;
		szH.sValue = dResultLength;
		szH.sIndex = uResultLength_Idx;
	}

	if ( INVALID_DOUBLE != dResultHeightMax )
	{	rH.sValueMax = dResultHeightMax;	}
	if ( INVALID_DOUBLE != dResultHeightMin )
	{	rH.sValueMin = dResultHeightMin;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CheckModelUseDataModel() const//取得是否使用資料模型
{	
	const double AttachedAngle = GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( true == IsExceptionAngle ) 
	{	return false; }
	return m_ModelDataModelEnabled;	
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::GetModelDataModelEnabled() const//取得資料模型啟用
{
	return m_ModelDataModelEnabled;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelDataModelEnabled(bool val)//設定資料模型啟用
{
	m_ModelDataModelEnabled = val;
}
//-------------------------------------------------------------------------------------//
int CAOIModel::GetModelDataModelLevelID() const//取得資料模型等級
{
	return m_ModelDataModelLevelID;
}
//-------------------------------------------------------------------------------------//
void CAOIModel::SetModelDataModelLevelID(int val)//設定資料模型等級
{
	m_ModelDataModelLevelID = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelDataModelParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TDataModelParam &Param)//建立虛擬模型參數
{	
	RECT MaskRect;
	TPOINT2D  Scale;
	TREGION4D BoxRgn, ModelRgn;
	TPOINT2D PatRgnCp, PatImageCp;
	const CAOIBox &BodyBoxRef=GetModelBodyBox();
	GetModelTotalRegion(ModelRgn);
	BodyBoxRef.GetBoxRegion(BoxRgn);

	const double RgnCpX=ModelRgn.GetCpX();
	const double RgnCpY=ModelRgn.GetCpY();
	const double RgnW=ModelRgn.GetWidth();
	const double RgnH=ModelRgn.GetHeight();
	PatRgnCp.x = RgnCpX;
	PatRgnCp.y = RgnCpY;
	PatImageCp.x = (ImageW*0.5);
	PatImageCp.y = (ImageH*0.5);
	Scale.x = ImageW/RgnW;
	Scale.y = ImageH/RgnH;

	if ( CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, PatRgnCp, Scale, PatImageCp, MaskRect) == false ) 
	{	return false; }		

	Param.PosX = (MaskRect.left+MaskRect.right)/2;
	Param.PosY = (MaskRect.top+MaskRect.bottom)/2;
	Param.SizeX = MaskRect.right-MaskRect.left;
	Param.SizeY = MaskRect.bottom-MaskRect.top;
	Param.SizeZ = GetModelBodyHeight();		
	Param.DataLevel = GetModelDataModelLevelID();//資料模型等級

	Param.ShapeMode = BodyBoxRef.GetBoxShapeMode();//BOX_SHAPE_RECTANGLE, BOX_SHAPE_ROUND_RECT, BOX_SHAPE_ELLIPSE, BOX_SHAPE_CAPSULE, BOX_SHAPE_BULLET, BOX_SHAPE_HALF_ROUND_RECT, BOX_SHAPE_T_SHAPE
	Param.ShapeParam= BodyBoxRef.GetBoxShapeParam();
	Param.ShapeParam2= BodyBoxRef.GetBoxShapeParam2();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelAIWndList(std::vector<CAOIWnd*> &List)//建立模組AI視窗列表
{
	CAOIWnd   *WndPtr = NULL;
	CAOIModel *ModelPtr = this;	
	const size_t WndCount = GetModelWndCount();

	List.clear();
	for ( size_t i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->CheckWndAlgAISupported() == false ) { continue; }
		const TALG_PARAM_AI_MODEL &aiParam=WndPtr->GetWndAlgParam().GetAlgParamAiModel();
		if ( ALG_AI_MODEL_NONE == aiParam.aiModelID ) { continue; }
		List.push_back(WndPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::BuildModelAIWndDefectList(std::vector<CAOIWnd*> &List)//建立模組AI視窗瑕疵列表
{	//return BuildModelAIWndList(List);	
	CAOIWnd   *WndPtr = NULL;
	CAOIModel *ModelPtr = this;	
	const size_t WndCount = GetModelWndCount();

	List.clear();
	for ( size_t i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->CheckWndAlgAISupported() == false ) { continue; }
		const TALG_PARAM_AI_MODEL &aiModelParam=WndPtr->GetWndAlgParam().GetAlgParamAiModel();
		if ( ALG_AI_MODEL_NONE == aiModelParam.aiModelID ) { continue; }
		
		RESULT_ID WndResultID = WndPtr->GetWndResultID();
		//RESULT_ID WndLogicResultID = WndPtr->GetWndLogicResultID();		

		bool bDefect=false;
		switch ( WndResultID )
		{
		case RESULT_ID_NONE:
		case RESULT_ID_OK:
		case RESULT_ID_BYPASS:
		case RESULT_ID_SKIP:
			bDefect = false;
			break;
		case RESULT_ID_NG:
		case RESULT_ID_EXCEPTION:
			bDefect = true;
			break;
		}
		//bDefect = true;
		if ( false == bDefect ) { continue; }
		List.push_back(WndPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::ExecModelAIInspectionKernel(std::vector<TUNI_FRAME> &UniFrameList)//執行模組Ai檢測
{
#ifdef AI_MODEL_USE
	if ( GetAIServerIsReady() == false ) { return true; }
	const char fnName[]="CAOIModel::ExecModelAIInspectionKernel";
	const size_t UniFrameCount=UniFrameList.size();
	if ( 0 == UniFrameCount ) { return false; }	

	size_t i=0;
	CAOIWnd *WndPtr=NULL;	
	std::vector<CAOIWnd*> WndList;
	const double AttachedAngle=GetModelAttachedAngle();
	CAOIComponent *ComponentPtr=GetModelComponentPtr();
	if ( NULL == ComponentPtr ) { return true; }
	CAOIProject *ProjectPtr = ComponentPtr->GetComponentProjectPtr();
	if ( NULL == ProjectPtr ) { return true; }
	CString DateTime=ProjectPtr->GetProjectInspectionDateTime();
	if ( BuildModelAIWndDefectList(WndList) == false ) { return false; }
	const size_t WndCount=WndList.size();	
	if ( 0 == WndCount ) { return true; }
	
	std::vector<CAOIWnd*> AiWndList;
	std::vector<TUNI_FRAME> AiUniFrameList;	
	CAOIModel *AiModelPtr = CloneModelObj();		
	if ( NULL == AiModelPtr ) { return false; }		
	AiModelPtr->RotateModel(-AttachedAngle, 0, 0);	
	AiModelPtr->BuildModelAIWndDefectList(AiWndList);
	if ( ImageAPI.RotateUniImageList(-AttachedAngle, UniFrameList, 4, fnName, AiUniFrameList) == false )
	{
		AOIObjManager.DestroyModelObj(AiModelPtr);
		return false;
	}
	const size_t AiUniFrameCount=AiUniFrameList.size();
	if ( 0 == AiUniFrameCount )
	{		
		AOIObjManager.DestroyModelObj(AiModelPtr);		
		return false;
	}
	TUNI_FRAME AiUniFrame=AiUniFrameList[0];
	IMAGE_SIZE AiImageW = AiUniFrame.ImageW;
	IMAGE_SIZE AiImageH = AiUniFrame.ImageH;
	//是否需要重新計算呢??	

	const bool   bReverse = true;
	const bool   bEnhance = false;
	const bool   bSave3D = false;
	const bool   bAppend = false;	
	const double SpaceRatio = 50;

	const bool bUseDateTime=true;
	CString strImgExtName=_T("JPG");
	CString strFilename, strSyncName;
	CString strFolderSend=GetAIServerFolderSend();
	CString strFolderRecv=GetAIServerFolderRecv();
	CString strKeyname=ComponentPtr->GetComponentFullName();
	if ( true == bUseDateTime )
	{	strKeyname.Format(_T("%s_%s"), DateTime, ComponentPtr->GetComponentFullName()); }
	strFilename.Format(_T("%s\\%s.%s"), strFolderSend, strKeyname, strImgExtName);	
	if ( ImageAPI.SaveUniFrameImage(strFilename, AiUniFrameList, bReverse, bEnhance, bSave3D, bAppend, SpaceRatio) == false )
	{			
		JetAPI::ClearUniFrameList(AiUniFrameList);
		AOIObjManager.DestroyModelObj(AiModelPtr);
		SetModelAiWndListResultID(WndList, RESULT_ID_EXCEPTION, _T("AI-Save Model Image Fault"));
		return false; 
	}	

	strFilename.Format(_T("%s\\%s.JSON"), strFolderSend, strKeyname);	
	JetAPI::GetSyncFilename(strFilename, strSyncName);
	::DeleteFile(strSyncName); ::Sleep(0);
	if ( AiModelPtr->SaveModelAIModelFile(strFilename, strImgExtName, AiUniFrameList) == false )
	{
		JetAPI::ClearUniFrameList(AiUniFrameList);
		AOIObjManager.DestroyModelObj(AiModelPtr);
		SetModelAiWndListResultID(WndList, RESULT_ID_EXCEPTION, _T("AI-Save AOI2AI File Fault"));
		return false; 
	}			
	
	JetAPI::CreateSyncFile(strFilename);
	::Sleep(0);	
	
	const size_t Timeout=GetAIServerTimeout();
	const size_t MaxWaitCount=(100, Timeout/10);
	const bool bAiModelDebug=GetAIServerDebugMode();
	strFilename.Format(_T("%s\\%s.JSON"), strFolderRecv, strKeyname);	
	JetAPI::GetSyncFilename(strFilename, strSyncName);	
	for ( i=0; i<MaxWaitCount; i++ )
	{
		if ( GetAIServerIsReady() == false )
		{
			AOIObjManager.DestroyModelObj(AiModelPtr);
			JetAPI::ClearUniFrameList(AiUniFrameList);
			SetModelAiWndListResultID(WndList, RESULT_ID_EXCEPTION, _T("AI-Fault (Server not ready)"));
			return false;
		}
		if ( JetAPI::IsFileExist(strSyncName) == true )
		{	break;	}
		::Sleep(10);
	}
	if ( MaxWaitCount == i )
	{	
		if ( false == bAiModelDebug )
		{	SetAIServerIsReady(false); }
		AOIObjManager.DestroyModelObj(AiModelPtr);
		JetAPI::ClearUniFrameList(AiUniFrameList);
		SetModelAiWndListResultID(WndList, RESULT_ID_EXCEPTION, _T("AI-Fault (Wait for Sync File)"));
		return false;
	}

	AiModelPtr->InitModelInspection(GetModelSelfTest());
	if ( AiModelPtr->LoadModelAIModelFile(strFilename) == false )
	{	
		AOIObjManager.DestroyModelObj(AiModelPtr);
		JetAPI::ClearUniFrameList(AiUniFrameList);
		SetModelAiWndListResultID(WndList, RESULT_ID_EXCEPTION, _T("AI-Fault (Load Json File"));
		return false;
	}

	if ( false == bAiModelDebug )
	{
		DeleteFile(strSyncName);
		DeleteFile(strFilename);
	}
	
	//再更新至原來的模組內	
	RECT AiRoiRect;
	RECT AiWndRect;
	TPOINT2D AiScale;
	TPOINT2D AiRgnCp;
	TPOINT2D AiImageCp;
	TREGION4D AiModelRgn;	
	CAOIWnd *AiWndPtr=NULL;	
	AiModelPtr->GetModelTotalRegion(AiModelRgn);
	const double AiRegionW = AiModelRgn.GetWidth();
	const double AiRegionH = AiModelRgn.GetHeight();
	const size_t AiWndCount=AiWndList.size();	
	AiRgnCp.x = AiModelRgn.GetCpX();
	AiRgnCp.y = AiModelRgn.GetCpY();
	AiImageCp.x = (AiImageW*0.5);
	AiImageCp.y = (AiImageH*0.5);
	AiScale.x = AiImageW/AiRegionW;
	AiScale.y = AiImageH/AiRegionH;
	for ( i=0; i<AiWndCount; i++ )
	{
		AiWndPtr = AiWndList[i];
		if ( NULL == AiWndPtr ) { continue; }	
		AiWndPtr->CalcWndInspectRect(AiImageW, AiImageH, AiRgnCp, AiScale, AiImageCp, AiRoiRect, AiWndRect);		
		if ( AiWndPtr->GetWndAlgParam().ExecAlgInspection_AiModel_Public(AiModelPtr, AiModelRgn, AiRoiRect, AiWndRect, AiUniFrameList) == false )
		{
			AiWndPtr->SetWndResultID(RESULT_ID_EXCEPTION);
			AiWndPtr->SetWndLogicResultID(RESULT_ID_EXCEPTION);
		}		
	}
	AiModelPtr->RotateModel(AttachedAngle, 0, 0);
	
	//再更新至原來的模組內	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = WndList[i];
		if ( NULL == WndPtr ) { continue; }
		const unsigned int WndIndex=WndPtr->GetWndIndex();
		AiWndPtr = AiModelPtr->GetModelWndPtr(WndIndex, true);
		if ( NULL == AiWndPtr ) { continue; }
		const TALG_PARAM_AI_MODEL &AiModel=AiWndPtr->GetWndAlgParam().GetAlgParamAiModel();		
		WndPtr->CloneWndResult(AiWndPtr);		
	}
	
	AOIObjManager.DestroyModelObj(AiModelPtr);
	JetAPI::ClearUniFrameList(AiUniFrameList);
#endif//AI_MODEL_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::LoadModelAIModelFile(LPCTSTR filename)//載入AI檔案
{
	bool bVal=true;
	bool bRet=false;
	int  nVal=1;
	float fVal=1.f;	
	double dVal=1.0;	
	RECT   rcVal;	
	std::wstring wsVal;	
	std::wstring wsSender;
	std::wstring wsVersion;
	std::vector<int> nList;
	std::vector<float> fList;
	std::vector<double> dList;
	std::vector<std::wstring> wsList;
	CAOIWnd   *WndPtr = NULL;
	CAOIModel *ModelPtr = this;
	rapidjson::CGMItr itr;
	std::wstring strfilename;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;		
	const int AiExceptionCode = -1;

	JetAPI::TCHAR2wstring(filename, strfilename);
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);	
	if ( JSonCtrl.OpenFile(strfilename.c_str(), Doc) == false )
	{	return false; }

	if ( JSonCtrl.ReadDocItr(Doc, L"HeaderData", itr) == false )		
	{	return false;	}
	if ( JSonCtrl.ReadObjString(itr, L"Version_s", wsVersion) == false )
	{	return false; }
	if ( JSonCtrl.ReadObjString(itr, L"Sender_s", wsSender) == false )
	{	return false; }

	if ( JSonCtrl.ReadDocItr(Doc, L"AIResultW", itr) == false )		
	{	return false;	}	
	auto pName = itr->name.GetString();	
	auto arrayV = itr->value.GetArray();
	for (auto m = arrayV.Begin(); arrayV.End() != m; ++m)
	{	
		if ( m->IsObject() == false )
		{	continue; }
		
		rapidjson::CGMItr itrLv2;
		if ( JSonCtrl.ReadValInt(*m, L"WindowID", nVal) == false )//"WindowID": 0,
		{	continue;	}
		if ( AiExceptionCode == nVal )
		{	return false; }

		const int WndIdx=nVal;
		WndPtr = ModelPtr->GetModelWndPtr(WndIdx, true);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->CheckWndAlgAISupported() == false ) { continue; }
		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		TALG_PARAM_AI_MODEL &aiModelParam=AlgParam.GetAlgParamAiModel();
		if ( ALG_AI_MODEL_NONE == aiModelParam.aiModelID ) { continue; }

		if ( JSonCtrl.ReadValInt(*m, L"IsOK", nVal) == true )//"IsOK": true,
		{
			switch ( nVal )
			{
			case AI_RESULT_ID_NONE:
			case AI_RESULT_ID_OK:
			case AI_RESULT_ID_NG:
			case AI_RESULT_ID_BYPASS:
				aiModelParam.aiResultID=(AI_RESULT_ID)(nVal);
				break;
			default:
			case AI_RESULT_ID_EXCEPTION:
				aiModelParam.aiResultID=AI_RESULT_ID_EXCEPTION;
				break;
			}			
		}

		if ( JSonCtrl.ReadValString(*m, L"ErrorMessage", wsVal) == true )//"ErrorMessage": "None",
		{	aiModelParam.aiResultText = wsVal;	}

		if ( JSonCtrl.ReadValFloat(*m, L"AIConfidenceW", fVal) == true )//"AIConfidenceW": 0.9999,
		{	aiModelParam.aiConfidence = fVal;	}		
		
		if ( JSonCtrl.ReadValItr(*m, L"AIResultB", itrLv2) == true )//AIResultB": []			
		{
			TALG_PARAM_AI_CHAR aiChar;							
			if ( itrLv2->value.IsArray() == true )
			{					
				auto arrayLv2 = itrLv2->value.GetArray();					
				for (auto m2=arrayLv2.Begin(); arrayLv2.End()!=m2; ++m2)
				{
					if ( m2->IsObject() == false ) { continue; }

					if ( JSonCtrl.ReadValString(*m2, L"AIBoxClassName", wsVal) == true )//"AIBoxClassName": "P",
					{	aiChar.acChar = wsVal;	}

					if ( JSonCtrl.ReadValRect(*m2, L"AIBoxRect", rcVal) == true )//"AIBoxRect": [93,99,147,200],
					{	aiChar.acCharRect = rcVal;	}

					if ( JSonCtrl.ReadValFloat(*m2, L"AIBoxConfidence", fVal) == true )//"AIBoxConfidence": 0.999999
					{	aiChar.acCharConfidence = fVal;	}

					aiModelParam.aiOcrCharList.push_back(aiChar);
				}
			}			
		}	
		
		bRet = bRet;
	}
	bRet = bRet;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SaveModelAIModelFile(LPCTSTR filename, LPCTSTR ImgExtName, std::vector<TUNI_FRAME> &UniFrameList)//儲存AI檔案
{	
	CString        strText;
	RECT           RoiRect;
	RECT           WndRect;
	RECT           ComponentRect;	
	CString        Filename=filename;	
	CString        MainName, ExtName;
	ALG_TYPE       AlgType;
	BOX_TOWARD     WndToward;
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = this;
	TREGION4D      Region;	
	const size_t   FrameCount=UniFrameList.size();
	const CAOIBox &BodyBox=ModelPtr->GetModelBodyBox();	
	const size_t   szBuffer = 256;
	wchar_t        strTag[szBuffer] = L"";
	wchar_t        strBuffer[szBuffer] = L"";		
	if ( 0 == FrameCount ) { return false; }
	TUNI_FRAME &UniFrameRef=UniFrameList[0];	

	TPOINT2D         WndCornerPos[4];
	TREGION4D        WndRgn, ImageRgn;
	TPOINT2D         ModelRgnCp, ImageCp, Scale;
	const IMAGE_SIZE ImageW=UniFrameRef.ImageW;
	const IMAGE_SIZE ImageH=UniFrameRef.ImageH;	
	const TREGION4D  ModelRgn = m_ModelTotalRgn;
	CalcModelInspectionParam(ImageW, ImageH, ModelRgnCp, ImageCp, Scale);	

	ExtName = ImgExtName;//_T("JPG");
	JetAPI::ExtractMainFileNameNoPath(filename, MainName);

	//AIWndList
	std::vector<CAOIWnd*> AIWndList;	
	if ( BuildModelAIWndDefectList(AIWndList) == false )
	{	return false; }
	const size_t AIWndCount=AIWndList.size();	
	
	BodyBox.GetBoxCornerPosRes(WndCornerPos);
	JetAPI::CornerPtToRegion(WndCornerPos, WndRgn);
	CalcModelBoxRegionRect(WndRgn, ImageW, ImageH, ModelRgnCp, Scale, ImageCp, ComponentRect);	

	FILE *pfile = ::_tfopen(Filename, _T("w+"));
	if ( NULL == pfile )
	{	return false;	}

	::fwprintf(pfile, L"{\n");

	::fwprintf(pfile, L"  \"HeaderData\":\n");//Header_cs
	::fwprintf(pfile, L"  {\n");//Header_cs Begin
	//"Version_s": "1.0.2",
	::wcscpy(strTag, L"Version_s");	
	::wcscpy(strBuffer, AI_MODEL_VERSION);	
	::fwprintf(pfile, L"    \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Sender_s");	
	::wcscpy(strBuffer, AOI3D_APP_NAME_W);//L"JET8000"	
	::fwprintf(pfile, L"    \"%s\": \"%s\"\n", strTag, strBuffer);
	::fwprintf(pfile, L"  },\n");//Header_cs End

	::fwprintf(pfile, L"  \"%s\":\n", L"AIInferenceDataW");//AIInferenceDataW
	::fwprintf(pfile, L"  [\n");//AIInferenceDataW Begin
	for ( size_t i=0; i<AIWndCount; i++ )
	{
		WndPtr = AIWndList[i];
		if ( NULL == WndPtr ) { continue; }		
		AlgType = WndPtr->GetWndAlgType();
		WndToward = WndPtr->GetWndToward();
		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		//if ( WndPtr->GetWndExtendBoxUsed() == false )
		//{	WndPtr->GetWndRegionRes(WndRgn);	}
		//else
		//{	WndPtr->GetWndExtendBox().GetBoxRegionRes(WndRgn);	}
		if ( WndPtr->GetWndExtendBoxUsed() == false )
		{	WndPtr->GetWndCornerPosRes(WndCornerPos);	}
		else
		{	WndPtr->GetWndExtendBox().GetBoxCornerPosRes(WndCornerPos);	}
		JetAPI::CornerPtToRegion(WndCornerPos, WndRgn);
		CalcModelBoxRegionRect(WndRgn, ImageW, ImageH, ModelRgnCp, Scale, ImageCp, WndRect);
		JetAPI::BoundaryRect(ImageW, ImageH, WndRect);

		const CAlgBinaryParam &BinParam =AlgParam.GetAlgImageBinParam();
		const TALG_PARAM_AI_MODEL &aiParam=AlgParam.GetAlgParamAiModel();		

		if ( 0 != i )
		{	::fwprintf(pfile, L",\n");	}
		
		::fwprintf(pfile, L"    {\n");//Wnd Begin
		
		//"ComponentImageFileName": "PanelID_BoardID_ComponentName_ImgID.jpg"				
		::wcscpy(strTag, L"ComponentImageFileName");
		strText.Format(_T("%s#%d.%s"), MainName, BinParam.GetBinaryFrameIndex()+1, ExtName);		
		JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
		::fwprintf(pfile, L"      \"%s\": \"%s\",\n", strTag, strBuffer);

		//"ROI":
		::fwprintf(pfile, L"      \"%s\":\n", L"Rect");
		::fwprintf(pfile, L"      [\n");//Rect Begin
		//"window rect": [0,0,342,282],		
		::fwprintf(pfile, L"        [%d, %d, %d, %d],\n", WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

		//"Component rect": [0,0,342,282],		
		::fwprintf(pfile, L"        [%d, %d, %d, %d]\n", ComponentRect.left, ComponentRect.top, ComponentRect.right, ComponentRect.bottom);
		::fwprintf(pfile, L"      ],\n");//Rect End

		//"AImodelID": 0
		::wcscpy(strTag, L"AImodelID");
		::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, aiParam.aiModelID);

		//"WindowDirection": 1,
		::wcscpy(strTag, L"WindowDirection");
		::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, WndPtr->GetWndAlgParam().GetAlgPatternPolarity());

		//"ConfidenceThreshold": 1,
		::wcscpy(strTag, L"ConfidenceThreshold");
		::fwprintf(pfile, L"      \"%s\": %.2f,\n", strTag, aiParam.aiConfidenceThreshold/100.0f);

		//"OCRImageRotateAngle": 0,
		::wcscpy(strTag, L"OCRImageRotateAngle");
		const int nPatternAngle=(int)(JetAPI::AdjustRotationAngle(aiParam.aiPatternAngle));
		::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, nPatternAngle);

		//"CharMatchNumThreshold":65535//字元相符數量閥值
		::wcscpy(strTag, L"CharMatchNumThreshold");
		::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, aiParam.aiCharMatchNumThreshold);

		//"CharNumUpperThreshold":65535;//字元數量上限閥值
		::wcscpy(strTag, L"CharNumUpperThreshold");
		::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, aiParam.aiCharNumUpperThreshold);

		bool bInputStringMode=false;
		std::vector<RECT> CharRectList;
		std::vector<std::wstring> OcrTextList;
		if ( ALG_CHAR_VERIFY == AlgType )
		{	
			const int PolarityIdx = 0;
			AlgParam.GetAlgPatternParamPatTextList(OcrTextList);
			CPatternParam *PatternParamPtr=AlgParam.GetAlgPatternParamPtrByMaxPatRoiCount(WndToward, PolarityIdx);			
			if ( NULL != PatternParamPtr )
			{	
				bInputStringMode = PatternParamPtr->GetPatInputStringMode();
				const std::vector<TPATTERN_ROI> &PatRoiList=PatternParamPtr->GetPatRoiList(WndToward, PolarityIdx);				
				const size_t PatRoiCount=PatRoiList.size();
				for ( size_t j=0; j<PatRoiCount; j++ )
				{
					const TPATTERN_ROI &PatRoi=PatRoiList[j];
					CharRectList.push_back(PatRoi.RoiRect);
				}
			}			
		}
		const size_t OcrTextCount=OcrTextList.size();
		//"OCRGroundTruthStr": []
		::wcscpy(strTag, L"OCRGroundTruthStr");
		::fwprintf(pfile, L"      \"%s\":\n", strTag);
		::fwprintf(pfile, L"      [\n");//OCRGroundTruthStr Begin
		for ( size_t j=0; j<OcrTextCount; j++ )
		{
			if ( 0 != j )
			{	::fwprintf(pfile, L",\n");	}
			::fwprintf(pfile, L"        \"%s\"", OcrTextList[j].c_str());				
		}
		::fwprintf(pfile, L"\n");		
		::fwprintf(pfile, L"      ],\n");//OCRGroundTruthStr End

		size_t CharRectCount=0;
		if ( false == bInputStringMode )
		{	CharRectCount=CharRectList.size(); }
		::wcscpy(strTag, L"OCRGroundTruthBox");//"OCRGroundTruthBox": []
		::fwprintf(pfile, L"      \"%s\":\n", strTag);
		::fwprintf(pfile, L"      [\n");//OCRGroundTruthBox Begin
		for ( size_t j=0; j<CharRectCount; j++ )
		{
			if ( 0 != j )
			{	::fwprintf(pfile, L",\n");	}
			const RECT &rc=CharRectList[j];
			::fwprintf(pfile, L"        [%d, %d, %d, %d]", rc.left, rc.top, rc.right, rc.bottom);
		}
		::fwprintf(pfile, L"\n");		
		::fwprintf(pfile, L"      ],\n");//OCRGroundTruthBox End		

		//"WindowID": 0,
		::wcscpy(strTag, L"WindowID");
		::fwprintf(pfile, L"      \"%s\": %d\n", strTag, WndPtr->GetWndIndex());

		::fwprintf(pfile, L"    }");//Wnd End
	}
	::fwprintf(pfile, L"\n");
	::fwprintf(pfile, L"  ]\n");//AIInferenceDataW End		
	
	::fwprintf(pfile, L"}\n");
	::fclose(pfile); pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::SetModelAiWndListResultID(std::vector<CAOIWnd*> &List, RESULT_ID ResultID, LPCTSTR RexultText)//設定模組AI檢測結果
{
	CAOIWnd *WndPtr = NULL;
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		WndPtr = List[i];
		if ( NULL == WndPtr ) { continue; }
		CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
		AlgParam.SetAlgResultID(ResultID);
		AlgParam.SetAlgResultText(RexultText);
		WndPtr->SetWndResultID(ResultID);
		WndPtr->SetWndResultText(RexultText);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelImageRect_CustomerAI(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH)//計算模組影像區域-客戶AI;
{	
	CAOIRgn* RgnPtr=GetModelAttachedPtr();
	if ( NULL == RgnPtr ) { return false; }

	TREGION4D rgnTotal, rgnBodyLand;
	GetModelTotalRegion(rgnTotal);
	GetModelBodyLandRegion(rgnBodyLand);
	const double AIExtendum=150;
	const double ModelW=rgnTotal.GetWidth();
	const double ModelH=rgnTotal.GetHeight();
	const double ScaleX=(ImageW/ModelW);
	const double ScaleY=(ImageH/ModelH);
	rgnBodyLand.minX -= AIExtendum;
	rgnBodyLand.minY -= AIExtendum;
	rgnBodyLand.maxX += AIExtendum;
	rgnBodyLand.maxY += AIExtendum;

	RECT AiRect;
	AiRect.left   = (int)((rgnBodyLand.minX-rgnTotal.minX)*ScaleX);
	AiRect.top    = (int)((rgnBodyLand.minY-rgnTotal.minY)*ScaleY);
	AiRect.right  = (int)((rgnBodyLand.maxX-rgnTotal.minX)*ScaleX);
	AiRect.bottom = (int)((rgnBodyLand.maxY-rgnTotal.minY)*ScaleY);
	if ( AiRect.left < 0 ) { AiRect.left = 0; }
	if ( AiRect.top < 0 ) { AiRect.top = 0; }
	if ( AiRect.right > ImageW ) { AiRect.right = ImageW; }
	if ( AiRect.bottom > ImageH ) { AiRect.bottom = ImageH; }
	RgnPtr->SetRgnModelImageRect_AI(AiRect);

	AOI_CUSTOMER_ID CustomerID = AOIDataCollect.GetAOICustomerID();
	if ( AOI_CUSTOMER_ID_FOXCONN_LONGHUA != CustomerID )	
	{
		JetAPI::SizeToRect(ImageW, ImageH, AiRect);
		RgnPtr->SetRgnModelImageRect_AI(AiRect);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIModel::CalcModelBodyOutsideParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TBasePlaneParam &BasePlaneParam)//計算模組本體外圍資料
{		
	if ( BASE_PLANE_BODY_OUTSIDE_DISABLE == BasePlaneParam.BodyOutsideMode ) { return true; }
	CAOIComponent *ComponentPtr=GetModelComponentPtr();
	if ( NULL == ComponentPtr ) { return false; }	
	
	RECT BodyRect;
	TREGION4D BodyRgn;	
	TPOINT2D   RgnCp, Scale, ImageCp;
	const double AttachedAngle=ComponentPtr->GetComponentAngle();
	const bool IsExceptionAngle=JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( true == IsExceptionAngle )
	{	return true; }
	switch ( BasePlaneParam.BodyOutsideMode )
	{
	default:
	case BASE_PLANE_BODY_OUTSIDE_BODY:		
		if ( false == IsExceptionAngle )
		{	GetModelBodyRegion(BodyRgn);	}
		else
		{
			TPOINT2D CornerPos[4];
			const CAOIBox &BodyBox=GetModelBodyBox();
			BodyBox.GetBoxCornerPos(CornerPos);
			JetAPI::CornerPtToRegion(CornerPos, BodyRgn);	
		}
		break;
	case BASE_PLANE_BODY_OUTSIDE_BODY_LAND:	
		GetModelBodyLandRegion(BodyRgn);	
		break;
	}
	CalcModelInspectionParam(ImageW, ImageH, RgnCp, ImageCp, Scale, true);	
	CalcModelBoxRegionRect(BodyRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, BodyRect);	
	if ( BodyRect.left<0 || BodyRect.top<0 || BodyRect.right>ImageW || BodyRect.bottom>ImageH )
	{	return false; }
	BasePlaneParam.SetBasePlaneBodyRect(BodyRect);	
	BasePlaneParam.BodyOutsideWPxl=JetAPI::ToInt(BasePlaneParam.BodyOutsideW*Scale.x);
	BasePlaneParam.BodyOutsideHPxl=JetAPI::ToInt(BasePlaneParam.BodyOutsideH*Scale.y);	
	return true;
}
//-------------------------------------------------------------------------------------//