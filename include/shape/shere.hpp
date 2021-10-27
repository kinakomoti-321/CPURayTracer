#pragma once
#include <algorithm>>
#include <cmath>
#include <utility>

#include "include/core/constant.hpp"
#include "include/core/ray.hpp"
#include "include/intersection/intersectinfo.h"
#include "include/math/vec3.hpp"
#include "include/shape/shape.hpp"
class Sphere : public Shape {
 private:
  const Vec3 center;
  const float radius;

 public:
  Sphere(const Vec3& center, float raduis) : center(center), radius(raduis){};

  bool intersect(const Ray& ray, IntersectInfo& info) {
    Vec3 alpha = ray.origin - center;
    float b = dot(ray.direction, alpha);
    float c = norm2(alpha) - radius * radius;
    float D = b * b - c;

    if (D < 0) return false;

    float t1 = -b + std::sqrt(D);
    float t2 = -b - std::sqrt(D);
    float t = t1;

    if (ray.Min > t || ray.Max < t) {
      t = t2;
      if (ray.Min > t || ray.Max < t) {
        return false;
      }
    }

    info.distance = t;
    info.position = ray.post(t);
    info.normal = normalize(info.position - center);
    info.uv = sphereUV(info.normal);
  }

  std::pair<float, float> sphereUV(const Vec3& normal) {
    std::pair<float, float> uv;
    float theta = std::acos(normal[1]);
    float phi = (normal[2] > 0 - normal[2] < 0) *
                std::acos(normal[0] / std::sqrt(normal[2] * normal[2] +
                                                normal[0] * normal[0]));

    uv.first = std::clamp(1.0f - theta * PI_INV, 0.0f, 1.0f);
    uv.second = std::clamp(1.0f - (phi + PI) / (2.0f * PI), 0.0f, 1.0f);
    return uv;
  }
};