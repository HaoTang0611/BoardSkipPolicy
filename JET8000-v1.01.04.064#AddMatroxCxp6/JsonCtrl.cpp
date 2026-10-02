#include "stdafx.h"
#include "JsonCtrl.h"
#include "rapidjson/stringbuffer.h"  
#include "rapidjson/writer.h"
#include "rapidjson/encodings.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filereadstream.h"   // FileReadStream
#include "rapidjson/encodedstream.h"    // EncodedInputStream
#include "rapidjson/filewritestream.h"
#include <fstream>
#include <algorithm>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace rapidjson
{
//private
bool CJsonCtrl::FindObject(const std::vector<std::wstring> &rParentNameList, CGMItr &rGMItr)
{
	if (nullptr != m_pDoc)
	{
		if (FindMember(*m_pDoc, rParentNameList, rGMItr))
		{
			if (rGMItr->value.IsObject())
			{
				return true;
			}
		}
	}

	return false;
}

//public
CJsonCtrl::CJsonCtrl() : m_pDoc(nullptr)
{
	m_ErrStr = "";
}

CJsonCtrl::~CJsonCtrl()
{
}

bool CJsonCtrl::OpenFile(const wchar_t *pFilePath, WDocument &rDoc)
{
	FILE *fp = nullptr;
	try
	{
		errno_t err = _wfopen_s(&fp, pFilePath, L"rb"); // non-Windows use "r"

		if (0 != err)
		{
			throw std::exception("The file open fail!!");
		}
	#ifndef _X64
		fseek(fp, 0, SEEK_END); // seek to end of file
		auto nLen = ftell(fp) + 10; // get current file pointer
		fseek(fp, 0, SEEK_SET); // seek back to beginning of file	
	#else
		_fseeki64(fp, 0, SEEK_END); // seek to end of file
		auto nLen = _ftelli64(fp) + 10; // get current file pointer
		_fseeki64(fp, 0, SEEK_SET); // seek back to beginning of file	
	#endif//_X64

		std::string strBuffer(nLen, '\0');
		FileReadStream bis(fp, &strBuffer.at(0), strBuffer.length());
		AutoUTFInputStream<unsigned, FileReadStream> eis(bis);  // wraps bis into eis

		if (rDoc.ParseStream<rapidjson::ParseFlag::kParseCommentsFlag, AutoUTF<unsigned> >(eis).HasParseError()
			|| (kParseErrorNone != rDoc.GetParseError() ) )
		{	
			throw std::exception("json parse error");			
		}

		::fclose(fp);
	}
	catch (std::exception e)
	{
		m_ErrStr = e.what();
		if (fp) ::fclose(fp);
		return false;
	}

	return true;
}

bool CJsonCtrl::SetBuffer(const std::wstring &rBuf, WDocument &rDoc)
{
	typedef GenericStringStream<UTF16<> > StringStreamW;
	StringStreamW str(rBuf.c_str());
	//
	if (rDoc.ParseStream(str).HasParseError()
		|| (kParseErrorNone != rDoc.GetParseError() ) )
	{		
		ParseErrorCode eCode=rDoc.GetParseError();
		//throw std::invalid_argument("json parse error");
		return false;
	}

	return true;
}


bool CJsonCtrl::SetBuffer(const StringBufferW &rBuf, WDocument &rDoc)
{
	return true;
}

bool CJsonCtrl::SaveFile(const wchar_t *pFilePath, const WDocument &rDoc)
{
	FILE *fp = nullptr;
	errno_t err = _wfopen_s(&fp, pFilePath, L"wb"); // non-Windows use "w"

	//const int kBLen = 20 * 1024 * 1024;
	//auto kBLen = ( CountMember(rDoc.MemberBegin(), rDoc.MemberEnd() ) ) * 128;
	auto kBLen = (EstimatSize(rDoc.MemberBegin(), rDoc.MemberEnd()) << 1);

	std::string strBuffer(kBLen, '\0');	

	FileWriteStream bos(fp, &strBuffer.at(0), kBLen);    
	typedef rapidjson::EncodedOutputStream<rapidjson::UTF8<>, rapidjson::FileWriteStream> OutputStream;
	OutputStream eos(bos, true);   // Write BOM    
	rapidjson::Writer<OutputStream, rapidjson::UTF16<>, rapidjson::UTF8<>> writer(eos);
    rDoc.Accept(writer);
	fclose(fp);
	return true;
}

bool CJsonCtrl::SaveFileIncSpace(const wchar_t *pFilePath, const WDocument &rDoc)
{
	std::wstring wsBuffer;
	if ( GetBufferIncSpace(wsBuffer, rDoc) == false ) 
	{	return false; }

	FILE *fp = nullptr;
	const size_t szBufferLen=wsBuffer.size();
	errno_t err = _wfopen_s(&fp, pFilePath, L"wb"); // non-Windows use "w"	
	if ( NULL != fp )
	{		
		::fwrite(wsBuffer.c_str(), sizeof(wchar_t)*szBufferLen, 1, fp);
		::fclose(fp); fp=NULL;
	}	
	return true;
}

bool CJsonCtrl::GetBuffer(std::wstring &rBuf, const WDocument &rDoc)
{
	StringBufferW strbuf;
	//Writer<StringBufferW, UTF16<> > writer(strbuf);
	Writer<StringBufferW, UTF16<>, UTF16LE<> > writer(strbuf);
	rDoc.Accept(writer);

	rBuf.assign(strbuf.GetString());
	return true;
}

bool CJsonCtrl::GetBufferIncSpace(std::wstring &rBuf, const WDocument &rDoc)
{
	std::wstringstream ss;

	const std::wstring WhiteSpace(L" ");

	ss << L"{" << L"\r\n";
	ss << WhiteSpace;
	ParserAll(ss, rDoc.MemberBegin(), rDoc.MemberEnd(), WhiteSpace);
	ss << WhiteSpace;
	ss << L"}" << L"\r\n";

	rBuf = ss.str();

	return true;
}

bool CJsonCtrl::FindMember(const WDocument &rDoc, const std::wstring &rStr, CGMItr &rValue)
{
	auto tValue = FindMember(rDoc, rStr);

	if (rDoc.MemberEnd() != tValue )
	{
		rValue = tValue;
		return true;
	}

	return false;
}

bool CJsonCtrl::FindMember(const WDocument &rDoc, const std::vector<std::wstring> &rStrList, CGMItr &rValue)
{	
	CGMItr Value;	
	try
	{	
		if (rDoc.MemberEnd() == (Value = FindMember(rDoc, rStrList[0]) ) )
		{
			throw std::exception("String level 1 is not exist!");
		}

		unsigned int Succ = 2;
		std::for_each(rStrList.begin() + 1, rStrList.end(), [&](const auto &rStr)
		{			
			auto ValueLn = FindMember(Value->value.GetObject(), rStr);

			if (Value->value.MemberEnd() != ValueLn )
			{			
				Value = ValueLn;
				++Succ;
			}
			else
			{
				char Err[64];
				sprintf_s(Err, "String level %d is not exist!", Succ);
				throw std::exception(Err);
			}
		});
	}
	catch (std::exception e)
	{
		return false;
	}

	rValue = Value;

	return true;
}

bool CJsonCtrl::FindMember(const std::wstring &rStr, CGMItr &rValue)
{
	return (nullptr == m_pDoc) ? false : FindMember(*m_pDoc, rStr, rValue);
}

bool CJsonCtrl::FindMember(const std::vector<std::wstring> &rStrList, CGMItr &rValue)
{
	return (nullptr == m_pDoc) ? false : FindMember(*m_pDoc, rStrList, rValue);
}

bool CJsonCtrl::FindMember(CGMItr &rObject, const std::wstring &rStr, CGMItr &rValue)
{
	if (rObject->value.IsObject())
	{
		rValue = FindMember(rObject->value.GetObject(), rStr);

		if (rObject->value.MemberEnd() != rValue)
			return true;
	}

	return (false);
}

bool CJsonCtrl::AddObject(WValue *Parent, const std::wstring &rName, WValue &rValue)
{
	if ((nullptr == m_pDoc) || (nullptr == Parent))
		return false;

	AddMemberT(Parent, rName.c_str(), rValue);

	return true;
}

bool CJsonCtrl::EraseObjectMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName)
{
	CGMItr ItrValue;
	if (FindObject(rParentNameList, ItrValue))
	{
		WValue *pObj = const_cast<WValue *>(&ItrValue->value);
		pObj->EraseMember(StringW(rName.c_str()));
		return true;
	}

	return false;
}

bool CJsonCtrl::EraseObjectMember(const std::vector<std::wstring> &rParentNameList)
{
	CGMItr ItrValue;
	if (FindObject(rParentNameList, ItrValue))
	{
		WValue *pObj = const_cast<WValue *>(&ItrValue->value);
		pObj->EraseMember(pObj->MemberBegin(), pObj->MemberEnd());
		return true;
	}

	return false;
}

bool CJsonCtrl::EraseMember(const std::wstring &rName)
{	
	if (m_pDoc->HasMember(rName.c_str() ) )
	{		
		m_pDoc->EraseMember(rName.c_str());
		return true;
	}
	return false;
}

void CJsonCtrl::Set(WDocument *pDoc)
{
	m_pDoc = pDoc;
}

WDocument *CJsonCtrl::Get()
{
	return m_pDoc;
}

const std::string &CJsonCtrl::GetLastErrorStr() const
{
	return this->m_ErrStr;
}


}