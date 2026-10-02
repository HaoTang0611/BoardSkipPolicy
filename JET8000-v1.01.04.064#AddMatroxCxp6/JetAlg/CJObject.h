//Author : Joe
#pragma once
#include "JETAlg_Std.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

#ifndef _JET_TKT_JOBJECT
#define _JET_TKT_JOBJECT
#endif // !_JET_TKT_JOBJECT

#ifndef JString
#define JString(arg) #arg
#endif // JString

#ifndef JWString
#define JWString(arg) L#arg
#endif // JString

namespace JET {
	namespace tkt {

		enum class EJsonType {
			kNullType = 0,      //!< null
			kFalseType = 1,     //!< false
			kTrueType = 2,      //!< true
			kObjectType = 3,    //!< object
			kArrayType = 4,     //!< array 
			kStringType = 5,    //!< string
			kNumberType = 6     //!< number
		};

		struct StException_JObject {
			const char* FileOpenFail = "Open json file fail.";
			const char* MemberNotExist = "Can not find json member.";
		};
	
		template<typename T_jobj, typename T_type> using Loader = bool (*)(T_jobj&, T_type&);
		template<typename T_jobj, typename T_type> using Writer = T_jobj* (*)(const T_type&);
		class JETALG_API JValue
		{
		private:
			bool m_isDispose;
			std::string m_Key;
			EJsonType m_Type;
			std::map<std::string, JValue*> m_Items;
			std::string m_Parent;
			bool m_isNeedDispose;
			void* m_Doc;
			void* m_Value;

		public:
			const char* GetVersion();
			explicit JValue();
			explicit JValue(const JValue& obj) = default;
			explicit JValue(void* memdoc, void* memval);
			~JValue();
			JValue& operator=(const JValue& obj) { return *this; }
			/// <summary>
			/// 獲取Json成員(Member)，可利用GetValue/SetValue 獲取、修改數值
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			JValue& operator [](const std::string& member);
			/// <summary>
			/// 獲取Json成員(Member)，可利用GetValue/SetValue 獲取、修改數值
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			JValue& FindMember(const char* member);
			bool Exist(const char* member) { return m_Items.find(member) != m_Items.end(); }
			/// <summary>
			/// 判斷此Json成員數值是否為null
			/// </summary>
			/// <returns></returns>
			bool IsNull() const;
			/// <summary>
			/// 將此Json成員數值設定為null
			/// </summary>
			/// <returns></returns>
			bool SetNull();

			template<typename T> bool SetValue(const T& val) {
				return setVal(val);
			}

#ifdef _DLL
			template<typename T> bool GetValue(T& val) const {
				return getVal(val);
			}
#else
			template<typename T> bool GetValue(T& val) const {
				std::shared_ptr<T> spVal;
				auto isok = getVal(spVal);
				if (isok) { val = *spVal; }
				return isok;
			}
#endif // _MD
			/// <summary>
			/// 利用轉換函示將Json匯入Class物件中
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="data"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool GetValue(T& data, Loader<JValue, T> parser) {
				return parser(*this, data);
			}
			template<typename T> bool GetValue(T& data, Loader<JValue, T> parser) const {
				return parser(*this, data);
			}
			/// <summary>
			/// 利用轉換函示將Json匯入Class物件中
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="data"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool GetValue(std::vector<T>& data, Loader<JValue, T> parser) const {
				size_t idx = 0;
				bool isok = true;
				for (auto obj_i = this->m_Items.begin(); obj_i != this->m_Items.end(); ++obj_i) {
					data.push_back(T());
					isok &= parser(*obj_i->second, data[idx++]);
				}
				return isok;
			}
			/// <summary>
			/// 利用轉換函示將Json匯入Class物件中
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="data"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool GetValue(std::vector<std::vector<T>>& data, Loader<JValue, T> parser) const {
				size_t idx = 0;
				bool isok = true;
				for (auto obj_i = this->m_Items.begin(); obj_i != this->m_Items.end(); ++obj_i, idx++) {
					size_t idy = 0;
					data.push_back(std::vector<T>());
					for (auto obj_j = obj_i->second->m_Items.begin(); obj_j != obj_i->second->m_Items.end(); ++obj_j) {
						data[idx].push_back(T());
						isok &= parser(*obj_i->second, data[idx][idy++]);
					}
				}
				return isok;
			}

