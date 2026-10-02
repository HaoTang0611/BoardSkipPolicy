// ITSFileCheck.h: interface for the CITSFileCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITSFILECHECKER_H__AA1895B6_79C4_49BF_B1E1_2A94C375A9B4__INCLUDED_)
#define AFX_ITSFILECHECKER_H__AA1895B6_79C4_49BF_B1E1_2A94C375A9B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <queue>
//-------------------------------------------------------------------------------------//
enum MES_FILE_CHECKER_MODE
{
	MES_FILE_CHECKER_ITS = 1,
	MES_FILE_CHECKER_IPS = 3,
	MES_FILE_CHECKER_RETURN
};
//-------------------------------------------------------------------------------------//
class CITSLinker;
//-------------------------------------------------------------------------------------//
class CITSFileChecker  
{
private:
	//---------------------------------------------------------------------------------//
	CITSLinker                *m_ITSLinkerPtr;
	//---------------------------------------------------------------------------------//
	bool                       m_Connected;		
	bool                       m_FreezeComm;//凍結通訊
	bool                       m_UseSyncFile;//是否使用Sync檔案		
	bool                       m_TheSameFolder;//是否相同資料夾
	DWORD                      m_CreateSyncFileDelayTime;//使用Sync檔案的延遲時間
	CString                    m_FolderTemp;//暫存資料夾
	CString                    m_FolderSend;//傳送資料夾
	CString                    m_FolderRecv;//接收資料夾	
	CString                    m_FilenameSend;//傳送檔案名稱
	SYSTEMTIME                 m_FilenameSendTime;//傳送檔案名稱時間
	DWORD                      m_FilenameSendChkCnt;//傳送檔案名稱尾數
	MES_FILE_CHECKER_MODE      m_FileCheckMode;//檔案確認模式
	//---------------------------------------------------------------------------------//
	bool                       m_BackupFile;//備份檔案
	CString                    m_BackupFolderSend;//備份傳送資料夾
	CString                    m_BackupFolderRecv;//備份接收資料夾	
	//---------------------------------------------------------------------------------//
	std::string                m_SendName;
	std::vector<int>           m_ReceIdList;
	std::vector<std::string>   m_RecvMsgList;//收到的訊息列表	
	//---------------------------------------------------------------------------------//	
	HWND                       m_hWnd;
	UINT                       m_uMsg;
	WPARAM                     m_wRecv;
	WPARAM                     m_wSend;
	//---------------------------------------------------------------------------------//
	bool                       m_ThreadStop;
	bool                       m_ThreadExit;
	DWORD                      m_ThreadSleepTime;
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;
	//---------------------------------------------------------------------------------//
	unsigned int               m_thRecvID;  //接收訊息執行緒編號
	HANDLE                     m_thRecvHandle; //接收訊息執行緒處理碼
	//---------------------------------------------------------------------------------//
	CRITICAL_SECTION           m_csRecvThread;
	CRITICAL_SECTION           m_csRecvMsgList;	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CITSFileChecker(const CITSFileChecker &Other);
	CITSFileChecker& operator=(const CITSFileChecker &Other);
	//---------------------------------------------------------------------------------//		
	bool                       CloseThread_Recv();
	bool                       CreateThread_Recv();
	//---------------------------------------------------------------------------------//
	bool                       RemoveUnicodeHeader(std::string &str);//移除Unicode檔頭
	int                        CheckUnicodeFile(const std::string &str);//確認是否為Unicode檔案
	//---------------------------------------------------------------------------------//
	bool                       Send_ITS(const std::string &rData, LPCTSTR filename);
	bool                       Send_IPS(const std::string &rData, LPCTSTR filename);
	//---------------------------------------------------------------------------------//
	bool                       ReceiveExtName(CString &ExtName);
	bool                       ReceiveFilenameCheck_IPS(LPCTSTR Filename);
	bool                       Receive_ITS(std::queue<std::string> &rData);	
	bool                       Receive_IPS(std::queue<std::string> &rData);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CITSFileChecker();
	virtual ~CITSFileChecker();
	//---------------------------------------------------------------------------------//
	CITSLinker*                GetITSLinkerPtr();	
	void                       SetITSLinkerPtr(CITSLinker *Ptr);	
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;
	//---------------------------------------------------------------------------------//	
	bool                       CheckConnected();
	bool                       GetConnected() const;	
	//---------------------------------------------------------------------------------//
	bool                       GetFreezeComm() const;
	void                       SetFreezeComm(bool val);	
	//---------------------------------------------------------------------------------//
	bool                       CheckAddFilename() const;
	bool                       ExecAddFilename(std::string &str, CString filename);
	//---------------------------------------------------------------------------------//
	bool                       GetUseSyncFile() const;	
	void                       SetUseSyncFile(bool val);
	bool                       CheckUseSyncFile() const;
	bool                       ExecAddSyncFile(LPCTSTR Filename);
	//---------------------------------------------------------------------------------//
	DWORD                      GetCreateSyncFileDelayTime() const;	
	void                       SetCreateSyncFileDelayTime(DWORD val);
	//---------------------------------------------------------------------------------//
	MES_FILE_CHECKER_MODE      GetFileCheckMode() const;
	void                       SetFileCheckMode(MES_FILE_CHECKER_MODE val);		
	//---------------------------------------------------------------------------------//	
	bool                       GetTheSameFolder() const;
	void                       SetTheSameFolder(bool val);		
	//---------------------------------------------------------------------------------//		
	LPCTSTR                    GetFilenameSend();//傳送檔案名稱
	//---------------------------------------------------------------------------------//
	void                       SetHWnd(HWND h, UINT m, WPARAM wRecv, WPARAM wSend);
	//---------------------------------------------------------------------------------//	
	void                       LockRecvThread();
	void                       UnockRecvThread();
	//---------------------------------------------------------------------------------//
	void                       LockRecvMsgList();
	void                       UnlockRecvMsgList();
	//---------------------------------------------------------------------------------//	
	bool                       GetThreadStop() const;
	void                       SetThreadStop(bool val);	
	//---------------------------------------------------------------------------------//
	bool                       GetThreadExit() const;
	void                       SetThreadExit(bool val);	
	//---------------------------------------------------------------------------------//	
	DWORD                      GetThreadSleepTime() const;
	void                       SetThreadSleepTime(DWORD val);	
	//---------------------------------------------------------------------------------//
	void                       ShowRecv(const char *Str);
	void                       ShowSend(const char *Str);
	//---------------------------------------------------------------------------------//
	bool                       ExecThread_Recv();
	//---------------------------------------------------------------------------------//
	bool                       ConnectServer(LPCTSTR fdSend, LPCTSTR fdRecv, LPCTSTR fdTemp);
	void                       Disconnect();
	//---------------------------------------------------------------------------------//				
	bool                       Send(const std::string &rData, LPCTSTR filename);
	//---------------------------------------------------------------------------------//
	bool                       Receive(std::queue<std::string> &rData);	
	//---------------------------------------------------------------------------------//
	bool                       AddRecvMsg(const std::string &msg);	
	bool                       CloneRecvMsgList(std::vector<std::string> &sList, bool bClear);//取得接收訊息列表		
	//---------------------------------------------------------------------------------//
	void                       SetBackupFile(bool val); //設定是否備份檔案	
	bool                       GetBackupFile() const;	//取得是否備份檔案		
	LPCTSTR                    GetBackupFolderSend() const;//備份檔案資料夾-傳送
	void                       SetBackupFolderSend(LPCTSTR Folder);//備份檔案資料夾-傳送
	LPCTSTR                    GetBackupFolderRecv() const;//備份檔案資料夾-接收
	void                       SetBackupFolderRecv(LPCTSTR Folder);//備份檔案資料夾-接收
	bool                       ExecBackupFile(bool bSend, LPCTSTR filename);//執行備份
	//---------------------------------------------------------------------------------//		
	bool                       GetFilenameDateTime_ITS(LPCTSTR filename, CString &DateTime);
	bool                       GetFilenameDateTime_IPS(LPCTSTR filename, CString &RecvName, CString &DateTime, DWORD &ChkCnt);
	//---------------------------------------------------------------------------------//	
};

#endif // !defined(AFX_ITSFILECHECKER_H__AA1895B6_79C4_49BF_B1E1_2A94C375A9B4__INCLUDED_)
