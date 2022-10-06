#include "Shader.h"

ShaderContext::ShaderContext() {}

ShaderContext ShaderContext::BaryInterpol(const std::array<ShaderContext, 3>& context, const Vec3d& bary, const Vec3d& Z, double depth)
{
	ShaderContext c;
	Vec2d baryvec2;
	Vec3d baryvec3;
	Vec4d baryvec4;
	Mat4d barymat4;
	for (size_t i = 0; i < context[0].vec2.size(); i++)
	{
		//baryvec2 = bary.x * context[0].vec2[i] + bary.y * context[1].vec2[i] + bary.z * context[2].vec2[i];
		baryvec2 = (bary.x * context[0].vec2[i] / Z.x + bary.y * context[1].vec2[i] / Z.y + bary.z * context[2].vec2[i] / Z.z) * depth;
		c.vec2.emplace_back(baryvec2);
	}
	for (size_t i = 0; i < context[0].vec3.size(); i++)
	{
		//baryvec3 = bary.x * context[0].vec3[i] + bary.y * context[1].vec3[i] + bary.z * context[2].vec3[i];
		baryvec3 = (bary.x * context[0].vec3[i] / Z.x + bary.y * context[1].vec3[i] / Z.y + bary.z * context[2].vec3[i] / Z.z) * depth;
		c.vec3.emplace_back(baryvec3);
	}
	for (size_t i = 0; i < context[0].vec4.size(); i++)
	{
		//baryvec4 = bary.x * context[0].vec4[i] + bary.y * context[1].vec4[i] + bary.z * context[2].vec4[i];
		baryvec4 = (bary.x * context[0].vec4[i] / Z.x + bary.y * context[1].vec4[i] / Z.y + bary.z * context[2].vec4[i] / Z.z) * depth;
		c.vec4.emplace_back(baryvec4);
	}
	for (size_t i = 0; i < context[0].mat4.size(); i++)
	{
		//barymat4 = bary.x * context[0].mat4[i] + bary.y * context[1].mat4[i] + bary.z * context[2].mat4[i];
		barymat4 = (bary.x * context[0].mat4[i] / Z.x + bary.y * context[1].mat4[i] / Z.y + bary.z * context[2].mat4[i] / Z.z) * depth;
		c.mat4.emplace_back(barymat4);
	}
	for (const Bitmap& map : context[0].sampler2D)
	{
		c.sampler2D.emplace_back(map);
	}
	return c;
}

Shader::Shader(const ShaderContext& context) :uniformContext(context) {}

PhongShader::PhongShader(const ShaderContext& context) : Shader(context) {}

Vec4d PhongShader::Vertex(const ShaderContext& attriContext, ShaderContext& varyContext) const
{
	varyContext.vec3.emplace_back(convert<double, 4, 3>(uniformContext.mat4[0] * convert<double, 3, 4>(attriContext.vec3[0], 1.0)));
	varyContext.vec3.emplace_back(convert<double, 4, 3>(uniformContext.mat4[0] * convert<double, 3, 4>(attriContext.vec3[1], 0.0)).normalize());
	varyContext.vec2.emplace_back(attriContext.vec2[0]);

	return uniformContext.mat4[2] * uniformContext.mat4[1] * uniformContext.mat4[0] * convert<double, 3, 4>(attriContext.vec3[0], 1.0);
}

Color PhongShader::Fragment(const ShaderContext& varyContext) const
{
	
	Vec3d light2frag = uniformContext.vec3[0] - varyContext.vec3[0];
	Vec3d lightDir = light2frag / light2frag.length();
	double diff = std::max(varyContext.vec3[1] * lightDir, 0.0);
	Vec3d light_atten_coff = uniformContext.vec3[1] / (light2frag * light2frag);
	Color diffuseSample = uniformContext.sampler2D[0].Sample2D(varyContext.vec2[0]);
	Vec3d diffuse(light_atten_coff.x * diff * (double)diffuseSample.r, light_atten_coff.y * diff * (double)diffuseSample.g,
		light_atten_coff.y * diff * (double)diffuseSample.b);

	Vec3d viewDir = (uniformContext.vec3[2] - varyContext.vec3[0]).normalize();
	Vec3d halfDir = (lightDir + viewDir).normalize();
	double spec = std::pow(std::max(varyContext.vec3[1] * halfDir, 0.0), 1.0);
	Color specularSample = uniformContext.sampler2D[1].Sample2D(varyContext.vec2[0]);
	Vec3d specular(light_atten_coff.x * diff * (double)specularSample.r, light_atten_coff.y * diff * (double)specularSample.g,
		light_atten_coff.y * diff * (double)specularSample.b);
	return Color(diffuse.x + specular.x > 255.0 ? 255 : diffuse.x + specular.x, diffuse.y + specular.y > 255.0 ? 255 : diffuse.y + specular.y,
		diffuse.z + specular.z > 255.0 ? 255 : diffuse.z + specular.z);

	/*
	Vec3d lightDir = (uniformContext.vec3[0] - varyContext.vec3[0]).normalize();
	Vec3d viewDir = (uniformContext.vec3[2] - varyContext.vec3[0]).normalize();
	Vec3d halfDir = (lightDir + viewDir).normalize();

	Color diffuse = uniformContext.sampler2D[0].Sample2D(varyContext.vec2[0]) * std::max(varyContext.vec3[1] * lightDir, 0.0);
	Color specular = uniformContext.sampler2D[1].Sample2D(varyContext.vec2[0]) * std::max(varyContext.vec3[1] * halfDir, 0.0);
	return diffuse + 0.6 * specular;*/
}