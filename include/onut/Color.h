#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

// STL
#include <cinttypes>
#include <utility>

// Forward declarations
struct Vector3;
struct Vector4;

struct Color
{
    float r, g, b, a;

    Color() : r(0), g(0), b(0), a(1.f) {}
    Color(float _rgba) : r(_rgba), g(_rgba), b(_rgba), a(_rgba) {}
    Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1.f) {}
    Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
    Color(const Color& rgb, float _a) : r(rgb.r), g(rgb.g), b(rgb.b), a(_a) {}
    Color(const Vector3& rgb, float _a);
    explicit Color(const Vector3& clr);
    explicit Color(const Vector4& clr);
    explicit Color(const float *pArray) : r(pArray[0]), g(pArray[1]), b(pArray[2]), a(pArray[3]) {}

    void unpack(uint32_t packed)
    {
        r = (float)((packed >> 24) & 0xff) / 255.f;
        g = (float)((packed >> 16) & 0xff) / 255.f;
        b = (float)((packed >> 8) & 0xff) / 255.f;
        a = (float)(packed & 0xff) / 255.f;
    }

    uint32_t pack() const
    {
        uint32_t packed = 0;
        packed |= ((uint32_t)(r * 255.f) << 24) & 0xff000000;
        packed |= ((uint32_t)(g * 255.f) << 16) & 0x00ff0000;
        packed |= ((uint32_t)(b * 255.f) << 8) & 0x0000ff00;
        packed |= (uint32_t)(a * 255.f) & 0x000000ff;
        return packed;
    }

    static Color fromHexRGB(unsigned int hex)
    {
        return Color(
            static_cast<float>((hex & 0xff0000) >> 16) / 255.f,
            static_cast<float>((hex & 0x00ff00) >> 8) / 255.f,
            static_cast<float>((hex & 0x0000ff)) / 255.f,
            1.f
            );
    }

    static Color fromHexRGBA(unsigned int hex)
    {
        return Color(
            static_cast<float>((hex & 0xff000000) >> 24) / 255.f,
            static_cast<float>((hex & 0x00ff0000) >> 16) / 255.f,
            static_cast<float>((hex & 0x0000ff00) >> 8) / 255.f,
            static_cast<float>((hex & 0x000000ff)) / 255.f
            );
    }

    operator const float*() const { return &r; }

    // Comparision operators
    bool operator == (const Color& c) const
    {
        return r == c.r && g == c.g && b == c.b && a == c.a;
    }

    bool operator != (const Color& c) const
    {
        return r != c.r || g != c.g || b != c.b || a != c.a;
    }

    // Assignment operators
    Color& operator= (const Color& c) { r = c.r; g = c.g; b = c.b; a = c.a; return *this; }
    Color& operator+= (const Color& c) { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
    Color& operator-= (const Color& c) { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }
    Color& operator*= (const Color& c) { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
    Color& operator*= (float S) { r *= S; g *= S; b *= S; a *= S; return *this; }
    Color& operator/= (const Color& c)
    {
        r = c.r; g = c.g; b = c.b; a = c.a;
        return *this;
    }

    // Urnary operators
    Color operator+ () const { return *this; }
    Color operator- () const { return Color(-r, -g, -b, -a); };

    Vector3 ToVector3() const;
    Vector4 ToVector4() const;

    void Negate()
    {
        r = 1.0f - r;
        g = 1.0f - g;
        b = 1.0f - b;
    }
    void Negate(Color& result) const
    {
        result.r = 1.0f - r;
        result.g = 1.0f - g;
        result.b = 1.0f - b;
        result.a = a;
    }

    void Saturate()
    {
        r = (((r < 0.0f) ? 0.0f : r) > 1.0f) ? 1.0f : r;
        g = (((g < 0.0f) ? 0.0f : g) > 1.0f) ? 1.0f : g;
        b = (((b < 0.0f) ? 0.0f : b) > 1.0f) ? 1.0f : b;
        a = (((a < 0.0f) ? 0.0f : a) > 1.0f) ? 1.0f : a;
    }
    void Saturate(Color& result) const
    {
        result.r = (((r < 0.0f) ? 0.0f : r) > 1.0f) ? 1.0f : r;
        result.g = (((g < 0.0f) ? 0.0f : g) > 1.0f) ? 1.0f : g;
        result.b = (((b < 0.0f) ? 0.0f : b) > 1.0f) ? 1.0f : b;
        result.a = (((a < 0.0f) ? 0.0f : a) > 1.0f) ? 1.0f : a;
    }

    void Premultiply()
    {
        r *= a;
        g *= a;
        b *= a;
    }
    void Premultiply(Color& result) const
    {
        result.r = r * a;
        result.g = g * a;
        result.b = b * a;
        result.a = a;
    }

    void AdjustSaturation(float sat)
    {
        float fLuminance = (r * 0.2125f) + (g * 0.7154f) + (b * 0.0721f);
        r = ((r - fLuminance) * sat) + fLuminance;
        g = ((g - fLuminance) * sat) + fLuminance;
        b = ((b - fLuminance) * sat) + fLuminance;
    }
    void AdjustSaturation(float sat, Color& result) const
    {
        float fLuminance = (r * 0.2125f) + (g * 0.7154f) + (b * 0.0721f);
        result.r = ((r - fLuminance) * sat) + fLuminance;
        result.g = ((g - fLuminance) * sat) + fLuminance;
        result.b = ((b - fLuminance) * sat) + fLuminance;
        result.a = a;
    }
    Color AdjustedSaturation(float sat) const
    {
        Color result;
        float fLuminance = (r * 0.2125f) + (g * 0.7154f) + (b * 0.0721f);
        result.r = ((r - fLuminance) * sat) + fLuminance;
        result.g = ((g - fLuminance) * sat) + fLuminance;
        result.b = ((b - fLuminance) * sat) + fLuminance;
        result.a = a;
        return std::move(result);
    }

    void AdjustContrast(float contrast)
    {
        r = ((r - 0.5f) * contrast) + 0.5f;
        g = ((g - 0.5f) * contrast) + 0.5f;
        b = ((b - 0.5f) * contrast) + 0.5f;
    }
    void AdjustContrast(float contrast, Color& result) const
    {
        result.r = ((r - 0.5f) * contrast) + 0.5f;
        result.g = ((g - 0.5f) * contrast) + 0.5f;
        result.b = ((b - 0.5f) * contrast) + 0.5f;
        result.a = a;
    }
    Color AdjustedContrast(float contrast) const
    {
        Color result;
        result.r = ((r - 0.5f) * contrast) + 0.5f;
        result.g = ((g - 0.5f) * contrast) + 0.5f;
        result.b = ((b - 0.5f) * contrast) + 0.5f;
        result.a = a;
        return std::move(result);
    }

    // Static functions
    static void Modulate(const Color& c1, const Color& c2, Color& result)
    {
        result.r = c1.r * c2.r;
        result.g = c1.g * c2.g;
        result.b = c1.b * c2.b;
        result.a = c1.a * c2.a;
    }
    static Color Modulate(const Color& c1, const Color& c2)
    {
        Color result;
        result.r = c1.r * c2.r;
        result.g = c1.g * c2.g;
        result.b = c1.b * c2.b;
        result.a = c1.a * c2.a;
        return std::move(result);
    }

    static void Lerp(const Color& c1, const Color& c2, float t, Color& result)
    {
        result.r = c1.r + (c2.r - c1.r) * t;
        result.g = c1.g + (c2.g - c1.g) * t;
        result.b = c1.b + (c2.b - c1.b) * t;
        result.a = c1.a + (c2.a - c1.a) * t;
    }
    static Color Lerp(const Color& c1, const Color& c2, float t)
    {
        Color result;
        result.r = c1.r + (c2.r - c1.r) * t;
        result.g = c1.g + (c2.g - c1.g) * t;
        result.b = c1.b + (c2.b - c1.b) * t;
        result.a = c1.a + (c2.a - c1.a) * t;
        return std::move(result);
    }

    // Constants
    static const Color Black;
    static const Color White;
    static const Color Transparent;
    static const Color TransparentWhite;
};

// Binary operators
Color operator+ (const Color& C1, const Color& C2);
Color operator- (const Color& C1, const Color& C2);
Color operator* (const Color& C1, const Color& C2);
Color operator* (const Color& C, float S);
Color operator/ (const Color& C1, const Color& C2);
Color operator* (float S, const Color& C);

// Defines
#define OColorHex(hex)                          Color::fromHexRGB(0x ## hex)
#define OColorRGB(r, g, b)                      Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
#define OColorRGBA(r, g, b, a)                  Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f)
#define OColorBGR(b, g, r)                      Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f)
#define OColorBGRA(b, g, r, a)                  Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f)

#endif
