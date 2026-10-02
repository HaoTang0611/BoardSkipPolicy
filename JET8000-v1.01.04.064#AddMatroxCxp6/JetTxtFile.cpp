#include "stdafx.h"
#include "JetTxtFile.h"

#include <codecvt>
#define WCHART_CONVERT_STL        1
#define WCHART_CONVERT_WIN        2
//#define WCHART_CONVERT_MODE       WCHART_CONVERT_STL
#define WCHART_CONVERT_MODE       WCHART_CONVERT_WIN

CJetTxtFile::CJetTxtFile()
{
	m_TxtFileMode = TXT_FILE_NONE;
}


CJetTxtFile::~CJetTxtFile()
{
}

std::string& CJetTxtFile::GetBuffer()
{
	return m_Buffer;
}

void CJetTxtFile::SetBuffer(const std::string &s)
{
	m_Buffer = s;
}

TXT_FILE_MODE CJetTxtFile::GetTxtFileMode() const
{
	return m_TxtFileMode;
}

void CJetTxtFile::SetTxtFileMode(TXT_FILE_MODE Mode)
{
	m_TxtFileMode = Mode;
}

bool CJetTxtFile::CheckTxtFileMode(std::string &Buf)
{
	const size_t nReads=Buf.size();
	TXT_FILE_MODE Mode = TXT_FILE_ANSI;		
	unsigned char uch1=0, uch2=0, uch3=0;
	if ( nReads > 3 )
	{
		uch1 = Buf[0];
		uch2 = Buf[1];
		uch3 = Buf[2];		
	}

	if ( 0xEF==uch1 && 0xBB==uch2 && 0xBF==uch3 )
	{
		ShiftString(Buf, 3);
		Mode = TXT_FILE_UTF8_BOM;	
	}
	else if ( 0xFF==uch1 && 0xFE==uch2 )
	{
		ShiftString(Buf, 2);
		Mode = TXT_FILE_UTF16_LE;	
	}
	else if ( 0xFE==uch1 && 0xFF==uch2 )
	{
		ShiftString(Buf, 2);
		Mode = TXT_FILE_UTF16_BE;	
	}
	else if ( CheckTxtFileMode_Utf8(Buf)==true )
	{	Mode = TXT_FILE_UTF8;	}

	SetTxtFileMode(Mode);
	return true;
}

bool CJetTxtFile::CheckTxtFileMode_Utf8(const std::string &Buf) const//確認檔案格式-UTF8
{	
	unsigned char uch=0, uch1=0, uch2=0;
	const int nReads=(int)(Buf.size());
	const int nReads_1 = nReads-1;
	const int nReads_2 = nReads-2;
	for ( int i=0; i<nReads; i++ )
	{
		uch = (unsigned char)(Buf[i]);
		if ( uch < 0x80 )//10000000->0x80為ASCII
		{	continue; }

		if ( uch < 0xC0 )//11000000-Utf8無此區間編碼(0x80~0xC0)
		{	return false;	}

		if ( uch < 0xE0 )//11100000-2字符編碼
		{
			if ( i < nReads_1 )
			{
				uch1 = (unsigned char)(Buf[i+1]);
				if ( (uch1&0xC0) !=0x80 )
				{	return false; }
				i ++;
			}
			continue;
		}
		if ( uch < 0xF0 )//11110000-Utf8-3字符編碼
		{
			if ( i < nReads_2 )
			{
				uch1 = (unsigned char)(Buf[i+1]);
				uch2 = (unsigned char)(Buf[i+2]);
				if ( (uch1&0xC0)!=0x80 || (uch2&0xC0)!=0x80 )
				{	return false; }
				i +=2 ;
			}
			continue;
		}
		return false;		
	}
	return true;
}

bool CJetTxtFile::ReverseUnicode(std::wstring &ws)
{
	const size_t szW=ws.size();
	const size_t szA=szW*2;

	char t1=0, t2=0;
	char *p=(char*)(&(ws.front()));
	for ( size_t i=0; i<szA; i+=2 )
	{
		t1=p[i];
		t2=p[i+1];
		p[i]=t2;
		p[i+1]=t1;
	}
	return true;
}

