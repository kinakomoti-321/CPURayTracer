#pragma once
#include <vector>

#include "include/core/ray.hpp"
#include "include/intersection/intersectinfo.h"
#include "include/shape/shape.hpp"

class Scene {
 private:
 public:
  Scene();

  bool intersect(const Ray& ray, const IntersectInfo& info) const {}
  Vec3 sky() const;
};