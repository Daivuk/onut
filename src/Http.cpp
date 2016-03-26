// Onut
#include <onut/Async.h>
#include <onut/Dispatcher.h>
#include <onut/Http.h>
#include <onut/Strings.h>
#include <onut/Texture.h>

// Third party
#if defined(WIN32)
#include <atlbase.h>
#include <comutil.h>
#include <msxml6.h>
#endif

// STL
#include <locale>

namespace onut
{
    namespace http
    {
        std::string post(const std::string& url, const Body& body, const ErrorCallback& onError)
        {
#if defined(WIN32)
            HRESULT hr;
            CComPtr<IXMLHTTPRequest> request;

            hr = request.CoCreateInstance(CLSID_XMLHTTP60);
            hr = request->open(_bstr_t("POST"),
                                _bstr_t(url.c_str()),
                                _variant_t(VARIANT_FALSE),
                                _variant_t(),
                                _variant_t());

            if (body.empty())
            {
                hr = request->send(_variant_t());
            }
            else
            {
                request->setRequestHeader(_bstr_t("Content-type"), _bstr_t("application/x-www-form-urlencoded"));
                request->setRequestHeader(_bstr_t("Content-length"), _bstr_t(std::to_string(body.size()).c_str()));

                _variant_t var;
                var.vt = VT_ARRAY | VT_UI1;

                SAFEARRAYBOUND rgsabound[1];
                rgsabound[0].cElements = body.size();
                rgsabound[0].lLbound = 0;

                var.parray = SafeArrayCreate(VT_UI1, 1, rgsabound);

                void * pArrayData = NULL;
                SafeArrayAccessData(var.parray, &pArrayData);
                memcpy(pArrayData, body.data(), body.size());
                SafeArrayUnaccessData(var.parray);

                hr = request->send(var);
            }

            if (hr != S_OK)
            {
                if (onError)
                {
                    onError(0, "error");
                }
                return "";
            }

            // get status - 200 if succuss
            long status;
            hr = request->get_status(&status);
            if (hr != S_OK)
            {
                if (onError)
                {
                    onError(0, "error");
                }
                return "";
            }

            // Load the text
            BSTR bstrResponse = NULL;
            request->get_responseText(&bstrResponse);
            std::wstring wret = bstrResponse;
            if (bstrResponse) SysFreeString(bstrResponse);
            std::string ret = onut::utf16ToUtf8(wret);

            if (status == 200)
            {
                return ret;
            }
            else
            {
                if (onError)
                {
                    onError(status, ret);
                }
                return "";
            }
#endif
        }

        std::string post(const std::string& url, const ErrorCallback& onError)
        {
            return post(url, Body{}, onError);
        }

        static std::string packArguments(const Arguments& arguments)
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

        std::string post(const std::string& url, const Arguments& arguments, const ErrorCallback& onError)
        {
            auto args = packArguments(arguments);
            return post(url, Body(args.begin(), args.end()), onError);
        }

        void postAsync(const std::string& url, const Body& body, const PostCallback& onSuccess, const ErrorCallback& onError)
        {
            OAsync([url, body, onSuccess, onError]
            {
                auto ret = post(url, body, [onError](long errCode, std::string message)
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

        void postAsync(const std::string& url, const PostCallback& onSuccess, const ErrorCallback& onError)
        {
            postAsync(url, Body{}, onSuccess, onError);
        }

        void postAsync(const std::string& url, const Arguments& arguments, const PostCallback& onSuccess, const ErrorCallback& onError)
        {
            auto args = packArguments(arguments);
            postAsync(url, Body(args.begin(), args.end()), onSuccess, onError);
        }

        Body get(const std::string& url, const Arguments& arguments, const ErrorCallback& onError)
        {
#if defined(WIN32)
            auto args = packArguments(arguments);
            auto fullUrl = url;
            if (!args.empty())
            {
                fullUrl = url + "?" + args;
            }

            HRESULT hr;
            CComPtr<IXMLHTTPRequest> request;

            hr = request.CoCreateInstance(CLSID_XMLHTTP60);
            hr = request->open(_bstr_t("GET"),
                               _bstr_t(fullUrl.c_str()),
                               _variant_t(VARIANT_FALSE),
                               _variant_t(),
                               _variant_t());
            hr = request->send(_variant_t());

            if (hr != S_OK)
            {
                if (onError)
                {
                    onError(0, "error");
                }
                return{};
            }

            // get status - 200 if succuss
            long status;
            hr = request->get_status(&status);
            if (hr != S_OK)
            {
                if (onError)
                {
                    onError(0, "error");
                }
                return{};
            }

            // Load the texture
            VARIANT body;
            request->get_responseBody(&body);

            if (status == 200)
            {
                auto pData = (const uint8_t*)body.parray->pvData;
                return{pData, pData + body.parray->rgsabound[0].cElements};
            }
            else
            {
                if (onError)
                {
                    BSTR bstrResponse = NULL;
                    request->get_responseText(&bstrResponse);
                    std::wstring wret = bstrResponse;
                    if (bstrResponse) SysFreeString(bstrResponse);
                    std::string ret = onut::utf16ToUtf8(wret);
                    onError(status, ret);
                }
                return{};
            }
#endif
        }

        Body get(const std::string& url, const ErrorCallback& onError)
        {
            return get(url, {}, onError);
        }

        std::string getString(const std::string& url, const ErrorCallback& onError)
        {
            auto body = get(url, {}, onError);
            return{reinterpret_cast<const char*>(body.data()), body.size()};
        }

        std::string getString(const std::string& url, const Arguments& arguments, const ErrorCallback& onError)
        {
            auto body = get(url, arguments, onError);
            return{reinterpret_cast<const char*>(body.data()), body.size()};
        }

        void getAsync(const std::string& url, const Arguments& arguments, const GetCallback& onSuccess, const ErrorCallback& onError)
        {
            OAsync([url, arguments, onSuccess, onError]
            {
                auto ret = get(url, arguments, [onError](long errCode, std::string message)
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

        void getAsync(const std::string& url, const GetCallback& onSuccess, const ErrorCallback& onError)
        {
            getAsync(url, {}, onSuccess, onError);
        }

        void getStringAsync(const std::string& url, const Arguments& arguments, const GetStringCallback& onSuccess, const ErrorCallback& onError)
        {
            OAsync([url, arguments, onSuccess, onError]
            {
                auto ret = getString(url, arguments, [onError](long errCode, std::string message)
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

        void getStringAsync(const std::string& url, const GetStringCallback& onSuccess, const ErrorCallback& onError)
        {
            getStringAsync(url, {}, onSuccess, onError);
        }
    }
}

OTextureRef OHTTPGetTexture(const std::string& url, const onut::http::Arguments& arguments, const onut::http::ErrorCallback& onError)
{
    auto body = onut::http::get(url, arguments, onError);
    if (!body.empty())
    {
        return OTexture::createFromFileData(body.data(), body.size());
    }
    return nullptr;
}

void OHTTPGetTextureAsync(const std::string& url, const onut::http::Arguments& arguments, const onut::http::TextureCallback& onSuccess, const onut::http::ErrorCallback& onError)
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

void OHTTPGetTextureAsync(const std::string& url, const onut::http::TextureCallback& onSuccess, const onut::http::ErrorCallback& onError)
{
    OHTTPGetTextureAsync(url, {}, onSuccess, onError);
}
