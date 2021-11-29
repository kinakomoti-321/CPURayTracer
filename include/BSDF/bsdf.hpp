#pragma once
#include <algorithm>
#include <cmath>
#include <memory>
#include "core/constant.hpp"
#include "math/vec3.hpp"
#include "sampling/sampler.hpp"
class BSDF {
public:
  virtual Vec3 samplingBSDF(const Vec3& wo, Vec3& wi, float& pdf,
    const std::shared_ptr<Sampler>& sampler) const = 0;
  virtual Vec3 evaluateBSDF(const Vec3& wo, const Vec3& wi) const = 0;
  virtual float samplePDF(const Vec3& wo, const Vec3& wi)const = 0;
};

namespace BSDFMath {
  inline float cosTheta(const Vec3& w) { return w[1]; }
  inline float cos2Theta(const Vec3& w) { return w[1] * w[1]; }
  inline float sinTheta(const Vec3& w) { return std::sqrt(std::max(1.0f - cosTheta(w) * cosTheta(w), 0.0f)); }
  inline float tanTheta(const Vec3& w) { return sinTheta(w) / cosTheta(w); }
  inline float tan2Theta(const Vec3& w) { return tanTheta(w) * tanTheta(w); }


}