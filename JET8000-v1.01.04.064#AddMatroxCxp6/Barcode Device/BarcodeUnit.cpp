#include "stdafx.h"
#include "BarcodeUnit.h"
#include <process.h>

#include "Honeywell_3310GHD.h"

#if (!_ONLY_HONEYWELL_3310)
#include "Honeywell_1900.h"
#include "DataLogic_1000.h"
#include "DataLogic_Matrix210.h"
#include "DataLogic_Matrix200.h"
#include "DataLogic_GFS4400.h"
//
#include "MicroScan_Mini.h"
#include "MicroScan_Mini3.h"
#include "MicroScan_MiniHawk.h"
#include "MicroScan_Velocity.h"
#include "MicroScan_MS3.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int s_ABC = 0;

enum BarcodeThreadEventType
{	
	BTET_EXIT,	
	BTET_RUN, //event high run, low idle	
	TET_TOTAL,
};

enum BarcodeDataFinishStatus
{
	BDFS_NO_ENTRY,
	BDFS_ENTRY_FINISH,
};

class CThreadObj
{	
	void init()
	{
		m_pBarcodePtr = NULL;
		m_DataFinish = BDFS_NO_ENTRY;
		m_hThread = NULL;
		m_ThreadId = 0;
		m_ObjId = -1;
		m_vEvents.resize(TET_TOTAL);
		m_vBarcodeList.clear();
		m_ErrorString = "";
		m_TriggerTimeoutMs = -1;
	}

	std::string					m_ErrorString;

	//<exit, run>
	std::vector<HANDLE>			m_vEvents;
	//
	int							m_DataFinish;
public:
	CThreadObj()
	{
		init();
	}

	CThreadObj(int ObjId)
	{
		init();
		m_ObjId = ObjId;
	}

	~CThreadObj()
	{}

	CThreadObj &operator=(const CThreadObj &rhs)
	{
		this->m_pBarcodePtr		= rhs.m_pBarcodePtr;
		this->m_vEvents			= rhs.m_vEvents;		
		this->m_DataFinish		= rhs.m_DataFinish;
		this->m_hThread			= rhs.m_hThread;
		this->m_ThreadId		= rhs.m_ThreadId;
		this->m_vBarcodeList	= rhs.m_vBarcodeList;
		this->m_ErrorString		= rhs.m_ErrorString;
		this->m_ObjId			= rhs.m_ObjId;
		this->m_TriggerTimeoutMs= rhs.m_TriggerTimeoutMs;
		return *this;
	}

	void SetEvents(HANDLE hExitHandle, HANDLE hRunHandle)
	{
		m_vEvents[BTET_EXIT] = hExitHandle;
		m_vEvents[BTET_RUN] = hRunHandle;		
	}

	BarCode_CS					*m_pBarcodePtr;
	//
	HANDLE						m_hThread;
	unsigned int				m_ThreadId;
	std::vector<std::string>	m_vBarcodeList;	
	int							m_ObjId;
	int							m_TriggerTimeoutMs;

	void SetDataFinishStatus(int Status)
	{
		m_DataFinish = Status;
	}

	int GetDataFinishStatus() const
	{
		return m_DataFinish;
	}

	bool IsLegalHandle()
	{
		if ( (NULL == m_pBarcodePtr) || (NULL == m_hThread)
			|| (NULL == m_vEvents[BTET_RUN]) || (NULL == m_vEvents[BTET_EXIT] ) )
		{
			return false;
		}
		return true;
	}

	const std::string *GetErrString() const
	{
		return (&m_ErrorString);
	}

	void SetErrString(const std::string &rErrStr)
	{
		m_ErrorString = rErrStr;
	}

	void CreateEvents()
	{		
		for (int j=0;j < TET_TOTAL;++j)
		{
			CString EventName;
			EventName.Format(_T("Barcode%d-Event-%d"), m_ObjId, j);
			m_vEvents[j] = ::CreateEvent(NULL, TRUE, TRUE, EventName);
			::ResetEvent(m_vEvents[j]);
		}
	}

	void DeleteEvents()
	{
		for (int j=0;j < TET_TOTAL;++j)
		{
			if ( NULL != m_vEvents[j])
			{
				::CloseHandle(m_vEvents[j]);
				m_vEvents[j] = NULL;
			}
		}
	}

	void ResetEvents(int Id)
	{
		if (NULL != m_vEvents[Id])
		{
			::ResetEvent(m_vEvents[Id]);
		}
	}

