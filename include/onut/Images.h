#ifndef IMAGES_H_INCLUDED
#define IMAGES_H_INCLUDED

// Onut
#include <onut/Point.h>

// STL
#include <cinttypes>
#include <string>
#include <vector>

namespace onut
{
    bool savePNG(const std::string& filename, const std::vector<uint8_t>& data, const Point& size);
    std::vector<uint8_t> convertToPNG(const std::vector<uint8_t>& data, const Point& size);
    std::vector<uint8_t> convertToPNG(const uint8_t* pData, const Point& size);
    std::vector<uint8_t> loadPNG(const std::vector<uint8_t>& data, Point& size);
};

#endif
