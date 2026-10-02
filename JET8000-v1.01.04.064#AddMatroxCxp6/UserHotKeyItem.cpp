#include "stdafx.h"
#include "resource.h"
#include "UserHotKeyItem.h"
//-------------------------------------------------------------------------------------//
CUserHotKeyItem::CUserHotKeyItem(DWORD FuncID, LPCTSTR Text, UINT VK, UINT MK, UINT ExecID):m_FuncID(FuncID), m_FuncText(Text), m_VirtualKey(VK), m_ModifyKey(MK), m_ExecID(ExecID)
{
	m_hWnd = NULL;
	//m_FuncID = 0;
	//m_FuncText = _T("");
	//m_ExecID = 0;	
	//m_ModifyKey = 0;
	//m_VirtualKey = 0;
}
//-------------------------------------------------------------------------------------//
CUserHotKeyItem::~CUserHotKeyItem()
{
}
//-------------------------------------------------------------------------------------//
UINT CUserHotKeyItem::GetCmdID(DWORD FuncID) const
{
	UINT CmdID = 0;
	switch ( FuncID )
	{
	case USER_HOT_KEY_ROTATE_OBJ: CmdID = ID_HOTKEY_ROTATE_OBJ; break;				
	}
	return CmdID;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyItem::SaveUserHotKey(LPCTSTR pfilename, LPCTSTR pSection)
{
	bool bSucc = true;
	CString Err;
	CString Key;
	CString String;
	CString Section = GetFuncText();
	if ( NULL != pSection )
	{	Section = pSection;	}

	Key = _T("Func ID");
	String.Format(_T("%d"), GetFuncID());	
	if ( JetAPI::SaveINIData(Section, Key, String, pfilename, Err) == false )
	{	bSucc = false; }

	Key = _T("Func Text");
	String = GetFuncText();
	if ( JetAPI::SaveINIData(Section, Key, String, pfilename, Err) == false )
	{	bSucc = false; }

	Key = _T("Modify Key");
	String.Format(_T("%d"), GetModifyKey());	
	if ( JetAPI::SaveINIData(Section, Key, String, pfilename, Err) == false )
	{	bSucc = false; }

	Key = _T("Virtual Key");
	String.Format(_T("%d"), GetVirtualKey());	
	if ( JetAPI::SaveINIData(Section, Key, String, pfilename, Err) == false )
	{	bSucc = false; }

	Key = _T("Exec ID");
	String.Format(_T("%d"), GetExecID());	
	if ( JetAPI::SaveINIData(Section, Key, String, pfilename, Err) == false )
	{	bSucc = false; }
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyItem::LoadUserHotKey(LPCTSTR pfilename, LPCTSTR pSection)
{
	bool bSucc = true;
	CString Err;
	CString Key;		
	CString Default;
	CString Section = GetFuncText();
	const size_t textlen=256;
	TCHAR String[textlen];
	const bool IsCheckLens = false;
	if ( NULL != pSection )
	{	Section = pSection;	}	

	Key = _T("Func ID");
	Default.Format(_T("%d"), GetFuncID());	
	if ( JetAPI::LoadINIData(Section, Key, Default, String, textlen, pfilename, IsCheckLens, Err) == false )
	{	bSucc = false; }
	else
	{	SetFuncID(::_ttoi(String));	}

	Key = _T("Func Text");
	Default = GetFuncText();	
	if ( JetAPI::LoadINIData(Section, Key, Default, String, textlen, pfilename, IsCheckLens, Err) == false )
	{	bSucc = false; }
	else
	{	SetFuncText(String);	}

	Key = _T("Modify Key");
	Default.Format(_T("%d"), GetModifyKey());	
	if ( JetAPI::LoadINIData(Section, Key, Default, String, textlen, pfilename, IsCheckLens, Err) == false )
	{	bSucc = false; }
	else
	{	SetModifyKey(::_ttoi(String));	}

	Key = _T("Virtual Key");
	Default.Format(_T("%d"), GetVirtualKey());	
	if ( JetAPI::LoadINIData(Section, Key, Default, String, textlen, pfilename, IsCheckLens, Err) == false )
	{	bSucc = false; }
	else
	{	SetVirtualKey(::_ttoi(String));	}

	Key = _T("Exec ID");
	Default.Format(_T("%d"), GetExecID());	
	if ( JetAPI::LoadINIData(Section, Key, Default, String, textlen, pfilename, IsCheckLens, Err) == false )
	{	bSucc = false; }
	else
	{	SetExecID(::_ttoi(String));	}
	return bSucc;	
}
//-------------------------------------------------------------------------------------//
HWND CUserHotKeyItem::GetHWnd() const
{
	return m_hWnd;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyItem::SetHWnd(HWND val)
{
	m_hWnd = val;
}
//-------------------------------------------------------------------------------------//
DWORD CUserHotKeyItem::GetFuncID() const
{
	return m_FuncID;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyItem::SetFuncID(DWORD val)
{
	m_FuncID = val;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CUserHotKeyItem::GetFuncText() const
{
	return m_FuncText;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyItem::SetFuncText(LPCTSTR val)
{
	m_FuncText = val;
}
//-------------------------------------------------------------------------------------//
DWORD CUserHotKeyItem::GetExecID() const
{
	return m_ExecID;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyItem::SetExecID(DWORD val)
{
	m_ExecID = val;
}
//-------------------------------------------------------------------------------------//
UINT CUserHotKeyItem::GetModifyKey() const
{
	return m_ModifyKey;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyItem::SetModifyKey(UINT val)
{
	m_ModifyKey = val;
}
//-------------------------------------------------------------------------------------//
UINT CUserHotKeyItem::GetVirtualKey() const
{
	return m_VirtualKey;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyItem::SetVirtualKey(UINT val)
{
	m_VirtualKey = val;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyItem::CheckEnabled() const//確認有啟用熱鍵
{
	if ( 0 == GetVirtualKey() )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyItem::DisableUserHotyKey()//取消熱鍵
{
	SetVirtualKey(0);
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyItem::Unregister()
{
	if ( NULL == GetHWnd() ) { return true; }
	if ( UnregisterHotKey(GetHWnd(), GetFuncID()) == FALSE )
	{	return false; }
	SetHWnd(NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyItem::Register(HWND hWnd)
{
	if ( NULL == hWnd ) { return false; }
	if ( GetHWnd() != NULL )
	{
		if ( Unregister() == false )
		{	return false; }
	}
	if ( CheckEnabled() == false ) { return true; }
	//MOD_ALT, MOD_CONTROL, MOD_SHIFT, MOD_WIN, MOD_NOREPEAT
	if ( RegisterHotKey(hWnd, GetFuncID(), GetModifyKey()|MOD_NOREPEAT, GetVirtualKey()) == FALSE )
	{	return false; }
	SetHWnd(hWnd);
	return true;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyItem::GetHotKeyText(CString &Text)
{
	Text.Format(_T("ID:%d, MO:%d, VK:%d"), GetFuncID(), GetModifyKey(), GetVirtualKey());
	return;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyItem::Convert2Accel(ACCEL &rAccel) const
{
	const DWORD FuncID = GetFuncID();
	const UINT CmdID = GetCmdID(FuncID);
	if ( 0 == CmdID ) { return false; }

	BYTE fVirt = 0;
	const UINT ModifyKey = GetModifyKey();
	const UINT VirtualKey = GetVirtualKey();	

	fVirt = 0;
	const UINT nA = 'A';
	const UINT nZ = 'z';
	if ( 0 != (ModifyKey&MOD_ALT) ) { fVirt |= FALT; }
	if ( 0 != (ModifyKey&MOD_SHIFT) ) { fVirt |= FSHIFT; }
	if ( 0 != (ModifyKey&MOD_CONTROL) ) { fVirt |= FCONTROL; }
	if ( VirtualKey<nA || VirtualKey>nZ || 0!=fVirt )
	{	fVirt |= FVIRTKEY;	}
		
	rAccel.fVirt = fVirt;
	rAccel.cmd = static_cast<WORD>(CmdID);	
	rAccel.key = static_cast<WORD>(VirtualKey);	
	return true;
}
//-------------------------------------------------------------------------------------//