#ifndef _RAY_H
#define _RAY_H

#include "math/vec3.hpp"
struct Ray {
  Vec3 origin;
  Vec3 direction;
  float Min = 0.001;
  float Max = 100000;

  Ray(const Vec3& origin, const Vec3& direction)
    : origin(origin), direction(direction) {}
  Ray(const Vec3& origin, const Vec3& direction, float Min, float Max)
    : origin(origin), direction(direction), Min(Min), Max(Max) {}

  Vec3 post(float t) const { return origin + t * direction; }
};
#endif