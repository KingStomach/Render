#ifndef _SHADER_H_
#define _SHADER_H_

#include "Bitmap.h"
#include "Martex.h"
#include "Vector.h"

#include <array>
#include <vector>

// to save uniform, varying and attribute variable
struct ShaderContext
{
	std::vector<Vec2d> vec2;
	std::vector<Vec3d> vec3;
	std::vector<Vec4d> vec4;
	std::vector<Mat4d> mat4;
	std::vector<Bitmap> sampler2D;

	ShaderContext();
	//static ShaderContext BaryInterpol(const std::array<ShaderContext, 3>& context, const Vec3d& bary);
	static ShaderContext BaryInterpol(const std::array<ShaderContext, 3>& context, const Vec3d& bary, const Vec3d& Z, double depth);
};

class Shader
{
public:
	Shader(const ShaderContext& context);
	virtual Vec4d Vertex(const ShaderContext& attriContext, ShaderContext& varyContext) const = 0;
	virtual Color Fragment(const ShaderContext& context) const = 0;

protected:
	ShaderContext uniformContext;

};

// uniform variable : mat4[0]: ModelMatrix, mat4[1]: ViewMatrix, mat4[2]: ProjectionMatrix
//                    sampler2D[0]: diffusemap, sampler2D[1]: specularmap
//                    vec3[0]: light position, vec3[1]: light intensity, vec3[2]: camera position
// varying variable : vec3[0]: FragPos, vec3[1]: normal
//                    vec2[0]: uv
// attribute variable : vec3[0]: VertexPosition, vec3[1]: normal
//                      vec2[0]: uv
class PhongShader : public Shader
{
public:
	PhongShader(const ShaderContext& context);
	Vec4d Vertex(const ShaderContext& attriContext, ShaderContext& varyContext) const override;
	Color Fragment(const ShaderContext& varyContext) const override;

};


#endif
