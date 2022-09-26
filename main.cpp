#include <iostream>
#include "Bitmap.h"
#include "Geometry.h"


int main(void)
{
    Bitmap map(100, 100);
    map.SaveFile("out.bmp");
    return 0;
}