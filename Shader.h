#ifndef _SHADER_H_
#define _SHADER_H_

#include "Bitmap.h"
#include "Martex.h"
#include "Vector.h"

struct ShaderContext
{
	Vec3d FragPos;
	Vec3d normal;
	Vec2d uv;

	ShaderContext();
};

class Shader
{
public:
	Shader(const Mat4d& ModelMatrix, const Mat4d& ViewMatrix, const Mat4d& ProjectionMatrix, const Bitmap& diffusemap);
	// attribute variable
	virtual Vec3d Vertex(const Vec3d& vertex, const Vec3d& normal, const Vec2d& uv) = 0;
	virtual Color Fragment() = 0;

protected:
	Mat4d ModelMatrix;
	Mat4d ViewMatrix;
	Mat4d ProjectionMatrix;
	Bitmap diffusemap;
	// use member of class shader to save uniform variable
	ShaderContext context; // to save varying variable

};

class PhongShader :public Shader
{
public:
	PhongShader(const Mat4d& ModelMatrix, const Mat4d& ViewMatrix, const Mat4d& ProjectionMatrix, const Bitmap& diffusemap, const Vec3d& light);

	Vec3d Vertex(const Vec3d& vertex, const Vec3d& normal, const Vec2d& uv) override;
	Color Fragment() override;

private:
	Vec3d light;
};

#endif