	void SetEvents(int Id)
	{
		if (NULL != m_vEvents[Id])
		{
			::SetEvent(m_vEvents[Id]);
		}
	}

	DWORD WaitThreadEvent(DWORD WaitTime)
	{	
		HANDLE pHandleList[] = {m_vEvents[BTET_EXIT], m_vEvents[BTET_RUN] };

		DWORD EId = WaitForMultipleObjects(TET_TOTAL, pHandleList, FALSE, WaitTime);

		if (WAIT_OBJECT_0 == EId)
		{
			EId = BTET_EXIT;
			::ResetEvent(pHandleList[BTET_EXIT]);
		}
		else if ( (WAIT_OBJECT_0 + 1)== EId)
		{
			EId = BTET_RUN;
		}	

		return EId;
	}

	void FetchBarcodeList()
	{
		m_vBarcodeList.clear();
		m_pBarcodePtr->GetCode_N(m_vBarcodeList);		
	}
};

static unsigned int __stdcall BarcodeReadingThreadFn(void *pParam);

class CBarcodeUnitImpl
{
	bool StartBarcodeReadingThread(CThreadObj *pThreadObj)
	{
		if (NULL == pThreadObj->m_hThread)
		{
			s_strLastErr = "Eorror, Barcode Thread Fault (ThreadHandle == NULL)";
			pThreadObj->SetErrString(s_strLastErr);
		}

		::SetThreadPriority(pThreadObj->m_hThread,
								THREAD_PRIORITY_BELOW_NORMAL);

		pThreadObj->SetEvents(BTET_RUN);
		//
		return true;
	}	

	void CloseHandleV(HANDLE *pHandle)
	{
		if ( (*pHandle) != NULL )
		{
			::CloseHandle( (*pHandle) );
			(*pHandle) = NULL;
		}
	}

	void DeleteBarcodePtr(BarCode_CS** pObj)
	{
		if ( (*pObj) != NULL )
		{
			delete *pObj;
			(*pObj) = NULL;
		}
	}

	int CreateObjectErr(int BarcodeId, int BarcodeType)
	{
		std::string BarcodeName(Barcode_API::GetBarcodeReaderName(BarcodeType) );
		char ErrMsg[128];
		sprintf(ErrMsg, "Create Barcode Object Fault - %s (BarcodeID:%d)", 
							BarcodeName.c_str(), BarcodeId);
		s_strLastErr = ErrMsg;

		BarcodeType = BARCODE_TYPE_NULL;

		return BarcodeType;
	}

	int CreateBarcodePtr(BarCode_CS** pBarcodeServer, int BarcodeId, int BarcodeType)
	{
		switch ( BarcodeType )
		{
		case BARCODE_HONEYWELL_1900:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new Honeywell_1900;
	#endif
		break;
		case BARCODE_HONEYWELL_3310GHD://Barcode Honeywell 3310GHD
			(*pBarcodeServer) = new Honeywell_3310GHD;		
		break;
		case BARCODE_DATALOGIC_M1000:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new DataLogic_1000;
	#endif
		break;
		case BARCODE_DATALOGIC_MATRIX_210: //barcode datalogic matrix 210
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new DataLogic_Matrix210;
	#endif
		break;
		case BARCODE_DATALOGIC_MATRIX_200:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new DataLogic_Matrix200;
	#endif
		break;
		case BARCODE_DATALOGIC_GFS4400:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new DataLogic_GFS4400;
	#endif
		case BARCODE_MICROSCAN_MINI:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new MicroScan_Mini;
	#endif
		break;
		case BARCODE_MICROSCAN_MINI3:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new MicroScan_Mini3;
	#endif
		break;
		case BARCODE_MICROSCAN_MINI_HAWK:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new MicroScan_MiniHawk;
	#endif
		break;
		case BARCODE_MICROSCAN_MINI_VELOCITY:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new MicroScan_Velocity;
	#endif
		break;
		case BARCODE_MICROSCAN_MS3:
	#if (!_ONLY_HONEYWELL_3310)
			(*pBarcodeServer) = new MicroScan_MS3;
	#endif
		break;
		default:
			BarcodeType = BARCODE_TYPE_NULL;
		break;
		}

		//
		if ( NULL == (*pBarcodeServer) )
		{
			BarcodeType = CreateObjectErr(BarcodeId, BarcodeType);			
		}

		if (BARCODE_TYPE_NULL != BarcodeType)
		{
			(*pBarcodeServer)->SetBarcodeID(BarcodeId);			
		}

		return BarcodeType;
	}	

