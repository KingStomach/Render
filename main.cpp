#include <array>
#include <iostream>
#include "Bitmap.h"
#include "Geometry.h"
#include "Martex.h"
#include "Model.h"
#include "Shader.h"
#include "Scene.h"

struct Camera
{
    Vec3d position;
    Vec3d up;
    Vec3d lookat;
};

struct Light
{
    Vec3d position;
    Vec3d color;
    double intensity;
};

int main(void)
{
    const int width = 1024;
    const int height = 768;
    Model model("res/diablo3_pose.obj");
    //Model model("res/african_head.obj");
    Scene scene(width, height, model);
    Light light{ {0,0,30}, {255,255,255}, 5 };
    Camera camera{ {0,0,2.5},{0,1,0},{0,0,0} };
    Mat4d ModelMatrix = identity<double, 4>();
    Mat4d ViewMatrix = LookAt(camera.position, camera.lookat, camera.up);
    //Mat4d ViewMatrix = identity<double, 4>();
    Vec4d a = ViewMatrix * ModelMatrix * Vec4d(1, 1, 1, 1);
    Mat4d ProjectionMatrix = Perspective(45.0, (double)width / height, 0.1, 100.0);
    //Mat4d ProjectionMatrix = identity<double, 4>();
    
    ShaderContext uniformContext;
    uniformContext.mat4.emplace_back(ModelMatrix);
    uniformContext.mat4.emplace_back(ViewMatrix);
    uniformContext.mat4.emplace_back(ProjectionMatrix);
    uniformContext.sampler2D.emplace_back(model.diffuse());
    uniformContext.sampler2D.emplace_back(model.specular());
    uniformContext.vec3.emplace_back(light.position);
    uniformContext.vec3.emplace_back(light.color * light.intensity);
    uniformContext.vec3.emplace_back(camera.position);

    PhongShader shader(uniformContext);
    Bitmap map = scene.render(shader);
    map.SaveFile("out.bmp", RGBA);
    return 0;
}