// Onut
#include <onut/Http.h>

namespace onut
{
    class HttpCURL final : public Http
    {
    public:
        HttpCURL();
        ~HttpCURL();

        std::string post(const std::string& url, const Body& body, const ErrorCallback& onError = nullptr) override;
        Body get(const std::string& url, const Arguments& arguments, const ErrorCallback& onError = nullptr) override;
    };
};
