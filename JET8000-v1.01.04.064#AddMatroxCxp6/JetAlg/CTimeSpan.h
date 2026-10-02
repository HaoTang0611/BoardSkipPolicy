#pragma once
#include "ITimeSpan.h"

namespace JET {
	namespace tkt {

		class JETALG_API CTimeSpan : public ATimeSpan
		{
		private:
			void Cal_TimeSpan();
			std::wstring Get_TimeSpanReport();

		public:
			explicit CTimeSpan() {}
			~CTimeSpan() {}
			CTimeSpan& operator=(const CTimeSpan& obj) { return *this; }

			// ³z¹L ATimeSpan Ä~©Ó
			virtual const char* GetVersion() override;
			virtual void EntryFunc(const wchar_t* func) override;
			virtual void TimePoint(const wchar_t* description) override;
			virtual void EndTimePoint() override;
			virtual void Clear() override;
			virtual void Show_TimeSpanReport(const wchar_t* caption) override;
			virtual void Save_TimeSpanReport(const wchar_t* filename) override;			
		};
	}
}