	bool DeleteBarcodeReadingThread(CThreadObj **pObj)
	{
		//stop barcode reading therad
		(*pObj)->SetEvents(BTET_EXIT);		

		if (NULL == (*pObj)->m_hThread)
		{
			return true;
		}
	
		//wait end of thread
		DWORD Res = ::WaitForSingleObject((*pObj)->m_hThread, INFINITE);

		CloseHandleV(&( (*pObj)->m_hThread) );	

		(*pObj)->DeleteEvents();

		return true;
	}	

	bool CreateBarcodeReadingThread(CThreadObj *pThradObj)
	{
		if (false == DeleteBarcodeReadingThread(&pThradObj) )
		{
			return false;
		}

		BarCode_CS *pBarcodePtr = pThradObj->m_pBarcodePtr;

		if (NULL == pBarcodePtr) return false;

		//
		pThradObj->CreateEvents();
	
		pThradObj->m_hThread = 
			(HANDLE)::_beginthreadex(NULL, NULL, &BarcodeReadingThreadFn,
							(void*)(pThradObj), NULL, &(pThradObj->m_ThreadId) );

		Sleep(200);		
	
		if ( NULL == pThradObj->m_hThread)
		{				
			s_strLastErr = "Eorror, Create Thread Fault (ThreadHandle == NULL)";
			pThradObj->SetErrString(s_strLastErr);
			return false;
		}

		::SetThreadPriority(pThradObj->m_hThread, THREAD_PRIORITY_LOWEST);
		::SetThreadAffinityMask(pThradObj->m_hThread, 0x03);			

		return true;
	}

	void DeleteCreateBarcodeObject_Kernel(CThreadObj *pThreadObj)
	{
		if (pThreadObj->m_pBarcodePtr)
		{
			DeleteBarcodeReadingThread(&pThreadObj);
			
			DeleteBarcodePtr(&(pThreadObj->m_pBarcodePtr) );
		}
	}

	bool CreateBarcodeObject_Kernel(CThreadObj *pThreadObj, const CBarcodeParameter &rParas)
	{
		DeleteCreateBarcodeObject_Kernel(pThreadObj);

		int BId = pThreadObj->m_ObjId;

		if (BARCODE_TYPE_NULL == 
			CreateBarcodePtr(&(pThreadObj->m_pBarcodePtr), BId, rParas.m_BarcodeType) )
		{
			return false;
		}

		BarCode_CS *pBarcodeServer = pThreadObj->m_pBarcodePtr;

		//pBarcodeServer->SetBarcodeID(BId);
		pBarcodeServer->SetTimeout(rParas.m_WaitTimeoutMs);

		if (false == rParas.m_iniPath.empty() )
			pBarcodeServer->SetINIFilePath(rParas.m_iniPath);

		if ( pBarcodeServer->SetComPort(rParas.m_COMPort) == false ) 
		{ 	
			s_strLastErr = pBarcodeServer->GetErrMsg();						
			pThreadObj->SetErrString(s_strLastErr);
			return false;
		}

		if (rParas.m_InitializeSW)
		{
			pBarcodeServer->SetInitializeFlag(false);
			if( pBarcodeServer->Initialize()== false)
			{
				s_strLastErr = pBarcodeServer->GetErrMsg();
				pThreadObj->SetErrString(s_strLastErr);
				return false;
			}
		}
		else
		{
			pBarcodeServer->SetInitializeFlag(true);
		}

		return true;
	}

	int IsLegalBarcodeId(int BarcodeId)
	{
		for (size_t j=s_vBarcodeThreadObj.size();j--;)
		{
			if (BarcodeId == s_vBarcodeThreadObj[j]->m_ObjId)
			{
				return j;
			}
		}

		return -1;
	}

	CThreadObj *GetThreadObjByBarcodeId(int BarcodeId)
	{
		int SerId = -1;
		if (0 <= (SerId = IsLegalBarcodeId(BarcodeId) ) )
		{
			return (s_vBarcodeThreadObj[SerId]);
		}

		return NULL;
	}

	CThreadObj *GetThreadObjByComport(int ComportId, int BarcodeId)
	{
		if (s_vBarcodeThreadObj.size() )
		{
			//check same comport
			for (size_t j=s_vBarcodeThreadObj.size();j--;)
			{
				//if (ComportId == s_vBarcodeThreadObj[j]->m_pBarcodePtr->GetComPort() )
				if (BarcodeId == s_vBarcodeThreadObj[j]->m_pBarcodePtr->GetBarcodeID() )
				{
					return s_vBarcodeThreadObj[j];
				}
			}
		}		

		//int BId = static_cast<int>(s_vBarcodeThreadObj.size() );
		int BId = BarcodeId;

		s_vBarcodeThreadObj.push_back(new CThreadObj(BId) );

		return s_vBarcodeThreadObj.back();
	}

public:
	CBarcodeUnitImpl()
	{
	}