bool CJetTxtFile::ShiftString(std::string &s, size_t shift)
{	
	const size_t sz=s.size();
	if ( shift >= sz ) { return false; }

	size_t i=0;
	for ( i=0; i<sz-shift; i++ )
	{	s[i]=s[i+shift];	}	
	for ( i; i<sz; i++ )
	{	s[i]='\0';	}
	return true;
}

bool CJetTxtFile::CopyToString(const std::wstring &ws, std::string &s)
{
	const size_t szW=ws.size();
	const size_t szA=szW*2;	
	s.resize(szA+1);
	::memcpy(&(s.front()), &(ws.front()), sizeof(wchar_t)*szW);
	s[szA]='\0';
	return true;
}


bool CJetTxtFile::CopyToWString(const std::string &s, std::wstring &ws)
{	
	const size_t szA=s.size();
	const size_t szW=szA/2;
	ws.resize(szW+1);
	::memcpy(&(ws.front()), &(s.front()), sizeof(char)*szA);
	ws[szW]=L'\0';
	return true;
}

bool CJetTxtFile::AnsiToUnicode(const std::string &s, std::wstring &ws)
{
#if WCHART_CONVERT_MODE == WCHART_CONVERT_STL
	return AnsiToUnicode_STL(s, ws);
#endif //WCHART_CONVERT_MODE
	return AnsiToUnicode_WIN(s, ws);
}
bool CJetTxtFile::AnsiToUnicode_STL(const std::string &s, std::wstring &ws)
{	//測試出來有問題
	try
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
		ws=converter.from_bytes(s);
	}
	catch ( const std::exception &e )
	{
		SetErrorString(e.what());
		return false;	
	}
	return true;
}

bool CJetTxtFile::AnsiToUnicode_WIN(const std::string &s, std::wstring &ws)
{
	size_t len = s.length();
	const UINT CodePage=CP_ACP;
    size_t unicodeLen = ::MultiByteToWideChar(CodePage, 0, s.c_str(), -1, NULL, 0);
	ws.resize(unicodeLen);
    ::MultiByteToWideChar(CodePage, 0, s.c_str(), -1, &(ws.front()), unicodeLen);
	//ws[unicodeLen-1]=L'\0';    
	EraseUnicodeEndZero(ws);
	return true;
}

bool CJetTxtFile::UnicodeToAnsi(const std::wstring &ws, std::string &s)
{
#if WCHART_CONVERT_MODE == WCHART_CONVERT_STL
	return UnicodeToAnsi_STL(ws, s);	
#endif //WCHART_CONVERT_MODE
	return UnicodeToAnsi_WIN(ws, s);	
}

bool CJetTxtFile::UnicodeToAnsi_STL(const std::wstring &ws, std::string &s)
{	//測試出來有問題
	try
	{		
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		s = converter.to_bytes(ws);			
	}
	catch ( const std::exception &e )
	{
		SetErrorString(e.what());
		return false;	
	}
	return true;
}

bool CJetTxtFile::UnicodeToAnsi_WIN(const std::wstring &ws, std::string &s)
{
	size_t len = ws.length();
	const UINT CodePage=CP_ACP;
	size_t AnsiLen=::WideCharToMultiByte(CodePage, 0, ws.c_str(), -1, NULL, 0, NULL, NULL);
	s.resize(AnsiLen);    
    ::WideCharToMultiByte(CodePage, 0, ws.c_str(), len, &(s.front()), AnsiLen, NULL, NULL);
    //s[AnsiLen]='\0';
    return true;
}

bool CJetTxtFile::Utf8ToUnicode(const std::string &s, std::wstring &ws)
{
#if WCHART_CONVERT_MODE == WCHART_CONVERT_STL
	return Utf8ToUnicode_STL(s, ws);
#endif //WCHART_CONVERT_MODE
	return Utf8ToUnicode_WIN(s, ws);
}

bool CJetTxtFile::Utf8ToUnicode_STL(const std::string &s, std::wstring &ws)
{
	try
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		ws=converter.from_bytes(s);
	}
	catch ( const std::exception &e )
	{
		SetErrorString(e.what());
		return false;	
	}
	return true;
}

