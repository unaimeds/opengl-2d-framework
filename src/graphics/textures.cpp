#include <stb/stb_image.h>
#include <glad/glad.h>

#include "textures.hpp"
#include "../utilities/debug.hpp"
#include "../io/resources.hpp"

TextureLayer::TextureLayer(glm::ivec2& parent_size, i32 index, u8* data, glm::ivec2 size, u8 channels, i32 sprite_size) : parent_size(parent_size), index(index), data(data), size(size), channels(channels), sprite_size(sprite_size), custom_data(true) {
    debug.info("Loaded texture with custom data at index {} ({}x{}, {} channels)", index, size.x, size.y, channels);
}

glm::mat4x2 TextureLayer::get_uv(u32 row, u32 column) const {
    glm::vec2 min = { (column * sprite_size) / (f32)parent_size.x, (row * sprite_size) / (f32)parent_size.y };
    glm::vec2 max = { ((column + 1) * sprite_size) / (f32)parent_size.x, ((row + 1) * sprite_size) / (f32)parent_size.y };

    glm::mat4x2 uv = {
        { min.x, min.y },
        { max.x, min.y },
        { max.x, max.y },
        { min.x, max.y }
    };

    return uv;
}

//SubTexture TextureLayer::get_sub_texture(i32 row, i32 column) const {
//    glm::vec2 min = { (column * sprite_size) / (f32)parent_size.x, (row * sprite_size) / (f32)parent_size.y };
//    glm::vec2 max = { ((column + 1) * sprite_size) / (f32)parent_size.x, ((row + 1) * sprite_size) / (f32)parent_size.y };
//
//    glm::vec2 uv[] = {
//        { min.x, min.y },
//        { max.x, min.y },
//        { max.x, max.y },
//        { min.x, max.y }
//    };
//
//    return { index, uv };
//}

Texture::Texture() {
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &id);

    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

cref<TextureLayer> Texture::add_layer(const std::string& file, i32 sprite_size, bool absolute_path) {
    auto path = absolute_path ? file : resources::get_resource_path(file);

    glm::ivec2 layer_size;
    i32 channels;
    auto data = stbi_load(path.data(), &layer_size.x, &layer_size.y, &channels, 0);
    if (data == nullptr) {
        debug.error("Failed to load texture: {}", path);
        throw std::runtime_error(std::format("Failed to load texture: {}", path));
    }

    if (size.x < layer_size.x)
        size.x = layer_size.x;
    if (size.y < layer_size.y)
        size.y = layer_size.y;

    auto index = (i32)layers.size();
    auto& layer = layers.emplace_back(size, index, data, layer_size, channels, sprite_size);
    layer.custom_data = false;
    return layer;
}

const TextureLayer &Texture::add_layer(u8* data, const glm::ivec2& layer_size, u8 channels) {
    if (size.x < layer_size.x)
        size.x = layer_size.x;
    if (size.y < layer_size.y)
        size.y = layer_size.y;

    auto index = (i32)layers.size();
    auto& layer = layers.emplace_back(size, index, data, layer_size, channels); // currently supporting 1 channel using custom data
    return layer;
}

void Texture::create_texture() {
    if (layers.empty()) {
        debug.warn("Tried to create texture array but no textures were loaded");
        return;
    }

    debug.info("Creating texture array: {}x{} with {} textures", size.x, size.y, layers.size());
    glTextureStorage3D(id, 1, GL_RGBA8, size.x, size.y, layers.size());

    for (auto& layer : layers) {
        auto channels = GL_RGBA;
        if (layer.channels == 1)
            channels = GL_RED;
        else if (layer.channels == 3)
            channels = GL_RGB;

        glTextureSubImage3D(id, 0, 0, 0, layer.index, layer.size.x, layer.size.y, 1, channels, GL_UNSIGNED_BYTE, layer.data);

        if (layer.custom_data)
            delete[] layer.data;
        else
            stbi_image_free(layer.data);
    }
}

void Texture::bind(u32 unit) const {
    glBindTextureUnit(unit, id);
}
