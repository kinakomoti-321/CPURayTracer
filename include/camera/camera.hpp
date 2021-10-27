#ifndef _CAMERA_H
#define _CAMERA_H

#include "math/vec3.hpp"
#include "core/ray.hpp"
class Camera
{
protected:
	Vec3 origin;
	Vec3 atlook;

public:
	Camera(const Vec3 &origin, const Vec3 &atlook) : origin(origin), atlook(atlook) {}
	virtual Ray getRay(float u, float v) {}
};
#endif