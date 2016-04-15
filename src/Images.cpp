// Onut
#include <onut/Images.h>

// Third party
#include "lodepng/LodePNG.h"

namespace onut
{
    bool savePNG(const std::string& filename, const std::vector<uint8_t>& data, const Point& size)
    {
        return !lodepng::encode(filename, data, size.x, size.y);
    }

    std::vector<uint8_t> convertToPNG(const std::vector<uint8_t>& data, const Point& size)
    {
        std::vector<uint8_t> ret;
        lodepng::encode(ret, data, size.x, size.y);
        return ret;
    }

    std::vector<uint8_t> convertToPNG(const uint8_t* pData, const Point& size)
    {
        std::vector<uint8_t> ret;
        lodepng::encode(ret, pData, size.x, size.y);
        return ret;
    }

    std::vector<uint8_t> loadPNG(const std::vector<uint8_t>& data, Point& size)
    {
        std::vector<uint8_t> ret;
        unsigned int w, h;
        lodepng::decode(ret, w, h, data);
        size.x = static_cast<int>(w);
        size.y = static_cast<int>(h);
        return ret;
    }
};
