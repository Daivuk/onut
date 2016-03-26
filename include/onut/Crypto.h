#pragma once
// STL
#include <cinttypes>
#include <string>
#include <vector>

namespace onut
{
    namespace crypto
    {
        uint32_t hash(const std::string& s, unsigned int seed = 0);

        std::string sha1(const std::string& str);

        bool validateEmail(const std::string& email);

        std::string base64_encode(const uint8_t* buf, unsigned int bufLen);
        std::vector<uint8_t> base64_decode(const std::string& encoded_string);
    };
};

#define OHash onut::crypto::hash
#define OSha1 onut::crypto::sha1
#define OValidateEmail onut::crypto::validateEmail
#define OEncodeBase64 onut::crypto::base64_encode
#define ODecodeBase64 onut::crypto::base64_decode