			bool _isDispose() const;
			std::string _GetKey() const;
			void* _GetMemDoc();
			void* _GetMemValue();

			void _SetParent(std::string obj);
			void _SetKey(std::string key);
			void _SetType(EJsonType type);
			void _AddItem(std::string key, JValue* jval);
		private:
			//-----------------------------------Get-----------------------------------
			bool getVal(int& val) const;
			bool getVal(unsigned int& val) const;
			bool getVal(long long& val) const;
			bool getVal(unsigned long long& val) const;
			bool getVal(float& val) const;
			bool getVal(double& val) const;
			bool getVal(bool& val) const;
			bool getVal(const char*& val) const;
			bool getVal(std::string& val) const;

			bool getVal(std::shared_ptr<int>& val) const;
			bool getVal(std::shared_ptr<unsigned int>& val) const;
			bool getVal(std::shared_ptr<long long>& val) const;
			bool getVal(std::shared_ptr<unsigned long long>& val) const;
			bool getVal(std::shared_ptr<float>& val) const;
			bool getVal(std::shared_ptr<double>& val) const;
			bool getVal(std::shared_ptr<bool>& val) const;
			bool getVal(std::shared_ptr<const char*>& val) const;
			bool getVal(std::shared_ptr<std::string>& val) const;

			bool getVal(std::vector<int>& val) const;
			bool getVal(std::vector<unsigned int>& val) const;
			bool getVal(std::vector<long long>& val) const;
			bool getVal(std::vector<unsigned long long>& val) const;
			bool getVal(std::vector<float>& val) const;
			bool getVal(std::vector<double>& val) const;
			bool getVal(std::vector<bool>& val) const;
			bool getVal(std::vector<const char*>& val) const;
			bool getVal(std::vector<std::string>& val) const;

			bool getVal(std::shared_ptr<std::vector<int>>& val) const;
			bool getVal(std::shared_ptr<std::vector<unsigned int>>& val) const;
			bool getVal(std::shared_ptr<std::vector<long long>>& val) const;
			bool getVal(std::shared_ptr<std::vector<unsigned long long>>& val) const;
			bool getVal(std::shared_ptr<std::vector<float>>& val) const;
			bool getVal(std::shared_ptr<std::vector<double>>& val) const;
			bool getVal(std::shared_ptr<std::vector<bool>>& val) const;
			bool getVal(std::shared_ptr<std::vector<const char*>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::string>>& val) const;

			bool getVal(std::vector<std::vector<int>>& val) const;
			bool getVal(std::vector<std::vector<unsigned int>>& val) const;
			bool getVal(std::vector<std::vector<long long>>& val) const;
			bool getVal(std::vector<std::vector<unsigned long long>>& val) const;
			bool getVal(std::vector<std::vector<float>>& val) const;
			bool getVal(std::vector<std::vector<double>>& val) const;
			bool getVal(std::vector<std::vector<bool>>& val) const;
			bool getVal(std::vector<std::vector<const char*>>& val) const;
			bool getVal(std::vector<std::vector<std::string>>& val) const;

			bool getVal(std::shared_ptr<std::vector<std::vector<int>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<unsigned int>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<long long>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<unsigned long long>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<float>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<double>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<bool>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<const char*>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<std::string>>>& val) const;

			//-----------------------------------Set-----------------------------------
			bool setVal(const int& val);
			bool setVal(const unsigned int& val);
			bool setVal(const long long& val);
			bool setVal(const unsigned long long& val);
			bool setVal(const float& val);
			bool setVal(const double& val);
			bool setVal(const bool& val);
			bool setVal(const const char* val);
			bool setVal(const std::string& val);

			bool setVal(const std::vector<int>& val, bool isClear = true);
			bool setVal(const std::vector<unsigned int>& val, bool isClear = true);
			bool setVal(const std::vector<long long>& val, bool isClear = true);
			bool setVal(const std::vector<unsigned long long>& val, bool isClear = true);
			bool setVal(const std::vector<float>& val, bool isClear = true);
			bool setVal(const std::vector<double>& val, bool isClear = true);
			bool setVal(const std::vector<bool>& val, bool isClear = true);
			bool setVal(const std::vector<const char*>& val, bool isClear = true);
			bool setVal(const std::vector<std::string>& val, bool isClear = true);

