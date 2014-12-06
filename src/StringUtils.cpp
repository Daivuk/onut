#include <codecvt>

#include "StringUtils.h"

namespace onut {
	std::wstring utf8ToUtf16(const std::string& utf8) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
		return conversion.from_bytes(utf8);
	}

	std::string utf16ToUtf8(const std::wstring& utf16) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
		return conversion.to_bytes(utf16);
	}
}
