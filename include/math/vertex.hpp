#pragma once

#include <vector>
#include "math/vec3.hpp"
namespace Vertex
{

inline void box(const Vec3& v1,const Vec3& v2,std::vector<float>& vertex,std::vector<float>& index){
    vertex = {v1[0],v1[1],v1[2], v1[0],v1[1],v2[2], v1[0],v2[1],v2[2], v1[0],v2[1],v1[2], v2[0],v1[1],v1[2], v2[0],v2[1],v1[2], v2[0],v2[1],v2[2], v2[0],v1[1],v2[2]};
    index = {1,2,3,1,3,4,1,6,4,1,5,6,4,7,3, 4,6,7, 5,7,6, 5,8,7, 5,1,8 ,1,2,8, 8,7,3, 8,3,2};
}

inline void invNormal(std::vector<float>& index){
    for(int i = 0; i < index.size(); i++){
        int idx = i * 3;
        unsigned int a = index[idx];
        index[idx] = index[idx + 2];
        index[idx+2] = a;
    } 

}
} // namespace name
