#pragma once

#include "./integrator.hpp"
#include "core/scene.hpp"
#include "core/ray.hpp"
#include "sampling/sampler.hpp"
#include "math/vec3.hpp"

class NormalChecker: public Integrator{
    public:
    Vec3 integrate(const Ray& ray, const Scene& scene,
                         std::shared_ptr<Sampler>& sampler){

                            IntersectInfo info;
                            if(scene.intersect(ray,info)){

                                return (info.normal + Vec3(1.0)) * 0.5f;
                            }

                            return Vec3(0.0);
                         };
};

