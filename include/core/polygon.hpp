#pragma once 
#include "shape/triangle.hpp"
#include "core/object.hpp"
#include "BSDF/bsdf.hpp"
#include "light/light.hpp"
#include "math/vec3.hpp"
#include "core/modelloader.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <string>
class Polygon {
private:
    std::vector<std::shared_ptr<Triangle>> poly;
    std::shared_ptr<BSDF> bsdf;
    std::shared_ptr<Light> light;
    unsigned int nface;

public:
    Polygon(const std::vector<float>& vertex, const std::vector<float>& index, const std::shared_ptr<BSDF>& bsdf, const std::shared_ptr<Light>& light = nullptr) :bsdf(bsdf), light(light) {
        nface = index.size() / 3;
        for (int i = 0; i < index.size() / 3; i++) {
            TriInfo info;
            for (int k = 0; k < 3; k++) {
                unsigned int idx = index[i * 3 + k] * 3;
                Vec3 point = Vec3(vertex[idx], vertex[idx + 1], vertex[idx + 2]);
                std::cout << point << std::endl;
                info.point[k] = point;
            }
            std::cout << std::endl;
            poly.push_back(std::make_shared<Triangle>(info));
        }
    }

    Polygon(const std::string& filepath, const std::shared_ptr<BSDF>& bsdf, const std::shared_ptr<Light>& light = nullptr) :bsdf(bsdf), light(light) {
        std::vector<float> vertex;
        std::vector<unsigned int> index;
        std::vector<float> normal;
        std::vector<float> uv;

        loadObj(filepath, vertex, index, normal, uv);

        nface = index.size() / 3;
        for (int i = 0; i < index.size() / 3; i++) {
            TriInfo info;
            for (int k = 0; k < 3; k++) {
                unsigned int idx = index[i * 3 + k] * 3;
                Vec3 point = Vec3(vertex[idx], vertex[idx + 1], vertex[idx + 2]);
                std::cout << point << std::endl;
                info.point[k] = point;
            }
            std::cout << std::endl;
            poly.push_back(std::make_shared<Triangle>(info));
        }
    }

    std::shared_ptr<Triangle> getMesh(unsigned int FaceID) {
        if (FaceID > nface) return nullptr;
        return poly[FaceID];
    }

    std::shared_ptr<Light> getLight() {
        return light;
    }

    std::shared_ptr<BSDF> getBSDF() {
        return bsdf;
    }

    unsigned int nFace() {
        return nface;
    }
};