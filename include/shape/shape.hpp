#pragma once
#include "core/aabb.hpp"
#include "core/ray.hpp"
#include "image/texture.hpp"
#include "intersection/intersectinfo.h"
class Shape {
 public:
  virtual bool intersect(const Ray& ray, IntersectInfo& info)const = 0;
  // return AABB covering the Object
  virtual AABB aabb() const = 0;
  // return the area of surface
  virtual float surfaceArea() const = 0;
};