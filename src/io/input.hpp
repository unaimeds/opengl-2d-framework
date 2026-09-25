#pragma once

#include <unordered_map>

#include "../graphics/window.hpp"

enum class Key {
    W = GLFW_KEY_W,
    A = GLFW_KEY_A,
    S = GLFW_KEY_S,
    D = GLFW_KEY_D,
};

class Input {
public:
    Input(const Window& window);

    bool is_key_pressed(Key key);
private:
    std::unordered_map<int, bool> key_states;

    void key_callback(int key, int action);
};
