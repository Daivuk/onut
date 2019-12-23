// Onut
#include <onut/Async.h>
#include <onut/Dispatcher.h>
#include <onut/Strings.h>
#include <onut/Texture.h>

// Internal
#include "HttpXMLHTTPRequest.h"

// Third party
#include <atlbase.h>
#include <comutil.h>
#include <msxml6.h>

// STL
#include <locale>

namespace onut
{
    OHttpRef Http::create()
    {
        return std::shared_ptr<Http>(new HttpXMLHTTPRequest());
    }

    HttpXMLHTTPRequest::HttpXMLHTTPRequest()
    {
    }

    HttpXMLHTTPRequest::~HttpXMLHTTPRequest()
    {
    }

    std::string HttpXMLHTTPRequest::post(const std::string& url, const Body& body, const ErrorCallback& onError)
    {
        HRESULT hr;
        CComPtr<IXMLHTTPRequest> request;

        if (CoInitializeEx(NULL, COINIT_MULTITHREADED) != S_OK)
        {
            if (onError)
            {
                onError(0, "error");
            }
            return "";
        }
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
            rgsabound[0].cElements = (ULONG)body.size();
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
    }

    Http::Body HttpXMLHTTPRequest::get(const std::string& url, const Arguments& arguments, const ErrorCallback& onError)
    {
        auto args = packArguments(arguments);
        auto fullUrl = url;
        if (!args.empty())
        {
            fullUrl = url + "?" + args;
        }

        HRESULT hr;
        CComPtr<IXMLHTTPRequest> request;

        if (CoInitializeEx(NULL, COINIT_MULTITHREADED) != S_OK)
        {
            if (onError)
            {
                onError(0, "error");
            }
            return{};
        }
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
    }
}
