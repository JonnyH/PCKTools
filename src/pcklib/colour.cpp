#include "colour.h"

bool Colour::isTransparent() const { return this->a == 0; }

bool Colour::operator==(const Colour &other) const
{
	return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
}
