// Light3DCtrl.cpp: implementation of the CLight3DCtrl class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DCtrl.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CLight3DCtrl Light3DCtrl;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::CheckBypassLight3D(LIGHT_3D_CAST_ID CastID)
{
#ifdef BYPASS_DLP1_USE
	if ( LIGHT_3D_CAST_01 == CastID ) { return true; }
#endif//BYPASS_DLP1_USE

#ifdef BYPASS_DLP2_USE
	if ( LIGHT_3D_CAST_02 == CastID ) { return true; }
#endif//BYPASS_DLP2_USE

#ifdef BYPASS_DLP3_USE
	if ( LIGHT_3D_CAST_03 == CastID ) { return true; }
#endif//BYPASS_DLP3_USE

#ifdef BYPASS_DLP4_USE
	if ( LIGHT_3D_CAST_04 == CastID ) { return true; }
#endif//BYPASS_DLP4_USE
	return false;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CAST_ID CLight3DCtrl::GetLight3DCastIDByIndex(size_t index)//眔3Dщ紇絪腹
{
	LIGHT_3D_CAST_ID CastID=LIGHT_3D_CAST_00;
	switch ( index )
	{
	case 0:	CastID = LIGHT_3D_CAST_01;	break;
	case 1:	CastID = LIGHT_3D_CAST_02;	break;
	case 2:	CastID = LIGHT_3D_CAST_03;	break;
	case 3:	CastID = LIGHT_3D_CAST_04;	break;
	case 4:	CastID = LIGHT_3D_CAST_05;	break;
	case 5:	CastID = LIGHT_3D_CAST_06;	break;
	case 6:	CastID = LIGHT_3D_CAST_07;	break;
	case 7:	CastID = LIGHT_3D_CAST_08;	break;
	}
	return CastID;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CAST_ID CLight3DCtrl::GetLight3DCastIDByDLPChannel(size_t Channel)//眔3Dщ紇絪腹
{
	LIGHT_3D_CAST_ID CastID=LIGHT_3D_CAST_00;
	switch ( Channel )
	{
	case 1:	CastID = LIGHT_3D_CAST_01;	break;
	case 2:	CastID = LIGHT_3D_CAST_02;	break;
	case 3:	CastID = LIGHT_3D_CAST_03;	break;
	case 4:	CastID = LIGHT_3D_CAST_04;	break;
	case 5:	CastID = LIGHT_3D_CAST_05;	break;
	case 6:	CastID = LIGHT_3D_CAST_06;	break;
	case 7:	CastID = LIGHT_3D_CAST_07;	break;
	case 8:	CastID = LIGHT_3D_CAST_08;	break;
	}
	return CastID;
}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DCtrl::GetLight3DIndexByCastID(LIGHT_3D_CAST_ID CastID)//眔3Dщ紇ま计
{
	unsigned int index = -1;
	switch ( CastID )
	{
	case LIGHT_3D_CAST_01:	index = 0;	break;
	case LIGHT_3D_CAST_02:	index = 1;	break;
	case LIGHT_3D_CAST_03:	index = 2;	break;
	case LIGHT_3D_CAST_04:	index = 3;	break;
	case LIGHT_3D_CAST_05:	index = 4;	break;
	case LIGHT_3D_CAST_06:	index = 5;	break;
	case LIGHT_3D_CAST_07:	index = 6;	break;
	case LIGHT_3D_CAST_08:	index = 7;	break;	
	}
	return index;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::BuildCastIDCombox(CComboBox &Combox, bool bAddAll)//ミщ甮絪腹跌怠
{
	CString str;
	int     idx=0;	
	const int DlpMaxCount = DLP_CAST_COUNT;
	LIGHT_3D_CAST_ID Light3DID=LIGHT_3D_CAST_00;

	JetAPI::ClearCombox(Combox);
	for ( int i=0; i<DlpMaxCount; i++ )
	{		
		str.Format(_T("%d"), i+1);
		Light3DID = GetLight3DCastIDByIndex(i);
		if ( LIGHT_3D_CAST_00 == Light3DID )
		{	continue; }
		if ( CheckBypassLight3D(Light3DID) == true )
		{	continue; }
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Light3DID);
		idx ++;
	}	

	if ( true == bAddAll )
	{
		str = _T("Closed");
		Light3DID=LIGHT_3D_CAST_00;
		Combox.InsertString(-1, str);
		Combox.SetItemData(idx, Light3DID);
		idx ++;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetDLPExposureMinTime(bool Internal, bool MultiTable, int NFrames)//眔程眎计丁
{
	//DLPず场牟祇程祏丁3000us
	//DLP场牟祇程祏丁3000usx2=6000us	
	//Ω牟祇妓狾, 程胣丁6000us-ぃ阶琌ず场牟祇
	//Ω牟祇妓狾, 程胣丁3000us-ぃ阶琌ず场牟祇
	//㏄戳丁惠璶胣丁, 玥礚猭糶

	int TriggerPeriodus = 1000;//	
	if ( true == MultiTable )//家Α, –妓狾牟祇癟腹
	{	TriggerPeriodus = 3000;	}
	else//虫家Α, –妓狾牟祇癟腹
	{
		if ( NFrames > 1 ) 
		{	TriggerPeriodus = 6000;	 }
		else
		{	TriggerPeriodus = 3000;	 }
	}
	return TriggerPeriodus;
}
//-------------------------------------------------------------------------------------//
CLight3DCtrl::CLight3DCtrl()
{
	CLight3DCtrl::PreInitLight3DCtrl();
	CLight3DCtrl::InitialLight3DCtrl();
}
//-------------------------------------------------------------------------------------//
CLight3DCtrl::~CLight3DCtrl()
{
	CLight3DCtrl::ReleaseLight3DCast();
}
//-------------------------------------------------------------------------------------//
inline void CLight3DCtrl::PreInitLight3DCtrl()//箇﹍て北
{
	this->m_Light3DCast01 = NULL;
	this->m_Light3DCast02 = NULL;
	this->m_Light3DCast03 = NULL;
	this->m_Light3DCast04 = NULL;
}
//-------------------------------------------------------------------------------------//
inline void CLight3DCtrl::InitialLight3DCtrl()//﹍て北
{
	m_PatternBitCount=6;//妓狾ㄏノじ计

	CLight3DCtrl::ReleaseLight3DCast();
#ifndef BYPASS_DLP1_USE
	this->m_Light3DCast01 = new LIGHT_3D_CLS(0, LIGHT_3D_CAST_01);
#endif//BYPASS_DLP1_USE

#ifndef BYPASS_DLP2_USE
	this->m_Light3DCast02 = new LIGHT_3D_CLS(0, LIGHT_3D_CAST_02);
#endif//BYPASS_DLP2_USE

#ifndef BYPASS_DLP3_USE
	this->m_Light3DCast03 = new LIGHT_3D_CLS(0, LIGHT_3D_CAST_03);
#endif//BYPASS_DLP3_USE

#ifndef BYPASS_DLP4_USE
	this->m_Light3DCast04 = new LIGHT_3D_CLS(0, LIGHT_3D_CAST_04);
#endif//BYPASS_DLP4_USE
}
//-------------------------------------------------------------------------------------//
inline void CLight3DCtrl::ReleaseLight3DCast()//睦北
{
	if ( NULL != m_Light3DCast01 )
	{	m_Light3DCast01->DLPDisconnect();	}

	if ( NULL != m_Light3DCast02 )
	{	m_Light3DCast02->DLPDisconnect();	}

	if ( NULL != m_Light3DCast03 )
	{	m_Light3DCast03->DLPDisconnect();	}

	if ( NULL != m_Light3DCast04 )
	{	m_Light3DCast04->DLPDisconnect();	}
//	::Sleep(2000);
	
	if ( NULL != m_Light3DCast01 )
	{	delete m_Light3DCast01; m_Light3DCast01 = NULL;	}

	if ( NULL != m_Light3DCast02 )
	{	delete m_Light3DCast02; m_Light3DCast02 = NULL;	}

	if ( NULL != m_Light3DCast03 )
	{	delete m_Light3DCast03; m_Light3DCast03 = NULL;	}

	if ( NULL != m_Light3DCast04 )
	{	delete m_Light3DCast04; m_Light3DCast04 = NULL;	}
	
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DCtrl::GetErrorString()
{
	AOIExceptionCodeCtrl.SetAOIExceptionCode_DLP_Others(m_ErrorString);
	return this->m_ErrorString;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_CLS_PTR CLight3DCtrl::GetLight3DCastPtr(LIGHT_3D_CAST_ID Light3DCastID)
{
	LIGHT_3D_CLS_PTR Ptr = NULL;
	switch ( Light3DCastID )
	{
	case LIGHT_3D_CAST_00:
		this->m_ErrorString.Format(_T("Error, Light 3D 00 is null"));
		break;
	case LIGHT_3D_CAST_01:
		Ptr = m_Light3DCast01;
		if ( NULL == Ptr )
		{	this->m_ErrorString.Format(_T("Error, Light 3D 01 is null")); }
		break;
	case LIGHT_3D_CAST_02:
		Ptr = m_Light3DCast02;
		if ( NULL == Ptr )
		{	this->m_ErrorString.Format(_T("Error, Light 3D 02 is null")); }
		break;
	case LIGHT_3D_CAST_03:
		Ptr = m_Light3DCast03;
		if ( NULL == Ptr )
		{	this->m_ErrorString.Format(_T("Error, Light 3D 03 is null")); }
		break;
	case LIGHT_3D_CAST_04:
		Ptr = m_Light3DCast04;
		if ( NULL == Ptr )
		{	this->m_ErrorString.Format(_T("Error, Light 3D 04 is null")); }
		break;
	default:
		this->m_ErrorString.Format(_T("Error, Light 3D is not defined"));
		break;
	}
	return Ptr;
}
//-------------------------------------------------------------------------------------//
void CLight3DCtrl::InitialErrorString()
{
	this->m_ErrorString = _T("");
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::BuildLight3DCastList(std::vector<LIGHT_3D_CLS_PTR> &List)
{
	List.clear();
	if ( NULL != m_Light3DCast01 )
	{	List.push_back(m_Light3DCast01); }
	if ( NULL != m_Light3DCast02 )
	{	List.push_back(m_Light3DCast02); }
	if ( NULL != m_Light3DCast03 )
	{	List.push_back(m_Light3DCast03); }
	if ( NULL != m_Light3DCast04 )
	{	List.push_back(m_Light3DCast04); }
	if ( 0 == List.size() )
	{	
		m_ErrorString = _T("Error, BuildLight3DCastList Fault");
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CLight3DCtrl::GetPatternIndexIniFilename() const
{
	CString Filename;
	CString Folder = AOIDataCollect.GetAOIDirectory();
	Filename.Format(_T("%s\\%s.INI"), Folder, _T("PatternImageList"));
	return Filename;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::LoadPatternIndexIniFile()
{
#ifndef PHASE_CTRL_DISABLE
	const int InvalidIndex = -1;
	std::map<int, int> &Map3Bit=m_PatternIndexMap_3Bit;
	std::map<int, int> &Map5Bit=m_PatternIndexMap_5Bit;	
	std::map<int, int> &Map6Bit=m_PatternIndexMap_6Bit;
	std::map<int, int> &Map8Bit=m_PatternIndexMap_8Bit;	
	int &GrayCodeIndex_1Bit=m_PatternIndexGrayCode_1Bit;
	int &BinaryCodeIndex_1Bit=m_PatternIndexBinaryCode_1Bit;
	int &GrayCodeStartNum_1Bit=m_PatternNumStartGrayCode_1Bit;
	int &BinaryCodeStartNum_1Bit=m_PatternNumStartBinaryCode_1Bit;	
	int &GrayCodeIndex_8Bit=m_PatternIndexGrayCode_8Bit;
	int &BinaryCodeIndex_8Bit=m_PatternIndexBinaryCode_8Bit;
	int &GrayCodeStartNum_8Bit=m_PatternNumStartGrayCode_8Bit;
	int &BinaryCodeStartNum_8Bit=m_PatternNumStartBinaryCode_8Bit;	

	Map3Bit.clear();
	Map5Bit.clear();
	Map6Bit.clear();
	Map8Bit.clear();
	m_PatternBitCount=6;//妓狾ㄏノじ计

	GrayCodeIndex_1Bit = -1;	
	BinaryCodeIndex_1Bit = -1;
	GrayCodeStartNum_1Bit = -1;
	BinaryCodeStartNum_1Bit = -1;	
	GrayCodeIndex_8Bit = -1;	
	BinaryCodeIndex_8Bit = -1;
	GrayCodeStartNum_8Bit = -1;
	BinaryCodeStartNum_8Bit = -1;	
	if ( ReadPatternParameter() == false )
	{	return false; }
	if ( ReadPatternIndexIniFile(3, Map3Bit) == false )
	{	return false; }
	if ( ReadPatternIndexIniFile(5, Map5Bit) == false )
	{	return false; }	
	if ( ReadPatternIndexIniFile(6, Map6Bit) == false )
	{	return false; }
	if ( ReadPatternIndexIniFile(8, Map8Bit) == false )
	{	return false; }
	if ( ReadPatternIndexIniFile_GC(1, GrayCodeIndex_1Bit, GrayCodeStartNum_1Bit) == false )
	{	return false; }
	if ( ReadPatternIndexIniFile_BC(1, BinaryCodeIndex_1Bit, BinaryCodeStartNum_1Bit) == false )
	{	return false; }
	if ( ReadPatternIndexIniFile_GC(8, GrayCodeIndex_8Bit, GrayCodeStartNum_8Bit) == false )
	{	return false; }
	if ( ReadPatternIndexIniFile_BC(8, BinaryCodeIndex_8Bit, BinaryCodeStartNum_8Bit) == false )
	{	return false; }

	const size_t Count3Bit=Map3Bit.size();
	const size_t Count5Bit=Map5Bit.size();	
	const size_t Count6Bit=Map6Bit.size();
	const size_t Count8Bit=Map8Bit.size();	

	bool bSaveIni_8Bit=false;
	if ( 0 == Count8Bit )
	{
		bSaveIni_8Bit = true;
		Map8Bit.insert(std::make_pair<int, int>(8, 0));
		Map8Bit.insert(std::make_pair<int, int>(224, 4));
	}
	if ( InvalidIndex == GrayCodeIndex_8Bit )
	{
		bSaveIni_8Bit = true;
		GrayCodeIndex_8Bit = 12;
		GrayCodeStartNum_8Bit = 0;
	}
	if ( InvalidIndex == BinaryCodeIndex_8Bit )
	{
		bSaveIni_8Bit = true;
		BinaryCodeIndex_8Bit = 19;
		BinaryCodeStartNum_8Bit = 0;
	}

	if ( 8 == m_PatternBitCount )
	{	
		if ( true == bSaveIni_8Bit )
		{	SavePatternIndexIniFile(); }
		else
		{	WritePatternParameter();	}
	}
	else
	{
		if ( 0 == Count6Bit )
		{
			Map6Bit.insert(std::make_pair<int, int>(16, 0));
			Map6Bit.insert(std::make_pair<int, int>(224, 1));
			SavePatternIndexIniFile();
		}
		else
		{	WritePatternParameter(); }
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SavePatternIndexIniFile()
{
#ifndef PHASE_CTRL_DISABLE
	const std::map<int, int> &Map3Bit=m_PatternIndexMap_3Bit;
	const std::map<int, int> &Map5Bit=m_PatternIndexMap_5Bit;	
	const std::map<int, int> &Map6Bit=m_PatternIndexMap_6Bit;
	const std::map<int, int> &Map8Bit=m_PatternIndexMap_8Bit;
	const int GrayCodeIndex_1Bit=GetPatternIndexGrayCode_1Bit();	
	const int BinaryCodeIndex_1Bit=GetPatternIndexBinaryCode_1Bit();
	const int GrayCodeStartNum_1Bit=GetPatternIndexGrayCode_1Bit();
	const int BinaryCodeStartNum_1Bit=GetPatternNumStartBinaryCode_1Bit();	
	const int GrayCodeIndex_8Bit=GetPatternIndexGrayCode_8Bit();	
	const int BinaryCodeIndex_8Bit=GetPatternIndexBinaryCode_8Bit();
	const int GrayCodeStartNum_8Bit=GetPatternNumStartGrayCode_8Bit();
	const int BinaryCodeStartNum_8Bit=GetPatternNumStartBinaryCode_8Bit();	

	if ( WritePatternParameter() == false )
	{	return false; }
	if ( WritePatternIndexIniFile(3, Map3Bit) == false )
	{	return false; }
	if ( WritePatternIndexIniFile(5, Map5Bit) == false )
	{	return false; }
	if ( WritePatternIndexIniFile(6, Map6Bit) == false )
	{	return false; }
	if ( WritePatternIndexIniFile(8, Map8Bit) == false )
	{	return false; }
	if ( WritePatternIndexIniFile_GC(1, GrayCodeIndex_1Bit, GrayCodeStartNum_1Bit) == false )
	{	return false; }
	if ( WritePatternIndexIniFile_BC(1, BinaryCodeIndex_1Bit, BinaryCodeStartNum_1Bit) == false )
	{	return false; }	
	if ( WritePatternIndexIniFile_GC(8, GrayCodeIndex_8Bit, GrayCodeStartNum_8Bit) == false )
	{	return false; }
	if ( WritePatternIndexIniFile_BC(8, BinaryCodeIndex_8Bit, BinaryCodeStartNum_8Bit) == false )
	{	return false; }	
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ReadPatternParameter()
{
	size_t  i=0;
	CString Section = _T("");	
	CString KeyName = _T("");	
	CString Default  = _T("");	
	const size_t textlen = 128;
	TCHAR   String[textlen]=_T("");	
	CString FileName = GetPatternIndexIniFilename();

	Section.Format(_T("Pattern Parameter"));

	KeyName = _T("Bit Count");	
	Default.Format(_T("%d"), m_PatternBitCount);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	m_PatternBitCount = ::_ttoi(String); }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ReadPatternIndexIniFile_GC(int BitMode, int &Index, int &StartNum)
{
	size_t  i=0;
	CString Section = _T("");	
	CString KeyName = _T("");	
	CString Default  = _T("");	
	const size_t textlen = 128;
	TCHAR   String[textlen]=_T("");	
	CString FileName = GetPatternIndexIniFilename();

	Index = -1;
	StartNum = -1;
	//Section.Format(_T("Pattern Gray Code 1Bit"));
	Section.Format(_T("Pattern Gray Code %dBit"), BitMode);

	KeyName = _T("Index");	
	Default.Format(_T("%d"), Index);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	Index = ::_ttoi(String); }	

	KeyName = _T("Start Num");	
	Default.Format(_T("%d"), StartNum);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	StartNum = ::_ttoi(String); }

	int Period = -1;
	//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024		
	KeyName = _T("Start Period");
	Default.Format(_T("%d"), Period);
	//if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	//{	Period = ::_ttoi(String); }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ReadPatternIndexIniFile_BC(int BitMode, int &Index, int &StartNum)
{
	size_t  i=0;	
	CString Section = _T("");	
	CString KeyName = _T("");	
	CString Default  = _T("");	
	const size_t textlen = 128;
	TCHAR   String[textlen]=_T("");	
	CString FileName = GetPatternIndexIniFilename();

	Index = -1;
	StartNum = -1;
	//Section.Format(_T("Pattern Binary Code 1Bit"));
	Section.Format(_T("Pattern Binary Code %dBit"), BitMode);

	KeyName = _T("Index");	
	Default.Format(_T("%d"), Index);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	Index = ::_ttoi(String); }

	KeyName = _T("Start Num");	
	Default.Format(_T("%d"), StartNum);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	StartNum = ::_ttoi(String); }

	int Period = -1;
	//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
	KeyName = _T("Start Period");//2, 4, 
	Default.Format(_T("%d"), Period);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	Period = ::_ttoi(String); }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ReadPatternIndexIniFile(int BitMode, std::map<int, int> &Map)
{
	size_t  i=0;
	int     Index=0;
	double  Period=0;
	size_t  PatternCount=0;	
	CString Section;	
	CString KeyName = _T("");	
	CString Default  = _T("");
	const size_t textlen = 128;
	std::vector<double> PeriodList;
	TCHAR   String[textlen]=_T("");	
	CString FileName = GetPatternIndexIniFilename();	
	
	Section.Format(_T("Pattern %dBit"), BitMode);	

	Map.clear();
	PatternCount = 0;
	KeyName = _T("Pattern Count");	
	Default.Format(_T("%d"), PatternCount);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
	{	PatternCount = ::_ttoi(String); }	

	PeriodList.clear();
	for ( i=0; i<PatternCount; i++ )
	{	
		Period = 0.0;
		KeyName.Format(_T("%d"), i+1);
		Default.Format(_T("%.2f"), Period);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	Period = ::_ttof(String); }	
		if ( fabs(Period) < 0.001 )
		{	continue; }
		PeriodList.push_back(Period);
	}

	Section.Format(_T("Pattern %dBit Index"), BitMode);	
	const size_t PeriodCount=PeriodList.size();
	for ( i=0; i<PeriodCount; i++ )
	{
		Index = -1;
		Period = PeriodList[i];
		KeyName.Format(_T("%.2f"), Period);
		Default.Format(_T("%d"), Index);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, FileName, false, m_ErrorString) == true ) 	
		{	Index = ::_ttoi(String); }	

		if ( Index < 0 )
		{	continue; }		

		std::pair<int, int> Node((int)(Period), Index);
		Map.insert(Node);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::WritePatternParameter()
{
	size_t  i=0;	
	CString Section = _T("");	
	CString KeyName = _T("");	
	CString String  = _T("");		
	CString FileName = GetPatternIndexIniFilename();	

	Section.Format(_T("Pattern Parameter"));

	KeyName = _T("Bit Count");	
	String.Format(_T("%d"), m_PatternBitCount);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::WritePatternIndexIniFile_GC(int BitMode, int Index, int StartNum)
{
	size_t  i=0;	
	CString Section = _T("");	
	CString KeyName = _T("");	
	CString String  = _T("");		
	CString FileName = GetPatternIndexIniFilename();

	//Section.Format(_T("Pattern Gray Code 1Bit"));
	Section.Format(_T("Pattern Gray Code %dBit"), BitMode);

	KeyName = _T("Index");	
	String.Format(_T("%d"), Index);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
	{	return false; }	

	KeyName = _T("Start Num");	
	String.Format(_T("%d"), StartNum);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
	{	return false; }		

	KeyName = _T("Start Period");//2, 4, 
	String.Format(_T("%d"), 16);
	//GrayCode 0~9:2+4+8+16+32+64+128+256+512+1024
	//if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
	//{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::WritePatternIndexIniFile_BC(int BitMode, int Index, int StartNum)
{
	size_t  i=0;	
	CString Section = _T("");	
	CString KeyName = _T("");	
	CString String  = _T("");		
	CString FileName = GetPatternIndexIniFilename();

	//Section.Format(_T("Pattern Binary Code 1Bit"));
	Section.Format(_T("Pattern Binary Code %dBit"), BitMode);

	KeyName = _T("Index");	
	String.Format(_T("%d"), Index);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
	{	return false; }	

	KeyName = _T("Start Num");	
	String.Format(_T("%d"), StartNum);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
	{	return false; }		

	KeyName = _T("Start Period");//2, 4, 
	String.Format(_T("%d"), 256);
	//BinaryCode 10~19:2+4+8+16+32+64+128+256+512+1024
	//if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
	//{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::WritePatternIndexIniFile(int BitMode, const std::map<int, int> &Map)
{
	size_t  i=0;
	int     Index=0;
	double  Period=0;	
	CString Section = _T("");
	CString KeyName = _T("");	
	CString String  = _T("");	
	const size_t  PatternCount=Map.size();
	CString FileName = GetPatternIndexIniFilename();
	
	Section.Format(_T("Pattern %dBit"), BitMode);	
	KeyName = _T("Pattern Count");	
	String.Format(_T("%d"), PatternCount);
	if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
	{	return false; }	

	i = 0;
	//纗ぶ秅戳
	for ( auto Iter=Map.begin(); Iter!=Map.end(); ++Iter )	
	{	
		Index = Iter->second;
		Period = Iter->first;
		KeyName.Format(_T("%d"), i+1);
		String.Format(_T("%.2f"), Period);
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
		{	return false; }	
		i ++;
	}
	
	Section.Format(_T("Pattern %dBit Index"), BitMode);	
	//纗–秅戳癸莱妓ま计
	for ( auto Iter=Map.begin(); Iter!=Map.end(); ++Iter )	
	{	
		Index = Iter->second;
		Period = Iter->first;
		KeyName.Format(_T("%.2f"), Period);
		String.Format(_T("%d"), Index);
		if ( SaveINIData(Section, KeyName, String, FileName, m_ErrorString) == false ) 	
		{	return false; }			
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternBitCount() const//眔妓狾じ计
{
	return m_PatternBitCount;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternIndexGrayCode_1Bit() const//GrayCodeま计秨﹍
{
	return m_PatternIndexGrayCode_1Bit;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternIndexBinaryCode_1Bit() const//BinaryCodeま计秨﹍
{
	return m_PatternIndexBinaryCode_1Bit;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternNumStartGrayCode_1Bit() const//GrayCodeま计-材碭ず甧
{
	return m_PatternNumStartGrayCode_1Bit;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternNumStartBinaryCode_1Bit() const//BinaryCodeま计-材碭ず甧
{
	return m_PatternNumStartBinaryCode_1Bit;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternIndexGrayCode_8Bit() const//GrayCodeま计秨﹍
{
	return m_PatternIndexGrayCode_8Bit;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternIndexBinaryCode_8Bit() const//BinaryCodeま计秨﹍
{
	return m_PatternIndexBinaryCode_8Bit;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternNumStartGrayCode_8Bit() const//GrayCodeま计-材碭ず甧
{
	return m_PatternNumStartGrayCode_8Bit;
}
//-------------------------------------------------------------------------------------//
int CLight3DCtrl::GetPatternNumStartBinaryCode_8Bit() const//BinaryCodeま计-材碭ず甧
{
	return m_PatternNumStartBinaryCode_8Bit;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::FindPatternIndex(int BitMode, double Period, int &Index)
{
	bool IsOK = true;
	switch ( BitMode )
	{
	case 3://3Bit Mode
		IsOK = FindPatternIndex(m_PatternIndexMap_3Bit, Period, Index);
		break;
	case 5://5Bit Mode
		IsOK = FindPatternIndex(m_PatternIndexMap_5Bit, Period, Index);
		break;
	case 8://8Bit Mode
		IsOK = FindPatternIndex(m_PatternIndexMap_8Bit, Period, Index);
		break;
	case 6://6Bit Mode
	default:
		IsOK = FindPatternIndex(m_PatternIndexMap_6Bit, Period, Index);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::FindPatternIndex(const std::map<int, int> &Map, double Period, int &Index)
{	
	for ( auto Iter=Map.begin(); Iter!=Map.end(); ++Iter )	
	{	
		if ( fabs(Period-(Iter->first)) < 0.001 )
		{
			Index = Iter->second;			
			return true;
		}
	}
	m_ErrorString.Format(_T("Error, Find 3D Cast Pattern[%.2f] Index Fault"), Period);
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ExecLight3DCastConnect()//北硈絬
{
	bool IsOK = true;
	CString verLib, verFrm;
	
	const int  NFrames = 1;
	const bool bForce = true;
	const bool bSleep = true;
	const bool DLPMultiTable = true;
	const bool DLPInternalTrigger = false;	
	const int PhasePatMode = DLP_PATTERN_SEQUENCE_4_4_M;
	const int CameraExpTimeus = 6000;
	LIGHT_3D_CLS_PTR Light3DPtr=NULL;
	this->m_Version = _T("");
	this->InitialErrorString();		
	if ( NULL != m_Light3DCast01 )
	{	
		Light3DPtr = m_Light3DCast01;
		if ( Light3DPtr->DLPConnect() == false )
		{
			if ( m_ErrorString.GetLength() == 0 ) 
			{	m_ErrorString = Light3DPtr->GetErrorString(); }
			else
			{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
			IsOK = false;
		}
		else
		{	
			if ( this->m_Version.GetLength() == 0 ) 
			{
				verLib = Light3DPtr->GetTiAPIVersion();
				verFrm = Light3DPtr->GetDLPFrmVersion();
				this->m_Version.Format(_T("Ti:%s; Frm:%s"), verLib, verFrm);
			}			

			const int DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
			const int DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
			const int DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;		
			if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )
			{	JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());	}	
			if ( Light3DPtr->ExecDLPPattern_Run()  == false )
			{	JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());	}
		}
	}	

	if ( NULL != m_Light3DCast02 )
	{	
		Light3DPtr = m_Light3DCast02;
		if ( Light3DPtr->DLPConnect() == false )
		{
			if ( m_ErrorString.GetLength() == 0 ) 
			{	m_ErrorString = Light3DPtr->GetErrorString(); }
			else
			{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
			IsOK = false;
		}
		else
		{
			if ( this->m_Version.GetLength() == 0 ) 
			{
				verLib = Light3DPtr->GetTiAPIVersion();
				verFrm = Light3DPtr->GetDLPFrmVersion();
				this->m_Version.Format(_T("Ti:%s; Frm:%s"), verLib, verFrm);
			}	
			const int DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
			const int DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
			const int DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;		
			if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )
			{	JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());	}	
			if ( Light3DPtr->ExecDLPPattern_Run()  == false )
			{	JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());	}					
		}
	}

	if ( NULL != m_Light3DCast03 )
	{	
		Light3DPtr = m_Light3DCast03;
		if ( Light3DPtr->DLPConnect() == false )
		{
			if ( m_ErrorString.GetLength() == 0 ) 
			{	m_ErrorString = Light3DPtr->GetErrorString(); }
			else
			{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
			IsOK = false;
		}
		else
		{
			if ( this->m_Version.GetLength() == 0 ) 
			{
				verLib = Light3DPtr->GetTiAPIVersion();
				verFrm = Light3DPtr->GetDLPFrmVersion();
				this->m_Version.Format(_T("Ti:%s; Frm:%s"), verLib, verFrm);
			}
			const int DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
			const int DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
			const int DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;		
			if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )
			{	JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());	}	
			if ( Light3DPtr->ExecDLPPattern_Run()  == false )
			{	JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());	}			
		}
	}

	if ( NULL != m_Light3DCast04 )
	{	
		Light3DPtr = m_Light3DCast04;
		if ( Light3DPtr->DLPConnect() == false )
		{
			if ( m_ErrorString.GetLength() == 0 ) 
			{	m_ErrorString = Light3DPtr->GetErrorString(); }
			else
			{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
			IsOK = false;
		}
		else
		{
			if ( this->m_Version.GetLength() == 0 ) 
			{
				verLib = Light3DPtr->GetTiAPIVersion();
				verFrm = Light3DPtr->GetDLPFrmVersion();
				this->m_Version.Format(_T("Ti:%s; Frm:%s"), verLib, verFrm);
			}
			const int DLPLEDColor = Light3DPtr->GetDLPParam().m_LEDColor;
			const int DLPPeriod_us = Light3DPtr->GetDLPParam().m_PeriodTime_us;
			const int DLPExposure_us = Light3DPtr->GetDLPParam().m_ExposureTime_us;		
			if ( Light3DPtr->ExecDLPPatBuildSendValidate(PhasePatMode, DLPInternalTrigger, DLPMultiTable, DLPPeriod_us, DLPExposure_us, DLPLEDColor, bSleep, bForce) == false )
			{	JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());	}	
			if ( Light3DPtr->ExecDLPPattern_Run()  == false )
			{	JetAPI::ShowMessageBox(Light3DPtr->GetErrorString());	}			
		}
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ExecLight3DCastDisconnect()//北耞絬
{
	bool IsOK = true;
	LIGHT_3D_CLS_PTR Light3DPtr=NULL;

	this->m_Version = _T("");
	this->InitialErrorString();	
	if ( NULL != m_Light3DCast01 )
	{	
		Light3DPtr = m_Light3DCast01;
		Light3DPtr->ExecDLPPattern_Stop();
		if ( Light3DPtr->DLPDisconnect() == false )
		{
			if ( m_ErrorString.GetLength() == 0 ) 
			{	m_ErrorString = Light3DPtr->GetErrorString(); }
			else
			{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
			IsOK = false;
		}
	}	

	if ( NULL != m_Light3DCast02 )
	{	
		Light3DPtr = m_Light3DCast02;
		Light3DPtr->ExecDLPPattern_Stop();
		if ( Light3DPtr->DLPDisconnect() == false )
		{
			if ( m_ErrorString.GetLength() == 0 ) 
			{	m_ErrorString = Light3DPtr->GetErrorString(); }
			else
			{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
			IsOK = false;
		}
	}

	if ( NULL != m_Light3DCast03 )
	{	
		Light3DPtr = m_Light3DCast03;
		Light3DPtr->ExecDLPPattern_Stop();
		if ( m_Light3DCast03->DLPDisconnect() == false )
		{
			if ( m_ErrorString.GetLength() == 0 ) 
			{	m_ErrorString = Light3DPtr->GetErrorString(); }
			else
			{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
			IsOK = false;
		}
	}

	if ( NULL != m_Light3DCast04 )
	{	
		Light3DPtr = m_Light3DCast04;
		Light3DPtr->ExecDLPPattern_Stop();
		if ( Light3DPtr->DLPDisconnect() == false )
		{
			if ( m_ErrorString.GetLength() == 0 ) 
			{	m_ErrorString = Light3DPtr->GetErrorString(); }
			else
			{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
			IsOK = false;
		}
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::CheckLight3DCastIsConnected()//北琌硈絬
{
	bool IsOK = true;
#ifndef PHASE_CTRL_DISABLE
	LIGHT_3D_CLS_PTR Light3DPtr=NULL;
	std::vector<LIGHT_3D_CLS_PTR> Light3DPtrList;

	this->m_Version = _T("");
	this->InitialErrorString();	
	if ( BuildLight3DCastList(Light3DPtrList) == false )
	{	return false;	}	
	const int Light3DCount=(int)(Light3DPtrList.size());			
	for ( int i=0; i<Light3DCount; i++ )
	{		
		Light3DPtr = Light3DPtrList[i];
		if ( NULL == Light3DPtr ) { continue; }		
		if ( Light3DPtr->CheckDLPIsConnected() == true )
		{	continue; }
		if ( m_ErrorString.GetLength() == 0 ) 
		{	m_ErrorString = Light3DPtr->GetErrorString(); }
		else
		{	m_ErrorString = m_ErrorString + CString(_T("\n")) + Light3DPtr->GetErrorString(); }
		IsOK = false;
	}
#endif//PHASE_CTRL_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CLight3DCtrl::GetLight3DCastVersion()
{
	return this->m_Version;
}
//-------------------------------------------------------------------------------------//
LIGHT_3D_DEVICE_TYPE CLight3DCtrl::GetLight3DDeviceType(LIGHT_3D_CAST_ID CastID)
{
	LIGHT_3D_DEVICE_TYPE Type=LIGHT_3D_DEVICE_DLP4500;
	LIGHT_3D_CLS_PTR CastPtr = GetLight3DCastPtr(CastID);
	if ( NULL == CastPtr ) { return Type; }
	Type = CastPtr->GetDeviceType();
	return Type;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ChangeLight3DDeviceType(LIGHT_3D_DEVICE_TYPE Type)
{
	std::vector<LIGHT_3D_CAST_ID> CastIDList;
	if ( GetLight3DCastIDList(CastIDList) == false ) { return false; }

	size_t i=0;	
	bool IsOK = true;
	CString str, Error;
	LIGHT_3D_CAST_ID CastID;
	LIGHT_3D_CLS_PTR CastPtr = NULL;
	const size_t CastIDCount=CastIDList.size();

	InitialErrorString();	
	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = CastIDList[i];
		CastPtr = GetLight3DCastPtr(CastID);
		if ( NULL == CastPtr ) { continue; }
		if ( CastPtr->ChangeDeviceType(Type) == false )
		{
			IsOK = false;
			if ( Error.GetLength() == 0 ) 
			{	Error = CastPtr->GetErrorString(); }
			else
			{
				str = CastPtr->GetErrorString();;
				Error += CString(_T("\n"))+str;
			}
		}
	}
	if ( false == IsOK )
	{	m_ErrorString = Error; }
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::GetLight3DCastIDList(std::vector<LIGHT_3D_CAST_ID> &CastIDList)
{
	CastIDList.clear();
	CastIDList.push_back(LIGHT_3D_CAST_01);
	CastIDList.push_back(LIGHT_3D_CAST_02);
	CastIDList.push_back(LIGHT_3D_CAST_03);
	CastIDList.push_back(LIGHT_3D_CAST_04);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DLightSetting(LIGHT_3D_CAST_ID Light3DID, int CurrentID)//砞﹚3Dщ縊方砞﹚
{
#ifndef PHASE_CTRL_DISABLE	
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->ExecDLPLightSetting(CurrentID) == false )	
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}		
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetAllLight3DLEDColor(int LEDColor)//砞﹚┮ΤLED縊肅︹
{
#ifndef PHASE_CTRL_DISABLE	
	this->InitialErrorString();	
	if ( NULL != m_Light3DCast01 )
	{	m_Light3DCast01->GetDLPParam().m_LEDColor = LEDColor;	}
	if ( NULL != m_Light3DCast02 )
	{	m_Light3DCast02->GetDLPParam().m_LEDColor = LEDColor;	}
	if ( NULL != m_Light3DCast03 )
	{	m_Light3DCast03->GetDLPParam().m_LEDColor = LEDColor;	}
	if ( NULL != m_Light3DCast04 )
	{	m_Light3DCast04->GetDLPParam().m_LEDColor = LEDColor;	}	
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetAllLight3DLEDCurrentID(int CurrentID)//砞﹚┮Τ縊方眏
{
	int Red=-1, Grn=-1, Blu=-1;
	if ( SetAllLight3DLEDCurrent(Red, Grn, Blu, CurrentID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetAllLight3DLEDCurrent(int red, int green, int blue, int CurrentID)//砞﹚┮Τ縊方眏
{
#ifndef PHASE_CTRL_DISABLE
	const bool bUpdate=false;	
	int Red=0, Grn=0, Blu=0;
	int curRed=red, curGrn=green, curBlu=blue;	
	this->InitialErrorString();	
	if ( NULL != m_Light3DCast01 )
	{			
		m_Light3DCast01->GetDLPParamLEDCurrent(Red, Grn, Blu, CurrentID);
		if ( red < 0 )   { curRed = Red; }
		if ( green < 0 ) { curGrn = Grn; }
		if ( blue < 0 )  { curBlu = Blu; }
		if ( m_Light3DCast01->SetDLPLEDCurrent(curRed, curGrn, curBlu, bUpdate, CurrentID) == false )
		{
			m_ErrorString = m_Light3DCast01->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast02 )
	{	
		m_Light3DCast02->GetDLPParamLEDCurrent(Red, Grn, Blu, CurrentID);
		if ( red < 0 )   { curRed = Red; }
		if ( green < 0 ) { curGrn = Grn; }
		if ( blue < 0 )  { curBlu = Blu; }
		if ( m_Light3DCast02->SetDLPLEDCurrent(curRed, curGrn, curBlu, bUpdate, CurrentID) == false )
		{
			m_ErrorString = m_Light3DCast02->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast03 )
	{	
		m_Light3DCast03->GetDLPParamLEDCurrent(Red, Grn, Blu, CurrentID);
		if ( red < 0 )   { curRed = Red; }
		if ( green < 0 ) { curGrn = Grn; }
		if ( blue < 0 )  { curBlu = Blu; }		
		if ( m_Light3DCast03->SetDLPLEDCurrent(curRed, curGrn, curBlu, bUpdate, CurrentID) == false )
		{
			m_ErrorString = m_Light3DCast03->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast04 )
	{	
		m_Light3DCast04->GetDLPParamLEDCurrent(Red, Grn, Blu, CurrentID);
		if ( red < 0 )   { curRed = Red; }
		if ( green < 0 ) { curGrn = Grn; }
		if ( blue < 0 )  { curBlu = Blu; }
		if ( m_Light3DCast04->SetDLPLEDCurrent(curRed, curGrn, curBlu, bUpdate, CurrentID) == false )
		{
			m_ErrorString = m_Light3DCast04->GetErrorString();
			return false;
		}		
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DLEDColor(LIGHT_3D_CAST_ID Light3DID, int LEDColor)//砞﹚3DщLED縊肅︹
{
#ifndef PHASE_CTRL_DISABLE
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	Light3DPtr->GetDLPParam().m_LEDColor = LEDColor;
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DLEDCurrent(LIGHT_3D_CAST_ID Light3DID, int red, int green, int blue, int CurrentID)//砞﹚┮Τ縊方眏
{
#ifndef PHASE_CTRL_DISABLE
	const bool bUpdate = false;
	int Red=0, Grn=0, Blu=0;
	int LEDColor=DLP_LED_COLOR_WHITE;	
	int curRed=red, curGrn=green, curBlu=blue;	
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}

	Light3DPtr->GetDLPParamLEDCurrent(Red, Grn, Blu, CurrentID);		
	if ( red < 0 )   { curRed = Red; }
	if ( green < 0 ) { curGrn = Grn; }
	if ( blue < 0 )  { curBlu = Blu; }	
	
	if ( Light3DPtr->SetDLPLEDCurrent(curRed, curGrn, curBlu, bUpdate, CurrentID) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}		
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DLEDEnabled(LIGHT_3D_CAST_ID Light3DID, bool bAuto, int bRed, int bGreen, int bBlue)//砞﹚┮Τ縊方币ノ
{
#ifndef PHASE_CTRL_DISABLE
	bool enaAuto=bAuto, enaRed=bRed, enaGrn=bGreen, enaBlu=bBlue;
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}	
	if ( Light3DPtr->SetDLPLEDEnable(enaAuto, enaRed, enaGrn, enaBlu) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}		
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ClearAllLight3DPatternList()//睲埃┮Τ3D妓狾
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	if ( NULL != m_Light3DCast01 )
	{	m_Light3DCast01->ClearDLPPatternList();	}
	if ( NULL != m_Light3DCast02 )
	{	m_Light3DCast02->ClearDLPPatternList();	}
	if ( NULL != m_Light3DCast03 )
	{	m_Light3DCast03->ClearDLPPatternList();	}
	if ( NULL != m_Light3DCast04 )
	{	m_Light3DCast04->ClearDLPPatternList();	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ReleaseAllLight3DBinParam()//睦┮Τ3DBin把计
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	if ( NULL != m_Light3DCast01 )
	{	
		m_Light3DCast01->ClearPhaseZeroBuffer();	
		m_Light3DCast01->ClearPhaseFactorBuffer();	
	}
	if ( NULL != m_Light3DCast02 )
	{	
		m_Light3DCast02->ClearPhaseZeroBuffer();	
		m_Light3DCast02->ClearPhaseFactorBuffer();	
	}
	if ( NULL != m_Light3DCast03 )
	{	
		m_Light3DCast03->ClearPhaseZeroBuffer();	
		m_Light3DCast03->ClearPhaseFactorBuffer();	
	}
	if ( NULL != m_Light3DCast04 )
	{	
		m_Light3DCast04->ClearPhaseZeroBuffer();	
		m_Light3DCast04->ClearPhaseFactorBuffer();	
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ExecAllLight3DSequencePlay()//秨﹍┮Τ
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	if ( NULL != m_Light3DCast01 )
	{	
		if ( m_Light3DCast01->ExecDLPPattern_Run() == false )
		{
			m_ErrorString = m_Light3DCast01->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast02 )
	{	
		if ( m_Light3DCast02->ExecDLPPattern_Run() == false )
		{
			m_ErrorString = m_Light3DCast02->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast03 )
	{	
		if ( m_Light3DCast03->ExecDLPPattern_Run() == false )
		{
			m_ErrorString = m_Light3DCast03->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast04 )
	{	
		if ( m_Light3DCast04->ExecDLPPattern_Run() == false )
		{
			m_ErrorString = m_Light3DCast04->GetErrorString();
			return false;
		}		
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ExecLight3DSequencePlay(LIGHT_3D_CAST_ID Light3DID)//秨﹍
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->ExecDLPPattern_Run() == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}		
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ExecAllLight3DSequenceStop()//氨ゎ┮Τ	
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	if ( NULL != m_Light3DCast01 )
	{	
		if ( m_Light3DCast01->ExecDLPPattern_Stop() == false )
		{
			m_ErrorString = m_Light3DCast01->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast02 )
	{	
		if ( m_Light3DCast02->ExecDLPPattern_Stop() == false )
		{
			m_ErrorString = m_Light3DCast02->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast03 )
	{	
		if ( m_Light3DCast03->ExecDLPPattern_Stop() == false )
		{
			m_ErrorString = m_Light3DCast03->GetErrorString();
			return false;
		}		
	}
	if ( NULL != m_Light3DCast04 )
	{	
		if ( m_Light3DCast04->ExecDLPPattern_Stop() == false )
		{
			m_ErrorString = m_Light3DCast04->GetErrorString();
			return false;
		}		
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ExecLight3DSequenceStop(LIGHT_3D_CAST_ID Light3DID)//氨ゎ
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->ExecDLPPattern_Stop() == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}		
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DPhaseMode(LIGHT_3D_CAST_ID Light3DID, int Mode)//砞﹚舱篈家Α
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->SetDLPPhaseMode(Mode) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::GetLight3DPhaseMode(LIGHT_3D_CAST_ID Light3DID, int &Mode)//眔舱篈家Α
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	Mode = Light3DPtr->GetDLPPhaseMode();	
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DImageGamma(LIGHT_3D_CAST_ID Light3DID, double Gamma)//砞﹚紇钩Gamma把计
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->SetImageGamma(Gamma) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::GetLight3DImageGamma(LIGHT_3D_CAST_ID Light3DID, double &Gamma)//眔紇钩Gamma把计
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	Gamma = Light3DPtr->GetImageGamma();
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DPhaseZero(LIGHT_3D_CAST_ID Light3DID, int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr)//砞﹚キ
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->SetPhaseZero(PhaseMode, LEDColor, ImageW, ImageH, ImageSetp, Ptr) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::GetLight3DPhaseZero(LIGHT_3D_CAST_ID Light3DID, int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr)//眔キ
{
//#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->GetPhaseZero(PhaseMode, LEDColor, ImageW, ImageH, ImageSetp, Ptr) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
//#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DPhaseFactor(LIGHT_3D_CAST_ID Light3DID, int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr)//砞﹚キ玒计
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->SetPhaseFactor(PhaseMode, LEDColor, ImageW, ImageH, ImageSetp, Ptr) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::GetLight3DPhaseFactor(LIGHT_3D_CAST_ID Light3DID, int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr)//眔キ玒计
{
//#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->GetPhaseFactor(PhaseMode, LEDColor, ImageW, ImageH, ImageSetp, Ptr) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
//#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::AdjustLight3DHeightFactorMappingParam()//秸俱蔼ゑㄒ玒计
{
	this->InitialErrorString();	
	std::vector<LIGHT_3D_CAST_ID> CastIDList;
	GetLight3DCastIDList(CastIDList);	

	size_t  i=0;
	LIGHT_3D_CAST_ID CastID;
	LIGHT_3D_CLS_PTR Light3DPtr = NULL;
	const size_t CastIDCount=CastIDList.size();
	if ( CastIDCount < 2 ) { return true; }	 
	
	int    TargetNo=0;
	int    BaseCount=0;
	double BaseHeight=0;			
	const  bool bRecv = true;
	const  bool bCalcOnly = true;
	const  int  MaxTargetNo = MAX_HEIGHT_TARGET_COUNT;
	TPhaseFactorTable PhaseFactorTable;	

	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = CastIDList[i];
		PhaseFactorTable.GridList.clear();
		Light3DPtr = GetLight3DCastPtr(CastID);
		if ( NULL == Light3DPtr ) { continue; }			
		Light3DPtr->SortHeightFactorTableList();
		Light3DPtr->RestoreHeightFactorTableList();
		Light3DPtr->BuildHeightFactorMappingParam(bRecv);
	}	
	for ( int j=0; j<MaxTargetNo; j++ )
	{	
		BaseCount=0;
		BaseHeight=0;		
		TargetNo = j+1;		
		for ( i=0; i<CastIDCount; i++ )
		{
			CastID = CastIDList[i];
			PhaseFactorTable.GridList.clear();
			Light3DPtr = GetLight3DCastPtr(CastID);
			if ( NULL == Light3DPtr ) { continue; }
			Light3DPtr->CloneHeightFactorTable(TargetNo, PhaseFactorTable);
			if ( PhaseFactorTable.GridList.size() == 0 ) { continue; }
			for ( size_t k=0; k<PhaseFactorTable.GridList.size(); k++ )
			{
				BaseHeight += PhaseFactorTable.GridList[k].m_HeightBaseCalc;
				BaseCount ++;
			}
		}
		if ( 0 == BaseCount ) { continue; }
		BaseHeight /= BaseCount;

		for ( i=0; i<CastIDCount; i++ )
		{
			CastID = CastIDList[i];
			PhaseFactorTable.GridList.clear();
			Light3DPtr = GetLight3DCastPtr(CastID);
			if ( NULL == Light3DPtr ) { continue; }				
			Light3DPtr->CloneHeightFactorTable(TargetNo, PhaseFactorTable);
			if ( PhaseFactorTable.GridList.size() == 0 ) { continue; }
			for ( size_t k=0; k<PhaseFactorTable.GridList.size(); k++ )
			{
				PhaseFactorTable.GridList[k].m_HeightBaseCalc = BaseHeight;
				PhaseFactorTable.GridList[k].m_HeightTargetCalc = BaseHeight+PhaseFactorTable.GridList[k].m_HeightOffset;
			}			
			Light3DPtr->SetHeightFactorTable(TargetNo, PhaseFactorTable);			
		}
	}

	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = CastIDList[i];
		PhaseFactorTable.GridList.clear();
		Light3DPtr = GetLight3DCastPtr(CastID);
		if ( NULL == Light3DPtr ) { continue; }					
		Light3DPtr->BuildHeightFactorMappingParam(false);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::BuildLight3DHeightFactorMappingParam(LIGHT_3D_CAST_ID Light3DID)//ミ蔼ゑㄒ玒计
{
//#ifndef PHASE_CTRL_DISABLE
	bool bRecv = false;
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->SortHeightFactorTableList() == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
	if ( Light3DPtr->RestoreHeightFactorTableList() == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
	if ( Light3DPtr->BuildHeightFactorMappingParam(bRecv) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
//#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::GetLight3DHeightFactor(LIGHT_3D_CAST_ID Light3DID, double T0[], double T1[], double T2[])//眔蔼ゑㄒ玒计
{
//#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->GetHeightFactorMappingParam(T0, T1, T2) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
//#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::ClearLight3DHeightFactorTableList(LIGHT_3D_CAST_ID Light3DID)//睲埃蔼玒计翴
{
#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	bool bIncludeFiles = false;	
	Light3DPtr->ClearHeightFactorTableList(bIncludeFiles);
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::CloneLight3DHeightFactorTable(LIGHT_3D_CAST_ID Light3DID, int TargetNo, TPhaseFactorTable &GridTable)//狡籹蔼玒计翴
{
//#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->CloneHeightFactorTable(TargetNo, GridTable) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
//#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetLight3DHeightFactorTable(LIGHT_3D_CAST_ID Light3DID, int TargetNo, const TPhaseFactorTable &GridTable)//砞﹚蔼玒计翴
{
//#ifndef PHASE_CTRL_DISABLE
	this->InitialErrorString();	
	LIGHT_3D_CLS_PTR Light3DPtr = this->GetLight3DCastPtr(Light3DID);
	if ( NULL == Light3DPtr )
	{	return false;	}
	if ( Light3DPtr->SetHeightFactorTable(TargetNo, GridTable) == false )
	{
		m_ErrorString = Light3DPtr->GetErrorString();
		return false;
	}
//#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SaveAllLight3DCastParameter()//纗┮Τ把计
{
	return SaveAllLight3DCastParameter(false);
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SaveAllLight3DCastParameter(bool bParamOnly)//纗┮Τ把计
{
	bool bSucc = true;
#ifndef OFFLINE_VERSION	
	if ( SaveLight3DCastParameter(LIGHT_3D_CAST_01, bParamOnly) == false )
	{	bSucc = false; }
	if ( SaveLight3DCastParameter(LIGHT_3D_CAST_02, bParamOnly) == false )
	{	bSucc = false; }                                                                                 
	if ( SaveLight3DCastParameter(LIGHT_3D_CAST_03, bParamOnly) == false )
	{	bSucc = false; }
	if ( SaveLight3DCastParameter(LIGHT_3D_CAST_04, bParamOnly) == false )
	{	bSucc = false; }
#endif//OFFLINE_VERSION
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SaveAllLight3DCastParameter_PhaseZero()//纗┮Τ把计-キ
{
#ifndef OFFLINE_VERSION	
	InitialErrorString();	

	size_t  i=0;
	LIGHT_3D_CAST_ID CastID;
	std::vector<LIGHT_3D_CAST_ID> CastIDList;
	GetLight3DCastIDList(CastIDList);
	const size_t Count=CastIDList.size();
	for ( i=0; i<Count; i++ )
	{
		CastID = CastIDList[i];
		if ( SaveLight3DCastParameter_PhaseZero(CastID) == false )
		{	return false; }
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::LoadAllLight3DCastParameter(bool bLoadParam)//更┮Τ把计
{
	bool bSucc = true;
#ifndef OFFLINE_VERSION	
	CString ErrorString;
 	if ( LoadLight3DCastParameter(LIGHT_3D_CAST_01, bLoadParam) == false )
	{	
		ErrorString = m_ErrorString;
		bSucc = false; 
	}
	if ( LoadLight3DCastParameter(LIGHT_3D_CAST_02, bLoadParam) == false )
	{	
		ErrorString = m_ErrorString;
		bSucc = false; 
	}
	if ( LoadLight3DCastParameter(LIGHT_3D_CAST_03, bLoadParam) == false )
	{	
		ErrorString = m_ErrorString;
		bSucc = false; 
	}
	if ( LoadLight3DCastParameter(LIGHT_3D_CAST_04, bLoadParam) == false )
	{	
		ErrorString = m_ErrorString;
		bSucc = false; 
	}	
	if ( true == bSucc )
	{	AdjustLight3DHeightFactorMappingParam();	}
	else {
		m_ErrorString = ErrorString;
	}
#endif//OFFLINE_VERSION
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SaveLight3DCastParameter(LIGHT_3D_CAST_ID Light3DID, bool bParamOnly)//纗﹚把计
{
#ifndef OFFLINE_VERSION	
	InitialErrorString();	

	LIGHT_3D_CLS_PTR  Ptr = GetLight3DCastPtr(Light3DID);
	if ( NULL == Ptr )
	{	return CheckBypassLight3D(Light3DID); }	
	if ( Ptr->SaveDLPParameter() == false )
	{
		m_ErrorString = Ptr->GetErrorString();
		return false;
	}
	if ( false == bParamOnly )
	{
		if ( Ptr->SavePhaseZero() == false )
		{
			m_ErrorString = Ptr->GetErrorString();
			return false;
		}
		if ( Ptr->SavePhaseFactor() == false )
		{
			m_ErrorString = Ptr->GetErrorString();
			return false;
		}	
		if ( Ptr->SaveHeightFactorTableList() == false )
		{
			m_ErrorString = Ptr->GetErrorString();
			return false;
		}
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SaveLight3DCastParameter_PhaseZero(LIGHT_3D_CAST_ID Light3DID)//纗﹚把计-キ
{
#ifndef OFFLINE_VERSION	
	InitialErrorString();	
	LIGHT_3D_CLS_PTR  CastPtr = GetLight3DCastPtr(Light3DID);
	if ( NULL == CastPtr )
	{	return CheckBypassLight3D(Light3DID); }		
	if ( CastPtr->SavePhaseZero() == false )
	{
		m_ErrorString = CastPtr->GetErrorString();
		return false;
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::LoadLight3DCastParameter(LIGHT_3D_CAST_ID Light3DID, bool bLoadParam)//更﹚把计
{	
#ifndef OFFLINE_VERSION	
	InitialErrorString();
	LIGHT_3D_CLS_PTR  Ptr = GetLight3DCastPtr(Light3DID);
	if ( NULL == Ptr )
	{	return CheckBypassLight3D(Light3DID); }

	if ( true == bLoadParam )
	{
		if ( Ptr->LoadDLPParameter() == false )
		{
			m_ErrorString = Ptr->GetErrorString();
			return false;
		}
	}
	if ( Ptr->LoadPhaseZero() == false )
	{
		m_ErrorString = Ptr->GetErrorString();
		return false;
	}		
	if ( Ptr->LoadPhaseFactor() == false )
	{
		m_ErrorString = Ptr->GetErrorString();
		return false;
	}

	if ( Ptr->LoadHeightFactorTableList() == false )
	{
		m_ErrorString = Ptr->GetErrorString();
		return false;
	}
	bool bRecv = false;
	Ptr->BuildHeightFactorMappingParam(bRecv);
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLight3DCtrl::SetAllLight3DCastPatternIndexByPeriod(double Period1, double Period2)//穝3Dщ妓-ㄌ沮秅戳
{
#ifndef PHASE_CTRL_DISABLE
	int Index1_3Bit = -1;
	int Index2_3Bit = -1;
	int Index1_5Bit = -1;
	int Index2_5Bit = -1;
	int Index1_6Bit = -1;
	int Index2_6Bit = -1;
	int Index1_8Bit = -1;
	int Index2_8Bit = -1;
	const int BitCount=GetPatternBitCount();
	const int GrayCodeIndex_1Bit=GetPatternIndexGrayCode_1Bit();
	const int BinaryCodeIndex_1Bit=GetPatternIndexBinaryCode_1Bit();
	const int GrayCodeStartNum_1Bit=GetPatternNumStartGrayCode_1Bit();
	const int BinaryCodeStartNum_1Bit=GetPatternNumStartBinaryCode_1Bit();
	const int GrayCodeIndex_8Bit=GetPatternIndexGrayCode_8Bit();
	const int BinaryCodeIndex_8Bit=GetPatternIndexBinaryCode_8Bit();
	const int GrayCodeStartNum_8Bit=GetPatternNumStartGrayCode_8Bit();
	const int BinaryCodeStartNum_8Bit=GetPatternNumStartBinaryCode_8Bit();

	FindPatternIndex(3, Period1, Index1_3Bit);
	FindPatternIndex(3, Period2, Index2_3Bit);
	FindPatternIndex(5, Period1, Index1_5Bit);
	FindPatternIndex(5, Period2, Index2_5Bit);
	FindPatternIndex(6, Period1, Index1_6Bit);
	FindPatternIndex(6, Period2, Index2_6Bit);
	FindPatternIndex(8, Period1, Index1_8Bit);
	FindPatternIndex(8, Period2, Index2_8Bit);

	size_t i=0;	
	LIGHT_3D_CAST_ID CastID;
	LIGHT_3D_CLS_PTR Light3DPtr = NULL;
	std::vector<LIGHT_3D_CAST_ID> CastIDList;

	GetLight3DCastIDList(CastIDList);	
	const size_t CastIDCount=CastIDList.size();
	for ( i=0; i<CastIDCount; i++ )
	{
		CastID = CastIDList[i];
		Light3DPtr = GetLight3DCastPtr(CastID);
		if ( NULL == Light3DPtr ) { continue; }
		
		Light3DPtr->ClearDLPPatternList();
		Light3DPtr->ClearPhaseZeroBuffer();
		Light3DPtr->ClearPhaseFactorBuffer();		

		Light3DPtr->SetPatternBitCount(BitCount);

		Light3DPtr->SetPatternIndex1_3Bit(Index1_3Bit);
		Light3DPtr->SetPatternIndex2_3Bit(Index2_3Bit);

		Light3DPtr->SetPatternIndex1_5Bit(Index1_5Bit);
		//Light3DPtr->SetPatternIndex2_5Bit(Index2_5Bit);

		Light3DPtr->SetPatternIndex1_6Bit(Index1_6Bit);
		Light3DPtr->SetPatternIndex2_6Bit(Index2_6Bit);		

		Light3DPtr->SetPatternIndex1_8Bit(Index1_8Bit);
		Light3DPtr->SetPatternIndex2_8Bit(Index2_8Bit);	

		Light3DPtr->SetPatternIndexGC_1Bit(GrayCodeIndex_1Bit);
		Light3DPtr->SetPatternIndexBC_1Bit(BinaryCodeIndex_1Bit);
		Light3DPtr->SetPatternStartNumGC_1Bit(GrayCodeStartNum_1Bit);
		Light3DPtr->SetPatternStartNumBC_1Bit(BinaryCodeStartNum_1Bit);

		Light3DPtr->SetPatternIndexGC_8Bit(GrayCodeIndex_8Bit);	
		Light3DPtr->SetPatternIndexBC_8Bit(BinaryCodeIndex_8Bit);
		Light3DPtr->SetPatternStartNumGC_8Bit(GrayCodeStartNum_8Bit);
		Light3DPtr->SetPatternStartNumBC_8Bit(BinaryCodeStartNum_8Bit);		
	}
#endif//PHASE_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//