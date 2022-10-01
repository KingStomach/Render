#include "Scene.h"

Bitmap Scene::render()
{
    Bitmap map(this->w, this->h);
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::array<Vec3d, 3> TriangleWorld;
        std::array<Vec3d, 3> TriangleScreen;
        for (int j = 0; j < 3; j++)
        {
            TriangleWorld[j] = model.vert(i, j);
            TriangleScreen[j] = worldtoscreen(model.vert(i, j));
        }
        double intensity = getTriangleNormal(TriangleWorld).dot(light.direction);
        if (intensity > 0.0)
            drawTriangle(map, TriangleScreen, White * intensity);
    }
    return map;
}

void Scene::drawLine(Bitmap& map, const Vec3d& p1, const Vec3d& p2, const Color& color)
{
    int x0 = p1.x, x1 = p2.x;
    int y0 = p1.y, y1 = p2.y;
    bool steep = false;
    if (std::abs(y1 - y0) > std::abs(x1 - x0))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x = x0; x < x1; x++)
    {
        float t = static_cast<float>(x - x0) / (x1 - x0);
        int y = y0 + (y1 - y0) * t;
        if (steep)
            map.SetPixel(y, x, color);
        else
            map.SetPixel(x, y, color);
    }
}

void Scene::drawTriangle(Bitmap& map, const std::array<Vec3d, 3>& Triangle, const Color& color)
{
    switch (drawtype)
    {
    case DrawType::Point:
        drawTrianglePoint(map, Triangle, color);
        break;
    case DrawType::WireFrame:
        drawTriangleWireFrame(map, Triangle, color);
        break;
    case DrawType::Triangle:
        drawTriangleTriangle(map, Triangle, color);
        break;
    default:
        break;
    }
}

void Scene::drawTriangleTriangle(Bitmap& map, const std::array<Vec3d, 3>& Triangle, const Color& color)
{
    int xmin = INT_MAX, ymin = INT_MAX;
    int xmax = INT_MIN, ymax = INT_MIN;
    for (auto&& point : Triangle)
    {
        xmin = std::min(xmin, (int)point.x);
        xmax = std::max(xmax, (int)point.x);
        ymin = std::min(ymin, (int)point.y);
        ymax = std::max(ymax, (int)point.y);
    }

    for (int i = xmin; i <= xmax; i++)
    {
        for (int j = ymin; j <= ymax; j++)
        {
            if (isIntersect(Triangle, Vec3d(i, j, 0)))
            {
                map.SetPixel(i, j, color);
            }
        }
    }
}
