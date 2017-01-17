#include "pcklib/colour.h"
#include "pcklib/image.h"
#include "pcklib/pck.h"
#include "pcklib/png.h"
#include "pcklib/tab.h"
#include <boost/program_options.hpp>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

namespace po = boost::program_options;

static const std::map<std::string, PCKFormat> pckFormats = {
    {"UFO", PCKFormat::UFO},
};
static const std::map<std::string, TABFormat> tabFormats = {
    {"TAB16", TABFormat::TAB16}, {"TAB32", TABFormat::TAB32},
};

static const std::map<std::string, PaletteConversion> paletteConversions = {
    {"None", PaletteConversion::None},
    {"LeastSquareDifference", PaletteConversion::LeastSquareDifference},
};

int main(int argc, char **argv)
{
	bool verbose = false;
	bool expandPalette = true;

	po::options_description desc("Usage: png2pck -o output.pck -t output.tab -p palette.pal "
	                             "[options...] inputfile1.png (inputfile2.png ...)\nOptions");

	// clang-format doesn't behave well for these boost::program_options () operators
	// clang-format off
	desc.add_options()
		("help", "Show help message and exit")
		("format", po::value<std::string>(), "Output PCK format (default: UFO)")
		("list-formats", "List valid output PCK formats")
		("tabformat",po::value<std::string>(), "Output TAB format (default: TAB16)")
		("list-tabformats", "List valid output TAB formats and exit and exit")
		("output,o",po::value<std::string>(), "Set output .pck file (required)")
		("outtab,t",po::value<std::string>(), "Set output .tab file (required)")
		("palette,p",po::value<std::string>(),
			"Set palette to map from RGB (required) - note, intex '0' is always considered transparent")
		("palette-conversion", po::value<std::string>(),
			"Set the method to use if no exact palette match found (default: NONE)") 
		("list-palette-conversions", "List valid palette-conversions and exit")
		("verbose,v", "Print info messages")
		("expand-vga-palete", po::value<bool>(&expandPalette)->default_value(true),
			"Specified palette containes 6bit 'vga' RGB entries")
		("input,i", po::value<std::vector<std::string>>(), "Input .png files");

	// clang-format on

	po::positional_options_description p;
	p.add("input", -1);
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	PCKFormat pckFormat = PCKFormat::UFO;
	TABFormat tabFormat = TABFormat::TAB16;
	PaletteConversion paletteConversion = PaletteConversion::None;

	if (vm.count("help"))
	{
		std::cout << desc << "\n";
		return EXIT_SUCCESS;
	}

	if (vm.count("list-formats"))
	{
		std::cout << "Valid PCK formats:\n";
		for (auto &f : pckFormats)
		{
			std::cout << "\t" << f.first << "\n";
		}
		return EXIT_SUCCESS;
	}

	if (vm.count("list-tabformats"))
	{
		std::cout << "Valid TAB formats:\n";
		for (auto &f : tabFormats)
		{
			std::cout << "\t" << f.first << "\n";
		}
		return EXIT_SUCCESS;
	}

	if (vm.count("list-palette-conversions"))
	{
		std::cout << "Valid palette conversion methods:\n";
		for (auto &f : paletteConversions)
		{
			std::cout << "\t" << f.first << "\n";
		}
		return EXIT_SUCCESS;
	}

	if (!vm.count("output"))
	{
		std::cerr << "Must supply \"output\"\n";
		return EXIT_FAILURE;
	}

	if (!vm.count("outtab"))
	{
		std::cerr << "Must supply \"outtab\"\n";
		return EXIT_FAILURE;
	}

	if (!vm.count("palette"))
	{
		std::cerr << "Must supply \"palette\"\n";
		return EXIT_FAILURE;
	}

	if (!vm.count("palette"))
	{
		std::cerr << "Must supply \"palette\"\n";
		return EXIT_FAILURE;
	}

	if (!vm.count("input"))
	{
		std::cerr << "No input files supplied\n";
		return EXIT_FAILURE;
	}

	if (vm.count("tabformat"))
	{
		auto tabFormatString = vm["tabformat"].as<std::string>();
		auto it = tabFormats.find(tabFormatString);
		if (it == tabFormats.end())
		{
			std::cerr << "Unknonw tab format: \"" << tabFormatString << "\"\n";
			return EXIT_FAILURE;
		}
		tabFormat = it->second;
	}
	if (vm.count("format"))
	{
		auto pckFormatString = vm["format"].as<std::string>();
		auto it = pckFormats.find(pckFormatString);
		if (it == pckFormats.end())
		{
			std::cerr << "Unknonw pck format: \"" << pckFormatString << "\"\n";
			return EXIT_FAILURE;
		}
		pckFormat = it->second;
	}
	if (vm.count("palette-conversion"))
	{
		auto palConversionString = vm["palette-conversion"].as<std::string>();
		auto it = paletteConversions.find(palConversionString);
		if (it == paletteConversions.end())
		{
			std::cerr << "Unknown palette conversion: \"" << palConversionString << "\"\n";
			return EXIT_FAILURE;
		}
		paletteConversion = it->second;
	}

	if (vm.count("verbose"))
		verbose = true;

	std::ifstream paletteFile(vm["palette"].as<std::string>());
	if (!paletteFile)
	{
		std::cerr << "Failed to open palette file \"" << vm["palette"].as<std::string>() << "\"\n";
		return EXIT_FAILURE;
	}

	Palette pal;

	try
	{
		pal = loadPalette(paletteFile, expandPalette);
	}
	catch (std::runtime_error &e)
	{
		std::cerr << "Failed to read palette file \"" << vm["palette"].as<std::string>() << "\"\n";
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}

	std::ofstream outPCKFile(vm["output"].as<std::string>());
	if (!outPCKFile)
	{
		std::cerr << "Failed to open output file \"" << vm["output"].as<std::string>() << "\"\n";
		return EXIT_FAILURE;
	}

	std::ofstream outTABFile(vm["outtab"].as<std::string>());
	if (!outTABFile)
	{
		std::cerr << "Failed to open output TAB file \"" << vm["outtab"].as<std::string>()
		          << "\"\n";
		return EXIT_FAILURE;
	}

	auto inputFiles = vm["input"].as<std::vector<std::string>>();
	int index = 0;
	size_t offset = 0;

	for (auto &inputFileName : inputFiles)
	{
		std::ifstream inFile(inputFileName);
		if (!inFile)
		{
			std::cerr << "Failed to open input file \"" << inputFileName << "\"\n";
			return EXIT_FAILURE;
		}

		inFile.seekg(0, inFile.end);
		size_t fileLength = inFile.tellg();
		inFile.seekg(0, inFile.beg);

		try
		{
			auto img = loadPNG(inFile, fileLength);
			auto paletteImage = img.toPaletteImage(pal, paletteConversion);
			if (verbose)
				std::cout << index << "\t" << inputFileName << "\n";

			std::stringstream pckImageStream;
			auto pckSize = writePCK(paletteImage, pckImageStream, pckFormat);
			auto startOffset = writeTABEntry(outTABFile, tabFormat, offset);
			assert(startOffset <= offset);
			// Pad with NULLs up to the aligned start offset
			while (startOffset < offset)
			{
				outPCKFile.write("\0", 1);
				startOffset++;
			}
			assert(pckImageStream.str().size() == pckSize);
			outPCKFile.write(pckImageStream.str().c_str(), pckSize);
			offset = startOffset + pckSize;
			index++;
		}
		catch (std::runtime_error &e)
		{
			std::cerr << "Failed to process input file \"" << inputFileName << "\"\n";
			std::cerr << e.what() << "\n";
			return EXIT_FAILURE;
		}

		index++;
	}

	return EXIT_SUCCESS;
}
