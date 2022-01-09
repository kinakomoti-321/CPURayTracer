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



public:
    GGX(const Vec3& F0, const float roughness) :F0(F0) {
        alpha = std::clamp(roughness * roughness, 0.01f, 1.0f);
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
        float theta = std::atan(alpha * std::sqrt(v) / std::sqrt(std::max(1.0f - v, 0.0f)));
        DebugLog("theta_GGX", theta);
        float phi = 2.0f * PI * u;
        return Vec3(std::cos(phi) * std::sin(theta), std::cos(theta),
            std::sin(phi) * std::sin(theta));
    }
    float D(const Vec3& m) const {
        // float sign = 1.0f;
        // float cosm = BSDFMath::cosTheta(m);
        // float tanm = BSDFMath::tanTheta(m);

        // float cosm4 = cosm * cosm * cosm * cosm;
        // float delta = alpha * alpha + tanm * tanm;

        // float result = alpha * alpha * sign / (PI * cosm4 * delta * delta);
        // return result;
        // float cosm = std::abs(m[1]);
        // float delta = 1.0f - (1.0f - alpha * alpha) * cosm * cosm;
        // return alpha * alpha / (PI * delta * delta);

        const float tan2theta = BSDFMath::tan2Theta(m);
        if (std::isinf(tan2theta)) return 0;
        const float cos4theta = BSDFMath::cos2Theta(m) * BSDFMath::cos2Theta(m);
        const float term = 1.0f + tan2theta / (alpha * alpha);
        return 1.0f / ((PI * alpha * alpha * cos4theta) * term * term);
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

        // if (brdf[1] < 0.01f) {
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
        if (m[1] < 0.0f) {
            DebugLog("m", m);
            DebugLog("wo", wo);
            DebugLog("wi", wi);
        }
        float D_ = this->D(m);
        float pdf = D_ * BSDFMath::cosTheta(m) / (4.0f * absdot(m, wo));
        return pdf;
    }
};

class GGX_anistropic :public BSDF {

private:
    Vec3 F0;
    float a_x;
    float a_y;

    Vec3 Fresnel(const float im) const {
        float delta = std::max(1.0f - im, 0.0f);
        return F0 + (Vec3(1.0f) - F0) * std::pow(delta, 5);
    }



public:
    GGX_anistropic(const Vec3& F0, const float ax, const float ay) :F0(F0) {
        a_x = std::clamp(ax * ax, 0.001f, 1.0f);
        a_y = std::clamp(ay * ay, 0.001f, 1.0f);
    }

    float D(const Vec3& m) const {
        const float tan2theta = BSDFMath::tan2Theta(m);
        if (std::isinf(tan2theta)) return 0;
        const float cos2phi = BSDFMath::cosPhi2(m);
        const float sin2phi = BSDFMath::sinPhi2(m);
        const float cos4theta = BSDFMath::cos2Theta(m) * BSDFMath::cos2Theta(m);

        float term_alpha = cos2phi / (a_x * a_x) + sin2phi / (a_y * a_y);
        float term = 1.0f + tan2theta * term_alpha;
        return 1.0f / (M_PI * a_x * a_y * cos4theta * term * term);
    }

    float G1(const Vec3& v)const {
        return 1.0f / (1.0f + this->lambda(v));
    }

    float lambda(const Vec3& v) const {
        float alpha = std::sqrt(BSDFMath::cosPhi2(v) * a_x * a_x + BSDFMath::sinPhi2(v) * a_y * a_y);
        float delta = std::max(alpha * BSDFMath::tanTheta(v), 0.0f);
        return std::max((-1.0f + std::sqrt(1.0f + delta * delta)) / 2.0f, 0.0f);
    }

    float G2(const Vec3& o, const Vec3& i) const {
        return 1.0f / (1.0f + this->lambda(o) + this->lambda(i));
    }


    Vec3 samplem(float u, float v) const {
        float tanTerm = std::tan(2.0f * PI * u) * a_y / a_x;
        float phi = std::atan(tanTerm);
        if (u >= 0.75) {
            phi += 2.0f * PI;
        }
        else if (u > 0.25) {
            phi += PI;
        }

        DebugLog("phi", phi);
        DebugLog("phi_a", 2.0f * PI * u);
        const float cos2phi = std::cos(phi) * std::cos(phi);
        const float sin2phi = std::sin(phi) * std::sin(phi);
        const float term_alpha = cos2phi / (a_x * a_x) + sin2phi / (a_y * a_y);
        DebugLog("x", a_x);
        DebugLog("y", a_y);

        DebugLog("alpha", term_alpha);
        DebugLog("cos2", cos2phi);
        DebugLog("sin2", sin2phi);
        // const float term = term_alpha * (1.0f - v) / v;
        const float term = v / ((1.0f - v) * term_alpha);
        const float theta = std::atan(std::sqrt(term));
        DebugLog("theta", theta);

        return Vec3(std::cos(phi) * std::sin(theta), std::cos(theta),
            std::sin(phi) * std::sin(theta));
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
        // Vec3 m = SphereSampling(sampler->sample(), sampler->sample(), pdf);
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

        // if (brdf[1] < 0.01f) {
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
        float G_ = this->G2(i, o);
        float D_ = this->D(m);

        Vec3 brdf = F * G_ * D_ / (4.0f * in * on);

        return brdf;
    }

