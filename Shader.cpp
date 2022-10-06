#include "Shader.h"

ShaderContext::ShaderContext() {}

Shader::Shader(const Mat4d& ModelMatrix, const Mat4d& ViewMatrix, const Mat4d& ProjectionMatrix, const Bitmap& diffusemap)
	: ModelMatrix(ModelMatrix), ViewMatrix(ViewMatrix), ProjectionMatrix(ProjectionMatrix), diffusemap(diffusemap) {}

PhongShader::PhongShader(const Mat4d& ModelMatrix, const Mat4d& ViewMatrix, const Mat4d& ProjectionMatrix, const Bitmap& diffusemap, const Vec3d& light)
	: Shader(ModelMatrix, ViewMatrix, ProjectionMatrix, diffusemap), light(light) {}

Vec3d PhongShader::Vertex(const Vec3d& vertex, const Vec3d& normal, const Vec2d& uv)
{
	context.FragPos = convert<double, 4, 3>(ModelMatrix * convert<double, 3, 4>(vertex, 1.0));
	//context.normal = convert<double, 4, 3>(ModelMatrix * convert<double, 3, 4>(vertex, 0.0)).normalize();
	context.normal = normal;
	context.uv = uv;

	return convert<double, 4, 3>(ProjectionMatrix * ViewMatrix * ModelMatrix * convert<double, 3, 4>(vertex, 1.0));
}

Color PhongShader::Fragment()
{
	double intensity = std::max(context.normal.normalize() * (light - context.FragPos).normalize(), 0.0);
	return White * intensity;
}