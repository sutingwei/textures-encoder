# CMake configuration
cmake_minimum_required(VERSION 3.15)

project(texturesencoder VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)

add_subdirectory(3rd/astc-encoder)
add_subdirectory(3rd/etc2comp)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /utf-8")   # msvc fix __cplusplus
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBX_CONFIG_DEBUG -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DUSE_STB_IMAGE_LOAD")

FILE(GLOB bxSrc 3rd/bx/src/*.cpp)
list(FILTER bxSrc EXCLUDE REGEX amalgamated)

FILE(GLOB etcToolSrc 3rd/etc2comp/EtcTool/*.cpp)
#list(FILTER etcToolSrc EXCLUDE REGEX EtcTool/EtcTool)
FILE(GLOB lodepngSrc 3rd/etc2comp/third_party/lodepng/*.cpp)

add_executable(textures_encoder textures_encoder.cpp ${bxSrc} ${etcToolSrc} ${lodepngSrc})

target_include_directories(textures_encoder PUBLIC
        ./
        ./3rd/bx/include
        ./3rd/bx/include/compat/msvc
        ./3rd/bx/3rdparty
        ./3rd/etc2comp/EtcTool
        ./3rd/etc2comp/EtcLib/Etc
        ./3rd/etc2comp/EtcLib/EtcCodec
        ./3rd/etc2comp/third_party/lodepng)

target_link_libraries(textures_encoder PUBLIC EtcLib)