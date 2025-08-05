

message("构建 ${PROJECT_NAME}... CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


# 设置目标名称
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 扫描所有cpp
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS *.cpp *.h)

add_executable(${TARGET} ${SRCS})


if (MSVC)
    target_compile_options(${TARGET} PRIVATE /wd4100 /wd4244 /wd4267)
endif()


target_link_libraries(
    ${TARGET}
        ${SDL2_LIBRARIES} # 在sdl2-config.cmake中定义的变量
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
)