#include "stdafx.h"
#include "JetIniFile.h"


CJetIniFile::CJetIniFile()
{
	m_Opened = false;
}

CJetIniFile::~CJetIniFile()
{
}

bool CJetIniFile::GetFileOpened() const//取得檔案是否開啟
{
	return m_Opened;
}
void CJetIniFile::SetFileOpened(bool val)//設定檔案是否開啟
{
	m_Opened = val;
}

void CJetIniFile::SetFilename(const char *filename)
{
	AnsiToUnicode(filename, m_Filename);
}

void CJetIniFile::SetFilename(const wchar_t *filename)
{
	m_Filename = filename;
}

bool CJetIniFile::CheckFilenameTheSame(const char *filename)
{
	std::wstring ws;
	AnsiToUnicode(filename, ws);
	return m_Filename==ws;
}
bool CJetIniFile::CheckFilenameTheSame(const wchar_t *filename)
{
	return m_Filename==filename;
}

std::vector<CJetIniSection>& CJetIniFile::GetSectionList()
{
	return m_SectionList;
}

bool CJetIniFile::BuildSectionList()
{	
	std::wstring Buf;
	if ( CJetTxtFile::GetTxtData(Buf) == false )
	{	return false; }

	size_t Pos=0;
	CJetIniSection Section;
	std::wstring Str, Name, Value;
	while ( true )
	{
		if ( ReadString(Buf, Pos, Str) == false )
		{	return false; }
		if ( AddIniString(Str, Section) == false )
		{	return false; }
		if ( CheckStringEnd(Buf, Pos) == true )
		{	break; }
	};
	AddSection(Section);
	return true;
}

size_t CJetIniFile::ClacSectionListAllocateSize()
{
	size_t i=0;	
	size_t size=0;	
	const std::vector<CJetIniSection> &List=GetSectionList();
	const size_t SectionCount=List.size();

	for ( i=0; i<SectionCount; i++ )
	{
		const CJetIniSection &SectionRef=List[i];
		size += SectionRef.CalcAllocateSize();		
	}
	return size;	
}

bool CJetIniFile::BuildUnicodeString(std::wstring &ws)
{
	const size_t size=ClacSectionListAllocateSize();
	if ( 0 == size ) { return false; }

	size_t i=0, j=0;	
	const std::vector<CJetIniSection> &List=GetSectionList();
	const size_t SectionCount=List.size();

	ws.resize(size+1);
	ws.clear();
	for ( i=0; i<SectionCount; i++ )
	{
		const CJetIniSection &SectionRef=List[i];		
		SectionRef.AppendString(ws);		
	}
	size_t len=ws.length();
	return true;
}

bool CJetIniFile::AddSection(const CJetIniSection &Section)
{
	if ( Section.CheckEmpty() == true ) { return true; }	
	m_SectionList.push_back(Section);
	return true;
}

CJetIniSection* CJetIniFile::GetSectionPtr(const std::wstring &Str)
{
	size_t i=0;
	std::vector<CJetIniSection> &List=GetSectionList();
	const size_t SectionCount=List.size();
	for ( i=0; i<SectionCount; i++ )
	{
		CJetIniSection &Ref=List[i];
		if ( Ref.CompareName(Str) == false ) { continue; }		
		return &Ref;
	}
	return NULL;
}

bool CJetIniFile::AddIniString(const std::wstring &Str, CJetIniSection &Section)
{
	if ( CheckSectionText(Str) == true )
	{	
		AddSection(Section);	
		Section.Init();
		Section.SetSectionName(Str);		
		return true;
	}
	
	CJetIniNode Node;
	if ( CheckIniNode(Str, Node) == true )
	{	Section.AddIniNode(Node);	}
	return true;
}

bool CJetIniFile::CheckSectionText(const std::wstring &Str)
{
	const size_t len=Str.length();
	if ( L'[' != Str[0] ) { return false; }
	if ( L']' != Str[len-1] ) { return false; }
	return true;
}

bool CJetIniFile::CheckIniNode(const std::wstring &Str, CJetIniNode &Node)
{
	size_t i=0;	
	bool bValid=false;
	const size_t len=Str.length();
	size_t NameStart=0, NameEnd=len;
	size_t ValueStart=len, ValueEnd=len;
	for ( i=0; i<len; i++ )
	{
		if ( L'=' != Str[i] )
		{	continue; }

		bValid = true;
		NameEnd = i;
		ValueStart = i+1;
		break;		
	}

	if ( false == bValid )
	{	return false; }
	const size_t NameSize=NameEnd-NameStart;	
	Node.GetName().assign(Str.begin()+NameStart, Str.begin()+NameEnd);

	const size_t ValueSize=ValueEnd-ValueStart;
	if ( ValueSize > 0 )
	{	Node.GetValue().assign(Str.begin()+ValueStart, Str.begin()+ValueEnd);	}
	return true;
}
bool CJetIniFile::CheckStringEnd(const std::wstring &Buf, size_t Pos)
{
	if ( Pos < Buf.length() )
	{	return false; }
	size_t len=Buf.length();
	return true;
}

