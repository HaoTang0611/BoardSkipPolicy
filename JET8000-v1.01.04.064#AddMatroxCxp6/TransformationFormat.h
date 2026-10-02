#ifndef _TRANSFORMATION_FROMAT_FILE_H_
#define _TRANSFORMATION_FROMAT_FILE_H_

#include "JsonCtrl.h"
#include <limits>

namespace rapidjson
{
	class CTransformationFormat
	{
		WDocument m_Doc;
		CJsonCtrl m_JSonCtrl;

		const std::wstring m_DataInfoName = L"Data_Info";

		void init()
		{
			// define the document as an object rather than an array
			m_Doc.SetObject();

			m_JSonCtrl.Set(&m_Doc);
		}

	public:
		explicit CTransformationFormat()
		{
			init();
		}

		CTransformationFormat(const std::wstring &rContent)
		{
			m_JSonCtrl.SetBuffer(rContent, m_Doc);
			m_JSonCtrl.Set(&m_Doc);
		}

		~CTransformationFormat()
		{}		

		template<typename T>
		void AddMainStruct(const std::wstring &rName, T Value)
		{	
			if (false == m_JSonCtrl.ModifyMember(rName, Value))
			{
				m_JSonCtrl.AddMember(rName, Value);
			}
		}		

		template<typename T>
		void ModifyMainStruct(const std::wstring &rName, T Value)
		{
			AddMainStruct(rName, Value);			
		}

		void CreateDataInfoObject()
		{
			WValue object(rapidjson::kObjectType);
			m_JSonCtrl.AddMember(m_DataInfoName, object);
		}

		template<typename T>
		void AddDataInfoStruct(const std::wstring &rName, T Value)
		{
			m_JSonCtrl.AddObjectMember({ m_DataInfoName }, rName, Value);
		}

		template<typename T>
		void ModifyDataInfoStruct(const std::wstring &rName, T Value)
		{
			m_JSonCtrl.ModifyObjectMember({ m_DataInfoName }, rName, Value);
		}

		void ModifyDataInfoStruct(const std::wstring &rName, const wchar_t *Value)
		{
			m_JSonCtrl.ModifyObjectMember({ m_DataInfoName }, rName, Value);
		}

		void EraseDataInfoStruct(const std::wstring&rName)
		{
			m_JSonCtrl.EraseObjectMember({ m_DataInfoName }, rName);
		}

		void EraseDataInfoStruct(const std::vector<std::wstring> &rNameList)
		{
			for (const auto &itr : rNameList)
				m_JSonCtrl.EraseObjectMember({ m_DataInfoName }, itr);
		}

		void EraseDataInfoStruct()
		{
			m_JSonCtrl.EraseObjectMember({ m_DataInfoName });
		}

		bool Paser(const std::wstring &rContent)
		{
			if (m_JSonCtrl.SetBuffer(rContent, m_Doc))
			{
				if (m_Doc.IsObject() )
				{
					m_JSonCtrl.Set(&m_Doc);
					return true;
				}
			}
			return false;
		}

		bool Paser(const std::wstring &rContent, WDocument &rDoc)
		{
			if (m_JSonCtrl.SetBuffer(rContent, rDoc))
			{
				return true;
			}
			return false;
		}

		void SetDocObj(WDocument *pDoc)
		{
			m_JSonCtrl.Set(pDoc);
		}

		CJsonCtrl *GetJsonCtrl()
		{
			return &m_JSonCtrl;
		}

		int GetMainStructVInt(const std::wstring &rName)
		{
			CGMItr gmitr;
			if (m_JSonCtrl.FindMember(rName, gmitr))
			{
				if (gmitr->value.IsInt())
					return gmitr->value.GetInt();
			}

			#undef min
			return std::numeric_limits<int>::min();
		}		

		const std::wstring GetMainStructVStr(const std::wstring &rName)
		{
			CGMItr gmitr;
			if (m_JSonCtrl.FindMember(rName, gmitr))
			{
				if (gmitr->value.IsString())
				{
					return gmitr->value.GetString();
				}
			}
			return L"";
		}		

		bool GetDataInfoStructObj(CGMItr &gmitr)
		{
			return (m_JSonCtrl.FindMember(m_DataInfoName.c_str(), gmitr)) ? true : false;
		}

		void GetJsonString(std::wstring &rContent, const WDocument *pDoc = nullptr)
		{
			if (pDoc)
				m_JSonCtrl.GetBuffer(rContent, *pDoc);
			else
				m_JSonCtrl.GetBuffer(rContent, m_Doc);
		}
	};
}

#endif
