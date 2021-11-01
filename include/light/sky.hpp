#pragma once 
#include "image/texture.hpp"
#include <memory>
class Sky{
    private:
    std::shared_ptr<Texture> tex;


    public:
    Sky(std::shared_ptr<Texture> tex): tex(tex){}
    Sky(const Vec3& color){
        tex = std::make_shared<Texture>(color);
    }

    Vec3 Le(const float u,const float v)const{
        return tex->getColor(u,v);
    }

};