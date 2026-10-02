#ifndef _JSON_CTRL_FILE_H_
#define _JSON_CTRL_FILE_H_


#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include <vector>
#include <utility>
#include <iomanip>      // std::setprecision

namespace rapidjson
{

typedef GenericMemberIterator<true,UTF16<>,MemoryPoolAllocator<> > CGMItr;
typedef GenericStringBuffer<UTF16<> > StringBufferW;

class CJsonCtrl
{	
	#define NewWVStr(C_WCHAR_PTR) WValue(C_WCHAR_PTR, wcslen(C_WCHAR_PTR), m_pDoc->GetAllocator() )

	typedef GenericStringRef<wchar_t> StringW;

	WDocument *m_pDoc;
	
	template <typename T>
	CGMItr FindMember(T &rObj, const std::wstring &rstrMember)
	{	
		auto Itr = rObj.FindMember(rstrMember.c_str());
		if (rObj.MemberEnd() ==  Itr)
		{		
			for (auto m = rObj.MemberBegin(); m != rObj.MemberEnd(); ++m)
			{
				if (m->value.IsObject())
				{
					auto NextObj = m->value.GetObject();
					auto Itr2 = FindMember(NextObj, rstrMember);
					if (NextObj.MemberEnd() != Itr2)
					{
						return Itr2;
					}
				}
			}			
		}
		else
		{		
			return Itr;
		}

		return rObj.MemberEnd();
	}
	
	template <typename T1, typename T2>
	size_t EstimatSize(T1 MB, T2 ME)
	{
		size_t nSize = 0;
		for (auto m = MB; m != ME; ++m)
		{
			auto pName = m->name.GetString();
			nSize += (wcslen(pName) << 1) + 8;
			switch (m->value.GetType())
			{
			case kStringType:
				nSize += (wcslen(m->value.GetString()) << 1) + 8;
				break;
			case kArrayType:
			{
				auto arrayV = m->value.GetArray();
				for (auto itr = arrayV.Begin(); itr != arrayV.End(); ++itr)
				{
					if (itr->IsString())
						nSize += (wcslen(itr->GetString()) << 1) + 8;
					else
						nSize += 32;
				}
			}
			break;
			case kNumberType:
			{
				nSize += 32;
			}
			break;
			case kObjectType:
			{
				nSize += EstimatSize(m->value.MemberBegin(), m->value.MemberEnd());
			}
			break;
			default:
				break;
			}
		}

		return nSize;
	}

	template <typename T1, typename T2>
	size_t CountMember(T1 MB, T2 ME)
	{	
		size_t nSize = 0;
		for (auto m = MB;m != ME; ++m)
		{
			++nSize;
			if (m->value.IsObject() )
			{
				nSize += CountMember(m->value.MemberBegin(), m->value.MemberEnd() );
			}			
		}

		return nSize;
	}	

