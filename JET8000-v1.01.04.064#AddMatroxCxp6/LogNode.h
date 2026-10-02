// LogNode.h: interface for the CLogNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGNODE_H__F57CA891_A261_4A1E_AE25_7CFB6EE47C3D__INCLUDED_)
#define AFX_LOGNODE_H__F57CA891_A261_4A1E_AE25_7CFB6EE47C3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
class CLogManager;
//-------------------------------------------------------------------------------------//
typedef struct _LOG_MSG_TXT
{
	CString     Msg;
	SYSTEMTIME  SysTime;
} TLOG_MSG_TXT, *PLOG_MSG_TXT;
//-------------------------------------------------------------------------------------//
class CLogNode  
{	
	friend CLogManager;	
public:
	//---------------------------------------------------------------------------------//
	enum LOG_FILENAME_MODE
	{
		LOG_FILENAME_FIX     = 1,
		LOG_FILENAME_BY_DATE = 2
	};
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitLogNode();
	void                       InitialLogNode();
	void                       CloneLogNode(const CLogNode &Log);
	//---------------------------------------------------------------------------------//
	bool                       m_LogSaveHeader;//儲存頭訊息
	LOG_FILENAME_MODE          m_FilenameMode;//訊息檔名模式
	CString                    m_LogFolder;//訊息檔案資料夾
	CString                    m_LogFilename;//訊息檔案名稱
	CString                    m_LogExtName;//訊息檔案副檔名
	CString                    m_LogBackup;//訊息檔案備註名稱
	CString                    m_LogPathName;//訊息檔案全名
	CString                    m_ErrorString;//訊息檔案錯誤敘述	
	SYSTEMTIME                 m_LogNameDatTime;//訊息檔案名稱時間
	std::vector<TLOG_MSG_TXT>  m_LogMsgListIn;//訊息列表-輸入
	std::vector<TLOG_MSG_TXT>  m_LogMsgListOut;//訊息列表-輸出
	//---------------------------------------------------------------------------------//
	void                       GetPathFilename(CString &filename);	
	void                       SetLogFile(LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName);//設定訊息檔案路徑與主檔名與副檔名
	void                       SetLogFile(LPCTSTR Folder, LPCTSTR Filename, LPCTSTR ExtName, LPCTSTR Backup, CLogNode::LOG_FILENAME_MODE Mode, bool bSaveHeader);//設定訊息檔案路徑與主檔名與副檔名
	//---------------------------------------------------------------------------------//
	void                       PushBackLogMsg();//
	bool                       CheckLogMsgOutEmpty() const;//確認訊息輸出完畢	
	bool                       WaitLogMsgEmpty(DWORD Timeout);//等待訊息輸出完畢		
	//---------------------------------------------------------------------------------//
	bool                       AddLogString(const char *Txt);//加入新一筆訊息
	bool                       AddLogString(const wchar_t *Txt);//加入新一筆訊息
	bool                       FlushLogFile();//將訊息寫入檔案
	bool                       DeleteLogFile();//刪除訊息檔案
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	
	CLogNode();
	virtual ~CLogNode();
	//---------------------------------------------------------------------------------//	
	CLogNode(const CLogNode &Log);	
	CLogNode& operator=(const CLogNode &Log);
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() const;
	SYSTEMTIME                 GetLogLastTime() const;
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetLogFolder() const;
	LPCTSTR                    GetLogFilename() const;
	LPCTSTR                    GetLogPathName() const;	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LOGNODE_H__F57CA891_A261_4A1E_AE25_7CFB6EE47C3D__INCLUDED_)
