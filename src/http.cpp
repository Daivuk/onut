#include "http.h"
#include <atlbase.h>
#include <comutil.h>
#include <msxml6.h>
#include <locale>
#include "Utils.h"
#include "Asynchronous.h"

std::string OHTTPGet(const std::string &url,
                     std::function<void(long, std::string)> onError)
{
    return OHTTPPost(url, {}, onError);
}

void OHTTPGetAsync(const std::string &url,
                   std::function<void(std::string)> onSuccess,
                   std::function<void(long, std::string)> onError)
{
    OHTTPPostAsync(url, {}, onSuccess, onError);
}

void OHTTPPostAsync(const std::string &url,
                    const std::vector<std::pair<std::string, std::string>> &postArgs,
                    std::function<void(std::string)> onSuccess,
                    std::function<void(long, std::string)> onError)
{
    OAsync([url, postArgs, onSuccess, onError]
    {
        auto ret = OHTTPPost(url, postArgs, [onError](long errCode, std::string message)
        {
            OSync([errCode, message, onError]
            {
                if (onError)
                {
                    onError(errCode, message);
                }
            });
        });
        if (!ret.empty())
        {
            OSync([ret, onSuccess]
            {
                onSuccess(ret);
            });
        }
    });
}

std::string OHTTPPost(const std::string &url, 
                      const std::vector<std::pair<std::string, std::string>> &postArgs,
                      std::function<void(long, std::string)> onError)
{
    HRESULT hr;
    CComPtr<IXMLHTTPRequest> request;

    hr = request.CoCreateInstance(CLSID_XMLHTTP60);
    if (postArgs.empty())
    {
        hr = request->open(_bstr_t("GET"),
                           _bstr_t(url.c_str()),
                           _variant_t(VARIANT_FALSE),
                           _variant_t(),
                           _variant_t());
        hr = request->send(_variant_t());
    }
    else
    {
        hr = request->open(_bstr_t("POST"),
                           _bstr_t(url.c_str()),
                           _variant_t(VARIANT_FALSE),
                           _variant_t(),
                           _variant_t());
        std::string args;
        bool first = true;
        for (auto &kv : postArgs)
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
        request->setRequestHeader(_bstr_t("Content-type"), _bstr_t("application/x-www-form-urlencoded"));
        request->setRequestHeader(_bstr_t("Content-length"), _bstr_t(std::to_string(args.size()).c_str()));
        hr = request->send(_variant_t(args.c_str()));
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
}

OTexture* OHTTPGetTexture(const std::string &url,
                               std::function<void(long, std::string)> onError)
{
    HRESULT hr;
    CComPtr<IXMLHTTPRequest> request;

    hr = request.CoCreateInstance(CLSID_XMLHTTP60);
    hr = request->open(_bstr_t("GET"),
                        _bstr_t(url.c_str()),
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
        return nullptr;
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
        return nullptr;
    }

    // Load the texture
    VARIANT body;
    request->get_responseBody(&body);

    if (status == 200)
    {
        return OTexture::createFromFileData((const uint8_t*)body.parray->pvData, body.parray->rgsabound[0].cElements);
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
        return nullptr;
    }
}

void OHTTPGetTextureAsync(const std::string &url,
                          std::function<void(OTexture*)> onSuccess,
                          std::function<void(long, std::string)> onError)
{
    OAsync([url, onSuccess, onError]
    {
        auto ret = OHTTPGetTexture(url, [onError](long errCode, std::string message)
        {
            OSync([errCode, message, onError]
            {
                if (onError)
                {
                    onError(errCode, message);
                }
            });
        });
        OSync([ret, onSuccess]
        {
            onSuccess(ret);
        });
    });
}