	template <typename T1, typename T2>
	void ParserAll(std::wstringstream &ss, T1 MB, T2 ME, const std::wstring &rWhiteSpace)
	{
		for (auto m = MB; m != ME; ++m)
		{			
			auto pName = m->name.GetString();

			switch (m->value.GetType())
			{
			case kStringType:
				ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : \"" << m->value.GetString() << L"\"";
				ss << ",\r\n";
				break;
			case kArrayType:
			{
				ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : [";
				auto arrayV = m->value.GetArray();
				std::wstringstream Arrstream;
				for (auto itr = arrayV.Begin(); itr != arrayV.End(); ++itr)
				{
					if (itr->IsInt())
					{	Arrstream << itr->GetInt() << L","; }
					else if (itr->IsDouble())
					{	Arrstream << std::fixed << std::setprecision(16) << itr->GetDouble() << L","; }
					else if (itr->IsString())
					{	Arrstream << L"\"" << itr->GetString() << L"\","; }	
					else if (itr->IsBool())
					{
						if (itr->GetBool())
						{	Arrstream << itr->GetInt() << L"true,"; }
						else
						{	Arrstream << itr->GetInt() << L"false,"; }
					}
					else if (itr->IsUint())
					{	Arrstream << itr->GetUint() << L","; }
					else if (itr->IsInt64())
					{	Arrstream << itr->GetInt64() << L","; }
					else if (itr->IsUint64())
					{	Arrstream << itr->GetUint64() << L","; }
					else if (itr->IsFloat())
					{	Arrstream << std::fixed << std::setprecision(7) << itr->GetFloat() << L","; }
				}
				auto strtmp = Arrstream.str();
				if ( strtmp.length() > 0 )
				{	strtmp.pop_back();	}
				ss << strtmp << L"]";
				ss << ",\r\n";
			}
			break;
			case kNumberType:
			{
				if (m->value.IsInt())
				{	ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : " << m->value.GetInt();	}
				else if (m->value.IsDouble())
				{
					ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : ";
					ss << std::fixed << std::setprecision(16) << m->value.GetDouble();
				}
				else if (m->value.IsUint())
				{	ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : " << m->value.GetUint();	}
				else if (m->value.IsInt64())
				{	ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : " << m->value.GetInt64();	}
				else if (m->value.IsUint64())
				{	ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : " << m->value.GetUint64();	}
				else if (m->value.IsFloat())
				{
					ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : ";
					ss << std::fixed << std::setprecision(7) << m->value.GetFloat();
				}
				ss << ",\r\n";
			}
			break;
			case kObjectType:
			{
				ss << rWhiteSpace.c_str() << L"\"" << pName << L"\" : " << L"\r\n";
				ss << rWhiteSpace.c_str() << L"{" << L"\r\n";
				auto Obj = m->value.GetObject();
				ParserAll(ss, Obj.MemberBegin(), Obj.MemberEnd(), rWhiteSpace + std::wstring(L"    "));
				ss << rWhiteSpace.c_str() << L"}" << L"\r\n";
			}
			break;
			default:
				break;
			}
		}
	}

	template<typename T1, typename T2>
	void AddObjectPaser(WValue *pObj, T1 MBegin, T2 MEnd)
	{
		for (auto m = MBegin; m != MEnd; ++m)
		{
			auto MName = m->name.GetString();
			if (m->value.IsString())
			{
				AddMemberT(pObj, MName, NewWVStr(m->value.GetString()));
			}
			else if (m->value.IsArray())
			{
				WValue wvArray(rapidjson::kArrayType);
				//copy array value
				auto arrayV = m->value.GetArray();
				for (auto itr = arrayV.Begin(); itr != arrayV.End(); ++itr)
				{
					if (itr->IsInt())
						wvArray.PushBack(itr->GetInt(), m_pDoc->GetAllocator());
					else if (itr->IsString())
					{
						wvArray.PushBack(NewWVStr(itr->GetString()), m_pDoc->GetAllocator());
					}
					else if (itr->IsDouble())
						wvArray.PushBack(itr->GetDouble(), m_pDoc->GetAllocator());
				}
				AddMemberT(pObj, MName, wvArray);
			}
			else if (m->value.IsInt())
			{
				AddMemberT(pObj, MName, m->value.GetInt());
			}
			else if (m->value.IsDouble())
			{
				AddMemberT(pObj, MName, m->value.GetDouble());
			}
			else if (m->value.IsObject())
			{
				WValue MObj(rapidjson::kObjectType);
				auto Obj = m->value.GetObject();
				AddObjectPaser(&MObj, Obj.MemberBegin(), Obj.MemberEnd());
				AddMemberT(pObj, MName, MObj);
			}
		}
	}

	bool FindObject(const std::vector<std::wstring> &rParentNameList, CGMItr &rGMItr);

	template<typename T, typename TV>
	bool AddArrayT(T *pObj, const StringW &rName, TV Value)
	{
		if (nullptr == pObj) return false;

		if (pObj->HasMember(rName))
		{
			auto itr = pObj->FindMember(rName);
			itr->value.PushBack(Value, m_pDoc->GetAllocator());
			return true;
		}
		else
		{
			WValue varray(rapidjson::kArrayType);
			varray.PushBack(Value, m_pDoc->GetAllocator());
			
			pObj->AddMember(NewWVStr(rName.s), varray, m_pDoc->GetAllocator() );

			return true;
		}

		return false;
	}

	template<typename T>
	bool ModifyMemberT(WDocument &rDoc, const StringW &Name, T Value)
	{
		if (rDoc.IsObject())
		{
			if (rDoc.HasMember(Name))
			{
				auto itr = rDoc.FindMember(Name);
				itr->value = WValue(Value);
				return true;
			}
		}

		return false;
	}
	
	bool ModifyMemberT(WDocument &rDoc, const StringW &Name, WValue &Value)
	{
		if (rDoc.IsObject())
		{
			if (rDoc.HasMember(Name))
			{
				auto itr = rDoc.FindMember(Name);
				itr->value = Value;
				return true;
			}
		}

		return false;
	}

	template<typename T>
	bool ModifyObjectMemberT(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, T Value)
	{
		if (nullptr != m_pDoc)
		{
			CGMItr ItrValue;
			if (FindMember(*m_pDoc, rParentNameList, ItrValue))
			{
				if (ItrValue->value.IsObject())
				{
					WValue *pObj = const_cast<WValue *>(&ItrValue->value);
					if (pObj->HasMember(rName.c_str()))
					{
						auto itr = pObj->FindMember(rName.c_str());
						itr->value = WValue(Value);
						return true;
					}
				}
			}
		}

		return false;
	}

	bool ModifyObjectMemberT(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, WValue &Value)
	{
		if (nullptr != m_pDoc)
		{
			CGMItr ItrValue;
			if (FindMember(*m_pDoc, rParentNameList, ItrValue))
			{
				if (ItrValue->value.IsObject())
				{
					WValue *pObj = const_cast<WValue *>(&ItrValue->value);
					if (pObj->HasMember(rName.c_str()))
					{
						auto itr = pObj->FindMember(rName.c_str());
						itr->value = Value;
						return true;
					}
				}
			}
		}

		return false;
	}

	template<typename T, typename TV>
	bool AddMemberT(T *pObj, const wchar_t *pName, TV Value)
	{
		if (nullptr == pObj) return false;

		auto itr = pObj->FindMember(StringW(pName) );
		if (pObj->MemberEnd() == itr)
			pObj->AddMember(NewWVStr(pName), Value, m_pDoc->GetAllocator());
		else
		{
			itr->value = Value;
		}

		return true;		
	}

	template<typename T>
	bool AddMemberT(T *pObj, const wchar_t *pName, WValue &Value)
	{
		if (nullptr == pObj) return false;

		auto itr = pObj->FindMember(StringW(pName) );
		if (pObj->MemberEnd() == itr)
			pObj->AddMember(NewWVStr(pName), Value, m_pDoc->GetAllocator());
		else
		{
			itr->value = Value;	
		}
		return true;
	}

	
	bool AddObjectMemberT(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, WValue &rValue)
	{
		if (nullptr != m_pDoc)
		{
			CGMItr ItrValue;
			if (FindMember(*m_pDoc, rParentNameList, ItrValue))
			{
				if (ItrValue->value.IsObject())
				{
					WValue *pObj = const_cast<WValue *>(&ItrValue->value);
					pObj->AddMember(NewWVStr(rName.c_str()), rValue, m_pDoc->GetAllocator());
					return true;
				}
			}
		}

		return false;
	}

	std::string m_ErrStr;

public:	

	explicit CJsonCtrl();	

	virtual ~CJsonCtrl();

	bool OpenFile(const wchar_t *pFilePath, WDocument &rDoc);

	bool SetBuffer(const std::wstring &rBuf, WDocument &rDoc);

	bool SetBuffer(const StringBufferW &rBuf, WDocument &rDoc);

	bool SaveFile(const wchar_t *pFilePath, const WDocument &rDoc);

	bool SaveFileIncSpace(const wchar_t *pFilePath, const WDocument &rDoc);	

	bool GetBuffer(std::wstring &rBuf, const WDocument &rDoc);

	bool GetBufferIncSpace(std::wstring &rBuf, const WDocument &rDoc);

	bool FindMember(const WDocument &rDoc, const std::wstring &rStr, CGMItr &rValue);	

	bool FindMember(const WDocument &rDoc, const std::vector<std::wstring> &rStrList, CGMItr &rValue);

	bool FindMember(const std::wstring &rStr, CGMItr &rValue);

	bool FindMember(const std::vector<std::wstring> &rStrList, CGMItr &rValue);

	bool FindMember(CGMItr &rObject, const std::wstring &rStr, CGMItr &rValue);

	template<typename T>
	bool AddMember(const std::wstring &rName, T Value)
	{		
		return (nullptr != m_pDoc) ? (AddMemberT(m_pDoc, rName.c_str(), Value)) : false;
	}
	
	bool AddMember(const std::wstring &rName, WValue &Value)
	{		
		return (nullptr != m_pDoc) ? (AddMemberT(m_pDoc, rName.c_str(), Value)) : false;
	}
	
	bool AddMember(const std::wstring &rName, const wchar_t *pValue)
	{
		return AddMember(rName, NewWVStr(pValue) );
	}

	bool AddMember(const std::wstring &rName, const std::wstring &rValue)
	{
		return AddMember(rName, NewWVStr(rValue.c_str()));
	}

	template<typename T>
	bool AddArray(const std::wstring &rName, T Value)
	{
		return AddArrayT(m_pDoc, StringW(rName.c_str()), Value);
	}

	bool AddArray(const std::wstring &rName, const wchar_t *pValue)
	{
		return AddArrayT(m_pDoc, StringW(rName.c_str()), NewWVStr(pValue) );
	}
	
	bool AddArray(const std::wstring &rName, const std::wstring &Value)
	{
		return AddArrayT(m_pDoc, StringW(rName.c_str()), NewWVStr(Value.c_str()) );
	}

	template<typename T>
	bool AddArray(WValue *pParent, const std::wstring &rName, T Value)
	{
		return (nullptr != m_pDoc) ? AddArrayT(pParent, StringW(rName.c_str()), Value) : false;
	}

	bool AddArray(WValue *pParent, const std::wstring &rName, const wchar_t *pValue)
	{
		return (nullptr != m_pDoc) ? AddArrayT(pParent, StringW(rName.c_str()), NewWVStr(pValue) ) : false;
	}

	bool AddArray(WValue *pParent, const std::wstring &rName, const std::wstring &Value)
	{
		return (nullptr != m_pDoc) ? AddArrayT(pParent, StringW(rName.c_str()), NewWVStr(Value.c_str() ) ): false;
	}
	
	template<typename T>
	bool AddArrayMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, T Value)
	{
		if (nullptr != m_pDoc)
		{
			CGMItr ItrValue;
			if (FindMember(*m_pDoc, rParentNameList, ItrValue))
			{
				if (ItrValue->value.IsObject())
				{
					WValue *pObj = const_cast<WValue *>(&ItrValue->value);
					return AddArray(pObj, rName, Value);
				}
			}
		}

		return false;
	}
	
	bool AddArrayMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, const wchar_t *pValue)
	{
		return AddArrayMember(rParentNameList, rName, pValue);
	}

	bool AddArrayMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, const std::wstring &Value)
	{
		return AddArrayMember(rParentNameList, rName, Value.c_str());
	}
	
	template<typename T>
	bool AddObject(WValue *pParent, const std::wstring &rName, T Value)
	{
		return (nullptr != m_pDoc) ? (AddMemberT(pParent, rName.c_str(), Value) ) : false;
	}

	bool AddObject(WValue *pParent, const std::wstring &rName, const wchar_t *pValue)
	{		
		return (nullptr != m_pDoc) ? AddMemberT(pParent, rName.c_str(), NewWVStr(pValue) ): false;
	}
	
	bool AddObject(WValue *pParent, const std::wstring &rName, const std::wstring &rValue)
	{
		return (nullptr != m_pDoc) ? AddMemberT(pParent, rName.c_str(), NewWVStr(rValue.c_str() ) ) : false;
	}

	bool AddObject(WValue *Parent, const std::wstring &rName, WValue &Value);

	template<typename TB, typename TE>
	bool AddObject(const std::wstring &rName, TB MBegin, TE MEnd)
	{
		if (nullptr == m_pDoc) return false;
		
		EraseObjectMember({ rName });

		WValue Parent(rapidjson::kObjectType);
		AddObjectPaser(&Parent, MBegin, MEnd);
		return AddMember(rName.c_str(), Parent);
	}

	template<typename T>
	bool AddObjectMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, T Value)
	{
		if (nullptr != m_pDoc)
		{
			CGMItr ItrValue;
			if (FindMember(*m_pDoc, rParentNameList, ItrValue))
			{
				if (ItrValue->value.IsObject())
				{
					WValue *pObj = const_cast<WValue *>(&ItrValue->value);
					pObj->AddMember(NewWVStr(rName.c_str() ), Value, m_pDoc->GetAllocator());
					return true;
				}
			}
		}

		return false;
	}

	template<>
	bool AddObjectMember<const wchar_t *>(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, const wchar_t *pValue)
	{
		return AddObjectMemberT(rParentNameList, rName, NewWVStr(pValue) );
	}

	template<>
	bool AddObjectMember<const std::wstring &>(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, const std::wstring &rValue)
	{
		return AddObjectMemberT(rParentNameList, rName, NewWVStr(rValue.c_str() ) );
	}

	template<>
	bool AddObjectMember<WValue *>(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, WValue *pValue)
	{
		return AddObjectMemberT(rParentNameList, rName, *pValue);		
	}

	template<typename T>
	bool ModifyMember(const std::wstring &rName, T Value)
	{
		return (nullptr != m_pDoc) ? ModifyMemberT(*m_pDoc, StringW(rName.c_str() ), Value) : false;
	}

	bool ModifyMember(const std::wstring &rName, const wchar_t *pValue)
	{
		return ModifyMember(rName, NewWVStr(pValue));
	}

	bool ModifyMember(const std::wstring &rName, const std::wstring &rValue)
	{		
		return ModifyMember(rName, NewWVStr(rValue.c_str()));
	}

	template<typename T>
	bool ModifyObjectMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, T Value)
	{
		return ModifyObjectMemberT(rParentNameList, rName, Value);
	}	