    float samplePDF(const Vec3& wo, const Vec3& wi)const override {
        // return 0.5f * PI_INV;
        Vec3 m = normalize(wo + wi);
        if (m[1] < 0.0f) {
            DebugLog("m", m);
            DebugLog("wo", wo);
            DebugLog("wi", wi);
        }
        float D_ = this->D(m);
        float pdf = D_ * BSDFMath::cosTheta(m) / (4.0f * absdot(m, wo));
        return pdf;
    }
};

class GGX_VisibleNormal :public BSDF {

private:
    Vec3 F0;
    float a_x;
    float a_y;

    Vec3 Fresnel(const float im) const {
        float delta = std::max(1.0f - im, 0.0f);
        return F0 + (Vec3(1.0f) - F0) * std::pow(delta, 5);
    }

public:
    GGX_VisibleNormal(const Vec3& F0, const float ax, const float ay) :F0(F0) {
        a_x = std::clamp(ax * ax, 0.001f, 1.0f);
        a_y = std::clamp(ay * ay, 0.001f, 1.0f);
    }

    float D(const Vec3& m) const {
        const float tan2theta = BSDFMath::tan2Theta(m);
        if (std::isinf(tan2theta)) return 0;
        const float cos2phi = BSDFMath::cosPhi2(m);
        const float sin2phi = BSDFMath::sinPhi2(m);
        const float cos4theta = BSDFMath::cos2Theta(m) * BSDFMath::cos2Theta(m);

        float term_alpha = cos2phi / (a_x * a_x) + sin2phi / (a_y * a_y);
        float term = 1.0f + tan2theta * term_alpha;
        return 1.0f / (M_PI * a_x * a_y * cos4theta * term * term);
    }

    float G1(const Vec3& v)const {
        return 1.0f / (1.0f + this->lambda(v));
    }

    float lambda(const Vec3& v) const {
        float alpha = std::sqrt(BSDFMath::cosPhi2(v) * a_x * a_x + BSDFMath::sinPhi2(v) * a_y * a_y);
        float delta = std::max(alpha * BSDFMath::tanTheta(v), 0.0f);
        return std::max((-1.0f + std::sqrt(1.0f + delta * delta)) / 2.0f, 0.0f);
    }

    float G2(const Vec3& o, const Vec3& i) const {
        return 1.0f / (1.0f + this->lambda(o) + this->lambda(i));
    }


    Vec3 samplem(float u, float v) const {
        float tanTerm = std::tan(2.0f * PI * u) * a_y / a_x;
        float phi = std::atan(tanTerm);
        if (u >= 0.75) {
            phi += 2.0f * PI;
        }
        else if (u > 0.25) {
            phi += PI;
        }

        DebugLog("phi", phi);
        DebugLog("phi_a", 2.0f * PI * u);
        const float cos2phi = std::cos(phi) * std::cos(phi);
        const float sin2phi = std::sin(phi) * std::sin(phi);
        const float term_alpha = cos2phi / (a_x * a_x) + sin2phi / (a_y * a_y);
        DebugLog("x", a_x);
        DebugLog("y", a_y);

        DebugLog("alpha", term_alpha);
        DebugLog("cos2", cos2phi);
        DebugLog("sin2", sin2phi);
        // const float term = term_alpha * (1.0f - v) / v;
        const float term = v / ((1.0f - v) * term_alpha);
        const float theta = std::atan(std::sqrt(term));
        DebugLog("theta", theta);

        return Vec3(std::cos(phi) * std::sin(theta), std::cos(theta),
            std::sin(phi) * std::sin(theta));
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
        // Vec3 m = SphereSampling(sampler->sample(), sampler->sample(), pdf);
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

        // if (brdf[1] < 0.01f) {
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
        float G_ = this->G2(i, o);
        float D_ = this->D(m);

        Vec3 brdf = F * G_ * D_ / (4.0f * in * on);

        return brdf;
    }

    float samplePDF(const Vec3& wo, const Vec3& wi)const override {
        // return 0.5f * PI_INV;
        Vec3 m = normalize(wo + wi);
        if (m[1] < 0.0f) {
            DebugLog("m", m);
            DebugLog("wo", wo);
            DebugLog("wi", wi);
        }
        float D_ = this->D(m);
        float pdf = D_ * BSDFMath::cosTheta(m) / (4.0f * absdot(m, wo));
        return pdf;
    }
};