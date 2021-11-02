#pragma once
#include <iostream>
#include <memory>
#include <omp.h>

#include "camera/camera.hpp"
#include "core/object.hpp"
#include "core/scene.hpp"
#include "image/image.hpp"
#include "integrator/integrator.hpp"
#include "math/vec2.hpp"
#include "image/image.hpp"
#include <string>
#include <chrono>
class Renderer {
 private:
  unsigned int width, height;
  std::shared_ptr<Integrator> integrator;
  std::shared_ptr<Camera> camera;
  std::shared_ptr<Image> img;
 public:
  Renderer(unsigned int width, unsigned int height,
           std::shared_ptr<Integrator> integrator, 
           std::shared_ptr<Camera> camera)
      : width(width),
        height(height),
        integrator(integrator),
        camera(camera){
          img = std::make_shared<Image>(width,height);
        };

  void reset(unsigned int width, unsigned int height,
           std::shared_ptr<Integrator> integrator, 
           std::shared_ptr<Camera> camera)
  { 
    this->width = width;
    this->height = height;
    this->integrator = integrator;
    this->camera = camera;
    img = std::make_shared<Image>(width,height);
  }

  void render(const Scene& scene,int sampling,const std::string& filename,std::shared_ptr<Sampler> sampler) {
    std::cout << "Render Start" << std::endl;
    auto start = std::chrono::system_clock::now();
    
    #pragma omp parallel for schedule(dynamic,1)
    for(int i = 0; i < width; i++){
      for(int j = 0; j < height; j++){
        Vec3 sumRadiance(0.0);
        sampler->reset(i + width * j);
        // std::cout << i << " : " << j << ": samplingStart" << std::endl;
        for(int k = 0; k < sampling; k++){
          Vec2 cameraUV;
          cameraUV[0] = (2.0f * (i+sampler->sample() - 0.5f) -width) / height;
          cameraUV[1] = (2.0f * (j + sampler->sample() - 0.5f) - height)/ height;
          float cweight;
          Ray cameraray = camera->getRay(cameraUV,sampler,cweight);
          // std::cout << "Integrate Start" << std::endl;
          sumRadiance += cweight * integrator->integrate(cameraray,scene,sampler); 
        } 
      
        // std::cout << i << " : " << j << ": sampling End" << std::endl;
        Vec3 radiance = sumRadiance / static_cast<float>(sampling);
        img->setPixel(i,j,radiance);    
      }
    }
  auto end = std::chrono::system_clock::now();
  std::cout << "End :"
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     start).count()
            << "ms" << std::endl;
    img->writePNG(filename);
  }
};