#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <cstdint>
#include <vector>
#include <string>
#include "Vector.h"

struct Color
{
    union
    {
        Vector<std::uint8_t, 4> bgra;
        struct { std::uint8_t b, g, r, a; };
    };

    Color();
    Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);
    Color(const Vector<std::uint8_t, 4>& v);
    Color(const Color& c);
    
    bool operator==(const Color& c) const;
    bool operator!=(const Color& c) const;
    Color operator+(const Color& c) const;
    Color operator*(double x) const;
    friend Color operator*(double x, const Color& c);
};

const Color Red(255, 0, 0);
const Color Blue(0, 0, 255);
const Color White(255, 255, 255);
const Color Black(0, 0, 0);

enum Format { GRAYSCALE = 1, RGB = 3, RGBA = 4 };

class Bitmap
{
public:
    Bitmap(const int width = 0, const int height = 0);
    Bitmap(const std::string& filename);


    int width() const;
    int height() const;

    void Fill(const Color& color);
    void FlipVertical();
    void FlipHorizontal();
    bool LoadFile(const std::string& filename);
    bool SaveFile(const std::string& filename, Format format = RGB) const;
    void SetPixel(int x, int y, const Color& color);
    void SetPixel(const Vector<int, 2>& v, const Color& color);
    const Color& GetPixel(int x, int y) const;

    const Color& Sample2D(double u, double v) const;
    const Color& Sample2D(const Vec2d& uv) const;

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

    int getIndex(int x, int y) const;
};


inline bool Color::operator==(const Color& c) const
{
    return this->bgra == c.bgra;
}

inline bool Color::operator!=(const Color& c) const
{
    return this->bgra != c.bgra;
}

inline Color Color::operator+(const Color& c) const
{
    return Color(c.bgra + this->bgra);
}

inline Color Color::operator*(double x) const
{
    return Color(x * this->r + 0.5, x * this->g + 0.5, x * this->b + 0.5, a);
}

inline Color operator*(double x, const Color& c)
{
    return Color(x * c.r + 0.5, x * c.g + 0.5, x * c.b + 0.5, c.a);
}

inline void Bitmap::SetPixel(int x, int y, const Color& color)
{
    data[getIndex(x, y)] = color;
}

inline void Bitmap::SetPixel(const Vector<int, 2>& v, const Color& color)
{
    data[getIndex(v.x, v.y)] = color;
}

inline const Color& Bitmap::GetPixel(int x, int y) const
{
    return data[getIndex(x, y)];
}

inline const Color& Bitmap::Sample2D(const Vec2d& uv) const
{
    return Sample2D(uv.u, uv.v);
}

inline int Bitmap::getIndex(int x, int y) const
{
    return x + y * w;
}

#endif