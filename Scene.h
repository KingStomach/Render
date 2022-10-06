#ifndef _SCENE_H_
#define _SCENE_H_

#include <array>
#include <vector>

#include "Shader.h"
#include "Model.h"

struct Light
{
	Vec3d position;
	Vec3d direction;

	Light();
	Light(const Vec3d& position, const Vec3d& direction);
};

enum class DrawType { Point, WireFrame, Triangle };

class Scene
{
public:
	Scene(int width, int height, const Model& model);

	void setDrawType(DrawType drawtype);
	void setWidth(int width);
	void setHeight(int height);
	Bitmap render(Shader& shader);

private:
	Model model;
	int w;
	int h;
	DrawType drawtype;
	std::vector<double> zbuffer;

	Bitmap renderPoint(Shader& shader);
	Bitmap renderWireFrame(Shader& shader);
	Bitmap renderTriangle(Shader& shader);
	void drawLine(Bitmap& map, const Vec3d &p1, const Vec3d& p2, const Color& color);
	void drawTriangle(Bitmap& map, const std::array<Vec3d, 3>& TriangleWorld, const std::array<Vec3d, 3>& TriangleScreen,
		const std::array<Color, 3>& color);

	inline int getIndex(int x, int y) const { return x * h + y; }
};
#endif