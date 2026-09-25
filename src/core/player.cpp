#include "player.hpp"

Player::Player(Texture& texture) :
    position(100.0f, 50.0f), size(48.0f, 48.0f),
    sprite_sheet(texture.add_layer("character.png", 48)),
    idle_animation(sprite_sheet, 0.25f, 0, { 0, 1, 2, 3 })
{}

void Player::update(Renderer& renderer, Input& input, const Map&map, float delta_time) {
    idle_animation.update(delta_time);

    // get direction
    glm::vec2 direction(0);
    if (input.is_key_pressed(Key::W))
        direction.y -= 1.0f;
    if (input.is_key_pressed(Key::S))
        direction.y += 1.0f;
    if (input.is_key_pressed(Key::A))
        direction.x -= 1.0f;
    if (input.is_key_pressed(Key::D))
        direction.x += 1.0f;

    // cap diagonal speed
    if (glm::length(direction) > 1.0f)
        direction = glm::normalize(direction);

    // check for map collisions
    glm::vec2 velocity = { direction.x * SPEED * delta_time, direction.y * SPEED * delta_time };
    if (map.is_colliding({ position.x + velocity.x, position.y }, size)) {
        velocity.x = 0.0f;
    }
    position.x += velocity.x;

    if (map.is_colliding({ position.x, position.y + velocity.y }, size)) {
        velocity.y = 0.0f;
    }
    position.y += velocity.y;

    renderer.draw_quad(position, size, sprite_sheet.index, idle_animation.get_current_uv());
}
