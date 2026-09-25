#include "input.hpp"

#include "../utilities/debug.hpp"

Input::Input(const Window& window) {
    debug.info("Initializing input");

    window.set_user_pointer(this);
    window.set_key_callback([](GLFWwindow* window, int key, int scan_code, int action, int mods) {
        auto instance = reinterpret_cast<Input*>(glfwGetWindowUserPointer(window));
        instance->key_callback(key, action);
    });
}

bool Input::is_key_pressed(Key key) {
    return key_states[static_cast<int>(key)];
}

void Input::key_callback(int key, int action) {
    key_states[key] = action != GLFW_RELEASE;
}
