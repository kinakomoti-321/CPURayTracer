#pragma once
#include <cmath>
#include <memory>
#include <iostream>
#include "BSDF/bsdf.hpp"
#include "math/vec3.hpp"
#include "BSDF/specular.hpp"

inline float fresnel(const Vec3& wo,const Vec3& n,const float ior1,const float ior2){
   float F0 = std::pow((ior1 - ior2) / (ior1 + ior2), 2.0);
   float F = F0 + (1.0 - F0) * std::pow(1.0f - dot(wo, n), 5.0);
   return F;  
}

inline bool refraction(const Vec3 &v, const Vec3 &n, const float n1,
                       const float n2, Vec3 &r) {
  Vec3 th = (-n1 / n2) * (v - dot(v, n) * n);

  if (norm2(th) > 1.0) {
    return false;
  }

  const Vec3 tp = -std::sqrt(std::max(1.0f - norm2(th), 0.0f)) * n;
  r = tp + th;
  return true;
}
inline bool refract(const Vec3& wo,Vec3& wi,const Vec3& n,float n1,float n2){
    const Vec3 th = -(n1/n2) * (wo - dot(wo,n) * n);
    if(norm2(th) > 1.0) return false;
    const Vec3 tp = -std::sqrt(std::max(1.0f - norm2(th),0.0f)) * n;
    wi = th + tp;
    return true;
}

class Grass:public BSDF{
    private:
    Vec3 rho;
    float ior;
    public:
    Grass(const Vec3& rho,const float& ior):rho(rho),ior(ior){};

   Vec3 samplingBSDF(const Vec3& wo, Vec3& wi, float& pdf,
                            const std::shared_ptr<Sampler>& sampler) const override{
          Vec3 n(0, 1, 0);
  float ior1, ior2;
  //内部かどうかの判定
  //外側からの侵入
  if (wo[1] > 0) {
    ior1 = 1.0f;
    ior2 = ior;
  }
  //内部から出る
  else {
    ior1 = ior;
    ior2 = 1.0f;
    n = Vec3(0, -1, 0);
    // std::cout << "test" << std::endl;
  }

  const float Fr = fresnel(wo, n, ior1, ior2);
  //フレネル反射の場合
  if (Fr > sampler->sample()) {
    wi = reflect(wo, n);
    pdf = 1.0;
    return rho / std::abs(wi[1]);
  }
  //屈折の場合
  else {
    Vec3 t;
    if (refract(wo, t,n,ior1, ior2)) {
      //屈折した場合
      wi = t;
      pdf = 1;
      return rho / std::abs(wi[1]);
    } else {
      //全反射
      wi = reflect(wo, n);
      pdf = 1.0f;
      return rho / std::abs(wi[1]);
    }
  }
   };
   Vec3 evaluateBSDF(const Vec3& wo,const Vec3& wi) const override{
       return Vec3(0);
   };         
   float samplePDF(const Vec3& wi)const override{
      return 0; 
   };
};