bool CJetTxtFile::Utf8ToUnicode_WIN(const std::string &s, std::wstring &ws)
{
	size_t len = s.length();
	const UINT CodePage=CP_UTF8;
    size_t unicodeLen = ::MultiByteToWideChar(CodePage, 0, s.c_str(), -1, NULL, 0);
	ws.resize(unicodeLen);
    ::MultiByteToWideChar(CodePage, 0, s.c_str(), -1, &(ws.front()), unicodeLen);
	//ws[unicodeLen-1]=L'\0';    
	EraseUnicodeEndZero(ws);
	return true;
}

bool CJetTxtFile::UnicodeToUtf8(const std::wstring &ws, std::string &s)
{
#if WCHART_CONVERT_MODE == WCHART_CONVERT_STL
	return UnicodeToUtf8_STL(ws, s);
#endif //WCHART_CONVERT_MODE
	return UnicodeToUtf8_WIN(ws, s);
}

bool CJetTxtFile::UnicodeToUtf8_STL(const std::wstring &ws, std::string &s)
{
	try
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		s = converter.to_bytes(ws);	
	}
	catch ( const std::exception &e )
	{
		SetErrorString(e.what());
		return false;	
	}
	return true;
}

bool CJetTxtFile::UnicodeToUtf8_WIN(const std::wstring &ws, std::string &s)
{
	size_t len = ws.length();
	const UINT CodePage=CP_UTF8;
	size_t AnsiLen=::WideCharToMultiByte(CodePage, 0, ws.c_str(), -1, NULL, 0, NULL, NULL);
	s.resize(AnsiLen);    
    ::WideCharToMultiByte(CodePage, 0, ws.c_str(), len, &(s.front()), AnsiLen, NULL, NULL);
    //s[AnsiLen]='\0';	
    return true;
}

bool CJetTxtFile::EraseUnicodeEndZero(std::wstring &ws)
{
	size_t len=ws.length();
	if ( L'\0' != ws[len-1] )
	{	return true;	}
	ws.resize(len-1);
	return true;
}

bool CJetTxtFile::ModifyUnicodeForTextFile(const std::wstring &ws, std::wstring &dst)
{
	size_t i=0, j=0;	
	const size_t sz=ws.size();
	dst.resize(sz*2+1);
	for ( i=0; i<sz; i++ )
	{
		dst[j] = ws[i];
		if ( 0x000A == ws[i] )
		{				
			dst[j]=0x000D; j++;
			dst[j]=0x000A;
		}
		j ++;
	}	
	dst[j]=0x0000; j++;
	dst.resize(j);
	return true;
}

const char* CJetTxtFile::GetErrorString() const
{
	return m_ErrorString.c_str();
}

void  CJetTxtFile::SetErrorString(const std::string &s)
{
	m_ErrorString = s;
}


bool CJetTxtFile::GetTxtData(std::string &str)
{
	std::string &Buf=GetBuffer();
	TXT_FILE_MODE Mode=GetTxtFileMode();
	if ( TXT_FILE_ANSI==Mode )
	{	str=Buf;	}
	else	
	{
		std::wstring sUni;
		if ( GetTxtData(sUni) == false )
		{	return false; }
		UnicodeToAnsi(sUni, str);
	}	
	return true;
}

bool CJetTxtFile::GetTxtData(std::wstring &str)
{
	std::string &Buf=GetBuffer();
	TXT_FILE_MODE Mode=GetTxtFileMode();
	if ( TXT_FILE_ANSI==Mode )
	{	
		AnsiToUnicode(Buf, str);	
		return true;
	}
	if ( TXT_FILE_UTF8==Mode || TXT_FILE_UTF8_BOM==Mode )
	{	
		Utf8ToUnicode(Buf, str);	
		return true;
	}
	if ( TXT_FILE_UTF16_LE==Mode )
	{	
		CopyToWString(Buf, str);	
		return true;
	}
	if ( TXT_FILE_UTF16_BE==Mode )
	{	
		CopyToWString(Buf, str);
		ReverseUnicode(str);
		return true;
	}	
	SetErrorString("Error, Txt File Mode Exception");
	return false;
}

bool CJetTxtFile::ReturnErrStr_OpenFileFault(const TCHAR *Filename)
{
	std::string Name;
	std::wstring wName;
#ifndef UNICODE
	Name = Filename;	
#else
	wName = Filename;
	UnicodeToUtf8(wName, Name);
#endif//UNICODE

	std::string Err;
	Err=std::string("Error, Open File Fanult\n")+Name;
	SetErrorString(Err);
	return false;
}

