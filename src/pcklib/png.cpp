#include "png.h"
#include "lodepng/lodepng.h"
#include <cassert>
#include <memory>

RGBAImage loadPNG(std::istream &input, size_t fileSize)
{
	lodepng::State png_state;
	unsigned int width, height;

	// Lodepng requires we read everything into memory up front
	std::unique_ptr<unsigned char[]> fileData(new unsigned char[fileSize]);
	input.read((char *)fileData.get(), fileSize);
	if (!input)
	{
		throw std::runtime_error("Unexpected EOF reading png file");
	}
	unsigned err = lodepng_inspect(&width, &height, &png_state, fileData.get(), fileSize);
	if (err)
	{
		throw std::runtime_error("Failed to read PNG header");
	}

	std::vector<unsigned char> rgbaData;

	err = lodepng::decode(rgbaData, width, height, fileData.get(), fileSize);
	if (err)
	{
		throw std::runtime_error("Failed to decode PNG");
	}

	assert(rgbaData.size() == width * height * 4);

	RGBAImage img(width, height);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// lodepng outputs packed RGBA bytes
			size_t offset = 4 * (width * y) + x;
			uint8_t r = rgbaData[offset + 0];
			uint8_t g = rgbaData[offset + 1];
			uint8_t b = rgbaData[offset + 2];
			uint8_t a = rgbaData[offset + 3];
			img.setPixel(x, y, {r, g, b, a});
		}
	}
	return img;
}
