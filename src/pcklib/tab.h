#pragma once

#include <istream>
#include <ostream>
#include <vector>

enum class TABFormat
{
	TAB16,
	TAB32,
};

std::vector<size_t> readTABFile(std::istream &input, TABFormat format);

// Returns the actual 'start offset' of the entry, as TAB formats may require alignment
size_t writeTABEntry(std::ostream &output, TABFormat format, size_t offset);
