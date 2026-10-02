#include "JETAlg_Std.h"
#include <memory>
#include <string>

namespace JET {
	namespace tkt {
		class JETALG_API CStringCvt {
		public:
			static inline std::shared_ptr<std::string> ASCII_To_UTF8(const std::string& s);
			static inline std::shared_ptr<std::string> UTF8_To_ASCII(const std::string& s);
			static inline std::shared_ptr<std::wstring> ASCII_To_UNICODE(const std::string& s);
			static inline std::shared_ptr<std::wstring> UTF8_To_UNICODE(const std::string& s);
			static inline std::shared_ptr<std::string> UNICODE_To_ASCII(const std::wstring& ws);
			static inline std::shared_ptr<std::string> UNICODE_To_UTF8(const std::wstring& ws);
		};
	}
}