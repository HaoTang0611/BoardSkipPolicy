//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "JetTxtFile.h"
#include "AOIDataCollect.h"

unsigned int HASI_MoniterThreadID = 0;
HANDLE HASI_MoniterThreadHandle = NULL;
HANDLE HASI_MoniterThreadEvent = NULL;
unsigned __stdcall HASI_MoniterThreadFn(void* pParam);

//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckNPM_FuncBypass()
{
	bool bBypass = true;
#ifdef OFFLINE_VERSION
	bBypass = true;
#else
	#ifdef M2M_DISABLE	
		bBypass = true;
	#else
		if ( CheckNPM_APC_Enable() == false ) 
		{	bBypass = true;	}
		else
		{	bBypass = false;	}
	#endif//M2M_DISABLE
#endif//OFFLINE_VERSION
	SetNPM_FuncBypass(bBypass);
	return bBypass;
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::GetNPM_LogFolder() const
{
	CString str;
	str.Format(_T("%s\\%s"), GetAOILogDirectory(), _T("NPM"));
	return str;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetNPM_FuncBypass() const
{
	return m_NPM_FuncBypass;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetNPM_FuncBypass(bool val)
{
	m_NPM_FuncBypass = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckNPM_APC_Enable() const
{
	if ( GetNPM_APC_FF1_Enable() == true ) { return true; }
	if ( GetNPM_APC_FF2_Enable() == true ) { return true; }
	if ( GetNPM_APC_MFB_Enable() == true ) { return true; }	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetNPM_BarcodeEnable() const
{
	if ( FN_DISABLE != m_SystemParameter.m_M2M_NPM_Barcode_Enable ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetNPM_APC_FF1_Enable() const
{
	if ( FN_DISABLE != m_SystemParameter.m_M2M_NPM_APC_FF1_Enable ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetNPM_APC_FF2_Enable() const
{
	if ( FN_DISABLE != m_SystemParameter.m_M2M_NPM_APC_FF2_Enable ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetNPM_APC_MFB_Enable() const
{
	if ( FN_DISABLE != m_SystemParameter.m_M2M_NPM_APC_MFB_Enable ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataCollect::GetNPM_LaneName(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return m_SystemParameter.m_M2M_NPM_LaneName_LB; }
	return m_SystemParameter.m_M2M_NPM_LaneName_LA;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataCollect::GetNPM_SrcShareFolder(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return m_SystemParameter.m_M2M_NPM_InputShareFolder_LB; }
	return m_SystemParameter.m_M2M_NPM_InputShareFolder_LA;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataCollect::GetNPM_DstShareFolder(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return m_SystemParameter.m_M2M_NPM_OutputShareFolder_LB; }
	return m_SystemParameter.m_M2M_NPM_OutputShareFolder_LA;
}
//-------------------------------------------------------------------------------------//
LPCTSTR  CAOIDataCollect::GetNPM_PCBSerialName(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return m_NPM_PCBSerialName_LB; }
	return m_NPM_PCBSerialName_LA;		
}
//-------------------------------------------------------------------------------------//
CString CAOIDataCollect::GetNPM_LocalPCBSerialName(LANE_ID LaneID) const
{
	CString str;
	CString Folder=GetAOIDirectory();
	switch ( LaneID )
	{
	case LANE_ID_B:	str.Format(_T("%s\\NPM_PCBSerial_LB.srl"), Folder); break;
	default:
	case LANE_ID_A: str.Format(_T("%s\\NPM_PCBSerial_LA.srl"), Folder); break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataCollect::GetNPM_DateTime(LANE_ID LaneID) const
{
	if ( LANE_ID_B == LaneID )
	{	return m_NPM_DateTime_LB; }
	return m_NPM_DateTime_LA;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetNPM_DateTime(LPCTSTR DateTime, LANE_ID LaneID)
{
	if ( LANE_ID_B == LaneID )
	{	m_NPM_DateTime_LB = DateTime; }
	else
	{	m_NPM_DateTime_LA = DateTime; }
	return;		
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetNPM_DateTime(const CTime &DateTime, LANE_ID LaneID)
{
	CString str=DateTime.Format(_T("%Y%m%d%H%M%S"));	
	SetNPM_DateTime(str, LaneID);
}
//-------------------------------------------------------------------------------------//
std::vector<CString>& CAOIDataCollect::GetNPM_DeleteFilenameList(LANE_ID LaneID)
{
	if ( LANE_ID_B == LaneID )
	{	return m_NPM_DeleteFileList_LB;	}
	return m_NPM_DeleteFileList_LA;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::RemoveNPM_DeleteFilenameList(LANE_ID LaneID)
{	return true;//Debug
	std::vector<CString> &FileList=GetNPM_DeleteFilenameList(LaneID);
	const size_t Cnt=FileList.size();
	for ( size_t i=0; i<Cnt; i++ )
	{	::DeleteFile(FileList[i]);	}
	FileList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::AddNPM_DeleteFilename(LPCTSTR Filename, LANE_ID LaneID)
{
	std::vector<CString> &FileList=GetNPM_DeleteFilenameList(LaneID);
	FileList.push_back(Filename);	
	return true;
}
//-------------------------------------------------------------------------------------//
std::vector<CString>& CAOIDataCollect::GetNPM_BackupFilenameList(LANE_ID LaneID)
{
	if ( LANE_ID_B == LaneID )
	{	return m_NPM_BackupFileList_LB;	}
	return m_NPM_BackupFileList_LA;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::BackupNPM_BackupFilenameList(LANE_ID LaneID)
{
	CString Date, Time;
	CString DateFolder, TimeFolder;				
	CString LogFolder=GetNPM_LogFolder();
	CString DateTime=GetNPM_DateTime(LaneID);		
	std::vector<CString> &FileList=GetNPM_BackupFilenameList(LaneID);

	if ( JetAPI::GetDateTime(DateTime, Date, Time) == false )
	{	return false; }
	::CreateDirectory(LogFolder, NULL);
	DateFolder.Format(_T("%s\\%s"), LogFolder, Date);
	::CreateDirectory(DateFolder, NULL);
	TimeFolder.Format(_T("%s\\%s"), DateFolder, Time);
	::CreateDirectory(TimeFolder, NULL);

	CString Filename, PathName;
	const size_t Cnt=FileList.size();
	for ( size_t i=0; i<Cnt; i++ )
	{
		JetAPI::ExtractFileNameNoPath(FileList[i], Filename);
		PathName.Format(_T("%s\\%s"), TimeFolder, Filename);
		::CopyFile(FileList[i], PathName, FALSE);
	}
	FileList.clear();
	SetNPM_DateTime(_T(""), LaneID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::AddNPM_BackupFilename(LPCTSTR Filename, LANE_ID LaneID)
{
	std::vector<CString> &FileList=GetNPM_BackupFilenameList(LaneID);
	FileList.push_back(Filename);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetNPM_PCBSerialName(LANE_ID LaneID, LPCTSTR Name)
{
	if ( LANE_ID_B == LaneID )
	{	m_NPM_PCBSerialName_LB = Name; }
	else
	{	m_NPM_PCBSerialName_LA = Name; }
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecNPM_CopyPCBSerialFile(LANE_ID LaneID)//執行複製NPM的PCBSerial檔案
{	
#ifndef M2M_DISABLE	
	if ( GetNPM_FuncBypass() == true )	
	{	return true;	}

	CString Error;
	CString ShareFolder=GetNPM_SrcShareFolder(LaneID);
	std::vector<CString> FileList;
	SetNPM_PCBSerialName(LaneID, _T(""));	
	if ( JetAPI::ListFilesInFolder(ShareFolder, _T("srl"), FileList) == false )
	{
		Error.Format(_T("Error, List File In Folder Fault\n%s"), ShareFolder);
		SetErrorString(Error);
		return false;
	}

	const size_t FileCount=FileList.size();
	const bool UseBarcode=GetNPM_BarcodeEnable();
	if ( 0 == FileCount )
	{
		Error.Format(_T("Error, No PCB Serial File In Folder Fault\n%s"), ShareFolder);
		SetErrorString(Error);
		return false;
	}

	if ( FileCount>1 && false==UseBarcode )
	{
		Error.Format(_T("Error, More than one PCB Serial File In Folder Fault\n%s"), ShareFolder);
		SetErrorString(Error);
		return false;
	}
	
	CString ShortName;	
	if ( false == UseBarcode )	
	{	ShortName = FileList[0];	}
	else
	{
	}
	if ( ShortName.GetLength() == 0 )
	{
		Error.Format(_T("Error, No Matched PCB Serial File In Folder Fault\n%s"), ShareFolder);
		SetErrorString(Error);
		return false;
	}

	CString PCBSerialFilename;
	CString LocalPCBSerialFilename=GetNPM_LocalPCBSerialName(LaneID);
	PCBSerialFilename.Format(_T("%s\\%s"), ShareFolder, ShortName);
	
	::DeleteFile(LocalPCBSerialFilename);
	::Sleep(0);
	
	DWORD Res=0;
	FILE *pfile=::_tfopen(LocalPCBSerialFilename, _T("w"));
	if ( NULL == pfile )
	{	
		Error.Format(_T("Error, Open File Fault\n%s"), LocalPCBSerialFilename);
		SetErrorString(Error);
		return false;
	}
#ifndef _UNICODE
	Res = ::fprintf(pfile, "%s", ShortName);
#else
	Res = ::fwprintf(pfile, L"%s", ShortName);
#endif//_UNICODE	
	::fclose(pfile); pfile=NULL;
	if ( 0 == Res )
	{
		Error.Format(_T("Error, Write File Fault\n%s"), LocalPCBSerialFilename);
		SetErrorString(Error);
		return false;
	}	
	SetNPM_DateTime(_T(""), LaneID);
	AddNPM_BackupFilename(PCBSerialFilename, LaneID);
	AddNPM_DeleteFilename(PCBSerialFilename, LaneID);	
#endif//M2M_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecNPM_LoadPCBSerialFile(LANE_ID LaneID)//執行載入NPM的PCBSerial檔案
{
#ifndef M2M_DISABLE
	if ( GetNPM_FuncBypass() == true ) { return true; }
	SetNPM_PCBSerialName(LaneID, _T(""));
	CString Error;
	CString LocalPCBSerialFilename=GetNPM_LocalPCBSerialName(LaneID);
	FILE *pfile=::_tfopen(LocalPCBSerialFilename, _T("r"));
	if ( NULL == pfile )
	{	
		Error.Format(_T("Error, Open File Fault\n%s"), LocalPCBSerialFilename);
		SetErrorString(Error);
		return false;
	}

	bool IsOK = true;	
	const size_t TextSize=256;	
#ifndef _UNICODE
	char Buffer[TextSize]="";
	if ( ::fgets(Buffer, TextSize, pfile) == NULL )
	{	IsOK = false; }
#else
	wchar_t Buffer[TextSize]=L"";
	if ( ::fgetws(Buffer, TextSize, pfile) == NULL )
	{	IsOK = false; }
#endif//_UNICODE
	::fclose(pfile); pfile=NULL;

	if ( false == IsOK )
	{
		Error.Format(_T("Error, Read File Fault\n%s"), LocalPCBSerialFilename);
		SetErrorString(Error);
		return false;
	}

	CString Filename=Buffer;

	//濾出PCBSerial
	const int Pos1=Filename.Find(_T('_'), 0)+1;
	const int Pos2=Filename.Find(_T('.'), 0);
	CString PCBSerial = Filename.Mid(Pos1, Pos2-Pos1);
	if ( PCBSerial.GetLength() != 10 )
	{
		Error.Format(_T("Error, Decode PCBSerial Fault\n%s"), Filename);
		SetErrorString(Error);
		return false;
	}
	SetNPM_PCBSerialName(LaneID, PCBSerial);
#endif//M2M_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecNPM_LoadAPC_FF1_File(LANE_ID LaneID)//執行載入NPM的APC-FF1檔案
{
#ifndef M2M_DISABLE
	if ( GetNPM_FuncBypass() == true ) { return true; }
	if ( GetNPM_APC_FF1_Enable() == false ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	//PCBSerial.apc2
	CString Error;
	CString Filename;
	CString FileFolder=GetNPM_SrcShareFolder(LaneID);
	CString PCBSerialName=GetNPM_PCBSerialName(LaneID);
	Filename.Format(_T("%s\\%s.%s"), FileFolder, PCBSerialName, _T("apc2"));

	const size_t textlen=256;	
	CString Section, KeyName, Default;
	TCHAR String[textlen]=_T("");

	ProjectPtr->ResetProjectComponentNPM_APC_FF1();

	Section = _T("Index");

	KeyName = _T("Format"); Default=_T("");//Should Be "APC-AOI";
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Version"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Machine"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Date"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("AuthorType"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Author"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }	
	
	CJetTxtFile TextFile;
	if ( TextFile.LoadTxtFile(Filename) == false )
	{
		Error = TextFile.GetErrorString();
		SetErrorString(Error);
		return false;
	}

	std::string Buf;
	if ( TextFile.GetTxtData(Buf) == false )
	{
		Error = TextFile.GetErrorString();
		SetErrorString(Error);
		return false;
	}
	size_t Pos=0;	
	bool bSectionString=false;
	bool bStartReadPosOffsetAOI=false;
	std::string Str, Name, Value;
	std::vector<std::string> strList;
	while ( true )
	{
		if ( JetAPI::ReadNextString(Buf, Pos, Str) == false )
		{	break;	}
		bSectionString = JetAPI::IsIniSection(Str.c_str());
		if ( true == bSectionString )
		{
			bStartReadPosOffsetAOI = false;
			if ( Str == "[PosOffsetAOI]")
			{	bStartReadPosOffsetAOI = true; }
			continue;
		}
		
		if ( true == bStartReadPosOffsetAOI )
		{
			strList.clear();
			JetAPI::ListSubString(Str, ' ', strList);
			const size_t strCount=strList.size();
			if ( 6 == strCount )
			{					
				Str = strList[0];
				if ( "IDNUM" == Str)
				{	continue;	}
				CString CName;
				unsigned int IDNUM=0, PNUM=0;
				double MffX=0.0, MffY=0.0, MffA=0.0;
				//IDNUM PNUM CName MffX MffY MffA=>Idx, Board, Part, OffsetX-mm, OffsetY-mm, Skew-Deg				
				for ( size_t i=0; i<strCount; i++ )
				{	
					Str = strList[i];
					switch ( i )
					{
					case 0://IDNUM->Idx
						IDNUM = ::atoi(Str.c_str());
						break;
					case 1://PNUM->Board
						PNUM = ::atoi(Str.c_str());
						break;
					case 2://CName->Part
						JetAPI::RemoveChar('"', Str);
						CName=Str.c_str();
						break;
					case 3://MffX->OffsetX-mm
						MffX = ::atof(Str.c_str());
						break;
					case 4://MffY->OffsetY-mm
						MffY = ::atof(Str.c_str());
						break;
					case 5://MffA->Skew-Deg
						MffA = ::atof(Str.c_str());
						break;
					}			
				}
				ProjectPtr->SetProjectComponentNPM_APC_FF1(IDNUM, PNUM, CName, MffX, MffY, MffA);
			}
		}
	};
	AddNPM_BackupFilename(Filename, LaneID);
	AddNPM_DeleteFilename(Filename, LaneID);
#endif//M2M_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecNPM_LoadAPC_FF2_File(LANE_ID LaneID)//執行載入NPM的APC-FF2檔案
{
#ifndef M2M_DISABLE
	if ( GetNPM_FuncBypass() == true ) { return true; }
	if ( GetNPM_APC_FF2_Enable() == false ) { return true; }
	//PCBSerial.badblockAPC
	CString Error;
	CString Filename;
	CString FileFolder=GetNPM_SrcShareFolder(LaneID);
	CString PCBSerialName=GetNPM_PCBSerialName(LaneID);
	Filename.Format(_T("%s\\%s.%s"), FileFolder, PCBSerialName, _T("badblockAPC"));

	const size_t textlen=256;	
	CString Section, KeyName, Default;
	TCHAR String[textlen]=_T("");

	Section = _T("Index");

	KeyName = _T("Format"); Default=_T("");//Should Be "BADBLOCKAPC";
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Version"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Machine"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Date"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("AuthorType"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Author"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }
	
	
	CJetTxtFile TextFile;
	if ( TextFile.LoadTxtFile(Filename) == false )
	{
		Error = TextFile.GetErrorString();
		SetErrorString(Error);
		return false;
	}

	std::string Buf;
	if ( TextFile.GetTxtData(Buf) == false )
	{
		Error = TextFile.GetErrorString();
		SetErrorString(Error);
		return false;
	}
	size_t Pos=0;	
	size_t nRow=0, nCol=0, nIdx=0;
	std::string ColCode;
	bool bSectionString=false;
	bool bStartBadBlockInfo=false;
	std::string Str, Name, Value;	
	while ( true )
	{
		if ( JetAPI::ReadNextString(Buf, Pos, Str) == false )
		{	break;	}
		bSectionString = JetAPI::IsIniSection(Str.c_str());
		if ( true == bSectionString )
		{
			bStartBadBlockInfo = false;
			if ( Str == "[BadBlockInfo]")
			{	bStartBadBlockInfo = true; }
			continue;
		}
		
		if ( true == bStartBadBlockInfo )
		{
			//BadBlockInfoC=010A, 
			//BadBlockInfo?=?表示第幾列(Row), A=1, B=2, C=3,...Z=256, 2A, 2B, 2C 每一列有256個Block
			//讀入後[->010A], 剔除最左側0[->10A], 再反轉字元[->A01], 再由16進位轉成2進位[A01=1010 0000 0001]
			if ( JetAPI::DecoderNPM_BadBlockText(Str, nRow, ColCode) == false )
			{	continue;	}
			size_t idx=0;
			const size_t ColCodeCount=ColCode.length();
			for ( size_t i=0; i<ColCodeCount; i++ )
			{
				idx = ColCodeCount-i-1;
				if ( '0' == ColCode[idx] ) { continue; }
				nCol = i+1;
				nIdx = ((nRow-1)*256)+nCol;
			}
		}
	};	
	AddNPM_BackupFilename(Filename, LaneID);
	AddNPM_DeleteFilename(Filename, LaneID);
#endif//M2M_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecNPM_LoadAPC_MFB_File(LANE_ID LaneID)//執行載入NPM的APC-MFB檔案
{
#ifndef M2M_DISABLE
	if ( GetNPM_FuncBypass() == true ) { return true; }
	if ( GetNPM_APC_MFB_Enable() == false ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	//PCBSerial.apc3
	CString Error;
	CString Filename;
	CString FileFolder=GetNPM_SrcShareFolder(LaneID);
	CString PCBSerialName=GetNPM_PCBSerialName(LaneID);
	Filename.Format(_T("%s\\%s.%s"), FileFolder, PCBSerialName, _T("apc3"));
	
	const size_t textlen=256;	
	CString Section, KeyName, Default;
	TCHAR String[textlen]=_T("");

	ProjectPtr->ResetProjectComponentNPM_APC_MFB();

	Section = _T("Index");

	KeyName = _T("Format"); Default=_T("");//Should Be "ELECPOSAPC";
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Version"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Machine"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Date"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("AuthorType"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Author"); Default=_T("");
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == false )
	{	return false; }
	
	
	CJetTxtFile TextFile;
	if ( TextFile.LoadTxtFile(Filename) == false )
	{
		Error = TextFile.GetErrorString();
		SetErrorString(Error);
		return false;
	}

	std::string Buf;
	if ( TextFile.GetTxtData(Buf) == false )
	{
		Error = TextFile.GetErrorString();
		SetErrorString(Error);
		return false;
	}
	size_t Pos=0;	
	bool bSectionString=false;
	bool bStartReadElecPosOffsetAOI=false;
	std::string Str, Name, Value;
	std::vector<std::string> strList;
	while ( true )
	{
		if ( JetAPI::ReadNextString(Buf, Pos, Str) == false )
		{	break;	}
		bSectionString = JetAPI::IsIniSection(Str.c_str());
		if ( true == bSectionString )
		{
			bStartReadElecPosOffsetAOI = false;
			if ( Str == "[ElecPosOffsetAOI]")
			{	bStartReadElecPosOffsetAOI = true; }
			continue;
		}
		
		if ( true == bStartReadElecPosOffsetAOI )
		{
			strList.clear();
			JetAPI::ListSubString(Str, ' ', strList);
			const size_t strCount=strList.size();
			if ( 6 == strCount )
			{	
				Str = strList[0];
				if ( "IDNUM" == Str)
				{	continue;	}				
				CString CName;
				unsigned int IDNUM=0, PNUM=0;
				double EPosX=0.0, EPosY=0.0, EPosA=0.0;
				//IDNUM PNUM CName EPosX EPosY EPosA=>Idx, Board, Part, OffsetX-mm, OffsetY-mm, Skew-Deg
				for ( size_t i=0; i<strCount; i++ )
				{	
					Str = strList[i];
					switch ( i )
					{
					case 0://IDNUM->Idx
						IDNUM = ::atoi(Str.c_str());
						break;
					case 1://PNUM->Board
						PNUM = ::atoi(Str.c_str());
						break;
					case 2://CName->Part
						JetAPI::RemoveChar('"', Str);
						CName=Str.c_str();
						break;
					case 3://EPosX->OffsetX-mm
						EPosX = ::atof(Str.c_str());
						break;
					case 4://EPosY->OffsetY-mm
						EPosY = ::atof(Str.c_str());
						break;
					case 5://EPosA->Skew-Deg
						EPosA = ::atof(Str.c_str());
						break;
					}			
				}
				ProjectPtr->SetProjectComponentNPM_APC_MFB(IDNUM, PNUM, CName, EPosX, EPosY, EPosA);
			}
		}
	};
	AddNPM_BackupFilename(Filename, LaneID);
	AddNPM_DeleteFilename(Filename, LaneID);
#endif//M2M_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecNPM_SaveAOILog1File(CAOIProject *ProjectPtr)//執行儲存NPM的AOI-訊息檔案-1
{
#ifndef M2M_DISABLE
	if ( GetNPM_FuncBypass() == true ) { return true; }
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	//Inspection lane number.aoipos
	CString Error;
	CString Filename, DstName;
	const bool bUsedMFB2=false;	
	LANE_ID LaneID=ProjectPtr->GetProjectActLaneID();
	CString LaneName=GetNPM_LaneName(LaneID);
	CString FileFolder=GetNPM_DstShareFolder(LaneID);	
	const TTestResult &TestReuslt=ProjectPtr->GetProjectResultCurrent();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	
	SetNPM_DateTime(TestReuslt.sDateTimeS, LaneID);	
	Filename.Format(_T("%s\\%s.%s"), FileFolder, LaneName, _T("aoipos"));		
	Filename.Format(_T("%s\\%s.%s"), FileFolder, LaneName, _T("Tmp"));	
	
	//Sort By IDNUM		
	std::vector<CSortObj> SortList;
	if ( ProjectPtr->BuildProjectComponentNPM_APC_List(SortList) == false )
	{
		SetErrorString(ProjectPtr->GetErrorString());
		return false; 
	}
	const size_t SortCount=SortList.size();

	CString Section, KeyName, String;
	Section = _T("Index");

	KeyName = _T("Format"); String=_T("MFBStdPartsPos");//Should Be "MFBStdPartsPos";
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Version"); String=_T("1.00");//MFB:1.00, MFB2:2.00
	if ( true == bUsedMFB2 ) { String=_T("2.00"); }
	else { String=_T("1.00"); }
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Machine"); String=_T("NPM");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Date"); String=_T("2021/11/16, 14:12:23");
	String=TestReuslt.sDateTimeS.Format(_T("%Y/%m/%d, %H:%M:%S"));
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("AuthorType"); String=_T("Machine");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Author"); String=AOI3D_APP_NAME;//_T("JET8000");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	Section = _T("SettingInfo");
	KeyName = _T("CertifiedMc"); String=_T("YES");//YES, NO
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("McOption"); String=_T("ON");//ON, OFF
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("MFB");
	if ( GetNPM_APC_MFB_Enable() == true ) { String=_T("ON"); }
	else { String=_T("OFF"); }
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("FF1");
	if ( GetNPM_APC_FF1_Enable() == true ) { String=_T("ON"); }
	else { String=_T("OFF"); }
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("FF2");
	if ( GetNPM_APC_FF2_Enable() == true ) { String=_T("ON"); }
	else { String=_T("OFF"); }
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }
	
	KeyName = _T("FiducialMode"); String=_T("ON");//ON, OFF
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Bcd");
	if ( GetNPM_BarcodeEnable() == true ) { String=_T("ON"); }
	else { String=_T("OFF"); }
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	//[MFBStandardPositionData]	
	const size_t BufferSize=256;
	char CName[BufferSize]="";
	char OutputStr[BufferSize]="";	
	FILE *pfile=::_tfopen(Filename, _T("a+"));	
	if ( NULL == pfile )
	{
		Error.Format(_T("Error, Open File Fault\n%s"), Filename);
		SetErrorString(Error);
		return false;
	}
#ifndef _X64
	::fseek(pfile, 0, SEEK_END);
#else
	::_fseeki64(pfile, 0, SEEK_END);
#endif//_X64
	Section = _T("MFBStandardPositionData");	
	::fprintf(pfile, "[MFBStandardPositionData]\n");

	//IDNUM PNUM CName MountX MountY MountA=>MFB
	//IDNUM PNUM CName MountX MountY MountA MfbMode=>MFB2
	unsigned int IDNUM=0;
	for ( size_t i=0; i<SortCount; i++ )
	{
		IDNUM = SortList[i].GetID();
		CAOIComponent *ComponentPtr=(CAOIComponent*)(SortList[i].GetPtr());
		if ( NULL == ComponentPtr ) { continue; }

		int MfbMode=1;		
		double MountX=0.0, MountY=0.0, MountA=0.0;//mm
		JetAPI::TCHAR2char(ComponentPtr->GetComponentName(), CName, BufferSize);
		unsigned int BoardIndex=ComponentPtr->GetComponentBoardIndex_Project();

		MountX = ComponentPtr->GetComponentCadPosX();
		MountY = ComponentPtr->GetComponentCadPosY();
		MountA = ComponentPtr->GetComponentAngle();

		if ( false == bUsedMFB2 )
		{	::sprintf(OutputStr, "%u %u %s %.3f %.3f %.3f", IDNUM, BoardIndex+1, CName, MountX, MountY, MountA);	}
		else
		{	::sprintf(OutputStr, "%u %u %s %.3f %.3f %.3f %d", IDNUM, BoardIndex+1, CName, MountX, MountY, MountA, MfbMode); }		
		::fprintf(pfile, "%s\n", OutputStr);
	}	
	::fclose(pfile); pfile=NULL;
	
	DstName.Format(_T("%s\\%s.%s"), FileFolder, LaneName, _T("aoipos"));	
	if ( ::CopyFile(Filename, DstName, FALSE) == FALSE )
	{
		Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), Filename, DstName);
		SetErrorString(Error);
		return false;
	}
	
	//For Backup File
	DstName.Format(_T("%s\\%s.%s"), GetAOITempDirectory(), LaneName, _T("aoipos"));	
	if ( ::CopyFile(Filename, DstName, FALSE) == FALSE )
	{
		Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), Filename, DstName);
		SetErrorString(Error);
		return false;
	}
	::DeleteFile(Filename);
	AddNPM_BackupFilename(DstName, LaneID);
	AddNPM_DeleteFilename(DstName, LaneID);
#endif//M2M_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecNPM_SaveAOILog2File(CAOIProject *ProjectPtr)//執行儲存NPM的AOI-訊息檔案-2
{
#ifndef M2M_DISABLE
	if ( GetNPM_FuncBypass() == true ) { return true; }
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }
	if ( GetNPM_APC_FF1_Enable()==false && GetNPM_APC_MFB_Enable() == false ) { return true; }

	//PCBSerial.aoiFF
	CString Error;
	CString Filename, DstName;
	const bool bUsedMFB2=false;	
	LANE_ID LaneID=ProjectPtr->GetProjectActLaneID();
	CString FileFolder=GetNPM_DstShareFolder(LaneID);
	CString PCBSerialName=GetNPM_PCBSerialName(LaneID);
	const TTestResult &TestReuslt=ProjectPtr->GetProjectResultCurrent();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();

	SetNPM_DateTime(TestReuslt.sDateTimeS, LaneID);
	Filename.Format(_T("%s\\%s.%s"), FileFolder, PCBSerialName, _T("aoimff"));
	Filename.Format(_T("%s\\%s.%s"), FileFolder, PCBSerialName, _T("Tmp"));

	//Sort By IDNUM		
	std::vector<CSortObj> SortList;
	if ( ProjectPtr->BuildProjectComponentNPM_APC_List(SortList) == false )
	{
		SetErrorString(ProjectPtr->GetErrorString());
		return false; 
	}
	const size_t SortCount=SortList.size();

	CString Section, KeyName, String;
	Section = _T("Index");

	KeyName = _T("Format"); String=_T("MFFOffset");//Should Be "MFFOffset";
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Version"); String=_T("1.00");//MFB:1.00, MFB2:2.00
	if ( true == bUsedMFB2 ) { String=_T("2.00"); }
	else { String=_T("1.00"); }
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Machine"); String=_T("NPM");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Date"); String=_T("2021/11/16, 14:12:23");
	String=TestReuslt.sDateTimeS.Format(_T("%Y/%m/%d, %H:%M:%S"));
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("AuthorType"); String=_T("Machine");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Author"); String=AOI3D_APP_NAME;//_T("JET8000");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	//[MFBStandardPositionOffset]	
	const size_t BufferSize=256;	
	char OutputStr[BufferSize]="";	
	FILE *pfile=::_tfopen(Filename, _T("a+"));	
	if ( NULL == pfile )
	{
		Error.Format(_T("Error, Open File Fault\n%s"), Filename);
		SetErrorString(Error);
		return false;
	}
#ifndef _X64
	::fseek(pfile, 0, SEEK_END);
#else
	::_fseeki64(pfile, 0, SEEK_END);
#endif//_X64
	Section = _T("MFBStandardPositionOffset");	
	::fprintf(pfile, "[MFBStandardPositionOffset]\n");

	//IDNUM MffX MffY MffA=>MFB
	//IDNUM MffX MffY MffA EPosX EPosY EPosA=>MFB2
	unsigned int IDNUM=0;
	for ( size_t i=0; i<SortCount; i++ )
	{
		IDNUM = SortList[i].GetID();
		CAOIComponent *ComponentPtr=(CAOIComponent*)(SortList[i].GetPtr());
		if ( NULL == ComponentPtr ) { continue; }
		
		double MffX=0.0, MffY=0.0, MffA=0.0;//mm
		double EPosX=0.0, EPosY=0.0, EPosA=0.0;//mm

		MffX = ComponentPtr->GetComponentNPM_APC_MffX();
		MffY = ComponentPtr->GetComponentNPM_APC_MffY();
		MffA = ComponentPtr->GetComponentNPM_APC_MffA();

		EPosX = ComponentPtr->GetComponentNPM_APC_EPosX();
		EPosY = ComponentPtr->GetComponentNPM_APC_EPosY();
		EPosA = ComponentPtr->GetComponentNPM_APC_EPosA();
		if ( false == bUsedMFB2 )
		{	::sprintf(OutputStr, "%u %.3f %.3f %.3f", IDNUM, MffX, MffY, MffA);	}
		else
		{	::sprintf(OutputStr, "%u %.3f %.3f %.3f %.3f %.3f %.3f", IDNUM, MffX, MffY, MffA, EPosX, EPosY, EPosA); }		
		::fprintf(pfile, "%s\n", OutputStr);
	}	
	::fclose(pfile); pfile=NULL;	
	
	DstName.Format(_T("%s\\%s.%s"), FileFolder, PCBSerialName, _T("aoiFF"));
	if ( ::CopyFile(Filename, DstName, FALSE) == FALSE )
	{
		Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), Filename, DstName);
		SetErrorString(Error);
		return false;
	}
	//For Backup File
	DstName.Format(_T("%s\\%s.%s"), GetAOITempDirectory(), PCBSerialName, _T("aoiFF"));
	if ( ::CopyFile(Filename, DstName, FALSE) == FALSE )
	{
		Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), Filename, DstName);
		SetErrorString(Error);
		return false;
	}
	::DeleteFile(Filename);
	AddNPM_BackupFilename(DstName, LaneID);
	AddNPM_DeleteFilename(DstName, LaneID);
#endif//M2M_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecNPM_SaveAOIInspectionResultFile(CAOIProject *ProjectPtr)//執行儲存NPM的AOI檢測結果檔案
{
#ifndef M2M_DISABLE
	if ( GetNPM_FuncBypass() == true ) { return true; }
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	//lane number_PCBSerial.rst2==L1_3000000000.rst2
	CString Error;
	CString Filename, DstName;
	const bool bUsedMFB2=false;	
	LANE_ID LaneID=ProjectPtr->GetProjectActLaneID();
	CString LaneName=GetNPM_LaneName(LaneID);
	CString FileFolder=GetNPM_DstShareFolder(LaneID);
	CString PCBSerialName=GetNPM_PCBSerialName(LaneID);	
	const TTestResult &TestReuslt=ProjectPtr->GetProjectResultCurrent();
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();

	SetNPM_DateTime(TestReuslt.sDateTimeS, LaneID);
	Filename.Format(_T("%s\\%s_%s.%s"), FileFolder, LaneName, PCBSerialName, _T("rst2"));
	Filename.Format(_T("%s\\%s_%s.%s"), FileFolder, LaneName, PCBSerialName, _T("Tmp"));

	//Sort By IDNUM		
	std::vector<CSortObj> SortList;
	if ( ProjectPtr->BuildProjectComponentNPM_APC_List(SortList) == false )
	{
		SetErrorString(ProjectPtr->GetErrorString());
		return false; 
	}
	const size_t SortCount=SortList.size();

	CString Section, KeyName, String;
	Section = _T("Index");

	KeyName = _T("Format"); String=_T("MFBPartsPosGap");//Should Be "MFBPartsPosGap";
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Version"); String=_T("1.00");//MFB:1.00, MFB2:2.00
	if ( true == bUsedMFB2 ) { String=_T("2.00"); }
	else { String=_T("1.00"); }
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Machine"); String=_T("NPM");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Date"); String=_T("2021/11/16, 14:12:23");
	String=TestReuslt.sDateTimeS.Format(_T("%Y/%m/%d, %H:%M:%S"));
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("AuthorType"); String=_T("Machine");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	KeyName = _T("Author"); String=AOI3D_APP_NAME;//_T("JET8000");
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	//[MFBBoardData]
	//NgCode=0
	Section = _T("MFBBoardData");
	KeyName = _T("NgCode");
	switch ( TestReuslt.sResultID )
	{
	case TEST_RESULT_OK: String = _T("0"); break;
	case TEST_RESULT_NG: String = _T("1"); break;
	default:			 String = _T("2"); break;		
	}
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }
	
	//BarcodeJdg=0
	bool DefaultBarcode=false;
	KeyName = _T("BarcodeJdg");
	CString Barcode=ProjectPtr->GetProjectBarcode();
	if ( _T('J')!=Barcode[0] || _T('E')!=Barcode[1] || _T('T')!=Barcode[2] )
	{	DefaultBarcode = false;	}
	else
	{	DefaultBarcode = true;	}
	if ( ProjectPtr->GetProjectIsGetBarcode() == false )
	{	String = _T("2");	}
	else
	{
		if ( true == DefaultBarcode )
		{	String = _T("1");	}
		else
		{	String = _T("0");	}
	}	
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	//BarcodeStr
	KeyName = _T("BarcodeStr");
	String =  Barcode;
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }
	
	//PartsNum
	KeyName = _T("PartsNum");
	String.Format(_T("%d"), SortCount);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false; }

	//[MFBPartsPositionGap]
	const size_t BufferSize=256;
	char CName[BufferSize]="";
	char OutputStr[BufferSize]="";	
	FILE *pfile=::_tfopen(Filename, _T("a+"));	
	if ( NULL == pfile )
	{
		Error.Format(_T("Error, Open File Fault\n%s"), Filename);
		SetErrorString(Error);
		return false;
	}
#ifndef _X64
	::fseek(pfile, 0, SEEK_END);
#else
	::_fseeki64(pfile, 0, SEEK_END);
#endif//_X64
	Section = _T("MFBPartsPositionGap");	
	::fprintf(pfile, "[MFBPartsPositionGap]\n");
	

	//IDNUM PNUM CName GapX GapY GapA STS NgCode =>MFB
	//IDNUM PNUM CName GapX GapY GapA STS NgCode SX SY ST MesPos=>MFB2	
	unsigned int IDNUM=0;
	for ( size_t i=0; i<SortCount; i++ )
	{
		IDNUM = SortList[i].GetID();
		CAOIComponent *ComponentPtr=(CAOIComponent*)(SortList[i].GetPtr());
		if ( NULL == ComponentPtr ) { continue; }

		TNPM_APC_Result apcRes;
		const int UseMesPos = 0;
		JetAPI::TCHAR2char(ComponentPtr->GetComponentName(), CName, BufferSize);
		unsigned int BoardIndex=ComponentPtr->GetComponentBoardIndex_Project();
		ComponentPtr->CalcComponentNPM_APC_Result(apcRes);

		if ( false == bUsedMFB2 )
		{	::sprintf(OutputStr, "%u %u %s %.4f %.4f %.4f %d %d", IDNUM, BoardIndex+1, CName, apcRes.dGapX, apcRes.dGapY, apcRes.dGapA, apcRes.nSTS, apcRes.nNgCode);	}
		else
		{	::sprintf(OutputStr, "%u %u %s %.4f %.4f %.4f %d %d %.3f %.3f %.3f %d", IDNUM, BoardIndex+1, CName, apcRes.dGapX, apcRes.dGapY, apcRes.dGapA, apcRes.nSTS, apcRes.nNgCode, apcRes.dSX, apcRes.dSY, apcRes.dST, UseMesPos); }		
		::fprintf(pfile, "%s\n", OutputStr);
	}	
	::fclose(pfile); pfile=NULL;

	DstName.Format(_T("%s\\%s_%s.%s"), FileFolder, LaneName, PCBSerialName, _T("rst2"));
	if ( ::CopyFile(Filename, DstName, FALSE) == FALSE )
	{
		Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), Filename, DstName);
		SetErrorString(Error);
		return false;
	}
	//For Backup File
	DstName.Format(_T("%s\\%s_%s.%s"), GetAOITempDirectory(), LaneName, PCBSerialName, _T("rst2"));
	if ( ::CopyFile(Filename, DstName, FALSE) == FALSE )
	{
		Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), Filename, DstName);
		SetErrorString(Error);
		return false;
	}
	::DeleteFile(Filename);
	AddNPM_BackupFilename(DstName, LaneID);
	AddNPM_DeleteFilename(DstName, LaneID);
#endif//M2M_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateHASI_MoniterThread()
{
#ifndef HASI_DISABLE
	if (DeleteHASI_MoniterThread() == false) { return false; }
	this->m_HASI_MonitorThreadCmd = THREAD_COMMAND_TO_IDLE;
	HASI_MoniterThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &HASI_MoniterThreadFn, (void*)0, NULL, &HASI_MoniterThreadID);
	if (NULL == HASI_MoniterThreadHandle)
	{
		this->m_HASI_MonitorThreadState = THREAD_STATE_NONE;
		this->m_ErrorString = _T("Error, Create Thread Fault (HASI_Moniter ThreadHandle == NULL)");
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}
	HASI_MoniterThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Online Tuning HASI_Moniter Event"));
	::SetThreadPriority(HASI_MoniterThreadEvent, THREAD_PRIORITY_BELOW_NORMAL);
	//::SetThreadAffinityMask(RemoveFolderThreadHandle, 0x03);//保留2個		
	//StartRemoveFolderThread(false);
	StartHAIS_MoniterThread(false);
#else
	HASI_MoniterThreadHandle = NULL;
	HASI_MoniterThreadEvent = NULL;
#endif//HASI_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteHASI_MoniterThread()
{
#ifndef HASI_DISABLE

	DWORD WaitTime = 10000;//1 sec	
	if (NULL == HASI_MoniterThreadHandle) { return true; }
	this->SetHASI_MonitorThreadCmd(THREAD_COMMAND_TO_EXIT);
	DWORD Res = ::WaitForSingleObject(HASI_MoniterThreadHandle, WaitTime);
	::CloseHandle(HASI_MoniterThreadHandle);
	HASI_MoniterThreadHandle = NULL;
	if (NULL != HASI_MoniterThreadEvent)
	{
		::CloseHandle(HASI_MoniterThreadEvent);
		HASI_MoniterThreadEvent = NULL;
	}
#endif//HASI_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartHAIS_MoniterThread(bool WaitOn)
{
#ifndef HASI_DISABLE
	size_t i = 0;
	if (NULL == HASI_MoniterThreadHandle) { return true; }
	if (NULL != HASI_MoniterThreadEvent)
	{
		::ResetEvent(HASI_MoniterThreadEvent);
	}
	if (THREAD_STATE_FINISH == m_HASI_MonitorThreadState)
	{
		SetHASI_MonitorThreadState(THREAD_STATE_IDLE);
	}
	SetHASI_MonitorThreadCmd(THREAD_COMMAND_TO_RUN);

	if (false == WaitOn)
	{
		return true;
	}
	if (WaitForHAIS_MoniterThreadStart() == false)
	{
		return false;
	}
#endif//HASI_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForHAIS_MoniterThreadStart()
{
#ifndef HASI_DISABLE
	size_t i = 0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 20;
	for (i = 0; i<MaxCount; i++)
	{
		if (THREAD_COMMAND_TO_RUN != m_HASI_MonitorThreadCmd)//切入下一個階段
		{
			break;
		}
		if (THREAD_STATE_IDLE != m_HASI_MonitorThreadState)
		{
			break;
		}
		::Sleep(SleepTime);
	}
	if (i == MaxCount)
	{
		this->m_ErrorString.Format(_T("Error, wait for Start HASI Monitor Thread too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);
		return false;
	}
#endif//HASI_DISABLE	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_MoniterFn()
{
	if (false == GetHASI_Enable()) { return true; }
#ifndef HASI_DISABLE
	THREAD_COMMAND_MODE    ThreadCmd;
	ThreadCmd = GetHASI_MonitorThreadCmd();
	if (THREAD_COMMAND_TO_IDLE == ThreadCmd) { return true; }
	if (THREAD_COMMAND_TO_EXIT == ThreadCmd) { return true; }
	if (THREAD_COMMAND_TO_NONE == ThreadCmd) { return true; }
	LockThreadHASI_Monitor();
	bool bFileAllRead = false;
	//ReadDirectoryChangesW
	while (false == bFileAllRead) {
		if (false == ExecHASI_LoadStateFile(bFileAllRead)) { 
			UnlockThreadHASI_Monitor();
			return false; 
		};
	}
	UnlockThreadHASI_Monitor();
#endif // !HASI_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetHASI_Enable() const
{
#ifndef HASI_DISABLE
	return m_SystemParameter.m_M2M_HASI_Enable;
#endif
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetHASI_SerialFileEnable() const
{
	return m_SystemParameter.m_M2M_HASI_SerialFile_Enable;
}
//-------------------------------------------------------------------------------------//
int CAOIDataCollect::GetHASI_SerialFileQueueSize() const
{
	return m_SystemParameter.m_M2M_HASI_SerialFile_QueueSize;
}
//-------------------------------------------------------------------------------------//
DWORD CAOIDataCollect::GetHASI_SerialFileDwellTime() const
{
	return  m_SystemParameter.m_M2M_HASI_SerialFile_DwellTime;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetHASI_SPIOffsetFileEnable() const
{
	return m_SystemParameter.m_M2M_HASI_SPIOffsetFile_Enable;
}
//-------------------------------------------------------------------------------------//
HASI_AOI_STAGE CAOIDataCollect::GetHASI_AOIStage() const
{
	return m_SystemParameter.m_M2M_HASI_AOI_Stage;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetHASI_PnPEnable() const
{
	return  m_SystemParameter.m_M2M_HASI_PNP_Enable;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataCollect::GetHASI_ShareFolder() const
{
	return m_SystemParameter.m_M2M_HASI_ShareFolder;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateHASI_ShareFolder()
{
#ifndef HASI_DISABLE
	if (false == GetHASI_Enable()) { return true; }
	CString Path = GetHASI_ShareFolder();
	std::vector<CString> folders = {
		_T("\\fromM2M"),
		_T("\\fromM2M\\M2MState"),
		_T("\\fromM2M\\SerialData"),
		_T("\\fromM2M\\SerialData\\FRONT"),
		_T("\\fromM2M\\SerialData\\REAR"),
		_T("\\fromM2M\\SPIOffsetData"),
		_T("\\fromM2M\\SPIOffsetData\\FRONT"),
		_T("\\fromM2M\\SPIOffsetData\\REAR"),
		_T("\\toPnP"),
		_T("\\toPnP\\AOIState"),
		_T("\\toPnP\\Inspect"),
		_T("\\toPnP\\Inspect\\Image"),
		_T("\\toPnP\\JobData"),
		_T("\\toPnP\\MonitoringImg"),
	};
	CString ErrorString;
	for (const CString folder : folders) {
		CString FullPath = Path + folder;
		if (!JetAPI::IsFolderExist(FullPath)) {
			if (!JetAPI::CreateFolder(FullPath)) {
				ErrorString.Format(_T("Create folder[%s] failed"), FullPath);
				SetErrorString(ErrorString);
			}
		}
	}
#endif//HASI_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SetHASI_PCBSerialPorperty(LANE_ID LaneID, std::vector<CString>& Fields, std::vector<CString>& Data)
{
	if (LANE_ID_B == LaneID) {
		if (false == m_HASI_PCBSerialProperty_LB.HASI_InitSerialFile(Fields, Data)) {
			return false;
		}
		return true;
	}
	if (false == m_HASI_PCBSerialProperty_LA.HASI_InitSerialFile(Fields, Data)) {
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
tHASIPCBSerial CAOIDataCollect::GetHASI_PCBSerialPorperty(LANE_ID LaneID)
{
	if (LANE_ID_B == LaneID) { return m_HASI_PCBSerialProperty_LB; }
	return m_HASI_PCBSerialProperty_LA;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ClearHASI_PCBSerialPorperty(LANE_ID LaneID)
{
	if (LANE_ID_B == LaneID) {
		m_HASI_PCBSerialProperty_LB.HASI_ClearSerialFile();
		return true;
	}
	m_HASI_PCBSerialProperty_LA.HASI_ClearSerialFile();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SetHASI_SPIFileStateMode(LANE_ID LaneID, HASI_STATE_MODE mode)
{
	//if (LANE_ID_B == LaneID) {
	//	m_HASI_StateMode_LB = mode;
	//	return true;
	//}
	//m_HASI_StateMode_LB = mode;
	//m_HASI_StateMode_LA = mode;
	m_SystemParameter.m_M2M_HASI_StateMode = mode;
	return true;
}
//-------------------------------------------------------------------------------------//
HASI_STATE_MODE CAOIDataCollect::GetHASI_SPIFileStateMode(LANE_ID LaneID)
{
	//if (LANE_ID_B == LaneID) {
	//	return m_HASI_StateMode_LB;
	//}
	//return m_HASI_StateMode_LA;
	return m_SystemParameter.m_M2M_HASI_StateMode;
}
//-------------------------------------------------------------------------------------//
//bool CAOIDataCollect::GetHASI_ProjectMinCad(CAOIProject * ProjectPtr, TPOINT2D &Cad)
//{
//	TPOINT2D ProjectMinCad = ProjectPtr->GetHASI_MinCadPos();
//	if ((INVALID_DOUBLE == ProjectMinCad.x) || (INVALID_DOUBLE == ProjectMinCad.y)) {
//		bool bUseTestSize = false;
//		TPOINT2D Res, MinCad;
//		TREGION4D CadRgn; TREGION4D StageRgn;
//		if (false == ProjectPtr->GetProjectMapInfo_DA(Res, CadRgn, StageRgn)) { return false; }
//		if (true == bUseTestSize) {
//			TPOINT2D CadCenterPoint;
//			CadCenterPoint.x = CadRgn.GetCpX();	CadCenterPoint.y = CadRgn.GetCpY();
//			double BoardSizeW = ProjectPtr->GetProjectParameter().m_TestSizeWidth;
//			double BoardSizeH = ProjectPtr->GetProjectParameter().m_TestSizeHeight;
//			MinCad.x = CadCenterPoint.x - 0.5*JetAPI::Unit_MMtoUM(BoardSizeW);
//			MinCad.y = CadCenterPoint.y - 0.5*JetAPI::Unit_MMtoUM(BoardSizeH);
//		}
//		else {
//			MinCad.x = CadRgn.minX;
//			MinCad.y = CadRgn.minY;
//		}
//		Cad = MinCad;
//		ProjectPtr->SetHASI_MinCadPos(Cad);
//	}
//
//		//else {
//		//	//偏差很大
//		//	TPOINT2D StageAPoint, StageBPoint, CadAPoint, CadBPoint;
//		//	CAOIPanel *PanelPtr = NULL;
//		//	PanelPtr = ProjectPtr->GetProjectPanelPtr(0, false);
//		//	if (NULL == PanelPtr) { return false; }
//		//	const DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
//		//	CMapCoordinate *STCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);
//		//	ProjectPtr->GetProjectMapLocStageRgn(StageRgn);
//		//	StageAPoint.x = StageRgn.maxX; StageAPoint.y = StageRgn.minY;
//		//	StageBPoint.x = StageRgn.minX; StageBPoint.y = StageRgn.maxY;
//		//	STCPtr->Map2D(StageAPoint.x, StageAPoint.y, CadAPoint.x, CadAPoint.y);
//		//	STCPtr->Map2D(StageBPoint.x, StageBPoint.y, CadBPoint.x, CadBPoint.y);
//		//	MinCad.x = MIN(CadAPoint.x, CadBPoint.x);
//		//	MinCad.y = MIN(CadAPoint.y, CadBPoint.y);
//		//}
//	else {
//		Cad = ProjectMinCad;
//	}
//	return true;
//}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_LoadStateFile(bool &bIsok)
{
	if (false == GetHASI_Enable()) { return true; }
	if (CreateHASI_ShareFolder() == false) {return false;}

	CString Error, FileFolder;
	CString ShareFolder = GetHASI_ShareFolder();
	FileFolder.Format(L"%s\\fromM2M\\M2MState", ShareFolder);
	CString ShareFileName;
	CString ShareFilePath;
	CString BackupFilePath;
	CString SearchPattern = FileFolder + L"\\*.ast";
	//-------------------------------------------------------------------------------------//
	//Find File and backup
	CFileFind Finder;
	BOOL bWorking = Finder.FindFile(SearchPattern);
	//CTime latestTime;
	CTime EarliestTime;
	bool bFound = false;
	bIsok = true;
	int nfile = 0;
	while (bWorking) {
		bWorking = Finder.FindNextFile();
		if (!Finder.IsDots() && !Finder.IsDirectory()) {
			CTime lastWriteTime;
			Finder.GetLastWriteTime(lastWriteTime);
			//從最舊的開始讀取
			if (!bFound || lastWriteTime < EarliestTime) {
				bIsok = false;
				EarliestTime = lastWriteTime;
				ShareFileName = Finder.GetFileName();
				ShareFilePath = Finder.GetFilePath();
				bFound = true;
			}
		}
	}
	if (false == bFound) {return true;}

	//For Backup File
	//BackupFilePath.Format(_T("%s\\%s"), GetAOITempDirectory(), ShareFileName);
	//if (::CopyFile(ShareFilePath, BackupFilePath, FALSE) == FALSE)
	//{
	//	Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), ShareFileName, BackupFilePath);
	//	SetErrorString(Error);
	//	return false;
	//}
	FILE *pfile = ::_tfopen(ShareFilePath, _T("rb"));
	if (NULL == pfile)
	{
		Error.Format(_T("Error, Open File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	CString Section, cSection;
	std::vector<CString> Fields, Data;
	//-------------------------------------------------------------------------------------//
	//[M2MSTATEDATA]
	cSection = _T("M2MSTATEDATA");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, State File file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[LINE]
	cSection = _T("LINE");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, State File file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[MACHINE]
	cSection = _T("MACHINE");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, State File file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[STATE]
	cSection = _T("STATE");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, State File file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	::fclose(pfile); pfile = NULL;
#ifndef _HANWHA_DEBUG
	::DeleteFile(ShareFilePath);
#endif // !_HANWHA_DEBUG
	//-------------------------------------------------------------------------------------//

	CString str;
	LANE_ID LaneID;
	HASI_STATE_MODE SPIFileState;
	TASK_STATE_MODE TaskStateMode;
	size_t index = std::find(Fields.begin(), Fields.end(), L"REQUEST") - Fields.begin();
	const size_t Req = _ttoi(Data[index]);
	index = std::find(Fields.begin(), Fields.end(), L"LANETYPE") - Fields.begin();

	str = Data[index];
	if (str == "FRONT") { LaneID = LANE_ID_A; }
	else { LaneID = LANE_ID_B; }

	switch (Req)
	{
	case HASI_STATE_REQUEST_JOB:
		TaskStateMode = GetOnlineTaskState();
		if (TASK_STATE_RUNNING != TaskStateMode) { break; }
		return ExecHASI_SaveAOIJobFile(LaneID, false);
	case HASI_STATE_REQUEST_MODE_TRANSFER:
		index = std::find(Fields.begin(), Fields.end(), L"MESSAGE") - Fields.begin();
		str = Data[index];
		if (str == L"STOP") { SPIFileState = HASI_STATE_MODE_STOP; }
		else if (str == L"SC") { SPIFileState = HASI_STATE_MODE_SC; }
		else if (str == L"AC") { SPIFileState = HASI_STATE_MODE_AC; }
		else if (str == L"SCAC") { SPIFileState = HASI_STATE_MODE_SCAC; }
		else if (str == L"RUN") { SPIFileState = HASI_STATE_MODE_RUN; }
		else {
			Error.Format(_T("Error, Undifined M2M State Message[%s]"), str);
			return false;
		}
		if (SPIFileState != GetHASI_SPIFileStateMode(LaneID)) {
			SetHASI_SPIFileStateMode(LaneID, SPIFileState);
			if (false == ExecHASI_SaveStateFile()) { return false; }
		}
		break;
	case HASI_STATE_REQUEST_M2M_USAGE:
	default:
		Error.Format(_T("Error, Undifined M2M State Request[%d]"), Req);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_LoadPCBSerialFile(LANE_ID LaneID)
{
	if (false == GetHASI_Enable()) { return true; }
	if (false == GetHASI_SerialFileEnable()) {
		ClearHASI_PCBSerialPorperty(LaneID);
		return true;
	}
	HASI_AOI_STAGE AOIStage = GetHASI_AOIStage();
	if (HASI_AOI_STAGE_POST == AOIStage) { return true; }

	CString LandType;
	CString Error, FileFolder;
	CString ShareFolder = GetHASI_ShareFolder();

	switch (LaneID)
	{
	case LANE_ID_A:	LandType = L"FRONT";	break;
	case LANE_ID_B:	LandType = L"REAR";		break;
	default:
		break;
	}
	FileFolder.Format(L"%s\\fromM2M\\SerialData\\%s", ShareFolder, LandType);

	//-------------------------------------------------------------------------------------//
	//Find File and backup
	CString ShareFileName;
	CString ShareFilePath;
	CString BackupFilePath;
	CString SearchPattern = FileFolder + L"\\*.srl";

	CFileFind Finder;
	bool bFind = false;
	const int QueueSize = GetHASI_SerialFileQueueSize();
	const int SecondIndex = QueueSize - 1;
	std::vector<std::pair<FILETIME, CString>> fileList;

	BOOL bWorking = Finder.FindFile(SearchPattern);
	while (true == bWorking) {
		bWorking = Finder.FindNextFile();
		if (!Finder.IsDots() && !Finder.IsDirectory()) {
			FILETIME currentTime;
			Finder.GetCreationTime(&currentTime);
			CString filePath = Finder.GetFilePath();
			fileList.push_back(std::make_pair(currentTime, filePath));
		}
	}
	if (false == fileList.empty()) {
		std::sort(fileList.begin(), fileList.end(),
			[](const auto& a, const auto& b) {
			return CompareFileTime(&a.first, &b.first) > 0; // 新的在前
		});
		// 保留最新兩個，刪掉其他
		if (fileList.size() > QueueSize) {
			for (size_t i = QueueSize; i < fileList.size(); ++i) {
				::DeleteFile(fileList[i].second);
			}
			fileList.erase(fileList.begin() + QueueSize, fileList.end());
		}
		
		ShareFilePath = fileList.back().second;
		ShareFileName = ShareFilePath.Mid(ShareFilePath.ReverseFind('\\') + 1);
		bFind = true;
	}
	if (false == bFind) {
		m_ErrorString.Format(_T("File not existed in SrcShareFolder[%s]."), FileFolder);
		return false;
	}

	FILE *pfile = ::_tfopen(ShareFilePath, _T("rb"));
	if (NULL == pfile)
	{
		Error.Format(_T("Error, Open File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	CString Section;
	std::vector<CString> Fields, Data;
	//-------------------------------------------------------------------------------------//
	//[BOARDSERIALDATA]
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (_T("BOARDSERIALDATA") != Section) {
		Error.Format(_T("Error, Serial file missing section[%s] \n%s"), _T("BOARDSERIALDATA"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[PCBDATA]
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (_T("PCBDATA") != Section) {
		Error.Format(_T("Error, Serial file missing section[%s] \n%s"), _T("BOARDSERIALDATA"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[LINE]
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (_T("LINE") != Section) {
		Error.Format(_T("Error, Serial file missing section[%s] \n%s"), _T("BOARDSERIALDATA"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	if (false == SetHASI_PCBSerialPorperty(LaneID, Fields, Data)) {
		Error = GetHASI_PCBSerialPorperty(LaneID).m_ErrorString;
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	::fclose(pfile); pfile = NULL;
	SetErrorString(_T(" "));
#ifndef _HANWHA_DEBUG
	::DeleteFile(ShareFilePath);
#endif // !_HANWHA_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_LoadSPIOffsetFile(CAOIProject *ProjectPtr)
{
	if (false == GetHASI_Enable()) { return true; }
	if (false == GetHASI_SPIOffsetFileEnable()) { return true; }
	HASI_AOI_STAGE AOIStage = GetHASI_AOIStage();
	if (HASI_AOI_STAGE_POST == AOIStage) { return true; }
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	if (HASI_STATE_MODE_AC == GetHASI_SPIFileStateMode(LaneID)) { return true; }

	CString Error;
	CString FileFolder;
	CString ShareFolder = GetHASI_ShareFolder();
	CString LandType;
	switch (LaneID)
	{
	case LANE_ID_A:	LandType = L"FRONT";	break;
	case LANE_ID_B:	LandType = L"REAR";		break;
	default:
		break;
	}
	FileFolder.Format(L"%s\\fromM2M\\SPIOffsetData\\%s", ShareFolder, LandType);

	CString ShareFileName;
	CString ShareFilePath;
	CString BackupFilePath;
	CString SearchPattern = FileFolder + L"\\*.sco";
	//-------------------------------------------------------------------------------------//
	//Find File and backup
	CFileFind Finder;
	BOOL bWorking = Finder.FindFile(SearchPattern);

	int nfile = 0;
	while (bWorking) {
		bWorking = Finder.FindNextFile();
		if (!Finder.IsDots() && !Finder.IsDirectory()) {
			nfile++;
			if (nfile>1) {
				m_ErrorString.Format(_T("Exist Multiple file in SrcShareFolder[%s]."), FileFolder);
				return false;
			}
			ShareFileName = Finder.GetFileName();
			ShareFilePath = Finder.GetFilePath();
		}
	}
	if (0 == nfile) {
		m_ErrorString.Format(_T("File not existed in SrcShareFolder[%s]."), FileFolder);
		return false;
	}

	//For Backup File
	//BackupFilePath.Format(_T("%s\\%s"), GetAOITempDirectory(), ShareFileName);
	//if (::CopyFile(ShareFilePath, BackupFilePath, FALSE) == FALSE)
	//{
	//	Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), ShareFileName, BackupFilePath);
	//	SetErrorString(Error);
	//	return false;
	//}
	FILE *pfile = ::_tfopen(ShareFilePath, _T("rb"));
	if (NULL == pfile)
	{
		Error.Format(_T("Error, Open File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	CString Section, cSection;
	std::vector<CString> Fields, Data;
	//-------------------------------------------------------------------------------------//
	//[SCOFFSETDATA]
	cSection = _T("SCOFFSETDATA");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, SPI offset file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[LINE]
	cSection = _T("LINE");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, SPI offset file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[PCBDATA]
	cSection = _T("PCBDATA");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, SPI offset file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//Data:BOARDID	SERIALNO	BARCODE;
	CString BarcodeSPI = Data[Data.size() - 1];
	CString BarcodeAOI = ProjectPtr->GetProjectBarcode();
#ifndef _HANWHA_DEBUG
	if (BarcodeAOI != BarcodeSPI) {
	::fclose(pfile); pfile = NULL;
	return true;
	}
#endif // !_HANWHA_DEBUG
	//-------------------------------------------------------------------------------------//
	//[BOARD]
	cSection = _T("BOARD");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, SPI offset file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[MODELMARK]
	cSection = _T("MODELMARK");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, SPI offset file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[STATE]
	cSection = _T("STATE");
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, Data)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, SPI offset file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[POINT]
	cSection = _T("POINT");
	std::vector<std::vector<CString>> SPIComponentData;
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	CAOIComponent *ComponentPtr = NULL, *ComponentAgentPtr = NULL;
	CAOIModel* ModelPtr = NULL;
	std::vector<CAOIModel*> ModelListPtr;
	CAOIWnd* WndPtr = NULL;
	CAOILand* LandPtr = NULL;
	CAOIBox* BoxPtr = NULL;

	double DeltaX, DeltaY, DeltaT;//DeltaT:※ No angles are used
	if (false == ExecHASI_LoadTableFile(pfile, Section, Fields, SPIComponentData)) {
		Error.Format(_T("Error, Read File Fault\n%s"), ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}
	if (cSection != Section) {
		Error.Format(_T("Error, SPI offset file missing section[%s] \n%s"), cSection, ShareFilePath);
		SetErrorString(Error);
		::fclose(pfile); pfile = NULL;
		return false;
	}

	size_t i, j, k;

	for (i = 0; i < ComponentCount; i++) {
		ModelListPtr.clear();
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if (NULL == ComponentPtr) { continue; }
		if (true == ComponentPtr->CheckComponentIsAgent()) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if (NULL == ModelPtr) { continue; }
		ModelListPtr.push_back(ModelPtr);

		if (false == ComponentPtr->GetComponentHASI_SPIOffset_Enable()) { continue; }
		const int ComponentUID = ComponentPtr->GetComponentUniqueID()+1;
		for (j = 0; j < SPIComponentData.size(); j++)
		{
			const int SPIComponentUID = _ttoi(SPIComponentData[j][HASI_SPI_OFFSET_POINT_POINTID]);
			if (SPIComponentUID > ComponentUID) {
				ComponentPtr->SetComponentHASI_SPIOffset_IsApplied(false);
				break;
			}
			if (ComponentUID != SPIComponentUID) { continue; }

			DeltaX = _tstof((SPIComponentData[j][HASI_SPI_OFFSET_POINT_DELTAX])) * 1000;// mm to micro m
			DeltaY = _tstof((SPIComponentData[j][HASI_SPI_OFFSET_POINT_DELTAY])) * 1000;
			ComponentPtr->SetComponentHASI_SPIOffset_IsApplied(true);
			break;
		}


		const int AgentCount = ComponentPtr->GetComponentAgentCount();
		for (j = 0; j < AgentCount; j++) {
			ComponentAgentPtr = ComponentPtr->GetComponentAgentPtr(j, false);
			if (NULL == ComponentAgentPtr) { continue; }
			ModelListPtr.push_back(ComponentAgentPtr->GetComponentModelPtr());
		}
		for (j = 0; j < ModelListPtr.size(); j++) {
			ModelPtr = ModelListPtr[j];
			if (NULL == ModelPtr) { continue; }
			//參考UpdateModelInspectionPosRes
			const size_t LandCount = ModelPtr->GetModelLandCount();
			const size_t WndCount = ModelPtr->GetModelWndCount();
			BoxPtr = ModelPtr->GetModelBodyBoxPtr();
			if (BoxPtr == NULL) { continue; }
			BoxPtr->MoveBoxRes(DeltaX, DeltaY);
			for (k = 0; k < LandCount; k++)
			{
				LandPtr = ModelPtr->GetModelLandPtr(k, false);
				if (NULL == LandPtr) { continue; }
				LandPtr->MoveLandLeadResult(DeltaX, DeltaY);
				//LandPtr->MoveLandPadResult(DeltaX, DeltaY);
				BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();
				if (NULL == BoxPtr) { continue; }
				BoxPtr->MoveBoxRes(DeltaX, DeltaY);
			}
			for (k = 0; k < WndCount; k++)
			{
				WndPtr = ModelPtr->GetModelWndPtr(k, false);
				if (NULL == WndPtr) { continue; }
				if (WndPtr->GetWndDefectID() == WND_DEFECT_PAD_ALIGN) { continue; }
				WndPtr->MoveWndResult(DeltaX, DeltaY);
			}
		}
	}
	::fclose(pfile); pfile = NULL;
#ifndef _HANWHA_DEBUG
	::DeleteFile(ShareFilePath);
#endif // !_HANWHA_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_LoadTableFile(FILE * pfile, CString & Section, std::vector<CString>& Fields, std::vector<CString>& Data)
{
	Section = _T("");
	if (pfile == nullptr) return false;

	// 讀取 Section 標頭
	wchar_t wline[512];
	memset(wline, 0, sizeof(wchar_t) * 512);
	if (fgetws(wline, sizeof(wline) / sizeof(wchar_t), pfile))
	{
		if (wline[0] == L'\n' || wline[0] == L'\r') {fgetws(wline, sizeof(wline) / sizeof(wchar_t), pfile);}
		if (wline[0] == L'[') {
			wchar_t* start = wcschr(wline, L'[');
			wchar_t* end = wcschr(wline, L']');
			if (start && end && end > start) {
				*end = L'\0';
				Section = CString(start + 1); // CString 可接 wchar_t
			}
		}
	}

	if (Section.IsEmpty()) return false;

	// 讀取 field line
	if (!fgetws(wline, sizeof(wline) / sizeof(wchar_t), pfile)) return false;
	{
		CString line(wline);
		line.TrimRight(L"\r\n");

		// 用 tab 分隔
		int pos = 0;
		CString token;
		while (!(token = line.Tokenize(L"\t", pos)).IsEmpty())
		{
			Fields.push_back(token);
		}
	}

	// 讀取 data line
	if (!fgetws(wline, sizeof(wline) / sizeof(wchar_t), pfile)) return false;
	{
		CString line(wline);
		line.TrimRight(L"\r\n");

		int pos = 0;
		CString token;
		while (!(token = line.Tokenize(L"\t", pos)).IsEmpty())
		{
			Data.push_back(token);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_LoadTableFile(FILE * pfile, CString & Section, std::vector<CString>& Fields, std::vector<std::vector<CString>>& Data)
{
	Section = _T("");
	if (pfile == nullptr) return false;

	wchar_t wline[512];

	// 讀取 Section 標頭
	while (fgetws(wline, sizeof(wline) / sizeof(wchar_t), pfile))
	{
		if (wline[0] == L'\n' || wline[0] == L'\r') { fgetws(wline, sizeof(wline) / sizeof(wchar_t), pfile); }
		if (wline[0] == L'[')  // 找到區段
		{
			wchar_t* start = wcschr(wline, L'[');
			wchar_t* end = wcschr(wline, L']');
			if (start && end && end > start)
			{
				*end = L'\0';
				Section = CString(start + 1); // CString 可直接接 wchar_t*
			}
			break;
		}
	}

	if (Section.IsEmpty()) return false;

	// 讀取 field line
	if (!fgetws(wline, sizeof(wline) / sizeof(wchar_t), pfile)) return false;
	{
		CString line(wline);
		line.TrimRight(L"\r\n");

		int pos = 0;
		CString token;
		while (!(token = line.Tokenize(L"\t", pos)).IsEmpty())
		{
			Fields.push_back(token);
		}
	}

	// 讀取 data line
	size_t index = 0;
	while (fgetws(wline, sizeof(wline) / sizeof(wchar_t), pfile))
	{
		CString line(wline);
		line.TrimRight(L"\r\n");
		if (line.IsEmpty()) continue;

		Data.push_back({});
		int pos = 0;
		CString token;
		while (!(token = line.Tokenize(L"\t", pos)).IsEmpty())
		{
			Data[index].push_back(token);
		}
		index++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_SaveStateFile()
{
	//Trigger timing:
	//	When the Inspector is Stopped and Run
	//	When the M2M usage status changes
	//	When a job(.jbc) is sent
	//if (CheckProjectPtr(ProjectPtr) == false) { return false; }
	//if (false == GetHASI_Enable()) { return true; }
	CString FuntionName = _T("ExecSaveStateFile_Hanwha");
	//LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	CString FileFolder;
	CString ShareFolder = GetHASI_ShareFolder();
	FileFolder.Format(L"%s\\toPnP\\AOIState", ShareFolder);
	////如果資料夾存在，才去修改StateFile
	if (!JetAPI::IsFolderExist(FileFolder)) { return true;	}
	CString str;
	//-------------------------------------------------------------------------------------//
	// AOIStageStr 根據AOI在產線上的位置，爐前:AOIB,爐後:SAOI
	CString AOIStageStr;
	HASI_AOI_STAGE AOIStage = GetHASI_AOIStage();

	switch (AOIStage)
	{
	case HASI_AOI_STAGE_PRE:
		AOIStageStr = _T("AOIB");
		break;
	case HASI_AOI_STAGE_POST:
		AOIStageStr = _T("SAOI");
		break;
	case HASI_AOI_STAGE_NONE:
		m_ErrorString.Format(_T("Error, Please select AOI Stage (Pre/Post reflow)"));
		return false;
	default:
		m_ErrorString.Format(_T("Error, Undifined AOI Stage occured in [%s]"), FuntionName);
		return false;
	}


	TCHAR   TMode[32] = _T("");
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);  // 取得當地時間（含毫秒）
	CString DateTime, Filename;
	DateTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d%.3d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds);
	Filename.Format(_T("%s\\%s_%s_%s.%s"), FileFolder, DateTime, _T("State"), AOIStageStr, _T("ast"));
	_tcscpy(TMode, _T("wb"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(Filename, TMode);
	if (NULL == pfile)
	{
		m_ErrorString.Format(_T("Error, Open Hanwha_State_AOIB to Save Fault (%s)"), Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[M2MSTATEDATA]
	CString Setion = _T("M2MSTATEDATA");
	CString Date, Time;
	Date.Format(_T("%.4d%.2d%.2d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay);
	Time.Format(_T("%.2d%.2d%.2d"), SysTime.wHour, SysTime.wMinute, SysTime.wSecond);

	std::vector<CString> Fields = { _T("DATE"),	_T("TIME") };
	std::vector<CString> Data = { Date, Time };

	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[LANETYPE]
	Setion = _T("LINE");
	Fields = { _T("LANETYPE") };
	//Information regarding the availability of M2M functions
	//is transmitted regardless of lane(MAOI->M2M), so leave this field blank.
	Data = { _T("") };

	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[MACHINE]
	Setion = _T("MACHINE");
	//MASTERSTATUS:MAOI Non-Use Information
	Fields = { _T("IPADDR"),_T("MASTERSTATUS") };
	CString IPAddress = m_SystemParameter.m_IPHostComputer;
	Data = { IPAddress, _T("") };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[STATE]
	Setion = _T("STATE");
	//M2MSTATE,INLINECALIBSTATUS,ERRORCODE :MAOI Non-Use Information
	//REQUEST: 14: Delivery of M2M	usage
	CString M2MEnable;
	if (true == GetHASI_Enable()) { M2MEnable = _T("M2MON"); }
	else { M2MEnable = _T("M2MOFF"); }
	Fields = { _T("M2MSTATE"),_T("INLINECALIBSTATUS"),_T("REQUEST"),_T("ERRORCODE"),_T("MESSAGE") };
	Data = { _T(""), _T(""), _T("14"), _T(""), M2MEnable };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		return false;
	}
	::fclose(pfile); pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_SaveAOIJobFile(LANE_ID LaneID, bool bDual)
{
	if (false == GetHASI_Enable()) { return true; }
	CAOIProject* ProjectPtr = NULL;
	size_t LaneProjectCount = 0;
	MULTI_LANE_MODE MultiLaneMode = CheckMultiLaneMode();
	if (true == bDual) {
		if (MULTI_LANE_2 == MultiLaneMode) {
			LaneProjectCount = GetLaneProjectCount(LANE_ID_A);
			for (size_t i = 0; i < LaneProjectCount; i++) {
				ProjectPtr = GetLaneProjectPtr(LANE_ID_A, i, false);
				if (CheckProjectPtr(ProjectPtr) == false) { continue; }
				if (false == ExecHASI_SaveAOIJobFile(ProjectPtr)) { return false; }
			}
			LaneProjectCount = GetLaneProjectCount(LANE_ID_B);
			for (size_t i = 0; i < LaneProjectCount; i++) {
				ProjectPtr = GetLaneProjectPtr(LANE_ID_B, i, false);
				if (CheckProjectPtr(ProjectPtr) == false) { continue; }
				if (false == ExecHASI_SaveAOIJobFile(ProjectPtr)) { return false; }
			}
			return true;
		}
	}
	
	if (MULTI_LANE_1 == MultiLaneMode) {
		//如果單軌則只回應有啟動的軌道
		LANE_ID LaneID_Active = GetActiveLaneID();
		if (LaneID_Active != LaneID) { return true; }
	}
	LaneProjectCount = GetLaneProjectCount(LaneID);
	for (size_t i = 0; i < LaneProjectCount; i++) {
		ProjectPtr = GetLaneProjectPtr(LaneID, i, false);
		if (CheckProjectPtr(ProjectPtr) == false) { continue; }
		if (false == ExecHASI_SaveAOIJobFile(ProjectPtr)) { return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_SaveAOIJobFile(CAOIProject * ProjectPtr)
{
	if (false == GetHASI_Enable()) { return true; }
	if (CheckProjectPtr(ProjectPtr) == false) { return false; }
	CString FuntionName = _T("SaveAOIJobFile_Hanwha");
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	CString FileFolder, str;
	//-------------------------------------------------------------------------------------//
	const int ComponentCount = ProjectPtr->GetProjectComponentCount();
	const size_t ProjectCount = GetLaneProjectCount(LaneID);
	CString ShareFolder = GetHASI_ShareFolder();
	FileFolder.Format(L"%s\\toPnP\\JobData", ShareFolder);
	//-------------------------------------------------------------------------------------//
	TCHAR   TMode[32] = _T("");
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);  // 取得當地時間（含毫秒）
	CString DateTime, Filename, AOIStageStr;
	HASI_AOI_STAGE AOIStage = GetHASI_AOIStage();
	switch (AOIStage)
	{
	case HASI_AOI_STAGE_PRE:	AOIStageStr = _T("AOIB");	break;
	case HASI_AOI_STAGE_POST:	AOIStageStr = _T("SAOI");	break;
	case HASI_AOI_STAGE_NONE:
		m_ErrorString.Format(_T("Error, Please select AOI Stage (Pre/Post reflow)"));
		return false;
	default:
		m_ErrorString.Format(_T("Error, Undifined AOI Stage occured in [%s]"), FuntionName);
		return false;
	}

	DateTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d%.3d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds);
	Filename.Format(_T("%s\\%s_%s_%s.%s"), FileFolder, DateTime, _T("Job"), AOIStageStr, _T("jbc"));
	_tcscpy(TMode, _T("wb"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(Filename, TMode);
	if (NULL == pfile)
	{
		m_ErrorString.Format(_T("Error, Open Hanwha_JOB_AOIB to Save Fault (%s)"), Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[AOIJOBDATA]
	CString Setion = _T("AOIJOBDATA");

	CString Date, Time;
	Date.Format(_T("%.4d%.2d%.2d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay);
	Time.Format(_T("%.2d%.2d%.2d"), SysTime.wHour, SysTime.wMinute, SysTime.wSecond);

	std::vector<CString> Fields = { _T("DATE"),	_T("TIME") };
	std::vector<CString> Data = { Date, Time };

	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[LINE]
	Setion = _T("LINE");
	Fields = { _T("LANETYPE"),	_T("MACHINETYPE") };
	CString LaneType, MachineType;
	switch (LaneID)
	{
	case LANE_ID_A:
		LaneType = _T("FRONT");
		break;
	case LANE_ID_B:
		LaneType = _T("REAR");
		break;
	case LANE_ID_NULL:
	case LANE_ID_BOTH:
	case LANE_ID_RETURN:
	default:
		m_ErrorString.Format(_T("Get Land type Failed[%s]"), FuntionName);
		return false;
		break;
	}
	MULTI_LANE_MODE LANE_MODE = CheckMultiLaneMode();
	switch (LANE_MODE)
	{
	case MULTI_LANE_1:	MachineType = _T("SINGLE");	break;
	case MULTI_LANE_2:	MachineType = _T("DUAL");	break;
	default:
		break;
	}
	Data = { LaneType, MachineType };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[PCBDATA]
	Setion = _T("PCBDATA");
	CString BoardID = ProjectPtr->GetProjectFileMainName();
	Fields = { _T("BOARDID") };
	Data = { BoardID };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[BOARD]
	//BOARDSTOP:虛擬原點
	//COORDINATE:坐標軸方向
	//後續的點位都會參考這兩個
	TPOINT2D Res, MinCad;
	double PosX, PosY, PosZ;
	TREGION4D CadRgn; TREGION4D StageRgn;
	CString ProjectSizeWidth, ProjectSizeHeight, ProjectCoor, ProjectStop;
	//if (false == ProjectPtr->GetProjectMapInfo_DA(Res, CadRgn, StageRgn)) {
	//	::fclose(pfile); pfile = NULL;
	//	::DeleteFile(Filename);
	//	return false;
	//}
	//if (false == GetHASI_ProjectMinCad(ProjectPtr, MinCad)) {
	//	::fclose(pfile); pfile = NULL;
	//	::DeleteFile(Filename);
	//}
	//MinCad.x = StageRgn.minX; MinCad.y = StageRgn.minY;
	MinCad.x = CadRgn.minX; MinCad.y = CadRgn.minY;
	const double ProjectSizeW = ProjectPtr->GetProjectParameter().m_TestSizeWidth;
	const double ProjectSizeH = ProjectPtr->GetProjectParameter().m_TestSizeHeight;
	Setion = _T("BOARD");
	Fields = { _T("X"),_T("Y"),_T("COORDINATE"),_T("BOARDSTOP") };
	ProjectSizeWidth.Format(L"%4.4f", ProjectSizeW);
	ProjectSizeHeight.Format(L"%4.4f", ProjectSizeH);
	Data = { ProjectSizeWidth, ProjectSizeHeight, _T("RIGHT-UP"),_T("LEFT-DOWN") };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[MODELMARK]
	//TYPE: Write by changing the work surface (TOP/BOT) value set in MAOI to number (1: TOP, 2: BOTTOM)
	//USE:Write whether to use mixed flow mode in MAOI(0 : Do not use mixed flow mod)
	Setion = _T("MODELMARK");
	CString WorkType, MixFlowMode;
	PANEL_SIDE_MODE Mode = ProjectPtr->GetProjectPanelSideMode();
	switch (Mode)
	{
	case PANEL_SIDE_TOP:	WorkType = _T("1");	break;
	case PANEL_SIDE_BOTTOM:	WorkType = _T("2");	break;
	case PANEL_SIDE_HYBRID:
		m_ErrorString.Format(_T("Error, [Panel side] Hybrid mode not available in M2M."));
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	case PANEL_SIDE_RETURN:
	default:
		m_ErrorString.Format(_T("Error, Undifined Panel side occured in [%s]"), FuntionName);
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	if (ProjectCount < 2) { MixFlowMode = _T("0"); }
	else if (ProjectCount == 2) { MixFlowMode = _T("1"); }
	else {
		m_ErrorString.Format(_T("Error, Invalided project count >2 in M2M "));
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	Fields = { _T("TYPE"),_T("USE"),_T("COLOR"),_T("POSX"),_T("POSY"),_T("SIZEX"),_T("SIZEY") };
	Data = { WorkType,MixFlowMode,_T(" "),_T(" "),_T(" "),_T(" "),_T(" ") };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[FIDUCIAL]
	Setion = _T("FIDUCIAL");
	//Simply write the original coordinates of all Fiducial Marks registered in AOI
	const int FdCount = ProjectPtr->GetProjectFdCount();
	CAOIFd* FdPtr = NULL;
	HASI_FieldMap Table;
	Fields.clear();
	Fields = { L"X", L"Y"};
	for (const auto& item : Fields) { Table[item] = std::vector<CString>(); }
	for (int i = 0; i < FdCount; i++) {
		FdPtr = ProjectPtr->GetProjectFdPtr(i, false);
		if (NULL == FdPtr) { continue; }
		PosX = FdPtr->GetFdSpecialCadPosX();
		PosY = FdPtr->GetFdSpecialCadPosY();
		if (INVALID_DOUBLE == PosX && INVALID_DOUBLE == PosY) {
			PosX = FdPtr->GetFdCadPosX() - MinCad.x;
			PosY = FdPtr->GetFdCadPosY() - MinCad.y;
		}
		//PosX = FdPtr->GetFdStagePosX() - MinCad.x;
		//PosY = FdPtr->GetFdStagePosY() - MinCad.y;
		PosX = JetAPI::Unit_UmtoMM(PosX);
		PosY = JetAPI::Unit_UmtoMM(PosY);
		str.Format(_T("%4.4f"), PosX);
		Table[_T("X")].push_back(str);
		str.Format(_T("%4.4f"), PosY);
		Table[_T("Y")].push_back(str);
	}
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Table)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[POINT]
	Setion = _T("POINT");
	HASI_FieldMap ComponentMap;
	Fields.clear();
	Fields = {
		_T("POINTID"),	_T("REF"),	_T("PART"),
		_T("POSX"),	_T("POSY"),	_T("POST"),	_T("POSZ"),	_T("SIZEX"), _T("SIZEY"), _T("SIZEZ"),
		_T("XLSL"),	_T("XUSL"), _T("YLSL"),	_T("YUSL"),
	};
	for (const auto& field : Fields) { ComponentMap[field] = std::vector<CString>(); }
	RESULT_ID ResultID;
	WND_DEFECT_ID    WndDefectID;
	CAOIWnd *WndPtr = NULL;
	CAOIModel* ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	double XLSL, XUSL, YLSL, YUSL, SizeZ;
	bool bUsed = false;
	for (int i = 0; i < ComponentCount; i++) {
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if (NULL == ComponentPtr) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if (NULL == ModelPtr) { continue; }
		const int WndCount = ModelPtr->GetModelWndOrderCount();
		XLSL = 0; XUSL = 0; YLSL = 0; YUSL = 0;

		SizeZ = INVALID_DOUBLE;
		for (int j = 0; j < WndCount; j++) {
			WndPtr = ModelPtr->GetModelWndOrderPtr(j, false);
			if (NULL == WndPtr) { continue; }
			WndDefectID = WndPtr->GetWndDefectID();
			if (WndPtr->GetWndEnabled() == false) { continue; }
			bUsed = false;
			CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
			ALG_TYPE  AlgType = AlgParam.GetAlgType();
			unsigned int FrameUniqueID = AlgParam.GetAlgImageBinParam().GetBinaryFrameUniqueID();
			if (WND_DEFECT_PART_ALIGN == WndDefectID) {
				switch (AlgType)
				{
				case ALG_MODEL_MATCH:
				case ALG_IMAGE_MATCH:
				case ALG_FD_MATCH:
				case ALG_EDGE_SEARCH:
					XLSL = JetAPI::Unit_UmtoMM(AlgParam.GetAlgOffsetXLSL());
					XUSL = JetAPI::Unit_UmtoMM(AlgParam.GetAlgOffsetXUSL());
					YLSL = JetAPI::Unit_UmtoMM(AlgParam.GetAlgOffsetYLSL());
					YUSL = JetAPI::Unit_UmtoMM(AlgParam.GetAlgOffsetYUSL());
				}
			}
			if (FRAME_UNIQUE_ID_DLP == FrameUniqueID) {
				if (INVALID_DOUBLE == SizeZ) { bUsed = true; }
				else { if (WND_DEFECT_BODY_MISSING == WndDefectID) { bUsed = true; } }
			}
			if (true == bUsed) {
				switch (AlgType)
				{
				case ALG_BRIGHT_RATIO:	SizeZ = AlgParam.GetAlgParamBrightRatio().brTargetValue;	break;
				case ALG_OBJECT_MEASURE:SizeZ = AlgParam.GetAlgParamObjectMeasure().omHeightSpec;	break;
				default:	continue;}
			}
		}
		if (INVALID_DOUBLE == SizeZ) { SizeZ = 0.0f; }
		else { SizeZ = JetAPI::Unit_UmtoMM(SizeZ); }//mm
		PosX = ComponentPtr->GetComponentSpecialCadPosX();
		PosY = ComponentPtr->GetComponentSpecialCadPosY();
		if (INVALID_DOUBLE == PosX && INVALID_DOUBLE == PosY) {
			PosX = ComponentPtr->GetComponentCadPosX() - MinCad.x;
			PosY = ComponentPtr->GetComponentCadPosY() - MinCad.y;
		}
		//PosX = ComponentPtr->GetComponentCadPosX() - MinCad.x;
		//PosY = ComponentPtr->GetComponentCadPosY() - MinCad.y;
		//PosX = ComponentPtr->GetComponentStagePosX() - MinCad.x;
		//PosY = ComponentPtr->GetComponentStagePosY() - MinCad.y;
		PosX = JetAPI::Unit_UmtoMM(PosX);
		PosY = JetAPI::Unit_UmtoMM(PosY);
		PosZ = 0.0f;

		str.Format(L"%d", ComponentPtr->GetComponentUniqueID()+1);
		ComponentMap[L"POINTID"].push_back(str);
		str.Format(L"%s", ComponentPtr->GetComponentName());
		ComponentMap[L"REF"].push_back(str);
		str.Format(L"%s", ComponentPtr->GetComponentPartNumber());
		ComponentMap[L"PART"].push_back(str);
		str.Format(L"%4.4f", PosX);//mm
		ComponentMap[L"POSX"].push_back(str);
		str.Format(L"%4.4f", PosY);//mm
		ComponentMap[L"POSY"].push_back(str);
		str.Format(L"%4.4f", ComponentPtr->GetComponentAngle());
		ComponentMap[L"POST"].push_back(str);
		str.Format(L"%4.4f", PosZ);
		ComponentMap[L"POSZ"].push_back(str);
		str.Format(L"%4.4f", JetAPI::Unit_UmtoMM(ComponentPtr->GetComponentBodySizeW()));//mm
		ComponentMap[L"SIZEX"].push_back(str);
		str.Format(L"%4.4f", JetAPI::Unit_UmtoMM(ComponentPtr->GetComponentBodySizeH()));//mm
		ComponentMap[L"SIZEY"].push_back(str);
		str.Format(L"%4.4f", SizeZ);//
		ComponentMap[L"SIZEZ"].push_back(str);
		str.Format(L"%4.4f", XLSL);
		ComponentMap[L"XLSL"].push_back(str);
		str.Format(L"%4.4f", XUSL);
		ComponentMap[L"XUSL"].push_back(str);
		str.Format(L"%4.4f", YLSL);
		ComponentMap[L"YLSL"].push_back(str);
		str.Format(L"%4.4f", YUSL);
		ComponentMap[L"YUSL"].push_back(str);
	}
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, ComponentMap)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	::fclose(pfile); pfile = NULL;
	if (false == ExecHASI_SaveStateFile()) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_SaveAOIInspectionResultFile(CAOIProject * ProjectPtr)
{
	if (false == GetHASI_Enable()) { return true; }
	if (CheckProjectPtr(ProjectPtr) == false) { return false; }
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	HASI_STATE_MODE HASI_SPIFileState = GetHASI_SPIFileStateMode(LaneID);
	if (HASI_STATE_MODE_STOP == HASI_SPIFileState) { return true; }
	
	CString FuntionName = _T("SaveAOIInspectionResultFile_Hanwha");
	//lane number_PCBSerial.rst2==L1_3000000000.rst2
	CString Error;
	CString Filename, DstName;
	

	CString FileFolder;
	CString ShareFolder = GetHASI_ShareFolder();
	FileFolder.Format(L"%s\\toPnP\\Inspect", ShareFolder);
	CString str;
	const TTestResult &TestReuslt = ProjectPtr->GetProjectResultCurrent();
	tHASIPCBSerial M2MHanwha = GetHASI_PCBSerialPorperty(LaneID);
	//-------------------------------------------------------------------------------------//
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);  // 取得當地時間（含毫秒）
	CString DateTime;
	DateTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d%.3d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds);

	//PCBSerialNo 從傳遞的文件取出
	CString PCBSerialNo = M2MHanwha.m_PCBSerialNo;
	if (PCBSerialNo.IsEmpty()) { PCBSerialNo.Format(_T("EmptySerial%s"), DateTime); }
	else { PCBSerialNo.Format(_T("%d"), _ttoi(PCBSerialNo)); }

	
	// AOIStageStr 根據AOI在產線上的位置，爐前:AOIB,爐後:SAOI
	CString AOIStageStr;
	HASI_AOI_STAGE AOIStage = GetHASI_AOIStage();
	switch (AOIStage)
	{
	case HASI_AOI_STAGE_PRE:	AOIStageStr = _T("AOIB");	break;
	case HASI_AOI_STAGE_POST:	AOIStageStr = _T("SAOI");	break;
	case HASI_AOI_STAGE_NONE:
		m_ErrorString.Format(_T("Error, Please select AOI Stage (Pre/Post reflow)"));
		return false;
	default:
		m_ErrorString.Format(_T("Error, Undifined AOI Stage occured in [%s]"), FuntionName);
		return false;
	}
	
	bool DefaultBarcode = false;
	CString BarcodeStr = ProjectPtr->GetProjectBarcode();
	if (_T('J') != BarcodeStr[0] || _T('E') != BarcodeStr[1] || _T('T') != BarcodeStr[2])
	{	DefaultBarcode = false;}
	else {	DefaultBarcode = true;	}
	//爐後沒有barcode設定，不輸出.omr file
	if (AOIStage == HASI_AOI_STAGE_POST && true == DefaultBarcode) { return true; }

	//Barcode is created by	excluding special characters and symbols from the entire string and 
	//using only English letters and numbers
	for (int i = 0; i < BarcodeStr.GetLength(); ++i) {
		TCHAR ch = BarcodeStr[i];
		if (isalnum(static_cast<unsigned char>(ch))) {	str += ch;	}
	}
	BarcodeStr = str;
	
	if (HASI_AOI_STAGE_PRE == AOIStage) { str = PCBSerialNo; }
	else if (HASI_AOI_STAGE_POST == AOIStage) { PCBSerialNo = _T("0"); }
	Filename.Format(_T("%s\\%s_%s_%s_%s.%s"), FileFolder, str, DateTime, _T("Measure"), AOIStageStr, _T("omr"));
	
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("wb"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(Filename, TMode);
	if (NULL == pfile)
	{
		m_ErrorString.Format(_T("Error, Open Hanwha_Measure_AOIB to Save Fault (%s)"), Filename);
		return false;
	}

	//-------------------------------------------------------------------------------------//
	//[MAOIMEASUREDATA]
	CTime DataTimeStart = TestReuslt.sDateTimeS;
	CTime DataTimeEnd = TestReuslt.sDateTimeE;
	CString Date, Time;
	Date.Format(_T("%.4d%.2d%.2d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay);
	Time.Format(_T("%.2d%.2d%.2d"), SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	SetNPM_DateTime(DataTimeStart, LaneID);
	CString Setion = _T("MAOIMEASUREDATA");
	std::vector<CString> Fields = {
		_T("DATE"),	_T("TIME"),	_T("STARTDATE"), _T("STARTTIME"), _T("ENDDATE"), _T("ENDTIME")
	};
	std::vector<CString> Data = {
		Date,	Time,
		DataTimeStart.Format(_T("%Y%m%d")),
		DataTimeStart.Format(_T("%H%M%S")),
		DataTimeEnd.Format(_T("%Y%m%d")),
		DataTimeEnd.Format(_T("%H%M%S")),
	};

	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[LINE]
	Setion = _T("LINE");
	CString LaneType, MachineType, SerialLane;
	switch (LaneID)
	{
	case LANE_ID_A:	LaneType = _T("FRONT");	break;
	case LANE_ID_B:	LaneType = _T("REAR");	break;
	default:
		m_ErrorString.Format(_T("Get Land type Failed[%s]"), FuntionName);
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	MULTI_LANE_MODE LANE_MODE = CheckMultiLaneMode();
	switch (LANE_MODE)
	{
	case MULTI_LANE_1:	MachineType = _T("SINGLE");	break;
	case MULTI_LANE_2:	MachineType = _T("DUAL");	break;
	default:
		break;
	}
	SerialLane = M2MHanwha.m_PCBSerialLaneType;
	Fields = {
		_T("LANETYPE"),
		_T("MACHINETYPE"),
		_T("SERIALLANE"),
	};
	Data = {
		LaneType,
		MachineType,
		SerialLane
	};
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[PCBDATA]
	Setion = _T("PCBDATA");
	CString BoardID, RegNumber, Modified;
	BoardID = ProjectPtr->GetProjectFileMainName();
	//BarcodeStr = ProjectPtr->GetProjectBarcode();
	//if (M2MHanwha.m_PCBSerialBarcode != BarcodeStr) { BarcodeStr = _T(" "); }

	if (AOIStage == HASI_AOI_STAGE_POST) {
		RegNumber = _T("0");
		Modified = _T(" ");
	}
	else {
		RegNumber = M2MHanwha.m_PCBSerialRegNo;
		Modified = M2MHanwha.m_PCBSerialModified;
	}
	
	Fields = { _T("BOARDID"), _T("SERIALNO "), _T("BARCODE"), _T("REGNUMBER"), _T("MODIFIED")};
	Data = { BoardID, PCBSerialNo, BarcodeStr, RegNumber, Modified };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[MODELMARK]
	Setion = _T("MODELMARK");
	//COLOR後:MAOI Non-Use Information,
	//The content is treated with spaces, and between items are treated with tab
	//TYPE:Write by changing the work surface (TOP/BOT) value set in MAOI to a number
	//USE:Write whether to use mixed flow mode in MAOI
	CString WorkType, MixFlowMode;
	PANEL_SIDE_MODE Mode = ProjectPtr->GetProjectPanelSideMode();
	const size_t ProjectCount = GetLaneProjectCount(LaneID);
	switch (Mode)
	{
	case PANEL_SIDE_TOP:	WorkType = _T("1");	break;
	case PANEL_SIDE_BOTTOM:	WorkType = _T("2");	break;
	case PANEL_SIDE_HYBRID:
		m_ErrorString.Format(_T("Error, [Panel side] Hybrid mode not available in M2M."));
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	case PANEL_SIDE_RETURN:
	default:
		m_ErrorString.Format(_T("Error, Undifined Panel side occured in [%s]"), FuntionName);
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	if (ProjectCount < 2) { MixFlowMode = _T("0"); }
	else if (ProjectCount == 2) { MixFlowMode = _T("1"); }
	else {
		m_ErrorString.Format(_T("Error, Invalided project count >2 in M2M "));
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	Fields = { _T("TYPE"),	_T("USE"),	_T("COLOR"),_T("POSX"),	_T("POSY"),	_T("SIZEX"), _T("SIZEY") };
	Data = { WorkType, MixFlowMode,	_T(" "),_T(" "),_T(" "),_T(" "),_T(" ") };
	
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[FIDUCIAL]
	Setion = _T("FIDUCIAL");
	//Simply write the original coordinates of all Fiducial Marks registered in AOI
	TPOINT2D Res, MinCad;
	MinCad = ProjectPtr->GetHASI_MinCadPos();

	double PosX, PosY;
	const int FdCount = ProjectPtr->GetProjectFdCount();
	CAOIFd* FdPtr = NULL;
	HASI_FieldMap Table;
	Fields.clear();
	Fields = { L"X",L"Y" };
	for (const auto& item : Fields) { Table[item] = std::vector<CString>(); }
	for (int i = 0; i < FdCount; i++) {
		FdPtr = ProjectPtr->GetProjectFdPtr(i, false);
		if (NULL == FdPtr) { continue; }
		PosX = FdPtr->GetFdSpecialCadPosX();
		PosY = FdPtr->GetFdSpecialCadPosY();
		if (INVALID_DOUBLE == PosX && INVALID_DOUBLE == PosY) {
			PosX = FdPtr->GetFdCadPosX() - MinCad.x;
			PosY = FdPtr->GetFdCadPosY() - MinCad.y;
		}
		//PosX = FdPtr->GetFdStagePosX() - MinCad.x;
		//PosY = FdPtr->GetFdStagePosY() - MinCad.y;
		PosX = JetAPI::Unit_UmtoMM(PosX);
		PosY = JetAPI::Unit_UmtoMM(PosY);
		str.Format(_T("%4.4f"), PosX);	Table[_T("X")].push_back(str);
		str.Format(_T("%4.4f"), PosY);	Table[_T("Y")].push_back(str);
	}
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Table)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[STATE]
	Setion = _T("STATE");
	//STATE:
	//NOT_INSPECTED:If the PCB is discharged in BYPASS mode or without other inspection
	//INSPECTED:If the inspection equipment performed the inspection normally
	//REVIEWED:If you work at NG Buffer (複判)
	TASK_MODE TaskMode = GetTaskMode();
	CString ProjectState;
	switch (TaskMode)
	{
	case TASK_INSPECT_PROJECT:	ProjectState = _T("INSPECTED");	break;
	case TASK_TUNING_OFFLINE:	
	case TASK_LANE_BYPASS:		ProjectState = _T("NOT_INSPECTED");	break;
	default:
		break;
	}
	Fields = {_T("STATE") };
	Data = {ProjectState };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[POINT]
	Setion = _T("POINT");
	//POINTID: Unique ID within the board
	//PANEL:Not using in AOI,write it as 1.
	//ARRAY:Depending on the method of creating the ARRAY, the ARRAY numbers of CM and MAOI may be different.
	//REF:Mounting point name 零件名稱
	//PART:Part name 料號
	//DELTAX, DELTAY, DELTAT, DELTAZ :The deviation of x,y,angel and z .
	//INSPECT:
	//	OK : If the part is judged OK in the inspection
	//	NG: If the part is judged NG in the inspection
	//	SKIP : In case of parts that have been inspected skipped
	//	PASS : If the part is judged as NG by the inspection equipment and then finally judged as OK by the user.
	//	NSCO : If the inspector cannot apply the contents of the sco file to a component
	//	M2MOFF : If the part is set to not be M2M - applied in the inspector(See 2.2.3.3)
	//	ABNORMAL : If the part has unreliable inspection results from other inspection machines
	HASI_FieldMap ComponentMap;
	Fields.clear();
	Fields = {
		_T("POINTID"),	_T("PANEL"),	_T("ARRAY"),	_T("REF"),	_T("PART"),
		_T("DELTAX"),	_T("DELTAY"),	_T("DELTAT"),	_T("DELTAZ"),	_T("INSPECT")
	};
	for (const auto& field : Fields) { ComponentMap[field] = std::vector<CString>(); }
	CAOIComponent *ComponentPtr = NULL;
	RESULT_ID ResultID;

	//-------------------------------------------------------------------------------------//
	//Save component image
	CString DerivedName, SpcImageFolder;
	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolder();
	if (ProjectPtr->GetProjectUseLocalFolder())
	{	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolderLocal(); }

	CString ImageName;
	CString ImageFileFolder;
	ImageFileFolder.Format(L"%s\\toPnP\\MonitoringImg\\%s", ShareFolder, BoardID);
	if (!JetAPI::IsFolderExist(ImageFileFolder)) {
		if (!JetAPI::CreateFolder(ImageFileFolder)) {
			::fclose(pfile); pfile = NULL;
			::DeleteFile(Filename);
			Error.Format(_T("Create folder[%s] failed"), ImageFileFolder);
			SetErrorString(Error);
		}
	}
	//-------------------------------------------------------------------------------------//
	const int BoardCount = ProjectPtr->GetProjectBoardCount();
	CAOIBoard *BoardPtr = NULL;
	CAOIModel *ModelPtr = NULL;
	CAOIWnd *WndPtr = NULL;
	unsigned int WndIdx;
	CString PointID, ArrayStr, RefStr, ResultStr, PCBSideStr;
	//When the serial number is not received or there is no barcode, it is displayed as 0.
	if (PCBSerialNo == " ") { PCBSerialNo = L"0"; }
	if (BarcodeStr == " ") { BarcodeStr = L"0"; }

	double SizeZ, ResultZ, DeltaZ;
	for (int i = 0; i < BoardCount; i++) {
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if (NULL == BoardPtr) { continue; }
		const int BoardComponentCount = BoardPtr->GetBoardComponentCount();
		const int BoardIndex = BoardPtr->GetBoardIndex_Project() + 1;
		for (int j = 0; j < BoardComponentCount; j++) {
			ComponentPtr = BoardPtr->GetBoardComponentPtr(j, false);
			if (NULL == ComponentPtr) { continue; }
			if (true == ComponentPtr->CheckComponentIsAgent()) { continue; }
			ResultID = ComponentPtr->CheckComponentResultID_AOI();
			if (ResultID != RESULT_ID_NONE) {
				ModelPtr = ComponentPtr->GetComponentModelPtr();
				if (NULL == ModelPtr) { DeltaZ = 0; }
				else {
					WndIdx = ComponentPtr->GetComponentResultHeight_WndIdx();
					WndPtr = ModelPtr->GetModelWndPtr(WndIdx, true);
					if (NULL == WndPtr) { DeltaZ = 0; }
					else {
						SizeZ = WndPtr->GetWndAlgParam().GetAlgParamBrightRatio().brTargetValue;
						ResultZ = ComponentPtr->GetComponentResultHeight();
						DeltaZ = (ResultZ - SizeZ);
						DeltaZ = JetAPI::Unit_UmtoMM(DeltaZ);
					}
				}
			}
			else {
				DeltaZ = 0;
			}

			str.Format(L"%d", ComponentPtr->GetComponentUniqueID()+1);
			ComponentMap[L"POINTID"].push_back(str);
			PointID = str; // Use in Image File Name
			str = L"1";
			ComponentMap[L"PANEL"].push_back(str);
			str.Format(L"%d", BoardIndex);;
			ComponentMap[L"ARRAY"].push_back(str);
			ArrayStr = str;// Use in Image File Name

			str.Format(L"%s", ComponentPtr->GetComponentName());
			ComponentMap[L"REF"].push_back(str);
			RefStr = str;// Use in Image File Name
			str.Format(L"%s", ComponentPtr->GetComponentPartNumber());
			ComponentMap[L"PART"].push_back(str);
			str.Format(L"%4.4f", JetAPI::Unit_UmtoMM(ComponentPtr->GetComponentResultOffsetX()));//mm
			ComponentMap[L"DELTAX"].push_back(str);
			str.Format(L"%4.4f", JetAPI::Unit_UmtoMM(ComponentPtr->GetComponentResultOffsetY()));//mm
			ComponentMap[L"DELTAY"].push_back(str);
			str.Format(L"%4.4f", ComponentPtr->GetComponentResultSkewAngle());
			ComponentMap[L"DELTAT"].push_back(str);
			str.Format(L"%4.4f", DeltaZ);
			ComponentMap[L"DELTAZ"].push_back(str);
			switch (ResultID)
			{
			case RESULT_ID_OK:		str = _T("OK");		break;
			case RESULT_ID_NG:	    str = _T("NG");	    break;
			case RESULT_ID_NONE:
			case RESULT_ID_BYPASS:
			case RESULT_ID_SKIP:
				str = _T("SKIP");		break;
			case RESULT_ID_EXCEPTION:	
			default:				
				str = _T("ABNORMAL");	break;
			}
			if (true == GetHASI_SPIOffsetFileEnable()) {
				if (HASI_STATE_MODE_RUN != HASI_SPIFileState || HASI_STATE_MODE_AC != HASI_SPIFileState) {
					//覆寫
					if (false == ComponentPtr->GetComponentHASI_SPIOffset_Enable()) { str = _T("M2MOFF"); }
					else if (false == ComponentPtr->GetComponentHASI_SPIOffset_IsApplied()) { str = _T("NSCO"); }
					if (ResultID == RESULT_ID_NONE|| ResultID == RESULT_ID_BYPASS|| ResultID == RESULT_ID_SKIP) { str = _T("SKIP"); }
				}
			}

			ResultStr = str;// Use in Image File Name
			ComponentMap[L"INSPECT"].push_back(str);
			//-------------------------------------------------------------------------------------//
			// Component image save to M2M shared folder
			if (false == ComponentPtr->GetComponentHASI_SaveImage()) { continue; }
			if (ResultID == RESULT_ID_NONE) { continue; }
			switch (Mode)
			{
			case PANEL_SIDE_TOP:	PCBSideStr = _T("T");	break;
			case PANEL_SIDE_BOTTOM:	PCBSideStr = _T("B");	break;
			}
			//ImageName : BoardID\\DateTime_Serial_Barcode_POINTID_ARRAY_REF_InspectionResult_PCBSIDE_WORKLANE.jpg
			if (HASI_AOI_STAGE_PRE == AOIStage) { 
				ImageName.Format(_T("%s\\%s_%s_%s_%s_%s_%s_%s_%s_%c.jpg"),
					ImageFileFolder, DateTime, PCBSerialNo, BarcodeStr,
					PointID, ArrayStr, RefStr, ResultStr, PCBSideStr, LaneType[0]);
			}
			else if (HASI_AOI_STAGE_POST == AOIStage) {
				ImageName.Format(_T("%s\\%s_%s_%s_%s_%s_%s_%s_%c.jpg"),
					ImageFileFolder, DateTime, BarcodeStr,
					PointID, ArrayStr, RefStr, ResultStr, PCBSideStr, LaneType[0]);
			}
			//從AOITemp複製
			DerivedName = ComponentPtr->GetRgnDerivedName();
			str.Format(_T("%s\\%s#1.%s"), SpcImageFolder, DerivedName, _T("JPG"));
			if (::CopyFile(str, ImageName, FALSE) == FALSE) {
				Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), str, ImageName);
				SetErrorString(Error);
				::fclose(pfile); pfile = NULL;
				::DeleteFile(Filename);
				return false;
			}
			//-------------------------------------------------------------------------------------//
		}
	}

	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, ComponentMap)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	::fclose(pfile); pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_SaveAOIInspectionResultFile_VPnP(CAOIProject * ProjectPtr,bool bCurrent)
{
	if (false == GetHASI_Enable()) { return true; }
	if (false == GetHASI_PnPEnable()) { return true; }
	if (CheckProjectPtr(ProjectPtr) == false) { return false; }
	CString FuntionName = _T("SaveAOIInspectionResultFilePNP_Hanwha");
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	CString ShareFolder = GetHASI_ShareFolder();
	CString str,Error;
	CString FileFolder,Filename, DstName;
	FileFolder.Format(L"%s\\toPnP\\Inspect", ShareFolder);

	TTestResult TestReuslt;
	if (bCurrent) {	TestReuslt = ProjectPtr->GetProjectResultCurrent();}
	else {	TestReuslt = ProjectPtr->GetProjectResultLatest_ARS();}
	
	//-------------------------------------------------------------------------------------//
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);  // 取得當地時間（含毫秒）

	CString DateTime;
	DateTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d%.3d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds);
	CString BoardKey = TestReuslt.sDateTimeS.Format(_T("%Y%m%d%H%M%S"));
	TCHAR   TMode[32] = _T("");
	Filename.Format(_T("%s\\%s_%s_%s.%s"), FileFolder, BoardKey, DateTime, _T("Defect"), _T("pdta"));
	_tcscpy(TMode, _T("wb"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(Filename, TMode);
	if (NULL == pfile)
	{
		m_ErrorString.Format(_T("Error, Open (%s) to Save Fault "), Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[MAOIMEASUREDATA]
	CTime DataTimeStart = TestReuslt.sDateTimeS;
	CTime DataTimeEnd = TestReuslt.sDateTimeE;
	CString Date, Time;
	Date.Format(_T("%.4d%.2d%.2d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay);
	Time.Format(_T("%.2d%.2d%.2d"), SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	SetNPM_DateTime(DataTimeStart, LaneID);
	CString Setion = _T("TIME");
	std::vector<CString> Fields = { 
		_T("DATE"),	_T("TIME"),	_T("STARTDATE"), _T("STARTTIME"), _T("ENDDATE"), _T("ENDTIME") 
	};
	std::vector<CString> Data = {
		Date,	Time,
		DataTimeStart.Format(_T("%Y%m%d")),
		DataTimeStart.Format(_T("%H%M%S")),
		DataTimeEnd.Format(_T("%Y%m%d")),
		DataTimeEnd.Format(_T("%H%M%S")),
	};
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[LINE]
	Setion = _T("LINE");
	CString LaneType, MachineType;
	switch (LaneID)
	{
	case LANE_ID_A:	LaneType = _T("FRONT");	break;
	case LANE_ID_B:	LaneType = _T("REAR");	break;
	default:
		m_ErrorString.Format(_T("Get Land type Failed[%s]"), FuntionName);
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	MULTI_LANE_MODE LANE_MODE = CheckMultiLaneMode();
	switch (LANE_MODE)
	{
	case MULTI_LANE_1:	MachineType = _T("SINGLE");	break;
	case MULTI_LANE_2:	MachineType = _T("DUAL");	break;
	default:
		break;
	}
	Fields = { _T("LANETYPE"),	_T("MACHINETYPE"), };
	Data = { LaneType,	MachineType, };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[PCBDATA]
	Setion = _T("PCBDATA");
	CString BoardID, BarcodeStr, ComponentCountStr, BoardCountStr, ProjectResult;
	BoardID = ProjectPtr->GetProjectFileMainName();
	BarcodeStr = ProjectPtr->GetProjectBarcode();

	//const int ComponentCount = ProjectPtr->GetProjectComponentCount();
	const int ComponentCount = ProjectPtr->GetProjectComponentNotAgentCount();
	const int BoardCount = ProjectPtr->GetProjectBoardCount();
	ComponentCountStr.Format(L"%d", ComponentCount);
	BoardCountStr.Format(L"%d", BoardCount);
	switch (TestReuslt.sResultID)
	{
	case(TEST_RESULT_OK): ProjectResult = _T("OK");	break; //良品
	case(TEST_RESULT_NG): ProjectResult = _T("NG");	break; //不良品
	default: break;
	}
	Fields = {
		_T("BOARDID"),	_T("BOARDKEY"),	_T("BARCODE"),	_T("INSPECTPOINT"),	_T("ARRAYCOUNT"),	_T("RESULT") };
	Data = {
		BoardID ,	BoardKey,	BarcodeStr,	ComponentCountStr,	BoardCountStr,	ProjectResult,
	};
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[STATE]
	Setion = _T("STATE");
	//STATE:
	//NOT_INSPECTED:If the PCB is discharged in BYPASS mode or without other inspection
	//INSPECTED:If the inspection equipment performed the inspection normally
	//REVIEWED:If you work at NG Buffer(複判)
	TASK_MODE TaskMode = GetTaskMode();
	CString ProjectState;
	if (true == bCurrent) {	ProjectState = _T("INSPECTED");}
	else { ProjectState = _T("REVIEWED"); }
	if (TaskMode == TASK_TUNING_OFFLINE) { ProjectState = _T("NOT_INSPECTED"); }
	Fields = { _T("STATE") };
	Data = { ProjectState };
	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, Data)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	//-------------------------------------------------------------------------------------//
	//[POINT]
	Setion = _T("POINT");
	
	HASI_FieldMap ComponentMap;
	Fields.clear();
	Fields = {
		_T("PANEL"),	_T("ARRAY"),	_T("REF"),	_T("PART"),
		_T("INSPECT"),	_T("TEST"),		_T("DEFECTCODE"),	_T("IMAGENAME")
	};
	for (const auto& field : Fields) { ComponentMap[field] = std::vector<CString>(); }
	//-------------------------------------------------------------------------------------//
	//Save component image
	CString DerivedName, SpcImageFolder;
	CString ImageName;
	CString ImageFileFolder;

	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolder();
	if (ProjectPtr->GetProjectUseLocalFolder())
	{	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolderLocal();}

	ImageFileFolder.Format(L"%s\\toPnP\\Inspect\\Image", ShareFolder);
	if (!JetAPI::IsFolderExist(ImageFileFolder)) {
		if (!JetAPI::CreateFolder(ImageFileFolder)) {
			::fclose(pfile); pfile = NULL;
			::DeleteFile(Filename);
			Error.Format(_T("Create folder[%s] failed"), ImageFileFolder);
			SetErrorString(Error);
			return false;
		}
	}
	//-------------------------------------------------------------------------------------//
	CString PointID, PanelStr, ArrayStr, RefStr, ResultStr;
	CAOIBoard *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	RESULT_ID ResultID, ResultID_RSM;
	CString DefectCode;
	for (int i = 0; i < BoardCount; i++) {
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if (NULL == BoardPtr) { continue; }
		const int BoardComponentCount = BoardPtr->GetBoardComponentCount();
		const int BoardIndex = BoardPtr->GetBoardIndex_Project() + 1;
		for (int j = 0; j < BoardComponentCount; j++) {
			ComponentPtr = BoardPtr->GetBoardComponentPtr(j, false);
			if (NULL == ComponentPtr) { continue; }
			if (true == ComponentPtr->CheckComponentIsAgent()) { continue; }
			str = L"1";	PanelStr = str;	ComponentMap[L"PANEL"].push_back(str);
			str.Format(L"%d", BoardIndex);	ArrayStr = str;	ComponentMap[L"ARRAY"].push_back(str);
			str.Format(L"%s", ComponentPtr->GetComponentName());	RefStr = str;	ComponentMap[L"REF"].push_back(str);
			str.Format(L"%s", ComponentPtr->GetComponentPartNumber());	ComponentMap[L"PART"].push_back(str);
			ResultID = ComponentPtr->CheckComponentResultID_AOI();
			
			switch (ResultID)
			{
			case RESULT_ID_OK:		str = _T("OK");		break;
			case RESULT_ID_NG:	    str = _T("NG");	    break;
			case RESULT_ID_NONE:
			case RESULT_ID_BYPASS:
			case RESULT_ID_SKIP:
				str = _T("SKIP");		break;
			case RESULT_ID_EXCEPTION:
			default:
				str = _T("ABNORMAL");	break;
			}

			ComponentMap[L"INSPECT"].push_back(str);

			if (ResultID != RESULT_ID_NG) {
				ComponentMap[L"TEST"].push_back(_T(""));
				ComponentMap[L"DEFECTCODE"].push_back(_T(""));
				ComponentMap[L"IMAGENAME"].push_back(_T(""));
				continue;
			}
			
			
			if (true == bCurrent) {
				str = _T("");
				ComponentMap[L"TEST"].push_back(str);
				ComponentMap[L"DEFECTCODE"].push_back(str);
			}
			else {
				if (MULTI_LANE_2 == LANE_MODE) {
					ResultID_RSM = ComponentPtr->GetComponentResultID_ARS_Lane(LaneID);
				}
				else { ResultID_RSM = ComponentPtr->GetComponentResultID_ARS(); }
				switch (ResultID_RSM)
				{
				case RESULT_ID_OK:		str = _T("OK");		break;
				case RESULT_ID_NG:	    str = _T("NG");	    break;
				case RESULT_ID_SKIP:
					str = _T("SKIP");		break;
				case RESULT_ID_EXCEPTION:
				default:
					str = _T("ABNORMAL");	break;
				}
				ComponentMap[L"TEST"].push_back(str);
				if (RESULT_ID_NG == ResultID_RSM) {
					if (false == ComponentPtr->CalcComponentHASI_Result(DefectCode)) 
					{ 	DefectCode = _T("CalcComponentHASI DefectCode result::unknown error"); } 
					str.Format(_T("%s"), DefectCode);
				}
				else { str = _T(""); }
				ComponentMap[L"DEFECTCODE"].push_back(str);
			}
			//-------------------------------------------------------------------------------------//

			if (ResultID != RESULT_ID_NG) { 
				ComponentMap[L"IMAGENAME"].push_back(_T(""));
				continue; 
			}

			ImageName.Format(_T("%s_%s_%s_%s_%s.jpg"),
				BoardKey, BoardKey, PanelStr, ArrayStr, RefStr);
			ComponentMap[L"IMAGENAME"].push_back(ImageName);
			
			//ImageName.Format(_T("%s\\%s"), ImageFileFolder, ImageName);
			////從AOITemp複製
			//DerivedName = ComponentPtr->GetRgnDerivedName();
			//str.Format(_T("%s\\%s#1.%s"), SpcImageFolder, DerivedName, _T("JPG"));
			//if (::CopyFile(str, ImageName, FALSE) == FALSE) {
			//	::fclose(pfile); pfile = NULL;
			//	::DeleteFile(Filename);
			//	Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), str, ImageName);
			//	SetErrorString(Error);
			//	return false;
			//}
			//-------------------------------------------------------------------------------------//
		}
	}

	if (false == ExecHASI_SaveTableFile(pfile, Setion, Fields, ComponentMap)) {
		::fclose(pfile); pfile = NULL;
		::DeleteFile(Filename);
		return false;
	}
	::fclose(pfile); pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_SaveAOIInspectionImage_VPnP(CAOIProject * ProjectPtr)
{
	if (false == GetHASI_Enable()) { return true; }
	if (false == GetHASI_PnPEnable()) { return true; }
	//Save component image
	CString ShareFolder = GetHASI_ShareFolder();
	CString DerivedName, SpcImageFolder;
	CString ImageName;
	CString ImageFileFolder;
	CString Error, str;
	CString DateTime;
	
	
	const TTestResult &TestReuslt = ProjectPtr->GetProjectResultCurrent();
	CString BoardID = ProjectPtr->GetProjectFileMainName();
	CString BoardKey = TestReuslt.sDateTimeS.Format(_T("%Y%m%d%H%M%S"));
	//DateTime.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d%.3d"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds);

	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolder();
	if (ProjectPtr->GetProjectUseLocalFolder())
	{	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolderLocal();}
	ImageFileFolder.Format(L"%s\\toPnP\\Inspect\\Image", ShareFolder);
	if (!JetAPI::IsFolderExist(ImageFileFolder)) {
		if (!JetAPI::CreateFolder(ImageFileFolder)) {
			Error.Format(_T("Create folder[%s] failed"), ImageFileFolder);
			SetErrorString(Error);
			return false;
		}
	}
	//-------------------------------------------------------------------------------------//
	CString PointID, PanelStr, ArrayStr, RefStr, ResultStr;
	const int BoardCount = ProjectPtr->GetProjectBoardCount();
	CAOIBoard *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	RESULT_ID ResultID;
	for (int i = 0; i < BoardCount; i++) {
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if (NULL == BoardPtr) { continue; }
		const int BoardComponentCount = BoardPtr->GetBoardComponentCount();
		const int BoardIndex = BoardPtr->GetBoardIndex_Project() + 1;

		PanelStr = L"1";
		ArrayStr.Format(L"%d", BoardIndex);

		for (int j = 0; j < BoardComponentCount; j++) {
			ComponentPtr = BoardPtr->GetBoardComponentPtr(j, false);
			if (NULL == ComponentPtr) { continue; }
			if (true == ComponentPtr->CheckComponentIsAgent()) { continue; }
			RefStr.Format(L"%s", ComponentPtr->GetComponentName());
			//-------------------------------------------------------------------------------------//
			ResultID = ComponentPtr->CheckComponentResultID_AOI();
			if (ResultID != RESULT_ID_NG) {	continue;	}
			//ImageName : BOARDKEY_TIME_PANEL_ARRAY_REF.jpg
			//ImageName.Format(_T("%s\\%s_%s_%s_%s_%s.jpg"),
			//ImageFileFolder, BoardKey, DateTime, PanelStr, ArrayStr, RefStr);
			ImageName.Format(_T("%s\\%s_%s_%s_%s_%s.jpg"),
				ImageFileFolder, BoardKey, BoardKey, PanelStr, ArrayStr, RefStr);
			//從AOITemp複製
			DerivedName = ComponentPtr->GetRgnDerivedName();
			str.Format(_T("%s\\%s#1.%s"), SpcImageFolder, DerivedName, _T("JPG"));
			if (::CopyFile(str, ImageName, FALSE) == FALSE) {
				Error.Format(_T("Error, Copy File Fault\nFrom:%s\nTo:%s"), str, ImageName);
				SetErrorString(Error);
				return false;
			}
			//-------------------------------------------------------------------------------------//
		}
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_SaveTableFile(FILE * pfile, CString Section, std::vector<CString> Fields, std::vector<CString> Data)
{
	if (pfile == nullptr || Fields.size() != Data.size())
		return false;
	if (pfile == nullptr || Fields.size() != Data.size())
		return false;
	// 輸出 Section 標頭，例如 [BOARDSERIALDATA]
	const size_t     szBuffer = 256;
	wchar_t          str[szBuffer] = L"";
	::wcscpy(str, Section);
	::fwprintf(pfile, L"[%s]\r\n", str);

	// 輸出欄位名稱 (key)
	for (size_t i = 0; i < Fields.size(); i++)
	{
		::wcscpy(str, Fields[i]);
		::fwprintf(pfile, str);
		if (i < Fields.size() - 1) fwprintf(pfile, L"\t");
	}
	::fwprintf(pfile, L"\r\n");

	// 輸出資料值 (value)
	for (size_t i = 0; i < Data.size(); i++)
	{
		::wcscpy(str, Data[i]);
		::fwprintf(pfile, str);
		if (i < Data.size() - 1) ::fwprintf(pfile, L"\t");
	}
	::fwprintf(pfile, L"\r\n\r\n"); // 每個 Section 之後空一行

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecHASI_SaveTableFile(FILE * pfile, CString Section, std::vector<CString> Fields, HASI_FieldMap & Table)
{
	if (!pfile) return false;

	// 輸出 Section 標頭，例如 [BOARDSERIALDATA]
	const size_t     szBuffer = 256;
	wchar_t          str[szBuffer] = L"";
	::wcscpy(str, Section);
	::fwprintf(pfile, L"[%s]\r\n", str);

	// 輸出欄位名稱 (tab 分隔)
	for (size_t i = 0; i < Fields.size(); i++)
	{
		::wcscpy(str, Fields[i]);
		::fwprintf(pfile, str);
		if (i < Fields.size() - 1) ::fwprintf(pfile, L"\t");
	}
	::fwprintf(pfile, L"\r\n");
	std::vector<CString> dataList;
	Table.Lookup(Fields[0], dataList);
	const size_t ArraySize = dataList.size();
	// 依 Fields 的順序寫出
	for (size_t i = 0; i < ArraySize; i++) {
		for (size_t j = 0; j < Fields.size(); j++) {
			dataList.clear();
			if (!Table.Lookup(Fields[j], dataList)) continue;
			::wcscpy(str, dataList[i]);
			::fwprintf(pfile, str);
			if (j < Fields.size() - 1) ::fwprintf(pfile, L"\t");
		}
		::fwprintf(pfile, L"\r\n");
	}
	::fwprintf(pfile, L"\r\n");
	return true;
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetHASI_MonitorThreadCmd()//取得M2M監測傳訊執行緒命令
{
	return m_HASI_MonitorThreadCmd;
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetHASI_MonitorThreadState()//取得M2M監測傳訊執行緒狀態
{
	return m_HASI_MonitorThreadState;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetHASI_MonitorThreadCmd(THREAD_COMMAND_MODE Cmd)
{
	if (m_HASI_MonitorThreadCmd == Cmd) { return; }
	LockThreadHASI_Monitor();
	m_HASI_MonitorThreadCmd = Cmd;
	UnlockThreadHASI_Monitor();
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetHASI_MonitorThreadState(THREAD_STATE_MODE State)
{
	if (m_HASI_MonitorThreadState == State) { return; }
	LockThreadHASI_Monitor();
	m_HASI_MonitorThreadState = State;
	UnlockThreadHASI_Monitor();
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::LockThreadHASI_Monitor()
{
	::EnterCriticalSection(&m_csThreadHASI_Monitor);
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::UnlockThreadHASI_Monitor()
{
	::LeaveCriticalSection(&m_csThreadHASI_Monitor);
}
//-------------------------------------------------------------------------------------//
unsigned __stdcall HASI_MoniterThreadFn(void* pParam)
{
#ifndef HASI_DISABLE
	bool IsOK = true;
	DWORD  SleepTime = 5000;
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;

	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256] = _T("");

	while (true)
	{
		ThreadCmd = AOIDataCollect.GetHASI_MonitorThreadCmd();
		ThreadState = AOIDataCollect.GetHASI_MonitorThreadState();
		if (THREAD_COMMAND_TO_EXIT == ThreadCmd){ break;	}
		if (THREAD_COMMAND_TO_IDLE == ThreadCmd)
		{
			AOIDataCollect.SetHASI_MonitorThreadState(THREAD_STATE_IDLE);
			::Sleep(SleepTime);
			continue;
		}
		if (THREAD_COMMAND_TO_RUN == ThreadCmd)
		{
			
			if (THREAD_STATE_FINISH == ThreadState || THREAD_STATE_EXCEPTION == ThreadState)
			{
				::Sleep(SleepTime);
				continue;
			}
			AOIDataCollect.SetHASI_MonitorThreadState(THREAD_STATE_RUNNING);
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecHASI_MoniterFn();
			QueryPerformanceCounter(&nEndTime);
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0 / AOIDataCollect.m_SystemFreq.QuadPart;
			if (false == IsOK)
			{
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_HASI_MONITER, true, ErrorMSG);
			}
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if (FN_ENABLE == SystemParam.m_SaveHASIMonitorThreadLog)
			{
				_stprintf(strBuffer, _T("HASI_MoniterThreadFn[%d]: %.2f ms"), ThreadIdx + 1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);
			}
		}
		::Sleep(SleepTime);
	};

	AOIDataCollect.SetHASI_MonitorThreadState(THREAD_STATE_NONE);
	if (NULL != HASI_MoniterThreadEvent)
	{
		::SetEvent(HASI_MoniterThreadEvent);
	}
	//	::_endthreadex(0);
#endif//HASI_DISABLE

	return true;
}
//-------------------------------------------------------------------------------------//