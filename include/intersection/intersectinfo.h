#ifndef _INTERSECTION_H
#define _INTERSECTION_H

#include <utility>

#include "include/core/object.hpp"
#include "include/math/vec3.hpp"
class Object {};
struct IntersectInfo {
  float distance = 10000;
  Vec3 position;
  Vec3 normal;
  std::pair<float, float> uv = {0, 0};
  Object* object;
};
#endif