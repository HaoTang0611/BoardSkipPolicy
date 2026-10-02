// LogManager.h: interface for the CLogManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMANAGER_H__832FD291_E602_462F_BBDE_C8D8F4F26599__INCLUDED_)
#define AFX_LOGMANAGER_H__832FD291_E602_462F_BBDE_C8D8F4F26599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "LogNode.h"
//-------------------------------------------------------------------------------------//
class CLogManager  
{
private:
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;
	std::vector<CLogNode>      m_LogList;	
	CRITICAL_SECTION           m_csLogManager;//同步化	
	DWORD                      m_ThreadDwellTime;//執行緒延遲時間
	//---------------------------------------------------------------------------------//
	THREAD_COMMAND_MODE        m_LogManagerThreadCmd;
	THREAD_STATE_MODE          m_LogManagerThreadState;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitLogManager();
	void                       InitialLogManager();	
	//---------------------------------------------------------------------------------//
	CLogManager(const CLogManager &Manager);
	CLogManager& operator=(const CLogManager &Manager);
	//---------------------------------------------------------------------------------//
	void                       LockLogManager();
	void                       UnlockLogManager();
	//---------------------------------------------------------------------------------//	
	bool                       CheckLogIndex(unsigned int idx);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CLogManager();
	virtual ~CLogManager();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;
	//---------------------------------------------------------------------------------//
	void                       SetThreadDwellTime(DWORD time) { m_ThreadDwellTime = time; }
	DWORD                      GetThreadDwellTime() const { return m_ThreadDwellTime; }//執行緒延遲時間
	//---------------------------------------------------------------------------------//
	bool                       CreateLogManagerThread();
	bool                       DeleteLogManagerThread();
	bool                       StartLogManagerThread(bool WaitOn);
	bool                       StopLogManagerThread(bool WaitOn);
	bool                       WaitForLogManagerThreadFinish();
	//---------------------------------------------------------------------------------//
	THREAD_COMMAND_MODE        GetLogManagerThreadCmd() const;
	void                       SetLogManagerThreadCmd(THREAD_COMMAND_MODE Cmd);
	//---------------------------------------------------------------------------------//
	THREAD_STATE_MODE          GetLogManagerThreadState() const;
	void                       SetLogManagerThreadState(THREAD_STATE_MODE State);			
	//---------------------------------------------------------------------------------//
	unsigned int               AddLogFile(LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName, LPCTSTR Backup, CLogNode::LOG_FILENAME_MODE Mode, bool bSaveHeader);
	unsigned int               AddLogFileOnRuning(LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName, LPCTSTR Backup, CLogNode::LOG_FILENAME_MODE Mode, bool bSaveHeader);
	bool                       ResetLogFilename(unsigned int idx, LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName);
	bool                       DeleteLogFile(unsigned int idx);
	bool                       RemoveLogNode(unsigned int idx);//移除訊息物件
	bool                       GetLogFilePathName(unsigned int idx, CString &Name);
	bool                       AddLogMessage(unsigned int idx, const char* Msg);	
	bool                       AddLogMessage(unsigned int idx, const wchar_t* Msg);	
	bool                       GetLogLastMessageTime(unsigned int idx, CString &Time);
	bool                       FlushLogMsgToFile();		
	bool                       FlushLogNodeMsgToFile(unsigned int idx);
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CLogManager             LogManager;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LOGMANAGER_H__832FD291_E602_462F_BBDE_C8D8F4F26599__INCLUDED_)
