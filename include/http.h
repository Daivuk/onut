#pragma once
#include <string>
#include <vector>
#include <functional>

std::string OStringFromURL(const std::string &url, 
                           const std::vector<std::pair<std::string, std::string>> &postArgs = {}, 
                           std::function<void(long, std::string)> onError = nullptr);
