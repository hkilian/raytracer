
#include "Vec3.hpp"

Vec3 Vec3::operator+(const Vec3& obj) {
    return Vec3((*this).x_ + obj.x_, (*this).y_ + obj.y_, (*this).z_ + obj.z_);
}

Vec3 Vec3::operator-(const Vec3& obj) {
    return Vec3((*this).x_ - obj.x_, (*this).y_ - obj.y_, (*this).z_ - obj.z_);
}

Vec3 Vec3::operator*(const Vec3& obj) {
    return Vec3((*this).x_ * obj.x_, (*this).y_ * obj.y_, (*this).z_ * obj.z_);
}

Vec3 Vec3::operator/(const Vec3& obj) {
    return Vec3((*this).x_ / obj.x_, (*this).y_ / obj.y_, (*this).z_ / obj.z_);
}
