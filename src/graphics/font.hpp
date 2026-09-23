#pragma once

#include <unordered_map>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "textures.hpp"

struct Glyph {
    glm::ivec2 offset;
    glm::ivec2 size;
    glm::ivec2 bearing;
    u32 x_advance;
};

class Font {
public:
    Font(Texture& texture, std::string_view file_name);

    const Glyph& get_glyph(u8 character) const;
    const std::shared_ptr<TextureLayer>& get_texture() const { return layer; }
    u32 get_max_ascent() const { return max_ascent; }
private:
    static constexpr auto MAX_TEXTURE_WIDTH = 512;
    static constexpr auto LOWEST_CHAR = 32;
    static constexpr auto HIGHEST_CHAR = 128;

    FT_Library library;
    FT_Face face;

    std::unordered_map<u8, Glyph> glyphs;
    std::shared_ptr<TextureLayer> layer;
    u32 max_ascent;
};
