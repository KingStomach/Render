#include "Scene.h"

Light::Light() {}

Light::Light(const Vec3d& position, const Vec3d& direction) : position(position), direction(direction) {}

Scene::Scene(int width, int height, const Model& model) : w(width), h(height), model(model),
    drawtype(DrawType::Triangle), zbuffer(width* height, -DBL_MAX) {}

void Scene::setDrawType(DrawType drawtype)
{
    this->drawtype = drawtype;
}

void Scene::setWidth(int width)
{
    this->w = width;
}

void Scene::setHeight(int height)
{
    this->h = height;
}

Bitmap Scene::render(Shader& shader)
{
    switch (drawtype)
    {
    case DrawType::Point:
        return renderPoint(shader);
    case DrawType::WireFrame:
        return renderWireFrame(shader);
    case DrawType::Triangle:
        return renderTriangle(shader);
    default:
        return Bitmap();
    }
}

Bitmap Scene::renderPoint(Shader& shader)
{   
    Bitmap map(this->w, this->h);
    Vec3d bcScreen;
    for (int i = 0; i < model.nfaces(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            bcScreen = shader.Vertex(model.vert(i, j), model.normal(i, j), model.uv(i, j));
            map.SetPixel(convert<double, 3, 2>(bcScreen), White);
        }
    }
    return map;
}

Bitmap Scene::renderWireFrame(Shader& shader)
{
    Bitmap map(this->w, this->h);
    for (int i = 0; i < model.nfaces(); i++)
    {
        //std::array<Vec3d, 3> TriangleWorld;
        std::array<Vec3d, 3> TriangleScreen;
        Color c;
        for (int j = 0; j < 3; j++)
        {
            //TriangleWorld[j] = model.vert(i, j);
            TriangleScreen[j] = shader.Vertex(model.vert(i, j), model.normal(i), model.uv(i, j));
            c = shader.Fragment();
        }
        if (c != Black)
        {
            drawLine(map, TriangleScreen[0], TriangleScreen[1], White);
            drawLine(map, TriangleScreen[0], TriangleScreen[2], White);
            drawLine(map, TriangleScreen[1], TriangleScreen[2], White);
        }
    }
    return map;
}

Bitmap Scene::renderTriangle(Shader& shader)
{
    Bitmap map(this->w, this->h);
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::array<Vec3d, 3> TriangleWorld;
        std::array<Vec3d, 3> TriangleScreen;
        std::array<Color, 3> TriangleColor;
        Mat4d viewPort = Scale(Vec3d(w / 2.0, h / 2.0, 1.0)) * Translate(Vec3d(1, 1, 1));
        for (int j = 0; j < 3; j++)
        {
            TriangleWorld[j] = model.vert(i, j);
            TriangleScreen[j] = shader.Vertex(model.vert(i, j), model.normal(i, j), model.uv(i, j));
            TriangleScreen[j] = convert<double, 4, 3>(viewPort * convert<double, 3, 4>(TriangleScreen[j], 1.0));
            TriangleColor[j] = shader.Fragment();
        }
        drawTriangle(map, TriangleWorld, TriangleScreen, TriangleColor);
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

void Scene::drawTriangle(Bitmap& map, const std::array<Vec3d, 3>& TriangleWorld, const std::array<Vec3d, 3>& TriangleScreen, 
    const std::array<Color, 3>& color)
{
    if (color[0] == Black && color[1] == Black && color[2] == Black)
        return;
    
    int xmin = INT_MAX, ymin = INT_MAX;
    int xmax = INT_MIN, ymax = INT_MIN;
    for (auto&& point : TriangleScreen)
    {
        xmin = std::min(xmin, (int)point.x);
        xmax = std::max(xmax, (int)point.x + 1);
        ymin = std::min(ymin, (int)point.y);
        ymax = std::max(ymax, (int)point.y + 1);
    }

    for (int i = xmin; i <= xmax; i++)
    {
        for (int j = ymin; j <= ymax; j++)
        {
            if (isIntersect(TriangleScreen, Vec3d(i, j, 0)))
            {
                Vec3d bary = BarycentricInterpol2D<Vec3d, double>(TriangleScreen, Vec3d(i, j, 0));
                //Vec3d bary = barycentric(TriangleScreen[0], TriangleScreen[1], TriangleScreen[2], Vec3d(i, j, 0));
                if (bary.x < 0.0 || bary.y < 0.0 || bary.z < 0.0)
                    continue;
                //todo Í¸ÊÓ½ÃÕý
                
                double t = ((TriangleWorld[1] - TriangleWorld[0]) ^ (TriangleWorld[2] - TriangleWorld[0])).normalize() * Vec3d(0, 0, 1);
                double depth = bary.x * TriangleWorld[0].z + bary.y * TriangleWorld[1].z + bary.z * TriangleWorld[2].z;
                if (zbuffer[getIndex(i, j)] < depth)
                {
                    zbuffer[getIndex(i, j)] = depth;
                    Color c = bary.x * color[0] + bary.y * color[1] + bary.z * color[2];
                    map.SetPixel(i, j, c);
                }
            }
        }
    }
}
