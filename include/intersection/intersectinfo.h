#ifndef _INTERSECTION_H
#define _INTERSECTION_H

#include <utility>

#include "core/object.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"

class Object;
struct IntersectInfo {
  float distance = 10000;
  Vec3 position;
  Vec3 normal;
  Vec2 uv; 
  Object* object;
};
#endif