bool CJetIniFile::ReadString(const std::wstring &Buf, size_t &Pos, std::wstring &String)
{
	size_t i=0;
	size_t Len=Buf.length();
	size_t StartPos=Pos, EndPos=Pos;	
	if ( Pos > Len ) { return false; } 
	for ( i=Pos; i<Buf.length(); i++ )
	{		
		if ( 0x000A != Buf[i] )
		{	continue; }
		EndPos = i;
		break;
	}
	if ( EndPos == StartPos ) { EndPos = Len; }
	Pos = EndPos+1;
	if ( 0x000A == Buf[EndPos] ) { EndPos--; }
	if ( 0x000D == Buf[EndPos] ) { EndPos--; }
	EndPos ++;
	if ( EndPos > Len ) { EndPos = Len; }
	String.assign(Buf.begin()+StartPos, Buf.begin()+EndPos);
	return true;
}

bool CJetIniFile::Initial()
{	
	SetFilename(L"");
	SetFileOpened(false);	
	m_SectionList.clear();
	return true;
}

bool CJetIniFile::LoadIniFile(const TCHAR *Filename)
{
	Initial();	
	if ( CJetTxtFile::LoadTxtFile(Filename) == false )
	{	return false; }
	SetFileOpened(true);
	SetFilename(Filename);
	if ( BuildSectionList() == false )
	{	return false; }	
	return true;
}

bool CJetIniFile::SaveIniFile(const TCHAR *Filename, TXT_FILE_MODE Mode)
{
	std::wstring ws;
	if ( TXT_FILE_NONE != Mode )
	{	SetTxtFileMode(Mode);	}	
	if ( BuildUnicodeString(ws) == false ) 
	{	return false; }
	if ( CJetTxtFile::SaveTxtFile(Filename, ws, GetTxtFileMode()) == false )
	{	return false; }	
	SetFileOpened(true);
	SetFilename(Filename);
	return true;
}

bool CJetIniFile::CheckIniFileLoaded(const TCHAR *Filename)
{
	if ( GetFileOpened()==false || CheckFilenameTheSame(Filename)==false )
	{	return false;	}
	return true;
}


CJetIniNode* CJetIniFile::GetIniNode(const char *Section, const char *Name)
{
	std::wstring wSection;
	std::string Sect="["+std::string(Section)+"]";	
	if ( CJetTxtFile::AnsiToUnicode(Sect.c_str(), wSection) == false ) { return false; }	
	CJetIniSection *Ptr=GetSectionPtr(wSection);
	if ( NULL == Ptr )
	{	return NULL;	}

	std::wstring wName;
	if ( CJetTxtFile::AnsiToUnicode(Name, wName) == false ) { return false; }
	return Ptr->GetIniNodeByName(wName);
}

CJetIniNode* CJetIniFile::GetIniNode(const wchar_t *Section, const wchar_t *Name)
{
	std::wstring wSection=L"["+std::wstring(Section)+L"]";	
	CJetIniSection *Ptr=GetSectionPtr(wSection);
	if ( NULL == Ptr )
	{	return NULL; }
	return Ptr->GetIniNodeByName(Name);	
}

bool CJetIniFile::AddIniData(const char *Section, const char *Name, const char *Value)
{
	std::wstring wSection, wName, wVaule;
	std::string Sect="["+std::string(Section)+"]";	
	if ( CJetTxtFile::AnsiToUnicode(Sect.c_str(), wSection) == false ) { return false; }
	if ( CJetTxtFile::AnsiToUnicode(Name, wName) == false ) { return false; }
	if ( CJetTxtFile::AnsiToUnicode(Value, wVaule) == false ) { return false; }

	CJetIniSection *Ptr=GetSectionPtr(wSection);
	if ( NULL == Ptr )
	{
		AddSection(CJetIniSection(wSection, CJetIniNode(wName, wVaule)));
		return true;
	}
	
	CJetIniNode *NodePtr=Ptr->GetIniNodeByName(wName);
	if ( NULL == NodePtr )
	{
		Ptr->AddIniNode(CJetIniNode(wName, wVaule));
		return true;
	}
	NodePtr->SetValue(wVaule);	
	return true;
}

