#include "game.hpp"

#include "../utilities/debug.hpp"

Game::Game() :
    window("OpenGL 2D Framework", { 1280, 720 }),
    input(window),

    texture(),
    renderer(window.get_size(), texture),

    map(texture),
    player(texture),
    vignette_layer(texture.add_layer("vignette.png")) {

    renderer.set_clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
    renderer.set_line_width(2.0f);
}

void Game::start() {
    debug.info("Starting game");
    texture.create_texture();
    texture.bind(0);

    auto previous_time = std::chrono::high_resolution_clock::now();
    while (window.is_open()) {
        auto current_time = std::chrono::high_resolution_clock::now();
        auto delta_time = std::chrono::duration<float>(current_time - previous_time).count();
        previous_time = current_time;

        update(delta_time);
    }
}

void Game::update(float delta_time) {
    player.update(renderer, input, map, delta_time);
    map.render(renderer);
    renderer.draw_text({ 50.0f, 150.0f }, "test halo");
//    renderer.draw_quad({ 0.0f, 0.0f }, window.get_size(), vignette_layer.index, );
//    renderer.draw_quad_lines({ 150.0f, 150.0f }, { 100.0f, 100.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });
    renderer.render();
}

int main() {
    debug.info("Start of logging");
    auto game = Game();
    game.start();
    debug.info("End of logging");
}
