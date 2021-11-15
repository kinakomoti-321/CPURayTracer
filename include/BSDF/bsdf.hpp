#pragma once
#include <algorithm>
#include <cmath>

#include "core/constant.hpp"
#include "math/vec3.hpp"
#include "sampling/sampler.hpp"
class BSDF {
 public:
  virtual Vec3 samplingBSDF(const Vec3& wo, Vec3& wi, float& pdf,
                            const std::shared_ptr<Sampler>& sampler) const = 0;
  virtual Vec3 evaluateBSDF(const Vec3& wo,const Vec3& wi) const = 0;         
  virtual float samplePDF(const Vec3& wi)const =0;
};