			bool setVal(const std::vector<std::vector<int>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<unsigned int>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<long long>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<unsigned long long>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<float>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<double>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<bool>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<const char*>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<std::string>>& val, bool isClear = true);
		};

		class JETALG_API JObject
		{
		private:
			bool m_isDispose = false;
			std::map<std::string, JValue*> m_Obj;
			void* m_Doc;
		public:
			std::string m_ErrorStr;

		public:
			const char* GetVersion();
			explicit JObject();
			explicit JObject(const char* jsonStr);
			~JObject();
			/// <summary>
			/// 獲取Json成員(Member)，可利用GetValue/SetValue 獲取、修改數值
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			JValue& operator [](const std::string& member);
			/// <summary>
			/// 獲取Json成員(Member)，可利用GetValue/SetValue 獲取、修改數值
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			JValue& FindMember(const char* member);
			bool Exist(const char* member) { return m_Obj.find(member) != m_Obj.end(); }
			bool LoadFile(const char* filename);
			bool SaveFile(const char* filename, bool pretty = false);
			/// <summary>
			/// 將目前Json內容轉成字串(ASCII編碼)
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<std::string> ToJsonStr(bool pretty = false);
			/// <summary>
			/// 須自行釋放記憶體
			/// </summary>
			/// <returns></returns>
			JValue* ToValue();
			/// <summary>
			/// 新增Json成員(member)並預設初始值null
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			bool AddMember(const char* member);

			bool AddMember(const char* member, const int& val);
			bool AddMember(const char* member, const unsigned int& val);
			bool AddMember(const char* member, const long long& val);
			bool AddMember(const char* member, const unsigned long long& val);
			bool AddMember(const char* member, const float& val);
			bool AddMember(const char* member, const double& val);
			bool AddMember(const char* member, const bool& val);
			bool AddMember(const char* member, const char* val);
			bool AddMember(const char* member, const std::string& val);
			bool AddMember(const char* member, const JObject& val);

			bool AddMember(const char* member, const std::vector<int>& val);
			bool AddMember(const char* member, const std::vector<unsigned int>& val);
			bool AddMember(const char* member, const std::vector<long long>& val);
			bool AddMember(const char* member, const std::vector<unsigned long long>& val);
			bool AddMember(const char* member, const std::vector<float>& val);
			bool AddMember(const char* member, const std::vector<double>& val);
			bool AddMember(const char* member, const std::vector<bool>& val);
			bool AddMember(const char* member, const std::vector<const char*>& val);
			bool AddMember(const char* member, const std::vector<std::string>& val);
			bool AddMember(const char* member, const std::vector<JObject*>& val);

			bool AddMember(const char* member, const std::vector<std::vector<int>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<unsigned int>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<long long>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<unsigned long long>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<float>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<double>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<bool>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<const char*>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<std::string>>& val);
			bool AddMember(const char* member, const std::vector<std::vector<JObject*>>& val);

			void _AddObject(std::string member, JValue* jval);

			/// <summary>
			/// 利用轉換函示將Class轉成JObject
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="member"></param>
			/// <param name="val"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool AddMember(const char* member, const T& val, Writer<JObject, T> parser) {
				auto vobj = parser(val);
				bool isok = AddMember(member, *vobj);
				delete vobj; vobj = nullptr;
				return isok;
			}
			/// <summary>
			/// 利用轉換函示將Class轉成JObject
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="member"></param>
			/// <param name="val"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool AddMemberVec(const char* member, const std::vector<T>& val, Writer<JObject, T> parser) {
				std::vector<JObject*> out;
				for (size_t i = 0; i < val.size(); i++) {
					auto vobj = parser(val[i]);
					out.push_back(vobj);
				}
				bool isok = this->AddMember(member, out);
				for (size_t i = 0; i < out.size(); i++) {
					delete out[i];
				}
				out.clear();
				return isok;
			}

