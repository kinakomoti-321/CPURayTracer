#include "BSDF/bsdf.hpp"
#include "BSDF/ggx.hpp"
#include "sampling/sampler.hpp"
#include "BSDF/lambert.hpp"
#include "sampling/rng.hpp"
#include <memory.h>
#include <iostream>
int main() {

    int Sampling = 1000000;
    auto BSDF = std::make_shared<GGX>(Vec3(0.9), 0.6f);
    RNGrandom rng(1000);
    float Dcheck = 0.0;
    float pdf;
    Vec3 wo = SphereSampling(rng.sample(), rng.sample(), pdf);
    for (int i = 0; i < Sampling; i++) {
        Vec3 wi = SphereSampling(rng.sample(), rng.sample(), pdf);
        Vec3 bsdf = BSDF->evaluateBSDF(wo, wi);
        Vec3 half = wi;
        float D = BSDF->D(wi);
        // DebugLog("D * half[1]", D * half[1]);
        // DebugLog("h", half);
        Dcheck += D * half[1];

        bool nancheck = std::isnan(bsdf[0]) || std::isnan(bsdf[1]) || std::isnan(bsdf[2]);
        bool infcheck = std::isinf(bsdf[0]) || std::isinf(bsdf[1]) || std::isinf(bsdf[2]);
        bool minuscheck = bsdf[0] <= 0.0f || bsdf[1] <= 0.0f || bsdf[2] <= 0.0f;
        bool bsdfover = bsdf[0] >= 1.0f || bsdf[1] >= 1.0f || bsdf[2] >= 1.0f;
        // DebugLog("test");

        // DebugLog("wo", wo);
        // DebugLog("wi", wi);
        // DebugLog("BSDF", bsdf);
        if (nancheck || infcheck || minuscheck) {
            DebugLog("wo", wo);
            DebugLog("wi", wi);
            if (nancheck) DebugLog("nan");
            if (infcheck) DebugLog("inf");
            if (minuscheck) DebugLog("minus");
        }
    }

    DebugLog("Dcheck", Dcheck);


    return 0;
}