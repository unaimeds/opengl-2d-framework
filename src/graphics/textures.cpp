#include "textures.hpp"

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <stdexcept>

#include "../utilities/debug.hpp"
#include "../io/resources.hpp"

TextureLayer::TextureLayer(
    glm::ivec2& parent_size, int index, std::uint8_t* data,
    glm::ivec2 size, std::uint8_t channels, int sprite_size
) :
    parent_size(parent_size), index(index), data(data), size(size),
    channels(channels), sprite_size(sprite_size), custom_data(true)
{
    debug.info(
        "Loaded texture with custom data at index {} ({}x{}, {} channels)",
        index, size.x, size.y, channels
    );
}

glm::mat4x2 TextureLayer::get_uv(std::uint32_t row, std::uint32_t column) const {
    glm::vec2 min = { (column * sprite_size) / (float)parent_size.x, (row * sprite_size) / (float)parent_size.y };
    glm::vec2 max = { ((column + 1) * sprite_size) / (float)parent_size.x, ((row + 1) * sprite_size) / (float)parent_size.y };

    glm::mat4x2 uv = {
        { min.x, min.y },
        { max.x, min.y },
        { max.x, max.y },
        { min.x, max.y }
    };

    return uv;
}

//SubTexture TextureLayer::get_sub_texture(int row, int column) const {
//    glm::vec2 min = { (column * sprite_size) / (float)parent_size.x, (row * sprite_size) / (float)parent_size.y };
//    glm::vec2 max = { ((column + 1) * sprite_size) / (float)parent_size.x, ((row + 1) * sprite_size) / (float)parent_size.y };
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

const TextureLayer& Texture::add_layer(const std::string& file, int sprite_size, bool absolute_path) {
    auto path = absolute_path ? file : resources::get_resource_path(file);

    glm::ivec2 layer_size;
    int channels;
    auto data = stbi_load(path.data(), &layer_size.x, &layer_size.y, &channels, STBI_rgb_alpha);

    if (data == nullptr)
        throw std::runtime_error(std::format("Failed to load texture: {}", path));

    if (size.x < layer_size.x)
        size.x = layer_size.x;
    if (size.y < layer_size.y)
        size.y = layer_size.y;

    int index = static_cast<int>(layers.size());
    auto layer = std::make_unique<TextureLayer>(size, index, data, layer_size, channels, sprite_size);
    const auto& result = *layer;

    layers.push_back(std::move(layer));
    return result;
}

const TextureLayer& Texture::add_layer(std::uint8_t* data, const glm::ivec2& layer_size, std::uint8_t channels) {
    if (size.x < layer_size.x)
        size.x = layer_size.x;
    if (size.y < layer_size.y)
        size.y = layer_size.y;

    int index = static_cast<int>(layers.size());

    // Only 1 channel is supported for custom data (for now).
    auto layer = std::make_unique<TextureLayer>(size, index, data, layer_size, channels);
    const auto& result = *layer;

    layers.push_back(std::move(layer));
    return result;
}

void Texture::create_texture() {
    if (layers.empty()) {
        debug.warn("Tried to create texture array but no textures were loaded");
        return;
    }

    debug.info("Creating texture array: {}x{} with {} textures", size.x, size.y, layers.size());
    glTextureStorage3D(id, 1, GL_RGBA8, size.x, size.y, layers.size());

    for (auto& layer : layers) {
        if (layer->created)
            throw std::logic_error("Texture layer already created");

        auto channels = GL_RGBA;
        if (layer->channels == 1)
            channels = GL_RED;
        else if (layer->channels == 3)
            channels = GL_RGB;

        glTextureSubImage3D(id, 0, 0, 0, layer->index, layer->size.x, layer->size.y, 1, channels, GL_UNSIGNED_BYTE, layer->data);

        if (layer->custom_data) {
            delete[] layer->data;
        } else {
            stbi_image_free(layer->data);
            layer->data = nullptr;
        }
    }
}

void Texture::bind(std::uint32_t unit) const {
    glBindTextureUnit(unit, id);
}
