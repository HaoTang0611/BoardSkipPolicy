
#include "stdafx.h"

#include "ConfigFile.h"

int ConfigFile::SetPathAndName(string strPath, string strName)
{
	if (strPath == "" || strPath == " " || strName == "" || strName == " ")
	{
		return -1;
	}

	m_strFilePath = strPath;
	m_strFileName = strName;
	//string strPathName = m_strFilePath + "\\" + m_strFileName + ".ini";


	// 刪除資料夾.
	//string strPath = GetPreviousFolder(m_strFilePath);
	//myDeleteDirectory((CString)m_strFilePath.c_str());	
	//::Sleep(100);

	// 建立存檔資料夾
	//wstring wstrPath = stringToWstring(strPathName);
	//CreateDirectory(wstrPath.c_str(), NULL);

	return 1;
}

string ConfigFile::GetPathName()
{
	return (m_strFilePath + "\\" + m_strFileName + ".ini");
}

int ConfigFile::SetData(string strAppName, string strKeyName, int nValue)
{
	return SetDataInternal(strAppName, strKeyName, nValue, DATATYPE_INTEGER);
}

int ConfigFile::SetData(string strAppName, string strKeyName, double dValue)
{
	return SetDataInternal(strAppName, strKeyName, dValue, DATATYPE_FLOAT);
}

int ConfigFile::SetData(string strAppName, string strKeyName, bool bValue)
{
	return SetDataInternal(strAppName, strKeyName, bValue ? 1 : 0, DATATYPE_BOOLEAN);
}

int ConfigFile::SetData(string strAppName, string strKeyName, string strValue)
{
	if (strAppName=="" || strAppName==" ")
	{
		return -1;
	}

	//char pchValue[50];
	int nCount = 0;
	bool bThesameApp = false;
	bool bThesameKey = false;

	for (int k=0; k<m_nAppNameCount; ++k)
	{
		if (strAppName == m_vtConfigData[k].AppName)
		{
			bThesameApp = true;
			bThesameKey = false;
			nCount = m_vtConfigData[k].vtstrKeyName.size();

			// 相同 keyName 值覆蓋
			for (int h=0; h<nCount; ++h)	
			{
				if (strKeyName == m_vtConfigData[k].vtstrKeyName[h])
				{
					bThesameKey = true;
					m_vtConfigData[k].vtstrKeyNameValue[h] = make_pair(strValue, DATATYPE_STRING);

					h = nCount;
					k = m_nAppNameCount;
				}
			}

			// 不相同 keyName, 新增欄位
			if(bThesameKey == false)
			{
				m_vtConfigData[k].vtstrKeyName.push_back(strKeyName);
				m_vtConfigData[k].vtstrKeyNameValue.push_back(make_pair(strValue, DATATYPE_STRING));
			}
		}
	}


	if(bThesameApp == false)
	{
		ConfigData CDTemp;
		CDTemp.AppName = strAppName;
		CDTemp.vtstrKeyName.push_back(strKeyName);
		CDTemp.vtstrKeyNameValue.push_back(make_pair(strValue, DATATYPE_STRING));

		m_vtConfigData.push_back(CDTemp);
		++m_nAppNameCount;
	}

	return 1;
}

// strPathName = 檔案的完整路徑(不包含副檔名)
// vtsData : 要儲存資料
// nSaveType : 1 => ini
//           : 2 => txt 
int ConfigFile::Save(const string& strPathName, const vector<ConfigData>& vtsData, const int& nSaveType)
{
	if (strPathName.empty())
	{
		return -1;
	}

	string strExtension = "";
	switch (nSaveType)
	{
	case 1:
		strExtension = ".ini";
		break;
	case 2:
		strExtension = ".txt";
		break;
	default:
		return false;
	}

	string strSavePathName = strPathName + strExtension;
	if (!OpenFileForWrite(strSavePathName))
	{
		return -1;
	}

	for (const auto& config : vtsData)
	{
		m_File_out << "[" << config.AppName << "]" << endl;
		for (size_t i = 0; i < config.vtstrKeyName.size(); ++i)
		{
			m_File_out << config.vtstrKeyName[i] << "=" << config.vtstrKeyNameValue[i].first << endl;
		}
		m_File_out << endl;
	}

	m_File_out.close();

	return 1;
}

