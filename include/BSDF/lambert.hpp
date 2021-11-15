#pragma once
#include "BSDF/bsdf.hpp"
#include "core/constant.hpp"
#include <iostream>
#include <cmath>

inline Vec3 cosineSampling(float u, float v, float& pdf) {
  const float theta =
      std::acos(std::clamp(1.0f - 2.0f * u, -1.0f, 1.0f)) / 2.0f;
  const float phi = 2.0f * PI * v;
  pdf = cos(theta) / PI;
  return Vec3(std::cos(phi) * std::sin(theta), std::cos(theta),
              std::sin(phi) * std::sin(theta));
}

class Lambert : public BSDF {
 private:
  Vec3 rho;

 public:
  Lambert(const Vec3& rho) : rho(rho){};
  Vec3 samplingBSDF(const Vec3& wo, Vec3& wi, float& pdf,const std::shared_ptr<Sampler>& sampler)const override {
    wi = cosineSampling(sampler->sample(), sampler->sample(), pdf);
    return rho * PI_INV;
  }
  Vec3 evaluateBSDF(const Vec3& wo,const Vec3& wi)const override{
    return rho * PI_INV;
  }
  float samplePDF(const Vec3& wi)const override{
    return std::abs(wi[1]) / PI;
  }
};