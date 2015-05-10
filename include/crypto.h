#pragma once
#include <string>

namespace onut
{
    unsigned int hash(const std::string &s, unsigned int seed = 0);
    std::string sha1(const std::string& str);
    bool validateEmail(const std::string& email);
};
