#ifndef _CAMERA_H
#define _CAMERA_H

#include "math/vec3.hpp"
#include "core/ray.hpp"
#include "sampling/sampler.hpp"
#include "math/vec2.hpp"
#include <memory>
class Camera
{
public:
	virtual Ray getRay(const Vec2& uv,const std::shared_ptr<Sampler>& sampler,float& weight) const = 0;
};
#endif