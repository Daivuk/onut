#pragma once
// STL
#include <functional>
#include <string>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Texture);

namespace onut
{
    namespace http
    {
        using ArgumentKey = std::string;
        using ArgumentValue = std::string;
        using Argument = std::pair<ArgumentKey, ArgumentValue>;
        using Arguments = std::vector<Argument>;
        using Body = std::vector<uint8_t>;
        using ErrorCallback = std::function<void(long, std::string)>;
        using PostCallback = std::function<void(std::string)>;
        using GetCallback = std::function<void(Body)>;
        using GetStringCallback = PostCallback;
        using TextureCallback = std::function<void(OTextureRef)>;

        std::string post(const std::string& url, const ErrorCallback& onError = nullptr);
        std::string post(const std::string& url, const Body& body, const ErrorCallback& onError = nullptr);
        std::string post(const std::string& url, const Arguments& arguments, const ErrorCallback& onError = nullptr);

        void postAsync(const std::string& url, const PostCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void postAsync(const std::string& url, const Body& body, const PostCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void postAsync(const std::string& url, const Arguments& arguments, const PostCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);

        Body get(const std::string& url, const ErrorCallback& onError = nullptr);
        Body get(const std::string& url, const Arguments& arguments, const ErrorCallback& onError = nullptr);
        std::string getString(const std::string& url, const ErrorCallback& onError = nullptr);
        std::string getString(const std::string& url, const Arguments& arguments, const ErrorCallback& onError = nullptr);

        void getAsync(const std::string& url, const GetCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void getAsync(const std::string& url, const Arguments& arguments, const GetCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void getStringAsync(const std::string& url, const GetStringCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void getStringAsync(const std::string& url, const Arguments& arguments, const GetStringCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
    };
};

#define OHttpPost onut::http::post
#define OHttpPostAsync onut::http::postAsync
#define OHttpGet onut::http::get
#define OHttpGetAsync onut::http::getAsync
#define OHttpGetString onut::http::getString
#define OHttpGetStringAsync onut::http::getStringAsync

OTextureRef OHTTPGetTexture(const std::string& url, const onut::http::Arguments& arguments = {}, const onut::http::ErrorCallback& onError = nullptr);
void OHTTPGetTextureAsync(const std::string& url, const onut::http::Arguments& arguments, const onut::http::TextureCallback& onSuccess = nullptr, const onut::http::ErrorCallback& onError = nullptr);
void OHTTPGetTextureAsync(const std::string& url, const onut::http::TextureCallback& onSuccess = nullptr, const onut::http::ErrorCallback& onError = nullptr);
