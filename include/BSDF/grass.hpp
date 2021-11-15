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
  float F0 = std::pow((ior1 - ior2) / (ior1 + ior2), 2.0);
  float F = F0 + (1.0 - F0) * std::pow(1.0f - dot(wo, n), 5.0);
  return F;        float ior1,ior2;
        Vec3 n;
        if(wo[1] > 0.0){
            ior1 = 1.0f;
            ior2 = ior;
            n = Vec3(0.0,1.0,0.0);
        }
        else{
            ior1 = ior;
            ior2 = 1.0f;
            n = Vec3(0.0,-1.0,0.0);
        }

        const float frn = fresnel(wo,n,ior1,ior2);

       if(sampler->sample() < frn){
           wi = reflect(wo,n);
           pdf = 1;
           return rho / std::abs(wi[1]);
       }
       else{
           Vec3 t;
           if(refract(wo,t,n,ior1,ior2)){
               wi = t;
               pdf = 1;
                // std::cout << "Yes" << std::endl; 
                // std::cout << "t : " << t << std::endl; 
                // std::cout << "bsdf : " << rho / std::abs(wi[1]) <<  std::endl; 
               return rho / std::abs(wi[1]);
           }
           else{
               wi = reflect(wo,n);
               pdf = 1;
               return rho/std::abs(wi[1]);
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
