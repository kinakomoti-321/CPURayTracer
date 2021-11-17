#pragma once

#include "core/ray.hpp"
#include "core/scene.hpp"
#include "sampling/sampler.hpp"
#include "integrator/integrator.hpp"
#include "core/constant.hpp"
class MIS :public Integrator {
public:
    Vec3 integrate(const Ray& ray, const Scene& scene,
        std::shared_ptr<Sampler>& sampler) const override {
        const int MaxDepth = 100;
        float p = 0.99;
        Vec3 throughput(1.0);
        Vec3 LTE(0);
        Ray next_ray = ray;
        // std::cout << "start" << std::endl;
        for (int depth = 0; depth < MaxDepth; depth++) {
            // Russian roulette
            p = std::min(std::max(std::max(throughput[0], throughput[1]), throughput[2]), 1.0f);
            if (sampler->sample() > p) break;
            throughput /= p;

            IntersectInfo info;
            if (!scene.intersect(next_ray, info)) {
                if (depth == 0) {
                    LTE = throughput * scene.skyLe(next_ray);
                }
                break;
            }

            //  std::cout << "HiT" << std::endl;
            const Object& obj = *info.object;
            //  std::cout << "E" << std::endl;
            if (obj.hasLight()) {
                if (depth == 0) {
                    LTE += throughput * obj.Le();
                }
                break;
            }


            // std::cout << "nextSampling" << std::endl;
              // wo: 入射方向,wi:反射方向
            Vec3 wo = -next_ray.direction;
            Vec3 wi;
            float pdf;
            Vec3 bsdf;

            Vec3 t, b;
            tangentSpaceBasis(info.normal, t, b);


            //光源サンプリング
            IntersectInfo lightInfo;
            Vec3 lightPos = scene.lightPointSampling(sampler, lightInfo, pdf);

            Vec3 lightDir = normalize(lightPos - info.position);
            Ray shadowRay(info.position, lightDir);
            lightInfo.distance = norm(lightPos - info.position);

            shadowRay.Max = lightInfo.distance - 0.001f;
            IntersectInfo shadowInfo;

            if (!scene.intersect(shadowRay, shadowInfo)) {
                float cosine1 = std::abs(dot(info.normal, lightDir));
                float cosine2 = std::abs(dot(lightInfo.normal, -lightDir));

                wi = lightDir;

                Vec3 local_wo = worldtoLocal(wo, t, info.normal, b);
                Vec3 local_wi = worldtoLocal(wi, t, info.normal, b);

                bsdf = info.object->evaluateBSDF(local_wo, local_wi);

                // std::cout << "cosine1 : " << cosine1<< std::endl;
                // std::cout << "cosine2 : " << cosine2<<std::endl;
                // std::cout << "light_normal : " <<lightInfo.normal <<std::endl;
                // std::cout << "light_distance : " <<lightInfo.distance <<std::endl;
                // std::cout << "lightDir : " << -lightDir << cosine2<<std::endl;
                // std::cout << "wi : " << wi<<std::endl;
                // std::cout << "bsdf : " << bsdf<<std::endl;
                float waight = cosine1 * cosine2 / (lightInfo.distance * lightInfo.distance);

                LTE += throughput * (bsdf * waight / pdf) * lightInfo.object->Le();
            }
            wo = worldtoLocal(-ray.direction, t, info.normal, b);
            // BSDF計算
            bsdf = info.object->sampleBSDF(wo, wi, pdf, sampler);

            const Vec3 next_direction = localToWorld(wi, t, info.normal, b);
            const float cosine = std::abs(dot(info.normal, next_direction));
            throughput *= bsdf * cosine / pdf;

            next_ray = Ray(info.position + 0.001f * info.normal, next_direction);
        }

        return LTE;

    };
};