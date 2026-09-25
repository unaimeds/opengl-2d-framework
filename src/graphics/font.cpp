#include <stdexcept>
#include <algorithm>

#include "font.hpp"
#include "../utilities/debug.hpp"
#include "../io/resources.hpp"

Font::Font(Texture& texture, std::string_view file_name) : max_ascent(0) {
    auto path = resources::get_resource_path(file_name);

    if (FT_Init_FreeType(&library)) {
        debug.error("Failed to initialize FreeType library");
        throw std::runtime_error("Failed to initialize FreeType library");
    }
    if (FT_New_Face(library, path.c_str(), 0, &face)) {
        debug.error("Failed to load font: {}", file_name);
        throw std::runtime_error(std::format("Failed to load font: {}", file_name));
    }
    FT_Set_Pixel_Sizes(face, 0, 24);

    // calculate texture size
    glm::ivec2 texture_size(0);
    glm::ivec2 offset(0);
    for (std::uint8_t i = LOWEST_CHAR; i < HIGHEST_CHAR; i++) {
        FT_Load_Char(face, i, FT_LOAD_NO_BITMAP | FT_LOAD_NO_HINTING);
        auto& bmp = face->glyph->bitmap;

        auto height = face->size->metrics.height >> 6;
        if (offset.x + bmp.width >= MAX_TEXTURE_WIDTH) {
            offset.x = 0;
            offset.y += height;
        }

        glyphs.emplace(i, Glyph{ offset, { bmp.width, bmp.rows }, { face->glyph->bitmap_left, face->glyph->bitmap_top }, (std::uint32_t)face->glyph->advance.x >> 6 });

        offset.x += bmp.width;
        texture_size.x = std::max(texture_size.x, offset.x);
        texture_size.y = std::max(texture_size.y, offset.y + (int)height);

        if (face->glyph->bitmap_top > max_ascent && i != '_') {
            max_ascent = face->glyph->bitmap_top;
        }
    }

    // load glyphs into buffer
    std::uint8_t* pixels = new std::uint8_t[texture_size.x * texture_size.y];
    for (auto& [ch, glyph] : glyphs) {
        FT_Load_Char(face, ch, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
        auto& bmp = face->glyph->bitmap;

        for (int row = 0; row < bmp.rows; ++row) {
            std::memcpy(&pixels[(glyph.offset.y + row) * texture_size.x + glyph.offset.x], &bmp.buffer[row * bmp.pitch], bmp.width);
        }
    }

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    // create texture using buffer
    layer = std::make_shared<TextureLayer>(texture.add_layer(pixels, texture_size, 1));
}

const Glyph& Font::get_glyph(std::uint8_t character) const {
    if (!glyphs.contains(character)) {
        debug.error("Character {} not found in font", character);
        throw std::runtime_error(std::format("Character {} not found in font", character));
    }
    return glyphs.at(character);
}
