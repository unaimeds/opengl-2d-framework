#include <tmxlite/TileLayer.hpp>

#include "map.hpp"
#include "../io/resources.hpp"
#include "../utilities/debug.hpp"

MapLayer::MapLayer(tmx::TileLayer& tile_layer, const glm::uvec2& tile_count, const std::unordered_map<std::uint32_t, TileTexture>& tile_textures) : name(tile_layer.getName()) {
    debug.info("Loading map layer: {}", name);

    auto& tile_ids = tile_layer.getTiles();

    for (std::uint32_t y = 0; y < tile_count.y; ++y) {
        for (std::uint32_t x = 0; x < tile_count.x; ++x) {
            auto current_tile = y * tile_count.x + x;
            auto tile_id = tile_ids[current_tile].ID - 1;

            if (!tile_textures.contains(tile_id))
                continue;

            glm::vec2 position = { TILE_SIZE * x - 1.5f, TILE_SIZE * y + 5.0f };
            glm::vec2 size = { TILE_SIZE, TILE_SIZE };

            auto& tile = tile_textures.at(tile_id);
            tiles.emplace_back(position, size, tile.uv, tile.index);
        }
    }
}

void MapLayer::render(Renderer& renderer) {
    for (auto& tile : tiles) {
        renderer.draw_quad(tile.position, tile.size, tile.layer_index, tile.uv);
    }
}

Map::Map(Texture& texture) {
    const auto path = resources::get_resource_path("untitled.tmx");

    debug.info("Loading map: {}", path);
    tmx::Map map;
    map.load(path);

    glm::uvec2 tile_count = { map.getTileCount().x, map.getTileCount().y };

    std::vector<TextureLayer> tileset_textures;
    std::unordered_map<std::uint32_t, TileTexture> tile_textures;
    for (auto& tileset : map.getTilesets()) {
        auto& tileset_texture = tileset_textures.emplace_back(texture.add_layer(tileset.getImagePath(), 32, true));
        debug.info("Loaded tileset: {}, layer index: {}", tileset.getName(), tileset_texture.index);

        for (auto& tile : tileset.getTiles()) {
            auto row = tile.ID / tileset.getColumnCount();
            auto column = tile.ID % tileset.getColumnCount();
            tile_textures.emplace(tile.ID, TileTexture{ tileset_texture.index, tileset_texture.get_uv(row, column) });
        }
    }

    for (auto& layer : map.getLayers()) {
        if (layer->getType() != tmx::Layer::Type::Tile)
            continue;

        auto& tile_layer = layer->getLayerAs<tmx::TileLayer>();
        auto& layer_class = layers.emplace_back(tile_layer, tile_count, tile_textures);
        if (layer_class.is_collidable()) {
            debug.info("Found collision layer");
            collision_rects = layer_class.get_tiles();
        }
    }
}

void Map::render(Renderer& renderer) {
    for (auto& layer : layers) {
        layer.render(renderer);
    }
}

bool Map::is_colliding(const glm::vec2& position, const glm::vec2& size) const {
    return std::any_of(collision_rects.begin(), collision_rects.end(), [&position, &size](const Tile& tile) {
        return false; // TODO: AABB
//        return rect.intersects({ position, size });
    });
}
