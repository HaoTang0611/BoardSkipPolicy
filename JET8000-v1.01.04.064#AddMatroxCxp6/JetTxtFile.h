#pragma once
#include <string>

enum TXT_FILE_MODE
{
	TXT_FILE_NONE     = 0,
	TXT_FILE_ANSI     = 1,
	TXT_FILE_UTF8     = 2,
	TXT_FILE_UTF8_BOM = 3,
	TXT_FILE_UTF16_LE = 4,
	TXT_FILE_UTF16_BE = 5,
	TXT_FILE_RETURN   = 6
};

class CJetTxtFile
{
private:
	std::string                m_Buffer;//檔案內容
	std::string                m_ErrorString;//錯誤訊息
	TXT_FILE_MODE              m_TxtFileMode;//檔案格式-Ansi, Utf8, Unicode, UnicodeLE

protected:
	std::string&               GetBuffer();//取得檔案內容
	void                       SetBuffer(const std::string &s);//設定檔案內容

	TXT_FILE_MODE              GetTxtFileMode() const;//取得檔案格式
	void                       SetTxtFileMode(TXT_FILE_MODE Mode);//設定檔案格式
	bool                       CheckTxtFileMode(std::string &Buf);//確認檔案格式
	bool                       CheckTxtFileMode_Utf8(const std::string &Buf) const;//確認檔案格式-UTF8

	bool                       ReverseUnicode(std::wstring &ws);//反轉Unicode文字
	bool                       ShiftString(std::string &s, size_t shift);//移動資料起始位置
	bool                       CopyToString(const std::wstring &ws, std::string &s);//將寬字元貼上字元陣列
	bool                       CopyToWString(const std::string &s, std::wstring &ws);//將字元貼上寬字元陣列
	
	bool                       AnsiToUnicode(const std::string &s, std::wstring &ws);//字元轉成Unicode
	bool                       AnsiToUnicode_STL(const std::string &s, std::wstring &ws);
	bool                       AnsiToUnicode_WIN(const std::string &s, std::wstring &ws);
	bool                       UnicodeToAnsi(const std::wstring &ws, std::string &s);//Unicode轉成字元
	bool                       UnicodeToAnsi_STL(const std::wstring &ws, std::string &s);
	bool                       UnicodeToAnsi_WIN(const std::wstring &ws, std::string &s);

	bool                       Utf8ToUnicode(const std::string &s, std::wstring &ws);//Utf8轉成Unicode
	bool                       Utf8ToUnicode_STL(const std::string &s, std::wstring &ws);
	bool                       Utf8ToUnicode_WIN(const std::string &s, std::wstring &ws);
	bool                       UnicodeToUtf8(const std::wstring &ws, std::string &s);//Unicode轉成Utf8
	bool                       UnicodeToUtf8_STL(const std::wstring &ws, std::string &s);
	bool                       UnicodeToUtf8_WIN(const std::wstring &ws, std::string &s);

	bool                       EraseUnicodeEndZero(std::wstring &ws);//移除Unicode結束字元L'\0';
	bool                       ModifyUnicodeForTextFile(const std::wstring &ws, std::wstring &dst);//修改Unicode文字

	bool                       LoadTxtFileKernal(const TCHAR *Filename, std::string &Buf);//載入文字檔案的核心

	bool                       ReturnErrStr_OpenFileFault(const TCHAR *Filename);//回傳錯誤字串-開檔失敗
public:
	CJetTxtFile();
	~CJetTxtFile();	
	
	const char*                GetErrorString() const;//取得錯誤文字
	void                       SetErrorString(const std::string &s);//設定錯誤文字


	bool                       GetTxtData(std::string &str);//取得文字檔案內容
	bool                       GetTxtData(std::wstring &str);//取得文字檔案內容

	bool                       LoadTxtFile(const TCHAR *Filename);//載入文字檔案	

	bool                       SaveTxtFile(const TCHAR *Filename, const std::string &s, TXT_FILE_MODE Mode=TXT_FILE_ANSI);//儲存文字檔案
	bool                       SaveTxtFile(const TCHAR *Filename, const std::wstring &ws, TXT_FILE_MODE Mode=TXT_FILE_UTF8);//儲存文字檔案	
};

