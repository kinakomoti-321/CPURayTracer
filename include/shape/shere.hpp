#pragma once
#include <algorithm>
#include <cmath>
#include <utility>

#include "core/constant.hpp"
#include "core/ray.hpp"
#include "intersection/intersectinfo.h"
#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include "shape/shape.hpp"
#include "core/constant.hpp"
#include "core/aabb.hpp"
class Sphere : public Shape {
 private:
  const Vec3 center;
  const float radius;

 public:
  Sphere():center(Vec3(0.0)),radius(1.0f){}

  Sphere(const Vec3& center, float raduis) : center(center), radius(raduis){};

  AABB aabb()const override{
    return AABB(center - Vec3(radius),center + Vec3(radius));
  }
  float surfaceArea()const override{
   return 4.0f * PI * radius * radius; 
  }
  bool intersect(const Ray& ray, IntersectInfo& info)const override{
    float b = dot(ray.origin - center,ray.direction);
    float c = norm2(ray.origin - center) - radius * radius;
    float D = b * b - c;

    float t1 = - b - std::sqrt(D);
    float t2 = - b + std::sqrt(D);

    float t = t1;
    if(t > ray.Max || t < ray.Min){
        float t = t2;
    if(t > ray.Max || t < ray.Min){
        return false;
    }
    }

    info.distance = t;
    info.position = ray.post(t);
    info.normal = normalize(info.position - center);
    info.uv = sphereUV(info.normal); 
    return true;
  }

  Vec2 sphereUV(const Vec3& normal)const {
Vec2 uv;
    float theta = std::acos(normal[1]);
    float phi = (normal[2] > 0 - normal[2] < 0) *
                std::acos(normal[0] / std::sqrt(normal[2] * normal[2] +
                                                normal[0] * normal[0]));

    uv[0] = std::clamp(1.0f - theta * PI_INV, 0.0f, 1.0f);
    uv[1]= std::clamp(1.0f - (phi + PI) / (2.0f * PI), 0.0f, 1.0f);
    return uv;
  }
  
  Vec3 areaSampling(Sampler& sampler,IntersectInfo& info)const override{
    float u = sampler.sample(),v = sampler.sample();
    float z = - 2.0f * u + 1.0f;
    float y = std::sqrt(std::max(1.0f - z * z,0.0f)) * std::sin(2.0f * v);
    float x = std::sqrt(std::max(1.0f - z * z,0.0f)) * std::cos(2.0f * v);

    Vec3 pos = center + radius * Vec3(x,y,z);
    info.position = pos;
    info.normal = normalize(Vec3(x,y,z));
    info.uv = sphereUV(info.normal);
    return pos;
  }
};