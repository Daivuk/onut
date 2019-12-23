// Onut
#include <onut/Async.h>
#include <onut/Dispatcher.h>
#include <onut/Http.h>
#include <onut/Strings.h>
#include <onut/Texture.h>

// STL
#include <locale>

OHttpRef oHttp;

namespace onut
{
    Http::Http()
    {
    }

    Http::~Http()
    {
    }

    std::string Http::post(const std::string& url, const ErrorCallback& onError)
    {
        return post(url, Body{}, onError);
    }

    std::string Http::packArguments(const Http::Arguments& arguments)
    {
        std::string args;
        bool first = true;
        for (auto &kv : arguments)
        {
            if (first)
            {
                first = false;
                args += kv.first + "=" + kv.second;
            }
            else
            {
                args += "&" + kv.first + "=" + kv.second;
            }
        }
        return std::move(args);
    }

    std::string Http::post(const std::string& url, const Arguments& arguments, const ErrorCallback& onError)
    {
        auto args = packArguments(arguments);
        return post(url, Body(args.begin(), args.end()), onError);
    }

    void Http::postAsync(const std::string& url, const Body& body, const PostCallback& onSuccess, const ErrorCallback& onError)
    {
        auto pThis = OThis;
        OAsync([url, body, onSuccess, onError, pThis]
        {
            auto ret = pThis->post(url, body, [onError](long errCode, std::string message)
            {
                if (onError)
                {
                    OSync([errCode, message, onError]
                    {
                        onError(errCode, message);
                    });
                }
            });
            if (!ret.empty() && onSuccess)
            {
                OSync([ret, onSuccess]
                {
                    onSuccess(ret);
                });
            }
        });
    }

    void Http::postAsync(const std::string& url, const PostCallback& onSuccess, const ErrorCallback& onError)
    {
        postAsync(url, Body{}, onSuccess, onError);
    }

    void Http::postAsync(const std::string& url, const Arguments& arguments, const PostCallback& onSuccess, const ErrorCallback& onError)
    {
        auto args = packArguments(arguments);
        postAsync(url, Body(args.begin(), args.end()), onSuccess, onError);
    }

    Http::Body Http::get(const std::string& url, const ErrorCallback& onError)
    {
        return get(url, {}, onError);
    }

    std::string Http::getString(const std::string& url, const ErrorCallback& onError)
    {
        auto body = get(url, {}, onError);
        return{reinterpret_cast<const char*>(body.data()), body.size()};
    }

    std::string Http::getString(const std::string& url, const Arguments& arguments, const ErrorCallback& onError)
    {
        auto body = get(url, arguments, onError);
        return{reinterpret_cast<const char*>(body.data()), body.size()};
    }

    void Http::getAsync(const std::string& url, const Arguments& arguments, const GetCallback& onSuccess, const ErrorCallback& onError)
    {
        auto pThis = OThis;
        OAsync([url, arguments, onSuccess, onError, pThis]
        {
            auto ret = pThis->get(url, arguments, [onError](long errCode, std::string message)
            {
                if (onError)
                {
                    OSync([errCode, message, onError]
                    {
                        onError(errCode, message);
                    });
                }
            });
            if (!ret.empty() && onSuccess)
            {
                OSync([ret, onSuccess]
                {
                    onSuccess(ret);
                });
            }
        });
    }

    void Http::getAsync(const std::string& url, const GetCallback& onSuccess, const ErrorCallback& onError)
    {
        getAsync(url, {}, onSuccess, onError);
    }

    void Http::getStringAsync(const std::string& url, const Arguments& arguments, const GetStringCallback& onSuccess, const ErrorCallback& onError)
    {
        auto pThis = OThis;
        OAsync([url, arguments, onSuccess, onError, pThis]
        {
            auto ret = pThis->getString(url, arguments, [onError](long errCode, std::string message)
            {
                if (onError)
                {
                    OSync([errCode, message, onError]
                    {
                        onError(errCode, message);
                    });
                }
            });
            if (!ret.empty() && onSuccess)
            {
                OSync([ret, onSuccess]
                {
                    onSuccess(ret);
                });
            }
        });
    }

    void Http::getStringAsync(const std::string& url, const GetStringCallback& onSuccess, const ErrorCallback& onError)
    {
        getStringAsync(url, {}, onSuccess, onError);
    }
}

OTextureRef OHTTPGetTexture(const std::string& url, const onut::Http::Arguments& arguments, const onut::Http::ErrorCallback& onError)
{
    auto body = oHttp->get(url, arguments, onError);
    if (!body.empty())
    {
        return OTexture::createFromFileData(body.data(), (uint32_t)body.size());
    }
    return nullptr;
}

void OHTTPGetTextureAsync(const std::string& url, const onut::Http::Arguments& arguments, const onut::Http::TextureCallback& onSuccess, const onut::Http::ErrorCallback& onError)
{
    OAsync([url, arguments, onSuccess, onError]
    {
        auto ret = OHTTPGetTexture(url, arguments, [onError](long errCode, std::string message)
        {
            if (onError)
            {
                OSync([errCode, message, onError]
                {
                    onError(errCode, message);
                });
            }
        });
        if (ret && onSuccess)
        {
            OSync([ret, onSuccess]
            {
                onSuccess(ret);
            });
        }
    });
}

void OHTTPGetTextureAsync(const std::string& url, const onut::Http::TextureCallback& onSuccess, const onut::Http::ErrorCallback& onError)
{
    OHTTPGetTextureAsync(url, {}, onSuccess, onError);
}
