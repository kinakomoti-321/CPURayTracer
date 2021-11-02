#pragma once 

#include <iostream>
#include "core/object.hpp"
#include <memory>
#include <vector>
#include "core/ray.hpp"
#include "intersection/intersectinfo.h"
#include "light/sky.hpp"
#include "shape/shere.hpp"
#include "shape/shape.hpp"
class Scene{
    private:
        std::vector<std::shared_ptr<Object>>  geometry;
        std::vector<std::shared_ptr<Object>>  lightgeo;
        std::shared_ptr<Sky> sky;
        std::shared_ptr<Shape> skySphere;
    public:
        Scene(const std::shared_ptr<Object>& obj,const std::shared_ptr<Sky>& sky = nullptr,const std::shared_ptr<Shape>& skySphere = nullptr): sky(sky),skySphere(skySphere){
            geometry.push_back(obj);
        } 

        void addObject(const std::shared_ptr<Object>& obj){
            geometry.push_back(obj);
        }
        
        void build(){
            if(sky == nullptr || skySphere == nullptr){
               std::cout << "Scene build error: not define sky or skySohere" << std::endl; 
               sky = std::make_shared<Sky>(Vec3(1.0));
               skySphere = std::make_shared<Sphere>(Vec3(0.0),10000.0f);
            }

            int count = 0;
            for(int i = 0 ; i < geometry.size(); i++){
                if(geometry[i]->hasLight()){
                    lightgeo.push_back(geometry[i]);
                    ++count;
                }
            }

            std::cout << "LightSource : " << count << std::endl;
            std::cout << "Primitive : " << geometry.size() << std::endl;

            std::cout << "Build_Complete" <<std::endl;
        }

        bool intersect(const Ray& ray,IntersectInfo& info)const{
            bool ishit = false;
            for(int i = 0; i < geometry.size(); i++){
                IntersectInfo hitinfo;
                if(geometry[i]->intersection(ray,hitinfo)){
                    if(hitinfo.distance < info.distance){
                        info = hitinfo;
                        ishit = true;
                        // std::cout << "hit" << std::endl;
                    }     
                }
            }
            return ishit;
        }

        Vec3 skyLe(const Ray& ray)const {
            IntersectInfo info;
            skySphere->intersect(ray,info);
            return sky->Le(info.uv[0],info.uv[1]);
        }

        Vec3 lightPointSampling(Sampler& sampler,IntersectInfo& info){
            int idx = std::clamp(static_cast<int>(lightgeo.size() * sampler.sample()) - 1,0,static_cast<int>(lightgeo.size())-1);
            return lightgeo[idx]->areaSampling(sampler,info);
        }
};