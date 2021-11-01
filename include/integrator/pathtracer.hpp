#pragma once
#include "core/object.hpp"
#include "integrator/integrator.hpp"
#include "intersection/intersectinfo.h"
class Pathtracer : public Integrator {
 public:
  Vec3 integrate(const Ray& ray, const Scene& scene, std::shared_ptr<Sampler>& sample) {
    const int MaxDepth = 100;
    const float p = 0.99;
    Vec3 throughput(1.0);
    Vec3 LTE(0);
    Ray next_ray = ray;
    for (int i = 0; i < MaxDepth; i++) {
      // Russian roulette
      if (sample->sample() > p) break;
      throughput /= p;

      IntersectInfo info;
      if (scene.intersect(next_ray, info)) {
        LTE = throughput * scene.skyLe(ray);
        break;
      }

      const Object& obj = *info.object;

      if (obj.hasLight()) {
        LTE = throughput * obj.Le();
        break;
      }

      // wo: 入射方向,wi:反射方向
      Vec3 wo = -next_ray.direction;
      Vec3 wi;
      float pdf;
      Vec3 bsdf;

      // BSDF計算
      bsdf = info.object->evaluateBSDF(wo, wi, pdf, sample);

      const float cosine = std::abs(dot(wi, info.normal));
      throughput *= bsdf * cosine / pdf;

      next_ray = Ray(info.position + 0.001f * info.normal, wi);
    }

    return LTE;
  }
};