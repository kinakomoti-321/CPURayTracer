#pragma once
#include <memory>

#include "BSDF/bsdf.hpp"
#include "core/ray.hpp"
#include "intersection/intersectinfo.h"
#include "light/light.hpp"
#include "shape/shape.hpp"

class Object {
 private:
  std::shared_ptr<Shape> shape;
  std::shared_ptr<Light> light;
  std::shared_ptr<BSDF> bsdf;

 public:
  Object(std::shared_ptr<Shape> shape, std::shared_ptr<BSDF> bsdf)
      : shape(shape), bsdf(bsdf){};

  Object(std::shared_ptr<Shape> shape, std::shared_ptr<Light> light,
         std::shared_ptr<BSDF> bsdf)
      : shape(shape), light(light), bsdf(bsdf){};

  bool hasLight() const { return light != nullptr; }
  Vec3 evaluateBSDF(const Vec3& wo, Vec3& wi, float& pdf, std::shared_ptr<Sampler>& sampler) {
    return bsdf->samplingBSDF(wo, wi, pdf, sampler);
  }
  Vec3 Le() const { return light->Le(); }

  bool intersection(const Ray& ray, IntersectInfo& info) {
    bool check = shape->intersect(ray, info);
    if (check) {
      info.object = this;
    }
    return check;
  }
};