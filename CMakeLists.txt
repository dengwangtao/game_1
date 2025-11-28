cmake_minimum_required(VERSION 3.10.0)
project(SDLShooter VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_BUILD_TYPE Debug)


# 设置编译选项
if (MSVC)
    add_compile_options(/W4)
    add_compile_options(/utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-Wno-unused-but-set-variable)
    add_compile_options(-Wno-unused-parameter)
    add_compile_options(-Wno-reorder)
endif()


# 设置输出目录
set(BIN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUT_DIR})

set(SDL_VC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL-VC")
list(APPEND CMAKE_PREFIX_PATH ${SDL_VC_DIR})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)


# 拷贝dll到bin目录
if(MSVC)
    file(GLOB_RECURSE SDL_DLLS "${SDL_VC_DIR}/lib/x64/*.dll")
    file(COPY ${SDL_DLLS} DESTINATION ${BIN_OUTPUT_DIR})
endif()


add_subdirectory(src)