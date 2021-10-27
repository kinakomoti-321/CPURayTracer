#pragma once
#include <iostream>
#include <memory>

#include "include/camera/camera.hpp"
#include "include/core/object.hpp"
#include "include/core/scene.hpp"
#include "include/image/image.hpp"
#include "include/integrator/integrator.hpp"
class Renderer {
 private:
  unsigned int width, height;
  std::shared_ptr<Integrator> integrator;
  std::shared_ptr<Scene> scene;
  std::shared_ptr<Camera> camera;

 public:
  Renderer(unsigned int width, unsigned int height,
           std::shared_ptr<Integrator> integrator, std::shared_ptr<Scene> scene,
           std::shared_ptr<Camera> camera)
      : width(width),
        height(height),
        integrator(integrator),
        scene(scene),
        camera(camera){};

  void render(int sampling) {}
};