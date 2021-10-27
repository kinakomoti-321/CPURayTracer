#pragma once

#include "include/core/ray.hpp"
#include "include/core/scene.hpp"
#include "include/sampling/sampler.hpp"
class Integrator {
 public:
  virtual Vec3 integrate(const Ray& ray, const Scene& scene,
                         Sampler& sampler) const = 0;
};