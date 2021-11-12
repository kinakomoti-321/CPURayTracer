//Light Sampling Test
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
#include <memory>
int main()
{
	std::cout << "Hello World" << std::endl;
	const int width = 512;
	const int height = 512;
	auto sphere1 = std::make_shared<Sphere>(Vec3(0.0),1.0f);
	auto sphere2 = std::make_shared<Sphere>(Vec3(3.0,0.0,0.0),1.0f);
	auto sphere3 = std::make_shared<Sphere>(Vec3(-3.0,0.0,0.0),1.0f);
	auto sphere4 = std::make_shared<Sphere>(Vec3(0,-1001.0,0.0),1000.0f);

	auto mat1 = std::make_shared<Lambert>(Vec3(0.8,0.2,0.2));
	auto mat2 = std::make_shared<Lambert>(Vec3(0.2,0.8,0.2));
	auto mat3 = std::make_shared<Lambert>(Vec3(0.2,0.2,0.8));
	auto mat4 = std::make_shared<Lambert>(Vec3(0.9));

	auto lit1 = std::make_shared<Light>(Vec3(1.0),1.0);
	
	auto obj1 = std::make_shared<Object>(sphere1,mat1);
	auto obj2 = std::make_shared<Object>(sphere2,mat2);
	auto obj3 = std::make_shared<Object>(sphere3,mat3,lit1);
	auto obj4 = std::make_shared<Object>(sphere4,mat4);

	Scene scene(obj1,std::make_shared<Sky>(Vec3(0.0)),std::make_shared<Sphere>(Vec3(0.0),1000.0f));
	scene.addObject(obj2);
	scene.addObject(obj3);
	scene.addObject(obj4);

	scene.build();

	const auto integrator = std::make_shared<NEE>();
	auto sampler = std::make_shared<RNGrandom>();
	
	Vec3 camPos = Vec3(0.0,0.0,-5.0);
	Vec3 camLook = Vec3(0.0,0.0,1.0);

	const auto camera = std::make_shared<Pinhole>(camPos,camLook,Vec2(2.0),1.0f);

	Renderer renderer(width,height,integrator,camera);
	renderer.render(scene,100,"test",sampler);

	return 0;
}