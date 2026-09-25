#pragma once

#include "../graphics/renderer.hpp"
#include "../graphics/animation.hpp"
#include "../io/input.hpp"
#include "../maps/map.hpp"

class Player {
public:
    Player(Texture& texture);

    void update(Renderer& renderer, Input& input, const Map& map, float delta_time);
private:
    static constexpr auto SPEED = 200.0f;

    const TextureLayer& sprite_sheet;
    glm::vec2 position;
    glm::vec2 size;
    Animation idle_animation;
};
