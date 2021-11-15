#pragma once
#include "core/object.hpp"
#include "integrator/integrator.hpp"
#include "intersection/intersectinfo.h"
#include <iostream>
class Pathtracer : public Integrator {
 public:
  Vec3 integrate(const Ray& ray, const Scene& scene, std::shared_ptr<Sampler>& sample) const override{
    const int MaxDepth = 100;
    const float p = 0.99;
    Vec3 throughput(1.0);
    Vec3 LTE(0);
    Ray next_ray = ray;
    // std::cout << "start" << std::endl;
    for (int i = 0; i < MaxDepth; i++) {
      // Russian roulette
      if (sample->sample() > p) break;
      throughput /= p;

      IntersectInfo info;
      if (!scene.intersect(next_ray, info)) {
        //segmentationErrorcheck
        LTE = throughput * scene.skyLe(next_ray);
        break;
      }

    //  std::cout << "HiT" << std::endl;
      const Object& obj = *info.object;
    //  std::cout << "E" << std::endl;
      if (obj.hasLight()) {
        LTE = throughput * obj.Le();
        break;
      }

    // std::cout << "nextSampling" << std::endl;
      // wo: 入射方向,wi:反射方向
      Vec3 t,b;
      tangentSpaceBasis(info.normal,t,b);
      Vec3 wo = worldtoLocal(-ray.direction,t,info.normal,b);
      Vec3 wi;
      float pdf;
      Vec3 bsdf;

      // BSDF計算
      bsdf = info.object->sampleBSDF(wo, wi, pdf, sample);

      const Vec3 next_direction= localToWorld(wi,t,info.normal,b);
      const float cosine = std::abs(dot(info.normal, next_direction));
      throughput *= bsdf * cosine / pdf;
      next_ray = Ray(info.position , next_direction);
      // std::cout << "Pathtrace_check" << std::endl;
    }
    // std::cout << "1 sample finished" << std::endl;
    return LTE;
  }
};