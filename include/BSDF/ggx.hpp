#pragma once
#include "BSDF/bsdf.hpp"
#include "core/constant.hpp"
#include <iostream>
#include <cmath>
#include "BSDF/lambert.hpp"
#include "math/vec3.hpp"
#include <memory>

class GGX :public BSDF {

private:
    Vec3 F0;
    float alpha;

    Vec3 Fresnel(const float im) const {
        float delta = std::max(1.0f - im, 0.0f);
        return F0 + (Vec3(1.0f) - F0) * std::pow(delta, 5);
    }

    float G(const Vec3& v, const Vec3& m)const {
        float delta = alpha * BSDFMath::tanTheta(v);
        // float sign = ((dot(v, m) / BSDFMath::cosTheta(v)) > 0.0f) ? 1.0f : 0.0f;
        float sign = 1.0f;
        float result = sign * 2.0f / (1.0f + std::sqrt(1.0f + delta * delta));

        return result;
    }

    float lambda(const Vec3& v) const {
        float delta = std::max(alpha * BSDFMath::tanTheta(v), 0.0f);
        return std::max((-1.0f + std::sqrt(1.0f + delta * delta)) / 2.0f, 0.0f);
    }
    float G2(const Vec3& o, const Vec3& i) const {
        return 1.0f / (1.0f + this->lambda(o) + this->lambda(i));
    }


    Vec3 samplem(float u, float v) const {
        float theta = std::atan(alpha * std::sqrt(u) / std::sqrt(std::max(1.0f - u, 0.0f)));
        float phi = 2.0f * PI * v;
        return Vec3(std::cos(phi) * std::sin(theta), std::cos(theta),
            std::sin(phi) * std::sin(theta));
    }


public:
    GGX(const Vec3& F0, const float roughness) :F0(F0) {
        alpha = std::clamp(roughness * roughness, 0.01f, 1.0f);
    }

    float D(const Vec3& m) const {
        // float sign = 1.0f;
        // float cosm = BSDFMath::cosTheta(m);
        // float tanm = BSDFMath::tanTheta(m);

        // float cosm4 = cosm * cosm * cosm * cosm;
        // float delta = alpha * alpha + tanm * tanm;

        // float result = alpha * alpha * sign / (PI * cosm4 * delta * delta);
        // return result;
        float cosm = BSDFMath::cosTheta(m);
        cosm = cosm * cosm;
        float delta = 1.0f - (1.0f - alpha * alpha) * cosm;
        return alpha * alpha / (PI * delta * delta);
    }

    Vec3 samplingBSDF(const Vec3& wo, Vec3& wi, float& pdf,
        const std::shared_ptr<Sampler>& sampler) const override {
        // wi = SphereSampling(sampler->sample(), sampler->sample(), pdf);

        // Vec3 i = wo;
        // Vec3 o = wi;
        // Vec3 m = normalize(i + o);
        // Vec3 n = Vec3(0.0, 1.0, 0.0);

        Vec3 i = wo;
        Vec3 n = Vec3(0.0, 1.0, 0.0);
        Vec3 m = this->samplem(sampler->sample(), sampler->sample());
        Vec3 o = reflect(wo, m);
        wi = o;
        if (wi[1] < 0.0f) return Vec3(0.0);

        // DebugLog("o", o);
        // DebugLog("m", m);

        float im = absdot(i, m);
        float in = absdot(i, n);
        float on = absdot(o, n);

        Vec3 F = this->Fresnel(im);
        float G_ = this->G2(i, o);
        float D_ = this->D(m);

        Vec3 brdf = F * G_ * D_ / (4.0f * in * on);

        pdf = D_ * BSDFMath::cosTheta(m) / (4.0f * absdot(m, o));

        // if (brdf[1] > 1.0f) {
        //     DebugLog("F", F);
        //     DebugLog("G", G_);
        //     DebugLog("D", D_);
        //     DebugLog("brdf", brdf);
        //     DebugLog("pdf", pdf);
        //     DebugLog("m", m);
        //     DebugLog("i", i);
        // }
        return brdf;
    }

    Vec3 evaluateBSDF(const Vec3& wo, const Vec3& wi) const  override {
        Vec3 i = wo;
        Vec3 o = wi;
        Vec3 m = normalize(i + o);
        Vec3 n = Vec3(0.0, 1.0, 0.0);

        float im = absdot(i, m);
        float in = absdot(i, n);
        float on = absdot(o, n);

        Vec3 F = this->Fresnel(im);
        float G_ = this->G(i, m) * this->G(o, m);
        float D_ = this->D(m);

        Vec3 brdf = F * G_ * D_ / (4.0f * in * on);

        return brdf;
    }

    float samplePDF(const Vec3& wo, const Vec3& wi)const override {
        // return 0.5f * PI_INV;
        Vec3 m = normalize(wo + wi);
        float D_ = this->D(m);
        float pdf = D_ * BSDFMath::cosTheta(m) / (4.0f * absdot(wi, m));
        return pdf;
    }
};