#include "StdAfx.h"
#include "LogOperCtrl.h"
//-------------------------------------------------------------------------------------//
CLogOperCtrl LogOperCtrl;
//-------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------//
CLogOperCtrl::CLogOperCtrl()
{
}
//-------------------------------------------------------------------------------------//
CLogOperCtrl::~CLogOperCtrl()
{
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckStrValid(LPCTSTR str)
{
	if ( NULL == str ) { return false; }
	if ( 0 == ::_tcslen(str) ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckFdPtr(const CAOIFd *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckBoxPtr(const CAOIBox *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckWndPtr(const CAOIWnd *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckLandPtr(const CAOILand *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CheckMarkPtr(const CAOIMark *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckRGBVPtr(const CColorRGBV *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckModelPtr(const CAOIModel *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckPanelPtr(const CAOIPanel *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckBoardPtr(const CAOIBoard *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckBarcodePtr(const CAOIBarcode *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckProjectPtr(const CAOIProject *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::CheckComponentPtr(const CAOIComponent *Ptr)
{
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::FormatContent(LPCTSTR sOper, LPCTSTR sKey, int nVal, CString &str)
{
	if ( CheckStrValid(sKey) == false )
	{	str.Format(_T("%s %d"), sOper, nVal); }
	else
	{	str.Format(_T("%s [%s] %d"), sOper, sKey, nVal); }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::FormatContent(LPCTSTR sOper, LPCTSTR sKey, double fVal, CString &str)
{
	if ( CheckStrValid(sKey) == false )
	{	str.Format(_T("%s %.2f"), sOper, fVal); }
	else
	{	str.Format(_T("%s [%s] %.2f"), sOper, sKey, fVal); }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::FormatContent(LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal, CString &str)
{
	if ( CheckStrValid(sKey) == false )
	{	str.Format(_T("%s %s"), sOper, sVal); }
	else
	{	str.Format(_T("%s [%s] %s"), sOper, sKey, sVal); }
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::FormatContent(LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew, CString &str)
{
	CString sTo = AOIDataDefine.GetToText();
	CString sFrom = AOIDataDefine.GetFromText();
	if ( CheckStrValid(sKey) == false )
	{	str.Format(_T("%s %s [%s] %s [%s]"), sOper, sFrom, sOld, sTo, sNew);	}
	else
	{	str.Format(_T("%s [%s] %s [%s] %s [%s]"), sOper, sKey, sFrom, sOld, sTo, sNew);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CLogOperCtrl::FormatContent(LPCTSTR sOper, LPCTSTR sAlg, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew, CString &str)
{
	CString sTo = AOIDataDefine.GetToText();
	CString sFrom = AOIDataDefine.GetFromText();
	if ( CheckStrValid(sAlg) == false )
	{
		if ( CheckStrValid(sKey) == false )
		{	str.Format(_T("%s %s [%s] %s [%s]"), sOper, sFrom, sOld, sTo, sNew);	}
		else
		{	str.Format(_T("%s [%s] %s [%s] %s [%s]"), sOper, sKey, sFrom, sOld, sTo, sNew);	}	
	}
	else
	{
		if ( CheckStrValid(sKey) == false )
		{	str.Format(_T("%s [%s] %s [%s] %s [%s]"), sOper, sAlg, sFrom, sOld, sTo, sNew);	}
		else
		{	str.Format(_T("%s [%s]-[%s] %s [%s] %s [%s]"), sOper, sAlg, sKey, sFrom, sOld, sTo, sNew);	}	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CloseLogOperFile(FILE *&pfile)
{
	if ( NULL == pfile ) { return true; }
	::fclose(pfile);
	pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
FILE* CLogOperCtrl::OpenLogOperFile(LPCTSTR filename)
{
	FILE *pfile = ::_tfopen(filename, _T("r"));
	if ( NULL == pfile ) { return NULL; }	
	//Shift for Utf8
#ifndef _X64
	::fseek(pfile, 3, SEEK_SET);	
#else
	::_fseeki64(pfile, 3, SEEK_SET);	
#endif//_X64
	::memset(m_ReadBufferA, 0x00, sizeof(m_ReadBufferA));
	::memset(m_ReadBufferW, 0x00, sizeof(m_ReadBufferW));
	return pfile;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CheckLogOperFileEnd(FILE *pfile)
{
	if ( NULL == pfile ) { return true; }
	int Res = ::feof(pfile);
	if ( 0 == Res ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::ReadLogOperText(FILE *pfile, CString &Str)
{
	const int Len = 2048;
	if ( ReadLogOperText(pfile, m_ReadBufferW, Len) == false )
	{	return false; }
	Str = m_ReadBufferW;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::ReadLogOperText(FILE *pfile, LPWSTR Buffer, int szBuffer)
{
	if ( NULL == pfile ) { return false; }
	char  *BufferPtr=m_ReadBufferA;
	const int Len = sizeof(m_ReadBufferA);	
	if ( ::fgets(BufferPtr, Len, pfile) == NULL ) 
	{	return false; }
	if ( JetAPI::char2wchar(BufferPtr, Buffer, szBuffer, CP_UTF8) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
int CLogOperCtrl::CheckLogOperContentCount(LPCTSTR Content)
{
	int Count = 0;
#ifndef _UNICODE
	Count = CheckLogOperContentCountA(Content);
#else
	Count = CheckLogOperContentCountW(Content);
#endif//_UNICODE
	return Count;
}
//-------------------------------------------------------------------------------------//
int CLogOperCtrl::CheckLogOperContentCountA(LPCSTR Content)
{
	if ( NULL == Content ) { return 0; }
	int   i=0;
	int   Cnt=0;
	const char ch = '@';	
	const int Len = (int)(::strlen(Content));
	for ( i=0; i<Len; i++ )
	{
		if ( ch==Content[i] )
		{	Cnt ++;	}
	}
	Cnt ++;
	return Cnt;
}
//-------------------------------------------------------------------------------------//
int CLogOperCtrl::CheckLogOperContentCountW(LPCWSTR Content)
{
	if ( NULL == Content ) { return 0; }
	int   i=0;
	int   Cnt=0;
	const wchar_t ch = L'@';	
	const int Len = (int)(::wcslen(Content));
	for ( i=0; i<Len; i++ )
	{
		if ( ch==Content[i] )
		{	Cnt ++;	}
	}
	Cnt ++;	
	return Cnt;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::DecodeLogOperContent(LPCTSTR Content, std::vector<CString> &List)
{
	bool IsOK = true;
#ifndef _UNICODE
	IsOK = DecodeLogOperContentA(Content, List);
#else
	IsOK = DecodeLogOperContentW(Content, List);
#endif//_UNICODE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::DecodeLogOperContentA(LPCSTR Content, std::vector<CString> &List)
{
	if ( NULL == Content ) { return false; }
	int   i=0, j=0;
	const int szBuf=1024;
	char  Buffer[szBuf] = "";	
	const char ch = '@';
	std::vector<int> IdxList;
	const int Len = (int)(::strlen(Content));
	for ( i=0; i<Len; i++ )
	{
		if ( ch==Content[i] )
		{	IdxList.push_back(i);	}
	}
	IdxList.push_back(i);
	const int IdxCnt=(int)(IdxList.size());
	j = 0;
	for ( i=0; i<IdxCnt; i++ )
	{
		int nLen=IdxList[i]-j;
		::memcpy(Buffer, &(Content[j]), sizeof(char)*(nLen));
		Buffer[nLen] = '\0';
		j = IdxList[i]+1;
		List.push_back(CString(Buffer));
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::DecodeLogOperContentW(LPCWSTR Content, std::vector<CString> &List)
{
	if ( NULL == Content ) { return false; }
	int   i=0, j=0;
	const int szBuf=1024;
	wchar_t   Buffer[szBuf] = L"";	
	const wchar_t ch = L'@';
	std::vector<int> IdxList;	
	const int Len = (int)(::wcslen(Content));
	for ( i=0; i<Len; i++ )
	{
		if ( ch==Content[i] )
		{	IdxList.push_back(i);	}
	}
	IdxList.push_back(i);
	const int IdxCnt=(int)(IdxList.size());
	j = 0;
	for ( i=0; i<IdxCnt; i++ )
	{
		int nLen=IdxList[i]-j;
		::memcpy(Buffer, &(Content[j]), sizeof(wchar_t)*(nLen));
		Buffer[nLen] = L'\0';
		j = IdxList[i]+1;
		List.push_back(CString(Buffer));
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::AddLogOperModified_Key(LPCTSTR Key, TLogOperModify &Obj, std::vector<TLogOperModify> &List)
{
	Obj.SetKey(Key);
	List.push_back(Obj);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CmpAndAddLogOperModif_OBJ(TLogOperModify &Obj, std::vector<TLogOperModify> &List)
{
	if ( Obj.m_sOld == Obj.m_sNew ) { return true; }
	List.push_back(Obj);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CmpAndAddLogOperModif_BOL(LPCTSTR Key, int Old, int New, std::vector<TLogOperModify> &List, DWORD_PTR dwData)
{
	if ( Old == New ) { return true; }
	TLogOperModify Node;
	Node.SetBol(Key, Old, New,  dwData);
	List.push_back(Node);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CmpAndAddLogOperModif_INT(LPCTSTR Key, int Old, int New, std::vector<TLogOperModify> &List, DWORD_PTR dwData)
{
	if ( Old == New ) { return true; }
	TLogOperModify Node;
	Node.SetInt(Key, Old, New, dwData);
	List.push_back(Node);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CmpAndAddLogOperModif_CLR(LPCTSTR Key, int Old, int New, std::vector<TLogOperModify> &List, DWORD_PTR dwData)
{
	if ( Old == New ) { return true; }
	CString sOld;
	CString sNew;	
	TLogOperModify Node;	
	sOld.Format(_T("(%d, %d, %d)"), GetRValue(Old), GetGValue(Old), GetBValue(Old));
	sNew.Format(_T("(%d, %d, %d)"), GetRValue(New), GetGValue(New), GetBValue(New));
	Node.SetStr(Key, sOld, sNew, dwData);
	List.push_back(Node);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CmpAndAddLogOperModif_DBL(LPCTSTR Key, double Old, double New, std::vector<TLogOperModify> &List, DWORD_PTR dwData)
{
	if ( Old == New ) { return true; }
	TLogOperModify Node;
	Node.SetDbl(Key, Old, New, dwData);
	List.push_back(Node);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CmpAndAddLogOperModif_STR(LPCTSTR Key, LPCTSTR Old, LPCTSTR New, std::vector<TLogOperModify> &List, DWORD_PTR dwData)
{
	if ( Old == New ) { return true; }
	if ( _tcscmp(Old, New) == 0 ) { return true; }
	TLogOperModify Node;
	CString sOld(Old);
	CString sNew(New);	
	Node.SetStr(Key, sOld, sNew, dwData);
	List.push_back(Node);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CmpAndAddLogOperModif_STR(LPCTSTR Key, const std::string &Old, const std::string &New, std::vector<TLogOperModify> &List, DWORD_PTR dwData)
{
	if ( Old == New ) { return true; }
	TLogOperModify Node;
	CString sOld(Old.c_str());
	CString sNew(New.c_str());
	Node.SetStr(Key, sOld, sNew, dwData);
	List.push_back(Node);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::CmpAndAddLogOperModif_STR(LPCTSTR Key, const std::wstring &Old, const std::wstring &New, std::vector<TLogOperModify> &List, DWORD_PTR dwData)
{
	if ( Old == New ) { return true; }
	TLogOperModify Node;
	CString sOld(Old.c_str());
	CString sNew(New.c_str());
	Node.SetStr(Key, sOld, sNew, dwData);
	List.push_back(Node);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogOper_SystemContent(LPCTSTR Content)//儲存操作訊息-系統函式
{
	CString AppName = ::AfxGetAppName();
	return AOIDataCollect.SaveLogOper_SystemFunc(AppName, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogUserLogDelAdd(LPCTSTR Oper, LPCTSTR Val)
{
	CString Operation, Content;
	if (NULL == Val) { return false; }
	Operation.Format(_T("%s User"), Oper);
	Content.Format(_T("%s %s"), Operation, Val);
	return AOIDataCollect.SaveLogOper_UserFunc(Operation, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogUserLogInOut(LPCTSTR Oper, LPCTSTR Val)//儲存操作訊息使用者
{
	CString Content;
	if ( NULL == Val ) 
	{	Content = Oper; }
	else
	{	Content.Format(_T("%s %s"), Oper, Val); }
	return AOIDataCollect.SaveLogOper_UserFunc(Oper, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogUserLogIn(const TUserNode &User)//儲存操作訊息使用者-登入
{	
	CString UserName(User.wUserName);	
	return SaveLogUserLogInOut(_T("Login"), UserName);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogUserLogOut(const TUserNode &User)//儲存操作訊息使用者-登出
{
	CString UserName(User.wUserName);	
	return SaveLogUserLogInOut(_T("Logout"), UserName);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogSystemComparParam(const TSystemParameter &OldParam, const TSystemParameter &NewParam)//儲存操作訊息-系統參數修改
{
	CString strOld, strNew;
	std::vector<TLogOperModify>  List;
	CString strTag=_T("System Param");
	//CmpAndAddLogOperModif_STR(_T("AAAAAAAAAAAAAAAAAAAA"), NewParam.STRSTRSTRSTRSTRSTR, OldParam.STRSTRSTRSTRSTRSTR, List);	
	//CmpAndAddLogOperModif_INT(_T("AAAAAAAAAAAAAAAAAAAA"), NewParam.INTINTINTINTINTINT, OldParam.INTINTINTINTINTINT, List);	
	//CmpAndAddLogOperModif_DBL(_T("AAAAAAAAAAAAAAAAAAAA"), NewParam.DBLDBLDBLDBLDBLDBL, OldParam.DBLDBLDBLDBLDBLDBL, List);
	//CmpAndAddLogOperModif_CLR(_T("AAAAAAAAAAAAAAAAAAAA"), NewParam.CLRCLRCLRCLRCLRCLR, OldParam.CLRCLRCLRCLRCLRCLR, List);
	
	CmpAndAddLogOperModif_STR(_T("AOI Directory"), OldParam.m_AOIDirectory, NewParam.m_AOIDirectory, List);	
	CmpAndAddLogOperModif_STR(_T("AOI Base Temp Directory"), OldParam.m_AOIBaseTempDirectory, NewParam.m_AOIBaseTempDirectory, List);	
	CmpAndAddLogOperModif_STR(_T("AOI Log Directory"), OldParam.m_AOILogDirectory, NewParam.m_AOILogDirectory, List);	
	CmpAndAddLogOperModif_STR(_T("AOI Project Folder"), OldParam.m_AOIProjectFolder, NewParam.m_AOIProjectFolder, List);	
	CmpAndAddLogOperModif_STR(_T("AOI Result Folder"), OldParam.m_AOIResultFolder, NewParam.m_AOIResultFolder, List);	
	CmpAndAddLogOperModif_STR(_T("AOI Server Folder"), OldParam.m_AOIServerFolder, NewParam.m_AOIServerFolder, List);	
	CmpAndAddLogOperModif_STR(_T("Online Tuning Folder"), OldParam.m_OnlineTuningFolder, NewParam.m_OnlineTuningFolder, List);	
	CmpAndAddLogOperModif_STR(_T("Online Barcode Folder"), OldParam.m_OnlineBarcodeFolder, NewParam.m_OnlineBarcodeFolder, List);	
	CmpAndAddLogOperModif_STR(_T("Online Offline Folder"), OldParam.m_OnlineOfflineFolder, NewParam.m_OnlineOfflineFolder, List);	
	CmpAndAddLogOperModif_STR(_T("Project Test Map Folder"), OldParam.m_ProjectTestMapFolder, NewParam.m_ProjectTestMapFolder, List);
	CmpAndAddLogOperModif_STR(_T("Project Test Map Folder Temp"), OldParam.m_ProjectTestMapFolderTemp, NewParam.m_ProjectTestMapFolderTemp, List);
	CmpAndAddLogOperModif_STR(_T("Project Test Map Folder Backup"), OldParam.m_ProjectTestMapFolderBackup, NewParam.m_ProjectTestMapFolderBackup, List);
	CmpAndAddLogOperModif_STR(_T("Project Raw Folder"), OldParam.m_ProjectRawFolder, NewParam.m_ProjectRawFolder, List);
	CmpAndAddLogOperModif_STR(_T("Project Debug Folder"), OldParam.m_ProjectDebugFolder, NewParam.m_ProjectDebugFolder, List);		
	CmpAndAddLogOperModif_STR(_T("Monitor Status Folder"), OldParam.m_MonitorStatusFolder, NewParam.m_MonitorStatusFolder, List);	
	CmpAndAddLogOperModif_STR(_T("Customer Log Folder"), OldParam.m_CustomerLogFolder, NewParam.m_CustomerLogFolder, List);	
	CmpAndAddLogOperModif_STR(_T("Customer Report Folder"), OldParam.m_CustomerReportFolder, NewParam.m_CustomerReportFolder, List);	
	CmpAndAddLogOperModif_STR(_T("Barcode File Folder LA"), OldParam.m_BarcodeFileFolder_LA, NewParam.m_BarcodeFileFolder_LA, List);	
	CmpAndAddLogOperModif_STR(_T("Barcode File Folder LB"), OldParam.m_BarcodeFileFolder_LB, NewParam.m_BarcodeFileFolder_LB, List);	
	CmpAndAddLogOperModif_STR(_T("AI Export File Folder"), OldParam.m_AIFileExportFolder, NewParam.m_AIFileExportFolder, List);	
	CmpAndAddLogOperModif_STR(_T("AI Export Image Folder"), OldParam.m_AIImageExportFolder, NewParam.m_AIImageExportFolder, List);	
	CmpAndAddLogOperModif_STR(_T("AOI Test Track Folder"), OldParam.m_AOITestTrackFolder, NewParam.m_AOITestTrackFolder, List);		
	CmpAndAddLogOperModif_STR(_T("Library Host"), OldParam.m_LibraryHost, NewParam.m_LibraryHost, List);	
	//CmpAndAddLogOperModif_STR(_T("Library Host Bottom"), OldParam.m_LibraryHostBot, NewParam.m_LibraryHostBot, List);	
	CmpAndAddLogOperModif_STR(_T("Library Remote"), OldParam.m_LibraryRemote, NewParam.m_LibraryRemote, List);

	CmpAndAddLogOperModif_INT(_T("Save Moving Time Message"), OldParam.m_SaveMovingTimeMessage, NewParam.m_SaveMovingTimeMessage, List);	
	CmpAndAddLogOperModif_INT(_T("Backup Moving Time Message"), OldParam.m_BackupMovingTimeMessage, NewParam.m_BackupMovingTimeMessage, List);		
	CmpAndAddLogOperModif_INT(_T("Save Current Process Message"), OldParam.m_SaveCurrentProcessMessage, NewParam.m_SaveCurrentProcessMessage, List);	
	CmpAndAddLogOperModif_INT(_T("Backup Current Process Message"), OldParam.m_BackupCurrentProcessMessage, NewParam.m_BackupCurrentProcessMessage, List);		
	CmpAndAddLogOperModif_INT(_T("Save Machine Monitor Message"), OldParam.m_SaveMachineMonitorMessage, NewParam.m_SaveMachineMonitorMessage, List);	
	CmpAndAddLogOperModif_INT(_T("Save Machine Monitor Message Web"), OldParam.m_SaveMachineMonitorMessageWeb, NewParam.m_SaveMachineMonitorMessageWeb, List);	
	CmpAndAddLogOperModif_INT(_T("Save Camera Add Ring Buffer Log"), OldParam.m_SaveCameraAddRingBufferLog, NewParam.m_SaveCameraAddRingBufferLog, List);	
	CmpAndAddLogOperModif_INT(_T("Show Memory Leak Message"), OldParam.m_ShowMemoryLeakMessage, NewParam.m_ShowMemoryLeakMessage, List);	
	//CmpAndAddLogOperModif_INT(_T("Show AOI Obj Exception Message"), OldParam.m_ShowAOIObjExceptionMessage, NewParam.m_ShowAOIObjExceptionMessage, List);	
	CmpAndAddLogOperModif_INT(_T("Send Debug View String"), OldParam.m_SendDebugViewString, NewParam.m_SendDebugViewString, List);	
	CmpAndAddLogOperModif_INT(_T("Save Slice Fill Thread Log"), OldParam.m_SaveSliceFillThreadLog, NewParam.m_SaveSliceFillThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Frame Merge Thread Log"), OldParam.m_SaveFrameMergeThreadLog, NewParam.m_SaveFrameMergeThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Field Merge Thread Log"), OldParam.m_SaveFieldMergeThreadLog, NewParam.m_SaveFieldMergeThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Frame Load Thread Log"), OldParam.m_SaveFrameLoadThreadLog, NewParam.m_SaveFrameLoadThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Region Calc Thread Log"), OldParam.m_SaveRegionCalcThreadLog, NewParam.m_SaveRegionCalcThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Sequence Thread Log"), OldParam.m_SaveSequenceThreadLog, NewParam.m_SaveSequenceThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Field Frame Release Thread Log"), OldParam.m_SaveFieldFrameReleaseThreadLog, NewParam.m_SaveFieldFrameReleaseThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Online Inspection Thread Log"), OldParam.m_SaveOnlineInspectionThreadLog, NewParam.m_SaveOnlineInspectionThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Remove Folder Thread Log"), OldParam.m_SaveRemoveFolderThreadLog, NewParam.m_SaveRemoveFolderThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Conveyer Auto-Run Thread Log"), OldParam.m_SaveConveyerPreRunThreadLog, NewParam.m_SaveConveyerPreRunThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save ITS-Proc Thread Log"), OldParam.m_SaveITSProcThreadLog, NewParam.m_SaveITSProcThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Load Repair File Thread Log"), OldParam.m_SaveLoadRepairFileThreadLog, NewParam.m_SaveLoadRepairFileThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Repair Result Signal Thread Log"), OldParam.m_SaveRepairResultSignalThreadLog, NewParam.m_SaveRepairResultSignalThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Simple Job Thread Log"), OldParam.m_SaveSimpleJobThreadLog, NewParam.m_SaveSimpleJobThreadLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save Test Object Finish Log"), OldParam.m_SaveTestObjectFinishLog, NewParam.m_SaveTestObjectFinishLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save UI Draw Func Log"), OldParam.m_SaveUIDrawFuncLog, NewParam.m_SaveUIDrawFuncLog, List);	
	CmpAndAddLogOperModif_INT(_T("Save User Operation Log"), OldParam.m_SaveUserOperationLog, NewParam.m_SaveUserOperationLog, List);		
	CmpAndAddLogOperModif_INT(_T("Save Test Track File"), OldParam.m_SaveTestTrackFile, NewParam.m_SaveTestTrackFile, List);

	CmpAndAddLogOperModif_INT(_T("Save Project Report Text"), OldParam.m_SaveProjectReportText, NewParam.m_SaveProjectReportText, List);	
	CmpAndAddLogOperModif_INT(_T("Save Project Report Text Filename Mode"), OldParam.m_SaveProjectReportTextFilenameMode, NewParam.m_SaveProjectReportTextFilenameMode, List);
	CmpAndAddLogOperModif_INT(_T("Save Customer Report File"), OldParam.m_SaveCustomerReportFile, NewParam.m_SaveCustomerReportFile, List);	
	CmpAndAddLogOperModif_INT(_T("Save Project Report Wnd Reading"), OldParam.m_SaveProjectReportWndReading, NewParam.m_SaveProjectReportWndReading, List);	
	CmpAndAddLogOperModif_INT(_T("Save Project Spc File Mode"), OldParam.m_SaveProjectSpcFileMode, NewParam.m_SaveProjectSpcFileMode, List);	
	CmpAndAddLogOperModif_INT(_T("Save Project Spc Library Mode"), OldParam.m_SaveProjectSpcLibraryMode, NewParam.m_SaveProjectSpcLibraryMode, List);

	CmpAndAddLogOperModif_INT(_T("Pre-Load Project Offline Image"), OldParam.m_PreLoadProjectOfflineImage, NewParam.m_PreLoadProjectOfflineImage, List);	
	CmpAndAddLogOperModif_INT(_T("Auto Release Field Frame Buffer"), OldParam.m_AutoReleaseFieldFrameBuffer, NewParam.m_AutoReleaseFieldFrameBuffer, List);	
	CmpAndAddLogOperModif_INT(_T("Auto Release Offline Field Frame Buffer"), OldParam.m_AutoReleaseOfflineFieldFrameBuffer, NewParam.m_AutoReleaseOfflineFieldFrameBuffer, List);	
	
	CmpAndAddLogOperModif_STR(_T("App Version"), OldParam.m_AppVersion, NewParam.m_AppVersion, List);	
	CmpAndAddLogOperModif_STR(_T("IP Host Computer"), OldParam.m_IPHostComputer, NewParam.m_IPHostComputer, List);	

	CmpAndAddLogOperModif_STR(_T("Machine Location"), OldParam.m_MachineLocation, NewParam.m_MachineLocation, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Building"), OldParam.m_MachineBuilding, NewParam.m_MachineBuilding, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Floor"), OldParam.m_MachineFloor, NewParam.m_MachineFloor, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Room"), OldParam.m_MachineRoom, NewParam.m_MachineRoom, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Line"), OldParam.m_MachineLine, NewParam.m_MachineLine, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Line LB"), OldParam.m_MachineLine_LB, NewParam.m_MachineLine_LB, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Station"), OldParam.m_MachineStation, NewParam.m_MachineStation, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Station LB"), OldParam.m_MachineStation_LB, NewParam.m_MachineStation_LB, List);	
	CmpAndAddLogOperModif_STR(_T("Machine SN"), OldParam.m_MachineSN, NewParam.m_MachineSN, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Name"), OldParam.m_MachineName, NewParam.m_MachineName, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Vendor"), OldParam.m_MachineVendor, NewParam.m_MachineVendor, List);	
	CmpAndAddLogOperModif_STR(_T("Machine Alias"), OldParam.m_MachineAlias, NewParam.m_MachineAlias, List);	

	CmpAndAddLogOperModif_STR(_T("Machine MES Name"), OldParam.m_MachineMES_Name, NewParam.m_MachineMES_Name, List);	
	CmpAndAddLogOperModif_STR(_T("Machine MES Password"), OldParam.m_MachineMES_Password, NewParam.m_MachineMES_Password, List);	
	CmpAndAddLogOperModif_STR(_T("Machine MES Device"), OldParam.m_MachineMES_Device, NewParam.m_MachineMES_Device, List);	
	CmpAndAddLogOperModif_STR(_T("Machine MES Device 2"), OldParam.m_MachineMES_Device2, NewParam.m_MachineMES_Device2, List);
	CmpAndAddLogOperModif_STR(_T("Machine MES CodeName"), OldParam.m_MachineMES_CodeName, NewParam.m_MachineMES_CodeName, List);	

	CmpAndAddLogOperModif_INT(_T("AOI Customer ID"), OldParam.m_AOICustomerID, NewParam.m_AOICustomerID, List);	
	CmpAndAddLogOperModif_INT(_T("Machine Model Type"), OldParam.m_MachineModelType, NewParam.m_MachineModelType, List);	
	CmpAndAddLogOperModif_INT(_T("Machine Camera Side"), OldParam.m_MachineCameraSide, NewParam.m_MachineCameraSide, List);	
	CmpAndAddLogOperModif_INT(_T("Multi Language Mode"), OldParam.m_MultiLanguageMode, NewParam.m_MultiLanguageMode, List);	
	CmpAndAddLogOperModif_INT(_T("Stretch Blt Mode"), OldParam.m_StretchBltMode, NewParam.m_StretchBltMode, List);	
	CmpAndAddLogOperModif_INT(_T("Debayer Mode"), OldParam.m_DebayerMode, NewParam.m_DebayerMode, List);	

	//CmpAndAddLogOperModif_INT(_T("Unwrapping Mode"), OldParam.m_UnwrappingMode, NewParam.m_UnwrappingMode, List);	
	CmpAndAddLogOperModif_INT(_T("Image Display Mode"), OldParam.m_ImageDisplayMode, NewParam.m_ImageDisplayMode, List);	
	CmpAndAddLogOperModif_INT(_T("Image Display Enhance Mode"), OldParam.m_ImageDisplayEnhanceMode, NewParam.m_ImageDisplayEnhanceMode, List);	
	CmpAndAddLogOperModif_DBL(_T("Image Display Gain"), OldParam.m_ImageDisplayGain, NewParam.m_ImageDisplayGain, List);
	CmpAndAddLogOperModif_DBL(_T("Image Display Gamma"), OldParam.m_ImageDisplayGamma, NewParam.m_ImageDisplayGamma, List);
	CmpAndAddLogOperModif_INT(_T("Image Display Sharpness Radius"), OldParam.m_ImageDisplaySharpnessRadius, NewParam.m_ImageDisplaySharpnessRadius, List);	
	CmpAndAddLogOperModif_INT(_T("Image Display Sharpness Amount"), OldParam.m_ImageDisplaySharpnessAmount, NewParam.m_ImageDisplaySharpnessAmount, List);	
	CmpAndAddLogOperModif_INT(_T("Image Display Sharpness Threshold"), OldParam.m_ImageDisplaySharpnessThreshold, NewParam.m_ImageDisplaySharpnessThreshold, List);	
	CmpAndAddLogOperModif_INT(_T("Image Display Local Gamma Calc Size"), OldParam.m_ImageDisplayLocalGammaCalcSize, NewParam.m_ImageDisplayLocalGammaCalcSize, List);	
	CmpAndAddLogOperModif_DBL(_T("Image Display Local Gamma Scale Val"), OldParam.m_ImageDisplayLocalGammaScaleVal, NewParam.m_ImageDisplayLocalGammaScaleVal, List);
	CmpAndAddLogOperModif_DBL(_T("Image Display Max Zoom Scale"), OldParam.m_ImageDisplayMaxZoomScale, NewParam.m_ImageDisplayMaxZoomScale, List);	
	CmpAndAddLogOperModif_INT(_T("Online Form View Mode"), OldParam.m_OnlineFormViewMode, NewParam.m_OnlineFormViewMode, List);	
	CmpAndAddLogOperModif_INT(_T("Auto Retry Max Count"), OldParam.m_AutoRetryMaxCount, NewParam.m_AutoRetryMaxCount, List);	
	CmpAndAddLogOperModif_INT(_T("Auto Setup 3D Pattern"), OldParam.m_AutoSetupDlpPattern, NewParam.m_AutoSetupDlpPattern, List);		

	CmpAndAddLogOperModif_INT(_T("Resolution Mode"), OldParam.m_ResolutionModeTmp, NewParam.m_ResolutionModeTmp, List);		
	CmpAndAddLogOperModif_DBL(_T("Resolution Show Scale"), OldParam.m_ResolutionShowScale, NewParam.m_ResolutionShowScale, List);

	CmpAndAddLogOperModif_INT(_T("CPU Max Count Used"), OldParam.m_CpuMaxCountUsed, NewParam.m_CpuMaxCountUsed, List);		
	CmpAndAddLogOperModif_INT(_T("MT Count Slice Fill"), OldParam.m_MTCount_SliceFill, NewParam.m_MTCount_SliceFill, List);	
	CmpAndAddLogOperModif_INT(_T("MT Count Frame Merge"), OldParam.m_MTCount_FrameMerge, NewParam.m_MTCount_FrameMerge, List);	
	CmpAndAddLogOperModif_INT(_T("MT Count Field Merge"), OldParam.m_MTCount_FieldMerge, NewParam.m_MTCount_FieldMerge, List);	
	CmpAndAddLogOperModif_INT(_T("MT Count Frame Load"), OldParam.m_MTCount_FrameLoad, NewParam.m_MTCount_FrameLoad, List);	
	CmpAndAddLogOperModif_INT(_T("MT Count Region Calc"), OldParam.m_MTCount_RegionCalc, NewParam.m_MTCount_RegionCalc, List);	
	CmpAndAddLogOperModif_INT(_T("MT Count Proc Idle"), OldParam.m_MTCount_ProcIdle, NewParam.m_MTCount_ProcIdle, List);
	CmpAndAddLogOperModif_INT(_T("MT Count Grab Idle"), OldParam.m_MTCount_GrabIdle, NewParam.m_MTCount_GrabIdle, List);

	CmpAndAddLogOperModif_INT(_T("OpenMP Count General"), OldParam.m_OpenMPCount_General, NewParam.m_OpenMPCount_General, List);	
	CmpAndAddLogOperModif_INT(_T("OpenMP Count Inspection"), OldParam.m_OpenMPCount_Inspection, NewParam.m_OpenMPCount_Inspection, List);	
	CmpAndAddLogOperModif_INT(_T("OpenMP Check Size Inspection"), OldParam.m_OpenMPCheckSize_Inspection, NewParam.m_OpenMPCheckSize_Inspection, List);	
	
	CmpAndAddLogOperModif_DBL(_T("Phase Period 1"), OldParam.m_PhasePeriod1, NewParam.m_PhasePeriod1, List);
	CmpAndAddLogOperModif_DBL(_T("Phase Period 2"), OldParam.m_PhasePeriod2, NewParam.m_PhasePeriod2, List);
	CmpAndAddLogOperModif_DBL(_T("Phase Period 3"), OldParam.m_PhasePeriod3, NewParam.m_PhasePeriod3, List);
	CmpAndAddLogOperModif_BOL(_T("Separate DLP 2-Exp Table"), OldParam.m_SeparateDLP2ExpTable, NewParam.m_SeparateDLP2ExpTable, List);
	CmpAndAddLogOperModif_INT(_T("Phase Convert Height Mode"), OldParam.m_PhaseConvertHeightMode, NewParam.m_PhaseConvertHeightMode, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Define"), OldParam.m_PhaseNoiseDefine, NewParam.m_PhaseNoiseDefine, List);
	CmpAndAddLogOperModif_INT(_T("Phase Noise Low Contrast A"), OldParam.m_PhaseNoiseLowContrastA, NewParam.m_PhaseNoiseLowContrastA, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Low Potential A"), OldParam.m_PhaseNoiseLowPotentialA, NewParam.m_PhaseNoiseLowPotentialA, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Over Saturated A"), OldParam.m_PhaseNoiseOverSaturatedA, NewParam.m_PhaseNoiseOverSaturatedA, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Low Contrast B"), OldParam.m_PhaseNoiseLowContrastB, NewParam.m_PhaseNoiseLowContrastB, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Low Potential B"), OldParam.m_PhaseNoiseLowPotentialB, NewParam.m_PhaseNoiseLowPotentialB, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Over Saturated B"), OldParam.m_PhaseNoiseOverSaturatedB, NewParam.m_PhaseNoiseOverSaturatedB, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Low Contrast C"), OldParam.m_PhaseNoiseLowContrastC, NewParam.m_PhaseNoiseLowContrastC, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Low Potential C"), OldParam.m_PhaseNoiseLowPotentialC, NewParam.m_PhaseNoiseLowPotentialC, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Over Saturated C"), OldParam.m_PhaseNoiseOverSaturatedC, NewParam.m_PhaseNoiseOverSaturatedC, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Extend Void"), OldParam.m_PhaseNoiseExtendVoid, NewParam.m_PhaseNoiseExtendVoid, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Noise Smooth Filter"), OldParam.m_PhaseNoiseSmoothFilter, NewParam.m_PhaseNoiseSmoothFilter, List);	

	CmpAndAddLogOperModif_CLR(_T("Phase Noise Low Contrast Color"), OldParam.m_PhaseNoiseLowContrastColor, NewParam.m_PhaseNoiseLowContrastColor, List);
	CmpAndAddLogOperModif_CLR(_T("Phase Noise Low Potential Color"), OldParam.m_PhaseNoiseLowPotentialColor, NewParam.m_PhaseNoiseLowPotentialColor, List);
	CmpAndAddLogOperModif_CLR(_T("Phase Noise Over Saturated Color"), OldParam.m_PhaseNoiseOverSaturatedColor, NewParam.m_PhaseNoiseOverSaturatedColor, List);
	CmpAndAddLogOperModif_CLR(_T("Phase Noise Extend Void Color"), OldParam.m_PhaseNoiseExtendVoidColor, NewParam.m_PhaseNoiseExtendVoidColor, List);
	CmpAndAddLogOperModif_CLR(_T("Space Noise Height Unexpected Color"), OldParam.m_SpaceNoiseHeightUnexpectedColor, NewParam.m_SpaceNoiseHeightUnexpectedColor, List);
	CmpAndAddLogOperModif_CLR(_T("Space Noise Height Over Low Color"), OldParam.m_SpaceNoiseHeightOverLowColor, NewParam.m_SpaceNoiseHeightOverLowColor, List);
	CmpAndAddLogOperModif_CLR(_T("Space Best Valid Color"), OldParam.m_SpaceBestValidColor, NewParam.m_SpaceBestValidColor, List);

	CmpAndAddLogOperModif_DBL(_T("3D Object Draw Scale X"), OldParam.m_3DObjectDrawScaleX, NewParam.m_3DObjectDrawScaleX, List);
	CmpAndAddLogOperModif_DBL(_T("3D Object Draw Scale Y"), OldParam.m_3DObjectDrawScaleY, NewParam.m_3DObjectDrawScaleY, List);
	CmpAndAddLogOperModif_DBL(_T("3D Object Draw Scale Z"), OldParam.m_3DObjectDrawScaleZ, NewParam.m_3DObjectDrawScaleZ, List);

	CmpAndAddLogOperModif_CLR(_T("Panel Color 1"), OldParam.m_PanelColor1, NewParam.m_PanelColor1, List);	
	CmpAndAddLogOperModif_CLR(_T("Panel Color 2"), OldParam.m_PanelColor2, NewParam.m_PanelColor2, List);	
	CmpAndAddLogOperModif_CLR(_T("Panel Text Color"), OldParam.m_PanelTextColor, NewParam.m_PanelTextColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Panel Selected Color"), OldParam.m_PanelSelectedColor, NewParam.m_PanelSelectedColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Board Color 1"), OldParam.m_BoardColor1, NewParam.m_BoardColor1, List);	
	CmpAndAddLogOperModif_CLR(_T("Board Color 2"), OldParam.m_BoardColor2, NewParam.m_BoardColor2, List);	
	CmpAndAddLogOperModif_CLR(_T("Board Text Color"), OldParam.m_BoardTextColor, NewParam.m_BoardTextColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Board Selected Color"), OldParam.m_BoardSelectedColor, NewParam.m_BoardSelectedColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Fd Color 1"), OldParam.m_FdColor1, NewParam.m_FdColor1, List);	
	CmpAndAddLogOperModif_CLR(_T("Fd Color 2"), OldParam.m_FdColor2, NewParam.m_FdColor2, List);	
	CmpAndAddLogOperModif_CLR(_T("Fd Tex tColor"), OldParam.m_FdTextColor, NewParam.m_FdTextColor, List);
	CmpAndAddLogOperModif_CLR(_T("Fd Selected Color"), OldParam.m_FdSelectedColor, NewParam.m_FdSelectedColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Barcode Color 1"), OldParam.m_BarcodeColor1, NewParam.m_BarcodeColor1, List);	
	CmpAndAddLogOperModif_CLR(_T("Barcode Color 2"), OldParam.m_BarcodeColor2, NewParam.m_BarcodeColor2, List);	
	CmpAndAddLogOperModif_CLR(_T("Barcode Text Color"), OldParam.m_BarcodeTextColor, NewParam.m_BarcodeTextColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Barcode Selected Color"), OldParam.m_BarcodeSelectedColor, NewParam.m_BarcodeSelectedColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Component Color 1"), OldParam.m_ComponentColor1, NewParam.m_ComponentColor1, List);	
	CmpAndAddLogOperModif_CLR(_T("Component Color 2"), OldParam.m_ComponentColor2, NewParam.m_ComponentColor2, List);	
	CmpAndAddLogOperModif_CLR(_T("Component Text Color"), OldParam.m_ComponentTextColor, NewParam.m_ComponentTextColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Component Selected Color"), OldParam.m_ComponentSelectedColor, NewParam.m_ComponentSelectedColor, List);	
	CmpAndAddLogOperModif_CLR(_T("District Color 1"), OldParam.m_DistrictColor1, NewParam.m_DistrictColor1, List);	
	CmpAndAddLogOperModif_CLR(_T("District Color 2"), OldParam.m_DistrictColor2, NewParam.m_DistrictColor2, List);	
	CmpAndAddLogOperModif_CLR(_T("Inspected Result OK Color"), OldParam.m_InspectedResultOKColor, NewParam.m_InspectedResultOKColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Inspected Result NG Color"), OldParam.m_InspectedResultNGColor, NewParam.m_InspectedResultNGColor, List);
	CmpAndAddLogOperModif_CLR(_T("Inspected Result Skip Color"), OldParam.m_InspectedResultSkipColor, NewParam.m_InspectedResultSkipColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Inspected Result Bypass Color"), OldParam.m_InspectedResultBypassColor, NewParam.m_InspectedResultBypassColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Inspected Result UnTest Color"), OldParam.m_InspectedResultUnTestColor, NewParam.m_InspectedResultUnTestColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Inspected Result Warning Color"), OldParam.m_InspectedResultWarningColor, NewParam.m_InspectedResultWarningColor, List);	
	CmpAndAddLogOperModif_CLR(_T("Inspected Result Exception Color"), OldParam.m_InspectedResultExceptionColor, NewParam.m_InspectedResultExceptionColor, List);	
	
	CmpAndAddLogOperModif_DBL(_T("Space Noise Single Cast Low Limit"), OldParam.m_SpaceNoiseSingleCastLowLimit, NewParam.m_SpaceNoiseSingleCastLowLimit, List);
	CmpAndAddLogOperModif_INT(_T("Space Noise Multi Cast Patch Size"), OldParam.m_SpaceNoiseMultiCastPatchSize, NewParam.m_SpaceNoiseMultiCastPatchSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Multi Cast Merge Mode"), OldParam.m_SpaceNoiseMultiCastMergeMode, NewParam.m_SpaceNoiseMultiCastMergeMode, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Multi Cast Merge Best Mode"), OldParam.m_SpaceNoiseMultiCastMergeBestMode, NewParam.m_SpaceNoiseMultiCastMergeBestMode, List);
	CmpAndAddLogOperModif_INT(_T("Space Noise Multi Intensity Merge Mode"), OldParam.m_SpaceNoiseMultiIntensityMergeMode, NewParam.m_SpaceNoiseMultiIntensityMergeMode, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Multi Cast Min Valid Count"), OldParam.m_SpaceNoiseMultiCastMinValidCount, NewParam.m_SpaceNoiseMultiCastMinValidCount, List);
	CmpAndAddLogOperModif_DBL(_T("Space Noise Multi Cast Max Difference"), OldParam.m_SpaceNoiseMultiCastMaxDifference, NewParam.m_SpaceNoiseMultiCastMaxDifference, List);
	CmpAndAddLogOperModif_DBL(_T("Space Noise Multi Cast Limit Difference"), OldParam.m_SpaceNoiseMultiCastLimitDifference, NewParam.m_SpaceNoiseMultiCastLimitDifference, List);
	CmpAndAddLogOperModif_DBL(_T("Space Noise Multi Cast Valid Best Ratio"), OldParam.m_SpaceNoiseMultiCastValidBestRatio, NewParam.m_SpaceNoiseMultiCastValidBestRatio, List);
	CmpAndAddLogOperModif_DBL(_T("Space Noise Multi Cast Valid Difference"), OldParam.m_SpaceNoiseMultiCastValidDifference, NewParam.m_SpaceNoiseMultiCastValidDifference, List);
	CmpAndAddLogOperModif_INT(_T("Space Noise Multi Cast Opposite Max Gray"), OldParam.m_SpaceNoiseMultiCastOppositeMaxGray, NewParam.m_SpaceNoiseMultiCastOppositeMaxGray, List);

	CmpAndAddLogOperModif_INT(_T("Space Noise Cast Filter Mode"), OldParam.m_SpaceNoiseCastFilterMode, NewParam.m_SpaceNoiseCastFilterMode, List);
	CmpAndAddLogOperModif_INT(_T("Space Noise Cast Median Filter Size"), OldParam.m_SpaceNoiseCastMedianFilterSize, NewParam.m_SpaceNoiseCastMedianFilterSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Cast Median Filter Use Size"), OldParam.m_SpaceNoiseCastMedianFilterUseSize, NewParam.m_SpaceNoiseCastMedianFilterUseSize, List);	
	
	CmpAndAddLogOperModif_INT(_T("Space Merge Recursion Mode"), OldParam.m_SpaceMergeRecursionMode, NewParam.m_SpaceMergeRecursionMode, List);	
	CmpAndAddLogOperModif_INT(_T("Space Merge Recursion Max Count"), OldParam.m_SpaceMergeRecursionMaxCount, NewParam.m_SpaceMergeRecursionMaxCount, List);	
	CmpAndAddLogOperModif_INT(_T("Space Merge Recursion Kernel Size"), OldParam.m_SpaceMergeRecursionKernelSize, NewParam.m_SpaceMergeRecursionKernelSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Merge Recursion Mask Size"), OldParam.m_SpaceMergeRecursionMaskSize, NewParam.m_SpaceMergeRecursionMaskSize, List);	

	CmpAndAddLogOperModif_INT(_T("Space Noise Data Void Expand Size"), OldParam.m_SpaceNoiseDataVoidExpandSize, NewParam.m_SpaceNoiseDataVoidExpandSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Data Void Expand Enabed"), OldParam.m_SpaceNoiseDataVoidExpandEnabed, NewParam.m_SpaceNoiseDataVoidExpandEnabed, List);	

	CmpAndAddLogOperModif_INT(_T("Space Noise First Filter Mode"), OldParam.m_SpaceNoiseFirstFilterMode, NewParam.m_SpaceNoiseFirstFilterMode, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise First Filter Pitch"), OldParam.m_SpaceNoiseFirstFilterPitch, NewParam.m_SpaceNoiseFirstFilterPitch, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise First Ker Size"), OldParam.m_SpaceNoiseFirstKerSize, NewParam.m_SpaceNoiseFirstKerSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise First Use Size"), OldParam.m_SpaceNoiseFirstUseSize, NewParam.m_SpaceNoiseFirstUseSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise First Filter Alpha F"), OldParam.m_SpaceNoiseFirstFilterAlphaF, NewParam.m_SpaceNoiseFirstFilterAlphaF, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise First Filter Alpha S"), OldParam.m_SpaceNoiseFirstFilterAlphaS, NewParam.m_SpaceNoiseFirstFilterAlphaS, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise First Filter Alpha M"), OldParam.m_SpaceNoiseFirstFilterAlphaM, NewParam.m_SpaceNoiseFirstFilterAlphaM, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise First Filter Alpha I"), OldParam.m_SpaceNoiseFirstFilterAlphaI, NewParam.m_SpaceNoiseFirstFilterAlphaI, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise First Filter Thresdhold Outlier"), OldParam.m_SpaceNoiseFirstFilterThresdhold_Outlier, NewParam.m_SpaceNoiseFirstFilterThresdhold_Outlier, List);	
	CmpAndAddLogOperModif_BOL(_T("Space Noise First Filter Search On"), OldParam.m_SpaceNoiseFirstFilterSearchOn, NewParam.m_SpaceNoiseFirstFilterSearchOn, List);	

	CmpAndAddLogOperModif_INT(_T("Space Noise Over Lower Mode"), OldParam.m_SpaceNoiseOverLowerMode, NewParam.m_SpaceNoiseOverLowerMode, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Over Lower Range"), OldParam.m_SpaceNoiseOverLowerRange, NewParam.m_SpaceNoiseOverLowerRange, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Over Lower Limit"), OldParam.m_SpaceNoiseOverLowerLimit, NewParam.m_SpaceNoiseOverLowerLimit, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Over Lower KerSize"), OldParam.m_SpaceNoiseOverLowerKerSize, NewParam.m_SpaceNoiseOverLowerKerSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Over Lower UseSize"), OldParam.m_SpaceNoiseOverLowerUseSize, NewParam.m_SpaceNoiseOverLowerUseSize, List);	

	CmpAndAddLogOperModif_INT(_T("Space Noise Height Abnormal Mode"), OldParam.m_SpaceNoiseHeightAbnormalMode, NewParam.m_SpaceNoiseHeightAbnormalMode, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Height Abnormal Pitch"), OldParam.m_SpaceNoiseHeightAbnormalPitch, NewParam.m_SpaceNoiseHeightAbnormalPitch, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Height Abnormal Range"), OldParam.m_SpaceNoiseHeightAbnormalRange, NewParam.m_SpaceNoiseHeightAbnormalRange, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Height Abnormal ChkSize"), OldParam.m_SpaceNoiseHeightAbnormalChkSize, NewParam.m_SpaceNoiseHeightAbnormalChkSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Height Abnormal KerSize"), OldParam.m_SpaceNoiseHeightAbnormalKerSize, NewParam.m_SpaceNoiseHeightAbnormalKerSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Height Abnormal UseSize"), OldParam.m_SpaceNoiseHeightAbnormalUseSize, NewParam.m_SpaceNoiseHeightAbnormalUseSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Height Abnormal RepeatCnt"), OldParam.m_SpaceNoiseHeightAbnormalRepeatCnt, NewParam.m_SpaceNoiseHeightAbnormalRepeatCnt, List);	

	CmpAndAddLogOperModif_INT(_T("Space Noise Data Void ReContructed ExtSize"), OldParam.m_SpaceNoiseDataVoidReContructedExtSize, NewParam.m_SpaceNoiseDataVoidReContructedExtSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Data Void ReContructed Enabled"), OldParam.m_SpaceNoiseDataVoidReContructedEnabled, NewParam.m_SpaceNoiseDataVoidReContructedEnabled, List);	

	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Mode"), OldParam.m_SpaceNoiseFinalFilterMode, NewParam.m_SpaceNoiseFinalFilterMode, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Pitch"), OldParam.m_SpaceNoiseFinalPitch, NewParam.m_SpaceNoiseFinalPitch, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Ker Size"), OldParam.m_SpaceNoiseFinalKerSize, NewParam.m_SpaceNoiseFinalKerSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Use Size"), OldParam.m_SpaceNoiseFinalUseSize, NewParam.m_SpaceNoiseFinalUseSize, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Alpha F"), OldParam.m_SpaceNoiseFinalFilterAlphaF, NewParam.m_SpaceNoiseFinalFilterAlphaF, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Alpha S"), OldParam.m_SpaceNoiseFinalFilterAlphaS, NewParam.m_SpaceNoiseFinalFilterAlphaS, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Alpha M"), OldParam.m_SpaceNoiseFinalFilterAlphaM, NewParam.m_SpaceNoiseFinalFilterAlphaM, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Alpha I"), OldParam.m_SpaceNoiseFinalFilterAlphaI, NewParam.m_SpaceNoiseFinalFilterAlphaI, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Thresdhold Outlier"), OldParam.m_SpaceNoiseFinalFilterThresdhold_Outlier, NewParam.m_SpaceNoiseFinalFilterThresdhold_Outlier, List);	
	CmpAndAddLogOperModif_BOL(_T("Space Noise Final Filter Search On"), OldParam.m_SpaceNoiseFinalFilterSearchOn, NewParam.m_SpaceNoiseFinalFilterSearchOn, List);	

	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Mode 2"), OldParam.m_SpaceNoiseFinalFilterMode2, NewParam.m_SpaceNoiseFinalFilterMode2, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Pitch 2"), OldParam.m_SpaceNoiseFinalPitch2, NewParam.m_SpaceNoiseFinalPitch2, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Ker Size 2"), OldParam.m_SpaceNoiseFinalKerSize2, NewParam.m_SpaceNoiseFinalKerSize2, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Use Size 2"), OldParam.m_SpaceNoiseFinalUseSize2, NewParam.m_SpaceNoiseFinalUseSize2, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Alpha F 2"), OldParam.m_SpaceNoiseFinalFilterAlphaF2, NewParam.m_SpaceNoiseFinalFilterAlphaF2, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Alpha S 2"), OldParam.m_SpaceNoiseFinalFilterAlphaS2, NewParam.m_SpaceNoiseFinalFilterAlphaS2, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Alpha M 2"), OldParam.m_SpaceNoiseFinalFilterAlphaM2, NewParam.m_SpaceNoiseFinalFilterAlphaM2, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Alpha I 2"), OldParam.m_SpaceNoiseFinalFilterAlphaI2, NewParam.m_SpaceNoiseFinalFilterAlphaI2, List);	
	CmpAndAddLogOperModif_INT(_T("Space Noise Final Filter Thresdhold Outlier 2"), OldParam.m_SpaceNoiseFinalFilterThresdhold_Outlier2, NewParam.m_SpaceNoiseFinalFilterThresdhold_Outlier2, List);	
	CmpAndAddLogOperModif_BOL(_T("Space Noise Final Filter Search On 2"), OldParam.m_SpaceNoiseFinalFilterSearchOn2, NewParam.m_SpaceNoiseFinalFilterSearchOn2, List);	
	
	CmpAndAddLogOperModif_INT(_T("Phase Smooth Cuda Mode"), OldParam.m_PhaseSmoothCudaMode, NewParam.m_PhaseSmoothCudaMode, List);	
	CmpAndAddLogOperModif_INT(_T("Phase Smooth Cuda Mask Size"), OldParam.m_PhaseSmoothCudaMaskSize, NewParam.m_PhaseSmoothCudaMaskSize, List);	

	CmpAndAddLogOperModif_INT(_T("CudaFn Enabled"), OldParam.m_CudaFnEnabled, NewParam.m_CudaFnEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Cuda Block Number"), OldParam.m_CudaBlockNumber, NewParam.m_CudaBlockNumber, List);	
	CmpAndAddLogOperModif_INT(_T("Cuda Thread Number"), OldParam.m_CudaThreadNumber, NewParam.m_CudaThreadNumber, List);	

	CmpAndAddLogOperModif_INT(_T("Calc Focus Smooth Size"), OldParam.m_CalcFocusSmoothSize, NewParam.m_CalcFocusSmoothSize, List);	
	CmpAndAddLogOperModif_INT(_T("Calc Focus Mode"), OldParam.m_CalcFocusMode, NewParam.m_CalcFocusMode, List);	

	CmpAndAddLogOperModif_INT(_T("Memory Keep Mode"), OldParam.m_MemoryKeepMode, NewParam.m_MemoryKeepMode, List);	
	CmpAndAddLogOperModif_INT(_T("Match Lib Type"), OldParam.m_MatchLibType, NewParam.m_MatchLibType, List);	
	CmpAndAddLogOperModif_INT(_T("Enable Honeywell Swift Decoder"), OldParam.m_HoneywellSwiftDecoderEnabled, NewParam.m_HoneywellSwiftDecoderEnabled, List);	

	CmpAndAddLogOperModif_INT(_T("Dongle Warning Remaining Days"), OldParam.m_DongleWarningRemainingDays, NewParam.m_DongleWarningRemainingDays, List);	
	CmpAndAddLogOperModif_INT(_T("Dongle Warning Remaining Count"), OldParam.m_DongleWarningRemainingCount, NewParam.m_DongleWarningRemainingCount, List);	

	CmpAndAddLogOperModif_INT(_T("Last Station Line Mode"), OldParam.m_LastStationLineMode, NewParam.m_LastStationLineMode, List);		
	CmpAndAddLogOperModif_INT(_T("Lane Work Mode LA"), OldParam.m_LaneWorkMode_LA, NewParam.m_LaneWorkMode_LA, List);	
	CmpAndAddLogOperModif_INT(_T("Lane Work Mode LB"), OldParam.m_LaneWorkMode_LB, NewParam.m_LaneWorkMode_LB, List);	
	CmpAndAddLogOperModif_INT(_T("Multi Tower Light"), OldParam.m_MultiTowerLight, NewParam.m_MultiTowerLight, List);	
	CmpAndAddLogOperModif_INT(_T("Check PCB Removed Count"), OldParam.m_CheckPCBRemovedCount, NewParam.m_CheckPCBRemovedCount, List);	
	CmpAndAddLogOperModif_INT(_T("Next Connected Buffer Type"), OldParam.m_NextConnectedBufferType, NewParam.m_NextConnectedBufferType, List);	
	CmpAndAddLogOperModif_INT(_T("Multi Project Test Order Mode"), OldParam.m_MultiProjectTestOrderMode, NewParam.m_MultiProjectTestOrderMode, List);	
	
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration DLP LED Color"), OldParam.m_OnlineAutoCalibrationDlpLedColor, NewParam.m_OnlineAutoCalibrationDlpLedColor, List);		
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Target Cap Delay Time"), OldParam.m_OnlineAutoCalibrationCapDelayTime, NewParam.m_OnlineAutoCalibrationCapDelayTime, List);		
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Mode - XYZ Home"), OldParam.m_OnlineAutoCalibrationMode_XYZHome, NewParam.m_OnlineAutoCalibrationMode_XYZHome, List);		
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Period - XYZ Home"), OldParam.m_OnlineAutoCalibrationPeriod_XYZHome, NewParam.m_OnlineAutoCalibrationPeriod_XYZHome, List);
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Mode - 2D Current"), OldParam.m_OnlineAutoCalibrationMode_2DCurrent, NewParam.m_OnlineAutoCalibrationMode_2DCurrent, List);		
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Period - 2D Current"), OldParam.m_OnlineAutoCalibrationPeriod_2DCurrent, NewParam.m_OnlineAutoCalibrationPeriod_2DCurrent, List);		
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Period - 3D Current"), OldParam.m_OnlineAutoCalibrationPeriod_3DCurrent, NewParam.m_OnlineAutoCalibrationPeriod_3DCurrent, List);
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Mode - 3D Zero Plane"), OldParam.m_OnlineAutoCalibrationMode_3DZeroPlane, NewParam.m_OnlineAutoCalibrationMode_3DZeroPlane, List);		
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Period - 3D Zero Plane"), OldParam.m_OnlineAutoCalibrationPeriod_3DZeroPlane, NewParam.m_OnlineAutoCalibrationPeriod_3DZeroPlane, List);		
	CmpAndAddLogOperModif_INT(_T("Online Auto Calibration Period - 3D Height Factor"), OldParam.m_OnlineAutoCalibrationPeriod_3DHeightFactor, NewParam.m_OnlineAutoCalibrationPeriod_3DHeightFactor, List);			

	CmpAndAddLogOperModif_INT(_T("Online Auto Stop By Idle Time"), OldParam.m_OnlineAutoStopByIdleTime, NewParam.m_OnlineAutoStopByIdleTime, List);

	strOld=AOIDataDefine.GetOnlineAutoStopBySpecTimeText(OldParam.m_OnlineAutoStopBySpecTime1);
	strNew=AOIDataDefine.GetOnlineAutoStopBySpecTimeText(NewParam.m_OnlineAutoStopBySpecTime1);	
	CmpAndAddLogOperModif_STR(_T("Online Auto Stop By Spec Time 1"), strOld, strNew, List);

	strOld=AOIDataDefine.GetOnlineAutoStopBySpecTimeText(OldParam.m_OnlineAutoStopBySpecTime2);
	strNew=AOIDataDefine.GetOnlineAutoStopBySpecTimeText(NewParam.m_OnlineAutoStopBySpecTime2);	
	CmpAndAddLogOperModif_STR(_T("Online Auto Stop By Spec Time 2"), strOld, strNew, List);

	strOld=AOIDataDefine.GetOnlineAutoStopBySpecTimeText(OldParam.m_OnlineAutoStopBySpecTime3);
	strNew=AOIDataDefine.GetOnlineAutoStopBySpecTimeText(NewParam.m_OnlineAutoStopBySpecTime3);	
	CmpAndAddLogOperModif_STR(_T("Online Auto Stop By Spec Time 3"), strOld, strNew, List);

	CmpAndAddLogOperModif_INT(_T("Auto Switch To OnlineView Time"), OldParam.m_AutoSwitchToOnlineViewTime, NewParam.m_AutoSwitchToOnlineViewTime, List);	
	CmpAndAddLogOperModif_INT(_T("Online Show Project Test Map"), OldParam.m_OnlineShowProjectTestMap, NewParam.m_OnlineShowProjectTestMap, List);
	CmpAndAddLogOperModif_INT(_T("Auto Switch To Online Remote Ctrl Time"), OldParam.m_AutoSwitchToOnlineRemoteCtrlTime, NewParam.m_AutoSwitchToOnlineRemoteCtrlTime, List);
	CmpAndAddLogOperModif_INT(_T("Auto Switch To Online Remote Ctrl Mode"), OldParam.m_AutoSwitchToOnlineRemoteCtrlMode, NewParam.m_AutoSwitchToOnlineRemoteCtrlMode, List);

	CmpAndAddLogOperModif_INT(_T("Move Camera Before PCB-In"), OldParam.m_MoveCameraBeforePCBIn, NewParam.m_MoveCameraBeforePCBIn, List);
	CmpAndAddLogOperModif_INT(_T("PCB-In Use Camera Image Mode"), OldParam.m_PCBInUseCameraImageMode, NewParam.m_PCBInUseCameraImageMode, List);
	CmpAndAddLogOperModif_INT(_T("Clamp Board Before Test Mode"), OldParam.m_ClampPcbBeforeTestMode, NewParam.m_ClampPcbBeforeTestMode, List);
	CmpAndAddLogOperModif_INT(_T("Grab Fiducial Delay Time ms"), OldParam.m_GrabFiducialDelayTime_ms, NewParam.m_GrabFiducialDelayTime_ms, List);	
	CmpAndAddLogOperModif_INT(_T("PCB-Out Direction"), OldParam.m_PCBOutDirection, NewParam.m_PCBOutDirection, List);	
	CmpAndAddLogOperModif_INT(_T("Bypass Last Signal"), OldParam.m_BypassLastSignal, NewParam.m_BypassLastSignal, List);	
	CmpAndAddLogOperModif_INT(_T("Bypass Next Signal"), OldParam.m_BypassNextSignal, NewParam.m_BypassNextSignal, List);
	CmpAndAddLogOperModif_INT(_T("PCB OK-NG Signal Delay Time"), OldParam.m_PCBOKNGSignalDelayTime, NewParam.m_PCBOKNGSignalDelayTime, List);	
	CmpAndAddLogOperModif_INT(_T("Edit Line Size Level"), OldParam.m_EditLineSizeLevel, NewParam.m_EditLineSizeLevel, List);	
	CmpAndAddLogOperModif_INT(_T("Online Tuning Keep Max Time"), OldParam.m_OnlineTuningKeepMaxTime, NewParam.m_OnlineTuningKeepMaxTime, List);	
	CmpAndAddLogOperModif_INT(_T("Online Tuning Saved Max Count"), OldParam.m_OnlineTuningSavedMaxCount, NewParam.m_OnlineTuningSavedMaxCount, List);	
	CmpAndAddLogOperModif_INT(_T("User Login Mode"), OldParam.m_UserLoginMode, NewParam.m_UserLoginMode, List);		
	CmpAndAddLogOperModif_INT(_T("User Login Options"), OldParam.m_UserLoginOptions, NewParam.m_UserLoginOptions, List);	
	//CmpAndAddLogOperModif_INT(_T("User Logout Time Online"), OldParam.m_UserLogoutTimeOnline, NewParam.m_UserLogoutTimeOnline, List);	
	
	CmpAndAddLogOperModif_INT(_T("Open Project Mode"), OldParam.m_OpenProjectMode, NewParam.m_OpenProjectMode, List);	
	CmpAndAddLogOperModif_INT(_T("Open Project Map Index"), OldParam.m_OpenProjectMapIndex, NewParam.m_OpenProjectMapIndex, List);	

	CmpAndAddLogOperModif_INT(_T("Verify Project Mode"), OldParam.m_VerifyProjectMode, NewParam.m_VerifyProjectMode, List);	
	CmpAndAddLogOperModif_STR(_T("Verify Project Filename"), OldParam.m_VerifyProjectFilename, NewParam.m_VerifyProjectFilename, List);	

	CmpAndAddLogOperModif_INT(_T("Model Name Use Part Number"), OldParam.m_ModelNameUsePartNumber, NewParam.m_ModelNameUsePartNumber, List);		

	CmpAndAddLogOperModif_INT(_T("Online Open Project Mode"), OldParam.m_OnlineOpenProjectMode, NewParam.m_OnlineOpenProjectMode, List);	
	CmpAndAddLogOperModif_INT(_T("Online Open Project Camera Barcode"), OldParam.m_OnlineOpenProjectCameraBarcode, NewParam.m_OnlineOpenProjectCameraBarcode, List);	
	CmpAndAddLogOperModif_INT(_T("Online Open Project Barcode Device Grab Mode"), OldParam.m_OnlineOpenProjectBarcodeDeviceGrabMode, NewParam.m_OnlineOpenProjectBarcodeDeviceGrabMode, List);	

	CmpAndAddLogOperModif_CLR(_T("Model Unset Color"), OldParam.m_ModelUnsetColor, NewParam.m_ModelUnsetColor, List);		
	CmpAndAddLogOperModif_CLR(_T("Binary Mask Color RGB"), OldParam.m_BinaryMaskColorRGB, NewParam.m_BinaryMaskColorRGB, List);		
	CmpAndAddLogOperModif_CLR(_T("Binary Mask Color Default"), OldParam.m_BinaryMaskColorDefault, NewParam.m_BinaryMaskColorDefault, List);		
	CmpAndAddLogOperModif_INT(_T("Binary Mask Color Alpha"), OldParam.m_BinaryMaskColorAlpha, NewParam.m_BinaryMaskColorAlpha, List);	

	CmpAndAddLogOperModif_INT(_T("Inspection Finish Show Result List"), OldParam.m_InspectionFinishShowResultList, NewParam.m_InspectionFinishShowResultList, List);	
	CmpAndAddLogOperModif_INT(_T("Mode lDefault Wnd Level"), OldParam.m_ModelDefaultWndLevel, NewParam.m_ModelDefaultWndLevel, List);	
	CmpAndAddLogOperModif_INT(_T("Switch Project 3D Frame"), OldParam.m_SwitchProject3DFrame, NewParam.m_SwitchProject3DFrame, List);	
	CmpAndAddLogOperModif_INT(_T("Auto Switch Wnd 3D Frame Mode"), OldParam.m_AutoSwitchWnd3DFrameMode, NewParam.m_AutoSwitchWnd3DFrameMode, List);	
	CmpAndAddLogOperModif_INT(_T("Auto Switch Wnd 3D Frame Size Limit"), OldParam.m_AutoSwitchWnd3DFrameSizeLimit, NewParam.m_AutoSwitchWnd3DFrameSizeLimit, List);	
	CmpAndAddLogOperModif_INT(_T("Show Component Full Map"), OldParam.m_ShowComponentFullMap, NewParam.m_ShowComponentFullMap, List);	
	CmpAndAddLogOperModif_INT(_T("Show Component Defect Only"), OldParam.m_ShowComponentDefectOnly, NewParam.m_ShowComponentDefectOnly, List);
	CmpAndAddLogOperModif_INT(_T("Show Debug Form View"), OldParam.m_ShowDebugFormView, NewParam.m_ShowDebugFormView, List);	
	CmpAndAddLogOperModif_INT(_T("Level Filter Shift Enabled"), OldParam.m_LevelFilterShiftEnabled, NewParam.m_LevelFilterShiftEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Median Filter Shift Enabled"), OldParam.m_MedianFilterShiftEnabled, NewParam.m_MedianFilterShiftEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Smooth Filter Shift Enabled"), OldParam.m_SmoothFilterShiftEnabled, NewParam.m_SmoothFilterShiftEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Pyramid Median Filter Shift Enabled"), OldParam.m_PyramidMedianFilterShiftEnabled, NewParam.m_PyramidMedianFilterShiftEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Level Filter Pitch Tolerance"), OldParam.m_LevelFilterPitchTolerance, NewParam.m_LevelFilterPitchTolerance, List);	
	CmpAndAddLogOperModif_INT(_T("Median Filter Pitch Tolerance"), OldParam.m_MedianFilterPitchTolerance, NewParam.m_MedianFilterPitchTolerance, List);	
	CmpAndAddLogOperModif_INT(_T("Smooth Filter Pitch Tolerance"), OldParam.m_SmoothFilterPitchTolerance, NewParam.m_SmoothFilterPitchTolerance, List);	
	CmpAndAddLogOperModif_INT(_T("Pyramid Median Filter Pitch Tolerance"), OldParam.m_PyramidMedianFilterPitchTolerance, NewParam.m_PyramidMedianFilterPitchTolerance, List);	
	CmpAndAddLogOperModif_INT(_T("Grr Offset Random Value"), OldParam.m_GrrOffsetRandomValue, NewParam.m_GrrOffsetRandomValue, List);	
	CmpAndAddLogOperModif_INT(_T("Grr Average Reset Enabled"), OldParam.m_GrrAverageResetEnabled, NewParam.m_GrrAverageResetEnabled, List);		
	CmpAndAddLogOperModif_DBL(_T("Grr Average Reset Match Ratio"), OldParam.m_GrrAverageResetMatchRatio, NewParam.m_GrrAverageResetMatchRatio, List); 
	CmpAndAddLogOperModif_DBL(_T("Grr Skew Average Enable Angle"), OldParam.m_GrrSkewAverageEnbVal, NewParam.m_GrrSkewAverageEnbVal, List);		
	CmpAndAddLogOperModif_DBL(_T("Grr Skew Average Start Gap"), OldParam.m_GrrSkewAverageStartGap, NewParam.m_GrrSkewAverageStartGap, List);		
	CmpAndAddLogOperModif_INT(_T("Grr Skew Average Weighting"), OldParam.m_GrrSkewAverageWeighting, NewParam.m_GrrSkewAverageWeighting, List);		
	CmpAndAddLogOperModif_INT(_T("Grr Offset Average Enable Pixel"), OldParam.m_GrrOffsetAverageEnbPxl, NewParam.m_GrrOffsetAverageEnbPxl, List);		
	CmpAndAddLogOperModif_DBL(_T("Grr Offset Average Start Gap"), OldParam.m_GrrOffsetAverageStartGap, NewParam.m_GrrOffsetAverageStartGap, List);		
	CmpAndAddLogOperModif_INT(_T("Grr Offset Average Weighting"), OldParam.m_GrrOffsetAverageWeighting, NewParam.m_GrrOffsetAverageWeighting, List);	
	CmpAndAddLogOperModif_DBL(_T("Grr Height Average Enable Value"), OldParam.m_GrrHeightAverageEnbVal, NewParam.m_GrrHeightAverageEnbVal, List);		
	CmpAndAddLogOperModif_DBL(_T("Grr Height Average Start Gap"), OldParam.m_GrrHeightAverageStartGap, NewParam.m_GrrHeightAverageStartGap, List);		
	CmpAndAddLogOperModif_INT(_T("Grr Height Average Weighting"), OldParam.m_GrrHeightAverageWeighting, NewParam.m_GrrHeightAverageWeighting, List);		
	CmpAndAddLogOperModif_INT(_T("Lock Screen Enabled"), OldParam.m_LockScreenEnabled, NewParam.m_LockScreenEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Lock Screen Key ID"), OldParam.m_LockScreenKeyID, NewParam.m_LockScreenKeyID, List);	
	CmpAndAddLogOperModif_INT(_T("Multi District Mode Enabled"), OldParam.m_MultiDistrictModeEnabled, NewParam.m_MultiDistrictModeEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Online Tuning Enable Barcode"), OldParam.m_OnlineTuningEnableBarcode, NewParam.m_OnlineTuningEnableBarcode, List);	
	CmpAndAddLogOperModif_INT(_T("Show PLC Safty Setting UI"), OldParam.m_ShowPlcSaftySettingUI, NewParam.m_ShowPlcSaftySettingUI, List);	
	CmpAndAddLogOperModif_INT(_T("Show Alg Offset-L Param"), OldParam.m_ShowAlgOffsetLParam, NewParam.m_ShowAlgOffsetLParam, List);	
	CmpAndAddLogOperModif_INT(_T("Show Alg Offset-A Param"), OldParam.m_ShowAlgOffsetAParam, NewParam.m_ShowAlgOffsetAParam, List);	
	CmpAndAddLogOperModif_INT(_T("Show Alg Bright-Ratio Scale Param"), OldParam.m_ShowAlgBrightRatioScaleParam, NewParam.m_ShowAlgBrightRatioScaleParam, List);
	CmpAndAddLogOperModif_INT(_T("Show Model Property Param"), OldParam.m_ShowModelPropertyParam, NewParam.m_ShowModelPropertyParam, List);	
	CmpAndAddLogOperModif_INT(_T("Continue Paste Mode"), OldParam.m_ContinuePasteMode, NewParam.m_ContinuePasteMode, List);	
	CmpAndAddLogOperModif_INT(_T("Stitch Image Padding Size"), OldParam.m_StitchImagePaddingSize, NewParam.m_StitchImagePaddingSize, List);	
	CmpAndAddLogOperModif_INT(_T("Resin Height Align Enabled"), OldParam.m_ResinHeightAlignEnabled, NewParam.m_ResinHeightAlignEnabled, List);
	CmpAndAddLogOperModif_INT(_T("Model Image Cad Offset Enabled"), OldParam.m_ModelImageCadOffsetEnabled, NewParam.m_ModelImageCadOffsetEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Model Default Transistor Type"), OldParam.m_ModelDefaultTransistorType, NewParam.m_ModelDefaultTransistorType, List);	
	CmpAndAddLogOperModif_INT(_T("Copy HugeFiles Mode"), OldParam.m_CopyHugeFilesMode, NewParam.m_CopyHugeFilesMode, List);		
	CmpAndAddLogOperModif_INT(_T("Project Local Folder Enabled"), OldParam.m_ProjectLocalFolderEnabled, NewParam.m_ProjectLocalFolderEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Partial Copy Project Library"), OldParam.m_PartialCopyProjectLibrary, NewParam.m_PartialCopyProjectLibrary, List);	
	CmpAndAddLogOperModif_INT(_T("Auto Arrange Model Bk-Image Files"), OldParam.m_AutoArrangeModelBkImageFiles, NewParam.m_AutoArrangeModelBkImageFiles, List);	
	CmpAndAddLogOperModif_INT(_T("Auto Copy Spc Component Image Files"), OldParam.m_AutoCopySpcComponentImageFiles, NewParam.m_AutoCopySpcComponentImageFiles, List);		
	CmpAndAddLogOperModif_INT(_T("Auto Bypass Grab 3D Frame"), OldParam.m_AutoBypassGrab3DFrame, NewParam.m_AutoBypassGrab3DFrame, List);
	CmpAndAddLogOperModif_INT(_T("Check Project Fd Ready"), OldParam.m_CheckProjectFdReady, NewParam.m_CheckProjectFdReady, List);		
	CmpAndAddLogOperModif_INT(_T("Lock Model Body Position"), OldParam.m_LockModelBodyPosition, NewParam.m_LockModelBodyPosition, List);				
	CmpAndAddLogOperModif_INT(_T("Max Uncheck Test File Count"), OldParam.m_MaxUncheckTestFileCount, NewParam.m_MaxUncheckTestFileCount, List); 
	CmpAndAddLogOperModif_INT(_T("Confirm Component Barcode Enabled"), OldParam.m_ConfirmComponentBarcodeEnabled, NewParam.m_ConfirmComponentBarcodeEnabled, List);
	CmpAndAddLogOperModif_INT(_T("Save JPEG Quality"), OldParam.m_SaveJpegQuality, NewParam.m_SaveJpegQuality, List);	
	CmpAndAddLogOperModif_INT(_T("Offline Version Mode"), OldParam.m_OfflineVersionMode, NewParam.m_OfflineVersionMode, List);	
	CmpAndAddLogOperModif_INT(_T("Use Project System Param Mode"), OldParam.m_UseProjectSystemParamMode, NewParam.m_UseProjectSystemParamMode, List);	

	CmpAndAddLogOperModif_INT(_T("UI Wnd Font Add Size"), OldParam.m_UIWndFontAddSize, NewParam.m_UIWndFontAddSize, List);	
	CmpAndAddLogOperModif_INT(_T("UI Dock Wnd Slide Steps"), OldParam.m_UIDockWndSlideSteps, NewParam.m_UIDockWndSlideSteps, List);	
	CmpAndAddLogOperModif_INT(_T("UI Enable PCB Out Button"), OldParam.m_UIEnablePCBOutButton, NewParam.m_UIEnablePCBOutButton, List);	

	CmpAndAddLogOperModif_INT(_T("Rabbit-MQ Server Port"), OldParam.m_RabbitMQServerPort, NewParam.m_RabbitMQServerPort, List);	
	CmpAndAddLogOperModif_STR(_T("Rabbit-MQ Server Address"), OldParam.m_RabbitMQServerAddress, NewParam.m_RabbitMQServerAddress, List);	
	CmpAndAddLogOperModif_STR(_T("Rabbit-MQ User Name"), OldParam.m_RabbitMQUserName, NewParam.m_RabbitMQUserName, List);	
	CmpAndAddLogOperModif_STR(_T("Rabbit-MQ Password"), OldParam.m_RabbitMQPassword, NewParam.m_RabbitMQPassword, List);	

	CmpAndAddLogOperModif_STR(_T("ITS Filename"), OldParam.m_ITSFilename, NewParam.m_ITSFilename, List);	
	CmpAndAddLogOperModif_INT(_T("ITS Communication Mode"), OldParam.m_ITSCommunicationMode, NewParam.m_ITSCommunicationMode, List);	
	CmpAndAddLogOperModif_INT(_T("ITS Socket IP Port"), OldParam.m_ITSSocketIPPort, NewParam.m_ITSSocketIPPort, List);	
	CmpAndAddLogOperModif_STR(_T("ITS Socket IP Address"), OldParam.m_ITSSocketIPAddress, NewParam.m_ITSSocketIPAddress, List);	
	CmpAndAddLogOperModif_STR(_T("ITS Rabbit-MQ Recv Queue Name"), OldParam.m_ITSRabbitMQRecvQueueName, NewParam.m_ITSRabbitMQRecvQueueName, List);	
	CmpAndAddLogOperModif_STR(_T("ITS Rabbit-MQ Send Queue Name"), OldParam.m_ITSRabbitMQSendQueueName, NewParam.m_ITSRabbitMQSendQueueName, List);	
	CmpAndAddLogOperModif_INT(_T("ITS Contact Software"), OldParam.m_ITSContactSoftware, NewParam.m_ITSContactSoftware, List);
	CmpAndAddLogOperModif_INT(_T("ITS Communication-Enabled"), OldParam.m_ITSCommunicationEnabled, NewParam.m_ITSCommunicationEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("ITS Communication Timeout"), OldParam.m_ITSCommunicationTimeoutMS, NewParam.m_ITSCommunicationTimeoutMS, List);
	CmpAndAddLogOperModif_STR(_T("ITS File Folder Send"), OldParam.m_ITSFileFolderSend, NewParam.m_ITSFileFolderSend, List);	
	CmpAndAddLogOperModif_STR(_T("ITS File Folder Recv"), OldParam.m_ITSFileFolderRecv, NewParam.m_ITSFileFolderRecv, List);	
	CmpAndAddLogOperModif_INT(_T("ITS File Backup Enabled"), OldParam.m_ITSFileBackupEnabled, NewParam.m_ITSFileBackupEnabled, List);		
	CmpAndAddLogOperModif_INT(_T("ITS File Use Sync File Enabled"), OldParam.m_ITSFileUseSyncFileEnabled, NewParam.m_ITSFileUseSyncFileEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("ITS Set SECS/GEM Enabled"), OldParam.m_ITSSetSecsGemEnabled, NewParam.m_ITSSetSecsGemEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("ITS SECS/GEM Remote Local"), OldParam.m_ITSSetSecsGemRemoteLocal, NewParam.m_ITSSetSecsGemRemoteLocal, List);		
	CmpAndAddLogOperModif_INT(_T("ITS Set System Param Enabled"), OldParam.m_ITSSetSystemParamEnabled, NewParam.m_ITSSetSystemParamEnabled, List);
	CmpAndAddLogOperModif_INT(_T("ITS Set Project Param Enabled"), OldParam.m_ITSSetProjectParamEnabled, NewParam.m_ITSSetProjectParamEnabled, List);
	CmpAndAddLogOperModif_INT(_T("ITS Set Machine Status Enabled"), OldParam.m_ITSSetMacineStatusEnabled, NewParam.m_ITSSetMacineStatusEnabled, List);
	CmpAndAddLogOperModif_INT(_T("ITS Set App Open Close Enabled"), OldParam.m_ITSSetAppOpnCloseEnabled, NewParam.m_ITSSetAppOpnCloseEnabled, List);
	CmpAndAddLogOperModif_INT(_T("ITS Set Process ID Enabled"), OldParam.m_ITSSetProcessIDEnabled, NewParam.m_ITSSetProcessIDEnabled, List);
	CmpAndAddLogOperModif_INT(_T("ITS Set Use Login-out Enabled"), OldParam.m_ITSSetUserLogin_outEnabled, NewParam.m_ITSSetUserLogin_outEnabled, List);	

	//CmpAndAddLogOperModif_BOL(_T("Save Pair Project Enabled"), OldParam.m_SavePairProjectEnabled, NewParam.m_SavePairProjectEnabled, List);	
	//CmpAndAddLogOperModif_BOL(_T("Online Pair Project Enabled"), OldParam.m_OnlinePairProjectEnabled, NewParam.m_OnlinePairProjectEnabled, List);	
	//CmpAndAddLogOperModif_BOL(_T("Tuning Pair Project Enabled"), OldParam.m_TuningPairProjectEnabled, NewParam.m_TuningPairProjectEnabled, List);	
	//CmpAndAddLogOperModif_BOL(_T("Map Pair Project StagePos"), OldParam.m_MapPairProjectStagePos, NewParam.m_MapPairProjectStagePos, List);	

	CmpAndAddLogOperModif_INT(_T("Default Space To Gray Ratio Mode"), OldParam.m_DefaultSpaceToGrayRatioMode, NewParam.m_DefaultSpaceToGrayRatioMode, List);	
	CmpAndAddLogOperModif_INT(_T("Default Space Base Plane Index"), OldParam.m_DefaultSpaceBasePlaneIndex, NewParam.m_DefaultSpaceBasePlaneIndex, List);	
	CmpAndAddLogOperModif_INT(_T("Default Space Noise Filter Index"), OldParam.m_DefaultSpaceNoiseFilterIndex, NewParam.m_DefaultSpaceNoiseFilterIndex, List);	
	CmpAndAddLogOperModif_INT(_T("Default Enable Conveyer Pre-Run"), OldParam.m_DefaultEnableConveyerPreRun, NewParam.m_DefaultEnableConveyerPreRun, List);	
	CmpAndAddLogOperModif_INT(_T("Default Fd NG Handle Mode"), OldParam.m_DefaultFdNGHandleMode, NewParam.m_DefaultFdNGHandleMode, List);	
	CmpAndAddLogOperModif_INT(_T("Default Board Fd Grab Mode"), OldParam.m_DefaultBoardFdGrabMode, NewParam.m_DefaultBoardFdGrabMode, List);	
	CmpAndAddLogOperModif_INT(_T("Default Defect Handle Mode"), OldParam.m_DefaultDefectHandleMode, NewParam.m_DefaultDefectHandleMode, List);	
	CmpAndAddLogOperModif_INT(_T("Default PCB Out Mode"), OldParam.m_DefaultPCBOutMode, NewParam.m_DefaultPCBOutMode, List);	
	CmpAndAddLogOperModif_INT(_T("Default Project Save Test Map Mode"), OldParam.m_DefaultProjectSaveTestMap, NewParam.m_DefaultProjectSaveTestMap, List);
	CmpAndAddLogOperModif_INT(_T("Default Project Link Server Mode"), OldParam.m_DefaultProjectLinkServerMode, NewParam.m_DefaultProjectLinkServerMode, List);	
	CmpAndAddLogOperModif_INT(_T("Default Project Save Offline Image Files"), OldParam.m_DefaultProjectSaveOfflineImageFiles, NewParam.m_DefaultProjectSaveOfflineImageFiles, List);		
	CmpAndAddLogOperModif_INT(_T("Default Barcode Verify Mode"), OldParam.m_DefaultBarcodeVerifyMode, NewParam.m_DefaultBarcodeVerifyMode, List);	
	CmpAndAddLogOperModif_INT(_T("Default Barcode Retrieve Mode"), OldParam.m_DefaultBarcodeRetrieveMode, NewParam.m_DefaultBarcodeRetrieveMode, List);	

	CmpAndAddLogOperModif_INT(_T("Operate Level Project Open"), OldParam.m_OperateLevelProjectOpen, NewParam.m_OperateLevelProjectOpen, List);	
	CmpAndAddLogOperModif_INT(_T("Operate Level Project Save"), OldParam.m_OperateLevelProjectSave, NewParam.m_OperateLevelProjectSave, List);	
	CmpAndAddLogOperModif_INT(_T("Operate Level Project Param"), OldParam.m_OperateLevelProjectParam, NewParam.m_OperateLevelProjectParam, List);		
	CmpAndAddLogOperModif_INT(_T("Operate Level Online Run"), OldParam.m_OperateLevelOnlineRun, NewParam.m_OperateLevelOnlineRun, List);
	CmpAndAddLogOperModif_INT(_T("Operate Level Online Bypass"), OldParam.m_OperateLevelOnlineBypass, NewParam.m_OperateLevelOnlineBypass, List);
	CmpAndAddLogOperModif_INT(_T("Operate Level Online Stop"), OldParam.m_OperateLevelOnlineStop, NewParam.m_OperateLevelOnlineStop, List);
	CmpAndAddLogOperModif_INT(_T("Operate Level Online Save Image"), OldParam.m_OperateLevelOnlineSaveImage, NewParam.m_OperateLevelOnlineSaveImage, List);		
	CmpAndAddLogOperModif_INT(_T("Operate Level Online Unlock"), OldParam.m_OperateLevelOnlineUnlock, NewParam.m_OperateLevelOnlineUnlock, List);	
	CmpAndAddLogOperModif_INT(_T("Operate Level Edit Func Add"), OldParam.m_OperateLevelEditFuncAdd, NewParam.m_OperateLevelEditFuncAdd, List);
	CmpAndAddLogOperModif_INT(_T("Operate Level Edit Func Delete"), OldParam.m_OperateLevelEditFuncDel, NewParam.m_OperateLevelEditFuncDel, List);
	CmpAndAddLogOperModif_INT(_T("Operate Level Edit Func Bypass"), OldParam.m_OperateLevelEditFuncBypass, NewParam.m_OperateLevelEditFuncBypass, List);	
	CmpAndAddLogOperModif_INT(_T("Operate Level MES Comm Ctrl State [Offline]"), OldParam.m_OperateLevelMesCtrlState_Offline, NewParam.m_OperateLevelMesCtrlState_Offline, List);
	CmpAndAddLogOperModif_INT(_T("Operate Level MES Comm Ctrl State [Local]"), OldParam.m_OperateLevelMesCtrlState_Local, NewParam.m_OperateLevelMesCtrlState_Local, List);
	CmpAndAddLogOperModif_INT(_T("Operate Level MES Comm Ctrl State [Remote]"), OldParam.m_OperateLevelMesCtrlState_Remote, NewParam.m_OperateLevelMesCtrlState_Remote, List);
	CmpAndAddLogOperModif_INT(_T("Operate Level MES Comm Show Context"), OldParam.m_OperateLevelMesShowContext, NewParam.m_OperateLevelMesShowContext, List);	

	CmpAndAddLogOperModif_INT(_T("M2M NPM Barcode Enable"), OldParam.m_M2M_NPM_Barcode_Enable, NewParam.m_M2M_NPM_Barcode_Enable, List);
	CmpAndAddLogOperModif_INT(_T("M2M NPM APC FF1 Enable"), OldParam.m_M2M_NPM_APC_FF1_Enable, NewParam.m_M2M_NPM_APC_FF1_Enable, List);
	CmpAndAddLogOperModif_INT(_T("M2M NPM APC FF2 Enable"), OldParam.m_M2M_NPM_APC_FF2_Enable, NewParam.m_M2M_NPM_APC_FF2_Enable, List);
	CmpAndAddLogOperModif_INT(_T("M2M NPM APC MFB Enable"), OldParam.m_M2M_NPM_APC_MFB_Enable, NewParam.m_M2M_NPM_APC_MFB_Enable, List);	 
	CmpAndAddLogOperModif_STR(_T("M2M NPM Lane Name LA"), NewParam.m_M2M_NPM_LaneName_LA, OldParam.m_M2M_NPM_LaneName_LA, List);
	CmpAndAddLogOperModif_STR(_T("M2M NPM Lane Name LB"), NewParam.m_M2M_NPM_LaneName_LB, OldParam.m_M2M_NPM_LaneName_LB, List);
	CmpAndAddLogOperModif_STR(_T("M2M NPM Input Share Folder LA"), NewParam.m_M2M_NPM_InputShareFolder_LA, OldParam.m_M2M_NPM_InputShareFolder_LA, List);	
	CmpAndAddLogOperModif_STR(_T("M2M NPM Input Share Folder LB"), NewParam.m_M2M_NPM_InputShareFolder_LB, OldParam.m_M2M_NPM_InputShareFolder_LB, List);
	CmpAndAddLogOperModif_STR(_T("M2M NPM Output Share Folder LA"), NewParam.m_M2M_NPM_OutputShareFolder_LA, OldParam.m_M2M_NPM_OutputShareFolder_LA, List);	
	CmpAndAddLogOperModif_STR(_T("M2M NPM Output Share Folder LB"), NewParam.m_M2M_NPM_OutputShareFolder_LB, OldParam.m_M2M_NPM_OutputShareFolder_LB, List);

	CmpAndAddLogOperModif_INT(_T("AI Model Server Enable"), NewParam.m_AiModelServerEnabled, OldParam.m_AiModelServerEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("AI Model Server Timeout"), NewParam.m_AiModelServerTimeout, OldParam.m_AiModelServerTimeout, List);	
	CmpAndAddLogOperModif_STR(_T("AI Model Server Filename"), NewParam.m_AiModelServerFilename, OldParam.m_AiModelServerFilename, List);	
	CmpAndAddLogOperModif_STR(_T("AI Model File Folder Send"), NewParam.m_AiModelFileFolderSend, OldParam.m_AiModelFileFolderSend, List);	
	CmpAndAddLogOperModif_STR(_T("AI Model File Folder Recv"), NewParam.m_AiModelFileFolderRecv, OldParam.m_AiModelFileFolderRecv, List);	
	CmpAndAddLogOperModif_DBL(_T("AI Model Label Min Cluster Distance"), NewParam.m_AIModelLabelMinClusterDistance, OldParam.m_AIModelLabelMinClusterDistance, List);	

	CmpAndAddLogOperModif_INT(_T("External Copy File Enable"), NewParam.m_ExternalCopyFileEnabled, OldParam.m_ExternalCopyFileEnabled, List);
	CmpAndAddLogOperModif_STR(_T("External Copy File App Name"), OldParam.m_ExternalCopyFileAppName, NewParam.m_ExternalCopyFileAppName, List);
	CmpAndAddLogOperModif_STR(_T("External Copy File Send Folder"), OldParam.m_ExternalCopyFileSendFolder, NewParam.m_ExternalCopyFileSendFolder, List);

	CmpAndAddLogOperModif_INT(_T("CPK Chart Enabled"), NewParam.m_CpkChartEnabled, OldParam.m_CpkChartEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Wnd Rotation Followed"), NewParam.m_WndRotationFollowed, OldParam.m_WndRotationFollowed, List);	

	/*			
	//CmpAndAddLogOperModif_STR(_T("AAAAAAAAAAAAAAAAAAAA"), NewParam.STRSTRSTRSTRSTRSTR, OldParam.STRSTRSTRSTRSTRSTR, List);	
	//CmpAndAddLogOperModif_INT(_T("AAAAAAAAAAAAAAAAAAAA"), NewParam.INTINTINTINTINTINT, OldParam.INTINTINTINTINTINT, List);	
	//CmpAndAddLogOperModif_DBL(_T("AAAAAAAAAAAAAAAAAAAA"), NewParam.DBLDBLDBLDBLDBLDBL, OldParam.DBLDBLDBLDBLDBLDBL, List);
	//CmpAndAddLogOperModif_CLR(_T("AAAAAAAAAAAAAAAAAAAA"), NewParam.CLRCLRCLRCLRCLRCLR, OldParam.CLRCLRCLRCLRCLRCLR, List);		
	*/

	size_t i=0;
	CString Content;		
	const size_t Count=List.size();
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	for ( i=0; i<Count; i++ )
	{
		const TLogOperModify &NodeRef=List[i];
		Content.Format(_T("%s [%s] %s [%s] %s [%s]"), sSet, NodeRef.m_sKey, sFrom, NodeRef.m_sOld, sTo, NodeRef.m_sNew);
		if ( AOIDataCollect.SaveLogOper_SystemFunc(strTag, Content) == false )		
		{	return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProject(CAOIProject *ProjectPtr, LPCTSTR Oper, LPCTSTR Val)//儲存操作訊息專案
{
	CString Content;
	if ( NULL == Val ) 
	{	Content = Oper; }
	else
	{	Content.Format(_T("%s %s"), Oper, Val); }
	return AOIDataCollect.SaveLogOper_ProjectFunc(ProjectPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectContent(CAOIProject *ProjectPtr, LPCTSTR Content)//儲存操作訊息-專案內容	
{
	return AOIDataCollect.SaveLogOper_ProjectFunc(ProjectPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectOperate(CAOIProject *ProjectPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-專案修改		
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogProjectContent(ProjectPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectNew(CAOIProject *ProjectPtr, LPCTSTR filename)//儲存操作訊息專案-新建
{
	return SaveLogProject(ProjectPtr, _T("New"), filename);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectOpen(CAOIProject *ProjectPtr, LPCTSTR filename)//儲存操作訊息專案
{
	return SaveLogProject(ProjectPtr, _T("Open"), filename);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectSave(CAOIProject *ProjectPtr, LPCTSTR filename)//儲存操作訊息專案
{	
	return SaveLogProject(ProjectPtr, _T("Save"), filename);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectClose(CAOIProject *ProjectPtr, LPCTSTR filename)//儲存操作訊息專案
{
	return SaveLogProject(ProjectPtr, _T("Close"), filename);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectCompare(CAOIProject *OldPtr, CAOIProject *NewPtr)//儲存操作訊息-專案比較
{
	if ( CheckProjectPtr(OldPtr) == false ) { return false; }	
	if ( CheckProjectPtr(NewPtr) == false ) { return false; }	

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	
	
	sKey = _T("Multi District Mode");	
	ChangeNode.SetBol(sKey, OldPtr->GetProjectMultiDistrictMode(), NewPtr->GetProjectMultiDistrictMode(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Deleted");	
	ChangeNode.SetBol(sKey, OldPtr->GetProjectDeleted(), NewPtr->GetProjectDeleted(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Barcode Device Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetProjectBarcodeDeviceIndex(), NewPtr->GetProjectBarcodeDeviceIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Barcode Device Code Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetProjectBarcodeDeviceCodeIndex(), NewPtr->GetProjectBarcodeDeviceCodeIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	CString sOper;
	ChangeCount=ChangeList.size();	
	sOper.Format(_T("%s %s"), AOIDataDefine.GetSetText(), _T("Project"));
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogProjectOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComparParam(CAOIProject *ProjectPtr, const TProjectParameter &Param)//儲存操作訊息-專案修改
{
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }	
	std::vector<TLogOperModify>  List;
	CString ShowName=ProjectPtr->GetProjectFileMainName(); 	
	const TProjectParameter &NewParam=ProjectPtr->GetProjectParameter();

	CmpAndAddLogOperModif_STR(_T("Model"), NewParam.m_ProjectModuleName, Param.m_ProjectModuleName, List);
	CmpAndAddLogOperModif_STR(_T("Version"), NewParam.m_ProjectVersion, Param.m_ProjectVersion, List);
	CmpAndAddLogOperModif_STR(_T("Work Number"), NewParam.m_ProjectWorkNumber, Param.m_ProjectWorkNumber, List);
	CmpAndAddLogOperModif_STR(_T("Open Code"), NewParam.m_ProjectOpenCode, Param.m_ProjectOpenCode, List);
	CmpAndAddLogOperModif_DBL(_T("Map Gan"), NewParam.m_RegionMapGain, Param.m_RegionMapGain, List);
	CmpAndAddLogOperModif_DBL(_T("Test Size Width"), NewParam.m_TestSizeWidth, Param.m_TestSizeWidth, List);
	CmpAndAddLogOperModif_DBL(_T("Test Size Height"), NewParam.m_TestSizeHeight, Param.m_TestSizeHeight, List);
	CmpAndAddLogOperModif_DBL(_T("Focus Offset"), NewParam.m_ProjectFocusOffset, Param.m_ProjectFocusOffset, List);
	CmpAndAddLogOperModif_DBL(_T("Lane Width"), NewParam.m_ProjectLaneWidth, Param.m_ProjectLaneWidth, List);
	CmpAndAddLogOperModif_DBL(_T("X-Board Check Ratio"), NewParam.m_ProjectXBoardCheckRatio, Param.m_ProjectXBoardCheckRatio, List);

	CmpAndAddLogOperModif_INT(_T("Space to Gray Ratio Mode"), NewParam.m_SpaceToGrayRatioMode, Param.m_SpaceToGrayRatioMode, List);
	CmpAndAddLogOperModif_INT(_T("DLP LED Color"), NewParam.m_ProjectDlpLedColor, Param.m_ProjectDlpLedColor, List);
	           
	CmpAndAddLogOperModif_INT(_T("Panel Side Mode"), NewParam.m_ProjectPanelSideMode, Param.m_ProjectPanelSideMode, List);//PANEL_SIDE_MODE 	            
	CmpAndAddLogOperModif_INT(_T("Field Size Mode W"), NewParam.m_ProjectFieldSizeModeW, Param.m_ProjectFieldSizeModeW, List);//FIELD_SIZE_MODE	
	CmpAndAddLogOperModif_INT(_T("Field Size Mode H"), NewParam.m_ProjectFieldSizeModeH, Param.m_ProjectFieldSizeModeH, List);//FIELD_SIZE_MODE	
	
	CmpAndAddLogOperModif_INT(_T("Save Test Map Mode"), NewParam.m_SaveProjectTestMap, Param.m_SaveProjectTestMap, List);//SAVE_TEST_MAP_MODE
	CmpAndAddLogOperModif_BOL(_T("Save Test Map 1"), NewParam.m_SaveProjectTestMap_01, Param.m_SaveProjectTestMap_01, List);
	CmpAndAddLogOperModif_BOL(_T("Save Test Map 2"), NewParam.m_SaveProjectTestMap_02, Param.m_SaveProjectTestMap_02, List);
	CmpAndAddLogOperModif_BOL(_T("Save Test Map 3"), NewParam.m_SaveProjectTestMap_03, Param.m_SaveProjectTestMap_03, List);
	CmpAndAddLogOperModif_BOL(_T("Save Test Map 4"), NewParam.m_SaveProjectTestMap_04, Param.m_SaveProjectTestMap_04, List);
	CmpAndAddLogOperModif_BOL(_T("Save Test Map 5"), NewParam.m_SaveProjectTestMap_05, Param.m_SaveProjectTestMap_05, List);
	CmpAndAddLogOperModif_BOL(_T("Save Test Map 6"), NewParam.m_SaveProjectTestMap_06, Param.m_SaveProjectTestMap_06, List);
	CmpAndAddLogOperModif_BOL(_T("Save Test Map 7"), NewParam.m_SaveProjectTestMap_07, Param.m_SaveProjectTestMap_07, List);
	CmpAndAddLogOperModif_BOL(_T("Save Test Map 8"), NewParam.m_SaveProjectTestMap_08, Param.m_SaveProjectTestMap_08, List);	
	CmpAndAddLogOperModif_BOL(_T("Save Test Map To Repair"), NewParam.m_SaveProjectTestMapToRepair, Param.m_SaveProjectTestMapToRepair, List);
	CmpAndAddLogOperModif_INT(_T("Test Test Map Scale Mode"), NewParam.m_ProjectTestMapScaleMode, Param.m_ProjectTestMapScaleMode, List);
	
	CmpAndAddLogOperModif_INT(_T("Online Tuning Mode"), NewParam.m_OnlineTuningMode, Param.m_OnlineTuningMode, List);//SAVE_TEST_IMAGE_MODE
	CmpAndAddLogOperModif_INT(_T("Save Model Image Mode"), NewParam.m_SaveModelImageMode, Param.m_SaveModelImageMode, List);//SAVE_TEST_IMAGE_MODE
	CmpAndAddLogOperModif_INT(_T("Save Field Image Mode"), NewParam.m_SaveFieldImageMode, Param.m_SaveFieldImageMode, List);//SAVE_TEST_IMAGE_MODE
	CmpAndAddLogOperModif_INT(_T("Save Model Image Mode For AI"), NewParam.m_SaveModelImageMode_AI, Param.m_SaveModelImageMode_AI, List);//SAVE_TEST_IMAGE_MODE	
	CmpAndAddLogOperModif_INT(_T("Save Model Image On/Off For AI"), NewParam.m_SaveModelImageOnOff_AI, Param.m_SaveModelImageOnOff_AI, List);
	CmpAndAddLogOperModif_INT(_T("Save Model Image 3D File For AI"), NewParam.m_SaveModelImage3DFile_AI, Param.m_SaveModelImage3DFile_AI, List);	
	CmpAndAddLogOperModif_INT(_T("Save Offline Image Scope"), NewParam.m_SaveOfflineImageScope, Param.m_SaveOfflineImageScope, List);//OFFLINE_IMAGE_SCOPE	
	CmpAndAddLogOperModif_INT(_T("Save Static Data"), NewParam.m_SaveStaticData, Param.m_SaveStaticData, List);
	CmpAndAddLogOperModif_INT(_T("Save Component Wnd List Mode"), NewParam.m_SaveComponentWndListMode, Param.m_SaveComponentWndListMode, List);	

	CmpAndAddLogOperModif_INT(_T("PCB Out Mode"), NewParam.m_PCBOutMode, Param.m_PCBOutMode, List);//PCB_OUT_MODE
	CmpAndAddLogOperModif_INT(_T("Panel Fd NG Skip Count"), NewParam.m_PanelFdNGSkipCount, Param.m_PanelFdNGSkipCount, List);	
	CmpAndAddLogOperModif_INT(_T("Panel Fd NG Handle Mode"), NewParam.m_PanelFdNGHandleMode, Param.m_PanelFdNGHandleMode, List);
	CmpAndAddLogOperModif_INT(_T("Board Fd NG Skip Count"), NewParam.m_BoardFdNGSkipCount, Param.m_BoardFdNGSkipCount, List);		
	CmpAndAddLogOperModif_INT(_T("Board Fd NG Handle Mode"), NewParam.m_BoardFdNGHandleMode, Param.m_BoardFdNGHandleMode, List);	
	CmpAndAddLogOperModif_INT(_T("Board Grab Mode"), NewParam.m_BoardFdGrabMode, Param.m_BoardFdGrabMode, List);//BOARD_FD_GRAB_MODE

	//CWndDefectItem             m_DefectTestItem;//瑕疵檢測項目模式
	//CWndDefectItem             m_DefectAlarmItem;//瑕疵警報項目模式
	//CWndDefectItem             m_DefectEnableItem;//瑕疵啟用項目模式	

	CmpAndAddLogOperModif_INT(_T("Defect Handle Mode"), NewParam.m_DefectHandleMode, Param.m_DefectHandleMode, List);//DEFECT_HANDLE_MODE
	CmpAndAddLogOperModif_BOL(_T("Conveyer Pre Run"), NewParam.m_EnableConveyerPreRun, Param.m_EnableConveyerPreRun, List);
	
	CmpAndAddLogOperModif_INT(_T("Field Build Mode"), NewParam.m_InspectionFieldBuildMode, Param.m_InspectionFieldBuildMode, List);//FIELD_BUILD_MODE
	CmpAndAddLogOperModif_INT(_T("Field Build Area Mode"), NewParam.m_InspectionFieldBuildAreaMode, Param.m_InspectionFieldBuildAreaMode, List);//FIELD_BUILD_MODE
	CmpAndAddLogOperModif_INT(_T("Field Division Mode"), NewParam.m_FieldDivisionMode, Param.m_FieldDivisionMode, List);//FIELD_DIVISION_MODE
	CmpAndAddLogOperModif_INT(_T("Field Division Board Fd First"), NewParam.m_FieldDivisionBoardFdFirst, Param.m_FieldDivisionBoardFdFirst, List);	
	CmpAndAddLogOperModif_INT(_T("Field Path Mode"), NewParam.m_FieldPathMode, Param.m_FieldPathMode, List);//FIELD_PATH_MODE
	CmpAndAddLogOperModif_DBL(_T("Field Section Factor"), NewParam.m_FieldSectionFactor, Param.m_FieldSectionFactor, List);
	
	CmpAndAddLogOperModif_INT(_T("Server Link Mode"), NewParam.m_ProjectLinkServerMode, Param.m_ProjectLinkServerMode, List);//PROJECT_LINK_SERVER_MODE
	CmpAndAddLogOperModif_STR(_T("Server Library Group"), NewParam.m_ProjectServerLibraryGroup, Param.m_ProjectServerLibraryGroup, List);	
	
	CmpAndAddLogOperModif_INT(_T("X-Board Mapping File Mode"), NewParam.m_XBoardMappingFileMode, Param.m_XBoardMappingFileMode, List);	
	CmpAndAddLogOperModif_INT(_T("X-Board Mapping File Trigger Time"), NewParam.m_XBoardMappingFileFlow, Param.m_XBoardMappingFileFlow, List);
	CmpAndAddLogOperModif_BOL(_T("X-Board Mapping File Board Check"), NewParam.m_XBoardMappingMESCheck, Param.m_XBoardMappingMESCheck, List);

	CmpAndAddLogOperModif_STR(_T("General String 01"), NewParam.m_ProjectGeneralParamStr_01, Param.m_ProjectGeneralParamStr_01, List);
	CmpAndAddLogOperModif_STR(_T("General String 02"), NewParam.m_ProjectGeneralParamStr_02, Param.m_ProjectGeneralParamStr_02, List);
	CmpAndAddLogOperModif_STR(_T("General String 03"), NewParam.m_ProjectGeneralParamStr_03, Param.m_ProjectGeneralParamStr_03, List);
	CmpAndAddLogOperModif_STR(_T("General String 04"), NewParam.m_ProjectGeneralParamStr_04, Param.m_ProjectGeneralParamStr_04, List);

	CmpAndAddLogOperModif_INT(_T("Statistic Defect From Mode"), NewParam.m_StatisticDefectFromMode, Param.m_StatisticDefectFromMode, List);//DEFECT_FROM_MODE
	CmpAndAddLogOperModif_INT(_T("Statistic Mode"), NewParam.m_StatisticByMode, Param.m_StatisticByMode, List);//STATISTIC_BY_MODE
	CmpAndAddLogOperModif_INT(_T("Statistic By Time"), NewParam.m_StatisticByTimeValue, Param.m_StatisticByTimeValue, List);	
	CmpAndAddLogOperModif_INT(_T("Statistic By Count"), NewParam.m_StatisticByCountValue, Param.m_StatisticByCountValue, List);	
	CmpAndAddLogOperModif_DBL(_T("Alarm Test Yield Min"), NewParam.m_AlaramTestYieldMin, Param.m_AlaramTestYieldMin, List);
	CmpAndAddLogOperModif_DBL(_T("Alarm Panel Yield Min"), NewParam.m_AlaramPanelYieldMin, Param.m_AlaramPanelYieldMin, List);
	CmpAndAddLogOperModif_DBL(_T("Alarm Board Yield Min"), NewParam.m_AlaramBoardYieldMin, Param.m_AlaramBoardYieldMin, List);
	CmpAndAddLogOperModif_DBL(_T("Alarm Component Yield Min"), NewParam.m_AlaramComponentYieldMin, Param.m_AlaramComponentYieldMin, List);
	CmpAndAddLogOperModif_DBL(_T("Alarm Component Defect Ratio Max"), NewParam.m_AlaramComponentDefectRateMax, Param.m_AlaramComponentDefectRateMax, List);	
	CmpAndAddLogOperModif_INT(_T("Alarm Each Component Total NG Count"), NewParam.m_AlaramEachComponentTotalNGCount, Param.m_AlaramEachComponentTotalNGCount, List);	
	CmpAndAddLogOperModif_INT(_T("Alarm Each Component Continue NG Count"), NewParam.m_AlaramEachComponentContinueNGCount, Param.m_AlaramEachComponentContinueNGCount, List);			
	CmpAndAddLogOperModif_INT(_T("Alarm Lock Mode"), NewParam.m_AlarmLockMode, Param.m_AlarmLockMode, List);	

	CmpAndAddLogOperModif_INT(_T("Statistic Defect From Mode (ARS)"), NewParam.m_StatisticDefectFromMode_ARS, Param.m_StatisticDefectFromMode_ARS, List);//DEFECT_FROM_MODE
	CmpAndAddLogOperModif_INT(_T("Statistic Mode (ARS)"), NewParam.m_StatisticByMode_ARS, Param.m_StatisticByMode_ARS, List);//STATISTIC_BY_MODE
	CmpAndAddLogOperModif_INT(_T("Statistic By Time (ARS)"), NewParam.m_StatisticByTimeValue_ARS, Param.m_StatisticByTimeValue_ARS, List);	
	CmpAndAddLogOperModif_INT(_T("Statistic By Count (ARS)"), NewParam.m_StatisticByCountValue_ARS, Param.m_StatisticByCountValue_ARS, List);	
	CmpAndAddLogOperModif_DBL(_T("Alarm Test Yield Min (ARS)"), NewParam.m_AlaramTestYieldMin_ARS, Param.m_AlaramTestYieldMin_ARS, List);
	CmpAndAddLogOperModif_DBL(_T("Alarm Panel Yield Min (ARS)"), NewParam.m_AlaramPanelYieldMin_ARS, Param.m_AlaramPanelYieldMin_ARS, List);
	CmpAndAddLogOperModif_DBL(_T("Alarm Board Yield Min (ARS)"), NewParam.m_AlaramBoardYieldMin_ARS, Param.m_AlaramBoardYieldMin_ARS, List);
	CmpAndAddLogOperModif_DBL(_T("Alarm Component Yield Min (ARS)"), NewParam.m_AlaramComponentYieldMin_ARS, Param.m_AlaramComponentYieldMin_ARS, List);
	CmpAndAddLogOperModif_DBL(_T("Alarm Component Defect Ratio Max (ARS)"), NewParam.m_AlaramComponentDefectRateMax_ARS, Param.m_AlaramComponentDefectRateMax_ARS, List);
	CmpAndAddLogOperModif_INT(_T("Alarm Each Component Total NG Count (ARS)"), NewParam.m_AlaramEachComponentTotalNGCount_ARS, Param.m_AlaramEachComponentTotalNGCount_ARS, List);	
	CmpAndAddLogOperModif_INT(_T("Alarm Each Component Continue NG Count (ARS)"), NewParam.m_AlaramEachComponentContinueNGCount_ARS, Param.m_AlaramEachComponentContinueNGCount_ARS, List);	

	//條碼設定
	CmpAndAddLogOperModif_BOL(_T("Barcode Input By File"), NewParam.m_BarcodeInputFileEnabled, Param.m_BarcodeInputFileEnabled, List);	
	CmpAndAddLogOperModif_BOL(_T("Barcode Input By Camera"), NewParam.m_BarcodeInputCameraEnabled, Param.m_BarcodeInputCameraEnabled, List);
	CmpAndAddLogOperModif_BOL(_T("Barcode Camera Save Image"), NewParam.m_BarcodeCameraSaveImageEnabled, Param.m_BarcodeCameraSaveImageEnabled, List);	
	CmpAndAddLogOperModif_INT(_T("Barcode Input Type"), NewParam.m_BarcodeInputType, Param.m_BarcodeInputType, List);	//BARCODE_INPUT_TYPE
	CmpAndAddLogOperModif_INT(_T("Barcode NG Handle Type"), NewParam.m_BarcodeNGHandleMode, Param.m_BarcodeNGHandleMode, List);	//BARCODE_NG_HANDLE_MODE	
	CmpAndAddLogOperModif_INT(_T("Barcode Camera Grab Mode"), NewParam.m_BarcodeCameraGrabMode, Param.m_BarcodeCameraGrabMode, List);//BARCODE_CAMERA_GRAB_MODE
	CmpAndAddLogOperModif_INT(_T("Barcode Devicde Grab Mode"), NewParam.m_BarcodeDeviceGrabMode, Param.m_BarcodeDeviceGrabMode, List);//BARCODE_DEVICE_GRAB_MODE
	CmpAndAddLogOperModif_INT(_T("Barcode HandHeld Read Mode"), NewParam.m_BarcodeHandHeldReadMode, Param.m_BarcodeHandHeldReadMode, List);//BARCODE_HANDHELD_READ_MODE		
	CmpAndAddLogOperModif_INT(_T("Barcode Input By File Delay Time"), NewParam.m_BarcodeInputFileDelayTime, Param.m_BarcodeInputFileDelayTime, List);	
	CmpAndAddLogOperModif_INT(_T("Barcode End Remove Char Count"), NewParam.m_BarcodeEndRemoveCharCount, Param.m_BarcodeEndRemoveCharCount, List);
	CmpAndAddLogOperModif_INT(_T("Barcode Begin Remove Char Count"), NewParam.m_BarcodeBeginRemoveCharCount, Param.m_BarcodeBeginRemoveCharCount, List);
	CmpAndAddLogOperModif_INT(_T("Barcode Auto Expand Mode [Panel]"), NewParam.m_BarcodeAutoExpandMode_Panel, Param.m_BarcodeAutoExpandMode_Panel, List);	
	CmpAndAddLogOperModif_INT(_T("Barcode Auto Expand Mode [Board]"), NewParam.m_BarcodeAutoExpandMode_Board, Param.m_BarcodeAutoExpandMode_Board, List);	
	CmpAndAddLogOperModif_INT(_T("Barcode Verify Mode"), NewParam.m_BarcodeVerifyMode, Param.m_BarcodeVerifyMode, List);	
	CmpAndAddLogOperModif_INT(_T("Barcode Retrieve Mode"), NewParam.m_BarcodeRetrieveMode, Param.m_BarcodeRetrieveMode, List);		

	CmpAndAddLogOperModif_INT(_T("Version Code Active Index"), NewParam.m_VersionCodeActiveIndex, Param.m_VersionCodeActiveIndex, List);

	//檢測拋件
	//unsigned int               m_DropOutPartFrameIndex;//影像唯一碼	
	CmpAndAddLogOperModif_BOL(_T("DropOut Part Enable"), NewParam.m_DropOutPartEnable, Param.m_DropOutPartEnable, List);
	CmpAndAddLogOperModif_BOL(_T("DropOut Part Save Image"), NewParam.m_DropOutPartSaveImage, Param.m_DropOutPartSaveImage, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Frame Unique ID"), NewParam.m_DropOutPartFrameUniqueID, Param.m_DropOutPartFrameUniqueID, List);
	CmpAndAddLogOperModif_BOL(_T("DropOut Part Match Use Scale"), NewParam.m_DropOutPartMatchUseScale, Param.m_DropOutPartMatchUseScale, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Over Low"), NewParam.m_DropOutPartOverLow, Param.m_DropOutPartOverLow, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Over High"), NewParam.m_DropOutPartOverHigh, Param.m_DropOutPartOverHigh, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Dark Level"), NewParam.m_DropOutPartDarkLevel, Param.m_DropOutPartDarkLevel, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Light Level"), NewParam.m_DropOutPartLightLevel, Param.m_DropOutPartLightLevel, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Tolerance Size"), NewParam.m_DropOutPartTolerance, Param.m_DropOutPartTolerance, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Smooth Size"), NewParam.m_DropOutPartSmoothSize, Param.m_DropOutPartSmoothSize, List);
	CmpAndAddLogOperModif_BOL(_T("DropOut Part Edge Remove"), NewParam.m_DropOutPartEdgeRemove, Param.m_DropOutPartEdgeRemove, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Dilate Size"), NewParam.m_DropOutPartDilateSize, Param.m_DropOutPartDilateSize, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Gaussian Size"), NewParam.m_DropOutPartGaussian, Param.m_DropOutPartGaussian, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Filter Open Size"), NewParam.m_DropOutPartFilterOpenSize, Param.m_DropOutPartFilterOpenSize, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Filter Close Size"), NewParam.m_DropOutPartFilterCloseSize, Param.m_DropOutPartFilterCloseSize, List);	
	CmpAndAddLogOperModif_INT(_T("DropOut Part Calc Size W"), NewParam.m_DropOutPartCalcSizeW, Param.m_DropOutPartCalcSizeW, List);
	CmpAndAddLogOperModif_INT(_T("DropOut Part Calc Size H"), NewParam.m_DropOutPartCalcSizeH, Param.m_DropOutPartCalcSizeH, List);
	CmpAndAddLogOperModif_DBL(_T("DropOut Part Min Size W"), NewParam.m_DropOutPartMinSizeW, Param.m_DropOutPartMinSizeW, List);
	CmpAndAddLogOperModif_DBL(_T("DropOut Part Min Size H"), NewParam.m_DropOutPartMinSizeH, Param.m_DropOutPartMinSizeH, List);
	CmpAndAddLogOperModif_DBL(_T("DropOut Part Min Size R"), NewParam.m_DropOutPartMaxSizeR, Param.m_DropOutPartMaxSizeR, List);
	//TNoiseFilterParam          m_DropOutPartSpaceNoiseFilter;//空間雜訊過濾處理
	CmpAndAddLogOperModif_BOL(_T("DropOut Part XBoard Excluded"), NewParam.m_DropOutPartXBoardExcluded, Param.m_DropOutPartXBoardExcluded, List);

	//檢測刮傷	
	//unsigned int               m_ScratchPartFrameIndex;//影像唯一碼
	CmpAndAddLogOperModif_BOL(_T("Scratch Part Enable"), NewParam.m_ScratchPartEnable, Param.m_ScratchPartEnable, List);
	CmpAndAddLogOperModif_BOL(_T("Scratch Part Save Image"), NewParam.m_ScratchPartSaveImage, Param.m_ScratchPartSaveImage, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Frame Unique ID"), NewParam.m_ScratchPartFrameUniqueID, Param.m_ScratchPartFrameUniqueID, List);
	CmpAndAddLogOperModif_BOL(_T("Scratch Part Match Use Scale"), NewParam.m_ScratchPartMatchUseScale, Param.m_ScratchPartMatchUseScale, List);
	CmpAndAddLogOperModif_DBL(_T("Scratch Part Calc Size W"), NewParam.m_ScratchPartCalcSizeW, Param.m_ScratchPartCalcSizeW, List);
	CmpAndAddLogOperModif_DBL(_T("Scratch Part Calc Size H"), NewParam.m_ScratchPartCalcSizeH, Param.m_ScratchPartCalcSizeH, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Color Expand"), NewParam.m_ScratchPartColorExpand, Param.m_ScratchPartColorExpand, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Filter Open Size"), NewParam.m_ScratchPartFilterOpenSize, Param.m_ScratchPartFilterOpenSize, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Filter Close Size"), NewParam.m_ScratchPartFilterCloseSize, Param.m_ScratchPartFilterCloseSize, List);
	CmpAndAddLogOperModif_DBL(_T("Scratch Part Min Size W"), NewParam.m_ScratchPartMinSizeW, Param.m_ScratchPartMinSizeW, List);
	CmpAndAddLogOperModif_DBL(_T("Scratch Part Min Size H"), NewParam.m_ScratchPartMinSizeH, Param.m_ScratchPartMinSizeH, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Smooth Size"), NewParam.m_ScratchPartSmoothSize, Param.m_ScratchPartSmoothSize, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Edge Threshold"), NewParam.m_ScratchPartEdgeThreshold, Param.m_ScratchPartEdgeThreshold, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Min Pixels"), NewParam.m_ScratchPartMinPixels, Param.m_ScratchPartMinPixels, List);
	CmpAndAddLogOperModif_DBL(_T("Scratch Part Min Size L"), NewParam.m_ScratchPartMinSizeD, Param.m_ScratchPartMinSizeD, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Min Grayscale"), NewParam.m_ScratchPartMinGrayscale, Param.m_ScratchPartMinGrayscale, List);
	CmpAndAddLogOperModif_INT(_T("Scratch Part Max Grayscale"), NewParam.m_ScratchPartMaxGrayscale, Param.m_ScratchPartMaxGrayscale, List);

	//檢測尺寸		
	//unsigned int               m_PartDimensionFrameIndex;//影像唯一碼	
	CmpAndAddLogOperModif_INT(_T("Part Dimension Frame UniqueID"), NewParam.m_PartDimensionFrameUniqueID, Param.m_PartDimensionFrameUniqueID, List);
	CmpAndAddLogOperModif_INT(_T("Part Dimension Over Low"), NewParam.m_PartDimensionOverLow, Param.m_PartDimensionOverLow, List);
	CmpAndAddLogOperModif_INT(_T("Part Dimension Over High"), NewParam.m_PartDimensionOverHigh, Param.m_PartDimensionOverHigh, List);
	CmpAndAddLogOperModif_INT(_T("Part Dimension Dilate Open Size"), NewParam.m_PartDimensionDilateSize, Param.m_PartDimensionDilateSize, List);
	CmpAndAddLogOperModif_INT(_T("Part Dimension Filter Open Size"), NewParam.m_PartDimensionFilterOpenSize, Param.m_PartDimensionFilterOpenSize, List);
	CmpAndAddLogOperModif_INT(_T("Part Dimension Filter Close Size"), NewParam.m_PartDimensionFilterCloseSize, Param.m_PartDimensionFilterCloseSize, List);
	CmpAndAddLogOperModif_DBL(_T("Part Dimension Max Size W"), NewParam.m_PartDimensionMinSizeW, Param.m_PartDimensionMinSizeW, List);
	CmpAndAddLogOperModif_DBL(_T("Part Dimension Max Size H"), NewParam.m_PartDimensionMinSizeH, Param.m_PartDimensionMinSizeH, List);
	CmpAndAddLogOperModif_DBL(_T("Part Dimension Max Size R"), NewParam.m_PartDimensionMaxSizeR, Param.m_PartDimensionMaxSizeR, List);
	//TNoiseFilterParam          m_PartDimensionSpaceNoiseFilter;//空間雜訊過濾處理

	size_t i=0;
	CString Content;		
	const size_t Count=List.size();
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	for ( i=0; i<Count; i++ )
	{
		const TLogOperModify &NodeRef=List[i];
		Content.Format(_T("%s Project [%s] %s [%s] %s [%s]"), sSet, NodeRef.m_sKey, sFrom, NodeRef.m_sOld, sTo, NodeRef.m_sNew);
		if ( AOIDataCollect.SaveLogOper_ProjectFunc(ShowName, Content) == false )		
		{	return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogPanelContent(CAOIPanel *PanelPtr, LPCTSTR Content)//儲存操作訊息-整板內容
{
	return AOIDataCollect.SaveLogOper_PanelFunc(PanelPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogPanelCompare(CAOIPanel *OldPtr, CAOIPanel *NewPtr)//儲存操作訊息-整板比較
{
	if ( CheckPanelPtr(OldPtr) == false ) { return true; }
	if ( CheckPanelPtr(NewPtr) == false ) { return true; }

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	
	
	//TB_SYSTEM_ID               m_PanelSystemID;//整板的系統編號
	//unsigned int               m_PanelIndex_Project;//整板的引數編號

	sKey = _T("Deleted");	
	ChangeNode.SetBol(sKey, OldPtr->GetPanelDeleted(), NewPtr->GetPanelDeleted(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Bypassed");	
	ChangeNode.SetBol(sKey, OldPtr->GetPanelBypassed(), NewPtr->GetPanelBypassed(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Barcode Enabled");	
	ChangeNode.SetBol(sKey, OldPtr->GetPanelBarcodeEnabled(), NewPtr->GetPanelBarcodeEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Panel Type");	
	ChangeNode.SetInt(sKey, OldPtr->GetPanelType(), NewPtr->GetPanelType(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetPanelBoardFdGrabMode() != NewPtr->GetPanelBoardFdGrabMode() )
	{
		sKey = _T("Board Fd Grab Mode");
		sOld = AOIDataDefine.GetBoardFdGrabModeText(OldPtr->GetPanelBoardFdGrabMode());
		sNew = AOIDataDefine.GetBoardFdGrabModeText(NewPtr->GetPanelBoardFdGrabMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Barcode Device Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetPanelBarcodeDeviceIndex(), NewPtr->GetPanelBarcodeDeviceIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Barcode Device Code Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetPanelBarcodeDeviceCodeIndex(), NewPtr->GetPanelBarcodeDeviceCodeIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);		
	
	ChangeCount=ChangeList.size();	
	CString sOper=AOIDataDefine.GetSetText();
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogPanelOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogPanelOperate(CAOIPanel *PanelPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-整板修改		
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogPanelContent(PanelPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBoardContent(CAOIBoard *BoardPtr, LPCTSTR Content)//儲存操作訊息-單板內容
{
	return AOIDataCollect.SaveLogOper_BoardFunc(BoardPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBoardCompare(CAOIBoard *OldPtr, CAOIBoard *NewPtr)//儲存操作訊息-單板比較
{
	if ( CheckBoardPtr(OldPtr) == false ) { return true; }
	if ( CheckBoardPtr(NewPtr) == false ) { return true; }

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	
	
	//unsigned int               m_BoardIndex_Project;//單板在專案的引數編號
	sKey = _T("Panel Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetBoardPanelIndex_Project()+1, NewPtr->GetBoardPanelIndex_Project()+1, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Board Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetBoardIndex_Panel()+1, NewPtr->GetBoardIndex_Panel()+1, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Deleted");	
	ChangeNode.SetBol(sKey, OldPtr->GetBoardDeleted(), NewPtr->GetBoardDeleted(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Bypassed");	
	ChangeNode.SetBol(sKey, OldPtr->GetBoardBypassed(), NewPtr->GetBoardBypassed(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Rotate Angle");	
	ChangeNode.SetDbl(sKey, OldPtr->GetBoardRoatedAngle(), NewPtr->GetBoardRoatedAngle(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Barcode Enabled");	
	ChangeNode.SetBol(sKey, OldPtr->GetBoardBarcodeEnabled(), NewPtr->GetBoardBarcodeEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Board Type");	
	ChangeNode.SetInt(sKey, OldPtr->GetBoardType(), NewPtr->GetBoardType(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Side Mode");	
	ChangeNode.SetInt(sKey, OldPtr->GetBoardSideMode(), NewPtr->GetBoardSideMode(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetBoardBoardFdGrabMode() != NewPtr->GetBoardBoardFdGrabMode() )
	{
		sKey = _T("Board Fd Grab Mode");
		sOld = AOIDataDefine.GetBoardFdGrabModeText(OldPtr->GetBoardBoardFdGrabMode());
		sNew = AOIDataDefine.GetBoardFdGrabModeText(NewPtr->GetBoardBoardFdGrabMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Orientation Mode");	
	ChangeNode.SetInt(sKey, OldPtr->GetBoardOrientationMode(), NewPtr->GetBoardOrientationMode(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Barcode Device Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetBoardBarcodeDeviceIndex(), NewPtr->GetBoardBarcodeDeviceIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Barcode Device Code Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetBoardBarcodeDeviceCodeIndex(), NewPtr->GetBoardBarcodeDeviceCodeIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);		
	
	ChangeCount=ChangeList.size();	
	CString sOper=AOIDataDefine.GetSetText();
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogBoardOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBoardOperate(CAOIBoard *BoardPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-單板修改
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogBoardContent(BoardPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogFdContent(CAOIFd *FdPtr, LPCTSTR Content)//儲存操作訊息-定位點內容
{
	return AOIDataCollect.SaveLogOper_FdFunc(FdPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogFdCompare(CAOIFd *OldPtr, CAOIFd *NewPtr)//儲存操作訊息-定位點比較
{
	if ( CheckFdPtr(OldPtr) == false ) { return true; }
	if ( CheckFdPtr(NewPtr) == false ) { return true; }

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;		

	sKey = _T("Deleted");	
	ChangeNode.SetBol(sKey, OldPtr->GetFdDeleted(), NewPtr->GetFdDeleted(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Group ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetFdGroupID(), NewPtr->GetFdGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Unique ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetFdUniqueID(), NewPtr->GetFdUniqueID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Sort ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetFdSortID(), NewPtr->GetFdSortID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Extend Size X");	
	ChangeNode.SetInt(sKey, OldPtr->GetFdPatExtendSize().cx, NewPtr->GetFdPatExtendSize().cx, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Extend Size Y");	
	ChangeNode.SetInt(sKey, OldPtr->GetFdPatExtendSize().cy, NewPtr->GetFdPatExtendSize().cy, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	
	//double                     m_FdMinScore;//定位點最低相似度
	//CAOIModel                  m_FdModel;  //定位點模組	
	ChangeCount=ChangeList.size();	
	CString sOper = AOIDataDefine.GetSetText();	
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogFdOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogFdOperate(CAOIFd *FdPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-定位點修改
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogFdContent(FdPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogMarkContent(CAOIMark *MarkPtr, LPCTSTR Content)//儲存操作訊息-特徵內容
{
	return AOIDataCollect.SaveLogOper_MarkFunc(MarkPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogMarkCompare(CAOIMark *OldPtr, CAOIMark *NewPtr)//儲存操作訊息-特徵比較
{
	if ( CheckMarkPtr(OldPtr) == false ) { return true; }
	if ( CheckMarkPtr(NewPtr) == false ) { return true; }

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;		

	sKey = _T("Deleted");	
	ChangeNode.SetBol(sKey, OldPtr->GetMarkDeleted(), NewPtr->GetMarkDeleted(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Group ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetMarkGroupID(), NewPtr->GetMarkGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Unique ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetMarkUniqueID(), NewPtr->GetMarkUniqueID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey = _T("Mark Type Mode");	
	ChangeNode.SetInt(sKey, OldPtr->GetMarkTypeMode(), NewPtr->GetMarkTypeMode(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	//CAOIModel                  m_MarkModel;	
	ChangeCount=ChangeList.size();	
	CString sOper=AOIDataDefine.GetSetText();
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogMarkOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogMarkOperate(CAOIMark *MarkPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal)//儲存操作訊息-特徵操作	
{
	CString Content;
	FormatContent(sOper, sKey, nVal, Content);
	return SaveLogMarkContent(MarkPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogMarkOperate(CAOIMark *MarkPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal)//儲存操作訊息-特徵操作	
{
	CString Content;
	FormatContent(sOper, sKey, fVal, Content);
	return SaveLogMarkContent(MarkPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogMarkOperate(CAOIMark *MarkPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal)//儲存操作訊息-特徵操作	
{
	CString Content;
	FormatContent(sOper, sKey, sVal, Content);
	return SaveLogMarkContent(MarkPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogMarkOperate(CAOIMark *MarkPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-特徵操作	
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogMarkContent(MarkPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBarcodeContent(CAOIBarcode *BarcodePtr, LPCTSTR Content)//儲存操作訊息-條碼內容
{
	return AOIDataCollect.SaveLogOper_BarcodeFunc(BarcodePtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBarcodeCompare(CAOIBarcode *OldPtr, CAOIBarcode *NewPtr)//儲存操作訊息-條碼比較
{
	if ( CheckBarcodePtr(OldPtr) == false ) { return true; }
	if ( CheckBarcodePtr(NewPtr) == false ) { return true; }

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;		

	sKey = _T("Deleted");	
	ChangeNode.SetBol(sKey, OldPtr->GetBarcodeDeleted(), NewPtr->GetBarcodeDeleted(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Group ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetBarcodeGroupID(), NewPtr->GetBarcodeGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Unique ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetBarcodeUniqueID(), NewPtr->GetBarcodeUniqueID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	//CAOIModel                  m_BarcodeModel;	
	
	ChangeCount=ChangeList.size();	
	CString sOper=AOIDataDefine.GetSetText();
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogBarcodeOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBarcodeOperate(CAOIBarcode *BarcodePtr, LPCTSTR sOper, LPCTSTR sKey, int nVal)//儲存操作訊息-條碼操作	
{
	CString Content;
	FormatContent(sOper, sKey, nVal, Content);
	return SaveLogBarcodeContent(BarcodePtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBarcodeOperate(CAOIBarcode *BarcodePtr, LPCTSTR sOper, LPCTSTR sKey, double fVal)//儲存操作訊息-條碼操作	
{
	CString Content;
	FormatContent(sOper, sKey, fVal, Content);
	return SaveLogBarcodeContent(BarcodePtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBarcodeOperate(CAOIBarcode *BarcodePtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal)//儲存操作訊息-條碼操作	
{
	CString Content;
	FormatContent(sOper, sKey, sVal, Content);
	return SaveLogBarcodeContent(BarcodePtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBarcodeOperate(CAOIBarcode *BarcodePtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-條碼操作	
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogBarcodeContent(BarcodePtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogComponentContent(CAOIComponent *ComponentPtr, LPCTSTR Content)//儲存操作訊息-零件內容
{
	return AOIDataCollect.SaveLogOper_ComponentFunc(ComponentPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogComponentCompare(CAOIComponent *OldPtr, CAOIComponent *NewPtr)//儲存操作訊息-零件比較
{
	if ( CheckComponentPtr(OldPtr) == false ) { return true; }
	if ( CheckComponentPtr(NewPtr) == false ) { return true; }

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;		

	sKey = _T("Deleted");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentDeleted(), NewPtr->GetComponentDeleted(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	//if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_PANEL_INDEX, pComponent->GetComponentPanelIndex_Project()) == false ) { return false; }
	//if ( FileIO.SaveChunk_INT(FILE_IO_COMPONENT_BOARD_INDEX, pComponent->GetComponentBoardIndex_Project()) == false ) { return false; }

	sKey = _T("Unique ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentUniqueID(), NewPtr->GetComponentUniqueID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Name");	
	ChangeNode.SetStr(sKey, OldPtr->GetComponentName(), NewPtr->GetComponentName(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Component Type");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentType(), NewPtr->GetComponentType(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Model Name");	
	ChangeNode.SetStr(sKey, OldPtr->GetComponentModelName(), NewPtr->GetComponentModelName(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Part Number");	
	ChangeNode.SetStr(sKey, OldPtr->GetComponentPartNumber(), NewPtr->GetComponentPartNumber(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Nozzle Name");	
	ChangeNode.SetStr(sKey, OldPtr->GetComponentNozzleName(), NewPtr->GetComponentNozzleName(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Angle");	
	ChangeNode.SetDbl(sKey, OldPtr->GetComponentAngle(), NewPtr->GetComponentAngle(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Pos X");	
	ChangeNode.SetDbl(sKey, OldPtr->GetComponentCadPosX(), NewPtr->GetComponentCadPosX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Pos Y");	
	ChangeNode.SetDbl(sKey, OldPtr->GetComponentCadPosY(), NewPtr->GetComponentCadPosY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	//if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ORG_CAD_POS_X, pComponent->GetComponentOrgCadPosX()) == false ) { return false; }
	//if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ORG_CAD_POS_Y, pComponent->GetComponentOrgCadPosY()) == false ) { return false; }	

	sKey = _T("Bypassed");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentBypassed(), NewPtr->GetComponentBypassed(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("XBoard Unit");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentXBoardUnit(), NewPtr->GetComponentXBoardUnit(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Model Isolated");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentModelIsolated(), NewPtr->GetComponentModelIsolated(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Bypass 3D");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentBypass3D(), NewPtr->GetComponentBypass3D(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Frame Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentFrameIndex(), NewPtr->GetComponentFrameIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Field Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentFieldIndex(), NewPtr->GetComponentFieldIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Mask Base Enabled");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentMaskEnable_Base(), NewPtr->GetComponentMaskEnable_Base(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Mask Base Frame Unique ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentMaskFrameUniqueID_Base(), NewPtr->GetComponentMaskFrameUniqueID_Base(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Mask Color Group Link Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentMaskColorGroupLinkIndex(), NewPtr->GetComponentMaskColorGroupLinkIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Mask Extend Body X");	
	ChangeNode.SetDbl(sKey, OldPtr->GetComponentMaskExtendW_Body(), NewPtr->GetComponentMaskExtendW_Body(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Mask Extend Body Y");	
	ChangeNode.SetDbl(sKey, OldPtr->GetComponentMaskExtendH_Body(), NewPtr->GetComponentMaskExtendH_Body(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Mask Extend Land X");	
	ChangeNode.SetDbl(sKey, OldPtr->GetComponentMaskExtendW_Land(), NewPtr->GetComponentMaskExtendW_Land(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Mask Extend Land Y");	
	ChangeNode.SetDbl(sKey, OldPtr->GetComponentMaskExtendH_Land(), NewPtr->GetComponentMaskExtendH_Land(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Field Self Enabled");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentSelfFieldEnabled(), NewPtr->GetComponentSelfFieldEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("District ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentDistrictID(), NewPtr->GetComponentDistrictID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	/*	
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_BODY_SIZE_CX, pComponent->GetComponentBodySizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_BODY_SIZE_CY, pComponent->GetComponentBodySizeH()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ROI_SIZE_CX, pComponent->GetComponentRoiSizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_COMPONENT_ROI_SIZE_CY, pComponent->GetComponentRoiSizeH()) == false ) { return false; }	
	*/

	sKey = _T("Alarm Enabled");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentEnableAlarm(), NewPtr->GetComponentEnableAlarm(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Group ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentGroupID(), NewPtr->GetComponentGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Group ORG");	
	ChangeNode.SetBol(sKey, OldPtr->GetComponentGroupOrg(), NewPtr->GetComponentGroupOrg(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Base Plane ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentLocalBasePlaneID(), NewPtr->GetComponentLocalBasePlaneID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Class ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetComponentModelClassID(), NewPtr->GetComponentModelClassID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	if ( ChangeNode.ChkBol(OldPtr->GetComponentDefectAlarmEnableOnAOI(), NewPtr->GetComponentDefectAlarmEnableOnAOI(), dwData) == false )
	{	AddLogOperModified_Key(_T("Defect Alarm Enabled On AOI"), ChangeNode, ChangeList);	}

	if ( ChangeNode.ChkBol(OldPtr->GetComponentDefectAlarmEnableOnARS(), NewPtr->GetComponentDefectAlarmEnableOnARS(), dwData) == false )
	{	AddLogOperModified_Key(_T("Defect Alarm Enabled On ARS"), ChangeNode, ChangeList);	}

	if ( ChangeNode.ChkBol(OldPtr->GetComponentTotalNGCountEnable(), NewPtr->GetComponentTotalNGCountEnable(), dwData) == false )
	{	AddLogOperModified_Key(_T("Total NG Count Alarm Enabled"), ChangeNode, ChangeList);	}

	if ( ChangeNode.ChkBol(OldPtr->GetComponentContinueNGCountEnable(), NewPtr->GetComponentContinueNGCountEnable(), dwData) == false )
	{	AddLogOperModified_Key(_T("Continue NG Count Alarm Enabled"), ChangeNode, ChangeList);	}	

	if ( ChangeNode.ChkInt(OldPtr->GetComponentTotalNGCountLimit(), NewPtr->GetComponentTotalNGCountLimit(), dwData) == false )
	{	AddLogOperModified_Key(_T("Total NG Count Alarm Limit"), ChangeNode, ChangeList);	}

	if ( ChangeNode.ChkInt(OldPtr->GetComponentContinueNGCountLimit(), NewPtr->GetComponentContinueNGCountLimit(), dwData) == false )
	{	AddLogOperModified_Key(_T("Continue NG Count Alarm Limit"), ChangeNode, ChangeList);	}	

	if ( OldPtr->GetComponentSaveTestImageMode() != NewPtr->GetComponentSaveTestImageMode() )
	{
		sKey = _T("Save Test Image Mode");	
		sOld = AOIDataDefine.GetSaveTestImageModeText(OldPtr->GetComponentSaveTestImageMode());
		sNew = AOIDataDefine.GetSaveTestImageModeText(NewPtr->GetComponentSaveTestImageMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}
	
	ChangeCount=ChangeList.size();	
	CString sOper=AOIDataDefine.GetSetText();
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogComponentOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogComponentOperate(CAOIComponent *ComponentPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal)//儲存操作訊息-零件操作
{
	CString Content;
	FormatContent(sOper, sKey, nVal, Content);
	return SaveLogComponentContent(ComponentPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogComponentOperate(CAOIComponent *ComponentPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal)//儲存操作訊息-零件操作
{
	CString Content;
	FormatContent(sOper, sKey, fVal, Content);
	return SaveLogComponentContent(ComponentPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogComponentOperate(CAOIComponent *ComponentPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal)//儲存操作訊息-零件操作		
{
	CString Content;
	FormatContent(sOper, sKey, sVal, Content);
	return SaveLogComponentContent(ComponentPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogComponentOperate(CAOIComponent *ComponentPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-零件修改		
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogComponentContent(ComponentPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogPartGroupContent(CAOIPartGroup *PartGroupPtr, LPCTSTR Content)//儲存操作訊息-元件群組內容
{
	return AOIDataCollect.SaveLogOper_PartGroupFunc(PartGroupPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogPartGroupCompare(CAOIPartGroup *OldPtr, CAOIPartGroup *NewPtr)//儲存操作訊息-元件群組比較
{
	if ( NULL == OldPtr ) { return false; }
	if ( NULL == NewPtr ) { return false; }	

	size_t i=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;	
	std::vector<TLogOperModify> ChangeList;

	//TB_SYSTEM_ID               m_PartGroupSystemID;//零件群組的系統編號
	if ( OldPtr->GetPartGroupMode() != NewPtr->GetPartGroupMode() )
	{
		sKey = _T("Group Mode");
		sOld = AOIDataDefine.GetAOIGroupModeText(OldPtr->GetPartGroupMode());
		sNew = AOIDataDefine.GetAOIGroupModeText(NewPtr->GetPartGroupMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);	
	}

	sKey = _T("Group Name");
	ChangeNode.SetStr(sKey, OldPtr->GetPartGroupName(), NewPtr->GetPartGroupName(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Unique ID");
	ChangeNode.SetInt(sKey, OldPtr->GetPartGroupUniqueID(), NewPtr->GetPartGroupUniqueID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Group ID");
	ChangeNode.SetInt(sKey, OldPtr->GetPartGroupGroupID_UI(), NewPtr->GetPartGroupGroupID_UI(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Deleted");
	ChangeNode.SetBol(sKey, OldPtr->GetPartGroupDeleted(), NewPtr->GetPartGroupDeleted(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	
	if ( OldPtr->GetColinearityMode() != NewPtr->GetColinearityMode() )
	{
		sKey = _T("Colinearity Mode");
		sOld = AOIDataDefine.GetPartGroupColinearityeModeText(OldPtr->GetColinearityMode());
		sNew = AOIDataDefine.GetPartGroupColinearityeModeText(NewPtr->GetColinearityMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);	
	}

	if ( OldPtr->GetColinearityTargetMode() != NewPtr->GetColinearityTargetMode() )
	{
		sKey = _T("Colinearity Target Mode");
		sOld = AOIDataDefine.GetPartGroupTargetModeText(OldPtr->GetColinearityTargetMode());
		sNew = AOIDataDefine.GetPartGroupTargetModeText(NewPtr->GetColinearityTargetMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);	
	}

	sKey = _T("Colinearity Gap USL");
	ChangeNode.SetDbl(sKey, OldPtr->GetColinearityGapUSL(), NewPtr->GetColinearityGapUSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Colinearity Gap LSL");
	ChangeNode.SetDbl(sKey, OldPtr->GetColinearityGapLSL(), NewPtr->GetColinearityGapLSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey = _T("Distance Gap Enabled X");
	ChangeNode.SetBol(sKey, OldPtr->GetDistanceGapEnbX(), NewPtr->GetDistanceGapEnbX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap Enabled Y");
	ChangeNode.SetBol(sKey, OldPtr->GetDistanceGapEnbY(), NewPtr->GetDistanceGapEnbY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap Enabled L");
	ChangeNode.SetBol(sKey, OldPtr->GetDistanceGapEnbL(), NewPtr->GetDistanceGapEnbL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap USL X");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapUSLX(), NewPtr->GetDistanceGapUSLX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap LSL X");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapLSLX(), NewPtr->GetDistanceGapLSLX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap USL Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapUSLY(), NewPtr->GetDistanceGapUSLY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap LSL Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapLSLY(), NewPtr->GetDistanceGapLSLY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap USL L");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapUSLL(), NewPtr->GetDistanceGapUSLL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap LSL L");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapLSLL(), NewPtr->GetDistanceGapLSLL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap Std Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetDistanceGapStdEnb(), NewPtr->GetDistanceGapStdEnb(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap Std X");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapStdX(), NewPtr->GetDistanceGapStdX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap Std Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapStdY(), NewPtr->GetDistanceGapStdY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap Add X");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapAddX(), NewPtr->GetDistanceGapAddX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap Add Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapAddY(), NewPtr->GetDistanceGapAddY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap ABS Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetDistanceGapEnbAbs(), NewPtr->GetDistanceGapEnbAbs(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Distance Gap Scale X");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapScaleX(), NewPtr->GetDistanceGapScaleX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	sKey = _T("Distance Gap Scale Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapScaleY(), NewPtr->GetDistanceGapScaleY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	sKey = _T("Distance Gap Scale L");
	ChangeNode.SetDbl(sKey, OldPtr->GetDistanceGapScaleL(), NewPtr->GetDistanceGapScaleL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	ChangeCount=ChangeList.size();	
	if ( SaveLogPartGroupNodeCompare(OldPtr->GetPartGroupNodePtr1(), NewPtr->GetPartGroupNodePtr1(), ChangeList) == false )
	{	return false; }
	if ( ChangeList.size() != ChangeCount )
	{
		for ( i=ChangeCount; i<ChangeList.size(); i++ )
		{
			sOld = ChangeList[i].GetKey();
			sKey.Format(_T("%s %s"), _T("Node 1"), sOld);
			ChangeList[i].SetKey(sKey);
		}
	}

	ChangeCount=ChangeList.size();	
	if ( SaveLogPartGroupNodeCompare(OldPtr->GetPartGroupNodePtr2(), NewPtr->GetPartGroupNodePtr2(), ChangeList) == false )
	{	return false; }
	if ( ChangeList.size() != ChangeCount )
	{
		for ( i=ChangeCount; i<ChangeList.size(); i++ )
		{
			sOld = ChangeList[i].GetKey();
			sKey.Format(_T("%s %s"), _T("Node 2"), sOld);
			ChangeList[i].SetKey(sKey);
		}
	}

	ChangeCount=ChangeList.size();	
	if ( SaveLogPartGroupNodeCompare(OldPtr->GetPartGroupNodePtr3(), NewPtr->GetPartGroupNodePtr3(), ChangeList) == false )
	{	return false; }
	if ( ChangeList.size() != ChangeCount )
	{
		for ( i=ChangeCount; i<ChangeList.size(); i++ )
		{
			sOld = ChangeList[i].GetKey();
			sKey.Format(_T("%s %s"), _T("Node 3"), sOld);
			ChangeList[i].SetKey(sKey);
		}
	}

	ChangeCount=ChangeList.size();	
	if ( SaveLogPartGroupNodeCompare(OldPtr->GetPartGroupNodePtr4(), NewPtr->GetPartGroupNodePtr4(), ChangeList) == false )
	{	return false; }
	if ( ChangeList.size() != ChangeCount )
	{
		for ( i=ChangeCount; i<ChangeList.size(); i++ )
		{
			sOld = ChangeList[i].GetKey();
			sKey.Format(_T("%s %s"), _T("Node 4"), sOld);
			ChangeList[i].SetKey(sKey);
		}
	}

	
	ChangeCount=ChangeList.size();	
	CString sOper=AOIDataDefine.GetSetText();
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogPartGroupOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogPartGroupOperate(CAOIPartGroup *PartGroupPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-元件群組修改
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogPartGroupContent(PartGroupPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogPartGroupNodeCompare(TPartGroupNode *OldPtr, TPartGroupNode *NewPtr, std::vector<TLogOperModify> &ChangeList)//儲存操作訊息-元件群組比較
{
	if ( NULL == OldPtr ) { return false; }
	if ( NULL == NewPtr ) { return false; }

	CString sKey;
	CString sOld;
	CString sNew;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	if ( OldPtr->ComponentPtr != NewPtr->ComponentPtr )
	{
		sKey = _T("Component");
		if ( NULL == OldPtr->ComponentPtr ) 
		{	sOld = AOIDataDefine.GetDisableText(); }
		else
		{	sOld = OldPtr->ComponentPtr->GetComponentFullName(); }

		if ( NULL == NewPtr->ComponentPtr ) 
		{	sOld = AOIDataDefine.GetDisableText(); }
		else
		{	sOld = NewPtr->ComponentPtr->GetComponentFullName(); }
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogBoxCompare(CAOIBox *OldPtr, CAOIBox *NewPtr, std::vector<TLogOperModify> &ChangeList)//儲存操作訊息模組-框比較
{
	if ( CheckBoxPtr(NewPtr) == false ) { return false; }
	if ( CheckBoxPtr(OldPtr) == false ) { return false; }

	CString sKey;
	CString sOld;
	CString sNew;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;	
	
	if ( OldPtr->GetBoxToward() != NewPtr->GetBoxToward() )
	{
		sKey = _T("Toward");
		sOld = AOIDataDefine.GetBoxTowardText(OldPtr->GetBoxToward());
		sNew = AOIDataDefine.GetBoxTowardText(NewPtr->GetBoxToward());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);	
	}

	if ( OldPtr->GetBoxShapeMode() != NewPtr->GetBoxShapeMode() )
	{
		sKey = _T("Shape Mode");
		sOld = AOIDataDefine.GetBoxShapeModeText(OldPtr->GetBoxShapeMode());
		sNew = AOIDataDefine.GetBoxShapeModeText(NewPtr->GetBoxShapeMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);	
	}

	sKey = _T("Shape Param");
	ChangeNode.SetDbl(sKey, OldPtr->GetBoxShapeParam(), NewPtr->GetBoxShapeParam(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Shape Param 2");
	ChangeNode.SetDbl(sKey, OldPtr->GetBoxShapeParam2(), NewPtr->GetBoxShapeParam2(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetBoxEnabled(), NewPtr->GetBoxEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Angle");
	ChangeNode.SetDbl(sKey, OldPtr->GetBoxAngle(), NewPtr->GetBoxAngle(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Pos X");
	ChangeNode.SetDbl(sKey, OldPtr->GetBoxPosX(), NewPtr->GetBoxPosX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Pos Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetBoxPosY(), NewPtr->GetBoxPosY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Size X");
	ChangeNode.SetDbl(sKey, OldPtr->GetBoxSizeX(), NewPtr->GetBoxSizeX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Size Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetBoxSizeY(), NewPtr->GetBoxSizeY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	
	//CAD
	//TPOINT2D                   m_BoxPosCad;                  //框座標-Cad內	
	//Stage
	//TPOINT2D                   m_BoxPosStage;                //框座標-機台內	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelContent(CAOIModel *ModelPtr, LPCTSTR Content)//儲存操作訊息模組-框座標
{
	return AOIDataCollect.SaveLogOper_ModelFunc(ModelPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelCompare(CAOIModel *OldPtr, CAOIModel *NewPtr)//儲存操作訊息模組-模組比較
{
	if ( CheckModelPtr(NewPtr) == false ) { return false; }
	if ( CheckModelPtr(OldPtr) == false ) { return false; }

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	
	
	if ( OldPtr->GetModelType() != NewPtr->GetModelType() )
	{
		sKey = _T("Model Type");
		sOld = AOIDataDefine.GetModelTypeText(OldPtr->GetModelType());
		sNew = AOIDataDefine.GetModelTypeText(NewPtr->GetModelType());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Model Name");
	sOld = OldPtr->GetModelName();
	sNew = NewPtr->GetModelName();
	ChangeNode.SetStr(sKey, sOld, sNew, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Group Name");
	sOld = OldPtr->GetModelGroupName();
	sNew = NewPtr->GetModelGroupName();
	ChangeNode.SetStr(sKey, sOld, sNew, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	//TB_SYSTEM_ID               m_ModelSystemID;              //模組系統編號

	sKey = _T("Chip Size Mode");	
	ChangeNode.SetInt(sKey, OldPtr->GetModelChipSizeMode(), NewPtr->GetModelChipSizeMode(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Land Direction");	
	ChangeNode.SetInt(sKey, OldPtr->GetModelLandDirection(), NewPtr->GetModelLandDirection(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Pad Adjust Mode");	
	ChangeNode.SetInt(sKey, OldPtr->GetModelPadAdjustMode(), NewPtr->GetModelPadAdjustMode(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Lead Adjust Mode");	
	ChangeNode.SetInt(sKey, OldPtr->GetModelLeadAdjustMode(), NewPtr->GetModelLeadAdjustMode(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);		

	sKey = _T("Class ID");	
	ChangeNode.SetInt(sKey, OldPtr->GetModelClassID(), NewPtr->GetModelClassID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Isolated");	
	ChangeNode.SetBol(sKey, OldPtr->GetModelIsolated(), NewPtr->GetModelIsolated(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Wnd Link Mode");	
	ChangeNode.SetInt(sKey, OldPtr->GetModelWndLinkMode(), NewPtr->GetModelWndLinkMode(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	//CWndDefectItem             m_ModelDefectItemTest;        //模組瑕疵檢測項目模式
	//CWndDefectItem             m_ModelDefectItemAlarm;       //模組瑕疵警報項目模式		
	//CWndDefectItem             m_ModelDefectItemRecheck_ARS; //模組瑕疵重複確認項目-ARS

	sKey = _T("Extend Auto Adjust");	
	ChangeNode.SetBol(sKey, OldPtr->GetModelExtendAutoAdjust(), NewPtr->GetModelExtendAutoAdjust(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Extend Range X");	
	ChangeNode.SetDbl(sKey, OldPtr->GetModelExtendRange().cx, NewPtr->GetModelExtendRange().cx, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Extend Range Y");	
	ChangeNode.SetDbl(sKey, OldPtr->GetModelExtendRange().cy, NewPtr->GetModelExtendRange().cy, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	
	if ( SaveLogBoxCompare(OldPtr->GetModelBodyBoxPtr(), NewPtr->GetModelBodyBoxPtr(), ChangeList) == false )
	{	return false; }

	sKey = _T("Body Size X");	
	ChangeNode.SetDbl(sKey, OldPtr->GetModelBodySizeX(), NewPtr->GetModelBodySizeX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Body Size Y");	
	ChangeNode.SetDbl(sKey, OldPtr->GetModelBodySizeY(), NewPtr->GetModelBodySizeY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey = _T("Body Height");	
	ChangeNode.SetDbl(sKey, OldPtr->GetModelBodyHeight(), NewPtr->GetModelBodyHeight(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	//CColorGroup                m_ModelBodyColorGroup;        //模組本體顏色		
	
	ChangeCount=ChangeList.size();	
	CString sOper=AOIDataDefine.GetSetText();
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogModelOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelOperate(CAOIModel *ModelPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal)//儲存操作訊息-模組操作
{
	CString Content;
	FormatContent(sOper, sKey, nVal, Content);
	return SaveLogModelContent(ModelPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelOperate(CAOIModel *ModelPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal)//儲存操作訊息-模組操作
{
	CString Content;
	FormatContent(sOper, sKey, fVal, Content);
	return SaveLogModelContent(ModelPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelOperate(CAOIModel *ModelPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal)//儲存操作訊息-模組操作		
{
	CString Content;
	FormatContent(sOper, sKey, sVal, Content);
	return SaveLogModelContent(ModelPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelOperate(CAOIModel *ModelPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-模組修改		
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogModelContent(ModelPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndDefectItemCompare(CAOIModel *ModelPtr, LPCTSTR Name, const CWndDefectItem &Old, const CWndDefectItem &New)//儲存操作訊息模組-檢測框瑕疵數比較		
{	
	if ( CheckModelPtr(ModelPtr) == false ) { return false; }

	size_t  i=0;	
	CString sKey;
	size_t  ChangeCount=0;	
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;			
	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		WND_DEFECT_ID WndDefectID=List[i];
		const int OldValue=Old.GetItemCount(WndDefectID);
		const int NewValue=New.GetItemCount(WndDefectID);
		if ( OldValue != NewValue )		
		{	
			ChangeNode.SetOld(AOIDataDefine.GetWndDefectItemModeText(OldValue));
			ChangeNode.SetNew(AOIDataDefine.GetWndDefectItemModeText(NewValue));			
			AddLogOperModified_Key(AOIDataDefine.GetWndDefectIDText(WndDefectID), ChangeNode, ChangeList);	
			if ( NULL != Name && ChangeList.size()>0 )
			{
				TLogOperModify &ChangeNodeRef=ChangeList[ChangeList.size()-1];		
				sKey.Format(_T("%s %s"), Name, ChangeNodeRef.GetKey());
				ChangeNodeRef.SetKey(sKey);		
			}
		}	
	}		

	CString sOper;
	ChangeCount=ChangeList.size();	
	sOper.Format(_T("%s %s"), AOIDataDefine.GetSetText(), AOIDataDefine.GetModelText());
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogModelOperate(ModelPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandContent(CAOILand *LandPtr, LPCTSTR Content)//儲存操作訊息模組-特徵框內容
{
	return AOIDataCollect.SaveLogOper_LandFunc(LandPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandCompare(CAOILand *OldPtr, CAOILand *NewPtr)//儲存操作訊息模組-特徵框-比較
{
	if ( CheckLandPtr(NewPtr) == false ) { return false; }
	if ( CheckLandPtr(OldPtr) == false ) { return false; }

	size_t  i=0, j=0;
	CString sKey;
	CString sOld;
	CString sNew;
	size_t  ChangeCount=0;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	
	
	if ( OldPtr->GetLandType() != NewPtr->GetLandType() )
	{
		sKey = _T("Land Type");
		sOld = AOIDataDefine.GetLandTypeText(OldPtr->GetLandType());
		sNew = AOIDataDefine.GetLandTypeText(NewPtr->GetLandType());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Group ID");
	ChangeNode.SetInt(sKey, OldPtr->GetLandGroupID(), NewPtr->GetLandGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Align ID");
	ChangeNode.SetInt(sKey, OldPtr->GetLandAlignID(), NewPtr->GetLandAlignID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Align ID");
	ChangeNode.SetInt(sKey, OldPtr->GetLandAlignID(), NewPtr->GetLandAlignID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Lead Size X");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadSizeX(), NewPtr->GetLandLeadSizeX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Lead Size Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadSizeY(), NewPtr->GetLandLeadSizeY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Lead Height");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadHeight(), NewPtr->GetLandLeadHeight(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey = _T("Lead Tip Size X");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadTipSizeX(), NewPtr->GetLandLeadTipSizeX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Lead Tip Size Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadTipSizeY(), NewPtr->GetLandLeadTipSizeY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Lead Tip Height");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadTipHeight(), NewPtr->GetLandLeadTipHeight(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey = _T("Lead Shoulder Size X");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadShoulderSizeX(), NewPtr->GetLandLeadShoulderSizeX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Lead Shoulder Size Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadShoulderSizeY(), NewPtr->GetLandLeadShoulderSizeY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Lead Shoulder Height");
	ChangeNode.SetDbl(sKey, OldPtr->GetLandLeadShoulderHeight(), NewPtr->GetLandLeadShoulderHeight(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	//bool                       m_LandIncludePadAlign;        //特徵框是否加入焊盤定位
	//bool                       m_LandIncludePartAlign;       //特徵框是否加入零件定位	

	
	int LandBasicID = 0;
	for ( j=0; j<5; j++ )
	{
		LandBasicID = (int)(j+1);
		switch ( LandBasicID )
		{
		case LAND_BOX_PAD: sNew = _T("Pad");	break;
		case LAND_BOX_LEAD: sNew = _T("Lead");	break;
		case LAND_BOX_LEAD_TIP: sNew = _T("Lead Tip");	break;
		case LAND_BOX_LEAD_SHOULDER: sNew = _T("Lead Shoulder");	break;
		case LAND_BOX_BODY_EDGE: sNew = _T("Body Edge");	break;
		default:
			sNew = _T("");
			break;
		}
		ChangeCount = ChangeList.size();		
		SaveLogBoxCompare(OldPtr->GetLandBasicBoxPtr(LandBasicID), OldPtr->GetLandBasicBoxPtr(LandBasicID), ChangeList);
		if ( ChangeCount != ChangeList.size() > 0 ) 
		{
			for ( size_t i=ChangeCount; i<ChangeList.size(); i++ )
			{
				sOld = ChangeList[i].GetKey();
				sKey.Format(_T("%s %s"), sNew, sOld);
				ChangeList[i].SetKey(sKey);
			}
		}
	}	
	//CColorGroup                m_LandLeadColorGroup;         //特徵框引腳顏色		
	
	CString sOper;
	ChangeCount=ChangeList.size();	
	sOper.Format(_T("%s %s"), AOIDataDefine.GetSetText(), _T("Land"));
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogModelLandOperate(NewPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandOperate(CAOILand *LandPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal)//儲存操作訊息-特徵框操作
{
	CString Content;
	FormatContent(sOper, sKey, nVal, Content);
	return SaveLogModelLandContent(LandPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandOperate(CAOILand *LandPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal)//儲存操作訊息-特徵框操作
{
	CString Content;
	FormatContent(sOper, sKey, fVal, Content);
	return SaveLogModelLandContent(LandPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandOperate(CAOILand *LandPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal)//儲存操作訊息-特徵框操作		
{
	CString Content;
	FormatContent(sOper, sKey, sVal, Content);
	return SaveLogModelLandContent(LandPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandOperate(CAOILand *LandPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-特徵框修改		
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogModelLandContent(LandPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndContent(CAOIWnd *WndPtr, LPCTSTR Content)//儲存操作訊息模組-檢測框內容
{
	return AOIDataCollect.SaveLogOper_WndFunc(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndCompare(CAOIWnd *OldPtr, CAOIWnd *NewPtr)//儲存操作訊息模組-檢測框-比較
{
	if ( CheckWndPtr(OldPtr) == false ) { return false; }
	if ( CheckWndPtr(NewPtr) == false ) { return false; }

	CString sKey;
	CString sOld;
	CString sNew;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	

	sKey = _T("Band ID");
	ChangeNode.SetInt(sKey, OldPtr->GetWndBandID(), NewPtr->GetWndBandID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Group ID");
	ChangeNode.SetInt(sKey, OldPtr->GetWndGroupID(), NewPtr->GetWndGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Class ID");
	ChangeNode.SetInt(sKey, OldPtr->GetWndClassID(), NewPtr->GetWndClassID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Bypass");
	ChangeNode.SetBol(sKey, OldPtr->GetWndBypassed(), NewPtr->GetWndBypassed(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Isonlated");
	ChangeNode.SetBol(sKey, OldPtr->GetWndIsolated(), NewPtr->GetWndIsolated(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetWndDefectID() != NewPtr->GetWndDefectID() )
	{
		sKey = _T("Defect ID");
		sOld = AOIDataDefine.GetWndDefectIDText(OldPtr->GetWndDefectID());
		sNew = AOIDataDefine.GetWndDefectIDText(NewPtr->GetWndDefectID());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}

	sKey = _T("Defect Group ID");
	ChangeNode.SetInt(sKey, OldPtr->GetWndDefectGroupID(), NewPtr->GetWndDefectGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetWndConstrainMode() != NewPtr->GetWndConstrainMode() )
	{
		sKey = _T("Constrain Mode");		
		sOld = AOIDataDefine.GetWndConstrainModeText(OldPtr->GetWndConstrainMode());
		sNew = AOIDataDefine.GetWndConstrainModeText(NewPtr->GetWndConstrainMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}

	sKey = _T("Land Index");
	ChangeNode.SetInt(sKey, OldPtr->GetWndLandIndex(), NewPtr->GetWndLandIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetWndLogicType() != NewPtr->GetWndLogicType() )
	{
		sKey = _T("Logic Type");		
		sOld = AOIDataDefine.GetWndLogicTypeText(OldPtr->GetWndLogicType());
		sNew = AOIDataDefine.GetWndLogicTypeText(NewPtr->GetWndLogicType());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}

	sKey = _T("Logic Group ID");
	ChangeNode.SetInt(sKey, OldPtr->GetWndLogicGroupID(), NewPtr->GetWndLogicGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetWndFollowMode() != NewPtr->GetWndFollowMode() )
	{
		sKey = _T("Follow Mode");
		sOld = AOIDataDefine.GetWndFollowModeText(OldPtr->GetWndFollowMode());
		sNew = AOIDataDefine.GetWndFollowModeText(NewPtr->GetWndFollowMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}

	if ( OldPtr->GetWndSyncMoveMode() != NewPtr->GetWndSyncMoveMode() )
	{
		sKey = _T("Sync Move Mode");
		sOld = AOIDataDefine.GetWndSyncMoveModeText(OldPtr->GetWndSyncMoveMode());
		sNew = AOIDataDefine.GetWndSyncMoveModeText(NewPtr->GetWndSyncMoveMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}

	sKey = _T("Region Link Auto");
	ChangeNode.SetBol(sKey, OldPtr->GetWndRgnLinkAuto(), NewPtr->GetWndRgnLinkAuto(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetWndRgnLinkMode() != NewPtr->GetWndRgnLinkMode() )
	{
		sKey = _T("Region Link Mode");
		sOld = AOIDataDefine.GetWndRgnLinkModeText(OldPtr->GetWndRgnLinkMode());
		sNew = AOIDataDefine.GetWndRgnLinkModeText(NewPtr->GetWndRgnLinkMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}

	sKey = _T("Region Link Ratio X");
	ChangeNode.SetDbl(sKey, OldPtr->GetWndRgnLinkRatioX(), NewPtr->GetWndRgnLinkRatioX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Region Link Ratio Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetWndRgnLinkRatioY(), NewPtr->GetWndRgnLinkRatioY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Region Link Group ID");
	ChangeNode.SetInt(sKey, OldPtr->GetWndRgnLinkGoupID(), NewPtr->GetWndRgnLinkGoupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	

	sKey = _T("Extend Range X");
	ChangeNode.SetDbl(sKey, OldPtr->GetWndExtendRangeX(), NewPtr->GetWndExtendRangeX(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Extend Range Y");
	ChangeNode.SetDbl(sKey, OldPtr->GetWndExtendRangeY(), NewPtr->GetWndExtendRangeY(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Extend Box Used");
	ChangeNode.SetBol(sKey, OldPtr->GetWndExtendBoxUsed(), NewPtr->GetWndExtendBoxUsed(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	//int                        m_WndModelMaskFlag;           //檢測框-模組遮罩旗標	
	//CAlgParam                  m_WndAlgParam;                //檢測框-演算法參數
	
	if ( SaveLogBoxCompare(OldPtr->GetWndBoxPtr(), NewPtr->GetWndBoxPtr(), ChangeList) == false )
	{	return false; }

	size_t i=0;	
	CString sOper;
	const size_t ChangeCount=ChangeList.size();	
	sOper.Format(_T("%s %s"), AOIDataDefine.GetSetText(), _T("Wnd"));
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		SaveLogModelWndOperate(NewPtr, sOper, _T("Param"), Node.GetKey(), Node.GetOld(), Node.GetNew());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal)//儲存操作訊息-檢測框操作
{
	CString Content;
	FormatContent(sOper, sKey, nVal, Content);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal)//儲存操作訊息-檢測框操作
{
	CString Content;
	FormatContent(sOper, sKey, fVal, Content);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal)//儲存操作訊息-檢測框操作		
{
	CString Content;
	FormatContent(sOper, sKey, sVal, Content);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sAlg, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-檢測框修改		
{
	CString Content;
	FormatContent(sOper, sAlg, sKey, sOld, sNew, Content);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndRoiContent(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR Content)//儲存操作訊息-檢測子框內容
{
	return AOIDataCollect.SaveLogOper_WndRoiFunc(WndPtr, WndRoiPtr, Content);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndRoiOperate(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal)//儲存操作訊息-檢測子框操作
{
	CString Content;
	FormatContent(sOper, sKey, nVal, Content);
	return SaveLogModelWndRoiContent(WndPtr, WndRoiPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndRoiOperate(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal)//儲存操作訊息-檢測子框操作
{
	CString Content;
	FormatContent(sOper, sKey, fVal, Content);
	return SaveLogModelWndRoiContent(WndPtr, WndRoiPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndRoiOperate(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal)//儲存操作訊息-檢測子框操作		
{
	CString Content;
	FormatContent(sOper, sKey, sVal, Content);
	return SaveLogModelWndRoiContent(WndPtr, WndRoiPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndRoiOperate(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR sOper, LPCTSTR sAlg, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-檢測子框修改		
{
	CString Content;
	FormatContent(sOper, sAlg, sKey, sOld, sNew, Content);
	return SaveLogModelWndRoiContent(WndPtr, WndRoiPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndMaskContent(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR Content)//儲存操作訊息-遮罩框內容
{
	return AOIDataCollect.SaveLogOper_WndMaskFunc(WndPtr, WndMaskPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndMaskOperate(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal)//儲存操作訊息-遮罩框操作
{
	CString Content;
	FormatContent(sOper, sKey, nVal, Content);
	return SaveLogModelWndMaskContent(WndPtr, WndMaskPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndMaskOperate(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal)//儲存操作訊息-遮罩框操作
{
	CString Content;
	FormatContent(sOper, sKey, fVal, Content);
	return SaveLogModelWndMaskContent(WndPtr, WndMaskPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndMaskOperate(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal)//儲存操作訊息-遮罩框操作		
{
	CString Content;
	FormatContent(sOper, sKey, sVal, Content);
	return SaveLogModelWndMaskContent(WndPtr, WndMaskPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndMaskOperate(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR sOper, LPCTSTR sAlg, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-遮罩框修改		
{
	CString Content;
	FormatContent(sOper, sAlg, sKey, sOld, sNew, Content);
	return SaveLogModelWndMaskContent(WndPtr, WndMaskPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgCompare(CAOIWnd *WndPtr, CAlgParam *OldPtr, CAlgParam *NewPtr)//儲存操作訊息模組-演算法比較
{
	if ( CheckWndPtr(WndPtr) == false ) { return false; }
	if ( NULL == (OldPtr) ) { return false; }
	if ( NULL == (NewPtr) ) { return false; }

	CString sKey;
	CString sOld;
	CString sNew;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	
	
	if ( OldPtr->GetAlgType() != NewPtr->GetAlgType() )
	{
		sKey = _T("Alg Type");
		sOld = AOIDataDefine.GetAlgTypeText(OldPtr->GetAlgType());
		sNew = AOIDataDefine.GetAlgTypeText(NewPtr->GetAlgType());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}

	sKey = _T("Group ID");
	ChangeNode.SetInt(sKey, OldPtr->GetAlgGroupID(), NewPtr->GetAlgGroupID(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	//CAlgBinaryParam            m_AlgMaskBinParam;            //演算法-遮罩二值化參數-輔助二值化
	//CAlgBinaryParam            m_AlgImageBinParam;           //演算法-影像二值化參數-主要影像
		
	//double                     m_AlgSpaceToGrayRatio;        //演算法-空間比例
	//bool                       m_AlgBaseValueEnabled;        //演算法-基準值啟用
	//int                        m_AlgBaseValueGroupID;        //演算法-基準值群組編號
	//double                     m_AlgBaseValueReading;        //演算法-基準值讀值
	//---------------------------------------------------------------------------------//	

	/*
	bool                        m_AlgPatternTestAll;          //演算法-樣板每個都測試
	unsigned int                m_AlgPatternCount;            //演算法-樣板圖數量
	int                         m_AlgPatternPolarity;         //演算法-樣板圖方向
	unsigned int                m_AlgPatternResultIndex;      //演算法-樣板圖結果引數	
	double                      m_AlgPatternSimilarityUSL;    //演算法-樣板相似度上限
	double                      m_AlgPatternSimilarityLSL;    //演算法-樣板相似度下限
	double                      m_AlgPatternSimilarityReading;//演算法-樣板相似度讀值
	double                      m_AlgPatternAngleExpand;      //演算法-樣板角度外擴值
	double                      m_AlgPatternScaleExpand;      //演算法-樣板縮放外擴值	
	bool                        m_AlgPatternScaleIsotropic;   //演算法-樣板縮放等方向性
	int                         m_AlgPatternMinReducedArea;   //演算法-樣板最小保留面積	
	int                         m_AlgPatternFinalReduction;   //演算法-樣板最末殘餘層數	
	bool                        m_AlgPatternAdvancedLearning; //演算法-樣板進階學習
	bool                        m_AlgPatternFileUsed;         //演算法-是否使用樣板圖檔案		
	CString                     m_AlgPatternFolder;           //演算法-樣板圖資料夾	
	std::vector<CPatternParam>  m_AlgPatternParamList;       //演算法-樣板圖二值化列表
	//---------------------------------------------------------------------------------//
	TALG_PARAM_BRIGHT_RATIO    m_AlgParamBrightRatio;        //演算法-亮度比例參數	
	TALG_PARAM_OUTER_SHORT     m_AlgParamOuterShort;         //演算法-外部短路參數
	TALG_PARAM_BLOB_COUNT      m_AlgParamBlobCount;          //演算法-區塊數量參數
	TALG_PARAM_BODY_TILT       m_AlgParamBodyTilt;           //演算法-本體傾斜參數
	TALG_PARAM_MODEL_MATCH     m_AlgParamModelMatch;         //演算法-模板匹配參數
	TALG_PARAM_IMAGE_MATCH     m_AlgParamImageMatch;         //演算法-影像匹配參數
	TALG_PARAM_CHAR_VERIFY     m_AlgParamCharVerify;         //演算法-文字驗證參數
	TALG_PARAM_GROUP_COMPARE   m_AlgParamGroupCompare;       //演算法-群組比較參數
	TALG_PARAM_BARCODE_RECOGNIZE m_AlgParamBarcodeRecognize; //演算法-條碼辨識
	TALG_PARAM_OBJECT_MEASURE  m_AlgParamObjectMeasure;      //演算法-物體量測
	TALG_PARAM_COLOR_CODE      m_AlgParamColorCode;          //演算法-色碼檢測
	TALG_PARAM_FD_MATCH        m_AlgParamFdMatch;            //演算法-定位點搜尋
	TALG_PARAM_EDGE_SEARCH     m_AlgParamEdgeSearch;         //演算法-邊緣搜尋
	TALG_PARAM_SHAPE_VERIFY    m_AlgParamShapeVerify;        //演算法-外形驗證
	TALG_PARAM_ANGLE_MEASURE   m_AlgParamAngleMeasure;       //演算法-角度量測
	TALG_PARAM_PIXEL_COMPARE   m_AlgParamPixelCompare;       //演算法-像素比較
	//---------------------------------------------------------------------------------//
	*/

	sKey = _T("Offste X Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetAlgOffsetXEnabled(), NewPtr->GetAlgOffsetXEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste X USL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgOffsetXUSL(), NewPtr->GetAlgOffsetXUSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste X LSL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgOffsetXLSL(), NewPtr->GetAlgOffsetXLSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste Y Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetAlgOffsetYEnabled(), NewPtr->GetAlgOffsetYEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste Y USL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgOffsetYUSL(), NewPtr->GetAlgOffsetYUSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste Y LSL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgOffsetYLSL(), NewPtr->GetAlgOffsetYLSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste L Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetAlgOffsetLEnabled(), NewPtr->GetAlgOffsetLEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste L USL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgOffsetLUSL(), NewPtr->GetAlgOffsetLUSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste L LSL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgOffsetLLSL(), NewPtr->GetAlgOffsetLLSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste A Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetAlgOffsetAEnabled(), NewPtr->GetAlgOffsetAEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste A USL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgOffsetAUSL(), NewPtr->GetAlgOffsetAUSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Offste A LSL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgOffsetALSL(), NewPtr->GetAlgOffsetALSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Skew Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetAlgSkewEnabled(), NewPtr->GetAlgSkewEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Skew USL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgSkewUSL(), NewPtr->GetAlgSkewUSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Skew LSL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgSkewLSL(), NewPtr->GetAlgSkewLSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey = _T("Scale Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetAlgScaleEnabled(), NewPtr->GetAlgScaleEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Scale USL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgScaleUSL(), NewPtr->GetAlgScaleUSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Scale LSL");
	ChangeNode.SetDbl(sKey, OldPtr->GetAlgScaleLSL(), NewPtr->GetAlgScaleLSL(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	size_t i=0;
	CString sOper;
	CString Content;	
	const size_t ChangeCount=ChangeList.size();	
	sOper.Format(_T("%s %s"), AOIDataDefine.GetSetText(), _T("Algorithm"));
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];
		FormatContent(sOper, Node.GetKey(), Node.GetOld(), Node.GetNew(), Content);
		LogOperCtrl.SaveLogModelWndContent(WndPtr, Content);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgBinaryCompare(CAOIWnd *WndPtr, CAlgBinaryParam *OldPtr, CAlgBinaryParam *NewPtr)//儲存操作訊息模組-檢測框-演算法-2值化
{
	if ( CheckWndPtr(WndPtr) == false ) { return false; }
	if ( NULL == (OldPtr) ) { return false; }
	if ( NULL == (NewPtr) ) { return false; }

	CString sKey;
	CString sOld;
	CString sNew;
	DWORD_PTR dwData=0;
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	
	//unsigned int               GetBinaryFrameIndex() const { return m_FrameIndex; }	
	//unsigned int               GetBinaryFrameUniqueID() const { return m_FrameUniqueID; }		
	//bool                       GetBinaryFrameSpaceEnabled() const { return m_FrameSpaceEnabled; }		
	//unsigned int               GetBinaryBelongIndex() const { return m_BelongIndex; }				
	//BIN_PARAM_BELONG_TO        GetBinaryBelongToWho() const { return m_BelongToWho; }		        
	
	if ( OldPtr->GetBinaryImageSourceMode() != NewPtr->GetBinaryImageSourceMode() )
	{
		sKey = _T("Image Source");
		sOld = AOIDataDefine.GetAlgImageSourceModeText(OldPtr->GetBinaryImageSourceMode());
		sNew = AOIDataDefine.GetAlgImageSourceModeText(NewPtr->GetBinaryImageSourceMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);		
	}

	if ( OldPtr->GetBinaryMode() != NewPtr->GetBinaryMode() )
	{
		sKey = _T("Binary Mode");
		sOld = AOIDataDefine.GetAlgBinaryModeText(OldPtr->GetBinaryMode());
		sNew = AOIDataDefine.GetAlgBinaryModeText(NewPtr->GetBinaryMode());
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Binary Invert");
	ChangeNode.SetBol(sKey, OldPtr->GetBinaryInvert(), NewPtr->GetBinaryInvert(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Gray Invert");
	ChangeNode.SetBol(sKey, OldPtr->GetGrayInvert(), NewPtr->GetGrayInvert(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Gain Value");
	ChangeNode.SetDbl(sKey, OldPtr->GetGrayGainValue(), NewPtr->GetGrayGainValue(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Gain Enabled");
	ChangeNode.SetBol(sKey, OldPtr->GetGrayGainEnabled(), NewPtr->GetGrayGainEnabled(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	
	if ( OldPtr->GetGrayNoiseFilter1().FilterMode != NewPtr->GetGrayNoiseFilter1().FilterMode )
	{
		sKey = _T("Gray Filter Mode");
		sOld = AOIDataDefine.GetAlgNoiseFilterModeText(OldPtr->GetGrayNoiseFilter1().FilterMode);
		sNew = AOIDataDefine.GetAlgNoiseFilterModeText(NewPtr->GetGrayNoiseFilter1().FilterMode);	
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Gray Filter Param 1");	
	ChangeNode.SetInt(sKey, OldPtr->GetGrayNoiseFilter1().FilterParam1, NewPtr->GetGrayNoiseFilter1().FilterParam1, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Gray Filter Param 2");	
	ChangeNode.SetInt(sKey, OldPtr->GetGrayNoiseFilter1().FilterParam2, NewPtr->GetGrayNoiseFilter1().FilterParam2, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetGrayNoiseFilter2().FilterMode != NewPtr->GetGrayNoiseFilter2().FilterMode )
	{
		sKey = _T("Gray Filter 2 Mode");
		sOld = AOIDataDefine.GetAlgNoiseFilterModeText(OldPtr->GetGrayNoiseFilter2().FilterMode);
		sNew = AOIDataDefine.GetAlgNoiseFilterModeText(NewPtr->GetGrayNoiseFilter2().FilterMode);	
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Gray Filter 2 Param 1");	
	ChangeNode.SetInt(sKey, OldPtr->GetGrayNoiseFilter2().FilterParam1, NewPtr->GetGrayNoiseFilter2().FilterParam1, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Gray Filter 2 Param 2");	
	ChangeNode.SetInt(sKey, OldPtr->GetGrayNoiseFilter2().FilterParam2, NewPtr->GetGrayNoiseFilter2().FilterParam2, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	if ( OldPtr->GetBinaryNoiseFilter1().FilterMode != NewPtr->GetBinaryNoiseFilter1().FilterMode )
	{
		sKey = _T("Binary Filter Mode");
		sOld = AOIDataDefine.GetAlgNoiseFilterModeText(OldPtr->GetBinaryNoiseFilter1().FilterMode);
		sNew = AOIDataDefine.GetAlgNoiseFilterModeText(NewPtr->GetBinaryNoiseFilter1().FilterMode);	
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Binary Filter Param 1");	
	ChangeNode.SetInt(sKey, OldPtr->GetBinaryNoiseFilter1().FilterParam1, NewPtr->GetBinaryNoiseFilter1().FilterParam1, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Binary Filter Param 2");	
	ChangeNode.SetInt(sKey, OldPtr->GetBinaryNoiseFilter1().FilterParam2, NewPtr->GetBinaryNoiseFilter1().FilterParam2, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	if ( OldPtr->GetBinaryNoiseFilter2().FilterMode != NewPtr->GetBinaryNoiseFilter2().FilterMode )
	{
		sKey = _T("Binary Filter 2 Mode");
		sOld = AOIDataDefine.GetAlgNoiseFilterModeText(OldPtr->GetBinaryNoiseFilter2().FilterMode);
		sNew = AOIDataDefine.GetAlgNoiseFilterModeText(NewPtr->GetBinaryNoiseFilter2().FilterMode);	
		ChangeNode.SetStr(sKey, sOld, sNew, dwData);
		ChangeList.push_back(ChangeNode);
	}

	sKey = _T("Binary Filter 2 Param 1");	
	ChangeNode.SetInt(sKey, OldPtr->GetBinaryNoiseFilter2().FilterParam1, NewPtr->GetBinaryNoiseFilter2().FilterParam1, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Binary Filter 2 Param 2");	
	ChangeNode.SetInt(sKey, OldPtr->GetBinaryNoiseFilter2().FilterParam2, NewPtr->GetBinaryNoiseFilter2().FilterParam2, dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	sKey.Format(_T("%s [%s]"), _T("Weightting"), AOIDataDefine.GetColorText_Red());	
	ChangeNode.SetInt(sKey, OldPtr->GetBinarySynthesisWR(), NewPtr->GetBinarySynthesisWR(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey.Format(_T("%s [%s]"), _T("Weightting"), AOIDataDefine.GetColorText_Green());	
	ChangeNode.SetInt(sKey, OldPtr->GetBinarySynthesisWG(), NewPtr->GetBinarySynthesisWG(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey.Format(_T("%s [%s]"), _T("Weightting"), AOIDataDefine.GetColorText_Blue());	
	ChangeNode.SetInt(sKey, OldPtr->GetBinarySynthesisWB(), NewPtr->GetBinarySynthesisWB(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey = _T("Color Gorup Link Index");	
	ChangeNode.SetInt(sKey, OldPtr->GetBinaryColorGroupLinkIndex(), NewPtr->GetBinaryColorGroupLinkIndex(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_FIXED_THRESHOLD), _T("High"));
	ChangeNode.SetInt(sKey, OldPtr->GetFixedThresholdHigh(), NewPtr->GetFixedThresholdHigh(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_FIXED_THRESHOLD), _T("Low"));
	ChangeNode.SetInt(sKey, OldPtr->GetFixedThresholdLow(), NewPtr->GetFixedThresholdLow(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_FIXED_THRESHOLD), _T("Target"));
	ChangeNode.SetDbl(sKey, OldPtr->GetRatioThresholdTarget(), NewPtr->GetRatioThresholdTarget(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_FIXED_THRESHOLD), _T("High Ratio"));
	ChangeNode.SetDbl(sKey, OldPtr->GetRatioThresholdRatioHigh(), NewPtr->GetRatioThresholdRatioHigh(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_FIXED_THRESHOLD), _T("Low Ratio"));
	ChangeNode.SetDbl(sKey, OldPtr->GetRatioThresholdRatioLow(), NewPtr->GetRatioThresholdRatioLow(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_DYNAMIC_THRESHOLD), AOIDataDefine.GetRatioText());
	ChangeNode.SetDbl(sKey, OldPtr->GetDynamicThresholdRatio(), NewPtr->GetDynamicThresholdRatio(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_RELATIVE_AVE_THRESHOLD), _T("Above"));
	ChangeNode.SetInt(sKey, OldPtr->GetRelativeAveThresholdAbove(), NewPtr->GetRelativeAveThresholdAbove(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_RELATIVE_AVE_THRESHOLD), _T("Below"));
	ChangeNode.SetInt(sKey, OldPtr->GetRelativeAveThresholdBelow(), NewPtr->GetRelativeAveThresholdBelow(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_RELATIVE_AVE_THRESHOLD), _T("Bias"));
	ChangeNode.SetInt(sKey, OldPtr->GetRelativeAveThresholdBias(), NewPtr->GetRelativeAveThresholdBias(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_ADAPTIVE_THRESHOLD), _T("Gap"));
	ChangeNode.SetInt(sKey, OldPtr->GetAdaptiveThresholdGap(), NewPtr->GetAdaptiveThresholdGap(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);
	
	sKey.Format(_T("%s %s"), AOIDataDefine.GetAlgBinaryModeText(BINARY_ADAPTIVE_THRESHOLD), _T("Calc Size"));
	ChangeNode.SetInt(sKey, OldPtr->GetAdaptiveThresholdCalcSize(), NewPtr->GetAdaptiveThresholdCalcSize(), dwData);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);

	size_t i=0;
	CString sOper;
	CString Content;	
	const size_t ChangeCount=ChangeList.size();	
	sOper.Format(_T("%s %s"), AOIDataDefine.GetSetText(), _T("Binary"));	
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];
		FormatContent(sOper, Node.GetKey(), Node.GetOld(), Node.GetNew(), Content);
		LogOperCtrl.SaveLogModelWndContent(WndPtr, Content);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgPatternContent(CAOIWnd *WndPtr, LPCTSTR sOper)//儲存操作訊息模組-檢測框-演算法-樣板內容
{
	CString Content;
	LPCTSTR sKey = AOIDataDefine.GetPatternText();
	FormatContent(sOper, sKey, _T(""), Content);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgPatternContentAdd(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測框-演算法-樣板-新增
{
	return SaveLogModelWndAlgPatternContent(WndPtr, AOIDataDefine.GetAddText());	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgPatternContentText(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測框-演算法-樣板-文字
{
	return SaveLogModelWndAlgPatternContent(WndPtr, _T("Change Text"));	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgPatternContentDelete(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測框-演算法-樣板-刪除
{	
	return SaveLogModelWndAlgPatternContent(WndPtr, AOIDataDefine.GetDeleteText());	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgPatternContentModify(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測框-演算法-樣板-修改
{
	return SaveLogModelWndAlgPatternContent(WndPtr, AOIDataDefine.GetModifyText());	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgPatternContentClearAll(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測框-演算法-樣板-清除全部
{	
	return SaveLogModelWndAlgPatternContent(WndPtr, AOIDataDefine.GetClearText());	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgColorFilterContent(CAOIWnd *WndPtr, LPCTSTR sOper)//儲存操作訊息模組-檢測框-演算法-抽色內容
{	
	return SaveLogModelWndContent(WndPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgColorFilterOperate(CAOIWnd *WndPtr, LPCTSTR sOper)//儲存操作訊息-檢測框-演算法-抽色修改		
{
	CString sContent;
	CString sKey = AOIDataDefine.GetAlgBinaryModeText(BINARY_COLOR_FILTER);
	sContent.Format(_T("%s %s"), sKey, sOper);
	return SaveLogModelWndContent(WndPtr, sContent);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgColorFilterOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew)//儲存操作訊息-檢測框-演算法-抽色修改		
{
	CString Content;
	FormatContent(sOper, sKey, sOld, sNew, Content);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgColorFilterOperateExtractWndColor(CAOIWnd *WndPtr)//儲存操作訊息-檢測框-演算法-抽色-檢測框顏色
{
	return SaveLogModelWndAlgColorFilterOperate(WndPtr, _T("Extract Wnd Color"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndAlgColorFilterCompare(CAOIWnd *WndPtr, int Index, CColorRGBV *OldPtr, CColorRGBV *NewPtr)//儲存操作訊息模組-檢測框-演算法-抽色比較
{
	if ( CheckWndPtr(WndPtr) == false ) { return false; }	
	if ( CheckRGBVPtr(OldPtr) == false ) { return false; }
	if ( CheckRGBVPtr(NewPtr) == false ) { return false; }

	CString sMax = _T("Upper");
	CString sMin = _T("Lower");
	TLogOperModify ChangeNode;
	std::vector<TLogOperModify> ChangeList;	
	//COLOR_RGBV_MODE            GetColorMode() const { return m_RGBVColorMode; }	
	//COLOR_LOGIC_MODE           GetLogicMode() const { return m_RGBVLogicMode; }
	// Red
	ChangeNode.SetInt(sMax, OldPtr->GetRedMax(), NewPtr->GetRedMax(), COLOR_RGBV_RED);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	ChangeNode.SetInt(sMin, OldPtr->GetRedMin(), NewPtr->GetRedMin(), COLOR_RGBV_RED);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	ChangeNode.SetBol(_T(""), OldPtr->GetRedEnabled(), NewPtr->GetRedEnabled(), COLOR_RGBV_RED);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	// Green
	ChangeNode.SetInt(sMax, OldPtr->GetGreenMax(), NewPtr->GetGreenMax(), COLOR_RGBV_GREEN);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	ChangeNode.SetInt(sMin, OldPtr->GetGreenMin(), NewPtr->GetGreenMin(), COLOR_RGBV_GREEN);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	ChangeNode.SetBol(_T(""), OldPtr->GetGreenEnabled(), NewPtr->GetGreenEnabled(), COLOR_RGBV_GREEN);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	// Blue
	ChangeNode.SetInt(sMax, OldPtr->GetBlueMax(), NewPtr->GetBlueMax(), COLOR_RGBV_BLUE);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	ChangeNode.SetInt(sMin, OldPtr->GetBlueMin(), NewPtr->GetBlueMin(), COLOR_RGBV_BLUE);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	ChangeNode.SetBol(_T(""), OldPtr->GetBlueEnabled(), NewPtr->GetBlueEnabled(), COLOR_RGBV_BLUE);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	// Value
	ChangeNode.SetInt(sMax, OldPtr->GetValueMax(), NewPtr->GetValueMax(), COLOR_RGBV_VALUE);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	ChangeNode.SetInt(sMin, OldPtr->GetValueMin(), NewPtr->GetValueMin(), COLOR_RGBV_VALUE);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	
	ChangeNode.SetBol(_T(""), OldPtr->GetValueEnabled(), NewPtr->GetValueEnabled(), COLOR_RGBV_VALUE);
	CmpAndAddLogOperModif_OBJ(ChangeNode, ChangeList);	

	size_t i=0;	
	CString sOper, sColor;
	const int rgbvIndex=Index;
	const size_t ChangeCount=ChangeList.size();
	CString sFunc = AOIDataDefine.GetAlgBinaryModeText(BINARY_COLOR_FILTER);	
	for ( i=0; i<ChangeCount; i++ )
	{
		const TLogOperModify &Node=ChangeList[i];		
		COLOR_RGBV_MODE RGBVMode = (COLOR_RGBV_MODE)(Node.GetData());		
		sColor = AOIDataDefine.GetAlgColorRGBVModeText(RGBVMode);
		sOper.Format(_T("%s %s_%02d[%s]"), AOIDataDefine.GetSetText(), sFunc, rgbvIndex+1, sColor);
		LogOperCtrl.SaveLogModelWndAlgColorFilterOperate(WndPtr, sOper, Node.GetKey(), Node.GetOld(), Node.GetNew());
		return true;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelected(CAOIProject *ProjrectPtr, LPCTSTR Oper, LPCTSTR Val)//儲存操作訊息專案整板選取
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	std::vector<CAOIPanel*> PanelSelList;
	if ( ProjrectPtr->GetProjectPanelSelected(PanelSelList) == false ) 
	{	return false; }
	
	size_t       i=0;			
	CString      str;	
	CAOIPanel   *PanelPtr = NULL;		
	const size_t PanelCount = PanelSelList.size();
	if ( 0 == PanelCount ) { return true; }
	for ( i=0; i<PanelCount; i++ )
	{		
		PanelPtr = PanelSelList[i];		
		if ( NULL == Val )
		{	str = Oper; }
		else
		{	str.Format(_T("%s %s"), Oper, Val); }
		SaveLogPanelContent(PanelPtr, str);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedCreate(CAOIProject *ProjrectPtr)//儲存操作訊息專案整板選取-建立
{
	return SaveLogProjectPanelSelected(ProjrectPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedDelete(CAOIProject *ProjrectPtr)//儲存操作訊息專案整板選取-刪除
{
	return SaveLogProjectPanelSelected(ProjrectPtr, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedBypassed(CAOIProject *ProjrectPtr)//儲存操作訊息專案整板選取-不檢測
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	CString str;
	CAOIPanel *PanelPtr = ProjrectPtr->GetProjectPanelPtrBySelected();
	if ( CheckPanelPtr(PanelPtr) == false ) { return true; }

	bool Bypass = PanelPtr->GetPanelBypassed();
	if ( false == Bypass ) { str = _T("Cancel Bypass"); }
	else { str = _T("Enable Bypass"); }
	return SaveLogProjectPanelSelected(ProjrectPtr, str);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedMirrorX(CAOIProject *ProjrectPtr)//儲存操作訊息專案整板選取-鏡射X
{
	return SaveLogProjectPanelSelected(ProjrectPtr, _T("Mirror X"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedMirrorY(CAOIProject *ProjrectPtr)//儲存操作訊息專案整板選取-鏡射Y	
{
	return SaveLogProjectPanelSelected(ProjrectPtr, _T("Mirror Y"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedRotate(CAOIProject *ProjrectPtr, double Angle)//儲存操作訊息專案整板選取-旋轉
{
	CString sOper, sVal;
	sOper = _T("Rotate");
	sVal.Format(_T("%.2f"), Angle);
	return SaveLogProjectPanelSelected(ProjrectPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedMove(CAOIProject *ProjrectPtr, double X, double Y)//儲存操作訊息專案整板選取-移動
{
	CString sOper, sVal;
	sOper = _T("Move");
	sVal.Format(_T("(%.2f, %.2f)"), X, Y);
	return SaveLogProjectPanelSelected(ProjrectPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedBarcodeCodeIndex(CAOIProject *ProjrectPtr, unsigned int index)//儲存操作訊息專案整板選取-條碼序號
{
	CString sOper;
	sOper.Format(_T("Set Barcode Code Index (%d)"), index);
	return SaveLogProjectPanelSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectPanelSelectedBarcodeDeviceIndex(CAOIProject *ProjrectPtr, unsigned int index)//儲存操作訊息專案整板選取-條碼機序號	
{
	CString sOper;
	sOper.Format(_T("Set Barcode Device Index (%d)"), index);
	return SaveLogProjectPanelSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelected(CAOIProject *ProjrectPtr, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息專案單板選取
{	
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	std::vector<CAOIBoard*> BoardSelList;
	if ( ProjrectPtr->GetProjectBoardSelected(BoardSelList) == false ) 
	{	return false; }
	
	size_t       i=0;			
	CString      str;	
	CAOIBoard   *BoardPtr = NULL;		
	const size_t BoardCount = BoardSelList.size();
	if ( 0 == BoardCount ) { return true; }
	for ( i=0; i<BoardCount; i++ )
	{		
		BoardPtr = BoardSelList[i];		
		if ( NULL == val )
		{	str = Oper; }
		else
		{	str.Format(_T("%s %s"), Oper, val); }
		SaveLogBoardContent(BoardPtr, str);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedCreate(CAOIProject *ProjrectPtr)//儲存操作訊息專案單板選取-建立
{
	return SaveLogProjectBoardSelected(ProjrectPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedDelete(CAOIProject *ProjrectPtr)//儲存操作訊息專案單板選取-刪除	
{
	return SaveLogProjectBoardSelected(ProjrectPtr, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedBypassed(CAOIProject *ProjrectPtr)//儲存操作訊息專案單板選取-不檢測	
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	CString str;
	CAOIBoard *BoardPtr = ProjrectPtr->GetProjectBoardPtrBySelected();
	if ( CheckBoardPtr(BoardPtr) == false ) { return true; }
	
	bool Bypass = BoardPtr->GetBoardBypassed();
	if ( false == Bypass ) { str = _T("Cancel Bypass"); }
	else { str = _T("Enable Bypass"); }
	return SaveLogProjectBoardSelected(ProjrectPtr, str);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedMirrorX(CAOIProject *ProjrectPtr)//儲存操作訊息專案單板選取-鏡射X
{
	return SaveLogProjectBoardSelected(ProjrectPtr, _T("Mirror X"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedMirrorY(CAOIProject *ProjrectPtr)//儲存操作訊息專案單板選取-鏡射Y
{
	return SaveLogProjectBoardSelected(ProjrectPtr, _T("Mirror Y"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedRotate(CAOIProject *ProjrectPtr, double val)//儲存操作訊息專案單板選取-旋轉
{
	CString sOper, sVal;
	sOper = _T("Rotate");
	sVal.Format(_T("%.2f"), val);	
	return SaveLogProjectBoardSelected(ProjrectPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedMove(CAOIProject *ProjrectPtr, double X, double Y)//儲存操作訊息專案單板選取-移動
{
	CString sOper, sVal;
	sOper = _T("Move");	
	sVal.Format(_T("(%.2f, %.2f"), X, Y);	
	return SaveLogProjectBoardSelected(ProjrectPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedBarcodeCodeIndex(CAOIProject *ProjrectPtr, unsigned int index)//儲存操作訊息專案單板選取-條碼序號
{
	CString sOper;
	sOper.Format(_T("Set Barcode Code Index (%d)"), index);
	return SaveLogProjectBoardSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBoardSelectedBarcodeDeviceIndex(CAOIProject *ProjrectPtr, unsigned int index)//儲存操作訊息專案單板選取-條碼機序號	
{
	CString sOper;
	sOper.Format(_T("Set Barcode Device Index (%d)"), index);
	return SaveLogProjectBoardSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectFd(CAOIFd *FdPtr, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息專案定位點
{
	if ( CheckFdPtr(FdPtr) == false ) { return false; }
	CString str;
	if ( NULL == val )
	{	str = Oper; }
	else
	{	str.Format(_T("%s %s"), Oper, val); }		
	return SaveLogFdContent(FdPtr, str);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectFdAdd(CAOIFd *FdPtr)//儲存操作訊息專案定位點選取-新增
{
	return SaveLogProjectFd(FdPtr, AOIDataDefine.GetAddText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectFdSelected(const std::vector<CAOIFd*> &FdList, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息專案定位點選取
{
	bool bSucc=true;
	size_t       i=0;				
	CAOIFd      *FdPtr = NULL;	
	const size_t FdCount = FdList.size();
	if ( 0 == FdCount ) { return true; }
	for ( i=0; i<FdCount; i++ )
	{		
		FdPtr = FdList[i];		
		if ( SaveLogProjectFd(FdPtr, Oper, val) == false )
		{	bSucc = false; }
	}	
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectFdSelectedAdd(const std::vector<CAOIFd*> &FdList)//儲存操作訊息專案定位點選取-複製
{
	return SaveLogProjectFdSelected(FdList, AOIDataDefine.GetAddText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectFdSelectedClone(const std::vector<CAOIFd*> &FdList)//儲存操作訊息專案定位點選取-複製
{
	return SaveLogProjectFdSelected(FdList, AOIDataDefine.GetCloneText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectFdSelectedDelete(CAOIProject *ProjrectPtr)//儲存操作訊息專案定位點選取-刪除
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	std::vector<CAOIFd*> FdSelList;
	if ( ProjrectPtr->GetProjectFdSelected(FdSelList) == false ) 
	{	return false; }
	return SaveLogProjectFdSelectedDelete(FdSelList);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectFdSelectedDelete(const std::vector<CAOIFd*> &FdList)//儲存操作訊息專案定位點選取-刪除
{
	return SaveLogProjectFdSelected(FdList, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBarcodeSelected(CAOIProject *ProjrectPtr, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息專案條碼選取
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	std::vector<CAOIBarcode*> BarcodeSelList;
	if ( ProjrectPtr->GetProjectBarcodeSelected(BarcodeSelList) == false ) 
	{	return false; }
	
	size_t       i=0;			
	CString      str;		
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = BarcodeSelList.size();
	if ( 0 == BarcodeCount ) { return true; }
	for ( i=0; i<BarcodeCount; i++ )
	{		
		BarcodePtr = BarcodeSelList[i];		
		if ( NULL == val )
		{	str = Oper; }
		else
		{	str.Format(_T("%s %s"), Oper, val); }		
		SaveLogBarcodeContent(BarcodePtr, str);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBarcodeSelectedDelete(CAOIProject *ProjrectPtr)//儲存操作訊息專案條碼選取-刪除
{
	return SaveLogProjectBarcodeSelected(ProjrectPtr, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBarcodeSelectedRotate(CAOIProject *ProjrectPtr, double Angle)//儲存操作訊息專案條碼選取-旋轉
{
	CString sOper, sVal;
	sOper = _T("Rotate");
	sVal.Format(_T("%.2f"), Angle);
	return SaveLogProjectBarcodeSelected(ProjrectPtr, sOper, sVal);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectBarcodeSelectedLocalBasePlaneID(CAOIProject *ProjrectPtr, int val)//儲存操作訊息專案條碼選取-局部基準面
{
	CString sOper;
	sOper.Format(_T("Set Barcode Local Base Panel ID %d"), val);	
	return SaveLogProjectBarcodeSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelected(CAOIProject *ProjrectPtr, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息專案零件選取
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	std::vector<CAOIComponent*> ComponentSelList;
	if ( ProjrectPtr->GetProjectComponentSelected(ComponentSelList) == false ) 
	{	return false; }
	
	size_t       i=0;			
	CString      str;	
	CAOIComponent *ComponentPtr = NULL;		
	const size_t ComponentCount = ComponentSelList.size();
	if ( 0 == ComponentCount ) { return true; }
	for ( i=0; i<ComponentCount; i++ )
	{		
		ComponentPtr = ComponentSelList[i];		
		if ( NULL == ComponentPtr ) { continue; }
		if ( NULL == val )
		{	str = Oper; }
		else
		{	str.Format(_T("%s %s"), Oper, val); }
		SaveLogComponentContent(ComponentPtr, str);		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedCreate(CAOIProject *ProjrectPtr)//儲存操作訊息專案零件選取-建立
{
	return SaveLogProjectComponentSelected(ProjrectPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedDelete(CAOIProject *ProjrectPtr)//儲存操作訊息專案零件選取-刪除
{
	return SaveLogProjectComponentSelected(ProjrectPtr, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedMirrorX(CAOIProject *ProjrectPtr)//儲存操作訊息專案零件選取-鏡射X
{
	return SaveLogProjectComponentSelected(ProjrectPtr, _T("Mirror X"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedMirrorY(CAOIProject *ProjrectPtr)//儲存操作訊息專案零件選取-鏡射Y
{
	return SaveLogProjectComponentSelected(ProjrectPtr, _T("Mirror Y"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedBypassed(CAOIProject *ProjrectPtr)//儲存操作訊息專案零件選取-不檢測
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	CAOIComponent *ComponentPtr = ProjrectPtr->GetProjectActiveComponent();
	if ( CheckComponentPtr(ComponentPtr) == false ) { return true; }	

	CString sOper;
	bool Bypass =  ComponentPtr->GetComponentBypassed();
	if ( false == Bypass ) { sOper = _T("Cancel Bypass"); }
	else { sOper = _T("Enable Bypass"); }
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedBypass3D(CAOIProject *ProjrectPtr)//儲存操作訊息專案零件選取-不檢測3D
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	CAOIComponent *ComponentPtr = ProjrectPtr->GetProjectActiveComponent();
	if ( CheckComponentPtr(ComponentPtr) == false ) { return true; }

	CString sOper;
	bool Bypass =  ComponentPtr->GetComponentBypass3D();
	if ( false == Bypass ) { sOper = _T("Cancel Bypass3D"); }
	else { sOper = _T("Enable Bypass3D"); }
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedXBoardUnit(CAOIProject *ProjrectPtr)//儲存操作訊息專案零件選取-報廢件
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	CAOIComponent *ComponentPtr = ProjrectPtr->GetProjectActiveComponent();
	if ( CheckComponentPtr(ComponentPtr) == false ) { return true; }

	CString sOper;
	bool XBoardUnit =  ComponentPtr->GetComponentXBoardUnit();
	if ( false == XBoardUnit ) { sOper = _T("Cancel XBoardUnit"); }
	else { sOper = _T("Enable XBoardUnit"); }
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedModelIsolated(CAOIProject *ProjrectPtr)//儲存操作訊息專案零件選取-模組隔離
{
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }
	CAOIComponent *ComponentPtr = ProjrectPtr->GetProjectActiveComponent();
	if ( CheckComponentPtr(ComponentPtr) == false ) { return true; }

	CString sOper;
	bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	if ( false == ModelIsolated ) { sOper = _T("Cancel Model Isolated"); }
	else { sOper = _T("Enable Model Isolated"); }
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedRotate(CAOIProject *ProjrectPtr, double Angle)//儲存操作訊息專案零件選取-旋轉
{
	CString sOper, sVal;
	sOper = _T("Rotate");
	sVal.Format(_T("%.2f"), Angle);
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedMove(CAOIProject *ProjrectPtr, double X, double Y)//儲存操作訊息專案零件選取-移動
{
	CString sOper, sVal;
	sOper = _T("Move");
	sVal.Format(_T("(%.2f, %.2f)"), X, Y);
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedPartNumber(CAOIProject *ProjrectPtr, LPCTSTR PartNumber)//儲存操作訊息專案零件選取-料號
{
	CString sOper;
	sOper.Format(_T("Set PartNumber [%s]"), PartNumber);	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedNozzlName(CAOIProject *ProjrectPtr, LPCTSTR NozzlName)//儲存操作訊息專案零件選取-吸嘴
{
	CString sOper;
	sOper.Format(_T("Set Nozzl [%s]"), NozzlName);	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedComponentName(CAOIProject *ProjrectPtr, LPCTSTR RefName, LPCTSTR NewName)//儲存操作訊息專案零件選取-名稱
{
	CString sOper;
	sOper.Format(_T("Change Name [%s] to [%s]"), RefName, NewName);	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedMaskExtendSize_Body(CAOIProject *ProjrectPtr, double ExtW, double ExtH)//儲存操作訊息專案零件選取-遮罩外擴尺寸-本體
{
	CString sOper;
	sOper.Format(_T("Set Mask Extend Size Body (%.2f, %.2f)"), ExtW, ExtH);
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedAlarm(CAOIProject *ProjrectPtr, bool bEnable)//儲存操作訊息專案零件選取-停機警報
{
	CString sOper;
	if ( true == bEnable )
	{	sOper = _T("Enable Alarm");		}
	else
	{	sOper = _T("Cancel Alarm");	}	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedAlarmOnAOI(CAOIProject *ProjrectPtr, bool bEnable)//儲存操作訊息專案零件選取-停機警報
{
	CString sOper;
	if ( true == bEnable )
	{	sOper = _T("Enable Defect Alarm On AOI");		}
	else
	{	sOper = _T("Cancel Defect Alarm On AOI");	}	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedAlarmOnARS(CAOIProject *ProjrectPtr, bool bEnable)//儲存操作訊息專案零件選取-維修站顯示
{
	CString sOper;
	if ( true == bEnable )
	{	sOper = _T("Enable Defect Alarm On ARS");		}
	else
	{	sOper = _T("Cancel Defect Alarm On ARS");	}	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedDefectCountOnARS(CAOIProject *ProjrectPtr, bool bEnable)//儲存操作訊息專案零件選取-維修站顯示
{
	CString sOper;
	if ( true == bEnable )
	{	sOper = _T("Enable Defect Coun On ARSt");		}
	else
	{	sOper = _T("Cancel Defect Count On ARS");	}	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedSaveReportARS(CAOIProject *ProjrectPtr, bool bSave)//儲存操作訊息專案零件選取-是否儲存報告-維修站
{
	CString sOper;
	if ( true == bSave )
	{	sOper = _T("Save Report (ARS)");		}
	else
	{	sOper = _T("NO-Save Report (ARS)");	}	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedSelfField(CAOIProject *ProjrectPtr, bool bEnable)//儲存操作訊息專案零件選取-專屬區域	
{
	CString sOper;
	if ( true == bEnable )
	{	sOper = _T("Enable Self Field");		}
	else
	{	sOper = _T("Cancel Self Field");	}	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedSaveWndList(CAOIProject *ProjrectPtr, bool bEnable)//儲存操作訊息專案零件選取-儲存檢測框列表
{
	CString sOper;
	if ( true == bEnable )
	{	sOper = _T("Save Wnd List");		}
	else
	{	sOper = _T("Cancel Save Wnd List");	}	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool  CLogOperCtrl::SaveLogProjectComponentSelectedChangeBoard(CAOIProject *ProjrectPtr, CAOIBoard *BoardPtr)//儲存操作訊息專案零件選取-單板編號
{		
	if ( CheckBoardPtr(BoardPtr) == false ) { return true; }
	if ( CheckProjectPtr(ProjrectPtr) == false ) { return false; }

	CString sOper;
	CAOIPanel *PanelPtr = BoardPtr->GetBoardPanelPtr();
	if ( NULL == PanelPtr )
	{	sOper.Format(_T("Set Board ID [%d]"), BoardPtr->GetBoardIndex_Project()+1);	 }
	else
	{	sOper.Format(_T("Set Board ID [%d]"), BoardPtr->GetBoardIndex_Panel()+1);	}
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedLocalBasePlaneID(CAOIProject *ProjrectPtr, int BasePlaneID)//儲存操作訊息專案零件選取-局部基準面編號		
{
	CString sOper;
	sOper.Format(_T("Set Local Base Plane ID [%d]"), BasePlaneID);	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedDataModelParam(CAOIProject *ProjrectPtr, int Param)//儲存操作訊息專案零件選取-資料物件參數
{
	CString sOper;
	CString sEnb;
	if ( Param > FN_DISABLE ) { sEnb = AOIDataDefine.GetEnableText(); }
	else { sEnb = AOIDataDefine.GetDisableText(); }
	sOper.Format(_T("Set Component Data Model Param [%s:Lv%d]"), sEnb, Param);	
	return SaveLogProjectComponentSelected(ProjrectPtr, sOper);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedDefectAlarmAOI(CAOIProject *ProjrectPtr, DEFECT_PARAM_FROM_MODE FromMode, const CWndDefectItem &DefectItem)//儲存操作訊息專案零件選取-停機警報
{
	CString sOper;
	CString sFrom=AOIDataDefine.GetDefectParamFromText(FromMode);	
	sOper.Format(_T("Set Component Defect Alarm Param From Mode AOI [%s]"), sFrom);	
	if ( SaveLogProjectComponentSelected(ProjrectPtr, sOper) == false ) { return false; }

	sOper.Format(_T("Set Component Defect Alarm Param AOI"));	
	if ( SaveLogProjectComponentSelected(ProjrectPtr, sOper) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogProjectComponentSelectedDefectAlarmARS(CAOIProject *ProjrectPtr, DEFECT_PARAM_FROM_MODE FromMode, const CWndDefectItem &DefectItem)//儲存操作訊息專案零件選取-停機警報
{
	CString sOper;
	CString sFrom=AOIDataDefine.GetDefectParamFromText(FromMode);	
	sOper.Format(_T("Set Component Defect Alarm Param From Mode ARS [%s]"), sFrom);	
	if ( SaveLogProjectComponentSelected(ProjrectPtr, sOper) == false ) { return false; }

	sOper.Format(_T("Set Component Defect Alarm Param ARS"));	
	if ( SaveLogProjectComponentSelected(ProjrectPtr, sOper) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelModifyPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOILand *LandPtr, CAOIWndRoi *WndRoiPtr, CAOIWndMask *MaskWndPtr, double dPx, double dPy)//儲存操作訊息模組-框座標
{	
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	bool IsOK = true;	
	if ( NULL !=WndRoiPtr )
	{	IsOK = SaveLogModelWndRoiPos(ModelPtr, BoxPtr, WndPtr, WndRoiPtr, dPx, dPy);	}	
	else if ( NULL !=MaskWndPtr )
	{	IsOK = SaveLogModelWndMaskPos(ModelPtr, BoxPtr, WndPtr, MaskWndPtr, dPx, dPy);	}
	else if ( NULL != WndPtr )
	{	IsOK = SaveLogModelWndPos(ModelPtr, BoxPtr, WndPtr, dPx, dPy);	}
	else if ( NULL != LandPtr )
	{	IsOK = SaveLogModelLandPos(ModelPtr, BoxPtr, LandPtr, dPx, dPy);	}
	else
	{	IsOK = SaveLogModelBodyPos(ModelPtr, BoxPtr, dPx, dPy); }	
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelBodyPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, double dPx, double dPy)//儲存操作訊息模組-框座標
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sPos = _T("Pos");
	const double NewPx = BoxPtr->GetBoxPosX();
	const double NewPy = BoxPtr->GetBoxPosY();
	const double OldPx = NewPx-dPx;
	const double OldPy = NewPy-dPy;	
	Content.Format(_T("%s [Body] %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, sPos, sFrom, OldPx, OldPy, sTo, NewPx, NewPy);
	return SaveLogModelContent(ModelPtr, Content);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, double dPx, double dPy)//儲存操作訊息模組-框座標
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sPos = _T("Pos");
	const double NewPx = BoxPtr->GetBoxPosX();
	const double NewPy = BoxPtr->GetBoxPosY();
	const double OldPx = NewPx-dPx;
	const double OldPy = NewPy-dPy;	
	Content.Format(_T("%s %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, sPos, sFrom, OldPx, OldPy, sTo, NewPx, NewPy);
	return SaveLogModelWndContent(WndPtr, Content);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOILand *LandPtr, double dPx, double dPy)//儲存操作訊息模組-框座標
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sPos = _T("Pos");
	const double NewPx = BoxPtr->GetBoxPosX();
	const double NewPy = BoxPtr->GetBoxPosY();
	const double OldPx = NewPx-dPx;
	const double OldPy = NewPy-dPy;	
	Content.Format(_T("%s %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, sPos, sFrom, OldPx, OldPy, sTo, NewPx, NewPy);
	return SaveLogModelLandContent(LandPtr, Content);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndRoiPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, double dPx, double dPy)//儲存操作訊息模組-框座標
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sPos = _T("Pos");
	const double NewPx = BoxPtr->GetBoxPosX();
	const double NewPy = BoxPtr->GetBoxPosY();
	const double OldPx = NewPx-dPx;
	const double OldPy = NewPy-dPy;
	const unsigned int WndRoiIndex = WndRoiPtr->GetWndRoiIndex();	
	Content.Format(_T("%s [Roi_%04d] %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, WndRoiIndex+1, sPos, sFrom, OldPx, OldPy, sTo, NewPx, NewPy);
	return SaveLogModelWndContent(WndPtr, Content);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndMaskPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, double dPx, double dPy)//儲存操作訊息模組-框座標
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sPos = _T("Pos");
	const double NewPx = BoxPtr->GetBoxPosX();
	const double NewPy = BoxPtr->GetBoxPosY();
	const double OldPx = NewPx-dPx;
	const double OldPy = NewPy-dPy;
	const unsigned int WndMaskIndex = WndMaskPtr->GetWndMaskIndex();
	Content.Format(_T("%s [Mask_%04d] %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, WndMaskIndex+1, sPos, sFrom, OldPx, OldPy, sTo, NewPx, NewPy);
	return SaveLogModelWndContent(WndPtr, Content);		
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelModifySize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOILand *LandPtr, CAOIWndRoi *WndRoiPtr, CAOIWndMask *MaskWndPtr, const TREGION4D &dPos)//儲存操作訊息模組-框尺寸	
{	
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }

	bool IsOK = true;
	if ( NULL != WndRoiPtr )
	{	IsOK = SaveLogModelWndRoiSize(ModelPtr, BoxPtr, WndPtr, WndRoiPtr, dPos);	}
	else if ( NULL != MaskWndPtr )
	{	IsOK = SaveLogModelWndMaskSize(ModelPtr, BoxPtr, WndPtr, MaskWndPtr, dPos);	}
	else if ( NULL != WndPtr )
	{	IsOK = SaveLogModelWndSize(ModelPtr, BoxPtr, WndPtr, dPos);	}
	else if ( NULL != LandPtr )
	{	IsOK = SaveLogModelLandSize(ModelPtr, BoxPtr, LandPtr, dPos);	}
	else
	{	IsOK = SaveLogModelBodySize(ModelPtr, BoxPtr, dPos); }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelBodySize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, const TREGION4D &dPos)//儲存操作訊息模組-框尺寸
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sSize=AOIDataDefine.GetSizeText();
	const double NewW = BoxPtr->GetBoxSizeX();
	const double NewH = BoxPtr->GetBoxSizeY();	
	const double OldW = NewW-dPos.GetWidth();
	const double OldH = NewH-dPos.GetHeight();	
	Content.Format(_T("%s [Body] %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, sSize, sFrom, OldW, OldH, sTo, NewW, NewH);
	return SaveLogModelContent(ModelPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, const TREGION4D &dPos)//儲存操作訊息模組-框尺寸
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;	
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sSize=AOIDataDefine.GetSizeText();
	const double NewW = BoxPtr->GetBoxSizeX();
	const double NewH = BoxPtr->GetBoxSizeY();	
	const double OldW = NewW-dPos.GetWidth();
	const double OldH = NewH-dPos.GetHeight();	
	Content.Format(_T("%s %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, sSize, sFrom, OldW, OldH, sTo, NewW, NewH);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOILand *LandPtr, const TREGION4D &dPos)//儲存操作訊息模組-框尺寸
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sSize=AOIDataDefine.GetSizeText();
	const double NewW = BoxPtr->GetBoxSizeX();
	const double NewH = BoxPtr->GetBoxSizeY();	
	const double OldW = NewW-dPos.GetWidth();
	const double OldH = NewH-dPos.GetHeight();	
	const unsigned int LandIndex = LandPtr->GetLandIndex();	
	Content.Format(_T("%s %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, sSize, sFrom, OldW, OldH, sTo, NewW, NewH);
	return SaveLogModelLandContent(LandPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndRoiSize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, const TREGION4D &dPos)//儲存操作訊息模組-框尺寸
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sSize=AOIDataDefine.GetSizeText();
	const double NewW = BoxPtr->GetBoxSizeX();
	const double NewH = BoxPtr->GetBoxSizeY();	
	const double OldW = NewW-dPos.GetWidth();
	const double OldH = NewH-dPos.GetHeight();
	const unsigned int WndRoiIndex = WndRoiPtr->GetWndRoiIndex();	
	Content.Format(_T("%s [Roi_%04d] %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, WndRoiIndex+1, sSize, sFrom, OldW, OldH, sTo, NewW, NewH);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndMaskSize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndMask *MaskWndPtr, const TREGION4D &dPos)//儲存操作訊息模組-框尺寸
{
	if ( CheckBoxPtr(BoxPtr) == false ) { return false; }
	CString Content;	
	CString sTo=AOIDataDefine.GetToText();
	CString sSet=AOIDataDefine.GetSetText();
	CString sFrom=AOIDataDefine.GetFromText();
	CString sSize=AOIDataDefine.GetSizeText();
	const double NewW = BoxPtr->GetBoxSizeX();
	const double NewH = BoxPtr->GetBoxSizeY();	
	const double OldW = NewW-dPos.GetWidth();
	const double OldH = NewH-dPos.GetHeight();
	const unsigned int WndMaskIndex = MaskWndPtr->GetWndMaskIndex();
	Content.Format(_T("%s [Mask_%04d] %s %s (%.2f, %.2f) %s (%.2f, %.2f)"), sSet, WndMaskIndex+1, sSize, sFrom, OldW, OldH, sTo, NewW, NewH);
	return SaveLogModelWndContent(WndPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandFunc(CAOIModel *ModelPtr, CAOILand *LandPtr, LPCTSTR Content)//儲存操作訊息模組-特徵框函式
{
	CString str;
	if ( NULL == LandPtr ) 
	{	str = Content; }
	else
	{
		const int  GroupID = LandPtr->GetLandGroupID();
		BOX_TOWARD Toward = LandPtr->GetLandToward();
		CString strToward = AOIDataDefine.GetBoxTowardText(Toward);
		str.Format(_T("Land Group ID:%d, Toward:%s, %s"), GroupID, strToward, Content);		
	}
	return SaveLogModelContent(ModelPtr, str);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandCount(CAOIModel *ModelPtr, CAOILand *LandPtr, int Count)//儲存操作訊息模組-特徵框-數量	
{
	CString Content;	
	Content.Format(_T("Modify Model Land Count %d"), Count);
	return SaveLogModelLandFunc(ModelPtr, LandPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandPitch(CAOIModel *ModelPtr, CAOILand *LandPtr, double Pitch)//儲存操作訊息模組-特徵框-間距
{
	CString Content;	
	Content.Format(_T("Modify Model Land Count %.2f"), Pitch);
	return SaveLogModelLandFunc(ModelPtr, LandPtr, Content);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandGroupID(CAOIModel *ModelPtr, const std::vector<size_t> &IndexList, int GroupID)//儲存操作訊息模組-特徵框-群組編號
{
	CString Content;
	size_t       i=0;
	const size_t Count = IndexList.size();
	for ( i=0; i<Count; i++ )
	{
		Content.Format(_T("Set Land_%04d Group ID [%d]"), IndexList[i]+1, GroupID+1);
		SaveLogModelLandFunc(ModelPtr, NULL, Content);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandAlignID(CAOIModel *ModelPtr, const std::vector<size_t> &IndexList, int AlignID)//儲存操作訊息模組-特徵框-對齊編號	
{
	CString Content;
	size_t       i=0;
	const size_t Count = IndexList.size();
	for ( i=0; i<Count; i++ )
	{
		Content.Format(_T("Set Land_%04d Align ID [%d]"), IndexList[i]+1, AlignID+1);
		SaveLogModelLandFunc(ModelPtr, NULL, Content);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandAlignID(CAOIModel *ModelPtr, const std::vector<size_t> &IndexList, const std::vector<int> &IDList)//儲存操作訊息模組-特徵框-對齊編號	
{
	CString Content;
	size_t       i=0;
	const size_t IDCount = IDList.size();
	const size_t Count = IndexList.size();	
	if ( IDCount != Count )
	{	return SaveLogModelLandAlignID(ModelPtr, IndexList, -1);	}

	for ( i=0; i<Count; i++ )
	{
		Content.Format(_T("Set Land_%04d Align ID [%d]"), IndexList[i]+1, IDList[i]+1);
		SaveLogModelLandFunc(ModelPtr, NULL, Content);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelected(CAOIModel *ModelPtr, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息模組-特徵框選取
{	
	if ( CheckModelPtr(ModelPtr) == false ) { return false; }
	std::vector<CAOILand*> LandSelList;	
	ModelPtr->GetModelLandSelectedList(LandSelList);

	size_t       i=0;			
	CString      str;		
	CAOILand    *LandPtr = NULL;	
	const size_t LandCount = LandSelList.size();
	if ( 0 == LandCount ) { return true; }
	for ( i=0; i<LandCount; i++ )
	{		
		LandPtr = LandSelList[i];		
		if ( NULL == val )
		{	str = Oper; }
		else
		{	str.Format(_T("%s %s"), Oper, val); }		
		SaveLogModelLandContent(LandPtr, str);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedAlign(CAOIModel *ModelPtr)//儲存操作訊息模組-特徵框選取-至中
{
	return SaveLogModelLandSelected(ModelPtr, _T("Align"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedClone(CAOIModel *ModelPtr)//儲存操作訊息模組-特徵框選取-複製
{
	return SaveLogModelLandSelected(ModelPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedCreate(CAOIModel *ModelPtr)//儲存操作訊息模組-特徵框選取-建立
{
	return SaveLogModelLandSelected(ModelPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedDelete(CAOIModel *ModelPtr)//儲存操作訊息模組-特徵框選取-刪除
{
	return SaveLogModelLandSelected(ModelPtr, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedMirrorX(CAOIModel *ModelPtr)//儲存操作訊息模組-特徵框選取-鏡射X
{
	return SaveLogModelLandSelected(ModelPtr, _T("Mirror X"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedMirrorY(CAOIModel *ModelPtr)//儲存操作訊息模組-特徵框選取-鏡射Y
{
	return SaveLogModelLandSelected(ModelPtr, _T("Mirror Y"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedAlignCenterU(CAOIModel *ModelPtr)//儲存操作訊息模組-特徵框選取-至中U
{
	return SaveLogModelLandSelected(ModelPtr, _T("Align Center U"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedRotate(CAOIModel *ModelPtr, double Angle)//儲存操作訊息模組-特徵框選取-旋轉
{
	CString sOper, sVal;
	sOper = _T("Rotate");
	sVal.Format(_T("%.2f"), Angle);
	return SaveLogModelLandSelected(ModelPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelLandSelectedMove(CAOIModel *ModelPtr, double X, double Y)//儲存操作訊息模組-特徵框選取-移動
{
	CString sOper, sVal;
	sOper = _T("Move");
	sVal.Format(_T("(%.2f, %.2f)"), X, Y);
	return SaveLogModelLandSelected(ModelPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelected(CAOIModel *ModelPtr, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息模組-檢測框選取
{
	if ( CheckModelPtr(ModelPtr) == false ) { return false; }
	std::vector<CAOIWnd*> WndSelList;	
	ModelPtr->GetModelWndSelectedList(WndSelList);

	size_t       i=0;			
	CString      str;		
	CAOIWnd     *WndPtr = NULL;	
	const size_t WndCount = WndSelList.size();
	if ( 0 == WndCount ) { return true; }
	for ( i=0; i<WndCount; i++ )
	{		
		WndPtr = WndSelList[i];		
		if ( NULL == val )
		{	str = Oper; }
		else
		{	str.Format(_T("%s %s"), Oper, val); }		
		SaveLogModelWndContent(WndPtr, str);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedClone(CAOIModel *ModelPtr)//儲存操作訊息模組-檢測框選取-複製
{
	return SaveLogModelWndSelected(ModelPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedCreate(CAOIModel *ModelPtr)//儲存操作訊息模組-檢測框選取-建立
{
	return SaveLogModelWndSelected(ModelPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedDelete(CAOIModel *ModelPtr)//儲存操作訊息模組-檢測框選取-刪除		
{
	return SaveLogModelWndSelected(ModelPtr, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedMirrorX(CAOIModel *ModelPtr)//儲存操作訊息模組-檢測框選取-鏡射X
{
	return SaveLogModelWndSelected(ModelPtr, _T("Mirror X"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedMirrorY(CAOIModel *ModelPtr)//儲存操作訊息模組-檢測框選取-鏡射Y	
{
	return SaveLogModelWndSelected(ModelPtr, _T("Mirror Y"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedAlignCenter(CAOIModel *ModelPtr)//儲存操作訊息模組-檢測框選取-至中
{
	return SaveLogModelWndSelected(ModelPtr, _T("Align Center"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedAlignCenterU(CAOIModel *ModelPtr)//儲存操作訊息模組-檢測框選取-至中U
{
	return SaveLogModelWndSelected(ModelPtr, _T("Align Center U"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedAlignCenterV(CAOIModel *ModelPtr)//儲存操作訊息模組-檢測框選取-至中V
{
	return SaveLogModelWndSelected(ModelPtr, _T("Align Center V"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedRotate(CAOIModel *ModelPtr, double Angle)//儲存操作訊息模組-檢測框選取-旋轉 
{
	CString sOper, sVal;
	sOper = _T("Rotate");
	sVal.Format(_T("%.2f"), Angle);
	return SaveLogModelWndSelected(ModelPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogModelWndSelectedMove(CAOIModel *ModelPtr, double X, double Y)//儲存操作訊息模組-檢測框選取-移動	
{
	CString sOper, sVal;
	sOper = _T("Move");
	sVal.Format(_T("(%.2f, %.2f)"), X, Y);
	return SaveLogModelWndSelected(ModelPtr, sOper, sVal);
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndRoiSelected(CAOIWnd *WndPtr, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息模組-檢測子框選取	
{
	if ( CheckWndPtr(WndPtr) == false ) { return false; }
	std::vector<CAOIWndRoi*> WndRoiSelList;		
	if ( WndPtr->GetWndRoiWndSelectedList(WndRoiSelList) == false )
	{	return false; }

	size_t       i=0;			
	CString      str;		
	CAOIWndRoi  *WndRoiPtr = NULL;	
	const size_t WndRoiCount = WndRoiSelList.size();
	if ( 0 == WndRoiCount ) { return true; }
	for ( i=0; i<WndRoiCount; i++ )
	{		
		WndRoiPtr = WndRoiSelList[i];		
		if ( NULL == val )
		{	str = Oper; }
		else
		{	str.Format(_T("%s %s"), Oper, val); }		
		SaveLogModelWndRoiContent(WndPtr, WndRoiPtr, str);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndRoiSelectedClone(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測子框選取-複製
{
	return SaveLogWndRoiSelected(WndPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndRoiSelectedCreate(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測子框選取-建立
{
	return SaveLogWndRoiSelected(WndPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndRoiSelectedDelete(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測子框選取-刪除
{
	return SaveLogWndRoiSelected(WndPtr, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndRoiSelectedClearAll(CAOIWnd *WndPtr)//儲存操作訊息模組-檢測子框選取-清除全部
{
	LPCTSTR sOper = AOIDataDefine.GetClearText();
	return SaveLogModelWndOperate(WndPtr, sOper, NULL, _T("All Roi"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelected(CAOIWnd *WndPtr, LPCTSTR Oper, LPCTSTR val)//儲存操作訊息模組-遮罩框選取	
{
	if ( CheckWndPtr(WndPtr) == false ) { return false; }
	std::vector<CAOIWndMask*> WndMaskSelList;		
	if ( WndPtr->GetWndMaskWndSelectedList(WndMaskSelList) == false )
	{	return false; }

	size_t       i=0;			
	CString      str;		
	CAOIWndMask *WndMaskPtr = NULL;	
	const size_t WndMaskCount = WndMaskSelList.size();
	if ( 0 == WndMaskCount ) { return true; }
	for ( i=0; i<WndMaskCount; i++ )
	{		
		WndMaskPtr = WndMaskSelList[i];		
		if ( NULL == val )
		{	str = Oper; }
		else
		{	str.Format(_T("%s %s"), Oper, val); }		
		SaveLogModelWndMaskContent(WndPtr, WndMaskPtr, str);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedClone(CAOIWnd *WndPtr)//儲存操作訊息模組-遮罩框選取-複製
{
	return SaveLogWndMaskSelected(WndPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedCreate(CAOIWnd *WndPtr)//儲存操作訊息模組-遮罩框選取-建立
{
	return SaveLogWndMaskSelected(WndPtr, AOIDataDefine.GetCreateText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedDelete(CAOIWnd *WndPtr)//儲存操作訊息模組-遮罩框選取-刪除
{
	return SaveLogWndMaskSelected(WndPtr, AOIDataDefine.GetDeleteText());
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedClearAll(CAOIWnd *WndPtr)//儲存操作訊息模組-遮罩框選取-清除全部
{
	LPCTSTR sOper = AOIDataDefine.GetClearText();	
	return SaveLogModelWndOperate(WndPtr, sOper, NULL, _T("All Mask"));
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedRotate(CAOIWnd *WndPtr, double Angle)//儲存操作訊息模組-遮罩框選取-旋轉
{
	CString sOper, sVal;
	sOper = _T("Rotate");
	sVal.Format(_T("%.2f"), Angle);
	return SaveLogWndMaskSelected(WndPtr, sOper, sVal);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedEraseMode(CAOIWnd *WndPtr, bool bErase)//儲存操作訊息模組-遮罩框選取-框外型-清除
{
	CString sOper, sVal;
	sOper.Format(_T("%s Erase Mode"), AOIDataDefine.GetSetText());	
	sVal = AOIDataDefine.GetEnableDisableText(bErase);
	return SaveLogWndMaskSelected(WndPtr, sOper, sVal);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedShapeMode(CAOIWnd *WndPtr, BOX_SHAPE_MODE Mode)//儲存操作訊息模組-遮罩框選取-框外型
{
	CString sOper, sVal;
	sOper.Format(_T("%s Shape Mode"), AOIDataDefine.GetSetText());	
	sVal = AOIDataDefine.GetBoxShapeModeText(Mode);
	return SaveLogWndMaskSelected(WndPtr, sOper, sVal);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedShapeParam(CAOIWnd *WndPtr, double Param)//儲存操作訊息模組-遮罩框選取-框外型
{
	CString sOper, sVal;
	sOper.Format(_T("%s Shape Param"), AOIDataDefine.GetSetText());		
	sVal.Format(_T("%.2f"), Param);
	return SaveLogWndMaskSelected(WndPtr, sOper, sVal);	
}
//-------------------------------------------------------------------------------------//
bool CLogOperCtrl::SaveLogWndMaskSelectedShapeParam2(CAOIWnd *WndPtr, double Param)//儲存操作訊息模組-遮罩框選取-框外型
{
	CString sOper, sVal;
	sOper.Format(_T("%s Shape Param"), AOIDataDefine.GetSetText());		
	sVal.Format(_T("%.2f"), Param);
	return SaveLogWndMaskSelected(WndPtr, sOper, sVal);	
}
//-------------------------------------------------------------------------------------//