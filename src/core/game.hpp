#pragma once

#include "../io/input.hpp"
#include "player.hpp"
#include "../maps/map.hpp"

class Game {
public:
    Game();

    void start();
private:
    Window window;
    Input input;

    Texture texture;
    Renderer renderer;

    Map map;
    Player player;
    TextureLayer vignette_layer;

    void update(f32 delta_time);
};
