#pragma once
#include <string>
#include <vector>
#include <functional>

std::string OHTTPPost(const std::string &url,
                      const std::vector<std::pair<std::string, std::string>> &postArgs = {},
                      std::function<void(long, std::string)> onError = nullptr);

void OHTTPPostAsync(const std::string &url,
                    const std::vector<std::pair<std::string, std::string>> &postArgs = {},
                    std::function<void(std::string)> onSuccess = nullptr,
                    std::function<void(long, std::string)> onError = nullptr);
