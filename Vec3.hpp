
#pragma once

// System
#include <iostream>
#include <string>
#include <stdio.h>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Vec3
{
public:
    
    // Constuctor/destructor
    Vec3(float x, float y, float z) { x_ = x; y_ = y; z_ = z;}
    ~Vec3() {};
    
    // Arithmetic operators
    Vec3 operator+(const Vec3& obj);
    Vec3 operator-(const Vec3& obj);
    Vec3 operator*(const Vec3& obj);
    Vec3 operator/(const Vec3& obj);
    
    // Values
    float x_;
    float y_;
    float z_;
    
private:
    
};
