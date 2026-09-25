#pragma once

#include <tmxlite/Map.hpp>

#include "../graphics/renderer.hpp"

struct Tile {
    glm::vec2 position;
    glm::vec2 size;
    glm::mat4x2 uv;
    std::uint32_t layer_index;
};

struct TileTexture {
    int index;
    glm::mat4x2 uv;
};

class MapLayer {
public:
    MapLayer(tmx::TileLayer& tile_layer, const glm::uvec2& tile_count, const std::unordered_map<std::uint32_t, TileTexture>& tile_textures);

    void render(Renderer& renderer);

    const std::vector<Tile>& get_tiles() const { return tiles; }
    bool is_collidable() const { return name == "collision"; }
private:
    static constexpr auto TILE_SIZE = 48.0f;

    std::vector<Tile> tiles;
    std::string name;
};

class Map { // TODO: make map take sprite sheet file name as parameter
public:
    Map(Texture& texture);

    void render(Renderer& renderer);

    bool is_colliding(const glm::vec2& position, const glm::vec2& size) const;
private:
    std::vector<MapLayer> layers;
    std::vector<Tile> collision_rects;
};
