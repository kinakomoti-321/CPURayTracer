#pragma once
#include <algorithm>
#include <cmath>

#include "core/constant.hpp"
#include "math/vec3.hpp"
#include "sampling/sampler.hpp"
class BSDF {
 public:
  virtual Vec3 samplingBSDF(const Vec3& wo, Vec3& wi, float& pdf,
                            std::shared_ptr<Sampler>& sampler) const = 0;
};

inline Vec3 cosineSampling(float u, float v, float& pdf) {
  const float theta =
      std::acos(std::clamp(1.0f - 2.0f * u, -1.0f, 1.0f)) / 2.0f;
  const float phi = 2.0f * PI * v;
  pdf = cos(theta) / PI;
  return Vec3(std::cos(phi) * std::sin(theta), std::cos(theta),
              std::sin(phi) * std::sin(theta));
}