#pragma once
#include "camera/camera.hpp"
#include "math/vec3.hpp"
#include "math/vec2.hpp"
#include "core/ray.hpp"
class Pinhole : public Camera
{
	private:
	const Vec3 origin;
	const Vec3 atlook;
	const Vec2 sensorW;
	const float f;

	Vec3 ly;
	Vec3 lx;

public:
	Pinhole(const Vec3 &origin, const Vec3 &atlook,const Vec2& sensorW,float f):origin(origin),atlook(atlook),sensorW(sensorW),f(f){
		lx = normalize(cross(atlook,Vec3(0,1,0)));
		ly = normalize(cross(lx,atlook));
	}

	Ray getRay(const Vec2& uv,std::shared_ptr<Sampler>& sampler,float& weight){
		Vec3 sensor_pos = origin + uv[0] * sensorW[0] * lx + uv[1] * sensorW[1] * ly;
		Vec3 direction = normalize(origin + f * atlook -  sensor_pos);
		weight = 1.0f;
		return Ray(sensor_pos,direction);
	}

};