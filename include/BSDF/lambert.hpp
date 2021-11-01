#pragma once
#include "BSDF/bsdf.hpp"
#include "core/constant.hpp"
class Lambert : public BSDF {
 private:
  Vec3 rho;

 public:
  Lambert(const Vec3& rho) : rho(rho){};
  Vec3 samplingBSDF(const Vec3& wo, Vec3& wi, float& pdf, std::shared_ptr<Sampler>& sampler) {
    wi = cosineSampling(sampler->sample(), sampler->sample(), pdf);
    return rho * PI_INV;
  }
};