#include "Scene.h"

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
    case DrawType::Triangle:
        return renderTriangle(shader);
    default:
        return Bitmap();
    }
}

Bitmap Scene::renderPoint(Shader& shader)
{   
    Bitmap map(this->w, this->h);
    Vec4d vertex;
    ShaderContext varyContext;
    Mat4d viewPort = Scale(Vec3d(w / 2.0, h / 2.0, 1.0)) * Translate(Vec3d(1, 1, 1));
    for (int i = 0; i < model.nverts(); i++)
    {
        ShaderContext attriContext;
        attriContext.vec3.emplace_back(model.vert(i));
        attriContext.vec3.emplace_back(Vec3d(0, 0, 0));
        attriContext.vec2.emplace_back(Vec2d(0, 0));
        vertex = shader.Vertex(attriContext, varyContext);
        vertex /= vertex.w;
        map.SetPixel(Vec2i(vertex.x, vertex.y), White);
    }
    return map;
}

Bitmap Scene::renderTriangle(Shader& shader)
{

    Bitmap map(this->w, this->h);
    for (size_t i = 0; i < model.nfaces(); i++)
    {
        std::array<ShaderContext, 3> context;
        std::array<Vec3d, 3> triangle;
        Vec4d vertex;
        Mat4d viewPort = Scale(Vec3d(w / 2.0, h / 2.0, 1.0)) * Translate(Vec3d(1, 1, 1));
        bool isDraw = true;
        for (size_t j = 0; j < 3; j++)
        {
            ShaderContext attriContext;
            attriContext.vec3.emplace_back(model.vert(i, j));
            attriContext.vec3.emplace_back(model.normal(i, j));
            attriContext.vec2.emplace_back(model.uv(i, j));
            vertex = shader.Vertex(attriContext, context[j]);
            vertex /= vertex.w;
            // Frustum Culling
            if (vertex.x < -1.0 || vertex.x >= 1.0 || vertex.y < -1.0 || vertex.y >= 1.0)
            {
                isDraw = false;
                break;
            }
            triangle[j] = convert<double, 4, 3>(viewPort * vertex);
        }
        // Face Culling
        if (isDraw)
        {
            Vec3d normal = (triangle[2] - triangle[0]) ^ (triangle[1] - triangle[0]);
            if (normal.z < 0)
                isDraw = false;
        }

        if (!isDraw)
            continue;

        if (drawtype == DrawType::Triangle)
        {
            drawTriangle(map, shader, triangle, context);
        }
        else if (drawtype == DrawType::WireFrame)
        {
            drawLine(map, triangle[0], triangle[1], White);
            drawLine(map, triangle[0], triangle[2], White);
            drawLine(map, triangle[1], triangle[2], White);
        }
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

void Scene::drawTriangle(Bitmap& map, Shader& shader, const std::array<Vec3d, 3>& Triangle, const std::array<ShaderContext, 3>& context)
{
    int xmin = INT_MAX, ymin = INT_MAX;
    int xmax = INT_MIN, ymax = INT_MIN;
    for (auto&& point : Triangle)
    {
        xmin = std::min(xmin, (int)point.x);
        xmax = std::max(xmax, (int)point.x + 1);
        ymin = std::min(ymin, (int)point.y);
        ymax = std::max(ymax, (int)point.y + 1);
    }

    double depth = 0.0;
    ShaderContext baryContext;
    Color c;
    for (int i = xmin; i <= xmax; i++)
    {
        for (int j = ymin; j <= ymax; j++)
        {
            if (isIntersect(Triangle, Vec3d(i + 0.5, j + 0.5, 0)))
            {
                Vec3d bary = BarycentricInterpol2D<Vec3d, double>(Triangle, Vec3d(i + 0.5, j + 0.5, 0));
                if (bary.x < 0.0 || bary.y < 0.0 || bary.z < 0.0)
                    continue;
                //todo Í¸ÊÓ½ÃÕý
                
                //depth = bary * Vec3d(Triangle[0].z, Triangle[1].z, Triangle[2].z);
                depth = 1.0 / (bary.x / Triangle[0].z + bary.y / Triangle[1].z + bary.z / Triangle[2].z);
                if (zbuffer[getIndex(i, j)] < depth)
                {
                    zbuffer[getIndex(i, j)] = depth;
                    baryContext = ShaderContext::BaryInterpol(context, bary, Vec3d(Triangle[0].z, Triangle[1].z, Triangle[2].z), depth);
                    c = shader.Fragment(baryContext);
                    map.SetPixel(i, j, c);
                }
            }
        }
    }
}