#ifndef _SCENE_H_
#define _SCENE_H_

#include <array>
#include <vector>
#include "Model.h"

struct Light
{
	Vec3d position;
	Vec3d direction;

	Light() {}
	Light(const Vec3d& position, const Vec3d& direction) : position(position), direction(direction) {}
};

enum class DrawType { Point, WireFrame, Triangle };

class Scene
{
public:
	Scene(int width, int height, const std::string& filename): w(width), h(height), model(filename), drawtype(DrawType::Triangle) {}

	inline void setLight(const Light& light) { this->light = light; }
	inline void setDrawType(DrawType drawtype) { this->drawtype = drawtype; }
	inline void setWidth(int width) { this->w = width; }
	inline void setHeight(int height) { this->h = height; }
	Bitmap render();

private:
	Model model;
	Light light;
	int w;
	int h;
	DrawType drawtype;

	void drawLine(Bitmap& map, const Vec3d &p1, const Vec3d& p2, const Color& color);
	inline Vec3d worldtoscreen(const Vec3d& p) { return Vec3d((p.x + 1.0) / 2.0 * w, (p.y + 1.0) / 2.0 * h, p.z); }
	void drawTriangle(Bitmap& map, const std::array<Vec3d, 3>& Triangle, const Color& color);
	inline void drawTrianglePoint(Bitmap& map, const std::array<Vec3d, 3>& Triangle, const Color& color)
	{ for (auto&& point : Triangle) map.SetPixel((int)point.x, (int)point.y, color); }
	inline void drawTriangleWireFrame(Bitmap& map, const std::array<Vec3d, 3>& Triangle, const Color& color)
	{
		drawLine(map, Triangle[0], Triangle[1], color);
		drawLine(map, Triangle[0], Triangle[2], color);
		drawLine(map, Triangle[1], Triangle[2], color);
	}
	void drawTriangleTriangle(Bitmap& map, const std::array<Vec3d, 3>& Triangle, const Color& color);
};
#endif