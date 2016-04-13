#pragma once
// Onut
#include <onut/Point.h>

// STL
#include <cinttypes>
#include <string>
#include <vector>

namespace onut
{
    bool savePNG(const std::string& filename, const std::vector<uint8_t>& data, const Point& size);
};
