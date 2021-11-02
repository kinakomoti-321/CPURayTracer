#pragma once
#include <memory>
#include <iostream>
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
  Object(const std::shared_ptr<Shape>& shape, 
         const std::shared_ptr<BSDF>& bsdf,const std::shared_ptr<Light>& light = nullptr)
      : shape(shape), bsdf(bsdf),light(light){};

  bool hasLight() const { return light != nullptr; }
  Vec3 sampleBSDF(const Vec3& wo, Vec3& wi, float& pdf,const std::shared_ptr<Sampler>& sampler) {
    return bsdf->samplingBSDF(wo, wi, pdf, sampler);
  }
  Vec3 Le() const {
     return light->Le(); }

  bool intersection(const Ray& ray, IntersectInfo& info) {
    bool check = shape->intersect(ray, info);
    if (check) {
      info.object = this;
    }
    return check;
  }

  Vec3 areaSampling(Sampler& sampler,IntersectInfo& info){
    info.object = this;
    return shape->areaSampling(sampler,info);
  }

  float areaShape(){
    return shape->surfaceArea();
  }
};