#pragma once

#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/mat4x2.hpp>

class TextureLayer {
public:
    TextureLayer(
        glm::ivec2& parent_size, int index, std::uint8_t* data,
        glm::ivec2 size, std::uint8_t channels, int sprite_size = 32
    );

    glm::mat4x2 get_uv(std::uint32_t row, std::uint32_t column) const;

    int index;
    std::uint8_t channels;
    glm::ivec2 size;
    glm::ivec2& parent_size; // maximum size of a parent texture ("Texture" class)
    int sprite_size;
    std::uint8_t* data;
    bool custom_data;
    bool created = false;
};

class Texture {
public:
    Texture();
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    const TextureLayer& add_layer(const std::string& file, int sprite_size = 32, bool absolute_path = false);
    const TextureLayer& add_layer(std::uint8_t* data, const glm::ivec2& layer_size, std::uint8_t channels);
    void create_texture();
    void bind(std::uint32_t unit) const;

    std::uint32_t id;
    glm::ivec2 size;
    std::vector<std::unique_ptr<TextureLayer>> layers;
};