	~CBarcodeUnitImpl()
	{
	}

	static std::vector<CThreadObj*> s_vBarcodeThreadObj;
	static std::string s_strLastErr;

	//
	int CreateBarcodeObject(const CBarcodeParameter &rParas)
	{	
		CThreadObj *pThradObj = GetThreadObjByComport(rParas.m_COMPort, rParas.m_BarcodeID);

		//create barcode object
		bool IsOK = CreateBarcodeObject_Kernel(pThradObj, rParas);

		if ( IsOK == false ) return -1;

		//set time out message
		pThradObj->m_TriggerTimeoutMs = rParas.m_WaitTimeoutMs;

		//create thread handle
		IsOK = CreateBarcodeReadingThread(pThradObj);

		if ( IsOK == false )
		{
			if (pThradObj->m_pBarcodePtr)
				pThradObj->SetErrString(pThradObj->m_pBarcodePtr->GetErrMsg() );

			return -1;
		}
	
		return pThradObj->m_ObjId;
	}

	bool DelectBarcodeObject(int BarcodeId)
	{			
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BarcodeId);
		if (pThreadObj)
		{
			StopFire(pThreadObj);

			DeleteBarcodeReadingThread(&pThreadObj);

			DeleteBarcodePtr(&pThreadObj->m_pBarcodePtr);

			int SerId = IsLegalBarcodeId(BarcodeId);
			delete s_vBarcodeThreadObj[SerId];
			s_vBarcodeThreadObj.erase(s_vBarcodeThreadObj.begin() + SerId);

			return true;
		}

		return false;
	}

	bool SetBarcodeMultiCode(int BarcodeId, int Count)
	{
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BarcodeId);
		if (NULL != pThreadObj->m_pBarcodePtr)
		{
			return (pThreadObj->m_pBarcodePtr->SetNMultiCodes(Count) );
		}

		return false;
	}

	void StopFire(CThreadObj *pThreadObj)
	{
		BarCode_CS *pBarcodeServer = pThreadObj->m_pBarcodePtr;
		if (pBarcodeServer)
		{
			pBarcodeServer->EndRead();
			//pBarcodeServer->ClearBuffer();
		}
		
		Sleep(110);

		pThreadObj->ResetEvents(BTET_EXIT);
		//pThreadObj->ResetEvents(BTET_RUN);
	}

	bool TriggerBarcode(int BId)
	{
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BId);

		if ( (NULL == pThreadObj) || (false == pThreadObj->IsLegalHandle() ) )
			return false;
		
		StopFire(pThreadObj);		

		//set read Data Status
		pThreadObj->SetDataFinishStatus(BDFS_NO_ENTRY);

		if (StartBarcodeReadingThread(pThreadObj) )
			return (pThreadObj->m_pBarcodePtr->Trigger() );		

		return false;
	}

	bool TurnOffBarcode(int BId)
	{
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BId);

		if ( (NULL == pThreadObj) || (NULL == pThreadObj->m_pBarcodePtr) )
			return false;
		
		StopFire(pThreadObj);		

		return true;
	}

	bool SetTriggerTimeoutMs(int BId, int TimeoutMs)
	{
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BId);

		if (NULL == pThreadObj)
			return false;

		pThreadObj->m_TriggerTimeoutMs = TimeoutMs;

		return true;
	}

	bool WaitBarcodeReading(int BarcodeId)
	{	
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BarcodeId);

		if (NULL == pThreadObj) return false;
		
		return (pThreadObj->GetDataFinishStatus() ? true : false);
	}

	bool GetBarcodeData(int BarcodeId, std::vector<std::string> &rBarcodeList)
	{
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BarcodeId);

		if (NULL == pThreadObj) return false;

		rBarcodeList = pThreadObj->m_vBarcodeList;

		return rBarcodeList.size() ? true : false;	
	}

	bool ClearBarcodeData(int BarcodeId)
	{
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BarcodeId);

		if (NULL == pThreadObj) return false;

		pThreadObj->m_vBarcodeList.clear();

		pThreadObj->SetDataFinishStatus(BDFS_NO_ENTRY);

		return true;
	}

	const std::string *GetErrorString(int BId)
	{
		CThreadObj *pThreadObj = GetThreadObjByBarcodeId(BId);

		if (pThreadObj)
			return (pThreadObj->GetErrString() );		

		return (&s_strLastErr);
	}

	void DeleteAllObject()
	{
		for (size_t j= s_vBarcodeThreadObj.size();j--;)
		{
			DelectBarcodeObject(s_vBarcodeThreadObj[j]->m_ObjId);
			//delete s_vBarcodeThreadObj[j];
		}
	}
};

