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

int main(void)
{
    const int width = 800;
    const int height = 800;
    //Model model("res/diablo3_pose.obj");
    Model model("res/african_head.obj");
    Scene scene(width, height, model);
    //scene.setDrawType(DrawType::Point);
    //scene.setDrawType(DrawType::WireFrame);
    Vec3d light(0, 0, 15);
    Camera camera{ {0,0,15},{0,1,0},{0,0,0} };
    //Mat4d ModelMatrix = Scale(Vec3d(10.0, 10.0, 10.0));
    Mat4d ModelMatrix = identity<double, 4>();
    //Mat4d ModelMatrix = Scale(Vec3d(width / 2.0, height / 2.0, 1.0)) * Translate(Vec3d(1, 1, 1));
    //Mat4d ViewMatrix = LookAt(camera.position, camera.lookat, camera.up);
    Mat4d ViewMatrix = identity<double, 4>();
    //Mat4d ViewMatrix = identity<double, 4>();
    //Mat4d ProjectionMatrix = Perspective(45.0, (double)width / height, 0.1, 10.0);
    Mat4d ProjectionMatrix = identity<double, 4>();
    
    PhongShader shader(ModelMatrix, ViewMatrix, ProjectionMatrix, model.diffuse(), light);
    Bitmap map = scene.render(shader);
    map.SaveFile("out.bmp", RGBA);
    return 0;
}