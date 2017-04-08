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

    std::vector<uint8_t> loadPNG(const std::string& filename, Point& size)
    {
        std::vector<uint8_t> ret;
        unsigned int w, h;
        lodepng::decode(ret, w, h, filename);
        size.x = static_cast<int>(w);
        size.y = static_cast<int>(h);
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

#if defined(WIN32)
    // Reference: https://www.codeproject.com/articles/5220/creating-a-color-cursor-from-a-bitmap
    void GetMaskBitmaps(HBITMAP hSourceBitmap,
                        COLORREF clrTransparent,
                        HBITMAP &hAndMaskBitmap,
                        HBITMAP &hXorMaskBitmap)
    {
        HDC hDC = ::GetDC(NULL);
        HDC hMainDC = ::CreateCompatibleDC(hDC);
        HDC hAndMaskDC = ::CreateCompatibleDC(hDC);
        HDC hXorMaskDC = ::CreateCompatibleDC(hDC);

        //Get the dimensions of the source bitmap
        BITMAP bm;
        ::GetObject(hSourceBitmap, sizeof(BITMAP), &bm);

        hAndMaskBitmap = ::CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);
        hXorMaskBitmap = ::CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight);

        //Select the bitmaps to DC
        HBITMAP hOldMainBitmap = (HBITMAP)::SelectObject(hMainDC, hSourceBitmap);
        HBITMAP hOldAndMaskBitmap = (HBITMAP)::SelectObject(hAndMaskDC,
                                                            hAndMaskBitmap);
        HBITMAP hOldXorMaskBitmap = (HBITMAP)::SelectObject(hXorMaskDC,
                                                            hXorMaskBitmap);

        //Scan each pixel of the souce bitmap and create the masks
        COLORREF MainBitPixel;
        for (int x = 0; x<bm.bmWidth; ++x)
        {
            for (int y = 0; y<bm.bmHeight; ++y)
            {
                MainBitPixel = ::GetPixel(hMainDC, x, y);
                if (MainBitPixel == clrTransparent)
                {
                    ::SetPixel(hAndMaskDC, x, y, RGB(255, 255, 255));
                    ::SetPixel(hXorMaskDC, x, y, RGB(0, 0, 0));
                }
                else
                {
                    ::SetPixel(hAndMaskDC, x, y, RGB(0, 0, 0));
                    ::SetPixel(hXorMaskDC, x, y, MainBitPixel);
                }
            }
        }

        ::SelectObject(hMainDC, hOldMainBitmap);
        ::SelectObject(hAndMaskDC, hOldAndMaskBitmap);
        ::SelectObject(hXorMaskDC, hOldXorMaskBitmap);

        ::DeleteDC(hXorMaskDC);
        ::DeleteDC(hAndMaskDC);
        ::DeleteDC(hMainDC);

        ::ReleaseDC(NULL, hDC);
    }

    HBITMAP createHBitmapFromData(const uint8_t* pngData, const Point& size)
    {
        HDC hDC = ::GetDC(NULL);
        HDC bDC = ::CreateCompatibleDC(hDC);
        HBITMAP b = ::CreateCompatibleBitmap(hDC, size.x, size.y);
        HBITMAP oldB = (HBITMAP)::SelectObject(bDC, b);

        auto len = size.x * size.y;
        std::vector<uint8_t> bitmapData(len * 3, 0);
        for (auto i = 0; i < len; ++i)
        {
            if (pngData[i * 4 + 3] < 128)
            {
                ::SetPixel(bDC, i % size.x, i / size.x, RGB(255, 0, 255));
            }
            else
            {
                ::SetPixel(bDC, i % size.x, i / size.x, RGB(pngData[i * 4 + 0], pngData[i * 4 + 1], pngData[i * 4 + 2]));
            }
        }

        ::SelectObject(bDC, oldB);
        ::DeleteDC(bDC);
        ::ReleaseDC(NULL, hDC);

        return b;
    }

    HCURSOR pngToCursor(const std::string& filename, const Point& center)
    {
        Point size;
        auto pngData = onut::loadPNG(filename, size);
        if (!pngData.empty())
        {
            auto b = createHBitmapFromData(pngData.data(), size);
            HBITMAP hAndMask = NULL, hXorMask = NULL;
            GetMaskBitmaps(b, RGB(255, 0, 255), hAndMask, hXorMask);

            ICONINFO iconinfo = {0};
            iconinfo.fIcon = FALSE;
            iconinfo.xHotspot = center.x;
            iconinfo.yHotspot = center.y;
            iconinfo.hbmMask = hAndMask;
            iconinfo.hbmColor = hXorMask;

            HCURSOR hCursor = ::CreateIconIndirect(&iconinfo);

            DeleteObject(hAndMask);
            DeleteObject(hXorMask);
            DeleteObject(b);

            return hCursor;
        }
        return NULL;
    }
#endif
};
