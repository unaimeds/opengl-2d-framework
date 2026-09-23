# OpenGL 2D Framework

A small 2D game framework written in C++ using OpenGL 4.6.

## Features

- Batched sprite and text rendering
- OpenGL texture arrays and GLSL shaders
- FreeType font rendering
- Spritesheet animation
- TMX tile map loading
- Keyboard input with GLFW

## Building

> [!IMPORTANT]  
> C++23-compatible compiler is required.

```
git clone --recurse-submodules https://github.com/unaimeds/opengl-2d-framework.git
cd opengl-2d-framework
```
```
cmake -S . -B build
cmake --build build
```
```
cd build
./game_remake
```

## License

Project is licensed under GPL-3.0
