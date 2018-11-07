cmake_minimum_required(VERSION 3.5.2)
project(ios_minicap)

FIND_PACKAGE(ImageMagick
  REQUIRED
  COMPONENTS MagickWand
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -g -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16")

set(SOURCE_FILES
        src/minicap.cpp
        src/SimpleServer.cpp src/SimpleServer.hpp
        src/FrameListener.cpp src/FrameListener.hpp
        src/Banner.cpp src/Banner.hpp
        src/JpegEncoder.cpp src/JpegEncoder.hpp
        src/StreamClient.mm src/StreamClient.h
        src/Frame.hpp)

add_executable(ios_minicap ${SOURCE_FILES})

include_directories(${ImageMagick_INCLUDE_DIRS})

target_link_libraries (ios_minicap
        "-framework Foundation"
        "-framework CoreFoundation"
        "-framework CoreMedia"
        "-framework CoreVideo"
        "-framework CoreMediaIO"
        "-framework AVFoundation"
        ${ImageMagick_MagickWand_LIBRARY}
        )
