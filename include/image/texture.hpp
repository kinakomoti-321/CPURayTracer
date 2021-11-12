#pragma once
#include "image/image.hpp"
#include <memory>
#include <string>
#include <algorithm>
class Texture{
    private:
    std::shared_ptr<Image> image;

    public:
    Texture(const std::string& filepath){

    }
    Texture(const Vec3& color){
        image = std::make_shared<Image>(1,1,color);
    }

    Vec3 getColor(const float u,const float v) const{
        unsigned int idxu = std::clamp(static_cast<int>(u * image->getWidth()),0,static_cast<int>(image->getWidth()));
        unsigned int idxv = std::clamp(static_cast<int>(v * image->getHeight()),0,static_cast<int>(image->getHeight()));

        return image->getPixel(idxu,idxv);
    }
};