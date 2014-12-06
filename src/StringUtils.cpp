#include <codecvt>

#include "StringUtils.h"

namespace onut {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> g_stringConverter;

	std::wstring utf8ToUtf16(const std::string& utf8) {
		return g_stringConverter.from_bytes(utf8);
	}

	std::string utf16ToUtf8(const std::wstring& utf16) {
		return g_stringConverter.to_bytes(utf16);
	}
}
