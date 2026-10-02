// AOILogic.cpp: implementation of the CAOILogic class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOILogic.h"
//-------------------------------------------------------------------------------------//
#include "AOIModel.h"
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
IMPLEMENT_DYNAMIC(CAOILogic, CAOIObj)
//-------------------------------------------------------------------------------------//
CString CAOILogic::GetLogicModeText(LOGIC_MODE LogicMode)
{
	CString str;
	switch ( LogicMode )
	{
	case LOGIC_MODE_AND:
		str = _T(" AND ");
		break;
	case LOGIC_MODE_XOR:
		str = _T(" XOR ");
		break;
	case LOGIC_MODE_BIG:
		str = _T(" > ");
		break;
	case LOGIC_MODE_SMALL:
		str = _T(" < ");
		break;
	case LOGIC_MODE_EQU_BIG:
		str = _T(" >= ");
		break;
	case LOGIC_MODE_EQU_SMALL:
		str = _T(" <= ");
		break;
	case LOGIC_MODE_GAP_BIG:
		str = _T(" |Gap|> ");
		break;
	case LOGIC_MODE_GAP_SMALL:
		str = _T(" |Gap|< ");
		break;
	case LOGIC_MODE_GAP_RATIO_BIG:
		str = _T(" |AC/DC|> ");
		break;
	case LOGIC_MODE_GAP_RATIO_SMALL:
		str = _T(" |AC/DC|< ");
		break;
	default:
		str = _T("Undefined");
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
CAOILogic::CAOILogic():CAOIObj(AOI_OBJ_LOGIC)
{
	PreInitLogic();
	InitialLogic();
}
//-------------------------------------------------------------------------------------//
CAOILogic::CAOILogic(const CAOILogic &Logic):CAOIObj(Logic)
{
	PreInitLogic();
	CloneLogic(Logic);
}
//-------------------------------------------------------------------------------------//
CAOILogic::~CAOILogic()
{

}
//-------------------------------------------------------------------------------------//
CAOILogic& CAOILogic::operator=(const CAOILogic &Logic)
{
	if ( this == &Logic ) { return *this; }
	CAOIObj::operator=(Logic);
	CloneLogic(Logic);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOILogic::PreInitLogic()
{	
}
//-------------------------------------------------------------------------------------//
inline void CAOILogic::InitialLogic()
{
	CAOILogic::m_LogicIndex = -1;	
	//---------------------------------------------------------------------------------//	
	CAOILogic::m_LogicModelPtr = NULL;
	CAOILogic::m_LogicLandPtr = NULL;
	CAOILogic::m_LogicLandIndex = -1;
	//---------------------------------------------------------------------------------//
	CAOILogic::m_LogicGroupID = -1;
	CAOILogic::m_LogicModified = false;
//	RESULT_ID                  m_LogicResultID;//
//	WND_DEFECT_ID              m_LogicDefectID;	
	//---------------------------------------------------------------------------------//
	CAOILogic::m_LogicWndPtrList.clear();
	CAOILogic::m_LogicWndIndexList.clear();
	//---------------------------------------------------------------------------------//	
	CAOILogic::m_LogicEnabled = true;
	CAOILogic::m_LogicSelected = false;
	CAOILogic::m_LogicIsolated = false;
	//---------------------------------------------------------------------------------//
	CAOILogic::m_LogicLogicMode = LOGIC_MODE_AND;	
	CAOILogic::m_LogicParamDBL1 = 0;	
	CAOILogic::m_LogicResultString = _T("");
}
//-------------------------------------------------------------------------------------//
inline void CAOILogic::CloneLogic(const CAOILogic &Logic)
{
	CAOILogic::m_LogicIndex = Logic.m_LogicIndex;	
	CAOILogic::m_LogicModelPtr = Logic.m_LogicModelPtr;
	CAOILogic::m_LogicLandPtr  = Logic.m_LogicLandPtr;
	CAOILogic::m_LogicLandIndex = Logic.m_LogicLandIndex;
	CAOILogic::m_LogicGroupID = Logic.m_LogicGroupID;
	CAOILogic::m_LogicModified = Logic.m_LogicModified;
	//CAOILogic::m_LogicResultID = Logic.m_LogicResultID;
	//CAOILogic::m_LogicDefectID = Logic.m_LogicDefectID;
	CAOILogic::m_LogicWndPtrList = Logic.m_LogicWndPtrList;
	CAOILogic::m_LogicWndIndexList = Logic.m_LogicWndIndexList;	
	CAOILogic::m_LogicEnabled = Logic.m_LogicEnabled;
	CAOILogic::m_LogicSelected = Logic.m_LogicSelected;
	CAOILogic::m_LogicIsolated = Logic.m_LogicIsolated;
	CAOILogic::m_LogicLogicMode = Logic.m_LogicLogicMode;
	CAOILogic::m_LogicResultString = Logic.m_LogicResultString;	
	CAOILogic::m_LogicParamDBL1 = Logic.m_LogicParamDBL1;
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOILogic::CloneLogicObj() const//複製出實體的CAOILogic指標
{
	CAOILogic *ObjPtr = AOIObjManager.CreateLogicObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
inline size_t CAOILogic::GetLogicWndPtrCount_Inline() const
{
	return CAOILogic::m_LogicWndPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOILogic::RemoveLogicWndList_Inline()
{
	CAOILogic::m_LogicWndPtrList.clear();
}
//-------------------------------------------------------------------------------------//
inline void CAOILogic::AddLogicWndPtr_Inline(CAOIWnd* Ptr)
{
	CAOILogic::m_LogicWndPtrList.push_back(Ptr);
}
//-------------------------------------------------------------------------------------//
inline CAOIWnd* CAOILogic::GetLogicWndPtr_Inline(size_t index, bool Check) const
{
	if ( Check == true )
	{
		const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
		if ( index >= WndCount ) 
		{	return NULL; }
	}
	return CAOILogic::m_LogicWndPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOILogic::GetLogicWndIndexCount_Inline() const
{
	return CAOILogic::m_LogicWndIndexList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOILogic::RemoveLogicWndIndexList_Inline()
{
	CAOILogic::m_LogicWndIndexList.clear();
}
//-------------------------------------------------------------------------------------//
inline void CAOILogic::AddLogicWndIndex_Inline(unsigned int WndIndex)
{
	CAOILogic::m_LogicWndIndexList.push_back(WndIndex);
}
//-------------------------------------------------------------------------------------//
inline unsigned int CAOILogic::GetLogicWndIndex_Inline(size_t index, bool Check) const
{
	if ( Check == true )
	{
		const size_t WndCount = CAOILogic::GetLogicWndIndexCount_Inline();
		if ( index >= WndCount ) 
		{	return -1; }
	}
	return CAOILogic::m_LogicWndIndexList[index];
}
//-------------------------------------------------------------------------------------//
void CAOILogic::AddLogicWndPtr(CAOIWnd* WndPtr)
{
	if ( NULL == WndPtr ) { return; }	
	CAOILogic::AddLogicWndPtr_Inline(WndPtr);
	CAOILogic::AddLogicWndIndex_Inline(WndPtr->GetWndIndex());
}
//-------------------------------------------------------------------------------------//
void CAOILogic::AddLogicWndIndex(unsigned int WndIndex)
{	
	CAOILogic::AddLogicWndPtr_Inline(NULL);
	CAOILogic::AddLogicWndIndex_Inline(WndIndex);
}
//-------------------------------------------------------------------------------------//
void CAOILogic::RemoveLogicWndPtrSelected()
{	
	size_t      i = 0;
	int         WndIndex = 0;
	CAOIWnd    *WndPtr = NULL;
	std::vector<CAOIWnd*>      TempLogicWndPtrList = CAOILogic::m_LogicWndPtrList;
	const size_t WndCount = TempLogicWndPtrList.size();

	WndIndex = 0;
	CAOILogic::RemoveLogicWndList_Inline();
	CAOILogic::RemoveLogicWndIndexList_Inline();
	for ( i=0; i<WndCount; i++)
	{
		WndPtr = TempLogicWndPtrList[i];
		if ( WndPtr == NULL ) { continue; }
		if ( WndPtr->GetWndSelected() == true ) 
		{	continue; }		
		CAOILogic::AddLogicWndPtr_Inline(WndPtr);
		CAOILogic::AddLogicWndIndex_Inline(WndPtr->GetWndIndex());
		WndIndex ++;
	}

	const size_t Count = CAOILogic::GetLogicWndPtrCount_Inline();	
}
//-------------------------------------------------------------------------------------//
void CAOILogic::RemoveLogicWndPtrList()
{
	CAOILogic::RemoveLogicWndList_Inline();
	CAOILogic::RemoveLogicWndIndexList_Inline();	
}
//-------------------------------------------------------------------------------------//
unsigned int CAOILogic::GetLogicWndIndex(size_t index, bool Check) const
{
	return CAOILogic::GetLogicWndIndex_Inline(index, Check);	
}
//-------------------------------------------------------------------------------------//	
CAOIWnd* CAOILogic::GetLogicWndPtr(size_t index, bool Check) const
{
	return CAOILogic::GetLogicWndPtr_Inline(index, Check);	
}
//-------------------------------------------------------------------------------------//
void CAOILogic::UpdateLogicWndIndex()
{
	size_t      i = 0;	
	CAOIWnd    *WndPtr = NULL;	
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();	
	CAOILogic::RemoveLogicWndIndexList_Inline();
	for ( i=0; i<WndCount; i++)
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }		
		CAOILogic::AddLogicWndIndex_Inline(WndPtr->GetWndIndex());
	}
}
//-------------------------------------------------------------------------------------//
void CAOILogic::SetLogicWndSelected(bool val)
{
	size_t      i = 0;	
	CAOIWnd    *WndPtr = NULL;	
	const size_t WndCount = GetLogicWndPtrCount_Inline();	
	
	for ( i=0; i<WndCount; i++)
	{
		WndPtr = GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndSelected(val);			
	}
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOILogic::GetLogicWndSelected() const
{
	size_t i=0;
	CAOIWnd *WndPtr = NULL;
	const size_t WndCount = GetLogicWndPtrCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == false ) { continue; }
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::GetLogicHaveParamDBL(LOGIC_MODE LogicMode, int Index)
{
	bool HaveParam = true;
	switch ( LogicMode )
	{
	case LOGIC_MODE_AND:
		HaveParam = false;
		break;
	case LOGIC_MODE_XOR:
		HaveParam = false; 
		break;
	case LOGIC_MODE_BIG:
		if ( Index == 1 )
		{	HaveParam = true; }
		else
		{	HaveParam = false; }
		break;
	case LOGIC_MODE_SMALL:
		if ( Index == 1 )
		{	HaveParam = true; }
		else
		{	HaveParam = false; }
		break;
	case LOGIC_MODE_EQU_BIG:
		if ( Index == 1 )
		{	HaveParam = true; }
		else
		{	HaveParam = false; }
		break;
	case LOGIC_MODE_EQU_SMALL:
		if ( Index == 1 )
		{	HaveParam = true; }
		else
		{	HaveParam = false; }
		break;
	case LOGIC_MODE_GAP_BIG:
		if ( Index == 1 )
		{	HaveParam = true; }
		else
		{	HaveParam = false; }
		break;
	case LOGIC_MODE_GAP_SMALL:
		if ( Index == 1 )
		{	HaveParam = true; }
		else
		{	HaveParam = false; }
		break;
	case LOGIC_MODE_GAP_RATIO_BIG:
		if ( Index == 1 )
		{	HaveParam = true; }
		else
		{	HaveParam = false; }
		break;
	case LOGIC_MODE_GAP_RATIO_SMALL:
		if ( Index == 1 )
		{	HaveParam = true; }
		else
		{	HaveParam = false; }
		break;
	}
	return HaveParam;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::ApplyLogic(const CAOILogic *RefLogicPtr)//套用相同的邏輯閘
{
	CAOILogic::m_LogicModified = RefLogicPtr->m_LogicModified;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::SynchronousLogic(const CAOILogic *RefLogicPtr)//同步化同一個邏輯閘
{
	CAOILogic::m_LogicModified = RefLogicPtr->m_LogicModified;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis()//執行邏輯分析
{	
	bool IsOK = true;
	/*
	const bool LogicEnabled = CAOILogic::GetLogicEnabled();
	const LOGIC_MODE LogicMode = CAOILogic::GetLogicLogicMode();
	
	if ( false == LogicEnabled )		
	{
		CAOILogic::SetLogicResultID(RESULT_ID_OK);
		CAOILogic::SetLogicResultString(_T(""));
		return true;
	}

	switch ( LogicMode )
	{
	case LOGIC_MODE_AND:
		IsOK = CAOILogic::DoLogicAnalysis_AND();
		break;
	case LOGIC_MODE_XOR:
		IsOK = CAOILogic::DoLogicAnalysis_XOR();
		break;
	case LOGIC_MODE_BIG:
		IsOK = CAOILogic::DoLogicAnalysis_BIG();
		break;
	case LOGIC_MODE_SMALL:
		IsOK = CAOILogic::DoLogicAnalysis_SMALL();
		break;
	case LOGIC_MODE_EQU_BIG:
		IsOK = CAOILogic::DoLogicAnalysis_EQU_BIG();
		break;
	case LOGIC_MODE_EQU_SMALL:
		IsOK = CAOILogic::DoLogicAnalysis_EQU_SMALL();
		break;
	case LOGIC_MODE_GAP_BIG:
		IsOK = CAOILogic::DoLogicAnalysis_GAP_BIG();
		break;
	case LOGIC_MODE_GAP_SMALL:
		IsOK = CAOILogic::DoLogicAnalysis_GAP_SMALL();
		break;
	case LOGIC_MODE_GAP_RATIO_BIG:
		IsOK = CAOILogic::DoLogicAnalysis_GAP_Ratio_BIG();
		break;
	case LOGIC_MODE_GAP_RATIO_SMALL:
		IsOK = CAOILogic::DoLogicAnalysis_GAP_Ratio_SMALL();
		break;
	}
	*/
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_AND()//執行邏輯分析-且
{	
	/*
	size_t       i = 0;	
	bool         First = true;
	RESULT_ID    ResultID = RESULT_ID_OK;
	RESULT_ID    WndResultID;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();

	First = true;	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }

		if ( First == true )
		{
			First = false;
			ResultID = RESULT_ID_NG;
		}

		WndResultID = WndPtr->GetWndResultID();
		if ( WndResultID == RESULT_ID_UNTEST )
		{	return false; }

		if ( WndResultID == RESULT_ID_OK )
		{	ResultID = RESULT_ID_OK;	}
	}
	CAOILogic::SetLogicResultID(ResultID);
	CAOILogic::SetLogicResultString(_T(""));
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_XOR()//執行邏輯分析-XOR
{
	/*
	size_t       i = 0;			
	RESULT_ID    WndResultID;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<RESULT_ID> WndResultIDList;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }

		WndResultID = WndPtr->GetWndResultID();
		if ( WndResultID == RESULT_ID_UNTEST )
		{	return false; }
		WndResultIDList.push_back(WndResultID);
	}
	
	const size_t WndResultIDCount = WndResultIDList.size();
	for ( i=0; i<WndResultIDCount; i++ )
	{		
		if ( i == 0 ) 
		{
			WndResultID = WndResultIDList[i];
			ResultID = RESULT_ID_OK;			
		}
		else
		{
			if ( WndResultID != WndResultIDList[i] )
			{	
				ResultID = RESULT_ID_NG; 
				break;
			}
		}
	}
	CAOILogic::SetLogicResultID(ResultID);
	CAOILogic::SetLogicResultString(_T(""));
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_BIG()//執行邏輯分析-大於
{
	/*
	CString      str;
	size_t       i = 0;	
	double       WndReading=0;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<double> WndReadingList;
	CAOIWnd     *WndPtr = NULL;
	const double dBigThan = CAOILogic::GetLogicParamDBL1();
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndResultID() == RESULT_ID_UNTEST )
		{	return false; }

		WndReading = WndPtr->GetWndResultReading();
		WndReadingList.push_back(WndReading);
	}	
	
	const size_t WndReadingCount = WndReadingList.size();
	if ( WndReadingCount > 0 ) 
	{
		double ReadingA = 0;
		double ReadingB = 0;
		for ( i=0; i<WndReadingCount; i++ )
		{		
			if ( i == 0 ) 
			{	ReadingA = ReadingB = WndReadingList[i];	}
			else if ( i == 1 )
			{	ReadingB = WndReadingList[i];	}
			else 
			{
				if ( ReadingB > WndReadingList[i] )
				{	ReadingB = WndReadingList[i];	}
			}
		}
		WndReading = ReadingA-ReadingB;
		if ( WndReading > dBigThan )
		{	ResultID = RESULT_ID_NG; }
		else
		{	ResultID = RESULT_ID_OK; }
		str.Format(_T("%.1f"), WndReading);
	}
	else 
	{	
		WndReading = 0;	
		str = _T("");
	}
	CAOILogic::SetLogicResultID(ResultID);	
	CAOILogic::SetLogicResultString(str);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_SMALL()//執行邏輯分析-小於
{
	/*
	CString      str;
	size_t       i = 0;			
	double       WndReading=0;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<double> WndReadingList;
	CAOIWnd     *WndPtr = NULL;
	const double dSmallThan = CAOILogic::GetLogicParamDBL1();
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndResultID() == RESULT_ID_UNTEST )
		{	return false; }

		WndReading = WndPtr->GetWndResultReading();
		WndReadingList.push_back(WndReading);
	}	
	
	const size_t WndReadingCount = WndReadingList.size();
	if ( WndReadingCount > 0 ) 
	{
		double ReadingA = 0;
		double ReadingB = 0;
		for ( i=0; i<WndReadingCount; i++ )
		{		
			if ( i == 0 ) 
			{	ReadingA = ReadingB = WndReadingList[i];	}
			else if ( i == 1 )
			{	ReadingB = WndReadingList[i];	}
			else 
			{
				if ( ReadingB < WndReadingList[i] )
				{	ReadingB = WndReadingList[i];	}
			}
		}
		WndReading = ReadingA-ReadingB;
		if ( WndReading < dSmallThan )
		{	ResultID = RESULT_ID_NG; }
		else
		{	ResultID = RESULT_ID_OK; }
		str.Format(_T("%.1f"), WndReading);
	}
	else 
	{	
		WndReading = 100;	
		str = _T("");
	}
	CAOILogic::SetLogicResultID(ResultID);
	CAOILogic::SetLogicResultString(str);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_EQU_BIG()//執行邏輯分析-大於等於
{
	/*
	CString      str;
	size_t       i = 0;			
	double       WndReading=0;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<double> WndReadingList;
	CAOIWnd     *WndPtr = NULL;
	const double dBigThan = CAOILogic::GetLogicParamDBL1();
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndResultID() == RESULT_ID_UNTEST )
		{	return false; }

		WndReading = WndPtr->GetWndResultReading();
		WndReadingList.push_back(WndReading);
	}	
	
	const size_t WndReadingCount = WndReadingList.size();
	if ( WndReadingCount > 0 ) 
	{
		double ReadingA = 0;
		double ReadingB = 0;
		for ( i=0; i<WndReadingCount; i++ )
		{		
			if ( i == 0 ) 
			{	ReadingA = ReadingB = WndReadingList[i];	}
			else if ( i == 1 )
			{	ReadingB = WndReadingList[i];	}
			else 
			{
				if ( ReadingB > WndReadingList[i] )
				{	ReadingB = WndReadingList[i];	}
			}
		}
		WndReading = ReadingA-ReadingB;
		if ( WndReading >= dBigThan )
		{	ResultID = RESULT_ID_NG; }
		else
		{	ResultID = RESULT_ID_OK; }
		str.Format(_T("%.1f"), WndReading);
	}
	else 
	{	
		WndReading = 0;	
		str = _T("");
	}
	CAOILogic::SetLogicResultID(ResultID);	
	CAOILogic::SetLogicResultString(str);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_EQU_SMALL()//執行邏輯分析-小於等於
{
	/*
	CString      str;
	size_t       i = 0;			
	double       WndReading=0;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<double> WndReadingList;
	CAOIWnd     *WndPtr = NULL;
	const double dSmallThan = CAOILogic::GetLogicParamDBL1();
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndResultID() == RESULT_ID_UNTEST )
		{	return false; }

		WndReading = WndPtr->GetWndResultReading();
		WndReadingList.push_back(WndReading);
	}	
	
	const size_t WndReadingCount = WndReadingList.size();
	if ( WndReadingCount > 0 ) 
	{
		double ReadingA = 0;
		double ReadingB = 0;
		for ( i=0; i<WndReadingCount; i++ )
		{		
			if ( i == 0 ) 
			{	ReadingA = ReadingB = WndReadingList[i];	}
			else if ( i == 1 )
			{	ReadingB = WndReadingList[i];	}
			else 
			{
				if ( ReadingB < WndReadingList[i] )
				{	ReadingB = WndReadingList[i];	}
			}
		}
		WndReading = ReadingA-ReadingB;
		if ( WndReading <= dSmallThan )
		{	ResultID = RESULT_ID_NG; }
		else
		{	ResultID = RESULT_ID_OK; }
		str.Format(_T("%.1f"), WndReading);
	}
	else 
	{	
		WndReading = 100;	
		str = _T("");
	}
	CAOILogic::SetLogicResultID(ResultID);
	CAOILogic::SetLogicResultString(str);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_GAP_BIG()//執行邏輯分析-差距大於
{
	/*
	CString      str;
	size_t       i = 0;			
	double       WndReading=0;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<double> WndReadingList;
	CAOIWnd     *WndPtr = NULL;
	const double dBigThan = CAOILogic::GetLogicParamDBL1();
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndResultID() == RESULT_ID_UNTEST )
		{	return false; }
		WndReading = WndPtr->GetWndResultReading();
		WndReadingList.push_back(WndReading);
	}	
	
	const size_t WndReadingCount = WndReadingList.size();
	if ( WndReadingCount > 0 ) 
	{
		double ReadingMax = 0;
		double ReadingMin = 0;
		for ( i=0; i<WndReadingCount; i++ )
		{		
			if ( i == 0 ) 
			{	ReadingMax = ReadingMin = WndReadingList[i];	}
			else 
			{
				if ( ReadingMax < WndReadingList[i] )
				{	ReadingMax = WndReadingList[i];	}
				else if ( ReadingMin > WndReadingList[i] )
				{	ReadingMin = WndReadingList[i];	}
			}
		}
		WndReading = ReadingMax-ReadingMin;
		if ( WndReading > dBigThan )
		{	ResultID = RESULT_ID_NG; }
		else
		{	ResultID = RESULT_ID_OK; }
		str.Format(_T("%.1f"), WndReading);
	}
	else 
	{	
		WndReading = 0;	
		str = _T("");
	}
	CAOILogic::SetLogicResultID(ResultID);
	CAOILogic::SetLogicResultString(str);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_GAP_SMALL()//執行邏輯分析-差距小於
{
	/*
	CString      str;
	size_t       i = 0;			
	double       WndReading=0;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<double> WndReadingList;
	CAOIWnd     *WndPtr = NULL;
	const double dSmallThan = CAOILogic::GetLogicParamDBL1();
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndResultID() == RESULT_ID_UNTEST )
		{	return false; }
		WndReading = WndPtr->GetWndResultReading();
		WndReadingList.push_back(WndReading);
	}	
	
	const size_t WndReadingCount = WndReadingList.size();
	if ( WndReadingCount > 0 ) 
	{
		double ReadingMax = 0;
		double ReadingMin = 0;
		for ( i=0; i<WndReadingCount; i++ )
		{		
			if ( i == 0 ) 
			{	ReadingMax = ReadingMin = WndReadingList[i];	}
			else 
			{
				if ( ReadingMax < WndReadingList[i] )
				{	ReadingMax = WndReadingList[i];	}
				else if ( ReadingMin > WndReadingList[i] )
				{	ReadingMin = WndReadingList[i];	}
			}
		}
		WndReading = ReadingMax-ReadingMin;
		if ( WndReading < dSmallThan )
		{	ResultID = RESULT_ID_NG; }
		else
		{	ResultID = RESULT_ID_OK; }
		str.Format(_T("%.1f"), WndReading);
	}
	else 
	{	
		WndReading = 100;	
		str = _T("");
	}
	CAOILogic::SetLogicResultID(ResultID);
	CAOILogic::SetLogicResultString(str);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_GAP_Ratio_BIG()//執行邏輯分析-差距比率大於
{
	/*
	CString      str;
	size_t       i = 0;			
	double       WndReading=0;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<double> WndReadingList;
	CAOIWnd     *WndPtr = NULL;
	const double dBigThan = CAOILogic::GetLogicParamDBL1();
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndResultID() == RESULT_ID_UNTEST )
		{	return false; }
		WndReading = WndPtr->GetWndResultReading();
		WndReadingList.push_back(WndReading);
	}	
	
	const size_t WndReadingCount = WndReadingList.size();
	if ( WndReadingCount > 0 ) 
	{
		double ReadingAve = 0;
		double ReadingMax = 0;
		double ReadingMin = 0;
		for ( i=0; i<WndReadingCount; i++ )
		{		
			if ( i == 0 ) 
			{	ReadingMax = ReadingMin = WndReadingList[i];	}
			else 
			{
				if ( ReadingMax < WndReadingList[i] )
				{	ReadingMax = WndReadingList[i];	}
				else if ( ReadingMin > WndReadingList[i] )
				{	ReadingMin = WndReadingList[i];	}
			}
			ReadingAve += WndReadingList[i];
		}
		WndReading = ReadingMax-ReadingMin;
		WndReading = WndReading*200.0/ReadingAve;
		if ( WndReading > dBigThan )
		{	ResultID = RESULT_ID_NG; }
		else
		{	ResultID = RESULT_ID_OK; }
		str.Format(_T("%.1f"), WndReading);
	}	
	else 
	{	
		WndReading = 0;	
		str = _T("");
	}
	CAOILogic::SetLogicResultID(ResultID);
	CAOILogic::SetLogicResultString(str);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILogic::DoLogicAnalysis_GAP_Ratio_SMALL()//執行邏輯分析-差距比率小於
{
	/*
	CString      str;
	size_t       i = 0;			
	double       WndReading=0;
	RESULT_ID    ResultID = RESULT_ID_OK;
	std::vector<double> WndReadingList;
	CAOIWnd     *WndPtr = NULL;
	const double dSmallThan = CAOILogic::GetLogicParamDBL1();
	const size_t WndCount = CAOILogic::GetLogicWndPtrCount_Inline();
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILogic::GetLogicWndPtr_Inline(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndResultID() == RESULT_ID_UNTEST )
		{	return false; }
		WndReading = WndPtr->GetWndResultReading();
		WndReadingList.push_back(WndReading);
	}	
	
	const size_t WndReadingCount = WndReadingList.size();
	if ( WndReadingCount > 0 ) 
	{
		double ReadingAve = 0;
		double ReadingMax = 0;
		double ReadingMin = 0;
		for ( i=0; i<WndReadingCount; i++ )
		{		
			if ( i == 0 ) 
			{	ReadingMax = ReadingMin = WndReadingList[i];	}
			else 
			{
				if ( ReadingMax < WndReadingList[i] )
				{	ReadingMax = WndReadingList[i];	}
				else if ( ReadingMin > WndReadingList[i] )
				{	ReadingMin = WndReadingList[i];	}
			}
			ReadingAve += WndReadingList[i];
		}
		WndReading = ReadingMax-ReadingMin;
		WndReading = WndReading*200.0/ReadingAve;
		if ( WndReading < dSmallThan )
		{	ResultID = RESULT_ID_NG; }
		else
		{	ResultID = RESULT_ID_OK; }
		str.Format(_T("%.1f"), WndReading);
	}
	else 
	{	
		WndReading = 100;	
		str = _T("");
	}
	CAOILogic::SetLogicResultID(ResultID);
	CAOILogic::SetLogicResultString(str);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//