		private:
			bool _LoadFile(const char* filename, std::string& content);
			bool _CreateFile(const char* filename, const std::string& content);
			bool _SetJsonStrToDoc(const char* jsonStr);
			void _UnPack();
		};


		class JETALG_API JWValue
		{
		private:
			bool m_isDispose;
			std::wstring m_Key;
			EJsonType m_Type;
			std::map<std::wstring, JWValue*> m_Items;
			std::wstring m_Parent;
			bool m_isNeedDispose;
			void* m_Doc;
			void* m_Value;

		public:
			const char* GetVersion();
			explicit JWValue();
			explicit JWValue(const JWValue& obj) = default;
			explicit JWValue(void* memdoc, void* memval);
			~JWValue();
			JWValue& operator=(const JWValue& obj) { return *this; }
			/// <summary>
			/// 獲取Json成員(Member)，可利用GetValue/SetValue 獲取、修改數值
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			JWValue& operator [](const std::wstring& member);
			/// <summary>
			/// 獲取Json成員(Member)，可利用GetValue/SetValue 獲取、修改數值
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			JWValue& FindMember(const wchar_t* member);
			bool Exist(const wchar_t* member) { return m_Items.find(member) != m_Items.end(); }
			/// <summary>
			/// 判斷此Json成員數值是否為null
			/// </summary>
			/// <returns></returns>
			bool IsNull() const;
			/// <summary>
			/// 將此Json成員數值設定為null
			/// </summary>
			/// <returns></returns>
			bool SetNull();

			template<typename T> bool SetValue(const T& val) {
				return setVal(val);
			}

			#ifdef _DLL
			template<typename T> bool GetValue(T& val) const {
				return getVal(val);
			}
			#else
			template<typename T> bool GetValue(T& val) const {
				std::shared_ptr<T> spVal;
				auto isok = getVal(spVal);
				if (isok) { val = *spVal; }
				return isok;
			}
			#endif // _MD
			/// <summary>
			/// 利用轉換函示將Json匯入Class物件中
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="data"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool GetValue(T& data, Loader<JWValue, T> parser) {
				return parser(*this, data);
			}
			template<typename T> bool GetValue(T& data, Loader<JWValue, T> parser) const {
				return parser(*this, data);
			}
			/// <summary>
			/// 利用轉換函示將Json匯入Class物件中
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="data"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool GetValue(std::vector<T>& data, Loader<JWValue, T> parser) const {
				size_t idx = 0;
				bool isok = true;
				for (auto obj_i = this->m_Items.begin(); obj_i != this->m_Items.end(); ++obj_i) {
					data.push_back(T());
					isok &= parser(*obj_i->second, data[idx++]);
				}
				return isok;
			}
			/// <summary>
			/// 利用轉換函示將Json匯入Class物件中
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="data"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool GetValue(std::vector<std::vector<T>>& data, Loader<JWValue, T> parser) const {
				size_t idx = 0;
				bool isok = true;
				for (auto obj_i = this->m_Items.begin(); obj_i != this->m_Items.end(); ++obj_i, idx++) {
					size_t idy = 0;
					data.push_back(std::vector<T>());
					for (auto obj_j = obj_i->second->m_Items.begin(); obj_j != obj_i->second->m_Items.end(); ++obj_j) {
						data[idx].push_back(T());
						isok &= parser(*obj_i->second, data[idx][idy++]);
					}
				}
				return isok;
			}

			bool _isDispose() const;
			std::wstring _GetKey() const;
			void* _GetMemDoc();
			void* _GetMemValue();

			void _SetParent(std::wstring obj);
			void _SetKey(std::wstring key);
			void _SetType(EJsonType type);
			void _AddItem(std::wstring key, JWValue* jval);
		private:
			//-----------------------------------Get-----------------------------------
			bool getVal(int& val) const;
			bool getVal(unsigned int& val) const;
			bool getVal(long long& val) const;
			bool getVal(unsigned long long& val) const;
			bool getVal(float& val) const;
			bool getVal(double& val) const;
			bool getVal(bool& val) const;
			bool getVal(const wchar_t*& val) const;
			bool getVal(std::wstring& val) const;

			bool getVal(std::shared_ptr<int>& val) const;
			bool getVal(std::shared_ptr<unsigned int>& val) const;
			bool getVal(std::shared_ptr<long long>& val) const;
			bool getVal(std::shared_ptr<unsigned long long>& val) const;
			bool getVal(std::shared_ptr<float>& val) const;
			bool getVal(std::shared_ptr<double>& val) const;
			bool getVal(std::shared_ptr<bool>& val) const;
			bool getVal(std::shared_ptr<const wchar_t*>& val) const;
			bool getVal(std::shared_ptr<std::wstring>& val) const;

			bool getVal(std::vector<int>& val) const;
			bool getVal(std::vector<unsigned int>& val) const;
			bool getVal(std::vector<long long>& val) const;
			bool getVal(std::vector<unsigned long long>& val) const;
			bool getVal(std::vector<float>& val) const;
			bool getVal(std::vector<double>& val) const;
			bool getVal(std::vector<bool>& val) const;
			bool getVal(std::vector<const wchar_t*>& val) const;
			bool getVal(std::vector<std::wstring>& val) const;

			bool getVal(std::shared_ptr<std::vector<int>>& val) const;
			bool getVal(std::shared_ptr<std::vector<unsigned int>>& val) const;
			bool getVal(std::shared_ptr<std::vector<long long>>& val) const;
			bool getVal(std::shared_ptr<std::vector<unsigned long long>>& val) const;
			bool getVal(std::shared_ptr<std::vector<float>>& val) const;
			bool getVal(std::shared_ptr<std::vector<double>>& val) const;
			bool getVal(std::shared_ptr<std::vector<bool>>& val) const;
			bool getVal(std::shared_ptr<std::vector<const wchar_t*>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::wstring>>& val) const;

			bool getVal(std::vector<std::vector<int>>& val) const;
			bool getVal(std::vector<std::vector<unsigned int>>& val) const;
			bool getVal(std::vector<std::vector<long long>>& val) const;
			bool getVal(std::vector<std::vector<unsigned long long>>& val) const;
			bool getVal(std::vector<std::vector<float>>& val) const;
			bool getVal(std::vector<std::vector<double>>& val) const;
			bool getVal(std::vector<std::vector<bool>>& val) const;
			bool getVal(std::vector<std::vector<const wchar_t*>>& val) const;
			bool getVal(std::vector<std::vector<std::wstring>>& val) const;

			bool getVal(std::shared_ptr<std::vector<std::vector<int>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<unsigned int>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<long long>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<unsigned long long>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<float>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<double>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<bool>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<const wchar_t*>>>& val) const;
			bool getVal(std::shared_ptr<std::vector<std::vector<std::wstring>>>& val) const;

			//-----------------------------------Set-----------------------------------
			bool setVal(const int& val);
			bool setVal(const unsigned int& val);
			bool setVal(const long long& val);
			bool setVal(const unsigned long long& val);
			bool setVal(const float& val);
			bool setVal(const double& val);
			bool setVal(const bool& val);
			bool setVal(const const wchar_t* val);
			bool setVal(const std::wstring& val);

			bool setVal(const std::vector<int>& val, bool isClear = true);
			bool setVal(const std::vector<unsigned int>& val, bool isClear = true);
			bool setVal(const std::vector<long long>& val, bool isClear = true);
			bool setVal(const std::vector<unsigned long long>& val, bool isClear = true);
			bool setVal(const std::vector<float>& val, bool isClear = true);
			bool setVal(const std::vector<double>& val, bool isClear = true);
			bool setVal(const std::vector<bool>& val, bool isClear = true);
			bool setVal(const std::vector<const wchar_t*>& val, bool isClear = true);
			bool setVal(const std::vector<std::wstring>& val, bool isClear = true);

			bool setVal(const std::vector<std::vector<int>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<unsigned int>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<long long>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<unsigned long long>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<float>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<double>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<bool>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<const wchar_t*>>& val, bool isClear = true);
			bool setVal(const std::vector<std::vector<std::wstring>>& val, bool isClear = true);
		};

		class JETALG_API JWObject
		{
		private:
			bool m_isDispose = false;
			std::map<std::wstring, JWValue*> m_Obj;
			void* m_Doc;
		public:
			std::string m_ErrorStr;

		public:
			const char* GetVersion();
			explicit JWObject();
			explicit JWObject(const wchar_t* jsonStr);
			~JWObject();
			/// <summary>
			/// 獲取Json成員(Member)，可利用GetValue/SetValue 獲取、修改數值
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			JWValue& operator [](const std::wstring& member);
			/// <summary>
			/// 獲取Json成員(Member)，可利用GetValue/SetValue 獲取、修改數值
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			JWValue& FindMember(const wchar_t* member);
			bool Exist(const wchar_t* member) { return m_Obj.find(member) != m_Obj.end(); }
			bool LoadFile(const wchar_t* filename);
			bool SaveFile(const wchar_t* filename, bool pretty = false, bool wBom = false);
			/// <summary>
			/// 將目前Json內容轉成字串
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<std::wstring> ToJsonStr(bool pretty = false);
			/// <summary>
			/// 須自行釋放記憶體
			/// </summary>
			/// <returns></returns>
			JWValue* ToValue();
			/// <summary>
			/// 新增Json成員(member)並預設初始值null
			/// </summary>
			/// <param name="member"></param>
			/// <returns></returns>
			bool AddMember(const wchar_t* member);

			bool AddMember(const wchar_t* member, const int& val);
			bool AddMember(const wchar_t* member, const unsigned int& val);
			bool AddMember(const wchar_t* member, const long long& val);
			bool AddMember(const wchar_t* member, const unsigned long long& val);
			bool AddMember(const wchar_t* member, const float& val);
			bool AddMember(const wchar_t* member, const double& val);
			bool AddMember(const wchar_t* member, const bool& val);
			bool AddMember(const wchar_t* member, const wchar_t* val);
			bool AddMember(const wchar_t* member, const std::wstring& val);
			bool AddMember(const wchar_t* member, const JWObject& val);

			bool AddMember(const wchar_t* member, const std::vector<int>& val);
			bool AddMember(const wchar_t* member, const std::vector<unsigned int>& val);
			bool AddMember(const wchar_t* member, const std::vector<long long>& val);
			bool AddMember(const wchar_t* member, const std::vector<unsigned long long>& val);
			bool AddMember(const wchar_t* member, const std::vector<float>& val);
			bool AddMember(const wchar_t* member, const std::vector<double>& val);
			bool AddMember(const wchar_t* member, const std::vector<bool>& val);
			bool AddMember(const wchar_t* member, const std::vector<const wchar_t*>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::wstring>& val);
			bool AddMember(const wchar_t* member, const std::vector<JWObject*>& val);

			bool AddMember(const wchar_t* member, const std::vector<std::vector<int>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<unsigned int>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<long long>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<unsigned long long>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<float>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<double>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<bool>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<const wchar_t*>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<std::wstring>>& val);
			bool AddMember(const wchar_t* member, const std::vector<std::vector<JWObject*>>& val);

			void _AddObject(std::wstring member, JWValue* jval);

			/// <summary>
			/// 利用轉換函示將Class轉成JObject
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="member"></param>
			/// <param name="val"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool AddMember(const wchar_t* member, const T& val, Writer<JWObject, T> parser) {
				auto vobj = parser(val);
				bool isok = AddMember(member, *vobj);
				delete vobj; vobj = nullptr;
				return isok;
			}
			/// <summary>
			/// 利用轉換函示將Class轉成JObject
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="member"></param>
			/// <param name="val"></param>
			/// <param name="parser"></param>
			/// <returns></returns>
			template<typename T> bool AddMemberVec(const wchar_t* member, const std::vector<T>& val, Writer<JWObject, T> parser) {
				std::vector<JWObject*> out;
				for (size_t i = 0; i < val.size(); i++) {
					auto vobj = parser(val[i]);
					out.push_back(vobj);
				}
				bool isok = this->AddMember(member, out);
				for (size_t i = 0; i < out.size(); i++) {
					delete out[i];
				}
				out.clear();
				return isok;
			}

		private:
			bool _LoadFile(const wchar_t* filename, std::wstring& content);
			bool _CreateFile(const wchar_t* filename, const std::wstring& content, bool wBOM);
			bool _SetJsonStrToDoc(const wchar_t* jsonStr);
			void _UnPack();
		};
	
		#define _Combine_(v) v
		#define Def_JLoader(typeClass, typeJson) static bool LoadJObject(typeJson& jobj, typeClass& data)
		#define Def_JWriter(typeClass, typeJson) static typeJson* _Combine_(To)typeJson(const typeClass& data)
		#define Impl_JLoader(typeClass, typeJson) bool _Combine_(typeClass)::_Combine_(Load)JObject(typeJson& jobj, typeClass& data)
		#define Impl_JWriter(typeClass, typeJson) typeJson* _Combine_(typeClass)::_Combine_(To)typeJson(const typeClass& data)
	}
}