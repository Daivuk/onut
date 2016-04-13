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
};
