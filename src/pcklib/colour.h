#pragma once

#include <cstdint>

class Colour
{
  public:
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 255;

	bool isTransparent() const;
	bool operator==(const Colour &other) const;
};