// strPathName = 讀檔的完整路徑(不包含副檔名)
// vtsData : 讀取檔案後輸出的資料
// nReadType : 1 => ini
//           : 2 => txt 
int ConfigFile::Read(const string& strPathName, vector<ConfigData>& vtsData, const int& nReadType)
{
	m_nAppNameCount = 0;
	if (strPathName.empty())
	{
		return -1;
	}

	string strExtension = "";
	switch (nReadType)
	{
	case 1:
		strExtension = ".ini";
		break;
	case 2:
		strExtension = ".txt";
		break;
	default:
		return false;
	}

	string strSavePathName = strPathName + strExtension;
	vector<string> lines = ReadAllLines(strSavePathName);
	if (lines.empty())
	{
		return -1;
	}

	vtsData.clear();
	ConfigData currentConfig;
	for (const auto& line : lines)
	{
		if (!line.empty() && line.front() == '[' && line.back() == ']')
		{
			if (!currentConfig.AppName.empty())
			{
				vtsData.push_back(currentConfig);
			}
			currentConfig = ConfigData();
			currentConfig.AppName = line.substr(1, line.length() - 2);
		}
		else if (!line.empty())
		{
			size_t pos = line.find('=');
			if (pos != string::npos)
			{
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1);
				currentConfig.vtstrKeyName.push_back(key);
				currentConfig.vtstrKeyNameValue.push_back(make_pair(value, DATATYPE_INTEGER)); // 假設類型為1，根據實際需求修改
			}
		}
	}

	if (!currentConfig.AppName.empty())
	{
		vtsData.push_back(currentConfig);
	}

	m_nAppNameCount = vtsData.size();

	return 1;
}

void ConfigFile::Initial()
{ 
	m_nAppNameCount = 0; 
	m_vtConfigData.clear(); 
}
//-------------------------------------------
// 小工具：用來從 vtsConfig 裡找出指定 AppName+KeyName 的字串值
//-------------------------------------------
bool ConfigFile::FindConfigValue(const std::vector<ConfigData>& vtsConfig, const std::string& appName, const std::string& keyName, std::string& outValue)
{
	for (const auto& cfg : vtsConfig)
	{
		if (cfg.AppName == appName)
		{
			for (size_t i = 0; i < cfg.vtstrKeyName.size(); ++i)
			{
				if (cfg.vtstrKeyName[i] == keyName)
				{
					outValue = cfg.vtstrKeyNameValue[i].first;
					return true;
				}
			}
		}
	}
	return false;
}

//-------------------------------------------
// 小工具：字串轉布林
//-------------------------------------------
bool ConfigFile::parseBoolean(const std::string& s)
{
	// 視需求調整：常見作法就是 "1" / "true" => true
	// 其他 => false
	if (s == "1" || s == "true" || s == "True")
		return true;
	return false;
}

//-------------------------------------------
// 小工具：字串轉整數
//-------------------------------------------
int ConfigFile::parseInt(const std::string& s)
{
	// 可考慮加上例外處理 try/catch
	return std::stoi(s);
}

//-------------------------------------------
// 小工具：字串轉浮點
//-------------------------------------------
float ConfigFile::parseFloat(const std::string& s)
{
	// 可考慮加上例外處理 try/catch
	return std::stof(s);
}

template<typename T>
int ConfigFile::SetDataInternal(const string& strAppName, const string& strKeyName, T value, ValueType eType)
{
	if (strAppName.empty() || strKeyName.empty())
	{
		return -1;
	}

	string strValue = to_string(value);
	int nCount = 0;
	bool bThesameApp = false;
	bool bThesameKey = false;

	for (int k = 0; k < m_nAppNameCount; ++k)
	{
		if (strAppName == m_vtConfigData[k].AppName)
		{
			bThesameApp = true;
			nCount = m_vtConfigData[k].vtstrKeyName.size();

			for (int h = 0; h < nCount; ++h)
			{
				if (strKeyName == m_vtConfigData[k].vtstrKeyName[h])
				{
					bThesameKey = true;
					m_vtConfigData[k].vtstrKeyNameValue[h] = make_pair(strValue, eType);
					break;
				}
			}

			if (!bThesameKey)
			{
				m_vtConfigData[k].vtstrKeyName.push_back(strKeyName);
				m_vtConfigData[k].vtstrKeyNameValue.push_back(make_pair(strValue, eType));
			}
			break;
		}
	}

	if (!bThesameApp)
	{
		ConfigData CDTemp;
		CDTemp.AppName = strAppName;
		CDTemp.vtstrKeyName.push_back(strKeyName);
		CDTemp.vtstrKeyNameValue.push_back(make_pair(strValue, eType));

		m_vtConfigData.push_back(CDTemp);
		++m_nAppNameCount;
	}

	return 1;
}

