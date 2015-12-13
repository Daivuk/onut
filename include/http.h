#pragma once
#include <string>
#include <vector>
#include <functional>

#include "Texture.h"

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

OTexture* OHTTPGetTexture(const std::string &url,
                          std::function<void(long, std::string)> onError = nullptr);
void OHTTPGetTextureAsync(const std::string &url,
                          std::function<void(OTexture*)> onSuccess = nullptr,
                          std::function<void(long, std::string)> onError = nullptr);
