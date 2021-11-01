#pragma once

#include "core/ray.hpp"
#include "core/scene.hpp"
#include "sampling/sampler.hpp"
class Integrator {
 public:
  virtual Vec3 integrate(const Ray& ray, const Scene& scene,
                         std::shared_ptr<Sampler>& sampler) const = 0;
};