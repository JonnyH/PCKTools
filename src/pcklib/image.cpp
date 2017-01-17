#include "image.h"
#include <stdexcept>

RGBAImage::RGBAImage(int width, int height) : width(width), height(height)
{
	pixels.resize(width * height);
}

const Colour &RGBAImage::getPixel(int x, int y) const
{
	if (x < 0 || x >= this->width || y < 0 || y >= this->height)
	{
		throw std::runtime_error("getPixel out of bounds");
	}
	return this->pixels[y * this->width + x];
}

void RGBAImage::setPixel(int x, int y, const Colour &c)
{
	if (x < 0 || x >= this->width || y < 0 || y >= this->height)
	{
		throw std::runtime_error("getPixel out of bounds");
	}
	this->pixels[y * this->width + x] = c;
}

PaletteImage::PaletteImage(int width, int height) : width(width), height(height)
{
	pixels.resize(width * height);
}

uint8_t PaletteImage::getPixel(int x, int y) const
{
	if (x < 0 || x >= this->width || y < 0 || y >= this->height)
	{
		throw std::runtime_error("getPixel out of bounds");
	}
	return this->pixels[y * this->width + x];
}

void PaletteImage::setPixel(int x, int y, uint8_t p)
{
	if (x < 0 || x >= this->width || y < 0 || y >= this->height)
	{
		throw std::runtime_error("getPixel out of bounds");
	}
	this->pixels[y * this->width + x] = p;
}

PaletteImage RGBAImage::toPaletteImage(const Palette &palette, PaletteConversion conversion) const
{
	PaletteImage newImage(this->width, this->height);

	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
			newImage.setPixel(x, y, palette.getIndex(this->getPixel(x, y), conversion));
		}
	}
	return newImage;
}
