#pragma once 
#include "shape/shape.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include <cmath>
#include <memory>
#include "core/aabb.hpp"
struct TriInfo{
    Vec3 point[3];
};
class Triangle: public Shape{
    private:
    TriInfo triinfo;
    Vec3 e1,e2;

    public:
    Triangle(const TriInfo& triinfo): triinfo(triinfo){
       e1 = triinfo.point[1] - triinfo.point[0];
       e2 = triinfo.point[2] - triinfo.point[0];
    }

    AABB aabb() const override{
        Vec3 pMax,pMin;
        for(int i = 0; i < 3; i++){
            pMin[i] = std::min(std::min(triinfo.point[0][i],triinfo.point[1][i]),triinfo.point[2][i]);
            pMax[i] = std::max(std::max(triinfo.point[0][i],triinfo.point[1][i]),triinfo.point[2][i]);
        }
        return AABB(pMin,pMax);
    }

    float surfaceArea() const override{
        return std::abs(norm(cross(e1,e2))) / 2.0f;
    }
    
    Vec3 areaSampling(const std::shared_ptr<Sampler>& sampler,IntersectInfo& info)const override{
        float u = sampler->sample(),v = sampler->sample();
         
        info.position = triinfo.point[0] * (1.0f - std::sqrt(u)) + triinfo.point[1] * (std::sqrt(u) * (1.0f - v)) + triinfo.point[2] * (std::sqrt(u) * v);
        info.normal = normalize(cross(e1,e2));

        return info.position;
    }

   bool intersect(const Ray& ray, IntersectInfo& info) const override{
        const Vec3 alpha = cross(ray.direction,e2);
        float Det = dot(alpha,e1);

        if(-0.001 < Det && 0.001 > Det){
            return false;
        }

        const Vec3 r = ray.origin - triinfo.point[0];
        const Vec3 beta = cross(r,e1);

        float u = dot(alpha,r) / Det;
        if(u < 0.0 || 1.0 < u) return false;

        float v = dot(beta,ray.direction)/Det;
        if(v < 0.0 || 1.0 < u + v) return false;

        float t = dot(beta,e2)/Det;
        if(ray.Min > t || ray.Max < t) return false;

        info.distance = t;
        info.position = ray.post(t);
        info.normal = normalize(cross(e1,e2));
        // std::cout << info.normal << std::endl;
        info.uv = Vec2(1.0);
        
        // std::cout << "tri" << std::endl;
        return true;  
   }
};