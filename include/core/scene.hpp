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
        std::shared_ptr<Sky> sky;
        std::shared_ptr<Shape> skySphere;
    public:
        Scene(std::shared_ptr<Object> obj,std::shared_ptr<Sky> sky,std::shared_ptr<Shape> skySphere): sky(sky),skySphere(skySphere){
            geometry.push_back(obj);
        } 

        void addObject(std::shared_ptr<Object> obj){
            geometry.push_back(obj);
            skySphere = std::make_shared<Sphere>(Vec3(0),10000.0f);
        }
        
        void build(){
            if(sky == nullptr || skySphere == nullptr){
               std::cout << "Scene build error: not define sky or skySohere" << std::endl; 
               sky = std::make_shared<Sky>(Vec3(1.0));
               skySphere = std::make_shared<Sphere>(Vec3(0.0),10000.0f);
            }
        }

        bool intersect(const Ray& ray,IntersectInfo& info)const{
            bool ishit = false;
            for(int i = 0; i < geometry.size(); i++){
                IntersectInfo hitinfo;
                if(geometry[i]->intersection(ray,hitinfo)){
                    if(hitinfo.distance < info.distance){
                        info = hitinfo;
                        ishit = true;
                    }     
                }
            }
            return ishit;
        }

        Vec3 skyLe(const Ray& ray)const {
            IntersectInfo info;
            skySphere->intersect(ray,info);
            return sky->Le(info.uv.first,info.uv.second);
        }
};