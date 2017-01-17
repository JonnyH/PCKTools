#include "pck.h"

void writeByte(const char c, std::ostream &output, size_t &totalSize)
{
	output.write(&c, 1);
	totalSize++;
}

static size_t writeUFOPCK(const PaletteImage &img, std::ostream &output)
{
	size_t size = 0;
	char c = 0;
	int x = 0;
	int y = 0;

	//First find the first non-transparent pixel
	while (img.getPixel(x, y) == 0)
	{
		x++;
		if (x >= img.width)
		{
			x = 0;
			y++;
		}
		if (y >= img.height)
		{
			//Completely empty image?
			break;
		}
	}
	
	//Check for Y skip being too large
	if (y > 253)
	{
		y = 253;
	}
	//The first byte is the initial Y skip
	writeByte(y, output, size);
	for (; y < img.height; y++)
	{
		for (x = 0; x < img.width; x++)
		{
			auto index = img.getPixel(x, y);
			if (index == 0xff || index == 0xff)
			{
				throw std::runtime_error("PaletteImage contains special value as index");
			}
			//TODO: RLE transparent pixels
			writeByte(index, output, size);
		}
	}
	//Write end-of-image
	writeByte(0xff, output, size);
	return size;
}

size_t writePCK(const PaletteImage &img, std::ostream &output, PCKFormat format)
{
	switch (format)
	{
		case PCKFormat::UFO:
			return writeUFOPCK(img, output);
	}
}
