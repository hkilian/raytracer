
#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// Raytracer
#include "Vec3.hpp"

class Ray {
public:
    
    // Constuctor/destructor
    Ray(Vec3 origin,Vec3 direction) { origin_ = origin; direction_ = direction;}
    ~Ray() {};
    
    // Getters
    Vec3 Origin() const { return origin_; }
    Vec3 Direction() const { return direction_; }
    
    // Functions
    Vec3 PointAtParameter(float t) const {
        return origin_ + (t * direction_);
    }
    
private:
    
    Vec3 origin_;
    Vec3 direction_;
    
};