bool CJetTxtFile::LoadTxtFile(const TCHAR *Filename)
{
	SetTxtFileMode(TXT_FILE_NONE);
	std::string &Buf=GetBuffer();
	Buf.clear();	
	if ( LoadTxtFileKernal(Filename, Buf) == false )
	{	return false; }
	return true;
}

bool CJetTxtFile::LoadTxtFileKernal(const TCHAR *Filename, std::string &Buf)
{	
	FILE *pfile=::_tfopen(Filename, _T("rb+"));
	if ( NULL == pfile )
	{	
		ReturnErrStr_OpenFileFault(Filename);
		return false; 
	}

	size_t len=0;
	size_t nReads=0;
#ifndef _X64
	::fseek(pfile, 0, SEEK_END);
	len=::ftell(pfile);
	::fseek(pfile, 0, SEEK_SET);
#else
	::_fseeki64(pfile, 0, SEEK_END);
	len=::_ftelli64(pfile);
	::_fseeki64(pfile, 0, SEEK_SET);
#endif//_X64
	if ( 0 == len )
	{		
		::fclose(pfile); pfile=NULL;
		SetErrorString("Error, the File is Empty");
		return false;
	}
	
	Buf.resize(len+1);			
	nReads=::fread(&Buf.front(), sizeof(char), len, pfile);
	if ( nReads > len ) 
	{	Buf[len]='\0'; }
	else
	{	Buf[nReads]='\0';	}
	::fclose(pfile); pfile=NULL;

	if ( CheckTxtFileMode(Buf) == false )
	{	return false; }
	return true;
}

bool CJetTxtFile::SaveTxtFile(const TCHAR *Filename, const std::string &s, TXT_FILE_MODE Mode)
{	
	const bool bKeep=true;//是否保留
	if ( TXT_FILE_ANSI == Mode )
	{
		FILE *pfile=::_tfopen(Filename, _T("w+"));
		if ( NULL == pfile ) 
		{
			ReturnErrStr_OpenFileFault(Filename);
			return false; 
		}
		::fwrite(s.c_str(), sizeof(char), s.length(), pfile);
		::fclose(pfile); 
		pfile = NULL;

		if ( bKeep )
		{
			SetBuffer(s);
			SetTxtFileMode(Mode);
		}
		return true;
	}

	if ( TXT_FILE_UTF8==Mode || TXT_FILE_UTF8_BOM==Mode )
	{	
		std::string  sUtf;
		std::wstring sUni;
		AnsiToUnicode(s, sUni);
		UnicodeToUtf8(sUni, sUtf);
		unsigned char header[3]={0xEF, 0xBB, 0xBF};		
		FILE *pfile=::_tfopen(Filename, _T("w+"));
		if ( NULL == pfile ) 
		{
			ReturnErrStr_OpenFileFault(Filename);
			return false; 
		}
		if ( TXT_FILE_UTF8_BOM == Mode )
		{	::fwrite(header, sizeof(char), sizeof(header), pfile);	}
		::fwrite(sUtf.c_str(), sizeof(char), sUtf.length(), pfile);
		::fclose(pfile); 
		pfile = NULL;

		if ( bKeep )
		{
			SetBuffer(sUtf);
			SetTxtFileMode(Mode);
		}
		return true;
	}

	if ( TXT_FILE_UTF16_LE==Mode )
	{			
		std::wstring sUni, sUni2;
		AnsiToUnicode(s, sUni2);
		ModifyUnicodeForTextFile(sUni2, sUni);
		unsigned char header[2]={0xFF, 0xFE};
		FILE *pfile=::_tfopen(Filename, _T("wb"));
		if ( NULL == pfile ) 
		{
			ReturnErrStr_OpenFileFault(Filename);
			return false; 
		}
		::fwrite(header, sizeof(char), sizeof(header), pfile);
		::fwrite(sUni.c_str(), sizeof(wchar_t), sUni.length(), pfile);		
		::fclose(pfile); 
		pfile = NULL;	

		if ( bKeep )
		{
			SetTxtFileMode(Mode);
			CopyToString(sUni, GetBuffer());
		}
		return true;
	}

	if ( TXT_FILE_UTF16_BE == Mode )
	{			
		std::wstring sUni, sUni2;
		AnsiToUnicode(s, sUni2);
		ModifyUnicodeForTextFile(sUni2, sUni);
		ReverseUnicode(sUni);
		unsigned char header[2]={0xFE, 0xFF};
		FILE *pfile=::_tfopen(Filename, _T("wb"));
		if ( NULL == pfile )
		{
			ReturnErrStr_OpenFileFault(Filename);
			return false; 
		}
		::fwrite(header, sizeof(char), sizeof(header), pfile);		
		::fwrite(sUni.c_str(), sizeof(wchar_t), sUni.length(), pfile);		
		::fclose(pfile); 
		pfile = NULL;	

		if ( bKeep )
		{
			SetTxtFileMode(Mode);
			CopyToString(sUni, GetBuffer());
		}
		return true;
	}
	return true;
}

