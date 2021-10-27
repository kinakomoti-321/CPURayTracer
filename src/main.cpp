#include <iostream>
#include "math/vec3.hpp"
#include "image/image.hpp"
int main()
{
	std::cout << "Hello World" << std::endl;
	const int width = 512;
	const int height = 512;
	Image img(width, height);

	for (int j = 0; j < width; ++j)
	{
		for (int i = 0; i < height; ++i)
		{
			float u = float(i) / width;
			float v = float(j) / height;
			Vec3 RGB(u, v, 1.0);
			img.setPixel(i, j, RGB);
		}
	}
	img.writePNG("output");
	return 0;
}