	bool ModifyObjectMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, const wchar_t *Value)
	{
		return ModifyObjectMember(rParentNameList, rName, NewWVStr(Value));
	}

	bool ModifyObjectMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName, const std::wstring &Value)
	{
		return ModifyObjectMember(rParentNameList, rName, NewWVStr(Value.c_str() ) );
	}

	bool EraseObjectMember(const std::vector<std::wstring> &rParentNameList, const std::wstring &rName);

	bool EraseObjectMember(const std::vector<std::wstring> &rParentNameList);

	bool EraseMember(const std::wstring &rName);

	void Set(WDocument *pDoc);

	WDocument *Get();

	const std::string &GetLastErrorStr() const;

	bool ReadBool(const GenericValue<UTF16<>> &Value, bool &Out)//讀取數據-布林
	{
		//std::string type_name=typeid(Out).name();
		if (Value.IsBool() == true)
		{	Out = Value.GetBool(); }
		else if (Value.IsInt() == true)
		{	Out = static_cast<int>(Value.GetInt()); }		
		else
		{	return false;	}
		return true;
	}
	bool ReadInt(const GenericValue<UTF16<>> &Value, int &Out)//讀取數據-整數
	{
		//std::string type_name=typeid(Out).name();
		if (Value.IsInt() == true)
		{	Out = Value.GetInt(); }
		else if (Value.IsFloat() == true)
		{	Out = static_cast<int>(Value.GetFloat()); }
		else if (Value.IsDouble() == true)
		{	Out = static_cast<int>(Value.GetDouble()); }
		else
		{	return false;	}
		return true;
	}
	bool ReadFloat(const GenericValue<UTF16<>> &Value, float &Out)//讀取數據-浮點數
	{
		//std::string type_name=typeid(Out).name();
		if (Value.IsFloat() == true)
		{	Out = Value.GetFloat(); }
		else if (Value.IsDouble() == true)
		{	Out = static_cast<float>(Value.GetDouble()); }
		else if (Value.IsInt() == true)
		{	Out = static_cast<float>(Value.GetInt()); }
		else
		{	return false;	}
		return true;
	}
	bool ReadDouble(const GenericValue<UTF16<>> &Value, double &Out)//讀取數據-浮點數
	{
		//std::string type_name=typeid(Out).name();
		if (Value.IsDouble() == true)
		{	Out = Value.GetDouble(); }
		else if (Value.IsFloat() == true)
		{	Out = static_cast<double>(Value.GetFloat()); }		
		else if (Value.IsInt() == true)
		{	Out = static_cast<double>(Value.GetInt()); }
		else
		{	return false;	}
		return true;
	}
	bool ReadString(const GenericValue<UTF16<>> &Value, wchar_t Out[])//讀取數據-字串
	{
		//std::string type_name=typeid(Out).name();
		//std::string type_name2=typeid(wchar_t[]).name();
		if (Value.IsString() == false )
		{	return false; }
		::wcscpy(Out, Value.GetString());
		return true;
	}
	bool ReadString(const GenericValue<UTF16<>> &Value, std::wstring &Out)//讀取數據-字串
	{
		std::string type_name=typeid(Out).name();
		if (Value.IsString() == false )
		{	return false; }
		Out = Value.GetString();
		return true;
	}
	bool ReadArrayInt(const GenericValue<UTF16<>> &Value, std::vector<int> &Out)//讀取數據-整數陣列
	{
		if (Value.IsArray() == false )
		{	return false; }		
		int val=0;
		Out.clear();
		for (auto itr = Value.Begin(); itr!=Value.End(); ++itr)
		{			
			if ( ReadInt(*itr, val) == false )
			{	return false; }
			Out.push_back(val);	
		}
		return true;
	}
	bool ReadArrayBol(const GenericValue<UTF16<>> &Value, size_t Max, bool Out[])//讀取數據-布林陣列
	{
		if (Value.IsArray() == false )
		{	return false; }		
		bool val=false;
		size_t Count=0;
		for (auto itr = Value.Begin(); itr!=Value.End(); ++itr)
		{			
			if ( Count >= Max )
			{	return false; }
			if ( ReadBool(*itr, val) == false )
			{	return false; }
			Out[Count] = val;
			Count ++;			
		}
		return true;
	}
	bool ReadArrayInt(const GenericValue<UTF16<>> &Value, size_t Max, int Out[])//讀取數據-整數陣列
	{
		if (Value.IsArray() == false )
		{	return false; }		
		int val=0;
		size_t Count=0;
		for (auto itr = Value.Begin(); itr!=Value.End(); ++itr)
		{			
			if ( Count >= Max )
			{	return false; }
			if ( ReadInt(*itr, val) == false )
			{	return false; }
			Out[Count] = val;
			Count ++;			
		}
		return true;
	}
	bool ReadArrayFloat(const GenericValue<UTF16<>> &Value, std::vector<float> &Out)//讀取數據-浮點數陣列
	{
		if (Value.IsArray() == false )
		{	return false; }		
		float val=0;
		Out.clear();
		for (auto itr = Value.Begin(); itr!=Value.End(); ++itr)
		{			
			if ( ReadFloat(*itr, val) == false )
			{	return false; }
			Out.push_back(val);	
		}
		return true;
	}
	bool ReadArrayFloat(const GenericValue<UTF16<>> &Value, size_t Max, float Out[])//讀取數據-浮點數陣列
	{
		if (Value.IsArray() == false )
		{	return false; }		
		float val=0;
		size_t Count=0;
		for (auto itr = Value.Begin(); itr!=Value.End(); ++itr)
		{			
			if ( Count >= Max )
			{	return false; }
			if ( ReadFloat(*itr, val) == false )
			{	return false; }
			Out[Count] = val;
			Count ++;
		}
		return true;
	}	
	bool ReadArrayDouble(const GenericValue<UTF16<>> &Value, std::vector<double> &Out)//讀取數據-浮點數陣列
	{
		if (Value.IsArray() == false )
		{	return false; }		
		double val=0;
		Out.clear();
		for (auto itr = Value.Begin(); itr!=Value.End(); ++itr)
		{			
			if ( ReadDouble(*itr, val) == false )
			{	return false; }
			Out.push_back(val);	
		}
		return true;
	}
	bool ReadArrayDouble(const GenericValue<UTF16<>> &Value, size_t Max, double Out[])//讀取數據-浮點數陣列
	{
		if (Value.IsArray() == false )
		{	return false; }		
		double val=0;
		size_t Count=0;
		for (auto itr = Value.Begin(); itr!=Value.End(); ++itr)
		{			
			if ( Count >= Max )
			{	return false; }
			if ( ReadDouble(*itr, val) == false )
			{	return false; }
			Out[Count] = val;
			Count ++;
		}
		return true;
	}
	bool ReadArrayString(const GenericValue<UTF16<>> &Value, std::vector<std::wstring> &Out)//讀取數據-字串陣列
	{
		if (Value.IsArray() == false )
		{	return false; }				
		Out.clear();
		//wchar_t val[64]=L"";
		std::wstring val;
		for (auto itr = Value.Begin(); itr!=Value.End(); ++itr)
		{	
			if ( ReadString(*itr, val) == false )
			{	return false; }
			Out.push_back(val);	
		}
		return true;
	}

	bool CheckValItr(const GenericValue<UTF16<>> &Value, const CGMItr &Out) //確認是否成功
	{
		if ( Out == Value.MemberEnd() )
		{	return false;	}
		return true;
	}

	bool ReadValItr(const GenericValue<UTF16<>> &Value, const wchar_t *Key, CGMItr &Out)//讀取數據內-Iter
	{
		Out=Value.FindMember(Key);
		if ( CheckValItr(Value, Out) == false )
		{	return false; }
		return true;
	}

	bool ReadValBool(const GenericValue<UTF16<>> &Value, const wchar_t *Key, bool &Out)//讀取數據內-布林
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadBool(Itr->value, Out);
	}
	bool ReadValInt(const GenericValue<UTF16<>> &Value, const wchar_t *Key, int &Out)//讀取數據內-整數
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadInt(Itr->value, Out);
	}
	bool ReadValFloat(const GenericValue<UTF16<>> &Value, const wchar_t *Key, float &Out)//讀取數據內-浮點數
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadFloat(Itr->value, Out);
	}
	bool ReadValDouble(const GenericValue<UTF16<>> &Value, const wchar_t *Key, double &Out)//讀取數據內-浮點數
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadDouble(Itr->value, Out);
	}
	bool ReadValString(const GenericValue<UTF16<>> &Value, const wchar_t *Key, wchar_t Out[])//讀取數據內-字串
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadString(Itr->value, Out);
	}
	bool ReadValString(const GenericValue<UTF16<>> &Value, const wchar_t *Key, std::wstring &Out)//讀取數據內-字串
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadString(Itr->value, Out);
	}
	bool ReadValRect(const GenericValue<UTF16<>> &Value, const wchar_t *Key, RECT &Out)//讀取數據內-矩形
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		const int Max = 4;
		const int Default=INT_MAX;
		int nList[Max]={Default, Default, Default, Default};
		if ( ReadArrayInt(Itr->value, Max, nList) == false )
		{	return false; }
		for ( size_t i=0; i<Max; i++ )
		{
			if ( Default == nList[i] )
			{	return false; }
		}
		Out.left  = nList[0];
		Out.top   = nList[1];
		Out.right = nList[2];
		Out.bottom= nList[3];
		return true;
	}
	bool ReadValArrayInt(const GenericValue<UTF16<>> &Value, const wchar_t *Key, std::vector<int> &Out)//讀取數據內-整數陣列
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadArrayInt(Itr->value, Out);
	}
	bool ReadValArrayInt(const GenericValue<UTF16<>> &Value, const wchar_t *Key, size_t Max, int Out[])//讀取數據內-整數陣列
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadArrayInt(Itr->value, Max, Out);
	}
	bool ReadValArrayFloat(const GenericValue<UTF16<>> &Value, const wchar_t *Key, std::vector<float> &Out)//讀取數據內-浮點數陣列
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadArrayFloat(Itr->value, Out);
	}
	bool ReadValArrayFloat(const GenericValue<UTF16<>> &Value, const wchar_t *Key, size_t Max, float Out[])//讀取數據內-浮點數陣列
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadArrayFloat(Itr->value, Max, Out);
	}	

	bool ReadValArrayDouble(const GenericValue<UTF16<>> &Value, const wchar_t *Key, std::vector<double> &Out)//讀取數據內-浮點數陣列
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadArrayDouble(Itr->value, Out);
	}
	bool ReadValArrayDouble(const GenericValue<UTF16<>> &Value, const wchar_t *Key, size_t Max, double Out[])//讀取數據內-浮點數陣列
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadArrayDouble(Itr->value, Max, Out);
	}
	bool ReadValArrayString(const GenericValue<UTF16<>> &Value, const wchar_t *Key, std::vector<std::wstring> &Out)//讀取數據內-字串陣列
	{
		auto Itr=Value.FindMember(Key);
		if ( CheckValItr(Value, Itr) == false )
		{	return false; }
		return ReadArrayString(Itr->value, Out);
	}	
	
	bool ReadObjItr(const CGMItr &Obj, const wchar_t *Key, CGMItr &Out)//讀取文件內-Iter
	{	
		if ( ReadValItr(Obj->value, Key, Out) == false )
		{	return false; }
		return true;
	}

	bool ReadObjBool(const CGMItr &Obj, const wchar_t *Key, bool &Out)//讀取文件內布林
	{
		if ( ReadValBool(Obj->value, Key, Out) == false )
		{	return false; }
		return true;
	}
	bool ReadObjInt(const CGMItr &Obj, const wchar_t *Key, int &Out)//讀取文件內整數
	{
		if ( ReadValInt(Obj->value, Key, Out) == false )
		{	return false; }
		return true;
	}
	bool ReadObjFloat(const CGMItr &Obj, const wchar_t *Key, float &Out)//讀取文件內浮點數
	{
		if ( ReadValFloat(Obj->value, Key, Out) == false )
		{	return false; }
		return true;
	}
	bool ReadObjDouble(const CGMItr &Obj, const wchar_t *Key, double &Out)//讀取文件內浮點數
	{
		if ( ReadValDouble(Obj->value, Key, Out) == false )
		{	return false; }
		return true;
	}
	bool ReadObjString(const CGMItr &Obj, const wchar_t *Key, wchar_t Out[])//讀取文件內字串
	{
		if ( ReadValString(Obj->value, Key, Out) == false )
		{	return false; }
		return true;
	}
	bool ReadObjString(const CGMItr &Obj, const wchar_t *Key, std::wstring &Out)//讀取文件內字串
	{
		if ( ReadValString(Obj->value, Key, Out) == false )
		{	return false; }
		return true;
	}	
	
	bool CheckDocItr(const WDocument &Doc, const CGMItr &Out) //確認是否成功
	{
		if ( Out == Doc.MemberEnd() )
		{	return false; }		
		return true;
	}
	bool ReadDocItr(const WDocument &Doc, const wchar_t *Key, CGMItr &Out)//讀取文件內-Iter
	{
		Out=Doc.FindMember(Key);
		if ( CheckDocItr(Doc, Out) == false )		
		{	return false; }		
		return true;
	}

	bool ReadDocBool(const WDocument &Doc, const wchar_t *Key, bool &Out)//讀取文件內布林
	{
		auto Itr = Doc.FindMember(Key);
		if ( CheckDocItr(Doc, Itr) == false )
		{	return false;	}		
		return ReadBool(Itr->value, Out);
	}
	bool ReadDocInt(const WDocument &Doc, const wchar_t *Key, int &Out)//讀取文件內整數
	{
		auto Itr = Doc.FindMember(Key);
		if ( CheckDocItr(Doc, Itr) == false )
		{	return false;	}
		return ReadInt(Itr->value, Out);
	}
	bool ReadDocFloat(const WDocument &Doc, const wchar_t *Key, float &Out)//讀取文件內浮點數
	{
		auto Itr = Doc.FindMember(Key);
		if ( CheckDocItr(Doc, Itr) == false )
		{	return false;	}
		return ReadFloat(Itr->value, Out);
	}
	bool ReadDocDouble(const WDocument &Doc, const wchar_t *Key, double &Out)//讀取文件內浮點數
	{
		auto Itr = Doc.FindMember(Key);
		if ( CheckDocItr(Doc, Itr) == false )
		{	return false;	}
		return ReadDouble(Itr->value, Out);
	}
	bool ReadDocString(const WDocument &Doc, const wchar_t *Key, wchar_t Out[])//讀取文件內字串
	{
		auto Itr = Doc.FindMember(Key);	
		if ( CheckDocItr(Doc, Itr) == false )
		{	return false;	}		
		return ReadString(Itr->value, Out);
	}
	bool ReadDocString(const WDocument &Doc, const wchar_t *Key, std::wstring &Out)//讀取文件內字串
	{
		auto Itr = Doc.FindMember(Key);	
		if ( CheckDocItr(Doc, Itr) == false )
		{	return false;	}
		return ReadString(Itr->value, Out);		
	}	
};

}


#endif