bool CJetTxtFile::SaveTxtFile(const TCHAR *Filename, const std::wstring &ws, TXT_FILE_MODE Mode)
{
	const bool bKeep=false;//是否保留
	if ( TXT_FILE_ANSI == Mode )
	{
		std::string s;
		UnicodeToAnsi(ws, s);		
		FILE *pfile=::_tfopen(Filename, _T("w+"));
		if ( NULL == pfile ) 
		{
			ReturnErrStr_OpenFileFault(Filename);
			return false; 
		}
		::fwrite(s.c_str(), sizeof(char), s.length(), pfile);
		::fclose(pfile); 
		pfile = NULL;

		if ( bKeep )
		{
			SetBuffer(s);
			SetTxtFileMode(Mode);
		}
		return true;
	}

	if ( TXT_FILE_UTF8==Mode || TXT_FILE_UTF8_BOM==Mode )
	{
		std::string s;
		unsigned char header[3]={0xEF, 0xBB, 0xBF};		
		UnicodeToUtf8(ws, s);
		FILE *pfile=::_tfopen(Filename, _T("w+"));
		if ( NULL == pfile ) 
		{
			ReturnErrStr_OpenFileFault(Filename);
			return false; 
		}
		if ( TXT_FILE_UTF8_BOM == Mode )
		{	::fwrite(header, sizeof(char), sizeof(header), pfile); }
		::fwrite(s.c_str(), sizeof(char), s.length(), pfile);
		::fclose(pfile); 
		pfile = NULL;

		if ( bKeep )
		{
			SetBuffer(s);
			SetTxtFileMode(Mode);
		}
		return true;
	}
	if ( TXT_FILE_UTF16_LE == Mode )
	{	
		std::wstring ws2;
		ModifyUnicodeForTextFile(ws, ws2);
		unsigned char header[2]={0xFF, 0xFE};
		FILE *pfile=::_tfopen(Filename, _T("wb"));
		if ( NULL == pfile ) 
		{
			ReturnErrStr_OpenFileFault(Filename);
			return false;
		}
		::fwrite(header, sizeof(char), sizeof(header), pfile);
		::fwrite(ws2.c_str(), sizeof(wchar_t), ws2.length(), pfile);		
		::fclose(pfile); 
		pfile = NULL;

		if ( bKeep )
		{
			SetTxtFileMode(Mode);
			CopyToString(ws2, GetBuffer());
		}
		return true;
	}
	if ( TXT_FILE_UTF16_BE == Mode )
	{
		std::wstring ws2;
		ModifyUnicodeForTextFile(ws, ws2);
		ReverseUnicode(ws2);
		unsigned char header[2]={0xFE, 0xFF};
		FILE *pfile=::_tfopen(Filename, _T("wb"));
		if ( NULL == pfile )
		{
			ReturnErrStr_OpenFileFault(Filename);
			return false; 
		}
		::fwrite(header, sizeof(char), sizeof(header), pfile);
		::fwrite(ws2.c_str(), sizeof(wchar_t), ws2.length(), pfile);		
		::fclose(pfile); 
		pfile = NULL;

		if ( bKeep )
		{
			SetTxtFileMode(Mode);
			CopyToString(ws2, GetBuffer());
		}
		return true;
	}	
	return true;
}

