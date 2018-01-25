
#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Cameratracer
#include "Vec3.hpp"

class Camera {
public:
    
    // Constuctor/destructor
    Camera(int screenWidth, int screenHeight) {
    
        ratio = float(screenWidth) / float(screenHeight);
        origin = Vec3(0,0,0);
        lowerLeft = Vec3(-ratio, -1, -1);
        horizontal = Vec3(ratio * 2, 0, 0);
        vertical = Vec3(0, 2, 0);
    
    }
    ~Camera() {};
    
    int screenWidth;
    int screenHeight;
    float ratio;
    
    Vec3 origin = Vec3(0,0,0);
    Vec3 lowerLeft = Vec3(0, -1, -1);
    Vec3 horizontal = Vec3(2, 0, 0);
    Vec3 vertical = Vec3(0, 2, 0);
    
private:
    

    
};
