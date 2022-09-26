//=====================================================================
//
// Bitmap.h - 该文件参考了 RenderHelp 的 RenderHelp.h
//
//=====================================================================

#include <cstdint>
#include <vector>
#include <string>
#include "Geometry.h"

struct Color
{
    union
    {
        Vector<std::uint8_t, 4> rgba;
        struct { std::uint8_t r, g, b, a; };
    };

    Color() : rgba{ 255,0,0,0 } {}
    Color(std::uint8_t& r, std::uint8_t& g, std::uint8_t& b, std::uint8_t& a) : rgba{ r,g,b,a } {}
    Color(const Color& c) : rgba(c.rgba) {}
};

class Bitmap
{
private:
    int32_t _w;
    int32_t _h;
    std::vector<Color> _bits;

    struct BITMAPINFOHEADER { // bmih  
        uint32_t	biSize;
        uint32_t	biWidth;
        int32_t		biHeight;
        uint16_t	biPlanes;
        uint16_t	biBitCount;
        uint32_t	biCompression;
        uint32_t	biSizeImage;
        uint32_t	biXPelsPerMeter;
        uint32_t	biYPelsPerMeter;
        uint32_t	biClrUsed;
        uint32_t	biClrImportant;
    };

    inline int getIndex(int x, int y) const { return x * _w + y; }

public:
    Bitmap(int width, int height) : _w(width), _h(height) { _bits.resize(width * height); }
    Bitmap(const std::string& filename) { this->LoadFile(filename); }

    inline int weight() const { return _w; }
    inline int height() const { return _h; }

    inline void Fill(const Color& color) { for (auto&& i : _bits) i = color; }
    void FlipVertical();
    void FlipHorizontal();
    void LoadFile(const std::string& filename);
    bool SaveFile(const std::string& filename, bool withAlpha = false) const;
    inline void SetPixel(int x, int y, const Color& color) { _bits[getIndex(x, y)] = color; }
    inline void SetPixel(const Vec2i& xy, const Color& color) { _bits[getIndex(xy.x, xy.y)] = color; }
    inline Color GetPixel(int x, int y) const { return _bits[getIndex(x, y)]; }
    inline Color GetPixel(const Vec2i& xy) const { return _bits[getIndex(xy.x, xy.y)]; }

    inline Color Sample2D(int u, int v) const { return GetPixel(u * _w, v * _h); }
    inline Color Sample2D(const Vec2i& uv) const { return GetPixel(uv.u * _w, uv.v * _h); }
    inline Color Sample2D(float u, float v);
    inline Color Sample2D(const Vec2f& uv);
};