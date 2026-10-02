#include "stdafx.h"
#include "UserHotKeyCtrl.h"
//-------------------------------------------------------------------------------------//
CUserHotKeyCtrl UserHotKeyCtrl;
//-------------------------------------------------------------------------------------//
CUserHotKeyCtrl::CUserHotKeyCtrl()
{
}
//-------------------------------------------------------------------------------------//
CUserHotKeyCtrl::~CUserHotKeyCtrl()
{
}
//-------------------------------------------------------------------------------------//
LPCTSTR CUserHotKeyCtrl::GetErrorString()
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyCtrl::BuildUserHotKeyList()
{	
	ClearUserHotKeyList();	
	AddUserHotKey(CUserHotKeyItem(LoadIDAndName(USER_HOT_KEY_ROTATE_OBJ), 'R', MOD_CONTROL, 180));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyCtrl::SaveUserHotKeyList(LPCTSTR pfilename)
{
	CString Key, String;
	CString Filename, Section;
	CString str, Err, ItemText;	
	const size_t Count = GetUserHotKeyCount();
	if ( NULL != pfilename )
	{	Filename = pfilename;	}
	else
	{	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("UserHotKey.INI"));	}

	Section = _T("HotKey Info");
	Key = _T("Count");
	String.Format(_T("%d"), Count);	
	if ( JetAPI::SaveINIData(Section, Key, String, Filename, Err) == false )
	{	
		m_ErrorString = Err;
		return false;	
	}

	for ( size_t i=0; i<Count; i++ )
	{
		CUserHotKeyItem *ItemPtr=GetUserHotKeyPtr(i, false);
		if ( NULL == ItemPtr ) { continue; }
		Section.Format(_T("%s_%04d"), _T("HotKey"), i+1);
		if ( ItemPtr->SaveUserHotKey(Filename, Section) == false )
		{
			ItemPtr->GetHotKeyText(ItemText);
			str.Format(_T("Error, SaveUserHotKey Fault[%s]"), ItemText);
			if ( Err.GetLength() == 0 )
			{	Err = str; }
			else
			{
				CString Temp=Err;
				Err.Format(_T("%s\n%s"), Temp, str);
			}
		}
	}
	m_ErrorString = Err;
	if ( Err.GetLength() > 0 )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyCtrl::LoadUserHotKeyList(LPCTSTR pfilename)
{
	const size_t textlen=256;
	TCHAR String[textlen];
	CString Key, Default;
	CString Filename, Section;
	CString str, Err, ItemText;	
	const bool IsCheckLens = false;		
	if ( NULL != pfilename )
	{	Filename = pfilename;	}
	else
	{	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("UserHotKey.INI"));	}

	Section = _T("HotKey Info");
	Key = _T("Count");
	Default.Format(_T("%d"), 0);	
	if ( JetAPI::LoadINIData(Section, Key, Default, String, textlen, Filename, IsCheckLens, Err) == false )
	{	
		m_ErrorString = Err;
		return false;	
	}
	CUserHotKeyItem HotKey;	
	const size_t Count = ::_ttoi(String);	
	if ( 0 == Count )
	{	return true; }

	ClearUserHotKeyList();	
	for ( size_t i=0; i<Count; i++ )
	{
		Section.Format(_T("%s_%04d"), _T("HotKey"), i+1);
		if ( HotKey.LoadUserHotKey(Filename, Section) == false )				
		{
			HotKey.GetHotKeyText(ItemText);
			str.Format(_T("Error, LoadUserHotKey Fault[%s]"), ItemText);
			if ( Err.GetLength() == 0 )
			{	Err = str; }
			else
			{
				CString Temp=Err;
				Err.Format(_T("%s\n%s"), Temp, str);
			}
			continue;
		}
		AddUserHotKey(HotKey);
	}
	m_ErrorString = Err;
	if ( Err.GetLength() > 0 )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CUserHotKeyCtrl::GetUserHotKeyCount() const
{
	return m_UserHotKeyList.size();
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyCtrl::ClearUserHotKeyList()
{
	m_UserHotKeyList.clear();
}
//-------------------------------------------------------------------------------------//
CUserHotKeyItem* CUserHotKeyCtrl::GetUserHotKeyPtr(size_t idx, bool bChk)
{
	if ( bChk )
	{
		const size_t Count=m_UserHotKeyList.size();
		if ( idx >= Count )
		{	return NULL; }
	}
	return &(m_UserHotKeyList[idx]);
}
//-------------------------------------------------------------------------------------//
CUserHotKeyItem* CUserHotKeyCtrl::FindUserHotKeyPtrByFuncID(DWORD FuncID)
{
	const size_t Count = GetUserHotKeyCount();
	for ( size_t i=0; i<Count; i++ )
	{
		CUserHotKeyItem *ItemPtr=GetUserHotKeyPtr(i, false);
		if ( NULL == ItemPtr ) { continue; }
		if ( ItemPtr->GetFuncID() != FuncID ) { continue; }
		return ItemPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyCtrl::AddUserHotKey(const CUserHotKeyItem &HotKey)
{
	m_UserHotKeyList.push_back(HotKey);
	return true;
}
//-------------------------------------------------------------------------------------//
const std::vector<CUserHotKeyItem> &CUserHotKeyCtrl::GetUserHotKeyList() const
{
	return m_UserHotKeyList;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyCtrl::SetUserHotKeyList(const std::vector<CUserHotKeyItem> &List)
{
	m_UserHotKeyList = List;
}
//-------------------------------------------------------------------------------------//
void CUserHotKeyCtrl::CloneUserHotKeyList(std::vector<CUserHotKeyItem> &List) const
{
	List = m_UserHotKeyList;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyCtrl::UnregisterUserHotKey(DWORD FuncID)
{
	CString str, Err, ItemText;	
	const size_t Count = GetUserHotKeyCount();
	for ( size_t i=0; i<Count; i++ )
	{
		CUserHotKeyItem *ItemPtr=GetUserHotKeyPtr(i, false);
		if ( NULL == ItemPtr ) { continue; }
		if ( ItemPtr->GetFuncID() != FuncID ) { continue; }
		if ( ItemPtr->Unregister() == false )
		{			
			ItemPtr->GetHotKeyText(ItemText);
			Err.Format(_T("Error, UnegisterHotKey Fault[%s]"), ItemText);
		}
		break;
	}
	m_ErrorString = Err;
	if ( Err.GetLength() > 0 )
	{	return false;	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyCtrl::RegisterUserHotKey(CWnd *pWnd, DWORD FuncID)
{
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() ) { return false; }

	CString str, Err, ItemText;
	HWND hWnd = pWnd->GetSafeHwnd();	
	const size_t Count = GetUserHotKeyCount();
	for ( size_t i=0; i<Count; i++ )
	{
		CUserHotKeyItem *ItemPtr=GetUserHotKeyPtr(i, false);
		if ( NULL == ItemPtr ) { continue; }
		if ( ItemPtr->GetFuncID() != FuncID ) { continue; }
		if ( ItemPtr->Register(hWnd) == false )
		{
			ItemPtr->GetHotKeyText(ItemText);
			Err.Format(_T("Error, RegisterHotKey Fault[%s]"), ItemText);			
		}
		break;
	}
	m_ErrorString = Err;
	if ( Err.GetLength() > 0 )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyCtrl::UnregisterUserHotKeyList()
{
	CString str, Err, ItemText;	
	const size_t Count = GetUserHotKeyCount();
	for ( size_t i=0; i<Count; i++ )
	{
		CUserHotKeyItem *ItemPtr=GetUserHotKeyPtr(i, false);
		if ( NULL == ItemPtr ) { continue; }
		if ( ItemPtr->Unregister() == false )
		{
			ItemPtr->GetHotKeyText(ItemText);
			str.Format(_T("Error, UnegisterHotKey Fault[%s]"), ItemText);
			if ( Err.GetLength() == 0 )
			{	Err = str; }
			else
			{
				CString Temp=Err;
				Err.Format(_T("%s\n%s"), Temp, str);
			}
		}
	}
	m_ErrorString = Err;
	if ( Err.GetLength() > 0 )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CUserHotKeyCtrl::RegisterUserHotKeyList(CWnd *pWnd)
{
	if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() ) { return false; }

	CString str, Err, ItemText;
	HWND hWnd = pWnd->GetSafeHwnd();	
	const size_t Count = GetUserHotKeyCount();
	for ( size_t i=0; i<Count; i++ )
	{
		CUserHotKeyItem *ItemPtr=GetUserHotKeyPtr(i, false);
		if ( NULL == ItemPtr ) { continue; }
		if ( ItemPtr->Register(hWnd) == false )
		{
			ItemPtr->GetHotKeyText(ItemText);
			str.Format(_T("Error, RegisterHotKey Fault[%s]"), ItemText);
			if ( Err.GetLength() == 0 )
			{	Err = str; }
			else
			{
				CString Temp=Err;
				Err.Format(_T("%s\n%s"), Temp, str);
			}
		}
	}
	m_ErrorString = Err;
	if ( Err.GetLength() > 0 )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
