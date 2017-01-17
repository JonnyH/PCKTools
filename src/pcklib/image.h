#pragma once

#include "colour.h"
#include "palette.h"
#include <istream>
#include <vector>

class Palette;
class PaletteImage;

class RGBAImage
{
  private:
	std::vector<Colour> pixels;

  public:
	RGBAImage(int width, int height);

	int width;
	int height;

	const Colour &getPixel(int x, int y) const;
	void setPixel(int x, int y, const Colour &c);

	PaletteImage toPaletteImage(const Palette &palette, PaletteConversion conversion) const;
};

class PaletteImage
{
  private:
	std::vector<uint8_t> pixels;

  public:
	PaletteImage(int width, int height);
	int width;
	int height;

	uint8_t getPixel(int x, int y) const;
	void setPixel(int x, int y, uint8_t p);
};
