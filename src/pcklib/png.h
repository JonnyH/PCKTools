#pragma once

#include "image.h"
#include <istream>

RGBAImage loadPNG(std::istream &input, size_t fileSize);
