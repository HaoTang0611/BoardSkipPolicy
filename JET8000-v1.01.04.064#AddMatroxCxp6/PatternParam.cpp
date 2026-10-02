// PatternParam.cpp: implementation of the CPatternParam class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "PatternParam.h"
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
CPatternParam::CPatternParam()
{
	CPatternParam::PreInitPatternParam();
	CPatternParam::InitialPatternParam();
}
//-------------------------------------------------------------------------------------//
CPatternParam::CPatternParam(const CPatternParam &Param)
{
	CPatternParam::PreInitPatternParam();
	CPatternParam::ClonePatternParam(Param);
}
//-------------------------------------------------------------------------------------//
CPatternParam::~CPatternParam()
{

}
//-------------------------------------------------------------------------------------//
CPatternParam& CPatternParam::operator=(const CPatternParam &Param)
{
	if ( this == &Param ) { return *this; }
	CPatternParam::ClonePatternParam(Param);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CPatternParam::PreInitPatternParam()
{
}
//-------------------------------------------------------------------------------------//
void CPatternParam::InitialPatternParam()
{
	m_BinaryParam = CAlgBinaryParam();
	m_BinaryParam.SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_PATTERN_IMAGE);

	ResetResultValue();	
	m_PatText = L"";	
	m_PatSimilarTextList.clear();
	m_PatRoiListDummy.clear();

	m_PatRoiListL[0].clear();
	m_PatRoiListT[0].clear();
	m_PatRoiListR[0].clear();
	m_PatRoiListB[0].clear();

	m_PatRoiListL[1].clear();
	m_PatRoiListT[1].clear();
	m_PatRoiListR[1].clear();
	m_PatRoiListB[1].clear();
}
//-------------------------------------------------------------------------------------//
void CPatternParam::ClonePatternParam(const CPatternParam &Param)
{
	m_ResultPolarityIdx = Param.m_ResultPolarityIdx;
	m_BinaryParam = Param.m_BinaryParam;		

	m_PatResultW[0] = Param.m_PatResultW[0];	
	m_PatResultH[0] = Param.m_PatResultH[0];
	m_PatResultX[0] = Param.m_PatResultX[0];	
	m_PatResultY[0] = Param.m_PatResultY[0];	
	m_PatResultSkew[0] = Param.m_PatResultSkew[0];	
	m_PatResultScore[0] = Param.m_PatResultScore[0];	
	m_PatResultScaleX[0] = Param.m_PatResultScaleX[0];	
	m_PatResultScaleY[0] = Param.m_PatResultScaleY[0];		 

	m_ResultX[0] = Param.m_ResultX[0];	
	m_ResultY[0] = Param.m_ResultY[0];	
	m_ResultSkew[0] = Param.m_ResultSkew[0];
	m_ResultScaleX[0] = Param.m_ResultScaleX[0];	
	m_ResultScaleY[0] = Param.m_ResultScaleY[0];
	m_ResultReading[0] = Param.m_ResultReading[0];
	m_ReultID[0] = Param.m_ReultID[0];		

	m_PatResultW[1] = Param.m_PatResultW[1];	
	m_PatResultH[1] = Param.m_PatResultH[1];	
	m_PatResultX[1] = Param.m_PatResultX[1];	
	m_PatResultY[1] = Param.m_PatResultY[1];	
	m_PatResultSkew[1] = Param.m_PatResultSkew[1];	
	m_PatResultScore[1] = Param.m_PatResultScore[1];	
	m_PatResultScaleX[1] = Param.m_PatResultScaleX[1];	
	m_PatResultScaleY[1] = Param.m_PatResultScaleY[1];		 

	m_ResultX[1] = Param.m_ResultX[1];	
	m_ResultY[1] = Param.m_ResultY[1];	
	m_ResultSkew[1] = Param.m_ResultSkew[1];
	m_ResultScaleX[1] = Param.m_ResultScaleX[1];	
	m_ResultScaleY[1] = Param.m_ResultScaleY[1];
	m_ResultReading[1] = Param.m_ResultReading[1];
	m_ReultID[1] = Param.m_ReultID[1];		

	m_PatText = Param.m_PatText;		
	m_PatSimilarTextList = Param.m_PatSimilarTextList;
	m_PatRoiListDummy = Param.m_PatRoiListDummy;

	m_PatRoiListL[0] = Param.m_PatRoiListL[0];	
	m_PatRoiListT[0] = Param.m_PatRoiListT[0];	
	m_PatRoiListR[0] = Param.m_PatRoiListR[0];	
	m_PatRoiListB[0] = Param.m_PatRoiListB[0];	

	m_PatRoiListL[1] = Param.m_PatRoiListL[1];	
	m_PatRoiListT[1] = Param.m_PatRoiListT[1];	
	m_PatRoiListR[1] = Param.m_PatRoiListR[1];	
	m_PatRoiListB[1] = Param.m_PatRoiListB[1];	
}
//-------------------------------------------------------------------------------------//
inline bool CPatternParam::CheckPatternRoiIndex(int index) const
{
	if ( index<0 || index>=MAX_PATTERN_ROI_COUNT )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPatternParam::InitPatRoiListInspection(std::vector<TPATTERN_ROI> &List)
{
	size_t i=0;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{
		TPATTERN_ROI &Ref=List[i];
		Ref.ResultScore = 0.0;
		Ref.ResultID = RESULT_ID_NONE;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPatternParam::WritePatternParamFile(CAOIFileIO &FileIO)//纗妓狾把计
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	CPatternParam *PatParamPtr = this;		
	const std::vector<std::wstring> &SimilarTextList=GetPatSimilarTextList();
	FileIO.SetFnName(_T("CPatternParam::WritePatternParamFile"));
	//----------------------------------------------------------------------------------------//
	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_PARAM_START, 0) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_PARAM_BINARY_PARAM, 0) == false ) { return false; }
	if ( PatParamPtr->m_BinaryParam.WriteAlgBinaryParamFile(FileIO) == false ) { return false; }	

	if ( FileIO.SaveChunk_STR(FILE_IO_PATTERN_PARAM_PAT_TEXT, PatParamPtr->m_PatText.c_str()) == false ) { return false; }		

	const size_t SimilarTextCount=SimilarTextList.size();
	for ( size_t i=0; i<SimilarTextCount; i++ )
	{
		const std::wstring &SimilarText=SimilarTextList[i];
		if ( SimilarText.length() == 0 ) { continue; }
		if ( FileIO.SaveChunk_STR(FILE_IO_PATTERN_PARAM_PAT_SIMILAR_TEXT, SimilarText.c_str()) == false ) { return false; }		
	}	

	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_PARAM_ROI_LIST_L, 0) == false ) { return false; }
	if ( WritePatternRoiListFile(FileIO, PatParamPtr->m_PatRoiListL[0]) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_PARAM_ROI_LIST_T, 0) == false ) { return false; }
	if ( WritePatternRoiListFile(FileIO, PatParamPtr->m_PatRoiListT[0]) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_PARAM_ROI_LIST_R, 0) == false ) { return false; }
	if ( WritePatternRoiListFile(FileIO, PatParamPtr->m_PatRoiListR[0]) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_PARAM_ROI_LIST_B, 0) == false ) { return false; }
	if ( WritePatternRoiListFile(FileIO, PatParamPtr->m_PatRoiListB[0]) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_PARAM_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPatternParam::ReadPatternParamFile(CAOIFileIO &FileIO)//更妓狾把计
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG	
	
	int    index = 0;
	std::wstring SimilarText;
	CPatternParam *PatParamPtr = this;		
	PatParamPtr->ClearPatSimilarTextList();
	FileIO.SetFnName(_T("CAlgParam::ReadAlgPatternParamFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_PATTERN_PARAM_START://妓狾把计-癬翴	
			PatParamPtr->InitialPatternParam();
			break;
		case FILE_IO_PATTERN_PARAM_END://妓狾把计-沧翴			
			return true;
			break;
		case FILE_IO_PATTERN_PARAM_BINARY_PARAM://妓狾把计-2て把计		
			if ( PatParamPtr->m_BinaryParam.ReadAlgBinaryParamFile(FileIO) == false )			
			{	return false; }
			PatParamPtr->m_BinaryParam.SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_PATTERN_IMAGE);
			break;
		case FILE_IO_PATTERN_PARAM_PAT_TEXT://妓狾ゅ
			if ( FileIO.GetLoadWStr()==true )
			{	PatParamPtr->m_PatText = FileIO.GetData_WSTR();	}
			else
			{	JetAPI::char2wstring(FileIO.GetData_STR(), PatParamPtr->m_PatText);	}			
			break;		
		case FILE_IO_PATTERN_PARAM_PAT_SIMILAR_TEXT:
			if ( FileIO.GetLoadWStr()==true )
			{	PatParamPtr->AddPatSimilarText(FileIO.GetData_WSTR());	}
			else
			{	
				JetAPI::char2wstring(FileIO.GetData_STR(), SimilarText);	
				PatParamPtr->AddPatSimilarText(SimilarText.c_str());
			}
			break;	
		case FILE_IO_PATTERN_PARAM_ROI_LIST_L://妓狾把计-竚把计-绰オ			
			if ( ReadPatternRoiListFile(FileIO, PatParamPtr->m_PatRoiListL[0]) == false ) { return false; }
			PatParamPtr->m_PatRoiListL[1] = PatParamPtr->m_PatRoiListL[0];
			break;
		case FILE_IO_PATTERN_PARAM_ROI_LIST_T://妓狾把计-竚把计-绰			
			if ( ReadPatternRoiListFile(FileIO, PatParamPtr->m_PatRoiListT[0]) == false ) { return false; }
			PatParamPtr->m_PatRoiListT[1] = PatParamPtr->m_PatRoiListT[0];
			break;
		case FILE_IO_PATTERN_PARAM_ROI_LIST_R://妓狾把计-竚把计-绰			
			if ( ReadPatternRoiListFile(FileIO, PatParamPtr->m_PatRoiListR[0]) == false ) { return false; }
			PatParamPtr->m_PatRoiListR[1] = PatParamPtr->m_PatRoiListR[0];
			break;
		case FILE_IO_PATTERN_PARAM_ROI_LIST_B://妓狾把计-竚把计-绰			
			if ( ReadPatternRoiListFile(FileIO, PatParamPtr->m_PatRoiListB[0]) == false ) { return false; }
			PatParamPtr->m_PatRoiListB[1] = PatParamPtr->m_PatRoiListB[0];
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
bool CPatternParam::WritePatternRoiListFile(CAOIFileIO &FileIO, std::vector<TPATTERN_ROI> &RoiRectList)//纗妓狾跋办把计
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG	
	
	size_t        i=0;
	TPATTERN_ROI *RoiPtr = NULL;
	const size_t  RoiCount = RoiRectList.size();
	FileIO.SetFnName(_T("CPatternParam::WritePatternRoiListFile"));
	//----------------------------------------------------------------------------------------//
	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_ROI_SECTION_START, 0) == false ) { return false; }

	for ( i=0; i<RoiCount; i++ )
	{
		RoiPtr = &(RoiRectList[i]);

		if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_ROI_START, 0) == false ) { return false; }

		if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_ROI_LEFT, RoiPtr->RoiRect.left) == false ) { return false; }
		if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_ROI_TOP, RoiPtr->RoiRect.top) == false ) { return false; }
		if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_ROI_RIGHT, RoiPtr->RoiRect.right) == false ) { return false; }
		if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_ROI_BOTTOM, RoiPtr->RoiRect.bottom) == false ) { return false; }	

		if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_ROI_END, 0) == false ) { return false; }
	}

	if ( FileIO.SaveChunk_INT(FILE_IO_PATTERN_ROI_SECTION_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPatternParam::ReadPatternRoiListFile(CAOIFileIO &FileIO, std::vector<TPATTERN_ROI> &RoiRectList)//更妓狾跋办把计
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int          index = 0;
	TPATTERN_ROI PatRoi;
	FileIO.SetFnName(_T("CPatternParam::ReadPatternRoiListFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_PATTERN_ROI_SECTION_START://妓狾把计跋丁-癬翴-29201
			break;
		case FILE_IO_PATTERN_ROI_SECTION_END://妓狾把计跋丁-沧翴-29201
			return true;
			break;
		case FILE_IO_PATTERN_ROI_START://妓狾把计-癬翴
			PatRoi = TPATTERN_ROI();
			break;
		case FILE_IO_PATTERN_ROI_END://妓狾把计-沧翴	
			RoiRectList.push_back(PatRoi);
		//	return true;
			break;		
		case FILE_IO_PATTERN_ROI_LEFT://妓狾把计-オ
			PatRoi.RoiRect.left = FileIO.GetData_INT();
			break;
		case FILE_IO_PATTERN_ROI_TOP://妓狾把计-
			PatRoi.RoiRect.top = FileIO.GetData_INT();
			break;
		case FILE_IO_PATTERN_ROI_RIGHT://妓狾把计-
			PatRoi.RoiRect.right = FileIO.GetData_INT();
			break;
		case FILE_IO_PATTERN_ROI_BOTTOM://妓狾把计-
			PatRoi.RoiRect.bottom = FileIO.GetData_INT();
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
int  CPatternParam::GetPatResultW(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0; }
	return m_PatResultW[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatResultW(int idx, int val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatResultW[idx] = val;
}
//-------------------------------------------------------------------------------------//
int  CPatternParam::GetPatResultH(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0; }
	return m_PatResultH[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatResultH(int idx, int val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatResultH[idx] = val;	 
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetPatResultX(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_PatResultX[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatResultX(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatResultX[idx] = val;	 
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetPatResultY(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_PatResultY[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatResultY(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatResultY[idx] = val;	 
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetPatResultSkew(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_PatResultSkew[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatResultSkew(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatResultSkew[idx] = val;	
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetPatResultScore(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_PatResultScore[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatResultScore(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatResultScore[idx] = val;
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetPatResultScaleX(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_PatResultScaleX[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatResultScaleX(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatResultScaleX[idx] = val;
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetPatResultScaleY(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_PatResultScaleY[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatResultScaleY(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatResultScaleY[idx] = val;
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetResultX(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_ResultX[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetResultX(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_ResultX[idx] = val;
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetResultY(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_ResultY[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetResultY(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_ResultY[idx] = val;
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetResultSkew(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 0.0; }
	return m_ResultSkew[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetResultSkew(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_ResultSkew[idx] = val;
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetResultScaleX(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 1.0; }
	return m_ResultScaleX[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetResultScaleX(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_ResultScaleX[idx] = val;
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetResultScaleY(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 1.0; }
	return m_ResultScaleY[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetResultScaleY(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_ResultScaleY[idx] = val;
}
//-------------------------------------------------------------------------------------//
double CPatternParam::GetResultReading(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return 1.0; }
	return m_ResultReading[idx];
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetResultReading(int idx, double val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_ResultReading[idx] = val;
}
//-------------------------------------------------------------------------------------//
int CPatternParam::GetResultPolarityIdx() const
{
	return m_ResultPolarityIdx;
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetResultPolarityIdx(int val)
{
	m_ResultPolarityIdx = val;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CPatternParam::GetReultID(int idx) const
{
	if ( CheckPatternRoiIndex(idx) == false ) { return RESULT_ID_NONE; }
	return m_ReultID[idx];	
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetReultID(int idx, RESULT_ID val)
{
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_ReultID[idx] = val;
}
//-------------------------------------------------------------------------------------//
void CPatternParam::ResetResultValue()
{
	size_t i=0;
	const size_t Count=MAX_PATTERN_ROI_COUNT;
	m_ResultPolarityIdx = 0;	

	for ( i=0; i<Count; i++ )
	{
		m_PatResultW[i] = 0;
		m_PatResultH[i] = 0;
		m_PatResultX[i] = 0;	
		m_PatResultY[i] = 0;	
		m_PatResultSkew[i] = 0;	
		m_PatResultScore[i] = 0;	
		m_PatResultScaleX[i] = 1.0;	
		m_PatResultScaleY[i] = 1.0;		 

		m_ResultX[i] = 0.0;
		m_ResultY[i] = 0.0;
		m_ResultSkew[i] = 0.0;	
		m_ResultScaleX[i] = 1.0;
		m_ResultScaleY[i] = 1.0;
		m_ResultReading[i] = 0.0;
		m_ReultID[i] = RESULT_ID_NONE;	

		InitPatRoiListInspection(m_PatRoiListL[i]);
		InitPatRoiListInspection(m_PatRoiListT[i]);
		InitPatRoiListInspection(m_PatRoiListR[i]);
		InitPatRoiListInspection(m_PatRoiListB[i]);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CPatternParam::CheckResultIndex()
{
	if ( RESULT_ID_OK==m_ReultID[0] || RESULT_ID_NONE==m_ReultID[1])
	{	
		SetResultPolarityIdx(0);
		return;
	}
	if ( RESULT_ID_OK==m_ReultID[1] || RESULT_ID_NONE==m_ReultID[0] )
	{	
		SetResultPolarityIdx(1);
		return;
	}
	const double Dif=fabs(m_ResultReading[0]-m_ResultReading[1]);
	if ( Dif < 0.00001 )
	{
		if ( m_PatResultScore[1] > m_PatResultScore[0] ) 
		{	SetResultPolarityIdx(1);	}
		else
		{	SetResultPolarityIdx(0);	}
		return;
	}
	if ( m_ResultReading[1] > m_ResultReading[0] ) 
	{	SetResultPolarityIdx(1);	}
	else
	{	SetResultPolarityIdx(0);	}
	return;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CPatternParam::GetPatText() const
{
	return m_PatText.c_str();	
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatText(const wchar_t *val)
{
	m_PatText=val; 	
}
//-------------------------------------------------------------------------------------//
bool CPatternParam::GetPatInputStringMode() const//妓狾块﹃家Α
{
	return true;
}
//-------------------------------------------------------------------------------------//
std::vector<std::wstring>& CPatternParam::GetPatSimilarTextList()
{
	return m_PatSimilarTextList;
}
//-------------------------------------------------------------------------------------//
void CPatternParam::ClearPatSimilarTextList()
{
	m_PatSimilarTextList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CPatternParam::GetPatSimilarTextCount() const
{
	return m_PatSimilarTextList.size();
}
//-------------------------------------------------------------------------------------//
void CPatternParam::AddPatSimilarText(const wchar_t *val)
{
	m_PatSimilarTextList.push_back(val);
}
//-------------------------------------------------------------------------------------//
const wchar_t* CPatternParam::GetPatSimilarText(size_t idx, bool bCheck) const
{
	if ( true == bCheck )
	{
		const size_t Count=m_PatSimilarTextList.size();
		if ( idx >= Count )
		{	return NULL; }
	}
	return m_PatSimilarTextList[idx].c_str();
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatSimilarTextList(const std::vector<std::wstring> &val)
{
	m_PatSimilarTextList = val;
}
//-------------------------------------------------------------------------------------//
std::vector<TPATTERN_ROI>& CPatternParam::GetPatRoiList(BOX_TOWARD Toward, int idx)
{
	switch ( Toward )
	{
	case BOX_TOWARD_UP:    return GetPatRoiListT(idx);
	case BOX_TOWARD_LEFT:  return GetPatRoiListL(idx);
	case BOX_TOWARD_DOWN:  return GetPatRoiListB(idx);
	case BOX_TOWARD_RIGHT: return GetPatRoiListR(idx);
	default:		
		break;
	}
	return m_PatRoiListDummy;
}
//-------------------------------------------------------------------------------------//
std::vector<TPATTERN_ROI>& CPatternParam::GetPatRoiListL(int idx)
{ 
	if ( CheckPatternRoiIndex(idx) == false ) { return m_PatRoiListL[0]; }
	return m_PatRoiListL[idx]; 
}	
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatRoiListL(const std::vector<TPATTERN_ROI> &val)
{
	m_PatRoiListL[0]=val; 
	m_PatRoiListL[1]=val; 
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatRoiListL(int idx, const std::vector<TPATTERN_ROI> &val) 
{ 
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatRoiListL[idx]=val; 
}
//-------------------------------------------------------------------------------------//
std::vector<TPATTERN_ROI>& CPatternParam::GetPatRoiListT(int idx) 
{ 
	if ( CheckPatternRoiIndex(idx) == false ) { return m_PatRoiListT[0]; }
	return m_PatRoiListT[idx]; 
}	
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatRoiListT(const std::vector<TPATTERN_ROI> &val)
{	
	m_PatRoiListT[0]=val; 
	m_PatRoiListT[1]=val; 
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatRoiListT(int idx, const std::vector<TPATTERN_ROI> &val)
{ 
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatRoiListT[idx]=val; 
}
//-------------------------------------------------------------------------------------//
std::vector<TPATTERN_ROI>& CPatternParam::GetPatRoiListR(int idx) 
{ 
	if ( CheckPatternRoiIndex(idx) == false ) { return m_PatRoiListR[0]; }
	return m_PatRoiListR[idx]; 
}	
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatRoiListR(const std::vector<TPATTERN_ROI> &val)
{
	m_PatRoiListR[0]=val; 
	m_PatRoiListR[1]=val; 
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatRoiListR(int idx, const std::vector<TPATTERN_ROI> &val) 
{ 
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatRoiListR[idx]=val; 
}
//-------------------------------------------------------------------------------------//
std::vector<TPATTERN_ROI>& CPatternParam::GetPatRoiListB(int idx) 
{ 
	if ( CheckPatternRoiIndex(idx) == false ) { return m_PatRoiListB[0]; }
	return m_PatRoiListB[idx]; 
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatRoiListB(const std::vector<TPATTERN_ROI> &val)
{
	m_PatRoiListB[0]=val; 
	m_PatRoiListB[1]=val; 
}
//-------------------------------------------------------------------------------------//
void CPatternParam::SetPatRoiListB(int idx, const std::vector<TPATTERN_ROI> &val)
{ 
	if ( CheckPatternRoiIndex(idx) == false ) { return ; }
	m_PatRoiListB[idx]=val; 
}
//-------------------------------------------------------------------------------------//
bool CPatternParam::ModifyPatternSize(BOX_TOWARD Toward, IMAGE_SIZE BeforeW, IMAGE_SIZE BeforeH, unsigned int PatW, unsigned int PatH)
{
	size_t i=0, j=0;
	size_t PatRoiCount=0;	
	int    StartX=0, StartY=0;
	int    SXL=0, SYL=0;//癬﹍竚
	int    SXT=0, SYT=0;
	int    SXR=0, SYR=0;
	int    SXB=0, SYB=0;
	unsigned int LocPatW=0, LocPatH=0;
	unsigned int PatWL=0, PatHL=0;//妓狾へ
	unsigned int PatWT=0, PatHT=0;
	unsigned int PatWR=0, PatHR=0;
	unsigned int PatWB=0, PatHB=0;
	const size_t MaxPatRoiCount = MAX_PATTERN_ROI_COUNT;
	if ( PatW>BeforeW || PatH>BeforeH ) 
	{	return false; }

	switch ( Toward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:		
		PatWT = PatW;	PatHT = PatH;
		PatWB = PatW;	PatHB = PatH;
		PatWL = PatH;	PatHL = PatW;
		PatWR = PatH;	PatHR = PatW;
		SXT = (BeforeW-PatW)/2;	SYT = (BeforeH-PatH)/2;
		SXB = (BeforeW-PatW)/2;	SYB = (BeforeH-PatH)/2;
		SXL = (BeforeH-PatH)/2;	SYL = (BeforeW-PatW)/2;
		SXR = (BeforeH-PatH)/2;	SYR = (BeforeW-PatW)/2;
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		PatWL = PatW;	PatHL = PatH;
		PatWR = PatW;	PatHR = PatH;
		PatWT = PatH;	PatHT = PatW;
		PatWB = PatH;	PatHB = PatW;
		SXL = (BeforeW-PatW)/2;	SYL = (BeforeH-PatH)/2;
		SXR = (BeforeW-PatW)/2;	SYR = (BeforeH-PatH)/2;
		SXT = (BeforeH-PatH)/2;	SYT = (BeforeW-PatW)/2;
		SXB = (BeforeH-PatH)/2;	SYB = (BeforeW-PatW)/2;
		break;
	}
	for ( i=0; i<MaxPatRoiCount; i++ )
	{
		//For Left
		StartX = SXL;
		StartY = SYL;
		LocPatW= PatWL;
		LocPatH= PatHL;
		PatRoiCount = m_PatRoiListL[i].size();
		for ( j=0; j<PatRoiCount; j++ )
		{
			RECT &LocRoi = m_PatRoiListL[i][j].RoiRect;
			//Ι埃癬﹍竚
			LocRoi.left   -= StartX;
			LocRoi.top    -= StartY;
			LocRoi.right  -= StartX;
			LocRoi.bottom -= StartY;
			JetAPI::BoundaryRect(LocPatW, LocPatH, LocRoi);//秸俱へ
		}

		//For Top
		StartX = SXT;
		StartY = SYT;
		LocPatW= PatWT;
		LocPatH= PatHT;
		PatRoiCount = m_PatRoiListT[i].size();
		for ( j=0; j<PatRoiCount; j++ )
		{
			RECT &LocRoi = m_PatRoiListT[i][j].RoiRect;
			//Ι埃癬﹍竚
			LocRoi.left   -= StartX;
			LocRoi.top    -= StartY;
			LocRoi.right  -= StartX;
			LocRoi.bottom -= StartY;
			JetAPI::BoundaryRect(LocPatW, LocPatH, LocRoi);	//秸俱へ		
		}

		//For Right
		StartX = SXR;
		StartY = SYR;
		LocPatW= PatWR;
		LocPatH= PatHR;
		PatRoiCount = m_PatRoiListR[i].size();
		for ( j=0; j<PatRoiCount; j++ )
		{
			RECT &LocRoi = m_PatRoiListR[i][j].RoiRect;
			//Ι埃癬﹍竚
			LocRoi.left   -= StartX;
			LocRoi.top    -= StartY;
			LocRoi.right  -= StartX;
			LocRoi.bottom -= StartY;
			JetAPI::BoundaryRect(LocPatW, LocPatH, LocRoi);	//秸俱へ	
		}

		//For Bottom
		StartX = SXB;
		StartY = SYB;
		LocPatW= PatWB;
		LocPatH= PatHB;
		PatRoiCount = m_PatRoiListB[i].size();
		for ( j=0; j<PatRoiCount; j++ )
		{
			RECT &LocRoi = m_PatRoiListB[i][j].RoiRect;
			//Ι埃癬﹍竚
			LocRoi.left   -= StartX;
			LocRoi.top    -= StartY;
			LocRoi.right  -= StartX;
			LocRoi.bottom -= StartY;
			JetAPI::BoundaryRect(LocPatW, LocPatH, LocRoi);	//秸俱へ		
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//

