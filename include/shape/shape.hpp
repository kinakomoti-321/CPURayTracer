#pragma once
#include "core/aabb.hpp"
#include "core/ray.hpp"
#include "image/texture.hpp"
#include "intersection/intersectinfo.h"
#include <memory>
class Shape {
 public:
  virtual bool intersect(const Ray& ray, IntersectInfo& info)const = 0;
  // return AABB covering the Object
  virtual AABB aabb()const = 0;
  // return the area of surface
  virtual float surfaceArea()const  = 0;

  virtual Vec3 areaSampling(const std::shared_ptr<Sampler>& sampler,IntersectInfo& info)const = 0;
};