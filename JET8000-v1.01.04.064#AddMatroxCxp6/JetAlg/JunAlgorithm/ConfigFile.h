
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

enum ValueType 
{
	DATATYPE_INTEGER = 1,
	DATATYPE_FLOAT,
	DATATYPE_BOOLEAN,
	DATATYPE_STRING
};

struct ConfigData
{
	string AppName;
	vector<string> vtstrKeyName;

	// pair<string, int> 
	// first = KeyName
	// second = 變數類型(1=整數, 2=浮點數, 3=布林代數, 4=字串).
	vector<pair<string, ValueType>> vtstrKeyNameValue;

	ConfigData() : AppName(""), vtstrKeyName(), vtstrKeyNameValue() {}
	~ConfigData() = default;

	void Initial()
	{
		AppName = "";
		vtstrKeyName.clear();
		vtstrKeyNameValue.clear();
	}

	bool Check() const
	{
		int nCount = vtstrKeyName.size();
		if (nCount != vtstrKeyNameValue.size())
		{
			return false;
		}

		for (const auto& kv : vtstrKeyNameValue)
		{
			if (kv.second < DATATYPE_INTEGER || kv.second > DATATYPE_STRING)
			{
				return false;
			}
		}

		return true;
	}

	void AddKeyValue(const string& strKeyName, const string& strValue, ValueType valueType)
	{
		vtstrKeyName.emplace_back(strKeyName);
		vtstrKeyNameValue.emplace_back(strValue, valueType);
	}
};

class ConfigFile
{
	protected:
		ifstream				m_File_in;
		ofstream				m_File_out;

		// ini檔路徑(結尾要有\\)
		string		m_strFilePath;

		// ini檔檔名(不須副檔名)
		string		m_strFileName;

	public:
		int						m_nAppNameCount;
		vector<ConfigData>		m_vtConfigData;

	public:

		ConfigFile() : m_nAppNameCount(0), m_vtConfigData() {}

		ConfigFile(const string& strPath, const string& strName)
			: m_strFilePath(strPath), m_strFileName(strName), m_nAppNameCount(0), m_vtConfigData() {}

		~ConfigFile() = default;

		int SetPathAndName(string strPath, string strName);
		string GetPathName();

		// 增加資料到 m_vtConfigData
		int SetData(string strAppName, string strKeyName, int nValue);
		int SetData(string strAppName, string strKeyName, double dValue);
		int SetData(string strAppName, string strKeyName, bool bValue);
		int SetData(string strAppName, string strKeyName, string strValue);

		// 儲存 ConfigFile 的成員變數 m_vtConfigData, 存檔路徑為 : m_strFilePath + m_strFileName
		// 呼叫前須先使用 SetPathAndName() 設置 m_strFilePath 與 m_strFileName
		// 存檔類型為 ini
		int Save() { return Save(m_strFilePath + "\\" + m_strFileName, m_vtConfigData); }

		// 儲存 ConfigFile 的成員變數 m_vtConfigData
		// strPathName = 存檔的完整路徑(不包含副檔名)
		// 存檔類型為 ini
		int Save(const string& strPathName) { return  Save(strPathName, m_vtConfigData); }
		
		// strPathName = 存檔的完整路徑(不包含副檔名)
		// vtsData : 要儲存資料
		// nSaveType : 1 => ini
		//           : 2 => txt 
		int Save(const string& strPathName, const vector<ConfigData>& vtsData, const int& nSaveType=1);

		// 讀取檔案到 ConfigFile 的成員變數 m_vtConfigData, 讀檔路徑為 : m_strFilePath + m_strFileName
		// 呼叫前須先使用 SetPathAndName() 設置 m_strFilePath 與 m_strFileName
		// 讀檔類型為 ini
		int Read() { return Read(m_strFilePath + "\\" + m_strFileName, m_vtConfigData); }

		// 讀取檔案到 ConfigFile 的成員變數 m_vtConfigData
		// strPathName = 讀檔的完整路徑(不包含副檔名)
		// 讀檔類型為 ini
		int Read(const string& strPathName) { return Read(strPathName, m_vtConfigData); }

		// strPathName = 讀檔的完整路徑(不包含副檔名)
		// vtsData : 讀取檔案後輸出的資料
		// nReadType : 1 => ini
		//           : 2 => txt 
		int Read(const string& strPathName, vector<ConfigData>& vtsData, const int& nReadType=1);

		void Initial();

		// 根據 AppName + KeyName 取得對應字串值，找不到就傳回 false
		bool FindConfigValue(const vector<ConfigData>& vtsConfig, const string& appName, const string& keyName, string& outValue);

		// 字串轉布林
		bool parseBoolean(const std::string& s);

		// 字串轉整數
		int parseInt(const std::string& s);

		// 字串轉浮點
		float parseFloat(const std::string& s);
private:
	template<typename T>
	int SetDataInternal(const string& strAppName, const string& strKeyName, T value, ValueType eType);

	bool OpenFileForWrite(const string& filePath);

	bool OpenFileForRead(const string& filePath);

	// 輸入檔案路徑 取出檔案內容
	vector<string> ReadAllLines(const string& filePath);

	// 從 Ini讀取檔案
	int ParseIniFile(const vector<string>& lines, vector<ConfigData>& vtsData);

	// 從 Txt讀取檔案
	int ParseTxtFile(const vector<string>& lines, vector<ConfigData>& vtsData);

	// 取得路徑.
	string GetFilePath(const string &strFileName);

	wstring stringToWstring(const std::string& str);
};

// 顯式實例化聲明
//extern template int ConfigFile::SetDataInternal(const string& strAppName, const string& strKeyName, int value, ValueType eType);
//extern template int ConfigFile::SetDataInternal(const string& strAppName, const string& strKeyName, double value, ValueType eType);
//extern template int ConfigFile::SetDataInternal(const string& strAppName, const string& strKeyName, bool value, ValueType eType);