#pragma once
#include <iostream>
#include <limits>

#include "core/ray.hpp"
#include "math/vec3.hpp"
struct AABB {
  // bounds[0] => min edge, bounds[1] => max edge
  Vec3 bounds[2];

  explicit AABB() {
    bounds[0] = Vec3(std::numeric_limits<float>::max());
    bounds[1] = Vec3(-std::numeric_limits<float>::max());
  }
  explicit AABB(const Vec3& pMin, const Vec3& pMax) {
    bounds[0] = pMin;
    bounds[1] = pMax;
  }

  Vec3 center() const { return 0.5f * (bounds[0] + bounds[1]); }

  int longestAxis() const {
    const Vec3 length = bounds[1] - bounds[0];
    // 0 -> x axis, 1 -> y axis, 2->z axis
    if (length[0] >= length[1] && length[0] >= length[2]) {
      return 0;
    } else if (length[1] >= length[2]) {
      return 1;
    } else {
      return 2;
    }
  }

  float surfaceArea() const {
    return 1;
  }

  bool intersect(const Ray& ray, const Vec3& dirInv,
                 const int dirInvSign[3]) const {
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    tmin = (bounds[dirInvSign[0]][0] - ray.origin[0]) * dirInv[0];
    tmax = (bounds[1 - dirInvSign[0]][0] - ray.origin[0]) * dirInv[0];
    tymin = (bounds[dirInvSign[1]][1] - ray.origin[1]) * dirInv[1];
    tymax = (bounds[1 - dirInvSign[1]][1] - ray.origin[1]) * dirInv[1];
    if (tmin > tymax || tymin > tmax) return false;
    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    tzmin = (bounds[dirInvSign[2]][2] - ray.origin[2]) * dirInv[2];
    tzmax = (bounds[1 - dirInvSign[2]][2] - ray.origin[2]) * dirInv[2];
    if (tmin > tzmax || tzmin > tmax) return false;
    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    return tmin < ray.Max && tmax > ray.Min;
  }
  void checkBounds() {
    std::cout << bounds[0] << " : " << bounds[1] << std::endl;
  }
};

inline AABB mergeAABB(const AABB& bbox, const Vec3& p) {
  AABB res;
  for (int i = 0; i < 3; i++) {
    res.bounds[0][i] = std::min(bbox.bounds[0][i], p[i]);
    res.bounds[1][i] = std::max(bbox.bounds[1][i], p[i]);
  }
  return res;
};
AABB mergeAABB(const AABB& bbox1, const AABB& bbox2) {
  AABB res;
  for (int i = 0; i < 3; i++) {
    res.bounds[0][i] = std::min(bbox1.bounds[0][i], bbox2.bounds[0][i]);
    res.bounds[1][i] = std::max(bbox1.bounds[1][i], bbox2.bounds[0][i]);
  }
  return res;
};

std::ostream& operator<<(std::ostream& stream, const AABB& bbox);