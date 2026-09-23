#pragma once

#include <vector>
#include <array>

#include <glm/vec2.hpp>
#include <glm/mat4x2.hpp>

class TextureLayer {
public:
    TextureLayer(glm::ivec2& parent_size, i32 index, u8* data, glm::ivec2 size, u8 channels, i32 sprite_size = 32);

    glm::mat4x2 get_uv(u32 row, u32 column) const;

    i32 index;
    u8 channels;
    glm::ivec2 size;
    glm::ivec2& parent_size; // maximum size of a parent texture ("Texture" class)
    i32 sprite_size;
    u8* data;
    bool custom_data;
};

class Texture {
public:
    Texture();
    ~Texture();

    cref<TextureLayer> add_layer(const std::string& file, i32 sprite_size = 32, bool absolute_path = false);
    cref<TextureLayer> add_layer(u8* data, const glm::ivec2& layer_size, u8 channels);
    void create_texture();
    void bind(u32 unit) const;

    u32 id;
    glm::ivec2 size;
    std::vector<TextureLayer> layers;
};
