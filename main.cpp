#include <iostream>
#include "Bitmap.h"
#include "Geometry.h"

void drawLine(Bitmap& map, Vec2i p1, Vec2i p2, const Color& color)
{
    bool steep = false;
    if (std::abs(p2.y - p1.y) > std::abs(p2.x - p1.x))
    {
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
        steep = true;
    }
    if (p1.x > p2.x)
    {
        std::swap(p1.x, p2.x);
        std::swap(p1.y, p2.y);
    }

    for (int x = p1.x; x < p2.x; x++)
    {
        float t = static_cast<float>(x - p1.x) / (p2.x - p1.x);
        int y = p1.y + (p2.y - p1.y) * t;
        if (steep)
            map.SetPixel(y, x, color);
        else
            map.SetPixel(x, y, color);
    }
}


int main(void)
{
    Bitmap map(100, 100);
    Vec2i p1(1, 1), p2(90, 10), p3(10, 90);
    drawLine(map, p1, p2, Red);
    drawLine(map, p1, p3, Blue);
    map.SaveFile("out.bmp", RGBA);
    return 0;
}