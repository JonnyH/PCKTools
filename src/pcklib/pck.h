#pragma once

#include "colour.h"
#include "image.h"
#include <istream>

enum class PCKFormat
{
	UFO,
};

PaletteImage loadPCK(std::istream &input, PCKFormat format);

// Returns the number of bytes written
size_t writePCK(const PaletteImage &img, std::ostream &output, PCKFormat format);
