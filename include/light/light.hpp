#pragma once
#include "math/vec3.hpp"
class Light {
 private:
  Vec3 color;
  float LE;

 public:
  Light(const Vec3& color, float LE) : color(color), LE(LE) {}
  Vec3 Le() const { return color * LE; }
};