cmake_minimum_required(VERSION 3.25)

project(opengl_2d_framework VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)


# Dependencies configuration
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

SET(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)


# Dependencies
add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)
add_subdirectory(lib/glm)
add_subdirectory(lib/stb)
add_subdirectory(lib/spdlog)
add_subdirectory(lib/tmxlite/tmxlite)
add_subdirectory(lib/freetype)


add_executable(
    opengl_2d_framework

    src/graphics/window.cpp src/graphics/renderer.cpp src/graphics/shader.cpp
    src/graphics/shader.cpp src/graphics/textures.cpp
    src/core/game.cpp src/core/player.cpp
    src/io/input.cpp src/io/resources.cpp
    src/graphics/animation.cpp src/graphics/font.cpp src/graphics/font.hpp
    src/maps/map.cpp
    src/utilities/debug.cpp
)


# Compiler config
add_compile_definitions(GLFW_INCLUDE_NONE GLM_ENABLE_EXPERIMENTAL)

target_precompile_headers(
    opengl_2d_framework PRIVATE
    src/utilities/defines.hpp <string_view>
)

target_link_libraries(
    opengl_2d_framework PRIVATE
    glfw glad glm::glm stb spdlog::spdlog tmxlite freetype
)


# Copy resources after building
add_custom_target(
    copy_res COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
)
add_dependencies(opengl_2d_framework copy_res)
