#include "palette.h"
#include <limits>
#include <stdexcept>

Palette loadPalette(std::istream &input, bool expandPalette)
{
	Palette p;
	for (int count = 0; count < 256; count++)
	{
		uint8_t r, g, b;
		Colour c;

		input.get((char *)&r, 1);
		input.get((char *)&g, 1);
		input.get((char *)&b, 1);

		if (count == 0)
		{
			c.r = 0;
			c.g = 0;
			c.b = 0;
			c.a = 0;
		}
		else if (expandPalette)
		{
			c.r = (r << 2 | r >> 6);
			c.g = (g << 2 | g >> 6);
			c.b = (b << 2 | b >> 6);
			c.a = 255;
		}
		else
		{
			c.r = r;
			c.g = g;
			c.b = b;
			c.a = 255;
		}
		if (!input)
		{
			throw std::runtime_error("Unexpected end of palette file");
		}
		p.entries.push_back(c);
	}
	return p;
}

const Colour &Palette::get(int index) const
{
	if (index >= entries.size())
	{
		throw std::runtime_error("Palette index out of range");
	}
	return entries.at(index);
}

void Palette::set(int index, const Colour &c)
{
	if (index >= entries.size())
	{
		throw std::runtime_error("Palette index out of range");
	}
	entries[index] = c;
}

static uint8_t getPaletteIndexNoConversion(const Colour &c, const Palette &pal)
{
	if (c.isTransparent())
		return 0;

	for (int i = 0; i < pal.entries.size(); i++)
	{
		if (pal.entries.at(i) == c)
			return i;
	}
	throw std::runtime_error("No matching colour found and no conversion method set");
	return 0;
}

constexpr static float squareDifference(float a, float b) { return (a - b) * (a - b); }

constexpr static float calculateColourSquareDifference(const Colour &ca, const Colour &cb)
{
	return squareDifference(ca.r, cb.r) + squareDifference(ca.g, cb.g) +
	       squareDifference(ca.b, cb.b);
}

static uint8_t getPaletteIndexLeastSquare(const Colour &c, const Palette &pal)
{
	if (c.isTransparent())
		return 0;

	float currentBestDifference = std::numeric_limits<float>::max();
	uint8_t currentBestIndex = 0;

	for (int i = 0; i < pal.entries.size(); i++)
	{
		auto difference = calculateColourSquareDifference(c, pal.entries.at(i));
		if (difference < currentBestDifference)
		{
			currentBestIndex = i;
			currentBestDifference = difference;
		}
	}
	return currentBestIndex;
}

uint8_t Palette::getIndex(const Colour &c, PaletteConversion conversionMethod) const
{
	switch (conversionMethod)
	{
		case PaletteConversion::None:
			return getPaletteIndexNoConversion(c, *this);
		case PaletteConversion::LeastSquareDifference:
			return getPaletteIndexLeastSquare(c, *this);
	}
}
