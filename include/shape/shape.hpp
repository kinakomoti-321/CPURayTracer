#pragma once
#include "include/core/aabb.hpp"
#include "include/core/ray.hpp"
#include "include/image/texture.hpp"
#include "include/intersection/intersectinfo.h"
class Shape {
 public:
  virtual bool intersect(const Ray& ray, IntersectInfo& info){};
  // return AABB covering the Object
  virtual AABB aabb() const = 0;
  // return the area of surface
  virtual float surfaceArea() const = 0;
};