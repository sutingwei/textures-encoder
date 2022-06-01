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

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DBX_CONFIG_DEBUG -D__STDC_LIMIT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_CONSTANT_MACROS -DUSE_STB_IMAGE_LOAD")

FILE(GLOB bxSrc 3rd/bx/src/*.cpp)
list(FILTER bxSrc EXCLUDE REGEX amalgamated)

add_executable(textures_encoder textures_encoder.cpp ${bxSrc})

if (MSVC)
    set(bxCompatSrc ./3rd/bx/include/compat/msvc)
elseif(${MACOSX})
    set(bxCompatSrc ./3rd/bx/include/compat/osx)
endif()

target_include_directories(textures_encoder PUBLIC
        ./
        ./3rd/bx/include
        ${bxCompatSrc}
        ./3rd/bx/3rdparty)

if (${MACOSX})
    target_link_libraries(textures_encoder PUBLIC "-framework CoreFoundation" "-framework Foundation")
endif()

# copy bin
if (MSVC)
add_custom_target(
        copy3rdTarget
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bin/win/astcenc-native.exe ${CMAKE_CURRENT_BINARY_DIR}/astcenc-native.exe
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bin/win/EtcTool.exe ${CMAKE_CURRENT_BINARY_DIR}/EtcTool.exe
)
elseif(${MACOSX})
    add_custom_target(
            copy3rdTarget
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bin/osx/astcenc-native ${CMAKE_CURRENT_BINARY_DIR}/astcenc-native
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/bin/osx/EtcTool ${CMAKE_CURRENT_BINARY_DIR}/EtcTool
    )
endif()

add_dependencies(textures_encoder copy3rdTarget)