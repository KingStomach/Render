#ifndef _SCENE_H_
#define _SCENE_H_

#include <array>
#include <vector>

#include "Shader.h"
#include "Model.h"

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
	Bitmap renderTriangle(Shader& shader);
	void drawLine(Bitmap& map, const Vec3d &p1, const Vec3d& p2, const Color& color);
	void drawTriangle(Bitmap& map, Shader& shader, const std::array<Vec3d, 3>& Triangle, const std::array<ShaderContext, 3>& context);

	inline int getIndex(int x, int y) const { return x + y * w; }
};
#endif