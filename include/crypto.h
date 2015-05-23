#pragma once
#include <cinttypes>
#include <string>
#include <vector>

namespace onut
{
    unsigned int hash(const std::string &s, unsigned int seed = 0);
    std::string sha1(const std::string& str);
    bool validateEmail(const std::string& email);
    std::string base64_encode(uint8_t const* buf, unsigned int bufLen);
    std::vector<uint8_t> base64_decode(std::string const&);
};
