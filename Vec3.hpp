
#pragma once

// System
#include <iostream>
#include <string>
#include <stdio.h>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Vec3 {
    
public:
    
    // Constuctor/destructor
    Vec3() { x_ = 0; y_ = 0; z_ = 0;}
    Vec3(float v) { x_ = v; y_ = v; z_ = v;}
    Vec3(float x, float y, float z) { x_ = x; y_ = y; z_ = z;}
    ~Vec3() {};
    
    // Arithmetic operators
    Vec3 operator+(const Vec3& obj);
    Vec3 operator-(const Vec3& obj);
    Vec3 operator*(const Vec3& obj);
    Vec3 operator/(const Vec3& obj);
    Vec3 operator+(const float& v);
    Vec3 operator-(const float& v);
    Vec3 operator/(const float& v);
    
    // Compound assignment
    Vec3 operator+=(const Vec3& obj);
    Vec3 operator-=(const Vec3& obj);
    Vec3 operator*=(const Vec3& obj);
    Vec3 operator/=(const Vec3& obj);
    Vec3 operator+=(const float& v);
    Vec3 operator-=(const float& v);
    Vec3 operator*=(const float& v);
    Vec3 operator/=(const float& v);
    
    // Functions
    float Length();
    Vec3 Normalized();
    float SquaredLength();
    
    // Values
    float x_;
    float y_;
    float z_;
    
private:
    
};

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.x_ * v2.x_, v1.y_ * v2.y_, v1.z_ * v2.z_);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.x_ / v2.x_, v1.y_ / v2.y_, v1.z_ / v2.z_);
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.x_ + v2.x_, v1.y_ + v2.y_, v1.z_ + v2.z_);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.x_ - v2.x_, v1.y_ - v2.y_, v1.z_ - v2.z_);
}

inline Vec3 operator*(const Vec3 &v1, float t) {
    return Vec3(v1.x_ * t, v1.y_ * t, v1.z_ * t);
}

inline Vec3 operator*(float t, const Vec3 &v1) {
    return Vec3(v1.x_ * t, v1.y_ * t, v1.z_ * t);
}

inline Vec3 operator/(const Vec3 &v1, float t) {
    return Vec3(v1.x_ / t, v1.y_ / t, v1.z_ / t);
}

inline Vec3 operator/(float t, const Vec3 &v1) {
    return Vec3(v1.x_ / t, v1.y_ / t, v1.z_ / t);
}

inline Vec3 operator+(const Vec3 &v1, float t) {
    return Vec3(v1.x_ + t, v1.y_ + t, v1.z_ + t);
}

inline Vec3 operator+(float t, const Vec3 &v1) {
    return Vec3(v1.x_ + t, v1.y_ + t, v1.z_ + t);
}

inline Vec3 operator-(const Vec3 &v1, float t) {
    return Vec3(v1.x_ - t, v1.y_ - t, v1.z_ - t);
}

inline Vec3 operator-(float t, const Vec3 &v1) {
    return Vec3(v1.x_ - t, v1.y_ - t, v1.z_ - t);
}

inline float Dot(const Vec3 v1, const Vec3 v2) {
    return v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
}


