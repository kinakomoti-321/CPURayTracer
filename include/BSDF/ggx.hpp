#pragma once
#include "BSDF/bsdf.hpp"
#include "core/constant.hpp"
#include <iostream>
#include <cmath>

class GGX :public BSDF {

private:
    Vec3 F0;
    float alpha;
public:
    GGX(const Vec3& F0, const float alpha) :F0(F0), alpha(alpha) {}


    Vec3 samplingBSDF(const Vec3& wo, Vec3& wi, float& pdf,
        const std::shared_ptr<Sampler>& sampler) const override {

    };
    Vec3 evaluateBSDF(const Vec3& wo, const Vec3& wi) const = 0;
    float samplePDF(const Vec3& wi)const = 0;

};