cmake_minimum_required(VERSION 2.8)
project(PCKTools)

set (PCKLIB_SOURCE_FILES
	src/pcklib/colour.h
	src/pcklib/colour.cpp
	src/pcklib/image.h
	src/pcklib/image.cpp
	src/pcklib/pck.h
	src/pcklib/pck.cpp
	src/pcklib/tab.h
	src/pcklib/tab.cpp
	src/pcklib/png.h
	src/pcklib/png.cpp
	src/pcklib/palette.h
	src/pcklib/palette.cpp
	src/pcklib/lodepng/lodepng.cpp
	src/pcklib/lodepng/lodepng.h
)

add_library(pcklib STATIC ${PCKLIB_SOURCE_FILES})

set_property(TARGET pcklib PROPERTY CXX_STANDARD 14)
target_include_directories(pcklib PUBLIC "src/")

set(BOOST_PACKAGES program_options)

find_package(Boost REQUIRED COMPONENTS ${BOOST_PACKAGES})


add_executable(png2pck src/png2pck.cpp)
set_property(TARGET png2pck PROPERTY CXX_STANDARD 14)
target_link_libraries(png2pck PUBLIC pcklib ${Boost_LIBRARIES})
target_include_directories(png2pck PUBLIC ${Boost_INCLUDE_DIRS})
