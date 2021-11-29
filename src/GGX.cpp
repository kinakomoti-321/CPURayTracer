
#include <iostream>
#include "math/vec3.hpp"
#include "integrator/nee.hpp"
#include "camera/pinhole.hpp"
#include "image/image.hpp"
#include "core/scene.hpp"
#include "core/object.hpp"
#include "core/renderer.hpp"
#include "integrator/normalchecker.hpp"
#include "integrator/pathtracer.hpp"
#include "light/sky.hpp"
#include "light/light.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include "sampling/rng.hpp"
#include "shape/shere.hpp"
#include "BSDF/lambert.hpp"
#include "core/polygon.hpp"
#include "math/vertex.hpp"
#include "BSDF/grass.hpp"
#include "BSDF/specular.hpp"
#include "integrator/mis.hpp"
#include "BSDF/ggx.hpp"
#include <memory>
#include <vector>

int main()
{
    std::cout << "Hello World" << std::endl;
    const int width = 512;
    const int height = 512;
    std::vector<float> vex = { 1,0,1,1,0,-1,-1,0,-1,-1,0,1 };
    std::vector<float> idx = { 0,1,2,0,2,3 };

    std::vector<float> vex1;
    std::vector<float> idx1;
    Vertex::box(Vec3(5), Vec3(-5), vex1, idx1);
    idx1 = { 4,3,7, 4,7,6, 1,8,2,1,5,8,2,8,3,3,8,7 };
    std::vector<float> idx2 = { 1,3,2,1,4,3 };
    std::vector<float> idx3 = { 5,6,8,6,7,8 };

    auto sphere1 = std::make_shared<Sphere>(Vec3(0.0, 1.0, -1.0), 1.0f);
    auto sphere2 = std::make_shared<Sphere>(Vec3(0.0, 1.5, 3.0), 1.5f);
    auto sphere3 = std::make_shared<Sphere>(Vec3(0.0, 3.0, 0.0), 2.0f);
    auto sphere4 = std::make_shared<Sphere>(Vec3(0.0, 1.5, -3.0), 1.5f);
    auto sphere5 = std::make_shared<Sphere>(Vec3(0.0, 1.5, 0.0), 1.5f);

    auto mat1 = std::make_shared<Lambert>(Vec3(1.0, 0.2, 0.2));
    auto mat2 = std::make_shared<Lambert>(Vec3(0.2, 1.0, 0.2));
    auto mat3 = std::make_shared<Lambert>(Vec3(0.2, 0.2, 1.0));
    auto mat4 = std::make_shared<Lambert>(Vec3(0.9));

    auto mat5 = std::make_shared<Specular>(Vec3(0.9));
    auto mat6 = std::make_shared<Grass>(Vec3(0.9), 1.5f);
    auto mat7 = std::make_shared<GGX>(Vec3(0.9), 0.1f);
    auto mat8 = std::make_shared<GGX>(Vec3(0.9), 0.3f);
    auto mat9 = std::make_shared<GGX>(Vec3(0.9), 0.5f);


    auto lit1 = std::make_shared<Light>(Vec3(1.0, 0.9, 0.9), 5.0);

    auto obj1 = std::make_shared<Object>(sphere1, mat1);
    auto obj2 = std::make_shared<Object>(sphere2, mat7);
    auto obj3 = std::make_shared<Object>(sphere3, mat6);
    auto obj4 = std::make_shared<Object>(sphere4, mat9);
    auto obj5 = std::make_shared<Object>(sphere5, mat8);
    // auto obj4 = std::make_shared<Object>(sphere4,mat4);
    auto mesh = std::make_shared<Polygon>("cornelBox.obj", mat4);
    auto mesh1 = std::make_shared<Polygon>("cornelBox_L.obj", mat1);
    auto mesh2 = std::make_shared<Polygon>("cornelBox_R.obj", mat2);
    auto mesh3 = std::make_shared<Polygon>("poly_Le.obj", mat4, lit1);
    auto mesh4 = std::make_shared<Polygon>("Box1.obj", mat4);
    auto mesh5 = std::make_shared<Polygon>("Box2.obj", mat5);
    Scene scene(std::make_shared<Sky>(Vec3(0.0)), std::make_shared<Sphere>(Vec3(0.0), 1000.0f));
    scene.addObject(obj2);
    scene.addObject(obj4);
    scene.addObject(obj5);
    // scene.addObject(obj1);
    // scene.addObject(obj3);
    // scene.addObject(obj4);
    // scene.addObject(obj5);
    scene.addPolygon(mesh);
    scene.addPolygon(mesh1);
    scene.addPolygon(mesh2);
    // scene.addObject(obj6);
    scene.addPolygon(mesh3);
    // scene.addPolygon(mesh4);
    // scene.addPolygon(mesh5);

    scene.build();

    const auto mis = std::make_shared<MIS>();
    const auto nee = std::make_shared<NEE>();
    const auto pathtracer = std::make_shared<Pathtracer>();
    const auto normalcheck = std::make_shared<NormalChecker>();
    auto sampler = std::make_shared<RNGrandom>();

    Vec3 camPos = Vec3(15.0, 5, 0.0);
    Vec3 camLook = Vec3(-1.0, 0.0, 0.0);

    const auto camera = std::make_shared<Pinhole>(camPos, camLook, Vec2(2.0), 2.0f);

    Renderer renderer(width, height, mis, camera);
    renderer.timeRender(scene, 50000, "GGXnee_01", sampler);

    // renderer.reset(width, height, nee, camera);
    // renderer.timeRender(scene, 50000, "nee_01", sampler);

    // renderer.reset(width, height, pathtracer, camera);
    // renderer.timeRender(scene, 50000, "pt_01", sampler);
    return 0;
}