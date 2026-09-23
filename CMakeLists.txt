cmake_minimum_required(VERSION 3.25)
project(opengl_2d_framework)

set(CMAKE_CXX_STANDARD 23)

add_executable(opengl_2d_framework src/graphics/window.cpp src/graphics/renderer.cpp src/graphics/shader.cpp src/graphics/shader.cpp src/io/resources.cpp src/graphics/textures.cpp src/utilities/debug.cpp src/core/game.cpp src/core/player.cpp src/io/input.cpp src/graphics/animation.cpp src/maps/map.cpp src/graphics/font.cpp src/graphics/font.hpp)

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)
target_link_libraries(opengl_2d_framework glfw)

# glad
add_subdirectory(lib/glad)
target_link_libraries(opengl_2d_framework glad)

# glm
add_subdirectory(lib/glm)
target_link_libraries(opengl_2d_framework glm::glm)

# stb
add_subdirectory(lib/stb)
target_link_libraries(opengl_2d_framework stb)

# spdlog
add_subdirectory(lib/spdlog)
target_link_libraries(opengl_2d_framework spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# tmxlite
add_subdirectory(lib/tmxlite/tmxlite)
SET(TMXLITE_STATIC_LIB ON CACHE BOOL "" FORCE)
target_link_libraries(opengl_2d_framework tmxlite)

# freetype
add_subdirectory(lib/freetype)
target_link_libraries(opengl_2d_framework freetype)

# defines
add_compile_definitions(GLFW_INCLUDE_NONE)

# pch
target_precompile_headers(opengl_2d_framework PRIVATE src/utilities/defines.hpp <string_view>)

# copy resources to build directory
add_custom_target(copy_res COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)
add_dependencies(opengl_2d_framework copy_res)
