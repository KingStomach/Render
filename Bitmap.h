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
        struct { std::uint8_t r, g, b, a; };
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
    template <typename T>
    void SetPixel(const Vector<T, 2>& v, const Color& color) { data[getIndex((int)v.x, (int)v.y)] = color; }
    const Color& GetPixel(int x, int y) const;

    const Color& Sample2D(double u, double v) const;
    template <typename T>
    const Color& Sample2D(const Vector<T, 2>& v) const { return data[getIndex((int)v.x, (int)v.y)]; }

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
#endif