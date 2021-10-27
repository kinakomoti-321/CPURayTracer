#ifndef _PINHOLE_H
#define _PINHOLE_H

#include "camera/camera.hpp"

class Pinhole : protected Camera
{
public:
	Pinhole(const Vec3 &origin, const Vec3 &atlook)
	{
		this->origin = origin;
		this->atlook = atlook;
	}
};
#endif