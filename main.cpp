#include <array>
#include <iostream>
#include "Bitmap.h"
#include "Geometry.h"
#include "Model.h"

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

void drawTritangle(Bitmap& map, const Vec2i& p1, const Vec2i& p2, const Vec2i& p3, const Color& color)
{
    drawLine(map, p1, p2, color);
    drawLine(map, p1, p3, color);
    drawLine(map, p2, p3, color);
}

Vec2i worldtoscreen(const Vec3d& p)
{
    Vec2i t;
    t.x = (p.x + 1.0) / 2.0 * 1024.0;
    t.y = (p.y + 1.0) / 2.0 * 768.0;
    return t;
}

int main(void)
{
    Bitmap map(1024, 768);
    //Vec2i p1(1, 1), p2(90, 10), p3(10, 90);
    //drawLine(map, p1, p2, Red);
    //drawLine(map, p1, p3, Blue);
    Model model("res/diablo3_pose.obj");
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::array<Vec2i, 3> points;
        for (int j = 0; j < 3; j++)
        {
            points[j] = worldtoscreen(model.vert(i, j));
        }
        drawTritangle(map, points[0], points[1], points[2], White);
    }
    map.SaveFile("out.bmp", RGBA);
    return 0;
}