bool CJetIniFile::AddIniData(const wchar_t *Section, const wchar_t *Name, const wchar_t *Value)
{
	std::wstring wSection=L"["+std::wstring(Section)+L"]";	
	CJetIniSection *Ptr=GetSectionPtr(wSection);	
	if ( NULL == Ptr )
	{
		AddSection(CJetIniSection(wSection, CJetIniNode(Name, Value)));
		return true;
	}
	
	CJetIniNode *NodePtr=Ptr->GetIniNodeByName(Name);
	if ( NULL == NodePtr )
	{
		Ptr->AddIniNode(CJetIniNode(Name, Value));
		return true;
	}
	NodePtr->SetValue(Value);	
	return true;
}

bool CJetIniFile::AddIniData(const char *Section, const char *Name, const char *Value, const TCHAR *Filename)
{
	if ( CheckIniFileLoaded(Filename)==false )
	{	LoadIniFile(Filename);	}
	if ( AddIniData(Section, Name, Value) == false )
	{	return false; }
	return true;
}

bool CJetIniFile::AddIniData(const wchar_t *Section, const wchar_t *Name, const wchar_t *Value, const TCHAR *Filename)
{
	if ( CheckIniFileLoaded(Filename)==false )
	{	LoadIniFile(Filename);	}
	if ( AddIniData(Section, Name, Value) == false )
	{	return false; }
	return true;
}

bool CJetIniFile::GetIniData(const char *Section, const char *Name, const char *Default, char *Value, size_t szValue)
{
	std::wstring wSection, wName, wDefault;
	std::string Sect="["+std::string(Section)+"]";	
	if ( CJetTxtFile::AnsiToUnicode(Sect.c_str(), wSection) == false ) { return false; }
	if ( CJetTxtFile::AnsiToUnicode(Name, wName) == false ) { return false; }	
	if ( CJetTxtFile::AnsiToUnicode(Default, wDefault) == false ) { return false; }

	CJetIniSection *Ptr=GetSectionPtr(wSection);
	if ( NULL == Ptr )
	{
		::strcpy(Value, Default);
		AddSection(CJetIniSection(wSection, CJetIniNode(wName, wDefault)));	
		return true;
	}

	CJetIniNode *NodePtr=Ptr->GetIniNodeByName(wName);
	if ( NULL == NodePtr )
	{
		::strcpy(Value, Default);
		Ptr->AddIniNode(CJetIniNode(wName, wDefault));
		return true;
	}

	size_t CopyLen=0;
	std::string sVaule;
	CJetTxtFile::UnicodeToAnsi(NodePtr->GetValue(), sVaule);
	const size_t len=sVaule.length();
	if ( len < szValue )
	{	CopyLen = len; }
	else
	{	CopyLen = szValue-1; }
	::memcpy(Value, sVaule.c_str(), sizeof(char)*CopyLen);
	Value[CopyLen]='\0';
	return true;	
}

bool CJetIniFile::GetIniData(const wchar_t *Section, const wchar_t *Name, const wchar_t *Default, wchar_t *Value, size_t szValue)
{
	std::wstring wSection=L"["+std::wstring(Section)+L"]";	
	CJetIniSection *Ptr=GetSectionPtr(wSection);
	if ( NULL == Ptr )
	{	
		::wcscpy(Value, Default);
		AddSection(CJetIniSection(wSection, CJetIniNode(Name, Default)));	
		return true;
	}

	CJetIniNode *NodePtr=Ptr->GetIniNodeByName(Name);
	if ( NULL == NodePtr )
	{
		::wcscpy(Value, Default);
		Ptr->AddIniNode(CJetIniNode(Name, Default));
		return true;
	}

	size_t CopyLen=0;
	std::wstring &wVaule=NodePtr->GetValue();
	const size_t len=wVaule.length();
	if ( len < szValue )
	{	CopyLen = len; }
	else
	{	CopyLen = szValue-1; }
	::memcpy(Value, wVaule.c_str(), sizeof(wchar_t)*CopyLen);
	Value[CopyLen]=L'\0';
	return true;
}

bool CJetIniFile::GetIniData(const char *Section, const char *Name, const char *Default, char *Value, size_t szValue, const TCHAR *Filename)
{
	if ( CheckIniFileLoaded(Filename)==false )
	{	LoadIniFile(Filename);	}
	if ( GetIniData(Section, Name, Default, Value, szValue) == false )
	{	return false; }
	return true;
}

bool CJetIniFile::GetIniData(const wchar_t *Section, const wchar_t *Name, const wchar_t *Default, wchar_t *Value, size_t szValue, const TCHAR *Filename)
{
	if ( CheckIniFileLoaded(Filename)==false )
	{	LoadIniFile(Filename);	}
	if ( GetIniData(Section, Name, Default, Value, szValue) == false )
	{	return false; }
	return true;
}