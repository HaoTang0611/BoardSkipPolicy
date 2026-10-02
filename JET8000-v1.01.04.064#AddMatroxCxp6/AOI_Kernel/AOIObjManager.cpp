// AOIObjManager.cpp: implementation of the CAOIObjManager class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIObjManager.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#define USING_OBJ_MANAGER          1
#ifdef USING_OBJ_MANAGER
#define USING_RECYCLE_MODE         1
#endif//USING_OBJ_MANAGER
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CAOIObjManager  AOIObjManager;
//-------------------------------------------------------------------------------------//
CAOIObjManager::CAOIObjManager()
{
	m_glbObjDelCount = 0;//竒埃计秖	
	::InitializeCriticalSection(&m_csManager);

#ifdef OPEN_MP_USE
	m_RecycleMP = false;
	for ( int i=0; i<OBJ_MGR_MAX_MP_COUNT; i++ )
	{	::InitializeCriticalSection(&(m_csManager_MP[i]));	}	
#endif//OPEN_MP_USE
}
//-------------------------------------------------------------------------------------//
CAOIObjManager::~CAOIObjManager()
{
#ifdef USING_RECYCLE_MODE
	DestroyRecycleObjList();	
#endif//USING_RECYCLE_MODE
	CAOIObjManager::CheckGlobalObjList();
	::DeleteCriticalSection(&m_csManager);

#ifdef OPEN_MP_USE
	for ( int i=0; i<OBJ_MGR_MAX_MP_COUNT; i++ )
	{	::DeleteCriticalSection(&(m_csManager_MP[i]));	}
#endif//OPEN_MP_USE
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::AddObjPtr(CAOIObj *ObjPtr)
{
#ifdef USING_OBJ_MANAGER
	LockManager();
	const size_t size = (size_t)(this->m_glbObjList.size());
	ObjPtr->SetObjGlobalPtrIndex(size);
	this->m_glbObjList.push_back(ObjPtr);
#ifdef USING_RECYCLE_MODE		
	AOI_OBJ_TYPE ObjType = ObjPtr->GetObjType();
	switch ( ObjType )
	{
	case AOI_OBJ_WND:		
	case AOI_OBJ_LAND:
		ObjPtr->SetObjUsed(true);
		ObjPtr->SetObjRecycleMode(true);		
		break;
	}	
#endif//USING_RECYCLE_MODE
	UnlockManager();

	if ( CAOIObjManager::m_glbObjDelCount > 0 ) 
	{
		const size_t MaxSize = INT_MAX-1;
		if ( size > MaxSize )
		{	CAOIObjManager::RearrangeGlobalObjList();	}	
	}
#endif//USING_OBJ_MANAGER
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOIObjManager::DestroyObject(CAOIObj *ObjPtr)
{
	if ( NULL == ObjPtr ) { return true; }
#ifdef USING_OBJ_MANAGER
#ifdef USING_RECYCLE_MODE		
	if ( true == ObjPtr->CheckObjRecycleMode() )
	{
		ObjPtr->SetObjUsed(false);
		AOI_OBJ_TYPE ObjType = ObjPtr->GetObjType();
		switch ( ObjType )
		{
		case AOI_OBJ_WND:
			((CAOIWnd*)(ObjPtr))->ReleaseWndObj();
			AddFreeRecycleObjPtr(m_csManager, m_RecycleWnd, ObjPtr, false);
			break;
		case AOI_OBJ_LAND:
			((CAOILand*)(ObjPtr))->ReleaseLandObj();
			AddFreeRecycleObjPtr(m_csManager, m_RecycleLand, ObjPtr, false);
			break;
		}
	#ifdef OPEN_MP_USE
		if ( m_RecycleMP )
		{	
			const bool bMP = true;
			const int ThreadIdx = omp_get_thread_num();
			CRITICAL_SECTION &cs=m_csManager_MP[ThreadIdx];
			switch ( ObjType )
			{
			case AOI_OBJ_WND:
				AddFreeRecycleObjPtr(cs, m_RecycleWnd_MP[ThreadIdx], ObjPtr, bMP);
				break;
			case AOI_OBJ_LAND:
				AddFreeRecycleObjPtr(cs, m_RecycleLand_MP[ThreadIdx], ObjPtr, bMP);
				break;
			}			
		}
	#endif//OPEN_MP_USE
		return true;
	}	
#endif//USING_RECYCLE_MODE

	size_t i = 0;
	const size_t size = (size_t)(this->m_glbObjList.size());
	const size_t PtrIndex = ObjPtr->GetObjGlobalPtrIndex();

	if ( PtrIndex < size )
	{
		LockManager();
		if ( ObjPtr == m_glbObjList[PtrIndex] )
		{
			delete m_glbObjList[PtrIndex];
			m_glbObjList[PtrIndex] = NULL;
			ObjPtr = NULL;			
			CAOIObjManager::m_glbObjDelCount ++;
			UnlockManager();
			return true;
		}
		UnlockManager();
	}

	CString str;	
	str.Format(_T("CAOIObjManager::DestroyObj Exception(%s)"), CAOIObj::ObtainAOITypeText(ObjPtr->GetObjType()));	
	SetObjExceptionCode_Delete(str);
	JetAPI::ShowMessageBox(str);
	
	LockManager();
	for ( i=0; i<size; i++ )
	{
		if ( ObjPtr == m_glbObjList[i] )
		{
			delete m_glbObjList[i];
			m_glbObjList[i] = NULL;
			ObjPtr = NULL;			
			CAOIObjManager::m_glbObjDelCount ++;			
			break;
		}
	}
	UnlockManager();

	if ( NULL != ObjPtr )
	{	delete ObjPtr; ObjPtr=NULL; }
	return true;
#else
	delete ObjPtr; ObjPtr=NULL;
#endif//USING_OBJ_MANAGER
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIObjManager::ShowMessageBox(LPCTSTR str)
{
	JetAPI::ShowMessageBox(str);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::LockManagerFn(CRITICAL_SECTION &cs)
{	
	::EnterCriticalSection(&cs);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::UnlockManagerFn(CRITICAL_SECTION &cs)
{
	::LeaveCriticalSection(&cs);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIObjManager::CheckGlobalObjList()
{	
#ifdef USING_OBJ_MANAGER
	size_t       i=0;
	CString      str;
	bool         ShowMsg = true;
	CAOIObj     *ObjPtr = NULL;	
	CAOIFrame   *FramePtr = NULL;
	AOI_OBJ_TYPE ObjType = AOI_OBJ_BASIC;
	const size_t size = (size_t)(m_glbObjList.size());

	LockManager();
	for ( i=0; i<size; i++ )
	{
		ObjPtr = m_glbObjList[i];
		if ( NULL == ObjPtr ) { continue; }		

		ObjType = ObjPtr->GetObjType();
		str.Format(_T("Memory leaks (%s): %d"), CAOIObj::ObtainAOITypeText(ObjType), i+1);
		::_tprintf(_T("%s\n"), (LPCTSTR)str);

		switch ( ObjType )
		{
		case AOI_OBJ_FRAME:
			FramePtr = (CAOIFrame*)(ObjPtr);
			break;
		}
	#ifdef _DEBUG
		if ( ShowMsg == true ) 
		{			
			JetAPI::ShowMessageBox(str);
			ShowMsg = false;
		}
	#endif
	}
	UnlockManager();

#endif//USING_OBJ_MANAGER
}
//-------------------------------------------------------------------------------------//
void CAOIObjManager::DestroyRecycleObjList()
{
#ifdef USING_OBJ_MANAGER
#ifdef USING_RECYCLE_MODE
	LockManager();		
	size_t     NgCnt=0;
	size_t     PtrIndex = 0;
	CAOIObj   *ObjPtr = NULL;
	std::vector<CAOIObj*> &glbObjList = m_glbObjList;	
	const size_t size = glbObjList.size();
	ClearRecycleWndList();
	ClearRecycleLandList();	
	for ( size_t i=0; i<size; i++ )
	{
		if ( glbObjList[i] == NULL ) { continue; }		
		ObjPtr = glbObjList[i];		
		if ( ObjPtr->CheckObjRecycleMode() == false ) { continue; }
		if ( ObjPtr->GetObjUsed() == true )
		{	NgCnt ++; }		
		delete ObjPtr;
		glbObjList[i]=NULL;
	}
	UnlockManager();
#endif//USING_RECYCLE_MODE
#endif//USING_OBJ_MANAGER
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIObjManager::RearrangeGlobalObjList()
{
#ifdef USING_OBJ_MANAGER
	LockManager();

	size_t     i = 0;
	size_t     PtrIndex = 0;
	CAOIObj   *ObjPtr = NULL;
	std::vector<CAOIObj*>    glbObjList = CAOIObjManager::m_glbObjList;	
	const size_t size = glbObjList.size();

	CAOIObjManager::m_glbObjList.clear();
	for ( i=0; i<size; i++ )
	{
		if ( glbObjList[i] == NULL ) { continue; }
		ObjPtr = glbObjList[i];
		ObjPtr->SetObjGlobalPtrIndex(PtrIndex);
		CAOIObjManager::m_glbObjList.push_back(ObjPtr);
		PtrIndex ++;
	}
	CAOIObjManager::m_glbObjDelCount = 0;

	UnlockManager();
#endif
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIObjManager::ClearRecycleWndList()
{
	m_RecycleWnd.Clear();	
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIObjManager::GetFreeRecycleWndPtr()
{	
#ifdef USING_RECYCLE_MODE	
#ifdef OPEN_MP_USE
	if ( m_RecycleMP )
	{		
		const int ThreadIdx = omp_get_thread_num();
		CAOIObj *ObjPtr = GetFreeRecycleObjPtr(m_csManager_MP[ThreadIdx], m_RecycleWnd_MP[ThreadIdx]);
		if ( NULL == ObjPtr ) { return NULL; }
		return (CAOIWnd*)(ObjPtr);
	}
#endif//OPEN_MP_USE	
	CAOIObj *ObjPtr = GetFreeRecycleObjPtr(m_csManager, m_RecycleWnd);
	if ( NULL == ObjPtr ) { return NULL; }
	return (CAOIWnd*)(ObjPtr);
#endif//USING_RECYCLE_MODE
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOIObjManager::ClearRecycleLandList()
{
	m_RecycleLand.Clear();	
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIObjManager::GetFreeRecycleLandPtr()
{	
#ifdef USING_RECYCLE_MODE	
#ifdef OPEN_MP_USE
	if ( m_RecycleMP )
	{		
		const int ThreadIdx = omp_get_thread_num();
		CAOIObj *ObjPtr = GetFreeRecycleObjPtr(m_csManager_MP[ThreadIdx], m_RecycleLand_MP[ThreadIdx]);
		if ( NULL == ObjPtr ) { return NULL; }
		return (CAOILand*)(ObjPtr);		
	}
#endif//OPEN_MP_USE
	CAOIObj *ObjPtr = GetFreeRecycleObjPtr(m_csManager, m_RecycleLand);
	if ( NULL == ObjPtr ) { return NULL; }
	return (CAOILand*)(ObjPtr);
#endif//USING_RECYCLE_MODE
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIObj* CAOIObjManager::GetFreeRecycleObjPtr(CRITICAL_SECTION &cs, TObjRecycleNode &RecycleNode)
{
#ifdef USING_RECYCLE_MODE	
	LockManagerFn(cs);	
	std::vector<CAOIObj*> &List=RecycleNode.ObjList;
	const size_t Count = List.size();
	const size_t StartIdx = RecycleNode.StartIndex;
	for ( size_t i=StartIdx; i<Count; i++ )
	{
		if ( NULL == List[i] ) { continue; }
		CAOIObj *ObjPtr = List[i];
		if ( ObjPtr->GetObjUsed() == true ) { continue; }		
		ObjPtr->SetObjUsed(true);
		RecycleNode.StartIndex = i+1;
		UnlockManagerFn(cs);
		return ObjPtr;
	}
	RecycleNode.StartIndex = Count;
	UnlockManagerFn(cs);	
#endif//USING_RECYCLE_MODE
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::AddFreeRecycleObjPtr(CRITICAL_SECTION &cs, TObjRecycleNode &RecycleNode, CAOIObj *ObjPtr, bool bMP)
{
	LockManagerFn(cs);	
	size_t &StartIdx=RecycleNode.StartIndex;
	std::vector<CAOIObj*> &List=RecycleNode.ObjList;
	if ( ObjPtr->CheckObjRecycleIndex(bMP) == false)
	{	ObjPtr->AddInObjRecycleList(List, bMP);	}
	else
	{
		const size_t ReCycleIndex = ObjPtr->GetObjRecycleIndex(bMP);
		if ( ReCycleIndex < StartIdx )
		{	StartIdx = ReCycleIndex;	}				
	}
	UnlockManagerFn(cs);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::ClearRecycleObjList_MP()//睲埃碻吏ㄏノ-MP
{
#ifdef OPEN_MP_USE
	m_RecycleMP = false;	
	for ( int i=0 ;i<OBJ_MGR_MAX_MP_COUNT; i++ )
	{
		m_RecycleWnd_MP[i].Clear();		
		m_RecycleLand_MP[i].Clear();
	}
#endif//OPEN_MP_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::BuildRecycleObjList_MP(const TObjRecycleNode &RecycleNode, TObjRecycleNode Node_MP[], int MPCount)//俱碻吏ㄏノ	
{
	const bool bMP = true;	
	size_t Index = RecycleNode.StartIndex;	
	const std::vector<CAOIObj*> &List = RecycleNode.ObjList;
	const size_t Count=List.size();
	while ( true )
	{
		for ( int i=0; i<MPCount; i++ )
		{
			if ( Index >= Count ) { break; }
			if ( NULL != List[Index] )
			{	List[Index]->AddInObjRecycleList(Node_MP[i].ObjList, bMP);	}
			Index ++;
		}
		if ( Index >= Count ) { break; }
	};
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIObjManager::GetErrorString() const
{
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Object_Others(m_ErrorString);
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
void CAOIObjManager::SetObjExceptionCode_Create(LPCTSTR Err)
{	
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Object(AOI_EXCEPTION_OBJECT_CREATE, str);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_System(AOI_EXCEPTION_SYSTEM_OBJECT_CREATE, str);
}
//-------------------------------------------------------------------------------------//
void CAOIObjManager::SetObjExceptionCode_Delete(LPCTSTR Err)
{	
	CString str = (NULL!=Err) ? Err:m_ErrorString;
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Object(AOI_EXCEPTION_OBJECT_DELETE, str);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_System(AOI_EXCEPTION_SYSTEM_OBJECT_DELETE, str);	
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::LockManager()
{
	return LockManagerFn(m_csManager);
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::UnlockManager()
{
	return UnlockManagerFn(m_csManager);
}
//-------------------------------------------------------------------------------------//
CString CAOIObjManager::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("AOI_OBJ_MANAGER");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CAOIFd* CAOIObjManager::CreateFdObj()//承 CAOIFd ン
{
	CAOIFd *ObjPtr = NULL;	
	ObjPtr = new CAOIFd();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIFd Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}
	CAOIObjManager::AddObjPtr(ObjPtr);	
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyFdObj(CAOIFd *&FdPtr)//篟反 CAOIFd ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = FdPtr;
	IsOK = DestroyObject(TempPtr);
	FdPtr = NULL;
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyFdList(std::vector<CAOIFd*> &List)//篟反 CAOIFd
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyFdObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIObj* CAOIObjManager::CreateObject()
{
	CAOIObj *ObjPtr = NULL;	
	ObjPtr = new CAOIObj();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIObj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);	
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyAOIObj(CAOIObj *&ObjPtr)
{
	bool IsOK = true;
	CAOIObj *TempPtr = ObjPtr;
	IsOK = DestroyObject(TempPtr);
	ObjPtr = NULL;
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyAOIList(std::vector<CAOIObj*> &List)//篟反 CAOIObj
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyAOIObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIObjManager::CreateRgnObj()//承CAOIRgnン
{
	CAOIRgn *ObjPtr = NULL;	
	ObjPtr = new CAOIRgn();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIRgn Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);	
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyRgnObj(CAOIRgn *&RgnPtr)
{
	bool IsOK = true;
	CAOIObj *TempPtr = RgnPtr;
	IsOK = DestroyObject(TempPtr);
	RgnPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyRgnList(std::vector<CAOIRgn*> &List)//篟反 CAOIRgn
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyRgnObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOISlice* CAOIObjManager::CreateSliceObj()//承 CAOISlice ン
{
	CAOISlice *ObjPtr = NULL;	
	ObjPtr = new CAOISlice();
	if ( ObjPtr == NULL )
	{	
		CString str = _T("Error, Create CAOISlice Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroySliceObj(CAOISlice *&SlicePtr)//篟反 CAOISlice ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = SlicePtr;
	IsOK = DestroyObject(TempPtr);
	SlicePtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroySliceList(std::vector<CAOISlice*> &List)//篟反 CAOISlice
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroySliceObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIObjManager::CreateFieldObj()//承 CAOIField ン
{
	CAOIField *ObjPtr = NULL;	
	ObjPtr = new CAOIField();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIField Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyFieldObj(CAOIField *&FieldPtr)//篟反 CAOIField ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = FieldPtr;
	IsOK = DestroyObject(TempPtr);
	FieldPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyFieldList(std::vector<CAOIField*> &List)//篟反 CAOIField
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyFieldObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIFrame* CAOIObjManager::CreateFrameObj()//承 CAOIFrame ン
{
	CAOIFrame *ObjPtr = NULL;	
	ObjPtr = new CAOIFrame();
	if ( ObjPtr == NULL )
	{	
		CString str = _T("Error, Create CAOIFrame Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyFrameObj(CAOIFrame *&FramePtr)//篟反 CAOIFrame ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = FramePtr;
	IsOK = DestroyObject(TempPtr);
	FramePtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyFrameList(std::vector<CAOIFrame*> &List)//篟反 CAOIFrame
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyFrameObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIBoard* CAOIObjManager::CreateBoardObj()//承 CAOIBoard ン
{
	CAOIBoard *ObjPtr = NULL;	
	ObjPtr = new CAOIBoard();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIBoard Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyBoardObj(CAOIBoard *&BoardPtr)//篟反 CAOIBoard ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = BoardPtr;
	IsOK = DestroyObject(TempPtr);
	BoardPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyBoardList(std::vector<CAOIBoard*> &List)//篟反 CAOIBoard
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyBoardObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CAOIObjManager::CreatePanelObj()//承 CAOIPanel ン
{
	CAOIPanel *ObjPtr = NULL;	
	ObjPtr = new CAOIPanel();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIPanel Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);	
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyPanelObj(CAOIPanel *&PanelPtr)//篟反 CAOIPanel ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = PanelPtr;
	IsOK = DestroyObject(TempPtr);
	PanelPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyPanelList(std::vector<CAOIPanel*> &List)//篟反 CAOIPanel
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyPanelObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIWindow* CAOIObjManager::CreateWindowObj()//承 CAOIWindow ン
{
	CAOIWindow *ObjPtr = NULL;	
	ObjPtr = new CAOIWindow();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIWindow Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);	
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyWindowObj(CAOIWindow *&WindowPtr)//篟反 CAOIWindow ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = WindowPtr;
	IsOK = DestroyObject(TempPtr);
	WindowPtr = NULL;
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyWindowList(std::vector<CAOIWindow*> &List)//篟反 CAOIWindow
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyWindowObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CAOIObjManager::CreateProjectObj()//承 CAOIProject ン
{
	CAOIProject *ObjPtr = NULL;	
	ObjPtr = new CAOIProject();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIProject Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyProjectObj(CAOIProject *&ProjectPtr)//篟反 CAOIProject ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = ProjectPtr;
	IsOK = DestroyObject(TempPtr);
	ProjectPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyProjectList(std::vector<CAOIProject*> &List)//篟反 CAOIProject
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyProjectObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIObjManager::CreateComponentObj()//承 CAOIComponent ン
{
	CAOIComponent *ObjPtr = NULL;	
	ObjPtr = new CAOIComponent();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIComponent Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyComponentObj(CAOIComponent *&CmpPtr)//篟反 CAOIComponent ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = CmpPtr;
	IsOK = DestroyObject(TempPtr);
	CmpPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyComponentList(std::vector<CAOIComponent*> &List)//篟反 CAOIComponent
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyComponentObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIFov* CAOIObjManager::CreateFovObj()//承 CAOIFov ン
{
	CAOIFov *ObjPtr = NULL;	
	ObjPtr = new CAOIFov();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIFov Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyFovObj(CAOIFov *&FovPtr)//篟反 CAOIFov ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = FovPtr;
	IsOK = DestroyObject(TempPtr);
	FovPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyFovList(std::vector<CAOIFov*> &List)//篟反 CAOIFov
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyFovObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIBox* CAOIObjManager::CreateBoxObj()//承 CAOIBox ン
{
	CAOIBox *ObjPtr = NULL;	
	ObjPtr = new CAOIBox();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIBox Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyBoxObj(CAOIBox *&BoxPtr)//篟反 CAOIBox ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = BoxPtr;
	IsOK = DestroyObject(TempPtr);
	BoxPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyBoxList(std::vector<CAOIBox*> &List)//篟反 CAOIBox
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyBoxObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIObjManager::CreateWndObj()//承 CAOIWnd ン
{
	CAOIWnd *ObjPtr = NULL;
	ObjPtr = GetFreeRecycleWndPtr();
	if ( NULL != ObjPtr )
	{
		ObjPtr->ResetWndObj();
		return ObjPtr; 
	}

	ObjPtr = new CAOIWnd();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIWnd Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyWndObj(CAOIWnd *&WndPtr)//篟反 CAOIWnd ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = WndPtr;
	IsOK = DestroyObject(TempPtr);
	WndPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyWndList(std::vector<CAOIWnd*> &List)//篟反 CAOIWnd
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyWndObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOIObjManager::CreateLandObj()//承 CAOILand ン
{
	CAOILand *ObjPtr = NULL;
	ObjPtr = GetFreeRecycleLandPtr();	
	if ( NULL != ObjPtr )
	{
		ObjPtr->ResetLandObj();
		return ObjPtr; 
	}

	ObjPtr = new CAOILand();
	if ( ObjPtr == NULL )
	{	
		CString str = _T("Error, Create CAOILand Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyLandObj(CAOILand *&LandPtr)//篟反 CAOILand ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = LandPtr;
	IsOK = DestroyObject(TempPtr);
	LandPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyLandList(std::vector<CAOILand*> &List)//篟反 CAOILand
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyLandObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIModel* CAOIObjManager::CreateModelObj()//承 CAOIModel ン
{
	CAOIModel *ObjPtr = NULL;	
	ObjPtr = new CAOIModel();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIModel Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyModelObj(CAOIModel *&ModelPtr)//篟反 CAOIModel ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = ModelPtr;
	IsOK = DestroyObject(TempPtr);
	ModelPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyModelList(std::vector<CAOIModel*> &List)//篟反 CAOIModel
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyModelObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOIObjManager::CreateLogicObj()//承 CAOILogic ン 
{
	CAOILogic *ObjPtr = NULL;	
	ObjPtr = new CAOILogic();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOILogic Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyLogicObj(CAOILogic *&LogicPtr)//篟反 CAOILogic ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = LogicPtr;
	IsOK = DestroyObject(TempPtr);
	LogicPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyLogicList(std::vector<CAOILogic*> &List)//篟反 CAOILogic
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyLogicObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIWndRoi* CAOIObjManager::CreateWndRoiObj()//承ン CAOIWndRoi ン
{
	CAOIWndRoi *ObjPtr = NULL;	
	ObjPtr = new CAOIWndRoi();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIWndRoi Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyWndRoiObj(CAOIWndRoi *&WndRoiPtr)//篟反 CAOIWndRoi ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = WndRoiPtr;
	IsOK = DestroyObject(TempPtr);
	WndRoiPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyWndRoiList(std::vector<CAOIWndRoi*> &List)//篟反 CAOIWndRoi
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyWndRoiObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIWndMask* CAOIObjManager::CreateWndMaskObj()//承ン CAOIWndMask ン
{
	CAOIWndMask *ObjPtr = NULL;	
	ObjPtr = new CAOIWndMask();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIWndMask Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyWndMaskObj(CAOIWndMask *&WndMaskPtr)//篟反 CAOIWndMask ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = WndMaskPtr;
	IsOK = DestroyObject(TempPtr);
	WndMaskPtr = NULL;
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyWndMaskList(std::vector<CAOIWndMask*> &List)//篟反 CAOIWndMask
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyWndMaskObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIBarcode* CAOIObjManager::CreateBarcodeObj()//承ン CAOIBarcode ン
{
	CAOIBarcode *ObjPtr = NULL;	
	ObjPtr = new CAOIBarcode();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIBarcode Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyBarcodeObj(CAOIBarcode *&BarcodePtr)//篟反 CAOIBarcode ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = BarcodePtr;
	IsOK = DestroyObject(TempPtr);
	BarcodePtr = NULL;
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIObjManager::DestroyBarcodeList(std::vector<CAOIBarcode*> &List)//篟反 CAOIBarcode
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyBarcodeObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
CAOIMark* CAOIObjManager::CreateMarkObj()//承 CAOIMark ン
{
	CAOIMark *ObjPtr = NULL;	
	ObjPtr = new CAOIMark();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIMark Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------/
bool CAOIObjManager::DestroyMarkObj(CAOIMark *&MarkPtr)//篟反 CAOIMark ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = MarkPtr;
	IsOK = DestroyObject(TempPtr);
	MarkPtr = NULL;
	return IsOK;	
}
//-------------------------------------------------------------------------------------/
bool CAOIObjManager::DestroyMarkList(std::vector<CAOIMark*> &List)//篟反 CAOIMark
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyMarkObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------/
CAOIPartGroup* CAOIObjManager::CreatePartGroupObj()//承 CAOIPartGroup ン
{
	CAOIPartGroup *ObjPtr = NULL;	
	ObjPtr = new CAOIPartGroup();
	if ( ObjPtr == NULL )
	{
		CString str = _T("Error, Create CAOIPartGroup Obj Fault");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetObjExceptionCode_Create();
		return NULL; 
	}

	CAOIObjManager::AddObjPtr(ObjPtr);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------/
bool CAOIObjManager::DestroyPartGroupObj(CAOIPartGroup *&PartGroupPtr)//篟反 CAOIPartGroup ン
{
	bool IsOK = true;
	CAOIObj *TempPtr = PartGroupPtr;
	IsOK = DestroyObject(TempPtr);
	PartGroupPtr = NULL;
	return IsOK;	
}
//-------------------------------------------------------------------------------------/
bool CAOIObjManager::DestroyPartGroupList(std::vector<CAOIPartGroup*> &List)//篟反 CAOIPartGroup
{
	size_t i=0;
	bool IsOK = true;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	
		if ( DestroyPartGroupObj(List[i]) == false )
		{	IsOK = false; }
	}
	List.clear();
	return IsOK;
}
//-------------------------------------------------------------------------------------/
void CAOIObjManager::LayoutRecycleObjList(int MPCount)//俱碻吏ㄏノ
{
#ifdef USING_RECYCLE_MODE
	LockManager();	
	const bool bMP = false;
	const std::vector<CAOIObj*> &ObjList=m_glbObjList;
	const size_t ObjCount = ObjList.size();
	ClearRecycleWndList();
	ClearRecycleLandList();		
	ClearRecycleObjList_MP();
	for ( size_t i=0; i<ObjCount; i++ )
	{
		if ( NULL == ObjList[i] ) { continue; }
		CAOIObj *ObjPtr = ObjList[i];
		if ( ObjPtr->CheckObjRecycleMode() == false ) { continue; }

		AOI_OBJ_TYPE ObjType = ObjPtr->GetObjType();
		if ( ObjPtr->GetObjUsed() == true ) { continue; }
		switch ( ObjType )
		{
		case AOI_OBJ_WND:
			ObjPtr->AddInObjRecycleList(m_RecycleWnd.ObjList, bMP);			
			break;
		case AOI_OBJ_LAND:
			ObjPtr->AddInObjRecycleList(m_RecycleLand.ObjList, bMP);			
			break;
		}		
	}	
	UnlockManager();

#ifdef OPEN_MP_USE
	if ( MPCount>1 && MPCount<OBJ_MGR_MAX_MP_COUNT )
	{
		m_RecycleMP = true;
		BuildRecycleObjList_MP(m_RecycleWnd, m_RecycleWnd_MP, MPCount);
		BuildRecycleObjList_MP(m_RecycleLand, m_RecycleLand_MP, MPCount);
	}
#endif//OPEN_MP_USE
#endif//USING_RECYCLE_MODE
	return ;
}
//-------------------------------------------------------------------------------------//