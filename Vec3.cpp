
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

Vec3 Vec3::operator+(const float& v) {
    return Vec3((*this).x_ + v, (*this).y_ + v, (*this).z_ + v);
}

Vec3 Vec3::operator-(const float& v) {
    return Vec3((*this).x_ - v, (*this).y_ -v, (*this).z_ - v);
}

Vec3 Vec3::operator/(const float& v) {
    return Vec3((*this).x_ / v, (*this).y_ / v, (*this).z_ / v);
}

Vec3 Vec3::operator+=(const Vec3& obj) {
    x_ += obj.x_;
    y_ += obj.y_;
    z_ += obj.z_;
    return (*this);
}

Vec3 Vec3::operator-=(const Vec3& obj) {
    x_ -= obj.x_;
    y_ -= obj.y_;
    z_ -= obj.z_;
    return (*this);
}

Vec3 Vec3::operator*=(const Vec3& obj) {
    x_ *= obj.x_;
    y_ *= obj.y_;
    z_ *= obj.z_;
    return (*this);
}

Vec3 Vec3::operator/=(const Vec3& obj) {
    x_ /= obj.x_;
    y_ /= obj.y_;
    z_ /= obj.z_;
    return (*this);
}

Vec3 Vec3::operator+=(const float& v) {
    x_ += v;
    y_ += v;
    z_ += v;
    return (*this);
}

Vec3 Vec3::operator-=(const float& v) {
    x_ -= v;
    y_ -= v;
    z_ -= v;
    return (*this);
}

Vec3 Vec3::operator*=(const float& v) {
    x_ *= v;
    y_ *= v;
    z_ *= v;
    return (*this);
}

Vec3 Vec3::operator/=(const float& v) {
    x_ /= v;
    y_ /= v;
    z_ /= v;
    return (*this);
}

float Vec3::Length() {
    return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

float Vec3::SquaredLength() {
    return x_ * x_ + y_ * y_ + z_ * z_;
}

Vec3 Vec3::Normalized() {
    return (*this) / (*this).Length();
}