std::vector<CThreadObj*> CBarcodeUnitImpl::s_vBarcodeThreadObj;
std::string CBarcodeUnitImpl::s_strLastErr = "";

static unsigned int __stdcall BarcodeReadingThreadFn(void *pParam)
{	
	const int WaitTime = 10;

	CThreadObj *pThreadObj = (CThreadObj *)(pParam);	

	BarCode_CS *pBarcodeServer = pThreadObj->m_pBarcodePtr;

	int nTimeoutMax = pBarcodeServer->GetTimeout()/100;
	int nCount = 0;

	while (true)
	{
		DWORD EId = pThreadObj->WaitThreadEvent(WaitTime);

		if (BTET_EXIT == EId)
			break;

		if (BTET_RUN == EId)
		{			
			if ( pBarcodeServer->DoBarcodeReadFn() == true )
			{
				pThreadObj->ResetEvents(BTET_RUN);
				
				//fetch barcode data
				pThreadObj->FetchBarcodeList();			

				//set read finish
				pThreadObj->SetDataFinishStatus(BDFS_ENTRY_FINISH);

				nCount = 0;
			}
			else if (0 < nTimeoutMax)
			{
				//check trigger time out
				if (nTimeoutMax < nCount )
				{	
					//stop read data if time out					
					pBarcodeServer->EndRead();
					nCount = 0;
				}

				++nCount;
			}
		}
		else if (WAIT_TIMEOUT == EId)
		{
			if (pBarcodeServer->GetTimeout() != pThreadObj->m_TriggerTimeoutMs)
			{
				nTimeoutMax = pBarcodeServer->GetTimeout()/100;
				pBarcodeServer->SetTimeout(pThreadObj->m_TriggerTimeoutMs);
			}
		}
	}

	return 0;	
}

CBarcodeUnit::CBarcodeUnit()
{
	_pImpl = new CBarcodeUnitImpl();
}

CBarcodeUnit::~CBarcodeUnit()
{
	//delete object	in the buffer
	_pImpl->DeleteAllObject();

	delete _pImpl;
}

int CBarcodeUnit::CreateBarcodeObject(const CBarcodeParameter &rParas)
{
	return _pImpl->CreateBarcodeObject(rParas);
}

bool CBarcodeUnit::DelectBarcodeObject(int BarcodeId)
{
	return _pImpl->DelectBarcodeObject(BarcodeId);
}

bool CBarcodeUnit::SetBarcodeMultiCode(int BarcodeId, int Count)
{
	return _pImpl->SetBarcodeMultiCode(BarcodeId, Count);
}

bool CBarcodeUnit::TriggerBarcode(int BarcodeId)
{
	return _pImpl->TriggerBarcode(BarcodeId);
}

bool CBarcodeUnit::TurnOffBarcode(int BarcodeId)
{	
	return _pImpl->TurnOffBarcode(BarcodeId);
}

bool CBarcodeUnit::SetTriggerTimeoutMs(int BarcodeId, int TimeoutMs)
{
	return _pImpl->SetTriggerTimeoutMs(BarcodeId, TimeoutMs);
}

bool CBarcodeUnit::WaitBarcodeReading(int BarcodeId)
{	
	return _pImpl->WaitBarcodeReading(BarcodeId);
}

bool CBarcodeUnit::GetBarcodeData(int BarcodeId, std::vector<std::string> &rBarcodeList)
{
	return _pImpl->GetBarcodeData(BarcodeId, rBarcodeList);
}

bool CBarcodeUnit::ClearBarcodeData(int BarcodeId)
{
	return _pImpl->ClearBarcodeData(BarcodeId);
}

const std::string *CBarcodeUnit::GetErrorString(int BId)
{
	return (_pImpl->GetErrorString(BId) );
}

const std::string CBarcodeUnit::GetBarcodeDeviceName(BarcodeDeviceType Type)
{
	return (Barcode_API::GetBarcodeReaderName(Type) );
}

CBarcodeUnit &theBarcodeUnitCtrl()
{
	static CBarcodeUnit theCtrl;
	return theCtrl;	
}