#include "core/ray.hpp"
#include <iostream>
#include <memory>
#include "image/image.hpp"
#include "camera/pinhole.hpp"
#include "sampling/rng.hpp"
int main(){
    unsigned int width = 512;
    unsigned int height = 512;

    Image img(width,height);

    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            Pinhole camera(Vec3(0,0,0),Vec3(0,0,-1),Vec2(2.0),1.0f);
            Vec2 uv;
            uv[0]= (2.0f * i - width) / height;
            uv[1]= (2.0f * j - width) / height;
            float camWaight;
            std::shared_ptr<Sampler> sampler = std::make_shared<RNGrandom>(1);
            const Ray ray = camera.getRay(uv,sampler,camWaight);

            img.setPixel(i,j, 0.5f * (ray.direction + Vec3(1.0f)));
        }
    }
    std::cout << "rayTest" << std::endl;
    img.writePNG("test");

    return 0;
}