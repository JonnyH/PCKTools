#pragma once

#include "colour.h"
#include <istream>
#include <vector>

enum class PaletteConversion
{
	None,
	LeastSquareDifference,
};

class Palette
{
  public:
	std::vector<Colour> entries;

	const Colour &get(int index) const;
	void set(int index, const Colour &c);
	uint8_t getIndex(const Colour &c, PaletteConversion conversionMethod) const;
};

Palette loadPalette(std::istream &input, bool expandPalette);
