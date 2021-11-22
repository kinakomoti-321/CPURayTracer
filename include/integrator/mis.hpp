#pragma once

#include "core/ray.hpp"
#include "core/scene.hpp"
#include "sampling/sampler.hpp"
#include "integrator/integrator.hpp"
#include "core/constant.hpp"
class MIS :public Integrator {
public:

    //Multiple Importance Sampling by Multi Sample Model. BalanceHeuristic
    Vec3 integrate(const Ray& ray, const Scene& scene, std::shared_ptr<Sampler>& sample) const override {
        float p = 1.0; //Probability Rossian Roulette
        const unsigned int MaxDepth = 100; //Max of RayDepth
        Vec3 LTE = Vec3(0.0); //Result of Radiance
        Vec3 throughput = Vec3(1.0); //ThroughPut of Radiance. Midway Waight
        IntersectInfo info; //information of intersected surface. use as information of Current location
        Ray next_ray = ray; //the ray to shot next

        for (int depth = 0; depth < MaxDepth; depth++) {
            // Russian roulette
            p = std::min(std::max(std::max(throughput[0], throughput[1]), throughput[2]), 1.0f);
            if (sample->sample() > p) break;
            throughput /= p;


            //Ray shot
            IntersectInfo info;
            if (!scene.intersect(next_ray, info)) {
                //Ray into sky
                if (depth == 0) {
                    //first shot
                    LTE = throughput * scene.skyLe(next_ray);
                }
                break;
            }


            if (info.object->hasLight()) {
                //Ray into light
                if (depth == 0) {
                    //first shot
                    LTE += throughput * info.object->Le();
                }
                break;
            }

            //directions
            // wo: 入射方向,wi:反射方向
            Vec3 t, b;
            tangentSpaceBasis(info.normal, t, b);
            Vec3 wo = localToWorld(-next_ray.direction, t, info.normal, b);
            Vec3 wi;
            float pdf;
            Vec3 bsdf;

            //NEE
            {
                //光源サンプリング
                IntersectInfo lightInfo;
                Vec3 lightPos = scene.lightPointSampling(sample, lightInfo, pdf);

                //LightPath生成
                Vec3 lightDir = normalize(lightPos - info.position);
                Ray shadowRay(info.position, lightDir);
                lightInfo.distance = norm(lightPos - info.position);

                shadowRay.Max = lightInfo.distance - 0.001f;
                IntersectInfo shadowInfo;

                if (!scene.intersect(shadowRay, shadowInfo)) {
                    //LightPathの接続が成功

                    float cosine1 = std::abs(dot(info.normal, lightDir));
                    float cosine2 = std::abs(dot(lightInfo.normal, -lightDir));

                    wi = lightDir;

                    Vec3 local_wo = worldtoLocal(wo, t, info.normal, b);
                    Vec3 local_wi = worldtoLocal(wi, t, info.normal, b);

                    bsdf = info.object->evaluateBSDF(local_wo, local_wi);
                    //幾何項
                    float G = cosine2 / (lightInfo.distance * lightInfo.distance);

                    //MISweight
                    float bsdfPDF = info.object->BSDFpdf(wi) * G; //PathtraceにおけるPDF
                    float MISweight = pdf / (bsdfPDF + pdf);

                    //result
                    LTE += throughput * MISweight * (bsdf * G * cosine1 / pdf) * lightInfo.object->Le();
                }
            }

            //Pathtrace
            {
                //方向サンプリング
                float pathPdf;
                wo = worldtoLocal(-ray.direction, t, info.normal, b);
                Vec3 bsdf = info.object->sampleBSDF(wo, wi, pathPdf, sample);
                const Vec3 nextDir = localToWorld(wi, t, info.normal, b);

                Ray lightRay(info.position, nextDir);
                IntersectInfo lightInfo;

                if (scene.intersect(lightRay, lightInfo)) {
                    if (lightInfo.object->hasLight()) {
                        //衝突かつその物体がLight

                        float cosine1 = absdot(info.normal, nextDir);
                        float cosine2 = absdot(lightInfo.normal, -nextDir);
                        float dis2 = lightInfo.distance * lightInfo.distance;

                        //幾何項の逆数
                        float invG = dis2 / cosine2;

                        //MISweight
                        //NEEでのPDF
                        float lightPdf = 1.0f / (lightInfo.object->areaShape() * scene.getLightN()) * invG;
                        float MISweight = pathPdf / (pathPdf + lightPdf);

                        //Result
                        LTE += throughput * cosine1 * MISweight * lightInfo.object->Le() * bsdf / pathPdf;
                    }
                }
            }

            //次に進む方向のサンプリング

            wo = worldtoLocal(-next_ray.direction, t, info.normal, b);
            bsdf = info.object->sampleBSDF(wo, wi, pdf, sample);

            const Vec3 next_direction = localToWorld(wi, t, info.normal, b);
            const float cosine = std::abs(dot(info.normal, next_direction));
            throughput *= bsdf * cosine / pdf;

            //次のレイ
            next_ray = Ray(info.position + 0.001f * info.normal, next_direction);
        }

        return LTE;
    }
};