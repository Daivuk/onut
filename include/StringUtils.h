#pragma once
#include <string>

namespace onut {
	std::wstring	utf8ToUtf16(const std::string& utf8);
	std::string		utf16ToUtf8(const std::wstring& utf16);
}
