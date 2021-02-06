#pragma once

#include <string>
#include "JsonHelper.h"

class Project final
{
public:
    std::string path;
    Json::Value json;

    Project();
    ~Project();

    void openFolder(const std::string& path);
    void openScene(const std::string& filename);
};
