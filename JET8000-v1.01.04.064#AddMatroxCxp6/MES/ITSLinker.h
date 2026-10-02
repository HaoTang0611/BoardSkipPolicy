// ITSLinker.h: interface for the CITSLinker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITSLINKER_H__3FE3AB23_E28F_4260_9936_542036844C04__INCLUDED_)
#define AFX_ITSLINKER_H__3FE3AB23_E28F_4260_9936_542036844C04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "JetSocket.h"
#include "ITSRabbitMQ.h"
#include "ITSFileChecker.h"
//-------------------------------------------------------------------------------------//
enum ITS_LINK_MODE
{
	ITS_LINK_SOCKET    = 1,
	ITS_LINK_RABBIT_MQ = 2,
	ITS_LINK_FILE      = 3,
	ITS_LINK_RETTURN
};
//-------------------------------------------------------------------------------------//
class CITSLinker  //ITS的連接類別
{
private:
	//---------------------------------------------------------------------------------//	
	CString                       m_ErrorString;
	ITS_LINK_MODE                 m_ITSLinkMode;//連到ITS軟體的方式
	//---------------------------------------------------------------------------------//	
	CJetSocketClient              m_ITSSocket;//連到ITS軟體的WinSocket	
	CITSRabbitMQ                  m_ITSRabbitMQ;//連到Rabbit Server的物件
	CITSFileChecker               m_ITSFileChecker;//連到ITS的檔案檢查器
	//---------------------------------------------------------------------------------//	
	CRITICAL_SECTION              m_csRecvSend;//同步化
	DWORD                         m_TickCountRecv;
	std::string                   m_NewBufferRecv;
	DWORD                         m_TickCountSend;
	std::string                   m_NewBufferSend;	
	//---------------------------------------------------------------------------------//	
protected:
	CITSLinker(const CITSLinker &Linker);
	//---------------------------------------------------------------------------------//	
	CITSLinker& operator=(const CITSLinker &Linker);
public:
	//---------------------------------------------------------------------------------//	
	CITSLinker();
	virtual ~CITSLinker();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();
	ITS_LINK_MODE              GetITSLinkMode() const;
	//---------------------------------------------------------------------------------//
	bool                       ConnectITS_Socket(const std::string& host, int port);
	bool                       ConnectITS_Rabbit(const std::string& host, int port, const std::string &rName, const std::string &rPwd, const std::string &rRecv, const std::string &rSend);
	bool                       ConnectITS_File(LPCTSTR fdSend, LPCTSTR fdRecv, LPCTSTR fdTemp, bool bBackup, bool bUseSync, LPCTSTR bkSend, LPCTSTR bkRecv);
	bool                       GetITSConnected();
	void                       DisconnectITS();
	//---------------------------------------------------------------------------------//	
	void                       SetFreezeComm(bool val);//凍結通訊
	void                       SetITSHWnd(HWND hWnd, UINT msg, WPARAM wRecv, WPARAM wSend);
	//---------------------------------------------------------------------------------//
	bool                       AddITSSendMsg(const std::string &msg, LPCTSTR filename=NULL);
	//---------------------------------------------------------------------------------//
	bool                       CloneITSRecvMsgList(std::vector<std::string> &sList, bool bClear);//取得接收訊息列表		
	//---------------------------------------------------------------------------------//
	bool                       GetITSThreadStop() const;
	void                       SetITSThreadStop(bool val);
	//---------------------------------------------------------------------------------//
	CJetSocketClient&          GetITSSocket();//連到ITS軟體的WinSocket	
	CITSRabbitMQ&              GetITSRabbitMQ();//連到Rabbit Server的物件
	CITSFileChecker&           GetITSFileChecker();//連到ITS的檔案檢查器
	//---------------------------------------------------------------------------------//	
	void                       LockBufferRecv();
	void                       UnlockBufferRecv();
	//---------------------------------------------------------------------------------//	
	void                       LockBufferSend();
	void                       UnlockBufferSend();
	//---------------------------------------------------------------------------------//	
	DWORD                      GetTickCountRecv() const;
	void                       SetTickCountRecv(DWORD val);	
	//---------------------------------------------------------------------------------//	
	const char*                GetNewBufferRecv() const;
	void                       SetNewBufferRecv(const char *str);	
	//---------------------------------------------------------------------------------//	
	DWORD                      GetTickCountSend() const;
	void                       SetTickCountSend(DWORD val);	
	//---------------------------------------------------------------------------------//	
	const char*                GetNewBufferSend() const;
	void                       SetNewBufferSend(const char *str);	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_ITSLINKER_H__3FE3AB23_E28F_4260_9936_542036844C04__INCLUDED_)
