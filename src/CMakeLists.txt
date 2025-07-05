

message("构建 ${PROJECT_NAME}... CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


# 设置目标名称
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

add_executable(${TARGET} hello.cpp)



target_link_libraries(
    ${TARGET}
        ${SDL2_LIBRARIES} # 在sdl2-config.cmake中定义的变量
)