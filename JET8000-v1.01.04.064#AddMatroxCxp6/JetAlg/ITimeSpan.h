#pragma once
#include "JETAlg_Std.h"
#include <string>
#include <sstream>
#include <vector>
#include <chrono>

namespace JET {
	namespace tkt {
		__interface JETALG_API ITimeSpan
		{
			//獲取類別版本
			const char* GetVersion();
			//獲取CrossFunc狀態
			bool GetState_CrossFunc();
			//獲取Skip狀態
			bool GetState_Skip();
			//開啟-省略跨函式細項統計模式
			void SkipOn();
			//關閉-省略跨函式細項統計模式
			void SkipOff();

			//TimeSpan函式進入點
			void EntryFunc(const wchar_t* func);
			//TimeSpan函式進入點
			void EntryFunc(const char* func);
			//時間記錄點
			void TimePoint(const wchar_t* description);
			//時間記錄點
			void TimePoint(const char* description);
			//截止時間紀錄點
			void EndTimePoint();
			//清除資訊
			void Clear();
			//顯示時間記錄報告
			void Show_TimeSpanReport(const wchar_t* caption);
			//顯示時間記錄報告
			void Show_TimeSpanReport(const char* caption);
			//儲存時間記錄報告
			void Save_TimeSpanReport(const wchar_t* filename);
			//儲存時間記錄報告
			void Save_TimeSpanReport(const char* filename);
		};

		class JETALG_API ATimeSpan : public ITimeSpan
		{
		protected:
			bool m_CrossFunc = false;
			bool m_Skip = false;
			std::vector<std::wstring> m_Funcnames;
			std::vector<std::chrono::high_resolution_clock::time_point> m_CheckPoints;
			std::chrono::high_resolution_clock::time_point m_EndPoint;
			std::vector<double> m_TimeSpans;
			std::vector<std::wstring> m_Descriptions;

			template<typename T> std::wstring val2wstring(T val) { return std::to_wstring(val); }
			const wchar_t* string2wstring(const char* text) {
				auto length = strlen(text) + 1;
				wchar_t* wtext = new wchar_t[length];
				size_t olength;
				mbstowcs_s(&olength, wtext, length, text, length - 1);
				return wtext;
			}

		public:
			virtual ~ATimeSpan() {
				this->m_Funcnames.clear();
				this->m_CheckPoints.clear();
				this->m_TimeSpans.clear();
				this->m_Descriptions.clear();
			}

			ATimeSpan& operator=(const ATimeSpan& obj) { return *this; }

			bool GetState_CrossFunc() { return m_CrossFunc; }
			bool GetState_Skip() { return m_Skip; }
			void SkipOn() { m_Skip = true; }
			void SkipOff() { m_Skip = false; }

			virtual const char* GetVersion() = 0;
			virtual void EntryFunc(const wchar_t* func) = 0;
			virtual void EntryFunc(const char* func) {
				auto wstr = string2wstring(func);
				EntryFunc(wstr);
				delete wstr;
			}
			virtual void TimePoint(const wchar_t* description) = 0;
			virtual void TimePoint(const char* description) {
				auto wstr = string2wstring(description);
				TimePoint(wstr);
				delete wstr;
			}
			virtual void EndTimePoint() = 0;
			virtual void Clear() = 0;
			virtual void Show_TimeSpanReport(const wchar_t* caption) = 0;
			virtual void Show_TimeSpanReport(const char* caption) { 
				auto wstr = string2wstring(caption);
				Show_TimeSpanReport(wstr);
				delete wstr;
			}
			virtual void Save_TimeSpanReport(const wchar_t* filename) = 0;
			virtual void Save_TimeSpanReport(const char* filename) {
				auto wstr = string2wstring(filename);
				Save_TimeSpanReport(wstr);
				delete wstr;
			}
		};

		//For Testing & Default
		class JETALG_API CTimeSpan_Empty : public ITimeSpan
		{
		public:
			explicit CTimeSpan_Empty() {}
			virtual ~CTimeSpan_Empty() {}
			CTimeSpan_Empty& operator=(const CTimeSpan_Empty& obj) { return *this; }

			inline const char* GetVersion() { return "1.0.0"; }
			inline bool GetState_CrossFunc() { return false; }
			inline bool GetState_Skip() { return false; }
			inline void SkipOn() {}
			inline void SkipOff() {}

			inline void EntryFunc(const wchar_t* func) {}
			inline void EntryFunc(const char* func) {}
			inline void TimePoint(const wchar_t* description) {}
			inline void TimePoint(const char* description) {}
			inline void EndTimePoint() {}
			inline void Clear() {}
			inline void Show_TimeSpanReport(const wchar_t* caption) {}
			inline void Show_TimeSpanReport(const char* caption) {}
			inline void Save_TimeSpanReport(const wchar_t* filename) {}
			inline void Save_TimeSpanReport(const char* filename) {}
		};
	}
}
