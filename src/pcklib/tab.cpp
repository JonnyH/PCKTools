#include "tab.h"
#include <limits>

static constexpr size_t alignup(size_t input, size_t alignment)
{
	return ((input + (alignment - 1)) & ~(alignment - 1));
}

static_assert(alignup(5, 4) == 8, "alignup test failed");
static_assert(alignup(5, 2) == 6, "alignup test failed");
static_assert(alignup(4, 4) == 4, "alignup test failed");
static_assert(alignup(4, 2) == 4, "alignup test failed");
static_assert(alignup(0, 4) == 0, "alignup test failed");

size_t writeTABEntry(std::ostream &output, TABFormat format, size_t offset)
{
	switch (format)
	{
		case TABFormat::TAB16:
		{
			offset = alignup(offset, 4);
			if (offset >= std::numeric_limits<uint16_t>::max())
			{
				throw std::runtime_error("Offset too large for TAB16 format");
			}
			uint16_t value = offset;
			output.write((char *)&value, 2);
			return offset;
		}
		case TABFormat::TAB32:
		{
			offset = alignup(offset, 4);
			if (offset >= std::numeric_limits<uint32_t>::max())
			{
				throw std::runtime_error("Offset too large for TAB32 format");
			}
			uint32_t value = offset;
			output.write((char *)&value, 4);
			return offset;
		}
	}
}
