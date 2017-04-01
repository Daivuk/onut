#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

// STL
#include <functional>
#include <string>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Http);
OForwardDeclare(Texture);

namespace onut
{
    class Http : public std::enable_shared_from_this<Http>
    {
    public:
        static OHttpRef create();

        virtual ~Http();

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

        virtual std::string post(const std::string& url, const Body& body, const ErrorCallback& onError = nullptr) = 0;
        virtual Body get(const std::string& url, const Arguments& arguments, const ErrorCallback& onError = nullptr) = 0;

        std::string post(const std::string& url, const ErrorCallback& onError = nullptr);
        std::string post(const std::string& url, const Arguments& arguments, const ErrorCallback& onError = nullptr);

        void postAsync(const std::string& url, const PostCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void postAsync(const std::string& url, const Body& body, const PostCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void postAsync(const std::string& url, const Arguments& arguments, const PostCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);

        Body get(const std::string& url, const ErrorCallback& onError = nullptr);
        std::string getString(const std::string& url, const ErrorCallback& onError = nullptr);
        std::string getString(const std::string& url, const Arguments& arguments, const ErrorCallback& onError = nullptr);

        void getAsync(const std::string& url, const GetCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void getAsync(const std::string& url, const Arguments& arguments, const GetCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void getStringAsync(const std::string& url, const GetStringCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);
        void getStringAsync(const std::string& url, const Arguments& arguments, const GetStringCallback& onSuccess = nullptr, const ErrorCallback& onError = nullptr);

    protected:
        Http();

        static std::string packArguments(const Http::Arguments& arguments);
    };
};

extern OHttpRef oHttp;

#define OHttpPost oHttp->post
#define OHttpPostAsync oHttp->postAsync
#define OHttpGet oHttp->get
#define OHttpGetAsync oHttp->getAsync
#define OHttpGetString oHttp->getString
#define OHttpGetStringAsync oHttp->getStringAsync

OTextureRef OHTTPGetTexture(const std::string& url, const onut::Http::Arguments& arguments = {}, const onut::Http::ErrorCallback& onError = nullptr);
void OHTTPGetTextureAsync(const std::string& url, const onut::Http::Arguments& arguments, const onut::Http::TextureCallback& onSuccess = nullptr, const onut::Http::ErrorCallback& onError = nullptr);
void OHTTPGetTextureAsync(const std::string& url, const onut::Http::TextureCallback& onSuccess = nullptr, const onut::Http::ErrorCallback& onError = nullptr);

#endif
