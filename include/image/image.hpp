#ifndef _IMAGE_H
#define _IMAGE_H

#include "math/vec3.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

class Image
{
private:
	unsigned int width;
	unsigned int height;
	float *pixel;

public:
	Image(unsigned int width, unsigned int height) : width(width), height(height)
	{
		pixel = new float[width * height * 3];

		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				const int idx = i * 3 + 3 * width * j;
				pixel[idx] = 0;
				pixel[idx + 1] = 0;
				pixel[idx + 2] = 0;
			}
		}
	}

	~Image()
	{
		delete[] pixel;
	}

	void setPixel(unsigned int i, unsigned int j, Vec3 &RGB)
	{
		const int idx = i * 3 + 3 * width * j;
		pixel[idx] = RGB[0];
		pixel[idx + 1] = RGB[1];
		pixel[idx + 2] = RGB[2];
	}

	void writePNG(std::string filename)
	{
		std::ofstream file(filename + ".ppm");
		if (!file)
		{
			std::cerr << "failed to open " << filename << std::endl;
			return;
		}

		file << "P3" << std::endl;
		file << width << " " << height << std::endl;
		file << "255" << std::endl;
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				const int idx = 3 * i + 3 * width * j;
				float R = pixel[idx];
				float G = pixel[idx + 1];
				float B = pixel[idx + 2];

				// 各成分を[0, 255]に含まれるように変換し出力
				file << static_cast<unsigned int>(std::clamp(255.0f * R, 0.0f, 255.0f))
				     << " ";
				file << static_cast<unsigned int>(std::clamp(255.0f * G, 0.0f, 255.0f))
				     << " ";
				file << static_cast<unsigned int>(std::clamp(255.0f * B, 0.0f, 255.0f))
				     << std::endl;
			}
		}
		file.close();
		std::string cmd = "convert " + filename + ".ppm " + filename + ".png";
		system(cmd.c_str());
	}

	// ガンマ補正
	void gammaCorrection()
	{
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				const int idx = 3 * i + 3 * width * j;
				pixel[idx] = std::pow(pixel[idx], 1 / 2.2f);	     // R
				pixel[idx + 1] = std::pow(pixel[idx + 1], 1 / 2.2f); // G
				pixel[idx + 2] = std::pow(pixel[idx + 2], 1 / 2.2f); // B
			}
		}
	}
};
#endif