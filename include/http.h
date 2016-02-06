#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

// Forward declare
namespace onut
{
    class Texture;
};
using OTextureRef = std::shared_ptr<onut::Texture>;

std::string OHTTPPost(const std::string &url,
                      const std::vector<std::pair<std::string, std::string>> &postArgs = {},
                      std::function<void(long, std::string)> onError = nullptr);
void OHTTPPostAsync(const std::string &url,
                    const std::vector<std::pair<std::string, std::string>> &postArgs = {},
                    std::function<void(std::string)> onSuccess = nullptr,
                    std::function<void(long, std::string)> onError = nullptr);

std::string OHTTPGet(const std::string &url,
                      std::function<void(long, std::string)> onError = nullptr);
void OHTTPGetAsync(const std::string &url,
                    std::function<void(std::string)> onSuccess = nullptr,
                    std::function<void(long, std::string)> onError = nullptr);

OTextureRef OHTTPGetTexture(const std::string &url,
                          std::function<void(long, std::string)> onError = nullptr);
void OHTTPGetTextureAsync(const std::string &url,
                          std::function<void(OTextureRef)> onSuccess = nullptr,
                          std::function<void(long, std::string)> onError = nullptr);