bool ConfigFile::OpenFileForWrite(const string& filePath)
{
	m_File_out.open(filePath, ios::out);
	if (!m_File_out)
	{
		wstring wstrPath = stringToWstring(GetFilePath(filePath));
		CreateDirectory(wstrPath.c_str(), NULL);
		m_File_out.open(filePath, ios::out);
		if (!m_File_out)
		{
			return false;
		}
	}
	return true;
}

bool ConfigFile::OpenFileForRead(const string& filePath)
{
	m_File_in.open(filePath);
	return m_File_in.is_open();
}

// 輸入檔案路徑 取出檔案內容
vector<string> ConfigFile::ReadAllLines(const string& filePath)
{
	vector<string> lines;
	if (OpenFileForRead(filePath))
	{
		string line;
		while (getline(m_File_in, line))
		{
			lines.push_back(line);
		}
		m_File_in.close();
	}
	return lines;
}

// 從 Ini讀取檔案
int ConfigFile::ParseIniFile(const vector<string>& lines, vector<ConfigData>& vtsData) 
{
	ConfigData currentConfig;
	for (const auto& line : lines) {
		if (!line.empty() && line.front() == '[' && line.back() == ']') 
		{
			if (!currentConfig.AppName.empty()) 
			{
				vtsData.push_back(currentConfig);
			}
			currentConfig = ConfigData();
			currentConfig.AppName = line.substr(1, line.length() - 2);
		}
		else if (!line.empty()) 
		{
			size_t pos = line.find('=');
			if (pos != string::npos) 
			{
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1);
				currentConfig.vtstrKeyName.push_back(key);
				currentConfig.vtstrKeyNameValue.push_back(make_pair(key, DATATYPE_STRING)); // 假设类型为 STRING，依据实际情况调整
			}
		}
	}
	if (!currentConfig.AppName.empty()) 
	{
		vtsData.push_back(currentConfig);
	}
	return 1;
}

// 從 Txt讀取檔案
int ConfigFile::ParseTxtFile(const vector<string>& lines, vector<ConfigData>& vtsData) 
{
	ConfigData currentConfig;
	currentConfig.AppName = "Default";
	for (const auto& line : lines) 
	{
		if (!line.empty()) {
			size_t pos = line.find('=');
			if (pos != string::npos) {
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1);
				currentConfig.vtstrKeyName.push_back(key);
				currentConfig.vtstrKeyNameValue.push_back(make_pair(key, DATATYPE_STRING)); // 假设类型为 STRING，依据实际情况调整
			}
		}
	}
	if (!currentConfig.vtstrKeyName.empty()) 
	{
		vtsData.push_back(currentConfig);
	}
	return 1;
}

// 取得路徑.
string ConfigFile::GetFilePath(const string &strFileName)
{
	string strName;
	string::size_type idx = strFileName.find_last_of("/\\");

	if (idx == string::npos)	// 沒有'\\'符號 代表輸入的可能是 檔名+副檔名.
	{
		strName = "";
	}
	else						// 有'\\'符號 代表輸入的可能是 路徑+檔名+副檔名.	
	{
		string strTemp = strFileName.substr(idx - 1, 1);
		if (strTemp == "\\")
		{
			strName = strFileName.substr(0, idx);
		}
		else
		{
			strName = strFileName.substr(0, idx + 1);
		}
	}

	return strName;
}

wstring ConfigFile::stringToWstring(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring(L"");

	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;
	return wstr;
}

// 顯式實例化
//template int ConfigFile::SetDataInternal(const string& strAppName, const string& strKeyName, int value, ValueType eType);
//template int ConfigFile::SetDataInternal(const string& strAppName, const string& strKeyName, double value, ValueType eType);
//template int ConfigFile::SetDataInternal(const string& strAppName, const string& strKeyName, bool value, ValueType eType);