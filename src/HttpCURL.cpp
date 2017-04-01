// Onut
#include <onut/Async.h>
#include <onut/Dispatcher.h>
#include <onut/Strings.h>
#include <onut/Texture.h>

// Internal
#include "HttpCURL.h"

// Third party
#include <curl/curl.h>

// STL
#include <locale>

namespace onut
{
    OHttpRef Http::create()
    {
        return std::shared_ptr<Http>(new HttpCURL());
    }

    HttpCURL::HttpCURL()
    {
        curl_global_init(CURL_GLOBAL_SSL);
    }

    HttpCURL::~HttpCURL()
    {
        curl_global_cleanup();
    }

    static size_t curlWrite(void *buffer, size_t size, size_t nmemb, void *userp)
    {
        auto& ret = *(Http::Body*)userp;
        auto totalSize = size * nmemb;
        auto pos = ret.size();
        ret.resize(pos + totalSize);
        memcpy(ret.data() + pos, buffer, totalSize);
        return totalSize;
    }

    std::string HttpCURL::post(const std::string& url, const Body& body, const ErrorCallback& onError)
    {
        auto easyhandle = curl_easy_init();
        if (!easyhandle)
        {
            if (onError)
            {
                onError(0, "curl_easy_init failed");
            }
            return {};
        }

        Body ret;

        curl_easy_setopt(easyhandle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(easyhandle, CURLOPT_POST, 1);
        curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDSIZE, (long)body.size());
        curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, (char*)body.data());
        curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &ret);
        curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, curlWrite);
        auto performRet = curl_easy_perform(easyhandle);
        curl_easy_cleanup(easyhandle);

        if (performRet != CURLE_OK)
        {
            if (onError)
            {
                onError(0, "curl_easy_perform failed");
            }
            return {};
        }

        if (ret.empty()) return "";
        if (ret[ret.size() - 1] != 0) ret.push_back(0);

        return (char*)ret.data();
    }

    Http::Body HttpCURL::get(const std::string& url, const Arguments& arguments, const ErrorCallback& onError)
    {
        auto args = packArguments(arguments);
        auto fullUrl = url;
        if (!args.empty())
        {
            fullUrl = url + "?" + args;
        }

        auto easyhandle = curl_easy_init();
        if (!easyhandle)
        {
            if (onError)
            {
                onError(0, "curl_easy_init failed");
            }
            return {};
        }

        Body ret;

        curl_easy_setopt(easyhandle, CURLOPT_URL, fullUrl.c_str());
        curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &ret);
        curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, curlWrite);
        auto performRet = curl_easy_perform(easyhandle);
        curl_easy_cleanup(easyhandle);

        if (performRet != CURLE_OK)
        {
            if (onError)
            {
                onError(0, "curl_easy_perform failed");
            }
            return {};
        }

        return std::move(ret);
    }
}
