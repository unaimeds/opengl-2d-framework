#pragma once

#include "../graphics/window.hpp"

enum class Key {
    W = GLFW_KEY_W,
    A = GLFW_KEY_A,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,
};

class Input {
public:
    Input(cref<Window> window);

    bool is_key_pressed(Key key);
private:
    std::unordered_map<i32, bool> key_states;

    void key_callback(i32 key, i32 action);
};
