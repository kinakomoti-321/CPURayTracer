#pragma once

#include "sampling/sampler.hpp"
#include "core/constant.hpp"
#include <memory>
#include <cmath>

class Volume {
public:
    virtual float sampleS(std::shared_ptr<Sampler>& Sampler) const = 0;
    virtual Vec3 phaseFanc(const Vec3& wo, std::shared_ptr<Sampler>& Sampler)const = 0;
    virtual float getmueA()const = 0;
    virtual float getmueS()const = 0;
    virtual float getmueT()const = 0;
    virtual Vec3 Le() const = 0;
};

class HomoVolume : public Volume {

private:
    float mueA;
    float mueS;
    float mueT;
    float phaseG;
    Vec3 le;

public:
    HomoVolume(float mueA, float mueS, float phaseG, const Vec3& le) :mueA(mueA), mueS(mueS), phaseG(phaseG), le(le) {
        mueT = mueA + mueS;
    }
    float sampleS(std::shared_ptr<Sampler>& sampler) const override {
        float random = sampler->sample();
        return -std::log(std::max(1.0f - random, 0.0f)) / mueT;
    };
    Vec3 phaseFanc(const Vec3& wo, std::shared_ptr<Sampler>& Sampler)const override {
        Vec3 wi;
        //woの接空間
        Vec3 b, t;
        tangentSpaceBasis(wo, t, b);

        float u = 2.0f * Sampler->sample() - 1.0f;
        float v = Sampler->sample();

        float g = phaseG;
        float cosTheta = 0.0f;
        if (g != 0.0f) {
            float sqrTerm = (1.0f - g * g) / (1.0f + g * u);
            cosTheta = (1.0f + g * g - sqrTerm * sqrTerm) / (2.0f * g);
        }
        else {
            cosTheta = u;
        }
        float sinTheta = std::sqrt(std::max(1.0f - cosTheta * cosTheta, 0.0f));

        float phi = 2.0f * M_PI * v;
        wi = Vec3(sinTheta * std::cos(phi), cosTheta, sinTheta * std::sin(phi));
        DebugLog("wi", wi);
        DebugLog("contheta", cosTheta);
        DebugLog("sintheta", sinTheta);
        DebugLog("u", u);
        DebugLog("v", v);
        DebugLog("g", g);

        wi = localToWorld(wi, t, wo, b);

        return wi;
    };
    float getmueA()const override {
        return mueA;
    };
    float getmueS()const override {
        return mueS;
    };
    float getmueT()const override {
        return mueT;
    };
    Vec3 Le() const override {
        return le;
    };
};