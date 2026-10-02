#pragma once
#include <vector>
#include "JetTxtFile.h"

class CJetIniNode
{
private:
	std::wstring m_Name;
	std::wstring m_Value;
public:
	CJetIniNode()
	{
	}

	CJetIniNode(const wchar_t *name, const wchar_t *value)
	{
		m_Name = name;
		m_Value= value;
	}
	CJetIniNode(const std::wstring &name, const std::wstring &value)
	{
		m_Name = name;
		m_Value= value;
	}

	void Init()
	{
		m_Name = L"";
		m_Value = L"";
	}

	void SetName(const wchar_t *value)
	{	m_Name = value;	}
	void SetName(const std::wstring &name)
	{	m_Name = name;	}
	std::wstring& GetName()
	{	return m_Name; }
	const std::wstring& GetName() const
	{	return m_Name; }
	bool CompareName(const std::wstring &name) const
	{	return m_Name==name;	}

	void SetValue(const wchar_t *value)
	{	m_Value = value;	}
	void SetValue(const std::wstring &value)
	{	m_Value = value;	}	
	std::wstring& GetValue()
	{	return m_Value; }
	const std::wstring& GetValue() const
	{	return m_Value; }

	size_t CalcAllocateSize() const//name=value\n
	{	return m_Name.length()+m_Value.length()+2;	}

	bool AppendString(std::wstring &ws) const
	{
		ws.append(m_Name);
		ws.append(L"=");
		ws.append(m_Value);
		ws.append(L"\n");		
		return true;
	}
};

class CJetIniSection
{
private:
	std::wstring m_Section;
	std::vector<CJetIniNode> m_NodeList;
public:
	CJetIniSection()
	{
	}

	CJetIniSection(const wchar_t *section, const CJetIniNode &Node)
	{
		m_Section = section;
		m_NodeList.push_back(Node);
	}
	CJetIniSection(const std::wstring &section, const CJetIniNode &Node)
	{
		m_Section = section;
		m_NodeList.push_back(Node);
	}

	void Init()
	{
		m_Section=L"";
		m_NodeList.clear();
	}

	bool CheckEmpty() const 
	{	return m_Section.empty();	}

	bool CompareName(const std::wstring &ws) const
	{	return m_Section == ws;	}

	void SetSectionName(const std::wstring &ws)
	{	m_Section = ws;	}

	size_t CalcAllocateSize() const
	{
		size_t i=0, size=0;
		const std::vector<CJetIniNode> &List=m_NodeList;

		size=m_Section.length()+1;//section\n
		for ( auto iter=List.begin(); iter!=List.end(); ++iter )
		{	size += iter->CalcAllocateSize();	}		
		return size;
	}

	CJetIniNode* GetIniNodeByName(const std::wstring &Name)
	{
		size_t i=0;
		std::vector<CJetIniNode> &List=m_NodeList;
		const size_t Cnt=List.size();
		for ( i=0; i<Cnt; i++ )
		{
			CJetIniNode &NodeRef=List[i];
			if ( NodeRef.CompareName(Name) == false ) { continue; }
			return &NodeRef;
		}
		return NULL;		
	}

	bool AddIniNode(const CJetIniNode &Node)
	{
		m_NodeList.push_back(Node);
		return true;
	}

	bool AppendString(std::wstring &ws) const
	{
		ws.append(m_Section);
		ws.append(L"\n");

		const std::vector<CJetIniNode> &List=m_NodeList;
		for ( auto iter=List.begin(); iter!=List.end(); ++iter )
		{	iter->AppendString(ws);	}
		return true;
	}
};

class CJetIniFile:public CJetTxtFile
{
private:
	bool                        m_Opened;
	std::wstring                m_Filename;
	std::vector<CJetIniSection> m_SectionList;

protected:
	bool                       GetFileOpened() const;//取得檔案是否開啟
	void                       SetFileOpened(bool val);//設定檔案是否開啟

	void                       SetFilename(const char *filename);
	void                       SetFilename(const wchar_t *filename);
	bool                       CheckFilenameTheSame(const char *filename);
	bool                       CheckFilenameTheSame(const wchar_t *filename);

	std::vector<CJetIniSection>& GetSectionList();//取得區間列表	
	bool                       BuildSectionList();//建立區間列表
	size_t                     ClacSectionListAllocateSize();//計算區間列表記憶需求尺寸
	bool                       BuildUnicodeString(std::wstring &ws);//建立Unicode寫檔案字串
	bool                       AddSection(const CJetIniSection &Section);//加入區間資料
	CJetIniSection*            GetSectionPtr(const std::wstring &Str);//取得區間指標
	bool                       AddIniString(const std::wstring &Str, CJetIniSection &Section);//加入INI字串	

	bool                       CheckSectionText(const std::wstring &Str);//確認是否為區間文字
	bool                       CheckIniNode(const std::wstring &Str, CJetIniNode &Node);//確認是否為Key=Value資料
	bool                       CheckStringEnd(const std::wstring &Buf, size_t Pos);//確認字串結束
	bool                       ReadString(const std::wstring &Buf, size_t &Pos, std::wstring &String);//讀取字串

public:
	CJetIniFile();
	~CJetIniFile();

	bool                       Initial();
	bool                       LoadIniFile(const TCHAR *Filename);
	bool                       SaveIniFile(const TCHAR *Filename, TXT_FILE_MODE Mode=TXT_FILE_NONE);
	bool                       CheckIniFileLoaded(const TCHAR *Filename);

	CJetIniNode*               GetIniNode(const char *Section, const char *Name);
	CJetIniNode*               GetIniNode(const wchar_t *Section, const wchar_t *Name);

	bool                       AddIniData(const char *Section, const char *Name, const char *Value);
	bool                       AddIniData(const wchar_t *Section, const wchar_t *Name, const wchar_t *Value);
	bool                       AddIniData(const char *Section, const char *Name, const char *Value, const TCHAR *Filename);
	bool                       AddIniData(const wchar_t *Section, const wchar_t *Name, const wchar_t *Value, const TCHAR *Filename);

	bool                       GetIniData(const char *Section, const char *Name, const char *Default, char *Value, size_t szValue);
	bool                       GetIniData(const wchar_t *Section, const wchar_t *Name, const wchar_t *Default, wchar_t *Value, size_t szValue);
	bool                       GetIniData(const char *Section, const char *Name, const char *Default, char *Value, size_t szValue, const TCHAR *Filename);
	bool                       GetIniData(const wchar_t *Section, const wchar_t *Name, const wchar_t *Default, wchar_t *Value, size_t szValue, const TCHAR *Filename);
};

