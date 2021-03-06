#pragma once

#include "core/ray.hpp"
#include "core/scene.hpp"
#include "sampling/sampler.hpp"
#include "integrator/integrator.hpp"
#include "core/constant.hpp"
class Volume_homo_render :public Integrator {
public:
    Vec3 integrate(const Ray& ray, const Scene& scene,
        std::shared_ptr<Sampler> sample) const override {

        float p = 1.0; //Probability Rossian Roulette
        const unsigned int MaxDepth = 100; //Max of RayDepth
        Vec3 LTE = Vec3(0.0); //Result of Radiance
        Vec3 throughput = Vec3(1.0); //ThroughPut of Radiance. Midway Waight
        IntersectInfo info; //information of intersected surface. use as information of Current location
        Ray next_ray = ray; //the ray to shot next

        bool is_inVolume = false;

        for (int depth = 0; depth < MaxDepth; depth++) {
            // Russian roulette
            p = std::min(std::max(std::max(throughput[0], throughput[1]), throughput[2]), 1.0f);
            if (sample->sample() > p) break;
            throughput /= p;


            IntersectInfo info;
            //Ray shot
            if (!scene.intersect(next_ray, info)) {
                LTE = throughput * scene.skyLe(next_ray);
                break;
            }

            if (info.object->hasLight()) {
                LTE = throughput * info.object->Le();
                break;
            }

            if (info.object->hasVolume()) {
                // DebugLog("Volume_render");
                Vec3 volDir = next_ray.direction;
                Ray vol_ray = Ray(info.position + 0.001f * volDir, volDir);
                for (int i = 0;;i++) {
                    // if (i > 40) return LTE;
                    // 
                    p = std::min(std::max(std::max(throughput[0], throughput[1]), throughput[2]), 1.0f);
                    if (sample->sample() > p) break;
                    throughput /= p;

                    IntersectInfo volInfo;
                    scene.intersect(vol_ray, volInfo);
                    auto vol = info.object->getVolume();

                    float t = vol->sampleS(sample);
                    //Term2?????????
                    if (t >= volInfo.distance) {
                        next_ray = Ray(volInfo.position + 0.001f * volDir, volDir);
                        break;
                    }

                    //Term1?????????
                    //??????????????????????????????????????????
                    float mueT = vol->getmueT();
                    float mueA = vol->getmueA();
                    // //??????????????????
                    // if (sample->sample() < (mueA / mueT)) {
                    //     // throughput *
                    //     vol_ray = Ray(vol_ray.origin + t * volDir, volDir);
                    //     LTE += vol->Le();
                    //     return LTE;
                    // }
                    // //??????????????????
                    // else {
                    volDir = vol->phaseFanc(volDir, sample);
                    DebugLog("volDir", volDir);
                    vol_ray = Ray(vol_ray.origin + t * vol_ray.direction, volDir);
                    throughput *= (1.0f - mueA / mueT);
                    // }

                }
            }
            else {
                //raytracing

                // wo: ????????????,wi:????????????
                Vec3 t, b;
                tangentSpaceBasis(info.normal, t, b);
                Vec3 wo = worldtoLocal(-next_ray.direction, t, info.normal, b);
                Vec3 wi;
                float pdf;
                Vec3 bsdf;

                // BSDF??????
                bsdf = info.object->sampleBSDF(wo, wi, pdf, sample);

                const Vec3 next_direction = localToWorld(wi, t, info.normal, b);
                const float cosine = std::abs(dot(info.normal, next_direction));
                throughput *= bsdf * cosine / pdf;

                next_ray = Ray(info.position, next_direction);
            }
        }
        return LTE;
    }
};