//=====================================================================
//
// TGAImage.h - 该文件参考了 RenderHelp 的 RenderHelp.h
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
        Vector<std::uint8_t, 4> bgra;
        struct { std::uint8_t r, g, b, a; };
    };

    Color() : bgra{ 0,0,0,255 } {}
    Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) : bgra{ b,g,r,a } {}
    Color(const Color& c) : bgra(c.bgra) {}

    bool operator==(const Color& c) const { return this->bgra == c.bgra;  }
};

const Color Red(255, 0, 0);
const Color Blue(0, 0, 255);


enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4 };


class Bitmap
{
public:
    Bitmap(const int width, const int height) : w(width), h(height) { data.resize(width * height); }
    Bitmap(const std::string& filename) { this->LoadFile(filename); }


    inline int weight() const { return w; }
    inline int height() const { return h; }

    inline void Fill(const Color& color) { for (auto&& i : data) i = color; }
    void FlipVertical();
    void FlipHorizontal();
    void LoadFile(const std::string& filename);
    bool SaveFile(const std::string& filename, Format format = RGB) const;
    inline void SetPixel(int x, int y, const Color& color) { data[getIndex(x, y)] = color; }
    inline void SetPixel(const Vec2i& xy, const Color& color) { data[getIndex(xy.x, xy.y)] = color; }
    inline Color GetPixel(int x, int y) const { return data[getIndex(x, y)]; }
    inline Color GetPixel(const Vec2i& xy) const { return data[getIndex(xy.x, xy.y)]; }

    inline Color Sample2D(int u, int v) const { return GetPixel(u * w, v * h); }
    inline Color Sample2D(const Vec2i& uv) const { return GetPixel(uv.u * w, uv.v * h); }
    inline Color Sample2D(float u, float v);
    inline Color Sample2D(const Vec2f& uv);

private:
    int w;
    int h;
    std::vector<Color> data;

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

    inline int getIndex(int x, int y) const { return x * w + y; }
    bool unload_rle_data(std::ofstream& out, Format format) const;
};