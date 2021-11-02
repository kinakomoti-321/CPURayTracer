#pragma once
#include "BSDF/bsdf.hpp"
#include "core/constant.hpp"
#include <iostream>
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
};