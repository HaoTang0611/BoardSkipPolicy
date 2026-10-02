#ifndef USER_HOT_KEY_CTRL_H_
#define USER_HOT_KEY_CTRL_H_
//-------------------------------------------------------------------------------------//
#pragma once
//-------------------------------------------------------------------------------------//
#include <vector>
#include "UserHotKeyItem.h"
//-------------------------------------------------------------------------------------//
class CUserHotKeyCtrl
{
private:
	//---------------------------------------------------------------------------------//
	CString                      m_ErrorString;
	std::vector<CUserHotKeyItem> m_UserHotKeyList;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CUserHotKeyCtrl();
	~CUserHotKeyCtrl();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();
	//---------------------------------------------------------------------------------//
	bool                       BuildUserHotKeyList();
	bool                       SaveUserHotKeyList(LPCTSTR pfilename=NULL);
	bool                       LoadUserHotKeyList(LPCTSTR pfilename=NULL);
	//---------------------------------------------------------------------------------//
	size_t                     GetUserHotKeyCount() const;
	void                       ClearUserHotKeyList();
	CUserHotKeyItem*           GetUserHotKeyPtr(size_t idx, bool bChk);	
	CUserHotKeyItem*           FindUserHotKeyPtrByFuncID(DWORD FuncID);	
	bool                       AddUserHotKey(const CUserHotKeyItem &HotKey);
	const std::vector<CUserHotKeyItem> &GetUserHotKeyList() const;
	void                       SetUserHotKeyList(const std::vector<CUserHotKeyItem> &List);
	void                       CloneUserHotKeyList(std::vector<CUserHotKeyItem> &List) const;	
	//---------------------------------------------------------------------------------//	
	bool                       UnregisterUserHotKey(DWORD FuncID);
	bool                       RegisterUserHotKey(CWnd *pWnd, DWORD FuncID);	

	bool                       UnregisterUserHotKeyList();
	bool                       RegisterUserHotKeyList(CWnd *pWnd);	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
extern CUserHotKeyCtrl UserHotKeyCtrl;
//-------------------------------------------------------------------------------------//
#endif//USER_HOT_KEY_CTRL_H_
//-------------------------------------------------------------------------------------//
