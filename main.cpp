#include <array>
#include <iostream>
#include "Bitmap.h"
#include "Geometry.h"
#include "Model.h"
#include "Scene.h"

int main(void)
{
    Scene scene(1024, 768, "res/diablo3_pose.obj");
    //scene.setDrawType(DrawType::Point);
    scene.setDrawType(DrawType::WireFrame);
    Vec3d LightPosition(0, 0, -5);
    Vec3d LightDirection(0, 0, 1);
    Light light(LightPosition, LightDirection);
    scene.setLight(light);
    Bitmap map = scene.render();
    map.SaveFile("out.bmp", RGBA);
    return 0;
}